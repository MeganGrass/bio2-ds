/*
 * STATUS.C - Status/inventory screen, item and ammo management
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STATUS.C
 */
#include "TYPES.H"
#include "MAIN.H"
#include "CONFIG.H"
#include "ROOM.H"
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSPU.H>
#include "STATUS.h"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "GAME.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "PAD.H"
#include "PL_WEP.H"
#include "PLAYER.H"
#include "SELECT.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "ST_SUB01.H"
#include "ST_SUB02.H"
#include "ST_SUB04.H"
#include "SUB.H"
#include "DOOR.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x800d4e00 */
UCHAR bk_cut[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/* DS 0x800d5bf0 (St) */
STATUS St = { 0 };

/* DS 0x800dcbcc (bk_light) */
LIGHT_CUT_DATA *bk_light = 0;


/* DS 0x801a8820: TIM buffer uploaded by Status_init_m (no Ghidra label) */
#define St_tim_buf2		((ULONG *)0x801a8820)
/* DS 0x801b5000: VRAM save buffer for St_pltex_operation (no Ghidra label) */
#define St_pltex_buf		((ULONG *)0x801b5000)

/* DS 0x80068068 (Status_init) */
VOID Status_init(VOID)
{
	register STATUS *pSt;				// 4208 bytes
	register ITEM_WORK *i_work;			// 4 bytes
	register ULONG i;

	ULONG tblBase = 0;
	pSt = &St;

	if ((G.System_flg & SYS_EX_BATTLE) == 0) {
		tblBase = -((LONG)G.Status_flg < 0) & 0xb;
		if (G.Player.Id == 0x0c) {
			tblBase = 0x16;
		}
		if (G.Player.Id == 0x0d) {
			tblBase = 0x21;
		}
	} else if (G.SaveData.Player_life == 1) {
		tblBase = 0x37;
	} else if (G.SaveData.Player_life == 0) {
		tblBase = 0x2c;
	} else if (G.SaveData.Player_life == 2) {
		tblBase = 0x42;
	} else if (G.SaveData.Player_life == 3) {
		tblBase = 0x4d;
	}

	i_work = G.SaveData.Item_boxwork;
	for (i = 0xb; i > 0; i--) {
		LONG idx = i - 1 + tblBase;
		i_work[-1].Id   = init_item_tbl_Leon[idx][0];
		i_work[-1].Num  = init_item_tbl_Leon[idx][1];
		i_work[-1].Size = init_item_tbl_Leon[idx][2];
		i_work--;
	}

	pSt->Equip_item_id = init_item_tbl_Leon[tblBase][0];
	pSt->Equip_item_no = 0;

	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		if (G.SaveData.Player_life > 1) {
			pSt->Equip_item_id = init_item_tbl_Leon[tblBase + 2][0];
			pSt->Equip_item_no = 0x02;
		}
		if (G.SaveData.Sce_timer == 2) {
			G.SaveData.Item_work[3].Num = 0x03;
		} else if (G.SaveData.Sce_timer == 3) {
			G.SaveData.Item_work[3].Num = 0x01;
		} else {
			G.SaveData.Item_work[3].Num = 0x05;
		}
	}

	if ((G.System_flg & SYS_ARRANGE) != 0) {
		G.SaveData.Item_work[0].Size = 0x01;
		G.SaveData.Item_work[0].Id   = 0x0f;
		G.SaveData.Item_work[0].Num  = 0xff;
		G.SaveData.Item_work[1].Id   = 0x0f;
		G.SaveData.Item_work[1].Num  = 0xff;
		G.SaveData.Item_work[1].Size = 0x02;
		pSt->Equip_item_id = 0x0f;
		pSt->Equip_item_no = 0;
	}

	for (i = 0x40; i > 0; i--) {
		G.SaveData.Item_boxwork[i - 1].Id   = 0;
		G.SaveData.Item_boxwork[i - 1].Num  = 0;
		G.SaveData.Item_boxwork[i - 1].Size = 0;
	}

	G.SaveData.Max_item_num  = 0x08;
	G.SaveData.Weapon_mugen  = 0;
	Flg_on(&G.SaveData.Weapon_mugen, 0x12);
	Flg_off(G.SaveData.Common_flg, 0x7e);

	for (i = 0x17; ; i--) {
		G.SaveData.File_tag_bk[i] = 0xff;
		if (i == 0) break;
	}

	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		G.SaveData.Item_boxwork[2].Id   = 0x01;
		G.SaveData.Item_boxwork[2].Num  = 0x01;
		G.SaveData.Item_boxwork[2].Size = 0x00;
	}

	if ((G.Status_flg & STAT_EASY_MODE) != 0) {
		if (((G.System_flg & SYS_ENDING) == 0) && ((G.Status_flg & STAT_SCENARIO_B) == 0)) {
			File_data[0x2e].Page_max = 0x01;
			File_data[0x2e].Back_h   = 0x00;
			G.SaveData.File_tag_bk[0] = 0x17;
		}
		G.SaveData.Item_work[3].Id   = 0x14;
		G.SaveData.Item_work[3].Num  = 120;
		G.SaveData.Item_work[3].Size = 0x00;
		G.SaveData.Item_boxwork[1].Id   = 35;
		G.SaveData.Item_boxwork[1].Num  = 0x01;
		G.SaveData.Item_boxwork[1].Size = 0x00;
		G.SaveData.Item_boxwork[2].Id   = 35;
		G.SaveData.Item_boxwork[2].Num  = 0x01;
		G.SaveData.Item_boxwork[2].Size = 0x00;
		G.SaveData.Item_boxwork[3].Id   = 35;
		G.SaveData.Item_boxwork[3].Num  = 0x01;
		G.SaveData.Item_boxwork[3].Size = 0x00;
	}

	if ((G.System_flg & SYS_ARRANGE) != 0) {
		G.SaveData.Item_work[3].Id   = 0x00;
		G.SaveData.Item_work[3].Num  = 0x00;
		G.SaveData.Item_work[3].Size = 0x00;
		G.SaveData.Item_boxwork[4].Id   = 0x11;
		G.SaveData.Item_boxwork[0].Id   = 0x12;
		G.SaveData.Item_boxwork[0].Num  = 0xff;
		G.SaveData.Item_boxwork[0].Size = 0x03;
		G.SaveData.Item_boxwork[4].Num  = 0xff;
		G.SaveData.Item_boxwork[4].Size = 0x03;
		G.SaveData.Item_boxwork[5].Id   = 0x01;
		G.SaveData.Item_boxwork[5].Num  = 0x01;
		G.SaveData.Item_boxwork[5].Size = 0x00;
		if ((LONG)G.Status_flg < 0) {
			G.SaveData.Item_boxwork[6].Id  = 0x03;
			G.SaveData.Item_boxwork[6].Num = 0x0d;
		} else {
			G.SaveData.Item_boxwork[6].Id  = 0x02;
			G.SaveData.Item_boxwork[6].Num = 0x12;
		}
		G.SaveData.Item_boxwork[6].Size = 0x00;
		G.SaveData.Item_boxwork[7].Id   = 0x14;
		G.SaveData.Item_boxwork[7].Num  = 120;
		G.SaveData.Item_boxwork[7].Size = 0x00;
	}
}

