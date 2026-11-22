// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Shared read-only YAML objects: bundled rapidyaml lookup, standard conversions.
#ifndef CUSTOM_YAML_READER_HPP
#define CUSTOM_YAML_READER_HPP

#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <ryml.hpp>

namespace custom_yaml {
class Error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

namespace detail {
[[noreturn]] inline void fail(const std::string& file, const std::string& field, const std::string& reason) {
	throw Error(file + ": " + field + ": " + reason);
}
inline std::string copy(c4::csubstr value) {
	return value.len ? std::string(value.str, value.len) : std::string{};
}
struct Storage {
	std::string path;
	ryml::Tree tree;
	size_t root = ryml::NONE;

	explicit Storage(const std::string& file) : path(file), tree(ryml::Callbacks(this, nullptr, nullptr, on_error)) {}
	Storage(const Storage&) = delete;
	Storage& operator=(const Storage&) = delete;
	Storage(Storage&&) = delete;
	Storage& operator=(Storage&&) = delete;
	static void on_error(const char* message, size_t length, ryml::Location location, void* user) {
		const auto& file = static_cast<const Storage*>(user)->path;
		if (length && message[length - 1] == '\0') --length;
		std::string reason(message, length);
		if (location.name == c4::csubstr(file.data(), file.size()))
			reason += " at line " + std::to_string(location.line + 1) + ", column " + std::to_string(location.col + 1);
		fail(file, "root", reason);
	}
	std::string context(size_t id) const {
		std::vector<std::string> parts;
		while (id != root && id != ryml::NONE) {
			const size_t parent = tree.parent(id);
			if (parent == ryml::NONE) break;
			parts.push_back(tree.is_map(parent) ? "." + copy(tree.key(id)) :
				"[" + std::to_string(tree.child_pos(parent, id)) + "]");
			id = parent;
		}
		std::string result = "root";
		for (auto part = parts.rbegin(); part != parts.rend(); ++part) result += *part;
		return result;
	}
};
template<class> inline constexpr bool unsupported = false;
} // namespace detail

class Node {
	std::shared_ptr<const detail::Storage> owner_;
	size_t id_;
	Node(std::shared_ptr<const detail::Storage> owner, size_t id) : owner_(std::move(owner)), id_(id) {}
	friend Node read_file(const std::string& path);

