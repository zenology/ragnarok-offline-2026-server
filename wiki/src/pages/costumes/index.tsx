import type { ReactNode } from 'react'

import { Link } from '@tanstack/react-router'

import { Callout, ServiceSummaryCard } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { acquisitionServices, enhancementServices, stoneServices } from './data/costumes'
import { costumesPage } from './styles/recipes'

export default function CostumePage(): ReactNode {
  const styles = costumesPage()

  return (
    <main className={styles.root} aria-label="Malangdo costume player guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Costumes in Malangdo"
        description="A complete route through Malangdo's costume acquisition, Fashion Stone, and enhancement services."
      />
      <div className={styles.content}>
        <GuideSection
          id="wardrobe"
          number="01"
          eyebrow="Start with a look"
          title="Build your wardrobe"
        >
          <div className={styles.grid}>
            {acquisitionServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service}>
                {service.name === 'Moth' && <Link to="/card-eater">Search accepted cards →</Link>}
              </ServiceSummaryCard>
            ))}
          </div>
          <Callout variant="notice">
            Equipment charms and Random Options are a different corner of mal_in01.{' '}
            <Link to="/enchantment">Open the Enchantment guide →</Link>
          </Callout>
        </GuideSection>

        <GuideSection
          id="stones"
          number="02"
          eyebrow="Add the finishing touch"
          title="Buy Fashion Stones"
        >
          <div className={styles.grid}>
            {stoneServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service} />
            ))}
          </div>
        </GuideSection>

        <GuideSection
          id="enhance"
          number="03"
          eyebrow="Inside the costume room"
          title="Costume services inside mal_in01"
        >
          <div className={styles.grid}>
            {enhancementServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service} />
            ))}
          </div>
          <Callout variant="notice">
            Every enhancement attempt has a 75% success chance. A failed attempt consumes the
            attempted stone and clears only that enchant slot; the costume, refine level, and other
            enchant slots remain safe.
          </Callout>
        </GuideSection>
      </div>
    </main>
  )
}
