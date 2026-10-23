/*
 * PLAYER.C - Player character model setup, movement and damage states
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PLAYER.C
 */

#include "TYPES.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "PLAYER.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "OM.H"
#include "OMA.H"
#include "PAD.H"
#include "PL_WEP.H"
#include "PL_WEP2.H"
#include "SCA.H"
#include "SND.H"
#include "SPL_AUTO.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "GAME.H"
#include "ROOM.H"
#include "SCE2.H"
#include "SPL.H"
#include "SPL_SCE.H"
#include "FILECODE.H"
#include "flag.h"
#include "DIVCHK.H"
#include "SPL_TBL.H"
#include "DEFINES.H"

/* DS takes Rnd() as a signed int (no andi after the jal); Rnd_s32() calls it through a LONG-returning type (see ESP_R.C). */
#define Rnd_s32()	(((LONG (*)(VOID))Rnd)())
/* Same call through an unsigned-returning type: DS does not fold the offset into the neighbouring subtraction. */
#define Rnd_u32()	(((ULONG (*)(VOID))Rnd)())

/* DS 0x801bfa14 (Font_tim_buf) */
ULONG *Font_tim_buf = (ULONG *)0x801bfa14;

/* DS 0x80010ec8: Cd_read file name PLD, 4 bytes incl NUL (used by Player_set) */
UCHAR  s_Pl_cd_msg_80010ec8[4] = "PLD";

/* DS 0x8003b9e0 (Pl_model_set) - model/TIM/TMD loader. pData is a block whose first words are
   UCHAR offsets to: [0] seq table, [1] kan table, [2] TMD, [3] TIM (then parts data). */
ULONG Pl_model_set(ENEMY_WORK *pEm, int *pData, UCHAR Tpage, UCHAR Clut)
{
	int    *pOfs;
	ULONG  *pTmd_top;
	int     Kan_ofs;
	int     Tim_ofs;
	ULONG   Packet_top;

	pOfs = (int *)((char *)pData + pData[0]);
	pEm->pSeq_t_ptr = (ULONG *)((char *)pData + pOfs[0]);
	Kan_ofs = pOfs[1];
	pEm->Clut = Clut;
	pEm->Tpage = Tpage;
	pEm->pKan_t_ptr = (ULONG *)((char *)pData + Kan_ofs);
	pEm->pTmd = (ULONG *)((char *)pData + pOfs[2]);
	G.Ctpage = Tpage;
	G.Cclut = Clut;
	Mapping_tim((ULONG *)((char *)pData + pOfs[3]));
	DrawSync(0);
	Mapping_tmd(1, pEm->pTmd, Tpage, Clut);
	pTmd_top = pEm->pTmd;
	pEm->pTmd = pTmd_top + 3;
	pEm->pTmd2 = pTmd_top + 10;
	Tim_ofs = pOfs[3];
	Packet_top = PartsWork_set(pEm, (ULONG)((char *)pData + Tim_ofs));
	PartsWork_link(pEm, Packet_top, pEm->pKan_t_ptr, 1);
	pEm->Parts0_pos_y = 0x1000;
	return (ULONG)((char *)pData + Tim_ofs);
}

/* DS 0x80010ecc, pl_mv00 */
const USHORT Pl_taiki_mot[3] = {
	2, 5, 8,	/* +0x0 */
};



/* DS 0x800A3FE4 (Pld_no): PLxx_PLD names (FILECODE.H), by enemy Id */
USHORT Pld_no[16] = {
	PL00_PLD, PL01_PLD, PL02_PLD, PL03_PLD,
	PL04_PLD, PL05_PLD, PL06_PLD, PL07_PLD,
	PL08_PLD, PL09_PLD, PL0A_PLD, PL0B_PLD,
	PL0C_PLD, PL0D_PLD, PL0E_PLD, PL0F_PLD,
};

/* DS 0x800a4004 (Pl_neck_timer) */
UCHAR Pl_neck_timer = 0x2d;

/* DS 0x800a4005 (Pl_poison_tint) */
UCHAR Pl_poison_tint = 0x00;

/* DS 0x800a4008 (Pld_name): PLD file path of the player model, zero padded to 40 bytes */
UCHAR Pld_name[40] = "d:/bio2/pl0/pld/pl00ch.pld";

/* DS 0x800a4030 (Pl_routine). Slot 8 holds 0x80700fb8 in retail, which is outside the image and not a function, so it stays NULL. */
PL_FUNC Pl_routine[9] = {
	Pl_init,
	Pl_move,
	Pl_damage,
	Pl_die,
	Spl_Scenario,
	Pl_Em_damage,
	Pl_Em_die,
	Pl_dead,
	NULL,
};

/* DS 0x800a4054 (Pl_mv_pad) */
PL_FUNC Pl_mv_pad[12] = {
	pl_mv00_pad,
	pl_mv01_pad,
	pl_mv02_pad,
	pl_mv03_pad,
	pl_mv04_pad,
	pl_mv05_pad,
	pl_mv06_pad,
	pl_mv07_pad,
	pl_mv08_pad,
	pl_mv09_pad,
	pl_mv0a_pad,
	pl_mv0b_pad,
};

/* DS 0x8003baf0 (Player_set) */
void Player_set(ENEMY_WORK *pEm)
{
	int tbl_ofs;
	int kan_ofs;
	int Tim_ofs;
	ULONG *pBase;
	ULONG *pTmd_top;
	PARTS_W *pParts;
	ULONG ck_res;
	UCHAR pl_id;
	UCHAR *work_top;
	UCHAR *Jh_top;
	UCHAR *Buf_top = (UCHAR *)PLDFile;			/* load buffer: word table, then data */
	UCHAR *Cd_top = (UCHAR *)(PLDFile + 0xDA00);	/* copy of the buffer, used as the data base */
	PL_DATA_TBL *pTbl;

	Cd_read((UINT)Pld_no[pEm->Id], (ULONG *)PLDFile, 1, s_Pl_cd_msg_80010ec8);
	tbl_ofs = *(int *)PLDFile;
	pTbl = (PL_DATA_TBL *)(Buf_top + tbl_ofs);
	pEm->pSeq_t_ptr = (ULONG *)(Cd_top + pTbl->Seq_ofs);
	kan_ofs = pTbl->Kan_ofs;
	pEm->Clut = 1;
	pEm->Tpage = 0x17;
	pEm->pKan_t_ptr = (ULONG *)(Cd_top + kan_ofs);
	pEm->pTmd = (ULONG *)(Cd_top + pTbl->Tmd_ofs);
	G.Cclut = 1;
	G.Ctpage = 0x17;
	Mapping_tim((ULONG *)(Buf_top + pTbl->Tim_ofs));
	DrawSync(0);
	Memcpy(Cd_top, Buf_top, pTbl->Tim_ofs);
	Mapping_tmd(1, pEm->pTmd, 0x17, 1);
	pTmd_top = pEm->pTmd;
	pEm->pTmd = pTmd_top + 3;
	pEm->pTmd2 = pTmd_top + 10;
	/* Player 0x0e (Aida) and 0x0a (Jh) get their work area at a fixed DS address (0x801cfc14 + ofs). */
	Tim_ofs = pTbl->Tim_ofs;
	work_top = Cd_top + Tim_ofs;
	if ((pEm->Id == 14) || (pEm->Id == 10)) {
		work_top = (UCHAR *)(0x801cfc14U + (ULONG)Tim_ofs);
		Jh_top = Cd_top + Tim_ofs;	/* only read by the Aida and Jh initialisers below */
	}
	G.pPl_emd_top = (ULONG *)PartsWork_set(pEm, (ULONG)work_top);
	pBase = (ULONG *)PartsWork_link(pEm, PLDFile, pEm->pKan_t_ptr, 1);
	pl_id = pEm->Id;
	if (pl_id == 14) {
		Aida_Jh_init(pEm, (ULONG)Jh_top);
		pl_id = pEm->Id;
	}
	if (pl_id == 10) {
		Pl_jh_init(pEm, (ULONG)Jh_top);
	}
	G.Random = 0xd2706ca4;
	pParts = pEm->pSin_parts_ptr;
	gpPl_tmd_ptr = (ULONG (*)[1])pParts[0xb].pTmd;
	gpPl_tmd2_ptr = (ULONG (*)[1])pParts[0xb].pTmd2;
	gpPl_pac_ptr = (ULONG (*)[1])pParts[0xb].pPacket;
	gpPl_pac2_ptr = (ULONG (*)[1])pParts[0xb].pPacket2;
	G.pPl_pac_top = pBase;
	pEm->Max_life = 200;
	if (pEm->Id == 13) {
		pEm->Max_life = 400;
	}
	pEm->Status_flg = 0;
	pEm->Parts0_pos_y = 0x1000;
	if (St.Equip_item_no != 0x80) {
		pEm->Type = (USHORT)St.Equip_item_id;
	}
	else {
		pEm->Type = 0;
	}
	Pl_weapon_ch(pEm);
	pEm->Sc_id = 0x80;
	G.Player.Water = 0;
	G.SaveData.Change_player = (short)pEm->Id;
	if ((G.System_flg & SYS_SPAD_STACK) != 0) {
		SetSpadStack2();
		((void (*)(UINT))Snd_sys_arms_set)(pEm->Type & 0xfff);
		ResetSpadStack2();
	}
	else {
		((void (*)(UINT))Snd_sys_arms_set)(pEm->Type & 0xfff);
	}
	pEm->Atd[0].At_w = 0x1c2;
	pEm->Atd[0].At_d = 0x1c2;
	pEm->Atd[0].W = 0x1c2;
	pEm->Atd[0].D = 0x1c2;
	pl_id = pEm->Id;
	pEm->nOba = 1;
	pEm->Atd[0].Ofs_y = -0x5fa;
	pEm->Atd[0].At_h = 0x5fa;
	pEm->pTbefore_func = Pl_before;
	pEm->pTafter_func = Pl_after;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_z = 0;
	if ((pl_id == 14) || (pl_id == 10)) {
		pEm->pSin_parts_ptr->Be_flg |= 0x4000;
	}
	Pl_poison_tint = 0;
	if (((pEm->Id == 0) || (pEm->Id == 2)) && (ck_res = Flg_ck(G.SaveData.Common_flg, 0x3e), ck_res != 0)) {
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	}
	ck_res = Tama_ck_i(pEm->Type & 0xfff);
	if (ck_res != 0) {
		pEm->Status_flg |= 1;
	}
	return;
}

