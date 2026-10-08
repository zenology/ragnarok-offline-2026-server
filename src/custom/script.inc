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
#include "black_market_catalog.hpp"
#include "silvervine_enchant.hpp"
#include "silvervine_enchant_upgrade.hpp"
#include "premium_service.hpp"
#include "premium_autobuff.hpp"

BUILDIN_FUNC(premibuffbuy) {
	map_session_data* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, -6);
		return SCRIPT_CMD_FAILURE;
	}
	script_pushint(st, offline_premi_autobuff::buy(*sd, script_getnum64(st, 2), script_getnum64(st, 3)));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(premisummon) {
	map_session_data* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushstrcopy(st, "");
		return SCRIPT_CMD_FAILURE;
	}
	npc_data* duplicate = offline_premi::summon(*sd);
	script_pushstrcopy(st, duplicate == nullptr ? "" : duplicate->exname);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(premiumopenstorage) {
	map_session_data* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}
	script_pushint(st, storage_storageopen(sd) == 0 ? 1 : 0);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bmc_load) {
	script_pushint(st, bmc::load());
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_ready) {
	script_pushint(st, bmc::ready(script_getstr(st, 2)));
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_categorycount) {
	std::string key = script_getstr(st, 2);
	script_pushint(st, bmc::ready(key) && key != "riven" ? bmc::catalogs[key].drawers.size() : 0);
	return SCRIPT_CMD_SUCCESS;
}
static int32 bmc_category_at(const std::string& key, int64 position) {
	if (!bmc::ready(key) || key == "riven" || position < 0) return -1;
	auto& categories = bmc::catalogs[key].drawers;
	if (static_cast<uint64>(position) >= categories.size()) return -1;
	auto it = categories.begin(); std::advance(it, position);
	return it->first;
}
BUILDIN_FUNC(bmc_categorykey) {
	script_pushint(st, bmc_category_at(script_getstr(st, 2), script_getnum64(st, 3)));
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_categorylabel) {
	std::string key = script_getstr(st, 2);
	int32 category = bmc_category_at(key, script_getnum64(st, 3));
	auto p = bmc::profile_for(key);
	script_pushstrcopy(st, category >= 0 && p ? p->labels[category].c_str() : "");
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_drawercount) {
	auto rows = bmc::drawers(script_getstr(st, 2), script_getnum64(st, 3));
	script_pushint(st, rows ? rows->size() : 0);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_drawerlabel) {
	auto rows = bmc::drawers(script_getstr(st, 2), script_getnum64(st, 3));
	int64 index = script_getnum64(st, 4);
	script_pushstrcopy(st, rows && index >= 0 && static_cast<uint64>(index) < rows->size() ? (*rows)[index].label.c_str() : "");
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_drawershop) {
	auto rows = bmc::drawers(script_getstr(st, 2), script_getnum64(st, 3));
	int64 index = script_getnum64(st, 4);
	script_pushstrcopy(st, rows && index >= 0 && static_cast<uint64>(index) < rows->size() ? (*rows)[index].shop.c_str() : "");
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_shopcount) {
	const char* name = script_getstr(st, 2);
	auto nd = bmc::shop(name);
	script_pushint(st, nd && bmc::managed_ready(name) ? nd->u.shop.count : 0);
	return SCRIPT_CMD_SUCCESS;
}
// Resolve numeric arrays using the same registry/ref rules as native setarray.
static bool bmc_array_ref(script_state* st, int argument, script_data*& data, TBL_PC*& sd) {
	data = script_getdata(st, argument);
	if (!data_isreference(data)) return false;
	const char* name = reference_getname(data);
	if (is_string_variable(name)) return false;
	if (not_server_variable(*name) && !script_rid2sd(sd)) return false;
	return script_array_src(st, sd, name, reference_getref(data)) != nullptr;
}
BUILDIN_FUNC(bmc_search) {
	script_data* data = nullptr; TBL_PC* sd = nullptr;
	if (!bmc_array_ref(st, 3, data, sd)) { script_pushint(st, -1); return SCRIPT_CMD_FAILURE; }
	const char* name = reference_getname(data);
	uint32 start = reference_getindex(data), id = reference_getid(data);
	if (start >= SCRIPT_MAX_ARRAYSIZE) { script_pushint(st, -1); return SCRIPT_CMD_FAILURE; }
	uint32 end = script_array_highest_key(st, sd, name, reference_getref(data));
	for (uint32 i = start; i < end; ++i) clear_reg(st, sd, reference_uid(id, i), name, reference_getref(data));
	std::vector<t_itemid> result;
	int count = bmc::search(script_getstr(st, 2), result);
	if (count >= 0 && result.size() > SCRIPT_MAX_ARRAYSIZE - start) { script_pushint(st, -1); return SCRIPT_CMD_FAILURE; }
	for (size_t i = 0; i < result.size(); ++i) set_reg_num(st, sd, reference_uid(id, start + i), name, result[i], reference_getref(data));
	script_pushint(st, count);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_searchprepare) {
	script_data* data = nullptr; TBL_PC* sd = nullptr;
	if (!bmc_array_ref(st, 2, data, sd)) { bmc::clear_shop("Riven_SearchShop"); script_pushint(st, 0); return SCRIPT_CMD_FAILURE; }
	const char* name = reference_getname(data);
	uint32 end = script_array_highest_key(st, sd, name, reference_getref(data));
	std::vector<t_itemid> ids;
	for (uint32 i = reference_getindex(data); i < end; ++i) {
		int64 value = get_val2_num(st, reference_uid(reference_getid(data), i), reference_getref(data));
		if (value <= 0 || value > INT32_MAX) { bmc::clear_shop("Riven_SearchShop"); script_pushint(st, 0); return SCRIPT_CMD_SUCCESS; }
		ids.push_back(static_cast<t_itemid>(value));
	}
	script_pushint(st, bmc::search_prepare(ids));
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_cardoffer) {
	int64 id = script_getnum64(st, 2);
	auto row = id > 0 && id <= INT32_MAX ? bmc::card(static_cast<t_itemid>(id)) : nullptr;
	script_pushint(st, row ? row->offer : 0);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_cardtier) {
	int64 id = script_getnum64(st, 2); int tier = 0;
	if (id > 0 && id <= INT32_MAX) bmc::card(static_cast<t_itemid>(id), &tier);
	script_pushint(st, tier);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_mvpcount) {
	script_pushint(st, bmc::ready("card-mvp") ? bmc::catalogs["card-mvp"].db->ordered.size() : 0);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bmc_mvpid) {
	int64 index = script_getnum64(st, 2); t_itemid id = 0;
	if (bmc::ready("card-mvp")) {
		auto& rows = bmc::catalogs["card-mvp"].db->ordered;
		if (index >= 0 && static_cast<uint64>(index) < rows.size()) id = rows[index]->id;
	}
	script_pushint(st, id);
	return SCRIPT_CMD_SUCCESS;
}

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

