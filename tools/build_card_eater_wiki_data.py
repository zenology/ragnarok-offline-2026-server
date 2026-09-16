"""Generate the Wiki Card Eater catalog from the live NPC whitelist."""
from __future__ import annotations

import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
NPC = ROOT / "npc" / "custom" / "card_eater.txt"
ITEM_DB = ROOT / "db" / "re" / "item_db_etc.yml"
OUT = ROOT / "wiki" / "src" / "pages" / "card-eater" / "data" / "cards.ts"


def read_cards() -> list[tuple[int, int]]:
    text = NPC.read_text(encoding="utf-8")
    result: list[tuple[int, int]] = []
    for class_no in range(1, 5):
        match = re.search(rf"setarray \.Class{class_no}\[0\],\s*([^;]+);", text)
        if not match:
            raise SystemExit(f"missing Class{class_no} array")
        ids = [int(value) for value in re.findall(r"\d+", match.group(1))]
        if len(ids) != 30:
            raise SystemExit(f"Class{class_no} has {len(ids)} IDs, expected 30")
        result.extend((item_id, class_no) for item_id in ids)
    if len({item_id for item_id, _ in result}) != 120:
        raise SystemExit("whitelist IDs are not unique")
    return result


def read_names() -> dict[int, str]:
    text = ITEM_DB.read_text(encoding="utf-8")
    rows = re.finditer(r"(?m)^\s*- Id:\s*(\d+)\s*$([\s\S]*?)(?=^\s*- Id:\s*\d+\s*$|\Z)", text)
    names: dict[int, str] = {}
    for row in rows:
        item_id = int(row.group(1))
        name = re.search(r"(?m)^\s*Name:\s*(.+?)\s*$", row.group(2))
        if name:
            names[item_id] = name.group(1).strip().replace('"', '\\"')
    return names


def render() -> str:
    rewards = {1: (2, 10), 2: (8, 30), 3: (14, 50), 4: (30, 90)}
    cards = read_cards()
    names = read_names()
    missing = [item_id for item_id, _ in cards if item_id not in names or not names[item_id]]
    if missing:
        raise SystemExit(f"missing item names: {missing}")
    lines = [
        "export type CardEaterCard = {",
        "  itemId: number",
        "  name: string",
        "  tier: 1 | 2 | 3 | 4",
        "  levelBand: string",
        "  silvervineReward: number",
        "  eventStoneCoinReward: number",
        "}",
        "",
        "export const cardEaterCards: CardEaterCard[] = [",
    ]
    bands = {1: "1–50", 2: "51–99", 3: "100–199", 4: "200+"}
    for item_id, tier in cards:
        silvervine, coins = rewards[tier]
        lines.append(f'  {{ itemId: {item_id}, name: "{names[item_id]}", tier: {tier}, levelBand: "{bands[tier]}", silvervineReward: {silvervine}, eventStoneCoinReward: {coins} }},')
    lines += ["]", ""]
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    generated = render()
    if args.check:
        existing = OUT.read_text(encoding="utf-8") if OUT.exists() else ""
        row_pattern = r"itemId:\s*(\d+),\s*name:\s*(['\"])(.*?)\2"
        expected_rows = [(item_id, name) for item_id, _, name in re.findall(row_pattern, generated)]
        actual_rows = [(item_id, name) for item_id, _, name in re.findall(row_pattern, existing)]
        if actual_rows != expected_rows:
            raise SystemExit("Card Eater Wiki data is stale; run without --check")
        print("check ok: 120 Card Eater cards (30/30/30/30)")
        return
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(generated, encoding="utf-8", newline="\n")
    print(f"generated {OUT}")


if __name__ == "__main__":
    main()
