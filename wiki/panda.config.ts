import { defineConfig } from '@pandacss/dev'

export default defineConfig({
  presets: ['@shadow-panda/preset'],
  preflight: false,
  jsxFramework: 'react',
  include: ['./src/**/*.{js,jsx,ts,tsx}'],
  theme: {
    extend: {
      tokens: {
        colors: {
          marketBg: { value: '#10110f' },
          marketPanel: { value: '#181a17' },
          marketLine: { value: 'rgba(245, 241, 233, 0.13)' },
          marketInk: { value: '#f5f1e9' },
          marketMuted: { value: '#a9a79f' },
          marketGold: { value: '#e7b96d' },
          marketGoldSoft: { value: '#f2d7a7' },
          marketGreen: { value: '#93b79a' }
        }
      }
    }
  }
})
