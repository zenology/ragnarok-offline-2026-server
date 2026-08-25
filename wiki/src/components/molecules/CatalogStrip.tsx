import type { ReactNode } from 'react'

type CatalogStripProps = {
  rookeShelves: string[]
  harlanCatalogs: Array<{ name: string; count: number }>
}

export function CatalogStrip({ rookeShelves, harlanCatalogs }: CatalogStripProps): ReactNode {
  return (
    <div className="catalog-strip">
      <div>
        <strong>Rooke’s shelves</strong>
        <span>{rookeShelves.join(' · ')}</span>
      </div>
      <div>
        <strong>Harlan’s catalogs</strong>
        <span>
          {harlanCatalogs.map((catalog) => `${catalog.name} ${catalog.count}`).join(' · ')}
        </span>
      </div>
    </div>
  )
}
