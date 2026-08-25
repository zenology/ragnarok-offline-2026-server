import { sva } from 'styled-system/css'

export const guideSection = sva({
  slots: ['root', 'heading', 'number'],
  base: {
    root: {
      padding: '4.5rem 0',
      borderTop: '1px solid var(--colors-site-border)',
      scrollMarginTop: '1.5rem',
      _mobile: { padding: '3.5rem 0' }
    },
    heading: { display: 'flex', gap: '1.25rem', alignItems: 'flex-start', marginBottom: '2.25rem' },
    number: {
      paddingTop: '0.35rem',
      color: 'siteAccent',
      fontSize: '0.7rem',
      letterSpacing: '0.14em',
      textTransform: 'uppercase'
    }
  },
  variants: {},
  defaultVariants: {}
})
