import type { ReactNode } from 'react'

import { link, page } from '../styles/recipes'

const links = [
  ['#enter', 'Enter'],
  ['#earn', 'Earn Cash Points'],
  ['#spend', 'Spend Cash Points'],
  ['#featured', 'Featured Items']
] as const

export function JumpNavigation(): ReactNode {
  const styles = page()

  return (
    <nav className={styles.jump} aria-label="Black Market guide sections">
      {links.map(([href, label]) => (
        <a className={link({ kind: 'jump' })} href={href} key={href}>
          {label}
        </a>
      ))}
    </nav>
  )
}
