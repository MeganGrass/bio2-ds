/*
 * ENDING.C - Ending sequence overlay (Resident Evil 2 DS, SLUS_00748)
 * Overlay addresses use the ENDING:: prefix (ENDING::8011xxxx).
 * Function addresses in the comments are DS retail addresses from Ghidra.
 */

#include "TYPES.H"
#include <ASM.H>
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
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

/* Overlay data (ENDING::8011xxxx) and DS work RAM referenced by this overlay, defined here.
 * Items inside ENDING.BIN (ENDING::8011a000..8012585a) carry the retail bytes as initialisers;
 * items past the end of the file are zero-initialised RAM.
 * Record tables are indexed by address-of, e.g. (&Ending_glyph_W)[n]. */

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
UCHAR Ending_msg_row_00[19] = {
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

/* DS 0x8011df34 (Ending_msg_row_tbl): 22 row pointers in entry order; entries 1, 6, 17, 18 and 20 share Ending_msg_row_fe */
UCHAR *Ending_msg_row_tbl[22] = {
	Ending_msg_row_00, Ending_msg_row_fe, Ending_msg_row_02, Ending_msg_row_03, Ending_msg_row_04, Ending_msg_row_05,
	Ending_msg_row_fe, Ending_msg_row_07, Ending_msg_row_08, Ending_msg_row_09, Ending_msg_row_10, Ending_msg_row_11,
	Ending_msg_row_12, Ending_msg_row_13, Ending_msg_row_14, Ending_msg_row_15, Ending_msg_row_16, Ending_msg_row_fe,
	Ending_msg_row_fe, Ending_msg_row_19, Ending_msg_row_fe, Ending_msg_row_21
};
/* DS 0x8011ded4 (Ending_stat_rec_tbl) */
UCHAR  Ending_stat_rec_tbl = 0x00;   /* stat-display record base */
/* DS 0x8011de48 (Ending_stat_glyph_Tbl) */
UCHAR  Ending_stat_glyph_Tbl = 0x0C;   /* glyph record table (stride 10) */
/* DS 0x8011de49 (Ending_stat_glyph_Clut_x) */
UCHAR  Ending_stat_glyph_Clut_x = 0x00;   /* glyph record, +1 */
/* DS 0x8011de4a (Ending_stat_glyph_Clut_y) */
UCHAR  Ending_stat_glyph_Clut_y = 0xF1;   /* glyph record, +2 (USHORT) */
/* DS 0x8011de4c (Ending_stat_glyph_U) */
UCHAR  Ending_stat_glyph_U = 0x00;   /* glyph record, +4 */
/* DS 0x8011de4d (Ending_stat_glyph_V) */
UCHAR  Ending_stat_glyph_V = 0x00;   /* glyph record, +5 */
/* DS 0x8011de4e (Ending_stat_glyph_Uw) */
UCHAR  Ending_stat_glyph_Uw = 0xFF;   /* glyph record, +6 */
/* DS 0x8011de4f (Ending_stat_glyph_Vh) */
UCHAR  Ending_stat_glyph_Vh = 0x20;   /* glyph record, +7 */
/* DS 0x8011de50 (Ending_stat_glyph_W) */
UCHAR  Ending_stat_glyph_W = 0xFF;   /* glyph record, +8 */
/* DS 0x8011de51 (Ending_stat_glyph_H) */
UCHAR  Ending_stat_glyph_H = 0x20;   /* glyph record, +9 */
/* DS 0x8011df8c (Ending_msg_char_cnt_tbl) */
UCHAR  Ending_msg_char_cnt_tbl = 0x05;   /* per-row character count table */
/* DS 0x8011e0b8 (Ending_glyph_Tbl) */
UCHAR  Ending_glyph_Tbl = 0x0E;   /* glyph descriptor table (stride 10) */
/* DS 0x8011e0ba (Ending_glyph_Clut) */
UCHAR  Ending_glyph_Clut = 0xC0;   /* glyph descriptor, +2 (short) */
/* DS 0x8011e0bc (Ending_glyph_U) */
UCHAR  Ending_glyph_U = 0x00;   /* glyph descriptor, +4 */
/* DS 0x8011e0bd (Ending_glyph_V) */
UCHAR  Ending_glyph_V = 0x00;   /* glyph descriptor, +5 */
/* DS 0x8011e0be (Ending_glyph_Uw) */
UCHAR  Ending_glyph_Uw = 0x0D;   /* glyph descriptor, +6 */
/* DS 0x8011e0bf (Ending_glyph_Vh) */
UCHAR  Ending_glyph_Vh = 0x0E;   /* glyph descriptor, +7 */
/* DS 0x8011e0c0 (Ending_glyph_W) */
UCHAR  Ending_glyph_W = 0x0D;   /* glyph descriptor, +8 */
/* DS 0x8011e0c1 (Ending_glyph_H) */
UCHAR  Ending_glyph_H = 0x0E;   /* glyph descriptor, +9 */
/* DS 0x8011e4b0 (Ending_glyph_idx_tbl) */
UCHAR  Ending_glyph_idx_tbl = 0x02;   /* glyph index list (0xff terminated) */

/* Glyph record (stride 10) at Ending_glyph_Tbl (ENDING::8011e0b8): one record base, fields at +0..+9.
 * Natural alignment gives the same offsets as the retail record. */
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

/* Result/score display state (ENDING overlay data) */
/* DS 0x8011e324 (Ending_result_rec_tbl) */
UCHAR  Ending_result_rec_tbl = 0x00;   /* result slot records (12 bytes each) */
/* DS 0x8011e32a (Ending_result_rec_Y) */
UCHAR  Ending_result_rec_Y = 0x5A;   /* result slot record field base */
/* DS 0x8011e3c0 (Ending_result_mode_flg) */
UCHAR  Ending_result_mode_flg = 0x00;   /* result display mode flags */
/* DS 0x8011e3c8 (Ending_result_rank_col) */
UINT   Ending_result_rank_col = 0x00000000;   /* result rank colour bits */
/* DS 0x8011e404 (Ending_item_a_col) */
UINT   Ending_item_a_col = 0x00000000;   /* item-A colour */
/* DS 0x8011e410 (Ending_item_b_col) */
UINT   Ending_item_b_col = 0x00000000;   /* item-B colour */
/* DS 0x8011e41c (Ending_item_c_col) */
UINT   Ending_item_c_col = 0x00000000;   /* item-C colour */
/* DS 0x8011e498 (Ending_fade_scratch0) */
UCHAR  Ending_fade_scratch0 = 0x00;   /* fade-in scratch (byte store) */
/* DS 0x8011e4a4 (Ending_fade_scratch1) */
UCHAR  Ending_fade_scratch1 = 0x00;   /* fade-in scratch (byte store) */
/* DS 0x8011e578 (Ending_scroll_step) */
SHORT  Ending_scroll_step = 0x0000;   /* timeline step increment */
/* DS 0x8011e57c (Ending_scroll_spd) */
int    Ending_scroll_spd = 0x00000000;   /* stat scroll speed */
/* DS 0x8011e580 (Ending_msg_col) */
int    Ending_msg_col = 0x00000000;   /* message column index */
/* DS 0x8011e588 (Ending_msg_line_buf) */
UCHAR  Ending_msg_line_buf = 0x00;   /* message line buffer */
/* DS 0x8011e589 (Ending_msg_line_buf1) */
UCHAR  Ending_msg_line_buf1 = 0x00;   /* message line buffer, +1 */
/* DS 0x8011e5a8 (Ending_msg_row) */
int    Ending_msg_row = 0x00000000;   /* message row index */
/* DS 0x8011e5b0 (Ending_inittbl_buf) */
UCHAR  Ending_inittbl_buf = 0x00;   /* Init_tbl load buffer */

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
/* DS 0x80121378 (Ending_room_work) */
UCHAR  Ending_room_work = 0x00;   /* ROOM_WORK block */
/* DS 0x8012137e (Ending_room_flg) */
UCHAR  Ending_room_flg = 0x00;   /* room flag */
/* DS 0x80121380 (Ending_pwork_tim) */
UCHAR *Ending_pwork_tim = 0;   /* work pointer (-> Ending_tim_buf) */
/* DS 0x80121384 (Ending_pwork_inita) */
UCHAR *Ending_pwork_inita = 0;   /* work pointer (-> Ending_inita_tbl) */
/* DS 0x80121388 (Ending_pwork_initb) */
UCHAR *Ending_pwork_initb = 0;   /* work pointer (-> Ending_initb_tbl) */
/* DS 0x801213fc (Ending_fade_dir) */
UCHAR  Ending_fade_dir = 0x00;   /* fade direction state */
/* DS 0x80121400 (Ending_timeline_cnt) */
int    Ending_timeline_cnt = 0x00000000;   /* timeline frame counter */
/* DS 0x80121404 (Ending_msg_col_base) */
UCHAR  Ending_msg_col_base = 0x00;   /* message table column base */
/* DS 0x8012142d (Ending_cbuf) */
char   Ending_cbuf = 0x00;   /* character buffer (per cut-buffer, stride 8000) */
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
/* DS 0x801252b8 (Ending_save_copy) */
ULONG  Ending_save_copy = 0x00000000;   /* copied save record (0x598 bytes) */
/* DS 0x801252d4 (Ending_zombie_cnt) */
USHORT Ending_zombie_cnt = 0x0000;   /* zombie count (save copy) */
/* DS 0x801252d6 (Ending_save_rank_in) */
USHORT Ending_save_rank_in = 0x0000;   /* save field (rank input) */
/* DS 0x801252d8 (Ending_save_bonus_in) */
USHORT Ending_save_bonus_in = 0x0000;   /* save field (bonus input) */
/* DS 0x801252e0 (Ending_save_weapon_mugen) */
UINT   Ending_save_weapon_mugen = 0x00000000;   /* save field (weapon mugen) */
/* DS 0x801254d0 (Ending_flg_ck_tbl) */
ULONG  Ending_flg_ck_tbl = 0x00000000;   /* flag word array (Flg_ck) */
/* DS 0x80125640 (Ending_flg_tbl) */
ULONG  Ending_flg_tbl = 0x00000000;   /* ending flag word array (Flg_on/Flg_ck) */
/* DS 0x80125644 (Ending_flg_tbl_b1) */
ULONG  Ending_flg_tbl_b1 = 0x00000000;   /* zapping flags word 1 (low byte used) */
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
/* DS 0x8012a000 (Ending_tim_buf) */
UCHAR  Ending_tim_buf;   /* TIM load buffer */
/* DS 0x8012a0c0 (Ending_inita_tbl) */
UCHAR  Ending_inita_tbl;   /* init table A */
/* DS 0x8012ace0 (Ending_initb_tbl) */
UCHAR  Ending_initb_tbl;   /* init table B */
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
	UCHAR cbuf_save;
	ULONG read_size;
	BGMCTR bgm_ctr[4];

	Snd_bgm_fade_ON(0xb4, 24);
	G.Status_flg &= ~STAT_CINESCO;
	Bg_set_mode(2, 0);
	Main.Bg_chg_flg = 0;
	Pad_rep_set(0xf01c, 0x308);
	SetGeomScreen(200);
	SetGeomOffset(0, 0);
	G.pFree_work = &Ending_tim_buf;
	Main.Vloop = 0;
	Ending_load_err_flg = 0;
	if ((G.System_flg & SYS_EX_BATTLE) == 0) {
		do {
			read_size = Cd_read(END00_TIM, (ULONG *)G.pFree_work, 1, Ending_mess_end00);
		} while (read_size == 0xffffffff);
		G.Cclut = 16;
		G.Ctpage = 8;
	}
	else {
		do {
			read_size = Cd_read(END19_TIM, (ULONG *)G.pFree_work, 1, Ending_mess_end1a);
		} while (read_size == 0xffffffff);
		G.Cclut = 17;
		G.Ctpage = 12;
	}
	Mapping_tim((ULONG *)G.pFree_work);
	do {
		read_size = Cd_read(ROOM800_SND, (ULONG *)G.pFree_work, 1, Ending_mess_room800);
	} while (read_size == 0xffffffff);
	Ending_room_flg = 0;
	Ending_pwork_inita = &Ending_inita_tbl;
	Ending_pwork_tim = &Ending_tim_buf;
	Ending_pwork_initb = &Ending_initb_tbl;
	G.pRoom = (ROOM_WORK *)&Ending_room_work;
	G.pFree_work += read_size;
	Snd_sys_room_set();
	Ending_fade_dir = 1;
	if ((G.Status_flg & STAT_SPECIAL_RUN) == 0) {
		Ending_disp_mode = 0;
	}
	else {
		Ending_disp_mode = 0x11;
	}
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		G.ExData[2]    = (UCHAR)G.SaveData.Sce_timer;
		{
			UCHAR *pSrc = (UCHAR *)&G.SaveData.ExData3[8];
			int i;
			for (i = 0; i < 0x3c; i++) {
				G.ExData[4 + i] = pSrc[i];
			}
		}
		G.ExData[3]    = (UCHAR)G.SaveData.Player_life;
	}
	Ending_timeline_cnt = 0;
	Ending_state_flg = 0x2000;
	Ending_result_slot_mask = 0;
	memcpy16((ULONG *)&Ending_save_copy, &G.SaveData.Game_play_time, 0x598);
	Ending_rank_no = 0;
	Ending_score_time = 0;
	Ending_score_life = 0;
	Ending_score_item = 0;
	Ending_score_set();
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
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
	cbuf_save = G.Cbuf_id;
	G.Cbuf_id = 0;
	Fade_adjust(0, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(1, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(2, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(3, 0xffffffff, 0xffffff, NULL);
	G.Cbuf_id = 1;
	Fade_adjust(0, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(1, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(2, 0xffffffff, 0xffffff, NULL);
	Fade_adjust(3, 0xffffffff, 0xffffff, NULL);
	G.Cbuf_id = cbuf_save;
	return;
}

/* Ending_exit - ENDING::8011a630
 * Ending teardown / return to title: waits 60 frames, calls Init_global,
 * sets Task_lchain to restart. */
/* DS 0x8011a630 (Ending_exit) */
static void Ending_exit(void)
{
	BOOL loop_flg;
	int wait_cnt;

	wait_cnt = 0x3b;
	do {
		Bg_set_mode(2, 0);
		Task_sleep(1);
		loop_flg = wait_cnt != 0;
		wait_cnt = wait_cnt + -1;
	} while (loop_flg);
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
				Ending_fade_scratch0 = 0;
				Ending_fade_scratch1 = 0;
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
 * Timeline thresholds are frame counts. Ending_result_rec_tbl records are 12 bytes,
 * so (&Ending_result_rec_tbl)[12 * n] is record n. */
/* DS 0x8011a8e4 (Ending_fade_unk) */
static void Ending_fade_unk(void)
{
	BGMCTR bgm_ctr;

	switch (Ending_disp_mode) {
	case 0:
		if (pEdt_adr[0].id_check == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				bgm_ctr.Main = 0x26;
				bgm_ctr.Sub = 0xff;
				pBgm = &bgm_ctr;
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
				bgm_ctr.Main = 0xff;
				bgm_ctr.Sub = 0xff;
				pBgm = &bgm_ctr;
				Seq_ctr[0].Ctrl = 0;
				Seq_ctr[0].Vol.left = 100;
				Seq_ctr[0].Vol.right = 100;
				Snd_bgm_main();
			}
			Ending_tim_load((G.Status_flg & STAT_PLAYER) != 0);
			Ending_fade_set(0, 0, 0, NULL);
			Ending_state_flg |= 0x800;
			Ending_fade_out();
			bgm_ctr.Main = 0x2a;
			bgm_ctr.Sub = 0xff;
			pBgm = &bgm_ctr;
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
			(&Ending_result_rec_tbl)[12 * (Ending_rank_no + 4)] = 5;
			if ((G.Status_flg & STAT_SCENARIO_B) != 0) {
				if ((G.Status_flg & STAT_PLAYER) == 0) {
					(&Ending_result_rec_tbl)[12 * 11] = 5;
				}
				else {
					(&Ending_result_rec_tbl)[12 * 12] = 5;
				}
			}
			else {
				if ((G.Status_flg & STAT_PLAYER) == 0) {
					(&Ending_result_rec_tbl)[12 * 9] = 5;
				}
				else {
					(&Ending_result_rec_tbl)[12 * 10] = 5;
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
		bgm_ctr.Main = 0xff;
		bgm_ctr.Sub = 0xff;
		pBgm = &bgm_ctr;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		bgm_ctr.Main = 0xff;
		bgm_ctr.Sub = 0x33;
		pBgm = &bgm_ctr;
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
		memcpy16(&G.SaveData.Game_play_time, &Ending_save_copy, 0x598);
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
		memcpy16(&G.SaveData.Game_play_time, &Ending_save_copy, 0x598);
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
		bgm_ctr.Main = 0x30;
		bgm_ctr.Sub = 0xff;
		pBgm = &bgm_ctr;
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
		bgm_ctr.Main = 0x2a;
		bgm_ctr.Sub = 0xff;
		pBgm = &bgm_ctr;
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
			(&Ending_result_rec_tbl)[12 * (Ending_rank_no + 4)] = 5;
			(&Ending_result_rec_tbl)[12 * 9] = 0;
			(&Ending_result_rec_tbl)[12 * 10] = 0;
			(&Ending_result_rec_tbl)[12 * 11] = 0;
			(&Ending_result_rec_tbl)[12 * 12] = 0;
			Ending_result_flg_set(0x1e000, 5, 0);
			if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) {
				Ending_fade_scratch0 = 0;
				Ending_fade_scratch1 = 5;
			}
			else {
				Ending_fade_scratch0 = 5;
				Ending_fade_scratch1 = 0;
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
		memcpy16(&G.SaveData.Game_play_time, &Ending_save_copy, 0x598);
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
	UCHAR *pFlg;
	int prim_code;
	int rem_cnt;
	int tbl_ofs;
	USHORT x_pos;
	USHORT *pRec;
	UCHAR *pIdx;

	pFlg = &Ending_result_rec_tbl;
	pRec = (USHORT *)&Ending_result_rec_Y;
	Ending_pPoly = (POLY_FT4 *)((UINT)G.Cbuf_id * 8000 + -0x7fedebf8);
	rem_cnt = 0x21;
	do {
		rem_cnt = rem_cnt + -1;
		if ((pFlg[1] & 0x20) != 0) {
			*pFlg = 0;
		}
		if ((*pFlg & 1) != 0) {
			pIdx = &Ending_glyph_idx_tbl;
			row_no = 0;
			if (*(UCHAR *)((int)pRec + -3) != 0) {
				do {
					if (*pIdx == 0xff) {
						row_no++;
					}
					pIdx++;
				} while (*(UCHAR *)((int)pRec + -3) != row_no);
			}
			ot_idx = *(UCHAR *)(pRec - 2);
			prim_code = 0x2c000000;
			if ((*pFlg & 8) != 0) {
				prim_code = 0x2e000000;
				scroll_pos = *(int *)(pRec + 1) - Ending_scroll_spd;
				*(int *)(pRec + 1) = scroll_pos;
				if (0x80807f < scroll_pos) {
					*pFlg = 0;
					pRec[1] = 0;
					pRec[2] = 0;
				}
			}
			if ((*pFlg & 4) != 0) {
				prim_code = 0x2e000000;
				scroll_pos = *(int *)(pRec + 1) + Ending_scroll_spd;
				*(int *)(pRec + 1) = scroll_pos;
				if (0xf0f0ef < scroll_pos) {
					prim_code = 0x2c000000;
					*pFlg = *pFlg & 0xfb;
					pRec[1] = -0x7f80;
					pRec[2] = 0x80;
				}
			}
			x_pos = pRec[-1];
			scroll_pos = *(int *)(pRec + 1);
			y_pos = *pRec;
			ch_idx = *pIdx;
			p = Ending_pPoly;
			while (Ending_pPoly = p, ch_idx != 0xff) {
				tbl_ofs = (UINT)ch_idx * 10;
				ch_idx = (&Ending_glyph_W)[tbl_ofs];
				glyph_h = (&Ending_glyph_H)[tbl_ofs];
				glyph_u = (&Ending_glyph_U)[tbl_ofs];
				glyph_uw = (&Ending_glyph_Uw)[tbl_ofs];
				glyph_v = (&Ending_glyph_V)[tbl_ofs];
				glyph_vh = (&Ending_glyph_Vh)[tbl_ofs];
				SetPolyFT4(p);
				p->x0 = x_pos;
				*(int *)&p->r0 = prim_code + scroll_pos;
				p->y0 = y_pos;
				p->u0 = (&Ending_glyph_U)[tbl_ofs];
				p->v0 =(&Ending_glyph_V)[tbl_ofs];
				attr_val = *(USHORT *)(&Ending_glyph_Clut + tbl_ofs);
				p->x1 = x_pos + (USHORT)ch_idx;
				p->y1 = y_pos;
				p->u1 = glyph_u + glyph_uw;
				p->clut = attr_val + 0x400;
				p->v1 =(&Ending_glyph_V)[tbl_ofs];
				attr_val = ((ENDING_GLYPH *)((UCHAR *)&Ending_glyph_Tbl + tbl_ofs))->tpage;
				p->x2 = x_pos;
				p->y2 = y_pos + (USHORT)glyph_h;
				p->tpage = attr_val + 0x80;
				tex_u2 = (&Ending_glyph_U)[tbl_ofs];
				p->v2 = glyph_v + glyph_vh;
				p->x3 = x_pos + (USHORT)ch_idx;
				p->y3 = y_pos + (USHORT)glyph_h;
				p->u3 = glyph_u + glyph_uw;
				p->v3 = glyph_v + glyph_vh;
				p->u2 = tex_u2;
				AddPrim(G.pOt1 + ot_idx, p);
				pIdx++;
				p = Ending_pPoly + 1;
				x_pos = x_pos + (USHORT)(UCHAR)(&Ending_glyph_W)[tbl_ofs];
				ch_idx = *pIdx;
			}
		}
		pRec += 6;
		pFlg += 0xc;
	} while (rem_cnt != 0);
	if ((Ending_result_mode_flg & 1) != 0) {
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
	int tbl_ofs;
	int ofs;
	USHORT *pCoord;
	ENDING_GLYPH *pGlyph;
	int dig_idx;
	ULONG rank_x[6];   /* ENDING::8011a08c table, retail sp+0x10; rank_x[5] (sp+0x24) reserved, never read */
	ULONG rank_y[6];   /* ENDING::8011a0a0 table, retail sp+0x28; rank_y[5] (sp+0x3c) reserved, never read */

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
	if ((Ending_result_mode_flg & 0xc) == 0) {
		col_code = 0x2c000000;
	}
	col_code = Ending_result_rank_col | col_code;
	dig_idx = 9;
	do {
		p = Ending_pPoly;
		ofs = dig_idx * 2;
		tbl_ofs = (UINT)(UCHAR)(&Ending_time_dig_d10)[dig_idx] * 10;
		pGlyph = (ENDING_GLYPH *)((UCHAR *)&Ending_glyph_Tbl + tbl_ofs);
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
	UCHAR *pFlg;
	UINT slot_no;

	pFlg = &Ending_result_rec_tbl;
	slot_no = 0;
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
		pFlg += 0xc;
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
	if (((G.Status_flg & STAT_INF_AMMO) != 0) &&
	   (Ending_rank_no = Ending_rank_no - 0x32, low_rank_flg)) {
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
		Ending_item_b_col = 0;
		Ending_state_flg |= 0x20;
	}
	else if ((Ending_score_time == 100) && (rank_idx < 2)) {
		Ending_item_b_col = 0x808080;
		Ending_state_flg |= 0xa0;
	}
	if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
		ret_val = Ending_item_ck(15);
		if (ret_val != 0) {
			Ending_item_a_col = 0;
			Ending_state_flg |= 0x10;
		}
		ret_val = Ending_item_ck(18);
		if (ret_val != 0) {
			Ending_item_c_col = 0;
			Ending_state_flg |= 0x40;
		}
		flg_res = Flg_ck((ULONG *)&Ending_flg_ck_tbl, 0x9c);
		if (flg_res != 0) {
			return;
		}
		Flg_on(&Ending_flg_tbl, 0x12);
		return;
	}
	ret_val = Ending_item_ck(15);
	if (ret_val != 0) {
		Ending_item_a_col = 0;
		Ending_state_flg |= 0x10;
	}
	else if ((Ending_score_item == 100) && (Ending_rank_no < 2)) {
		Ending_item_a_col = 0x808080;
		Ending_state_flg |= 0x90;
	}
	ret_val = Ending_item_ck(18);
	if (ret_val != 0) {
		Ending_item_c_col = 0;
		Ending_state_flg |= 0x40;
	}
	else if ((Ending_score_life == 100) && (Ending_rank_no < 2)) {
		Ending_item_c_col = 0x808080;
		Ending_state_flg |= 0xc0;
	}
	flg_res = Flg_ck(&Ending_flg_tbl, 0x13);
	if (flg_res == 0) {
		if (Ending_rank_no == 0) {
			Flg_on(&Ending_flg_tbl, 0x13);
			Ending_state_flg |= 0x100;
		}
	}
	else {
		flg_res = Flg_ck(&Ending_flg_tbl, 0x14);
		if ((flg_res == 0) && (1 < Ending_save_zapping_cnt)) {
			Flg_on(&Ending_flg_tbl, 0x14);
			Ending_state_flg |= 0x200;
		}
	}
	flg_res = Flg_ck((ULONG *)&Ending_flg_ck_tbl, 0x9c);
	if ((flg_res == 0) && (flg_res = Flg_ck(&Ending_flg_tbl, 0x12), flg_res != 0)) {
		Ending_state_flg |= 0x400;
	}
	flg_res = Flg_ck(&Ending_flg_tbl, 0x15);
	if (flg_res == 0) {
		Flg_on(&Ending_flg_tbl, 0x15);
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
				if ((slot_id == Item_id) &&
				   (((slot_id != 17 && (slot_id != 15)) || (*(char *)(ofs + -0x7feda983) == 0xff))))
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
	G.SaveData.Save_area = save_area[(UINT)G.Dummy8[1]];
	G.SaveData.Vol_Bgm = Volume_Bgm;
	G.SaveData.Pl_poison_down = 0;
	G.Pl_poison_down = 0;
	G.SaveData.Pl_poison_timer = 0;
	G.Pl_poison_timer = 0;
	G.SaveData.Vol_Se = Volume_Se;
	G.SaveData.Key_idx = Main.Key_idx;
	G.SaveData.Dummy16 = Ending_save_bonus_in;
	G.Status_flg &= (STAT_PLAYER | STAT_SCENARIO_B | STAT_EASY_MODE);
	G.SaveData.Status_bak = G.Status_flg;
	if ((G.System_flg & SYS_TIMER) != 0) {
		Flg_on(&G.Status_flg, 6);
		Flg_on(&G.SaveData.Status_bak, 6);
	}
	if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) {
		Flg_on(&G.Status_flg, 6);
		Flg_on(&G.SaveData.Status_bak, 6);
	}
	if (!flg4000_off) {
		Flg_on(&G.Status_flg, 0x11);
		Flg_on(&G.SaveData.Status_bak, 0x11);
	}
	G.SaveData.Cure_cnt = 0;
	G.SaveData.Save_cnt = 0xffff;
	if (((G.System_flg & SYS_TIMER) == 0) && ((G.Status_flg & STAT_EX_SAVE) == 0)) {
		G.SaveData.Zonbi_cnt = Ending_zombie_cnt;
		if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			G.SaveData.Front_play_time = 0;
			G.SaveData.Dummy1[0] = Ending_save_zapping_cnt + 1;
			flg_res = Flg_ck(&Ending_flg_tbl, 0x14);
			if (flg_res != 0) {
				Flg_on(G.SaveData.Zapping_flg, 0x14);
			}
			flg_res = Flg_ck(&Ending_flg_tbl, 0x13);
			if (flg_res != 0) {
				Flg_on(G.SaveData.Zapping_flg, 0x13);
			}
		}
		else {
			G.SaveData.Front_play_time = 0;
			G.SaveData.Dummy1[0] = Ending_save_zapping_cnt;
			G.SaveData.Zapping_flg[0] = Ending_flg_tbl;
			G.SaveData.Zapping_flg[1] = Ending_flg_tbl_b1;
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

	pInit_buf = (ULONG *)&Ending_inittbl_buf;
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

	read_ret = Cd_read((UINT)*(Ending_tim_sect_tbl + (Tim_no & 0xff)),
					Bg_img_buf, 0, Ending_mess_end_tim);
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
	short wait_cnt;

	if (Ending_timeline_cnt == 0) {
		Ending_stat_rec_ptr = &Ending_stat_rec_tbl;
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
					(&Ending_msg_line_buf1)[Ending_msg_col] = 0xfe;
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
					if ((UCHAR)(&Ending_msg_char_cnt_tbl)[Ending_msg_row_cnt] < Ending_type_cnt) {
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
					if ((Ending_scroll_step == 0) &&
					   (Ending_scroll_step = 0x80, Ending_exit_req == 0)) {
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
							Mess_print(0x3c, Ending_msg_draw_idx * 0xd + 0x20,
									   Ending_msg_row_tbl[Ending_msg_draw_idx + Ending_msg_col_base], 0x4002);
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
	UCHAR glyph_w;
	UCHAR glyph_h;
	char glyph_u;
	char glyph_uw;
	char glyph_v;
	char glyph_vh;
	char uv_tmp;
	UCHAR tpage_b;
	USHORT x_pos;
	USHORT y_pos;
	USHORT clut_val;
	int tbl_ofs;
	char *pBuf;
	UCHAR *pRec;
	int rem_cnt;
	UCHAR *pFlg;
	POLY_FT4 *pPrim;

	rem_cnt = 0xc;
	pPrim = (POLY_FT4 *)((UINT)G.Cbuf_id * 8000 + -0x7fedebf8);
	pRec = Ending_stat_rec_ptr + 6;
	pBuf = &Ending_cbuf + (UINT)G.Cbuf_id * 8000;
	pFlg = Ending_stat_rec_ptr;
	do {
		rem_cnt = rem_cnt + -1;
		if ((*pFlg & 1) != 0) {
			x_pos = *(USHORT *)(pRec + -2);
			y_pos = *(USHORT *)pRec;
			tbl_ofs = (UINT)pRec[-3] * 10;
			glyph_w = (&Ending_stat_glyph_W)[tbl_ofs];
			glyph_h = (&Ending_stat_glyph_H)[tbl_ofs];
			glyph_u = (&Ending_stat_glyph_U)[tbl_ofs];
			glyph_uw = (&Ending_stat_glyph_Uw)[tbl_ofs];
			glyph_v = (&Ending_stat_glyph_V)[tbl_ofs];
			glyph_vh = (&Ending_stat_glyph_Vh)[tbl_ofs];
			SetPolyFT4(pPrim);
			pBuf[-0x21] = -0x80;
			pBuf[-0x20] = -0x80;
			pBuf[-0x1f] = -0x80;
			pBuf[-0x1e] = 46;
			*(short *)(pBuf + -0x1d) = x_pos;
			*(short *)(pBuf + -0x1b) = y_pos;
			pBuf[-0x19] = (&Ending_stat_glyph_U)[tbl_ofs];
			uv_tmp = (&Ending_stat_glyph_V)[tbl_ofs];
			*(USHORT *)(pBuf + -0x15) = x_pos + (USHORT)glyph_w;
			*(short *)(pBuf + -0x13) = y_pos;
			pBuf[-0x11] = glyph_u + glyph_uw;
			pBuf[-0x18] = uv_tmp;
			pBuf[-0x10] = (&Ending_stat_glyph_V)[tbl_ofs];
			clut_val = GetClut((UINT)(UCHAR)(&Ending_stat_glyph_Clut_x)[tbl_ofs] << 4,
							 (UINT)*(USHORT *)(&Ending_stat_glyph_Clut_y + tbl_ofs));
			*(USHORT *)(pBuf + -0x17) = clut_val;
			tpage_b = (&Ending_stat_glyph_Tbl)[tbl_ofs];
			*(short *)(pBuf + -0xd) = x_pos;
			*(USHORT *)(pBuf + -0xb) = y_pos + (USHORT)glyph_h;
			*(USHORT *)(pBuf + -0xf) = tpage_b & 0xf;
			uv_tmp = (&Ending_stat_glyph_U)[tbl_ofs];
			pBuf[-8] = glyph_v + glyph_vh;
			*(USHORT *)(pBuf + -5) = x_pos + (USHORT)glyph_w;
			*(USHORT *)(pBuf + -3) = y_pos + (USHORT)glyph_h;
			pBuf[-1] = glyph_u + glyph_uw;
			*pBuf = glyph_v + glyph_vh;
			pBuf[-9] = uv_tmp;
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
 * NOT YET RECONSTRUCTED (no Ghidra function is defined over this range,
 * so disassemble_function cannot read it; the Ghidra tools available here
 * cannot create functions):
 *  - ENDING::8011a0e0 .. 8011a18c : overlay entry (no Ghidra function)
 *  - Routine table ENDING::8011de34 .. 8011de40: 8011de34 = Ending_fade_out,
 *      8011de38 = Ending_fade_unk (ENDING::8011a8e4), 8011de3c = Ending_fade_in,
 *      8011de40 = Ending_exit.
 *  - Overlay and DS globals are declared as extern at the top of this file,
 *      with their ENDING:: / DS addresses. Types are inferred from use.
 */
