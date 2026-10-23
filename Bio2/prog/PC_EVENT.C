/*
 * PC_EVENT.C - In-game computer terminal event handler
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PC_EVENT.C
 */
#include "TYPES.H"
#include <LIBGTE.H>
#include "MAIN.H"
#include "CONFIG.H"
#include "PC_SYS.H"
#include "PC_EVENT.H"
#include "ESP_R.H"
#include "MEM_CARD.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "PAD.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "SUB.H"
#include "DOOR.H"
#include "GAME.H"
#include "flag.h"

/* DS 0x80010bcc */
const UCHAR s_DOOR_LOCK_SERVICE________________80010bcc[52] = "DOOR LOCK SERVICE]------------------------------\0\0\0";

/* Computer200/Computer613 message strings (retail rodata; declared in PC_EVENT.H) */
/* DS 0x80010c00 */
const UCHAR s_Hall_side_doors___2RELEASED_0__80010c00[36] = "Hall side doors:      |2RELEASED|0]";
/* Wait: declared but never read or written by the code in this file. */
static char   Wait;
/* Kend: key-end flag, set to 1 when the player ends the current text page with a button or key press.
 * DS keeps one copy per terminal function: Computer200 uses 0x800a3e7c, Computer613 uses 0x800a3e88. */
static ULONG  Kend;
/* Mend: message-end flag, filled in by Cprint through its out parameter when the text page has been drawn completely.
 * DS keeps one copy per terminal function: Computer200 uses 0x800a3e80, Computer613 uses 0x800a3e8c. */
static ULONG  Mend;
/* DS 0x80010c24 */
const UCHAR s_Hall_side_doors___2LOCKED_0__80010c24[36] = "Hall side doors:      |2LOCKED|0]\0\0";
/* DS 0x80010c48 (48 bytes: the string ends where s_Pc_event_msg_80010c78 begins) */
const UCHAR s__The_doors_can_be_unlocked_by_a___80010c48[48] = "]The doors can be unlocked]by a |2CARD KEY|0.]\0";
static UCHAR  Key_mode;			/* 0: keyboard input, 1: wait for any pad button (PAD.H owns Key_type[][16]) */

/* DS 0x80010c78 */
UCHAR s_Pc_event_msg_80010c78[4] = {
	93, 0, 0, 0
};
/* DS 0x80010c7c */
const UCHAR s__Checking_up_ID_CARD_025__025__0_80010c7c[44] = "]Checking up ID CARD[025.[025.[025.[025.\0\0\0";
/* DS 0x80010ca8 */
const UCHAR s__OK__Hall_side_doors_lock_releas_80010ca8[36] = "]OK!]Hall side doors lock released.";
/* DS 0x80010ccc */
UCHAR s_press_any_key___80010ccc[16] = "press any key]>";
/* DS 0x80010d0c */
const UCHAR s_Register_your_fingerprint___80010d0c[28] = "Register your fingerprint.]";
/* DS 0x80010d28 */
const UCHAR s_UMBRELLA__RODEM__SYSTEM__Ver_5_0_80010d28[56] = "UMBRELLA \"RODEM\" SYSTEM]                      Ver 5.0]\0";
/* DS 0x80010d60 */
const UCHAR s_Culture_Experiment_Room_Staff_Re_80010d60[44] = "Culture Experiment Room Staff Registry.]]\0\0";
/* DS 0x80010d8c */
const UCHAR s_Accessing_020__020__020__020__02_80010d8c[40] = "Accessing[020.[020.[020.[020.[020.]]\0\0\0";
/* DS 0x80010db4 */
const UCHAR s_Enter_your_user_name__80010db4[24] = "Enter your user name.]>";
/* DS 0x80010dcc */
UCHAR s_GUEST_80010dcc[8] = "GUEST\0\0";
/* DS 0x80010dd4 */
const UCHAR s__1This_name_is_not_in_the_resear_80010dd4[52] = "|1This name is not in the]researcher data base.|0]\0";
/* DS 0x80010e08 */
const UCHAR s_Register_your_fingerprint__80010e08[28] = "Register your fingerprint.\0";
/* DS 0x80010e24 */
const UCHAR s_Please_wait_025__025__025__025___80010e24[40] = "Please wait[025.[025.[025.[025.[025.]\0\0";
/* DS 0x80010e4c */
const UCHAR s_Registration_complete____80010e4c[28] = "Registration complete.]]\0\0\0";
/* DS 0x80010e68 */
const UCHAR s_Guest_registration_is_valid_for_2_80010e68[44] = "Guest registration is valid]for 24 hours.]\0";


