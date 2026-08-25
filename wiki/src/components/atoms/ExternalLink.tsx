import type { ReactNode } from 'react'

import { Link } from './Link'

type ExternalLinkProps = { href: string; children: ReactNode }

export function ExternalLink({ href, children }: ExternalLinkProps): ReactNode {
  return (
    <Link href={href} kind="external" external>
      {children}
    </Link>
  )
}
