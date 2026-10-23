/*
 * TRANS.C - Effect sprite and primitive transform routines
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\TRANS.C
 */
#include "TYPES.H"
#include "CONFIG.H"
#include "ROOM.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <TYPES.H>
#include "TRANS.h"
#include "DIEDEMO.H"
#include "ESP.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "ST_SUB00.H"
#include "flag.h"
#include "DIVCHK.H"

/* DS 0x800c4418 (Trans): transform work buffer, 2 x 384 POLY_FT4 plus the keep pointer; the file bytes are all zero */
TRANS Trans;

/* DS 0x800ab218 (Poly_ft4_count): written by Prim_trans; the file bytes are all zero */
ULONG Poly_ft4_count = 0;

/* Per-frame transform step: runs the effect sprite pass, then records the high-water mark of polygons used
 * this frame in the current buffer (Poly_ft4_count), so the buffer size can be checked. */
/* DS 0x80077874 (Prim_trans) */
VOID Prim_trans(VOID)
{
	auto SVECTOR Svec;				// FFFFFFF0		// 8 bytes
	register ULONG D0;
	(void)Svec;		/* unused; retail Prim_trans frame 0x20 needs this slot */

	Eff_sprt_trans();
	D0 = (ULONG)((int)Trans.pPoly_ft4_keep - (int)Trans.Prim_poly_ft4[G.Cbuf_id]) / sizeof(POLY_FT4);
	if (Poly_ft4_count < D0) {
		Poly_ft4_count = D0;
	}
}

/* Start the current frame's polygon list: the keep pointer is set to the head of this buffer's
 * POLY_FT4 array (double-buffered by Cbuf_id). */
/* DS 0x800778f8 (Trans_pointer_set) */
VOID Trans_pointer_set(VOID)
{
	Trans.pPoly_ft4_keep = Trans.Prim_poly_ft4[G.Cbuf_id];
}

/* Effect sprite pass: each active ESP work item is turned into POLY_FT4 quads. Each item goes through
 * Esp_trans_main or Esp_trans_3d (the branch depends on the item's animation sequence), both with the
 * projection for this frame. */
