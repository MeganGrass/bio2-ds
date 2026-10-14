#ifndef DOOR_H
#define DOOR_H
/*
 * DOOR.H - Door rendering and scheduling module
 * Resident Evil 2 Dual Shock (SLUS-00748) reconstruction
 */


#include <KERNEL.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"
#include "MD_SUB.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "OMA.H"
#include "SCA.H"
#include "SCE0.H"

/* ---- types migrated from data.h ---- */


struct tagMOJIDISP {
	DR_MODE Moji_mode[2][16];
	SPRT Prim_sprt[2][256];
	SPRT_8 Prim_sprt_8[2][384];		/* 0x2980 (0x1800 per CLUT bank) */
	SPRT* pPrim14_cur;				/* 0x5980 DS MOJIDISP+0x5980 */
	SPRT_8* pPrim8_cur;				/* 0x5984 DS MOJIDISP+0x5984 */
	UCHAR Moji_buff[768];			/* 0x5988 */
	UCHAR* pBuff_ptr;
	UCHAR Mess_flg;
	UCHAR Mess_r_no0;
	UCHAR Mess_type;
	UCHAR Mess_item;
	UCHAR Mess_wait;
	CHAR Mess_time;
	UCHAR Mcol_init;
	UCHAR Mcol_next;
	UCHAR* Mess_top;
	UCHAR* Mess_end;
	UCHAR* Mess_svp;
	USHORT Mess_pos_x;
	USHORT Mess_pos_y;
	USHORT Pos_x_def;
	USHORT Pos_y_def;
	USHORT Mess_attr;
	CHAR Timer;
	UCHAR dummy0;
	ULONG Stop_keep;
	ULONG Stop_data;
};

struct tagKO_TBL {
	USHORT ko_num;
	USHORT ko_t_off;
};



struct tagSCE_EM_SET_WORK {
	UCHAR Com;
	UCHAR Nop;
	CHAR Em_no;
	UCHAR Id;
	USHORT Type;
	UCHAR nFloor;
	UCHAR Sound_flg;
	UCHAR Model_type;
	UCHAR Em_set_flg;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Cdir_y;
	SHORT Motion;
	SHORT Ctr_flg;
};




/* ------------------------------------------------------------------ */
/* Structures                                                           */
/* ------------------------------------------------------------------ */

/*
 * DOOR_WORK - per-door instance work area (0x14c bytes)
 * Offsets verified against the DS disassembly of Door_init / Door_Trans /
 * DoorSort / Door_model_set / Door_model_init.  (Ghidra's DOOR_WORK labels
 * are shifted: it calls 0x144 "Free", 0x146 "Free2", 0x74 "Cdir_z",
 * 0x7c "pSuper", 0x80 "Atd[0].X" - the field names below are the correct
 * ones for these offsets.)  0x010..0x083 is laid out like MODEL_WORK and is
 * passed as such to DoorSort / PresetObjectGT3_neo.
 */
struct tagDOOR_WORK {
	/* 0x000 */ ULONG       Be_flg;
	/* 0x004 */ UCHAR       Routine_0;
	/* 0x005 */ UCHAR       Routine_1;
	/* 0x006 */ UCHAR       Routine_2;
	/* 0x007 */ UCHAR       Routine_3;
	/* 0x008 */ UCHAR       Id;
	/* 0x009 */ UCHAR       Sc_id;
	/* 0x00a */ UCHAR       At_obj_no;
	/* 0x00b */ UCHAR       At_sce_no;
	/* 0x00c */ UCHAR       Work_no;
	/* 0x00d */ UCHAR       At_em_no;
	/* 0x00e */ USHORT      At_em_flg;
	/* 0x010 */ ULONG       Attribute;      /* MODEL_WORK starts here */
	/* 0x014 */ ULONG      *pTmd;
	/* 0x018 */ ULONG      *pPacket;
	/* 0x01c */ ULONG      *pTmd2;
	/* 0x020 */ ULONG      *pPacket2;
	/* 0x024 */ SHORT       M[3][3];        /* rotation part of a MATRIX ... */
	/* 0x036 */ SHORT       _pad36;
	/* 0x038 */ LONG        Pos_x;          /* ... whose t[] is Pos_x/y/z    */
	/* 0x03c */ LONG        Pos_y;
	/* 0x040 */ LONG        Pos_z;
	/* 0x044 */ SHORT       Old_x;
	/* 0x046 */ SHORT       Old_y;
	/* 0x048 */ SHORT       Old_z;
	/* 0x04a */ SHORT       Old_x2;
	/* 0x04c */ SHORT       Old_y2;
	/* 0x04e */ SHORT       Old_z2;
	/* 0x050 */ USHORT      dummy00;
	/* 0x052 */ USHORT      _pad52;
	/* 0x054 */ MATRIX      Workm;
	/* 0x074 */ SHORT       Cdir_x;
	/* 0x076 */ SHORT       Cdir_y;
	/* 0x078 */ SHORT       Cdir_z;
	/* 0x07a */ USHORT      dummy01;
	/* 0x07c */ ULONG       Poly_rgb;       /* CVECTOR: r,g,b,code          */
	/* 0x080 */ ULONG      *pSuper;         /* parent MATRIX                */
	/* 0x084 */ AT_DATA     Atd[4];
	/* 0x104 */ UCHAR       Tpage;
	/* 0x105 */ UCHAR       Clut;
	/* 0x106 */ UCHAR       nFloor;
	/* 0x107 */ UCHAR       Parts_num;
	/* 0x108 */ ULONG      *pKan_t_ptr;
	/* 0x10c */ SHORT       Water;
	/* 0x10e */ USHORT      Type;           /* Ctr2 value that toggles OT mode (attr 0x400) */
	/* 0x110 */ USHORT      Sca_info_lo;
	/* 0x112 */ USHORT      Sca_info_hi;    /* door timer compared with DOOR.Ctr2 (DoorSort) */
	/* 0x114 */ void       *pSca_hit_data;
	/* 0x118 */ SHORT       Sca_old_x;
	/* 0x11a */ SHORT       Sca_old_z;
	/* 0x11c */ MATRIX      Super_matrix;
	/* 0x13c */ SVECTOR     Super_vector;
	/* 0x144 */ USHORT      Attribute2;     /* door model flags (see DoorSort) */
	/* 0x146 */ USHORT      Model_no;
	/* 0x148 */ USHORT      Free;           /* brightness ramp counter (attr 0x100) */
	/* 0x14a */ USHORT      Free2;          /* current brightness (0..0x100)        */
};

