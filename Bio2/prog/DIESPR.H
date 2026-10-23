#ifndef DIESPR_H
#define DIESPR_H
/*
 * DIESPR.H - Death demo sprite / blood effect work
 * D:\Bio2\prog\DIESPR.C
 * Overlay DIEDEMO:: 0x80190000 - 0x801938c7 (DS retail)
 */

#include "TYPES.H"


/* ------------------------------------------------------------------ */
/* Structures                                                          */
/* ------------------------------------------------------------------ */

/*
 * DIESPR - sprite work area. The pointer pDiespr (DS 0x801938c4) holds its address,
 * DS 0x801ad000, which Die_spr_init stores there. Offsets from Ghidra decompile of
 * Die_spr_init / Die_spr_move_init / Die_blood_move_init / Die_blood_move_main.
 * POLY_GT4 must be 0x34 bytes and POLY_FT4 0x28 bytes (retail sizes).
 */
struct tagDIESPR {
	/* 0x0000 */ USHORT    Rno;            /* state index into Die_spr_tbl          */
	/* 0x0002 */ USHORT    Free2;          /* zeroed by Die_spr_move_init           */
	/* 0x0004 */ UCHAR     Free4;          /* 0xf at init                           */
	/* 0x0005 */ UCHAR     Free5;          /* 1 at init                             */
	/* 0x0006 */ USHORT    Ctr;            /* frame counter; progress = 100 - Ctr   */
	/* 0x0008 */ POLY_GT4  Pgt4[2][256];   /* 16x16 quads per group                 */
	/* 0x6808 */ SVECTOR   Pos[17][17];    /* grid vertices: vx, vy (=0x212), vz    */
	/* 0x7110 */ SHORT     Dist[17][17];   /* distance from grid origin (SquareRoot0) */
	/* 0x7352 */ USHORT    Pad7352;
	/* 0x7354 */ POLY_FT4  Blood[3][2][16][8]; /* blood sprites [group][Cbuf][row][col] */
	/* 0xeb54 */ USHORT    Move_start;     /* frame count for erase: 0x16 or 0x34   */
	/* 0xeb56 */ USHORT    Blood_flg;      /* set when blood sprite fades out       */
};

/* ------------------------------------------------------------------ */
/* DIESPR.C function prototypes (DS address order)                     */
/* ------------------------------------------------------------------ */
void Die_spr_init(void);            /* 0x801915b8 */
void Die_spr_move(void);            /* 0x8019162c */
void Die_spr_move_init(void);       /* 0x801916f4 */
void Die_blood_move_init(void);     /* 0x801919f4 */
void Die_spr_move_main(void);       /* 0x80191bac */
void Die_spr_move_hold(void);       /* 0x801921d8 */
void Die_spr_move_erase(void);      /* 0x8019255c */
void Die_spr_move_end(void);        /* 0x801929c0 */
void Die_blood_move_main(void);     /* 0x80192a00 */
void Die_spr_load(void);            /* 0x801932b0 */
void Die_snd_trans(ULONG Size);    /* 0x80193424 */

/* Work buffers (formerly DAT_ addresses) */
extern UCHAR *Die_spr_work;	/* DS 0x801ad000 */
extern UCHAR *Die_vh_src;	/* DS 0x801a4220, death-sound VH source. Size from the DS Die_snd_trans(0xc68) calls */
extern UCHAR *Die_vb_buf;	/* DS 0x801a4e88, death-sound VB body: Bg_img_buf + 0xce88, the tail of the DIEDxx.DIE file after the 0xc68-byte VH block. Length = VAB fsize - 0xc28: 0x8518 (D00), 0x8888 (D01), 0x6148 (D0B), 0x8928 (D0C), 0x8058 (D0D), 0x9058 (D0E, the largest), 0x86f8 (D0F) */
extern UCHAR *Arms_edt_buf;	/* DS 0x801faa00 */
extern ULONG *Bg_img_buf;	/* DS 0x80198000 */


#endif /* DIESPR_H_H */
