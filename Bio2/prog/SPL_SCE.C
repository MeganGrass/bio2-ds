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

static VOID *Spl_sce[] = {
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

/* DS 0x80011a44 (Spl_sce_walk_mot) */
const SHORT Spl_sce_walk_mot = 0x0000;

/* DS 0x80011a4c (Spl_sce_run_mot) */
const UCHAR Spl_sce_run_mot = 0x01;

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
	if (((pEm->Sce_flg & 0x10) == 0) || ((pEm->Sce_flg & 0x20) == 0)) {
		((CODE_PTR)(Spl_sce[pEm->Routine_1]))(pEm, Kan, Seq);
	}
	pEm->Sce_flg ^= 0x20;
	if ((pEm->Sce_flg & 1) != 0) {
		Foot_set_pl(pEm, 0, 0);
	}
	if ((pEm->Sce_flg & 2) != 0) {
		Foot_set_pl(pEm, 0, 1);
	}
}

/* DS 0x80065dd0 (Spl_sce00) */
static VOID Spl_sce00(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr);
}

/* DS 0x80065df8 (Spl_sce01) */
static VOID Spl_sce01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	Spl_sce_mv00(pEm, Kan, Seq);
}

/* DS 0x80065e18 (Spl_sce02) */
static VOID Spl_sce02(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr);
}

/* DS 0x80065e40 (Spl_sce03) */
static VOID Spl_sce03(register ENEMY_WORK *pEm)
{
	Spl_sce_mv00(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

/* DS 0x80065e68 (Spl_sce04) */
static VOID Spl_sce04(register ENEMY_WORK *pEm)
{
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
		SPL_SET_MOVE(pEm, (SHORT)Spl_sce_walk_mot + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
	}
}

/* DS 0x80065fb8 (Spl_sce05) */
static VOID Spl_sce05(register ENEMY_WORK *pEm)
{
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
		pEm->Spd_x = (SHORT)Subpl_run[Idx][0];
		pEm->Spd_z = 0;
		pEm->Routine_2 = 1;
		SPL_SET_MOVE(pEm, (SHORT)Spl_sce_walk_mot + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
		SPL_SET_MOVE(pEm, (UINT)Spl_sce_run_mot + 0x70000);
	}
}

/* DS 0x80066138 (Spl_sce06) */
static VOID Spl_sce06(register ENEMY_WORK *pEm)
{
	USHORT Pl_taiki_mot[3] = { 2, 5, 8 };

	if (pEm->Routine_2 == 0) {
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = (UINT)Pl_taiki_mot[0] + 0x70000;
	} else if (pEm->Routine_2 != 1) {
		return;
	}
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
}

/* DS 0x800661c0 (Spl_sce07) */
static VOID Spl_sce07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	register ULONG Idx;

	if (pEm->Routine_2 == 0) {
		Idx = (UINT)pEm->Id;
		if (Idx < 0x10) {
			Idx += 0x10;
		} else {
			Idx -= 0x40;
		}
		pEm->Spd_x = -(SHORT)Subpl_back[Idx][0];
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
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
			pEm->Routine_1 = 7;
			pEm->Routine_2 = 2;
		}
	}
}

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
		pEm->Move_no = 1;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
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

/* DS 0x8006637c (Spl_sce09) */
static VOID Spl_sce09(register ENEMY_WORK *pEm)
{
	SHORT Pl_walk_mot[3] = { 0, 3, 6 };

	if (pEm->Routine_2 == 0) {
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = (SHORT)Pl_walk_mot[0] + 0x70000;
	} else if (pEm->Routine_2 != 1) {
		return;
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
		pEm->Routine_1 = 6;
		pEm->Routine_2 = 0;
		if ((pEm->Sce_flg & 4) != 0) {
			pEm->Routine_1 = 9;
			pEm->Routine_2 = 2;
		}
	}
}

/* DS 0x80066450 (Spl_sce0a) */
static VOID Spl_sce0a(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr);
}

/* DS 0x80066478 (Spl_sce0b) */
static VOID Spl_sce0b(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	Spl_sce_mv04(pEm, Kan, Seq);
}

/* DS 0x80066498 (Spl_sce0c) */
static VOID Spl_sce0c(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr);
}

