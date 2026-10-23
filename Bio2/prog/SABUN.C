/*
 * SABUN.C - Enemy sabun (frame-difference) animation blending
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SABUN.C
 */

#include "TYPES.H"
#include <INLINE_C.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "GTE_OPS.H"
#include "SABUN.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "TITLE.H"

/* One 6-byte frame vector record (SHORT vx, vy, vz; SVECTOR field names) at idx * 6 from the
 * SA frame base. Retail Sa_parts_allmv (DS 0x8004baf0): idx * 6 is built with sll/addu, then
 * "lh a0,0x0(v1)" (DS 0x8004bc14), "lh v0,0x2(v1)" (DS 0x8004bc20), "lh v0,0x4(v1)" (DS 0x8004bc2c). */
typedef struct {
	SHORT vx;
	SHORT vy;
	SHORT vz;
} SA_VEC;

/* DS 0x8004b3b8 (Sa_dat_set) */
void Sa_dat_set(ENEMY_WORK *pEm, ULONG *pSa)
{
	ULONG  *pBits;
	ULONG   Bit_no;
	ULONG **ppHead;
	UINT    Parts_cnt;

	pEm->pSa_dat = pSa;
	pBits        = pSa;
	if (*pBits != 0) {
		Bit_no    = 0;
		pSa       = pSa + 3;
		ppHead    = &pEm->pSin_parts_ptr->pSa_dat_head;
		Parts_cnt = pEm->pTmd[-1] >> 1;
		do {
			*ppHead = (ULONG *)0;
			if ((*pBits & 1 << Bit_no) != 0) {
				*ppHead = pSa;
				pSa += pSa[2] + 3;
			}
			ppHead = ppHead + sizeof(PARTS_W) / sizeof(ULONG *);		/* one PARTS_W */
			Parts_cnt--;
			Bit_no++;
		} while (Parts_cnt != 0);
	}
}

/* DS 0x8004b434 (Sa_parts_mod)
 * Blends frame A (pSa_dat_head[0]) and frame B (pSa_dat[1] further on) with weight
 * pSa_dat_head[3] into the TMD vertices, then into every part listed in the KAN table
 * entry for Parts_no. SA_sv0 / SA_sv1 are real locals (the original aliased two
 * consecutive stack slots through pointer arithmetic).
 */
