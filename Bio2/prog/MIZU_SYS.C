/*
 * MIZU_SYS.C - Water surface / ripple system module
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */

#include "TYPES.H"
#include <INLINE_C.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <STRINGS.H>
#include "ROOM.H"
#include "MIZU_SYS.H"
#include "EM_SUB.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "OMA.H"
#include "SCE0.H"
#include "SCE_AT.H"

/* ------------------------------------------------------------------ */
/* Data                                                                 */
/* ------------------------------------------------------------------ */

/* DS 0x8009dca0 (Mizu_size_tbl) */
UCHAR Mizu_size_tbl[][2] =
{
	{ 0x08, 0x07 }, { 0x02, 0x04 }, { 0x02, 0x0b }, { 0x02, 0x05 },
	{ 0x0b, 0x0a }, { 0x02, 0x0c }, { 0x03, 0x0b }, { 0x02, 0x0c },
	{ 0x03, 0x0d }, { 0x03, 0x05 }, { 0x01, 0x02 }, { 0x01, 0x01 },
	{ 0x03, 0x0e }, { 0x04, 0x0a }, { 0x04, 0x08 }
};

/* DS 0x8009dcc0 (Mizu_tbl) */
UCHAR Mizu_tbl[15] =
{
	0x00, 0x07, 0x0b, 0x16, 0x1b, 0x25, 0x31, 0x3c,
	0x48, 0x55, 0x5a, 0x5c, 0x5d, 0x6b, 0x75
};

/* DS 0x8009dcd0 (Mizu_struct_tbl) */
USHORT Mizu_struct_tbl[125] =
{
	0x3f00, 0x3e00, 0x3c00, 0x7c00, 0x7c00, 0x7c00, 0xfc00, 0xc000,
	0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000,
	0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000,
	0xc000, 0xc000, 0xc000, 0x0600, 0x3fe0, 0x3fe0, 0x1f80, 0x1f80,
	0x1f80, 0xff80, 0xff80, 0xf000, 0xf000, 0xc000, 0xc000, 0xc000,
	0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000,
	0xc000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000,
	0xe000, 0xe000, 0xe000, 0xe000, 0xc000, 0xc000, 0xc000, 0xc000,
	0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000, 0xc000,
	0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000,
	0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000,
	0xe000, 0xe000, 0x8000, 0x8000, 0x8000, 0xe000, 0xe000, 0xe000,
	0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000, 0xe000,
	0xe000, 0xe000, 0xe000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
	0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
	0xf000, 0xf000, 0xf000, 0xf000, 0xf000
};

/* DS 0x8009ddcc (Div_prim_coord) */
UCHAR Div_prim_coord[13][8] =
{
	{ 0x12, 0x12, 0x0c, 0x0f, 0x00, 0x12, 0x06, 0x0f },
	{ 0x03, 0x0c, 0x03, 0x06, 0x00, 0x12, 0x00, 0x00 },
	{ 0x0c, 0x03, 0x12, 0x00, 0x06, 0x03, 0x00, 0x00 },
	{ 0x12, 0x12, 0x12, 0x00, 0x0f, 0x0c, 0x0f, 0x06 },
	{ 0x06, 0x0f, 0x06, 0x0c, 0x00, 0x12, 0x03, 0x0c },
	{ 0x06, 0x06, 0x06, 0x03, 0x03, 0x06, 0x00, 0x00 },
	{ 0x0f, 0x06, 0x12, 0x00, 0x0c, 0x06, 0x0c, 0x03 },
	{ 0x12, 0x12, 0x0f, 0x0c, 0x0c, 0x0f, 0x0c, 0x0c },
	{ 0x0c, 0x0f, 0x0c, 0x0c, 0x06, 0x0f, 0x06, 0x0c },
	{ 0x06, 0x0c, 0x06, 0x06, 0x03, 0x0c, 0x03, 0x06 },
	{ 0x0c, 0x06, 0x0c, 0x03, 0x06, 0x06, 0x06, 0x03 },
	{ 0x0f, 0x0c, 0x0f, 0x06, 0x0c, 0x0c, 0x0c, 0x06 },
	{ 0x0c, 0x0c, 0x0c, 0x06, 0x06, 0x0c, 0x06, 0x06 }
};

