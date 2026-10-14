/*
 * PSP.H - Sprite work structures and Psp_* prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PSP.H
 */
#ifndef PSP_H
#define PSP_H
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */

struct tagMODEL {
	UCHAR Be_flg;
	UCHAR Free0;
	SHORT speed;
	SHORT speedy;
	SHORT cdir_y;
	SHORT m[3][3];
	LONG Pos_x;
	LONG Pos_y;
	LONG Pos_z;
	SHORT Old_x;
	SHORT Old_y;
	SHORT Old_z;
	MATRIX workm;
	LINE_F2 prim[2][308];
};

struct tagPSP_DATA {
	USHORT nPrim;
	USHORT Padd0;
	ULONG pData_adr;
};

struct tagPXPY {
	USHORT x;
	USHORT y;
};

struct tagGROUPWK {
	USHORT Nsprite;
	USHORT Cba;
	USHORT Hot_x;
	USHORT Hot_y;
};

struct tagPARTSWK {
	UCHAR U;
	UCHAR V;
	UCHAR Ofs_x;
	UCHAR Ofs_y;
	USHORT Z;
	USHORT Flag;
	USHORT W;
	USHORT H;
};

struct tagPARTSST {
	ULONG Be_flg;
	USHORT Id;
	USHORT No;
	SHORT Tbl_x;
	SHORT Tbl_y;
	USHORT Super;
	USHORT mennseki;
	USHORT Hot_x_old;
	USHORT Hot_y_old;
	UCHAR Mag_x;
	UCHAR Mag_y;
	UCHAR Size_x;
	UCHAR Size_y;
	USHORT Nsprite;
	USHORT Cba;
	USHORT Hot_x;
	USHORT Hot_y;
	UCHAR U;
	UCHAR V;
	UCHAR Ofs_x;
	UCHAR Ofs_y;
	USHORT Z;
	USHORT Flag;
	USHORT W;
	USHORT H;
};

struct tagBOXLINE {
	LINE_F3 Line0;
	LINE_F3 Line1;
};
/* D:\Bio2\prog\PSP.H */



/* Sprite work entry (4 bytes, array at pPsp_work) */
struct PSP {
	UCHAR  be_flg;	/* +0 bit0: active, set by Psp_set/Psp_ctr */
	UCHAR  id;		/* +1 sprite group id (matched against Pri_be_flg) */
	short  otz;		/* +2 ordering table index */
};

/* DS 0x800eaad0 */
extern PSP   *pPsp_work;
/* DS 0x800ea240 */
extern TSPRT *pPsp_prim[2];

/* Function prototypes */
void    Psp_init0(void);
void    Psp_init1(void);
void    Psp_set(void);
void    Psp_trans(void);
void    Psp_ctr(UCHAR psp_id, UCHAR st);


#endif /* PSP_H_H */
