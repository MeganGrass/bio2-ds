/*
 * ENDING.C - Ending sequence overlay (Resident Evil 2 DS, SLUS_00748)
 * Overlay addresses use the ENDING:: prefix (ENDING::8011xxxx).
 * Function addresses in the comments are DS retail addresses from Ghidra.
 */

#include "TYPES.H"
#include <ASM.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include <MALLOC.H>
#include <STDLIB.H>
#include <STRINGS.H>
#include "MAIN.H"
#include "SLD.H"
#include "ENDING.H"
#include "CD.H"
#include "CONFIG.H"
#include "ESP_R.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "MOVIE.H"
#include "PAD.H"
#include "ROOM.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "DOOR.H"
#include "GAME.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x8011e5b0 (Ending_inittbl_buf) */
UCHAR Ending_inittbl_buf[0x2d90];
/* DS 0x80121408 (Ending_prim) */
POLY_FT4 Ending_prim[2][200];
/* DS 0x801252b8 (Ending_save_copy) */
ULONG Ending_save_copy[0x598 / 4];
/* DS 0x8012a000 (Ending_work_buf): load area of Ending_init for END00.TIM (0x41020 bytes, the largest load; END19.TIM is 0x8040) and then ROOM800.SND (0x9c4c bytes, over the same start). The later END1x.TIM and INIT_TBL.DAT loads go to Bg_img_buf and Ending_inittbl_buf */
UCHAR *Ending_work_buf = (UCHAR *)0x8012a000;

/* Overlay data (ENDING::8011xxxx) and DS work RAM referenced by this overlay, defined here.
 * Items inside ENDING.BIN (ENDING::8011a000..8012585a) carry the retail bytes as initialisers;
 * items past the end of the file are zero-initialised RAM.

 * The stat and result screen tables (Ending_stat_glyph_tbl, Ending_stat_rec_tbl, Ending_glyph_Tbl, Ending_result_rec_tbl,
 * Ending_glyph_idx_tbl) are sized typed arrays. */

/* Rank/score display tables (ENDING overlay data) */
/* DS 0x8011a08c (Ending_rank_x0) */
ULONG  Ending_rank_x0 = 0x00D100C3;   /* rank tile word 0 */
/* DS 0x8011a090 (Ending_rank_x1) */
ULONG  Ending_rank_x1 = 0x00E900DF;   /* rank tile word 1 */
/* DS 0x8011a094 (Ending_rank_x2) */
ULONG  Ending_rank_x2 = 0x010500F7;   /* rank tile word 2 */
/* DS 0x8011a098 (Ending_rank_x3) */
ULONG  Ending_rank_x3 = 0x011D010F;   /* rank tile word 3 */
/* DS 0x8011a09c (Ending_rank_x4) */
ULONG  Ending_rank_x4 = 0x00F700E9;   /* rank tile word 4 */
/* DS 0x8011a0a0 (Ending_rank_y0) */
ULONG  Ending_rank_y0 = 0x008C008C;   /* rank tile word 5 */
/* DS 0x8011a0a4 (Ending_rank_y1) */
ULONG  Ending_rank_y1 = 0x008C008C;   /* rank tile word 6 */
/* DS 0x8011a0a8 (Ending_rank_y2) */
ULONG  Ending_rank_y2 = 0x008C008C;   /* rank tile word 7 */
/* DS 0x8011a0ac (Ending_rank_y3) */
ULONG  Ending_rank_y3 = 0x008C008C;   /* rank tile word 8 */
/* DS 0x8011a0b0 (Ending_rank_y4) */
ULONG  Ending_rank_y4 = 0x00AC00AC;   /* rank tile word 9 */
/* DS 0x8011a0b4 (Ending_rank_thr_init0) */
ULONG  Ending_rank_thr_init0 = 0x00C7010D;   /* score local init, word 0 */
/* DS 0x8011a0b8 (Ending_rank_thr_init1) */
ULONG  Ending_rank_thr_init1 = 0x00630090;   /* score local init, word 1 */
/* DS 0x8011a0bc (Ending_rank_thr_init2) */
USHORT Ending_rank_thr_init2 = 0x0000;   /* score local init, half */
/* DS 0x8011a0c0 (Ending_save_area_init0) */
ULONG  Ending_save_area_init0 = 0x100F100F;   /* next_set local init, word */
/* DS 0x8011a0c4 (Ending_save_area_init1) */
UCHAR  Ending_save_area_init1 = 0x11;   /* next_set local init, byte 0 */
/* DS 0x8011a0c5 (Ending_save_area_init2) */
UCHAR  Ending_save_area_init2 = 0x12;   /* next_set local init, byte 1 */

/* Glyph/record tables used by Ending_draw_score, Ending_draw_rank, Ending_moji_draw */
/* DS 0x8011dcc0 (Ending_msg_row_00) */
UCHAR Ending_msg_row_00[20] = {
	0x29, 0x45, 0x4f, 0x4f, 0x45, 0x4b, 0x4a, 0x00, 0x3f, 0x4b, 0x49, 0x4c, 0x48, 0x41, 0x50, 0x41,
	0x1a, 0x1a, 0xfe
};

/* DS 0x8011dcd4 (Ending_msg_row_02) */
UCHAR Ending_msg_row_02[24] = {
	0x30, 0x44, 0x3d, 0x4a, 0x47, 0x4f, 0x00, 0x50, 0x4b, 0x00, 0x55, 0x4b, 0x51, 0x4e, 0x00, 0x41,
	0x42, 0x42, 0x4b, 0x4e, 0x50, 0x4f, 0x18, 0xfe
};

/* DS 0x8011dced (Ending_msg_row_03) */
UCHAR Ending_msg_row_03[20] = {
	0x50, 0x44, 0x41, 0x00, 0x44, 0x3d, 0x56, 0x3d, 0x4e, 0x40, 0x4b, 0x51, 0x4f, 0x00, 0x52, 0x45,
	0x4e, 0x51, 0x4f, 0xfe
};

/* DS 0x8011dd02 (Ending_msg_row_04) */
UCHAR Ending_msg_row_04[20] = {
	0x44, 0x3d, 0x4f, 0x00, 0x3e, 0x41, 0x41, 0x4a, 0x00, 0x41, 0x4e, 0x3d, 0x40, 0x45, 0x3f, 0x3d,
	0x50, 0x41, 0x40, 0xfe
};

/* DS 0x8011dd17 (Ending_msg_row_05) */
UCHAR Ending_msg_row_05[16] = {
	0x42, 0x4e, 0x4b, 0x49, 0x00, 0x4b, 0x51, 0x4e, 0x00, 0x53, 0x4b, 0x4e, 0x48, 0x40, 0x01, 0xfe
};

/* DS 0x8011dd28 (Ending_msg_row_07) */
UCHAR Ending_msg_row_07[20] = {
	0x1d, 0x48, 0x48, 0x00, 0x4b, 0x42, 0x00, 0x44, 0x51, 0x49, 0x3d, 0x4a, 0x45, 0x50, 0x55, 0x00,
	0x44, 0x3d, 0x4f, 0xfe
};

/* DS 0x8011dd3d (Ending_msg_row_08) */
UCHAR Ending_msg_row_08[16] = {
	0x3e, 0x41, 0x41, 0x4a, 0x00, 0x4f, 0x3d, 0x52, 0x41, 0x40, 0x00, 0x42, 0x4e, 0x4b, 0x49, 0xfe
};

/* DS 0x8011dd4e (Ending_msg_row_09) */
UCHAR Ending_msg_row_09[19] = {
	0x50, 0x44, 0x41, 0x00, 0x50, 0x41, 0x4e, 0x4e, 0x4b, 0x4e, 0x00, 0x4b, 0x42, 0x00, 0x50, 0x44,
	0x45, 0x4f, 0xfe
};

/* DS 0x8011dd62 (Ending_msg_row_10) */
UCHAR Ending_msg_row_10[12] = {
	0x1e, 0x45, 0x4b, 0x00, 0x24, 0x3d, 0x56, 0x3d, 0x4e, 0x40, 0x01, 0xfe
};

/* DS 0x8011dd6f (Ending_msg_row_11) */
UCHAR Ending_msg_row_11[16] = {
	0x2f, 0x50, 0x4b, 0x4e, 0x45, 0x41, 0x4f, 0x00, 0x53, 0x45, 0x48, 0x48, 0x00, 0x3e, 0x41, 0xfe
};

/* DS 0x8011dd80 (Ending_msg_row_12) */
UCHAR Ending_msg_row_12[24] = {
	0x50, 0x4b, 0x48, 0x40, 0x00, 0x42, 0x4b, 0x4e, 0x00, 0x43, 0x41, 0x4a, 0x41, 0x4e, 0x3d, 0x50,
	0x45, 0x4b, 0x4a, 0x4f, 0x00, 0x50, 0x4b, 0xfe
};

/* DS 0x8011dd99 (Ending_msg_row_13) */
UCHAR Ending_msg_row_13[22] = {
	0x3f, 0x4b, 0x49, 0x41, 0x00, 0x3d, 0x3e, 0x4b, 0x51, 0x50, 0x00, 0x55, 0x4b, 0x51, 0x4e, 0x00,
	0x43, 0x4e, 0x41, 0x3d, 0x50, 0xfe
};

/* DS 0x8011ddb0 (Ending_msg_row_14) */
UCHAR Ending_msg_row_14[12] = {
	0x3d, 0x3f, 0x44, 0x45, 0x41, 0x52, 0x41, 0x49, 0x41, 0x4a, 0x50, 0xfe
};

/* DS 0x8011ddbd (Ending_msg_row_15) */
UCHAR Ending_msg_row_15[18] = {
	0x4b, 0x42, 0x00, 0x4f, 0x45, 0x4a, 0x43, 0x48, 0x41, 0x44, 0x3d, 0x4a, 0x40, 0x41, 0x40, 0x48,
	0x55, 0xfe
};

/* DS 0x8011ddd0 (Ending_msg_row_16) */
UCHAR Ending_msg_row_16[26] = {
	0x4b, 0x52, 0x41, 0x4e, 0x3f, 0x4b, 0x49, 0x45, 0x4a, 0x43, 0x00, 0x3d, 0x48, 0x48, 0x00, 0x3d,
	0x40, 0x52, 0x41, 0x4e, 0x4f, 0x45, 0x50, 0x55, 0x01, 0xfe
};

/* DS 0x8011ddeb (Ending_msg_row_19) */
UCHAR Ending_msg_row_19[17] = {
	0x00, 0x1f, 0x2b, 0x2a, 0x23, 0x2e, 0x1d, 0x30, 0x31, 0x28, 0x1d, 0x30, 0x25, 0x2b, 0x2a, 0x2f,
	0xfe
};

/* DS 0x8011ddfd (Ending_msg_row_21) */
UCHAR Ending_msg_row_21[16] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x21, 0x2e, 0x2b, 0x1a, 0x1a, 0xfe
};

/* DS 0x8011de44 (Ending_msg_row_fe) */
UCHAR Ending_msg_row_fe[1] = {
	0xfe
};

/* DS 0x8011de48 (Ending_stat_glyph_tbl): 14 glyph records of the stat screen, 0x8c bytes. Ending_moji_draw reads
 * record n at ENDING::8011de48 + n * 10 (lbu +0 and +1, lhu +2, lbu +4..+9). The retail bytes are ENDING.BIN offset 0x3e48. */
typedef struct {
	UCHAR  Tpage;     /* +0, low four bits are the tpage number */
	UCHAR  Clut_x;    /* +1, CLUT x in 16-pixel units */
	USHORT Clut_y;    /* +2 */
	UCHAR  U;         /* +4 */
	UCHAR  V;         /* +5 */
	UCHAR  Uw;        /* +6, texture width */
	UCHAR  Vh;        /* +7, texture height */
	UCHAR  W;         /* +8, screen width */
	UCHAR  H;         /* +9, screen height */
} ENDING_STAT_GLYPH;
ENDING_STAT_GLYPH Ending_stat_glyph_tbl[14] = {
	{ 0x0c, 0x00, 0x01f1, 0x00, 0x00, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0c, 0x00, 0x01f1, 0x00, 0x20, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0c, 0x00, 0x01f1, 0x00, 0x40, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0c, 0x00, 0x01f1, 0x00, 0x60, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0c, 0x00, 0x01f1, 0x00, 0x80, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0c, 0x00, 0x01f1, 0x00, 0xa0, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0e, 0x00, 0x01f3, 0x00, 0x00, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x03, 0x14, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x05, 0x1e, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x07, 0x00, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x00, 0x4b, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x05, 0x32, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x03, 0x28, 0x78, 0xb4, 0x50, 0x78 },
	{ 0x0e, 0x00, 0x01f3, 0x07, 0x4b, 0x78, 0xb4, 0x50, 0x78 },
};

/* DS 0x8011ded4 (Ending_stat_rec_tbl): 12 stat-display records of 8 bytes (0x60 bytes). Ending_moji_draw and
 * Ending_stat_flg_set walk them through Ending_stat_rec_ptr in steps of 8: byte +0 is the shown flag, byte +1
 * bit 0x20 disables the record, +3 is the glyph index, +4 and +6 the screen x and y. Byte +2 is never read. */
typedef struct {
	UCHAR  Flg;       /* +0 */
	UCHAR  Opt;       /* +1 */
	UCHAR  Kind;      /* +2, unread (0x02 in every record) */
	UCHAR  Glyph;     /* +3, index into Ending_stat_glyph_tbl */
	USHORT X;         /* +4 */
	USHORT Y;         /* +6 */
} ENDING_STAT_REC;
ENDING_STAT_REC Ending_stat_rec_tbl[12] = {
	{ 0x00, 0x20, 0x02, 0, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 0, 0x0020, 0x00b4 },
	{ 0x00, 0x20, 0x02, 1, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 1, 0x0020, 0x00b4 },
	{ 0x00, 0x20, 0x02, 2, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 2, 0x0020, 0x00b4 },
	{ 0x00, 0x20, 0x02, 3, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 3, 0x0020, 0x00b4 },
	{ 0x00, 0x20, 0x02, 4, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 4, 0x0020, 0x00b4 },
	{ 0x00, 0x20, 0x02, 5, 0x0020, 0x00b4 },
	{ 0x00, 0x21, 0x02, 5, 0x0020, 0x00b4 },
};