void Sa_parts_mod(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR  SA_sv0;
	SVECTOR  SA_sv1;
	SVECTOR  SA_sv2;		/* a third SVECTOR slot at sp+0x10, never accessed */
	SVECTOR *pVtx;
	ULONG   *pSa_head;
	ULONG   *pSa_dat;
	ULONG   *pKan;
	short   *pFrmA;
	short   *pFrmB;
	short   *pNxtA;
	short   *pNxtB;
	ULONG    Vtx_num;
	UINT     Weight;
	int      Inv_w;
	int      Base_a;
	int      Base_b;
	UINT     Ent;
	UINT     Ent_ofs;
	UINT     Cnt;
	UCHAR *pIdx;
	PARTS_W *pParts;

	(void)SA_sv2;
	pSa_head = pP_ptr->pSa_dat_head;
	Vtx_num  = pP_ptr->pTmd[1];
	pVtx     = (SVECTOR *)pP_ptr->pTmd[0];
	Weight   = (UINT)(USHORT)pSa_head[3];
	Inv_w    = 0xfff - Weight;
	pFrmA    = (short *)(pSa_head[0] + (int)pEm->pSa_dat);
	pFrmB    = pFrmA + ((int)pSa_head[1] / 2) * 2;

	SA_sv0.vx = pFrmA[0];
	SA_sv0.vy = pFrmA[1];
	SA_sv0.vz = pFrmA[2];
	pFrmA += 3;
	do {
		gte_ldIR0(Inv_w);
		gte_ldsv(&SA_sv0);
		gte_gpf12();
		SA_sv1.vx = pFrmB[0];
		pNxtB     = pFrmB + 2;
		SA_sv1.vy = pFrmB[1];
		pFrmB += 3;
		SA_sv1.vz = *pNxtB;
		gte_ldIR0(Weight);
		gte_ldsv(&SA_sv1);
		gte_gpl12();
		SA_sv0.vx = *pFrmA;
		pNxtA     = pFrmA + 2;
		SA_sv0.vy = pFrmA[1];
		pFrmA += 3;
		SA_sv0.vz = *pNxtA;
		gte_stsv(pVtx);
		Vtx_num--;
		pVtx++;
	} while (Vtx_num != 0);

	pSa_dat = pEm->pSa_dat;
	pKan    = pEm->pKan_t_ptr;
	Base_a  = (int)pSa_dat[1] + (int)pSa_dat;
	Ent_ofs = (USHORT)*pKan & 0xfffc;
	Ent     = *(UINT *)((int)pKan + (UINT)pP_ptr->Parts_no * 4 + Ent_ofs);
	Base_b  = Base_a + (int)pSa_dat[2];
	pIdx    = (UCHAR *)((int)pKan + (Ent >> 0x10) + Ent_ofs);
	Cnt     = Ent & 0xffff;
	pParts  = pEm->pSin_parts_ptr;
	while (Cnt != 0) {
		Cnt--;
		SA_sv0.vx = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_a))->vx;
		SA_sv0.vy = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_a))->vy;
		SA_sv0.vz = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_a))->vz;
		gte_ldIR0(Inv_w);
		gte_ldsv(&SA_sv0);
		gte_gpf12();
		SA_sv1.vx = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_b))->vx;
		SA_sv1.vy = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_b))->vy;
		SA_sv1.vz = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Base_b))->vz;
		gte_ldIR0(Weight);
		gte_ldsv(&SA_sv1);
		gte_gpl12();
		gte_stlvl((VECTOR *)&pParts[*pIdx].Pos_x);
		pIdx++;
	}
}

