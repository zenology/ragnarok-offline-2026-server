"""Read-only item-catalog migration validation; no files or runtime state are written.

Python fixtures model schema/grouping, not execution of the production C++ loader.
Requires PyYAML. Player build, reload and purchase acceptance remain separate.
"""
from __future__ import annotations

import argparse
import ast
import copy
import hashlib
from functools import lru_cache
from collections import Counter
from pathlib import Path
import re
import subprocess
import sys

import yaml

SERVER = Path(__file__).resolve().parents[1]
KB = Path(r"D:\A04 AI Knowledge Base\Ragnarok Offline")
KEYS = ("dewey", "remy", "lemy", "milo")
CATEGORIES = {
    "dewey": ("top_ordinary", "top_special", "mid_ordinary", "mid_special", "low_ordinary",
              "low_special", "garment_ordinary", "garment_special", "garment_limited"),
    "remy": ("weapon", "armor", "shield", "shoes", "right_accessory", "left_accessory"),
    "lemy": ("materials",),
    "milo": ("top", "middle", "lower", "garment", "dual", "effect"),
}
MILO_LABELS = ("^4d4dffTop^000000 Stones", "^1C8008Middle^000000 Stones", "^ff0000Lower^000000 Stones",
               "^9905d8Garment^000000 Stones", "^8B4513Dual^000000 Stones", "^8B4513Effect^000000 Stones")
NPC_PATHS = {
    "dewey": "npc/custom/costume_catalog.txt",
    "remy": "npc/custom/shadow_gear_broker.txt",
    "milo": "npc/re/merchants/mysterious_cookie_shop.txt",
    "milo_helper": "npc/custom/milo_crumb_placement.txt",
}
RETIRED_STOCK = {
    "npc/re/merchants/barters/costume_catalog.yml",
    "npc/re/merchants/barters/shadow_gear_broker.yml",
    "npc/custom/barters/enchantment_materials.yml",
    "npc/custom/barters/milo_stone_sections.yml",
}
IMPORT_FILES = {"npc/re/merchants/barters.yml", "npc/custom/barters.yml"}
REMOVED_IMPORTS = RETIRED_STOCK | {"npc/re/merchants/barters/mysterious_cookie_shop.yml"}
ASCII_LOWER = str.maketrans("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz")
CATALOG_SHA256 = {
    "dewey": "92bb1204ce8e7535d64cc52cd1229b9d9d5b5c7a1c1dc04b88b207611fa51860",
    "remy": "739331edee2e40ce746c85f3b746953e7b2bbe5122ce99961ca119d7bef2846f",
    "lemy": "ecaf973081735c645a1b82e1731e0b176b3ade5ff57a94f13a732efd652d8ab2",
    "milo": "258b89acd9cdd2db8a36babf021fc4e763377eeea673fa65e929055b3be2b53b",
}


def check(condition, message):
    if not condition:
        raise ValueError(message)


def git(*args):
    return subprocess.check_output(["git", "-c", f"safe.directory={SERVER.as_posix()}",
                                    "-C", str(SERVER), *args])


@lru_cache(maxsize=None)
def baseline_bytes(ref, path):
    return git("show", f"{ref}:{path}")


def baseline_text(ref, path):
    return baseline_bytes(ref, path).decode("utf-8-sig")


class UniqueLoader(yaml.SafeLoader):
    pass


def unique_mapping(loader, node, deep=False):
    result = {}
    for key, value in node.value:
        name = loader.construct_object(key, deep=deep)
        check(isinstance(name, str) and name not in result, f"duplicate/nonstring field {name!r}")
        result[name] = loader.construct_object(value, deep=deep)
    return result


def decimal_integer(loader, node):
    check(re.fullmatch(r"0|[1-9][0-9]*", node.value), "expected unquoted decimal integer without leading zero")
    return int(node.value)


UniqueLoader.add_constructor(yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG, unique_mapping)
UniqueLoader.add_constructor("tag:yaml.org,2002:int", decimal_integer)


def load_yaml(text):
    for event in yaml.parse(text):
        check(not getattr(event, "tag", None) and not getattr(event, "anchor", None),
              "tags, anchors and aliases are unsupported")
    return yaml.load(text, Loader=UniqueLoader)


def effective_items(ref):
    items, files = {}, set()

    def visit(path):
        check(path not in files, f"duplicate/cyclic item import {path}")
        files.add(path)
        doc = yaml.safe_load(baseline_text(ref, path))
        for row in doc.get("Body", []):
            items.setdefault(row["Id"], {}).update(row)
        for entry in doc.get("Footer", {}).get("Imports", []) or []:
            if entry.get("Mode", "Renewal") == "Renewal":
                visit(entry["Path"])

    visit("db/re/item_db.yml")
    visit("db/import/item_db.yml")
    return items, files


def merge_indexed(old, new):
    """Native Barter overrides update fields and requirement indices, not whole rows."""
    result = copy.deepcopy(old)
    for field, value in new.items():
        if field == "RequiredItems":
            requirements = {row["Index"]: row for row in result.get(field, [])}
            for row in value:
                requirements[row["Index"]] = merge_indexed(requirements.get(row["Index"], {}), row)
            result[field] = [requirements[index] for index in sorted(requirements)]
        else:
            result[field] = copy.deepcopy(value)
    return result


