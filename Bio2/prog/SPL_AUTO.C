/*
 * SPL_AUTO.C - Auto move/AI routines for Aida, Sherry and Spl enemies
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_AUTO.C
 */
#include "TYPES.H"
#include "CD.H"
#include "MAIN.H"
#include "PL_WEP2.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <MEMORY.H>
#include "SPL_AUTO.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "JOINT.H"
#include "OMA.H"
#include "PL_WEP.H"
#include "ROOT_CK.H"
#include "SCA.H"
#include "SND.H"
#include "SPL.H"
#include "SUB.H"
#include "ESP_R.H"
#include "flag.h"
#include "DIVCHK.H"
#include "SPL_TBL.H"


/* file-local prototypes */
VOID Spl_Auto(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type09(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_Move_type0a(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_em10_at00_pl(register ENEMY_WORK *pEm, register ULONG Type);
static VOID Aida_em25_at00_pl(register ENEMY_WORK *pEm, register ULONG Type);
static VOID Aida_mv00_br(register ENEMY_WORK *pEm);
static VOID Aida_mv00_br_1(register ENEMY_WORK *pEm);
static VOID Aida_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv01_br(register ENEMY_WORK *pEm);
static VOID Aida_mv01_br_1(register ENEMY_WORK *pEm);
static VOID Aida_mv01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv02_br(register ENEMY_WORK *pEm);
static VOID Aida_mv02_br_1(register ENEMY_WORK *pEm);
static VOID Aida_mv02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv03_br(register ENEMY_WORK *pEm);
static VOID Aida_mv03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv04_br(register ENEMY_WORK *pEm);
static VOID Aida_mv04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_br(register ENEMY_WORK *pEm);
static VOID Aida_mv05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1_t0(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1_t1_br(register ENEMY_WORK *pEm);
static VOID Aida_mv05_w1_t1(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1_t2(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1_t3(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv05_w1_t4(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Aida_mv0b_br(register ENEMY_WORK *pEm);
static VOID Aida_mv0b_br_1(register ENEMY_WORK *pEm);
static VOID Aida_mv0c_br(register ENEMY_WORK *pEm);
static VOID Sherry_move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type09(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_Move_type0a(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_mv00_br(register ENEMY_WORK *pEm);
static VOID Sherry_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Sherry_mv01_br(register ENEMY_WORK *pEm);
static VOID Sherry_mv02_br(register ENEMY_WORK *pEm);
static VOID Sherry_mv0b_br(register ENEMY_WORK *pEm);
VOID Spl_dm00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
VOID Spl_dm01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
VOID Spl_die00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* forward declarations (definitions are in retail address order) */
static UCHAR Spl_pos_ck_timer;
extern SHORT Spl_r_pos[2][6];

/* Route target for the sub-player AI. Sherry (Ids 0x45, 0x4f) uses the second row of Spl_r_pos. The
 * sub-player walks to the Spl_r_pos point for its mode, or to the player when there is no guide
 * point. Root_ck then steps toward the goal, and Dist keeps the distance to it. */
/* DS 0x80060374 (Spl_Root_ck_sys) */
VOID Spl_Root_ck_sys(register ENEMY_WORK *pEm)
{
	USHORT flg;
	UCHAR guide_no;
	INT dz;
	INT sel;
	LONG dist;
	USHORT mode;
	SHORT ll;
	SHORT dir;
	SHORT dir2;
	VECTOR vec;
	SVECTOR svec;

	sel = 0;
	if ((pEm->Id == 0x45) || (pEm->Id == 0x4f)) {
		sel = 1;
	}
	if (((G.Status_flg & STAT_SUB_PLAYER) == 0) || (pEm != G.pEnemy[1])) {
		vec.vx = (LONG)pEm->Sce_free2;
		vec.vz = (LONG)pEm->Sce_free3;
		vec.vy = 0;
		Root_ck(pEm, &vec, 0, 0);
		return;
	}
	flg = (USHORT)SPL_FREE(pEm)->Flg;
	mode = flg & 3;
	if (mode == 1) {
		vec.vx = (LONG)SPL_FREE(pEm)->Goal_x;
		vec.vy = 0;
		vec.vz = (LONG)SPL_FREE(pEm)->Goal_z;
	} else {
		if (mode < 2) {
			if ((flg & 3) != 0) goto guide_ck;
			ll = Spl_r_pos[sel][0];
			dir = Spl_r_pos[sel][1];
			dir2 = Spl_r_pos[sel][2];
		} else {
			if (mode != 2) goto guide_ck;
			ll = Spl_r_pos[sel][3];
			dir = Spl_r_pos[sel][4];
			dir2 = Spl_r_pos[sel][5];
		}
		Spl_soba_pos_set(ll, dir, dir2, &svec);
		vec.vx = (LONG)svec.vx;
		SPL_FREE(pEm)->Goal_x = svec.vx;
		vec.vy = (LONG)svec.vy;
		vec.vz = (LONG)svec.vz;
		SPL_FREE(pEm)->Goal_z = svec.vz;
	}
guide_ck:
	guide_no = Guide_check((SHORT)vec.vx, (SHORT)vec.vz);
	if (guide_no == 0xff) {
		SPL_FREE(pEm)->Goal_x = (SHORT)G.Player.Pos_x;
		SPL_FREE(pEm)->Goal_z = (SHORT)G.Player.Pos_z;
		vec.vx = G.Player.Pos_x;
		vec.vz = G.Player.Pos_z;
	}
	Root_ck(pEm, &vec, 0, 0);
	sel = pEm->Pos_x - (INT)SPL_FREE(pEm)->Goal_x;
	dz = pEm->Pos_z - (INT)SPL_FREE(pEm)->Goal_z;
	pEm->pEnemy_neck = &G.Player.Be_flg;
	dist = SquareRoot0(sel * sel + dz * dz);
	SPL_FREE(pEm)->Dist = (USHORT)dist;
}

/* Per-frame sub-player AI. While At_em_flg bit 0x8000 is set, Blink counts up on its low 7 bits and sets
 * bit 0x80 above 0x3c. Cool counts down toward zero. Sherry (Ids 0x45, 0x4f) runs Sherry_move; everyone
 * else runs Aida_move. */
/* DS 0x800605ac (Spl_Auto) */
VOID Spl_Auto(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG blink;
	CHAR cool;

	if (pEm->At_em_flg & 0x8000) {
		blink = (UCHAR)((SPL_FREE(pEm)->Blink + 1) & 0x7f);
		SPL_FREE(pEm)->Blink = blink;
		if (0x3c < blink) {
			SPL_FREE(pEm)->Blink = blink | 0x80;
		}
	} else {
		SPL_FREE(pEm)->Blink = 0;
	}
	cool = (CHAR)SPL_FREE(pEm)->Cool;
	if (cool != 0) {
		SPL_FREE(pEm)->Cool = (UCHAR)(cool - 1);
	}
	if ((pEm->Id == 0x45) || (pEm->Id == 0x4f)) {
		Sherry_move(pEm, Kan, Seq);
	} else {
		Aida_move(pEm, Kan, Seq);
	}
}

/* Empty in the retail binary (kept as a placeholder entry point). */
/* DS 0x80060648 (Aida) */
VOID Aida(register ENEMY_WORK *pEm)
{
}

/* Aida movement step. Spl_pos_ck_timer counts down; every 16th frame (timer reset to 0xf) the position is
 * re-checked. When the position check changes the low type nibble, the new start state is loaded from
 * Aida_St_pos with the 0x8000 flag set. The move routine for the low type nibble runs every frame. */
/* DS 0x80060650 (Aida_move) */
static VOID Aida_move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	USHORT type;
	USHORT type_new;
	UCHAR next_timer;
	ULONG type_bits;
	USHORT flg;

	Spl_enemy_ck(pEm);
	next_timer = (UCHAR)(Spl_pos_ck_timer - 1);
	if (Spl_pos_ck_timer == 0) {
		type = pEm->Type;
		pEm->Type = type & 0xff00;
		type_bits = Spl_em_pos_ck(pEm, 3000);
		flg = SPL_FREE(pEm)->Flg;
		pEm->Type = pEm->Type | (USHORT)type_bits;
		type_new = pEm->Type;
		SPL_FREE(pEm)->Flg = flg & 0x7fff;
		if ((type & 0xf) != (type_new & 0xf)) {
			type = pEm->Type;
			flg &= 0x7ffc;
			SPL_FREE(pEm)->Flg = flg;
			SPL_FREE(pEm)->Flg = flg | Aida_St_pos[type & 0xf] | 0x8000;
		}
		next_timer = 0xf;
	}
	Spl_pos_ck_timer = next_timer;
	((CODE_PTR)(Aida_Move_type[pEm->Type & 0xf]))(pEm, Kan, Seq);
}

/* DS 0x80060750 (Aida_Move_type00) */
static VOID Aida_Move_type00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type00[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x800607d4 (Aida_Move_type01) */
static VOID Aida_Move_type01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type01[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060858 (Aida_Move_type02) */
static VOID Aida_Move_type02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type02[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x800608dc (Aida_Move_type03) */
static VOID Aida_Move_type03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type03[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060960 (Aida_Move_type04) */
static VOID Aida_Move_type04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type04[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x800609e4 (Aida_Move_type05) */
static VOID Aida_Move_type05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type05[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060a68 (Aida_Move_type06) */
static VOID Aida_Move_type06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type06[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060aec (Aida_Move_type07) */
static VOID Aida_Move_type07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type07[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060b70 (Aida_Move_type08) */
static VOID Aida_Move_type08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type08[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060bf4 (Aida_Move_type09) */
static VOID Aida_Move_type09(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type09[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060c78 (Aida_Move_type0a) */
static VOID Aida_Move_type0a(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv_br_type0a[pEm->Routine_1]))(pEm);
	((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x80060cfc (Aida_em10_at00_pl) */
static VOID Aida_em10_at00_pl(register ENEMY_WORK *pEm, register ULONG Type)
{
	ENEMY_WORK *pYo;

	pYo = pSpl_near_em[0];
	if (SPL_FREE(pEm)->Cool == 0) {
		if ((Type & 0xf) == 10) {
			pYo = (ENEMY_WORK *)Spl_em_cdir_ck(pEm);
			if (pYo == NULL) {
				return;
			}
			if ((pYo->Life < 0) || ((pYo->Type & 0x2000) != 0)) {
				pYo = (ENEMY_WORK *)Spl_pllock_get(pEm, pYo);
			}
		}
		if ((pYo != NULL) && (pYo->Life >= 0)) {
			if ((pYo->Type & 0x2000) == 0) {
				SPL_SET_ROUTINE(pEm, 0x0501);
				pEm->pEnemy_ptr = (ULONG *)pYo;
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
			} else {
				if ((pYo->Type & 1) == 0) {
					return;
				}
				if ((pYo->Be_flg & 0x4000000) == 0) {
					return;
				}
				SPL_SET_ROUTINE(pEm, 0x0501);
				pEm->pEnemy_ptr = (ULONG *)pYo;
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			}
		}
	}
}

/* DS 0x80060e08 (Aida_em10_at00_spl) */
static VOID Aida_em10_at00_spl(register ENEMY_WORK *pEm)
{
	ENEMY_WORK *pYo;
	USHORT len;

	pYo = pSpl_near_em[0];
	if (SPL_FREE(pEm)->Cool != 0) {
		return;
	}
	if ((pYo != NULL) && (pYo->Life >= 0)) {
		if ((pYo->Type & 0x2000) == 0) {
			goto lock_target;
		}
		if (((pYo->Type & 1) != 0) && ((pYo->Be_flg & 0x4000000) != 0)) {
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pYo;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
		}
	}
	pYo = pSpl_near_em[1];
	len = Em_l[1];
	if (pYo == NULL) {
		return;
	}
	if (pYo->Life < 0) {
		return;
	}
	if ((pYo->Type & 0x2000) != 0) {
		return;
	}
	if (len > 2999) {
		return;
	}
lock_target:
	SPL_SET_ROUTINE(pEm, 0x0501);
	pEm->pEnemy_ptr = (ULONG *)pYo;
	pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
}

/* DS 0x80060ee4 (Aida_em25_at00_pl) */
static VOID Aida_em25_at00_pl(register ENEMY_WORK *pEm, register ULONG Type)
{
	USHORT status;
	USHORT em_l0;
	ENEMY_WORK *pYo;
	UINT dy_u;
	INT dy;

	em_l0 = Em_l[0];
	pYo = pSpl_near_em[0];
	if ((Type & 0xf) == 10) {
		pYo = (ENEMY_WORK *)Spl_em_cdir_ck(pEm);
		if (pYo == NULL) {
			return;
		}
		if (pYo->Life >= 0) {
			goto height_ck;
		}
		pYo = (ENEMY_WORK *)Spl_pllock_get(pEm, pYo);
	}
	if (pYo == NULL) {
		return;
	}
height_ck:
	dy = pYo->Pos_y - pEm->Pos_y;
	if ((pYo->Life >= 0) && (em_l0 < 0x5dc)) {
		dy_u = (UINT)(dy + 4999) & 0xffff;
		if (dy_u < 6999) {
			status = pEm->Status_flg;
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pYo;
			pEm->Status_flg = (status & 0x1fff) | 0x4000;
			if (dy_u < 2999) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			}
			if (((UINT)(dy + 499) & 0xffff) < 0x9c3) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			}
		}
	}
}

/* DS 0x80060fec (Aida_em25_at00_spl) */
static VOID Aida_em25_at00_spl(register ENEMY_WORK *pEm)
{
	USHORT em_l1;
	ENEMY_WORK *pNear0;
	ENEMY_WORK *pNear1;
	INT dy;
	INT dy0;
	UINT dy_u;

	pNear0 = pSpl_near_em[0];
	if (pNear0 != NULL) {
		dy0 = pNear0->Pos_y - pEm->Pos_y;
		if ((pNear0->Life >= 0) && ((pNear0->Type & 0x2000) == 0) && (((UINT)(dy0 + 4999) & 0xffff) < 0x1b57)) {
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pNear0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
			dy0 = pNear0->Pos_y - pEm->Pos_y;
			if (((UINT)(dy0 + 4999) & 0xffff) < 2999) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			}
			if (((UINT)(dy0 + 499) & 0xffff) < 0x9c3) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			}
			return;
		}
	}
	pNear1 = pSpl_near_em[1];
	em_l1 = Em_l[1];
	if (pNear1 == NULL) {
		return;
	}
	dy = pNear1->Pos_y - pEm->Pos_y;
	if (pNear1->Life < 0) {
		return;
	}
	if ((pNear1->Type & 0x2000) != 0) {
		return;
	}
	if (em_l1 > 1999) {
		return;
	}
	dy_u = (UINT)(dy + 4999) & 0xffff;
	if (dy_u > 0x1b56) {
		return;
	}
	SPL_SET_ROUTINE(pEm, 0x0501);
	pEm->pEnemy_ptr = (ULONG *)pNear1;
	pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
	if (dy_u < 2999) {
		pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
	}
	if (((UINT)(dy + 499) & 0xffff) < 0x9c3) {
		pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
	}
}

/* DS 0x80061140 (Aida_em26_at00_pl) */
static VOID Aida_em26_at00_pl(VOID)
{
}

/* DS 0x80061148 (Aida_em26_at00_spl) */
static VOID Aida_em26_at00_spl(VOID)
{
}

/* DS 0x80061150 (Aida_mv00_br) */
static VOID Aida_mv00_br(register ENEMY_WORK *pEm)
{
	USHORT dist;
	UCHAR cdir_ck;
	LONG goto_res;
	ULONG routine;
	INT life_shift;
	INT life_max;
	INT life_rate;

	if (pEm->Routine_2 == 0) {
		pEm->D_life_u = 0;
		life_max = pEm->Max_life;
		life_shift = pEm->Life << 12;
		DIV_S(life_rate, life_shift, life_max);	/* DS: div with break checks */
		if ((G.SaveData.Dummy1[1] & 1) != 0) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 2000) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 800) {
			pEm->D_life_u = 2;
		}
	}
	if ((pSpl_near_em[0] != NULL) && ((G.Room_player_flg & 1) != 0)) {
		((CODE_PTR)(Aida_em_at00_pl[E_A_at_tbl[pSpl_near_em[0]->Id - 0x10]]))(pEm, pEm->Type);
	}
	goto_res = Goto00_ck(pEm, SPL_FREE(pEm)->Goal_x, SPL_FREE(pEm)->Goal_z, 0x400);
	if ((SPL_FREE(pEm)->Flg & 0x8000) == 0) {
		if (((pEm->Type & 0xf) == 10) && (500 < SPL_FREE(pEm)->Dist)) {
			routine = 0x0c01;
			if (goto_res == 0) {
				routine = 0x0101;
			}
		} else {
			dist = SPL_FREE(pEm)->Dist;
			if (dist < 0xbb9) {
				if (dist < 0x1771) {
					if ((pEm->L_pl < 4000) && (G.Player.Routine_1 == 2)) {
						cdir_ck = Cdir_ck2(pEm, (ENEMY_WORK *)&G.Player);
						if (cdir_ck == 0) {
							goto_res = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
							routine = 0x0301;
							if (goto_res == 0) {
								SPL_SET_ROUTINE(pEm, routine);
								return;
							}
						}
					}
					if (pEm->L_pl > 999) {
						return;
					}
					if (G.Player.Routine_1 != 1) {
						return;
					}
					cdir_ck = Cdir_ck2(pEm, (ENEMY_WORK *)&G.Player);
					if (cdir_ck != 0) {
						return;
					}
					goto_res = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
					routine = 0x0301;
					if (goto_res != 0) {
						return;
					}
				} else {
					routine = 0x0d01;
					if (goto_res == 0) {
						routine = 0x0201;
					}
				}
			} else {
				routine = 0x0c01;
				if (goto_res == 0) {
					routine = 0x0101;
				}
			}
		}
	} else {
		routine = 0x0c01;
		if (goto_res == 0) {
			routine = 0x0101;
		}
	}
	SPL_SET_ROUTINE(pEm, routine);
}

/* DS 0x800613c0 (Aida_mv00_br_1) */
static VOID Aida_mv00_br_1(register ENEMY_WORK *pEm)
{
	USHORT dist;
	ENEMY_WORK *pNear0;
	LONG goto_res;
	ULONG routine;
	INT life_shift;
	INT life_max;
	INT life_rate;

	pNear0 = pSpl_near_em[0];
	if (pEm->Routine_2 == 0) {
		pEm->D_life_u = 0;
		life_max = pEm->Max_life;
		life_shift = pEm->Life << 12;
		DIV_S(life_rate, life_shift, life_max);	/* DS: div with break checks */
		if ((G.SaveData.Dummy1[1] & 1) != 0) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 2000) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 800) {
			pEm->D_life_u = 2;
		}
	}
	if (pNear0 != NULL) {
		((CODE_PTR)(Aida_em_at00_spl[E_A_at_tbl[pNear0->Id - 0x10]]))(pEm, pEm->Type);
	}
	goto_res = Goto00_ck(pEm, SPL_FREE(pEm)->Goal_x, SPL_FREE(pEm)->Goal_z, 0x400);
	dist = SPL_FREE(pEm)->Dist;
	if (dist >= 0xbb9) {
		routine = 0x0c01;
		if (goto_res == 0) {
			routine = 0x0101;
		}
	} else {
		if (dist < 0x1771) {
			return;
		}
		routine = 0x0d01;
		if (goto_res == 0) {
			routine = 0x0201;
		}
	}
	SPL_SET_ROUTINE(pEm, routine);
}

/* DS 0x800614f8 (Aida_mv00) */
static VOID Aida_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	USHORT Mot[3];
	ULONG move;

	memcpy(Mot, Spl_mv_taiki_rom, sizeof(Mot));
	switch (pEm->Routine_2) {
	case 0:
		move = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(int *)&pEm->Move_no = move + 0x70000;
		/* DS: the first pass sets Routine_2 to 1 and drops into the
		 * Joint_move call of case 1 in the same frame. */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		break;
	}
}

/* DS 0x80061594 (Aida_em10_at01) */
static VOID Aida_em10_at01(register ENEMY_WORK *pEm)
{
	ENEMY_WORK *pNear0;
	USHORT em_l0;

	pNear0 = pSpl_near_em[0];
	em_l0 = Em_l[0];
	if ((SPL_FREE(pEm)->Cool == 0) && (pNear0 != NULL) && (pNear0->Life >= 0)) {
		if (((pNear0->Type & 0x2000) == 0) && (em_l0 < 0x9c4)) {
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pNear0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
		} else {
			if (pNear0->Life < 0) {
				return;
			}
			if ((pNear0->Type & 1) == 0) {
				return;
			}
			if ((pNear0->Be_flg & 0x4000000) == 0) {
				return;
			}
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pNear0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
		}
	}
}

/* DS 0x80061654 (Aida_em25_at01) */
static VOID Aida_em25_at01(register ENEMY_WORK *pEm)
{
	USHORT em_l0;
	ENEMY_WORK *pNear0;
	UINT dy_u;
	INT dy;

	pNear0 = pSpl_near_em[0];
	em_l0 = Em_l[0];
	if (pNear0 == NULL) {
		return;
	}
	dy = pNear0->Pos_y - pEm->Pos_y;
	if ((pNear0->Life >= 0) && ((pNear0->Type & 0x2000) == 0) && (em_l0 < 0x9c4)) {
		dy_u = (UINT)(dy + 4999) & 0xffff;
		if (dy_u < 6999) {
			SPL_SET_ROUTINE(pEm, 0x0501);
			pEm->pEnemy_ptr = (ULONG *)pNear0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
			if (dy_u < 2999) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			}
			if (((UINT)(dy + 499) & 0xffff) < 0x9c3) {
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			}
		}
	}
}

/* DS 0x8006170c (Aida_em26_at01) */
static VOID Aida_em26_at01(VOID)
{
}

/* Shared first half of the Aida_mv01 / Aida_mv02 "random turn" state machine */
/* DS 0x80061714 (Aida_mv01_br) */
static VOID Aida_mv01_br(register ENEMY_WORK *pEm)
{
	USHORT dist;
	ENEMY_WORK *pNear0;
	UCHAR rnd_a;
	UCHAR rnd_b;
	SHORT s_work;
	LONG tgt_x;
	LONG Vec_z;
	UINT em_l0;

	pNear0 = pSpl_near_em[0];
	em_l0 = (UINT)Em_l[0];
	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
			goto turn_ck;
		}
	} else {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = Rnd();
				rnd_b = Rnd();
				s_work = pEm->Cdir_y;
				SPL_FREE(pEm)->Timer4 = 0x1e;
				SPL_FREE(pEm)->Turn_dir = (SHORT)(s_work - ((((UINT)rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880));
				return;
			}
			goto turn_ck;
		}
	}
turn_ck:
	if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
		goto dist_ck;
	}
	if (SPL_FREE(pEm)->Timer4 != 0) {
		Goto01(pEm, SPL_FREE(pEm)->Turn_dir, 0x28);
		s_work = SPL_FREE(pEm)->Timer4;
		SPL_FREE(pEm)->Timer4 = s_work - 1;
		if (s_work != 1) {
			return;
		}
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xefff;
		return;
	}
dist_ck:
	if (pNear0 != NULL) {
		if (pEm->D_life_u != 0) {
			((CODE_PTR)(Aida_em_at01[E_A_at_tbl[pNear0->Id - 0x10]]))(pEm, pEm->Type);
		}
		if ((em_l0 < 2000) && (em_l0 < pEm->L_pl)) {
			((CODE_PTR)(Aida_em_at01[E_A_at_tbl[pNear0->Id - 0x10]]))(pEm, pEm->Type);
		}
	}
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 300) {
		SPL_SET_ROUTINE(pEm, 0x0b01);
	} else if (dist < 0x1389) {
		if ((dist < 2000) && (G.Player.Routine_1 == 2) && (Cdir_ck2(pEm, (ENEMY_WORK *)&G.Player) == 0)) {
			SPL_SET_ROUTINE(pEm, 0x0301);
		} else {
			if ((em_l0 < 0x5dc) && ((tgt_x = Goto00_ck(pEm, pNear0->Pos_x, pNear0->Pos_z, 0x4b0)) == 0) && ((s_work = -0x28) == -0x28) && ((pEm->Sca_info & 1) == 0)) {
				tgt_x = pNear0->Pos_x;
				Vec_z = pNear0->Pos_z;
			} else {
				tgt_x = pEm->Dest_x;
				Vec_z = pEm->Dest_z;
				s_work = 0x28;
			}
			Goto00(pEm, tgt_x, Vec_z, s_work);
			Oma_Spl_updown_ck(pEm);
		}
	} else {
		SPL_SET_ROUTINE(pEm, 0x0201);
	}
}

/* DS 0x800619e4 (Aida_mv01_br_1) */
static VOID Aida_mv01_br_1(register ENEMY_WORK *pEm)
{
	USHORT dist;
	UCHAR rnd_a;
	UCHAR rnd_b;
	SHORT s_work;

	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
			goto turn_ck;
		}
	} else {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = Rnd();
				rnd_b = Rnd();
				s_work = pEm->Cdir_y;
				SPL_FREE(pEm)->Timer4 = 0x1e;
				SPL_FREE(pEm)->Turn_dir = (SHORT)(s_work - ((((UINT)rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880));
				return;
			}
			goto turn_ck;
		}
	}
turn_ck:
	if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
		goto dist_ck;
	}
	if (SPL_FREE(pEm)->Timer4 != 0) {
		Goto01(pEm, SPL_FREE(pEm)->Turn_dir, 0x28);
		s_work = SPL_FREE(pEm)->Timer4;
		SPL_FREE(pEm)->Timer4 = s_work - 1;
		if (s_work != 1) {
			return;
		}
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xefff;
		return;
	}
dist_ck:
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 300) {
		SPL_SET_ROUTINE(pEm, 0x0b01);
	} else if (dist < 0x1389) {
		if ((dist < 2000) && (G.Player.Routine_1 == 2) && (Cdir_ck2(pEm, (ENEMY_WORK *)&G.Player) == 0)) {
			SPL_SET_ROUTINE(pEm, 0x0301);
		} else {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x28);
			Oma_Spl_updown_ck(pEm);
		}
	} else {
		SPL_SET_ROUTINE(pEm, 0x0201);
	}
}

/* DS 0x80061bbc (Aida_mv01) */
static VOID Aida_mv01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	SHORT s_work;
	SHORT Mot[3];

	memcpy(Mot, Spl_mv01_rom, sizeof(Mot));
	switch (pEm->Routine_2) {
	case 0:
		s_work = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x3c;
		pEm->Timer1 = 0x3c;
		*(int *)&pEm->Move_no = s_work + 0x70000;
		/* fall through */
	case 1:
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
		}
		if ((SPL_FREE(pEm)->Dist < 0x5dc) && ((pEm->Sca_info & 1) != 0)) {
			s_work = pEm->Timer0;
			pEm->Timer0 = s_work - 1;
			if (s_work == 1) {
				SPL_SET_ROUTINE(pEm, 0x0401);
			}
		} else {
			pEm->Timer0 = 0x3c;
		}
		if (pEm->L_pl < 0x5dc) {
			s_work = pEm->Timer1;
			pEm->Timer1 = s_work - 1;
			if (s_work == 1) {
				SPL_SET_ROUTINE(pEm, 0x0401);
			}
		} else {
			pEm->Timer1 = 0x3c;
		}
		break;
	}
	if ((INT)pEm->Water < pEm->Ground - 500) {
		s_work = pEm->Spd_x;
		pEm->Spd_x = s_work - s_work / 3;
	}
	Add_speedXZ(pEm, 0);
}

