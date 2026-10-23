/* EM29.H - types used only by the EM29 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM29_H
#define EM29_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"

/* This overlay calls Rnd() as a plain int-returning function (no andi after the jal), while EM_SUB.H declares it UCHAR;
 * calling it through a LONG-returning type reproduces the call without changing the project prototype (see Rnd_s32 in ESP_R.C). */
#define Rnd()	(((LONG (*)(VOID))Rnd)())

#ifdef OVL_2C
#define EM_MAIN		Em229
#define EM_PLDM		Em229_Pldm
#else
#define EM_MAIN		Em129
#define EM_PLDM		Em129_Pldm
#endif

/* ---- functions of EM29.C ---- */
/* retail-only resident Dual Shock vibration helpers (names per docs/RETAIL_MATCHING.md "Shared retail names",
 * from references/bio2-ds/Bio2/prog/PLAYER.H:265-268). Unprototyped: retail passes plain ints. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();
extern void Snd_se_stad();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em29_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_taiki_br(ENEMY_WORK *pEm);
static void Em29_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_run_br(ENEMY_WORK *pEm);
static void Em29_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_fly_br(ENEMY_WORK *pEm);
static void Em29_fly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_hold_br(ENEMY_WORK *pEm);
static void Em29_hold(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_fall_br(ENEMY_WORK *pEm);
static void Em29_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_die_ck(ENEMY_WORK *pEm);
static void Em29_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_die0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_die1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Pl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Pl0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em29_Dir_set(ENEMY_WORK *pEm);
static unsigned long Em29_sel_hold(void);
static unsigned long Dm_eff(ENEMY_WORK *pEm, unsigned long Wep_no);
static unsigned long Dm_eff_disp(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Z);
static unsigned long Dm_eff_explosion(ENEMY_WORK *pEm, PARTS_W *pP_ptr);

#endif /* EM29_H */
