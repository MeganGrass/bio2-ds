/*
 * OM.H - Object model (OM) work types and color/texture change API
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OM.H
 */
#ifndef OM_H
#define OM_H
#include <LIBGPU.H>
#include "TYPES.H"
#include "OMA.H"

/* ---- types migrated from data.h ---- */
struct tagCC_PARTS_WORK {
	UCHAR Cc_ctr;
	UCHAR Cc_cnt;
	UCHAR Cc_wait;
	UCHAR Cc_num;
	USHORT Cc_pos_x;
	USHORT Cc_pos_y;
	DR_MOVE Cc_dr_move[2];
	RECT Cc_dr_rect[2];
	OBJ_MODEL_WORK* pOm;
};

struct tagCC_WORK {
	CC_PARTS_WORK Cc_parts[30];
	USHORT Ccol_old;
	UCHAR Ccol_no;
	UCHAR Ctex_old;
};

/* Cross-module globals used by OM.C */
/* DS 0x800d6c48 */ extern CC_WORK  Cc_work;
/* DS 0x800dcb98 */ extern SHORT    Om_fall_dir_x;
/* DS 0x800dcba2 */ extern SHORT    Om_fall_dir_z;
/* DS 0x800d7638 */ extern SHORT    Om_fall_old_dir_x;
/* DS 0x800d7642 */ extern SHORT    Om_fall_old_dir_z;

void    Om_init(ULONG Om_no, ULONG *pTmd_adr);                          /* 0x80035710 */
void    Col_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);             /* 0x800357d8 */
void    Tex_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);             /* 0x800358f0 */
void    Tex_ctr(void);                                                  /* 0x80035994 */
void    Col_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);           /* 0x80035ad0 */
void    Tex_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);           /* 0x80035c98 */
void    Om_move(void);                                                  /* 0x80035f68 */
void    Om_trans(void);                                                 /* 0x800363f0 */
void    Om_move_box(OBJ_MODEL_WORK *pOm);                               /* 0x80036710 */
void    Om_set_fall_dir(OBJ_MODEL_WORK *pOm);                           /* 0x80036c5c */
ULONG   Ob_test_load2(void);                                            /* 0x80036cf8 */
void    Oma_set_ofs(OBJ_MODEL_WORK *pOm);                               /* 0x80036e30 */
void    Om_explosion_set(OBJ_MODEL_WORK *pOm);                          /* 0x80036eb4 (undefined in Ghidra) */


#endif /* OM_H_H */
