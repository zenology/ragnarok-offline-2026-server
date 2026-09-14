// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

#include "black_market_cashshop.hpp"

// Result codes: 1=success, 2=selected item changed, 3=insufficient Black Market Points,
// 4=invalid generated/script data. The NPC owns the roll; this command owns the
// atomic validation, character-point debit, mutation, client refresh, and re-equip.
BUILDIN_FUNC(blackmarketenhance)
{
	TBL_PC* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_FAILURE;
	}

	const int32 slot = script_getnum(st, 2);
	if (slot != EQI_HEAD_TOP && slot != EQI_HEAD_MID && slot != EQI_HEAD_LOW &&
		slot != EQI_ARMOR && slot != EQI_HAND_L && slot != EQI_HAND_R &&
		slot != EQI_GARMENT && slot != EQI_SHOES && slot != EQI_ACC_L && slot != EQI_ACC_R) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}

	char* end = nullptr;
	const uint64 expected_unique_id = std::strtoull(script_getstr(st, 3), &end, 10);
	if (end == script_getstr(st, 3) || *end != '\0') {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}

	struct item replacement{};
	if (!script_getitem_randomoption(st, sd, &replacement, "blackmarketenhance", 4)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}

	for (int32 argument = 4; argument <= 6; ++argument) {
		struct script_data* data = script_getdata(st, argument);
		const char* name = reference_getname(data);
		if (!data_isreference(data) || !script_array_src(st, sd, name, reference_getref(data)) ||
			script_array_highest_key(st, sd, name, reference_getref(data)) > MAX_ITEM_RDM_OPT) {
			ShowError("buildin_blackmarketenhance: option arrays contain data beyond slot %d.\n", MAX_ITEM_RDM_OPT - 1);
			script_pushint(st, 4);
			return SCRIPT_CMD_SUCCESS;
		}
	}

	for (int32 option_index = 0; option_index < MAX_ITEM_RDM_OPT; ++option_index) {
		const auto& option = replacement.option[option_index];
		if (option.id == 0) {
			if (option.value != 0 || option.param != 0) {
				script_pushint(st, 4);
				return SCRIPT_CMD_SUCCESS;
			}
			continue;
		}
		if (random_option_db.find(static_cast<uint16>(option.id)) == nullptr) {
			ShowError("buildin_blackmarketenhance: random option ID %d does not exist.\n", option.id);
			script_pushint(st, 4);
			return SCRIPT_CMD_SUCCESS;
		}
		for (int32 prior = 0; prior < option_index; ++prior) {
			if (replacement.option[prior].id == option.id) {
				ShowError("buildin_blackmarketenhance: duplicate random option ID %d.\n", option.id);
				script_pushint(st, 4);
				return SCRIPT_CMD_SUCCESS;
			}
		}
	}

	const int16 inventory_index = pc_checkequip(sd, equip_bitmask[slot]);
	if (inventory_index < 0 || !sd->inventory_data[inventory_index] ||
		sd->inventory.u.items_inventory[inventory_index].unique_id != expected_unique_id) {
		script_pushint(st, 2);
		return SCRIPT_CMD_SUCCESS;
	}
	const item_data* item_data = sd->inventory_data[inventory_index];
	if (item_data->type != IT_WEAPON && item_data->type != IT_ARMOR) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}
	if ((item_data->type == IT_WEAPON && slot != EQI_HAND_R && slot != EQI_HAND_L) ||
		(item_data->type == IT_ARMOR && slot == EQI_HAND_R)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}

	bool has_option = false;
	for (const auto& option : sd->inventory.u.items_inventory[inventory_index].option)
		has_option = has_option || option.id != 0;
	const int32 price = has_option ? 2000 : 1000;
	const int32 black_market_points = static_cast<int32>(pc_readreg2(sd, BLACK_MARKET_POINT_VAR));
	if (black_market_points < price ||
		!set_reg_num(nullptr, sd, add_str(BLACK_MARKET_POINT_VAR), BLACK_MARKET_POINT_VAR,
			black_market_points - price, nullptr)) {
		script_pushint(st, 3);
		return SCRIPT_CMD_SUCCESS;
	}

	const uint32 equip_mask = sd->inventory.u.items_inventory[inventory_index].equip;
	log_pick_pc(sd, LOG_TYPE_SCRIPT, -1, &sd->inventory.u.items_inventory[inventory_index]);
	pc_unequipitem(sd, inventory_index, 2);
	for (int32 option_index = 0; option_index < MAX_ITEM_RDM_OPT; ++option_index)
		sd->inventory.u.items_inventory[inventory_index].option[option_index] = replacement.option[option_index];
	clif_delitem(*sd, inventory_index, 1, 3);
	log_pick_pc(sd, LOG_TYPE_SCRIPT, -1, &sd->inventory.u.items_inventory[inventory_index]);
	clif_additem(sd, inventory_index, 1, 0);
	if (!pc_equipitem(sd, inventory_index, equip_mask))
		ShowWarning("buildin_blackmarketenhance: changed item %u could not be re-equipped (CID=%d/AID=%d).\n", sd->inventory.u.items_inventory[inventory_index].nameid, sd->status.char_id, sd->status.account_id);
	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(callblackmarketshop)
{
	const char *shopname = script_getstr(st, 2);
	const int32 flag = script_getnum(st, 3);
	const int32 rate = script_getnum(st, 4);
	TBL_PC *sd = nullptr;
	if (!script_rid2sd(sd)) {
		ShowError("buildin_callblackmarketshop: Shop [%s] has no attached player RID (rate %d).\n", shopname, rate);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}

	npc_data *nd = npc_name2id(shopname);
	if (!nd || nd->type != BL_NPC || nd->subtype != NPCTYPE_POINTSHOP) {
		ShowError("buildin_callblackmarketshop: Shop [%s] is not a pointshop (rate %d).\n", shopname, rate);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}
	if (std::strcmp(nd->u.shop.pointshop_str, BLACK_MARKET_POINT_VAR) != 0) {
		ShowError("buildin_callblackmarketshop: Shop [%s] does not use %s (rate %d).\n", shopname, BLACK_MARKET_POINT_VAR, rate);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}
	if (rate < -10 || rate > 30) {
		ShowError("buildin_callblackmarketshop: Shop [%s] has invalid daily rate %d.\n", shopname, rate);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}
	if (nd->u.shop.count > 0 && !nd->u.shop.shop_item) {
		ShowError("buildin_callblackmarketshop: Shop [%s] has no item catalog (rate %d).\n", shopname, rate);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}

	auto &cache = black_market_catalog_cache_by_shop[shopname];
	bool recapture = cache.item_ids.size() != nd->u.shop.count ||
		cache.base_prices.size() != nd->u.shop.count ||
		cache.last_applied_prices.size() != nd->u.shop.count;
	if (!recapture) {
		for (uint16 i = 0; i < nd->u.shop.count; ++i) {
			const auto &item = nd->u.shop.shop_item[i];
			if (cache.item_ids[i] != item.nameid || cache.last_applied_prices[i] != item.value) {
				recapture = true;
				break;
			}
		}
	}
	if (recapture) {
		black_market_catalog_cache replacement;
		replacement.item_ids.reserve(nd->u.shop.count);
		replacement.base_prices.reserve(nd->u.shop.count);
		replacement.last_applied_prices.reserve(nd->u.shop.count);
		for (uint16 i = 0; i < nd->u.shop.count; ++i) {
			const auto &item = nd->u.shop.shop_item[i];
			if (item.value == 0 || item.value > static_cast<uint32>(std::numeric_limits<int32_t>::max())) {
				ShowError("buildin_callblackmarketshop: Shop [%s] has invalid base price for item %u (rate %d).\n", shopname, item.nameid, rate);
				script_pushint(st, 0);
				return SCRIPT_CMD_FAILURE;
			}
			replacement.item_ids.push_back(item.nameid);
			replacement.base_prices.push_back(item.value);
			replacement.last_applied_prices.push_back(item.value);
		}
		cache = std::move(replacement);
	}

	std::vector<uint32> adjusted_prices;
	adjusted_prices.reserve(cache.base_prices.size());
	for (uint32 base_price : cache.base_prices) {
		int32 adjusted_price = 0;
		if (!black_market_adjusted_price(base_price, rate, adjusted_price)) {
			ShowError("buildin_callblackmarketshop: Shop [%s] cannot adjust base price %u at rate %d.\n", shopname, base_price, rate);
			script_pushint(st, 0);
			return SCRIPT_CMD_FAILURE;
		}
		adjusted_prices.push_back(static_cast<uint32>(adjusted_price));
	}
	for (uint16 i = 0; i < nd->u.shop.count; ++i)
		nd->u.shop.shop_item[i].value = adjusted_prices[i];
	cache.last_applied_prices = adjusted_prices;

	return buildin_callshop(st);
}
