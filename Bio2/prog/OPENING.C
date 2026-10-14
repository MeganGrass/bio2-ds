/*
 * OPENING.C - Opening (intro) scene timelines, sprites and fades
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OPENING.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include <MCGUI.H>
#include "OPENING.H"
#include "CD.H"
#include "CONFIG.H"
#include "DIESPR.H"
#include "MAIN.H"
#include "MONITOR.H"
#include "MOVIE.H"
#include "SND.H"
#include "SUB.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "FILECODE.H"
#include "flag.h"


/* Cd_read file codes. [0..3] TIM loads at the top of Opening_init; the
 * scene branches load [4], [5], [7], [8] as TIM and [6], [9] into
 * Opening_scene_tim_buf. */
/* DS 0x801c24ac (Opening_cd_no) */
USHORT Opening_cd_no[10] = {
	OPEN00_TIM, OPEN01_TIM, OPEN02_TIM, OPEN03_TIM, OPEN04_TIM,
	OPEN10_TIM, OPEN11_TIM, OPEN12_TIM, OPEN13_TIM, OPEN14_TIM
};

/* Overlay data: ROM items take the OPENING.BIN bytes; runtime variables are zero-initialised. */
/* DS 0x801bfa24 (Opening_ctpage) */
UCHAR Opening_ctpage[5] = { 0x05, 0x0a, 0x15, 0x18, 0x18 };

/* DS 0x801bfa2c (Opening_str_op_tim_ld) */
CHAR Opening_str_op_tim_ld[10] = { 'O', 'P', '_', 'T', 'I', 'M', '_', 'L', 'D', 0x00 };

/* DS 0x801c2068 (Opening_draw_tbl) */
VOID *Opening_draw_tbl[4] = {
	(VOID *)Opening_bg_fade_in,	/* 0 */
	(VOID *)Opening_draw_idle,	/* 1 */
	(VOID *)Opening_bg_fade_out,	/* 2 */
	(VOID *)Opening_exit	/* 3 */
};

/* DS 0x801c2078 (Opening_scene_tbl) */
VOID *Opening_scene_tbl[5] = {
	(VOID *)Opening_timeline_a,	/* 0 */
	(VOID *)Opening_timeline_a,	/* 1 */
	(VOID *)Opening_timeline_b,	/* 2 */
	(VOID *)Opening_timeline_c,	/* 3 */
	(VOID *)Opening_trans_wait	/* 4 */
};

/* DS 0x801c208c (Opening_moji_tbl) */
OP_MOJI_TBL Opening_moji_tbl[14] = {
	{ 0x0005, 0x7800, 0x00, 0x00, 0xa0, 0xa0, 0xa0, 0xa0 },
	{ 0x0006, 0x7800, 0x20, 0x00, 0x9f, 0xa0, 0xa0, 0xa0 },
	{ 0x0007, 0x7800, 0x40, 0x00, 0xa0, 0xa0, 0xa0, 0xa0 },
	{ 0x0008, 0x7800, 0x60, 0x00, 0x9f, 0xa0, 0xa0, 0xa0 },
	{ 0x0015, 0x78c0, 0x00, 0x00, 0xbd, 0xbe, 0xc8, 0xc8 },
	{ 0x000a, 0x7840, 0x00, 0x00, 0xa0, 0xa0, 0xa0, 0xa0 },
	{ 0x000a, 0x7840, 0x00, 0xa0, 0x9f, 0x50, 0xa0, 0xa0 },
	{ 0x000b, 0x7840, 0x20, 0x00, 0xa0, 0xa0, 0xa0, 0xa0 },
	{ 0x000b, 0x7840, 0x20, 0xa0, 0x9f, 0x50, 0xa0, 0x50 },
	{ 0x0016, 0x7900, 0x40, 0x00, 0xbd, 0xbe, 0x14, 0x14 },
	{ 0x000c, 0x7880, 0x40, 0x00, 0xa0, 0xa0, 0xa0, 0xa0 },
	{ 0x000c, 0x7880, 0x40, 0xa0, 0xa0, 0x50, 0xa0, 0x50 },
	{ 0x000d, 0x7880, 0x60, 0x00, 0x9f, 0xa0, 0xa0, 0xa0 },
	{ 0x000d, 0x7880, 0x60, 0xa0, 0x9f, 0x50, 0xa0, 0x50 }
};

/* DS 0x801c2118 (Opening_pict_tbl) */
OP_MOJI_TBL Opening_pict_tbl[22] = {
	{ 0x0018, 0x7940, 0x00, 0x00, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0x20, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0x40, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0x60, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0x80, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0xa0, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0018, 0x7940, 0x00, 0xc0, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x00, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x20, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x40, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x60, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x80, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0xa0, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x00, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x20, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x40, 0xff, 0x10, 0xff, 0x10 },
	{ 0x0019, 0x7940, 0x00, 0x50, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0x70, 0xff, 0x10, 0xff, 0x10 },
	{ 0x0019, 0x7940, 0x00, 0x80, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0xa0, 0xff, 0x20, 0xff, 0x20 },
	{ 0x0019, 0x7940, 0x00, 0xc0, 0xff, 0x10, 0xff, 0x10 },
	{ 0x0019, 0x7940, 0x00, 0xd0, 0xff, 0x10, 0xff, 0x10 }
};

