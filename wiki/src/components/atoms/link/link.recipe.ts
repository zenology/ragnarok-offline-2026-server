import { cva } from 'styled-system/css'

export const link = cva({
  base: {
    color: 'accent.soft',
    textDecoration: 'none',
    _focusVisible: { outline: '2px solid', outlineColor: 'accent.default', outlineOffset: '4px' }
  },
  variants: {
    variant: {
      jump: {
        padding: '10px 16px',
        border: '1px solid var(--colors-line-default)',
        borderRadius: '100px',
        background: 'var(--colors-accent-muted)',
        fontSize: '14px',
        backdropFilter: 'blur(12px)',
        _hover: { borderColor: 'accent.default', background: 'var(--colors-accent-hover)' }
      },
      external: {
        fontSize: '12px',
        textAlign: 'right',
        _hover: { color: 'text.default', textDecoration: 'underline' }
      }
    }
  },
  defaultVariants: { variant: 'external' }
})
