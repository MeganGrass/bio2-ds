/*
 * ST_SUB00.C - Status screen sub-menus: items, files and map
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB00.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSPU.H>
#include "ST_SUB00.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "GAME.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "SND.H"
#include "STATUS.H"
#include "ST_SUB04.H"
#include "SUB.H"
#include "DOOR.H"
#include "ST_SUB02.H"
#include "FILECODE.H"
#include "flag.h"

UCHAR St_tmd_b[140];		/* TMD block B: 0x8c bytes, copied from the loaded file */

UCHAR St_tmd_a[1612];		/* TMD block A: 0x64c bytes, copied from the loaded file (see St_load_buf) */

SPRT St_sprt_b[2];		/* Map/item sprites: 2, one per buffer */

/* DS 0x800a9b8c */
static UCHAR Map_char_tbl[2][4] = {{0x00, 0x00, 0x60, 0x20}, {0x60, 0x00, 0x20, 0x30}};

/* DS 0x800a9ba0 (St_room_tbl) */
UCHAR St_room_tbl[] = {
	0x16, 0x0a, 0x1a, 0x07, 0x08, 0x00, 0x00, 0x00,
};

/* file-level data used only in this file */
static UCHAR shot;

/* DS 0x8006a574 (St_init00) */
VOID St_init00(VOID)
{
	register GLOBAL *pG;				// 35412 bytes
	register STATUS *pSt;				// 4208 bytes
	register FILE_DATA *pFd;
	ULONG flg_ret;

	pG  = &G;
	pSt = &St;
	pFd = &File_data[0x2e];

	pSt->Main_rtn = 1;
	pSt->Sub_rtn0 = 3;
	pSt->Mixed_id = 0;
	pSt->Mixed_pix_no = 0;
	if ((pG->Status_flg & STAT_EASY_MODE) != 0) {
		pFd->Page_max = 2;
		pFd->Back_h = 0;
		flg_ret = Flg_ck(pG->SaveData.Common_flg, 0xf);
		if (flg_ret != 0) {
			pFd->Page_max = 3;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Map_flg, 0x40);
		if (flg_ret != 0) {
			pFd->Page_max = 5;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Map_flg, 0x3c);
		if (flg_ret != 0) {
			pFd->Page_max = 6;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Map_flg, 0x4f);
		if (flg_ret != 0) {
			pFd->Page_max = 8;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Map_flg, 0x5a);
		if (flg_ret != 0) {
			pFd->Page_max = 9;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Map_flg, 0x69);
		if (flg_ret != 0) {
			pFd->Page_max = 11;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Item_flg2, 0x2b);
		if (flg_ret != 0) {
			pFd->Page_max = 12;
			pFd->Back_h = 0;
		}
		flg_ret = Flg_ck(pG->SaveData.Scenario_flg, 0x24);
		if (flg_ret != 0) {
			pFd->Page_max = 14;
			pFd->Back_h = 0;
		}
	}
	Fade_set(0x200, -0x1800, 7, 1);
	do {
		Task_sleep(1);
		St_disp_itemlist();
		St_disp_num();
		St_disp_equip();
		flg_ret = Check_pl_life();
		St_disp_ECG(flg_ret);
		St_disp_face();
		St_disp_message();
		St_disp_menu0();
		St_disp_name(0x10, 0xaf, 2, pG->SaveData.Item_work[pSt->Icursol_main].Id);
		St_disp_cursol0(pG->SaveData.Item_work[pSt->Icursol_main].Size);
		flg_ret = Fade_status(0);
	} while (flg_ret == 0);
}

/* DS 0x8006a774 (St_main00) */
VOID St_main00(VOID)
{
	static CODE_PTR Status_menu0_proc_tbl[6] = {St_fade_out_set, St_fade_out_wait, St_menu0_select, St_menu0_item, St_menu0_map, St_menu0_file};

	((CODE_PTR)((Status_menu0_proc_tbl)[St.Sub_rtn0]))();
	St_disp_itemlist();
	St_disp_num();
	St_disp_equip();
	St_disp_ECG(Check_pl_life());
	St_disp_face();
	St_disp_message();
	St_disp_menu0();
}

/* DS 0x8006a7f0 (St_menu0_select) */
VOID St_menu0_select(VOID)
{
	register STATUS *pSt;				// 4208 bytes
	register GLOBAL *pG;				// 35412 bytes
	UCHAR c_old;
	ULONG snd;

	pSt = &St;
	pG  = &G;

	c_old = pSt->Mcursol0;
	pSt->Exit_flg = 1;
	if ((int)pG->System_flg < 0) {
		if (((pG->Padd1 & 0x2000) != 0) && (pSt->Mcursol0 < 3)) {
			pSt->Mcursol0++;
		}
		if (((pG->Padd1 & 0x8000) != 0) && (pSt->Mcursol0 != 0)) {
			pSt->Mcursol0--;
		}
		if (c_old != pSt->Mcursol0) {
			Snd_se_stad(0x4040000, (VECTOR *)0);
		}
	}
	if ((pG->Key_trg & 0x1000) != 0) {
		if (pSt->Mcursol0 < 3) {
			pSt->Sub_rtn0 = 5 - pSt->Mcursol0;
			pSt->Sub_rtn1 = 0;
			pSt->Sub_rtn2 = 0;
			pSt->Sub_rtn3 = 0;
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn5 = 0;
			pSt->Sub_rtn6 = 0;
			pSt->Icursol_main = 0;
			snd = 0x4060000;
		} else if (pSt->Mcursol0 == 3) {
			pSt->Sub_rtn0 = 0;
			snd = 0x4050000;
		} else {
			snd = 0;
		}
		if (snd != 0) {
			Snd_se_stad(snd, (VECTOR *)0);
		}
	}
	if ((pG->Key_trg & 0x2000) != 0) {
		Snd_se_stad(0x4050000, (VECTOR *)0);
		pSt->Sub_rtn0 = 0;
	}
	if (((pG->Key_trg & 0x4000) != 0) && (pSt->Exit_flg == 1)) {
		Snd_se_stad(0x4050000, (VECTOR *)0);
		pSt->Sub_rtn0 = 0;
	}
}

/* DS 0x8006a994 (St_menu0_item) */
VOID St_menu0_item(VOID)
{
	tagGLOBAL *pG = &G;
	STATUS *pSt = &St;
	UCHAR c_old;
	UCHAR new_cur;
	UCHAR item_id;
	int done_flg;
	ULONG tmp_u;
	ULONG tmp_v;
	int do_key;

	c_old = pSt->Icursol_main;
	switch (pSt->Sub_rtn1) {
	case 0:
		pSt->Exit_flg = 1;
		do_key = 1;
		if ((int)pG->System_flg < 0) {
			if (((pG->Padd1 & 0x2000) != 0) && ((int)pSt->Icursol_main < (int)(pG->SaveData.Max_item_num - 1))) {
				if (pG->SaveData.Item_work[pSt->Icursol_main].Size == 1) {
					pSt->Icursol_main++;
				}
				pSt->Icursol_main++;
			}
			if (((pG->Padd1 & 0x8000) != 0) && (pSt->Icursol_main != 0) && (pSt->Icursol_main != 10)) {
				tmp_v = pSt->Icursol_main;
				new_cur = pSt->Icursol_main;
				if (pG->SaveData.Item_work[tmp_v].Size == 2) {
					new_cur = pSt->Icursol_main - 1;
					if (tmp_v == 1) {
						new_cur = pSt->Icursol_main + 1;
					}
				}
				pSt->Icursol_main = new_cur;
				pSt->Icursol_main--;
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				if ((int)pSt->Icursol_main < (int)(pG->SaveData.Max_item_num - 2)) {
					pSt->Icursol_main += 2;
				} else if (pSt->Icursol_main == 10) {
					pSt->Icursol_main = 1;
				}
			}
			if (((pG->Padd1 & 0x1000) != 0) && (pSt->Icursol_main == 10)) {
				pSt->Sub_rtn0 = 2;
				Snd_se_stad(0x4050000, (VECTOR *)0);
				do_key = 0;
			} else {
				if ((pG->Padd1 & 0x1000) != 0) {
					new_cur = 10;
					if (1 < pSt->Icursol_main) {
						new_cur = pSt->Icursol_main - 2;
					}
					pSt->Icursol_main = new_cur;
				}
				if (c_old != pSt->Icursol_main) {
					Snd_se_stad(0x4040000, (VECTOR *)0);
				}
			}
		}
		if (do_key) {
			if (((pG->Key_trg & 0x1000) == 0) || (pG->SaveData.Item_work[pSt->Icursol_main].Id == 0)) {
				if ((pG->Key_trg & 0x2000) == 0) {
					if (((pG->Key_trg & 0x4000) != 0) && (pSt->Exit_flg == 1)) {
						Snd_se_stad(0x4050000, (VECTOR *)0);
						pSt->Sub_rtn0 = 0;
					}
				} else {
					Snd_se_stad(0x4050000, (VECTOR *)0);
					pSt->Sub_rtn0 = 2;
				}
			} else {
				shot = 0;
				item_id = pG->SaveData.Item_work[pSt->Icursol_main].Id;
				if (item_id < 0x14) {
					tmp_u = 1;
					if (item_id == 4) {
						tmp_u = Flg_ck(pG->SaveData.Common_flg, 0x7e);
						St_init_disp_menu1((tmp_u != 0) << 1 | 1, 1);
						shot = 1;
					} else {
						St_init_disp_menu1(tmp_u, 0);
					}
				} else {
					St_init_disp_menu1(0, 0);
				}
				Snd_se_stad(0x4060000, (VECTOR *)0);
				pSt->Sub_rtn1 = 3;
				pSt->Sub_rtn2 = 0;
				pSt->Menu1_pos_x += 0x41;
			}
		}
		St_disp_name(0x10, 0xaf, 2, pG->SaveData.Item_work[pSt->Icursol_main].Id);
		goto St_lbl_afbc;
	case 1:
		pSt->Exit_flg = 1;
		if ((int)pG->System_flg < 0) {
			if (shot == 0) {
				if (((pG->Padd1 & 0x4000) != 0) && (pSt->Mcursol1 == 1)) {
					pSt->Mcursol1 = 3;
				} else if ((pG->Padd1 & 0x4000) != 0) {
					pSt->Mcursol1--;
				}
				if (((pG->Padd1 & 0x1000) != 0) && (pSt->Mcursol1 == 3)) {
					pSt->Mcursol1 = 1;
				} else if ((pG->Padd1 & 0x1000) != 0) {
					pSt->Mcursol1++;
				}
			} else {
				if (((pG->Padd1 & 0x4000) != 0) && (pSt->Mcursol1 == 0)) {
					pSt->Mcursol1 = 3;
				} else if ((pG->Padd1 & 0x4000) != 0) {
					pSt->Mcursol1--;
				}
				if (((pG->Padd1 & 0x1000) != 0) && (pSt->Mcursol1 == 3)) {
					pSt->Mcursol1 = 0;
				} else if ((pG->Padd1 & 0x1000) != 0) {
					pSt->Mcursol1++;
				}
			}
			if ((pG->Padd1 & 0x5000) != 0) {
				Snd_se_stad(0x4040000, (VECTOR *)0);
			}
		}
		if ((pG->Key_trg & 0x1000) != 0) {
			tmp_u = 0x4060000;
			pSt->Sub_rtn1 = 2;
			pSt->Sub_rtn3 = 0;
			pSt->Sub_rtn2 = pSt->Mcursol1;
			pSt->Icursol_sub = pSt->Icursol_main;
		} else if ((pG->Key_trg & 0x2000) != 0) {
			tmp_u = 0x4050000;
			pSt->Sub_rtn1 = 4;
			pSt->Sub_rtn2 = 0;
		} else {
			tmp_u = 0;
		}
		if (tmp_u != 0) {
			Snd_se_stad(tmp_u, (VECTOR *)0);
		}
		break;
	case 2:
		St_disp_menu1(shot);
		if (pSt->Sub_rtn2 == 1) {
			St_com_mix();
		} else if (pSt->Sub_rtn2 < 2) {
			if (pSt->Sub_rtn2 == 0) {
				tmp_u = Flg_ck(pG->SaveData.Common_flg, 0x7e);
				if (tmp_u == 0) {
					Flg_on(pG->SaveData.Common_flg, 0x7e);
					tmp_v = 2;
				} else {
					Flg_off(pG->SaveData.Common_flg, 0x7e);
					tmp_v = 0;
				}
				St_init_disp_menu1(tmp_v | 1, 1);
				pSt->Mcursol1 = 0;
				pSt->Sub_rtn1 = 1;
				pSt->Sub_rtn2 = 0;
			}
		} else if (pSt->Sub_rtn2 == 2) {
			St_com_inspect();
		} else if (pSt->Sub_rtn2 == 3) {
			St_com_use_equip();
		}
		goto St_lbl_afbc;
	case 3:
		done_flg = 3 < pSt->Sub_rtn2;
		pSt->Sub_rtn2++;
		if (done_flg) {
			pSt->Sub_rtn1 = 1;
			pSt->Sub_rtn2 = 0;
		}
		pSt->Menu1_pos_x -= 13;
		break;
	case 4:
		done_flg = 3 < pSt->Sub_rtn2;
		pSt->Sub_rtn2++;
		if (done_flg) {
			pSt->Sub_rtn1 = 0;
			pSt->Sub_rtn2 = 0;
		}
		pSt->Menu1_pos_x += 13;
		break;
	default:
		goto St_lbl_afbc;
	}
	St_disp_menu1(shot);
St_lbl_afbc:
	St_disp_cursol0(pG->SaveData.Item_work[pSt->Icursol_main].Size);
}

