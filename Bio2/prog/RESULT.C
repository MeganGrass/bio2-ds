/*
 * RESULT.C - Result screen task: score, rank and text display
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\RESULT.C
 */
#include "TYPES.H"
#include "MAIN.H"
#include "CONFIG.H"
#include "SLD.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include <MALLOC.H>
#include "RESULT.H"
#include "CD.H"
#include "DIESPR.H"
#include "MAIN.H"
#include "MONITOR.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x80159A48 (Result_prim) */
POLY_FT4 (*Result_prim)[128] = (POLY_FT4 (*)[128])0x80159A48;

/* Result overlay work table (declared extern in RESULT.H) */

/* DS 0x80158000 (Result_mess_bg_tim) */
UCHAR Result_mess_bg_tim[] = {
	0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x00, 0x00,	/* "Result" + 2 NUL */
};

/* DS 0x80158008 (Result_mess_moji_tim) */
UCHAR Result_mess_moji_tim[] = {
	0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x20, 0x4d,
	0x6f, 0x6a, 0x69, 0x00,	/* "Result Moji" + NUL */
};

/* DS 0x80158014 (Result_mess_init_tbl) */
UCHAR Result_mess_init_tbl[] = {
	0x49, 0x6e, 0x69, 0x74, 0x20, 0x74, 0x62, 0x6c,
	0x00,	/* "Init tbl" + NUL */
};

/* DS 0x8015981C (Result_str_buf) */
UCHAR Result_str_buf[164] = {
	0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a,
	0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a,
	0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27,
	0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a,
	0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a,
	0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27,
	0x1a, 0x1a, 0xff, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0x27, 0x1a, 0x1a, 0xff, 0x0b, 0x04, 0x0e, 0x0d,
	0xff, 0x02, 0x0b, 0x00, 0x08, 0x11, 0x04, 0xff, 0x00, 0x03, 0x00, 0xff, 0x02, 0x07, 0x11, 0x08,
	0x12, 0xff, 0x0d, 0x0e, 0x29, 0x03, 0x00, 0x13, 0x00, 0xff, 0x0b, 0x15, 0x1b, 0xff, 0x0b, 0x15,
	0x1c, 0xff, 0x0b, 0x15, 0x1d, 0xff, 0x2e, 0xff, 0x2f, 0xff, 0x30, 0xff, 0x31, 0xff, 0x3d, 0xff,
	0x3e, 0xff, 0x41, 0xff,
};

/* DS 0x801598C0 (Result_str_ofs) */
USHORT Result_str_ofs[28] = {
	0x0000, 0x0009, 0x0012, 0x001b, 0x0024, 0x002d, 0x0036, 0x003f,
	0x0048, 0x0051, 0x005a, 0x0063, 0x006c, 0x0071, 0x0078, 0x007c,
	0x0082, 0x008a, 0x008e, 0x0092, 0x0096, 0x0098, 0x009a, 0x009c,
	0x009e, 0x00a0, 0x00a2, 0x0000,
};

/* DS 0x801598F8 (Result_row_y) */
USHORT Result_row_y[3] = {
	0x0060, 0x003c, 0x0030,
};

/* DS 0x80159900 (Result_row_y2) */
USHORT Result_row_y2[3] = {
	0x0000, 0x0092, 0x006a,
};

/* DS 0x80159908 (Result_time_ofs) */
USHORT Result_time_ofs[3][4] = {
	{ 0x0012, 0x0024, 0x0030, 0x003a },
	{ 0x000f, 0x001e, 0x0028, 0x0030 },
	{ 0x000c, 0x0018, 0x0020, 0x0026 },
};

