/*
 * MD_SUB.C - Model sub-routines: packet preset, GTE sort, mirror, parts management
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 *
 * DS retail range: 0x8002cbc4 - 0x8002ef4c
 */

#include "TYPES.H"
#include "ROOM.H"
#include "SLD.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <INLINE_S.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <MALLOC.H>
#include "MD_SUB.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "SCE1.H"
#include "SUB.H"
#include "DIVCHK.H"

/* DS 0x8009dc28 (Bomb_tbl) */
short Bomb_tbl[2][30] = {
	{
		0x001E, 0x003B, 0x0057, 0x0072, 0x008C, 0x00A5, 0x00BD, 0x00D4, 0x00EA, 0x00FF,
		0x0113, 0x0126, 0x0138, 0x0149, 0x0159, 0x0168, 0x0176, 0x0183, 0x018F, 0x019A,
		0x01A4, 0x01AD, 0x01B5, 0x01BC, 0x01C2, 0x01C7, 0x01CB, 0x01CE, 0x01D0, 0x01D1,
	},
	{
		0x00B4, 0x0162, 0x020A, 0x02AC, 0x0348, 0x03DE, 0x046E, 0x04F8, 0x057C, 0x05FA,
		0x0672, 0x06E4, 0x0750, 0x07B6, 0x0816, 0x0870, 0x08C4, 0x0912, 0x095A, 0x099C,
		0x09D8, 0x0A0E, 0x0A3E, 0x0A68, 0x0A8C, 0x0AAA, 0x0AC2, 0x0AD4, 0x0AE0, 0x0AE6,
	},
};
ULONG *m_pl_weapon_tmd;			/* DS 0x800cc1e0 - mirrored weapon TMD work area        */
ULONG *m_pl_w00_tmd;			/* DS 0x800d5220 - player parts[11] pTmd backup         */
ULONG *m_pl_w00_tmd2;			/* DS 0x800d5be0 - player parts[11] pTmd2 backup        */


/* ------------------------------------------------------------------ */
/* Globals owned by this module                                         */
/* ------------------------------------------------------------------ */

ULONG *m_pl_weapon_parts;		/* DS 0x800d8ce8 - mirrored player weapon PARTS_W block */
ULONG *m_pl_weapon_packet;		/* DS 0x800df334 - mirrored weapon packet area          */

/* DS 0x800e2a90 (GsMWSMATRIX) */
MATRIX GsMWSMATRIX = {
	{ { 0x0000, 0x0000, 0x0000 }, { 0x0000, 0x0000, 0x0000 }, { 0x0000, 0x0000, 0x0000 } },
	{ 0x00000000, 0x00000000, 0x00000000 }
};

ULONG *m_pl_w00_packet;			/* DS 0x800ea248 - player parts[11] pPacket backup      */
ULONG *m_pl_w00_packet2;		/* DS 0x800eae38 - player parts[11] pPacket2 backup     */

/* OT used by all sorters below: G.Ot1[G.Cbuf_id] (DS 0x800cc22c + Cbuf_id * 0x1000) */

/*
 * PresetObjectGT3_neo - DS 0x8002cbc4
 * Build the double-buffered POLY_GT3 packets (2 x 0x28 bytes per polygon)
 * for pMw->pTmd. Flg == 0 caches the first build: pTmd[6] is replaced by
 * 0xff000000 | packet address so later calls only memcpy16 the image.
 */
/* DS 0x8002cbc4 (PresetObjectGT3_neo) */
ULONG PresetObjectGT3_neo(MODEL_WORK *pMw, ULONG *G_packet, ULONG Flg)
{
	ULONG    *p;
	ULONG     Poly_num;
	ULONG    *pPrim_ptr;
	ULONG     Late;
	ULONG    *pEnd;
	POLY_GT3 *gt3;

	p            = pMw->pTmd;
	pMw->pPacket = G_packet;
	Poly_num     = ((TMD_MEM *)p)->n_poly;
	pPrim_ptr    = (ULONG *)p[6];
	pEnd         = G_packet + Poly_num * 20;
	if (Flg == 0)
	{
		if (((ULONG)pPrim_ptr & 0xff000000) == 0xff000000)
		{
			memcpy16(G_packet, (ULONG *)(((ULONG)pPrim_ptr & 0x00ffffff) | 0x80000000), Poly_num * 0x50);
			return (ULONG)pEnd;
		}
		p[6] = ((ULONG)G_packet & 0x00ffffff) | 0xff000000;
	}
	Late = (pMw->Attribute & 0xc) << 19;
	gt3  = (POLY_GT3 *)G_packet;
	while (Poly_num-- != 0)
	{
		setPolyGT3(gt3);
		*(ULONG *)&gt3->r0 = 0x34808080;
		*(ULONG *)&gt3->r1 = 0x00808080;
		*(ULONG *)&gt3->u0 = pPrim_ptr[0];
		*(ULONG *)&gt3->r2 = 0x00808080;
		*(ULONG *)&gt3->u1 = pPrim_ptr[1] | Late;
		*(ULONG *)&gt3->u2 = pPrim_ptr[2];
		gt3[1] = gt3[0];
		gt3       += 2;
		pPrim_ptr += 3;
	}
	return (ULONG)pEnd;
}

/*
 * PresetObjectGT4_neo - DS 0x8002cd24
 * Same as PresetObjectGT3_neo for the POLY_GT4 list (pTmd2, 2 x 0x34 bytes).
 * Note: the retail code writes code byte 0x3c and then overwrites it with
 * the 0x34808080 word; the sorters rewrite r0/code anyway.
 */
