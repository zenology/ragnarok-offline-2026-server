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

struct svu_series {
	std::vector<t_itemid> levels;
};

struct svu_slot {
	int32 card_index = 0;
	std::vector<svu_series> series;
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

		auto parse_levels = [&](const ryml::NodeRef& levelsNode, const ryml::NodeRef& warnNode, int32 card_index, std::vector<t_itemid>& seen_in_slot, std::vector<t_itemid>& out) -> bool {
			if (!levelsNode.is_seq() || levelsNode.num_children() < 2) {
				this->invalidWarning(warnNode, "Slot CardIndex needs at least 2 Levels for Id %u.\n", nameid);
				return false;
			}

			for (const ryml::NodeRef& levelNode : levelsNode) {
				t_itemid item_id = 0;
				if (!this->asUInt32(levelNode, "Item", item_id))
					return false;
				if (item_id < 1) {
					this->invalidWarning(levelNode, "Level Item for Id %u must be positive.\n", nameid);
					return false;
				}
				if (this->nodeExists(levelNode, "Chance") || this->nodeExists(levelNode, "Grade")) {
					this->invalidWarning(levelNode, "Level Item %u for Id %u has Chance or Grade; upgrade weights are derived.\n", item_id, nameid);
					return false;
				}
				for (t_itemid existing_id : seen_in_slot) {
					if (existing_id == item_id) {
						this->invalidWarning(levelNode, "Duplicate Level Item %u for Id %u CardIndex %d.\n", item_id, nameid, card_index);
						return false;
					}
				}
				seen_in_slot.push_back(item_id);
				out.push_back(item_id);
			}

			return true;
		};

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

			const bool has_levels = this->nodeExists(slotNode, "Levels");
			const bool has_series = this->nodeExists(slotNode, "Series");
			if (has_levels == has_series) {
				this->invalidWarning(slotNode, "Slot CardIndex %d for Id %u must have Levels or Series, not both or neither.\n", slot.card_index, nameid);
				return 0;
			}

			std::vector<t_itemid> seen_in_slot;

			if (has_levels) {
				svu_series one;
				if (!parse_levels(slotNode["Levels"], slotNode["Levels"], slot.card_index, seen_in_slot, one.levels))
					return 0;
				slot.series.push_back(std::move(one));
			} else {
				const auto& seriesNode = slotNode["Series"];
				if (!seriesNode.is_seq() || seriesNode.num_children() < 1) {
					this->invalidWarning(slotNode["Series"], "Slot CardIndex %d for Id %u has empty Series.\n", slot.card_index, nameid);
					return 0;
				}

				for (const ryml::NodeRef& seriesChild : seriesNode) {
					if (this->nodeExists(seriesChild, "Chance") || this->nodeExists(seriesChild, "Grade")) {
						this->invalidWarning(seriesChild, "Series for Id %u CardIndex %d has Chance or Grade; upgrade weights are derived.\n", nameid, slot.card_index);
						return 0;
					}
					if (!this->nodeExists(seriesChild, "Levels")) {
						this->invalidWarning(seriesChild, "Series for Id %u CardIndex %d is missing Levels.\n", nameid, slot.card_index);
						return 0;
					}

					svu_series one;
					if (!parse_levels(seriesChild["Levels"], seriesChild["Levels"], slot.card_index, seen_in_slot, one.levels))
						return 0;
					slot.series.push_back(std::move(one));
				}
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
				for (const auto& series : slot.series) {
					for (t_itemid item_id : series.levels) {
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

inline const svu_series* svu_find_series(const svu_item_pool& pool, int32 card_index, t_itemid charm_id) {
	const int32 slot_index = svu_find_slot_index(pool, card_index);
	if (slot_index < 0 || charm_id < 1)
		return nullptr;

	for (const auto& series : pool.slots[slot_index].series) {
		for (t_itemid item_id : series.levels) {
			if (item_id == charm_id)
				return &series;
		}
	}
	return nullptr;
}

inline int32 svu_find_level(const svu_item_pool& pool, int32 card_index, t_itemid charm_id) {
	const svu_series* series = svu_find_series(pool, card_index, charm_id);
	if (series == nullptr)
		return 0;

	for (size_t i = 0; i < series->levels.size(); ++i) {
		if (series->levels[i] == charm_id)
			return static_cast<int32>(i + 1);
	}
	return 0;
}

inline int32 svu_roll_series(const std::vector<t_itemid>& levels, bool force_max) {
	const size_t n = levels.size();
	if (n == 0)
		return 0;
	if (n == 1)
		return static_cast<int32>(levels[0]);
	if (force_max)
		return static_cast<int32>(levels[n - 1]);
	if (n == 2) {
		const uint32 r = rnd_value<uint32>(1, 5);
		return static_cast<int32>(r <= 4 ? levels[0] : levels[1]);
	}

	const uint32 r = rnd_value<uint32>(1, 10);
	if (r <= 5)
		return static_cast<int32>(levels[0]);
	if (r >= 9)
		return static_cast<int32>(levels[n - 1]);

	const uint32 mid = rnd_value<uint32>(1, static_cast<uint32>(n - 2));
	return static_cast<int32>(levels[mid]);
}

inline int32 svu_roll_level(const svu_item_pool& pool, int32 card_index, t_itemid charm_id, bool force_max) {
	const svu_series* series = svu_find_series(pool, card_index, charm_id);
	if (series == nullptr)
		return 0;

	return svu_roll_series(series->levels, force_max);
}

#endif /* SILVERVINE_ENCHANT_UPGRADE_HPP */
