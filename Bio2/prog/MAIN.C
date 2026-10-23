/*
 * MAIN.C - Main game loop and system module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MAIN.C
 */

#include "TYPES.H"
#include "CONFIG.H"
#include "ROOM.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBPAD.H>
#include <LIBPRESS.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include "MAIN.H"
#include "CD.H"
#include "DIESPR.H"
#include "LINE_SCR.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "MOVIE.H"
#include "PAD.H"
#include "PSP.H"
#include "SCE_AT.H"
#include "SND.H"
#include "SUB.H"
#include "TRANS.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "SLD.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x80198000 (Bg_img_buf) */
ULONG *Bg_img_buf = (ULONG *)0x80198000;
/* DS 0x8011a000 (Tex_tim_buf) */
ULONG *Tex_tim_buf = (ULONG *)0x8011a000;
/* DS 0x80190000 (Bs_buf): BS chunk load area. Bg_draw reads CD.Fsize bytes (the Bs_size entry, at most 0xf8e4 in STAGE5.C; a cut owns 0x20 sectors of its ROOMxxx.BSS), so the load runs past Bg_img_buf (0x80198000). The 8-byte BS_INFO entry (ofs, flag) is the last 8 bytes of the chunk, which is why Bs_info_tbl (MAIN.H) is Bs_buf - 8 indexed by the Bs_size entry */
ULONG *Bs_buf = (ULONG *)0x80190000;
/* DS 0x801a5800 (Bg_tim_buf): decompressed background TIM images; largest room block 64,020 bytes (ROOM20E.BSS cut 4), measured by decoding the SLD blocks of every ROOMxxx.BSS with the Desld stream format */
ULONG *Bg_tim_buf = (ULONG *)0x801a5800;

/* BSS file index = RDT file index - 570 */
#define RDT_BSS_OFS	0x23a