/* DS 0x80068484 (Status_init_m) */
VOID Status_init_m(VOID)
{
	register STATUS *pSt;				// 4208 bytes
	register ULONG i;

	ULONG File_no;
	UCHAR *Mess;
	DR_MODE *pDmode0;
	DR_MODE *pDmode1;
	ULONG iconIdx;
	MAP_DATA *pMapData;

	pSt = &St;

	if ((G.System_flg & SYS_EX_BATTLE) == 0 ||
		G.SaveData.Player_life == 0 || G.SaveData.Player_life == 2) {
		File_no = ST0_TIM;
		Mess = (UCHAR *)"ST0";
		Cd_read(File_no, Bg_img_buf, 0, Mess);
	} else if (G.SaveData.Player_life == 1) {
		File_no = ST1_TIM;
		Mess = (UCHAR *)"ST1";
		Cd_read(File_no, Bg_img_buf, 0, Mess);
	} else if (G.SaveData.Player_life == 3) {
		File_no = ST1__TIM;
		Mess = (UCHAR *)"ST1";
		Cd_read(File_no, Bg_img_buf, 0, Mess);
	}

	G.Ctpage = 0x1a;
	G.Cclut  = 0x04;
	Mapping_tim(Bg_img_buf);
	G.Ctpage = 0x1b;
	G.Cclut  = 0x0a;
	Mapping_tim(St_tim_buf2);
	Cd_read(ITEMALL_PIX, Bg_img_buf, 0, (UCHAR *)"ITEM ALL");

	i = 10;
	do {
		i--;
		if (G.SaveData.Item_work[i].Size == 0x02) {
			iconIdx = G.SaveData.Item_work[i].Id + 0x56;
		} else {
			iconIdx = G.SaveData.Item_work[i].Id;
		}
		Pix_trans((i >> 1) + i, Bg_img_buf + iconIdx * 300);
	} while (i != 0);

	/* SetDrawMode for Dmode[0xb..0x6] tpage 0x9b */
	pDmode0 = (DR_MODE *)&St.Dmode[0xb][0].code[1];
	pDmode1 = (DR_MODE *)&St.Dmode[0xa][1].code[1];
	for (i = 8; i > 0; i--) {
		pDmode0 -= 2;
		SetDrawMode(pDmode0, 0, 0, 0x9b, (RECT *)0);
		SetDrawMode(pDmode1, 0, 0, 0x9b, (RECT *)0);
		pDmode1 -= 2;
	}

	/* SetDrawMode for Dmode[0x11..0x6] tpage 0x1b */
	pDmode0 = (DR_MODE *)&St.Dmode[0x11][0].code[1];
	pDmode1 = (DR_MODE *)&St.Dmode[0x10][1].code[1];
	for (i = 6; i > 0; i--) {
		pDmode0 -= 2;
		SetDrawMode(pDmode0, 0, 0, 0x1b, (RECT *)0);
		SetDrawMode(pDmode1, 0, 0, 0x1b, (RECT *)0);
		pDmode1 -= 2;
	}

	/* SetDrawMode for Dmode[0x15..0x12] tpage 0x9a */
	pDmode0 = (DR_MODE *)&St.Dmode[0x15][0].code[1];
	pDmode1 = (DR_MODE *)&St.Dmode[0x14][1].code[1];
	for (i = 4; i > 0; i--) {
		pDmode0 -= 2;
		SetDrawMode(pDmode0, 0, 0, 0x9a, (RECT *)0);
		SetDrawMode(pDmode1, 0, 0, 0x9a, (RECT *)0);
		pDmode1 -= 2;
	}

	SetDrawMode((DR_MODE *)&St.Dmode[0x15][0].code[1], 0, 0, 0x97, (RECT *)0);
	SetDrawMode((DR_MODE *)&St.Dmode[0x15][1].code[1], 0, 0, 0x97, (RECT *)0);
	SetDrawMode(&St_end_tag_c[0], 0, 0, 0xb7, (RECT *)0);
	SetDrawMode(&St_end_tag_c[1], 0, 0, 0xb7, (RECT *)0);
	SetDrawMode(&Map_end_tag[0], 0, 0, 0x17, (RECT *)0);
	SetDrawMode(&Map_end_tag[1], 0, 0, 0x17, (RECT *)0);
	SetDrawMode(&Map_end_tag[2], 0, 0, 0x1b, (RECT *)0);
	SetDrawMode(&Map_end_tag[3], 0, 0, 0x1b, (RECT *)0);

	pSt->St_type        = 0;
	pSt->Machinegun_trg = 0;
	pSt->Pl_chenge_flg  = 0;
	St.Dmode[3][0].tag &= 0xffff;
	pSt->Main_rtn = 0;
	pSt->Sub_rtn0 = 0;
	pSt->Sub_rtn1 = 0;
	pSt->Sub_rtn2 = 0;
	pSt->Sub_rtn3 = 0;
	pSt->Sub_rtn4 = 0;
	pSt->Sub_rtn5 = 0;
	pSt->Sub_rtn6 = 0;

	if (G.SaveData.Item_work[pSt->Equip_item_no].Num != 0) {
		G.Player.Status_flg |= 1;
	}
	if (G.SaveData.Key_idx != 0x02) {
		Flg_on(&G.SaveData.Weapon_mugen, 0x1f);
	}
	if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
		Map_info[0x11].pMap_data[4].W = 95;
	} else {
		Map_info[0x11].pMap_data[4].W = 44;
	}

	pMapData = Map_info[6].pMap_data;
	if (((G.Status_flg & STAT_SPECIAL_RUN) == 0) && ((G.System_flg & SYS_EX_BATTLE) == 0)) {
		pMapData[0].Pl1_flg = 0x01;
		pMapData[1].Pl1_flg = 0x01;
		pMapData[2].Pl1_flg = 0x01;
		pMapData[9].Pl1_flg = 0x01;
		pMapData = Map_info[5].pMap_data;
		pMapData[1].Pl1_flg  = 0x01;
		pMapData[9].Pl1_flg  = 0x01;
		pMapData[10].Pl1_flg = 0x01;
	} else {
		if ((G.System_flg & SYS_ENDING) == 0) {
			Flg_on(G.SaveData.Map_c_flg + 1, 4);
		}
		pMapData[0].Pl1_flg = 0;
		pMapData[1].Pl1_flg = 0;
		pMapData[2].Pl1_flg = 0;
		pMapData[9].Pl1_flg = 0;
		pMapData = Map_info[5].pMap_data;
		pMapData[1].Pl1_flg  = 0;
		pMapData[9].Pl1_flg  = 0;
		pMapData[10].Pl1_flg = 0;
	}
	if (((G.System_flg & SYS_EX_BATTLE) != 0) && ((G.System_flg & SYS_ENDING) == 0)) {
		Flg_on(G.SaveData.Map_c_flg + 1, 1);
	}
}

