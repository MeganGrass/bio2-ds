/*
 * MOVIE.C - STR movie playback (DS streaming + MDEC)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MOVIE.C
 */

#define _MOVIE_C_
#include "TYPES.H"
#include <LIBCD.H>
#include <LIBDS.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBPRESS.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include "MAIN.H"
#include "CONFIG.H"
#include "SND.H"
#include "CD.H"
#include "PAD.H"
#include "MOVIE.H"
#include "MAIN.H"
#include "SCE1.H"
#include "STATUS.H"
#include "TITLE.H"
#include "GAME.H"
#include "PLAYER.H"
#include "FILECODE.H"
#include "flag.h"

/* CD interrupt pending flag (DS 800d5268); defined by the LIBDS library, not declared in the SDK headers */
extern volatile int StCdIntrFlag;

/* DS 0x800a216c (Movie_tbl) - 18 entries, indexed by Movie_id; Pos = x | (y << 16) */
MOVIE_TBL   Movie_tbl[] =
{
	{ TITLEL_STR, 851, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x5800, 0x0080, 110, 0, 0 },	/* 0 */
	{ R200_STR, 250, 0x00ff, 0x09f0, 320, 1, 0, 0x00000000, 0, 0xc800, 0x0041, 40, 0, 0 },	/* 1 */
	{ R200_STR, 146, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0xb000, 0x0041, 40, 0, 0 },	/* 2 */
	{ OPN1STL_STR, 4167, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0041, 110, 0, 0 },	/* 3 */
	{ R10B_STR, 150, 0x00ff, 0x09f0, 320, 0, 0, 0x00280000, 0, 0x6000, 0x0008, 60, 0, 0 },	/* 4 */
	{ R200_STR, 90, 0x00ff, 0x09f0, 320, 0, 0, 0x00280000, 0, 0x6000, 0x0008, 110, 0, 0 },	/* 5 */
	{ R505_STR, 598, 0x00ff, 0x09f0, 320, 0, 0, 0x00280000, 0, 0x6000, 0x0008, 110, 0, 0 },	/* 6 */
	{ R505_STR, 598, 0x00ff, 0x09f0, 320, 0, 0, 0x00280000, 0, 0x6000, 0x0008, 110, 407, 0 },	/* 7 */
	{ R108L_STR, 1326, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x5800, 0x0008, 110, 0, 0 },	/* 8 */
	{ R109_STR, 676, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0008, 110, 0, 0 },	/* 9 */
	{ R204L_STR, 500, 0x00ff, 0x09f0, 320, 0, 0, 0x00280000, 0, 0x5800, 0x0008, 110, 0, 0 },	/* 10 */
	{ R409_STR, 4287, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0008, 110, 0, 0 },	/* 11 */
	{ R700L_STR, 2125, 0x00ff, 0x0000, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0041, 110, 0, 0 },	/* 12 */
	{ R703L_STR, 3115, 0x00ff, 0x0000, 320, 1, 0, 0x00280000, 0, 0x6000, 0x000c, 110, 0, 0 },	/* 13 */
	{ R704L_STR, 9187, 0x00ff, 0x0000, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0041, 110, 0, 0 },	/* 14 */
	{ OPN2NDL_STR, 3003, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0043, 110, 0, 0 },	/* 15 */
	{ OPN2NDRL_STR, 3003, 0x00ff, 0x09f0, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0043, 125, 0, 0 },	/* 16 */
	{ R704L_STR, 9247, 0x00ff, 0x0000, 320, 1, 0, 0x00280000, 0, 0x6000, 0x0041, 0, 4589, 0 },	/* 17 */
};

/* ------------------------------------------------------------------ */
/* Movie work data: retail keeps these runtime variables in the exe     */
/* image as zero bytes; a zero initialiser keeps each one in .data (an  */
/* uninitialised global would go to bss and move).                      */
/* ------------------------------------------------------------------ */
/* DS 0x800d5b80 (Channel) */
DslFILTER   Channel;				/* file, chan, pad */

/* DS 0x800d75d8 (Vm_disp) */
VM_DISP     Vm_disp;

/* DS 0x800d763c (pVm_frm) */
VM_FRAME    *pVm_frm;

/* DS 0x800d7670 (Vm_work) */
VM_WORK     Vm_work;

