/*
 * DIESPR.C - Death demo sprite / blood effect work (overlay DIEDEMO)
 * D:\Bio2\prog\DIESPR.C
 * Overlay DIEDEMO:: 0x80190000 - 0x801938c7 (DS retail)
 *
 * Die_spr_move_main / _hold / _erase / _end are ported from the DIEDEMO.BIN
 * bytes (DS 0x80191bac, 0x801921d8, 0x8019255c, 0x801929c0). They are not in
 * the Ghidra function index, so each body was decoded by hand and compared
 * against the bytes.
 */

#include "TYPES.H"
#include <INLINE_C.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include <MEMORY.H>
#include <TYPES.H>
#include "DIESPR.H"
#include "DIEDEMO.H"
#include "CD.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "MAIN.H"
#include "FILECODE.H"
#include "flag.h"
#include "DIVCHK.H"

/* DS 0x801ad000 (Die_spr_work) */
UCHAR *Die_spr_work = (UCHAR *)0x801ad000;
/* DS 0x801a4220 (Die_vh_src) */
UCHAR *Die_vh_src = (UCHAR *)0x801a4220;
/* DS 0x801a4e88 (Die_vb_buf) */
UCHAR *Die_vb_buf = (UCHAR *)0x801a4e88;

/* ------------------------------------------------------------------ */
/* Static data                                                         */
/* ------------------------------------------------------------------ */

/* Death animation file names by player ID (DS strings 0x80190004..0x8019004c) */
static UCHAR *s_DIED00_DIE = "DIED00.DIE";
static UCHAR *s_DIED01_DIE = "DIED01.DIE";
static UCHAR *s_DIED0E_DIE = "DIED0E.DIE";
static UCHAR *s_DIED0F_DIE = "DIED0F.DIE";
static UCHAR *s_DIED0C_DIE = "DIED0C.DIE";
static UCHAR *s_DIED0D_DIE = "DIED0D.DIE";
static UCHAR *s_DIED0B_DIE = "DIED0B.DIE";

/* Set_view arguments for Die_spr_move: view point DS 0x80193844, view reference DS 0x80193850 */
static long Dispr_vp[3];
static long Dispr_vr[3];

/* Sprite state dispatch table, DS 0x80193860, indexed by pDiespr->Rno */
static void (*Die_spr_tbl[4])(void) = {
	Die_spr_move_main,      /* 0x80191bac */
	Die_spr_move_hold,      /* 0x801921d8 */
	Die_spr_move_erase,     /* 0x8019255c */
	Die_spr_move_end,       /* 0x801929c0 */
};

/* Sprite work pointer, DS 0x801938c4 (points at DS 0x801ad000) */
static DIESPR *pDiespr;


