import type { ReactNode } from 'react'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

export default function DungeonsPage(): ReactNode {
  return (
    <main aria-label="Dungeon guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · DUNGEONS"
        title="Ancient Odin Temple"
        description="Enter Odin Past through the portal in Odin Temple's north side."
      />
      <GuideSection id="odin-past-access" number="01" eyebrow="ODIN PAST" title="Odin Past access">
        <Text as="p">
          The Ancient Odin Temple is a late-game dungeon. You need Base Level 180 or higher to use
          its entrance portal.
        </Text>
        <Callout>
          <strong>Where to enter:</strong> Navigate to <strong>odin_tem03 276/236</strong> in Hugel
          Odin Temple North Side. Talk to the portal marked <strong>Entry: Level 180+</strong>, then
          choose <em>Enter Odin Past.</em>
        </Callout>
      </GuideSection>
      <GuideSection
        id="odin-past-return"
        number="02"
        eyebrow="ODIN PAST"
        title="Arrival and return"
      >
        <Text as="p">
          The portal sends you to <strong>odin_past 291/230</strong>. To leave, talk to the exit
          portal at <strong>odin_past 291/235</strong> and choose <em>Return to Odin Temple.</em>
        </Text>
      </GuideSection>
    </main>
  )
}
