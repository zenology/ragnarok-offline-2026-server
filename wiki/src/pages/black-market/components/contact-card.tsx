import type { ReactNode } from 'react'

import { Text } from './text'
import type { BlackMarketContact } from '../data/black-market'
import { disclosure } from '../styles/recipes'

export function ContactCard({ contact }: { contact: BlackMarketContact }): ReactNode {
  const styles = disclosure()

  return (
    <details className={styles.root}>
      <summary className={styles.summary}>
        <span>{contact.city}</span>
        <span className={styles.chevron} aria-hidden="true">
          +
        </span>
      </summary>
      <div className={styles.details}>
        <span>{contact.location}</span>
        <Text tone="muted">Contact position: {contact.coordinates}</Text>
      </div>
    </details>
  )
}
