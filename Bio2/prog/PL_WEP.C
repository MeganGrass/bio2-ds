/*
 * PL_WEP.C - Player weapon state and attack hit-check routines
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PL_WEP.C
 */
#include "TYPES.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include "MAIN.H"
#include "PL_WEP.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PL_WEP2.H"
#include "SCA.H"
#include "SELECT.H"
#include "SND.H"
#include "STATUS.H"
#include "SUB.H"
#include "GAME.H"
#include "SCE2.H"
#include "FILECODE.H"

/* DS 0x80010fe4: Cd_read file name WEP DATA, padded to 16 bytes (used by Pl_weapon_ch) */
char   s_WEP_DATA_80010fe4[] = "WEP DATA\0\0\0\0\0\0\0";

/* DS 0x80011010 (Pl_mvtbl0_w0) */
ULONG  Pl_mvtbl0_w0 = 0x000E0000;
/* DS 0x80011014 (Pl_mvtbl0_w1) */
ULONG  Pl_mvtbl0_w1 = 0x0000000A;
/* DS 0x80011018 (Pl_mvtbl0_w2) */
USHORT Pl_mvtbl0_w2 = 0x000C;
/* DS 0x8001101c (Pl_mvtbl1_w0) */
ULONG  Pl_mvtbl1_w0 = 0x0000FF38;
/* DS 0x80011020 (Pl_mvtbl1_w1) */
ULONG  Pl_mvtbl1_w1 = 0x007D00FA;
/* DS 0x80011024 (Pl_mvtbl2_w0) */
ULONG  Pl_mvtbl2_w0 = 0x000F0000;
/* DS 0x80011028 (Pl_mvtbl2_w1) */
ULONG  Pl_mvtbl2_w1 = 0x0000000B;
/* DS 0x8001102c (Pl_mvtbl2_w2) */
USHORT Pl_mvtbl2_w2 = 0x000D;

/* DS 0x800a6d08 (Plw_no): PLxxWxx_PLW names (FILECODE.H), by enemy Id */
USHORT Plw_no[16] = {
	PL00W00_PLW, PL01W00_PLW, PL00W00_PLW, PL01W00_PLW,
	PL00W00_PLW, PL01W00_PLW, PL00W00_PLW, PL01W00_PLW,
	PL00W00_PLW, PL01W00_PLW, PL00W00_PLW, PL0BW00_PLW,
	PL0CW00_PLW, PL0DW00_PLW, PL0EW00_PLW, PL0FW00_PLW,
};

/* DS 0x800a68e8 (Pl_weapon_at_base): base of Pl_weapon_tbl (0x800a6900 - 0x18), 0x1e0 bytes to pEnemy_damage_tbl (0x800a6ac8) */
UCHAR  Pl_weapon_at_base[] = {
	0x04, 0x10, 0x00, 0x00, 0xe8, 0x03, 0xf4, 0x01, 0x08, 0x20, 0x00, 0x00, 0xc4, 0x09, 0x71, 0x02,
	0x00, 0x02, 0xff, 0x01, 0xfe, 0xff, 0x00, 0x00, 0x7c, 0x65, 0x0a, 0x80, 0x08, 0x66, 0x0a, 0x80,
	0xa8, 0x63, 0x0a, 0x80, 0x34, 0x64, 0x0a, 0x80, 0xe0, 0x64, 0x0a, 0x80, 0x6c, 0x65, 0x0a, 0x80,
	0x18, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x34, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x50, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x18, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x34, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x50, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x18, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x34, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x50, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x70, 0x66, 0x0a, 0x80, 0xc4, 0x66, 0x0a, 0x80,
	0x8c, 0x66, 0x0a, 0x80, 0xc4, 0x66, 0x0a, 0x80, 0xa8, 0x66, 0x0a, 0x80, 0xc4, 0x66, 0x0a, 0x80,
	0xc8, 0x66, 0x0a, 0x80, 0x1c, 0x67, 0x0a, 0x80, 0xe4, 0x66, 0x0a, 0x80, 0x1c, 0x67, 0x0a, 0x80,
	0x00, 0x67, 0x0a, 0x80, 0x1c, 0x67, 0x0a, 0x80, 0x24, 0x67, 0x0a, 0x80, 0x78, 0x67, 0x0a, 0x80,
	0x40, 0x67, 0x0a, 0x80, 0x78, 0x67, 0x0a, 0x80, 0x5c, 0x67, 0x0a, 0x80, 0x78, 0x67, 0x0a, 0x80,
	0x88, 0x67, 0x0a, 0x80, 0xdc, 0x67, 0x0a, 0x80, 0xa4, 0x67, 0x0a, 0x80, 0xdc, 0x67, 0x0a, 0x80,
	0xc0, 0x67, 0x0a, 0x80, 0xdc, 0x67, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x18, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x34, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x50, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0xf0, 0x67, 0x0a, 0x80, 0x44, 0x68, 0x0a, 0x80, 0x0c, 0x68, 0x0a, 0x80, 0x44, 0x68, 0x0a, 0x80,
	0x28, 0x68, 0x0a, 0x80, 0x44, 0x68, 0x0a, 0x80, 0x48, 0x68, 0x0a, 0x80, 0x9c, 0x68, 0x0a, 0x80,
	0x64, 0x68, 0x0a, 0x80, 0x9c, 0x68, 0x0a, 0x80, 0x80, 0x68, 0x0a, 0x80, 0x9c, 0x68, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80, 0x50, 0x63, 0x0a, 0x80, 0xa4, 0x63, 0x0a, 0x80,
	0xa4, 0x68, 0x0a, 0x80, 0xf8, 0x68, 0x0a, 0x80, 0xc0, 0x68, 0x0a, 0x80, 0xf8, 0x68, 0x0a, 0x80,
	0xdc, 0x68, 0x0a, 0x80, 0xf8, 0x68, 0x0a, 0x80, 0x18, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
	0x34, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80, 0x50, 0x66, 0x0a, 0x80, 0x6c, 0x66, 0x0a, 0x80,
};
/* DS 0x800a702c (pl_mv05_w0_tbl), index Routine_2 */
PL_MV05_FN pl_mv05_w0_tbl[7] = {
	pl_mv05_w1_t0, pl_mv05_w1_t1, pl_mv05_w0_t2, pl_mv05_w1_t3,
	pl_mv05_w1_t4, pl_mv05_w1_t5, pl_mv05_w0_t6,
};
/* DS 0x800a7048 (pl_mv05_w1_tbl), index Routine_2 */
PL_MV05_FN pl_mv05_w1_tbl[6] = {
	pl_mv05_w1_t0, pl_mv05_w1_t1, pl_mv05_w1_t2, pl_mv05_w1_t3,
	pl_mv05_w1_t4, pl_mv05_w1_t5,
};

/* DS 0x800408c8 (Pl_weapon_ch) */
void Pl_weapon_ch(ENEMY_WORK *pEm)
{
	TIM_IMAGE *pTim;
	ULONG *pData;
	PARTS_W *pP_ptr;
	ULONG cd_or_addr;
	int *pOfs;
	ULONG *pPkt;
	ULONG *pMdl;
	UINT parts_cnt;
	UCHAR parts_no;
	TIM_IMAGE Tim_image;
	ULONG *pEmdTop;
	ULONG   Frame_pad[2];		/* retail never reads sp+0x24-0x27 or 0x2c-0x2f (its spill is at 0x28) */

	(void)Frame_pad;
	pMdl = G.pPl_emd_top;
	pData = G.pPl_emd_top + 0x35c;
	pEmdTop = G.pPl_emd_top;
	parts_no = pEm->Id;
	if (parts_no == 15) {
		parts_cnt = (UINT)((pEm->Type & 0xfff) != 0);
		cd_or_addr = Plw_no[0xf] + parts_cnt;
	}
	else if (((parts_no == 8) || (parts_no == 10)) && ((pEm->Type & 0xfff) == 2)) {
		cd_or_addr = PL08W02_PLW;
	}
	else {
		parts_cnt = pEm->Type & 0xfff;
		cd_or_addr = Plw_no[pEm->Id] + parts_cnt;
	}
	Cd_read(cd_or_addr, pData, 0, (UCHAR *)s_WEP_DATA_80010fe4);
	pOfs = (int *)((int)pData + *pData);
	parts_no = pEm->Parts_num;
	parts_cnt = *(UINT *)((int)pData + pOfs[2] + 8) >> 1;
	pEm->Parts_num = parts_no + (char)parts_cnt;
	pEm->pSub0_seq_t_ptr = (ULONG *)((int)pData + *pOfs);
	pEm->pSub0_kan_t_ptr = (ULONG *)((int)pData + pOfs[1]);
	OpenTIM((ULONG *)((int)pData + pOfs[3]));
	pTim = ReadTIM(&Tim_image);
	pTim->prect->x = (USHORT)pEm->Tpage * 0x40 + -0x39c;
	pTim->prect->y = 0x1e0;
	pTim->prect->w = 0x1c;
	pTim->prect->h = 0x20;
	pPkt = pMdl + 5;
	LoadImage(pTim->prect, pTim->paddr);
	DrawSync(0);
	pTim->crect->x = 0xf0;
	pTim->crect->y = pEm->Clut + 0x1e1;
	pTim->crect->w = 0x10;
	LoadImage(pTim->crect, pTim->caddr);
	DrawSync(0);
	Mapping_tmd(2, (ULONG *)((int)pData + pOfs[2]), (UINT)pEm->Tpage, (UINT)pEm->Clut);
	cd_or_addr = (int)pData + pOfs[2] + 0xc;
	pData = G.pPl_pac_top;
	do {
		pPkt[0x18] = (ULONG)&GsIDMATRIX;
		pPkt[0x20] = 0;
		*(USHORT *)(pPkt + 9) = 0;
		pPkt[6] = 0;
		*(USHORT *)((int)pPkt + 0x26) = 0;
		pPkt[7] = 0;
		*(USHORT *)(pPkt + 10) = 0;
		pPkt[8] = 0;
		pPkt[-3] = cd_or_addr;
		pPkt[-1] = cd_or_addr + 0x1c;
		pPkt[0xd] = 0x1000;
		pPkt[0xe] = 0;
		pPkt[0xf] = 0x1000;
		pPkt[0x10] = 0;
		pPkt[0x11] = 0x1000;
		pPkt[0x12] = 0;
		pPkt[0x13] = 0;
		pPkt[0x14] = 0;
		pPkt[1] = 0x1000;
		pPkt[2] = 0;
		pPkt[3] = 0x1000;
		pPkt[4] = 0;
		pPkt[5] = 0x1000;
		pPkt[-4] = 0;
		pPkt[0x17] = 0x808080;
		pPkt[0x15] = 0;
		*(USHORT *)(pPkt + 0x16) = 0;
		*pMdl = 0;
		*(USHORT *)(pPkt + 0x12) = 0;
		*(UCHAR *)(pPkt + 0x19) = parts_no;
		pPkt[-2] = (ULONG)pData;
		pData = (ULONG *)PresetObjectGT3_neo((MODEL_WORK *)(pMdl + 1), pData, 1);
		*pPkt = (ULONG)pData;
		pData = (ULONG *)PresetObjectGT4_neo((MODEL_WORK *)(pMdl + 1), pData, 1);
		parts_no++;
		pPkt += 0x2b;
		pMdl += 0x2b;
		parts_cnt--;
		cd_or_addr += 0x38;
	} while (parts_cnt != 0);
	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr[0xb].pTmd  = (ULONG *)pEmdTop[2];
	pP_ptr[0xb].pTmd2 = (ULONG *)pEmdTop[4];
	pP_ptr[0xb].pPacket  = (ULONG *)pEmdTop[3];
	pP_ptr[0xb].pPacket2 = (ULONG *)pEmdTop[5];
	Pl_weapon_init[pEm->Type & 0xfff](pEm);	/* DS 0x800a6d28 (Pl_weapon_init) */
}