/* DS 0x8006affc (St_com_use_equip) */
VOID St_com_use_equip(VOID)
{
	UCHAR item_id;
	ULONG flg_res;
	SHORT heal_amt;
	ULONG frame_pad[2];			/* DS frame reserves 8 bytes here (0x20 frame); never read */
	tagGLOBAL *pG = &G;
	STATUS *pSt = &St;

	pSt->Exit_flg = 0;
	item_id = pSt->Icursol_main;
	if (pG->SaveData.Item_work[item_id].Id < 0x14) {
		if (pSt->Equip_item_no == item_id) {
			pSt->Sub_rtn1 = 4;
			pSt->Sub_rtn2 = 0;
			pSt->Equip_item_no = 0x80;
			pSt->Equip_item_id = 0;
			pSt->Exit_flg = 0;
			return;
		}
		pSt->Sub_rtn1 = 4;
		pSt->Sub_rtn2 = 0;
		pSt->Equip_item_no = pSt->Icursol_main;
		pSt->Equip_item_id = pG->SaveData.Item_work[item_id].Id;
		pSt->Exit_flg = 0;
		return;
	}
	switch (pSt->Sub_rtn3) {
	case 0:
		item_id = pG->SaveData.Item_work[pSt->Icursol_main].Id;
		if (item_id < 0x1e) {
			Mess_set(0xaf0010, 0xe400, 5, 0);
			pSt->Sub_rtn3 = 1;
			return;
		}
		if ((item_id < 0x23) || (item_id > 0x2e) || (item_id == 0x24) || (item_id == 0x25) || (item_id == 0x27)) {
			flg_res = Flg_ck(pG->Use_flg, pG->SaveData.Item_work[pSt->Icursol_main].Id);
			if (flg_res != 0) {
				pSt->Sub_rtn3 = 3;
				return;
			}
			Mess_set(0xaf0010, 0xe400, 7, 0);
			pSt->Sub_rtn3 = 1;
			return;
		}
		heal_amt = pG->Player.Max_life >> 2;
		switch (item_id) {
		case 0x23:
			pG->SaveData.Cure_cnt++;
			*(SHORT *)&pSt->Dmode[3][0].tag = pG->Player.Max_life;
			pSt->Recover_type = 1;
			break;
		case 0x26:
			*(SHORT *)&pSt->Dmode[3][0].tag = heal_amt;
			pSt->Recover_type = 1;
			break;
		case 0x28:
			pSt->Recover_type = 2;
			*(SHORT *)&pSt->Dmode[3][0].tag = 0;
			pG->Pl_poison_timer = 0;
			break;
		case 0x29:
			*(SHORT *)&pSt->Dmode[3][0].tag = pG->Player.Max_life >> 1;
			pSt->Recover_type = 1;
			break;
		case 0x2a:
		case 0x2c:
			*(SHORT *)&pSt->Dmode[3][0].tag = pG->Player.Max_life;
			pSt->Recover_type = 1;
			break;
		case 0x2b:
			pSt->Recover_type = 3;
			*(SHORT *)&pSt->Dmode[3][0].tag = heal_amt;
			pG->Pl_poison_timer = 0;
			break;
		case 0x2d:
			heal_amt = pG->Player.Max_life >> 1;
			pSt->Recover_type = 3;
			*(SHORT *)&pSt->Dmode[3][0].tag = heal_amt;
			pG->Pl_poison_timer = 0;
			break;
		case 0x2e:
			heal_amt = pG->Player.Max_life;
			pSt->Recover_type = 3;
			*(SHORT *)&pSt->Dmode[3][0].tag = heal_amt;
			pG->Pl_poison_timer = 0;
			break;
		}
		pSt->Sub_rtn3 = 4;
		break;
	case 1:
		if ((Moji.Mess_flg & 0x80) == 0) {
			pSt->Sub_rtn1 = 4;
			pSt->Sub_rtn2 = 0;
		}
		break;
	case 2:
		Set_iwork(pSt->Icursol_main, 0, 0, 0);
		Pix_clear((pSt->Icursol_main >> 1) + pSt->Icursol_main);
		pSt->Sub_rtn2 = 1;
		pSt->Sub_rtn3 = 2;
		pSt->Sub_rtn5 = 0;
		pSt->Sub_rtn4 = 0;
		break;
	case 3:
		pSt->Sub_rtn0 = 0;
		pG->SaveData.Use_id = (SHORT)pG->SaveData.Item_work[pSt->Icursol_main].Id;
		break;
	case 4:
		if (pSt->Recover_type == 0) {
			pSt->Sub_rtn3 = 2;
			pG->Player.Status_flg &= 0xefff;
			pG->Player.Life = pG->Player.Life + *(SHORT *)&pSt->Dmode[3][0].tag;
			if (pG->Player.Max_life <= pG->Player.Life) {
				pG->Player.Life = pG->Player.Max_life;
			}
		}
		break;
	}
}

