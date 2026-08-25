import type { ReactNode } from 'react'

type ExternalLinkProps = {
  href: string
  children: ReactNode
}

export function ExternalLink({ href, children }: ExternalLinkProps): ReactNode {
  return (
    <a href={href} target="_blank" rel="noreferrer">
      {children}
    </a>
  )
}
