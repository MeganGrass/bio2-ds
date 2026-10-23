/* EM27.H - types used only by the EM27 overlay (retail layout). */
#ifndef EM27_H
#define EM27_H

#include "TYPES.H"

/* Retail calls these through unprototyped declarations (int result, int arguments, no narrowing), so the
 * prototypes of the owner headers are renamed while they are read. */
#define Rnd Rnd_owner
#define Pad_vib_set0 Pad_vib_set0_owner
#define Pad_vib_set1 Pad_vib_set1_owner
#define Pad_vib_fade_set Pad_vib_fade_set_owner
#define Goto00 Goto00_owner
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "OMA.H"
#include "SCA.H"

#undef Rnd
#undef Pad_vib_set0
#undef Pad_vib_set1
#undef Pad_vib_fade_set
#undef Goto00

extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();
#ifdef OVL_2A
#define EM_MAIN		Em227
#define PLDAMEGE_EM	Pldamege_Em227
#define PLDIE_EM	Pldie_Em227
#else
#define EM_MAIN		Em127
#define PLDAMEGE_EM	Pldamege_Em127
#define PLDIE_EM	Pldie_Em127
#endif

/* ---- functions of EM27.C ---- */

void EM_MAIN(ENEMY_WORK *pEm);
static void Em27_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv00_br(ENEMY_WORK *pEm);
static void Em27_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv01_br(ENEMY_WORK *pEm);
static void Em27_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv02_br(ENEMY_WORK *pEm);
static void Em27_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv03_br(ENEMY_WORK *pEm);
static void Em27_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv04_br(ENEMY_WORK *pEm);
static void Em27_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static unsigned long Em27_gacha_ch(void);
static void Em27_mv05_br(ENEMY_WORK *pEm);
static void Em27_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv06_br(ENEMY_WORK *pEm);
static void Em27_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_mv07_br(ENEMY_WORK *pEm);
static void Em27_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_pl_on(ENEMY_WORK *pEm, unsigned long L, short Dir);
static void Em27_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em27_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

/* retail overlay work area is ENEMY_WORK+0x218; project Free is at +0x214 */
#define OVL_FREE(p) ((ULONG *)((UCHAR *)(p)->Free + 4))

#endif /* EM27_H */