/* DS 0x8004b734 (Sa_parts_allmv2) */
void Sa_parts_allmv2(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR  SA_sv;
	SVECTOR *pVtx;
	SVECTOR *pVtx_cur;
	SVECTOR *pVtx_save;
	short   *pVz;
	short   *pFrm;
	short   *pFrm_cur;
	short   *pFrm_nxt;
	ULONG    Vtx_cnt;
	ULONG    Vtx_save;
	ULONG    Cnt;
	int      Blk_no;
	int      Frm_step;
	int      Frm_ofs;
	int      Inv_base;
	int      Inv_w;
	UINT     W;
	UINT     Blk_cnt;
	UINT     Blk_left;
	ULONG    Blk_left2;
	UINT    *pEnt_p;
	int      Kan_base;
	int      Frm_b;
	ULONG    Stride;
	UINT     Idx;
	int      Blk_ofs;
	int      Cnt3;
	UCHAR *pIdx;
	PARTS_W *pParts;
	VECTOR  *pLvl;
	UINT     Hi_mask;

	pVtx    = (SVECTOR *)pP_ptr->pTmd[0];
	Vtx_cnt = pP_ptr->pTmd[1];
	pFrm    = (short *)(*pP_ptr->pSa_dat_head + (int)pEm->pSa_dat);

	/* frame A -> vertex array */
	pVz      = &pVtx->vz;
	pVtx_cur = pVtx;
	pFrm_cur = pFrm;
	Cnt      = Vtx_cnt;
	do {
		Cnt--;
		pVtx_cur->vx = *pFrm_cur;
		pFrm_nxt     = pFrm_cur + 2;
		pVtx_cur++;
		pVz[-1]      = pFrm_cur[1];
		pFrm_cur    += 3;
		*pVz         = *pFrm_nxt;
		pVz         += 4;
	} while (Cnt != 0);

	/* blend each block (weight pSa_dat_head[blk+3]) over the vertices */
	Frm_step  = (int)pP_ptr->pSa_dat_head[1] >> 1;
	Blk_cnt   = (USHORT)pP_ptr->pSa_dat_head[2];
	Inv_base  = 0xfff;
	Frm_ofs   = Frm_step;
	Blk_left  = Blk_cnt;
	Blk_left2 = Blk_cnt;
	Blk_no    = 0;
	do {
		pFrm_cur  = pFrm + Frm_ofs;
		W         = (USHORT)pP_ptr->pSa_dat_head[Blk_no + 3];
		Inv_w     = Inv_base - W;
		pVtx_save = pVtx;
		Vtx_save  = Vtx_cnt;
		do {
			gte_ldIR0(Inv_w);
			gte_ldsv(pVtx);
			gte_gpf12();
			SA_sv.vx = *pFrm_cur;
			pFrm_nxt = pFrm_cur + 2;
			SA_sv.vy = pFrm_cur[1];
			pFrm_cur += 3;
			SA_sv.vz = *pFrm_nxt;
			gte_ldIR0(W);
			gte_ldsv(&SA_sv);
			gte_gpl12();
			gte_stsv(pVtx);
			Vtx_cnt--;
			pVtx++;
		} while (Vtx_cnt != 0);
		Frm_ofs += Frm_step;
		Blk_left--;
		Blk_no++;
		pVtx    = pVtx_save;
		Vtx_cnt = Vtx_save;
	} while (Blk_left != 0);

	/* part positions from the KAN entry for Parts_no */
	pParts  = pEm->pSin_parts_ptr;
	Kan_base = (int)pEm->pKan_t_ptr + ((USHORT)*pEm->pKan_t_ptr & 0xfffc);
	pEnt_p  = (UINT *)(Kan_base + (UINT)pP_ptr->Parts_no * 4);
	Frm_b   = (int)pEm->pSa_dat[1] + (int)pEm->pSa_dat;
	pIdx    = (UCHAR *)(Kan_base + (*pEnt_p >> 0x10));
	for (Idx = *pEnt_p & 0xffff; Idx != 0; Idx--) {
		pParts[*pIdx].Pos_x = (int)((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Frm_b))->vx;
		pParts[*pIdx].Pos_y = (int)((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Frm_b))->vy;
		pParts[*pIdx].Pos_z = (int)((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Frm_b))->vz;
		pIdx++;
	}

	/* per block: blend the entry's parts with the block's frame (stride pSa_dat[2]) */
	Stride  = pEm->pSa_dat[2];
	Hi_mask = 0xffff0000;
	Cnt3    = 0;
	do {
		pIdx   = (UCHAR *)(Kan_base + ((*pEnt_p & Hi_mask) >> 0x10));
		Idx    = *pEnt_p & 0xffff;
		Blk_ofs = Frm_b + (int)(Stride * ((pP_ptr->pSa_dat_head[Cnt3 + 3] & Hi_mask) >> 0x10));
		while (Idx != 0) {
			Idx--;
			gte_ldIR0(Inv_w);
			pLvl = (VECTOR *)&pParts[*pIdx].Pos_x;
			gte_ldlvl(pLvl);
			gte_gpf12();
			SA_sv.vx = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Blk_ofs))->vx;
			SA_sv.vy = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Blk_ofs))->vy;
			SA_sv.vz = ((SA_VEC *)((UINT)*pIdx * sizeof(SA_VEC) + Blk_ofs))->vz;
			gte_ldIR0(W);
			gte_ldsv(&SA_sv);
			pIdx++;
			gte_gpl12();
			gte_stlvl(pLvl);
		}
		Blk_left2--;
		Cnt3++;
	} while (Blk_left2 != 0);
}

/* DS 0x8004baf0 (Sa_parts_allmv)
 * Frame-A copy into the vertices, per-block blend, then (only when Parts_no is in
 * range of the KAN count) the entry's parts are placed from the frame data.
 */
