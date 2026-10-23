/*
 * ST_SUB02.C - Status screen sub-module 02: item get and item list
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB02.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include "ST_SUB02.H"
#include "CD.H"
#include "ESP.H"
#include "GAME.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "ST_SUB04.H"
#include "SUB.H"
#include "DOOR.H"
#include "FILECODE.H"

/* The file-select header uses St_pool_c100 (BUFFERS.C), the same retail buffer that St_init_disp_file fills */

UCHAR St_anim_cnt;		/* animation counter, DS 0x800CBC28 */

/* DS 0x800d783e (yy) */
SHORT yy;

/* DS stack frame: 0x20 bytes; s0..s1 saved at sp+0x10..0x14, ra saved at sp+0x18. No local variable slots are
 * used. */
/* DS 0x80071ba0 (St_init02) */
VOID St_init02(VOID)
{
	register STATUS *pSt;
	register ULONG i;

	pSt = &St;
	i = (ULONG)G.Get_item_id;

	if (i < 0x68) {
		pSt->Main_rtn = 1;
		pSt->Sub_rtn0 = 2;
		CD.Fno = ITPS_ITP;
		CD.Fsize = 0x3000;
		pSt->File_flg = 0;
		CD.Fsector = (UINT)floc[ITPS_ITP].sec_high * 0x10000 + (UINT)floc[ITPS_ITP].sector + i * 6;
		CD.Fsum = Itp_check_sum[i];
		Cd_read(ITPS_ITP, (ULONG *)St_load_buf, 2, (UCHAR *)"ITP");
		/* Ctpage (low byte) and Cclut (high byte) are one halfword store (DS: one sh at G.Ctpage) */
		*(USHORT *)&G.Ctpage = 0x0817;
		Mapping_tim((ULONG *)St_load_buf);
		St_init_disp_itp(1);
		St_init_disp_frame(0);
		Fade_set(0x200, -0x1800, 7, 1);
		do {
			Task_sleep(1);
			St_disp_itp(1);
			St_disp_itemlist();
			St_disp_num();
			St_disp_equip();
			St_disp_ECG(Check_pl_life());
			St_disp_face();
			St_disp_message();
			St_disp_frame(0);
			St_disp_menu0();
		} while (Fade_status(0) == 0);
	} else {
		ULONG slot = St_file_set(i - 0x68);
		pSt->F_sel_book = (CHAR)(slot >> 3);
		pSt->F_sel_file = (UCHAR)slot & 7;
		pSt->F_page_no = 0;
		pSt->File_flg = 1;
		pSt->Fread_char = 0;
		Task_execute(2, file_moji_load);
		do {
			Task_sleep(1);
		} while (Task_status(2) != 0);
		/* Ctpage (low byte) and Cclut (high byte) are one halfword store (DS: one sh at G.Ctpage) */
		*(USHORT *)&G.Ctpage = 0x0917;
		Mapping_tim((ULONG *)St_load_buf);
		/* Ctpage (low byte) and Cclut (high byte) are one halfword store (DS: one sh at G.Ctpage) */
		*(USHORT *)&G.Ctpage = 0x0A17;
		Mapping_tim((ULONG *)St_load_buf2);
		Bg_set_mode(2, 0);
		St_init_disp_file();
		pSt->File_pos_x = 0x138;
		pSt->Main_rtn = 1;
		pSt->Sub_rtn0 = 4;
		pSt->Sub_rtn1 = 3;
		Fade_set(0x200, -0x1800, 7, 1);
		do {
			Task_sleep(1);
			St_disp_file();
		} while (Fade_status(0) == 0);
		Snd_se_stad(0x4080000, (VECTOR *)0x0);
	}
}

/* Item get / check screen, one frame: run the proc for Sub_rtn0 (fade out, fade wait, check, item, file).
 * The check page looks up the item set that the Item_get_flg tables describe, the item page adds it, and
 * the file page shows the document text. */
/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80071e14 (St_main02) */
VOID St_main02(VOID)
{
	static CODE_PTR St_getitem_proc_tbl[5] = {St_fade_out_set, St_fade_out_wait, St_get_check, St_get_item, St_get_file};
	ULONG type;

	((CODE_PTR)(St_getitem_proc_tbl[St.Sub_rtn0]))();
	if (St.File_flg == 0) {
		St_disp_itp(1);
		St_disp_itemlist();
		St_disp_num();
		St_disp_equip();
		type = Check_pl_life();
		St_disp_ECG(type);
		St_disp_face();
		St_disp_message();
		St_disp_frame(0);
		St_disp_menu0();
	}
}

