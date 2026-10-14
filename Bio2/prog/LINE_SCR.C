/*
 * LINE_SCR.C - Line-scroll screen effects (quake and rasta wave)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\LINE_SCR.C
 */
#include "TYPES.H"
#include "CONFIG.H"
#include <ASM.H>
#include <LIBGPU.H>
#include "LINE_SCR.h"
#include "MAIN.H"
#include "SCE1.H"
#include "flag.h"


/* DS 0x800dfd68 (Line) */
LINE_SCR Line;

/* Scr_rasta state: sinpuku, hacho and speed (DS 0x8009db34..0x8009db3c) */
static LONG   sinpuku;	/* DS 0x8009db34: divisor for sin_tbl */

/* DS 0x8009d9f4 (sin_tbl) */
/* signed char: DS reads it with lb; plain CHAR is unsigned in CCPSX */
signed char sin_tbl[256] = {
	3, 6, 9, 12, 15, 18, 21, 24, 28, 31, 34, 37, 40, 43, 46, 48,
	51, 54, 57, 60, 63, 65, 68, 71, 73, 76, 78, 81, 83, 85, 88, 90,
	92, 94, 96, 98, 100, 102, 104, 106, 108, 109, 111, 112, 114, 115, 117, 118,
	119, 120, 121, 122, 123, 124, 124, 125, 126, 126, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 126, 126, 125, 124, 124, 123, 122, 121, 120, 119, 118,
	117, 115, 114, 112, 111, 109, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90,
	88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 57, 54, 51, 48,
	46, 43, 40, 37, 34, 31, 28, 24, 21, 18, 15, 12, 9, 6, 3, 0,
	-3, -6, -9, -12, -15, -18, -21, -24, -28, -31, -34, -37, -40, -43, -46, -48,
	-51, -54, -57, -60, -63, -65, -68, -71, -73, -76, -78, -81, -83, -85, -88, -90,
	-92, -94, -96, -98, -100, -102, -104, -106, -108, -109, -111, -112, -114, -115, -117, -118,
	-119, -120, -121, -122, -123, -124, -124, -125, -126, -126, -127, -127, -127, -127, -127, -127,
	-127, -127, -127, -127, -127, -126, -126, -125, -124, -124, -123, -122, -121, -120, -119, -118,
	-117, -115, -114, -112, -111, -109, -108, -106, -104, -102, -100, -98, -96, -94, -92, -90,
	-88, -85, -83, -81, -78, -76, -73, -71, -68, -65, -63, -60, -57, -54, -51, -48,
	-46, -43, -40, -37, -34, -31, -28, -24, -21, -18, -15, -12, -9, -6, -3, 0
};

/* DS 0x8009daf4 (quake_tbl1) */
UCHAR quake_tbl1[32] = {
	0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0
};

/* DS 0x8009db14 (quake_tbl2) */
UCHAR quake_tbl2[32] = {
	0, 2, 0, 0, 0, 2, 0, 3, 1, 0, 0, 1, 0, 2, 0, 0,
	0, 2, 0, 0, 2, 1, 0, 2, 0, 2, 0, 0, 1, 1, 0, 2
};
static LONG   hacho;	/* DS 0x8009db38: phase step per line */
static LONG   speed;	/* DS 0x8009db3c: phase advance per call */

/* DS 0x8002aa28 (Scr_effect) */
VOID Scr_effect(VOID) {
	ULONG Amp;

	if ((G.Status_flg & STAT_QUAKE_SMALL) == 0) {
		if ((G.Status_flg & STAT_QUAKE_LARGE) == 0) {
			return;
		}
		Amp = 2;
	} else {
		Amp = 1;
	}
	Scr_quake(Amp);
}

/* DS 0x8002aa6c (Scr_quake) */
VOID Scr_quake(ULONG Amp) {
	ULONG  D0;
	LONG   i;
	LONG   D1;
	LONG   D2;
	LONG   Ofs;

	D0 = (Line.Quake_ctr0 + 1) & 0xf;
	i  = D0 * 2;
	Line.Quake_ctr0 = (UCHAR)D0;
	if (Amp == 1) {
		D1 = quake_tbl1[i];
		D2 = quake_tbl1[i + 1];
	} else {
		D1 = quake_tbl2[i];
		D2 = quake_tbl2[i + 1];
	}
	if ((G.Status_flg & STAT_QUAKE_RASTA) == 0) {
		i = 0;
		Ofs = ((Main.pDb)->Draw).ofs[1];
		line_mode_set(0, 7);
		do {
			line_trans(i, -(ULONG)D1, (ULONG)D2, (LONG)Ofs);
			i++;
		} while (i != 0xf0);
		line_scr_end();
	} else {
		Scr_rasta((ULONG)D1, (ULONG)D2);
	}
}