/* DS 0x8006b358 (St_com_mix) */
VOID St_com_mix(VOID)
{
	tagGLOBAL *pG = &G;
	STATUS *pSt = &St;
	UCHAR id_main;
	UCHAR id_sub;
	UCHAR tmp_c;
	LONG empty_cnt;
	ULONG idx_sub;
	ULONG tmp_v;
	UCHAR mix_id;
	ULONG slot_idx;
	ULONG tmp_x;

	id_main = pG->SaveData.Item_work[pSt->Icursol_main].Id;
	id_sub = pG->SaveData.Item_work[pSt->Icursol_sub].Id;
	pSt->Exit_flg = 0;
	slot_idx = (ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Num + (ULONG)pG->SaveData.Item_work[pSt->Icursol_sub].Num;
	mix_id = 0;
	switch (pSt->Sub_rtn3) {
	case 0:
		slot_idx = pSt->Icursol_sub;
		if ((int)pG->System_flg < 0) {
			if (((pG->Padd1 & 0x2000) != 0) && ((int)slot_idx < (int)(pG->SaveData.Max_item_num - 1))) {
				if (pG->SaveData.Item_work[slot_idx].Size == 1) {
					pSt->Icursol_sub++;
				}
				pSt->Icursol_sub++;
			}
			if (((pG->Padd1 & 0x8000) != 0) && (pSt->Icursol_sub != 0) && (pSt->Icursol_sub != 10)) {
				idx_sub = pSt->Icursol_sub;
				tmp_c = pSt->Icursol_sub;
				if (pG->SaveData.Item_work[idx_sub].Size == 2) {
					tmp_c = pSt->Icursol_sub - 1;
					if (idx_sub == 1) {
						tmp_c = pSt->Icursol_sub + 1;
					}
				}
				pSt->Icursol_sub = tmp_c;
				pSt->Icursol_sub--;
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				if ((int)pSt->Icursol_sub < (int)(pG->SaveData.Max_item_num - 2)) {
					pSt->Icursol_sub += 2;
				} else if (pSt->Icursol_sub == 10) {
					pSt->Icursol_sub = 1;
				}
			}
			tmp_c = pSt->Icursol_sub;
			if (((pG->Padd1 & 0x1000) != 0) && (pSt->Icursol_sub != 10)) {
				tmp_c = 10;
				if (1 < pSt->Icursol_sub) {
					tmp_c = pSt->Icursol_sub - 2;
				}
			}
			pSt->Icursol_sub = tmp_c;
			if (slot_idx != pSt->Icursol_sub) {
				Snd_se_stad(0x4040000, (VECTOR *)0);
			}
		}
		if ((pG->Key_trg & 0x1000) == 0) {
			if ((pG->Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0);
				pSt->Sub_rtn1 = 1;
				pSt->Sub_rtn2 = 0;
				pSt->Sub_rtn3 = 0;
			}
		} else {
			tmp_v = Check_item_mix();
			pSt->Sub_rtn3 = (UCHAR)tmp_v;
			tmp_x = 0x4070000;
			if ((tmp_v & 0xff) != 0) {
				tmp_x = 0x4060000;
			}
			Snd_se_stad(tmp_x, (VECTOR *)0);
		}
		St_disp_cursol1(pG->SaveData.Item_work[pSt->Icursol_sub].Size);
		St_disp_name(0x10, 0xaf, 2, pG->SaveData.Item_work[pSt->Icursol_sub].Id);
		break;
	case 1:
		if (pSt->Sub_rtn4 > 9) {
			if (pSt->Sub_rtn5 != 0) {
				pSt->Icursol_main = pSt->Icursol_sub;
			}
			pSt->Sub_rtn3++;
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn5 = 0;
			pSt->Exit_flg = 0;
			pSt->Add_cp_x0 = 0;
			pSt->Add_cp_y0 = 0;
			pSt->X0 = 0;
			pSt->Y0 = 0;
			pSt->Add_cp_x1 = 0;
			pSt->Add_cp_y1 = 0;
			pSt->X1 = 0;
			pSt->Y1 = 0;
			return;
		}
		pSt->X0 += pSt->Add_cp_x0;
		pSt->Y0 += pSt->Add_cp_y0;
		pSt->X1 += pSt->Add_cp_x1;
		pSt->Y1 += pSt->Add_cp_y1;
		pSt->Sub_rtn4++;
		St_disp_cursol1(pG->SaveData.Item_work[pSt->Icursol_sub].Size);
		break;
	case 2:
		pSt->Sub_rtn4++;
		if (pSt->Sub_rtn4 < 4) {
St_lbl_b818:
			if (pSt->Sub_rtn5 == 0) {
				return;
			}
		} else {
			slot_idx = Search_item(0);
			if (slot_idx < 9) {
				empty_cnt = Search_item(1);
				tmp_v = slot_idx + 1;
				if ((int)slot_idx < (int)((ULONG)pG->SaveData.Max_item_num - empty_cnt)) {
					tmp_x = (int)tmp_v / 2 + 1 + slot_idx;
					Pix_move(tmp_x, (int)slot_idx / 2 + slot_idx);
					pG->SaveData.Item_work[slot_idx].Id = pG->SaveData.Item_work[slot_idx + 1].Id;
					pG->SaveData.Item_work[slot_idx].Num = pG->SaveData.Item_work[slot_idx + 1].Num;
					pG->SaveData.Item_work[slot_idx].Size = pG->SaveData.Item_work[slot_idx + 1].Size;
					pSt->Sub_rtn4 = 0;
					if (pSt->Equip_item_no == tmp_v) {
						pSt->Equip_item_no--;
					}
					Pix_clear(tmp_x);
					Set_iwork(tmp_v, 0, 0, 0);
					goto St_lbl_b818;
				}
			}
		}
		pSt->Sub_rtn1 = 4;
		pSt->Sub_rtn5 = 0;
		pSt->Sub_rtn4 = 0;
		pSt->Sub_rtn3 = 0;
		pSt->Sub_rtn2 = 0;
		break;
	case 3:
		if (pSt->Sub_rtn4 == 0) {
			pSt->Sub_rtn4++;
			Mess_set(0xaf0010, 0xe400, 2, 0);
		} else {
			pSt->Sub_rtn4++;
			if ((Moji.Mess_flg & 0x80) == 0) {
				pSt->Sub_rtn4 = 0;
				pSt->Sub_rtn3 = 0;
				pSt->Sub_rtn2 = 0;
				pSt->Sub_rtn1 = 0;
			}
		}
		break;
	case 4:
		Cd_read(MIX_PIX, (ULONG *)St_load_buf, 0, (UCHAR *)"MIX ITP");
		Pix_trans((pSt->Icursol_main >> 1) + pSt->Icursol_main, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		pG->SaveData.Item_work[pSt->Icursol_main].Id = pSt->Mixed_id;
		pG->SaveData.Item_work[pSt->Icursol_main].Num = Item_data_tbl[pSt->Mixed_id].Item_max;
		if (pSt->Icursol_main == pSt->Equip_item_no) {
			pSt->Equip_item_id = pSt->Mixed_id;
		}
		if (pSt->Icursol_sub == pSt->Equip_item_no) {
			pSt->Equip_item_no = pSt->Icursol_main;
			pSt->Equip_item_id = pSt->Mixed_id;
		}
		Set_iwork(pSt->Icursol_sub, 0, 0, 0);
		Pix_clear((pSt->Icursol_sub >> 1) + pSt->Icursol_sub);
		tmp_v = 0;
		goto St_lbl_bf40;
	case 5:
		if (((pG->SaveData.Item_work[pSt->Icursol_main].Num == Item_data_tbl[id_main].Item_max) &&
			(pG->SaveData.Item_work[pSt->Icursol_main].Id < 0x14)) ||
			((pG->SaveData.Item_work[pSt->Icursol_main].Id == 0x0f) && (pG->SaveData.Item_work[pSt->Icursol_main].Num == 0xff))) {
			goto St_lbl_bb60;
		}
		if (Item_data_tbl[id_main].Item_max < slot_idx) {
			Set_iwork_num(pSt->Icursol_main, Item_data_tbl[id_main].Item_max);
			pG->SaveData.Item_work[pSt->Icursol_sub].Num = (UCHAR)(slot_idx - Item_data_tbl[id_main].Item_max);
			tmp_v = 0;
		} else {
			Set_iwork_num(pSt->Icursol_main, slot_idx);
			Set_iwork(pSt->Icursol_sub, 0, 0, 0);
			Pix_clear((pSt->Icursol_sub >> 1) + pSt->Icursol_sub);
			tmp_v = 0;
		}
		goto St_lbl_bf40;
	case 6:
		if (((pG->SaveData.Item_work[pSt->Icursol_sub].Num != Item_data_tbl[id_sub].Item_max) ||
			(pG->SaveData.Item_work[pSt->Icursol_sub].Id > 0x13)) &&
			((pG->SaveData.Item_work[pSt->Icursol_sub].Id != 0x0f) || (pG->SaveData.Item_work[pSt->Icursol_sub].Num != 0xff))) {
			if (Item_data_tbl[id_sub].Item_max < slot_idx) {
				Set_iwork_num(pSt->Icursol_sub, Item_data_tbl[id_sub].Item_max);
				pG->SaveData.Item_work[pSt->Icursol_main].Num = (UCHAR)(slot_idx - Item_data_tbl[id_sub].Item_max);
			} else {
				Set_iwork_num(pSt->Icursol_sub, slot_idx);
				Set_iwork(pSt->Icursol_main, 0, 0, 0);
				Pix_clear((pSt->Icursol_main >> 1) + pSt->Icursol_main);
			}
			pSt->Sub_rtn3 = 1;
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn5 = 1;
			Check_cursol_distance(1);
			return;
		}
St_lbl_bb60:
		pSt->Sub_rtn3 = 3;
		pSt->Sub_rtn4 = 0;
		break;
	case 7:
		Cd_read(MIX_PIX, (ULONG *)St_load_buf, 0, (UCHAR *)"MIX ITP");
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_main].Id;
		pG->SaveData.Item_work[pSt->Icursol_main].Id = pG->SaveData.Item_work[pSt->Icursol_sub].Id + 0xf1;
		if (pSt->Equip_item_no == pSt->Icursol_main) {
			pSt->Equip_item_id = pG->SaveData.Item_work[pSt->Equip_item_no].Id;
		}
		pG->SaveData.Item_work[pSt->Icursol_sub].Id = tmp_c + 0x0f;
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_main].Num;
		pG->SaveData.Item_work[pSt->Icursol_main].Num = pG->SaveData.Item_work[pSt->Icursol_sub].Num;
		pG->SaveData.Item_work[pSt->Icursol_sub].Num = tmp_c;
		slot_idx = pSt->Icursol_sub;
		if (pG->SaveData.Item_work[slot_idx].Num == 0) {
			Set_iwork(slot_idx, 0, 0, 0);
			Pix_clear((pSt->Icursol_sub >> 1) + pSt->Icursol_sub);
		} else {
			Pix_trans((pSt->Icursol_sub >> 1) + slot_idx, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		}
		tmp_v = 0;
		goto St_lbl_bf40;
	case 8:
		Cd_read(MIX_PIX, (ULONG *)St_load_buf, 0, (UCHAR *)"MIX ITP");
		Pix_trans((pSt->Icursol_main >> 1) + pSt->Icursol_main, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_sub].Id;
		pG->SaveData.Item_work[pSt->Icursol_sub].Id = pG->SaveData.Item_work[pSt->Icursol_main].Id + 0xf1;
		if (pSt->Equip_item_no == pSt->Icursol_sub) {
			pSt->Equip_item_id = pG->SaveData.Item_work[pSt->Equip_item_no].Id;
		}
		pG->SaveData.Item_work[pSt->Icursol_main].Id = tmp_c + 0x0f;
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_main].Num;
		pG->SaveData.Item_work[pSt->Icursol_main].Num = pG->SaveData.Item_work[pSt->Icursol_sub].Num;
		pG->SaveData.Item_work[pSt->Icursol_sub].Num = tmp_c;
		slot_idx = pSt->Icursol_main;
		if (pG->SaveData.Item_work[slot_idx].Num == 0) {
			Set_iwork(slot_idx, 0, 0, 0);
			Pix_clear((pSt->Icursol_main >> 1) + pSt->Icursol_main);
		} else {
			Pix_trans((pSt->Icursol_main >> 1) + slot_idx, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		}
		tmp_v = 1;
St_lbl_bf40:
		pSt->Sub_rtn3 = 1;
		pSt->Sub_rtn5 = 0;
		pSt->Sub_rtn4 = 0;
		Check_cursol_distance(tmp_v);
		break;
	case 9:
		Cd_read(MIX_PIX, (ULONG *)St_load_buf, 0, (UCHAR *)"MIX ITP");
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_sub].Id;
		if (tmp_c == 0x24) {
			mix_id = 0x19;
		}
		if (tmp_c == 0x25) {
			mix_id = 0x1a;
		}
		slot_idx = pSt->Icursol_main;
		id_main = pG->SaveData.Item_work[slot_idx].Num;
		if (id_main < 7) {
			pG->SaveData.Item_work[slot_idx].Id = mix_id;
			Pix_trans((pSt->Icursol_main >> 1) + pSt->Icursol_main, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
			Set_iwork(pSt->Icursol_sub, 0, 0, 0);
			tmp_c = pSt->Icursol_sub;
			goto St_lbl_c15c;
		}
		pG->SaveData.Item_work[slot_idx].Num = id_main - 6;
		pG->SaveData.Item_work[pSt->Icursol_sub].Id = mix_id;
		pG->SaveData.Item_work[pSt->Icursol_sub].Num = 6;
		tmp_c = pSt->Icursol_sub;
St_lbl_c1b4:
		Pix_trans((tmp_c >> 1) + tmp_c, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		goto St_lbl_c1e0;
	case 10:
		Cd_read(MIX_PIX, (ULONG *)St_load_buf, 0, (UCHAR *)"MIX ITP");
		tmp_c = pG->SaveData.Item_work[pSt->Icursol_main].Id;
		if (tmp_c == 0x24) {
			mix_id = 0x19;
		}
		if (tmp_c == 0x25) {
			mix_id = 0x1a;
		}
		slot_idx = pSt->Icursol_sub;
		id_main = pG->SaveData.Item_work[slot_idx].Num;
		if (6 < id_main) {
			pG->SaveData.Item_work[slot_idx].Num = id_main - 6;
			pG->SaveData.Item_work[pSt->Icursol_main].Id = mix_id;
			pG->SaveData.Item_work[pSt->Icursol_main].Num = 6;
			tmp_c = pSt->Icursol_main;
			goto St_lbl_c1b4;
		}
		pG->SaveData.Item_work[slot_idx].Id = mix_id;
		Pix_trans((pSt->Icursol_sub >> 1) + pSt->Icursol_sub, (ULONG *)((UCHAR *)St_load_buf + pSt->Mixed_pix_no * 0x4b0));
		Set_iwork(pSt->Icursol_main, 0, 0, 0);
		tmp_c = pSt->Icursol_main;
St_lbl_c15c:
		Pix_clear((tmp_c >> 1) + tmp_c);
St_lbl_c1e0:
		Check_cursol_distance(0);
		pSt->Sub_rtn3 = 1;
		pSt->Sub_rtn5 = 0;
		pSt->Sub_rtn4 = 0;
		break;
	case 11:
		if (pSt->Sub_rtn4 == 0) {
			Mess_set(0xaf0010, 0xe400, 0xb, 0);
			pSt->Sub_rtn4++;
		} else if ((Moji.Mess_flg & 0x80) == 0) {
			if ((Moji.Mess_flg & 1) == 0) {
				pSt->Sub_rtn3 = 4;
			} else {
				pSt->Sub_rtn5 = 0;
				pSt->Sub_rtn4 = 0;
				pSt->Sub_rtn3 = 0;
			}
		}
		St_disp_cursol1(pG->SaveData.Item_work[pSt->Icursol_sub].Size);
		break;
	}
}

/* DS 0x8006c2b0 (St_com_inspect) */
VOID St_com_inspect(VOID)
{
	STATUS *pSt = &St;
	ULONG item_id;

	item_id = G.SaveData.Item_work[pSt->Icursol_main].Id;
	pSt->Exit_flg = 0;
	switch (pSt->Sub_rtn3) {
	case 0:
		CD.Fno = ITPS_ITP;
		CD.Fsize = 0x3000;
		CD.Fsector = (ULONG)floc[ITPS_ITP].sec_high * 0x10000 + (ULONG)floc[ITPS_ITP].sector + (ULONG)item_id * 6;
		CD.Fsum = Itp_check_sum[item_id];
		Cd_read(ITPS_ITP, (ULONG *)St_load_buf, 2, (UCHAR *)"ITP");
		G.Ctpage = 0x17;
		G.Cclut = 8;
		Mapping_tim((ULONG *)St_load_buf);
		St_init_disp_frame(0);
		St_init_disp_itp(0);
		pSt->Frame_pos_y = 0x102;
		pSt->Sub_rtn4 = 0;
		pSt->Sub_rtn3++;
		/* falls through */
	case 1:
		pSt->Message_pos_x -= 12;
		/* falls through */
	case 2:
		pSt->Sub_rtn4++;
		if (4 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Message_pos_x += 6;
		pSt->Message_pos_y += 8;
		pSt->Menu1_pos_x += 8;
		pSt->Sub_pos_x -= 6;
		break;
	case 3:
		pSt->Sub_rtn4++;
		if (6 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Frame_pos_y -= 15;
		break;
	case 4:
		pSt->Frame_pos_y -= 15;
		pSt->Message_pos_x -= 12;
		/* falls through */
	case 5:
		pSt->Sub_rtn4++;
		if (4 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Message_pos_y -= 8;
St_lbl_c5ec:
		pSt->Message_pos_x += 6;
		break;
	case 6:
		pSt->Sub_rtn4++;
		if (8 < pSt->Sub_rtn4) {
			Mess_set(0xaf0010, 0xe400, item_id + 0x10, 0);
			pSt->Sub_rtn3++;
		}
		break;
	case 7:
		if ((Moji.Mess_flg & 0x80) != 0) {
			break;
		}
		Snd_se_stad(0x4050000, (VECTOR *)0);
		Flg_on(G.SaveData.Item_c_flg, Item_data_tbl[item_id].Item_attribute);
		pSt->Sub_rtn3 = 8;
		pSt->Sub_rtn4 = 0;
		break;
	case 8:
		pSt->Message_pos_x -= 12;
		/* falls through */
	case 9:
		pSt->Sub_rtn4++;
		if (4 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Message_pos_y += 8;
		goto St_lbl_c5ec;
	case 10:
		pSt->Sub_rtn4++;
		if (6 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Frame_pos_y += 15;
		break;
	case 11:
		pSt->Frame_pos_y += 15;
		pSt->Message_pos_x -= 12;
		/* falls through */
	case 12:
		pSt->Sub_rtn4++;
		if (4 < pSt->Sub_rtn4) {
			pSt->Sub_rtn4 = 0;
			pSt->Sub_rtn3++;
		}
		pSt->Message_pos_x += 6;
		pSt->Sub_pos_x += 6;
		pSt->Message_pos_y -= 8;
		break;
	case 13:
		pSt->Sub_rtn3 = 0;
		pSt->Sub_rtn2 = 0;
		pSt->Sub_rtn1 = 0;
		pSt->Sub_rtn4 = 0;
		break;
	}
	St_disp_itp(0);
	St_disp_frame(0);
}

/*
 * Initialises the MODEL_WORK fields that DS St_menu0_file writes inline before the packet presets.
 * pTmd/pTmd2 are TMD object records, pSuper is the parent model pointer, cdir_y and poly_rgb are the
 * per-model colour/direction words. Position and matrix come from the identity matrix (GsIDMATRIX).
 */
static VOID St_init_model_work(MODEL_WORK *pMw, ULONG *pTmd, ULONG *pTmd2, ULONG *pSuper, SHORT cdir_y, ULONG poly_rgb)
{
	SHORT r;
	SHORT c;

	pMw->pTmd = pTmd;
	pMw->pTmd2 = pTmd2;
	for (r = 0; r < 3; r++) {
		for (c = 0; c < 3; c++) {
			pMw->M[r][c] = GsIDMATRIX.m[r][c];
		}
	}
	pMw->Pos_x = GsIDMATRIX.t[0];
	pMw->Pos_y = GsIDMATRIX.t[1];
	pMw->Pos_z = GsIDMATRIX.t[2];
	pMw->Old_x = 0;
	pMw->Old_y = 0;
	pMw->Old_z = 0;
	pMw->Cdir_x = 0;
	pMw->Cdir_y = cdir_y;
	pMw->Cdir_z = 0;
	pMw->Poly_rgb = poly_rgb;
	pMw->pSuper = pSuper;
}

/* DS 0x8006c6e4 (St_menu0_file) */
VOID St_menu0_file(VOID)
{
	tagGLOBAL *pG = &G;
	STATUS *pSt = &St;
	static UCHAR add[12] = {1, 2, 4, 6, 8, 10, 16, 32, 48, 64, 96, 112};
	register MODEL_WORK *pOm;
	MODEL_WORK *pFm;
	register ULONG *pw;
	register ULONG old_file;
	register ULONG i;
	register ULONG j;
	register ULONG k;

	old_file = pSt->F_sel_file;
	pSt->Exit_flg = 0;
	pw = (ULONG *)St_tmd_c;
	switch (pSt->Sub_rtn1) {
	case 0:
		Cd_read(ST_FILE_TS, (ULONG *)St_load_buf, 0, (UCHAR *)"FILE TIM TM2");
		pG->Ctpage = 0x17;
		pG->Cclut = 9;
		Mapping_tim((ULONG *)St_load_buf);
		Memcpy(St_tmd_a, (St_load_buf + 0x4e20), 0x64c);
		Mapping_tmd(1, (ULONG *)St_tmd_a, 0x17, 9);
		Memcpy(St_tmd_b, (St_load_buf + 0x546c), 0x8c);
		Mapping_tmd(1, (ULONG *)St_tmd_b, 0x17, 9);
		/*
		 * DS writes M[3][3] and Pos_x/y/z as one MATRIX (32-byte struct copy from GsIDMATRIX into M),
		 * so the parent link of each file page is &pOm->M seen as a MATRIX, not pOm itself.
		 */
		i = 30;
		j = 3;
		do {
			i -= 10;
			pOm = (MODEL_WORK *)(CD.Work_buf + (j - 1) * 0x488 + 0x910);
			pOm->pTmd2 = (ULONG *)&St_tmd_a[0x28];
			pOm->Cdir_y = 3072;
			pOm->pTmd = (ULONG *)&St_tmd_a[0x0c];
			pOm->Poly_rgb = 0x00808080;
			pOm->Pos_x = 0;
			pOm->Old_x = 0;
			pOm->Pos_y = 0;
			pOm->Old_y = 0;
			pOm->Pos_z = 0;
			pOm->Old_z = 0;
			pOm->Cdir_x = 0;
			pOm->Cdir_z = 0;
			pOm->pSuper = (ULONG *)&GsIDMATRIX;
			*(MATRIX *)pOm->M = GsIDMATRIX;
			pw = (ULONG *)PresetObjectGT3_neo(pOm, pw, 0);
			pw = (ULONG *)PresetObjectGT4_neo(pOm, pw, 0);
			Texture_Move(pOm, 0x17, 0, (SHORT)j + 0x1e8);
			k = 8;
			do {
				k--;
				pFm = &pSt->Fmodel[i + k];
				pFm->pTmd = (ULONG *)&St_tmd_b[0x0c];
				pFm->pTmd2 = (ULONG *)&St_tmd_b[0x28];
				pFm->Poly_rgb = 0x00808080;
				pFm->Pos_x = 0;
				pFm->Old_x = 0;
				pFm->Cdir_x = 0;
				pFm->Pos_y = 0;
				pFm->Old_y = 0;
				pFm->Cdir_y = 0;
				pFm->Pos_z = 0;
				pFm->Old_z = 0;
				pFm->Cdir_z = 0;
				pFm->pSuper = (ULONG *)pOm->M;
				*(MATRIX *)pFm->M = GsIDMATRIX;
				pw = (ULONG *)PresetObjectGT3_neo(pFm, pw, 0);
				pw = (ULONG *)PresetObjectGT4_neo(pFm, pw, 0);
			} while (k != 0);
			j--;
		} while (j != 0);
		St_init_disp_frame(1);
		St_init_disp_filesel();
		pSt->F_sel_book = 0;
		pSt->Frame_pos_y = 0x11e;
		pSt->F_move_x = 0x400;
		pSt->F_sel_file = 0;
		pSt->Goffset_add = 0;
		if (pG->SaveData.File_tag_bk[0] == 0xff) {
			move_file_tag(0);
		} else {
			pSt->F_sel_file = 0;
		}
		pSt->Sub_rtn1 = 1;
		pSt->Sub_rtn2 = 0;
	case 1:
		if (0xb < pSt->Sub_rtn2) {
			pSt->Sub_rtn1 = 3;
			pSt->Sub_rtn2 = 0;
		} else {
			pSt->Sub_rtn2++;
		}
		pSt->Message_pos_y = pSt->Message_pos_y + (pSt->Sub_rtn2 + 1) * 2;
		pSt->Frame_pos_y = pSt->Frame_pos_y + (pSt->Sub_rtn2 + 2) * -2;
		pSt->Sub_pos_x = pSt->Sub_pos_x + -6;
		goto RTN_8;
	case 2:
		if (0xb < pSt->Sub_rtn2) {
			pSt->Sub_rtn0 = 2;
			pSt->Sub_rtn1 = 0;
			pSt->Sub_rtn2 = 0;
		} else {
			pSt->Sub_rtn2++;
		}
		pSt->Message_pos_y = pSt->Message_pos_y + -0xe;
		pSt->Sub_pos_x += 6;
		pSt->Frame_pos_y += 0xd;
		goto RTN_8;
	case 3:
		if ((int)pG->System_flg < 0) {
			if ((pG->Padd1 & 0x8000) != 0) {
				pSt->Sub_rtn1 = 4;
				Snd_se_stad(0x40a0000, (VECTOR *)0x0);
			}
			if ((pG->Padd1 & 0x2000) != 0) {
				pSt->Sub_rtn1 = 5;
				Snd_se_stad(0x40a0000, (VECTOR *)0x0);
			}
		}
		if ((pG->Key_trg & 0x1000) != 0 &&
			pG->SaveData.File_tag_bk[pSt->F_sel_book * 8] != 0xff) {
			pSt->F_page_no = 0;
			pSt->Sub_rtn1 = 6;
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
		} else if ((pG->Key_trg & 0x2000) != 0) {
			pSt->Sub_rtn1 = 2;
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
		}
		goto RTN_8;
	case 4:
	case 5:
		if (pSt->Sub_rtn2 < 0xc) {
			if (pSt->Sub_rtn1 == 4) {
				pSt->F_move_x = pSt->F_move_x + (USHORT)(pSt->Sub_rtn2 + 1) * -0x10;
			} else {
				pSt->F_move_x = pSt->F_move_x + (USHORT)(pSt->Sub_rtn2 + 1) * 0x10;
			}
			pSt->Sub_rtn2++;
		} else {
			if (pSt->Sub_rtn1 == 4) {
				pSt->F_sel_book--;
				if ((int)((UINT)(UCHAR)pSt->F_sel_book << 0x18) < 0) {
					pSt->F_sel_book = 2;
				}
			} else {
				pSt->F_sel_book++;
				if (2 < pSt->F_sel_book) {
					pSt->F_sel_book = 0;
				}
			}
			if (pG->SaveData.File_tag_bk[pSt->F_sel_book * 8] == 0xff) {
				pSt->Sub_rtn2 = (UCHAR)(pSt->Sub_rtn2 + 1);
				move_file_tag(0);
			} else {
				pSt->F_sel_file = 0;
			}
			pSt->F_move_x = 0x400;
			pSt->F_move_y = 0;
			pSt->Sub_rtn1 = 3;
			pSt->Sub_rtn2 = 0;
			pSt->Sub_rtn3 = 0;
			if ((pG->Padd1 & 0x8000) != 0) {
				pSt->Sub_rtn1 = 4;
				Snd_se_stad(0x40a0000, (VECTOR *)0x0);
			}
			if ((pG->Padd1 & 0x2000) != 0) {
				pSt->Sub_rtn1 = 5;
				Snd_se_stad(0x40a0000, (VECTOR *)0x0);
			}
		}
RTN_8:
		St_disp_filesel();
		break;
	case 6:
		if (pSt->Sub_rtn2 < 0xb) {
			pSt->Goffset_add = pSt->Goffset_add + (USHORT)(UCHAR)(pSt->Sub_rtn2 + 1) * 4 + 0x14;
			pSt->Sub_rtn2++;
		} else {
			pSt->Sub_rtn1 = 8;
			pSt->Sub_rtn2 = 0;
		}
		break;
	case 7:
		if (pSt->Sub_rtn2 < 0xb) {
			pSt->Goffset_add = pSt->Goffset_add - ((USHORT)(UCHAR)(pSt->Sub_rtn2 + 1) * 4 + 0x14);
			pSt->Sub_rtn2++;
		} else {
			pSt->Sub_rtn1 = 3;
			pSt->Sub_rtn2 = 0;
		}
		break;
	case 8:
	case 9:
		if (10 < pSt->Sub_rtn2) {
			pSt->Sub_rtn1 = pSt->Sub_rtn1 == 8 ? 10 : 7;
			pSt->Sub_rtn2 = 0;
		} else {
			pSt->Sub_rtn2++;
		}
		break;
	case 10:
		if ((int)pG->System_flg < 0) {
			if ((pG->Padd1 & 0x1000) != 0) {
				move_file_tag(1);
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				move_file_tag(0);
			}
			if (old_file != pSt->F_sel_file) {
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
			}
		}
		if ((pG->Key_trg & 0x1000) != 0 &&
			pG->SaveData.File_tag_bk[pSt->F_sel_book * 8 + (int)pSt->F_sel_file] != 0xff) {
			pSt->F_page_no = 0;
			pSt->Sub_rtn1 = 11;
			St_init_disp_file();
			pSt->Fread_char = 0;
			Task_execute(2, file_moji_load);
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
		} else if ((pG->Key_trg & 0x2000) != 0) {
			pSt->Sub_rtn1 = 9;
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
		}
		St_disp_name(0x10, 0xcb, 2,
			pG->SaveData.File_tag_bk[pSt->F_sel_book * 8 + (int)pSt->F_sel_file] + 0x68);
		break;
	case 11:
		if (0xd < pSt->Sub_rtn2) {
			pSt->Sub_rtn2++;
			if (Task_status(2) == 0) {
				pG->Ctpage = 0x17;
				pG->Cclut = 9;
				Mapping_tim((ULONG *)St_load_buf);
				pG->Ctpage = 0x17;
				pG->Cclut = 10;
				Mapping_tim((ULONG *)St_load_buf2);
				pSt->Sub_rtn1 = 16;
				pSt->Sub_rtn2 = 0;
				pSt->File_pos_x = 0x138;
				Snd_se_stad(0x4080000, (VECTOR *)0x0);
				Bg_set_mode(2, 0);
			} else {
				pSt->Sub_rtn2 = 0xe;
			}
			goto St_lbl_frame;
		} else {
			pSt->Frame_pos_y += 0x16;
			pSt->Menu0_pos_y = pSt->Menu0_pos_y + -0x13;
			pSt->Status_pos_x = pSt->Status_pos_x + -0x16;
			pSt->Message_pos_y += 0x15;
			pSt->Itemlist_pos_x += 0x15;
			pSt->Sub_rtn2++;
		}
		break;
	case 12:
St_lbl_case12:
		if (pSt->Sub_rtn2 < 0xd) {
			pSt->Frame_pos_y = pSt->Frame_pos_y + -0x16;
			pSt->Menu0_pos_y += 0x13;
			pSt->Status_pos_x += 0x16;
			pSt->Message_pos_y = pSt->Message_pos_y + -0x15;
			pSt->Itemlist_pos_x = pSt->Itemlist_pos_x + -0x15;
			pSt->Sub_rtn2++;
		} else {
			pSt->Sub_rtn2++;
			Bg_set_mode(2, 0x200808);
			pSt->Sub_rtn1 = 10;
			pSt->Sub_rtn2 = 0;
			pSt->Frame_pos_y = pSt->Frame_pos_y + -0x16;
			pSt->Status_pos_x += 0x16;
			pSt->Menu0_pos_y += 0x13;
			pSt->Itemlist_pos_x = pSt->Itemlist_pos_x + -0x15;
			pSt->Message_pos_y = pSt->Message_pos_y + -0x15;
		}
		break;
	case 13:
		if (pSt->blink_sw0 == 0) {
			if (0x32 < pSt->blink_timer0) {
				pSt->blink_sw0 = 1;
			}
			pSt->blink_timer0 += 2;
		} else {
			if (pSt->blink_timer0 < 10) {
				pSt->blink_sw0 = 0;
			}
			pSt->blink_timer0 += 0xfe;
		}
		if ((pG->Key_trg & 0x2000) != 0) {
			Cd_read(ST_FILE_TS, (ULONG *)St_load_buf, 0, (UCHAR *)"FILE TIM TM2");
			pG->Ctpage = 0x17;
			pG->Cclut = 9;
			Mapping_tim((ULONG *)St_load_buf);
			pSt->Sub_rtn1 = 12;
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			goto St_lbl_case12;
		}
		if ((int)pG->System_flg < 0 && (pG->Padd1 & 0xa000) != 0) {
			int load_flg;
			int at_last;

			/* 0x2000 (next page): at the last page of the file the DS falls through to the back check */
			load_flg = 0;
			at_last = 0;
			if ((pG->Padd1 & 0x2000) != 0) {
				at_last = (*(USHORT *)&File_data[(UINT)pG->SaveData.File_tag_bk[pSt->F_sel_book * 8 + (int)pSt->F_sel_file] * 2] <= (USHORT)pSt->F_page_no);
			}
			if (((pG->Padd1 & 0x2000) != 0) && !at_last) {
				pSt->Sub_rtn1 = 15;
				pSt->F_page_no++;
				load_flg = 1;
			} else {
				if ((pG->Padd1 & 0x2000) != 0) {
					pSt->Sub_rtn1 = 14;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
				if ((pG->Padd1 & 0x8000) != 0 && pSt->F_page_no != 0) {
					pSt->Sub_rtn1 = 17;
					pSt->F_page_no += 0xff;
					load_flg = 1;
				}
			}
			if (load_flg) {
				Snd_se_stad(0x4080000, (VECTOR *)0x0);
				pSt->Fread_char = 1;
				Task_execute(2, file_moji_load);
			}
		}
		St_disp_file();
		goto St_lbl_frame;
	case 14:
		if ((pG->Key_trg & 0x3000) != 0) {
			Cd_read(ST_FILE_TS, (ULONG *)St_load_buf, 0, (UCHAR *)"FILE TIM TM2");
			pG->Ctpage = 0x17;
			pG->Cclut = 9;
			Mapping_tim((ULONG *)St_load_buf);
			pSt->Sub_rtn1 = 12;
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
			goto St_lbl_case12;
		}
		if ((int)pG->System_flg < 0 && (pG->Padd1 & 0x8000) != 0) {
			pSt->Sub_rtn1 = 13;
			pSt->blink_sw0 = 0;
			pSt->blink_timer0 = 2;
		}
		St_disp_file();
		goto St_lbl_frame;
	case 15:
	case 16:
		if (pSt->Sub_rtn2 < 0xb) {
			UCHAR step = add[pSt->Sub_rtn2 + 1];
			SHORT delta = pSt->Sub_rtn1 == 15 ? -(SHORT)step : -(SHORT)add[0xb - (UINT)(pSt->Sub_rtn2 + 1)];
			pSt->Sub_rtn2++;
			pSt->File_pos_x += delta;
		} else {
			if (pSt->Sub_rtn1 == 15) {
				pSt->Sub_rtn2++;
				if (Task_status(2) != 0) {
					pSt->Sub_rtn2 = 0xe;
				} else {
					pG->Ctpage = 0x17;
					pG->Cclut = 10;
					Mapping_tim((ULONG *)St_load_buf);
					pSt->File_pos_x = 0x138;
					pSt->Sub_rtn2 = 0;
					pSt->Sub_rtn1 = 16;
					pSt->blink_sw0 = 0;
					pSt->blink_timer0 = 2;
				}
			} else {
				pSt->Sub_rtn2 = 0;
				pSt->Sub_rtn1 = 13;
				pSt->blink_sw0 = 0;
				pSt->blink_timer0 = 2;
			}
		}
		St_disp_file();
		goto St_lbl_frame;
	case 17:
	case 18:
		if (pSt->Sub_rtn2 < 0xb) {
			UCHAR step = add[pSt->Sub_rtn2 + 1];
			SHORT delta = pSt->Sub_rtn1 == 17 ? (SHORT)step : add[0xb - (UINT)(pSt->Sub_rtn2 + 1)];
			pSt->Sub_rtn2++;
			pSt->File_pos_x += delta;
		} else {
			if (pSt->Sub_rtn1 == 17) {
				pSt->Sub_rtn2++;
				if (Task_status(2) != 0) {
					pSt->Sub_rtn2 = 0xe;
				} else {
					pG->Ctpage = 0x17;
					pG->Cclut = 10;
					Mapping_tim((ULONG *)St_load_buf);
					pSt->File_pos_x = -0x106;
					pSt->Sub_rtn2 = 0;
					pSt->Sub_rtn1 = 18;
					pSt->blink_sw0 = 0;
					pSt->blink_timer0 = 2;
				}
			} else {
				pSt->Sub_rtn2 = 0;
				pSt->Sub_rtn1 = 13;
				pSt->blink_sw0 = 0;
				pSt->blink_timer0 = 2;
			}
		}
		St_disp_file();
		goto St_lbl_frame;
	default:
		goto St_lbl_frame;
	}
	ST_Om_trans();
St_lbl_frame:
	St_disp_frame(1);
}

/* DS 0x8006d444 (file_moji_load) */
VOID file_moji_load(VOID)
{
	static UCHAR File_offset[24] = {0x00, 0x07, 0x0e, 0x11, 0x15, 0x20, 0x2b, 0x36, 0x3e, 0x44, 0x4c, 0x4f, 0x53, 0x58, 0x5c, 0x61, 0x6b, 0x73, 0x7e, 0x85, 0x88, 0x90, 0x97, 0x9d};
	register ULONG tmp;

	tmp = (UINT)File_offset[G.SaveData.File_tag_bk[St.F_sel_book * 8 + (int)St.F_sel_file]];
	if (St.Fread_char != 0) {
		tmp = tmp + 1 + (UINT)St.F_page_no;
	}
	CD.Fno = FILES_TIM;
	CD.Fsize = fits_floc[tmp].size;
	CD.Fsum = fits_floc[tmp].sum;
	CD.Fsector = (UINT)floc[FILES_TIM].sec_high * 0x10000 + (UINT)floc[FILES_TIM].sector +
				 (UINT)fits_floc[tmp].sec_high * 0x10000 + (UINT)fits_floc[tmp].sector;
	Cd_read(FILES_TIM, (ULONG *)St_load_buf, 3, (UCHAR *)"FILE ETC");
	Task_exit();
}

/* DS 0x8006d550 (move_file_tag) */
VOID move_file_tag(register ULONG type)
{
	ULONG i;
	ULONG cnt;
	ULONG base;

	cnt = 0;
	base = St.F_sel_book * 8;
	for (i = 7; ; i--) {
		if (G.SaveData.File_tag_bk[base + i] != 0xff) {
			cnt++;
		}
		if (i == 0) {
			break;
		}
	}
	if (cnt != 0) {
		if (type == 0) {
			do {
				St.F_sel_file++;
				if (St.F_sel_file > 7) {
					St.F_sel_file = 0;
				}
			} while (G.SaveData.File_tag_bk[base + St.F_sel_file] == 0xff);
		} else {
			do {
				St.F_sel_file--;
				if ((INT)((UINT)(UCHAR)St.F_sel_file << 24) < 0) {
					St.F_sel_file = 7;
				}
			} while (G.SaveData.File_tag_bk[base + St.F_sel_file] == 0xff);
		}
	}
}

/* DS 0x8006d650 (St_menu0_map) */
VOID St_menu0_map(VOID)
{
	UCHAR old_map;
	USHORT file_no;
	ULONG chk;
	int done_flg;
	RECT rect;

	old_map = St.Map_no;
	St.Exit_flg = 0;
	switch (St.Sub_rtn1) {
	case 0:
		chk = Check_map_no(G.SaveData.Stage_no, G.SaveData.Room_no);
		St.Map_no = (UCHAR)chk;
		file_no = Map_info[chk & 0xff].File_no;
		CD.Fno = MAPS_PIX;
		CD.Fsize = maps_floc[file_no].size;
		CD.Fsum = maps_floc[file_no].sum;
		CD.Fsector = (ULONG)floc[MAPS_PIX].sec_high * 0x10000 + (ULONG)floc[MAPS_PIX].sector +
					 (ULONG)maps_floc[file_no].sec_high * 0x10000 + (ULONG)maps_floc[file_no].sector;
		Cd_read(MAPS_PIX, (ULONG *)St_load_buf, 2, (UCHAR *)"MAP FILE");
		rect.x = 0x1c0;
		rect.y = 0x100;
		rect.w = 0x40;
		rect.h = 0x100;
		LoadImage(&rect, (ULONG *)St_load_buf);
		DrawSync(0);
		St.Sub_rtn1 = 1;
		St.Sub_rtn2 = 0;
		St.Map_offset_x = 0;
		St.Map_offset_y = 0;
		St_init_disp_map();
		break;
	case 1:
		done_flg = 0xc < St.Sub_rtn2;
		St.Sub_rtn2++;
		if (done_flg) {
			St.Sub_rtn1 = 3;
			St.Sub_rtn2 = 0;
			Snd_se_stad(0x4090000, (VECTOR *)0);
		}
		St.Status_pos_x -= 0x16;
		St.Menu0_pos_y -= 0x13;
		St.Itemlist_pos_x += 0x15;
		St.Message_pos_y += 0x15;
		break;
	case 2:
		done_flg = 0xc < St.Sub_rtn2;
		St.Sub_rtn2++;
		if (done_flg) {
			St.Sub_rtn0 = 2;
			St.Sub_rtn1 = 0;
			St.Sub_rtn2 = 0;
		}
		St.Status_pos_x += 0x16;
		St.Menu0_pos_y += 0x13;
		St.Itemlist_pos_x -= 0x15;
		St.Message_pos_y -= 0x15;
		break;
	case 3:
		if (St.blink_sw0 == 0) {
			if (0x50 < St.blink_timer0) {
				St.blink_sw0 = 1;
			}
			St.blink_timer0 += 2;
		} else {
			if (St.blink_timer0 < 10) {
				St.blink_sw0 = 0;
			}
			St.blink_timer0 -= 2;
		}
		if (((int)G.System_flg < 0) && (1 < St.Map_no)) {
			if (((G.Padd1 & 0x1000) != 0) && (1 < map_up_floor[St.Map_no]) &&
				(Flg_ck(&G.SaveData.Map_o_flg, map_up_floor[St.Map_no]) != 0)) {
				St.Map_no = map_up_floor[St.Map_no];
			}
			if (((G.Padd1 & 0x4000) != 0) && (1 < map_down_floor[St.Map_no]) &&
				(Flg_ck(&G.SaveData.Map_o_flg, map_down_floor[St.Map_no]) != 0)) {
				St.Map_no = map_down_floor[St.Map_no];
			}
			if (old_map != St.Map_no) {
				St.Sub_rtn1 = 4;
				Snd_se_stad(0x4040000, (VECTOR *)0);
			}
		}
		if ((G.Key_trg & 0x2000) != 0) {
			St.Sub_rtn1 = 2;
			Snd_se_stad(0x4050000, (VECTOR *)0);
		}
		if (St.Sub_rtn1 == 3) {
			St_disp_map();
		}
		break;
	case 4:
		file_no = Map_info[St.Map_no].File_no;
		CD.Fno = MAPS_PIX;
		CD.Fsize = maps_floc[file_no].size;
		CD.Fsum = maps_floc[file_no].sum;
		CD.Fsector = (ULONG)floc[MAPS_PIX].sec_high * 0x10000 + (ULONG)floc[MAPS_PIX].sector +
					 (ULONG)maps_floc[file_no].sec_high * 0x10000 + (ULONG)maps_floc[file_no].sector;
		Cd_read(MAPS_PIX, (ULONG *)St_load_buf, 2, (UCHAR *)"MAP FILE");
		rect.x = 0x1c0;
		rect.y = 0x100;
		rect.w = 0x40;
		rect.h = 0x100;
		LoadImage(&rect, (ULONG *)St_load_buf);
		DrawSync(0);
		St_init_disp_map();
		St.blink_timer0 = 2;
		St.blink_sw0 = 0;
		St.Sub_rtn1 = 3;
		break;
	}
	St_disp_wall();
}

/* DS 0x8006db44 (St_init_map_floor) */
VOID St_init_map_floor(VOID)
{
	SPRT *sp;
	UCHAR *pMapTbl;
	int tbl_ofs;
	int col_cnt;
	UCHAR loop_flg;
	UCHAR map_bits;
	CHAR tex_u;

	sp = (SPRT *)St_sprt_a;
	pMapTbl = (UCHAR *)St_map_tbl_start;
	tbl_ofs = 0x70;
	do {
		col_cnt = 2;
		do {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			map_bits = *pMapTbl;
			sp->v0 = 0xf8;
			if (map_down_floor[tbl_ofs + 0x13] == 0) {
				tex_u = (CHAR)((map_bits & 3) * 8 + 0x20);
			} else {
				tex_u = (CHAR)((map_bits & 3) << 3);
			}
			sp->u0 = (UCHAR)tex_u;
			sp->x0 = *(SHORT *)(map_down_floor + tbl_ofs + 0x14);
			sp->y0 = *(SHORT *)(map_down_floor + tbl_ofs + 0x16);
			sp->w = 8;
			sp->h = 8;
			sp->clut = GetClut(0x100, 0x1fc);
			sp++;
			col_cnt--;
		} while (col_cnt != 0);
		loop_flg = (pMapTbl != (UCHAR *)St_map_tbl_end);
		pMapTbl -= 8;
		tbl_ofs -= 8;
	} while (loop_flg);
	SetDrawMode((DR_MODE *)&St.Dmode[0x16][0].code[1], 0, 0, 0x17, (RECT *)0);
	SetDrawMode((DR_MODE *)&St.Dmode[0x16][1].code[1], 0, 0, 0x17, (RECT *)0);
}

/* DS 0x8006dcc0 (St_disp_map_floor) */
VOID St_disp_map_floor(VOID)
{
	UCHAR *p;
	UCHAR *pCol;
	ULONG *ot;
	int rem_cnt;
	int tbl_ofs;
	ULONG flg_res;
	UCHAR bit_val;

	ot = G.Ot0[G.Cbuf_id] + 4;
	p = (UCHAR *)St_sprt_a;
	if (G.Cbuf_id != 0) {
		p = (UCHAR *)((SPRT *)St_sprt_a + 1);
	}
	pCol = p + 6;
	tbl_ofs = 0x70;
	rem_cnt = 0xe;
	do {
		rem_cnt--;
		if (St.blink_sw0 == 0) {
			pCol[-2] = 0x50;
			pCol[-1] = 0x50;
			pCol[0] = 0x50;
		} else {
			pCol[-2] = 0x80;
			pCol[-1] = 0x80;
			pCol[0] = 0x80;
		}
		flg_res = Flg_ck(&G.SaveData.Map_i_flg, St.Map_no);
		if ((flg_res != 0) && (map_down_floor[tbl_ofs + 0x11] == St.Map_no) &&
			(Flg_ck(G.SaveData.Key_flg, map_down_floor[tbl_ofs + 0x10]) == 0)) {
			if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
				bit_val = map_down_floor[tbl_ofs + 0x12] & 0x40;
			} else {
				if (((int)G.Status_flg < 0) && ((map_down_floor[tbl_ofs + 0x12] & 0x40) != 0)) {
					goto next_row;
				}
				bit_val = map_down_floor[tbl_ofs + 0x12] & 0x80;
			}
			if (bit_val == 0) {
				AddPrim(ot, (ULONG *)p);
			}
		}
next_row:
		pCol += 0x28;
		p += 0x28;
		tbl_ofs -= 8;
		if (rem_cnt == 0) {
			AddPrim(ot, (ULONG *)&St.Dmode[0x16][G.Cbuf_id].code[1]);
			return;
		}
	} while (1);
}

/* DS 0x8006dea0 (St_init_disp_map) */
VOID St_init_disp_map(VOID)
{
	SPRT *sp;
	ULONG i;
	ULONG j;
	ULONG no;
	UCHAR cval;
	ULONG idx;
	int more;
	MAP_DATA *pRoom;
	ULONG room_cnt;

	sp = St_sprt_b;
	for (i = 0; i < 2; i++) {
		setSprt(sp);
		setSemiTrans(sp, 1);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->u0 = 0;
		sp->v0 = 0;
		sp->w = 0x0c;
		sp->h = 0x0c;
		sp->clut = GetClut(0x100, 0x1ec);
		sp++;
	}
	cval = 0x0e;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = cval;
			sp->v0 = 0x0c;
			sp->w = 0x0e;
			sp->h = 0x0c;
			sp->clut = GetClut(0x100, 0x1ec);
			sp++;
		}
		cval -= 0x0e;
	}
	idx = 4;
	do {
		more = (idx != 0);
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = ((UCHAR *)Map_char_tbl)[idx];
			sp->v0 = ((UCHAR *)Map_char_tbl)[idx + 1];
			sp->w = ((UCHAR *)Map_char_tbl)[idx + 2];
			sp->h = ((UCHAR *)Map_char_tbl)[idx + 3];
			sp->clut = GetClut(0x100, 0x1f1);
			sp++;
		}
		idx -= 4;
	} while (more);
	no = (UINT)St.Map_no;
	room_cnt = Map_info[no].Room_num;
	pRoom = Map_info[no].pMap_data + room_cnt;
	do {
		room_cnt--;
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = pRoom[-1].U;
			sp->v0 = pRoom[-1].V;
			sp->w = (USHORT)pRoom[-1].W;
			sp->h = (USHORT)pRoom[-1].H;
			sp->clut = GetClut(0x100, 0x1f2);
			sp++;
		}
		pRoom--;
	} while (room_cnt != 0);
	St_init_map_floor();
}

/* DS 0x800d6c08 - DR_MODE[4]: end-of-list primitives, 0xc bytes each. Entries 0,1 are the per-buffer tags; entries 2,3 are St_end_tag_b (DS 0x800d6c20) */
DR_MODE Map_end_tag[4] = {
	{ 0x00000000, { 0x00000000, 0x00000000 } },	/* +0x00 */
	{ 0x00000000, { 0x00000000, 0x00000000 } },	/* +0x0c */
	{ 0x00000000, { 0x00000000, 0x00000000 } },	/* +0x18 */
	{ 0x00000000, { 0x00000000, 0x00000000 } },	/* +0x24 */
};

/* DS 0x8006e120 (St_disp_map) */
VOID St_disp_map(VOID)
{
	UCHAR *pSpr;
	UCHAR *p;
	ULONG *ot;
	ULONG now_map;
	ULONG now_room;
	ULONG room_idx;
	ULONG flg_res;
	ULONG clut_idx;
	UCHAR tmp_b;
	UCHAR tex_u;
	UCHAR tex_v;
	USHORT clut_val;
	SHORT y_pos;
	SHORT y_pos2;
	LONG lp_cnt;
	SHORT *pPos;
	USHORT *pClut;
	MAP_DATA *pMap;
	MAP_DATA *pRoom;
	MAP_DATA *pPrev;

	pSpr = (UCHAR *)St_sprt_b;
	ot = G.Ot0[G.Cbuf_id] + 5;
	now_map = Check_map_no(G.SaveData.Stage_no, G.SaveData.Room_no);
	if (G.Cbuf_id != 0) {
		pSpr = (UCHAR *)(St_sprt_b + 1);
	}
	now_room = Check_room_no(St.Map_no, G.SaveData.Room_no);
	pMap = Map_info[St.Map_no].pMap_data;
	clut_idx = 0x1f5;
	if (St.Map_no == now_map) {
		St.Map_pos_x = (SHORT)((G.Player.Pos_x + 28000) / 0x1c2) + St.Map_offset_x;
		St.Map_pos_y = St.Map_offset_y - (SHORT)((G.Player.Pos_z + 28000) / 0x1c2);
		((SPRT *)pSpr)->u0 = ((UCHAR)((G.Player.Cdir_y + 0x100) >> 9) & 7) * 0x0c;
		((SPRT *)pSpr)->v0 = 0;
		((SPRT *)pSpr)->r0 = (UCHAR)(St.blink_timer0 + 0x28);
		((SPRT *)pSpr)->g0 = (UCHAR)(St.blink_timer0 + 0x28);
		((SPRT *)pSpr)->b0 = (UCHAR)(St.blink_timer0 + 0x28);
		((SPRT *)pSpr)->x0 = St.Map_pos_x + pMap[now_room].B_pos_x;
		((SPRT *)pSpr)->y0 = St.Map_pos_y + pMap[now_room].B_pos_y;
		AddPrim(ot, (ULONG *)pSpr);
	}
	p = pSpr + 0x28;
	if (St.Map_no < 2) {
		p = pSpr + 0x78;
	} else {
		lp_cnt = 2;
		y_pos = 0x198;
		do {
			y_pos -= 0xc2;
			((LINE_F2 *)p)->x0 = 0x98;
			((LINE_F2 *)p)->y0 = y_pos;
			lp_cnt--;
			if (St.blink_sw0 == 0) {
				((LINE_F2 *)p)->r0 = 0x80;
				((LINE_F2 *)p)->g0 = 0x80;
				((LINE_F2 *)p)->b0 = 0x80;
			} else {
				((LINE_F2 *)p)->r0 = 0x40;
				((LINE_F2 *)p)->g0 = 0x40;
				((LINE_F2 *)p)->b0 = 0x40;
			}
			if (lp_cnt == 0) {
				tmp_b = map_up_floor[St.Map_no];
			} else {
				tmp_b = map_down_floor[St.Map_no];
			}
			if ((1 < tmp_b) && (Flg_ck(&G.SaveData.Map_o_flg, tmp_b) != 0)) {
				AddPrim(ot, (ULONG *)p);
			}
			p += 0x28;
		} while (lp_cnt != 0);
	}
	lp_cnt = 2;
	pPos = &((SPRT *)p)->y0;
	y_pos2 = 0x1a;
	y_pos = 0x1ea;
	AddPrim(ot, (ULONG *)&Map_end_tag[2 + G.Cbuf_id]);
	do {
		pSpr = p;
		y_pos2 -= 5;
		y_pos -= 0xf0;
		lp_cnt--;
		pPos[-1] = y_pos;
		pPos[0] = y_pos2;
		AddPrim(ot, (ULONG *)pSpr);
		pPos += 0x14;
		p = pSpr + 0x28;
	} while (lp_cnt != 0);
	pClut = &((SPRT *)pSpr + 2)->clut;
	room_idx = Map_info[St.Map_no].Room_num;
	pRoom = pMap + room_idx;
	do {
		pPrev = pRoom - 1;
		pClut[-3] = pRoom[-1].C_pos_x + St.Map_offset_x;
		pClut[-2] = pRoom[-1].C_pos_y + St.Map_offset_y;
		room_idx--;
		if ((St.Map_no == 2) && (room_idx == 0xe)) {
			if (G.SaveData.Room_no == 0xe) {
				flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 8);
				if (flg_res != 0) {
					flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					if (flg_res == 0) {
						clut_idx = 0x1fe;
						tex_u = pMap[0xe].U;
						tex_v = 0x98;
					} else {
						clut_idx = 0x1fa;
						tex_u = pMap[0xe].U;
						tex_v = 0x50;
					}
					((UCHAR *)pClut)[-2] = tex_u;
					((UCHAR *)pClut)[-1] = tex_v;
				}
				flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 9);
				if (flg_res != 0) {
					flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 0xb);
					if (flg_res == 0) {
						clut_idx = 0x1fe;
						tex_u = pPrev->U;
						tex_v = 0x50;
					} else {
						clut_idx = 0x1fa;
						tex_u = pPrev->U;
						tex_v = 0x98;
					}
					((UCHAR *)pClut)[-2] = tex_u;
					((UCHAR *)pClut)[-1] = tex_v;
				}
			} else {
				flg_res = Flg_ck(G.SaveData.Map_flg, 0x2c);
				if ((flg_res == 0) && (Flg_ck(&G.SaveData.Map_i_flg, Map_info[St.Map_no].Map) == 0)) {
					goto skip_room;
				}
				flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 0xb);
				if (flg_res == 0) {
					clut_idx = 0x1f2;
					tex_u = pMap[0xe].U;
					((UCHAR *)pClut)[-2] = tex_u;
					((UCHAR *)pClut)[-1] = 0x50;
					flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					if (flg_res != 0) {
						clut_idx = 0x1fd;
					}
				} else {
					clut_idx = 0x1fd;
					tex_u = pMap[0xe].U;
					((UCHAR *)pClut)[-2] = tex_u;
					((UCHAR *)pClut)[-1] = 0x98;
					flg_res = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					if (flg_res != 0) {
						clut_idx = 0x1f5;
					}
				}
			}
			goto write_clut;
		}
		clut_idx = 0x1f5;
		if (Flg_ck(G.SaveData.Map_c_flg + 1, pRoom[-1].Change_flag) != 0) {
			clut_idx = 0x1fa;
		}
		if ((St.Map_no == now_map) && (room_idx == now_room)) {
			clut_idx++;
		}
		if (Flg_ck(&G.SaveData.Map_i_flg, Map_info[St.Map_no].Map) != 0) {
			if (Flg_ck(G.SaveData.Map_flg, pRoom[-1].Map_flg) == 0) {
				if ((G.Player.Id & 1) == 0) {
					tmp_b = pRoom[-1].Pl0_flg;
				} else {
					tmp_b = pRoom[-1].Pl1_flg;
				}
				if ((((tmp_b & 1) != 0) && ((G.Status_flg & STAT_SCENARIO_B) == 0)) ||
					(((tmp_b & 2) != 0) && ((G.Status_flg & STAT_SCENARIO_B) != 0))) {
					goto skip_room;
				}
				clut_idx = 0x1f2;
				if (Flg_ck(G.SaveData.Map_c_flg + 1, pRoom[-1].Change_flag) != 0) {
					clut_idx = 0x1f7;
				}
			}
			goto write_clut;
		}
		if (Flg_ck(G.SaveData.Map_flg, pRoom[-1].Map_flg) != 0) {
			goto write_clut;
		}
		goto skip_room;

write_clut:
		clut_val = GetClut(0x100, clut_idx);
		*pClut = clut_val;
		AddPrim(ot, (ULONG *)p);

skip_room:
		pClut += 0x14;
		p += 0x28;
		pRoom = pPrev;
		if (room_idx == 0) {
			AddPrim(ot, (ULONG *)&Map_end_tag[G.Cbuf_id]);
			if ((G.Status_flg & STAT_SPECIAL_RUN) == 0) {
				St_disp_map_floor();
			}
			return;
		}
	} while (1);
}

