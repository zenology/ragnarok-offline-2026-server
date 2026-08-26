import { sva } from 'styled-system/css'

export const costumesPage = sva({
  slots: ['root', 'content', 'grid', 'route'],
  base: {
    root: { minHeight: '100vh' },
    content: { maxWidth: '1120px', margin: '0 auto', padding: '0 24px 96px' },
    grid: {
      display: 'grid',
      gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
      gap: '16px',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    route: {
      padding: '20px 24px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '8px',
      background: 'var(--colors-accent-subtle)',
      color: 'text.muted',
      fontSize: '16px',
      lineHeight: 1.7,
      '& strong': { color: 'secondary.300' }
    }
  }
})

export const costumeCard = sva({
  slots: ['root', 'topline'],
  base: {
    root: {
      display: 'flex',
      flexDirection: 'column',
      gap: '12px',
      minHeight: '190px',
      padding: '20px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '8px'
    },
    topline: { display: 'flex', flexDirection: 'column', gap: '8px' }
  }
})
