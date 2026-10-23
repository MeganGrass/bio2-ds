/*
 * SPL.C - Spl / Aida / Sherry enemy state routines
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL.C
 */
#include "SPL.H"
#include "TYPES.H"
#include "MAIN.H"
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <MEMORY.H>
#include "SPL_AUTO.H"
#include "SPL_SCE.H"
#include "SPL_SP.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "OBA.H"
#include "SABUN.H"
#include "PLAYER.H"
#include "SUB.H"
#include "flag.h"
#include "SPL_TBL.H"

/* file-local prototypes */
static VOID Spl_mv40(register ENEMY_WORK *pEm);
static VOID Spl_mv42(register ENEMY_WORK *pEm);
static VOID Spl_mv41(register ENEMY_WORK *pEm);
static VOID Spl_mv44(register ENEMY_WORK *pEm);
static VOID Spl_mv45(register ENEMY_WORK *pEm);
static VOID Spl_mv47(register ENEMY_WORK *pEm);
static VOID Spl_mv49(register ENEMY_WORK *pEm);
static VOID Spl_mv48(register ENEMY_WORK *pEm);
static VOID Spl_mv4a(register ENEMY_WORK *pEm);
static VOID Spl_mv4b(register ENEMY_WORK *pEm);
static VOID Spl_mv50(register ENEMY_WORK *pEm);
static VOID Spl_mv51(register ENEMY_WORK *pEm);
static VOID Spl_mv00(register ENEMY_WORK *pEm);
static VOID Spl_Init(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_in40(register ENEMY_WORK *pEm);
static VOID Spl_in42(register ENEMY_WORK *pEm);
static VOID Spl_in41(register ENEMY_WORK *pEm);
static VOID Spl_in44(register ENEMY_WORK *pEm);
static VOID Spl_in45(register ENEMY_WORK *pEm);
static VOID Spl_in47(register ENEMY_WORK *pEm);
static VOID Spl_in48(register ENEMY_WORK *pEm);
static VOID Spl_in4a(register ENEMY_WORK *pEm);
static VOID Spl_in4b(register ENEMY_WORK *pEm);
static VOID Spl_in50(register ENEMY_WORK *pEm);
static VOID Spl_in_jelly_hair(register ENEMY_WORK *pEm);
static VOID Spl_in51(register ENEMY_WORK *pEm);
static VOID Spl_in00(register ENEMY_WORK *pEm);
static VOID Spl_Damage(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_Die(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_Dead(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_dead00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_chenge00(ENEMY_WORK *pEm);
static VOID Spl_chenge01(ENEMY_WORK *pEm);
static VOID Spl_kizu00(register ENEMY_WORK *pEm);
static VOID Spl_chenge02(register ENEMY_WORK *pEm);
static VOID Spl_chenge03(ENEMY_WORK *pEm);
static VOID Spl_chenge04(register ENEMY_WORK *pEm);
static VOID Spl_chenge05(register ENEMY_WORK *pEm);
static VOID Spl_chenge06(register ENEMY_WORK *pEm);
static VOID Spl_chenge07(register ENEMY_WORK *pEm);
static VOID Spl_chenge08(register ENEMY_WORK *pEm);
static VOID Spl_chenge09(register ENEMY_WORK *pEm);
static VOID Spl_chenge10(register ENEMY_WORK *pEm);

/* Dispatch tables: initialised data in SLUS_007.48, one DS address per table */
/* DS 0x800a83b8 (Spl_routine_0) */
VOID *Spl_routine_0[9] = {
	(VOID *)Spl_Init,			/* 0 */
	(VOID *)Spl_Auto,					/* 1: Spl_Auto (DS 0x800605ac) */
	(VOID *)Spl_Damage,			/* 2 */
	(VOID *)Spl_Die,			/* 3 */
	(VOID *)Spl_Scenario,			/* 4 */
	0,					/* 5 (NULL in retail) */
	0,					/* 6 (NULL in retail) */
	(VOID *)Spl_Dead,			/* 7 */
	(VOID *)Spl_Sp_Move,			/* 8 */
};
/* DS 0x800a83dc (Spl_mv), indexed by Id - 0x40 */
VOID *Spl_mv[28] = {
	(VOID *)Spl_mv40,
	(VOID *)Spl_mv41,
	(VOID *)Spl_mv42,
	(VOID *)Spl_mv41,
	(VOID *)Spl_mv44,
	(VOID *)Spl_mv45,
	(VOID *)Spl_mv44,
	(VOID *)Spl_mv47,
	(VOID *)Spl_mv48,
	(VOID *)Spl_mv49,
	(VOID *)Spl_mv4a,
	(VOID *)Spl_mv4b,
	(VOID *)Spl_mv00,
	(VOID *)Spl_mv00,
	(VOID *)Spl_mv00,
	(VOID *)Spl_mv45,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
	(VOID *)Spl_mv50,
	(VOID *)Spl_mv51,
};
/* DS 0x800a844c (Spl_in), indexed by Id - 0x40 */
VOID *Spl_in[28] = {
	(VOID *)Spl_in40,
	(VOID *)Spl_in41,
	(VOID *)Spl_in42,
	(VOID *)Spl_in41,
	(VOID *)Spl_in44,
	(VOID *)Spl_in45,
	(VOID *)Spl_in44,
	(VOID *)Spl_in47,
	(VOID *)Spl_in48,
	(VOID *)Spl_in47,
	(VOID *)Spl_in4a,
	(VOID *)Spl_in4b,
	(VOID *)Spl_in00,
	(VOID *)Spl_in00,
	(VOID *)Spl_in00,
	(VOID *)Spl_in45,
	(VOID *)Spl_in50,
	(VOID *)Spl_in51,
	(VOID *)Spl_in50,
	(VOID *)Spl_in51,
	(VOID *)Spl_in50,
	(VOID *)Spl_in51,
	(VOID *)Spl_in50,
	(VOID *)Spl_in51,
	(VOID *)Spl_in50,
	(VOID *)Spl_in51,
	(VOID *)Spl_in_jelly_hair,
	(VOID *)Spl_in51,
};
/* DS 0x800a84bc (Spl_dm): retail = { Spl_dm00, Spl_dm00, Spl_dm01, Spl_dm01 } */
VOID Spl_dm00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
VOID Spl_dm01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
VOID Spl_die00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
CODE_PTR Spl_dm[4] = { Spl_dm00, Spl_dm00, Spl_dm01, Spl_dm01 };
/* DS 0x800a84cc (Spl_die): retail = { Spl_die00 } */
CODE_PTR Spl_die[1] = { Spl_die00 };
/* DS 0x800a84d0 (Spl_dead) */
VOID *Spl_dead[1] = {
	(VOID *)Spl_dead00,
};

/* Per-frame sub-player update: route and root check first, then (unless the stop mask is set) the
 * routine for Routine_0, the neck rotation, the motion for Id (Spl_mv indexed from 0x40), the object
 * hit check, and the enemy-vs-stage check. Sherry (Ids 0x45, 0x4f) passes check id 0x800 to Sca_ck_em;
 * the others pass 0x1000. The stop mask is 0x40000000 for the sub-player and 0x20000000 for the rest.
 * DS reads G.Stop_flg separately in each arm of the sub-player test and calls Sca_ck_em from each arm
 * of the Sherry test (the compiler then merges the common tails). */
/* DS 0x8005cfc8 (Spl) */
VOID Spl(register ENEMY_WORK *pEm)
{
	ULONG stop_mask;
	ULONG flg;
	ULONG Tv0[4];
	SVECTOR Sv0;

	(void)Tv0;
	(void)Sv0;
	Spl_Root_ck_sys(pEm);
	if (pEm == G.pEnemy[1]) {
		flg = G.Stop_flg;
		stop_mask = 0x40000000;
	} else {
		flg = G.Stop_flg;
		stop_mask = 0x20000000;
	}
	if ((flg & stop_mask) == 0) {
		((CODE_PTR)(Spl_routine_0[pEm->Routine_0]))(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
		Rot_neck(pEm, pEm->Cdir_y);
		((CODE_PTR)(Spl_mv[pEm->Id - 0x40]))(pEm);
		Oba_ck_em(pEm);
		if ((pEm->Id == 0x45) || (pEm->Id == 0x4f)) {
			Sca_ck_em(pEm, 0x800);
		} else {
			Sca_ck_em(pEm, 0x1000);
		}
	}
}

/* DS 0x8005d0bc (Spl_mv40) */
static VOID Spl_mv40(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;

	pParts = pEm->pSin_parts_ptr;
	/* M and Pos_x..Pos_z are one 32-byte MATRIX block in PARTS_W (M at +0x18, Pos_x at +0x2c); DS copies it as a whole */
	*(MATRIX *)pParts[0x10].M = *(MATRIX *)pParts->M;
}

/* DS 0x8005d10c (Spl_mv42) */
static VOID Spl_mv42(register ENEMY_WORK *pEm)
{
	Spl_chenge04(pEm);
}

/* DS 0x8005d12c (Spl_mv41) */
static VOID Spl_mv41(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG *pPacket2Save;
	ULONG *pPacketSave;
	ULONG *pTmd2Save;
	ULONG *pTmdSave;
	ULONG *pPacket2B;

	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Type &= 0xf7ff;
		pTmdSave = pParts[0xf].pTmd;
		pTmd2Save = pParts[0xf].pTmd2;
		pPacketSave = pParts[0xf].pPacket;
		pPacket2Save = pParts[0xf].pPacket2;
		pParts[0xf].pTmd = pParts[0xb].pTmd;
		pParts[0xf].pTmd2 = pParts[0xb].pTmd2;
		pParts[0xf].pPacket = pParts[0xb].pPacket;
		pPacket2B = pParts[0xb].pPacket2;
		pParts[0xb].Be_flg = 1;
		pParts[0xf].Be_flg = 0;
		pParts[0xb].pTmd = pTmdSave;
		pParts[0xb].pTmd2 = pTmd2Save;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
		pParts[0xf].pPacket2 = pPacket2B;
	}
	Spl_chenge09(pEm);
	Spl_chenge10(pEm);
}

/* DS 0x8005d1d0 (Spl_mv44) */
static VOID Spl_mv44(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;

	pParts = pEm->pSin_parts_ptr;
	/* M and Pos_x..Pos_z are one 32-byte MATRIX block in PARTS_W (M at +0x18, Pos_x at +0x2c); DS copies it as a whole */
	*(MATRIX *)pParts[0xf].M = *(MATRIX *)pParts->M;
	Spl_chenge05(pEm);
}

/* DS 0x8005d238 (Spl_mv45) */
static VOID Spl_mv45(register ENEMY_WORK *pEm)
{
	if (Flg_ck(G.SaveData.Common_flg, 0xff) != 0) {
		Op_PartsSort06(pEm);
	}
}

/* DS 0x8005d27c (Spl_mv47) */
static VOID Spl_mv47(register ENEMY_WORK *pEm)
{
	Spl_chenge06(pEm);
}

/* DS 0x8005d29c (Spl_mv49) */
static VOID Spl_mv49(register ENEMY_WORK *pEm)
{
	Spl_chenge07(pEm);
}

/* DS 0x8005d2bc (Spl_mv48) */
static VOID Spl_mv48(register ENEMY_WORK *pEm)
{
	Spl_chenge02(pEm);
}

/* DS 0x8005d2dc (Spl_mv4a) */
static VOID Spl_mv4a(register ENEMY_WORK *pEm)
{
	Spl_chenge00(pEm);
	Spl_chenge01(pEm);
}

/* DS 0x8005d30c (Spl_mv4b) */
static VOID Spl_mv4b(register ENEMY_WORK *pEm)
{
}

/* DS 0x8005d314 (Spl_mv50) */
static VOID Spl_mv50(register ENEMY_WORK *pEm)
{
	if ((Flg_ck(G.SaveData.Zapping_flg, 5) == 0) && (Flg_ck(G.SaveData.Zapping_flg, 0xf) == 0) && ((pEm->Type & 0x100) != 0)) {
		pEm->Type &= 0xfeff;
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	}
}

/* DS 0x8005d3a4 (Spl_mv51) */
static VOID Spl_mv51(register ENEMY_WORK *pEm)
{
	Op_PartsSort05(pEm);
}

/* DS 0x8005d3c4 (Spl_mv00) */
static VOID Spl_mv00(register ENEMY_WORK *pEm)
{
}

/* DS 0x8005d3cc (Spl_Init) */
static VOID Spl_Init(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	PARTS_W *pParts;
	USHORT parts0_pos_y;
	ULONG id_a;
	/* DS 0x80011620 (56 bytes): parts0 y (Parts0_pos_y), index Id - 0x40 */
	USHORT parts0_y_tbl[28] = {
		4219, 4178, 4219, 4178, 4506, 4096, 4506, 4096,
		4342, 4096, 3927, 4096, 0, 0, 0, 4096,
		4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
		4096, 4096, 4096, 4096
	};
	/* DS 0x80011658 (112 bytes): ofs y (Atd Ofs_y, At_h) then at (Atd At_w, At_d, W, D); retail copies both as one block, index Id - 0x40 */
	struct {
		USHORT ofs_y[28];
		USHORT at[28];
	} atd_tbl = {
		{
			85, 75, 85, 75, 86, 45, 86, 75,
			85, 75, 85, 75, 0, 75, 0, 45,
			85, 75, 85, 75, 85, 75, 85, 75,
			85, 75, 85, 85
		},
		{
			500, 350, 500, 350, 86, 250, 86, 350,
			450, 350, 450, 350, 0, 0, 0, 250,
			450, 350, 450, 350, 450, 350, 450, 350,
			450, 350, 450, 450
		}
	};
	/* DS 0x800116c8 (112 bytes): kage lo then kage hi (Kage_work_set), one block, index Id - 0x40 */
	struct {
		SHORT lo[28];
		SHORT hi[28];
	} kage_tbl = {
		{
			500, 420, 500, 420, 500, 370, 500, 420,
			500, 420, 500, 420, 500, 500, 500, 370,
			500, 420, 500, 420, 500, 420, 500, 420,
			500, 420, 500, 420
		},
		{
			600, 520, 600, 520, 600, 460, 600, 520,
			600, 520, 600, 520, 600, 600, 600, 460,
			600, 520, 600, 520, 600, 520, 600, 520,
			600, 520, 600, 520
		}
	};

	*(UINT *)&pEm->Routine_0 = 0x1;		/* Routine_0 1, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
	parts0_pos_y = (parts0_y_tbl - 0x40)[pEm->Id];
	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Spl_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;
	pEm->pEnemy_ptr = (ULONG *)pEm;
	pEm->pEnemy_neck = &pEm->Be_flg;
	*(UINT *)&pEm->Move_no = 0x0;		/* Move_no 0, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
	pEm->Parts0_pos_y = parts0_pos_y;
	Joint_move(pEm, Kan, Seq, 0x100);
	if (pEm->Id != 0x4b) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Neck_no = 8;
		pEm->Neck_flg = 0;
		pParts[8].Free[2] = 0x60;
		pParts[8].Free[3] = 0x60;
		pParts[8].Free[4] = 0x200;
		pParts[8].Free[0] = 0;
		pParts[8].Free[1] = 0;
		pParts[8].Free[5] = 0x138;
		pEm->Neck_flg |= 0x92;
	}
	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	/* DS negates the table value before the multiply (subu, then sll 3, addu, sll 1): Ofs_y is -(value) * 18. */
	pEm->Atd[0].Ofs_y = -(atd_tbl.ofs_y - 0x40)[pEm->Id] * 0x12;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = (atd_tbl.at - 0x40)[pEm->Id];
	pEm->Atd[0].At_h = (atd_tbl.ofs_y - 0x40)[pEm->Id] * 0x12;
	pEm->Atd[0].At_d = (atd_tbl.at - 0x40)[pEm->Id];
	pEm->Atd[0].W = (atd_tbl.at - 0x40)[pEm->Id];
	pEm->Atd[0].D = (atd_tbl.at - 0x40)[pEm->Id];
	Kage_work_set((ULONG *)&pEm->pKage_work, 0, (INT)(kage_tbl.lo - 0x40)[pEm->Id] | ((INT)(kage_tbl.hi - 0x40)[pEm->Id] << 16), 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
	/* DS stores Type | 0x1000 and tests 0x400 on that stored value; the else branch's 0x4000 is folded with the
	   0x1000 into one ori 0x5000 of the Type loaded here. */
	pEm->Type |= 0x1000;
	if ((pEm->Type & 0x400) != 0) {
		G.Status_flg |= STAT_SUB_PLAYER;
		pEm->Type &= 0xefff;
	} else {
		/* DS loads Id once (lbu) before the Life stores and compares it without a further mask. */
		id_a = pEm->Id;
		pEm->Type |= 0x4000;
		pEm->Life = -1;
		pEm->Max_life = -1;
		if ((id_a == 65) || (id_a == 67) || (id_a == 69) || (id_a == 79)) {
			G.Status_flg &= ~STAT_SUB_PLAYER;
		}
	}
	if ((pEm->Type & 0x4000) != 0) {
		pEm->Routine_0 = 4;
		pEm->Routine_1 = 6;
	}
	((CODE_PTR)(Spl_in[pEm->Id - 0x40]))(pEm);
}

/* DS 0x8005d85c (Spl_in40) */
static VOID Spl_in40(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG *pSwap;
	ULONG *pSwap2;
	ULONG *pPacketSave;
	ULONG *pPacket2Save;
	LONG pos_x;
	LONG pos_y;
	LONG pos_z;

	pEm->pSin_parts_ptr[0xf].Be_flg = 0;
	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pSwap = pParts[0xf].pTmd;
		pSwap2 = pParts[0xf].pTmd2;
		pPacketSave = pParts[0xf].pPacket;
		pPacket2Save = pParts[0xf].pPacket2;
		pParts[0xf].Be_flg = 0;
		pParts[0xb].Be_flg = 1;
		pParts[0xb].pTmd = pSwap;
		pParts[0xb].pTmd2 = pSwap2;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
	}
	pParts = pEm->pSin_parts_ptr;
	pSwap = pParts->pSuper;
	pSwap2 = pParts->pOya_parts;
	pos_x = pParts->Pos_x;
	pos_y = pParts->Pos_y;
	pos_z = pParts->Pos_z;
	pParts[0x10].Be_flg = 0;
	pParts[0x10].Cdir_x = 0;
	pParts[0x10].Cdir_y = 0;
	pParts[0x10].Cdir_z = 0;
	pParts[0x10].pSuper = pSwap;
	pParts[0x10].pOya_parts = pSwap2;
	pParts[0x10].Pos_x = pos_x;
	pParts[0x10].Pos_y = pos_y;
	pParts[0x10].Pos_z = pos_z;
	RotMatrix((SVECTOR *)&pParts[0x10].Cdir_x, (MATRIX *)pParts[0x10].M);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, (USHORT)pEm->Tpage, 0, pEm->Clut + 0x1e0);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, (USHORT)pEm->Tpage, 0, pEm->Clut + 0x1e0);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute, (USHORT)pEm->Tpage, 0, pEm->Clut + 0x1e0);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, (USHORT)pEm->Tpage, 0, pEm->Clut + 0x1e0);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute, (USHORT)pEm->Tpage, 0, pEm->Clut + 0x1e0);
	pParts = pEm->pSin_parts_ptr;
	*(SHORT *)&pParts->pSa_dat_head[4] = 0;
	Sa_parts_allmv(pEm, pParts);
	pParts = pEm->pSin_parts_ptr;
	*(SHORT *)&pParts[0x10].pSa_dat_head[4] = 0;
	Sa_parts_allmv(pEm, &pParts[0x10]);
	if ((pEm->Type & 0x200) != 0) {
		pParts = pEm->pSin_parts_ptr;
		*(SHORT *)&pParts->pSa_dat_head[4] = 0x1000;
		Sa_parts_allmv(pEm, pParts);
		pParts = pEm->pSin_parts_ptr;
		pParts[0x10].Be_flg |= 1;
		*(SHORT *)&pParts[0x10].pSa_dat_head[4] = 0x1000;
		Sa_parts_allmv(pEm, &pParts[0x10]);
		Spl_chenge08(pEm);
	}
}