/* DS 0x8009db64 (LIGHTMATRIX): DS keeps it in .data with zero bytes, so it has an initialiser here. */
MATRIX LIGHTMATRIX = {
	{ { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
	{ 0, 0, 0 }
};

/* DS 0x8009db84 (COLORMATRIX): DS keeps it in .data with zero bytes, so it has an initialiser here. */
MATRIX COLORMATRIX = {
	{ { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
	{ 0, 0, 0 }
};

/* DS 0x8009dba4 (Pad_act_align): actuator alignment table for PadSetActAlign (PAD.C Pad_set). DS has it in
 * MAIN's .data directly after COLORMATRIX (0x8009db84-0x8009dba3); the two bytes after the table are alignment
 * padding before Curtain. */
UCHAR Pad_act_align[6] = {
	0x00, 0x01, 0xff, 0xff, 0xff, 0xff,	/* +0x0 */
};

/* Scroll block at DS 0x8009dc10: the scroll offset word, the RECT Bg_draw sends to LoadImage, and an unused word */
typedef struct {
	LONG Ofs_y;
	RECT Rect;
	LONG Pad;
} tagSCRL_BLK;
extern tagSCRL_BLK Scrl_blk;

/* DS 0x800109e8 (ClearImageRect) */
const RECT ClearImageRect = { 0x0000, 0x0000, 0x0140, 0x01e0 };

/* DS 0x8009dbac (Curtain) */
TILE Curtain[2][2] = {
	{ { 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0140, 0x0018 }, { 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0140, 0x0018 } },
	{ { 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x00d8, 0x0140, 0x0018 }, { 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x00d8, 0x0140, 0x0018 } }
};

/* DS 0x800dfae0 (Main): main work area (struct tagMAIN, MAIN.H). Zero at start; DS allocates it with the
 * common symbols in .bss, so it has no initialiser here. */
tagMAIN Main;



/* 8002aee0 */
/* DS 0x8002aee0 (main) */
/* DS stack frame: 0x40 bytes. s0..s4 are saved at sp+0x28..sp+0x38 and ra at sp+0x3c. sp+0x10 is the outgoing
 * fifth argument of two calls: the address of the "Paused" literal (DS 0x800109e0) for Print14 and the null RECT
 * pointer for SetDrawMode. rect is the only local kept in memory: it sits at sp+0x20..0x27, DS copies the 8 bytes of
 * ClearImageRect (DS 0x800109e8) into it with an lwl/lwr + swl/swr pair and passes sp+0x20 to ClearImage.
 * sp+0x14..0x1f is never written. */
void main(void)
{
	register tagMAIN *pMain;
	register tagGLOBAL *pG;
	RECT rect_unused;
	RECT rect;
	ULONG Fade_busy;

	Init_system();
	pMain = &Main;
	pG = &G;
	for (;;) {
frame_start:
		if (pMain->Pollhost_flg != 0) {
			trap(0x400);
		}
		pG->pOt0 = pG->Ot0[pG->Cbuf_id];
		pG->pOt1 = pG->Ot1[pG->Cbuf_id];
		pG->pOt2 = pG->Ot2[pG->Cbuf_id];
		pMain->pDb = pMain->Db + pG->Cbuf_id;
		ClearOTagR(pG->pOt0, 8);
		ClearOTagR(pG->pOt1, 0x400);
		ClearOTagR(pG->pOt2, 0x10);
		if (pG->Padd_data[0][0] == 0xff) {
			pG->Padd_data[0][3] = 0xff;
			pG->Padd_data[0][2] = 0xff;
			pG->Padd_data[1][3] = 0xff;
			pG->Padd_data[1][2] = 0xff;
		}
		if ((G.System_flg & SYS_RESET) != 0) goto frame_reset;
		do {
			if ((G.System_flg & SYS_IN_GAME) != 0 && Main.Bg_chg_flg == 0 && Game.Doordemo_flg == 0 &&
				*(USHORT *)&CD.Cd_rtn_no == 0 && (G.Stop_flg & 0x1000000) == 0 &&
				(G.System_flg & (SYS_DOOR_PROCESS | SYS_ENDING | SYS_DEMO_MODE | SYS_STR_MOVIE)) == 0 &&
				(G.Status_flg & (STAT_CINESCO | 0x100000)) == 0 &&
				(Main.Padoff_stop_flg[1][0] != 0 || pG->Padd_data[0][0] == 0xff)) {
				if (pMain->Padoff_stop_flg[0][0] < 2) {
					if (pMain->Padoff_stop_flg[0][0] == 0) {
						AddPrim(pG->pOt0 + 1, Pause + pG->Cbuf_id);
						AddPrim(pG->pOt0 + 1, &pMain->Db[pG->Cbuf_id].Pause_Dm);
						Print14(0x87, 0x6b, 0, 0x4000, (UCHAR *)"Paused");
					}
					pMain->Padoff_stop_flg[0][0]++;
					if (Main.Padoff_stop_flg[1][0] != 0) {
						Snd_bgm_pause(0);
						goto frame_update;
					}
				} else {
					*(USHORT *)Main.Padoff_stop_flg[3] = *(USHORT *)Main.Padoff_stop_flg[2];
					*(USHORT *)Main.Padoff_stop_flg[2] = ~((pG->Padd_data[0][2] << 8) | pG->Padd_data[0][3]);
					if ((*(USHORT *)Main.Padoff_stop_flg[2] & 0x400) != 0) {
						*(USHORT *)Main.Padoff_stop_flg[2] |= 0x800;
					}
					*(USHORT *)Main.Padoff_stop_flg[3] = *(USHORT *)Main.Padoff_stop_flg[2] &
						(*(USHORT *)Main.Padoff_stop_flg[3] ^ *(USHORT *)Main.Padoff_stop_flg[2]);
					if ((*(USHORT *)Main.Padoff_stop_flg[3] & 0x800) != 0) {
						Snd_bgm_pause(1);
						Main.Padoff_stop_flg[1][0] = 0;
					}
					PadActData[1] = 0;
					PadActData[0] = 0;
					VSync(0);
					goto frame_start;
frame_reset:
					rect = ClearImageRect;
					pG->Padd1 = 0;
					DrawSync(0);
					VSync(0);
					ResetCallback();
					CD.Cd_rtn_no = 0;
					CD.Xa_rtn_no = 0;
					ClearImage((RECT *)&rect, 0, 0, 0);
					Bg_set_mode(2, 0);
					Task_kill(0);
					Task_kill(1);
					Task_kill(2);
					Snd_sys_init2();
					SsEnd();
					SsQuit();
					Snd_sys_init();
					Init_global();
					Main.Curtain_kido = 0;
					ClearOTagR(pG->pOt0, 8);
					ClearOTagR(pG->pOt1, 0x400);
					ClearOTagR(pG->pOt2, 0x10);
					Moji_mode_init();
					Trans_pointer_set();
					Task_lexecute(0, 0);
					goto frame_start;
				}
			} else {
				pMain->Padoff_stop_flg[0][0] = 0;
				if (Main.Padoff_stop_flg[1][0] != 0) break;
			}
			Pad_set();
			if ((pG->Padd_trg & 0x800) == 0) break;
			Main.Padoff_stop_flg[1][0] = 1;
			*(USHORT *)Main.Padoff_stop_flg[2] = (USHORT)pG->Padd;
		} while (1);
		Main.Padoff_stop_flg[1][0] = 0;
frame_update:
		Trans_pointer_set();
		Moji_mode_init();
		Scheduler();
		pG->Status_flg &= ~(STAT_INSPECT | 0x100000);
		if ((G.System_flg & SYS_FADE_OUT) != 0) {
			Fade_busy = Fade_status(0);
			if (Fade_busy != 0) {
				DrawSync(0);
				PutDispEnv((DISPENV *)&(pMain->pDb)->Disp);
				SetDrawMode((DR_MODE *)&pMain->Fade[0].Dr_mode[pG->Cbuf_id], 1, 0, 0x40, (RECT *)0x0);
				Fade_adjust(0, 0x1f, 0x80808, (RECT *)0x0);
			}
			AddPrim(pG->pOt0, (void *)&pMain->Fade[0].T[pG->Cbuf_id]);
			AddPrim(pG->pOt0, (void *)&pMain->Fade[0].Dr_mode[pG->Cbuf_id]);
			pMain->Fade[0].Kido--;
			if ((int)((UINT)(USHORT)pMain->Fade[0].Kido << 0x10) < 0) {
				G.System_flg &= ~SYS_FADE_OUT;
			}
			DrawSync(0);
			VSync(0);
			DrawOTag(pG->pOt0 + 7);
		} else {
			Moji_trans_main();
			System_trans();
			Swap_Cbuff();
			Cd_system_control();
		}
	}
}

/* 8002b48c */
/* DS 0x8002b48c (Init_system) */
void Init_system(void)
{
	tagMAIN *pM;
	volatile UCHAR Rsv[24];
	/* retail frame is 0x40 (ra, s0, s1 saved); reserve 24 bytes so this frame matches, never read */

	pM = &Main;
	Init_scheduler();
	G.System_flg |= 0x40;
	ResetCallback();
	SetGraphDebug(0);
	InitGeom();
	Cd_init();
	SetDefDrawEnv((DRAWENV *)pM, 0, 0, 0x140, 0xf0);
	SetDefDrawEnv((DRAWENV *)&pM->Db[1].Draw, 0, 0xf0, 0x140, 0xf0);
	SetDefDispEnv((DISPENV *)&pM->Db[0].Disp, 0, 0xf0, 0x140, 0xf0);
	SetDefDispEnv((DISPENV *)&pM->Db[1].Disp, 0, 0, 0x140, 0xf0);
	pM->Db[1].Draw.dfe = 1;
	pM->Db[0].Draw.dfe = 1;
	pM->Db[1].Draw.dtd = 0;
	pM->Db[0].Draw.dtd = 0;
	InitCARD(0);
	StartCARD();
	_bu_init();
	PadInitDirect((char *)G.Padd_data, (char *)G.Padd_data[1]);
	PadStartCom();
	SetDrawMode((DR_MODE *)&pM->Db[0].Curtain_Dm, 0, 0, 0xc0, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&pM->Db[1].Curtain_Dm, 0, 0, 0xc0, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&pM->Db[0].Pause_Dm, 0, 0, 0x80, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&pM->Db[1].Pause_Dm, 0, 0, 0x80, (RECT *)0x0);
	Init_global();
	G.Cbuf_id = 0;
	Volume_Se = 100;
	Volume_Bgm = 100;
	G.Sound_mode = 0;
	pM->Key_idx = 0;
	pM->Rect.x = 0;
	pM->Rect.y = 0;
	pM->Rect.w = 0x140;
	pM->Rect.h = 0xf0;
	Moji_init();
	Trans_work_init();
	Line_work_init();
	Task_execute(0, Init_main);
	Task_execute(1, InitGeom_func);
	Task_execute(2, InitGeom_func);
	ResetGraph(0);
}

/* 8002b6ac */
/* DS 0x8002b6ac (InitGeom_func) */
void InitGeom_func(void)
{
	InitGeom();
	Task_exit();
}

/* 8002b6d4 */
/* DS 0x8002b6d4 (Init_main) */
void Init_main(void)
{
	TILE *p;
	/* volatile: keeps pCurtain in a stack slot (retail spills it at 0x10(sp)) so the frame matches retail at 64 bytes. */
	TILE * volatile pCurtain;
	int r;
	int i;

	Snd_sys_init();
	InitGeom();
	Vram_clr(0, 0, 0x400, 0x200);
	SetDispMask(1);
	Bg_set_mode(2, 0);
	for (r = 0; r < 4; r++) {
		for (i = 0; i < 2; i++) {
			p = &Main.Fade[r].T[i];
			p->w = 0x140;
			p->x0 = 0;
			p->y0 = 0;
			p->h = 0xf0;
			SetTile(p);
			SetSemiTrans(p, 1);
			Main.Fade[r].T[i].r0 = 0;
			Main.Fade[r].T[i].g0 = 0;
			Main.Fade[r].T[i].b0 = 0;
		}
	}
	Task_sleep(1);
	Main.Padoff_stop_flg[0][1] = 0;
	Task_execute(1, Logo);
	while (Main.Padoff_stop_flg[0][1] == 0) {
		Task_sleep(1);
	}
	Card_event_set();
	DecDCTReset(0);
	for (r = 0; r < 2; r++) {
		pCurtain = Curtain[0];
		for (i = 0; i < 2; i++) {
			SetTile(pCurtain + r * 2 + i);
			SetSemiTrans(pCurtain + r * 2 + i, 1);
		}
		SetTile(Pause + r);
		SetSemiTrans(Pause + r, 1);
	}
	Cd_read(TEX_TIM, Tex_tim_buf, 1, (UCHAR *)"TEX TIM");
	G.Ctpage = 28;
	G.Cclut = 0;
	Mapping_tim(Tex_tim_buf);
	G.Dummy8[0] = 0;
	G.Sound_mode = 0;
	Snd_sys_stereo(0);
	PadSetMainMode(0, 1, 2);
	G.Random = 0xd2706ca4;
	G.System_flg |= SYS_INTRO_PENDING;
	Task_lchain(0);
}

/* 8002b968 */
/* DS 0x8002b968 (Swap_Cbuff) */
/* DS stack frame: 0x38 bytes. s0..s5 are saved at sp+0x18..sp+0x2c and ra at sp+0x30. No local variable lives in
 * the frame: sp+0x10 only carries the fifth argument (the height 0xf0) of the seven SetDefDrawEnv and
 * SetDefDispEnv calls. */
void Swap_Cbuff(void)
{
	register tagMAIN *pMain;
	register tagGLOBAL *pG;
	int y;
	ULONG *pScrl_a;
	ULONG *pScrl_b;

	pMain = &Main;
	pG = &G;
	DrawSync(0);
	VSync((UINT)Main.Vloop);
	Snd_se_call();
	PutDrawEnv((DRAWENV *)Main.pDb);
	PutDispEnv((DISPENV *)&(Main.pDb)->Disp);
	if (((G.Status_flg & STAT_BG_HOLD) == 0) && (G.SaveData.Cut_no != (USHORT)G.Next_cut_no)) {
		Main.Bg_chg_flg = 1;
	}
	if ((G.System_flg & SYS_STR_MOVIE) != 0) {
		Movie();
	}
	if (pMain->Bg_mode == 0) {
		if (pMain->Bg_chg_flg != 0) {
			Bg_draw();
			G.Status_flg |= 0x100000;
			if (pMain->Db[0].Draw.ofs[1] == 0) {
				SetDefDrawEnv((DRAWENV *)pMain, 0, 0xf0, 0x140, 0xf0);
				SetDefDrawEnv((DRAWENV *)&pMain->Db[1].Draw, 0, 0, 0x140, 0xf0);
				SetDefDispEnv((DISPENV *)&pMain->Db[0].Disp, 0, 0, 0x140, 0xf0);
				SetDefDispEnv((DISPENV *)&pMain->Db[1].Disp, 0, 0xf0, 0x140, 0xf0);
			} else {
				SetDefDrawEnv((DRAWENV *)pMain, 0, 0, 0x140, 0xf0);
				SetDefDrawEnv((DRAWENV *)&pMain->Db[1].Draw, 0, 0xf0, 0x140, 0xf0);
				SetDefDispEnv((DISPENV *)&pMain->Db[0].Disp, 0, 0xf0, 0x140, 0xf0);
				SetDefDispEnv((DISPENV *)&pMain->Db[1].Disp, 0, 0, 0x140, 0xf0);
			}
			pMain->Db[1].Draw.dfe = 1;
			pMain->Db[0].Draw.dfe = 1;
			pMain->Db[1].Draw.dtd = 0;
			pMain->Db[0].Draw.dtd = 0;
			if ((G.System_flg & SYS_STR_MOVIE) != 0) goto movie_draw;
			goto no_draw;
		}
		if ((pG->Scrl_flg & 1U) != 0) {
			pScrl_a = (pG->pRoom)->pScrl;
			pScrl_b = Bg_img_buf;
		} else {
			pScrl_a = Bg_img_buf;
			pScrl_b = (pG->pRoom)->pScrl;
		}
		if (pMain->Scrl_type != 0) {
			pScrl_a += 5;
		}
		if (Scrl_blk.Ofs_y != 0) {
			Scrl_blk.Rect.y = ((pMain->pDb)->Draw).ofs[1];
			Scrl_blk.Rect.h = (short)Scrl_blk.Ofs_y;
			LoadImage((RECT *)&Scrl_blk.Rect, pScrl_a);
		}
		Scrl_blk.Rect.y = ((pMain->pDb)->Draw).ofs[1] + (short)Scrl_blk.Ofs_y;
		Scrl_blk.Rect.h = pG->Scrl_y;
		LoadImage((RECT *)&Scrl_blk.Rect, pScrl_b + ((Scrl_blk.Ofs_y + Scrl_h) - (int)pG->Scrl_y) * 0xa0);
		Scrl_blk.Rect.y = pG->Scrl_y + ((pMain->pDb)->Draw).ofs[1] + (short)Scrl_blk.Ofs_y;
		Scrl_blk.Rect.h = (short)Scrl_h - pG->Scrl_y;
		LoadImage((RECT *)&Scrl_blk.Rect, pScrl_a + Scrl_blk.Ofs_y * 0xa0);
		if (Scrl_h != 0xf0) {
			Scrl_blk.Rect.y = ((pMain->pDb)->Draw).ofs[1] + (short)Scrl_blk.Ofs_y + (short)Scrl_h;
			Scrl_blk.Rect.h = 0xf0 - (short)(Scrl_blk.Ofs_y + Scrl_h);
			LoadImage((RECT *)&Scrl_blk.Rect, pScrl_a + (Scrl_blk.Ofs_y + Scrl_h) * 0xa0);
		}
	}
	if (pMain->Bg_mode == 2) {
		SetTile((TILE *)&(pMain->pDb)->Bg_clr);
		AddPrim(pG->pOt2 + 0xf, &(pMain->pDb)->Bg_clr);
	}
movie_draw:
	DrawOTag(pG->pOt2 + 0xf);
	DrawOTag(pG->pOt1 + 0x3ff);
	DrawOTag(pG->pOt0 + 7);
no_draw:
	pG->Cbuf_id ^= 1;
	if ((G.System_flg & SYS_STR_MOVIE) == 0) {
		pMain->Bg_chg_flg = 0;
	}
}

/* 8002bda8 */
/* DS 0x8002bda8 (Bg_set_mode) */
void Bg_set_mode(ULONG Mode, ULONG Rgb)
{
	tagMAIN *pMain;
	UINT i;

	/* pMain is loaded before the Bg_mode store: the store then addresses Main directly (lui at, sb) as in DS, and the loop indexes Db[i] from pMain (displacements 0x8c/0x90/0x94, pointer advanced by sizeof(DBUF) in the branch delay slot). */
	pMain = &Main;
	Main.Bg_mode = (UCHAR)Mode;
	if (Mode == 2) {
		for (i = 0; i < 2; i++) {
			/* DS fills the TILE with three word stores: r0/g0/b0/code, x0/y0, w/h. */
			*(ULONG *)&pMain->Db[i].Bg_clr.r0 = Rgb;
			*(ULONG *)&pMain->Db[i].Bg_clr.x0 = 0;
			*(ULONG *)&pMain->Db[i].Bg_clr.w  = 0xf00140;
		}
	}
}

/* 8002bdf4 */
/* DS 0x8002bdf4 (Bg_draw) */
void Bg_draw(void)
{
	/* volatile: pad_save0 and pad_save1 live on the stack across the calls, as in retail (0x10 and 0x11(sp)). */
	volatile UCHAR pad_save0;
	volatile UCHAR pad_save1;
	USHORT *pBs_size;
	USHORT ctpage_save;
	int done;
	UINT File_no;
	int bs_idx;
	int bs_ofs;
	int x_ofs;

	pad_save1 = PadActData[1];
	pad_save0 = PadActData[0];
	PadActData[1] = 0;
	PadActData[0] = 0;
	if (Main.Bg_chg_flg != 2) {
		G.SaveData.Cut_no_old = G.SaveData.Cut_no;
		G.SaveData.Cut_no = (short)G.Next_cut_no;
		G.Cccut = Ccut_search((UINT)G.Next_cut_no);
		if ((G.System_flg & SYS_PSP_TRANS) == 0) {
			Psp_set();
		}
		pBs_size = G.pBs_size;
		if ((G.System_flg & SYS_BS_READ_SKIP) == 0) {
			CD.Check_sum_flg = 2;
			CD.Fsize = (ULONG)*(USHORT *)((int)G.pBs_size + G.SaveData.Cut_no * 2 + G.SaveData.Room_no * 0x20);
			CD.Fno = ((short *)Rdt_tbl[G.SaveData.Stage_no])[G.SaveData.Room_no] - RDT_BSS_OFS;
			File_no = (UINT)CD.Fno;
			CD.Fsum = *(UCHAR *)((int)G.pBs_check_sum + G.SaveData.Cut_no + G.SaveData.Room_no * 0x10);
			CD.Fsector = (UINT)floc[File_no].sec_high * 0x10000 + (UINT)floc[File_no].sector +
						 G.SaveData.Cut_no * 0x20;
			Cd_read(File_no, (ULONG *)Bs_buf, 2, (UCHAR *)"BS");
			CD.Check_sum_flg = 0;
		} else {
			G.System_flg &= ~SYS_BS_READ_SKIP;
		}
		bs_idx = *(USHORT *)((int)pBs_size + G.SaveData.Cut_no * 2 + G.SaveData.Room_no * 0x20);
		if (((BS_INFO *)(Bs_info_tbl + bs_idx))->flag != 0) {
			Desld_init(Bg_tim_buf);
			bs_ofs = ((BS_INFO *)(Bs_info_tbl + bs_idx))->ofs;
			do {
				memcpy16((ULONG *)ScratchPad, (ULONG *)((UCHAR *)Bs_buf + bs_ofs), 0x400);
				bs_ofs += 0x400;
				done = Desld_proc();
			} while (done == 0);
			/* Ghidra G._14856_2_: Ctpage and Cclut (adjacent bytes) saved and restored as one USHORT */
				ctpage_save = *(USHORT *)&G.Ctpage;
			G.Ctpage = 21;
			G.Cclut = 0;
			Mapping_tim(Bg_tim_buf);
			*(USHORT *)&G.Ctpage = ctpage_save;
		}
	}
	x_ofs = 0;
	Rect_bg.x = 0;
	Rect_bg.y = ((Main.pDb)->Draw).ofs[1];
	Main.Rect.y = Rect_bg.y;
	DecDCTReset(0);
	DecDCToutCallback((void (*)())0x0);
	DecDCTvlc((ULONG *)Bs_buf, (ULONG *)Dct_in_buf);
	DecDCTin((ULONG *)Dct_in_buf, 0);
	do {
		DecDCTout(Bg_img_buf, 0x780);
		DecDCToutSync(0);
		LoadImage((RECT *)&Rect_bg, Bg_img_buf);
		x_ofs += 0x10;
		Rect_bg.x += 0x10;
	} while (x_ofs < 0x140);
	StoreImage((RECT *)&Main.Rect, Bg_img_buf);
	SetGeomScreen((UINT)((G.pRoom)->pRcut[G.SaveData.Cut_no].ViewR >> 7));
	Set_view((G.pRoom)->pRcut[G.SaveData.Cut_no].View_p, (G.pRoom)->pRcut[G.SaveData.Cut_no].View_r);
	PadActData[0] = pad_save0;
	PadActData[1] = pad_save1;
}

/* 8002c1a0 */
/* DS 0x8002c1a0 (Fade_set) */
/* DS stack frame: 0x28 bytes. s0 and s1 are saved at sp+0x18 and sp+0x1c and ra at sp+0x20. sp+0x10 carries the
 * fifth argument of the two SetDrawMode calls (the null RECT pointer); there is no other stack local. */
void Fade_set(USHORT A0, short Add, ULONG Mask, ULONG Pri)
{
	FADE    *pF;

	pF = &Main.Fade[A0 & 0xff];
	pF->Add   = Add;
	pF->Hrate = (UCHAR)(A0 >> 8);
	pF->Pri   = (UCHAR)Pri;
	if ((Mask & 4) == 0) {
		pF->Mask_r = 0;
	} else {
		pF->Mask_r = 0xff;
	}
	if ((Mask & 2) == 0) {
		pF->Mask_g = 0;
	} else {
		pF->Mask_g = 0xff;
	}
	if ((Mask & 1) == 0) {
		pF->Mask_b = 0;
	} else {
		pF->Mask_b = 0xff;
	}
	SetDrawMode((DR_MODE *)pF->Dr_mode, 1, 0, (UINT)pF->Hrate << 5, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&pF->Dr_mode[1], 1, 0, (UINT)pF->Hrate << 5, (RECT *)0x0);
	/* Rect {0, 0, 0x140, 0xf0}: w and h go out as one word, then x and y */
	*(ULONG *)&pF->Rect.w = 0xf00140;
	*(ULONG *)&pF->Rect.x = 0;
	if ((int)((UINT)(USHORT)Add << 0x10) < 0) {
		pF->Kido = Add + -0x8000;
	} else {
		pF->Kido = 0;
	}
}

/* 8002c2b0 */
/* DS 0x8002c2b0 (Fade_adjust) */
/* DS addresses everything through one &Main.Fade[No] pointer, copies *Rect as two word moves and
   otherwise stores the default rect {0, 0, 0x140, 0xf0} as two word stores. */
void Fade_adjust(ULONG No, ULONG Kido, ULONG Rgb, RECT *Rect)
{
	UCHAR alpha;
	FADE *pF;

	pF = &Main.Fade[No];
	alpha = pF->T[0].code;
	pF->Kido = (short)Kido;
	*(ULONG *)&pF->T[G.Cbuf_id].r0 = Rgb | (UINT)alpha << 0x18;
	if (Rect == (RECT *)0x0) {
		*(ULONG *)&pF->Rect.x = 0;
		*(ULONG *)&pF->Rect.w = 0xf00140;
	} else {
		*(ULONG *)&pF->Rect.x = *(ULONG *)&Rect->x;
		*(ULONG *)&pF->Rect.w = *(ULONG *)&Rect->w;
	}
}

/* 8002c324 */
/* DS 0x8002c324 (Fade_off) */
void Fade_off(ULONG No)
{
	*(SHORT *)&Main.Fade[No].Kido = -1;
}

/* 8002c350 */
/* DS 0x8002c350 (Fade_status) */
ULONG Fade_status(ULONG No)
{
	return (UINT)(int)*(short *)&Main.Fade[No].Kido >> 0x1f;
}

/* 8002c378 */
/* DS 0x8002c378 (System_trans) */
void System_trans(void)
{
	UCHAR mask;
	UCHAR curtain_kido8;
	int kido_hi;
	ULONG *pOt;
	UCHAR ot_idx;
	UINT cb;
	FADE *pF;
	int i;

	pF = Main.Fade;
	i = 4;
	do {
		kido_hi = (UINT)(USHORT)pF->Kido << 0x10;
		if (-1 < kido_hi) {
			mask = (UCHAR)(kido_hi >> 0x17);
			pF->T[G.Cbuf_id].r0 = pF->Mask_r & mask;
			pF->T[G.Cbuf_id].g0 = pF->Mask_g & mask;
			pF->T[G.Cbuf_id].b0 = pF->Mask_b & mask;
			pF->Kido = pF->Kido + pF->Add;
			*(ULONG *)&pF->T[G.Cbuf_id].x0 = *(ULONG *)&pF->Rect;
			*(ULONG *)&pF->T[G.Cbuf_id].w = *(ULONG *)&pF->Rect.w;
			if (i < 2) {
				AddPrim(G.pOt2 + pF->Pri, (void *)&pF->T[G.Cbuf_id]);
				ot_idx = pF->Pri;
				pOt = G.pOt2;
			} else {
				AddPrim(G.pOt0 + pF->Pri, (void *)&pF->T[G.Cbuf_id]);
				ot_idx = pF->Pri;
				pOt = G.pOt0;
			}
			AddPrim(pOt + ot_idx, (void *)&pF->Dr_mode[G.Cbuf_id]);
		}
		i = i + -1;
		pF++;
	} while (0 < i);
	if ((G.Status_flg & STAT_CINESCO) != 0) {
		if (Main.Curtain_kido < 0xf0) {
			Main.Curtain_kido += 16;
		}
	} else if (0xf < Main.Curtain_kido) {
		Main.Curtain_kido -= 16;
	}
	curtain_kido8 = Main.Curtain_kido;
	cb = (UINT)G.Cbuf_id;
	Curtain[1][cb].r0 = Main.Curtain_kido;
	Curtain[1][cb].g0 = curtain_kido8;
	Curtain[1][cb].b0 = curtain_kido8;
	Curtain[0][cb].r0 = curtain_kido8;
	Curtain[0][cb].g0 = curtain_kido8;
	Curtain[0][cb].b0 = curtain_kido8;
	if (((G.System_flg & SYS_DOOR_INIT) == 0) && (Main.Curtain_kido != 0)) {
		AddPrim(G.pOt0 + 4, Curtain[0] + G.Cbuf_id);
		AddPrim(G.pOt0 + 4, Curtain[1] + G.Cbuf_id);
		AddPrim(G.pOt0 + 4, &(Main.pDb)->Curtain_Dm);
	}
}

/* 8002c610 */
/* DS 0x8002c610 (Init_global) */
void Init_global(void)
{
	Memclr(&G.Status_flg, 0x1419);
	/* Every fade level starts at Kido -1; retail loads -1 once into v0 and stores it to the four Fade Kido halves, Cut_no_old and Room_no_old */
	Main.Fade[0].Kido = -1;
	Main.Fade[1].Kido = -1;
	Main.Fade[2].Kido = -1;
	Main.Fade[3].Kido = -1;
	G.SaveData.Cut_no_old = -1;
	G.SaveData.Room_no_old = -1;
	/* Each pad table row is {0, 0, 0xff, 0xff}: one word store per row, as in retail */
	*(ULONG *)&G.Padd_data[0][0] = 0xffff0000;
	*(ULONG *)&G.Padd_data[1][0] = 0xffff0000;
	Moji.Mess_flg = 0;
	G.System_flg &= (SYS_EX_BATTLE | SYS_TIMER | SYS_TOFU_SURVIVOR | 0x8d0);
	G.SaveData.Key_idx = Main.Key_idx;
}

/* 8002c6b4 */
/* DS 0x8002c6b4 (Cut_check) */
ULONG Cut_check(ULONG Flg)
{
	UCHAR next_cut;
	VCUT *pVcut;
	ULONG hit_flg;
	UCHAR *pFcut;
	VCUT *cp;
	tagGLOBAL *pG;

	pG = &G;

	if (pG->Cccut_next[1].Fcut == pG->Next_cut_no) {
		pFcut = &pG->Cccut_next[1].Fcut;
		pVcut = pG->Cccut_next;
		do {
			cp = pVcut + 1;
			if ((cp->Be_flg != 0) && (((pFcut[-1] == 0xff || (pFcut[-1] == pG->Player.nFloor)) && (hit_flg = Hit_ck_point4((VECTOR *)&pG->Player.Pos_x, cp), hit_flg != 0)))) {
				next_cut = cp->Tcut;
				goto apply_cut;
			}
			pFcut += 0x14;
			pVcut = cp;
		} while (*pFcut == pG->Next_cut_no);
	}
	hit_flg = 0;
	next_cut = pG->Next_cut_no;
	if (Flg != 0) {
apply_cut:
		Cut_change((UINT)next_cut);
		hit_flg = 1;
	}
	return hit_flg;
}

/* 8002c7ac */
/* DS 0x8002c7ac (Cut_change) */
void Cut_change(ULONG Fc)
{
	G.Next_cut_no = (UCHAR)Fc;
	G.Cccut_next = Ccut_search(Fc);
}

/* 8002c7dc */
/* DS 0x8002c7dc (Ccut_search) */
VCUT *Ccut_search(ULONG Fc)
{
	VCUT *pCcut;

	pCcut = (G.pRoom)->pVcut;
	while (pCcut->Fcut != Fc) {
		pCcut++;
	}
	return pCcut;
}

/* 8002c820 */
/* DS 0x8002c820 (Hit_ck_point4) */
ULONG Hit_ck_point4(VECTOR *p, VCUT *cp)
{
	int x;
	int y;
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;

	x = (int)cp->Xz[0][0];
	y = (int)cp->Xz[0][1];
	a = p->vz - y;
	b = cp->Xz[1][0] - x;
	c = p->vx - x;
	d = cp->Xz[1][1] - y;
	e = cp->Xz[3][0] - x;
	f = cp->Xz[3][1] - y;
	if (b * a <= d * c) {
		if (e * a < f * c) {
			return 0;
		}
		y = cp->Xz[2][1] - y;
		x = cp->Xz[2][0] - x;
		if (((d - y) * (c - x) <= (b - x) * (a - y)) && ((e - x) * (a - y) <= (f - y) * (c - x))) {
			return 1;
		}
	}
	return 0;
}

/* 8002c904 */
/* DS 0x8002c904 (Hit_ck_box) */
ULONG Hit_ck_box(VECTOR *p, BOX *q)
{
	ULONG hit_flg;

	if ((UINT)(p->vx - (int)q->X) > (UINT)q->W) {
		hit_flg = 0;
	} else {
		if ((UINT)(p->vz - (int)q->Z) > (UINT)q->D) {
			hit_flg = 0;
		} else {
			hit_flg = 1;
		}
	}
	return hit_flg;
}

/* 8002c94c */
/* DS 0x8002c94c (Card_event_set) */
void Card_event_set(void)
{
	EnterCriticalSection();
	Ev0 = OpenEvent(0xf4000001, 4, 0x2000, (long (*)())0x0);
	Ev1 = OpenEvent(0xf4000001, 0x100, 0x2000, (long (*)())0x0);
	Ev2 = OpenEvent(0xf4000001, 0x2000, 0x2000, (long (*)())0x0);
	Ev3 = OpenEvent(0xf4000001, 0x8000, 0x2000, (long (*)())0x0);
	Ev10 = OpenEvent(0xf0000011, 4, 0x2000, (long (*)())0x0);
	Ev11 = OpenEvent(0xf0000011, 0x100, 0x2000, (long (*)())0x0);
	Ev13 = OpenEvent(0xf0000011, 0x8000, 0x2000, (long (*)())0x0);
	ExitCriticalSection();
	EnableEvent(Ev0);
	EnableEvent(Ev1);
	EnableEvent(Ev2);
	EnableEvent(Ev3);
	EnableEvent(Ev10);
	EnableEvent(Ev11);
	EnableEvent(Ev13);
}

/* 8002cac4 */
/* DS 0x8002cac4 (Logo) */
void Logo(void)
{
	Main.Scrl_type = 1;
	Bg_set_mode(0, 0);
	Cd_read(GWARNING_TIM, (unsigned long *)0x80198000, 0, (UCHAR *)"Warning");	/* Bg_img_buf (DS 0x80198000) */
	Fade_set(0x200, -0x400, 7, 0);
	while (Fade_status(0) == 0) {
		Task_sleep(1);
	}
	Main.Padoff_stop_flg[0][1] = 1;
	do {
		Task_sleep(1);
	} while (Main.Padoff_stop_flg[0][1] != 2);
	Fade_set(0x200, 0x400, 7, 0);
	while (Fade_status(0) == 0) {
		Task_sleep(1);
	}
	Main.Scrl_type = 0;
	Bg_set_mode(2, 0);
	Main.Padoff_stop_flg[0][1] = 0;
	Task_exit();
}

/* DS 0x8009dbec (Pause) */
TILE Pause[2] = {
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0140, 0x00f0 },
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0140, 0x00f0 }
};

/* DS 0x8009dc0c (Scrl_h) */
LONG Scrl_h = 0x000000f0;

/* DS 0x8009dc10 (Scrl_blk) */
tagSCRL_BLK Scrl_blk = { 0, { 0x0000, 0x0000, 0x0140, 0x00f0 }, 0 };

/* DS 0x8009dc20 (Rect_bg) */
RECT Rect_bg = { 0x0000, 0x0000, 0x0010, 0x00f0 };

/* DS 0x800cc1e8 (G) */
tagGLOBAL G;

/* DS 0x800d7698 (Ev0) */
ULONG Ev0;

/* DS 0x800d7828 (Ev1) */
ULONG Ev1;

/* DS 0x800d782c (Ev2) */
ULONG Ev2;

/* DS 0x800d7830 (Ev3) */
ULONG Ev3;

/* DS 0x800d8cd4 (Ev10) */
ULONG Ev10;

/* DS 0x800d8cdc (Ev11) */
ULONG Ev11;

/* DS 0x800d8ce0 (Ev13) */
ULONG Ev13;