/* DS 0x8006e7f0 (Check_map_no) */
ULONG Check_map_no(register ULONG stage, register ULONG room)
{
	ULONG ret_no;

	switch (stage) {
	case 0:
		if ((room == 0x11) || (room == 0x17) || ((room == 0x12) && (G.Player.nFloor == 3))) {
			return 4;
		}
		if (room < 4) {
			return 0;
		}
		if (room < 8) {
			return 1;
		}
		if (room < 0x16) {
			return 3;
		}
		if (room == 0x16) {
			if (G.Player.nFloor == 4) {
				return 3;
			}
			return 2;
		}
		return 0;
	case 1:
		if (room == 0x1b) {
			return 3;
		}
		if (room < 0x11) {
			return 2;
		}
		return 5;
	case 2:
		if (room < 2) {
			return 5;
		}
		if (room != 8) {
			return 6;
		}
		return 5;
	case 3:
		if ((((room == 0) && (G.Player.nFloor > 3)) || ((room == 1) && (G.Player.nFloor > 3))) ||
			(((room == 3) && (G.Player.nFloor > 3)) || (room == 0xb)) ||
			((room == 2) || (room == 9))) {
			return 7;
		}
		return 8;
	case 4:
		ret_no = 9;
		if (2 < room) {
			if (room == 8) {
				ret_no = 9;
			} else {
				if (((G.Status_flg & STAT_SCENARIO_B) == 0) || (room != 3)) {
					if (room == 5) {
						if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
							ret_no = Flg_ck(G.SaveData.Common_flg, 0xbe);
							if (ret_no != 0) {
								return 0x10;
							}
						} else {
							ret_no = Flg_ck(G.SaveData.Common_flg, 0xbe);
							if (ret_no != 0) {
								return 0xd;
							}
						}
						if (Flg_ck(G.SaveData.Common_flg, 99) != 0) {
							return 0xc;
						}
					}
					if (5 < room) {
						if (room == 9) {
							return 0xc;
						}
						return 0xb;
					}
				} else {
					if (Flg_ck(G.SaveData.Common_flg, 0x62) == 0) {
						return 0xb;
					}
				}
				ret_no = 10;
			}
		}
		return ret_no;
	case 5:
		if (room == 1) {
			ret_no = 0x10;
			if (G.SaveData.Room_no_old == 0x602) {
				ret_no = 0xd;
			}
		} else if ((room != 3) || (ret_no = 0xe, G.Player.nFloor > 2)) {
			if ((room == 0xe) && (bk_cut[0] == 1)) {
				ret_no = 0x11;
			} else {
				ret_no = 0xd;
				if (4 < room) {
					if (room == 5) {
						ret_no = 0xf;
					} else {
						ret_no = 0x11;
						if (room < 0x12) {
							ret_no = 0x10;
						}
					}
				}
			}
		}
		return ret_no;
	case 6:
		if (Flg_ck(G.SaveData.Common_flg, 0x89) != 0) {
			return 0x13;
		}
		return 0x12;
	default:
		return 0;
	}
}

