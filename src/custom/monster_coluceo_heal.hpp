// Hanom's published self-heal schedule. Native player party healing is retained.
#pragma once

#include <map/mob.hpp>
#include <map/skills/acolyte/coluceoheal.hpp>
#include <map/skills/acolyte/heal.hpp>

namespace offline_monster_coluceo_heal {

inline bool cast(block_list* src, block_list* target, uint16 level, t_tick tick, int32& flag) {
	if (src == nullptr || src->type != BL_MOB || src != target)
		return false;
	const uint16 id = static_cast<mob_data*>(src)->mob_id;
	if (id != 22435 && id != 22440)
		return false;
	// Monsters have no player party or learned Heal level. Use this scheduled
	// level for native Heal, including its immunity/status/reversal handling.
	SkillHeal heal;
	heal.castendNoDamageId(src, target, level, tick, flag);
	return true;
}

class SkillMonsterColuceoHeal final : public SkillColuceoHeal {
public:
	void castendNoDamageId(block_list* src, block_list* target, uint16 level, t_tick tick, int32& flag) const override {
		if (cast(src, target, level, tick, flag))
			return;
		SkillColuceoHeal::castendNoDamageId(src, target, level, tick, flag);
	}
};

} // namespace offline_monster_coluceo_heal
