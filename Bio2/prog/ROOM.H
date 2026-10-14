/*
 * ROOM.H - Room work, block, floor and light data structures
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ROOM.H
 */
#ifndef ROOM_H
#define ROOM_H
#include <LIBGPU.H>
#include "TYPES.H"
#include "DEFINES.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "SCE0.H"
#include "SUB.H"

/* ---- types migrated from data.h ---- */

/* Room block (G.pRoom->pRbj): word 0 = byte offset of the entry table from pRbj,
 * word 1 = entry count. Each 8-byte entry is one block: Data_ofs points at the
 * block flags word (kan_t_ptr = that + 4), Seq_ofs at the sequence data.
 * Evidence: Rbj_set (DS 0x80018e08). */
struct tagRBJ_ENTRY {
	ULONG Data_ofs;
	ULONG Seq_ofs;
};
typedef struct tagRBJ_ENTRY RBJ_ENTRY;

struct tagROOM_WORK {
	UCHAR nSprite;
	UCHAR nCut;
	UCHAR nOmodel;
	UCHAR nItem;
	UCHAR nDoor;
	UCHAR nRoom_at;
	UCHAR Reverb_lv;
	UCHAR nSprite_max;
	ULONG* pEdt0;
	ULONG* pVh0;
	ULONG* pVb0;
	ULONG padd0;
	ULONG padd1;
	ULONG* pRbj_end;
	ULONG* pSca;
	RCUT* pRcut;
	VCUT* pVcut;
	LIGHT_CUT_DATA* pLight;
	MODEL_LINK_INFO* pOmodel;
	SHORT* pFloor;
	SHORT* pBlock;
	UCHAR* pMessage;
	UCHAR* pMessage_sub;
	ULONG* pScrl;
	SHORT* pScdx;
	SHORT* pScd;
	ULONG* pEsp_hed;
	ULONG* pEsp_end;
	ULONG* pEsp_tim;
	ULONG* pEsp_tim_end;
	ULONG* pRbj;
	RCUT Cut[1];
};

struct tagBLOCK_DATA {
	UCHAR Disp_flg;
	UCHAR Dir;
	UCHAR Sel;
	UCHAR Sel_bit;
	CHAR Be_flg[16];
	SHORT Pos_x[16][2];
	SHORT Pos_z[16][2];
	USHORT Dir_bit[16];
	USHORT Abut[16];
	CHAR Main_rtn;
	CHAR Routine_0;
	CHAR Routine_1;
	CHAR Routine_2;
	USHORT Header;
	USHORT Dummy;
	USHORT Clut_id;
	LONG Viewx;
	LONG Viewy;
	LONG Viewz;
	LONG Gx;
	LONG Gy;
	UCHAR BIts[16];
	USHORT Io_buff[98];
	POLY_F4 Area[16];
	LINE_F2 Waku[4];
};

struct tagFLOOR_DATA {
	UCHAR Disp_flg;
	UCHAR Sel;
	UCHAR Dir;
	CHAR Be_flg[16];
	SHORT Pos_x[16][2];
	SHORT Pos_z[16][2];
	USHORT Size_x[16];
	USHORT Size_z[16];
	USHORT Se_no[16];
	USHORT Floor_height[16];
	USHORT Header;
	USHORT Io_buff[98];
	USHORT Clut_id;
	CHAR Main_rtn;
	CHAR Routine_0;
	CHAR Routine_1;
	UCHAR Dummy00;
	USHORT Dummy01;
	LONG Viewx;
	LONG Viewy;
	LONG Viewz;
	LONG Gx;
	LONG Gy;
	POLY_F4 Area[16];
	LINE_F2 Waku[4];
};

struct tagLIGHT_DATA {
	CHAR Main_rtn;
	UCHAR Mode;
	UCHAR Disp_flg;
	UCHAR Input_mode;
	UCHAR Timer1;
	UCHAR Timer2;
	UCHAR Copy_no;
	UCHAR dm0;
	UCHAR Cur_0;
	UCHAR Cur_1;
	UCHAR Cur_2;
	UCHAR Cur_3;
	UCHAR Rtn_0;
	UCHAR Rtn_1;
	UCHAR Rtn_2;
	UCHAR Rtn_3;
	SHORT Add;
	USHORT dm01;
	LONG Viewx;
	LONG Viewy;
	LONG Viewz;
	LONG Gx;
	LONG Gy;
	LIGHT_CUT_DATA* pCut;
	POLY_F4 Dispwin[2];
	POLY_F4 Mainpwin[2];
	POLY_F4 Point[4][2];
	POLY_F4 Subwin[4][2];
	POLY_F4 Colwin[3][2];
	DR_MODE Draw_mode[2];
};
/* D:\Bio2\prog\ROOM.H */



/* DS 0x801bfa14: font TIM load buffer (Set_room) */
extern ULONG  Font_tim_buf[0x90EC];
/* Runtime address, not a fixed DS object: the free-work area starts at the stage overlay base 0x8011a000 plus the aligned stage size (Set_stage) */
#define Free_work_base ((UCHAR *)RDTFile)	/* stage overlay base (DEFINES.H RDTFile) */

/* Function prototypes */
void    Set_room(void);
void    Set_stage(void);
void    Replace_rdt(void);
void    Em_init_move(void);



/* Globals defined in ROOM.C */
	extern USHORT Rdt_tbl1[30];							// 0x800a70fc	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl2[28];							// 0x800a7138	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl3[16];							// 0x800a7170	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl4[20];							// 0x800a7190	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl5[12];							// 0x800a71b8	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl6[24];							// 0x800a71d0	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern USHORT Rdt_tbl7[8];							// 0x800a7200	// ROOMxxxx_RDT names (FILECODE.H), by room
	extern ULONG *Rdt_tbl[7];							// 0x800a7210	// RDT table: 7 pointers, indexed by stage, to Rdt_tbl1..7
	extern UCHAR UnknownSetRoomByte;					// 0x800dfad7
#endif /* ROOM_H_H */
