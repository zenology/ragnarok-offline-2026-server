import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'

import { guideSection } from './guide-section.recipe'

type GuideSectionProps = {
  id: string
  number?: string
  eyebrow: string
  title: string
  children: ReactNode
}

export function GuideSection({ id, number, eyebrow, title, children }: GuideSectionProps): ReactNode {
  const styles = guideSection()
  const titleId = `${id}-title`

  return (
    <section id={id} className={styles.root} aria-labelledby={titleId}>
      <div className={styles.heading}>
        {number && (
          <Text kind="meta" tone="accent" className={styles.number}>
            {number}
          </Text>
        )}
        <div>
          <Text as="p" kind="kicker" tone="accent">
            {eyebrow}
          </Text>
          <Heading as="h2" level="section" id={titleId}>
            {title}
          </Heading>
        </div>
      </div>
      {children}
    </section>
  )
}
