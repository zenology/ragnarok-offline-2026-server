import { cva } from 'styled-system/css'

export const heading = cva({
  base: { margin: 0, color: 'text.default', fontFamily: 'siteHeading' },
  variants: {
    level: {
      hero: {
        maxWidth: '760px',
        margin: '12px 0 16px',
        fontSize: 'clamp(56px, 9vw, 120px)',
        lineHeight: 1,
        letterSpacing: '-0.065em'
      },
      section: {
        fontSize: 'clamp(32px, 4vw, 52px)',
        letterSpacing: '-0.04em'
      },
      card: { fontSize: '18px' }
    }
  },
  defaultVariants: { level: 'card' }
})
