import type { ReactNode } from 'react'

import { Callout } from './components/callout'
import { CatalogStrip } from './components/catalog-strip'
import { ContactCard } from './components/contact-card'
import { ExchangeCard } from './components/exchange-card'
import { GuideSection } from './components/guide-section'
import { ItemCard } from './components/item-card'
import { LoopCard } from './components/loop-card'
import { MarketHero } from './components/market-hero'
import { PriceCard } from './components/price-card'
import { ServiceCard } from './components/service-card'
import { Text } from './components/text'
import {
  blackMarketContacts,
  cardPriceBands,
  cashPointRules,
  featuredItems,
  harlanCatalogs,
  rookeShelves
} from './data/black-market'
import { contactList, grids, page } from './styles/recipes'

export default function BlackMarketPage(): ReactNode {
  const pageStyles = page()
  const gridStyles = grids()

  return (
    <main className={pageStyles.root} aria-label="Black Market player guide">
      <MarketHero />

      <div className={pageStyles.content}>
        <GuideSection
          id="enter"
          titleId="enter-title"
          number="01"
          kicker="Find the quiet door"
          title="Enter the Black Market"
        >
          <div className={pageStyles.sectionIntro}>
            <Text as="p" tone="muted">
              Speak to any <strong>Black Market Contact</strong> in the world. Entry is free for
              now, and the Contact sends you directly to the market hall.
            </Text>
            <Text as="p" tone="muted">
              When you are done, use the Contact inside the hall to leave. You return to the most
              recently used Contact, so you do not have to retrace your route.
            </Text>
          </div>
          <div className={contactList()} aria-label="Black Market Contact locations">
            {blackMarketContacts.map((contact) => (
              <ContactCard contact={contact} key={contact.city} />
            ))}
          </div>
        </GuideSection>

        <GuideSection
          id="earn"
          titleId="earn-title"
          number="02"
          kicker="Turn your drops into options"
          title="Earn Cash Points"
        >
          <LoopCard />
          <Callout>
            The Card Collector buys standalone cards from your inventory. Cards already placed into
            equipment are not part of the sale.
          </Callout>
          <div className={gridStyles.price}>
            {cardPriceBands.map((band) => (
              <PriceCard band={band} key={`${band.catalog}-${band.source}`} />
            ))}
          </div>
          <ExchangeCard rules={cashPointRules} />
        </GuideSection>

        <GuideSection
          id="spend"
          titleId="spend-title"
          number="03"
          kicker="Useful things, carefully curated"
          title="Spend Cash Points"
        >
          <div className={gridStyles.service}>
            <ServiceCard
              featured
              icon="◆"
              location="Market hall · 41, 159"
              title="Black Market Boss"
              description="Sell accepted monster cards and check your Cash Point balance and loyalty progress."
              tag="Card exchange"
            />
            <ServiceCard
              icon="◎"
              location="Market hall · 26, 181"
              title="Coin Exchange"
              description="Convert Zeny into Cash Points, cash points back into Zeny, or try the optional gacha."
              tag="Currency"
            />
            <ServiceCard
              icon="✦"
              location="Market hall · 39, 185"
              title="Rooke"
              description="Browse 94 curated consumables across seven shelves, from EXP and recovery to upgrades and style."
              tag="94 items · CP"
            />
            <ServiceCard
              icon="◇"
              location="Market hall · 41, 173"
              title="Harlan"
              description="Browse equipment, rentals, specialty circlets, and event headgear. Every hat costs 100 CP."
              tag="495 hats · 100 CP"
            />
            <ServiceCard
              icon="▣"
              location="Market hall · 48, 161"
              title="Kafra Storage"
              description="Store and retrieve your items inside the market. This service is Storage-only."
              tag="Storage"
            />
          </div>
          <CatalogStrip rookeShelves={rookeShelves} harlanCatalogs={harlanCatalogs} />
        </GuideSection>

        <GuideSection
          id="featured"
          titleId="featured-title"
          number="04"
          kicker="A few things worth knowing"
          title="Featured Items"
        >
          <Callout notice>
            <strong>Small sample, not the full inventory.</strong> These examples show the range of
            the market. Check the NPC shop in-game for the current complete selection.
          </Callout>
          <div className={gridStyles.item}>
            {featuredItems.map((item) => (
              <ItemCard item={item} key={item.itemId} />
            ))}
          </div>
        </GuideSection>
      </div>
    </main>
  )
}
