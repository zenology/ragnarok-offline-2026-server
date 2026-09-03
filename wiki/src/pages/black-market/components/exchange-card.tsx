import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'

import { cashPointRules } from '../data/black-market'
import { exchange } from '../styles/recipes'

export function ExchangeCard({ rules }: { rules: typeof cashPointRules }): ReactNode {
  const styles = exchange()

  return (
    <div className={styles.root}>
      <div>
        <Text as="p" kind="kicker" tone="accent">
          Coin Exchange
        </Text>
        <Heading as="h3" level="card">
          Choose the conversion that fits your run
        </Heading>
      </div>
      <div className={styles.rates}>
        <div className={styles.rate}>
          <span className={styles.label}>Buy</span>
          <strong className={styles.value}>{rules.buyRate}</strong>
        </div>
        <div className={styles.rate}>
          <span className={styles.label}>Cash out</span>
          <strong className={styles.value}>{rules.cashOutRate}</strong>
        </div>
        <div className={styles.rate}>
          <span className={styles.label}>Bonus</span>
          <strong className={styles.value}>{rules.bonus}</strong>
        </div>
      </div>
      <Text as="p" tone="muted" kind="small" className={styles.note}>
        You can also spin the optional gacha. Review its displayed odds and stake before confirming.
      </Text>
    </div>
  )
}
