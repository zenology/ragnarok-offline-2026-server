"""Read-only consistency checks for the Card Eater Wiki catalog and local card art."""

from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
NPC = ROOT / 'npc' / 'custom' / 'card_eater.txt'
SELLER = ROOT / 'npc' / 'custom' / 'card_seller.txt'
ITEM_DB = ROOT / 'db' / 're' / 'item_db_etc.yml'
MOB_DB = ROOT / 'db' / 're' / 'mob_db.yml'
CARDS = ROOT / 'wiki' / 'src' / 'pages' / 'card-eater' / 'data' / 'cards.ts'
ASSETS = ROOT / 'wiki' / 'src' / 'assets' / 'card-eater' / 'cards'
PNG_SIGNATURE = b'\x89PNG\r\n\x1a\n'


def read_indexed_arrays(path: Path, array_name: str, expected_length: int) -> list[int]:
    text = path.read_text(encoding='utf-8')
    matches = re.findall(rf'setarray \.{array_name}\[(\d+)\],\s*([^;]+);', text)
    if not matches:
        raise SystemExit(f'missing {array_name} arrays in {path}')
    values: list[int] = []
    expected_index = 0
    for index_text, body in matches:
        index = int(index_text)
        if index != expected_index:
            raise SystemExit(f'{array_name} index {index}, expected {expected_index}')
        row = [int(value) for value in re.findall(r'\d+', body)]
        values.extend(row)
        expected_index += len(row)
        if len(values) == expected_length:
            return values
        if len(values) > expected_length:
            raise SystemExit(f'{array_name} exceeds {expected_length} rows')
    raise SystemExit(f'{array_name} has {len(values)} rows, expected {expected_length}')


def read_whitelist() -> list[tuple[int, int]]:
    text = NPC.read_text(encoding='utf-8')
    cards: list[tuple[int, int]] = []
    for tier in range(1, 5):
        match = re.search(rf'setarray \.Class{tier}\[0\],\s*([^;]+);', text)
        if not match:
            raise SystemExit(f'missing Class{tier} array')
        ids = [int(value) for value in re.findall(r'\d+', match.group(1))]
        if len(ids) != 30:
            raise SystemExit(f'Class{tier} has {len(ids)} IDs, expected 30')
        cards.extend((item_id, tier) for item_id in ids)
    if len({item_id for item_id, _ in cards}) != 120:
        raise SystemExit('whitelist IDs are not unique')
    return cards


def read_item_names() -> dict[int, str]:
    text = ITEM_DB.read_text(encoding='utf-8')
    names: dict[int, str] = {}
    for row in re.finditer(r'(?m)^\s*- Id:\s*(\d+)\s*$([\s\S]*?)(?=^\s*- Id:\s*\d+\s*$|\Z)', text):
        name = re.search(r'(?m)^\s*Name:\s*(.+?)\s*$', row.group(2))
        if name:
            names[int(row.group(1))] = name.group(1).strip()
    return names


def read_mobs() -> dict[int, tuple[str, int]]:
    text = MOB_DB.read_text(encoding='utf-8')
    mobs: dict[int, tuple[str, int]] = {}
    for row in re.finditer(r'(?m)^  - Id:\s*(\d+)\s*$([\s\S]*?)(?=^  - Id:|\Z)', text):
        name = re.search(r'(?m)^    Name:\s*(.+?)\s*$', row.group(2))
        level = re.search(r'(?m)^    Level:\s*(\d+)\s*$', row.group(2))
        if name and level:
            mobs[int(row.group(1))] = (name.group(1).strip(), int(level.group(1)))
    return mobs


def read_cards() -> tuple[dict[int, tuple[str, int, str, int, int]], dict[int, tuple[int, str, int]]]:
    text = CARDS.read_text(encoding='utf-8')
    rows: dict[int, tuple[str, int, str, int, int]] = {}
    for row in re.finditer(r'(?ms)^  \{\s*itemId:\s*(\d+),(.*?)^  \},?$', text):
        item_id = int(row.group(1))
        body = row.group(2)
        name = re.search(r"name:\s*'([^']+)'", body)
        tier = re.search(r'tier:\s*(\d+)', body)
        band = re.search(r"levelBand:\s*'([^']+)'", body)
        silvervine = re.search(r'silvervineReward:\s*(\d+)', body)
        coins = re.search(r'eventStoneCoinReward:\s*(\d+)', body)
        if not all((name, tier, band, silvervine, coins)):
            raise SystemExit(f'malformed Card Eater row for {item_id}')
        rows[item_id] = (name.group(1), int(tier.group(1)), band.group(1), int(silvervine.group(1)), int(coins.group(1)))

    source_mobs = {
        int(item_id): (int(mob_id), name, int(level))
        for item_id, mob_id, name, level in re.findall(
            r"^  (\d+): \[(\d+), '([^']+)', (\d+)\],?$", text, flags=re.MULTILINE
        )
    }
    return rows, source_mobs


def main() -> None:
    whitelist = read_whitelist()
    expected_tiers = dict(whitelist)
    normal_cards = read_indexed_arrays(SELLER, 'CardIds', 782)
    normal_sources = read_indexed_arrays(SELLER, 'SourceMobIds', 782)
    seller_sources = dict(zip(normal_cards, normal_sources, strict=True))
    names = read_item_names()
    mobs = read_mobs()
    rows, source_mobs = read_cards()

    if set(rows) != set(expected_tiers):
        raise SystemExit('cards.ts IDs do not exactly match the Card Eater whitelist')
    if set(source_mobs) != set(expected_tiers):
        raise SystemExit('cards.ts source monster IDs do not exactly match the Card Eater whitelist')

    bands = {1: '1–50', 2: '51–99', 3: '100–199', 4: '200+'}
    rewards = {1: (2, 10), 2: (8, 30), 3: (14, 50), 4: (30, 90)}
    for item_id, tier in whitelist:
        name, actual_tier, band, silvervine, coins = rows[item_id]
        if names.get(item_id) != name:
            raise SystemExit(f'item name mismatch for {item_id}')
        if (actual_tier, band, silvervine, coins) != (tier, bands[tier], *rewards[tier]):
            raise SystemExit(f'tier, band, or reward mismatch for {item_id}')
        source_mob_id, monster_name, monster_level = source_mobs[item_id]
        if seller_sources.get(item_id) != source_mob_id:
            raise SystemExit(f'SourceMobIds mismatch for {item_id}')
        if mobs.get(source_mob_id) != (monster_name, monster_level):
            raise SystemExit(f'mob name or level mismatch for {item_id}')

    files = {path.stem: path for path in ASSETS.glob('*.png')}
    expected_assets = {str(item_id) for item_id, _ in whitelist}
    if set(files) != expected_assets:
        missing = sorted(expected_assets - set(files), key=int)
        extra = sorted(set(files) - expected_assets, key=int)
        raise SystemExit(f'Card Eater PNG manifest mismatch; missing={missing}, extra={extra}')
    for item_id, path in files.items():
        if path.read_bytes()[:8] != PNG_SIGNATURE:
            raise SystemExit(f'invalid PNG signature for {item_id}')

    print('check ok: 120 Card Eater cards (30/30/30/30), 120 source mobs, 120 PNG assets')


if __name__ == '__main__':
    main()