/* DS 0x80159920 (Result_glyph) */
UCHAR Result_glyph[66][4] = {
	{ 0x00, 0x00, 0x0d, 0x0e }, { 0x0f, 0x00, 0x0d, 0x0e }, { 0x1d, 0x00, 0x0d, 0x0e }, { 0x2b, 0x00, 0x0d, 0x0e },
	{ 0x3a, 0x00, 0x0b, 0x0e }, { 0x47, 0x00, 0x0b, 0x0e }, { 0x54, 0x00, 0x0e, 0x0e }, { 0x71, 0x20, 0x0c, 0x0e },
	{ 0x66, 0x00, 0x06, 0x0e }, { 0x80, 0x20, 0x0a, 0x0e }, { 0x70, 0x00, 0x0d, 0x0e }, { 0x7f, 0x00, 0x0b, 0x0e },
	{ 0x8c, 0x00, 0x0e, 0x0e }, { 0x9a, 0x00, 0x0d, 0x0e }, { 0xa8, 0x00, 0x0e, 0x0e }, { 0xb7, 0x00, 0x0b, 0x0e },
	{ 0x8c, 0x20, 0x0e, 0x0e }, { 0xc5, 0x00, 0x0d, 0x0e }, { 0xd4, 0x00, 0x0a, 0x0e }, { 0xe1, 0x00, 0x0c, 0x0e },
	{ 0xee, 0x00, 0x0d, 0x0e }, { 0x00, 0x0e, 0x0e, 0x0e }, { 0x0e, 0x0e, 0x0e, 0x0e }, { 0x1c, 0x0e, 0x0e, 0x0e },
	{ 0x2a, 0x0e, 0x0e, 0x0e }, { 0x9c, 0x20, 0x0a, 0x0e }, { 0x38, 0x0e, 0x0e, 0x0e }, { 0x46, 0x0e, 0x0e, 0x0e },
	{ 0x54, 0x0e, 0x0e, 0x0e }, { 0x62, 0x0e, 0x0e, 0x0e }, { 0x70, 0x0e, 0x0e, 0x0e }, { 0x7e, 0x0e, 0x0e, 0x0e },
	{ 0x8c, 0x0e, 0x0e, 0x0e }, { 0x9a, 0x0e, 0x0e, 0x0e }, { 0xa8, 0x0e, 0x0e, 0x0e }, { 0xb6, 0x0e, 0x0e, 0x0e },
	{ 0xc4, 0x0e, 0x0a, 0x0e }, { 0xd5, 0x0e, 0x08, 0x0e }, { 0xe0, 0x0e, 0x0e, 0x0e }, { 0xf0, 0x0e, 0x0a, 0x0e },
	{ 0x98, 0x20, 0x0a, 0x0e }, { 0xf1, 0x20, 0x08, 0x0e }, { 0x00, 0x20, 0x2a, 0x0e }, { 0x38, 0x20, 0x2a, 0x0e },
	{ 0x00, 0x2e, 0x2a, 0x0e }, { 0x2a, 0x30, 0x2a, 0x0e }, { 0x00, 0x4c, 0x68, 0x24 }, { 0x88, 0x50, 0x68, 0x14 },
	{ 0x54, 0x30, 0x8c, 0x08 }, { 0x00, 0x70, 0xc8, 0x18 }, { 0x01, 0x3e, 0x0b, 0x0b }, { 0x0e, 0x3e, 0x0b, 0x0b },
	{ 0x1d, 0x3e, 0x0b, 0x0b }, { 0x2c, 0x3e, 0x0b, 0x0b }, { 0x38, 0x3e, 0x0b, 0x0b }, { 0x47, 0x3e, 0x0b, 0x0b },
	{ 0x54, 0x3e, 0x0b, 0x0b }, { 0x62, 0x3e, 0x0b, 0x0b }, { 0x70, 0x3e, 0x0b, 0x0b }, { 0x7f, 0x3e, 0x0b, 0x0b },
	{ 0xf0, 0x10, 0x0a, 0x0c }, { 0x00, 0x88, 0x50, 0x0c }, { 0x50, 0x88, 0x48, 0x0c }, { 0xa6, 0x20, 0x0d, 0x0e },
	{ 0x8c, 0x3e, 0x0b, 0x0b }, { 0x00, 0x98, 0x99, 0x14 },
};

/* DS 0x80159A28 (Result_state_tbl) */
VOID *Result_state_tbl[4] = {
	(VOID *)Result_init,	/* 0: RESULT::801580cc */
	(VOID *)Result_fade_wait,	/* 1: RESULT::801581ac */
	(VOID *)Result_main,	/* 2: RESULT::801581dc */
	(VOID *)Result_exit,	/* 3: RESULT::801583e0 */
};

