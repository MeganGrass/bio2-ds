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

/* DS 0x80010ec8: Cd_read file name PLD, 4 bytes incl NUL (used by Player_set) */
UCHAR  s_Pl_cd_msg_80010ec8[] = "PLD";

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

/* DS 0x801bfa14 (Font_tim_buf) */
ULONG Font_tim_buf[0x90EC] RAM_SECTION(".buf_font_tim_buf");	/* 0x243B0 bytes: largest player file (PL0A_PLD, floc size) */


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

/* DS 0x8003baf0 (Player_set) */
void Player_set(ENEMY_WORK *pEm)
{
	int tbl_ofs;
	int kan_ofs;
	ULONG *pBase;
	PARTS_W *pParts;
	ULONG ck_res;
	UCHAR pl_id;
	UCHAR *work_top;
	UCHAR *Jh_top = (UCHAR *)0;
	UCHAR *Buf_top = (UCHAR *)Font_tim_buf;		/* load buffer: word table, then data */
	UCHAR *Cd_top = Pl_model_copy_buf;				/* copy of the buffer, used as the data base */
	int    Tim_ofs;
	PL_DATA_TBL *pTbl;

	Cd_read((UINT)Pld_no[pEm->Id], Font_tim_buf, 1, s_Pl_cd_msg_80010ec8);
	tbl_ofs = (int)Font_tim_buf[0];
	pTbl = (PL_DATA_TBL *)(Buf_top + tbl_ofs);
	pEm->pSeq_t_ptr = (ULONG *)(Cd_top + pTbl->Seq_ofs);
	kan_ofs = pTbl->Kan_ofs;
	pEm->Clut = 1;
	pEm->Tpage = 0x17;
	pEm->pKan_t_ptr = (ULONG *)(Cd_top + kan_ofs);
	pEm->pTmd = (ULONG *)(Cd_top + pTbl->Tmd_ofs);
	G.Cclut = 1;
	G.Ctpage = 0x17;
	Tim_ofs = pTbl->Tim_ofs;
	Mapping_tim((ULONG *)(Buf_top + Tim_ofs));
	DrawSync(0);
	Memcpy(Cd_top, Buf_top, Tim_ofs);
	Mapping_tmd(1, pEm->pTmd, 0x17, 1);
	pBase = pEm->pTmd;
	pEm->pTmd = pBase + 3;
	pEm->pTmd2 = pBase + 10;
	/* Player 0x0e (Aida) and 0x0a (Jh) get their work area at a fixed DS address (0x801cfc14 + ofs). */
	work_top = Cd_top + Tim_ofs;
	if ((pEm->Id == 14) || (pEm->Id == 10)) {
		work_top = (UCHAR *)(0x801cfc14U + (ULONG)Tim_ofs);
		Jh_top = Cd_top + Tim_ofs;
	}
	G.pPl_emd_top = (ULONG *)PartsWork_set(pEm, (ULONG)work_top);
	pBase = (ULONG *)PartsWork_link(pEm, (ULONG)Font_tim_buf, pEm->pKan_t_ptr, 1);
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
	if (St.Equip_item_no == 0x80) {
		pEm->Type = 0;
	}
	else {
		pEm->Type = (USHORT)St.Equip_item_id;
	}
	Pl_weapon_ch(pEm);
	pEm->Sc_id = 0x80;
	G.Player.Water = 0;
	G.SaveData.Change_player = (short)pEm->Id;
	if ((G.System_flg & SYS_SPAD_STACK) == 0) {
		Snd_sys_arms_set((UCHAR)pEm->Type);
	}
	else {
		SetSpadStack2();
		Snd_sys_arms_set((UCHAR)pEm->Type);
		ResetSpadStack2();
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
	if (((pEm->Id == 0) || (pEm->Id == 2)) &&
		(ck_res = Flg_ck(G.SaveData.Common_flg, 0x3e), ck_res != 0)) {
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
	UCHAR Pl_id;

	if ((((int)G.Stop_flg < 0) || (9 < pEm->Id)) || ((pEm->Id & 1) == 0)) {
		Pl_id = pEm->Id;
		if (Pl_id == 14) {
			Aida_Jh_move(pEm);
			Pl_id = pEm->Id;
		}
		if ((Pl_id == 15) && (ck_res = Flg_ck(G.SaveData.Common_flg, 0xff), ck_res != 0)) {
			Op_PartsSort06(pEm);
		}
	}
	else {
		Op_PartsSort05(pEm);
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
	VECTOR unused_vec;

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
		if (G.Pl_poison_timer == 0) {
			if ((Pl_poison_tint != 0) && ((pEm->Move_cnt & 1) == 0)) {
				Pl_poison_tint--;
			}
		}
		else {
			if ((pEm->Move_cnt & 1) == 0) {
				Pl_poison_tint++;
			}
			if (0x40 < Pl_poison_tint) {
				Pl_poison_tint = 0x40;
			}
		}
		pParts->Poly_rgb = pParts->Poly_rgb + (UINT)Pl_poison_tint * 0x10000;
	}
	if (G.Pl_poison_timer != 0) {
		tick = (G.Pl_poison_down == 0);
		G.Pl_poison_down--;
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
	UINT be_0f;
	UINT be_10;
	int half_z;
	UINT half_x;
	UCHAR Pl_id;

	*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;
	pEm->pEnemy_ptr = (ULONG *)pEm;
	pEm->pEnemy_neck = &pEm->Be_flg;
	pEm->Move_no = 2;
	pEm->Move_cnt = 0;
	pEm->Hokan_flg = 0;
	pEm->Mplay_flg = 0;
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);
	half_x = 500;
	pParts = pEm->pSin_parts_ptr;
	pEm->Neck_no = 8;
	pEm->Neck_flg = 0;
	pParts[8].Free[2] = 0x60;
	pParts[8].Free[3] = 0x60;
	pParts[8].Free[4] = 0x200;
	pParts[8].Free[0] = 0;
	pParts[8].Free[1] = 0;
	pParts[8].Free[5] = 0x138;
	Pl_id = pEm->Id;
	half_z = 600;
	if ((((Pl_id == 1) || (Pl_id == 3)) || (Pl_id == 5)) ||
		((Pl_id == 7 || (Pl_id == 9)))) {
		half_x = 0x1a4;
		half_z = 0x208;
		pEm->Atd[0].Ofs_y = -0x546;
		pEm->Atd[0].At_w = 0x15e;
		pEm->Atd[0].At_h = 0x546;
		pEm->Atd[0].At_d = 0x15e;
		pEm->Atd[0].W = 0x15e;
		pEm->Atd[0].D = 0x15e;
		Pl_id = pEm->Id;
	}
	if (Pl_id == 14) {
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
	Pl_id = pEm->Id;
	if (Pl_id == 15) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Atd[0].Ofs_y = -0x32a;
		pEm->Atd[0].At_w = 0xfa;
		pEm->Atd[0].At_h = 0x32a;
		pEm->Atd[0].At_d = 0xfa;
		pEm->Atd[0].W = 0xfa;
		pEm->Atd[0].D = 0xfa;
		pParts[0xf].Be_flg = 0;
		half_x = pParts[0xf].Be_flg;
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
		pParts[0xf].Be_flg = half_x | 0x40;
		ck_res = Flg_ck(G.SaveData.Common_flg, 0xff);
		half_x = 0x172;
		if (ck_res != 0) {
			pParts[0xf].Be_flg |= 1;
		}
		half_z = 0x1cc;
		Pl_id = pEm->Id;
	}
	if (((Pl_id == 1) || (Pl_id == 3)) ||
		((Pl_id == 5 || ((Pl_id == 7 || (Pl_id == 9)))))) {
		pParts = pEm->pSin_parts_ptr;
		pParts[0xf].Pos_x = -0x28;
		pParts[0xf].Pos_y = -0x170;
		pParts[0x10].Pos_x = -0x30;
		pParts[0x10].Pos_y = -0x20;
		pParts[0x11].Pos_x = -0x5b;
		pParts[0x11].Pos_y = 0x61;
		pParts[0x12].Pos_x = -1;
		pParts[0x12].Pos_y = 0x7f;
		be_0f = pParts[0xf].Be_flg;
		be_10 = pParts[0x10].Be_flg;
		pParts[0xf].Pos_z = 0;
		pParts[0xf].Free[0] = 0;
		pParts[0xf].Free[1] = 0;
		pParts[0xf].Cdir_y = 0;
		pParts[0x10].Pos_z = 0;
		pParts[0x10].Free[0] = 0;
		pParts[0x10].Free[1] = 0;
		pParts[0x10].Cdir_y = 0;
		pParts[0x11].Pos_z = 0;
		pParts[0x11].Free[0] = 0;
		pParts[0x11].Free[1] = 0;
		pParts[0x11].Cdir_y = 0;
		pParts[0x12].Pos_z = 0;
		pParts[0x12].Free[0] = 0;
		pParts[0x12].Free[1] = 0;
		pParts[0x12].Cdir_y = 0;
		pParts[0xf].Be_flg = be_0f | 0x41;
		pParts[0x10].Be_flg = be_10 | 0x41;
		pParts[0x11].Be_flg |= 0x41;
		pParts[0x12].Be_flg |= 0x41;
	}
	if ((G.Kage9[0].Be_flg & 1) == 0) {
		Kage_work9_set((ULONG *)&pEm->pKage_work, 0, half_x | half_z << 0x10, 0,
					   (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
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
	char prev_life_u;
	ULONG ck_res;

	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		prev_life_u = pEm->D_life_u;
		pEm->D_life_u = 0;
		if (pEm->Life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (pEm->Life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != pEm->D_life_u) {
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
		if ((pEm->Sca_info & 0x100000) != 0) {
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
	UCHAR routine2;
	short timer0_new;
	ULONG jnt_ret;
	USHORT u_tmp;
	USHORT taiki_mot[3] = { 2, 5, 8 };	/* local copy of Pl_taiki_mot (DS 0x80010ecc, retail keeps it on the stack) */

	routine2 = pEm->Routine_2;
	if (routine2 != 1) {
		if (1 < routine2) {
			if (routine2 == 2) {
				jnt_ret = Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
				if (jnt_ret == 0) {
					return;
				}
				pEm->Move_no = 18;
				pEm->Move_cnt = 0;
				pEm->Hokan_flg = 7;
				pEm->Mplay_flg = 0;
				pEm->Routine_2 = 3;
				return;
			}
			if (routine2 != 3) {
				return;
			}
			Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
			return;
		}
		if (routine2 != 0) {
			return;
		}
		u_tmp = taiki_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = u_tmp + 0x70000;		/* Move_no, Move_cnt, Hokan_flg, Mplay_flg */
		G.Status_flg &= 0xffffff3f;
		pEm->Timer0 = 0;
	}
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
	timer0_new = pEm->Timer0 + 1;
	pEm->Timer0 = timer0_new;
	if ((300 < timer0_new) && (pEm->Id == 11)) {
		u_tmp = pEm->Type & 0xfff;
		if ((u_tmp == 0x13) || ((u_tmp < 2 || (u_tmp == 8)))) {
			pEm->Move_no = 17;
			pEm->Move_cnt = 0;
			pEm->Hokan_flg = 7;
			pEm->Mplay_flg = 0;
			pEm->Routine_2 = 2;
		}
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
	char prev_life_u;
	ULONG ck_res;
	int mul_hi;
	int i_tmp;
	UCHAR walk_spd[3] = { 0x28, 0x20, 0x10 };	/* local copy of Pl_walk_spd (DS 0x80010ed4) */
	VECTOR unused_vec;

	(void)unused_vec;
	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		prev_life_u = pEm->D_life_u;
		pEm->D_life_u = 0;
		if (pEm->Life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (pEm->Life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != pEm->D_life_u) {
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
		i_tmp = (UINT)walk_spd[(signed char)pEm->D_life_u] * (UINT)s__800a2694[(signed char)UnkMvPadFlg];
		if (Pad_act_flg == 0) {
			mul_hi = (int)((ULONGLONG)((LONGLONG)i_tmp * -0x49f49f49) >> 0x20);
		}
		else {
			mul_hi = (int)((ULONGLONG)((LONGLONG)i_tmp * -0x63f63f63) >> 0x20);
		}
		pEm->Cdir_y = (short)((mul_hi + i_tmp) >> 6) + pEm->Cdir_y;
	}
	if ((Key & 8) != 0) {
		if (Pad_act_flg == 0) {
			i_tmp = -0x49f49f49;
		}
		else {
			i_tmp = -0x63f63f63;
		}
		pEm->Cdir_y = pEm->Cdir_y -
					  (short)((int)((int)((ULONGLONG)
										  ((LONGLONG)
										   (int)((UINT)walk_spd[(signed char)pEm->D_life_u] *
												(UINT)s__800a2694[(signed char)UnkMvPadFlg]) * (LONGLONG)i_tmp) >>
										 0x20) +
								   (UINT)walk_spd[(signed char)pEm->D_life_u] * (UINT)s__800a2694[(signed char)UnkMvPadFlg]) >> 6);
	}
	if ((Key & 0x200) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0x201;	/* DS stores the four Routine bytes as one word */
	}
	if ((G.Status_flg & STAT_PUSH_OBJ) != 0) {
		*(ULONG *)&pEm->Routine_0 = 0xa01;	/* DS stores the four Routine bytes as one word */
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((pEm->Sca_info & 0x100000) != 0) {
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

/* DS 0x80010ee4, pl_mv02 */
const UCHAR Pl_run_mot[3] = {
	0x01, 0x04, 0x07,	/* +0x0 */
};

/* DS 0x8003cbdc (pl_mv01) */
void pl_mv01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	short s_tmp;
	UCHAR rnd;
	PARTS_W *pParts;
	SHORT walk_mot[3] = { 0, 3, 6 };	/* local copy of Pl_walk_mot (DS 0x80010ed8) */
	SVECTOR esp_ofs;

	if (pEm->Routine_2 == 0) {
		s_tmp = walk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = s_tmp + 0x70000;
		G.Status_flg &= 0xffffff3f;
		G.Room_player_flg |= 2;
	}
	if (pEm->Routine_2 == 1) {
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
				rnd = Rnd();
				Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				rnd = Rnd();
				Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		s_tmp = pEm->Spd_x;
		pEm->Spd_x = s_tmp - ((short)((ULONGLONG)((LONGLONG)(int)s_tmp * 0x55555556) >> 0x20) - (s_tmp >> 0xf));
	}
	Add_speedXZ(pEm, 0);
	return;
}

/* DS 0x80010ee8, pl_mv03_pad */
const UCHAR Pl_backwalk_spd[3] = {
	0x28, 0x10, 0x10,	/* +0x0 */
};

/* High word of the signed 32x32 product, as the DS divide-by-constant sequences compute it: DS has
   "mult; mfhi" with no sign fix-up (CCPSX always adds one to a C '/'), so the pair is written out. */
#if defined(__mips__) || defined(__psx__) || defined(__R3000__)
#define PL_MULHI(res, x, magic) \
	__asm__ volatile ("mult %1,%2\n\tmfhi %0" : "=r"(res) : "r"(x), "r"(magic) : "hi", "lo")
#else
#define PL_MULHI(res, x, magic) ((res) = (int)(((LONGLONG)(x) * (magic)) >> 0x20))
#endif

/* DS 0x8003ce24 (pl_mv02_pad) */
void pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	char prev_life_u;
	int run_spd;
	int i_tmp;
	int i_hi;
	ULONG ck_res;
	UCHAR run_spd_tbl[3] = { 0x38, 0x30, 0x18 };	/* local copy of Pl_run_spd (DS 0x80010ee0) */
	VECTOR unused_vec;

	(void)unused_vec;
	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		prev_life_u = pEm->D_life_u;
		pEm->D_life_u = 0;
		if (pEm->Life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (pEm->Life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != pEm->D_life_u) {
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
			/* DS divides by 0x50 / 0x69 with the signed mult-high sequence (no sign fix-up) */
			i_tmp = (int)run_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			if (Pad_act_flg == 0) {
				PL_MULHI(i_hi, i_tmp, 0x66666667);
				run_spd = i_hi >> 5;
			}
			else {
				PL_MULHI(i_hi, i_tmp, -0x63f63f63);
				run_spd = (i_hi + i_tmp) >> 6;
			}
			pEm->Cdir_y = run_spd + pEm->Cdir_y;
		}
		if ((Key & 8) != 0) {
			/* DS divides by 0x50 / 0x69 with the signed mult-high sequence (no sign fix-up) */
			i_tmp = (int)run_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
			if (Pad_act_flg == 0) {
				PL_MULHI(i_hi, i_tmp, 0x66666667);
				run_spd = i_hi >> 5;
			}
			else {
				PL_MULHI(i_hi, i_tmp, -0x63f63f63);
				run_spd = (i_hi + i_tmp) >> 6;
			}
			pEm->Cdir_y -= run_spd;
		}
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((pEm->Sca_info & 0x100000) != 0) {
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

/* DS 0x80010eec, pl_mv03 */
const UCHAR Pl_backwalk_mot[3] = {
	0x00, 0x09, 0x09,	/* +0x0 */
};

/* DS 0x8003d0e8 (pl_mv02) */
void pl_mv02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UCHAR mot_no;
	short spd_x;
	UCHAR rnd;
	PARTS_W *pParts;
	UCHAR mot_tbl[3] = { 1, 4, 7 };	/* local copy of Pl_run_mot (DS 0x80010ee4) */
	SVECTOR esp_ofs;
	int i_hi;

	if (pEm->Routine_2 == 0) {
		mot_no = mot_tbl[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = mot_no + 0x70000;
		G.Status_flg &= 0xffffff3f;
		G.Room_player_flg |= 4;
	}
	if (pEm->Routine_2 == 1) {
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
				rnd = Rnd();
				Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				rnd = Rnd();
				Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
		if ((pEm->Id == 13) && ((pEm->Move_cnt & 7) == 7)) {
			pParts = pEm->pSin_parts_ptr;
			esp_ofs.vx = 0x100 - Rnd() * 2;
			esp_ofs.vy = -0x2e8 - Rnd() * 2;
			esp_ofs.vz = 0x100 - Rnd() * 2;
			Esp_call(0x1262000, pEm->Cdir_y - (Rnd() * 2 + -0x900), &pParts->Workm, &esp_ofs);
		}
	}
	if (pEm->D_life_u < 2) {
		pEm->Spd_x = pEm->Spd_x + -0x11;
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		spd_x = pEm->Spd_x;
		PL_MULHI(i_hi, (int)spd_x, 0x55555556);	/* DS: bare mult/mfhi for the divide by 3 */
		pEm->Spd_x = spd_x - ((short)i_hi - (spd_x >> 0xf));
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
	char prev_life_u;
	int mul_hi;
	int i_tmp;
	UCHAR bw_spd[3] = { 0x28, 0x10, 0x10 };	/* local copy of Pl_backwalk_spd (DS 0x80010ee8) */
	VECTOR unused_vec;

	(void)unused_vec;
	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		prev_life_u = pEm->D_life_u;
		pEm->D_life_u = 0;
		if (pEm->Life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (pEm->Life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != pEm->D_life_u) {
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
			i_tmp = (UINT)bw_spd[(signed char)pEm->D_life_u] * (UINT)s__800a2694[(signed char)UnkMvPadFlg];
			if (Pad_act_flg == 0) {
				mul_hi = (int)((ULONGLONG)((LONGLONG)i_tmp * -0x49f49f49) >> 0x20);
			}
			else {
				mul_hi = (int)((ULONGLONG)((LONGLONG)i_tmp * -0x63f63f63) >> 0x20);
			}
			pEm->Cdir_y = (short)((mul_hi + i_tmp) >> 6) + pEm->Cdir_y;
		}
		if ((Key & 8) != 0) {
			if (Pad_act_flg == 0) {
				i_tmp = -0x49f49f49;
			}
			else {
				i_tmp = -0x63f63f63;
			}
			pEm->Cdir_y = pEm->Cdir_y -
						  (short)((int)((int)((ULONGLONG)
											  ((LONGLONG)
											   (int)((UINT)bw_spd[(signed char)pEm->D_life_u] *
													(UINT)s__800a2694[(signed char)UnkMvPadFlg]) * (LONGLONG)i_tmp) >>
											 0x20) +
									   (UINT)bw_spd[(signed char)pEm->D_life_u] * (UINT)s__800a2694[(signed char)UnkMvPadFlg]) >> 6);
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

/* DS 0x800a2694: pad speed scale, indexed by UnkMvPadFlg (12 bytes) */
UCHAR  s__800a2694[] = {
	0x00, 0x10, 0x1f, 0x2d, 0x3a, 0x48, 0x52, 0x5a,
	0x5e, 0x62, 0x69, 0x00,
};

/* DS 0x800a4005 (Pl_poison_tint) */
UCHAR Pl_poison_tint = 0x00;

/* DS 0x8003d5f4 (pl_mv03) */
void pl_mv03(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UCHAR b_tmp;
	short spd_x;
	UCHAR u_tmp;
	ULONG ck_res;
	PARTS_W *pParts;
	UINT div7;
	SVECTOR esp_ofs;
	int Move_chg;				/* 1 when Move_cnt was rewritten below and Spd_get must run twice */

	if (pEm->Routine_2 == 0) {
		b_tmp = Pl_backwalk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(UINT *)&pEm->Move_no = b_tmp + 0x70000;
		G.Status_flg &= 0xffffff3f;
		pEm->Timer2 = 0;
		pEm->Timer3 = 0;
		G.Room_player_flg |= 2;
	}
	if (pEm->Routine_2 == 1) {
		if ((pEm->Timer3 == 0) && (pEm->D_life_u == 0)) {
			Move_chg = 0;
			ck_res = Lim_dir_ck(pEm, 5000, 800);
			if (ck_res == 0) {
				if (pEm->Timer2 != 0) {
					b_tmp = pEm->Move_cnt;
					div7 = b_tmp / 7;
					pEm->Move_no = 0;
					pEm->Timer2 = 0;
					pEm->Hokan_flg = 7;
					pEm->Move_cnt = 0;
					u_tmp = (char)((div7 + ((b_tmp - div7) >> 1)) >> 2) * 7;
					Move_chg = 1;
				}
			}
			else if (pEm->Timer2 == 0) {
				u_tmp = pEm->Move_cnt;
				pEm->Move_no = 1;
				pEm->Timer2 = 1;
				pEm->Hokan_flg = 7;
				pEm->Move_cnt = 0;
				Move_chg = 1;
			}
			if (Move_chg) {
				Spd_get(pEm, Kan, Seq, 0);
				pEm->Move_cnt = u_tmp;
				Spd_get(pEm, Kan, Seq, 0);
			}
			pEm->Timer3 = 15;
		}
		pEm->Timer3 = pEm->Timer3 + -1;
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
				u_tmp = Rnd();
				Esp_call((u_tmp * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
			}
			pParts = pEm->pSin_parts_ptr;
			if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
				u_tmp = Rnd();
				Esp_call((u_tmp * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xe].Workm, &esp_ofs);
			}
		}
	}
	if ((int)pEm->Water < pEm->Ground + -500) {
		spd_x = pEm->Spd_x;
		pEm->Spd_x = spd_x - ((short)((ULONGLONG)((LONGLONG)(int)spd_x * 0x55555556) >> 0x20) - (spd_x >> 0xf));
	}
	Add_speedXZ(pEm, 0);
	return;
}

/* DS 0x8003d96c (Lim_dir_ck) */
ULONG Lim_dir_ck(ENEMY_WORK *pEm, ULONG Lim, short Dir)
{
	UCHAR cdir_res;
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
				cdir_res = Cdir_ck(pEm, pT);
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
	ENEMY_WORK *pCand;
	UINT cand_dist;
	ENEMY_WORK **ppEm;
	UINT min_dist;
	ENEMY_WORK *pSec;
	ENEMY_WORK *pNear;

	pNear = (ENEMY_WORK *)0x0;
	min_dist = 0x7fffffff;
	pSec = (ENEMY_WORK *)0x0;
	if ((pEm->Neck_flg & 1) != 0) {
		return;
	}
	if (Pl_neck_timer == 0) {
		Pl_neck_timer = 0x2d;
		if ((G.Enemy_num != 0) || ((G.Status_flg & STAT_SUB_PLAYER) != 0)) {
			ppEm = G.pEnemy + 1;
			do {
				pCand = *ppEm;
				if (((pCand->Be_flg & 1) != 0) && ((pCand->Type & 0xc000) == 0)) {
					cand_dist = pCand->L_pl;
					line_res = Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
										(VECTOR *)pCand->pSin_parts_ptr[pCand->Neck_no].Workm.t,
										0x2080, 1);
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
	Pl_neck_timer--;
	return;
}

/* DS 0x800A3FE4 (Pld_no): PLxx_PLD names (FILECODE.H), by enemy Id */
USHORT Pld_no[16] = {
	PL00_PLD, PL01_PLD, PL02_PLD, PL03_PLD,
	PL04_PLD, PL05_PLD, PL06_PLD, PL07_PLD,
	PL08_PLD, PL09_PLD, PL0A_PLD, PL0B_PLD,
	PL0C_PLD, PL0D_PLD, PL0E_PLD, PL0F_PLD,
};

/* DS 0x8003dd48 (pl_mv04_pad) */
void pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
	char prev_life_u;
	int rot_spd;
	int i_tmp;
	int i_hi;
	ULONG ck_res;
	UCHAR rot_spd_tbl[3] = { 0x50, 0x30, 0x20 };	/* local copy of Pl_rot_spd (DS 0x80010ef0) */
	VECTOR unused_vec;

	(void)unused_vec;
	if ((pEm->Routine_2 == 0) || (pEm->Move_cnt == 0)) {
		prev_life_u = pEm->D_life_u;
		pEm->D_life_u = 0;
		if (pEm->Life < 0x65) {
			pEm->D_life_u = 1;
		}
		if (G.Pl_poison_timer != 0) {
			pEm->D_life_u = 1;
		}
		if (pEm->Life < 0x15) {
			pEm->D_life_u = 2;
		}
		if (prev_life_u != pEm->D_life_u) {
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
		/* DS divides by 100 / 0x69 with the signed mult-high sequence and no sign fix-up (the product is
		   never negative), so the quotient is formed from the high word of the product with the magic constant */
		i_tmp = (int)rot_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		if (Pad_act_flg == 0) {
			PL_MULHI(i_hi, i_tmp, 0x51eb851f);
			rot_spd = i_hi >> 5;
		}
		else {
			PL_MULHI(i_hi, i_tmp, -0x63f63f63);
			rot_spd = (i_hi + i_tmp) >> 6;
		}
		pEm->Cdir_y = rot_spd + pEm->Cdir_y;
	}
	if ((Key & 8) != 0) {
		/* DS divides by 100 / 0x69 with the signed mult-high sequence and no sign fix-up (the product is
		   never negative), so the quotient is formed from the high word of the product with the magic constant */
		i_tmp = (int)rot_spd_tbl[(signed char)pEm->D_life_u] * (int)s__800a2694[(signed char)UnkMvPadFlg];
		if (Pad_act_flg == 0) {
			PL_MULHI(i_hi, i_tmp, 0x51eb851f);
			rot_spd = i_hi >> 5;
		}
		else {
			PL_MULHI(i_hi, i_tmp, -0x63f63f63);
			rot_spd = (i_hi + i_tmp) >> 6;
		}
		pEm->Cdir_y -= rot_spd;
	}
	if (((Key & 0x80) != 0) || ((Key_trg & 0x80) != 0)) {
		if ((pEm->Sca_info & 0x100000) != 0) {
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
	short mot_no;
	UCHAR rnd;
	PARTS_W *pParts;
	SHORT walk_mot[3] = { 0, 3, 6 };	/* local copy of Pl_walk_mot (DS 0x80010ed8) */
	SVECTOR esp_ofs;

	if (pEm->Routine_2 == 0) {
		mot_no = walk_mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		*(int *)&pEm->Move_no = mot_no + 0x70000;
		G.Status_flg &= 0xffffff3f;
	}
	else if (pEm->Routine_2 != 1) {
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
			rnd = Rnd();
			Esp_call((rnd * 4 + 0x60cU) | 0x1a000000, pEm->Cdir_y, &pParts[0xb].Workm, &esp_ofs);
		}
		pParts = pEm->pSin_parts_ptr;
		if ((int)pEm->Water < pParts[0xe].Workm.t[1] + 300) {
			rnd = Rnd();
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
		pEm->Move_no = 6;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
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
		pEm->Move_no = 6;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 5;
		/* fall through */
	case 5:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x10200);
		break;
	case 6:
		*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		G.Status_flg &= 0xffffff3f;
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
	short timer1_old;
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
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
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
		if (pEm->Timer2 != 0) {
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
			timer1_old = pEm->Timer1;
			mot_no = pl_mv07_mot[(signed char)pEm->Timer2];
			pEm->Timer1 = timer1_old - 1;
			*(UINT *)&pEm->Move_no = mot_no + 1;
			if (timer1_old == 1) {
				pEm->Routine_2 = 5;
				*(UINT *)&pEm->Move_no = pl_mv07_mot[(signed char)pEm->Timer2] + 2;
			}
		}
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Timer2 == 0) {
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
		if ((jnt_ret != 0) && (timer1_old = pEm->Timer1, pEm->Timer1 = timer1_old - 1, timer1_old == 1)) {
			pEm->Routine_2 = 5;
			*(UINT *)&pEm->Move_no = pl_mv07_mot[(signed char)pEm->Timer2] + 2;
		}
		Add_speedXYZ(pEm, 0, 0);
		Sv.vx = (pEm->Timer2 == 0) ? -200 : 200;
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
		Sv.vx = (pEm->Timer2 == 0) ? -200 : 200;
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
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 7;
		pEm->Timer0 = 200;
		if (pEm->Timer2 == 0) {
			pEm->Timer0 = -200;
		}
		/* fall through */
	case 7:
		if (pEm->Timer2 != 0) {
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
	USHORT cdir_step;
	UCHAR rnd;
	UCHAR cnt_tgt;
	ULONG jnt_ret;
	UCHAR mv_cnt;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Move_no = 6;
		if ((pEm->Routine_2 & 2) != 0) {
			pEm->Move_no = 7;
		}
		pEm->Hokan_flg = 7;
		pEm->Move_cnt = 0;
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Spd_x = 0x32;
		G.Status_flg |= STAT_PL_BUSY;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) == 0) {
			cdir_step = -(cdir_y >> 2 & 0xff);
		}
		else {
			cdir_step = cdir_y >> 2 & 0xff;
		}
		pEm->Cdir_y = cdir_y + cdir_step;
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
		if (pEm->Id == 15) {
			mv_cnt = pEm->Move_cnt;
			if (mv_cnt == 57) {
				rnd = Rnd();
				if ((rnd & 3) != 0) {
					pEm->Move_no = 6;
					pEm->Move_cnt = 86;
					pEm->Hokan_flg = 5;
					pEm->Mplay_flg = 0;
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
		else {
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
		if (mv_cnt == cnt_tgt) {
			pEm->Damage_cnt &= 0x7f;
			pEm->Be_flg &= 0xfffffff3;
		}
		jnt_ret = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x400);
		pEm->Routine_3 = pEm->Routine_3 + (char)jnt_ret;
		if (((pEm->Water < pEm->Ground) && (pEm->Move_cnt < 0xf)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		/* fall through */
	default:
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(1, (short)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		}
		break;
	case 4:
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 1000);
		}
		pEm->Routine_1 = 0;
		pEm->Routine_2 = 0;
		pEm->Routine_3 = 0;
		G.Status_flg &= ~STAT_PL_BUSY;
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
		pEm->Move_no = 7;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (char)res;
		if (pEm->Move_cnt == ((pEm->Id == 15) ? 26 : 14)) {
			pEm->Routine_3 = 2;
		}
		break;
	case 2:
		pEm->Routine_3 = 3;
		pEm->Move_no = 8;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 0;
		pEm->Mplay_flg = 0;
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
		pEm->nFloor += 0xff;
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
		pEm->nFloor = -(char)(pEm->Pos_y / 0x708);
		pOm = G.Ob_model;
		if (G.Ob_model < G.pObj_work_end) {
			do {
				if (((pOm->Be_flg & 1) != 0) && ((pOm->Be_flg & 2) == 0)) {
					Oma_set_ofs(pOm);
					res = Omd_in_check((VECTOR *)&pEm->Pos_x, pOm, (UINT)pEm->Atd[0].At_w, 0);
					if ((res != 0) && ((int)pEm->Ground == pOm->Atd[0].Y - (UINT)pOm->Atd[0].At_h)) {
						pEm->pOn_om = pOm;
						break;
					}
				}
				pOm++;
			} while (pOm < G.pObj_work_end);
		}
		if (pEm->pOn_om != (OBJ_MODEL_WORK *)0x0) {
			Oma_ob_pull2((ENEMY_WORK *)&G.Player, pEm->pOn_om, (UINT)pEm->Sc_id << 8, 0x30);
		}
		Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if ((pEm->Water < pEm->Ground) && (pEm->Timer3 == 0)) {
			pEm->Timer3 = -0x7b;
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
		pEm->nFloor = -(char)(pEm->Pos_y / 0x708);
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
	USHORT cdir_step;
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
		if ((cdir_y & 0x200) == 0) {
			cdir_step = -(cdir_y >> 2 & 0xff);
		}
		else {
			cdir_step = cdir_y >> 2 & 0xff;
		}
		pEm->Cdir_y = cdir_y + cdir_step;
		if ((pEm->Cdir_y & 0x3e0U) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_2 = 2;
		}
		break;
	case 2:
		pEm->Routine_2 = 3;
		pEm->Move_no = 7;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 3:
		pEm->Routine_2 = pEm->Routine_2 + (char)Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 4:
		pEm->Routine_2 = 5;
		pEm->Move_no = 8;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
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
			Foot_set_pl(pEm, 1, (UINT)(pEm->Id == 15));
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
	UCHAR routine2;
	USHORT cdir_y;
	short timer0_old;
	USHORT cdir_step;
	UCHAR dummy_tbl[2] = { 0, 3 };	/* DS stores {0, 3} at sp+0x10; never read (dead store) */
	SVECTOR dummy_sv;		/* DS copies Pl_mv07_init_sv to sp+0x18; never read (dead store) */

	/* DS copies the 8-byte init vector with lwl/lwr pairs */
	*(BYTES8 *)&dummy_sv = *(BYTES8 *)&Pl_mv07_init_sv;
	(void)dummy_tbl;
	(void)dummy_sv;
	routine2 = pEm->Routine_2;
	if (routine2 == 0) {
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		G.Status_flg |= STAT_PL_BUSY;
		pEm->Damage_cnt |= 0x80;
		routine2 = 1;
	}
	if (routine2 == 1) {
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) == 0) {
			cdir_step = -(cdir_y >> 2 & 0xff);
		}
		else {
			cdir_step = cdir_y >> 2 & 0xff;
		}
		pEm->Cdir_y = cdir_y + cdir_step;
		if ((pEm->Cdir_y & 0x3e0U) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_2 = 2;
			pEm->Timer0 = 0x2d;
		}
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
	}
	else if (routine2 == 2) {
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		timer0_old = pEm->Timer0;
		pEm->Timer0 = timer0_old - 1;
		if (timer0_old == 0) {
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
		}
		Add_speedXZ(pEm, 0);
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
	UCHAR routine2;
	ULONG jnt_ret;
	UINT snd_code;

	routine2 = pEm->Routine_2;
	if (routine2 != 1) {
		if (1 < routine2) {
			if (routine2 != 2) {
				return;
			}
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
			pEm->Damage_cnt = 0;
			pEm->Neck_flg &= 0xed;
			pEm->Status_flg |= 2;
			return;
		}
		if (routine2 != 0) {
			return;
		}
		pEm->Move_no = 5;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		snd_code = 0x4010000;
		if (pEm->Routine_1 == 0) {
			snd_code = 0x4000000;
		}
		Snd_se_stad(snd_code | 1, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg |= 0x12;
		pEm->Be_flg &= 0xfffffffb;
	}
	jnt_ret = Joint_move(pEm, Kan, Seq, 0x400);
	pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
	return;
}

/* DS 0x8003f778 (pl_dm01) */
void pl_dm01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	UCHAR routine2;
	ULONG jnt_ret;

	routine2 = pEm->Routine_2;
	if (routine2 != 1) {
		if (1 < routine2) {
			if (routine2 != 2) {
				return;
			}
			*(ULONG *)&pEm->Routine_0 = 0x1;	/* DS stores the four Routine bytes as one word */
			pEm->Damage_cnt = 0;
			pEm->Neck_flg &= 0xed;
			pEm->Status_flg |= 2;
			return;
		}
		if (routine2 != 0) {
			return;
		}
		pEm->Routine_2 = 1;
		pEm->Spd_z = 0;
		*(UINT *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30003;
		*(UINT *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 + -200;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(((pEm->Routine_1 & 1) + 1) * 0x10000 | 0x4000001, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg |= 0x12;
		pEm->Be_flg &= 0xfffffffb;
	}
	if (pEm->Hokan_flg != 0) {
		Add_speedXZ(pEm, 0);
	}
	jnt_ret = Joint_move(pEm, Kan, Seq, 0x400);
	pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
	return;
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
		pEm->Move_no = 2;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		pEm->Move_no = 3;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		pEm->Move_no = 4;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		pEm->Move_no = 1;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 10;
		pEm->Damage_cnt = 0;
		/* fall through */
	case 10:
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		if (res == 0) {
			return;
		}
		pEm->Routine_2 = 11;
		pEm->Move_no = 6;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
	short spd_new;
	ULONG res;
	int timer0_new;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_no = 5;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		if (pEm->Hokan_flg == 0) {
			spd_new = pEm->Spd_x + -0x32;
		}
		else {
			spd_new = pEm->Spd_x + -0x14;
		}
		pEm->Spd_x = spd_new;
		if (pEm->Spd_x < 0) {
			pEm->Spd_x = 0;
		}
		break;
	case 2:
		pEm->Move_no = 4;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
		timer0_new = ((USHORT)pEm->Timer0 - 1) + res * -3;
		pEm->Timer0 = (short)timer0_new;
		if (timer0_new * 0x10000 < 0) {
			pEm->Routine_2 = 4;
		}
		break;
	case 4:
		pEm->Move_no = 1;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 5;
		pEm->Damage_cnt = 0;
		/* fall through */
	case 5:
		res = Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x400);
		if (res == 0) {
			return;
		}
		pEm->Routine_2 = 6;
		pEm->Move_no = 6;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 3;
		pEm->Mplay_flg = 0;
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
	UCHAR routine2;
	ULONG jnt_ret;

	routine2 = pEm->Routine_2;
	if (routine2 == 0) {
		pEm->Move_no = 2;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(0x4030001, (VECTOR *)&pEm->Pos_x);
		pEm->Neck_flg = 1;
		Pad_vib_set0(3, 0);
		Pad_vib_set1(8, 200, 0);
		Pad_vib_fade_set(0x5a, 200, 0, 8);
		routine2 = 1;
	}
	if (routine2 == 1) {
		jnt_ret = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_2 = pEm->Routine_2 + (char)jnt_ret;
	}
	else if (routine2 == 2) {
		*(ULONG *)&pEm->Routine_0 = 0x7;	/* DS stores the four Routine bytes as one word */
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
	UCHAR routine1;
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
	int Life;

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
	Life = (UINT)(USHORT)G.Player.Life - Down_life;
	G.Player.Life = (short)Life;
	if (Mode == 1) {
		if (-1 < Life * 0x10000) {
			return 0;
		}
	}
	else {
		if (Mode != 0) {
			return 1;
		}
		Life = Life * 0x10000 >> 0x10;
		if (-1 < Life) {
			return 0;
		}
		if ((Life < -0xe) || ((G.Player.Status_flg & 0x1000) != 0)) {
			G.System_flg |= SYS_GAME_OVER;
			G.Player.Damage_cnt |= 0x80;
			return 2;
		}
	}
	G.Player.Life = 0;
	G.Player.Status_flg |= 0x1000;
	return 1;
}

/* DS 0x800402f4 (Pl_life_up) */
ULONG Pl_life_up(long Up_life)
{
	UINT Life;
	ULONG new_life;
	UCHAR Frame_pad[8];	/* retail reserves 8 bytes: addiu sp,-0x8 in the beq delay slot (0x80040338), released at 0x80040344; no retail instruction addresses them */

	(void)Frame_pad;
	Life = (UINT)(USHORT)G.Player.Life;
	G.Player.Life = (short)(Life + Up_life);
	G.Player.Status_flg &= 0xefff;
	new_life = (int)((Life + Up_life) * 0x10000) >> 0x10;
	if ((int)G.Player.Max_life < (int)new_life) {
		G.Player.Life = G.Player.Max_life;
	}
	return new_life;
}

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

/* DS 0x80040350 (Pl_view_esp_set) - camera-view Esp spawn (only when G.Enemy_num < 3).
   pParts is part 11 of the player (pSin_parts_ptr + 11); its Workm is the view matrix. */
void Pl_view_esp_set(ENEMY_WORK *pEm, PARTS_W *pParts)
{
	RCUT        *pCut;
	PL_VIEW_OFS *pOfs;
	UCHAR        view_no;
	UCHAR        r;
	int          i;
	short        Dir;
	SVECTOR      Dist;
	MATRIX       Rot;
	SVECTOR      Svec;
	VECTOR       Left;
	VECTOR       Right;
	VECTOR       Dir_vec;
	VECTOR       Norm;

	view_no = Pl_view_no_tbl[pEm->Type & 0xfff];
	if ((G.Enemy_num >= 3) || (view_no == 0)) {
		return;
	}
	pOfs = &Pl_view_ofs_tbl[view_no - 1];
	pCut = &G.pRoom->pRcut[G.SaveData.Cut_no];

	Dir = Direction_ck((short)pCut->View_p[0], (short)pCut->View_p[2],
					   (short)pCut->View_r[0], (short)pCut->View_r[2]);

	Dist.vx = pOfs->Ofs_dist;
	Dist.vy = 0;
	Dist.vz = 0;
	Rot.m[0][0] = 0x1000;	Rot.m[0][1] = 0;		Rot.m[0][2] = 0;
	Rot.m[1][0] = 0;		Rot.m[1][1] = 0x1000;	Rot.m[1][2] = 0;
	Rot.m[2][0] = 0;		Rot.m[2][1] = 0;		Rot.m[2][2] = 0x1000;
	RotMatrixY(Dir + 0x400, &Rot);
	ApplyMatrixSV(&Rot, &Dist, &Svec);

	Left.vx  = (pCut->View_p[0] + Svec.vx - pParts->Workm.t[0]) >> 4;
	Left.vy  = ((pCut->View_p[1] + pOfs->Ofs_dist) - pParts->Workm.t[1] - pOfs->Ofs_height) >> 4;
	Left.vz  = (pCut->View_p[2] + Svec.vz - pParts->Workm.t[2]) >> 4;
	Right.vx = (pCut->View_p[0] - Svec.vx - pParts->Workm.t[0]) >> 4;
	Right.vy = ((pCut->View_p[1] - pOfs->Ofs_dist) - pParts->Workm.t[1] - pOfs->Ofs_height) >> 4;
	Right.vz = (pCut->View_p[2] - Svec.vz - pParts->Workm.t[2]) >> 4;

	Svec.vx = pOfs->Dir_vx;
	Svec.vy = pOfs->Dir_vy;
	Svec.vz = 0;
	ApplyMatrix(&pParts->Workm, &Svec, &Norm);
	OuterProduct0(&Norm, &Left, &Left);
	OuterProduct0(&Norm, &Right, &Right);

	if ((Left.vy < 0) && (Right.vy >= 0) &&
		((Left.vx ^ Right.vx) < 0) && ((Left.vz ^ Right.vz) < 0)) {
		Dir_vec.vx = pCut->View_r[0] - pCut->View_p[0];
		Dir_vec.vy = pCut->View_r[1] - pCut->View_p[1];
		Dir_vec.vz = pCut->View_r[2] - pCut->View_p[2];
		VectorNormal(&Dir_vec, &Dir_vec);

		r = Rnd();
		Svec.vx = ((short)(Dir_vec.vx >> 3) + (short)pCut->View_p[0]) - (r - 0x80);
		r = Rnd();
		Svec.vy = ((short)(Dir_vec.vy >> 3) + (short)pCut->View_p[1]) - (r - 0x80);
		r = Rnd();
		Svec.vz = ((short)(Dir_vec.vz >> 3) + (short)pCut->View_p[2]) - (r - 0x80);
		Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);

		if (view_no != 1) {
			Dist.vx = (short)(Dir_vec.vx >> 3) + (short)pCut->View_p[0];
			Dist.vy = (short)(Dir_vec.vy >> 3) + (short)pCut->View_p[1];
			Dist.vz = (short)(Dir_vec.vz >> 3) + (short)pCut->View_p[2];
			for (i = 0; i < 5; i++) {
				r = Rnd();
				Svec.vx = Dist.vx - (r * 2 - 0x100);
				r = Rnd();
				Svec.vy = Dist.vy - (r * 2 - 0x100);
				r = Rnd();
				Svec.vz = Dist.vz - (r * 2 - 0x100);
				Esp_call(0x70002c8, 0, (MATRIX *)&GsIDMATRIX, &Svec);
			}
		}
	}
}

/* DS 0x800a4004 (Pl_neck_timer) */
UCHAR Pl_neck_timer = 0x2d;

/* DS 0x800a40b4 (Pl_kage_pos) */
VECTOR Pl_kage_pos;

/* DS 0x800d785c (UnkMvPadFlg) */
CHAR UnkMvPadFlg = 0;

/* DS 0x800d7660 (gpPl_pac_ptr) */
ULONG(*gpPl_pac_ptr)[1] = 0;

/* DS 0x800d7840 (gpPl_tmd_ptr) */
ULONG(*gpPl_tmd_ptr)[1] = 0;

/* DS 0x800d7858 (gpPl_pac2_ptr) */
ULONG(*gpPl_pac2_ptr)[1] = 0;

/* DS 0x800d8cec (gpPl_tmd2_ptr) */
ULONG(*gpPl_tmd2_ptr)[1] = 0;

/* Tables moved here from PL_WEP.C / PL_WEP2.C */
/* DS 0x800a6350 (PLW00_W) */
ATTACK_WORK3 PLW00_W[3] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};

/* DS 0x800a63a4 (PLW00_S) */
ATTACK_SEQ PLW00_S[2] = {
	{ 0x00, 0xff }, { 0x00, 0x00 }
};

/* DS 0x800a6618 (PLW02_W) */
ATTACK_WORK3 PLW02_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x0177, 0x1004, 0x0000, 0x03e8, 0x01f4, 0x1fa4, 0x0000, 0x09c4, 0x0271 },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x0177, 0x1068, 0x0000, 0x03e8, 0x01f4, 0x2008, 0x0000, 0x1d4c, 0x0271 },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x03e8, 0x0177, 0x1004, 0x0000, 0x03e8, 0x01f4, 0x2008, 0x0000, 0x09c4, 0x0271 }
};

/* DS 0x800a666c (PLW02_S) */
ATTACK_SEQ PLW02_S[2] = {
	{ 0x00, 0x01 }, { 0x00, 0xff }
};

/* DS 0x800a66c4 (PLW05_S) */
ATTACK_SEQ PLW05_S[2] = {
	{ 0x00, 0x01 }, { 0x00, 0xff }
};

/* DS 0x800a6670 (PLW05_W) */
ATTACK_WORK3 PLW05_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x007d, 0x1004, 0x0000, 0x03e8, 0x0096, 0x1fa4, 0x0000, 0x09c4, 0x00af },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x007d, 0x1068, 0x0000, 0x03e8, 0x0096, 0x1c20, 0x0000, 0x1d4c, 0x00af },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x03e8, 0x007d, 0x1004, 0x0000, 0x03e8, 0x0096, 0x1fa4, 0x0000, 0x09c4, 0x00af }
};

/* DS 0x800a6724 (PLW07_W) */
ATTACK_WORK3 PLW07_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x01f4, 0x1004, 0x0000, 0x03e8, 0x02ee, 0x1fa4, 0x0000, 0x09c4, 0x03e8 },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x0271, 0x1068, 0x0000, 0x03e8, 0x036b, 0x2008, 0x0000, 0x1d4c, 0x0465 },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x02ee, 0x01f4, 0x0c1c, 0x0000, 0x05dc, 0x02ee, 0x238c, 0x0000, 0x09c4, 0x03e8 }
};

/* DS 0x800a6778 (PLW07_S) */
ATTACK_SEQ PLW07_S[8] = {
	{ 0x00, 0x01 }, { 0xff, 0x01 }, { 0x00, 0x01 }, { 0xff, 0x01 },
	{ 0x00, 0x01 }, { 0xff, 0x01 }, { 0x00, 0xff }, { 0x00, 0x00 }
};

/* DS 0x800a6788 (PLW08_W) */
ATTACK_WORK3 PLW08_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x01f4, 0x1004, 0x0000, 0x03e8, 0x036b, 0x1fa4, 0x0000, 0x09c4, 0x0465 },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x0271, 0x1068, 0x0000, 0x03e8, 0x03e8, 0x2008, 0x0000, 0x1d4c, 0x04e2 },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x02ee, 0x01f4, 0x0c1c, 0x0000, 0x03e8, 0x036b, 0x1bbc, 0x0000, 0x09c4, 0x0465 }
};

/* DS 0x800a67dc (PLW08_S) */
ATTACK_SEQ PLW08_S[10] = {
	{ 0x00, 0x01 }, { 0xff, 0x01 }, { 0x00, 0x01 }, { 0xff, 0x01 },
	{ 0x00, 0x01 }, { 0xff, 0x01 }, { 0x00, 0x01 }, { 0xff, 0x01 },
	{ 0x00, 0xff }, { 0x00, 0x00 }
};

/* DS 0x800a67f0 (PLW0E_W) */
ATTACK_WORK3 PLW0E_W[3] = {
	{ 0x00, 0x04, 0x00, 0x00, 0x00c8, 0x0000, 0x0dac, 0x01f4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x08, 0x00, 0x00, 0x00c8, 0x0000, 0x0dac, 0x01f4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x00, 0x10, 0x00, 0x00, 0x00c8, 0x0000, 0x0dac, 0x01f4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};

/* DS 0x800a6844 (PLW0E_S) */
ATTACK_SEQ PLW0E_S[2] = {
	{ 0x00, 0x01 }, { 0x00, 0xff }
};

/* DS 0x800a6848 (PLW0F_W) */
ATTACK_WORK3 PLW0F_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x0177, 0x1004, 0x0000, 0x03e8, 0x01f4, 0x1fa4, 0x0000, 0x09c4, 0x0271 },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x0177, 0x1068, 0x0000, 0x03e8, 0x01f4, 0x2008, 0x0000, 0x1d4c, 0x0271 },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x03e8, 0x0177, 0x1004, 0x0000, 0x03e8, 0x01f4, 0x2008, 0x0000, 0x09c4, 0x0271 }
};

/* DS 0x800a689c (PLW0F_S) */
ATTACK_SEQ PLW0F_S[4] = {
	{ 0x00, 0x01 }, { 0xff, 0x02 }, { 0xfe, 0xff }, { 0x00, 0x00 }
};

/* DS 0x800a68a4 (PLW12_W) */
ATTACK_WORK3 PLW12_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x01f4, 0x1004, 0x0000, 0x03e8, 0x0271, 0x1fa4, 0x0000, 0x09c4, 0x0271 },
	{ 0x00, 0x08, 0x04, 0x10, 0x00c8, 0x0000, 0x1d4c, 0x01f4, 0x00c8, 0x0000, 0x1d4c, 0x01f4, 0x00c8, 0x0000, 0x1d4c, 0x01f4 },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x03e8, 0x0177, 0x1004, 0x0000, 0x03e8, 0x01f4, 0x2008, 0x0000, 0x09c4, 0x0271 }
};

/* DS 0x800a68f8 (PLW12_S) */
ATTACK_SEQ PLW12_S[4] = {
	{ 0x00, 0x02 }, { 0xff, 0x01 }, { 0xfe, 0xff }, { 0x00, 0x00 }
};

/* DS 0x800a66c8 (PLW13_W) */
ATTACK_WORK3 PLW13_W[3] = {
	{ 0x00, 0x04, 0x02, 0x01, 0x0064, 0x0000, 0x03e8, 0x007d, 0x1004, 0x0000, 0x03e8, 0x0096, 0x1fa4, 0x0000, 0x09c4, 0x00af },
	{ 0x00, 0x08, 0x08, 0x08, 0x00c8, 0x0000, 0x03e8, 0x007d, 0x1068, 0x0000, 0x03e8, 0x0096, 0x1c20, 0x0000, 0x1d4c, 0x00af },
	{ 0x00, 0x10, 0x20, 0x40, 0x0064, 0x0000, 0x03e8, 0x007d, 0x1004, 0x0000, 0x03e8, 0x0096, 0x1fa4, 0x0000, 0x09c4, 0x00af }
};

/* DS 0x800a671c (PLW13_S) */
ATTACK_SEQ PLW13_S[4] = {
	{ 0x00, 0x01 }, { 0x00, 0x01 }, { 0x00, 0x01 }, { 0x00, 0xff }
};

/* DS 0x800a6900 (Pl_weapon_tbl) */
ATTACK_WEAPON_TBL Pl_weapon_tbl[19][3] = {
	{ { Pl_w012, Pl_w01_s2 }, { Pl_w011, Pl_w01_s1 }, { Pl_w010, Pl_w01_s0 } },
	{ { PLW02_W, PLW02_S }, { &PLW02_W[1], PLW02_S }, { &PLW02_W[2], PLW02_S } },
	{ { PLW02_W, PLW02_S }, { &PLW02_W[1], PLW02_S }, { &PLW02_W[2], PLW02_S } },
	{ { PLW02_W, PLW02_S }, { &PLW02_W[1], PLW02_S }, { &PLW02_W[2], PLW02_S } },
	{ { PLW05_W, PLW05_S }, { &PLW05_W[1], PLW05_S }, { &PLW05_W[2], PLW05_S } },
	{ { PLW13_W, PLW13_S }, { &PLW13_W[1], PLW13_S }, { &PLW13_W[2], PLW13_S } },
	{ { PLW07_W, PLW07_S }, { &PLW07_W[1], PLW07_S }, { &PLW07_W[2], PLW07_S } },
	{ { PLW08_W, PLW08_S }, { &PLW08_W[1], PLW08_S }, { &PLW08_W[2], PLW08_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW02_W, PLW02_S }, { &PLW02_W[1], PLW02_S }, { &PLW02_W[2], PLW02_S } },
	{ { PLW0E_W, PLW0E_S }, { &PLW0E_W[1], PLW0E_S }, { &PLW0E_W[2], PLW0E_S } },
	{ { PLW0F_W, PLW0F_S }, { &PLW0F_W[1], PLW0F_S }, { &PLW0F_W[2], PLW0F_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S }, { PLW00_W, PLW00_S } },
	{ { PLW12_W, PLW12_S }, { &PLW12_W[1], PLW12_S }, { &PLW12_W[2], PLW12_S } },
	{ { PLW02_W, PLW02_S }, { &PLW02_W[1], PLW02_S }, { &PLW02_W[2], PLW02_S } }
};