/* DS 0x8002cd24 (PresetObjectGT4_neo) */
ULONG PresetObjectGT4_neo(MODEL_WORK *pMw, ULONG *G_packet, ULONG Flg)
{
	ULONG    *p;
	ULONG     Poly_num;
	ULONG    *pPrim_ptr;
	ULONG     Late;
	ULONG    *pEnd;
	POLY_GT4 *gt4;

	p             = pMw->pTmd2;
	pMw->pPacket2 = G_packet;
	Poly_num      = ((TMD_MEM *)p)->n_poly;
	pPrim_ptr     = (ULONG *)p[6];
	pEnd          = G_packet + Poly_num * 26;
	if (Flg == 0)
	{
		if (((ULONG)pPrim_ptr & 0xff000000) == 0xff000000)
		{
			memcpy16(G_packet, (ULONG *)(((ULONG)pPrim_ptr & 0x00ffffff) | 0x80000000), Poly_num * 0x68);
			return (ULONG)pEnd;
		}
		p[6] = ((ULONG)G_packet & 0x00ffffff) | 0xff000000;
	}
	Late = (pMw->Attribute & 0xc) << 19;
	gt4  = (POLY_GT4 *)G_packet;
	if (Poly_num != 0)
	{
		do
		{
			setPolyGT4(gt4);
			*(ULONG *)&gt4->r0 = 0x34808080;
			*(ULONG *)&gt4->r1 = 0x00808080;
			*(ULONG *)&gt4->u0 = pPrim_ptr[0];
			*(ULONG *)&gt4->r2 = 0x00808080;
			*(ULONG *)&gt4->u1 = pPrim_ptr[1] | Late;
			*(ULONG *)&gt4->r3 = 0x00808080;
			*(ULONG *)&gt4->u2 = pPrim_ptr[2];
			*(ULONG *)&gt4->u3 = pPrim_ptr[3];
			gt4[1] = gt4[0];
			gt4       += 2;
			pPrim_ptr += 4;
		} while (--Poly_num != 0);
	}
	return (ULONG)pEnd;
}

/*
 * CompM - DS 0x8002ce94
 * m2 = m0 * m1 (rotation and translation), done column by column on the GTE.
 */
/* DS 0x8002ce94 (CompM) */
void CompM(MATRIX *m0, MATRIX *m1, MATRIX *m2)
{
	gte_SetRotMatrix(m0);
	gte_SetTransMatrix(m0);
	gte_ldclmv(&m1->m[0][0]);
	gte_rtir();
	gte_stclmv(&m2->m[0][0]);
	gte_ldclmv(&m1->m[0][1]);
	gte_rtir();
	gte_stclmv(&m2->m[0][1]);
	gte_ldclmv(&m1->m[0][2]);
	gte_rtir();
	gte_stclmv(&m2->m[0][2]);
	gte_ldlv0(m1->t);
	gte_rt();
	gte_stlvnl(m2->t);
}

/*
 * mapModelingData - DS 0x8002cfd8
 * Relocate the offsets of every TMD_MEM object header (7 words each) that
 * follows the 2-word file header: ver_top, nor_top, prim_top, word 6.
 */
/* DS 0x8002cfd8 (mapModelingData) */
void mapModelingData(ULONG *pHead)
{
	ULONG  nPoly;
	ULONG *pT_head;
	ULONG  ad_head;

	nPoly   = pHead[1];
	pT_head = pHead + 2;
	ad_head = (ULONG)pT_head;
	do
	{
		nPoly--;
		*(ULONG *)&((TMD_MEM *)pT_head)->ver_top += ad_head;
		*(ULONG *)&((TMD_MEM *)pT_head)->nor_top += ad_head;
		pT_head[6] += ad_head;
		*(ULONG *)&((TMD_MEM *)pT_head)->prim_top += ad_head;
		pT_head    += 7;
	} while (nPoly != 0);
}

/*
 * MulLMatrix - DS 0x8002d030
 * nlm = transpose(lwm) * klwm, nlm.t = transpose(lwm) * (klwm.t - lwm.t).
 */
/* DS 0x8002d030 (MulLMatrix) */
void MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm)
{
	MATRIX tmp;
	VECTOR vec;

	TransposeMatrix(lwm, &tmp);
	MulMatrix0(&tmp, klwm, nlm);
	vec.vx = klwm->t[0] - lwm->t[0];
	vec.vy = klwm->t[1] - lwm->t[1];
	vec.vz = klwm->t[2] - lwm->t[2];
	ApplyMatrixLV(&tmp, &vec, &vec);
	nlm->t[0] = vec.vx;
	nlm->t[1] = vec.vy;
	nlm->t[2] = vec.vz;
}

/*
 * SortGT3_lno - DS 0x8002d0e8
 * Light-less GT3 sort: back-face culled, colour/code word = Cv with
 * code (D0 << 1 | 0x34) (D0 = semi-transparency on/off).
 */
/* DS 0x8002d0e8 (SortGT3_lno) */
void SortGT3_lno(TMD_MEM *pT_head, POLY_GT3 *bp, ULONG Cv, ULONG D0)
{
	SVECTOR *sp;
	short   *p_prim;
	ULONG   *tmp;
	SVECTOR *s1;
	SVECTOR *s2;
	SVECTOR *s3;
	ULONG    n;
	long     D2;

	p_prim = (short *)pT_head->prim_top;
	sp     = (SVECTOR *)pT_head->ver_top;
	Cv     = (Cv & 0x00ffffff) | (ULONG)(UCHAR)(D0 << 1 | 0x34) << 24;
	n      = pT_head->n_poly;
	s1     = &sp[p_prim[1]];
	s2     = &sp[p_prim[3]];
	s3     = &sp[p_prim[5]];
	while (n != 0)
	{
		gte_ldv3(s1, s2, s3);
		p_prim += 6;
		n--;
		gte_rtpt();
		s2 = &sp[p_prim[3]];
		s3 = &sp[p_prim[5]];
		gte_nclip();
		s1 = &sp[p_prim[1]];
		gte_stopz(&D2);
		if (D2 < 0)
		{
			bp += 2;
			continue;
		}
		gte_avsz3();
		*(ULONG *)&bp->r0 = Cv;
		gte_stsxy3_gt3(bp);
		gte_stotz(&D2);
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = G.Ot1[G.Cbuf_id] + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x09000000;
		bp += 2;
	}
}

/*
 * SortGT4_lno - DS 0x8002d244
 * Light-less GT4 sort (see SortGT3_lno), code = D0 << 1 | 0x3c.
 */
