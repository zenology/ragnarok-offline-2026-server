import { cva, sva } from 'styled-system/css'

export const blackMarketPage = sva({
  slots: [
    'root',
    'content',
    'jump',
    'sectionIntro',
    'muted'
  ],
  base: {
    root: {
      minHeight: '100vh',
      background:
        'radial-gradient(circle at 80% -10%, rgba(231, 185, 109, 0.11), transparent 30rem), #10110f'
    },
    content: {
      width: 'min(calc(100% - 3rem), 1180px)',
      margin: '0 auto',
      paddingBottom: '6rem',
      _mobile: { width: 'min(calc(100% - 2rem), 1180px)' }
    },
    jump: { display: 'flex', flexWrap: 'wrap', gap: '0.65rem', marginTop: '3rem' },
    sectionIntro: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '2rem',
      color: 'siteTextMuted',
      lineHeight: 1.7,
      '& strong': { color: 'siteText' },
      _mobile: { gridTemplateColumns: '1fr' }
    },
    muted: { color: 'siteTextMuted' }
  },
  variants: {},
  defaultVariants: {}
})

export const contactList = cva({
  base: {
    display: 'grid',
    gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
    alignItems: 'start',
    gap: '0.65rem',
    marginTop: '2rem',
    _mobile: { gridTemplateColumns: '1fr' }
  }
})

export const disclosure = sva({
  slots: ['root', 'summary', 'chevron', 'details'],
  base: {
    root: {
      border: '1px solid var(--colors-site-border)',
      borderRadius: '0.35rem',
      backgroundColor: 'siteSurface'
    },
    summary: {
      display: 'flex',
      justifyContent: 'space-between',
      padding: '1rem 1.1rem',
      cursor: 'pointer',
      listStyle: 'none',
      fontWeight: 600,
      '&::-webkit-details-marker': { display: 'none' }
    },
    chevron: {
      color: 'siteAccent',
      fontSize: '1.2rem',
      lineHeight: 1,
      '[open] &': { transform: 'rotate(45deg)' }
    },
    details: {
      display: 'flex',
      flexWrap: 'wrap',
      gap: '0.5rem 1rem',
      padding: '0 1.1rem 1rem',
      color: 'siteTextMuted',
      fontSize: '0.85rem'
    }
  },
  variants: {},
  defaultVariants: {}
})

export const loop = sva({
  slots: ['root', 'step', 'number', 'arrow', 'small'],
  base: {
    root: {
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'space-between',
      gap: '1rem',
      padding: '1.5rem',
      border: '1px solid var(--colors-site-border)',
      borderRadius: '0.4rem',
      background: 'linear-gradient(100deg, rgba(231, 185, 109, 0.11), rgba(255, 255, 255, 0.025))',
      _mobile: { alignItems: 'stretch', flexDirection: 'column' }
    },
    step: { display: 'grid', gap: '0.25rem' },
    number: { color: 'siteAccent', fontSize: '0.7rem', letterSpacing: '0.12em' },
    arrow: {
      color: 'siteAccent',
      fontSize: '1.5rem',
      _mobile: { transform: 'rotate(90deg)', alignSelf: 'center' }
    },
    small: { color: 'siteTextMuted' }
  },
  variants: {},
  defaultVariants: {}
})

export const grids = sva({
  slots: ['price', 'service', 'item'],
  base: {
    price: {
      display: 'grid',
      gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
      gap: '0.75rem',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    service: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '0.75rem',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    item: {
      display: 'grid',
      gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
      gap: '0.75rem',
      marginTop: '1.5rem',
      _mobile: { gridTemplateColumns: '1fr' }
    }
  },
  variants: {},
  defaultVariants: {}
})

export const card = sva({
  slots: [
    'root',
    'label',
    'value',
    'small',
    'meta',
    'footer',
    'top',
    'icon',
    'location',
    'tag',
    'description',
    'link',
    'title'
  ],
  base: {
    root: {
      border: '1px solid var(--colors-site-border)',
      borderRadius: '0.4rem',
      backgroundColor: 'siteSurface'
    },
    label: { color: 'siteTextMuted', fontSize: '0.85rem' },
    value: { color: 'siteAccentSoft', fontSize: '1.25rem' },
    small: { color: 'siteTextMuted' },
    meta: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '1rem',
      color: 'siteSuccess',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    footer: { display: 'flex', justifyContent: 'space-between', alignItems: 'end', gap: '1rem' },
    top: { display: 'flex', justifyContent: 'space-between', gap: '1rem' },
    icon: { color: 'siteAccent' },
    location: {
      color: 'siteTextMuted',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textAlign: 'right',
      textTransform: 'uppercase'
    },
    tag: {
      color: 'siteSuccess',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    description: {
      minHeight: '3.4rem',
      margin: '0.7rem 0 1rem',
      color: 'siteTextMuted',
      lineHeight: 1.6
    },
    link: {},
    title: { margin: 0, color: 'siteText', fontFamily: 'siteHeading' }
  },
  variants: {
    kind: {
      price: { root: { display: 'grid', gap: '0.45rem', padding: '1.1rem' } },
      service: {
        root: { padding: '1.35rem' },
        title: { marginTop: '1.25rem', fontSize: '1.3rem' }
      },
      item: {
        root: { display: 'grid', gap: '1.5rem', padding: '1.25rem' },
        title: { minHeight: '2.8rem', fontSize: '1.1rem' },
        value: { fontSize: '1.45rem', '& small': { fontSize: '0.65rem', letterSpacing: '0.08em' } }
      }
    },
    featured: {
      true: { root: { border: '1px solid rgba(231, 185, 109, 0.45)' } },
      false: {}
    }
  },
  defaultVariants: { kind: 'price', featured: false }
})

export const exchange = sva({
  slots: ['root', 'rates', 'rate', 'label', 'value', 'note'],
  base: {
    root: {
      display: 'grid',
      gridTemplateColumns: '0.8fr 1.2fr',
      gap: '1.5rem',
      marginTop: '1.5rem',
      padding: '1.5rem',
      border: '1px solid var(--colors-site-border)',
      borderRadius: '0.4rem',
      backgroundColor: 'siteSurfaceRaised',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    rates: { display: 'grid', gap: '0.7rem' },
    rate: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '1rem',
      paddingBottom: '0.7rem',
      borderBottom: '1px solid var(--colors-site-border)'
    },
    label: { color: 'siteTextMuted' },
    value: { color: 'siteAccentSoft', textAlign: 'right' },
    note: {
      gridColumn: '1 / -1',
      margin: 0,
      color: 'siteTextMuted',
      fontSize: '0.85rem',
      _mobile: { gridColumn: 'auto' }
    }
  },
  variants: {},
  defaultVariants: {}
})

export const catalog = sva({
  slots: ['root', 'column', 'label', 'value'],
  base: {
    root: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '1rem',
      marginTop: '1rem',
      padding: '1rem 1.25rem',
      border: '1px solid var(--colors-site-border)',
      color: 'siteTextMuted',
      fontSize: '0.83rem',
      lineHeight: 1.6,
      _mobile: { gridTemplateColumns: '1fr' }
    },
    column: { display: 'grid', gap: '0.25rem' },
    label: { color: 'siteAccentSoft' },
    value: {}
  },
  variants: {},
  defaultVariants: {}
})