/* DS 0x8003bed8 (Pl_before) */
void Pl_before(ENEMY_WORK *pEm)
{
	ULONG ck_res;

	if (((int)G.Stop_flg >= 0) && (pEm->Id < 10) && ((pEm->Id & 1) != 0)) {
		Op_PartsSort05(pEm);
	}
	else {
		if (pEm->Id == 14) {
			Aida_Jh_move(pEm);
		}
		if ((pEm->Id == 15) && (ck_res = Flg_ck(G.SaveData.Common_flg, 0xff), ck_res != 0)) {
			Op_PartsSort06(pEm);
		}
	}
	return;
}

/* DS 0x8003bf7c (Pl_after) */
void Pl_after(ENEMY_WORK *pEm)
{
	if (pEm->Id == 10) {
		Pl_jh_move(pEm);
	}
	return;
}

/* DS 0x8003bfac (Player_move) */
void Player_move(ENEMY_WORK *pEm)
{
	BOOL tick;
	int Life;
	UINT life_col;
	int Max_life;
	PARTS_W *pParts;
	SVECTOR unused_vec;

	(void)unused_vec;
	if ((int)G.Stop_flg < 0) {
		return;
	}
	G.Status_flg &= ~STAT_BG_HOLD;
	G.Room_player_flg &= 0xffe0;
	if ((pEm->Damage_cnt & 0x7f) != 0) {
		pEm->Damage_cnt--;
	}
	if (pEm->Id == 13) {
		Max_life = (int)pEm->Max_life;
		Life = (int)pEm->Life << 0xf;
		pParts = pEm->pSin_parts_ptr;
		{ int q; DIV_S(q, Life, Max_life); life_col = (UINT)q >> 8; }
		pParts->Poly_rgb = (life_col << 0x10 | life_col << 8) + 0x80;
		if (pEm->Life < 0) {
			pParts->Poly_rgb = 0x80;
		}
		/* Poison tint: rises while poisoned (capped at 0x40), fades otherwise. */
		if (G.Pl_poison_timer != 0) {
			if ((pEm->Move_cnt & 1) == 0) {
				Pl_poison_tint++;
			}
			if (0x40 < Pl_poison_tint) {
				Pl_poison_tint = 0x40;
			}
		}
		else if ((Pl_poison_tint != 0) && ((pEm->Move_cnt & 1) == 0)) {
			Pl_poison_tint--;
		}
		pParts->Poly_rgb = pParts->Poly_rgb + (UINT)Pl_poison_tint * 0x10000;
	}
	if (G.Pl_poison_timer != 0) {
		tick = (G.Pl_poison_down == 0);
		G.Pl_poison_down = G.Pl_poison_down + -1;
		if (tick) {
			G.Pl_poison_down = 30;
			if (1 < pEm->Life) {
				pEm->Life--;
			}
		}
	}
	PL_TBL_FN(Pl_routine, pEm->Routine_0)(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
	Pl_neck(pEm, 7000, 1500);
	Rot_neck(pEm, pEm->Cdir_y);
	if ((pEm->Type & 0xfff) == 0xc) {
		pl_bow(pEm);
	}
	G_rot(pEm);
	Gat_rot(pEm);
	Mag_down(pEm);
	return;
}

/* DS 0x8003c210 (Pl_init) */
void Pl_init(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ULONG ck_res;
	PARTS_W *pParts;
	PARTS_W *pNeck;
	PARTS_W *pBody;
	UINT be_0f;
	UINT be_10;
	UINT be_flg;
	int half_z;
	UINT half_x;

	*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;
	pEm->pEnemy_ptr = (ULONG *)pEm;
	pEm->pEnemy_neck = &pEm->Be_flg;
	*(UINT *)&pEm->Move_no = 0x2;		/* Move_no 2, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);
	half_x = 500;
	pNeck = pEm->pSin_parts_ptr;
	pEm->Neck_no = 8;
	pEm->Neck_flg = 0;
	pNeck[8].Free[2] = 0x60;
	pNeck[8].Free[3] = 0x60;
	pNeck[8].Free[4] = 0x200;
	pNeck[8].Free[0] = 0;
	pNeck[8].Free[1] = 0;
	pNeck[8].Free[5] = 0x138;
	half_z = 600;
	if ((((pEm->Id == 1) || (pEm->Id == 3)) || (pEm->Id == 5)) || ((pEm->Id == 7 || (pEm->Id == 9)))) {
		half_x = 0x1a4;
		half_z = 0x208;
		pEm->Atd[0].Ofs_y = -0x546;
		pEm->Atd[0].At_w = 0x15e;
		pEm->Atd[0].At_h = 0x546;
		pEm->Atd[0].At_d = 0x15e;
		pEm->Atd[0].W = 0x15e;
		pEm->Atd[0].D = 0x15e;
	}
	if (pEm->Id == 14) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Atd[0].Ofs_y = -0x546;
		pEm->Atd[0].At_w = 0x15e;
		pEm->Atd[0].At_h = 0x546;
		pEm->Atd[0].At_d = 0x15e;
		pEm->Atd[0].W = 0x15e;
		pEm->Atd[0].D = 0x15e;
		pParts[0xf].Be_flg = 0;
		ck_res = Flg_ck(G.SaveData.Scenario_flg, 0x50);
		if (ck_res != 0) {
			pParts[0xf].Be_flg = 1;
		}
		half_x = 0x1a4;
		half_z = 0x208;
		pParts[0xf].pSuper = (ULONG *)&pParts->Workm;
		pParts[0xf].Pos_x = 0x42;
		pParts[0xf].pOya_parts = &pParts->Be_flg;
		pParts[0xf].Pos_y = -0x28e;
		pParts[0xf].Pos_z = 0;
		pParts[0xf].Cdir_x = 0;
		pParts[0xf].Cdir_y = 0;
		pParts[0xf].Cdir_z = 0;
		pParts[0xf].Free[0] = 0;
		pParts[0xf].Free[1] = 0;
	}
	if (pEm->Id == 15) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Atd[0].Ofs_y = -0x32a;
		pEm->Atd[0].At_w = 0xfa;
		pEm->Atd[0].At_h = 0x32a;
		pEm->Atd[0].At_d = 0xfa;
		pEm->Atd[0].W = 0xfa;
		pEm->Atd[0].D = 0xfa;
		pParts[0xf].Be_flg = 0;
		be_flg = *(volatile ULONG *)&pParts[0xf].Be_flg;
		pParts[0xf].pSuper = (ULONG *)&pParts->Workm;
		pParts[0xf].Pos_x = 8;
		pParts[0xf].pOya_parts = &pParts->Be_flg;
		pParts[0xf].Pos_y = -0x1ad;
		pParts[0xf].Pos_z = 0;
		pParts[0xf].Cdir_x = 0;
		pParts[0xf].Cdir_y = 0;
		pParts[0xf].Cdir_z = 0;
		pParts[0xf].Free[0] = 0;
		pParts[0xf].Free[1] = 0;
		pParts[0xf].Be_flg = be_flg | 0x40;
		ck_res = Flg_ck(G.SaveData.Common_flg, 0xff);
		half_x = 0x172;
		if (ck_res != 0) {
			pParts[0xf].Be_flg |= 1;
		}
		half_z = 0x1cc;
	}
	if (((pEm->Id == 1) || (pEm->Id == 3)) || ((pEm->Id == 5 || ((pEm->Id == 7 || (pEm->Id == 9)))))) {
		pBody = pEm->pSin_parts_ptr;
		pBody[0xf].Pos_x = -0x28;
		pBody[0xf].Pos_y = -0x170;
		pBody[0x10].Pos_x = -0x30;
		pBody[0x10].Pos_y = -0x20;
		pBody[0x11].Pos_x = -0x5b;
		pBody[0x11].Pos_y = 0x61;
		pBody[0x12].Pos_x = -1;
		pBody[0x12].Pos_y = 0x7f;
		be_0f = pBody[0xf].Be_flg;
		be_10 = pBody[0x10].Be_flg;
		pBody[0xf].Pos_z = 0;
		pBody[0xf].Free[0] = 0;
		pBody[0xf].Free[1] = 0;
		pBody[0xf].Cdir_y = 0;
		pBody[0x10].Pos_z = 0;
		pBody[0x10].Free[0] = 0;
		pBody[0x10].Free[1] = 0;
		pBody[0x10].Cdir_y = 0;
		pBody[0x11].Pos_z = 0;
		pBody[0x11].Free[0] = 0;
		pBody[0x11].Free[1] = 0;
		pBody[0x11].Cdir_y = 0;
		pBody[0x12].Pos_z = 0;
		pBody[0x12].Free[0] = 0;
		pBody[0x12].Free[1] = 0;
		pBody[0x12].Cdir_y = 0;
		pBody[0xf].Be_flg = be_0f | 0x41;
		pBody[0x10].Be_flg = be_10 | 0x41;
		pBody[0x11].Be_flg |= 0x41;
		pBody[0x12].Be_flg |= 0x41;
	}
	if ((G.Kage9[0].Be_flg & 1) == 0) {
		Kage_work9_set((ULONG *)&pEm->pKage_work, 0, half_x | half_z << 0x10, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
	}
	if ((pEm->Type & 0xfff) == 0xc) {
		pl_bow_clr(pEm);
	}
	return;
}

/* DS 0x8003c57c (Pl_move) */
void Pl_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	PL_TBL_FN(Pl_mv_pad, pEm->Routine_1)(pEm, G.Key, G.Key_trg);
	PL_TBL_FN(Pl_mv, pEm->Routine_1)(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
	return;
}

/* DS 0x8003c5f8 (pl_mv00_pad) */
void pl_mv00_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	LONG prev_life_u;
	signed char *pLife_u = (signed char *)&pEm->D_life_u;
	LONG life;
	ULONG ck_res;

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		life = pEm->Life;
		prev_life_u = *pLife_u;
		pEm->D_life_u = 0;
		if (life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != *pLife_u) {
			pEm->Routine_2 = 0;
		}
	}
	if ((Key & 10) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x401;	/* DS stores the four Routine bytes as one word */
	}
	if ((Key & 4) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x301;	/* DS stores the four Routine bytes as one word */
	}
	if ((Key & 1) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x101;	/* DS stores the four Routine bytes as one word */
		if ((Key & 0x200) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x201;	/* DS stores the four Routine bytes as one word */
		}
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((((USHORT *)&pEm->Sca_info)[1] & 0x10) != 0) {
			Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs);
			return;
		}
		ck_res = Oma_pl_updown_ck(pEm->Id + 4);
		if (ck_res != 0) {
			return;
		}
		if ((Key_trg & 0x80) != 0) {
			G.Status_flg |= STAT_INSPECT;
		}
	}
	if (((Key & 0x100) != 0) && ((pEm->Type & 0xfff) != 0)) {
		*(ULONG *)&pEm->Routine_0 = 0x501;	/* DS stores the four Routine bytes as one word */
	}
	return;
}

