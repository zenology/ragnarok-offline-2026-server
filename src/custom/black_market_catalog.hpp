// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Included only by custom/script.inc. No changes to the native pricing path.
#ifndef BLACK_MARKET_CATALOG_HPP
#define BLACK_MARKET_CATALOG_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <common/database.hpp>

namespace bmc {
enum class Kind { Equipment, Hat, Specialty, Card, Rooke };
struct Profile {
	std::string key;
	Kind kind;
	std::vector<std::string> labels, prefixes;
	const char* rate;
	int tier = 0;
	std::string path() const { return "npc/custom/black_market_catalog/" + key + ".yml"; }
};

inline const std::vector<Profile> profiles = {
	{"harlan", Kind::Hat, {"Top", "Middle", "Lower"}, {"hat0", "hat1", "hat2"}, "$BlackMarketRateHarlan"},
	{"soren", Kind::Equipment,
		{"Book", "Bow", "Dagger", "Gatling Gun", "Grenade Launcher", "Huuma Shuriken", "Instrument", "Katar", "Knuckle", "Mace", "One-Handed Axe", "One-Handed Spear", "One-Handed Sword", "Revolver", "Rifle", "Shotgun", "Staff", "Two-Handed Axe", "Two-Handed Spear", "Two-Handed Staff", "Two-Handed Sword", "Whip"},
		{"wep0_0", "wep1_0", "wep2_0", "wep3_0", "wep4_0", "wep5_0", "wep6_0", "wep7_0", "wep8_0", "wep9_0", "wep10_0", "wep11_0", "wep12_0", "wep13_0", "wep14_0", "wep15_0", "wep16_0", "wep17_0", "wep18_0", "wep19_0", "wep20_0", "wep21_0"}, "$BlackMarketRateSoren"},
	{"mordain", Kind::Equipment, {"Armor"}, {"arm0"}, "$BlackMarketRateMordain"},
	{"kaedra", Kind::Equipment, {"Shield"}, {"shd0"}, "$BlackMarketRateKaedra"},
	{"tess", Kind::Equipment, {"Footgear"}, {"ftr0"}, "$BlackMarketRateTess"},
	{"weaver", Kind::Equipment, {"Garment"}, {"gar0"}, "$BlackMarketRateWeaver"},
	{"lady-seraphine", Kind::Equipment, {"Accessory", "Left Accessory", "Right Accessory"}, {"acc0", "acl0", "acr0"}, "$BlackMarketRateSeraphine"},
	{"madame-celestine", Kind::Specialty,
		{"Abyss Glastheim", "Abyss Lake Equipment", "Adulter and Vivatus Fides Weapons", "Airship Set", "Ancient Hero", "Automatic Equipment", "Biolab 5th Floor Equipment", "Biolab Gear", "Brave Heart", "Brilliant Light", "Charleston Crisis", "Circulation of Life", "Clergy Equipment", "Constellation Tower Gear", "Convertible Gears", "Dimension Equipment", "Edda Equipment", "Einbech Weapons", "Engraved Equipment", "Enhanced Time Guardian Equipment", "Entwined Equipment", "Evil Slayer", "Excellion Gear", "Ferlock Equipment", "Fortified Weapons", "Frontier", "Frontier Rune Crowns", "Furious Equipment", "Gaebolg Equipment", "Geffen Magic Tournament", "Geffen Night Arena", "Glacier Equipment", "Godlike Equipment", "Good and Evil", "Grace Equipment", "Gray Equipment", "Gray Wolf Equipment", "Helms of Faith", "Heroic Equipment", "Horror Toy Equipment", "Illusion of Abyss", "Illusion of Frozen", "Illusion of Labyrinth", "Illusion of Luanda", "Illusion of Moonlight", "Illusion of Teddy Bear", "Illusion of Twins", "Illusion of Underwater", "Illusion of Vampire", "Illusion/OS Gears", "Imperial Equipment", "Imperial Set", "Infinity Weapons", "Issgard Equipment", "King Schmidt's Equipment", "Magma Equipment", "Memories of Thanatos Equipment", "Mora Equipment", "Muqaddas Weapons", "Nightmarish Jitterburg Equipment", "Noblesse Set", "Poenitentia", "Precision Weapons", "Racing Caps", "Relapse Weapons", "Rift Armor", "Royal and Guardian Weapons", "Royal Banquet / Royal Guard Shield", "Runaway Chips", "Rune Equipment", "Sarah's Gear", "Schwartz's Equipment", "Scrap Weapons", "Sin Weapons", "Solid Weapons", "Soutane Equipment", "Stardust Equipment", "Supplement Part Equipment", "Sweet Candy Backpack", "Temporal Armors", "Thanatos Equipment", "Time Dimension Rune Crown", "Time Gap Weapons", "Upgrade Part Equipment", "Varmundt Equipment", "Vicious Mind", "Werner's Laboratory", "Wicked Weapons", "Y.S.F.01 Set", "Yorscalp Equipment"},
		{"spc#S00", "spc#S01", "spc#S02", "spc#S03", "spc#S04", "spc#S05", "spc#S06", "spc#S07", "spc#S08", "spc#S09", "spc#S10", "spc#S11", "spc#S12", "spc#S13", "spc#S14", "spc#S15", "spc#S16", "spc#S17", "spc#S18", "spc#S19", "spc#S20", "spc#S21", "spc#S22", "spc#S23", "spc#S24", "spc#S25", "spc#S26", "spc#S27", "spc#S28", "spc#S29", "spc#S30", "spc#S31", "spc#S32", "spc#S33", "spc#S34", "spc#S35", "spc#S36", "spc#S37", "spc#S38", "spc#S39", "spc#S40", "spc#S41", "spc#S42", "spc#S43", "spc#S44", "spc#S45", "spc#S46", "spc#S47", "spc#S48", "spc#S49", "spc#S50", "spc#S51", "spc#S52", "spc#S53", "spc#S54", "spc#S55", "spc#S56", "spc#S57", "spc#S58", "spc#S59", "spc#S60", "spc#S61", "spc#S62", "spc#S63", "spc#S64", "spc#S65", "spc#S66", "spc#S67", "spc#S68", "spc#S69", "spc#S70", "spc#S71", "spc#S72", "spc#S73", "spc#S74", "spc#S75", "spc#S76", "spc#S77", "spc#S78", "spc#S79", "spc#S80", "spc#S81", "spc#S82", "spc#S83", "spc#S84", "spc#S85", "spc#S86", "spc#S87", "spc#S88", "spc#S89"}, "$BlackMarketRateSpecialty"},
	{"rooke", Kind::Rooke, {"EXP and Drop", "Consumable#1", "Consumable#2", "Scrolls", "Potions", "Convenience", "Style / Reset", "Upgrade", "Premium Service", "Battle Tools"}, {}, nullptr},
	{"card-normal", Kind::Card, {"Normal"}, {"card_mob"}, "$BlackMarketRateCards", 1},
	{"card-miniboss", Kind::Card, {"Miniboss"}, {"card_miniboss"}, "$BlackMarketRateCards", 2},
	{"card-mvp", Kind::Card, {"MVP"}, {"card_mvp"}, "$BlackMarketRateCards", 3},
	{"card-special", Kind::Card, {"Special"}, {"card_special"}, "$BlackMarketRateCards", 4},
};
inline const std::array<const char*, 6> tier_keys = {"Normal1_50", "Normal51_99", "Rebirth1_50", "Rebirth51_99", "Level100_200", "Level201Plus"};
inline const std::vector<std::string> suffixes = {"ALL", "AH", "IP", "QZ9", "0", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
struct Record {
	t_itemid id = 0;
	uint32 price = 0, offer = 0, source_mob = 0, category = 0;
	std::string name;
	std::array<uint32, 6> tiers{};
};
inline std::string lower(std::string value) {
	for (char& c : value) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
	return value;
}
inline int letter(const std::string& name, bool strict) {
	if (!name.empty()) {
		char c = name.front();
		if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
		if (c >= 'A' && c <= 'Z') return c - 'A';
		if (c >= '0' && c <= '9') return 26;
	}
	return strict ? -1 : 0;
}
inline void yaml_error(const char* message, size_t length, ryml::Location location, void*) {
	throw std::runtime_error(std::string(message, length) + " at line " +
		std::to_string(location.line + 1) + ", column " + std::to_string(location.col + 1));
}

class Database : public TypesafeYamlDatabase<t_itemid, Record> {
	const Profile& profile;
	std::string record_context;
	bool fail(const ryml::NodeRef& node, const char* reason) {
		valid = false;
		this->invalidWarning(node, "Black Market catalog %s %s: %s.\n", profile.path().c_str(), record_context.c_str(), reason);
		return false;
	}
	bool number(const ryml::NodeRef& node, const char* field, uint32& out, bool zero = false) {
		if (!node.is_map() || !this->nodeExists(node, field)) return fail(node, (std::string("missing field ") + field).c_str());
		auto value = node[c4::to_csubstr(field)];
		if (!value.has_val() || value.is_val_quoted() || value.val().empty()) return fail(value, "expected an unquoted integer");
		if (value.has_val_tag() || (value.val().len > 1 && value.val().str[0] == '0')) return fail(value, "expected a decimal integer without a tag or leading zero");
		uint64 result = 0;
		for (char c : value.val()) {
			if (c < '0' || c > '9') return fail(value, "expected a nonnegative decimal integer");
			result = result * 10 + c - '0';
			if (result > INT32_MAX) return fail(value, "integer exceeds INT32_MAX");
		}
		if (!zero && result == 0) return fail(value, "value must be positive");
		out = static_cast<uint32>(result);
		return true;
	}
public:
	bool valid = true;
	std::vector<std::shared_ptr<Record>> ordered;
	explicit Database(const Profile& p) : TypesafeYamlDatabase("BLACK_MARKET_CATALOG", 1), profile(p) {}
	const std::string getDefaultLocation() override { return profile.path(); }
	void clear() override { TypesafeYamlDatabase::clear(); ordered.clear(); valid = true; }
	// Validate the envelope before the common loader can process Footer.Imports.
	bool load_catalog() {
		std::ifstream input(profile.path());
		if (!input) { ShowError("Black Market catalog %s: cannot open file.\n", profile.path().c_str()); return false; }
		std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		try {
			auto callbacks = ryml::get_callbacks();
			callbacks.m_error = yaml_error;
			ryml::Parser preflight(callbacks);
			auto tree = preflight.parse_in_arena(c4::to_csubstr(profile.path()), c4::to_csubstr(text));
			auto root = tree.rootref();
			auto envelope_error = [&](const ryml::NodeRef& node, const char* reason) {
				auto location = preflight.location(node);
				ShowError("Black Market catalog %s: %s at line %zu, column %zu.\n", profile.path().c_str(), reason, location.line + 1, location.col + 1);
				return false;
			};
			if (!root.is_map() || root.num_children() != 2 || !root.has_child("Header") || !root.has_child("Body") || !root["Body"].is_seq()) {
				return envelope_error(root, "requires Header and sequence Body only; imports are unsupported");
			}
			auto header = root["Header"];
			if (!header.is_map() || header.num_children() != 2 || !header.has_child("Type") || !header.has_child("Version")) {
				return envelope_error(header, "invalid Header");
			}
			if (!header["Version"].has_val() || header["Version"].is_val_quoted() || header["Version"].has_val_tag() || header["Version"].val() != "1" ||
				!header["Type"].has_val() || header["Type"].val() != "BLACK_MARKET_CATALOG") {
				return envelope_error(header, "expected BLACK_MARKET_CATALOG Version 1");
			}
		} catch (const std::runtime_error& error) {
			ShowError("Black Market catalog %s: malformed YAML: %s\n", profile.path().c_str(), error.what()); return false;
		}
		bool opened = this->load();
		return opened && valid;
	}
	uint64 parseBodyNode(const ryml::NodeRef& node) override {
		record_context.clear();
		if (!node.is_map()) { fail(node, "record must be a mapping"); return 0; }
		for (const char* field : {"Id", "Category"}) if (node.has_child(c4::to_csubstr(field)) && node[c4::to_csubstr(field)].has_val()) {
			auto value = node[c4::to_csubstr(field)].val();
			record_context += std::string(field) + "=" + std::string(value.str, value.len) + " ";
		}
		std::set<std::string> fields;
		for (const auto& child : node) {
			std::string key(child.key().str, child.key().len);
			if (!fields.insert(key).second) { fail(child, "duplicate field"); return 0; }
		}
		auto row = std::make_shared<Record>();
		if (!number(node, "Id", row->id)) return 0;
		if (this->exists(row->id)) { fail(node["Id"], "duplicate Id"); return 0; }
		if (!item_db.find(row->id)) { fail(node["Id"], "Id does not exist in item DB"); return 0; }
		bool is_rooke = profile.kind == Kind::Rooke;
		bool has_category = profile.kind == Kind::Equipment || profile.kind == Kind::Specialty || is_rooke;
		std::set<std::string> expected = {"Id"};
		if (has_category) {
			expected.insert("Category");
			if (!number(node, "Category", row->category, true)) return 0;
			if (row->category >= profile.labels.size()) { fail(node["Category"], "Category outside profile range"); return 0; }
		}
		if (is_rooke) {
			expected.insert("TierPrices");
			if (!node.has_child("TierPrices") || !node["TierPrices"].is_map() || node["TierPrices"].num_children() != 6) { fail(node, "TierPrices requires all six tiers"); return 0; }
			for (size_t i = 0; i < tier_keys.size(); ++i) if (!number(node["TierPrices"], tier_keys[i], row->tiers[i], true)) return 0;
		} else {
			expected.insert("Price"); expected.insert("DisplayName");
			if (!number(node, "Price", row->price)) return 0;
			if (!node.has_child("DisplayName") || !node["DisplayName"].has_val() || node["DisplayName"].has_val_tag() || !this->asString(node, "DisplayName", row->name) || row->name.empty()) { fail(node, "DisplayName requires a nonempty string without an explicit tag"); return 0; }
			if (!node["DisplayName"].is_val_quoted()) {
				auto plain = lower(row->name);
				if (plain == "true" || plain == "false" || plain == "yes" || plain == "no" || plain == "on" || plain == "off" || plain == "null" || plain == "~" ||
					std::all_of(plain.begin(), plain.end(), [](char c) { return (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '+'; })) {
					fail(node["DisplayName"], "DisplayName scalar has a non-string type; quote it"); return 0;
				}
			}
			if (profile.kind == Kind::Card) {
				expected.insert("CollectorOffer"); expected.insert("SourceMob");
				if (!number(node, "CollectorOffer", row->offer) || !number(node, "SourceMob", row->source_mob, profile.tier == 4)) return 0;
				if (profile.tier == 4 && row->source_mob != 0) { fail(node["SourceMob"], "Special SourceMob must be zero"); return 0; }
			}
		}
		if (fields != expected) { fail(node, "missing or unexpected record field"); return 0; }
		this->put(row->id, row);
		ordered.push_back(row);
		return 1;
	}
};

struct Drawer { std::string shop, label; };
struct Catalog {
	bool ready = false;
	std::shared_ptr<Database> db;
	std::map<uint32, std::vector<Drawer>> drawers;
};
inline std::map<std::string, Catalog> catalogs;
inline std::map<std::string, std::string> shop_owner;
inline std::vector<std::pair<std::shared_ptr<Record>, const Profile*>> search_rows;
inline bool search_ready = false;
using Inventory = std::vector<std::pair<t_itemid, uint32>>;
using Inventories = std::map<std::string, Inventory>;

inline const Profile* profile_for(const std::string& key) {
	for (const auto& p : profiles) if (p.key == key) return &p;
	return nullptr;
}
inline bool ready(const std::string& key) {
	if (key == "riven") return search_ready;
	auto it = catalogs.find(key);
	return it != catalogs.end() && it->second.ready;
}
inline npc_data* shop(const std::string& name) {
	auto nd = npc_name2id(name.c_str());
	return nd && nd->subtype == NPCTYPE_POINTSHOP && strcmp(nd->u.shop.pointshop_str, BLACK_MARKET_POINT_VAR) == 0 ? nd : nullptr;
}
inline void clear_shop(const std::string& name) {
	// Native npcshopitem supports these shop-list types. Clear an owned shell
	// even when its declaration was mistakenly changed to another shop type.
	auto nd = npc_name2id(name.c_str());
	if (nd && (nd->subtype == NPCTYPE_POINTSHOP || nd->subtype == NPCTYPE_SHOP ||
		nd->subtype == NPCTYPE_CASHSHOP || nd->subtype == NPCTYPE_ITEMSHOP || nd->subtype == NPCTYPE_MARKETSHOP)) {
		aFree(nd->u.shop.shop_item); nd->u.shop.shop_item = nullptr; nd->u.shop.count = 0;
	}
	black_market_catalog_cache_by_shop.erase(name);
}
inline Inventories empty_shops(const Profile& p) {
	Inventories result;
	if (p.kind == Kind::Rooke) {
		for (int c = 0; c < 10; ++c) for (int t = 0; t < 6; ++t) result["cons" + std::to_string(c) + "#T" + std::to_string(t)] = {};
		for (int t = 0; t < 6; ++t) result["rookup#T" + std::to_string(t)] = {};
	} else for (const auto& prefix : p.prefixes) {
		if (p.kind == Kind::Specialty) result[prefix] = {};
		else for (const auto& suffix : suffixes) result[prefix + "#" + suffix] = {};
	}
	return result;
}
inline bool admitted(const Profile& p, const Record& row) {
	auto item = item_db.find(row.id);
	if (!item) return false;
	if (p.kind == Kind::Equipment || p.kind == Kind::Specialty) return true;
	if (p.kind == Kind::Hat && (item->ename.empty() || item->ename == "Unknown Item" || item->ename == "null")) return false;
	if (p.kind == Kind::Rooke && (item->ename.empty() || item->ename == "Unknown Item")) return false;
	if (p.kind == Kind::Hat) return item->type == IT_ARMOR && !(item->equip & (EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW));
	if (p.kind == Kind::Card) return item->type == IT_CARD && item->subtype == CARD_NORMAL && letter(row.name, p.tier != 4) >= 0;
	if (p.kind == Kind::Rooke) return item->type == IT_USABLE || item->type == IT_HEALING || item->type == IT_DELAYCONSUME || item->type == IT_ETC || item->type == IT_CASH;
	return true;
}
inline std::string drawer_suffix(int initial, size_t count) {
	if (count < 100) return "ALL";
	if (count < 200) return initial <= 7 ? "AH" : initial <= 15 ? "IP" : "QZ9";
	return initial == 26 ? "0" : std::string(1, 'A' + initial);
}
inline std::string drawer_label(const std::string& suffix, bool cards) {
	if (cards && suffix == "ALL") return "All Cards";
	if (suffix == "AH") return cards ? "A-H Cards" : "A-H";
	if (suffix == "IP") return cards ? "I-P Cards" : "I-P";
	if (suffix == "QZ9") return cards ? "Q-Z / 0-9 Cards" : "Q-Z / 0-9";
	if (suffix == "0") return cards ? "0-9 Cards" : "0-9";
	if (cards) return suffix + " Cards";
	return suffix;
}

inline bool prepare(const Profile& p, Catalog& cat, Inventories& stock) {
	stock = empty_shops(p);
	for (const auto& entry : stock) {
		shop_owner[entry.first] = p.key;
		if (!shop(entry.first)) {
			ShowError("Black Market catalog %s: missing pointshop %s or wrong type/currency.\n", p.path().c_str(), entry.first.c_str()); return false;
		}
	}
	if (p.kind == Kind::Rooke) {
		for (const auto& row : cat.db->ordered) if (admitted(p, *row)) {
			for (int t = 0; t < 6; ++t) if (row->tiers[t]) {
				stock["cons" + std::to_string(row->category) + "#T" + std::to_string(t)].emplace_back(row->id, row->tiers[t]);
				if (row->category == 7) {
					uint64 price = (static_cast<uint64>(row->tiers[t]) * 120 + 50) / 100;
					if (price > INT32_MAX) { ShowError("Black Market catalog %s: Id %u Category 7 courier price overflow.\n", p.path().c_str(), row->id); return false; }
					stock["rookup#T" + std::to_string(t)].emplace_back(row->id, static_cast<uint32>(price));
				}
			}
		}
		return true;
	}
	for (uint32 category = 0; category < p.labels.size(); ++category) {
		std::vector<std::shared_ptr<Record>> rows;
		for (const auto& row : cat.db->ordered) {
			if (!admitted(p, *row)) continue;
			if (p.kind == Kind::Hat) {
				const uint32 locations[] = {EQP_HEAD_TOP, EQP_HEAD_MID, EQP_HEAD_LOW};
				if (!(item_db.find(row->id)->equip & locations[category])) continue;
			} else if (row->category != category) continue;
			rows.push_back(row);
		}
		if (p.kind != Kind::Card) std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) {
			bool adigit = letter(a->name, false) == 26, bdigit = letter(b->name, false) == 26;
			if (adigit != bdigit) return !adigit;
			auto aname = lower(a->name), bname = lower(b->name);
			return aname == bname ? a->id < b->id : aname < bname;
		});
		std::map<std::string, Inventory> grouped;
		for (const auto& row : rows) {
			auto suffix = p.kind == Kind::Specialty ? "" : drawer_suffix(letter(row->name, p.kind == Kind::Card && p.tier != 4), rows.size());
			grouped[suffix].emplace_back(row->id, row->price);
		}
		std::vector<std::string> order = {"ALL", "AH", "IP", "QZ9"};
		if (p.kind == Kind::Card) order.push_back("0");
		for (char c = 'A'; c <= 'Z'; ++c) order.emplace_back(1, c);
		if (p.kind != Kind::Card) order.push_back("0");
		if (p.kind == Kind::Specialty) order = {""};
		for (const auto& suffix : order) if (grouped.count(suffix)) {
			auto name = p.prefixes[category] + (suffix.empty() ? "" : "#" + suffix);
			stock[name] = std::move(grouped[suffix]);
			cat.drawers[category].push_back({name, drawer_label(suffix, p.kind == Kind::Card)});
		}
	}
	return true;
}

// All allocations and checks happen before the first live pointer is replaced.
inline bool install(const Inventories& stock) {
	struct FreeRows { void operator()(npc_item_list* rows) const { aFree(rows); } };
	struct Buffer { npc_data* nd; std::unique_ptr<npc_item_list, FreeRows> rows; uint16 count; };
	std::vector<Buffer> buffers;
	for (const auto& entry : stock) {
		auto nd = shop(entry.first);
		if (!nd || entry.second.size() > UINT16_MAX) return false;
		auto raw = entry.second.empty() ? nullptr : static_cast<npc_item_list*>(aCalloc(entry.second.size(), sizeof(npc_item_list)));
		std::unique_ptr<npc_item_list, FreeRows> rows(raw);
		for (size_t i = 0; i < entry.second.size(); ++i) { raw[i].nameid = entry.second[i].first; raw[i].value = entry.second[i].second; }
		buffers.push_back({nd, std::move(rows), static_cast<uint16>(entry.second.size())});
	}
	for (auto& buffer : buffers) {
		aFree(buffer.nd->u.shop.shop_item);
		buffer.nd->u.shop.shop_item = buffer.rows.release(); buffer.nd->u.shop.count = buffer.count;
	}
	for (const auto& entry : stock) black_market_catalog_cache_by_shop.erase(entry.first);
	return true;
}
inline int load() {
	search_ready = false; search_rows.clear(); clear_shop("Riven_SearchShop");
	std::map<std::string, Catalog> staged;
	for (const auto& p : profiles) {
		auto& cat = staged[p.key]; cat.db = std::make_shared<Database>(p);
		cat.ready = cat.db->load_catalog();
	}
	std::map<t_itemid, std::string> owners;
	for (size_t i = 0; i < 8; ++i) {
		auto& cat = staged[profiles[i].key];
		for (const auto& row : cat.db->ordered) {
			auto inserted = owners.emplace(row->id, profiles[i].key);
			if (!inserted.second) {
				cat.ready = false; staged[inserted.first->second].ready = false;
				ShowError("Black Market catalog %s: duplicate equipment Id %u also in %s.\n", profiles[i].path().c_str(), row->id, inserted.first->second.c_str());
			}
		}
	}
	int count = 0;
	for (const auto& p : profiles) {
		auto& cat = staged[p.key]; Inventories stock;
		if (cat.ready) cat.ready = prepare(p, cat, stock) && install(stock);
		if (!cat.ready) {
			for (const auto& entry : empty_shops(p)) { shop_owner[entry.first] = p.key; clear_shop(entry.first); }
			cat.db->clear(); cat.drawers.clear();
			ShowError("Black Market catalog %s: unavailable; entire inventory cleared.\n", p.path().c_str());
		} else ++count;
	}
	catalogs = std::move(staged);
	search_ready = shop("Riven_SearchShop") != nullptr;
	for (size_t i = 0; i < 8; ++i) search_ready = search_ready && ready(profiles[i].key);
	if (search_ready) {
		for (size_t i = 0; i < 8; ++i) for (const auto& row : catalogs[profiles[i].key].db->ordered) {
			if (!admitted(profiles[i], *row)) continue;
			if (profiles[i].kind == Kind::Hat && !(item_db.find(row->id)->equip & (EQP_HEAD_TOP | EQP_HEAD_MID | EQP_HEAD_LOW))) continue;
			search_rows.emplace_back(row, &profiles[i]);
		}
		std::sort(search_rows.begin(), search_rows.end(), [](const auto& a, const auto& b) {
			auto aname = lower(a.first->name), bname = lower(b.first->name);
			return aname == bname ? a.first->id < b.first->id : aname < bname;
		});
	}
	return count;
}
inline const std::vector<Drawer>* drawers(const std::string& key, int64 category) {
	if (!ready(key) || category < 0 || category > UINT32_MAX) return nullptr;
	auto& all = catalogs[key].drawers; auto it = all.find(static_cast<uint32>(category));
	return it == all.end() ? nullptr : &it->second;
}
inline bool managed_ready(const std::string& name) {
	if (name == "Riven_SearchShop") return search_ready;
	auto it = shop_owner.find(name);
	if (it != shop_owner.end()) return ready(it->second);
	// Guard managed shells even before the first loader OnInit executes.
	for (const auto& p : profiles) if (empty_shops(p).count(name)) return ready(p.key);
	return true;
}
inline std::shared_ptr<Record> card(t_itemid id, int* tier = nullptr) {
	for (const auto& p : profiles) if (p.kind == Kind::Card && ready(p.key)) {
		auto row = catalogs[p.key].db->find(id);
		if (row) { if (tier) *tier = p.tier; return row; }
	}
	return nullptr;
}
inline int search(std::string query, std::vector<t_itemid>& out) {
	if (!search_ready) return -1;
	auto begin = query.find_first_not_of(' '), end = query.find_last_not_of(' ');
	query = begin == std::string::npos ? "" : query.substr(begin, end - begin + 1);
	if (query.empty()) return -2;
	bool numeric = std::all_of(query.begin(), query.end(), [](char c) { return c >= '0' && c <= '9'; });
	if (numeric) {
		uint32 id = 0;
		for (char c : query) { uint32 digit = c - '0'; if (id > (INT32_MAX - digit) / 10) return 0; id = id * 10 + digit; }
		for (const auto& row : search_rows) if (row.first->id == id) out.push_back(id);
	} else {
		if (query.size() < 4) return -2;
		query = lower(query); std::vector<std::string> tokens; std::string token;
		std::istringstream input(query);
		// Only ASCII spaces split tokens, matching the NPC's explode(..., " ").
		while (std::getline(input, token, ' ')) if (!token.empty()) tokens.push_back(token);
		for (const auto& row : search_rows) {
			auto name = lower(row.first->name);
			if (std::all_of(tokens.begin(), tokens.end(), [&](const auto& part) { return name.find(part) != std::string::npos; })) out.push_back(row.first->id);
		}
	}
	return static_cast<int>(out.size());
}
inline bool search_prepare(const std::vector<t_itemid>& ids) {
	clear_shop("Riven_SearchShop");
	if (!search_ready || ids.empty()) return false;
	Inventory stock; std::set<t_itemid> seen;
	for (auto id : ids) {
		if (!seen.insert(id).second) return false;
		auto row = std::find_if(search_rows.begin(), search_rows.end(), [&](const auto& entry) { return entry.first->id == id; });
		if (row == search_rows.end()) return false;
		int64 rate = mapreg_readreg(add_str(row->second->rate));
		int32 price = 0;
		if (rate < -10 || rate > 30 || !black_market_adjusted_price(row->first->price, static_cast<int32>(rate), price)) return false;
		stock.emplace_back(id, price);
	}
	return install({{"Riven_SearchShop", std::move(stock)}});
}
} // namespace bmc
#endif
