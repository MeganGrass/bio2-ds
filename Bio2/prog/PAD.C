/*
 * PAD.C - Controller input and vibration module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PAD.C
 */
#include "TYPES.H"
#include <LIBGPU.H>
#include <LIBPAD.H>
#include <MALLOC.H>
#include "MOJIDISP.H"
#include "MAIN.H"
#include "PAD.H"
#include "CONFIG.H"
#include "ESP_R.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCE2.H"
#include "flag.h"

/* DS 0x800c3b40 (Pad_type_old) */
UCHAR Pad_type_old;

/* DS 0x800e898c (Pad_act_flg) */
UCHAR Pad_act_flg;

/* DS 0x800e8984 (Pad_type1): pad mode of port 1 from the pad driver (4 digital, 7 analog). Zero at start; DS
 * allocates it with the common symbols in .bss, so it has no initialiser here. */
UCHAR Pad_type1;

/* Vibration request queues: DS 0x800eaad8 (small motor), 0x800eac28 (large motor) */
static VIB_REQ Vib_req0[33];
static VIB_REQ Vib_req1[33];

/* DS 0x800a268c, 0x800a2690: last request slot handed out for the small / large motor queue */
static VIB_REQ *Vib_cur0 = Vib_req0;
static VIB_REQ *Vib_cur1 = Vib_req1;

/* DS 0x800a2694 (s__800a2694): pad speed scale, indexed by UnkMvPadFlg (12 bytes). Read by the PLAYER.C walk, run, back-walk
 * and turn routines; DS has it in PAD's .data, between Vib_cur1 and Key_type. */
UCHAR  s__800a2694[12] = {
	0x00, 0x10, 0x1f, 0x2d, 0x3a, 0x48, 0x52, 0x5a,
	0x5e, 0x62, 0x69, 0x00,
};

/* DS 0x800a26a0 */
USHORT Key_type[3][16] = {
	{ 0x1000, 0x2000, 0x4000, 0x8000, 0x1000, 0x4000, 0x0040, 0x0040, 0x0008, 0x0080, 0x8000, 0x2000, 0x0040, 0x0010, 0x0020, 0x0000 },
	{ 0x1000, 0x2000, 0x4000, 0x8000, 0x1000, 0x4000, 0x0040, 0x0040, 0x0008, 0x0080, 0x8000, 0x2000, 0x0040, 0x0090, 0x0020, 0x0000 },
	{ 0x1000, 0x2000, 0x4000, 0x8000, 0x1000, 0x4000, 0x0040, 0x0040, 0x0008, 0x0080, 0x8000, 0x2000, 0x0040, 0x0010, 0x0020, 0x0000 },
};

