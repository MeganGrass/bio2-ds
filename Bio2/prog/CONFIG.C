/*
 * D:\Bio2\prog\CONFIG.C
 *
 * Options / configuration screen overlay (SLUS_00748).
 * Overlay range: CONFIG::801bfa18 -- CONFIG::801c6683.
 *
 * Functions are in retail (DS) address order.  Every function body is taken
 * from the Ghidra decompilation / disassembly of the overlay.  Addresses in
 * comments are retail DS addresses.
 */

#include "TYPES.H"
#include "ROOM.H"
#include <ASM.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include <STDLIB.H>
#include <STRINGS.H>
#include "CONFIG.H"
#include "PAD.H"
#include "SND.H"
#include "MAIN.H"
#include "CD.H"
#include "ESP_R.H"
#include "MONITOR.H"
#include "SCE1.H"
#include "SELECT.H"
#include "ST_SUB00.H"
#include "DOOR.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCE2.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x801c3ce8 (Cnf_eclt) */
unsigned short *Cnf_eclt = (unsigned short *)0x801c3ce8;

/* DS 0x801c40f8 (Cnf_Oprim) */
POLY_FT4 (*Cnf_Oprim)[120] = (POLY_FT4 (*)[120])0x801c40f8;

/* ---------------------------------------------------------------------------
 * Macros
 * ---------------------------------------------------------------------------*/
#define CNF_SE(id)       Snd_se_stad((id), (VECTOR *)0)

/* Second CLUT row of CONFIG.TIM: Config_init loads it from DS 0x801a8220, the bytes that follow the TIM's
   128x256 image in Bg_img_buf (TIM header 8 + CLUT block 0x20c + image header 12 + image 0x10000 = 0x10220). */
#define CNF_PAL_BUF      (&Bg_img_buf[0x10220 / 4])

/* ---------------------------------------------------------------------------
 * Overlay data (values from the retail CONFIG.BIN, loaded at DS 0x801bfa18)
 * ---------------------------------------------------------------------------*/

/* DS 0x801c3798 (Cnf_st) - first sprite (Cnf_char index) and sprite count of each option row */
CNF_ST         Cnf_st[CNF_ROW_COUNT] = {
	{ 0x00, 14 },	/* row 0 */
	{ 0x0e, 25 },	/* row 1 */
	{ 0x27,  5 },	/* row 2 */
	{ 0x2c, 16 },	/* row 3 */
	{ 0x3c,  2 },	/* row 4 */
	{ 0x3e,  6 },	/* row 5 */
};

/* DS 0x801c37a4 (Cnf_uv) - glyph u, v, w, h in the CONFIG TIM page; 72 glyphs (Mojiretu ids 0x00-0x47) */
unsigned char  Cnf_uv[72][4] = {
	{ 0x00, 0x00, 13, 14 },	/*  0 */
	{ 0x0f, 0x00, 13, 14 },	/*  1 */
	{ 0x1d, 0x00, 13, 14 },	/*  2 */
	{ 0x2b, 0x00, 13, 14 },	/*  3 */
	{ 0x3a, 0x00, 11, 14 },	/*  4 */
	{ 0x47, 0x00, 11, 14 },	/*  5 */
	{ 0x54, 0x00, 14, 14 },	/*  6 */
	{ 0xe1, 0x0e, 12, 14 },	/*  7 */
	{ 0x66, 0x00,  6, 14 },	/*  8 */
	{ 0x70, 0x00, 13, 14 },	/*  9 */
	{ 0x7f, 0x00, 11, 14 },	/* 10 */
	{ 0x8c, 0x00, 14, 14 },	/* 11 */
	{ 0x9a, 0x00, 13, 14 },	/* 12 */
	{ 0xa8, 0x00, 14, 14 },	/* 13 */
	{ 0xb7, 0x00, 11, 14 },	/* 14 */
	{ 0xc5, 0x00, 13, 14 },	/* 15 */
	{ 0xd4, 0x00, 10, 14 },	/* 16 */
	{ 0xe1, 0x00, 12, 14 },	/* 17 */
	{ 0xee, 0x00, 13, 14 },	/* 18 */
	{ 0x00, 0x0e, 14, 14 },	/* 19 */
	{ 0x0e, 0x0e, 14, 14 },	/* 20 */
	{ 0x1c, 0x0e, 14, 14 },	/* 21 */
	{ 0x2a, 0x0e, 14, 14 },	/* 22 */
	{ 0x38, 0x0e, 14, 14 },	/* 23 */
	{ 0x46, 0x0e, 14, 14 },	/* 24 */
	{ 0x54, 0x0e, 14, 14 },	/* 25 */
	{ 0x62, 0x0e, 14, 14 },	/* 26 */
	{ 0x70, 0x0e, 14, 14 },	/* 27 */
	{ 0x7e, 0x0e, 14, 14 },	/* 28 */
	{ 0x8c, 0x0e, 14, 14 },	/* 29 */
	{ 0x9a, 0x0e, 14, 14 },	/* 30 */
	{ 0xa8, 0x0e, 14, 14 },	/* 31 */
	{ 0xb6, 0x0e, 14, 14 },	/* 32 */
	{ 0xc6, 0x0e,  8, 14 },	/* 33 */
	{ 0xf0, 0x0e, 10, 14 },	/* 34 */
	{ 0x20, 0x30, 14, 14 },	/* 35 */
	{ 0xf0, 0x0e, 10, 14 },	/* 36 */
	{ 0xd3, 0x0f, 11, 14 },	/* 37 */
	{ 0xa4, 0x30, 90, 14 },	/* 38 */
	{ 0x28, 0x20, 72, 16 },	/* 39 */
	{ 0x37, 0x30, 41, 16 },	/* 40 */
	{ 0x70, 0x20, 72, 16 },	/* 41 */
	{ 0x70, 0x30, 72, 16 },	/* 42 */
	{ 0x00, 0x20, 14, 14 },	/* 43 */
	{ 0x0e, 0x20, 14, 14 },	/* 44 */
	{ 0x00, 0x30, 14, 14 },	/* 45 */
	{ 0x0e, 0x30, 14, 14 },	/* 46 */
	{ 0xb8, 0x20, 71, 16 },	/* 47 */
	{ 0xb8, 0x30, 71, 16 },	/* 48 */
	{ 0x94, 0x20,  2,  5 },	/* 49 */
	{ 0x94, 0x20,  2,  1 },	/* 50 */
	{ 0x94, 0x20,  2,  3 },	/* 51 */
	{ 0x1c, 0x38, 64,  8 },	/* 52 */
	{ 0x1c, 0x38, 42,  8 },	/* 53 */
	{ 0x1c, 0x38, 20,  8 },	/* 54 */
	{ 0x60, 0x30, 13, 14 },	/* 55 */
	{ 0x00, 0x40, 64, 48 },	/* 56 */
	{ 0x00, 0x70, 64, 48 },	/* 57 */
	{ 0x00, 0xa0, 64, 48 },	/* 58 */
	{ 0x40, 0x40, 64, 48 },	/* 59 */
	{ 0x40, 0x70, 64, 48 },	/* 60 */
	{ 0x40, 0xa0, 64, 48 },	/* 61 */
	{ 0x80, 0x40, 64, 48 },	/* 62 */
	{ 0x80, 0x70, 64, 48 },	/* 63 */
	{ 0x80, 0xa0, 62, 46 },	/* 64 */
	{ 0x00, 0xd0, 62, 46 },	/* 65 */
	{ 0xc0, 0x70, 62, 46 },	/* 66 */
	{ 0x40, 0xd0, 62, 46 },	/* 67 */
	{ 0xc0, 0xa0, 62, 46 },	/* 68 */
	{ 0x80, 0xd0, 62, 46 },	/* 69 */
	{ 0xc0, 0x40, 62, 46 },	/* 70 */
	{ 0xc0, 0xd0, 62, 46 }	/* 71 */
};

/* DS 0x801c38c4 (Cnf_char) - flags, row, otz, glyph sequence, x, y; the Config_main_trans loop ends at
   0x801c3ba4 (92 x 8 bytes). Rows 6-8 are the background tiles and the two scrolling marquee rows. */