/* DS 0x800664c0 (Spl_sce0d) */
static VOID Spl_sce0d(register ENEMY_WORK *pEm)
{
	Spl_sce_mv04(pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

/* DS 0x80066508 (Spl_sce0e) */
static VOID Spl_sce0e(register ENEMY_WORK *pEm)
{
	register ULONG Idx;

	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) {
			if (pEm->Routine_2 != 2) {
				return;
			}
			if (((pEm->Sca_info & 0x100000) != 0) &&
				(Sca_hit_stairs(pEm, 0x1c2, (SCA_DATA *)pStairs) != 0)) {
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
		SPL_SET_MOVE(pEm, (SHORT)Spl_sce_walk_mot + 0x70000);
	}
	if (Spl_sce_mv01(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr) != 0) {
		pEm->Routine_2 = 2;
	}
}

/* DS 0x8006668c (Spl_sce12) */
static VOID Spl_sce12(register ENEMY_WORK *pEm)
{
	SHORT Pl_walk_mot[3] = { 0, 3, 6 };

	if (pEm->Routine_2 == 0) {
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = (SHORT)Pl_walk_mot[0] + 0x70000;
	} else if (pEm->Routine_2 != 1) {
		return;
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

/* DS 0x80066778 (Spl_sce20) */
static VOID Spl_sce20(register ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv;
	UCHAR Em10_pldie00_mot[2];
	UCHAR Rnd_a;
	UCHAR Rnd_b;
	ULONG Pad16[4];

	(void)Pad16;
	Em10_pldie00_mot[0] = 0xf;
	Em10_pldie00_mot[1] = 0x11;
	if (pEm->Routine_2 != 1) {
		if (pEm->Routine_2 > 1) return;
		if (pEm->Routine_2 != 0) return;
		pEm->Move_no = 0xd;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 0xf;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x800;
		pEm->Timer2 = 6;
		pEm->Timer3 = 6;
		pEm->Neck_flg |= 1;
	}
	if (pEm->Timer2 != 0) {
		pEm->Timer2--;
		pEm->Pos_y += 0x32;
		if (pEm->Timer2 != 0) goto after_timer3;
	}
	if (pEm->Timer3 != 0) {
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
		Rnd_a = (UCHAR)Rnd();
		Rnd_b = (UCHAR)Rnd();
		Esp_call((ULONG)Rnd_a * 8 + 0x400, (SHORT)Rnd_b << 4, &pP_ptr[8].Workm, &Sv);
	}
	if ((UINT)(pEm->Move_cnt - 100) < 5) {
		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Rnd_a = (UCHAR)Rnd();
		Esp_call(0x1000, (SHORT)Rnd_a << 4, &pP_ptr[8].Workm, &Sv);
		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Rnd_a = (UCHAR)Rnd();
		Esp_call(0x1400, (SHORT)Rnd_a << 4, &pP_ptr[8].Workm, &Sv);
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr[8].Free[0] = 0x40;
		pP_ptr[8].Free[1] = 0;
		pP_ptr[8].Free[2] = 0x2030;
		pP_ptr[8].Free[3] = 0x20;
		pP_ptr[8].Be_flg |= 0x80;
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Free[0] = 0x80;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[2] = 0x1030;
		pP_ptr->Free[3] = 0x10;
		pP_ptr->Be_flg |= 0x80;
	}
}

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
		if (((pEm->Sce_flg & 8) != 0) &&
			(Joint_move(pEm, Kan, Seq, ((pEm->Sce_flg & 0x80) != 0 ? 0x10000 : 0) | 0x200) != 0)) {
			pEm->Routine_2 = 2;
			if ((pEm->Sce_flg & 4) != 0) {
				pEm->Routine_2 = 1;
			}
			pEm->Cdir_y += pEm->Sce_free1;
		}
		pEm->Cdir_y += pEm->Sce_free0;
	}
}

/* DS 0x80066afc (Spl_sce_mv01) */
static UCHAR Spl_sce_mv01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	if (Goto00_ck(pEm, (INT)pEm->Dest_x, (INT)pEm->Dest_z, 0x15e) != 0) {
		Goto00(pEm, (INT)pEm->Dest_x, (INT)pEm->Dest_z, pEm->Sce_free0);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 2;
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		return 0;
	}
	return 1;
}

/* DS 0x80066bcc (Spl_sce_mv02) */
static UCHAR Spl_sce_mv02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	LONG Dx;
	LONG Dz;

	Goto00(pEm, (INT)pEm->Dest_x, (INT)pEm->Dest_z,
		   ((pEm->Spd_x >> 0xf) * 2 + 1) * pEm->Sce_free0);
	Add_speedXZ(pEm, 0);
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		if (pEm->Routine_1 == 5) {
			Snd_se_walk(1, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		} else {
			Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 2;
		}
	}
	Joint_move(pEm, Kan, Seq, 0x200);
	Dx = pEm->Pos_x - (INT)pEm->Dest_x;
	Dz = pEm->Pos_z - (INT)pEm->Dest_z;
	return (SquareRoot0(Dx * Dx + Dz * Dz) < 100) ? 1 : 0;
}

/* DS 0x80066d24 (Spl_sce_mv03) */
static UCHAR Spl_sce_mv03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	if (Goto01_ck(pEm, pEm->Dest_x, 0x80) != 0) {
		Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 2;
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		return 0;
	}
	return 1;
}

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
