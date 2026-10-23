// Forgotten Time restrictions from the player's 2026-10-23 skill/buff roster.
#ifndef OFFLINE_FORGOTTEN_TIME_RESTRICTIONS_HPP
#define OFFLINE_FORGOTTEN_TIME_RESTRICTIONS_HPP

#include <array>
#include <cstring>
#include <map/clif.hpp>
#include <map/map.hpp>
#include <map/pc.hpp>
#include <map/skill.hpp>
#include <map/status.hpp>

namespace offline_forgotten_time {

inline bool restricted_map(int16 map_id) {
	if (map_id < 0)
		return false;
	const map_data* map = map_getmapdata(map_id);
	return map != nullptr && (std::strcmp(map->name, "for_dun01") == 0
		|| std::strcmp(map->name, "for_dun02") == 0);
}

inline bool skill_blocked(map_session_data& sd, uint16 skill_id) {
	if (!restricted_map(sd.m))
		return false;
	switch (skill_id) {
		case AL_TELEPORT:
		case TF_BACKSLIDING:
		case RG_INTIMIDATE:
		case MO_BODYRELOCATION:
		case TK_HIGHJUMP:
		case NJ_SHADOWJUMP:
		case SU_LOPE:
		case AB_CONVENIO:
		case SC_DIMENSIONDOOR:
		case RL_FALLEN_ANGEL:
		case SC_ESCAPE:
		case NC_F_SIDESLIDE:
		case NC_B_SIDESLIDE:
			clif_msg_color(sd, MSI_IMPOSSIBLE_SKILL_AREA, color_table[COLOR_CYAN]);
			return true;
		default:
			return false;
	}
}

inline constexpr std::array<sc_type, 11> removed_buffs = {
	SC_COMPETENTIA,
	SC_D_MACHINE,
	SC_MUSICAL_INTERLUDE,
	SC_JAWAII_SERENADE,
	SC_PRON_MARCH,
	SC_SPELL_ENCHANTING,
	SC_TALISMAN_OF_PROTECTION,
	SC_TALISMAN_OF_WARRIOR,
	SC_TALISMAN_OF_MAGICIAN,
	SC_HEAVEN_AND_EARTH,
	SC_TALISMAN_OF_FIVE_ELEMENTS,
};

inline bool status_blocked(const block_list* target, sc_type type) {
	if (target == nullptr || target->type != BL_PC || !restricted_map(target->m))
		return false;
	for (sc_type buff : removed_buffs) {
		if (type == buff)
			return true;
	}
	return false;
}

inline void clear_on_entry(block_list* target, status_change* sc) {
	if (target == nullptr || target->type != BL_PC || sc == nullptr
		|| !restricted_map(target->m))
		return;
	for (sc_type buff : removed_buffs) {
		if (sc->getSCE(buff) != nullptr)
			status_change_end(target, buff);
	}
}

} // namespace offline_forgotten_time
#endif
