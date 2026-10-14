/*
 * ST_SUB01.C - Status screen item box and item menu logic
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB01.C
 */
#include "TYPES.H"
#include "ST_SUB02.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include "ST_SUB01.H"
#include "CD.H"
#include "GAME.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "SND.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "ST_SUB04.H"
#include "DOOR.H"
#include "FILECODE.H"

LINE_F2 St_prim_c[6];		/* Item box lines: 6, written by St_init_itembox */

POLY_F4 St_prim_b[8];		/* Item box polygons: 4 x 2 = 8, written by St_init_itembox */

POLY_FT4 St_prim_a[12];		/* item box cells: 6 x 2, written by St_init_itembox */

/* DS 0x8006f900 (St_init01) */
VOID St_init01(VOID)
{
	register STATUS *pSt;
	register GLOBAL *pG;
	register ULONG i;

	pSt = &St;
	pG = &G;

	Cd_read(ITEMALL_PIX, (ULONG *)St_item_img, 0, (UCHAR *)"ITEM_ALL");
	pSt->Main_rtn = 1;
	pSt->Sub_rtn0 = 3;
	pSt->Ibox_moxe = 0;
	pSt->Ibox_sel = 0;
	St_init_itembox();
	SetDrawMode((DR_MODE *)(pSt->Dmode[0x17][0].code + 1), 0, 0,
				(pG->Ob_model[pSt->Itembox_tpage].Tpage & 0xf) | 0x80, (RECT *)0x0);
	SetDrawMode((DR_MODE *)(pSt->Dmode[0x17][1].code + 1), 0, 0,
				(pG->Ob_model[pSt->Itembox_tpage].Tpage & 0xf) | 0x80, (RECT *)0x0);
	Pix_trans_pl(7, (ULONG)pG->SaveData.Item_boxwork[pSt->Ibox_sel].Id,
				 (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_boxwork[pSt->Ibox_sel].Id * 0x4b0));
	Move_x[0] = 1;
	sw = 0;
	{
		ULONG k = 5;
		i = 0x16;
		do {
			ULONG id;
			k--;
			id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + k) & 0x3f].Id;
			Pix_trans_pl(i, id, (ULONG *)(St_item_img + id * 0x4b0));
			i -= 3;
		} while (k != 0);
	}
	Fade_set(0x200, -0x1800, 7, 1);
	do {
		Task_sleep(1);
		St_disp_itembox();
		St_disp_itemlist();
		St_disp_num();
		St_disp_equip();
		St_disp_message();
		St_disp_name(0x10, 0xaf, 2, (ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Id);
		St_disp_cursol0((ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Size);
	} while (Fade_status(0) == 0);
}

/* DS 0x8006fb64 (St_main01) */
VOID St_main01(VOID)
{
	static CODE_PTR St_itembox_proc_tbl[4] = {St_fade_out_set, St_fade_out_wait, St_select_menu_m, St_select_item_m};

	St.Exit_flg = 1;
	((CODE_PTR)(St_itembox_proc_tbl[St.Sub_rtn0]))();
	St_disp_itembox();
	St_disp_itemlist();
	St_disp_num();
	St_disp_equip();
	St_disp_message();
}

/* DS 0x8006fbd4 (St_select_menu_m) */
VOID St_select_menu_m(VOID)
{
	if ((G.Padd1 & 0x4000) != 0) {
		St.Icursol_main = 1;
		St.Sub_rtn0++;
		Snd_se_stad(0x4040000, (VECTOR *)0x0);
	} else {
		if ((G.Key_trg & 0x3000) != 0) {
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
			St.Sub_rtn0 = 0;
		}
	}
}

/* DS 0x8006fc58 (St_select_item_m) */
VOID St_select_item_m(VOID)
{
	register STATUS *pSt;
	register GLOBAL *pG;
	register ULONG c_old;
	UCHAR nsel;

	pSt = &St;
	pG = &G;

	switch (pSt->Sub_rtn1) {
	case 0:
		c_old = (ULONG)pSt->Icursol_main;
		if ((int)pG->System_flg < 0) {
			if (((pG->Padd1 & 0x2000) != 0) &&
				((int)c_old < (int)(pG->SaveData.Max_item_num - 1))) {
				if (pG->SaveData.Item_work[c_old].Size == 1) {
					pSt->Icursol_main++;
				}
				pSt->Icursol_main++;
			}
			if (((pG->Padd1 & 0x8000) != 0) &&
				(ULONG)pSt->Icursol_main != 0 &&
				(ULONG)pSt->Icursol_main != 10) {
				UCHAR tmp = pSt->Icursol_main;
				if (pG->SaveData.Item_work[(ULONG)pSt->Icursol_main].Size == 2) {
					tmp = pSt->Icursol_main - 1;
					if ((ULONG)pSt->Icursol_main == 1) {
						tmp = pSt->Icursol_main + 1;
					}
				}
				pSt->Icursol_main = tmp;
				pSt->Icursol_main += 0xff;
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				UCHAR nxt = pSt->Icursol_main + 2;
				if ((int)(ULONG)pSt->Icursol_main < (int)(pG->SaveData.Max_item_num - 2) ||
					(nxt = 1, pSt->Icursol_main == 10)) {
					pSt->Icursol_main = nxt;
				}
			}
			{
				UCHAR tmp2 = pSt->Icursol_main;
				if (((pG->Padd1 & 0x1000) != 0) &&
					(pSt->Icursol_main < 2 ||
					 (tmp2 = pSt->Icursol_main + 0xfe, pSt->Icursol_main == 10))) {
					c_old = 10;
					pSt->Sub_rtn0 += 0xff;
					tmp2 = pSt->Icursol_main;
				}
				pSt->Icursol_main = tmp2;
			}
			if (c_old != (ULONG)pSt->Icursol_main) {
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
			}
		}
		if ((pG->Key_trg & 0x1000) != 0) {
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
			pSt->Sub_rtn1 = 1;
		} else if ((pG->Key_trg & 0x2000) != 0) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			pSt->Sub_rtn0 = 0;
		} else if ((pG->Key_trg & 0x4000) != 0 && pSt->Exit_flg == 1) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			pSt->Sub_rtn0 = 1;
		}
		break;

	case 1:
		if ((int)pG->System_flg < 0) {
			if ((pG->Padd1 & 0x1000) != 0) {
				ULONG id;
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 2;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
			} else if ((pG->Padd1 & 0x4000) != 0) {
				ULONG id;
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 3;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
			} else if ((pG->Padd1 & 4) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				nsel = pSt->Ibox_sel - 5;
no_name_disp:
				pSt->Ibox_sel = nsel & 0x3f;
				{
					ULONG id;
					ULONG k;
					id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
					Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
					for (k = 0; k < 5; k++) {
						id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + k) & 0x3f].Id;
						Pix_trans_pl(10 + k * 3, id, (ULONG *)(St_item_img + id * 0x4b0));
					}
				}
				goto case1_disp;
			} else if ((pG->Padd1 & 8) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				nsel = pSt->Ibox_sel + 5;
				goto no_name_disp;
			} else {
				if ((pG->Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					pSt->Sub_rtn1 = 0;
				} else if ((pG->Key_trg & 0x1000) != 0) {
					Snd_se_stad(0x4060000, (VECTOR *)0x0);
					pSt->Sub_rtn1 = 4;
				}
			}
		} else {
			if ((pG->Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 0;
			} else if ((pG->Key_trg & 0x1000) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 4;
			}
		}
		break;

	case 2:
	{
		ULONG id;
		LONG x;
		LONG y_off;
		pSt->Exit_flg = 0;
		if (5 < pSt->Sub_rtn2) {
			if ((pG->Padd1 & 0x1000) != 0) {
				pSt->Sub_rtn2++;
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
			} else {
				pSt->Sub_rtn1 = 1;
			}
			pSt->Ibox_moxe = 0;
			pSt->Sub_rtn2 = 0;
			{
				UCHAR nsel = pSt->Ibox_sel - 1;
				ULONG k;
				pSt->Ibox_sel = nsel & 0x3f;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
				for (k = 0; k < 5; k++) {
					id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + k) & 0x3f].Id;
					Pix_trans_pl(10 + k * 3, id, (ULONG *)(St_item_img + id * 0x4b0));
				}
			}
		} else {
			pSt->Ibox_moxe += 3;
			pSt->Sub_rtn2++;
		}