CNF_CHAR       Cnf_char[CNF_CHAR_COUNT] = {
	{ 0x27, 0, 10, 0x11,   37,  77 },	/*  0 */
	{ 0x05, 0, 10, 0x37,   37,  95 },	/*  1 */
	{ 0x05, 0, 10, 0x12,   37, 113 },	/*  2 */
	{ 0x05, 0, 10, 0x13,   37, 131 },	/*  3 */
	{ 0x05, 0, 10, 0x14,   37, 149 },	/*  4 */
	{ 0x05, 0, 10, 0x15,   37, 174 },	/*  5 */
	{ 0x40, 0, 10, 0x1a,  206,  77 },	/*  6 */
	{ 0x40, 0, 10, 0x1b,  206,  77 },	/*  7 */
	{ 0x40, 0, 10, 0x1c,  206,  77 },	/*  8 */
	{ 0x40, 0, 10, 0x16,  210, 113 },	/*  9 */
	{ 0x40, 0, 10, 0x17,  176, 113 },	/* 10 */
	{ 0x40, 0, 10, 0x38,  234,  95 },	/* 11 */
	{ 0x40, 0, 10, 0x1e,  226,  95 },	/* 12 */
	{ 0x01, 0, 10, 0x10,  125,  51 },	/* 13 */
	{ 0x40, 1, 10, 0x1a,  195,  20 },	/* 14 */
	{ 0x40, 1, 10, 0x1b,  195,  20 },	/* 15 */
	{ 0x40, 1, 10, 0x1c,  195,  20 },	/* 16 */
	{ 0x05, 1, 10, 0x15,  209,  37 },	/* 17 */
	{ 0x00, 1, 10, 0x28,  181,  20 },	/* 18 */
	{ 0x01, 1, 11, 0x29,  181,  20 },	/* 19 */
	{ 0x00, 1, 10, 0x2a,  270,  20 },	/* 20 */
	{ 0x01, 1, 11, 0x2b,  270,  20 },	/* 21 */
	{ 0x00, 1, 10, 0x2d,   31,  89 },	/* 22 */
	{ 0x00, 1, 10, 0x27,  239, 137 },	/* 23 */
	{ 0x00, 1, 10, 0x26,  202, 157 },	/* 24 */
	{ 0x00, 1, 10, 0x24,  202, 117 },	/* 25 */
	{ 0x00, 1, 10, 0x25,  181, 137 },	/* 26 */
	{ 0x00, 1, 10, 0x2d,   31,  89 },	/* 27 */
	{ 0x00, 1, 10, 0x27,  239, 137 },	/* 28 */
	{ 0x00, 1, 10, 0x26,  202, 157 },	/* 29 */
	{ 0x00, 1, 10, 0x24,  202, 117 },	/* 30 */
	{ 0x00, 1, 10, 0x2c,  164, 137 },	/* 31 */
	{ 0x00, 1, 10, 0x30,   50,  91 },	/* 32 */
	{ 0x00, 1, 10, 0x27,  239, 137 },	/* 33 */
	{ 0x00, 1, 10, 0x26,  202, 157 },	/* 34 */
	{ 0x00, 1, 10, 0x24,  202, 117 },	/* 35 */
	{ 0x00, 1, 10, 0x25,  181, 137 },	/* 36 */
	{ 0x00, 1, 10, 0x2f,  123,  91 },	/* 37 */
	{ 0x00, 1, 10, 0x2e,  136,  91 },	/* 38 */
	{ 0x05, 2, 10, 0x38,  111, 110 },	/* 39 */
	{ 0x05, 2, 10, 0x1e,  180, 110 },	/* 40 */
	{ 0x05, 2, 10, 0x15,  138, 167 },	/* 41 */
	{ 0x01, 2, 10, 0x21,  154, 110 },	/* 42 */
	{ 0x01, 2, 10, 0x37,  108,  51 },	/* 43 */
	{ 0x05, 3, 10, 0x16,   37,  83 },	/* 44 */
	{ 0x05, 3, 10, 0x18,   37, 108 },	/* 45 */
	{ 0x05, 3, 10, 0x19,   37, 131 },	/* 46 */
	{ 0x05, 3, 10, 0x15,   37, 167 },	/* 47 */
	{ 0x05, 3, 10, 0x17,  176,  83 },	/* 48 */
	{ 0x00, 3, 10, 0x1e,  209, 108 },	/* 49 */
	{ 0x00, 3, 10, 0x1e,  209, 131 },	/* 50 */
	{ 0x00, 3, 10, 0x28,  192, 108 },	/* 51 */
	{ 0x01, 3, 11, 0x29,  192, 108 },	/* 52 */
	{ 0x00, 3, 10, 0x2a,  248, 108 },	/* 53 */
	{ 0x01, 3, 11, 0x2b,  248, 108 },	/* 54 */
	{ 0x00, 3, 10, 0x28,  192, 131 },	/* 55 */
	{ 0x01, 3, 11, 0x29,  192, 131 },	/* 56 */
	{ 0x00, 3, 10, 0x2a,  248, 131 },	/* 57 */
	{ 0x01, 3, 11, 0x2b,  248, 131 },	/* 58 */
	{ 0x01, 3, 10, 0x12,  128,  51 },	/* 59 */
	{ 0x05, 4, 10, 0x15,  250, 202 },	/* 60 */
	{ 0x01, 4, 10, 0x13,   74,  38 },	/* 61 */
	{ 0x05, 5, 10, 0x22,  180, 122 },	/* 62 */
	{ 0x05, 5, 10, 0x20,  111, 122 },	/* 63 */
	{ 0x05, 5, 10, 0x15,  138, 167 },	/* 64 */
	{ 0x01, 5, 10, 0x36,   72,  94 },	/* 65 */
	{ 0x01, 5, 10, 0x21,  154, 122 },	/* 66 */
	{ 0x01, 5, 10, 0x14,  131,  51 },	/* 67 */
	{ 0x01, 6, 20, 0x00,   32,  72 },	/* 68 */
	{ 0x01, 6, 20, 0x01,   32, 120 },	/* 69 */
	{ 0x01, 6, 20, 0x02,   96,  72 },	/* 70 */
	{ 0x01, 6, 20, 0x03,   96, 120 },	/* 71 */
	{ 0x01, 6, 20, 0x04,  160,  72 },	/* 72 */
	{ 0x01, 6, 20, 0x05,  160, 120 },	/* 73 */
	{ 0x01, 6, 20, 0x06,  224,  72 },	/* 74 */
	{ 0x01, 6, 20, 0x07,  224, 120 },	/* 75 */
	{ 0x01, 7, 20, 0x08, -120,   0 },	/* 76 */
	{ 0x01, 7, 20, 0x09,  -60,   0 },	/* 77 */
	{ 0x01, 7, 20, 0x0a,    0,   0 },	/* 78 */
	{ 0x01, 7, 20, 0x0b,   60,   0 },	/* 79 */
	{ 0x01, 7, 20, 0x08,  120,   0 },	/* 80 */
	{ 0x01, 7, 20, 0x09,  180,   0 },	/* 81 */
	{ 0x01, 7, 20, 0x0a,  240,   0 },	/* 82 */
	{ 0x01, 7, 20, 0x0b,  300,   0 },	/* 83 */
	{ 0x01, 8, 20, 0x0c, -120, 191 },	/* 84 */
	{ 0x01, 8, 20, 0x0d,  -60, 191 },	/* 85 */
	{ 0x01, 8, 20, 0x0e,    0, 191 },	/* 86 */
	{ 0x01, 8, 20, 0x0f,   60, 191 },	/* 87 */
	{ 0x01, 8, 20, 0x0c,  120, 191 },	/* 88 */
	{ 0x01, 8, 20, 0x0d,  180, 191 },	/* 89 */
	{ 0x01, 8, 20, 0x0e,  240, 191 },	/* 90 */
	{ 0x01, 8, 20, 0x0f,  300, 191 }	/* 91 */
};

/* DS 0x801c3ba4 (Mojiretu) - glyph sequences (Cnf_uv ids), each ended by 0xff; 235 bytes up to the last
   terminator at 0x801c3c8e. One sequence per line. */
