// Activate the existing timed aura only for the three audited high-level MVPs.
#pragma once

#include <map/mob.hpp>
#include <map/skills/skill_impl.hpp>

namespace offline_high_level_killing_aura {

class SkillKillingAura final : public SkillImplRecursiveDamageSplash {
public:
	SkillKillingAura() : SkillImplRecursiveDamageSplash(NPC_KILLING_AURA) {}

	void castendNoDamageId(block_list* src, block_list* target, uint16 level,
		t_tick tick, int32& flag) const override {
		if (src == nullptr || src->type != BL_MOB || src != target)
			return;
		const uint16 id = static_cast<mob_data*>(src)->mob_id;
		if (id != 22394 && id != 22404 && id != 22438)
			return;
		// Native SC_KILLING_AURA already owns duration, one-second pulses,
		// damage calculation and cleanup. Only the missing activation is added.
		StatusSkillImpl status(NPC_KILLING_AURA);
		status.castendNoDamageId(src, target, level, tick, flag);
	}
};

} // namespace offline_high_level_killing_aura
