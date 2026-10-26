#ifndef OFFLINE_SHADOW_WEIGHTED_ROLL_HPP
#define OFFLINE_SHADOW_WEIGHTED_ROLL_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace offline_shadow {
// A ticket is zero-based; each entry owns exactly its weight many tickets.
template<class Entries, class Weight>
std::size_t weighted_index(const Entries& entries, Weight weight, std::uint32_t ticket) {
	for (std::size_t index = 0; index < entries.size(); ++index) {
		const auto count = static_cast<std::uint32_t>(weight(entries[index]));
		if (ticket < count)
			return index;
		ticket -= count;
	}
	throw std::out_of_range("Shadow random-option ticket outside pool");
}
inline bool second_option(std::uint32_t ticket) { return ticket < 33; }
inline bool owns_group(std::uint16_t id) { return id >= 200 && id <= 205; }
}
#endif
