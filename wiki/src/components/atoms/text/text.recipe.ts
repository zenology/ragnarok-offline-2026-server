import { cva } from 'styled-system/css'

export const text = cva({
  variants: {
    tone: {
      body: { color: 'text.default' },
      muted: { color: 'text.muted' },
      accent: { color: 'accent.default' },
      accentSoft: { color: 'accent.soft' },
      success: { color: 'status.success.foreground' }
    },
    kind: {
      body: { lineHeight: 1.7 },
      eyebrow: { fontSize: '12px', letterSpacing: '0.14em', textTransform: 'uppercase' },
      kicker: {
        margin: 0,
        fontSize: '12px',
        letterSpacing: '0.14em',
        textTransform: 'uppercase'
      },
      meta: { fontSize: '10px', letterSpacing: '0.08em', textTransform: 'uppercase' },
      location: {
        fontSize: '10px',
        letterSpacing: '0.08em',
        textAlign: 'right',
        textTransform: 'uppercase'
      },
      tag: { fontSize: '10px', letterSpacing: '0.08em', textTransform: 'uppercase' },
      small: { fontSize: '14px' }
    }
  },
  defaultVariants: { tone: 'body', kind: 'body' }
})
