import type { ReactNode } from 'react'

import type { CardPriceBand } from '../data/black-market'
import { card } from '../styles/recipes'

export function PriceCard({ band }: { band: CardPriceBand }): ReactNode {
  const styles = card({ kind: 'price' })

  return (
    <div className={styles.root}>
      <span className={styles.label}>{band.catalog}</span>
      <strong className={styles.value}>{band.range}</strong>
      <small className={styles.small}>{band.source}</small>
    </div>
  )
}
