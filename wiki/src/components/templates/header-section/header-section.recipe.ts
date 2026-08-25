import { sva } from 'styled-system/css'

export const headerSection = sva({
  slots: ['root', 'description'],
  base: {
    root: {
      width: '100%',
      padding: '6.5rem 4.5rem',
      _mobile: { padding: '4rem 0 3rem 1rem' }
    },
    description: {
      maxWidth: '560px',
      margin: 0,
      fontSize: 'clamp(1.05rem, 2vw, 1.35rem)',
      lineHeight: 1.6
    }
  },
  variants: {
    image: {
      true: {
        root: {
          backgroundImage:
            'linear-gradient(90deg, #10110f 0%, rgba(16, 17, 15, 0.94) 35%, rgba(16, 17, 15, 0.45) 75%, transparent 100%), var(--header-image)',
          backgroundPosition: 'center',
          backgroundRepeat: 'no-repeat',
          backgroundSize: 'cover',
          backdropFilter: 'blur(10px)'
        }
      },
      false: {}
    }
  },
  defaultVariants: { image: false }
})