/* DS 0x8005da3c (Spl_in42) */
static VOID Spl_in42(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG *pTmdSave;
	ULONG *pTmd2Save;
	ULONG *pPacketSave;
	ULONG *pPacket2Save;

	pEm->pSin_parts_ptr[0xf].Be_flg = 0;
	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pTmdSave = pParts[0xf].pTmd;
		pTmd2Save = pParts[0xf].pTmd2;
		pPacketSave = pParts[0xf].pPacket;
		pPacket2Save = pParts[0xf].pPacket2;
		pParts[0xf].Be_flg = 0;
		pParts[0xb].Be_flg = 1;
		pParts[0xb].pTmd = pTmdSave;
		pParts[0xb].pTmd2 = pTmd2Save;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
	}
}

/* DS 0x8005da94 (Spl_in41) */
static VOID Spl_in41(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG flg;
	ULONG *pPacket2Save;
	ULONG *pPacketSave;
	ULONG *pTmd2Save;
	ULONG *pTmdSave;
	ULONG *pPacket2B;
	USHORT life;

	if ((pEm->Type & 0x400) != 0) {
		pEm->Type |= 0x800;
	}
	pEm->pSin_parts_ptr[0xf].Be_flg = 0;
	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Type &= 0xf7ff;
		pTmdSave = pParts[0xf].pTmd;
		pTmd2Save = pParts[0xf].pTmd2;
		pPacketSave = pParts[0xf].pPacket;
		pPacket2Save = pParts[0xf].pPacket2;
		pParts[0xf].pTmd = pParts[0xb].pTmd;
		pParts[0xf].pTmd2 = pParts[0xb].pTmd2;
		pParts[0xf].pPacket = pParts[0xb].pPacket;
		pPacket2B = pParts[0xb].pPacket2;
		pParts[0xb].Be_flg = 1;
		pParts[0xf].Be_flg = 0;
		pParts[0xb].pTmd = pTmdSave;
		pParts[0xb].pTmd2 = pTmd2Save;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
		pParts[0xf].pPacket2 = pPacket2B;
	}
	pParts = pEm->pSin_parts_ptr;
	pParts[0x10].pSuper = (ULONG *)&pParts->Workm;
	pParts[0x10].Pos_x = 0x42;
	pParts[0x10].Be_flg = 0;
	pParts[0x10].pOya_parts = &pParts->Be_flg;
	pParts[0x10].Pos_y = -0x28e;
	pParts[0x10].Pos_z = 0;
	pParts[0x10].Cdir_x = 0;
	pParts[0x10].Cdir_y = 0;
	pParts[0x10].Cdir_z = 0;
	pParts[0x10].Free[0] = 0;
	pParts[0x10].Free[1] = 0;
	flg = Flg_ck(G.SaveData.Scenario_flg, 0x50);
	if (flg != 0) {
		pParts[0x10].Be_flg = 1;
	}
	pEm->pSin_parts_ptr->Be_flg |= 0x4000;
	pEm->pTbefore_func = Aida_before;
	pEm->pTafter_func = Aida_after;
	if ((pEm->Type & 0x80) == 0) {
		G.pFree_work = (UCHAR *)Aida_Jh_init(pEm, (ULONG)G.pFree_work);
	}
	pEm->Type |= 0x80;
	if ((G.Status_flg & STAT_SUB_PLAYER) != 0) {
		pEm->pEnemy_neck = &G.Player.Be_flg;
		pEm->Neck_flg = 0;
		pEm->Routine_0 = 1;
		pEm->Routine_1 = 0;
		life = G.SaveData.Dummy[0];
		pEm->Max_life = 200;
		pEm->Life = life;
	}
	SPL_FREE(pEm)->Flg = 0;
	SPL_FREE(pEm)->Timer4 = 0;
	SPL_FREE(pEm)->Turn_dir = 0;
	SPL_FREE(pEm)->Blink = 0;
	SPL_FREE(pEm)->Cool = 0;
	pEm->D_life_u = 0;
}

