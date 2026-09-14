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

inline bool black_market_is_rooke_shop(const char* shop_name) {
	return std::strcmp(shop_name, "cons0") == 0 ||
		std::strcmp(shop_name, "cons1") == 0 ||
		std::strcmp(shop_name, "cons2") == 0 ||
		std::strcmp(shop_name, "cons3") == 0 ||
		std::strcmp(shop_name, "cons4") == 0 ||
		std::strcmp(shop_name, "cons5") == 0 ||
		std::strcmp(shop_name, "cons6") == 0;
}

constexpr int32_t black_market_level_multiplier(int32_t base_level, bool reborn) {
	if (base_level <= 0)
		return 0;
	if (base_level <= 50)
		return reborn ? 3 : 1;
	if (base_level <= 99)
		return reborn ? 4 : 2;
	if (base_level <= 200)
		return 6;
	return 10;
}

constexpr int64_t black_market_player_price_unchecked(int64_t daily_adjusted_price, int32_t multiplier) {
	return daily_adjusted_price * multiplier;
}

constexpr bool black_market_player_price_is_valid(int64_t daily_adjusted_price, int32_t multiplier) {
	if (daily_adjusted_price <= 0 || multiplier <= 0)
		return false;
	const int64_t result = black_market_player_price_unchecked(daily_adjusted_price, multiplier);
	return result > 0 && result <= std::numeric_limits<int32_t>::max();
}

constexpr bool black_market_player_price(int64_t daily_adjusted_price, int32_t multiplier, int32_t& player_price) {
	if (!black_market_player_price_is_valid(daily_adjusted_price, multiplier))
		return false;
	const int64_t result = black_market_player_price_unchecked(daily_adjusted_price, multiplier);
	player_price = static_cast<int32_t>(result);
	return true;
}

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
static_assert(black_market_level_multiplier(1, false) == 1);
static_assert(black_market_level_multiplier(50, true) == 3);
static_assert(black_market_level_multiplier(51, false) == 2);
static_assert(black_market_level_multiplier(99, true) == 4);
static_assert(black_market_level_multiplier(100, false) == 6);
static_assert(black_market_level_multiplier(200, true) == 6);
static_assert(black_market_level_multiplier(201, false) == 10);
static_assert(black_market_player_price_is_valid(214748364, 10));
static_assert(!black_market_player_price_is_valid(214748365, 10));

#endif
