
import eslint from '@eslint/js'
import prettierPlugin from 'eslint-plugin-prettier/recommended'
import reactPlugin from 'eslint-plugin-react'
import securityPlugin from 'eslint-plugin-security'
import globals from 'globals'
import tseslint from 'typescript-eslint'
import importPlugin from 'eslint-plugin-import-x'
import hookPlugin from 'eslint-plugin-react-hooks'

import {defineConfig} from 'eslint/config'


export default defineConfig(
  eslint.configs.recommended,
  ...tseslint.configs.recommended,
  reactPlugin.configs.flat.recommended,
  reactPlugin.configs.flat['jsx-runtime'],
  importPlugin.flatConfigs.react,
  importPlugin.flatConfigs.typescript,
  hookPlugin.configs.flat.recommended,
  prettierPlugin,
  securityPlugin.configs.recommended,
  {
    files: ['**/*.{js,mjs,cjs}'],
    languageOptions: {
      ecmaVersion: 'latest',
      sourceType: 'module',
      globals: {
        ...globals.node // This defines 'module', 'process', etc.
      }
    }
  },
  {
    settings: {
      react: {
        version: '19'
      }
    }
  },
  {
    files: ['eslint.config.mjs'],
    rules: {
      'import/no-unresolved': 'off'
    }
  },
  {
    rules: {
      'no-var': 'error',
      'no-console': 'warn',
      'no-debugger': 'warn',
      'array-callback-return': 'off',
      'prefer-promise-reject-errors': ['off'],
      complexity: ['error', 15],
      'consistent-return': 'off',
      'default-case': 'error',
      'default-case-last': 'off',
      'no-eval': 'error',
      'eol-last': ['error', 'always'],
      'no-return-await': 'off',
      'padding-line-between-statements': [
        'error',
        {
          blankLine: 'always',
          prev: '*',
          next: 'return'
        }
      ],
      'no-unreachable-loop': 'off',
      'no-loop-func': 'error',
      'no-multi-str': 'error',
      'no-nested-ternary': 'off',
      'no-duplicate-imports': 'error',
      'prefer-const': 'error',
      'prefer-template': 'error',
      'prefer-rest-params': 'error',
      'no-useless-rename': 'error',
      quotes: ['error', 'single'],
      'block-spacing': 'error',
      'arrow-parens': ['error', 'always'],
      'template-curly-spacing': ['error', 'never'],
      'no-unused-vars': 'warn',
      '@typescript-eslint/interface-name-prefix': 'off',
      '@typescript-eslint/explicit-function-return-type': 'off',
      '@typescript-eslint/no-explicit-any': 'off',
      '@typescript-eslint/no-unused-vars': 'warn',
      'react/jsx-filename-extension': ['off'],
      'react/prop-types': 'off',
      'react-hooks/rules-of-hooks': 'error',
      'react-hooks/exhaustive-deps': 'warn',
      'react/display-name': 'off',
      'security/detect-object-injection': ['off'],
      'import-x/no-named-as-default-member': 'off',
      'import-x/first': 'error',
      'import-x/exports-last': 'error',
      'import-x/newline-after-import': 'error',
      'import-x/order': [
        'error',
        {
          groups: ['builtin', 'external', 'internal', 'parent', 'sibling', 'index', 'type'],
          pathGroups: [
            {
              pattern: 'react',
              group: 'builtin',
              position: 'before'
            },
            {
              pattern: 'components/**',
              group: 'parent',
              position: 'after'
            },
            {
              pattern: 'pages/**',
              group: 'parent',
              position: 'after'
            },
            {
              pattern: 'core/**',
              group: 'parent',
              position: 'after'
            }
          ],
          pathGroupsExcludedImportTypes: ['react', 'react-dom'],
          'newlines-between': 'always',
          alphabetize: {
            order: 'asc',
            caseInsensitive: true
          }
        }
      ]
    }
  },
  {
    ignores: ['**/node_modules/**', '**/styled-system/**', '**/dist/**', '**/build/**']
  }
)
