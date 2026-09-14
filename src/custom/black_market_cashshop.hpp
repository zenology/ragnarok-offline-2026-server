// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Black Market daily-rate support. Included only by custom/script.inc.

#ifndef BLACK_MARKET_CASHSHOP_HPP
#define BLACK_MARKET_CASHSHOP_HPP

#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct black_market_catalog_cache {
	std::vector<t_itemid> item_ids;
	std::vector<uint32> base_prices;
	std::vector<uint32> last_applied_prices;
};

inline std::unordered_map<std::string, black_market_catalog_cache> black_market_catalog_cache_by_shop;

// Permanent character registry used by every Black Market pointshop and
// service. Do not change this name without migrating the player-facing
// currency contract.
inline constexpr char BLACK_MARKET_POINT_VAR[] = "BlackMarketPoints";

constexpr int64_t black_market_adjusted_price_unchecked(int64_t base_price, int32_t rate) {
	return (base_price * (100 + rate) + 50) / 100;
}

constexpr bool black_market_adjusted_price(int64_t base_price, int32_t rate, int32_t& adjusted_price) {
	if (base_price <= 0 || rate < -10 || rate > 30)
		return false;
	const int64_t result = black_market_adjusted_price_unchecked(base_price, rate);
	if (result <= 0 || result > std::numeric_limits<int32_t>::max())
		return false;
	adjusted_price = static_cast<int32_t>(result);
	return true;
}

static_assert(black_market_adjusted_price_unchecked(100, -10) == 90);
static_assert(black_market_adjusted_price_unchecked(100, 0) == 100);
static_assert(black_market_adjusted_price_unchecked(100, 30) == 130);
static_assert(black_market_adjusted_price_unchecked(5, 10) == 6);
static_assert(black_market_adjusted_price_unchecked(1, -10) == 1);

#endif