def barter_graph(ref=None):
    shops, files = {}, set()

    def visit(path):
        check(path not in files, f"duplicate/cyclic Barter import {path}")
        files.add(path)
        raw = baseline_text(ref, path) if ref else (SERVER / path).read_text(encoding="utf-8-sig")
        doc = yaml.safe_load(raw)
        for row in doc.get("Body", []) or []:
            shop = shops.setdefault(row["Name"], {"metadata": {}, "items": {}})
            shop["metadata"].update({k: v for k, v in row.items() if k not in ("Name", "Items")})
            for item in row.get("Items", []):
                shop["items"][item["Index"]] = merge_indexed(shop["items"].get(item["Index"], {}), item)
        for entry in doc.get("Footer", {}).get("Imports", []) or []:
            if entry.get("Mode", "Renewal") == "Renewal":
                visit(entry["Path"])

    visit("npc/barters.yml")
    return shops, files


def baseline_routes(ref):
    sources = {key: baseline_text(ref, path) for key, path in NPC_PATHS.items()}
    routes = {key: {} for key in KEYS}
    match = re.search(r"setarray \.GroupNames\$\[0\], ([^;]+);", sources["dewey"])
    check(match, "baseline Dewey GroupNames not found")
    names = ast.literal_eval("[" + match[1] + "]")
    for name in names:
        slot, kind, _ = map(int, re.fullmatch(r"costume_catalog_(\d)_(\d)_(\d+)", name).groups())
        routes["dewey"][name] = CATEGORIES["dewey"][slot * 2 + kind]
    routes["dewey"]["costume_catalog_limited"] = "garment_limited"
    remy = sources["remy"].split("mal_in01,62,116", 1)[0]
    blocks = re.findall(r'switch \(\.@letter\) \{(.*?)\n\s*\}', remy, re.S)
    check(len(blocks) == 6, "baseline Remy category switches changed")
    for category, block in zip(CATEGORIES["remy"], blocks):
        for name in re.findall(r'case \d+: \.@shop\$ = "([^"]+)";', block):
            check(name not in routes["remy"], f"duplicate baseline Remy route {name}")
            routes["remy"][name] = category
    lemy = sources["remy"].split("mal_in01,62,116", 1)[1]
    routes["lemy"][re.search(r'callshop\("([^"]+)"\)', lemy)[1]] = "materials"
    names = re.findall(r'callshop\( "([^"]+)" \)', sources["milo"])
    names += re.findall(r'callshop "([^"]+)";', sources["milo_helper"])
    check(len(names) == 6, "baseline Milo shop count changed")
    routes["milo"] = dict(zip(names, CATEGORIES["milo"]))
    check([len(routes[key]) for key in KEYS] == [350, 131, 1, 6], "baseline route counts changed")
    return routes, sources


def baseline_inventory(ref, items):
    shops, files = barter_graph(ref)
    routes, sources = baseline_routes(ref)
    aegis = {row["AegisName"]: ident for ident, row in items.items() if "AegisName" in row}
    result = {key: [] for key in KEYS}
    for key in KEYS:
        for name, category in routes[key].items():
            shop = shops[name]
            check(not shop["metadata"], f"unexpected mapped shop metadata {name}")
            for _, item in sorted(shop["items"].items()):
                requirements = item.get("RequiredItems", [])
                check(len(requirements) == 1 and requirements[0]["Index"] == 0, f"unexpected requirements {name}")
                req = requirements[0]
                check(item.get("Stock", 0) == item.get("Zeny", 0) == item.get("Refine", 0) == 0 and
                      req.get("Refine", -1) == -1, f"nondefault stock/zeny/refine {name}")
                result[key].append((category, aegis[item["Item"]], aegis[req["Item"]], req.get("Amount", 1)))
    return result, routes, shops, files, sources


def sort_name(name, costume=False):
    value = name.strip(" \t\r\n")
    while value.startswith("[") and "]" in value:
        value = value.split("]", 1)[1].strip(" \t\r\n")
    if costume and value[:7].translate(ASCII_LOWER) == "costume":
        value = value[7:].strip(" \t\r\n")
        if value.startswith(":"):
            value = value[1:].strip(" \t\r\n")
    return value


def letter(name, position=0):
    char = name[position:position + 1].translate(ASCII_LOWER)
    return ord(char) - ord("a") if "a" <= char <= "z" else 26 if char and char in "0123456789" else 27


