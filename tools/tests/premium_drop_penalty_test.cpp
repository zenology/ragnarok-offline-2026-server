// Exercise the actual entitlement helper with native registry/packet boundaries mocked.
#include <cassert>
#include <cstring>
#include <iostream>
#include "../../src/custom/premium_drop_penalty.hpp"

static int64 expiry = 0;
static int reads = 0;
static int icon_type = 0, icon_flag = 0;
static t_tick icon_time = 0;

// This helper does not read or mutate status changes; no live status timers exist here.
status_change::status_change() = default;
status_change_entry::~status_change_entry() = default;

int32 add_str(const char* name) {
	assert(std::strcmp(name, "#PREMIUM_NODROPPENALTY") == 0);
	return 123;
}
int64 pc_readregistry(const map_session_data*, int64 key) {
	assert(key == 123);
	++reads;
	return expiry;
}
void clif_status_change(const block_list*, int32 type, int32 flag, t_tick tick, int32, int32, int32) {
	icon_type = type; icon_flag = flag; icon_time = tick;
}

int main() {
	map_session_data sd{};
	using namespace offline_premi_drop_penalty;
	expiry = std::time(nullptr) + 3600;
	assert(!bypass(sd, PENALTY_DROP) && reads == 0); // Never read unloaded registries.
	sd.vars_ok = true;
	assert(!bypass(sd, PENALTY_EXP) && !bypass(sd, PENALTY_MVP_EXP) && reads == 0);
	assert(!bypass(sd, PENALTY_NONE) && !bypass(sd, PENALTY_MAX));
	assert(bypass(sd, PENALTY_DROP) && bypass(sd, PENALTY_MVP_DROP));
	sync_icon(sd);
	assert(icon_type == EFST_IGNOREITEMDROPPENALTY && icon_flag == 1);
	assert(icon_time > 3590000 && icon_time <= 3600000);
	expiry = std::time(nullptr) + 100000;
	sync_icon(sd);
	assert(icon_time == 86400000); // Packet duration is bounded even for malformed future data.
	for (auto ended : {int64{0}, int64{-1}, int64{std::time(nullptr) - 1}, int64{std::time(nullptr)}}) {
		expiry = ended;
		assert(!bypass(sd, PENALTY_DROP) && !bypass(sd, PENALTY_MVP_DROP));
		sync_icon(sd);
		assert(icon_flag == 0 && icon_time == 0);
	}
	std::cout << "PASS: drop/MVP-only entitlement, unloaded/expired registry, timed icon and duration bound.\n";
}
