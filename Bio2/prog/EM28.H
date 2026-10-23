/* EM28.H - types used only by the EM28 overlay (retail layout). */
#ifndef EM28_H
#define EM28_H

#include "TYPES.H"

/* Retail calls these through unprototyped declarations (int result, int arguments, no narrowing), so the
 * prototypes of the owner headers are renamed while they are read. */
#define Rnd Rnd_owner
#define Pad_vib_set0 Pad_vib_set0_owner
#define Pad_vib_set1 Pad_vib_set1_owner
#define Pad_vib_fade_set Pad_vib_fade_set_owner
#define Joint_move Joint_move_owner
#define Joint_move2 Joint_move2_owner

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
#undef Joint_move
#undef Joint_move2

extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();
extern int Joint_move();
extern int Joint_move2();
#ifdef OVL_2B
#define EM_MAIN		Em228
#define PLDAMEGE_EM	Pldamege_Em228
#define PLDIE_EM	Pldie_Em228
#else
#define EM_MAIN		Em128
#define PLDAMEGE_EM	Pldamege_Em128
#define PLDIE_EM	Pldie_Em128
#endif

/* ---- functions of EM28.C ---- */

extern ENEMY_WORK *Set_enemy_work();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em28_after(ENEMY_WORK *pEm);
static void Em28_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv00_br(ENEMY_WORK *pEm);
static void Em28_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv01_br(ENEMY_WORK *pEm);
static void Em28_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv02_br(ENEMY_WORK *pEm);
static void Em28_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv03_br(ENEMY_WORK *pEm);
static void Em28_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv04_br(ENEMY_WORK *pEm);
static void Em28_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv05_br(ENEMY_WORK *pEm);
static void Em28_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv06_br(ENEMY_WORK *pEm);
static void Em28_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv07_br(ENEMY_WORK *pEm);
static void Em28_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv08_br(ENEMY_WORK *pEm);
static void Em28_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_mv09_br(ENEMY_WORK *pEm);
static void Em28_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_firedm(ENEMY_WORK *pEm);
static void Em28_ryudm(ENEMY_WORK *pEm);
static void Em28_bomdm(ENEMY_WORK *pEm);
static void Em28_bom2dm(ENEMY_WORK *pEm);
static void Em28_spdm(ENEMY_WORK *pEm, unsigned char Flg);
static void Em28_mgdm(ENEMY_WORK *pEm);
static void Em28_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_ude(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em28_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
/* Em28_die00 has no prototype: Em28_Die calls it through an implicit declaration
   (which opens the record-less block of Em28_Die in the retail SYM). */
static void Em28_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em28_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);

/* retail overlay work area is ENEMY_WORK+0x218; project Free is at +0x214 */
#define OVL_FREE(p) ((ULONG *)((UCHAR *)(p)->Free + 4))

#endif /* EM28_H */
