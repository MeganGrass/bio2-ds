/*
 * SCE_AT.H - Scene atari (collision/event) structures and jump table
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE_AT.H
 */
#ifndef SCE_AT_H
#define SCE_AT_H
#include <LIBGPU.H>
#include "TYPES.H"
#include "SCE1.H"

/* ---- types migrated from data.h ---- */
struct tagBOX {
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
};

struct tagIN_DOOR_WORK {
	SHORT Next_pos_x;
	SHORT Next_pos_y;
	SHORT Next_pos_z;
	SHORT Next_cdir_y;
	UCHAR Next_stage;
	UCHAR Next_room;
	UCHAR Next_cut;
	UCHAR Next_nfloor;
	UCHAR Dtex_type;
	UCHAR Door_type;
	UCHAR Knock_type;
	UCHAR Key_id;
	UCHAR Key_type;
	UCHAR Free;
};

struct tagSCE_ITEM_DATA {
	SHORT Data0;
	SHORT Data1;
	SHORT Data2;
	UCHAR Om_no;
	UCHAR Free;
};

struct tagAOT_DATA {
	UCHAR Be_flg;
	UCHAR Ck_bit;
	UCHAR Stage_bak;
	UCHAR Room_bak;
	ULONG Data[44];
};

struct tagATE {
	UCHAR Main_rtn;
	UCHAR Sub_rtn0;
	UCHAR Sub_rtn1;
	UCHAR Sub_rtn2;
	UCHAR Sub_rtn3;
	UCHAR Sub_rtn4;
	UCHAR Sub_rtn5;
	CHAR At_no;
	UCHAR Base_pos_no;
	UCHAR View_mode;
	UCHAR View_rtn;
	UCHAR Bit_no;
	UCHAR List_no;
	UCHAR Win_rtn;
	UCHAR Padd1;
	UCHAR Padd2;
	SHORT Menu_x;
	SHORT Menu_y;
	SHORT Menu_w;
	SHORT Menu_h;
	SHORT Y_h;
	SHORT Pos[4][2];
	LONG Viewx;
	LONG Viewy;
	LONG Viewz;
	LONG Gx;
	LONG Gy;
	UCHAR Stage_no_old;
	UCHAR Room_no_old;
	UCHAR Cut_no_old;
	UCHAR nFloor_old;
	SHORT Pos_x_old;
	SHORT Pos_y_old;
	SHORT Pos_z_old;
	SHORT Cdir_y_old;
	POLY_F4 Tview[32][2];
	POLY_F4 Point[2];
	POLY_F4 Bottom[2];
	POLY_F4 Top[2];
	LINE_F2 Waku[2][2][4];
	LINE_F2 Point2[2][4];
	DR_MODE Draw_mode[2];
	TILE Menu_prim[2];
	UCHAR Aot_data_be[16];
	AOT_DATA Aot_data[16][32];
	AOT_DATA CAot_data[32];
	AOT_DATA* pCAot;
};

struct tagMACRO_TBL {
	ULONG Offset;
	USHORT Length;
	USHORT Num;
};

struct tagMACRO_WORK {
	USHORT nAt_id;
	USHORT nFlg;
	USHORT nEvt;
	USHORT nIid;
	USHORT nMap;
	USHORT nFile;
	USHORT nImax;
	USHORT nIflg;
	USHORT nDtex;
	USHORT nDtype;
	MACRO_TBL* pAt_id;
	MACRO_TBL* pFlg;
	MACRO_TBL* pEvt;
	MACRO_TBL* pIid;
	MACRO_TBL* pMap;
	MACRO_TBL* pFile;
	MACRO_TBL* pImax;
	MACRO_TBL* pIflg;
	MACRO_TBL* pDtex;
	MACRO_TBL* pDtype;
	MACRO_TBL* pMtbl;
	UCHAR* pBuff;
};


/* AOT handler (entries of Sce_at_jump_tbl); argument = AOT data part */
typedef ULONG (*SCE_AT_FUNC)(void *pAot);

/* globals defined in SCE_AT.C */
/* DS 0x800ead94 */
extern SCE_AOT      *pCAot;
/* DS 0x800eade0 */
extern ULONG         Old_Fatari;
/* DS 0x800eae30 */
extern ULONG         Old_Uatari;
/* DS 0x800a73c4 */
extern SCE_AT_FUNC   Sce_at_jump_tbl[15];

/* function prototypes (DS address order) */
ULONG Sce_at_check(ENEMY_WORK *pEm, ULONG attribute, ULONG auto_flg);  /* 80051088 */
ULONG  Nothing(void *pAot);                                           /* 800514ec */
ULONG  Sce_Door(void *pAot);                                          /* 80051514 */
void   Key_lost(void);                                                /* 80051718 */
ULONG  Sce_Item(void *pAot);                                          /* 80051884 */
ULONG  Sce_Normal(void *pAot);                                        /* 80051940 */
ULONG  Sce_Message(void *pAot);                                       /* 80051948 */
ULONG  Sce_Event(void *pAot);                                         /* 80051980 */
ULONG  Sce_Flg_chg(void *pAot);                                       /* 800519c0 */
ULONG  Sce_Water(void *pAot);                                         /* 80051a2c */
ULONG  Sce_Move(void *pAot);                                          /* 80051a48 */
ULONG  Sce_Save(void *pAot);                                          /* 80051ab0 */
void   Save_entrance(void);                                           /* 80051b04 */
ULONG  Sce_Itembox(void *pAot);                                       /* 80051c20 */
void   Item_box_entrance(void);                                       /* 80051c64 */
ULONG  Sce_Damage(void *pAot);                                        /* 80051e74 */
ULONG  Sce_Status(void *pAot);                                        /* 80052004 */
ULONG  Sce_Hikidashi(void *pAot);                                     /* 80052044 */
void   Hikidashi_entrance(void);                                      /* 800522cc */
ULONG  Sce_Windows(void *pAot);                                       /* 80052620 */
void   Sce_at(void);                                                  /* 8005264c */
long   Water_ck(long Pos_x, long Pos_z);                              /* 800527b4 */
UCHAR  Floor_check(long check_x, long check_z, ULONG nfloor);         /* 800528e8 */


#endif /* SCE_AT_H_H */
