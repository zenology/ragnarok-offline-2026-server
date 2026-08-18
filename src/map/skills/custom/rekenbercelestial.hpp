// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "../skill_impl.hpp"

#include "../../battle.hpp"

// Project-owned hidden item skill. Explicit ID stays above official e_skill values.
// Do not add this enumerator to skill.hpp.
constexpr e_skill OFFLINE_REKENBER_CELESTIAL = static_cast<e_skill>(10000);

class SkillRekenberCelestial : public SkillImpl
{
public:
	SkillRekenberCelestial();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
