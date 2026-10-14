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


struct TRANS {
	POLY_FT4 Prim_poly_ft4[2][384];		// 00000000
	POLY_FT4 * pPoly_ft4_keep;			// 00007800
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
