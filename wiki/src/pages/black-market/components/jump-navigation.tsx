import type { ReactNode } from 'react'

import { Link } from '@/components/atoms'

import { blackMarketPage } from '../styles/recipes'

const links = [
  ['#enter', 'Enter'],
  ['#earn', 'Earn Cash Points'],
  ['#spend', 'Spend Cash Points'],
  ['#featured', 'Featured Items']
] as const

export function JumpNavigation(): ReactNode {
  const styles = blackMarketPage()

  return (
    <nav className={styles.jump} aria-label="Black Market guide sections">
      {links.map(([href, label]) => (
        <Link variant="jump" href={href} key={href}>
          {label}
        </Link>
      ))}
    </nav>
  )
}