/* DS 0x80038bbc (Pad_set) */
void Pad_set(void)
{
	static int   Stp_bak;		/* G.Stop_flg saved while the game is stopped */
	static UCHAR Key_wait;		/* key-repeat countdown, DS 0x800a2704 */

	ULONG  Pad_raw;				/* inverted raw pad word built from Padd_data */
	UINT   Vib_mode = Vib_mode;		/* PadInfoMode(0,2,0); DS leaves it unset (s1) when PadGetState < 2 */
	UINT   PadState;				/* DS compares it unsigned (sltiu) */
	UINT   Cnt;					/* key-bit loop counter */
	UCHAR  Stk_x = Stk_x;			/* analog stick X, Padd_data[0][6] (mode 7 only; unset otherwise in DS) */
	UCHAR  Stk_y = Stk_y;			/* analog stick Y, Padd_data[0][7] (mode 7 only; unset otherwise in DS) */
	UCHAR  Stk_act = Stk_act;			/* stick pushed outside centre this frame */
	LONG   Stk_div;					/* Stk_x as a signed int for the divide by 8 (DS divides as signed int) */
	UCHAR  Pow_sm;				/* small-motor power from SetPadActData */
	UCHAR  Pow_lg;				/* large-motor power from SetPadActData */

	Pad_act_flg = 0;
	PadState = PadGetState(0);
	if (1 < PadState)
	{
		Pad_type1 = (char)PadInfoMode(0, 1, 0);
		Vib_mode     = PadInfoMode(0, 2, 0);
		if (Pad_type_old != Pad_type1)
			UnknownPadSetFlag = 0;

		/* DS has no flag for "pad mode accepted": mode 4 or 7 jumps over the reset below */
		if ((Pad_type1 == 4) || (Pad_type1 == 7))
		{
			if (Pad_type1 == 7)
			{
				Stk_x = G.Padd_data[0][6];
				Stk_y = G.Padd_data[0][7];
			}
			if (UnknownPadSetFlag == 0)
			{
				PadSetAct(0, PadActData, 2);
				if (PadState == 2)
					UnknownPadSetFlag = 1;
				else if ((PadState == 6) && (PadSetActAlign(0, Pad_act_align) != 0))
					UnknownPadSetFlag = 2;
			}
			goto pad_accepted;
		}
	}

	G.Padd_data[0][3] = 0xff;
	G.Padd_data[0][2] = 0xff;
	G.Padd_data[1][3] = 0xff;
	G.Padd_data[1][2] = 0xff;
	UnknownPadSetFlag = 0;
	Pad_type1      = 0;
pad_accepted:
	Pad_type_old = Pad_type1;

	if ((Vib_mode & 0xff) == 0)
	{
		if (G.Dummy8_3[0] == 0)
		{
			PadActData[1] = 0;
			PadActData[0] = 0;
			if ((((Moji.Mess_flg & 0x80) == 0) || ((G.Status_flg & STAT_CINESCO) != 0)) && ((G.System_flg & SYS_DEMO_MODE) == 0))
			{
				Pow_sm = SetPadActData(Vib_req0);
				Pow_lg = SetPadActData(Vib_req1);
				if ((Pow_sm != 0) || (Pow_lg != 0))
				{
					PadActData[0] = 64;
					PadActData[1] = 0xff;
				}
			}
		}
	}
	else if (G.Dummy8_3[0] == 0)
	{
		PadActData[1] = 0;
		PadActData[0] = 0;
		if ((((Moji.Mess_flg & 0x80) == 0) || ((G.Status_flg & STAT_CINESCO) != 0)) && ((G.System_flg & SYS_DEMO_MODE) == 0))
		{
			PadActData[0] = SetPadActData(Vib_req0);
			PadActData[1] = SetPadActData(Vib_req1);
		}
	}

	if ((G.Sound_mode & 0x80) != 0)
	{
		PadActData[1] = 0;
		PadActData[0] = 0;
	}

	G.Padd_trg = G.Padd;
	Pad_raw    = ~(((UINT)G.Padd_data[1][2] * 0x1000000 + (UINT)G.Padd_data[1][3] * 0x10000 + (UINT)G.Padd_data[0][2] * 0x100) | (UINT)G.Padd_data[0][3]);

	if ((G.System_flg & SYS_DEMO_MODE) == 0)
	{
		G.Padd = Pad_raw;
	}
	else
	{
		G.Padd = Pad_raw;
		if ((((G.Padd_trg ^ Pad_raw) & Pad_raw & 0xc00) == 0) && ((UINT)G.Pdemo_cnt < (UINT)Demo.Pdemo_cnt))
		{
			if ((G.System_flg & SYS_IN_GAME) != 0)
			{
				G.Padd      = (ULONG)Demo.Padd[G.Pdemo_cnt];
				G.Pdemo_cnt++;
			}
		}
		else if (UnknownPadInfoByte == 0)
		{
			if ((G.System_flg & SYS_IN_GAME) != 0)
			{
				if (G.Pdemo_cnt < Demo.Pdemo_cnt)
					Main.Padoff_stop_flg[1][1] = 1;
				G.Padd      = 0;
				G.Pdemo_cnt = Demo.Pdemo_cnt + 1;
			}
			else
				G.System_flg |= SYS_RESET;
		}
	}

	if ((G.System_flg & SYS_DEMO_MODE) == 0)
	{
		if ((G.Padd & 0xa000) != 0)
			Pad_act_flg = 1;

		if (Pad_type1 == 7)
		{
			UnkMvPadFlg = 0;
			Stk_act     = 0;

			if (Stk_y < 0x30)
				G.Padd |= 0x1000;
			if (0xd0 < Stk_y)
				G.Padd |= 0x4000;

			if (Stk_x < 0x50)
			{
				if (Stk_x == 0)
					UnkMvPadFlg = 10;
				else
				{
					Stk_x       = 0x50 - Stk_x;
					Stk_div     = Stk_x;
					UnkMvPadFlg = Stk_div / 8;	/* DS divides as signed int (bgez/addiu 7/sra) */
				}
				Stk_act |= 1;
				G.Padd |= 0x8000;
			}

			if (0xb0 < Stk_x)
			{
				UnkMvPadFlg = 10;
				if (Stk_x != 0xff)
				{
					Stk_x += 0x50;
					Stk_div     = Stk_x;
					UnkMvPadFlg = Stk_div / 8;
				}
				Stk_act |= 1;
				G.Padd |= 0x2000;
			}

			if ((G.Padd & 0x400) != 0)
				G.Padd |= 0x800;
		}
	}
	else
	{
		Pad_act_flg = 1;
	}

	G.Key_old = G.Key;
	G.Key = 0;
	Cnt   = 0x10;
	do {
		Cnt--;
		if ((G.Padd & Key_type[G.SaveData.Key_idx][(UCHAR)Cnt]) != 0)
			G.Key = G.Key | 1 << (Cnt & 0x1f);
	} while ((UCHAR)Cnt != 0);

	if ((G.Stop_flg & 0x1000000) == 0)
	{
		if ((Stp_bak & 0x1000000U) != 0)
		{
			Stp_bak   = 0;
			G.Key_old = G.Key;
		}
	}
	else
	{
		Stp_bak = G.Stop_flg;
		G.Key &= 0x3c00;
	}

	if (((G.System_flg & SYS_DEMO_MODE) == 0) && (Pad_type1 == 7))
	{
		if (Stk_act == 0)
		{
			if ((G.Key & 10) != 0)
				UnkMvPadFlg = 10;
		}
		else
		{
			if (Stk_x != 0xff)
			{
				G.Padd &= 0xffffdfff;
				G.Padd_trg &= 0xffffdfff;
			}
			if (Stk_x != 0)
			{
				G.Padd &= 0xffff7fff;
				G.Padd_trg &= 0xffff7fff;
			}
		}
	}

	if ((char)Pad_act_flg != 0)	/* DS reads this flag with lb */
		UnkMvPadFlg = 10;

	G.Padd_trg  = (G.Padd_trg ^ G.Padd) & G.Padd;
	G.Key_trg   = (G.Key_old ^ G.Key) & G.Key;
	G.Padd2     = (USHORT)(G.Padd >> 0x10);
	G.Padd1_trg = (USHORT)G.Padd_trg;
	G.Padd1     = (USHORT)G.Padd;
	G.Padd2_trg = (USHORT)(G.Padd_trg >> 0x10);

	if ((G.Padd_trg & Main.Pad_rmask) == 0)
	{
		if (Key_wait == 0)
		{
			Key_wait     = Main.Pad_repeat_time;
			G.System_flg |= SYS_PAD_REPEAT;
		}
		else
		{
			if ((G.Padd & Main.Pad_rmask) != 0)
				Key_wait--;
			G.System_flg &= ~SYS_PAD_REPEAT;
		}
	}
	else
	{
		Key_wait     = Main.Pad_wait_time;
		G.System_flg |= SYS_PAD_REPEAT;
	}
}


