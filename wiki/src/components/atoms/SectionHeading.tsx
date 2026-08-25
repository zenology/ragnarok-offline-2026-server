import type {ReactNode} from 'react';
import {pandaStyles} from '../pandaStyles';

type SectionHeadingProps = {
  number: string;
  kicker: string;
  title: string;
  id: string;
};

export function SectionHeading({number, kicker, title, id}: SectionHeadingProps): ReactNode {
  return (
    <div className={`section-heading ${pandaStyles.sectionHeading}`}>
      <span className="section-number">{number}</span>
      <div>
        <p className="section-kicker">{kicker}</p>
        <h2 id={id}>{title}</h2>
      </div>
    </div>
  );
}
