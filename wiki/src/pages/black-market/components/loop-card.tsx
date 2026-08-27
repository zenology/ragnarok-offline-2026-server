import { Fragment, type ReactNode } from 'react'

import { loop } from '../styles/recipes'

const steps = [
  ['01', 'Farm', 'Find monster cards'],
  ['02', 'Sell', 'Visit the Card Collector'],
  ['03', 'Spend', 'Choose a market service']
] as const

export function LoopCard(): ReactNode {
  const styles = loop()

  return (
    <div className={styles.root} aria-label="Cash Point gameplay loop">
      {steps.map(([number, title, description], index) => (
        <Fragment key={number}>
          {index > 0 && (
            <div className={styles.arrow} aria-hidden="true">
              →
            </div>
          )}
          <div className={styles.step}>
            <span className={styles.number}>{number}</span>
            <strong>{title}</strong>
            <small className={styles.small}>{description}</small>
          </div>
        </Fragment>
      ))}
    </div>
  )
}
