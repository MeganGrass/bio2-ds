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

/* St_prim_a (DS 0x80198000), St_prim_b (0x80198800), St_prim_c (0x80199000) and St_prim_d (0x80199200)
 * are fixed-RAM screen buffers; the functions below address them by their DS addresses. */

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
	SetDrawMode((DR_MODE *)&pSt->Dmode[0x14][0], 0, 0, (G.Ob_model[St.Itembox_tpage].Tpage & 0xf) | 0x80, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&pSt->Dmode[0x14][1], 0, 0, (G.Ob_model[St.Itembox_tpage].Tpage & 0xf) | 0x80, (RECT *)0x0);
	Pix_trans_pl(7, (ULONG)pG->SaveData.Item_boxwork[pSt->Ibox_sel].Id, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_boxwork[pSt->Ibox_sel].Id * 0x4b0));
	Move_x[0] = 1;
	sw = 0;
	{
		ULONG k = 5;
		/* Slots 22, 19, 16, 13 and 10 take the five box items before Ibox_sel (k = 4 down to 0); i steps down by 3 at the top of each pass, as DS does (s2 starts at 0x19). */
		i = 0x19;
		do {
			ULONG id;
			i -= 3;
			k--;
			id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + k) & 0x3f].Id;
			Pix_trans_pl(i, id, (ULONG *)(St_item_img + id * 0x4b0));
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

/* Item box screen, one frame: run the proc for Sub_rtn0 (fade out, fade wait, menu select, item select), then
 * draw the item box, the item list, the count, the equipped item and the message line. */
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

/* Menu step of the item box: the 0x4000 pad button moves into the item list (Sub_rtn0 advances), and a
 * cancel key (0x3000) returns to Sub_rtn0 = 0. */
