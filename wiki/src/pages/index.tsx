import type { ReactNode } from 'react'

import {
  CatalogStrip,
  ContactCard,
  ExchangeCard,
  GuideSection,
  ItemCard,
  LoopCard,
  PriceCard,
  ServiceCard
} from '../components'
import {
  blackMarketContacts,
  cardPriceBands,
  cashPointRules,
  featuredItems,
  harlanCatalogs,
  rookeShelves
} from '../data/blackMarket'

export default function Home(): ReactNode {
  return (
    <main className="market-page" aria-label="Black Market player guide">
      <header className="market-hero">
        <div className="market-hero__eyebrow">RAGNAROK OFFLINE · PLAYER GUIDE</div>
        <h1>The Black Market</h1>
        <p className="market-hero__lead">
          A quiet network for turning cards, Zeny, and hard-won loot into useful tools, upgrades,
          and style.
        </p>
        <nav className="market-jump" aria-label="Black Market guide sections">
          <a href="#enter">Enter</a>
          <a href="#earn">Earn Cash Points</a>
          <a href="#spend">Spend Cash Points</a>
          <a href="#featured">Featured Items</a>
        </nav>
      </header>

      <div className="market-content">
        <GuideSection
          id="enter"
          titleId="enter-title"
          number="01"
          kicker="Find the quiet door"
          title="Enter the Black Market"
        >
          <div className="section-intro two-column">
            <p>
              Speak to any <strong>Black Market Contact</strong> in the world. Entry is free for
              now, and the Contact sends you directly to the market hall.
            </p>
            <p>
              When you are done, use the Contact inside the hall to leave. You return to the most
              recently used Contact, so you do not have to retrace your route.
            </p>
          </div>
          <div className="contact-list" aria-label="Black Market Contact locations">
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
          <p className="callout">
            The Card Collector buys standalone cards from your inventory. Cards already placed into
            equipment are not part of the sale.
          </p>
          <div className="card-price-grid">
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
          <div className="service-grid">
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
          <div className="notice">
            <strong>Small sample, not the full inventory.</strong> These examples show the range of
            the market. Check the NPC shop in-game for the current complete selection.
          </div>
          <div className="item-grid">
            {featuredItems.map((item) => (
              <ItemCard item={item} key={item.itemId} />
            ))}
          </div>
        </GuideSection>
      </div>
    </main>
  )
}
