/*
 * SCA.C - Room collision: hit tests, ground height, floor search
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCA.C
 */

#include "TYPES.H"
#include "ROOM.H"
#include "DIVCHK.H"
#include <ASM.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include "SCA.H"
#include "EM_SUB.H"
#include "OM.H"
#include "OMA.H"
#include "SUB.H"
#include "flag.h"
#include "SPL_TBL.H"

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

/* Module statics. Old_x, Old_z, In_flg, pSca_data and Tmp_Sca are declared extern in
 * the owner headers at their DS addresses, so they are not redefined here. */
static short     Hit_flg_0;        /* 0x800c3b68 - hit flags accumulated by Sca_ck_hit */
static short     Hit_flg;          /* 0x800c3b78 - hit flags accumulated by Sca_ck_hit_om */

/* DS 0x800c3b7c (Gnd_y) */
SHORT Gnd_y;

/* DS 0x800dcbc8 (Ground_in_flg) */
long Ground_in_flg;

/* DS 0x800dcc2c (Ground_in_pos) - lowest ground offset under the position, found by Sca_get_ground (Beta name).
 * Oma_pl_updown_ck and the enemy overlays EM22, EM24, EM29 and EM34 read it. */
long Ground_in_pos;

/* DS 0x800d5be4 (Ceiling) - set by Sca_get_ground; retail keeps it as zero bytes in the image, so the zero initialiser keeps it in .data. */
long Ceiling;

/* DS 0x800ea198 (Ceiling_in_pos) - long[36]; only [0] is used (Sca_get_ground). Rows of four longs at +0x0, +0x10, ... +0x80. Bytes are zero in the image. */
long Ceiling_in_pos[36];

/* DS 0x8004c198 (Sca_get_area) */
ULONG Sca_get_area(long X, long Z, long Sx, long Sz)
{
	return (1 << ((UINT)(X - Sx) >> 0x1f)) << ((UINT)(Z - Sz) >> 0x1e & 2);
}

