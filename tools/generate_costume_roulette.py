#!/usr/bin/env python3
"""Generate the slot-based Costume Roulette NPC from the local item database."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


SLOTS = (
    ("Costume_Head_Top", "Head Top"),
    ("Costume_Head_Mid", "Head Mid"),
    ("Costume_Head_Low", "Head Low / Mouth"),
    ("Costume_Garment", "Garment"),
)
SOURCE_FILES = (
    Path("db/re/item_db_equip.yml"),
    Path("db/import/item_db_iro_hats.yml"),
    Path("db/import/item_db_iro_hats_hard.yml"),
)
FIELD = re.compile(r"^    (Type|Name): (.*)$")
LOCATION = re.compile(r"^      (Costume_[A-Za-z_]+): true$")


def parse_items(root: Path) -> dict[int, dict[str, object]]:
    items: dict[int, dict[str, object]] = {}
    for relative in SOURCE_FILES:
        text = (root / relative).read_text(encoding="utf-8")
        for block in re.split(r"(?m)^  - Id: ", text)[1:]:
            first, *rest = block.splitlines()
            match = re.match(r"(\d+)", first)
            if not match:
                continue
            item_id = int(match.group(1))
            item_type = ""
            name = ""
            locations: set[str] = set()
            for line in rest:
                field = FIELD.match(line)
                if field:
                    if field.group(1) == "Type":
                        item_type = field.group(2).strip()
                    else:
                        name = field.group(2).strip()
                location = LOCATION.match(line)
                if location:
                    locations.add(location.group(1))
            items[item_id] = {
                "type": item_type,
                "name": name.strip("\"'"),
                "locations": locations,
            }
    return items


def build_pool(root: Path) -> tuple[list[list[int]], list[int]]:
    items = parse_items(root)
    pools: list[list[int]] = []
    for slot, _label in SLOTS:
        candidates = []
        for item_id, item in items.items():
            locations = item["locations"]
            if (
                item["type"] == "Armor"
                and slot in locations
                and "Costume_Floor" not in locations
                and "rental" not in str(item["name"]).lower()
            ):
                candidates.append(item_id)
        candidates.sort(key=lambda item_id: (str(items[item_id]["name"]).lower(), item_id))
        pools.append(candidates)
    return pools, sorted({item_id for pool in pools for item_id in pool})


def format_setarray(name: str, values: list[int], chunk_size: int = 24) -> list[str]:
    if not values:
        return [f"\tsetarray {name}[0];"]
    lines = []
    for start in range(0, len(values), chunk_size):
        chunk = ",".join(str(value) for value in values[start : start + chunk_size])
        lines.append(f"\tsetarray {name}[{start}], {chunk};")
    return lines


def render(root: Path) -> tuple[str, dict[str, object]]:
    pools, unique_ids = build_pool(root)
    starts = []
    cursor = 0
    for pool in pools:
        starts.append(cursor)
        cursor += len(pool)

    lines = [
        "//===== Ragnarok Offline =====================================",
        "//= Slot-based Costume Roulette",
        "//= Generated from the local Armor item database.",
        "//= Regenerate with tools/generate_costume_roulette.py after item DB changes.",
        "//============================================================",
        "",
        "malangdo,141,137,0\tscript\tCostume Roulette\t4_M_MERCAT2,{",
        '\tmes "[Costume Roulette]";',
        '\tmes "Mrrp~ Pick a costume part and I will spin that drawer for you, nya~";',
        '\tmes "One Silvervine Fruit buys one spin, nya.";',
        '\tmes "Pity: " + #COSTUME_ROULETTE_PITY + "/20";',
        "\tnext;",
        '\t.@slot = select("Costume Head Top:Costume Head Mid:Costume Head Low / Mouth:Costume Garment:Leave") - 1;',
        '\tif (.@slot < 0 || .@slot >= 4) close;',
        '\t.@first_spin = 1;',
        'L_Spin:',
        '\t.@count = getvariableofnpc(.SlotCount[.@slot], "costume_roulette_db");',
        '\tif (.@count <= 0) {',
        '\t\tmes "Mrr...? That drawer is empty, nya.";',
        '\t\tclose;',
        "\t}",
        '\tif (countitem(6417) < 1) {',
        '\t\tmes "Mrrp... Bring me one Silvervine Fruit, nya~";',
        '\t\tclose;',
        "\t}",
        '\t.@start = getvariableofnpc(.SlotStart[.@slot], "costume_roulette_db");',
        '\t.@id = getelementofarray(getvariableofnpc(.Ids[0], "costume_roulette_db"), .@start + rand(.@count));',
        '\tif (!checkweight(.@id, 1) || !checkweight(7051, 1)) {',
        '\t\tmes "Mrr... Your bag is too full, nya. Make some room first, or I cannot promise the costume and pity gift.";',
        '\t\tclose;',
        "\t}",
        '\tif (.@first_spin) {',
        '\t\tmes "Spend one Silvervine Fruit for this spin, nya?";',
        '\t\tif (select("Spin:Cancel") == 2) close;',
        '\t\t.@first_spin = 0;',
        '\t}',
        '\tif (countitem(6417) < 1) {',
        '\t\tmes "Mrr... You do not have any Silvervine Fruit left, nya.";',
        '\t\tclose;',
        "\t}",
        '\tdelitem 6417, 1;',
        '\tgetitem .@id, 1;',
        '\t#COSTUME_ROULETTE_PITY = #COSTUME_ROULETTE_PITY + 1;',
        '\tmes "Mrrp~ You received ^0055FF" + getitemname(.@id) + "^000000, nya!";',
        '\tif (#COSTUME_ROULETTE_PITY >= 20) {',
        '\t\t#COSTUME_ROULETTE_PITY = 0;',
        '\t\tgetitem 7051, 1;',
        '\t\tmes "Thank you, nya~ Silvervine was delicious! Here, take this ^0055FFSilk Mat^000000.";',
        '\t\tmes "My mama keeps knitting these for me, so I have lots. Please take one, nya~";',
        "\t}",
        '\tnext;',
        '\tif (select("Spin again:Finish:Cancel") != 1) close;',
        '\tgoto L_Spin;',
        "}",
        "",
        "-\tscript\tcostume_roulette_db\t-1,{",
        "OnInit:",
        "\tsetarray .SlotStart[0], " + ",".join(str(start) for start in starts) + ";",
        "\tsetarray .SlotCount[0], " + ",".join(str(len(pool)) for pool in pools) + ";",
    ]
    lines.extend(format_setarray(".Ids", [item_id for pool in pools for item_id in pool]))
    lines.extend([
        '\tdebugmes "Costume Roulette: loaded " + getarraysize(.Ids) + " slot entries (" + .SlotCount[0] + "/" + .SlotCount[1] + "/" + .SlotCount[2] + "/" + .SlotCount[3] + ").";',
        "\tend;",
        "}",
        "",
    ])
    stats = {
        "slot_counts": [len(pool) for pool in pools],
        "unique_ids": len(unique_ids),
        "total_slot_entries": sum(len(pool) for pool in pools),
    }
    return "\n".join(lines), stats


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--output", type=Path, default=Path("npc/custom/costume_roulette.txt"))
    args = parser.parse_args()
    content, stats = render(args.root)
    output = args.root / args.output if not args.output.is_absolute() else args.output
    output.write_text(content, encoding="utf-8", newline="\n")
    print(f"Wrote {output}")
    print(f"Unique IDs: {stats['unique_ids']}")
    print(f"Slot counts: {stats['slot_counts']}")
    print(f"Slot entries: {stats['total_slot_entries']}")


if __name__ == "__main__":
    main()