/* DS 0x80061d90 (Aida_mv02_br) */
static VOID Aida_mv02_br(register ENEMY_WORK *pEm)
{
	ENEMY_WORK *pNear0;
	USHORT em_l0;
	UCHAR rnd_a;
	UCHAR rnd_b;
	SHORT s_work;
	LONG tgt_x;
	LONG Vec_z;

	em_l0 = Em_l[0];
	pNear0 = pSpl_near_em[0];
	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
			goto turn_ck;
		}
	} else {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = Rnd();
				rnd_b = Rnd();
				s_work = pEm->Cdir_y;
				SPL_FREE(pEm)->Timer4 = 0x0f;
				SPL_FREE(pEm)->Turn_dir = (SHORT)(s_work - ((((UINT)rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880));
				return;
			}
			goto turn_ck;
		}
	}
turn_ck:
	if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
		goto dist_ck;
	}
	if (SPL_FREE(pEm)->Timer4 != 0) {
		Goto01(pEm, SPL_FREE(pEm)->Turn_dir, 0x50);
		s_work = SPL_FREE(pEm)->Timer4;
		SPL_FREE(pEm)->Timer4 = s_work - 1;
		if (s_work != 1) {
			return;
		}
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xefff;
		return;
	}
dist_ck:
	if (SPL_FREE(pEm)->Dist < 3000) {
		SPL_SET_ROUTINE(pEm, 0x0101);
	} else {
		if ((em_l0 < 2000) && ((tgt_x = Goto00_ck(pEm, pNear0->Pos_x, pNear0->Pos_z, 0x400)) == 0) && ((s_work = -0x38) == -0x38) && ((pEm->Sca_info & 1) == 0)) {
			tgt_x = pNear0->Pos_x;
			Vec_z = pNear0->Pos_z;
		} else {
			tgt_x = pEm->Dest_x;
			Vec_z = pEm->Dest_z;
			s_work = 0x38;
		}
		Goto00(pEm, tgt_x, Vec_z, s_work);
		Oma_Spl_updown_ck(pEm);
	}
}