// Treasure Magnet is a Premi entitlement, not a new status-change enum. The NPC
// owns the account expiry; this command owns only the current session's treasure
// autoloot flag and its matching client-side timed icon.
BUILDIN_FUNC(premiumtreasureloot)
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

	sd->state.premium_treasureloot = (enable == 1);
	if (enable == 1) {
		clif_status_change(sd, EFST_AID_PERIOD_ADDSTOREITEMCOUNT, 1,
			static_cast<t_tick>(remaining_ms), 0, 0, 0);
	} else {
		clif_status_change(sd, EFST_AID_PERIOD_ADDSTOREITEMCOUNT, 0, 0, 0, 0, 0);
	}

	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

// Return the highest occupied native socket containing an actual normal card.
// Both Vesper commands use this predicate; enchant stones and CARD0_* are excluded.
static int32 vesper_card_target(map_session_data* sd, int32 slot, int16& index)
{
	index = -1;
	if (slot != EQI_HEAD_TOP && slot != EQI_HEAD_MID && slot != EQI_HEAD_LOW &&
		slot != EQI_ARMOR && slot != EQI_HAND_L && slot != EQI_HAND_R &&
		slot != EQI_GARMENT && slot != EQI_SHOES && slot != EQI_ACC_L && slot != EQI_ACC_R)
		return 0;
	index = pc_checkequip(sd, equip_bitmask[slot]);
	if (index < 0 || !sd->inventory_data[index])
		return 0;
	const item& equipped = sd->inventory.u.items_inventory[index];
	const item_data* data = sd->inventory_data[index];
	if (!equipped.equip || !equipped.unique_id || equipped.attribute == 1 ||
		itemdb_isspecial(equipped.card[0]) ||
		(data->type != IT_WEAPON && data->type != IT_ARMOR) ||
		(data->type == IT_WEAPON && slot != EQI_HAND_R && slot != EQI_HAND_L) ||
		(data->type == IT_ARMOR && slot == EQI_HAND_R))
		return 0;
	for (int32 card_index = std::min<int32>(data->slots, MAX_SLOTS) - 1; card_index >= 0; --card_index) {
		const auto card = item_db.find(equipped.card[card_index]);
		if (card && card->type == IT_CARD && card->subtype == CARD_NORMAL)
			return card_index + 1;
	}
	return 0;
}