/* DS 0x800d8cd8 (Loc) */
DslLOC      Loc;

/* DS 0x800df338 (pMovie_result) */
UCHAR       *pMovie_result;

/* DS 0x800e2a88 (Ret_Value) */
UCHAR       Ret_Value[8];

/* DS 0x800e8980 (pAtv) */
DslATV      *pAtv;

/* DS 0x800eae34 (pFile) */
DslFILE     *pFile;

/* ------------------------------------------------------------------ */
/* DS 0x80032150 (Init_movie_work)                                     */
/* ------------------------------------------------------------------ */
void Init_movie_work(ULONG Id)
{
	ULONG       *pBuff;
	ULONG       width;
	MOVIE_TBL   *pMovie_tbl;
	VM_WORK     *pWork = &Vm_work;
	VM_DISP     *pDisp = &Vm_disp;

	pMovie_tbl = &Movie_tbl[Id];
	switch (pMovie_tbl->Work_top)
	{
	case 0:
		pBuff = Bg_img_buf;
		break;
	case 1:
		pBuff = (ULONG *)MOVIE_WORK_TOP1;
		break;
	case 2:
		pBuff = (ULONG *)G.pFree_work;
		break;
	}
	/* Work_top picks the memory for the frame buffers: 0 = background image buffer, 1 = MOVIE_WORK_TOP1,
	 * 2 = the free work area. Any other value leaves pBuff unset, as in retail. */
	pVm_frm = (VM_FRAME *)pBuff;

	G.Movie_id      = (UCHAR)Id;
	G.Movie_Isrgb24 = pMovie_tbl->Isrgb24;
	pFile           = (DslFILE *)(pVm_frm + 1);
	pAtv            = (DslATV *)(pVm_frm + 4);
	pWork->Movie_w = pMovie_tbl->Disp_w;
	pMovie_result   = Ret_Value;
	pWork->Rno        = 0;
	pWork->Movie_sync = 0;

	width = 16;
	if (G.Movie_Isrgb24 == 1)
		width = 24;
	pDisp->Width        = width;
	pDisp->Rect.w = width;
	pDisp->Pos_x     = (USHORT)pMovie_tbl->Pos;
	pDisp->Pos_y     = (USHORT)(pMovie_tbl->Pos >> 16);
	pDisp->Attribute = pMovie_tbl->Attribute;
	pDisp->Isrgb24   = pMovie_tbl->Isrgb24;

	if ((pDisp->Attribute & 2) == 0)
	{
		pWork->Eom = 0;
	}
	else if (pWork->Eom == 2)
	{
		/* previous movie of a chain was skipped: skip this one too */
		pWork->Eom = 1;
		Main.Padoff_stop_flg[1][1] = 2;
		pDisp->Attribute = (pDisp->Attribute & ~0x80) | 0x40;
	}
	else
	{
		pWork->Eom = 0;
		Main.Padoff_stop_flg[1][1] = 0;
	}

	pWork->Frame        = 0;
	pWork->Free0        = 0;
	/* movie volume is the table volume scaled by the BGM volume setting (percent). The USHORT and UCHAR
	 * operands promote to int, so the division by 100 is signed, as in retail (mult by 0x51eb851f, sra 5);
	 * the product is at most 65535 * 255, so it is never negative and the quotient equals the unsigned one. */
	pWork->Movie_volume = pMovie_tbl->Volume * Volume_Bgm / 100;
	pWork->Movie_cancel = pMovie_tbl->Cancel;
	G.System_flg         = (G.System_flg & ~SYS_XA_MOVIE) | SYS_STR_MOVIE;
	Pad_vib_init();
}

/* ------------------------------------------------------------------ */
/* DS 0x80032340 (Movie)                                               */
/* Movie state machine: Vm_work.Rno selects init (0), main (1) or exit (2) and the loop runs until Rno
 * reaches 2. The draw buffer flips after any frame that was drawn (Vm_work.Frame != 0). */