/* DS 0x80039464 (Pad_rep_set): Mask -> Main.Pad_rmask, Data low byte -> Pad_wait_time, high byte -> Pad_repeat_time */
void Pad_rep_set(ULONG Mask, USHORT Data)
{
	Main.Pad_rmask       = Mask;
	/* DS stores both bytes with one sh (wait low byte, repeat high byte) */
	*(USHORT *)&Main.Pad_wait_time = Data;
}

/* DS 0x8003947c (Pad_vib_set0) */
void Pad_vib_set0(int Time, USHORT Attr)
{
	VIB_REQ *pReq = Vib_req0;	/* DS keeps the queue address in s1 */

	if (((UINT)Pad_vib_count(pReq) < 32) && (Time != 0))
	{
		Vib_cur0        = Pad_vib_get_free(pReq);
		Vib_cur0->Flg   = 1;
		/* Time is assigned before Pow: DS stores Time through the same reload of Vib_cur0 that it
		 * uses for the Pow byte, and reloads Vib_cur0 once more for Delay and Step. */
		Vib_cur0->Time  = (short)Time;
		Vib_cur0->Pow   = (Time != 0);
		Vib_cur0->Delay = Attr;
		Vib_cur0->Step  = 0;
	}
}

/* DS 0x80039514 (Pad_vib_set1) */
void Pad_vib_set1(int Time, UCHAR Power, USHORT Attr)
{
	VIB_REQ *pReq = Vib_req1;	/* DS keeps the queue address in s0 */

	if (((UINT)Pad_vib_count(pReq) < 32) && (Time != 0))
	{
		Vib_cur1        = Pad_vib_get_free(pReq);
		Vib_cur1->Flg   = 1;
		Vib_cur1->Pow   = Power;
		Vib_cur1->Time  = (short)Time;
		Vib_cur1->Delay = Attr;
		Vib_cur1->Step  = 0;
	}
}

