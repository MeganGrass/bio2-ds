/*
 * POLY_SUB.C - Polygon effect (POLY_EFF) init, move and sort
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\POLY_SUB.C
 */

#include "TYPES.H"
#include "MAIN.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include "POLY_SUB.H"
#include "EM_SUB.H"
#include "MD_SUB.H"
#include "ESP_R.H"

/* DS 0x80048f24 (Poly_eff2_init) */
void Poly_eff2_init(void)
{
	ULONG i;
	POLY_EFF2 *pPe;
	POLY_EFF2_WORK *pPew;
	ULONG *pRgb;
	CHAR *pTim;

	i = 0x20;
	pPe = Poly_eff2;
	pPew = Poly2_work;
	/* pRgb walks the Rgb words (Be_flg is 30 bytes below Rgb5), pTim walks
	 * Poly2_work[].Timer1; both advance with the loop, as in the retail code */
	pRgb = &Poly_eff2[0].Rgb5;
	pTim = &Poly2_work[0].Timer1;
	do {
		*(UCHAR *)((char *)pRgb - 0x1e) = 0;	/* POLY_EFF2.Be_flg of this element (pRgb == &pPe->Rgb5) */
		SetPolyGT4((POLY_GT4 *)pPe->Prim0);
		SetPolyGT4(&pPe->Prim0[1]);
		SetPolyGT4((POLY_GT4 *)pPe->Prim1);
		SetPolyGT4(&pPe->Prim1[1]);
		pRgb[-5] = 0x808080;
		pRgb[-4] = 0x808080;
		pRgb[-3] = 0x808080;
		pRgb[-2] = 0x808080;
		pRgb[-1] = 0x808080;
		pRgb[0] = 0x808080;
		pRgb += 0x4a;
		pPe++;
		pPew->Be_flg = 0;
		/* POLY_EFF2_WORK of this element: _R0, _R1, Timer0 (SHORT), Timer1, _R2 */
		pTim[-2] = 0;
		pTim[-1] = 0;
		*(short *)(pTim + 2) = 0;
		*pTim = 0;
		pTim[1] = 0;
		pTim += 0xc;
		i--;
		pPew++;
	} while (i != 0);
}

/* DS 0x80048ffc (Move_poly_eff2)
 * pPe is set to Poly_eff2 at entry, so nothing carries over from earlier passes.
 */