/* DS 0x8002d244 (SortGT4_lno) */
void SortGT4_lno(TMD_MEM *pT_head, POLY_GT4 *bp, ULONG Cv, ULONG D0)
{
	SVECTOR *sp;
	short   *p_prim;
	ULONG   *ot;
	ULONG   *tmp;
	long     D2;
	SVECTOR *s0;
	SVECTOR *s1;
	SVECTOR *s2;
	SVECTOR *s3;
	ULONG    n;

	p_prim = (short *)pT_head->prim_top;
	Cv     = (Cv & 0x00ffffff) | (ULONG)(UCHAR)(D0 << 1 | 0x3c) << 24;
	sp     = (SVECTOR *)pT_head->ver_top;
	n      = pT_head->n_poly;
	ot     = G.Ot1[G.Cbuf_id];
	s1     = &sp[p_prim[1]];
	s2     = &sp[p_prim[3]];
	s3     = &sp[p_prim[5]];
	while (n != 0)
	{
		gte_ldv3(s1, s2, s3);
		p_prim += 8;
		n--;
		gte_rtpt();
		s1 = &sp[p_prim[1]];
		s2 = &sp[p_prim[3]];
		s3 = &sp[p_prim[5]];
		gte_nclip();
		s0 = &sp[p_prim[-1]];
		gte_stopz(&D2);
		if (D2 < 0)
		{
			bp += 2;
			continue;
		}
		gte_stsxy3_gt3(bp);
		gte_ldv0(s0);
		gte_rtps();
		*(ULONG *)&bp->r0 = Cv;
		gte_avsz4();
		gte_stsxy(&bp->x3);
		gte_stotz(&D2);
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = ot + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x0c000000;
		bp += 2;
	}
}

/*
 * Bomb_Parts_SortGT3 - DS 0x8002d3c8
 * Exploding-part GT3 sort: every polygon is pushed out along its normal
 * by Bomb_tbl[Tb_no >> 15][Tb_no & 0x7fff] (Tb_no = pP_ptr->Free[4]).
 * The offset for the next polygon is computed while the current one is
 * being processed (bbm0 = original rot/trans, bbm1.t = shifted trans).
 */
/* DS 0x8002d3c8 (Bomb_Parts_SortGT3) */
void Bomb_Parts_SortGT3(TMD_MEM *pT_head, POLY_GT3 *pPacket, PARTS_W *pP_ptr)
{
	POLY_GT3 *bp;
	SVECTOR  *sp;
	SVECTOR  *np;
	ULONG    *p_prim;
	ULONG    *ot;
	ULONG    *tmp;
	SVECTOR  *s0;
	SVECTOR  *s1;
	SVECTOR  *s2;
	SVECTOR  *s3;
	VECTOR   *v;
	short    *pBomb;
	long      D2;
	MATRIX    bbm1;
	MATRIX    bbm0;
	ULONG     cnt0;
	ULONG     rgb;
	ULONG     Tb_no;

	bp     = pPacket;
	p_prim = pT_head->prim_top;
	cnt0   = pT_head->n_poly;
	np     = (SVECTOR *)pT_head->nor_top;
	sp     = (SVECTOR *)pT_head->ver_top;
	Tb_no  = (USHORT)pP_ptr->Free[4];
	s0     = &np[((USHORT *)p_prim)[0]];
	ot     = G.Ot1[G.Cbuf_id];
	gte_ReadRotMatrix(&bbm0);
	pBomb = &Bomb_tbl[Tb_no >> 15][Tb_no & 0x7fff];
	v  = (VECTOR *)bbm1.t;
	v->vx = (s0->vx >> 10) * *pBomb;
	v->vy = (s0->vy >> 10) * *pBomb;
	v->vz = (s0->vz >> 10) * *pBomb;
	gte_ldlv0(v);
	rgb = (pP_ptr->Poly_rgb & 0x00ffffff) | 0x34000000;
	gte_rtv0tr();
	s1 = &sp[((USHORT *)p_prim)[1]];
	s2 = &sp[((USHORT *)p_prim)[3]];
	s3 = &sp[((USHORT *)p_prim)[5]];
	while (cnt0 != 0)
	{
		gte_stlvnl(v);
		gte_SetTransMatrix(&bbm1);
		gte_ldv3(s1, s2, s3);
		p_prim += 3;
		cnt0--;
		gte_rtpt();
		s1 = &sp[((USHORT *)p_prim)[1]];
		s2 = &sp[((USHORT *)p_prim)[3]];
		s0 = &np[((USHORT *)p_prim)[0]];
		s3 = &sp[((USHORT *)p_prim)[5]];
		gte_avsz3();
		*(ULONG *)&bp->r0 = rgb;
		*(ULONG *)&bp->r1 = rgb;
		*(ULONG *)&bp->r2 = rgb;
		gte_stsxy3_gt3(bp);
		gte_stotz(&D2);
		gte_SetTransMatrix(&bbm0);
		v->vx = (s0->vx >> 10) * *pBomb;
		v->vy = (s0->vy >> 10) * *pBomb;
		v->vz = (s0->vz >> 10) * *pBomb;
		gte_ldlv0(v);
		gte_rt();
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = ot + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x09000000;
		bp += 2;
	}
}

/*
 * Bomb_Parts_SortGT4 - DS 0x8002d718
 * GT4 version of Bomb_Parts_SortGT3 (bbm0 = original, bbm1.t = shifted).
 */
/* DS 0x8002d718 (Bomb_Parts_SortGT4) */
void Bomb_Parts_SortGT4(TMD_MEM *pT_head, POLY_GT4 *pPacket, PARTS_W *pP_ptr)
{
	POLY_GT4 *bp;
	SVECTOR  *sp;
	SVECTOR  *np;
	ULONG    *p_prim;
	ULONG    *ot;
	ULONG    *tmp;
	short    *pBomb;
	long      D2;
	SVECTOR  *s0;
	SVECTOR  *s1;
	SVECTOR  *s2;
	SVECTOR  *s3;
	VECTOR   *v;
	MATRIX    bbm0;
	MATRIX    bbm1;
	long      cnt0;
	ULONG     rgb;
	ULONG     Tb_no;

	bp     = pPacket;
	ot     = G.Ot1[G.Cbuf_id];
	p_prim = pT_head->prim_top;
	cnt0   = pT_head->n_poly;
	Tb_no  = (USHORT)pP_ptr->Free[4];
	sp     = (SVECTOR *)pT_head->ver_top;
	np     = (SVECTOR *)pT_head->nor_top;
	gte_ReadRotMatrix(&bbm0);
	s0 = &np[((USHORT *)p_prim)[0]];
	pBomb = &Bomb_tbl[Tb_no >> 15][Tb_no & 0x7fff];
	v  = (VECTOR *)bbm1.t;
	v->vx = (s0->vx >> 10) * *pBomb;
	v->vy = (s0->vy >> 10) * *pBomb;
	v->vz = (s0->vz >> 10) * *pBomb;
	gte_ldlv0(v);
	rgb = (pP_ptr->Poly_rgb & 0x00ffffff) | 0x3c000000;
	gte_rtv0tr();
	s1 = &sp[((USHORT *)p_prim)[1]];
	s2 = &sp[((USHORT *)p_prim)[3]];
	s3 = &sp[((USHORT *)p_prim)[5]];
	while (cnt0 != 0)
	{
		gte_stlvnl(v);
		gte_SetTransMatrix(&bbm1);
		gte_ldv3(s1, s2, s3);
		cnt0--;
		p_prim += 4;
		gte_rtpt();
		s1 = &sp[((USHORT *)p_prim)[1]];
		s2 = &sp[((USHORT *)p_prim)[3]];
		s3 = &sp[((USHORT *)p_prim)[5]];
		s0 = &sp[((USHORT *)p_prim)[-1]];
		gte_stsxy3_gt3(bp);
		gte_ldv0(s0);
		gte_rtps();
		*(ULONG *)&bp->r0 = rgb;
		*(ULONG *)&bp->r1 = rgb;
		*(ULONG *)&bp->r2 = rgb;
		*(ULONG *)&bp->r3 = rgb;
		gte_avsz4();
		gte_stsxy(&bp->x3);
		gte_stotz(&D2);
		s0 = &np[((USHORT *)p_prim)[0]];
		gte_SetTransMatrix(&bbm0);
		v->vx = (s0->vx >> 10) * *pBomb;
		v->vy = (s0->vy >> 10) * *pBomb;
		v->vz = (s0->vz >> 10) * *pBomb;
		gte_ldlv0(v);
		gte_rt();
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = ot + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x0c000000;
		bp += 2;
	}
}

