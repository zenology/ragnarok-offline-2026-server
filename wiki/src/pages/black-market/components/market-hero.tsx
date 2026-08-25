import type { ReactNode } from 'react'

import { Heading } from './heading'
import { JumpNavigation } from './jump-navigation'
import { Text } from './text'
import { page } from '../styles/recipes'

export function MarketHero(): ReactNode {
  const styles = page()

  return (
    <header className={styles.hero}>
      <Text kind="eyebrow" tone="gold">
        RAGNAROK OFFLINE · PLAYER GUIDE
      </Text>
      <Heading as="h1" level="hero">
        The Black Market
      </Heading>
      <Text as="p" kind="body" tone="muted" className={styles.lead}>
        A quiet network for turning cards, Zeny, and hard-won loot into useful tools, upgrades, and
        style.
      </Text>
      <JumpNavigation />
    </header>
  )
}
