import type { ReactNode } from 'react'

import { pandaStyles } from '../pandaStyles'

import type { CardPriceBand } from '../../data/blackMarket'

export function PriceCard({ band }: { band: CardPriceBand }): ReactNode {
  return (
    <div className={`price-card ${pandaStyles.priceCard}`}>
      <span className="price-card__label">{band.catalog}</span>
      <strong>{band.range}</strong>
      <small>{band.source}</small>
    </div>
  )
}
