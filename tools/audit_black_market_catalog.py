"""Read-only comparison with the pre-migration Git baseline."""
from __future__ import annotations

import argparse
import ast
import json
import re
import subprocess
from pathlib import Path

from black_market_catalog import (CARDS, EQUIPMENT, KEYS, PREFIXES, SERVER, TIERS,
                                  CatalogError, admitted, courier_price, drawers,
                                  hat_slots, read_all, read_items, search)

BASELINE = "b0f2e389bf24249159292e48d21739f5669f99ee"
INDEX = Path(r"D:\A04 AI Knowledge Base\Ragnarok Offline\reference\black-market-equipment-search-index.json")
FILES = dict(zip(EQUIPMENT, ("hat_seller.txt", "weapon_seller.txt", "armor_seller.txt",
                            "shield_seller.txt", "footgear_seller.txt", "garment_seller.txt",
                            "accessory_seller.txt", "specialty_seller.txt")))
# Player-approved client-name reconciliation, 2026-10-05. Freeze old names so
# editing the derived index cannot redefine the Git baseline's Riven names.
CLIENT_NAME_AMENDMENTS = {
    450272: ("Celine's Bloody Black Dress-LT", "Bloody Celine Dress-LT"),
    560061: ("Dimensions Judgment Knuckle", "Dimensions Judgment Claw"),
    2663: ("Gauntlet of Hit", "Gauntlet of HIT"),
    2957: ("Modified Ring of Flame Lord", "Enhanced Ring of Flame Lord"),
    2958: ("Modified Ring of Resonance", "Enhanced Ring of Resonance"),
    28595: ("Ring of Azure Dragon", "Blue Dragon Ring"),
    420019: ("Young Leaf of World Tree(Agi)", "Young Leaf of World Tree(AGI)"),
    420018: ("Young Leaf of World Tree(Dex)", "Young Leaf of World Tree(DEX)"),
    420022: ("Young Leaf of World Tree(Int)", "Young Leaf of World Tree(INT)"),
    420020: ("Young Leaf of World Tree(Luk)", "Young Leaf of World Tree(LUK)"),
    420017: ("Young Leaf of World Tree(Str)", "Young Leaf of World Tree(STR)"),
    420021: ("Young Leaf of World Tree(Vit)", "Young Leaf of World Tree(VIT)"),
}


def approved_names(rows: list[dict]) -> list[dict]:
    result = []
    for row in rows:
        copy = dict(row)
        if row["Id"] in CLIENT_NAME_AMENDMENTS:
            old, current = CLIENT_NAME_AMENDMENTS[row["Id"]]
            if row["DisplayName"] != old:
                raise CatalogError(f"baseline name amendment input changed: {row['Id']}")
            copy["DisplayName"] = current
        result.append(copy)
    return result


def source(ref: str, filename: str) -> str:
    return subprocess.check_output(["git", "-c", f"safe.directory={SERVER.as_posix()}",
                                    "show", f"{ref}:npc/custom/{filename}"], cwd=SERVER).decode("utf-8-sig")


def array(text: str, name: str) -> list:
    entries = {}
    pattern = rf"setarray\s+\.{re.escape(name)}\[(\d+)\],\s*(.*?);"
    for match in re.finditer(pattern, text, re.S):
        offset = int(match[1])
        raw = re.sub(r"//[^\n]*", "", match[2])
        values = [ast.literal_eval(token) for token in re.findall(r'"(?:\\.|[^"\\])*"', raw)] if name.endswith("$") else [int(x) for x in raw.split(",") if x.strip()]
        for i, value in enumerate(values, offset):
            if i in entries:
                raise CatalogError(f"baseline {name}: duplicate array index {i}")
            entries[i] = value
    if sorted(entries) != list(range(len(entries))):
        raise CatalogError(f"baseline {name}: missing array index")
    return [entries[i] for i in range(len(entries))]


