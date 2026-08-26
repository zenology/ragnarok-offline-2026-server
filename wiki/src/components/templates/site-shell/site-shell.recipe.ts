import { sva } from 'styled-system/css'

export const siteShell = sva({
  slots: ['root', 'nav', 'brand', 'links', 'link', 'linkActive'],
  base: {
    root: { minHeight: '100vh' },
    nav: {
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'space-between',
      gap: '24px',
      maxWidth: '1120px',
      margin: '0 auto',
      padding: '20px 24px',
      borderBottom: '1px solid var(--colors-line-default)',
      position: 'relative',
      zIndex: 1,
      _mobile: { alignItems: 'flex-start', flexDirection: 'column', gap: '16px' }
    },
    brand: {
      color: 'text.default',
      fontFamily: 'siteHeading',
      fontSize: '20px',
      textDecoration: 'none',
      _hover: { color: 'primary.400' },
      _focusVisible: { outline: '2px solid', outlineColor: 'secondary.400', outlineOffset: '4px' }
    },
    links: { display: 'flex', flexWrap: 'wrap', gap: '8px' },
    link: {
      border: '1px solid transparent',
      borderRadius: '100px',
      color: 'text.muted',
      fontSize: '13px',
      padding: '8px 12px',
      textDecoration: 'none',
      transition: 'color 160ms ease, border-color 160ms ease, background 160ms ease',
      _hover: { borderColor: 'secondary.500', color: 'secondary.300' },
      _focusVisible: { outline: '2px solid', outlineColor: 'primary.400', outlineOffset: '3px' }
    },
    linkActive: {
      background: 'var(--colors-accent-muted)',
      borderColor: 'primary.500',
      color: 'primary.300'
    }
  }
})
