import type { ReactNode } from 'react'

import { SectionHeading } from '../atoms/SectionHeading'
import { pandaStyles } from '../pandaStyles'

type GuideSectionProps = {
  id: string
  titleId: string
  number: string
  kicker: string
  title: string
  children: ReactNode
}

export function GuideSection({
  id,
  titleId,
  number,
  kicker,
  title,
  children
}: GuideSectionProps): ReactNode {
  return (
    <section
      id={id}
      className={`market-section ${pandaStyles.guideSection}`}
      aria-labelledby={titleId}
    >
      <SectionHeading number={number} kicker={kicker} title={title} id={titleId} />
      {children}
    </section>
  )
}
