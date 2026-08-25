import { defineConfig } from "@pandacss/dev";

export default defineConfig({
  presets: ["@shadow-panda/preset"],

  // Docusaurus already supplies its global reset through Infima.
  preflight: false,

  jsxFramework: "react",

  // Keep future copy-paste components on Shadow Panda's documented import path.
  emitPackage: true,
  outdir: "@shadow-panda/styled-system",

  // Guide UI code will live under src; no content is authored yet.
  include: ["./src/**/*.{js,jsx,ts,tsx}"],

  // Files to exclude
  exclude: [],

  theme: {
    extend: {},
  },

});
