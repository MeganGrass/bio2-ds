/* EM2D.H - types used only by the EM2D overlay (retail layout). Shared types are in the project headers. */
#ifndef EM2D_H
#define EM2D_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"

/* The generic Free[] area of the overlay ENEMY_WORK begins 4 bytes after the shared ENEMY_WORK.Free (two halfwords precede it in retail). */
#define EM_FREE_AREA(pEm)	((UCHAR *)&(pEm)->Free[1])

#ifdef OVL_30
#define EM_MAIN		Em22d
#define PLDIE_EM	Pldie_Em22d
#define PLDAMEGE_EM	Pldamege_Em22d
#else
#define EM_MAIN		Em12d
#define PLDIE_EM	Pldie_Em12d
#define PLDAMEGE_EM	Pldamege_Em12d
#endif

/* ---- functions of EM2D.C ---- */
extern unsigned long Joint_move();
/* ---- functions of other translation units ----
 * EM2D.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */
/* retail-only resident Dual Shock vibration helpers (shared retail names, config/retail/main.symbols.txt;
 * parameter types from references/bio2-ds/Bio2/prog/PLAYER.H:266-268) */
extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_set1(int Time, unsigned char Power, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);

void EM_MAIN(ENEMY_WORK *pEm);
static void Em2d_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv00_br(ENEMY_WORK *pEm);
static void Em2d_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv01_br(ENEMY_WORK *pEm);
static void Em2d_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv02_br(ENEMY_WORK *pEm);
static void Em2d_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv03_br(ENEMY_WORK *pEm);
static void Em2d_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv04_br(ENEMY_WORK *pEm);
static void Em2d_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv05_br(ENEMY_WORK *pEm);
static void Em2d_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv06_br(ENEMY_WORK *pEm);
static void Em2d_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_mv07_br(ENEMY_WORK *pEm);
static void Em2d_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Hand_display(ENEMY_WORK *pEm, unsigned char L_r, unsigned char On_off);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2d_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

#endif /* EM2D_H */