extern VOID St_exit(VOID);

/* DS 0x800689bc (Status) */
VOID Status(VOID)
{
	/* [St_type][Main_rtn] state handlers; DS table at 0x800a936c (5 rows in the exe; St_map_hint sets St_type 4) */
	static CODE_PTR Status_proc_tbl[5][3] = {
		{ St_init00, St_main00, St_exit },
		{ St_init01, St_main01, St_exit },
		{ St_init02, St_main02, St_exit },
		{ St_init03, St_main03, St_exit },
		{ St_init_map_rooms, St_main_map_rooms, St_exit }
	};
	/* DS 0x800a93a8: light set used while the status screen is up (initial values from the exe) */
	static LIGHT_CUT_DATA st_light = {
		2, { 0, 0, 0 },
		{ { 240, 240, 240 }, { 240, 240, 240 }, { 240, 240, 240 } },
		{ 73, 73, 73 },
		{ { -240, 1130, 3330 }, { -1500, -8000, 3000 }, { -1500, -8000, 3000 } },
		{ 4800, 3000, 0 }
	};
	register STATUS *pSt;					// 4208 bytes
	register GLOBAL *pG;					// 35412 bytes
	register ULONG i;
	ULONG iconIdx;
	UCHAR *pNumCol;

	pSt = &St;
	pG  = &G;

	Task_suspend(0);
	Bg_set_mode(2, 0x200808);
	Pad_rep_set(0xf01c, 0x60a);
	SetGeomScreen(0xd0);
	Main.Vloop = 0;

	if (St.Pl_chenge_flg != 0) {
		Cd_read(ITEMALL_PIX, Bg_img_buf, 0, (UCHAR *)"ITEM ALL");
		i = 10;
		do {
			i--;
			if (pG->SaveData.Item_work[i].Size == 0x02) {
				iconIdx = pG->SaveData.Item_work[i].Id + 0x56;
			} else {
				iconIdx = pG->SaveData.Item_work[i].Id;
			}
			Pix_trans((i >> 1) + i, Bg_img_buf + iconIdx * 300);
		} while (i != 0);
		pSt->Pl_chenge_flg = 0;
	}

	if (Flg_ck(G.SaveData.Item_flg, 0x3f) != 0) {
		G.SaveData.Max_item_num = 10;
	}
	if (Flg_ck(G.SaveData.Scenario_flg, 0xa2) != 0) {
		G.SaveData.Item_work[10].Id = 47;
	}

	i = 1;
	pNumCol = &Item_data_tbl[1].Item_num_col;
	do {
		if (Flg_ck(&G.SaveData.Weapon_mugen, i) == 0) {
			*pNumCol &= ~0x10;
		} else {
			*pNumCol |= 0x10;
		}
		i++;
		pNumCol += 8;
	} while (i < 0x14);

	bk_light = (G.pRoom)->pLight;
	bk_cut[0] = (UCHAR)G.SaveData.Cut_no;
	G.SaveData.Cut_no  = 0;
	G.Next_cut_no      = 0;
	(G.pRoom)->pLight  = &st_light;

	St_init_disp_itemlist();
	St_init_disp_num();
	St_init_disp_equip();
	St_init_disp_ECG();
	St_init_disp_face();
	St_init_disp_cursol0();
	St_init_disp_cursol1();
	St_init_disp_menu0();
	St_init_disp_wall();
	St_init_disp_message();

	pSt->Main_rtn    = 0;
	pSt->Sub_rtn0    = 0;
	pSt->Sub_rtn1    = 0;
	pSt->Sub_rtn2    = 0;
	pSt->Sub_rtn3    = 0;
	pSt->Sub_rtn4    = 0;
	pSt->Sub_rtn5    = 0;
	pSt->Sub_rtn6    = 0;
	pSt->Icursol_main = 0;
	pSt->Icursol_sub  = 0;

	if ((St.Dmode[3][0].tag >> 24) == 0) {
		pSt->Equip_item_no_old = pSt->Equip_item_no;
		pSt->Equip_item_id_old = pSt->Equip_item_id;
	} else {
		pSt->Equip_item_no_old = 0x80;
		pSt->Equip_item_id_old = 0;
		St.Dmode[3][0].tag &= 0x00ffffff;
		pSt->Equip_item_no = (UCHAR)St.Dmode[3][0].code[0];
		pSt->Equip_item_id = (UCHAR)(St.Dmode[3][0].code[0] >> 8);
	}

	pSt->Exit_flg  = 0x01;
	pSt->Sub_pos_x = 0;
	pSt->Sub_pos_y = 0;
	St_pltex_operation(0);
	Sort_item();

	do {
		(Status_proc_tbl[pSt->St_type][pSt->Main_rtn])();
		Task_sleep(1);
	} while (1);
}

