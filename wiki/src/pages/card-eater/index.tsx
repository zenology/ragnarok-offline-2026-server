import { useMemo, useState, type ReactNode } from 'react'

import { Link } from '@tanstack/react-router'

import { css } from 'styled-system/css'

import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { treasureEaterItems, type TreasureEaterItem } from './data/cards'

const groupLabels = ['Iron I–II', 'Iron III–V', 'Silver I–II', 'Silver III–V', 'Gold I–V'] as const

function normalize(value: string): string {
  return value.toLocaleLowerCase().trim().replace(/\s+/g, ' ')
}

function distance(left: string, right: string): number {
  const row = Array.from({ length: right.length + 1 }, (_, index) => index)
  for (let i = 1; i <= left.length; i += 1) {
    let diagonal = row[0]
    row[0] = i
    for (let j = 1; j <= right.length; j += 1) {
      const above = row[j]
      row[j] = Math.min(
        row[j] + 1,
        row[j - 1] + 1,
        diagonal + (left[i - 1] === right[j - 1] ? 0 : 1)
      )
      diagonal = above
    }
  }

  return row[right.length]
}

function fuzzyScore(name: string, query: string): number | undefined {
  const normalizedName = normalize(name)
  if (!query) return 0
  if (normalizedName.includes(query)) return normalizedName.startsWith(query) ? 10 : 20
  const queryTokens = query.split(' ')
  const nameTokens = normalizedName.split(' ')
  let score = 30
  for (const queryToken of queryTokens) {
    if (queryToken.length < 4) return undefined
    const limit = queryToken.length >= 8 ? 2 : 1
    const best = Math.min(...nameTokens.map((token) => distance(queryToken, token)))
    if (best > limit) return undefined
    score += best
  }

  return score
}

function searchTreasures(query: string): TreasureEaterItem[] {
  const normalizedQuery = normalize(query)
  const numericQuery = /^\d+$/.test(normalizedQuery)

  return treasureEaterItems
    .filter((item) =>
      numericQuery
        ? String(item.itemId) === normalizedQuery
        : fuzzyScore(item.name, normalizedQuery) !== undefined
    )
    .sort((left, right) => left.name.localeCompare(right.name) || left.itemId - right.itemId)
}

function TreasureGrid({ items }: { items: TreasureEaterItem[] }): ReactNode {
  return (
    <div
      className={css({
        display: 'grid',
        gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
        gap: '12px',
        _mobile: { gridTemplateColumns: '1fr' }
      })}
    >
      {items.map((item) => (
        <article
          key={item.itemId}
          className={css({
            display: 'grid',
            gap: '8px',
            padding: '18px',
            border: '1px solid var(--colors-line-default)',
            borderRadius: '6px',
            backgroundColor: 'surface.default'
          })}
        >
          <h3
            className={css({
              margin: 0,
              fontFamily: 'siteHeading',
              color: 'text.default',
              fontSize: '18px'
            })}
          >
            {item.name}
          </h3>
          <p className={css({ margin: 0, color: 'accent.soft', fontSize: '13px' })}>
            Item ID {item.itemId}
          </p>
          <p className={css({ margin: 0, color: 'text.muted', fontSize: '14px', lineHeight: 1.5 })}>
            Per box: {item.silvervineReward} Silvervine Fruit or {item.eventStoneCoinReward} Event
            Stone Coins (Cookie).
          </p>
        </article>
      ))}
    </div>
  )
}

