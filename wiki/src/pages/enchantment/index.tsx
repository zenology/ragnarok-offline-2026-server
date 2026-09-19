import type { ReactNode } from 'react'

import { Link } from '@tanstack/react-router'

import { Callout, ServiceSummaryCard } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { costServices, materialServices, npcServices } from './data/enchantment'
import { enchantmentPage } from './styles/recipes'

export default function EnchantmentPage(): ReactNode {
  const styles = enchantmentPage()

  return (
    <main className={styles.root} aria-label="Malangdo enchantment player guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Enchantment in Malangdo"
        description="Fill charms with Bramble, roll official Random Options with Stripe, and push numbered charms with Doberkin."
      />
      <div className={styles.content}>
        <GuideSection
          id="meet"
          number="01"
          eyebrow="Meet the three"
          title="Bramble, Stripe, and Doberkin"
        >
          <div className={styles.grid}>
            {npcServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service} />
            ))}
          </div>
          <Callout variant="notice">
            Costume stones in the same building are a different service.{' '}
            <Link to="/costumes">costume services</Link>
          </Callout>
        </GuideSection>

        <GuideSection
          id="costs"
          number="02"
          eyebrow="Pay the listed cost"
          title="What each attempt costs"
        >
          <div className={styles.grid}>
            {costServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service} />
            ))}
          </div>
        </GuideSection>

        <GuideSection
          id="materials"
          number="03"
          eyebrow="Gather the materials"
          title="Fruit and Rooke's stones"
        >
          <div className={styles.grid}>
            {materialServices.map((service) => (
              <ServiceSummaryCard key={service.name} service={service}>
                {service.name === 'Card Eater' && (
                  <Link to="/card-eater">Search accepted cards →</Link>
                )}
                {service.name === 'Rooke · Upgrade' && (
                  <Link to="/black-market">Open the Black Market guide →</Link>
                )}
              </ServiceSummaryCard>
            ))}
          </div>
          <Callout variant="notice">
            Keep Stripe&apos;s work away from Vesper. Vesper in the Black Market hall (paramk 43,
            181) shakes a generic Random Option bag and can overwrite Stripe&apos;s official
            options.
          </Callout>
        </GuideSection>
      </div>
    </main>
  )
}
