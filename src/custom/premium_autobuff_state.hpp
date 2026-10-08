#ifndef OFFLINE_PREMI_AUTOBUFF_STATE_HPP
#define OFFLINE_PREMI_AUTOBUFF_STATE_HPP

#include <algorithm>
#include <cstdint>

namespace offline_premi_autobuff {
constexpr std::int32_t round_ms = 300000;
constexpr std::int32_t max_lifetime_ms = 10800000;
struct State {
	std::int32_t tier, phase, slice, tail;
};
enum class Step { Invalid, Expire, Wait, BeginDelay, Deliver };
inline bool valid(const State& s) {
	return s.tier >= 1 && s.tier <= 5 && s.phase >= -10001 && s.phase <= round_ms
		&& s.slice >= 0 && s.slice <= 1000 && s.tail >= 0
		&& std::int64_t(s.slice) + s.tail <= max_lifetime_ms;
}
inline std::int32_t phase_left(const State& s) {
	return s.phase < 0 ? -s.phase - 1 : s.phase;
}
inline void consume_phase(State& s, std::int64_t elapsed) {
	const auto left = static_cast<std::int32_t>(std::max<std::int64_t>(0, phase_left(s) - elapsed));
	s.phase = s.phase < 0 ? -(left + 1) : left;
}
// deadline is the actual native timer deadline, never the callback's tick argument.
inline std::int64_t lifetime_left(const State& s, std::int64_t deadline, std::int64_t now) {
	return std::min<std::int64_t>(std::int64_t(s.slice) + s.tail, s.tail + deadline - now);
}
inline bool arm(State& s, std::int64_t lifetime) {
	if (!valid(s) || lifetime <= 0 || lifetime > max_lifetime_ms)
		return false;
	s.slice = static_cast<std::int32_t>(std::min<std::int64_t>({1000, lifetime, std::max(1, phase_left(s))}));
	s.tail = static_cast<std::int32_t>(lifetime - s.slice);
	return true;
}
inline Step advance(State& s, std::int64_t deadline, std::int64_t now, std::int64_t& lifetime) {
	if (!valid(s)) return Step::Invalid;
	lifetime = lifetime_left(s, deadline, now);
	if (lifetime <= 0) return Step::Expire;
	consume_phase(s, std::max<std::int64_t>(0, s.slice + now - deadline));
	if (phase_left(s) > 0) return Step::Wait;
	return s.phase < 0 ? Step::Deliver : Step::BeginDelay;
}
// Freeze only a copy. Overdue boundaries are saved as zero and processed after load.
inline bool freeze(State& s, std::int64_t deadline, std::int64_t now) {
	if (!valid(s)) return false;
	const auto lifetime = lifetime_left(s, deadline, now);
	if (lifetime <= 0) return false;
	consume_phase(s, std::max<std::int64_t>(0, s.slice + now - deadline));
	s.slice = static_cast<std::int32_t>(std::clamp<std::int64_t>(deadline - now, 0, s.slice));
	s.tail = static_cast<std::int32_t>(lifetime - s.slice);
	return true;
}
inline bool load(State& s, std::int64_t tick) {
	if (!valid(s) || tick != s.slice || tick + s.tail <= 0) return false;
	if (s.slice == 0) { s.slice = 1; --s.tail; }
	return valid(s);
}
inline int package_cost(int tier) {
	constexpr int costs[] = {0, 2, 3, 5, 7, 10};
	return tier >= 1 && tier <= 5 ? costs[tier] : -1;
}
inline int opening_cost(std::int64_t seconds) {
	switch (seconds) {
		case 0: return 0;
		case 1800: return 20;
		case 3600: return 40;
		case 10800: return 80;
		default: return -1;
	}
}
} // namespace offline_premi_autobuff
#endif
