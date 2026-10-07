"""Read-only Black Market catalog normalization and policy checks.

This module never writes NPCs, YAML, indexes, or client assets.
"""
from __future__ import annotations

import re
from pathlib import Path
from typing import Any

import yaml

SERVER = Path(__file__).resolve().parents[1]
CATALOG_DIR = SERVER / "npc/custom/black_market_catalog"
EQUIPMENT = ("harlan", "soren", "mordain", "kaedra", "tess", "weaver",
             "lady-seraphine", "madame-celestine")
CARDS = ("card-normal", "card-miniboss", "card-mvp", "card-special")
KEYS = EQUIPMENT + ("rooke",) + CARDS
TIERS = ("Normal1_50", "Normal51_99", "Rebirth1_50", "Rebirth51_99",
         "Level100_200", "Level201Plus")
CATEGORY_MAX = {"harlan": 0, "soren": 21, "mordain": 0, "kaedra": 0,
                "tess": 0, "weaver": 0, "lady-seraphine": 2,
                "madame-celestine": 89, "rooke": 9, **dict.fromkeys(CARDS, 0)}
PREFIXES = {"harlan": ("hat0", "hat1", "hat2"),
            "soren": tuple(f"wep{i}_0" for i in range(22)),
            "mordain": ("arm0",), "kaedra": ("shd0",), "tess": ("ftr0",),
            "weaver": ("gar0",), "lady-seraphine": ("acc0", "acl0", "acr0"),
            "madame-celestine": tuple(f"spc#S{i:02}" for i in range(90)),
            "card-normal": ("card_mob",), "card-miniboss": ("card_miniboss",),
            "card-mvp": ("card_mvp",), "card-special": ("card_special",)}
SELLER_KEYS = dict(zip(("hat_seller.txt", "weapon_seller.txt", "armor_seller.txt",
                       "shield_seller.txt", "footgear_seller.txt", "garment_seller.txt",
                       "accessory_seller.txt", "specialty_seller.txt"), EQUIPMENT))


class CatalogError(ValueError):
    pass


class UniqueKeyLoader(yaml.SafeLoader):
    """Reject duplicate mapping keys before normalization can hide them."""


def _mapping(loader: UniqueKeyLoader, node: yaml.MappingNode, deep: bool = False):
    result = {}
    for key_node, value_node in node.value:
        key = loader.construct_object(key_node, deep=deep)
        if key in result:
            raise CatalogError(f"duplicate field {key!r} at line {key_node.start_mark.line + 1}")
        result[key] = loader.construct_object(value_node, deep=deep)
    return result


UniqueKeyLoader.add_constructor(yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG, _mapping)


def _decimal(loader: UniqueKeyLoader, node: yaml.ScalarNode) -> int:
    if not re.fullmatch(r"0|[1-9][0-9]*", node.value):
        raise CatalogError(f"expected nonnegative decimal integer at line {node.start_mark.line + 1}")
    return int(node.value)


UniqueKeyLoader.add_constructor("tag:yaml.org,2002:int", _decimal)


def integer(row: dict, field: str, minimum: int, maximum: int = 2147483647) -> int:
    value = row.get(field)
    if type(value) is not int or not minimum <= value <= maximum:
        raise CatalogError(f"Id {row.get('Id', '?')}: {field} must be an integer in {minimum}..{maximum}")
    return value