void Move_poly_eff2(void)
{
	UCHAR rnd;
	ULONG i;
	POLY_EFF2 *pPe;
	SVECTOR Psv0;
	SVECTOR Tsv;
	SVECTOR Bv;			/* running base point, kept in memory as in DS */
	MATRIX M;
	UCHAR  frame_pad[32];	/* retail reserves 32 bytes after M and never reads them */
	POLY_EFF2_WORK *pW0;

	i = 8;
	pPe = Poly_eff2;
	pW0 = &Poly2_work[0];
	pW0->Be_flg = 1;
	if (pW0->_R0 == 0) {
		do {
			pPe->Be_flg = 1;
			SetSemiTrans(pPe->Prim0, 1);
			SetSemiTrans(&pPe->Prim0[1], 1);
			SetSemiTrans(pPe->Prim1, 1);
			SetSemiTrans(&pPe->Prim1[1], 1);
			pPe->Prim0[0].tpage = GetTPage(2, 0, 0x40, 0xf0);
			pPe->Prim0[0].clut  = GetClut(0, 0x1e2);
			pPe->Prim0[1].tpage = GetTPage(2, 0, 0x40, 0);
			pPe->Prim0[1].clut  = GetClut(0, 0x1e2);
			pPe->Prim0[0].u0 = 0x20; pPe->Prim0[0].v0 = 0x20;
			pPe->Prim0[0].u2 = 0x20; pPe->Prim0[0].v1 = 0x20;
			pPe->Prim0[0].u1 = 0xa0; pPe->Prim0[0].v2 = 0xa0;
			pPe->Prim0[0].u3 = 0xa0; pPe->Prim0[0].v3 = 0xa0;
			pPe->Prim0[1].u0 = 0x20; pPe->Prim0[1].v0 = 0x20;
			pPe->Prim0[1].u2 = 0x20; pPe->Prim0[1].v1 = 0x20;
			pPe->Prim0[1].u1 = 0xa0; pPe->Prim0[1].v2 = 0xa0;
			pPe->Prim0[1].u3 = 0xa0; pPe->Prim0[1].v3 = 0xa0;
			pPe->Prim1[0].tpage = GetTPage(2, 0, 0, 0xf0);
			pPe->Prim1[0].clut  = GetClut(0, 0x1e2);
			pPe->Prim1[1].tpage = GetTPage(2, 0, 0, 0);
			pPe->Prim1[1].clut  = GetClut(0, 0x1e2);
			pPe->Prim1[0].u0 = 0x20; pPe->Prim1[0].v0 = 0x20;
			pPe->Prim1[0].u2 = 0x20; pPe->Prim1[0].v1 = 0x20;
			pPe->Prim1[0].u1 = 0xa0; pPe->Prim1[0].v2 = 0xa0;
			pPe->Prim1[0].u3 = 0xa0; pPe->Prim1[0].v3 = 0xa0;
			pPe->Prim1[1].u0 = 0x20; pPe->Prim1[1].v0 = 0x20;
			pPe->Prim1[1].u2 = 0x20; pPe->Prim1[1].v1 = 0x20;
			pPe->Prim1[1].u1 = 0xa0; pPe->Prim1[1].v2 = 0xa0;
			pPe->Prim1[1].u3 = 0xa0; pPe->Prim1[1].v3 = 0xa0;
			pPe->Cdir_x = 0;
			pPe->Cdir_y = 0;
			pPe->Cdir_z = 0;
			i--;
			pPe++;
		} while (i != 0);
		pW0->_R0 = 1;
	} else if (pW0->_R0 != 1) {
		return;
	}

	/* base point from the player; each effect's base is the previous effect's Sv3 */
	Bv.vx = (short)G.Player.Pos_x;
	Bv.vy = (short)G.Player.Pos_y - 2000;
	Bv.vz = (short)G.Player.Pos_z;

	i = 8;
	do {
		rnd = Rnd();
		pPe->Cdir_y = G.Player.Cdir_y + ((rnd & 0x7f) - 0x40);
		RotMatrix((SVECTOR *)&pPe->Cdir_x, &M);
		Psv0.vx = 0x226;
		Psv0.vy = 0;
		Psv0.vz = 0;
		ApplyMatrixSV(&M, &Psv0, &Tsv);

		pPe->Sv0.vx = Bv.vx;
		pPe->Sv0.vy = Bv.vy - 200;
		pPe->Sv0.vz = Bv.vz;
		pPe->Sv1.vx = pPe->Sv0.vx + Tsv.vx;
		pPe->Sv1.vy = pPe->Sv0.vy;
		pPe->Sv1.vz = pPe->Sv0.vz + Tsv.vz;
		pPe->Sv2.vx = Bv.vx;
		pPe->Sv2.vy = Bv.vy;
		pPe->Sv2.vz = Bv.vz;
		pPe->Sv3.vx = pPe->Sv2.vx + Tsv.vx;
		pPe->Sv3.vy = pPe->Sv2.vy;
		pPe->Sv3.vz = pPe->Sv2.vz + Tsv.vz;
		/* Sv4 is (x, x+200, x) in the retail code */
		pPe->Sv4.vx = Bv.vx;
		pPe->Sv4.vy = Bv.vx + 200;
		pPe->Sv4.vz = Bv.vx;
		pPe->Sv5.vx = pPe->Sv4.vx + Tsv.vx;
		pPe->Sv5.vy = pPe->Sv4.vy + 200;
		pPe->Sv5.vz = pPe->Sv4.vz + Tsv.vz;

		Bv.vx = pPe->Sv3.vx;
		Bv.vy = pPe->Sv3.vy;
		Bv.vz = pPe->Sv3.vz;
		i--;
		pPe++;
	} while (i != 0);
}

