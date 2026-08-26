import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'
import { HeaderSection } from '@/components/templates'
import { Link } from '@tanstack/react-router'

import { landingPage } from './landing.recipe'

const guides = [
  {
    to: '/black-market',
    number: '01',
    title: 'The Black Market',
    description: 'Turn cards, Zeny, and hard-won loot into useful tools, upgrades, and style.'
  },
  {
    to: '/costumes',
    number: '02',
    title: 'Costumes in Malangdo',
    description:
      'Find every costume acquisition, Fashion Stone, and enhancement service in one route.'
  }
] as const

export default function LandingPage(): ReactNode {
  const styles = landingPage()

  return (
    <main className={styles.root} aria-label="Ragnarok Offline player guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Ragnarok Offline Player Guide"
        description="A concise guide to the custom features that shape your solo adventure."
      />
      <div className={styles.content}>
        <Text as="p" tone="muted" className={styles.intro}>
          Start with a guide below. Each page describes confirmed in-game services and the route to
          use them.
        </Text>
        <div className={styles.cards}>
          {guides.map((guide) => (
            <Link className={styles.card} key={guide.to} to={guide.to}>
              <Text kind="eyebrow" tone="accent">
                {guide.number}
              </Text>
              <Heading as="h2" level="section">
                {guide.title}
              </Heading>
              <Text as="p" tone="muted">
                {guide.description}
              </Text>
              <span className={styles.cardAction}>Open guide →</span>
            </Link>
          ))}
        </div>
      </div>
    </main>
  )
}
