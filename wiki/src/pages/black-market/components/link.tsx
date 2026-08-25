import type { ReactNode } from 'react'

import { link } from '../styles/recipes'

type LinkProps = {
  href: string
  children: ReactNode
  kind: 'jump' | 'external'
  external?: boolean
}

export function Link({ href, children, kind, external = false }: LinkProps): ReactNode {
  return (
    <a
      className={link({ kind })}
      href={href}
      {...(external ? { target: '_blank', rel: 'noreferrer' } : {})}
    >
      {children}
    </a>
  )
}
