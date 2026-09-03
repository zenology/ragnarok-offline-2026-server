import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'

import { costumeCard } from '../styles/recipes'

import type { CostumeService } from '../data/costumes'

type CostumeServiceCardProps = {
  service: CostumeService
}

export function CostumeServiceCard({ service }: CostumeServiceCardProps): ReactNode {
  const styles = costumeCard()

  return (
    <article className={styles.root}>
      <div className={styles.topline}>
        <Text kind="tag" tone="accentSoft">
          {service.tag}
        </Text>
        <Text kind="location" tone="muted">
          {service.location}
        </Text>
      </div>
      <Heading as="h3" level="card">
        {service.name}
      </Heading>
      <Text as="p" tone="muted">
        {service.description}
      </Text>
    </article>
  )
}
