import {defineConfig} from 'vite';
import {fileURLToPath} from 'node:url';

export default defineConfig({
  build: {
    outDir: 'build',
    emptyOutDir: true,
  },
  server: {
    host: '0.0.0.0',
    watch: {
      usePolling: true,
      interval: 500,
    },
  },
  preview: {
    host: '0.0.0.0',
  },
  resolve: {
    alias: {
      '@shadow-panda/styled-system': fileURLToPath(new URL('./@shadow-panda/styled-system', import.meta.url)),
    },
  },
});