/* ------------------------------------------------------------------ */
void Movie(void)
{
	/* DS 0x800a23ac */
	static void (*pMovie_proc_tbl[])(void) =
	{
		Movie_init,
		Movie_main,
		Movie_exit,
	};

	do {
		pMovie_proc_tbl[Vm_work.Rno]();
		if (G.System_flg & SYS_XA_MOVIE)
		{
			Main.Db[G.Cbuf_id ^ 1].Disp.isrgb24 = (UCHAR)G.Movie_Isrgb24;
			Main.Db[G.Cbuf_id ^ 1].Disp.disp.w  = Vm_work.Movie_w;
		}
		if (Vm_work.Frame != 0)
			G.Cbuf_id ^= 1;
	} while (Vm_work.Rno < 2);
}

/* ------------------------------------------------------------------ */
/* DS 0x80032450 (Movie_init)                                          */
/* ------------------------------------------------------------------ */
/* DS stack frame: 0x38 bytes. s0..s3 are saved at sp+0x20..sp+0x2c and ra at sp+0x30. Rect occupies sp+0x10..0x17
 * and Rect2 sp+0x18..0x1f: DS fills them with lwl/lwr + swl/swr copies of the two 8-byte templates at DS 0x80010ae0
 * and 0x80010ae8, and StoreImage receives their addresses. The word at sp+0x14 (Rect.w and Rect.h) is written again
 * in the Attribute & 4 path. */