/* DS 0x80061f68 (Aida_mv02_br_1) */
static VOID Aida_mv02_br_1(register ENEMY_WORK *pEm)
{
	UCHAR rnd_a;
	UCHAR rnd_b;
	SHORT s_work;

	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
			goto turn_ck;
		}
	} else {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = Rnd();
				rnd_b = Rnd();
				s_work = pEm->Cdir_y;
				SPL_FREE(pEm)->Timer4 = 0x0f;
				SPL_FREE(pEm)->Turn_dir = (SHORT)(s_work - ((((UINT)rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880));
				return;
			}
			goto turn_ck;
		}
	}
turn_ck:
	if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
		goto dist_ck;
	}
	if (SPL_FREE(pEm)->Timer4 != 0) {
		Goto01(pEm, SPL_FREE(pEm)->Turn_dir, 0x50);
		s_work = SPL_FREE(pEm)->Timer4;
		SPL_FREE(pEm)->Timer4 = s_work - 1;
		if (s_work != 1) {
			return;
		}
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xefff;
		return;
	}
dist_ck:
	if (SPL_FREE(pEm)->Dist < 3000) {
		SPL_SET_ROUTINE(pEm, 0x0101);
	} else {
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x38);
		Oma_Spl_updown_ck(pEm);
	}
}

/* DS 0x800620ec (Aida_mv02) */
static VOID Aida_mv02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR mv_no;
	SHORT spd_x;
	USHORT lr;
	UCHAR Mot[3];

	memcpy(Mot, Spl_mv02_rom, sizeof(Mot));
	switch (pEm->Routine_2) {
	case 0:
		mv_no = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		SPL_SET_MOVE(pEm, (UINT)mv_no + 0x70000);
		/* fall through */
	case 1:
		Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
			Snd_se_walk(1, lr, pEm);
		}
		break;
	}
	if (pEm->Hokan_flg != 0) {
		pEm->Spd_x >>= 1;
	}
	if ((INT)pEm->Water < pEm->Ground - 500) {
		/* DS: x - ((x * 0x55555556) >> 32) - (x >> 31) is C's truncating x / 3 for every SHORT x */
		spd_x = pEm->Spd_x;
		pEm->Spd_x = spd_x - spd_x / 3;
	}
	Add_speedXZ(pEm, 0);
}

/* DS 0x80062248 (Aida_mv03_br) */
static VOID Aida_mv03_br(register ENEMY_WORK *pEm)
{
	/* DS has no local here: the Goto00_ck result is scaled straight into Timer1, and the
	   zero test reads the SHORT just stored (sll 0x15 of the x3 product at DS 0x8006228c,
	   bne in front of the sh at DS 0x80062290..0x80062294). Timer2 is read signed (lb at
	   DS 0x800622c0 and 0x800622d8, then slti), so both Timer2 tests cast to signed char. */
	if (pEm->Routine_2 == 0) {
		if ((pEm->Timer1 = Goto00_ck((ENEMY_WORK *)&G.Player, pEm->Pos_x, pEm->Pos_z, 1) * 0x60) == 0) {
			pEm->Timer1 = 0x60;
		}
		pEm->Timer2 = 0;
	}
	if ((Goto01_ck(pEm, G.Player.Cdir_y, 0x200) == 0) && ((signed char)pEm->Timer2 > 5)) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	}
	if ((signed char)pEm->Timer2 > 0x78) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	}
	pEm->Cdir_y += pEm->Timer1;
}

/* DS 0x80062318 (Aida_mv03) */
static VOID Aida_mv03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR mv_no;
	USHORT lr;
	UCHAR Mot[3];

	memcpy(Mot, Spl_mv03_rom, sizeof(Mot));
	switch (pEm->Routine_2) {
	case 0:
		mv_no = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		pEm->Timer2 = 0;
		SPL_SET_MOVE(pEm, (UINT)mv_no + 0x70000);
		if ((pEm->Timer1 = Goto00_ck((ENEMY_WORK *)&G.Player, pEm->Pos_x, pEm->Pos_z, 1) << 7) == 0) {
			pEm->Timer1 = 0x80;
		}
		pEm->Timer0 = 0x3c;
		/* falls through to the walk step on the same frame */
	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x200);
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
			Snd_se_walk(0, lr, pEm);
		}
		pEm->Timer2++;
	}
	if (pEm->Hokan_flg != 0) {
		pEm->Spd_x >>= 1;
	}
	Add_speedXZ(pEm, 0);
}

/* DS 0x80062490 (Aida_mv04_br) */
static VOID Aida_mv04_br(register ENEMY_WORK *pEm)
{
	/* G.Player is read directly, not through a pointer local. DS loads &G.Player.Pos_x
	 * once into s1 (lui/addiu) and reads Pos_x and Pos_z as 0x0(s1) and 0x8(s1) before
	 * both Goto00_ck and Goto00; a pointer local is folded away to separate absolute loads. */
	if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x50) == 0) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	} else {
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x50);
	}
}

/* DS 0x800624f8 (Aida_mv04) */
static VOID Aida_mv04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	SHORT mv_no;
	SHORT Mot[3];

	memcpy(Mot, Spl_mv01_rom, sizeof(Mot));
	if (pEm->Routine_2 == 0) {
		mv_no = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		SPL_SET_MOVE(pEm, mv_no + 0x70000);
	} else if (pEm->Routine_2 != 1) {
		return;
	}
	Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		Snd_se_walk(0, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
	}
}

/* DS 0x800625dc (Aida_mv05_br) */
static VOID Aida_mv05_br(register ENEMY_WORK *pEm)
{
	((CODE_PTR)(Aida_mv05_w1_br_tbl[pEm->Routine_2]))(pEm);
}

/* DS 0x80062618 (Aida_mv05) */
static VOID Aida_mv05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	Aida_mv05_w1(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr);
	if ((pEm->Routine_2 != 0) || (pEm->Hokan_flg == 0)) {
		Foot_set_pl(pEm, 0, 1);
	}
}

/* DS 0x80062678 (Aida_mv05_w1) */
static VOID Aida_mv05_w1(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Aida_mv05_w1_tbl[pEm->Routine_2]))(pEm, Kan, Seq);
}

/* DS 0x800626b4 (Aida_mv05_w1_t0_br) */
static VOID Aida_mv05_w1_t0_br(VOID)
{
}

/* DS 0x800626bc (Aida_mv05_w1_t0) */
static VOID Aida_mv05_w1_t0(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR sub_state;
	SHORT water;
	ENEMY_WORK *pTarget;
	ULONG joint_res;

	sub_state = pEm->Routine_3;
	if (sub_state != 1) {
		if (sub_state > 1) {
			if (sub_state != 2) {
				return;
			}
			*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
			pEm->Hokan_flg = 7;
			return;
		}
		if (sub_state != 0) {
			return;
		}
		pEm->Routine_3 = 1;
		SPL_SET_MOVE(pEm, 0x70009);
		pEm->Timer3 = 6;
		pEm->Neck_flg |= 0x12;
		water = pEm->Water;
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x4000;
		if ((water != 0) && (water < pEm->Ground)) {
			Pl_water(pEm);
		}
	}
	pTarget = (ENEMY_WORK *)pEm->pEnemy_ptr;
	if (pTarget != pEm) {
		Goto00(pEm, pTarget->Pos_x, pTarget->Pos_z, 0xd8);
	}
	joint_res = Joint_move(pEm, Kan, Seq, 0x200);
	pEm->Routine_3 = pEm->Routine_3 + (CHAR)joint_res;
}

/* DS 0x800627dc (Aida_em10_at05) */
static VOID Aida_em10_at05(register ENEMY_WORK *pEm)
{
	signed char timer3_next;
	ENEMY_WORK *pTarget;

	pTarget = (ENEMY_WORK *)pEm->pEnemy_ptr;
	if ((pTarget->Damage_cnt == 0) && (pTarget->Life >= 0)) {
		if ((pTarget->Type & 0x2000) == 0) {
			timer3_next = (signed char)(pEm->Timer3 - 1);
			pEm->Timer3 = timer3_next;
			if (timer3_next >= 0) {
				*(USHORT *)&pEm->Routine_2 = 0x0002;		/* Routine_2 2, Routine_3 0 (DS: one sh at +0x6) */
				return;
			}
			goto timer3_reset;
		}
		if (((pEm->Type & 1) != 0) && ((pEm->Status_flg & 0x2000) != 0)) {
			timer3_next = (signed char)(pEm->Timer3 - 1);
			pEm->Timer3 = timer3_next;
			if (timer3_next >= 0) {
				*(USHORT *)&pEm->Routine_2 = 0x0002;
				return;
			}
timer3_reset:
			*(USHORT *)&pEm->Routine_2 = 0x0002;		/* Routine_2 2, Routine_3 0 (DS: one sh at +0x6) */
			pEm->Timer3 = 0x0d;
			return;
		}
		if ((pTarget->Life >= 0) && ((pTarget->Type & 0x2000) == 0)) {
			return;
		}
	}
	*(USHORT *)&pEm->Routine_2 = 0x0003;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
}

/* DS 0x800628c4 (Aida_mv05_w1_t1_br) */
static VOID Aida_mv05_w1_t1_br(register ENEMY_WORK *pEm)
{
	if ((SPL_FREE(pEm)->Flg & 0x4000) == 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0003;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
	} else if (((pEm->Status_flg & 0xa000) == 0) || (pEm->Hokan_flg == 0)) {
		((CODE_PTR)(Aida_em_at05[E_A_at_tbl[((ENEMY_WORK *)pEm->pEnemy_ptr)->Id - 0x10]]))(pEm, pEm->Type);
	}
}

/* DS 0x80062954 (Aida_mv05_w1_t1) */
static VOID Aida_mv05_w1_t1(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	LONG s_work;
	SHORT timer0_new;
	ULONG mot_idx;
	SHORT Mot[5];

	memcpy(Mot, Spl_mv05_t1_rom, sizeof(Mot));
	mot_idx = pEm->Status_flg >> 13;
	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		s_work = Mot[mot_idx];
		pEm->Timer0 = 0x1e;
		SPL_SET_MOVE(pEm, s_work + 0x70000);
	}
	Joint_move(pEm, Kan, Seq, 0x200);
	timer0_new = pEm->Timer0 - 1;
	pEm->Timer0 = timer0_new;
	if (timer0_new == 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0003;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
	}
}

/* DS 0x80062a20 (Aida_mv05_w1_t2_br) */
static VOID Aida_mv05_w1_t2_br(VOID)
{
}

