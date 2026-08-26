import { Link } from '@tanstack/react-router'
import type { ReactNode } from 'react'

import { siteShell } from './site-shell.recipe'

type SiteShellProps = {
  children: ReactNode
}

const navigation = [
  { label: 'Home', to: '/' },
  { label: 'Black Market', to: '/black-market' },
  { label: 'Costumes', to: '/costumes' }
] as const

export function SiteShell({ children }: SiteShellProps): ReactNode {
  const styles = siteShell()

  return (
    <div className={styles.root}>
      <nav className={styles.nav} aria-label="Primary navigation">
        <Link className={styles.brand} to="/">
          Ragnarok Offline
        </Link>
        <div className={styles.links}>
          {navigation.map((item) => (
            <Link
              activeOptions={{ exact: item.to === '/' }}
              activeProps={{ className: `${styles.link} ${styles.linkActive}` }}
              className={styles.link}
              key={item.to}
              to={item.to}
            >
              {item.label}
            </Link>
          ))}
        </div>
      </nav>
      {children}
    </div>
  )
}
