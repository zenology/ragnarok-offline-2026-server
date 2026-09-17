// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Silvervine enchant upgrade YAML pools. Included only by custom/script.inc.

#ifndef SILVERVINE_ENCHANT_UPGRADE_HPP
#define SILVERVINE_ENCHANT_UPGRADE_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <common/database.hpp>
#include <common/random.hpp>
#include <common/showmsg.hpp>

struct svu_slot {
	int32 card_index = 0;
	std::vector<t_itemid> levels;
};

struct svu_item_pool {
	t_itemid id = 0;
	std::vector<svu_slot> slots;
};

class SilvervineEnchantUpgradeDatabase : public TypesafeYamlDatabase<t_itemid, svu_item_pool> {
public:
	bool loaded_ok = true;

	SilvervineEnchantUpgradeDatabase() : TypesafeYamlDatabase("SILVERVINE_ENCHANT_UPGRADE_POOL", 1) {
	}

	void clear() override {
		TypesafeYamlDatabase<t_itemid, svu_item_pool>::clear();
		this->loaded_ok = true;
	}

	const std::string getDefaultLocation() override {
		return "npc/custom/silvervine_enchant_upgrade_item_pools.yml";
	}

	uint64 parseBodyNode(const ryml::NodeRef& node) override {
		t_itemid nameid;

		if (!this->asUInt32(node, "Id", nameid))
			return 0;

		if (this->find(nameid) != nullptr) {
			this->invalidWarning(node["Id"], "Duplicate Silvervine enchant upgrade Id %u.\n", nameid);
			return 0;
		}

		if (this->nodeExists(node, "Mode") || this->nodeExists(node, "Steps") || this->nodeExists(node, "Enchants") || this->nodeExists(node, "Groups")) {
			this->invalidWarning(node, "Id %u has cat-pool keys; upgrade YAML must not share Mode, Steps, Enchants, or Groups.\n", nameid);
			return 0;
		}

		if (this->nodeExists(node, "MinRefine") || this->nodeExists(node, "MinEnchantgrade") || this->nodeExists(node, "Chance") || this->nodeExists(node, "Grade")) {
			this->invalidWarning(node, "Id %u has MinRefine, MinEnchantgrade, Chance, or Grade; upgrade pools have no those keys.\n", nameid);
			return 0;
		}

		if (!this->nodeExists(node, "Slots")) {
			this->invalidWarning(node, "Id %u is missing Slots.\n", nameid);
			return 0;
		}

		const auto& slotsNode = node["Slots"];
		if (!slotsNode.is_seq() || slotsNode.num_children() < 1) {
			this->invalidWarning(node["Slots"], "Id %u has empty Slots.\n", nameid);
			return 0;
		}

		auto pool = std::make_shared<svu_item_pool>();
		pool->id = nameid;

		for (const ryml::NodeRef& slotNode : slotsNode) {
			svu_slot slot;

			if (!this->asInt32(slotNode, "CardIndex", slot.card_index))
				return 0;
			if (slot.card_index < 0 || slot.card_index > 3) {
				this->invalidWarning(slotNode["CardIndex"], "CardIndex %d for Id %u must be 0..3.\n", slot.card_index, nameid);
				return 0;
			}

			for (const auto& existing : pool->slots) {
				if (existing.card_index == slot.card_index) {
					this->invalidWarning(slotNode["CardIndex"], "Duplicate CardIndex %d for Id %u.\n", slot.card_index, nameid);
					return 0;
				}
			}

			if (this->nodeExists(slotNode, "Grade") || this->nodeExists(slotNode, "Chance")) {
				this->invalidWarning(slotNode, "Slot CardIndex %d for Id %u has Grade or Chance; upgrade weights are derived.\n", slot.card_index, nameid);
				return 0;
			}

			if (!this->nodeExists(slotNode, "Levels")) {
				this->invalidWarning(slotNode, "Slot CardIndex %d for Id %u is missing Levels.\n", slot.card_index, nameid);
				return 0;
			}

			const auto& levelsNode = slotNode["Levels"];
			if (!levelsNode.is_seq() || levelsNode.num_children() < 2) {
				this->invalidWarning(slotNode["Levels"], "Slot CardIndex %d for Id %u needs at least 2 Levels.\n", slot.card_index, nameid);
				return 0;
			}

			for (const ryml::NodeRef& levelNode : levelsNode) {
				t_itemid item_id = 0;
				if (!this->asUInt32(levelNode, "Item", item_id))
					return 0;
				if (item_id < 1) {
					this->invalidWarning(levelNode, "Level Item for Id %u must be positive.\n", nameid);
					return 0;
				}
				if (this->nodeExists(levelNode, "Chance") || this->nodeExists(levelNode, "Grade")) {
					this->invalidWarning(levelNode, "Level Item %u for Id %u has Chance or Grade; upgrade weights are derived.\n", item_id, nameid);
					return 0;
				}
				for (t_itemid existing_id : slot.levels) {
					if (existing_id == item_id) {
						this->invalidWarning(levelNode, "Duplicate Level Item %u for Id %u CardIndex %d.\n", item_id, nameid, slot.card_index);
						return 0;
					}
				}
				slot.levels.push_back(item_id);
			}

			pool->slots.push_back(std::move(slot));
		}

		this->put(nameid, pool);
		return 1;
	}