/* DS 0x80010ed4, pl_mv01_pad */
const UCHAR Pl_walk_spd[3] = {
	0x28, 0x20, 0x10,	/* +0x0 */
};

/* DS 0x8003c780 (pl_mv00) */
void pl_mv00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	short timer0_new;
	ULONG jnt_ret;
	USHORT u_tmp;
	USHORT mot_no;
	USHORT taiki_mot[3] = { 2, 5, 8 };	/* local copy of Pl_taiki_mot (DS 0x80010ecc, retail keeps it on the stack) */

	switch (pEm->Routine_2) {
	case 0:
		mot_no = taiki_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = mot_no + 0x70000;		/* Move_no, Move_cnt, Hokan_flg, Mplay_flg */
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
		pEm->Timer0 = 0;
		/* falls through: the first call runs the waiting code as well */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		timer0_new = pEm->Timer0 + 1;
		pEm->Timer0 = timer0_new;
		if ((300 < timer0_new) && (pEm->Id == 11)) {
			u_tmp = pEm->Type & 0xfff;
			if ((u_tmp == 0x13) || ((u_tmp < 2 || (u_tmp == 8)))) {
				*(UINT *)&pEm->Move_no = 0x70011;		/* Move_no 17, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
				pEm->Routine_2 = 2;
			}
		}
		break;
	case 2:
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if (jnt_ret == 0) {
			return;
		}
		*(UINT *)&pEm->Move_no = 0x70012;		/* Move_no 18, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 3;
		break;
	case 3:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		break;
	}
	return;
}

/* DS 0x80010ed8, pl_mv01 / pl_mv04 / pl_mv0a */
const SHORT Pl_walk_mot[3] = {
	0, 3, 6,	/* +0x0 */
};

/* DS 0x80010ee0, pl_mv02_pad */
const UCHAR Pl_run_spd[3] = {
	0x38, 0x30, 0x18,	/* +0x0 */
};

/* DS 0x8003c924 (pl_mv01_pad) */
void pl_mv01_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	LONG prev_life_u;
	signed char *pLife_u = (signed char *)&pEm->D_life_u;
	LONG life;
	ULONG ck_res;
	ULONG *pStatus_flg;
	int i_tmp;
	UCHAR walk_spd[3] = { 0x28, 0x20, 0x10 };	/* local copy of Pl_walk_spd (DS 0x80010ed4) */

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		life = pEm->Life;
		prev_life_u = *pLife_u;
		pEm->D_life_u = 0;
		if (life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != *pLife_u) {
			pEm->Routine_2 = 0;
		}
	}
	if ((Key & 1) == 0) {
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		if ((Key & 10) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x401;	/* DS stores the four Routine bytes as one word */
		}
		if ((Key & 4) == 0) {
			return;
		}
		*(ULONG *)&pEm->Routine_0 = 0x301;	/* DS stores the four Routine bytes as one word */
		return;
	}
	if ((Key & 2) != 0) {
		i_tmp = (int)walk_spd[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		pEm->Cdir_y = ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x5a) + (short)pEm->Cdir_y;
	}
	if ((Key & 8) != 0) {
		i_tmp = (int)walk_spd[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		pEm->Cdir_y = (short)pEm->Cdir_y - ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x5a);
	}
	if ((Key & 0x200) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x201;	/* DS stores the four Routine bytes as one word */
	}
	pStatus_flg = &G.Status_flg;
	if ((*pStatus_flg & STAT_PUSH_OBJ) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0xa01;	/* DS stores the four Routine bytes as one word */
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((((USHORT *)&pEm->Sca_info)[1] & 0x10) != 0) {
			Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs);
			return;
		}
		ck_res = Oma_pl_updown_ck(pEm->Id + 4);
		if (ck_res != 0) {
			return;
		}
		if ((Key_trg & 0x80) != 0) {
			*pStatus_flg |= STAT_INSPECT;
		}
	}
	if (((Key & 0x100) != 0) && ((pEm->Type & 0xfff) != 0)) {
		*(ULONG *)&pEm->Routine_0 = 0x501;	/* DS stores the four Routine bytes as one word */
	}
	return;
}

/* DS 0x80010ee4, pl_mv02 */
const UCHAR Pl_run_mot[3] = {
	0x01, 0x04, 0x07,	/* +0x0 */
};

/* DS 0x8003cbdc (pl_mv01) */
void pl_mv01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	int s_tmp;
	short spd_x;
	PARTS_W *pParts;
	short walk_mot[3] = { 0, 3, 6 };	/* local copy of Pl_walk_mot (DS 0x80010ed8) */
	SVECTOR esp_ofs;

	switch (pEm->Routine_2) {
	case 0:
		s_tmp = walk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = s_tmp + 0x70000;
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
		G.Room_player_flg |= 2;
		/* falls through: the first call runs the walking code as well */
	case 1:
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(0, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 2;
		}
		if ((pEm->Water != 0) && ((pEm->Move_cnt & 1) != 0)) {
			pParts = pEm->pSin_parts_ptr;
			esp_ofs.vx = 0;
			esp_ofs.vy = 300;
			esp_ofs.vz = 0;
			if ((int)pEm->Water < pParts[0xb].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
		break;
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		spd_x = pEm->Spd_x;
		pEm->Spd_x = spd_x - spd_x / 3;
	}
	Add_speedXZ(pEm, 0);
	return;
}

/* DS 0x80010ee8, pl_mv03_pad */
const UCHAR Pl_backwalk_spd[3] = {
	0x28, 0x10, 0x10,	/* +0x0 */
};

/* DS 0x8003ce24 (pl_mv02_pad) */
void pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	LONG prev_life_u;
	signed char *pLife_u = (signed char *)&pEm->D_life_u;
	LONG life;
	int i_tmp;
	ULONG ck_res;
	UCHAR run_spd_tbl[3] = { 0x38, 0x30, 0x18 };	/* local copy of Pl_run_spd (DS 0x80010ee0) */

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		life = pEm->Life;
		prev_life_u = *pLife_u;
		pEm->D_life_u = 0;
		if (life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != *pLife_u) {
			pEm->Routine_2 = 0;
		}
	}
	if ((Key & 0x201) != 0x201) {
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		if ((Key & 10) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x401;	/* DS stores the four Routine bytes as one word */
		}
		if ((Key & 4) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x301;	/* DS stores the four Routine bytes as one word */
		}
		if ((Key & 1) == 0) {
			return;
		}
		*(ULONG *)&pEm->Routine_0 = 0x101;	/* DS stores the four Routine bytes as one word */
		return;
	}
	if (0x32 < pEm->Spd_x) {
		if ((Key & 2) != 0) {
			/* DS: mult-high divide by 0x50 / 0x69 without the sign fix-up; the dividend is a product of two
			   unsigned bytes, so the compiler knows it is non-negative and drops the fix-up itself */
			i_tmp = (int)run_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			pEm->Cdir_y = ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x50) + (short)pEm->Cdir_y;
		}
		if ((Key & 8) != 0) {
			/* DS: mult-high divide by 0x50 / 0x69 without the sign fix-up; the dividend is a product of two
			   unsigned bytes, so the compiler knows it is non-negative and drops the fix-up itself */
			i_tmp = (int)run_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			pEm->Cdir_y = (short)pEm->Cdir_y - ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x50);
		}
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((((USHORT *)&pEm->Sca_info)[1] & 0x10) != 0) {
			Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs);
			return;
		}
		ck_res = Oma_pl_updown_ck(pEm->Id + 4);
		if (ck_res != 0) {
			return;
		}
		if ((Key_trg & 0x80) != 0) {
			G.Status_flg |= STAT_INSPECT;
		}
	}
	if (((Key & 0x100) != 0) && ((pEm->Type & 0xfff) != 0)) {
		*(ULONG *)&pEm->Routine_0 = 0x501;	/* DS stores the four Routine bytes as one word */
	}
	return;
}

/* DS 0x8003d0e8 (pl_mv02) */
void pl_mv02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	LONG dir_ofs;
	UCHAR mot_no;
	short spd_x;
	PARTS_W *pParts;
	PARTS_W *pParts2;
	UCHAR mot_tbl[3] = { 1, 4, 7 };	/* local copy of Pl_run_mot (DS 0x80010ee4) */
	SVECTOR esp_ofs;

	switch (pEm->Routine_2) {
	case 0:
		mot_no = mot_tbl[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = mot_no + 0x70000;
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
		G.Room_player_flg |= 4;
		/* falls through: the first call runs the walking code as well */
	case 1:
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(1, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		}
		if ((pEm->Water != 0) && ((pEm->Move_cnt & 1) != 0)) {
			pParts = pEm->pSin_parts_ptr;
			esp_ofs.vx = 0;
			esp_ofs.vy = 300;
			esp_ofs.vz = 0;
			if ((int)pEm->Water < pParts[0xb].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
		if ((pEm->Id == 13) && ((pEm->Move_cnt & 7) == 7)) {
			pParts2 = pEm->pSin_parts_ptr;
			esp_ofs.vx = 0x100 - Rnd_s32() * 2;
			esp_ofs.vy = -0x2e8 - Rnd_s32() * 2;
			esp_ofs.vz = 0x100 - Rnd_s32() * 2;
			dir_ofs = Rnd_s32() * 2 + -0x900;
			Esp_call(0x1262000, pEm->Cdir_y - dir_ofs, &pParts2->Workm, &esp_ofs);
		}
		break;
	}
	if ((signed char)pEm->D_life_u < 2) {
		pEm->Spd_x = pEm->Spd_x + -0x11;
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		spd_x = pEm->Spd_x;
		pEm->Spd_x = spd_x - spd_x / 3;	/* DS: mult-high divide by 3 with the sign fix-up taken from bit 15 of the short */
	}
	Add_speedXZ(pEm, 0);
	return;
}

/* DS 0x80010ef0, pl_mv04_pad */
const UCHAR Pl_rot_spd[3] = {
	0x50, 0x30, 0x20,	/* +0x0 */
};

/* DS 0x80010f14 (Pl_mv07_init_sv) */
const SVECTOR Pl_mv07_init_sv = { 0, 0, 0, 0 };

/* DS 0x8003d3e0 (pl_mv03_pad) */
void pl_mv03_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	LONG prev_life_u;
	signed char *pLife_u = (signed char *)&pEm->D_life_u;
	LONG life;
	int i_tmp;
	UCHAR bw_spd[3] = { 0x28, 0x10, 0x10 };	/* local copy of Pl_backwalk_spd (DS 0x80010ee8) */

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		life = pEm->Life;
		prev_life_u = *pLife_u;
		pEm->D_life_u = 0;
		if (life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != *pLife_u) {
			pEm->Routine_2 = 0;
		}
	}
	if ((Key & 4) == 0) {
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		if ((Key & 10) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x401;	/* DS stores the four Routine bytes as one word */
		}
		if ((Key & 1) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x101;	/* DS stores the four Routine bytes as one word */
		}
	}
	else {
		if ((Key & 2) != 0) {
			i_tmp = (int)bw_spd[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			pEm->Cdir_y = ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x5a) + (short)pEm->Cdir_y;
		}
		if ((Key & 8) != 0) {
			i_tmp = (int)bw_spd[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			pEm->Cdir_y = (short)pEm->Cdir_y - ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x5a);
		}
		if ((Key_trg & 0x80) != 0) {
			G.Status_flg |= STAT_INSPECT;
		}
		if (((Key & 0x100) != 0) && ((pEm->Type & 0xfff) != 0)) {
			*(ULONG *)&pEm->Routine_0 = 0x501;	/* DS stores the four Routine bytes as one word */
		}
	}
	return;
}

