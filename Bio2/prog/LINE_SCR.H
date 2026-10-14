/*
 * LINE_SCR.H - Screen line and quake effect work area and tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\LINE_SCR.H
 */
#ifndef LINE_SCR_H
#define LINE_SCR_H
#include <LIBGPU.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
// D:\Bio2\prog\LINE_SCR.H


/* ------------------------------------------------------------------ */
/* Structures                                                           */
/* ------------------------------------------------------------------ */

/* LINE_SCR - screen line effect work area (11544 bytes) */
struct tagLINE_SCR {
	/* 0x0000 */ UCHAR      r_no_0;
	/* 0x0001 */ UCHAR      r_no_1;
	/* 0x0002 */ UCHAR      Line_ctr0;
	/* 0x0003 */ UCHAR      Quake_ctr0;
	/* 0x0004 */ ULONG      Keep_tag0;
	/* 0x0008 */ ULONG     *pPrim_keep0;
	/* 0x000c */ ULONG     *pOt;
	/* 0x0010 */ DR_MOVE    Line_move[2][240];	/* 11520 bytes */
	/* 0x2d10 */ ULONG      Null_prim[2];
	/* 0x2d18 */
};

/* ------------------------------------------------------------------ */
/* External data                                                        */
/* ------------------------------------------------------------------ */

extern LINE_SCR Line;			/* 800dfd68 */
extern signed char sin_tbl[256];	/* 8009d9f4 signed byte (lb, unscaled index in Scr_rasta) */
extern UCHAR    quake_tbl1[32];	/* 8009daf4 */
extern UCHAR    quake_tbl2[32];	/* 8009db14 */

/* ------------------------------------------------------------------ */
/* Function prototypes                                                  */
/* ------------------------------------------------------------------ */

VOID Scr_effect(VOID);										/* 8002aa28 */
VOID Scr_quake(ULONG Amp);									/* 8002aa6c */
VOID Scr_rasta(ULONG Amp_x, ULONG Amp_y);					/* 8002ab84 */
VOID line_trans(LONG Line_no, LONG Slide_ofs, LONG Copy_ofs, LONG Draw_ofs);	/* 8002ac94 */
VOID line_scr_end(VOID);									/* 8002addc */
VOID line_mode_set(ULONG Ot_no, ULONG Prio);				/* 8002ae10 */
VOID Line_work_init(VOID);									/* 8002aec8 */


#endif /* LINE_SCR_H_H */
