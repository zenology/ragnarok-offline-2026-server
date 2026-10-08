#ifndef OFFLINE_PREMI_AUTOBUFF_HPP
#define OFFLINE_PREMI_AUTOBUFF_HPP

#include <ctime>
#include <common/random.hpp>
#include <common/showmsg.hpp>
#include <common/timer.hpp>
#include <map/clif.hpp>
#include <map/log.hpp>
#include <map/pc.hpp>
#include <map/script.hpp>
#include <map/skill.hpp>
#include <map/status.hpp>
#include "premium_autobuff_state.hpp"
#include "premium_autobuff_transaction.hpp"

namespace offline_premi_autobuff {
static_assert(SC_AUTOBUFF == 1046 && SC_MAX == 1047, "Auto Buff status baseline changed");
static_assert(EFST_AUTOBUFF == 1723 && EFST_MAX == 1724, "Auto Buff icon baseline changed");

inline bool premium(const map_session_data& sd) {
	if (!sd.vars_ok) return false;
	const auto now = std::time(nullptr);
	std::int64_t remaining = 0;
#ifdef VIP_ENABLE
	if (pc_isvip(&sd) && sd.vip.time > now) remaining = sd.vip.time - now;
#endif
	const auto until = pc_readaccountreg(&sd, add_str("#PREMIUM_UNTIL"));
	return std::max<std::int64_t>(remaining, until - now) > 0;
}
inline int eligibility(map_session_data& sd) {
	if (pc_isdead(&sd)) return -2;
	if (!sd.vars_ok) return -6;
	if (!premium(sd)) return -3;
	return 0;
}
inline int soul_link(const map_session_data& sd) {
	const int eac = pc_jobid2mapid(sd.status.class_);
	const int base = eac & MAPID_FIRSTMASK;
	if ((eac & JOBL_UPPER) && !(eac & (JOBL_2 | JOBL_THIRD | JOBL_FOURTH)) && sd.status.base_level < 70) {
		if (base == MAPID_SWORDMAN || base == MAPID_MAGE || base == MAPID_ARCHER
			|| base == MAPID_ACOLYTE || base == MAPID_MERCHANT || base == MAPID_THIEF)
			return SL_HIGH;
	}
	switch (eac & MAPID_THIRDMASK) {
		case MAPID_RUNE_KNIGHT: return SL_KNIGHT;
		case MAPID_WARLOCK: return SL_WIZARD;
		case MAPID_RANGER: return SL_HUNTER;
		case MAPID_ARCH_BISHOP: return SL_PRIEST;
		case MAPID_MECHANIC: return SL_BLACKSMITH;
		case MAPID_GUILLOTINE_CROSS: return SL_ASSASIN;
		case MAPID_ROYAL_GUARD: return SL_CRUSADER;
		case MAPID_SORCERER: return SL_SAGE;
		case MAPID_MINSTRELWANDERER: return SL_BARDDANCER;
		case MAPID_SURA: return SL_MONK;
		case MAPID_GENETIC: return SL_ALCHEMIST;
		case MAPID_SHADOW_CHASER: return SL_ROGUE;
		case MAPID_SOUL_REAPER: return SL_SOULLINKER;
		case MAPID_STAR_EMPEROR: return SL_STAR;
		case MAPID_SUPER_NOVICE_E: return SL_SUPERNOVICE;
	}
	switch (eac & MAPID_SECONDMASK) {
		case MAPID_ALCHEMIST: return SL_ALCHEMIST;
		case MAPID_ASSASSIN: return SL_ASSASIN;
		case MAPID_BARDDANCER: return SL_BARDDANCER;
		case MAPID_BLACKSMITH: return SL_BLACKSMITH;
		case MAPID_CRUSADER: return SL_CRUSADER;
		case MAPID_HUNTER: return SL_HUNTER;
		case MAPID_KNIGHT: return SL_KNIGHT;
		case MAPID_MONK: return SL_MONK;
		case MAPID_PRIEST: return SL_PRIEST;
		case MAPID_ROGUE: return SL_ROGUE;
		case MAPID_SAGE: return SL_SAGE;
		case MAPID_SOUL_LINKER: return SL_SOULLINKER;
		case MAPID_STAR_GLADIATOR: return SL_STAR;
		case MAPID_SUPER_NOVICE: return SL_SUPERNOVICE;
		case MAPID_WIZARD: return SL_WIZARD;
	}
	return 0;
}
inline int apply_package(map_session_data& sd, int tier) {
	const auto apply = [&sd](sc_type type, int level, int val2 = 0) {
		status_change_start(&sd, &sd, type, 10000, level, val2, 0, 0, round_ms, SCSTART_NOAVOID);
	};
	apply(SC_INCREASEAGI, 10); apply(SC_BLESSING, 10); apply(SC_ANGELUS, 10);
	if (tier >= 2) { apply(SC_MAGNIFICAT, 5); apply(SC_KYRIE, 10); }
	if (tier >= 3) { apply(SC_IMPOSITIO, 5); apply(SC_ASSUMPTIO, 5); apply(SC_SUFFRAGIUM, 3); }
	if (tier >= 4) { apply(SC_WINDWALK, 10); apply(SC_OVERTHRUST, 5, 1); apply(SC_WEAPONPERFECTION, 5); }
	const int link = tier == 5 ? soul_link(sd) : 0;
	if (link) apply(SC_SPIRIT, 5, link);
	return link;
}
struct PaymentEntry { int index, amount; item snapshot; };
inline bool prepare_payment(map_session_data& sd, int cost, std::vector<PaymentEntry>& entries) {
	return plan_payment(cost, MAX_INVENTORY, [&sd](int i, PaymentEntry& entry) {
		const auto& stack = sd.inventory.u.items_inventory[i];
		if (stack.nameid != 6909) return 0;
		if (stack.amount <= 0 || sd.inventory_data[i] == nullptr || sd.inventory_data[i]->nameid != 6909)
			return -1;
		entry.snapshot = stack;
		return static_cast<int>(stack.amount);
	}, entries);
}
inline bool debit(map_session_data& sd, const std::vector<PaymentEntry>& entries) {
	const auto result = debit_entries(entries,
		[&sd](const PaymentEntry& e) { return pc_delitem(&sd, e.index, e.amount, 0, 0, LOG_TYPE_SCRIPT) == 0; },
		[&sd](const PaymentEntry& e) { item restored = e.snapshot; return pc_additem(&sd, &restored, e.amount, LOG_TYPE_SCRIPT) == ADDITEM_SUCCESS; },
		[&sd](const PaymentEntry& e) { ShowError("[Premi Auto Buff] REFUND FAILED char=%u amount=%d index=%d\n", sd.status.char_id, e.amount, e.index); });
	if (result != Debit::Success)
		ShowError("[Premi Auto Buff] debit failed char=%u tick=%" PRId64 "\n", sd.status.char_id, gettick());
	return result == Debit::Success;
}
inline State read_state(const status_change_entry& sce) { return {sce.val1, sce.val2, sce.val3, sce.val4}; }
inline void write_state(status_change_entry& sce, const State& s) {
	sce.val1 = s.tier; sce.val2 = s.phase; sce.val3 = s.slice; sce.val4 = s.tail;
}
inline void log_event(const char* event, const map_session_data& sd, int tier, int cost) {
	ShowInfo("[Premi Auto Buff] %s char=%u tier=%d cost=%d tick=%" PRId64 "\n", event, sd.status.char_id, tier, cost, gettick());
}
inline bool start_status(map_session_data* sd, int tier, int32& tick, int& val2, int& val3, int& val4, int& tick_time) {
	if (sd == nullptr || eligibility(*sd) != 0 || sd->sc.getSCE(SC_AUTOBUFF) != nullptr
		|| tick <= 0 || tick > max_lifetime_ms) return false;
	State s{tier, round_ms, 0, 0};
	if (!arm(s, tick)) return false;
	val2 = s.phase; val3 = s.slice; val4 = s.tail; tick_time = s.slice;
	return true;
}
inline bool load_status(map_session_data* sd, int tier, int32& tick, int& val2, int& val3, int& val4, int& tick_time) {
	State s{tier, val2, val3, val4};
	if (sd == nullptr || pc_isdead(sd) || !load(s, tick)) return false;
	// Registries may not yet be ready; the timer checks them before any purchase.
	if (sd->vars_ok && !premium(*sd)) return false;
	tick = s.slice + s.tail; tick_time = s.slice;
	val2 = s.phase; val3 = s.slice; val4 = s.tail;
	return true;
}
inline bool prepare_status_save(sc_type type, const status_change_entry& sce, t_tick now, status_change_data& data) {
	if (type != SC_AUTOBUFF) return true;
	const auto* timer = get_timer(sce.timer);
	if (timer == nullptr || timer->func != status_change_timer) return false;
	State s = read_state(sce);
	if (!freeze(s, timer->tick, now)) return false;
	data.val1 = s.tier; data.val2 = s.phase; data.val3 = s.slice; data.val4 = s.tail; data.tick = s.slice;
	return true;
}
inline void log_status_end(block_list* bl, sc_type type, const status_change_entry& sce) {
	if (type == SC_AUTOBUFF && bl->type == BL_PC)
		log_event("end", *static_cast<map_session_data*>(bl), sce.val1, 0);
}
inline bool run_status_timer(map_session_data* sd, status_change_entry& sce) {
	if (sd == nullptr) return false;
	const auto* timer = get_timer(sce.timer);
	if (timer == nullptr || timer->func != status_change_timer) return false;
	const auto now = gettick_nocache();
	const auto deadline = timer->tick;
	State s = read_state(sce);
	std::int64_t lifetime = 0;
	const auto step = advance(s, deadline, now, lifetime);
	if (step == Step::Invalid || step == Step::Expire || pc_isdead(sd)) {
		if (step == Step::Expire) clif_displaymessage(sd->fd, "Auto Buff ended.");
		return false;
	}
	if (sd->vars_ok) {
		if (!premium(*sd)) { clif_displaymessage(sd->fd, "Auto Buff stopped: Premium expired."); return false; }
		if (step == Step::BeginDelay) s.phase = -((5 + rnd() % 6) * 1000 + 1);
		if (step == Step::Deliver) {
			std::vector<PaymentEntry> entries;
			const int cost = package_cost(s.tier);
			if (!prepare_payment(*sd, cost, entries)) {
				clif_displaymessage(sd->fd, "Auto Buff stopped: not enough Nyangvine."); return false;
			}
			// Check the actual deadline again immediately before debit.
			if (eligibility(*sd) != 0) { clif_displaymessage(sd->fd, "Auto Buff stopped: Premium expired."); return false; }
			if (lifetime <= gettick_nocache() - now) { clif_displaymessage(sd->fd, "Auto Buff ended."); return false; }
			if (!debit(*sd, entries)) { clif_displaymessage(sd->fd, "The purchase could not be completed. Please try again."); return false; }
			apply_package(*sd, s.tier);
			// A fresh paid round begins only after delivery; no catch-up purchases.
			const auto delivered = gettick_nocache();
			lifetime -= delivered - now;
			s.phase = round_ms;
			log_event("renew", *sd, s.tier, cost);
			const auto armed = gettick_nocache();
			lifetime -= armed - delivered;
			consume_phase(s, armed - delivered);
			if (!arm(s, lifetime)) { clif_displaymessage(sd->fd, "Auto Buff ended."); return false; }
			write_state(sce, s);
			sce.timer = add_timer(armed + s.slice, status_change_timer, sd->id, SC_AUTOBUFF);
			return true;
		}
	}
	const auto armed = gettick_nocache();
	lifetime -= armed - now;
	consume_phase(s, armed - now);
	if (!arm(s, lifetime)) { clif_displaymessage(sd->fd, "Auto Buff ended."); return false; }
	write_state(sce, s);
	sce.timer = add_timer(armed + s.slice, status_change_timer, sd->id, SC_AUTOBUFF);
	return true;
}
inline int buy(map_session_data& sd, std::int64_t tier, std::int64_t seconds) {
	if (tier < 1 || tier > 5 || opening_cost(seconds) < 0) return -1;
	const int eligible = eligibility(sd);
	if (eligible != 0) return eligible;
	if (sd.sc.getSCE(SC_AUTOBUFF) != nullptr) return -4;
	const int cost = package_cost(static_cast<int>(tier)) + opening_cost(seconds);
	std::vector<PaymentEntry> entries;
	if (!prepare_payment(sd, cost, entries)) return -5;
	if (seconds > 0 && !status_change_start(&sd, &sd, SC_AUTOBUFF, 10000, static_cast<int>(tier), 0, 0, 0,
		seconds * 1000, SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF)) return -6;
	if (seconds > 0) {
		const auto* marker = sd.sc.getSCE(SC_AUTOBUFF);
		const auto* timer = marker == nullptr ? nullptr : get_timer(marker->timer);
		if (timer == nullptr || timer->func != status_change_timer || !valid(read_state(*marker))) {
			status_change_end(&sd, SC_AUTOBUFF);
			return -6;
		}
	}
	const int rechecked = eligibility(sd);
	if (rechecked != 0 || !debit(sd, entries)) {
		if (seconds > 0) status_change_end(&sd, SC_AUTOBUFF);
		return rechecked != 0 ? rechecked : -6;
	}
	const int link = apply_package(sd, static_cast<int>(tier));
	if (seconds > 0) {
		auto* sce = sd.sc.getSCE(SC_AUTOBUFF);
		const auto* timer = sce == nullptr ? nullptr : get_timer(sce->timer);
		if (timer != nullptr) {
			State s = read_state(*sce);
			const auto now = gettick_nocache();
			const auto lifetime = lifetime_left(s, timer->tick, now);
			delete_timer(sce->timer, status_change_timer);
			s.phase = round_ms;
			if (arm(s, lifetime)) {
				write_state(*sce, s);
				sce->timer = add_timer(now + s.slice, status_change_timer, sd.id, SC_AUTOBUFF);
			} else { sce->timer = INVALID_TIMER; status_change_end(&sd, SC_AUTOBUFF); }
		}
		log_event("activate", sd, static_cast<int>(tier), cost);
	}
	return link;
}
} // namespace offline_premi_autobuff
#endif