/* DS 0x801c21f4 (Opening_sprite_s1) */
OP_WORK Opening_sprite_s1[28] = {
	{ 0x06, 0x10, 0x10, 0x00, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x10, 0x10, 0x00, 0x0020, 0x00b4, 0x00000000 },
	{ 0x00, 0x00, 0x20, 0x00, 0x0000, 0x000a, 0x00000000 },
	{ 0x00, 0x00, 0x20, 0x01, 0x00a0, 0x000a, 0x00000000 },
	{ 0x00, 0x00, 0x20, 0x02, 0x0000, 0x00aa, 0x00000000 },
	{ 0x00, 0x00, 0x20, 0x03, 0x00a0, 0x00aa, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x01, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x01, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x02, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x02, 0x0020, 0x00b4, 0x00000000 },
	{ 0x00, 0x20, 0x15, 0x04, 0x00a0, 0x0078, 0x00000000 },
	{ 0x00, 0x01, 0x20, 0x05, 0x0000, 0x0000, 0x00000000 },
	{ 0x00, 0x01, 0x20, 0x06, 0x0000, 0x00a0, 0x00000000 },
	{ 0x00, 0x01, 0x20, 0x07, 0x00a0, 0x0000, 0x00000000 },
	{ 0x00, 0x01, 0x20, 0x08, 0x00a0, 0x00a0, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x03, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x03, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x04, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x04, 0x0020, 0x00b4, 0x00000000 },
	{ 0x00, 0x20, 0x15, 0x09, 0x00a0, 0x0078, 0x00000000 },
	{ 0x00, 0x02, 0x20, 0x0a, 0x0000, 0x0000, 0x00000000 },
	{ 0x00, 0x02, 0x20, 0x0b, 0x0000, 0x00a0, 0x00000000 },
	{ 0x00, 0x02, 0x20, 0x0c, 0x00a0, 0x0000, 0x00000000 },
	{ 0x00, 0x02, 0x20, 0x0d, 0x00a0, 0x00a0, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x05, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x05, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x16, 0x10, 0x06, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x16, 0x10, 0x06, 0x0020, 0x00b4, 0x00000000 }
};

/* DS 0x801c2344 (Opening_sprite_s2) */
OP_WORK Opening_sprite_s2[12] = {
	{ 0x06, 0x10, 0x10, 0x07, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x10, 0x10, 0x07, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x08, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x08, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x09, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x09, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x0a, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x0a, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x0b, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x0b, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x0c, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x0c, 0x0020, 0x00b4, 0x00000000 }
};

/* DS 0x801c23d4 (Opening_sprite_s3) */
OP_WORK Opening_sprite_s3[18] = {
	{ 0x06, 0x10, 0x10, 0x0d, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x10, 0x10, 0x0d, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x0e, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x11, 0x10, 0x0e, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x0f, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x12, 0x10, 0x0f, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x10, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x13, 0x10, 0x10, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x11, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x14, 0x10, 0x11, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x12, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x15, 0x10, 0x12, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x16, 0x10, 0x13, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x16, 0x10, 0x13, 0x0020, 0x00b4, 0x00000000 },
	{ 0x06, 0x17, 0x10, 0x14, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x17, 0x10, 0x14, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x18, 0x10, 0x15, 0x0020, 0x00c4, 0x00000000 },
	{ 0x06, 0x18, 0x10, 0x15, 0x0020, 0x00c4, 0x00000000 }
};

/* DS 0x801c24c0 (Opening_fade_no) */
UCHAR Opening_fade_no;

/* DS 0x801c24c2 (Opening_seq_no) */
USHORT Opening_seq_no;

/* DS 0x801c24c4 (Opening_frame_cnt) */
ULONG Opening_frame_cnt;

/* DS 0x801c24c8 (Opening_scene_saved) */
USHORT Opening_scene_saved;

/* DS 0x801c24ca (Opening_skip_ok) */
UCHAR Opening_skip_ok;

/* DS 0x801c24cc (Opening_sprite_num) */
SHORT Opening_sprite_num;

/* DS 0x801c24ce (Opening_draw_mode) */
UCHAR Opening_draw_mode;

/* DS 0x801c24d0 (Opening_timer) */
USHORT Opening_timer;

/* DS 0x801c24d2 (Opening_scene_no) */
USHORT Opening_scene_no;

/* DS 0x801c24d4 (Opening_trans_phase) */
UCHAR Opening_trans_phase;

/* DS 0x801c24d8 (Opening_prim) */
POLY_FT4 Opening_prim[2][32] RAM_SECTION(".buf_opening_prim");

/* DS 0x8013f814: second scene TIM buffer (OPEN11/OPEN14, 0x25800 bytes = floc size); copied to Bg_img_buf */
ULONG Opening_scene_tim_buf[0x9600] RAM_SECTION(".buf_opening_scene_tim_buf");

/* DS 0x80165028: bank TIM load buffer (OPEN01_TIM, 0x25C20 bytes = the largest floc size) */
ULONG Opening_tim_load_buf[0x9708] RAM_SECTION(".buf_opening_tim_load_buf");

/* DS 0x801c2ed8 (Opening_sprite_cur) */
OP_WORK *Opening_sprite_cur;

/* DS 0x801c2edc (Opening_scroll_dy) */
SHORT Opening_scroll_dy;

/* DS 0x801c2ede (Opening_blink_dy) */
SHORT Opening_blink_dy;

/* DS 0x801c2ee0 (Opening_scroll_cnt) */
USHORT Opening_scroll_cnt;

/* DS 0x801c2ee2 (Opening_blink_cnt) */
USHORT Opening_blink_cnt;

