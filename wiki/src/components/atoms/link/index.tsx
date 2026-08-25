import type { ReactNode } from 'react'

import { link } from './link.recipe'

type LinkProps = {
  href: string
  children: ReactNode
  variant: 'jump' | 'external'
  external?: boolean
}

export function Link({ href, children, variant, external = false }: LinkProps): ReactNode {
  return (
    <a
      className={link({ variant })}
      href={href}
      {...(external ? { target: '_blank', rel: 'noreferrer' } : {})}
    >
      {children}
    </a>
  )
}
