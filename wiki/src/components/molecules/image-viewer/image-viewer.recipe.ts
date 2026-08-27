import { sva } from 'styled-system/css'

export const imageViewer = sva({
  slots: ['root', 'item', 'image', 'caption'],
  base: {
    root: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '12px',
      width: '100%',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    item: {
      display: 'grid',
      gap: '8px',
      minWidth: 0,
      margin: 0
    },
    image: {
      display: 'block',
      width: '100%',
      aspectRatio: '16 / 9',
      objectFit: 'cover',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '4px',
      cursor: 'zoom-in',
      transition: 'border-color 160ms ease, opacity 160ms ease',
      _hover: { borderColor: 'line.accent', opacity: 0.9 },
      _focusVisible: { outline: '2px solid', outlineColor: 'secondary.400', outlineOffset: '2px' }
    },
    caption: {
      color: 'text.muted',
      fontSize: '12px',
      letterSpacing: '0.04em'
    }
  },
  variants: {},
  defaultVariants: {}
})