/* DS 0x800a4084 (Pl_mv) */
PL_FUNC Pl_mv[12] = {
	pl_mv00,
	pl_mv01,
	pl_mv02,
	pl_mv03,
	pl_mv04,
	pl_mv05,
	pl_mv06,
	pl_mv07,
	pl_mv08,
	pl_mv09,
	pl_mv0a,
	pl_mv0b,
};

/* DS 0x8003d5f4 (pl_mv03) */
void pl_mv03(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UINT b_tmp;
	UCHAR mot_no;
	short spd_x;
	UCHAR u_tmp;
	ULONG ck_res;
	PARTS_W *pParts;
	/* DS 0x80010eec: 3-byte initialiser template in .rdata, byte-copied (lb/sb) into the local; indexed by D_life_u */
	UCHAR backwalk_mot[3] = { 0x00, 0x09, 0x09 };
	SVECTOR esp_ofs;

	switch (pEm->Routine_2) {
	case 0:
		mot_no = backwalk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = mot_no + 0x70000;
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
		pEm->Timer2 = 0;
		pEm->Timer3 = 0;
		G.Room_player_flg |= 2;
		/* falls through: the first call runs the walking code as well */
	case 1:
		if (((signed char)pEm->Timer3 == 0) && ((signed char)pEm->D_life_u == 0)) {
			ck_res = Lim_dir_ck(pEm, 5000, 800);
			if (ck_res != 0) {
				if ((signed char)pEm->Timer2 == 0) {
					u_tmp = pEm->Move_cnt;
					pEm->Move_no = 1;
					pEm->Timer2 = 1;
					pEm->Hokan_flg = 7;
					pEm->Move_cnt = 0;
					Spd_get(pEm, Kan, Seq, 0);
					pEm->Move_cnt = u_tmp;
					Spd_get(pEm, Kan, Seq, 0);
				}
			}
			else if ((signed char)pEm->Timer2 != 0) {
				b_tmp = pEm->Move_cnt;
				pEm->Move_no = 0;
				pEm->Timer2 = 0;
				pEm->Hokan_flg = 7;
				pEm->Move_cnt = 0;
				u_tmp = (char)(b_tmp / 7) * 7;
				Spd_get(pEm, Kan, Seq, 0);
				pEm->Move_cnt = u_tmp;
				Spd_get(pEm, Kan, Seq, 0);
			}
			pEm->Timer3 = 15;
		}
		pEm->Timer3--;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(0, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 2;
		}
		if ((pEm->Water != 0) && ((pEm->Move_cnt & 1) != 0)) {
			pParts = pEm->pSin_parts_ptr;
			esp_ofs.vx = 0;
			esp_ofs.vy = 300;
			esp_ofs.vz = 0;
			if ((int)pEm->Water < pParts[0xb].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				Esp_call((Rnd_s32() * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
		break;
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		spd_x = pEm->Spd_x;
		pEm->Spd_x = spd_x - spd_x / 3;
	}
	Add_speedXZ(pEm, 0);
	return;
}

/* DS 0x8003d96c (Lim_dir_ck) */
ULONG Lim_dir_ck(ENEMY_WORK *pEm, ULONG Lim, short Dir)
{
	LONG cdir_res;
	int dx;
	int dz;
	UINT dist;
	ULONG line_res;
	long goto_res;
	ENEMY_WORK *pT;
	ENEMY_WORK **ppEm;
	ENEMY_WORK *pNear;

	pNear = (ENEMY_WORK *)0x0;
	if (G.Enemy_num != 0) {
		ppEm = G.pEnemy + 2;
		do {
			pT = *ppEm;
			if (((((pT->Be_flg & 1) != 0) && ((pT->Type & 0xe000) == 0)) &&
				(dx = pEm->Pos_x - pT->Pos_x, dz = pEm->Pos_z - pT->Pos_z,
				dist = SquareRoot0(dx * dx + dz * dz), -1 < pT->Life)) &&
			   ((line_res = Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
									 (VECTOR *)pT->pSin_parts_ptr[pT->Neck_no].Workm.t, 0x2080, 1
									), line_res == 0 &&
				(goto_res = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, Dir), goto_res == 0)))) {
				cdir_res = ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck)(pEm, pT);	/* DS tests the result without a mask */
				if ((cdir_res == 0) && (dist < Lim)) {
					Lim = dist;
					pNear = pT;
				}
			}
			ppEm++;
		} while (ppEm != G.ppEnemy_end);
		if (pNear != (ENEMY_WORK *)0x0) {
			pEm->pEnemy_ptr = (ULONG *)pNear;
			return 1;
		}
	}
	pEm->pEnemy_ptr = (ULONG *)pEm;
	return 0;
}

/* DS 0x8003db38 (Pl_neck) */
void Pl_neck(ENEMY_WORK *pEm, ULONG Lim, short Dir)
{
	ULONG line_res;
	long goto_res;
	ENEMY_WORK *pNear;
	ENEMY_WORK *pSec;
	UINT min_dist;
	ENEMY_WORK **ppEm;
	UINT cand_dist;
	ENEMY_WORK *pCand;

	pNear = (ENEMY_WORK *)0x0;
	min_dist = 0x7fffffff;
	pSec = (ENEMY_WORK *)0x0;
	if ((pEm->Neck_flg & 1) != 0) {
		return;
	}
	if (Pl_neck_timer-- == 0) {
		Pl_neck_timer = 0x2d;
		if ((G.Enemy_num != 0) || ((G.Status_flg & STAT_SUB_PLAYER) != 0)) {
			ppEm = G.pEnemy + 1;
			do {
				pCand = *ppEm;
				if (((pCand->Be_flg & 1) != 0) && ((pCand->Type & 0xc000) == 0)) {
					cand_dist = pCand->L_pl;
					line_res = Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t, (VECTOR *)pCand->pSin_parts_ptr[pCand->Neck_no].Workm.t, 0x2080, 1);
					if ((line_res == 0) && (goto_res = Goto00_ck(pEm, pCand->Pos_x, pCand->Pos_z, Dir), goto_res == 0)) {
						if ((pCand->Type & 0x2000) == 0) {
							if (cand_dist < Lim) {
								Lim = cand_dist;
								pNear = pCand;
							}
						}
						else if (cand_dist < min_dist) {
							min_dist = cand_dist;
							pSec = pCand;
						}
					}
				}
				ppEm++;
			} while (ppEm != G.ppEnemy_end);
			if (pNear != (ENEMY_WORK *)0x0) {
				pEm->pEnemy_neck = &pNear->Be_flg;
				return;
			}
			if (pSec != (ENEMY_WORK *)0x0) {
				pEm->pEnemy_neck = &pSec->Be_flg;
				return;
			}
		}
		pEm->pEnemy_neck = &pEm->Be_flg;
		return;
	}
	return;
}

/* DS 0x800a40b4 (Pl_kage_pos) */
VECTOR Pl_kage_pos = { 0 };

/* DS 0x8003dd48 (pl_mv04_pad) */
void pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	LONG prev_life_u;
	signed char *pLife_u = (signed char *)&pEm->D_life_u;
	LONG life;
	int i_tmp;
	ULONG ck_res;
	UCHAR rot_spd_tbl[3] = { 0x50, 0x30, 0x20 };	/* local copy of Pl_rot_spd (DS 0x80010ef0) */

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		life = pEm->Life;
		prev_life_u = *pLife_u;
		pEm->D_life_u = 0;
		if (life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != *pLife_u) {
			pEm->Routine_2 = 0;
		}
	}
	if ((Key & 10) == 0) {
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
	}
	if ((Key & 4) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x301;	/* DS stores the four Routine bytes as one word */
	}
	if ((Key & 1) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x101;	/* DS stores the four Routine bytes as one word */
		if ((Key & 0x200) != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x201;	/* DS stores the four Routine bytes as one word */
		}
	}
	if ((Key & 2) != 0) {
		/* DS: mult-high divide by 100 / 0x69 without the sign fix-up; the dividend is a product of two
		   unsigned bytes, so the compiler knows it is non-negative and drops the fix-up itself */
		i_tmp = (int)rot_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		pEm->Cdir_y = ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x64) + (short)pEm->Cdir_y;
	}
	if ((Key & 8) != 0) {
		/* DS: mult-high divide by 100 / 0x69 without the sign fix-up; the dividend is a product of two
		   unsigned bytes, so the compiler knows it is non-negative and drops the fix-up itself */
		i_tmp = (int)rot_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		pEm->Cdir_y = (short)pEm->Cdir_y - ((*(signed char *)&Pad_act_flg != 0) ? i_tmp / 0x69 : i_tmp / 0x64);
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((((USHORT *)&pEm->Sca_info)[1] & 0x10) != 0) {
			Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs);
			return;
		}
		ck_res = Oma_pl_updown_ck(pEm->Id + 4);
		if (ck_res != 0) {
			return;
		}
		if ((Key_trg & 0x80) != 0) {
			G.Status_flg |= STAT_INSPECT;
		}
	}
	if (((Key & 0x100) != 0) && ((pEm->Type & 0xfff) != 0)) {
		*(ULONG *)&pEm->Routine_0 = 0x501;	/* DS stores the four Routine bytes as one word */
	}
	return;
}