/* DS 0x8005dc74 (Spl_in44) */
static VOID Spl_in44(register ENEMY_WORK *pEm)
{
	PARTS_W *pBase;
	PARTS_W *pParts;
	ULONG *pSuperSave;
	ULONG *pOyaSave;
	LONG pos_x;
	LONG pos_y;
	LONG pos_z;

	pBase = pEm->pSin_parts_ptr;
	pSuperSave = pBase->pSuper;
	pOyaSave = pBase->pOya_parts;
	pos_x = pBase->Pos_x;
	pos_y = pBase->Pos_y;
	pos_z = pBase->Pos_z;
	pBase[0xf].Be_flg = 0;
	pBase[0xf].Cdir_x = 0;
	pBase[0xf].Cdir_y = 0;
	pBase[0xf].Cdir_z = 0;
	pBase[0xf].pSuper = pSuperSave;
	pBase[0xf].pOya_parts = pOyaSave;
	pBase[0xf].Pos_x = pos_x;
	pBase[0xf].Pos_y = pos_y;
	pBase[0xf].Pos_z = pos_z;
	RotMatrix((SVECTOR *)&pBase[0xf].Cdir_x, (MATRIX *)pBase[0xf].M);
	pParts = pEm->pSin_parts_ptr;
	Texture_Move((MODEL_WORK *)&pParts->Attribute, pEm->Tpage + 1, 0, pEm->Clut + 0x1e1);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 1, 0, pEm->Clut + 0x1e1);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 1, 0, pEm->Clut + 0x1e1);
	pParts = pEm->pSin_parts_ptr;
	*(SHORT *)&pParts->pSa_dat_head[3] = 0;
	Sa_parts_allmv(pEm, pParts);
	pParts = pEm->pSin_parts_ptr;
	*(SHORT *)&pParts[0xf].pSa_dat_head[3] = 0;
	Sa_parts_allmv(pEm, &pParts[0xf]);
	if ((pEm->Type & 0x200) != 0) {
		pParts = pEm->pSin_parts_ptr;
		*(SHORT *)&pParts->pSa_dat_head[3] = 0x1000;
		Sa_parts_allmv(pEm, pParts);
		pParts = pEm->pSin_parts_ptr;
		pParts[0xf].Be_flg |= 1;
		*(SHORT *)&pParts[0xf].pSa_dat_head[3] = 0x1000;
		Sa_parts_allmv(pEm, &pParts[0xf]);
		pEm->Type |= 0x100;
	}
}

