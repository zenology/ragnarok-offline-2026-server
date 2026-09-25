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
        description="Access Odin Past, Abyss Lake F4, and Illusion of Labyrinth through their on-map routes."
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
      </GuideSection>
      <GuideSection
        id="abyss-04-hunting"
        number="08"
        eyebrow="ABYSS LAKE F4"
        title="Hunting roster"
      >
        <Text as="p">
          The map hunts six normal monsters: Purple Ferus, Treasure Mimic, Black Acidus, Silver
          Acidus, Bone Ferus, and Bone Acidus. One MVP also spawns here: Bone Detardeurus.
        </Text>
        <Text as="p">Bone Detardeurus respawns after 3 hours, with a 10-minute variance.</Text>
        <Text as="p">Cards from this map use the offline 1% card target.</Text>
      </GuideSection>
      <GuideSection id="abyss-04-dailies" number="09" eyebrow="ABYSS LAKE F4" title="Daily quests">
        <Text as="p">
          <strong>Folklorist Marigold</strong> stands in Hugel Square at{' '}
          <strong>hugel 102/148</strong>. You need Base Level 190. Four independent dailies grant
          EXP only, with a 4-hour standby after each turn-in.
        </Text>
        <Text as="p">
          <strong>A Strangely-colored Ferus:</strong> 20 Purple Ferus. Base 15,823,920 / Job
          11,076,720.
        </Text>
        <Text as="p">
          <strong>Studying the New Mimic:</strong> 10 Treasure Mimic. Base 7,979,360 / Job
          5,585,560.
        </Text>
        <Text as="p">
          <strong>Acidus Elemental Study:</strong> 20 Black Acidus, 20 Silver Acidus. Base
          32,090,640 / Job 22,463,360.
        </Text>
        <Text as="p">
          <strong>Against Providence:</strong> 30 Bone Ferus, 30 Bone Acidus. Base 48,548,160 / Job
          33,983,640.
        </Text>
        <Text as="p">
          Hunt inside <strong>abyss_04</strong>. Use the F3 entrance sign at{' '}
          <strong>abyss_03 91/110</strong>.
        </Text>
      </GuideSection>
      <GuideSection
        id="illusion-labyrinth-access"
        number="10"
        eyebrow="ILLUSION OF LABYRINTH"
        title="Access quest"
      >
        <Text as="p">
          Start at Base Level 170 or higher. Speak with <strong>Irene</strong> at{' '}
          <strong>prt_maze01 97/26</strong>, report to Esmeralda at Prontera Cathedral, then meet
          her beside Irene at <strong>prt_maze01 101/26</strong>.
        </Text>
        <Callout>
          <strong>Find:</strong> Search Navigation for{' '}
          <strong>Illusion of Labyrinth Entrance</strong> to reach the Twisted Crack at{' '}
          <strong>prt_maze01 99/23</strong>.
        </Callout>
        <Text as="p">
          Enter the Twisted Crack and inspect any three of four traces: Andrea 107/104, Anes 10/18,
          Silvano 135/68, and Cecilia 183/26. Return to Esmeralda, then speak with Andrea near the
          crack to accept guest membership. This permanently unlocks the crack and the custom
          warper.
        </Text>
      </GuideSection>
      <GuideSection
        id="illusion-labyrinth-move"
        number="11"
        eyebrow="ILLUSION OF LABYRINTH"
        title="MOVE and return"
      >
        <Callout>
          <strong>MOVE:</strong> Navigation can move you to{' '}
          <strong>Twisted Labyrinth Forest</strong> for 1 Nyangvine or 1 World Tour Ticket. Native
          Private Airship travel has no Base Level or quest requirement and lands on a random
          walkable cell.
        </Callout>
        <Text as="p">
          The exit portal at <strong>prt_mz03_i 182/84</strong> returns you to Labyrinth Forest.
          Daily and weekly Illusion of Labyrinth quests are not included yet.
        </Text>
      </GuideSection>
      <GuideSection
        id="illusion-world-view"
        number="12"
        eyebrow="WORLD VIEW AND MOVE"
        title="Illusion and Abyss Glastheim directory"
      >
        <Text as="p">
          World View now lists the Illusion floors under their normal dungeon dropdowns. Navigation
          MOVE can also reach each listed map for 1 Nyangvine or 1 World Tour Ticket, with no Base
          Level or quest requirement and a random walkable landing cell.
        </Text>
        <Text as="p">
          Payon Cave: <strong>Illusion of Moonlight</strong>. Geffen Dungeon:{' '}
          <strong>Illusion of Vampire</strong>. Ice Cave: <strong>Illusion of Frozen</strong>.
          Turtle Island: <strong>Illusion of Abyss F1</strong> and <strong>F2</strong>. Einbech
          Mine: <strong>Illusion of Teddy Bear</strong>. Beach Cave North:{' '}
          <strong>Illusion of Luanda</strong>.
        </Text>
        <Text as="p">
          Labyrinth Forest: <strong>Illusion of Labyrinth</strong>. Undersea Tunnel:{' '}
          <strong>Illusion of Underwater F1</strong> and <strong>F2</strong>. Ant Hell:{' '}
          <strong>Illusion of Twins</strong>. Glastheim: <strong>Abyss Glastheim Castle F1</strong>{' '}
          and <strong>Abyss Glastheim Castle F2</strong>.
        </Text>
        <Text as="p">
          Additional permanent dungeon floors are also listed under their established World View
          dropdowns: Bifrost Tower F4, Geffenia Dungeon, Rachel Ice Cave - Sealed Space, Somatology
          Laboratory 4th Basement, Twisted Clock Tower F2 and F3, Glastheim Churchyard - Nightmare
          Mode, Tomb of the Fallen, Morroc Pyramid B1 and B2 - Nightmare, Laboratory-OPTATIO,
          Research Building-WISH, and Niflheim Dungeon F1 and F2.
        </Text>
        <Text as="p">
          The World View also has a Rudus Dungeon marker on <strong>ein_fild05</strong>, with
          selectable destinations for Rudus F1 through F4. Private Airship MOVE is available to and
          from all four floors.
        </Text>
        <Text as="p">
          The Illusion and permanent dungeon directory destinations listed above are also available
          through native Private Airship travel. MOVE uses one Nyangvine or World Tour Ticket, has
          no quest or Base Level gate, and lands on a random walkable cell.
        </Text>
        <Callout>
          <strong>Abyss Glastheim is separate from Abyss Lake.</strong> Abyss Lake F4 remains its
          own destination and is not part of this Illusion directory.
        </Callout>
        <Text as="p">
          Abyss Glastheim Castle F2 follows its current roster: Wanderer, Rideword, Mimic, Evil
          Druid, Chimera, Baphomet, and Swift Wanderer Nightmare variants. The old Mysteltainn,
          Alice, and Whisper entries are not part of this floor.
        </Text>
      </GuideSection>
    </main>
  )
}
