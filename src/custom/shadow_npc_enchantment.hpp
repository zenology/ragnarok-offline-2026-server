#ifndef OFFLINE_SHADOW_NPC_ENCHANTMENT_HPP
#define OFFLINE_SHADOW_NPC_ENCHANTMENT_HPP

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "../map/pc.hpp"
#include "shadow_enchantment.hpp"

// Ordinary NPC dialogue adapter. Read the active native tables rather than
// maintaining a second set of target lists or bonuses. Inventory mutation and
// payment remain in the equipped-item NPC flow used by Doberkin.
namespace offline_shadow_npc {
constexpr t_itemid essence = 1001253;
constexpr uint32 reform_key = 0x10000000;
constexpr uint32 book_key = 0x20000000;
constexpr std::array<t_itemid, 22> books{{
	23720,100205,100206,100207,100208,101177,101178,101179,101180,
	101260,101261,101262,101264,101308,101309,101310,101311,
	101359,101360,101361,101362,101564
}};

struct action {
	uint32 key;
	int32 card_slot;
	t_itemid material;
	uint16 amount;
	std::string label;
	std::vector<t_itemid> enchants;
	std::shared_ptr<s_random_opt_group> group;
};

inline bool shadow_slot(int32 slot) {
	return slot == EQI_SHADOW_WEAPON || slot == EQI_SHADOW_ARMOR ||
		slot == EQI_SHADOW_SHIELD || slot == EQI_SHADOW_SHOES ||
		slot == EQI_SHADOW_ACC_R || slot == EQI_SHADOW_ACC_L;
}

inline struct item* equipped(map_session_data& sd, int32 slot) {
	if (!shadow_slot(slot) || sd.state.laphine_upgrade || sd.state.item_reform ||
		sd.state.item_enchant_index) return nullptr;
	const int16 index = pc_checkequip(&sd, equip_bitmask[slot]);
	if (index < 0 || !sd.inventory_data[index] ||
		sd.inventory_data[index]->type != IT_SHADOWGEAR) return nullptr;
	auto& gear = sd.inventory.u.items_inventory[index];
	if (!gear.identify || gear.attribute || gear.equipSwitch) return nullptr;
	return &gear;
}

inline bool valid_group(const std::shared_ptr<s_random_opt_group>& group) {
	return group && offline_shadow::owns_group(group->id) &&
		offline_shadow::valid_group(*group);
}

inline bool ro_eligible(const struct item& gear, uint16 minimum, uint16 maximum,
	bool cards_allowed, uint16 required_options) {
	if (gear.refine < minimum || gear.refine > maximum) return false;
	if (!cards_allowed)
		for (int32 i = 0; i < MAX_SLOTS; ++i) if (gear.card[i]) return false;
	int32 highest = 0;
	for (int32 i = 0; i < MAX_ITEM_RDM_OPT; ++i)
		if (gear.option[i].id) highest = i + 1;
	return highest >= required_options;
}

inline std::vector<action> actions(const struct item& gear) {
	std::vector<action> result;
	const auto data = item_db.find(gear.nameid);
	if (!data || data->type != IT_SHADOWGEAR) return result;
	for (uint64 id = 70; id <= 88; ++id) {
		const auto table = item_enchant_db.find(id);
		if (!table || std::find(table->target_item_ids.begin(), table->target_item_ids.end(),
			gear.nameid) == table->target_item_ids.end() ||
			gear.refine < table->minimumRefine || gear.enchantgrade < table->minimumEnchantgrade)
			continue;
		bool has_options = false;
		for (int32 i = 0; i < MAX_ITEM_RDM_OPT; ++i)
			if (gear.option[i].id) has_options = true;
		if (!table->allowRandomOptions && has_options) continue;
		for (const uint16 slot : table->order) {
			if (slot >= MAX_SLOTS || slot < data->slots) break;
			const auto definition = table->slots.find(slot);
			if (definition == table->slots.end()) break;
			const auto& pool = definition->second->perfect.enchants;
			// Existing valid enchants may be rerolled in their own socket. Empty
			// sockets still follow the original fourth-before-third fill order.
			if (gear.card[slot] && pool.find(gear.card[slot]) == pool.end()) break;
			std::map<uint16, std::vector<t_itemid>> costs;
			for (const auto& entry : pool) {
				const auto& enchant = entry.second;
				if (!enchant || enchant->zeny || enchant->materials.size() != 1) continue;
				const auto material = enchant->materials.find(essence);
				if (material == enchant->materials.end() ||
					(material->second != 1 && material->second != 3 && material->second != 5)) continue;
				const auto charm = item_db.find(enchant->item_id);
				if (charm && charm->type == IT_CARD)
					costs[material->second].push_back(enchant->item_id);
			}
			for (auto& cost : costs) {
				std::sort(cost.second.begin(), cost.second.end());
				const std::string family = slot == 3 ? (id == 70 ? "Attributes" : "Traits") :
					id == 70 ? "Traits" : cost.first == 3 ? "General Souls" : "Job Souls";
				result.push_back({static_cast<uint32>((id << 8) | (slot << 4) | cost.first),
					slot, essence, cost.first, "Socket " + std::to_string(slot + 1) + " - " + family,
					std::move(cost.second), nullptr});
			}
			if (!gear.card[slot]) break;
		}
	}
	const auto reform = item_reform_db.find(103309);
	if (reform) {
		const auto row = reform->base_items.find(gear.nameid);
		if (row != reform->base_items.end()) {
			const auto& base = row->second;
			if (base && base->resultItemId == gear.nameid && !base->refineChange &&
				!base->clearSlots && !base->removeEnchantgrade && base->materials.size() == 1 &&
				base->materials.find(essence) != base->materials.end() &&
				base->materials.at(essence) == 2 && valid_group(base->randomOptionGroup) &&
				ro_eligible(gear, base->minimumRefine, base->maximumRefine,
					base->cardsAllowed, base->requiredRandomOptions))
				result.push_back({reform_key, -1, essence, 2, "Fourth Job Random Options",
					{}, base->randomOptionGroup});
		}
	}
	for (const auto book : books) {
		const auto upgrade = laphine_upgrade_db.find(book);
		const auto material = item_db.find(book);
		if (!upgrade || !material || !valid_group(upgrade->randomOptionGroup) ||
			std::find(upgrade->target_item_ids.begin(), upgrade->target_item_ids.end(),
				gear.nameid) == upgrade->target_item_ids.end() ||
			upgrade->resultRefine || upgrade->resultRefineMinimum || upgrade->resultRefineMaximum ||
			!ro_eligible(gear, upgrade->minimumRefine, upgrade->maximumRefine,
				upgrade->cardsAllowed, upgrade->requiredRandomOptions)) continue;
		result.push_back({book_key | book, -1, book, 1, material->ename,
			{}, upgrade->randomOptionGroup});
	}
	return result;
}

// Roll into a copy only. The NPC rechecks its full snapshot, then uses the
// existing instance-preserving replaceequipped command and immediately pays.
inline bool roll(map_session_data& sd, int32 slot, uint32 key, struct item& output) {
	const auto gear = equipped(sd, slot);
	if (!gear) return false;
	for (const auto& entry : actions(*gear)) {
		if (entry.key != key) continue;
		output = *gear;
		if (entry.card_slot >= 0) {
			if (entry.enchants.empty()) return false;
			output.card[entry.card_slot] = entry.enchants[rnd_value<uint32>(0,
				static_cast<uint32>(entry.enchants.size() - 1))];
		} else {
			if (!valid_group(entry.group)) return false;
			entry.group->apply(output);
		}
		return true;
	}
	return false;
}
}
#endif