/* DS 0x800395b8 (Pad_vib_fade_set) */
void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr)
{
	VIB_REQ *pReq = Vib_req1;	/* DS keeps the queue address in s2 */
	short    Step;

	if (((UINT)Pad_vib_count(pReq) < 32) && (Time != 0))
	{
		Vib_cur1          = Pad_vib_get_free(pReq);
		Step              = (short)((Pow_ed - Pow_st) * 0x80 / Time);
		Vib_cur1->Flg     = 1;
		Vib_cur1->Time    = (short)Time;
		Vib_cur1->Delay   = Attr;
		Vib_cur1->Pow_cur = (USHORT)(Pow_st << 7);
		Vib_cur1->Step    = Step;
	}
}

/* DS 0x80039694 (Pad_vib_init) */
void Pad_vib_init(void)
{
	UINT i;

	for (i = 0; i < 33; i++)
	{
		Vib_req0[i].Flg = 0;
		Vib_req1[i].Flg = 0;
	}
	Vib_cur0 = Vib_req0;
	Vib_cur1 = Vib_req1;
	PadActData[1] = 0;
	PadActData[0] = 0;
}

/* DS 0x800396fc (SetPadActData): advance active requests one frame, return max motor power */
UCHAR SetPadActData(VIB_REQ *pReq)
{
	UINT   i;
	UCHAR  Max = 0;
	UCHAR *pB;			/* DS addresses the record fields from a cursor one byte past Flg */
	short  Step;
	ULONG  Stk_rsv[2];		/* 8-byte stack block (addiu sp,-8), never accessed */
	(void)Stk_rsv;

	for (i = 0; i < 32; i++, pReq++)
	{
		if (pReq->Flg == 0)
			continue;

		pB = &pReq->Pow;
		if (*(USHORT *)(pB + 1) == 0)
		{
			(*(USHORT *)(pB + 3))--;
			if (*(USHORT *)(pB + 3) == 0)
				pReq->Flg = 0;

			Step = *(short *)(pB + 5);
			if (Step != 0)
			{
				*(USHORT *)(pB + 7) += Step;
				*pB = *(USHORT *)(pB + 7) >> 7;
			}
			if (Max < *pB)
				Max = *pB;
		}
		else
		{
			(*(USHORT *)(pB + 1))--;
		}
	}
	return Max;
}

/* DS 0x800397b8 (Pad_vib_get_free): first free slot, or the end sentinel if all 32 are busy */
VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq)
{
	ULONG i;

	for (i = 0; i < 32; i++, pReq++)
	{
		if (pReq->Flg == 0)
			return pReq;
	}
	return pReq;
}

/* DS 0x800397e4 (Pad_vib_count): number of active requests */
int Pad_vib_count(VIB_REQ *pReq)
{
	ULONG i;
	int Cnt = 0;

	for (i = 0; i < 32; i++, pReq++)
	{
		if (pReq->Flg != 0)
			Cnt++;
	}
	return Cnt;
}


/* DS 0x800e2a80 (UnknownPadSetFlag) */
UCHAR UnknownPadSetFlag;

/* DS 0x800cbc20 (PadActData) */
UCHAR PadActData[2];

/* DS 0x800d7654 (UnknownPadInfoByte) */
UCHAR UnknownPadInfoByte;
