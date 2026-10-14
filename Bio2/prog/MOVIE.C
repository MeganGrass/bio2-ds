/*
 * MOVIE.C - STR movie playback (DS streaming + MDEC)
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */

#define _MOVIE_C_
#include "TYPES.H"
#include <LIBAPI.H>
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
DslFILTER   Channel = { 0, 0, 0 };				/* file, chan, pad */

/* DS 0x800d75d8 (Vm_disp) */
VM_DISP     Vm_disp =
{
	{ 0, 0, 0, 0 },						/* 0x00 Rect (x, y, w, h) */
	{									/* 0x08 Cinesco[2][2] (TILE) */
		{ { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
		{ { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 } }
	},
	0,									/* 0x48 Cine_switch */
	0,									/* 0x4c Pos_x */
	0,									/* 0x4e Cur_pos_x */
	0,									/* 0x50 Pos_y */
	0,									/* 0x52 Width */
	0,									/* 0x54 Size */
	0,									/* 0x56 Count */
	0,									/* 0x58 Loop */
	0,									/* 0x5a Attribute */
	0									/* 0x5c Isrgb24 */
};

/* DS 0x800d763c (pVm_frm) */
VM_FRAME    *pVm_frm = 0;

/* DS 0x800d7670 (Vm_work) */
VM_WORK     Vm_work =
{
	{ 0, 0 },							/* 0x00 Rl_buff[2] */
	0,									/* 0x08 pImage_buff */
	0,									/* 0x0c End_frame */
	0,									/* 0x0e Movie_volume */
	0,									/* 0x10 Rno */
	0,									/* 0x11 Cbuf_id */
	0,									/* 0x12 Movie_w */
	0,									/* 0x14 Movie_sync */
	0,									/* 0x15 Status */
	0,									/* 0x16 Eom */
	0,									/* 0x17 Free0 */
	0,									/* 0x18 Frame */
	0,									/* 0x1a Volume */
	0,									/* 0x1c Start_sector */
	0									/* 0x20 Movie_cancel */
};

/* DS 0x800d8cd8 (Loc) */
DslLOC      Loc = { 0, 0, 0, 0 };

/* DS 0x800df338 (pMovie_result) */
UCHAR       *pMovie_result = 0;

/* DS 0x800e2a88 (Ret_Value) */
UCHAR       Ret_Value[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/* DS 0x800e8980 (pAtv) */
DslATV      *pAtv = 0;

/* DS 0x800eae34 (pFile) */
DslFILE     *pFile = 0;

/* ------------------------------------------------------------------ */
/* DS 0x80032150 (Init_movie_work)                                     */
/* ------------------------------------------------------------------ */
void Init_movie_work(ULONG Id)
{
	ULONG       *pBuff;
	MOVIE_TBL   *pMovie_tbl;
	ULONG       unused_slot[2];

	(void)unused_slot;
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
	pVm_frm = (VM_FRAME *)pBuff;            /* any other Work_top leaves pBuff unset (as retail) */

	G.Movie_id      = (UCHAR)Id;
	G.Movie_Isrgb24 = pMovie_tbl->Isrgb24;
	pFile           = (DslFILE *)(pVm_frm + 1);
	pAtv            = (DslATV *)(pVm_frm + 4);
	Vm_work.Movie_w = pMovie_tbl->Disp_w;
	pMovie_result   = Ret_Value;
	Vm_work.Rno        = 0;
	Vm_work.Movie_sync = 0;

	Vm_disp.Rect.w = 16;
	if (G.Movie_Isrgb24 == 1)
		Vm_disp.Rect.w = 24;
	Vm_disp.Pos_x     = (USHORT)pMovie_tbl->Pos;
	Vm_disp.Pos_y     = (USHORT)(pMovie_tbl->Pos >> 16);
	Vm_disp.Attribute = pMovie_tbl->Attribute;
	Vm_disp.Isrgb24   = pMovie_tbl->Isrgb24;

	if ((Vm_disp.Attribute & 2) == 0)
	{
		Vm_work.Eom = 0;
	}
	else if (Vm_work.Eom == 2)
	{
		/* previous movie of a chain was skipped: skip this one too */
		Vm_work.Eom = 1;
		Main.Padoff_stop_flg[1][1] = 2;
		Vm_disp.Attribute = (Vm_disp.Attribute & ~0x80) | 0x40;
	}
	else
	{
		Vm_work.Eom = 0;
		Main.Padoff_stop_flg[1][1] = 0;
	}

	Vm_work.Frame        = 0;
	Vm_work.Free0        = 0;
	Vm_work.Movie_volume = ((UINT)pMovie_tbl->Volume * (UINT)Volume_Bgm) / 100;
	Vm_work.Movie_cancel = pMovie_tbl->Cancel;
	G.System_flg         = (G.System_flg & ~SYS_XA_MOVIE) | SYS_STR_MOVIE;
	Vm_disp.Width        = Vm_disp.Rect.w;
	Pad_vib_init();
}

/* ------------------------------------------------------------------ */
/* DS 0x80032340 (Movie)                                               */
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
static void Movie_init(void)
{
	RECT    Rect  = { 0x140, 0x000, 0x0a0, 0x1e0 };    /* DS 0x80010ae0 */
	RECT    Rect2 = { 0x1c0, 0x0f0, 0x020, 0x0f0 };    /* DS 0x80010ae8 */

	if (Vm_disp.Attribute & 8)
	{
		/* movie drawn over the game screen: save the VRAM it will overwrite */
		Set_movie_volume2(Vm_work.Movie_volume);
		SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
		if (Vm_disp.Isrgb24 == 1)
		{
			if (Vm_disp.Attribute & 4)
			{
				Rect.w = 0xa0;
				Rect.h = 0xf0;
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
			Vm_work.Status = 1;
			Vm_work.Rno += 2;
		}
		else
		{
			Vm_work.Status = 0;
			Vm_work.Frame  = 0;
			Vm_work.Rno++;
		}
	}
	else if (Vm_work.Eom)
	{
		Movie_end();
		Vm_work.Rno += 2;
	}
	else
	{
		Set_movie_volume2(Vm_work.Movie_volume);
		SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
		if (Open_movie(G.Movie_id))
		{
			Movie_end();
			Vm_work.Status = 1;
			Vm_work.Rno += 2;
		}
		else
		{
			Vm_work.Status = 0;
			if (Vm_disp.Attribute & 1)
			{
				Fade_off(0);
				Bg_set_mode(1, 0);
			}
			Vm_work.Frame = 0;
			Vm_work.Rno++;
		}
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80032700 (Movie_main)                                          */
/* ------------------------------------------------------------------ */
static void Movie_main(void)
{
	SHORT   Eom;
	RECT    Rect;

	DrawSync(0);
	VSync(2);
	PutDrawEnv(&Main.Db[G.Cbuf_id].Draw);
	PutDispEnv(&Main.Db[G.Cbuf_id].Disp);

	if (Vm_work.Frame < 2)
	{
		/* clear the half of VRAM the next frame will be decoded into */
		Rect.x = 0;
		if (Main.Db[0].Draw.ofs[1] == 0)
			Rect.y = (G.Cbuf_id != 0) ? 0xf0 : 0;
		else
			Rect.y = (G.Cbuf_id == 0) ? 0xf0 : 0;
		if (Vm_disp.Isrgb24 == 1)
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

		if (Vm_work.Frame == 0)
		{
			G.System_flg |= SYS_XA_MOVIE;
			if (G.Movie_id != 0x11)
				Movie_xa_start();
		}
	}

	Vm_work.Frame++;
	Eom = (SHORT)Play_movie();

	/* Retail: if neither the stream status nor the MDEC sync flag is set yet,
	   the code is a branch-to-self and never re-reads the flags. */
	if (Vm_work.Status == 0 && Vm_work.Movie_sync == 0)
	{
		for (;;);
	}

	if (Eom == 1)
	{
		Vm_work.Eom = 1;
		Main.Padoff_stop_flg[1][1] = 0;
	}
	else if (Eom != 0
		  || (((Vm_disp.Attribute & 2) || Vm_work.Frame >= 61)
			  && Vm_work.Movie_cancel
			  && (Pad_set(), G.Padd & 0x800)))
	{
		/* error, or cancelled with the START/skip button */
		Vm_work.Eom = 2;
		Main.Padoff_stop_flg[1][1] = 2;
		Vm_disp.Attribute = (Vm_disp.Attribute & ~0x80) | 0x40;
	}

	if (Vm_work.Eom && (Vm_work.Frame & 1))
	{
		Vm_work.Frame = 0;
		Vm_work.Rno++;
		Movie_end();
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80032984 (Movie_end)                                           */
/* ------------------------------------------------------------------ */
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
static void Movie_exit(void)
{
	RECT    Rect  = { 0x140, 0x000, 0x0a0, 0x1e0 };    /* DS 0x80010ae0 */
	RECT    Rect2 = { 0x1c0, 0x0f0, 0x020, 0x0f0 };    /* DS 0x80010ae8 */

	if (Vm_disp.Attribute & 8)
	{
		if (Vm_disp.Isrgb24 && Vm_work.Free0 == 0)
		{
			/* restore the VRAM saved by Movie_init */
			if (Vm_disp.Attribute & 4)
			{
				G.pFree_work -= 0x3c10;
				LoadImage(&Rect2, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				Rect.w = 0xa0;
				Rect.h = 0xf0;
				G.pFree_work -= 0x12c10;
				LoadImage(&Rect, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				DrawSync(0);
				Bg_set_mode(0, 0);
				Main.Bg_chg_flg = 1;
			}
			else
			{
				G.pFree_work -= 0x25810;
				LoadImage(&Rect, (ULONG *)(((ULONG)G.pFree_work + 15) & ~15));
				DrawSync(0);
				Bg_set_mode(0, 0);
				Main.Bg_chg_flg = 2;
			}
		}
		else if (Vm_disp.Isrgb24 == 0 || Vm_work.Free0 == 1)
		{
			Main.Bg_chg_flg = 0;
			G.System_flg   &= ~SYS_STR_MOVIE;
		}
	}
	else if (Vm_work.Free0 == 1)
	{
		G.System_flg &= ~(SYS_XA_MOVIE | 0x200);
	}

	Vm_work.Free0++;
}

/* ------------------------------------------------------------------ */
/* DS 0x80032ce8 (Open_movie)                                          */
/* Returns 0 when the first frame was received, 1 on time-out.          */
/* ------------------------------------------------------------------ */
/* DS 0x80032ce8 (Open_movie) */
ULONG Open_movie(ULONG Movie_id)
{
	StHEADER    *pRead_header;
	ULONG       *pBs_buff;
	ULONG       Time_out;
	ULONG       Sector;
	FLOC        *pFpos;
	UCHAR       Param[4];

	Vm_work.End_frame  = Movie_tbl[Movie_id].N_frame;
	Vm_work.Cbuf_id    = 0;
	Vm_work.Rl_buff[0] = (ULONG *)&pAtv[1] + (MOVIE_RING_SIZE * 0x800) / 4;
	if (Vm_disp.Attribute & 0x100)
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

			Vm_disp.Rect.h = pVm_frm->Pic_h = (pRead_header->height + 8) & 0xfff0;
			if (G.Movie_Isrgb24 == 1)
				Vm_disp.Size = pVm_frm->Pic_h * 12;
			else
				Vm_disp.Size = pVm_frm->Pic_h * 8;
			Vm_disp.Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
			Vm_disp.Loop  = (pRead_header->width + 8) >> 4;
			pVm_frm->Old_no = pVm_frm->No = (USHORT)pRead_header->frameCount;
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
ULONG Play_movie(void)
{
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
	Vm_disp.Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
	Vm_disp.Loop  = (pRead_header->width + 8) >> 4;
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
	Vm_disp.Cur_pos_x = Vm_disp.Pos_x;
	if (Main.Db[0].Draw.ofs[1] == 0)
		Vm_disp.Rect.y = (G.Cbuf_id != 0) ? 0xf0 : 0;
	else
		Vm_disp.Rect.y = (G.Cbuf_id == 0) ? 0xf0 : 0;
	Vm_disp.Count   = 0;
	Vm_disp.Rect.y += Vm_disp.Pos_y;
	DecDCTin((ULONG *)Vm_work.Rl_buff[Vm_work.Cbuf_id], G.Movie_Isrgb24);
	DecDCTout((ULONG *)Vm_work.pImage_buff, Vm_disp.Size);
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
static LONG CdReset2(LONG Mode)
{
	UCHAR   Param;
	UCHAR   Status;
	ULONG   Time_out;

	Time_out = 0;
	Param = Mode ? 0xa8     /* double speed | XA real-time | filter */
				 : 0x90;    /* double speed | size0 */
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
	} while (DsControlB(DslSeekL, (UCHAR *)&Loc, NULL) == 0
		  || DsControlB(DslPause, NULL, NULL) == 0);
	return 1;
}