/* DS 0x800a6d28: 20 x PL_WEP_FN, indexed by Type & 0xfff (targets from function_index.tsv) */
PL_WEP_FN Pl_weapon_init[20] = {
	Non_init, Non_init, Plw02_init, Plw02_init,
	Plw02_init, Plw02_init, Plw02_init, Non_init,
	Non_init, Plw09_init, Plw09_init, Plw09_init,
	Plw0c_init, Non_init, Non_init, Plw0f_init,
	Plw0f_init, Non_init, Plw12_init, Non_init
};

/* DS 0x80040c40 (Non_init) */
void Non_init(ENEMY_WORK *pEm)
{
	return;
}

/* DS 0x80040c48 (Plw02_init) */
void Plw02_init(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	PARTS_W *pO;

	pP_ptr = pEm->pSin_parts_ptr;
	pO = pP_ptr + 0x10;
	if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
		pO = pP_ptr + 0x14;
	}
	if (pEm->Id == 14) {
		pO++;
	}
	pO->pSuper = (ULONG *)&pP_ptr[0xb].Workm;
	pO->Be_flg = 0;
	pO->pOya_parts = &pP_ptr[0xb].Be_flg;
	pO->Pos_x = 0;
	pO->Pos_y = 0x96;
	pO->Pos_z = 0;
	if ((pEm->Type & 0xfff) == 2) {
		Pl_weapon_kyan[1][0] |= 0x80;
		if ((pEm->Id == 8) || (pEm->Id == 10)) {
			Pl_weapon_kyan[1][0] = 3;
		}
	}
}

/* DS 0x80040cf4 (Plw09_init) */
void Plw09_init(ENEMY_WORK *pEm)
{
	PARTS_W *pO;
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	pO = pP_ptr + 0x14;
	if (pEm->Id == 14) {
		pO = pP_ptr + 0x11;
	}
	pO->Be_flg = 1;
	pO->Pos_x = 0x40;
	pO->Pos_y = 0x3c0;
	pO->pSuper = (ULONG *)&pP_ptr[0xb].Workm;
	pO->pOya_parts = &pP_ptr[0xb].Be_flg;
	pO->Pos_z = 0;
	pO->Timer3 = 0xff;
	pO[1].Pos_x = 0x87;
	pO[1].Be_flg = 0;
	pO[1].pSuper = (ULONG *)&pP_ptr[0xb].Workm;
	pO[1].pOya_parts = &pP_ptr[0xb].Be_flg;
	pO[1].Pos_y = 0x1c3;
	pO[1].Pos_z = 0;
}

/* DS 0x80040d68 (Plw0c_init) */
void Plw0c_init(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	MATRIX *pMat;
	PARTS_W *pO;
	PARTS_W *pO2;

	pP_ptr = pEm->pSin_parts_ptr;
	pO  = pP_ptr + 0x14;
	pO2 = pP_ptr + 0xb;
	if (pEm->Id == 14) {
		pO = pP_ptr + 0x11;
	}
	pMat = &pP_ptr[0xb].Workm;
	pO->Be_flg  = 0;
	pO->pSuper  = (ULONG *)pMat;
	pO->pOya_parts = &pO2->Be_flg;
	pO->Pos_x = 200;
	pO->Pos_y = 0x390;
	pO->Pos_z = 0;
	*(UCHAR *)pO->Free = 0xff;
	pO[1].Be_flg = 0;
	pO[1].pSuper = (ULONG *)&pO->Workm;
	pO[1].pOya_parts = &pO->Be_flg;
	pO[1].Pos_x = 0;
	pO[1].Pos_y = 0x1bb;
	pO[1].Pos_z = 0;
	pO[2].pSuper = (ULONG *)pMat;
	pO[2].pOya_parts = &pO2->Be_flg;
	pO[2].Pos_x = 200;
	pO[2].Pos_y = 0x390;
	pO[2].Pos_z = -200;
	*(UCHAR *)pO[2].Free = 0xff;
	pO[3].pSuper = (ULONG *)&pO[2].Workm;
	pO[3].pOya_parts = &pO[2].Be_flg;
	pO[3].Pos_x = 0;
	pO[3].Pos_y = 0x1bb;
	pO[3].Pos_z = 0;
	pO[4].pSuper = (ULONG *)pMat;
	pO[4].pOya_parts = &pO2->Be_flg;
	pO[4].Pos_x = 200;
	pO[4].Pos_y = 0x390;
	pO[4].Pos_z = 200;
	*(UCHAR *)pO[4].Free = 0xff;
	pO[5].pSuper = (ULONG *)&pO[4].Workm;
	pO[5].pOya_parts = &pO[4].Be_flg;
	pO[5].Pos_x = 0;
	pO[5].Pos_y = 0x1bb;
	pO[5].Pos_z = 0;
	pO[6].pSuper = (ULONG *)pMat;
	pO[6].pOya_parts = &pO2->Be_flg;
	pO[6].Pos_x = 200;
	pO[6].Pos_y = 0x390;
	pO[6].Pos_z = 0;
	*(UCHAR *)pO[6].Free = 0xff;
	pO[7].pSuper = (ULONG *)&pO[6].Workm;
	pO[7].pOya_parts = &pO[6].Be_flg;
	pO[7].Pos_x = 0;
	pO[7].Pos_y = 0x1bb;
	pO[7].Pos_z = 0;
	pO[8].pSuper = (ULONG *)pMat;
	pO[8].pOya_parts = &pO2->Be_flg;
	pO[8].Pos_x = 200;
	pO[8].Pos_y = 0x390;
	pO[8].Pos_z = -200;
	*(UCHAR *)pO[8].Free = 0xff;
	pO[9].pSuper = (ULONG *)&pO[8].Workm;
	pO[9].pOya_parts = &pO[8].Be_flg;
	pO[9].Pos_x = 0;
	pO[9].Pos_y = 0x1bb;
	pO[9].Pos_z = 0;
	pO[10].pSuper = (ULONG *)pMat;
	pO[10].pOya_parts = &pO2->Be_flg;
	pO[10].Pos_x = 200;
	pO[10].Pos_y = 0x390;
	pO[10].Pos_z = 200;
	*(UCHAR *)pO[10].Free = 0xff;
	pO[0xb].pSuper = (ULONG *)&pO[10].Workm;
	pO[0xb].pOya_parts = &pO[10].Be_flg;
	pO[0xb].Pos_x = 0;
	pO[0xb].Pos_y = 0x1bb;
	pO[0xb].Pos_z = 0;
}

/* DS 0x80040edc (Plw0f_init) */
void Plw0f_init(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	PARTS_W *pO;

	pP_ptr = pEm->pSin_parts_ptr;
	pO = pP_ptr + 0x10;
	if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
		pO = pP_ptr + 0x14;
	}
	if (pEm->Id == 14) {
		pO++;
	}
	pO->pSuper = (ULONG *)&pP_ptr[0xb].Workm;
	pO->Be_flg = 0;
	pO->pOya_parts = &pP_ptr[0xb].Be_flg;
	pO->Pos_x = 0;
	pO->Pos_y = 0x96;
	pO->Pos_z = 0;
	if ((pEm->Type & 0xfff) == 0x10) {
		pO->Pos_y = 400;
		pO->Pos_x = -200;
	}
}

/* DS 0x80040f5c (Plw12_init) */
void Plw12_init(ENEMY_WORK *pEm)
{
	ULONG mtx_w;
	ULONG mtx_w1;
	long t_val;
	PARTS_W *pO;
	PARTS_W *pP_ptr;

	mtx_w1 = *(int *)&GsIDMATRIX.m[1][1];
	mtx_w = *(int *)&GsIDMATRIX.m[0][2];
	pP_ptr = pEm->pSin_parts_ptr;
	pO = pP_ptr + 0x10;
	*(ULONG *)&pP_ptr[0x10].M[0][0] = *(int *)&GsIDMATRIX.m[0][0];
	*(ULONG *)&pP_ptr[0x10].M[0][2] = mtx_w;
	*(ULONG *)&pP_ptr[0x10].M[1][1] = mtx_w1;
	t_val = GsIDMATRIX.t[0];
	mtx_w = *(int *)&GsIDMATRIX.m[2][2];
	*(ULONG *)&pP_ptr[0x10].M[2][0] = *(int *)&GsIDMATRIX.m[2][0];
	*(ULONG *)&pP_ptr[0x10].M[2][2] = mtx_w;
	pP_ptr[0x10].Pos_x = t_val;
	t_val = GsIDMATRIX.t[2];
	pP_ptr[0x10].Pos_y = GsIDMATRIX.t[1];
	pP_ptr[0x10].Pos_z = t_val;
	if (((pEm->Id & 1) == 0) || (9 < pEm->Id)) {
		if (pEm->Id == 14) {
			pO = pP_ptr + 0x11;
			pP_ptr[0x11].Pos_x = -0xd6;
			pP_ptr[0x11].Pos_y = 0x188;
			pP_ptr[0x11].Pos_z = 0;
		}
		else {
			pP_ptr[0x10].Pos_x = -0xc5;
			pP_ptr[0x10].Pos_y = 0x18b;
			pP_ptr[0x10].Pos_z = -1;
		}
	}
	else {
		pO = pP_ptr + 0x14;
		pP_ptr[0x14].Pos_x = -0xd6;
		pP_ptr[0x14].Pos_y = 0x188;
		pP_ptr[0x14].Pos_z = 0;
	}
	pO->Be_flg  = 1;
	pO->Timer0  = 0;
	pO->Timer1  = 0;
	pO->Timer2  = 0;
	pO->Timer3  = 0;
	pO->pSuper  = (ULONG *)&pP_ptr[0xb].Workm;
	pO->pOya_parts = &pP_ptr[0xb].Be_flg;
	pO->Cdir_x = 0;
	pO->Cdir_y = 0;
	pO->Cdir_z = 0;
}

/* DS 0x80041060 (Weapon_cls) */
void Weapon_cls(ENEMY_WORK *pEm)
{
	pEm->Parts_num = pEm->Parts_num - (char)(pEm->pSin_parts_ptr[0xb].pTmd[-1] >> 1);
}

