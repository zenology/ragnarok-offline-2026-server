#ifndef OFFLINE_SHADOW_ENCHANTMENT_HPP
#define OFFLINE_SHADOW_ENCHANTMENT_HPP

#include "shadow_weighted_roll.hpp"
#include "../map/itemdb.hpp"
#include "../common/random.hpp"
#include "../common/showmsg.hpp"

namespace offline_shadow {
template<class Entries>
bool valid_pool(const Entries& entries) {
	if (entries.empty()) return false;
	for (const auto& entry : entries)
		if (!entry || entry->id == 0 || entry->chance == 0 || entry->min_value > entry->max_value)
			return false;
	return true;
}
inline bool valid_group(const s_random_opt_group& group) {
	if (!owns_group(group.id)) return true;
	const auto first = group.slots.find(0);
	return group.slots.size() == 1 && first != group.slots.end() && valid_pool(first->second)
		&& (group.id == 200 ? group.max_random == 2 && valid_pool(group.random_options)
			: group.max_random == 0 && group.random_options.empty());
}
template<class Entries>
void roll_option(s_item_randomoption& output, const Entries& entries) {
	std::uint32_t total = 0;
	for (const auto& entry : entries) total += entry->chance;
	const auto index = weighted_index(entries, [](const auto& entry) { return entry->chance; },
		rnd_value<std::uint32_t>(0, total - 1));
	const auto& entry = entries[index];
	output.id = entry->id;
	output.value = rnd_value(entry->min_value, entry->max_value);
	output.param = entry->param;
}
inline bool apply(const s_random_opt_group& group, struct item& target) {
	if (!owns_group(group.id)) return false;
	if (!valid_group(group)) {
		ShowError("Invalid offline Shadow random-option group %hu.\n", group.id);
		return true;
	}
	for (auto& option : target.option) option = {};
	roll_option(target.option[0], group.slots.at(0));
	if (group.id == 200 && second_option(rnd_value<std::uint32_t>(0, 99)))
		roll_option(target.option[1], group.random_options);
	return true;
}
}
#endif