	[[noreturn]] void fail(const std::string& reason, std::string_view path = {}) const {
		std::string context = owner_->context(id_);
		if (!path.empty()) context += (path.front() == '[' ? "" : ".") + std::string(path);
		detail::fail(owner_->path, context, reason);
	}
	void require_container() const {
		if (!is_map() && !is_sequence()) fail("expected map or sequence");
	}
	size_t lookup(std::string_view path) const {
		if (path.empty()) return id_;
		if (path.find('\0') != std::string_view::npos) fail("embedded NUL in path", path);
		// c4core's index conversion can wrap: guard indices, then let ryml navigate.
		for (size_t pos = path.find('['); pos != std::string_view::npos; pos = path.find('[', pos + 1)) {
			const size_t end = path.find(']', pos + 1);
			if (end == std::string_view::npos) fail("unclosed path index", path);
			const auto digits = c4::csubstr(path.data() + pos + 1, end - pos - 1).trim(' ');
			size_t index{};
			if (digits.empty()) fail("empty path index", path);
			const auto parsed = std::from_chars(digits.str, digits.str + digits.len, index, 10);
			if (parsed.ec != std::errc{} || parsed.ptr != digits.str + digits.len || index == ryml::NONE)
				fail("invalid or overflowing path index", path);
		}
		try {
			return owner_->tree.lookup_path(c4::csubstr(path.data(), path.size()), id_).target;
		} catch (const Error& error) {
			fail(std::string("path lookup failed: ") + error.what(), path);
		}
	}
	template<class T> T number(c4::csubstr raw) const {
		T value{};
		const auto parsed = [&] {
			if constexpr (std::is_floating_point_v<T>)
				return std::from_chars(raw.str, raw.str + raw.len, value, std::chars_format::general);
			else
				return std::from_chars(raw.str, raw.str + raw.len, value, 10);
		}();
		if (parsed.ec != std::errc{} || parsed.ptr != raw.str + raw.len)
			fail("invalid number or value outside target range");
		if constexpr (std::is_floating_point_v<T>)
			if (!std::isfinite(value)) fail("expected finite floating-point value");
		return value;
	}
	void validate_plain_tree() const {
		std::vector<size_t> pending{id_};
		while (!pending.empty()) {
			const Node node(owner_, pending.back());
			pending.pop_back();
			const auto& tree = owner_->tree;
			const size_t id = node.id_;
			if (tree.has_key_tag(id) || tree.has_val_tag(id) || tree.has_key_anchor(id) ||
				tree.has_val_anchor(id) || tree.is_key_ref(id) || tree.is_val_ref(id))
				node.fail("tags, anchors and aliases are unsupported");
			if ((tree.has_key(id) && tree.key(id).find('\0') != c4::csubstr::npos) ||
				(tree.has_val(id) && tree.val(id).find('\0') != c4::csubstr::npos)) node.fail("embedded NUL");
			if (tree.is_stream(id)) node.fail("multiple YAML documents are unsupported");
			std::set<std::string_view> keys;
			for (size_t child = tree.first_child(id); child != ryml::NONE; child = tree.next_sibling(child)) {
				if (node.is_map()) {
					if (!tree.has_key(child)) node.fail("expected scalar mapping key");
					const auto key = tree.key(child);
					if (!keys.insert(key.len ? std::string_view(key.str, key.len) : std::string_view{}).second)
						Node(owner_, child).fail("duplicate YAML key");
				}
				pending.push_back(child);
			}
		}
	}

public:
	class iterator;
	Node at(std::string_view path) const {
		const size_t found = lookup(path);
		if (found == ryml::NONE) fail("missing field or path", path);
		return Node(owner_, found);
	}
	Node at(size_t index) const {
		if (!is_sequence()) fail("expected sequence for indexed access");
		const size_t found = owner_->tree.child(id_, index);
		if (found == ryml::NONE) fail("sequence index out of range", "[" + std::to_string(index) + "]");
		return Node(owner_, found);
	}
	bool contains(std::string_view path) const { return lookup(path) != ryml::NONE; }
	bool is_map() const { return owner_->tree.is_map(id_); }
	bool is_sequence() const { return owner_->tree.is_seq(id_); }
	bool is_null() const { return owner_->tree.has_val(id_) && owner_->tree.val_is_null(id_); }
	size_t size() const { require_container(); return owner_->tree.num_children(id_); }
	std::string key() const {
		if (!owner_->tree.has_key(id_)) fail("node has no mapping key");
		return detail::copy(owner_->tree.key(id_));
	}
	void require_fields(std::initializer_list<const char*> fields) const {
		if (!is_map()) fail("expected map");
		std::set<std::string_view> expected;
		for (const char* field : fields) {
			if (!field || !expected.insert(field).second) fail("null or duplicate required field name");
			if (owner_->tree.find_child(id_, c4::to_csubstr(field)) == ryml::NONE)
				fail("missing required field", field);
		}
		for (size_t child = owner_->tree.first_child(id_); child != ryml::NONE; child = owner_->tree.next_sibling(child)) {
			const auto name = owner_->tree.key(child);
			if (!expected.count(name.len ? std::string_view(name.str, name.len) : std::string_view{}))
				Node(owner_, child).fail("unknown field");
		}
	}
	template<class T> T get(std::string_view path = {}) const {
		const Node node = at(path);
		if (!node.owner_->tree.has_val(node.id_) || node.is_null()) node.fail("expected non-null scalar");
		const auto raw = node.owner_->tree.val(node.id_);
		if constexpr (std::is_same_v<T, std::string>) {
			return detail::copy(raw);
		} else {
			if (raw.empty()) node.fail("expected nonempty scalar for conversion");
			if constexpr (std::is_same_v<T, bool>) {
				if (raw.front() == 't' || raw.front() == 'T' || raw.front() == 'f' || raw.front() == 'F') {
					bool value{};
					if (!c4::from_chars(raw, &value)) node.fail("invalid boolean value");
					return value;
				}
				return node.number<std::int64_t>(raw) != 0;
			} else if constexpr ((std::is_integral_v<T> && sizeof(T) <= sizeof(std::uint64_t)) ||
				std::is_same_v<T, float> || std::is_same_v<T, double>) {
				return node.number<T>(raw);
			} else {
				static_assert(detail::unsupported<T>, "YAML get<T> supports string, bool, integers up to 64 bits, float and double; use at()/iteration for containers");
			}
		}
	}
	template<class T> T get_or(std::string_view path, T fallback) const {
		const size_t found = lookup(path);
		if (found == ryml::NONE) return fallback;
		const Node node(owner_, found);
		return node.is_null() ? fallback : node.get<T>();
	}
	iterator begin() const;
	iterator end() const;
};

class Node::iterator {
	std::shared_ptr<const detail::Storage> owner_;
	size_t id_ = ryml::NONE;
	friend class Node;
	iterator(std::shared_ptr<const detail::Storage> owner, size_t id) : owner_(std::move(owner)), id_(id) {}
public:
	using iterator_category = std::input_iterator_tag;
	using value_type = Node;
	using difference_type = std::ptrdiff_t;
	using pointer = void;
	using reference = Node;
	iterator() = default;
	Node operator*() const {
		if (!owner_) throw Error("YAML iterator: unbound iterator");
		if (id_ == ryml::NONE) detail::fail(owner_->path, "root", "cannot dereference end iterator");
		return Node(owner_, id_);
	}
	iterator& operator++() { id_ = owner_->tree.next_sibling(id_); return *this; }
	iterator operator++(int) { auto previous = *this; ++*this; return previous; }
	bool operator==(const iterator& other) const { return owner_ == other.owner_ && id_ == other.id_; }
	bool operator!=(const iterator& other) const { return !(*this == other); }
};

inline Node::iterator Node::begin() const {
	require_container();
	return iterator(owner_, owner_->tree.first_child(id_));
}
inline Node::iterator Node::end() const {
	require_container();
	return iterator(owner_, ryml::NONE);
}

inline Node read_file(const std::string& path) {
	try {
		auto storage = std::make_shared<detail::Storage>(path);
		std::ifstream input(path, std::ios::binary);
		if (!input) detail::fail(path, "root", "cannot open file");
		const std::string source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		if (input.bad()) detail::fail(path, "root", "cannot read file");
		if (source.find('\0') != std::string::npos) detail::fail(path, "root", "embedded NUL");
		ryml::Parser parser(storage->tree.callbacks());
		parser.parse_in_arena(c4::csubstr(path.data(), path.size()), c4::csubstr(source.data(), source.size()), &storage->tree);
		if (storage->tree.empty()) detail::fail(path, "root", "empty YAML document");
		storage->root = storage->tree.root_id();
		if (storage->tree.is_stream(storage->root)) {
			if (storage->tree.num_children(storage->root) != 1) detail::fail(path, "root", "multiple YAML documents are unsupported");
			storage->root = storage->tree.first_child(storage->root);
		}
		const auto& tree = storage->tree;
		if (!tree.is_map(storage->root) && !tree.is_seq(storage->root) &&
			(!tree.has_val(storage->root) || (!tree.val(storage->root).str && !tree.is_val_quoted(storage->root))))
			detail::fail(path, "root", "empty YAML document");
		const Node result(storage, storage->root);
		result.validate_plain_tree();
		return result;
	} catch (const Error&) { throw; }
	catch (const std::exception& error) { detail::fail(path, "root", error.what()); }
}
} // namespace custom_yaml
#endif
