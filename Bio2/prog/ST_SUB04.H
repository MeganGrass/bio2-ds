/*
 * ST_SUB04.H - Status screen display routine prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB04.H
 */
#ifndef ST_SUB04_H
#define ST_SUB04_H
// D:\Bio2\prog\ST_SUB04.H
#include "TYPES.H"
#include "GAME.H"


extern VOID St_init_disp_num(VOID);
extern VOID St_disp_num(VOID);
extern ULONG print_num(register SPRT *sp, register ULONG in, register ULONG pos, register ULONG mode);
extern VOID St_init_disp_cursol0(VOID);
extern VOID St_disp_cursol0(register ULONG type);
extern VOID St_init_disp_cursol1(VOID);
extern VOID St_disp_cursol1(register ULONG type);
extern VOID St_init_disp_face(VOID);
extern VOID St_disp_face(VOID);
extern VOID St_init_disp_message(VOID);
extern VOID St_disp_message(VOID);
extern VOID St_init_disp_frame(ULONG type);
extern VOID St_disp_frame(ULONG type);
extern VOID St_init_disp_menu0(VOID);
extern VOID St_disp_menu0(VOID);
extern VOID St_init_disp_menu1(register ULONG ctype, register ULONG wtype);
extern VOID St_disp_menu1(register ULONG wtype);
extern VOID St_init_disp_itemlist(VOID);
extern VOID St_disp_itemlist(VOID);
extern VOID St_init_disp_equip(VOID);
extern VOID St_disp_equip(VOID);
extern VOID St_init_disp_wall(VOID);
extern VOID St_disp_wall(VOID);
extern VOID St_init_disp_ECG(VOID);
extern VOID St_disp_ECG(register ULONG type);
extern VOID St_init_disp_itp(register ULONG type);
extern VOID St_disp_itp(register ULONG type);
extern VOID St_init_disp_filesel(VOID);
extern VOID St_disp_filesel(VOID);
extern VOID St_init_disp_file(VOID);
extern VOID St_disp_file(VOID);
extern VOID ST_Om_trans(VOID);
extern VOID model_disp(register MODEL_WORK *pOm, register ULONG type, register ULONG no);

extern DR_MODE Map_end_tag[4];		/* DS 0x800D6C08 */
/* DS 0x800D6C20 / 0x800D6BF0: end-of-list primitive tables */
extern DR_MODE St_end_tag_c[2];		/* DS 0x800D6BF0 */
/* DS 0x800CBC28: frame counter for the itp animation */
/* DS 0x8019A600..0x8019C200: SPRT pools per display section (one SPRT = 0x14 bytes) */
extern SPRT St_equip_sprt[14];		/* Equip sprites: 7 x 2 written by St_init_disp_equip */
extern SPRT St_num_sprt[160];		/* Number sprites: 160 written by St_init_disp_num (buffers interleaved) */
extern SPRT St_itemlist_sprt[14];		/* Item list sprites: 7 x 2 written by St_init_disp_itemlist */
extern SPRT St_cursol0_sprt[6];		/* cursor 0 pool, 3 x 2 sprites */
extern SPRT St_cursol1_sprt[4];		/* cursor 1 pool, 2 x 2 sprites */
extern SPRT St_face_sprt[26];		/* Face sprites: 13 x 2 written by St_init_disp_face */
extern SPRT St_face_sprt2[20];		/* Face sub-sprites: 10 x 2 written by St_init_disp_face */
extern SPRT St_menu0_sprt[16];		/* Menu 0 sprites: 8 x 2 written by St_init_disp_menu0 */
extern POLY_F4 St_message_f4[2];		/* Message frame polygons: 2 written by St_init_disp_message */
extern SPRT St_message_sprt[34];		/* Message sprites: 17 x 2 written by St_init_disp_message */
extern SPRT St_menu1_sprt[18];		/* Menu 1 sprites: up to 9 x 2 written by St_init_disp_menu1 */
extern POLY_F4 St_frame_f4[2];		/* Frame polygons: 2 written by St_init_disp_frame */
extern SPRT St_frame_sprt[34];		/* Frame sprites: 17 x 2 written by St_init_disp_frame (type != 0) */
typedef union {
	POLY_FT4 ft4[2];		/* itp: two item polygons */
	SPRT sp[4];		/* file select: 2 x 2 sprites */
} ST_POOL_C000;
extern ST_POOL_C000 St_pool_c000;
extern SPRT St_pool_c100[8];		/* Pool c100 sprites: 8 (see St_disp_file and St_init_disp_file) */
extern SPRT St_itp_sprt[2];		/* Itp sprites: 2 written by St_init_disp_itp */
extern SPRT St_itemget_sprt[16];		/* Item-get sprites: 8 x 2 written by St_init_disp_itp (type != 0) */
/* DS 0x80198000 .. 0x80199214: ECG / wall primitive pools */
extern SPRT St_prim_e[4];		/* ECG sprites: 4 (2 per buffer) written by St_init_disp_ECG */
extern LINE_F2 St_prim_f[64];		/* ECG lines: 32 x 2 written by St_init_disp_ECG */
extern SPRT St_prim_d[70];		/* SPRT (wall): 2 buffers x 35 sprites, step 2 */


/* Globals defined in ST_SUB04.C */
	extern ECG_POINT Ecg_normal_tbl[80];				// 0x800aaad8
	extern ECG_POINT Ecg_caution0_tbl[80];				// 0x800aab78
	extern ECG_POINT Ecg_caution1_tbl[80];				// 0x800aac18
	extern ECG_POINT Ecg_danger_tbl[80];				// 0x800aacb8
	extern ECG_POINT Ecg_poison_tbl[80];				// 0x800aad58
	extern UCHAR Ecg_col_tbl[5][6];						// 0x800ab160
	extern ECG_POINT* Ecg_line_tbl[5];					// 0x800ab180
#endif /* ST_SUB04_H_H */
