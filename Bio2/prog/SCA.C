/*
 * SCA.C - Room collision: hit tests, ground height, floor search
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCA.C
 */

#include "TYPES.H"
#include "ROOM.H"
#include "DIVCHK.H"
#include <ASM.H>
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include "SCA.H"
#include "EM_SUB.H"
#include "OM.H"
#include "OMA.H"
#include "SUB.H"
#include "flag.h"

/* Get_axis and Get_axis2 start from constant SVECTORs read from the data words (ghidramcp
 * list_data_items). Get_axis: 0x80011154 = 0x80010000, 0x80011158 = 0 -> vx 0, vy -32767, vz 0.
 * Get_axis2: 0x8001115c = 0x7FFF0000, 0x80011160 = 0 -> vx 0, vy 32767, vz 0. The copy below
 * matches the binary's 8-byte load. */
/* DS 0x80011154 (Get_axis_vec): constant SVECTOR read by Get_axis */
static const SVECTOR Get_axis_vec = { 0, -0x7fff, 0, 0 };

/* DS 0x8001115c (Get_axis2_vec): constant SVECTOR read by Get_axis2 */
static const SVECTOR Get_axis2_vec = { 0, 0x7fff, 0, 0 };

/* Height of one floor level in world units. */
#define SCA_LEVEL_H 0x708

