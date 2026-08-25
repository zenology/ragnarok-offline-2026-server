import type {ReactNode} from 'react';
import type {CardPriceBand} from '../../data/blackMarket';
import {pandaStyles} from '../pandaStyles';

export function PriceCard({band}: {band: CardPriceBand}): ReactNode {
  return (
    <div className={`price-card ${pandaStyles.priceCard}`}>
      <span className="price-card__label">{band.catalog}</span>
      <strong>{band.range}</strong>
      <small>{band.source}</small>
    </div>
  );
}