def validate(document, key, item_ids):
    check(isinstance(document, dict) and set(document) == {"Header", "Body"}, "invalid envelope")
    header = document["Header"]
    check(isinstance(header, dict) and set(header) == {"Type", "Version"} and
          header["Type"] == "MALANGDO_SHOP_CATALOG" and type(header["Version"]) is int and
          header["Version"] == 1, "invalid Header")
    rows = document["Body"]
    check(isinstance(rows, list) and rows and 7000 in item_ids, "empty Body or missing currency")
    ids = set()
    for row in rows:
        check(isinstance(row, dict) and set(row) == {"Id", "DisplayName", "Categories", "Price"}, "invalid record fields")
        ident = row["Id"]
        check(type(ident) is int and 0 < ident <= 0xffffffff and ident in item_ids and ident not in ids,
              f"invalid/missing/duplicate Id {ident}")
        ids.add(ident)
        check(type(row["Price"]) is int and 0 < row["Price"] <= 30000, f"invalid Price for {ident}")
        name = row["DisplayName"]
        check(isinstance(name, str) and name.strip() and not any(c in name for c in "\0\r\n") and
              sort_name(name, key == "dewey"), f"invalid DisplayName for {ident}")
        categories = row["Categories"]
        check(isinstance(categories, list) and categories and (key == "dewey" or len(categories) == 1) and
              all(isinstance(c, str) and c in CATEGORIES[key] for c in categories) and
              len(set(categories)) == len(categories), f"invalid Categories for {ident}")
    if key in ("milo", "lemy"):
        check({c for row in rows for c in row["Categories"]} == set(CATEGORIES[key]), "required category has no items")
    return rows


def generated_tree(rows, key):
    """Static specification model; nodes are (label, cancel, auto, children, stock)."""
    nodes = {"root": ["", key != "milo", False, [], []]}

    def add(ident, parent, label, cancel=False, auto=False, stock=None):
        check(ident not in nodes, f"duplicate generated node {ident}")
        nodes[ident] = [label, cancel, auto, [], stock or []]
        nodes[parent][3].append(ident)

    categories = {c: [row for row in rows if c in row["Categories"]] for c in CATEGORIES[key]}

    def letters(parent, stock):
        stock = sorted(stock, key=lambda r: (sort_name(r["DisplayName"], key == "dewey").translate(ASCII_LOWER).encode("utf-8"), r["Id"]))
        buckets = {}
        for row in stock:
            buckets.setdefault(letter(sort_name(row["DisplayName"], key == "dewey")), []).append(row)
        for initial, members in sorted(buckets.items()):
            label = chr(65 + initial) if initial < 26 else "0-9" if initial == 26 else "Other"
            ident = f"{parent}_l{initial}"
            if key == "remy":
                add(ident, parent, label, stock=members)
                continue
            add(ident, parent, label, auto=True)
            groups = {}
            for row in members:
                second = letter(sort_name(row["DisplayName"], True), 1) if len(members) > 40 and initial < 26 else -1
                groups.setdefault(27 if second >= 26 else second, []).append(row)
            for second, values in sorted(groups.items()):
                group_label = label if second < 0 else label + "-Other" if second == 27 else label + chr(65 + second)
                add(f"{ident}_g{second + 1}", ident, group_label, stock=values)

    if key == "dewey":
        for index, (ident, label) in enumerate(zip(("top", "mid", "low", "garment"), ("Top", "Mid", "Low / Mouth", "Garment"))):
            relevant = CATEGORIES[key][index * 2:index * 2 + 2] + (("garment_limited",) if index == 3 else ())
            if not any(categories[c] for c in relevant):
                continue
            add(ident, "root", label, cancel=True)
            for kind, category in enumerate(relevant[:2]):
                if categories[category]:
                    node = f"c{index * 2 + kind}"
                    add(node, ident, ("Ordinary Costumes", "Special Costumes")[kind])
                    letters(node, categories[category])
            if index == 3 and categories["garment_limited"]:
                add("limited", ident, "Limited Costume", stock=categories["garment_limited"])
    elif key == "remy":
        if categories["weapon"]:
            add("weapon", "root", "Weapon", cancel=True)
            letters("weapon", categories["weapon"])
        if any(categories[c] for c in CATEGORIES[key][1:]):
            add("armor", "root", "Armor", cancel=True)
            for index, (category, label) in enumerate(zip(CATEGORIES[key][1:], ("Armor", "Shield", "Shoes", "Right Accessory", "Left Accessory")), 1):
                if categories[category]:
                    add(f"c{index}", "armor", label, cancel=True)
                    letters(f"c{index}", categories[category])
    else:
        for category, label in zip(CATEGORIES[key], MILO_LABELS if key == "milo" else ("Spellbooks and Essence",)):
            add(category, "root", label, stock=categories[category])
    for ident, (_, cancel, _, children, stock) in nodes.items():
        check(bool(children) != bool(stock), f"empty or mixed generated node {ident}")
        if children:
            menu = ":".join(nodes[c][0] for c in children) + (":Cancel" if cancel else "")
            check(len(menu.encode("utf-8")) < 2047, "native select capacity exceeded")
        # Current PACKETVER 20250716 normal-Barter row is 31 packed bytes; header is 4.
        check(4 + 31 * len(stock) <= 32767, "native Barter packet capacity exceeded")
    return nodes