/* ------------------------------------------------------------------ */
/* Mizu_trans  (DS 0x8002ef50)                                          */
/* Sort the water-surface GT4 packets, then enable (len 12) or hide    */
/* (len 0) each cell depending on whether it lies in the current cut.  */
/* ------------------------------------------------------------------ */
/* DS 0x8002ef50 (Mizu_trans) */
void Mizu_trans(OBJ_MODEL_WORK *pOwork, ULONG toumei)
{
	ULONG   i, j, D0;
	ULONG   Dx, Dz;
	LONG    XX, ZZ;
	POLY_GT4 *pP;
	VECTOR  Vec;
	USHORT  *A0;
	UCHAR    frame_pad[24];	/* retail reserves the 24 bytes after Vec and never reads them */

	D0 = pOwork->Cdir_y;
	SortGT4_lno((TMD_MEM *)pOwork->pTmd2,
				(POLY_GT4 *)pOwork->pPacket2 + G.Cbuf_id,
				pOwork->Poly_rgb, toumei);

	/* Quadrant of the Y rotation selects the walk direction of the cell grid.
	   DS tests 1, 0, 2, 3 in that order (an if chain; 0 and 3 share the +900 Z setup) */
	D0 = (D0 & 0xfff) >> 10;
	if (D0 == 1)
	{
		XX     = 0x708;
		Vec.vx = pOwork->Pos_x + 900;
		ZZ     = -0x708;
		Vec.vz = pOwork->Pos_z - 900;
	}
	else if (D0 == 0)
	{
		XX     = 0x708;
		Vec.vx = pOwork->Pos_x + 900;
		goto Z_plus;
	}
	else if (D0 == 2)
	{
		XX     = -0x708;
		Vec.vx = pOwork->Pos_x - 900;
		ZZ     = -0x708;
		Vec.vz = pOwork->Pos_z - 900;
	}
	else if (D0 == 3)
	{
		XX     = -0x708;
		Vec.vx = pOwork->Pos_x - 900;
	Z_plus:
		ZZ     = 0x708;
		Vec.vz = pOwork->Pos_z + 900;
	}

	pP = (POLY_GT4 *)pOwork->pPacket2 + G.Cbuf_id;
	Dx = Mizu_size_tbl[pOwork->Type][0];
	Dz = Mizu_size_tbl[pOwork->Type][1];
	A0 = &Mizu_struct_tbl[Mizu_tbl[pOwork->Type]];

	/* DS: 1 jumps straight to the X-row walk; 0 and 2 take the Z-row walk; 3 falls into the X-row walk */
	if (D0 != 1)
	{
		if (D0 == 0 || D0 == 2)
		{
			/* rows run along Z, cells along X */
			for (i = 0; i < Dz; i++)
			{
				j = 0;
				if (j < Dx)
				{
Inner1:
					if (*A0 & (0x8000 >> j))
					{
						if (Hit_ck_point4(&Vec, G.Cccut))
							pP->tag |= 0x0c000000;
						else
							pP->tag &= 0x00ffffff;
						pP += 2;
					}
					Vec.vx += XX;
					j++;
					if (j < Dx)
						goto Inner1;
				}
				A0++;
				Vec.vz += ZZ;
				Vec.vx -= Dx * XX;
			}
			goto Done;
		}
		if (D0 != 3)
			goto Done;
	}
	/* rows run along X, cells along Z */
	for (i = 0; i < Dz; i++)
	{
		j = 0;
		if (j < Dx)
		{
Inner2:
			if (*A0 & (0x8000 >> j))
			{
				if (Hit_ck_point4(&Vec, G.Cccut))
					pP->tag |= 0x0c000000;
				else
					pP->tag &= 0x00ffffff;
				pP += 2;
			}
			Vec.vz += ZZ;
			j++;
			if (j < Dx)
				goto Inner2;
		}
		A0++;
		Vec.vx += XX;
		Vec.vz -= Dx * ZZ;
	}
Done:
	Mizu_div_ck(pOwork, Dx, Dz);
}

/* ------------------------------------------------------------------ */
/* Mizu_div_ck  (DS 0x8002f29c)                                         */
/* Run the ripple subdivision for the player, the partner (if active), */
/* every active Id 2 object model and every active enemy.              */
/* ------------------------------------------------------------------ */
/* DS 0x8002f29c (Mizu_div_ck) */
void Mizu_div_ck(OBJ_MODEL_WORK *pOwork, ULONG Size_x, ULONG Size_z)
{
	ENEMY_WORK  *pEw;
	ENEMY_WORK  *pEw2;
	ENEMY_WORK  **ppEw;

	Mizu_div_main(pOwork, Size_x, Size_z, G.pEnemy[0]);

	if (G.pEnemy[1]->Be_flg & 1)
		Mizu_div_main(pOwork, Size_x, Size_z, G.pEnemy[1]);

	/* object models (stride sizeof(OBJ_MODEL_WORK)) */
	pEw  = (ENEMY_WORK *)G.Ob_model;
	pEw2 = (ENEMY_WORK *)(G.Ob_model + G.pRoom->nOmodel);
	for ( ; pEw < pEw2; pEw = (ENEMY_WORK *)((OBJ_MODEL_WORK *)pEw + 1))
	{
		if ((pEw->Be_flg & 1) && pEw->Id == 2)
			Mizu_div_main(pOwork, Size_x, Size_z, pEw);
	}

	for (ppEw = &G.pEnemy[2]; ppEw != G.ppEnemy_end; ppEw++)
	{
		if ((*ppEw)->Be_flg & 1)
			Mizu_div_main(pOwork, Size_x, Size_z, *ppEw);
	}
}

