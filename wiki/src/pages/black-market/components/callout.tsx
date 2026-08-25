import type { ReactNode } from 'react'

import { page } from '../styles/recipes'

export function Callout({
  children,
  notice = false
}: {
  children: ReactNode
  notice?: boolean
}): ReactNode {
  const styles = page()

  return <p className={notice ? styles.notice : styles.callout}>{children}</p>
}