void Sa_parts_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR  SA_sv;
	SVECTOR *pVtx;
	SVECTOR *pVtx_save;
	SVECTOR *pVtx_cur;
	ULONG   *pHead;
	ULONG   *pSa_dat;
	ULONG   *pKan;
	short   *pFrmA;
	short   *pFrm_cur;
	short   *pVz;
	short   *pFrm_nxt;
	ULONG    Vtx_cnt;
	ULONG    Vtx_save;
	ULONG    Kan_cnt;
	ULONG    Stride;
	UINT     Blk_cnt;
	UINT     Blk_left;
	UINT     Frm_ofs;
	UINT     Frm_step;
	UINT     Ent_cnt;
	UINT     Ent;
	UINT     Ent_ofs;
	UINT     Parts_no;
	UINT     Idx;
	UINT     W;
	UINT     Cnt;
	int      Inv_w;
	int      Frm_b;
	int      Blk_base;
	int      In_range;
	UCHAR *pIdx;
	UCHAR *pIdx2;
	PARTS_W *pParts;
	VECTOR  *pLvl;

	pHead   = pP_ptr->pSa_dat_head;
	pSa_dat = pEm->pSa_dat;
	pVtx    = (SVECTOR *)pP_ptr->pTmd[0];
	Vtx_cnt = pP_ptr->pTmd[1];
	pFrmA   = (short *)(*pHead + (int)pSa_dat);

	/* frame A -> vertex array */
	pVz      = &pVtx->vz;
	pVtx_cur = pVtx;
	pFrm_cur = pFrmA;
	Cnt      = Vtx_cnt;
	do {
		Cnt--;
		pVtx_cur->vx = *pFrm_cur;
		pFrm_nxt     = pFrm_cur + 2;
		pVtx_cur++;
		pVz[-1]      = pFrm_cur[1];
		pFrm_cur    += 3;
		*pVz         = *pFrm_nxt;
		pVz         += 4;
	} while (Cnt != 0);

	pKan     = pEm->pKan_t_ptr;
	Parts_no = (UINT)pP_ptr->Parts_no;
	Blk_cnt  = (USHORT)pHead[2];
	pParts   = pEm->pSin_parts_ptr;
	Frm_step = (UINT)pHead[1] >> 1;
	Kan_cnt  = pKan[1];
	Ent_ofs  = (USHORT)*pKan & 0xfffc;
	Ent      = *(UINT *)((int)pKan + Parts_no * 4 + Ent_ofs);
	pIdx     = (UCHAR *)((int)pKan + (Ent >> 0x10) + Ent_ofs);
	Frm_b    = (int)pSa_dat[1] + (int)pSa_dat;
	Ent_cnt  = Ent & 0xffff;

	/* in_range: Parts_no < KAN count (the original flag is the inverse of 0 == "not in range") */
	In_range = ((UINT)(USHORT)Kan_cnt > Parts_no);
	if (In_range) {
		pIdx2 = pIdx;
		for (Idx = Ent_cnt; Idx != 0; Idx--) {
			Cnt = (UINT)*pIdx2++;
			pParts[Cnt].Pos_x = (int)((SA_VEC *)(Cnt * sizeof(SA_VEC) + Frm_b))->vx;
			pParts[Cnt].Pos_y = (int)((SA_VEC *)(Cnt * sizeof(SA_VEC) + Frm_b))->vy;
			pParts[Cnt].Pos_z = (int)((SA_VEC *)(Cnt * sizeof(SA_VEC) + Frm_b))->vz;
		}
	}

	Stride   = pSa_dat[2];
	Blk_left = Blk_cnt;
	Frm_ofs  = Frm_step;
	pHead    = pP_ptr->pSa_dat_head;
	Blk_base = 0;
	do {
		pFrm_cur = pFrmA + Frm_ofs;
		W        = (USHORT)pHead[3];
		Inv_w    = 0xfff - W;
		pVtx_save = pVtx;
		Vtx_save  = Vtx_cnt;
		if (W != 0) {
			do {
				gte_ldIR0(Inv_w);
				gte_ldsv(pVtx);
				SA_sv.vx = *pFrm_cur;
				gte_gpf12_b();
				pFrm_nxt = pFrm_cur + 2;
				SA_sv.vy = pFrm_cur[1];
				SA_sv.vz = *pFrm_nxt;
				gte_ldIR0(W);
				gte_ldsv(&SA_sv);
				pFrm_cur = pFrm_nxt + 1;
				gte_gpl12_b();
				gte_stsv(pVtx);
				Vtx_cnt--;
				pVtx++;
			} while (Vtx_cnt != 0);

			if (In_range) {
				Blk_base = Frm_b + (int)(Stride * (pHead[3] >> 0x10));
				pIdx2    = pIdx;
				for (Idx = Ent_cnt; Idx != 0; Idx--) {
					Cnt = (UINT)*pIdx2;
					gte_ldIR0(Inv_w);
					pLvl = (VECTOR *)&pParts[Cnt].Pos_x;
					gte_ldlvl(pLvl);
					gte_gpf12();
					SA_sv.vx = ((SA_VEC *)(Cnt * sizeof(SA_VEC) + Blk_base))->vx;
					SA_sv.vy = ((SA_VEC *)(Cnt * sizeof(SA_VEC) + Blk_base))->vy;
					SA_sv.vz = ((SA_VEC *)(Cnt * sizeof(SA_VEC) + Blk_base))->vz;
					gte_ldIR0(W);
					gte_ldsv(&SA_sv);
					pIdx2++;
					gte_gpl12_b();
					gte_stlvl(pLvl);
				}
			}
			Vtx_cnt = Vtx_save;
			pVtx    = pVtx_save;
		}
		Frm_ofs += Frm_step;
		Blk_left--;
		pHead++;
	} while (Blk_left != 0);
}