/* DS 0x80062a28 (Aida_mv05_w1_t2) */
static VOID Aida_mv05_w1_t2(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR Tmp_id;
	ULONG Ck;
	MATRIX *pmat;
	struct mv_tbl { SHORT w[5]; } Gun_at_mot;
	SVECTOR Sv;

	/* retail copies the ROM table to the stack and indexes the copy */
	Gun_at_mot = *(struct mv_tbl *)Spl_mv05_t2_rom;
	if (pEm->Routine_3 == 0) {
		/* local names Gun_at_mot, Tmp_id, Ck and Sv are the mockup's.
		   DS reads Id, stores Routine_3, loads the table entry with lh and
		   only then stores Id = 1, so the move number is written before
		   the Id store here and the entry is not kept in a SHORT local. */
		Tmp_id = pEm->Id;
		pEm->Routine_3 = 1;
		SPL_SET_MOVE(pEm, Gun_at_mot.w[pEm->Status_flg >> 13] + 0x70000);
		pEm->Id = 1;
		Hand_ck(pEm, pEm->pSin_parts_ptr[0xe].Workm.t[1], 3);
		pEm->Id = Tmp_id;
		init_Weapon_at(pEm, Pl_weapon_tbl[2] + Gun_jtg_bit[pEm->Be_flg >> 0x1d]);
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xbfff;
		if ((pEm->Water != 0) && (pEm->Water < pEm->Ground)) {
			Pl_water(pEm);
		}
		/* DS loads the part-11 matrix pointer before the three Sv stores */
		pmat = &pEm->pSin_parts_ptr[0xb].Workm;
		Sv.vx = 0x8a;
		Sv.vy = 0x267;
		Sv.vz = 8;
		Esp_call(0x30a0e00, 0, pmat, &Sv);
		Esp[Esp_call(0x1000e00, 0, pmat, &Sv)].Padd0 = 1;
	}
	Ck = Weapon_at_ck(pEm, (pEm->Be_flg >> 0x1d) | 0x20000, Pl_weapon_tbl[2] + Gun_jtg_bit[pEm->Be_flg >> 0x1d]);
	if ((Ck != 0) && (Ck != 0xff)) {
		SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x4000;
	}
	if (Joint_move(pEm, Kan, Seq, 0x200) != 0) {
		*(USHORT *)&pEm->Routine_2 = 1;
	}
}

/* DS 0x80062c44 (Aida_mv05_w1_t3_br) */
static VOID Aida_mv05_w1_t3_br(VOID)
{
}

/* DS 0x80062c4c (Aida_mv05_w1_t3) */
static VOID Aida_mv05_w1_t3(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR neck_flg;
	ULONG res;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		SPL_SET_MOVE(pEm, 0xf0109);
	}
	res = Joint_move(pEm, Kan, Seq, 0x10100);
	if (res != 0) {
		SPL_SET_ROUTINE(pEm, 0x0001);
		neck_flg = pEm->Neck_flg;
		SPL_FREE(pEm)->Cool = 0x1e;
		pEm->Neck_flg = neck_flg & 0xed;
	}
}

/* DS 0x80062cc0 (Aida_mv05_w1_t4_br) */
static VOID Aida_mv05_w1_t4_br(VOID)
{
}

/* DS 0x80062cc8 (Aida_mv05_w1_t4) */
static VOID Aida_mv05_w1_t4(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG res;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		SPL_SET_MOVE(pEm, 0x70010);
	}
	res = Joint_move(pEm, Kan, Seq, 0x200);
	if (res != 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	}
}

/* DS 0x80062d24 (Aida_mv05_w1_t5_br) */
static VOID Aida_mv05_w1_t5_br(VOID)
{
}

/* DS 0x80062d2c (Aida_mv05_w1_t5) */
static VOID Aida_mv05_w1_t5(register ENEMY_WORK *pEm)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		SPL_SET_MOVE(pEm, 0xf0309);
		/* fall through */
	case 1:
		{
			register ENEMY_WORK *pTa;	/* target enemy (beta local name pTa) */

			pTa = (ENEMY_WORK *)pEm->pEnemy_ptr;
			Goto00(pEm, pTa->Pos_x, pTa->Pos_z, 0xd8);
			if (Goto00_ck(pEm, pTa->Pos_x, pTa->Pos_z, 0x10) == 0) {
				*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
			}
		}
		break;
	}
}

/* DS 0x80062dc4 (Aida_mv06_br) */
static VOID Aida_mv06_br(VOID)
{
}

/* DS 0x80062dcc (Aida_mv06) */
static VOID Aida_mv06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG res;

	switch (pEm->Routine_2) {
	case 0:
		SPL_SET_MOVE(pEm, 0x70006);
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;
		/* fall through */
	case 1:
		res = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 2:
		pEm->Routine_2 = 4;
		break;
	case 4:
		SPL_SET_MOVE(pEm, 0x70006);
		pEm->Routine_2 = 5;
		/* fall through */
	case 5:
		res = Joint_move(pEm, Kan, Seq, 0x10200);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 6:
		SPL_SET_ROUTINE(pEm, 0x0001);
		break;
	}
}

/* DS 0x80062e90 (Aida_mv07_br) */
static VOID Aida_mv07_br(VOID)
{
}

/* DS 0x80062e98 (Aida_mv07) */
VOID Aida_mv07(register ENEMY_WORK *pEm)
{
	UCHAR mv_no;
	SHORT timer1_cur;
	LONG goto_res;
	ULONG res;
	UCHAR nfloor_new;
	/* Motion numbers by Timer2 (DS builds the two bytes on the stack). */
	UCHAR Mot[2] = { 0, 3 };
	/* Footstep side held in a USHORT local, so the sum keeps DS's sll/addu/addiu shape (as in Spl_sce_mv01). */
	USHORT lr;

	switch (pEm->Routine_2) {
	case 0:
		SPL_SET_MOVE(pEm, 0x70000);
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;
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
		nfloor_new = (UCHAR)(pEm->nFloor + (CHAR)pEm->Timer1);
		mv_no = Mot[(signed char)pEm->Timer2];
		pEm->Routine_2 = 3;
		pEm->nFloor = nfloor_new;
		SPL_SET_MOVE(pEm, (UINT)mv_no + 0x30000);
		if ((signed char)pEm->Timer2 != 0) {
			pEm->nFloor = (UCHAR)(nfloor_new + (CHAR)pEm->Timer1 * -2);
		}
		/* fall through */
	case 3:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x400);
		if (res != 0) {
			pEm->Routine_2 = 4;
			SPL_SET_MOVE(pEm, (UINT)Mot[(signed char)pEm->Timer2] + 1);
			timer1_cur = pEm->Timer1;
			timer1_cur--;
			pEm->Timer1 = timer1_cur;
			if (timer1_cur == 0) {
				pEm->Routine_2 = 5;
				SPL_SET_MOVE(pEm, (UINT)Mot[(signed char)pEm->Timer2] + 2);
			}
		}
		Add_speedXYZ(pEm, 0, 0);
		break;
	case 4:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if ((res != 0) && ((timer1_cur = pEm->Timer1 - 1, pEm->Timer1 = timer1_cur, timer1_cur == 0))) {
			pEm->Routine_2 = 5;
			SPL_SET_MOVE(pEm, (UINT)Mot[(signed char)pEm->Timer2] + 2);
		}
		Add_speedXYZ(pEm, 0, 0);
		break;
	case 5:
		Spd_get(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0);
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if (res != 0) {
			pEm->Be_flg &= 0xfffffff7;
			pEm->Damage_cnt &= 0x7f;
			Flg_on(G.SaveData.Room_flg, (UINT)pEm->Set_flg_no);
			pEm->Routine_1 = 6;
			pEm->Routine_2 = 0;
		}
		Add_speedXYZ(pEm, 0, 0);
		break;
	default:
		break;
	}
	if ((*pEm->pNow_seq & 0x4000) != 0) {
		lr = ((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4;
		Snd_se_walk(1, lr, pEm);
		G.Room_player_flg |= 4;
	}
}

/* DS 0x80063164 (Aida_mv08_br) */
static VOID Aida_mv08_br(VOID)
{
}

/* DS 0x8006316c (Aida_mv08) */
VOID Aida_mv08(register ENEMY_WORK *pEm)
{
	USHORT cdir_y;
	SHORT pos_x_s;
	SHORT pos_z_s;
	USHORT cdir_step;
	UCHAR cnt_want;
	UCHAR cnt;
	ULONG res;
	UINT id;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Move_no = 6;
		if ((pEm->Routine_2 & 2) != 0) {
			pEm->Move_no = 7;
		}
		pEm->Hokan_flg = 7;
		pEm->Move_cnt = 0;
		SPL_SET_MOVE(pEm, 0x70000);
		pEm->Spd_x = 0x32;
		pEm->Cdir_y = (SHORT)SPL_FREE(pEm)->Step;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		cdir_y = pEm->Cdir_y;
		if ((cdir_y & 0x200) == 0) {
			cdir_step = -(USHORT)((cdir_y >> 2) & 0xff);
		} else {
			cdir_step = (USHORT)((cdir_y >> 2) & 0xff);
		}
		pEm->Cdir_y = cdir_y + cdir_step;
		if ((pEm->Cdir_y & 0x3e0) == 0) {
			pEm->Cdir_y &= 0xff00;
			pEm->Routine_3 = 2;
			if ((pEm->Routine_2 & 2) != 0) {
				pEm->Routine_3 = 0;
				pEm->Routine_1++;
			}
		}
		break;
	case 2:
		pEm->Routine_3 = 3;
		pEm->Move_no = 6;
		pEm->Hokan_flg = 3;
		/* fall through */
	case 3:
		if (pEm->Move_cnt == 0x11) {
			pEm->Damage_cnt |= 0x80;
		}
		id = pEm->Id;
		if ((id == 0x45) || (id == 0x4f)) {
			cnt = pEm->Move_cnt;
			cnt_want = 119;
			if (cnt == 99) {
				pEm->Hokan_flg = 0;
				if (id < 0x10) {
					pEm->Spd_x = Up_add_Spl_tbl[id + 0x10];
				} else {
					pEm->Spd_x = (SHORT)((UCHAR *)Em4a_zobiparts_tbl)[id + 4];
				}
				Add_speedXZ(pEm, 0);
				pos_x_s = (SHORT)pEm->Pos_x;
				pos_z_s = (SHORT)pEm->Pos_z;
				pEm->Pos_y -= 0x708;
				pEm->nFloor++;
				pEm->Sca_old_x = pos_x_s;
				pEm->Old_x = pos_x_s;
				pEm->Sca_old_z = pos_z_s;
				pEm->Old_z = pos_z_s;
				pEm->Ground -= 0x708;
				pEm->Old_y = (SHORT)pEm->Pos_y;
				cnt = pEm->Move_cnt;
				cnt_want = 119;
			}
		} else {
			cnt = pEm->Move_cnt;
			cnt_want = 49;
			if (cnt == 0x1d) {
				if (id < 0x10) {
					pEm->Spd_x = Up_add_Spl_tbl[id + 0x10];
				} else {
					pEm->Spd_x = (SHORT)((UCHAR *)Em4a_zobiparts_tbl)[id + 4];
				}
				Add_speedXZ(pEm, 0);
				pos_x_s = (SHORT)pEm->Pos_x;
				pos_z_s = (SHORT)pEm->Pos_z;
				pEm->Pos_y -= 0x708;
				pEm->nFloor++;
				pEm->Sca_old_x = pos_x_s;
				pEm->Old_x = pos_x_s;
				pEm->Sca_old_z = pos_z_s;
				pEm->Old_z = pos_z_s;
				pEm->Ground -= 0x708;
				pEm->Old_y = (SHORT)pEm->Pos_y;
				cnt = pEm->Move_cnt;
				cnt_want = 49;
			}
		}
		if (cnt == cnt_want) {
			pEm->Damage_cnt &= 0x7f;
			pEm->Be_flg &= 0xfffffffb;
		}
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x400);
		pEm->Routine_3 = pEm->Routine_3 + (CHAR)res;
		/* fall through */
	default:
		if ((*pEm->pNow_seq & 0x4000) != 0) {
			Snd_se_walk(1, (SHORT)((*pEm->pNow_seq & 0x2000) >> 0xd) * 3 + 4, pEm);
			G.Room_player_flg |= 4;
		}
		break;
	case 4:
		if (((G.Status_flg & STAT_SUB_PLAYER) == 0) || (pEm != G.pEnemy[1])) {
			cnt = 5;
			if (pEm->Routine_1 == 0x16) {
				cnt = 4;
			}
			pEm->Routine_1 = cnt;
		} else {
			pEm->Routine_1 = 0;
		}
		*(USHORT *)&pEm->Routine_2 = 0x0000;		/* Routine_2 0, Routine_3 0 (DS: one sh at +0x6) */
		break;
	}
}

/* DS 0x8006351c (Aida_mv09_br) */
static VOID Aida_mv09_br(VOID)
{
}

