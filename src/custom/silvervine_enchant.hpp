// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Silvervine Enchanter YAML pools. Included only by custom/script.inc.

#ifndef SILVERVINE_ENCHANT_HPP
#define SILVERVINE_ENCHANT_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <common/database.hpp>
#include <common/random.hpp>
#include <common/showmsg.hpp>

enum sve_mode : int32 {
	SVE_MODE_NONE = 0,
	SVE_MODE_CARD = 1,
	SVE_MODE_RO = 2,
	SVE_MODE_DUAL = 3
};

inline bool sve_has_card_mode(sve_mode mode) {
	return mode == SVE_MODE_CARD || mode == SVE_MODE_DUAL;
}

inline bool sve_has_ro_mode(sve_mode mode) {
	return mode == SVE_MODE_RO || mode == SVE_MODE_DUAL;
}

struct sve_enchant_entry {
	t_itemid item_id = 0;
	uint16 chance = 10000;
};

struct sve_step {
	int32 card_index = 0;
	uint32 slot_chance = 100000;
	std::vector<sve_enchant_entry> enchants;
};

struct sve_ro_pool {
	std::string label;
	std::vector<uint16> groups;
	int32 max_slots = 0;
	int32 option_slots[5] = { 0, 0, 0, 0, 0 };
};

struct sve_item_pool {
	t_itemid id = 0;
	sve_mode mode = SVE_MODE_NONE;
	std::vector<sve_step> steps;
	std::vector<sve_ro_pool> ro_pools;
};

inline bool sve_label_is_ascii(const std::string& label) {
	if (label.empty())
		return false;
	for (unsigned char c : label) {
		if (c < 0x20 || c > 0x7E)
			return false;
	}
	return true;
}

class SilvervineEnchantDatabase : public TypesafeYamlDatabase<t_itemid, sve_item_pool> {
public:
	bool loaded_ok = true;

	SilvervineEnchantDatabase() : TypesafeYamlDatabase("SILVERVINE_ENCHANT_ITEM_POOL", 1) {
	}

	void clear() override {
		TypesafeYamlDatabase<t_itemid, sve_item_pool>::clear();
		this->loaded_ok = true;
	}

	const std::string getDefaultLocation() override {
		return "npc/custom/silvervine_enchant_item_pools.yml";
	}

