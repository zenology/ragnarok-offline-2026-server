#ifndef OFFLINE_PREMIUM_DROP_PENALTY_HPP
#define OFFLINE_PREMIUM_DROP_PENALTY_HPP

#include <algorithm>
#include <ctime>
#include <map/clif.hpp>
#include <map/pc.hpp>
#include <map/script.hpp>
#include <map/status.hpp>

namespace offline_premi_drop_penalty {
// The account deadline is authoritative, including after logout and NPC reload.
// There is no saved status or session flag that can outlive the paid entitlement.
inline int64 remaining(const map_session_data& sd) {
	if (!sd.vars_ok) return 0;
	return std::max<int64>(0, pc_readaccountreg(&sd, add_str("#PREMIUM_NODROPPENALTY")) - std::time(nullptr));
}

inline bool bypass(const map_session_data& sd, e_penalty_type type) {
	return (type == PENALTY_DROP || type == PENALTY_MVP_DROP) && remaining(sd) > 0;
}

inline void sync_icon(map_session_data& sd) {
	const int64 seconds = std::min<int64>(remaining(sd), 86400);
	clif_status_change(&sd, EFST_IGNOREITEMDROPPENALTY, seconds > 0 ? 1 : 0,
		static_cast<t_tick>(seconds * 1000), 0, 0, 0);
}
} // namespace offline_premi_drop_penalty
#endif
