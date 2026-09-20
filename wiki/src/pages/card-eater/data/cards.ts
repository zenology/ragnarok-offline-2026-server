type CardEaterCard = {
  itemId: number
  name: string
  sourceMobId: number
  monsterName: string
  monsterLevel: number
  tier: 1 | 2 | 3 | 4
  levelBand: string
  silvervineReward: number
  eventStoneCoinReward: number
}

type CardEaterCardBase = Omit<CardEaterCard, 'sourceMobId' | 'monsterName' | 'monsterLevel'>

const sourceMobs: Record<number, readonly [number, string, number]> = {
  4659: [3495, 'Eggring', 1],
  4663: [3496, 'Leaf Lunatic', 3],
  4664: [3497, 'Grass Fabre', 7],
  4021: [1052, 'Rocker', 15],
  4017: [1167, 'Savage Babe', 14],
  4022: [1014, 'Spore', 18],
  4037: [1025, 'Boa', 18],
  4034: [1024, 'Wormtail', 17],
  4031: [1019, 'Peco Peco', 25],
  4036: [1055, 'Muka', 23],
  4007: [1047, 'Peco Peco Egg', 7],
  4006: [1063, 'Lunatic', 3],
  4009: [1011, 'Chonchon', 5],
  4030: [1020, 'Mandragora', 13],
  4040: [1018, 'Creamy', 23],
  4014: [1012, 'Roda Frog', 13],
  4032: [1094, 'Ambernite', 19],
  4020: [1005, 'Familiar', 24],
  4025: [1076, 'Skeleton', 27],
  4033: [1031, 'Poporing', 30],
  4013: [1097, 'Ant Egg', 28],
  4074: [1060, 'Bigfoot', 29],
  4039: [1174, 'Stainer', 21],
  4063: [1103, 'Caramel', 25],
  4048: [1077, 'Poison Spore', 26],
  4044: [1056, 'Smokie', 29],
  4052: [1033, 'Elder Willow', 34],
  4041: [1104, 'Coco', 38],
  4045: [1128, 'Horn', 32],
  4066: [1023, 'Orc Warrior', 44],
  4104: [1164, 'Requiem', 71],
  4064: [1178, 'Zerom', 70],
  4117: [1037, 'Side Winder', 70],
  4079: [1139, 'Mantis', 65],
  4114: [1099, 'Argiope', 75],
  4120: [1156, 'Petite', 79],
  4118: [1155, 'Petite', 86],
  27122: [2074, 'Curupira', 68],
  27123: [2073, 'Toucan', 70],
  27124: [2072, 'Jaguar', 71],
  4325: [1376, 'Harpy', 83],
  4150: [1372, 'Goat', 80],
  4161: [1369, 'Grand Peco', 75],
  4228: [1386, 'Sleeper', 81],
  4299: [1269, 'Clock', 81],
  4313: [1199, 'Punk', 82],
  4185: [1195, 'Rideword', 74],
  4268: [1257, 'Injustice', 95],
  4194: [1201, 'Rybio', 98],
  4222: [1196, 'Skeleton Prisoner', 91],
  4171: [1198, 'Dark Priest', 98],
  4422: [1782, 'Roween', 95],
  4082: [1106, 'Desert Wolf', 103],
  4091: [1133, 'Kobold', 107],
  4167: [1255, 'Nereid', 98],
  4124: [1148, 'Medusa', 102],
  4166: [1379, 'Nightmare Terror', 107],
  4158: [1384, 'Deleter', 105],
  4432: [1836, 'Magmaring', 110],
  4369: [1675, 'Venatu', 77],
  4404: [1752, 'Skogul', 126],
  4405: [1753, 'Frus', 128],
  300001: [20592, 'Poisonous', 188],
  300005: [20603, 'Abyssman', 190],
  300006: [20598, 'Jewelry Ant', 191],
  300004: [20594, 'Green Mineral', 190],
  27347: [20377, 'Rigid Kaho', 173],
  27352: [20373, 'Rigid Nightmare Terror', 179],
  27354: [20367, 'Contaminated Raydric', 185],
  27356: [20369, 'Frozen Gargoyle', 186],
  4411: [1771, 'Vanberk', 123],
  4412: [1772, 'Isilla', 124],
  4380: [1714, 'Ferus', 126],
  4378: [1713, 'Acidus', 130],
  4448: [1992, 'Cornus', 120],
  4469: [1993, 'Naga', 117],
  4442: [1986, 'Tatacho', 128],
  4447: [1987, 'Centipede', 125],
  4640: [3442, 'Frozen Wolf', 140],
  4638: [3444, 'Watcher', 145],
  300211: [21295, 'Ash Toad', 179],
  300215: [21299, 'Volcaring', 185],
  27085: [3760, 'Resentful Munak', 110],
  27102: [3750, 'Matt Drainliar', 137],
  27101: [3754, 'Sweet Nightmare', 141],
  27110: [3792, 'Angry Gazeti', 126],
  27115: [3800, 'Ominous Permeter', 157],
  300140: [20801, 'Deep Sea Sropho', 147],
  300148: [20808, 'Deep Sea Strouf', 201],
  300106: [20649, 'Red Pitaya', 162],
  300240: [20935, 'Gan Ceann', 215],
  300241: [20937, 'Brutal Murderer', 214],
  300242: [20938, 'Ghost Cube', 213],
  300243: [20939, 'Lude Gal', 213],
  300244: [20936, 'Disguiser', 254],
  300246: [20941, 'Grote', 253],
  300247: [20942, 'Pierrotzoist', 255],
  300245: [20940, 'Blue Moon Loli Ruri', 255],
  300249: [20929, 'Giant Caput', 213],
  300250: [20930, 'Dolorian', 214],
  300252: [20932, 'Deadre', 214],
  300251: [20931, 'Plagarion', 215],
  300253: [20933, 'Venedi', 213],
  300254: [20924, 'Amitera', 227],
  300255: [20925, 'Litus', 228],
  300256: [20926, 'Fillia', 229],
  300257: [20927, 'Vanilaqus', 230],
  300258: [20920, 'Lavaeter', 243],
  300259: [20921, 'Fulgor', 244],
  300260: [20922, 'Napeo', 244],
  300261: [20923, 'Galensis', 244],
  300146: [20806, 'Deep Sea Sedora', 199],
  300147: [20807, 'Deep Sea Swordfish', 199],
  300149: [20809, 'Deep Sea Phen', 199],
  300150: [20810, 'Deep Sea King Dramoh', 205],
  27355: [20368, 'Contaminated Raydric Ar', 184],
  27357: [20370, 'Contaminated Sting', 180],
  27358: [20371, 'Prison Breaker', 186],
  27359: [20379, 'Ice Ghost', 189],
  27360: [20380, 'Flame Ghost', 189]
}