/* DS 0x80041088 (Init_W_Tool) */
void Init_W_Tool(void)
{
	return;
}

/* DS 0x80041090 (W_Tool) */
void W_Tool(void)
{
	return;
}

/* DS 0x80041098 (init_Weapon_at) */
void init_Weapon_at(ENEMY_WORK *pEm, ATTACK_WEAPON_TBL *At_tbl_top)
{
	if (G.Enemy_num != 0) {
		pEm->Attw_seq_no = 0;
		pEm->Attw_timer  = At_tbl_top->At_seq_top->Timer;
	}
}

/* DS 0x800410cc (Weapon_at_ck) */
ULONG Weapon_at_ck(ENEMY_WORK *pEm, ULONG W_no, ATTACK_WEAPON_TBL *At_tbl_top)
{
	ULONG Rnd_val;	/* full Rnd() result (decompile CONCAT31(extraout_var, uVar1)) */
	UCHAR ck_flg;
	short rz_delta;
	ULONG ck_ret;
	int diff_y;
	long step;
	ATTACK_WORK3 *pAt_work;
	UCHAR *pCk_tbl;
	UINT tbl_no;
	int ck_off;
	ENEMY_WORK *pTgt;
	UINT hit_flg;
	ULONG   Frame_pad[8];		/* retail reserves 0x3c-0x7c in 8-byte slots and never reads those words */
	UINT part_mask;
	UINT Hit_parts = 0;
	ENEMY_WORK **ppTgt;
	VECTOR tgt_pt;
	long save_x;
	long save_z;
	/* Left uninitialised on purpose: DS has no store to these slots before the range test on the
	 * Set_MinMax case-7 path either (see the default branch of Set_MinMax). */
	long ck_max;
	long ck_min;
	ENEMY_WORK *pAtk;
	ULONG w_info;
	UINT best_flg;
	ATTACK_WEAPON_EM *pAwe;
	ATTACK_WEAPON_EM *pBest_awe;
	UINT best_dist;
	ENEMY_WORK *pBest_em;
	UINT best_part;
	UINT at_w;
	VECTOR *pNeck_pos;

	best_flg = 0;
	best_dist = 0x7fffffff;
	pBest_em = (ENEMY_WORK *)0x0;
	if (G.Enemy_num != 0) {
		if (pEm->Attw_timer == 0) {
			ck_flg = pEm->Attw_seq_no + 1;
			pEm->Attw_seq_no = ck_flg;
			pEm->Attw_timer  = At_tbl_top->At_seq_top[ck_flg].Timer;
		}
		tbl_no = (UINT)At_tbl_top->At_seq_top[pEm->Attw_seq_no].Tbl_no;
		if (tbl_no == 0xff) {
			pEm->Attw_timer += 0xff;
			return 0;
		}
		if (tbl_no == 0xfe) {
			pEm->Attw_seq_no = 0;
			pEm->Attw_timer  = At_tbl_top->At_seq_top->Timer;
			tbl_no = (UINT)At_tbl_top->At_seq_top[pEm->Attw_seq_no].Tbl_no;
			if (tbl_no == 0xff) {
				return 0;
			}
		}
		if (pEm->Attw_timer != 0xff) {
			ppTgt = G.pEnemy + 2;
			pAt_work   = At_tbl_top->At_work_top;
			pNeck_pos = (VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t;
			pAtk = pEm;
			w_info = W_no;
			do {
				pTgt = *ppTgt;
				if (((((pTgt->Be_flg & 1) != 0) && (pTgt->Damage_cnt == 0)) &&
					 (-1 < pTgt->Life)) &&
					((pTgt->Type & 0xc000) == 0)) {
					tgt_pt.vx = pTgt->Atd[0].X;
					tgt_pt.vy = pTgt->pSin_parts_ptr[pTgt->Neck_no].Workm.t[1];
					tgt_pt.vz = pTgt->Atd[0].Z;
					ck_ret = Sca_ck_line(pNeck_pos, &tgt_pt, 0x80, 1);
					hit_flg = 0;
					if (ck_ret == 0) {
						at_w = (UINT)pTgt->Atd[0].At_w;
						rz_delta = (short)((int)at_w >> 2);
						pAt_work[tbl_no].Half0_R_z += rz_delta;
						pAt_work[tbl_no].Half2_R_z += rz_delta;
						pAt_work[tbl_no].Half1_R_z += rz_delta;
						pAt_work[tbl_no].R0_x += (pTgt->Eff_at_r >> 2);
						save_x = pTgt->Pos_x;
						save_z = pTgt->Pos_z;
						step = pTgt->Atd[0].Z;
						pTgt->Pos_x = pTgt->Atd[0].X;
						pTgt->Pos_z = step;
						pTgt->Damage_flg &= 0xff00;
						pAwe = (ATTACK_WEAPON_EM *)
								   (&(&Pl_weapon_tbl[0x10][1].At_work_top)[pTgt->Id]->Status) +
								   (w_info >> 0x10);
						part_mask = pTgt->Be_flg >> 0x1a & 7;
						diff_y  = pTgt->Pos_y - pAtk->Pos_y;
						if (part_mask != 0) {
							if ((pAt_work[tbl_no].Ck_kin != 0) &&
								(step = Set_MinMax(pAwe, pAt_work[tbl_no].Ck_kin, &ck_max, &ck_min),
								 (UINT)(diff_y - ck_max) <= (UINT)(ck_min - ck_max))) {
								ck_flg = pAt_work[tbl_no].Ck_kin;
								ck_off = 6;
								if (ck_min + step * 2 <= diff_y) ck_off = 3;
								if (ck_min + step <= diff_y)     ck_off = 0;
								ck_ret = Attack_r_ck0((MODEL_WORK *)&pAtk->Attribute,
													 (MODEL_WORK *)&pTgt->Attribute,
													 (ATTACK_WORK *)&pAt_work[tbl_no].Off0_x);
								if (ck_ret != 0) {
									pCk_tbl = W_at_ck[(ck_flg & 8) != 0] + ck_off;
									if ((pCk_tbl[2] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[2] >> 1); hit_flg = Hit_parts * 8 + 1; }
									if ((pCk_tbl[1] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[1] >> 1); hit_flg = Hit_parts * 8 + 1; }
									if ((*pCk_tbl & part_mask) != 0)   { Hit_parts = (UINT)(*pCk_tbl >> 1);   hit_flg = Hit_parts * 8 + 1; }
								}
							}
							if ((pAt_work[tbl_no].Ck_tyu != 0) &&
								(step = Set_MinMax(pAwe, pAt_work[tbl_no].Ck_tyu, &ck_max, &ck_min),
								 (UINT)(diff_y - ck_max) <= (UINT)(ck_min - ck_max))) {
								ck_flg = pAt_work[tbl_no].Ck_tyu;
								ck_off = 6;
								if (ck_min + step * 2 <= diff_y) ck_off = 3;
								if (ck_min + step <= diff_y)     ck_off = 0;
								ck_ret = Attack_r_ck0((MODEL_WORK *)&pAtk->Attribute,
													 (MODEL_WORK *)&pTgt->Attribute,
													 (ATTACK_WORK *)&pAt_work[tbl_no].Off1_x);
								if (ck_ret != 0) {
									pCk_tbl = W_at_ck[(ck_flg & 8) != 0] + ck_off;
									hit_flg = 0;
									if ((pCk_tbl[2] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[2] >> 1); hit_flg = Hit_parts * 8 + 2; }
									if ((pCk_tbl[1] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[1] >> 1); hit_flg = Hit_parts * 8 + 2; }
									if ((*pCk_tbl & part_mask) != 0)   { Hit_parts = (UINT)(*pCk_tbl >> 1);   hit_flg = Hit_parts * 8 + 2; }
								}
							}
							if ((pAt_work[tbl_no].Ck_en != 0) &&
								(step = Set_MinMax(pAwe, pAt_work[tbl_no].Ck_en, &ck_max, &ck_min),
								 (UINT)(diff_y - ck_max) <= (UINT)(ck_min - ck_max))) {
								ck_flg = pAt_work[tbl_no].Ck_en;
								ck_off = 6;
								if (ck_min + step * 2 <= diff_y) ck_off = 3;
								if (ck_min + step <= diff_y)     ck_off = 0;
								ck_ret = Attack_r_ck0((MODEL_WORK *)&pAtk->Attribute,
													 (MODEL_WORK *)&pTgt->Attribute,
													 (ATTACK_WORK *)&pAt_work[tbl_no].Off2_x);
								if (ck_ret != 0) {
									pCk_tbl = W_at_ck[(ck_flg & 8) != 0] + ck_off;
									hit_flg = 0;
									if ((pCk_tbl[2] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[2] >> 1); hit_flg = Hit_parts * 8 + 4; }
									if ((pCk_tbl[1] & part_mask) != 0) { Hit_parts = (UINT)(pCk_tbl[1] >> 1); hit_flg = Hit_parts * 8 + 4; }
									if ((*pCk_tbl & part_mask) != 0)   { Hit_parts = (UINT)(*pCk_tbl >> 1);   hit_flg = Hit_parts * 8 + 4; }
								}
							}
							pTgt->Damage_flg = pTgt->Damage_flg | (USHORT)hit_flg;
							if ((hit_flg != 0) &&
								(diff_y  = pAtk->Pos_x - pTgt->Pos_x,
								 ck_off = pAtk->Pos_z - pTgt->Pos_z,
								 part_mask = SquareRoot0(diff_y * diff_y + ck_off * ck_off),
								 part_mask < best_dist)) {
								pBest_awe = pAwe;
								best_flg = hit_flg;
								best_dist = part_mask;
								pBest_em = pTgt;
								best_part = Hit_parts;
							}
						}
						rz_delta = (short)((int)at_w >> 2);
						pAt_work[tbl_no].Half0_R_z -= rz_delta;
						pAt_work[tbl_no].Half2_R_z -= rz_delta;
						pAt_work[tbl_no].Half1_R_z -= rz_delta;
						pAt_work[tbl_no].R0_x = pAt_work[tbl_no].R0_x - (pTgt->Eff_at_r >> 2);
						pTgt->Pos_x = save_x;
						pTgt->Pos_z = save_z;
					}
				}
				ppTgt++;
			} while (ppTgt != G.ppEnemy_end);
			pAtk->Attw_timer += 0xff;
			if ((w_info & 0x8000) != 0) {
				return (ULONG)pBest_em;
			}
			if (pBest_em == (ENEMY_WORK *)0x0) {
				return 0xff;
			}
			tbl_no = (UINT)((best_flg & 2) != 0);
			if ((best_flg & 4) != 0) {
				tbl_no = 2;
			}
			if (w_info >> 0x10 == 0x12) {
				Rnd_val = Rnd();
				ck_flg = Rnd();
				if ((((UINT)Rnd_val) >> (ck_flg & 3) & 0xfU) == 0) {
					pBest_awe = pBest_awe + -0xe;
					w_info = 0x50000;
				}
				else {
					pBest_awe = pBest_awe + -0x11;
					w_info = 0x20000;
				}
			}
			ck_ret = *(ULONG *)&pBest_em->Routine_0;
			pBest_em->Life = pBest_em->Life - ((USHORT)(pBest_awe->Damage >> (tbl_no * 10 & 0x1f)) & 0x3ff);
			rz_delta = pBest_em->Life;
			pBest_em->Routine_0 = 2;
			pBest_em->Routine_1 = 0;
			pBest_em->Routine_2 = 0;
			pBest_em->Routine_3 = 0;
			pBest_em->Tmp_routine = ck_ret;
			if (rz_delta < 0) {
				pBest_em->Routine_0 = 3;
				pBest_em->Routine_1 = 0;
				pBest_em->Routine_2 = 0;
				pBest_em->Routine_3 = 0;
			}
			if ((best_part == 0) &&
				(diff_y = (UINT)(UCHAR)pBest_em->D_life_d - (pBest_awe->SubDamage >> tbl_no * 3 & 7),
				 pBest_em->D_life_d = (char)diff_y, diff_y * 0x1000000 < 0)) {
				pBest_em->D_life_d = -1;
			}
			if ((best_part == 1) &&
				(diff_y = (UINT)(UCHAR)pBest_em->D_life_c - (pBest_awe->SubDamage >> tbl_no * 3 & 7),
				 pBest_em->D_life_c = (char)diff_y, diff_y * 0x1000000 < 0)) {
				pBest_em->D_life_c = -1;
			}
			if ((best_part == 2) &&
				(diff_y = (UINT)(UCHAR)pBest_em->D_life_u - (pBest_awe->SubDamage >> tbl_no * 3 & 7),
				 pBest_em->D_life_u = (char)diff_y, diff_y * 0x1000000 < 0)) {
				pBest_em->D_life_u = -1;
			}
			rz_delta = Direction_ck((short)pAtk->Pos_x, (short)pAtk->Pos_z,
								  (short)pBest_em->Pos_x, (short)pBest_em->Pos_z);
			rz_delta = rz_delta - pBest_em->Cdir_y;
			if (((rz_delta + 0x400U) & 0xfff) < 0x800) pBest_em->Damage_flg = pBest_em->Damage_flg | 0x20;
			if (((rz_delta + 0x600U) & 0xfff) < 0x400) pBest_em->Damage_flg = pBest_em->Damage_flg | 0x40;
			if (((rz_delta - 0x200U) & 0xfff) < 0x400) pBest_em->Damage_flg = pBest_em->Damage_flg | 0x80;
			pBest_em->Damage_no = (char)tbl_no * 3 + (char)best_part;
			pBest_em->Routine_1 = (char)(w_info >> 0x10) + 1;
			ck_flg = pBest_em->Damage_cnt & 0x80;
			pBest_em->Damage_cnt = ck_flg;
			pBest_em->Damage_cnt = ck_flg | ((UCHAR)(pBest_awe->SubDamage >> ((tbl_no * 7 + 9) & 0x1f)) & 0x7f);
			return Hit_parts + 1;
		}
	}
	return 0;
}

/* DS 0x80041b20 (Set_MinMax) */
long Set_MinMax(ATTACK_WEAPON_EM *pAwe, UCHAR Ck_flg, long *pMax, long *pMin)
{
	UINT Flag;
	int bit_idx;
	int span;
	int Step = 0;

	Flag = (UINT)Ck_flg;
	bit_idx = 0;
	if ((Ck_flg & 1) == 0) {
		do {
			Flag >>= 1;
			bit_idx++;
		} while ((Flag & 1) == 0);
	}
	switch (bit_idx) {
	case 0:
		Step = ((int)pAwe->Ge_ue - (int)pAwe->Ge_ge) / 3;
		*pMax = (int)pAwe->Ge_ue + Step * -2;
		bit_idx = (int)pAwe->Ge_ge + Step * -2;
		break;
	case 1:
		Step = ((int)pAwe->Ge_ue - (int)pAwe->Ge_ge) / 3;
		*pMax = pAwe->Ge_ue - Step;
		bit_idx = pAwe->Ge_ge - Step;
		break;
	case 2:
		span = (int)pAwe->Ge_ue - (int)pAwe->Ge_ge;
		*pMax = (int)pAwe->Ge_ue;
		*pMin = (int)pAwe->Ge_ge;
		return span / 3;
	case 3:
		span = (int)pAwe->Tyu_ue - (int)pAwe->Tyu_ge;
		*pMax = (int)pAwe->Tyu_ue;
		*pMin = (int)pAwe->Tyu_ge;
		return span / 3;
	case 4:
		span = (int)pAwe->Jyo_ue - (int)pAwe->Jyo_ge;
		*pMax = (int)pAwe->Jyo_ue;
		*pMin = (int)pAwe->Jyo_ge;
		return span / 3;
	case 5:
		Step = ((int)pAwe->Jyo_ue - (int)pAwe->Jyo_ge) / 3;
		*pMax = pAwe->Jyo_ue + Step;
		bit_idx = pAwe->Jyo_ge + Step;
		break;
	case 6:
		Step = ((int)pAwe->Jyo_ue - (int)pAwe->Jyo_ge) / 3;
		*pMax = (int)pAwe->Jyo_ue + Step * 2;
		bit_idx = (int)pAwe->Jyo_ge + Step * 2;
		break;
	default:
		/* Ck_flg == 0x80 (bit 7 only) is reachable: Pl_w010[0].Ck_kin is 0x80.
		 * DS returns its input register t1 here, which Weapon_at_ck never
		 * writes and whose value depends on the caller chain. pMax/pMin are
		 * left unwritten on this path in DS too, so the range test reads stale stack there.
		 * UNMATCHED: DS returns t1 (undefined; the caller's register). Step (0) is kept, and this
		 * path cannot be reproduced in C. */
		return Step;
	}
	*pMin = bit_idx;
	return Step;
}

/* DS 0x80041ce4 (Attack_r_ck0) */
ULONG Attack_r_ck0(MODEL_WORK *pOm, MODEL_WORK *pPm, ATTACK_WORK *pAt)
{
	ULONG hit;
	short (*m0)[3];
	SVECTOR sv_in;
	SVECTOR sv_p0;
	SVECTOR sv_p1;
	UINT base_x;
	short base_z;
	int pt_a_x;
	short pt_a_z;
	int pt_b_x;
	short pt_b_z;
	MATRIX mtx_at;
	MATRIX mtx_comp;

	m0 = pOm->M;
	mtx_at.t[0] = (int)(pAt->Off_x);
	mtx_at.m[0][0] = 0x1000;
	mtx_at.m[0][1] = 0;
	mtx_at.m[0][2] = 0;
	mtx_at.m[1][0] = 0;
	mtx_at.m[1][1] = 0x1000;
	mtx_at.m[1][2] = 0;
	mtx_at.m[2][0] = 0;
	mtx_at.m[2][1] = 0;
	*(int *)&mtx_at.m[2][2] = 0x1000;
	mtx_at.t[1] = 0;
	mtx_at.t[2] = pAt->Half_R_z * -4 - (int)pAt->Off_z;
	CompM((MATRIX *)m0, &mtx_at, &mtx_comp);
	base_x = (int)mtx_comp.t[0] >> 2 & 0xffff;
	base_z = (short)((int)mtx_comp.t[2] >> 2);
	*(int *)&sv_in.vx = (int)(pAt->R_x);
	sv_in.vz = 0;
	sv_in.pad = 0;
	ApplyMatrixSV((MATRIX *)m0, &sv_in, &sv_p0);
	sv_in.vx = 0;
	sv_in.vy = 0;
	*(int *)&sv_in.vz = (int)pAt->Half_R_z << 1;
	ApplyMatrixSV((MATRIX *)m0, &sv_in, &sv_p1);
	*(int *)&mtx_at.m[0][0] = (int)(sv_p0.vx);
	*(short *)&pt_a_x = (short)base_x + sv_p0.vx;
	pt_a_z = base_z + sv_p0.vz;
	*(short *)&pt_b_x = (short)base_x + sv_p1.vx;
	pt_b_z = base_z + sv_p1.vz;
	*(int *)&mtx_at.m[1][1] = (UINT)(USHORT)sv_p1.vz << 0x10;
	base_z = (short)(pPm->Pos_z >> 2) - base_z;
	base_x = (pPm->Pos_x >> 2) - (int)(short)base_x;
	pt_a_z = (short)(pPm->Pos_z >> 2) - pt_a_z;
	pt_a_x = (pPm->Pos_x >> 2) - (int)(short)pt_a_x;
	mtx_at.m[1][0] = sv_p1.vx;
	pt_b_x = (pPm->Pos_x >> 2) - (int)(short)pt_b_x;
	*(int *)&mtx_comp.m[0][2] = (int)((short)pt_b_x);
	pt_b_z = (short)(pPm->Pos_z >> 2) - pt_b_z;
	mtx_comp.m[0][0] = (short)base_x;
	mtx_comp.m[0][1] = (short)pt_a_x;
	mtx_comp.m[1][1] = 0;
	mtx_comp.m[1][2] = 0;
	mtx_comp.m[2][0] = base_z;
	mtx_comp.m[2][1] = pt_a_z;
	mtx_comp.m[2][2] = pt_b_z;
	MulMatrix0((MATRIX *)&mtx_at, (MATRIX *)&mtx_comp, (MATRIX *)&mtx_at);
	hit = 0;
	if (((int)mtx_at.m[0][0] & 0x8000U) != ((int)mtx_at.m[0][1] & 0x8000U)) {
		hit = (ULONG)(((int)mtx_at.m[1][0] & 0x8000U) != ((int)mtx_at.m[1][2] & 0x8000U));
	}
	return hit;
}

/* DS 0x80041ef8 (Attack_r_ck1) */
ULONG Attack_r_ck1(VECTOR *pH, short Cdir_y, VECTOR *pT, ATTACK_WORK *pAt)
{
	ULONG hit;
	SVECTOR sv_in;
	SVECTOR sv_p0;
	SVECTOR sv_p1;
	UINT base_x;
	short base_z;
	int pt_a_x;
	short pt_a_z;
	int pt_b_x;
	short pt_b_z;
	MATRIX mtx_at;
	MATRIX mtx_comp;
	MATRIX mtx_rot;
	SVECTOR rot_vec[2];

	mtx_at.t[0] = (int)(pAt->Off_x);
	mtx_at.t[1] = 0;
	mtx_at.m[0][0] = 0x1000;
	mtx_at.m[0][1] = 0;
	mtx_at.m[0][2] = 0;
	mtx_at.m[1][0] = 0;
	mtx_at.m[1][1] = 0x1000;
	mtx_at.m[1][2] = 0;
	mtx_at.m[2][0] = 0;
	mtx_at.m[2][1] = 0;
	*(int *)&mtx_at.m[2][2] = 0x1000;
	rot_vec[0].vx = 0;
	rot_vec[0].vz = 0;
	mtx_at.t[2] = pAt->Half_R_z * -4 - (int)pAt->Off_z;
	rot_vec[0].vy = Cdir_y;
	RotMatrix(rot_vec, (MATRIX *)&mtx_rot);
	mtx_rot.t[0] = pH->vx;
	mtx_rot.t[1] = 0;
	mtx_rot.t[2] = pH->vz;
	CompM(&mtx_rot, &mtx_at, &mtx_comp);
	base_x = (int)mtx_comp.t[0] >> 2 & 0xffff;
	base_z = (short)((int)mtx_comp.t[2] >> 2);
	*(int *)&sv_in.vx = (int)(pAt->R_x);
	sv_in.vz = 0;
	sv_in.pad = 0;
	ApplyMatrixSV((MATRIX *)&mtx_rot, &sv_in, &sv_p0);
	sv_in.vx = 0;
	sv_in.vy = 0;
	*(int *)&sv_in.vz = (int)pAt->Half_R_z << 1;
	ApplyMatrixSV((MATRIX *)&mtx_rot, &sv_in, &sv_p1);
	*(int *)&mtx_at.m[0][0] = (int)(sv_p0.vx);
	*(short *)&pt_a_x = (short)base_x + sv_p0.vx;
	pt_a_z = base_z + sv_p0.vz;
	*(short *)&pt_b_x = (short)base_x + sv_p1.vx;
	pt_b_z = base_z + sv_p1.vz;
	*(int *)&mtx_at.m[1][1] = (UINT)(USHORT)sv_p1.vz << 0x10;
	base_z = (short)(pT->vz >> 2) - base_z;
	base_x = (pT->vx >> 2) - (int)(short)base_x;
	pt_a_z = (short)(pT->vz >> 2) - pt_a_z;
	pt_a_x = (pT->vx >> 2) - (int)(short)pt_a_x;
	mtx_at.m[1][0] = sv_p1.vx;
	pt_b_x = (pT->vx >> 2) - (int)(short)pt_b_x;
	*(int *)&mtx_comp.m[0][2] = (int)((short)pt_b_x);
	pt_b_z = (short)(pT->vz >> 2) - pt_b_z;
	mtx_comp.m[0][0] = (short)base_x;
	mtx_comp.m[0][1] = (short)pt_a_x;
	mtx_comp.m[1][1] = 0;
	mtx_comp.m[1][2] = 0;
	mtx_comp.m[2][0] = base_z;
	mtx_comp.m[2][1] = pt_a_z;
	mtx_comp.m[2][2] = pt_b_z;
	MulMatrix0((MATRIX *)&mtx_at, (MATRIX *)&mtx_comp, (MATRIX *)&mtx_at);
	hit = 0;
	if (((int)mtx_at.m[0][0] & 0x8000U) != ((int)mtx_at.m[0][1] & 0x8000U)) {
		hit = (ULONG)(((int)mtx_at.m[1][0] & 0x8000U) != ((int)mtx_at.m[1][2] & 0x8000U));
	}
	return hit;
}

/* DS 0x80042144 (Em_ck) */
ULONG Em_ck(ENEMY_WORK *pEm, ULONG LL)
{
	int dx;
	int dz;
	UINT dist;
	ULONG ret;
	ENEMY_WORK *pTgt;
	ENEMY_WORK **ppTgt;
	UINT min_dist;
	UINT min_dist_sp;
	ENEMY_WORK *pNear_sp;
	ENEMY_WORK *pNear;
	VECTOR tgt_pt;

	pNear = (ENEMY_WORK *)0x0;
	min_dist_sp = 0x7fffffff;
	pNear_sp = (ENEMY_WORK *)0x0;
	if (G.Enemy_num == 0) {
		pEm->pEnemy_ptr = (ULONG *)pEm;
		ret = 0;
	}
	else {
		ppTgt = G.pEnemy + 2;
		min_dist = 0x7fffffff;
		if (LL != 0) {
			min_dist = LL;
		}
		do {
			pTgt = *ppTgt;
			if (((pTgt->Be_flg & 1) != 0) && ((pTgt->Type & 0xc000) == 0)) {
				dx = pEm->Pos_x - pTgt->Pos_x;
				dz = pEm->Pos_z - pTgt->Pos_z;
				dist = SquareRoot0(dx * dx + dz * dz);
				tgt_pt.vx = pTgt->Atd[0].X;
				tgt_pt.vy = pTgt->pSin_parts_ptr[pTgt->Neck_no].Workm.t[1];
				tgt_pt.vz = pTgt->Atd[0].Z;
				if ((-1 < pTgt->Life) &&
					(ret = Sca_ck_line(
								 (VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
								 &tgt_pt, 0x80, 1),
					 ret == 0)) {
					if ((pTgt->Type & 0x2000) == 0) {
						if (dist < min_dist) { min_dist = dist; pNear = pTgt; }
					}
					else if (dist < min_dist_sp) { min_dist_sp = dist; pNear_sp = pTgt; }
				}
			}
			ppTgt++;
		} while (ppTgt != G.ppEnemy_end);
		if (pNear == (ENEMY_WORK *)0x0 && pNear_sp == (ENEMY_WORK *)0x0) {
			pEm->pEnemy_ptr = (ULONG *)pEm;
			return 0;
		}
		pEm->pEnemy_ptr = (ULONG *)((pNear != (ENEMY_WORK *)0x0) ? pNear : pNear_sp);
		ret = 1;
	}
	return ret;
}

/* DS 0x80042368 (Hand_ck) */
void Hand_ck(ENEMY_WORK *pEm, long Ck_pos_y, short Type)
{
	int tbl_off;
	UINT Be_flg;
	int Base_pos_y;
	UINT pos_flg;

	pEm->Be_flg &= 0x1fffffff;
	Base_pos_y = (int)*(short *)(W_at_ck[2] + (char)Mw_ck[pEm->Id] * 2 + Type * 8 + 6) + pEm->Pos_y;
	if ((Type == 0xd) && (pEm->Id == 0xe)) {
		Base_pos_y = (int)Ghand_len[0xc][2] + pEm->Pos_y;
	}
	tbl_off = (int)((UINT)(USHORT)Type << 0x10) >> 0xd;
	Base_pos_y = Base_pos_y * pEm->Parts0_pos_y >> 0xc;
	if (Ck_pos_y < Base_pos_y + *(short *)(W_at_ck[2] + tbl_off + 2)) {
		pos_flg = 0x80000000;
	}
	else if (Base_pos_y + *(short *)(W_at_ck[2] + tbl_off + 4) < Ck_pos_y) {
		pos_flg = 0x20000000;
	}
	else {
		pos_flg = 0x40000000;
	}
	Be_flg = pEm->Be_flg | pos_flg;
	pEm->Be_flg = Be_flg;
}

/* DS 0x80042470 (Mag_set) */
void Mag_set(ENEMY_WORK *pEm)
{
	signed char spd_y;
	UCHAR mag_flg;
	int Type;
	PARTS_W *pP2_ptr;
	PARTS_W *pP_ptr;
	MATRIX Tm;
	SVECTOR Ts;
	SVECTOR Ts0;

	if (((pEm->Type & 0xfff) != 0) &&
		(Type = Mw_ck[(pEm->Type & 0xfff) + 0xf] - 1,
		 Mw_ck[(pEm->Type & 0xfff) + 0xf] != 0)) {
		pP_ptr = pEm->pSin_parts_ptr + Pl_mag[Type].Parts_no;
		if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
			pP_ptr += 4;
		}
		if ((pEm->Id == 14) || (pEm->Id == 65)) {
			pP_ptr++;
		}
		pP2_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x41;
		spd_y = Pl_mag[Type].V_y;
		pP_ptr->Timer1 = 1;
		pP_ptr->Timer2 = 0;
		pP_ptr->Timer0 = (short)spd_y;
		mag_flg = Pl_mag[Type].Flg;
		pP_ptr->Cdir_x = 0;
		pP_ptr->Cdir_y = 0;
		pP_ptr->Cdir_z = 0;
		pP_ptr->Timer3 = (USHORT)mag_flg;
		RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
		CompM(&pP2_ptr[0xb].Workm, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
		pP_ptr->Old_x = pEm->Ground;
		Ts.vx = 0;
		Ts.vz = 0;
		Ts.vy = Pl_mag[Type].Dir + pEm->Cdir_y;
		RotMatrix(&Ts, &Tm);
		Ts.vy = 0;
		Ts.vx = (short)Pl_mag[Type].V_xz;
		ApplyMatrixSV(&Tm, &Ts, &Ts0);
		pP_ptr->Old_y = Ts0.vx;
		pP_ptr->Old_z = Ts0.vz;
	}
}

/* DS 0x80042644 (Mag_down) */
void Mag_down(ENEMY_WORK *pEm)
{
	short old_pos;
	UCHAR rnd_b;
	short spd;
	int wk_pos;
	PARTS_W *pP_ptr;

	if (((pEm->Type & 0xfff) != 0) && (Mw_ck[(pEm->Type & 0xfff) + 0xf] != 0)) {
		pP_ptr = pEm->pSin_parts_ptr + Pl_mag[Mw_ck[(pEm->Type & 0xfff) + 0xf] - 1].Parts_no;
		if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
			pP_ptr += 4;
		}
		if ((pEm->Id == 14) || (pEm->Id == 65)) {
			pP_ptr++;
		}
		if ((pP_ptr->Be_flg & 1) != 0) {
			wk_pos = pP_ptr->Workm.t[1];
			old_pos = pP_ptr->Old_x;
			spd = pP_ptr->Timer0 + 0x14;
			pP_ptr->Timer0 = spd;
			wk_pos += spd;
			pP_ptr->Workm.t[1] = wk_pos;
			if (old_pos < wk_pos) {
				if (pP_ptr->Timer1 == 0) {
					pP_ptr->Be_flg &= 0xfffffffe;
				}
				else {
					pP_ptr->Workm.t[1] = (int)old_pos;
					wk_pos = (UINT)(USHORT)pP_ptr->Timer0 << 0x10;
					pP_ptr->Timer1 = pP_ptr->Timer1 + -1;
					pP_ptr->Timer0 = -(short)(((wk_pos >> 0x10) - (wk_pos >> 0x1f)) >> 1);
					if (((pP_ptr->Timer3 & 2U) != 0) && (rnd_b = Rnd(), (rnd_b & 1) != 0)) {
						pP_ptr->Old_y = -pP_ptr->Old_y;
						pP_ptr->Old_z = -pP_ptr->Old_z;
					}
					Snd_se_stad(0x10e0001, (VECTOR *)(pP_ptr->Workm).t);
				}
			}
			if (pP_ptr->Timer1 != 0) {
				if ((pP_ptr->Timer3 & 1U) != 0) {
					pP_ptr->Cdir_x = 0x100;
					RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
					MulMatrix2((MATRIX *)pP_ptr->M, (MATRIX *)&pP_ptr->Workm);
					old_pos = pP_ptr->Old_z;
					wk_pos = pP_ptr->Workm.t[2];
					pP_ptr->Workm.t[0] = pP_ptr->Workm.t[0] + (int)pP_ptr->Old_y;
					pP_ptr->Workm.t[2] = wk_pos + old_pos;
				}
				if (pP_ptr->Timer1 != 0) {
					return;
				}
			}
			pP_ptr->Cdir_x = 0x100;
			RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
			MulMatrix2((MATRIX *)pP_ptr->M, (MATRIX *)&pP_ptr->Workm);
			old_pos = pP_ptr->Old_z;
			wk_pos = pP_ptr->Workm.t[2];
			pP_ptr->Workm.t[0] = pP_ptr->Workm.t[0] + (int)pP_ptr->Old_y;
			pP_ptr->Workm.t[2] = wk_pos + old_pos;
		}
	}
}

/* DS 0x800428a0 (G_rot_st) */
void G_rot_st(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr + 0x14;
	if (pEm->Id == 14) {
		pP_ptr = pEm->pSin_parts_ptr + 0x11;
	}
	pP_ptr->Timer0 = 0;
	pP_ptr->Timer2 = 0;
	pP_ptr->Timer3 = 0;
}

/* DS 0x800428cc (G_rot) */
void G_rot(ENEMY_WORK *pEm)
{
	USHORT rot_z;
	PARTS_W *pP_ptr;

	if ((pEm->Type & 0xfff) - 9U < 3) {
		pP_ptr = pEm->pSin_parts_ptr + 0x14;
		if (pEm->Id == 14) {
			pP_ptr = pEm->pSin_parts_ptr + 0x11;
		}
		if (pP_ptr->Timer3 != 0xff) {
			rot_z = G_rot_data_z[(signed char)pP_ptr->Timer2];
			pP_ptr->Cdir_z = rot_z;
			if (rot_z == 0x8000) {
				pP_ptr->Cdir_z = 0;
				pP_ptr->Timer3 = 0xff;
			}
			else {
				pP_ptr->Timer2++;
				RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
			}
		}
	}
}

/* DS 0x8004297c (Gat_lp_st) */
void Gat_lp_st(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr + 0x10;
	if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
		pP_ptr = pEm->pSin_parts_ptr + 0x14;
	}
	if (pEm->Id == 14) {
		pP_ptr++;
	}
	pP_ptr->Timer2 = 1;
}

