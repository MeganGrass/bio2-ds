/* EM31.H - types used only by the EM31 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM31_H
#define EM31_H

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

#ifdef OVL_34
#define EM_MAIN		Em231
#define EM_PLDAMEGE	Pldamege_Em231
#define EM_PLDIE	Pldie_Em231
#else
#define EM_MAIN		Em131
#define EM_PLDAMEGE	Pldamege_Em131
#define EM_PLDIE	Pldie_Em131
#endif

/* ---- functions of EM31.C ---- */
/* retail-only resident Dual Shock vibration helpers (shared retail names, config/retail/main.symbols.txt;
 * parameter types from references/bio2-ds/Bio2/prog/PLAYER.H:266-268) */
extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_set1(int Time, unsigned char Power, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);

void EM_MAIN(ENEMY_WORK *pEm);
void EM_PLDAMEGE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void EM_PLDIE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_after(ENEMY_WORK *pEm);
static void Em31_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Move_type02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv00_br(ENEMY_WORK *pEm);
static void Em31_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv01_br(ENEMY_WORK *pEm);
static void Em31_mv01_br1(ENEMY_WORK *pEm);
static void Em31_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv02_br(ENEMY_WORK *pEm);
static void Em31_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv03_br(ENEMY_WORK *pEm);
static void Em31_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv04_br(ENEMY_WORK *pEm);
static void Em31_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv05_br(ENEMY_WORK *pEm);
static void Em31_mv05_br1(ENEMY_WORK *pEm);
static void Em31_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv06_br(ENEMY_WORK *pEm);
static void Em31_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv07_br(ENEMY_WORK *pEm);
static void Em31_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv08_br(ENEMY_WORK *pEm);
static void Em31_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv09_br(ENEMY_WORK *pEm);
static void Em31_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv0a_br(ENEMY_WORK *pEm);
static void Em31_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv0b_br(ENEMY_WORK *pEm);
static void Em31_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_mv0c_br(ENEMY_WORK *pEm);
static void Em31_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static int Em31_Tume_color(ENEMY_WORK *pEm);
static void Em31_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_firedm(ENEMY_WORK *pEm);
static void Em31_ryudm(ENEMY_WORK *pEm);
static void Em31_bomdm(ENEMY_WORK *pEm);
static void func_80105B7C(ENEMY_WORK *pEm);
static void Em31_spdm(ENEMY_WORK *pEm);
static void Em31_mgdm(ENEMY_WORK *pEm);
static void Em31_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em31_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em31_core_me_sa(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em31_core_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em31_core_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em31_core_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em31_hand(ENEMY_WORK *pEm, MATRIX *pM);
static void Em31_hand_left(ENEMY_WORK *pEm, MATRIX *pM);

#endif /* EM31_H */