def validate_document(key: str, document: Any, item_ids: set[int] | None = None) -> list[dict]:
    if key not in KEYS:
        raise CatalogError(f"unknown catalog {key}")
    if not isinstance(document, dict) or not isinstance(document.get("Header"), dict):
        raise CatalogError("expected BLACK_MARKET_CATALOG version 1 header")
    header = document["Header"]
    if set(header) != {"Type", "Version"} or header["Type"] != "BLACK_MARKET_CATALOG" or type(header["Version"]) is not int or header["Version"] != 1:
        raise CatalogError("expected BLACK_MARKET_CATALOG version 1 header")
    if set(document) != {"Header", "Body"}:
        raise CatalogError("only Header and Body are allowed; imports are unsupported")
    if not isinstance(document["Body"], list):
        raise CatalogError("Body must be a sequence")
    seen = set()
    rows = []
    for source in document["Body"]:
        if not isinstance(source, dict):
            raise CatalogError("record must be a mapping")
        row = dict(source)
        item_id = integer(row, "Id", 1)
        if item_id in seen:
            raise CatalogError(f"duplicate Id {item_id}")
        seen.add(item_id)
        if item_ids is not None and item_id not in item_ids:
            raise CatalogError(f"Id {item_id}: item DB record does not exist")
        required = {"Id", "TierPrices", "Category"} if key == "rooke" else {"Id", "Price", "DisplayName"}
        if key in CARDS:
            required |= {"CollectorOffer", "SourceMob"}
        elif key not in ("harlan", "rooke"):
            required.add("Category")
        if set(row) != required:
            raise CatalogError(f"Id {item_id}: missing or unexpected fields: {set(row) ^ required}")
        if key != "rooke":
            integer(row, "Price", 1)
            if type(row["DisplayName"]) is not str or not row["DisplayName"]:
                raise CatalogError(f"Id {item_id}: DisplayName must be a nonempty string")
        if "Category" in row:
            integer(row, "Category", 0, CATEGORY_MAX[key])
        if key in CARDS:
            integer(row, "CollectorOffer", 1)
            integer(row, "SourceMob", 0 if key == "card-special" else 1)
            if key == "card-special" and row["SourceMob"] != 0:
                raise CatalogError(f"Id {item_id}: Special SourceMob must be 0")
        if key == "rooke":
            tiers = row["TierPrices"]
            if not isinstance(tiers, dict) or set(tiers) != set(TIERS):
                raise CatalogError(f"Id {item_id}: TierPrices must contain exactly six tiers")
            for tier in TIERS:
                integer({"Id": item_id, tier: tiers[tier]}, tier, 0)
        rows.append(row)
    return rows


def read_catalog(key: str, directory: Path = CATALOG_DIR, item_ids: set[int] | None = None) -> list[dict]:
    path = directory / f"{key}.yml"
    try:
        return validate_document(key, yaml.load(path.read_text(encoding="utf-8-sig"), Loader=UniqueKeyLoader), item_ids)
    except (OSError, yaml.YAMLError, CatalogError) as error:
        raise CatalogError(f"{path}: {error}") from error


def read_all(directory: Path = CATALOG_DIR, item_ids: set[int] | None = None) -> dict[str, list[dict]]:
    catalogs = {key: read_catalog(key, directory, item_ids) for key in KEYS}
    owners = {}
    for key in EQUIPMENT:
        for row in catalogs[key]:
            item_id = row["Id"]
            if item_id in owners:
                raise CatalogError(f"duplicate equipment Id {item_id}: {owners[item_id]} and {key}")
            owners[item_id] = key
    return catalogs


def read_items(server: Path = SERVER) -> dict[int, dict]:
    """Read the Renewal import graph for admission and equipment-slot audits.

    Locations are per-flag overrides, matching ItemDatabase::parseBodyNode.
    This is an audit view, not a replacement for native item DB validation.
    """
    items = {}
    visiting = set()

    def visit(relative: str) -> None:
        path = (server / relative).resolve()
        if not path.is_relative_to(server.resolve()) or path in visiting:
            raise CatalogError(f"invalid or cyclic item DB import {path}")
        visiting.add(path)
        try:
            document = yaml.load(path.read_text(encoding="utf-8-sig"),
                                 Loader=getattr(yaml, "CSafeLoader", yaml.SafeLoader))
            if document.get("Header", {}).get("Type") != "ITEM_DB":
                raise CatalogError(f"{path}: expected ITEM_DB")
            if document["Header"].get("Clear"):
                items.clear()
            for source in document.get("Body", []) or []:
                item_id = source["Id"]
                row = items.setdefault(item_id, {"Type": "Etc", "SubType": "Normal",
                                                 "Name": "", "Locations": {}})
                for field in ("Name", "Type", "SubType", "Slots"):
                    if field in source:
                        row[field] = source[field]
                if "Locations" in source:
                    row["Locations"].update(source["Locations"])
            for entry in document.get("Footer", {}).get("Imports", []) or []:
                if entry.get("Mode", "Renewal") == "Renewal":
                    visit(entry["Path"])
        except (OSError, yaml.YAMLError, KeyError) as error:
            raise CatalogError(f"{path}: {error}") from error
        finally:
            visiting.remove(path)

    visit("db/item_db.yml")
    return items


def admitted(key: str, row: dict, items: dict[int, dict]) -> bool:
    item = items.get(row["Id"])
    if item is None:
        return False
    if key == "harlan":
        return (item["Type"].casefold() == "armor" and item["Name"] not in ("", "Unknown Item", "null")
                and not any(item["Locations"].get(location, False)
                            for location in ("Costume_Head_Top", "Costume_Head_Mid", "Costume_Head_Low")))
    if key in CARDS:
        return (item["Type"].casefold() == "card" and item["SubType"].casefold() == "normal"
                and letter(row["DisplayName"], key != "card-special") is not None)
    if key == "rooke":
        return item["Name"] not in ("", "Unknown Item") and item["Type"].casefold() in ("usable", "healing", "delayconsume", "etc", "cash")
    return True