/* DS 0x800429c0 (Gat_rot) */
void Gat_rot(ENEMY_WORK *pEm)
{
	USHORT dec_timer;
	short inc_timer;
	PARTS_W *pP_ptr;

	if ((pEm->Type & 0xfff) == 0x12) {
		pP_ptr = pEm->pSin_parts_ptr + 0x10;
		if (((pEm->Id & 1) != 0) && (pEm->Id < 10)) {
			pP_ptr = pEm->pSin_parts_ptr + 0x14;
		}
		if (pEm->Id == 14) {
			pP_ptr++;
		}
		if (pP_ptr->Timer2 == 0) {
			dec_timer = pP_ptr->Timer3 - 8;
			pP_ptr->Timer3 = dec_timer;
			if ((int)((UINT)dec_timer << 0x10) < 0) {
				pP_ptr->Timer3 = 0;
			}
		}
		else {
			inc_timer = pP_ptr->Timer3 + 0x20;
			pP_ptr->Timer3 = inc_timer;
			if (0x200 < inc_timer) {
				pP_ptr->Timer3 = 0x200;
			}
		}
		pP_ptr->Cdir_y += pP_ptr->Timer3;
		RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
		pP_ptr->Timer2 = 0;
	}
}

/* DS 0x80042aac (Pl_water) */
void Pl_water(ENEMY_WORK *pEm)
{
	ULONG rnd_a;	/* full Rnd() results (decompile CONCAT31(extraout_var*, uVarN)) */
	ULONG rnd_b;
	PARTS_W *pP_ptr;
	SVECTOR water_pos;

	pP_ptr = pEm->pSin_parts_ptr;
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
	water_pos.vx = (short)pP_ptr->Workm.t[0];
	water_pos.vy = pEm->Water;
	water_pos.vz = (short)pP_ptr->Workm.t[2];
	rnd_a = Rnd();
	rnd_b = Rnd();
	Esp_call((((UINT)rnd_a) * 4 + 0x818U) | 0x1a000000,
			 (short)(((UINT)(USHORT)pEm->Cdir_y + ((UINT)rnd_b) * -4) * 0x10000 >> 0x10),
			 (MATRIX *)&GsIDMATRIX, &water_pos);
	rnd_a = Rnd();
	rnd_b = Rnd();
	Esp_call((((UINT)rnd_a) * 4 + 0x818U) | 0x1a000000,
			 (short)(((UINT)(USHORT)pEm->Cdir_y - (((UINT)rnd_b) * 4 + -0x400)) * 0x10000 >> 0x10),
			 (MATRIX *)&GsIDMATRIX, &water_pos);
	rnd_a = Rnd();
	rnd_b = Rnd();
	Esp_call((((UINT)rnd_a) * 4 + 0xc00U) | 0x1a000000,
			 (short)(((UINT)(USHORT)pEm->Cdir_y - (((UINT)rnd_b) * 4 + -0x800)) * 0x10000 >> 0x10),
			 (MATRIX *)&GsIDMATRIX, &water_pos);
	rnd_a = Rnd();
	rnd_b = Rnd();
	Esp_call((((UINT)rnd_a) * 4 + 0xc00U) | 0x1a000000,
			 (short)(((UINT)(USHORT)pEm->Cdir_y - (((UINT)rnd_b) * 4 + -0xc00)) * 0x10000 >> 0x10),
			 (MATRIX *)&GsIDMATRIX, &water_pos);
}