	uint64 parseBodyNode(const ryml::NodeRef& node) override {
		t_itemid nameid;

		if (!this->asUInt32(node, "Id", nameid))
			return 0;

		if (this->find(nameid) != nullptr) {
			this->invalidWarning(node["Id"], "Duplicate Silvervine enchant Id %u.\n", nameid);
			return 0;
		}

		std::string mode_name;
		if (!this->asString(node, "Mode", mode_name))
			return 0;

		auto pool = std::make_shared<sve_item_pool>();
		pool->id = nameid;

		if (mode_name == "CardSlot") {
			pool->mode = SVE_MODE_CARD;
		} else if (mode_name == "RandomOption") {
			pool->mode = SVE_MODE_RO;
		} else if (mode_name == "Dual") {
			pool->mode = SVE_MODE_DUAL;
		} else {
			this->invalidWarning(node["Mode"], "Unknown Mode \"%s\" for Id %u.\n", mode_name.c_str(), nameid);
			return 0;
		}

		if (this->nodeExists(node, "MinRefine") || this->nodeExists(node, "MinEnchantgrade")) {
			this->invalidWarning(node, "Id %u has MinRefine or MinEnchantgrade; Silvervine pools have no refine or grade gate.\n", nameid);
			return 0;
		}

		const bool has_steps = this->nodeExists(node, "Steps");
		const bool has_groups = this->nodeExists(node, "Groups");
		const bool has_pools = this->nodeExists(node, "Pools");

		if (pool->mode == SVE_MODE_CARD && has_groups) {
			this->invalidWarning(node, "CardSlot Id %u must not have Groups.\n", nameid);
			return 0;
		}
		if (pool->mode == SVE_MODE_CARD && has_pools) {
			this->invalidWarning(node, "CardSlot Id %u must not have Pools.\n", nameid);
			return 0;
		}
		if (pool->mode == SVE_MODE_RO && has_steps) {
			this->invalidWarning(node, "RandomOption Id %u must not have Steps.\n", nameid);
			return 0;
		}
		if (sve_has_ro_mode(pool->mode) && has_groups) {
			this->invalidWarning(node, "Id %u still has leftover Groups; RandomOption and Dual use Pools.\n", nameid);
			return 0;
		}
		if (sve_has_card_mode(pool->mode) && !has_steps) {
			this->invalidWarning(node, "Id %u is missing Steps.\n", nameid);
			return 0;
		}
		if (sve_has_ro_mode(pool->mode) && !has_pools) {
			this->invalidWarning(node, "Id %u is missing Pools.\n", nameid);
			return 0;
		}

		if (sve_has_card_mode(pool->mode)) {
			const auto& stepsNode = node["Steps"];
			if (!stepsNode.is_seq() || stepsNode.num_children() < 1) {
				this->invalidWarning(node["Steps"], "CardSlot Id %u has empty Steps.\n", nameid);
				return 0;
			}

			for (const ryml::NodeRef& stepNode : stepsNode) {
				sve_step step;

				if (!this->asInt32(stepNode, "CardIndex", step.card_index))
					return 0;
				if (step.card_index < 0 || step.card_index > 3) {
					this->invalidWarning(stepNode["CardIndex"], "CardIndex %d for Id %u must be 0..3.\n", step.card_index, nameid);
					return 0;
				}

				if (this->nodeExists(stepNode, "Grade")) {
					this->invalidWarning(stepNode, "Step CardIndex %d for Id %u has Grade; Silvervine steps are not grade-gated.\n", step.card_index, nameid);
					return 0;
				}

				if (this->nodeExists(stepNode, "SlotChance")) {
					if (!this->asUInt32(stepNode, "SlotChance", step.slot_chance))
						return 0;
				}

				if (!this->nodeExists(stepNode, "Enchants")) {
					this->invalidWarning(stepNode, "Step CardIndex %d for Id %u is missing Enchants.\n", step.card_index, nameid);
					return 0;
				}

				const auto& enchantsNode = stepNode["Enchants"];
				if (!enchantsNode.is_seq() || enchantsNode.num_children() < 1) {
					this->invalidWarning(stepNode["Enchants"], "Step CardIndex %d for Id %u has empty Enchants.\n", step.card_index, nameid);
					return 0;
				}

				for (const ryml::NodeRef& enchantNode : enchantsNode) {
					sve_enchant_entry entry;
					if (!this->asUInt32(enchantNode, "Item", entry.item_id))
						return 0;
					if (entry.item_id < 1) {
						this->invalidWarning(enchantNode, "Enchant Item for Id %u must be positive.\n", nameid);
						return 0;
					}
					if (this->nodeExists(enchantNode, "Chance")) {
						if (!this->asUInt16(enchantNode, "Chance", entry.chance))
							return 0;
					}
					step.enchants.push_back(entry);
				}

				pool->steps.push_back(std::move(step));
			}
		}

		if (sve_has_ro_mode(pool->mode)) {
			const auto& poolsNode = node["Pools"];
			if (!poolsNode.is_seq() || poolsNode.num_children() < 1) {
				this->invalidWarning(node["Pools"], "Id %u has empty Pools.\n", nameid);
				return 0;
			}

			for (const ryml::NodeRef& poolNode : poolsNode) {
				sve_ro_pool ro_pool;

				if (!this->asString(poolNode, "Label", ro_pool.label))
					return 0;
				if (!sve_label_is_ascii(ro_pool.label)) {
					this->invalidWarning(poolNode["Label"], "Id %u pool Label must be non-empty ASCII.\n", nameid);
					return 0;
				}

				if (!this->asInt32(poolNode, "MaxSlots", ro_pool.max_slots))
					return 0;
				if (ro_pool.max_slots < 1 || ro_pool.max_slots > MAX_ITEM_RDM_OPT) {
					this->invalidWarning(poolNode["MaxSlots"], "Id %u MaxSlots %d must be 1..%d.\n", nameid, ro_pool.max_slots, MAX_ITEM_RDM_OPT);
					return 0;
				}

				if (!this->nodeExists(poolNode, "Groups")) {
					this->invalidWarning(poolNode, "Id %u pool is missing Groups.\n", nameid);
					return 0;
				}

				const auto& groupsNode = poolNode["Groups"];
				if (!groupsNode.is_seq()) {
					this->invalidWarning(poolNode["Groups"], "Id %u pool Groups must be a sequence of one group id.\n", nameid);
					return 0;
				}

				int32 group_count = 0;
				uint16 group_id = 0;
				for (const ryml::NodeRef& groupNode : groupsNode) {
					if (group_count >= 1) {
						this->invalidWarning(poolNode["Groups"], "Id %u pool Groups must have exactly one group id.\n", nameid);
						return 0;
					}
					try {
						groupNode >> group_id;
					} catch (const std::runtime_error&) {
						this->invalidWarning(groupNode, "Id %u pool has a Groups value that is not a number.\n", nameid);
						return 0;
					}
					if (group_id < 1) {
						this->invalidWarning(groupNode, "Id %u pool has invalid group id %hu.\n", nameid, group_id);
						return 0;
					}
					++group_count;
				}
				if (group_count != 1) {
					this->invalidWarning(poolNode["Groups"], "Id %u pool Groups must have exactly one group id.\n", nameid);
					return 0;
				}
				ro_pool.groups.push_back(group_id);

				if (this->nodeExists(poolNode, "OptionSlots")) {
					const auto& optNode = poolNode["OptionSlots"];
					if (!optNode.is_seq() || static_cast<int32>(optNode.num_children()) != ro_pool.max_slots) {
						this->invalidWarning(poolNode["OptionSlots"], "Id %u OptionSlots length must equal MaxSlots %d.\n", nameid, ro_pool.max_slots);
						return 0;
					}
					int32 slot_i = 0;
					for (const ryml::NodeRef& slotNode : optNode) {
						int32 yaml_slot = 0;
						try {
							slotNode >> yaml_slot;
						} catch (const std::runtime_error&) {
							this->invalidWarning(slotNode, "Id %u OptionSlots value is not a number.\n", nameid);
							return 0;
						}
						if (yaml_slot < 1 || yaml_slot > MAX_ITEM_RDM_OPT) {
							this->invalidWarning(slotNode, "Id %u OptionSlots value %d must be 1..%d.\n", nameid, yaml_slot, MAX_ITEM_RDM_OPT);
							return 0;
						}
						ro_pool.option_slots[slot_i] = yaml_slot;
						++slot_i;
					}
				} else {
					for (int32 i = 0; i < ro_pool.max_slots; ++i)
						ro_pool.option_slots[i] = i + 1;
				}

				pool->ro_pools.push_back(std::move(ro_pool));
			}
		}

		this->put(nameid, pool);
		return 1;
	}

