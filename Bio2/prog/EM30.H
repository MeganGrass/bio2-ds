/* EM30.H - types used only by the EM30 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM30_H
#define EM30_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "OMA.H"
#include "SCA.H"

/* ENEMY_WORK in the project header has Free[] at +0x214; the overlays address it at +0x218 (two halfwords of ENEMY_WORK sit between Kage_ofs and Free[]).
 * The project layout is left alone, so the overlay offsets Free[] by 4 bytes. */
#define EM_FREE(pEm)	((ULONG *)((UCHAR *)(pEm)->Free + 4))

/* The project declares Rnd() as UCHAR; the overlay code uses the full int result in register v0 (no zero-extension after the call). */
#define Rnd()	(((int (*)())Rnd)())

#ifdef OVL_33
#define EM_MAIN		Em230
#define PLDAMEGE_EM	Pldamege_Em230
#define PLDIE_EM	Pldie_Em230
#else
#define EM_MAIN		Em130
#define PLDAMEGE_EM	Pldamege_Em130
#define PLDIE_EM	Pldie_Em130
#endif

/* ---- functions of EM30.C ---- */

void EM_MAIN(ENEMY_WORK *pEm);
static void Em30_after(ENEMY_WORK *pEm);
static void Em30_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Move_type05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv00_br(ENEMY_WORK *pEm);
static void Em30_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv01_br(ENEMY_WORK *pEm);
static void Em30_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv02_br(ENEMY_WORK *pEm);
static void Em30_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv03_br(ENEMY_WORK *pEm);
static void Em30_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv04_br(ENEMY_WORK *pEm);
static void Em30_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv05_br(ENEMY_WORK *pEm);
static void Em30_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv06_br(ENEMY_WORK *pEm);
static void Em30_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv07_br(ENEMY_WORK *pEm);
static void Em30_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv08_br(ENEMY_WORK *pEm);
static void Em30_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv09_br(ENEMY_WORK *pEm);
static void Em30_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv0a_br(ENEMY_WORK *pEm);
static void Em30_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_san(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_san_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_san_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_san_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_mv0b_br(ENEMY_WORK *pEm);
static void Em30_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_mv0c_br(ENEMY_WORK *pEm);
static void Em30_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_firedm(ENEMY_WORK *pEm);
static void Em30_ryudm(ENEMY_WORK *pEm);
static void Em30_bomdm(ENEMY_WORK *pEm);
static void Em30_dm_type17(ENEMY_WORK *pEm);
static void Em30_spdm(ENEMY_WORK *pEm);
static void Em30_mgdm(ENEMY_WORK *pEm);
static void Em30_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em30_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_kuchi_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr, unsigned char *pSeq);
static void Em30_core_me_sa(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_core_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_core_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_core_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_syoku(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em30_hand(ENEMY_WORK *pEm, MATRIX *pM);

#endif /* EM30_H */