/* Check page: decides what the pickup does. Item 0x52 opens sub-page 0x13. A stackable item (ids 0x14..0x1f)
 * that is already held with room below Item_max merges into its slot (Sub_rtn2 = 1). Otherwise the result of
 * Search_item(1) and the free-space check pick the message, and the item page (Sub_rtn0 = 3) is entered. */
/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80071eb4 (St_get_check) */
VOID St_get_check(VOID)
{
	register LONG num;
	register SCE_AOT *pAot;
	register SCE_AOT *pTop;
	register ULONG i;

	num = Search_item(1);
	pTop = G.pAot_data;

	/* data part at DS +0xc (SCE_AOT is 0xe bytes, so +1 would be wrong) */
	if ((pTop->Type & 0x80) != 0) {
		pAot = (SCE_AOT *)&((SCE_AOT *)&pTop->Data)->W;
	} else {
		pAot = (SCE_AOT *)&pTop->Data;
	}

	i = (UINT)G.Get_item_id;
	if (i == 0x52) {
		St.Sub_rtn0 = 3;
		St.Sub_rtn1 = 0x13;
		St.Sub_rtn2 = 0;
		return;
	} else {
		ULONG slot = 0;
		if (i - 0x14 < 0xc) {
			LONG idx = 0;
			do {
				if ((i == (ULONG)(&G.SaveData.Item_work[0].Id)[idx]) && ((INT)(&G.SaveData.Item_work[0].Num)[idx] <= (INT)((UINT)Item_data_tbl[i].Item_max - (UINT)*(USHORT *)&pAot->nFloor))) {
					St.Same_id_work_no = (UCHAR)slot;
					St.Sub_rtn2 = 1;
					goto get;
				}
				slot++;
				idx += 4;
			} while (slot < G.SaveData.Max_item_num);
		}
		if (num < 1) {
			Mess_set(0xaf0010, 0xe400, 1, 0);
		} else {
			if (num != 1 || Check_item_space((UINT)G.Get_item_id) == 0) {
get:
				Mess_set(0xaf0010, 0xe400, 0, 0);
				St.Sub_rtn1 = 1;
				goto done;
			}
			Mess_set(0xaf0010, 0xe400, 4, 0);
		}
	}
done:
	St.Sub_rtn0 = 3;
}

/* DS stack frame: 0x20 bytes; s0..s1 saved at sp+0x10..0x14, ra saved at sp+0x18. No local variable slots are
 * used. */