/* DS 0x8005dddc (Spl_in45) */
static VOID Spl_in45(register ENEMY_WORK *pEm)
{
	USHORT life;
	ULONG be_flg;
	ULONG flg;
	PARTS_W *pParts;

	if ((G.Status_flg & STAT_SUB_PLAYER) != 0) {
		pEm->pEnemy_neck = &G.Player.Be_flg;
		pEm->Neck_flg = 0;
		pEm->Routine_0 = 1;
		pEm->Routine_1 = 0;
		life = G.SaveData.Dummy[0];
		pEm->Max_life = 200;
		pEm->Life = life;
	}
	pParts = pEm->pSin_parts_ptr;
	SPL_FREE(pEm)->Flg = 0;
	SPL_FREE(pEm)->Timer4 = 0;
	SPL_FREE(pEm)->Turn_dir = 0;
	SPL_FREE(pEm)->Blink = 0;
	SPL_FREE(pEm)->Cool = 0;
	pEm->D_life_u = 0;
	pParts[0xf].pSuper = (ULONG *)&pParts->Workm;
	pParts[0xf].Pos_x = 8;
	pParts[0xf].Be_flg = 0;
	pParts[0xf].pOya_parts = &pParts->Be_flg;
	pParts[0xf].Pos_y = -0x1ad;
	pParts[0xf].Pos_z = 0;
	pParts[0xf].Cdir_x = 0;
	pParts[0xf].Cdir_y = 0;
	pParts[0xf].Cdir_z = 0;
	RotMatrix((SVECTOR *)&pParts[0xf].Cdir_x, (MATRIX *)pParts[0xf].M);
	be_flg = pParts[0xf].Be_flg;
	pParts[0xf].Free[0] = 0;
	pParts[0xf].Free[1] = 0;
	pParts[0xf].Be_flg = be_flg | 0x40;
	flg = Flg_ck(G.SaveData.Common_flg, 0xff);
	if (flg != 0) {
		pParts[0xf].Be_flg |= 1;
	}
}

/* DS 0x8005ded8 (Spl_in47) */
static VOID Spl_in47(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG *pTmdSave;
	ULONG *pTmd2Save;
	ULONG *pPacketSave;
	ULONG *pPacket2Save;

	pEm->pSin_parts_ptr[0xf].Be_flg = 0;
	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pTmdSave = pParts[0xf].pTmd;
		pTmd2Save = pParts[0xf].pTmd2;
		pPacketSave = pParts[0xf].pPacket;
		pPacket2Save = pParts[0xf].pPacket2;
		pParts[0xf].Be_flg = 0;
		pParts[0xb].Be_flg = 1;
		pParts[0xb].pTmd = pTmdSave;
		pParts[0xb].pTmd2 = pTmd2Save;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
	}
}

/* DS 0x8005df30 (Spl_in48) */
static VOID Spl_in48(register ENEMY_WORK *pEm)
{
}

