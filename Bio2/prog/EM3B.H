/* EM3B.H - types used only by the EM3B overlay (retail layout). Shared types are in the project headers. */
#ifndef EM3B_H
#define EM3B_H

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

#ifdef OVL_3C
#define EM_MAIN		Em23b
#else
#define EM_MAIN		Em13b
#endif

/* ---- functions of EM3B.C ---- */
/* Declared (not implicit): retail SYM opens no implicit-declaration block at their call sites */
extern ENEMY_WORK *Set_enemy_work();
extern void Kill_enemy_work();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em3b_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Init_wall(ENEMY_WORK *pEm, short rx, short ry, short rz);
static void Em3b_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Move_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_move00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_move10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Move_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Move_oya(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_oya00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_oya10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_oya20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Ck_total(ENEMY_WORK *pEm);
static void Em3b_Move_wall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Move_loop(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Move_oya2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_oya200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_mv_oya220(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3b_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static void Em3b_Pl_to_dmg(unsigned long pl, unsigned char dir);
static ENEMY_WORK *Set_child(ENEMY_WORK *pEm, unsigned long Type, long Px, long Pz);

#endif /* EM3B_H */