/* DS 0x8003dff4 (pl_mv04) */
void pl_mv04(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	LONG mot_no;
	LONG rnd;
	PARTS_W *pParts;
	SHORT walk_mot[3] = { 0, 3, 6 };	/* local copy of Pl_walk_mot (DS 0x80010ed8) */
	SVECTOR esp_ofs;

	switch (pEm->Routine_2) {
	case 0:
		mot_no = walk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		*(int *)&pEm->Move_no = mot_no + 0x70000;
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
		/* fall through */
	case 1:
		break;
	default:
		return;
	}
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		Snd_se_walk(0, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
		G.Room_player_flg |= 2;
	}
	if ((pEm->Water != 0) && ((pEm->Move_cnt & 1) != 0)) {
		pParts = pEm->pSin_parts_ptr;
		esp_ofs.vx = 0;
		esp_ofs.vy = 300;
		esp_ofs.vz = 0;
		if ((int)pEm->Water < pParts[0xb].Workm.t[1] + 300) {
			rnd = ((LONG (*)(VOID))Rnd)();
			Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
		}
		pParts = pEm->pSin_parts_ptr;
		if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
			rnd = ((LONG (*)(VOID))Rnd)();
			Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
		}
	}
	return;
}

/* DS 0x8003e1d4 (pl_mv06_pad) */
void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	return;
}

/* DS 0x8003e1dc (pl_mv06) */
void pl_mv06(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x70006;		/* Move_no 6, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y, two adjacent halfwords, cleared by one sw (DS: one sw at +0x144) */
		G.Status_flg |= 0xc0;
		/* fall through */
	case 1:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 2:
		St.St_type = 2;
		Game.Doordemo_flg = 1;
		G.Status_flg |= STAT_STATUS_SCREEN;
		Game.Stop_bak = G.Stop_flg;
		pEm->Routine_2 = 3;
		break;
	case 4:
		*(UINT *)&pEm->Move_no = 0x70006;		/* Move_no 6, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 5;
		/* fall through */
	case 5:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x10200);
		break;
	case 6:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		G.Status_flg &= ~STAT_BG_HOLD;
		G.Status_flg &= ~STAT_PL_BUSY;
	}
	return;
}

/* DS 0x8003e314 (pl_mv07_pad) */
void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	return;
}

/* DS 0x8003e31c (pl_mv07) */
void pl_mv07(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UCHAR mot_no;
	short timer1_new;
	long goto_res;
	ULONG jnt_ret;
	UCHAR new_floor;
	UCHAR pl_mv07_mot[2];
	SVECTOR Sv;

	pl_mv07_mot[0] = 0;
	pl_mv07_mot[1] = 3;
	Sv = Pl_mv07_init_sv;

	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x70000;		/* Move_no 0, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		G.Status_flg |= STAT_PL_BUSY;
		pEm->Damage_cnt |= 0x80;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		Goto01(pEm, pEm->Timer0, 0x50);
		goto_res = Goto01_ck(pEm, pEm->Timer0, 0x28);
		if (goto_res == 0) {
			pEm->Routine_2 = 2;
			Goto01(pEm, pEm->Timer0, 0x50);
		}
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		break;
	case 2:
		new_floor = pEm->nFloor + (char)pEm->Timer1;
		mot_no = pl_mv07_mot[(signed char)pEm->Timer2];
		pEm->Routine_2 = 3;
		pEm->nFloor = new_floor;
		*(UINT *)&pEm->Move_no = mot_no + 0x30000;
		if ((signed char)pEm->Timer2 != 0) {
			pEm->nFloor = new_floor + (char)pEm->Timer1 * -2;
		}
		((KAGE_WORK9 *)pEm->pKage_work)->pPos = &Pl_kage_pos;
		pEm->Timer0 = 0;
		/* fall through */
	case 3:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x400);
		if (jnt_ret != 0) {
			pEm->Routine_2 = 4;
			timer1_new = pEm->Timer1 - 1;
			mot_no = pl_mv07_mot[(signed char)pEm->Timer2];
			pEm->Timer1 = timer1_new;
			*(UINT *)&pEm->Move_no = mot_no + 1;
			if (timer1_new == 0) {
				pEm->Routine_2 = 5;
				*(UINT *)&pEm->Move_no = pl_mv07_mot[(signed char)pEm->Timer2] + 2;
			}
		}
		Add_speedXYZ(pEm, 0, 0);
		if ((signed char)pEm->Timer2 == 0) {
			pEm->Timer0 -= 0x14;
			pEm->Kage_ofs += 0x14;
			if (pEm->Timer0 < -200) {
				pEm->Timer0 = -200;
				pEm->Kage_ofs = 200;
			}
		}
		else {
			pEm->Timer0 += 0x14;
			pEm->Kage_ofs += 0x14;
			if (200 < pEm->Timer0) {
				pEm->Timer0 = 200;
				pEm->Kage_ofs = 200;
			}
		}
		Sv.vx = pEm->Timer0;
		/* camera-side shadow position: rotate Sv by the player's matrix, add parts translation */
		{
			PARTS_W *pParts;

			ApplyMatrixSV((MATRIX *)pEm->M, &Sv, &Sv);
			pParts = pEm->pSin_parts_ptr;
			Pl_kage_pos.vx = (int)Sv.vx + pParts->Workm.t[0];
			Pl_kage_pos.vy = (int)Sv.vy + pParts->Workm.t[1];
			Pl_kage_pos.vz = (int)Sv.vz + pParts->Workm.t[2];
		}
		break;
	case 4:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if ((jnt_ret != 0) && (timer1_new = pEm->Timer1 - 1, pEm->Timer1 = timer1_new, timer1_new == 0)) {
			pEm->Routine_2 = 5;
			*(UINT *)&pEm->Move_no = pl_mv07_mot[(signed char)pEm->Timer2] + 2;
		}
		Add_speedXYZ(pEm, 0, 0);
		Sv.vx = 200;
		if ((signed char)pEm->Timer2 == 0) {
			Sv.vx = -200;
		}
		/* camera-side shadow position: rotate Sv by the player's matrix, add parts translation */
		{
			PARTS_W *pParts;

			ApplyMatrixSV((MATRIX *)pEm->M, &Sv, &Sv);
			pParts = pEm->pSin_parts_ptr;
			Pl_kage_pos.vx = (int)Sv.vx + pParts->Workm.t[0];
			Pl_kage_pos.vy = (int)Sv.vy + pParts->Workm.t[1];
			Pl_kage_pos.vz = (int)Sv.vz + pParts->Workm.t[2];
		}
		break;
	case 5:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if (jnt_ret != 0) {
			pEm->Routine_2 = 6;
		}
		Add_speedXYZ(pEm, 0, 0);
		Sv.vx = 200;
		if ((signed char)pEm->Timer2 == 0) {
			Sv.vx = -200;
		}
		/* camera-side shadow position: rotate Sv by the player's matrix, add parts translation */
		{
			PARTS_W *pParts;

			ApplyMatrixSV((MATRIX *)pEm->M, &Sv, &Sv);
			pParts = pEm->pSin_parts_ptr;
			Pl_kage_pos.vx = (int)Sv.vx + pParts->Workm.t[0];
			Pl_kage_pos.vy = (int)Sv.vy + pParts->Workm.t[1];
			Pl_kage_pos.vz = (int)Sv.vz + pParts->Workm.t[2];
		}
		break;
	case 6:
		*(UINT *)&pEm->Move_no = 0x70000;		/* Move_no 0, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 7;
		pEm->Timer0 = 200;
		if ((signed char)pEm->Timer2 == 0) {
			pEm->Timer0 = -200;
		}
		/* fall through */
	case 7:
		if ((signed char)pEm->Timer2 != 0) {
			pEm->Timer0 -= 0x32;
			pEm->Kage_ofs -= 0x32;
			if (pEm->Timer0 < 0) {
				pEm->Timer0 = 0;
				pEm->Kage_ofs = 0;
			}
		}
		else {
			pEm->Timer0 += 0x32;
			pEm->Kage_ofs -= 0x32;
			if (0 < pEm->Timer0) {
				pEm->Timer0 = 0;
				pEm->Kage_ofs = 0;
			}
		}
		Sv.vx = pEm->Timer0;
		/* camera-side shadow position: rotate Sv by the player's matrix, add parts translation */
		{
			PARTS_W *pParts;

			ApplyMatrixSV((MATRIX *)pEm->M, &Sv, &Sv);
			pParts = pEm->pSin_parts_ptr;
			Pl_kage_pos.vx = (int)Sv.vx + pParts->Workm.t[0];
			Pl_kage_pos.vy = (int)Sv.vy + pParts->Workm.t[1];
			Pl_kage_pos.vz = (int)Sv.vz + pParts->Workm.t[2];
		}
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		Add_speedXZ(pEm, 0);
		if (9 < pEm->Move_cnt) {
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
			pEm->Be_flg &= 0xfffffff7;
			G.Status_flg &= ~STAT_PL_BUSY;
			pEm->Damage_cnt &= 0x7f;
			pEm->Be_flg &= 0xfffffffb;
			((KAGE_WORK9 *)pEm->pKage_work)->pPos = (VECTOR *)pEm->pSin_parts_ptr->Workm.t;
		}
		break;
	default:
		break;
	}
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		Snd_se_walk(0, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
		G.Room_player_flg |= 4;
	}
	return;
}

/* DS 0x8003e8cc (pl_mv08_pad) */
void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	VECTOR unused_vec;

	(void)unused_vec;
	return;
}