/* DS 0x8005df38 (Spl_in4a) */
static VOID Spl_in4a(register ENEMY_WORK *pEm)
{
	UCHAR part_a;
	PARTS_W *pParts;
	PARTS_W *pPartner;
	PARTS_W *pPartA;
	UCHAR *pRow;
	ULONG attr;
	ULONG *pSwap;
	ULONG *pSwap2;
	ULONG *pPacketSave;
	ULONG *pPacket2Save;
	INT idx;

	((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;
	pEm->pSin_parts_ptr[0x17].Be_flg = 0;
	if ((pEm->Type & 0x800) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pSwap = pParts[0x17].pTmd;
		pSwap2 = pParts[0x17].pTmd2;
		pPacketSave = pParts[0x17].pPacket;
		pPacket2Save = pParts[0x17].pPacket2;
		pParts[0x17].Be_flg = 0;
		pParts[0xb].Be_flg = 1;
		pParts[0xb].pTmd = pSwap;
		pParts[0xb].pTmd2 = pSwap2;
		pParts[0xb].pPacket = pPacketSave;
		pParts[0xb].pPacket2 = pPacket2Save;
	}
	idx = 7;
	if ((pEm->Type & 0x300) == 0) {
		do {
			idx--;
			pRow = Em4a_zobiparts_tbl[idx];
			pPartA = &pEm->pSin_parts_ptr[pRow[0]];
			pPartner = &pEm->pSin_parts_ptr[pRow[1]];
			Late_set((MODEL_WORK *)&pPartA->Attribute, 1);
			Late_set((MODEL_WORK *)&pPartner->Attribute, 1);
			pSwap2 = pPartA->pTmd;
			pPartA->pTmd = pPartner->pTmd;
			pSwap = pPartner->pTmd2;
			pPartner->pTmd = pSwap2;
			pSwap2 = pPartA->pTmd2;
			pPartA->pTmd2 = pSwap;
			pSwap = pPartner->pPacket;
			pPartner->pTmd2 = pSwap2;
			pSwap2 = pPartA->pPacket;
			pPartA->pPacket = pSwap;
			pSwap = pPartner->pPacket2;
			pPartner->pPacket = pSwap2;
			pSwap2 = pPartA->pPacket2;
			pPartA->pPacket2 = pSwap;
			pPartner->pPacket2 = pSwap2;
			pPartner->pSuper = pPartA->pSuper;
			pPartner->pOya_parts = pPartA->pOya_parts;
			memcpy(pPartner->M, pPartA->M, sizeof(pPartA->M));
			pPartner->Pos_x = pPartA->Pos_x;
			pPartner->Pos_y = pPartA->Pos_y;
			pPartner->Pos_z = pPartA->Pos_z;
			attr = pPartA->Attribute;
			pPartA->Timer0 = 0;
			pPartA->Poly_rgb = 0;
			pPartA->Attribute = attr | 2;
		} while (idx != 0);
	} else {
		do {
			idx--;
			pRow = Em4a_zobiparts_tbl[idx];
			pPartner = &pEm->pSin_parts_ptr[pRow[1]];
			pPartA = &pEm->pSin_parts_ptr[pRow[0]];
			pPartA->pTmd = pPartner->pTmd;
			pPartA->pTmd2 = pPartner->pTmd2;
			pPartA->pPacket = pPartner->pPacket;
			pPartA->pPacket2 = pPartner->pPacket2;
			pPartner->Be_flg = 0;
		} while (idx != 0);
		pEm->Type &= 0xfcff;
	}

}

/* DS 0x8005e1c0 (Spl_in4b) */
static VOID Spl_in4b(register ENEMY_WORK *pEm)
{
	pEm->pSin_parts_ptr->Pos_y = 0;
	pEm->Neck_flg = 1;
	pEm->Routine_0 = 4;
	pEm->Neck_no = 0;
	pEm->Routine_1 = 0x14;
	pEm->Be_flg |= 10;
}

/* DS 0x8005e1f8 (Spl_in50) */
static VOID Spl_in50(register ENEMY_WORK *pEm)
{
}

/* DS 0x8005e200 (Spl_in_jelly_hair): marks the body part as jelly-hair and hooks Pl_jh_move / Aida_after */
static VOID Spl_in_jelly_hair(register ENEMY_WORK *pEm)
{
	pEm->pSin_parts_ptr->Be_flg |= 0x4000;
	pEm->pTbefore_func = Pl_jh_move;
	pEm->pTafter_func = Aida_after;
	if ((pEm->Type & 0x80) == 0) {
		G.pFree_work = (UCHAR *)Pl_jh_init(pEm, (ULONG)G.pFree_work);
	}
	SPL_FREE(pEm)->Flg = 0;
	SPL_FREE(pEm)->Timer4 = 0;
	SPL_FREE(pEm)->Turn_dir = 0;
	SPL_FREE(pEm)->Blink = 0;
	pEm->D_life_u = 0;
	pEm->Type |= 0x80;
}

/* DS 0x8005e2a0 (Spl_in51) */
static VOID Spl_in51(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG be_flg15;
	ULONG be_flg16;

	pParts = pEm->pSin_parts_ptr;
	pParts[0xf].Pos_x = -0x28;
	pParts[0xf].Pos_y = -0x170;
	pParts[0x10].Pos_x = -0x30;
	pParts[0x10].Pos_y = -0x20;
	pParts[0x11].Pos_x = -0x5b;
	pParts[0x11].Pos_y = 0x61;
	pParts[0x12].Pos_x = -1;
	pParts[0x12].Pos_y = 0x7f;
	be_flg15 = pParts[0xf].Be_flg;
	be_flg16 = pParts[0x10].Be_flg;
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
	pParts[0xf].Be_flg = be_flg15 | 0x41;
	pParts[0x10].Be_flg = be_flg16 | 0x41;
	pParts[0x11].Be_flg |= 0x41;
	pParts[0x12].Be_flg |= 0x41;
}

/* DS 0x8005e358 (Spl_in00) */
static VOID Spl_in00(register ENEMY_WORK *pEm)
{
}

/* DS 0x8005e360 (Spl_Damage) */
static VOID Spl_Damage(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Spl_dm[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x8005e39c (Spl_Die) */
static VOID Spl_Die(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Spl_die[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x8005e3d8 (Spl_Dead) */
static VOID Spl_Dead(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Spl_dead[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x8005e414 (Spl_dead00) */
static VOID Spl_dead00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
}

/* DS 0x8005e41c (Spl_set) */
VOID Spl_set(VOID)
{
	G.Em_move_tbl[0x30] = Spl;
	G.Em_move_tbl[0x31] = Spl;
	G.Em_move_tbl[0x32] = Spl;
	G.Em_move_tbl[0x33] = Spl;
	G.Em_move_tbl[0x34] = Spl;
	G.Em_move_tbl[0x35] = Spl;
	G.Em_move_tbl[0x36] = Spl;
	G.Em_move_tbl[0x37] = Spl;
	G.Em_move_tbl[0x38] = Spl;
	G.Em_move_tbl[0x39] = Spl;
	G.Em_move_tbl[0x3a] = Spl;
	G.Em_move_tbl[0x3b] = Spl;
	G.Em_move_tbl[0x3c] = Spl;
	G.Em_move_tbl[0x3d] = Spl;
	G.Em_move_tbl[0x3e] = Spl;
	G.Em_move_tbl[0x3f] = Spl;
	G.Em_move_tbl[0x40] = Spl;
	G.Em_move_tbl[0x41] = Spl;
	G.Em_move_tbl[0x42] = Spl;
	G.Em_move_tbl[0x43] = Spl;
	G.Em_move_tbl[0x44] = Spl;
	G.Em_move_tbl[0x45] = Spl;
	G.Em_move_tbl[0x46] = Spl;
	G.Em_move_tbl[0x47] = Spl;
	G.Em_move_tbl[0x48] = Spl;
	G.Em_move_tbl[0x49] = Spl;
	G.Em_move_tbl[0x4a] = Spl;
	G.Em_move_tbl[0x4b] = Spl;
}


/* DS 0x8005e50c (Spl_chenge00) */
static VOID Spl_chenge00(ENEMY_WORK *pEm)
{
	PARTS_W *pPart;
	ULONG *pSwap;
	ULONG poly_rgb;
	PARTS_W *pParts;
	PARTS_W *pPartner;
	INT k;

	if ((pEm->Type & 0x200) != 0) {
		for (k = 7; k != 0; k--) {
			pParts = pEm->pSin_parts_ptr;
			pPart = pParts + Em4a_zobiparts_tbl[k - 1][0];
			pPartner = pParts + Em4a_zobiparts_tbl[k - 1][1];
			if ((*pPart).Timer0 == 0) {
				(*pPart).Poly_rgb += 0x10101;
				pPartner->Poly_rgb -= 0x10101;
				if (((*pPart).Poly_rgb & 0xffffff) == 0x404040) {
					(*pPart).Timer0 = 1;
					pSwap = (*pPart).pTmd;
					(*pPart).pTmd = pPartner->pTmd;
					pPartner->pTmd = pSwap;
					pSwap = (*pPart).pTmd2;
					(*pPart).pTmd2 = pPartner->pTmd2;
					pPartner->pTmd2 = pSwap;
					pSwap = (*pPart).pPacket;
					(*pPart).pPacket = pPartner->pPacket;
					pPartner->pPacket = pSwap;
					pSwap = (*pPart).pPacket2;
					(*pPart).pPacket2 = pPartner->pPacket2;
					pPartner->pPacket2 = pSwap;
				}
			}
			if ((*pPart).Timer0 == 1) {
				(*pPart).Poly_rgb -= 0x10101;
				pPartner->Poly_rgb += 0x10101;
				if (((*pPart).Poly_rgb & 0xffffff) == 0) {
					(*pPart).Timer0 = 2;
					pEm->Type &= 0xfdff;
					pSwap = (*pPart).pTmd;
					(*pPart).pTmd = pPartner->pTmd;
					pPartner->pTmd = pSwap;
					pSwap = (*pPart).pTmd2;
					(*pPart).pTmd2 = pPartner->pTmd2;
					pPartner->pTmd2 = pSwap;
					pSwap = (*pPart).pPacket;
					(*pPart).pPacket = pPartner->pPacket;
					pPartner->pPacket = pSwap;
					pSwap = (*pPart).pPacket2;
					(*pPart).pPacket2 = pPartner->pPacket2;
					pPartner->pPacket2 = pSwap;
					poly_rgb = (*pPart).Poly_rgb;
					(*pPart).Poly_rgb = pPartner->Poly_rgb;
					pPartner->Poly_rgb = poly_rgb;
					(*pPart).Attribute &= 0xfffffffd;
					pPartner->Be_flg = 0;
				}
			}
		}
	}
}

/* DS 0x8005e6f0 (Spl_chenge01) */
static VOID Spl_chenge01(ENEMY_WORK *pEm)
{
	*(MATRIX *)pEm->pSin_parts_ptr[0x11].M = *(MATRIX *)pEm->pSin_parts_ptr[8].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0x12].M = *(MATRIX *)pEm->pSin_parts_ptr[0xc].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0xf].M = *(MATRIX *)pEm->pSin_parts_ptr[0xd].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0x13].M = *(MATRIX *)pEm->pSin_parts_ptr[0xe].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0x14].M = *(MATRIX *)pEm->pSin_parts_ptr[9].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0x15].M = *(MATRIX *)pEm->pSin_parts_ptr[10].M;
	*(MATRIX *)pEm->pSin_parts_ptr[0x16].M = *(MATRIX *)pEm->pSin_parts_ptr[0xb].M;
}

/* DS 0x8005e8f0 (Spl_chenge03) */
static VOID Spl_chenge03(ENEMY_WORK *pEm)
{
	PARTS_W *pPart;
	ULONG *pSwap;
	ULONG poly_rgb;
	PARTS_W *pParts;
	PARTS_W *pPartner;
	INT k;

	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		for (k = 7; k != 0; k--) {
			pParts = pEm->pSin_parts_ptr;
			pPart = pParts + Em4a_zobiparts_tbl[k - 1][0];
			pPartner = pParts + Em4a_zobiparts_tbl[k - 1][1];
			if ((*pPart).Timer0 == 0) {
				(*pPart).Poly_rgb = 0x404040;
				pPartner->Poly_rgb = 0x404040;
				if (((*pPart).Poly_rgb & 0xffffff) == 0x404040) {
					(*pPart).Timer0 = 1;
					pSwap = (*pPart).pTmd;
					(*pPart).pTmd = pPartner->pTmd;
					pPartner->pTmd = pSwap;
					pSwap = (*pPart).pTmd2;
					(*pPart).pTmd2 = pPartner->pTmd2;
					pPartner->pTmd2 = pSwap;
					pSwap = (*pPart).pPacket;
					(*pPart).pPacket = pPartner->pPacket;
					pPartner->pPacket = pSwap;
					pSwap = (*pPart).pPacket2;
					(*pPart).pPacket2 = pPartner->pPacket2;
					pPartner->pPacket2 = pSwap;
				}
			}
			if ((*pPart).Timer0 == 1) {
				(*pPart).Poly_rgb = 0;
				pPartner->Poly_rgb = 0x808080;
				if (((*pPart).Poly_rgb & 0xffffff) == 0) {
					(*pPart).Timer0 = 2;
					pEm->Type &= 0xfeff;
					pSwap = (*pPart).pTmd;
					(*pPart).pTmd = pPartner->pTmd;
					pPartner->pTmd = pSwap;
					pSwap = (*pPart).pTmd2;
					(*pPart).pTmd2 = pPartner->pTmd2;
					pPartner->pTmd2 = pSwap;
					pSwap = (*pPart).pPacket;
					(*pPart).pPacket = pPartner->pPacket;
					pPartner->pPacket = pSwap;
					pSwap = (*pPart).pPacket2;
					(*pPart).pPacket2 = pPartner->pPacket2;
					pPartner->pPacket2 = pSwap;
					poly_rgb = (*pPart).Poly_rgb;
					(*pPart).Poly_rgb = pPartner->Poly_rgb;
					pPartner->Poly_rgb = poly_rgb;
					(*pPart).Attribute &= 0xfffffffd;
					pPartner->Be_flg = 0;
				}
			}
		}
	}
}

/* DS 0x8005eaac (Spl_kizu00) */
static VOID Spl_kizu00(register ENEMY_WORK *pEm)
{
	ULONG (*pOt)[16];
	DR_MOVE *pPrim;
	UINT tpage;
	INT y;
	RECT rect;

	if ((pEm->Type & 0x100) != 0) {
		tpage = (UINT)pEm->Tpage;
		pEm->Type &= 0xfeff;
		y = 0;
		if (tpage > 0xf) {
			tpage -= 0x10;
			y = 0x100;
		}
		rect.x = (SHORT)(tpage * 0x40) + 0x80;
		rect.w = 0x40;
		rect.y = (SHORT)y;
		rect.h = 0x100;
		pPrim = (DR_MOVE *)&EM40_Prim;
		SetDrawMove(pPrim, &rect, tpage * 0x40 + 0x40, y);
		pOt = G.Ot2 + G.Cbuf_id;
		EM40_Prim.tag = (EM40_Prim.tag & 0xff000000) | ((*pOt)[0] & 0xffffff);
		(*pOt)[0] = ((*pOt)[0] & 0xff000000) | ((ULONG)pPrim & 0xffffff);
	}
}

/* DS 0x8005eb80 (Spl_chenge02) */
static VOID Spl_chenge02(register ENEMY_WORK *pEm)
{
	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[1].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[2].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[3].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[5].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[6].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	}
}