/* DS 0x80068cd4 (St_exit) */
VOID St_exit(VOID)
{
	register STATUS *pSt;				// 4208 bytes

	UCHAR equipId;
	UCHAR equipNo;

	pSt = &St;
	St_pltex_operation(1);
	equipId = pSt->Equip_item_id;
	equipNo = pSt->Equip_item_no;

	if ((G.SaveData.Use_id == 0x32) || (G.SaveData.Use_id == 0x4b) || (G.SaveData.Use_id == 0x43)) {
		pSt->Equip_item_no = 0x80;
		pSt->Equip_item_id = 0;
		St.Dmode[3][0].tag = (St.Dmode[3][0].tag & 0x00ffffff) | 0x01000000;
		St.Dmode[3][0].code[0] = (St.Dmode[3][0].code[0] & 0xffff0000) | ((ULONG)equipId << 8) | equipNo;
	}

	if (pSt->St_type == 0x01) {
		pl_tim_load((ENEMY_WORK *)&G.Player);
		Weapon_cls((ENEMY_WORK *)&G.Player);
		Pl_weapon_ch((ENEMY_WORK *)&G.Player);
	}

	if (pSt->Equip_item_id_old != pSt->Equip_item_id) {
		G.Player.Type &= 0xf000;
		if (pSt->Equip_item_id != 0) {
			G.Player.Type |= G.SaveData.Item_work[pSt->Equip_item_no].Id;
		}
		Weapon_cls((ENEMY_WORK *)&G.Player);
		Pl_weapon_ch((ENEMY_WORK *)&G.Player);
		if ((G.Status_flg & STAT_MIRROR_MATRIX) != 0) {
			Mirror_weapon_cp((ENEMY_WORK *)&G.Player);
		}
		Snd_sys_arms_set((UCHAR)G.Player.Type);
	}

	Main.Bg_chg_flg = 0x02;
	if (pSt->St_type == 0x02) {
		Main.Bg_chg_flg = 0x01;
	}
	pSt->St_type = 0;

	if (G.SaveData.Item_work[pSt->Equip_item_no].Num != 0) {
		G.Player.Status_flg |= 1;
	}

	Main.Vloop = 0x02;
	Bg_set_mode(0, 0);
	SetGeomOffset(0xa0, 0x78);
	SetGeomScreen((ULONG)((G.pRoom)->pRcut[G.SaveData.Cut_no].ViewR >> 7));
	G.SaveData.Cut_no = (SHORT)bk_cut[0];
	G.Next_cut_no     = bk_cut[0];
	(G.pRoom)->pLight = bk_light;
	Task_signal(0);
	Task_exit();
}

/* DS 0x80068f08 (St_fade_out_set) */
VOID St_fade_out_set(VOID)
{
	Fade_set(0x200, 0x1800, 7, 1);
	St.Sub_rtn0 = 0x01;
}

/* DS 0x80068f40 (St_fade_out_wait) */
VOID St_fade_out_wait(VOID)
{
	ULONG fadeSt;

	fadeSt = Fade_status(0);
	if (fadeSt != 0) {
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0);
		St.Main_rtn = 0x02;
	}
}

/* DS 0x80068f9c (St_chenge_pl) */
VOID St_chenge_pl(register ULONG pl_id)
{
	register GLOBAL *pG;					// 35412 bytes
	register STATUS *pSt;					// 4208 bytes
	register ULONG i;
	ULONG offset;

	pG  = &G;
	pSt = &St;
	i   = 0;

	if (pl_id - 0xe < 2) {
		G.Bk_equip_no = St.Equip_item_no;
		G.Bk_item_num = G.SaveData.Max_item_num;
		Flg_on(G.SaveData.Common_flg, 0x7f);
		offset = 11;
		if (pl_id == 0xe) {
			St.Equip_item_id = 3;
			St.Equip_item_no = 0;
			offset = 0;
		} else {
			St.Equip_item_id = 0;
			St.Equip_item_no = 0x80;
		}
		pG->SaveData.Max_item_num = 8;
		for (; i < 11; i++) {
			pG->Bk_item_work[i].Id   = pG->SaveData.Item_work[i].Id;
			pG->Bk_item_work[i].Num  = pG->SaveData.Item_work[i].Num;
			pG->Bk_item_work[i].Size = pG->SaveData.Item_work[i].Size;
			pG->SaveData.Item_work[i].Id   = init_item_tbl_Ada[offset + i][0];
			pG->SaveData.Item_work[i].Num  = init_item_tbl_Ada[offset + i][1];
			pG->SaveData.Item_work[i].Size = init_item_tbl_Ada[offset + i][2];
		}
	} else {
		if (Flg_ck(pG->SaveData.Common_flg, 0x7f) != 0) {
			do {
				pG->SaveData.Item_work[i].Id   = pG->Bk_item_work[i].Id;
				pG->SaveData.Item_work[i].Num  = pG->Bk_item_work[i].Num;
				pG->SaveData.Item_work[i].Size = pG->Bk_item_work[i].Size;
				i++;
			} while (i < 10);
			pG->SaveData.Max_item_num = pG->Bk_item_num;
			pSt->Equip_item_no = pG->Bk_equip_no;
			if (pG->Bk_equip_no == 0x80) {
				pSt->Equip_item_id = 0;
			} else {
				pSt->Equip_item_id = pG->SaveData.Item_work[pG->Bk_equip_no].Id;
			}
			pG->SaveData.Item_work[10].Id = 0x30;
			if ((pl_id & 1) == 0) {
				pG->SaveData.Item_work[10].Id = 47;
			}
			pG->SaveData.Item_work[10].Num  = 0x01;
			pG->SaveData.Item_work[10].Size = 0x00;
		}
		Flg_off(pG->SaveData.Common_flg, 0x7f);
	}
}

/* DS 0x80069210 (pl_tim_load) */
VOID pl_tim_load(register ENEMY_WORK *pEm)
{
	register UCHAR *Buf_top;
	PL_DATA_TBL *pTbl;
	ULONG *pBase;
	ULONG unused_slot[6];				/* DS reserves 24 bytes of frame here and never touches them */

	(void)unused_slot;

	/* Reload the player file only for its TIM; the TMD pointers are rebuilt into the model copy
	   that Player_set left at Pl_model_copy_buf (same +3 / +10 word offsets as Player_set). */
	Buf_top = (UCHAR *)Bg_img_buf;
	Cd_read((ULONG)Pld_no[pEm->Id], Bg_img_buf, 1, (UCHAR *)"PLD");
	pTbl = (PL_DATA_TBL *)(Buf_top + Bg_img_buf[0]);
	pEm->Clut  = 1;
	pEm->Tpage = 0x17;
	pBase      = (ULONG *)(Pl_model_copy_buf + pTbl->Tmd_ofs);
	pEm->pTmd  = pBase + 3;
	pEm->pTmd2 = pBase + 10;
	G.Cclut  = 1;
	G.Ctpage = 0x17;
	Mapping_tim((ULONG *)(Buf_top + pTbl->Tim_ofs));
	DrawSync(0);
}

