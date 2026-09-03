import type { ReactNode } from 'react'

import {
  faBoxArchive,
  faCoins,
  faGem,
  faHatWizard,
  faStore,
  faWandMagic
} from '@fortawesome/free-solid-svg-icons'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { CatalogStrip } from './components/catalog-strip'
import { ContactCard } from './components/contact-card'
import { ExchangeCard } from './components/exchange-card'
import { ItemCard } from './components/item-card'
import { JumpNavigation } from './components/jump-navigation'
import { LoopCard } from './components/loop-card'
import { PriceCard } from './components/price-card'
import { ServiceCard } from './components/service-card'
import {
  blackMarketContacts,
  cardPriceBands,
  cashPointRules,
  featuredItems,
  harlanCatalogs,
  rookeShelves
} from './data/black-market'
import { blackMarketPage, contactList, grids } from './styles/recipes'

export default function BlackMarketPage(): ReactNode {
  const pageStyles = blackMarketPage()
  const gridStyles = grids()

  return (
    <main className={pageStyles.root} aria-label="Black Market player guide">
      <HeaderSection
        image="../../src/assets/black-market/header.png"
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="The Black Market"
        description="A quiet network for turning cards, Zeny, and hard-won loot into useful tools, upgrades, and style."
      >
        <JumpNavigation />
      </HeaderSection>

      <div className={pageStyles.content}>
        <GuideSection
          id="enter"
          number="01"
          eyebrow="Find the quiet door"
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
          number="02"
          eyebrow="Turn your drops into options"
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
          number="03"
          eyebrow="Useful things, carefully curated"
          title="Spend Cash Points"
        >
          <div className={gridStyles.service}>
            <ServiceCard
              featured
              icon={faStore}
              location="Market hall · 41, 159"
              title="Black Market Boss"
              description="Sell accepted monster cards and check your Cash Point balance and loyalty progress."
              tag="Card exchange"
            />
            <ServiceCard
              icon={faCoins}
              location="Market hall · 26, 181"
              title="Coin Exchange"
              description="Convert Zeny into Cash Points, cash points back into Zeny, or try the optional gacha."
              tag="Currency"
            />
            <ServiceCard
              icon={faGem}
              location="Market hall · 39, 185"
              title="Rooke"
              description="Browse 94 curated consumables across seven shelves, from EXP and recovery to upgrades and style."
              tag="94 items · CP"
            />
            <ServiceCard
              icon={faHatWizard}
              location="Market hall · 41, 173"
              title="Harlan"
              description="Browse equipment, rentals, specialty circlets, and event headgear. Specialty opens by endgame set, including Temporal, Helm of Faith, Glacier, Thanos, and Rune Crowns."
              tag="613 hats · priced by power"
            />
            <ServiceCard
              icon={faWandMagic}
              location="Market hall · 51, 175"
              title="Soren"
              description="Browse 745 weapons by type. Specialty drawers use the weapon family name, so you can go straight to a set such as EDDA Biolab, Illusion, Frontier, Glacier, or Fides."
              tag="745 weapons · CP"
            />
            <ServiceCard
              icon={faBoxArchive}
              location="Market hall · 48, 161"
              title="Kafra Storage"
              description="Store and retrieve your items inside the market. This service is Storage-only."
              tag="Storage"
            />
          </div>
          <Callout variant="notice">
            <strong>Harlan&apos;s prices are a useful hint.</strong> Hats with stronger combat
            effects, useful casting bonuses, or powerful combinations cost more Cash Points.
            Requirements such as refinement, level, and grading are considered too, but the effect
            itself remains the main signal. A higher price can point you toward a valuable hat for
            your build, but always check the item description before you buy.
          </Callout>
          <CatalogStrip rookeShelves={rookeShelves} harlanCatalogs={harlanCatalogs} />
        </GuideSection>

        <GuideSection
          id="featured"
          number="04"
          eyebrow="A few things worth knowing"
          title="Featured Items"
        >
          <Callout variant="notice">
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