/* DS 0x8006eae8 (Check_room_no) */
ULONG Check_room_no(register ULONG map_no, register ULONG room)
{
	ULONG ret_no;

	ret_no = room;
	switch (map_no) {
	case 0:
		if (room > 0x17) {
			ret_no = room - 0x14;
		}
		break;
	case 2:
		if (room == 0x16) {
			ret_no = 0x11;
		}
		break;
	case 3:
		if (room == 0x1b) {
			ret_no = 0xe;
		} else {
			ret_no = room - 8;
			if (room > 0x11) {
				ret_no = room - 9;
			}
		}
		break;
	case 4:
		if (room != 0x17) {
			return room - 0x10;
		}
		ret_no = 0;
		break;
	case 5:
		if (room == 8) {
			ret_no = 2;
		} else if (room < 0x19) {
			if (room < 0x11) {
				return room;
			}
			return room - 0xe;
		} else {
			ret_no = room - 0x10;
		}
		break;
	case 6:
		ret_no = room - 3;
		if (room < 9) {
			ret_no = room - 2;
		}
		break;
	case 7:
		if (room == 9) {
			ret_no = 4;
		} else if (room == 0xb) {
			ret_no = 5;
		}
		break;
	case 8:
		if (room < 2) {
			break;
		}
		if (room < 6) {
			return room - 1;
		}
		if (room < 9) {
			return room - 2;
		}
		if (room < 0xb) {
			return room - 3;
		}
		if (room > 0x10) {
			return room;
		}
		ret_no = room - 4;
		break;
	case 1:
		ret_no = room - 4;
		break;
	case 9:
		if (room == 8) {
			ret_no = 3;
		}
		break;
	case 10:
		ret_no = room - 3;
		break;
	case 0xb:
		if (room != 3) {
			return room - 5;
		}
		ret_no = 0;
		break;
	case 0xc:
		ret_no = (room != 9);
		break;
	case 0xd:
	case 0x12:
		break;
	case 0x10:
		if (room == 5) {
			return 0xc;
		}
		if (room == 1) {
			return 0xb;
		}
		if (room != 0x11) {
			return room - 6;
		}
		ret_no = 2;
		break;
	case 0x11:
		ret_no = room - 0x12;
		if (room == 0xe) {
			ret_no = 6;
		}
		break;
	case 0x13:
		ret_no = (room != 3);
		break;
	default:
		ret_no = 0;
		break;
	}
	return ret_no;
}