/* DS 0x80159A38 (Result_sel_tbl) */
VOID *Result_sel_tbl[4] = {
	(VOID *)Result_sel_rtn,	/* 0: RESULT::80158218 */
	(VOID *)Result_rank_main,	/* 1: RESULT::80158240 */
	(VOID *)Result_score_main,	/* 2: RESULT::8015829c */
	(VOID *)Result_bgm_end,	/* 3: RESULT::80158310 */
};


/* DS 0x8015C248 (Result_timer) */
SHORT Result_timer;

/* DS 0x8015C24A (Result_state) */
UCHAR Result_state;

/* DS 0x8015C24B (Result_sel) */
UCHAR Result_sel;

/* DS 0x8015C250 (Result_moji) */
RESULT_MOJI Result_moji[32];

/* Result screen task. Result_state_tbl runs one state per frame (init, fade wait, main, BGM end, exit)
 * and the loop stops when the state reaches 4. The task holds slot 0 suspended while it runs. */
/* DS 0x80158020 (Result_task) */
VOID Result_task(VOID)
{
	VOID **tbl = Result_state_tbl;
	UCHAR *state_p = &Result_state;

	*state_p = 0;
	Result_sel = 0;
	Result_timer = 0;
	Task_suspend(0);
	do {
		((VOID (*)(VOID))tbl[*state_p])();
		Result_moji_draw();
		Task_sleep(1);
	} while (*state_p < 4);
	Task_signal(0);
	Task_exit();
}

/* DS 0x801580cc (Result_init) */
VOID Result_init(VOID)
{
	ULONG i;

	Main.Vloop = 0;
	/* clears 32 entries from 0x8015C250 to 0x8015C3C4 (the last one is past Result_moji) */
	for (i = 0; i <= 31; i++) {
		Result_moji[i].Flg = 0;
	}
	Bg_set_mode(2, 0);
	Cd_read(RESULT_TIM, Bg_img_buf, 0, Result_mess_bg_tim);
	Task_sleep(1);
	Cd_read(RES_MOJI_TIM, Tex_tim_buf, 0, Result_mess_moji_tim);
	G.Ctpage = 0x15;
	G.Cclut = 0x1b;
	Mapping_tim(Tex_tim_buf);
	Task_sleep(1);
	Fade_set(0x200, -0x200, 7, 1);
	Main.Scrl_type = 1;
	Bg_set_mode(0, 0);
	Result_state = 1;
}

/* DS 0x801581ac (Result_fade_wait) */
VOID Result_fade_wait(VOID)
{
	if (Fade_status(0) != 0) {
		Result_state = 2;
	}
}

/* DS 0x801581dc (Result_main) */
VOID Result_main(VOID)
{
	((VOID (*)(VOID))Result_sel_tbl[Result_sel])();
}

/* DS 0x80158218 (Result_sel_rtn) */
VOID Result_sel_rtn(VOID)
{
	RESULT_EX *pEx;

	pEx = (RESULT_EX *)G.ExData;
	Result_sel = 2;
	if (pEx->Valid == 0) {
		Result_sel = 1;
	}
}

/* Rank page: once the rank timeline has finished, a confirm press (pad mask 0x8e0) fades out and moves
 * to state 3. */
/* DS 0x80158240 (Result_rank_main) */
VOID Result_rank_main(VOID)
{
	ULONG done;

	done = Result_rank_timeline();
	if (done != 0 && (G.Padd1_trg & 0x8e0) != 0) {
		Fade_set(0x200, 0x400, 7, 1);
		Result_state = 3;
	}
}

/* Score page: after the score timeline finishes, a confirm press (pad mask 0x8e0) starts the result BGM fade
 * and fades the screen, then waits 0x5a frames (Result_timer) in selection 3. */
/* DS 0x8015829c (Result_score_main) */
VOID Result_score_main(VOID)
{
	ULONG done;

	done = Result_score_timeline();
	if (done != 0 && (G.Padd1_trg & 0x8e0) != 0) {
		Snd_bgm_fade_ON(0x5a, 0x18);
		Fade_set(0x200, 0x400, 7, 1);
		Result_sel = 3;
		Result_timer = 0x5a;
	}
}

