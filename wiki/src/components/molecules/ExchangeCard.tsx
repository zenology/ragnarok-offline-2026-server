import type {ReactNode} from 'react';
import {cashPointRules} from '../../data/blackMarket';

export function ExchangeCard({rules}: {rules: typeof cashPointRules}): ReactNode {
  return (
    <div className="exchange-card">
      <div>
        <p className="section-kicker">Coin Exchange</p>
        <h3>Choose the conversion that fits your run</h3>
      </div>
      <div className="exchange-rates">
        <div><span>Buy</span><strong>{rules.buyRate}</strong></div>
        <div><span>Cash out</span><strong>{rules.cashOutRate}</strong></div>
        <div><span>Bonus</span><strong>{rules.bonus}</strong></div>
      </div>
      <p className="muted">You can also spin the optional gacha. Review its displayed odds and stake before confirming.</p>
    </div>
  );
}
