import { cva, sva } from 'styled-system/css'

export const text = cva({
  variants: {
    tone: {
      body: { color: 'marketInk' },
      muted: { color: 'marketMuted' },
      gold: { color: 'marketGold' },
      goldSoft: { color: 'marketGoldSoft' },
      green: { color: 'marketGreen' }
    },
    kind: {
      body: { lineHeight: 1.7 },
      eyebrow: { fontSize: '0.7rem', letterSpacing: '0.14em', textTransform: 'uppercase' },
      kicker: {
        margin: 0,
        fontSize: '0.7rem',
        letterSpacing: '0.14em',
        textTransform: 'uppercase'
      },
      meta: { fontSize: '0.62rem', letterSpacing: '0.08em', textTransform: 'uppercase' },
      location: {
        fontSize: '0.62rem',
        letterSpacing: '0.08em',
        textAlign: 'right',
        textTransform: 'uppercase'
      },
      tag: { fontSize: '0.62rem', letterSpacing: '0.08em', textTransform: 'uppercase' },
      small: { fontSize: '0.85rem' }
    }
  },
  defaultVariants: { tone: 'body', kind: 'body' }
})

export const heading = cva({
  base: { margin: 0, color: 'marketInk', fontFamily: 'marketHeading' },
  variants: {
    level: {
      hero: {
        maxWidth: '760px',
        margin: '0.75rem 0 1rem',
        fontSize: 'clamp(3.5rem, 9vw, 7.5rem)',
        lineHeight: 0.9,
        letterSpacing: '-0.065em'
      },
      section: {
        fontSize: 'clamp(2rem, 4vw, 3.25rem)',
        letterSpacing: '-0.04em'
      },
      card: { fontSize: '1.1rem' },
      service: { marginTop: '1.25rem', fontSize: '1.3rem' },
      exchange: { fontSize: '1.1rem' }
    }
  },
  defaultVariants: { level: 'card' }
})

export const link = cva({
  base: {
    color: 'marketGoldSoft',
    textDecoration: 'none',
    _focusVisible: { outline: '2px solid', outlineColor: 'marketGold', outlineOffset: '3px' }
  },
  variants: {
    kind: {
      jump: {
        padding: '0.65rem 1rem',
        border: '1px solid var(--colors-market-line)',
        borderRadius: '999px',
        background: 'rgba(255, 255, 255, 0.025)',
        fontSize: '0.84rem',
        _hover: { borderColor: 'marketGold', background: 'rgba(231, 185, 109, 0.1)' }
      },
      external: {
        fontSize: '0.75rem',
        textAlign: 'right',
        _hover: { color: 'marketInk', textDecoration: 'underline' }
      }
    }
  },
  defaultVariants: { kind: 'external' }
})

export const page = sva({
  slots: [
    'root',
    'hero',
    'content',
    'eyebrow',
    'lead',
    'jump',
    'sectionIntro',
    'muted',
    'callout',
    'notice'
  ],
  base: {
    root: {
      minHeight: '100vh',
      background:
        'radial-gradient(circle at 80% -10%, rgba(231, 185, 109, 0.11), transparent 30rem), #10110f'
    },
    hero: {
      width: 'min(calc(100% - 3rem), 1180px)',
      margin: '0 auto',
      padding: '6.5rem 0 4.5rem',
      _mobile: { width: 'min(calc(100% - 2rem), 1180px)', padding: '4rem 0 3rem' }
    },
    content: {
      width: 'min(calc(100% - 3rem), 1180px)',
      margin: '0 auto',
      paddingBottom: '6rem',
      _mobile: { width: 'min(calc(100% - 2rem), 1180px)' }
    },
    eyebrow: {
      color: 'marketGold',
      fontSize: '0.7rem',
      letterSpacing: '0.14em',
      textTransform: 'uppercase'
    },
    lead: {
      maxWidth: '560px',
      margin: 0,
      color: 'marketMuted',
      fontSize: 'clamp(1.05rem, 2vw, 1.35rem)',
      lineHeight: 1.6
    },
    jump: { display: 'flex', flexWrap: 'wrap', gap: '0.65rem', marginTop: '3rem' },
    sectionIntro: {
      display: 'grid',
      gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
      gap: '2rem',
      color: 'marketMuted',
      lineHeight: 1.7,
      '& strong': { color: 'marketInk' },
      _mobile: { gridTemplateColumns: '1fr' }
    },
    muted: { color: 'marketMuted' },
    callout: {
      margin: '1.25rem 0 1.5rem',
      padding: '1rem 1.25rem',
      borderLeft: '2px solid var(--colors-market-gold)',
      background: 'rgba(231, 185, 109, 0.07)',
      color: 'marketMuted',
      lineHeight: 1.6,
      '& strong': { color: 'marketGoldSoft' }
    },
    notice: {
      margin: '1.25rem 0 0',
      padding: '1rem 1.25rem',
      borderLeft: '2px solid var(--colors-market-gold)',
      background: 'rgba(231, 185, 109, 0.07)',
      color: 'marketMuted',
      lineHeight: 1.6,
      '& strong': { color: 'marketGoldSoft' }
    }
  },
  variants: {},
  defaultVariants: {}
})

