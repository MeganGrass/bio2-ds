#ifndef PL_WEP2_H
#define PL_WEP2_H
/*
 * PL_WEP2.H - Player weapon attack/bow types and init prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PL_WEP2.H
 */


#include "TYPES.H"
#include "PLAYER.H"
#include "PL_WEP.H"

/* ---- types migrated from data.h ---- */
/* Bow attack hit table: for each of the four hit frames, the parts index (HitN_parts) and the ren4
 * index (HitN_ren4) that the hit test checks. */
struct tagBOW_HIT_PARTS {
	UCHAR Hit0_parts;
	UCHAR Hit0_ren4;
	UCHAR Hit1_parts;
	UCHAR Hit1_ren4;
	UCHAR Hit2_parts;
	UCHAR Hit2_ren4;
	UCHAR Hit3_parts;
	UCHAR Hit3_ren4;
};

/* pl_mv05_t2_init and pl_mv05_t2_spr (declared in PL_WEP.H) are the per-weapon t2 routine tables used here */

/* pl_mv05 dispatcher */
VOID pl_mv05(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);

/* t2 weapon init table */
VOID pl_w01_t2_init(ENEMY_WORK *pEm);
VOID pl_w02_t2_init(ENEMY_WORK *pEm);
VOID pl_w03_t2_init(ENEMY_WORK *pEm);
VOID pl_w04_t2_init(ENEMY_WORK *pEm);
VOID pl_w05_t2_init(ENEMY_WORK *pEm);
VOID pl_w06_t2_init(ENEMY_WORK *pEm);
VOID pl_w07_t2_init(ENEMY_WORK *pEm);
VOID pl_w08_t2_init(ENEMY_WORK *pEm);
VOID pl_w09_t2_init(ENEMY_WORK *pEm);
VOID pl_w0a_t2_init(ENEMY_WORK *pEm);
VOID pl_w0b_t2_init(ENEMY_WORK *pEm);
VOID pl_w0c_t2_init(ENEMY_WORK *pEm);
VOID pl_w0d_t2_init(ENEMY_WORK *pEm);
VOID pl_w0e_t2_init(ENEMY_WORK *pEm);
VOID pl_w0f_t2_init(ENEMY_WORK *pEm);
VOID pl_w10_t2_init(ENEMY_WORK *pEm);
VOID pl_w11_t2_init(ENEMY_WORK *pEm);
VOID pl_w12_t2_init(ENEMY_WORK *pEm);

/* t2 weapon move table */
VOID pl_w01_t2_move(ENEMY_WORK *pEm);
VOID pl_w02_t2_move(ENEMY_WORK *pEm);
VOID pl_w03_t2_move(ENEMY_WORK *pEm);
VOID pl_w04_t2_move(ENEMY_WORK *pEm);
VOID pl_w05_t2_move(ENEMY_WORK *pEm);
VOID pl_w06_t2_move(ENEMY_WORK *pEm);
VOID pl_w07_t2_move(ENEMY_WORK *pEm);
VOID pl_w08_t2_move(ENEMY_WORK *pEm);
VOID pl_w09_t2_move(ENEMY_WORK *pEm);
VOID pl_w0a_t2_move(ENEMY_WORK *pEm);
VOID pl_w0b_t2_move(ENEMY_WORK *pEm);
VOID pl_w0c_t2_move(ENEMY_WORK *pEm);
VOID pl_w0d_t2_move(ENEMY_WORK *pEm);
VOID pl_w0e_t2_move(ENEMY_WORK *pEm);
VOID pl_w0f_t2_move(ENEMY_WORK *pEm);
VOID pl_w10_t2_move(ENEMY_WORK *pEm);
VOID pl_w11_t2_move(ENEMY_WORK *pEm);
VOID pl_w12_t2_move(ENEMY_WORK *pEm);

/* pl_mv05 weapon state sub-routines */
VOID pl_mv05_w1_t3(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w1_t4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w1_t5(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_sub(ENEMY_WORK *pEm);

/* bow subsystem */
UCHAR pl_bow_init(ENEMY_WORK *pEm, ULONG No);
UCHAR pl_bow_clr(ENEMY_WORK *pEm);
ENEMY_WORK *Bow_Lim_dir_ck(VECTOR *pV, SHORT Cdir_y, ULONG Lim, SHORT Dir);
VOID pl_bow(ENEMY_WORK *pEm);

/* 3D aiming helpers */
LONG Goto_pro(LONG Tdir, LONG Pdir, LONG Add_dir, SHORT *pDir);
VOID Bow_3D(SVECTOR *pTs, SVECTOR *pRot);
SHORT Rot_3D(LONG Vx, LONG Vz);
ULONG Pos_em_at_ck(VECTOR *pP, SHORT Cdir_y, ATTACK_WORK *pAtw, ULONG W_no);
ULONG Pos_pl_at_ck(VECTOR *pP, ULONG Len, LONG Down_life);
ULONG Bow_goto_parts_no(ENEMY_WORK *pTa, MATRIX *pM, BOW_HIT_PARTS *pBow_t);

/* Dispatch function type for the pl_mv05 tables (ENEMY_WORK from TYPES.H) */
typedef void (*PL_MV05_FN)(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);

/* pl_mv05 dispatch tables (DS addresses in comments) */
extern PL_MV05_FN pl_mv05_w_tbl[20];	/* DS 0x800a6f3c, index (Type & 0xfff) - 1 */
extern PL_MV05_FN pl_mv05_w2_tbl[];	/* DS 0x800a7068, index Routine_2 */
extern PL_MV05_FN pl_mv05_w3_tbl[];	/* DS 0x800a7080, index Routine_2 */
extern PL_MV05_FN pl_mv05_w0_tbl[7];	/* DS 0x800a702c, index Routine_2 */
extern PL_MV05_FN pl_mv05_w1_tbl[6];	/* DS 0x800a7048, index Routine_2 */
extern PL_MV05_FN pl_mv05_w4_tbl[];	/* DS 0x800a7098, index Routine_2 */
extern PL_MV05_FN pl_mv05_w5_tbl[];	/* DS 0x800a70b0, index Routine_2 */

/* pl_mv05 weapon-mode dispatchers */
VOID pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w2_t1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w2_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w2_t4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w3(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w3_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w4_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w5(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
VOID pl_mv05_w5_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);




/* Globals defined in PL_WEP2.C */
	extern ATTACK_WORK3 Pl_w011[5];						// 0x800a63a8
	extern ATTACK_SEQ Pl_w01_s1[7];						// 0x800a6434
	extern ATTACK_SEQ Pl_w01_s0[7];						// 0x800a656c
	extern ATTACK_WORK3 Pl_w012[5];						// 0x800a657c
	extern ATTACK_SEQ Pl_w01_s2[7];						// 0x800a6608
	extern BOW_HIT_PARTS Pl_bow_hit_parts[48];			// 0x800a6b88
	extern UCHAR Pl_weapon_kyan[19][3];					// 0x800a6d78
	extern UCHAR Gun_jtg_bit[5];						// 0x800a6f8c
	extern SHORT Bow_Room509[4];						// 0x800a7060
#endif /* PL_WEP2_H_H */
