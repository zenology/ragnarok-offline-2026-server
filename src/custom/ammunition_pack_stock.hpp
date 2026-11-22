// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
#ifndef AMMUNITION_PACK_STOCK_HPP
#define AMMUNITION_PACK_STOCK_HPP

#include <limits>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <common/showmsg.hpp>
#include "../map/itemdb.hpp"
#include "../map/npc.hpp"
#include "yaml_reader.hpp"

namespace aps {
inline constexpr char path[] = "npc/custom/ammunition_pack_stock.yml";
struct Entry { t_itemid id; int32 price; int32 stock; };
struct Shop { std::string name; std::vector<Entry> entries; };
// Typed runtime cache for constant-time script getters; no YAML model binding.
inline std::vector<Shop> snapshot;

inline int64 load() {
	std::string context = "root";
	try {
		const auto config = custom_yaml::read_file(path);
		config.require_fields({"Header", "Body"});
		config.at("Header").require_fields({"Type", "Version"});
		if (config.get<std::string>("Header.Type") != "AMMUNITION_PACK_STOCK")
			throw custom_yaml::Error("Header.Type must be AMMUNITION_PACK_STOCK");
		if (config.get<uint16>("Header.Version") != 1) throw custom_yaml::Error("Header.Version must be 1");
		const auto body = config.at("Body");
		if (!body.is_sequence()) throw custom_yaml::Error("Body must be a sequence");
		const size_t count = body.size();
		if (!count || count > static_cast<size_t>(std::numeric_limits<int32>::max()))
			throw custom_yaml::Error("Body must contain a nonempty script-addressable shop list");
		std::vector<Shop> pending;
		pending.reserve(count);
		std::set<std::string> names;
		for (const auto shop_node : body) {
			context = "Body[" + std::to_string(pending.size()) + "]";
			shop_node.require_fields({"Shop", "Items"});
			Shop shop{shop_node.get<std::string>("Shop"), {}};
			if (shop.name.empty() || shop.name.find_first_of("\r\n") != std::string::npos)
				throw custom_yaml::Error("Shop must be nonempty without newline");
			context += " Shop " + shop.name;
			if (!names.insert(shop.name).second) throw custom_yaml::Error("duplicate Shop");
			auto nd = npc_name2id(shop.name.c_str());
			if (!nd || nd->subtype != NPCTYPE_MARKETSHOP)
				throw custom_yaml::Error("missing shop or not NPCTYPE_MARKETSHOP");
			const auto items = shop_node.at("Items");
			if (!items.is_sequence()) throw custom_yaml::Error("Items must be a sequence");
			const size_t rows = items.size();
			if (!rows || rows > std::numeric_limits<uint16>::max())
				throw custom_yaml::Error("Items must contain 1..UINT16_MAX entries");
			shop.entries.reserve(rows);
			std::set<t_itemid> ids;
			const std::string shop_context = context;
			for (const auto row : items) {
				context = shop_context + ".Items[" + std::to_string(shop.entries.size()) + "]";
				row.require_fields({"Id", "Price", "Stock"});
				const Entry entry{row.get<t_itemid>("Id"), row.get<int32>("Price"), row.get<int32>("Stock")};
				context += " Id " + std::to_string(entry.id);
				if (!entry.id) throw custom_yaml::Error("Id must be positive");
				if (!ids.insert(entry.id).second) throw custom_yaml::Error("duplicate Id within Shop");
				auto item = item_db.find(entry.id);
				if (!item) throw custom_yaml::Error("Id does not exist in item DB");
				if (entry.price < -1) throw custom_yaml::Error("Price must be -1 or nonnegative");
				if (entry.stock != -1) throw custom_yaml::Error("Stock must be -1");
				if (entry.price == -1 && item->value_buy > static_cast<uint32>(std::numeric_limits<int32>::max()))
					throw custom_yaml::Error("invalid DB Buy for Price -1");
				shop.entries.push_back(entry);
			}
			pending.push_back(std::move(shop));
		}
		snapshot.swap(pending);
		return static_cast<int64>(snapshot.size());
	} catch (const std::exception& error) {
		snapshot.clear();
		ShowError("Ammunition pack stock %s: %s: %s.\n", path, context.c_str(), error.what());
		return 0;
	}
}

inline const Shop* shop_at(int64 index) {
	if (index < 0 || static_cast<uint64>(index) >= snapshot.size()) {
		ShowError("Ammunition pack stock %s: invalid shop index %lld.\n", path, static_cast<long long>(index));
		return nullptr;
	}
	return &snapshot[static_cast<size_t>(index)];
}
inline const Entry* entry_at(int64 shop_index, int64 row_index) {
	const auto shop = shop_at(shop_index);
	if (!shop) return nullptr;
	if (row_index < 0 || static_cast<uint64>(row_index) >= shop->entries.size()) {
		ShowError("Ammunition pack stock %s: Shop %s invalid row index %lld.\n",
			path, shop->name.c_str(), static_cast<long long>(row_index));
		return nullptr;
	}
	return &shop->entries[static_cast<size_t>(row_index)];
}
} // namespace aps
#endif