/* DS 0x80063524 (Aida_mv09) */
VOID Aida_mv09(register ENEMY_WORK *pEm)
{
	UCHAR cnt;
	SHORT pos_z_s;
	SHORT s_work;
	UCHAR tgt;
	LONG ground;
	ULONG res;
	UINT id;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Move_no = 7;
		pEm->Hokan_flg = 7;
		pEm->Move_cnt = 0;
		pEm->Be_flg |= 4;
		/* fall through */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		tgt = (pEm->Id == 0x45 || pEm->Id == 0x4f) ? 0x1d : 0x0e;
		cnt = pEm->Move_cnt;
		if (cnt == tgt) {
			pEm->Routine_3 = 2;
		}
		break;
	case 2:
		pEm->Routine_3 = 3;
		pEm->Move_no = 8;
		id = pEm->Id;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 0;
		pEm->Timer0 = 0;
		pEm->Damage_cnt |= 0x80;
		if (id < 0x10) {
			pEm->Spd_x = Down_add_Spl_tbl[id + 0x10];
		} else {
			pEm->Spd_x = Down_add_tbl[id + 8];
		}
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);
		pEm->Timer0 = 5;
		s_work = (SHORT)pEm->Pos_x;
		pos_z_s = (SHORT)pEm->Pos_z;
		pEm->Pos_y += 0x226;
		pEm->Sca_old_x = s_work;
		pEm->Old_x = s_work;
		pEm->Sca_old_z = pos_z_s;
		pEm->Old_z = pos_z_s;
		pEm->nFloor -= 1;
		pEm->Old_y = (SHORT)pEm->Pos_y;
		/* fall through */
	case 3:
		s_work = pEm->Timer0;
		pEm->Timer0 = s_work + 1;
		pEm->Pos_y = pEm->Pos_y + s_work * 0x1e;
		ground = Sca_get_ground((VECTOR *)&pEm->Pos_x, 400, 0x8000, 1);
		pEm->Ground = (SHORT)ground;
		s_work = (SHORT)pEm->Pos_x;
		pos_z_s = (SHORT)pEm->Pos_z;
		pEm->Sca_old_z = pos_z_s;
		pEm->Old_z = pos_z_s;
		pEm->Sca_old_x = s_work;
		pEm->Old_x = s_work;
		pEm->Old_y = (SHORT)pEm->Pos_y;
		pEm->nFloor = (CHAR)-(pEm->Pos_y / 0x708);
		Sca_ck_hit((VECTOR *)&pEm->Pos_x, 400, 1 << (-(pEm->Pos_y / 0x708) & 0x1f), 0x8000);
		Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		if (pEm->Ground - 300 < pEm->Pos_y) {
			pEm->Routine_3 = 4;
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
		pEm->Pos_y = (LONG)pEm->Ground;
		pEm->Be_flg &= 0xfffffffb;
		Snd_se_walk(1, 4, pEm);
		G.Room_player_flg |= 4;
		/* fall through */
	case 5:
		pEm->nFloor = (CHAR)-(pEm->Pos_y / 0x708);
		res = Joint_move(pEm, (ULONG)pEm->pRoom_kan_t_ptr, (ULONG)pEm->pRoom_seq_t_ptr, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (CHAR)res;
		break;
	case 6:
		if (((G.Status_flg & STAT_SUB_PLAYER) == 0) || (pEm != G.pEnemy[1])) {
			tgt = 5;
			if (pEm->Routine_1 == 0x17) {
				tgt = 4;
			}
			pEm->Routine_1 = tgt;
		} else {
			pEm->Routine_1 = 0;
		}
		*(USHORT *)&pEm->Routine_2 = 0x0000;		/* Routine_2 0, Routine_3 0 (DS: one sh at +0x6) */
		break;
	}
}

/* DS 0x800638c0 (Aida_mv0a_br) */
static VOID Aida_mv0a_br(VOID)
{
}

/* DS 0x800638c8 (Aida_mv_nop_br): retail empty routine (jr ra) */
static VOID Aida_mv_nop_br(VOID)
{
}

/* DS 0x800638d0 (Aida_mv0a) */
static VOID Aida_mv0a(register ENEMY_WORK *pEm)
{
	/* DS keeps the address of the player's Cdir_y in s1 for the whole routine and reads it with "lh a1,0(s1)"
	   before each call, so the source holds a pointer to that field rather than reading G.Player.Cdir_y directly. */
	register SHORT *pCdir = &G.Player.Cdir_y;

	if (Goto01_ck(pEm, *pCdir, 0x50) == 0) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	} else {
		Goto01(pEm, *pCdir, 0xa0);
	}
}

/* DS 0x80063930 (Aida_mv0b_br) */
static VOID Aida_mv0b_br(register ENEMY_WORK *pEm)
{
	LONG goto_res;
	SHORT *pCdir_y = &G.Player.Cdir_y;

	/* DS keeps &G.Player.Cdir_y in s1 and reloads it with lh before each call, then passes
	 * Cdir_y + 0x800 with no sll/sra narrowing to a halfword, so both calls go through a
	 * type with LONG direction arguments (Goto01 and Goto01_ck still narrow them themselves). */
	goto_res = ((LONG (*)(ENEMY_WORK *, LONG, LONG))Goto01_ck)(pEm, *pCdir_y + 0x800, 0x50);
	if (goto_res == 0) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	} else {
		((VOID (*)(ENEMY_WORK *, LONG, LONG))Goto01)(pEm, *pCdir_y + 0x800, 0xa0);
	}
}

/* DS 0x800639a0 (Aida_mv0b_br_1) */
static VOID Aida_mv0b_br_1(register ENEMY_WORK *pEm)
{
	USHORT dist;
	ENEMY_WORK *pNear0;
	USHORT em_l0;
	LONG Vec_x;
	LONG Vec_z;
	SHORT Add_dir;
	SVECTOR Pad8;

	em_l0 = Em_l[0];
	pNear0 = pSpl_near_em[0];
	if (pEm->Routine_2 == 0) {
		pEm->Timer0 = 0x3c;
	}
	if (pEm->Timer0 != 0) {
		pEm->Timer0--;
	}
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 200) {
		SPL_SET_ROUTINE(pEm, 0x0b01);
	} else if ((pEm->Timer0 < 0x1e) && (dist > 1000)) {
		SPL_SET_ROUTINE(pEm, 0x0d01);
	} else {
		if ((SPL_FREE(pEm)->Dist > 700) && (G.Player.Spd_x > 100)) {
			SPL_SET_ROUTINE(pEm, 0x0d01);
		}
		if (pEm->Timer0 == 0) {
			SPL_SET_ROUTINE(pEm, 0x0d01);
		}
		if ((em_l0 < 0x5dc) && ((Add_dir = 0x30) == 0x30) && ((pEm->Sca_info & 1) == 0)) {
			Vec_x = pNear0->Pos_x;
			Vec_z = pEm->Pos_z;
		} else {
			Vec_x = pEm->Dest_x;
			Vec_z = pEm->Dest_z;
			Add_dir = -0x30;
		}
		Goto00(pEm, Vec_x, Vec_z, Add_dir);
	}
}

/* DS 0x80063abc (Aida_mv0b) */
static VOID Aida_mv0b(register ENEMY_WORK *pEm)
{
	USHORT dist;
	SVECTOR Pad8;

	if (pEm->Routine_2 == 0) {
		pEm->Timer0 = 0x3c;
	}
	if (pEm->Timer0 != 0) {
		pEm->Timer0--;
	}
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 200) {
		SPL_SET_ROUTINE(pEm, 0x0b01);
	} else if ((pEm->Timer0 < 0x1e) && (dist > 1000)) {
		SPL_SET_ROUTINE(pEm, 0x0d01);
	} else {
		if ((SPL_FREE(pEm)->Dist > 700) && (G.Player.Spd_x > 100)) {
			SPL_SET_ROUTINE(pEm, 0x0d01);
		}
		if (pEm->Timer0 == 0) {
			SPL_SET_ROUTINE(pEm, 0x0d01);
		}
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x30);
	}
}

/* DS 0x80063b94 (Aida_mv0c_br) */
static VOID Aida_mv0c_br(register ENEMY_WORK *pEm)
{
	LONG goto_res;

	goto_res = Goto00_ck(pEm, SPL_FREE(pEm)->Goal_x, SPL_FREE(pEm)->Goal_z, 0x50);
	if (goto_res == 0) {
		SPL_SET_ROUTINE(pEm, 0x0101);
	} else {
		Goto00(pEm, SPL_FREE(pEm)->Goal_x, SPL_FREE(pEm)->Goal_z, 0xa0);
	}
}

/* DS 0x80063bec (Sherry_move) */
static VOID Sherry_move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	USHORT type;
	USHORT type_new;
	UCHAR next_timer;
	ULONG type_bits;
	USHORT flg;

	Spl_enemy_ck(pEm);
	next_timer = (UCHAR)(Spl_pos_ck_timer - 1);
	if (Spl_pos_ck_timer == 0) {
		type = pEm->Type;
		pEm->Type = type & 0xff00;
		type_bits = Spl_em_pos_ck(pEm, 3000);
		flg = SPL_FREE(pEm)->Flg;
		pEm->Type = pEm->Type | (USHORT)type_bits;
		type_new = pEm->Type;
		SPL_FREE(pEm)->Flg = flg & 0x7fff;
		if ((type & 0xf) != (type_new & 0xf)) {
			type = pEm->Type;
			flg &= 0x7ffc;
			SPL_FREE(pEm)->Flg = flg;
			SPL_FREE(pEm)->Flg = flg | Sherry_St_pos[type & 0xf] | 0x8000;
		}
		next_timer = 0xf;
	}
	Spl_pos_ck_timer = next_timer;
	((CODE_PTR)(Sherry_Move_type[pEm->Type & 0xf]))(pEm, Kan, Seq);
}

