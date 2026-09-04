import { sva } from 'styled-system/css'

export const jobChangesPage = sva({
  slots: ['root', 'content', 'grid', 'card', 'location', 'description'],
  base: {
    root: { minHeight: '100vh' },
    content: { maxWidth: '1120px', margin: '0 auto', padding: '0 24px 96px' },
    grid: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '16px',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    card: {
      display: 'grid',
      gap: '8px',
      minHeight: '120px',
      padding: '20px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '8px',
      background: 'surface.default'
    },
    location: { color: 'secondary.300', fontSize: '13px', letterSpacing: '0.03em' },
    description: { color: 'text.muted', lineHeight: 1.7 }
  }
})