/* DS 0x80042c20 (pl_mv05_pad) */
void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	return;
}

/* DS 0x80042c28 (pl_mv05_w0) */
void pl_mv05_w0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w0_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

/* DS 0x800a6f90 (pl_mv05_w_tbl, index Type & 0xfff): ROM words 0x800a6f90..0x800a6fdb.
 * [0] is the word 0x00000002 (not a routine); [1..18] repeat the pl_mv05_t2_init[0..17] entries (0x800a6f94..). */
PL_WEP_FN pl_mv05_w_tbl[19] = {
	(PL_WEP_FN)0x00000002,
	pl_w01_t2_init,	/* [1] 0x80043ecc */
	pl_w02_t2_init,	/* [2] 0x80043ed4 */
	pl_w03_t2_init,	/* [3] 0x80043efc */
	pl_w04_t2_init,	/* [4] 0x80043f24 */
	pl_w05_t2_init,	/* [5] 0x80043f2c */
	pl_w06_t2_init,	/* [6] 0x80043f54 */
	pl_w07_t2_init,	/* [7] 0x80043f7c */
	pl_w08_t2_init,	/* [8] 0x80043fa4 */
	pl_w09_t2_init,	/* [9] 0x80043fcc */
	pl_w0a_t2_init,	/* [10] 0x80043ff4 */
	pl_w0b_t2_init,	/* [11] 0x8004401c */
	pl_w0c_t2_init,	/* [12] 0x80044044 */
	pl_w0d_t2_init,	/* [13] 0x800440cc */
	pl_w0e_t2_init,	/* [14] 0x800440f4 */
	pl_w0f_t2_init,	/* [15] 0x800440fc */
	pl_w10_t2_init,	/* [16] 0x80044104 */
	pl_w11_t2_init,	/* [17] 0x8004410c */
	pl_w12_t2_init,	/* [18] 0x80044134 */
};

