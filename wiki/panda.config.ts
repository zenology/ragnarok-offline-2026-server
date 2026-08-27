import { defineConfig } from '@pandacss/dev'

export default defineConfig({
  presets: ['@shadow-panda/preset'],
  preflight: false,
  jsxFramework: 'react',
  include: ['./src/**/*.{js,jsx,ts,tsx}'],
  hash: false,
  theme: {
    extend: {
      tokens: {
        colors: {
          dark: { value: '#000000' },
          light: { value: '#ffffff' },
          primary: {
            0: { value: '#fffaf0' },
            50: { value: '#fbf0dc' },
            100: { value: '#f7e3bd' },
            200: { value: '#f2d7a7' },
            300: { value: '#edc783' },
            400: { value: '#e7b96d' },
            500: { value: '#c99a52' },
            600: { value: '#a87b3f' },
            700: { value: '#855f31' },
            800: { value: '#604323' },
            900: { value: '#3c2816' }
          },
          secondary: {
            0: { value: '#f4f7ff' },
            50: { value: '#e7edff' },
            100: { value: '#cfd9ff' },
            200: { value: '#adbdff' },
            300: { value: '#86a0ff' },
            400: { value: '#5f83f6' },
            500: { value: '#3f68d4' },
            600: { value: '#3152aa' },
            700: { value: '#273f84' },
            800: { value: '#1e2f62' },
            900: { value: '#15213f' }
          },
          neutral: {
            0: { value: '#f5f1e9' },
            50: { value: '#e9e4da' },
            100: { value: '#d8d2c7' },
            200: { value: '#c3bdb2' },
            300: { value: '#a9a79f' },
            400: { value: '#85857f' },
            500: { value: '#686964' },
            600: { value: '#4b4d49' },
            700: { value: '#20231e' },
            800: { value: '#181a17' },
            900: { value: '#10110f' }
          },
          success: {
            0: { value: '#f2f7f1' },
            50: { value: '#e5efe4' },
            100: { value: '#d2e2d2' },
            200: { value: '#bbd0bd' },
            300: { value: '#a5c2a9' },
            400: { value: '#93b79a' },
            500: { value: '#738f79' },
            600: { value: '#5b7060' },
            700: { value: '#465746' },
            800: { value: '#354134' },
            900: { value: '#242d23' }
          },
          warning: {
            0: { value: '#fbf6e9' },
            50: { value: '#f5eacd' },
            100: { value: '#ead7a8' },
            200: { value: '#dbc080' },
            300: { value: '#c9a55d' },
            400: { value: '#b48a43' },
            500: { value: '#8d6b35' },
            600: { value: '#6f542b' },
            700: { value: '#564122' },
            800: { value: '#403119' },
            900: { value: '#2c2111' }
          },
          error: {
            0: { value: '#fbf2ef' },
            50: { value: '#f3e1db' },
            100: { value: '#e7c6bd' },
            200: { value: '#d7a99d' },
            300: { value: '#c3887a' },
            400: { value: '#ad6d60' },
            500: { value: '#885349' },
            600: { value: '#6c4139' },
            700: { value: '#54332d' },
            800: { value: '#3f2622' },
            900: { value: '#2b1a18' }
          },
          info: {
            0: { value: '#f1f5f5' },
            50: { value: '#e2eaeb' },
            100: { value: '#c8d8d9' },
            200: { value: '#adc5c7' },
            300: { value: '#90aeb1' },
            400: { value: '#77999d' },
            500: { value: '#5c787d' },
            600: { value: '#496066' },
            700: { value: '#394b50' },
            800: { value: '#2c383c' },
            900: { value: '#1e272a' }
          },
          green: {
            0: { value: '#effcf4' },
            50: { value: '#dcf8e7' },
            100: { value: '#b8f1ce' },
            200: { value: '#84e4aa' },
            300: { value: '#4ed084' },
            400: { value: '#2fbf71' },
            500: { value: '#1f9f5d' },
            600: { value: '#197d4a' },
            700: { value: '#145f39' },
            800: { value: '#10472b' },
            900: { value: '#0b2f1d' }
          },
          yellow: {
            0: { value: '#fff8e8' },
            50: { value: '#fff0c2' },
            100: { value: '#ffe18a' },
            200: { value: '#ffc94d' },
            300: { value: '#f7ae28' },
            400: { value: '#e99412' },
            500: { value: '#c6750c' },
            600: { value: '#9d5b0a' },
            700: { value: '#774607' },
            800: { value: '#573304' },
            900: { value: '#382101' }
          },
          red: {
            0: { value: '#fff0ed' },
            50: { value: '#ffdfd9' },
            100: { value: '#ffc2b8' },
            200: { value: '#fa9586' },
            300: { value: '#ef725f' },
            400: { value: '#e45b4f' },
            500: { value: '#c64037' },
            600: { value: '#9e302b' },
            700: { value: '#782520' },
            800: { value: '#571a17' },
            900: { value: '#36100f' }
          },
          blue: {
            0: { value: '#eef8ff' },
            50: { value: '#d9efff' },
            100: { value: '#b8dfff' },
            200: { value: '#85c7fa' },
            300: { value: '#56afef' },
            400: { value: '#2f96e8' },
            500: { value: '#1d78c5' },
            600: { value: '#175fa0' },
            700: { value: '#12497b' },
            800: { value: '#0d355a' },
            900: { value: '#08223a' }
          }
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
      },
      semanticTokens: {
        colors: {
          surface: {
            canvas: { value: '{colors.neutral.900}' },
            default: { value: '{colors.neutral.800}' },
            raised: { value: '{colors.neutral.700}' },
            highlight: { value: 'rgba(245, 241, 233, 0.025)' }
          },
          line: {
            default: { value: 'rgba(245, 241, 233, 0.13)' },
            accent: { value: 'rgba(231, 185, 109, 0.45)' },
            success: { value: 'rgba(147, 183, 154, 0.45)' },
            warning: { value: 'rgba(180, 138, 67, 0.45)' },
            error: { value: 'rgba(173, 109, 96, 0.45)' },
            info: { value: 'rgba(119, 153, 157, 0.45)' }
          },
          text: {
            default: { value: '{colors.neutral.0}' },
            muted: { value: '{colors.neutral.300}' }
          },
          accent: {
            default: { value: '{colors.primary.400}' },
            soft: { value: '{colors.primary.200}' },
            muted: { value: 'rgba(231, 185, 109, 0.05)' },
            subtle: { value: 'rgba(231, 185, 109, 0.07)' },
            emphasis: { value: 'rgba(231, 185, 109, 0.11)' },
            hover: { value: 'rgba(231, 185, 109, 0.15)' }
          },
          status: {
            success: {
              foreground: { value: '{colors.success.400}' },
              subtle: { value: 'rgba(147, 183, 154, 0.14)' }
            },
            warning: {
              foreground: { value: '{colors.warning.400}' },
              subtle: { value: 'rgba(180, 138, 67, 0.14)' }
            },
            error: {
              foreground: { value: '{colors.error.400}' },
              subtle: { value: 'rgba(173, 109, 96, 0.14)' }
            },
            info: {
              foreground: { value: '{colors.info.400}' },
              subtle: { value: 'rgba(119, 153, 157, 0.14)' }
            }
          }
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
