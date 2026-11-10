// Physical-only class resistance for the two reviewed iRO garments.
#ifndef OFFLINE_BLACK_MARKET_REVIEW_HPP
#define OFFLINE_BLACK_MARKET_REVIEW_HPP

#include <map/pc.hpp>
#include <map/pc_groups.hpp>
#include <map/itemdb.hpp>

namespace offline_black_market_review {

// Called only at the native weapon-defense class multiplier. Keep magic/misc
// damage, native card immunity and all other equipment on their existing path.
inline int32 physical_class_reduction(map_session_data& defender, const block_list& attacker, int32 attacker_class) {
    if (attacker.type != BL_MOB)
        return 0;
    const int16 index = defender.equip_index[EQI_GARMENT];
    if (index < 0)
        return 0;
    const item_data* garment = defender.inventory_data[index];
    if (garment == nullptr)
        return 0;
    if (!pc_has_permission(&defender, PC_PERM_USE_ALL_EQUIPMENT) && itemdb_isNoEquip(garment, defender.m))
        return 0;
    if (garment->nameid == 480156 && attacker_class == CLASS_NORMAL)
        return 5;
    if (garment->nameid == 480077 && attacker_class == CLASS_BOSS)
        return 5;
    return 0;
}

} // namespace offline_black_market_review
#endif
