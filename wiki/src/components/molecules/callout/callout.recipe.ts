import { cva } from 'styled-system/css'

export const callout = cva({
  base: {
    padding: '1rem 1.25rem',
    borderLeft: '2px solid var(--colors-site-accent)',
    background: 'rgba(231, 185, 109, 0.07)',
    color: 'siteTextMuted',
    lineHeight: 1.6,
    '& strong': { color: 'siteAccentSoft' }
  },
  variants: {
    variant: {
      standard: { margin: '1.25rem 0 1.5rem' },
      notice: { margin: '1.25rem 0 0' }
    }
  },
  defaultVariants: { variant: 'standard' }
})
