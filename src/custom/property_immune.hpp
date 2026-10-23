// Project-owned NPC_IMMUNE_PROPERTY level 11 extension.
#ifndef OFFLINE_PROPERTY_IMMUNE_HPP
#define OFFLINE_PROPERTY_IMMUNE_HPP

#include <map/status.hpp>
#include <map/mob.hpp>

namespace offline_property_immune {

// Reuse the native neutral immunity lifecycle and visual. Its val1 stores
// the skill level, so level 1 remains neutral-only and level 11 covers all
// elements without starting mutually exclusive elemental statuses.
inline sc_type initial_status(const block_list* caster, uint16 skill_level, sc_type native_status) {
	if (skill_level != 11 || caster == nullptr || caster->type != BL_MOB)
		return native_status;
	const auto* monster = static_cast<const mob_data*>(caster);
	// Scope this content addition to the audited Biosphere MVPs and Renire.
	// Existing EP20 level-11 rows keep their native behavior.
	switch (monster->mob_id) {
		case 21927:
		case 21935:
		case 21943:
		case 21955:
			return SC_IMMUNE_PROPERTY_NOTHING;
		default:
			return native_status;
	}
}

inline bool block_all_elements(const status_change* sc, int64& damage) {
	if (sc == nullptr)
		return false;
	const auto* immunity = sc->getSCE(SC_IMMUNE_PROPERTY_NOTHING);
	if (immunity == nullptr || immunity->val1 != 11)
		return false;
	damage = 0;
	return true;
}

} // namespace offline_property_immune
#endif
