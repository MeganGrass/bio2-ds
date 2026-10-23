/*
 * ENDING.H - Ending sequence sprite record type
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ENDING.H
 */
#ifndef ENDING_H
#define ENDING_H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif

#include "TYPES.H"

/* ---- types migrated from data.h ---- */
/* Sprite record of the ending sequence, laid out as in the retail ENDING.BIN data (packed, so the
 * fields sit at their retail byte offsets). */
struct tagEND_SPR {
	UCHAR Be_flg;
	UCHAR Id;
	UCHAR Otz;
	UCHAR Chars;
	SHORT Ofs_x;
	SHORT Ofs_y;
};


#if defined(_MSC_VER)
#pragma pack(pop)
#endif



/* Globals defined in ENDING.C */
	extern LONG mode;									// 0x8009db40
	extern UCHAR Ending_inittbl_buf[0x2d90];
	extern POLY_FT4 Ending_prim[2][200];
	extern ULONG Ending_save_copy[0x598 / 4];
	extern UCHAR *Ending_work_buf;
#endif /* ENDING_H_H */
