import { sva } from 'styled-system/css'

export const serviceSummaryCard = sva({
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