/* DS 0x801c2ee4 (Opening_bg_flg) */
UCHAR Opening_bg_flg;

/* DS 0x801c2ee8 (Opening_vis_mask) */
ULONG Opening_vis_mask;

/* Stack copy of Opening_ctpage. Opening_init copies the 5-byte table as one block at the top
 * of the function, as retail does, and indexes that copy. */
typedef struct { UCHAR t[5]; } OP_CTPAGE_TBL;

/* DS 0x801bfad8 (Opening) */
VOID Opening(VOID)
{
	Opening_init();
	do {
		if ((Opening_frame_cnt > 400) && (Opening_skip_ok != 0) && ((G.Padd1 & 0x800) != 0)) {
			if (Opening_trans_phase == 0) {
				Opening_trans_phase = 1;
				if ((G.Status_flg & STAT_XA_VOICE) != 0) {
					Xa_error();
				}
				Snd_bgm_fade_ON(0x78, 0x18);
				Opening_bg_flg = 8;
				((OP_FADE *)Main.Fade)[3].Add = 1;
				Opening_fade_no = 0;
				Opening_bg_fade_out();
				Opening_timer = 0;
				Opening_scene_saved = Opening_scene_no;
				/* retail switch on scene 0-3; any other scene keeps Opening_seq_no */
				switch (Opening_scene_no) {
				case 0:
				case 1:
					Opening_seq_no = 6;
					break;
				case 2:
					Opening_seq_no = 7;
					break;
				case 3:
					Opening_seq_no = 10;
					break;
				}
				Opening_scene_no = 4;
			}
			if (Opening_trans_phase == 2) {
				Opening_trans_phase = 3;
				Opening_scene_saved = 4;
				Opening_scene_no = 4;
				Opening_draw_mode = 3;
			}
		}
		((VOID (*)(VOID))Opening_draw_tbl[Opening_draw_mode])();
		((VOID (*)(VOID))Opening_scene_tbl[Opening_scene_no])();
		Opening_sprite_draw();
		if (++Opening_frame_cnt == 0xffffffff) {
			Opening_frame_cnt = 0x191;
		}
		Task_sleep(1);
	} while (1);
}

/* DS 0x801bfcfc (Opening_trans_wait) */
VOID Opening_trans_wait(VOID)
{
	UINT i;
	OP_WORK *pSpr;

	pSpr = Opening_sprite_cur;
	for (i = 0; i < (UINT)(INT)Opening_sprite_num; i++) {
		if ((pSpr[i].type & 0x10) != 0) {
			pSpr[i].flg = 0;
		}
	}
	if (Opening_trans_phase == 1) {
		Opening_frame_cnt = 0;
		if ((pEdt_adr[0].id_check == 0) && (Fade_status(0) != 0)) {
			Opening_vis_mask = 0;
			Opening_vis_set(0, 1);
			Opening_trans_phase = 2;
			Opening_timer = 0x5a;
			Opening_scene_no = Opening_scene_saved;
		}
	}
}

