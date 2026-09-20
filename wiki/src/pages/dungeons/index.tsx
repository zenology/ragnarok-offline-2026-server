import type { ReactNode } from 'react'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

export default function DungeonsPage(): ReactNode {
  return (
    <main aria-label="Dungeon guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · DUNGEONS"
        title="Late-game dungeons"
        description="Enter Odin Past and Abyss Lake F4 from their on-map portals and signs."
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
      <GuideSection
        id="abyss-04-access"
        number="05"
        eyebrow="ABYSS LAKE F4"
        title="Abyss Lake F4 access"
      >
        <Text as="p">
          Abyss Lake Underground Cave F4 requires Base Level 190 or higher to use the F3 entrance
          sign.
        </Text>
        <Callout>
          <strong>Find:</strong> Search Navigation for <strong>Abyss Lake F4 Entrance</strong>. Find
          guides you to the sign at <strong>abyss_03 91/110</strong>. Talk to the sign marked{' '}
          <strong>Entry: Level 190+</strong>, then choose <em>Enter Abyss Lake F4.</em>
        </Callout>
      </GuideSection>
      <GuideSection id="abyss-04-move" number="06" eyebrow="ABYSS LAKE F4" title="MOVE">
        <Callout>
          <strong>MOVE:</strong> Select <strong>Abyss Lake Underground Cave F4</strong> in
          Navigation to warp to a random walkable point in <strong>abyss_04</strong> for{' '}
          <strong>1 Nyangvine</strong>. MOVE has no Base Level 190 requirement and is available only
          from maps that support Private Airship travel.
        </Callout>
      </GuideSection>
      <GuideSection
        id="abyss-04-return"
        number="07"
        eyebrow="ABYSS LAKE F4"
        title="Arrival and return"
      >
        <Text as="p">
          The sign sends you to <strong>abyss_04 169/159</strong>. To leave, talk to the exit sign
          at <strong>abyss_04 169/164</strong> marked <strong>Exit</strong> and choose{' '}
          <em>Return to Abyss Lake F3.</em> That returns you to <strong>abyss_03 97/104</strong>.
        </Text>
        <Text as="p">This pass does not spawn monsters on F4.</Text>
      </GuideSection>
    </main>
  )
}
