import { sva } from 'styled-system/css'

export const privateAirshipPage = sva({
  slots: ['root', 'content'],
  base: {
    root: { minHeight: '100vh' },
    content: { maxWidth: '1120px', margin: '0 auto', padding: '0 24px 96px' }
  }
})
