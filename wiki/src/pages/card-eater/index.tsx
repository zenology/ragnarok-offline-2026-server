import { useMemo, useState, type ReactNode } from 'react'

import { Link } from '@tanstack/react-router'

import { css } from 'styled-system/css'

import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { cardEaterCards, type CardEaterCard } from './data/cards'

const tierLabels = ['1–50', '51–99', '100–199', '200+'] as const

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

function searchCards(query: string): CardEaterCard[] {
  const normalizedQuery = normalize(query)

  return cardEaterCards
    .map((card) => ({ card, score: fuzzyScore(card.name, normalizedQuery) }))
    .filter((entry): entry is { card: CardEaterCard; score: number } => entry.score !== undefined)
    .sort(
      (left, right) =>
        left.score - right.score ||
        left.card.name.localeCompare(right.card.name) ||
        left.card.itemId - right.card.itemId
    )
    .map((entry) => entry.card)
}

function rewardText(card: CardEaterCard): string {
  return `Silvervine mode: ${card.silvervineReward} Silvervine · Event Stone Coin mode: ${card.eventStoneCoinReward} Event Stone Coins`
}

function CardGrid({ cards }: { cards: CardEaterCard[] }): ReactNode {
  return (
    <div
      className={css({
        display: 'grid',
        gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
        gap: '12px',
        _mobile: { gridTemplateColumns: '1fr' }
      })}
    >
      {cards.map((card) => (
        <article
          key={card.itemId}
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
            {card.name}
          </h3>
          <p className={css({ margin: 0, color: 'accent.soft', fontSize: '13px' })}>
            Item ID {card.itemId}
          </p>
          <p className={css({ margin: 0, color: 'text.muted', fontSize: '14px', lineHeight: 1.5 })}>
            {rewardText(card)}
          </p>
        </article>
      ))}
    </div>
  )
}

export default function CardEaterPage(): ReactNode {
  const [query, setQuery] = useState('')
  const results = useMemo(() => searchCards(query), [query])
  const resultsByTier = tierLabels.map((label, index) => ({
    label,
    cards: results.filter((card) => card.tier === index + 1)
  }))

  return (
    <main
      aria-label="Card Eater player guide"
      className={css({ minHeight: '100vh', background: 'var(--colors-surface-canvas)' })}
    >
      <HeaderSection
        eyebrow="MALANGDO · PLAYER GUIDE"
        title="Card Eater"
        description="A searchable reference for every Normal monster card accepted by the Malangdo Card Eater."
      />
      <div
        className={css({
          width: 'min(calc(100% - 48px), 1180px)',
          margin: '0 auto',
          paddingBottom: '96px',
          _mobile: { width: 'min(calc(100% - 32px), 1180px)' }
        })}
      >
        <GuideSection id="search" number="01" eyebrow="Find a card" title="Accepted cards">
          <Callout variant="notice">
            The Card Eater accepts 120 loose Normal monster cards. Cards inserted into equipment and
            MVP or miniboss cards are not on this menu.
          </Callout>
          <label
            htmlFor="card-eater-search"
            className={css({
              display: 'grid',
              gap: '8px',
              marginTop: '24px',
              color: 'text.default',
              fontWeight: 600
            })}
          >
            Search by card name
            <input
              id="card-eater-search"
              type="search"
              value={query}
              onChange={(event) => setQuery(event.target.value)}
              placeholder="Try Lunatic or savag babe"
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
            {results.length} of 120 cards shown
          </p>
          {results.length === 0 ? (
            <p role="status">
              No accepted cards match “{query}”. Try a longer part of the card name.
            </p>
          ) : (
            <>
              <div className={css({ display: 'grid', gap: '36px', marginTop: '28px' })}>
                {resultsByTier.map(({ label, cards }) =>
                  cards.length > 0 ? (
                    <section key={label} aria-labelledby={`tier-${label}`}>
                      <div
                        className={css({
                          display: 'flex',
                          justifyContent: 'space-between',
                          alignItems: 'baseline',
                          gap: '16px',
                          marginBottom: '14px'
                        })}
                      >
                        <h2
                          id={`tier-${label}`}
                          className={css({
                            margin: 0,
                            color: 'text.default',
                            fontFamily: 'siteHeading',
                            fontSize: '24px'
                          })}
                        >
                          Card Tier {label}
                        </h2>
                        <span className={css({ color: 'text.muted', fontSize: '14px' })}>
                          {cards.length} cards
                        </span>
                      </div>
                      <CardGrid cards={cards} />
                    </section>
                  ) : null
                )}
              </div>
              <div
                className={css({
                  display: 'none',
                  gridTemplateColumns: 'repeat(3, minmax(0, 1fr))',
                  gap: '12px',
                  _mobile: { gridTemplateColumns: '1fr' }
                })}
              >
                {results.map((card) => (
                  <article
                    key={card.itemId}
                    className={css({
                      display: 'grid',
                      gap: '8px',
                      padding: '18px',
                      border: '1px solid var(--colors-line-default)',
                      borderRadius: '6px',
                      backgroundColor: 'surface.default'
                    })}
                  >
                    <h2
                      className={css({
                        margin: 0,
                        fontFamily: 'siteHeading',
                        color: 'text.default',
                        fontSize: '18px'
                      })}
                    >
                      {card.name}
                    </h2>
                    <p className={css({ margin: 0, color: 'accent.soft', fontSize: '13px' })}>
                      Item ID {card.itemId} · Tier {card.levelBand}
                    </p>
                    <p
                      className={css({
                        margin: 0,
                        color: 'text.muted',
                        fontSize: '14px',
                        lineHeight: 1.5
                      })}
                    >
                      {rewardText(card)}
                    </p>
                  </article>
                ))}
              </div>
            </>
          )}
        </GuideSection>
        <GuideSection id="tiers" number="02" eyebrow="Know the payout" title="Four card tiers">
          <div
            className={css({
              display: 'grid',
              gridTemplateColumns: 'repeat(4, minmax(0, 1fr))',
              gap: '12px',
              _mobile: { gridTemplateColumns: 'repeat(2, minmax(0, 1fr))' }
            })}
          >
            {tierLabels.map((label, index) => (
              <div
                key={label}
                className={css({
                  padding: '16px',
                  border: '1px solid var(--colors-line-default)',
                  borderRadius: '6px',
                  backgroundColor: 'surface.raised'
                })}
              >
                <strong>{label}</strong>
                <p className={css({ margin: '8px 0 0', color: 'text.muted', fontSize: '14px' })}>
                  {cardEaterCards.filter((card) => card.tier === index + 1).length} cards
                </p>
              </div>
            ))}
          </div>
          <p className={css({ color: 'text.muted', marginTop: '24px' })}>
            See the{' '}
            <Link to="/costumes" className={css({ color: 'accent.soft' })}>
              Malangdo Costumes guide
            </Link>{' '}
            for the complete service route.
          </p>
        </GuideSection>
      </div>
    </main>
  )
}