/* DS 0x8002ab84 (Scr_rasta) */
VOID Scr_rasta(ULONG Amp_x, ULONG Amp_y) {
	LONG   i;
	ULONG  D0;
	LONG   Ofs;

	D0 = (ULONG)Line.Line_ctr0;
	Line.Line_ctr0 = (UCHAR)(D0 + speed);
	Ofs = ((Main.pDb)->Draw).ofs[1];
	D0 = (D0 + speed) & 0xff;
	line_mode_set(0, 7);
	i = 0;
	do {
		D0 = (D0 + hacho) & 0xff;
		line_trans(i, -Amp_x, (LONG)sin_tbl[D0] / sinpuku + Amp_y, (LONG)Ofs);
		i++;
	} while (i != 0xf0);
	line_scr_end();
}

/* DS 0x8002ac94 (line_trans) */
VOID line_trans(LONG Line_no, LONG Slide_ofs, LONG Copy_ofs, LONG Draw_ofs) {
	register LINE_SCR *pLine;
	DR_MOVE *pMove;
	LONG     D0;
	RECT     Rect;

	if (Slide_ofs == 0 && Copy_ofs == 0) {
		return;
	}
	Rect.x = -(SHORT)Slide_ofs;
	if (0 < Slide_ofs) {
		return;
	}
	Rect.w = (SHORT)Slide_ofs + 320;
	Rect.h = 1;
	pLine = &Line;
	pMove = &pLine->Line_move[G.Cbuf_id][Line_no];
	Rect.y = (SHORT)Draw_ofs;
	if (Line_no + Copy_ofs < 0xf0) {
		D0 = (SHORT)(Line_no + Draw_ofs) + (SHORT)Copy_ofs;
		if (Line_no + Copy_ofs < 0) goto set_y;
	} else {
		D0 = Rect.y + 0xef;
	}
	Rect.y = D0;
set_y:
	SetDrawMove(pMove, &Rect, 0, Line_no + Draw_ofs);
	if (Copy_ofs < 0) {
		addPrim(pLine->pOt, pMove);
	} else {
		*pLine->pPrim_keep0 = (*pLine->pPrim_keep0 & 0xff000000) | ((ULONG)pMove & 0xffffff);
		pLine->pPrim_keep0 = (ULONG *)pMove;
	}
}

/* DS 0x8002addc (line_scr_end) */
VOID line_scr_end(VOID) {
	*Line.pPrim_keep0 = (*Line.pPrim_keep0 & 0xff000000) | (Line.Keep_tag0 & 0xffffff);
}

/* DS 0x8002ae10 (line_mode_set) */
VOID line_mode_set(ULONG Ot_no, ULONG Prio) {
	ULONG    *pOt;
	ULONG    *pNull;
	ULONG     Id;

	Id = (ULONG)G.Cbuf_id;
	if (Ot_no == 1) {
		pOt = (ULONG *)((LONG)G.Ot0[0] + Prio * 4 + Id * 0x1000 + 0x44);
	} else if (Ot_no == 0) {
		pOt = (ULONG *)((LONG)G.Ot0[0] + Prio * 4 + (Id << 5));
	} else {
		/* DS 0x8002ae10: Ot_no values other than 0, 1 and 2 jump to the shared
		 * tail with $a3 (the OT slot pointer) never set, so DS links into
		 * whatever stale $a3 holds. Not reproducible in C; returning early. */
		if (Ot_no != 2) return;
		pOt = (ULONG *)((LONG)G.Ot0[0] + Prio * 4 + Id * 0x40 + 0x2048);
	}
	pNull = Line.Null_prim + Id;
	addPrim(pOt, pNull);
	Line.pOt         = pOt;
	Line.Keep_tag0   = *pNull;
	Line.pPrim_keep0 = pNull;
}

/* DS 0x8002aec8 (Line_work_init) */
VOID Line_work_init(VOID) {
	Line.Null_prim[0] = 0;
	Line.Null_prim[1] = 0;
}