/*
 * Ryu_Parts_SortGT3 - DS 0x8002da80
 * Lit (NCCT) GT3 sort for dripping/flowing parts: vertex 0 of every front
 * facing polygon is re-projected T (= pP_ptr->Free[0]) units lower, clamped
 * to the enemy's ground height g.
 */
/* DS 0x8002da80 (Ryu_Parts_SortGT3) */
void Ryu_Parts_SortGT3(TMD_MEM *pT_head, POLY_GT3 *bp, PARTS_W *pP_ptr, ULONG D0)
{
	SVECTOR *sp;
	short   *p_prim;
	ULONG   *tmp;
	SVECTOR *s0;
	SVECTOR *s1;
	SVECTOR *s2;
	SVECTOR *s3;
	long     D2;
	long     D3;
	ULONG    rgb;
	VECTOR   vv;
	VECTOR  *v;
	MATRIX   bbm0;
	MATRIX   bbm1;
	long     Y;
	long     T;
	long     g;
	ULONG    n;

	g     = G.pEm->Ground;
	vv.vx = 0;
	T     = pP_ptr->Free[0];
	vv.vz = 0;
	vv.vy = T;
	gte_ReadRotMatrix(&bbm0);
	v      = (VECTOR *)bbm1.t;
	p_prim = (short *)pT_head->prim_top;
	n      = pT_head->n_poly;
	rgb    = (pP_ptr->Poly_rgb & 0x00ffffff) | 0x34000000;
	Y      = pP_ptr->Workm.t[1];
	sp     = (SVECTOR *)pT_head->ver_top;
	gte_ldrgb(&rgb);
	s1 = &sp[p_prim[1]];
	s2 = &sp[p_prim[3]];
	s3 = &sp[p_prim[5]];
gt3_top:
	if (n == 0)
	{
		goto gt3_end;
	}
	{
		gte_ldv3(s1, s2, s3);
		p_prim += 6;
		n--;
		gte_rtpt();
		s2 = &((SVECTOR *)pT_head->nor_top)[p_prim[-4]];
		s3 = &((SVECTOR *)pT_head->nor_top)[p_prim[-2]];
		s0 = &((SVECTOR *)pT_head->nor_top)[p_prim[-6]];
		gte_nclip();
		gte_stopz(&D3);
		gte_avsz3();
		gte_stsxy3_gt3(bp);
		gte_stotz(&D2);
		gte_ldv3(s0, s2, s3);
		gte_ncct();
		s2 = &sp[p_prim[3]];
		s3 = &sp[p_prim[5]];
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = G.Ot1[G.Cbuf_id] + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x09000000;
		bp += 2;
		gte_strgb3_gt3(bp - 2);
		if (D3 < 0)
		{
			s1 = &sp[p_prim[1]];
			goto gt3_top;
		}
		vv.vy = T;
		if (g < Y + s1->vy + T)
		{
			vv.vy = -Y - s1->vy + g;
		}
		gte_SetRotMatrix(&GsWSMATRIX);
		gte_ldlv0(&vv);
		gte_rt();
		gte_stlvnl(v);
		gte_SetTransMatrix(&bbm1);
		gte_ldv0(s1);
		gte_rtps();
		s1 = &sp[p_prim[1]];
		gte_stsxy(&bp[-2].x0);
		gte_SetRotMatrix(&bbm0);
		gte_SetTransMatrix(&bbm0);
		goto gt3_top;
	}
gt3_end:
	;
}

/*
 * Ryu_Parts_SortGT4 - DS 0x8002ddf0
 * GT4 version of Ryu_Parts_SortGT3 (T = pP_ptr->Free[1]).
 */
