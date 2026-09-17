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
#include "silvervine_enchant.hpp"
#include "silvervine_enchant_upgrade.hpp"

BUILDIN_FUNC(blackmarketprice)
{
	TBL_PC* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}

	int32 price = 0;
	if (!black_market_player_price(script_getnum(st, 2),
		black_market_normal_price_ratio(sd->status.base_level, (sd->class_ & JOBL_UPPER) != 0), price)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, price);
	return SCRIPT_CMD_SUCCESS;
}

// Card Magnet is a Premi entitlement, not a new status-change enum.  The NPC
// owns the account expiry; this command owns only the current session's card
// autoloot bit and its matching client-side timed icon.
BUILDIN_FUNC(premiumcardloot)
{
	TBL_PC* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}

	const int32 enable = script_getnum(st, 2);
	const int64 remaining_ms = script_getnum(st, 3);
	if ((enable != 0 && enable != 1) || remaining_ms < 0 || remaining_ms > 86400000 ||
		(enable == 1 && remaining_ms == 0) || (enable == 0 && remaining_ms != 0)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	constexpr uint16 card_autoloot_mask = (1 << IT_CARD);
	if (enable == 1) {
		sd->state.autoloottype |= card_autoloot_mask;
		clif_status_change(sd, EFST_AID_PERIOD_ADDSTOREITEMCOUNT, 1,
			static_cast<t_tick>(remaining_ms), 0, 0, 0);
	} else {
		sd->state.autoloottype &= ~card_autoloot_mask;
		clif_status_change(sd, EFST_AID_PERIOD_ADDSTOREITEMCOUNT, 0, 0, 0, 0, 0);
	}

	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

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
	int32 price = 0;
	if (!black_market_player_price(has_option ? 2000 : 1000,
		black_market_normal_price_ratio(sd->status.base_level, (sd->class_ & JOBL_UPPER) != 0), price)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}
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

// Result codes: 1=success, 2=selected item changed, 4=invalid generated/script data.
// The NPC owns the roll and fruit debit; this command owns unique-id validation,
// in-place card/option mutation, client refresh, and re-equip on the saved mask.
BUILDIN_FUNC(replaceequipped)
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
	if (!script_getitem_randomoption(st, sd, &replacement, "replaceequipped", 8)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}

	for (int32 argument = 8; argument <= 10; ++argument) {
		struct script_data* data = script_getdata(st, argument);
		const char* name = reference_getname(data);
		if (!data_isreference(data) || !script_array_src(st, sd, name, reference_getref(data)) ||
			script_array_highest_key(st, sd, name, reference_getref(data)) > MAX_ITEM_RDM_OPT) {
			ShowError("buildin_replaceequipped: option arrays contain data beyond slot %d.\n", MAX_ITEM_RDM_OPT - 1);
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
			ShowError("buildin_replaceequipped: random option ID %d does not exist.\n", option.id);
			script_pushint(st, 4);
			return SCRIPT_CMD_SUCCESS;
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

	const uint32 equip_mask = sd->inventory.u.items_inventory[inventory_index].equip;
	log_pick_pc(sd, LOG_TYPE_SCRIPT, -1, &sd->inventory.u.items_inventory[inventory_index]);
	pc_unequipitem(sd, inventory_index, 2);
	sd->inventory.u.items_inventory[inventory_index].card[0] = static_cast<t_itemid>(script_getnum(st, 4));
	sd->inventory.u.items_inventory[inventory_index].card[1] = static_cast<t_itemid>(script_getnum(st, 5));
	sd->inventory.u.items_inventory[inventory_index].card[2] = static_cast<t_itemid>(script_getnum(st, 6));
	sd->inventory.u.items_inventory[inventory_index].card[3] = static_cast<t_itemid>(script_getnum(st, 7));
	for (int32 option_index = 0; option_index < MAX_ITEM_RDM_OPT; ++option_index)
		sd->inventory.u.items_inventory[inventory_index].option[option_index] = replacement.option[option_index];
	clif_delitem(*sd, inventory_index, 1, 3);
	log_pick_pc(sd, LOG_TYPE_SCRIPT, -1, &sd->inventory.u.items_inventory[inventory_index]);
	clif_additem(sd, inventory_index, 1, 0);
	if (!pc_equipitem(sd, inventory_index, equip_mask))
		ShowWarning("buildin_replaceequipped: changed item %u could not be re-equipped (CID=%d/AID=%d).\n", sd->inventory.u.items_inventory[inventory_index].nameid, sd->status.char_id, sd->status.account_id);
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

BUILDIN_FUNC(sve_load)
{
	silvervine_enchant_db.clear();
	const bool file_ok = silvervine_enchant_db.load();
	if (!file_ok) {
		silvervine_enchant_db.clear();
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}
	if (silvervine_enchant_db.empty()) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(silvervine_enchant_db.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_mode)
{
	const int32 item_id = script_getnum(st, 2);
	if (item_id < 1) {
		script_pushint(st, SVE_MODE_NONE);
		return SCRIPT_CMD_SUCCESS;
	}

	auto pool = silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	script_pushint(st, pool == nullptr ? SVE_MODE_NONE : pool->mode);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_stepcount)
{
	const int32 item_id = script_getnum(st, 2);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->steps.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_stepcard)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 ui_step = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD || ui_step < 1 || ui_step > static_cast<int32>(pool->steps.size())) {
		script_pushint(st, -1);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, pool->steps[ui_step - 1].card_index);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_findstep)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD) {
		script_pushint(st, -1);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, sve_find_step_index(*pool, card_index));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_poolcount)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const int32 step_index = sve_find_step_index(*pool, card_index);
	if (step_index < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->steps[step_index].enchants.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_poolitem)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	const int32 n = script_getnum(st, 4);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD || n < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const int32 step_index = sve_find_step_index(*pool, card_index);
	if (step_index < 0 || n >= static_cast<int32>(pool->steps[step_index].enchants.size())) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->steps[step_index].enchants[n].item_id));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_rollcharm)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_CARD) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const int32 step_index = sve_find_step_index(*pool, card_index);
	if (step_index < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, sve_roll_charm(pool->steps[step_index]));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_rogroupcount)
{
	const int32 item_id = script_getnum(st, 2);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || pool->mode != SVE_MODE_RO) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->ro_groups.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_rollro)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 want = script_getnum(st, 3);
	int32 out_id[MAX_ITEM_RDM_OPT];
	int32 out_val[MAX_ITEM_RDM_OPT];
	int32 out_param[MAX_ITEM_RDM_OPT];

	if (item_id < 1 || !sve_roll_ro(static_cast<t_itemid>(item_id), want, out_id, out_val, out_param)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	for (int32 i = 0; i < MAX_ITEM_RDM_OPT; ++i) {
		setd_sub_num(st, nullptr, ".@sve_opt_id", i, out_id[i], nullptr);
		setd_sub_num(st, nullptr, ".@sve_opt_value", i, out_val[i], nullptr);
		setd_sub_num(st, nullptr, ".@sve_opt_param", i, out_param[i], nullptr);
	}

	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_load)
{
	silvervine_enchant_upgrade_db.clear();
	const bool file_ok = silvervine_enchant_upgrade_db.load();
	if (!file_ok) {
		silvervine_enchant_upgrade_db.clear();
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}
	if (silvervine_enchant_upgrade_db.empty()) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(silvervine_enchant_upgrade_db.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_slotcount)
{
	const int32 item_id = script_getnum(st, 2);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->slots.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_slotcard)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 ui_slot = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || ui_slot < 1 || ui_slot > static_cast<int32>(pool->slots.size())) {
		script_pushint(st, -1);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, pool->slots[ui_slot - 1].card_index);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_levelcount)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const int32 slot_index = svu_find_slot_index(*pool, card_index);
	if (slot_index < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	if (pool->slots[slot_index].series.empty()) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->slots[slot_index].series[0].levels.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_levelitem)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	const int32 n = script_getnum(st, 4);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || n < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const int32 slot_index = svu_find_slot_index(*pool, card_index);
	if (slot_index < 0 || pool->slots[slot_index].series.empty() || n >= static_cast<int32>(pool->slots[slot_index].series[0].levels.size())) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->slots[slot_index].series[0].levels[n]));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_findlevel)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	const int32 charm_id = script_getnum(st, 4);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || charm_id < 1) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, svu_find_level(*pool, card_index, static_cast<t_itemid>(charm_id)));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_roll)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	const int32 charm_id = script_getnum(st, 4);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || charm_id < 1) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, svu_roll_level(*pool, card_index, static_cast<t_itemid>(charm_id)));
	return SCRIPT_CMD_SUCCESS;
}
