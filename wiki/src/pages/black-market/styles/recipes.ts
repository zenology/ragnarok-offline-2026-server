import { cva, sva } from 'styled-system/css'

export const blackMarketPage = sva({
  slots: ['root', 'content', 'jump', 'sectionIntro', 'muted'],
  base: {
    root: {
      minHeight: '100vh',
      background:
        'radial-gradient(circle at 80% -10%, var(--colors-accent-emphasis), transparent 480px), var(--colors-surface-canvas)'
    },
    content: {
      width: 'min(calc(100% - 48px), 1180px)',
      margin: '0 auto',
      paddingBottom: '96px',
      _mobile: { width: 'min(calc(100% - 32px), 1180px)' }
    },
    jump: { display: 'flex', flexWrap: 'wrap', gap: '10px', marginTop: '48px' },
    sectionIntro: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '32px',
      color: 'text.muted',
      lineHeight: 1.7,
      '& strong': { color: 'text.default' },
      _mobile: { gridTemplateColumns: '1fr' }
    },
    muted: { color: 'text.muted' }
  },
  variants: {},
  defaultVariants: {}
})

export const contactList = cva({
  base: {
    display: 'grid',
    gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
    alignItems: 'start',
    gap: '10px',
    marginTop: '32px',
    _mobile: { gridTemplateColumns: '1fr' }
  }
})

export const disclosure = sva({
  slots: ['root', 'summary', 'chevron', 'details'],
  base: {
    root: {
      border: '1px solid var(--colors-line-default)',
      borderRadius: '6px',
      backgroundColor: 'surface.default'
    },
    summary: {
      display: 'flex',
      justifyContent: 'space-between',
      padding: '16px 18px',
      cursor: 'pointer',
      listStyle: 'none',
      fontWeight: 600,
      '&::-webkit-details-marker': { display: 'none' }
    },
    chevron: {
      color: 'accent.default',
      fontSize: '20px',
      lineHeight: 1,
      '[open] &': { transform: 'rotate(45deg)' }
    },
    details: {
      display: 'grid',
      gap: '8px 16px',
      padding: '0 18px 16px',
      color: 'text.muted',
      fontSize: '14px'
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
      gap: '16px',
      padding: '24px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '6px',
      background:
        'linear-gradient(100deg, var(--colors-accent-emphasis), var(--colors-surface-highlight))',
      _mobile: { alignItems: 'stretch', flexDirection: 'column' }
    },
    step: { display: 'grid', gap: '4px' },
    number: { color: 'accent.default', fontSize: '12px', letterSpacing: '0.12em' },
    arrow: {
      color: 'accent.default',
      fontSize: '24px',
      _mobile: { transform: 'rotate(90deg)', alignSelf: 'center' }
    },
    small: { color: 'text.muted' }
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
      gap: '12px',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    service: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '12px',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    item: {
      display: 'grid',
      gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
      gap: '12px',
      marginTop: '24px',
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
      border: '1px solid var(--colors-border-default)',
      borderRadius: '6px',
      backgroundColor: 'surface.default'
    },
    label: { color: 'text.muted', fontSize: '14px' },
    value: { color: 'accent.soft', fontSize: '20px' },
    small: { color: 'text.muted' },
    meta: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '16px',
      color: 'status.success.foreground',
      fontSize: '10px',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    footer: { display: 'flex', justifyContent: 'space-between', alignItems: 'end', gap: '16px' },
    top: { display: 'flex', justifyContent: 'space-between', gap: '16px' },
    icon: { color: 'accent.default' },
    location: {
      color: 'text.muted',
      fontSize: '10px',
      letterSpacing: '0.08em',
      textAlign: 'right',
      textTransform: 'uppercase'
    },
    tag: {
      color: 'status.success.foreground',
      fontSize: '10px',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    description: {
      minHeight: '56px',
      margin: '12px 0 16px',
      color: 'text.muted',
      lineHeight: 1.6
    },
    link: {},
    title: { margin: 0, color: 'text.default', fontFamily: 'siteHeading' }
  },
  variants: {
    kind: {
      price: { root: { display: 'grid', gap: '8px', padding: '18px' } },
      service: {
        root: { padding: '22px' },
        title: { marginTop: '20px', fontSize: '20px' }
      },
      item: {
        root: { display: 'grid', gap: '24px', padding: '20px' },
        title: { minHeight: '44px', fontSize: '18px' },
        value: { fontSize: '24px', '& small': { fontSize: '10px', letterSpacing: '0.08em' } }
      }
    },
    featured: {
      true: { root: { border: '1px solid var(--colors-line-accent)' } },
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
      gap: '24px',
      marginTop: '24px',
      padding: '24px',
      border: '1px solid var(--colors-line-default)',
      borderRadius: '6px',
      backgroundColor: 'surface.raised',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    rates: { display: 'grid', gap: '12px' },
    rate: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '16px',
      paddingBottom: '12px',
      borderBottom: '1px solid var(--colors-line-default)'
    },
    label: { color: 'text.muted' },
    value: { color: 'accent.soft', textAlign: 'right' },
    note: {
      gridColumn: '1 / -1',
      margin: 0,
      color: 'text.muted',
      fontSize: '14px',
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
      gap: '16px',
      marginTop: '16px',
      padding: '16px 20px',
      border: '1px solid var(--colors-line-default)',
      color: 'text.muted',
      fontSize: '14px',
      lineHeight: 1.6,
      _mobile: { gridTemplateColumns: '1fr' }
    },
    column: { display: 'grid', gap: '4px' },
    label: { color: 'accent.soft' },
    value: {}
  },
  variants: {},
  defaultVariants: {}
})
