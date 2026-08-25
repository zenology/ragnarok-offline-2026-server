import type { ReactNode } from 'react'

import { catalog as catalogStyles } from '../recipes/market'

type CatalogStripProps = {
  rookeShelves: string[]
  harlanCatalogs: Array<{ name: string; count: number }>
}

export function CatalogStrip({ rookeShelves, harlanCatalogs }: CatalogStripProps): ReactNode {
  const styles = catalogStyles()

  return (
    <div className={styles.root}>
      <div className={styles.column}>
        <strong className={styles.label}>Rooke’s shelves</strong>
        <span className={styles.value}>{rookeShelves.join(' · ')}</span>
      </div>
      <div className={styles.column}>
        <strong className={styles.label}>Harlan’s catalogs</strong>
        <span className={styles.value}>
          {harlanCatalogs.map((catalog) => `${catalog.name} ${catalog.count}`).join(' · ')}
        </span>
      </div>
    </div>
  )
}
