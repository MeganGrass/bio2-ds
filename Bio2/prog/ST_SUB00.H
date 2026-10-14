#ifndef ST_SUB00_H
#define ST_SUB00_H
/*
 * ST_SUB00.H - Status screen sub-module 00 prototypes and map tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB00.H
 */
#include "TYPES.H"
#include "CD.H"
#include "GAME.H"


extern VOID St_init00(VOID);
extern VOID St_main00(VOID);
extern VOID St_menu0_select(VOID);
extern VOID St_menu0_item(VOID);
extern VOID St_com_use_equip(VOID);
extern VOID St_com_mix(VOID);
extern VOID St_com_inspect(VOID);
extern VOID St_menu0_file(VOID);
extern VOID file_moji_load(VOID);
extern VOID move_file_tag(register ULONG type);
extern VOID St_menu0_map(VOID);
extern VOID St_init_disp_map(VOID);
extern VOID St_disp_map(VOID);
extern ULONG Check_map_no(register ULONG stage, register ULONG room);
extern ULONG Check_room_no(register ULONG map_no, register ULONG room);
extern VOID St_menu1_map(VOID);
extern VOID St_init03(VOID);
extern VOID St_main03(VOID);
extern VOID St_init_map_rooms(VOID);
extern VOID St_main_map_rooms(VOID);
extern VOID St_map_cancel(VOID);
extern VOID St_init_map_floor(VOID);
extern VOID St_disp_map_floor(VOID);
extern VOID St_disp_map_rooms(VOID);

/* DS 0x800A9BA0: room index per Dmode tag byte 2 (used by St_disp_map_rooms) */
extern UCHAR St_room_tbl[];
/* DS 0x800D6C08: end-of-list primitive, one 0xc-byte entry per OT buffer */
extern DR_MODE Map_end_tag[4];
/* DS 0x800D6C20: second end-of-list primitive table */
/* DS 0x800A9B86 / 0x800A9B1E: map floor tile walk start / end (stepped by 8) */
#define St_map_tbl_start	((UCHAR *)0x800a9b86)
#define St_map_tbl_end		((UCHAR *)0x800a9b1e)
/* DS 0x801A0000 / 0x801A8220: overlay CD/TIM load buffers */
#define St_load_buf		((UCHAR *)0x801a0000)
#define St_load_buf2		((UCHAR *)0x801a8220)
/* DS 0x8019C000 / 0x8019D000: SPRT pools (one SPRT = 0x14 bytes) */
#define St_sprt_a		((UCHAR *)0x8019c000)
extern SPRT St_sprt_b[2];		/* Map/item sprites: 2 (one per buffer), written by the map and item-box code */
/* DS 0x8019C300 / 0x8019C94C / 0x8019C9D8: model work buffers */
extern UCHAR St_tmd_a[1612];		/* TMD block A, 0x64c bytes copied from the MIX file by St_disp_mapping (St_init/00) */
extern UCHAR St_tmd_b[140];		/* TMD block B, 0x8c bytes copied from the loaded file */
#define St_tmd_c		((UCHAR *)0x8019c9d8)





/* Globals defined in ST_SUB00.C */
	extern tagfpos maps_floc[20];						// 0x800a9414
	extern tagfpos fits_floc[120];						// 0x800a94b4
	extern UCHAR map_up_floor[24];						// 0x800a9aec
	extern UCHAR map_down_floor[24];					// 0x800a9b04
	extern void (*Status_direct_map_proc_tbl2[3])(void);		// 0x800a9ba8
	extern MAP_STAGE_INFO Map_info[20];					// 0x800aaa38
#endif /* ST_SUB00_H_H */
