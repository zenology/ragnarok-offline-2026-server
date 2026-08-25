import type { ReactNode } from 'react'

import { pandaStyles } from '../pandaStyles'

import type { BlackMarketContact } from '../../data/blackMarket'

export function ContactCard({ contact }: { contact: BlackMarketContact }): ReactNode {
  return (
    <details className={`contact-card ${pandaStyles.contactCard}`}>
      <summary>
        <span>{contact.city}</span>
        <span className="contact-card__chevron" aria-hidden="true">
          +
        </span>
      </summary>
      <div className="contact-card__details">
        <span>{contact.location}</span>
        <span className="muted">Contact position: {contact.coordinates}</span>
      </div>
    </details>
  )
}
