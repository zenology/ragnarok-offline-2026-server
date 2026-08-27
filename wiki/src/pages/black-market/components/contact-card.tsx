import type { ReactNode } from 'react'

import { Text } from '@/components/atoms'
import { ImageViewer } from '@/components/molecules'

import { disclosure } from '../styles/recipes'

import type { BlackMarketContact } from '../data/black-market'

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
        <ImageViewer images={contact.images} />
      </div>
    </details>
  )
}
