// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Card calibration of the shared Equipment curve, with two uncapped tangents.
#ifndef BLACK_MARKET_CARD_PRICING_HPP
#define BLACK_MARKET_CARD_PRICING_HPP

#include <array>
#include "black_market_pricing.hpp"

namespace bmc {
struct CardCurve {
	int64_t raw_low = 0, raw_high = 0, price_low = 0, price_high = 0;
};
struct CardPricingSettings {
	// Normal, Miniboss, MVP. Special deliberately shares the MVP entry.
	std::array<CardCurve, 3> curves{};
	bool valid = false;
};
inline bool derive_card_pricing_settings(CardPricingSettings& settings, std::string& reason) {
	settings.valid = false;
	const int64_t limit = std::numeric_limits<int32_t>::max();
	for (const auto& c : settings.curves) {
		if (!(0 <= c.raw_low && c.raw_low < c.raw_high && c.raw_high <= limit &&
			0 < c.price_low && c.price_low < c.price_high && c.price_high <= limit)) {
			reason = "card curves require 0 <= RawLow < RawHigh <= INT32_MAX and 0 < PriceLow < PriceHigh <= INT32_MAX";
			return false;
		}
	}
	settings.valid = true;
	reason.clear();
	return true;
}
inline bool assess_card_raw_bp(const PricingSettings& shape, const CardPricingSettings& cards,
	size_t group, int64_t raw_bp, int32_t& out_assessed) {
	if (!shape.valid || !cards.valid || group >= cards.curves.size() ||
		raw_bp <= 0 || raw_bp > std::numeric_limits<int32_t>::max()) return false;
	const auto& c = cards.curves[group];
	const double fraction = static_cast<double>(raw_bp - c.raw_low) / (c.raw_high - c.raw_low);
	const double x = fraction * shape.p3x;
	double y;
	if (fraction < 0.0) {
		y = x * shape.p1y / shape.p1x;
	} else if (fraction >= 1.0) {
		y = shape.p3y + (x - shape.p3x) * shape.slope;
	} else {
		double lo = 0.0, hi = 1.0;
		for (int i = 0; i < 64; ++i) {
			const double t = (lo + hi) / 2.0;
			if (bezier_coordinate(t, shape.p1x, shape.p2x, shape.p3x) < x) lo = t;
			else hi = t;
		}
		y = bezier_coordinate((lo + hi) / 2.0, shape.p1y, shape.p2y, shape.p3y);
	}
	const double price = c.price_low + (c.price_high - c.price_low) * (y / shape.p3y);
	const double rounded = std::floor(price + 0.5);
	// These are storage/transaction limits, not clamps to the price guides.
	if (!std::isfinite(rounded) || rounded <= 0 || rounded > std::numeric_limits<int32_t>::max()) return false;
	out_assessed = static_cast<int32_t>(rounded);
	return true;
}
} // namespace bmc
#endif