unsigned char  Mojiretu[235] = {
	0x38, 0xff,
	0x39, 0xff,
	0x3a, 0xff,
	0x3b, 0xff,
	0x3c, 0xff,
	0x3d, 0xff,
	0x3e, 0xff,
	0x3f, 0xff,
	0x40, 0xff,
	0x41, 0xff,
	0x42, 0xff,
	0x43, 0xff,
	0x44, 0xff,
	0x45, 0xff,
	0x46, 0xff,
	0x47, 0xff,
	0x0d, 0x0e, 0x11, 0x08, 0x0d, 0x0c, 0xff,
	0x09, 0x04, 0x16, 0x37, 0x02, 0x0d, 0x0c, 0x05, 0x08, 0x06, 0x21, 0xff,
	0x10, 0x0d, 0x12, 0x0c, 0x03, 0xff,
	0x0b, 0x0d, 0x0c, 0x08, 0x11, 0x0d, 0x0f, 0x37, 0x00, 0x03, 0x22, 0x12, 0x10, 0x11, 0xff,
	0x0f, 0x04, 0x10, 0x04, 0x11, 0xff,
	0x04, 0x15, 0x08, 0x11, 0xff,
	0x10, 0x11, 0x04, 0x0f, 0x04, 0x0d, 0xff,
	0x0b, 0x0d, 0x0c, 0x00, 0x12, 0x0f, 0x00, 0x0a, 0xff,
	0x01, 0x06, 0x0b, 0x21, 0x13, 0x0d, 0x0a, 0xff,
	0x10, 0x04, 0x21, 0x13, 0x0d, 0x0a, 0xff,
	0x11, 0x16, 0x0e, 0x04, 0x37, 0x00, 0xff,
	0x11, 0x16, 0x0e, 0x04, 0x37, 0x01, 0xff,
	0x11, 0x16, 0x0e, 0x04, 0x37, 0x02, 0xff,
	0x0d, 0x09, 0xff,
	0x0d, 0x05, 0x05, 0xff,
	0x02, 0x00, 0x0c, 0x02, 0x04, 0x0a, 0xff,
	0x16, 0x04, 0x10, 0xff,
	0x23, 0xff,
	0x0c, 0x0d, 0xff,
	0x06, 0xff,
	0x27, 0xff,
	0x28, 0xff,
	0x29, 0xff,
	0x2a, 0xff,
	0x2b, 0xff,
	0x2c, 0xff,
	0x2d, 0xff,
	0x2e, 0xff,
	0x2f, 0xff,
	0x30, 0xff,
	0x00, 0x12, 0x11, 0x0d, 0xff,
	0x0b, 0x00, 0x0c, 0x12, 0x00, 0x0a, 0xff,
	0x00, 0x08, 0x0b, 0xff,
	0x34, 0xff,
	0x35, 0xff,
	0x36, 0xff,
	0x26, 0xff,
	0x07, 0xff,
	0x0f, 0x04, 0x10, 0x04, 0x11, 0x37, 0x11, 0x07, 0x04, 0x37, 0x06, 0x00, 0x0b, 0x04, 0x25, 0xff,
	0x13, 0x08, 0x01, 0x0f, 0x00, 0x11, 0x08, 0x0d, 0x0c, 0xff,
	0x0d, 0x0c, 0xff
};

/* DS 0x801c3c90 (Botan_offset) - key-config sprite visibility bits, one word per Key_idx (Config_move tests bits 0-16) */
unsigned long  Botan_offset[3] = { 0xfffe801f, 0xfffe83e0, 0xffff7800 };

/* DS 0x801c3c9c (Config_tblj) - state jump table, indexed by Cnf_status */
void (*Config_tblj[6])(void) = {
	Config_fade_in_wait,    /* 0 */
	Config_main,            /* 1 */
	Config_fade_out_set,    /* 2 */
	Config_fade_out_wait,   /* 3 */
	Config_exit,            /* 4 */
	Config_lrgb             /* 5 */
};

/* DS 0x801c3cb4 (Config_padd) - row jump table, indexed by Cnf_r_no */
void (*Config_padd[6])(void) = {
	Config_padd_opt,        /* 0 */
	Config_padd_con,        /* 1 */
	Config_padd_vib,        /* 2 */
	Config_padd_snd,        /* 3 */
	Config_padd_clr,        /* 4 */
	Config_padd_res         /* 5 */
};

/* DS 0x801c3cd0 (Cnf_mode_naw) */
unsigned char  Cnf_mode_naw;
/* DS 0x801c3cd1 (Bak_cut) */
unsigned char  Bak_cut;
/* DS 0x801c3cd2 (Cnf_cd2) */
unsigned char  Cnf_cd2;
/* DS 0x801c3cd3 (Cnf_cd3) */
unsigned char  Cnf_cd3;
/* DS 0x801c3cd4 (Cnf_cd4) */
unsigned char  Cnf_cd4;
/* DS 0x801c3cd5 (Cnf_cd5) */
unsigned char  Cnf_cd5;
/* DS 0x801c3cd8 (Cnf_cur) */
unsigned char  Cnf_cur[6];
/* DS 0x801c3cde (Cnf_r_no) */
unsigned char  Cnf_r_no;
/* DS 0x801c3cdf (Cnf_cdf) */
unsigned char  Cnf_cdf;
/* DS 0x801c3ce0 (Cnf_scrl_bak) */
unsigned char  Cnf_scrl_bak;
/* DS 0x801c3ce1 (Cnf_ce1) */
unsigned char  Cnf_ce1;
/* DS 0x801c40e8 (Cnf_status) */
unsigned char  Cnf_status;
/* DS 0x801c40e9 (Cnf_pad_mode) */
unsigned char  Cnf_pad_mode;
/* DS 0x801c40ea (Cnf_mode_next) */
unsigned char  Cnf_mode_next;
/* DS 0x801c40ec (B_rgb) */
unsigned long  B_rgb;
/* DS 0x801c40f0 (pBak_light) */
void          *pBak_light;
/* DS 0x801c40f4 (Cnf_err) - load error bits; retail reads and writes it with lbu / sb */
unsigned char  Cnf_err;
/* DS 0x801c6678 (Suuji) */
unsigned char  Suuji[6];
/* DS 0x801c6680 (pOprim) */
void          *pOprim;

/* ===========================================================================
 * Config  (DS 801bfa84)
 * Overlay task entry: init once, then loop dispatching the current state.
 * ===========================================================================*/
/* DS 0x801bfa84 (Config) */
void Config(void)
{
	Config_init();
	do {
		pOprim = Cnf_Oprim[G.Cbuf_id];
		Config_tblj[Cnf_status]();
		Config_move();
		Config_main_trans();
		Task_sleep(1);
	} while (1);
}

/* ===========================================================================
 * Config_main  (DS 801bfb28)
 * State 1: per-frame option input (dispatches to the padd handler).
 * ===========================================================================*/
/* DS 0x801bfb28 (Config_main) */
void Config_main(void)
{
	Config_padd_main();
}

/* ===========================================================================
 * Config_fade_in_wait  (DS 801bfb48)
 * State 0: wait for the fade-in to finish, then go to state 1.
 * ===========================================================================*/
/* DS 0x801bfb48 (Config_fade_in_wait) */
void Config_fade_in_wait(void)
{
	/* retail reserves 8 more bytes of locals (Config_fade_in_wait frame 32); never read */
	volatile int Rsv[2];
	unsigned long fade_st;
	/* retail reserves sp+0x10..0x17, never read */

	fade_st = Fade_status(0);
	if (fade_st != 0) {
		Fade_set(0x200, 0, 7, 1);
		Cnf_status   = 1;
		Cnf_mode_naw &= 0xf7;
	}
}

/* ===========================================================================
 * Config_fade_out_set  (DS 801bfba0)
 * State 2: start the fade-out (partial-screen rect for option screens).
 * ===========================================================================*/
/* DS 0x801bfba0 (Config_fade_out_set) */
void Config_fade_out_set(void)
{
	RECT  rect;
	RECT *pRect;

	if ((Cnf_mode_naw & 8) == 0) {
		Fade_set(0x200, 0x1800, 7, 1);
		pRect = NULL;
	} else {
		Fade_set(0x200, 0xc00, 7, 1);
		if (Cnf_mode_next != 4 && Cnf_r_no != 4 && Cnf_mode_next != 1 && Cnf_r_no != 1) {
			rect.x = 0;
			rect.y = 0x30;
			rect.w = 0x140;
			rect.h = 0x8f;
			pRect  = &rect;
		} else {
			pRect = NULL;
		}
	}
	Fade_adjust(0, 0, 0xffffff, pRect);
	if (Cnf_cd2 != 0) {
		Cnf_status = 3;
	}
	Cnf_cd2 = (Cnf_cd2 == 0);
}

/* ===========================================================================
 * Config_fade_out_wait  (DS 801bfca4)
 * State 3: wait for the fade-out, apply the chosen option and return.
 * ===========================================================================*/