export const guideSection = sva({
  slots: ['root', 'heading', 'number', 'kicker'],
  base: {
    root: {
      padding: '4.5rem 0',
      borderTop: '1px solid var(--colors-market-line)',
      scrollMarginTop: '1.5rem',
      _mobile: { padding: '3.5rem 0' }
    },
    heading: { display: 'flex', gap: '1.25rem', alignItems: 'flex-start', marginBottom: '2.25rem' },
    number: {
      paddingTop: '0.35rem',
      color: 'marketGold',
      fontSize: '0.7rem',
      letterSpacing: '0.14em',
      textTransform: 'uppercase'
    },
    kicker: {
      margin: '0 0 0.45rem',
      color: 'marketGold',
      fontSize: '0.7rem',
      letterSpacing: '0.14em',
      textTransform: 'uppercase'
    }
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
      border: '1px solid var(--colors-market-line)',
      borderRadius: '0.35rem',
      backgroundColor: 'marketPanel'
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
      color: 'marketGold',
      fontSize: '1.2rem',
      lineHeight: 1,
      '[open] &': { transform: 'rotate(45deg)' }
    },
    details: {
      display: 'flex',
      flexWrap: 'wrap',
      gap: '0.5rem 1rem',
      padding: '0 1.1rem 1rem',
      color: 'marketMuted',
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
      border: '1px solid var(--colors-market-line)',
      borderRadius: '0.4rem',
      background: 'linear-gradient(100deg, rgba(231, 185, 109, 0.11), rgba(255, 255, 255, 0.025))',
      _mobile: { alignItems: 'stretch', flexDirection: 'column' }
    },
    step: { display: 'grid', gap: '0.25rem' },
    number: { color: 'marketGold', fontSize: '0.7rem', letterSpacing: '0.12em' },
    arrow: {
      color: 'marketGold',
      fontSize: '1.5rem',
      _mobile: { transform: 'rotate(90deg)', alignSelf: 'center' }
    },
    small: { color: 'marketMuted' }
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
      border: '1px solid var(--colors-market-line)',
      borderRadius: '0.4rem',
      backgroundColor: 'marketPanel'
    },
    label: { color: 'marketMuted', fontSize: '0.85rem' },
    value: { color: 'marketGoldSoft', fontSize: '1.25rem' },
    small: { color: 'marketMuted' },
    meta: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '1rem',
      color: 'marketGreen',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    footer: { display: 'flex', justifyContent: 'space-between', alignItems: 'end', gap: '1rem' },
    top: { display: 'flex', justifyContent: 'space-between', gap: '1rem' },
    icon: { color: 'marketGold' },
    location: {
      color: 'marketMuted',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textAlign: 'right',
      textTransform: 'uppercase'
    },
    tag: {
      color: 'marketGreen',
      fontSize: '0.62rem',
      letterSpacing: '0.08em',
      textTransform: 'uppercase'
    },
    description: {
      minHeight: '3.4rem',
      margin: '0.7rem 0 1rem',
      color: 'marketMuted',
      lineHeight: 1.6
    },
    link: {},
    title: { margin: 0, color: 'marketInk', fontFamily: 'marketHeading' }
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
      border: '1px solid var(--colors-market-line)',
      borderRadius: '0.4rem',
      backgroundColor: 'marketPanelRaised',
      _mobile: { gridTemplateColumns: '1fr' }
    },
    rates: { display: 'grid', gap: '0.7rem' },
    rate: {
      display: 'flex',
      justifyContent: 'space-between',
      gap: '1rem',
      paddingBottom: '0.7rem',
      borderBottom: '1px solid var(--colors-market-line)'
    },
    label: { color: 'marketMuted' },
    value: { color: 'marketGoldSoft', textAlign: 'right' },
    note: {
      gridColumn: '1 / -1',
      margin: 0,
      color: 'marketMuted',
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
      border: '1px solid var(--colors-market-line)',
      color: 'marketMuted',
      fontSize: '0.83rem',
      lineHeight: 1.6,
      _mobile: { gridTemplateColumns: '1fr' }
    },
    column: { display: 'grid', gap: '0.25rem' },
    label: { color: 'marketGoldSoft' },
    value: {}
  },
  variants: {},
  defaultVariants: {}
})
