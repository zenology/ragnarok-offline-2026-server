import type { ReactNode } from 'react'

import { Heading } from './heading'
import { Text } from './text'
import { guideSection } from '../styles/recipes'

type SectionHeadingProps = {
  number: string
  kicker: string
  title: string
  id: string
}

export function SectionHeading({ number, kicker, title, id }: SectionHeadingProps): ReactNode {
  const styles = guideSection()

  return (
    <div className={styles.heading}>
      <Text kind="meta" tone="gold" className={styles.number}>
        {number}
      </Text>
      <div>
        <Text as="p" kind="kicker" tone="gold">
          {kicker}
        </Text>
        <Heading as="h2" level="section" id={id}>
          {title}
        </Heading>
      </div>
    </div>
  )
}
