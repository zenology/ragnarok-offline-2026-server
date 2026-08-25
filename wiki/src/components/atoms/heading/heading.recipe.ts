import { cva } from 'styled-system/css'

export const heading = cva({
  base: { margin: 0, color: 'siteText', fontFamily: 'siteHeading' },
  variants: {
    level: {
      hero: {
        maxWidth: '760px',
        margin: '0.75rem 0 1rem',
        fontSize: 'clamp(3.5rem, 9vw, 7.5rem)',
        lineHeight: 0.9,
        letterSpacing: '-0.065em'
      },
      section: {
        fontSize: 'clamp(2rem, 4vw, 3.25rem)',
        letterSpacing: '-0.04em'
      },
      card: { fontSize: '1.1rem' }
    }
  },
  defaultVariants: { level: 'card' }
})