/* ------------------------------------------------------------------ */
/* DS 0x801915b8 (Die_spr_init)                                         */
/* Initialise sprite work area: map TIM, set up GT4/FT4 primitives    */
/* ------------------------------------------------------------------ */
void Die_spr_init(void)
{
	G.Ctpage = 0x15;
	G.Cclut  = 0x1f;
	Mapping_tim(Bg_img_buf);
	pDiespr = (DIESPR *)Die_spr_work;
	pDiespr->Rno = 0;
	pDiespr->Ctr = 0;
	Die_spr_move_init();
	if ((G.System_flg & 0x800) == 0) {
		Die_blood_move_init();
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x8019162c (Die_spr_move)                                         */
/* Per-frame sprite dispatcher: set view matrix then call state handler */
/* ------------------------------------------------------------------ */
void Die_spr_move(void)
{
	MATRIX *pWs;

	Set_view(Dispr_vp, Dispr_vr);
	SetGeomScreen(0xd0);
	pWs = &GsWSMATRIX;
	gte_SetRotMatrix((MATRIX *)&GsWSMATRIX);
	gte_SetTransMatrix((MATRIX *)pWs);
	(*Die_spr_tbl[pDiespr->Rno])();
	if ((G.System_flg & SYS_DIE_BLOOD) != 0) {
		Die_blood_move_main();
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x801916f4 (Die_spr_move_init)                                    */
/* Set up the 2 x 16x16 POLY_GT4 grid, grid vertices and distances     */
/* ------------------------------------------------------------------ */
void Die_spr_move_init(void)
{
	short     Grp;
	short     Row;
	short     Col;
	UCHAR    Wx;
	UCHAR    Wy;
	char      Tx;
	char      Ty;
	int       Rot;
	int       Dx;
	int       Dz;
	int       Vz;
	POLY_GT4 *p;
	SVECTOR  *pPos;
	/* retail reserves 8 more bytes of locals in Die_spr_move_init (frame 88); never read */
	volatile int Rsv0;
	volatile int Rsv1;

	for (Grp = 0; Grp < 2; Grp++) {
		Wy = 0;
		p  = &pDiespr->Pgt4[Grp][0];
		for (Row = 0; Row < 17; Row++) {
			Wx = 0;
			for (Col = 0; Col < 17; Col++) {
				if (Grp != 0) {
					pPos     = &pDiespr->Pos[Col][Row];
					pPos->vx = Col * 0x1e;
					pPos->vz = Row * 11;                    /* (Row*2)*4 + Row*3 */
					pPos->vy = 0x212;
					Rot = rcos(Col << 12);
					Dx  = (short)(0xf0 - Col * 0x1e);
					Vz  = (int)(USHORT)pPos->vz + (Rot >> 9);
					pPos->vz = (short)Vz;
					Dz  = (short)(0x58 - Vz);
					pDiespr->Dist[Col][Row] = (short)SquareRoot0((long)(Dx * Dx + Dz * Dz));
				}
				if ((Row != 0x10) && (Col != 0x10)) {
					SetPolyGT4(p);
					Tx = 0x10;
					if (Col == 0xf) {
						Tx = 0x0f;
					}
					Ty = 0x04;
					if (Row == 0xf) {
						Ty = 0x03;
					}
					p->tpage = 0xb5;
					p->clut = 0x7fc0;
					*(ULONG *)&p->r0 = 0x3e202020;
					*(ULONG *)&p->r1 = 0x3e202020;
					*(ULONG *)&p->r2 = 0x3e202020;
					*(ULONG *)&p->r3 = 0x3e202020;
					p->u2 = Wx;
					p->u0 = Wx;
					p->v0 = Wy;
					p->u1 = Tx + Wx;
					p->v1 = Wy;
					p->v2 = Wy + Ty;
					p->u3 = Tx + Wx;
					p->v3 = Wy + Ty;
					p++;
					Wx += 0x10;
				}
			}
			Wy += 4;
		}
	}

	pDiespr->Free4      = 0x0f;
	pDiespr->Free2      = 0;
	pDiespr->Blood_flg  = 0;
	pDiespr->Free5      = 1;
	pDiespr->Move_start = (G.System_flg & SYS_DIE_BLOOD) ? 0x34 : 0x16;
}

/* ------------------------------------------------------------------ */
/* DS 0x801919f4 (Die_blood_move_init)                                  */
/* Set up 3 x 2 x 16x8 POLY_FT4 blood particle primitives              */
/* ------------------------------------------------------------------ */
void Die_blood_move_init(void)
{
	int       Grp;
	int       Half;
	int       Row;
	int       Col;
	UCHAR    Wx;
	UCHAR    Wy;
	char      Tx;
	char      Ty;
	POLY_FT4 *p;

	for (Grp = 0; Grp < 3; Grp++) {
		for (Half = 0; Half < 2; Half++) {
			Wy = 0x40;
			p  = &pDiespr->Blood[Grp][Half][0][0];
			for (Row = 0; Row < 16; Row++) {
				Wx = 0;
				for (Col = 0; Col < 8; Col++) {
					SetPolyFT4(p);
					p->tpage = 0x95;
					p->clut = 0x7fc0;
					*(ULONG *)&p->r0 = 0x2c000000;
					Tx = 0x20;
					if (Col == 7) {
						Tx = 0x1f;
					}
					Ty = 8;
					if (Row == 0xf) {
						Ty = 7;
					}
					p->u2 = Wx;
					p->u0 = Wx;
					p->v0 = Wy;
					p->u1 = Tx + Wx;
					p->v1 = Wy;
					p->v2 = Wy + Ty;
					p->u3 = Tx + Wx;
					p->v3 = Wy + Ty;
					p++;
					Wx += 0x20;
				}
				Wy += 8;
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Die_spr_move_main helper: colour step for one grid vertex            */
/* Die_spr_move_main grid sizes (17x17 vertices, 16x16 quads) and the frame
 * count after Move_start at which the state advances to Die_spr_move_hold. */
#define DIE_SPR_GRID_VERTS      17
#define DIE_SPR_GRID_QUADS      16
#define DIE_SPR_MAIN_END_FRAMES 48

/* ------------------------------------------------------------------ */
/* DS 0x80191bac (Die_spr_move_main)                                    */
/* Per-frame grid update: vertices fall by Free4, Free4 steps down on   */
/* the Free5 cycle, each 16x16 quad is projected with the GTE and its   */
/* vertex colours brighten by distance, then it is added to the OT.     */
/* Free2 counts frames from the Die_spr_move_init start; after           */
/* Move_start + 48 frames it resets Free2 and advances Rno to hold.      */
/* ------------------------------------------------------------------ */
void Die_spr_move_main(void)
{
	SVECTOR   V[4];
	short     Row;
	short     Col;
	POLY_GT4 *p;
	POLY_GT4 *q;

	pDiespr->Free2++;
	if ((SHORT)pDiespr->Free2 < (SHORT)pDiespr->Move_start) {
		return;
	}

	for (Row = 0; Row < DIE_SPR_GRID_VERTS; Row++) {
		for (Col = 0; Col < DIE_SPR_GRID_VERTS; Col++) {
			if (pDiespr->Free4 != 0) {
				pDiespr->Pos[Col][Row].vy += pDiespr->Free4;
			}
		}
	}

	pDiespr->Free5--;
	if (pDiespr->Free5 == 0) {
		switch (pDiespr->Free4) {
		case 2:
			pDiespr->Free5 = 8;
			break;
		case 3:
			pDiespr->Free5 = 5;
			break;
		case 4:
			pDiespr->Free5 = 3;
			break;
		default:
			pDiespr->Free5 = 1;
			break;
		}
		if (pDiespr->Free4 != 0) {
			pDiespr->Free4--;
		}
	}

	for (Row = 0; Row < DIE_SPR_GRID_QUADS; Row++) {
		for (Col = 0; Col < DIE_SPR_GRID_QUADS; Col++) {
			p = &pDiespr->Pgt4[G.Cbuf_id][Row * DIE_SPR_GRID_QUADS + Col];
			q = &pDiespr->Pgt4[G.Cbuf_id ^ 1][Row * DIE_SPR_GRID_QUADS + Col];

			/* Quad corners: V[0]=Pos[Col][Row], V[1]=Pos[Col+1][Row],
			 * V[2]=Pos[Col][Row+1], V[3]=Pos[Col+1][Row+1]. */
			V[0] = pDiespr->Pos[Col][Row];
			V[1] = pDiespr->Pos[Col + 1][Row];
			V[2] = pDiespr->Pos[Col][Row + 1];
			V[3] = pDiespr->Pos[Col + 1][Row + 1];

			gte_ldv3(&V[0], &V[1], &V[2]);
			gte_rtpt();
			gte_stsxy3((long *)&p->x0, (long *)&p->x1, (long *)&p->x2);
			gte_ldv0(&V[3]);
			gte_rtps();
			gte_stsxy((long *)&p->x3);

			/* Each vertex colour is written to the current and the other buffer */
			{
				int v = p->r0 + (255 - pDiespr->Dist[Col][Row]) / 28;
				if (v >= 251) {
					v = 250;
				}
				p->r0 = q->r0 = (UCHAR)v;
			}
			{
				int v = p->r1 + (255 - pDiespr->Dist[Col + 1][Row]) / 28;
				if (v >= 251) {
					v = 250;
				}
				p->r1 = q->r1 = (UCHAR)v;
			}
			{
				int v = p->r2 + (255 - pDiespr->Dist[Col][Row + 1]) / 28;
				if (v >= 251) {
					v = 250;
				}
				p->r2 = q->r2 = (UCHAR)v;
			}
			{
				int v = p->r3 + (255 - pDiespr->Dist[Col + 1][Row + 1]) / 28;
				if (v >= 251) {
					v = 250;
				}
				p->r3 = q->r3 = (UCHAR)v;
			}

			AddPrim(G.pOt0 + 5, p);
		}
	}

	if ((SHORT)pDiespr->Free2 > (SHORT)pDiespr->Move_start + DIE_SPR_MAIN_END_FRAMES) {
		pDiespr->Free2 = 0;
		pDiespr->Rno++;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x801921d8 (Die_spr_move_hold)                                    */
/* Hold state (225 instructions, DS 0x801921d8-0x8019255b): wobble the  */
/* grid vz values, project the 16x16 GT4 quads of buffer Cbuf_id into  */
/* the OT, then count Free2 frames and advance Rno after 61 (0x3d).    */
/* ------------------------------------------------------------------ */
void Die_spr_move_hold(void)
{
	short     Row;
	short     Col;
	short     Row2;
	POLY_GT4 *p;
	SVECTOR  *pv;
	/* Retail stages the four Pos vertices in its stack copy at sp+0x10..0x2f;
	 * this port reads Pos directly, so V is never written or read. The
	 * volatile keeps the 32 bytes reserved so the frame stays 80 as in retail. */
	volatile SVECTOR V[4];

	/* Phase 1: Row 15 (odd Col) gets vz += Rnd()&1 at Pos[Col][15]. Row 12
	 * (even Col) gets vz += Rnd()&1 at Pos[Col][12..16] when Free2 is odd. */
	for (Row = 0; Row < 17; Row++) {
		for (Col = 0; Col < 17; Col++) {
			if ((Row == 15) && ((Col & 1) != 0)) {
				pDiespr->Pos[Col][15].vz += (Rnd() & 1);
			}
			if ((Row == 12) && ((Col & 1) == 0) && ((pDiespr->Free2 & 1) != 0)) {
				for (Row2 = 12; Row2 < 17; Row2++) {
					pDiespr->Pos[Col][Row2].vz += (Rnd() & 1);
				}
			}
		}
	}

	/* Phase 2: project each quad of Pgt4[Cbuf_id] (p advances one POLY_GT4 per
	 * quad, 256 quads), then AddPrim into G.pOt0+5. */
	p = &pDiespr->Pgt4[G.Cbuf_id][0];
	for (Row = 0; Row < 16; Row++) {
		for (Col = 0; Col < 16; Col++) {
			pv = &pDiespr->Pos[Col][Row];

			gte_ldv3(&pv[0], &pv[17], &pv[1]);
			gte_rtpt();
			gte_stsxy3((long *)&p->x0, (long *)&p->x1, (long *)&p->x2);
			gte_ldv0(&pv[18]);
			gte_rtps();
			gte_stsxy((long *)&p->x3);
			AddPrim(G.pOt0 + 5, p);
			p++;
		}
	}

	/* Phase 3: Free2 counts frames; at 61 it resets and Rno advances to the
	 * next state. */
	pDiespr->Free2++;
	if ((SHORT)pDiespr->Free2 >= 61) {
		pDiespr->Free2 = 0;
		pDiespr->Rno++;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x8019255c (Die_spr_move_erase)                                   */
/* Erase state (281 instructions, DS 0x8019255c-0x801929bc): rebuild    */
/* the 17x17 grid heights with rsin(), then darken and project the      */
/* 16x16 POLY_GT4 quads of group G.Cbuf_id; Free2 > 200 moves Rno on. */
/* ------------------------------------------------------------------ */
#define DIE_ERASE_WAVE_BIAS	0x29f	/* added to the wave quotient (DS 0x8019255c) */
#define DIE_ERASE_LAST_FRAME	200	/* Free2 above this advances Rno (DS 0x8019255c) */
void Die_spr_move_erase(void)
{
	short     Row;
	short     Col;
	int       Frame;
	int       Dist;
	int       Rot;
	int       Vy;
	int       Rgb;
	SVECTOR  *pv;
	/* Retail stages the four Pos vertices in its stack copy at sp+0x10..0x2f;
	 * this port reads Pos directly, so V is never written or read. The
	 * volatile keeps the 32 bytes reserved so the frame stays 80 as in retail. */
	volatile SVECTOR V[4];
	POLY_GT4 *p;

	Frame = (SHORT)pDiespr->Free2;

	/* Wave height: vy = (short)((rsin(Dist*0x40 + Frame*-0x80) >> 6) * Frame / (Dist*0x14/0xff + 0x50)) + 0x29f.
	 * The divisor is variable and signed, so DIV_S keeps the DS zero and overflow traps. */
	for (Row = 0; Row < 17; Row++) {
		for (Col = 0; Col < 17; Col++) {
			Dist = pDiespr->Dist[Col][Row];
			Rot  = rsin((Dist * 0x40) + (Frame * -0x80));
			DIV_S(Vy, (Rot >> 6) * Frame, (Dist * 20 / 255) + 80);
			pDiespr->Pos[Col][Row].vy = (SHORT)((SHORT)Vy + DIE_ERASE_WAVE_BIAS);
		}
	}

	p = &pDiespr->Pgt4[G.Cbuf_id][0];
	for (Row = 0; Row < 16; Row++) {
		for (Col = 0; Col < 16; Col++) {
			pv = &pDiespr->Pos[Col][Row];

			gte_ldv3(&pv[0], &pv[17], &pv[1]);
			gte_rtpt();
			gte_stsxy3((long *)&p->x0, (long *)&p->x1, (long *)&p->x2);
			gte_ldv0(&pv[18]);
			gte_rtps();
			gte_stsxy((long *)&p->x3);

			/* darken each vertex colour by its grid distance, clamped at 0 */
			Rgb = p->r0 - ((255 - pDiespr->Dist[Col][Row]) >> 5);
			p->r0 = (UCHAR)(Rgb < 0 ? 0 : Rgb);
			Rgb = p->r1 - ((255 - pDiespr->Dist[Col + 1][Row]) >> 5);
			p->r1 = (UCHAR)(Rgb < 0 ? 0 : Rgb);
			Rgb = p->r2 - ((255 - pDiespr->Dist[Col][Row + 1]) >> 5);
			p->r2 = (UCHAR)(Rgb < 0 ? 0 : Rgb);
			Rgb = p->r3 - ((255 - pDiespr->Dist[Col + 1][Row + 1]) >> 5);
			p->r3 = (UCHAR)(Rgb < 0 ? 0 : Rgb);

			AddPrim(G.pOt0 + 5, p);
			p++;
		}
	}

	pDiespr->Free2++;
	if ((SHORT)pDiespr->Free2 > DIE_ERASE_LAST_FRAME) {
		pDiespr->Free2 = 0;
		pDiespr->Rno++;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x801929c0 (Die_spr_move_end)                                     */
/* Per-frame end state (16 instructions, DS 0x801929c0-0x801929fc):     */
/* Free2 counts frames; after 181 (0xb5) it resets Free2 and Rno to 0.  */
/* ------------------------------------------------------------------ */
void Die_spr_move_end(void)
{
	pDiespr->Free2++;
	if ((SHORT)pDiespr->Free2 >= 181) {
		pDiespr->Free2 = 0;
		pDiespr->Rno   = 0;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80192a00 (Die_blood_move_main)                                  */
/* Per-frame: project blood quads via GTE, fade by Ctr, add to OT      */
/* Progress percent Pct = 100 - Ctr (the retail "local_50" value).     */
/* ------------------------------------------------------------------ */
void Die_blood_move_main(void)
{
	SVECTOR   V[4];
	short     Pct;
	short     Grp;
	short     Row;
	int       Col;
	int       Size;
	int       Amp;
	int       Sel;
	int       t;
	int       a;
	int       b;
	int       k;
	POLY_FT4 *p;
	UCHAR   *pRgb;

	Size = 0;   /* retail keeps this in a register across primitives */
	Pct  = (short)(100 - pDiespr->Ctr);
	if ((SsVabTransCompleted(0) != 0) && (Pct == 100)) {
		Snd_se_stad(0x4000000, (VECTOR *)0x0);
	}

	if ((G.System_flg & 0x800) == 0) {
		if (Pct < 0) {
			return;
		}
		for (Grp = 0; Grp < 3; Grp++) {
			p = &pDiespr->Blood[Grp][G.Cbuf_id][0][0];
			for (Row = 0; Row < 16; Row++) {
				for (Col = 0; Col < 16; Col += 2) {
					/* quad: V0=Pos[Col][Row], V1=Pos[Col+2][Row], V2=Pos[Col][Row+1], V3=Pos[Col+2][Row+1] */
					if ((SHORT)pDiespr->Ctr > 0x14) {
						Amp = rcos(Grp * 0x10000 + Col * 0x40)
							+ rcos((Grp * 7 + Col) * 0x100) + 0x1000;
						Amp = ((int)(((long long)Amp * 0x66666667) >> 0x27) - (Amp >> 0x1f))
							* ((SHORT)pDiespr->Ctr - 0x14);
						Sel = (Col >> 1) & 3;
						a   = Amp * (Row + 1);
						b   = Amp * (Row + 2);
					}

					V[0] = pDiespr->Pos[Col][Row];
					V[1] = pDiespr->Pos[Col + 2][Row];
					V[2] = pDiespr->Pos[Col][Row + 1];
					V[3] = pDiespr->Pos[Col + 2][Row + 1];
					pRgb = &p->r0;                     /* rgb0 UCHAR of the FT4 */

					/* The Ctr tests below are signed halfword compares in retail (lh + slti),
					 * and Ctr - 4 / Ctr - 9 are truncated to short (sll/sra 16) before the
					 * sign test, so Ctr is read through (SHORT) casts. */
					if (Grp == 0) {
						if ((SHORT)pDiespr->Ctr < 5) {
							*pRgb = (UCHAR)(pDiespr->Ctr * 40);
							Size  = (SHORT)pDiespr->Ctr * (SHORT)pDiespr->Ctr * 14 + 0x168;
						} else {
							*pRgb = 200;
							Size  = 600;
						}
						for (k = 0; k < 4; k++) {
							V[k].vx += 0x5a;
						}
					} else if (Grp == 1) {
						t = (SHORT)(pDiespr->Ctr - 4);
						if (t >= 0) {
							if (t < 5) {
								*pRgb = (UCHAR)(t * 40);
								Size  = t * t * 14 + 200;
							} else {
								*pRgb = 200;
								Size  = 0x1b8;
							}
							for (k = 0; k < 4; k++) {
								V[k].vx += 0x104;
								V[k].vz += -0x70;
							}
						}
					} else if (Grp == 2) {
						t = (SHORT)(pDiespr->Ctr - 9);
						if (t >= 0) {
							if (t < 3) {
								*pRgb = (UCHAR)(t * 40);
								Size  = t * t * 10 + 0xa0;
							} else {
								*pRgb = 200;
								Size  = 0x140;
							}
							for (k = 0; k < 4; k++) {
								V[k].vx += -0x10e;
								V[k].vz += -0x54;
							}
						}
					}

					if (*pRgb != 0) {
						for (k = 0; k < 4; k++) {
							V[k].vy = (short)Size;
						}
						for (k = 0; k < 4; k++) {
							V[k].vz <<= 1;
						}

						if ((SHORT)pDiespr->Ctr > 0x14) {
							switch (Sel) {
							case 0:
								t = (short)(((int)(((long long)a * 0x66666667) >> 0x24) - (a >> 0x1f)) >> 4);
								V[0].vz += (short)t;
								V[1].vz += (short)t;
								t = (short)(((int)(((long long)b * 0x66666667) >> 0x24) - (b >> 0x1f)) >> 4);
								V[2].vz += (short)t;
								V[3].vz += (short)t;
								break;
							case 1:
								t = ((int)(((long long)a * 0x66666667) >> 0x24) - (a >> 0x1f));
								V[0].vz += (short)(t >> 4);
								V[1].vz += (short)(t >> 3);
								t = ((int)(((long long)b * 0x66666667) >> 0x24) - (b >> 0x1f));
								V[2].vz += (short)(t >> 4);
								V[3].vz += (short)(t >> 3);
								break;
							case 2:
								t = ((int)(((long long)a * 0x66666667) >> 0x24) - (a >> 0x1f));
								V[0].vz += (short)(t >> 3);
								V[1].vz += (short)(t >> 4);
								t = ((int)(((long long)b * 0x66666667) >> 0x24) - (b >> 0x1f));
								V[2].vz += (short)(t >> 3);
								V[3].vz += (short)(t >> 4);
								break;
							case 3:
								t = (short)(((int)(((long long)a * 0x66666667) >> 0x24) - (a >> 0x1f)) >> 4);
								V[0].vz += (short)t;
								V[1].vz += (short)t;
								t = (short)(((int)(((long long)b * 0x66666667) >> 0x24) - (b >> 0x1f)) >> 4);
								V[2].vz += (short)t;
								V[3].vz += (short)t;
								break;
							}
							if (Row == 0xf) {
								t = Amp >> 4;
								V[2].vz += (short)t;
								V[3].vz += (short)t;
							}
						}

						gte_ldv3(&V[0], &V[1], &V[2]);
						gte_rtpt();
						gte_stsxy3((long *)&p->x0, (long *)&p->x1, (long *)&p->x2);
						gte_ldv0(&V[3]);
						gte_rtps();

						if (Pct < 0x29) {
							SetSemiTrans(p, 1);
							*pRgb = (UCHAR)(Pct * 6);
							if (Pct < 0x14) {
								pDiespr->Blood_flg = 1;
								p->tpage = 0xb5;
								*pRgb = (UCHAR)(Pct * 3);
							}
						}
						gte_stsxy((long *)&p->x3);
						AddPrim(G.pOt0 + 6, p);
					}
					p++;
				}
			}
		}
	}
	pDiespr->Ctr++;
}

/* ------------------------------------------------------------------ */
/* DS 0x801932b0 (Die_spr_load)                                         */
/* Load death sprite file from CD by player/enemy type                 */
/* ------------------------------------------------------------------ */
void Die_spr_load(void)
{
	UCHAR die_type;
	ULONG File_no;
	char  *Mess;

	die_type = G.Player.Id;
	if (((G.System_flg & SYS_DIE_BLOOD) == 0) && ((G.Status_flg & STAT_SUB_PLAYER) != 0) && (G.pEnemy[1]->Routine_0 == 3)) {
		Diedemo.Phase = 1;
		die_type = 0x0e;
		if ((G.pEnemy[1]->Id & 4) != 0) {
			die_type = 0x0f;
		}
	}

	switch (die_type) {
	default:
		File_no = DIED00_DIE;
		Mess    = (char *)s_DIED00_DIE;
		break;
	case 0x01:
	case 0x03:
	case 0x05:
	case 0x07:
	case 0x09:
		File_no = DIED01_DIE;
		Mess    = (char *)s_DIED01_DIE;
		break;
	case 0x0b:
		File_no = DIED0B_DIE;
		Mess    = (char *)s_DIED0B_DIE;
		break;
	case 0x0c:
		File_no = DIED0C_DIE;
		Mess    = (char *)s_DIED0C_DIE;
		break;
	case 0x0d:
		File_no = DIED0D_DIE;
		Mess    = (char *)s_DIED0D_DIE;
		break;
	case 0x0e:
		File_no = DIED0E_DIE;
		Mess    = (char *)s_DIED0E_DIE;
		break;
	case 0x0f:
		File_no = DIED0F_DIE;
		Mess    = (char *)s_DIED0F_DIE;
		break;
	}
	Cd_read(File_no, Bg_img_buf, 1, (UCHAR *)Mess);
}

/* ------------------------------------------------------------------ */
/* DS 0x80193424 (Die_snd_trans)                                        */
/* Copy VAB header and open sound bank for death sounds                */
/* ------------------------------------------------------------------ */
void Die_snd_trans(ULONG Size)
{
	short vab_handle;
	CHAR *pVab_id4;

	pVab_id4 = &Vab_id[4];
	memcpy(Arms_edt_buf, Die_vh_src, Size);
	pVh_tbl[4] = (ULONG *)(Arms_edt_buf + EDT_VAB_OFS(Arms_edt_buf, Size));

	if (-1 < (signed char)*pVab_id4) {
		SsVabClose((short)(signed char)*pVab_id4);
		*pVab_id4 = -1;
	}

	UnkSndFlg0 = (ULONG)Arms_edt_buf;
	do {
		vab_handle = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[4], 4, 0x1020);
		*pVab_id4  = (char)vab_handle;
	} while ((signed char)*pVab_id4 == -1);

	SsVabTransBody(Die_vb_buf, (short)(signed char)*pVab_id4);
}
