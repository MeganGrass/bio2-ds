/*
 * ROOT_CK.C - Enemy route check across room areas
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ROOT_CK.C
 */

#include "TYPES.H"
#include "ROOM.H"
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include "ROOT_CK.H"
#include "EM_SUB.H"
#include "SCA.H"
#include "ESP_R.H"
#include "DIVCHK.H"

/* Room block layout per area (6 shorts): [0] area count (first short only),
 * [2] x0, [3] z0, [4] x1, [5] z1, [7] neighbour bit mask.
 */

/* DS 0x8004a808 (Root_ck) */
UCHAR Root_ck(ENEMY_WORK *pEm, VECTOR *Dest, UCHAR Dest_area, UCHAR Mode)
{
	UCHAR   from_area;
	UCHAR   to_area;
	UINT    area;
	ULONG   line_hit;
	UCHAR   cnt;
	short  *pBlock;
	LONG    target_x;
	LONG    target_z;

	if ((pEm->Root_ck_cnt & 0x7f) == 0) {
		pEm->Root_ck_cnt |= 7;
	}
	cnt = pEm->Root_ck_cnt - 1;
	pEm->Root_ck_cnt = cnt;
	if ((cnt & 0x80) == 0) {
		pEm->Status_flg &= 0xf7ff;
		line_hit = Sca_ck_line((VECTOR *)((int)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t), (VECTOR *)((int)G.Player.pSin_parts_ptr[G.Player.Neck_no].Workm.t), 0x2000, 1);
		if (line_hit == 0) {
			pEm->Status_flg |= 0x800;
		}
	}

	if ((pEm->Type & 0x1000) != 0) {
		pEm->Dest_x = (short)Dest->vx;
		pEm->Dest_z = (short)Dest->vz;
		return 2;
	}
	if (((pEm->Type & 0x4000) != 0) || ((pEm->Root_ck_cnt & 0x80) != 0)) {
		return 2;
	}
	from_area = Guide_check((short)pEm->Pos_x, (short)pEm->Pos_z);
	target_x  = Dest->vx;
	target_z  = Dest->vz;
	to_area   = (UCHAR)Guide_check((short)Dest->vx, (short)Dest->vz);
	area      = to_area;
	if (((from_area != 0xff) && (area != 0xff)) || (Mode != 0)) {
		if (Mode != 0) {
			pBlock   = (G.pRoom)->pBlock;
			area     = Dest_area;
			target_x = (pBlock[area * 6 + 2] + pBlock[area * 6 + 4]) >> 1;
			target_z = (pBlock[area * 6 + 3] + pBlock[area * 6 + 5]) >> 1;
		}
		if (from_area == area) {
			pEm->Dest_x = (short)target_x;
			pEm->Dest_z = (short)target_z;
			return 1;
		}
		if ((G.pEm)->Root_ck_cnt != 0) {
			return 2;
		}
		kara_rootck2(from_area, (UCHAR)area, (short)target_x, (short)target_z);
		return 1;
	}
	return Mode;
}

/* DS 0x8004aa50 (Rnd_area) */
UCHAR Rnd_area(void)
{
	UINT   count;

	count = (UINT)(USHORT)*(G.pRoom)->pBlock;
	if (count == 0) {
		return 0xff;
	}
	/* DS divides Rnd()'s full return word (no andi before the divu); the call goes through a UINT-returning type. */
	return (UCHAR)(((UINT (*)(VOID))Rnd)() % count);
}

/* DS 0x8004aab8 (Guide_check) : area containing (target_x, target_z), or 0xff */
UCHAR Guide_check(short target_x, short target_z)
{
	USHORT *pBlock;
	USHORT *pX0;
	USHORT *pZ0;
	UINT    cnt;

	pBlock = (USHORT *)(G.pRoom)->pBlock;
	cnt    = *pBlock;
	pX0    = pBlock + cnt * 6 + 2;
	pZ0    = pBlock + cnt * 6 - 3;
	while (1) {
		pX0 -= 6;
		cnt--;
		if (((USHORT)(target_x - *pX0) < (USHORT)(pZ0[1] - *pX0)) && ((USHORT)(target_z - *pZ0) < (USHORT)(pZ0[2] - *pZ0)))
			break;
		pZ0 -= 6;
		if ((cnt & 0xffff) == 0) {
			return 0xff;
		}
	}
	return (UCHAR)cnt;
}

