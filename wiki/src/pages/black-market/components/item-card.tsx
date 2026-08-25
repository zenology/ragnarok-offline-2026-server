import type { ReactNode } from 'react'

import { ExternalLink } from './external-link'
import type { FeaturedItem } from '../data/black-market'
import { card } from '../styles/recipes'

export function ItemCard({ item }: { item: FeaturedItem }): ReactNode {
  const styles = card({ kind: 'item' })

  return (
    <article className={styles.root}>
      <div className={styles.meta}>
        <span>{item.category}</span>
        <span>#{item.itemId}</span>
      </div>
      <h3 className={styles.title}>{item.name}</h3>
      <div className={styles.footer}>
        <strong className={styles.value}>
          {item.price} <small>CP</small>
        </strong>
        <ExternalLink href={item.divinePrideUrl}>View on Divine Pride ↗</ExternalLink>
      </div>
    </article>
  )
}
