import type { ReactNode } from 'react'

import { heading } from '../styles/recipes'

type HeadingProps = {
  as: 'h1' | 'h2' | 'h3'
  level: 'hero' | 'section' | 'card' | 'service' | 'exchange'
  children: ReactNode
  id?: string
}

export function Heading({ as: Element, level, children, id }: HeadingProps): ReactNode {
  return (
    <Element id={id} className={heading({ level })}>
      {children}
    </Element>
  )
}