/* DS 0x8004be28 (Sa_parts_offset_allmv) */
void Sa_parts_offset_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	ULONG   *pSa_dat;
	ULONG   *pHead;
	ULONG   *pNxt;
	short   *pFrm;
	short   *pSrc;
	long    *pPos;
	PARTS_W *pParts;
	SVECTOR  SA_sv;
	VECTOR  *pLvl;
	UINT     Kan_cnt;
	int      Cnt;
	int      Cnt2;
	int      Inv_w;
	UINT     Inv_base;
	UINT     W;
	UINT     Blk_cnt;
	ULONG    Stride;
	int      Done;

	pSa_dat = pEm->pSa_dat;
	pHead   = pP_ptr->pSa_dat_head;
	pParts  = pEm->pSin_parts_ptr;
	pFrm    = (short *)(pSa_dat[1] + (int)pSa_dat);
	Kan_cnt = (UINT)(USHORT)pEm->pKan_t_ptr[1];

	if ((pP_ptr->Parts_no + 1 < Kan_cnt) && (Kan_cnt != 1)) {
		pPos = &pParts[1].Pos_z;
		Cnt  = Kan_cnt - 2;
		pSrc = pFrm;
		do {
			pPos[-2] = (int)pSrc[3];
			pPos[-1] = (int)pSrc[4];
			*pPos    = (int)pSrc[5];
			Done     = (Cnt != 0);
			pPos    += sizeof(PARTS_W) / sizeof(long);		/* one PARTS_W */
			Cnt--;
			pSrc    += 3;
		} while (Done);
	}

	Inv_base = 0xfff;
	Blk_cnt  = (UINT)(USHORT)pHead[2];
	Stride   = pSa_dat[2];
	do {
		pNxt   = pHead + 1;
		W      = pHead[3] & 0xffff;
		/* 0 - W + Inv_base == Inv_base - W (mod 2^32); this form keeps 0xfff out of a
		 * callee-saved register (retail frame, see Sa_morph_trans) */
		Inv_w  = 0 - W + Inv_base;
		pSrc   = (short *)((int)pFrm + Stride * (pHead[3] >> 0x10));
		if (Kan_cnt != 1) {
			Cnt2 = Kan_cnt - 2;
			pLvl = (VECTOR *)&pParts[1].Pos_x;
			do {
				pSrc += 3;
				gte_ldIR0(Inv_w);
				gte_ldlvl(pLvl);
				gte_gpf12();
				SA_sv.vx = pSrc[0];
				SA_sv.vy = pSrc[1];
				SA_sv.vz = pSrc[2];
				gte_ldIR0(W);
				gte_ldsv(&SA_sv);
				gte_gpl12();
				gte_stlvl(pLvl);
				pLvl = (VECTOR *)((UCHAR *)pLvl + sizeof(PARTS_W));	/* one PARTS_W */
				Done = (Cnt2 != 0);
				Cnt2--;
			} while (Done);
		}
		Blk_cnt--;
		pHead = pNxt;
	} while (Blk_cnt != 0);
}

