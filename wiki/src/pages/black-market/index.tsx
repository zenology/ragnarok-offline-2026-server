import type { ReactNode } from 'react'

import {
  faBoxArchive,
  faCoins,
  faGem,
  faHatWizard,
  faRing,
  faShieldHalved,
  faShirt,
  faShoePrints,
  faStore,
  faUserSecret,
  faWandMagic
} from '@fortawesome/free-solid-svg-icons'
import { Link } from '@tanstack/react-router'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { CatalogStrip } from './components/catalog-strip'
import { ContactCard } from './components/contact-card'
import { ExchangeCard } from './components/exchange-card'
import { ItemCard } from './components/item-card'
import { JumpNavigation } from './components/jump-navigation'
import { LoopCard } from './components/loop-card'
import { ServiceCard } from './components/service-card'
import {
  blackMarketContacts,
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
          title="Earn Black Market Points"
        >
          <LoopCard />
          <Callout>
            <strong>Treasure Collector (Black Market Boss)</strong> ที่ paramk 41, 159
            รับกล่องสมบัติ และการ์ดที่ร้าน Black Market มีขาย
            โดยรับการ์ดในกระเป๋าที่ไม่ได้ใส่ในอุปกรณ์ ทั้ง Normal, Miniboss, MVP และ Special
            การ์ดที่ไม่มีขายในร้านจะไม่อยู่ในรายการรับซื้อ
          </Callout>
          <Callout variant="notice">
            <strong>รับซื้อการ์ดที่ 50% ของราคาขายในร้านขณะนั้น</strong> คิดหลัง Daily Rate
            และปัดเศษ BP ลงต่อใบ เช่น ร้านขาย 19,129 BP จะรับซื้อ 9,564 BP ต่อใบ
            ราคาการ์ดไม่คูณตามเลเวลหรือการเกิดใหม่ ส่วนราคากล่องสมบัติยังเป็นราคาเดิม
          </Callout>
          <Callout>
            เลือก Sell one card type. เพื่อระบุชนิดและจำนวน หรือ Sell every accepted card.
            เพื่อขายการ์ดที่รับซื้อทั้งหมด ตรวจรายการและยอด BP ก่อนกด Sell the cards.
            หากเปลี่ยนใจเลือก Keep them. การขายการ์ดไม่เพิ่มแต้มสะสมกล่องและไม่มีรางวัลสะสม
            เมนูขายกล่องแยกจากการ์ด หากราคาหรือจำนวนเปลี่ยนระหว่างยืนยัน ระบบจะยกเลิกดีลให้เริ่มใหม่
          </Callout>
          <ExchangeCard rules={cashPointRules} />
        </GuideSection>

        <GuideSection
          id="spend"
          number="03"
          eyebrow="Useful things, carefully curated"
          title="Spend Black Market Points"
        >
          <div className={gridStyles.service}>
            <ServiceCard
              featured
              icon={faStore}
              location="Market hall · 41, 159"
              title="Black Market Boss"
              description="รับกล่องสมบัติราคาเดิม และรับการ์ดที่ร้านมีขายในราคา 50% หลัง Daily Rate ปัดเศษลงต่อใบ การ์ดไม่เพิ่มแต้มสะสมกล่อง"
              tag="Treasure Collector"
            />
            <ServiceCard
              icon={faCoins}
              location="Market hall · 26, 181"
              title="Coin Exchange"
              description="Convert Zeny into Black Market Points, points back into Zeny, or try the optional gacha."
              tag="Currency"
            />
            <ServiceCard
              icon={faGem}
              location="Market hall · 39, 185"
              title="Rooke"
              description="Browse 100 curated consumables across seven shelves. The Upgrade shelf also sells Etel Dust, Shadowdecon, and Zelunium from Base Level 100."
              tag="100 items · BP"
            />
            <ServiceCard
              icon={faHatWizard}
              location="Market hall · 41, 173"
              title="Harlan"
              description="Browse 532 headgear items in the Equipment catalog, organized by slot and display name."
              tag="532 equipment · BP"
            />
            <ServiceCard
              icon={faGem}
              location="Market hall · 27, 158"
              title="Madame Celestine"
              description="Browse the complete Specialty Broker catalog: 1,631 equipment items across 90 named endgame families."
              tag="1,631 specialty items · BP"
            />
            <ServiceCard
              icon={faUserSecret}
              location="Market hall · 24, 172"
              title="Riven"
              description="Search 3,438 Black Market equipment items by name or exact Item ID, then open the matching seller drawer."
              tag="Equipment Search"
            />
            <ServiceCard
              icon={faWandMagic}
              location="Market hall · 51, 175"
              title="Soren"
              description="Browse weapons by type in the Equipment catalog. Specialty families are now handled by Madame Celestine."
              tag="719 weapons · BP"
            />
            <ServiceCard
              icon={faShieldHalved}
              location="Market hall · 37, 163"
              title="Mordain"
              description="Browse body armor in the Equipment catalog. Specialty families are now handled by Madame Celestine."
              tag="319 armor · BP"
            />
            <ServiceCard
              icon={faShieldHalved}
              location="Market hall · 33, 166"
              title="Kaedra"
              description="Recover shields through the Equipment catalog, organized for quick browsing. Specialty families are now handled by Madame Celestine."
              tag="Shield recovery · BP"
            />
            <ServiceCard
              icon={faShoePrints}
              location="Market hall · 23, 185"
              title="Tess"
              description="Browse footgear recovery stock in the Equipment catalog with complete client-asset coverage. Specialty families are now handled by Madame Celestine."
              tag="Footgear · BP"
            />
            <ServiceCard
              icon={faShirt}
              location="Market hall · 24, 160"
              title="Weaver"
              description="Browse garment recovery equipment with imported executable effects and verified client assets. Specialty families are now handled by Madame Celestine."
              tag="326 garments · BP"
            />
            <ServiceCard
              icon={faRing}
              location="Market hall · 23, 160"
              title="Lady Seraphine Vellmont"
              description="Browse accessory recovery stock by letter, including imported combo partners. Specialty families are now handled by Madame Celestine."
              tag="Accessories · BP"
            />
            <ServiceCard
              icon={faUserSecret}
              location="Market hall · 47–51, 186"
              title="Card Sellers"
              description="ซื้อการ์ดด้วย Black Market Points ที่ร้านการ์ด หากต้องการขายการ์ดคืน 50% ให้ไปหา Treasure Collector"
              tag="Cards · BP"
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
            effects, useful casting bonuses, or powerful combinations cost more Black Market Points.
            Requirements such as refinement, level, and grading are considered too, but the effect
            itself remains the main signal. A higher price can point you toward a valuable hat for
            your build, but always check the item description before you buy.
          </Callout>
          <CatalogStrip rookeShelves={rookeShelves} harlanCatalogs={harlanCatalogs} />
          <Callout variant="notice">
            Malangdo stones on Rooke. Etel Dust, Shadowdecon, and Zelunium are 150 Black Market
            Points at Base Level 100–200 and 300 at 201+. They are not sold below 100. Bramble uses
            Shadowdecon, Doberkin uses Etel Dust, and Stripe uses Zelunium. Special and exact-choice
            services also require Blacksmith Blessing.{' '}
            <Link to="/enchantment">Open the Enchantment guide →</Link>
          </Callout>
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
