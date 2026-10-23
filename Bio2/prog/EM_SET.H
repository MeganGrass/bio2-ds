#ifndef EM_SET_H
#define EM_SET_H
/*
 * EM_SET.H - Enemy work allocation and EMD model loading
 * Resident Evil 2 Dual Shock (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\EM_SET.C
 */


#include <LIBGPU.H>
#include "TYPES.H"
#include "DOOR.H"

/* ---- types migrated from data.h ---- */


struct tagEM_TBL {
	UCHAR Be_flg;
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
};

struct tagT_EM_SET {
	UCHAR R_no_0;
	UCHAR R_no_1;
	UCHAR R_no_2;
	UCHAR R_no_3;
	CHAR C_em_no;
	UCHAR R1_bak;
	UCHAR List_no;
	UCHAR Trans_be;
	SHORT Menu_x;
	SHORT Menu_y;
	SHORT Menu_w;
	SHORT Menu_h;
	SHORT Gx;
	SHORT Gy;
	LONG View_y;
	EM_TBL* CEm_tbl;
	EM_TBL Em_tbl[34];
	EM_TBL Test_em_tbl[34];
	SCE_EM_SET_WORK* pTest_esd_tbl[16];
	UCHAR Test_esd_num[16];
	DR_MODE Dr_mode[2];
	POLY_G3 Em_prim[2][34];
	LINE_F2 Em_line_prim[2][34][3];
	TILE Menu_prim[2];
};

struct tagSIGHT_TBL {
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Size;
};

struct tagSIGHT_WORK {
	LONG Sight_num;
	SIGHT_TBL Sight_tbl[20];
	LINE_F2 Sight_line[2][20][4];
};

struct tagROOM_ENEMY_TBL {
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Cdir_y;
	SHORT Type;
	UCHAR Work_no;
	UCHAR Id;
	UCHAR nFloor;
	UCHAR Sound_flg;
	UCHAR Model_type;
};

struct tagSET_EM_JMP {
	ULONG Num;
	ROOM_ENEMY_TBL* pAdr;
};

/* ------------------------------------------------------------------ */
/* Structures                                                           */
/* ------------------------------------------------------------------ */

/* EM_BIN_INFO - which enemy overlay is resident in a bank (4 bytes)   */
/* (G.Em_bin_info[2] @ G+0x4134)   */
struct tagEM_BIN_INFO {
	/* 0x00 */ USHORT Id;
	/* 0x02 */ USHORT Use;
};

/* DATA_POS - CD sector offset / size of a file inside an archive      */
struct tagDATA_POS {
	/* 0x00 */ ULONG Offset;
	/* 0x04 */ ULONG Size;
};

/* ------------------------------------------------------------------ */
/* Externs                                                              */
/* ------------------------------------------------------------------ */

/* Enemy overlay load area: two 0xd000-byte banks. They start at the   */
/* EM_BIN_0 id object (DS 0x80100000, linked by Makefile.link).        */
/* DS 0x80100000 (EM_BIN_0_id) */
extern UCHAR EM_BIN_0_id[];
#define Em_ov_bank	((UCHAR (*)[0xd000])EM_BIN_0_id)

/* CD position / checksum tables of the enemy EMD archives             */
/* DS 0x8009adf4 */
extern DATA_POS  cdemd0_pos[300];
/* DS 0x8009b754 */
extern UCHAR     cdemd0_sum[300];
/* DS 0x8009b880 */
extern DATA_POS  cdemd1_pos[300];
/* DS 0x8009c1e0 */
extern UCHAR     cdemd1_sum[300];

/* ------------------------------------------------------------------ */
/* Prototypes (DS address order)                                        */
/* ------------------------------------------------------------------ */

void         Enemy_work_set(void);										/* 8001aa38 */
void         Em_set(void);												/* 8001aaa0 */
ULONG        Emd_load(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top);		/* 8001aaa8 */
ENEMY_WORK  *Set_enemy_work(UCHAR Id);									/* 8001ad3c */
void         Kill_enemy_work(ENEMY_WORK *pEm);							/* 8001b250 */
void         Em_move_tbl_set(void);										/* 8001b274 */
void         Em_bin_load(ULONG Id);										/* 8001b710 */
ULONG Emd_load_file(ULONG Em_no, ULONG *Address); 				/* 8001b934 */


#endif /* EM_SET_H_H */