/* DS 0x8004bf90 (Sa_morph_trans)
 * GTE morph helper: frame 1 is copied into the TMD vertex array (5 vertices per
 * pass, so the vertex count must be a multiple of 5), then each morph target k
 * (weight pSa_head[3+k], 0..0xfff) is blended into every vertex (gpf12/gpl12):
 * v = v*(0xfff-w)/0x1000 + target*w/0x1000.
 * pP_ptr is the second argument the caller in EM36.C passes; this function does not read it.
 */
void Sa_morph_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG *pTmd, ULONG *pSa_head)
{
	SVECTOR  Tmp_sv;
	SVECTOR *pVtx;
	SVECTOR *pVtx_cur;
	short   *pFrm;
	short   *pTgt;
	ULONG    Vtx_num;
	ULONG    Vtx_left;
	ULONG    Blk_num;
	ULONG    Frm_step;
	ULONG    Frm_ofs;
	ULONG    blk;
	ULONG    j;
	ULONG    k;
	UINT     Weight;

	Vtx_num = pTmd[1];
	pVtx    = (SVECTOR *)pTmd[0];
	pFrm    = (short *)((int)pSa_head[0] + (int)pEm->pSa_dat);

	/* frame 1 -> vertex array */
	Vtx_left = Vtx_num;
	k        = 0;
	do {
		for (j = 0; j < 5; j++, k++) {
			pVtx[k].vx = pFrm[k * 3];
			pVtx[k].vy = pFrm[k * 3 + 1];
			pVtx[k].vz = pFrm[k * 3 + 2];
		}
		Vtx_left -= 5;
	} while (Vtx_left != 0);

	Frm_step = (UINT)pSa_head[1] >> 1;
	Blk_num  = (USHORT)pSa_head[2];
	Frm_ofs  = Frm_step;
	for (blk = 0; blk < Blk_num; blk++) {
		Weight = (USHORT)pSa_head[blk + 3];
		if (Weight != 0) {
			pTgt     = pFrm + Frm_ofs;
			pVtx_cur = pVtx;
			/* 0 - Weight + 0xfff is 0xfff - Weight mod 2^32; written this way so the
			 * 0xfff constant is not kept in a callee-saved register (retail frame). */
			for (j = Vtx_num; j != 0; j--) {
				gte_ldIR0(0 - Weight + 0xfff);
				gte_ldsv(pVtx_cur);
				Tmp_sv.vx = pTgt[0];
				gte_gpf12_b();
				Tmp_sv.vy = pTgt[1];
				Tmp_sv.vz = pTgt[2];
				gte_ldIR0(Weight);
				gte_ldsv(&Tmp_sv);
				gte_gpl12_b();
				gte_stsv(pVtx_cur);
				pTgt += 3;
				pVtx_cur++;
			}
		}
		Frm_ofs += Frm_step;
	}
}