def legacy(ref: str = BASELINE) -> dict[str, list[dict]]:
    names = {r["item_id"]: r["shop_display_name"] for r in json.loads(INDEX.read_text(encoding="utf-8-sig"))["rows"]}
    for item_id, (old, current) in CLIENT_NAME_AMENDMENTS.items():
        if names.get(item_id) not in (old, current):
            raise CatalogError(f"unapproved index name for {item_id}")
        names[item_id] = old
    catalogs = {}
    for key, filename in FILES.items():
        text = source(ref, filename)
        rows = []
        if key == "harlan":
            active = text.split("OnInit:", 1)[1]
            count = int(re.search(r"\.IroCount\s*=\s*(\d+)", active)[1])
            ids, prices, display = (array(active, name) for name in ("IroId", "IroPrice", "IroClientDisp$"))
            if not len(ids) == len(prices) == len(display) == count == 532:
                raise CatalogError("baseline Harlan active range changed")
            for item_id, price, name in zip(ids, prices, display):
                if names[item_id] != name:
                    raise CatalogError(f"baseline Harlan name mismatch {item_id}")
                rows.append({"Id": item_id, "Price": price, "DisplayName": name})
        else:
            for match in re.finditer(r"^-\tpointshop\t([^\t]+)\t-1,BlackMarketPoints,([^\r\n]+)", text, re.M):
                shop, products = match.groups()
                if key == "madame-celestine":
                    category = PREFIXES[key].index(shop)
                else:
                    category = PREFIXES[key].index(shop.split("#")[0])
                for product in products.split(","):
                    item_id, price = map(int, product.split(":"))
                    rows.append({"Id": item_id, "Price": price, "DisplayName": names[item_id], "Category": category})
        catalogs[key] = rows
    text = source(ref, "card_seller.txt")
    blocks = re.split(r"^-\tscript\t(?:card_seller_creation|miniboss_card_seller_creation|mvp_card_seller_creation)\t", text, flags=re.M)[1:]
    for key, block in zip(CARDS[:3], blocks):
        ids, prices, offers, mobs, display = (array(block, name) for name in ("CardIds", "CardPrice", "CardOffer", "SourceMobIds", "CardDisp$"))
        if len({len(ids), len(prices), len(offers), len(mobs), len(display)}) != 1:
            raise CatalogError(f"baseline {key}: arrays misaligned")
        catalogs[key] = [dict(Id=i, Price=p, DisplayName=n, CollectorOffer=o, SourceMob=m) for i, p, o, m, n in zip(ids, prices, offers, mobs, display)]
    special = blocks[-1]
    ids, prices, offers, display = (array(special, name) for name in ("SpecialCardIds", "SpecialCardPrice", "SpecialCollectorOffer", "SpecialCardDisp$"))
    if len({len(ids), len(prices), len(offers), len(display)}) != 1:
        raise CatalogError("baseline Special: arrays misaligned")
    catalogs["card-special"] = [dict(Id=i, Price=p, DisplayName=n, CollectorOffer=o, SourceMob=0) for i, p, o, n in zip(ids, prices, offers, display)]
    text = source(ref, "consumable_seller.txt").split("OnInit:")[-1]
    ids, categories = array(text, "ItemId"), array(text, "ItemCat")
    tiers = [array(text, f"Tier{i}") for i in range(6)]
    if len({len(ids), len(categories), *(len(t) for t in tiers)}) != 1:
        raise CatalogError("baseline Rooke: arrays misaligned")
    catalogs["rooke"] = [dict(Id=i, Category=c, TierPrices=dict(zip(TIERS, values))) for i, c, *values in zip(ids, categories, *tiers)]
    riven = source(ref, "equipment_search.txt")
    riven_ids, riven_prices, riven_names = (array(riven, name) for name in ("ItemId", "BasePrice", "SearchName$"))
    riven_sellers, riven_rates = array(riven, "SellerName$"), array(riven, "RateKey")
    all_rows = {r["Id"]: r for key in EQUIPMENT for r in catalogs[key]}
    if len(all_rows) != 3444 or any(len(rows) != 3444 for rows in (riven_ids, riven_prices, riven_names, riven_sellers, riven_rates)):
        raise CatalogError("baseline Equipment count changed")
    owners = {r["Id"]: position for position, key in enumerate(EQUIPMENT) for r in catalogs[key]}
    sellers = ("Harlan", "Soren", "Mordain", "Kaedra", "Tess", "Weaver", "Lady Seraphine", "Madame Celestine")
    for i, p, n, seller, rate in zip(riven_ids, riven_prices, riven_names, riven_sellers, riven_rates):
        if (all_rows[i]["Price"], all_rows[i]["DisplayName"].lower()) != (p, n):
            raise CatalogError(f"baseline Riven mismatch {i}")
        if (seller, rate) != (sellers[owners[i]], owners[i]):
            raise CatalogError(f"baseline Riven seller/Daily Rate owner mismatch {i}")
    collector = source(ref, "card_collector.txt")
    for key, ids_name, mobs_name in (("card-mvp", "Offer10", "MvpSources"),
                                    ("card-miniboss", "MinibossCards", "MinibossSources")):
        if array(collector, ids_name) != [r["Id"] for r in catalogs[key]] or array(collector, mobs_name) != [r["SourceMob"] for r in catalogs[key]]:
            raise CatalogError(f"baseline Collector {key}: source alignment changed")
    return catalogs


def compare(expected: dict, actual: dict) -> None:
    expected = {key: approved_names(rows) if key in EQUIPMENT else rows
                for key, rows in expected.items()}
    for key in KEYS:
        old, new = expected[key], actual[key]
        if key in CARDS or key == "rooke":
            if old != new:
                raise CatalogError(f"{key}: records or source order changed")
        elif {r["Id"]: r for r in old} != {r["Id"]: r for r in new}:
            raise CatalogError(f"{key}: item/name/price/category changed")
        if key != "rooke" and key != "harlan":
            for category in range(len(PREFIXES[key])):
                kwargs = dict(cards=key in CARDS, specialty=key == "madame-celestine",
                              card_special=key == "card-special")
                if drawers([r for r in old if r.get("Category", 0) == category], **kwargs) != drawers([r for r in new if r.get("Category", 0) == category], **kwargs):
                    raise CatalogError(f"{key} category {category}: ordered drawers changed")
    for tier in TIERS:
        upgrade = [(r["Id"], courier_price(r["TierPrices"][tier])) for r in actual["rooke"] if r["Category"] == 7 and r["TierPrices"][tier] > 0]
        expected_upgrade = [(r["Id"], courier_price(r["TierPrices"][tier])) for r in expected["rooke"] if r["Category"] == 7 and r["TierPrices"][tier] > 0]
        if upgrade != expected_upgrade:
            raise CatalogError(f"courier {tier}: inventory changed")
    if search(expected, "boots") != search(actual, "boots"):
        raise CatalogError("Riven order changed")