/* DS 0x80072050 (St_get_item) */
VOID St_get_item(VOID)
{
	/* DS 0x800A9CB0: 11 x {x, y, add_y, sub_rtn3} item move steps */
	static CHAR move_tbl[11][4] = {
		{16, 8, -1, 13}, {14, 9, -1, 15}, {16, 11, 1, 13}, {14, 11, 1, 15}, {17, 15, 3, 13}, {14, 10, 3, 15},
		{18, 8, 6, 13}, {14, 10, 5, 15}, {17, 11, 8, 13}, {14, 10, 7, 15}, {14, 2, -3, 15}
	};
	register STATUS *pSt;
	register ULONG w_no;
	register SCE_AOT *pAot;
	register SCE_AOT *pItem;
	register ULONG D0;
	register ULONG Om_no;

	pSt = &St;

	switch (pSt->Sub_rtn1) {
	case 0:
		if ((Moji.Mess_flg & 0x80) != 0) {
			return;
		}
		goto case_18;
	case 1:
		if ((Moji.Mess_flg & 0x80) == 0) {
			if ((Moji.Mess_flg & 1) == 0) {
				if (pSt->Sub_rtn2 != 0) {
					w_no = (ULONG)pSt->Same_id_work_no;
				} else {
					w_no = Search_item(0);
				}
				D0 = w_no;			/* move_tbl row: the work slot, or row 10 for 'R' */
				if (G.Get_item_id == 82) {
					Snd_se_stad(0x4060000, (VECTOR *)0x0);
					D0 = 10;
				}
				f_x = (SHORT)(signed char)move_tbl[D0][0];
				f_y = (SHORT)(signed char)move_tbl[D0][1];
				add_y = (LONG)(signed char)move_tbl[D0][2];
				pSt->Sub_rtn3 = move_tbl[D0][3];
				pSt->Sub_rtn1 = 2;
			} else {
				pSt->Sub_rtn1 = 4;
			}
		}
		break;
	case 2:
		if (pSt->Sub_rtn3 != 0) {
			f_x += pSt->Sub_rtn3 + 8;
			f_y = f_y + (SHORT)add_y;
			xx = xx + -6;
			if (xx < 0x2e) {
				xx = 0x2d;
			}
			yy = yy + -4;
			if (yy < 0x1f) {
				yy = 0x1e;
			}
			pSt->Sub_rtn3--;
		}
		St_anim_cnt++;
		if (St_anim_cnt != 0x12) {
			return;
		}
		/* fall through */
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		pSt->Sub_rtn1++;
		break;
	case 3:
		pItem = G.pAot_data;
		pItem->Id = 0;
		if ((pItem->Type & 0x80) != 0) {
			pAot = (SCE_AOT *)&((SCE_AOT *)&pItem->Data)->W;
		} else {
			pAot = (SCE_AOT *)&pItem->Data;
		}
		if (pSt->Sub_rtn2 != 0) {
			G.SaveData.Item_work[pSt->Same_id_work_no].Num =
				G.SaveData.Item_work[pSt->Same_id_work_no].Num + (CHAR)*(USHORT *)&pAot->nFloor;
		} else {
			if (G.Get_item_id == 0x52) {
				Set_iwork(10, 0x52, 1, 0);
			} else {
				Set_item((UINT)G.Get_item_id, (UINT)*(USHORT *)&pAot->nFloor, (ULONG *)St_item_tbl);
			}
		}
		G.SaveData.Get_id = *(SHORT *)pAot;
		if (G.SaveData.Stage_no >= 4) {
			Flg_on(G.SaveData.Item_flg2, (UINT)(USHORT)pAot->X);
		} else {
			Flg_on(G.SaveData.Item_flg, (UINT)(USHORT)pAot->X);
		}
		Om_no = (UINT)(UCHAR)pAot->Z;
		if (Om_no != 0xff) {
			G.Ob_model[Om_no].Be_flg = 0;
		}
		{
			UCHAR extra = *(UCHAR *)((int)&pAot->Z + 1);
			if ((extra & 0x80) != 0) {
				D0 = (extra & 0x60) >> 5;
				if (D0 == 0) D0 = 1;
				Esp_kill(5, (D0 << 3) | 7, (MATRIX *)G.Ob_model[Om_no].M);
			}
		}
		Mess_set(0xaf0010, 0xe400, 6, 0);
		pSt->Sub_rtn1 = 0;
		break;
	case 18:
case_18:
		Snd_se_stad(0x4050000, (VECTOR *)0x0);
		pSt->Sub_rtn0 = 0;
		break;
	case 19:
	{
		if (St.Sub_rtn2++ < 0x33) {
			return;
		}
		pSt->Sub_rtn1++;
		break;
	}
	case 20:
		pSt->Sub_rtn1 = 2;
		pSt->Sub_rtn2 = 0;
		f_x = (SHORT)(signed char)move_tbl[10][0];
		f_y = (SHORT)(signed char)move_tbl[10][1];
		add_y = (LONG)(signed char)move_tbl[10][2];
		pSt->Sub_rtn3 = move_tbl[10][3];
		Snd_se_stad(0x4060000, (VECTOR *)0x0);
		break;
	}
}

/* DS stack frame: 0x28 bytes; s0..s4 saved at sp+0x10..0x20, ra saved at sp+0x24. No local variable slots are
 * used. */