/* DS 0x8006ecb8 (St_menu1_map) */
VOID St_menu1_map(VOID)
{
	UCHAR old_map;
	UCHAR new_blink;
	USHORT file_no;
	RECT rect;

	old_map = St.Map_no;
	if (St.Sub_rtn1 == 0) {
		if (St.blink_sw0 == 0) {
			new_blink = St.blink_timer0 + 2;
			if (0x50 < St.blink_timer0) {
				St.blink_sw0 = 1;
				new_blink = St.blink_timer0 + 2;
			}
		} else {
			new_blink = St.blink_timer0 - 2;
			if (St.blink_timer0 < 10) {
				St.blink_sw0 = 0;
			}
		}
		St.blink_timer0 = new_blink;
		if (((int)G.System_flg < 0) && (1 < St.Map_no)) {
			if (((G.Padd1 & 0x1000) != 0) && (1 < map_up_floor[St.Map_no]) &&
				(Flg_ck(&G.SaveData.Map_o_flg, map_up_floor[St.Map_no]) != 0)) {
				St.Map_no = map_up_floor[St.Map_no];
			}
			if (((G.Padd1 & 0x4000) != 0) && (1 < map_down_floor[St.Map_no]) &&
				(Flg_ck(&G.SaveData.Map_o_flg, map_down_floor[St.Map_no]) != 0)) {
				St.Map_no = map_down_floor[St.Map_no];
			}
			if (old_map != St.Map_no) {
				St.Sub_rtn1 = 1;
				Snd_se_stad(0x4040000, (VECTOR *)0);
				return;
			}
		}
		if (((G.Key_trg & 0x4000) != 0) || ((G.Key_trg & 0x6000) != 0)) {
			Snd_se_stad(0x4050000, (VECTOR *)0);
			St.Sub_rtn0 = 0;
		}
	} else if (St.Sub_rtn1 == 1) {
		file_no = Map_info[St.Map_no].File_no;
		CD.Fno = MAPS_PIX;
		CD.Fsize = maps_floc[file_no].size;
		CD.Fsum = maps_floc[file_no].sum;
		CD.Fsector = (ULONG)floc[MAPS_PIX].sec_high * 0x10000 + (ULONG)floc[MAPS_PIX].sector +
					 (ULONG)maps_floc[file_no].sec_high * 0x10000 + (ULONG)maps_floc[file_no].sector;
		Cd_read(MAPS_PIX, (ULONG *)St_load_buf, 2, (UCHAR *)"MAP FILE");
		rect.x = 0x1c0;
		rect.y = 0x100;
		rect.w = 0x40;
		rect.h = 0x100;
		LoadImage(&rect, (ULONG *)St_load_buf);
		DrawSync(0);
		St_init_disp_map();
		St.blink_sw0 = 0;
		St.blink_timer0 = 2;
		St.Sub_rtn1 = 0;
	}
}