/* DS 0x8003e8dc (pl_mv08) */
void pl_mv08(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	USHORT cdir_y;
	UCHAR rnd;
	UINT cnt_tgt;
	ULONG jnt_ret;
	UINT mv_cnt;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Move_no = 6;
		if ((pEm->Routine_2 & 2) != 0) {
			pEm->Move_no = 7;
		}
		pEm->Hokan_flg = 7;
		pEm->Move_cnt = 0;
		*(UINT *)&pEm->Move_no = 0x70000;		/* Move_no 0, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Spd_x = 0x32;
		G.Status_flg |= STAT_PL_BUSY;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) != 0) {
			pEm->Cdir_y = cdir_y + (cdir_y >> 2 & 0xff);
		}
		else {
			pEm->Cdir_y = cdir_y - (cdir_y >> 2 & 0xff);
		}
		if ((pEm->Cdir_y & 0x3e0U) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_3 = 2;
			if ((pEm->Routine_2 & 2) != 0) {
				pEm->Routine_1 = 9;
				pEm->Routine_3 = 0;
			}
		}
		break;
	case 2:
		pEm->Routine_3 = 3;
		pEm->Move_no = 6;
		pEm->Hokan_flg = 3;
		/* fall through */
	case 3:
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 4);
		}
		if (pEm->Move_cnt == 17) {
			pEm->Damage_cnt |= 0x80;
			pEm->nFloor++;
			pEm->Be_flg |= 8;
		}
		if (pEm->Id != 15) {
			mv_cnt = pEm->Move_cnt;
			if (mv_cnt == 29) {
				pEm->Spd_x = 1000;
				Add_speedXZ(pEm, 0);
				pEm->Pos_y = pEm->Pos_y + -0x708;
				pEm->Ground = pEm->Ground + -0x708;
				mv_cnt = pEm->Move_cnt;
			}
			cnt_tgt = 49;
		}
		else {
			mv_cnt = pEm->Move_cnt;
			if (mv_cnt == 57) {
				rnd = Rnd();
				if ((rnd & 3) != 0) {
					*(UINT *)&pEm->Move_no = 0x55606;		/* Move_no 6, Move_cnt 86, Hokan_flg 5, Mplay_flg 0 (DS: one sw at +0x14c) */
				}
				mv_cnt = pEm->Move_cnt;
			}
			if (mv_cnt == 99) {
				pEm->Spd_x = 0x224;
				Add_speedXZ(pEm, 0);
				pEm->Pos_y = pEm->Pos_y + -0x708;
				pEm->Ground = pEm->Ground + -0x708;
			}
			mv_cnt = pEm->Move_cnt;
			cnt_tgt = 119;
		}
		if (mv_cnt == cnt_tgt) {
			pEm->Damage_cnt &= 0x7f;
			pEm->Be_flg &= ~0x4;
			pEm->Be_flg &= ~0x8;
		}
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x400);
		pEm->Routine_3 = pEm->Routine_3 + (char)jnt_ret;
		if (((pEm->Water < pEm->Ground) && (pEm->Move_cnt < 0xf)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		goto Walk_se;
	case 4:
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 1000);
		}
		pEm->Routine_1 = 0;
		pEm->Routine_2 = 0;
		pEm->Routine_3 = 0;
		G.Status_flg &= ~STAT_PL_BUSY;
		break;
	default:
Walk_se:
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(1, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		}
	}
	return;
}

/* DS 0x8003ec60 (pl_mv09_pad) */
void pl_mv09_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	VECTOR unused_vec;

	(void)unused_vec;
	return;
}

/* DS 0x8003ec70 (pl_mv09) */
void pl_mv09(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	short timer0_old;
	USHORT at_w;
	long ground;
	ULONG res;
	OBJ_MODEL_WORK *pOm;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		*(UINT *)&pEm->Move_no = 0x70007;		/* Move_no 7, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (char)res;
		if (pEm->Id != 15) {
			if (pEm->Move_cnt == 14) {
				pEm->Routine_3 = 2;
			}
		}
		else if (pEm->Move_cnt == 26) {
			pEm->Routine_3 = 2;
		}
		break;
	case 2:
		pEm->Routine_3 = 3;
		*(UINT *)&pEm->Move_no = 0x8;		/* Move_no 8, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Spd_x = 1000;
		pEm->Timer0 = 0;
		pEm->Timer0 = 5;
		pEm->Pos_y += 0x226;
		pEm->Damage_cnt |= 0x80;
		if (pEm->Id == 15) {
			pEm->Spd_x = 600;
			pEm->Timer0 = 3;
			pEm->Pos_y = pEm->Pos_y + -400;
		}
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);
		pEm->Timer3 = 0;
		pEm->nFloor--;
		pEm->Sca_old_x = (short)pEm->Pos_x;
		pEm->Sca_old_z = (short)pEm->Pos_z;
		/* fall through */
	case 3:
		timer0_old = pEm->Timer0;
		at_w = pEm->Atd[0].At_w;
		pEm->Timer0 = timer0_old + 1;
		pEm->Pos_y = pEm->Pos_y + timer0_old * 0x1e;
		ground = Sca_get_ground((VECTOR *)&pEm->Pos_x, (UINT)at_w, 0x8000, 0);
		pEm->Ground = (short)ground;
		pEm->nFloor = pEm->Pos_y / -0x708;
		pOm = G.Ob_model;
		if (G.Ob_model < G.pObj_work_end) {
			do {
				if ((pOm->Be_flg & 1) != 0) {
					if ((pOm->Be_flg & 2) == 0) {
						Oma_set_ofs(pOm);
						res = Omd_in_check((VECTOR *)&pEm->Pos_x, pOm, (UINT)pEm->Atd[0].At_w, 0);
						if ((res != 0) && ((int)pEm->Ground == pOm->Atd[0].Y - (UINT)pOm->Atd[0].At_h)) {
							pEm->pOn_om = pOm;
							break;
						}
					}
				}
				pOm++;
			} while (pOm < G.pObj_work_end);
		}
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 0x30);
		}
		Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if ((pEm->Water < pEm->Ground) && ((signed char)pEm->Timer3 == 0)) {
			*(signed char *)&pEm->Timer3 = -0x7b;
		}
		if ((pEm->Timer3 & 0x7fU) != 0) {
			pEm->Timer3--;
			Pl_water(pEm);
		}
		if (pEm->Ground + -300 < pEm->Pos_y) {
			pEm->Routine_3 = 4;
			Pad_vib_set0(1, 0);
			Pad_vib_set1(6, 0x96, 0);
		}
		break;
	case 4:
		pEm->Routine_3 = 5;
		pEm->Move_no = 9;
		if (4 < (pEm->Routine_2 & 0xc)) {
			pEm->Move_no = 7;
		}
		pEm->Move_cnt = 4;
		pEm->Hokan_flg = 7;
		pEm->Damage_cnt &= 0x7f;
		pEm->Pos_y = (int)pEm->Ground;
		pEm->Be_flg &= 0xfffffffb;
		Snd_se_walk(1, 4, pEm);
		G.Room_player_flg |= 4;
		/* fall through */
	case 5:
		if (pEm->Move_cnt == 6) {
			Snd_se_walk(1, 7, pEm);
		}
		pEm->nFloor = pEm->Pos_y / -0x708;
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 0x30);
		}
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (char)res;
		break;
	case 6:
		G.Status_flg &= ~STAT_PL_BUSY;
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 1000);
		}
		pEm->Routine_1 = 0;
		pEm->Routine_2 = 0;
		pEm->Routine_3 = 0;
	}
	return;
}

/* DS 0x8003f0fc (pl_mv0a_pad) */
void pl_mv0a_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	if ((((Key & 0x10) == 0) || ((G.Status_flg & STAT_PUSH_OBJ) == 0)) && (pEm->Routine_2 == 5)) {
		pEm->Routine_2 = 6;
		pEm->Move_no = 7;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		Snd_se_stad(0x2000001, (VECTOR *)&pEm->Pos_x);
		Pad_vib_init();
	}
	return;
}

/* DS 0x8003f174 (pl_mv0a) */
void pl_mv0a(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	USHORT cdir_y;
	SHORT walk_mot[3] = { 0, 3, 6 };	/* local copy of Pl_walk_mot (DS 0x80010ed8) */

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = walk_mot[(signed char)pEm->D_life_u] + 0x70000;
		/* fall through */
	case 1:
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(1, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		}
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) != 0) {
			pEm->Cdir_y = cdir_y + (cdir_y >> 2 & 0xff);
		}
		else {
			pEm->Cdir_y = cdir_y - (cdir_y >> 2 & 0xff);
		}
		if ((pEm->Cdir_y & 0x3e0U) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_2 = 2;
		}
		break;
	case 2:
		pEm->Routine_2 = 3;
		*(UINT *)&pEm->Move_no = 0x70007;		/* Move_no 7, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		/* fall through */
	case 3:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 4:
		pEm->Routine_2 = 5;
		*(UINT *)&pEm->Move_no = 0x70008;		/* Move_no 8, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		/* fall through */
	case 5:
		if (pEm->Move_cnt == 1) {
			Snd_se_stad(0x2040001, (VECTOR *)&pEm->Pos_x);
			Pad_vib_set0(2, 0);
			Pad_vib_set1(3, 0x96, 0);
			Pad_vib_fade_set(0x14, 0x96, 0, 3);
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		if (pEm->Id == 15) {
			pEm->Spd_x = 5;
		}
		if ((*pEm->pNow_seq & 0x2000) != 0) {
			if (pEm->Id != 15) {
				Foot_set_pl(pEm, 1, 0);
			}
			else {
				Foot_set_pl(pEm, 1, 1);
			}
		}
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);
		break;
	case 6:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x10200);
		break;
	case 7:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
	}
	return;
}

/* DS 0x8003f420 (pl_mv0b_pad) */
void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	return;
}

typedef struct { UCHAR b[8]; } BYTES8;