/* DS 0x80077924 (Eff_sprt_trans) */
VOID Eff_sprt_trans(VOID)
{
	register POLY_FT4 * pP;				// 40 bytes
	auto POLY_FT4 * pP_limit;			// FFFFFFA8		// 40 bytes
	register ESP_WORK * pEff;			// 124 bytes
	register SSEQUENCE * pAseq;			// 8 bytes
	auto VCUT * pCcut;					// FFFFFFB0		// 20 bytes
	register ULONG nSpr;
	auto ULONG * pOt;					// FFFFFFB8
	auto LONG Proj;						// FFFFFFC0
	register ULONG D2;
	register ULONG Be_flg;
	auto ULONG Flg;						// FFFFFFC8
	auto MATRIX m;						// FFFFFF38		// 32 bytes
	auto MATRIX workm;					// FFFFFF58		// 32 bytes
	auto MATRIX workm2;					// FFFFFF78		// 32 bytes
	auto VECTOR Vec;					// FFFFFF98		// 16 bytes
	/* retail reserves 16 more bytes of locals (Eff_sprt_trans frame 232); never read */
	volatile int Rsv[4];


	pP = Trans.pPoly_ft4_keep;
	pP_limit = Trans.Prim_poly_ft4[G.Cbuf_id] + 384;
	pCcut = G.Cccut;
	Proj = (LONG)((G.pRoom)->Cut[G.SaveData.Cut_no].ViewR >> 7);
	pOt = G.Ot1[G.Cbuf_id];
	SetRotMatrix((MATRIX *)&GsWSMATRIX);
	SetTransMatrix((MATRIX *)&GsWSMATRIX);
	pEff = (ESP_WORK *)pEdt_adr;
	Flg = 0;

loop_start:									// 800779E8
	pEff--;
	/* DS keeps the effect position in the translation of workm (sp+0x54..0x5c): it is the point for
	 * Hit_ck_point4 here, and the 3D branch below overwrites it with Hot + Mov before CompM reads it. */
	workm.t[0] = (LONG)pEff->Pos_x;
	workm.t[1] = (LONG)pEff->Pos_y;
	workm.t[2] = (LONG)pEff->Pos_z;
	Be_flg = pEff->Be_flg;
	pAseq = pEff->pAnmseq + pEff->Anm_cnt;
	nSpr = (ULONG)pAseq->nSpr;

	if ((Be_flg & 0xa000) == 0xa000) {
		if (Hit_ck_point4((VECTOR *)workm.t, pCcut) != 0 && nSpr != 0) {
			D2 = 0x2c;
			if ((Be_flg & 0x1000) != 0) {
				D2 = 0x2e;
			}
			if ((Be_flg & 0x200) == 0) {
				if (Flg != 0) {
					gte_SetRotMatrix((MATRIX *)&GsWSMATRIX);
					gte_SetTransMatrix((MATRIX *)&GsWSMATRIX);
					Flg = 0;
				}
				if ((G.Status_flg & STAT_MIRROR_MATRIX) == 0) {
					if (pP_limit <= pP + nSpr) goto kill_effect;
					Esp_trans_main(pP, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
					pP += nSpr;
				} else {
					if (pP_limit <= pP + nSpr + nSpr) goto kill_effect;
					Esp_trans_main(pP, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
					Vec.vx = (LONG)pEff->Pos_x;
					Vec.vy = (LONG)pEff->Pos_y;
					Vec.vz = (LONG)pEff->Pos_z;
					if (Mirror_in_view((G.pRoom)->pRcut + G.SaveData.Cut_no, G.Mirror_flg, (VECTOR *)&Vec) != 0) {
						gte_SetRotMatrix((MATRIX *)&GsMWSMATRIX);
						gte_SetTransMatrix((MATRIX *)&GsMWSMATRIX);
						Esp_trans_main(pP + nSpr, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
						Flg = 1;
						pP += nSpr + nSpr;
					} else {
						pP += nSpr;
					}
				}
			} else {
				/* Local offset of the sprite: RotMatrix fills only workm.m, so CompM transforms this
				 * translation by pEff->m into workm2.t. */
				workm.t[0] = (LONG)pEff->Hot_x + (LONG)pEff->Mov_x;
				workm.t[1] = (LONG)pEff->Hot_y + (LONG)pEff->Mov_y;
				workm.t[2] = (LONG)pEff->Hot_z + (LONG)pEff->Mov_z;
				RotMatrix((SVECTOR *)&pEff->Cdir_x, &workm);
				CompM((MATRIX *)pEff->m, &workm, &workm2);
				CompM(&GsWSMATRIX, &workm2, &m);
				gte_SetRotMatrix(&m);
				gte_SetTransMatrix(&m);
				if ((G.Status_flg & STAT_MIRROR_MATRIX) == 0) {
					if (pP_limit <= pP + nSpr) goto kill_effect;
					Esp_trans_3d(pP, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
					pP += nSpr;
				} else {
					if (pP_limit <= pP + nSpr + nSpr) {
kill_effect:
						pEff->Be_flg = 0;
						goto loop_check;
					}
					Esp_trans_3d(pP, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
					Vec.vx = (LONG)pEff->Pos_x;
					Vec.vy = (LONG)pEff->Pos_y;
					Vec.vz = (LONG)pEff->Pos_z;
					if (Mirror_in_view((G.pRoom)->pRcut + G.SaveData.Cut_no, G.Mirror_flg, (VECTOR *)&Vec) != 0) {
						CompM(&GsMWSMATRIX, &workm2, &m);
						gte_SetRotMatrix(&m);
						gte_SetTransMatrix(&m);
						Esp_trans_3d(pP + nSpr, pEff, pAseq->Twh, nSpr, Proj, pOt, D2, (ULONG)pAseq->pGp);
						pP += nSpr + nSpr;
					} else {
						pP += nSpr;
					}
				}
				Flg = 1;
			}
		}
	}

loop_check:									// 80077E84
	if (pEff == Esp) {
		Trans.pPoly_ft4_keep = pP;
		return;
	}
	goto loop_start;
}

/* DS 0x80077ed0 (Esp_trans_main) */
VOID Esp_trans_main(register POLY_FT4 * pP, register ESP_WORK * pEff, register ULONG Def_size, register ULONG nSpr, LONG Projection, register ULONG * pOt, register ULONG H_clarity, register ULONG pG_offset)
{
	register SPRITEGp * pSpGp;			// 4 bytes
	register LONG Xsize;
	register LONG Ysize;
	register LONG Def_size_s;
	auto LONG D0;						// FFFFFFEC
	auto LONG D1;						// FFFFFFE8
	register LONG D2;
	register LONG D3;
	register ULONG Clut;
	register ULONG Tpage;
	register LONG Hot_x;
	register LONG Hot_y;
	register ULONG Mag_x;
	register ULONG Mag_y;
	register ULONG * pOld_prim;
	register ULONG Sxy;

	gte_ldv0((SVECTOR *)&pEff->Pos_x);
	Def_size_s = (LONG)Def_size << 8;
	gte_rtps_b();
	D2 = (LONG)Def_size * pEff->Mag * Projection;
	D1 = nSpr;
	pSpGp = pEff->pSprg + pG_offset;
	Clut = (ULONG)pEff->Clut << 16;
	Tpage = (ULONG)pEff->Tpage << 16;
	gte_stsz((long *)&D0);
	if ((LONG)D0 >> 9 != 0) {
		if (0x7fff < (ULONG)D0) {
			D0 = 0x7fff;
		}
		pOld_prim = pOt;
		D3 = D0;
		{
			POLY_FT4 * pTmp = pP;
			ULONG * ppOt;
			pOt += D0 >> 5;
			ppOt = pOt;
			do {
				pTmp->tag = (*ppOt & 0xffffff) | 0x9000000;
				*ppOt = (ULONG)pTmp & 0xffffff;
				pTmp++;
				D1--;
			} while (D1 != 0);
		}
		gte_stsxy((long *)&D1);
		Xsize = D0 << 4;
		DIV_S(Mag_x, D2, Xsize);	/* DS: signed div with break checks */
		Ysize = Mag_x * pEff->Transx;
		DIV_U(Hot_x, (ULONG)Ysize, Def_size);	/* DS: divu */
		Mag_y = Mag_x * pEff->Transy;
		DIV_U(Hot_y, Mag_y, Def_size);
		Sxy = (ULONG)D1;	/* DS reads the stsxy result here, then reuses D1 as scratch */
		D1 = (LONG)Mag_x;
		if (0x1ffff < (ULONG)Hot_x) {
			Def_size--;
		}
		if (0x1ffff < (ULONG)Hot_y) {
			Def_size_s = Def_size_s + -0x100;
		}

loop_addprim:								// 8007808C
		pP->code = (UCHAR)H_clarity;
		{
			/* D1 = SXY from gte_stsxy: X in the low halfword, Y in the high halfword.
			   DS reads it once (Sxy) before the loop and stores to D0/D1 as scratch (dead stores kept) */
			ULONG x0 = (Sxy << 16) + (signed char)pSpGp->Ofsx * (LONG)Hot_x;
			ULONG x1 = (x0 + (ULONG)Ysize) >> 16;
			ULONG y0, y1, p0, p1, p2, p3;
			D1 = (LONG)x0;
			x0 >>= 16;
			D1 = (LONG)x0;
			y0 = (Sxy & 0xffff0000) + (signed char)pSpGp->Ofsy * (LONG)Hot_y;
			y1 = (y0 + Mag_y) & 0xffff0000;
			D0 = (LONG)y0;
			y0 &= 0xffff0000;
			D0 = (LONG)y0;
			/* quad corners: 0 = (x0,y0), 1 = (x1,y0), 2 = (x0,y1), 3 = (x1,y1) */
			p1 = x1 | y0;
			p2 = x0 | y1;
			x1 |= y1;
			p0 = x0 | y0;
			p3 = x1;
			*(ULONG *)&pP->x1 = p1;
			*(ULONG *)&pP->x0 = p0;
			*(ULONG *)&pP->x2 = p2;
			*(ULONG *)&pP->x3 = p3;
		}
		{
			ULONG u = (ULONG)pSpGp->U;
			ULONG v;
			ULONG uv0, uv1, uv2, uv3;
			nSpr--;
			D1 = (LONG)u;
			v = (ULONG)pSpGp->V * 0x100;
			pSpGp++;
			uv0 = u | v | Clut;
			D0 = (LONG)v;
			uv1 = (u + Def_size) | v | Tpage;
			uv2 = u | (v + (ULONG)Def_size_s);
			uv3 = (u + Def_size) | (v + (ULONG)Def_size_s);
			*(ULONG *)&pP->u0 = uv0;
			*(ULONG *)&pP->u1 = uv1;
			*(ULONG *)&pP->u2 = uv2;
			*(ULONG *)&pP->u3 = uv3;
		}
		pP++;


		if (nSpr != 0) goto loop_addprim;
	}
}

/* DS 0x80078174 (Esp_trans_3d) */
VOID Esp_trans_3d(register POLY_FT4 * pP, register ESP_WORK * pEff, register ULONG Def_size, register ULONG nSpr, LONG Projection, ULONG * pOt, ULONG H_clarity, ULONG pG_offset)
{
	register SPRITEGp * pSpGp;			// 4 bytes
	register LONG D0;
	register LONG D1;
	register LONG D2;
	register ULONG Clut;
	register ULONG Tpage;
	auto ULONG Otz;						// FFFFFFE0
	SVECTOR *sv;
	auto SVECTOR Svec[4];				// FFFFFFC0		// 32 bytes
	register LONG Mag_x;
	register LONG Mag_y;
	register LONG Size_x;

	sv = Svec;
	D2 = (LONG)((ULONG)pEff->Mag * (ULONG)(Projection >> 4));
	Mag_x = (LONG)((ULONG)D2 * (ULONG)pEff->Transx) >> 12;
	Clut = (ULONG)pEff->Clut << 16;
	Mag_y = (LONG)((ULONG)D2 * (ULONG)pEff->Transy) >> 12;
	Tpage = (ULONG)pEff->Tpage << 16;
	Svec[0].vz = 0;
	Svec[1].vz = 0;
	Svec[2].vz = 0;
	Svec[3].vz = 0;
	pSpGp = pEff->pSprg + pG_offset;
	Size_x = (LONG)Def_size - 1;
	D0 = (LONG)(((ULONG)Def_size * (ULONG)Mag_x) >> 12);
	D1 = (LONG)(((ULONG)Def_size * (ULONG)Mag_y) >> 12);

loop_start:									// 80078228
	sv[0].vx = (short)((signed char)pSpGp->Ofsx * Mag_x >> 12);
	sv[0].vy = (short)((signed char)pSpGp->Ofsy * Mag_y >> 12);
	gte_ldv0(&sv[0]);
	sv[1].vx = (short)(sv[0].vx + (short)D0);
	sv[1].vy = sv[0].vy;
	sv[2].vx = sv[0].vx;
	sv[2].vy = (short)(sv[0].vy + (short)D1);
	sv[3].vx = (short)(sv[0].vx + (short)D0);
	sv[3].vy = (short)(sv[0].vy + (short)D1);
	gte_rtps_b();
	gte_stsxy((long *)&pP->x0);
	gte_stszotz((long *)&Otz);
	gte_ldv3(&sv[1], &sv[2], &sv[3]);
	if (0x1fff < Otz) {
		Otz = 0x1fff;
	}
	gte_rtpt_b();
	if (Otz >> 7 == 0) goto loop_exit;
	{
		ULONG u = (ULONG)pSpGp->U;
		ULONG v = (ULONG)pSpGp->V * 0x100;
		ULONG uv0 = u | v | Clut;
		ULONG uv1 = (u + (ULONG)Size_x) | v | Tpage;
		ULONG uv2 = u | (v + (ULONG)(Size_x * 0x100));
		ULONG uv3 = (u + (ULONG)Size_x) | (v + (ULONG)(Size_x * 0x100));
		*(ULONG *)&pP->u0 = uv0;
		*(ULONG *)&pP->u1 = uv1;
		*(ULONG *)&pP->u2 = uv2;
		*(ULONG *)&pP->u3 = uv3;
	}
	pP->code = (UCHAR)H_clarity;
	pP->tag = (pOt[Otz >> 3] & 0xffffff) | 0x9000000;
	pOt[Otz >> 3] = (ULONG)pP & 0xffffff;
	gte_stsxy3((long *)&pP->x1, (long *)&pP->x2, (long *)&pP->x3);
	pSpGp++;
	pP++;
	nSpr--;
	if (nSpr != 0) goto loop_start;

loop_exit:
	return;
}

/* Initialise both polygon buffers: 0x300 POLY_FT4 entries are cleared to their default tag, colour
 * (0x80 grey) and code, then the keep pointer is reset. */
/* DS 0x800783b4 (Trans_work_init) */
VOID Trans_work_init(VOID)
{
	register POLY_FT4 * pP2;
	register ULONG D2;

	pP2 = (POLY_FT4 *)Trans.Prim_poly_ft4;
	D2 = 0x300;
	do {
		pP2->tag  = 0x9000000;
		*(ULONG *)&pP2->r0 = 0x2c808080;	/* r0 g0 b0 = 0x80, code = 0x2c (',') as one word, as DS */
		pP2++;
		D2--;
	} while (D2 != 0);
	Trans_pointer_set();
}