/* DS 0x8011df34 (Ending_msg_row_tbl): 22 row pointers in entry order; entries 1, 6, 17, 18 and 20 share Ending_msg_row_fe */
UCHAR *Ending_msg_row_tbl[22] = {
	Ending_msg_row_00, Ending_msg_row_fe, Ending_msg_row_02, Ending_msg_row_03, Ending_msg_row_04, Ending_msg_row_05,
	Ending_msg_row_fe, Ending_msg_row_07, Ending_msg_row_08, Ending_msg_row_09, Ending_msg_row_10, Ending_msg_row_11,
	Ending_msg_row_12, Ending_msg_row_13, Ending_msg_row_14, Ending_msg_row_15, Ending_msg_row_16, Ending_msg_row_fe,
	Ending_msg_row_fe, Ending_msg_row_19, Ending_msg_row_fe, Ending_msg_row_21
};
/* DS 0x8011df8c (Ending_msg_char_cnt_tbl): typing delay per message character, 300 bytes up to Ending_glyph_Tbl at 0x8011e0b8
 * (the last byte is 0x03, so no alignment pad belongs to it). Indexed by Ending_msg_row_cnt in Ending_main. */
UCHAR Ending_msg_char_cnt_tbl[300] = {
	0x05, 0x03, 0x05, 0x05, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x05, 0x05, 0x03, 0x05, 0x03,
	0x06, 0x04, 0x05, 0x05, 0x03, 0x05, 0x05, 0x05, 0x0a, 0x05, 0x03, 0x0a, 0x05, 0x03, 0x03, 0x05,
	0x0a, 0x03, 0x05, 0x05, 0x03, 0x05, 0x05, 0x05, 0x04, 0x05, 0x05, 0x03, 0x0a, 0x05, 0x03, 0x05,
	0x03, 0x05, 0x05, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05, 0x03, 0x05, 0x0a,
	0x05, 0x03, 0x03, 0x05, 0x0a, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05,
	0x05, 0x03, 0x05, 0x0a, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x06, 0x03, 0x05,
	0x03, 0x0a, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05, 0x0a, 0x05, 0x03,
	0x05, 0x05, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x0a, 0x05, 0x05, 0x03, 0x05,
	0x05, 0x05, 0x03, 0x0a, 0x05, 0x03, 0x05, 0x05, 0x03, 0x05, 0x0a, 0x03, 0x05, 0x0a, 0x05, 0x05,
	0x03, 0x05, 0x05, 0x03, 0x03, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05, 0x06, 0x05, 0x05, 0x03,
	0x05, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05,
	0x0a, 0x05, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x03, 0x05, 0x05,
	0x0a, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x0a, 0x03, 0x05, 0x03, 0x03, 0x05, 0x0a, 0x05, 0x03,
	0x03, 0x05, 0x0a, 0x05, 0x05, 0x03, 0x03, 0x05, 0x03, 0x05, 0x05, 0x03, 0x03, 0x05, 0x03, 0x05,
	0x03, 0x05, 0x05, 0x03, 0x05, 0x0a, 0x05, 0x03, 0x05, 0x05, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05,
	0x03, 0x05, 0x05, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05, 0x03, 0x05, 0x03, 0x05, 0x05, 0x0a, 0x03,
	0x05, 0x03, 0x0a, 0x03, 0x05, 0x05, 0x03, 0x05, 0x05, 0x03, 0x05, 0x05, 0x06, 0x01, 0x05, 0x03,
	0x05, 0x05, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x05, 0x03, 0x03, 0x05, 0x05, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x05, 0x03, 0x05, 0x03, 0x04, 0x03
};
/* Glyph record (10 bytes) of Ending_glyph_Tbl (ENDING::8011e0b8 + n*10). The offsets and access widths
 * are the retail reads in Ending_draw_score (ENDING::8011bd40..8011bddc, s1 = record) and
 * Ending_draw_rank (s0 = record): lhu +0 tpage, lhu +2 clut, lbu +4 u, +5 v, +6 uw, +7 vh, +8 w, +9 h.
 * uw/vh are the texture width/height, w/h the screen width/height (equal except on the
 * three reward images, glyphs 54..56). Natural alignment gives these offsets; sizeof is 10. */
typedef struct {
	USHORT tpage;   /* +0 */
	USHORT clut;    /* +2 */
	UCHAR  u;       /* +4 */
	UCHAR  v;       /* +5 */
	UCHAR  uw;      /* +6 */
	UCHAR  vh;      /* +7 */
	UCHAR  w;       /* +8 */
	UCHAR  h;       /* +9 */
} ENDING_GLYPH;

/* Result-screen record (12 bytes) of Ending_result_rec_tbl (ENDING::8011e324 + n*12).
 * Offsets and access widths from retail Ending_draw_score (ENDING::8011bb94), which walks the
 * table with a2 = record and s7 = record + 6, and Ending_result_flg_set (ENDING::8011c140, v1 = record):
 *   +0 flg     lbu/sb 0(a2): bit 0 draw, bit 2 colour fades in, bit 3 colour fades out.
 *              Ending_result_flg_set stores Val (0, 1 or 5) and Ending_fade_unk stores 0 or 5 (draw + fade in).
 *   +1 attr    lbu -5(s7): bit 0x20 keeps the record off (its flg is cleared every frame).
 *   +2 ot      lbu -4(s7): ordering-table index, the quads go to G.pOt1 + ot.
 *   +3 row     lbu -3(s7): row number in Ending_glyph_idx_tbl (the row starts after `row` 0xff bytes).
 *   +4 x       lhu -2(s7): screen x of the first glyph; each glyph advances it by its w.
 *   +6 y       lhu 0(s7): screen y.
 *   +8 colour  lw/sw 2(s7): RGB added to the POLY_FT4 code word. The fades step it by Ending_scroll_spd
 *              and stop at 0 (faded out) or 0x808080 (faded in).
 * Natural alignment gives these offsets (no padding); sizeof is 12, and the int member keeps the
 * array 4-aligned, so the halfword and word accesses are aligned. */
typedef struct {
	UCHAR flg;      /* +0 */
	UCHAR attr;     /* +1 */
	UCHAR ot;       /* +2 */
	UCHAR row;      /* +3 */
	SHORT x;        /* +4 */
	SHORT y;        /* +6 */
	int   colour;   /* +8 */
} ENDING_RESULT_REC;

/* Records of Ending_result_rec_tbl that the code addresses by number. The text of each record is its
 * row in Ending_glyph_idx_tbl (see the table comments below). */
#define END_REC_RANK      4    /* records 4..8: rank letters "A".."E", record END_REC_RANK + Ending_rank_no */
#define END_REC_LEON_A    9    /* "LEON A" */
#define END_REC_CLAIRE_A  10   /* "CLAIRE A" */
#define END_REC_LEON_B    11   /* "LEON B" */
#define END_REC_CLAIRE_B  12   /* "CLAIRE B" */
#define END_REC_CONGRATS  13   /* "CONGRATULATIONS!!", first record of the result page (slot mask bits 13..17);
                                  its flg also gates Ending_draw_rank and its colour tints the time digits */
#define END_REC_ITEM_A    18   /* reward image, glyph 54 (Ending_item_ck(15)) */
#define END_REC_ITEM_B    19   /* reward image, glyph 55 (Ending_item_ck(17)) */
#define END_REC_ITEM_C    20   /* reward image, glyph 56 (Ending_item_ck(18)) */
#define END_REC_HUNK      31   /* "HUNK" */
#define END_REC_TOFU      32   /* "TOFU" */

/* DS 0x8011e0b8 (Ending_glyph_Tbl): font and image glyphs of the result screen, 57 records.
 * Size (largest reachable glyph index 56): Ending_glyph_idx_tbl holds glyphs 0..56 (largest 0x38, row 29),
 * and Ending_draw_rank reads glyphs 23..38 through the time digits (Ending_score_set stores
 * digit + 23, 32, 28, 0x24 and 0x26). No code writes the index list, so nothing else can be drawn.
 * ENDING.BIN holds five more records of the same font after glyph 56 (ENDING::8011e2f2..8011e323);
 * no retail code reaches them, so they are not defined here.
 * The letters were decoded from the rows of Ending_glyph_idx_tbl, which spell words with one
 * consistent map; the digits follow from Ending_score_set (glyph = digit + 23).
 * Initial values: the retail bytes, ENDING.BIN offset 0x40b8 (file offset = address - 0x8011a000), 570 bytes.
 *      tpage   clut    u     v     uw    vh    w     h */