/* DS 0x8003f428 (pl_mv0b) */
void pl_mv0b(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	USHORT cdir_y;
	short timer0_old;
	UCHAR dummy_tbl[2] = { 0, 3 };	/* DS stores {0, 3} at sp+0x10, which nothing reads */
	SVECTOR dummy_sv;		/* DS copies Pl_mv07_init_sv to sp+0x18, which nothing reads */

	/* DS copies the 8-byte init vector with lwl/lwr pairs */
	*(BYTES8 *)&dummy_sv = *(BYTES8 *)&Pl_mv07_init_sv;
	(void)dummy_tbl;
	(void)dummy_sv;
	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x70000;		/* Move_no 0, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		G.Status_flg |= STAT_PL_BUSY;
		pEm->Damage_cnt |= 0x80;
		/* fall through */
	case 1:
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) != 0) {
			pEm->Cdir_y = cdir_y + (cdir_y >> 2 & 0xff);
		}
		else {
			pEm->Cdir_y = cdir_y - (cdir_y >> 2 & 0xff);
		}
		if ((pEm->Cdir_y & 0x3e0U) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_2 = 2;
			pEm->Timer0 = 0x2d;
		}
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		break;
	case 2:
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		timer0_old = pEm->Timer0;
		pEm->Timer0 = timer0_old - 1;
		if (timer0_old == 0) {
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		}
		Add_speedXZ(pEm, 0);
		break;
	}
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		Snd_se_walk(0, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
		G.Room_player_flg |= 2;
	}
	return;
}

/* DS 0x8003f600 (Pl_damage) */
void Pl_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	G.Status_flg |= STAT_PL_BUSY;
	PL_TBL_FN(Pl_dm, pEm->Routine_1)(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
	return;
}

/* DS 0x8003f658 (pl_dm00) */
void pl_dm00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ULONG jnt_ret;
	UINT snd_code;

	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x30005;		/* Move_no 5, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y (DS: one sw at +0x144) */
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		snd_code = 0x4010000;
		if (pEm->Routine_1 == 0) {
			snd_code = 0x4000000;
		}
		Snd_se_stad(snd_code | 1, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg |= 0x12;
		pEm->Be_flg &= 0xfffffffb;
		/* falls through: the first call runs Joint_move as well */
	case 1:
		jnt_ret = Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
		break;
	case 2:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		pEm->Damage_cnt = 0;
		pEm->Neck_flg &= 0xed;
		pEm->Status_flg |= 2;
		break;
	}
	return;
}

/* DS 0x8003f778 (pl_dm01) */
void pl_dm01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ULONG jnt_ret;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		pEm->Spd_z = 0;
		*(UINT *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30003;
		*(UINT *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 + -200;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(((pEm->Routine_1 & 1) + 1) * 0x10000 | 0x4000001, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg |= 0x12;
		pEm->Be_flg &= 0xfffffffb;
		/* fall through */
	case 1:
		if (pEm->Hokan_flg != 0) {
			Add_speedXZ(pEm, 0);
		}
		jnt_ret = Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
		break;
	case 2:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		pEm->Damage_cnt = 0;
		pEm->Neck_flg &= 0xed;
		pEm->Status_flg |= 2;
		break;
	default:
		break;
	}
}

/* DS 0x8003f8e4 (pl_dm02) */
void pl_dm02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UCHAR next_rt;
	ULONG res;
	USHORT spd_new;
	int timer0_new;

	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x30002;		/* Move_no 2, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		pEm->Spd_x = -800;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;
		Snd_se_stad(0x4010001, (VECTOR *)&pEm->Pos_x);
		pEm->Be_flg &= 0xfffffffb;
		/* fall through */
	case 1:
		if (pEm->Hokan_flg == 0) {
			pEm->Routine_2 = 2;
		}
		Add_speedXZ(pEm, 0);
		Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		pEm->Spd_x += 0x96;
		if ((pEm->Sca_info & 1) != 0) {
			next_rt = 5;
			goto Set_rt;	/* DS shares the Routine_2 store with case 0xb */
		}
		break;
	case 2:
		*(UINT *)&pEm->Move_no = 0x30003;		/* Move_no 3, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 3;
		Snd_se_stad(0x22a0001, (VECTOR *)&pEm->Pos_x);
		/* fall through */
	case 3:
		Add_speedXZ(pEm, 0);
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		spd_new = pEm->Spd_x + 0xf;
		pEm->Spd_x = spd_new;
		pEm->Routine_2 = pEm->Routine_2 + (char)res;
		if (0 < (int)((UINT)spd_new << 0x10)) {
			pEm->Spd_x = 0;
		}
		break;
	case 4:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		pEm->Damage_cnt = 0;
		pEm->Neck_flg &= 0xed;
		return;
	case 5:
		*(UINT *)&pEm->Move_no = 0x30000;		/* Move_no 0, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 6;
		Snd_se_stad(0x2290001, (VECTOR *)&pEm->Pos_x);
		Snd_se_stad(0x4020001, (VECTOR *)&pEm->Pos_x);
		Pad_vib_set0(1, 0);
		Pad_vib_set1(5, 0xfa, 0);
		/* fall through */
	case 6:
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		pEm->Routine_2 = pEm->Routine_2 + (char)res;
		break;
	case 7:
		*(UINT *)&pEm->Move_no = 0x30004;		/* Move_no 4, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 8;
		pEm->Timer0 = 0x1e;
		if (pEm->Life < 0x96) {
			pEm->Timer0 = 0x3c;
		}
		if (pEm->Life < 0x46) {
			pEm->Timer0 += 0x32;
		}
		if (pEm->Life < 0x1e) {
			pEm->Timer0 += 0x32;
		}
		if (pEm->Life == 0) {
			pEm->Timer0 += 100;
		}
		/* fall through */
	case 8:
		Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x10400);
		res = Gacha_ch();
		timer0_new = ((USHORT)pEm->Timer0 - 1) + res * -5;
		pEm->Timer0 = (short)timer0_new;
		if (timer0_new * 0x10000 < 0) {
			pEm->Routine_2 = 9;
		}
		break;
	case 9:
		*(UINT *)&pEm->Move_no = 0x30001;		/* Move_no 1, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 10;
		pEm->Damage_cnt = 0;
		/* fall through */
	case 10:
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		if (res == 0) {
			return;
		}
		pEm->Routine_2 = 11;
		*(UINT *)&pEm->Move_no = 0x30006;		/* Move_no 6, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		return;
	case 11:
		res = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x10400);
		next_rt = 4;
		if (res == 0) {
			return;
		}
	Set_rt:
		pEm->Routine_2 = next_rt;
		break;
	default:
		break;
	}
	return;
}

/* DS 0x8003fc3c (pl_dm03) */
void pl_dm03(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ULONG res;
	int timer0_new;

	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x30005;		/* Move_no 5, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		pEm->Spd_x = 400;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;
		Snd_se_stad(0x4020001, (VECTOR *)&pEm->Pos_x);
		pEm->Be_flg &= 0xfffffffb;
		/* fall through */
	case 1:
		if ((pEm->Move_cnt == 5) && (G.Player.Hokan_flg == 3)) {
			Snd_se_stad(0x4020001, (VECTOR *)&pEm->Pos_x);
		}
		Add_speedXZ(pEm, 0);
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		pEm->Routine_2 = pEm->Routine_2 + (char)res;
		if (pEm->Hokan_flg != 0) {
			pEm->Spd_x += -0x14;
		}
		else {
			pEm->Spd_x += -0x32;
		}
		if (pEm->Spd_x < 0) {
			pEm->Spd_x = 0;
		}
		break;
	case 2:
		*(UINT *)&pEm->Move_no = 0x30004;		/* Move_no 4, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 3;
		pEm->Timer0 = 0x1e;
		if (pEm->Life < 0x96) {
			pEm->Timer0 = 0x3c;
		}
		if (pEm->Life < 0x46) {
			pEm->Timer0 += 0x32;
		}
		if (pEm->Life < 0x1e) {
			pEm->Timer0 += 0x32;
		}
		if (pEm->Life == 0) {
			pEm->Timer0 += 100;
		}
		/* fall through */
	case 3:
		Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x10400);
		res = Gacha_ch();
		timer0_new = (USHORT)pEm->Timer0 - 1;
		timer0_new -= res * 3;
		pEm->Timer0 = (short)timer0_new;
		if (timer0_new * 0x10000 < 0) {
			pEm->Routine_2 = 4;
		}
		break;
	case 4:
		*(UINT *)&pEm->Move_no = 0x30001;		/* Move_no 1, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 5;
		pEm->Damage_cnt = 0;
		/* fall through */
	case 5:
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		if (res == 0) {
			return;
		}
		pEm->Routine_2 = 6;
		*(UINT *)&pEm->Move_no = 0x30006;		/* Move_no 6, Move_cnt 0, Hokan_flg 3, Mplay_flg 0 (DS: one sw at +0x14c) */
		return;
	case 6:
		res = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x10400);
		if (res != 0) {
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
			pEm->Damage_cnt = 0;
			pEm->Neck_flg &= 0xed;
		}
		break;
	default:
		break;
	}
	return;
}


/* DS 0x8003fee4 (Pl_die) */
void Pl_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	G.Status_flg |= STAT_PL_BUSY;
	PL_TBL_FN(Pl_die_t, pEm->Routine_1)(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
	return;
}

/* DS 0x8003ff3c (pl_die00) */
void pl_die00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ULONG jnt_ret;

	switch (pEm->Routine_2) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x70002;		/* Move_no 2, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y (DS: one sw at +0x144) */
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(0x4030001, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg = 1;
		Pad_vib_set0(3, 0);
		Pad_vib_set1(8, 200, 0);
		Pad_vib_fade_set(0x5a, 200, 0, 8);
		/* falls through: the first call runs Joint_move as well */
	case 1:
		jnt_ret = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
		break;
	case 2:
		*(ULONG *)&pEm->Routine_0 = 0x7;	/* DS stores the four Routine bytes as one word */
		break;
	}
	if (pEm->Hokan_flg == 0) {
		Foot_set_pl(pEm, 0, 1);
	}
	return;
}

/* DS 0x8004006c (Pl_Em_damage) */
void Pl_Em_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ENEMY_WORK *pEnemy;

	G.Status_flg |= STAT_PL_BUSY;
	pEm->Be_flg &= 0xfffffffb;
	pEnemy = (ENEMY_WORK *)pEm->pEnemy_ptr;
	PL_TBL_FN(G.pPldamage_Em, pEnemy->Id - 0x10)(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
	return;
}

/* DS 0x800400d0 (Pl_Em_die) */
void Pl_Em_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	ENEMY_WORK *pEnemy;

	G.Status_flg |= STAT_PL_BUSY;
	pEm->Be_flg &= 0xfffffffb;
	pEnemy = (ENEMY_WORK *)pEm->pEnemy_ptr;
	PL_TBL_FN(G.pPldie_Em, pEnemy->Id - 0x10)(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
	return;
}