/* DS 0x800692dc (St_file_set) */
ULONG St_file_set(register ULONG file_no)
{
	register ULONG i;

	UCHAR *pFileTag = G.SaveData.File_tag_bk;
	for (i = 0; i < 0x18; i++) {
		if (*pFileTag == 0xff) {
			*pFileTag = (UCHAR)file_no;
			return i;
		}
		pFileTag++;
	}
	return 0;
}

/* DS 0x8006931c (St_room_set) */
VOID St_room_set(VOID)
{
	register ULONG flag;

	LONG flgBase = 0;
	switch (G.SaveData.Stage_no) {
	case 1: flgBase = 0x1e; break;
	case 2: flgBase = 0x3a; break;
	case 3: flgBase = 0x48; break;
	case 4: flgBase = 0x59; break;
	case 5: flgBase = 99;   break;
	case 6: flgBase = 0x7b; break;
	}
	Flg_on(G.SaveData.Map_flg, flgBase + G.SaveData.Room_no);
	flag = Check_map_no((ULONG)G.SaveData.Stage_no, (ULONG)G.SaveData.Room_no);
	Flg_on(&G.SaveData.Map_o_flg, flag);
}

/* DS 0x800693d0 (St_disp_name) */
VOID St_disp_name(register LONG x, register LONG y, register ULONG at, register ULONG id)
{
	register ULONG item_at;

	item_at = Item_data_tbl[id].Item_attribute & 0xf;
	if ((item_at != 0) && (Flg_ck(G.SaveData.Item_c_flg, item_at) != 0)) {
		id = item_at + 0x81;
	}
	Name_disp(x, y, at | 0x4000, id);
}

/* DS 0x8006947c (Set_iwork) */
VOID Set_iwork(register ULONG no, register ULONG id, register ULONG num, register ULONG size)
{
	register GLOBAL *pG;				// 35412 bytes

	pG = &G;
	pG->SaveData.Item_work[no].Id   = (UCHAR)id;
	pG->SaveData.Item_work[no].Num  = (UCHAR)num;
	pG->SaveData.Item_work[no].Size = (UCHAR)size;
}

/* DS 0x800694b8 (Set_iwork_num) */
/* DS stores num as passed (no UCHAR mask) and, for a size-2 item, writes the Num of the preceding slot (no - 1). */
VOID Set_iwork_num(register ULONG no, register ULONG num)
{
	register GLOBAL *pG;				// 35412 bytes

	UCHAR itemSize;

	pG = &G;
	itemSize = pG->SaveData.Item_work[no].Size;
	if (itemSize == 0x01) {
		pG->SaveData.Item_work[no + 1].Num = num;
		itemSize = pG->SaveData.Item_work[no].Size;
	}
	if (itemSize == 0x02) {
		pG->SaveData.Item_work[no - 1].Num = num;
	}
	pG->SaveData.Item_work[no].Num = num;
}

/* DS 0x80069538 (Check_pl_life) */
ULONG Check_pl_life(VOID)
{
	register ULONG now_life;
	register ULONG max_life;

	now_life = (ULONG)G.Player.Life;
	max_life = (ULONG)G.Player.Max_life;
	if (G.Pl_poison_timer == 0) {
		if (now_life <= (max_life >> 1)) {
			if (now_life < 0x29) {
				if (now_life < 0x15) {
					return 3;
				}
				return 2;
			}
			return 1;
		}
		return 0;
	}
	return 4;
}

/* DS 0x80069590 (Check_item_space) */
ULONG Check_item_space(register ULONG id)
{
	if (id == 0x13) {
		return 0;
	}
	return (ULONG)(id - 0xe < 6);
}

/* DS 0x800695b0 (Check_item_mix) */
ULONG Check_item_mix(VOID)
{
	register STATUS *pSt;				// 4208 bytes
	register ITEM_MIX_DATA *im_data;	// 4 bytes
	register ULONG id_m;
	register ULONG id_s;
	register ULONG mix_max;
	register ULONG ctr;

	pSt  = &St;
	id_m = G.SaveData.Item_work[pSt->Icursol_main].Id;
	id_s = G.SaveData.Item_work[pSt->Icursol_sub].Id;

	if ((pSt->Icursol_sub != pSt->Icursol_main) && (id_s != 0) &&
		(Item_data_tbl[id_m].Item_mix_num != 0)) {
		im_data  = Item_data_tbl[id_m].Item_mix_data;
		mix_max  = Item_data_tbl[id_m].Item_mix_num;
		for (ctr = 0; ctr < mix_max; ctr++) {
			if (im_data->Object_item_id == id_s) {
				pSt->Mixed_id     = im_data->Result_item;
				pSt->Mixed_pix_no = im_data->Mixed_pix_no;
				return (ULONG)im_data->Mix_type;
			}
			im_data++;
		}
	}
	return 0;
}

/* DS 0x80069668 (Search_item) */
LONG Search_item(register ULONG type)
{
	register ULONG ctr;
	register ULONG rt;

	ctr = 0;
	rt  = 0;
	while (1) {
		if (G.SaveData.Item_work[ctr].Id == 0) {
			if (type == 0) {
				return (LONG)ctr;
			}
			rt++;
		}
		ctr++;
		if (G.SaveData.Max_item_num <= ctr) {
			if (type != 0) {
				return (LONG)rt;
			}
			return -1;
		}
	}
}

/* DS 0x800696cc (Search_item_id) */
LONG Search_item_id(register ULONG id)
{
	register ULONG ctr;

	ctr = 0;
	do {
		if (G.SaveData.Item_work[ctr].Id == id) {
			return (LONG)ctr;
		}
		ctr++;
	} while (ctr < G.SaveData.Max_item_num);
	return -1;
}

