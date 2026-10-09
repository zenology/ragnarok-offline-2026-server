#ifndef OFFLINE_STRIPE_FALLBACK_HPP
#define OFFLINE_STRIPE_FALLBACK_HPP

#include "../map/itemdb.hpp"
#include "../map/pc.hpp"

namespace offline_stripe {

// Marker 127 suppresses the native global bonus and protects only this item.
inline bool has_self_indestructible(const item& equipped, const item_data& data) {
	for (const auto& option : equipped.option) {
		if (option.param == 127 &&
			((option.id == 185 && data.type == IT_WEAPON) ||
			 (option.id == 186 && data.type == IT_ARMOR)))
			return true;
	}
	return false;
}

inline bool protected_item(const map_session_data& sd, int16 inventory_index) {
	if (inventory_index < 0 || inventory_index >= MAX_INVENTORY ||
		!sd.inventory_data[inventory_index])
		return false;
	const item& equipped = sd.inventory.u.items_inventory[inventory_index];
	return equipped.equip != 0 && has_self_indestructible(equipped, *sd.inventory_data[inventory_index]);
}

} // namespace offline_stripe

#endif
