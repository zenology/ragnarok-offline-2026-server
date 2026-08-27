import type { ReactNode } from 'react'

import { Link } from '@/components/atoms'

import { card } from '../styles/recipes'

import type { FeaturedItem } from '../data/black-market'

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
        <Link variant="external" external href={item.divinePrideUrl}>
          View on Divine Pride ↗
        </Link>
      </div>
    </article>
  )
}
