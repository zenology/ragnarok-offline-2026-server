import type { ReactNode } from 'react'

import { Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

export default function EdenLevelingPage(): ReactNode {
  return (
    <main aria-label="Eden Group leveling guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · EDEN GROUP"
        title="Eden Group Leveling Quests"
        description="Repeatable EXP missions from the mission boards at Eden Group headquarters."
      />
      <GuideSection
        id="eden-join"
        number="01"
        eyebrow="EDEN GROUP"
        title="Join and find the boards"
      >
        <Text as="p">
          Talk to <strong>Secretary Lime Evenor</strong> at <strong>moc_para01 27/35</strong> to
          join the Eden Group and receive an <strong>Eden Group Mark</strong>. You need the mark to
          read the mission boards on the back wall of headquarters.
        </Text>
      </GuideSection>
      <GuideSection id="eden-1-11" number="02" eyebrow="LEVEL 1–11" title="Mission [1 - 11]">
        <Text as="p">
          Board at <strong>moc_para01 34/38</strong>. Turn in to <strong>Sprakki</strong> at{' '}
          <strong>moc_para01 32/36</strong>. Accept only at Base Level 1–11; you can still turn in
          after you outlevel the band. All five hunts can be active at once. There is no cooldown.
        </Text>
        <Callout>
          <strong>Drops Hunt (60401):</strong> 10 Drops — 220 Base EXP. Optional warp to Morroc
          Field 7.
        </Callout>
        <Callout>
          <strong>Lunatic and Fabre Hunt (60402):</strong> 10 Lunatic, 10 Fabre — 765 Base EXP.
          Optional warp to Prontera fields.
        </Callout>
        <Callout>
          <strong>ChonChon and Pupa Hunt (60403):</strong> 20 Chonchon, 10 Pupa — 1,088 Base EXP.
          Optional warp to Geffen fields.
        </Callout>
        <Callout>
          <strong>Peco Peco Egg Hunt (60404):</strong> 20 Peco Peco Egg — 918 Base EXP. Optional
          warp to Sograt Desert.
        </Callout>
        <Callout>
          <strong>Willow and Picky Hunt (60405):</strong> 20 Willow, 20 Picky — 2,108 Base EXP.
          Optional warp to Payon Forest or Sograt Desert.
        </Callout>
      </GuideSection>
      <GuideSection id="eden-boards" number="03" eyebrow="DIRECTORY" title="Other mission boards">
        <Text as="p">
          <strong>11–25:</strong> Board <strong>36/38</strong>, manager Spike at{' '}
          <strong>32/30</strong>.
        </Text>
        <Text as="p">
          <strong>26–40:</strong> Board <strong>38/38</strong> (hunt, delivery, and gather NPCs
          off-site).
        </Text>
        <Text as="p">
          <strong>41–55:</strong> Board <strong>40/38</strong> (16 hunts including Phen, Orc
          Skeleton, Zenorc, and Mummy).
        </Text>
        <Text as="p">
          <strong>56–70:</strong> Board <strong>42/38</strong>.
        </Text>
        <Text as="p">
          <strong>71–85:</strong> Board <strong>44/38</strong>.
        </Text>
        <Text as="p">
          <strong>86–90 / 91–99:</strong> Boards on the second floor at <strong>48/175</strong> and{' '}
          <strong>48/177</strong>.
        </Text>
        <Text as="p">
          <strong>100+:</strong> Behind the blue access door — Gelkah, Mingmin, Melody-Jack, Aigu,
          and Ragi. Commander Arquien on Eden 2F and in Verus offers Memory Record dailies after the
          Episode 15 story unlock.
        </Text>
      </GuideSection>
      <GuideSection
        id="eden-advanced"
        number="04"
        eyebrow="BASE 130+"
        title="Advanced Requests Board"
      >
        <Text as="p">
          Join the Eden Group and carry an <strong>Eden Group Mark</strong> (Secretary Lime Evenor
          at <strong>moc_para01 27/35</strong>). On Eden HQ 2F, use the{' '}
          <strong>Advanced Requests Board</strong> at <strong>moc_para01 48/179</strong>. You need{' '}
          <strong>Base Level 130+</strong>. Each hunt asks for <strong>30</strong> kills per target.
          Rewards are <strong>EXP only</strong> (no coins or medals). After turn-in, a{' '}
          <strong>4-hour</strong> standby quest blocks the same request until the wait ends. All
          listed hunts below can be active at the same time. Accept and turn in on this board only.
        </Text>
        <Callout>
          <strong>Blooming Flower Land (19000):</strong> 30 Menblatt, 30 Petal —{' '}
          <strong>ecl_fild01</strong> — 1,500,000 Base / 1,000,000 Job EXP.
        </Callout>
        <Callout>
          <strong>Bifrost Tower (19012):</strong> 30 Cenere, 30 Antique Book —{' '}
          <strong>ecl_tdun01</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Prontera Prison (19016):</strong> 30 Taffy, 30 Watcher —{' '}
          <strong>prt_prison</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Invaded Prontera (19018):</strong> 30 Zombie Guard, 30 Immortal Corps —{' '}
          <strong>prt_q</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Rockridge Mine (19020):</strong> 30 Gaster — <strong>rockmi1</strong> — 1,500,000
          / 1,000,000.
        </Callout>
        <Callout>
          <strong>Glastheim Castle F2 (Nightmare) (19022):</strong> 30 Wanderer (Nightmare) —{' '}
          <strong>gl_cas02_</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Underground Bunker (19024):</strong> 30 Smelly Ghoul — <strong>un_bunker</strong>{' '}
          (not Heart Hunter <strong>slabw01</strong>) — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Laboratory-OPTATIO (19026):</strong> 30 Repair Robot Turbo, 30 Green Cenere —{' '}
          <strong>verus01</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Twisted Clock Tower F2 (19028):</strong> 30 Big Ben, 30 Neo Punk —{' '}
          <strong>c_tower2_</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Twisted Clock Tower F3 (19032):</strong> 30 Big Bell — <strong>c_tower3_</strong>{' '}
          — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Od Canyon and Ida Plains (19034):</strong> 30 Ashen Goat, 30 Baby Gray Wolf —{' '}
          <strong>ra_fild10</strong> — requires <strong>Base Level 170+</strong> to accept —
          8,000,000 / 4,000,000.
        </Callout>
        <Callout>
          <strong>Eastern Ruins of Juperos (19038):</strong> 30 Recon Robot —{' '}
          <strong>ver_eju</strong> — 1,500,000 / 1,000,000.
        </Callout>
        <Callout>
          <strong>Verus City (19040):</strong> 30 Explorer Robot, 30 Explorer Robot Turbo —{' '}
          <strong>verus02</strong> and <strong>verus03</strong> — 1,500,000 / 1,000,000.
        </Callout>
      </GuideSection>
      <GuideSection
        id="geffen-petite-daily"
        number="05"
        eyebrow="GEFFEN FIELD 8"
        title="Wind Petite Hunt (Field Watcher Rhea)"
      >
        <Text as="p">
          <strong>Field Watcher Rhea</strong> at <strong>gef_fild08 196/348</strong> offers a field
          daily that is <strong>not</strong> on the Eden Advanced board and does{' '}
          <strong>not</strong> require an Eden Group Mark. <strong>Base Level 70+</strong>. Hunt{' '}
          <strong>30 Wind Petites</strong> (<strong>PETIT_</strong>) on <strong>gef_fild08</strong>{' '}
          only; earth Petites on other Geffen fields do not count. Reward:{' '}
          <strong>50,000 Base / 40,000 Job EXP</strong>. <strong>4-hour</strong> standby after
          turn-in (quest IDs <strong>8596</strong> / <strong>8597</strong>).
        </Text>
      </GuideSection>
    </main>
  )
}