/* DS 0x80042c64 (pl_mv05_w0_t2) */
void pl_mv05_w0_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG rnd_r;
	ULONG ck_ret;
	UINT be_masked;
	short (*m0)[3];
	PARTS_W *pP_ptr;
	ULONG mv_w0;
	ULONG mv_w1;
	USHORT mv_w2;
	MATRIX mtx_acc;
	SVECTOR spd_vec;
	ATTACK_WORK atk_wk;

	mv_w0 = Pl_mvtbl0_w0;
	mv_w1 = Pl_mvtbl0_w1;
	mv_w2 = Pl_mvtbl0_w2;
	*(int *)&atk_wk.Off_x = Pl_mvtbl1_w0;
	*(int *)&atk_wk.R_x = Pl_mvtbl1_w1;
	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(int *)&pEm->Move_no =
			((short *)&mv_w0)[(UINT)(pEm->Status_flg >> 0xd)] + 0x70000;
		pl_mv05_w_tbl[pEm->Type & 0xfff](pEm);
		pEm->Timer3 = 0;
		pEm->Timer2 = 0;
		pEm->Timer1 = 0;
		be_masked = pEm->Be_flg & 0x1fffffff;
		pEm->Be_flg = be_masked;
		pEm->Be_flg = be_masked | (pEm->Status_flg & 0xe000) << 0x10;
	}
	if (((Key & 0x100) == 0) && ((Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][2] & 0x7f) < pEm->Move_cnt)) {
		pEm->Routine_2 = 1;
		pEm->Routine_3 = 0;
	}
	else {
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			pEm->Routine_2 = 1;
			pEm->Routine_3 = 0;
		}
		if (((Key & 0x100) == 0) && (0x14 < pEm->Move_cnt)) {
			pEm->Routine_2 = 1;
			pEm->Routine_3 = 0;
			pEm->Spd_x = 0;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
		}
		else {
			m0    = pEm->M;
			pP_ptr = pEm->pSin_parts_ptr;
			RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)m0);
			spd_vec.vx = 0;
			spd_vec.vy = 0;
			spd_vec.vz = (pEm->Move_cnt - 5) * -0xf + 300;
			ApplyMatrixSV((MATRIX *)m0, &spd_vec, &spd_vec);
			CompM((MATRIX *)m0,   (MATRIX *)pP_ptr->M,       (MATRIX *)&mtx_acc);
			CompM((MATRIX *)&mtx_acc, (MATRIX *)pP_ptr[9].M,  (MATRIX *)&mtx_acc);
			CompM((MATRIX *)&mtx_acc, (MATRIX *)pP_ptr[10].M, (MATRIX *)&mtx_acc);
			CompM((MATRIX *)&mtx_acc, (MATRIX *)pP_ptr[0xb].M,(MATRIX *)&mtx_acc);
			if (pEm->Water != 0) {
				if (((pEm->Water < pEm->Ground) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
					Pl_water(pEm);
				}
				if (pEm->Water < mtx_acc.t[1]) {
					rnd_r = Rnd();
					Esp_call((((UINT)rnd_r) * 4 + 0x60cU) | 0x1a000000,
							 pEm->Cdir_y, (MATRIX *)&mtx_acc, (SVECTOR *)0x0);
					if (pEm->Timer2 == 0) {
						pEm->Timer2 = 1;
						Snd_se_stad(0x1070001, (VECTOR *)&pEm->Pos_x);
					}
				}
			}
			ck_ret = 0;
			if (pEm->Move_cnt - 7U < 5) {
				mtx_acc.t[1] += 200;
				ck_ret = 0x20001;
				if ((pEm->Status_flg & 0x2000) != 0) {
					ck_ret = 0x60001;
				}
				ck_ret = Pos_em_at_ck((VECTOR *)mtx_acc.t, pEm->Cdir_y, &atk_wk, ck_ret);
				if (ck_ret != 0) {
					pEm->Sce_free2 = (short)ck_ret;
					pEm->Sce_free3 = (short)(ck_ret >> 0x10);
				}
				mtx_acc.t[1] = mtx_acc.t[1] + -200;
			}
			mtx_acc.t[0] = mtx_acc.t[0] + (int)spd_vec.vx;
			mtx_acc.t[2] += spd_vec.vz;
			Sca_get_ground((VECTOR *)mtx_acc.t, 10, 0x2080, 0);
			if ((pEm->Move_cnt - 7U < 6) && (Ground_in_flg != 0)) {
				pEm->Routine_2 = 6;
				pEm->Routine_3 = 0;
				pEm->Move_no   = 16;
				pEm->Move_cnt  = 0;
				pEm->Hokan_flg = 3;
				pEm->Mplay_flg = 0;
				Joint_move(pEm, Kan, Seq, 0x400);
				Snd_se_stad(0x1000001, (VECTOR *)&pEm->Pos_x);
				Pad_vib_set0(6, 0);
				Pad_vib_set1(3, 200, 0);
			}
			else {
				if ((ck_ret != 0) && (ck_ret != 0xff)) {
					Snd_se_stad(0x1060001, (VECTOR *)&pEm->Pos_x);
					pEm->Timer3 = 2;
					Pad_vib_set1(8, 0xa0, 0);
				}
				if ((pEm->Timer3 != 0) && ((**(UINT **)&pP_ptr->Free[2] & 0x10000) == 0)) {
					spd_vec.vx = 200;
					spd_vec.vy = 200;
					spd_vec.vz = 0;
					Esp_call((**(UINT **)&pEm->Sce_free2 >> 0x13 & 0x18) << 0x10 | 0xf38,
							 pEm->Cdir_y, &pEm->pSin_parts_ptr[0xb].Workm, (SVECTOR *)&spd_vec);
					pEm->Timer3 = pEm->Timer3 + -1;
				}
				if ((pEm->Id == 13) && ((pEm->Move_cnt & 7) == 0)) {
					pP_ptr = pEm->pSin_parts_ptr;
					rnd_r = Rnd();
					spd_vec.vx = (short)((UINT)rnd_r) * -2 + 0x100;
					rnd_r = Rnd();
					spd_vec.vy = (short)((UINT)rnd_r) * -2 + -0x2e8;
					rnd_r = Rnd();
					spd_vec.vz = (short)((UINT)rnd_r) * -2 + 0x100;
					rnd_r = Rnd();
					Esp_call(0x1262000,
							 (short)(((UINT)(USHORT)pEm->Cdir_y -
									  (((UINT)rnd_r) * 2 + -0x900)) * 0x10000 >> 0x10),
							 &pP_ptr->Workm, (SVECTOR *)&spd_vec);
				}
				pl_mv05_t2_init[(pEm->Type & 0xfff) + 0x12](pEm);
			}
		}
	}
}

