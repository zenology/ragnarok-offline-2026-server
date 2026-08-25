import type { ReactNode } from 'react'

import { ExternalLink } from '../atoms/ExternalLink'
import { pandaStyles } from '../pandaStyles'

import type { FeaturedItem } from '../../data/blackMarket'

export function ItemCard({ item }: { item: FeaturedItem }): ReactNode {
  return (
    <article className={`item-card ${pandaStyles.itemCard}`}>
      <div className="item-card__meta">
        <span>{item.category}</span>
        <span>#{item.itemId}</span>
      </div>
      <h3>{item.name}</h3>
      <div className="item-card__footer">
        <strong>
          {item.price} <small>CP</small>
        </strong>
        <ExternalLink href={item.divinePrideUrl}>View on Divine Pride ↗</ExternalLink>
      </div>
    </article>
  )
}
