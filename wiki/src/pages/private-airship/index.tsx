import type { ReactNode } from 'react'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { privateAirshipPage } from './styles/recipes'

export default function PrivateAirshipPage(): ReactNode {
  const styles = privateAirshipPage()

  return (
    <main className={styles.root} aria-label="Private Airship guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Private Airship"
        description="This airship checks the original entrance requirements first. The original game does not."
      />
      <div className={styles.content}>
        <GuideSection
          id="airship-difference"
          number="01"
          eyebrow="HOW IT WORKS"
          title="Different from the original game"
        >
          <Text as="p">
            In the original game, Private Airship MOVE does not check whether you are allowed into
            the destination. You press it and you go.
          </Text>
          <Text as="p">
            In this game, MOVE checks first. It asks whether this character has already met every
            original requirement for walking into that place.
          </Text>
          <Text as="p">
            Those requirements are the original ones: the destination&apos;s Base Level, the
            entrance quest stage, or both when the place has both. MOVE does not invent a new quest.
          </Text>
          <Text as="p">
            Open Navigation and choose MOVE. A successful trip still costs one Nyangvine Fruit or
            one World Tour Ticket and lands on a random walkable cell.
          </Text>
          <Text as="p">
            Before the item is taken, the check uses only that original Base Level and
            entrance-quest stage. Both are required when a place has both.
          </Text>
          <Text as="p">
            A zeny fare, a per-visit ticket, a permit, an equipped item, and a party-size
            requirement do not block MOVE.
          </Text>
          <Text as="p">
            If you are not ready, the client shows{' '}
            <strong>This map isn&apos;t available for teleport.</strong> It does not say that your
            level is too low or that a quest is unfinished. The item stays in your inventory and you
            do not move.
          </Text>
          <Text as="p">
            Places with no level or entrance-quest requirement still accept MOVE and still cost the
            item. Abyss Lake floors 1 through 3 are in that group. Floor 4 is not.
          </Text>
        </GuideSection>
        <GuideSection
          id="airship-level-only"
          number="02"
          eyebrow="BASE LEVEL"
          title="Places with a level requirement only"
        >
          <Callout>
            <strong>Abandoned Lab Amicitia, floor 1:</strong> Base Level 215. Floor 2: Base Level
            230.
          </Callout>
          <Callout>
            <strong>Clock Tower Unknown Basement:</strong> Base Level 240.
          </Callout>
          <Callout>
            <strong>Einbech Mine, floor 3:</strong> Base Level 180.
          </Callout>
          <Callout>
            <strong>Nogg Road, floor 3:</strong> Base Level 175.
          </Callout>
          <Callout>
            <strong>Odin Shrine Past:</strong> Base Level 180.
          </Callout>
          <Callout>
            <strong>Nifflheim Dungeon, floor 1:</strong> Base Level 200. Floor 2: Base Level 240.
          </Callout>
          <Callout>
            <strong>Illusion of Frozen:</strong> Base Level 130.
          </Callout>
          <Callout>
            <strong>Luanda, the North Cave:</strong> Base Level 160.
          </Callout>
          <Callout>
            <strong>Desolate Village and Bleak Turtle Palace:</strong> Base Level 150.
          </Callout>
          <Callout>
            <strong>Abyss Lake Underground Cave, floor 4:</strong> Base Level 190.
          </Callout>
        </GuideSection>
        <GuideSection
          id="airship-entrance-quest"
          number="03"
          eyebrow="ENTRANCE QUEST"
          title="Places with an entrance quest"
        >
          <Callout>
            <strong>Cursed Abbey floors 1–3 and Nameless Island:</strong> Nameless Island entrance
            reached, and Base Level 80. The abbey floors open only after the island entrance is
            finished.
          </Callout>
          <Callout>
            <strong>Amatsu Dungeon floor 1:</strong> Amatsu dungeon entrance finished. Floors 2 and
            3: that same entrance, and Base Level 40.
          </Callout>
          <Callout>
            <strong>Illusion of Twins:</strong> its entrance quest started, and Base Level 160.
          </Callout>
          <Callout>
            <strong>Ancient Shrine Maze:</strong> Ayothaya dungeon entrance reached the maze. Inside
            Ancient Shrine: the later shrine step after the maze.
          </Callout>
          <Callout>
            <strong>Brasilis dungeon, both floors:</strong> Brasilis dungeon entrance finished, and
            Base Level 40.
          </Callout>
          <Callout>
            <strong>Krakatoa:</strong> Dewata legend reached the volcano gate, and Base Level 60.
            Istana Cave has no extra gate.
          </Callout>
          <Callout>
            <strong>Kamidal Tunnel:</strong> Sapha&apos;s visit far enough to enter the tunnel, and
            Base Level 70. El Dicastes and Dicastes Diel: far enough to enter the city, and Base
            Level 70. Scaraba Hall and Scaraba Hole: Doha&apos;s secret orders far enough to enter,
            and Base Level 70.
          </Callout>
          <Callout>
            <strong>Eclage:</strong> Eclage entrance finished, and Base Level 120.
          </Callout>
          <Callout>
            <strong>Nasarin Empire:</strong> Illusion of Teddy Bear entrance open, and Base Level
            150.
          </Callout>
          <Callout>
            <strong>250 Pages:</strong> Illusion of Vampire entrance open, and Base Level 130.
          </Callout>
          <Callout>
            <strong>Geffenia, all four floors:</strong> Sign Quest far enough to receive
            Lucifer&apos;s Lament, and Base Level 50.
          </Callout>
          <Callout>
            <strong>Grey Wolf Forest, both fields:</strong> Episode 18 far enough to climb the rope
            from Oz Labyrinth. Grey Wolf Village: Episode 18 far enough for the Camper in the forest
            to take you there. These are earlier than finishing Episode 18.
          </Callout>
          <Callout>
            <strong>
              Issgard, the Ice Castle, and every Issgard field, pit, nest, root, and sanctuary:
            </strong>{' '}
            Episode 18 complete.
          </Callout>
          <Callout>
            <strong>Illusion of Underwater, floor 1:</strong> its entrance open, and Base Level 140.
            Floor 2: the same entrance, and Base Level 180.
          </Callout>
          <Callout>
            <strong>Robot Factory floor 1:</strong> Kiel Hyre far enough to open the first factory
            door, and Base Level 70. Floor 2: Kiel Hyre finished, and Base Level 70.
          </Callout>
          <Callout>
            <strong>Somatology Laboratory floors 1 and 2:</strong> Biolabs entrance finished, and
            Base Level 60. Floor 3: the same entrance, and Base Level 95, or 90 if the character is
            Transcendent. Floor 4: the investigation of the lab&apos;s dangerous rumors finished,
            and Base Level 60.
          </Callout>
          <Callout>
            <strong>Bangungot Hospital floor 1:</strong> the Port Malaya nurse has opened the
            hospital, and Base Level 100.
          </Callout>
          <Callout>
            <strong>Manuk fields 1–3 and Splendide fields 1–3:</strong> New Surroundings advanced,
            Attitude to the New World finished, and Base Level 70. The towns themselves have no
            extra gate.
          </Callout>
          <Callout>
            <strong>Midgard Expedition Camp:</strong> Cat Hand New World or Onward to the New World
            finished, and Base Level 70.
          </Callout>
          <Callout>
            <strong>Flame Cave:</strong> Dimensional Travel finished, and Base Level 140. Flame
            Basin: Dimensional Travel far enough to enter the basin, and Base Level 140.
          </Callout>
          <Callout>
            <strong>Moscovia dungeon floors 1–3:</strong> Finding the Moving Island finished.
          </Callout>
          <Callout>
            <strong>Varmundt&apos;s Mansion areas that share one unlock:</strong> Tartaros Storage,
            both floors, needs Illusion complete, the 17.2 storage unlock, and Base Level 160. The
            bath, Magic Power Plant 1, and Magic Power Plant 2 need Illusion complete, their 17.2
            unlock, and Base Level 130. The library needs Illusion complete, the library unlock, and
            Base Level 130. Farm Lost Valley needs Illusion complete, the farm guide finished,
            Episodes 16.1 and 16.2 finished, and Base Level 130. The mansion garden needs Illusion
            complete, 17.2 started through the mansion, and Base Level 130. Sewage Treatment Plant
            needs Illusion complete, 17.2 started, and Base Level 130.
          </Callout>
          <Callout>
            <strong>Varmundt&apos;s Biosphere, all four maps:</strong> Base Level 240, and Episodes
            16.1, 16.2, 17.1, and 17.2 complete.
          </Callout>
          <Callout>
            <strong>Oz Labyrinth, both floors:</strong> Episodes 16.1, 16.2, 17.1, 17.2, and 18
            complete. No Base Level.
          </Callout>
          <Callout>
            <strong>Twisted Labyrinth Forest:</strong> Base Level 170 and the investigation far
            enough to use the Twisted Crack. Finishing the whole quest is not required. During the
            report step that sends you back to the maze, MOVE refuses this forest.
          </Callout>
          <Callout>
            <strong>Nightmare of Moonlight:</strong> its entrance open, and Base Level 100.
          </Callout>
          <Callout>
            <strong>Prontera Culvert, all four floors:</strong> the culvert entrance quest finished.
            No Base Level.
          </Callout>
          <Callout>
            <strong>Rachel Sanctuary floors 1–5 and Freya&apos;s Grand Temple:</strong> Rachel
            Sanctuary quest finished, and Base Level 60.
          </Callout>
          <Callout>
            <strong>Rudus floors 1–3:</strong> the Illusion quest that opens Rudus finished, and
            Base Level 110. Rudus floor 4: the separate fourth-floor errand accepted. Do not copy
            the floor 1–3 level or quest onto floor 4.
          </Callout>
          <Callout>
            <strong>Thanatos Tower, all twelve maps:</strong> Thanatos Tower quest finished. No Base
            Level.
          </Callout>
          <Callout>
            <strong>Turtle Island, its dungeon, village, palace, and underground swamp:</strong>{' '}
            Turtle Island entrance quest finished. The zeny fare is not a gate.
          </Callout>
          <Callout>
            <strong>Verus Central Plaza and the Excavation Site:</strong> Phantasmagorika far enough
            to enter the plaza, and Base Level 140. Laboratory-OPTATIO and Research Building-WISH:
            the later Phantasmagorika step, and Base Level 140.
          </Callout>
        </GuideSection>
      </div>
    </main>
  )
}