/* DS 0x800431f8 (pl_mv05_w0_t6) */
void pl_mv05_w0_t6(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG ck_ret;

	ck_ret = Joint_move(pEm, Kan, Seq, 0x400);
	if (ck_ret != 0) {
		pEm->Routine_2 = 1;
		pEm->Routine_3 = 0;
	}
}

/* DS 0x80043230 (pl_mv05_w1) */
void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w1_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

/* DS 0x8004326c (pl_mv05_w1_t0) */
void pl_mv05_w1_t0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	UCHAR sub_st;
	ULONG rnd_r;
	UINT wep_type;
	ULONG lim;
	long Vec_x;
	ENEMY_WORK *pTgt;
	long Vec_z;
	short Add_dir;
	PARTS_W *pP_ptr;
	SVECTOR esp_vec;

	sub_st = pEm->Routine_3;
	if (sub_st != 1) {
		if (1 < sub_st) {
			if (sub_st != 2) {
				return;
			}
			pEm->Routine_2 = 1;
			pEm->Routine_3 = 0;
			return;
		}
		if (sub_st != 0) {
			return;
		}
		pEm->Routine_3 = 1;
		pEm->Move_no   = 9;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
		if ((pEm->Type & 0xfff) == 1) {
			if ((*(int *)&G.SaveData.Stage_no == 0xa0003) || (*(int *)&G.SaveData.Stage_no == 0x40006)) {
				lim = 30000;
			}
			else {
				lim = 5000;
			}
		}
		else {
			lim = 0;
		}
		Em_ck(pEm, lim);
		pEm->Timer3 = 0;
		pEm->Spd_x  = 0;
		pEm->Neck_flg |= 0x12;
	}
	if (pEm->Water != 0) {
		if (((pEm->Water < pEm->Ground) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		pP_ptr = pEm->pSin_parts_ptr;
		esp_vec.vx = 0;
		esp_vec.vy = 300;
		esp_vec.vz = 0;
		if ((int)pEm->Water < pP_ptr[0xb].Workm.t[1] + 300) {
			rnd_r = Rnd();
			Esp_call((((UINT)rnd_r) * 4 + 0x60cU) | 0x1a000000,
					 pEm->Cdir_y, &pP_ptr[0xb].Workm, &esp_vec);
		}
		pP_ptr = pEm->pSin_parts_ptr;
		if ((int)pEm->Water < pP_ptr[0xe].Workm.t[1] + 300) {
			rnd_r = Rnd();
			Esp_call((((UINT)rnd_r) * 4 + 0x60cU) | 0x1a000000,
					 pEm->Cdir_y, &pP_ptr[0xe].Workm, &esp_vec);
		}
	}
	if ((((G.SaveData.Weapon_mugen & 1) == 0) && ((G.System_flg & 0x840) != 0x840)) &&
		(pTgt = (ENEMY_WORK *)pEm->pEnemy_ptr, pTgt != pEm)) {
		if ((pEm->Type & 0xfff) == 0xd) {
			Vec_x   = pTgt->Atd[0].X;
			Vec_z   = pTgt->Atd[0].Z;
			Add_dir = 0x120;
		}
		else {
			Vec_x   = pTgt->Atd[0].X;
			Vec_z   = pTgt->Atd[0].Z;
			Add_dir = 0xd8;
		}
		Goto00(pEm, Vec_x, Vec_z, Add_dir);
	}
	if (((G.Key_trg & 0x20) != 0) && (pEm->Timer3 == 0)) {
		Em_ck(pEm, 3000);
		pEm->Timer3 = 1;
	}
	if ((Key & 8) != 0) pEm->Cdir_y = pEm->Cdir_y + -0x20;
	if ((Key & 2) != 0) pEm->Cdir_y = pEm->Cdir_y + 0x20;
	lim = Joint_move(pEm, Kan, Seq, 0x200);
	pEm->Routine_3 = pEm->Routine_3 + (char)lim;
	wep_type = pEm->Type & 0xfff;
	if ((wep_type != 0xe) && (wep_type != 0x11)) {
		if (((Pl_weapon_kyan[wep_type - 1][0] & 0x7f) < pEm->Move_cnt) && ((Key & 0x20) != 0)) {
			pEm->Routine_2  = 1;
			pEm->Routine_3  = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
		}
		else if (((Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][1] & 0x7f) < pEm->Move_cnt) && ((Key & 0x10) != 0)) {
			pEm->Routine_2  = 1;
			pEm->Routine_3  = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
		}
	}
}

/* DS 0x8004362c (pl_mv05_w1_t1) */
void pl_mv05_w1_t1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG reload_ck;
	ULONG tama_ck;
	USHORT mv_tbl[5];

	*(ULONG *)&mv_tbl[0] = Pl_mvtbl2_w0;
	*(ULONG *)&mv_tbl[2] = Pl_mvtbl2_w1;
	mv_tbl[4] = Pl_mvtbl2_w2;
	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(int *)&pEm->Move_no = (short)mv_tbl[(UINT)(pEm->Status_flg >> 0xd)] + 0x70000;
		G.Room_player_flg |= 0x18;
		if ((pEm->Type & 0xfff) == 1) {
			G.Room_player_flg &= 0xffef;
		}
	}
	if ((Key & 0x100) == 0) {
		pEm->Routine_2 = 3;
		pEm->Routine_3 = 0;
		return;
	}
	if ((pEm->Type & 0xfff) == 0x11) {
		G.Status_flg |= 0x40000;
	}
	else {
		if (((Key & 0x10) != 0) && ((pEm->Status_flg & 0x8000) == 0)) {
			pEm->Routine_3  = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			return;
		}
		if (((Key & 0x20) != 0) && ((pEm->Status_flg & 0x2000) == 0)) {
			pEm->Routine_3  = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			return;
		}
		if (((Key & 0x30) == 0) && ((pEm->Status_flg & 0x4000) == 0)) {
			pEm->Routine_3  = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
			return;
		}
	}
	if ((((G.SaveData.Weapon_mugen & 1) == 0) && ((G.System_flg & 0x840) != 0x840)) &&
		((G.Padd1_trg & 4) != 0)) {
		pEm->Routine_2 = 5;
		pEm->Routine_3 = 0;
	}
	if ((Key & 0x40) != 0) {
		tama_ck = Tama_ck_i(pEm->Type & 0xfff);
		if (tama_ck != 0) {
			pEm->Routine_2 = 2;
			pEm->Routine_3 = 0;
			return;
		}
		if ((G.Key_trg & 0x40) != 0) {
			reload_ck = Tama_reload_ck();
			if ((((UINT)reload_ck) == 0) || ((pEm->Type & 0xfff) - 9U < 3)) {
				Snd_se_stad(0x1010001, (VECTOR *)&pEm->Pos_x);
			}
			else {
				pEm->Routine_2 = 4;
				pEm->Routine_3 = 0;
			}
		}
	}
	if ((Key & 8) != 0) pEm->Cdir_y = pEm->Cdir_y + -0x40;
	if ((Key & 2) != 0) pEm->Cdir_y = pEm->Cdir_y + 0x40;
	Joint_move(pEm, Kan, Seq, 0x200);
}

