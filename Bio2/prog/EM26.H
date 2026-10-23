/* EM26.H - types used only by the EM26 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM26_H
#define EM26_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"

/* The generic Free[] area of the overlay ENEMY_WORK begins 4 bytes after the shared ENEMY_WORK.Free (two halfwords precede it in retail). */
#define EM_FREE_AREA(pEm)	((UCHAR *)&(pEm)->Free[1])

#ifdef OVL_29
#define EM_MAIN		Em226
#define PLDAMAGE_EM	Pldamage_Em226
#else
#define EM_MAIN		Em126
#define PLDAMAGE_EM	Pldamage_Em126
#endif

/* ---- functions of EM26.C ---- */
/* retail-only resident Dual Shock vibration helpers (names per docs/RETAIL_MATCHING.md "Shared retail names",
 * from references/bio2-ds/Bio2/prog/PLAYER.H:265-268). Unprototyped: retail passes plain ints. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();

/* Spl_life_down is the one external routine EM26.C sees declared: retail Em26_atck_ck has
 * no block for the unbraced else-arm that calls it (an implicit declaration would open one).
 * The declaration's presence is inferred from that block structure; its exact spelling is not
 * recorded (unprototyped here, so the call's code is unaffected). */
extern unsigned long Spl_life_down();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em26_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_walk00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_walk10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_jump(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Mv_swim(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Die10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em26_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMAGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static unsigned long Em26_atck_ck(ENEMY_WORK *pEm, unsigned long pl);
static void Set_blood(ENEMY_WORK *pTg, unsigned char parts, unsigned char type);
static void Em26_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir);
static long Get_sc_hit(ENEMY_WORK *pEm);

#endif /* EM26_H */
