// Player-approved local implementation of NPC_RANDOMBREAK (777).
#pragma once

#include <array>
#include <common/random.hpp>
#include <map/clif.hpp>
#include <map/pc.hpp>
#include <map/skill.hpp>
#include <map/skills/skill_impl.hpp>

namespace offline_random_break {

// Native callers retain their unrestricted behavior. The custom skill supplies
// one inventory index so dual-wield weapons cannot both break in one cast.
inline bool selected_item(int16 inventory_index, int16 selected_index) {
	return selected_index < 0 || inventory_index == selected_index;
}

class SkillRandomBreak final : public SkillImpl {
public:
	SkillRandomBreak() : SkillImpl(NPC_RANDOMBREAK) {}

	void castendNoDamageId(block_list* src, block_list* target, uint16 level,
		t_tick tick, int32& flag) const override {
		map_session_data* sd = BL_CAST(BL_PC, target);
		if (sd == nullptr)
			return;
		// The four equipment families supported by native NPC Break skills.
		// Keep protected items in the draw: protection absorbs the attempt.
		constexpr std::array<int, 4> slots = {
			EQI_HEAD_TOP, EQI_ARMOR, EQI_HAND_R, EQI_HAND_L
		};
		std::array<int16, 4> candidates{};
		std::array<uint16, 4> masks{};
		size_t count = 0;
		for (int slot : slots) {
			const int16 index = sd->equip_index[slot];
			if (index < 0 || sd->inventory_data[index] == nullptr
				|| sd->inventory.u.items_inventory[index].attribute == 1)
				continue;
			bool duplicate = false;
			for (size_t i = 0; i < count; ++i)
				duplicate = duplicate || candidates[i] == index;
			if (duplicate)
				continue;
			uint16 mask = slot == EQI_HEAD_TOP ? EQP_HELM : EQP_ARMOR;
			if (slot == EQI_HAND_R || slot == EQI_HAND_L)
				mask = sd->inventory_data[index]->type == IT_WEAPON ? EQP_WEAPON : EQP_SHIELD;
			candidates[count] = index;
			masks[count++] = mask;
		}
		if (count != 0) {
			const size_t chosen = rnd() % count;
			skill_break_equip(src, target, masks[chosen], 150 * level,
				BCT_ENEMY, candidates[chosen]);
		}
		clif_skill_nodamage(src, *target, NPC_RANDOMBREAK, level);
	}
};

} // namespace offline_random_break