case1_disp:
		id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
		if (id == 0) id = 100;
		x = pSt->Ibox_pos_x + 7;
		y_off = pSt->Ibox_moxe + 9;
		St_disp_name(x, (LONG)pSt->Ibox_pos_y + y_off, 6, id);
		break;
	}

	case 3:
	{
		ULONG id;
		LONG x;
		LONG y_off;
		pSt->Exit_flg = 0;
		if (pSt->Sub_rtn2 < 6) {
			pSt->Ibox_moxe = pSt->Ibox_moxe + -3;
			pSt->Sub_rtn2++;
		} else {
			if ((pG->Padd1 & 0x4000) != 0) {
				pSt->Sub_rtn2++;
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
			} else {
				pSt->Sub_rtn1 = 1;
			}
			{
				ULONG k;
				pSt->Ibox_moxe = 0;
				pSt->Sub_rtn2 = 0;
				pSt->Ibox_sel = (pSt->Ibox_sel + 1) & 0x3f;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
				for (k = 0; k < 5; k++) {
					id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + k) & 0x3f].Id;
					Pix_trans_pl(10 + k * 3, id, (ULONG *)(St_item_img + id * 0x4b0));
				}
			}
		}
		id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
		if (id == 0) id = 100;
		x = pSt->Ibox_pos_x + 7;
		y_off = pSt->Ibox_moxe + 0x81;
		St_disp_name(x, (LONG)pSt->Ibox_pos_y + y_off, 6, id);
		break;
	}

	case 4:
		pSt->Exit_flg = 0;
		pSt->Sub_rtn1 = 0;
		Exchange_item();
		break;

	case 5:
		pSt->Exit_flg = 0;
		if ((Moji.Mess_flg & 0x80) == 0) {
			pSt->Sub_rtn1 = 1;
		}
		St_disp_cursol0((ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Size);
		return;
	}
	St_disp_name(0x10, 0xaf, 6, (ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Id);
	St_disp_cursol0((ULONG)pG->SaveData.Item_work[pSt->Icursol_main].Size);
}

