/*
 * ST_SUB01.H - Status screen and item box prototypes and buffers
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB01.H
 */
#ifndef ST_SUB01_H
#define ST_SUB01_H
// D:\Bio2\prog\ST_SUB01.H
#include "TYPES.H"


extern VOID St_init01(VOID);
extern VOID St_main01(VOID);
extern VOID St_select_menu_m(VOID);
extern VOID St_select_item_m(VOID);
extern VOID Exchange_item(VOID);
extern VOID St_init_itembox(VOID);
extern VOID St_disp_itembox(VOID);

/* Ibox tables are function-local statics in ST_SUB01.C (St_main01, St_init_itembox,
   St_disp_itembox), each used by one function only; no header copies. */

/* DS 0x8019C000: item image buffer, 0x4B0 bytes per item id */
#define St_item_img		((UCHAR *)0x8019c000)
/* DS 0x80198000 / 0x80198800 / 0x80199000 / 0x80199200: itembox primitive pools */
extern POLY_FT4 St_prim_a[12];		/* Item box cells: 6 x 2 written by St_init_itembox */
extern POLY_F4 St_prim_b[8];		/* Item box polygons: 4 x 2 written by St_init_itembox */
extern LINE_F2 St_prim_c[6];		/* Item box lines: 6 written by St_init_itembox */

static SHORT Move_x[1];		// 0x800d4cd0 (halfword in binary)
static SHORT sw;		// 0x800d766c (halfword in binary)


#endif /* ST_SUB01_H_H */