/* DS 0x80069714 (Sort_item) */
VOID Sort_item(VOID)
{
	register GLOBAL *pG;				// 35412 bytes
	register STATUS *pSt;				// 4208 bytes
	register ULONG ctr;
	register ULONG last;
	LONG no;

	no  = Search_item(0);
	pG  = &G;
	pSt = &St;
	ctr = no + 1;

	if (no == 9) {
		Pix_clear(13);
		G.SaveData.Item_work[9].Id   = 0;
		G.SaveData.Item_work[9].Num  = 0;
		G.SaveData.Item_work[9].Size = 0;
	} else if ((no >= 0) && (ctr < 10)) {
		/* move every slot after the empty one down by one */
		do {
			Pix_move((ctr >> 1) + ctr, ((ctr - 1) >> 1) - 1 + ctr);
			pG->SaveData.Item_work[ctr - 1].Id   = pG->SaveData.Item_work[ctr].Id;
			pG->SaveData.Item_work[ctr - 1].Num  = pG->SaveData.Item_work[ctr].Num;
			pG->SaveData.Item_work[ctr - 1].Size = pG->SaveData.Item_work[ctr].Size;
			if (pSt->Equip_item_no == ctr) {
				pSt->Equip_item_no--;
			}
			if ((UCHAR)pSt->Dmode[3][0].code[0] == ctr) {
				pSt->Dmode[3][0].code[0] = (pSt->Dmode[3][0].code[0] & 0xffffff00) | (UCHAR)((UCHAR)pSt->Dmode[3][0].code[0] - 1);
			}
			ctr++;
		} while (ctr < pG->SaveData.Max_item_num);
		last = ctr - 1;
		Pix_clear(((last >> 1) - 1) + ctr);
		pG->SaveData.Item_work[last].Id   = 0;
		pG->SaveData.Item_work[last].Num  = 0;
		pG->SaveData.Item_work[last].Size = 0;
	}
}

/* DS 0x800698b4 (Shift_item) */
VOID Shift_item(register ULONG type)
{
	register GLOBAL *pG;				// 35412 bytes
	register STATUS *pSt;				// 4208 bytes
	register ULONG i;

	pG  = &G;
	pSt = &St;

	if (type == 1) {
		ULONG idx = pG->SaveData.Max_item_num - 2;
		do {
			idx--;
			pG->SaveData.Item_work[idx + 2].Id   = pG->SaveData.Item_work[idx].Id;
			pG->SaveData.Item_work[idx + 2].Num  = pG->SaveData.Item_work[idx].Num;
			pG->SaveData.Item_work[idx + 2].Size = pG->SaveData.Item_work[idx].Size;
			Pix_move((idx >> 1) + idx, (idx >> 1) + idx + 3);
		} while (idx != 0);
		if (((LONG)(ULONG)pSt->Equip_item_no < (LONG)(pG->SaveData.Max_item_num - 2)) &&
			(pSt->Equip_item_no != 0x80)) {
			pSt->Equip_item_no += 0x02;
			return;
		}
	} else {
		for (i = 0; i < pG->SaveData.Max_item_num - 2; i++) {
			ULONG base = (i >> 1) + i;
			pG->SaveData.Item_work[i].Id   = pG->SaveData.Item_work[i + 2].Id;
			pG->SaveData.Item_work[i].Num  = pG->SaveData.Item_work[i + 2].Num;
			pG->SaveData.Item_work[i].Size = pG->SaveData.Item_work[i + 2].Size;
			Pix_move(base + 3, base);
		}
		Set_iwork(pG->SaveData.Max_item_num - 1, 0, 0, 0);
		Pix_clear(0xb);
		Set_iwork(pG->SaveData.Max_item_num - 2, 0, 0, 0);
		Pix_clear(0xc);
		if ((1 < pSt->Equip_item_no) && (pSt->Equip_item_no != 0x80)) {
			pSt->Equip_item_no += 0xfe;
			return;
		}
	}
	pSt->Equip_item_id = 0;
	pSt->Equip_item_no = 0x80;
}

/* DS 0x80069adc (Set_item) */
VOID Set_item(register ULONG id, register ULONG num, register ULONG *adr)
{
	register ULONG work_no;

	work_no = Search_item(0);
	if ((id == 0x13) || (id - 0xe > 5)) {
		Set_iwork(work_no, id, num, 0);
		work_no = (work_no >> 1) + work_no;
	} else {
		Shift_item(1);
		Set_iwork(0, id, num, 1);
		Pix_trans(0, adr);
		Set_iwork(1, id, num, 2);
		work_no = 1;
		adr += 300;
	}
	Pix_trans(work_no, adr);
}

/* DS 0x80069bb4 (Pix_trans) */
VOID Pix_trans(register ULONG set_no, register ULONG *data_addr)
{
	auto RECT rect;						// FFFFFFF0		// 8 bytes

	rect.w = 0x14;
	rect.h = 0x1e;
	rect.x = (SHORT)(((set_no % 3) * 0x28 - 0x80) >> 1) + 0x2c0;
	rect.y = (SHORT)(set_no / 3) * 0x1e + 0x100;
	LoadImage(&rect, data_addr);
	DrawSync(0);
}

/* DS 0x80069c40 (Pix_trans_pl) */
VOID Pix_trans_pl(register ULONG set_no, register ULONG id, register ULONG *data_addr)
{
	auto RECT rect;						// FFFFFFE0		// 8 bytes

	rect.w = 0x14;
	rect.h = 0x1e;
	rect.x = (SHORT)(((set_no % 3) * 0x28 - 0x80) >> 1) + 0x200;
	rect.y = (SHORT)(set_no / 3) * 0x1e + 0x100;
	LoadImage(&rect, data_addr);
	DrawSync(0);
	if ((id != 0x13) && (id - 0xe < 6)) {
		rect.x = (SHORT)((((set_no + 1) % 3) * 0x28 - 0x80) >> 1) + 0x200;
		rect.y = (SHORT)((set_no + 1) / 3) * 0x1e + 0x100;
		LoadImage(&rect, data_addr);
		DrawSync(0);
		LoadImage(&rect, data_addr + 0x64c8);
		DrawSync(0);
	}
}

/* DS 0x80069d88 (Pix_move) */
VOID Pix_move(register ULONG base, register ULONG object)
{
	auto RECT rect;						// FFFFFFF0		// 8 bytes

	rect.w = 0x14;
	rect.h = 0x1e;
	rect.x = (SHORT)(((base % 3) * 0x28 - 0x80) >> 1) + 0x2c0;
	rect.y = (SHORT)(base / 3) * 0x1e + 0x100;
	MoveImage(&rect, (((object % 3) * 0x28 - 0x80) >> 1) + 0x2c0,
			  (object / 3) * 0x1e + 0x100);
	DrawSync(0);
}

/* DS 0x80069e54 (Pix_clear) */
VOID Pix_clear(register ULONG set_no)
{
	auto RECT rect;						// FFFFFFF0		// 8 bytes

	rect.x = 0x2a8;
	rect.y = 0x100;
	rect.w = 0x14;
	rect.h = 0x1e;
	MoveImage(&rect, (((set_no % 3) * 0x28 - 0x80) >> 1) + 0x2c0,
			  (set_no / 3) * 0x1e + 0x100);
	DrawSync(0);
}

