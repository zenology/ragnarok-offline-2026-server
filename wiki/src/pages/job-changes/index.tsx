import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { jobChangeGroups } from './data/job-changes'
import { jobChangesPage } from './styles/recipes'

export default function JobChangesPage(): ReactNode {
  const styles = jobChangesPage()

  return (
    <main className={styles.root} aria-label="Job change player guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Where to Change Jobs"
        description="A location-only reference for every supported job path, from Class 1 through Fourth Jobs."
      />
      <div className={styles.content}>
        <Callout variant="notice">
          This page lists where to begin each job change. It does not replace the quest dialogue or
          show level, item, or currency requirements.
        </Callout>
        {jobChangeGroups.map((group, index) => (
          <GuideSection
            id={group.title.toLowerCase().replace(/ /g, '-')}
            key={group.title}
            number={String(index + 1).padStart(2, '0')}
            eyebrow="Find the right guide"
            title={group.title}
          >
            <Text as="p" tone="muted">
              {group.description}
            </Text>
            <div className={styles.grid}>
              {group.entries.map((entry) => (
                <article className={styles.card} key={entry.job}>
                  <Heading as="h3" level="card">
                    {entry.job}
                  </Heading>
                  <Text as="p" className={styles.location}>
                    {entry.location}
                  </Text>
                  <Text as="p" tone="muted" className={styles.description}>
                    {entry.npc}
                  </Text>
                </article>
              ))}
            </div>
          </GuideSection>
        ))}
      </div>
    </main>
  )
}
