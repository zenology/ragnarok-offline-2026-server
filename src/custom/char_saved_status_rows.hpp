#ifndef OFFLINE_CHAR_SAVED_STATUS_ROWS_HPP
#define OFFLINE_CHAR_SAVED_STATUS_ROWS_HPP
#include <cstddef>
#include <cstdint>
#include <limits>
#include <cerrno>
#include <cstdlib>

namespace offline_saved_status {
inline bool parse_value(const char* text, std::int64_t& value) {
	if (text == nullptr || *text == '\0') return false;
	char* end = nullptr;
	errno = 0;
	value = std::strtoll(text, &end, 10);
	return errno != ERANGE && end != text && *end == '\0';
}

template<class ReadField>
bool validate_values(ReadField read) {
	std::int64_t fields[6];
	for (unsigned int i = 0; i < 6; ++i) if (!read(i, fields[i])) return false;
	if (fields[0] < 0 || fields[0] > std::numeric_limits<std::uint16_t>::max() || fields[1] < -1) return false;
	for (int i = 2; i < 6; ++i)
		if (fields[i] < std::numeric_limits<std::int32_t>::min() || fields[i] > std::numeric_limits<std::int32_t>::max()) return false;
	return true;
}
template<class Row> constexpr std::size_t capacity() {
	return (std::numeric_limits<std::int16_t>::max() - 14) / sizeof(Row);
}
template<class Row> constexpr std::int32_t bounded_count(std::uint64_t count) {
	return count <= capacity<Row>() ? static_cast<std::int32_t>(count) : 0;
}

inline constexpr std::int32_t complete_count(std::int32_t expected, std::int32_t actual) {
	return expected >= 0 && expected == actual ? actual : 0;
}
} // namespace offline_saved_status
#endif