def fixture_checks(documents, item_ids):
    def reject(doc, key="milo", raw=False):
        try:
            validate(load_yaml(doc) if raw else doc, key, item_ids)
        except (ValueError, yaml.YAMLError):
            return
        raise ValueError("invalid fixture was accepted")

    reject("Header: [", raw=True)
    for text in ('Header: {}\nHeader: {}\nBody: []', 'Header: &h {}\nBody: [*h]', 'Header: !!map {}\nBody: []'):
        reject(text, raw=True)
    doc = copy.deepcopy(documents["milo"])
    doc["Body"] = []
    reject(doc)
    for price in (0, -1, 1.5, True, "35", 30001, 10 ** 30):
        doc = copy.deepcopy(documents["milo"])
        doc["Body"][0]["Price"] = price
        reject(doc)
    for field, value in (("Id", 0), ("Id", 0xffffffff), ("Categories", []),
                         ("Categories", ["top", "top"]), ("Categories", ["missing"]),
                         ("DisplayName", ""), ("DisplayName", "a\nb"), ("DisplayName", "a\0b")):
        doc = copy.deepcopy(documents["milo"])
        doc["Body"][0][field] = value
        reject(doc)
    doc = copy.deepcopy(documents["milo"])
    doc["Body"].append(copy.deepcopy(doc["Body"][0]))
    reject(doc)
    doc = copy.deepcopy(documents["milo"])
    doc["Body"][0]["Shop"] = "forbidden"
    reject(doc)
    doc = copy.deepcopy(documents["milo"])
    doc["Header"]["Version"] = 2
    reject(doc)
    doc = copy.deepcopy(documents["milo"])
    doc["Footer"] = {"Imports": []}
    reject(doc)
    doc = copy.deepcopy(documents["milo"])
    doc["Body"] = [row for row in doc["Body"] if "dual" not in row["Categories"]]
    reject(doc)
    for raw in ('01', '1_000', '0x23', '1e2', '"35"'):
        text = 'Header: {Type: MALANGDO_SHOP_CATALOG, Version: 1}\nBody: [{Id: 1000528, DisplayName: "Test", Categories: [materials], Price: ' + raw + '}]'
        reject(text, "lemy", raw=True)
    stock = [{"Id": i + 1, "DisplayName": f"Ab{i:03}", "Categories": ["top_ordinary"], "Price": 1} for i in range(41)]
    forty = generated_tree(stock[:40], "dewey")
    forty_one = generated_tree(stock, "dewey")
    check(forty["c0_l0_g0"][0] == "A" and forty_one["c0_l0_g2"][0] == "AB", "40/41 boundary")
    extra = copy.deepcopy(stock[0])
    extra.update(Id=10000, DisplayName="1 test")
    other = dict(extra, Id=10001, DisplayName="! test")
    same = dict(stock[0], Id=10002)
    tree = generated_tree([same, other, extra, stock[0]], "dewey")
    check(tree["c0"][3] == ["c0_l0", "c0_l26", "c0_l27"], "A-Z/digits/Other order")
    check([r["Id"] for r in tree["c0_l0_g0"][4]] == [1, 10002], "equal-name ID tie break")
    changed = copy.deepcopy(stock[:1])
    changed[0]["Price"] = 7
    check(generated_tree(changed, "dewey")["c0_l0_g0"][4][0]["Price"] == 7, "price edit did not reach stock")
    changed[0]["Categories"] = ["mid_special"]
    tree = generated_tree(changed, "dewey")
    check(tree["root"][3] == ["mid"] and "top" not in tree, "category move left stale menu")
    # New item creates a populated bucket; deleting it removes that bucket.
    check("c0_l1" in generated_tree(stock[:1] + [dict(extra, DisplayName="Beta")], "dewey") and
          "c0_l1" not in generated_tree(stock[:1], "dewey"), "add/delete left stale drawer")
    check(sort_name(" [Event] Costume: Apple ", True) == "Apple", "costume normalization")
    check(letter("") == 27 and letter("É") == 27, "ASCII grouping")
    # File order cannot change the fixed native case order.
    reordered = copy.deepcopy(documents["milo"])
    reordered["Body"].reverse()
    tree = generated_tree(validate(reordered, "milo", item_ids), "milo")
    check(tree["root"][3] == list(CATEGORIES["milo"]) and
          [tree[c][0] for c in tree["root"][3]] == list(MILO_LABELS), "Milo native case order shifted")


def search_model(rows, key, query, available=True):
    """Static data model only; this does not call the production C++ implementation."""
    if not available:
        return -1, []
    query = query.strip(" ")
    if not query:
        return -2, []
    if all("0" <= c <= "9" for c in query):
        ident = 0
        for char in query:
            digit = ord(char) - ord("0")
            if ident > (0x7fffffff - digit) // 10:
                return 0, []
            ident = ident * 10 + digit
        matches = [row for row in rows if row["Id"] == ident]
    else:
        if len(query.encode("utf-8")) < 4:
            return -2, []
        tokens = [part for part in query.translate(ASCII_LOWER).split(" ") if part]
        matches = [row for row in rows if all(part in row["DisplayName"].translate(ASCII_LOWER) for part in tokens)]
    matches.sort(key=lambda row: (sort_name(row["DisplayName"], key == "dewey").translate(ASCII_LOWER).encode("utf-8"), row["Id"]))
    return len(matches), matches