def compare_declared_drawers(ref: str, actual: dict) -> None:
    """Compare real legacy declaration order, not a re-sorted legacy view."""
    for key in EQUIPMENT[1:]:
        text = source(ref, FILES[key])
        declared = {m[1]: [tuple(map(int, product.split(":"))) for product in m[2].split(",")]
                    for m in re.finditer(r"^-\tpointshop\t([^\t]+)\t-1,BlackMarketPoints,([^\r\n]+)", text, re.M)}
        planned = {}
        for category, prefix in enumerate(PREFIXES[key]):
            subset = [r for r in actual[key] if r["Category"] == category]
            for suffix, inventory in drawers(subset, specialty=key == "madame-celestine").items():
                planned[prefix + ("#" + suffix if suffix else "")] = inventory
        # Only five renamed records can move; case-only changes cannot reorder.
        exceptions = {"mordain": {450272}, "lady-seraphine": {28595, 2957, 2958},
                      "madame-celestine": {560061}}.get(key, set())
        unchanged_declared = {name: [pair for pair in rows if pair[0] not in exceptions]
                              for name, rows in declared.items()}
        unchanged_planned = {name: [pair for pair in rows if pair[0] not in exceptions]
                             for name, rows in planned.items()}
        if unchanged_declared != unchanged_planned:
            changed = sorted(name for name in declared.keys() | planned.keys() if declared.get(name) != planned.get(name))
            raise CatalogError(f"{key}: baseline drawer/order conflict: {changed}")
        if key == "mordain":
            rows = planned["arm0#AH"]
            if (450272, 12180) not in rows or rows.index((450272, 12180)) >= rows.index((450135, 2300)):
                raise CatalogError("client Bloody Celine ordering: item or price changed")
        if key == "lady-seraphine":
            if ((28595, 9360) not in planned["acr0#AH"] or
                    any(i == 28595 for i, _ in planned["acr0#QZ9"]) or
                    (2957, 1800) not in planned["acc0#E"] or (2958, 1440) not in planned["acc0#E"]):
                raise CatalogError("client Blue Dragon/Enhanced Ring routing: item or price changed")


def compare_admission(expected: dict, actual: dict, items: dict) -> None:
    old_slots, new_slots = hat_slots(expected["harlan"], items), hat_slots(actual["harlan"], items)
    if [len(rows) for rows in new_slots] != [361, 128, 87]:
        raise CatalogError("Harlan slot counts changed from 361/128/87")
    if any(drawers(old) != drawers(new) for old, new in zip(old_slots, new_slots)):
        raise CatalogError("Harlan ordered slot inventories changed")
    sale_cards = {}
    for key in CARDS:
        old = [r for r in expected[key] if admitted(key, r, items)]
        new = [r for r in actual[key] if admitted(key, r, items)]
        if drawers(old, cards=True, card_special=key == "card-special") != drawers(new, cards=True, card_special=key == "card-special"):
            raise CatalogError(f"{key}: admitted inventory changed")
        sale_cards[key] = new
    if [len(sale_cards[key]) for key in CARDS] != [782, 93, 156, 192]:
        raise CatalogError("card admission counts changed")
    normal = next(r for r in actual["card-normal"] if r["Id"] == 27350)
    if not normal["DisplayName"].startswith(" ") or admitted("card-normal", normal, items):
        raise CatalogError("27350 leading-space sale exclusion changed")
    if any(not admitted("rooke", row, items) for row in actual["rooke"]):
        raise CatalogError("Rooke admission changed")
    if len(actual["madame-celestine"]) != 1637 or len({r["Category"] for r in actual["madame-celestine"]}) != 90:
        raise CatalogError("Specialty count/family coverage changed")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline-ref", default=BASELINE)
    args = parser.parse_args()
    try:
        items = read_items()
        expected, actual = legacy(args.baseline_ref), read_all(item_ids=set(items))
        compare(expected, actual)
        compare_declared_drawers(args.baseline_ref, actual)
        compare_admission(expected, actual, items)
    except (CatalogError, KeyError, ValueError) as error:
        print(f"FAIL: {error}")
        return 1
    print("PASS: Equipment/Riven 3444, cards 1224 records/1223 sale IDs, Rooke 106; prices preserved; 12 client-name amendments and ordered inventories verified")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
