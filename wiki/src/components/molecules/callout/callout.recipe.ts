import { cva } from 'styled-system/css'

export const callout = cva({
  base: {
    padding: '16px 20px',
    borderLeft: '2px solid var(--colors-accent-default)',
    background: 'var(--colors-accent-subtle)',
    color: 'text.muted',
    lineHeight: 1.6,
    '& strong': { color: 'accent.soft' }
  },
  variants: {
    variant: {
      standard: { margin: '20px 0 24px' },
      notice: { margin: '20px 0 0' }
    }
  },
  defaultVariants: { variant: 'standard' }
})