/* DS 0x80040134 (Pl_dead) */
void Pl_dead(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	LONG routine1;
	short timer0_old;
	KAGE_WORK9 *pKage;

	routine1 = pEm->Routine_1;
	pKage = (KAGE_WORK9 *)pEm->pKage_work;
	if (routine1 != 1) {
		if (1 < routine1) {
			return;
		}
		if (routine1 != 0) {
			return;
		}
		pEm->Routine_1 = 1;
		Kage_work9_color_set((ULONG *)pKage, 0xbfbf10);
		pEm->Timer0 = 0x3c;
	}
	pKage->Half_x += 8;
	pKage->Half_z += 8;
	timer0_old = pEm->Timer0;
	pEm->Timer0 = timer0_old - 1;
	if (timer0_old == 0) {
		pEm->Routine_1 = 2;
	}
	return;
}

/* DS 0x800401d4 (Pl_life_down) */
ULONG Pl_life_down(long Down_life, ULONG Mode)
{
	PLAYER_WORK *pPl = &G.Player;
	int Life;
	short sLife;

	if ((((G.System_flg & 0x40) != 0) || (0x78 < G.Player.Life)) && (Down_life < 0x29)) {
		Down_life += (Down_life >> 1);
	}
	if (G.SaveData.Sce_timer == 3) {
		if (Down_life < 0x1e) {
			Down_life *= 5;
		}
		else if (Down_life < 0x3c) {
			Down_life <<= 1;
		}
	}
	Life = (UINT)(USHORT)pPl->Life - Down_life;
	pPl->Life = (short)Life;
	if (Mode == 1) {
		if (-1 < Life * 0x10000) {
			return 0;
		}
	}
	else {
		if (Mode != 0) {
			return 1;
		}
		sLife = Life;
		if (-1 < sLife) {
			goto ret0;
		}
		if ((sLife < -0xe) || ((pPl->Status_flg & 0x1000) != 0)) {
			goto over;
		}
	}
	pPl->Life = 0;
	pPl->Status_flg |= 0x1000;
	return 1;
over:
	pPl->Damage_cnt |= 0x80;
	G.System_flg |= SYS_GAME_OVER;
	return 2;
ret0:
	return 0;
}

/* DS 0x800402f4 (Pl_life_up) */
ULONG Pl_life_up(long Up_life)
{
	/* retail reserves 8 more bytes of locals (Pl_life_up frame 8); never read */
	volatile int Rsv[2];
	UINT Life;
	ULONG new_life;

	Life = (UINT)(USHORT)G.Player.Life;
	G.Player.Life = (short)(Life + Up_life);
	G.Player.Status_flg &= 0xefff;
	new_life = (int)((Life + Up_life) * 0x10000) >> 0x10;
	if ((int)G.Player.Max_life < (int)new_life) {
		G.Player.Life = G.Player.Max_life;
	}
	return new_life;
}

/* DS 0x800a40c4 (pl_water) */
VECTOR pl_water = { 0 };

/* DS 0x800a40d4 (Wat_vec00) */
VECTOR Wat_vec00 = { 0 };

/* DS 0x80040350 (Pl_view_esp_set) - camera-view Esp spawn (only when G.Enemy_num < 3).
   pParts is part 11 of the player (pSin_parts_ptr + 11); its Workm is the view matrix. */
void Pl_view_esp_set(ENEMY_WORK *pEm, PARTS_W *pParts)
{
	RCUT        *pCut;
	LONG         view_no;
	LONG         idx;
	short        Dir;
	short        Ofs_dist;
	short        Ofs_height;
	int          Dvx;
	int          Dvz;
	SVECTOR      Dist;
	MATRIX       Rot;
	SVECTOR      Svec;
	VECTOR       Dir_vec;
	VECTOR       Left;
	VECTOR       Right;
	VECTOR       Norm;
	ULONG        Sign;
	/* Retail's locals end at 0x7f and its frame keeps 8 more bytes before the saved registers;
	   retail never reads or writes that space. Unwritten, so it only sets the frame size. */
	volatile UCHAR Reserved[8];

	if (G.Enemy_num >= 3) {
		return;
	}
	view_no = Pl_view_no_tbl[pEm->Type & 0xfff];
	if (view_no == 0) {
		return;
	}
	idx = view_no - 1;
	Ofs_dist = Pl_view_ofs_tbl[idx].Ofs_dist;
	Ofs_height = Pl_view_ofs_tbl[idx].Ofs_height;
	pCut = &G.pRoom->pRcut[G.SaveData.Cut_no];
	Dvx = pCut->View_r[0] - pCut->View_p[0];
	Dvz = pCut->View_r[2] - pCut->View_p[2];
	Dir = Direction_ck(pCut->View_p[0], pCut->View_p[2], pCut->View_r[0], pCut->View_r[2]);

	Dist.vx = Ofs_dist;
	Dist.vy = 0;
	Dist.vz = 0;
	/* DS clears the identity matrix with five word stores (the last also clears the pad halfword after m[2][2]). */
	*(LONG *)&Rot.m[0][0] = 0x1000;
	*(LONG *)&Rot.m[0][2] = 0;
	*(LONG *)&Rot.m[1][1] = 0x1000;
	*(LONG *)&Rot.m[2][0] = 0;
	*(LONG *)&Rot.m[2][2] = 0x1000;
	RotMatrixY(Dir + 0x400, &Rot);
	ApplyMatrixSV(&Rot, &Dist, &Svec);

	Left.vx  = (pCut->View_p[0] + Svec.vx - pParts->Workm.t[0]) >> 4;
	Left.vy  = ((pCut->View_p[1] + Ofs_dist) - pParts->Workm.t[1] - Ofs_height) >> 4;
	Left.vz  = (pCut->View_p[2] + Svec.vz - pParts->Workm.t[2]) >> 4;
	Right.vx = (pCut->View_p[0] - Svec.vx - pParts->Workm.t[0]) >> 4;
	Right.vy = ((pCut->View_p[1] - Ofs_dist) - pParts->Workm.t[1] - Ofs_height) >> 4;
	Right.vz = (pCut->View_p[2] - Svec.vz - pParts->Workm.t[2]) >> 4;

	Svec.vx = Pl_view_ofs_tbl[idx].Dir_vx;
	Svec.vy = Pl_view_ofs_tbl[idx].Dir_vy;
	Svec.vz = 0;
	ApplyMatrix(&pParts->Workm, &Svec, &Norm);
	OuterProduct0(&Norm, &Left, &Left);
	OuterProduct0(&Norm, &Right, &Right);

	Sign = 0x80000000;
	if ((Left.vy < 0) && !(Right.vy & Sign) && ((Left.vx ^ Right.vx) & Sign) && ((Left.vz ^ Right.vz) & Sign)) {
		Dir_vec.vx = Dvx;
		Dir_vec.vy = pCut->View_r[1] - pCut->View_p[1];
		Dir_vec.vz = Dvz;
		VectorNormal(&Dir_vec, &Dir_vec);

		Svec.vx = ((short)(Dir_vec.vx >> 3) + (short)pCut->View_p[0]) - (Rnd_u32() - 0x80);
		Svec.vy = ((short)(Dir_vec.vy >> 3) + (short)pCut->View_p[1]) - (Rnd_u32() - 0x80);
		Svec.vz = ((short)(Dir_vec.vz >> 3) + (short)pCut->View_p[2]) - (Rnd_u32() - 0x80);
		Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);

		if (idx != 0) {
			Dist.vx = (short)(Dir_vec.vx >> 3) + (short)pCut->View_p[0];
			Dist.vy = (short)(Dir_vec.vy >> 3) + (short)pCut->View_p[1];
			Dist.vz = (short)(Dir_vec.vz >> 3) + (short)pCut->View_p[2];
			Svec.vx = Dist.vx - (Rnd_u32() * 2 - 0x100);
			Svec.vy = Dist.vy - (Rnd_u32() * 2 - 0x100);
			Svec.vz = Dist.vz - (Rnd_u32() * 2 - 0x100);
			Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
			Svec.vx = Dist.vx - (Rnd_u32() * 2 - 0x100);
			Svec.vy = Dist.vy - (Rnd_u32() * 2 - 0x100);
			Svec.vz = Dist.vz - (Rnd_u32() * 2 - 0x100);
			Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
			Svec.vx = Dist.vx - (Rnd_u32() * 2 - 0x100);
			Svec.vy = Dist.vy - (Rnd_u32() * 2 - 0x100);
			Svec.vz = Dist.vz - (Rnd_u32() * 2 - 0x100);
			Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
			Svec.vx = Dist.vx - (Rnd_u32() * 2 - 0x100);
			Svec.vy = Dist.vy - (Rnd_u32() * 2 - 0x100);
			Svec.vz = Dist.vz - (Rnd_u32() * 2 - 0x100);
			Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
			Svec.vx = Dist.vx - (Rnd_u32() * 2 - 0x100);
			Svec.vy = Dist.vy - (Rnd_u32() * 2 - 0x100);
			Svec.vz = Dist.vz - (Rnd_u32() * 2 - 0x100);
			Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
		}
	}
}

/* DS 0x800a40e4 (Pl_dm) */
PL_FUNC Pl_dm[6] = {
	pl_dm00,
	pl_dm00,
	pl_dm01,
	pl_dm01,
	pl_dm02,
	pl_dm03,
};

/* DS 0x800a40fc (Pl_die_t) */
PL_FUNC Pl_die_t[1] = {
	pl_die00,
};

/* DS 0x800a4100, indexed by Type & 0xfff */
UCHAR Pl_view_no_tbl[20] = {
	0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x00,	/* +0x0 */
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,	/* +0xa */
};

/* DS 0x800a4114 - PL_VIEW_OFS[3], 8 bytes each; index view_no - 1 */
PL_VIEW_OFS Pl_view_ofs_tbl[3] = {
	{ 100, 0, 4096, 120 },
	{ 400, 0, 4096, 150 },
	{ 400, 3120, 5320, 0 },
};

/* DS 0x800d785c (UnkMvPadFlg) */
CHAR UnkMvPadFlg;

/* DS 0x800d7660 (gpPl_pac_ptr) */
ULONG(*gpPl_pac_ptr)[1];

/* DS 0x800d7840 (gpPl_tmd_ptr) */
ULONG(*gpPl_tmd_ptr)[1];

/* DS 0x800d7858 (gpPl_pac2_ptr) */
ULONG(*gpPl_pac2_ptr)[1];

/* DS 0x800d8cec (gpPl_tmd2_ptr) */
ULONG(*gpPl_tmd2_ptr)[1];
