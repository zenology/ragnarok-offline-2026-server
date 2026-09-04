#!/usr/bin/env python3
"""Code-first Black Market catalog audit and narrow garment itemInfo repair.

This tool uses only the checked-out server/client/resource trees.  It does not
read Knowledge Base manifests or audit JSON, and it never reads or writes GRF
archives.  ``--repair-garment-iteminfo`` appends only rows missing from the
live garment itemInfo table, using server item-db facts and an AegisName
resource stem; it never changes seller stock, prices, or item DB rows.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import sys
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SERVER = ROOT / "Server"
SYSTEM_EN = ROOT / "Client" / "ragnarok-offline-client" / "SystemEN"
GRF = ROOT / "Client" / "GRF-Resource"
IRO = GRF / "02 IRO"
OVERLAY = GRF / "03 output"
GARMENT_INFO = SYSTEM_EN / "itemInfo_garment_seller.lua"

sys.path.insert(0, r"D:\A04 AI Knowledge Base\Ragnarok Offline\tools")
from grf_path_encoding import canonical_overlay_path, logical_component, logical_to_display  # noqa: E402

POINTSHOP = re.compile(r"pointshop\s+(\S+)\s+-1,#CASHPOINTS,(.*)$")
STOCK = re.compile(r"(?<!\d)(\d+):\d+")
ARRAY = re.compile(r"setarray\s+\.(\w+)\[\d+\],\s*([0-9, ]+);")
ITEM_ID = re.compile(r"^\s*- Id:\s*(\d+)")


def seller_stock() -> dict[str, set[int]]:
    out: dict[str, set[int]] = {}
    for path in sorted((SERVER / "npc" / "custom").glob("*_seller.txt")):
        ids: set[int] = set()
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            match = POINTSHOP.search(line)
            if match:
                ids.update(int(value) for value in STOCK.findall(match.group(2)))
        if path.name == "hat_seller.txt":
            text = path.read_text(encoding="utf-8", errors="replace")
            for match in ARRAY.finditer(text):
                if match.group(1) in {"IroId", "RentBoxId", "EvtId"}:
                    ids.update(int(value) for value in match.group(2).split(",") if value.strip())
        elif path.name == "card_seller.txt":
            text = path.read_text(encoding="utf-8", errors="replace")
            for match in ARRAY.finditer(text):
                if match.group(1) in {"CardIds", "MinibossCardIds", "MvpCardIds", "SpecialCardIds"}:
                    ids.update(int(value) for value in match.group(2).split(",") if value.strip())
        out[path.name] = ids
    return out


def item_db() -> dict[int, dict[str, str]]:
    records: dict[int, dict[str, str]] = {}
    for path in (SERVER / "db").rglob("item_db*.yml"):
        current: int | None = None
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            match = ITEM_ID.match(line)
            if match:
                current = int(match.group(1))
                records[current] = {"path": str(path.relative_to(ROOT)).replace("\\", "/")}
                continue
            if current is None:
                continue
            for key in ("AegisName", "Name", "Weight", "Defense", "Slots", "EquipLevelMin"):
                field = re.match(r"^\s*" + key + r":\s*(.*)", line)
                if field:
                    records[current][key] = field.group(1).strip()
    return records


def iteminfo_ids() -> dict[int, dict[str, str]]:
    rows: dict[int, dict[str, str]] = {}
    paths = [SYSTEM_EN / "LuaFiles514" / "itemInfo.lua", *sorted(SYSTEM_EN.glob("itemInfo_*.lua"))]
    for path in paths:
        if not path.exists():
            continue
        current: int | None = None
        depth = 0
        row: dict[str, str] | None = None
        for line in path.read_text(encoding="cp1252", errors="replace").splitlines():
            match = re.match(r"^\s*\[(\d+)\]\s*=\s*\{", line)
            if match:
                current = int(match.group(1))
                depth = line.count("{") - line.count("}")
                row = {"path": str(path.relative_to(ROOT)).replace("\\", "/"), "name": "", "resource": ""}
                continue
            if current is None or row is None:
                continue
            for key, target in (("identifiedDisplayName", "name"), ("identifiedResourceName", "resource")):
                field = re.match(r'^\s*' + key + r'\s*=\s*"((?:\\.|[^"])*)"', line)
                if field:
                    row[target] = field.group(1)
            depth += line.count("{") - line.count("}")
            if depth <= 0:
                # Some sidecars contain resource override tables after the
                # item table.  They are not itemInfo rows and must not erase
                # the real display-name row from the loader audit.
                if current not in rows or row["name"]:
                    rows[current] = row
                current = None
                row = None
                depth = 0
    return rows


def resource_path(root: Path, data_prefix: bool, resource: str, kind: str, ext: str) -> Path:
    base = root / "data" if data_prefix else root
    display = logical_to_display(logical_component(resource))
    if kind in {"item", "collection"}:
        candidates = [base / "texture" / kind / f"{display}.{ext}", base / "texture" / logical_to_display("유저인터페이스") / kind / f"{display}.{ext}"]
    else:
        candidates = [base / "sprite" / logical_to_display("아이템") / f"{display}.{ext}"]
    return next((path for path in candidates if path.is_file()), candidates[0])


def existing_asset(root: Path, data_prefix: bool, resource: str, kind: str, ext: str) -> Path | None:
    path = resource_path(root, data_prefix, resource, kind, ext)
    return path if path.is_file() else None


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def copy_exact_from_iro(resource: str, kind: str, ext: str) -> str:
    source = existing_asset(IRO, True, resource, kind, ext)
    if source is None:
        return "missing_source"
    destination = canonical_overlay_path(OVERLAY / "data" / source.relative_to(IRO / "data"), OVERLAY)
    if destination.exists():
        return "already_present" if sha256(destination) == sha256(source) else "conflict"
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)
    return "copied"


def fetch_dp_texture(item_id: int, resource: str, kind: str) -> str:
    from PIL import Image
    from io import BytesIO

    url = f"https://www.divine-pride.net/img/items/{kind}/iRO/{item_id}"
    request = urllib.request.Request(url, headers={"User-Agent": "ragnarok-offline-code-first-audit/1.0"})
    with urllib.request.urlopen(request, timeout=30) as response:
        payload = response.read()
    with Image.open(BytesIO(payload)) as image:
        expected = (24, 24) if kind == "item" else (75, 100)
        image = image.convert("RGB").resize(expected, Image.Resampling.NEAREST)
        logical = logical_component(resource)
        destination = canonical_overlay_path(
            OVERLAY / "data" / "texture" / logical_to_display("유저인터페이스") / kind / f"{logical_to_display(logical)}.bmp",
            OVERLAY,
        )
        if destination.exists():
            return "already_present"
        destination.parent.mkdir(parents=True, exist_ok=True)
        image.save(destination, format="BMP")
    return "fetched"


def append_missing_garment_rows(missing: list[int], db: dict[int, dict[str, str]]) -> None:
    if not missing:
        return
    text = GARMENT_INFO.read_text(encoding="cp949")
    additions: list[str] = []
    for item_id in missing:
        row = db[item_id]
        name = row.get("Name", row.get("AegisName", "Item"))
        resource = row.get("AegisName", "")
        defense = row.get("Defense", "0")
        weight = row.get("Weight", "0")
        slots = row.get("Slots", "0")
        additions.extend([
            f"\t[{item_id}] = {{",
            '\t\tunidentifiedDisplayName = "Unidentified Garment",',
            '\t\tunidentifiedResourceName = "Garment",',
            '\t\tunidentifiedDescriptionName = { "Can be identified by using a ^990099Magnifier^000000." },',
            f'\t\tidentifiedDisplayName = "{name.replace(chr(34), chr(92)+chr(34))}",',
            f'\t\tidentifiedResourceName = "{resource}",',
            '\t\tidentifiedDescriptionName = {',
            f'\t\t\t"^0000CCType:^000000 Garment",',
            f'\t\t\t"^0000CCDefense:^000000 {defense}",',
            f'\t\t\t"^0000CCWeight:^000000 {weight}",',
            f'\t\t\t"^0000CCSlots:^000000 {slots}"',
            '\t\t},',
            f'\t\tslotCount = {slots},',
            '\t\tClassNum = 0,',
            '\t\tcostume = false',
            '\t},',
        ])
    anchor = "\n}"
    if anchor not in text:
        raise RuntimeError(f"cannot find garment table closing anchor in {GARMENT_INFO}")
    GARMENT_INFO.write_text(text.replace(anchor, "\n" + "\n".join(additions) + anchor, 1), encoding="cp949")


def audit(repair: bool, repair_assets: bool) -> int:
    shops = seller_stock()
    stock = set().union(*shops.values()) if shops else set()
    db = item_db()
    info = iteminfo_ids()
    missing_info = sorted(stock - set(info))
    if repair:
        garment_missing = [item_id for item_id in missing_info if item_id in shops.get("garment_seller.txt", set())]
        append_missing_garment_rows(garment_missing, db)
        info = iteminfo_ids()
        missing_info = sorted(stock - set(info))

    asset_actions: list[dict[str, object]] = []
    if repair_assets:
        for item_id in sorted(stock & set(info)):
            resource = info[item_id]["resource"]
            if not resource:
                continue
            for kind, ext in (("item", "bmp"), ("collection", "bmp"), ("ground", "spr"), ("ground", "act")):
                if existing_asset(GRF / "01 rAthena", False, resource, kind, ext) or existing_asset(OVERLAY, True, resource, kind, ext):
                    continue
                if existing_asset(IRO, True, resource, kind, ext):
                    action = copy_exact_from_iro(resource, kind, ext)
                    asset_actions.append({"id": item_id, "kind": f"{kind}.{ext}", "action": action})
            for kind in ("item", "collection"):
                if existing_asset(GRF / "01 rAthena", False, resource, kind, "bmp") or existing_asset(OVERLAY, True, resource, kind, "bmp") or existing_asset(IRO, True, resource, kind, "bmp"):
                    continue
                try:
                    action = fetch_dp_texture(item_id, resource, kind)
                except Exception as exc:  # preserve an explicit unresolved source result
                    action = f"fetch_failed:{type(exc).__name__}"
                asset_actions.append({"id": item_id, "kind": f"{kind}.bmp", "action": action})

    unknown = sorted(item_id for item_id in stock if item_id not in info or info[item_id]["name"] in {"", "Unknown Item"})
    placeholder = {"", "Armor", "Accessory", "Shield", "Shoes", "Garment", "Weapon", "Unknown Item"}
    missing_assets: list[dict[str, object]] = []
    for item_id in sorted(stock & set(info)):
        resource = info[item_id]["resource"]
        if resource in placeholder:
            missing_assets.append({"id": item_id, "reason": "placeholder_resource", "resource": resource})
            continue
        checks = {
            "item": existing_asset(IRO, True, resource, "item", "bmp") or existing_asset(OVERLAY, True, resource, "item", "bmp") or existing_asset(GRF / "01 rAthena", False, resource, "item", "bmp"),
            "collection": existing_asset(IRO, True, resource, "collection", "bmp") or existing_asset(OVERLAY, True, resource, "collection", "bmp") or existing_asset(GRF / "01 rAthena", False, resource, "collection", "bmp"),
        }
        # Cards, consumables, and other non-equipment stock do not have a
        # wearable ground model.  Armor rows do require an exact .spr/.act
        # pair; unresolved ground models are reported, never substituted.
        if db.get(item_id, {}).get("Type") == "Armor":
            checks.update({
                "spr": existing_asset(IRO, True, resource, "ground", "spr") or existing_asset(OVERLAY, True, resource, "ground", "spr") or existing_asset(GRF / "01 rAthena", False, resource, "ground", "spr"),
                "act": existing_asset(IRO, True, resource, "ground", "act") or existing_asset(OVERLAY, True, resource, "ground", "act") or existing_asset(GRF / "01 rAthena", False, resource, "ground", "act"),
            })
        absent = [kind for kind, path in checks.items() if path is None]
        if absent:
            missing_assets.append({"id": item_id, "resource": resource, "missing": absent})

    result = {
        "stock_total": len(stock),
        "db_missing": sorted(stock - set(db)),
        "iteminfo_missing": missing_info,
        "unknown_item": unknown,
        "asset_gap_count": len(missing_assets),
        "asset_gaps": missing_assets,
        "seller_counts": {name: len(ids) for name, ids in sorted(shops.items())},
        "asset_actions": asset_actions,
    }
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 1 if result["db_missing"] or result["iteminfo_missing"] or result["unknown_item"] or result["asset_gap_count"] else 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repair-garment-iteminfo", action="store_true")
    parser.add_argument("--repair-assets", action="store_true")
    args = parser.parse_args()
    return audit(args.repair_garment_iteminfo, args.repair_assets)


if __name__ == "__main__":
    raise SystemExit(main())
