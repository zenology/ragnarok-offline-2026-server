// Pesca's published Servant Weapon schedule, scoped to monster 22438.
#ifndef OFFLINE_MONSTER_SERVANT_WEAPON_HPP
#define OFFLINE_MONSTER_SERVANT_WEAPON_HPP

#include <common/random.hpp>
#include <map/mob.hpp>
#include <map/skill.hpp>
#include <map/status.hpp>

namespace offline_monster_servant_weapon {

inline bool supported(const block_list* caster) {
	return caster != nullptr && caster->type == BL_MOB
		&& static_cast<const mob_data*>(caster)->mob_id == 22438;
}

// val3 is unused by the native player status. Keep monster charges inside the
// status entry so expiration, death and status cleanup own their lifetime.
inline void replenish(const block_list* caster, int32& charges) {
	if (supported(caster))
		charges = 5;
}

inline void on_normal_attack(block_list* caster, block_list* target, t_tick tick, int64 damage) {
	if (!supported(caster) || target == nullptr || caster == target || damage <= 0
		|| status_isdead(*caster) || status_isdead(*target))
		return;
	status_change* sc = status_get_sc(caster);
	status_change_entry* servant = sc == nullptr ? nullptr : sc->getSCE(SC_SERVANTWEAPON);
	if (servant == nullptr || servant->val3 <= 0 || !rnd_chance(5 * servant->val1, 100))
		return;
	const uint16 level = servant->val1;
	--servant->val3;
	// Use the native damage calculation and visual; this does not recurse into
	// battle_weapon_attack, and player charge/autocast handling is unchanged.
	skill_castend_damage_id(caster, target, DK_SERVANTWEAPON_ATK, level, tick, 0);
}

} // namespace offline_monster_servant_weapon
#endif
