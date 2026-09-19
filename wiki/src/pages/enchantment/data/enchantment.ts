import type { ServiceSummary } from '@/components/molecules'

export const npcServices: ServiceSummary[] = [
  {
    name: 'Bramble',
    location: 'mal_in01 · 11, 176',
    tag: 'Silvervine Enchantment',
    description:
      'Wear registered charm gear with an open card-slot step. He fills unused enchant sockets. Random Options are Stripe\u2019s job.'
  },
  {
    name: 'Stripe',
    location: 'mal_in01 · 23, 176',
    tag: 'Random Option Enchant',
    description:
      'Wear registered Random Option gear. He fills one official option slot at a time, in order, and leaves later options in place.'
  },
  {
    name: 'Doberkin',
    location: 'mal_in01 · 25, 167',
    tag: 'Enchantment Stone Upgrade',
    description:
      'Wear a numbered charm Bramble already placed. He tries to push that same charm stronger. It can rise, stay, or drop. The piece is not destroyed.'
  }
]

export const costServices: ServiceSummary[] = [
  {
    name: 'Bramble · Spin',
    location: 'First fill or reroll',
    tag: 'Silvervine Fruit',
    description:
      'A new step costs that step\u2019s number in Silvervine Fruit. Step one costs 1. A reroll of a filled step costs one extra Fruit.'
  },
  {
    name: 'Bramble · Choose',
    location: 'Exact charm',
    tag: '5 Fruit + Shadowdecon',
    description:
      'Always 5 Silvervine Fruit and 1 Shadowdecon. Cancel or missing materials spend nothing.'
  },
  {
    name: 'Stripe · Options',
    location: 'Same price to fill or reroll',
    tag: 'Fruit + Zelunium',
    description:
      'Slot 1: 5 Fruit. Slot 2: 7 Fruit. Slot 3: 10 Fruit. Slot 4: 5 Fruit and 1 Zelunium. Slot 5: 5 Fruit and 2 Zelunium.'
  },
  {
    name: 'Doberkin · Push',
    location: 'Every try',
    tag: '5 Fruit + Etel Dust',
    description:
      'Always 5 Silvervine Fruit and 1 Etel Dust. Missing Etel Dust is called out first, then Fruit. Nothing is taken until the charm is rewritten.'
  }
]

export const materialServices: ServiceSummary[] = [
  {
    name: 'Card Eater',
    location: 'Malangdo · 138, 140',
    tag: 'Silvervine Fruit',
    description:
      'Feed the listed Normal cards for Silvervine Fruit. Fruit is shared with Costume Roulette and these enchant NPCs.'
  },
  {
    name: 'Rooke · Upgrade',
    location: 'Black Market hall · 39, 185',
    tag: '150 / 300 CP',
    description:
      'Etel Dust, Shadowdecon, and Zelunium appear from Base Level 100. All jobs 100–200 pay 150 Cash Points. 201+ pay 300. They are not sold below 100.'
  }
]
