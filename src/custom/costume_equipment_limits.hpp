#ifndef OFFLINE_COSTUME_EQUIPMENT_LIMITS_HPP
#define OFFLINE_COSTUME_EQUIPMENT_LIMITS_HPP

#include <common/mmo.hpp>

namespace offline_costume_limits {

inline bool is_blocked(t_itemid item_id, int32 job_id) {
	if (item_id != 20513 && item_id != 31917)
		return false;

	switch (job_id) {
		case JOB_SHINKIRO:
		case JOB_NIGHT_WATCH:
		case JOB_HYPER_NOVICE:
		case JOB_SPIRIT_HANDLER:
			return true;
		default:
			return false;
	}
}

} // namespace offline_costume_limits

#endif
