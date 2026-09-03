import { sva } from 'styled-system/css'

export const landingPage = sva({
  slots: ['root', 'content', 'intro', 'cards', 'card', 'cardAction'],
  base: {
    root: { minHeight: '100vh' },
    content: { maxWidth: '1120px', margin: '0 auto', padding: '0 24px 96px' },
    intro: { maxWidth: '600px', margin: '0 0 40px', lineHeight: 1.7 },
    cards: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '20px',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    card: {
      display: 'flex',
      flexDirection: 'column',
      gap: '16px',
      minHeight: '260px',
      padding: '28px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '12px',
      color: 'inherit',
      textDecoration: 'none',
      transition: 'border-color 160ms ease, transform 160ms ease',
      _hover: { borderColor: 'secondary.500', transform: 'translateY(-4px)' },
      _focusVisible: { outline: '2px solid', outlineColor: 'primary.400', outlineOffset: '4px' },
      '& h2': { marginTop: 'auto' }
    },
    cardAction: {
      display: 'inline-flex',
      alignItems: 'center',
      gap: '8px',
      color: 'primary.300',
      fontSize: '13px',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    }
  }
})