ENDING_GLYPH Ending_glyph_Tbl[57] = {
	{ 0x000e, 0x79c0, 0x00, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /*  0 'A' */
	{ 0x000e, 0x79c0, 0x0f, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /*  1 'B' */
	{ 0x000e, 0x79c0, 0x1d, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /*  2 'C' */
	{ 0x000e, 0x79c0, 0x2b, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /*  3 'D' */
	{ 0x000e, 0x79c0, 0x3a, 0x00, 0x0b, 0x0e, 0x0b, 0x0e },   /*  4 'E' */
	{ 0x000e, 0x79c0, 0x47, 0x00, 0x0b, 0x0e, 0x0b, 0x0e },   /*  5 'F' */
	{ 0x000e, 0x79c0, 0x54, 0x00, 0x0e, 0x0e, 0x0e, 0x0e },   /*  6 'G' */
	{ 0x000e, 0x79c0, 0x71, 0x20, 0x0c, 0x0e, 0x0c, 0x0e },   /*  7 'H' */
	{ 0x000e, 0x79c0, 0x66, 0x00, 0x06, 0x0e, 0x06, 0x0e },   /*  8 'I' */
	{ 0x000e, 0x79c0, 0x70, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /*  9 'K' */
	{ 0x000e, 0x79c0, 0x7f, 0x00, 0x0b, 0x0e, 0x0b, 0x0e },   /* 10 'L' */
	{ 0x000e, 0x79c0, 0x8c, 0x00, 0x0e, 0x0e, 0x0e, 0x0e },   /* 11 'M' */
	{ 0x000e, 0x79c0, 0x9a, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /* 12 'N' */
	{ 0x000e, 0x79c0, 0xa8, 0x00, 0x0e, 0x0e, 0x0e, 0x0e },   /* 13 'O' */
	{ 0x000e, 0x79c0, 0xb7, 0x00, 0x0b, 0x0e, 0x0b, 0x0e },   /* 14 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0xc5, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /* 15 'R' */
	{ 0x000e, 0x79c0, 0xd4, 0x00, 0x0a, 0x0e, 0x0a, 0x0e },   /* 16 'S' */
	{ 0x000e, 0x79c0, 0xe1, 0x00, 0x0c, 0x0e, 0x0c, 0x0e },   /* 17 'T' */
	{ 0x000e, 0x79c0, 0xee, 0x00, 0x0d, 0x0e, 0x0d, 0x0e },   /* 18 'U' */
	{ 0x000e, 0x79c0, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 19 'V' */
	{ 0x000e, 0x79c0, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 20 'W' */
	{ 0x000e, 0x79c0, 0x1c, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 21 'X' */
	{ 0x000e, 0x79c0, 0x2a, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 22 'Y' */
	{ 0x000e, 0x79c0, 0x38, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 23 digit '0' */
	{ 0x000e, 0x79c0, 0x46, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 24 digit '1' */
	{ 0x000e, 0x79c0, 0x54, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 25 digit '2' */
	{ 0x000e, 0x79c0, 0x62, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 26 digit '3' */
	{ 0x000e, 0x79c0, 0x70, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 27 digit '4' */
	{ 0x000e, 0x79c0, 0x7e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 28 digit '5' */
	{ 0x000e, 0x79c0, 0x8c, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 29 digit '6' */
	{ 0x000e, 0x79c0, 0x9a, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 30 digit '7' */
	{ 0x000e, 0x79c0, 0xa8, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 31 digit '8' */
	{ 0x000e, 0x79c0, 0xb6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 32 digit '9' */
	{ 0x000e, 0x79c0, 0xc4, 0x0e, 0x0a, 0x0e, 0x0a, 0x0e },   /* 33 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0xd5, 0x0e, 0x08, 0x0e, 0x08, 0x0e },   /* 34 '!' */
	{ 0x000e, 0x79c0, 0xe0, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e },   /* 35 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0xf0, 0x0e, 0x0a, 0x0e, 0x0a, 0x0e },   /* 36 time separator (Ending_label_chr_a/b) */
	{ 0x000e, 0x79c0, 0x98, 0x20, 0x0a, 0x0e, 0x0a, 0x0e },   /* 37 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0xf1, 0x20, 0x08, 0x0e, 0x08, 0x0e },   /* 38 blank (word gap in the rows) */
	{ 0x000e, 0x79c0, 0x00, 0x20, 0x2a, 0x0e, 0x2a, 0x0e },   /* 39 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0x38, 0x20, 0x2a, 0x0e, 0x2a, 0x0e },   /* 40 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0x00, 0x20, 0x38, 0x0e, 0x38, 0x0e },   /* 41 not used by the rows or the digit code */
	{ 0x000e, 0x79c0, 0x38, 0x20, 0x38, 0x0e, 0x38, 0x0e },   /* 42 not used by the rows or the digit code */
	{ 0x000c, 0x7980, 0x00, 0x00, 0xfe, 0x30, 0xfe, 0x30 },   /* 43 image */
	{ 0x000c, 0x7980, 0x00, 0x30, 0xfe, 0x20, 0xfe, 0x20 },   /* 44 image */
	{ 0x000c, 0x7980, 0x00, 0x50, 0xfe, 0x20, 0xfe, 0x20 },   /* 45 image */
	{ 0x000c, 0x7980, 0x00, 0x70, 0xfe, 0x20, 0xfe, 0x20 },   /* 46 image */
	{ 0x000a, 0x7940, 0x00, 0xa0, 0xfe, 0x20, 0xfe, 0x20 },   /* 47 image */
	{ 0x000a, 0x7940, 0x00, 0xc0, 0xfe, 0x20, 0xfe, 0x20 },   /* 48 image */
	{ 0x000a, 0x7940, 0x00, 0xe0, 0xfe, 0x1f, 0xfe, 0x1f },   /* 49 image */
	{ 0x000a, 0x7900, 0x00, 0x00, 0xfe, 0x40, 0xfe, 0x40 },   /* 50 image */
	{ 0x000a, 0x7900, 0x00, 0x40, 0xfe, 0x20, 0xfe, 0x20 },   /* 51 image */
	{ 0x000a, 0x7900, 0x00, 0x60, 0xfe, 0x20, 0xfe, 0x20 },   /* 52 image */
	{ 0x000a, 0x7900, 0x00, 0x80, 0xfe, 0x20, 0xfe, 0x20 },   /* 53 image */
	{ 0x0008, 0x7800, 0x00, 0x00, 0x70, 0x48, 0x54, 0x36 },   /* 54 image, reward item A */
	{ 0x0008, 0x7840, 0x00, 0x48, 0x70, 0x48, 0x54, 0x36 },   /* 55 image, reward item B */
	{ 0x0008, 0x7880, 0x00, 0x90, 0x70, 0x48, 0x54, 0x36 }    /* 56 image, reward item C */
};

/* DS 0x8011e324 (Ending_result_rec_tbl): the 33 records of the result screen.
 * Size: Ending_draw_score walks 0x21 records (li t2,0x21 at ENDING::8011bba0, stride addiu 0xc), and
 * Ending_glyph_idx_tbl follows at 0x8011e324 + 33*12 = 0x8011e4b0. Ending_result_flg_set covers
 * records 0..30 (one slot-mask bit each); records 31 and 32 are set by number.
 * Initial values: the retail bytes, ENDING.BIN offset 0x4324, 396 bytes.
 *      flg   attr  ot  row   x    y    colour */
ENDING_RESULT_REC Ending_result_rec_tbl[33] = {
	{ 0x00, 0x00, 10, 16,  32,  90, 0x00000000 },   /*  0: row 16, image glyph 43 */
	{ 0x00, 0x00, 10, 17,  32,  97, 0x00000000 },   /*  1: row 17, image glyph 44 */
	{ 0x00, 0x20, 10, 18,  32, 180, 0x00808080 },   /*  2: row 18, image glyph 45 */
	{ 0x00, 0x20, 10, 19,  32, 180, 0x00808080 },   /*  3: row 19, image glyph 46 */
	{ 0x00, 0x00, 10,  7, 240,  76, 0x00000000 },   /*  4: row  7, "A" (END_REC_RANK) */
	{ 0x00, 0x00, 10,  8, 240,  76, 0x00000000 },   /*  5: row  8, "B" */
	{ 0x00, 0x00, 10,  9, 240,  76, 0x00000000 },   /*  6: row  9, "C" */
	{ 0x00, 0x00, 10, 10, 240,  76, 0x00000000 },   /*  7: row 10, "D" */
	{ 0x00, 0x00, 10, 11, 240,  76, 0x00000000 },   /*  8: row 11, "E" */
	{ 0x00, 0x00, 10,  2, 210, 108, 0x00000000 },   /*  9: row  2, "LEON A" */
	{ 0x00, 0x00, 10,  3, 201, 108, 0x00000000 },   /* 10: row  3, "CLAIRE A" */
	{ 0x00, 0x00, 10,  4, 210, 108, 0x00000000 },   /* 11: row  4, "LEON B" */
	{ 0x00, 0x00, 10,  5, 201, 108, 0x00000000 },   /* 12: row  5, "CLAIRE B" */
	{ 0x00, 0x00, 10,  0,  60,  25, 0x00000000 },   /* 13: row  0, "CONGRATULATIONS!!" */
	{ 0x00, 0x00, 10,  6,  30,  76, 0x00000000 },   /* 14: row  6, "RANKING" */
	{ 0x00, 0x00, 10,  1,  30, 108, 0x00000000 },   /* 15: row  1, "CLEAR GAME" */
	{ 0x00, 0x00, 10, 12,  30, 140, 0x00000000 },   /* 16: row 12, "TOTAL TIME" */
	{ 0x00, 0x00, 10, 13,  30, 172, 0x00000000 },   /* 17: row 13, "NUMBER OF SAVES" */
	{ 0x00, 0x10, 10, 27,  28,  80, 0x00000000 },   /* 18: row 27, image glyph 54 (END_REC_ITEM_A) */
	{ 0x00, 0x11, 10, 28, 122,  80, 0x00000000 },   /* 19: row 28, image glyph 55 (END_REC_ITEM_B) */
	{ 0x00, 0x12, 10, 29, 216,  80, 0x00000000 },   /* 20: row 29, image glyph 56 (END_REC_ITEM_C) */
	{ 0x00, 0x00, 10, 21,  32, 150, 0x00808080 },   /* 21: row 21, image glyph 48 */
	{ 0x00, 0x20, 10, 22,  32, 150, 0x00808080 },   /* 22: row 22, image glyph 49 */
	{ 0x00, 0x00, 10, 14,  99,  30, 0x00808080 },   /* 23: row 14, "YOUR REWARD" */
	{ 0x00, 0x00, 10, 15,  71, 200, 0x00808080 },   /* 24: row 15, "CHECK ITEM BOX!!" */
	{ 0x00, 0x20, 10, 20,  32, 180, 0x00808080 },   /* 25: row 20, image glyph 47 */
	{ 0x00, 0x20, 10, 20,  32, 180, 0x00808080 },   /* 26: row 20, image glyph 47 */
	{ 0x00, 0x00, 10, 23,  32,  70, 0x00000000 },   /* 27: row 23, image glyph 50 */
	{ 0x00, 0x00, 10, 24,  32, 100, 0x00000000 },   /* 28: row 24, image glyph 51 */
	{ 0x00, 0x20, 10, 25,  32, 180, 0x00808080 },   /* 29: row 25, image glyph 52 */
	{ 0x00, 0x20, 10, 26,  32, 180, 0x00808080 },   /* 30: row 26, image glyph 53 */
	{ 0x00, 0x00, 10, 30, 224, 108, 0x00000000 },   /* 31: row 30, "HUNK" (END_REC_HUNK) */
	{ 0x00, 0x00, 10, 31, 217, 108, 0x00000000 }    /* 32: row 31, "TOFU" (END_REC_TOFU) */
};

/* DS 0x8011e4b0 (Ending_glyph_idx_tbl): glyph index rows of the result records, each ended by 0xff.
 * Ending_draw_score finds row n by skipping n 0xff bytes, then draws glyphs up to the next 0xff.
 * Size: the largest row number in Ending_result_rec_tbl is 31 (record 32), and no code writes a
 * row byte or this list (the only reference is the lui at ENDING::8011bc38 with the addiu at 8011bc3c), so 32 rows are
 * reachable; the 32nd 0xff (ENDING::8011e55c) is the last byte, 173 bytes in all.
 * Initial values: the retail bytes, ENDING.BIN offset 0x44b0, 173 bytes. */
UCHAR Ending_glyph_idx_tbl[173] = {
	0x02, 0x0d, 0x0c, 0x06, 0x0f, 0x00, 0x11, 0x12, 0x0a, 0x00, 0x11, 0x08, 0x0d, 0x0c, 0x10, 0x22, 0x22, 0xff,   /* row  0: "CONGRATULATIONS!!" */
	0x02, 0x0a, 0x04, 0x00, 0x0f, 0x26, 0x06, 0x00, 0x0b, 0x04, 0xff,   /* row  1: "CLEAR GAME" */
	0x0a, 0x04, 0x0d, 0x0c, 0x26, 0x00, 0xff,   /* row  2: "LEON A" */
	0x02, 0x0a, 0x00, 0x08, 0x0f, 0x04, 0x26, 0x00, 0xff,   /* row  3: "CLAIRE A" */
	0x0a, 0x04, 0x0d, 0x0c, 0x26, 0x01, 0xff,   /* row  4: "LEON B" */
	0x02, 0x0a, 0x00, 0x08, 0x0f, 0x04, 0x26, 0x01, 0xff,   /* row  5: "CLAIRE B" */
	0x0f, 0x00, 0x0c, 0x09, 0x08, 0x0c, 0x06, 0xff,   /* row  6: "RANKING" */
	0x00, 0xff,   /* row  7: "A" */
	0x01, 0xff,   /* row  8: "B" */
	0x02, 0xff,   /* row  9: "C" */
	0x03, 0xff,   /* row 10: "D" */
	0x04, 0xff,   /* row 11: "E" */
	0x11, 0x0d, 0x11, 0x00, 0x0a, 0x26, 0x11, 0x08, 0x0b, 0x04, 0xff,   /* row 12: "TOTAL TIME" */
	0x0c, 0x12, 0x0b, 0x01, 0x04, 0x0f, 0x26, 0x0d, 0x05, 0x26, 0x10, 0x00, 0x13, 0x04, 0x10, 0xff,   /* row 13: "NUMBER OF SAVES" */
	0x16, 0x0d, 0x12, 0x0f, 0x26, 0x0f, 0x04, 0x14, 0x00, 0x0f, 0x03, 0xff,   /* row 14: "YOUR REWARD" */
	0x02, 0x07, 0x04, 0x02, 0x09, 0x26, 0x08, 0x11, 0x04, 0x0b, 0x26, 0x01, 0x0d, 0x15, 0x22, 0x22, 0xff,   /* row 15: "CHECK ITEM BOX!!" */
	0x2b, 0xff,   /* row 16: image glyph 43 */
	0x2c, 0xff,   /* row 17: image glyph 44 */
	0x2d, 0xff,   /* row 18: image glyph 45 */
	0x2e, 0xff,   /* row 19: image glyph 46 */
	0x2f, 0xff,   /* row 20: image glyph 47 */
	0x30, 0xff,   /* row 21: image glyph 48 */
	0x31, 0xff,   /* row 22: image glyph 49 */
	0x32, 0xff,   /* row 23: image glyph 50 */
	0x33, 0xff,   /* row 24: image glyph 51 */
	0x34, 0xff,   /* row 25: image glyph 52 */
	0x35, 0xff,   /* row 26: image glyph 53 */
	0x36, 0xff,   /* row 27: image glyph 54 */
	0x37, 0xff,   /* row 28: image glyph 55 */
	0x38, 0xff,   /* row 29: image glyph 56 */
	0x07, 0x12, 0x0c, 0x09, 0xff,   /* row 30: "HUNK" */
	0x11, 0x0d, 0x05, 0x12, 0xff    /* row 31: "TOFU" */
};
/* TIM sector-table at ENDING::8011e560: 12 USHORT entries (0x18 bytes), indexed by Tim_no & 0xff.
 * Entries 0-8 are FILECODE TIM ids (FILECODE.H END10_TIM..END18_TIM); entries 9-11 are raw
 * values, not file codes. Retail call sites pass Tim_no 0-8 (largest index 8).
 *   [0]  byte 0x00  0x009A  END10_TIM (154)
 *   [1]  byte 0x02  0x009B  END11_TIM (155)
 *   [2]  byte 0x04  0x009C  END12_TIM (156)
 *   [3]  byte 0x06  0x009D  END13_TIM (157)
 *   [4]  byte 0x08  0x009E  END14_TIM (158)
 *   [5]  byte 0x0A  0x009F  END15_TIM (159)
 *   [6]  byte 0x0C  0x00A0  END16_TIM (160)
 *   [7]  byte 0x0E  0x00A1  END17_TIM (161)
 *   [8]  byte 0x10  0x00A2  END18_TIM (162)
 *   [9]  byte 0x12  0xF0D2  raw value, not a file code
 *   [10] byte 0x14  0x0014  raw value, not a file code
 *   [11] byte 0x16  0x0000  raw value, not a file code */
/* DS 0x8011e560 (Ending_tim_sect_tbl) */
USHORT Ending_tim_sect_tbl[12] = {   /* TIM sector-table (USHORT entries) */
	0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F, 0x00A0, 0x00A1,
	0x00A2, 0xF0D2, 0x0014, 0x0000,
};

/* Result/score display state (ENDING overlay data).
 * The former scalars Ending_result_rec_Y (0x8011e32a), Ending_result_mode_flg (0x8011e3c0),
 * Ending_result_rank_col (0x8011e3c8), Ending_item_a/b/c_col (0x8011e404/0x8011e410/0x8011e41c) and
 * Ending_fade_scratch0/1 (0x8011e498/0x8011e4a4) lie inside Ending_result_rec_tbl; they are now the
 * members record 0 .y, END_REC_CONGRATS .flg and .colour, END_REC_ITEM_A/B/C .colour and
 * END_REC_HUNK/END_REC_TOFU .flg. */
/* DS 0x8011e578 (Ending_scroll_step) */
SHORT  Ending_scroll_step = 0x0000;   /* timeline step increment */
/* DS 0x8011e57c (Ending_scroll_spd) */
int    Ending_scroll_spd = 0x00000000;   /* stat scroll speed */
/* DS 0x8011e580 (Ending_msg_col) */
int    Ending_msg_col = 0x00000000;   /* message column index */
/* DS 0x8011e588 (Ending_msg_line_buf) */
UCHAR  Ending_msg_line_buf = 0x00;   /* message line buffer */
/* DS 0x8011e589 (Ending_msg_line_buf1) */
UCHAR  Ending_msg_line_buf1[31];   /* message line buffer, +1 */
/* DS 0x8011e5a8 (Ending_msg_row) */
int    Ending_msg_row = 0x00000000;   /* message row index */

/* Overlay runtime state */
/* DS 0x80121340 (Ending_type_cnt) */
int    Ending_type_cnt = 0x00000000;   /* text typewriter counter */
/* DS 0x80121344 (Ending_disp_mode) */
int    Ending_disp_mode = 0x00000000;   /* display mode */
/* DS 0x80121348 (Ending_result_slot_mask) */
UINT   Ending_result_slot_mask = 0x00000000;   /* result slot mask */
/* DS 0x80121370 (Ending_fade_ofs) */
SHORT  Ending_fade_ofs = 0x0000;   /* scroll/fade offset */
/* DS 0x80121374 (Ending_msg_state) */
int    Ending_msg_state = 0x00000000;   /* message state (switch selector) */
/* DS 0x80121378 (Ending_room_work): ROOM_WORK block that Ending_init hands to Snd_sys_room_set through
 * G.pRoom. Retail Ending_init stores Reverb_lv (+6), pEdt0 (+8), pVh0 (+0xc) and pVb0 (+0x10) into it;
 * sizeof(ROOM_WORK) is 0x84. The member pointers point into Ending_work_buf (see below). */
ROOM_WORK Ending_room_work;
/* DS 0x801213fc (Ending_fade_dir) */
UCHAR  Ending_fade_dir = 0x00;   /* fade direction state */
/* DS 0x80121400 (Ending_timeline_cnt) */
int    Ending_timeline_cnt = 0x00000000;   /* timeline frame counter */
/* DS 0x80121404 (Ending_msg_col_base) */
UCHAR  Ending_msg_col_base = 0x00;   /* message table column base */
/* DS 0x80125288 (Ending_load_err_flg) */
UCHAR  Ending_load_err_flg = 0x00;   /* load error flags (byte store) */
/* DS 0x80125290 (Ending_time_dig_d10) */
UCHAR  Ending_time_dig_d10 = 0x00;   /* time/score digit 0 */
/* DS 0x80125291 (Ending_time_dig_d1) */
UCHAR  Ending_time_dig_d1 = 0x00;   /* time/score digit 1 */
/* DS 0x80125292 (Ending_label_chr_a) */
UCHAR  Ending_label_chr_a = 0x00;   /* label char */
/* DS 0x80125293 (Ending_time_dig_h10) */
UCHAR  Ending_time_dig_h10 = 0x00;   /* time digit (tens, hour) */
/* DS 0x80125294 (Ending_time_dig_h1) */
UCHAR  Ending_time_dig_h1 = 0x00;   /* time digit (ones, hour) */
/* DS 0x80125295 (Ending_label_chr_b) */
UCHAR  Ending_label_chr_b = 0x00;   /* label char */
/* DS 0x80125296 (Ending_time_dig_m10) */
UCHAR  Ending_time_dig_m10 = 0x00;   /* time digit (tens, minute) */
/* DS 0x80125297 (Ending_time_dig_m1) */
UCHAR  Ending_time_dig_m1 = 0x00;   /* time digit (ones, minute) */
/* DS 0x80125298 (Ending_bonus_dig_10) */
UCHAR  Ending_bonus_dig_10 = 0x00;   /* bonus digit (tens) */
/* DS 0x80125299 (Ending_bonus_dig_1) */
UCHAR  Ending_bonus_dig_1 = 0x00;   /* bonus digit (ones) */
/* DS 0x8012529c (Ending_stat_rec_ptr) */
UCHAR *Ending_stat_rec_ptr = 0;   /* stat-display record pointer */
/* DS 0x801252a0 (Ending_rank_no) */
USHORT Ending_rank_no = 0x0000;   /* final rank index */
/* DS 0x801252a2 (Ending_score_item) */
UCHAR  Ending_score_item = 0x00;   /* score component (item/ammo) */
/* DS 0x801252a3 (Ending_score_life) */
UCHAR  Ending_score_life = 0x00;   /* score component (health) */
/* DS 0x801252a4 (Ending_score_time) */
UCHAR  Ending_score_time = 0x00;   /* score component (time) */
/* DS 0x801252a8 (Ending_msg_draw_idx) */
int    Ending_msg_draw_idx = 0x00000000;   /* message draw index */
/* DS 0x801252ac (Ending_pPoly) */
POLY_FT4 *Ending_pPoly = 0;   /* next free POLY_FT4 in OT buffer */
/* DS 0x801252b0 (Ending_state_flg) */
ULONG  Ending_state_flg = 0x00000000;   /* ending state flag word */
/* DS 0x801252d4 (Ending_zombie_cnt) */
USHORT Ending_zombie_cnt = 0x0000;   /* zombie count (save copy) */
/* DS 0x801252d6 (Ending_save_rank_in) */
USHORT Ending_save_rank_in = 0x0000;   /* save field (rank input) */
/* DS 0x801252d8 (Ending_save_bonus_in) */
USHORT Ending_save_bonus_in = 0x0000;   /* save field (bonus input) */
/* DS 0x801252e0 (Ending_save_weapon_mugen) */
UINT   Ending_save_weapon_mugen = 0x00000000;   /* save field (weapon mugen) */
/* The copy of G.SaveData from Game_play_time (0x598 bytes, memcpy16 in Ending_init) holds the flag words the ending tests:
 * Item_flg at copy +0x218 (DS 0x801254d0), Zapping_flg[0] at +0x388 (DS 0x80125640) and Zapping_flg[1] at +0x38c (DS 0x80125644). */
/* DS 0x80125796 (Ending_save_zapping_cnt) */
UCHAR  Ending_save_zapping_cnt = 0x00;   /* save field (zapping count) */
/* DS 0x80125850 (Ending_step_flg) */
UCHAR  Ending_step_flg = 0x00;   /* sub-step done flag (project name, no retail name) */
/* DS 0x80125851 (Ending_exit_req) */
UCHAR  Ending_exit_req = 0x00;   /* exit-request flag */
/* DS 0x80125852 (Ending_time_hour) */
UCHAR  Ending_time_hour = 0x00;   /* time-digit scratch (hours) */
/* DS 0x80125853 (Ending_time_min) */
UCHAR  Ending_time_min = 0x00;   /* time-digit scratch (minutes) */
/* DS 0x80125854 (Ending_msg_row_cnt) */
int    Ending_msg_row_cnt = 0x00000000;   /* message row counter */
/* DS 0x80125858 (Ending_time_sec) */
UCHAR  Ending_time_sec = 0x00;   /* time-digit scratch (seconds) */
/* DS 0x80125859 (Ending_bonus_val) */
UCHAR  Ending_bonus_val = 0x00;   /* bonus-digit scratch */
/* shared TIM load buffer at DS 0x80198000 is Bg_img_buf (DIESPR.C) */
/* DS 0x8011a004 (Ending_mess_end1a) */
UCHAR Ending_mess_end1a[16] = {   /* overlay string (END1A_TIM_LD) */
	0x45, 0x4E, 0x44, 0x31, 0x41, 0x5F, 0x54, 0x49, 0x4D, 0x5F, 0x4C, 0x44, 0x00, 0x00, 0x00, 0x00,
};
/* DS 0x8011a014 (Ending_mess_end00) */
UCHAR Ending_mess_end00[16] = {   /* overlay string (END00_TIM_LD) */
	0x45, 0x4E, 0x44, 0x30, 0x30, 0x5F, 0x54, 0x49, 0x4D, 0x5F, 0x4C, 0x44, 0x00, 0x00, 0x00, 0x00,
};
/* DS 0x8011a024 (Ending_mess_room800) */
UCHAR Ending_mess_room800[16] = {   /* overlay string (ROOM800_SND_LD) */
	0x52, 0x4F, 0x4F, 0x4D, 0x38, 0x30, 0x30, 0x5F, 0x53, 0x4E, 0x44, 0x5F, 0x4C, 0x44, 0x00, 0x00,
};
/* DS 0x8011a0c8 (Ending_mess_init_tbl) */
UCHAR Ending_mess_init_tbl[12] = {   /* overlay string (Init_tbl) */
	0x49, 0x6E, 0x69, 0x74, 0x20, 0x74, 0x62, 0x6C,
	0x00, 0x00, 0x00, 0x00,
};
/* DS 0x8011a0d4 (Ending_mess_end_tim) */
UCHAR Ending_mess_end_tim[16] = {   /* overlay string (END_TIM_LD) */
	0x45, 0x4E, 0x44, 0x5F, 0x54, 0x49, 0x4D, 0x5F, 0x4C, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* Forward declarations (file-local helpers; ENDING.H declares no entry point) */
static void Ending_init(void);
static void Ending_exit(void);
static void Ending_fade_out(void);
static void Ending_fade_in(void);
static void Ending_draw_score(void);
static void Ending_draw_rank(void);
static void Ending_result_flg_set(UINT Slot_mask, UCHAR Val, char Clr_sw);
static void Ending_score_set(void);
static int  Ending_item_ck(char Item_id);
static void Ending_next_set(void);
static void Ending_inittbl_load(void);
static void Ending_tim_load(UINT Tim_no);
static void Ending_fade_set(int Fade_no, USHORT Ctrl, UINT Col, ULONG *pPos);
static void Ending_timeline(void);
static void Ending_main(void);
static void Ending_moji_draw(void);
static void Ending_stat_flg_set(UINT Slot_mask, UCHAR Val, char Clr_sw);

/* Ending_init - ENDING::8011a18c
 * Ending initialization: loads TIM graphics, sound room, copies save data,
 * resets fade buffers, and calls score init. */
/* DS 0x8011a18c (Ending_init) */
static void Ending_init(void)
{
	GLOBAL *pG = &G;
	UCHAR cbuf_save;
	ULONG read_size;
	BGMCTR bgm_ctr[4];
	/* Frame reservation: retail Ending_init reserves sp+0x10..0x37 (40 bytes, between the outgoing
	 * argument area and bgm_ctr) and never reads it, so this volatile pad keeps the frame at 0x50. */
	volatile UCHAR frame_pad[0x28];

	Snd_bgm_fade_ON(0xb4, 24);
	pG->Status_flg &= ~STAT_CINESCO;
	Bg_set_mode(2, 0);
	Main.Bg_chg_flg = 0;
	Pad_rep_set(0xf01c, 0x308);
	SetGeomScreen(200);
	SetGeomOffset(0, 0);
	pG->pFree_work = Ending_work_buf;
	Main.Vloop = 0;
	Ending_load_err_flg = 0;
	if ((pG->System_flg & SYS_EX_BATTLE) == 0) {
		do {
			read_size = Cd_read(END00_TIM, (ULONG *)pG->pFree_work, 1, Ending_mess_end00);
		} while (read_size + 1 == 0);
		pG->Cclut = 16;
		pG->Ctpage = 8;
	}
	else {
		do {
			read_size = Cd_read(END19_TIM, (ULONG *)pG->pFree_work, 1, Ending_mess_end1a);
		} while (read_size + 1 == 0);
		pG->Cclut = 17;
		pG->Ctpage = 12;
	}
	Mapping_tim((ULONG *)pG->pFree_work);
	do {
		read_size = Cd_read(ROOM800_SND, (ULONG *)pG->pFree_work, 1, Ending_mess_room800);
	} while (read_size + 1 == 0);
	{
		ROOM_WORK *pRoom = &Ending_room_work;
		/* G.pFree_work still equals Ending_work_buf here (set at the top, advanced only after this block) */
		pRoom->Reverb_lv = 0;
		pRoom->pEdt0 = (ULONG *)pG->pFree_work;
		pRoom->pVh0 = (ULONG *)(pG->pFree_work + 0xc0);
		pRoom->pVb0 = (ULONG *)(pG->pFree_work + 0xce0);
		pG->pRoom = pRoom;
	}
	pG->pFree_work += read_size;
	Snd_sys_room_set();
	Ending_fade_dir = 1;
	if ((pG->Status_flg & STAT_SPECIAL_RUN) == 0) {
		Ending_disp_mode = 0;
	}
	else {
		Ending_disp_mode = 0x11;
	}
	if ((pG->System_flg & SYS_EX_BATTLE) != 0) {
		pG->ExData[2]    = (UCHAR)pG->SaveData.Sce_timer;
		{
			UCHAR *pSrc = (UCHAR *)&pG->SaveData.ExData3[8];
			int i;
			for (i = 0; i < 0x3c; i++) {
				pG->ExData[4 + i] = pSrc[i];
			}
		}
		pG->ExData[3]    = (UCHAR)pG->SaveData.Player_life;
	}
	Ending_timeline_cnt = 0;
	Ending_state_flg = 0x2000;
	Ending_result_slot_mask = 0;
	memcpy16(Ending_save_copy, &pG->SaveData.Game_play_time, 0x598);
	Ending_rank_no = 0;
	Ending_score_time = 0;
	Ending_score_life = 0;
	Ending_score_item = 0;
	Ending_score_set();
	if ((pG->System_flg & SYS_EX_BATTLE) != 0) {
		do {
			Task_sleep(1);
		} while (pEdt_adr[0].id_check != 0);
		bgm_ctr[0].Main = 56;
		bgm_ctr[0].Sub = 0xff;
		pBgm = bgm_ctr;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Snd_bgm_ctr(0x1000000);
	}
	cbuf_save = pG->Cbuf_id;
	pG->Cbuf_id = 0;
	Fade_adjust(0, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(1, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(2, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(3, 0xffffffff, 0xffffff, NULL);
	pG->Cbuf_id = 1;
	Fade_adjust(0, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(1, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(2, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(3, 0xffffffff, 0xffffff, NULL);
	pG->Cbuf_id = cbuf_save;
	return;
}

/* Ending_exit - ENDING::8011a630
 * Ending teardown / return to title: waits 60 frames, calls Init_global,
 * sets Task_lchain to restart. */
/* DS 0x8011a630 (Ending_exit) */
static void Ending_exit(void)
{
	int wait_cnt;

	wait_cnt = 60;
	do {
		wait_cnt--;
		Bg_set_mode(2, 0);
		Task_sleep(1);
	} while (wait_cnt != 0);
	Init_global();
	Bg_set_mode(2, 0);
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		G.ExData[0] = 1;
	}
	Task_lchain(0);
	return;
}

/* Ending_fade_out - ENDING::8011a6b0
 * Fade-out / screen transition: sets fade-out and toggles bg mode bits. */
/* DS 0x8011a6b0 (Ending_fade_out) */
static void Ending_fade_out(void)
{
	ULONG fade_sts;

	if ((Ending_state_flg & 1) == 0) {
		Fade_set(0x200, -0x100, 7, 1);
		Ending_fade_dir = 0;
		if ((Ending_state_flg & 0x800) == 0) {
			Ending_state_flg |= 1;
			Bg_set_mode(2, 0);
		}
		else {
			Ending_state_flg |= 1;
			Bg_set_mode(0, 0);
			Ending_state_flg ^= 0x800;
		}
		if ((Ending_state_flg & 0x1000) != 0) {
			Ending_result_flg_set(Ending_result_slot_mask, 1, 0);
			Ending_result_slot_mask = 0;
			Ending_state_flg ^= 0x1000;
		}
	}
	else {
		fade_sts = Fade_status(0);
		if (fade_sts != 0) {
			Ending_fade_dir = 1;
			Ending_state_flg ^= 1;
		}
	}
	return;
}

/* Ending_fade_in - ENDING::8011a7b4
 * Fade-in / screen restore: sets fade-in and updates control flags. */
/* DS 0x8011a7b4 (Ending_fade_in) */
static void Ending_fade_in(void)
{
	ULONG fade_sts;
	UINT new_state;

	if ((Ending_state_flg & 2) == 0) {
		Fade_set(0x200, 0x100, 7, 1);
		Ending_fade_dir = 2;
		Ending_state_flg |= 2;
	}
	else {
		fade_sts = Fade_status(0);
		if (fade_sts != 0) {
			new_state = Ending_state_flg ^ 2;
			if ((new_state & 4) == 0) {
				if ((new_state & 8) == 0) {
					Ending_fade_dir = 0;
					Ending_state_flg = new_state;
				}
				else {
					Ending_fade_dir = 1;
					Ending_state_flg ^= 0xa;
				}
			}
			else {
				Ending_fade_dir = 3;
				Ending_state_flg ^= 6;
			}
			if ((Ending_state_flg & 0x2000) != 0) {
				Ending_state_flg ^= 0x2000;
				Ending_result_flg_set(0xffffffff, 0, 1);
				Ending_result_rec_tbl[END_REC_HUNK].flg = 0;
				Ending_result_rec_tbl[END_REC_TOFU].flg = 0;
			}
			Bg_set_mode(2, 0);
		}
	}
	return;
}

/* Ending_fade_unk - ENDING::8011a8e4
 * Fade direction 1 handler (entry 1 of Ending_fade_tbl). Runs the ending scene
 * state machine: a 22-way switch on Ending_disp_mode (jump table ENDING::8011a034;
 * modes 22 and up do nothing), then counts Ending_timeline_cnt up by one.
 * Timeline thresholds are frame counts. Ending_result_rec_tbl[n].flg is the flag byte
 * of result record n (retail stores sb at 0x8011e324 + 12 * n). */
/* DS 0x8011a8e4 (Ending_fade_unk) */
static void Ending_fade_unk(void)
{
	/* retail reserves 12 BGMCTR entries (local_28[12]) and writes only element 0 */
	BGMCTR bgm_ctr[12];
	/* retail stores 600 and 1200 at sp+0x10 and sp+0x12 and never reads them; volatile keeps those 8 bytes in the frame */
	volatile SHORT rsv[4];
	rsv[0] = 600;
	rsv[1] = 1200;

	switch (Ending_disp_mode) {
	case 0:
		if (pEdt_adr[0].id_check == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				*(USHORT *)bgm_ctr = 0xff26;
				pBgm = bgm_ctr;
				Seq_ctr[0].Ctrl = 0;
				Seq_ctr[0].Vol.left = 100;
				Seq_ctr[0].Vol.right = 100;
				Snd_bgm_main();
				Snd_bgm_ctr(0x01000000);
				Ending_disp_mode = 1;
				Ending_timeline_cnt = 0;
			}
			else {
				Init_movie_work(0x0e);
				Ending_disp_mode = 3;
				Ending_timeline_cnt = 0;
			}
		}
		break;

	case 1:
		if (Ending_timeline_cnt == 300) {
			Init_movie_work(0x11);
			Ending_disp_mode = 3;
			Ending_timeline_cnt = 0;
		}
		break;

	case 2:
		break;

	case 3:
		if ((G.System_flg & SYS_STR_MOVIE) == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				Ending_state_flg |= 0x800;
				Snd_bgm_fade_ON(0xb4, 0x18);
			}
			Ending_disp_mode = 4;
			Ending_timeline_cnt = 0;
		}
		break;

	case 4:
		if (pEdt_adr[0].id_check == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				/* Main is the low byte and Sub the high byte, so one halfword store sets both (retail uses sh too) */
				*(USHORT *)bgm_ctr = 0xffff;
				pBgm = bgm_ctr;
				Seq_ctr[0].Ctrl = 0;
				Seq_ctr[0].Vol.left = 100;
				Seq_ctr[0].Vol.right = 100;
				Snd_bgm_main();
			}
			Ending_tim_load((G.Status_flg & STAT_PLAYER) != 0);
			Ending_fade_set(0, 0, 0, NULL);
			Ending_state_flg |= 0x800;
			Ending_fade_out();
			*(USHORT *)bgm_ctr = 0xff2a;
			pBgm = bgm_ctr;
			Seq_ctr[0].Ctrl = 0;
			Seq_ctr[0].Vol.left = 100;
			Seq_ctr[0].Vol.right = 100;
			Snd_bgm_main();
			Snd_bgm_ctr(0x01000000);
			Ending_disp_mode = 5;
			Ending_timeline_cnt = 0;
			Ending_step_flg = 0;
		}
		break;

	case 5:
		if ((G.Padd1 & 0x800) != 0) {
			if (Ending_timeline_cnt < 300) {
				if (Ending_step_flg == 0) {
					Ending_timeline_cnt = 480;
				}
			}
			else {
				if (Ending_step_flg == 0) {
					Ending_timeline_cnt = 480;
				}
				else if (Ending_step_flg == 1) {
					Ending_timeline_cnt = 1201;
				}
			}
		}
		if (Ending_timeline_cnt == 480 && Ending_step_flg == 0) {
			Ending_result_rec_tbl[Ending_rank_no + END_REC_RANK].flg = 5;
			if ((G.Status_flg & STAT_SCENARIO_B) != 0) {
				if ((G.Status_flg & STAT_PLAYER) == 0) {
					Ending_result_rec_tbl[END_REC_LEON_B].flg = 5;
				}
				else {
					Ending_result_rec_tbl[END_REC_CLAIRE_B].flg = 5;
				}
			}
			else {
				if ((G.Status_flg & STAT_PLAYER) == 0) {
					Ending_result_rec_tbl[END_REC_LEON_A].flg = 5;
				}
				else {
					Ending_result_rec_tbl[END_REC_CLAIRE_A].flg = 5;
				}
			}
			Ending_result_flg_set(0x3e000, 5, 0);
			Ending_scroll_spd = 0x00101010;
			Ending_step_flg = 1;
			Ending_timeline_cnt = 0;
		}
		if (Ending_timeline_cnt < 1201) {
			break;
		}
		if ((Ending_state_flg & 0x80) != 0) {
			Ending_state_flg |= 0x3000;
			Ending_result_slot_mask = 0x01bc0000;
			Ending_disp_mode = 6;
		}
		else {
			Snd_bgm_fade_ON(0xb4, 0x18);
			Ending_disp_mode = 7;
			Ending_state_flg |= 0x2008;
		}
		Ending_fade_in();
		Ending_timeline_cnt = 0;
		break;

	case 6:
		if (Ending_timeline_cnt >= 301 && (G.Padd1 & 0x800) != 0) {
			Ending_timeline_cnt = 901;
		}
		if (Ending_timeline_cnt >= 901) {
			Snd_bgm_fade_ON(0xb4, 0x18);
			Ending_state_flg |= 0x2008;
			Ending_fade_in();
			Ending_disp_mode = 7;
			Ending_timeline_cnt = 0;
		}
		break;

	case 7:
		if (pEdt_adr[0].id_check != 0) {
			break;
		}
		if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			G.Status_flg |= STAT_SCENARIO_B;
		}
		else {
			G.Status_flg &= ~STAT_SCENARIO_B;
		}
		if ((G.Status_flg & STAT_PLAYER) == 0) {
			G.Status_flg |= STAT_PLAYER;
		}
		else {
			G.Status_flg &= ~STAT_PLAYER;
		}
		Ending_next_set();
		G.System_flg &= ~SYS_SAVE_GAME;
		Task_lexecute(1, 1);
		Task_sleep(1);
		if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			G.Status_flg |= STAT_SCENARIO_B;
		}
		else {
			G.Status_flg &= ~STAT_SCENARIO_B;
		}
		if ((G.Status_flg & STAT_PLAYER) == 0) {
			G.Status_flg |= STAT_PLAYER;
		}
		else {
			G.Status_flg &= ~STAT_PLAYER;
		}
		if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			Ending_exit();
			break;
		}
		*(USHORT *)bgm_ctr = 0xffff;
		pBgm = bgm_ctr;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		*(USHORT *)bgm_ctr = 0x33ff;
		pBgm = bgm_ctr;
		Seq_ctr[1].Ctrl = 0;
		Seq_ctr[1].Vol.left = 100;
		Seq_ctr[1].Vol.right = 100;
		Seq_ctr[2].Ctrl = 0;
		Seq_ctr[2].Vol.left = 100;
		Seq_ctr[2].Vol.right = 100;
		Snd_bgm_main();
		Snd_bgm_sub();
		Ending_disp_mode = 8;
		Ending_timeline_cnt = 0;
		if ((Ending_state_flg & 0x10000) == 0) {
			break;
		}
		Ending_tim_load(8);
		Ending_fade_set(0, 0, 0, NULL);
		Ending_state_flg |= 0x800;
		Ending_fade_out();
		Snd_bgm_ctr(0x11000000);
		Ending_disp_mode = 0x14;
		Ending_step_flg = 0;
		break;

	case 8:
		if ((Ending_state_flg & 0x100) != 0) {
			Ending_tim_load(2);
			Ending_fade_set(0, 0, 0, NULL);
			Ending_state_flg |= 0x800;
			Ending_fade_out();
			Snd_bgm_ctr(0x11000000);
			Ending_disp_mode = 9;
		}
		else {
			Ending_disp_mode = 0x0b;
		}
		Ending_timeline_cnt = 0;
		Ending_step_flg = 1;
		break;

	case 9:
		if ((G.Padd1 & 0x800) != 0 && Ending_timeline_cnt >= 181 && Ending_step_flg == 1) {
			Ending_timeline_cnt = 341;
		}
		if (Ending_timeline_cnt == 40 && Ending_step_flg == 0) {
			Ending_result_flg_set(0x2000000, 1, 0);
			Ending_step_flg = 1;
		}
		if (Ending_timeline_cnt < 341) {
			break;
		}
		Ending_result_flg_set(0x2000000, 0, 0);
		Ending_state_flg |= 0x2008;
		Ending_fade_in();
		Ending_disp_mode = 0x0a;
		Ending_timeline_cnt = 0;
		break;

	case 10:
		if (Ending_timeline_cnt != 60) {
			break;
		}
		memcpy16(&G.SaveData.Game_play_time, Ending_save_copy, 0x598);
		G.System_flg |= SYS_TIMER;
		Ending_next_set();
		G.System_flg &= ~SYS_SAVE_GAME;
		Task_lexecute(1, 1);
		Task_sleep(1);
		Ending_disp_mode = 0x0b;
		Ending_timeline_cnt = 0;
		G.System_flg &= ~SYS_TIMER;
		break;

	case 11:
		if ((Ending_state_flg & 0x200) != 0) {
			Ending_tim_load(4);
			Ending_fade_set(0, 0, 0, NULL);
			Ending_state_flg |= 0x800;
			Ending_fade_out();
			Snd_bgm_ctr(0x11000000);
			Ending_disp_mode = 0x0c;
		}
		else {
			Ending_disp_mode = 0x0e;
		}
		Ending_timeline_cnt = 0;
		Ending_step_flg = 1;
		break;

	case 12:
		if ((G.Padd1 & 0x800) != 0 && Ending_timeline_cnt >= 181 && Ending_step_flg == 1) {
			Ending_timeline_cnt = 341;
		}
		if (Ending_timeline_cnt == 40 && Ending_step_flg == 0) {
			Ending_result_flg_set(0x4000000, 1, 0);
			Ending_step_flg = 1;
		}
		if (Ending_timeline_cnt < 341) {
			break;
		}
		Ending_result_flg_set(0x4000000, 0, 0);
		Ending_state_flg |= 0x2008;
		Ending_fade_in();
		Ending_disp_mode = 0x0d;
		Ending_timeline_cnt = 0;
		break;

	case 13:
		if (Ending_timeline_cnt != 60) {
			break;
		}
		memcpy16(&G.SaveData.Game_play_time, Ending_save_copy, 0x598);
		G.System_flg = G.System_flg | (SYS_TIMER | SYS_TOFU_SURVIVOR);
		Ending_next_set();
		G.System_flg &= ~SYS_SAVE_GAME;
		Task_lexecute(1, 1);
		Task_sleep(1);
		Ending_disp_mode = 0x0e;
		Ending_timeline_cnt = 0;
		G.System_flg &= ~(SYS_TIMER | SYS_TOFU_SURVIVOR);
		break;

	case 14:
		if ((Ending_state_flg & 0x400) == 0) {
			Ending_exit();
			break;
		}
		Ending_tim_load(3);
		Ending_fade_set(0, 0, 0, NULL);
		Ending_state_flg |= 0x800;
		Ending_fade_out();
		Snd_bgm_ctr(0x21000000);
		Ending_disp_mode = 0x0f;
		Ending_timeline_cnt = 0;
		Ending_step_flg = 0;
		break;

	case 15:
		if (Ending_timeline_cnt == 40 && Ending_step_flg == 0) {
			Ending_result_flg_set(0x8000000, 5, 0);
		}
		if (Ending_timeline_cnt == 60) {
			Ending_result_flg_set(0x2000000, 1, 0);
		}
		if (Ending_timeline_cnt == 240) {
			Ending_result_flg_set(0x28000000, 0, 0);
			Ending_result_flg_set(0x10000000, 5, 0);
		}
		if (Ending_timeline_cnt == 260) {
			Ending_result_flg_set(0x40000000, 1, 0);
		}
		if (Ending_timeline_cnt < 441) {
			break;
		}
		Ending_result_flg_set(0x40000000, 0, 0);
		Ending_state_flg |= 0x2008;
		Ending_fade_in();
		Ending_disp_mode = 0x10;
		Ending_timeline_cnt = 0;
		break;

	case 16:
		if (Ending_timeline_cnt == 30) {
			Ending_result_flg_set(0x50000000, 0, 0);
			Ending_exit();
		}
		break;

	case 17:
		if (pEdt_adr[0].id_check != 0) {
			break;
		}
		bgm_ctr[0].Main = 0x30;
		bgm_ctr[0].Sub = 0xff;
		pBgm = bgm_ctr;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Snd_bgm_ctr(0x01000000);
		Ending_fade_out();
		Ending_scroll_spd = 0x00101010;
		Ending_disp_mode = 0x12;
		Ending_timeline_cnt = 0;
		break;

	case 18:
		if (Ending_timeline_cnt == 180) {
			Ending_result_flg_set(1, 5, 0);
		}
		if (Ending_timeline_cnt == 200) {
			Ending_result_flg_set(4, 1, 0);
		}
		if (Ending_timeline_cnt == 500) {
			Ending_result_flg_set(4, 0, 0);
			Ending_state_flg |= 0x2008;
			Ending_fade_in();
		}
		if (Ending_timeline_cnt == 640) {
			Ending_result_flg_set(2, 5, 0);
		}
		if (Ending_timeline_cnt == 660) {
			Ending_result_flg_set(8, 1, 0);
		}
		if (Ending_timeline_cnt == 1060) {
			Ending_result_flg_set(8, 0, 0);
			Ending_state_flg |= 0x2008;
			Ending_fade_in();
			Snd_bgm_fade_ON(0xb4, 0x18);
		}
		if (Ending_timeline_cnt != 1200) {
			break;
		}
		Ending_tim_load((G.System_flg & SYS_TOFU_SURVIVOR) != 0 ? 6 : 5);
		Ending_fade_set(0, 0, 0, NULL);
		Ending_state_flg |= 0x800;
		Ending_fade_out();
		bgm_ctr[0].Main = 0x2a;
		bgm_ctr[0].Sub = 0xff;
		pBgm = bgm_ctr;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Snd_bgm_ctr(0x01000000);
		Ending_disp_mode = 0x13;
		Ending_timeline_cnt = 0;
		Ending_step_flg = 0;
		break;

	case 19:
		if ((G.Padd1 & 0x800) != 0) {
			if (Ending_timeline_cnt < 300) {
				if (Ending_step_flg == 0) {
					Ending_timeline_cnt = 480;
				}
			}
			else {
				if (Ending_step_flg == 0) {
					Ending_timeline_cnt = 480;
				}
				else if (Ending_step_flg == 1) {
					Ending_timeline_cnt = 1201;
				}
			}
		}
		if (Ending_timeline_cnt == 480 && Ending_step_flg == 0) {
			Ending_result_rec_tbl[Ending_rank_no + END_REC_RANK].flg = 5;
			Ending_result_rec_tbl[END_REC_LEON_A].flg = 0;
			Ending_result_rec_tbl[END_REC_CLAIRE_A].flg = 0;
			Ending_result_rec_tbl[END_REC_LEON_B].flg = 0;
			Ending_result_rec_tbl[END_REC_CLAIRE_B].flg = 0;
			Ending_result_flg_set(0x1e000, 5, 0);
			if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) {
				Ending_result_rec_tbl[END_REC_HUNK].flg = 0;
				Ending_result_rec_tbl[END_REC_TOFU].flg = 5;
			}
			else {
				Ending_result_rec_tbl[END_REC_HUNK].flg = 5;
				Ending_result_rec_tbl[END_REC_TOFU].flg = 0;
			}
			Ending_scroll_spd = 0x00101010;
			Ending_step_flg = 1;
			Ending_timeline_cnt = 0;
		}
		if (Ending_timeline_cnt == 1200) {
			Snd_bgm_fade_ON(0xb4, 0x18);
			Ending_state_flg |= 0x2008;
			Ending_fade_in();
		}
		if (Ending_timeline_cnt >= 1301) {
			Ending_exit();
		}
		break;

	case 20:
		if ((G.Padd1 & 0x800) != 0 && Ending_timeline_cnt >= 181 && Ending_step_flg == 1) {
			Ending_timeline_cnt = 341;
		}
		if (Ending_timeline_cnt == 40 && Ending_step_flg == 0) {
			Ending_result_flg_set(0x2000000, 1, 0);
			Ending_step_flg = 1;
		}
		if (Ending_timeline_cnt < 341) {
			break;
		}
		Ending_result_flg_set(0x2000000, 0, 0);
		Ending_state_flg |= 0x2008;
		Ending_fade_in();
		Ending_disp_mode = 0x15;
		Ending_timeline_cnt = 0;
		break;

	case 21:
		if (Ending_timeline_cnt != 60) {
			break;
		}
		memcpy16(&G.SaveData.Game_play_time, Ending_save_copy, 0x598);
		G.Status_flg |= STAT_EX_SAVE;
		G.System_flg |= SYS_PSP_TRANS;
		Ending_next_set();
		G.System_flg &= ~SYS_SAVE_GAME;
		Task_lexecute(1, 1);
		Task_sleep(1);
		Ending_disp_mode = 8;
		G.Status_flg &= ~STAT_EX_SAVE;
		G.System_flg &= ~SYS_EX_BATTLE;
		Ending_timeline_cnt = 0;
		break;

	default:
		break;
	}
	Ending_timeline_cnt++;
	return;
}

/* Fade dispatch table, retail ENDING::8011de34 (ENDING.BIN offset 0x3e34).
 * Indexed by Ending_fade_dir (DS 0x801213fc): the overlay entry at
 * ENDING::8011a134 loads table[Ending_fade_dir] and calls it.
 * Entry 1 is ENDING::8011a8e4 (Ending_fade_unk, defined above). */
VOID *Ending_fade_tbl[4] = {
	(VOID *)Ending_fade_out,	/* 0: ENDING::8011a6b0 */
	(VOID *)Ending_fade_unk,	/* 1: ENDING::8011a8e4 */
	(VOID *)Ending_fade_in,	/* 2: ENDING::8011a7b4 */
	(VOID *)Ending_exit,	/* 3: ENDING::8011a630 */
};

/* Ending - ENDING::8011a0fc
 * Overlay entry. Runs Ending_init, then unless SYS_EX_BATTLE is set it loops once per frame: calls the fade
 * handler selected by Ending_fade_dir (Ending_fade_tbl), draws the score and sleeps one frame. With SYS_EX_BATTLE
 * set it runs Ending_main once instead. */
/* DS 0x8011a0fc (Ending) */
VOID Ending(VOID)
{
	Ending_init();
	if ((G.System_flg & SYS_EX_BATTLE) == 0) {
		do {
			(((void (*)(void))Ending_fade_tbl[Ending_fade_dir])());
			Ending_draw_score();
			Task_sleep(1);
		} while (1);
	}
	Ending_main();
}

/* Ending_draw_score - ENDING::8011bb94
 * Draw ranking/score tile glyphs: iterates result entries and emits
 * POLY_FT4 quads for each character. */
/* DS 0x8011bb94 (Ending_draw_score) */
static void Ending_draw_score(void)
{
	UCHAR ot_idx;
	UCHAR ch_idx;
	UCHAR glyph_h;
	char glyph_u;
	char glyph_uw;
	char glyph_v;
	char glyph_vh;
	UCHAR tex_u2;
	USHORT y_pos;
	USHORT attr_val;
	POLY_FT4 *p;
	UINT row_no;
	int scroll_pos;
	int prim_code;
	int rem_cnt;
	USHORT x_pos;
	ENDING_RESULT_REC *pRec;
	ENDING_GLYPH *pGlyph;
	UCHAR *pIdx;

	pRec = Ending_result_rec_tbl;
	Ending_pPoly = Ending_prim[G.Cbuf_id];
	rem_cnt = 0x21;
	do {
		rem_cnt = rem_cnt + -1;
		if ((pRec->attr & 0x20) != 0) {
			pRec->flg = 0;
		}
		if ((pRec->flg & 1) != 0) {
			pIdx = Ending_glyph_idx_tbl;
			row_no = 0;
			if (pRec->row != 0) {
				do {
					if (*pIdx == 0xff) {
						row_no++;
					}
					pIdx++;
				} while (pRec->row != row_no);
			}
			ot_idx = pRec->ot;
			prim_code = 0x2c000000;
			if ((pRec->flg & 8) != 0) {
				prim_code = 0x2e000000;
				scroll_pos = pRec->colour - Ending_scroll_spd;
				pRec->colour = scroll_pos;
				if (0x80807f < scroll_pos) {
					pRec->flg = 0;
					pRec->colour = 0;
				}
			}
			if ((pRec->flg & 4) != 0) {
				prim_code = 0x2e000000;
				scroll_pos = pRec->colour + Ending_scroll_spd;
				pRec->colour = scroll_pos;
				if (0xf0f0ef < scroll_pos) {
					prim_code = 0x2c000000;
					pRec->flg = pRec->flg & 0xfb;
					pRec->colour = 0x808080;
				}
			}
			x_pos = pRec->x;
			scroll_pos = pRec->colour;
			y_pos = pRec->y;
			ch_idx = *pIdx;
			p = Ending_pPoly;
			while (Ending_pPoly = p, ch_idx != 0xff) {
				pGlyph = &Ending_glyph_Tbl[ch_idx];
				ch_idx = pGlyph->w;
				glyph_h = pGlyph->h;
				glyph_u = pGlyph->u;
				glyph_uw = pGlyph->uw;
				glyph_v = pGlyph->v;
				glyph_vh = pGlyph->vh;
				SetPolyFT4(p);
				p->x0 = x_pos;
				*(int *)&p->r0 = prim_code + scroll_pos;
				p->y0 = y_pos;
				p->u0 = pGlyph->u;
				p->v0 = pGlyph->v;
				attr_val = pGlyph->clut;
				p->x1 = x_pos + (USHORT)ch_idx;
				p->y1 = y_pos;
				p->u1 = glyph_u + glyph_uw;
				p->clut = attr_val + 0x400;
				p->v1 = pGlyph->v;
				attr_val = pGlyph->tpage;
				p->x2 = x_pos;
				p->y2 = y_pos + (USHORT)glyph_h;
				p->tpage = attr_val + 0x80;
				tex_u2 = pGlyph->u;
				p->v2 = glyph_v + glyph_vh;
				p->x3 = x_pos + (USHORT)ch_idx;
				p->y3 = y_pos + (USHORT)glyph_h;
				p->u3 = glyph_u + glyph_uw;
				p->v3 = glyph_v + glyph_vh;
				p->u2 = tex_u2;
				AddPrim(G.pOt1 + ot_idx, p);
				pIdx++;
				p = Ending_pPoly + 1;
				x_pos = x_pos + (USHORT)pGlyph->w;
				ch_idx = *pIdx;
			}
		}
		pRec++;
	} while (rem_cnt != 0);
	if ((Ending_result_rec_tbl[END_REC_CONGRATS].flg & 1) != 0) {
		Ending_draw_rank();
	}
	return;
}

/* Ending_draw_rank - ENDING::8011becc
 * Draw score rank medal/icon tiles: emits 10 POLY_FT4 quads from the
 * timer-digit display table. */
/* DS 0x8011becc (Ending_draw_rank) */
static void Ending_draw_rank(void)
{
	BOOL loop_flg;
	POLY_FT4 *p;
	UINT col_code;
	int ofs;
	USHORT *pCoord;
	ENDING_GLYPH *pGlyph;
	int dig_idx;
	ULONG rank_x[6];   /* ENDING::8011a08c table, retail sp+0x10; rank_x[5] (sp+0x24) unused, never read */
	ULONG rank_y[6];   /* ENDING::8011a0a0 table, retail sp+0x28; rank_y[5] (sp+0x3c) unused, never read */

	rank_x[0] = Ending_rank_x0;
	rank_x[1] = Ending_rank_x1;
	rank_x[2] = Ending_rank_x2;
	rank_x[3] = Ending_rank_x3;
	rank_x[4] = Ending_rank_x4;
	rank_y[0] = Ending_rank_y0;
	rank_y[1] = Ending_rank_y1;
	rank_y[2] = Ending_rank_y2;
	rank_y[3] = Ending_rank_y3;
	rank_y[4] = Ending_rank_y4;
	col_code = 0x2e000000;
	if ((Ending_result_rec_tbl[END_REC_CONGRATS].flg & 0xc) == 0) {
		col_code = 0x2c000000;
	}
	col_code = Ending_result_rec_tbl[END_REC_CONGRATS].colour | col_code;
	dig_idx = 9;
	do {
		p = Ending_pPoly;
		ofs = dig_idx * 2;
		pGlyph = &Ending_glyph_Tbl[(UCHAR)(&Ending_time_dig_d10)[dig_idx]];
		pCoord = (USHORT *)((int)rank_x + ofs);
		SetPolyFT4(Ending_pPoly);
		p->r0 = (char)col_code;
		p->g0 = (char)(col_code >> 8);
		p->b0 = (char)(col_code >> 0x10);
		p->code = (char)(col_code >> 0x18);
		p->x0 = *pCoord;
		p->y0 = *(USHORT *)((int)rank_y + ofs);
		p->u0 = pGlyph->u;
		p->v0 = pGlyph->v;
		p->clut = pGlyph->clut + 0x400;
		p->x1 = *pCoord + (USHORT)pGlyph->w;
		p->y1 = *(USHORT *)((int)rank_y + ofs);
		p->u1 = pGlyph->u + pGlyph->uw;
		p->v1 = pGlyph->v;
		p->tpage = pGlyph->tpage + 0x80;
		p->x2 = *pCoord;
		p->y2 = *(USHORT *)((int)rank_y + ofs) + (USHORT)pGlyph->h;
		p->u2 = pGlyph->u;
		p->v2 = pGlyph->v + pGlyph->vh;
		p->x3 = *pCoord + (USHORT)pGlyph->w;
		p->y3 = *(USHORT *)((int)rank_y + ofs) + (USHORT)pGlyph->h;
		p->u3 = pGlyph->u + pGlyph->uw;
		p->v3 = pGlyph->v + pGlyph->vh;
		AddPrim(G.pOt1 + 10, p);
		Ending_pPoly++;
		loop_flg = dig_idx != 0;
		dig_idx = dig_idx + -1;
	} while (loop_flg);
	return;
}

/* Ending_result_flg_set - ENDING::8011c140
 * Set/clear display flags for result entry slots (31 slots).
 * Slot_mask: bitmask of slots, Val: value to set, Clr_sw: clear-on-zero flag. */
/* DS 0x8011c140 (Ending_result_flg_set) */
static void Ending_result_flg_set(UINT Slot_mask, UCHAR Val, char Clr_sw)
{
	ENDING_RESULT_REC *pRec;
	UINT slot_no;

	pRec = Ending_result_rec_tbl;
	slot_no = 0;
	do {
		if ((Slot_mask >> (slot_no & 0x1f) & 1) == 0) {
			if (Clr_sw != 0) {
				pRec->flg = 0;
			}
		}
		else {
			pRec->flg = Val;
		}
		if ((pRec->attr & 0x20) != 0) {
			pRec->flg = 0;
		}
		slot_no++;
		pRec++;
	} while (slot_no < 0x1f);
	return;
}

/* Ending_score_set - ENDING::8011c1a4
 * Calculate and display time/rank score: converts play time to H:MM:SS
 * digits, evaluates bonus items and final rank. */
/* DS 0x8011c1a4 (Ending_score_set) */
static void Ending_score_set(void)
{
	USHORT thr_val;
	BOOL low_rank_flg;
	char tens;
	UINT lp_idx;
	int ret_val;
	ULONG flg_res;
	USHORT rank_idx;
	UINT tmp_u;
	UINT tmp_v;
	USHORT *pThr;
	UINT play_time;
	USHORT rank_thr[5];   /* threshold table, ENDING::8011a0b4: word [0], word [2], half [4] (10 bytes) */

	*(ULONG *)&rank_thr[0] = Ending_rank_thr_init0;
	*(ULONG *)&rank_thr[2] = Ending_rank_thr_init1;
	rank_thr[4] = Ending_rank_thr_init2;
	if ((G.Status_flg & STAT_SPECIAL_RUN) == 0) {
		ret_val = VSync(-1);
		play_time = (G.SaveData.Game_play_time + ret_val) - G.Game_time_offset;
	}
	else {
		play_time = G.SaveData.Count_down_timer * 0x3c;
	}
	if (play_time < 0x28fe205) {
		Ending_time_hour = (UCHAR)(play_time / 0x34bc0);
		tmp_u = play_time / 0x34bc0 & 0xff;
		if (tmp_u == 0) {
			Ending_time_dig_d1 = 23;
			Ending_time_dig_d10 = Ending_time_dig_d1;
		}
		else if (tmp_u < 99) {
			Ending_time_dig_d1 = (char)(tmp_u % 10) + 23;
			Ending_time_dig_d10 = (char)(tmp_u / 10) + 23;
		}
		else {
			Ending_time_dig_d1 = ' ';
			Ending_time_dig_d10 = Ending_time_dig_d1;
		}
		tmp_v = (play_time % 0x34bc0) / 0xe10;
		tmp_u = (play_time % 0xe10) / 0x3c;
		Ending_time_min = (char)tmp_v;
		Ending_time_sec = (char)tmp_u;
		tens = (char)(tmp_v / 10);
		Ending_time_dig_h10 = tens + 23;
		Ending_time_dig_h1 = Ending_time_min + tens * -10 + 23;
		tens = (char)(tmp_u / 10);
		Ending_time_dig_m10 = tens + 23;
		Ending_time_dig_m1 = Ending_time_sec + tens * -10 + 23;
	}
	else {
		Ending_time_dig_d10 = ' ';
		Ending_time_dig_d1 = ' ';
		Ending_time_dig_h10 = 28;
		Ending_time_dig_h1 = ' ';
		Ending_time_dig_m10 = 28;
		Ending_time_dig_m1 = ' ';
	}
	Ending_label_chr_b = 0x24;
	Ending_label_chr_a = 0x24;
	tmp_u = Ending_save_bonus_in & 0xff;
	Ending_bonus_val = (char)Ending_save_bonus_in;
	Ending_bonus_dig_1 = ' ';
	if (tmp_u < 99) {
		Ending_bonus_dig_10 = (char)(tmp_u / 10) + 23;
		Ending_bonus_dig_1 = (char)(tmp_u % 10) + 23;
	}
	else {
		Ending_bonus_dig_10 = ' ';
	}
	tmp_u = 0x3840;
	if ((G.Status_flg & STAT_SPECIAL_RUN) != 0) {
		tmp_v = 0;
		if (0x3840 < play_time) {
			lp_idx = 1;
			do {
				tmp_v = lp_idx;
				tmp_u += 0xe10;
				if (3 < tmp_v) {
					tmp_v = 4;
					break;
				}
				lp_idx = tmp_v + 1;
			} while (tmp_u < play_time);
		}
		Ending_label_chr_a = 0x24;
		Ending_label_chr_b = 0x24;
		Ending_bonus_dig_10 = 0x26;
		Ending_bonus_dig_1 = 0x26;
		Ending_rank_no = (short)tmp_v;
		return;
	}
	tmp_v = 540000;
	tmp_u = 0;
	if (540000 < play_time) {
		lp_idx = 1;
		do {
			tmp_u = lp_idx;
			tmp_v += 0x34bc0;
			if (9 < tmp_u) {
				tmp_u = 10;
				break;
			}
			lp_idx = tmp_u + 1;
		} while (tmp_v < play_time);
	}
	Ending_score_time = (char)tmp_u * -10 + 100;
	if (Ending_save_bonus_in < 0x14) {
		Ending_score_life = Ending_bonus_val * -5 + 100;
	}
	else {
		Ending_score_life = 0;
	}
	if (Ending_save_rank_in < 6) {
		Ending_score_item = (char)Ending_save_rank_in * -0xf + 100;
	}
	else {
		Ending_score_item = 0;
	}
	Ending_rank_no =
		(USHORT)Ending_score_item + (USHORT)Ending_score_time + (USHORT)Ending_score_life;
	low_rank_flg = Ending_rank_no < 0x32;
	if (((G.Status_flg & STAT_INF_AMMO) != 0) && (Ending_rank_no = Ending_rank_no - 0x32, low_rank_flg)) {
		Ending_rank_no = 0;
	}
	rank_idx = 0;
	pThr = rank_thr;
	thr_val = rank_thr[0];
	while (Ending_rank_no < thr_val) {
		pThr++;
		rank_idx++;
		thr_val = *pThr;
	}
	if ((G.Status_flg & STAT_EASY_MODE) != 0) {
		Ending_label_chr_a = 0x24;
		Ending_label_chr_b = 0x24;
		Ending_rank_no = rank_idx;
		return;
	}
	Ending_rank_no = rank_idx;
	ret_val = Ending_item_ck(17);
	if (ret_val != 0) {
		Ending_result_rec_tbl[END_REC_ITEM_B].colour = 0;
		Ending_state_flg |= 0x20;
	}
	else if ((Ending_score_time == 100) && (rank_idx < 2)) {
		Ending_result_rec_tbl[END_REC_ITEM_B].colour = 0x808080;
		Ending_state_flg |= 0xa0;
	}
	if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
		ret_val = Ending_item_ck(15);
		if (ret_val != 0) {
			Ending_result_rec_tbl[END_REC_ITEM_A].colour = 0;
			Ending_state_flg |= 0x10;
		}
		ret_val = Ending_item_ck(18);
		if (ret_val != 0) {
			Ending_result_rec_tbl[END_REC_ITEM_C].colour = 0;
			Ending_state_flg |= 0x40;
		}
		flg_res = Flg_ck(&Ending_save_copy[0x218 / 4], 0x9c);
		if (flg_res != 0) {
			return;
		}
		Flg_on(&Ending_save_copy[0x388 / 4], 0x12);
		return;
	}
	ret_val = Ending_item_ck(15);
	if (ret_val != 0) {
		Ending_result_rec_tbl[END_REC_ITEM_A].colour = 0;
		Ending_state_flg |= 0x10;
	}
	else if ((Ending_score_item == 100) && (Ending_rank_no < 2)) {
		Ending_result_rec_tbl[END_REC_ITEM_A].colour = 0x808080;
		Ending_state_flg |= 0x90;
	}
	ret_val = Ending_item_ck(18);
	if (ret_val != 0) {
		Ending_result_rec_tbl[END_REC_ITEM_C].colour = 0;
		Ending_state_flg |= 0x40;
	}
	else if ((Ending_score_life == 100) && (Ending_rank_no < 2)) {
		Ending_result_rec_tbl[END_REC_ITEM_C].colour = 0x808080;
		Ending_state_flg |= 0xc0;
	}
	flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x13);
	if (flg_res == 0) {
		if (Ending_rank_no == 0) {
			Flg_on(&Ending_save_copy[0x388 / 4], 0x13);
			Ending_state_flg |= 0x100;
		}
	}
	else {
		flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x14);
		if ((flg_res == 0) && (1 < Ending_save_zapping_cnt)) {
			Flg_on(&Ending_save_copy[0x388 / 4], 0x14);
			Ending_state_flg |= 0x200;
		}
	}
	flg_res = Flg_ck(&Ending_save_copy[0x218 / 4], 0x9c);
	if ((flg_res == 0) && (flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x12), flg_res != 0)) {
		Ending_state_flg |= 0x400;
	}
	flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x15);
	if (flg_res == 0) {
		Flg_on(&Ending_save_copy[0x388 / 4], 0x15);
		Ending_state_flg |= 0x10000;
	}
	return;
}

/* Ending_item_ck - ENDING::8011c9f4
 * Search player/box inventory for item by type Item_id.
 * Returns slot index (high-shifted for box slots) or 0 if not found. */
/* DS 0x8011c9f4 (Ending_item_ck) */
static int Ending_item_ck(char Item_id)
{
	char slot_id;
	int nxt_idx;
	int slot_idx;
	int ofs;

	ofs = 0x28;
	slot_idx = 0xb;
	do {
		slot_id = *(char *)(ofs + -0x7feda9b0);
		nxt_idx = slot_idx + -1;
		if (slot_id == Item_id) {
			if ((slot_id != 17) && (slot_id != 15)) {
				return slot_idx;
			}
			if (*(char *)(ofs + -0x7feda9af) == 0xff) {
				return slot_idx;
			}
		}
		ofs = ofs + -4;
		slot_idx = nxt_idx;
		if (nxt_idx == 0) {
			ofs = 0xfc;
			slot_idx = 0x40;
			while (1) {
				slot_id = *(char *)(ofs + -0x7feda984);
				nxt_idx = slot_idx + -1;
				if ((slot_id == Item_id) && (((slot_id != 17 && (slot_id != 15)) || (*(char *)(ofs + -0x7feda983) == 0xff))))
					break;
				ofs = ofs + -4;
				slot_idx = nxt_idx;
				if (nxt_idx == 0) {
					return 0;
				}
			}
			return slot_idx << 0x10;
		}
	} while (1);
}

/* Ending_next_set - ENDING::8011cad8
 * Post-ending save data setup: sets player position, stage/room, loads
 * init table, prepares new-game save record. */
/* DS 0x8011cad8 (Ending_next_set) */
static void Ending_next_set(void)
{
	BOOL flg30_off;
	BOOL flg4000_off;
	ULONG flg_res;
	ULONG *ex3_a;   /* SaveData.ExData3 word walk A: starts at word 16, steps -4 words */
	ULONG *ex3_b;   /* SaveData.ExData3 word walk B: starts at word 22, steps -1 word */
	ULONG *exd_a;   /* G.ExData word walk A: starts at byte 36, steps -16 bytes */
	ULONG *exd_b;   /* G.ExData word walk B: starts at byte 60, steps -4 bytes */
	int walk_a;
	int cnt;
	UCHAR save_area[6];   /* ENDING::8011a0c0 init bytes: word [0..3], init1 [4], init2 [5] (retail sp+0x10..0x15) */

	*(ULONG *)&save_area[0] = Ending_save_area_init0;
	save_area[4] = Ending_save_area_init1;
	save_area[5] = Ending_save_area_init2;
	flg4000_off = (G.Status_flg & STAT_EX_SAVE) == 0;
	if (flg4000_off && (G.System_flg & SYS_TIMER) == 0) {
		flg30_off = (G.Status_flg & STAT_SCENARIO_B) == 0;
		if ((G.Status_flg & STAT_PLAYER) != 0) {
			G.Dummy8[1] = flg30_off ? 2 : 3;
		}
		else {
			G.Dummy8[1] = !flg30_off;
		}
	}
	else {
		G.Dummy8[1] = flg4000_off ? 4 : 5;
	}
	Ending_inittbl_load();
	G.SaveData.Save_area = save_area[(UINT)G.Dummy8[1]];
	cnt = 3;
	if (flg4000_off) {
		if ((G.System_flg & SYS_TIMER) == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				G.Player.Pos_x = 0x4972;
				G.Player.Pos_z = -0xc5c;
				G.Player.Cdir_y = 0x800;
				G.SaveData.Room_no = 0;
			}
			else {
				G.SaveData.Room_no = 4;
				G.Player.Pos_x = -0x4600;
				G.Player.Pos_z = -0x54da;
				G.Player.Cdir_y = 400;
			}
			G.SaveData.Stage_no = 0;
			G.Next_cut_no = 0;
			G.SaveData.Cut_no = 0;
			G.Player.Pos_y = 0;
			G.SaveData.Pl_id = (G.Status_flg & STAT_PLAYER) != 0;
			G.SaveData.Pl_life = 200;
			G.Player.Life = 200;
			G.SaveData.Dummy[0] = 200;
			G.SaveData.Dummy1[1] = 0;
		}
		else {
			G.SaveData.Stage_no = 2;
			G.SaveData.Room_no = 4;
			G.Next_cut_no = 3;
			G.SaveData.Cut_no = 3;
			G.Player.Pos_x = -0x1cff;
			G.Player.Pos_y = 0;
			G.Player.Pos_z = -0x5fac;
			G.Player.Cdir_y = 0x800;
			if ((G.System_flg & SYS_TOFU_SURVIVOR) == 0) {
				G.SaveData.Pl_id = 12;
				G.Player.Life = 200;
				G.SaveData.Pl_life = G.Player.Life;
				G.SaveData.Dummy[0] = G.Player.Life;
			}
			else {
				G.SaveData.Pl_id = 13;
				G.Player.Life = 400;
				G.SaveData.Pl_life = G.Player.Life;
				G.SaveData.Dummy[0] = G.Player.Life;
			}
		}
	}
	else {
		G.SaveData.Sce_timer = 1;
		G.ExData[2] = 1;
		G.SaveData.Dummy[1] = 0;
		G.ExData[1] = 0;
		ex3_a = &G.SaveData.ExData3[16];
		ex3_b = &G.SaveData.ExData3[22];
		exd_a = (ULONG *)&G.ExData[36];
		exd_b = (ULONG *)&G.ExData[60];
		do {
			cnt = cnt + -1;
			*(USHORT *)ex3_a = 0;
			((USHORT *)ex3_a)[1] = 0;
			ex3_a[1] = 0;
			ex3_a[2] = 0;
			ex3_a[3] = 0;
			*ex3_b = 0;
			*(USHORT *)exd_a = 0;
			((USHORT *)exd_a)[1] = 0;
			exd_a[1] = 0;
			exd_a[2] = 0;
			exd_a[3] = 0;
			*exd_b = 0;
			ex3_a = ex3_a + -4;
			ex3_b = ex3_b + -1;
			exd_a = exd_a + -4;
			exd_b = exd_b + -1;
		} while (cnt != 0);
		G.SaveData.Pl_id = 16;
		G.ExData[3] = 16;
	}
	G.Player.Id = G.SaveData.Pl_id;
	Status_init();
	Status_init_m();
	G.SaveData.Game_play_time = 0;
	G.SaveData.Max_item_num = 8;
	G.SaveData.Stereo = G.Sound_mode;
	G.SaveData.Vol_Bgm = Volume_Bgm;
	G.SaveData.Pl_poison_down = 0;
	G.Pl_poison_down = 0;
	G.SaveData.Pl_poison_timer = 0;
	G.Pl_poison_timer = 0;
	G.SaveData.Vol_Se = Volume_Se;
	G.SaveData.Key_idx = Main.Key_idx;
	G.SaveData.Dummy16 = Ending_save_bonus_in;
	G.Status_flg &= (STAT_PLAYER | STAT_SCENARIO_B | STAT_EASY_MODE);
	if ((G.System_flg & SYS_TIMER) != 0) {
		Flg_on(&G.Status_flg, 6);
	}
	if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) {
		Flg_on(&G.Status_flg, 6);
	}
	if (!flg4000_off) {
		Flg_on(&G.Status_flg, 0x11);
	}
	G.SaveData.Status_bak = G.Status_flg;
	G.SaveData.Cure_cnt = 0;
	G.SaveData.Save_cnt = 0xffff;
	if (((G.System_flg & SYS_TIMER) == 0) && ((G.Status_flg & STAT_EX_SAVE) == 0)) {
		G.SaveData.Zonbi_cnt = Ending_zombie_cnt;
		if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			G.SaveData.Front_play_time = 0;
			G.SaveData.Dummy1[0] = Ending_save_zapping_cnt + 1;
			flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x14);
			if (flg_res != 0) {
				Flg_on(G.SaveData.Zapping_flg, 0x14);
			}
			flg_res = Flg_ck(&Ending_save_copy[0x388 / 4], 0x13);
			if (flg_res != 0) {
				Flg_on(G.SaveData.Zapping_flg, 0x13);
			}
		}
		else {
			G.SaveData.Front_play_time = 0;
			G.SaveData.Dummy1[0] = Ending_save_zapping_cnt;
			G.SaveData.Zapping_flg[0] = Ending_save_copy[0x388 / 4];
			G.SaveData.Zapping_flg[1] = Ending_save_copy[0x38c / 4];
		}
		G.SaveData.Weapon_mugen = Ending_save_weapon_mugen & 1;
		cnt = 2;
		if ((Ending_state_flg & 0x10) != 0) {
			cnt = 3;
			G.SaveData.Item_boxwork[2].Id = 15;
			G.SaveData.Item_boxwork[2].Num = 0xff;
			G.SaveData.Item_boxwork[2].Size = 1;
		}
		walk_a = cnt;
		if ((Ending_state_flg & 0x20) != 0) {
			walk_a = cnt + 1;
			G.SaveData.Item_boxwork[cnt].Id = 17;
			G.SaveData.Item_boxwork[cnt].Num = 0xff;
			G.SaveData.Item_boxwork[cnt].Size = 1;
		}
		if ((Ending_state_flg & 0x40) != 0) {
			Flg_on(&G.SaveData.Weapon_mugen, 0x12);
			G.SaveData.Item_boxwork[walk_a].Id = 18;
			G.SaveData.Item_boxwork[walk_a].Num = 1;
			G.SaveData.Item_boxwork[walk_a].Size = 1;
		}
	}
	return;
}

/* Ending_inittbl_load - ENDING::8011d1c8
 * Load Init_tbl from CD and copy into G.SaveData. */
/* DS 0x8011d1c8 (Ending_inittbl_load) */
static void Ending_inittbl_load(void)
{
	ULONG *pInit_buf;

	pInit_buf = (ULONG *)Ending_inittbl_buf;
	Cd_read(INIT_TBL_DAT, pInit_buf, 1, Ending_mess_init_tbl);
	memcpy16((ULONG *)&G.SaveData, (ULONG *)&((SAVE_DATA *)pInit_buf)[(UINT)G.Dummy8[1]], 0x798);
	return;
}

/* Ending_tim_load - ENDING::8011d240
 * Load ending TIM file from CD by sector-table index.
 * Sets error flag in Ending_load_err_flg if read fails. */
/* DS 0x8011d240 (Ending_tim_load) */
static void Ending_tim_load(UINT Tim_no)
{
	ULONG read_ret;

	read_ret = Cd_read((UINT)*(Ending_tim_sect_tbl + (Tim_no & 0xff)), Bg_img_buf, 0, Ending_mess_end_tim);
	if (read_ret == 0xffffffff) {
		Ending_load_err_flg |= 1;
	}
	return;
}

/* Ending_fade_set - ENDING::8011d2a8
 * Set fade parameters for one fade slot: writes control word, color,
 * and RECT into Main.Fade[Fade_no]. */
/* DS 0x8011d2a8 (Ending_fade_set) */
static void Ending_fade_set(int Fade_no, USHORT Ctrl, UINT Col, ULONG *pPos)
{
	UCHAR fade_code;
	ULONG rect_wh;
	UINT col_word;

	/* FADE fields: Kido; T[0]/T[1] are TILE records (r0 colour word, x0/y0 position, w/h size). */
	fade_code = Main.Fade[Fade_no].T[0].code;
	Main.Fade[Fade_no].Kido = Ctrl;
	col_word = Col | (UINT)fade_code << 0x18;
	*(UINT *)&Main.Fade[Fade_no].T[0].r0 = col_word;
	*(UINT *)&Main.Fade[Fade_no].T[1].r0 = col_word;
	if (pPos == NULL) {
		rect_wh = 0xf00140;   /* packed screen rect: h=0xf0 (240) << 16 | w=0x140 (320) */
		*(ULONG *)&Main.Fade[Fade_no].T[0].x0 = 0;
		*(ULONG *)&Main.Fade[Fade_no].T[0].w = 0xf00140;
		*(ULONG *)&Main.Fade[Fade_no].T[1].x0 = 0;
	}
	else {
		*(ULONG *)&Main.Fade[Fade_no].T[0].x0 = *pPos;
		*(ULONG *)&Main.Fade[Fade_no].T[0].w = pPos[1];
		*(ULONG *)&Main.Fade[Fade_no].T[1].x0 = *pPos;
		rect_wh = pPos[1];
	}
	*(ULONG *)&Main.Fade[Fade_no].T[1].w = rect_wh;
	return;
}

/* Ending_timeline - ENDING::8011d330
 * Ending timeline controller: fires scene transitions, credit reveals,
 * and TIM loads at specific Ending_timeline_cnt frame counts. */
/* DS 0x8011d330 (Ending_timeline) */
static void Ending_timeline(void)
{
	BOOL loop_flg;
	int wait_cnt;

	if (Ending_timeline_cnt == 0) {
		Ending_stat_rec_ptr = (UCHAR *)Ending_stat_rec_tbl;
		Ending_msg_state = 0;
		Ending_msg_row = 0;
		Ending_msg_col = 0;
		Ending_type_cnt = 0;
		Ending_msg_row_cnt = 0;
		Ending_fade_ofs = 0;
		Ending_scroll_step = 0;
		Ending_exit_req = 0;
		Ending_msg_col_base = 0;
		Fade_set(0x200, 0, 7, 0);
		wait_cnt = 0x59;
		do {
			Task_sleep(1);
			loop_flg = wait_cnt != 0;
			wait_cnt = wait_cnt + -1;
		} while (loop_flg);
	}
	if (Ending_timeline_cnt == 0x92)  { Ending_stat_flg_set(3,    1, 0); }
	if (Ending_timeline_cnt == 0x11e) { Ending_stat_flg_set(3,    0, 0); }
	if (Ending_timeline_cnt == 0x134) { Ending_stat_flg_set(0xc,  1, 0); }
	if (Ending_timeline_cnt == 0x2a9) { Ending_stat_flg_set(0xc,  0, 0); }
	if (Ending_timeline_cnt == 0x2c1) { Ending_stat_flg_set(0x30, 1, 0); }
	if (Ending_timeline_cnt == 0x4b0) { Ending_stat_flg_set(0x30, 0, 0); }
	if ((Ending_timeline_cnt == 0x4db) && (G.SaveData.Sce_timer == 3)) {
		Ending_tim_load(7);
		Bg_set_mode(0, 0);
	}
	if (Ending_timeline_cnt == 0x4df) { Ending_stat_flg_set(0xc0,  1, 0); }
	if (Ending_timeline_cnt == 0x663) { Ending_stat_flg_set(0xc0,  0, 0); }
	if (Ending_timeline_cnt == 0x679) { Ending_stat_flg_set(0x300, 1, 0); }
	if (Ending_timeline_cnt == 0x7f4) { Ending_stat_flg_set(0x300, 0, 0); }
	Ending_timeline_cnt++;
	return;
}

/* Ending_main - ENDING::8011d58c
 * Ending main task loop: drives credit scroll, typewriter text effect,
 * fade in/out state machine, and score display. */
/* DS 0x8011d58c (Ending_main) */
static void Ending_main(void)
{
	ULONG fade_sts;
	BOOL bFadeEnd;
	do {
		Ending_timeline();
		Ending_fade_ofs += Ending_scroll_step;
		Fade_adjust(0, (int)Ending_fade_ofs, 0, NULL);
		fade_sts = Fade_status(0);
		bFadeEnd = (fade_sts != 0);
		if (bFadeEnd) {
			Ending_scroll_step = 0;
			Ending_fade_ofs = -1;
		}
		else {
			switch (Ending_msg_state) {
			case 0:
				Ending_msg_col = 0;
				Ending_msg_state = 1;
				/* fall through */
			case 1:
				if (Ending_msg_row_tbl[Ending_msg_row + Ending_msg_col_base][Ending_msg_col] == 0xfe) {
					Ending_msg_row++;
					if (Ending_msg_row < 0xb) {
						Snd_se_stad(0x2230000, (VECTOR *)0x0);
						Ending_msg_state = 2;
					}
					else {
						Ending_msg_state = 3;
					}
				}
				else {
					Memcpy(&Ending_msg_line_buf, Ending_msg_row_tbl[Ending_msg_row + Ending_msg_col_base], Ending_msg_col + 1);
					Ending_msg_line_buf1[Ending_msg_col] = 0xfe;
					Mess_print(0x3c, Ending_msg_row * 0xd + 0x20, &Ending_msg_line_buf, 0x4034);
					if (Ending_type_cnt == 0) {
						fade_sts = 0x2240000;
						if ((&Ending_msg_line_buf)[Ending_msg_col] != 0) {
							fade_sts = 0x2220000;
						}
						Snd_se_stad(fade_sts, (VECTOR *)0x0);
						if (Ending_type_cnt == 0) {
							(&Ending_msg_line_buf)[Ending_msg_col] = 0xfe;
						}
					}
					Mess_print(0x3c, Ending_msg_row * 0xd + 0x20, &Ending_msg_line_buf, 0x4002);
					Ending_type_cnt++;
					if (Ending_msg_char_cnt_tbl[Ending_msg_row_cnt] < Ending_type_cnt) {
						Ending_type_cnt = 0;
						Ending_msg_col++;
						Ending_msg_row_cnt++;
					}
				}
				break;
			case 2:
				Ending_type_cnt++;
				if (0x14 < Ending_type_cnt) {
					Ending_type_cnt = 0;
					Ending_msg_state = 0;
				}
				break;
			case 3:
				Ending_type_cnt = 0x3c;
				Ending_msg_state = 4;
				/* fall through */
			case 4:
				Ending_type_cnt--;
				if (Ending_type_cnt == 0) {
					if ((Ending_scroll_step == 0) && (Ending_scroll_step = 0x80, Ending_exit_req == 0)) {
						Ending_scroll_step = 0x400;
					}
					Ending_msg_state = 5;
				}
				break;
			case 5:
				fade_sts = Fade_status(0);
				bFadeEnd = (fade_sts != 0);
				break;
			}
			if (!bFadeEnd) {
				Ending_msg_draw_idx = 0;
				if (Ending_msg_row != 0) {
					do {
						if (Ending_msg_row_tbl[Ending_msg_draw_idx + Ending_msg_col_base] != NULL) {
							Mess_print(0x3c, Ending_msg_draw_idx * 0xd + 0x20, Ending_msg_row_tbl[Ending_msg_draw_idx + Ending_msg_col_base], 0x4002);
						}
						Ending_msg_draw_idx++;
					} while (Ending_msg_draw_idx < Ending_msg_row);
				}
				Ending_moji_draw();
				Task_sleep(1);
			}
		}
		if (bFadeEnd) {
			if (Ending_exit_req != 0) {
				Ending_exit();
				return;
			}
			if (G.SaveData.Sce_timer == 3) {
				Fade_set(0x203, -0x100, 7, 0xb);
			}
			Fade_set(0x200, 0, 7, 0);
			Ending_exit_req = 1;
			Ending_msg_state = 0;
			Ending_msg_row = 0;
			Ending_msg_col = 0;
			Ending_type_cnt = 0;
			Ending_msg_row_cnt = 0;
			Ending_fade_ofs = 0;
			Ending_scroll_step = 0;
			Ending_msg_col_base = 0xb;
		}
	} while (1);
}

/* Ending_moji_draw - ENDING::8011da60
 * Draw stat-screen character tiles: emits POLY_FT4 quads for each
 * active display entry in the stat-screen table. */
/* DS 0x8011da60 (Ending_moji_draw) */
static void Ending_moji_draw(void)
{
	USHORT x_pos;
	USHORT y_pos;
	USHORT x_end;
	USHORT y_end;
	char u_end;
	char v_end;
	ENDING_STAT_GLYPH *pGlyph;
	char *pBuf;
	UCHAR *pRec;
	int rem_cnt;
	UCHAR *pFlg;
	POLY_FT4 *pPrim;
	USHORT clut_val;

	rem_cnt = 0xc;
	pPrim = Ending_prim[G.Cbuf_id];
	pRec = Ending_stat_rec_ptr + 6;
	pBuf = (char *)&pPrim->v3;
	pFlg = Ending_stat_rec_ptr;
	do {
		rem_cnt = rem_cnt + -1;
		if ((*pFlg & 1) != 0) {
			x_pos = *(USHORT *)(pRec + -2);
			y_pos = *(USHORT *)pRec;
			pGlyph = &Ending_stat_glyph_tbl[pRec[-3]];
			x_end = x_pos + pGlyph->W;
			y_end = y_pos + pGlyph->H;
			u_end = pGlyph->U + pGlyph->Uw;
			v_end = pGlyph->V + pGlyph->Vh;
			SetPolyFT4(pPrim);
			*(ULONG *)(pBuf + -0x21) = 0x2e808080;
			*(short *)(pBuf + -0x1d) = x_pos;
			*(short *)(pBuf + -0x1b) = y_pos;
			pBuf[-0x19] = pGlyph->U;
			*(USHORT *)(pBuf + -0x15) = x_end;
			*(short *)(pBuf + -0x13) = y_pos;
			pBuf[-0x11] = u_end;
			pBuf[-0x18] = pGlyph->V;
			pBuf[-0x10] = pGlyph->V;
			clut_val = GetClut((UINT)pGlyph->Clut_x << 4, (UINT)pGlyph->Clut_y);
			*(USHORT *)(pBuf + -0x17) = clut_val;
			*(short *)(pBuf + -0xd) = x_pos;
			*(USHORT *)(pBuf + -0xb) = y_end;
			*(USHORT *)(pBuf + -0xf) = pGlyph->Tpage & 0xf;
			pBuf[-8] = v_end;
			*(USHORT *)(pBuf + -5) = x_end;
			*(USHORT *)(pBuf + -3) = y_end;
			pBuf[-1] = u_end;
			*pBuf = v_end;
			pBuf[-9] = pGlyph->U;
			pBuf += 0x28;
			AddPrim(Moji.Moji_mode[G.Cbuf_id] + 3, pPrim);
			pPrim++;
		}
		pFlg += 8;
		pRec += 8;
	} while (rem_cnt != 0);
	return;
}

/* Ending_stat_flg_set - ENDING::8011dc5c
 * Set/clear display flags for stat display slots (12 slots).
 * Slot_mask: bitmask, Val: value, Clr_sw: clear-on-zero flag. */
/* DS 0x8011dc5c (Ending_stat_flg_set) */
static void Ending_stat_flg_set(UINT Slot_mask, UCHAR Val, char Clr_sw)
{
	UCHAR *pFlg;
	UINT slot_no;

	slot_no = 0;
	pFlg = Ending_stat_rec_ptr;
	do {
		if ((Slot_mask >> (slot_no & 0x1f) & 1) == 0) {
			if (Clr_sw != 0) {
				*pFlg = 0;
			}
		}
		else {
			*pFlg = Val;
		}
		if ((pFlg[1] & 0x20) != 0) {
			*pFlg = 0;
		}
		slot_no++;
		pFlg += 8;
	} while (slot_no < 0xc);
	return;
}

/*
 * Ghidra defines no function over this range, so
 * the entries below are given by address only (overlay dump):
 *  - ENDING::8011a0e0 .. 8011a18c : overlay entry
 *  - Routine table ENDING::8011de34 .. 8011de40: 8011de34 = Ending_fade_out,
 *      8011de38 = Ending_fade_unk (ENDING::8011a8e4), 8011de3c = Ending_fade_in,
 *      8011de40 = Ending_exit.
 *  - Overlay and DS globals are declared as extern at the top of this file,
 *      with their ENDING:: / DS addresses. Types are inferred from use.
 */