/* DS 0x8002ddf0 (Ryu_Parts_SortGT4) */
void Ryu_Parts_SortGT4(TMD_MEM *pT_head, POLY_GT4 *bp, PARTS_W *pP_ptr, ULONG D0)
{
	SVECTOR *sp;
	short   *p_prim;
	ULONG   *ot;
	ULONG   *tmp;
	long     D2;
	long     D3;
	SVECTOR *s0;
	SVECTOR *s1;
	SVECTOR *s2;
	SVECTOR *s3;
	ULONG    rgb;
	VECTOR   vv;
	VECTOR  *v;
	MATRIX   bbm0;
	MATRIX   bbm1;
	long     Y;
	long     T;
	long     g;
	ULONG    n;

	g     = G.pEm->Ground;
	vv.vx = 0;
	T     = pP_ptr->Free[1];
	vv.vz = 0;
	vv.vy = T;
	gte_ReadRotMatrix(&bbm0);
	v      = (VECTOR *)bbm1.t;
	p_prim = (short *)pT_head->prim_top;
	n      = pT_head->n_poly;
	rgb    = (pP_ptr->Poly_rgb & 0x00ffffff) | 0x3c000000;
	sp     = (SVECTOR *)pT_head->ver_top;
	Y      = pP_ptr->Workm.t[1];
	s1     = &sp[p_prim[1]];
	s2     = &sp[p_prim[3]];
	s3     = &sp[p_prim[5]];
	ot     = G.Ot1[G.Cbuf_id];
	gte_ldrgb(&rgb);
gt4_top:
	if (n == 0)
	{
		goto gt4_end;
	}
	{
		gte_ldv3(s1, s2, s3);
		p_prim += 8;
		n--;
		gte_rtpt();
		s0 = &sp[p_prim[-1]];
		gte_nclip();
		gte_stopz(&D3);
		gte_stsxy3_gt3(bp);
		gte_ldv0(s0);
		gte_rtps();
		s0 = &((SVECTOR *)pT_head->nor_top)[p_prim[-2]];
		gte_ldv0(s0);
		gte_nccs();
		s0 = &((SVECTOR *)pT_head->nor_top)[p_prim[-8]];
		s2 = &((SVECTOR *)pT_head->nor_top)[p_prim[-6]];
		s3 = &((SVECTOR *)pT_head->nor_top)[p_prim[-4]];
		gte_avsz4();
		gte_stsxy(&bp->x3);
		gte_strgb(&bp->r3);
		gte_stotz(&D2);
		gte_ldv3(s0, s2, s3);
		gte_ncct();
		s2 = &sp[p_prim[3]];
		s3 = &sp[p_prim[5]];
		if (D2 > 0x1fff)
		{
			D2 = 0x1fff;
		}
		tmp     = ot + ((ULONG)D2 >> 3);
		D2      = *tmp;
		*tmp    = (ULONG)bp & 0x00ffffff;
		bp->tag = (D2 & 0x00ffffff) | 0x0c000000;
		bp += 2;
		gte_strgb3_gt3(bp - 2);
		if (D3 < 0)
		{
			s1 = &sp[p_prim[1]];
			goto gt4_top;
		}
		vv.vy = T;
		if (g < Y + s1->vy + T)
		{
			vv.vy = -Y - s1->vy + g;
		}
		gte_SetRotMatrix(&GsWSMATRIX);
		gte_ldlv0(&vv);
		gte_rt();
		gte_stlvnl(v);
		gte_SetTransMatrix(&bbm1);
		gte_ldv0(s1);
		gte_rtps();
		s1 = &sp[p_prim[1]];
		gte_stsxy(&bp[-2].x0);
		gte_SetRotMatrix(&bbm0);
		gte_SetTransMatrix(&bbm0);
		goto gt4_top;
	}
gt4_end:
	;
}

/*
 * mirror_TMD_GT3 - DS 0x8002e1b4
 * Reverse winding of every GT3 primitive record (swap words 1 and 2).
 */
/* DS 0x8002e1b4 (mirror_TMD_GT3) */
void mirror_TMD_GT3(ULONG *tmd_top)
{
	ULONG  n;
	ULONG  tmp;
	ULONG *p_v;

	n   = ((TMD_MEM *)tmd_top)->n_poly;
	p_v = ((TMD_MEM *)tmd_top)->prim_top + 1;
	while (n-- != 0)
	{
		tmp    = p_v[0];
		p_v[0] = p_v[1];
		p_v[1] = tmp;
		p_v   += 3;
	}
}

/*
 * mirror_PACKET_GT3 - DS 0x8002e1fc
 * Swap uv1/uv2 of every GT3 packet (both buffers).
 */
/* DS 0x8002e1fc (mirror_PACKET_GT3) */
void mirror_PACKET_GT3(ULONG *tmd_top, POLY_GT3 *p_top)
{
	ULONG     n;
	ULONG     tmp;
	POLY_GT3 *gt3;

	n   = ((TMD_MEM *)tmd_top)->n_poly * 2;
	gt3 = p_top;
	while (n-- != 0)
	{
		tmp                    = *(USHORT *)&gt3->u1;
		*(USHORT *)&gt3->u1   = *(USHORT *)&gt3->u2;
		*(USHORT *)&gt3->u2   = (USHORT)tmp;
		gt3++;
	}
}

/*
 * mirror_TMD_GT4 - DS 0x8002e240
 * Reverse winding of every GT4 primitive record (swap words 1 and 2).
 */
/* DS 0x8002e240 (mirror_TMD_GT4) */
void mirror_TMD_GT4(ULONG *tmd_top)
{
	ULONG  n;
	ULONG  tmp;
	ULONG *p_v;

	n   = ((TMD_MEM *)tmd_top)->n_poly;
	p_v = ((TMD_MEM *)tmd_top)->prim_top + 1;
	while (n-- != 0)
	{
		tmp    = p_v[0];
		p_v[0] = p_v[1];
		p_v[1] = tmp;
		p_v   += 4;
	}
}

/*
 * mirror_PACKET_GT4 - DS 0x8002e288
 * Swap uv1/uv2 of every GT4 packet (both buffers, 16-bit counter).
 */
/* DS 0x8002e288 (mirror_PACKET_GT4) */
void mirror_PACKET_GT4(ULONG *tmd_top, POLY_GT4 *p_top)
{
	USHORT    n;
	USHORT    tmp;
	POLY_GT4 *gt4;

	gt4 = p_top;
	for (n = (USHORT)(((TMD_MEM *)tmd_top)->n_poly * 2); n != 0; n--)
	{
		tmp                  = *(USHORT *)&gt4->u1;
		*(USHORT *)&gt4->u1 = *(USHORT *)&gt4->u2;
		*(USHORT *)&gt4->u2 = tmp;
		gt4++;
	}
}

/*
 * Mirror_model_cp - DS 0x8002e2d8
 * Build the mirror-image copy of pEm's model at Mem: PARTS_W array, TMD
 * header + primitive data (winding reversed), packets (uv swapped).
 * For players (Id < 0x10) 0xd70 bytes are reserved for the weapon parts.
 * Returns the next free address (from PartsWork_link).
 */