/* DS 0x800703b8 (Exchange_item) */
VOID Exchange_item(VOID)
{
	register STATUS *pSt;
	/* copies of the cursor slot taken on entry (DS keeps them in stack slots) */
	UCHAR bk_id;
	UCHAR bk_num;
	UCHAR bk_sz;
	UCHAR box_num;
	register LONG space_num;
	register ULONG sel;
	register ULONG main_idx;
	ULONG type;
	ULONG sel_id;
	ULONG i;

	pSt = &St;

	main_idx = (ULONG)pSt->Icursol_main;
	bk_id  = G.SaveData.Item_work[main_idx].Id;
	bk_num = G.SaveData.Item_work[main_idx].Num;
	bk_sz  = G.SaveData.Item_work[main_idx].Size;
	sel = (pSt->Ibox_sel + 2) & 0x3f;

	type = (G.SaveData.Item_work[main_idx].Size != 0) ? 2 : 0;
	if (G.SaveData.Item_boxwork[sel].Size != 0) {
		type++;
	}

	box_num = G.SaveData.Item_boxwork[sel].Num;
	sel_id = (ULONG)G.SaveData.Item_boxwork[sel].Id;

	/* Stacking ammo case */
	if (sel_id - 0x14 < 0xc) {
		ULONG wk_id = (ULONG)G.SaveData.Item_work[main_idx].Id;
		if (sel_id == wk_id) {
			UCHAR imax = Item_data_tbl[sel_id].Item_max;
			ULONG wk_num = (ULONG)G.SaveData.Item_work[main_idx].Num;
			if (wk_num == imax) {
				G.SaveData.Item_work[main_idx].Num = box_num;
				G.SaveData.Item_boxwork[sel].Num = imax;
				return;
			}
			wk_num += box_num;
			if (imax < wk_num) {
				G.SaveData.Item_work[main_idx].Num = imax;
				G.SaveData.Item_boxwork[sel].Num = (UCHAR)wk_num - Item_data_tbl[sel_id].Item_max;
				return;
			}
num_add:
			G.SaveData.Item_work[main_idx].Num += box_num;
			G.SaveData.Item_boxwork[sel].Id  = 0;
			G.SaveData.Item_boxwork[sel].Num = 0;
			G.SaveData.Item_boxwork[sel].Size = 0;
			Pix_trans_pl(0x10, (ULONG)G.SaveData.Item_boxwork[sel].Id,
						 (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_boxwork[sel].Id * 0x4b0));
			return;
		}
		if (wk_id == 0) {
			/* DS loop is do-while: slot 0 is always tested, even when Max_item_num is 0 */
			i = 0;
			do {
				if ((sel_id == (ULONG)G.SaveData.Item_work[i].Id) &&
					((ULONG)G.SaveData.Item_work[i].Num <=
					 (ULONG)Item_data_tbl[sel_id].Item_max - (ULONG)box_num)) {
					main_idx = i;
					goto num_add;
				}
				i++;
			} while (i < G.SaveData.Max_item_num);
		}
	}

	if (type == 1) {
		/* Box item → empty player slot */
		space_num = Search_item(1);
		if (((space_num == 1) && (G.SaveData.Item_work[main_idx].Id == 0)) || (space_num == 0)) {
			Mess_set(0xaf0010, 0xe400, 8, 0);
			pSt->Sub_rtn1 = 5;
			return;
		}
		if (pSt->Equip_item_no == (UCHAR)main_idx) {
			pSt->Equip_item_no = 0x80;
			pSt->Equip_item_id = 0;
		}
		G.SaveData.Item_work[main_idx].Id = 0;
		Sort_item();
		Shift_item(1);
		G.SaveData.Item_work[0].Id   = G.SaveData.Item_boxwork[sel].Id;
		G.SaveData.Item_work[0].Num  = G.SaveData.Item_boxwork[sel].Num;
		G.SaveData.Item_work[0].Size = 1;
		G.SaveData.Item_work[1].Size = 2;
		G.SaveData.Item_work[1].Id   = G.SaveData.Item_work[0].Id;
		G.SaveData.Item_work[1].Num  = G.SaveData.Item_work[0].Num;
		G.SaveData.Item_boxwork[sel].Id  = bk_id;
		G.SaveData.Item_boxwork[sel].Num = bk_num;
		G.SaveData.Item_boxwork[sel].Size = bk_sz;
		{
			ULONG pid = (ULONG)G.SaveData.Item_boxwork[sel].Id;
			Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
		}
		Pix_trans(0, (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_work[0].Id * 0x4b0));
		{
			ULONG set_no   = 1;
			ULONG *data_addr = (ULONG *)(St_item_img + (G.SaveData.Item_work[0].Id + 0x56) * 0x4b0);
			Pix_trans(set_no, data_addr);
		}
		return;
	}

	if (type == 0) {
		/* Both empty — simple slot swap */
		if (pSt->Equip_item_no == (UCHAR)main_idx) {
			pSt->Equip_item_no = 0x80;
			pSt->Equip_item_id = 0;
		}
		{
			ULONG empty = Search_item(0);
			if (empty < main_idx) main_idx = empty;
		}
		G.SaveData.Item_work[main_idx].Id   = G.SaveData.Item_boxwork[sel].Id;
		G.SaveData.Item_work[main_idx].Num  = G.SaveData.Item_boxwork[sel].Num;
		G.SaveData.Item_work[main_idx].Size = G.SaveData.Item_boxwork[sel].Size;
		G.SaveData.Item_boxwork[sel].Id  = bk_id;
		G.SaveData.Item_boxwork[sel].Num = bk_num;
		G.SaveData.Item_boxwork[sel].Size = bk_sz;
		{
			ULONG pid = (ULONG)G.SaveData.Item_boxwork[sel].Id;
			Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
			Pix_trans(main_idx / 2 + main_idx,
					  (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_work[main_idx].Id * 0x4b0));
		}
		Sort_item();
		return;
	}

	if (type == 2) {
		/* Player has item, box empty — handle wide items */
		UCHAR sz = G.SaveData.Item_work[main_idx].Size;
		if (sz == 2) {
			if ((pSt->Equip_item_no == (UCHAR)main_idx) || ((ULONG)pSt->Equip_item_no == main_idx - 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			G.SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
			G.SaveData.Item_work[main_idx - 1].Id = 0;
			Sort_item();
		} else if (sz == 1) {
			if ((pSt->Equip_item_no == (UCHAR)main_idx) || ((ULONG)pSt->Equip_item_no == main_idx + 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			G.SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
			G.SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
		}
		{
			LONG lspace = Search_item(0);
			G.SaveData.Item_work[lspace].Id   = G.SaveData.Item_boxwork[sel].Id;
			G.SaveData.Item_work[lspace].Num  = G.SaveData.Item_boxwork[sel].Num;
			G.SaveData.Item_work[lspace].Size = G.SaveData.Item_boxwork[sel].Size;
			G.SaveData.Item_boxwork[sel].Id   = bk_id;
			G.SaveData.Item_boxwork[sel].Size = 3;
			G.SaveData.Item_boxwork[sel].Num  = bk_num;
			{
				ULONG pid = (ULONG)G.SaveData.Item_boxwork[sel].Id;
				ULONG set_no = lspace / 2 + lspace;
				ULONG *data_addr = (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_work[lspace].Id * 0x4b0);
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans(set_no, data_addr);
			}
		}
		return;
	}

	if (type == 3) {
		/* Both have wide items */
		if (G.SaveData.Item_work[main_idx].Size == 1) {
			UCHAR bxid = G.SaveData.Item_boxwork[sel].Id;
			UCHAR bxnum = G.SaveData.Item_boxwork[sel].Num;
			if ((pSt->Equip_item_no == (UCHAR)main_idx) || ((ULONG)pSt->Equip_item_no == main_idx + 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			G.SaveData.Item_work[main_idx + 1].Id  = bxid;
			G.SaveData.Item_work[main_idx].Id      = bxid;
			G.SaveData.Item_work[main_idx + 1].Num = bxnum;
			G.SaveData.Item_work[main_idx].Num     = bxnum;
			G.SaveData.Item_work[main_idx].Size     = 1;
			G.SaveData.Item_work[main_idx + 1].Size = 2;
			G.SaveData.Item_boxwork[sel].Id   = bk_id;
			G.SaveData.Item_boxwork[sel].Size = 3;
			G.SaveData.Item_boxwork[sel].Num  = bk_num;
			{
				ULONG pid = (ULONG)G.SaveData.Item_boxwork[sel].Id;
				ULONG set_no2 = ((main_idx + 1) >> 1) + main_idx + 1;
				ULONG *data_addr2 = (ULONG *)(St_item_img + (G.SaveData.Item_work[main_idx].Id + 0x56) * 0x4b0);
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans((main_idx >> 1) + main_idx,
						  (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_work[main_idx].Id * 0x4b0));
				Pix_trans(set_no2, data_addr2);
			}
		} else {
			UCHAR bxid = G.SaveData.Item_boxwork[sel].Id;
			UCHAR bxnum = G.SaveData.Item_boxwork[sel].Num;
			if ((pSt->Equip_item_no == (UCHAR)main_idx) || ((ULONG)pSt->Equip_item_no == main_idx - 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			G.SaveData.Item_work[main_idx - 1].Id = bxid;
			G.SaveData.Item_work[main_idx].Id  = bxid;
			G.SaveData.Item_work[main_idx - 1].Num = bxnum;
			G.SaveData.Item_work[main_idx].Num  = bxnum;
			G.SaveData.Item_work[main_idx].Size = 2;
			G.SaveData.Item_work[main_idx - 1].Size = 1;
			G.SaveData.Item_boxwork[sel].Id   = bk_id;
			G.SaveData.Item_boxwork[sel].Size = 3;
			G.SaveData.Item_boxwork[sel].Num  = bk_num;
			{
				ULONG pid = (ULONG)G.SaveData.Item_boxwork[sel].Id;
				ULONG set_no3 = ((main_idx - 1) >> 1) + main_idx - 1;
				ULONG *data_addr3 = (ULONG *)(St_item_img + (ULONG)G.SaveData.Item_work[main_idx].Id * 0x4b0);
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans((main_idx >> 1) + main_idx,
						  (ULONG *)(St_item_img + (G.SaveData.Item_work[main_idx].Id + 0x56) * 0x4b0));
				Pix_trans(set_no3, data_addr3);
			}
		}
	}
}

/* DS 0x80070e58 (St_init_itembox) */
VOID St_init_itembox(VOID)
{
	/* DS 0x800a9bc4: 25 x {u0, v0, w, h} sprite rects */
	static UCHAR Ibox_char_tbl[25][4] = {
		{0, 248, 128, 4}, {0, 252, 128, 4}, {0, 240, 77, 4}, {0, 244, 77, 4}, {12, 60, 3, 153},
		{15, 60, 3, 153}, {0, 228, 39, 12}, {0, 0, 128, 20}, {0, 20, 128, 20}, {0, 40, 62, 20}, {62, 40, 62, 20},
		{8, 60, 2, 142}, {10, 60, 2, 142}, {0, 60, 8, 130}, {0, 202, 8, 6}, {0, 214, 8, 6}, {0, 190, 6, 2},
		{0, 190, 6, 2}, {0, 190, 6, 2}, {0, 190, 6, 2}, {0, 190, 6, 2}, {20, 78, 20, 20}, {20, 116, 20, 20},
		{0, 164, 55, 12}, {144, 46, 47, 10}
	};
	POLY_FT4 *pft4;
	register POLY_F4 *pf4;
	auto LINE_F2 *lf2;
	register SPRT *sp;
	register ULONG clut;
	register ULONG i;
	register ULONG j;

	pft4 = St_prim_a;
	pf4  = St_prim_b;
	sp   = St_prim_d;
	lf2  = St_prim_c;
	clut = GetClut(0, 0x1e7);

	/* Init item box icon sprites */
	for (i = 24; ; i--) {
		for (j = 2; j != 0; j--) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Ibox_char_tbl[i][0];
			sp->v0 = Ibox_char_tbl[i][1];
			sp->w  = (USHORT)Ibox_char_tbl[i][2];
			sp->h  = (USHORT)Ibox_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
		if (i == 0) break;
	}

	/* Init item box poly-FT4 cells */
	{
		UCHAR v0 = 0x0e;
		UCHAR v1 = 0x0e;
		for (i = 6; i != 0; i--) {
			v0 += 0xe2;
			for (j = 2; j != 0; j--) {
				SetPolyFT4(pft4);
				pft4->u0 = 0x28; pft4->v0 = v0;
				pft4->u1 = 0x50; pft4->v1 = v0;
				pft4->u2 = 0x28; pft4->v2 = v1;
				pft4->u3 = 0x50; pft4->v3 = v1;
				pft4->tpage = GetTPage(1, 0, 0x1c0, 0x100);
				pft4->clut  = GetClut(0, 0x1e5);
				pft4->r0 = pft4->g0 = pft4->b0 = 0x80;
				pft4->code &= 0xfd;
				pft4++;
			}
			v1 += 0xe2;
		}
	}

	/* Init background poly-F4 cells */
	for (i = 4; i != 0; i--) {
		for (j = 2; j != 0; j--) {
			SetPolyF4(pf4);
			pf4->r0 = 0x20;
			pf4->g0 = 0x20;
			pf4->b0 = 0x40;
			pf4->code |= 0x02;
			pf4++;
		}
	}

	/* Init line primitives */
	for (i = 6; i != 0; i--) {
		setLineF2(lf2);
		lf2->r0 = 0x80;
		lf2->g0 = 0x20;
		lf2->b0 = 0x20;
		lf2++;
	}

	St.Ibox_pos_x = 7;
	St.Ibox_pos_y = 0xe;
}

/* DS 0x800a9c28: 4 x {y offset, r, g, b} for the POLY_F4 rows */
static UCHAR St_ibox_f4_tbl[4][4] = {{26, 48, 48, 48}, {46, 48, 48, 48}, {87, 48, 48, 48}, {107, 48, 48, 48}};
/* DS 0x800a9c38: 25 x {x, y} item box positions */
static SHORT St_ibox_pos_tbl[25][2] = {
	{3, 0}, {3, 149}, {131, 0}, {131, 149}, {0, 0}, {208, 0}, {211, 0}, {6, 6}, {6, 127}, {134, 6},
	{134, 127}, {4, 6}, {196, 6}, {199, 11}, {199, 4}, {199, 142}, {200, 12}, {200, 12}, {200, 12},
	{200, 12}, {200, 12}, {94, 5}, {94, 127}, {248, 0}, {252, 1}
};

/* DS 0x800710dc (St_disp_itembox) */
VOID St_disp_itembox(VOID)
{
	register POLY_FT4 *pft4;
	register POLY_F4 *pf4;
	auto ULONG *pOt;
	auto ULONG *pOt2;
	register ULONG i;
	register UCHAR *ty;
	register GLOBAL *pG;
	SPRT *sp_base;
	register SPRT *sp;
	register LINE_F2 *lf2;
	ULONG   Frame_pad[2];		/* retail never reads the upper halves of its 8-byte spill slots (sp+0x14, 0x1c, ...) */

	SHORT ibox_y = St.Ibox_pos_y;
	SHORT ibox_x = St.Ibox_pos_x;
	ULONG cbuf   = (ULONG)G.Cbuf_id;
	(void)Frame_pad;

	pft4 = St_prim_a;
	pf4  = St_prim_b;
	pG = &G;
	sp_base = St_prim_d;
	lf2 = St_prim_c;

	pOt  = G.Ot0[cbuf] + 2;
	pOt2 = G.Ot0[cbuf] + 6;

	if (cbuf != 0) {
		sp_base  = &St_prim_d[1];
		pft4     = St_prim_a + 1;
		lf2 = &St_prim_c[1];
		pf4      = St_prim_b + 1;
	}

	/* Animate shimmer */
	if (sw == 0) {
		Move_x[0] += 2;
	} else {
		Move_x[0] -= 2;
	}
	if (0x3e < Move_x[0]) { Move_x[0] = 0x3f; sw = 1; }
	if (Move_x[0] < 1)    { Move_x[0] = 0;    sw = 0; }

	/* Render box icon rows: 4 POLY_F4 cells at pf4, one per f4_tbl row (3 down to 0) */
	for (i = 3, ty = St_ibox_f4_tbl[3]; (LONG)i >= 0; i--, ty = ty - 4) {
		pf4->x0 = ibox_x + 6;
		pf4->y0 = *ty + ibox_y;
		pf4->x1 = ibox_x + 0xc5;
		pf4->y1 = *ty + ibox_y;
		pf4->x2 = ibox_x + 6;
		pf4->y2 = *ty + ibox_y + 0x14;
		pf4->x3 = ibox_x + 0xc5;
		pf4->y3 = *ty + ibox_y + 0x14;
		pf4->r0 = St_ibox_f4_tbl[i][1];
		pf4->g0 = St_ibox_f4_tbl[i][2];
		pf4->b0 = St_ibox_f4_tbl[i][3];
		AddPrim(pOt, pf4);
		pf4 += 2;
	}
	/* Render cursor/selected box sprites; sp walks the SPRT pairs (2 buffers interleaved) like DS */
	sp = sp_base;
	sp->x0 = St_ibox_pos_tbl[0x18][0] + ibox_x;
	sp->y0 = St_ibox_pos_tbl[0x18][1] + ibox_y;
	sp->clut = GetClut(0x100, 0x1f0);
	{
		UCHAR bri = (St.Sub_rtn0 == 2) ? 0x80 : 0x40;
		sp->r0 = bri; sp->g0 = bri; sp->b0 = bri;
	}
	AddPrim(pOt, sp);
	AddPrim(pOt, St.Dmode[0xe][G.Cbuf_id].code + 1);
	sp += 2;

	sp->x0 = St_ibox_pos_tbl[0x17][0] + ibox_x;
	sp->y0 = St_ibox_pos_tbl[0x17][1] + ibox_y;
	sp->clut = GetClut(0, 0x1e4);
	AddPrim(pOt, sp);
	AddPrim(pOt, St.Dmode[4][G.Cbuf_id].code + 1);
	sp += 2;

	/* Down arrow */
	sp->x0 = St_ibox_pos_tbl[0x16][0] + ibox_x;
	sp->y0 = St_ibox_pos_tbl[0x16][1] + ibox_y;
	if (((G.Padd1 & 0x4008) == 0) || (St.Sub_rtn1 == 0)) {
		sp->u0 = 0x12; sp->v0 = 0x78;
	} else {
		sp->u0 = 0x12; sp->v0 = 100;
	}
	AddPrim(pOt, sp);
	sp += 2;

	/* Up arrow */
	sp->x0 = St_ibox_pos_tbl[0x15][0] + ibox_x;
	sp->y0 = St_ibox_pos_tbl[0x15][1] + ibox_y;
	if (((G.Padd1 & 0x1004) == 0) || (St.Sub_rtn1 == 0)) {
		sp->u0 = 0x12; sp->v0 = 0x50;
	} else {
		sp->u0 = 0x12; sp->v0 = 0x3c;
	}
	AddPrim(pOt, sp);
	sp += 2;

	/* Item slot sprites: 5 SPRT cells, Ibox_pos_tbl row 16+k, slot (Ibox_sel + 4 - k) & 0x3f */
	for (i = 4; (LONG)i >= 0; i--) {
		sp->x0 = St_ibox_pos_tbl[0x14 - i][0] + ibox_x;
		sp->y0 = St_ibox_pos_tbl[0x14 - i][1] + ibox_y +
							  (SHORT)((((ULONG)St.Ibox_sel + i) & 0x3f) * 2);
		AddPrim(pOt, sp);
		sp += 2;
	}

	/* Background cells: 16 SPRT cells, Ibox_pos_tbl row 15-k */
	for (i = 15; (LONG)i >= 0; i--) {
		sp->x0 = St_ibox_pos_tbl[i][0] + ibox_x;
		sp->y0 = St_ibox_pos_tbl[i][1] + ibox_y;
		AddPrim(pOt, sp);
		sp += 2;
	}
	AddPrim(pOt, St.Dmode[0x17][G.Cbuf_id].code + 1);

	/* Scroll highlight for box scroll animation */
	{
		ULONG sub1 = (ULONG)St.Sub_rtn1;
		if (sub1 - 2 < 2) {
			SHORT y_off;
			SHORT wd;
			LONG adj_slot;
			UCHAR col;
			if (sub1 == 2) { y_off = -0x14; adj_slot = -1; }
			else           { y_off = 100;    adj_slot = 5; }
			wd = (pG->SaveData.Item_boxwork[((ULONG)St.Ibox_sel + adj_slot) & 0x3f].Size == 0) ? 0x18 : 0x30;
			col = (pG->SaveData.Item_boxwork[((ULONG)St.Ibox_sel + adj_slot) & 0x3f].Size == 0) ? 0x50 : 0x78;
			/* Setup & add poly-FT4 primitive at pft4 */
			pft4->v0 = 0x3c;	/* v0 */
			pft4->v1 = 0x3c;	/* v1 */
			pft4->u0 = 0x28;	/* u0 */
			pft4->u1 = col;	/* u1 */
			pft4->u2 = 0x28;	/* u2 */
			pft4->v2 = 0x5a;	/* v2 */
			pft4->u3 = col;	/* u3 */
			pft4->v3 = 0x5a;	/* v3 */
			pft4->x0 = St.Ibox_pos_x + 0x92;
			pft4->y0 = (SHORT)St.Ibox_moxe + (St.Ibox_pos_y + (y_off + 0x1b));
			pft4->x1 = St.Ibox_pos_x + wd + 0x93;
			pft4->y1 = (SHORT)St.Ibox_moxe + (St.Ibox_pos_y + (y_off + 0x1b));
			pft4->x2 = St.Ibox_pos_x + 0x92;
			pft4->y2 = (SHORT)St.Ibox_moxe + (St.Ibox_pos_y + (y_off + 0x2e));
			pft4->x3 = St.Ibox_pos_x + wd + 0x93;
			pft4->y3 = (SHORT)St.Ibox_moxe + (St.Ibox_pos_y + (y_off + 0x2e));
			addPrim(pOt2, pft4);
		}
	}
	pft4 += 2;

	/* Render visible box items with names: 5 POLY_FT4 cells, slot (Ibox_sel + k) & 0x3f, k = 4 down to 0 */
	for (i = 4; (LONG)i >= 0; i--) {
		ULONG slot = ((ULONG)St.Ibox_sel + i) & 0x3f;
		SHORT wd = (pG->SaveData.Item_boxwork[slot].Size == 0) ? 0x18 : 0x30;
		UCHAR col = (pG->SaveData.Item_boxwork[slot].Size == 0) ? 0x50 : 0x78;
		ULONG id = (ULONG)pG->SaveData.Item_boxwork[slot].Id;
		if (id == 0) id = 100;

		pft4->u0 = 0x28;		/* u0 */
		pft4->v0 = (UCHAR)(0x5a + 0x1e * i);		/* v0 */
		pft4->u1 = col;		/* u1 */
		pft4->v1 = (UCHAR)(0x5a + 0x1e * i);		/* v1 */
		pft4->u2 = 0x28;		/* u2 */
		pft4->v2 = (UCHAR)(0x78 + 0x1e * i);		/* v2 */
		pft4->u3 = col;		/* u3 */
		pft4->v3 = (UCHAR)(0x78 + 0x1e * i);		/* v3 */
		pft4->x0 = St.Ibox_pos_x + 0x92;
		pft4->y0 = (SHORT)St.Ibox_moxe + (ibox_y + (0x1b + 0x14 * i));
		pft4->x1 = St.Ibox_pos_x + wd + 0x93;
		pft4->y1 = (SHORT)St.Ibox_moxe + (ibox_y + (0x1b + 0x14 * i));
		pft4->x2 = St.Ibox_pos_x + 0x92;
		pft4->y2 = (SHORT)St.Ibox_moxe + (ibox_y + (0x2e + 0x14 * i));
		pft4->y3 = (SHORT)St.Ibox_moxe + (ibox_y + (0x2e + 0x14 * i));
		pft4->x3 = St.Ibox_pos_x + wd + 0x93;
		addPrim(pOt2, pft4);
		pft4 += 2;
		St_disp_name(St.Ibox_pos_x + 7, (0x14 * i) + ibox_y + 0x1d + (SHORT)St.Ibox_moxe, 6, id);
	}

	/* Separator lines */
	lf2->x0 = ibox_x + 6;
	lf2->x1 = ibox_x + 0xc4;
	lf2->y0 = (SHORT)St.Ibox_moxe + ibox_y +
							 (0x3f - ((St.Ibox_sel - 4) & 0x3f)) * 0x14 + 7;
	lf2->y1 = (SHORT)St.Ibox_moxe + ibox_y +
							 (0x3f - ((St.Ibox_sel - 4) & 0x3f)) * 0x14 + 7;
	{
		UCHAR s2 = (St.Ibox_sel + 2) & 0x3f;
		if (s2 < 5 || s2 == 0x3f) {
			AddPrim(pOt, lf2);
		}
	}

	/* Two highlight bars */
	{
		SHORT ly = ibox_y + 0x6a;
		lf2 += 2;
		for (i = 2; i > 0; ) {
			i--;
			ly -= 0x14;
			lf2->y0 = ly;
			lf2->y1 = ly;
			lf2->x0 = ibox_x + 6;
			lf2->x1 = ibox_x + 0xc4;
			if (St.Sub_rtn1 == 0) {
				lf2->r0 = 0x30; lf2->g0 = 0x20; lf2->b0 = 8;
			} else {
				lf2->r0 = (UCHAR)Move_x[0] + 0x50;
				lf2->g0 = (UCHAR)Move_x[0] + 0x40;
				lf2->b0 = (UCHAR)Move_x[0] & 0x1f;
			}
			AddPrim(pOt, lf2);
			lf2 += 2;
		}
	}
}
