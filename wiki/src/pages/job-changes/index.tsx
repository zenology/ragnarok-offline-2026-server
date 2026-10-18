import type { ReactNode } from 'react'

import { Heading, Text } from '@/components/atoms'
import { Callout } from '@/components/molecules'
import { GuideSection, HeaderSection } from '@/components/templates'

import { fourthJobTrials, jobChangeGroups } from './data/job-changes'
import { jobChangesPage } from './styles/recipes'

export default function JobChangesPage(): ReactNode {
  const styles = jobChangesPage()

  return (
    <main className={styles.root} aria-label="Job change player guide">
      <HeaderSection
        eyebrow="RAGNAROK OFFLINE · PLAYER GUIDE"
        title="Where to Change Jobs"
        description="Find your job-change guide and prepare for Doppelganger's Fourth Job awakening trials."
      />
      <div className={styles.content}>
        <Callout variant="notice">
          คลาส 4 ทั้ง 19 อาชีพเริ่มเรื่องกับ Doppelganger ที่ dali · 65, 113 เมื่อถึง Base Level 200
          แล้วไปพบ Valkyrie ที่ valkyrie · 48, 86 เพื่อรับเควสต์ฟรี ต้องมี Job Level 70 ใช้ Skill
          Points ให้หมดและลงจากสัตว์ขี่ รวมถึงถอด Falcon และ Cart ก่อนรับบททดสอบและเปลี่ยนอาชีพ ส่วน
          Spirit Handler ใช้เควสต์เดิมแยกต่างหาก
        </Callout>
        <GuideSection id="awakening-trial" number="01" eyebrow="DOPPELGANGER" title="บททดสอบคลาส 4">
          <Text as="p" tone="muted">
            Doppelganger เป็นวิญญาณที่ติดอยู่ระหว่างมิติ ก่อนเลเวล 200
            เขาจะมีเพียงเสียงพึมพำที่ฟังไม่รู้เรื่องและยังไม่ให้เควสต์ เมื่อถึงเลเวล 200
            เขาจึงสัมผัสได้ถึงวิญญาณอันแข็งแกร่งของเราและขอให้ช่วย ถ้าเคยจุติแล้ว ตัวละครจะนึกถึง
            Valkyrie ผู้เคยนำทางวิญญาณของตนเอง และไปอ่าน Book of Ymir ที่ yuno_in02 · 93, 207
            เพื่อไปขอคำแนะนำจาก Valkyrie ผู้ดูแลการจุติ เธอจะมอบหมายให้รวบรวมวิญญาณจาก MVP
            เพื่อนำไปปลดปล่อยเขา
          </Text>
          <Text as="p" tone="muted">
            ตัวละครที่ไม่เคยจุติ รวมถึงคลาส 3 ที่ข้ามการจุติและ Expanded Class
            จะได้รับขั้นเควสต์ให้ไปถาม Secretary Lime Evenor ที่ Eden Headquarters · moc_para01 ·
            27, 35 ก่อน เลือกเรื่องวิญญาณที่ติดอยู่ระหว่างมิติ เธอจะเล่าเรื่อง Valkyrie
            และแนะนำให้ไปคุยกับ Metheus Sylphe เพื่ออ่าน Book of Ymir คำแนะนำนี้ฟรีและไม่ต้องสมัคร
            Eden จากนั้นขอสิทธิ์อ่านหนังสือได้เมื่อ Base Level 200 ขึ้นไป โดยคุยกับ Metheus Sylphe
            ที่ yuno_in02 · 88, 164 และจ่าย 1,000,000 Zeny ครั้งเดียว
            หากมีสิทธิ์เดิมแล้วไม่ต้องจ่ายซ้ำ ค่าอ่านหนังสือนี้แยกจากค่าพิธี
            ส่วนเส้นทางจุติเดิมยังใช้เงื่อนไขเดิม
          </Text>
          <Text as="p" tone="muted">
            รับเควสต์จาก Valkyrie ฟรี และเริ่มนับ kill หลังรับเควสต์เท่านั้น เมื่อรวบรวมครบ
            ต้องกลับไปหา Valkyrie พร้อมจ่าย 100,000,000 Zeny + Nyangvine (Silvervine Fruit) 100 +
            Cookie (Event Stone Coin) 200 โดยเงินเป็นค่าวัตถุดิบหายากที่เธอจัดหามาใช้ในพิธี
            เธอจะรวมวิญญาณ มีเอฟเฟกต์ที่ตัวเธอก่อน แล้วตามด้วยเอฟเฟกต์ที่ตัวละครขณะรับวิญญาณ Quest
            List จะเปลี่ยนเป็น A Woven Soul จากนั้นนำวิญญาณไปส่ง Doppelganger
            เพื่อปลดปล่อยเขาโดยไม่ต้องจ่ายเงินหรือของเพิ่ม การส่งครั้งนี้ยังไม่เปลี่ยนอาชีพ
          </Text>
          <Text as="p" tone="muted">
            ฆ่าบอสประจำอาชีพ 1 ตัวตามรายการด้านล่าง พร้อมฆ่า MVP อะไรก็ได้ 10 ตัว
            โดยฆ่าชนิดเดิมซ้ำได้ ครั้งแรกที่ฆ่าบอสประจำอาชีพจะเติมเฉพาะเงื่อนไขบอสและไม่เพิ่มยอด 10
            ตัว ฆ่าบอสตัวเดิมครั้งถัดไปจึงนับเข้ายอด 10 ได้ ส่วน MVP
            ตัวอื่นล่าก่อนหรือหลังบอสประจำอาชีพก็ได้
          </Text>
          <Text as="p" tone="muted">
            วิญญาณที่รวบรวมได้จะบันทึกอัตโนมัติในตัวนับทั้งสองส่วนของ Quest List
            ความคืบหน้าเก็บไว้หลังออกเกม เมื่อส่งวิญญาณแล้ว จะเกิดแสงรอบ Doppelganger
            ก่อนที่เขาจะหายไปพร้อมเสียงขอบคุณ จากนั้นอ่าน Book of Ymir อีกครั้งเพื่อกลับไปหา
            Valkyrie อีกครั้งเพื่อรับการปลุกพลังคลาส 4 โดยไม่ต้องจ่ายเพิ่ม มีเอฟเฟกต์ต่อเนื่องประมาณ
            4 วินาที ปิดท้ายด้วยพลุ พร้อมประกาศชื่อผู้เล่นและอาชีพใหม่ทั่วเซิร์ฟเวอร์ การหายตัวของ
            Doppelganger มีผลเฉพาะตัวละครที่ช่วยเขาสำเร็จ
          </Text>
          <Text as="p" tone="muted">
            เมื่อเปลี่ยนอาชีพสำเร็จ ทั้ง 19 อาชีพรวม Expanded Class จะได้รับ Hourglass Necklace 1
            ชิ้น ส่วน Windhawk จะได้รับ Hawk Flute เพิ่มอีก 1 ชิ้นสำหรับเรียกเหยี่ยวด้วย Hawk
            Mastery ต้องมีพื้นที่ในกระเป๋าพอรับของรางวัลก่อนเริ่มพิธี
          </Text>
          <Text as="p" tone="muted">
            หลังเปลี่ยนเป็น Biolo ซื้อ Beaker จาก Gregor ได้ที่ห้องแล็บใน Verus เข้าทางประตูที่
            verus04 · 53, 263 แล้วพบเขาที่ job4_bio · 31, 30 ราคาเริ่มต้น 1,000 Zeny ต่อชิ้น ใช้
            Discount ได้ตามร้านค้าปกติ
          </Text>
          <Text as="p" tone="muted">
            หลังจบพิธี Valkyrie จะกล่าวส่งท้ายและส่งกลับเมืองที่เริ่มเส้นทางอาชีพ: สาย Swordman ไป
            Izlude, Mage ไป Geffen, Archer และ Taekwon ไป Payon, Acolyte ไป Prontera, Merchant ไป
            Alberta, Thief ไป Morroc, Ninja ไป Amatsu, Gunslinger ไป Einbroch และ Super Novice ไป Al
            De Baran ระหว่างทำเควสต์ให้เดินทางไปหาแต่ละคนด้วยตัวเอง
          </Text>
          <Callout variant="notice">
            สำหรับ Cardinal สาย support ที่ต้องการข้ามการล่า ให้คุยกับ Valkyrie
            อีกครั้งหลังรับเควสต์ จ่ายให้เธอครั้งเดียว 300,000,000 Zeny + Nyangvine 100 + Cookie 200
            + Holy Water 500 ครอบคลุมทั้งค่าวัตถุดิบและพิธีศักดิ์สิทธิ์
            เพื่อสร้างวิญญาณศักดิ์สิทธิ์แทนทั้ง Amadarais และ MVP 10 ตัว
            เธอจะสร้างและส่งมอบวิญญาณพร้อมเอฟเฟกต์ที่เธอและตัวละครในครั้งนี้เลย
            แล้วนำวิญญาณนั้นไปส่ง Doppelganger โดยไม่จ่ายเพิ่ม หากเงินหรือของไม่ครบหรือยกเลิก
            จะไม่หักอะไร
            ผู้ที่เคยจ่ายในเวอร์ชันก่อนจะได้รับเครดิตส่วนที่จ่ายแล้วและชำระเฉพาะยอดค้างกับ Valkyrie
          </Callout>
          <div className={styles.grid}>
            {fourthJobTrials.map((trial) => (
              <article className={styles.card} key={trial.job}>
                <Heading as="h3" level="card">
                  {trial.job}
                </Heading>
                <Text as="p" className={styles.location}>
                  {trial.boss}
                </Text>
              </article>
            ))}
          </div>
        </GuideSection>
        {jobChangeGroups.map((group, index) => (
          <GuideSection
            id={group.title.toLowerCase().replace(/ /g, '-')}
            key={group.title}
            number={String(index + 2).padStart(2, '0')}
            eyebrow="Find the right guide"
            title={group.title}
          >
            <Text as="p" tone="muted">
              {group.description}
            </Text>
            <div className={styles.grid}>
              {group.entries.map((entry) => (
                <article className={styles.card} key={entry.job}>
                  <Heading as="h3" level="card">
                    {entry.job}
                  </Heading>
                  <Text as="p" className={styles.location}>
                    {entry.location}
                  </Text>
                  <Text as="p" tone="muted" className={styles.description}>
                    {entry.npc}
                  </Text>
                </article>
              ))}
            </div>
          </GuideSection>
        ))}
      </div>
    </main>
  )
}
