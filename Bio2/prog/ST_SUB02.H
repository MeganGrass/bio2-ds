/*
 * ST_SUB02.H - Item and file screen (ST_SUB02) declarations
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB02.H
 */
#ifndef ST_SUB02_H
#define ST_SUB02_H
// D:\Bio2\prog\ST_SUB02.H
#include "TYPES.H"
#include "GAME.H"


extern VOID St_init02(VOID);
extern VOID St_main02(VOID);
extern VOID St_get_check(VOID);
extern VOID St_get_item(VOID);
extern VOID St_disp_file2(VOID);
extern VOID St_get_file(VOID);

/* Jump table (St_main02), move_tbl (St_get_item) and add (St_get_file) are
   function-local statics in ST_SUB02.C; the header copies were shadowed and removed. */

/* DS 0x800A7AB8: per-file check sum table (St_init02 indexes up to 0x67) */
extern UCHAR Itp_check_sum[];
/* DS 0x800D783E: item/cursor animation Y coordinate (xx is the X counterpart) */
extern SHORT yy;
/* DS 0x800D6C20: second end-of-list primitive table */
/* DS 0x800CBC28: frame counter for the item/cursor animation */
extern UCHAR St_anim_cnt;		/* animation counter, DS 0x800CBC28 */
/* DS 0x801A0000 / 0x801A8220: overlay CD/TIM load buffers */
#define St_load_buf		((UCHAR *)0x801a0000)
#define St_load_buf2		((UCHAR *)0x801a8220)
/* DS 0x801A21A0: item table buffer passed to Set_item */
#define St_item_tbl		((UCHAR *)0x801a21a0)
/* DS 0x8019C100: the file-select header sprites are St_pool_c100 (declared in ST_SUB04.H, defined in BUFFERS.C) */



/* Item mix and map tables, defined in ST_SUB04.C */
	extern ITEM_MIX_DATA Mix_nodata;					// 0x800a9ce8
	extern ITEM_MIX_DATA Mix_beretta[2];				// 0x800a9cec
	extern ITEM_MIX_DATA Mix_magnam[2];					// 0x800a9cf4
	extern ITEM_MIX_DATA Mix_shotgun[2];				// 0x800a9cfc
	extern ITEM_MIX_DATA Mix_machinegun;				// 0x800a9d04
	extern ITEM_MIX_DATA Mix_fire;						// 0x800a9d08
	extern ITEM_MIX_DATA Mix_bowgun;					// 0x800a9d0c
	extern ITEM_MIX_DATA Mix_g_n[3];					// 0x800a9d10
	extern ITEM_MIX_DATA Mix_g_f[3];					// 0x800a9d1c
	extern ITEM_MIX_DATA Mix_g_a[3];					// 0x800a9d28
	extern ITEM_MIX_DATA Mix_spark;						// 0x800a9d34
	extern ITEM_MIX_DATA Mix_bullet[6];					// 0x800a9d38
	extern ITEM_MIX_DATA Mix_dot380[2];					// 0x800a9d50
	extern ITEM_MIX_DATA Mix_shot_dan[3];				// 0x800a9d58
	extern ITEM_MIX_DATA Mix_arrow[2];					// 0x800a9d64
	extern ITEM_MIX_DATA Mix_mag_dan[3];				// 0x800a9d6c
	extern ITEM_MIX_DATA Mix_nen[2];					// 0x800a9d78
	extern ITEM_MIX_DATA Mix_battley[2];				// 0x800a9d80
	extern ITEM_MIX_DATA Mix_g_normal[4];				// 0x800a9d88
	extern ITEM_MIX_DATA Mix_g_fire[4];					// 0x800a9d98
	extern ITEM_MIX_DATA Mix_g_acid[4];					// 0x800a9da8
	extern ITEM_MIX_DATA Mix_ink_ribbon;				// 0x800a9db8
	extern ITEM_MIX_DATA Mix_parts_a;					// 0x800a9dbc
	extern ITEM_MIX_DATA Mix_parts_b;					// 0x800a9dc0
	extern ITEM_MIX_DATA Mix_parts_c[3];				// 0x800a9dc4
	extern ITEM_MIX_DATA Mix_g[5];						// 0x800a9dd0
	extern ITEM_MIX_DATA Mix_r[2];						// 0x800a9de4
	extern ITEM_MIX_DATA Mix_b[3];						// 0x800a9dec
	extern ITEM_MIX_DATA Mix_gr;						// 0x800a9df8
	extern ITEM_MIX_DATA Mix_gg[2];						// 0x800a9dfc
	extern ITEM_MIX_DATA Mix_gb[2];						// 0x800a9e04
	extern ITEM_MIX_DATA Mix_p_bomb;					// 0x800a9e0c
	extern ITEM_MIX_DATA Mix_fuse;						// 0x800a9e10
	extern ITEM_MIX_DATA Mix_Jaguar_a;					// 0x800a9e14
	extern ITEM_MIX_DATA Mix_Jaguar_b;					// 0x800a9e18
	extern MAP_DATA Map_stage1_0[8];					// 0x800aa1a8
	extern MAP_DATA Map_stage1_1[4];					// 0x800aa228
	extern MAP_DATA Map_stage1_2[15];					// 0x800aa268
	extern MAP_DATA Map_stage1_3[3];					// 0x800aa358
	extern MAP_DATA Map_stage2_0[18];					// 0x800aa388
	extern MAP_DATA Map_stage2_1[11];					// 0x800aa4a8
	extern MAP_DATA Map_stage3_0[11];					// 0x800aa558
	extern MAP_DATA Map_stage4_0[6];					// 0x800aa608
	extern MAP_DATA Map_stage4_1[13];					// 0x800aa668
	extern MAP_DATA Map_stage5_0[4];					// 0x800aa738
	extern MAP_DATA Map_stage5_1[3];					// 0x800aa778
	extern MAP_DATA Map_stage5_2[3];					// 0x800aa7a8
	extern MAP_DATA Map_stage5_3[2];					// 0x800aa7d8
	extern MAP_DATA Map_stage6_0[6];					// 0x800aa7f8
	extern MAP_DATA Map_stage6_1[1];					// 0x800aa858
	extern MAP_DATA Map_stage6_2[1];					// 0x800aa868
	extern MAP_DATA Map_stage6_3[13];					// 0x800aa878
	extern MAP_DATA Map_stage6_4[4];					// 0x800aa948
	extern MAP_DATA Map_stage6_5[3];					// 0x800aa988
	extern MAP_DATA Map_stage7_0[6];					// 0x800aa9b8
	extern MAP_DATA Map_stage7_1[2];					// 0x800aaa18
/* Globals defined in ST_SUB02.C */
	extern LONG add_y;									// 0x800c4408
	extern SHORT xx;									// 0x800d7834
	extern SHORT f_x;									// 0x800dcba4
	extern SHORT f_y;									// 0x800dcba6
#endif /* ST_SUB02_H_H */