/* Wait for the fade, clear the Result_moji slots, then count Result_timer down. When the timer runs out
 * the BGM sequencer is reset to its defaults (main and sub bank 0xff, full volume) and the state moves to 3. */
/* DS 0x80158310 (Result_bgm_end) */
VOID Result_bgm_end(VOID)
{
	SHORT old;
	RESULT_MOJI *pMoji = &Result_moji[31];

	if (Fade_status(0) != 0) {
		for (; pMoji >= Result_moji; pMoji--) {
			pMoji->Flg = 0;
		}
		Bg_set_mode(2, 0);
	}
	old = Result_timer;
	Result_timer = old - 1;
	if (old <= 0) {
		G.pFree_work = RESULT_FREE_WORK_BASE;
		pBgm->Main = 0xff;
		pBgm->Sub = 0xff;
		Seq_ctr[0].Ctrl = 0;
		Seq_ctr[0].Vol.left = 100;
		Seq_ctr[0].Vol.right = 100;
		Snd_bgm_main();
		Result_state = 3;
	}
}

/* DS 0x801583e0 (Result_exit) */
VOID Result_exit(VOID)
{
	RESULT_EX *pEx;
	ULONG i;

	pEx = (RESULT_EX *)G.ExData;
	if (Fade_status(0) != 0) {
		if (pEx->Valid != 0) {
			Bg_set_mode(2, 0);
		}
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x7fff, 0xffffff, NULL);
		if (pEx->Valid == 0) {
			for (i = 0; i <= 31; i++) {
				Result_moji[i].Flg = 0;
			}
		}
		else {
			Result_inittbl_load();
			pEx->Valid = 0;
			G.SaveData.Save_cnt = 0xffff;
			G.SaveData.Dummy[1] = pEx->Variant;
			G.SaveData.Save_area = pEx->Variant + 0x12;
			G.SaveData.Sce_timer = 3;
			if (pEx->Cur_rec == 1) {
				G.SaveData.Sce_timer = 2;
			}
			G.Player.Id = 0x10;
			Main.Scrl_type = 0;
			G.System_flg |= SYS_ENDING;
			Task_lexecute(1, 1);
			Task_sleep(1);
			Main.Scrl_type = 1;
			G.System_flg &= ~SYS_ENDING;
		}
		Result_state = 4;
	}
}

/* Claims the first free entry (or the one-past-end slot at 0x8015C3C4 when the
 * table is full, as the original does) and fills it from X, Y, string and mode. */
/* DS 0x8015855c (Result_moji_set) */
VOID Result_moji_set(SHORT X, SHORT Y, UCHAR Str_no, ULONG Mode)
{
	RESULT_MOJI *pMoji;

	pMoji = Result_moji;
	do {
		if (pMoji->Flg == 0) {
			break;
		}
		pMoji++;
	} while (pMoji != &Result_moji[31]);

	pMoji->Ot_no = (UCHAR)(Mode >> 8);
	pMoji->Dummy = (UCHAR)(Mode >> 16);
	pMoji->X = X;
	pMoji->Y = Y;
	pMoji->Str_no = Str_no;
	pMoji->Rgb = 0x00808080;
	pMoji->Flg |= RESULT_MOJI_USE;

	if ((Mode & 0x80) == 0) {
		if ((Mode & 1) != 0) {
			pMoji->Flg |= RESULT_MOJI_F1;
		}
		if ((Mode & 2) != 0) {
			pMoji->Rgb = 0;
			pMoji->Flg |= RESULT_MOJI_FADE_IN;
		}
		else if ((Mode & 4) != 0) {
			pMoji->Flg |= RESULT_MOJI_FADE_OUT;
		}
		else if ((Mode & 8) != 0) {
			pMoji->Rgb = 0;
			pMoji->Flg |= RESULT_MOJI_FADE_IN | RESULT_MOJI_FAST;
		}
	}
}

/* Ramps each active entry's colour, then emits one POLY_FT4 per glyph into
 * Result_prim[G.Cbuf_id] and adds it to the OT at G.pOt1 + Ot_no. */