BUILDIN_FUNC(vespercardtarget)
{
	map_session_data* sd = nullptr;
	int16 index = -1;
	script_pushint(st, script_rid2sd(sd) ? vesper_card_target(sd, script_getnum(st, 2), index) : 0);
	return SCRIPT_CMD_SUCCESS;
}

// 1=removed, 2=stale selection, 3=insufficient BP, 4=invalid state/arguments,
// 5=missing protection materials, 6=failed/broken, 7=failed/protected, 8=rolled back.
BUILDIN_FUNC(vesperremovecard)
{
	map_session_data* sd = nullptr;
	if (!script_rid2sd(sd)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_FAILURE;
	}
	const int32 slot = script_getnum(st, 2);
	const char* uid = script_getstr(st, 3);
	const int32 card_index = script_getnum(st, 4);
	const int64 card_id = script_getnum64(st, 5);
	const int32 protect = script_getnum(st, 6);
	uint64 expected_uid = 0;
	if (!*uid || card_index < 0 || card_index >= MAX_SLOTS || card_id <= 0 ||
		(protect != 0 && protect != 1)) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}
	for (const char* digit = uid; *digit; ++digit) {
		if (*digit < '0' || *digit > '9' || expected_uid > (UINT64_MAX - (*digit - '0')) / 10) {
			script_pushint(st, 4);
			return SCRIPT_CMD_SUCCESS;
		}
		expected_uid = expected_uid * 10 + (*digit - '0');
	}
	int16 index = -1;
	const int32 target = vesper_card_target(sd, slot, index);
	if (!target || target != card_index + 1 ||
		sd->inventory.u.items_inventory[index].unique_id != expected_uid ||
		sd->inventory.u.items_inventory[index].card[card_index] != card_id) {
		script_pushint(st, 2);
		return SCRIPT_CMD_SUCCESS;
	}
	// Refuse a state which cannot safely re-equip the currently worn specimen.
	if (DIFF_TICK(sd->canequip_tick, gettick()) > 0 || pc_isequip(sd, index) != ITEM_EQUIP_ACK_OK) {
		script_pushint(st, 4);
		return SCRIPT_CMD_SUCCESS;
	}
	const int32 price = sd->status.base_level >= 201 ? 600 : sd->status.base_level >= 100 ? 500 :
		(sd->class_ & JOBL_UPPER) ? (sd->status.base_level <= 50 ? 300 : 350) :
		(sd->status.base_level <= 50 ? 200 : 250);
	const int64 points = pc_readreg2(sd, BLACK_MARKET_POINT_VAR);
	if (points < price) {
		script_pushint(st, 3);
		return SCRIPT_CMD_SUCCESS;
	}
	const t_itemid material_id[2] = { 6635, 1000322 };
	int16 material_index[2] = { -1, -1 };
	item materials[2]{};
	item_data* material_data[2] = { nullptr, nullptr };
	if (protect) {
		for (int32 i = 0; i < 2; ++i) {
			material_index[i] = pc_search_inventory(sd, material_id[i]);
			if (material_index[i] < 0 || !sd->inventory_data[material_index[i]] ||
				sd->inventory.u.items_inventory[material_index[i]].equip ||
				sd->inventory.u.items_inventory[material_index[i]].amount < 1) {
				script_pushint(st, 5);
				return SCRIPT_CMD_SUCCESS;
			}
			materials[i] = sd->inventory.u.items_inventory[material_index[i]];
			material_data[i] = sd->inventory_data[material_index[i]];
		}
	}
	const item original = sd->inventory.u.items_inventory[index];
	const bool success = rnd() % 100 < (card_index + 1) * 20;
	bool changed = false;
	bool paid = false;
	bool consumed[2] = { false, false };
	// Only this transaction's item, registry value and consumed stacks are restored.
	const auto rollback = [&]() {
		bool restored = true;
		for (int32 i = 0; i < 2; ++i) {
			if (!consumed[i]) continue;
			item& stack = sd->inventory.u.items_inventory[material_index[i]];
			if ((materials[i].amount == 1 && stack.nameid != 0) ||
				(materials[i].amount > 1 && (stack.nameid != materials[i].nameid ||
				stack.amount != materials[i].amount - 1))) {
				restored = false;
				continue;
			}
			stack = materials[i];
			sd->inventory_data[material_index[i]] = material_data[i];
			sd->weight += material_data[i]->weight;
			log_pick_pc(sd, LOG_TYPE_SCRIPT, 1, &stack);
			clif_additem(sd, material_index[i], 1, 0);
		}
		if (paid && !set_reg_num(nullptr, sd, add_str(BLACK_MARKET_POINT_VAR), BLACK_MARKET_POINT_VAR, points, nullptr))
			restored = false;
		if (changed) {
			item& equipped = sd->inventory.u.items_inventory[index];
			if (equipped.equip && !pc_unequipitem(sd, index, 2 | 4))
				restored = false;
			if (!equipped.equip) {
				equipped = original;
				equipped.equip = 0;
				clif_delitem(*sd, index, 1, 3);
				clif_additem(sd, index, 1, 0);
				if (!pc_equipitem(sd, index, original.equip) || equipped.equip != original.equip)
					restored = false;
			}
		}
		clif_updatestatus(*sd, SP_WEIGHT);
		clif_equiplist(sd);
		pc_show_questinfo(sd);
		if (!restored)
			ShowError("buildin_vesperremovecard: rollback failed (CID=%d/AID=%d).\n", sd->status.char_id, sd->status.account_id);
		script_pushint(st, 8);
	};
	if (success) {
		// Flag 4 suppresses collateral ammunition unequip during a successful update.
		if (!pc_unequipitem(sd, index, 2 | 4)) {
			rollback();
			return SCRIPT_CMD_SUCCESS;
		}
		changed = true;
		sd->inventory.u.items_inventory[index].card[card_index] = 0;
		clif_delitem(*sd, index, 1, 3);
		clif_additem(sd, index, 1, 0);
		if (!pc_equipitem(sd, index, original.equip) ||
			sd->inventory.u.items_inventory[index].equip != original.equip) {
			rollback();
			return SCRIPT_CMD_SUCCESS;
		}
	} else if (!protect) {
		changed = true;
		sd->inventory.u.items_inventory[index].attribute = 1;
		if (!pc_unequipitem(sd, index, 3)) {
			// Unequip failed before changing equipment indices; restore the broken flag.
			sd->inventory.u.items_inventory[index].attribute = original.attribute;
			changed = false;
			rollback();
			return SCRIPT_CMD_SUCCESS;
		}
		clif_equiplist(sd);
	}
	// Equip/unequip scripts run synchronously: do not overwrite a changed balance.
	if (pc_readreg2(sd, BLACK_MARKET_POINT_VAR) != points ||
		!set_reg_num(nullptr, sd, add_str(BLACK_MARKET_POINT_VAR), BLACK_MARKET_POINT_VAR, points - price, nullptr)) {
		rollback();
		return SCRIPT_CMD_SUCCESS;
	}
	paid = true;
	if (protect) {
		for (int32 i = 0; i < 2; ++i) {
			const item& stack = sd->inventory.u.items_inventory[material_index[i]];
			if (stack.nameid != materials[i].nameid || stack.unique_id != materials[i].unique_id ||
				stack.amount != materials[i].amount ||
				pc_delitem(sd, material_index[i], 1, 0, 0, LOG_TYPE_SCRIPT) != 0) {
				rollback();
				return SCRIPT_CMD_SUCCESS;
			}
			consumed[i] = true;
		}
	}
	if (changed) {
		log_pick_pc(sd, LOG_TYPE_SCRIPT, -1, &original);
		log_pick_pc(sd, LOG_TYPE_SCRIPT, 1, &sd->inventory.u.items_inventory[index]);
	}
	script_pushint(st, success ? 1 : protect ? 7 : 6);
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
	if (!bmc::managed_ready(shopname)) {
		ShowError("buildin_callblackmarketshop: managed catalog for shop [%s] is unavailable.\n", shopname);
		script_pushint(st, 0);
		return SCRIPT_CMD_FAILURE;
	}
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode)) {
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode) || ui_step < 1 || ui_step > static_cast<int32>(pool->steps.size())) {
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode)) {
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode)) {
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode) || n < 0) {
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
	if (pool == nullptr || !sve_has_card_mode(pool->mode)) {
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
	if (pool == nullptr || !sve_has_ro_mode(pool->mode)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->ro_pools.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_ropoolcount)
{
	const int32 item_id = script_getnum(st, 2);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || !sve_has_ro_mode(pool->mode)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(pool->ro_pools.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_ropoollabel)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 pool_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || !sve_has_ro_mode(pool->mode) || pool_index < 0 || pool_index >= static_cast<int32>(pool->ro_pools.size())) {
		script_pushconststr(st, "");
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushstrcopy(st, pool->ro_pools[pool_index].label.c_str());
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_ropoolmaxslots)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 pool_index = script_getnum(st, 3);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || !sve_has_ro_mode(pool->mode) || pool_index < 0 || pool_index >= static_cast<int32>(pool->ro_pools.size())) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, pool->ro_pools[pool_index].max_slots);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sve_rollro)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 pool_index = script_getnum(st, 3);
	const int32 option_index = script_getnum(st, 4);
	bool force_max = false;
	if (script_hasdata(st, 5))
		force_max = script_getnum(st, 5) != 0;
	int32 out_id = 0;
	int32 out_val = 0;
	int32 out_param = 0;

	if (item_id < 1 || !sve_roll_ro(static_cast<t_itemid>(item_id), pool_index, option_index, out_id, out_val, out_param, force_max)) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	setd_sub_num(st, nullptr, ".@sve_opt_id", 0, out_id, nullptr);
	setd_sub_num(st, nullptr, ".@sve_opt_value", 0, out_val, nullptr);
	setd_sub_num(st, nullptr, ".@sve_opt_param", 0, out_param, nullptr);

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
	const int32 charm_id = script_getnum(st, 4);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || charm_id < 1) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const svu_series* series = svu_find_series(*pool, card_index, static_cast<t_itemid>(charm_id));
	if (series == nullptr) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(series->levels.size()));
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(svu_levelitem)
{
	const int32 item_id = script_getnum(st, 2);
	const int32 card_index = script_getnum(st, 3);
	const int32 charm_id = script_getnum(st, 4);
	const int32 n = script_getnum(st, 5);
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || charm_id < 1 || n < 0) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	const svu_series* series = svu_find_series(*pool, card_index, static_cast<t_itemid>(charm_id));
	if (series == nullptr || n >= static_cast<int32>(series->levels.size())) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, static_cast<int32>(series->levels[n]));
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
	bool force_max = false;
	if (script_hasdata(st, 5))
		force_max = script_getnum(st, 5) != 0;
	auto pool = item_id < 1 ? nullptr : silvervine_enchant_upgrade_db.find(static_cast<t_itemid>(item_id));
	if (pool == nullptr || charm_id < 1) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}

	script_pushint(st, svu_roll_level(*pool, card_index, static_cast<t_itemid>(charm_id), force_max));
	return SCRIPT_CMD_SUCCESS;
}