/* DS 0x8004ab60 (kara_rootck2) : best-path search from hontai_area to target_area (DFS over the room graph) */
void kara_rootck2(UCHAR hontai_area, UCHAR target_area, short target_x, short target_z)
{
	/* retail reserves 48 more bytes of locals (kara_rootck2 frame 584); never read */
	volatile int Rsv[12];
	USHORT  open_st[16];		/* per depth: open neighbour bits (block[7]) */
	USHORT *open_pt;
	USHORT  visit_st[16];		/* per depth: visited area bits */
	USHORT *visit_pt;
	USHORT *visit_nx;
	USHORT  cost_st[16];		/* per depth: accumulated path cost */
	USHORT *cost_pt;
	USHORT *cost_nx;
	short   poxz_st[34];		/* per depth x/z pairs; [2..5] = best node mid + dest */
	short  *poxz_pt;
	short  *poxz_nx;
	short  *poxz_b;
	UCHAR   root[16];
	UCHAR   sflg[16];
	UCHAR  *sflg_pt;
	short   MinMax[64];
	short  *mm_pt;
	short  *mm_b;
	short  *mm_b_nx;
	short  *pBlock;
	ENEMY_WORK *pEm;

	UINT    node;
	UINT    depth;
	UINT    parent;
	UINT    nb;
	USHORT  target_bit;
	USHORT  open_bits;
	UCHAR   flag;
	short   e0;
	short   z0;
	short   e1;
	short   z1;
	short   sx;
	short   sz;
	int     d_a;
	int     d_b;
	long    dist_a;
	long    dist_b;
	int     cost;
	int     best_cost;
	int     way_x;
	int     way_z;
	int     dst_x;
	int     dst_z;
	char    best_flg;
	int     d;
	int     slope;
	int     tmp;
	int     off_x;
	int     off_z;
	long    dest_x;
	long    dest_z;
	VECTOR  Tv;
	VECTOR  Tv0;
	VECTOR  Tv1;

	node      = hontai_area;
	best_flg  = 0;
	way_x     = 0;
	way_z     = 0;
	dst_x     = 0;
	dst_z     = 0;
	open_pt   = open_st;
	poxz_pt   = poxz_st;
	mm_pt     = MinMax;
	sflg_pt   = sflg;
	depth     = 0;
	dest_x    = target_x;
	dest_z    = target_z;
	best_cost = 0x7fffffff;
	target_bit = (USHORT)(1 << (target_area & 0x1f));
	pBlock    = (G.pRoom)->pBlock;
	pEm       = G.pEm;
	visit_st[0] = (USHORT)(1 << (hontai_area & 0x1f));
	cost_st[0]  = 0;
	root[0]     = hontai_area;
	poxz_st[0]  = (short)(G.pEm)->Pos_x;
	poxz_st[1]  = (short)(G.pEm)->Pos_z;
	open_st[0]  = pBlock[(UINT)hontai_area * 6 + 7];

	if (pBlock[(UINT)hontai_area * 6 + 7] != 0) {
		visit_pt = visit_st;
		poxz_b   = poxz_st + 1;
		mm_b     = MinMax + 3;
		cost_pt  = cost_st;

		Search_loop:
		{
			open_bits = *open_pt;
			if (open_bits == 0) {
				/* pop */
				open_pt--;
				depth--;
				node     = root[depth & 0xff];
				sflg_pt--;
				poxz_pt -= 2;
				visit_nx = visit_pt - 1;
				mm_pt   -= 4;
				poxz_nx  = poxz_b - 2;
				mm_b_nx  = mm_b - 4;
				cost_nx  = cost_pt - 1;
			} else if ((open_bits & target_bit) != 0) {
				/* target area is a neighbour: candidate path */
				parent    = node & 0xff;
				*open_pt  = open_bits & ~target_bit;

				/* shared edge between area parent and target_area (same body as the nb case above) */
				{
					short pz0;
					short pz1;
					short nz0;
					short nz1;
					short ez;
					short lo;
					short hi;
					short ex;
					short zlo;
					short zhi;

					pz0 = pBlock[parent * 6 + 3];
					pz1 = pBlock[parent * 6 + 5];
					nz0 = pBlock[(UINT)target_area * 6 + 3];
					nz1 = pBlock[(UINT)target_area * 6 + 5];

					if ((pz0 == nz1) || (pz1 == nz0)) {
						ez = pz0;
						if (pz1 == nz0) {
							ez = pz1;
						}
						lo = pBlock[parent * 6 + 2];
						if (pBlock[parent * 6 + 2] < pBlock[(UINT)target_area * 6 + 2]) {
							lo = pBlock[(UINT)target_area * 6 + 2];
						}
						hi = pBlock[parent * 6 + 4];
						if (pBlock[(UINT)target_area * 6 + 4] < pBlock[parent * 6 + 4]) {
							hi = pBlock[(UINT)target_area * 6 + 4];
						}
						sx   = (short)((UINT)(lo + hi) >> 1);
						sz   = ez;
						e0   = lo;
						z0   = ez;
						e1   = hi;
						z1   = ez;
						flag = 0;
					} else {
						ex = pBlock[parent * 6 + 2];
						if (pBlock[parent * 6 + 4] == pBlock[(UINT)target_area * 6 + 2]) {
							ex = pBlock[(UINT)target_area * 6 + 2];
						}
						zlo = pz0;
						if (pz0 < nz0) {
							zlo = nz0;
						}
						zhi = pz1;
						if (nz1 < pz1) {
							zhi = nz1;
						}
						sx   = ex;
						sz   = (short)((UINT)(zlo + zhi) >> 1);
						e0   = ex;
						z0   = zlo;
						e1   = ex;
						z1   = zhi;
						flag = 1;
					}
				}

				mm_b[1] = e0;
				mm_b[2] = z0;
				mm_b[3] = e1;
				mm_b[4] = z1;
				sflg_pt[1] = flag;
				poxz_b[1]  = sx;
				poxz_b[2]  = sz;
				poxz_b[3]  = (short)dest_x;
				poxz_b[4]  = (short)dest_z;

				d_a = ((int)*poxz_pt - (int)sx) >> 3;
				d_b = ((int)*poxz_b - (int)sz) >> 3;
				dist_a = SquareRoot0(d_a * d_a + d_b * d_b);
				d_a = ((int)dest_x - (int)sx) >> 3;
				d_b = ((int)dest_z - (int)sz) >> 3;
				dist_b = SquareRoot0(d_a * d_a + d_b * d_b);
				cost = (UINT)*cost_pt + dist_a + dist_b;

				root[(depth & 0xff) + 1] = target_area;
				if (cost < best_cost) {
					node      = root[1];	/* retail reloads the search node from root[1] here */
					way_x     = poxz_st[2];
					way_z     = poxz_st[3];
					best_flg  = sflg[1];
					best_cost = cost;
					Tv0.vx    = MinMax[4];
					Tv0.vz    = MinMax[5];
					Tv1.vx    = MinMax[6];
					Tv1.vz    = MinMax[7];
					dst_x     = poxz_st[4];
					dst_z     = poxz_st[5];
				}

				/* no pointer advance on this path: carry the cursors over */
				visit_nx = visit_pt;
				poxz_nx  = poxz_b;
				mm_b_nx  = mm_b;
				cost_nx  = cost_pt;
			} else {
				/* expand into the lowest open neighbour */
				parent   = node & 0xff;
				node     = 0;
				if ((open_bits & 1) == 0) {
					nb = 1;
					do {
						node = nb;
						nb   = node + 1;
					} while (((int)(UINT)open_bits >> (node & 0x1f) & 1U) == 0);
				}
				cost_nx  = cost_pt + 1;
				poxz_nx  = poxz_b + 2;
				poxz_pt += 2;
				mm_b_nx  = mm_b + 4;
				mm_pt   += 4;
				sflg_pt++;
				nb       = node & 0xff;
				*open_pt = *open_pt & ~(USHORT)(1 << (node & 0x1f));

				/* shared edge between area parent and area nb:
				 * sx/sz = the node's mid point, e0/z0/e1/z1 = the stored edge values,
				 * flag = 0 when the edge is z-coincident (x-range midpoint), 1 otherwise */
				{
					short pz0;
					short pz1;
					short nz0;
					short nz1;
					short ez;
					short lo;
					short hi;
					short ex;
					short zlo;
					short zhi;

					pz0 = pBlock[parent * 6 + 3];
					pz1 = pBlock[parent * 6 + 5];
					nz0 = pBlock[nb * 6 + 3];
					nz1 = pBlock[nb * 6 + 5];

					if ((pz0 == nz1) || (pz1 == nz0)) {
						ez = pz0;
						if (pz1 == nz0) {
							ez = pz1;
						}
						lo = pBlock[parent * 6 + 2];
						if (pBlock[parent * 6 + 2] < pBlock[nb * 6 + 2]) {
							lo = pBlock[nb * 6 + 2];
						}
						hi = pBlock[parent * 6 + 4];
						if (pBlock[nb * 6 + 4] < pBlock[parent * 6 + 4]) {
							hi = pBlock[nb * 6 + 4];
						}
						sx   = (short)((UINT)(lo + hi) >> 1);
						sz   = ez;
						e0   = lo;
						z0   = ez;
						e1   = hi;
						z1   = ez;
						flag = 0;
					} else {
						ex = pBlock[parent * 6 + 2];
						if (pBlock[parent * 6 + 4] == pBlock[nb * 6 + 2]) {
							ex = pBlock[nb * 6 + 2];
						}
						zlo = pz0;
						if (pz0 < nz0) {
							zlo = nz0;
						}
						zhi = pz1;
						if (nz1 < pz1) {
							zhi = nz1;
						}
						sx   = ex;
						sz   = (short)((UINT)(zlo + zhi) >> 1);
						e0   = ex;
						z0   = zlo;
						e1   = ex;
						z1   = zhi;
						flag = 1;
					}
				}

				depth++;
				open_pt++;
				visit_nx = visit_pt + 1;
				mm_pt[0] = e0;
				mm_b[2]  = z0;
				mm_b[3]  = e1;
				mm_b_nx[0] = z1;
				*sflg_pt = flag;

				d_a = (int)poxz_b[-1] - (int)sx;
				if (d_a < 0) d_a = -d_a;
				d_b = (int)*poxz_b - (int)sz;
				if (d_b < 0) d_b = -d_b;
				*cost_nx = (USHORT)(*cost_pt + (short)d_a + (short)d_b);
				*poxz_pt = sx;
				*poxz_nx = sz;
				root[depth & 0xff] = (UCHAR)node;
				*visit_nx = *visit_pt | (USHORT)(1 << (node & 0x1f));
				*open_pt  = pBlock[(node & 0xff) * 6 + 7] & ~*visit_nx;
			}
			visit_pt = visit_nx;
			poxz_b   = poxz_nx;
			mm_b     = mm_b_nx;
			cost_pt  = cost_nx;
		}
		if ((open_st[0] != 0) || (open_pt != open_st))
			goto Search_loop;
	}

	if (best_flg == 0) {
		d = way_z - pEm->Pos_z;
		if (d == 0) {
			slope = 0x7fffffff;
		} else {
			{ int num = (way_x - pEm->Pos_x) * 0x1000; DIV_S(slope, num, d); }
		}
		if (slope < 0) {
			slope = -slope;
		}
		tmp = way_z;
		if (10000 < slope) {
			tmp = (d < 0) ? way_z + 800 : way_z - 800;
		}
		way_z = tmp;
		off_x = 800;
		off_z = 0;
	} else {
		d = way_x - pEm->Pos_x;
		if (d == 0) {
			slope = 0x7fffffff;
		} else {
			{ int num = (way_z - pEm->Pos_z) * 0x1000; DIV_S(slope, num, d); }
		}
		if (slope < 0) {
			slope = -slope;
		}
		tmp = way_x;
		if (10000 < slope) {
			tmp = (d < 0) ? way_x + 800 : way_x - 800;
		}
		way_x = tmp;
		off_x = 0;
		off_z = 800;
	}

	Tv.vy  = 0;
	Tv.vx  = dst_x - pEm->Pos_x;
	Tv.vz  = dst_z - pEm->Pos_z;
	Tv0.vy = 0;
	Tv0.vx = Tv0.vx - (pEm->Pos_x - off_x);
	Tv0.vz = Tv0.vz - (pEm->Pos_z - off_z);
	Tv1.vy = 0;
	Tv1.vx = Tv1.vx - (pEm->Pos_x + off_x);
	Tv1.vz = Tv1.vz - (pEm->Pos_z + off_z);
	OuterProduct0(&Tv0, &Tv, &Tv0);
	OuterProduct0(&Tv1, &Tv, &Tv1);
	if ((Tv0.vy & 0x80000000U) != (Tv1.vy & 0x80000000U)) {
		way_x = dst_x;
		way_z = dst_z;
	}
	pEm->Dest_x = (short)way_x;
	pEm->Dest_z = (short)way_z;
}