static char   Id_card_work;		/* DS 0x800c3b48 */
static UCHAR *pM1;				/* DS 0x800c3b50 */
static UCHAR *pM2;				/* DS 0x800c3b54 */

/* DS stack frame: 0x28 bytes; s0..s3 saved at sp+0x10..0x1c, ra saved at sp+0x20. No local variable slots are
 * used. */
/* DS 0x8003a9f4 (Computer200) */
void Computer200(void)
{
	ULONG     Flg_ret;
	long      Item;
	UINT      Rtn_bak;
	short     Cut_old;
	int       i;

	if (Wframe_in_out() == 0)
	{
		if (Key_mode == 0)
		{
			if ((Mend != 0) && ((G.Key & 0x1000) != 0))
				Kend = 1;
		}
		else if ((Key_mode == 1) && (Mend != 0))
		{
			if ((G.Padd1_trg & 0xf0) != 0)
				Kend = 1;
			if ((G.Padd1_trg & 0xf) != 0)
				Kend = 1;
		}

		if (Kend != 0)
		{
			pM2 = (UCHAR *)0;
			pM1 = (UCHAR *)0;
		}

		/* DS reads G.Up_eve_rtn here (s3), after the key test; the Ret != 0 exit below compares it with an unset value, as DS does */
		Rtn_bak = G.Up_eve_rtn;

		switch (Rtn_bak)
		{
		case C200_INIT:
			pCom_work    = (COM_WORK *)G.pFree_work;
			G.pFree_work = G.pFree_work + sizeof(COM_WORK);
			Computer_init((UINT)(G.Window_no >> 4), 1);
			Item = Search_item_id(0x35);
			Id_card_work = (char)Item;
			if ((signed char)Item < 0)
			{
				Item = Search_item_id(0x35);
				Id_card_work = (char)Item;
			}
			Mend         = 0;
			Kend         = 0;
			G.Up_eve_rtn = 1;
			/* fall through */

		case 1:
			pM2 = pc_mes_data + pc_mes_ofs[0];
			Cprint(1, (UCHAR *)s_DOOR_LOCK_SERVICE________________80010bcc, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 2;
			break;

		case 2:
			Flg_ret = Flg_ck(G.SaveData.Common_flg, 0x46);
			if (Flg_ret != 0)
			{
				pM1 = pc_mes_data + pc_mes_ofs[1];
				pM2 = pc_mes_data + pc_mes_ofs[2];
				Cprint(1, (UCHAR *)s_Hall_side_doors___2RELEASED_0__80010c00, &Mend);
				if ((Mend & Kend) != 0)
					G.Up_eve_rtn = 11;
				break;
			}
			pM1 = pc_mes_data + pc_mes_ofs[3];
			pM2 = pc_mes_data + pc_mes_ofs[4];
			Cprint(1, (UCHAR *)s_Hall_side_doors___2LOCKED_0__80010c24, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 3;
			break;

		case 3:
			pM1 = pc_mes_data + pc_mes_ofs[5];
			pM2 = pc_mes_data + pc_mes_ofs[6];
			Cprint(1, (UCHAR *)s__The_doors_can_be_unlocked_by_a___80010c48, &Mend);
			if ((Mend & Kend) != 0)
			{
				if (Flg_ck(G.SaveData.Common_flg, 0x46) != 0)
					G.Up_eve_rtn = 11;
				else
					G.Up_eve_rtn = 4;
			}
			break;

		case 4:
			Item = Search_item_id(0x35);
			if (Item >= 0)
			{
				Moji.Mess_item = 53;
				Mess_set(0, 0x100, 7, 0xff000000);
			}
			else
				G.Up_eve_rtn = 11;	/* DS stores 11 and then 5 when the card is missing */
			G.Up_eve_rtn = 5;
			break;

		case 5:
			if ((Moji.Mess_flg & 0x80) == 0)
			{
				Item = Search_item_id(0x35);
				if (Item < 0)
				{
					G.Up_eve_rtn = 11;
					Cprint(0, s_Pc_event_msg_80010c78, &Mend);
				}
				else if ((Moji.Mess_flg & 1) != 0)
				{
					Cprint(0, s_Pc_event_msg_80010c78, &Mend);
					Console_clr();
					G.Up_eve_rtn = 11;
				}
				else
				{
					Flg_on(G.SaveData.Common_flg, 0x46);
					Item = Search_item_id(0x35);
					G.SaveData.Item_work[Item].Id   = 0;
					G.SaveData.Item_work[Item].Num  = 0;
					G.SaveData.Item_work[Item].Size = 0;
					Sort_item();
					G.Up_eve_rtn = 6;
				}
			}
			break;

		case 6:
			pM2 = pc_mes_data + pc_mes_ofs[7];
			Cprint(1, (UCHAR *)s__Checking_up_ID_CARD_025__025__0_80010c7c, &Mend);
			if (Mend != 0)
				G.Up_eve_rtn = 7;
			break;

		case 7:
			Cut_old = G.SaveData.Cut_no;
			if (((G.Status_flg & STAT_SCENARIO_B) == 0) || (Flg_ck(G.SaveData.Key_flg, 0x11) == 0))
			{
				G.Next_cut_no   = 9;
				Main.Bg_chg_flg = 1;
				i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
				Snd_se_stad(0x2120000, (VECTOR *)0);
				i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
			}
			G.Next_cut_no   = 2;
			Main.Bg_chg_flg = 1;
			i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
			Snd_se_stad(0x2120000, (VECTOR *)0);
			i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
			if ((G.Status_flg & STAT_SCENARIO_B) != 0)
			{
				G.Next_cut_no   = 12;
				Main.Bg_chg_flg = 1;
				i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
				Snd_se_stad(0x2120000, (VECTOR *)0);
				i = 0;
				do { Task_sleep(1); i++; } while (i < 0x3c);
			}
			Main.Bg_chg_flg = 1;
			G.Next_cut_no   = (UCHAR)Cut_old;
			G.Up_eve_rtn    = 8;
			break;

		case 8:
			pM2 = pc_mes_data + pc_mes_ofs[8];
			Cprint(1, (UCHAR *)s__OK__Hall_side_doors_lock_releas_80010ca8, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 11;
			break;

		case 9:
			pM2 = pc_mes_data + pc_mes_ofs[9];
			Cprint(1, (UCHAR *)s_press_any_key___80010ccc, &Mend);
			Key_mode = 1;
			if ((Mend & Kend) != 0)
			{
				Console_clr();
				G.Up_eve_rtn = 11;
				Key_mode     = 0;
			}
			break;

		case 10:
			Flg_ret = Keyboard_Input(0, 2);
			if (Flg_ret != 0)
			{
				Console_clr();
				G.Up_key_rtn = 2;
				G.Up_eve_rtn = 11;
			}
			break;

		case C200_EXIT_1:
			Computer_exit(1);
			return;
		}

		if (pCom_work->Moji_code == 1)
		{
			pCom_work->Moji_code = 0;
			G.Up_eve_rtn         = 11;
		}
	}

	if (((Kend & Mend) != 0) || (Rtn_bak != G.Up_eve_rtn))
	{
		Mend = 0;
		Kend = 0;
		pM2  = (UCHAR *)0;
		pM1  = (UCHAR *)0;
	}
	Console_trans(1);
	Sprite_Trans(0x16c);
}

/* DS stack frame: 0x20 bytes; s0..s1 saved at sp+0x10..0x14, ra saved at sp+0x18. No local variable slots are
 * used. */
/* DS 0x8003b264 (Computer613) */
void Computer613(void)
{
	ULONG     Key_ret;
	UINT      Rtn_bak;
	static UCHAR  M1;			/* DS 0x800c3b60 (byte; only cleared, never read) */
	static UCHAR  M2;			/* DS 0x800c3b64 (byte; only cleared, never read) */
	static ULONG  Kend;			/* DS 0x800a3e88 (own copy; Computer200 uses the file-level Kend) */
	static ULONG  Mend;			/* DS 0x800a3e8c */
	static UCHAR  Key_mode;		/* DS 0x800a3e90 (never written in retail, so always 0) */

	if (Wframe_in_out() == 0)
	{
		if (Key_mode == 0)
		{
			if ((Mend != 0) && ((G.Key & 0x1000) != 0))
				Kend = 1;
		}
		else if ((Key_mode == 1) && (Mend != 0))
		{
			if ((G.Padd1_trg & 0xf0) != 0)
				Kend = 1;
			if ((G.Padd1_trg & 0xf) != 0)
				Kend = 1;
		}

		if (Kend != 0)
		{
			M2 = 0;
			M1 = 0;
		}

		Rtn_bak = G.Up_eve_rtn;
		switch (Rtn_bak)
		{
		case 0:
			pCom_work    = (COM_WORK *)G.pFree_work;
			G.pFree_work = G.pFree_work + sizeof(COM_WORK);
			if ((G.Status_flg & STAT_GUEST_REG) == 0)
				Computer_init((UINT)(G.Window_no >> 4), 1);
			else
			{
				Computer_init((UINT)(G.Window_no >> 4), 0);
				Cprint(0, (UCHAR *)s_Register_your_fingerprint___80010d0c, &Mend);
			}
			Mend = 0;
			Kend = 0;
			if ((G.Status_flg & STAT_GUEST_REG) != 0)
			{
				G.Up_eve_rtn = 8;
				break;
			}
			G.Up_eve_rtn = 1;
			/* fall through */

		case 1:
			Cprint(1, (UCHAR *)s_UMBRELLA__RODEM__SYSTEM__Ver_5_0_80010d28, &Mend);
			if (Mend != 0)
				G.Up_eve_rtn = 2;
			break;

		case 2:
			Cprint(1, (UCHAR *)s_Culture_Experiment_Room_Staff_Re_80010d60, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 3;
			break;

		case 3:
			Cprint(1, (UCHAR *)s_Accessing_020__020__020__020__02_80010d8c, &Mend);
			if (Mend != 0)
				G.Up_eve_rtn = 4;
			break;

		case C613_ENTER_USER_NAME:
			Cprint(1, (UCHAR *)s_Enter_your_user_name__80010db4, &Mend);
			if ((Mend & Kend) != 0)
			{
				G.Up_eve_rtn = 5;
				G.Up_key_rtn = 1;
			}
			break;

		case C613_KEYBOARD_NAME:
			Key_ret = Keyboard_Input(8, 0);
			if (Key_ret != 0)
			{
				G.Up_key_rtn = 2;
				if (strcmp((char *)pCom_work->In_buf, s_GUEST_80010dcc) == 0)
				{
					G.Up_eve_rtn = 7;
					Console_clr();
				}
				else
				{
					G.Up_eve_rtn = 6;
				}
			}
			break;

		case 6:
			Cprint(1, (UCHAR *)s__1This_name_is_not_in_the_resear_80010dd4, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 4;
			break;

		case 7:
			Cprint(1, (UCHAR *)s_Register_your_fingerprint__80010e08, &Mend);
			if ((Mend & Kend) != 0)
			{
				G.Status_flg |= STAT_GUEST_REG;
				Flg_on(G.SaveData.Common_flg, 0x5b);
				G.Up_eve_rtn = 12;
			}
			break;

		case 8:
			Cprint(1, (UCHAR *)s_Please_wait_025__025__025__025___80010e24, &Mend);
			if (Mend != 0)
				G.Up_eve_rtn = 9;
			break;

		case 9:
			Cprint(1, (UCHAR *)s_Registration_complete____80010e4c, &Mend);
			if ((Mend & Kend) != 0)
				G.Up_eve_rtn = 10;
			break;

		case 10:
			Cprint(1, (UCHAR *)s_Guest_registration_is_valid_for_2_80010e68, &Mend);
			if ((Mend & Kend) != 0)
			{
				G.Status_flg &= ~STAT_GUEST_REG;
				Computer_exit(1);
				return;
			}
			break;

		case C613_EXIT_1:
			Computer_exit(1);
			return;

		case C613_EXIT_0:
			Computer_exit(0);
			return;
		}

		if (pCom_work->Moji_code == 1)
		{
			pCom_work->Moji_code = 0;
			G.Up_eve_rtn         = 11;
		}
	}

	if (((Kend & Mend) != 0) || (Rtn_bak != G.Up_eve_rtn))
	{
		Mend = 0;
		Kend = 0;
		M2   = 0;
		M1   = 0;
	}
	Console_trans(1);
	Sprite_Trans(0x16c);
}

/* DS stack frame: 0x30 bytes; s0..s5 saved at sp+0x10..0x24, ra saved at sp+0x28. No local variable slots are
 * used. */
/* DS 0x8003b7e8 (Computer_init) */
void Computer_init(ULONG Cut_no, ULONG Open_flg)
{
	COM_WORK *pCom;
	ULONG     i;

	pCom       = pCom_work;
	Main.Vloop = 0;
	Console_clr();
	for (i = 0; i < 364; i++)
		Set_sp_work(&pCom->Sp_work[i], &Init_work_tbl[i]);

	pCom->Kcursor_disp = 64;
	pCom->Ky           = 0;
	pCom->Kx           = 0;
	pCom->Cursor_disp  = 0;
	pCom->nMoji        = 0;
	pCom->Kcursor_add  = 4;
	pCom->Key_timer    = 0;
	pCom->Cut_no_old   = G.Next_cut_no;
	G.Next_cut_no      = (UCHAR)Cut_no;
	Main.Bg_chg_flg    = 1;
	Task_sleep(1);

	if (Open_flg != 0)
	{
		Snd_se_stad(0x2060000, (VECTOR *)0);
		G.Up_wnd_rtn = 0;
		while (Wframe_move(0xc, 10, 0xfc, 0x8b) != 0)
		{
			Sprite_Trans(0x16c);
			Task_sleep(1);
		}
	}
}

/* DS stack frame: 0x18 bytes; s0 saved at sp+0x10, ra saved at sp+0x14. No local variable slots are used. */
/* DS 0x8003b918 (Computer_exit) */
void Computer_exit(ULONG Close_flg)
{
	COM_WORK *pCom;

	pCom = pCom_work;
	if (Close_flg != 0)
	{
		Snd_se_stad(0x2070000, (VECTOR *)0);
		G.Up_wnd_rtn = 1;
		while (Wframe_move(0xc, 10, 0xfc, 0x8b) != 0)
			Task_sleep(1);
	}
	G.Up_event_no   = 0;
	G.Up_eve_rtn    = 0;
	G.pFree_work    = G.pFree_work - sizeof(COM_WORK);
	G.Next_cut_no   = pCom->Cut_no_old;
	Main.Bg_chg_flg = 1;
	Main.Vloop      = 2;
	Task_signal(0);
	Task_exit();
}

/* DS 0x800a3dc8 (pc_mes_data) */
UCHAR pc_mes_data[160] = {
	0x00, 0x00, 0x00, 0x00, 0x09, 0xe3, 0xa7, 0xd1, 0xee, 0x0c, 0xae, 0xb1, 0x3b, 0xe5, 0xb3, 0x0a,
	0xfe, 0x00, 0x00, 0x09, 0xc4, 0x3b, 0xcf, 0x6f, 0xe3, 0xa7, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd1, 0xee, 0x0c, 0xae, 0xef, 0x5e, 0xf0, 0x45,
	0x0a, 0xfe, 0x00, 0x00, 0x09, 0xc4, 0x3b, 0xcf, 0x6f, 0xe3, 0xa7, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd1, 0xee, 0x0c, 0xae, 0xef, 0x5b,
	0x0a, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x09, 0xac, 0x3b, 0xe3, 0xad, 0x3b, 0x92, 0xe3, 0xa7, 0x6f,
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd1, 0xee, 0x0c, 0xae, 0x83, 0xef, 0x5e, 0xf0, 0x45, 0x92,
	0x5d, 0x75, 0x63, 0x0a, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x09, 0xac, 0x3b, 0xe3, 0xb7, 0xee, 0x07,
	0xee, 0x0c, 0xae, 0xef, 0x5b, 0x3c, 0x3c, 0x3c, 0x0a, 0xfe, 0x00, 0x00, 0x00, 0x09, 0xd1, 0xee,
	0x0c, 0xae, 0x85, 0xef, 0x5e, 0xf0, 0x45, 0x61, 0x80, 0x75, 0x62, 0x66, 0x0a, 0xfe, 0x00, 0x00
};

/* DS 0x800a3e68 (pc_mes_ofs) */
USHORT pc_mes_ofs[10] = {
	0x0000, 0x0012, 0x001c, 0x0033, 0x003d, 0x0053, 0x0062, 0x0076,
	0x008b, 0x0000
};
