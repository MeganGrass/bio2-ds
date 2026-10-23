/* EM2A.H - types used only by the EM2A overlay (retail layout). */
#ifndef EM2A_H
#define EM2A_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "SPL.H"
/* retail callers use the full word Cdir_ck returns; EM_SUB.H declares a UCHAR result, which adds a byte mask */
#define Cdir_ck ((long (*)())Cdir_ck)
/* retail callers take Direction_ck's result as a short and pass short arguments; EM_SUB.H declares long */
#define Direction_ck ((short (*)(short, short, short, short))Direction_ck)

#ifdef OVL_2D
#define Em12a Em22a
#define Em12a_Pldamage Em22a_Pldamage
#endif
extern MATRIX GsIDMATRIX;
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();
/* retail's Free[] starts 4 bytes after the project's ENEMY_WORK.Free (pEm+0x218 against +0x214) */
#define EF(p) ((EM2A_FREE *)((UCHAR *)(p)->Free + 4))
extern unsigned long Jh_set();
extern unsigned char Rnd();
extern void Goto00(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);
extern void Goto01(ENEMY_WORK *pEm, short Dir, short Add_dir);

/* ---- functions of EM2A.C ---- */
void Em12a_Pldamage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_117(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_117_br(ENEMY_WORK *pEm);
static void Em2a_20d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_20d_br(ENEMY_WORK *pEm);
static void Em2a_30a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_30a_br(ENEMY_WORK *pEm);
static void Em2a_507(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_507_br(ENEMY_WORK *pEm);
static void Em2a_602(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_602_br(ENEMY_WORK *pEm);
static void Em2a_604c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_604c_br(ENEMY_WORK *pEm);
static void Em2a_604l(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_604l_br(ENEMY_WORK *pEm);
static void Em2a_612(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_612_br(ENEMY_WORK *pEm);
static void Em2a_att2_l(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att2_l_br(ENEMY_WORK *pEm);
static void Em2a_att2_r(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att2_r_br(ENEMY_WORK *pEm);
static void Em2a_att_c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_c_br(ENEMY_WORK *pEm);
static void Em2a_att_cf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_cf_br(ENEMY_WORK *pEm);
static void Em2a_att_t(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_t_br(ENEMY_WORK *pEm);
static void Em2a_att_w(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_w_br(ENEMY_WORK *pEm);
static void Em2a_cri(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_cri_br(ENEMY_WORK *pEm);
static void Em2a_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_dm_down(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_pl_dm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_pl_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_pl_dm_c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_taiki_br(ENEMY_WORK *pEm);
static void Em2a_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_walk_br(ENEMY_WORK *pEm);
static void Em2a_walkf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_walkf_br(ENEMY_WORK *pEm);

static void Em2a_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em2a_where_br(ENEMY_WORK *pEm);	/* retail-only (from Beta EM2B) */
static void Em2a_where(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

/* ---- functions of EM2A_SUB.C ---- */
/* em2a_sub.c - reconstruction of D:\Bio2\prog\EM2A_SUB.C (enemy 2A support routines:
 * joint-shape setup, attack checks, player damage, damage effects).  Retail EM2A.C
 * #includes this file ahead of its own tables, so em2a.c includes it the same way;
 * em2a_sub.h supplies the shared declarations when it is compiled on its own. */
static void Dm_eff_flame(ENEMY_WORK *pEm);
static void Dm_eff_spark(ENEMY_WORK *pEm);

#endif /* EM2A_H */