/* DS 0x80063cec (Sherry_Move_type00) */
static VOID Sherry_Move_type00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type00[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x80063d8c (Sherry_Move_type01) */
static VOID Sherry_Move_type01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type01[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x80063e2c (Sherry_Move_type02) */
static VOID Sherry_Move_type02(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type02[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x80063ecc (Sherry_Move_type03) */
static VOID Sherry_Move_type03(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type03[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x80063f6c (Sherry_Move_type04) */
static VOID Sherry_Move_type04(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type04[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x8006400c (Sherry_Move_type05) */
static VOID Sherry_Move_type05(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type05[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x800640ac (Sherry_Move_type06) */
static VOID Sherry_Move_type06(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type06[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x8006414c (Sherry_Move_type07) */
static VOID Sherry_Move_type07(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type07[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x800641ec (Sherry_Move_type08) */
static VOID Sherry_Move_type08(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type08[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x8006428c (Sherry_Move_type09) */
static VOID Sherry_Move_type09(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type09[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x8006432c (Sherry_Move_type0a) */
static VOID Sherry_Move_type0a(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Sherry_mv_br_type0a[pEm->Routine_1]))(pEm);
	if (pEm->Routine_1 == 0) {
		Sherry_mv00(pEm, Kan, Seq);
	} else {
		((CODE_PTR)(Aida_mv_type00[pEm->Routine_1]))(pEm, Kan, Seq);
	}
}

/* DS 0x800643cc (Sherry_mv00_br) */
static VOID Sherry_mv00_br(register ENEMY_WORK *pEm)
{
	USHORT flg;
	USHORT dist;
	ULONG flg_copy;
	ENEMY_WORK *pNear0;
	USHORT em_l0;
	LONG goto_res;
	ULONG routine;
	UCHAR routine2_cur;
	INT life_shift;
	INT life_max;
	INT life_rate;

	em_l0 = Em_l[0];
	pNear0 = pSpl_near_em[0];
	goto_res = Goto00_ck(pEm, SPL_FREE(pEm)->Goal_x, SPL_FREE(pEm)->Goal_z, 0x400);
	if (pEm->Routine_2 == 0) {
		pEm->D_life_u = 0;
		life_max = pEm->Max_life;
		life_shift = pEm->Life << 12;
		DIV_S(life_rate, life_shift, life_max);	/* DS: div with break checks */
		if ((G.SaveData.Dummy1[1] & 1) != 0) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 2000) {
			pEm->D_life_u = 1;
		}
		if (life_rate < 800) {
			pEm->D_life_u = 2;
		}
	}
	flg = SPL_FREE(pEm)->Flg;
	if ((flg & 0x2000) != 0) {
		if ((pNear0 != NULL) && (em_l0 < 6000)) {
			SPL_SET_ROUTINE(pEm, 0x0101);
		}
		if (SPL_FREE(pEm)->Dist > 2999) {
			return;
		}
		routine2_cur = pEm->Routine_2;
		if (routine2_cur == 5) {
			pEm->Routine_2 = 6;
			routine2_cur = pEm->Routine_2;
		}
		if (routine2_cur != 1) {
			return;
		}
		flg_copy = SPL_FREE(pEm)->Flg;
		pEm->Routine_2 = 0;
		SPL_FREE(pEm)->Flg = (USHORT)flg_copy & 0xdfff;
		return;
	}
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 8000) {
		if (dist > 0x5dc) {
			routine = 0x0c01;
			if (goto_res == 0) {
				routine = 0x0101;
			}
			SPL_SET_ROUTINE(pEm, routine);
			return;
		}
		if (dist > 2000) {
			routine = 0x0d01;
			if (goto_res == 0) {
				routine = 0x0201;
			}
			SPL_SET_ROUTINE(pEm, routine);
			return;
		}
	} else {
		SPL_FREE(pEm)->Flg = flg | 0x2000;
		flg_copy = SPL_FREE(pEm)->Flg;
		pEm->Timer0 = 0;
		SPL_FREE(pEm)->Flg = (USHORT)flg_copy & 0xefff;
	}
	if ((SPL_FREE(pEm)->Dist < 200) && (pEm->Routine_2 < 2)) {
		SPL_SET_ROUTINE(pEm, 0x00020001);
	}
	if (SPL_FREE(pEm)->Dist < 0x191) {
		return;
	}
	routine = 1;
	if (pEm->Routine_2 < 2) {
		return;
	}
	SPL_SET_ROUTINE(pEm, routine);
}

/* DS 0x800645d8 (Sherry_mv00) */
static VOID Sherry_mv00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	USHORT u_work;
	LONG goto_res;
	UCHAR neck_flg;
	ULONG res;
	USHORT Mot[3];

	memcpy(Mot, Spl_mv_taiki_rom, sizeof(Mot));
	switch (pEm->Routine_2) {
	case 0:
		u_work = Mot[(signed char)pEm->D_life_u];
		pEm->Routine_2 = 1;
		pEm->Neck_flg &= 0xed;
		res = SPL_FREE(pEm)->Flg;
		SPL_SET_MOVE(pEm, (UINT)u_work + 0x70000);
		if ((res & 0x2000) != 0) {
			SPL_SET_MOVE(pEm, 0x70006);
			pEm->Routine_2 = 4;
			*(ULONG *)&pEm->Spd_x = 0;
			pEm->Neck_flg |= 0x12;
			if (pEm->Water == 0) {
				return;
			}
			if (pEm->Ground - 500 <= (INT)pEm->Water) {
				return;
			}
			u_work = Mot[(signed char)pEm->D_life_u];
			pEm->Routine_2 = 1;
			SPL_SET_MOVE(pEm, (UINT)u_work + 0x70000);
			return;
		}
		/* fall through */
	case 1:
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		break;
	case 2:
		goto_res = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1);
		SPL_SET_MOVE(pEm, 0x70009);
		if ((goto_res << 16) < 0) {
			SPL_SET_MOVE(pEm, 0x7000b);
		}
		pEm->Routine_2 = 3;
		pEm->Timer3 = 0;
		/* fall through */
	case 3:
		res = Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		if ((res != 0) && (pEm->Timer3 == 0)) {
			pEm->Timer3 = 1;
			pEm->Move_no++;
		}
		break;
	case 4:
		res = 0x200;
		res = Joint_move(pEm, Kan, Seq, res);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 5:
		u_work = SPL_FREE(pEm)->Dist;
		neck_flg = pEm->Neck_flg & 0xed;
		pEm->Neck_flg = neck_flg;
		if (u_work > 6000) {
			pEm->Neck_flg = neck_flg | 0x12;
		}
		SPL_SET_MOVE(pEm, 0x70006);
		break;
	case 6:
		res = 0x10200;
		res = Joint_move(pEm, Kan, Seq, res);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 7:
		res = SPL_FREE(pEm)->Flg;
		pEm->Routine_2 = 0;
		SPL_FREE(pEm)->Flg = (USHORT)res & 0xdfff;
		break;
	}
}

/* DS 0x80064814 (Sherry_mv01_br) */
static VOID Sherry_mv01_br(register ENEMY_WORK *pEm)
{
	USHORT dist;
	ULONG flg_copy;
	ENEMY_WORK *pNear0;
	USHORT em_l0;
	LONG rnd_a;
	LONG rnd_b;
	SHORT s_work;
	LONG tgt_x;
	LONG Vec_z;

	pNear0 = pSpl_near_em[0];
	em_l0 = Em_l[0];
	if ((pEm->Sca_info & 1) != 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = ((LONG (*)(VOID))Rnd)();
				rnd_b = ((LONG (*)(VOID))Rnd)();
				SPL_FREE(pEm)->Timer4 = 0x0f;
				s_work = ((rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880;
				SPL_FREE(pEm)->Turn_dir = pEm->Cdir_y - s_work;
				if ((pNear0 != NULL) && (em_l0 < 3000)) {
					s_work = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear0->Pos_x, pNear0->Pos_z);
					SPL_FREE(pEm)->Turn_dir = s_work;
					SPL_FREE(pEm)->Timer4 = 0x1e;
				}
				SPL_SET_ROUTINE(pEm, 0x0201);
				return;
			}
		}
	}
	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
		}
	}
	if ((SPL_FREE(pEm)->Flg & 0x2000) != 0) {
		if (pNear0 != NULL) {
			if (em_l0 < 5000) {
				SPL_SET_ROUTINE(pEm, 0x0201);
				return;
			}
			if (em_l0 > 7000) {
				SPL_SET_ROUTINE(pEm, 0x0001);
				return;
			}
			Goto00(pEm, pNear0->Pos_x, pNear0->Pos_z, -0x28);
		}
		if (SPL_FREE(pEm)->Dist < 4000) {
			SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xdfff;
		}
	} else {
		dist = SPL_FREE(pEm)->Dist;
		if (dist > 8000) {
			flg_copy = SPL_FREE(pEm)->Flg;
			pEm->Timer0 = 0;
			SPL_FREE(pEm)->Flg = (USHORT)flg_copy | 0x2000;
			flg_copy = SPL_FREE(pEm)->Flg;
			SPL_SET_ROUTINE(pEm, 0x0001);
			SPL_FREE(pEm)->Flg = (USHORT)flg_copy & 0xefff;
			return;
		}
		if (dist < 0x96) {
			SPL_SET_ROUTINE(pEm, 0x0b01);
			return;
		}
		if (dist < 0x7d1) {
			if ((dist < 2000) && (G.Player.Routine_1 == 2) && (Cdir_ck2(pEm, (ENEMY_WORK *)&G.Player) == 0)) {
				SPL_SET_ROUTINE(pEm, 0x0301);
				return;
			}
			if ((pNear0 == NULL) || (em_l0 > 0x5db) || ((tgt_x = Goto00_ck(pEm, pNear0->Pos_x, pNear0->Pos_z, 0x4b0)) != 0) || ((s_work = -0x28) == -0x28 && (pEm->Sca_info & 1) != 0)) {
				tgt_x = pEm->Dest_x;
				Vec_z = pEm->Dest_z;
				s_work = 0x28;
			} else {
				tgt_x = pNear0->Pos_x;
				Vec_z = pNear0->Pos_z;
			}
			Goto00(pEm, tgt_x, Vec_z, s_work);
			Oma_Spl_updown_ck(pEm);
			return;
		}
		SPL_SET_ROUTINE(pEm, 0x0201);
		return;
	}
}

/* DS 0x80064acc (Sherry_mv02_br) */
static VOID Sherry_mv02_br(register ENEMY_WORK *pEm)
{
	USHORT dist;
	ULONG flg_copy;
	ENEMY_WORK *pNear0;
	USHORT em_l0;
	LONG rnd_a;
	LONG rnd_b;
	SHORT s_work;
	LONG tgt_x;
	LONG Vec_z;
	LONG spd;

	pNear0 = pSpl_near_em[0];
	em_l0 = Em_l[0];
	if ((pEm->Sca_info & 1) != 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			s_work = SPL_FREE(pEm)->Timer4 + 1;
			SPL_FREE(pEm)->Timer4 = s_work;
			if (s_work > 0x3c) {
				SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg | 0x1000;
				rnd_a = ((LONG (*)(VOID))Rnd)();
				rnd_b = ((LONG (*)(VOID))Rnd)();
				SPL_FREE(pEm)->Timer4 = 0x0f;
				s_work = ((rnd_a >> (rnd_b & 3)) & 1) * 0x100 - 0x880;
				SPL_FREE(pEm)->Turn_dir = pEm->Cdir_y - s_work;
				if ((pNear0 != NULL) && (em_l0 < 3000)) {
					s_work = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear0->Pos_x, pNear0->Pos_z);
					SPL_FREE(pEm)->Turn_dir = s_work;
					SPL_FREE(pEm)->Timer4 = 0x1e;
				}
				SPL_SET_ROUTINE(pEm, 0x0201);
				return;
			}
		}
	}
	if ((pEm->Sca_info & 1) == 0) {
		if ((SPL_FREE(pEm)->Flg & 0x1000) == 0) {
			SPL_FREE(pEm)->Timer4 = 0;
		}
	}
	if ((SPL_FREE(pEm)->Flg & 0x1000) != 0) {
		if (SPL_FREE(pEm)->Timer4 != 0) {
			Goto01(pEm, SPL_FREE(pEm)->Turn_dir, 0x50);
			if (--SPL_FREE(pEm)->Timer4 != 0) {
				return;
			}
			SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xefff;
			return;
		}
	}
	if ((SPL_FREE(pEm)->Flg & 0x2000) != 0) {
		if (pNear0 != NULL) {
			if (em_l0 > 7000) {
				SPL_SET_ROUTINE(pEm, 0x0001);
				return;
			}
			Goto00(pEm, pNear0->Pos_x, pNear0->Pos_z, -0x38);
		}
		if (SPL_FREE(pEm)->Dist < 3000) {
			SPL_FREE(pEm)->Flg = SPL_FREE(pEm)->Flg & 0xdfff;
		}
	} else {
		dist = SPL_FREE(pEm)->Dist;
		if (dist > 0x1f40) {
			flg_copy = SPL_FREE(pEm)->Flg;
			pEm->Timer0 = 0;
			SPL_FREE(pEm)->Flg = (USHORT)flg_copy | 0x2000;
			flg_copy = SPL_FREE(pEm)->Flg;
			SPL_SET_ROUTINE(pEm, 0x0001);
			SPL_FREE(pEm)->Flg = (USHORT)flg_copy & 0xefff;
		} else {
			if (dist < 1000) {
				SPL_SET_ROUTINE(pEm, 0x0101);
			} else {
				if ((pNear0 != NULL) && (em_l0 < 2000) && (Goto00_ck(pEm, pNear0->Pos_x, pNear0->Pos_z, 0x400) == 0) && ((pEm->Sca_info & 1) == 0)) {
					tgt_x = pNear0->Pos_x;
					Vec_z = pNear0->Pos_z;
					spd = -0x48;
				} else {
					tgt_x = pEm->Dest_x;
					Vec_z = pEm->Dest_z;
					spd = 0x48;
				}
				Goto00(pEm, tgt_x, Vec_z, spd);
				Oma_Spl_updown_ck(pEm);
			}
		}
	}
}

/* DS 0x80064d90 (Spl_nop_br): retail empty routine (jr ra) */
static VOID Spl_nop_br(VOID)
{
}

/* DS 0x80064d98 (Sherry_mv06_br) */
static VOID Sherry_mv06_br(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG res;

	switch (pEm->Routine_2) {
	case 0:
		SPL_SET_MOVE(pEm, 0x70006);
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;
		/* fall through */
	case 1:
		res = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 2:
		pEm->Routine_2 = 4;
		break;
	case 4:
		SPL_SET_MOVE(pEm, 0x70006);
		pEm->Routine_2 = 5;
		/* fall through */
	case 5:
		res = Joint_move(pEm, Kan, Seq, 0x10200);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 6:
		SPL_SET_ROUTINE(pEm, 0x0001);
		break;
	}
}

/* DS 0x80064e5c (Sherry_mv06) */
static VOID Sherry_mv06(register ENEMY_WORK *pEm)
{
	/* DS keeps the address of G.Player.Cdir_y in s1 (DS 0x80064e6c: lui s1,0x800d; addiu s1,s1,-0x392) and
	   reads it with lh a1,0x0(s1) before each of the two calls, so the direction is read through a pointer
	   local. The Oct 97 mockup declares no locals for Sherry_mv06. */
	register SHORT *pDir = &G.Player.Cdir_y;

	if (Goto01_ck(pEm, *pDir, 0x50) == 0) {
		SPL_SET_ROUTINE(pEm, 0x0001);
	} else {
		Goto01(pEm, *pDir, 0xa0);
	}
}

/* DS 0x80064ebc (Sherry_mv0b_br) */
static VOID Sherry_mv0b_br(register ENEMY_WORK *pEm)
{
	USHORT dist;
	USHORT em_l0;
	ENEMY_WORK *pNear0;

	pNear0 = pSpl_near_em[0];
	em_l0 = Em_l[0];
	if (pEm->Routine_2 == 0) {
		pEm->Timer0 = 0x32;
	}
	if (pEm->Timer0 != 0) {
		pEm->Timer0--;
	}
	dist = SPL_FREE(pEm)->Dist;
	if (dist < 100) {
		SPL_SET_ROUTINE(pEm, 0x0b01);
	} else if ((pEm->Timer0 < 0x1e) && (dist > 1000)) {
		SPL_SET_ROUTINE(pEm, 0x0d01);
	} else {
		if (pEm->Timer0 == 0) {
			SPL_SET_ROUTINE(pEm, 0x0d01);
		}
		/* Near enemy within 1500 (Em_l[0], read at entry) and Sca_info bit 0 clear: head for the near
		 * enemy X with own Z, turn step 0x30. Otherwise head for Dest_x/Dest_z with turn step -0x30.
		 * DS shares one jal for both calls (a3 = 0x30 set in the delay slot of the Sca_info test). */
		if ((em_l0 < 0x5dc) && ((pEm->Sca_info & 1) == 0)) {
			Goto00(pEm, pNear0->Pos_x, pEm->Pos_z, 0x30);
		} else {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x30);
		}
	}
}

/* DS 0x80064fa8 (Spl_dm00) */
VOID Spl_dm00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	ULONG res;

	switch (pEm->Routine_2) {
	case 0:
		SPL_SET_MOVE(pEm, 0x30005);
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;
		Snd_se_enem(0, pEm);
		pEm->Be_flg &= 0xfffffffb;
	case 1:
		res = Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
		break;
	case 2:
		SPL_SET_ROUTINE(pEm, 0x0001);
		pEm->Damage_cnt = 0;
		pEm->Neck_flg &= 0xed;
		break;
	}
}

/* DS 0x800650a8 (Spl_dm01) */
VOID Spl_dm01(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR routine2_cur;
	ULONG res;
	LONG v;

	routine2_cur = pEm->Routine_2;
	if (routine2_cur != 1) {
		if (routine2_cur > 1) {
			if (routine2_cur != 2) {
				return;
			}
			SPL_SET_ROUTINE(pEm, 0x0001);
			pEm->Damage_cnt = 0;
			pEm->Neck_flg &= 0xed;
			return;
		}
		if (routine2_cur != 0) {
			return;
		}
		pEm->Routine_2 = 1;
		pEm->Spd_z = 0;
		SPL_SET_MOVE(pEm, (UINT)((pEm->Routine_1 & 1) + 0x30003));
		v = (pEm->Routine_1 & 1) * 400 - 200;
		pEm->Spd_x = (SHORT)v;
		pEm->Spd_y = (SHORT)(v >> 16);
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;
		Snd_se_enem(pEm->Routine_2 - 1, pEm);
		pEm->Be_flg &= 0xfffffffb;
	}
	if (pEm->Hokan_flg != 0) {
		Add_speedXZ(pEm, 0);
	}
	res = Joint_move(pEm, Kan, Seq, 0x400);
	pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
}

/* DS 0x800651f0 (Spl_die00) */
VOID Spl_die00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	UCHAR routine2_cur;
	ULONG res;

	routine2_cur = pEm->Routine_2;
	if (routine2_cur != 1) {
		if (routine2_cur > 1) {
			if (routine2_cur == 2) {
				SPL_SET_ROUTINE(pEm, 0x0007);
			}
			goto foot_set;
		}
		if (routine2_cur != 0) {
			goto foot_set;
		}
		SPL_SET_MOVE(pEm, 0x70002);
		pEm->Routine_2 = 1;
		*(ULONG *)&pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		pEm->Neck_flg = 1;
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(3, pEm);
		G.System_flg |= SYS_GAME_OVER;
	}
	res = Joint_move(pEm, Kan, Seq, 0x200);
	pEm->Routine_2 = pEm->Routine_2 + (CHAR)res;
foot_set:
	if (pEm->Hokan_flg == 0) {
		Foot_set_pl(pEm, 0, 1);
	}
}

/* DS 0x800652fc (Aida_before) */
VOID Aida_before(register ENEMY_WORK *pEm)
{
	Aida_Jh_move(pEm);
}

/* DS 0x8006531c (Aida_after) */
VOID Aida_after(register ENEMY_WORK *pEm)
{
}

/* DS 0x80065324 (Spl_soba_pos_set) */
ULONG Spl_soba_pos_set(register SHORT ll, register SHORT dir, register SHORT dir2, register SVECTOR *pPos)
{
	INT dx;
	INT dz;
	UINT dist1;
	UINT dist2;
	MATRIX rot_mat;
	SVECTOR ang;
	SVECTOR ofs;
	SVECTOR pos1;
	SVECTOR pos2;

	ang.vz = 0;
	ang.vx = 0;
	ang.vy = G.Player.Cdir_y + dir;
	RotMatrix(&ang, &rot_mat);
	ofs.vz = 0;
	ofs.vy = 0;
	ofs.vx = ll;
	ApplyMatrixSV(&rot_mat, &ofs, &pos1);
	dx = (INT)(((G.pEm)->Pos_x - ((UINT)(USHORT)pos1.vx + G.Player.Pos_x)) * 0x10000) >> 16;
	dz = (INT)(((G.pEm)->Pos_z - ((UINT)(USHORT)pos1.vz + G.Player.Pos_z)) * 0x10000) >> 16;
	dist1 = SquareRoot0(dx * dx + dz * dz);
	ang.vy = G.Player.Cdir_y + dir2;
	RotMatrix(&ang, &rot_mat);
	ApplyMatrixSV(&rot_mat, &ofs, &pos2);
	dx = (INT)(((G.pEm)->Pos_x - ((UINT)(USHORT)pos2.vx + G.Player.Pos_x)) * 0x10000) >> 16;
	dz = (INT)(((G.pEm)->Pos_z - ((UINT)(USHORT)pos2.vz + G.Player.Pos_z)) * 0x10000) >> 16;
	dist2 = SquareRoot0(dx * dx + dz * dz);
	pPos->vx = pos1.vx + (SHORT)G.Player.Pos_x;
	pPos->vz = pos1.vz + (SHORT)G.Player.Pos_z;
	if (dist2 < dist1) {
		pPos->vx = pos2.vx + (SHORT)G.Player.Pos_x;
		pPos->vz = pos2.vz + (SHORT)G.Player.Pos_z;
		dist1 = dist2;
	}
	return dist1;
}

/* DS 0x80065518 (Spl_enemy_ck) */
VOID Spl_enemy_ck(register ENEMY_WORK *pEm)
{
	INT dx;
	INT dz;
	UINT dist;
	ULONG res;
	ENEMY_WORK *pCur;
	ENEMY_WORK **ppEnemy;
	UINT d_near1;
	UINT d_next;
	UINT d_near2;
	UINT d_near0;
	/* DS initialises pNear from a rodata template (3 words copied to the stack) */
	ENEMY_WORK *pNear[3] = { NULL, NULL, NULL };

	d_near0 = 0x7fffffff;
	pSpl_near_em[0] = NULL;
	pSpl_near_em[1] = NULL;
	pSpl_near_em[2] = NULL;
	d_near2 = 0x7fffffff;
	if (G.Enemy_num != 0) {
		ppEnemy = G.pEnemy + 2;
		d_near1 = 0x7fffffff;
		do {
			pCur = *ppEnemy;
			d_next = d_near1;
			if (((pCur->Be_flg & 1) != 0) && ((pCur->Type & 0xc000) == 0)) {
				dx = pCur->Pos_x - pEm->Pos_x;
				dz = pCur->Pos_z - pEm->Pos_z;
				dist = SquareRoot0(dx * dx + dz * dz);
				pCur->L_spl = dist;
				res = Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t, (VECTOR *)pCur->pSin_parts_ptr[pCur->Neck_no].Workm.t, 0x2080, 1);
				if ((res == 0) && (pCur->Life >= 0)) {
					if (dist < d_near0) {
						pNear[2] = pNear[1];
						pNear[1] = pNear[0];
						d_next = d_near0;
						d_near2 = d_near1;
						d_near0 = dist;
						pNear[0] = pCur;
					} else if (dist < d_near1) {
						pNear[2] = pNear[1];
						d_next = dist;
						d_near2 = d_near1;
						pNear[1] = pCur;
					} else if (dist < d_near2) {
						d_near2 = dist;
						pNear[2] = pCur;
					}
				}
			}
			ppEnemy++;
			d_near1 = d_next;
		} while (ppEnemy != G.ppEnemy_end);
		Em_l[0] = (USHORT)d_near0;
		Em_l[1] = (USHORT)d_next;
		Em_l[2] = (USHORT)d_near2;
		pSpl_near_em[0] = pNear[0];
		pSpl_near_em[1] = pNear[1];
		pSpl_near_em[2] = pNear[2];
		if (pNear[0] != NULL) {
			Em_dir[0] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[0]->Pos_x, pNear[0]->Pos_z);
			pNear[0]->L_spl |= 0x40000000;
			pNear[0]->Dir_spl = (SHORT)((Em_dir[0] + 0x800) & 0xfff);
		}
		if (pNear[1] != NULL) {
			Em_dir[1] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[1]->Pos_x, pNear[1]->Pos_z);
			pNear[1]->L_spl |= 0x80000000;
			pNear[1]->Dir_spl = (SHORT)((Em_dir[1] + 0x800) & 0xfff);
		}
		if (pNear[2] != NULL) {
			Em_dir[2] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[2]->Pos_x, pNear[2]->Pos_z);
			pNear[2]->L_spl |= 0xc0000000;
			pNear[2]->Dir_spl = (SHORT)((Em_dir[2] + 0x800) & 0xfff);
		}
	}
}