/* DS 0x800724b4 (St_disp_file2) */
VOID St_disp_file2(VOID)
{
	register ULONG *pOt;
	register STATUS *pSt;

	UCHAR bri;
	SPRT *p;

	pSt = &St;
	pOt = G.Ot0[G.Cbuf_id] + 5;

	p = St_pool_c100;
	if (G.Cbuf_id != 0) {
		p = &St_pool_c100[1];
	}

	p[0].x0 = pSt->File_pos_x;
	p[0].y0 = pSt->File_pos_y;
	AddPrim(pOt, p);
	AddPrim(pOt, &St.Dmode[0x16][G.Cbuf_id]);

	p[2].x0 = 100;
	p[2].y0 = 0x3c;
	AddPrim(pOt, &p[2]);
	AddPrim(pOt, &pSt->Dmode[0x12][G.Cbuf_id]);

	{
		FILE_DATA fd;

		fd = File_data[(UINT)G.SaveData.File_tag_bk[pSt->F_sel_book * 8 + (int)pSt->F_sel_file] * 2];
		if (pSt->F_page_no == fd.Page_max && fd.Back_h == 0) {
			((SPRT *)p)[4].w = 0x2a;
			((SPRT *)p)[4].h = 0xe;
			((SPRT *)p)[4].u0 = 0x38;
			((SPRT *)p)[4].v0 = 0xc;
			((SPRT *)p)[4].x0 = 0x118;
			((SPRT *)p)[4].y0 = 0x6e;
			((SPRT *)p)[4].clut = GetClut(0x100, 0x1ea);
			bri = 0x30;
			if (pSt->Sub_rtn1 != 1) {
				((SPRT *)p)[4].r0 = 0x30;
				goto disp_bri;
			}
		} else {
			((SPRT *)p)[4].w = 0xc;
			((SPRT *)p)[4].h = 0xd;
			((SPRT *)p)[4].u0 = 0x2a;
			((SPRT *)p)[4].v0 = 0xc;
			((SPRT *)p)[4].x0 = (USHORT)pSt->blink_sw0 * 3 + 0x11a;
			((SPRT *)p)[4].y0 = 0x6e;
			((SPRT *)p)[4].clut = GetClut(0x100, 0x1ec);
		}
		bri = 0x80;
		((SPRT *)p)[4].r0 = 0x80;
disp_bri:
		((SPRT *)p)[4].g0 = bri;
		((SPRT *)p)[4].b0 = bri;
		if (pSt->Sub_rtn1 < 2) {
			AddPrim(pOt, &((SPRT *)p)[4]);
		}
	}

	((SPRT *)p)[6].r0 = 0x80;
	((SPRT *)p)[6].g0 = 0x80;
	((SPRT *)p)[6].b0 = 0x80;
	((SPRT *)p)[6].x0 = (USHORT)pSt->blink_sw0 * (USHORT)-3 + 0xc;
	((SPRT *)p)[6].y0 = 0x6e;
	if (pSt->Sub_rtn1 == 0 && pSt->F_page_no != 0) {
		AddPrim(pOt, &((SPRT *)p)[6]);
	}
	AddPrim(pOt, &St.Dmode[0x17][G.Cbuf_id]);
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x8007274c (St_get_file) */
VOID St_get_file(VOID)
{
	/* DS 0x800A9CDC: 12-entry scroll step table */
	static UCHAR add[12] = {1, 2, 4, 6, 8, 10, 16, 32, 48, 64, 96, 112};
	register GLOBAL *pG;
	register STATUS *pSt;
	register SCE_AOT *pAot;
	register ULONG Om_no;
	register ULONG max_page;
	register SCE_AOT *pItem;

	pG  = &G;
	pSt = &St;

	switch (pSt->Sub_rtn1) {
	case 0:
		max_page = *(USHORT *)&File_data[ (UINT)G.SaveData.File_tag_bk[pSt->F_sel_book * 8 + (int)pSt->F_sel_file] * 2 ];
		if (pSt->blink_sw0 != 0) {
			if (pSt->blink_timer0 < 10) {
				pSt->blink_sw0 = 0;
			}
			pSt->blink_timer0 -= 2;
		} else {
			if (0x50 < pSt->blink_timer0) {
				pSt->blink_sw0 = 1;
			}
			pSt->blink_timer0 += 2;
		}
		if ((pG->Key_trg & 0x2000) != 0) {
			pSt->Sub_rtn1 = 6;
			Mess_set(0xaf0010, 0xe400, 10, 0);
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			break;
		}
		if ((int)G.System_flg < 0 && (pG->Padd1 & 0xa000) != 0) {
			if ((pG->Padd1 & 0x2000) != 0) {
				if (max_page <= (USHORT)pSt->F_page_no) {
					pSt->Sub_rtn1 = 1;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
					goto check_prev;
				}
				pSt->F_page_no++;
				pSt->Sub_rtn1 = 2;
			} else {
check_prev:
				if ((pG->Padd1 & 0x8000) == 0 || pSt->F_page_no == 0) break;
				pSt->F_page_no--;
				pSt->Sub_rtn1 = 4;
			}
			Snd_se_stad(0x4080000, (VECTOR *)0x0);
			pSt->Fread_char = 1;
			Task_execute(2, file_moji_load);
		}
		break;
	case 1:
		if ((int)G.System_flg < 0 && (pG->Padd1 & 0x8000) != 0) {
			pSt->Sub_rtn1 = 0;
			pSt->blink_sw0 = 0;
			pSt->blink_timer0 = 2;
		}
		if ((pG->Key_trg & 0x3000) != 0) {
			pSt->Sub_rtn1 = 6;
			Mess_set(0xaf0010, 0xe400, 10, 0);
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
		}
		break;
	case 2:
	case 3:
	{
		UCHAR step = pSt->Sub_rtn2 + 1;
		if (pSt->Sub_rtn2 >= 0xb) {
			if (pSt->Sub_rtn1 != 2) {
				goto step_end;
			}
			pSt->Sub_rtn2 = step;
			if (Task_status(2) != 0) {
				goto page_busy;
			}
			/* DS stores Ctpage (low byte) and Cclut (high byte) as one halfword */
			*(USHORT *)&pG->Ctpage = 0xa17;
			Mapping_tim((ULONG *)St_load_buf);
			pSt->File_pos_x = 0x138;
			pSt->Sub_rtn1 = 3;
			goto step_reset;
		} else {
			UCHAR v = (pSt->Sub_rtn1 == 2) ? add[step] : add[0xb - (UINT)step];
			pSt->Sub_rtn2++;
			pSt->File_pos_x -= v;
			break;
		}
	}
	case 4:
	case 5:
	{
		UCHAR step = pSt->Sub_rtn2 + 1;
		if (pSt->Sub_rtn2 >= 0xb) {
			if (pSt->Sub_rtn1 != 4) {
step_end:
				pSt->Sub_rtn1 = 0;
step_reset:
				pSt->Sub_rtn2 = 0;
				pSt->blink_sw0 = 0;
				pSt->blink_timer0 = 2;
				break;
			}
			pSt->Sub_rtn2 = step;
			if (Task_status(2) == 0) {
				/* DS stores Ctpage (low byte) and Cclut (high byte) as one halfword */
				*(USHORT *)&pG->Ctpage = 0xa17;
				Mapping_tim((ULONG *)St_load_buf);
				pSt->File_pos_x = -0x106;
				pSt->Sub_rtn1 = 5;
				goto step_reset;
			}
page_busy:
			pSt->Sub_rtn2 = 0xe;
			break;
		} else {
			UCHAR v = (pSt->Sub_rtn1 == 4) ? add[step] : add[0xb - (UINT)step];
			pSt->Sub_rtn2++;
			pSt->File_pos_x += v;
		}
		break;
	}
	case 6:
		if ((Moji.Mess_flg & 0x80) == 0) {
			pAot = G.pAot_data;
			pAot->Id = 0;
			if ((pAot->Type & 0x80) != 0) {
				pItem = (SCE_AOT *)&((SCE_AOT *)&pAot->Data)->W;
			} else {
				pItem = (SCE_AOT *)&pAot->Data;
			}
			pG->SaveData.Get_id = (SHORT)(pItem->Id | (pItem->Type << 8));
			if (G.SaveData.Stage_no < 4) {
				Flg_on(G.SaveData.Item_flg, (UINT)pItem->X);
			} else {
				Flg_on(G.SaveData.Item_flg2, (UINT)pItem->X);
			}
			Om_no = (UINT)(UCHAR)pItem->Z;
			if (Om_no != 0xff) {
				pG->Ob_model[Om_no].Be_flg = 0;
			}
			{
				UCHAR extra = *(UCHAR *)((int)&pItem->Z + 1);
				if ((extra & 0x80) != 0) {
					Esp_kill(5, (UCHAR)((extra & 0x60) >> 2) | 7, (MATRIX *)G.Ob_model[Om_no].M);
				}
			}
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			pSt->Sub_rtn0 = 0;
		}
		/* fall through */
	default:
		return;
	}
	St_disp_file2();
}

/* DS 0x800c4408 (add_y) */
LONG add_y;

/* DS 0x800d7834 (xx) */
SHORT xx;

/* DS 0x800dcba4 (f_x) */
SHORT f_x;

/* DS 0x800dcba6 (f_y) */
SHORT f_y;