/* DS 0x801bfca4 (Config_fade_out_wait) */
void Config_fade_out_wait(void)
{
	RECT rect;

	if (Fade_status(0) == 0) {
		return;
	}

	CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;

	if ((Cnf_mode_naw & 8) == 0) {
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x7fff, 0, NULL);
		Cnf_status = 4;
		return;
	}

	Fade_set(0x200, -0xc00, 7, 1);

	/* Sub-screen case: stay on the same option screen, just return to state 0. */
	if (Cnf_mode_next != 1 && Cnf_r_no != 1 && Cnf_mode_next != 4) {
		if (Cnf_r_no != 4) {
			rect.x = 0;
			rect.y = 0x30;
			rect.w = 0x140;
			rect.h = 0x8f;
			Fade_adjust(0, 0x7fff, 0xffffff, &rect);
			Cnf_r_no   = Cnf_mode_next;
			Cnf_status = 0;
			return;
		}
	}

	if (Cnf_mode_next != 4 && Cnf_mode_next != 1) {
		/* Back to the option list with the mode bit cleared. */
		Bg_set_mode(2, 0);
		Cnf_status   = 0;
		Cnf_mode_naw &= 0xfe;
	} else {
		/* Leave to the colour-TIM reload (state 5). */
		Bg_set_mode(2, 0);
		Cnf_mode_naw |= 1;
		Bg_set_mode(0, 0);
		if (Cnf_mode_next == 4) {
			Cnf_cd5 = 1;
		} else {
			if (Cnf_pad_mode == 7) {
				Cnf_cd5 = 2;
			}
			if (Cnf_pad_mode == 4) {
				Cnf_cd5 = 0;
			}
		}
		Cnf_status = 5;
	}

	Fade_adjust(0, 0x7fff, 0xffffff, NULL);
	Cnf_r_no = Cnf_mode_next;
}

/* ===========================================================================
 * Config_init  (DS 801bfedc)
 * Save game state, reset option state, load the CONFIG TIM, upload to VRAM.
 * ===========================================================================*/
/* DS 0x801bfedc (Config_init) */
void Config_init(void)
{
	unsigned char cbuf_save;
	unsigned long ret;
	int           i;
	int           open_err;
	TIM_IMAGE    *ptim;
	RECT          rect0;
	RECT          rect1;
	TIM_IMAGE     tim_image;

	Cnf_err = 0;
	Task_suspend(0);
	Bg_set_mode(2, 0);

	cbuf_save = G.Cbuf_id;
	G.Cbuf_id = 0;
	Fade_adjust(0, 0x7fff, 0, NULL);
	G.Cbuf_id = 1;
	Fade_adjust(0, 0x7fff, 0, NULL);

	Cnf_scrl_bak   = Main.Scrl_type;
	Main.Scrl_type = 0;
	G.Cbuf_id      = cbuf_save;

	Pad_rep_set(0xf00c, 0x40c);
	SetGeomScreen(0xd0);
	SetGeomOffset(0, 0);
	Main.Vloop     = 0;
	pBak_light     = G.pRoom->pLight;
	Cnf_status     = 0;
	Cnf_ce1        = 0;
	B_rgb          = 0x2c808080;
	Cnf_mode_naw   = 0;
	Cnf_cd5        = 0;
	Cnf_cdd        = 0;
	Bak_cut        = (unsigned char)G.SaveData.Cut_no;

	if ((G.Sound_mode & 0x20) != 0) {
		Cnf_mode_naw = 0x20;
	}

	Cnf_mode_next = 0;
	Cnf_r_no      = 0;
	for (i = 0; i < 5; i++) {
		Cnf_cur[i] = 0;
	}
	Cnf_cd2 = 0;
	Cnf_cdf = 0;
	Cnf_cd3 = 0;

	rect1.x = 0;
	rect1.y = 0x1fe;
	rect1.w = 0x100;
	rect1.h = 2;
	StoreImage(&rect1, (ULONG *)Cnf_eclt);
	DrawSync(0);

	ret = Cd_read(CONFIG_TIM, Bg_img_buf, 1, (unsigned char *)"CONFIG_TIM_LD");
	if (ret == 0xffffffff) {
		Cnf_err |= 1;
	}

	open_err = OpenTIM(Bg_img_buf);
	if (open_err != 0) {
		Cnf_err |= 2;
	}

	ptim = ReadTIM(&tim_image);
	if (ptim == NULL) {
		Cnf_err |= 4;
	}

	rect0.x = 0x380;
	rect0.y = 0x100;
	rect0.w = 0x80;
	rect0.h = 0x100;
	LoadImage(&rect0, ptim->paddr);
	DrawSync(0);

	rect1.h = 1;
	LoadImage(&rect1, ptim->caddr);
	DrawSync(0);

	rect1.y++;
	LoadImage(&rect1, CNF_PAL_BUF);
	DrawSync(0);

	while (Pad_type1 == 0) {
		Task_sleep(1);
	}

	Cnf_pad_mode = Pad_type1;
	Fade_set(0x200, -0x1800, 7, 1);
}

/* ===========================================================================
 * Config_exit  (DS 801c01ec)
 * State 4: reload the game TIM, restore the saved game state, signal and exit.
 * ===========================================================================*/
/* DS 0x801c01ec (Config_exit) */
void Config_exit(void)
{
	unsigned long ret;
	RECT          rect0;
	volatile UCHAR Rsv[24];
	/* retail reserves sp+0x18..0x2f, never read (rect0 at sp+0x10) */

	ret = Cd_read(CORE01_TIM, Bg_img_buf, 1, (unsigned char *)"CORE01_TIM_LD");
	Bg_set_mode(2, 0);

	if (ret == 0xffffffff) {
		Cnf_err |= 1;
	}

	rect0.x = 0x380;
	rect0.y = 0x100;
	rect0.w = 0x80;
	rect0.h = 0x100;
	LoadImage(&rect0, Bg_img_buf);
	DrawSync(0);

	rect0.x = 0;
	rect0.y = 0x1fe;
	rect0.w = 0x100;
	rect0.h = 2;
	LoadImage(&rect0, (ULONG *)Cnf_eclt);
	DrawSync(0);

	Main.Scrl_type = Cnf_scrl_bak;

	if ((Cnf_mode_naw & 0x10) != 0) {
		G.System_flg |= SYS_RESET;
	}

	/* retail stores G.Sound_mode after each of the two tests */
	if ((G.SaveData.Weapon_mugen & 1) == 0) {
		G.Sound_mode &= 0xbf;
	} else {
		G.Sound_mode |= 0x40;
	}

	if ((G.SaveData.Weapon_mugen & 0xffffdf00) == 0) {
		G.Sound_mode &= 0xdf;
	} else {
		G.Sound_mode |= 0x20;
	}

	if ((G.System_flg & SYS_CONFIG) == 0) {
		G.pRoom->pLight        = pBak_light;
		G.SaveData.Cut_no      = Bak_cut;
		G.Next_cut_no          = Bak_cut;
		Main.Vloop             = 2;
		SetGeomOffset(0xa0, 0x78);
		SetGeomScreen((unsigned int)(G.pRoom->pRcut[Bak_cut].ViewR >> 7));
		Main.Bg_chg_flg        = 2;
		Bg_set_mode(0, 0);
	}

	Task_signal(0);
	Task_exit();
}

/* ===========================================================================
 * Config_padd_main  (DS 801c0414)
 * State 2 / input dispatch: Triangle cancels, otherwise run the row handler.
 * ===========================================================================*/
/* DS 0x801c0414 (Config_padd_main) */
void Config_padd_main(void)
{
	if ((Cnf_mode_naw & 0xc) == 0) {
		if ((G.Padd1_trg & 0x100) == 0) {
			Config_padd[Cnf_r_no]();
		} else {
			CNF_SE(SOUND_CORE_CANCEL);
			Cnf_status = 2;
		}
	}
}

/* ===========================================================================
 * Config_padd_opt  (DS 801c0498)
 * Row 0: top-level option row.  Cursor moves between the five entries;
 * Circle / Cross (0x1000) opens the selected sub-option (Cnf_mode_next).
 * ===========================================================================*/
