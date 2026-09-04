export type CostumeService = {
  name: string
  location: string
  description: string
  tag: string
}

export const acquisitionServices: CostumeService[] = [
  {
    name: 'Shadow Gear Broker',
    location: 'Malangdo · 112, 162',
    description:
      'Browse Shadow Equipment through Event Stone Coin barter drawers. This is the offline recovery route for class and progression shadow gear.',
    tag: 'Event Stone Coins'
  },
  {
    name: 'Costume Curator',
    location: 'Malangdo · 116, 169',
    description:
      'Browse the Costume Roulette pool by Top, Mid, Low / Mouth, or Garment. Ordinary and Special costumes use Event Stone Coins, with the live price tiers summarized as 10, 20, 30, 40, 50, or 70 coins.',
    tag: 'Event Stone Coins'
  },
  {
    name: 'Costume Roulette',
    location: 'Malangdo · 141, 137',
    description:
      'Choose one costume slot and spend one Silvervine Fruit per spin. Every 20 successful spins grants one Silk Mat.',
    tag: 'Silvervine Fruit'
  },
  {
    name: 'Card Eater',
    location: 'Malangdo · 138, 140',
    description:
      'Feed only the normal cards on the listed menu. The four card-level bands pay 2 / 8 / 14 / 30 Silvervine Fruit normally, or 2 / 6 / 10 / 20 Silvervine Fruit with Extras plus 0 / 2 / 4 / 10 Event Stone Coins.',
    tag: 'Cards → currencies'
  }
]

export const stoneServices: CostumeService[] = [
  {
    name: 'Milo Crumb',
    location: 'Malangdo · 124, 152',
    description:
      'Trade Event Stone Coins for Top, Middle, Lower, Garment, or Dual / Effect Fashion Stones. This is the stone shop, not a direct costume shop.',
    tag: 'Fashion Stones'
  }
]

export const enhancementServices: CostumeService[] = [
  {
    name: 'Designer Heidam',
    location: 'mal_in01 · 20, 124',
    description: 'Exchange an eligible owned costume for its random Costume Enchant Stone Box.',
    tag: 'Costume exchange'
  },
  {
    name: 'Aver De Dosh',
    location: 'mal_in01 · 23, 113',
    description: 'Enchant Costume Headgear with a matching Costume Enchant Stone.',
    tag: 'Headgear enchant'
  },
  {
    name: 'Lace La Zard',
    location: 'mal_in01 · 20, 107',
    description: 'Enchant Costume Garments with a Garment or Dual Enchant Stone.',
    tag: 'Garment enchant'
  },
  {
    name: 'Gregio Grumani',
    location: 'mal_in01 · 24, 121',
    description: 'Apply a Costume Effect Stone to the fourth enchant slot.',
    tag: 'Effect enchant'
  }
]
