import type { ReactNode } from 'react'

import { card } from '../styles/recipes'

type ServiceCardProps = {
  icon: string
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
        <span className={styles.icon}>{icon}</span>
        <span className={styles.location}>{location}</span>
      </div>
      <h3 className={styles.title}>{title}</h3>
      <p className={styles.description}>{description}</p>
      <span className={styles.tag}>{tag}</span>
    </article>
  )
}
