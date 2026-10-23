/*
 * POLY_SUB.H - Polygon effect work types (KI, POLY_EFF) and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\POLY_SUB.H
 */
#ifndef POLY_SUB_H
#define POLY_SUB_H
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
/* Polygon effect work blocks. KI holds four corner vectors and two flat quads; KI2 adds three matrices
 * and two offsets for the two-quad variant, and KI2_LINE_F3 holds the line primitives of that variant.
 * POLY_EFF and POLY_EFF2 are the timed polygon effects handled in POLY_SUB.C. */
struct tagKI {
	SVECTOR v0;
	SVECTOR v1;
	SVECTOR v2;
	SVECTOR v3;
	POLY_F4 prim[2];
};

struct tagKI2 {
	PRIM_F4 prim[2];
	PRIM_F4 prim1[2];
	MATRIX wm0;
	MATRIX wm1;
	MATRIX wm2;
	SVECTOR off0;
	SVECTOR off1;
};

struct tagKI2_LINE_F3 {
	PRIM_LINE_F3 prim[2];
};

struct tagPOLY_EFF {
	SVECTOR* pSv4;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	CHAR Be_flg;
	CHAR _R0;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	CHAR _R1;
	CHAR _R2;
	ULONG Rgb;
	POLY_GT4 Prim[2];
};

struct tagPOLY_EFF2 {
	SVECTOR Sv0;
	SVECTOR Sv1;
	SVECTOR Sv2;
	SVECTOR Sv3;
	SVECTOR Sv4;
	SVECTOR Sv5;
	POLY_GT4 Prim0[2];
	POLY_GT4 Prim1[2];
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	UCHAR Be_flg;
	CHAR dummy00;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT dummy01;
	ULONG Rgb0;
	ULONG Rgb1;
	ULONG Rgb2;
	ULONG Rgb3;
	ULONG Rgb4;
	ULONG Rgb5;
};

struct tagPOLY_EFF2_WORK {
	CHAR Be_flg;
	CHAR Id;
	CHAR _R0;
	CHAR _R1;
	CHAR Timer1;
	CHAR _R2;
	SHORT Timer0;
	UCHAR Pe_top_no;
	UCHAR Use_Pe_num;
	USHORT dummy00;
};
/* D:\Bio2\prog\POLY_SUB.H */



/* Function prototypes */
void    Poly_eff2_init(void);
void    Move_poly_eff2(void);
void    Sort_poly_eff2(void);
void    Poly_eff_init(void);
void    Set_poly_eff(void);
void    Sort_poly_eff(void);



/* Globals defined in POLY_SUB.C */
	extern SVECTOR Poly_test4[4];						// 0x800a70c8
	extern CHAR ascii_tbl2[20];						// 0x800a70e8
	extern POLY_EFF2_WORK Poly2_work[16];				// 0x800c4348
	extern POLY_EFF Poly_eff[32];						// 0x800dbb98
	extern POLY_EFF2 Poly_eff2[32];						// 0x800dce30
#endif /* POLY_SUB_H_H */