/* DS 0x80069eec (St_pltex_operation) */
VOID St_pltex_operation(register ULONG type)
{
	auto RECT rect;						// FFFFFFF0		// 8 bytes

	rect.x = 0x1c0;
	rect.y = 0x100;
	rect.w = 0x40;
	rect.h = 0x100;
	if (type == 0) {
		StoreImage(&rect, St_pltex_buf);
	} else {
		LoadImage(&rect, St_pltex_buf);
	}
	DrawSync(0);
}

/* DS 0x80069f54 (Tama_ck_i) */
ULONG Tama_ck_i(register ULONG id)
{
	ULONG Ret;

	if ((id == 0x12) && (Flg_ck(&G.SaveData.Weapon_mugen, 0x12) != 0)) {
		G.Status_flg |= STAT_INF_AMMO;
	}
	if (((id == 0x0f) && (G.SaveData.Item_work[St.Equip_item_no].Num == 0xff)) ||
		((id == 0x11) && (G.SaveData.Item_work[St.Equip_item_no].Num == 0xff))) {
		G.Status_flg |= STAT_INF_AMMO;
		Ret = 1;
	} else {
		if ((St.Equip_item_no != 0x80) && (Flg_ck(&G.SaveData.Weapon_mugen, id) != 0)) {
			return 1;
		}
		if (id == 0x0e) {
			Ret = 0;
			if (St.Equip_item_no != 0x80) {
				Ret = (ULONG)(G.SaveData.Item_work[St.Equip_item_no].Num >= 3);
			}
		} else {
			Ret = 0;
			if (St.Equip_item_no != 0x80) {
				Ret = (ULONG)(G.SaveData.Item_work[St.Equip_item_no].Num != 0);
			}
		}
	}
	return Ret;
}

/* DS 0x8006a0cc (Tama_decrease_i) */
ULONG Tama_decrease_i(register ULONG id, register ULONG flag)
{
	register ULONG item_no;
	register ULONG num;
	ULONG newNum;

	item_no = St.Equip_item_no;
	num     = G.SaveData.Item_work[item_no].Num;

	if (Flg_ck(&G.SaveData.Weapon_mugen, id) != 0) {
		return 1;
	}

	if (num != 0xff) {
		if ((id == 0x0f) || (id == 0x12)) {
			if ((SHORT)(St.Machinegun_trg + 1) < 8) {
				St.Machinegun_trg++;
				return 1;
			}
			St.Machinegun_trg = 0;
			if (num == 0) {
				return 0;
			}
			num--;
			Set_iwork_num(item_no, num);
		}
		if (id == 0x10) {
			if ((SHORT)(St.Machinegun_trg + 1) < 8) {
				St.Machinegun_trg++;
				return 1;
			}
			St.Machinegun_trg = 0;
			if (num == 0) {
				return 0;
			}
			num--;
			Set_iwork_num(item_no, num);
		}
		if (id == 0x0e) {
			newNum = (flag == 0) ? num - 1 : num - 2;
		} else {
			if (num == 0) {
				return 0;
			}
			newNum = num - 1;
		}
		Set_iwork_num(item_no, newNum);
	}
	return 1;
}

/* DS 0x8006a23c (Tama_reload_ck) */
UCHAR Tama_reload_ck(VOID)
{
	register ITEM_MIX_DATA *tbl_ptr;	// 4 bytes

	LONG itemIdx;
	tbl_ptr = Item_data_tbl[G.SaveData.Item_work[St.Equip_item_no].Id].Item_mix_data;
	itemIdx   = Search_item_id((ULONG)tbl_ptr->Object_item_id);
	if (St.Equip_item_id == 0x11) {
		return 0;
	}
	return (UCHAR)(itemIdx >= 0);
}

/* DS 0x8006a2b4 (Tama_reload) */
VOID Tama_reload(VOID)
{
	register ULONG num;					/* magazine size, then the rounds moved */
	LONG itemIdx;
	UCHAR weaponId;

	weaponId = G.SaveData.Item_work[St.Equip_item_no].Id;
	num      = Item_data_tbl[weaponId].Item_max;
	itemIdx  = Search_item_id((ULONG)Item_data_tbl[weaponId].Item_mix_data->Object_item_id);
	if (num < G.SaveData.Item_work[itemIdx].Num) {
		G.SaveData.Item_work[itemIdx].Num -= num;
	} else {
		num = G.SaveData.Item_work[itemIdx].Num;
		G.SaveData.Item_work[itemIdx].Id = 0;
		Sort_item();
	}
	Set_iwork_num((ULONG)St.Equip_item_no, G.SaveData.Item_work[St.Equip_item_no].Num + num);
}

/* DS 0x8006a38c (Check_cursol_distance) */
/* DS reads St.Icursol_main / St.Icursol_sub straight from memory after each Add_cp_* store and recomputes the Item_work Size
   in each branch (no cached copies); only the final Add_cp_y* calculation goes through the St pointer. */
VOID Check_cursol_distance(register ULONG type)
{
	register STATUS *pSt;				// 4208 bytes

	pSt  = &St;

	if (type != 0) {
		if ((St.Icursol_main & 1) == 0) {
			if (G.SaveData.Item_work[St.Icursol_sub].Size != 0) {
				St.Add_cp_x0 = 0x02;
			} else if ((St.Icursol_sub & 1) != 0) {
				St.Add_cp_x0 = 0x04;
			} else {
				St.Add_cp_x0 = 0;
			}
		} else {
			if (G.SaveData.Item_work[St.Icursol_sub].Size != 0) {
				St.Add_cp_x0 = -2;
			} else if ((St.Icursol_sub & 1) == 0) {
				St.Add_cp_x0 = -4;
			} else {
				St.Add_cp_x0 = 0;
			}
		}
		pSt->Add_cp_y0 = ((pSt->Icursol_sub >> 1) - (pSt->Icursol_main >> 1)) * 0x03;
		return;
	}

	if ((St.Icursol_main & 1) == 0) {
		if (G.SaveData.Item_work[St.Icursol_main].Size != 0) {
			if ((St.Icursol_sub & 1) == 0) {
				St.Add_cp_x1 = 0x02;
			} else {
				St.Add_cp_x1 = -2;
			}
		} else if ((St.Icursol_sub & 1) != 0) {
			St.Add_cp_x1 = -4;
		} else {
			St.Add_cp_x1 = 0;
		}
	} else {
		if (G.SaveData.Item_work[St.Icursol_main].Size != 0) {
			if ((St.Icursol_sub & 1) == 0) {
				St.Add_cp_x1 = 0x02;
			} else {
				St.Add_cp_x1 = -2;
			}
		} else if ((St.Icursol_sub & 1) == 0) {
			St.Add_cp_x1 = 0x04;
		} else {
			St.Add_cp_x1 = 0;
		}
	}
	pSt->Add_cp_y1 = ((pSt->Icursol_main >> 1) - (pSt->Icursol_sub >> 1)) * 0x03;
}