/* DS 0x8005ecfc (Spl_chenge04) */
static VOID Spl_chenge04(register ENEMY_WORK *pEm)
{
	PARTS_W *pParts;
	ULONG *pTmd2Save;
	ULONG *pPacketSave;
	ULONG *pPacket2Save;
	ULONG *pTmdSave;
	ULONG *pMw;
	/* DS 0x80011738: count 0x20, then 32 poly ids (Texture_Move_GT3) */
	UCHAR kuchi3[] = {
		0x20, 0x01, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x15,
		0x16, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
		0x21, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x29,
		0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
		0x43
	};
	/* DS 0x8001175c: count 0x0b, then 11 poly ids (Texture_Move_GT4) */
	UCHAR kuchi4[] = {
		0x0b, 0x08, 0x0a, 0x0e, 0x10, 0x11, 0x12, 0x13,
		0x15, 0x16, 0x1e, 0x25
	};
	/* DS 0x80011768: count 0x0b, then 11 poly ids (Texture_Move_GT3) */
	UCHAR kubi3[] = {
		0x0b, 0x0b, 0x0e, 0x0f, 0x10, 0x17, 0x18, 0x24,
		0x36, 0x37, 0x3d, 0x3e
	};
	/* DS 0x80011774: count 0x04, then 4 poly ids (Texture_Move_GT4) */
	UCHAR kubi4[] = {
		0x04, 0x03, 0x05, 0x0f, 0x23
	};

	if ((pEm->Type & 0x100) != 0) {
		pParts = pEm->pSin_parts_ptr;
		pEm->Type &= 0xfeff;
		pTmdSave = pParts[0x10].pTmd;
		pTmd2Save = pParts[0x10].pTmd2;
		pPacketSave = pParts[0x10].pPacket;
		pPacket2Save = pParts[0x10].pPacket2;
		pParts[0x10].Be_flg = 0;
		pParts[1].pTmd = pTmdSave;
		pParts[1].pTmd2 = pTmd2Save;
		pParts[1].pPacket = pPacketSave;
		pParts[1].pPacket2 = pPacket2Save;
		pEm->pSin_parts_ptr[2].Be_flg = 0;
		pEm->pSin_parts_ptr[3].Be_flg = 0;
		pEm->pSin_parts_ptr[4].Be_flg = 0;
		pEm->pSin_parts_ptr[5].Be_flg = 0;
		pEm->pSin_parts_ptr[6].Be_flg = 0;
		pEm->pSin_parts_ptr[7].Be_flg = 0;
		pMw = &pEm->pSin_parts_ptr[8].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, kuchi3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, kuchi4);
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xad0034, pEm->Clut + 0x1e2, kubi3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xad0034, pEm->Clut + 0x1e2, kubi4);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	}
}