/* DOOR - main door object (0x24c bytes) */
struct tagDOOR {
	/* 0x000 */ ULONG      *pPrepacket;
	/* 0x004 */ ULONG      *pTmd_adr;
	/* 0x008 */ POLY_GT3   *pPackettop;
	/* 0x00c */ DIVPOLYGON3 Divp;
	/* 0x224 */ ULONG      *pEdh_offset;
	/* 0x228 */ ULONG      *pVb_offset;
	/* 0x22c */ USHORT      Ctr1;
	/* 0x22e */ USHORT      Ctr2;
	/* 0x230 */ SVECTOR     V0;
	/* 0x238 */ SVECTOR     V1;
	/* 0x240 */ SVECTOR     V2;
	/* 0x248 */ USHORT      Sound_flg;
	/* 0x24a */ USHORT      Div_max;
};

/* DOOR_INFO - per-texture-type CD layout (12 bytes) */
struct tagDOOR_INFO {
	/* 0x00 */ USHORT  Sound_size;
	/* 0x02 */ USHORT  Data_size;
	/* 0x04 */ ULONG   Sector_offset;
	/* 0x08 */ UCHAR   Sound_sum;
	/* 0x09 */ UCHAR   Data_sum;
	/* 0x0a */ USHORT  Free;
};

/* Door data file header, loaded to DS 0x801a1000 by Door_texture_load.
 * The two words are file-relative offsets, relocated by Door_init. */
struct tagDOOR_DATA {
	ULONG  *pTmd;
	ULONG  *pTim;
	UCHAR   Scd[1];         /* door scenario script (Sce.pCScd) */
};

/* EDT/VH block copied to Door_snd_head before opening the door VAB (0xc38 bytes) */
struct tagDOOR_SND_HEAD {
	UCHAR   Edt[0xc30];
	ULONG   Vh_offset;      /* offset of the VAB header from the block start */
	ULONG   Pad;
};

/* One line of the clear-time result text (DS 0x800ead80 / 0x800ead88) */
struct tagDOOR_EX_LINE {
	SHORT   X;
	SHORT   Y;
	SHORT   Idx;            /* next character to reveal */
	USHORT  Cnt;            /* characters left          */
};

typedef void (*DOOR_PROC)(void);
typedef void (*EM_TRANS_FUNC)(ENEMY_WORK *pEm);

/* ------------------------------------------------------------------ */
/* Externs                                                              */
/* ------------------------------------------------------------------ */

/* DS 0x800c3a80  ([0] = DOOR work, [1] != NULL while texture load pending) */
extern DOOR       *pDoor[2];
/* DS 0x800d4dd8 */
extern DOOR_WORK  *pDwork[10];

/* DS .data owned by DOOR.C (initialiser contents not recovered from the binary) */
/* DS 0x8009a42c */
extern UCHAR       door_msg[0x3c];
/* DS 0x8009a468 */
extern USHORT      door_msg_ptr[4];
/* DS 0x8009a470 */
extern MATRIX      D_LIGHTMATRIX;
/* DS 0x8009a490 */
extern MATRIX      D_COLORMATRIX;
/* DS 0x8009a4b0 */
extern USHORT      Dtex_file[56];
/* DS 0x8009a520 */
extern DOOR_INFO   Dinfo[55];