/* DS 0x800a9264 (init_item_tbl_Leon) */
UCHAR init_item_tbl_Leon[11][3] = {
	{ 0x02, 0x12, 0x00 },
	{ 0x01, 0x01, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x2f, 0x01, 0x00 }
};

/* DS 0x800a93d0 (init_item_tbl_Ada) */
UCHAR init_item_tbl_Ada[11][3] = {
	{ 0x03, 0x0d, 0x00 },
	{ 0x14, 0x2d, 0x00 },
	{ 0x23, 0x01, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x57, 0x0f, 0x00 }
};

/* Tables moved here from ST_SUB02.C */
/* DS 0x800a9e1c (Item_data_tbl) */
ITEM_DATA Item_data_tbl[101] = {
	{ 0x00, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x12, 0x00, 0x00, 0x02, Mix_beretta },
	{ 0x0d, 0x00, 0x00, 0x01, Mix_beretta },
	{ 0x12, 0x00, 0x00, 0x01, Mix_beretta },
	{ 0x08, 0x00, 0x00, 0x02, Mix_magnam },
	{ 0x08, 0x00, 0x00, 0x01, Mix_magnam },
	{ 0x05, 0x00, 0x00, 0x02, Mix_shotgun },
	{ 0x07, 0x00, 0x00, 0x01, Mix_shotgun },
	{ 0xfa, 0x00, 0x03, 0x03, Mix_g_n },
	{ 0xfa, 0x00, 0x01, 0x03, Mix_g_f },
	{ 0xfa, 0x00, 0x02, 0x03, Mix_g_a },
	{ 0x12, 0x00, 0x00, 0x01, &Mix_bowgun },
	{ 0x06, 0x00, 0x00, 0x01, Mix_beretta },
	{ 0x64, 0x00, 0x80, 0x01, &Mix_spark },
	{ 0x64, 0x00, 0x80, 0x01, &Mix_machinegun },
	{ 0x64, 0x00, 0x81, 0x01, &Mix_fire },
	{ 0x04, 0x00, 0x00, 0x00, &Mix_nodata },
	{ 0x64, 0x00, 0x80, 0x00, &Mix_nodata },
	{ 0x0f, 0x00, 0x00, 0x01, Mix_beretta },
	{ 0xfa, 0x00, 0x03, 0x06, Mix_bullet },
	{ 0xfa, 0x00, 0x03, 0x03, Mix_shot_dan },
	{ 0xfa, 0x00, 0x03, 0x03, Mix_mag_dan },
	{ 0x64, 0x00, 0x81, 0x02, Mix_nen },
	{ 0xfa, 0x00, 0x03, 0x04, Mix_g_normal },
	{ 0xfa, 0x00, 0x01, 0x04, Mix_g_fire },
	{ 0xfa, 0x00, 0x02, 0x04, Mix_g_acid },
	{ 0x64, 0x00, 0x83, 0x02, Mix_dot380 },
	{ 0x64, 0x00, 0x83, 0x02, Mix_battley },
	{ 0xf0, 0x00, 0x03, 0x02, Mix_arrow },
	{ 0xfa, 0x00, 0x00, 0x01, &Mix_ink_ribbon },
	{ 0xfa, 0x00, 0x00, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_parts_a },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_parts_b },
	{ 0x01, 0x00, 0x04, 0x01, Mix_parts_c },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x05, Mix_g },
	{ 0x01, 0x00, 0x04, 0x02, Mix_r },
	{ 0x01, 0x00, 0x04, 0x03, Mix_b },
	{ 0x01, 0x00, 0x04, 0x02, Mix_gg },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_gr },
	{ 0x01, 0x00, 0x04, 0x02, Mix_gb },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x06, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_Jaguar_a },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_Jaguar_b },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_fuse },
	{ 0x01, 0x00, 0x04, 0x01, &Mix_p_bomb },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x07, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x0a, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x01, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x02, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x04, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x05, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x08, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x09, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata },
	{ 0x01, 0x00, 0x04, 0x00, &Mix_nodata }
};

/* DS 0x800aa144 (File_data) */
FILE_DATA File_data[50] = {
	{ 0x05, 0x00 },
	{ 0x70, 0x00 },
	{ 0x05, 0x00 },
	{ 0x50, 0x00 },
	{ 0x01, 0x00 },
	{ 0x50, 0x00 },
	{ 0x02, 0x00 },
	{ 0x50, 0x00 },
	{ 0x09, 0x00 },
	{ 0x50, 0x00 },
	{ 0x09, 0x00 },
	{ 0x50, 0x00 },
	{ 0x09, 0x00 },
	{ 0x50, 0x00 },
	{ 0x06, 0x00 },
	{ 0x70, 0x00 },
	{ 0x04, 0x00 },
	{ 0x70, 0x00 },
	{ 0x04, 0x00 },
	{ 0x50, 0x00 },
	{ 0x01, 0x00 },
	{ 0x80, 0x00 },
	{ 0x02, 0x00 },
	{ 0x90, 0x00 },
	{ 0x03, 0x00 },
	{ 0x90, 0x00 },
	{ 0x02, 0x00 },
	{ 0x90, 0x00 },
	{ 0x03, 0x00 },
	{ 0x70, 0x00 },
	{ 0x08, 0x00 },
	{ 0x50, 0x00 },
	{ 0x06, 0x00 },
	{ 0x50, 0x00 },
	{ 0x09, 0x00 },
	{ 0x50, 0x00 },
	{ 0x05, 0x00 },
	{ 0x50, 0x00 },
	{ 0x01, 0x00 },
	{ 0x90, 0x00 },
	{ 0x06, 0x00 },
	{ 0x50, 0x00 },
	{ 0x05, 0x00 },
	{ 0x50, 0x00 },
	{ 0x04, 0x00 },
	{ 0x50, 0x00 },
	{ 0x02, 0x00 },
	{ 0x50, 0x00 },
	{ 0x02, 0x00 },
	{ 0x50, 0x00 }
};
