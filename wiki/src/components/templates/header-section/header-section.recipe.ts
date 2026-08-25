import { sva } from 'styled-system/css'

export const headerSection = sva({
  slots: ['root', 'description'],
  base: {
    root: {
      width: '100%',
      padding: '104px 72px',
      _mobile: { padding: '64px 0 48px 16px' }
    },
    description: {
      maxWidth: '560px',
      margin: 0,
      fontSize: 'clamp(18px, 2vw, 22px)',
      lineHeight: 1.6
    }
  },
  variants: {
    image: {
      true: {
        root: {
          backgroundImage:
            'linear-gradient(90deg, var(--colors-surface-canvas) 0%, color-mix(in srgb, var(--colors-surface-canvas) 94%, transparent) 35%, color-mix(in srgb, var(--colors-surface-canvas) 45%, transparent) 75%, transparent 100%), var(--header-image)',
          backgroundPosition: 'center',
          backgroundRepeat: 'no-repeat',
          backgroundSize: 'cover',
          backdropFilter: 'blur(12px)'
        }
      },
      false: {}
    }
  },
  defaultVariants: { image: false }
})