def hat_slots(rows: list[dict], items: dict[int, dict]) -> list[list[dict]]:
    return [[row for row in rows if admitted("harlan", row, items)
             and items[row["Id"]]["Locations"].get(location, False)]
            for location in ("Head_Top", "Head_Mid", "Head_Low")]


def letter(name: str, strict: bool = False) -> str | None:
    first = name[:1].upper()
    if first and "A" <= first <= "Z":
        return first
    if first and "0" <= first <= "9":
        return "0"
    return None if strict else "A"


def drawer_key(name: str, count: int, strict: bool = False) -> str | None:
    initial = letter(name, strict)
    if initial is None or count == 0:
        return None
    if count < 100:
        return "ALL"
    if count < 200:
        return "AH" if "A" <= initial <= "H" else "IP" if "I" <= initial <= "P" else "QZ9"
    return initial


def drawers(rows: list[dict], *, cards: bool = False, specialty: bool = False,
            card_special: bool = False) -> dict[str, list[tuple[int, int]]]:
    strict = cards and not card_special
    accepted = [r for r in rows if letter(r["DisplayName"], strict) is not None]
    if not cards:
        accepted.sort(key=lambda r: (letter(r["DisplayName"]) == "0", r["DisplayName"].casefold(), r["Id"]))
    inventory = {}
    for row in accepted:
        suffix = "" if specialty else drawer_key(row["DisplayName"], len(accepted), strict)
        inventory.setdefault(suffix, []).append((row["Id"], row["Price"]))
    order = [""] if specialty else ["ALL", "AH", "IP", "QZ9"] + (["0"] if cards else []) + list("ABCDEFGHIJKLMNOPQRSTUVWXYZ") + ([] if cards else ["0"])
    return {suffix: inventory[suffix] for suffix in order if suffix in inventory}


def adjusted_price(price: int, rate: int) -> int:
    if not -10 <= rate <= 30 or price <= 0:
        raise CatalogError("invalid Daily Rate or price")
    result = (price * (100 + rate) + 50) // 100
    if not 0 < result <= 2147483647:
        raise CatalogError("Daily Rate price overflow")
    return result


def catalog_inventory(key: str, directory: Path = CATALOG_DIR,
                      items: dict[int, dict] | None = None) -> dict[str, list[tuple[int, int]]]:
    """Normalized base inventory, with the native loader's admission/grouping."""
    rows = read_catalog(key, directory)
    if key == "harlan" or key in CARDS or key == "rooke":
        items = read_items() if items is None else items
        rows = [row for row in rows if admitted(key, row, items)]
    if key == "rooke":
        stock = {f"cons{category}#T{tier}": [] for category in range(10) for tier in range(6)}
        stock.update({f"rookup#T{tier}": [] for tier in range(6)})
        for row in rows:
            for tier, field in enumerate(TIERS):
                price = row["TierPrices"][field]
                if price:
                    stock[f"cons{row['Category']}#T{tier}"].append((row["Id"], price))
                    if row["Category"] == 7:
                        stock[f"rookup#T{tier}"].append((row["Id"], courier_price(price)))
        return stock
    categories = hat_slots(rows, items) if key == "harlan" else [
        [row for row in rows if row.get("Category", 0) == category]
        for category in range(len(PREFIXES[key]))]
    stock = {}
    for prefix, subset in zip(PREFIXES[key], categories):
        for suffix, inventory in drawers(subset, cards=key in CARDS,
                                          specialty=key == "madame-celestine",
                                          card_special=key == "card-special").items():
            stock[prefix + ("#" + suffix if suffix else "")] = inventory
    return stock


def courier_price(price: int) -> int:
    result = (price * 120 + 50) // 100
    if not 0 < result <= 2147483647:
        raise CatalogError("courier price overflow")
    return result


def search(catalogs: dict[str, list[dict]], query: str) -> list[int]:
    query = query.strip(" ")
    if not query or (not re.fullmatch(r"[0-9]+", query) and len(query) < 4):
        raise CatalogError("query too short")
    rows = [row for key in EQUIPMENT for row in catalogs[key]]
    rows.sort(key=lambda row: (row["DisplayName"].casefold(), row["Id"]))
    if re.fullmatch(r"[0-9]+", query):
        item_id = int(query)
        return [row["Id"] for row in rows if row["Id"] == item_id] if 0 < item_id <= 2147483647 else []
    tokens = [token for token in query.lower().split(" ") if token]
    return [row["Id"] for row in rows if all(token in row["DisplayName"].lower() for token in tokens)]
