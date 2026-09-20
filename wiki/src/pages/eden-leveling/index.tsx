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
    </main>
  )
}
