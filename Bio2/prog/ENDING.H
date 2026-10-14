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
#endif /* ENDING_H_H */