	void loadingFinished() override {
		if (this->empty()) {
			ShowError("Silvervine enchant YAML loaded zero items.\n");
			this->loaded_ok = false;
			return;
		}

		std::vector<t_itemid> drop_ids;

		for (const auto& pair : *this) {
			const auto& pool = pair.second;
			bool drop = false;

			if (sve_has_card_mode(pool->mode)) {
				for (const auto& step : pool->steps) {
					for (const auto& entry : step.enchants) {
						if (item_db.find(entry.item_id) == nullptr) {
							ShowError("Silvervine enchant Id %u references missing charm %u.\n", pool->id, entry.item_id);
							drop = true;
							break;
						}
					}
					if (drop)
						break;
				}
			}
			if (!drop && sve_has_ro_mode(pool->mode)) {
				for (const auto& ro_pool : pool->ro_pools) {
					if (ro_pool.groups.size() != 1) {
						ShowError("Silvervine enchant Id %u pool \"%s\" does not have exactly one group.\n", pool->id, ro_pool.label.c_str());
						drop = true;
						break;
					}
					auto group = random_option_group.find(ro_pool.groups[0]);
					if (group == nullptr) {
						ShowError("Silvervine enchant Id %u references missing Random Option group %hu.\n", pool->id, ro_pool.groups[0]);
						drop = true;
						break;
					}
					for (int32 i = 0; i < ro_pool.max_slots; ++i) {
						const uint16 slot_key = static_cast<uint16>(ro_pool.option_slots[i] - 1);
						if (group->slots.find(slot_key) == group->slots.end()) {
							ShowError("Silvervine enchant Id %u pool \"%s\" OptionSlots %d is missing YAML Slot %d on group %hu.\n",
								pool->id, ro_pool.label.c_str(), i + 1, ro_pool.option_slots[i], ro_pool.groups[0]);
							drop = true;
							break;
						}
					}
					if (drop)
						break;
				}
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
		ShowStatus("Silvervine enchant: %zu item pool(s) loaded.\n", this->size());
	}
};

inline SilvervineEnchantDatabase silvervine_enchant_db;

inline size_t sve_pick_index(size_t n) {
	if (n == 0)
		return SIZE_MAX;
	if (n == 1)
		return 0;
	return rnd_value<size_t>(0, n - 1);
}

inline int32 sve_roll_charm(const sve_step& step) {
	if (step.enchants.empty())
		return 0;
	if (step.slot_chance < 100000 && rnd_value<uint32>(1, 100000) > step.slot_chance)
		return -1;

	const size_t count = step.enchants.size();
	const size_t maximum = 3 * count;
	for (size_t attempt = 0; attempt < maximum; ++attempt) {
		const size_t index = sve_pick_index(count);
		if (index >= count)
			return 0;
		const auto& entry = step.enchants[index];
		if (rnd_value<uint16>(0, 9999) < entry.chance)
			return static_cast<int32>(entry.item_id);
	}

	const size_t fallback = sve_pick_index(count);
	if (fallback >= count)
		return 0;
	return static_cast<int32>(step.enchants[fallback].item_id);
}

inline int32 sve_find_step_index(const sve_item_pool& pool, int32 card_index) {
	for (size_t i = 0; i < pool.steps.size(); ++i) {
		if (pool.steps[i].card_index == card_index)
			return static_cast<int32>(i);
	}
	return -1;
}

inline bool sve_roll_ro(t_itemid item_id, int32 pool_index, int32 option_index, int32& out_id, int32& out_val, int32& out_param) {
	out_id = 0;
	out_val = 0;
	out_param = 0;

	auto pool = silvervine_enchant_db.find(item_id);
	if (pool == nullptr || !sve_has_ro_mode(pool->mode) || pool->ro_pools.empty())
		return false;
	if (pool_index < 0 || pool_index >= static_cast<int32>(pool->ro_pools.size()))
		return false;

	const sve_ro_pool& ro_pool = pool->ro_pools[pool_index];
	if (option_index < 0 || option_index >= ro_pool.max_slots)
		return false;
	if (ro_pool.groups.size() != 1)
		return false;

	auto group = random_option_group.find(ro_pool.groups[0]);
	if (group == nullptr)
		return false;

	const uint16 slot_key = static_cast<uint16>(ro_pool.option_slots[option_index] - 1);
	auto slot_it = group->slots.find(slot_key);
	if (slot_it == group->slots.end() || slot_it->second.empty())
		return false;

	const auto& entries = slot_it->second;
	const size_t count = entries.size();
	const size_t maximum = 3 * count;
	for (size_t attempt = 0; attempt < maximum; ++attempt) {
		const size_t index = sve_pick_index(count);
		if (index >= count)
			return false;
		const auto& option = entries[index];
		if (rnd_value<uint16>(0, 9999) < option->chance) {
			out_id = option->id;
			out_val = rnd_value(option->min_value, option->max_value);
			out_param = option->param;
			return true;
		}
	}

	const size_t fallback = sve_pick_index(count);
	if (fallback >= count)
		return false;
	const auto& option = entries[fallback];
	out_id = option->id;
	out_val = rnd_value(option->min_value, option->max_value);
	out_param = option->param;
	return true;
}

#endif /* SILVERVINE_ENCHANT_HPP */