/* DS 0x8006efd8 (St_init03) */
VOID St_init03(VOID)
{
	register ULONG file_no;
	auto RECT rect;
	ULONG map_no;

	St.Main_rtn = 1;
	St.Sub_rtn0 = 2;
	map_no = Check_map_no((int)G.SaveData.Stage_no, (int)G.SaveData.Room_no);
	St.Map_no = (UCHAR)map_no;
	file_no = Map_info[map_no].File_no;
	CD.Fno = MAPS_PIX;
	CD.Fsize = maps_floc[file_no].size;
	CD.Fsum = maps_floc[file_no].sum;
	CD.Fsector = (UINT)floc[MAPS_PIX].sec_high * 0x10000 + (UINT)floc[MAPS_PIX].sector +
				 (UINT)maps_floc[file_no].sec_high * 0x10000 + (UINT)maps_floc[file_no].sector;
	Cd_read(MAPS_PIX, (ULONG *)St_load_buf, 2, (UCHAR *)"MAP FILE");
	rect.x = 0x1c0;
	rect.y = 0x100;
	rect.w = 0x40;
	rect.h = 0x100;
	LoadImage(&rect, (ULONG *)St_load_buf);
	DrawSync(0);
	St.Map_offset_x = 0;
	St.Map_offset_y = 0;
	St_init_disp_map();
	Fade_set(0x200, -0x1800, 7, 1);
	do {
		Task_sleep(1);
		St_disp_map();
		St_disp_wall();
	} while (Fade_status(0) == 0);
	Snd_se_stad(0x4090000, (VECTOR *)0x0);
}

/* DS 0x8006f1c4 (St_disp_map_rooms): draws the map room cells, the blinking cursor and the room change flags. */
VOID St_disp_map_rooms(VOID)
{
	UCHAR    Blink_val;
	UCHAR    Tex_u;
	UCHAR    Tex_v;
	UCHAR    Bits;
	USHORT   Clut;
	INT      Clut_idx;
	INT      Blink_cnt;
	ULONG    Cur_room;
	ULONG    Flg_val;
	ULONG    Room_ck;
	ULONG   *ot;
	UCHAR   *pPrim;
	UCHAR   *pPrim_top;
	SHORT   *pXy;
	SHORT    Ax;
	SHORT    Ay;
	USHORT  *pQuad;
	MAP_DATA *pMap;
	MAP_DATA *pRoom;
	MAP_DATA *pPrev;

	ot = G.Ot0[G.Cbuf_id] + 5;
	pPrim = (UCHAR *)St_sprt_b;
	if (St.blink_sw0 == 0) {
		Blink_val = St.blink_timer0 + 2;
		if (St.blink_timer0 > 0x50) {
			St.blink_sw0 = 1;
			Blink_val = St.blink_timer0 + 2;
		}
	} else {
		if (St.blink_timer0 < 10) {
			Snd_se_stad(0x22b0000, (VECTOR *)0);
			St.blink_sw0 = 0;
		}
		Blink_val = St.blink_timer0 - 2;
	}
	if (G.Cbuf_id != 0) {
		pPrim = (UCHAR *)(St_sprt_b + 1);
	}

	Blink_cnt = 2;
	pXy = &((SPRT *)pPrim + 6)->y0;
	Ay = 0x1a;
	St.blink_timer0 = Blink_val;
	Room_ck = Check_room_no(St.Map_no, St_room_tbl[*((UCHAR *)&St.Dmode[3][0].tag + 2)]);
	Ax = 0x1ea;
	pMap = Map_info[St.Map_no].pMap_data;
	pPrim += 0x78;
	do {
		pPrim_top = pPrim;
		Ay -= 5;
		Ax -= 0xf0;
		Blink_cnt--;
		pXy[-1] = Ax;
		pXy[0] = Ay;
		AddPrim(ot, pPrim_top);
		pXy += 0x14;
		pPrim = pPrim_top + 0x28;
	} while (Blink_cnt != 0);

	Cur_room = Map_info[St.Map_no].Room_num;
	pQuad = &((SPRT *)pPrim_top + 2)->clut;
	pRoom = pMap + Cur_room;
	do {
		pPrev = pRoom - 1;
		pQuad[-3] = pRoom[-1].C_pos_x + St.Map_offset_x;
		pQuad[-2] = pRoom[-1].C_pos_y + St.Map_offset_y;
		Cur_room--;

		if (St.Map_no == 2 && Cur_room == 0xe) {
			/* stage 2, room 14: the two door-side cells */
			if (G.SaveData.Room_no == 0xe) {
				Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 8);
				if (Flg_val != 0) {
					Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					Clut_idx = 0x1fa;
					if (Flg_val == 0) {
						Clut_idx = 0x1fe;
						Tex_u = pMap[0xe].U;
						Tex_v = 0x98;
					} else {
						Tex_u = pMap[0xe].U;
						Tex_v = 0x50;
					}
					((UCHAR *)pQuad)[-2] = Tex_u;
					((UCHAR *)pQuad)[-1] = Tex_v;
				}
				Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 9);
				if (Flg_val != 0) {
					Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 0xb);
					Clut_idx = 0x1fa;
					if (Flg_val == 0) {
						Clut_idx = 0x1fe;
						Tex_u = pPrev->U;
						Tex_v = 0x50;
					} else {
						Tex_u = pPrev->U;
						Tex_v = 0x98;
					}
					((UCHAR *)pQuad)[-2] = Tex_u;
					((UCHAR *)pQuad)[-1] = Tex_v;
				}
			} else {
				Flg_val = Flg_ck(G.SaveData.Map_flg, 0x2c);
				if (Flg_val == 0) {
					goto Map_next;
				}
				Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 0xb);
				if (Flg_val == 0) {
					Clut_idx = 0x1f2;
					Tex_u = pMap[0xe].U;
					((UCHAR *)pQuad)[-2] = Tex_u;
					((UCHAR *)pQuad)[-1] = 0x50;
					Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					if (Flg_val != 0) {
						Clut_idx = 0x1fd;
					}
				} else {
					Clut_idx = 0x1fd;
					Tex_u = pMap[0xe].U;
					((UCHAR *)pQuad)[-2] = Tex_u;
					((UCHAR *)pQuad)[-1] = 0x98;
					Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, 0xc);
					if (Flg_val != 0) {
						Clut_idx = 0x1f5;
					}
				}
			}
			goto Map_clut;
		}

		/* every other room: pick the CLUT row from the room's flags */
		Clut_idx = 0x1f5;
		Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, pRoom[-1].Change_flag);
		if (Flg_val != 0) {
			Clut_idx = 0x1fa;
		}
		if (Cur_room == Room_ck) {
			if (St.blink_sw0 == 0) {
				Clut_idx++;
				goto Map_clut;
			}
			Bits = pRoom[-1].Change_flag;
			goto Map_blink_bits;
		}
		Flg_val = Flg_ck(&G.SaveData.Map_i_flg, Map_info[St.Map_no].Map);
		if (Flg_val == 0) {
			Flg_val = Flg_ck(G.SaveData.Map_flg, pRoom[-1].Map_flg);
			if (Flg_val != 0) {
				goto Map_clut;
			}
		} else {
			Flg_val = Flg_ck(G.SaveData.Map_flg, pRoom[-1].Map_flg);
			if (Flg_val != 0) {
				goto Map_clut;
			}
			if ((G.Player.Id & 1) == 0) {
				Bits = pRoom[-1].Pl0_flg;
			} else {
				Bits = pRoom[-1].Pl1_flg;
			}
			if ((((Bits & 1) == 0) || ((G.Status_flg & STAT_SCENARIO_B) != 0)) &&
				(((Bits & 2) == 0) || ((G.Status_flg & STAT_SCENARIO_B) == 0))) {
				Bits = pRoom[-1].Change_flag;
				goto Map_blink_bits;
			}
		}
		goto Map_next;

Map_blink_bits:
		Clut_idx = 0x1f2;
		Flg_val = Flg_ck(G.SaveData.Map_c_flg + 1, Bits);
		if (Flg_val != 0) {
			Clut_idx = 0x1f7;
		}

Map_clut:
		Clut = GetClut(0x100, Clut_idx);
		*pQuad = Clut;
		AddPrim(ot, pPrim);

Map_next:
		pQuad += 0x14;
		pPrim += 0x28;
		pRoom = pPrev;
		if (Cur_room == 0) {
			AddPrim(ot, (ULONG *)&Map_end_tag[G.Cbuf_id]);
			if ((G.Status_flg & STAT_SPECIAL_RUN) == 0) {
				St_disp_map_floor();
			}
			return;
		}
	} while (1);
}

/* DS 0x8006f164 (St_main03) */
VOID St_main03(VOID)
{
	static CODE_PTR Status_direct_map_proc_tbl[3] = {St_fade_out_set, St_fade_out_wait, St_menu1_map};

	((CODE_PTR)((Status_direct_map_proc_tbl)[St.Sub_rtn0]))();
	if (St.Sub_rtn1 == 0) {
		St_disp_map();
	}
	St_disp_wall();
}