/* DS 0x80158634 (Result_moji_draw) */
VOID Result_moji_draw(VOID)
{
	/* retail reserves 32 more bytes of locals (Result_moji_draw frame 104); never read */
	volatile int Rsv[8];
	RESULT_MOJI *pMoji;
	POLY_FT4 *pPrim;
	UCHAR *pStr;
	ULONG code;
	ULONG rgb;
	ULONG semi;
	LONG v;
	UCHAR flg;
	SHORT x;
	SHORT y;
	SHORT gw;
	SHORT gh;
	UCHAR gu;
	UCHAR gv;

	pPrim = Result_prim[G.Cbuf_id];
	for (pMoji = Result_moji; pMoji != &Result_moji[31]; pMoji++) {
		/* the original spins forever on a nonzero entry without bit 0; such entries are skipped here */
		if ((pMoji->Flg & RESULT_MOJI_USE) == 0) {
			continue;
		}

		code = 0x2c000000;
		if ((pMoji->Flg & RESULT_MOJI_FADE_OUT) != 0) {
			code = 0x2e000000;
			if ((pMoji->Flg & RESULT_MOJI_FAST) == 0) {
				v = (LONG)pMoji->Rgb - 0x101010;
				pMoji->Rgb = v;
				if (v < 0x80808) {
					pMoji->Rgb = 0;
					pMoji->Flg = 0;
					continue;
				}
			}
			else {
				v = (LONG)pMoji->Rgb - 0xc0c0c;
				pMoji->Rgb = v;
				if (v < 0x404040) {
					pMoji->Flg = (pMoji->Flg & 0xf7) | RESULT_MOJI_FADE_IN;
				}
			}
		}

		if ((pMoji->Flg & RESULT_MOJI_FADE_IN) != 0) {
			code = 0x2e000000;
			if ((pMoji->Flg & RESULT_MOJI_FAST) == 0) {
				v = (LONG)pMoji->Rgb + 0x80808;
			}
			else {
				v = (LONG)pMoji->Rgb + 0x40404;
			}
			pMoji->Rgb = v;
			if (v > 0x80807f) {
				flg = pMoji->Flg;
				pMoji->Flg = (flg & 0xfb) | RESULT_MOJI_HOLD;
				if ((flg & RESULT_MOJI_FAST) == 0) {
					pMoji->Rgb = 0x00808080;
				}
				else {
					pMoji->Flg |= RESULT_MOJI_FADE_OUT;
				}
				code = 0x2c000000;
			}
		}

		rgb = pMoji->Rgb;
		if ((pMoji->Flg & RESULT_MOJI_FADE_IN) != 0) {
			semi = 1;
		}
		else {
			semi = (pMoji->Flg >> 1) & 1;
		}

		pStr = &Result_str_buf[Result_str_ofs[pMoji->Str_no]];
		x = pMoji->X;
		y = pMoji->Y;
		for (; *pStr != 0xff; pStr++) {
			gu = Result_glyph[*pStr][0];
			gv = Result_glyph[*pStr][1];
			gw = Result_glyph[*pStr][2];
			gh = Result_glyph[*pStr][3];

			SetPolyFT4(pPrim);
			pPrim->x0 = x;
			pPrim->y0 = y;
			pPrim->x1 = x + gw;
			pPrim->y1 = y;
			pPrim->x2 = x;
			pPrim->y2 = y + gh;
			pPrim->x3 = x + gw;
			pPrim->y3 = y + gh;
			pPrim->u0 = gu;
			pPrim->v0 = gv;
			pPrim->u1 = gu + gw;
			pPrim->v1 = gv;
			pPrim->u2 = gu;
			pPrim->v2 = gv + gh;
			pPrim->u3 = gu + gw;
			pPrim->v3 = gv + gh;
			pPrim->clut = GetClut(0, 0x1fb);
			pPrim->tpage = GetTPage(1, 0, 0x140, 0x100);
			*(ULONG *)&pPrim->r0 = code + rgb;
			SetSemiTrans(pPrim, semi);
			AddPrim(G.pOt1 + pMoji->Ot_no, pPrim);

			x += gw;
			pPrim++;
		}
	}
}

