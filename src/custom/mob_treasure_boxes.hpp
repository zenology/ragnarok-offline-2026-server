// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Project-owned treasure drop extension. Include from mob.cpp after its native drop helpers.

#ifndef CUSTOM_MOB_TREASURE_BOXES_HPP
#define CUSTOM_MOB_TREASURE_BOXES_HPP

namespace {

static t_itemid mob_treasure_box_itemid( e_mob_bosstype boss_type, int32 level ){
	level = std::max( level, 1 );

	if( boss_type == BOSSTYPE_MVP ){
		return 9000019 + ( level <= 100 ? 0 : level <= 200 ? 1 : 2 );
	}

	if( boss_type == BOSSTYPE_MINIBOSS ){
		return 9000016 + ( level <= 100 ? 0 : level <= 200 ? 1 : 2 );
	}

	return 9000001 + ( std::min( level, 300 ) - 1 ) / 20;
}

// Call only inside mob_dead's ordinary-drop eligibility branch, using its existing loot list.
static void mob_treasure_box_drop( mob_data* md, block_list* src, std::shared_ptr<s_item_drop_list>& dlist, bool companion_killonly ){
	int32 level = md->level;
	if( level <= 0 && md->db != nullptr )
		level = md->db->lv;

	e_mob_bosstype boss_type = md->get_bosstype();
	int32 base_rate = boss_type == BOSSTYPE_NONE ? 500 : 1000;
	t_itemid item_id = mob_treasure_box_itemid( boss_type, level );
	if( item_db.find( item_id ) == nullptr )
		return;

	int32 drop_rate = mob_getdroprate( src, md->db, base_rate, 100, md );
	if( rnd() % 10000 >= drop_rate )
		return;

	std::shared_ptr<s_mob_drop> drop = std::make_shared<s_mob_drop>();
	drop->nameid = item_id;
	drop->rate = base_rate;
	drop->randomopt_group = 0;
	drop->steal_protected = false;

	std::shared_ptr<s_item_drop> ditem = mob_setdropitem( drop, 1, md->mob_id );
	mob_item_drop( md, dlist, ditem, 0, battle_config.autoloot_adjust ? drop_rate : base_rate, companion_killonly );
}

} // namespace

#endif // CUSTOM_MOB_TREASURE_BOXES_HPP
