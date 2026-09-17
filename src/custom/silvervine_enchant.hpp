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
	SVE_MODE_RO = 2
};

struct sve_enchant_entry {
	t_itemid item_id = 0;
	uint16 chance = 10000;
};

struct sve_step {
	int32 card_index = 0;
	int32 grade = 0;
	uint32 slot_chance = 100000;
	std::vector<sve_enchant_entry> enchants;
};

struct sve_item_pool {
	t_itemid id = 0;
	sve_mode mode = SVE_MODE_NONE;
	int32 min_refine = 0;
	int32 min_enchantgrade = 0;
	std::vector<sve_step> steps;
	std::vector<uint16> ro_groups;
};

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

		if (!this->asUInt32(node, "Id", nameid)) {
			this->loaded_ok = false;
			return 0;
		}

		if (this->find(nameid) != nullptr) {
			this->invalidWarning(node["Id"], "Duplicate Silvervine enchant Id %u.\n", nameid);
			this->loaded_ok = false;
			return 0;
		}

		std::string mode_name;
		if (!this->asString(node, "Mode", mode_name)) {
			this->loaded_ok = false;
			return 0;
		}

		auto pool = std::make_shared<sve_item_pool>();
		pool->id = nameid;

		if (mode_name == "CardSlot") {
			pool->mode = SVE_MODE_CARD;
		} else if (mode_name == "RandomOption") {
			pool->mode = SVE_MODE_RO;
		} else {
			this->invalidWarning(node["Mode"], "Unknown Mode \"%s\" for Id %u.\n", mode_name.c_str(), nameid);
			this->loaded_ok = false;
			return 0;
		}

		if (this->nodeExists(node, "MinRefine")) {
			if (!this->asInt32(node, "MinRefine", pool->min_refine)) {
				this->loaded_ok = false;
				return 0;
			}
		}

		if (this->nodeExists(node, "MinEnchantgrade")) {
			if (!this->asInt32(node, "MinEnchantgrade", pool->min_enchantgrade)) {
				this->loaded_ok = false;
				return 0;
			}
		}

		const bool has_steps = this->nodeExists(node, "Steps");
		const bool has_groups = this->nodeExists(node, "Groups");
		if (has_steps && has_groups) {
			this->invalidWarning(node, "Id %u has both Steps and Groups.\n", nameid);
			this->loaded_ok = false;
			return 0;
		}

		if (pool->mode == SVE_MODE_CARD) {
			if (!has_steps) {
				this->invalidWarning(node, "CardSlot Id %u is missing Steps.\n", nameid);
				this->loaded_ok = false;
				return 0;
			}

			const auto& stepsNode = node["Steps"];
			if (!stepsNode.is_seq() || stepsNode.num_children() < 1) {
				this->invalidWarning(node["Steps"], "CardSlot Id %u has empty Steps.\n", nameid);
				this->loaded_ok = false;
				return 0;
			}

			for (const ryml::NodeRef& stepNode : stepsNode) {
				sve_step step;

				if (!this->asInt32(stepNode, "CardIndex", step.card_index)) {
					this->loaded_ok = false;
					return 0;
				}
				if (step.card_index < 0 || step.card_index > 3) {
					this->invalidWarning(stepNode["CardIndex"], "CardIndex %d for Id %u must be 0..3.\n", step.card_index, nameid);
					this->loaded_ok = false;
					return 0;
				}

				if (this->nodeExists(stepNode, "Grade")) {
					if (!this->asInt32(stepNode, "Grade", step.grade)) {
						this->loaded_ok = false;
						return 0;
					}
				}

				if (this->nodeExists(stepNode, "SlotChance")) {
					if (!this->asUInt32(stepNode, "SlotChance", step.slot_chance)) {
						this->loaded_ok = false;
						return 0;
					}
				}

				if (!this->nodeExists(stepNode, "Enchants")) {
					this->invalidWarning(stepNode, "Step CardIndex %d for Id %u is missing Enchants.\n", step.card_index, nameid);
					this->loaded_ok = false;
					return 0;
				}

				const auto& enchantsNode = stepNode["Enchants"];
				if (!enchantsNode.is_seq() || enchantsNode.num_children() < 1) {
					this->invalidWarning(stepNode["Enchants"], "Step CardIndex %d for Id %u has empty Enchants.\n", step.card_index, nameid);
					this->loaded_ok = false;
					return 0;
				}

				for (const ryml::NodeRef& enchantNode : enchantsNode) {
					sve_enchant_entry entry;
					if (!this->asUInt32(enchantNode, "Item", entry.item_id)) {
						this->loaded_ok = false;
						return 0;
					}
					if (entry.item_id < 1) {
						this->invalidWarning(enchantNode, "Enchant Item for Id %u must be positive.\n", nameid);
						this->loaded_ok = false;
						return 0;
					}
					if (this->nodeExists(enchantNode, "Chance")) {
						if (!this->asUInt16(enchantNode, "Chance", entry.chance)) {
							this->loaded_ok = false;
							return 0;
						}
					}
					step.enchants.push_back(entry);
				}

				pool->steps.push_back(std::move(step));
			}
		} else {
			if (!has_groups) {
				this->invalidWarning(node, "RandomOption Id %u is missing Groups.\n", nameid);
				this->loaded_ok = false;
				return 0;
			}

			const auto& groupsNode = node["Groups"];
			if (!groupsNode.is_seq() || groupsNode.num_children() < 1) {
				this->invalidWarning(node["Groups"], "RandomOption Id %u has empty Groups.\n", nameid);
				this->loaded_ok = false;
				return 0;
			}

			for (const ryml::NodeRef& groupNode : groupsNode) {
				uint16 group_id = 0;
				try {
					groupNode >> group_id;
				} catch (const std::runtime_error&) {
					this->invalidWarning(groupNode, "RandomOption Id %u has a Groups value that is not a number.\n", nameid);
					this->loaded_ok = false;
					return 0;
				}
				if (group_id < 1) {
					this->invalidWarning(groupNode, "RandomOption Id %u has invalid group id %hu.\n", nameid, group_id);
					this->loaded_ok = false;
					return 0;
				}
				pool->ro_groups.push_back(group_id);
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

		for (const auto& pair : *this) {
			const auto& pool = pair.second;
			if (pool->mode == SVE_MODE_CARD) {
				for (const auto& step : pool->steps) {
					for (const auto& entry : step.enchants) {
						if (item_db.find(entry.item_id) == nullptr) {
							ShowError("Silvervine enchant Id %u references missing charm %u.\n", pool->id, entry.item_id);
							this->loaded_ok = false;
						}
					}
				}
			} else if (pool->mode == SVE_MODE_RO) {
				for (uint16 group_id : pool->ro_groups) {
					if (random_option_group.find(group_id) == nullptr) {
						ShowError("Silvervine enchant Id %u references missing Random Option group %hu.\n", pool->id, group_id);
						this->loaded_ok = false;
					}
				}
			}
		}

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

inline int32 sve_find_step_index(const sve_item_pool& pool, int32 card_index, int32 grade) {
	int32 fallback = -1;
	for (size_t i = 0; i < pool.steps.size(); ++i) {
		if (pool.steps[i].card_index != card_index)
			continue;
		if (pool.steps[i].grade == grade)
			return static_cast<int32>(i);
		if (pool.steps[i].grade == 0 && fallback < 0)
			fallback = static_cast<int32>(i);
	}
	return fallback;
}

inline bool sve_roll_ro(t_itemid item_id, int32 want, int32 out_id[MAX_ITEM_RDM_OPT], int32 out_val[MAX_ITEM_RDM_OPT], int32 out_param[MAX_ITEM_RDM_OPT]) {
	for (int32 i = 0; i < MAX_ITEM_RDM_OPT; ++i) {
		out_id[i] = 0;
		out_val[i] = 0;
		out_param[i] = 0;
	}

	auto pool = silvervine_enchant_db.find(item_id);
	if (pool == nullptr || pool->mode != SVE_MODE_RO || pool->ro_groups.empty())
		return false;
	if (want < 1 || want > MAX_ITEM_RDM_OPT)
		return false;

	for (int32 n = 0; n < want; ++n) {
		bool picked = false;
		for (int32 attempt = 0; attempt < 12 && !picked; ++attempt) {
			const size_t group_index = sve_pick_index(pool->ro_groups.size());
			if (group_index >= pool->ro_groups.size())
				return false;
			auto group = random_option_group.find(pool->ro_groups[group_index]);
			if (group == nullptr)
				return false;

			struct item tmp{};
			group->apply(tmp);

			int32 cand_idx[MAX_ITEM_RDM_OPT];
			int32 cand = 0;
			for (int32 j = 0; j < MAX_ITEM_RDM_OPT; ++j) {
				if (tmp.option[j].id == 0)
					continue;
				bool duplicate = false;
				for (int32 prior = 0; prior < n; ++prior) {
					if (out_id[prior] == tmp.option[j].id) {
						duplicate = true;
						break;
					}
				}
				if (duplicate)
					continue;
				cand_idx[cand++] = j;
			}
			if (cand < 1)
				continue;

			const size_t pick = sve_pick_index(static_cast<size_t>(cand));
			if (pick >= static_cast<size_t>(cand))
				return false;
			const int32 j = cand_idx[pick];
			out_id[n] = tmp.option[j].id;
			out_val[n] = tmp.option[j].value;
			out_param[n] = tmp.option[j].param;
			picked = true;
		}
		if (!picked)
			return false;
	}

	return true;
}

#endif /* SILVERVINE_ENCHANT_HPP */
