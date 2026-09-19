import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'

import { serviceSummaryCard } from './service-summary-card.recipe'

type ServiceSummary = {
  name: string
  location: string
  description: string
  tag: string
}

type ServiceSummaryCardProps = {
  service: ServiceSummary
  children?: ReactNode
}

function ServiceSummaryCard({ service, children }: ServiceSummaryCardProps): ReactNode {
  const styles = serviceSummaryCard()

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
      {children}
    </article>
  )
}

export { ServiceSummaryCard }
export type { ServiceSummary }
