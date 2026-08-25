import type { ReactNode } from 'react'

import { guideSection } from '../recipes/market'

import { Heading } from './Heading'
import { Text } from './Text'

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