static void Movie_init(void)
{
	VM_DISP *pDisp = &Vm_disp;
	VM_WORK *pWork = &Vm_work;
	RECT    Rect  = { 0x140, 0x000, 0x0a0, 0x1e0 };    /* DS 0x80010ae0 */
	RECT    Rect2 = { 0x1c0, 0x0f0, 0x020, 0x0f0 };    /* DS 0x80010ae8 */

	/* Attribute bit 8 draws the movie over the game screen. The VRAM under it is saved first, in two
	 * pieces when bit 4 is set on an RGB24 movie, so it can be restored afterwards. */
	if (pDisp->Attribute & 8)
	{
		/* movie drawn over the game screen: save the VRAM it will overwrite */
		Set_movie_volume2(pWork->Movie_volume);
		SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
		if (pDisp->Isrgb24 == 1)
		{
			if (pDisp->Attribute & 4)
			{
				/* w = 0xa0 and h = 0xf0 in one word store (DS writes the pair with a single sw at sp+0x14) */
				*(ULONG *)&Rect.w = 0xf000a0;
				StoreImage(&Rect, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				G.pFree_work += 0x12c10;
				StoreImage(&Rect2, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				G.pFree_work += 0x3c10;
			}
			else
			{
				StoreImage(&Rect, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				G.pFree_work += 0x25810;
			}
			DrawSync(0);
		}
		if (Open_movie(G.Movie_id))
		{
			Movie_end();
			pWork->Status = 1;
			pWork->Rno += 2;
		}
		else
		{
			pWork->Status = 0;
			pWork->Frame  = 0;
			pWork->Rno++;
		}
	}
	else if (pWork->Eom)
	{
		Movie_end();
		pWork->Rno += 2;
	}
	else
	{
		Set_movie_volume2(pWork->Movie_volume);
		SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
		if (Open_movie(G.Movie_id))
		{
			Movie_end();
			pWork->Status = 1;
			pWork->Rno += 2;
		}
		else
		{
			pWork->Status = 0;
			if (pDisp->Attribute & 1)
			{
				Fade_off(0);
				Bg_set_mode(1, 0);
			}
			pWork->Frame = 0;
			pWork->Rno++;
		}
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80032700 (Movie_main)                                          */
/* ------------------------------------------------------------------ */
/* DS stack frame: 0x28 bytes. s0..s2 are saved at sp+0x18..sp+0x20 and ra at sp+0x24. Rect occupies sp+0x10..0x17:
 * DS writes x and y with one word store at sp+0x10 and w and h with one word store at sp+0x14, then passes sp+0x10
 * to ClearImage. */
static void Movie_main(void)
{
	VM_DISP *pDisp = &Vm_disp;
	VM_WORK *pWork = &Vm_work;
	SHORT   Eom;
	RECT    Rect;

	DrawSync(0);
	VSync(2);
	PutDrawEnv(&Main.Db[G.Cbuf_id].Draw);
	PutDispEnv(&Main.Db[G.Cbuf_id].Disp);

	if (pWork->Frame < 2)
	{
		/* clear the half of VRAM the next frame will be decoded into */
		Rect.x = 0;
		if (Main.Db[0].Draw.ofs[1] == 0)
			Rect.y = (G.Cbuf_id != 0) ? 0xf0 : 0;
		else
			Rect.y = (G.Cbuf_id == 0) ? 0xf0 : 0;
		if (pDisp->Isrgb24 == 1)
		{
			Rect.w = 0x1e0;
			Rect.h = 0xf0;
		}
		else
		{
			Rect.w = 0x140;
			Rect.h = 0xf0;
		}
		ClearImage(&Rect, 0, 0, 0);

		if (pWork->Frame == 0)
		{
			G.System_flg |= SYS_XA_MOVIE;
			if (G.Movie_id != 0x11)
				Movie_xa_start();
		}
	}

	pWork->Frame++;
	Eom = (SHORT)Play_movie();

	/* Retail: if neither the stream status nor the MDEC sync flag is set yet,
	   the code is a branch-to-self and never re-reads the flags. */
	if (pWork->Status == 0 && pWork->Movie_sync == 0)
	{
		for (;;);
	}

	if (Eom == 1)
	{
		pWork->Eom = 1;
		Main.Padoff_stop_flg[1][1] = 0;
	}
	else if (Eom != 0 || (((pDisp->Attribute & 2) || pWork->Frame >= 61) && pWork->Movie_cancel && (Pad_set(), G.Padd & 0x800)))
	{
		/* error, or cancelled with the START/skip button */
		pWork->Eom = 2;
		Main.Padoff_stop_flg[1][1] = 2;
		pDisp->Attribute = (pDisp->Attribute & ~0x80) | 0x40;
	}

	if (pWork->Eom && (pWork->Frame & 1))
	{
		pWork->Frame = 0;
		pWork->Rno++;
		Movie_end();
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80032984 (Movie_end)                                           */
/* ------------------------------------------------------------------ */
/* DS stack frame: 0x20 bytes, ra saved at sp+0x18. unused_rect occupies sp+0x10..0x17: DS copies 8 bytes from
 * DS 0x80010af0 into it with lwl/lwr + swl/swr before the call to Close_movie and never reads them back. */
static void Movie_end(void)
{
	RECT    unused_rect = {0x140, 0, 0xa0, 0x200};

	(void)unused_rect;
	Close_movie();
	if (Vm_disp.Attribute & 1)
		Bg_set_mode(0, 0);
	if (Vm_disp.Attribute & 0x40)
		Bg_set_mode(2, 0);
	if (Vm_disp.Attribute & 0x80)
		Bg_set_mode(2, 0xffffff);

	if ((Vm_disp.Attribute & 8) == 0)
	{
		G.Movie_Isrgb24 = 0;
		Vm_work.Movie_w = 0x140;
		G.System_flg   |= SYS_XA_MOVIE;
	}
	else if (Vm_disp.Isrgb24 == 0)
	{
		Bg_set_mode(0, 0);
		Main.Bg_chg_flg = 2;
	}
	else
	{
		G.Movie_Isrgb24 = 0;
		Vm_work.Movie_w = 0x140;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80032ab8 (Movie_exit)                                          */
/* ------------------------------------------------------------------ */
/* DS stack frame: 0x38 bytes. s0..s3 are saved at sp+0x20..sp+0x2c and ra at sp+0x30. Rect occupies sp+0x10..0x17
 * and Rect2 sp+0x18..0x1f, filled by lwl/lwr + swl/swr copies of the templates at DS 0x80010ae0 and 0x80010ae8 as
 * in Movie_init; the word at sp+0x14 (Rect.w and Rect.h) is written again before the second LoadImage. */
static void Movie_exit(void)
{
	VM_DISP *pDisp = &Vm_disp;
	VM_WORK *pWork = &Vm_work;
	RECT    Rect  = { 0x140, 0x000, 0x0a0, 0x1e0 };    /* DS 0x80010ae0 */
	RECT    Rect2 = { 0x1c0, 0x0f0, 0x020, 0x0f0 };    /* DS 0x80010ae8 */
	tagGLOBAL *pG = &G;				/* one pointer to G keeps its address in a register */

	if (pDisp->Attribute & 8)
	{
		if (pDisp->Isrgb24 && pWork->Free0 == 0)
		{
			/* restore the VRAM saved by Movie_init */
			if (pDisp->Attribute & 4)
			{
				pG->pFree_work -= 0x3c10;
				LoadImage(&Rect2, (ULONG *)(((ULONG)pG->pFree_work + 15) & ~15));
				pG->pFree_work -= 0x12c10;
				/* w and h are one word store in retail (sw at Rect+4) */
				*(ULONG *)&Rect.w = 0x00f000a0;
				LoadImage(&Rect, (ULONG *)(((ULONG)pG->pFree_work + 15) & ~15));
				DrawSync(0);
				Bg_set_mode(0, 0);
				Main.Bg_chg_flg = 1;
			}
			else
			{
				pG->pFree_work -= 0x25810;
				LoadImage(&Rect, (ULONG *)(((ULONG)pG->pFree_work + 15) & ~15));
				DrawSync(0);
				Bg_set_mode(0, 0);
				Main.Bg_chg_flg = 2;
			}
		}
		else if (pDisp->Isrgb24 == 0 || pWork->Free0 == 1)
		{
			Main.Bg_chg_flg = 0;
			pG->System_flg   &= ~SYS_STR_MOVIE;
		}
	}
	else if (pWork->Free0 == 1)
	{
		G.System_flg &= ~(SYS_XA_MOVIE | SYS_STR_MOVIE);
	}

	pWork->Free0++;
}

/* ------------------------------------------------------------------ */
/* DS 0x80032ce8 (Open_movie)                                          */
/* Returns 0 when the first frame was received, 1 on time-out.          */
/* ------------------------------------------------------------------ */
/* DS 0x80032ce8 (Open_movie) */
/* DS stack frame: 0x40 bytes. s0..s3 are saved at sp+0x28..sp+0x34 and ra at sp+0x38. sp+0x10 carries the fifth
 * argument of StSetStream (the null second callback). Param is at sp+0x18: DS writes only Param[0] (0xa8) as a byte
 * and passes sp+0x18 to DsCommand. pBs_buff (sp+0x20) and pRead_header (sp+0x24) are the two output words of
 * StGetNext, read back with lw after the call. */
ULONG Open_movie(ULONG Movie_id)
{
	VM_DISP *pDisp = &Vm_disp;
	StHEADER    *pRead_header;
	ULONG       *pBs_buff;
	ULONG       Time_out;
	ULONG       Sector;
	FLOC        *pFpos;
	UCHAR       Param[4];

	Vm_work.End_frame  = Movie_tbl[Movie_id].N_frame;
	Vm_work.Cbuf_id    = 0;
	Vm_work.Rl_buff[0] = (ULONG *)&pAtv[1] + (MOVIE_RING_SIZE * 0x800) / 4;
	if (pDisp->Attribute & 0x100)
		Vm_work.Rl_buff[1] = (ULONG *)((UCHAR *)Vm_work.Rl_buff[0] + Movie_tbl[Movie_id].Rl_size);
	else
		Vm_work.Rl_buff[1] = Vm_work.Rl_buff[0];
	Vm_work.pImage_buff = (ULONG *)((UCHAR *)Vm_work.Rl_buff[1] + Movie_tbl[Movie_id].Rl_size);
	pVm_frm->Old_no = 0;
	pVm_frm->No     = 0;

	Movie_xa_stop();
	StSetRing((ULONG *)&pAtv[1], MOVIE_RING_SIZE);
	StSetStream(1, 0, 0xfffffff, NULL, NULL);

	Sector = Movie_tbl[Movie_id].Free;
	pFpos  = &floc[Movie_tbl[Movie_id].Filecode];
	Vm_work.Start_sector = (pFpos->sec_high << 16) + pFpos->sector;
	if (Sector)
		Vm_work.Start_sector += Sector * 5;
	DsIntToPos(Vm_work.Start_sector, &Loc);

	Channel.chan = 1;
	Channel.file = 1;
	Param[0] = 0xa8;                                    /* double speed | XA real-time | filter */
	DsCommand(DslPause,     NULL,               NULL, -1);
	DsCommand(DslSetmode,   Param,              NULL, -1);
	DsCommand(DslSetfilter, (UCHAR *)&Channel, NULL, -1);
	DsRead2(&Loc, 0x1c8);                               /* stream | speed | RT | SF */

	for (Time_out = 0; Time_out < 0xb5; Time_out++)
	{
		if (StGetNext((ULONG **)&pBs_buff, (ULONG **)&pRead_header) == 0)
		{
			DecDCTReset(0);
			DecDCToutCallback(Mdec_callback);
			DecDCTvlc((ULONG *)pBs_buff, (ULONG *)Vm_work.Rl_buff[Vm_work.Cbuf_id]);

			pDisp->Rect.h = pVm_frm->Pic_h = (pRead_header->height + 8) & 0xfff0;
			if (G.Movie_Isrgb24 == 1)
				pDisp->Size = pVm_frm->Pic_h * 12;
			else
				pDisp->Size = pVm_frm->Pic_h * 8;
			pDisp->Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
			pDisp->Loop  = ((UINT)pRead_header->width + 8) >> 4;
			pVm_frm->No = pVm_frm->Old_no = (USHORT)pRead_header->frameCount;
			StFreeRing((ULONG *)pBs_buff);
			return 0;
		}
		VSync(0);
	}
	return 1;
}

/* ------------------------------------------------------------------ */
/* DS 0x80032fcc (Play_movie)                                          */
/* Returns 0 while playing, 1 at end of movie, 2 on stream time-out.    */
/* ------------------------------------------------------------------ */
/* DS 0x80032fcc (Play_movie) */
/* DS stack frame: 0x30 bytes. s0..s3 are saved at sp+0x18..sp+0x24 and ra at sp+0x28. pBs_buff (sp+0x10) and
 * pRead_header (sp+0x14) are the two output words of StGetNext, read back with lw after the call. */
ULONG Play_movie(void)
{
	VM_DISP *pDisp = &Vm_disp;
	ULONG       *pBs_buff;
	StHEADER    *pRead_header;
	ULONG       Ret;
	ULONG       Time_out;

	for (Time_out = 0; StGetNext((ULONG **)&pBs_buff, (ULONG **)&pRead_header) != 0; )
	{
		VSync(0);
		if (++Time_out >= 0xb4)
		{
			Vm_work.Status = 1;
			return 2;
		}
	}

	pVm_frm->No = (USHORT)pRead_header->frameCount;
	Ret = 1;
	if (pVm_frm->No < Vm_work.End_frame && pVm_frm->No >= pVm_frm->Old_no)
		Ret = 0;
	pVm_frm->Old_no = pVm_frm->No;

	RuntoImage();
	Vm_work.Cbuf_id ^= 1;
	pDisp->Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
	pDisp->Loop  = ((UINT)pRead_header->width + 8) >> 4;
	DecDCTvlc((ULONG *)pBs_buff, (ULONG *)Vm_work.Rl_buff[Vm_work.Cbuf_id]);
	StFreeRing((ULONG *)pBs_buff);
	return Ret;
}

/* ------------------------------------------------------------------ */
/* DS 0x80033118 (Close_movie)                                         */
/* ------------------------------------------------------------------ */
void Close_movie(void)
{
	Movie_xa_stop();
	DecDCToutCallback(NULL);
	DsDataCallback(NULL);
	DsReadyCallback(NULL);
	DsCommand(DslPause, NULL, NULL, -1);
	StUnSetRing();
	SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF);
}

/* ------------------------------------------------------------------ */
/* DS 0x8003317c (RuntoImage)                                          */
/* ------------------------------------------------------------------ */
static void RuntoImage(void)
{
	VM_WORK *pWork = &Vm_work;
	VM_DISP *pDisp = &Vm_disp;

	pDisp->Cur_pos_x = pDisp->Pos_x;
	if (Main.Db[0].Draw.ofs[1] == 0)
		pDisp->Rect.y = (G.Cbuf_id != 0) ? 0xf0 : 0;
	else
		pDisp->Rect.y = (G.Cbuf_id == 0) ? 0xf0 : 0;
	pDisp->Count   = 0;
	pDisp->Rect.y += pDisp->Pos_y;
	DecDCTin((ULONG *)pWork->Rl_buff[pWork->Cbuf_id], G.Movie_Isrgb24);
	DecDCTout((ULONG *)pWork->pImage_buff, pDisp->Size);
	DecDCTinSync(0);
}

/* ------------------------------------------------------------------ */
/* DS 0x8003324c (Mdec_callback)                                       */
/* Upload one decoded 16/24 pixel column strip and request the next.    */
/* ------------------------------------------------------------------ */
/* DS 0x8003324c (Mdec_callback) */
static void Mdec_callback(void)
{
	if (StCdIntrFlag)
	{
		StCdInterrupt();
		StCdIntrFlag = 0;
	}
	Vm_disp.Rect.x = Vm_disp.Cur_pos_x;
	LoadImage(&Vm_disp.Rect, (ULONG *)Vm_work.pImage_buff);
	if (++Vm_disp.Count < Vm_disp.Loop)
	{
		Vm_disp.Cur_pos_x += Vm_disp.Width;
		DecDCTout((ULONG *)Vm_work.pImage_buff, Vm_disp.Size);
	}
	else
	{
		Vm_work.Movie_sync = 1;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x8003331c (Movie_xa_start) - fade the movie audio in            */
/* ------------------------------------------------------------------ */
static void Movie_xa_start(void)
{
	LONG    Ctr;

	for (Ctr = 0; Ctr <= (LONG)Vm_work.Movie_volume; Ctr++)
		Set_movie_volume2(Ctr);
}

/* ------------------------------------------------------------------ */
/* DS 0x80033370 (Movie_xa_stop) - fade the movie audio out            */
/* ------------------------------------------------------------------ */
static void Movie_xa_stop(void)
{
	LONG    Ctr;

	for (Ctr = Vm_work.Movie_volume; Ctr >= 2; Ctr -= 2)
		Set_movie_volume2(Ctr);
	Set_movie_volume2(0);
}

/* ------------------------------------------------------------------ */
/* DS 0x800333c4 (Set_movie_volume2)                                   */
/* ------------------------------------------------------------------ */
static void Set_movie_volume2(ULONG Volume)
{
	if (G.Sound_mode == 0)
	{
		/* stereo */
		pAtv->val0 = pAtv->val2 = (UCHAR)Volume;
		pAtv->val1 = pAtv->val3 = 0;
	}
	else
	{
		/* mono: mix both channels at half volume */
		pAtv->val0 = pAtv->val1 = pAtv->val2 = pAtv->val3 = (UCHAR)(Volume >> 1);
	}
	DsMix(pAtv);
}

/* ------------------------------------------------------------------ */
/* DS 0x80033440 (CdReset2)                                            */
/* Set the CD mode and seek back to the first file. 1 = ok, 0 = fail.   */
/* ------------------------------------------------------------------ */
/* DS 0x80033440 (CdReset2) */
/* DS stack frame: 0x20 bytes. s0 is saved at sp+0x18 and ra at sp+0x1c. Param is the single byte at sp+0x10 (0x90 or
 * 0xa8 by Mode); its address goes to DsControlB for the DslSetmode command. */
static LONG CdReset2(LONG Mode)
{
	UCHAR   Param;
	UCHAR   Status;
	ULONG   Time_out;

	Time_out = 0;
	if (Mode == 0)
		Param = 0x90;  /* double speed | size0 */
	else
		Param = 0xa8;  /* double speed | XA real-time | filter */
	while (1)
	{
		DsControlB(DslSetmode, &Param, pMovie_result);
		Status = *pMovie_result;
		if ((Status & 0x15) == 0)
			break;

		while (Status & 0x10)               /* shell open */
		{
			VSync(5);
			DsControlB(DslNop, NULL, pMovie_result);
			Time_out++;
			Status = *pMovie_result;
			if (Time_out >= 0x25)
				return 0;
		}
		Time_out = 0;
		if (Status & 2)                     /* motor on: retry */
			continue;

		do {                                /* wait for spindle */
			VSync(5);
			DsControlB(DslNop, NULL, pMovie_result);
			Time_out++;
			Status = *pMovie_result;
			if (Time_out >= 0x25)
				return 0;
		} while ((Status & 2) == 0);
	}

	VSync(3);
	DsIntToPos((floc[0].sec_high << 16) | floc[0].sector, &Loc);
	do {
		while (DsSync(1, NULL) != DslComplete);
	} while (DsControlB(DslSeekL, (UCHAR *)&Loc, NULL) == 0 || DsControlB(DslPause, NULL, NULL) == 0);
	return 1;
}

