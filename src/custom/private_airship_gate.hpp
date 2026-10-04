// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Private Airship destination progression gates for ordinary characters.

#ifndef PRIVATE_AIRSHIP_GATE_HPP
#define PRIVATE_AIRSHIP_GATE_HPP

#include <cstring>

#include "../map/pc.hpp"
#include "../map/quest.hpp"

namespace {

inline bool private_airship_base_level_at_least( const map_session_data* sd, uint16 level ) {
	return sd != nullptr && sd->status.base_level >= level;
}

inline bool private_airship_reg_at_least( const map_session_data* sd, const char* reg, int64 threshold ) {
	return sd != nullptr && pc_readreg2( sd, reg ) >= threshold;
}

inline bool private_airship_misc_quest_bit( const map_session_data* sd, int64 mask ) {
	return sd != nullptr && ( pc_readreg2( sd, "MISC_QUEST" ) & mask ) != 0;
}

inline int32 private_airship_isbegin_quest( const map_session_data* sd, int32 quest_id ) {
	const int32 q = quest_check( sd, quest_id, HAVEQUEST );
	return q + ( q < 1 );
}

inline bool private_airship_is_transcendent_upper( const map_session_data* sd ) {
	return sd != nullptr && ( sd->class_ & JOBL_UPPER ) != 0;
}

inline bool private_airship_episode_17_1_complete( const map_session_data* sd ) {
	return private_airship_isbegin_quest( sd, 16360 ) >= 2;
}

inline bool private_airship_biosphere_episode_gate( const map_session_data* sd ) {
	return private_airship_reg_at_least( sd, "ep16_royal", 24 )
		&& private_airship_reg_at_least( sd, "terra_gloria_main", 25 )
		&& private_airship_episode_17_1_complete( sd )
		&& private_airship_reg_at_least( sd, "ep17_2_main", 36 );
}

inline bool private_airship_prt_mz03_i_allowed( const map_session_data* sd ) {
	if( !private_airship_base_level_at_least( sd, 170 ) )
		return false;
	if( pc_readreg2( sd, "ill_laby" ) < 3 )
		return false;
	if( pc_readreg2( sd, "ill_laby" ) == 4 && private_airship_isbegin_quest( sd, 12488 ) == 1 )
		return false;
	return true;
}

inline bool private_airship_lhz_dun03_allowed( const map_session_data* sd ) {
	if( !private_airship_misc_quest_bit( sd, 512 ) )
		return false;
	if( private_airship_is_transcendent_upper( sd ) )
		return private_airship_base_level_at_least( sd, 90 );
	return private_airship_base_level_at_least( sd, 95 );
}

inline bool private_airship_new_world_unlocked( const map_session_data* sd ) {
	return private_airship_reg_at_least( sd, "ep13_ryu", 100 )
		|| private_airship_reg_at_least( sd, "ep13_start", 100 );
}

inline bool private_airship_is_new_world_destination( const char* map_name ) {
	if( map_name == nullptr )
		return false;

	static const char* const destinations[] = {
		"mid_camp", "man_fild01", "man_fild02", "man_fild03", "spl_fild01", "spl_fild02", "spl_fild03",
		"manuk", "splendide", "bif_fild01", "bif_fild02", "mora", "ecl_fild01", "ecl_tdun01", "ecl_tdun02", "ecl_tdun03", "ecl_tdun04",
		"dic_dun01", "dic_fild01", "dic_fild02", "dicastes01", "dicastes02", "dic_dun02", "dic_dun03", "eclage", "moro_vol", "moro_cav"
	};

	for( const char* destination : destinations ) {
		if( strcmp( map_name, destination ) == 0 )
			return true;
	}

	return false;
}

inline bool private_airship_new_world_destination_allowed( const map_session_data* sd, const char* map_name ) {
	if( sd == nullptr || map_name == nullptr )
		return false;

	uint16 required_level = 0;
	if( strcmp( map_name, "mid_camp" ) == 0
		|| strcmp( map_name, "man_fild01" ) == 0 || strcmp( map_name, "man_fild02" ) == 0 || strcmp( map_name, "man_fild03" ) == 0
		|| strcmp( map_name, "spl_fild01" ) == 0 || strcmp( map_name, "spl_fild02" ) == 0 || strcmp( map_name, "spl_fild03" ) == 0
		|| strcmp( map_name, "dic_dun01" ) == 0 || strcmp( map_name, "dic_dun02" ) == 0 || strcmp( map_name, "dic_dun03" ) == 0
		|| strcmp( map_name, "dicastes01" ) == 0 || strcmp( map_name, "dicastes02" ) == 0 )
		required_level = 70;
	else if( strcmp( map_name, "eclage" ) == 0 )
		required_level = 120;
	else if( strcmp( map_name, "moro_vol" ) == 0 || strcmp( map_name, "moro_cav" ) == 0 )
		required_level = 140;

	if( !private_airship_base_level_at_least( sd, required_level ) )
		return false;
	if( !private_airship_new_world_unlocked( sd ) )
		return false;

	if( strcmp( map_name, "dic_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "ep13_3_invite", 4 );
	if( strcmp( map_name, "dic_dun02" ) == 0 || strcmp( map_name, "dic_dun03" ) == 0 )
		return private_airship_reg_at_least( sd, "ep13_3_secret", 6 );
	if( strcmp( map_name, "dicastes01" ) == 0 || strcmp( map_name, "dicastes02" ) == 0 )
		return private_airship_reg_at_least( sd, "ep13_3_invite", 5 );
	if( strcmp( map_name, "eclage" ) == 0 )
		return private_airship_reg_at_least( sd, "ep14_2_oliver", 3 );
	if( strcmp( map_name, "moro_vol" ) == 0 )
		return private_airship_reg_at_least( sd, "ep14_3_newerabs", 8 );
	if( strcmp( map_name, "moro_cav" ) == 0 )
		return private_airship_reg_at_least( sd, "ep14_3_newerabs", 24 );

	return true;
}

inline bool private_airship_ra_sanctuary_allowed( const map_session_data* sd ) {
	if( !private_airship_base_level_at_least( sd, 60 ) )
		return false;
	return pc_readreg2( sd, "ra_tem_q" ) > 21 || private_airship_misc_quest_bit( sd, 8192 );
}

} // namespace

inline bool private_airship_ordinary_destination_allowed( const map_session_data* sd, const char* map_name ) {
	if( sd == nullptr || map_name == nullptr )
		return false;
	if( private_airship_is_new_world_destination( map_name ) )
		return private_airship_new_world_destination_allowed( sd, map_name );

	// Gray Wolf Forest opens from the Oz rope. Field 2 is a plain walk from field 1.
	if( strcmp( map_name, "gw_fild01" ) == 0 || strcmp( map_name, "gw_fild02" ) == 0 )
		return private_airship_reg_at_least( sd, "ep18_main", 31 );
	// Grey Wolf Village opens at the Camper, after the story sets this stage.
	if( strcmp( map_name, "wolfvill" ) == 0 )
		return private_airship_reg_at_least( sd, "ep18_main", 35 );

	if( strcmp( map_name, "amicitia1" ) == 0 )
		return private_airship_base_level_at_least( sd, 215 );
	if( strcmp( map_name, "amicitia2" ) == 0 )
		return private_airship_base_level_at_least( sd, 230 );
	if( strcmp( map_name, "clock_01" ) == 0 )
		return private_airship_base_level_at_least( sd, 240 );
	if( strcmp( map_name, "ein_dun03" ) == 0 )
		return private_airship_base_level_at_least( sd, 180 );
	if( strcmp( map_name, "mag_dun03" ) == 0 )
		return private_airship_base_level_at_least( sd, 175 );
	if( strcmp( map_name, "odin_past" ) == 0 )
		return private_airship_base_level_at_least( sd, 180 );
	if( strcmp( map_name, "nif_dun01" ) == 0 )
		return private_airship_base_level_at_least( sd, 200 );
	if( strcmp( map_name, "nif_dun02" ) == 0 )
		return private_airship_base_level_at_least( sd, 240 );
	if( strcmp( map_name, "ice_d03_i" ) == 0 )
		return private_airship_base_level_at_least( sd, 130 );
	if( strcmp( map_name, "tur_d03_i" ) == 0 || strcmp( map_name, "tur_d04_i" ) == 0 )
		return private_airship_base_level_at_least( sd, 150 );
	if( strcmp( map_name, "com_d02_i" ) == 0 )
		return private_airship_base_level_at_least( sd, 160 );
	if( strcmp( map_name, "abyss_04" ) == 0 )
		return private_airship_base_level_at_least( sd, 190 );

	if( strcmp( map_name, "nameless_n" ) == 0 )
		return private_airship_reg_at_least( sd, "aru_monas", 20 ) && private_airship_base_level_at_least( sd, 80 );
	if( strcmp( map_name, "abbey01" ) == 0 || strcmp( map_name, "abbey02" ) == 0 || strcmp( map_name, "abbey03" ) == 0 )
		return private_airship_reg_at_least( sd, "aru_monas", 26 ) && private_airship_base_level_at_least( sd, 80 );

	if( strcmp( map_name, "ama_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "event_amatsu", 6 );
	if( strcmp( map_name, "ama_dun02" ) == 0 || strcmp( map_name, "ama_dun03" ) == 0 )
		return private_airship_reg_at_least( sd, "event_amatsu", 6 ) && private_airship_base_level_at_least( sd, 40 );

	if( strcmp( map_name, "ant_d02_i" ) == 0 )
		return private_airship_reg_at_least( sd, "ill_anthell", 2 ) && private_airship_base_level_at_least( sd, 160 );

	if( strcmp( map_name, "ayo_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "ayodunquest", 3 );
	if( strcmp( map_name, "ayo_dun02" ) == 0 )
		return private_airship_reg_at_least( sd, "ayodunquest", 11 );

	if( strcmp( map_name, "bra_dun01" ) == 0 || strcmp( map_name, "bra_dun02" ) == 0 )
		return private_airship_reg_at_least( sd, "brazil_ghost", 8 ) && private_airship_base_level_at_least( sd, 40 );

	if( strcmp( map_name, "dew_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "dew_legend", 8 ) && private_airship_base_level_at_least( sd, 60 );

	if( strcmp( map_name, "ein_d02_i" ) == 0 )
		return private_airship_reg_at_least( sd, "ill_teddy", 3 ) && private_airship_base_level_at_least( sd, 150 );

	if( strcmp( map_name, "gef_d01_i" ) == 0 )
		return private_airship_reg_at_least( sd, "ill_vampire", 4 ) && private_airship_base_level_at_least( sd, 130 );

	if( strcmp( map_name, "gefenia01" ) == 0 || strcmp( map_name, "gefenia02" ) == 0
		|| strcmp( map_name, "gefenia03" ) == 0 || strcmp( map_name, "gefenia04" ) == 0 )
		return private_airship_reg_at_least( sd, "sign_q", 144 ) && private_airship_base_level_at_least( sd, 50 );

	if( strcmp( map_name, "icecastle" ) == 0 || strcmp( map_name, "jor_ab01" ) == 0 || strcmp( map_name, "jor_ab02" ) == 0
		|| strcmp( map_name, "jor_back1" ) == 0 || strcmp( map_name, "jor_back2" ) == 0 || strcmp( map_name, "jor_back3" ) == 0
		|| strcmp( map_name, "jor_back4" ) == 0 || strcmp( map_name, "jor_back5" ) == 0 || strcmp( map_name, "jor_back6" ) == 0
		|| strcmp( map_name, "jor_dun01" ) == 0 || strcmp( map_name, "jor_dun02" ) == 0 || strcmp( map_name, "jor_nest" ) == 0
		|| strcmp( map_name, "jor_root1" ) == 0 || strcmp( map_name, "jor_root2" ) == 0 || strcmp( map_name, "jor_root3" ) == 0
		|| strcmp( map_name, "jor_tail" ) == 0 || strcmp( map_name, "jor_twice" ) == 0 || strcmp( map_name, "jor_twig" ) == 0 )
		return private_airship_reg_at_least( sd, "ep18_main", 57 );

	if( strcmp( map_name, "iz_d04_i" ) == 0 )
		return private_airship_reg_at_least( sd, "ill_underwater", 4 ) && private_airship_base_level_at_least( sd, 140 );
	if( strcmp( map_name, "iz_d05_i" ) == 0 )
		return private_airship_reg_at_least( sd, "ill_underwater", 4 ) && private_airship_base_level_at_least( sd, 180 );

	if( strcmp( map_name, "kh_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "KielHyreQuest", 36 ) && private_airship_base_level_at_least( sd, 70 );
	if( strcmp( map_name, "kh_dun02" ) == 0 )
		return private_airship_reg_at_least( sd, "KielHyreQuest", 106 ) && private_airship_base_level_at_least( sd, 70 );

	if( strcmp( map_name, "lhz_dun01" ) == 0 || strcmp( map_name, "lhz_dun02" ) == 0 )
		return private_airship_misc_quest_bit( sd, 512 ) && private_airship_base_level_at_least( sd, 60 );
	if( strcmp( map_name, "lhz_dun03" ) == 0 )
		return private_airship_lhz_dun03_allowed( sd );
	if( strcmp( map_name, "lhz_dun04" ) == 0 )
		return private_airship_reg_at_least( sd, "lhz_curse", 31 ) && private_airship_base_level_at_least( sd, 60 );

	if( strcmp( map_name, "ma_dun01" ) == 0 )
		return private_airship_reg_at_least( sd, "malaya_bang", 18 ) && private_airship_base_level_at_least( sd, 100 );

	if( strcmp( map_name, "mosk_dun01" ) == 0 || strcmp( map_name, "mosk_dun02" ) == 0 || strcmp( map_name, "mosk_dun03" ) == 0 )
		return private_airship_reg_at_least( sd, "mos_whale_edq", 41 );

	if( strcmp( map_name, "ba_2whs01" ) == 0 || strcmp( map_name, "ba_2whs02" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_reg_at_least( sd, "ep17_2_main", 9 )
			&& private_airship_base_level_at_least( sd, 160 );
	if( strcmp( map_name, "ba_bath" ) == 0 || strcmp( map_name, "ba_pw01" ) == 0 || strcmp( map_name, "ba_pw03" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_reg_at_least( sd, "ep17_2_main", 9 )
			&& private_airship_base_level_at_least( sd, 130 );
	if( strcmp( map_name, "ba_lib" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_reg_at_least( sd, "ep17_2_library", 2 )
			&& private_airship_base_level_at_least( sd, 130 );
	if( strcmp( map_name, "ba_lost" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_isbegin_quest( sd, 8586 ) >= 2
			&& private_airship_reg_at_least( sd, "ep16_royal", 24 )
			&& private_airship_reg_at_least( sd, "terra_gloria_main", 25 )
			&& private_airship_base_level_at_least( sd, 130 );
	if( strcmp( map_name, "ba_maison" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_reg_at_least( sd, "ep17_2_main", 7 )
			&& private_airship_base_level_at_least( sd, 130 );
	if( strcmp( map_name, "ba_pw02" ) == 0 )
		return private_airship_episode_17_1_complete( sd )
			&& private_airship_reg_at_least( sd, "ep17_2_main", 2 )
			&& private_airship_base_level_at_least( sd, 130 );

	if( strcmp( map_name, "bl_death" ) == 0 || strcmp( map_name, "bl_grass" ) == 0
		|| strcmp( map_name, "bl_ice" ) == 0 || strcmp( map_name, "bl_lava" ) == 0 )
		return private_airship_biosphere_episode_gate( sd ) && private_airship_base_level_at_least( sd, 240 );

	if( strcmp( map_name, "oz_dun01" ) == 0 || strcmp( map_name, "oz_dun02" ) == 0 )
		return private_airship_biosphere_episode_gate( sd ) && private_airship_reg_at_least( sd, "ep18_main", 57 );

	if( strcmp( map_name, "prt_mz03_i" ) == 0 )
		return private_airship_prt_mz03_i_allowed( sd );

	if( strcmp( map_name, "pay_d03_i" ) == 0 )
		return private_airship_reg_at_least( sd, "illusion_moonlight", 6 ) && private_airship_base_level_at_least( sd, 100 );

	if( strcmp( map_name, "prt_sewb1" ) == 0 || strcmp( map_name, "prt_sewb2" ) == 0
		|| strcmp( map_name, "prt_sewb3" ) == 0 || strcmp( map_name, "prt_sewb4" ) == 0 )
		return private_airship_misc_quest_bit( sd, 8 );

	if( strcmp( map_name, "ra_san01" ) == 0 || strcmp( map_name, "ra_san02" ) == 0 || strcmp( map_name, "ra_san03" ) == 0
		|| strcmp( map_name, "ra_san04" ) == 0 || strcmp( map_name, "ra_san05" ) == 0 || strcmp( map_name, "ra_temple" ) == 0 )
		return private_airship_ra_sanctuary_allowed( sd );

	if( strcmp( map_name, "sp_rudus" ) == 0 || strcmp( map_name, "sp_rudus2" ) == 0 || strcmp( map_name, "sp_rudus3" ) == 0 )
		return private_airship_isbegin_quest( sd, 7850 ) >= 2 && private_airship_base_level_at_least( sd, 110 );
	if( strcmp( map_name, "sp_rudus4" ) == 0 )
		return private_airship_isbegin_quest( sd, 16521 ) >= 1;

	if( strcmp( map_name, "tha_t01" ) == 0 || strcmp( map_name, "tha_t02" ) == 0 || strcmp( map_name, "tha_t03" ) == 0
		|| strcmp( map_name, "tha_t04" ) == 0 || strcmp( map_name, "tha_t05" ) == 0 || strcmp( map_name, "tha_t06" ) == 0
		|| strcmp( map_name, "tha_t07" ) == 0 || strcmp( map_name, "tha_t08" ) == 0 || strcmp( map_name, "tha_t09" ) == 0
		|| strcmp( map_name, "tha_t10" ) == 0 || strcmp( map_name, "tha_t11" ) == 0 || strcmp( map_name, "tha_t12" ) == 0 )
		return private_airship_reg_at_least( sd, "thana_tower", 10 );

	if( strcmp( map_name, "tur_dun01" ) == 0 || strcmp( map_name, "tur_dun02" ) == 0 || strcmp( map_name, "tur_dun03" ) == 0
		|| strcmp( map_name, "tur_dun04" ) == 0 || strcmp( map_name, "tur_dun05" ) == 0 )
		return private_airship_misc_quest_bit( sd, 65536 );

	if( strcmp( map_name, "verus03" ) == 0 || strcmp( map_name, "verus04" ) == 0 )
		return private_airship_reg_at_least( sd, "ep15_1_atnad", 9 ) && private_airship_base_level_at_least( sd, 140 );
	if( strcmp( map_name, "verus01" ) == 0 || strcmp( map_name, "verus02" ) == 0 )
		return private_airship_reg_at_least( sd, "ep15_1_atnad", 22 ) && private_airship_base_level_at_least( sd, 140 );

	return true;
}

#endif /* PRIVATE_AIRSHIP_GATE_HPP */
