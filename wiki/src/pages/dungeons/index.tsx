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
          <strong>Find:</strong> Search for <strong>Odin Past Entrance</strong> in Navigation. Find
          guides you to the portal at <strong>odin_tem03 276/236</strong> in Hugel Odin Temple North
          Side. Talk to the portal marked <strong>Entry: Level 180+</strong>, then choose{' '}
          <em>Enter Odin Past.</em>
        </Callout>
        <Callout>
          <strong>MOVE:</strong> Select <strong>Ancient Odin Temple</strong> in Navigation to warp
          to a random walkable point in <strong>odin_past</strong> for <strong>1 Nyangvine</strong>.
          MOVE has no Base Level 180 requirement and is available only from maps that support
          Private Airship travel.
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
      <GuideSection id="odin-past-hunting" number="03" eyebrow="ODIN PAST" title="Hunting roster">
        <Text as="p">
          The map hunts six normal monsters: two Angelgolt kinds, Holy Frus, Holy Skogul, Arch
          Plasma, and Spectral Plasma. Two MVPs also spawn here: Valkyrie Reginleif and Valkyrie
          Ingrid.
        </Text>
        <Text as="p">
          Reginleif and Ingrid are independent MVP spawns of about 8 hours, each with a 10-minute
          variance. Killing one does not reset the other.
        </Text>
        <Text as="p">Cards from this map use the offline 1% card target.</Text>
      </GuideSection>
      <GuideSection id="odin-past-dailies" number="04" eyebrow="ODIN PAST" title="Daily quests">
        <Text as="p">
          <strong>Folklorist Cinnamon</strong> sits at the Odin Temple ferry,{' '}
          <strong>odin_tem01 108/152</strong>. You need Base Level 180. Two independent dailies
          grant EXP only, with a 4-hour standby after each turn-in.
        </Text>
        <Text as="p">
          <strong>Xth Century XXgolt:</strong> 30 grey Angelgolt, 40 pink Angelgolt. Base 50,190,280
          / Job 35,133,280.
        </Text>
        <Text as="p">
          <strong>What is Valkyrie?:</strong> 30 Holy Frus, 30 Holy Skogul. Base 44,210,160 / Job
          30,947,160.
        </Text>
        <Text as="p">
          Hunt inside <strong>odin_past</strong>. Use the north portal on Odin Temple 3F at{' '}
          <strong>odin_tem03 276/236</strong>.
        </Text>
      </GuideSection>
    </main>
  )
}