/* DS 0x8006fbd4 (St_select_menu_m) */
VOID St_select_menu_m(VOID)
{
	if ((G.Padd1 & 0x4000) != 0) {
		UCHAR *pRtn = &St.Sub_rtn0;

		St.Icursol_main = 1;
		(*pRtn)++;
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
	register LONG i;
	ULONG nsel;
	ULONG id;

	pSt = &St;
	pG = &G;

	switch (pSt->Sub_rtn1) {
	case 0:
		c_old = (ULONG)pSt->Icursol_main;
		if ((int)G.System_flg < 0) {
			if (((pG->Padd1 & 0x2000) != 0) && ((int)c_old < (int)(pG->SaveData.Max_item_num - 1))) {
				if (pG->SaveData.Item_work[c_old].Size == 1) {
					pSt->Icursol_main++;
				}
				pSt->Icursol_main++;
			}
			if (((pG->Padd1 & 0x8000) != 0) && (ULONG)pSt->Icursol_main != 0 && (ULONG)pSt->Icursol_main != 10) {
				if (pG->SaveData.Item_work[(ULONG)pSt->Icursol_main].Size == 2) {
					UCHAR tmp = pSt->Icursol_main - 1;
					if ((ULONG)pSt->Icursol_main == 1) {
						tmp = pSt->Icursol_main + 1;
					}
					pSt->Icursol_main = tmp;
				}
				pSt->Icursol_main--;
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				if ((int)(ULONG)pSt->Icursol_main < (int)(pG->SaveData.Max_item_num - 2)) {
					pSt->Icursol_main += 2;
				} else if ((ULONG)pSt->Icursol_main == 10) {
					pSt->Icursol_main = 1;
				}
			}
			if ((pG->Padd1 & 0x1000) != 0) {
				if ((ULONG)pSt->Icursol_main >= 2 && (ULONG)pSt->Icursol_main != 10) {
					pSt->Icursol_main -= 2;
				} else {
					c_old = 10;
					pSt->Sub_rtn0--;
				}
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
		} else if ((G.Key_trg & 0x4000) != 0 && St.Exit_flg == 1) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			St.Sub_rtn0 = 1;
		}
		break;

	case 1:
		if ((int)G.System_flg < 0) {
			if ((pG->Padd1 & 0x1000) != 0) {
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 2;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
				break;
			}
			if ((pG->Padd1 & 0x4000) != 0) {
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
				pSt->Sub_rtn1 = 3;
				id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
				Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
				break;
			}
			/* Scrolling by five slots (Padd1 4 and 8) continues in case 2 at no_name_disp */
			if ((pG->Padd1 & 4) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				nsel = pSt->Ibox_sel - 5;
				goto no_name_disp;
			}
			if ((pG->Padd1 & 8) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				nsel = pSt->Ibox_sel + 5;
				goto no_name_disp;
			}
		}
		if ((pG->Key_trg & 0x2000) != 0) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			pSt->Sub_rtn1 = 0;
		} else if ((pG->Key_trg & 0x1000) != 0) {
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
			pSt->Sub_rtn1 = 4;
		}
		break;

	case 2:
		pSt->Exit_flg = 0;
		if (pSt->Sub_rtn2++ >= 6) {
			if ((pG->Padd1 & 0x1000) == 0) {
				pSt->Sub_rtn1 = 1;
			} else {
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
			}
			pSt->Ibox_moxe = 0;
			pSt->Sub_rtn2 = 0;
			nsel = pSt->Ibox_sel - 1;
no_name_disp:
			pSt->Ibox_sel = nsel & 0x3f;
			id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
			Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
			for (i = 0; i < 5; i++) {
				id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + i) & 0x3f].Id;
				Pix_trans_pl(10 + i * 3, id, (ULONG *)(St_item_img + id * 0x4b0));
			}
		} else {
			pSt->Ibox_moxe += 3;
		}
		{
			ULONG box_id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel - 1) & 0x3f].Id;
			if (box_id != 0) {
				St_disp_name(pSt->Ibox_pos_x + 7, (signed char)St.Ibox_moxe + (pSt->Ibox_pos_y + 9), 6, box_id);
			} else {
				St_disp_name(pSt->Ibox_pos_x + 7, (signed char)St.Ibox_moxe + (pSt->Ibox_pos_y + 9), 6, 100);
			}
		}
		break;

	case 3:
		pSt->Exit_flg = 0;
		if (pSt->Sub_rtn2++ >= 6) {
			if ((pG->Padd1 & 0x4000) == 0) {
				pSt->Sub_rtn1 = 1;
			} else {
				Snd_se_stad(0x2140000, (VECTOR *)0x0);
			}
			pSt->Ibox_moxe = 0;
			pSt->Sub_rtn2 = 0;
			pSt->Ibox_sel = (pSt->Ibox_sel + 1) & 0x3f;
			id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
			Pix_trans_pl(7, id, (ULONG *)(St_item_img + id * 0x4b0));
			for (i = 0; i < 5; i++) {
				id = (ULONG)pG->SaveData.Item_boxwork[((ULONG)pSt->Ibox_sel + i) & 0x3f].Id;
				Pix_trans_pl(10 + i * 3, id, (ULONG *)(St_item_img + id * 0x4b0));
			}
		} else {
			pSt->Ibox_moxe -= 3;
		}
		{
			ULONG box_id = (ULONG)pG->SaveData.Item_boxwork[(pSt->Ibox_sel + 5) & 0x3f].Id;
			if (box_id == 0) {
				St_disp_name(pSt->Ibox_pos_x + 7, (signed char)St.Ibox_moxe + (pSt->Ibox_pos_y + 0x81), 6, 100);
			} else {
				St_disp_name(pSt->Ibox_pos_x + 7, (signed char)St.Ibox_moxe + (pSt->Ibox_pos_y + 0x81), 6, box_id);
			}
		}
		break;

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

/* Move the item under the cursor to or from the item box. The cursor slot is copied on entry (bk_*). Stackable
 * items (ids 0x14..0x1f: ammunition, the ink ribbon 0x1e and the small key 0x1f; retail tests id - 0x14 < 0xc) merge into a matching stack and are capped at Item_max, with any excess
 * left in the box slot; other items swap places with the box slot. */
