// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Pure Black Market curve math; no map-server or NPC state.
#ifndef BLACK_MARKET_PRICING_HPP
#define BLACK_MARKET_PRICING_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>

namespace bmc {
struct PricingSettings {
	int64_t p1x = 0, p1y = 0, p2x = 0, p3x = 0;
	int64_t anchor_raw_bp = 0, anchor_assessed_bp = 0;
	int64_t tail_angle_degrees = 0, celestine_markup_percent = 0;
	double slope = 0, p2y = 0, p3y = 0;
	bool valid = false;
};

inline bool derive_pricing_settings(PricingSettings& s, std::string& reason) {
	s.valid = false;
	const int64_t limit = std::numeric_limits<int32_t>::max();
	for (auto value : {s.p1x, s.p1y, s.p2x, s.p3x, s.anchor_raw_bp, s.anchor_assessed_bp}) {
		if (value <= 0 || value > limit) { reason = "BP/control coordinates must be in 1..INT32_MAX"; return false; }
	}
	if (s.celestine_markup_percent < 0 || s.celestine_markup_percent > limit) {
		reason = "CelestineMarkupPercent must be in 0..INT32_MAX"; return false;
	}
	if (s.tail_angle_degrees < 0 || s.tail_angle_degrees > 89) {
		reason = "TailAngleDegrees must be in 0..89"; return false;
	}
	if (!(s.p1x <= s.p2x && s.p2x < s.p3x && s.p3x <= s.anchor_raw_bp)) {
		reason = "requires 0 < P1X <= P2X < P3X <= AnchorRawBP"; return false;
	}
	s.slope = std::tan(static_cast<double>(s.tail_angle_degrees) * std::acos(-1.0) / 180.0);
	s.p3y = static_cast<double>(s.anchor_assessed_bp) - (s.anchor_raw_bp - s.p3x) * s.slope;
	s.p2y = s.p3y - (s.p3x - s.p2x) * s.slope;
	if (!std::isfinite(s.slope) || !std::isfinite(s.p2y) || !std::isfinite(s.p3y) ||
		!(s.p1y <= s.p2y && s.p2y <= s.p3y)) {
		reason = "derived controls require finite 0 < P1Y <= P2Y <= P3Y"; return false;
	}
	s.valid = true;
	reason.clear();
	return true;
}

inline double bezier_coordinate(double t, double p1, double p2, double p3) {
	const double u = 1.0 - t;
	return 3.0 * u * u * t * p1 + 3.0 * u * t * t * p2 + t * t * t * p3;
}

inline bool assess_raw_bp(const PricingSettings& s, int64_t raw_bp, int32_t& out_assessed) {
	if (!s.valid || raw_bp <= 0 || raw_bp > std::numeric_limits<int32_t>::max()) return false;
	double y;
	if (raw_bp >= s.p3x) {
		y = static_cast<double>(s.anchor_assessed_bp) + (raw_bp - s.anchor_raw_bp) * s.slope;
	} else {
		double lo = 0.0, hi = 1.0;
		for (int i = 0; i < 64; ++i) {
			const double mid = (lo + hi) / 2.0;
			if (bezier_coordinate(mid, s.p1x, s.p2x, s.p3x) < raw_bp) lo = mid;
			else hi = mid;
		}
		y = bezier_coordinate((lo + hi) / 2.0, s.p1y, s.p2y, s.p3y);
	}
	const double rounded = std::floor(y + 0.5);
	if (!std::isfinite(rounded) || rounded <= 0 || rounded > std::numeric_limits<int32_t>::max()) return false;
	out_assessed = static_cast<int32_t>(rounded);
	return true;
}

inline bool apply_markup(int64_t daily_bp, int64_t markup_percent, int32_t& out_final) {
	const int64_t limit = std::numeric_limits<int32_t>::max();
	if (daily_bp <= 0 || daily_bp > limit || markup_percent < 0 || markup_percent > limit) return false;
	const int64_t result = (daily_bp * (100LL + markup_percent) + 50LL) / 100LL;
	if (result <= 0 || result > limit) return false;
	out_final = static_cast<int32_t>(result);
	return true;
}
} // namespace bmc
#endif
