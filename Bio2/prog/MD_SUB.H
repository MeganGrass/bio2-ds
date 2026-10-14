#ifndef MD_SUB_H
#define MD_SUB_H
#include <LIBGS.H>
#include <LIBGTE.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagTMD_MEM {
	ULONG* ver_top;
	ULONG n_ver;
	ULONG* nor_top;
	ULONG n_nor;
	ULONG* prim_top;
	ULONG n_poly;
	ULONG scale;
};

struct tagMODEL_WORK {
	ULONG Attribute;
	ULONG* pTmd;
	ULONG* pPacket;
	ULONG* pTmd2;
	ULONG* pPacket2;
	SHORT M[3][3];
	LONG Pos_x;
	LONG Pos_y;
	LONG Pos_z;
	SHORT Old_x;
	SHORT Old_y;
	SHORT Old_z;
	SHORT Old_x2;
	SHORT Old_y2;
	SHORT Old_z2;
	USHORT dummy00;
	MATRIX Workm;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	USHORT dummy01;
	ULONG Poly_rgb;
	ULONG* pSuper;
};

struct tagRCUT {
	USHORT end_flg;
	USHORT ViewR;
	LONG View_p[3];
	LONG View_r[3];
	ULONG* pSp;
};
/*
 * MD_SUB.H - Model sub-routines: packet preset, GTE sort, mirror, parts management
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */



/* ------------------------------------------------------------------ */
/* Module data                                                          */
/* ------------------------------------------------------------------ */

extern ULONG *m_pl_weapon_parts;	/* DS 0x800d8ce8 */
extern ULONG *m_pl_weapon_tmd;		/* DS 0x800cc1e0 */
extern ULONG *m_pl_weapon_packet;	/* DS 0x800df334 */
extern ULONG *m_pl_w00_tmd;			/* DS 0x800d5220 */
extern ULONG *m_pl_w00_packet;		/* DS 0x800ea248 */
extern ULONG *m_pl_w00_tmd2;		/* DS 0x800d5be0 */
extern ULONG *m_pl_w00_packet2;		/* DS 0x800eae38 */

/* DS 0x8009dc28 - explode distance per frame, [Free[4] >> 15][Free[4] & 0x7fff] */
extern short  Bomb_tbl[2][30];

/* Cross-module data used here (GsWSMATRIX DS 0x800dcba8 / GsIDMATRIX DS 0x8009db44 come from libgs.h) */
extern MATRIX GsMWSMATRIX;			/* DS 0x800e2a90 - mirrored world/screen matrix */

/* ------------------------------------------------------------------ */
/* Function prototypes (DS address order)                               */
/* ------------------------------------------------------------------ */

ULONG  PresetObjectGT3_neo(MODEL_WORK *pMw, ULONG *G_packet, ULONG Flg);			/* 8002cbc4 */
ULONG  PresetObjectGT4_neo(MODEL_WORK *pMw, ULONG *G_packet, ULONG Flg);			/* 8002cd24 */
void   CompM(MATRIX *m0, MATRIX *m1, MATRIX *m2);									/* 8002ce94 */
void   mapModelingData(ULONG *pHead);												/* 8002cfd8 */
void   MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm);							/* 8002d030 */
void SortGT3_lno(TMD_MEM *pT_head, POLY_GT3 *bp, ULONG Cv, ULONG D0); 				/* 8002d0e8 */
void SortGT4_lno(TMD_MEM *pT_head, POLY_GT4 *bp, ULONG Cv, ULONG D0); 				/* 8002d244 */
void   Bomb_Parts_SortGT3(TMD_MEM *pT_head, POLY_GT3 *pPacket, PARTS_W *pP_ptr);	/* 8002d3c8 */
void   Bomb_Parts_SortGT4(TMD_MEM *pT_head, POLY_GT4 *pPacket, PARTS_W *pP_ptr);	/* 8002d718 */
void   Ryu_Parts_SortGT3(TMD_MEM *pT_head, POLY_GT3 *bp, PARTS_W *pP_ptr, ULONG D0);	/* 8002da80 */
void   Ryu_Parts_SortGT4(TMD_MEM *pT_head, POLY_GT4 *bp, PARTS_W *pP_ptr, ULONG D0);	/* 8002ddf0 */
void   mirror_TMD_GT3(ULONG *tmd_top);												/* 8002e1b4 */
void   mirror_PACKET_GT3(ULONG *tmd_top, POLY_GT3 *p_top);							/* 8002e1fc */
void   mirror_TMD_GT4(ULONG *tmd_top);												/* 8002e240 */
void   mirror_PACKET_GT4(ULONG *tmd_top, POLY_GT4 *p_top);							/* 8002e288 */
ULONG  Mirror_model_cp(ENEMY_WORK *pEm, ULONG Mem);									/* 8002e2d8 */
ULONG  Init_mirror_weapon(ENEMY_WORK *pEm, ULONG Mem);								/* 8002e4f8 */
void   Mirror_weapon_cp(ENEMY_WORK *pEm);											/* 8002e548 */
void   Parts_work_cp(ENEMY_WORK *pEm);												/* 8002e788 */
void   Mirror_view(RCUT *pV0, RCUT *pV1, UCHAR flg, short mxz);						/* 8002e93c */
UCHAR  Mirror_in_view(RCUT *pV0, UCHAR flg, VECTOR *pos);							/* 8002ea40 */
void   Mirror_trans(ENEMY_WORK *pEm);												/* 8002eaf8 */
void   Mirror_parts_view_ck(ENEMY_WORK *pEm);										/* 8002ec70 retail only */
void   Mirror_matrix_set(void);														/* 8002ed30 */
void   Late_set(MODEL_WORK *pMw, USHORT Late);										/* 8002eeb4 */

/* Defined in JOINT.C (0x80027160); call site in Mirror_trans verified in asm (4 args). Move to JOINT.H when created. */
void Joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, VECTOR *pPos);


#endif /* MD_SUB_H_H */
