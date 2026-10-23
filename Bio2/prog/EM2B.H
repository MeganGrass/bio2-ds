/* EM2B.H - types used only by the EM2B overlay (retail layout). */
#ifndef EM2B_H
#define EM2B_H

#include "TYPES.H"
#include "ROOM.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "MAIN.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "PLAYER.H"
#include "SCA.H"
#include "SND.H"
#include "SUB.H"
#include "flag.h"

/* the overlay calls Direction_ck with halfword arguments and result (retail narrows at the caller); the project prototype uses long */
/* retail calls Rot_vector without a prototype: the direction is passed as an int, not narrowed to SHORT */
#define Rot_vector ((void (*)())Rot_vector)
/* retail calls Rnd without a prototype: no narrowing of the result at the call site */
#define Rnd ((unsigned char (*)())Rnd)
/* the project prototype returns UCHAR; the overlay calls Cdir_ck without a prototype */
#define Cdir_ck ((int (*)())Cdir_ck)
/* the project prototype returns ULONG; the overlay calls Gacha_ch without a prototype */
#define Gacha_ch ((int (*)())Gacha_ch)
#define Direction_ck ((short (*)(short, short, short, short))Direction_ck)

extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_set1(int Time, unsigned char Power, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);
#ifdef OVL_2E
#define Em12b Em22b
#define Em12b_Pldm Em22b_Pldm
#endif
extern MATRIX GsIDMATRIX;
extern short Bomb_tbl[2][30];
#define EF(p) ((EM2B_FREE *)((char *)(p)->Free + 4))	/* retail Free[] is at +0x218, 4 bytes behind the project member */
extern void Em_seq_sound(ENEMY_WORK *pEm);
extern void Oba_ck_em(ENEMY_WORK *pEm);
extern void Sca_ck_em(ENEMY_WORK *pEm, unsigned long Sc_id);
extern void Em_dead_flg_on(ENEMY_WORK *pEm);
extern UCHAR Esp_call2(ULONG bit, ULONG dir_y_id2, MATRIX *pmat, SVECTOR *svec);

/* ---- functions of EM2B.C ---- */
void Em12b_Pldm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em2b_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_taiki_br(ENEMY_WORK *pEm);
static void Em2b_walk_br(ENEMY_WORK *pEm);
static void Em2a_att_t_br(ENEMY_WORK *pEm);
static void Em2a_att_w_br(ENEMY_WORK *pEm);
static void Em2b_dash_br(ENEMY_WORK *pEm);
static void Em2a_att_t2_br(ENEMY_WORK *pEm);
static void Em2a_where_br(ENEMY_WORK *pEm);
static void Em2a_critical_br(ENEMY_WORK *pEm);
static void Em2a_opening_br(ENEMY_WORK *pEm);
static void Em2b_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_t(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_w(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_dash(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_att_t2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_where(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_opening(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_pl_die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_pl_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2b_pl_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

/* ---- functions of EM2B_SUB.C ---- */
/* em2b_sub.c - reconstruction of D:\Bio2\prog\EM2B_SUB.C (enemy 2B support routines:
 * flying parts, attack checks, player damage, damage effects).  Retail EM2B.C #includes
 * this file ahead of its own tables and functions (one object, two SLD files), so em2b.c
 * includes it the same way; em2b_sub.h supplies the shared declarations. */
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b);

#endif /* EM2B_H */
