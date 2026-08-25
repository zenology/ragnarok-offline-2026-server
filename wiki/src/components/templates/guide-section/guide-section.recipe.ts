import { sva } from 'styled-system/css'

export const guideSection = sva({
  slots: ['root', 'heading', 'number'],
  base: {
    root: {
      padding: '72px 0',
      borderTop: '1px solid var(--colors-line-default)',
      scrollMarginTop: '24px',
      _mobile: { padding: '56px 0' }
    },
    heading: { display: 'flex', gap: '20px', alignItems: 'flex-start', marginBottom: '36px' },
    number: {
      paddingTop: '6px',
      color: 'accent.default',
      fontSize: '12px',
      letterSpacing: '0.14em',
      textTransform: 'uppercase'
    }
  },
  variants: {},
  defaultVariants: {}
})
