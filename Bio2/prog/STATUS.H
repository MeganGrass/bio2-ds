/*
 * STATUS.H - Status screen and inventory function prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STATUS.H
 */
#ifndef STATUS_H
#define STATUS_H
#include "TYPES.H"
#include "GAME.H"
#include "SUB.H"

/* ---- types migrated from data.h ---- */
struct tagITEM_WORK {
	UCHAR Id;
	UCHAR Num;
	UCHAR Size;
	UCHAR dummy;
};
// D:\Bio2\prog\STATUS.H


extern VOID Status_init(VOID);
extern VOID Status_init_m(VOID);
extern VOID Status(VOID);
extern VOID St_exit(VOID);
extern VOID St_fade_out_set(VOID);
extern VOID St_fade_out_wait(VOID);
extern VOID St_chenge_pl(register ULONG pl_id);
extern VOID pl_tim_load(register ENEMY_WORK *pEm);
extern ULONG St_file_set(register ULONG file_no);
extern VOID St_room_set(VOID);
extern VOID St_disp_name(register LONG x, register LONG y, register ULONG at, register ULONG id);
extern VOID Set_iwork(register ULONG no, register ULONG id, register ULONG num, register ULONG size);
extern VOID Set_iwork_num(register ULONG no, register ULONG num);
extern ULONG Check_pl_life(VOID);
extern ULONG Check_item_space(register ULONG id);
extern ULONG Check_item_mix(VOID);
extern LONG Search_item(register ULONG type);
extern LONG Search_item_id(register ULONG id);
extern VOID Sort_item(VOID);
extern VOID Shift_item(register ULONG type);
extern VOID Set_item(register ULONG id, register ULONG num, register ULONG *adr);
extern VOID Pix_trans(register ULONG set_no, register ULONG *data_addr);
extern VOID Pix_trans_pl(register ULONG set_no, register ULONG id, register ULONG *data_addr);
extern VOID Pix_move(register ULONG base, register ULONG object);
extern VOID Pix_clear(register ULONG set_no);
extern VOID St_pltex_operation(register ULONG type);
extern ULONG Tama_ck_i(register ULONG id);
extern ULONG Tama_decrease_i(register ULONG id, register ULONG flag);
extern LONG Tama_reload_ck(VOID);
extern VOID Tama_reload(VOID);
extern VOID Check_cursol_distance(register ULONG type);

extern STATUS St;					// 800D5BF0		// 4208 bytes
extern UCHAR bk_cut[24];				// 800D4E00
extern LIGHT_CUT_DATA *bk_light;	// 800DCBCC		// 40 bytes
extern UCHAR mugen_sw;				// 800CF1D8




/* Globals defined in STATUS.C: the starting item lists, one per character (init_item_tbl_<name>, 11 entries
 * of 3 bytes each),
 * plus the item data table and the file (document) data table used by the status screen. */
	extern UCHAR init_item_tbl_Leon[11][3];				// 0x800a9264
	extern UCHAR init_item_tbl_Ada[11][3];				// 0x800a93d0
	extern UCHAR init_item_tbl_Claire[11][3];				// 0x800a9285
	extern UCHAR init_item_tbl_Hunk[11][3];				// 0x800a92a6
	extern UCHAR init_item_tbl_ToFu[11][3];				// 0x800a92c7
	extern UCHAR init_item_tbl_Leon_ex[11][3];				// 0x800a92e8
	extern UCHAR init_item_tbl_Claire_ex[11][3];				// 0x800a9309
	extern UCHAR init_item_tbl_Ada_ex[11][3];				// 0x800a932a
	extern UCHAR init_item_tbl_Chris_ex[11][3];				// 0x800a934b
	extern UCHAR init_item_tbl_Sherry[11][3];				// 0x800a93f1

/* Tables defined in STATUS.C (moved here from the old owners) */
	extern ITEM_DATA Item_data_tbl[101];
	extern FILE_DATA File_data[50];
#endif /* STATUS_H_H */