/* DS 0x801c0498 (Config_padd_opt) */
void Config_padd_opt(void)
{
	if ((G.Key_trg & 0x2000) != 0) {
		Cnf_status   = 2;
		Cnf_mode_naw |= 4;
		CNF_SE(SOUND_CORE_CANCEL);
	} else if ((G.Key_trg & 0x1000) != 0) {
		if (Cnf_cur[Cnf_r_no] != 5) {
			Cnf_status    = 2;
			Cnf_mode_naw |= 8;
			CNF_SE(SOUND_CORE_CONFIRM);
			Cnf_ce1       = 0;
			B_rgb         = 0x2c808080;
			Cnf_mode_next = Cnf_cur[Cnf_r_no] + 1;
			switch (Cnf_mode_next) {
			case 0:
			default:
				return;

			case 1:
				/* Key configuration sub-screen. */
				if ((G.SaveData.Weapon_mugen & 1) == 0) {
					Cnf_mode_naw &= 0xbf;
				} else {
					Cnf_mode_naw |= 0x40;
				}
				Cnf_cur[1] = G.SaveData.Key_idx;
				Cnf_cdd    = G.SaveData.Key_idx;
				CNF_SPR(1, 0).flags = 0;
				CNF_SPR(1, 1).flags = 0;
				CNF_SPR(1, 2).flags = 0;
				CNF_SPR(1, Cnf_cdd).flags = 5;
				while (Pad_type1 == 0) {
					Task_sleep(1);
				}
				Cnf_pad_mode = Pad_type1;
				return;

			case 2:
				/* Sound-type sub-option (stereo / mono). */
				if ((G.Sound_mode & 0x80) != 0) {
					CNF_SPR(2, 0).flags = 5;
					CNF_SPR(2, 1).flags = 1;
					Cnf_cur[Cnf_mode_next] = 1;
					Cnf_mode_naw |= 0x80;
				} else {
					CNF_SPR(2, 0).flags = 1;
					CNF_SPR(2, 1).flags = 5;
					Cnf_cur[Cnf_mode_next] = 0;
					Cnf_mode_naw &= 0x7f;
				}
				return;

			case 3:
				/* Volume sub-screen: show both volume values. */
				Cnf_cd3 = 1;
				Cnf_cd4 = 0;
				Config_suuji(0, Volume_Bgm, Cnf_mode_next);
				Config_suuji(3, Volume_Se,  Cnf_mode_next);
				Cnf_cur[Cnf_mode_next] = ((G.Sound_mode & 1) != 0) ? 4 : 0;
				return;

			case 4:
			case 5:
				Cnf_cur[Cnf_mode_next] = 0;
				return;
			}
		}
		Cnf_status   = 2;
		Cnf_mode_naw |= 4;
		CNF_SE(SOUND_CORE_CANCEL);
	} else {
		/* Cursor move (left / right) on the option list. */
		if ((G.System_flg & SYS_PAD_REPEAT) == 0) {
			return;
		}
		if ((G.Padd1 & 0x1000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			Cnf_cur[Cnf_r_no]--;
			if (Cnf_cur[Cnf_r_no] == 0xff) {
				Cnf_cur[Cnf_r_no] = 0;
			} else {
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}
		if ((G.Padd1 & 0x4000) == 0) {
			return;
		}
		Cnf_ce1 = 0;
		B_rgb   = 0x2c808080;
		CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		Cnf_cur[Cnf_r_no]++;
		if (Cnf_cur[Cnf_r_no] == 6) {
			Cnf_cur[Cnf_r_no] = 5;
			return;
		}
		CNF_SE(SOUND_CORE_CURSOR);
	}
}

/* ===========================================================================
 * Config_padd_con  (DS 801c0a34)
 * Row 1: key-config / analog-type selection and the cheat toggle.
 * ===========================================================================*/
/* DS 0x801c0a34 (Config_padd_con) */
void Config_padd_con(void)
{
	if ((G.System_flg & SYS_IN_GAME) != 0) {
		/* Held Select (0x8) with the cheat code button (0x80) counts up. */
		if ((G.Padd & 8) != 0 && G.Padd_trg == 0x80) {
			Cnf_cdf++;
			if (Cnf_cdf < 10) {
				return;
			}
			if ((Cnf_mode_naw & 0x20) != 0) {
				Cnf_mode_naw &= 0xdf;
				G.SaveData.Weapon_mugen &= 0x20ff;
			} else {
				Cnf_mode_naw |= 0x20;
				G.SaveData.Weapon_mugen |= 0xffffdf00;
				CNF_SE(SOUND_CORE_CONFIRM);
			}
			Cnf_cdf = 0;
			return;
		}
		if ((G.Padd & 8) == 0 || G.Padd_trg != 0) {
			Cnf_cdf = 0;
		}
	}

	/* Pad type changed underneath us (controller plugged / unplugged): reopen the key-config screen.
	   An unchanged, connected pad type is stored back (retail 801c0bc8). */
	if (Cnf_pad_mode == Pad_type1) {
		if (Pad_type1 != 0) {
			Cnf_pad_mode = Pad_type1;
		}
	} else if (Pad_type1 != 0) {
		Cnf_status    = 2;
		Cnf_mode_naw |= 8;
		CNF_SE(SOUND_CORE_CONFIRM);
		B_rgb         = 0x2c808080;
		Cnf_ce1       = 0;
		Cnf_mode_next = 1;
		Cnf_pad_mode  = Pad_type1;
		return;
	}

	if ((G.Key_trg & 0x2000) != 0) {
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		CNF_SE(SOUND_CORE_CANCEL);
		return;
	}

	if ((G.Key_trg & 0x1000) != 0) {
		/* Accept: store the selected key-config index and the stereo bit. */
		if (Cnf_cdd == 2) {
			G.SaveData.Weapon_mugen &= 0xfffffffe;
		} else {
			G.SaveData.Weapon_mugen |= 1;
		}
		Cnf_status    = 2;
		B_rgb         = 0x2c808080;
		Cnf_ce1       = 0;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		G.SaveData.Key_idx = Cnf_cdd;
		Main.Key_idx       = Cnf_cdd;
		CNF_SE(SOUND_CORE_CANCEL);
		return;
	}

	if ((G.System_flg & SYS_PAD_REPEAT) != 0) {
		if ((G.Padd1 & 0x1000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			if (Cnf_cur[Cnf_r_no] == 3) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
			CNF_SPR(Cnf_r_no, 0).flags = 0;
			CNF_SPR(Cnf_r_no, 1).flags = 0;
			CNF_SPR(Cnf_r_no, 2).flags = 0;
			CNF_SPR(Cnf_r_no, 3).flags = 5;
			Cnf_cur[Cnf_r_no] = Cnf_cdd;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		}
		if ((G.Padd1 & 0x4000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			if (Cnf_cur[Cnf_r_no] != 3) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
			CNF_SPR(Cnf_r_no, 0).flags = 0;
			CNF_SPR(Cnf_r_no, 1).flags = 0;
			CNF_SPR(Cnf_r_no, 2).flags = 0;
			CNF_SPR(Cnf_r_no, Cnf_cdd).flags = 5;
			Cnf_cur[Cnf_r_no] = 3;
			CNF_SPR(Cnf_r_no, 3).flags = 5;
		}
		if (((G.Padd1 & 0x8000) != 0) && (Cnf_cur[Cnf_r_no] != 3)) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SE(SOUND_CORE_CURSOR);
			Cnf_cdd--;
			if (Cnf_cdd == 0xff) {
				Cnf_cdd = 2;
			}
			Cnf_cur[Cnf_r_no] = Cnf_cdd;
			CNF_SPR(Cnf_r_no, 0).flags = 0;
			CNF_SPR(Cnf_r_no, 1).flags = 0;
			CNF_SPR(Cnf_r_no, 2).flags = 0;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		}
		if (((G.Padd1 & 0x2000) != 0) && (Cnf_cur[Cnf_r_no] != 3)) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SE(SOUND_CORE_CURSOR);
			Cnf_cdd++;
			if (Cnf_cdd == 3) {
				Cnf_cdd = 0;
			}
			Cnf_cur[Cnf_r_no] = Cnf_cdd;
			CNF_SPR(Cnf_r_no, 0).flags = 0;
			CNF_SPR(Cnf_r_no, 1).flags = 0;
			CNF_SPR(Cnf_r_no, 2).flags = 0;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		}
	}

	/* Arrow sprite updates (always run when the key block above is skipped). */
	CNF_SPR(Cnf_r_no, 4).flags = 0;
	CNF_SPR(Cnf_r_no, 5).flags = 1;
	CNF_SPR(Cnf_r_no, 6).flags = 0;
	CNF_SPR(Cnf_r_no, 7).flags = 1;
	if (((G.Padd1 & 0x8000) != 0) && (Cnf_cur[Cnf_r_no] != 3)) {
		CNF_SPR(Cnf_r_no, 4).flags = 1;
		CNF_SPR(Cnf_r_no, 5).flags = 0;
	}
	if (((G.Padd1 & 0x2000) != 0) && (Cnf_cur[Cnf_r_no] != 3)) {
		CNF_SPR(Cnf_r_no, 6).flags = 1;
		CNF_SPR(Cnf_r_no, 7).flags = 0;
	}
}

/* ===========================================================================
 * Config_padd_vib  (DS 801c14e4)
 * Row 2: vibration on/off (cur 0 / 1 / 2 = off / on / exit).
 * Retail-only: the Oct 97 mockup has no vibration row handler.
 * ===========================================================================*/
/* DS 0x801c14e4 (Config_padd_vib) */
void Config_padd_vib(void)
{
	unsigned char was;

	if ((G.Key_trg & 0x2000) != 0) {
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		CNF_SE(SOUND_CORE_CANCEL);
		return;
	}

	if ((G.Key_trg & 0x1000) != 0) {
		if ((Cnf_mode_naw & 0x80) == 0) {
			Pad_vib_set0(0xf, 0);
			Pad_vib_set1(0xf, 0xff, 0);
			G.Sound_mode &= 0x7f;
		} else {
			G.Sound_mode |= 0x80;
		}
		CNF_SE(SOUND_CORE_CONFIRM);
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		return;
	}

	if ((G.System_flg & SYS_PAD_REPEAT) == 0) {
		return;
	}

	if ((G.Padd1 & 0x1000) != 0) {
		Cnf_ce1 = 0;
		B_rgb   = 0x2c808080;
		if (Cnf_cur[Cnf_r_no] == 2) {
			CNF_SPR(Cnf_r_no, 2).flags = 5;
			if (Cnf_cur[Cnf_r_no] != 0) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
			if ((Cnf_mode_naw & 0x80) == 0) {
				Cnf_cur[Cnf_r_no] = 0;
			} else {
				Cnf_cur[Cnf_r_no] = 1;
			}
		}
	}
	if ((G.Padd1 & 0x4000) != 0) {
		Cnf_ce1 = 0;
		B_rgb   = 0x2c808080;
		CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		was = Cnf_cur[Cnf_r_no];
		Cnf_cur[Cnf_r_no] = 2;
		if (was != 2) {
			CNF_SE(SOUND_CORE_CURSOR);
		}
	}
	if (Cnf_cur[Cnf_r_no] != 2) {
		if ((G.Padd1 & 0x2000) != 0) {
			B_rgb   = 0x2c808080;
			Cnf_ce1 = 0;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			was = Cnf_cur[Cnf_r_no];
			Cnf_cur[Cnf_r_no] = 1;
			if (was != 1) {
				Cnf_mode_naw |= 0x80;
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}
		if ((G.Padd1 & 0x8000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			if (Cnf_cur[Cnf_r_no] != 0) {
				Cnf_mode_naw &= 0x7f;
				CNF_SE(SOUND_CORE_CURSOR);
			}
			Cnf_cur[Cnf_r_no] = 0;
		}
	}
}

/* ===========================================================================
 * Config_padd_snd  (DS 801c1954)
 * Row 3: BGM / SE volume sliders and the stereo toggle.
 * cur 0 = stereo entry, 1 = BGM, 2 = SE, 3 = exit, 4 = stereo option.
 * ===========================================================================*/
/* DS 0x801c1954 (Config_padd_snd) */
void Config_padd_snd(void)
{
	unsigned char cur_now;

	Config_suuji(0, Volume_Bgm, Cnf_r_no);
	Config_suuji(3, Volume_Se,  Cnf_r_no);

	/* Cancel (Triangle), or Circle on the exit entry. */
	if ((G.Key_trg & 0x2000) != 0 || ((G.Key_trg & 0x1000) != 0 && Cnf_cur[Cnf_r_no] == 3)) {
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		CNF_SE(SOUND_CORE_CANCEL);
		return;
	}
	if ((G.Key_trg & 0x1000) != 0) {
		return;
	}

	if ((G.Padd1 & 0xa000) != 0) {
		if (Cnf_cd4 > 9) {
			Cnf_cd4--;
			Cnf_cd3 = 10;
		}
	} else {
		Cnf_cd3 = 1;
		Cnf_cd4 = 0;
	}

	if ((G.System_flg & SYS_PAD_REPEAT) != 0) {
		if ((G.Padd1 & 0x1000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			if (Cnf_cur[Cnf_r_no] < 4) {
				Cnf_cur[Cnf_r_no]--;
				if (Cnf_cur[Cnf_r_no] == 0xff) {
					Cnf_cur[Cnf_r_no] = 0;
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
			}
		}

		if ((G.Padd1 & 0x4000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			cur_now = Cnf_cur[Cnf_r_no];
			CNF_SPR(Cnf_r_no, cur_now).flags = (cur_now == 0 || cur_now == 4) ? 1 : 5;
			if (Cnf_cur[Cnf_r_no] < 4) {
				Cnf_cur[Cnf_r_no]++;
				if (Cnf_cur[Cnf_r_no] == 4) {
					Cnf_cur[Cnf_r_no] = 3;   /* exit entry: no sound */
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
			} else {
				Cnf_cur[Cnf_r_no] = 1;
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}

		if ((G.Padd1 & 0x8000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			cur_now = Cnf_cur[Cnf_r_no];
			switch (cur_now) {
			case 1:
				Cnf_cd4++;
				Volume_Bgm -= Cnf_cd3;
				if (Volume_Bgm >= 0x65) {
					Volume_Bgm = 0;
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
				Config_suuji(0, Volume_Bgm, Cnf_r_no);
				CNF_SPR(Cnf_r_no, 7).flags = 1;
				/* Seq_no is sign-extended (retail lb), so a closed slot passes -1;
				   the volume is a signed int division by 100 (mult/sra). */
				SsSeqSetVol((short)(signed char)Seq_ctr[0].Seq_no, (short)((Seq_ctr[0].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[0].Vol.right * Volume_Bgm) / 100));
				SsSeqSetVol((short)(signed char)Seq_ctr[1].Seq_no, (short)((Seq_ctr[1].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[1].Vol.right * Volume_Bgm) / 100));
				SsSeqSetVol((short)(signed char)Seq_ctr[2].Seq_no, (short)((Seq_ctr[2].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[2].Vol.right * Volume_Bgm) / 100));
				break;
			case 2:
				Cnf_cd4++;
				Volume_Se -= Cnf_cd3;
				if (Volume_Se >= 0x65) {
					Volume_Se = 0;
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
				Config_suuji(3, Volume_Se, Cnf_r_no);
				CNF_SPR(Cnf_r_no, 0xb).flags = 1;
				break;
			case 0:
				CNF_SPR(Cnf_r_no, 0).flags = 5;
				goto stereo_off;
			case 4:
				CNF_SPR(Cnf_r_no, 4).flags = 5;
			stereo_off:
				Cnf_cur[Cnf_r_no] = 0;
				G.Sound_mode &= 0xfe;
				Snd_sys_stereo(G.Sound_mode);
				CNF_SE(SOUND_CORE_CURSOR);
				break;
			}
		}

		if ((G.Padd1 & 0x2000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			cur_now = Cnf_cur[Cnf_r_no];
			switch (cur_now) {
			case 1:
				Cnf_cd4++;
				Volume_Bgm += Cnf_cd3;
				if (Volume_Bgm >= 0x65) {
					Volume_Bgm = 100;
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
				Config_suuji(0, Volume_Bgm, Cnf_r_no);
				CNF_SPR(Cnf_r_no, 9).flags = 1;
				SsSeqSetVol((short)(signed char)Seq_ctr[0].Seq_no, (short)((Seq_ctr[0].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[0].Vol.right * Volume_Bgm) / 100));
				SsSeqSetVol((short)(signed char)Seq_ctr[1].Seq_no, (short)((Seq_ctr[1].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[1].Vol.right * Volume_Bgm) / 100));
				SsSeqSetVol((short)(signed char)Seq_ctr[2].Seq_no, (short)((Seq_ctr[2].Vol.left  * Volume_Bgm) / 100), (short)((Seq_ctr[2].Vol.right * Volume_Bgm) / 100));
				break;
			case 2:
				Cnf_cd4++;
				Volume_Se += Cnf_cd3;
				if (Volume_Se >= 0x65) {
					Volume_Se = 100;
				} else {
					CNF_SE(SOUND_CORE_CURSOR);
				}
				Config_suuji(3, Volume_Se, Cnf_r_no);
				CNF_SPR(Cnf_r_no, 0xd).flags = 1;
				break;
			case 0:
				CNF_SPR(Cnf_r_no, 0).flags = 5;
				Cnf_cur[Cnf_r_no] = 4;
				goto stereo_on;
			case 4:
				CNF_SPR(Cnf_r_no, 4).flags = 5;
				Cnf_cur[Cnf_r_no] = 0;
			stereo_on:
				G.Sound_mode |= 1;
				Snd_sys_stereo(G.Sound_mode);
				CNF_SE(SOUND_CORE_CURSOR);
				break;
			}
		}
	}

	/* Arrow sprite updates. */
	CNF_SPR(Cnf_r_no, 7).flags = 0;
	CNF_SPR(Cnf_r_no, 8).flags = 1;
	CNF_SPR(Cnf_r_no, 9).flags = 0;
	CNF_SPR(Cnf_r_no, 0xa).flags = 1;
	CNF_SPR(Cnf_r_no, 0xb).flags = 0;
	CNF_SPR(Cnf_r_no, 0xc).flags = 1;
	CNF_SPR(Cnf_r_no, 0xd).flags = 0;
	CNF_SPR(Cnf_r_no, 0xe).flags = 1;
	if ((G.Padd1 & 0x8000) != 0) {
		if (Cnf_cur[Cnf_r_no] == 1) {
			CNF_SPR(Cnf_r_no, 7).flags = 1;
			CNF_SPR(Cnf_r_no, 8).flags = 0;
		}
		if (Cnf_cur[Cnf_r_no] == 2) {
			CNF_SPR(Cnf_r_no, 0xb).flags = 1;
			CNF_SPR(Cnf_r_no, 0xc).flags = 0;
		}
	}
	if ((G.Padd1 & 0x2000) != 0) {
		if (Cnf_cur[Cnf_r_no] == 1) {
			CNF_SPR(Cnf_r_no, 9).flags = 1;
			CNF_SPR(Cnf_r_no, 0xa).flags = 0;
		}
		if (Cnf_cur[Cnf_r_no] == 2) {
			CNF_SPR(Cnf_r_no, 0xd).flags = 1;
			CNF_SPR(Cnf_r_no, 0xe).flags = 0;
		}
	}
}

/* ===========================================================================
 * Config_padd_clr  (DS 801c2848)
 * Row 4: accept / cancel only.
 * ===========================================================================*/
/* DS 0x801c2848 (Config_padd_clr) */
void Config_padd_clr(void)
{
	unsigned long bits;

	if ((G.Key_trg & 0x3000) != 0) {
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		Main.Scrl_type = Main.Scrl_type;   /* retail reloads and stores it back unchanged (801c289c / 801c28b0) */
		bits = SOUND_CORE_CANCEL;
		if ((G.Key_trg & 0x1000) != 0) {
			bits = SOUND_CORE_CONFIRM;
		}
		CNF_SE(bits);
	}
}

/* ===========================================================================
 * Config_padd_res  (DS 801c28d8)
 * Row 5: screen-position adjustment (cur 0 = off, 1 = accept, 2 = exit).
 * ===========================================================================*/
/* DS 0x801c28d8 (Config_padd_res) */
void Config_padd_res(void)
{
	unsigned char was;

	if ((G.Key_trg & 0x2000) != 0) {
		Cnf_status    = 2;
		Cnf_ce1       = 0;
		B_rgb         = 0x2c808080;
		Cnf_mode_next = 0;
		Cnf_mode_naw |= 8;
		CNF_SE(SOUND_CORE_CANCEL);
		return;
	}

	if ((G.Key_trg & 0x1000) != 0) {
		CNF_SE(SOUND_CORE_CONFIRM);
		if (Cnf_cur[Cnf_r_no] == 1) {
			Cnf_status   = 2;
			Cnf_mode_naw |= 0x10;
		} else {
			Cnf_status    = 2;
			Cnf_ce1       = 0;
			B_rgb         = 0x2c808080;
			Cnf_mode_next = 0;
			Cnf_mode_naw |= 8;
		}
		return;
	}

	if ((G.System_flg & SYS_PAD_REPEAT) == 0) {
		return;
	}

	if ((G.Padd1 & 0x1000) != 0) {
		Cnf_ce1 = 0;
		B_rgb   = 0x2c808080;
		if (Cnf_cur[Cnf_r_no] == 2) {
			CNF_SPR(Cnf_r_no, 2).flags = 5;
			was = Cnf_cur[Cnf_r_no];
			Cnf_cur[Cnf_r_no] = 0;
			if (was != 0) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}
	}
	if ((G.Padd1 & 0x4000) != 0) {
		Cnf_ce1 = 0;
		B_rgb   = 0x2c808080;
		CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
		was = Cnf_cur[Cnf_r_no];
		Cnf_cur[Cnf_r_no] = 2;
		if (was != 2) {
			CNF_SE(SOUND_CORE_CURSOR);
		}
	}
	if (Cnf_cur[Cnf_r_no] != 2) {
		if ((G.Padd1 & 0x8000) != 0) {
			B_rgb   = 0x2c808080;
			Cnf_ce1 = 0;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			was = Cnf_cur[Cnf_r_no];
			Cnf_cur[Cnf_r_no] = 1;
			if (was != 1) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}
		if ((G.Padd1 & 0x2000) != 0) {
			Cnf_ce1 = 0;
			B_rgb   = 0x2c808080;
			CNF_SPR(Cnf_r_no, Cnf_cur[Cnf_r_no]).flags = 5;
			was = Cnf_cur[Cnf_r_no];
			Cnf_cur[Cnf_r_no] = 0;
			if (was != 0) {
				CNF_SE(SOUND_CORE_CURSOR);
			}
		}
	}
}

/* ===========================================================================
 * Config_move  (DS 801c2ccc)
 * Per-frame sprite update: key-config bits, selection markers per row.
 * ===========================================================================*/
/* DS 0x801c2ccc (Config_move) */
void Config_move(void)
{
	unsigned int cnt0;
	unsigned int r0;
	int          more;
	unsigned int base1;

	if (Cnf_r_no == 1) {
		base1 = Cnf_st[1].base;
		r0    = 0x10;
		do {
			Cnf_char[base1 + 8 + r0].flags = 0;
			Cnf_char[base1 + 8 + r0].flags =
				(unsigned char)((Botan_offset[Cnf_cdd] >> (r0 & 0x1f)) & 1);
			more = (r0 != 0);
			r0--;
		} while (more);
	} else if (Cnf_r_no < 2) {
		if (Cnf_r_no == 0) {
			Cnf_char[6].flags  = 0x40;
			Cnf_char[7].flags  = 0x40;
			Cnf_char[8].flags  = 0x40;
			Cnf_char[9].flags  = 0x40;
			Cnf_char[10].flags = 0x40;
			Cnf_char[11].flags = 0x40;
			Cnf_char[12].flags = 0x40;
			Cnf_char[G.SaveData.Key_idx + 6].flags  = 0x41;
			Cnf_char[(G.Sound_mode & 1) + 9].flags  = 0x41;
			Cnf_char[(G.Sound_mode >> 7) + 0xb].flags = 0x41;
		}
	} else if (Cnf_r_no == 2) {
		if ((Cnf_mode_naw & 0x80) == 0) {
			CNF_SPR(2, 0).flags = 5;
			cnt0 = Cnf_st[2].base;
		} else {
			CNF_SPR(2, 1).flags = 5;
			cnt0 = Cnf_st[2].base + 1;
		}
		Cnf_char[cnt0].flags = 1;
	} else if (Cnf_r_no == 3) {
		if (Cnf_cur[3] == 0) {
			if ((G.Sound_mode & 1) == 0) {
				Cnf_cur[3] = 0;
			} else {
				Cnf_cur[3] = 4;
			}
		}
		Config_suuji_trans();
	}
}

/* ===========================================================================
 * Config_main_trans  (DS 801c2f44)
 * Build POLY_FT4 primitives for every visible CNF_CHAR and add them to the
 * ordering table.  Glyph sequences come from Mojiretu[]; UVs from Cnf_uv[].
 * ===========================================================================*/
/* DS 0x801c2f44 (Config_main_trans) */
void Config_main_trans(void)
{
	/* retail reserves 16 more bytes of locals (Config_main_trans frame 88); never read */
	volatile int Rsv[4];
	unsigned int   i;
	unsigned int   row;
	unsigned int   cnt;
	unsigned char  otz;
	unsigned char  u1, v2;
	unsigned short flg;
	unsigned long  rgb;
	int            ce_step;
	short          px;
	short          py;
	short          x1;
	short          y2;
	unsigned char *uv;
	unsigned char *pChars;
	CNF_CHAR      *pc;
	POLY_FT4      *p;

	if ((Cnf_ce1 & 0x20) == 0) {
		ce_step = -0x20202;
	} else {
		ce_step = 0x20202;
	}
	B_rgb += ce_step;
	Cnf_ce1++;

	for (i = 0; i < CNF_CHAR_COUNT; i++) {
		pc = &Cnf_char[i];
		if ((pc->flags & 1) != 0) {
			row = pc->row;
			flg = 0;
			if (row == Cnf_r_no) {
				Cnf_char[Cnf_st[row].base + Cnf_cur[row]].flags |= 0x22;
				rgb = 0x2c808080;
				if ((pc->flags & 4) != 0) {
					rgb = 0x2ea07070;
				}
				if (((G.System_flg & 0x40) != 0) && (i == 37)) {
					rgb = 0x2c505050;
				}
				if ((Cnf_mode_naw & 0x20) != 0) {
					if (i == 37) {
						rgb = 0x2c0000a0;
					}
					if (i == 38) {
						rgb = 0x2c0000a0;
					}
				}
				if ((pc->flags & 0x20) != 0) {
					rgb = B_rgb;
				}
				if ((pc->flags & 0x40) != 0) {
					rgb = 0x2eff5858;
				}
				flg = ((Cnf_mode_naw & 2) == 0) ? 0x7f80 : 0;
			} else {
				if (row > 5) {
					rgb = 0x2c606060;
					flg = 0x7fc0;
					if (G.Cbuf_id != 0) {
						/* Scrolling marquee rows (7 right, 8 left). */
						if (row == 7) {
							pc->x++;
							if (pc->x > 0x168) {
								pc->x = -0x77;
							}
						}
						if (pc->row == 8) {
							pc->x--;
							if (pc->x < -0x78) {
								pc->x = 0x167;
							}
						}
					}
				}
				if ((Cnf_mode_naw & 1) != 0) {
					flg = 0;
				}
			}

			if (flg != 0) {
				pChars = Mojiretu;
				cnt    = 0;
				if (pc->glyph != 0) {
					do {
						if (*pChars == 0xff) {
							cnt++;
						}
						pChars++;
					} while (pc->glyph != cnt);
				}
				px = pc->x;
				otz = pc->otz;
				py = pc->y;
				while (*pChars != 0xff) {
					p  = (POLY_FT4 *)pOprim;
					uv = Cnf_uv[*pChars];
					u1 = uv[0] + uv[2];
					v2 = uv[1] + uv[3];
					x1 = (unsigned short)uv[2] + px;
					y2 = (unsigned short)uv[3] + py;
					SetPolyFT4(p);
					p->x0    = px;
					p->r0    = (unsigned char)rgb;
					p->g0    = (unsigned char)(rgb >> 8);
					p->b0    = (unsigned char)(rgb >> 16);
					p->code  = (unsigned char)(rgb >> 24);
					p->y0    = py;
					p->u0    = uv[0];
					p->v0    = uv[1];
					p->x1    = x1;
					p->y1    = py;
					p->u1    = u1;
					p->clut  = flg;
					p->tpage = 0x9e;
					p->x2    = px;
					p->y2    = y2;
					p->v1    = uv[1];
					p->v2    = v2;
					p->x3    = x1;
					p->y3    = y2;
					p->u3    = u1;
					p->v3    = v2;
					p->u2    = uv[0];
					AddPrim(G.pOt1 + otz, p);
					pOprim = (POLY_FT4 *)pOprim + 1;
					px += uv[2];
					pChars++;
				}
			}
		}
	}
}

/* ===========================================================================
 * Config_suuji_trans  (DS 801c339c)
 * Draw the 6-digit volume strip (BGM digits 0-2, SE digits 3-5).
 * ===========================================================================*/
/* DS 0x801c339c (Config_suuji_trans) */
void Config_suuji_trans(void)
{
	int            i;
	int            more;
	/* Digit positions (retail copies these tables from DS 801bfa54 / 801bfa60). */
	short          pos_x[6] = { 0xed, 0xdf, 0xd1, 0xed, 0xdf, 0xd1 };
	short          pos_y[6] = { 0x6c, 0x6c, 0x6c, 0x83, 0x83, 0x83 };
	POLY_FT4      *p;
	unsigned char *uv;
	unsigned char  u1;
	unsigned char  v2;
	short          x1;
	short          y2;

	i = 5;
	do {
		p = (POLY_FT4 *)pOprim;
		if (Suuji[i] == 0xff) {
			i -= 2;
		} else {
			uv = Cnf_uv[Suuji[i]];
			u1 = uv[0] + uv[2];
			v2 = uv[1] + uv[3];
			x1 = (unsigned short)uv[2] + pos_x[i];
			y2 = (unsigned short)uv[3] + pos_y[i];
			SetPolyFT4(p);
			p->r0    = 88;
			p->g0    = 0xa0;
			p->b0    = 88;
			p->code  = 44;
			p->x0    = pos_x[i];
			p->y0    = pos_y[i];
			p->u0    = uv[0];
			p->v0    = uv[1];
			p->clut  = 0x7f80;
			p->x1    = x1;
			p->y1    = pos_y[i];
			p->u1    = u1;
			p->tpage = 0x9e;
			p->v1    = uv[1];
			p->x2    = pos_x[i];
			p->y2    = y2;
			p->v2    = v2;
			p->x3    = x1;
			p->y3    = y2;
			p->u3    = u1;
			p->v3    = v2;
			p->u2    = uv[0];
			AddPrim(G.pOt1 + 10, p);
			pOprim = p + 1;
		}
		more = (i != 0);
		i--;
	} while (more);
}

/* ===========================================================================
 * Config_lrgb  (DS 801c3598)
 * State 5: load the colour TIM selected by Cnf_cd5, then return to state 0.
 * ===========================================================================*/
/* DS 0x801c3598 (Config_lrgb) */
void Config_lrgb(void)
{
	/* TIM file numbers (retail copies this table from DS 801bfa6c onto the stack). */
	unsigned short lrgb_file[3] = { PADDU_TIM, COLOR_TIM, PADDDSU_TIM };
	unsigned long  ret;

	ret = Cd_read((unsigned int)lrgb_file[Cnf_cd5], Bg_img_buf, 0, (unsigned char *)"COLOR_TIM_LD");
	if (ret == 0xffffffff) {
		Cnf_err |= 1;
	}
	Cnf_status = 0;
}

/* ===========================================================================
 * Config_suuji  (DS 801c362c)
 * Set the Suuji[] glyph ids for one volume channel (0 = BGM, 3 = SE) and the
 * visibility flag of its sprite in row option.
 * vol: 0-100 (>=100 shows "100").
 * ===========================================================================*/
/* DS 0x801c362c (Config_suuji) */
void Config_suuji(int channel, unsigned int vol, int option)
{
	char tens_ch;
	int  spr_idx;

	if (vol == 0) {
		Suuji[channel]     = 0xff;
		Suuji[channel + 1] = 0xff;
		Suuji[channel + 2] = 0xff;
		if (channel == 0) {
			spr_idx = Cnf_st[option].base + 5;
		} else {
			spr_idx = Cnf_st[option].base + 6;
		}
		Cnf_char[spr_idx].flags = 1;
	} else {
		if (vol < 100) {
			tens_ch = (char)(vol / 10) + 23;
			if (vol / 10 == 0) {
				tens_ch = 55;
			}
			Suuji[channel + 1] = tens_ch;
			Suuji[channel + 2] = 0x37;
			Suuji[channel]     = (char)vol + (char)(vol / 10) * -10 + 23;
		} else {
			Suuji[channel]     = 0x17;
			Suuji[channel + 1] = 0x17;
			Suuji[channel + 2] = 0x18;
		}
		if (channel == 0) {
			spr_idx = Cnf_st[option].base + 5;
		} else {
			spr_idx = Cnf_st[option].base + 6;
		}
		Cnf_char[spr_idx].flags = 0;
	}
}
