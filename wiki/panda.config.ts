import { defineConfig } from '@pandacss/dev'

export default defineConfig({
  presets: ['@shadow-panda/preset'],
  preflight: false,
  jsxFramework: 'react',
  include: ['./src/**/*.{js,jsx,ts,tsx}'],
  hash: process.env.NODE_ENV === 'production',
  theme: {
    extend: {
      tokens: {
        colors: {
          siteCanvas: { value: '#10110f' },
          siteSurface: { value: '#181a17' },
          siteBorder: { value: 'rgba(245, 241, 233, 0.13)' },
          siteText: { value: '#f5f1e9' },
          siteTextMuted: { value: '#a9a79f' },
          siteAccent: { value: '#e7b96d' },
          siteAccentSoft: { value: '#f2d7a7' },
          siteSuccess: { value: '#93b79a' },
          siteSurfaceRaised: { value: '#20231e' }
        },
        fonts: {
          siteBody: {
            value:
              'Rubik, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, Segoe UI, sans-serif'
          },
          siteHeading: { value: 'Georgia, Times New Roman, serif' }
        },
        sizes: {
          siteContentMax: { value: '1180px' }
        }
      }
    }
  },
  conditions: {
    extend: {
      mobile: '@media (max-width: 760px)'
    }
  }
})
