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
    tag: '25 Fruit + stones + blessing',
    description:
      'Always 25 Silvervine Fruit. Steps 1–3 also cost 5 Shadowdecon; steps 4–5 cost 10. Blacksmith Blessing follows the step: 1, 2, 3, 4, or 5. Cancel or missing materials spend nothing.'
  },
  {
    name: 'Stripe · Ordinary',
    location: 'Roll the option and its value',
    tag: 'Fruit + optional Zelunium',
    description:
      'Slot 1: 5 Fruit. Slot 2: 7 Fruit. Slot 3: 10 Fruit. Slot 4: 5 Fruit and 1 Zelunium. Slot 5: 5 Fruit and 2 Zelunium.'
  },
  {
    name: 'Stripe · Special',
    location: 'Roll the option, force its maximum value',
    tag: 'Fruit + Zelunium + blessing',
    description:
      'Fruit is 5, 7, 10, 5, or 5 by slot. Zelunium is 5 for slots 1–3 and 10 for slots 4–5. Blacksmith Blessing follows the slot: 1 through 5.'
  },
  {
    name: 'Doberkin · Ordinary',
    location: 'The charm can rise, stay, or drop',
    tag: 'Fruit + optional Etel Dust',
    description:
      'Fruit follows the charm socket: 5, 7, 10, 15, or 20. Sockets 1–3 use no Etel Dust; socket 4 uses 1 and socket 5 uses 2. The equipment is never destroyed.'
  },
  {
    name: 'Doberkin · Special',
    location: 'Force the top charm in the series',
    tag: 'Fruit + Etel Dust + blessing',
    description:
      'Fruit is 5, 7, 10, 15, or 20 by charm socket. Etel Dust is 5 for sockets 1–3 and 10 for sockets 4–5. Blacksmith Blessing follows the socket: 1 through 5.'
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
    tag: '150 / 300 BP',
    description:
      'Etel Dust, Shadowdecon, and Zelunium appear from Base Level 100. All jobs 100–200 pay 150 Black Market Points. 201+ pay 300. They are not sold below 100.'
  }
]
