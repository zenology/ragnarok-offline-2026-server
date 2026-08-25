import { css } from 'styled-system/css/css'

// These recipes mirror the existing market CSS values. The semantic class names
// remain in place as a visual fallback while Panda owns reusable component rules.
export const pandaStyles = {
  sectionHeading: css({
    display: 'flex',
    gap: '1.25rem',
    alignItems: 'flex-start',
    marginBottom: '2.25rem'
  }),
  guideSection: css({
    scrollMarginTop: '1.5rem'
  }),
  contactCard: css({
    border: '1px solid',
    borderColor: 'marketLine',
    borderRadius: '0.35rem',
    backgroundColor: 'marketPanel'
  }),
  priceCard: css({
    display: 'grid',
    gap: '0.45rem',
    padding: '1.1rem',
    border: '1px solid',
    borderColor: 'marketLine',
    borderRadius: '0.4rem',
    backgroundColor: 'marketPanel'
  }),
  serviceCard: css({
    padding: '1.35rem',
    border: '1px solid',
    borderColor: 'marketLine',
    borderRadius: '0.4rem',
    backgroundColor: 'marketPanel'
  }),
  itemCard: css({
    display: 'grid',
    gap: '1.5rem',
    padding: '1.25rem',
    border: '1px solid',
    borderColor: 'marketLine',
    borderRadius: '0.4rem',
    backgroundColor: 'marketPanel'
  })
}