/* ROM poly lists for Spl_chenge05..07 (Texture_Move_GT3/GT4 pPoly_no: byte 0 = count, then count poly ids) */

/* DS 0x8005f044 (Spl_chenge05) */
static VOID Spl_chenge05(register ENEMY_WORK *pEm)
{
	ULONG *pMw;
	/* DS 0x8001177c (26 bytes) */
	UCHAR kuchi3[] = {
		0x19, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x12, 0x13, 0x16, 0x21, 0x22, 0x23,
		0x25, 0x28, 0x29, 0x2c, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
	};
	/* DS 0x80011798 (6 bytes) */
	UCHAR kuchi4[] = {
		0x05, 0x12, 0x13, 0x14, 0x15, 0x2d
	};
	/* DS 0x800117a0 (4 bytes) */
	UCHAR kubi3[] = {
		0x03, 0x0d, 0x0e, 0x17
	};
	/* DS 0x800117a4 (5 bytes) */
	UCHAR kubi4[] = {
		0x04, 0x08, 0x19, 0x24, 0x32
	};
	/* DS 0x800117ac (9 bytes) */
	UCHAR me3[] = {
		0x08, 0x11, 0x14, 0x1c, 0x27, 0x2a, 0x31, 0x32, 0x33
	};
	/* DS 0x800117b8 (14 bytes) */
	UCHAR me4[] = {
		0x0d, 0x05, 0x0c, 0x0d, 0x0e, 0x18, 0x28, 0x29, 0x2f, 0x30, 0x33, 0x34, 0x36, 0x37
	};

	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		pMw = &pEm->pSin_parts_ptr[8].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x850000, pEm->Clut + 0x1e2, kuchi3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x850000, pEm->Clut + 0x1e2, kuchi4);
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x50, pEm->Clut + 0x1e2, kubi3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x50, pEm->Clut + 0x1e2, kubi4);
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x380044, pEm->Clut + 0x1e2, me3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x380044, pEm->Clut + 0x1e2, me4);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	}
}

/* DS 0x8005f354 (Spl_chenge06) */
static VOID Spl_chenge06(register ENEMY_WORK *pEm)
{
	ULONG *pMw;
	/* DS 0x800117c8 (32 bytes) */
	UCHAR body3[] = {
		0x1f, 0x00, 0x04, 0x08, 0x09, 0x0d, 0x0f, 0x10, 0x15, 0x17, 0x19, 0x1a, 0x1b, 0x24, 0x28, 0x2d,
		0x2e, 0x2f, 0x30, 0x33, 0x34, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43
	};
	/* DS 0x800117e8 (14 bytes) */
	UCHAR body4[] = {
		0x0d, 0x00, 0x02, 0x05, 0x0a, 0x0b, 0x0c, 0x0d, 0x11, 0x1e, 0x1f, 0x20, 0x21, 0x22
	};
	/* DS 0x800117f8 (12 bytes) */
	UCHAR kao3[] = {
		0x0b, 0x06, 0x08, 0x0b, 0x32, 0x3e, 0x3f, 0x40, 0x41, 0x44, 0x45, 0x46
	};
	/* DS 0x80011804 (5 bytes) */
	UCHAR kao4[] = {
		0x04, 0x1f, 0x20, 0x22, 0x25
	};
	/* DS 0x8001180c (10 bytes) */
	UCHAR atama4[] = {
		0x09, 0x02, 0x03, 0x08, 0x09, 0x0c, 0x12, 0x13, 0x14, 0x1c
	};

	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		pMw = &pEm->pSin_parts_ptr->Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body4);
		pMw = &pEm->pSin_parts_ptr[8].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xac0046, pEm->Clut + 0x1e2, kao3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xac0046, pEm->Clut + 0x1e2, kao4);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xffda0004, pEm->Clut + 0x1e2, atama4);
	}
}

