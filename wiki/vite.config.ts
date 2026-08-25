import { fileURLToPath } from 'node:url'

import { defineConfig } from 'vite'

export default defineConfig({
  build: {
    outDir: 'build',
    emptyOutDir: true
  },
  server: {
    host: '0.0.0.0',
    watch: {
      usePolling: true,
      interval: 500
    }
  },
  preview: {
    host: '0.0.0.0'
  },
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
      'styled-system': fileURLToPath(new URL('./styled-system', import.meta.url))
    }
  }
})
