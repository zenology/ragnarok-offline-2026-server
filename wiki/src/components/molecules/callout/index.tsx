import type { ReactNode } from 'react'

import { callout } from './callout.recipe'

type CalloutProps = {
  children: ReactNode
  variant?: 'standard' | 'notice'
}

export function Callout({ children, variant = 'standard' }: CalloutProps): ReactNode {
  return <p className={callout({ variant })}>{children}</p>
}
