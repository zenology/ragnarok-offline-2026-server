"""Read-only Treasure Eater consistency check; retains the former command name."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
NPC = ROOT / 'npc' / 'custom' / 'card_eater.txt'
ITEM_DB = ROOT / 'db' / 'import' / 'item_db_treasure_boxes.yml'
CATALOG = ROOT / 'wiki' / 'src' / 'pages' / 'card-eater' / 'data' / 'cards.ts'
GROUPS = [list(range(9000001, 9000003)), list(range(9000003, 9000006)),
          list(range(9000006, 9000008)), list(range(9000008, 9000011)),
          list(range(9000011, 9000016))]
FRUIT = [2, 8, 14, 20, 30]
COOKIE = [10, 30, 50, 60, 90]


def read_array(text: str, name: str) -> list[int]:
    matches = re.findall(rf'setarray \.{name}\[0\],\s*([^;]+);', text)
    if len(matches) != 1:
        raise SystemExit(f'expected one {name} array')
    return [int(value.strip()) for value in matches[0].split(',')]


def main() -> None:
    npc = NPC.read_text(encoding='utf-8')
    for group, ids in enumerate(GROUPS, start=1):
        if read_array(npc, f'Group{group}') != ids:
            raise SystemExit(f'Group{group} membership mismatch')
    if read_array(npc, 'FruitReward') != FRUIT or read_array(npc, 'CookieReward') != COOKIE:
        raise SystemExit('NPC reward mismatch')
    copies = re.findall(r'copyarray \.AllTreasures\[(\d+)\], \.Group(\d+)\[0\], getarraysize\(\.Group(\d+)\);', npc)
    if copies != [('0', '1', '1'), ('2', '2', '2'), ('5', '3', '3'), ('7', '4', '4'), ('10', '5', '5')]:
        raise SystemExit('AllTreasures copy offsets mismatch')
    rows: dict[int, tuple[str, int, int, int]] = {}
    catalog = CATALOG.read_text(encoding='utf-8')
    for body in re.findall(r'\{([^{}]*\bitemId:\s*\d+[^{}]*)\}', catalog):
        item_id = re.search(r'itemId:\s*(\d+)', body)
        name = re.search(r"name:\s*'([^']+)'", body)
        group = re.search(r'group:\s*(\d+)', body)
        fruit = re.search(r'silvervineReward:\s*(\d+)', body)
        cookie = re.search(r'eventStoneCoinReward:\s*(\d+)', body)
        if not all((item_id, name, group, fruit, cookie)):
            raise SystemExit('malformed treasure Wiki row')
        key = int(item_id.group(1))
        if key in rows:
            raise SystemExit(f'duplicate Wiki ID {key}')
        rows[key] = (name.group(1), int(group.group(1)), int(fruit.group(1)), int(cookie.group(1)))
    if set(rows) != set(range(9000001, 9000016)):
        raise SystemExit('Wiki must contain exactly the 15 accepted treasures')
    db = ITEM_DB.read_text(encoding='utf-8')
    names: dict[int, str] = {}
    for item_id, body in re.findall(r'(?ms)^  - Id:\s*(\d+)\s*\n(.*?)(?=^  - Id:|\Z)', db):
        name = re.search(r'(?m)^    Name:\s*(.+)$', body)
        item_type = re.search(r'(?m)^    Type:\s*(.+)$', body)
        if int(item_id) in rows:
            if int(item_id) in names or not name or not item_type or item_type.group(1).strip() != 'Etc':
                raise SystemExit(f'invalid treasure DB row {item_id}')
            names[int(item_id)] = name.group(1).strip()
    for group, ids in enumerate(GROUPS, start=1):
        for item_id in ids:
            expected = (names.get(item_id), group, FRUIT[group - 1], COOKIE[group - 1])
            if rows[item_id] != expected:
                raise SystemExit(f'Wiki name/group/reward mismatch for {item_id}')
    if sum(row[2] for row in rows.values()) != 266 or sum(row[3] for row in rows.values()) != 840:
        raise SystemExit('one-of-each bulk payout mismatch')
    print('check ok: 15 Treasure Eater boxes (2/3/2/3/5), DB names/types and Wiki rewards; bulk 266 Fruit or 840 Cookie')


if __name__ == '__main__':
    main()