/* Fixed work memory (no symbol in the binary) */
/* DS 0x801a1000 */
extern DOOR_DATA     Door_data;
/* DS 0x801bd000 */
extern DOOR          Door_mem;
/* DS 0x801bd24c .. 0x801bdf44: 10 DOOR_WORK slots (0x14c each), pDwork[9] at 0x801bddf8 */
#define DOOR_WORK_SLOT9  ((DOOR_WORK *)0x801bddf8)
/* DS 0x801fb700 */
extern DOOR_SND_HEAD Door_snd_head;

/* Door_init camera initialiser (rodata) - DS 0x8001082c */
extern const RCUT    Door_camera;

/* Extended door / clear-time sequence ------------------------------- */

/* DS 0x800dcc1e  (also written/read by Set_door in GAME.C) */
extern UCHAR       Door_ex_rno;

/* DS 0x800dcbe0 */
extern RCUT        Ex_battle_rcut;
/* DS 0x800dcc50 */
extern VCUT       Ex_battle_vcut;

/* DS .data (contents not recovered) */
/* DS 0x800a23c0  "clear time" line, digits patched at [0xb..0x12] */
extern UCHAR       Door_ex_mess0[0x15];
/* DS 0x800a23d5  "press button" prompt */
extern UCHAR       Door_ex_mess_push[0x12];
/* DS 0x800a23e7  second line, (Ex_flg & 0x1f) == 0 */
extern UCHAR       Door_ex_mess1a[0x1f];
/* DS 0x800a2406  second line, (Ex_flg & 0x1f) != 0 */
extern UCHAR       Door_ex_mess1b[0x26];

/* DS 0x800c3ac0 / 0x800c3ad8  text being revealed */
extern UCHAR       Door_ex_buf0[0x18];
extern UCHAR       Door_ex_buf1[0x18];

/* DS 0x80190000  overlay area: clear screen TIM + VAB (file 0xdc) */
extern ULONG       Door_ex_data[0x18EA];
/* DS 0x80198000  clear1/clear2 TIM */
/* DS 0x80010b10  Cd_read message for file 0xdc */
extern UCHAR       StgNxt[];

/* SND.C: counter reset before Snd_cd_watchdog loops - DS 0x800a7fac */
extern int         Snd_cd_watchdog_ctr;

/* ------------------------------------------------------------------ */
/* Prototypes (DS order)                                                */
/* ------------------------------------------------------------------ */

void  Door_main(void);                                              /* 80013bc4 */
void  Door_init(void);                                              /* 80013c1c */
void  Door_move(void);                                              /* 80013eb4 */
void  Door_scheduler_main(void);                                    /* 80014058 */
void  Door_exit(void);                                              /* 8001417c */
void  Door_Trans(void);                                             /* 80014234 */
void  DoorSort(MODEL_WORK *mw, DOOR_WORK *pDoorwork, TMD_MEM *Tmd);  /* 8001468c */
void  Door_model_init(DOOR_WORK *pTmpDwork, ULONG Model_no);        /* 80014b40 */
ULONG Door_model_set(SCE_TASK *pSce);                               /* 80014ba4 */
void  Door_snd_trans(void);                                         /* 80014cd0 */
void  Door_texture_load(void);                                      /* 80015064 */
void  Bgm_unlock_ck(void);                                          /* 80015128 */
/* libgte, not declared in LIBGTE.H (PSY-Q LibRef 4.7, p.8-44) */
u_long *DivideGT3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2,
		u_long *uv0, u_long *uv1, u_long *uv2,
		CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2,
		POLY_GT3 *s, u_long *ot, DIVPOLYGON3 *divp);

/* Extended door / clear-time sequence (separate code block in DS) */
void  Door_main_ex(void);                                           /* 800335c0 */
void  Door_proc_ex_init(void);                                      /* 80033674 */
void  Door_proc_ex_move(void);                                      /* 800339a4 */
void  Door_proc_ex_end(void);                                       /* 80033b58 */
void  Door_proc_ex_xa_wait(void);                                   /* 80033d08 */
void  Door_proc_ex_result(void);                                    /* 80033d60 */
void  Door_proc_ex_rcut(void);                                      /* 8003432c */
void  Door_proc_ex_Set_View(void);                                  /* 8003458c */
void  Door_proc_ex_vcut(void);                                      /* 800348d0 */
void  Door_proc_ex_lit(void);                                       /* 80034918 */
void  Door_proc_ex_next_set(void);                                  /* 80034a04 */
void  Door_proc_ex_snd_load(void);                                  /* 80034a84 */
void  Door_proc_ex_ft4_trans(POLY_FT4 *pFt4);                       /* 80034ba8 */



/* Globals defined in DOOR.C */
	extern DOOR_PROC pDoor_proc_tbl[4];					// 0x8009a7b4
#endif /* DOOR_H_H */