const cardEaterCardsBase: CardEaterCardBase[] = [
  {
    itemId: 4659,
    name: 'Eggring Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4663,
    name: 'Leaf Lunatic Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4664,
    name: 'Grass Fabre Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4021,
    name: 'Rocker Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4017,
    name: 'Savage Babe Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4022,
    name: 'Spore Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4037,
    name: 'Snake Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4034,
    name: 'Wormtail Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4031,
    name: 'Peco Peco Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4036,
    name: 'Muka Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4007,
    name: 'Peco Peco Egg Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4006,
    name: 'Lunatic Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4009,
    name: 'Chonchon Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4030,
    name: 'Mandragora Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4040,
    name: 'Creamy Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4014,
    name: 'Roda Frog Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4032,
    name: 'Ambernite Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4020,
    name: 'Familiar Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4025,
    name: 'Skeleton Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4033,
    name: 'Poporing Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4013,
    name: 'Andre Egg Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4074,
    name: 'Bigfoot Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4039,
    name: 'Stainer Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4063,
    name: 'Caramel Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4048,
    name: 'Poison Spore Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4044,
    name: 'Smokie Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4052,
    name: 'Elder Willow Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4041,
    name: 'Coco Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4045,
    name: 'Horn Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4066,
    name: 'Orc Warrior Card',
    tier: 1,
    levelBand: '1–50',
    silvervineReward: 2,
    eventStoneCoinReward: 10
  },
  {
    itemId: 4104,
    name: 'Requiem Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4064,
    name: 'Zerom Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4117,
    name: 'Sidewinder Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4079,
    name: 'Mantis Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4114,
    name: 'Argiope Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4120,
    name: 'Sky Petite Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4118,
    name: 'Earth Petite Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 27122,
    name: 'Curupira Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 27123,
    name: 'Toucan Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 27124,
    name: 'Jaguar Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4325,
    name: 'Harpy Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4150,
    name: 'Goat Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4161,
    name: 'Grand Peco Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4228,
    name: 'Sleeper Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4299,
    name: 'Clock Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4313,
    name: 'Punk Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4185,
    name: 'Rideword Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4268,
    name: 'Injustice Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4194,
    name: 'Rybio Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4222,
    name: 'Skeleton Prisoner Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4171,
    name: 'Dark Priest Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4422,
    name: 'Roween Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4082,
    name: 'Desert Wolf Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4091,
    name: 'Kobold Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4167,
    name: 'Nereid Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4124,
    name: 'Medusa Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4166,
    name: 'Nightmare Terror Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4158,
    name: 'Sky Deleter Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4432,
    name: 'Magmaring Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4369,
    name: 'Venatu Card',
    tier: 2,
    levelBand: '51–99',
    silvervineReward: 8,
    eventStoneCoinReward: 30
  },
  {
    itemId: 4404,
    name: 'Skogul Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4405,
    name: 'Frus Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300001,
    name: 'Poisonous Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300005,
    name: 'Abyssman Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300006,
    name: 'Jewelry Ant Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300004,
    name: 'Neo Mineral Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27347,
    name: 'Rigid Kaho Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27352,
    name: 'Rigid Nightmare Terror Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27354,
    name: 'Polluted Raydric Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27356,
    name: 'Frozen Gargoyle Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4411,
    name: 'Vanberk Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4412,
    name: 'Isilla Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4380,
    name: 'Red Ferus Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4378,
    name: 'Gold Acidus Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4448,
    name: 'Cornus Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4469,
    name: 'Naga Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4442,
    name: 'Tatacho Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4447,
    name: 'Centipede Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4640,
    name: 'Frozen Wolf Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 4638,
    name: 'Watcher Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300211,
    name: 'Ash Toad Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300215,
    name: 'Volcaring Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27085,
    name: 'Resentful Munak Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27102,
    name: 'Matt Drainliar Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27101,
    name: 'Sweet Nightmare Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27110,
    name: 'Furious Gazeti Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 27115,
    name: 'Ominous Permeter Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300140,
    name: 'Abysmal Sropho Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300148,
    name: 'Abysmal Strouf Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300106,
    name: 'Red Pitaya Card',
    tier: 3,
    levelBand: '100–199',
    silvervineReward: 14,
    eventStoneCoinReward: 50
  },
  {
    itemId: 300240,
    name: 'Gan Ceann Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300241,
    name: 'Brutal Murderer Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300242,
    name: 'Ghost Cube Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300243,
    name: 'Lude Gal Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300244,
    name: 'Disguiser Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300246,
    name: 'Grote Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300247,
    name: 'Pierrotzoist Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300245,
    name: 'Blue Moon Loli Ruri Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300249,
    name: 'Giant Caput Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300250,
    name: 'Dolorian Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300252,
    name: 'Deadre Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300251,
    name: 'Plagarion Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300253,
    name: 'Venedi Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300254,
    name: 'Amitera Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300255,
    name: 'Litus Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300256,
    name: 'Fillia Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300257,
    name: 'Vanilaqus Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300258,
    name: 'Lavaeter Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300259,
    name: 'Fulgor Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300260,
    name: 'Napeo Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300261,
    name: 'Galensis Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300146,
    name: 'Abysmal Sedora Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300147,
    name: 'Abysmal Swordfish Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300149,
    name: 'Abysmal Phen Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 300150,
    name: 'Abysmal King Dramoh Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 27355,
    name: 'Polluted Raydric Archer Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 27357,
    name: 'Polluted Sting Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 27358,
    name: 'Prison Breaker Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 27359,
    name: 'Ice Ghost Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  },
  {
    itemId: 27360,
    name: 'Flame Ghost Card',
    tier: 4,
    levelBand: '200+',
    silvervineReward: 30,
    eventStoneCoinReward: 90
  }
]

const cardEaterCards: CardEaterCard[] = cardEaterCardsBase.map((card) => {
  const sourceMob = sourceMobs[card.itemId]

  if (!sourceMob) {
    throw new Error(`Missing Card Eater source monster for item ${card.itemId}`)
  }

  const [sourceMobId, monsterName, monsterLevel] = sourceMob

  return { ...card, sourceMobId, monsterName, monsterLevel }
})

export { cardEaterCards, type CardEaterCard }