/* DS 0x8006f6a8 (St_init_map_rooms) */
VOID St_init_map_rooms(VOID)
{
	RECT Rect;
	USHORT File_no;
	ULONG Fade_st;

	St.Main_rtn = 1;
	St.blink_sw0 = 1;
	St.Sub_rtn0 = 2;
	St.Map_offset_x = 0;
	St.Map_offset_y = 0;
	St.blink_timer0 = 10;
	switch (((UCHAR *)&St.Dmode[3][0].tag)[2]) {
	case 0:
		St.Map_no = 5;
		break;
	case 1:
	case 4:
		St.Map_no = 3;
		break;
	case 2:
		St.Map_no = 5;
		break;
	case 3:
		St.Map_no = 0x10;
		break;
	}
	File_no = Map_info[St.Map_no].File_no;
	CD.Fno = MAPS_PIX;
	CD.Fsize = maps_floc[File_no].size;
	CD.Fsum = maps_floc[File_no].sum;
	CD.Fsector = (ULONG)floc[MAPS_PIX].sec_high * 0x10000 + (ULONG)floc[MAPS_PIX].sector +
				 (ULONG)maps_floc[File_no].sec_high * 0x10000 + (ULONG)maps_floc[File_no].sector;
	Cd_read(MAPS_PIX, (ULONG *)St_load_buf, 2, (UCHAR *)"MAP FILE");
	Rect.x = 0x1c0;
	Rect.y = 0x100;
	Rect.w = 0x40;
	Rect.h = 0x100;
	LoadImage(&Rect, (ULONG *)St_load_buf);
	DrawSync(0);
	St_init_disp_map();
	Fade_set(0x200, -0x1800, 7, 1);
	do {
		Task_sleep(1);
		St_disp_map_rooms();
		St_disp_wall();
		Fade_st = Fade_status(0);
	} while (Fade_st == 0);
	Snd_se_stad(0x4090000, (VECTOR *)0);
}

/* DS 0x8006f878 (St_map_cancel) */
VOID St_map_cancel(VOID)
{
	if ((G.Key_trg & 0x6000) != 0) {
		Snd_se_stad(0x4050000, (VECTOR *)0);
		St.Sub_rtn0 = 0;
	}
}

/* DS 0x800a9ba8 (Status_direct_map_proc_tbl2) */
void (*Status_direct_map_proc_tbl2[3])(void) = {
	St_fade_out_set,
	St_fade_out_wait,
	St_map_cancel
};

/* DS 0x8006f8b4 (St_main_map_rooms) */
VOID St_main_map_rooms(VOID)
{
	((CODE_PTR)(((ULONG *)Status_direct_map_proc_tbl2)[St.Sub_rtn0]))();
	St_disp_map_rooms();
	St_disp_wall();
}

/* DS 0x800a9414 (maps_floc) */
tagfpos maps_floc[20] = {
	{ 0x00008000, 0x0000, 0x00, 0x50 },
	{ 0x00008000, 0x0010, 0x00, 0x60 },
	{ 0x00008000, 0x0020, 0x00, 0x49 },
	{ 0x00008000, 0x0030, 0x00, 0x3d },
	{ 0x00008000, 0x0040, 0x00, 0x0e },
	{ 0x00008000, 0x0050, 0x00, 0x86 },
	{ 0x00008000, 0x0060, 0x00, 0xc4 },
	{ 0x00008000, 0x0070, 0x00, 0x44 },
	{ 0x00008000, 0x0080, 0x00, 0x27 },
	{ 0x00008000, 0x0090, 0x00, 0xbe },
	{ 0x00008000, 0x00a0, 0x00, 0x8d },
	{ 0x00008000, 0x00b0, 0x00, 0x8d },
	{ 0x00008000, 0x00c0, 0x00, 0xc9 },
	{ 0x00008000, 0x00d0, 0x00, 0xf4 },
	{ 0x00008000, 0x00e0, 0x00, 0xf0 },
	{ 0x00008000, 0x00f0, 0x00, 0xf0 },
	{ 0x00008000, 0x0100, 0x00, 0xb0 },
	{ 0x00008000, 0x0110, 0x00, 0xf4 },
	{ 0x00008000, 0x0120, 0x00, 0x07 },
	{ 0x00008000, 0x0130, 0x00, 0x00 }
};

/* DS 0x800a94b4 (fits_floc) */
tagfpos fits_floc[120] = {
	{ 0x0000ca60, 0x0000, 0x00, 0x36 },
	{ 0x00004840, 0x001a, 0x00, 0x98 },
	{ 0x00004840, 0x0024, 0x00, 0x83 },
	{ 0x00004840, 0x002e, 0x00, 0xf7 },
	{ 0x00004840, 0x0038, 0x00, 0x30 },
	{ 0x00004840, 0x0042, 0x00, 0x53 },
	{ 0x00004840, 0x004c, 0x00, 0x48 },
	{ 0x0000da60, 0x0056, 0x00, 0x79 },
	{ 0x00005840, 0x0072, 0x00, 0xaf },
	{ 0x00005840, 0x007e, 0x00, 0x73 },
	{ 0x00005840, 0x008a, 0x00, 0xc0 },
	{ 0x00005840, 0x0096, 0x00, 0x65 },
	{ 0x00005840, 0x00a2, 0x00, 0xc6 },
	{ 0x00005840, 0x00ae, 0x00, 0x3b },
	{ 0x0000da60, 0x00ba, 0x00, 0x4f },
	{ 0x00005840, 0x00d6, 0x00, 0x00 },
	{ 0x00005840, 0x00e2, 0x00, 0xed },
	{ 0x0000da60, 0x00ee, 0x00, 0x28 },
	{ 0x00005840, 0x010a, 0x00, 0xf3 },
	{ 0x00005840, 0x0116, 0x00, 0xb4 },
	{ 0x00005840, 0x0122, 0x00, 0x10 },
	{ 0x0000da60, 0x012e, 0x00, 0x1f },
	{ 0x00005840, 0x014a, 0x00, 0xa3 },
	{ 0x00005840, 0x0156, 0x00, 0x1f },
	{ 0x00005840, 0x0162, 0x00, 0x14 },
	{ 0x00005840, 0x016e, 0x00, 0x84 },
	{ 0x00005840, 0x017a, 0x00, 0x24 },
	{ 0x00005840, 0x0186, 0x00, 0xd8 },
	{ 0x00005840, 0x0192, 0x00, 0x96 },
	{ 0x00005840, 0x019e, 0x00, 0xdf },
	{ 0x00005840, 0x01aa, 0x00, 0x67 },
	{ 0x00005840, 0x01b6, 0x00, 0xd0 },
	{ 0x0000da60, 0x01c2, 0x00, 0x7f },
	{ 0x00005840, 0x01de, 0x00, 0x90 },
	{ 0x00005840, 0x01ea, 0x00, 0x6f },
	{ 0x00005840, 0x01f6, 0x00, 0xf0 },
	{ 0x00005840, 0x0202, 0x00, 0x19 },
	{ 0x00005840, 0x020e, 0x00, 0xf2 },
	{ 0x00005840, 0x021a, 0x00, 0xa9 },
	{ 0x00005840, 0x0226, 0x00, 0x14 },
	{ 0x00005840, 0x0232, 0x00, 0x88 },
	{ 0x00005840, 0x023e, 0x00, 0x10 },
	{ 0x00005840, 0x024a, 0x00, 0x15 },
	{ 0x0000da60, 0x0256, 0x00, 0xf2 },
	{ 0x00005840, 0x0272, 0x00, 0x90 },
	{ 0x00005840, 0x027e, 0x00, 0x6f },
	{ 0x00005840, 0x028a, 0x00, 0xf0 },
	{ 0x00005840, 0x0296, 0x00, 0x19 },
	{ 0x00005840, 0x02a2, 0x00, 0xf2 },
	{ 0x00005840, 0x02ae, 0x00, 0xa9 },
	{ 0x00005840, 0x02ba, 0x00, 0x14 },
	{ 0x00005840, 0x02c6, 0x00, 0x88 },
	{ 0x00005840, 0x02d2, 0x00, 0x10 },
	{ 0x00005840, 0x02de, 0x00, 0x15 },
	{ 0x0000ca60, 0x02ea, 0x00, 0x96 },
	{ 0x00004840, 0x0304, 0x00, 0x18 },
	{ 0x00004840, 0x030e, 0x00, 0x44 },
	{ 0x00004840, 0x0318, 0x00, 0x89 },
	{ 0x00004840, 0x0322, 0x00, 0x4c },
	{ 0x00004840, 0x032c, 0x00, 0x10 },
	{ 0x00004840, 0x0336, 0x00, 0xf3 },
	{ 0x00004840, 0x0340, 0x00, 0x4b },
	{ 0x0000ca60, 0x034a, 0x00, 0xc3 },
	{ 0x00004840, 0x0364, 0x00, 0x18 },
	{ 0x00004840, 0x036e, 0x00, 0xf3 },
	{ 0x00004840, 0x0378, 0x00, 0x78 },
	{ 0x00004840, 0x0382, 0x00, 0xc8 },
	{ 0x00004840, 0x038c, 0x00, 0x10 },
	{ 0x0000da60, 0x0396, 0x00, 0x1f },
	{ 0x00005840, 0x03b2, 0x00, 0xa3 },
	{ 0x00005840, 0x03be, 0x00, 0x80 },
	{ 0x00005840, 0x03ca, 0x00, 0xc6 },
	{ 0x00005840, 0x03d6, 0x00, 0x20 },
	{ 0x00005840, 0x03e2, 0x00, 0xf8 },
	{ 0x00005840, 0x03ee, 0x00, 0x03 },
	{ 0x00005840, 0x03fa, 0x00, 0xc0 },
	{ 0x0000c260, 0x0406, 0x00, 0xb5 },
	{ 0x00004040, 0x041f, 0x00, 0x10 },
	{ 0x00004040, 0x0428, 0x00, 0x26 },
	{ 0x0000ba60, 0x0431, 0x00, 0x65 },
	{ 0x00003840, 0x0449, 0x00, 0x98 },
	{ 0x00003840, 0x0451, 0x00, 0x10 },
	{ 0x00003840, 0x0459, 0x00, 0x10 },
	{ 0x0000ba60, 0x0461, 0x00, 0xec },
	{ 0x00003840, 0x0479, 0x00, 0x98 },
	{ 0x00003840, 0x0481, 0x00, 0x10 },
	{ 0x00003840, 0x0489, 0x00, 0x74 },
	{ 0x00003840, 0x0491, 0x00, 0xc9 },
	{ 0x0000ba60, 0x0499, 0x00, 0x32 },
	{ 0x00003840, 0x04b1, 0x00, 0x98 },
	{ 0x00003840, 0x04b9, 0x00, 0x10 },
	{ 0x00003840, 0x04c1, 0x00, 0xa8 },
	{ 0x0000ca60, 0x04c9, 0x00, 0x03 },
	{ 0x00004840, 0x04e3, 0x00, 0xa3 },
	{ 0x00004840, 0x04ed, 0x00, 0xfb },
	{ 0x00004840, 0x04f7, 0x00, 0x6c },
	{ 0x00004840, 0x0501, 0x00, 0xe3 },
	{ 0x0000da60, 0x050b, 0x00, 0x04 },
	{ 0x00005840, 0x0527, 0x00, 0x95 },
	{ 0x00005840, 0x0533, 0x00, 0x43 },
	{ 0x00005840, 0x053f, 0x00, 0xdb },
	{ 0x00005840, 0x054b, 0x00, 0x48 },
	{ 0x00005840, 0x0557, 0x00, 0x63 },
	{ 0x00005840, 0x0563, 0x00, 0x06 },
	{ 0x00005840, 0x056f, 0x00, 0x49 },
	{ 0x00005840, 0x057b, 0x00, 0xb3 },
	{ 0x00005840, 0x0587, 0x00, 0x58 },
	{ 0x0000da60, 0x0593, 0x00, 0x6a },
	{ 0x00005840, 0x05af, 0x00, 0x93 },
	{ 0x00005840, 0x05bb, 0x00, 0x31 },
	{ 0x00005840, 0x05c7, 0x00, 0x34 },
	{ 0x00005840, 0x05d3, 0x00, 0x6d },
	{ 0x00005840, 0x05df, 0x00, 0x29 },
	{ 0x00005840, 0x05eb, 0x00, 0xe1 },
	{ 0x00005840, 0x05f7, 0x00, 0x8a },
	{ 0x0000da60, 0x0603, 0x00, 0x18 },
	{ 0x00005840, 0x061f, 0x00, 0xb6 },
	{ 0x00005840, 0x062b, 0x00, 0x03 },
	{ 0x00005840, 0x0637, 0x00, 0xf8 },
	{ 0x00005840, 0x0643, 0x00, 0x94 }
};

/* DS 0x800a9aec (map_up_floor) */
UCHAR map_up_floor[24] = {
	0x00, 0x00, 0x03, 0x04, 0x00, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x0e,
	0x0f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* DS 0x800a9b04 (map_down_floor) */
UCHAR map_down_floor[24] = {
	0x00, 0x00, 0x05, 0x02, 0x03, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0x10,
	0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* DS 0x800aaa38 (Map_info) */
MAP_STAGE_INFO Map_info[20] = {
	{ Map_stage1_0, 0x08, 0x00, 0x0000 },
	{ Map_stage1_1, 0x04, 0x01, 0x0001 },
	{ Map_stage2_0, 0x12, 0x02, 0x0004 },
	{ Map_stage1_2, 0x0f, 0x03, 0x0002 },
	{ Map_stage1_3, 0x03, 0x04, 0x0003 },
	{ Map_stage2_1, 0x0b, 0x05, 0x0005 },
	{ Map_stage3_0, 0x0b, 0x06, 0x0006 },
	{ Map_stage4_0, 0x06, 0x08, 0x0007 },
	{ Map_stage4_1, 0x0d, 0x07, 0x0008 },
	{ Map_stage5_0, 0x04, 0x09, 0x0009 },
	{ Map_stage5_1, 0x03, 0x0a, 0x000a },
	{ Map_stage5_2, 0x03, 0x0b, 0x000b },
	{ Map_stage5_3, 0x02, 0x0c, 0x000c },
	{ Map_stage6_0, 0x06, 0x0d, 0x000d },
	{ Map_stage6_1, 0x01, 0x0e, 0x000e },
	{ Map_stage6_2, 0x01, 0x0f, 0x000f },
	{ Map_stage6_3, 0x0d, 0x10, 0x0010 },
	{ Map_stage6_4, 0x07, 0x11, 0x0011 },
	{ Map_stage7_0, 0x06, 0x12, 0x0012 },
	{ Map_stage7_1, 0x02, 0x13, 0x0013 }
};