/* DS 0x80043908 (pl_mv05_w1_t2) */
void pl_mv05_w1_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG rnd_r;
	UINT jtg_idx;
	ULONG ck_ret;
	long Ck_pos_y;
	USHORT hand_tp;
	UINT wep_type;
	PARTS_W *pP_ptr;
	USHORT mv_tbl[5];
	SVECTOR esp_vec;

	*(ULONG *)&mv_tbl[0] = Pl_mvtbl0_w0;
	*(ULONG *)&mv_tbl[2] = Pl_mvtbl0_w1;
	mv_tbl[4] = Pl_mvtbl0_w2;
	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(int *)&pEm->Move_no = (short)mv_tbl[(UINT)(pEm->Status_flg >> 0xd)] + 0x70000;
		G.Room_player_flg |= 1;
		pl_mv05_w_tbl[pEm->Type & 0xfff](pEm);
		if (((pEm->Id == 8) || (pEm->Id == 10)) && ((pEm->Type & 0xfff) == 2)) {
			Ck_pos_y = pEm->pSin_parts_ptr[0xb].Workm.t[1];
			hand_tp = 2;
		}
		else {
			Ck_pos_y = pEm->pSin_parts_ptr[0xe].Workm.t[1];
			hand_tp = pEm->Type & 0xfff;
		}
		Hand_ck(pEm, Ck_pos_y, hand_tp);
		init_Weapon_at(pEm, (ATTACK_WEAPON_TBL *)((char *)Pl_weapon_at_base +
							(pEm->Type & 0xfff) * 0x18 +
							(UINT)Gun_jtg_bit[pEm->Be_flg >> 0x1d] * 8));
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		Pl_view_esp_set(pEm, pEm->pSin_parts_ptr + 0xb);
		Pl_weapon_vib(pEm);
	}
	wep_type = pEm->Type & 0xfff;
	if (3 < wep_type - 9) {
		jtg_idx = pEm->Be_flg >> 0x1d;
		Weapon_at_ck(pEm, (wep_type - 1) * 0x10000 | jtg_idx,
					 (ATTACK_WEAPON_TBL *)((char *)Pl_weapon_at_base + wep_type * 0x18 + (UINT)Gun_jtg_bit[jtg_idx] * 8));
	}
	if ((Key & 8) != 0) pEm->Cdir_y = pEm->Cdir_y + -8;
	if ((Key & 2) != 0) pEm->Cdir_y = pEm->Cdir_y + 8;
	if (((Key & 0x100) == 0) && ((Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][2] & 0x7f) < pEm->Move_cnt)) {
		pEm->Routine_2 = 1;
		pEm->Routine_3 = 0;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
	}
	else {
		hand_tp = pEm->Type & 0xfff;
		if (((hand_tp == 8) || (hand_tp == 6)) && (pEm->Id != 11)) {
			Spd_get(pEm, Kan, Seq, 0);
		}
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			pEm->Routine_2 = 1;
			pEm->Routine_3 = 0;
			pEm->Spd_x = 0;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
		}
		if (pEm->Water != 0) {
			if (((pEm->Water < pEm->Ground) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
				Pl_water(pEm);
			}
			pP_ptr = pEm->pSin_parts_ptr;
			esp_vec.vx = 0;
			esp_vec.vy = 300;
			esp_vec.vz = 0;
			if ((int)pEm->Water < pP_ptr[0xb].Workm.t[1] + 300) {
				rnd_r = Rnd();
				Esp_call((((UINT)rnd_r) * 4 + 0x60cU) | 0x1a000000,
						 pEm->Cdir_y, &pP_ptr[0xb].Workm, &esp_vec);
			}
			pP_ptr = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pP_ptr[0xe].Workm.t[1] + 200) {
				rnd_r = Rnd();
				Esp_call((((UINT)rnd_r) * 4 + 0x60cU) | 0x1a000000,
						 pEm->Cdir_y, &pP_ptr[0xe].Workm, &esp_vec);
			}
		}
		Add_speedXZ(pEm, 0);
		pl_mv05_t2_init[(pEm->Type & 0xfff) + 0x12](pEm);
	}
}

/* DS 0x800a6f94 */
PL_WEP_FN pl_mv05_t2_init[38] = {
	pl_w01_t2_init,	/* [0] 0x80043ecc */
	pl_w02_t2_init,	/* [1] 0x80043ed4 */
	pl_w03_t2_init,	/* [2] 0x80043efc */
	pl_w04_t2_init,	/* [3] 0x80043f24 */
	pl_w05_t2_init,	/* [4] 0x80043f2c */
	pl_w06_t2_init,	/* [5] 0x80043f54 */
	pl_w07_t2_init,	/* [6] 0x80043f7c */
	pl_w08_t2_init,	/* [7] 0x80043fa4 */
	pl_w09_t2_init,	/* [8] 0x80043fcc */
	pl_w0a_t2_init,	/* [9] 0x80043ff4 */
	pl_w0b_t2_init,	/* [10] 0x8004401c */
	pl_w0c_t2_init,	/* [11] 0x80044044 */
	pl_w0d_t2_init,	/* [12] 0x800440cc */
	pl_w0e_t2_init,	/* [13] 0x800440f4 */
	pl_w0f_t2_init,	/* [14] 0x800440fc */
	pl_w10_t2_init,	/* [15] 0x80044104 */
	pl_w11_t2_init,	/* [16] 0x8004410c */
	pl_w12_t2_init,	/* [17] 0x80044134 */
	pl_w02_t2_init,	/* [18] 0x80043ed4 */
	pl_w01_t2_move,	/* [19] 0x8004413c */
	pl_w02_t2_move,	/* [20] 0x8004418c */
	pl_w03_t2_move,	/* [21] 0x8004429c */
	pl_w04_t2_move,	/* [22] 0x800443ac */
	pl_w05_t2_move,	/* [23] 0x800444e0 */
	pl_w06_t2_move,	/* [24] 0x800445ec */
	pl_w07_t2_move,	/* [25] 0x80044730 */
	pl_w08_t2_move,	/* [26] 0x80044908 */
	pl_w09_t2_move,	/* [27] 0x80044b44 */
	pl_w0a_t2_move,	/* [28] 0x80044f44 */
	pl_w0b_t2_move,	/* [29] 0x80045090 */
	pl_w0c_t2_move,	/* [30] 0x800451dc */
	pl_w0d_t2_move,	/* [31] 0x8004522c */
	pl_w0e_t2_move,	/* [32] 0x8004534c */
	pl_w0f_t2_move,	/* [33] 0x80045354 */
	pl_w10_t2_move,	/* [34] 0x800454a0 */
	pl_w11_t2_move,	/* [35] 0x80045588 */
	pl_w12_t2_move,	/* [36] 0x80045648 */
	pl_w02_t2_move,	/* [37] 0x8004418c */
};

/* DS 0x80043d30 (Pl_weapon_vib) */
void Pl_weapon_vib(ENEMY_WORK *pEm)
{
	UCHAR *pVib;

	pVib = Pl_vib_tbl[(UCHAR)(pEm->Type - 2)];
	Pad_vib_set0(pVib[0], 0);
	Pad_vib_set1(pVib[1], Pl_vib_tbl[pEm->Type - 2][2], 0);
	Pad_vib_fade_set(pVib[5], pVib[3], pVib[4], pVib[1]);
}

/* DS 0x800a6ed0 (Pl_vib_tbl) - 18 rows of 6 bytes; row k is Type k+2 */
UCHAR Pl_vib_tbl[18][6] = {
	{ 0x03, 0x05, 0xfa, 0x00, 0x00, 0x00 },	/* +0x0 */
	{ 0x03, 0x05, 0xfa, 0x00, 0x00, 0x00 },	/* +0x6 */
	{ 0x02, 0x01, 0xfa, 0xb4, 0x00, 0x0f },	/* +0xc */
	{ 0x05, 0x07, 0xfa, 0xfa, 0x00, 0x0b },	/* +0x12 */
	{ 0x08, 0x08, 0xfa, 0xc8, 0x00, 0x10 },	/* +0x18 */
	{ 0x04, 0x09, 0xe6, 0xe6, 0x00, 0x08 },	/* +0x1e */
	{ 0x0a, 0x08, 0xe6, 0xb4, 0x00, 0x14 },	/* +0x24 */
	{ 0x05, 0x07, 0xe6, 0xc8, 0x00, 0x07 },	/* +0x2a */
	{ 0x05, 0x07, 0xe6, 0xc8, 0x00, 0x07 },	/* +0x30 */
	{ 0x05, 0x07, 0xe6, 0xc8, 0x00, 0x07 },	/* +0x36 */
	{ 0x02, 0x04, 0xe6, 0x00, 0x00, 0x00 },	/* +0x3c */
	{ 0x03, 0x06, 0xc8, 0x00, 0x00, 0x00 },	/* +0x42 */
	{ 0x0a, 0x08, 0xfa, 0xc8, 0x00, 0x0f },	/* +0x48 */
	{ 0x02, 0x01, 0xc8, 0x96, 0x00, 0x14 },	/* +0x4e */
	{ 0x01, 0x05, 0xc8, 0xc8, 0x64, 0x14 },	/* +0x54 */
	{ 0x0f, 0x0f, 0xfa, 0xfa, 0x00, 0x14 },	/* +0x5a */
	{ 0x02, 0x01, 0xfa, 0xc8, 0x00, 0x28 },	/* +0x60 */
	{ 0x03, 0x05, 0xfa, 0x00, 0x00, 0x00 },	/* +0x66 */
};

/* DS 0x800a64e0 (Pl_w010) */
ATTACK_WORK3 Pl_w010[5] = {
	{ 0x00, 0x80, 0x00, 0x00, -500, 0x0320, 0x0146, 0x0079, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x10, 0x00, 0x00, 0x0000, 0x044c, 0x016a, 0x0079, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x10, 0x00, 0x00, 0x02bc, 0x0320, 0x019a, 0x009d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x10, 0x00, 0x00, 0x012c, -100, 0x019a, 0x009d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x10, 0x00, 0x00, 0x041a, -550, 0x0146, 0x0079, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};

/* DS 0x800a6db4 (W_at_ck) */
UCHAR W_at_ck[3][9] = {
	{ 0x04, 0x02, 0x01, 0x02, 0x01, 0x04, 0x01, 0x02, 0x04 },
	{ 0x04, 0x02, 0x00, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

/* DS 0x800a6dd0 (Ghand_len) */
SHORT Ghand_len[19][4] = {
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, -2427, -2120 },
	{ -200, 0x012c, -2427, -2120 },
	{ -200, 0x012c, -2427, -2120 },
	{ -200, 0x012c, -2427, 0x0000 },
	{ -200, 0x012c, -2427, 0x0000 },
	{ -500, 0x0096, -1813, 0x0000 },
	{ -500, 0x00c8, -1813, 0x0000 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -400, 0x001e, -1850, -1890 },
	{ -600, 0x0258, -1894, -1809 },
	{ -400, 0x00c8, -1894, -1809 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, 0x0000, 0x0000 },
	{ -200, 0x012c, -2427, -2120 },
	{ -200, 0x012c, -2427, -2120 }
};

/* DS 0x800a6e68 (Mw_ck) */
CHAR Mw_ck[16] = {
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01
};

/* DS 0x800a6e90 (Pl_mag) */
PL_MAG_TBL Pl_mag[4] = {
	{ 0x10, 0x00, 0x1e, 0x02, 0x0400 },
	{ 0x10, 0x00, 0x1e, 0x02, 0x0400 },
	{ 0x11, 0xce, 0x32, 0x00, 0x0400 },
	{ 0x10, 0x00, 0x1e, 0x02, 0x0400 }
};

/* DS 0x800a6ea8 (G_rot_data_z) */
SHORT G_rot_data_z[20] = {
	0x0000, 0x0043, 0x0086, 0x00c9, 0x010c, 0x010c, 0x010c, 0x010c,
	0x010c, 0x010c, 0x010c, 0x010c, 0x010c, 0x010c, 0x010c, 0x00c9,
	0x0086, 0x0043, 0x0000, -32768
};
