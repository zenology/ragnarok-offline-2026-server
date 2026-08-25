import {Fragment, type ReactNode} from 'react';

const steps = [
  ['01', 'Farm', 'Find monster cards'],
  ['02', 'Sell', 'Visit the Card Collector'],
  ['03', 'Spend', 'Choose a market service'],
] as const;

export function LoopCard(): ReactNode {
  return (
    <div className="loop-card" aria-label="Cash Point gameplay loop">
      {steps.map(([number, title, description], index) => (
        <Fragment key={number}>
          {index > 0 && <div className="loop-arrow" aria-hidden="true">→</div>}
          <div className="loop-step"><span>{number}</span><strong>{title}</strong><small>{description}</small></div>
        </Fragment>
      ))}
    </div>
  );
}