	void loadingFinished() override {
		if (this->empty()) {
			ShowError("Silvervine enchant upgrade YAML loaded zero items.\n");
			this->loaded_ok = false;
			return;
		}

		std::vector<t_itemid> drop_ids;

		for (const auto& pair : *this) {
			const auto& pool = pair.second;
			bool drop = false;

			for (const auto& slot : pool->slots) {
				for (t_itemid item_id : slot.levels) {
					if (item_db.find(item_id) == nullptr) {
						ShowError("Silvervine enchant upgrade Id %u references missing charm %u.\n", pool->id, item_id);
						drop = true;
						break;
					}
				}
				if (drop)
					break;
			}

			if (drop)
				drop_ids.push_back(pool->id);
		}

		for (t_itemid id : drop_ids)
			this->erase(id);

		if (this->empty()) {
			this->loaded_ok = false;
			return;
		}

		this->loaded_ok = true;
		ShowStatus("Silvervine enchant upgrade: %zu item pool(s) loaded.\n", this->size());
	}
};

inline SilvervineEnchantUpgradeDatabase silvervine_enchant_upgrade_db;

inline int32 svu_find_slot_index(const svu_item_pool& pool, int32 card_index) {
	for (size_t i = 0; i < pool.slots.size(); ++i) {
		if (pool.slots[i].card_index == card_index)
			return static_cast<int32>(i);
	}
	return -1;
}

inline int32 svu_find_level(const svu_item_pool& pool, int32 card_index, t_itemid charm_id) {
	const int32 slot_index = svu_find_slot_index(pool, card_index);
	if (slot_index < 0 || charm_id < 1)
		return 0;

	const auto& levels = pool.slots[slot_index].levels;
	for (size_t i = 0; i < levels.size(); ++i) {
		if (levels[i] == charm_id)
			return static_cast<int32>(i + 1);
	}
	return 0;
}

inline int32 svu_roll_level(const svu_item_pool& pool, int32 card_index) {
	const int32 slot_index = svu_find_slot_index(pool, card_index);
	if (slot_index < 0)
		return 0;

	const auto& levels = pool.slots[slot_index].levels;
	const size_t n = levels.size();
	if (n == 0)
		return 0;
	if (n == 1)
		return static_cast<int32>(levels[0]);

	const uint32 total = static_cast<uint32>(n * (n + 1) / 2);
	const uint32 r = rnd_value<uint32>(1, total);
	uint32 acc = 0;
	for (size_t i = 0; i < n; ++i) {
		acc += static_cast<uint32>(n - i);
		if (r <= acc)
			return static_cast<int32>(levels[i]);
	}
	return static_cast<int32>(levels[n - 1]);
}

#endif /* SILVERVINE_ENCHANT_UPGRADE_HPP */