/* DS 0x80065890 (Spl_em_pos_ck) */
ULONG Spl_em_pos_ck(register ENEMY_WORK *pEm, register ULONG Lim)
{
	LONG goto_res;
	UINT bit;
	ENEMY_WORK *pNear;
	UINT ret_bits;
	INT idx;
	UINT cnt;
	UINT near_cnt;
	ENEMY_WORK **ppNear;
	USHORT *pDist;
	UINT lim_cnt;

	ret_bits = 0;
	near_cnt = 0;
	idx = 3;
	ppNear = pSpl_near_em + 2;
	do {
		if (*ppNear != NULL) {
			near_cnt++;
		}
		idx--;
		ppNear--;
	} while (idx != 0);
	lim_cnt = 0;
	if (near_cnt != 0) {
		pDist = Em_l;
		ppNear = pSpl_near_em;
		cnt = near_cnt;
		do {
			cnt--;
			pNear = *ppNear;
			if (*pDist < Lim) {
				goto_res = Goto00_ck(pEm, pNear->Pos_x, pNear->Pos_z, 0x400);
				bit = 0x20000;
				if (goto_res == 0) {
					bit = 0x10000;
				}
				ret_bits |= bit;
				lim_cnt++;
			}
			goto_res = Goto00_ck((ENEMY_WORK *)&G.Player, pNear->Pos_x, pNear->Pos_z, 0x400);
			if (goto_res == 0) {
				ret_bits |= 1;
			} else {
				ret_bits |= 2;
			}
			pDist++;
			ppNear++;
		} while (cnt != 0);
		if ((ret_bits & 0xffff0000) == 0) {
			if (near_cnt < 2) {
				return ret_bits * 2;
			}
			return ret_bits * 2 + 4;
		}
		if (lim_cnt > 1) {
			return (ret_bits >> 15) + 3;
		}
		return (ret_bits >> 15) - 1;
	}
	return 0;
}

/* DS 0x80065a0c (Spl_em_cdir_ck) */
ULONG Spl_em_cdir_ck(register ENEMY_WORK *pEm)
{
	/* DS keeps both 0x7fffffff limits in saved registers (s6, s8) and compares the USHORT distance against them
	   with sltu; a literal compare would be folded away by CCPSX. */
	register UINT Ok_lim = 0x7fffffff;
	register UINT Ng_lim = 0x7fffffff;
	LONG goto_res;
	ENEMY_WORK *pNear;
	USHORT *pDist;
	ENEMY_WORK **ppNear;
	INT idx;
	ENEMY_WORK *pCdirOk;
	ENEMY_WORK *pCdirNg;

	pCdirOk = NULL;
	pCdirNg = NULL;
	idx = 2;
	pDist = Em_l;
	ppNear = pSpl_near_em;
	/* Up to three near enemies (idx 2, 1, 0), stopping at the first NULL slot. DS tests the slot at the top
	   of the loop and the counter at the bottom (move v0,s3 / bne / addiu s3,-1); writing the NULL test
	   as an if with an else-break keeps CCPSX from rotating the slot load into the loop tail. */
	do {
		pNear = *ppNear;
		if (pNear != NULL) {
			goto_res = Goto00_ck(pEm, pNear->Pos_x, pNear->Pos_z, 0x400);
			if (goto_res == 0) {
				if (*pDist < Ok_lim) {
					pCdirOk = pNear;
				}
			} else if (*pDist < Ng_lim) {
				pCdirNg = pNear;
			}
			pDist++;
			ppNear++;
		} else {
			break;
		}
	} while (idx-- != 0);
	/* The clear-path (Ok) enemy wins; otherwise the blocked (Ng) one, which may be NULL. */
	if (pCdirOk != NULL) {
		return (ULONG)pCdirOk;
	}
	return (ULONG)pCdirNg;
}

/* DS 0x80065b18 (Spl_pllock_get) */
ULONG Spl_pllock_get(register ENEMY_WORK *pEm, register ENEMY_WORK *pYo)
{
	ULONG found_pl;
	ENEMY_WORK *pNear;
	ENEMY_WORK **ppNear;
	INT idx;
	ENEMY_WORK *pRet;
	ULONG **ppPl;		/* address of G.Player.pEnemy_ptr, kept for the loop (DS holds it in t1) */

	found_pl = 0;
	pRet = NULL;
	idx = 2;
	ppPl = &G.Player.pEnemy_ptr;
	ppNear = pSpl_near_em;
	do {
		pNear = *ppNear;
		if (pNear != NULL) {
			if ((ENEMY_WORK *)*ppPl == pNear) {
				ppNear++;
				found_pl = 1;
				continue;
			} else if (pYo != pNear) {
				pRet = pNear;
			}
		}
		ppNear++;
	} while (idx-- != 0);
	if (pRet != NULL) {
		return (ULONG)pRet;
	}
	if (found_pl == 0) {
		return 0;
	}
	return (ULONG)G.Player.pEnemy_ptr;
}

