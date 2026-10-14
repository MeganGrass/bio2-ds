/*
 * SCA.H - Stage collision (SCA) data types and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCA.H
 */
#ifndef SCA_H
#define SCA_H
#include <LIBGPU.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagSCA_DATA {
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	USHORT Id;
	USHORT Type;
	ULONG Floor;
};

struct tagSCA_HEAD {
	SHORT Cx;
	SHORT Cz;
	ULONG Num;
	LONG Ceiling;
	ULONG Dummy;
};

struct tagDISP_POLY {
	SHORT Xmax;
	SHORT Ymax;
	SHORT Xmin;
	SHORT Ymin;
	UCHAR Atari;
	UCHAR Area;
	USHORT Id;
	UCHAR High;
	UCHAR Shape;
	USHORT Dummy16;
	ULONG Floor;
	POLY_G3 Poly[10];
};

struct tagSC_UTIL {
	LONG Max;
	CHAR Rno_0;
	CHAR Rno_1;
	CHAR Rno_2;
	CHAR Rno_3;
	CHAR Rno_4;
	CHAR Rno_5;
	CHAR Rno_6;
	CHAR Rno_7;
	LONG Viewx;
	LONG Viewy;
	LONG Viewz;
	LONG Gx;
	LONG Gy;
	LONG Dxmax;
	LONG Dxmin;
	LONG Dymax;
	LONG Dymin;
	LONG Bg_bak;
	SHORT No;
	SHORT Sort_flr;
	SHORT Aspd;
	CHAR Timer;
	CHAR Comp_sw;
	SHORT Cent_X;
	SHORT Cent_Y;
	SHORT Num[4];
	SHORT StoreXmax;
	SHORT StoreYmax;
	SHORT StoreXmin;
	SHORT StoreYmin;
	LINE_F2 Line[4][3];
	UCHAR LineDispFlg[4];
	UCHAR Ldisp_flg;
	UCHAR Bk_ldisp_flg;
	UCHAR Ddisp_flg;
	UCHAR Adisp_flg;
	CHAR Ceil_M10;
	CHAR Ceil_M1;
	CHAR Ceil_CM10;
	CHAR Ceil_CM1;
	LONG Ceil_height;
};
/* D:\Bio2\prog\SCA.H */



/* Cross-module globals used by SCA.C and OMA.C. Addresses are from the Sca_get_ground
 * disassembly. the old global.h declares pStairs (SCA_DATA *), Gnd_y, In_flg, Old_x and Old_z; those
 * declarations are not repeated here. */
extern long      Ceiling;          /* DS 0x800d5be4 */
extern long      Ground_in_flg;    /* DS 0x800dcbc8 */
extern long      Ceiling_in_pos[]; /* DS 0x800ea198 (index 0 used by Sca_get_ground) */

/* Function prototypes */
ULONG   Sca_get_area(long X, long Z, long Sx, long Sz);
ULONG Sca_ck_hit(VECTOR *pPos, long R, ULONG Floor, ULONG Flg);
ULONG   Sca_ck_hit_om(OBJ_MODEL_WORK *pOm, ULONG Att, ULONG Flg);
ULONG   Sca_hit_box2(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_box(ENEMY_WORK *pEm, long R, SCA_DATA *pScd);
ULONG   Sca_hosei_box(ENEMY_WORK *pEm, SVECTOR *pOld, long X, long Z);
ULONG   Sca_hit_naname_a(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_naname_a(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_naname_b(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_naname_b(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_naname_c(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_naname_c(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_naname_d(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_naname_d(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_hishi(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_hishi(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_circle(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_circle(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_koban_x(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_koban_x(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_koban_z(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_ck_koban_z(VECTOR *pPos, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_slope(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_stairs(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG   Sca_hit_curve(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
long    Sca_get_slope_high(long Pos_x, long Pos_z, long R, SCA_DATA *pT_xz);
long    Sca_get_stairs_high(VECTOR *pPos, long Low_y, long High_y, SCA_DATA *pT_xz);
long    Sca_get_curve_high(VECTOR *pPos, long Low_y, long High_y, SCA_DATA *pT_xz);
long Sca_get_ground(VECTOR *pPos, long R, ULONG Att, ULONG Flg);
USHORT  Sca_get_dir(VECTOR *pPos, SCA_DATA *pT_xz);
long    Sca_get_dir_super(VECTOR *pPos, SCA_DATA *pSca_dat, MATRIX *pSuper, ULONG Flg);
USHORT Sca_get_dir_super_tmp(long X, long Z, long Sx, long Sz, long Sw, long Sd);
ULONG   Get_axis(MATRIX *pM);
ULONG   Get_axis2(MATRIX *pM);
long    Sca_get_pos(long Dir, SCA_DATA *pSca_dat);
void    Sca_get_pos_super(VECTOR *pPos, SCA_DATA *pSca_dat, MATRIX *pSuper, VECTOR *pVec);
USHORT Sca_get_pos_super_tmp(long X, long Z, long Sx, long Sz, long Sw, long Sd, long *pX, long *pZ);
ULONG Sca_ck_line(VECTOR *pV1, VECTOR *pV2, ULONG Att, ULONG Flg);
long    Sca_get_high(SCA_DATA *pSca_dat);
long    Sca_get_low(SCA_DATA *pSca_dat);
void    Sca_search(ULONG Area, long Att, ULONG Floor);



/* Globals defined in SCA.C */
	extern SCA_DATA** pSca_data;						// 0x800c3b6c
	extern LONG Old_x;									// 0x800c3b70
	extern LONG Old_z;									// 0x800c3b74
	extern ULONG In_flg;								// 0x800c3b80
	extern SCA_DATA Tmp_Sca;							// 0x800c3b84
	extern SCA_DATA* pStairs;							// 0x800d7694
#endif /* SCA_H_H */
