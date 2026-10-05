"""Read-only regression tests; no generated project data."""
import copy
import re
import tempfile
import unittest
from pathlib import Path

import yaml

from black_market_catalog import (CATALOG_DIR, EQUIPMENT, KEYS, SERVER, TIERS, CatalogError, UniqueKeyLoader,
                                  adjusted_price, courier_price, drawer_key,
                                  drawers, hat_slots, read_all, read_catalog, read_items, search, validate_document)
from audit_black_market_catalog import (BASELINE, CLIENT_NAME_AMENDMENTS, FILES,
                                       approved_names, compare, legacy, source)


def document(rows):
    return {"Header": {"Type": "BLACK_MARKET_CATALOG", "Version": 1}, "Body": rows}


class CatalogTests(unittest.TestCase):
    def test_record_keeps_price_after_reorder(self):
        rows = [dict(Id=10, Price=120, DisplayName="Alpha", Category=0),
                dict(Id=20, Price=980, DisplayName="Beta", Category=0)]
        for ordered in (rows, list(reversed(rows))):
            self.assertEqual(drawers(validate_document("soren", document(ordered)))["ALL"], [(10, 120), (20, 980)])

    def test_validation_rejects_entire_bad_input(self):
        row = dict(Id=10, Price=120, DisplayName="Alpha", Category=0)
        for field, value in (("Id", 0), ("Price", -1), ("Price", 0), ("Price", "120"),
                             ("Price", True), ("DisplayName", 12), ("Category", 22)):
            bad = {**row, field: value}
            with self.subTest(field=field, value=value), self.assertRaises(CatalogError):
                validate_document("soren", document([row, bad]))
        for field in row:
            bad = dict(row); del bad[field]
            with self.assertRaises(CatalogError):
                validate_document("soren", document([bad]))
        with self.assertRaises(CatalogError):
            validate_document("soren", document([row, row]))
        with self.assertRaises(CatalogError):
            validate_document("soren", document([row]), {20})

    def test_yaml_errors_and_duplicate_fields(self):
        with self.assertRaises(yaml.YAMLError):
            yaml.load("Body: [{Id: 10", Loader=UniqueKeyLoader)
        with self.assertRaises(CatalogError):
            yaml.load("Id: 10\nId: 20\n", Loader=UniqueKeyLoader)
        with self.assertRaises(CatalogError):
            validate_document("soren", {**document([]), "Footer": {"Imports": []}})
        with self.assertRaises(CatalogError):
            validate_document("soren", {"Header": {"Type": "BLACK_MARKET_CATALOG", "Version": True}, "Body": []})
        with self.assertRaises(CatalogError):
            yaml.load("Id: 010\n", Loader=UniqueKeyLoader)

    def test_boundaries(self):
        for count, expected in ((0, None), (1, "ALL"), (99, "ALL"), (100, "AH"), (199, "AH"), (200, "A")):
            self.assertEqual(drawer_key("Alpha", count), expected)
        self.assertEqual(drawer_key("Ice", 100), "IP")
        self.assertEqual(drawer_key("Zulu", 199), "QZ9")
        self.assertEqual(drawer_key("9 lives", 200), "0")
        self.assertEqual(drawer_key("! hat", 200), "A")
        self.assertIsNone(drawer_key(" Rigid Earth Deleter Card", 783, True))

    def test_cards_keep_source_order_and_digits_first(self):
        rows = [dict(Id=i, Price=i, DisplayName="Alpha") for i in range(1, 200)]
        rows.append(dict(Id=200, Price=200, DisplayName="9 lives"))
        result = drawers(rows, cards=True)
        self.assertEqual(list(result), ["0", "A"])
        self.assertEqual(result["A"], [(i, i) for i in range(1, 200)])

    def test_tiers_complete_and_zero_unavailable(self):
        row = dict(Id=985, Category=7, TierPrices=dict(zip(TIERS, (0, 0, 10, 20, 30, 40))))
        self.assertEqual(validate_document("rooke", document([row])), [row])
        bad = copy.deepcopy(row); del bad["TierPrices"][TIERS[0]]
        with self.assertRaises(CatalogError):
            validate_document("rooke", document([bad]))
        bad = copy.deepcopy(row); bad["TierPrices"][TIERS[0]] = -1
        with self.assertRaises(CatalogError):
            validate_document("rooke", document([bad]))
        self.assertEqual(courier_price(985), 1182)
        self.assertEqual(courier_price(3), 4)

    def test_special_cards_preserve_nonletter_fallback(self):
        rows = [dict(Id=31012, Price=70200, DisplayName="[LoVA] Bahamut Card"),
                dict(Id=27350, Price=100, DisplayName=" Rigid Earth Deleter Card")]
        self.assertEqual(drawers(rows, cards=True), {})
        self.assertEqual(drawers(rows[:1], cards=True, card_special=True),
                         {"ALL": [(31012, 70200)]})

    def test_search_semantics(self):
        catalogs = dict.fromkeys(EQUIPMENT, [])
        catalogs["harlan"] = [dict(Id=10, Price=1, DisplayName="Alpha Magic Hat")]
        catalogs["madame-celestine"] = [dict(Id=20, Price=2, DisplayName="Magic Temporal Boots")]
        self.assertEqual(search(catalogs, "  00010  "), [10])
        self.assertEqual(search(catalogs, " hat   ALPHA "), [10])
        self.assertEqual(search(catalogs, "TEMP boots"), [20])
        self.assertEqual(search(catalogs, "99999999999999999999"), [])
        self.assertEqual(search(catalogs, "123456"), [])
        self.assertEqual(search(catalogs, "no result"), [])
        for query in ("", "    ", "hat"):
            with self.assertRaises(CatalogError): search(catalogs, query)

    def test_daily_rounding_and_overflow(self):
        self.assertEqual(adjusted_price(5, 10), 6)
        self.assertEqual(adjusted_price(100, -10), 90)
        self.assertEqual(adjusted_price(100, 30), 130)
        for price, rate in ((0, 0), (100, 31), (2147483647, 30)):
            with self.assertRaises(CatalogError): adjusted_price(price, rate)

    def test_rooke_exact_baseline(self):
        expected = legacy()["rooke"]
        actual = read_catalog("rooke")
        self.assertEqual(actual, expected)
        self.assertEqual(len(actual), 106)
        counts = [sum(r["Category"] == 7 and r["TierPrices"][t] > 0 for r in actual) for t in TIERS]
        self.assertEqual(counts, [7, 7, 7, 8, 13, 13])

    def test_authored_catalog_records_and_real_ids(self):
        expected, items = legacy(), read_items()
        for key in KEYS:
            if not (CATALOG_DIR / f"{key}.yml").exists():
                continue  # Full-migration audit separately requires all 13.
            with self.subTest(catalog=key):
                actual = read_catalog(key, item_ids=set(items))
                if key in EQUIPMENT:
                    self.assertEqual({r["Id"]: r for r in actual},
                                     {r["Id"]: r for r in approved_names(expected[key])})
                else:
                    self.assertEqual(actual, expected[key])

    def test_client_name_amendments_do_not_allow_price_or_name_drift(self):
        expected, actual = legacy(), read_all()
        self.assertEqual(len(CLIENT_NAME_AMENDMENTS), 12)
        compare(expected, actual)
        for field, value in (("Price", 1), ("DisplayName", "Unapproved name")):
            changed = copy.deepcopy(actual)
            next(row for row in changed["mordain"] if row["Id"] == 450272)[field] = value
            with self.subTest(field=field), self.assertRaises(CatalogError):
                compare(expected, changed)

    def test_rooke_and_all_courier_routes_preserved(self):
        old = source(BASELINE, "consumable_seller.txt").split("// Nine categories x six progression tiers.")[0]
        new = (SERVER / "npc/custom/consumable_seller.txt").read_text(encoding="utf-8").split("// Nine categories x six progression tiers,")[0]
        old = old.replace('getvariableofnpc(.CatHas[.@tier * 9 + .@cat], "cons_seller_db")',
                          'bmc_shopcount("cons" + .@cat + "#T" + .@tier)')
        old = old.replace('getvariableofnpc(.CourierHas[.@tier], "cons_seller_db")',
                          'bmc_shopcount("rookup#T" + .@tier)')
        self.assertEqual(new, old)

    def test_card_entry_access_and_dialogue_preserved(self):
        old = source(BASELINE, "card_seller.txt")
        new = (SERVER / "npc/custom/card_seller.txt").read_text(encoding="utf-8")
        def entry(text):
            start = text.index("paramk,47,186,4")
            return text[start:text.index("-\t", start)]
        old, new = entry(old), entry(new)
        gates = ("eaclass()", ".@eac", "EAJL_", "JOB_SPIRIT_HANDLER",
                 "BlackMarketPoints <", ".@min_look =", ".@special_look =")
        self.assertEqual([line.strip() for line in old.splitlines() if any(token in line for token in gates)],
                         [line.strip() for line in new.splitlines() if any(token in line for token in gates)])
        replaced = ("I am sorry, it seems", "I cannot find any cards",
                    "Please contact a game master", "This stall's catalog is unavailable")
        def dialogue(text):
            return [line.strip() for line in text.splitlines() if line.strip().startswith("mes ")
                    and not any(token in line for token in replaced)]
        self.assertEqual(dialogue(old), dialogue(new))
        self.assertNotIn("getvariableofnpc", new)

    def test_card_shells_and_shared_functions_preserved(self):
        old = source(BASELINE, "card_seller.txt")
        new = (SERVER / "npc/custom/card_seller.txt").read_text(encoding="utf-8")
        self.assertEqual(old[old.index("function\tscript"):old.index("paramk,47,186,4")],
                         new[new.index("function\tscript"):new.index("paramk,47,186,4")])
        shells = re.findall(r"^-\tpointshop\t([^\t]+)\t-1,BlackMarketPoints,([^\r\n]+)$", new, re.M)
        suffixes = ("ALL", "AH", "IP", "QZ9", "0", *"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        self.assertEqual({name for name, _ in shells}, {f"{prefix}#{suffix}" for prefix in
                         ("card_mob", "card_miniboss", "card_mvp", "card_special") for suffix in suffixes})
        self.assertEqual(len(shells), 124)
        self.assertTrue(all(stock == "501:1" for _, stock in shells))
        self.assertNotIn("setarray", new)
        self.assertNotIn("npcshopadditem", new)

    def test_harlan_slots_and_shells(self):
        expected, actual, items = legacy()["harlan"], read_catalog("harlan"), read_items()
        old_slots, new_slots = hat_slots(expected, items), hat_slots(actual, items)
        self.assertEqual([len(rows) for rows in new_slots], [361, 128, 87])
        self.assertEqual([drawers(rows) for rows in old_slots], [drawers(rows) for rows in new_slots])
        npc = (SERVER / "npc/custom/hat_seller.txt").read_text(encoding="utf-8")
        shells = re.findall(r"^-\tpointshop\t([^\t]+)\t-1,BlackMarketPoints,([^\r\n]+)$", npc, re.M)
        suffixes = ("ALL", "AH", "IP", "QZ9", "0", *"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        self.assertEqual({name for name, _ in shells}, {f"hat{slot}#{suffix}" for slot in range(3) for suffix in suffixes})
        self.assertEqual(len(shells), 93)
        self.assertTrue(all(stock == "501:1" for _, stock in shells))
        self.assertNotIn("setarray", npc)
        self.assertNotIn("getvariableofnpc", npc)

    def test_complete_catalog_counts_and_cross_equipment_duplicate(self):
        items = read_items()
        catalogs = read_all(item_ids=set(items))
        self.assertEqual(sum(len(catalogs[key]) for key in EQUIPMENT), 3444)
        self.assertEqual(sum(len(catalogs[key]) for key in KEYS if key.startswith("card-")), 1224)
        self.assertEqual(len(catalogs["rooke"]), 106)
        boots = {row["Id"]: row for row in catalogs["madame-celestine"] if row["Id"] in range(22006, 22012)}
        self.assertEqual(set(boots), set(range(22006, 22012)))
        self.assertTrue(all(row["Category"] == 79 and items[item_id].get("Slots") == 1 for item_id, row in boots.items()))
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            for key in KEYS:
                rows = []
                if key in ("soren", "mordain"):
                    rows = [dict(Id=10, Price=120, DisplayName="Alpha", Category=0)]
                (directory / f"{key}.yml").write_text(yaml.safe_dump(document(rows)), encoding="utf-8")
            with self.assertRaises(CatalogError):
                read_all(directory)

    def test_equipment_dialogue_access_titles_and_shell_coverage(self):
        from black_market_catalog import PREFIXES
        suffixes = ("ALL", "AH", "IP", "QZ9", "0", *"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        for key, filename in FILES.items():
            with self.subTest(catalog=key):
                old = source(BASELINE, filename)
                new = (SERVER / "npc/custom" / filename).read_text(encoding="utf-8")
                without_guard = re.sub(r'\tif \(!bmc_ready\("[^"]+"\)\) \{.*?\n\t\}\n', "", new, count=1, flags=re.S)
                dialogue = lambda text: [line.strip() for line in text.splitlines() if line.strip().startswith("mes ")]
                self.assertEqual(dialogue(old), dialogue(without_guard))
                declarations = lambda text: re.findall(r'^paramk,[^\n]+', text, re.M)
                self.assertEqual(declarations(old), declarations(new))
                titles = lambda text: re.findall(r'setunittitle\([^;]+;', text)
                self.assertEqual(titles(old), titles(new))
                gates = lambda text: [line.strip() for line in text.splitlines() if any(token in line for token in ("Class ==", "Upper ==", "EAJL_", "EAJ_BASEMASK", ".@eac ="))]
                self.assertEqual(gates(old), gates(new))
                shells = re.findall(r"^-\tpointshop\t([^\t]+)\t-1,BlackMarketPoints,([^\r\n]+)$", new, re.M)
                expected = set(PREFIXES[key]) if key == "madame-celestine" else {prefix+'#'+suffix for prefix in PREFIXES[key] for suffix in suffixes}
                self.assertEqual({name for name, _ in shells}, expected)
                self.assertEqual(len(shells), len(expected))
                self.assertTrue(all(stock == "501:1" for _, stock in shells))
                self.assertNotIn("getvariableofnpc", new)
        config = (SERVER / "npc/scripts_custom.conf").read_text(encoding="utf-8")
        self.assertEqual(config.count("npc: npc/custom/black_market_catalog_db.txt"), 1)
        self.assertLess(config.index("black_market_catalog_db.txt"), config.index("card_seller.txt"))


if __name__ == "__main__":
    unittest.main()