/* DS 0x8004c1bc (Sca_ck_hit) */
ULONG Sca_ck_hit(VECTOR *pPos, long R, ULONG Floor, ULONG Flg)
{
	ENEMY_WORK *pEm;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	ULONG Area;
	ULONG Id_mask;       /* Flg & 0xffff: entry must share at least one Id bit */
	ULONG Ck_flg;
	long  Pos_x;         /* pPos->vx + R, updated when R is recomputed or the body is pushed */
	long  Pos_z;         /* pPos->vz + R */
	long  Dx;
	long  Dz;

	Id_mask = Flg & 0xffff;
	Flg >>= 16;
	G.pSca_hit_data[0] = (SCA_DATA *)0x0;
	pSca_data = G.pSca_hit_data;
	pEm = G.pEm;
	pT_xz = (SCA_DATA *)G.pRoom->pSca;
	pEnd = pT_xz + ((SCA_HEAD *)pT_xz)->Num;
	Pos_x = pPos->vx;
	Pos_z = pPos->vz;
	Area = Sca_get_area(Pos_x, Pos_z, ((SCA_HEAD *)pT_xz)->Cx, ((SCA_HEAD *)pT_xz)->Cz);
	Pos_x += R;
	Pos_z += R;
	Hit_flg_0 = 0;

	goto next;
loop2:
	{
		if ((pT_xz->Type & Area) == 0) goto next;

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
			W_e = pEm->Atd[0].W;
			D_e = pEm->Atd[0].D;
			if (D_e < W_e) {
				R = D_e + ((((long)(W_e - D_e)) * rcos((short)Qa)) >> 12);
			} else {
				R = W_e + ((((long)(D_e - W_e)) * rsin((short)Qa)) >> 12);
			}
			Pos_x = pPos->vx + R;
			Pos_z = pPos->vz + R;
		}

		if ((UINT)(Pos_x - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(R * 2)) goto next;
		if ((UINT)(Pos_z - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(R * 2)) goto next;
		if ((pT_xz->Id & Id_mask) == 0) goto next;
		if ((pT_xz->Id & Flg) != 0) goto next;
		if ((pT_xz->Floor & Floor) == 0) goto next;

		*pSca_data = pT_xz;
		pSca_data++;
		*pSca_data = (SCA_DATA *)0x0;

		if (Flg != 0) {
			/* Excluded-Id pass: the shape handler moves the enemy out of the box. */
			Old_x = pEm->Atd[0].X;
			Old_z = pEm->Atd[0].Z;
			if (Sca_rtn_tbl[pT_xz->Id & 0xf](pEm, R, pT_xz) != 0) {
				Dx = pEm->Atd[0].X - Old_x;
				Dz = pEm->Atd[0].Z - Old_z;
				Hit_flg_0 |= 1;
				pEm->Pos_x += Dx;
				Pos_x += Dx;
				pEm->Base_pos_x += (short)Dx;
				pEm->Base_pos_z += (short)Dz;
				pEm->Pos_z += Dz;
				Pos_z += Dz;
				if (Dz != 0) {
					pEm->Be_flg |= 0x4000;
				}
			}
		} else {
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
		}
	}
next:
	pT_xz++;
	if (pT_xz != pEnd) goto loop2;
	return (ULONG)Hit_flg_0;
}

/* DS 0x8004c658 (Sca_ck_hit_om) */
ULONG Sca_ck_hit_om(OBJ_MODEL_WORK *pOm, ULONG Att, ULONG Flg)
{
	SCA_HEAD *pHead;
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	USHORT Area;
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

	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz != pEnd; pT_xz++) {
		if ((pT_xz->Type & Area) == 0) continue;
		if ((UINT)(Mid_x - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(W * 2)) continue;
		if ((UINT)(Mid_z - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(D * 2)) continue;
		if ((pT_xz->Id & 0x4000) == 0) continue;
		if ((pT_xz->Id & Flg) != 0) continue;
		if ((pT_xz->Floor & Floor_bit) == 0) continue;

		if ((pT_xz->Id & 0xf) != 10) {
			if (Flg != 0) {
				Sca_hit_box((ENEMY_WORK *)pOm, D, pT_xz);
				Mid_x = pOm->Pos_x + pOm->Atd[0].Atw_x + W;
				Mid_z = pOm->Pos_z + pOm->Atd[0].Atw_z + D;
			}
			Hit_flg |= 1;
		} else {
			Hit_flg |= 4;
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

	Far_x  = (pT_xz->W + pT_xz->X + pEm->Atd[0].W) - pEm->Atd[0].X + 1;
	Near_x = (pT_xz->X - pEm->Atd[0].W) - pEm->Atd[0].X - 1;
	Lim_x = Far_x;
	if (-Near_x < Far_x) {
		Lim_x = Near_x;
	}

	Far_z  = (pT_xz->D + pT_xz->Z + R) - pEm->Atd[0].Z + 1;
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
	/* retail reserves 8 more bytes of locals (Sca_hosei_box frame 8); never read */
	volatile int Rsv[2];
	long  Abs_x;
	long  Abs_z;
	UINT  Mode;      /* bit 0: the move on X points against the saved position, bit 1: same on Z */

	/* pOld is not read; the saved position comes from pEm->Sca_old_x/z. */
	(void)pOld;
	Mode = ((UINT)(X ^ (pEm->Atd[0].X - pEm->Sca_old_x)) >> 31)
		 | (((UINT)(Z ^ (pEm->Atd[0].Z - pEm->Sca_old_z)) >> 30) & 2);

	switch (Mode) {
	case 1:
		/* X only: slide on X when the X step is small. */
		Abs_x = (X < 0) ? -X : X;
		if (Abs_x < 0x191) {
			pEm->Atd[0].X += X;
			return 1;
		}
		break;
	case 2:
		/* Z only: slide on Z when the Z step is small. */
		Abs_z = (Z < 0) ? -Z : Z;
		if (Abs_z < 0x191) {
			pEm->Atd[0].Z += Z;
			return 1;
		}
		break;
	case 3:
		break;
	default:
		goto Agree;
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
	goto Revert;

Agree:
	/* Both steps agree in sign: slide along the axis with the larger step. */
	Abs_x = (X < 0) ? -X : X;
	Abs_z = (Z < 0) ? -Z : Z;
	if (Abs_x < Abs_z) {
		pEm->Atd[0].X += X;
	} else {
		pEm->Atd[0].Z += Z;
	}
	return 1;

Revert:
	pEm->Atd[0].X = pEm->Sca_old_x;
	pEm->Atd[0].Z = pEm->Sca_old_z;
	return 1;
}

/* DS 0x8004cbd0 (Sca_hit_naname_a) */
ULONG Sca_hit_naname_a(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	int Cz_far;
	ULONG Ret;
	int Rel_z;
	int Rel2;
	int Cz_near;
	int Cx_near;
	UINT Width_x;
	int X0;
	int Em_x;
	int Em_z;
	int Cx_span;
	int Cz_span;
	UINT Depth_z;
	int Z0;
	int Num1;
	int Num2;
	int Num3;
	int Num4;
	int Den1;
	int Den2;
	int Den3;
	int Diag_z;
	int Qa;
	int Qx;
	int Qz;
	int Tmp;
	int Cx1;

	Depth_z = (UINT)pT_xz->D;
	Em_x  = pEm->Atd[0].X;
	X0  = (int)pT_xz->X;
	Num1 = (R * -2 - Depth_z) * (Em_x - (X0 - R));
	Width_x  = (UINT)pT_xz->W;
	Den1  = Width_x + R * 2;
	DIV_S(Diag_z, Num1, Den1);
	Em_z = pEm->Atd[0].Z;
	Z0 = (int)pT_xz->Z;
	Rel_z  = Em_z - (Depth_z + Z0);
	if (Diag_z < Rel_z) {
		Cx_near  = (X0  / 0x12) * 0x10000 >> 0x10;
		Cz_far  = (int)(Depth_z + Z0) / 0x12;
		Cz_near  = (Z0 / 0x12) * 0x10000 >> 0x10;
		Cz_span = Cz_near - Cz_far;
		Cx1 = (int)(Width_x + X0) / 0x12;
		Cx_span = Cx1 - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) - Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) - Cx_near * Cz_near) + Cx1 * Cz_far < 0) {
			if (((UINT)(Em_x - X0) < Width_x + R) && ((UINT)(Em_z - Z0) < Depth_z + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Rel2 = Rel_z - Diag_z;
			Num2 = (Width_x + R) * Rel2;
			Den2  = Depth_z + R;
			DIV_S(Qa, Num2, Den2);
			Num3 = Qa * Rel2 * Rel2;
			Den3  = Qa * Qa + Rel2 * Rel2;
			DIV_S(Qx, Num3, Den3);
			Num4  = Qa * Qa * Rel2;
			DIV_S(Qz, Num4, Den3);
			Tmp = Qx;
			if (Qx < 0) Tmp = -Qx;
			if (Tmp < 0x191) {
				Tmp = Qz;
				if (Qz < 0) Tmp = -Qz;
				if (Tmp < 0x191) {
					pEm->Atd[0].X = Em_x  - Qx;
					pEm->Atd[0].Z -= Qz;
					if (-1 < (Em_x / 0x12 - Cx_near) * Cz_span - (Em_z / 0x12 - Cz_far) * Cx_span) {
						return 1;
					}
				}
			}
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)pEm->Sca_old_z;
			return 1;
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
	Slope = -pT_xz->D - R * 2;
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
	int Em_x;
	int Em_z;
	int Z1;
	int Z0;
	int X0;
	int X1;
	int Span_z;
	int Width;
	int Diag_z;
	int Rel_z;
	int Cx_near;
	int Cz_near;
	int Cz1;
	int Cx1;
	int Cz_span;
	int Cx_span;
	int Rel2;
	int Qa;
	int Qx;
	int Qz;
	int Num1;
	int Num2;
	int Den;
	int Tmp;
	ULONG Ret;

	Em_x  = pEm->Atd[0].X;
	Z1  = (int)(short)(pT_xz->D + pT_xz->Z);
	Z0 = (int)pT_xz->Z;
	Span_z = Z1 - (Z0 - R);
	X0  = (int)pT_xz->X;
	Num1 = Span_z * (Em_x - X0);
	X1  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Width  = (X1 + R) - X0;
	DIV_S(Diag_z, Num1, Width);
	Em_z = pEm->Atd[0].Z;
	Rel_z = Em_z - (Z0 - R);
	if (Diag_z < Rel_z) {
		Cx_near  = (X0  / 0x12) * 0x10000 >> 0x10;
		Cz_near  = (Z0 / 0x12) * 0x10000 >> 0x10;
		Cx1  = (X1  / 0x12) * 0x10000 >> 0x10;
		Cz1  = (Z1  / 0x12) * 0x10000 >> 0x10;
		Cz_span = Cz1 - Cz_near;
		Cx_span = Cx1 - Cx_near;
		if (((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) - Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) - Cx_near * Cz1) + Cx1 * Cz_near < 0) {
			if (((UINT)(Em_x - (X0 - R)) < (UINT)pT_xz->W + R) && ((UINT)(Em_z - Z0) < (UINT)pT_xz->D + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Rel2 = Rel_z - Diag_z;
			DIV_S(Qa, Width * Rel2, Span_z);
			Num1 = Qa * Rel2 * Rel2;
			Den = Qa * Qa + Rel2 * Rel2;
			DIV_S(Qx, Num1, Den);
			Num2 = Qa * Qa * Rel2;
			DIV_S(Qz, Num2, Den);
			Tmp = Qx;
			if (Qx < 0) Tmp = -Qx;
			if (Tmp < 0x191) {
				Tmp = Qz;
				if (Qz < 0) Tmp = -Qz;
				if (Tmp < 0x191) {
					pEm->Atd[0].X = Em_x + Qx;
					pEm->Atd[0].Z -= Qz;
					if (-1 < (Em_x  / 0x12 - Cx_near) * Cz_span - (Em_z / 0x12 - Cz_near) * Cx_span) {
						return 1;
					}
				}
			}
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)pEm->Sca_old_z;
			return 1;
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
	int Em_x;
	int Em_z;
	int Z0;
	int Z1;
	int X0;
	int X1;
	int Span_z;
	int Width;
	int Diag_z;
	int Cx_near;
	int Cz_near;
	int Cz1;
	int Cx1;
	int Cz_span;
	int Cx_span;
	int Rel2;
	int Qa;
	int Qx;
	int Qz;
	int Num1;
	int Num3;
	int Num4;
	int Den3;
	int Tmp;
	ULONG Ret;

	Em_x  = pEm->Atd[0].X;
	Z1  = (int)(short)(pT_xz->D + pT_xz->Z);
	Z0  = (int)pT_xz->Z;
	Span_z = (Z1 + R) - Z0;
	X0 = (int)pT_xz->X;
	Num1 = Span_z * (Em_x - (X0 - R));
	X1  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Width  = X1 - (X0 - R);
	DIV_S(Diag_z, Num1, Width);
	Em_z = pEm->Atd[0].Z;
	if (Em_z - Z0 < Diag_z) {
		Cx_near  = (X0 / 0x12) * 0x10000 >> 0x10;
		Cz_near  = (Z0  / 0x12) * 0x10000 >> 0x10;
		Cx1  = (X1  / 0x12) * 0x10000 >> 0x10;
		Cz1  = (Z1  / 0x12) * 0x10000 >> 0x10;
		Cz_span = Cz1 - Cz_near;
		Cx_span = Cx1 - Cx_near;
		if (0 < ((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) - Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) - Cx_near * Cz1) + Cx1 * Cz_near) {
			if (((UINT)(Em_x  - X0) < (UINT)pT_xz->W + R) && ((UINT)(Em_z - (Z0 - R)) < (UINT)pT_xz->D + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Rel2 = (Em_z - Z0) - Diag_z;
			DIV_S(Qa, Width * Rel2, Span_z);
			Den3 = Qa * Qa + Rel2 * Rel2;
			Num3 = Qa * Rel2 * Rel2;
			DIV_S(Qx, Num3, Den3);
			Num4 = Qa * Qa * Rel2;
			DIV_S(Qz, Num4, Den3);
			Tmp = Qx;
			if (Qx < 0) Tmp = -Qx;
			if (Tmp < 0x191) {
				Tmp = Qz;
				if (Qz < 0) Tmp = -Qz;
				if (Tmp < 0x191) {
					pEm->Atd[0].X = Em_x + Qx;
					pEm->Atd[0].Z -= Qz;
					if ((Em_x  / 0x12 - Cx_near) * Cz_span - (Em_z / 0x12 - Cz_near) * Cx_span < 1) {
						return 1;
					}
				}
			}
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)pEm->Sca_old_z;
			return 1;
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
	int Em_x;
	int Em_z;
	int Z0;
	int Z1;
	int Z_lim;
	int X0;
	int X1;
	int Span_x;
	int Diag_z;
	int Cx_near;
	int Cz_far;
	int Cz_near;
	int Cx1;
	int Cz_span;
	int Cx_span;
	int Rel2;
	int Den0;
	int Qa;
	int Qx;
	int Qz;
	int Num1;
	int Num3;
	int Num4;
	int Den3;
	int Tmp;
	ULONG Ret;

	Em_x = pEm->Atd[0].X;
	Z0  = (int)pT_xz->Z;
	Z1  = (int)(short)(pT_xz->D + pT_xz->Z);
	Z_lim = Z1 + R;
	X0  = (int)pT_xz->X;
	Num1 = (Z0 - Z_lim) * (Em_x - X0);
	X1  = (int)(((UINT)pT_xz->W + (UINT)(USHORT)pT_xz->X) * 0x10000) >> 0x10;
	Span_x  = (X1 + R) - X0;
	DIV_S(Diag_z, Num1, Span_x);
	Em_z = pEm->Atd[0].Z;
	if (Em_z - Z_lim < Diag_z) {
		Cx_near  = (X0 / 0x12) * 0x10000 >> 0x10;
		Cz_far  = (Z1 / 0x12) * 0x10000 >> 0x10;
		Cz_near  = (Z0 / 0x12) * 0x10000 >> 0x10;
		Cz_span = Cz_near - Cz_far;
		Cx1  = (X1 / 0x12) * 0x10000 >> 0x10;
		Cx_span = Cx1 - Cx_near;
		if (0 < ((Cz_span * (((int)pEm->Sca_old_x / 0x12) * 0x10000 >> 0x10) - Cx_span * (((int)pEm->Sca_old_z / 0x12) * 0x10000 >> 0x10)) - Cx_near * Cz_near) + Cx1 * Cz_far) {
			if (((UINT)(Em_x - (X0 - R)) < (UINT)pT_xz->W + R) && ((UINT)(Em_z - (Z0 - R)) < (UINT)pT_xz->D + R)) {
				Ret = Sca_hit_box(pEm, R, pT_xz);
				return Ret;
			}
		} else {
			Rel2 = (Em_z - Z_lim) - Diag_z;
			Den0 = Z_lim - Z0;
			DIV_S(Qa, Span_x * Rel2, Den0);
			Num3  = Qa * Rel2 * Rel2;
			Den3 = Qa * Qa + Rel2 * Rel2;
			DIV_S(Qx, Num3, Den3);
			Num4 = Qa * Qa * Rel2;
			DIV_S(Qz, Num4, Den3);
			Tmp = Qx;
			if (Qx < 0) Tmp = -Qx;
			if (Tmp < 0x191) {
				Tmp = Qz;
				if (Qz < 0) Tmp = -Qz;
				if (Tmp < 0x191) {
					pEm->Atd[0].X = Em_x - Qx;
					pEm->Atd[0].Z -= Qz;
					if ((Em_x / 0x12 - Cx_near) * Cz_span - (Em_z / 0x12 - Cz_far) * Cx_span < 1) {
						return 1;
					}
				}
			}
			pEm->Atd[0].X = (int)pEm->Sca_old_x;
			pEm->Atd[0].Z = (int)pEm->Sca_old_z;
			return 1;
		}
		return 1;
	}
	return 0;
}

/* DS 0x8004dde8 (Sca_ck_naname_d) */
ULONG Sca_ck_naname_d(VECTOR *pPos, long R, SCA_DATA *pT_xz)
{
	/* Diagonal-edge test written as one expression, as DS evaluates it in one block:
	 *   pos.z against the near (-Z) edge, inset by R
	 *   < (pos.x against the far (+X) edge, inset by R) * -(D + 2R) / (W + 2R).
	 * The slope numerator is -D - 2R (DS: subu from zero, then subtracts R << 1),
	 * and the quotient is a plain signed divide (DS keeps the divide-by-zero and
	 * overflow break checks that cc1psx emits for "/"). */
	return (ULONG)(pPos->vz - (pT_xz->Z - R) < (pPos->vx - (pT_xz->X + pT_xz->W + R)) * (-pT_xz->D - R * 2) / (pT_xz->W + R * 2));
}

/* DS 0x8004de64 (Sca_hit_hishi) */
ULONG Sca_hit_hishi(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	UINT Box_x;
	UINT Box_z;
	int Quad;
	int Em_x;
	int Em_z;
	int Ext_x;
	int Ext_z;
	short Cx;
	short Cz;
	int Zb;
	int Zl;
	int Xa;
	int Num;
	int Den;
	int Dx;
	int Dz;
	int Diag;
	int Zr;
	int Rel;
	int Qa;
	int Qx;
	int Qz;
	int Num3;
	int Num4;
	int Den3;
	int Tmp;
	int Zn;
	int Cxn;
	int Czn;
	int Chk;
	int Chk2;

	Em_z = pEm->Atd[0].Z;
	Em_x  = pEm->Atd[0].X;
	Ext_x = (UINT)pT_xz->W + (USHORT)pT_xz->X;
	Box_x  = (USHORT)pT_xz->X;
	Ext_z  = (UINT)pT_xz->D + (USHORT)pT_xz->Z;
	Box_z  = (USHORT)pT_xz->Z;
	Cx = (short)((UINT)Box_x + ((UINT)pT_xz->W >> 1));
	Cz = (short)((UINT)Box_z + ((UINT)pT_xz->D >> 1));
	Quad  = ((UINT)(Em_z - Cz) >> 0x1e & 2) | (UINT)(Em_x - Cx) >> 0x1f;

	switch (Quad) {
	case 0:
		Zb = Ext_z * 0x10000 >> 0x10;
		Zl = Zb + R;
		Num = (Cz - Zl) * ((Em_x + pEm->Atd[0].Atw_x) - Cx);
		Xa  = Ext_x * 0x10000 >> 0x10;
		Den = (Xa + R) - Cx;
		DIV_S(Diag, Num, Den);
		Zr = (Em_z + pEm->Atd[0].Atw_z) - Zl;
		Rel = Zr - Diag;
		if (Diag <= Zr) return 0;
		Dz = Zl - Cz;
		DIV_S(Qa, Den * Rel, Dz);
		Den3  = Qa * Qa + Rel * Rel;
		Num3 = Qa * Rel * Rel;
		DIV_S(Qx, Num3, Den3);
		Num4 = Qa * Qa * Rel;
		DIV_S(Qz, Num4, Den3);
		Tmp = Qx; if (Qx < 0) Tmp = -Qx;
		if (400 < Tmp) goto old;
		Tmp = Qz; if (Qz < 0) Tmp = -Qz;
		if (400 < Tmp) goto old;
		pEm->Atd[0].X = Em_x - Qx;
		Zn = pEm->Atd[0].Z - Qz;
		pEm->Atd[0].Z = Zn;
		Cxn = (Cx / 0x12) * 0x10000 >> 0x10;
		Czn = (Zb / 0x12) * 0x10000 >> 0x10;
		Chk = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Cxn) *
				(((Cz / 0x12) * 0x10000 >> 0x10) - Czn);
		Chk2 = ((Zn + pEm->Atd[0].Atw_z) / 0x12 - Czn) *
				(((Xa / 0x12) * 0x10000 >> 0x10) - Cxn);
		break;
	case 1:
		Zb  = Ext_z * 0x10000 >> 0x10;
		Den = (Zb + R) - Cz;
		Num = Den * ((Em_x + pEm->Atd[0].Atw_x) - ((short)Box_x - R));
		Dx  = Cx - ((short)Box_x - R);
		DIV_S(Diag, Num, Dx);
		Zr = (Em_z + pEm->Atd[0].Atw_z) - Cz;
		Rel = Zr - Diag;
		if (Diag <= Zr) return 0;
		DIV_S(Qa, Dx * Rel, Den);
		Den3  = Qa * Qa + Rel * Rel;
		Num3 = Qa * Rel * Rel;
		DIV_S(Qx, Num3, Den3);
		Num4 = Qa * Qa * Rel;
		DIV_S(Qz, Num4, Den3);
		Tmp = Qx; if (Qx < 0) Tmp = -Qx;
		if (400 < Tmp) goto old;
		Tmp = Qz; if (Qz < 0) Tmp = -Qz;
		if (400 < Tmp) goto old;
		pEm->Atd[0].X = Em_x + Qx;
		Zn = pEm->Atd[0].Z - Qz;
		pEm->Atd[0].Z = Zn;
		Cxn = ((int)pT_xz->X / 0x12) * 0x10000 >> 0x10;
		Czn = (Cz / 0x12) * 0x10000 >> 0x10;
		Chk = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Cxn) *
				(((Zb / 0x12) * 0x10000 >> 0x10) - Czn);
		Chk2 = ((Zn + pEm->Atd[0].Atw_z) / 0x12 - Czn) *
				(((Cx / 0x12) * 0x10000 >> 0x10) - Cxn);
		break;
	case 2:
		Dz  = Cz - ((short)Box_z - R);
		Num = Dz * ((Em_x + pEm->Atd[0].Atw_x) - Cx);
		Xa = Ext_x * 0x10000 >> 0x10;
		Den  = (Xa + R) - Cx;
		DIV_S(Diag, Num, Den);
		Zr = (Em_z + pEm->Atd[0].Atw_z) - ((short)Box_z - R);
		Rel = Zr - Diag;
		if (Zr <= Diag) return 0;
		DIV_S(Qa, Den * Rel, Dz);
		Den3 = Qa * Qa + Rel * Rel;
		Num3 = Qa * Rel * Rel;
		DIV_S(Qx, Num3, Den3);
		Num4 = Qa * Qa * Rel;
		DIV_S(Qz, Num4, Den3);
		Tmp = Qx; if (Qx < 0) Tmp = -Qx;
		if (400 < Tmp) goto old;
		Tmp = Qz; if (Qz < 0) Tmp = -Qz;
		if (400 < Tmp) goto old;
		pEm->Atd[0].X = Em_x + Qx;
		Zn = pEm->Atd[0].Z - Qz;
		pEm->Atd[0].Z = Zn;
		Czn = ((int)pT_xz->Z / 0x12) * 0x10000 >> 0x10;
		Cxn = (Cx / 0x12) * 0x10000 >> 0x10;
		Chk = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Cxn) *
				(((Cz / 0x12) * 0x10000 >> 0x10) - Czn) -
				((Zn + pEm->Atd[0].Atw_z) / 0x12 - Czn) *
				(((Xa / 0x12) * 0x10000 >> 0x10) - Cxn);
		if (-1 < Chk) return 1;
		goto old;
	case 3:
		Num = (((short)Box_z - R) - Cz) *
				 ((Em_x + pEm->Atd[0].Atw_x) - ((short)Box_x - R));
		Dx  = Cx - ((short)Box_x - R);
		DIV_S(Diag, Num, Dx);
		Zr = (Em_z + pEm->Atd[0].Atw_z) - Cz;
		Rel = Zr - Diag;
		if (Zr <= Diag) return 0;
		Dz = Cz - ((short)Box_z - R);
		DIV_S(Qa, Dx * Rel, Dz);
		Den3  = Qa * Qa + Rel * Rel;
		Num3 = Qa * Rel * Rel;
		DIV_S(Qx, Num3, Den3);
		Num4 = Qa * Qa * Rel;
		DIV_S(Qz, Num4, Den3);
		Tmp = Qx; if (Qx < 0) Tmp = -Qx;
		if (400 < Tmp) goto old;
		Tmp = Qz; if (Qz < 0) Tmp = -Qz;
		if (400 < Tmp) goto old;
		pEm->Atd[0].X = Em_x - Qx;
		Zn = pEm->Atd[0].Z - Qz;
		pEm->Atd[0].Z = Zn;
		Cxn = ((int)pT_xz->X / 0x12) * 0x10000 >> 0x10;
		Czn = (Cz / 0x12) * 0x10000 >> 0x10;
		Chk = ((pEm->Atd[0].X + (int)pEm->Atd[0].Atw_x) / 0x12 - Cxn) *
				((((int)pT_xz->Z / 0x12) * 0x10000 >> 0x10) - Czn) -
				((Zn + pEm->Atd[0].Atw_z) / 0x12 - Czn) *
				(((Cx / 0x12) * 0x10000 >> 0x10) - Cxn);
		if (-1 < Chk) return 1;
		goto old;
	default:
		goto old;
	}
	if (Chk - Chk2 < 1) return 1;
old:
	pEm->Atd[0].X = (int)pEm->Sca_old_x;
	pEm->Atd[0].Z = (int)pEm->Sca_old_z;
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

	Box_x = pT_xz->X;
	Box_z = pT_xz->Z;
	Sx_end = (UINT)pT_xz->W + (UINT)Box_x;
	Sz_end = (UINT)pT_xz->D + (UINT)Box_z;
	Cx = (short)((UINT)Box_x + ((UINT)pT_xz->W >> 1));
	Cz = (short)((UINT)Box_z + ((UINT)pT_xz->D >> 1));
	Dx = pPos->vx - Cx;
	Dz = pPos->vz - Cz;
	Quad = (((UINT)Dz >> 30) & 2) | ((UINT)Dx >> 31);

	switch (Quad) {
	case 0:                    /* Dx >= 0, Dz >= 0 */
		Num = (Cz - ((short)Sz_end + R)) * Dx;
		Den = ((short)Sx_end + R) - Cx;
		DIV_S(Quot, Num, Den);
		if ((pPos->vz - ((short)Sz_end + R)) < Quot) {
			return 1;
		}
		break;
	case 1:                    /* Dx < 0, Dz >= 0 */
		Num = ((short)Sz_end + R - Cz) * (pPos->vx - ((short)Box_x - R));
		Den = Cx - ((short)Box_x - R);
		DIV_S(Quot, Num, Den);
		if (Dz < Quot) {
			return 1;
		}
		break;
	case 2:                    /* Dx >= 0, Dz < 0 */
		Num = (Cz - ((short)Box_z - R)) * Dx;
		Den = ((short)Sx_end + R) - Cx;
		DIV_S(Quot, Num, Den);
		if (Quot < (pPos->vz - ((short)Box_z - R))) {
			return 1;
		}
		break;
	case 3:                    /* Dx < 0, Dz < 0 */
		Num = (((short)Box_z - R) - Cz) * (pPos->vx - ((short)Box_x - R));
		Den = Cx - ((short)Box_x - R);
		DIV_S(Quot, Num, Den);
		if (Quot < Dz) {
			return 1;
		}
		break;
	}
	return 0;
}

/* DS 0x8004ec50 (Sca_hit_circle) */
ULONG Sca_hit_circle(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long Half;    /* radius of the circular obstacle (W / 2) */
	long Dx;      /* enemy centre minus obstacle centre, X */
	long Dz;      /* enemy centre minus obstacle centre, Z */
	long Dist;
	long Pen;     /* Half + R before the SquareRoot0 call, then the penetration depth (Half + R) - Dist */

	Half = pT_xz->W >> 1;
	Dx = pEm->Atd[0].X - (pT_xz->X + Half);
	Dz = pEm->Atd[0].Z - (pT_xz->Z + Half);
	Pen = Half + R;
	Dist = SquareRoot0(Dx * Dx + Dz * Dz);
	Pen -= Dist;
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
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.X = (short)(pT_xz->W + pT_xz->X - pT_xz->D);
		goto Circle;
	case 2:
		return Sca_hit_box(pEm, R, pT_xz);
	case 3:
		/* Far end: circle at the upper-X end of the coin. */
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.X = pT_xz->X;
Circle:
		Tmp_Sca.D = pT_xz->D;
		Tmp_Sca.Z = pT_xz->Z;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
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
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.X = (short)(pT_xz->W + pT_xz->X - pT_xz->D);
		Tmp_Sca.D = pT_xz->D;
		Tmp_Sca.Z = pT_xz->Z;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	case 2:
		return 1;
	case 3:
		Tmp_Sca.W = pT_xz->D;
		Tmp_Sca.X = pT_xz->X;
		Tmp_Sca.D = pT_xz->D;
		Tmp_Sca.Z = pT_xz->Z;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
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
		Tmp_Sca.D = pT_xz->W;
		Tmp_Sca.Z = (short)(pT_xz->D + pT_xz->Z - pT_xz->W);
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.X = pT_xz->X;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
	case 2:
		return Sca_hit_box(pEm, R, pT_xz);
	case 3:
		/* Far end: circle at the upper-Z end of the coin. */
		Tmp_Sca.D = pT_xz->W;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.X = pT_xz->X;
		return Sca_hit_circle(pEm, R, &Tmp_Sca);
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
		Tmp_Sca.D = pT_xz->W;
		Tmp_Sca.Z = (short)(pT_xz->D + pT_xz->Z - pT_xz->W);
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.X = pT_xz->X;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	case 2:
		return 1;
	case 3:
		Tmp_Sca.D = pT_xz->W;
		Tmp_Sca.Z = pT_xz->Z;
		Tmp_Sca.W = pT_xz->W;
		Tmp_Sca.X = pT_xz->X;
		return Sca_ck_circle(pPos, R, &Tmp_Sca);
	}
}

/* DS 0x8004f2ac (Sca_hit_slope) */
ULONG Sca_hit_slope(ENEMY_WORK *pEm, long R, SCA_DATA *pT_xz)
{
	long  Along;   /* enemy position along the slope's rising axis */
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
		if (pT_xz->Z > Along) goto off_slope;
		Edge = pT_xz->Z + pT_xz->D;
	} else {
		/* Slope rises along X: the enemy must overlap the box in Z first. */
		Cross = pEm->Sca_old_z + pEm->Atd[0].Atw_z;
		if (Cross < pT_xz->Z - R || pT_xz->Z + pT_xz->D + R < Cross) {
			return Sca_hit_box(pEm, R, pT_xz);
		}
		Along = pEm->Atd[0].X;
		if (pT_xz->X > Along) goto off_slope;
		Edge = pT_xz->X + pT_xz->W;
	}

	if (Along > Edge) {
off_slope:
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
	long   Edge;    /* box-edge offset used by the range tests */

	Flags = pT_xz->Type >> 4;
	if ((Flags & 2) != 0) {
		Edge = pT_xz->X + 0xfa;
		if (pT_xz->W - 500 < (UINT)(pEm->Atd[0].X - Edge)) {
			return 0;
		}
		Lo = pT_xz->Z;
		Pos = pEm->Atd[0].Z;
		Span = pT_xz->D;
	} else {
		Edge = pT_xz->Z + 0xfa;
		if (pT_xz->D - 500 < (UINT)(pEm->Atd[0].Z - Edge)) {
			return 0;
		}
		Lo = pT_xz->X;
		Pos = pEm->Atd[0].X;
		Span = pT_xz->W;
	}
	Edge = Lo - 0x28a;
	if ((UINT)(Pos - Edge) > (UINT)Span + 0x514) {
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

	if ((Flags & 2) != 0) {
		if (pEm->Atd[0].Z - pT_xz->Z < (pT_xz->D >> 1)) {
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
	} else if (pEm->Atd[0].X - pT_xz->X < (pT_xz->W >> 1)) {
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
	long Run;
	long Num;
	long Den;
	long Den2;

	/* R is the result variable: the parameter value is never used as an input. Each case takes
	   Run, the signed distance of the enemy past the slope's low edge along its axis, and
	   divides by that axis extent (W or D). DS shares one divide between cases 0 and 2 and
	   another between cases 1 and 3, so each pair is joined with a goto. */
	switch ((pT_xz->Type & 0x30) >> 4) {
	case 0:
		Run = Pos_x - (pT_xz->X + pT_xz->W);
		Den = pT_xz->W;
		goto neg_run;
	case 1:
		Num = (pT_xz->X - Pos_x) * SCA_LEVEL_H;
		Den2 = pT_xz->W;
		goto pos_run;
	case 2:
		Run = Pos_z - (pT_xz->Z + pT_xz->D);
		Den = pT_xz->D;
	neg_run:
		DIV_S(R, Run * -SCA_LEVEL_H, Den);
		break;
	case 3:
		Num = (pT_xz->Z - Pos_z) * SCA_LEVEL_H;
		Den2 = pT_xz->D;
	pos_run:
		DIV_S(R, -Num, Den2);
		break;
	}
	if (R >= 0x709) {
		R = 0x708;
	}
	if (R < 0) {
		R = 0;
	}
	return R;
}

/* DS 0x8004fa28 (Sca_get_stairs_high) */
long Sca_get_stairs_high(VECTOR *pPos, long Low_y, long High_y, SCA_DATA *pT_xz)
{
	long Dist;   /* distance along the stair run, measured from the box's entry edge */
	long Dist2;
	long Ans;

	/* Each 0x21c units of run rise 0x1c2; the result is clamped to the box's height difference.
	   DS shares one conversion between cases 0 and 2 and another between cases 1 and 3, so each
	   pair is joined with a goto. */
	switch ((pT_xz->Type & 0x30) >> 4) {
	case 0:
		Dist = (pT_xz->X + pT_xz->W) - pPos->vx;
		goto from_far_edge;
	case 1:
		Dist2 = pPos->vx - pT_xz->X;
		goto from_near_edge;
	case 2:
		Dist = (pT_xz->Z + pT_xz->D) - pPos->vz;
	from_far_edge:
		Ans = (Dist / 0x21c) * 0x1c2;
		break;
	case 3:
		Dist2 = pPos->vz - pT_xz->Z;
	from_near_edge:
		Ans = (Dist2 / 0x21c) * 0x1c2;
		break;
	}
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
	SCA_DATA *pT_xz;
	SCA_DATA *pEnd;
	OBJ_MODEL_WORK *pOm;
	ULONG Area;
	long  Pos_x;   /* pPos->vx read once at entry; the map-entry box test uses this copy */
	long  Pos_z;   /* pPos->vz read once at entry; the map-entry box test uses this copy */
	long  Floor;
	long  Low_y;   /* -SCA_LEVEL_H for each empty floor bit below the lowest set bit of Floor */
	long  Om_y;    /* base of the object-model box under the enemy */
	long  Base_y;  /* -SCA_LEVEL_H * ((Type >> 6) & 0x1f): base level of the entry */
	long  Top_y;   /* surface height of the entry under the enemy */

	/* The X/Z position is copied before any call. Om_move_box passes &pOm->Atd[0] as pPos, and
	   Oma_set_ofs in the object-model pass below rewrites Atd[0].X/Z, so *pPos can change while
	   this routine runs. DS keeps X + R and Z + R from these entry reads (s6/s5) for the box
	   test of every map entry; the shape tests and Sca_get_slope_high read *pPos again. */
	Pos_x = pPos->vx;
	Pos_z = pPos->vz;
	pT_xz = (SCA_DATA *)G.pRoom->pSca;
	pEnd = pT_xz + ((SCA_HEAD *)pT_xz)->Num;
	Area = Sca_get_area(Pos_x, Pos_z, ((SCA_HEAD *)pT_xz)->Cx, ((SCA_HEAD *)pT_xz)->Cz);
	Pos_x += R;
	Pos_z += R;
	Ceiling = ((SCA_HEAD *)pT_xz)->Ceiling;
	Ceiling_in_pos[0] = Ceiling;
	Ground_in_flg = 0;
	Gnd_y = 0;
	Ground_in_pos = 0;
	if (pPos->vy > 0) {
		Ground_in_flg = 1;
	}

	/* Object models (only when Flg is 0): a model whose top is under the enemy is ground. */
	if (Flg == 0) {
		for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++) {
			if ((pOm->Be_flg & 1) == 0) continue;
			if (pPos == (VECTOR *)&pOm->Pos_x) continue;
			Oma_set_ofs(pOm);
			if (Omd_in_check(pPos, pOm, R, 0) == 0) continue;

			if (pOm->Atd[0].Y - pOm->Atd[0].At_h >= pPos->vy) {
				Om_y = pOm->Atd[0].Y - pOm->Atd[0].At_h;
			} else {
				if (pOm->Atd[0].Y + pOm->Atd[0].At_h < pPos->vy) continue;
				Ground_in_flg |= 1;
				Om_y = pOm->Atd[0].Y - pOm->Atd[0].At_h;
				if (Om_y < Ground_in_pos) Ground_in_pos = Om_y;
				Om_y = pPos->vy - 1;
			}
			if (Om_y < Gnd_y) Gnd_y = (short)Om_y;
		}
	}

	/* Map entries: entry 0 is the header, so the walk starts at entry 1. */
	goto next;
loop2:
	{
		if ((pT_xz->Type & Area) == 0) goto next;
		if ((UINT)(Pos_x - pT_xz->X) >= (UINT)pT_xz->W + (UINT)(R * 2)) goto next;
		if ((UINT)(Pos_z - pT_xz->Z) >= (UINT)pT_xz->D + (UINT)(R * 2)) goto next;
		if ((pT_xz->Id & Att) == 0) goto next;
		if ((pT_xz->Id & 0xf) == 10) goto next;

		Low_y = 0;
		for (Floor = pT_xz->Floor; (Floor & 1) == 0; Floor >>= 1) {
			Low_y -= SCA_LEVEL_H;
		}
		Base_y = ((pT_xz->Type >> 6) & 0x1f) * -SCA_LEVEL_H;

		/* Surface height by shape. Shapes 1..8 are tested first; a miss skips the entry. */
		switch (pT_xz->Id & 0xf) {
		case 1:
			if (Sca_ck_naname_a(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 2:
			if (Sca_ck_naname_b(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 3:
			if (Sca_ck_naname_c(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 4:
			if (Sca_ck_naname_d(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 5:
			if (Sca_ck_hishi(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 6:
			if (Sca_ck_circle(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 7:
			if (Sca_ck_koban_x(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 8:
			if (Sca_ck_koban_z(pPos, R, pT_xz) != 0) goto normal;
			goto next;
		case 10:
			goto next;
		case 11:
			Top_y = Base_y + Sca_get_slope_high(pPos->vx, pPos->vz, R, pT_xz);
			goto ground;
		case 12:
			Top_y = Base_y + Sca_get_stairs_high(pPos, Low_y, Base_y, pT_xz);
			goto ground;
		case 13:
			Top_y = Sca_get_curve_high(pPos, Low_y, Base_y, pT_xz);
			goto ground;
		case 0:
		default:
		normal:
			Top_y = Base_y - (pT_xz->Type >> 11) * 100;
			break;
		}
	ground:

		/* Entry top is under the enemy: nothing to land on. */
		if (Low_y >= pPos->vy) {
			if (Top_y < pPos->vy) {
				Ground_in_flg |= 1;
				if (Top_y < Ground_in_pos) Ground_in_pos = Top_y;
			} else {
				if (Top_y < Gnd_y) Gnd_y = (short)Top_y;
				if (Top_y > pPos->vy) goto next;
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
next:
	pT_xz++;
	if (pT_xz != pEnd) goto loop2;
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
	} else if (-Slope > pPos->vz - (pT_xz->Z + pT_xz->D)) {
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
	Hi = Sca_get_high(pSca_dat);
	Bz = pSca_dat->Z;
	Bw = pSca_dat->W;
	Span = Sca_get_low(pSca_dat) - Hi;
	Bd = pSca_dat->D;

	if (Flg != 0) {
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
	else {
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
	} else if (-Slope > Z - (Sz + Sd)) {
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
	ULONG   Axis;

	Sv = Get_axis_vec;
	Rot_vector_super(0, pM, &Sv, &Sv);
	Ax = (Sv.vx < 0) ? -Sv.vx : Sv.vx;
	Axis = (Ax > 0x4000) ? 1 : 0;
	Az = (Sv.vz < 0) ? -Sv.vz : Sv.vz;
	if (Az > 0x4000) {
		Axis = 2;
	}
	return Axis;
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
	/* Edge coordinate of the box for Dir 0..3. DS has no default case: any other Dir leaves the
	 * function with whatever v0 last held from the case compares (no return value is set). */
	switch (Dir) {
	case 0:
		return pSca_dat->X + pSca_dat->W;
	case 1:
		return pSca_dat->Z;
	case 2:
		return pSca_dat->X;
	case 3:
		return pSca_dat->Z + pSca_dat->D;
	}
}

/* DS 0x800505fc (Sca_get_pos_super) */
void Sca_get_pos_super(VECTOR *pPos, SCA_DATA *pSca_dat, MATRIX *pSuper, VECTOR *pVec)
{
	/* DS 0x80011164: 8-byte initialiser template in .rdata (a vector {0, -100, 0, 0}), block-copied into this local (lwl/lwr, swl/swr); DS never reads it back */
	SVECTOR Unused_vec = { 0, -100, 0, 0 };
	long  Bx;     /* box X */
	long  Bz;     /* box Z */
	long  Bw;     /* box W */
	long  Bd;     /* box D */
	long  Hi;     /* Sca_get_high */
	long  Span;   /* Sca_get_low - Sca_get_high */

	Bx = pSca_dat->X;
	Hi = Sca_get_high(pSca_dat);
	Bz = pSca_dat->Z;
	Bw = pSca_dat->W;
	Span = Sca_get_low(pSca_dat) - Hi;
	Bd = pSca_dat->D;

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
	long Slope;
	long Dir;

	/* Same quadrant test as Sca_get_dir_super_tmp, inlined as in the retail function
	 * (no call); writes the box edge that the quadrant selects. The retail function
	 * has no return statement: at its jr ra (0x80050850) v0 holds whatever the
	 * selected case left there, and no caller reads it. */
	DIV_S(Slope, Sd * (X - Sx), Sw);
	Dir = 0;
	if (Slope < Z - Sz) {
		Dir = 2;
		if (-Slope < Z - (Sz + Sd)) {
			Dir = 3;
		}
	} else if (-Slope > Z - (Sz + Sd)) {
		Dir = 1;
	}
	switch (Dir) {
	case 0:
		*pX = Sx + Sw;
		break;
	case 1:
		*pZ = Sz;
		break;
	case 2:
		*pX = Sx;
		break;
	case 3:
		*pZ = Sz + Sd;
		break;
	}
}

/* DS 0x80050858 (Sca_ck_line) */
ULONG Sca_ck_line(VECTOR *pV1, VECTOR *pV2, ULONG Att, ULONG Flg)
{
	/* Entry fields are read through named members: X, Z, W (width), D (depth), Id, Type,
	 * Floor. The cross-product sign tests follow the disassembly term by term. */
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
			Span_y = Work_a + Wid_x;
			if (-1 < (int)(((Wid_x * (Work_c - Top_y) - Ext * (Work_b - Work_a)) ^
							 (Wid_x * (Base_y - Top_y) - Ext * (Work_g - Work_a))) &
							((Work_f * (Top_y - Work_c) - Work_e * (Work_a - Work_b)) ^
							 (Work_f * (Work_d - Work_c) - Work_e * (Span_y - Work_b))))) {
				/* DS evaluates the second conjunct of each cross-product test in full: all 16 products are
				 * multiplied again and none is taken from the first conjunct. The empty asm statements
				 * redefine the operands so that cc1psx cannot share the products between the two tests. */
				__asm__ volatile ("" : "=r"(Wid_x), "=r"(Ext), "=r"(Work_a), "=r"(Top_y), "=r"(Work_b) : "0"(Wid_x), "1"(Ext), "2"(Work_a), "3"(Top_y), "4"(Work_b));
				__asm__ volatile ("" : "=r"(Work_c), "=r"(Work_g), "=r"(Base_y), "=r"(Work_f), "=r"(Work_e) : "0"(Work_c), "1"(Work_g), "2"(Base_y), "3"(Work_f), "4"(Work_e));
				__asm__ volatile ("" : "=r"(Work_d) : "0"(Work_d));
				if (-1 < (int)(((Wid_x * (Work_d - Work_c) - Ext * (Work_b - Work_a)) ^
								 (Wid_x * (Work_d - Base_y) - Ext * (Work_g - Work_a))) &
								((Work_f * (Work_d - Work_c) - Work_e * (Work_a - Work_b)) ^
								 (Work_f * (Top_y - Work_c) - Work_e * (Span_y - Work_b)))))
					continue;
			}
		}

		if (Flg == 0) return 1;

		/* Vertical extent of the entry: Floor bits set the base, Type sets the top. */
		Base_y = 0;
		for (Floor_bits = pSca->Floor; (Floor_bits & 1) == 0; Floor_bits >>= 1) {
			Base_y += -SCA_LEVEL_H;
		}
		Top_y = (pSca->Type >> 11) * -100 + ((pSca->Type >> 6) & 0x1f) * -SCA_LEVEL_H;

		if ((((UINT)(pV1->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV1->vz - pSca->Z) < (UINT)pSca->D)) || (((UINT)(pV2->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV2->vz - pSca->Z) < (UINT)pSca->D))) {
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
			if (-1 < (int)(((Ext * (Work_e - Top_y) - Span_y * (Work_d - Work_c)) ^
							 (Ext * (Work_b - Top_y) - Span_y * (Work_a - Work_c))) &
							((Work_g * (Top_y - Work_e) - Work_f * (Work_c - Work_d)) ^
							 (Work_g * (Base_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d))))) {
				__asm__ volatile ("" : "=r"(Ext), "=r"(Work_c), "=r"(Work_d), "=r"(Work_e), "=r"(Work_f) : "0"(Ext), "1"(Work_c), "2"(Work_d), "3"(Work_e), "4"(Work_f));
				__asm__ volatile ("" : "=r"(Work_g), "=r"(Top_y), "=r"(Base_y), "=r"(Span_y), "=r"(Work_a) : "0"(Work_g), "1"(Top_y), "2"(Base_y), "3"(Span_y), "4"(Work_a));
				__asm__ volatile ("" : "=r"(Work_b) : "0"(Work_b));
				if (-1 < (int)(((Ext * (Base_y - Work_e) - Span_y * (Work_d - Work_c)) ^
							 (Ext * (Base_y - Work_b) - Span_y * (Work_a - Work_c))) &
							((Work_g * (Base_y - Work_e) - Work_f * (Work_c - Work_d)) ^
							 (Work_g * (Top_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d)))))
					continue;
			}
		}

		if ((((UINT)(pV1->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV1->vx - pSca->X) < (UINT)pSca->W)) || (((UINT)(pV2->vy - Top_y) < (UINT)(Base_y - Top_y)) && ((UINT)(pV2->vx - pSca->X) < (UINT)pSca->W))) {
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
						(Work_g * (Base_y - Work_e) - Work_f * ((Work_c + Ext) - Work_d)))) < 0)) {
				return 1;
			}
				__asm__ volatile ("" : "=r"(Ext), "=r"(Work_c), "=r"(Work_d), "=r"(Work_e), "=r"(Work_f) : "0"(Ext), "1"(Work_c), "2"(Work_d), "3"(Work_e), "4"(Work_f));
				__asm__ volatile ("" : "=r"(Work_g), "=r"(Top_y), "=r"(Base_y), "=r"(Span_y), "=r"(Work_a) : "0"(Work_g), "1"(Top_y), "2"(Base_y), "3"(Span_y), "4"(Work_a));
				__asm__ volatile ("" : "=r"(Work_b) : "0"(Work_b));
				if (((int)(((Ext * (Base_y - Work_e) - Span_y * (Work_d - Work_c)) ^
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
	long   Level; /* height of the floor levels: level count * SCA_LEVEL_H */

	/* DS multiplies the level count out first (v1), then the step count (v0), and ends with
	 * v0 = -v0 - v1; holding the level height in its own local gives that order. */
	Type = pSca_dat->Type;
	Level = ((Type >> 6) & 0x1f) * SCA_LEVEL_H;
	return (Type >> 11) * -100 - Level;
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

	pHead = (SCA_HEAD *)G.pRoom->pSca;
	G.pSca_hit_data[0] = (SCA_DATA *)0x0;
	pOut = G.pSca_hit_data;
	Count = 0;
	pEnd = (SCA_DATA *)pHead + pHead->Num;

	/* Entry 0 is the header, so the scan starts at entry 1. The list is capped at 32 hits. */
	for (pT_xz = (SCA_DATA *)pHead + 1; pT_xz != pEnd; pT_xz++) {
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
ULONG In_flg;

/* DS 0x800c3b84 (Tmp_Sca) */
SCA_DATA Tmp_Sca;

/* DS 0x800c3b6c (pSca_data) */
SCA_DATA** pSca_data;

/* DS 0x800c3b70 (Old_x) */
LONG Old_x;

/* DS 0x800c3b74 (Old_z) */
LONG Old_z;

/* DS 0x800d7694 (pStairs) */
SCA_DATA* pStairs;