/* DS 0x8002e2d8 (Mirror_model_cp) */
ULONG Mirror_model_cp(ENEMY_WORK *pEm, ULONG Mem)
{
	PARTS_W *pP_ptr;
	PARTS_W *pM_ptr;
	PARTS_W *pTmp;
	long     parts_num;
	long     i;
	long     byte;
	long     off0;
	ULONG    Top;

	pP_ptr           = pEm->pSin_parts_ptr;
	parts_num        = pEm->Parts_num;
	pM_ptr           = (PARTS_W *)Mem;
	pEm->pM_char_ptr = (ULONG *)Mem;
	i                = parts_num;
	Mem             += parts_num * sizeof(PARTS_W);
	if (pEm->Id < 0x10)
	{
		m_pl_weapon_parts = (ULONG *)Mem;
		Mem += 0xd70;
	}
	Memcpy(pM_ptr, pP_ptr, parts_num * sizeof(PARTS_W));

	/* TMD header block, then the primitive data block */
	byte          = (ULONG)((TMD_MEM *)pP_ptr->pTmd)->ver_top - (ULONG)pP_ptr->pTmd;
	pM_ptr->pTmd  = (ULONG *)Mem;
	Memcpy((void *)Mem, pP_ptr->pTmd, byte);
	Mem += byte;
	byte = pEm->pTmd[-3] - ((ULONG)((TMD_MEM *)pP_ptr->pTmd)->prim_top + 0xc - (ULONG)pEm->pTmd);
	Memcpy((void *)Mem, (void *)((TMD_MEM *)pP_ptr->pTmd)->prim_top, byte);
	Mem += byte;

	/* relocate every part's TMD pointers into the copy */
	off0  = (ULONG)pM_ptr->pTmd - (ULONG)pP_ptr->pTmd;
	Top   = (off0 >> 2) << 2;
	off0 -= (ULONG)((TMD_MEM *)pP_ptr->pTmd)->prim_top - (ULONG)((TMD_MEM *)pP_ptr->pTmd)->ver_top;
	pTmp  = pP_ptr;
	do
	{
		pM_ptr->pTmd     = (ULONG *)((ULONG)pTmp->pTmd + Top);
		pM_ptr->pTmd2    = (ULONG *)((ULONG)pTmp->pTmd2 + Top);
		*(ULONG *)&((TMD_MEM *)pM_ptr->pTmd)->prim_top += off0;
		*(ULONG *)&((TMD_MEM *)pM_ptr->pTmd2)->prim_top += off0;
		i--;
		pTmp++;
		mirror_TMD_GT3(pM_ptr->pTmd);
		mirror_TMD_GT4(pM_ptr->pTmd2);
		pM_ptr++;
	} while (i != 0);

	/* build packets for the copy, then restore the original model */
	pM_ptr              = (PARTS_W *)pEm->pM_char_ptr;
	pTmp                = pEm->pSin_parts_ptr;
	pP_ptr              = pEm->pSin_parts_ptr;
	pEm->pSin_parts_ptr = (PARTS_W *)pEm->pM_char_ptr;
	pEm->pTmd           = pM_ptr->pTmd;
	Mem                 = PartsWork_link(pEm, Mem, pEm->pKan_t_ptr, 0);
	pEm->pSin_parts_ptr = pP_ptr;
	pEm->pTmd           = pTmp->pTmd;
	do
	{
		parts_num--;
		mirror_PACKET_GT3(pM_ptr->pTmd, (POLY_GT3 *)pM_ptr->pPacket);
		mirror_PACKET_GT4(pM_ptr->pTmd2, (POLY_GT4 *)pM_ptr->pPacket2);
		pM_ptr->Poly_rgb = 0x807878;
		pM_ptr++;
	} while (parts_num != 0);
	return Mem;
}

/*
 * Init_mirror_weapon - DS 0x8002e4f8
 * Reserve the weapon mirror work areas at Mem and back up the player's
 * parts[11] (weapon) pointers of the mirror model.
 */
/* DS 0x8002e4f8 (Init_mirror_weapon) */
ULONG Init_mirror_weapon(ENEMY_WORK *pEm, ULONG Mem)
{
	PARTS_W *pM_ptr;

	pM_ptr             = (PARTS_W *)pEm->pM_char_ptr;
	m_pl_weapon_tmd    = (ULONG *)Mem;
	m_pl_weapon_packet = (ULONG *)(Mem + 0x3000);
	m_pl_w00_packet2   = pM_ptr[11].pPacket2;
	m_pl_w00_tmd       = pM_ptr[11].pTmd;
	m_pl_w00_packet    = pM_ptr[11].pPacket;
	m_pl_w00_tmd2      = pM_ptr[11].pTmd2;
	return Mem + 0x5200;
}

/*
 * Mirror_weapon_cp - DS 0x8002e548
 * Rebuild the mirrored weapon parts (TMD + packets) after a weapon change
 * and hook them into the mirror model's parts[11].
 */
/* DS 0x8002e548 (Mirror_weapon_cp) */
void Mirror_weapon_cp(ENEMY_WORK *pEm)
{
	long     i;
	long     Weapon_num;
	ULONG    byte;
	ULONG    off0;
	ULONG    Mem;
	ULONG    Top;
	PARTS_W *pP_ptr;
	PARTS_W *pM_ptr;

	pP_ptr       = pEm->pSin_parts_ptr;
	pM_ptr       = (PARTS_W *)m_pl_weapon_parts;
	Mem          = (ULONG)m_pl_weapon_tmd;
	byte         = (ULONG)((TMD_MEM *)pP_ptr[11].pTmd)->ver_top - (ULONG)pP_ptr[11].pTmd;
	pM_ptr->pTmd = (ULONG *)Mem;
	Memcpy((void *)Mem, pP_ptr[11].pTmd, byte);
	Mem += byte;
	Memcpy((void *)Mem, (void *)((TMD_MEM *)pP_ptr[11].pTmd)->prim_top,
		   pP_ptr[11].pTmd[-3] - ((ULONG)((TMD_MEM *)pP_ptr[11].pTmd)->prim_top + 0xc - (ULONG)pP_ptr[11].pTmd));

	pM_ptr     = (PARTS_W *)m_pl_weapon_parts;
	byte       = (ULONG)pM_ptr->pTmd - (ULONG)pP_ptr[11].pTmd;
	Top        = (byte >> 2) << 2;
	off0       = byte - ((ULONG)((TMD_MEM *)pP_ptr[11].pTmd)->prim_top - (ULONG)((TMD_MEM *)pP_ptr[11].pTmd)->ver_top);
	Weapon_num = pP_ptr[11].pTmd[-1] >> 1;
	i          = Weapon_num;
	pP_ptr     = &pEm->pSin_parts_ptr[pEm->Parts_num - Weapon_num];
	do
	{
		pM_ptr->pTmd       = (ULONG *)((ULONG)pP_ptr->pTmd + Top);
		pM_ptr->pTmd2      = (ULONG *)((ULONG)pP_ptr->pTmd2 + Top);
		*(ULONG *)&((TMD_MEM *)pM_ptr->pTmd)->prim_top += off0;
		*(ULONG *)&((TMD_MEM *)pM_ptr->pTmd2)->prim_top += off0;
		i--;
		pM_ptr->pSuper     = pP_ptr->pSuper;
		pM_ptr->pOya_parts = pP_ptr->pOya_parts;
		pP_ptr++;
		mirror_TMD_GT3(pM_ptr->pTmd);
		mirror_TMD_GT4(pM_ptr->pTmd2);
		pM_ptr++;
	} while (i != 0);

	pM_ptr = (PARTS_W *)m_pl_weapon_parts;
	Mem    = (ULONG)m_pl_weapon_packet;
	do
	{
		Mem = PresetObjectGT3_neo((MODEL_WORK *)&pM_ptr->Attribute, (ULONG *)Mem, 0);
		Mem = PresetObjectGT4_neo((MODEL_WORK *)&pM_ptr->Attribute, (ULONG *)Mem, 0);
		mirror_PACKET_GT3(pM_ptr->pTmd, (POLY_GT3 *)pM_ptr->pPacket);
		Weapon_num--;
		mirror_PACKET_GT4(pM_ptr->pTmd2, (POLY_GT4 *)pM_ptr->pPacket2);
		pM_ptr->Poly_rgb = 0xf0a0a0;
		pM_ptr++;
	} while (Weapon_num != 0);

	pM_ptr = (PARTS_W *)m_pl_weapon_parts;
	pP_ptr = (PARTS_W *)pEm->pM_char_ptr;
	pP_ptr[11].pTmd     = pM_ptr->pTmd;
	pP_ptr[11].pTmd2    = pM_ptr->pTmd2;
	pP_ptr[11].pPacket  = pM_ptr->pPacket;
	pP_ptr[11].pPacket2 = pM_ptr->pPacket2;
}

