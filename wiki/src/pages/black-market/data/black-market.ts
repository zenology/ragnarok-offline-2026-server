import dimensionEntrance from '@/assets/warp/dimension-entrance.png'
import dimension from '@/assets/warp/dimension.png'
import geffenEntrance from '@/assets/warp/geffen-entrance.png'
import geffen from '@/assets/warp/geffen.png'
import glastheimMap from '@/assets/warp/glastheim-map.png'
import glastheim from '@/assets/warp/glastheim.png'
import hugelEntrance from '@/assets/warp/hugel-entrance.png'
import hugel from '@/assets/warp/hugel.png'
import lasagnaMap from '@/assets/warp/lasagna-map.png'
import lasagna from '@/assets/warp/lasagna.png'
import lighthalzenMap from '@/assets/warp/lighthalzen-map.png'
import lighthalzen from '@/assets/warp/lighthalzen.png'
import moroccMap from '@/assets/warp/morocc-map.png'
import morocc from '@/assets/warp/morocc.png'
import payonEntrance from '@/assets/warp/payon-entrance.png'
import payon from '@/assets/warp/payon.png'
import pronteraMap from '@/assets/warp/prontera-map.png'
import prontera from '@/assets/warp/prontera.png'
import rachelMap from '@/assets/warp/rachel-map.png'
import rachel from '@/assets/warp/rachel.png'
import yunoMap from '@/assets/warp/yuno-map.png'
import yuno from '@/assets/warp/yuno.png'
import type { ImageViewerImage } from '@/components/molecules'

function locationImages(
  city: string,
  location: string,
  route: string,
  routeLabel: string
): readonly ImageViewerImage[] {
  return [
    { src: location, alt: `Black Market Contact in ${city}`, label: 'Contact location' },
    { src: route, alt: `${city} Contact route reference`, label: routeLabel }
  ]
}

export type BlackMarketContact = {
  city: string
  location: string
  coordinates: string
  images: readonly ImageViewerImage[]
}

export type CardPriceBand = {
  catalog: string
  source: string
  range: string
}

export type FeaturedItem = {
  name: string
  itemId: number
  price: number
  category: string
  divinePrideUrl: string
}

export const blackMarketContacts: BlackMarketContact[] = [
  {
    city: 'Prontera',
    location: 'South gate area',
    coordinates: '195, 56',
    images: locationImages('Prontera', prontera, pronteraMap, 'Navigation map')
  },
  {
    city: 'Morocc',
    location: 'Town market',
    coordinates: '38, 153',
    images: locationImages('Morocc', morocc, moroccMap, 'Navigation map')
  },
  {
    city: 'Payon',
    location: 'Payon Inn',
    coordinates: '182, 25',
    images: locationImages('Payon', payon, payonEntrance, 'Route landmark')
  },
  {
    city: 'Geffen',
    location: 'Geffen Tower',
    coordinates: '163, 173',
    images: locationImages('Geffen', geffen, geffenEntrance, 'Route landmark')
  },
  {
    city: 'Yuno',
    location: 'Town street',
    coordinates: '117, 126',
    images: locationImages('Yuno', yuno, yunoMap, 'Navigation map')
  },
  {
    city: 'Lighthalzen',
    location: 'Town street',
    coordinates: '320, 270',
    images: locationImages('Lighthalzen', lighthalzen, lighthalzenMap, 'Navigation map')
  },
  {
    city: 'Hugel',
    location: 'Hugel Inn',
    coordinates: '219, 385',
    images: locationImages('Hugel', hugel, hugelEntrance, 'Route landmark')
  },
  {
    city: 'Rachel',
    location: 'Town street',
    coordinates: '175, 201',
    images: locationImages('Rachel', rachel, rachelMap, 'Navigation map')
  },
  {
    city: 'Lasagna',
    location: 'Town street',
    coordinates: '160, 307',
    images: locationImages('Lasagna', lasagna, lasagnaMap, 'Navigation map')
  },
  {
    city: 'Glast Heim',
    location: 'Ruins',
    coordinates: '192, 292',
    images: locationImages('Glast Heim', glastheim, glastheimMap, 'Navigation map')
  },
  {
    city: 'Dimensional Rift',
    location: 'Rift entrance',
    coordinates: '80, 113',
    images: locationImages('Dimensional Rift', dimension, dimensionEntrance, 'Route landmark')
  }
]

export const cashPointRules = {
  buyRate: '1,000 Zeny = 1 Cash Point',
  cashOutRate: '1 Cash Point = 750 Zeny',
  bonus: 'Buy 10 Cash Points in one transaction and receive 1 bonus point.'
}

export const cardPriceBands: CardPriceBand[] = [
  { catalog: 'Normal card', source: 'Monster Lv. 1–50', range: '250–1,000 CP' },
  { catalog: 'Normal card', source: 'Monster Lv. 51–100', range: '2,000–4,000 CP' },
  { catalog: 'Normal card', source: 'Monster Lv. 101+', range: '7,000–15,000 CP' },
  { catalog: 'Miniboss card', source: 'Source monster', range: '7,000–20,000 CP' },
  { catalog: 'MVP card', source: 'Source monster', range: '20,000–100,000 CP' }
]

export const rookeShelves = [
  'Premium Service',
  'EXP and Drop',
  'Combat Buffs',
  'Recovery',
  'Convenience',
  'Upgrade',
  'Style and Reset'
]

export const harlanCatalogs = [
  { name: 'Equipment', count: 394 },
  { name: 'Rental', count: 19 },
  { name: 'Specialty', count: 37 },
  { name: 'Event', count: 45 }
]

export const featuredItems: FeaturedItem[] = [
  {
    name: 'Nyangvine',
    itemId: 6909,
    price: 10,
    category: 'Premium Service',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/6909/nyangvine'
  },
  {
    name: 'Battle Manual X3',
    itemId: 14545,
    price: 12,
    category: 'EXP and Drop',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/14545/battle-manual-x3'
  },
  {
    name: 'Chile Shrimp Gratin',
    itemId: 12086,
    price: 2,
    category: 'Combat Buffs',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/12086/chile-shrimp-gratin'
  },
  {
    name: 'Small Life Potion',
    itemId: 14534,
    price: 3,
    category: 'Recovery',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/14534/small-life-potion'
  },
  {
    name: 'Greed Scroll 30 Box',
    itemId: 13621,
    price: 15,
    category: 'Convenience',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/13621/greed-scroll-30-box'
  },
  {
    name: 'Blacksmith Blessing',
    itemId: 6635,
    price: 13,
    category: 'Upgrade',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/6635/blacksmith-blessing'
  },
  {
    name: 'Second Costume Ticket',
    itemId: 6959,
    price: 100,
    category: 'Style and Reset',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/6959/second-costume-ticket'
  },
  {
    name: 'Happy Balloon',
    itemId: 15972,
    price: 100,
    category: 'Harlan · Event',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/15972/event-happy-balloon'
  },
  {
    name: 'Issgard Helmet',
    itemId: 401019,
    price: 2100,
    category: 'Harlan · Equipment',
    divinePrideUrl: 'https://www.divine-pride.net/database/item/401019/s2-issgard-helmet'
  }
]