/* DS 0x800703b8 (Exchange_item) */
VOID Exchange_item(VOID)
{
	register STATUS *pSt;
	register GLOBAL *pG;
	/* copies of the cursor slot taken on entry (DS keeps them in stack slots) */
	ITEM_WORK bk_work;
	UCHAR box_num;
	ULONG box_ext;		/* box_num widened: DS keeps the byte (stores) and the widened copy (arithmetic) in two registers */
	register LONG space_num;
	register ULONG sel;
	register ULONG main_idx;
	register ULONG type;
	register ULONG sel_id;
	register ULONG i;

	pSt = &St;
	pG = &G;

	main_idx = (ULONG)pSt->Icursol_main;
	bk_work.Id  = pG->SaveData.Item_work[main_idx].Id;
	bk_work.Num = pG->SaveData.Item_work[main_idx].Num;
	bk_work.Size  = pG->SaveData.Item_work[main_idx].Size;
	sel = (pSt->Ibox_sel + 2) & 0x3f;

	type = (pG->SaveData.Item_work[main_idx].Size != 0) ? 2 : 0;
	if (pG->SaveData.Item_boxwork[sel].Size != 0) {
		type++;
	}

	box_num = pG->SaveData.Item_boxwork[sel].Num;
	box_ext = box_num;
	sel_id = (ULONG)pG->SaveData.Item_boxwork[sel].Id;

	/* Stacking ammo case */
	if (sel_id - 0x14 < 0xc) {
		ULONG wk_id = (ULONG)pG->SaveData.Item_work[main_idx].Id;
		if (sel_id == wk_id) {
			UCHAR imax = Item_data_tbl[sel_id].Item_max;
			ULONG wk_num = (ULONG)pG->SaveData.Item_work[main_idx].Num;
			ULONG tot_num;
			if (wk_num == imax) {
				pG->SaveData.Item_work[main_idx].Num = box_num;
				pG->SaveData.Item_boxwork[sel].Num = imax;
				return;
			}
			tot_num = wk_num + box_ext;
			if (imax < tot_num) {
				pG->SaveData.Item_work[main_idx].Num = imax;
				pG->SaveData.Item_boxwork[sel].Num = (UCHAR)tot_num - Item_data_tbl[sel_id].Item_max;
				return;
			}
			goto num_add;
		}
		if (wk_id == 0) {
			/* DS loop is do-while: slot 0 is always tested, even when Max_item_num is 0 */
			i = 0;
			do {
				if ((sel_id == (ULONG)pG->SaveData.Item_work[i].Id) && ((ULONG)pG->SaveData.Item_work[i].Num <= (ULONG)Item_data_tbl[sel_id].Item_max - box_ext)) {
					main_idx = i;
					/* num_add is also reached from the exact-fit case above; DS lays this code out after the search loop test */
num_add:
					pG->SaveData.Item_work[main_idx].Num += box_ext;
					pG->SaveData.Item_boxwork[sel].Id  = 0;
					pG->SaveData.Item_boxwork[sel].Num = 0;
					pG->SaveData.Item_boxwork[sel].Size = 0;
					Pix_trans_pl(0x10, (ULONG)pG->SaveData.Item_boxwork[sel].Id, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_boxwork[sel].Id * 0x4b0));
					return;
				}
				i++;
			} while (i < pG->SaveData.Max_item_num);
		}
	}

	switch (type) {
	case 0: {
		/* Both empty — simple slot swap */
		if (pSt->Equip_item_no == main_idx) {
			pSt->Equip_item_no = 0x80;
			pSt->Equip_item_id = 0;
		}
		{
			space_num = Search_item(0);
			if (space_num < main_idx) main_idx = space_num;
		}
		pG->SaveData.Item_work[main_idx].Id   = pG->SaveData.Item_boxwork[sel].Id;
		pG->SaveData.Item_work[main_idx].Num  = pG->SaveData.Item_boxwork[sel].Num;
		pG->SaveData.Item_work[main_idx].Size = pG->SaveData.Item_boxwork[sel].Size;
		pG->SaveData.Item_boxwork[sel].Id  = bk_work.Id;
		pG->SaveData.Item_boxwork[sel].Num = bk_work.Num;
		pG->SaveData.Item_boxwork[sel].Size = bk_work.Size;
		{
			ULONG pid = (ULONG)pG->SaveData.Item_boxwork[sel].Id;
			Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
			Pix_trans(main_idx / 2 + main_idx, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_work[main_idx].Id * 0x4b0));
		}
		Sort_item();
		return;
	}
	case 1: {
		/* Box item → empty player slot */
		space_num = Search_item(1);
		if ((space_num == 1) && (pG->SaveData.Item_work[main_idx].Id == 0)) {
			/* "no room" message: also reached when Search_item finds no free slot (space_num 0) */
no_room:
			Mess_set(0xaf0010, 0xe400, 8, 0);
			pSt->Sub_rtn1 = 5;
			return;
		}
		if (space_num == 0) {
			goto no_room;
		}
		if (pSt->Equip_item_no == main_idx) {
			pSt->Equip_item_no = 0x80;
			pSt->Equip_item_id = 0;
		}
		pG->SaveData.Item_work[main_idx].Id = 0;
		Sort_item();
		Shift_item(1);
		pG->SaveData.Item_work[0].Id = pG->SaveData.Item_work[1].Id = pG->SaveData.Item_boxwork[sel].Id;
		pG->SaveData.Item_work[0].Num = pG->SaveData.Item_work[1].Num = pG->SaveData.Item_boxwork[sel].Num;
		pG->SaveData.Item_work[0].Size = 1;
		pG->SaveData.Item_work[1].Size = 2;
		pG->SaveData.Item_boxwork[sel].Id  = bk_work.Id;
		pG->SaveData.Item_boxwork[sel].Num = bk_work.Num;
		pG->SaveData.Item_boxwork[sel].Size = bk_work.Size;
		{
			ULONG pid = (ULONG)pG->SaveData.Item_boxwork[sel].Id;
			Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
		}
		Pix_trans(0, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_work[0].Id * 0x4b0));
		Pix_trans(1, (ULONG *)(St_item_img + (pG->SaveData.Item_work[0].Id + 0x56) * 0x4b0));
		return;
	}
	case 2: {
		/* Player has item, box empty — handle wide items */
		UCHAR sz = pG->SaveData.Item_work[main_idx].Size;
		if (sz == 2) {
			if ((pSt->Equip_item_no == main_idx) || ((ULONG)pSt->Equip_item_no == main_idx - 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			pG->SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
			pG->SaveData.Item_work[main_idx - 1].Id = 0;
			Sort_item();
		} else if (sz == 1) {
			if ((pSt->Equip_item_no == main_idx) || ((ULONG)pSt->Equip_item_no == main_idx + 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			pG->SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
			pG->SaveData.Item_work[main_idx].Id = 0;
			Sort_item();
		}
		{
			space_num = Search_item(0);
			pG->SaveData.Item_work[space_num].Id   = pG->SaveData.Item_boxwork[sel].Id;
			pG->SaveData.Item_work[space_num].Num  = pG->SaveData.Item_boxwork[sel].Num;
			pG->SaveData.Item_work[space_num].Size = pG->SaveData.Item_boxwork[sel].Size;
			pG->SaveData.Item_boxwork[sel].Id   = bk_work.Id;
			pG->SaveData.Item_boxwork[sel].Num  = bk_work.Num;
			pG->SaveData.Item_boxwork[sel].Size = 3;
			{
				ULONG pid = (ULONG)pG->SaveData.Item_boxwork[sel].Id;
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans(space_num / 2 + space_num, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_work[space_num].Id * 0x4b0));
			}
		}
		return;
	}
	case 3: {
		/* Both have wide items */
		if (pG->SaveData.Item_work[main_idx].Size == 1) {
			if ((pSt->Equip_item_no == main_idx) || ((ULONG)pSt->Equip_item_no == main_idx + 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			pG->SaveData.Item_work[main_idx].Id  = pG->SaveData.Item_work[main_idx + 1].Id  = pG->SaveData.Item_boxwork[sel].Id;
			pG->SaveData.Item_work[main_idx].Num = pG->SaveData.Item_work[main_idx + 1].Num = pG->SaveData.Item_boxwork[sel].Num;
			pG->SaveData.Item_work[main_idx].Size     = 1;
			pG->SaveData.Item_work[main_idx + 1].Size = 2;
			pG->SaveData.Item_boxwork[sel].Id   = bk_work.Id;
			pG->SaveData.Item_boxwork[sel].Num  = bk_work.Num;
			pG->SaveData.Item_boxwork[sel].Size = 3;
			{
				ULONG pid = (ULONG)pG->SaveData.Item_boxwork[sel].Id;
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans((main_idx >> 1) + main_idx, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_work[main_idx].Id * 0x4b0));
				Pix_trans(((main_idx + 1) >> 1) + main_idx + 1, (ULONG *)(St_item_img + (pG->SaveData.Item_work[main_idx].Id + 0x56) * 0x4b0));
			}
		} else {
			if ((pSt->Equip_item_no == main_idx) || ((ULONG)pSt->Equip_item_no == main_idx - 1)) {
				pSt->Equip_item_no = 0x80;
				pSt->Equip_item_id = 0;
			}
			pG->SaveData.Item_work[main_idx].Id  = pG->SaveData.Item_work[main_idx - 1].Id  = pG->SaveData.Item_boxwork[sel].Id;
			pG->SaveData.Item_work[main_idx].Num = pG->SaveData.Item_work[main_idx - 1].Num = pG->SaveData.Item_boxwork[sel].Num;
			pG->SaveData.Item_work[main_idx].Size = 2;
			pG->SaveData.Item_work[main_idx - 1].Size = 1;
			pG->SaveData.Item_boxwork[sel].Id   = bk_work.Id;
			pG->SaveData.Item_boxwork[sel].Num  = bk_work.Num;
			pG->SaveData.Item_boxwork[sel].Size = 3;
			{
				ULONG pid = (ULONG)pG->SaveData.Item_boxwork[sel].Id;
				Pix_trans_pl(0x10, pid, (ULONG *)(St_item_img + pid * 0x4b0));
				Pix_trans((main_idx >> 1) + main_idx, (ULONG *)(St_item_img + (pG->SaveData.Item_work[main_idx].Id + 0x56) * 0x4b0));
				Pix_trans(((main_idx - 1) >> 1) + main_idx - 1, (ULONG *)(St_item_img + (ULONG)pG->SaveData.Item_work[main_idx].Id * 0x4b0));
			}
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
	register POLY_FT4 *pft4;
	register POLY_F4 *pf4;
	auto LINE_F2 *lf2;
	register SPRT *sp;
	register ULONG clut;
	register ULONG i;
	register ULONG j;

	pft4 = (POLY_FT4 *)0x80198000;	/* St_prim_a (DS 0x80198000) */
	pf4  = (POLY_F4 *)0x80198800;	/* St_prim_b (DS 0x80198800) */
	lf2  = (LINE_F2 *)0x80199000;	/* St_prim_c (DS 0x80199000) */
	sp   = (SPRT *)0x80199200;	/* St_prim_d (DS 0x80199200) */
	clut = GetClut(0, 0x1e7);

	/* Init item box icon sprites */
	for (i = 25; i > 0; ) {
		i--;
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
	}

	/* Init item box poly-FT4 cells */
	for (i = 6; i > 0; ) {
		i--;
		for (j = 2; j != 0; j--) {
			SetPolyFT4(pft4);
			pft4->u0 = 0x28; pft4->v0 = 0x5a + 0x1e * i;
			pft4->u1 = 0x50; pft4->v1 = 0x5a + 0x1e * i;
			pft4->u2 = 0x28; pft4->v2 = 0x78 + 0x1e * i;
			pft4->u3 = 0x50; pft4->v3 = 0x78 + 0x1e * i;
			pft4->tpage = GetTPage(1, 0, 0x1c0, 0x100);
			pft4->clut  = GetClut(0, 0x1e5);
			pft4->code &= 0xfd;
			pft4->r0 = 0x80;
			pft4->g0 = 0x80;
			pft4->b0 = 0x80;
			pft4++;
		}
	}

	/* Init background poly-F4 cells */
	for (i = 4; i > 0; ) {
		i--;
		for (j = 2; j != 0; j--) {
			SetPolyF4(pf4);
			pf4->code |= 0x02;
			pf4->r0 = 0x20;
			pf4->g0 = 0x20;
			pf4->b0 = 0x40;
			pf4++;
		}
	}

	/* Init line primitives */
	for (i = 6; i > 0; ) {
		i--;
		setLineF2(lf2);
		lf2->r0 = 0x80;
		lf2->g0 = 0x20;
		lf2->b0 = 0x20;
		lf2->code &= 0xfd;
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
	LINE_F2 *lf2;
	register SPRT *sp;
	ULONG *pOt;
	ULONG *pOt2;
	ULONG x;
	register ULONG y;
	register ULONG i;
	register LONG add_x;
	LONG add_y;
	LONG sel;
	UCHAR col;
	ULONG cbuf;
	register GLOBAL *pG;

	pG = &G;
	cbuf = (ULONG)pG->Cbuf_id;
	pft4 = (POLY_FT4 *)0x80198000;	/* St_prim_a (DS 0x80198000) */
	pf4  = (POLY_F4 *)0x80198800;	/* St_prim_b (DS 0x80198800) */
	lf2 = (LINE_F2 *)0x80199000;	/* St_prim_c (DS 0x80199000) */
	sp = (SPRT *)0x80199200;	/* St_prim_d (DS 0x80199200) */

	pOt  = pG->Ot0[cbuf] + 2;
	pOt2 = pG->Ot0[cbuf] + 6;
	y = St.Ibox_pos_y;
	x = St.Ibox_pos_x;

	if (cbuf != 0) {
		sp  = (SPRT *)0x80199200 + 1;
		pft4     = (POLY_FT4 *)0x80198000 + 1;
		lf2 = (LINE_F2 *)0x80199000 + 1;
		pf4      = (POLY_F4 *)0x80198800 + 1;
	}

	/* Animate shimmer */
	if (sw != 0) {
		Move_x[0] -= 2;
	} else {
		Move_x[0] += 2;
	}
	if (0x3e < Move_x[0]) { Move_x[0] = 0x3f; sw = 1; }
	if (Move_x[0] < 1)    { Move_x[0] = 0;    sw = 0; }

	/* Render box icon rows: 4 POLY_F4 cells at pf4, one per f4_tbl row (3 down to 0) */
	for (i = 4; i > 0; ) {
		i--;
		pf4->x0 = x + 6;
		pf4->y0 = St_ibox_f4_tbl[i][0] + y;
		pf4->x1 = x + 0xc5;
		pf4->y1 = St_ibox_f4_tbl[i][0] + y;
		pf4->x2 = x + 6;
		pf4->y2 = St_ibox_f4_tbl[i][0] + y + 0x14;
		pf4->x3 = x + 0xc5;
		pf4->y3 = St_ibox_f4_tbl[i][0] + y + 0x14;
		pf4->r0 = St_ibox_f4_tbl[i][1];
		pf4->g0 = St_ibox_f4_tbl[i][2];
		pf4->b0 = St_ibox_f4_tbl[i][3];
		AddPrim(pOt, pf4);
		pf4 += 2;
	}
	/* Render cursor/selected box sprites; sp walks the SPRT pairs (2 buffers interleaved) like DS */
	sp->x0 = St_ibox_pos_tbl[0x18][0] + x;
	sp->y0 = St_ibox_pos_tbl[0x18][1] + y;
	sp->clut = GetClut(0x100, 0x1f0);
	{
		UCHAR bri = (St.Sub_rtn0 == 2) ? 0x80 : 0x40;
		sp->r0 = bri; sp->g0 = bri; sp->b0 = bri;
	}
	AddPrim(pOt, sp);
	AddPrim(pOt, &St.Dmode[0xb][G.Cbuf_id]);
	sp += 2;

	sp->x0 = St_ibox_pos_tbl[0x17][0] + x;
	sp->y0 = St_ibox_pos_tbl[0x17][1] + y;
	sp->clut = GetClut(0, 0x1e4);
	AddPrim(pOt, sp);
	AddPrim(pOt, &St.Dmode[1][G.Cbuf_id]);
	sp += 2;

	/* Down arrow */
	sp->x0 = St_ibox_pos_tbl[0x16][0] + x;
	sp->y0 = St_ibox_pos_tbl[0x16][1] + y;
	if (((G.Padd1 & 0x4008) != 0) && (St.Sub_rtn1 != 0)) {
		sp->u0 = 0x12; sp->v0 = 100;
	} else {
		sp->u0 = 0x12; sp->v0 = 0x78;
	}
	AddPrim(pOt, sp);
	sp += 2;

	/* Up arrow */
	sp->x0 = St_ibox_pos_tbl[0x15][0] + x;
	sp->y0 = St_ibox_pos_tbl[0x15][1] + y;
	if (((G.Padd1 & 0x1004) != 0) && (St.Sub_rtn1 != 0)) {
		sp->u0 = 0x12; sp->v0 = 0x3c;
	} else {
		sp->u0 = 0x12; sp->v0 = 0x50;
	}
	AddPrim(pOt, sp);
	sp += 2;

	/* Item slot sprites: 5 SPRT cells, Ibox_pos_tbl row 16+k, slot (Ibox_sel + 4 - k) & 0x3f */
	for (i = 5; i > 0; ) {
		i--;
		sp->x0 = St_ibox_pos_tbl[0x14 - i][0] + x;
		sp->y0 = St_ibox_pos_tbl[0x14 - i][1] + y +
							  (SHORT)((((ULONG)St.Ibox_sel + i) & 0x3f) * 2);
		AddPrim(pOt, sp);
		sp += 2;
	}

	/* Background cells: 16 SPRT cells, Ibox_pos_tbl row 15-k */
	for (i = 16; i > 0; ) {
		i--;
		sp->x0 = St_ibox_pos_tbl[i][0] + x;
		sp->y0 = St_ibox_pos_tbl[i][1] + y;
		AddPrim(pOt, sp);
		sp += 2;
	}
	AddPrim(pOt, &St.Dmode[0x14][G.Cbuf_id]);

	/* Scroll highlight for box scroll animation: only while Sub_rtn1 is 2 (up) or 3 (down), so add_y and sel are set */
	if ((ULONG)St.Sub_rtn1 - 2 < 2) {
		if (St.Sub_rtn1 == 2) {
			add_y = -20;
			sel = -1;
		}
		if (St.Sub_rtn1 == 3) {
			add_y = 100;
			sel = 5;
		}
		if (pG->SaveData.Item_boxwork[((ULONG)St.Ibox_sel + sel) & 0x3f].Size != 0) {
			add_x = 0x30;
			col = 0x78;
		} else {
			add_x = 0x18;
			col = 0x50;
		}
		pft4->v0 = 0x3c;
		pft4->v1 = 0x3c;
		pft4->u0 = 0x28;
		pft4->u1 = col;
		pft4->u2 = 0x28;
		pft4->v2 = 0x5a;
		pft4->u3 = col;
		pft4->v3 = 0x5a;
		pft4->x0 = St.Ibox_pos_x + 0x92;
		pft4->y0 = (signed char)St.Ibox_moxe + (St.Ibox_pos_y + (add_y + 0x1b));
		pft4->x1 = St.Ibox_pos_x + add_x + 0x93;
		pft4->y1 = (signed char)St.Ibox_moxe + (St.Ibox_pos_y + (add_y + 0x1b));
		pft4->x2 = St.Ibox_pos_x + 0x92;
		pft4->y2 = (signed char)St.Ibox_moxe + (St.Ibox_pos_y + (add_y + 0x2e));
		pft4->x3 = St.Ibox_pos_x + add_x + 0x93;
		pft4->y3 = (signed char)St.Ibox_moxe + (St.Ibox_pos_y + (add_y + 0x2e));
		addPrim(pOt2, pft4);
	}
	pft4 += 2;

	/* Render visible box items with names: 5 POLY_FT4 cells, slot (Ibox_sel + k) & 0x3f, k = 4 down to 0 */
	for (i = 5; i > 0; ) {
		ULONG id;
		ULONG slot;
		ULONG ya;
		ULONG yb;
		i--;
		slot = ((ULONG)St.Ibox_sel + i) & 0x3f;
		ya = 0x1b + 0x14 * i;
		yb = 0x2e + 0x14 * i;
		if (pG->SaveData.Item_boxwork[slot].Size != 0) {
			add_x = 0x30;
			col = 0x78;
		} else {
			add_x = 0x18;
			col = 0x50;
		}
		pft4->u0 = 0x28;
		pft4->v0 = 0x5a + 0x1e * i;
		pft4->u1 = col;
		pft4->v1 = 0x5a + 0x1e * i;
		pft4->u2 = 0x28;
		pft4->v2 = 0x78 + 0x1e * i;
		pft4->u3 = col;
		pft4->v3 = 0x78 + 0x1e * i;
		pft4->x0 = x + 0x92;
		pft4->y0 = (signed char)St.Ibox_moxe + (y + ya);
		pft4->x1 = x + add_x + 0x93;
		pft4->y1 = (signed char)St.Ibox_moxe + (y + ya);
		pft4->x2 = x + 0x92;
		pft4->y2 = (signed char)St.Ibox_moxe + (y + yb);
		pft4->x3 = x + add_x + 0x93;
		pft4->y3 = (signed char)St.Ibox_moxe + (y + yb);
		addPrim(pOt2, pft4);
		pft4 += 2;
		id = (ULONG)pG->SaveData.Item_boxwork[slot].Id;
		if (id != 0) {
			St_disp_name(x + 7, 0x14 * i + y + 0x1d + (signed char)St.Ibox_moxe, 6, id);
		} else {
			St_disp_name(x + 7, 0x14 * i + y + 0x1d + (signed char)St.Ibox_moxe, 6, 100);
		}
	}

	/* Separator lines */
	lf2->x0 = x + 6;
	lf2->x1 = x + 0xc4;
	lf2->y0 = (signed char)St.Ibox_moxe + (y + ((0x3f - ((St.Ibox_sel - 4) & 0x3f)) * 0x14 + 7));
	lf2->y1 = (signed char)St.Ibox_moxe + (y + ((0x3f - ((St.Ibox_sel - 4) & 0x3f)) * 0x14 + 7));
	{
		UCHAR s2 = (St.Ibox_sel + 2) & 0x3f;
		if (s2 < 5 || s2 == 0x3f) {
			AddPrim(pOt, lf2);
		}
	}

	/* Two highlight bars */
	{
		SHORT ly = y + 0x6a;
		lf2 += 2;
		for (i = 2; i > 0; ) {
			i--;
			ly -= 0x14;
			lf2->y0 = ly;
			lf2->y1 = ly;
			lf2->x0 = x + 6;
			lf2->x1 = x + 0xc4;
			if (St.Sub_rtn1 != 0) {
				lf2->r0 = (UCHAR)Move_x[0] + 0x50;
				lf2->g0 = (UCHAR)Move_x[0] + 0x40;
				lf2->b0 = (UCHAR)Move_x[0] & 0x1f;
			} else {
				lf2->r0 = 0x30; lf2->g0 = 0x20; lf2->b0 = 8;
			}
			AddPrim(pOt, lf2);
			lf2 += 2;
		}
	}
}
