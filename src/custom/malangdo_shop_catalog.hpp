// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Item catalogs own navigation and stock; native Barter owns NPCs and purchases.
#ifndef MALANGDO_SHOP_CATALOG_HPP
#define MALANGDO_SHOP_CATALOG_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <common/database.hpp>
#include <common/socket.hpp>
#include "../map/itemdb.hpp"
#include "../map/npc.hpp"
#include "../map/pc.hpp"
#include "../map/packets_struct.hpp"

namespace msc {
inline const std::array<const char*, 4> keys = {{"dewey", "remy", "lemy", "milo"}};
inline const std::array<std::vector<std::string>, 4> category_keys = {{
	{"top_ordinary", "top_special", "mid_ordinary", "mid_special", "low_ordinary", "low_special", "garment_ordinary", "garment_special", "garment_limited"},
	{"weapon", "armor", "shield", "shoes", "right_accessory", "left_accessory"},
	{"materials"},
	{"top", "middle", "lower", "garment", "dual", "effect"}
}};
inline const std::array<const char*, 6> milo_labels = {{
	"^4d4dffTop^000000 Stones", "^1C8008Middle^000000 Stones", "^ff0000Lower^000000 Stones",
	"^9905d8Garment^000000 Stones", "^8B4513Dual^000000 Stones", "^8B4513Effect^000000 Stones"
}};
struct Record {
	t_itemid id = 0;
	uint16 price = 0;
	std::string name, sort_name;
	std::vector<size_t> categories;
};
struct Node {
	std::string label, shop;
	bool cancel = false, auto_single = false;
	std::vector<std::string> children;
};
struct Catalog {
	bool published = false;
	int64 generation = 0;
	std::vector<Record> records;
	std::map<t_itemid, size_t> by_id;
	std::vector<size_t> search_order;
	std::string search_shop;
	bool search_limited = false;
	std::map<std::string, Node> nodes;
	std::map<std::string, std::weak_ptr<s_npc_barter>> owned;
};
using Rows = std::vector<Record>;
using Inventory = std::vector<const Record*>;
using Stock = std::map<std::string, std::shared_ptr<s_npc_barter>>;
inline std::map<std::string, Catalog> catalogs;
inline int64 generation = 0;
inline constexpr size_t search_page_size = 50;

inline void require(bool condition, const std::string& reason) {
	if (!condition) throw std::runtime_error(reason);
}
inline std::string path_for(const std::string& key) {
	return "npc/custom/malangdo_shop_catalog/" + key + ".yml";
}
inline void yaml_error(const char* message, size_t length, ryml::Location location, void*) {
	throw std::runtime_error(std::string(message, length) + " at line " +
		std::to_string(location.line + 1) + ", column " + std::to_string(location.col + 1));
}
inline void strict_tree(const ryml::NodeRef& node, std::string& context) {
	if (node.is_map() && node.has_child("Id") && node["Id"].has_val())
		context = "Id " + std::string(node["Id"].val().str, node["Id"].val().len);
	require(!node.has_key_tag() && !node.has_val_tag() && !node.is_key_ref() && !node.is_val_ref() &&
		!node.has_key_anchor() && !node.has_val_anchor(), "tags, anchors and aliases are unsupported");
	std::set<std::string> fields;
	for (const auto& child : node) {
		if (node.is_map()) {
			require(child.has_key(), "expected scalar field name");
			std::string field(child.key().str, child.key().len);
			require(fields.insert(field).second, "duplicate field " + field);
		}
		strict_tree(child, context);
	}
}
inline std::string scalar(const ryml::NodeRef& node, const char* field) {
	require(node.has_child(c4::to_csubstr(field)), std::string("missing field ") + field);
	auto value = node[c4::to_csubstr(field)];
	// Mapping fields are KEYVAL; is_val() accepts only unkeyed sequence scalars.
	require(value.is_keyval() && value.has_val() && value.val().find('\0') == c4::csubstr::npos,
		std::string("expected scalar ") + field);
	require(value.is_val_quoted() || (!value.val().empty() && value.val() != "null" && value.val() != "~"),
		std::string("null field ") + field);
	return std::string(value.val().str, value.val().len);
}
inline uint32 number(const ryml::NodeRef& node, const char* field, uint32 maximum) {
	std::string value = scalar(node, field);
	require(!node[c4::to_csubstr(field)].is_val_quoted() && !value.empty() && value[0] != '0',
		std::string(field) + " requires a positive unquoted decimal integer without leading zero");
	uint64 result = 0;
	for (char c : value) {
		require(c >= '0' && c <= '9', std::string("invalid integer ") + field);
		result = result * 10 + c - '0';
		require(result <= maximum, std::string(field) + " exceeds " + std::to_string(maximum));
	}
	return static_cast<uint32>(result);
}
inline std::string trim(std::string value) {
	const auto first = value.find_first_not_of(" \t\r\n");
	return first == std::string::npos ? "" : value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
}
inline std::string lower(std::string value) {
	for (char& c : value) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
	return value;
}
inline std::string sort_name(std::string value, bool costume) {
	value = trim(value);
	while (!value.empty() && value[0] == '[') {
		auto end = value.find(']');
		if (end == std::string::npos) break;
		value = trim(value.substr(end + 1));
	}
	if (costume && lower(value).compare(0, 7, "costume") == 0) {
		value = trim(value.substr(7));
		if (!value.empty() && value[0] == ':') value = trim(value.substr(1));
	}
	return value;
}
inline int letter(const std::string& name, size_t position = 0) {
	char c = position < name.size() ? name[position] : '\0';
	if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
	if (c >= 'A' && c <= 'Z') return c - 'A';
	return c >= '0' && c <= '9' ? 26 : 27;
}
inline std::string letter_label(int index) {
	return index < 26 ? std::string(1, 'A' + index) : index == 26 ? "0-9" : "Other";
}
inline Rows read_catalog(size_t profile, std::string& context) {
	const auto path = path_for(keys[profile]);
	std::ifstream input(path);
	require(static_cast<bool>(input), "cannot open file");
	std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	auto callbacks = ryml::get_callbacks();
	callbacks.m_error = yaml_error;
	ryml::Parser parser(callbacks);
	auto tree = parser.parse_in_arena(c4::to_csubstr(path), c4::to_csubstr(text));
	auto root = tree.rootref();
	strict_tree(root, context);
	context = "envelope";
	require(root.is_map() && root.num_children() == 2 && root.has_child("Header") && root.has_child("Body"),
		"requires Header and Body only; imports are unsupported");
	auto header = root["Header"];
	require(header.is_map() && header.num_children() == 2 && scalar(header, "Type") == "MALANGDO_SHOP_CATALOG" &&
		number(header, "Version", 1) == 1, "expected MALANGDO_SHOP_CATALOG Version 1 Header");
	auto body = root["Body"];
	require(body.is_seq() && body.num_children() > 0, "Body must be a nonempty sequence");
	require(item_db.find(7000) != nullptr, "currency 7000 is missing from item DB");
	Rows rows;
	std::set<t_itemid> ids;
	for (const auto& node : body) {
		context = "Body row " + std::to_string(rows.size());
		require(node.is_map() && node.num_children() == 4, "record requires Id, DisplayName, Categories and Price only");
		Record row;
		row.id = number(node, "Id", UINT32_MAX);
		context = "Id " + std::to_string(row.id);
		require(ids.insert(row.id).second, "duplicate Id");
		require(item_db.find(row.id) != nullptr, "Id does not exist in item DB");
		row.price = static_cast<uint16>(number(node, "Price", MAX_AMOUNT));
		row.name = scalar(node, "DisplayName");
		require(!trim(row.name).empty() && row.name.find_first_of("\r\n") == std::string::npos,
			"DisplayName must be nonempty without newline");
		if (!node["DisplayName"].is_val_quoted()) {
			auto name = lower(row.name);
			require(name != "true" && name != "false" && name != "yes" && name != "no" && name != "on" && name != "off" &&
				!std::all_of(name.begin(), name.end(), [](char c) { return (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '+'; }),
				"DisplayName must be a string; quote it");
		}
		row.sort_name = sort_name(row.name, profile == 0);
		require(!row.sort_name.empty(), "DisplayName has no sortable name after prefix removal");
		require(node.has_child("Categories"), "missing Categories");
		auto categories = node["Categories"];
		require(categories.is_seq() && categories.num_children() > 0 && (profile == 0 || categories.num_children() == 1),
			"Categories requires a nonempty sequence; only Dewey permits multiple categories");
		std::set<size_t> admitted;
		for (const auto& category : categories) {
			require(category.has_val() && category.is_val(), "expected category string");
			std::string name(category.val().str, category.val().len);
			auto found = std::find(category_keys[profile].begin(), category_keys[profile].end(), name);
			require(found != category_keys[profile].end(), "unknown category " + name);
			size_t index = found - category_keys[profile].begin();
			require(admitted.insert(index).second, "duplicate category " + name);
			row.categories.push_back(index);
		}
		rows.push_back(std::move(row));
	}
	return rows;
}
inline void add_node(Catalog& catalog, const std::string& id, const std::string& parent,
	const std::string& label, bool cancel = false, bool auto_single = false) {
	Node node;
	node.label = label;
	node.cancel = cancel;
	node.auto_single = auto_single;
	require(catalog.nodes.emplace(id, std::move(node)).second, "duplicate generated node " + id);
	if (!parent.empty()) catalog.nodes.at(parent).children.push_back(id);
}
inline void check_capacity(size_t count, const std::string& name) {
#if PACKETVER_MAIN_NUM >= 20190116 || PACKETVER_RE_NUM >= 20190116 || PACKETVER_ZERO_NUM >= 20181226
	const auto bytes = sizeof(PACKET_ZC_NPC_BARTER_MARKET_ITEMINFO) +
		count * sizeof(PACKET_ZC_NPC_BARTER_MARKET_ITEMINFO_sub);
	require(bytes <= INT16_MAX && bytes <= sizeof(packet_buffer) && count <= UINT16_MAX,
		"drawer exceeds native Barter packet capacity: " + name);
#else
	require(false, "client packet version does not support Barter");
#endif
}
inline std::shared_ptr<s_npc_barter> make_barter(const std::string& name, const Inventory& inventory) {
	check_capacity(inventory.size(), name);
	auto barter = std::make_shared<s_npc_barter>();
	barter->name = name;
	barter->m = -1;
	barter->x = barter->y = 0;
	barter->dir = 0;
	barter->sprite = JT_FAKENPC;
	barter->npcid = 0;
	for (const auto row : inventory) {
		auto item = std::make_shared<s_npc_barter_item>();
		item->index = static_cast<uint16>(barter->items.size());
		item->nameid = row->id;
		item->stockLimited = false;
		item->stock = item->price = 0;
		item->refine = 0;
		auto requirement = std::make_shared<s_npc_barter_requirement>();
		requirement->index = 0;
		requirement->nameid = 7000;
		requirement->amount = row->price;
		requirement->refine = -1;
		item->requirements[0] = requirement;
		barter->items[item->index] = item;
	}
	return barter;
}
inline void add_leaf(Catalog& catalog, Stock& stock, const std::string& id, const std::string& parent,
	const std::string& label, const std::string& name, const Inventory& inventory) {
	require(!inventory.empty(), "empty generated drawer " + name);
	add_node(catalog, id, parent, label);
	catalog.nodes.at(id).shop = name;
	auto barter = make_barter(name, inventory);
	require(stock.emplace(name, barter).second, "duplicate generated shop " + name);
	catalog.owned[name] = barter;
}
inline std::string menu_text(const Catalog& catalog, const Node& node) {
	std::string result;
	for (const auto& id : node.children) {
		if (!result.empty()) result += ":";
		result += catalog.nodes.at(id).label;
	}
	if (node.cancel) result += ":Cancel";
	return result;
}
inline void add_letters(Catalog& catalog, Stock& stock, const std::string& parent,
	const std::string& prefix, Inventory rows, bool costume) {
	std::sort(rows.begin(), rows.end(), [](const Record* a, const Record* b) {
		auto aname = lower(a->sort_name), bname = lower(b->sort_name);
		return aname == bname ? a->id < b->id : aname < bname;
	});
	std::map<int, Inventory> letters;
	for (auto row : rows) letters[letter(row->sort_name)].push_back(row);
	for (const auto& bucket : letters) {
		auto id = parent + "_l" + std::to_string(bucket.first);
		auto label = letter_label(bucket.first);
		if (!costume) {
			add_leaf(catalog, stock, id, parent, label, prefix + "_l" + std::to_string(bucket.first), bucket.second);
			continue;
		}
		add_node(catalog, id, parent, label, false, true);
		std::map<int, Inventory> groups;
		for (auto row : bucket.second) {
			int second = bucket.second.size() > 40 && bucket.first < 26 ? letter(row->sort_name, 1) : -1;
			groups[second >= 26 ? 27 : second].push_back(row);
		}
		for (const auto& group : groups) {
			auto suffix = "_l" + std::to_string(bucket.first) + "_g" + std::to_string(group.first + 1);
			auto group_label = group.first < 0 ? label : group.first == 27 ? label + "-Other" : label + letter_label(group.first);
			add_leaf(catalog, stock, id + "_g" + std::to_string(group.first + 1), id, group_label, prefix + suffix, group.second);
		}
	}
}
inline void build(size_t profile, const Rows& rows, Catalog& catalog, Stock& stock) {
	std::vector<Inventory> categories(category_keys[profile].size());
	for (const auto& row : rows) for (auto category : row.categories) categories[category].push_back(&row);
	add_node(catalog, "root", "", "", profile != 3);
	if (profile == 0) {
		const std::array<const char*, 4> ids = {{"top", "mid", "low", "garment"}};
		const std::array<const char*, 4> labels = {{"Top", "Mid", "Low / Mouth", "Garment"}};
		for (size_t slot = 0; slot < 4; ++slot) {
			if (categories[slot * 2].empty() && categories[slot * 2 + 1].empty() && (slot != 3 || categories[8].empty())) continue;
			add_node(catalog, ids[slot], "root", labels[slot], true);
			for (size_t kind = 0; kind < 2; ++kind) {
				size_t index = slot * 2 + kind;
				if (categories[index].empty()) continue;
				auto id = "c" + std::to_string(index);
				add_node(catalog, id, ids[slot], kind == 0 ? "Ordinary Costumes" : "Special Costumes");
				add_letters(catalog, stock, id, "msc_d_" + std::to_string(index), categories[index], true);
			}
			if (slot == 3 && !categories[8].empty())
				add_leaf(catalog, stock, "limited", "garment", "Limited Costume", "msc_d_8", categories[8]);
		}
	} else if (profile == 1) {
		const std::array<const char*, 6> labels = {{"Weapon", "Armor", "Shield", "Shoes", "Right Accessory", "Left Accessory"}};
		if (!categories[0].empty()) {
			add_node(catalog, "weapon", "root", "Weapon", true);
			add_letters(catalog, stock, "weapon", "msc_r_0", categories[0], false);
		}
		if (std::any_of(categories.begin() + 1, categories.end(), [](const Inventory& category) { return !category.empty(); })) {
			add_node(catalog, "armor", "root", "Armor", true);
			for (size_t category = 1; category < categories.size(); ++category) {
				if (categories[category].empty()) continue;
				auto id = "c" + std::to_string(category);
				add_node(catalog, id, "armor", labels[category], true);
				add_letters(catalog, stock, id, "msc_r_" + std::to_string(category), categories[category], false);
			}
		}
	} else {
		for (size_t category = 0; category < categories.size(); ++category) {
			require(!categories[category].empty(), "required category has no items: " + category_keys[profile][category]);
			add_leaf(catalog, stock, category_keys[profile][category], "root",
				profile == 2 ? "Spellbooks and Essence" : milo_labels[category],
				std::string(profile == 2 ? "msc_l_" : "msc_m_") + std::to_string(category), categories[category]);
		}
	}
	for (const auto& entry : catalog.nodes) if (entry.second.shop.empty()) {
		require(!entry.second.children.empty(), "empty generated menu " + entry.first);
		require(menu_text(catalog, entry.second).size() < 2047, "menu exceeds native select length: " + entry.first);
	}
}
inline bool live(const std::string& name, const std::weak_ptr<s_npc_barter>& owned, bool reserved = false) {
	auto expected = owned.lock();
	auto barter = barter_db.find(name);
	auto npc = npc_name2id(name.c_str());
	return expected && barter == expected && (reserved || !barter->items.empty()) && npc &&
		npc->subtype == NPCTYPE_BARTER && !npc->u.barter.extended && npc->id == barter->npcid;
}
inline void remove_owned(BarterDatabase& db, const Catalog& catalog) {
	for (const auto& entry : catalog.owned) {
		auto owned = entry.second.lock();
		if (owned && db.find(entry.first) == owned) db.erase(entry.first);
	}
}
inline void prepare(BarterDatabase& db) {
	catalogs.clear();
	++generation;
	for (size_t profile = 0; profile < keys.size(); ++profile) {
		Catalog replacement;
		std::string context = "envelope";
		try {
			auto rows = read_catalog(profile, context);
			context = "generated stock";
			Stock stock;
			build(profile, rows, replacement, stock);
			replacement.generation = generation;
			replacement.records = std::move(rows);
			for (size_t i = 0; i < replacement.records.size(); ++i) {
				replacement.by_id.emplace(replacement.records[i].id, i);
				replacement.search_order.push_back(i);
			}
			std::sort(replacement.search_order.begin(), replacement.search_order.end(), [&](size_t a, size_t b) {
				const auto& left = replacement.records[a];
				const auto& right = replacement.records[b];
				auto lname = lower(left.sort_name), rname = lower(right.sort_name);
				return lname == rname ? left.id < right.id : lname < rname;
			});
			replacement.search_shop = std::string("msc_search_") + keys[profile];
			auto search = make_barter(replacement.search_shop, {});
			require(stock.emplace(replacement.search_shop, search).second, "duplicate search shop");
			replacement.owned[replacement.search_shop] = search;
			for (const auto& entry : stock) {
				context = "shop " + entry.first;
				require(!db.find(entry.first) && !npc_name2id(entry.first.c_str()), "generated shop name collides with existing NPC/Barter");
			}
			context = "install";
			for (const auto& entry : stock) db.put(entry.first, entry.second);
			catalogs.emplace(keys[profile], std::move(replacement));
		} catch (const std::exception& error) {
			remove_owned(db, replacement);
			ShowError("Malangdo shop catalog %s %s: %s.\n", path_for(keys[profile]).c_str(), context.c_str(), error.what());
		}
	}
}
inline void finalize(BarterDatabase& db) {
	int count = 0;
	for (auto& entry : catalogs) {
		auto& catalog = entry.second;
		bool complete = !catalog.owned.empty();
		for (const auto& owned : catalog.owned) if (!live(owned.first, owned.second, owned.first == catalog.search_shop)) {
			ShowError("Malangdo shop catalog %s shop %s: native Barter was not created.\n",
				path_for(entry.first).c_str(), owned.first.c_str());
			complete = false;
		}
		if (complete) { catalog.published = true; ++count; }
		else {
			remove_owned(db, catalog);
			catalog = Catalog{};
		}
	}
	ShowInfo("Malangdo shop catalogs: %d/4 ready.\n", count);
}
// The native function owns its existing loops/SQL and may return early.
// Scope exit publishes only shops that those native paths actually created.
class LoadGuard {
	BarterDatabase& db;
public:
	explicit LoadGuard(BarterDatabase& database) : db(database) { prepare(db); }
	~LoadGuard() { finalize(db); }
	LoadGuard(const LoadGuard&) = delete;
	LoadGuard& operator=(const LoadGuard&) = delete;
};
inline bool ready(const std::string& key) {
	auto catalog = catalogs.find(key);
	if (catalog == catalogs.end() || !catalog->second.published) return false;
	for (const auto& owned : catalog->second.owned)
		if (!live(owned.first, owned.second, owned.first == catalog->second.search_shop)) return false;
	return true;
}
inline const Node* find(const std::string& key, const std::string& id) {
	if (!ready(key)) return nullptr;
	auto& nodes = catalogs.at(key).nodes;
	auto node = nodes.find(id);
	return node == nodes.end() ? nullptr : &node->second;
}
inline std::string menu(const std::string& key, const std::string& id) {
	auto node = find(key, id);
	return node && node->shop.empty() ? menu_text(catalogs.at(key), *node) : "";
}
inline std::string child(const std::string& key, const std::string& id, int64 index) {
	auto node = find(key, id);
	return node && index >= 0 && static_cast<uint64>(index) < node->children.size() ? node->children[index] : "";
}
inline std::string shop(const std::string& key, const std::string& id) {
	auto node = find(key, id);
	return node ? node->shop : "";
}
inline bool autosingle(const std::string& key, const std::string& id) {
	auto node = find(key, id);
	return node && node->auto_single && node->children.size() == 1;
}
inline int64 revision(const std::string& key) {
	return ready(key) ? catalogs.at(key).generation : 0;
}
inline int search(const std::string& key, std::string query, Inventory& out) {
	out.clear();
	if (!ready(key)) return -1;
	auto begin = query.find_first_not_of(' '), end = query.find_last_not_of(' ');
	query = begin == std::string::npos ? "" : query.substr(begin, end - begin + 1);
	if (query.empty()) return -2;
	const auto& catalog = catalogs.at(key);
	bool numeric = std::all_of(query.begin(), query.end(), [](char c) { return c >= '0' && c <= '9'; });
	if (numeric) {
		uint32 id = 0;
		for (char c : query) {
			uint32 digit = c - '0';
			if (id > (INT32_MAX - digit) / 10) return 0;
			id = id * 10 + digit;
		}
		auto row = catalog.by_id.find(id);
		if (row != catalog.by_id.end()) out.push_back(&catalog.records[row->second]);
	} else {
		if (query.size() < 4) return -2;
		query = lower(query);
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream input(query);
		while (std::getline(input, token, ' ')) if (!token.empty()) tokens.push_back(token);
		for (auto index : catalog.search_order) {
			const auto& row = catalog.records[index];
			auto name = lower(row.name);
			if (std::all_of(tokens.begin(), tokens.end(), [&](const auto& part) { return name.find(part) != std::string::npos; }))
				out.push_back(&row);
		}
	}
	return static_cast<int>(out.size());
}
inline bool search_busy(int32 npcid) {
	auto iterator = mapit_getallusers();
	bool busy = false;
	for (auto block = mapit_first(iterator); block; block = mapit_next(iterator)) {
		auto sd = reinterpret_cast<map_session_data*>(block);
		if (sd->npc_shopid == npcid && (sd->state.barter_open || sd->state.barter_extended_open)) {
			busy = true;
			break;
		}
	}
	mapit_free(iterator);
	return busy;
}
inline std::string search_prepare(const std::string& key, const std::string& query, int64 page,
	int64 expected_generation, const map_session_data& sd) {
	if (page < 0 || expected_generation <= 0 || revision(key) != expected_generation ||
		sd.state.barter_open || sd.state.barter_extended_open) return "";
	Inventory matches;
	if (search(key, query, matches) <= 0 || static_cast<uint64>(page) > (matches.size() - 1) / search_page_size) return "";
	auto& catalog = catalogs.at(key);
	auto barter = catalog.owned.at(catalog.search_shop).lock();
	if (!barter || search_busy(barter->npcid)) return "";
	const size_t begin = static_cast<size_t>(page) * search_page_size;
	const size_t end = std::min(matches.size(), begin + search_page_size);
	Inventory selected(matches.begin() + begin, matches.begin() + end);
	std::set<t_itemid> seen;
	bool limited = false;
	for (auto row : selected) {
		if (!item_db.find(row->id) || !seen.insert(row->id).second) return "";
		if (key == "dewey" && std::find(row->categories.begin(), row->categories.end(), 8) != row->categories.end()) limited = true;
	}
	try {
		auto replacement = make_barter(catalog.search_shop, selected);
		// No yielding/callback occurs between validation and this inventory swap.
		barter->items.swap(replacement->items);
		catalog.search_limited = limited;
		return catalog.search_shop;
	} catch (const std::exception& error) {
		ShowError("Malangdo search %s: %s.\n", key.c_str(), error.what());
		return "";
	}
}
inline bool shop_limited(const std::string& key, const std::string& name) {
	if (key != "dewey" || !ready(key)) return false;
	const auto& catalog = catalogs.at(key);
	auto owned = catalog.owned.find(name);
	if (owned == catalog.owned.end() || !live(name, owned->second)) return false;
	auto limited = catalog.nodes.find("limited");
	return (limited != catalog.nodes.end() && name == limited->second.shop) ||
		(name == catalog.search_shop && catalog.search_limited);
}
} // namespace msc
#endif
