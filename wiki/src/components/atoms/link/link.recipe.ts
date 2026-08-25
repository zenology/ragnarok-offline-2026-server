import { cva } from 'styled-system/css'

export const link = cva({
  base: {
    color: 'siteAccentSoft',
    textDecoration: 'none',
    _focusVisible: { outline: '2px solid', outlineColor: 'siteAccent', outlineOffset: '3px' }
  },
  variants: {
    variant: {
      jump: {
        padding: '0.65rem 1rem',
        border: '1px solid var(--colors-site-border)',
        borderRadius: '999px',
        background: 'rgba(231, 185, 109, 0.05)',
        fontSize: '0.84rem',
        backdropFilter: 'blur(10px)',
        _hover: { borderColor: 'siteAccent', background: 'rgba(231, 185, 109, 0.15)' }
      },
      external: {
        fontSize: '0.75rem',
        textAlign: 'right',
        _hover: { color: 'siteText', textDecoration: 'underline' }
      }
    }
  },
  defaultVariants: { variant: 'external' }
})
