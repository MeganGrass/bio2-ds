/*
 * SPL_SCE.C - Scenario dispatch and per-routine enemy handlers (Spl_sce*)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_SCE.C
 */
#include "TYPES.H"
#include "CD.H"
#include "MAIN.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "SPL_SCE.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "JOINT.H"
#include "OMA.H"
#include "ROOT_CK.H"
#include "SCA.H"
#include "SND.H"
#include "SPL.H"
#include "SUB.H"
#include "ESP_R.H"
#include "SPL_AUTO.H"
#include "SPL_TBL.H"

/* file-local prototypes */
static VOID Spl_sce00(register ENEMY_WORK *pEm);
static VOID Spl_sce01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sce02(register ENEMY_WORK *pEm);
static VOID Spl_sce03(register ENEMY_WORK *pEm);
static VOID Spl_sce04(register ENEMY_WORK *pEm);
static VOID Spl_sce05(register ENEMY_WORK *pEm);
static VOID Spl_sce06(register ENEMY_WORK *pEm);
static VOID Spl_sce09(register ENEMY_WORK *pEm);
static VOID Spl_sce0a(register ENEMY_WORK *pEm);
static VOID Spl_sce0c(register ENEMY_WORK *pEm);
static VOID Spl_sce0d(register ENEMY_WORK *pEm);
static VOID Spl_sce0e(register ENEMY_WORK *pEm);
static VOID Spl_sce12(register ENEMY_WORK *pEm);
static VOID Spl_sce20(register ENEMY_WORK *pEm);
static VOID Spl_sce07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sce08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sce0b(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sce_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static UCHAR Spl_sce_mv01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static UCHAR Spl_sce_mv02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static UCHAR Spl_sce_mv03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sce_mv04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* Handler table indexed by pEm->Routine_1 (33 entries).
 * Entries 0x0f, 0x10, 0x11 and 0x13..0x1f are 0 (no handler).
 * Spl_Scenario calls the entry for the current routine number. */
static VOID *Spl_sce[33] = {
	Spl_sce00,
	Spl_sce01,
	Spl_sce02,
	Spl_sce03,
	Spl_sce04,
	Spl_sce05,
	Spl_sce06,
	Spl_sce07,
	Spl_sce08,
	Spl_sce09,
	Spl_sce0a,
	Spl_sce0b,
	Spl_sce0c,
	Spl_sce0d,
	Spl_sce0e,
	0,
	0,
	0,
	Spl_sce12,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Spl_sce20,
};

/* Scenario step for a sub-player (Spl) enemy. For Ids below 0x10, Type gets 0x5000 when bit 0x4000 is clear,
 * and a root check toward the scenario point runs when bit 0x1000 is clear. Ids 0x10 and up use their own motion
 * data pointers. Spl_sce[Routine_1] then runs the scenario handler. Foot sounds are played from Sce_flg
 * bits 1 and 2 (left and right). */
/* DS stack frame: 0x30 bytes; s0..s2 saved at sp+0x20..0x28, ra saved at sp+0x2c. Locals: Tv0: sp+0x10, 16 bytes. */
/* DS 0x80065c88 (Spl_Scenario) */
VOID Spl_Scenario(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	VECTOR Tv0;

	if (pEm->Id < 0x10) {
		if ((pEm->Type & 0x4000) == 0) {
			pEm->Type |= 0x5000;
		}
		if ((pEm->Type & 0x1000) == 0) {
			Tv0.vx = (LONG)pEm->Sce_free2;
			Tv0.vz = (LONG)pEm->Sce_free3;
			Tv0.vy = 0;
			Root_ck(pEm, &Tv0, 0, 0);
		}
	}
	if ((UINT)(pEm->Id - 0x10) < 0x10) {
		Kan = (ULONG)pEm->pKan_t_ptr;
		Seq = (ULONG)pEm->pSeq_t_ptr;
	}
	/* The handler is skipped only when Sce_flg bits 0x10 and 0x20 are both set. DS tests the two bits one after
	 * the other (andi 0x10 / beq, andi 0x20 / bne); a single "||" condition is folded into one (Sce_flg & 0x30)
	 * compare, so the test is written as two nested ifs. */
	if ((pEm->Sce_flg & 0x10) != 0) {
		if ((pEm->Sce_flg & 0x20) != 0) {
			goto Skip;
		}
	}
	((CODE_PTR)(Spl_sce[pEm->Routine_1]))(pEm, Kan, Seq);
Skip:
	pEm->Sce_flg ^= 0x20;
	if ((pEm->Sce_flg & 1) != 0) {
		Foot_set_pl(pEm, 0, 0);
	}
	if ((pEm->Sce_flg & 2) != 0) {
		Foot_set_pl(pEm, 0, 1);
	}
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80065dd0 (Spl_sce00) */
static VOID Spl_sce00(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80065df8 (Spl_sce01) */
static VOID Spl_sce01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	Spl_sce_mv00(pEm, Kan, Seq);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80065e18 (Spl_sce02) */
static VOID Spl_sce02(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80065e40 (Spl_sce03) */
static VOID Spl_sce03(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

/* DS stack frame: 0x20 bytes; s0 saved at sp+0x18, ra saved at sp+0x1c. Locals: Pl_walk_mot[3] (beta name):
 * sp+0x10, 6 bytes. */
/* DS 0x80065e68 (Spl_sce04) */
static VOID Spl_sce04(register ENEMY_WORK *pEm)
{
	/* DS 0x80011a44: 6-byte initialiser template in .rdata (shared by Spl_sce04/05/09/0e/12), block-copied into the local; only element 0 is read */
	SHORT Walk_mot[3] = { 0, 3, 6 };
	register ULONG Idx;

	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) {
			if (pEm->Routine_2 != 2) {
				return;
			}
			Oma_Spl_updown_ck(pEm);
			if (!Spl_sce_mv02(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr)) {
				return;
			}
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
			if ((pEm->Sce_flg & 4) == 0) {
				return;
			}
			pEm->Routine_1 = 4;
			pEm->Routine_2 = 2;
			return;
		}
		if (pEm->Routine_2 != 0) {
			return;
		}
		Idx = (UINT)pEm->Id;
		if (Idx < 0x10) {
			Idx += 0x10;
		} else {
			Idx -= 0x40;
		}
		pEm->Spd_x = (SHORT)Subpl_walk[Idx][0];
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
		SPL_SET_MOVE(pEm, (SHORT)Walk_mot[0] + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
	}
}

/* DS stack frame: 0x28 bytes; s0 saved at sp+0x20, ra saved at sp+0x24. Locals: Pl_walk_mot[3] (beta name):
 * sp+0x10, 6 bytes; Pl_run_mot[3] (beta name): sp+0x18, 3 bytes. */
/* DS 0x80065fb8 (Spl_sce05) */
static VOID Spl_sce05(register ENEMY_WORK *pEm)
{
	/* Retail copies these two tables from rodata (0x80011a44, 0x80011a4c) into the frame. Only element 0 is read,
	 * and the other elements are copied but never read, as in retail. The stored Routine_2 value 2 equals
	 * Run_mot[0] + 1. */
	SHORT Walk_mot[3] = { 0, 3, 6 };
	UCHAR Run_mot[3] = { 1, 4, 7 };
	register ULONG Idx;

	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) {
			if (pEm->Routine_2 != 2) {
				return;
			}
			Oma_Spl_updown_ck(pEm);
			if (!Spl_sce_mv02(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr)) {
				return;
			}
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
			if ((pEm->Sce_flg & 4) == 0) {
				return;
			}
			pEm->Routine_1 = 5;
			pEm->Routine_2 = (UCHAR)(Run_mot[0] + 1);
			return;
		}
		if (pEm->Routine_2 != 0) {
			return;
		}
		Idx = (UINT)pEm->Id;
		if (Idx < 0x10) {
			Idx += 0x10;
		} else {
			Idx -= 0x40;
		}
		pEm->Spd_x = (SHORT)Subpl_run[Idx][0];
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
		SPL_SET_MOVE(pEm, (SHORT)Walk_mot[0] + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
		SPL_SET_MOVE(pEm, (UINT)Run_mot[0] + 0x70000);
	}
}

/* DS stack frame: 0x20 bytes; ra saved at sp+0x18. Locals: Pl_taiki_mot[3]: sp+0x10, 6 bytes. */
/* DS 0x80066138 (Spl_sce06) */
static VOID Spl_sce06(register ENEMY_WORK *pEm)
{
	USHORT Pl_taiki_mot[3] = { 2, 5, 8 };

	switch (pEm->Routine_2) {
	case 0:
		*(int *)&pEm->Move_no = (UINT)Pl_taiki_mot[0] + 0x70000;
		pEm->Routine_2 = 1;
		/* falls through: the step that sets Routine_2 to 1 also runs the first Joint_move */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
	}
}

/* DS stack frame: 0x18 bytes; s0 saved at sp+0x10, ra saved at sp+0x14. No local variable slots are used. */
/* DS 0x800661c0 (Spl_sce07) */
static VOID Spl_sce07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	register ULONG Idx;

	switch (pEm->Routine_2) {
	case 0:
		Idx = (UINT)pEm->Id;
		Idx = (Idx < 0x10) ? Idx + 0x10 : Idx - 0x40;
		pEm->Spd_x = -(SHORT)Subpl_back[Idx][0];
		*(UINT *)&pEm->Move_no = 0x70000;		/* Move_no 0, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (Spl_sce_mv02(pEm, Kan, Seq)) {
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
			if ((pEm->Sce_flg & 4) != 0) {
				pEm->Routine_1 = 7;
				pEm->Routine_2 = 2;
			}
		}
	}
}

/* DS stack frame: 0x18 bytes; s0 saved at sp+0x10, ra saved at sp+0x14. No local variable slots are used. */
/* DS 0x8006629c (Spl_sce08) */
static VOID Spl_sce08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	register ULONG Idx;

	if (pEm->Routine_2 == 0) {
		Idx = (UINT)pEm->Id;
		if (Idx < 0x10) {
			Idx += 0x10;
		} else {
			Idx -= 0x40;
		}
		*(UINT *)&pEm->Spd_x = -(UINT)Subpl_ato[Idx][0];
		*(UINT *)&pEm->Move_no = 0x70001;		/* Move_no 1, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
	} else if (pEm->Routine_2 != 1) {
		return;
	}
	if (Spl_sce_mv02(pEm, Kan, Seq)) {
		Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
		pEm->Routine_1 = 6;
		pEm->Routine_2 = 0;
		if ((pEm->Sce_flg & 4) != 0) {
			pEm->Routine_1 = 8;
			pEm->Routine_2 = 2;
		}
	}
}

/* DS stack frame: 0x20 bytes; s0 saved at sp+0x18, ra saved at sp+0x1c. Locals: Pl_walk_mot[3]: sp+0x10, 6 bytes. */
/* DS 0x8006637c (Spl_sce09) */
static VOID Spl_sce09(register ENEMY_WORK *pEm)
{
	SHORT Pl_walk_mot[3] = { 0, 3, 6 };

	switch (pEm->Routine_2) {
	case 0:
		*(int *)&pEm->Move_no = (SHORT)Pl_walk_mot[0] + 0x70000;
		pEm->Routine_2 = 1;
		/* fall through */
	case 1:
		if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
			if ((pEm->Sce_flg & 4) != 0) {
				pEm->Routine_1 = 9;
				pEm->Routine_2 = 2;
			}
		}
		break;
	}
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80066450 (Spl_sce0a) */
static VOID Spl_sce0a(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80066478 (Spl_sce0b) */
static VOID Spl_sce0b(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	Spl_sce_mv04(pEm, Kan, Seq);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x80066498 (Spl_sce0c) */
static VOID Spl_sce0c(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr);
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x800664c0 (Spl_sce0d) */
static VOID Spl_sce0d(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

/* DS stack frame: 0x20 bytes; s0 saved at sp+0x18, ra saved at sp+0x1c. Frame slots: sp+0x10 (halfword read; word
 * (unaligned) written); sp+0x13 (word (unaligned) written); sp+0x14 (halfword written). */
/* DS 0x80066508 (Spl_sce0e) */
static VOID Spl_sce0e(register ENEMY_WORK *pEm)
{
	/* DS 0x80011a44: 6-byte initialiser template in .rdata (shared by Spl_sce04/05/09/0e/12), block-copied into the local; only element 0 is read */
	SHORT Walk_mot[3] = { 0, 3, 6 };
	register ULONG Idx;

	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) {
			if (pEm->Routine_2 != 2) {
				return;
			}
			if (((pEm->Sca_info & 0x100000) != 0) && (Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs) != 0)) {
				return;
			}
			if (!Spl_sce_mv02(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr)) {
				return;
			}
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
			if ((pEm->Sce_flg & 4) == 0) {
				return;
			}
			pEm->Routine_1 = 0x12;
			pEm->Routine_2 = 2;
			return;
		}
		if (pEm->Routine_2 != 0) {
			return;
		}
		Idx = (UINT)pEm->Id;
		if (Idx < 0x10) {
			Idx += 0x10;
		} else {
			Idx -= 0x40;
		}
		pEm->Spd_x = (SHORT)Subpl_walk[Idx][0];
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
		SPL_SET_MOVE(pEm, (SHORT)Walk_mot[0] + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
	}
}

/* DS stack frame: 0x20 bytes; s0 saved at sp+0x18, ra saved at sp+0x1c. Locals: Pl_walk_mot[3] (beta name):
 * sp+0x10, 6 bytes. */
/* DS 0x8006668c (Spl_sce12) */
static VOID Spl_sce12(register ENEMY_WORK *pEm)
{
	SHORT Walk_mot[3] = { 0, 3, 6 };

	if (pEm->Routine_2 != 0) {
		if (pEm->Routine_2 != 1) return;
	} else {
		SPL_SET_MOVE(pEm, (SHORT)Walk_mot[0] + 0x70000);
		pEm->Routine_2 = 1;
	}
	if (Spl_sce_mv03(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
		pEm->Routine_1 = 6;
		pEm->Routine_2 = 0;
		if ((pEm->Sce_flg & 4) != 0) {
			pEm->Routine_1 = 0x15;
			pEm->Routine_2 = 2;
		}
	}
}

/* DS stack frame: 0x40 bytes; s0..s2 saved at sp+0x30..0x38, ra saved at sp+0x3c. Locals: Sv: sp+0x10, 8 bytes;
 * Em10_pldie00_mot[2]: sp+0x18, 2 bytes. */
/* DS 0x80066778 (Spl_sce20) */
static VOID Spl_sce20(register ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv;
	UCHAR Em10_pldie00_mot[2];
	LONG Rnd_a;
	LONG Rnd_b;

	Em10_pldie00_mot[0] = 0xf;
	Em10_pldie00_mot[1] = 0x11;
	if (pEm->Routine_2 != 1) {
		if ((signed char)pEm->Routine_2 > 1) return;
		if (pEm->Routine_2 != 0) return;
		*(UINT *)&pEm->Move_no = 0xF000D;		/* Move_no 13, Move_cnt 0, Hokan_flg 15, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x800;
		pEm->Timer2 = 6;
		pEm->Timer3 = 6;
		pEm->Neck_flg |= 1;
	}
	if ((signed char)pEm->Timer2 != 0) {
		pEm->Timer2--;
		pEm->Pos_y += 0x32;
		if ((signed char)pEm->Timer2 != 0) goto after_timer3;
	}
	if ((signed char)pEm->Timer3 != 0) {
		pEm->Timer3--;
		pEm->Pos_y -= 0x32;
	}
after_timer3:
	pEm->Cdir_y += pEm->Timer0;
	Null_pos_set(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0);
	pEm->Cdir_y = pEm->Cdir_y - pEm->Timer0;
	pEm->Routine_2 += (CHAR)Joint_move(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr, 0x100);
	if (((UINT)(pEm->Move_cnt - 0x29) < 0x3a) && ((pEm->Move_cnt & 1) == 0)) {
		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Rnd_a = ((LONG (*)(VOID))Rnd)();
		Rnd_b = ((LONG (*)(VOID))Rnd)();
		Esp_call(Rnd_a * 8 + 0x400, Rnd_b << 4, &pP_ptr[8].Workm, &Sv);
	}
	if ((UINT)(pEm->Move_cnt - 100) < 5) {
		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Rnd_a = ((LONG (*)(VOID))Rnd)();
		Esp_call(0x1000, Rnd_a << 4, &pP_ptr[8].Workm, &Sv);
		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Rnd_a = ((LONG (*)(VOID))Rnd)();
		Esp_call(0x1400, Rnd_a << 4, &pP_ptr[8].Workm, &Sv);
		pP_ptr = pEm->pSin_parts_ptr;
		*(ULONG *)&pP_ptr[8].Free[0] = 0x40;
		*(ULONG *)&pP_ptr[8].Free[2] = 0x00202030;
		pP_ptr[8].Be_flg |= 0x80;
		pP_ptr = pEm->pSin_parts_ptr;
		*(ULONG *)&pP_ptr->Free[0] = 0x80;
		*(ULONG *)&pP_ptr->Free[2] = 0x00101030;
		pP_ptr->Be_flg |= 0x80;
	}
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x800669b0 (Spl_sce_mv00) */
static VOID Spl_sce_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) return;
		if (pEm->Routine_2 != 0) return;
		pEm->Routine_2 = 1;
		pEm->Hokan_flg = 7;
		if ((pEm->Sce_flg & 0x40) != 0) {
			pEm->Hokan_flg = 0;
		}
	}
	if (Joint_move(pEm, Kan, Seq, ((pEm->Sce_flg & 0x80) != 0 ? 0x10000 : 0) | 0x200) != 0) {
		pEm->Routine_2 = 2;
		if ((pEm->Sce_flg & 4) != 0) {
			pEm->Routine_2 = 1;
		}
		pEm->Cdir_y += pEm->Sce_free1;
	} else {
		if (((pEm->Sce_flg & 8) != 0) && (Joint_move(pEm, Kan, Seq, ((pEm->Sce_flg & 0x80) != 0 ? 0x10000 : 0) | 0x200) != 0)) {
			pEm->Routine_2 = 2;
			if ((pEm->Sce_flg & 4) != 0) {
				pEm->Routine_2 = 1;
			}
			pEm->Cdir_y += pEm->Sce_free1;
		}
		pEm->Cdir_y += pEm->Sce_free0;
	}
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x80066afc (Spl_sce_mv01) */
static UCHAR Spl_sce_mv01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	/* Footstep side for Snd_se_walk: 4 for one foot, 7 for the other (bit 0x2000 of the current sequence word).
	   DS builds it as sll/addu/addiu from the srl result; held in a USHORT local the sum keeps that shape. */
	USHORT lr;

	if (Goto00_ck(pEm, (INT)pEm->Dest_x, (INT)pEm->Dest_z, 0x15e) != 0) {
		Goto00(pEm, (INT)pEm->Dest_x, (INT)pEm->Dest_z, pEm->Sce_free0);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
			Snd_se_walk(0, lr, pEm);
			G.Room_player_flg |= 2;
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		return 0;
	}
	return 1;
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x80066bcc (Spl_sce_mv02) */
static UCHAR Spl_sce_mv02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	LONG Dx;
	LONG Dz;
	/* Footstep side for Snd_se_walk: 4 for one foot, 7 for the other (bit 0x2000 of the current sequence word).
	   DS builds it as sll/addu/addiu from the srl result; held in a USHORT local the sum keeps that shape
	   (the same form as Spl_sce_mv01). */
	USHORT lr;

	/* Turn rate: Sce_free0, negated when Spd_x is negative. DS computes 1 - (Spd_x as unsigned >> 15) * 2
	   (lhu/srl/sll/subu from 1) and passes the mflo product to Goto00 without narrowing it; Goto00 itself
	   narrows its position and rate arguments (sll/sra on a1, a2 and a3 at 0x80015574). The call goes
	   through a LONG-parameter type so the caller passes the full product as DS does. */
	((VOID (*)(ENEMY_WORK *, LONG, LONG, LONG))Goto00)(pEm, (LONG)pEm->Dest_x, (LONG)pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 0xf) * 2) * pEm->Sce_free0);
	Add_speedXZ(pEm, 0);
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		if (pEm->Routine_1 == 5) {
			lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
			Snd_se_walk(1, lr, pEm);
			G.Room_player_flg |= 4;
		} else {
			lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
			Snd_se_walk(0, lr, pEm);
			G.Room_player_flg |= 2;
		}
	}
	Joint_move(pEm, Kan, Seq, 0x200);
	Dx = pEm->Pos_x - (INT)pEm->Dest_x;
	Dz = pEm->Pos_z - (INT)pEm->Dest_z;
	return (SquareRoot0(Dx * Dx + Dz * Dz) < 100) ? 1 : 0;
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x80066d24 (Spl_sce_mv03) */
static UCHAR Spl_sce_mv03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG Lr;

	if (Goto01_ck(pEm, pEm->Dest_x, 0x80) != 0) {
		Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			/* Footstep side (bit 0x2000 of the sequence word) times three plus four. DS builds this with
			   sll/addu/addiu. Assigning Lr twice keeps CCPSX from proving the operands share no bits;
			   the one-expression form is compiled to sll/or/ori instead. */
			Lr = (*pEm->pNow_seq & 0x2000) >> 0xd;
			Lr = Lr * 3 + 4;
			Snd_se_walk(0, Lr, pEm);
			G.Room_player_flg |= 2;
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		return 0;
	}
	return 1;
}

/* DS stack frame: 0x20 bytes; s0..s2 saved at sp+0x10..0x18, ra saved at sp+0x1c. No local variable slots are
 * used. */
/* DS 0x80066dec (Spl_sce_mv04) */
static VOID Spl_sce_mv04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) return;
		if (pEm->Routine_2 != 0) return;
		pEm->Routine_2 = 1;
		pEm->Hokan_flg = 7;
		if ((pEm->Sce_flg & 0x40) != 0) {
			pEm->Hokan_flg = 0;
		}
		if ((pEm->Sce_flg & 0x100) != 0) {
			Spd_get(pEm, Kan, Seq, (pEm->Sce_flg & 0x80) != 0 ? 1 : 0);
		}
	}
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
		G.Room_player_flg |= 2;
	}
	Spd_get(pEm, Kan, Seq, (pEm->Sce_flg & 0x80) != 0 ? 1 : 0);
	if (Joint_move(pEm, Kan, Seq, ((pEm->Sce_flg & 0x80) != 0 ? 0x10000 : 0) | 0x200) != 0) {
		pEm->Routine_2 = 2;
		if ((pEm->Sce_flg & 4) != 0) {
			pEm->Routine_2 = 1;
		}
		pEm->Cdir_y += pEm->Sce_free1;
		Add_speedXYZ(pEm, 0, 0);
	} else {
		Add_speedXYZ(pEm, 0, 0);
		if ((pEm->Sce_flg & 8) != 0) {
			if (Joint_move(pEm, Kan, Seq, ((pEm->Sce_flg & 0x80) != 0 ? 0x10000 : 0) | 0x200) != 0) {
				pEm->Routine_2 = 2;
				if ((pEm->Sce_flg & 4) != 0) {
					pEm->Routine_2 = 1;
				}
				pEm->Cdir_y += pEm->Sce_free1;
			}
			Add_speedXYZ(pEm, 0, 0);
		}
		pEm->Cdir_y += pEm->Sce_free0;
	}
}