/* Hit handlers for the shape table below (defined further down). */
ULONG Sca_hit_box(ENEMY_WORK *pEm, long R, SCA_DATA *pScd);
ULONG Sca_hit_naname_a(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_naname_b(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_naname_c(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_naname_d(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_hishi(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_circle(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_koban_x(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_koban_z(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_slope(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
ULONG Sca_hit_curve(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);

/* DS 0x800a737c (Sca_rtn_tbl): hit handler per shape, indexed by Id & 0xf */
typedef ULONG (*SCA_RTN_FN)(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz);
static SCA_RTN_FN Sca_rtn_tbl[14] = {
	Sca_hit_box, Sca_hit_naname_a, Sca_hit_naname_b, Sca_hit_naname_c, Sca_hit_naname_d,
	Sca_hit_hishi, Sca_hit_circle, Sca_hit_koban_x, Sca_hit_koban_z, Sca_hit_box,
	Sca_hit_box, Sca_hit_slope, Sca_hit_box, Sca_hit_curve,
};

/* DS 0x800a73b4 (Sca_no_ck_tbl): one byte per Id & 0xf; nonzero means Sca_ck_line skips that shape */
static const UCHAR Sca_no_ck_tbl[16] = {
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};

/* Module statics. Old_x, Old_z, Gnd_y, In_flg, pSca_data and Tmp_Sca are declared extern in
 * the old global.h at their DS addresses, so they are not redefined here. */
static short     Hit_flg_0;        /* 0x800c3b68 - hit flags accumulated by Sca_ck_hit */
static short     Hit_flg;          /* 0x800c3b78 - hit flags accumulated by Sca_ck_hit_om */
static long      Gnd_low_y;        /* 0x800dcc2c - lowest ground offset seen by Sca_get_ground */

/* DS 0x800d5be4 (Ceiling) - set by Sca_get_ground; retail keeps it as zero bytes in the image, so the zero initialiser keeps it in .data. */
long Ceiling = 0;

/* DS 0x800ea198 (Ceiling_in_pos) - long[36]; only [0] is used (Sca_get_ground). Bytes are zero in the image. */
long Ceiling_in_pos[36] = {
	0, 0, 0, 0,	/* +0x0 */
	0, 0, 0, 0,	/* +0x10 */
	0, 0, 0, 0,	/* +0x20 */
	0, 0, 0, 0,	/* +0x30 */
	0, 0, 0, 0,	/* +0x40 */
	0, 0, 0, 0,	/* +0x50 */
	0, 0, 0, 0,	/* +0x60 */
	0, 0, 0, 0,	/* +0x70 */
	0, 0, 0, 0,	/* +0x80 */
};

/* DS 0x8004c198 (Sca_get_area) */
ULONG Sca_get_area(long X, long Z, long Sx, long Sz)
{
	return (1 << ((UINT)(X - Sx) >> 0x1f)) << ((UINT)(Z - Sz) >> 0x1e & 2);
}

/* DS 0x8004c1bc (Sca_ck_hit) */
ULONG Sca_ck_hit(VECTOR *pPos, long R, ULONG Floor, ULONG Flg)
{
	ENEMY_WORK *pEm;
	SCA_HEAD *pHead;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	ULONG Area;
	ULONG Id_mask;       /* Flg & 0xffff: entry must share at least one Id bit */
	ULONG Excl_mask;     /* Flg >> 16: entries sharing any of these Id bits are skipped */
	ULONG Ck_flg;
	long  Pos_x;         /* pPos->vx + R, updated when R is recomputed or the body is pushed */
	long  Pos_z;         /* pPos->vz + R */
	long  Dx;
	long  Dz;
	ULONG   Frame_pad[2];		/* retail never reads 0x14 and 0x1c (its spill slots are 0x10 and 0x18, 8-byte stride) */

	(void)Frame_pad;
	pEm = G.pEm;
	G.pSca_hit_data[0] = (SCA_DATA *)0x0;
	pSca_data = G.pSca_hit_data;
	Hit_flg_0 = 0;
	pHead = (SCA_HEAD *)G.pRoom->pSca;
	pEnd = (SCA_DATA *)pHead + pHead->Num;
	Area = Sca_get_area(pPos->vx, pPos->vz, pHead->Cx, pHead->Cz);
	Id_mask = Flg & 0xffff;
	Excl_mask = Flg >> 16;
	Pos_x = pPos->vx + R;
	Pos_z = pPos->vz + R;

	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz < pEnd; pT_xz++) {
		if ((pT_xz->Type & Area) == 0) continue;

		if ((R & 1) != 0) {
			/* Odd radius: the enemy footprint is rotated toward the box centre. The angle is
			 * folded into one quadrant and R is blended between the box's half-extents. */
			long  Ang;
			UINT  Ang_u;
			USHORT Qa;
			UINT  D_e;
			UINT  W_e;

			Ang = ratan2((pT_xz->Z + (pT_xz->D >> 1)) - Pos_z, (pT_xz->X + (pT_xz->W >> 1)) - Pos_x);
			Ang_u = (UINT)(Ang - (USHORT)pEm->Cdir_y);
			Qa = (USHORT)(Ang_u & 0xfff);
			if ((Ang_u & 0xc00) == 0xc00) {
				Qa = 0x1000 - Qa;
			} else if ((Ang_u & 0x800) == 0) {
				if ((Ang_u & 0x400) != 0) {
					Qa = 0x800 - Qa;
				}
			} else {
				Qa -= 0x800;
			}
			D_e = pEm->Atd[0].D;
			W_e = pEm->Atd[0].W;
			if (D_e < W_e) {
				R = D_e + ((((long)(W_e - D_e)) * rcos((short)Qa)) >> 12);
			} else {
				R = W_e + ((((long)(D_e - W_e)) * rsin((short)Qa)) >> 12);
			}
			Pos_x = pPos->vx + R;
			Pos_z = pPos->vz + R;
		}

		if ((UINT)(Pos_x - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(R * 2)) continue;
		if ((UINT)(Pos_z - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(R * 2)) continue;
		if ((pT_xz->Id & Id_mask) == 0) continue;
		if ((pT_xz->Id & Excl_mask) != 0) continue;
		if ((pT_xz->Floor & Floor) == 0) continue;

		*pSca_data = pT_xz;
		pSca_data++;
		*pSca_data = (SCA_DATA *)0x0;

		if (Excl_mask == 0) {
			/* Plain collision: classify the shape from Id & 0xf. Shapes 1..6 run a
			 * per-shape check; 0, 7, 8 and 9..12 set fixed flag bits. */
			switch (pT_xz->Id & 0xf) {
			case 0:
			case 7:
			case 8:
				Hit_flg_0 |= 1;
				break;
			case 1:
				Ck_flg = Sca_ck_naname_a((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 2:
				Ck_flg = Sca_ck_naname_b((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 3:
				Ck_flg = Sca_ck_naname_c((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 4:
				Ck_flg = Sca_ck_naname_d((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 5:
				Ck_flg = Sca_ck_hishi((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 6:
				Ck_flg = Sca_ck_circle((VECTOR *)pEm->Atd, R, pT_xz);
				Hit_flg_0 |= (USHORT)Ck_flg;
				break;
			case 9:
				Hit_flg_0 |= 3;
				break;
			case 10:
				Hit_flg_0 |= 4;
				break;
			case 11:
				Hit_flg_0 |= 8;
				break;
			case 12:
				Hit_flg_0 |= 0x10;
				pStairs = pT_xz;
				break;
			default:
				break;
			}
		} else {
			/* Excluded-Id pass: the shape handler moves the enemy out of the box. */
			Old_x = pEm->Atd[0].X;
			Old_z = pEm->Atd[0].Z;
			if (Sca_rtn_tbl[pT_xz->Id & 0xf](pEm, R, pT_xz) != 0) {
				Dx = pEm->Atd[0].X - Old_x;
				Dz = pEm->Atd[0].Z - Old_z;
				pEm->Pos_x += Dx;
				Pos_x += Dx;
				pEm->Base_pos_x += (short)Dx;
				Hit_flg_0 |= 1;
				pEm->Base_pos_z += (short)Dz;
				pEm->Pos_z += Dz;
				Pos_z += Dz;
				if (Dz != 0) {
					pEm->Be_flg |= 0x4000;
				}
			}
		}
	}
	return (ULONG)Hit_flg_0;
}

/* DS 0x8004c658 (Sca_ck_hit_om) */
ULONG Sca_ck_hit_om(OBJ_MODEL_WORK *pOm, ULONG Att, ULONG Flg)
{
	SCA_HEAD *pHead;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	ULONG Area;
	ULONG Floor_bit;
	long  Base_x;    /* model origin: Pos_x + Atd[0].Atw_x */
	long  Base_z;    /* model origin: Pos_z + Atd[0].Atw_z */
	long  Mid_x;     /* model centre: Base_x + W, refreshed after a box push */
	long  Mid_z;     /* model centre: Base_z + D */
	long  W;
	long  D;

	(void)Att;       /* not read by this routine; kept to match the mockup prototype */
	if ((pOm->Be_flg & 8) != 0) {
		return 0;
	}

	Base_x = pOm->Pos_x + pOm->Atd[0].Atw_x;
	Base_z = pOm->Pos_z + pOm->Atd[0].Atw_z;
	pHead = (SCA_HEAD *)G.pRoom->pSca;
	pEnd = (SCA_DATA *)pHead + pHead->Num;
	Area = Sca_get_area(Base_x, Base_z, pHead->Cx, pHead->Cz);
	W = pOm->Atd[0].W;
	D = pOm->Atd[0].D;
	Floor_bit = 1UL << (pOm->nFloor & 0x1f);
	Hit_flg = 0;
	Mid_x = Base_x + W;
	Mid_z = Base_z + D;

	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz < pEnd; pT_xz++) {
		if ((pT_xz->Type & Area) == 0) continue;
		if ((UINT)(Mid_x - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(W * 2)) continue;
		if ((UINT)(Mid_z - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(D * 2)) continue;
		if ((pT_xz->Id & 0x4000) == 0) continue;
		if ((pT_xz->Id & Flg) != 0) continue;
		if ((pT_xz->Floor & Floor_bit) == 0) continue;

		if ((pT_xz->Id & 0xf) == 10) {
			Hit_flg |= 4;
		} else {
			if (Flg != 0) {
				Sca_hit_box((ENEMY_WORK *)pOm, D, pT_xz);
				Mid_x = pOm->Pos_x + pOm->Atd[0].Atw_x + W;
				Mid_z = pOm->Pos_z + pOm->Atd[0].Atw_z + D;
			}
			Hit_flg |= 1;
		}
	}
	return (ULONG)Hit_flg;
}

/* DS 0x8004c858 (Sca_hit_box2) */
ULONG Sca_hit_box2(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long Lim_x;    /* allowed X offset: nearer of the far (+X) and near (-X) edges */
	long Lim_z;    /* allowed Z offset: nearer of the far (+Z) and near (-Z) edges */
	long Far_x;
	long Near_x;
	long Far_z;
	long Near_z;

	Far_x  = (pT_xz->X + pT_xz->W + pEm->Atd[0].W) - pEm->Atd[0].X + 1;
	Near_x = (pT_xz->X - pEm->Atd[0].W) - pEm->Atd[0].X - 1;
	Lim_x = Far_x;
	if (-Near_x < Far_x) {
		Lim_x = Near_x;
	}

	Far_z  = (pT_xz->Z + pT_xz->D + R) - pEm->Atd[0].Z + 1;
	Near_z = (pT_xz->Z - R) - pEm->Atd[0].Z - 1;
	Lim_z = Far_z;
	if (-Near_z < Far_z) {
		Lim_z = Near_z;
	}

	return Sca_hosei_box(pEm, (SVECTOR *)&pEm->Sca_old_x, Lim_x, Lim_z);
}

/* DS 0x8004c900 (Sca_hit_box) */
ULONG Sca_hit_box(ENEMY_WORK *pEm, long R, SCA_DATA *pScd)
{
	long  Span_x;     /* Atd.W * 2 + pScd->W: separation at which the boxes touch on X */
	long  Span_z;     /* Atd.D * 2 + pScd->D: separation at which the boxes touch on Z */
	long  Gap_x;      /* (Sca_old_x + Atd.W) - pScd->X: old position against box, X */
	long  Gap_z;      /* (Sca_old_z + Atd.D) - pScd->Z: old position against box, Z */
	UINT  Side;       /* bit 1: push out on X, bit 0: push out on Z */
	long  Half;
	long  Push;

	Span_x = pEm->Atd[0].W * 2 + pScd->W;
	Span_z = pEm->Atd[0].D * 2 + pScd->D;
	Gap_x = (pEm->Sca_old_x + pEm->Atd[0].W) - pScd->X;
	Gap_z = (pEm->Sca_old_z + pEm->Atd[0].D) - pScd->Z;

	/* Unsigned compares: a negative gap is "outside" on that axis. */
	Side = (((UINT)Gap_x >= (UINT)Span_x) ? 2 : 0) | (((UINT)Gap_z >= (UINT)Span_z) ? 1 : 0);
	if (Gap_x == -1 || Gap_x == Span_x + 1) {
		Side = 2;
	}
	if (Gap_z == -1 || Gap_z == Span_z + 1) {
		Side = 1;
	}

	if (Side == 0) {
		return Sca_hit_box2(pEm, R, pScd);
	}

	if ((Side & 2) != 0) {
		Half = pScd->W >> 1;
		Push = pEm->Atd[0].W + Half + 1;
		if (pEm->Atd[0].X - pEm->Sca_old_x >= 0) {
			Push = -Push;
		}
		pEm->Atd[0].X = pScd->X + Half + Push;
	}
	if ((Side & 1) != 0) {
		Half = pScd->D >> 1;
		Push = pEm->Atd[0].D + Half + 1;
		if (pEm->Atd[0].Z - pEm->Sca_old_z >= 0) {
			Push = -Push;
		}
		pEm->Atd[0].Z = pScd->Z + Half + Push;
	}
	return 1;
}

/* DS 0x8004ca68 (Sca_hosei_box) */
ULONG Sca_hosei_box(ENEMY_WORK *pEm, SVECTOR *pOld, long X, long Z)
{
	long  Abs_x;
	long  Abs_z;
	UINT  Mode;      /* bit 0: the move on X points against the saved position, bit 1: same on Z */
	ULONG Pad[2];

	/* pOld is not read; the saved position comes from pEm->Sca_old_x/z. */
	(void)pOld;
	(void)Pad;
	Mode = ((UINT)(X ^ (pEm->Atd[0].X - pEm->Sca_old_x)) >> 31)
		 | (((UINT)(Z ^ (pEm->Atd[0].Z - pEm->Sca_old_z)) >> 30) & 2);

	if (Mode == 2) {
		/* Z only: slide on Z when the Z step is small. */
		Abs_z = (Z < 0) ? -Z : Z;
		if (Abs_z < 0x191) {
			pEm->Atd[0].Z += Z;
			return 1;
		}
	} else if (Mode == 1) {
		/* X only: slide on X when the X step is small. */
		Abs_x = (X < 0) ? -X : X;
		if (Abs_x < 0x191) {
			pEm->Atd[0].X += X;
			return 1;
		}
	} else if (Mode == 0) {
		/* Both steps agree in sign: slide along the axis with the larger step. */
		Abs_x = (X < 0) ? -X : X;
		Abs_z = (Z < 0) ? -Z : Z;
		if (Abs_x < Abs_z) {
			pEm->Atd[0].X += X;
		} else {
			pEm->Atd[0].Z += Z;
		}
		return 1;
	}

	/* Fallback: slide along the smaller step if it is under 0x321, else revert to the saved
	 * position. */
	Abs_x = (X < 0) ? -X : X;
	Abs_z = (Z < 0) ? -Z : Z;
	if (Abs_x < Abs_z) {
		if (Abs_x < 0x321) {
			pEm->Atd[0].X += X;
			return 1;
		}
	} else if (Abs_z < 0x321) {
		pEm->Atd[0].Z += Z;
		return 1;
	}
	pEm->Atd[0].X = pEm->Sca_old_x;
	pEm->Atd[0].Z = pEm->Sca_old_z;
	return 1;
}

/* DS 0x8004cbd0 (Sca_hit_naname_a) */
ULONG Sca_hit_naname_a(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	short Save_z;
	int Cz_far;
	ULONG Ret;
	int Gap_z;
	int Cz_near;
	int Cx_near;
	UINT Width_x;
	int Work_a;
	int Em_x;
	int Em_z;
	int Cx_span;
	int Cz_span;
	UINT Depth_z;
	int Work_z;
	int Diag_z;
	int Work_b;

	Depth_z = (UINT)pT_xz->D;
	Em_x  = pEm->Atd[0].X;
	Work_a  = (int)pT_xz->X;
	Work_z = (R * -2 - Depth_z) * (Em_x - (Work_a - R));
	Width_x  = (UINT)pT_xz->W;
	Gap_z  = Width_x + R * 2;
	DIV_S(Diag_z, Work_z, Gap_z);
	Em_z = pEm->Atd[0].Z;
	Work_z = (int)pT_xz->Z;
	Gap_z  = Em_z - (Depth_z + Work_z);
	if (Diag_z < Gap_z) {
		Cz_far  = (int)(Depth_z + Work_z) / 0x12;
		Cz_near  = (Work_z / 0x12) * 0x10000 >> 0x10;
		Cz_span = Cz_near - Cz_far;
		Cx_near  = (Work_a  / 0x12) * 0x10000 >> 0x10;
		Work_b = (int)(Width_x + Work_a) / 0x12;
		Cx_span = Work_b - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) -
			  Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) -
			 Cx_near * Cz_near) + Work_b * Cz_far < 0) {
			if (((UINT)(Em_x - Work_a) < Width_x + R) &&
				((UINT)(Em_z - Work_z) < Depth_z + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Gap_z -= Diag_z;
			Work_z = (Width_x + R) * Gap_z;
			Work_a  = Depth_z + R;
			DIV_S(Diag_z, Work_z, Work_a);
			Work_z = Diag_z * Gap_z * Gap_z;
			Work_a  = Diag_z * Diag_z + Gap_z * Gap_z;
			DIV_S(Work_b, Work_z, Work_a);
			Gap_z  = Diag_z * Diag_z * Gap_z;
			DIV_S(Work_z, Gap_z, Work_a);
			Gap_z = Work_b;
			if (Work_b < 0) Gap_z = -Work_b;
			if (Gap_z < 0x191) {
				Gap_z = Work_z;
				if (Work_z < 0) Gap_z = -Work_z;
				if (Gap_z < 0x191) {
					pEm->Atd[0].X = Em_x  - Work_b;
					pEm->Atd[0].Z -= Work_z;
					if (-1 < (Em_x / 0x12 - Cx_near) * Cz_span -
							  (Em_z / 0x12 - Cz_far) * Cx_span) {
						return 1;
					}
				}
			}
			Save_z = pEm->Sca_old_z;
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)Save_z;
		}
		return 1;
	}
	return 0;
}

/* DS 0x8004cfc8 (Sca_ck_naname_a) */
ULONG Sca_ck_naname_a(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long Dx;      /* pos.x against the left edge, inset by R */
	long Slope;   /* -(D + 2R) */
	long Span;    /* W + 2R */
	long Dz;      /* pos.z against the far (+Z) edge, inset by R */

	Dx = pPos->vx - (pT_xz->X - R);
	Slope = -(pT_xz->D + R * 2);
	Span = pT_xz->W + R * 2;
	Dz = pPos->vz - (pT_xz->Z + pT_xz->D + R);
	{
		long Quot;
		DIV_S(Quot, Dx * Slope, Span);
		return (ULONG)(Quot < Dz);
	}
}

/* DS 0x8004d048 (Sca_hit_naname_b) */
ULONG Sca_hit_naname_b(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	short Save_z;
	ULONG Ret;
	int Work_a;
	int Work_b;
	int Cz_near;
	int Work_d;
	int Work_c;
	int Cx_near;
	int Em_x;
	int Box_z;
	int Cx_span;
	int Cz_span;
	int Em_z;
	int Span_z;
	int Rel_z;
	int Diag_z;

	Em_x  = pEm->Atd[0].X;
	Work_c  = (int)(short)(pT_xz->D + pT_xz->Z);
	Box_z = (int)pT_xz->Z;
	Span_z = Work_c - (Box_z - R);
	Work_b  = (int)pT_xz->X;
	Rel_z = Span_z * (Em_x - Work_b);
	Work_a  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Work_d  = (Work_a + R) - Work_b;
	DIV_S(Diag_z, Rel_z, Work_d);
	Em_z = pEm->Atd[0].Z;
	Rel_z = Em_z - (Box_z - R);
	if (Diag_z < Rel_z) {
		Cx_near  = (Work_b  / 0x12) * 0x10000 >> 0x10;
		Cz_near  = (Box_z / 0x12) * 0x10000 >> 0x10;
		Work_c  = (Work_c  / 0x12) * 0x10000 >> 0x10;
		Cz_span = Work_c - Cz_near;
		Work_a  = (Work_a  / 0x12) * 0x10000 >> 0x10;
		Cx_span = Work_a - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) -
			  Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) -
			 Cx_near * Work_c) + Work_a * Cz_near < 0) {
			if (((UINT)(Em_x - (Work_b - R)) < (UINT)pT_xz->W + R) &&
				((UINT)(Em_z - Box_z) < (UINT)pT_xz->D + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Rel_z -= Diag_z;
			DIV_S(Work_a, Work_d * Rel_z, Span_z);
			Work_d = Work_a * Rel_z * Rel_z;
			Work_b = Work_a * Work_a + Rel_z * Rel_z;
			DIV_S(Work_c, Work_d, Work_b);
			Rel_z = Work_a * Work_a * Rel_z;
			DIV_S(Work_a, Rel_z, Work_b);
			Work_b = Work_c;
			if (Work_c < 0) Work_b = -Work_c;
			if (Work_b < 0x191) {
				Work_b = Work_a;
				if (Work_a < 0) Work_b = -Work_a;
				if (Work_b < 0x191) {
					pEm->Atd[0].X = Em_x + Work_c;
					pEm->Atd[0].Z -= Work_a;
					if (-1 < (Em_x  / 0x12 - Cx_near) * Cz_span -
							  (Em_z / 0x12 - Cz_near) * Cx_span) {
						return 1;
					}
				}
			}
			Save_z = pEm->Sca_old_z;
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)Save_z;
		}
		return 1;
	}
	return 0;
}

/* DS 0x8004d484 (Sca_ck_naname_b) */
ULONG Sca_ck_naname_b(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long Dx;      /* pos.x against the far (+X) edge, inset by R */
	long Slope;   /* -(D + 2R) */
	long Span;    /* -(W + 2R) */
	long Dz;      /* pos.z against the far (+Z) edge, inset by R */

	Dx = pPos->vx - (pT_xz->X + pT_xz->W + R);
	Slope = -(pT_xz->D + R * 2);
	Span = -(pT_xz->W + R * 2);
	Dz = pPos->vz - (pT_xz->Z + pT_xz->D + R);
	{
		long Quot;
		DIV_S(Quot, Dx * Slope, Span);
		return (ULONG)(Quot < Dz);
	}
}

/* DS 0x8004d508 (Sca_hit_naname_c) */
ULONG Sca_hit_naname_c(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	short Save_z;
	ULONG Ret;
	int Work_a;
	int Cz_near;
	int Work_b;
	int Work_c;
	int Work_d;
	int Cx_near;
	int Em_x;
	int Span_z;
	int Cx_span;
	int Cz_span;
	int Box_x;
	int Em_z;
	int Diag_z;

	Em_x  = pEm->Atd[0].X;
	Work_d  = (int)(short)(pT_xz->D + pT_xz->Z);
	Work_c  = (int)pT_xz->Z;
	Span_z = (Work_d + R) - Work_c;
	Box_x = (int)pT_xz->X;
	Em_z = Span_z * (Em_x - (Box_x - R));
	Work_a  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Work_b  = Work_a - (Box_x - R);
	DIV_S(Diag_z, Em_z, Work_b);
	Em_z = pEm->Atd[0].Z;
	if (Em_z - Work_c < Diag_z) {
		Cx_near  = (Box_x / 0x12) * 0x10000 >> 0x10;
		Cz_near  = (Work_c  / 0x12) * 0x10000 >> 0x10;
		Work_d  = (Work_d  / 0x12) * 0x10000 >> 0x10;
		Cz_span = Work_d - Cz_near;
		Work_a  = (Work_a  / 0x12) * 0x10000 >> 0x10;
		Cx_span = Work_a - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) -
			  Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) -
			 Cx_near * Work_d) + Work_a * Cz_near < 1) {
			Diag_z = (Em_z - Work_c) - Diag_z;
			DIV_S(Work_a, Work_b * Diag_z, Span_z);
			Work_c = Work_a * Diag_z * Diag_z;
			Work_b = Work_a * Work_a + Diag_z * Diag_z;
			DIV_S(Work_d, Work_c, Work_b);
			Diag_z = Work_a * Work_a * Diag_z;
			DIV_S(Work_a, Diag_z, Work_b);
			Diag_z = Work_d;
			if (Work_d < 0) Diag_z = -Work_d;
			if (Diag_z < 0x191) {
				Diag_z = Work_a;
				if (Work_a < 0) Diag_z = -Work_a;
				if (Diag_z < 0x191) {
					pEm->Atd[0].X = Em_x + Work_d;
					pEm->Atd[0].Z -= Work_a;
					if ((Em_x  / 0x12 - Cx_near) * Cz_span -
						(Em_z / 0x12 - Cz_near) * Cx_span < 1) {
						return 1;
					}
				}
			}
			Save_z = pEm->Sca_old_z;
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)Save_z;
		} else if (((UINT)(Em_x  - Box_x) < (UINT)pT_xz->W + R) &&
				   ((UINT)(Em_z - (Work_c - R)) < (UINT)pT_xz->D + R)) {
			Ret = Sca_hit_box(pEm, R, pT_xz);
			return Ret;
		}
		return 1;
	}
	return 0;
}

/* DS 0x8004d940 (Sca_ck_naname_c) */
ULONG Sca_ck_naname_c(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long Dx;      /* pos.x against the left edge, inset by R */
	long Slope;   /* D + 2R */
	long Span;    /* W + 2R */
	long Dz;      /* pos.z against the near (-Z) edge, inset by R */

	Dx = pPos->vx - (pT_xz->X - R);
	Slope = pT_xz->D + R * 2;
	Span = pT_xz->W + R * 2;
	Dz = pPos->vz - (pT_xz->Z - R);
	{
		long Quot;
		DIV_S(Quot, Dx * Slope, Span);
		return (ULONG)(Dz < Quot);
	}
}

/* DS 0x8004d9b4 (Sca_hit_naname_d) */
ULONG Sca_hit_naname_d(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	short Save_z;
	ULONG Ret;
	int Work_a;
	int Work_e;
	int Work_b;
	int Span_x;
	int Cz_far;
	int Cx_near;
	int Box_z;
	int Work_c;
	int Cx_span;
	int Cz_span;
	int Em_x;
	int Em_z;
	int Diag_z;

	Em_x = pEm->Atd[0].X;
	Box_z  = (int)pT_xz->Z;
	Work_e  = (int)(short)(pT_xz->D + pT_xz->Z);
	Work_c = Work_e + R;
	Work_b  = (int)pT_xz->X;
	Em_z = (Box_z - Work_c) * (Em_x - Work_b);
	Work_a  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Span_x  = (Work_a + R) - Work_b;
	DIV_S(Diag_z, Em_z, Span_x);
	Em_z = pEm->Atd[0].Z;
	if (Em_z - Work_c < Diag_z) {
		Cx_near  = (Work_b / 0x12) * 0x10000 >> 0x10;
		Cz_far  = (Work_e / 0x12) * 0x10000 >> 0x10;
		Work_e  = (Box_z / 0x12) * 0x10000 >> 0x10;
		Cz_span = Work_e - Cz_far;
		Work_a  = (Work_a / 0x12) * 0x10000 >> 0x10;
		Cx_span = Work_a - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) -
			  Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) -
			 Cx_near * Work_e) + Work_a * Cz_far < 1) {
			Diag_z = (Em_z - Work_c) - Diag_z;
			Work_c -= Box_z;
			DIV_S(Work_a, Span_x * Diag_z, Work_c);
			Work_e  = Work_a * Diag_z * Diag_z;
			Work_c = Work_a * Work_a + Diag_z * Diag_z;
			DIV_S(Work_b, Work_e, Work_c);
			Diag_z = Work_a * Work_a * Diag_z;
			DIV_S(Work_a, Diag_z, Work_c);
			Work_c = Work_b;
			if (Work_b < 0) Work_c = -Work_b;
			if (Work_c < 0x191) {
				Work_c = Work_a;
				if (Work_a < 0) Work_c = -Work_a;
				if (Work_c < 0x191) {
					pEm->Atd[0].X = Em_x - Work_b;
					pEm->Atd[0].Z -= Work_a;
					if ((Em_x / 0x12 - Cx_near) * Cz_span -
						(Em_z / 0x12 - Cz_far) * Cx_span < 1) {
						return 1;
					}
				}
			}
			Save_z = pEm->Sca_old_z;
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)Save_z;
		} else if (((UINT)(Em_x - (Work_b - R)) < (UINT)pT_xz->W + R) &&
				   ((UINT)(Em_z - (Box_z - R)) < (UINT)pT_xz->D + R)) {
			Ret = Sca_hit_box(pEm, R, pT_xz);
			return Ret;
		}
		return 1;
	}
	return 0;
}

/* DS 0x8004dde8 (Sca_ck_naname_d) */
ULONG Sca_ck_naname_d(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long Dx;      /* pos.x against the far (+X) edge, inset by R */
	long Slope;   /* -(D + 2R) */
	long Span;    /* W + 2R */
	long Dz;      /* pos.z against the near (-Z) edge, inset by R */

	Dx = pPos->vx - (pT_xz->X + pT_xz->W + R);
	Slope = -(pT_xz->D + R * 2);
	Span = pT_xz->W + R * 2;
	Dz = pPos->vz - (pT_xz->Z - R);
	{
		long Quot;
		DIV_S(Quot, Dx * Slope, Span);
		return (ULONG)(Dz < Quot);
	}
}

/* DS 0x8004de64 (Sca_hit_hishi) */
ULONG Sca_hit_hishi(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	short Save_z;
	USHORT Box_x;
	USHORT Box_z;
	UINT Quad;
	int Work_d;
	int Work_a;
	int Work_g;
	int Work_h;
	int Work_i;
	int Work_j;
	int Work_b;
	int Work_e;
	int Work_f;
	int Work_c;

	Box_x  = pT_xz->X;
	Box_z  = pT_xz->Z;
	Work_j = pEm->Atd[0].Z;
	Work_i  = pEm->Atd[0].X;
	Work_e = (UINT)pT_xz->W + (UINT)Box_x;
	Work_d  = (UINT)pT_xz->D + (UINT)Box_z;
	Work_g  = (int)(((UINT)Box_z + (UINT)(pT_xz->D >> 1)) * 0x10000) >> 0x10;
	Work_h  = (int)(((UINT)Box_x + (UINT)(pT_xz->W >> 1)) * 0x10000) >> 0x10;
	Quad  = ((UINT)(Work_j - Work_g) >> 0x1e & 2) | (UINT)(Work_i - Work_h) >> 0x1f;

	if (Quad == 1) {
		Work_a  = Work_d * 0x10000 >> 0x10;
		Work_e = (Work_a + R) - Work_g;
		Work_b = Work_e * ((Work_i + pEm->Atd[0].Atw_x) - ((short)Box_x - R));
		Work_d  = Work_h - ((short)Box_x - R);
		DIV_S(Work_f, Work_b, Work_d);
		Work_j = (Work_j + pEm->Atd[0].Atw_z) - Work_g;
		Work_b = Work_j - Work_f;
		if (Work_f <= Work_j) return 0;
		DIV_S(Work_j, Work_d * Work_b, Work_e);
		Work_e = Work_j * Work_b * Work_b;
		Work_d  = Work_j * Work_j + Work_b * Work_b;
		DIV_S(Work_f, Work_e, Work_d);
		Work_b = Work_j * Work_j * Work_b;
		DIV_S(Work_j, Work_b, Work_d);
		Work_d = Work_f; if (Work_f < 0) Work_d = -Work_f;
		if (400 < Work_d) goto old;
		Work_d = Work_j; if (Work_j < 0) Work_d = -Work_j;
		if (400 < Work_d) goto old;
		pEm->Atd[0].X = Work_i + Work_f;
		Work_j = pEm->Atd[0].Z - Work_j;
		pEm->Atd[0].Z = Work_j;
		Work_i = (Work_g / 0x12) * 0x10000 >> 0x10;
		Work_g = ((int)pT_xz->X / 0x12) * 0x10000 >> 0x10;
		Work_d = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Work_g) *
				(((Work_a / 0x12) * 0x10000 >> 0x10) - Work_i);
		Work_g = ((Work_j + pEm->Atd[0].Atw_z) / 0x12 - Work_i) *
				(((Work_h / 0x12) * 0x10000 >> 0x10) - Work_g);
	} else if (Quad < 2) {
		if (Quad != 0) goto old;
		Work_b = Work_d * 0x10000 >> 0x10;
		Work_d  = Work_b + R;
		Work_f = (Work_g - Work_d) * ((Work_i + pEm->Atd[0].Atw_x) - Work_h);
		Work_a  = Work_e * 0x10000 >> 0x10;
		Work_e = (Work_a + R) - Work_h;
		DIV_S(Work_c, Work_f, Work_e);
		Work_j = (Work_j + pEm->Atd[0].Atw_z) - Work_d;
		Work_f = Work_j - Work_c;
		if (Work_c <= Work_j) return 0;
		Work_d -= Work_g;
		DIV_S(Work_j, Work_e * Work_f, Work_d);
		Work_e = Work_j * Work_f * Work_f;
		Work_d  = Work_j * Work_j + Work_f * Work_f;
		DIV_S(Work_c, Work_e, Work_d);
		Work_f = Work_j * Work_j * Work_f;
		DIV_S(Work_j, Work_f, Work_d);
		Work_d = Work_c; if (Work_c < 0) Work_d = -Work_c;
		if (400 < Work_d) goto old;
		Work_d = Work_j; if (Work_j < 0) Work_d = -Work_j;
		if (400 < Work_d) goto old;
		pEm->Atd[0].X = Work_i - Work_c;
		Work_j = pEm->Atd[0].Z - Work_j;
		Work_h = (Work_h / 0x12) * 0x10000 >> 0x10;
		Work_i = (Work_b / 0x12) * 0x10000 >> 0x10;
		Work_d = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Work_h) *
				(((Work_g / 0x12) * 0x10000 >> 0x10) - Work_i);
		Work_g = ((Work_j + pEm->Atd[0].Atw_z) / 0x12 - Work_i) *
				(((Work_a / 0x12) * 0x10000 >> 0x10) - Work_h);
		pEm->Atd[0].Z = Work_j;
	} else if (Quad == 2) {
		Work_a  = Work_g - ((short)Box_z - R);
		Work_b = Work_a * ((Work_i + pEm->Atd[0].Atw_x) - Work_h);
		Work_e = Work_e * 0x10000 >> 0x10;
		Work_d  = (Work_e + R) - Work_h;
		DIV_S(Work_f, Work_b, Work_d);
		Work_j = (Work_j + pEm->Atd[0].Atw_z) - ((short)Box_z - R);
		Work_b = Work_j - Work_f;
		if (Work_j <= Work_f) return 0;
		DIV_S(Work_j, Work_d * Work_b, Work_a);
		Work_a = Work_j * Work_b * Work_b;
		Work_d = Work_j * Work_j + Work_b * Work_b;
		DIV_S(Work_f, Work_a, Work_d);
		Work_b = Work_j * Work_j * Work_b;
		DIV_S(Work_j, Work_b, Work_d);
		Work_d = Work_f; if (Work_f < 0) Work_d = -Work_f;
		if (400 < Work_d) goto old;
		Work_d = Work_j; if (Work_j < 0) Work_d = -Work_j;
		if (400 < Work_d) goto old;
		pEm->Atd[0].X = Work_i + Work_f;
		Work_j = pEm->Atd[0].Z - Work_j;
		pEm->Atd[0].Z = Work_j;
		Work_d = (Work_h / 0x12) * 0x10000 >> 0x10;
		Work_h = ((int)pT_xz->Z / 0x12) * 0x10000 >> 0x10;
		Work_d = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Work_d) *
				(((Work_g / 0x12) * 0x10000 >> 0x10) - Work_h) -
				((Work_j + pEm->Atd[0].Atw_z) / 0x12 - Work_h) *
				(((Work_e / 0x12) * 0x10000 >> 0x10) - Work_d);
		if (-1 < Work_d) return 1;
		goto old;
	} else {
		if (Quad != 3) goto old;
		Work_e = (((short)Box_z - R) - Work_g) *
				 ((Work_i + pEm->Atd[0].Atw_x) - ((short)Box_x - R));
		Work_d  = Work_h - ((short)Box_x - R);
		DIV_S(Work_a, Work_e, Work_d);
		Work_j = (Work_j + pEm->Atd[0].Atw_z) - Work_g;
		Work_e = Work_j - Work_a;
		if (Work_j <= Work_a) return 0;
		Work_j = Work_g - ((short)Box_z - R);
		DIV_S(Work_a, Work_d * Work_e, Work_j);
		Work_j = Work_a * Work_e * Work_e;
		Work_d  = Work_a * Work_a + Work_e * Work_e;
		DIV_S(Work_b, Work_j, Work_d);
		Work_e = Work_a * Work_a * Work_e;
		DIV_S(Work_j, Work_e, Work_d);
		Work_d = Work_b; if (Work_b < 0) Work_d = -Work_b;
		if (400 < Work_d) goto old;
		Work_d = Work_j; if (Work_j < 0) Work_d = -Work_j;
		if (400 < Work_d) goto old;
		pEm->Atd[0].X = Work_i - Work_b;
		Work_j = pEm->Atd[0].Z - Work_j;
		pEm->Atd[0].Z = Work_j;
		Work_g = (Work_g / 0x12) * 0x10000 >> 0x10;
		Work_d = ((int)pT_xz->X / 0x12) * 0x10000 >> 0x10;
		Work_d = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Work_d) *
				((((int)pT_xz->Z / 0x12) * 0x10000 >> 0x10) - Work_g) -
				((Work_j + pEm->Atd[0].Atw_z) / 0x12 - Work_g) *
				(((Work_h / 0x12) * 0x10000 >> 0x10) - Work_d);
		if (-1 < Work_d) return 1;
		goto old;
	}
	if (Work_d - Work_g < 1) return 1;
old:
	Save_z = pEm->Sca_old_z;
	pEm->Atd[0].X = (int)pEm->Sca_old_x;
	pEm->Atd[0].Z = (int)Save_z;
	return 1;
}

/* DS 0x8004ea14 (Sca_ck_hishi) */
ULONG Sca_ck_hishi(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	USHORT Box_x;   /* X, loaded once as unsigned like DS (lhu) and sign-extended at each use */
	USHORT Box_z;   /* Z */
	UINT  Sx_end;   /* X + W; DS keeps the unextended sum and sign-extends at each use */
	UINT  Sz_end;   /* Z + D, same */
	short Cx;       /* diamond centre X: X + W / 2 */
	short Cz;       /* diamond centre Z: Z + D / 2 */
	long  Dx;       /* pPos->vx - Cx */
	long  Dz;       /* pPos->vz - Cz */
	UINT  Quad;     /* bit 0: Dx < 0, bit 1: Dz < 0 */
	long  Num;
	long  Den;
	long  Quot;
	ULONG Ret;

	Box_x = pT_xz->X;
	Box_z = pT_xz->Z;
	Sx_end = (UINT)pT_xz->W + (UINT)Box_x;
	Sz_end = (UINT)pT_xz->D + (UINT)Box_z;
	Cx = (short)((UINT)Box_x + ((UINT)pT_xz->W >> 1));
	Cz = (short)((UINT)Box_z + ((UINT)pT_xz->D >> 1));
	Dx = pPos->vx - Cx;
	Dz = pPos->vz - Cz;
	Quad = (((UINT)Dz >> 30) & 2) | ((UINT)Dx >> 31);

	Ret = 0;
	if (Quad == 1) {           /* Dx < 0, Dz >= 0 */
		Num = ((short)Sz_end + R - Cz) * (pPos->vx - ((short)Box_x - R));
		Den = Cx - ((short)Box_x - R);
		DIV_S(Quot, Num, Den);
		Ret = (ULONG)(Dz < Quot);
	} else if (Quad < 2) {
		if (Quad == 0) {       /* Dx >= 0, Dz >= 0 */
			Num = (Cz - ((short)Sz_end + R)) * Dx;
			Den = ((short)Sx_end + R) - Cx;
			DIV_S(Quot, Num, Den);
			Ret = (ULONG)((pPos->vz - ((short)Sz_end + R)) < Quot);
		}
	} else if (Quad == 2) {    /* Dx >= 0, Dz < 0 */
		Num = (Cz - ((short)Box_z - R)) * Dx;
		Den = ((short)Sx_end + R) - Cx;
		DIV_S(Quot, Num, Den);
		if (Quot < (pPos->vz - ((short)Box_z - R))) {
			Ret = 1;
		}
	} else if (Quad == 3) {    /* Dx < 0, Dz < 0 */
		Num = (((short)Box_z - R) - Cz) * (pPos->vx - ((short)Box_x - R));
		Den = Cx - ((short)Box_x - R);
		DIV_S(Quot, Num, Den);
		if (Quot < Dz) {
			Ret = 1;
		}
	}
	return Ret;
}

/* DS 0x8004ec50 (Sca_hit_circle) */
ULONG Sca_hit_circle(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long Half;    /* radius of the circular obstacle (W / 2) */
	long Dx;      /* enemy centre minus obstacle centre, X */
	long Dz;      /* enemy centre minus obstacle centre, Z */
	long Dist;
	long Pen;     /* penetration depth: (Half + R) - Dist */
	long Lim;     /* Half + R, formed before the SquareRoot0 call */

	Half = pT_xz->W >> 1;
	Dx = pEm->Atd[0].X - (pT_xz->X + Half);
	Dz = pEm->Atd[0].Z - (pT_xz->Z + Half);
	Lim = Half + R;
	Dist = SquareRoot0(Dx * Dx + Dz * Dz);
	Pen = Lim - Dist;
	if (Pen < 1) {
		return 0;
	}
	{
		long Quot;
		DIV_S(Quot, (Dx + 8) * Pen, Dist);
		pEm->Atd[0].X += Quot;
	}
	{
		long Quot;
		DIV_S(Quot, (Dz + 8) * Pen, Dist);
		pEm->Atd[0].Z += Quot;
	}
	return 1;
}

/* DS 0x8004ed84 (Sca_ck_circle) */
ULONG Sca_ck_circle(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long Half;    /* radius of the circular obstacle (W / 2) */
	long Dx;
	long Dz;
	long Dist;
	long Lim;     /* Half + R, formed before the SquareRoot0 call */

	Half = pT_xz->W >> 1;
	Dx = pPos->vx - (pT_xz->X + Half);
	Dz = pPos->vz - (pT_xz->Z + Half);
	Lim  = Half + R;
	Dist = SquareRoot0(Dx * Dx + Dz * Dz);
	return (ULONG)(0 < Lim - Dist);
}

/* DS 0x8004edf0 (Sca_hit_koban_x) */
ULONG Sca_hit_koban_x(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long  Half;   /* D / 2 */
	UINT  Zone;   /* bit 1: enemy X is left of (X + W - Half); bit 0: enemy X is left of (X + Half) */

	Half = pT_xz->D >> 1;
	Zone = (((UINT)(pEm->Atd[0].X - (short)(pT_xz->X + pT_xz->W - Half)) >> 30) & 2)
		 | ((UINT)(pEm->Atd[0].X - (short)(pT_xz->X + Half)) >> 31);

	switch (Zone) {
	case 0:
		/* Near end: circle at the lower-X end of the coin. */
		Tmp_Sca.X = (short)(pT_xz->W + pT_xz->X - pT_xz->D);
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.D = pT_xz->D;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
	case 1:
		return 1;
	case 2:
		return Sca_hit_box(pEm, R, pT_xz);
	case 3:
		/* Far end: circle at the upper-X end of the coin. */
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.D = pT_xz->D;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
	default:
		return 3;
	}
}

/* DS 0x8004ef0c (Sca_ck_koban_x) */
ULONG Sca_ck_koban_x(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long  Half;   /* D / 2 */
	UINT  Zone;   /* same zone split as Sca_hit_koban_x */

	Half = pT_xz->D >> 1;
	Zone = (((UINT)(pPos->vx - (short)(pT_xz->W + pT_xz->X - Half)) >> 30) & 2)
		 | ((UINT)(pPos->vx - (short)(pT_xz->X + Half)) >> 31);

	switch (Zone) {
	case 0:
		Tmp_Sca.X = (short)(pT_xz->W + pT_xz->X - pT_xz->D);
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.D = pT_xz->D;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.D = pT_xz->D;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	default:
		return 3;
	}
}

/* DS 0x8004f044 (Sca_hit_koban_z) */
ULONG Sca_hit_koban_z(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long  Half;   /* W / 2 */
	UINT  Zone;   /* bit 1: enemy Z is below (Z + D - Half); bit 0: below (Z + Half) */

	Half = pT_xz->W >> 1;
	Zone = (((UINT)(pEm->Atd[0].Z - (short)(pT_xz->D + pT_xz->Z - Half)) >> 30) & 2)
		 | ((UINT)(pEm->Atd[0].Z - (short)(pT_xz->Z + Half)) >> 31);

	switch (Zone) {
	case 0:
		/* Near end: circle at the lower-Z end of the coin. */
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = (short)(pT_xz->D + pT_xz->Z - pT_xz->W);
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.D = pT_xz->W;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
	case 1:
		return 1;
	case 2:
		return Sca_hit_box(pEm, R, pT_xz);
	case 3:
		/* Far end: circle at the upper-Z end of the coin. */
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.D = pT_xz->W;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
	default:
		return 3;
	}
}

/* DS 0x8004f17c (Sca_ck_koban_z) */
ULONG Sca_ck_koban_z(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	long  Half;   /* W / 2 */
	UINT  Zone;   /* same zone split as Sca_hit_koban_z */

	Half = pT_xz->W >> 1;
	Zone = (((UINT)(pPos->vz - (short)(pT_xz->D + pT_xz->Z - Half)) >> 30) & 2)
		 | ((UINT)(pPos->vz - (short)(pT_xz->Z + Half)) >> 31);

	switch (Zone) {
	case 0:
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = (short)(pT_xz->D + pT_xz->Z - pT_xz->W);
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.D = pT_xz->W;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.D = pT_xz->W;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	default:
		return 3;
	}
}

/* DS 0x8004f2ac (Sca_hit_slope) */
ULONG Sca_hit_slope(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long  Along;   /* enemy position along the slope's rising axis */
	long  Start;   /* low end of the slope on that axis */
	long  Edge;    /* high end of the slope on that axis */
	long  Cross;   /* enemy extent on the other axis, used for the overlap test */
	long  High;
	UINT  Drop;
	long  Pos_y;

	/* DS calls Sca_get_slope_high first and discards the result (the call is repeated below
	 * when the enemy is on the slope); keeping it keeps the DS frame and saved registers. */
	Sca_get_slope_high(pEm->Atd[0].X, pEm->Atd[0].Z, R, pT_xz);

	/* DS reads Pos_y here, before the branches. */
	Pos_y = pEm->Pos_y;

	if (pT_xz->Type & 0x20) {
		/* Slope rises along Z: the enemy must overlap the box in X first. */
		if (pEm->Sca_old_x < pT_xz->X - R || pT_xz->X + pT_xz->W + R < pEm->Sca_old_x) {
			return Sca_hit_box(pEm, R, pT_xz);
		}
		Along = pEm->Atd[0].Z;
		Start = pT_xz->Z;
		Edge = pT_xz->Z + pT_xz->D;
	} else {
		/* Slope rises along X: the enemy must overlap the box in Z first. */
		Cross = pEm->Sca_old_z + pEm->Atd[0].Atw_z;
		if (Cross < pT_xz->Z - R || pT_xz->Z + pT_xz->D + R < Cross) {
			return Sca_hit_box(pEm, R, pT_xz);
		}
		Along = pEm->Atd[0].X;
		Start = pT_xz->X;
		Edge = pT_xz->X + pT_xz->W;
	}

	if (Along < Start || Along > Edge) {
		/* Off the slope: snap to the floor level beneath the enemy. */
		Drop = -((Pos_y - 900) / SCA_LEVEL_H);
		pEm->nFloor = (UCHAR)Drop;
		pEm->Pos_y = (Drop & 0xff) * -SCA_LEVEL_H;
		return 1;
	}

	/* On the slope: height from the slope profile plus the box's base level. */
	High = Sca_get_slope_high(pEm->Atd[0].X, pEm->Atd[0].Z, R, pT_xz)
	     + ((pT_xz->Type >> 6) & 0x1f) * -SCA_LEVEL_H;
	pEm->Pos_y = High;
	pEm->nFloor = -(char)(High / SCA_LEVEL_H);
	return 1;
}

/* DS 0x8004f49c (Sca_hit_stairs) */
ULONG Sca_hit_stairs(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	USHORT Flags;   /* Type >> 4: bit 0 selects the entry end, bit 1 means the stairs run along Z */
	long   Lo;      /* low edge of the stairs on the cross axis (X or Z) */
	long   Pos;     /* enemy position on that cross axis */
	long   Span;    /* box extent on that axis (W or D) */
	long   Rise;

	Flags = pT_xz->Type >> 4;
	if ((Flags & 2) == 0) {
		if (pT_xz->D - 500 < (UINT)(pEm->Atd[0].Z - (pT_xz->Z + 0xfa))) {
			return 0;
		}
		Lo = pT_xz->X;
		Pos = pEm->Atd[0].X;
		Span = pT_xz->W;
	} else {
		if (pT_xz->W - 500 < (UINT)(pEm->Atd[0].X - (pT_xz->X + 0xfa))) {
			return 0;
		}
		Lo = pT_xz->Z;
		Pos = pEm->Atd[0].Z;
		Span = pT_xz->D;
	}
	if ((UINT)(Pos - (Lo - 0x28a)) > (UINT)Span + 0x514) {
		return 0;
	}

	/* Stair state: rise over the box and the routine codes used by the enemy's stair logic. */
	Rise = Sca_get_high(pT_xz) - Sca_get_low(pT_xz);
	pEm->Routine_1 = 7;
	if (pEm->Routine_0 == 4) {
		pEm->Routine_1 = 0x1a;
	}
	pEm->Timer1 = -1 - (short)(Rise / SCA_LEVEL_H);
	pEm->Routine_2 = 0;
	pEm->Timer2 = 0;

	if ((Flags & 2) == 0) {
		if (pEm->Atd[0].X - pT_xz->X < (pT_xz->W >> 1)) {
			pEm->Timer0 = 0;
			if ((Flags & 1) != 0) {
				pEm->Timer2 = 1;
			}
			pEm->Atd[0].X = pT_xz->X - R;
		} else {
			pEm->Timer0 = 0x800;
			if ((Flags & 1) == 0) {
				pEm->Timer2 = 1;
			}
			pEm->Atd[0].X = pT_xz->X + pT_xz->W + R;
		}
	} else if (pEm->Atd[0].Z - pT_xz->Z < (pT_xz->D >> 1)) {
		pEm->Timer0 = 0xc00;
		if ((Flags & 1) != 0) {
			pEm->Timer2 = 1;
		}
		pEm->Atd[0].Z = pT_xz->Z - R;
	} else {
		pEm->Timer0 = 0x400;
		if ((Flags & 1) == 0) {
			pEm->Timer2 = 1;
		}
		pEm->Atd[0].Z = pT_xz->Z + pT_xz->D + R;
	}
	pEm->Be_flg |= 8;
	G.Status_flg |= STAT_PL_BUSY;
	return 1;
}

/* DS 0x8004f6f8 (Sca_hit_curve) */
ULONG Sca_hit_curve(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long Half;    /* radius of the curved box: D / 2 */
	long Dz;      /* box centre Z minus enemy Z, made positive */
	long Ex;      /* enemy Z offset from the box centre (Atw_z included) */
	long Ey;      /* enemy height offset from the arc */
	long Pen;     /* arc radius minus distance to the arc centre */
	long Dist;
	long Low;

	(void)R;      /* not used by this routine */
	Half = pT_xz->D >> 1;

	/* Lift the enemy toward the arc when it is far from the box centre line. */
	Dz = (pT_xz->Z + Half) - pEm->Atd[0].Z;
	if (Dz < 0) {
		Dz = -Dz;
	}
	if ((pT_xz->D >> 2) < Dz) {
		Dz = (Dz * 2 - Half) * 0xa0;
		{
			long Quot;
			DIV_S(Quot, Dz, Half);
			pEm->Pos_y += Quot;
		}
	}

	Ex = (pEm->Atd[0].Z + pEm->Atd[0].Atw_z) - (pT_xz->Z + Half);
	Low = Sca_get_low(pT_xz);
	Ey = pEm->Pos_y - (Low - Half);
	Dist = SquareRoot0(Ex * Ex + Ey * Ey);

	/* Pull the enemy onto the arc when it is more than 10 units off it. */
	Pen = Half - Dist;
	if (((Pen < 0) ? -Pen : Pen) > 10) {
		{
			long Quot;
			DIV_S(Quot, Ex * Pen, Dist + 1);
			pEm->Atd[0].Z += Quot;
		}
		{
			long Quot;
			DIV_S(Quot, Ey * Pen, Dist + 1);
			pEm->Pos_y += Quot;
		}
	}
	return 1;
}

/* DS divide sequences (zero and overflow checks) come from DIV_S in DIVCHK.H. */

/* DS 0x8004f8b8 (Sca_get_slope_high) */
long Sca_get_slope_high(long Pos_x, long Pos_z, long R, SCA_DATA *pT_xz)
{
	long High;
	long Run;
	long Num;
	long Den;

	(void)R;      /* not used: the result depends on the enemy position only */
	/* Each case takes Run, the signed distance of the enemy past the slope's low edge
	   along its axis, and divides by that axis extent (W or D). */
	/* DS shares one divide between cases 0 and 2 and another between cases 1 and 3, so each
	   pair is joined with a goto. */
	switch ((pT_xz->Type & 0x30) >> 4) {
	case 0:
		Run = Pos_x - (pT_xz->X + pT_xz->W);
		Den = pT_xz->W;
		goto neg_run;
	case 1:
		Num = (pT_xz->X - Pos_x) * SCA_LEVEL_H;
		Den = pT_xz->W;
		goto pos_run;
	case 2:
		Run = Pos_z - (pT_xz->Z + pT_xz->D);
		Den = pT_xz->D;
	neg_run:
		DIV_S(High, Run * -SCA_LEVEL_H, Den);
		break;
	default:
		Num = (pT_xz->Z - Pos_z) * SCA_LEVEL_H;
		Den = pT_xz->D;
	pos_run:
		DIV_S(High, -Num, Den);
		break;
	}
	if (High >= 0x709) {
		High = 0x708;
	}
	if (High < 0) {
		High = 0;
	}
	return High;
}

/* DS 0x8004fa28 (Sca_get_stairs_high) */
long Sca_get_stairs_high(VECTOR *pPos, long Low_y, long High_y, SCA_DATA *pT_xz)
{
	long Dist;   /* distance along the stair run, measured from the box's entry edge */
	long Ans;

	switch ((pT_xz->Type & 0x30) >> 4) {
	case 0:
		Dist = (pT_xz->X + pT_xz->W) - pPos->vx;
		break;
	case 1:
		Dist = pPos->vx - pT_xz->X;
		break;
	case 2:
		Dist = (pT_xz->Z + pT_xz->D) - pPos->vz;
		break;
	default:
		Dist = pPos->vz - pT_xz->Z;
		break;
	}
	/* Each 0x21c units of run rise 0x1c2; the result is clamped to the box's height difference. */
	Ans = (Dist / 0x21c) * 0x1c2;
	if (Low_y - High_y < Ans) {
		Ans = Low_y - High_y;
	}
	if (Ans < 0) {
		Ans = 0;
	}
	return Ans;
}

/* DS 0x8004fb38 (Sca_get_curve_high) */
long Sca_get_curve_high(VECTOR *pPos, long Low_y, long High_y, SCA_DATA *pT_xz)
{
	long Half;   /* radius of the curved surface: D / 2 */
	long Dz;     /* box centre Z minus enemy Z */

	(void)High_y;   /* not used by this routine */
	Half = pT_xz->D >> 1;
	Dz = (pT_xz->Z + Half) - pPos->vz;
	return (Low_y - Half) + SquareRoot0(Half * Half - Dz * Dz);
}

/* DS 0x8004fba0 (Sca_get_ground) */
long Sca_get_ground(VECTOR *pPos, long R, ULONG Att, ULONG Flg)
{
	SCA_HEAD *pHead;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	OBJ_MODEL_WORK *pOm;
	ULONG Area;
	long  Floor;
	long  Low_y;   /* -SCA_LEVEL_H for each empty floor bit below the lowest set bit of Floor */
	long  Base_y;  /* -SCA_LEVEL_H * ((Type >> 6) & 0x1f): base level of the entry */
	long  Top_y;   /* surface height of the entry under the enemy */

	pHead = (SCA_HEAD *)G.pRoom->pSca;
	pEnd = (SCA_DATA *)pHead + pHead->Num;
	Area = Sca_get_area(pPos->vx, pPos->vz, pHead->Cx, pHead->Cz);
	Ceiling = pHead->Ceiling;
	Gnd_y = 0;
	Gnd_low_y = 0;
	Ground_in_flg = (pPos->vy > 0) ? 1 : 0;
	Ceiling_in_pos[0] = Ceiling;

	/* Object models (only when Flg is 0): a model whose top is under the enemy is ground. */
	if (Flg == 0 && G.pObj_work_end != G.Ob_model) {
		for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++) {
			if ((pOm->Be_flg & 1) == 0) continue;
			if (pPos == (VECTOR *)&pOm->Pos_x) continue;
			Oma_set_ofs(pOm);
			if (Omd_in_check(pPos, pOm, R, 0) == 0) continue;

			Base_y = pOm->Atd[0].Y - pOm->Atd[0].At_h;
			if (Base_y < pPos->vy) {
				if (pOm->Atd[0].Y + pOm->Atd[0].At_h < pPos->vy) continue;
				Ground_in_flg |= 1;
				if (Base_y < Gnd_low_y) Gnd_low_y = Base_y;
				Base_y = pPos->vy - 1;
			}
			if (Base_y < Gnd_y) Gnd_y = (short)Base_y;
		}
	}

	/* Map entries: entry 0 is the header, so the walk starts at entry 1. */
	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz < pEnd; pT_xz++) {
		if ((pT_xz->Type & Area) == 0) continue;
		if ((UINT)(pPos->vx + R - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(R * 2)) continue;
		if ((UINT)(pPos->vz + R - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(R * 2)) continue;
		if ((pT_xz->Id & Att) == 0) continue;
		if ((pT_xz->Id & 0xf) == 10) continue;

		Low_y = 0;
		for (Floor = pT_xz->Floor; (Floor & 1) == 0; Floor >>= 1) {
			Low_y -= SCA_LEVEL_H;
		}
		Base_y = ((pT_xz->Type >> 6) & 0x1f) * -SCA_LEVEL_H;

		/* Surface height by shape. Shapes 1..8 are tested first; a miss skips the entry. */
		switch (pT_xz->Id & 0xf) {
		case 1:
			if (Sca_ck_naname_a(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 2:
			if (Sca_ck_naname_b(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 3:
			if (Sca_ck_naname_c(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 4:
			if (Sca_ck_naname_d(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 5:
			if (Sca_ck_hishi(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 6:
			if (Sca_ck_circle(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 7:
			if (Sca_ck_koban_x(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 8:
			if (Sca_ck_koban_z(pPos, R, pT_xz) == 0) continue;
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		case 11:
			Top_y = Base_y + Sca_get_slope_high(pPos->vx, pPos->vz, R, pT_xz);
			break;
		case 12:
			Top_y = Base_y + Sca_get_stairs_high(pPos, Low_y, Base_y, pT_xz);
			break;
		case 13:
			Top_y = Sca_get_curve_high(pPos, Low_y, Base_y, pT_xz);
			break;
		default:
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		}

		/* Entry top is under the enemy: nothing to land on. */
		if (Low_y >= pPos->vy) {
			if (Top_y < pPos->vy) {
				Ground_in_flg |= 1;
				if (Top_y < Gnd_low_y) Gnd_low_y = Top_y;
			} else {
				if (Top_y < Gnd_y) Gnd_y = (short)Top_y;
				if (Top_y > pPos->vy) continue;
			}
		}

		/* Ceiling: the enemy is below the entry's base. */
		if (pPos->vy < Low_y) {
			Ground_in_flg |= 2;
			if (Ceiling_in_pos[0] < Low_y) {
				Ceiling_in_pos[0] = Low_y;
			}
		} else if (Ceiling < Low_y) {
			Ceiling = Low_y;
		}
	}
	return Gnd_y;
}

/* DS 0x80050110 (Sca_get_dir) */
USHORT Sca_get_dir(VECTOR *pPos, SCA_DATA *pT_xz)
{
	long   Slope;   /* D * (pos.x - X) / W: the box's Z edge line at the enemy's X */
	USHORT Dir;

	DIV_S(Slope, pT_xz->D * (pPos->vx - pT_xz->X), pT_xz->W);
	Dir = 0;
	if (Slope < pPos->vz - pT_xz->Z) {
		Dir = 2;
		if (-Slope < pPos->vz - (pT_xz->Z + pT_xz->D)) {
			Dir = 3;
		}
	} else if (pPos->vz - (pT_xz->Z + pT_xz->D) < -Slope) {
		Dir = 1;
	}
	return Dir;
}

/* DS 0x800501bc (Sca_get_dir_super) */
long Sca_get_dir_super(VECTOR *pPos, SCA_DATA *pSca_dat, MATRIX *pSuper, ULONG Flg)
{
	long  Bx;     /* box X */
	long  Bz;     /* box Z */
	long  Bw;     /* box W */
	long  Bd;     /* box D */
	long  Hi;     /* Sca_get_high: top of the box */
	long  Span;   /* Sca_get_low - Sca_get_high: vertical extent of the box */
	ULONG Axis;
	USHORT Dir;

	Bx = pSca_dat->X;
	Bz = pSca_dat->Z;
	Bw = pSca_dat->W;
	Bd = pSca_dat->D;
	Hi = Sca_get_high(pSca_dat);
	Span = Sca_get_low(pSca_dat) - Hi;

	if (Flg == 0) {
		Axis = Get_axis(pSuper);
		if (Axis == 1) {
			return Sca_get_dir_super_tmp(pPos->vz, pPos->vy, Bz, Hi, Bd, Span);
		}
		if (Axis == 2) {
			return Sca_get_dir_super_tmp(pPos->vy, pPos->vx, Hi, Bx, Span, Bw);
		}
		if (Axis != 0) {
			return 2;
		}
		return Sca_get_dir_super_tmp(pPos->vx, pPos->vz, Bx, Bz, Bw, Bd);
	}

	/* Flg != 0: Get_axis2 selects one of six orientations. The jump table at 0x8001113c is
	 * indexed by Axis (0..5); odd axes negate the quadrant code, even axes return it as is.
	 * Axes 2/3 test the Z/Y plane, axes 4/5 the Y/X plane. */
	Axis = Get_axis2(pSuper);
	if (Axis > 5) {
		return 0;
	}
	switch (Axis) {
	case 0:
		return Sca_get_dir_super_tmp(pPos->vx, pPos->vz, Bx, Bz, Bw, Bd);
	case 1:
		Dir = Sca_get_dir_super_tmp(pPos->vx, pPos->vz, Bx, Bz, Bw, Bd);
		return -(UINT)Dir & 3;
	case 2:
		return Sca_get_dir_super_tmp(pPos->vz, pPos->vy, Bz, Hi, Bd, Span);
	case 3:
		Dir = Sca_get_dir_super_tmp(pPos->vz, pPos->vy, Bz, Hi, Bd, Span);
		return -(UINT)Dir & 3;
	case 4:
		return Sca_get_dir_super_tmp(pPos->vy, pPos->vx, Hi, Bx, Span, Bw);
	default:
		Dir = Sca_get_dir_super_tmp(pPos->vy, pPos->vx, Hi, Bx, Span, Bw);
		return -(UINT)Dir & 3;
	}
}

/* DS 0x800503bc (Sca_get_dir_super_tmp) */
USHORT Sca_get_dir_super_tmp(long X, long Z, long Sx, long Sz, long Sw, long Sd)
{
	long   Slope;   /* Sd * (X - Sx) / Sw */
	USHORT Dir;

	DIV_S(Slope, Sd * (X - Sx), Sw);
	Dir = 0;
	if (Slope < Z - Sz) {
		Dir = 2;
		if (-Slope < Z - (Sz + Sd)) {
			Dir = 3;
		}
	} else if (Z - (Sz + Sd) < -Slope) {
		Dir = 1;
	}
	return Dir;
}

/* DS 0x80050454 (Get_axis) */
ULONG Get_axis(MATRIX *pM)
{
	SVECTOR Sv;
	long    Ax;
	long    Az;

	Sv = Get_axis_vec;
	Rot_vector_super(0, pM, &Sv, &Sv);
	Ax = (Sv.vx < 0) ? -Sv.vx : Sv.vx;
	Az = (Sv.vz < 0) ? -Sv.vz : Sv.vz;
	if (Az > 0x4000) {
		return 2;
	}
	return (Ax > 0x4000) ? 1 : 0;
}

/* DS 0x800504dc (Get_axis2) */
ULONG Get_axis2(MATRIX *pM)
{
	SVECTOR Sv;
	ULONG   Axis;

	Sv = Get_axis2_vec;
	Rot_vector_super(0, pM, &Sv, &Sv);
	Axis = (Sv.vy < -0x4000) ? 1 : 0;
	if (Sv.vx > 0x4000) {
		Axis = 2;
	}
	if (Sv.vx < -0x4000) {
		Axis = 3;
	}
	if (Sv.vz > 0x4000) {
		Axis = 4;
	}
	if (Sv.vz < -0x4000) {
		Axis = 5;
	}
	return Axis;
}

/* DS 0x8005057c (Sca_get_pos) */
long Sca_get_pos(long Dir, SCA_DATA *pSca_dat)
{
	/* Edge coordinate of the box for Dir 0..3. Other values fall through as in the binary:
	 * Dir < 0 returns 1, Dir >= 4 returns 3. */
	switch (Dir) {
	case 0:
		return pSca_dat->X + pSca_dat->W;
	case 1:
		return pSca_dat->Z;
	case 2:
		return pSca_dat->X;
	case 3:
		return pSca_dat->Z + pSca_dat->D;
	default:
		return (Dir < 2) ? 1 : 3;
	}
}

/* DS 0x800505fc (Sca_get_pos_super) */
void Sca_get_pos_super(VECTOR *pPos, SCA_DATA *pSca_dat, MATRIX *pSuper, VECTOR *pVec)
{
	long  Bx;     /* box X */
	long  Bz;     /* box Z */
	long  Bw;     /* box W */
	long  Bd;     /* box D */
	long  Hi;     /* Sca_get_high */
	long  Span;   /* Sca_get_low - Sca_get_high */
	ULONG Pad[2]; /* unused 8-byte stack slot the binary reserves (it copies an 8-byte constant
	                 from 0x80011164 into it and never reads it); kept for the frame size only */

	(void)Pad;
	Bx = pSca_dat->X;
	Bz = pSca_dat->Z;
	Bw = pSca_dat->W;
	Bd = pSca_dat->D;
	Hi = Sca_get_high(pSca_dat);
	Span = Sca_get_low(pSca_dat) - Hi;

	switch (Get_axis(pSuper)) {
	case 0:
		Sca_get_pos_super_tmp(pPos->vx, pPos->vz, Bx, Bz, Bw, Bd, &pVec->vx, &pVec->vz);
		break;
	case 1:
		Sca_get_pos_super_tmp(pPos->vy, pPos->vz, Hi, Bz, Span, Bd, &pVec->vy, &pVec->vz);
		break;
	case 2:
		Sca_get_pos_super_tmp(pPos->vx, pPos->vy, Bx, Hi, Bw, Span, &pVec->vx, &pVec->vy);
		break;
	default:
		break;
	}
}

/* DS 0x8005075c (Sca_get_pos_super_tmp) */
USHORT Sca_get_pos_super_tmp(long X, long Z, long Sx, long Sz, long Sw, long Sd, long *pX, long *pZ)
{
	long Edge;
	long Slope;
	USHORT Dir;

	/* Same quadrant test as Sca_get_dir_super_tmp, inlined as in the retail function
	 * (no call); writes the box edge that the quadrant selects and returns that edge
	 * (or the quadrant code for Dir 1 and 2). */
	DIV_S(Slope, Sd * (X - Sx), Sw);
	Dir = 0;
	if (Slope < Z - Sz) {
		Dir = 2;
		if (-Slope < Z - (Sz + Sd)) {
			Dir = 3;
		}
	} else if (Z - (Sz + Sd) < -Slope) {
		Dir = 1;
	}
	switch (Dir) {
	case 0:
		Edge = Sx + Sw;
		*pX = Edge;
		return (USHORT)Edge;
	case 1:
		*pZ = Sz;
		return 1;
	case 2:
		*pX = Sx;
		return 3;
	default:
		Edge = Sz + Sd;
		*pZ = Edge;
		return (USHORT)Edge;
	}
}

/* DS 0x80050858 (Sca_ck_line) */
ULONG Sca_ck_line(VECTOR *pV1, VECTOR *pV2, ULONG Att, ULONG Flg)
{
	/* Entry fields are read through named members: X, Z, W (width), D (depth), Id, Type,
	 * Floor. The cross-product sign tests were checked term by term against the disassembly. */
	SCA_HEAD *pHead;
	SCA_DATA *pSca;
	SCA_DATA *pEnd;
	long  Work_a, Work_b, Work_c, Work_d, Work_e, Top_y, Base_y, Work_f, Work_g, Span_y;
	UINT  Wid_x, Ext;
	long  Floor_bits;

	pHead = (SCA_HEAD *)G.pRoom->pSca;
	pEnd = (SCA_DATA *)pHead + pHead->Num;

	for (pSca = (SCA_DATA *)pHead + 1; pSca != pEnd; pSca++) {
		if ((pSca->Id & Att) == 0) continue;
		if (Sca_no_ck_tbl[pSca->Id & 0xf] != 0) continue;

		In_flg = 3;
		if (((UINT)(pV1->vx - pSca->X) < (UINT)pSca->W) && ((UINT)(pV1->vz - pSca->Z) < (UINT)pSca->D)) {
			In_flg = 2;
		} else if (((UINT)(pV2->vx - pSca->X) < (UINT)pSca->W) && ((UINT)(pV2->vz - pSca->Z) < (UINT)pSca->D)) {
			/* DS reloads In_flg and flips bit 0 (3 -> 2) here, unlike the V1 branch which stores 2 */
			In_flg ^= 1;
		} else {
			Top_y = pSca->Z;
			Work_c = pV1->vz;
			Wid_x = pSca->W;
			Work_a = pSca->X;
			Work_b = pV1->vx;
			Ext = pSca->D;
			Base_y = pV2->vz;
			Work_g = pV2->vx;
			Work_f = Work_b - Work_g;
			Work_e = Work_c - Base_y;
			Work_d = Top_y + Ext;
			if ((-1 < (int)(((Wid_x * (Work_c - Top_y) - Ext * (Work_b - Work_a)) ^
							 (Wid_x * (Base_y - Top_y) - Ext * (Work_g - Work_a))) &
							((Work_f * (Top_y - Work_c) - Work_e * (Work_a - Work_b)) ^
							 (Work_f * (Work_d - Work_c) - Work_e * ((Work_a + Wid_x) - Work_b))))) &&
				(-1 < (int)(((Wid_x * (Work_d - Work_c) - Ext * (Work_b - Work_a)) ^
							 (Wid_x * (Work_d - Base_y) - Ext * (Work_g - Work_a))) &
							((Work_f * (Work_d - Work_c) - Work_e * (Work_a - Work_b)) ^
							 (Work_f * (Top_y - Work_c) - Work_e * ((Work_a + Wid_x) - Work_b))))))
				continue;
		}

		if (Flg == 0) return 1;

		/* Vertical extent of the entry: Floor bits set the base, Type sets the top. */
		Base_y = 0;
		for (Floor_bits = pSca->Floor; (Floor_bits & 1) == 0; Floor_bits >>= 1) {
			Base_y += -SCA_LEVEL_H;
		}
		Top_y = (pSca->Type >> 11) * -100 + ((pSca->Type >> 6) & 0x1f) * -SCA_LEVEL_H;

		if ((((UINT)(pV1->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV1->vz - pSca->Z) < (UINT)pSca->D)) ||
			(((UINT)(pV2->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV2->vz - pSca->Z) < (UINT)pSca->D))) {
			In_flg ^= 2;
		} else {
			Work_e = pV1->vy;
			Ext = pSca->D;
			Work_c = pSca->Z;
			Work_d = pV1->vz;
			Span_y = Base_y - Top_y;
			Work_b = pV2->vy;
			Work_a = pV2->vz;
			Work_g = Work_d - Work_a;
			Work_f = Work_e - Work_b;
			if ((-1 < (int)(((Ext * (Work_e - Top_y) - Span_y * (Work_d - Work_c)) ^
							 (Ext * (Work_b - Top_y) - Span_y * (Work_a - Work_c))) &
							((Work_g * (Top_y - Work_e) - Work_f * (Work_c - Work_d)) ^
							 (Work_g * (Base_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d))))) &&
				(-1 < (int)(((Ext * (Base_y - Work_e) - Span_y * (Work_d - Work_c)) ^
							 (Ext * (Base_y - Work_b) - Span_y * (Work_a - Work_c))) &
							((Work_g * (Base_y - Work_e) - Work_f * (Work_c - Work_d)) ^
							 (Work_g * (Top_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d))))))
				continue;
		}

		if ((((UINT)(pV1->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV1->vx - pSca->X) < (UINT)pSca->W)) ||
			(((UINT)(pV2->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV2->vx - pSca->X) < (UINT)pSca->W))) {
			if (In_flg != 0) return 1;
		} else {
			Work_e = pV1->vy;
			Ext = pSca->W;
			Work_c = pSca->X;
			Work_d = pV1->vx;
			Span_y = Base_y - Top_y;
			Work_b = pV2->vy;
			Work_a = pV2->vx;
			Work_g = Work_d - Work_a;
			Work_f = Work_e - Work_b;
			if (((int)(((Ext * (Work_e - Top_y) - Span_y * (Work_d - Work_c)) ^
						(Ext * (Work_b - Top_y) - Span_y * (Work_a - Work_c))) &
					   ((Work_g * (Top_y - Work_e) - Work_f * (Work_c - Work_d)) ^
						(Work_g * (Base_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d)))) < 0) ||
				((int)(((Ext * (Base_y - Work_e) - Span_y * (Work_d - Work_c)) ^
						(Ext * (Base_y - Work_b) - Span_y * (Work_a - Work_c))) &
					   ((Work_g * (Base_y - Work_e) - Work_f * (Work_c - Work_d)) ^
						(Work_g * (Top_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d)))) < 0)) {
				return 1;
			}
		}
	}
	return 0;
}

/* DS 0x80050f80 (Sca_get_high) */
long Sca_get_high(SCA_DATA *pSca_dat)
{
	/* Type: bits 6..10 = floor level count, bits 11..15 = extra steps (x100). */
	USHORT Type;

	Type = pSca_dat->Type;
	return (long)(Type >> 11) * -100 + (long)((Type >> 6) & 0x1f) * -SCA_LEVEL_H;
}

/* DS 0x80050fc8 (Sca_get_low) */
long Sca_get_low(SCA_DATA *pSca_dat)
{
	/* Each zero bit below the lowest set bit of Floor is one level down. */
	long  Floor;
	long  Low;

	Low = 0;
	for (Floor = pSca_dat->Floor; (Floor & 1) == 0; Floor >>= 1) {
		Low -= SCA_LEVEL_H;
	}
	return Low;
}

/* DS 0x80050ff4 (Sca_search) */
void Sca_search(ULONG Area, long Att, ULONG Floor)
{
	SCA_HEAD *pHead;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	SCA_DATA **pOut;   /* write position in G.pSca_hit_data; the global pSca_data is not changed */
	int Count;

	G.pSca_hit_data[0] = (SCA_DATA *)0x0;
	pOut = G.pSca_hit_data;
	Count = 0;
	pHead = (SCA_HEAD *)G.pRoom->pSca;
	pEnd = (SCA_DATA *)pHead + pHead->Num;

	/* Entry 0 is the header, so the scan starts at entry 1. The list is capped at 32 hits. */
	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz < pEnd; pT_xz++) {
		if ((pT_xz->Type & Area) == 0) continue;
		if ((pT_xz->Id & Att) == 0) continue;
		if ((pT_xz->Floor & Floor) == 0) continue;
		*pOut++ = pT_xz;
		Count++;
		*pOut = (SCA_DATA *)0x0;
		if (Count == 0x20) {
			return;
		}
	}
}

/* DS 0x800c3b80 (In_flg) */
ULONG In_flg = 0;

/* DS 0x800c3b84 (Tmp_Sca) */
SCA_DATA Tmp_Sca;

/* DS 0x800c3b6c (pSca_data) */
SCA_DATA** pSca_data = 0;

/* DS 0x800c3b70 (Old_x) */
LONG Old_x = 0;

/* DS 0x800c3b74 (Old_z) */
LONG Old_z = 0;

/* DS 0x800d7694 (pStairs) */
SCA_DATA* pStairs = 0;
