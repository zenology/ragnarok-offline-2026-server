import type { ReactNode } from 'react'

import { SectionHeading } from './section-heading'
import { guideSection } from '../styles/recipes'

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
  const styles = guideSection()

  return (
    <section id={id} className={styles.root} aria-labelledby={titleId}>
      <SectionHeading number={number} kicker={kicker} title={title} id={titleId} />
      {children}
    </section>
  )
}
