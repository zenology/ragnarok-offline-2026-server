// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Premi-only lifecycle override. Native NPC/storage implementations stay intact.
#ifndef OFFLINE_PREMIUM_SERVICE_HPP
#define OFFLINE_PREMIUM_SERVICE_HPP

#include <cstring>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>

#include <common/showmsg.hpp>
#include <common/timer.hpp>
#include <map/battle.hpp>
#include <map/clif.hpp>
#include <map/npc.hpp>
#include <map/pc.hpp>
#include <map/script.hpp>

namespace offline_premi {

struct PremiLifecycleState {
	intptr_t generation;
	int32 timer_id;
	uint32 owner_char_id;
	int32 source_id;
	std::string exname;
	npc_data* identity;
};

static std::unordered_map<int32, PremiLifecycleState> lifecycles;
static intptr_t next_generation = 0;

// Never put this callback in dynamicnpc.removal_tid: native unload expects
// its own removal callback there. Our timer is owned only by lifecycles.
static TIMER_FUNC(removal_timer) {
	auto it = lifecycles.find(id);
	if (it == lifecycles.end() || it->second.generation != data || it->second.timer_id != tid)
		return 0;

	npc_data* nd = map_id2nd(id);
	const auto& state = it->second;
	if (nd == nullptr || nd != state.identity ||
		nd->dynamicnpc.owner_char_id != state.owner_char_id ||
		nd->src_id != state.source_id || state.exname != nd->exname) {
		lifecycles.erase(it);
		return 0;
	}

	map_session_data* owner = map_charid2sd(state.owner_char_id);
	if (owner != nullptr) {
		if (owner->npc_id == nd->id || owner->npc_shopid == nd->id) {
			nd->dynamicnpc.last_interaction = gettick();
			it->second.timer_id = add_timer(nd->dynamicnpc.last_interaction + battle_config.feature_dynamicnpc_timeout,
				removal_timer, id, data);
			return 0;
		}
		// Preserve the checked-out native callback's rearm formula exactly.
		if (DIFF_TICK(gettick(), nd->dynamicnpc.last_interaction) < battle_config.feature_dynamicnpc_timeout) {
			it->second.timer_id = add_timer(nd->dynamicnpc.last_interaction + DIFF_TICK(gettick(), nd->dynamicnpc.last_interaction),
				removal_timer, id, data);
			return 0;
		}
		if (owner->m == nd->m)
			clif_specialeffect_single(nd, EF_TELEPORTATION, owner->fd);
	}

	lifecycles.erase(it);
	npc_unload(nd, true);
	npc_read_event_script();
	return 0;
}

static intptr_t allocate_generation() {
	// Signed overflow is forbidden; avoid reusing a live token on wraparound.
	bool used;
	do {
		next_generation = next_generation == std::numeric_limits<intptr_t>::max() ? 1 : next_generation + 1;
		used = false;
		for (const auto& entry : lifecycles) {
			if (entry.second.generation == next_generation) {
				used = true;
				break;
			}
		}
	} while (used);
	return next_generation;
}

static npc_data* summon(map_session_data& owner) {
	npc_data* source = npc_name2id("Premi#src");
	if (source == nullptr) {
		ShowError("premisummon: Premi#src was not found.\n");
		return nullptr;
	}
	npc_data* duplicate = npc_duplicate_npc_for_player(*source, owner);
	if (duplicate == nullptr)
		return nullptr;

	const int32 native_tid = duplicate->dynamicnpc.removal_tid;
	const TimerData* native_timer = get_timer(native_tid);
	if (native_timer == nullptr || native_timer->func == nullptr) {
		ShowError("premisummon: Native removal timer is unavailable.\n");
		duplicate->dynamicnpc.removal_tid = INVALID_TIMER;
		npc_unload(duplicate, true);
		npc_read_event_script();
		return nullptr;
	}
	const t_tick deadline = native_timer->tick;
	const TimerFunc native_callback = native_timer->func;
	if (delete_timer(native_tid, native_callback) != 0) {
		ShowError("premisummon: Could not cancel native removal timer.\n");
		npc_unload(duplicate, true);
		npc_read_event_script();
		return nullptr;
	}
	duplicate->dynamicnpc.removal_tid = INVALID_TIMER;

	static bool registered = false;
	if (!registered) {
		add_timer_func_list(removal_timer, "offline_premi_removal_timer");
		registered = true;
	}
	// A previously unloaded NPC may leave a pending custom timer. Retire its
	// record before reusing the runtime ID; generation checks reject old calls.
	auto previous = lifecycles.find(duplicate->id);
	if (previous != lifecycles.end()) {
		delete_timer(previous->second.timer_id, removal_timer);
		lifecycles.erase(previous);
	}
	const intptr_t generation = allocate_generation();
	PremiLifecycleState state{generation, INVALID_TIMER, duplicate->dynamicnpc.owner_char_id,
		duplicate->src_id, duplicate->exname, duplicate};
	auto inserted = lifecycles.emplace(duplicate->id, std::move(state));
	inserted.first->second.timer_id = add_timer(deadline, removal_timer, duplicate->id, generation);
	if (inserted.first->second.timer_id == INVALID_TIMER) {
		ShowError("premisummon: Could not install custom removal timer.\n");
		lifecycles.erase(inserted.first);
		npc_unload(duplicate, true);
		npc_read_event_script();
		return nullptr;
	}
	clif_specialeffect_single(duplicate, EF_TELEPORTATION, owner.fd);
	return duplicate;
}

} // namespace offline_premi

#endif
