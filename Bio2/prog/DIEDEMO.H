/*
 * DIEDEMO.H - Death demo camera structs and effect base types
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\DIEDEMO.H
 */
#ifndef DIEDEMO_H
#define DIEDEMO_H
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"
#include "MAIN.H"
#include "MD_SUB.H"

/* ---- types migrated from data.h ---- */

struct tagSPRITEGp {
	UCHAR U;
	UCHAR V;
	CHAR Ofsx;
	CHAR Ofsy;
};


/* Base effect object (ESPECT) used by the particle and spark effects. Field offsets follow the retail
 * layout, and the Padd fields are placeholders that keep the later members at their retail offsets. */
struct tagESPECT_BASE {
	USHORT Be_flg;
	UCHAR Wk_no;
	CHAR Padd0;
	UCHAR Id;
	UCHAR Id2;
	UCHAR Type;
	UCHAR Mov_cnt;
	UCHAR Anm_time;
	UCHAR Anm_cnt;
	SHORT DirY;
	SHORT Mov_x;
	SHORT Mov_y;
	SHORT Mov_z;
	USHORT Tpage;
	SHORT Hot_x;
	SHORT Hot_y;
	SHORT Hot_z;
	USHORT Clut;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	USHORT Mag;
	SHORT Old_x;
	SHORT Old_y;
	SHORT Old_z;
	SHORT Padd1;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	SHORT Padd2;
	SHORT m[3][3];
	SHORT Padd3;
	LONG t[3];
	MATRIX* pMat;
	SSEQUENCE* pAnmseq;
	SPRITEGp* pSprg;
	MSEQUENCE* pMoveseq;
};



struct tagDIEDEMO {
	SHORT Addpos;
	USHORT Rno;
	SHORT Ctr;
	SHORT Free;
	RCUT Rcut;
	LONG Radius;
	SHORT Theta;
	USHORT Prad;
	LONG P_radius;
	UCHAR Phase;        /* +0x34 (0x801938a4) three-frame phase counter; Die_move_wait steps it 0, 1, 2 and wraps, handlers test bit 0 */
	UCHAR S_trans_flg;
	USHORT Daddpos;
	SHORT Wait_ctr;     /* +0x38 signed; Die_move_delay counts it down, Rno++ when below 0 */
};

/*
 * DIEDEMO.H - Death demo overlay (camera / state machine)
 * D:\Bio2\prog\DIEDEMO.C
 * Overlay DIEDEMO:: 0x80190000 - 0x801938c7 (DS retail)
 */



/* ------------------------------------------------------------------ */
/* Structures                                                          */
/* ------------------------------------------------------------------ */

/* CUTINFO - death camera cut parameters (28 bytes), table @0x80193520 */
struct tagCUTINFO {
	/* 0x00 */ LONG     Radius;     /* initial spiral radius                     */
	/* 0x04 */ LONG     P_radius;   /* radius decrement per frame                */
	/* 0x08 */ SHORT    Addpos;     /* initial camera height step (Cam_step)     */
	/* 0x0a */ SHORT    Prad;       /* theta step   (Cdir_ck != 0)               */
	/* 0x0c */ SHORT    P_theta;    /* theta offset (Cdir_ck != 0)               */
	/* 0x0e */ SHORT    Mrad;       /* theta step   (Cdir_ck == 0)               */
	/* 0x10 */ SHORT    M_theta;    /* theta offset (Cdir_ck == 0)               */
	/* 0x12 */ SHORT    P_viewp;    /* view point height offset (Cam_ly)         */
	/* 0x14 */ USHORT   Wait_ctr;   /* Die_move_pause frame count                */
	/* 0x16 */ SHORT    R_obj;      /* look-at object: 0 player, 1 enemy, else - */
	/* 0x18 */ SHORT    R_ptr;      /* parts index of the look-at object         */
	/* 0x1a */ SHORT    Free;
};

/* ------------------------------------------------------------------ */
/* Externs - DIEDEMO.C globals                                         */
/* ------------------------------------------------------------------ */
/* DS 0x801938a4: phase counter, 0..2 (Die_move_wait); bit 0 alternates each frame */
extern DIEDEMO Diedemo;
/* DS 0x801938b0 */ extern VCUT   Dievcut;

/* ------------------------------------------------------------------ */
/* DIEDEMO.C function prototypes (DS address order)                    */
/* ------------------------------------------------------------------ */
void Die_demo(void);                /* 0x8019009c */
void Die_demo_move(void);           /* 0x801900dc */
void Die_move_fade_out(void);       /* 0x80190118 */
void Die_move_delay(void);          /* 0x80190344 */
void Die_move_wait(void);           /* 0x80190388 */
void Die_move_set_cut(void);        /* 0x8019070c */
void Die_move_set_cut_usual(void);  /* 0x801909c4 */
void Die_enemy_set(void);           /* 0x80190aa4 */
void Die_light_set(void);           /* 0x80190d7c */
void Die_vcut_set(void);            /* 0x80190e44 */
void Die_move_pause(void);          /* 0x80190eec */
void Die_move_main(void);           /* 0x80190fb0 */
void Die_move_out(void);            /* 0x801911a8 */
void Die_move_end(void);            /* 0x801912cc */
void Spiral_move(void);             /* 0x80191328 */


#endif /* DIEDEMO_H_H */
