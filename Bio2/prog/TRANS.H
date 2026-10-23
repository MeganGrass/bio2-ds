/*
 * TRANS.H - Polygon transform work buffers and trans prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\TRANS.H
 */
#ifndef TRANS_H
#define TRANS_H
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */

struct tagT_WORK {
	ULONG tag;
	UCHAR D0;
	UCHAR D1;
	UCHAR D2;
	UCHAR Code;
	MATRIX M;
};
// D:\Bio2\prog\TRANS.H


/* Polygon transform buffers: two frames of POLY_FT4 quads (double-buffered by Cbuf_id), and the keep
 * pointer that marks the next free quad in the current frame. */
struct TRANS {
	POLY_FT4 Prim_poly_ft4[2][384];		// 00000000
	POLY_FT4 * pPoly_ft4_keep;			// 00007800
	/* Trans+0x7804..0x7807. No retail function reads or writes these bytes (no xrefs at 0x800CBC1C), and the
	 * next retail global starts at Trans+0x7808 (0x800CBC20). Keeps the struct's extent at the retail offset. */
	UCHAR Unknown_tail[4];
};

extern VOID Prim_trans(VOID);
extern VOID Trans_pointer_set(VOID);
extern VOID Eff_sprt_trans(VOID);
extern VOID Esp_trans_main(POLY_FT4 * pP, ESP_WORK * pEff, ULONG Def_size, ULONG nSpr, LONG Projection, ULONG * pOt, ULONG H_clarity, ULONG pG_offset);
extern VOID Esp_trans_3d(POLY_FT4 * pP, ESP_WORK * pEff, ULONG Def_size, ULONG nSpr, LONG Projection, ULONG * pOt, ULONG H_clarity, ULONG pG_offset);
extern VOID Trans_work_init(VOID);
extern TRANS Trans;						// 800C4418		// 30724 bytes
extern ULONG Poly_ft4_count;			// 800AB218


#endif /* TRANS_H */