/* DS 0x801bfdc8 (Opening_init) */
VOID Opening_init(VOID)
{
	BGMCTR bgm;
	USHORT idx;
	ULONG cd_ret;
	UCHAR *pFree_bak;
	OP_CTPAGE_TBL ctpage;

	ctpage = *(OP_CTPAGE_TBL *)Opening_ctpage;
	Task_suspend(0);
	Bg_set_mode(2, 0);
	G.Status_flg |= STAT_CINESCO;
	SetGeomScreen(0xd0);
	SetGeomOffset(0, 0);
	Opening_skip_ok = 1;
	Opening_draw_mode = 1;
	Opening_seq_no = 0;
	Opening_fade_no = 0;
	Opening_timer = 0;
	Opening_vis_mask = 0;
	Opening_scroll_cnt = 0;
	Opening_blink_cnt = 0;
	Opening_trans_phase = 0;
	Opening_frame_cnt = 0;
	Main.Vloop = 0;

	// Scene selection from System_flg / Status_flg / Save_area
	if ((G.System_flg & SYS_NEW_GAME) == 0) {
		if ((G.System_flg & SYS_TIMER) != 0) {
			Opening_draw_mode = 3;
			Opening_scene_no = 4;
		}
		else if (G.SaveData.Save_area - 0xf == 0) {
			if ((G.Status_flg & STAT_PLAYER) != 0) {
				Opening_scene_no = 1;
			}
			else {
				Opening_scene_no = 0;
			}
		}
		else if (G.SaveData.Save_area - 0xf != 1) {
			Opening_exit();
		}
		else if ((G.Status_flg & STAT_PLAYER) == 0) {
			Opening_scene_no = 2;
		}
		else {
			Opening_scene_no = 3;
		}
	}
	else {
		if ((G.System_flg & SYS_TIMER) != 0) {
			Opening_draw_mode = 3;
			Opening_scene_no = 4;
		}
		else if ((G.Status_flg & STAT_PLAYER) == 0) {
			if ((G.Status_flg & STAT_SCENARIO_B) != 0) {
				Opening_scene_no = 2;
			}
			else {
				Opening_scene_no = 0;
			}
		}
		else if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
			Opening_scene_no = 1;
		}
		else {
			Opening_scene_no = 3;
		}
	}
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		Opening_draw_mode = 3;
		Opening_scene_no = 4;
	}

	G.Cclut = 0;
	idx = 0;
	do {
		do {
			cd_ret = Cd_read(Opening_cd_no[idx], Opening_tim_load_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
		G.Ctpage = ctpage.t[idx];
		Mapping_tim(Opening_tim_load_buf);
		Task_sleep(1);
		idx++;
	} while (idx < 4);

	if (Opening_scene_no == 2) {
		pFree_bak = G.pFree_work;
		G.pFree_work = (UCHAR *)Tex_tim_buf;
		bgm.Main = 44;
		bgm.Sub = 0xff;
		pBgm = &bgm;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Opening_sprite_num = 12;
		Opening_sprite_cur = Opening_sprite_s2;
		G.pFree_work = pFree_bak;
		do {
			cd_ret = Cd_read(Opening_cd_no[8], (ULONG *)Tex_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
		do {
			cd_ret = Cd_read(Opening_cd_no[9], Opening_scene_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
	}
	else if (Opening_scene_no < 3) {
		pFree_bak = G.pFree_work;
		G.pFree_work = (UCHAR *)Tex_tim_buf;
		bgm.Main = 43;
		bgm.Sub = 0xff;
		pBgm = &bgm;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Opening_sprite_num = 28;
		Opening_sprite_cur = Opening_sprite_s1;
		G.pFree_work = pFree_bak;
		do {
			cd_ret = Cd_read(Opening_cd_no[5], (ULONG *)Tex_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
		do {
			cd_ret = Cd_read(Opening_cd_no[6], Opening_scene_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
	}
	else if (Opening_scene_no == 3) {
		do {
			cd_ret = Cd_read(Opening_cd_no[4], Opening_tim_load_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
		G.Cclut--;
		G.Ctpage = ctpage.t[4];
		Mapping_tim(Opening_tim_load_buf);
		Task_sleep(1);
		pFree_bak = G.pFree_work;
		G.pFree_work = (UCHAR *)Tex_tim_buf;
		bgm.Main = 44;
		bgm.Sub = 0xff;
		pBgm = &bgm;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Opening_sprite_num = 18;
		Opening_sprite_cur = Opening_sprite_s3;
		G.pFree_work = pFree_bak;
		do {
			cd_ret = Cd_read(Opening_cd_no[7], (ULONG *)Tex_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
		do {
			cd_ret = Cd_read(Opening_cd_no[9], Opening_scene_tim_buf, 1, (UCHAR *)Opening_str_op_tim_ld);
		} while (cd_ret == 0xffffffff);
	}
}

/* DS 0x801c0388 (Opening_exit) */
VOID Opening_exit(VOID)
{
	BGMCTR bgm;

	/* Top-tested wait loop as in the retail code (test, forward exit, Task_sleep, jump back).
	 * A while() here makes cc1psx loop-optimise it and hoist the global addresses used after
	 * the loop into extra callee-saved registers (frame 56 instead of the retail 32). */
wait_fade:
	if (Fade_status(Opening_fade_no) == 0) {
		Task_sleep(1);
		goto wait_fade;
	}
	if ((G.Status_flg & STAT_XA_VOICE) != 0) {
		Xa_error();
	}
	bgm.Main = 0xff;
	bgm.Sub = 0xff;
	pBgm = &bgm;
	Snd_bgm_main();
	Snd_sys_Allvoff();
	G.Status_flg &= ~STAT_CINESCO;
	Bg_set_mode(2, 0);
	Fade_off(0);
	Fade_off(3);
	((OP_FADE *)Main.Fade)[3].Kido = -1;
	((OP_FADE *)Main.Fade)[3].Add = 0;
	CD.Vol = 0x78;
	Task_signal(0);
	Task_exit();
}

/* DS 0x801c0474 (Opening_bg_fade_in) */
VOID Opening_bg_fade_in(VOID)
{
	if ((Opening_bg_flg & 1) == 0) {
		Fade_set(Opening_fade_no | 0x200, -0x100, 7, 1);
		Opening_draw_mode = 0;
		Opening_bg_flg |= 1;
		Bg_set_mode(0, 0);
	}
	else if (Fade_status(Opening_fade_no) != 0) {
		Opening_draw_mode = 1;
		Opening_bg_flg ^= 1;
	}
}

/* DS 0x801c051c (Opening_bg_fade_out) */
VOID Opening_bg_fade_out(VOID)
{
	if ((Opening_bg_flg & 2) == 0) {
		Fade_set(Opening_fade_no | 0x200, 0x100, 7, 1);
		Opening_draw_mode = 2;
		Opening_bg_flg |= 2;
	}
	else if (Fade_status(Opening_fade_no) != 0) {
		if ((Opening_bg_flg & 4) == 0) {
			Opening_draw_mode = (Opening_bg_flg & 8) != 0;
			Opening_bg_flg &= 0xf5;
		}
		else {
			Opening_draw_mode = 3;
			Opening_vis_mask = 0;
			Opening_bg_flg &= 0xfd;
			Opening_vis_set(0, 1);
		}
		Bg_set_mode(2, 0);
	}
}

/* DS 0x801c0630 (Opening_draw_idle) */
VOID Opening_draw_idle(VOID)
{
	/* empty draw handler (only a jr ra in the overlay) */
}

/* DS 0x801c0638 (Opening_timeline_a) */
VOID Opening_timeline_a(VOID)
{
	USHORT next_seq;

	next_seq = 0;
	switch (Opening_seq_no) {
	case 0:
		Opening_fade_set(3, 0, 0xffffff, NULL);
		Snd_bgm_ctr(0x1000000);
		CD.Vol = 0x6b;
		Opening_seq_no = 1;
		Opening_timer = 0;
		Opening_fade_no = 3;
		goto next_timer;
	case 1:
		if (Opening_timer == 0xf0) {
			Memcpy(Bg_img_buf, Tex_tim_buf, 0x25800);
			Opening_bg_fade_in();
		}
		if (Opening_timer == 0x1a4) {
			Xa_play(1, 1);
		}
		if (Opening_timer == 0x1ae) {
			Opening_vis_mask = 3;
			Opening_vis_set(7, 1);
		}
		if (Opening_timer == 0x319) {
			Opening_vis_mask = 3;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer != 0x337) {
			goto next_timer;
		}
		Opening_bg_fade_out();
		Opening_bg_flg = 8;
		Opening_vis_mask = 0x3c;
		Opening_vis_set(0x17, 0);
		next_seq = 2;
		break;
	case 2:
		if (Opening_timer == 0x40) {
			Xa_play(1, 2);
		}
		if (Opening_timer == 0x4a) {
			Opening_vis_mask = 0xc0;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x15d) {
			Opening_vis_mask = 0xc0;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer == 0x167) {
			Opening_vis_mask = 0x300;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x36b) {
			Opening_vis_mask = 0x300;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer != 0x389) {
			goto next_timer;
		}
		Opening_vis_mask = 0x3c;
		Opening_vis_set(0x1b, 0);
		Opening_vis_mask = 0x400;
		Opening_vis_set(0x47, 0);
		next_seq = 3;
		break;
	case 3:
		if (Opening_timer == 0x5e) {
			Opening_vis_mask = 0x7800;
			Opening_vis_set(7, 0);
			Xa_play(1, 3);
		}
		if (Opening_timer == 0x68) {
			Opening_vis_mask = 0x18000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0xcc) {
			Opening_vis_mask = 0x400;
			Opening_vis_set(0x4b, 0);
		}
		if (Opening_timer == 0x141) {
			Opening_vis_mask = 0x18000;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer != 0x15f) {
			goto next_timer;
		}
		Memcpy(Bg_img_buf, Opening_scene_tim_buf, 0x25800);
		Opening_bg_fade_in();
		Opening_vis_mask = 0x7800;
		Opening_vis_set(0xb, 0);
		next_seq = 4;
		break;
	case 4:
		if (Opening_timer == 0x5e) {
			Opening_vis_mask = 0;
			Opening_vis_set(0, 1);
			Xa_play(1, 4);
		}
		if (Opening_timer == 0x68) {
			Opening_vis_mask = 0x60000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x226) {
			Opening_vis_mask = 0x60000;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer != 0x244) {
			goto next_timer;
		}
		Opening_bg_fade_out();
		Opening_bg_flg = 8;
		Opening_vis_mask = 0x80000;
		Opening_vis_set(0x27, 0);
		next_seq = 5;
		break;
	case 5:
		if (Opening_timer == 0x60) {
			Opening_vis_mask = 0xf00000;
			Opening_vis_set(7, 0);
			Xa_play(1, 5);
		}
		if (Opening_timer == 0x6a) {
			Opening_vis_mask = 0x3000000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0xc9) {
			Opening_vis_mask = 0x3000000;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer == 0xd3) {
			Opening_vis_mask = 0xc000000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x105) {
			Opening_vis_mask = 0x80000;
			Opening_vis_set(0x2b, 0);
		}
		if (Opening_timer == 0x145) {
			Opening_vis_mask = 0xc000000;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer == 0x1bd) {
			Snd_bgm_fade_ON(0x78, 0x18);
			Opening_bg_flg = 8;
			Opening_fade_no = 0;
			Opening_skip_ok = 0;
			Opening_trans_phase = 2;
			Opening_bg_fade_out();
		}
		if ((Opening_timer < 0x1be) || (Fade_status(Opening_fade_no) == 0)) {
			goto next_timer;
		}
		Opening_skip_ok = 1;
		Opening_vis_mask = 0;
		Opening_vis_set(0, 1);
		next_seq = 6;
		break;
	case 6:
		if ((Opening_timer != 100) || (pEdt_adr[0].id_check != 0)) {
			goto next_timer;
		}
		G.Status_flg &= ~STAT_CINESCO;
		Init_movie_work(3);
		next_seq = 7;
		break;
	case 7:
		next_seq = 8;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 8:
		if (Opening_timer != 1) {
			goto next_timer;
		}
		Init_movie_work(0xf);
		next_seq = 9;
		break;
	case 9:
		next_seq = 10;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 10:
		if (Opening_timer == 10) {
			Opening_draw_mode = 3;
			Opening_scene_no = 4;
		}
		goto next_timer;
	default:
		goto next_timer;
	}
	Opening_timer = 0;
	Opening_seq_no = next_seq;
next_timer:
	Opening_timer++;
}

/* DS 0x801c0d58 (Opening_timeline_b) */
VOID Opening_timeline_b(VOID)
{
	USHORT next_seq;

	next_seq = 0;
	switch (Opening_seq_no) {
	case 0:
		Opening_fade_set(3, 0, 0xffffff, NULL);
		Snd_bgm_ctr(0x1000000);
		CD.Vol = 0x6b;
		Opening_seq_no = 1;
		Opening_timer = 0;
		Opening_fade_no = 3;
		goto next_timer;
	case 1:
		if (Opening_timer == 0xf0) {
			Xa_play(1, 6);
		}
		if (Opening_timer == 0xfa) {
			Opening_vis_mask = 3;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x20b) {
			goto next_timer;
		}
		Opening_vis_mask = 3;
		Opening_vis_set(0xb, 0);
		next_seq = 2;
		break;
	case 2:
		if (Opening_timer == 1) {
			Xa_play(1, 7);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0xc;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0xbe) {
			Xa_play(1, 8);
		}
		if (Opening_timer != 0x184) {
			goto next_timer;
		}
		Opening_vis_mask = 0xc;
		Opening_vis_set(0xb, 0);
		Memcpy(Bg_img_buf, Tex_tim_buf, 0x25800);
		Opening_bg_fade_in();
		next_seq = 3;
		break;
	case 3:
		if (Opening_timer == 0x80) {
			Xa_play(1, 9);
		}
		if (Opening_timer == 0x8a) {
			Opening_vis_mask = 0x30;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x150) {
			Opening_vis_mask = 0x30;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer != 0x18c) {
			goto next_timer;
		}
		Opening_bg_fade_out();
		Opening_bg_flg = 8;
		next_seq = 4;
		break;
	case 4:
		if (Opening_timer == 0x3c) {
			Xa_play(1, 10);
		}
		if (Opening_timer == 0x46) {
			Opening_vis_mask = 0xc0;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x112) {
			goto next_timer;
		}
		Opening_vis_mask = 0xc0;
		Opening_vis_set(0xb, 0);
		next_seq = 5;
		break;
	case 5:
		if (Opening_timer == 1) {
			Xa_play(1, 0xb);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0x300;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0xfd) {
			goto next_timer;
		}
		Opening_vis_mask = 0x300;
		Opening_vis_set(0xb, 0);
		Memcpy(Bg_img_buf, Opening_scene_tim_buf, 0x25800);
		Opening_bg_fade_in();
		next_seq = 6;
		break;
	case 6:
		if (Opening_timer == 0x80) {
			Xa_play(1, 0xc);
		}
		if (Opening_timer == 0x8a) {
			Opening_vis_mask = 0xc00;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x133) {
			Opening_vis_mask = 0xc00;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer == 0x205) {
			Snd_bgm_fade_ON(0x78, 0x18);
			Opening_bg_flg = 8;
			Opening_fade_no = 0;
			Opening_bg_fade_out();
			Opening_skip_ok = 0;
			Opening_trans_phase = 2;
		}
		if ((Opening_timer < 0x206) || (Fade_status(Opening_fade_no) == 0)) {
			goto next_timer;
		}
		Opening_vis_mask = 0;
		Opening_vis_set(0, 1);
		Opening_skip_ok = 1;
		next_seq = 7;
		break;
	case 7:
		if ((Opening_timer != 100) || (pEdt_adr[0].id_check != 0)) {
			goto next_timer;
		}
		G.Status_flg &= ~STAT_CINESCO;
		Init_movie_work(3);
		next_seq = 8;
		break;
	case 8:
		next_seq = 9;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 9:
		if (Opening_timer != 1) {
			goto next_timer;
		}
		Init_movie_work(0x10);
		next_seq = 10;
		break;
	case 10:
		next_seq = 0xb;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 0xb:
		if (Opening_timer == 10) {
			Opening_draw_mode = 3;
			Opening_scene_no = 4;
		}
		goto next_timer;
	default:
		goto next_timer;
	}
	Opening_timer = 0;
	Opening_seq_no = next_seq;
next_timer:
	Opening_timer++;
}

/* DS 0x801c1304 (Opening_timeline_c) */
VOID Opening_timeline_c(VOID)
{
	USHORT next_seq;

	next_seq = 0;
	switch (Opening_seq_no) {
	case 0:
		Opening_fade_set(3, 0, 0xffffff, NULL);
		Snd_bgm_ctr(0x1000000);
		CD.Vol = 0x6b;
		Opening_seq_no = 1;
		Opening_timer = 0;
		Opening_fade_no = 3;
		goto next_timer;
	case 1:
		if (Opening_timer == 0xf0) {
			Xa_play(1, 6);
		}
		if (Opening_timer == 0xfa) {
			Opening_vis_mask = 3;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x20b) {
			goto next_timer;
		}
		Opening_vis_mask = 3;
		Opening_vis_set(0xb, 0);
		next_seq = 2;
		break;
	case 2:
		if (Opening_timer == 1) {
			Xa_play(1, 7);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0xc;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x1b6) {
			goto next_timer;
		}
		Opening_vis_mask = 0xc;
		Opening_vis_set(0xb, 0);
		next_seq = 3;
		break;
	case 3:
		if (Opening_timer == 10) {
			Xa_play(1, 8);
		}
		if (Opening_timer == 0x14) {
			Opening_vis_mask = 0x30;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x8c) {
			goto next_timer;
		}
		Opening_vis_mask = 0x30;
		Opening_vis_set(0xb, 0);
		next_seq = 4;
		break;
	case 4:
		if (Opening_timer == 1) {
			Xa_play(1, 9);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0xc0;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x142) {
			goto next_timer;
		}
		Opening_vis_mask = 0xc0;
		Opening_vis_set(0xb, 0);
		Memcpy(Bg_img_buf, Tex_tim_buf, 0x25800);
		Opening_bg_fade_in();
		next_seq = 5;
		break;
	case 5:
		if (Opening_timer == 0x80) {
			Xa_play(1, 0xe);
		}
		if (Opening_timer != 0x151) {
			goto next_timer;
		}
		Opening_bg_fade_out();
		Opening_bg_flg = 8;
		next_seq = 6;
		break;
	case 6:
		if (Opening_timer == 0x5a) {
			Xa_play(1, 10);
		}
		if (Opening_timer == 100) {
			Opening_vis_mask = 0x300;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x150) {
			goto next_timer;
		}
		Opening_vis_mask = 0x300;
		Opening_vis_set(0xb, 0);
		next_seq = 7;
		break;
	case 7:
		if (Opening_timer == 1) {
			Xa_play(1, 0xb);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0xc00;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer != 0x10a) {
			goto next_timer;
		}
		Opening_vis_mask = 0xc00;
		Opening_vis_set(0xb, 0);
		next_seq = 8;
		break;
	case 8:
		if (Opening_timer == 1) {
			Xa_play(1, 0xc);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0x3000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x1b9) {
			Opening_vis_mask = 0x3000;
			Opening_vis_set(0xb, 0);
			Memcpy(Bg_img_buf, Opening_scene_tim_buf, 0x25800);
			Opening_bg_fade_in();
		}
		if (Opening_timer == 0x1c3) {
			Opening_vis_mask = 0xc000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x2e2) {
			Opening_vis_mask = 0xc000;
			Opening_vis_set(0xb, 0);
		}
		next_seq = 9;
		if (Opening_timer != 0x300) {
			goto next_timer;
		}
		break;
	case 9:
		if (Opening_timer == 1) {
			Xa_play(1, 0xd);
		}
		if (Opening_timer == 0xb) {
			Opening_vis_mask = 0x30000;
			Opening_vis_set(7, 0);
		}
		if (Opening_timer == 0x181) {
			Opening_vis_mask = 0x30000;
			Opening_vis_set(0xb, 0);
		}
		if (Opening_timer == 0x253) {
			Snd_bgm_fade_ON(0x78, 0x18);
			Opening_bg_flg = 8;
			Opening_fade_no = 0;
			Opening_bg_fade_out();
			Opening_skip_ok = 0;
			Opening_trans_phase = 2;
		}
		if ((Opening_timer < 0x254) || (Fade_status(Opening_fade_no) == 0)) {
			goto next_timer;
		}
		Opening_vis_mask = 0;
		Opening_vis_set(0, 1);
		Opening_skip_ok = 1;
		next_seq = 10;
		break;
	case 10:
		if ((Opening_timer != 100) || (pEdt_adr[0].id_check != 0)) {
			goto next_timer;
		}
		G.Status_flg &= ~STAT_CINESCO;
		Init_movie_work(3);
		next_seq = 0xb;
		break;
	case 0xb:
		next_seq = 0xc;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 0xc:
		if (Opening_timer != 1) {
			goto next_timer;
		}
		Init_movie_work(0x10);
		next_seq = 0xd;
		break;
	case 0xd:
		next_seq = 0xe;
		if ((G.System_flg & SYS_STR_MOVIE) != 0) {
			goto next_timer;
		}
		break;
	case 0xe:
		if (Opening_timer == 10) {
			Opening_draw_mode = 3;
			Opening_scene_no = 4;
		}
		goto next_timer;
	default:
		goto next_timer;
	}
	Opening_timer = 0;
	Opening_seq_no = next_seq;
next_timer:
	Opening_timer++;
}

/* DS 0x801c1a0c (Opening_sprite_draw) */
VOID Opening_sprite_draw(VOID)
{
	OP_WORK *pSpr;
	OP_MOJI_TBL *pTbl;
	POLY_FT4 *p;
	INT count;
	ULONG code_rgb;
	UCHAR flg;
	UCHAR type;
	INT x;
	INT y;
	INT w;
	INT h;
	INT tpage_add;
	UCHAR type_hi;
	BOOL first;

	/* scroll and blink phase counters (2ee0 / 2ee2) */
	if (Opening_scroll_cnt == 0) {
		Opening_scroll_cnt = 10;
		Opening_scroll_dy = -1;
	}
	else {
		Opening_scroll_cnt--;
		Opening_scroll_dy = 0;
	}
	first = (Opening_blink_cnt == 0);
	Opening_blink_cnt--;
	if (first) {
		Opening_blink_cnt = 3;
		Opening_blink_dy = -1;
	}
	else {
		Opening_blink_dy = 0;
	}

	count = (INT)Opening_sprite_num;
	pSpr = Opening_sprite_cur;
	p = Opening_prim[G.Cbuf_id];
	do {
		count--;
		if ((pSpr->type & 0x10) != 0) {
			pSpr->flg = 0;
		}
		flg = pSpr->flg;
		type = pSpr->type;
		if ((flg & 1) != 0) {
			type_hi = type >> 4;
			if (type_hi == 1) {
				/* picture: fixed grey, moved by x/y directly */
				pSpr->rgb = 0x00808080;
				pTbl = &Opening_pict_tbl[pSpr->no];
				if ((pSpr->flg & 8) != 0) {
					pSpr->flg = 0;
				}
				tpage_add = 0x20;
				x = pSpr->x;
				y = pSpr->y;
			}
			else if (type_hi == 0) {
				/* moji: fade in / fade out on the rgb value, optional scroll */
				pTbl = &Opening_moji_tbl[pSpr->no];
				if ((pSpr->flg & 4) != 0) {
					pSpr->rgb += 0x20202;
					if ((INT)pSpr->rgb > 0x60605f) {
						pSpr->rgb = 0x00606060;
						pSpr->flg &= 0xf9;
					}
				}
				if ((pSpr->flg & 8) != 0) {
					pSpr->rgb -= 0x20202;
					if ((INT)pSpr->rgb < 1) {
						pSpr->rgb = 0;
						pSpr->flg &= 0xf7;
					}
				}
				tpage_add = 0xa0;
				if ((pSpr->flg & 0x10) != 0) {
					pSpr->y += Opening_scroll_dy;
				}
				x = pSpr->x;
				y = pSpr->y;
			}
			else if (type_hi == 2) {
				/* zoom moji: fade on rgb, w/h scaled by blink_dy, centred on screen */
				pTbl = &Opening_moji_tbl[pSpr->no];
				if ((pSpr->flg & 4) != 0) {
					pSpr->rgb += 0x20202;
					if ((INT)pSpr->rgb > 0x60605f) {
						pSpr->rgb = 0x00606060;
						pSpr->flg &= 0xf9;
					}
				}
				if ((pSpr->flg & 8) != 0) {
					pSpr->rgb -= 0x10101;
					if ((INT)pSpr->rgb < 1) {
						pSpr->rgb = 0;
						pSpr->flg &= 0xf7;
					}
				}
				tpage_add = 0xa0;
				if ((pSpr->flg & 0x20) != 0) {
					pTbl->w = pTbl->w + (CHAR)Opening_blink_dy * -2;
					pTbl->h = pTbl->h + (CHAR)Opening_blink_dy * -2;
				}
				if ((pSpr->flg & 0x40) != 0) {
					pTbl->w = pTbl->w + (CHAR)Opening_blink_dy * 2;
					pTbl->h = pTbl->h + (CHAR)Opening_blink_dy * 2;
				}
				x = (0x140 - (INT)pTbl->w) / 2;
				y = (0xf0 - (INT)pTbl->h) / 2;
			}
			else {
				/* types 3..15: the binary branches straight to the draw block (801c1e28)
				 * with the table pointer left in s5 by the previous sprite. That stale
				 * pointer cannot be reproduced in C, so these sprites are skipped here. */
				goto next_spr;
			}

			code_rgb = (ULONG)pSpr->rgb + 0x2c000000;
			if (type_hi == 0 || type_hi == 2) {
				code_rgb = (ULONG)pSpr->rgb + 0x2e000000;
			}
			w = pTbl->w;
			h = pTbl->h;

			SetPolyFT4(p);
			*(ULONG *)&p->r0 = code_rgb;
			p->x0 = (SHORT)x;
			p->y0 = (SHORT)y;
			p->u0 = pTbl->u;
			p->v0 = pTbl->v;
			p->clut = pTbl->clut;
			p->x1 = (SHORT)(x + w);
			p->y1 = (SHORT)y;
			p->u1 = pTbl->u + pTbl->uw;
			p->v1 = pTbl->v;
			p->tpage = pTbl->tpage + tpage_add;
			p->x2 = (SHORT)x;
			p->y2 = (SHORT)(y + h);
			p->u2 = pTbl->u;
			p->v2 = pTbl->v + pTbl->vh;
			p->x3 = (SHORT)(x + w);
			p->y3 = (SHORT)(y + h);
			p->u3 = pTbl->u + pTbl->uw;
			p->v3 = pTbl->v + pTbl->vh;
			AddPrim(G.pOt1 + pSpr->ot, p);
			p++;
		}
next_spr:
		pSpr++;
	} while (count != 0);
}

/* DS 0x801c1f64 (Opening_vis_set) */
VOID Opening_vis_set(UCHAR Flg, CHAR Clr_sw)
{
	UINT i;
	OP_WORK *pSpr;

	pSpr = Opening_sprite_cur;
	for (i = 0; i < (UINT)(INT)Opening_sprite_num; i++) {
		if (((Opening_vis_mask >> (i & 0x1f)) & 1) == 0) {
			if (Clr_sw != 0) {
				pSpr[i].flg = 0;
			}
		}
		else {
			pSpr[i].flg = Flg;
		}
		if ((pSpr[i].type & 0x10) != 0) {
			pSpr[i].flg = 0;
		}
	}
}

/* DS 0x801c1fe0 (Opening_fade_set) */
VOID Opening_fade_set(INT Fade_no, USHORT Ctrl, ULONG Col, ULONG *pPos)
{
	OP_FADE *pF;
	ULONG wh_pack;
	ULONG rgb_code;

	pF = (OP_FADE *)((UCHAR *)Main.Fade + Fade_no * sizeof(OP_FADE));
	rgb_code = Col | ((ULONG)pF->T[0].code << 24);
	pF->Kido = (SHORT)Ctrl;
	*(ULONG *)&pF->T[0].r0 = rgb_code;
	*(ULONG *)&pF->T[1].r0 = rgb_code;
	if (pPos == NULL) {
		wh_pack = 0x00f00140;
		*(ULONG *)&pF->T[0].x0 = 0;
		*(ULONG *)&pF->T[0].w = 0x00f00140;
		*(ULONG *)&pF->T[1].x0 = 0;
	}
	else {
		*(ULONG *)&pF->T[0].x0 = pPos[0];
		*(ULONG *)&pF->T[0].w = pPos[1];
		*(ULONG *)&pF->T[1].x0 = pPos[0];
		wh_pack = pPos[1];
	}
	*(ULONG *)&pF->T[1].w = wh_pack;
}