export default function CardEaterPage(): ReactNode {
  const [query, setQuery] = useState('')
  const hasQuery = normalize(query).length > 0
  const results = useMemo(
    () =>
      hasQuery
        ? searchTreasures(query)
        : [...treasureEaterItems].sort((left, right) => left.itemId - right.itemId),
    [hasQuery, query]
  )
  const resultsByGroup = groupLabels.map((label, index) => ({
    label,
    items: results.filter((item) => item.group === index + 1)
  }))

  return (
    <main
      aria-label="Treasure Eater player guide"
      className={css({
        minHeight: '100vh',
        background: 'var(--colors-surface-canvas)',
        '& > header': { boxSizing: 'border-box' }
      })}
    >
      <HeaderSection
        eyebrow="MALANGDO · PLAYER GUIDE"
        title="Moth / Treasure Eater"
        description="Find every Iron, Silver, and Gold Treasure Box on Moth's menu and its reward per box."
      />
      <div className={css({ maxWidth: '1120px', margin: '0 auto', padding: '0 24px 96px' })}>
        <GuideSection id="search" number="01" eyebrow="Find a treasure" title="Accepted treasures">
          <Callout variant="notice">
            Moth accepts 15 Iron, Silver, and Gold Treasure Boxes. Cards, Diamond and Platinum
            boxes, and all other chests are not accepted.
          </Callout>
          <label
            htmlFor="treasure-eater-search"
            className={css({
              display: 'grid',
              gap: '8px',
              marginTop: '24px',
              color: 'text.default',
              fontWeight: 600
            })}
          >
            Search by treasure name or exact item ID
            <input
              id="treasure-eater-search"
              type="search"
              value={query}
              onChange={(event) => setQuery(event.target.value)}
              placeholder="Try Silver, tresure, or 9000003"
              className={css({
                minHeight: '48px',
                padding: '0 14px',
                border: '1px solid var(--colors-line-default)',
                borderRadius: '6px',
                backgroundColor: 'surface.default',
                color: 'text.default',
                font: 'inherit',
                _focusVisible: {
                  outline: '2px solid',
                  outlineColor: 'accent.default',
                  outlineOffset: '2px'
                }
              })}
            />
          </label>
          <button
            type="button"
            onClick={() => setQuery('')}
            disabled={!query}
            className={css({
              marginTop: '10px',
              border: '0',
              background: 'transparent',
              color: 'accent.soft',
              cursor: 'pointer',
              padding: '4px 0',
              _disabled: { cursor: 'default', opacity: 0.5 }
            })}
          >
            Clear search
          </button>
          <p aria-live="polite" className={css({ color: 'text.muted', margin: '18px 0' })}>
            {results.length} of 15 treasures shown
          </p>
          {results.length === 0 ? (
            <p role="status">
              No accepted treasures match “{query}”. Try a longer part of the treasure name or an
              exact item ID.
            </p>
          ) : hasQuery ? (
            <div className={css({ marginTop: '28px' })}>
              <TreasureGrid items={results} />
            </div>
          ) : (
            <div className={css({ display: 'grid', gap: '36px', marginTop: '28px' })}>
              {resultsByGroup.map(({ label, items }, index) => (
                <section key={label} aria-labelledby={`group-${index + 1}`}>
                  <h2
                    id={`group-${index + 1}`}
                    className={css({
                      margin: '0 0 14px',
                      color: 'text.default',
                      fontFamily: 'siteHeading',
                      fontSize: '24px'
                    })}
                  >
                    {label}
                  </h2>
                  <TreasureGrid items={items} />
                </section>
              ))}
            </div>
          )}
        </GuideSection>
        <GuideSection id="exchange" number="02" eyebrow="Feed Moth" title="One reward per exchange">
          <p>
            Moth waits in Malangdo at 138, 140, under the Silvervine Exchange sign. He eats the coin
            bugs hiding among the treasure inside your boxes.
          </p>
          <p>
            Choose one treasure type and a quantity, or feed every accepted treasure in your
            inventory. Pick Silvervine Fruit or Cookie (Event Stone Coin), review the total, then
            confirm.
          </p>
          <p>
            Rewards are fixed by treasure group. There is no random payout or player-level bonus,
            and one exchange never grants both currencies. Feeding does not advance Costume Roulette
            pity.
          </p>
          <p>
            Canceling, missing boxes, or insufficient reward capacity spends nothing. Once confirmed
            successfully, the boxes are consumed and cannot also be sold for Zeny or exchanged for
            Black Market Points.
          </p>
          <p className={css({ color: 'text.muted', marginTop: '24px' })}>
            See the{' '}
            <Link to="/costumes" className={css({ color: 'accent.soft' })}>
              Malangdo Costumes guide
            </Link>{' '}
            for places to spend your rewards.
          </p>
        </GuideSection>
      </div>
    </main>
  )
}
