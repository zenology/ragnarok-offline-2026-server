import type {ReactNode} from 'react';
import {pandaStyles} from '../pandaStyles';

type ServiceCardProps = {
  icon: string;
  location: string;
  title: string;
  description: string;
  tag: string;
  featured?: boolean;
};

export function ServiceCard({icon, location, title, description, tag, featured = false}: ServiceCardProps): ReactNode {
  return (
    <article className={`service-card ${pandaStyles.serviceCard}${featured ? ' service-card--featured' : ''}`}>
      <div className="service-card__top">
        <span className="service-icon">{icon}</span>
        <span className="service-location">{location}</span>
      </div>
      <h3>{title}</h3>
      <p>{description}</p>
      <span className="service-tag">{tag}</span>
    </article>
  );
}