/* Returns 1 once Result_timer passes 0x5A. */
/* DS 0x801589e4 (Result_rank_timeline) */
ULONG Result_rank_timeline(VOID)
{
	/* retail reserves 8 more bytes of locals (Result_rank_timeline frame 32); never read */
	volatile int Rsv[2];
	ULONG over;


	if (Result_timer == 0x1e) {
		Result_rank_name_set();
	}
	else if (Result_timer < 0x1f) {
		if (Result_timer == 0) {
			Result_moji_set(0x6c, 8, 0x14, 0x100);
		}
	}
	else if (Result_timer == 0x3c) {
		Result_rank_time_set();
	}
	over = (Result_timer > 0x5a);
	if (over == 0) {
		Result_timer++;
	}
	return over;
}

/* DS 0x80158a9c (Result_rank_name_set) */
VOID Result_rank_name_set(VOID)
{
	RESULT_EX *pEx;
	UCHAR name[3];
	UCHAR label[3];
	ULONG variant;
	ULONG i;
	SHORT y0;
	SHORT y1;

	pEx = (RESULT_EX *)G.ExData;
	for (i = 0; i < 3; i++) {
		name[i] = pEx->Rec[i].Rank + 0xc;
		label[i] = (UCHAR)(i + 0x11);
	}

	variant = pEx->Variant;
	if (variant > 2) {
		return;
	}
	y0 = Result_row_y[variant];
	y1 = Result_row_y2[variant];

	switch (variant) {
	case 2:
		if (pEx->Rec[2].Recorded != 0) {
			Result_moji_set(0x20, 0xb0, name[2], 0x102);
		}
		Result_moji_set(0x20, 0xa0, label[2], 0x102);
		/* fall through */
	case 1:
		if (pEx->Rec[1].Recorded != 0) {
			Result_moji_set(0x20, y1 + 0x10, name[1], 0x102);
		}
		Result_moji_set(0x20, y1, label[1], 0x102);
		/* fall through */
	case 0:
		if (pEx->Rec[0].Recorded != 0) {
			Result_moji_set(0x20, y0 + 0x10, name[0], 0x102);
		}
		Result_moji_set(0x20, y0, label[0], 0x102);
		break;
	}
}

/* DS 0x80158bfc (Result_rank_time_set) */
VOID Result_rank_time_set(VOID)
{
	RESULT_EX *pEx;
	ULONG sum[3];
	USHORT off[4];
	ULONG variant;
	ULONG i;
	SHORT y0;
	SHORT y1;

	pEx = (RESULT_EX *)G.ExData;
	for (i = 0; i < 3; i++) {
		sum[i] = pEx->Rec[i].Time[0] + pEx->Rec[i].Time[1] + pEx->Rec[i].Time[2];
	}

	variant = pEx->Variant;
	if (variant > 2) {
		return;
	}
	for (i = 0; i < 4; i++) {
		off[i] = Result_time_ofs[variant][i];
	}
	y0 = Result_row_y[variant];
	y1 = Result_row_y2[variant];

	if (variant == 2) {
		Result_moji_set(0xac, 0xa2, (UCHAR)Result_time_moji_set(pEx->Rec[2].Time[0], 8), 0x102);
		Result_moji_set(0xac, 0xae, (UCHAR)Result_time_moji_set(pEx->Rec[2].Time[1], 9), 0x102);
		Result_moji_set(0xac, 0xba, (UCHAR)Result_time_moji_set(pEx->Rec[2].Time[2], 10), 0x102);
		Result_moji_set(0x92, 0xc2, 0x16, 0x102);
		Result_moji_set(0xa0, 200, (UCHAR)Result_time_moji_set(sum[2], 11), 0x102);
	}
	if (variant >= 1) {
		Result_moji_set(0xac, y1, (UCHAR)Result_time_moji_set(pEx->Rec[1].Time[0], 4), 0x102);
		Result_moji_set(0xac, y1 + off[0], (UCHAR)Result_time_moji_set(pEx->Rec[1].Time[1], 5), 0x102);
		Result_moji_set(0xac, y1 + off[1], (UCHAR)Result_time_moji_set(pEx->Rec[1].Time[2], 6), 0x102);
		Result_moji_set(0x92, y1 + off[2], 0x16, 0x102);
		Result_moji_set(0xa0, y1 + off[3], (UCHAR)Result_time_moji_set(sum[1], 7), 0x102);
	}
	Result_moji_set(0xac, y0, (UCHAR)Result_time_moji_set(pEx->Rec[0].Time[0], 0), 0x102);
	Result_moji_set(0xac, y0 + off[0], (UCHAR)Result_time_moji_set(pEx->Rec[0].Time[1], 1), 0x102);
	Result_moji_set(0xac, y0 + off[1], (UCHAR)Result_time_moji_set(pEx->Rec[0].Time[2], 2), 0x102);
	Result_moji_set(0x92, y0 + off[2], 0x16, 0x102);
	Result_moji_set(0xa0, y0 + off[3], (UCHAR)Result_time_moji_set(sum[0], 3), 0x102);
}

