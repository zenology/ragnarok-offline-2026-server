// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "rekenbercelestial.hpp"

#include "../../clif.hpp"
#include "../../map.hpp"
#include "../../skill.hpp"
#include "../../status.hpp"

SkillRekenberCelestial::SkillRekenberCelestial() : SkillImpl(OFFLINE_REKENBER_CELESTIAL)
{
}

void SkillRekenberCelestial::calculateSkillRatio(const Damage *, const block_list *, const block_list *, uint16, int32 &base_skillratio, int32) const
{
	// Renewal skill ratio starts at 100. Official Grade B line is 400% damage.
	base_skillratio += 300;
}

void SkillRekenberCelestial::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const
{
	if (!(flag & 1))
		return;

	int32 heal = static_cast<int32>(skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag));

	if (heal <= 0)
		return;

	clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
	status_heal(src, heal, 0, 0);
}

void SkillRekenberCelestial::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	skill_area_temp[1] = 0;
	e_skill skillId = getSkillId();

	map_foreachinallrange(skill_area_sub, src, skill_get_splash(skillId, skill_lv), BL_SKILL | BL_CHAR,
		src, skillId, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	clif_skill_nodamage(src, *src, skillId, skill_lv);
}
