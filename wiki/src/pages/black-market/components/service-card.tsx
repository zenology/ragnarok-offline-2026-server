import type { ReactNode } from 'react'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'

import { card } from '../styles/recipes'

import type { IconDefinition } from '@fortawesome/fontawesome-svg-core'

type ServiceCardProps = {
  icon: IconDefinition
  location: string
  title: string
  description: string
  tag: string
  featured?: boolean
}

export function ServiceCard({
  icon,
  location,
  title,
  description,
  tag,
  featured = false
}: ServiceCardProps): ReactNode {
  const styles = card({ kind: 'service', featured })

  return (
    <article className={styles.root}>
      <div className={styles.top}>
        <span className={styles.icon} aria-hidden="true">
          <FontAwesomeIcon icon={icon} />
        </span>
        <span className={styles.location}>{location}</span>
      </div>
      <h3 className={styles.title}>{title}</h3>
      <p className={styles.description}>{description}</p>
      <span className={styles.tag}>{tag}</span>
    </article>
  )
}