/* Returns 1 once Result_timer passes 0xC8. Text is placed by timer value. */
/* DS 0x80158ef4 (Result_score_timeline) */
ULONG Result_score_timeline(VOID)
{
	/* retail reserves 8 more bytes of locals (Result_score_timeline frame 56); never read */
	volatile int Rsv[2];
	RESULT_EX *pEx;
	RESULT_REC *pRec;
	ULONG rec_total;
	ULONG run_total;
	UCHAR mode_glyph;
	ULONG over;
	UCHAR cur_rec;
	UCHAR rank;

	pEx = (RESULT_EX *)G.ExData;
	pRec = &pEx->Rec[(UCHAR)(pEx->Cur_rec - 1)];
	cur_rec = pEx->Cur_rec;
	rank = pRec->Rank;
	rec_total = pRec->Time[0] + pRec->Time[1] + pRec->Time[2];
	run_total = pEx->Run[0] + pEx->Run[1] + pEx->Run[2];

	/* the original leaves this register unset when Mode > 3; 0x0c is used here */
	mode_glyph = 0x0c;
	if (pEx->Mode == 1) {
		mode_glyph = 0x0d;
	}
	else if (pEx->Mode == 2) {
		mode_glyph = 0x0e;
	}
	else if (pEx->Mode == 3) {
		mode_glyph = 0x0f;
	}

	if (Result_timer == 0x5a) {
		Result_moji_set(0xac, 0x50, (UCHAR)Result_time_moji_set(pRec->Time[2], 2), 0x102);
		Result_moji_set(0xac, 0xa4, (UCHAR)Result_time_moji_set(pEx->Run[2], 6), 0x102);
	}
	else if (Result_timer < 0x5b) {
		if (Result_timer == 0x1e) {
			Result_moji_set(0x20, 0x30, 0x18, 0x102);
			Result_moji_set(0x20, 0x40, cur_rec + 0x10, 0x102);
			if (pRec->Recorded != 0) {
				Result_moji_set(0x20, 0x50, rank + 0xc, 0x102);
			}
			Result_moji_set(0xac, 0x30, (UCHAR)Result_time_moji_set(pRec->Time[0], 0), 0x102);
			Result_moji_set(0x20, 0x84, 0x19, 0x102);
			Result_moji_set(0x20, 0x94, cur_rec + 0x10, 0x102);
			Result_moji_set(0x20, 0xa4, mode_glyph, 0x102);
			Result_moji_set(0xac, 0x84, (UCHAR)Result_time_moji_set(pEx->Run[0], 4), 0x102);
		}
		else if (Result_timer < 0x1f) {
			if (Result_timer == 0) {
				Result_moji_set(0x6c, 8, 0x14, 0x100);
			}
		}
		else if (Result_timer == 0x3c) {
			Result_moji_set(0xac, 0x40, (UCHAR)Result_time_moji_set(pRec->Time[1], 1), 0x102);
			Result_moji_set(0xac, 0x94, (UCHAR)Result_time_moji_set(pEx->Run[1], 5), 0x102);
		}
	}
	else if (Result_timer == 0x96) {
		Result_moji_set(0xa0, 0x62, (UCHAR)Result_time_moji_set(rec_total, 3), 0x102);
		Result_moji_set(0xa0, 0xb6, (UCHAR)Result_time_moji_set(run_total, 7), 0x10a);
	}
	else if (Result_timer > 0x96) {
		if (Result_timer == 0xb4) {
			if (pEx->Cur_rec == 3) {
				Result_moji_set(0x60, 0xcc, 0x1a, 0x109);
			}
			else if (pEx->Variant < pEx->Cur_rec) {
				Result_moji_set(0x40, 0xcc, 0x17, 0x109);
				pEx->Variant++;
			}
			if (run_total < rec_total || pRec->Recorded == 0) {
				Result_moji_set(0x28, 0xb4, 0x15, 0x109);
				pRec->Rank = pEx->Mode;
				pRec->Dummy = 0;
				pRec->Recorded = 1;
				pRec->Time[0] = pEx->Run[0];
				pRec->Time[1] = pEx->Run[1];
				pRec->Time[2] = pEx->Run[2];
			}
		}
	}
	else if (Result_timer == 0x78) {
		Result_moji_set(0x90, 0x5a, 0x16, 0x102);
		Result_moji_set(0x90, 0xae, 0x16, 0x102);
	}

	over = (Result_timer > 0xc8);
	if (over == 0) {
		Result_timer++;
	}
	return over;
}

