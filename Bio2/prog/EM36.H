/* EM36.H - types used only by the EM36 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM36_H
#define EM36_H

#include "TYPES.H"
#include "MAIN.H"
#include "SPL.H"

/* ENEMY_WORK.Free[] starts 4 bytes later in the retail structure than in EM_SUB.H (two halfwords precede it), so the overlay addresses it through this offset. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares Direction_ck with long parameters; the retail call passes and returns 16-bit values. */
#define Direction_ck	((SHORT (*)())Direction_ck)

#ifdef OVL_37
#define EM_MAIN		Em236
#define EM_PLDM		Pldamege_Em236
#define EM_PLDIE	Pldie_Em236
#else
#define EM_MAIN		Em136
#define EM_PLDM		Pldamege_Em136
#define EM_PLDIE	Pldie_Em136
#endif

/* ---- functions of EM36.C ---- */
/* retail-only resident function 0x8004BF90 (config/retail/main.symbols.txt Sa_morph_trans;
 * references/bio2-ds/Bio2/prog/SABUN.H:29 calls it Sa_morph_trans, a TMD vertex morph).
 * Retail Em37_K passes four values (pEm, pP_ptr, pP_ptr->pTmd, pP_ptr->pSa_dat_head; 0x80103BEC..0x80103BFC). */
extern void Sa_morph_trans();
/* retail-only resident Dual Shock vibration helpers (config/retail/main.symbols.txt Pad_vib_set0
 * 0x8003947C / Pad_vib_set1 0x80039514 / Pad_vib_fade_set 0x800395B8; references/bio2-ds/Bio2/prog/PLAYER.H:266-268). */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();

/* ---- functions of other translation units ----
 * Rnd and the libgte routines are declared; most other external routines are
 * called through an implicit declaration (arguments unnarrowed, implicit int).
 * The implicit declarations are what open the record-less blocks of the retail SYM. */
extern unsigned char Rnd();

extern unsigned long Joint_move();
extern unsigned long Jh_set();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em36_after(ENEMY_WORK *pEm);
static void Em36_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv00_br(ENEMY_WORK *pEm);
static void Em36_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv01_br(ENEMY_WORK *pEm);
static void Em36_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv02_br(ENEMY_WORK *pEm);
static void Em36_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv03_br(ENEMY_WORK *pEm);
static void Em36_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv04_br(ENEMY_WORK *pEm);
static void Em36_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv05_br(ENEMY_WORK *pEm);
static void Em36_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv06_br(ENEMY_WORK *pEm);
static void Em36_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv07_br(ENEMY_WORK *pEm);
static void Em36_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv08_br(ENEMY_WORK *pEm);
static void Em36_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_mv09_br(ENEMY_WORK *pEm);
static void Em36_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_firedm(ENEMY_WORK *pEm);
static void Em36_ryudm(ENEMY_WORK *pEm);
static void Em36_bomdm(ENEMY_WORK *pEm);
static void Em36_spdm(ENEMY_WORK *pEm);
static void Em36_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void EM_PLDIE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em36_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static unsigned long G5a_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top);
static void G5a_Jh_move(ENEMY_WORK *pEm);
static void Em37_K(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_K_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_K_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_K_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_K_mv03(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_K_mv04(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void func_80104B94(ENEMY_WORK *pEm, PARTS_W *pP_ptr);	/* retail-only (no Beta twin, name unknown) */
static void Em36_set_em37_all(unsigned long R);
static void Em36_set_em37_1set(unsigned long No, unsigned long R);
static int Em36_mv_quake(ENEMY_WORK *pEm);
static void Em36_core_set_dest(unsigned char No, short D_x, short D_y);
static void Em36_core_me_mv(ENEMY_WORK *pEm, unsigned char No);
static void Em36_die_wait(ENEMY_WORK *pEm);

#endif /* EM36_H */