/* DS 0x800493a0 (Sort_poly_eff2) */
void Sort_poly_eff2(void)
{
	POLY_EFF2 *pPe;
	POLY_GT4 *pGt40;
	POLY_GT4 *pGt41;
	ULONG otz;
	ULONG *ot;
	ULONG *ot_base;
	ULONG i;
	MATRIX Tm;
	MATRIX Tm1;

	(void)Tm;
	(void)Tm1;
	i = 0x20;
	pPe = Poly_eff2;
	ot_base = G.Ot1[G.Cbuf_id];
	gte_SetRotMatrix((MATRIX *)&GsWSMATRIX);
	do {
		if ((pPe->Be_flg & 1) == 0) {
			return;
		}
		pGt40 = &pPe->Prim0[G.Cbuf_id];
		pGt41 = &pPe->Prim1[G.Cbuf_id];
		gte_ldv3((SVECTOR *)pPe, (SVECTOR *)&pPe->Sv1, (SVECTOR *)&pPe->Sv2);
		gte_rtpt();
		gte_stsxy3_gt3(&pGt40->tag);
		pGt41->x0 = pGt40->x2;
		pGt41->y0 = pGt40->y2;
		gte_ldv3((SVECTOR *)&pPe->Sv3, (SVECTOR *)&pPe->Sv4, (SVECTOR *)&pPe->Sv5);
		gte_rtpt();
		gte_stsxy3((long *)&pGt41->x1, (long *)&pGt41->x2, (long *)&pGt41->x3);
		pGt40->x3 = pGt41->x1;
		pGt40->y3 = pGt41->y1;
		gte_avsz4();
		gte_stotz((long *)&otz);
		if (otz > 0x1fff) {
			otz = 0x1fff;
		}

		/* colour words: keep each word's code byte, load the gradient RGB */
		*(ULONG *)&pGt40->r0 = (*(ULONG *)&pGt40->r0 & 0xff000000) | pPe->Rgb0;
		*(ULONG *)&pGt40->r1 = (*(ULONG *)&pGt40->r1 & 0xff000000) | pPe->Rgb1;
		*(ULONG *)&pGt40->r2 = (*(ULONG *)&pGt40->r2 & 0xff000000) | pPe->Rgb2;
		*(ULONG *)&pGt40->r3 = (*(ULONG *)&pGt40->r3 & 0xff000000) | pPe->Rgb3;
		*(ULONG *)&pGt41->r0 = (*(ULONG *)&pGt41->r0 & 0xff000000) | *(ULONG *)&pGt40->r2;
		*(ULONG *)&pGt41->r1 = *(ULONG *)&pGt40->r3;
		*(ULONG *)&pGt41->r2 = (*(ULONG *)&pGt41->r2 & 0xff000000) | pPe->Rgb4;
		*(ULONG *)&pGt41->r3 = (*(ULONG *)&pGt41->r3 & 0xff000000) | pPe->Rgb5;

		/* OT slot is computed from the table base for every effect */
		ot = (ULONG *)((otz >> 3) * 4 + (UINT)ot_base);
		addPrim(ot, pGt40);
		addPrim(ot, pGt41);
		pPe++;
		i--;
	} while (i != 0);
}

/* DS 0x80049644 (Poly_eff_init)
 * Note: the retail loop always touches Poly_eff[0] (no pointer advance).
 */
void Poly_eff_init(void)
{
	ULONG i;
	POLY_EFF *pEff = &Poly_eff[0];

	i = 0x20;
	do {
		pEff->Be_flg = 0;
		pEff->_R0     = 0;
		pEff->_R1     = 0;
		pEff->_R2     = 0;
		pEff->Rgb    = 0x808080;
		SetPolyGT4((POLY_GT4 *)pEff->Prim);
		SetPolyGT4(&pEff->Prim[1]);
		i--;
	} while (i != 0);
}

/* DS 0x800496b8 (Set_poly_eff) */
/* The gradient rgb words are updated through the 32-bit word at r0..code (DS
 * reads the word, masks off rgb with 0xff000000 and ors the colour in), and
 * the Cdir/Pos/pSv4 stores come last, as in DS. */