/* Writes an "HH:MM:SS" glyph string (60 Hz frame count input) into string Slot
 * and returns Slot. Slots 3, 7 and 11 use the alternate glyph set. */
/* DS 0x8015934c (Result_time_moji_set) */
ULONG Result_time_moji_set(ULONG Time, ULONG Slot)
{
	UCHAR *pStr;
	ULONG hour;
	ULONG rem;
	ULONG min;
	ULONG sec;
	UCHAR special;

	pStr = &Result_str_buf[Result_str_ofs[Slot]];
	special = (Slot == 3 || Slot == 7 || Slot == 0xb);

	if (Time == 0) {
		if (special) {
			pStr[0] = 0x3f;
			pStr[1] = 0x3f;
			pStr[3] = 0x3f;
			pStr[4] = 0x3f;
			pStr[6] = 0x3f;
			pStr[7] = 0x3f;
		}
		else {
			pStr[0] = 0x40;
			pStr[1] = 0x40;
			pStr[2] = 0x3c;
			pStr[3] = 0x40;
			pStr[4] = 0x40;
			pStr[5] = 0x3c;
			pStr[6] = 0x40;
			pStr[7] = 0x40;
		}
		return Slot;
	}

	hour = (SHORT)(Time / 216000);
	if (hour < 100) {
		rem = Time - hour * 216000;
		min = rem / 3600;
		sec = (rem - min * 3600) / 60;
		if (special) {
			pStr[0] = (UCHAR)(hour / 10 + 0x1a);
			pStr[1] = (UCHAR)(hour % 10 + 0x1a);
			pStr[3] = (UCHAR)(min / 10 + 0x1a);
			pStr[4] = (UCHAR)(min % 10 + 0x1a);
			pStr[6] = (UCHAR)(sec / 10 + 0x1a);
			pStr[7] = (UCHAR)(sec % 10 + 0x1a);
		}
		else {
			pStr[0] = (UCHAR)(hour / 10 + 0x32);
			pStr[1] = (UCHAR)(hour % 10 + 0x32);
			pStr[2] = 0x3c;
			pStr[3] = (UCHAR)(min / 10 + 0x32);
			pStr[4] = (UCHAR)(min % 10 + 0x32);
			pStr[5] = 0x3c;
			pStr[6] = (UCHAR)(sec / 10 + 0x32);
			pStr[7] = (UCHAR)(sec % 10 + 0x32);
		}
	}
	else {
		if (special) {
			pStr[0] = 0x23;
			pStr[1] = 0x23;
			pStr[3] = 0x1f;
			pStr[4] = 0x23;
			pStr[6] = 0x1f;
			pStr[7] = 0x23;
		}
		else {
			pStr[0] = 0x3b;
			pStr[1] = 0x3b;
			pStr[3] = 0x37;
			pStr[4] = 0x3b;
			pStr[6] = 0x37;
			pStr[7] = 0x3b;
		}
	}
	return Slot;
}

/* DS 0x801597d0 (Result_inittbl_load) */
VOID Result_inittbl_load(VOID)
{
	Cd_read(INIT_TBL_DAT, Tex_tim_buf, 0, Result_mess_init_tbl);
	memcpy16((ULONG *)&G.SaveData, RESULT_SAVE_INIT_SRC, 0x798);
}
