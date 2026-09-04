export type JobChangeEntry = {
  job: string
  location: string
  npc: string
}

export type JobChangeGroup = {
  title: string
  description: string
  entries: readonly JobChangeEntry[]
}

export const jobChangeGroups: readonly JobChangeGroup[] = [
  {
    title: 'Class 1',
    description: 'The first job change is handled inside each starting job guild.',
    entries: [
      { job: 'Swordman', location: 'izlude_in · 74, 172', npc: 'Swordman' },
      { job: 'Mage', location: 'geffen_in · 164, 124', npc: 'Mage Guildsman' },
      { job: 'Archer', location: 'payon_in02 · 64, 71', npc: 'Archer Guildsman' },
      { job: 'Acolyte', location: 'prt_church · 184, 41', npc: 'Cleric' },
      { job: 'Merchant', location: 'alberta_in · 53, 43', npc: 'Merchant' },
      { job: 'Thief', location: 'moc_prydb1 · 42, 133', npc: 'Thief Guildsman' }
    ]
  },
  {
    title: 'Class 2 and Transcendent Class 2',
    description:
      'Standard second jobs use their class guilds. Transcendent second jobs use the NPCs in the Juno Valkyrie.',
    entries: [
      { job: 'Knight', location: 'prt_in · 88, 101', npc: 'Chivalry Captain' },
      { job: 'Lord Knight', location: 'valkyrie · 44, 39', npc: 'Lord Knight' },
      { job: 'Wizard', location: 'gef_tower · 111, 37', npc: 'Wizard Guildsman' },
      { job: 'High Wizard', location: 'valkyrie · 44, 47', npc: 'High Wizard' },
      { job: 'Hunter', location: 'hu_in01 · 386, 373', npc: 'Hunter Guildsman' },
      { job: 'Sniper', location: 'valkyrie · 44, 55', npc: 'Sniper' },
      { job: 'Priest', location: 'prt_church · 16, 41', npc: 'High Bishop' },
      { job: 'High Priest', location: 'valkyrie · 44, 42', npc: 'High Priest' },
      { job: 'Blacksmith', location: 'ein_in01 · 18, 28', npc: 'Guildsman' },
      { job: 'Whitesmith', location: 'valkyrie · 44, 50', npc: 'MasterSmith' },
      { job: 'Assassin', location: 'in_moc_16 · 19, 33', npc: 'Guildsman' },
      { job: 'Assassin Cross', location: 'valkyrie · 44, 58', npc: 'Assassin Cross' },
      { job: 'Crusader', location: 'prt_church · 95, 127', npc: 'Crusader' },
      { job: 'Paladin', location: 'valkyrie · 53, 39', npc: 'Paladin' },
      { job: 'Monk', location: 'prt_monk · 59, 247', npc: 'Guarding Monk' },
      { job: 'Champion', location: 'valkyrie · 53, 42', npc: 'Champion' },
      { job: 'Sage', location: 'yuno_in02 · 38, 61', npc: 'Dean of the Academy' },
      { job: 'Professor', location: 'valkyrie · 53, 47', npc: 'Scholar' },
      { job: 'Alchemist', location: 'alde_alche · 27, 185', npc: 'Alchemist Guildsman' },
      { job: 'Creator', location: 'valkyrie · 53, 50', npc: 'Biochemist' },
      { job: 'Rogue', location: 'in_rogue · 363, 122', npc: 'Rogue Guildsman' },
      { job: 'Stalker', location: 'valkyrie · 53, 58', npc: 'Stalker' },
      { job: 'Bard', location: 'comodo · 226, 123', npc: 'Wandering Bard' },
      { job: 'Clown', location: 'valkyrie · 53, 54', npc: 'Minstrel' },
      { job: 'Dancer', location: 'comodo · 180, 153', npc: 'Sonotora' },
      { job: 'Gypsy', location: 'valkyrie · 53, 56', npc: 'Gypsy' }
    ]
  },
  {
    title: 'Class 3',
    description: 'Third-job quests begin at the following class locations.',
    entries: [
      { job: 'Rune Knight', location: 'prt_in · 162, 24', npc: 'Splendid-Looking Knight' },
      { job: 'Royal Guard', location: 'prt_cas · 172, 275', npc: 'Middle-aged Gentleman' },
      { job: 'Warlock', location: 'spl_in02 · 77, 107', npc: 'Assistant' },
      { job: 'Sorcerer', location: 'gef_tower · 102, 34', npc: 'Merito' },
      { job: 'Ranger', location: 'tur_dun01 · 156, 36', npc: 'Survival Instructor' },
      { job: 'Minstrel', location: 'alberta · 196, 133', npc: 'Bard' },
      { job: 'Wanderer', location: 'xmas · 162, 209', npc: 'Aspiring Wanderer, Soy' },
      { job: 'Archbishop', location: 'prt_church · 103, 88', npc: 'Praying Minister' },
      { job: 'Sura', location: 've_in · 237, 125', npc: 'King Crab' },
      { job: 'Mechanic', location: 'yuno · 129, 156', npc: 'Chainheart' },
      { job: 'Genetic', location: 'alde_alche · 35, 186', npc: 'Alchemist Union Member' },
      {
        job: 'Guillotine Cross',
        location: 'que_job01 · 75, 96',
        npc: 'Guild Member'
      },
      { job: 'Shadow Chaser', location: 'morocc · 156, 70', npc: 'Girl' }
    ]
  },
  {
    title: 'Expanded paths',
    description:
      'Expanded job changes retain their class-specific quest locations where the local scripts provide them.',
    entries: [
      { job: 'Taekwon', location: 'payon_in01 · 62, 10', npc: 'Phoenix' },
      { job: 'Star Gladiator', location: 'payon · 215, 102', npc: 'Moohyun' },
      { job: 'Soul Linker', location: 'morocc_in · 174, 30', npc: 'Kid' },
      { job: 'Ninja / Kagerou / Oboro', location: 'job_ko · 25, 115', npc: 'Old Man' },
      { job: 'Gunslinger / Rebellion', location: 'que_ng · 152, 167', npc: 'Master Miller' },
      {
        job: 'Super Novice / Expanded Super Novice',
        location: 'aldeba_in · 223, 167',
        npc: 'Tzerero'
      },
      {
        job: 'Spirit Handler',
        location: 'Payon and quest maps',
        npc: 'Dedicated Spirit Handler quest'
      }
    ]
  },
  {
    title: 'Class 4',
    description:
      'The implemented offline fallback is one location for the missing standard and expanded Fourth Job paths.',
    entries: [
      {
        job: 'Dragon Knight, Imperial Guard, Arch Mage, Elemental Master, Windhawk, Troubadour, Trouvere, Cardinal, Inquisitor, Meister, Biolo, Shadow Cross, Abyss Chaser',
        location: 'dali · 65, 113',
        npc: 'Doppelganger (Job Master)'
      },
      {
        job: 'Sky Emperor, Soul Ascetic, Shinkiro, Shiranui, Night Watch, Hyper Novice',
        location: 'dali · 65, 113',
        npc: 'Doppelganger (Job Master)'
      }
    ]
  }
]