/* ------------------------------------------------------------------ */
/* Mizu_div_main  (DS 0x8002f3fc)                                       */
/* If pEwork stands on a visible water cell, hide that cell and emit a */
/* 13-prim subdivided replacement into Sce.pMizu_div.                  */
/* ------------------------------------------------------------------ */
/* DS 0x8002f3fc (Mizu_div_main) */
void Mizu_div_main(OBJ_MODEL_WORK *pOwork, ULONG Size_x, ULONG Size_z,
				   ENEMY_WORK *pEwork)
{
	LONG     D0, D1, D2;
	LONG     Dx, Dz;
	ULONG    i, j;
	BOX      Box;
	VECTOR   Vec1;
	SVECTOR  Svec[4];
	MATRIX   m;
	MATRIX   workm;
	LONG     otz[2];	/* OTZ words of the two stszotz calls: retail keeps them at sp+0x88 and sp+0x8c */
	POLY_GT4 *pP0;
	USHORT   *A0;
	UCHAR    *A1;
	ULONG    *A2;
	ULONG    Frame_pad[4];	/* unused; retail Mizu_div_main keeps sp+0x90..0x9f free (frame 0xc8) */

	(void)Frame_pad;
	Box.W = (SHORT)(Size_x * 0x708 - 1);
	Box.D = (SHORT)(Size_z * 0x708 - 1);
	Box.X = 0;
	Box.Z = 0;

	/* position of pEwork in the water model's local space */
	Svec[0].vy = 0;
	Svec[0].vx = (SHORT)(pEwork->Pos_x - pOwork->Workm.t[0]);
	Svec[0].vz = (SHORT)(pEwork->Pos_z - pOwork->Workm.t[2]);
	m = GsIDMATRIX;
	RotMatrixY(-pOwork->Cdir_y, &m);
	ApplyMatrix(&m, &Svec[0], &Vec1);

	if (Hit_ck_box(&Vec1, &Box) == 0)
		return;

	/* find the packet index of the cell under the target */
	Dz = Vec1.vz / 0x708;
	Dx = Vec1.vx / 0x708;
	D0 = Dx + Dz * Size_x;
	D1 = 0;
	D2 = 0;
	A0 = &Mizu_struct_tbl[Mizu_tbl[pOwork->Type]];
	for (i = 0; i < Size_z; i++, A0++)
	{
		for (j = 0; j < Size_x; j++, D1++)
		{
			if (*A0 & (0x8000 >> j))
				D2++;
			if (D1 == D0)
				goto loop_out;
		}
	}
loop_out:
	D2--;
	if ((*A0 & (0x8000 >> j)) == 0)
		return;

	pP0 = (POLY_GT4 *)pOwork->pPacket2 + G.Cbuf_id + D2 * 2;
	if ((pP0->tag & 0x0c000000) == 0)
		return;
	if (Sce.Mizu_div_ctr >= Sce.Mizu_div_max)
		return;

	Dx *= 0x708;
	Dz *= 0x708;
	D0 = pP0->clut;
	D1 = pP0->tpage;
	pP0->tag &= 0x00ffffff;                 /* hide the original cell */

	m   = GsIDMATRIX;
	pP0 = Sce.pMizu_div + G.Cbuf_id + Sce.Mizu_div_ctr * 26;
	Sce.Mizu_div_ctr++;

	RotMatrix((SVECTOR *)&pOwork->Cdir_x, &workm);
	workm.t[0] = pOwork->Workm.t[0];
	workm.t[1] = pOwork->Workm.t[1];
	workm.t[2] = pOwork->Workm.t[2];
	CompM(&GsWSMATRIX, &workm, &m);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	A1 = Div_prim_coord[0];
	A2 = G.Ot1[G.Cbuf_id];
	for (i = 0; i < 13; i++, A1 += 8)
	{
		/* 32-bit stores: low half is vx/vz, high half lands in vy/pad */
		*(LONG *)&Svec[0].vx = Dx + A1[0] * 100;
		*(LONG *)&Svec[0].vz = Dz + A1[1] * 100;
		*(LONG *)&Svec[1].vx = Dx + A1[2] * 100;
		gte_ldv0(&Svec[0]);
		*(LONG *)&Svec[1].vz = Dz + A1[3] * 100;
		gte_rtps();
		*(LONG *)&Svec[2].vx = Dx + A1[4] * 100;
		*(LONG *)&Svec[2].vz = Dz + A1[5] * 100;
		*(LONG *)&Svec[3].vx = Dx + A1[6] * 100;
		*(LONG *)&Svec[3].vz = Dz + A1[7] * 100;
		gte_stsxy((LONG *)&pP0->x0);
		gte_stszotz(&otz[0]);
		gte_ldv3(&Svec[1], &Svec[2], &Svec[3]);
		pP0->clut  = (USHORT)D0;
		pP0->tpage = (USHORT)D1;
		gte_rtpt();
		gte_stsxy3((LONG *)&pP0->x1, (LONG *)&pP0->x2, (LONG *)&pP0->x3);
		gte_stszotz(&otz[1]);
		addPrim(&A2[otz[0] >> 3], pP0);
		pP0 += 2;
	}
}
