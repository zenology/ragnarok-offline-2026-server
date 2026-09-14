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

struct black_market_price_ratio {
	int32_t numerator;
	int32_t denominator;
};

constexpr black_market_price_ratio black_market_normal_price_ratio(int32_t base_level, bool reborn) {
	return { black_market_level_multiplier(base_level, reborn), 1 };
}

inline bool black_market_is_card_shop(const char* shop_name) {
	return std::strncmp(shop_name, "card_mob#", 9) == 0 ||
		std::strncmp(shop_name, "card_miniboss#", 14) == 0 ||
		std::strncmp(shop_name, "card_mvp#", 9) == 0 ||
		std::strncmp(shop_name, "card_special#", 13) == 0;
}

constexpr black_market_price_ratio black_market_card_price_ratio(int32_t base_level, bool reborn) {
	const int32_t normal_multiplier = black_market_level_multiplier(base_level, reborn);
	if (normal_multiplier <= 0)
		return { 0, 1 };
	if (normal_multiplier <= 2)
		return { 1, 1 };
	if (normal_multiplier == 3)
		return { 3, 2 };
	return { normal_multiplier / 2, 1 };
}

constexpr int64_t black_market_player_price_unchecked(int64_t daily_adjusted_price, black_market_price_ratio ratio) {
	return (daily_adjusted_price * ratio.numerator + ratio.denominator / 2) / ratio.denominator;
}

constexpr bool black_market_player_price_is_valid(int64_t daily_adjusted_price, black_market_price_ratio ratio) {
	if (daily_adjusted_price <= 0 || ratio.numerator <= 0 || ratio.denominator <= 0)
		return false;
	const int64_t result = black_market_player_price_unchecked(daily_adjusted_price, ratio);
	return result > 0 && result <= std::numeric_limits<int32_t>::max();
}

constexpr bool black_market_player_price(int64_t daily_adjusted_price, black_market_price_ratio ratio, int32_t& player_price) {
	if (!black_market_player_price_is_valid(daily_adjusted_price, ratio))
		return false;
	const int64_t result = black_market_player_price_unchecked(daily_adjusted_price, ratio);
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
static_assert(black_market_card_price_ratio(1, false).numerator == 1 && black_market_card_price_ratio(1, false).denominator == 1);
static_assert(black_market_card_price_ratio(99, false).numerator == 1 && black_market_card_price_ratio(99, false).denominator == 1);
static_assert(black_market_card_price_ratio(50, true).numerator == 3 && black_market_card_price_ratio(50, true).denominator == 2);
static_assert(black_market_card_price_ratio(99, true).numerator == 2 && black_market_card_price_ratio(99, true).denominator == 1);
static_assert(black_market_card_price_ratio(100, false).numerator == 3 && black_market_card_price_ratio(100, false).denominator == 1);
static_assert(black_market_card_price_ratio(201, false).numerator == 5 && black_market_card_price_ratio(201, false).denominator == 1);
static_assert(black_market_player_price_unchecked(325, { 3, 2 }) == 488);
static_assert(black_market_player_price_is_valid(214748364, { 10, 1 }));
static_assert(!black_market_player_price_is_valid(214748365, { 10, 1 }));

#endif