/*
 * Parts_work_cp - DS 0x8002e788
 * Copy the per-frame animation state of every part into the mirror model.
 */
/* DS 0x8002e788 (Parts_work_cp) */
void Parts_work_cp(ENEMY_WORK *pEm)
{
	ULONG    p_num;
	PARTS_W *pP_ptr;
	PARTS_W *pM_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	pM_ptr = (PARTS_W *)pEm->pM_char_ptr;
	p_num  = pEm->Parts_num;
	do
	{
		pM_ptr->Be_flg     = pP_ptr->Be_flg;
		pM_ptr->M[0][0]    = pP_ptr->M[0][0];	/* 0x18..0x37: M, Pos_x/y/z */
		pM_ptr->M[0][1]    = pP_ptr->M[0][1];
		pM_ptr->M[0][2]    = pP_ptr->M[0][2];
		pM_ptr->M[1][0]    = pP_ptr->M[1][0];
		pM_ptr->M[1][1]    = pP_ptr->M[1][1];
		pM_ptr->M[1][2]    = pP_ptr->M[1][2];
		pM_ptr->M[2][0]    = pP_ptr->M[2][0];
		pM_ptr->M[2][1]    = pP_ptr->M[2][1];
		pM_ptr->M[2][2]    = pP_ptr->M[2][2];
		pM_ptr->Pos_x      = pP_ptr->Pos_x;
		pM_ptr->Pos_y      = pP_ptr->Pos_y;
		pM_ptr->Pos_z      = pP_ptr->Pos_z;
		pM_ptr->Workm      = pP_ptr->Workm;
		pM_ptr->pSuper     = pP_ptr->pSuper;
		pM_ptr->pOya_parts = pP_ptr->pOya_parts;
		pM_ptr->Timer0     = pP_ptr->Timer0;
		pM_ptr->Timer1     = pP_ptr->Timer1;
		pM_ptr->Timer2     = pP_ptr->Timer2;
		pM_ptr->Timer3     = pP_ptr->Timer3;
		pM_ptr->Size_x     = pP_ptr->Size_x;
		pM_ptr->Size_y     = pP_ptr->Size_y;
		pM_ptr->Size_z     = pP_ptr->Size_z;
		pM_ptr->dummy03    = pP_ptr->dummy03;
		pM_ptr->Rot_x      = pP_ptr->Rot_x;
		pM_ptr->Rot_y      = pP_ptr->Rot_y;
		pM_ptr->Rot_z      = pP_ptr->Rot_z;
		pM_ptr->Sabun_cnt0 = pP_ptr->Sabun_cnt0;
		pM_ptr->Cdir_x     = pP_ptr->Cdir_x;
		pM_ptr->Cdir_y     = pP_ptr->Cdir_y;
		pM_ptr->Cdir_z     = pP_ptr->Cdir_z;
		pM_ptr->dummy01    = pP_ptr->dummy01;
		pM_ptr->Old_x      = pP_ptr->Old_x;
		pM_ptr->Old_y      = pP_ptr->Old_y;
		pM_ptr->Old_z      = pP_ptr->Old_z;
		pM_ptr->Old_x2     = pP_ptr->Old_x2;
		pM_ptr->Free[0]    = pP_ptr->Free[0];	/* 0x98..0xab: Free[0..9] */
		pM_ptr->Free[1]    = pP_ptr->Free[1];
		pM_ptr->Free[2]    = pP_ptr->Free[2];
		pM_ptr->Free[3]    = pP_ptr->Free[3];
		pM_ptr->Free[4]    = pP_ptr->Free[4];
		pM_ptr->Free[5]    = pP_ptr->Free[5];
		pM_ptr->Free[6]    = pP_ptr->Free[6];
		pM_ptr->Free[7]    = pP_ptr->Free[7];
		pM_ptr->Free[8]    = pP_ptr->Free[8];
		pM_ptr->Free[9]    = pP_ptr->Free[9];
		pP_ptr++;
		p_num--;
		pM_ptr++;
	} while (p_num != 0);
}

/*
 * Mirror_view - DS 0x8002e93c
 * pV1 = camera pV0 reflected on the mirror plane (flg 1: X = mxz,
 * flg 0: Z = mxz, otherwise Y = mxz).
 */
