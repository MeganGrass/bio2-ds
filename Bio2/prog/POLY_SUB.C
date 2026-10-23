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

/* Reset all 32 POLY_EFF2 slots at once: each slot's four GT4 prims are set up, its six RGB words go to
 * mid grey (0x808080), and its Be_flg and timers are cleared so no slot is active. */
/* DS stack frame: 0x30 bytes; s0..s5 saved at sp+0x10..0x24, ra saved at sp+0x28. No local variable slots are
 * used. */
/* DS 0x80048f24 (Poly_eff2_init) */
void Poly_eff2_init(void)
{
	ULONG i;
	POLY_EFF2 *pPe;
	POLY_EFF2_WORK *pPew;

	i = 32;
	pPe = Poly_eff2;
	pPew = Poly2_work;
	do {
		pPe->Be_flg = 0;
		SetPolyGT4(&pPe->Prim0[0]);
		SetPolyGT4(&pPe->Prim0[1]);
		SetPolyGT4(&pPe->Prim1[0]);
		SetPolyGT4(&pPe->Prim1[1]);
		pPe->Rgb0 = 0x808080;
		pPe->Rgb1 = 0x808080;
		pPe->Rgb2 = 0x808080;
		pPe->Rgb3 = 0x808080;
		pPe->Rgb4 = 0x808080;
		pPe->Rgb5 = 0x808080;
		pPe++;
		pPew->Be_flg = 0;
		pPew->_R0 = 0;
		pPew->_R1 = 0;
		pPew->_R2 = 0;
		pPew->Timer0 = 0;
		pPew->Timer1 = 0;
		pPew++;
		i--;
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
	MATRIX M[2];		/* M[1] is a frame pad: retail reserves 0x28..0x67 and reads only M[0] */
	POLY_EFF2_WORK *pW0;
	tagGLOBAL *pG;

	i = 8;
	pPe = Poly_eff2;
	pW0 = &Poly2_work[0];
	pG = &G;
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
	Bv.vx = (short)pG->Player.Pos_x;
	Bv.vy = (short)pG->Player.Pos_y - 2000;
	Bv.vz = (short)pG->Player.Pos_z;

	i = 8;
	do {
		rnd = Rnd();
		pPe->Cdir_y = pG->Player.Cdir_y + ((rnd & 0x7f) - 0x40);
		RotMatrix((SVECTOR *)&pPe->Cdir_x, M);
		Psv0.vx = 0x226;
		Psv0.vy = 0;
		Psv0.vz = 0;
		ApplyMatrixSV(M, &Psv0, &Tsv);

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

/* DS stack frame: 0x48 bytes; no registers saved. Frame slots: sp+0x40 (address taken; word
 * read/written). */
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
	POLY_EFF *pEff;

	i = 0x20;
	pEff = &Poly_eff[0];
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

/* DS stack frame: 0x18 bytes; s0 saved at sp+0x10, ra saved at sp+0x14. No local variable slots are used. */
/* DS 0x800496b8 (Set_poly_eff) */
/* The gradient rgb words are updated through the 32-bit word at r0..code (DS
 * reads the word, masks off rgb with 0xff000000 and ors the colour in), and
 * the Cdir/Pos/pSv4 stores come last, as in DS. */
#define PE_SET_RGB(f, v) (*(ULONG *)&(f) = (*(ULONG *)&(f) & 0xff000000) | (v))
void Set_poly_eff(void)
{

	SetSemiTrans(Poly_eff[0].Prim,     1);
	SetSemiTrans(&Poly_eff[0].Prim[1], 1);
	Poly_eff[0].Prim[0].tpage = GetTPage(2, 0, 0x40, 0xf0);
	Poly_eff[0].Prim[0].clut  = GetClut(0, 0x1e2);
	Poly_eff[0].Prim[1].tpage = GetTPage(2, 0, 0x40, 0);
	Poly_eff[0].Prim[1].clut  = GetClut(0, 0x1e2);
	Poly_eff[0].Prim[0].u0 = 0x20; Poly_eff[0].Prim[0].v0 = 0x20;
	Poly_eff[0].Prim[0].u1 = 0xa0; Poly_eff[0].Prim[0].v1 = 0x20;
	Poly_eff[0].Prim[0].u2 = 0x20; Poly_eff[0].Prim[0].v2 = 0xa0;
	Poly_eff[0].Prim[0].u3 = 0xa0; Poly_eff[0].Prim[0].v3 = 0xa0;
	Poly_eff[0].Prim[1].u0 = 0x20; Poly_eff[0].Prim[1].v0 = 0x20;
	Poly_eff[0].Prim[1].u1 = 0xa0; Poly_eff[0].Prim[1].v1 = 0x20;
	Poly_eff[0].Prim[1].u2 = 0x20; Poly_eff[0].Prim[1].v2 = 0xa0;
	Poly_eff[0].Prim[1].u3 = 0xa0; Poly_eff[0].Prim[1].v3 = 0xa0;
	/* gradient rgb: 0x000000, 0x404040, 0x808080, 0xf0f0f0 */
	PE_SET_RGB(Poly_eff[0].Prim[0].r0, 0x000000);
	PE_SET_RGB(Poly_eff[0].Prim[0].r1, 0x404040);
	PE_SET_RGB(Poly_eff[0].Prim[0].r2, 0x808080);
	PE_SET_RGB(Poly_eff[0].Prim[0].r3, 0xf0f0f0);
	PE_SET_RGB(Poly_eff[0].Prim[1].r0, 0x000000);
	PE_SET_RGB(Poly_eff[0].Prim[1].r1, 0x404040);
	PE_SET_RGB(Poly_eff[0].Prim[1].r2, 0x808080);
	PE_SET_RGB(Poly_eff[0].Prim[1].r3, 0xf0f0f0);
	Poly_eff[0].Cdir_x = 0;
	Poly_eff[0].Cdir_y = 0;
	Poly_eff[0].Cdir_z = 0;
	Poly_eff[0].Pos_x  = 0;
	Poly_eff[0].Pos_y  = 0;
	Poly_eff[0].Pos_z  = 0;
	Poly_eff[0].pSv4   = Poly_test4;
}

/* DS stack frame: 0x70 bytes; s0..s3 saved at sp+0x58..0x64, ra saved at sp+0x68. Locals: Tm: sp+0x10, 32 bytes;
 * Tm1: sp+0x30, 32 bytes. Other frame slots: sp+0x50 (address taken; word read/written). */
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
	Tm.t[1] = G.Player.Pos_y - 1000;
	Tm.t[2] = G.Player.Pos_z;
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

/* DS 0x800a70e8 (ascii_tbl2): 16 characters and the terminating NUL; retail pads 3 bytes after it, before Rdt_tbl1 at 0x800a70fc */
CHAR ascii_tbl2[20] = "123456789abcdefg";

/* DS 0x800c4348 (Poly2_work)
 * 16 entries of 12 bytes (0xc0): the next label, add_y, is at 0x800c4408 (the beta map has the same layout,
 * Poly2_work [16] at 0x800B6C78 and add_y 0xC0 bytes after it). Poly_eff2_init steps 32 entries of 0xc (retail
 * 0x80048f24..0x80048fcc), which runs 0xc0 bytes past the array over add_y, the SDK words, Reverb_flg and the start
 * of Trans, but no code in the main executable or in any overlay calls it. Move_poly_eff2 only touches entry 0
 * (retail 0x80049044 sb to 0x800c4348). */
POLY_EFF2_WORK Poly2_work[16];

/* DS 0x800dbb98 (Poly_eff) */
POLY_EFF Poly_eff[32];

/* DS 0x800dce30 (Poly_eff2) */
POLY_EFF2 Poly_eff2[32];
