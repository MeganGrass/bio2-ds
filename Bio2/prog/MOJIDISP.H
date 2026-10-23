/*
 * MOJIDISP.H - Message text display prototypes and branch table
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MOJIDISP.H
 */
#ifndef MOJIDISP_H
#define MOJIDISP_H
#include "TYPES.H"
#include "DOOR.H"

/* ---- types migrated from data.h ---- */
struct tagBRANCH_TBL {
	UCHAR Select_num;
	UCHAR Mess_ofs;
	UCHAR Skip_size;
	UCHAR Home_pos;
	USHORT Str_no;
};

/* Message printing. Print8 and Print14 print a string with the 8-dot or 14-dot font (Print14 adds the
 * 0x200 font flag). Mess_set queues a retail message, Mess_disp and mess_trans advance it, and the
 * moji_trans_* functions turn the queued glyph codes into sprites. */
long Print8(ULONG x, ULONG y, ULONG c, ULONG p, UCHAR *pPtr);
long Print14(ULONG x, ULONG y, ULONG c, ULONG p, UCHAR *pPtr);
long Print_main(ULONG x, ULONG y, ULONG attr, UCHAR **pSave);
long Mess_set(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data);
void Moji_trans_main(void);
void Mess_disp(MOJIDISP *pMoji);
void Branch_disp(MOJIDISP *pMoji, BRANCH_TBL *pBtbl, ULONG Flg, ULONG Mode);
UCHAR *name_ptr_set(UCHAR Item_id);
void mess_trans(MOJIDISP *pMoji);
void Name_disp(long Pos_x, long Pos_y, ULONG Attr, ULONG Item_id);
void Mess_print(ULONG Pos_x, ULONG Pos_y, UCHAR *pMess, ULONG Attr);
void moji_trans_main(MOJIDISP *pMoji);
UCHAR *moji_trans_8(UCHAR *pBuff0, ULONG Pos, MOJIDISP *pMoji);
UCHAR *moji_trans_14(UCHAR *pBuff0, ULONG Pos, MOJIDISP *pMoji);
void Moji_buff_init(void);
void Moji_init(void);
void Moji_mode_init(void);

/* DS 0x80010a1c: blinking next-page mark string, DS 0x80010a40: selection cursor string (Print14) */
extern UCHAR s_Moji_next_mark_80010a1c[8];
extern UCHAR s_Moji_sel_cursor_80010a40[4];



/* Globals defined in MOJIDISP.C */
	extern UCHAR msg_space[264];						// 0x8009de34
	extern UCHAR namelist_tbl_data[1276];				// 0x8009df3c
	extern USHORT namelist_tbl_ofs[140];				// 0x8009e438
	extern UCHAR namelist_u_data[1628];					// 0x8009e550
	extern USHORT namelist_u_ofs[140];					// 0x8009ebac
	extern UCHAR mess_tbl_data[728];					// 0x8009ecc4
	extern USHORT mess_tbl_ofs[24];						// 0x8009ef9c
	extern UCHAR mess_u_data[924];						// 0x8009efcc
	extern USHORT mess_u_ofs[24];						// 0x8009f368
	extern UCHAR mess_sub_data[4828];					// 0x8009f398
	extern USHORT mess_sub_ofs[116];					// 0x800a0674
	extern UCHAR mess_etc_data[5920];					// 0x800a075c
	extern USHORT mess_etc_ofs[116];					// 0x800a1e7c
	extern UCHAR branch_data[220];						// 0x800a1f64
	extern USHORT branch_ofs[42];						// 0x800a2040
	extern CHAR ascii_tbl[2][17];						// 0x800a2094
	extern MOJIDISP Moji;								// 0x800e2ab0
#endif /* MOJIDISP_H_H */