/* DS 0x8002e93c (Mirror_view) */
void Mirror_view(RCUT *pV0, RCUT *pV1, UCHAR flg, short mxz)
{
	*pV1 = *pV0;
	if (flg == 1)
	{
		pV1->View_p[0] = pV0->View_p[0] + (mxz - pV0->View_p[0]) * 2;
		pV1->View_r[0] = pV0->View_r[0] + (mxz - pV0->View_r[0]) * 2;
	}
	else if (flg == 0)
	{
		pV1->View_p[2] = pV0->View_p[2] + (mxz - pV0->View_p[2]) * 2;
		pV1->View_r[2] = pV0->View_r[2] + (mxz - pV0->View_r[2]) * 2;
	}
	else
	{
		pV1->View_p[1] = pV0->View_p[1] + (mxz - pV0->View_p[1]) * 2;
		pV1->View_r[1] = pV0->View_r[1] + (mxz - pV0->View_r[1]) * 2;
	}
}

/*
 * Mirror_in_view - DS 0x8002ea40
 * Is pos visible in the mirror from camera pV0? ck = flg & 0x7f selects the
 * mirror axis (0: plane Z = Mirror_pos, 1: plane X = Mirror_pos). The ray
 * camera->pos must cross the plane between Mirror_min and Mirror_max.
 */
/* DS 0x8002ea40 (Mirror_in_view) */
UCHAR Mirror_in_view(RCUT *pV0, UCHAR flg, VECTOR *pos)
{
	ULONG ck;
	long  D5;
	long  Dp;
	long  Dv;

	ck = flg & 0x7f;
	Dp = (&pos->vx)[2 - ck * 2] - G.Mirror_pos;
	Dv = pV0->View_p[2 - ck * 2] - G.Mirror_pos;
	if ((Dp ^ Dv) < 0)
	{
		return 0;
	}
	/* DS divides with the 3-operand div macro (zero/overflow checks) */
	DIV_S(D5, ((&pos->vx)[ck * 2] - pV0->View_p[ck * 2]) * Dv, Dp + Dv);
	D5 += pV0->View_p[ck * 2];
	return (ULONG)(D5 - G.Mirror_min) < (ULONG)(G.Mirror_max - G.Mirror_min);
}

/*
 * Mirror_trans - DS 0x8002eaf8
 * Transform the mirror model of pEm with the mirrored world/screen matrix.
 */
/* DS 0x8002eaf8 (Mirror_trans) */
void Mirror_trans(ENEMY_WORK *pEm)
{
	MATRIX   T_Wsmatrix;
	PARTS_W *pTmp;
	ULONG    Frame_pad[12];		/* retail reserves 0x80 bytes; its locals are sp+0x30..0x4c only */

	(void)Frame_pad;

	if (G.Mirror_flg != 2
	 && Mirror_in_view(&G.pRoom->pRcut[G.SaveData.Cut_no], G.Mirror_flg, (VECTOR *)&pEm->Pos_x) == 0)
	{
		return;
	}
	Parts_work_cp(pEm);
	pTmp                = pEm->pSin_parts_ptr;
	pEm->pSin_parts_ptr = (PARTS_W *)pEm->pM_char_ptr;
	T_Wsmatrix          = GsWSMATRIX;
	GsWSMATRIX          = GsMWSMATRIX;
	Mirror_parts_view_ck(pEm);
	/* retail Joint_trans2 takes (pEm, pP_ptr, Be_flg, pPos) */
	Joint_trans2(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, (VECTOR *)&pEm->Pos_x);
	GsWSMATRIX          = T_Wsmatrix;
	pEm->pSin_parts_ptr = pTmp;
}

/*
 * Mirror_parts_view_ck - DS 0x8002ec70 (retail only)
 * For every mirror part with bit 0x8000 set: clear the 0x4000 (hidden) bit
 * and set it again when the part's position is not visible in the mirror.
 */
/* DS 0x8002ec70 (Mirror_parts_view_ck) */
void Mirror_parts_view_ck(ENEMY_WORK *pEm)
{
	PARTS_W *pM_ptr;
	ULONG    p_num;
	UCHAR    flg;
	RCUT    *pRcut;

	pM_ptr = (PARTS_W *)pEm->pM_char_ptr;
	p_num  = pEm->Parts_num;
	flg    = G.Mirror_flg;
	pRcut  = &G.pRoom->pRcut[G.SaveData.Cut_no];
	do
	{
		if (pM_ptr->Be_flg & 0x8000)
		{
			pM_ptr->Be_flg &= ~0x4000;
			if (Mirror_in_view(pRcut, flg, (VECTOR *)pM_ptr->Workm.t) == 0)
			{
				pM_ptr->Be_flg |= 0x4000;
			}
		}
		p_num--;
		pM_ptr++;
	} while (p_num != 0);
}

/*
 * Mirror_matrix_set - DS 0x8002ed30
 * GsMWSMATRIX = (axis flip) * world/screen matrix of the reflected camera.
 * GsWSMATRIX is preserved.
 */
/* DS 0x8002ed30 (Mirror_matrix_set) */
void Mirror_matrix_set(void)
{
	MATRIX m;
	MATRIX T_Wsmatrix;
	RCUT   M_cut;

	Mirror_view(&G.pRoom->pRcut[G.SaveData.Cut_no], &M_cut, G.Mirror_flg, G.Mirror_pos);
	T_Wsmatrix = GsWSMATRIX;
	Set_view(M_cut.View_p, M_cut.View_r);
	m = GsIDMATRIX;
	if (G.Mirror_flg == 2)
	{
		m.m[1][1] = -m.m[1][1];
	}
	else
	{
		m.m[0][0] = -m.m[0][0];
	}
	CompM(&m, &GsWSMATRIX, &GsMWSMATRIX);
	GsWSMATRIX = T_Wsmatrix;
}

/*
 * Late_set - DS 0x8002eeb4
 * Set the semi-transparency rate (tpage bits 5-6) of all packets of pMw.
 */
/* DS 0x8002eeb4 (Late_set) */
void Late_set(MODEL_WORK *pMw, USHORT Late)
{
	POLY_GT3 *gt3;
	POLY_GT4 *gt4;
	ULONG     i;
	ULONG    *pT_head;

	gt4     = (POLY_GT4 *)pMw->pPacket2;
	gt3     = (POLY_GT3 *)pMw->pPacket;
	pT_head = pMw->pTmd;
	for (i = ((TMD_MEM *)pT_head)->n_poly * 2; i != 0; i--)
	{
		gt3->tpage = (gt3->tpage & 0xff9f) | Late << 5;
		gt3++;
	}
	pT_head = pMw->pTmd2;
	for (i = ((TMD_MEM *)pT_head)->n_poly * 2; i != 0; i--)
	{
		gt4->tpage = (gt4->tpage & 0xff9f) | Late << 5;
		gt4++;
	}
}