/* DS 0x80065b9c (Spl_life_down) */
ULONG Spl_life_down(register ENEMY_WORK *pEm, register LONG Down_life, register ULONG Mode)
{
	ENEMY_WORK *pT;

	pT = G.pEnemy[1];
	if (pT->Damage_cnt != 0) {
		return pT->Damage_cnt;
	}
	pT->Life -= Down_life;
	G.SaveData.Dummy[0] -= Down_life;
	SPL_SET_ROUTINE(pT, 0x0002);
	if (Mode & 2) {
		pT->Routine_1 = 2;
	}
	pT->Routine_1 += Cdir_ck(pEm, pT);
	if (Mode & 1) {
		if (pT->Life < 0) {
			pT->Life = 0;
			G.SaveData.Dummy[0] = 0;
		}
		return 0;
	}
	/* DS has no return value on the path where Mode is non-zero with bit 0 clear: the branch to the
	   epilogue (bne s1,zero) carries a nop in its delay slot, so v0 still holds Mode & 1 from the test
	   above, which is 0 on this path. The function therefore ends here without a return statement. */
	if (Mode == 0) {
		if (pT->Life < 0) {
			SPL_SET_ROUTINE(pT, 0x0003);
			return 2;
		}
		return 0;
	}
}

/* DS 0x8001195c: motion table, 3 entries */
const USHORT Spl_mv_taiki_rom[3] = {
	2, 5, 8
};

/* DS 0x80011964: motion table, 3 entries */
const SHORT Spl_mv01_rom[3] = {
	0, 3, 6
};

/* DS 0x8001196c: motion table, 3 entries */
const UCHAR Spl_mv02_rom[3] = {
	1, 4, 7
};

/* DS 0x80011970: motion table, 3 entries */
const UCHAR Spl_mv03_rom[3] = {
	0, 9, 9
};

/* DS 0x80011974: motion table, 5 entries */
const SHORT Spl_mv05_t1_rom[5] = {
	0, 15, 11, 0, 13
};

/* DS 0x80011980: motion table, 5 entries */
const SHORT Spl_mv05_t2_rom[5] = {
	0, 14, 10, 0, 12
};

/* DS 0x800a8624 */
SHORT Spl_r_pos[2][6] = {
	{ 1000, 1300, -1300, 800, 1920, 1920 },
	{ 700, 1100, -1100, 600, 2176, 1920 }
};

/* ---- function-pointer dispatch tables (defined from retail data) ---- */
/* DS 0x800a863c: 11 handler pointers (0x2c bytes, up to the Aida_St_pos label at 0x800a8668). Aida_move indexes
 * the table with Type & 0xf without a bound check, and retail has nothing valid after entry 10 (the bytes there
 * are the 0x02000000 word and the Aida_St_pos data), so the declared extent is 11. */
VOID *Aida_Move_type[11] = {
	(VOID *)Aida_Move_type00,	/* 0: DS 0x80060750 */
	(VOID *)Aida_Move_type01,	/* 1: DS 0x800607d4 */
	(VOID *)Aida_Move_type02,	/* 2: DS 0x80060858 */
	(VOID *)Aida_Move_type03,	/* 3: DS 0x800608dc */
	(VOID *)Aida_Move_type04,	/* 4: DS 0x80060960 */
	(VOID *)Aida_Move_type05,	/* 5: DS 0x800609e4 */
	(VOID *)Aida_Move_type06,	/* 6: DS 0x80060a68 */
	(VOID *)Aida_Move_type07,	/* 7: DS 0x80060aec */
	(VOID *)Aida_Move_type08,	/* 8: DS 0x80060b70 */
	(VOID *)Aida_Move_type09,	/* 9: DS 0x80060bf4 */
	(VOID *)Aida_Move_type0a,	/* 10: DS 0x80060c78 */
};

/* DS 0x800a8668: 11 one-byte start states, read with lbu and OR'd into the sprite flag. Retail has no
 * valid entry past index 10; the bytes after the table belong to the next data. */
UCHAR Aida_St_pos[11] = {
	0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,
};

static UCHAR Spl_pos_ck_timer;

/* DS 0x800a8674 */
VOID *Aida_mv_br_type00[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br_1,	/* 1: DS 0x800619e4 */
	(VOID *)Aida_mv02_br_1,	/* 2: DS 0x80061f68 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0a,	/* 11: DS 0x800638d0 */
	(VOID *)Aida_mv0b,	/* 12: DS 0x80063abc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a86ac */
VOID *Aida_mv_type00[] = {
	(VOID *)Aida_mv00,	/* 0: DS 0x800614f8 */
	(VOID *)Aida_mv01,	/* 1: DS 0x80061bbc */
	(VOID *)Aida_mv02,	/* 2: DS 0x800620ec */
	(VOID *)Aida_mv03,	/* 3: DS 0x80062318 */
	(VOID *)Aida_mv04,	/* 4: DS 0x800624f8 */
	(VOID *)Aida_mv05,	/* 5: DS 0x80062618 */
	(VOID *)Aida_mv06,	/* 6: DS 0x80062dcc */
	(VOID *)Aida_mv07,	/* 7: DS 0x80062e98 */
	(VOID *)Aida_mv08,	/* 8: DS 0x8006316c */
	(VOID *)Aida_mv09,	/* 9: DS 0x80063524 */
	(VOID *)Aida_mv_nop_br,	/* 10: DS 0x800638c8 */
	(VOID *)Aida_mv04,	/* 11: DS 0x800624f8 */
	(VOID *)Aida_mv03,	/* 12: DS 0x80062318 */
	(VOID *)Aida_mv04,	/* 13: DS 0x800624f8 */
};

/* DS 0x800a86e4 */
VOID *Aida_mv_br_type01[] = {
	(VOID *)Aida_mv00_br_1,	/* 0: DS 0x800613c0 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0a,	/* 11: DS 0x800638d0 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a871c */
VOID *Aida_mv_br_type02[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0a,	/* 11: DS 0x800638d0 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8754 */
VOID *Aida_mv_br_type03[] = {
	(VOID *)Aida_mv00_br_1,	/* 0: DS 0x800613c0 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a878c */
VOID *Aida_mv_br_type04[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a87c4 */
VOID *Aida_mv_br_type05[] = {
	(VOID *)Aida_mv00_br_1,	/* 0: DS 0x800613c0 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0a,	/* 11: DS 0x800638d0 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a87fc */
VOID *Aida_mv_br_type06[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0a,	/* 11: DS 0x800638d0 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8834 */
VOID *Aida_mv_br_type07[] = {
	(VOID *)Aida_mv00_br_1,	/* 0: DS 0x800613c0 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a886c */
VOID *Aida_mv_br_type08[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a88a4 */
VOID *Aida_mv_br_type09[] = {
	(VOID *)Aida_mv00_br_1,	/* 0: DS 0x800613c0 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a88dc */
VOID *Aida_mv_br_type0a[] = {
	(VOID *)Aida_mv00_br,	/* 0: DS 0x80061150 */
	(VOID *)Aida_mv01_br,	/* 1: DS 0x80061714 */
	(VOID *)Aida_mv02_br,	/* 2: DS 0x80061d90 */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Aida_mv06_br,	/* 6: DS 0x80062dc4 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Aida_mv0b_br,	/* 11: DS 0x80063930 */
	(VOID *)Aida_mv0b_br_1,	/* 12: DS 0x800639a0 */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8914 (E_A_at_tbl): Aida attack selector per enemy, indexed by enemy Id - 0x10.
 * 0 = em10 handler, 1 = em25 (Id 0x25), 2 = em26 (Id 0x26). Retail reads it as byte 0x800a8904 + Id,
 * folding the -0x10 into the address. It sits right after Aida_mv_br_type0a and before Aida_em_at00_pl. */
UCHAR E_A_at_tbl[48] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* DS 0x800a8944 */
VOID *Aida_em_at00_pl[] = {
	(VOID *)Aida_em10_at00_pl,	/* 0: DS 0x80060cfc */
	(VOID *)Aida_em25_at00_pl,	/* 1: DS 0x80060ee4 */
	(VOID *)Aida_em26_at00_pl,	/* 2: DS 0x80061140 */
};

/* DS 0x800a8950 */
VOID *Aida_em_at00_spl[] = {
	(VOID *)Aida_em10_at00_spl,	/* 0: DS 0x80060e08 */
	(VOID *)Aida_em25_at00_spl,	/* 1: DS 0x80060fec */
	(VOID *)Aida_em26_at00_spl,	/* 2: DS 0x80061148 */
};

/* DS 0x800a895c */
VOID *Aida_em_at01[] = {
	(VOID *)Aida_em10_at01,	/* 0: DS 0x80061594 */
	(VOID *)Aida_em25_at01,	/* 1: DS 0x80061654 */
	(VOID *)Aida_em26_at01,	/* 2: DS 0x8006170c */
};

/* DS 0x800a8968 */
VOID *Aida_mv05_w1_br_tbl[] = {
	(VOID *)Aida_mv05_w1_t0_br,	/* 0: DS 0x800626b4 */
	(VOID *)Aida_mv05_w1_t1_br,	/* 1: DS 0x800628c4 */
	(VOID *)Aida_mv05_w1_t2_br,	/* 2: DS 0x80062a20 */
	(VOID *)Aida_mv05_w1_t3_br,	/* 3: DS 0x80062c44 */
	(VOID *)Aida_mv05_w1_t4_br,	/* 4: DS 0x80062cc0 */
	(VOID *)Aida_mv05_w1_t5_br,	/* 5: DS 0x80062d24 */
};

/* DS 0x800a8980 */
VOID *Aida_mv05_w1_tbl[] = {
	(VOID *)Aida_mv05_w1_t0,	/* 0: DS 0x800626bc */
	(VOID *)Aida_mv05_w1_t1,	/* 1: DS 0x80062954 */
	(VOID *)Aida_mv05_w1_t2,	/* 2: DS 0x80062a28 */
	(VOID *)Aida_mv05_w1_t3,	/* 3: DS 0x80062c4c */
	(VOID *)Aida_mv05_w1_t4,	/* 4: DS 0x80062cc8 */
	(VOID *)Aida_mv05_w1_t5,	/* 5: DS 0x80062d2c */
};

/* DS 0x800a8998 */
VOID *Aida_em_at05[] = {
	(VOID *)Aida_em10_at05,	/* 0: DS 0x800627dc */
	(VOID *)Aida_em10_at05,	/* 1: DS 0x800627dc */
};

/* DS 0x800a89a0: 11 handler pointers (0x2c bytes, up to the Sherry_St_pos label at 0x800a89cc). Sherry_move
 * indexes the table with Type & 0xf without a bound check; retail bytes after entry 10 are the 0x00020200
 * word and then the Sherry_St_pos data, so the declared extent is 11. */
VOID *Sherry_Move_type[11] = {
	(VOID *)Sherry_Move_type00,	/* 0: DS 0x80063cec */
	(VOID *)Sherry_Move_type01,	/* 1: DS 0x80063d8c */
	(VOID *)Sherry_Move_type02,	/* 2: DS 0x80063e2c */
	(VOID *)Sherry_Move_type03,	/* 3: DS 0x80063ecc */
	(VOID *)Sherry_Move_type04,	/* 4: DS 0x80063f6c */
	(VOID *)Sherry_Move_type05,	/* 5: DS 0x8006400c */
	(VOID *)Sherry_Move_type06,	/* 6: DS 0x800640ac */
	(VOID *)Sherry_Move_type07,	/* 7: DS 0x8006414c */
	(VOID *)Sherry_Move_type08,	/* 8: DS 0x800641ec */
	(VOID *)Sherry_Move_type09,	/* 9: DS 0x8006428c */
	(VOID *)Sherry_Move_type0a,	/* 10: DS 0x8006432c */
};

/* DS 0x800a89cc */
SHORT Sherry_St_pos[] = {
	512, 2, 512, 2, 512, 2
};

/* DS 0x800a89d8 */
VOID *Sherry_mv_br_type00[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8a10 */
VOID *Sherry_mv_br_type01[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8a48 */
VOID *Sherry_mv_br_type02[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8a80 */
VOID *Sherry_mv_br_type03[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8ab8 */
VOID *Sherry_mv_br_type04[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8af0 */
VOID *Sherry_mv_br_type05[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8b28 */
VOID *Sherry_mv_br_type06[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8b60 */
VOID *Sherry_mv_br_type07[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8b98 */
VOID *Sherry_mv_br_type08[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8bd0 */
VOID *Sherry_mv_br_type09[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800a8c08 */
VOID *Sherry_mv_br_type0a[] = {
	(VOID *)Sherry_mv00_br,	/* 0: DS 0x800643cc */
	(VOID *)Sherry_mv01_br,	/* 1: DS 0x80064814 */
	(VOID *)Sherry_mv02_br,	/* 2: DS 0x80064acc */
	(VOID *)Aida_mv03_br,	/* 3: DS 0x80062248 */
	(VOID *)Aida_mv04_br,	/* 4: DS 0x80062490 */
	(VOID *)Aida_mv05_br,	/* 5: DS 0x800625dc */
	(VOID *)Spl_nop_br,	/* 6: DS 0x80064d90 */
	(VOID *)Aida_mv07_br,	/* 7: DS 0x80062e90 */
	(VOID *)Aida_mv08_br,	/* 8: DS 0x80063164 */
	(VOID *)Aida_mv09_br,	/* 9: DS 0x8006351c */
	(VOID *)Aida_mv0a_br,	/* 10: DS 0x800638c0 */
	(VOID *)Sherry_mv06,	/* 11: DS 0x80064e5c */
	(VOID *)Sherry_mv0b_br,	/* 12: DS 0x80064ebc */
	(VOID *)Aida_mv0c_br,	/* 13: DS 0x80063b94 */
};

/* DS 0x800d4c70 */
SHORT Em_dir[3];

/* DS 0x800d7648 */
ENEMY_WORK *pSpl_near_em[3];

/* DS 0x800dfad8 */
USHORT Em_l[3];