/* DS 0x8005f634 (Spl_chenge07) */
static VOID Spl_chenge07(register ENEMY_WORK *pEm)
{
	ULONG *pMw;
	/* DS 0x80011818 (67 bytes) */
	UCHAR body3[] = {
		0x42, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
		0x0f, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
		0x31, 0x32, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
		0x42, 0x43, 0x44
	};
	/* DS 0x8001185c (36 bytes) */
	UCHAR body4[] = {
		0x23, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
		0x0f, 0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
		0x20, 0x21, 0x22, 0x23
	};
	/* DS 0x80011880 (32 bytes) */
	UCHAR kao3[] = {
		0x1f, 0x06, 0x0d, 0x0e, 0x10, 0x11, 0x12, 0x20, 0x21, 0x22, 0x23, 0x28, 0x2b, 0x2c, 0x2d, 0x2e,
		0x2f, 0x30, 0x31, 0x32, 0x3e, 0x3f, 0x44, 0x45, 0x46, 0x47, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
	};
	/* DS 0x800118a0 (9 bytes) */
	UCHAR kao4[] = {
		0x08, 0x0a, 0x0e, 0x0f, 0x10, 0x22, 0x23, 0x24, 0x25
	};
	/* DS 0x800118ac (12 bytes) */
	UCHAR sub3[] = {
		0x0b, 0x0a, 0x0c, 0x1c, 0x1e, 0x29, 0x2a, 0x2b, 0x31, 0x32, 0x35, 0x36
	};
	/* DS 0x800118b8: count 0x06, then 6 poly ids (Texture_Move_GT4) */
	UCHAR sub4[] = {
		0x06, 0x08, 0x13, 0x14, 0x18, 0x1b, 0x1c
	};
	/* DS 0x800118c0 (6 bytes) */
	UCHAR mune3[] = {
		0x05, 0x00, 0x01, 0x02, 0x03, 0x04
	};
	/* DS 0x800118c8 (6 bytes) */
	UCHAR mune4[] = {
		0x05, 0x00, 0x04, 0x06, 0x08, 0x09
	};

	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		/*
		 * Each model pointer is set in two steps: the parts base goes into pMw,
		 * then pMw is moved to the part's Attribute word. DS loads
		 * pSin_parts_ptr straight into the saved register that holds pMw and adds
		 * the part offset in place (lw s0,0x198(s1); addiu s0,s0,N), which a
		 * single &pEm->pSin_parts_ptr[n].Attribute expression does not give.
		 */
		pMw = (ULONG *)pEm->pSin_parts_ptr;
		pMw = &((PARTS_W *)pMw)->Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body4);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[1].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		pMw = (ULONG *)pEm->pSin_parts_ptr;
		pMw = &((PARTS_W *)pMw)[2].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, mune3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, mune4);
		pMw = (ULONG *)pEm->pSin_parts_ptr;
		pMw = &((PARTS_W *)pMw)[5].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, mune3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, mune4);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		pMw = (ULONG *)pEm->pSin_parts_ptr;
		pMw = &((PARTS_W *)pMw)[8].Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xac0046, pEm->Clut + 0x1e2, kao3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xac0046, pEm->Clut + 0x1e2, kao4);
		pMw = (ULONG *)pEm->pSin_parts_ptr;
		pMw = &((PARTS_W *)pMw)->Attribute;
		Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, sub3);
		Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, sub4);
	}
}

/* DS 0x8005fbb4 (Spl_chenge08) */
static VOID Spl_chenge08(register ENEMY_WORK *pEm)
{
	ULONG *pMw;
	/* DS 0x80011738: count 0x20, then 32 poly ids (Texture_Move_GT3) */
	UCHAR kuchi3[] = {
		0x20, 0x01, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x15,
		0x16, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
		0x21, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x29,
		0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
		0x43
	};
	/* DS 0x8001175c: count 0x0b, then 11 poly ids (Texture_Move_GT4) */
	UCHAR kuchi4[] = {
		0x0b, 0x08, 0x0a, 0x0e, 0x10, 0x11, 0x12, 0x13,
		0x15, 0x16, 0x1e, 0x25
	};
	/* DS 0x800118d0: count 0x0d, then 13 poly ids (Texture_Move_GT3) */
	UCHAR kubi3[] = {
		0x0d, 0x06, 0x0b, 0x0e, 0x0f, 0x10, 0x17, 0x18,
		0x24, 0x36, 0x37, 0x3d, 0x3e, 0x42
	};
	/* DS 0x80011774: count 0x04, then 4 poly ids (Texture_Move_GT4) */
	UCHAR kubi4[] = { 0x04, 0x03, 0x05, 0x0f, 0x23 };

	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	pMw = &pEm->pSin_parts_ptr[8].Attribute;
	Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, kuchi3);
	Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, kuchi4);
	Texture_Move_GT3((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x2f0000, pEm->Clut + 0x1e2, kubi3);
	Texture_Move_GT4((MODEL_WORK *)pMw, pEm->Tpage + 2, 0x2f0000, pEm->Clut + 0x1e2, kubi4);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
	Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
}

/* DS 0x8005fe6c (Spl_chenge09) */
static VOID Spl_chenge09(register ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	/* DS 0x800118e0: count 0x12, then 18 poly ids (Texture_Move_GT3) */
	UCHAR body3[] = {
		0x12, 0x01, 0x02, 0x03, 0x06, 0x08, 0x09, 0x0f,
		0x10, 0x14, 0x16, 0x17, 0x18, 0x1a, 0x31, 0x42,
		0x4f, 0x51, 0x54
	};
	/* DS 0x800118f4: count 0x06, then 6 poly ids (Texture_Move_GT4) */
	UCHAR body4[] = {
		0x06, 0x09, 0x0e, 0x12, 0x1a, 0x45, 0x4b
	};
	/* DS 0x800118fc: count 0x18, then 24 poly ids (Texture_Move_GT3) */
	UCHAR kao3[] = {
		0x18, 0x00, 0x04, 0x06, 0x07, 0x09, 0x0a, 0x0e,
		0x0f, 0x10, 0x11, 0x16, 0x17, 0x20, 0x21, 0x22,
		0x23, 0x26, 0x27, 0x28, 0x2c, 0x30, 0x35, 0x3a,
		0x3c
	};
	/* DS 0x80011918: count 0x09, then 9 poly ids (Texture_Move_GT4) */
	UCHAR kao4[] = {
		0x09, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0e, 0x17,
		0x2a, 0x2b
	};

	if ((pEm->Type & 0x100) != 0) {
		pEm->Type &= 0xfeff;
		pP_ptr = pEm->pSin_parts_ptr;
		Texture_Move_GT3((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body3);
		Texture_Move_GT4((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2, body4);
		pP_ptr = &pEm->pSin_parts_ptr[8];
		Texture_Move_GT3((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x34004e, pEm->Clut + 0x1e2, kao3);
		Texture_Move_GT4((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x34004e, pEm->Clut + 0x1e2, kao4);
	}
}

/* DS 0x800600b8 (Spl_chenge10) */
static VOID Spl_chenge10(register ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	/* DS 0x80011924: count 0x21, then 33 poly ids (Texture_Move_GT3) */
	UCHAR body3[] = {
		0x21, 0x01, 0x02, 0x03, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0d, 0x0f, 0x10, 0x12, 0x14, 0x15,
		0x16, 0x17, 0x18, 0x1a, 0x21, 0x22, 0x2d, 0x2e,
		0x2f, 0x31, 0x42, 0x43, 0x4f, 0x51, 0x61, 0x62,
		0x6e, 0x70
	};
	/* DS 0x80011948: count 0x10, then 16 poly ids (Texture_Move_GT4) */
	UCHAR body4[] = {
		0x10, 0x07, 0x08, 0x09, 0x0a, 0x0e, 0x10, 0x12,
		0x13, 0x18, 0x1a, 0x1e, 0x1f, 0x23, 0x45, 0x48,
		0x4b
	};
	/* DS 0x800118fc: count 0x18, then 24 poly ids (Texture_Move_GT3) */
	UCHAR kao3[] = {
		0x18, 0x00, 0x04, 0x06, 0x07, 0x09, 0x0a, 0x0e,
		0x0f, 0x10, 0x11, 0x16, 0x17, 0x20, 0x21, 0x22,
		0x23, 0x26, 0x27, 0x28, 0x2c, 0x30, 0x35, 0x3a,
		0x3c
	};
	/* DS 0x80011918: count 0x09, then 9 poly ids (Texture_Move_GT4) */
	UCHAR kao4[] = {
		0x09, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0e, 0x17,
		0x2a, 0x2b
	};

	if ((pEm->Type & 0x200) != 0) {
		pEm->Type &= 0xfdff;
		pP_ptr = pEm->pSin_parts_ptr;
		Texture_Move_GT3((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x800000, pEm->Clut + 0x1e2, body3);
		Texture_Move_GT4((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x800000, pEm->Clut + 0x1e2, body4);
		pP_ptr = &pEm->pSin_parts_ptr[8];
		Texture_Move_GT3((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x34004e, pEm->Clut + 0x1e2, kao3);
		Texture_Move_GT4((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0x34004e, pEm->Clut + 0x1e2, kao4);
	}
}


/* DS 0x800c3bb8 (EM40_Prim) */
DR_MOVE EM40_Prim;
