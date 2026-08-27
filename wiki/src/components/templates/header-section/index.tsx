import type { CSSProperties, ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'

import { headerSection } from './header-section.recipe'

type HeaderSectionProps = {
  eyebrow: ReactNode
  title: ReactNode
  description?: ReactNode
  image?: string
  children?: ReactNode
}

export function HeaderSection({
  eyebrow,
  title,
  description,
  image,
  children
}: HeaderSectionProps): ReactNode {
  const styles = headerSection({ image: Boolean(image) })
  const style = image ? ({ '--header-image': `url("${image}")` } as CSSProperties) : undefined

  return (
    <header className={styles.root} style={style}>
      <Text kind="eyebrow" tone="accent">
        {eyebrow}
      </Text>
      <Heading as="h1" level="hero">
        {title}
      </Heading>
      {description && (
        <Text as="p" kind="body" tone="muted" className={styles.description}>
          {description}
        </Text>
      )}
      {children}
    </header>
  )
}