#define PE_SET_RGB(f, v) (*(ULONG *)&(f) = (*(ULONG *)&(f) & 0xff000000) | (v))
void Set_poly_eff(void)
{
	POLY_EFF *pEff = &Poly_eff[0];

	SetSemiTrans(pEff->Prim,     1);
	SetSemiTrans(&pEff->Prim[1], 1);
	pEff->Prim[0].tpage = GetTPage(2, 0, 0x40, 0xf0);
	pEff->Prim[0].clut  = GetClut(0, 0x1e2);
	pEff->Prim[1].tpage = GetTPage(2, 0, 0x40, 0);
	pEff->Prim[1].clut  = GetClut(0, 0x1e2);
	pEff->Prim[0].u0 = 0x20; pEff->Prim[0].v0 = 0x20;
	pEff->Prim[0].u1 = 0xa0; pEff->Prim[0].v1 = 0x20;
	pEff->Prim[0].u2 = 0x20; pEff->Prim[0].v2 = 0xa0;
	pEff->Prim[0].u3 = 0xa0; pEff->Prim[0].v3 = 0xa0;
	pEff->Prim[1].u0 = 0x20; pEff->Prim[1].v0 = 0x20;
	pEff->Prim[1].u1 = 0xa0; pEff->Prim[1].v1 = 0x20;
	pEff->Prim[1].u2 = 0x20; pEff->Prim[1].v2 = 0xa0;
	pEff->Prim[1].u3 = 0xa0; pEff->Prim[1].v3 = 0xa0;
	/* gradient rgb: 0x000000, 0x404040, 0x808080, 0xf0f0f0 */
	PE_SET_RGB(pEff->Prim[0].r0, 0x000000);
	PE_SET_RGB(pEff->Prim[0].r1, 0x404040);
	PE_SET_RGB(pEff->Prim[0].r2, 0x808080);
	PE_SET_RGB(pEff->Prim[0].r3, 0xf0f0f0);
	PE_SET_RGB(pEff->Prim[1].r0, 0x000000);
	PE_SET_RGB(pEff->Prim[1].r1, 0x404040);
	PE_SET_RGB(pEff->Prim[1].r2, 0x808080);
	PE_SET_RGB(pEff->Prim[1].r3, 0xf0f0f0);
	pEff->Cdir_x = 0;
	pEff->Cdir_y = 0;
	pEff->Cdir_z = 0;
	pEff->Pos_x  = 0;
	pEff->Pos_y  = 0;
	pEff->Pos_z  = 0;
	pEff->pSv4   = Poly_test4;
}

/* DS 0x800498f4 (Sort_poly_eff) */
void Sort_poly_eff(void)
{
	POLY_EFF *pPe;
	POLY_GT4 *pGt4;
	ULONG otz;
	ULONG *ot;
	MATRIX Tm;
	MATRIX Tm1;

	pPe  = Poly_eff;
	pGt4 = &pPe->Prim[G.Cbuf_id];
	ot   = G.Ot1[G.Cbuf_id];
	RotMatrix((SVECTOR *)&pPe->Cdir_x, &Tm);
	Tm.t[0] = G.Player.Pos_x + 1000;
	Tm.t[2] = G.Player.Pos_z;
	Tm.t[1] = G.Player.Pos_y - 1000;
	CompM(&GsWSMATRIX, &Tm, &Tm1);
	SetRotMatrix(&Tm1);
	SetTransMatrix(&Tm1);
	gte_ldv3c((SVECTOR *)pPe->pSv4);
	gte_rtpt();
	gte_stsxy3_gt3(&pGt4->tag);
	gte_ldv0(&pPe->pSv4[3]);
	gte_rtps();
	gte_stsxy((long *)&pGt4->x3);
	gte_avsz4();
	gte_stotz((long *)&otz);
	if (otz > 0x1fff) {
		otz = 0x1fff;
	}
	ot = (ULONG *)((otz >> 3) * 4 + (UINT)ot);
	addPrim(ot, pGt4);
}

/* DS 0x800a70c8 (Poly_test4) */
SVECTOR Poly_test4[4] = {
	{ -1000, -1000, 0x0000, 0x0000 },
	{ 0x03e8, -1000, 0x0000, 0x0000 },
	{ -1000, 0x03e8, 0x0000, 0x0000 },
	{ 0x03e8, 0x03e8, 0x0000, 0x0000 }
};

/* DS 0x800c4348 (Poly2_work) */
POLY_EFF2_WORK Poly2_work[16];

/* DS 0x800dbb98 (Poly_eff) */
POLY_EFF Poly_eff[32];

/* DS 0x800dce30 (Poly_eff2) */
POLY_EFF2 Poly_eff2[32];
