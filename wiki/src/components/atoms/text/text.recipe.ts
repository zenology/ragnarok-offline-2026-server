import { cva } from 'styled-system/css'

export const text = cva({
  variants: {
    tone: {
      body: { color: 'siteText' },
      muted: { color: 'siteTextMuted' },
      accent: { color: 'siteAccent' },
      accentSoft: { color: 'siteAccentSoft' },
      success: { color: 'siteSuccess' }
    },
    kind: {
      body: { lineHeight: 1.7 },
      eyebrow: { fontSize: '0.7rem', letterSpacing: '0.14em', textTransform: 'uppercase' },
      kicker: {
        margin: 0,
        fontSize: '0.7rem',
        letterSpacing: '0.14em',
        textTransform: 'uppercase'
      },
      meta: { fontSize: '0.62rem', letterSpacing: '0.08em', textTransform: 'uppercase' },
      location: {
        fontSize: '0.62rem',
        letterSpacing: '0.08em',
        textAlign: 'right',
        textTransform: 'uppercase'
      },
      tag: { fontSize: '0.62rem', letterSpacing: '0.08em', textTransform: 'uppercase' },
      small: { fontSize: '0.85rem' }
    }
  },
  defaultVariants: { tone: 'body', kind: 'body' }
})
