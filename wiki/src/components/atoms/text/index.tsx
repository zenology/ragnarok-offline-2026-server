import type { ReactNode } from 'react'

import { cx } from 'styled-system/css'

import { text } from './text.recipe'

type TextProps = {
  as?: 'p' | 'span' | 'small' | 'div'
  children: ReactNode
  tone?: 'body' | 'muted' | 'accent' | 'accentSoft' | 'success'
  kind?: 'body' | 'eyebrow' | 'kicker' | 'meta' | 'location' | 'tag' | 'small'
  className?: string
}

export function Text({
  as: Element = 'span',
  children,
  tone,
  kind,
  className
}: TextProps): ReactNode {
  return <Element className={cx(text({ tone, kind }), className)}>{children}</Element>
}