def search_page_model(rows, key, query, page, revision=1, expected=1, busy=False, available=True):
    count, matches = search_model(rows, key, query, available)
    if not available or revision <= 0 or revision != expected or busy or page < 0 or count <= 0 or page > (count - 1) // 50:
        return None
    selected = matches[page * 50:(page + 1) * 50]
    # index, item, currency, amount, limited-stock, stock, Zeny, refine, required-refine
    stock = [(i, row["Id"], 7000, row["Price"], False, 0, 0, 0, -1) for i, row in enumerate(selected)]
    limited = key == "dewey" and any("garment_limited" in row["Categories"] for row in selected)
    return stock, limited


def search_fixtures(documents):
    for key in KEYS:
        rows = documents[key]["Body"]
        ident = rows[0]["Id"]
        check(search_model(rows, key, f"  {ident}  ")[1] == [rows[0]], "exact ID/trim search")
        for query, expected in (("", -2), ("  ", -2), ("abc", -2), ("0", 0), ("2147483648", 0), ("9" * 100, 0)):
            check(search_model(rows, key, query)[0] == expected, "query validation/overflow")
        check(search_model(rows, key, str(ident), False)[0] == -1, "unavailable search")
        for row in rows:
            check(search_page_model(rows, key, str(row["Id"]), 0)[0] ==
                  [(0, row["Id"], 7000, row["Price"], False, 0, 0, 0, -1)], "search/native price/default parity")
        other_id = next(row["Id"] for other in KEYS if other != key for row in documents[other]["Body"]
                        if row["Id"] not in {r["Id"] for r in rows})
        check(search_model(rows, key, str(other_id))[0] == 0, "cross-NPC result leakage")
    base = {"Id": 900000, "DisplayName": "[Tag] Costume: Amber Wing", "Categories": ["top_ordinary", "mid_ordinary"], "Price": 7}
    same = dict(base, Id=900001)
    check(search_model([same, base], "dewey", "WING  aMbEr")[1] == [base, same], "token AND/case/order/dedup")
    check(search_model([base], "dewey", "Amber absent")[0] == 0, "all tokens required")
    check(search_model([base], "dewey", "900")[0] == 0, "numeric search must not become substring search")
    for total in (0, 1, 49, 50, 51, 100, 101, 1001):
        rows = [dict(base, Id=900000 + i, DisplayName=f"Amber Wing {i:04}") for i in range(total)]
        expected = search_model(rows, "dewey", "amber")[1]
        combined = []
        for page in range((total + 49) // 50):
            stock, limited = search_page_model(rows, "dewey", "amber", page)
            check(1 <= len(stock) <= 50 and 4 + len(stock) * 31 <= 32767 and not limited, "search page capacity")
            combined.extend(entry[1] for entry in stock)
        check(combined == [row["Id"] for row in expected] and len(set(combined)) == total, "page coverage/duplicates")
        check(search_page_model(rows, "dewey", "amber", (total + 49) // 50) is None, "past-last page accepted")
        # Every page is reachable through ten-page windows; labels stay below select's byte limit.
        pages = (total + 49) // 50
        reachable = []
        for first in range(0, pages, 10):
            window = list(range(first, min(first + 10, pages)))
            labels = [f"Items {p * 50 + 1}-{min(total, (p + 1) * 50)}" for p in window]
            labels += (["Previous pages"] if first else []) + (["Next pages"] if first + 10 < pages else [])
            labels += ["Look for something else", "Never mind"]
            check(len(":".join(labels).encode()) < 2047, "page selector capacity")
            reachable.extend(window)
        check(reachable == list(range(pages)), "ten-page window navigation")
    for kwargs in ({"page": -1}, {"page": 1}, {"page": 0, "expected": 2}, {"page": 0, "busy": True},
                   {"page": 0, "available": False}, {"page": 0, "revision": 0}):
        check(search_page_model([base], "dewey", "amber", **kwargs) is None, "unsafe prepare accepted")
    limited = dict(base, Id=42, Categories=["garment_limited"])
    check(search_page_model([limited], "dewey", "42", 0)[1], "Limited must follow categories, not hardcoded IDs")
    changed = dict(base, Price=99, Categories=["garment_limited"])
    check(search_page_model([changed], "dewey", "amber", 0)[0][0][3] == 99 and
          search_page_model([changed], "dewey", "amber", 0)[1], "price/category edits not reflected")
    check(search_model([], "dewey", "amber")[0] == 0 and
          search_model([base, same], "dewey", "amber")[0] == 2, "add/delete search maintenance")
    # Failed preparation returns no shop/payload and cannot overwrite an existing open result set.
    old = search_page_model([base], "dewey", "amber", 0)
    rejected = search_page_model([changed], "dewey", "amber", 0, busy=True)
    check(rejected is None and old[0][0][3] == 7, "busy/failed preparation changed previous stock")


def source_checks(ref, sources):
    current = {key: (SERVER / path).read_text(encoding="utf-8-sig") for key, path in NPC_PATHS.items()}
    for key, old in sources.items():
        new = current[key]
        original_new = re.sub(r'function\tscript\tF_MiloShopEntry\t\{.*?\n\}\n\n', '', new, flags=re.S)
        check(re.findall(r'^.*\bscript\b.*$', old, re.M) == re.findall(r'^.*\bscript\b.*$', original_new, re.M), f"NPC declarations changed: {key}")
        check([s.strip() for s in re.findall(r'^\s*mes .*$', old, re.M)] ==
              [s.strip() for s in re.findall(r'^\s*mes .*$', new, re.M)], f"dialogue changed: {key}")
        check([s.strip() for s in re.findall(r'^\s*next;', old, re.M)] ==
              [s.strip() for s in re.findall(r'^\s*next;', new, re.M)], f"dialogue paging changed: {key}")
    check("setarray" not in current["dewey"] and "S_OpenLetters" not in current["dewey"] and
          "switch" not in current["remy"] and "select(" not in current["remy"], "legacy catalog routing remains")
    check('if (msc_shoplimited("dewey", .@shop$))' in current["dewey"], "Limited warning guard missing")
    expected = re.sub(r'switch\( select\( .*? \) \)',
                      'if (!msc_ready("milo")) close;\n\tif (callfunc("F_MiloShopEntry")) close;\n\tswitch( select( msc_menu("milo", "root") ) )', sources["milo"])
    for old, ident in zip(("barter_stone_top", "barter_stone_mid", "barter_stone_low", "barter_stone_garment"), CATEGORIES["milo"]):
        expected = expected.replace(f'callshop( "{old}" )', f'callshop( msc_shop("milo", "{ident}") )')
    check(current["milo"] == expected, "Milo changed outside approved native anchors")
    helper = sources["milo_helper"].replace('callshop "milo_stone_dual";', 'callshop msc_shop("milo", "dual");')
    helper = helper.replace('callshop "milo_stone_effect";', 'callshop msc_shop("milo", "effect");')
    stripped_helper = re.sub(r'function\tscript\tF_MiloShopEntry\t\{.*?\n\}\n\n', '', current["milo_helper"], flags=re.S)
    check(stripped_helper == helper, "Milo helper changed outside search entry/shop arguments")
    definitions = (SERVER / "src/custom/script_def.inc").read_text(encoding="utf-8")
    bodies = (SERVER / "src/custom/script.inc").read_text(encoding="utf-8")
    for name, signature, args in (("ready", "s", [2]), ("menu", "ss", [2, 3]), ("child", "ssi", [2, 3, 4]),
                                  ("shop", "ss", [2, 3]), ("autosingle", "ss", [2, 3]),
                                  ("search", "ss", [2, 3]), ("revision", "s", [2]),
                                  ("searchprepare", "ssii", [2, 3, 4, 5]), ("shoplimited", "ss", [2, 3])):
        check(definitions.count(f'BUILDIN_DEF(msc_{name},"{signature}")') == 1, f"builtin signature {name}")
        body = re.search(r'BUILDIN_FUNC\(msc_' + name + r'\) \{(.*?)\n\}', bodies, re.S)
        check(body and [int(i) for i in re.findall(r'script_get(?:str|num64)\(st, (\d+)\)', body[1])] == args,
              f"builtin argument access {name}")
    check("msc_load" not in bodies + definitions, "obsolete loader binding remains")
    check(all(re.fullmatch(r'[isrlv]*(?:\?)*|[isrlv]*\*', signature)
              for signature in re.findall(r'BUILDIN_DEF\(msc_\w+,"([^"]*)"\)', definitions)), "invalid builtin descriptor grammar")
    npc = baseline_text(ref, "src/map/npc.cpp")
    npc = npc.replace('#include "script.hpp" // script_config', '#include "script.hpp" // script_config\n#include "../custom/malangdo_shop_catalog.hpp"')
    npc = npc.replace('void BarterDatabase::loadingFinished(){', 'void BarterDatabase::loadingFinished(){\n\tmsc::LoadGuard catalog_load(*this);')
    check((SERVER / "src/map/npc.cpp").read_text(encoding="utf-8") == npc, "native Barter changed outside include/guard anchors")
    header = (SERVER / "src/custom/malangdo_shop_catalog.hpp").read_text(encoding="utf-8")
    # The Python YAML model cannot catch a wrong rapidyaml node predicate.
    # Pin this source-level contract to the bundled library: mapping scalars
    # are KEYVAL, whereas Categories sequence elements are unkeyed VAL.
    node_types = (SERVER / "3rdparty/rapidyaml/src/c4/yml/tree.hpp").read_text(encoding="utf-8")
    check("bool is_val() const { return (type & (KEYVAL)) == VAL; }" in node_types and
          "bool is_keyval() const { return (type & KEYVAL) == KEYVAL; }" in node_types,
          "review scalar regression contract for changed rapidyaml predicates")
    scalar_body = re.search(r'inline std::string scalar\(.*?\n\}', header, re.S)
    check(scalar_body and "value.is_keyval() && value.has_val()" in scalar_body[0],
          "rapidyaml mapping scalars require KEYVAL; is_val rejects Header and item fields")
    check("category.has_val() && category.is_val()" in header,
          "Categories sequence scalar guard changed")
    check(all(label in header for label in MILO_LABELS) and "black_market" not in header, "Milo labels or Black Market separation")
    check('add_node(catalog, "weapon", "root", "Weapon", true)' in header and
          'add_node(catalog, id, "armor", labels[category], true)' in header, "Remy existing letter Cancel lost")
    check('require(static_cast<bool>(input), "cannot open file")' in header and
          "catalogs.clear();" in header and "std::weak_ptr<s_npc_barter>" in header,
          "missing-file/reload/weak-ownership guards absent")
    check("#define PACKETVER 20250716" in (SERVER / "src/custom/defines_pre.hpp").read_text(encoding="utf-8") and
          "#define MAX_AMOUNT 30000" in (SERVER / "src/common/mmo.hpp").read_text(encoding="utf-8"),
          "update validator capacity assumptions for changed native limits")
    conf = (SERVER / "npc/scripts_custom.conf").read_text(encoding="utf-8")
    registration = "npc: npc/custom/malangdo_shop_catalog_db.txt"
    check(conf.count(registration) == 1 and conf.index(registration) < conf.index("npc: npc/custom/milo_crumb_placement.txt"), "browse function registration order")
    browse = (SERVER / "npc/custom/malangdo_shop_catalog_db.txt").read_text(encoding="utf-8")
    check("OnInit:" not in browse and "callshop" not in browse and "mes " not in browse and
          "F_MalangdoShopBrowse" in browse, "browse function has extra responsibilities")
    search = (SERVER / "npc/custom/malangdo_shop_search.txt").read_text(encoding="utf-8")
    search_registration = "npc: npc/custom/malangdo_shop_search.txt"
    check(conf.count(search_registration) == 1 and conf.index(registration) < conf.index(search_registration) <
          conf.index("npc: npc/custom/milo_crumb_placement.txt"), "search function registration order")
    check("callshop" not in search and "OnInit:" not in search and "switch (.@key$)" not in search,
          "search must not open shops/load data/use numeric switch for strings")
    check('min(10, .@pages - .@first_page)' in search and 'getstrlen(.@menu$) >= 2047' in search and
          '.@pages = (.@count + 49) / 50;' in search, "bounded page selection missing")
    check(search.index('.@input_revision = msc_revision') < search.index('input .@query$;') <
          search.index('.@revision = msc_revision') < search.index('.@count = msc_search'), "input generation checks missing")
    prepared = search.split('.@shop$ = msc_searchprepare', 1)[1]
    check('return .@shop$;' in prepared and
          not re.search(r'\b(next|select|input|sleep)\b', prepared), "search success path yields")
    success = prepared.split('if (.@shop$ != "") {', 1)[1].split('return .@shop$;', 1)[0]
    check('if (!msc_shoplimited(.@key$, .@shop$))' in success and 'mes .@speaker$;' in success and
          'mes "I found " + .@count + " item(s). Here are items "' in success and
          '(.@page * 50 + 1)' in success and 'min(.@count, (.@page + 1) * 50)' in success and
          'clear;' not in success, "successful search must leave a result message or Dewey Limited warning")
    for key in ("dewey", "remy"):
        check('F_MalangdoShopChoose' in current[key], "seller missing browse/search entry")
        tail = current[key].split('F_MalangdoShopChoose', 1)[1].split('callshop', 1)[0]
        check(not re.search(r'\b(next|select|input|sleep)\b', tail), "seller yields before opening prepared results")
    entry = re.search(r'function\tscript\tF_MiloShopEntry\t\{(.*?)\n\}', current["milo_helper"], re.S)[1]
    tail = entry.split('F_MalangdoShopSearch', 1)[1].split('callshop', 1)[0]
    check(not re.search(r'\b(next|select|input|sleep)\b', tail) and '\tend;' in entry, "Milo search yields or returns to native switch")
    check('inline constexpr size_t search_page_size = 50;' in header and 'std::vector<Record> records;' in header and
          'std::map<t_itemid, size_t> by_id;' in header and '++generation;' in header, "search metadata/generation missing")
    check('std::string("msc_search_") + keys[profile]' in header and 'make_barter(replacement.search_shop, {})' in header and
          '(reserved || !barter->items.empty())' in header and '!npc->u.barter.extended' in header,
          "reserved empty native normal-Barter lifecycle missing")
    prepare = header.split('inline std::string search_prepare', 1)[1].split('inline bool shop_limited', 1)[0]
    check(prepare.index('search_busy(barter->npcid)') < prepare.index('make_barter(catalog.search_shop, selected)') <
          prepare.index('barter->items.swap(replacement->items)') and 'revision(key) != expected_generation' in prepare,
          "search inventory must swap only after generation/busy/temporary validation")
    check('mapit_getallusers()' in header and 'mapit_free(iterator);' in header and
          'sd->npc_shopid == npcid' in header and 'sd->state.barter_open || sd->state.barter_extended_open' in header,
          "active-window guard missing")
    for path in ("src/map/clif.cpp", "src/map/pc.cpp", "src/map/npc.hpp", "src/map/script.cpp"):
        check((SERVER / path).read_text(encoding="utf-8-sig") == baseline_text(ref, path).replace("\r\n", "\n"),
              f"native purchase/packet/script core changed: {path}")
    for path in ["src/custom/malangdo_shop_menu.hpp", "npc/custom/malangdo_shop_menu_db.txt",
                 "tools/validate_malangdo_shop_menu.py", "npc/custom/malangdo_shop_menu"]:
        check(not (SERVER / path).exists(), f"obsolete menu-only artifact remains: {path}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline-ref", required=True)
    args = parser.parse_args()
    items, item_files = effective_items(args.baseline_ref)
    baseline, routes, old_shops, old_files, sources = baseline_inventory(args.baseline_ref, items)
    sys.path.insert(0, str(KB / "tools"))
    from black_market_display_name import load_client_display_names
    names = load_client_display_names()
    documents = {}
    expected_counts = {"dewey": (2936, 3174), "remy": (1043, 1043), "lemy": (23, 23), "milo": (396, 396)}
    for key in KEYS:
        catalog_path = SERVER / f"npc/custom/malangdo_shop_catalog/{key}.yml"
        check(hashlib.sha256(catalog_path.read_bytes()).hexdigest() == CATALOG_SHA256[key], f"Search changed protected item YAML: {key}")
        document = load_yaml(catalog_path.read_text(encoding="utf-8-sig"))
        documents[key] = document
        rows = validate(document, key, items)
        expanded = [(category, row["Id"], 7000, row["Price"]) for row in rows for category in row["Categories"]]
        check(Counter(expanded) == Counter(baseline[key]), f"inventory/category/currency/price mismatch for {key}")
        check((len(rows), len(expanded)) == expected_counts[key], f"inventory count mismatch {key}")
        check(all(row["Id"] in names and row["DisplayName"] == names[row["Id"]] for row in rows), f"client DisplayName mismatch {key}")
        if key in ("milo", "lemy"):
            check(expanded == baseline[key], f"native stock order changed {key}")
        if key == "dewey":
            check({ident for category, ident, _, _ in expanded if category == "garment_limited"} == {20513, 31917}, "Limited inventory changed")
        tree = generated_tree(rows, key)
        leaves = sum(bool(node[4]) for node in tree.values())
        print(f"PASS: {key}: {len(rows)} item rows / {len(expanded)} listings; {leaves} generated drawers; exact stock/categories/prices/names")
    check(Counter(category for category, _, _, _ in baseline["milo"]) ==
          dict(zip(CATEGORIES["milo"], (88, 80, 90, 86, 16, 36))), "Milo category totals")
    new_shops, new_files = barter_graph()
    owned_names = {name for group in routes.values() for name in group} | {"barter_stone_dual"}
    check(not (owned_names & new_shops.keys()), "old catalog stock is still loaded")
    check({k: v for k, v in old_shops.items() if k not in owned_names} == new_shops, "unrelated Barter stock/metadata changed")
    check(not (new_files & REMOVED_IMPORTS), "retired import remains reachable")
    for path in old_files - RETIRED_STOCK - IMPORT_FILES:
        check((SERVER / path).read_bytes() == baseline_bytes(args.baseline_ref, path), f"protected Barter/import file changed: {path}")
    for path in IMPORT_FILES:
        old = yaml.safe_load(baseline_text(args.baseline_ref, path))
        new = yaml.safe_load((SERVER / path).read_text(encoding="utf-8-sig"))
        old["Footer"]["Imports"] = [entry for entry in old["Footer"]["Imports"] if entry["Path"] not in REMOVED_IMPORTS]
        check(new == old, f"unrelated import changed: {path}")
    for path in RETIRED_STOCK:
        check(not (SERVER / path).exists(), f"retired custom stock file remains: {path}")
    for path in item_files:
        check((SERVER / path).read_bytes() == baseline_bytes(args.baseline_ref, path), f"item DB changed: {path}")
    source_checks(args.baseline_ref, sources)
    fixture_checks(documents, items)
    search_fixtures(documents)
    print(f"PASS: {len(new_shops)} unrelated Barters and {len(item_files)} item DB files preserved; only five imports retired")
    print("PASS: NPC dialogue/paging/placement, Limited warning, native integration anchors and builtin signatures")
    print("PASS: bundled rapidyaml KEYVAL mapping versus VAL sequence scalar contract (source check)")
    print("PASS: schema rejection and 40/41, tie-break, digit/Other, add/delete/move/price fixtures (static model)")
    print("PASS: unchanged item YAML hashes; per-NPC search/matching, 50-item pages, generation/busy, Limited and price/default fixtures (static model/source checks)")
    print("PENDING: player build/link, startup/reload logs, native NPC lifecycle and purchases")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, yaml.YAMLError) as error:
        raise SystemExit(f"FAIL: {error}") from error
