/*
 * PSP.C - Room camera-sprite (PSP) setup and draw module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PSP.C
 */

#include "TYPES.H"
#include "ROOM.H"
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <STRINGS.H>
#include "PSP.H"
#include "ESP_R.H"
#include "SUB.H"
TSPRT *pPsp_prim[2];

PSP   *pPsp_work;

/* DS 0x80049a9c (Psp_init0) */
void Psp_init0(void)
{
	pPsp_work = (PSP *)G.pFree_work;
	G.pFree_work = G.pFree_work + (UINT)G.pRoom->nSprite_max * 4;
}

/* DS 0x80049ad0 (Psp_init1) */
void Psp_init1(void)
{
	pPsp_prim[0] = (TSPRT *)G.pFree_work;
	pPsp_prim[1] = (TSPRT *)(G.pFree_work + (UINT)G.pRoom->nSprite_max * 0x20);
	G.pFree_work = (UCHAR *)((TSPRT *)(G.pFree_work + (UINT)G.pRoom->nSprite_max * 0x20) +
				   G.pRoom->nSprite_max);
}

/* DS 0x80049b1c (Psp_set)
 * Cut sprite table layout (USHORT stream):
 *   [0]              group count
 *   [1]              (high half of first word) total sprite count
 *   group header x4: [0] sprite count, [1] unused, [2] x offset, [3] y offset
 *   sprite records:  uv, xy (x low byte, y high byte), otz, wh
 *                    wh == 0 -> 6-word record: uv, xy, otz, 0, w, h
 *                    otherwise w = h = wh
 */
void Psp_set(void)
{
	USHORT *pSpg;		/* cut sprite table */
	USHORT *pGrp;		/* current group header */
	USHORT *pRec;		/* current sprite record (uv word) */
	PSP    *pSprite;
	ULONG   grp_cnt;	/* groups remaining, also used as id */
	ULONG   spr_cnt;	/* sprites remaining in current group */
	ULONG   idx;		/* sprite work index (decrements) */
	ULONG   nsprite;
	USHORT  uv;
	int     xy;
	int     wh;
	USHORT *pNext;

	pSpg = (USHORT *)G.pRoom->pRcut[G.SaveData.Cut_no].pSp;
	if (*(int *)pSpg == -1) {
		G.pRoom->nSprite = 0;
		return;
	}

	G.pRoom->nSprite = (UCHAR)((UINT)*(int *)pSpg >> 16);
	nsprite = (UINT)G.pRoom->nSprite;
	idx = nsprite;

	pSprite = pPsp_work;
	do {
		pSprite->be_flg = 0;
		nsprite--;
		pSprite++;
	} while (nsprite != 0);

	grp_cnt = (UINT)pSpg[0];
	pGrp    = pSpg + 2;
	pRec    = pGrp + grp_cnt * 4;

	if (grp_cnt == 0) {
		return;
	}

	pGrp = pSpg + 2;
	do {
		spr_cnt = (UINT)pGrp[0];
		do {
			spr_cnt--;
			idx--;
			pSprite = pPsp_work + idx;
			pSprite->be_flg = 1;
			pSprite->id     = (UCHAR)grp_cnt;
			pSprite->otz    = (short)pRec[2];

			uv = pRec[0];
			xy = (int)((UINT)(UCHAR)pRec[1] + (int)(short)pGrp[2] +
				   ((UINT)(UCHAR)(pRec[1] >> 8) + (int)(short)pGrp[3]) * 0x10000);

			if (pRec[3] == 0) {
				/* 6-word record: explicit w/h in words 4 and 5 */
				wh    = (int)((UINT)pRec[5] * 0x10000 + (UINT)pRec[4]);
				pNext = pRec + 6;
			} else {
				wh    = (int)(((UINT)pRec[3] << 16) | pRec[3]);
				pNext = pRec + 4;
			}

			/* fill both primitive buffers (prim[0], prim[1]) for this sprite */
			{
				TSPRT *p = pPsp_prim[0] + idx;

				p->sprt.r0   = 0x80;
				p->sprt.g0   = 0x80;
				p->sprt.b0   = 0x80;
				p->sprt.code = 101;
				*(int *)&p->sprt.x0 = xy;
				*(UINT *)&p->sprt.u0 = (UINT)uv + 0x78000000;
				p->sprt.w = (short)wh;
				p->sprt.h = (short)((UINT)wh >> 16);
			}
			{
				TSPRT *p = pPsp_prim[1] + idx;

				p->sprt.r0   = 0x80;
				p->sprt.g0   = 0x80;
				p->sprt.b0   = 0x80;
				p->sprt.code = 101;
				*(int *)&p->sprt.x0 = xy;
				*(UINT *)&p->sprt.u0 = (UINT)uv + 0x78000000;
				p->sprt.w = (short)wh;
				p->sprt.h = (short)((UINT)wh >> 16);
			}
			pRec = pNext;
		} while (spr_cnt != 0);

		pGrp += 4;
		grp_cnt--;
	} while (grp_cnt != 0);
}

/* DS 0x80049ca8 (Psp_trans) */
void Psp_trans(void)
{
	ULONG   flg;
	UINT    nsp;
	int     cut;
	PSP    *pSprite;
	TSPRT  *p;

	SetGeomOffset(0xa0, 0x78);
	nsp     = (UINT)G.pRoom->nSprite;
	cut     = (int)G.SaveData.Cut_no;
	pSprite = pPsp_work + nsp;

	while (nsp != 0) {
		nsp--;
		pSprite--;
		if (((pSprite->be_flg & 1) != 0) &&
			(flg = Flg_ck(G.SaveData.Pri_be_flg[cut], (UINT)pSprite->id), flg != 0)) {
			p = pPsp_prim[G.Cbuf_id] + nsp;
			SetSprt((SPRT *)&p->sprt);
			SetDrawMode((DR_MODE *)p, 1, 0, 0x95, (RECT *)0);
			MargePrim(p, &p->sprt);
			AddPrim(G.pOt1 + (USHORT)pSprite->otz, p);
		}
	}
}

/* DS 0x80049df0 (Psp_ctr) */
void Psp_ctr(UCHAR psp_id, UCHAR st)
{
	PSP  *pPsp;
	UINT  nsp;
	UCHAR nsp_total;
	ULONG Stk_rsv[2];		/* retail reserves 8 bytes here (addiu sp,-8), never accessed */
	(void)Stk_rsv;

	nsp_total = G.pRoom->nSprite;
	nsp   = 0;
	pPsp  = pPsp_work;
	if (nsp_total != 0) {
		do {
			if (pPsp->id == psp_id) {
				pPsp->be_flg = st;
			}
			nsp++;
			pPsp++;
		} while (nsp < nsp_total);
	}
}
