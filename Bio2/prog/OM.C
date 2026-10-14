/*
 * OM.C - Object model (OM) init, movement and texture change
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OM.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include "ROOM.H"
#include "OM.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MIZU_SYS.H"
#include "OMA.H"
#include "SCA.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "flag.h"

/* DS 0x800d6c48 (Cc_work) */
CC_WORK Cc_work = { { { 0 } } };

/* DS 0x800d7638 (Om_fall_old_dir_x) */
SHORT Om_fall_old_dir_x = 0x0000;

/* DS 0x800d7642 (Om_fall_old_dir_z) */
SHORT Om_fall_old_dir_z = 0x0000;

/* DS 0x800dcb98 (Om_fall_dir_x) */
SHORT Om_fall_dir_x = 0x0000;

/* DS 0x800dcba2 (Om_fall_dir_z) */
SHORT Om_fall_dir_z = 0x0000;

/* Om_init - 0x80035710
   Map an object model's TMD and preset its GT3/GT4 packets.
*/
/* DS 0x80035710 (Om_init) */
void Om_init(ULONG Om_no, ULONG *pTmd_adr)
{
	MODEL_WORK      *pMw;

	if (G.pOld_tmd != pTmd_adr)
	{
		G.pOld_tmd = pTmd_adr;
		Mapping_tmd(1, pTmd_adr, (ULONG)G.Ob_model[Om_no].Tpage, (ULONG)G.Ob_model[Om_no].Clut);
	}
	pMw = (MODEL_WORK *)&G.Ob_model[Om_no].Attribute;
	G.Ob_model[Om_no].pTmd      = pTmd_adr + 3;
	G.Ob_model[Om_no].Routine_0 = 0;
	G.Ob_model[Om_no].Routine_1 = 0;
	G.Ob_model[Om_no].Routine_2 = 0;
	G.Ob_model[Om_no].Routine_3 = 0;
	G.Ob_model[Om_no].pTmd2     = pTmd_adr + 10;
	G.Ob_model[Om_no].Poly_rgb  = 0x808080;
	G.pFree_work = (UCHAR *)PresetObjectGT3_neo(pMw, (ULONG *)G.pFree_work, 0);
	G.pFree_work = (UCHAR *)PresetObjectGT4_neo(pMw, (ULONG *)G.pFree_work, 0);
}

/* Col_chg_init - 0x800357d8
   Redirect every polygon of the model to the colour-change CLUT row
   (0x130, 0x1e0 + Col_wk_no) and set up the two DR_MOVE source rects.
*/
/* DS 0x800357d8 (Col_chg_init) */
void Col_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
	LONG            Frame_pad[4];   /* DS 0x800357d8: retail frame 0x30 keeps 16 unread bytes at 0x10 */
	ULONG           Poly_num;
	ULONG           Clut;
	ULONG           i;
	ULONG           D0;     /* original CLUT y (left unset if the model has no polygons) */
	ULONG           *p;
	OBJ_MODEL_WORK  *pMw;

	(void)Frame_pad;
	pMw  = pCcp->pOm;
	Clut = GetClut(0x130, Col_wk_no + 0x1e0);

	/* GT3 packets (double buffered: n_poly * 2), clut is the upper half of word 3 */
	if (((TMD_MEM *)pMw->pTmd)->n_poly != 0)
		D0 = pMw->pPacket[3] >> 22;
	Poly_num = ((TMD_MEM *)pMw->pTmd)->n_poly << 1;
	p = pMw->pPacket + 3;
	for (i = 0; i < Poly_num; i++)
	{
		*p = (*p & 0xffff) | (Clut << 16);
		p += sizeof(POLY_GT3) / sizeof(ULONG);
	}

	/* GT4 packets */
	Poly_num = ((TMD_MEM *)pMw->pTmd2)->n_poly << 1;
	if (((TMD_MEM *)pMw->pTmd2)->n_poly != 0)
		D0 = pMw->pPacket2[3] >> 22;
	p = pMw->pPacket2 + 3;
	for (i = 0; i < Poly_num; i++)
	{
		*p = (*p & 0xffff) | (Clut << 16);
		p += sizeof(POLY_GT4) / sizeof(ULONG);
	}

	for (i = 0; i < 2; i++)
	{
		pCcp->Cc_dr_rect[i].y = (SHORT)D0;
		pCcp->Cc_dr_rect[i].w = 0x10;
		pCcp->Cc_dr_rect[i].h = 1;
	}
}

/* Tex_chg_init - 0x800358f0
   Register the model's texture page for texture animation.
*/
/* DS 0x800358f0 (Tex_chg_init) */
void Tex_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
	ULONG           i;
	OBJ_MODEL_WORK  *pMw;
	CC_PARTS_WORK   *pPart;

	pMw = pCcp->pOm;
	if (Cc_work.Ctex_old == pMw->Tpage)
		return;

	pPart = &Cc_work.Cc_parts[Col_wk_no];
	Cc_work.Ctex_old = pMw->Tpage;
	pPart->Cc_num++;
	pPart->Cc_cnt   = pPart->Cc_wait;
	pPart->Cc_pos_x = (USHORT)((pMw->Tpage & 0xf) << 6);
	pPart->Cc_pos_y = (USHORT)((pMw->Tpage >> 4) << 8);
	for (i = 0; i < 2; i++)
	{
		pPart->Cc_dr_rect[i].w = 0x20;
		pPart->Cc_dr_rect[i].h = 0x40;
	}
}

/* Tex_ctr - 0x80035994
   Per-frame colour/texture animation: update each part's source rect and
   queue a DR_MOVE into Ot2.
*/
/* DS 0x80035994 (Tex_ctr) */
void Tex_ctr(void)
{
	CC_PARTS_WORK   *pCcp;
	DR_MOVE         *pMove;
	RECT            *pRect;
	ULONG           i;
	ULONG           D0;
	ULONG           D1;
	ULONG           x, y;
	UCHAR           Ctr;

	D1 = Cc_work.Ccol_no;
	if (D1 == 0)
		return;

	D0   = G.Cbuf_id;
	pCcp = Cc_work.Cc_parts;
	for (i = 0; i < D1; i++, pCcp++)
	{
		pMove = &pCcp->Cc_dr_move[D0];
		pRect = &pCcp->Cc_dr_rect[D0];
		Ctr   = pCcp->Cc_ctr;
		if ((Ctr & 0x80) != 0)
		{
			Col_chg(pCcp, pRect, Ctr & 0x7f);
			x = 0x130;
			y = i + 0x1e0;
			SetDrawMove(pMove, pRect, x, y);
		}
		else if ((Ctr & 0x40) != 0)
		{
			Tex_chg(pCcp, pRect, Ctr & 0xbf);
			x = pCcp->Cc_pos_x;
			y = pCcp->Cc_pos_y;
			SetDrawMove(pMove, pRect, x, y);
		}
		AddPrim(G.Ot2 + D0, pMove);
	}
}

/* Col_chg - 0x80035ad0
   CLUT animation step. Ctr: 0 fixed, 1 one-shot, 2 loop, 3/4 ping-pong.
*/
/* DS 0x80035ad0 (Col_chg) */
void Col_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr)
{
	switch (Ctr)
	{
	case 0:
		pRect->x = (SHORT)(pCcp->Cc_num << 4);
		break;
	case 1:
		pRect->x = (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) << 4);
		if ((ULONG)pCcp->Cc_cnt != (ULONG)pCcp->Cc_num * pCcp->Cc_wait - 1)
			pCcp->Cc_cnt++;
		break;
	case 2:
		pRect->x = (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) << 4);
		pCcp->Cc_cnt++;
		if ((ULONG)pCcp->Cc_cnt == (ULONG)pCcp->Cc_num * pCcp->Cc_wait)
			pCcp->Cc_cnt = 0;
		break;
	case 3:
		pRect->x = (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) << 4);
		pCcp->Cc_cnt++;
		if ((ULONG)pCcp->Cc_cnt == (ULONG)pCcp->Cc_wait * pCcp->Cc_num)
		{
			pCcp->Cc_cnt -= pCcp->Cc_wait;
			pCcp->Cc_ctr  = 0x84;
		}
		break;
	case 4:
		pCcp->Cc_cnt--;
		pRect->x = (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) << 4);
		if (pCcp->Cc_cnt == 0)
		{
			pCcp->Cc_ctr = 0x83;
			pCcp->Cc_cnt = pCcp->Cc_wait;
		}
		break;
	}
}

/* Tex_chg - 0x80035c98
   Texture animation step (frames laid out 4 per column, 0x20 x 0x40 each).
*/
/* DS 0x80035c98 (Tex_chg) */
void Tex_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr)
{
	switch (Ctr)
	{
	case 0:
		pRect->x = (SHORT)((pCcp->Cc_num >> 2) << 5);
		pRect->y = (SHORT)((pCcp->Cc_num & 3) << 6);
		break;
	case 1:
		pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
		pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
		if ((ULONG)pCcp->Cc_cnt != (ULONG)pCcp->Cc_num * pCcp->Cc_wait - 1)
			pCcp->Cc_cnt++;
		break;
	case 2:
		pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
		pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
		pCcp->Cc_cnt++;
		if ((ULONG)pCcp->Cc_cnt == (ULONG)pCcp->Cc_wait * pCcp->Cc_num)
			pCcp->Cc_cnt = pCcp->Cc_wait;
		break;
	case 3:
		pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
		pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
		pCcp->Cc_cnt++;
		if ((ULONG)pCcp->Cc_cnt == (ULONG)pCcp->Cc_wait * pCcp->Cc_num)
		{
			pCcp->Cc_cnt -= pCcp->Cc_wait;
			pCcp->Cc_ctr  = 0x44;
		}
		break;
	case 4:
		pCcp->Cc_cnt--;
		pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
		pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
		if (pCcp->Cc_cnt == pCcp->Cc_wait)
		{
			pCcp->Cc_cnt += pCcp->Cc_wait;
			pCcp->Cc_ctr  = 0x43;
		}
		break;
	}
}

/* Om_move - 0x80035f68
   Per-frame object model update: box falls, object<->enemy/player collision,
   and player pushing of pushable objects.
*/
/* DS 0x80035f68 (Om_move) */
void Om_move(void)
{
	OBJ_MODEL_WORK  *pOm;
	OBJ_MODEL_WORK  *pOm2;
	OBJ_MODEL_WORK  *pHit_om;
	ENEMY_WORK      **ppEm;
	SHORT           Bak_pos_x, Bak_pos_z;
	ULONG           Hit_cnt;

	if ((G.Stop_flg & 0x8000000) != 0)
		return;

	Hit_cnt = 0;
	G.Status_flg &= ~STAT_PUSH_OBJ;
	pHit_om = NULL;
	Tex_ctr();
	Om_Jump_ck((ENEMY_WORK *)&G.Player);

	for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++)
	{
		if ((pOm->Be_flg & 1) != 0)
		{
			if (pOm->Id == 2)
				Om_move_box(pOm);
			Oma_set_ofs(pOm);
			pOm->nFloor = -(CHAR)(pOm->Pos_y / 0x708);
			if ((G.Player.Routine_1 != 1) && (G.Player.Routine_1 != 10))
				pOm->Push_cnt = 0;

			for (ppEm = G.pEnemy + 1; ppEm != G.ppEnemy_end; ppEm++)
			{
				if (((*ppEm)->Be_flg & 1) != 0)
					Oma_ck_oba(*ppEm, pOm, 0);
			}

			if ((pOm->Be_flg & 0x200) != 0)
			{
				Oma_ck_oba((ENEMY_WORK *)&G.Player, pOm, 1);
			}
			else if (pOm == G.Player.pOn_om)
			{
				Oma_pull((ENEMY_WORK *)&G.Player, pOm);
			}
			else if (Oma_ck_oba((ENEMY_WORK *)&G.Player, pOm, 2) != 0)
			{
				Hit_cnt++;
				if ((G.Player.nFloor == pOm->nFloor) &&
					(Oma_ck_front((ENEMY_WORK *)&G.Player, pOm) != 0))
					pHit_om = pOm;
			}

			/* falling box: start the fall when pushed off a ledge */
			if ((pOm->Id == 2) && (pOm->Routine_0 == 0) &&
				((Sca_ck_hit_om(pOm, 0x4000, 0x100) & 4) != 0))
			{
				Om_set_fall_dir(pOm);
				pOm->Routine_0 = 1;
			}
		}
		pOm->Water = 0;
	}

	/* player pushes a single pushable object */
	if ((Hit_cnt == 1) && (pHit_om != NULL) &&
		((pHit_om->Be_flg & 2) == 0) && ((pHit_om->Be_flg & 0x80) != 0))
	{
		Bak_pos_x = (SHORT)pHit_om->Pos_x;
		Bak_pos_z = (SHORT)pHit_om->Pos_z;
		if (5 < ++pHit_om->Push_cnt)
		{
			Oma_ck_oba((ENEMY_WORK *)&G.Player, pHit_om, 1);
			if ((Sca_ck_hit_om(pHit_om, 0x4000, 0) & 1) == 0)
			{
				Hit_cnt = 0;
				for (ppEm = G.pEnemy + 1; ppEm != G.ppEnemy_end; ppEm++)
				{
					if (((*ppEm)->Be_flg & 1) != 0)
						Hit_cnt += Oma_ck_oba(*ppEm, pHit_om, 1);
				}
				for (pOm2 = G.Ob_model; pOm2 != G.pObj_work_end; pOm2++)
				{
					if ((pHit_om != pOm2) && ((pOm2->Be_flg & 1) != 0))
						Hit_cnt += Oma_ck_om(pOm2, pHit_om);
				}
				if (Hit_cnt == 0)
				{
					G.Status_flg |= STAT_PUSH_OBJ;
					if (pHit_om->Push_cnt != 6)
					{
						pHit_om->Push_cnt = 7;
						goto no_push;
					}
					goto restore;
				}
			}
			pHit_om->Push_cnt = 0;
		}
restore:
		pHit_om->Pos_x = Bak_pos_x;
		pHit_om->Pos_z = Bak_pos_z;
		Oma_set_ofs(pHit_om);
		Sca_ck_hit_om(pHit_om, 0x4000, 0);
	}

no_push:
	for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++)
	{
		if ((pOm->Be_flg & 1) != 0)
			Oma_ck_oba((ENEMY_WORK *)&G.Player, pOm, 0);
		pOm->Sca_old_x = (SHORT)pOm->Atd[0].X;
		pOm->Sca_old_z = (SHORT)pOm->Atd[0].Z;
	}

	if (G.Player.pOn_om == NULL)
		G.Player.Status_flg &= 0xf9ff;
}

/* Om_trans - 0x800363f0
   Draw all object models.
*/
/* DS 0x800363f0 (Om_trans) */
void Om_trans(void)
{
	MATRIX          M;
	OBJ_MODEL_WORK  *pOm;
	VECTOR          *pV;
	ULONG           Cnt;
	ULONG           D0;
	UCHAR           nOmodel;

	nOmodel = G.pRoom->nOmodel;
	if (nOmodel == 0)
		return;

	pOm = G.Ob_model;
	Cnt = 0;
	do {
		Cnt++;
		if (((pOm->Be_flg & 1) != 0) && ((pOm->Be_flg & 0x80000) == 0))
		{
			RotMatrix((SVECTOR *)&pOm->Cdir_x, (MATRIX *)pOm->M);
			CompM((MATRIX *)pOm->pSuper, (MATRIX *)pOm->M, &pOm->Workm);
			pV = (VECTOR *)pOm->Workm.t;
			if (pOm->Id == 2)
				pOm->Workm.t[1] -= 900;
			Set_light(pV);
			SetColorMatrix((MATRIX *)&COLORMATRIX);
			MulMatrix0((MATRIX *)&LIGHTMATRIX, &pOm->Workm, &M);
			SetLightMatrix(&M);
			CompM(&GsWSMATRIX, &pOm->Workm, &M);
			SetRotMatrix(&M);
			SetTransMatrix(&M);
			D0 = (pOm->Attribute >> 1) & 1;

			if (pOm->Id == 1)
			{
				Mizu_trans(pOm, D0);
			}
			else if ((Hit_ck_point4(pV, G.Cccut) != 0) || ((pOm->Be_flg & 0x10) != 0))
			{
				if ((pOm->Parts.Be_flg & 8) != 0)
				{
					Bomb_Parts_SortGT3((TMD_MEM *)pOm->pTmd,
									   (POLY_GT3 *)(pOm->pPacket + G.Cbuf_id * 10), &pOm->Parts);
					Bomb_Parts_SortGT4((TMD_MEM *)pOm->pTmd2,
									   (POLY_GT4 *)(pOm->pPacket2 + G.Cbuf_id * 13), &pOm->Parts);
					if (++pOm->Parts.Free[4] == 0x1e)
						pOm->Be_flg = 0;
				}
				else if ((pOm->Attribute & 0x10) == 0)
				{
					SortGT3_neo((TMD_MEM *)pOm->pTmd,
								(POLY_GT3 *)(pOm->pPacket + G.Cbuf_id * 10), pOm->Poly_rgb, D0);
					SortGT4_neo((TMD_MEM *)pOm->pTmd2,
								(POLY_GT4 *)(pOm->pPacket2 + G.Cbuf_id * 13), pOm->Poly_rgb, D0);
				}
				else
				{
					SortGT3_lno((TMD_MEM *)pOm->pTmd,
								(POLY_GT3 *)(pOm->pPacket + G.Cbuf_id * 10), pOm->Poly_rgb, D0);
					SortGT4_lno((TMD_MEM *)pOm->pTmd2,
								(POLY_GT4 *)(pOm->pPacket2 + G.Cbuf_id * 13), pOm->Poly_rgb, D0);
				}
			}

			pOm->Old_x  = (SHORT)pOm->Pos_x;
			pOm->Old_y  = (SHORT)pOm->Pos_y;
			pOm->Old_z  = (SHORT)pOm->Pos_z;
			pOm->Old_x2 = (SHORT)pOm->Atd[0].X;
			pOm->Old_y2 = (SHORT)pOm->Atd[0].Y;
			pOm->Old_z2 = (SHORT)pOm->Atd[0].Z;
		}
		pOm++;
	} while (Cnt < nOmodel);
}

/* Om_move_box - 0x80036710
   Pushed box falling off a ledge: tip over, drop to the ground, bounce.
*/
/* DS reloads Pos_x/Pos_z from the model for each splash store (volatile read) */
#define Vm(p) ((volatile OBJ_MODEL_WORK *)(p))
/* DS 0x80036710 (Om_move_box) */
void Om_move_box(OBJ_MODEL_WORK *pOm)
{
	static SHORT    Pos_spd;        /* DS 0x800c3b10 */
	static SHORT    Pos_spd_add;    /* DS 0x800c3b14 */
	static SHORT    Dir_spd;        /* DS 0x800c3b18 */
	static SHORT    Dir_spd_add;    /* DS 0x800c3b1c */
	static SHORT    Fall_spd;       /* DS 0x800c3b20 */
	static SHORT    Dir_sum;        /* DS 0x800c3b24 */
	static SHORT    Ground;         /* DS 0x800c3b28 */
	static SVECTOR  Add_dir;        /* DS 0x800c3b2c */
	static UCHAR    In_water_flg;   /* DS 0x800c3b34 */
	volatile VECTOR Spl_pos[3];        /* DS stack sp+0x10, written only */

	switch (pOm->Routine_0)
	{
	case 0:
		return;

	case 1:
		Pos_spd_add  = 0xd;
		Pos_spd      = 0xd;
		Dir_spd_add  = 8;
		Dir_sum      = 0;
		Dir_spd      = 0;
		Add_dir.vx   = 0;
		Add_dir.vy   = 0;
		Add_dir.vz   = 0;
		Fall_spd     = 0;
		In_water_flg = 0;
		pOm->Routine_0 = 2;
		pOm->Routine_1 = 0;
		/* fall through */

	case 2:
		pOm->Pos_x += Pos_spd * Om_fall_dir_x;
		pOm->Pos_z += Pos_spd * Om_fall_dir_z;
		if (Dir_sum < 0x400)
		{
			Dir_sum += Dir_spd;
			if (0x400 < Dir_sum)
				Dir_spd = (Dir_spd + 0x400) - Dir_sum;
			Add_dir.vx = Dir_spd * -Om_fall_dir_z;
			Add_dir.vz = Om_fall_dir_x * Dir_spd;
		}
		else
		{
			Add_dir.vz = 0;
			Add_dir.vx = 0;
		}
		Dir_spd += Dir_spd_add;
		Pos_spd += Pos_spd_add;

		if (++pOm->Routine_1 < 0xd)
		{
			pOm->Pos_y += (SHORT)(Pos_spd / 3);
		}
		else
		{
			Pos_spd_add = 0;
			pOm->Pos_y += Fall_spd;
			Fall_spd   += 0xa0;
			if (pOm->Routine_1 == 0xd)
			{
				/* NOTE: the binary leaves the 4th argument register (a3) unset */
				Ground = (SHORT)Sca_get_ground((VECTOR *)pOm->Atd, 0x1c2, 0x4000, 0);
				pOm->Old_x  = (SHORT)pOm->Pos_x;
				pOm->Old_z  = (SHORT)pOm->Pos_z;
				/* DS: nFloor = (sign of ground word) - ((char)(ground / 0x708) + (char)(ground >> 7)) */
				pOm->nFloor = (CHAR)(((Ground < 0) ? -1 : 0)
								- ((CHAR)(Ground / 0x708) + (CHAR)(Ground >> 7)));
			}
			if ((In_water_flg == 0) && (pOm->Water != 0) && (pOm->Water < pOm->Pos_y))
			{
				/* DS builds 8 splash positions (Pos +/-900, +/-225) into a stack VECTOR
				   that is never read (dead stores DS performs, 0x48 frame); the flag is kept. */
				Spl_pos[0].vy = Vm(pOm)->Pos_y;
				Spl_pos[0].vx = Vm(pOm)->Pos_x + 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 225;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 225;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 900;
				Spl_pos[0].vx = Vm(pOm)->Pos_x + 225;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 900;
				Spl_pos[0].vx = Vm(pOm)->Pos_x - 225;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 900;
				Spl_pos[0].vx = Vm(pOm)->Pos_x - 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 900;
				Spl_pos[0].vz = Vm(pOm)->Pos_z + 225;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 225;
				In_water_flg = 1;
				Spl_pos[0].vz = Vm(pOm)->Pos_z - 900;
			}
			if (Ground < pOm->Pos_y)
			{
				pOm->Pos_y     = Ground;
				pOm->Routine_0 = 3;
			}
		}
		break;

	case 3:     /* bounce up */
		pOm->Pos_y -= 30;
		pOm->Pos_x += Om_fall_dir_x * 20;
		pOm->Pos_z += Om_fall_dir_z * 20;
		Add_dir.vx  = -(Om_fall_dir_z * 30);
		Add_dir.vz  = Om_fall_dir_x * 30;
		pOm->Routine_0++;
		break;

	case 4:     /* settle */
		pOm->Pos_y += 30;
		pOm->Pos_x += Om_fall_dir_x * 20;
		pOm->Pos_z += Om_fall_dir_z * 20;
		Add_dir.vx  = Om_fall_dir_z * 30;
		Add_dir.vz  = -(Om_fall_dir_x * 30);
		pOm->Routine_0 = 0;
		break;
	}
	Rot_add_matrix(&pOm->Pos_x, &pOm->Cdir_x, (MATRIX *)pOm->M, &Add_dir);
}

/* Om_set_fall_dir - 0x80036c5c
   Choose the fall axis (+/-1 on X or Z) from the player's relative position.
*/
/* DS 0x80036c5c (Om_set_fall_dir) */
void Om_set_fall_dir(OBJ_MODEL_WORK *pOm)
{
	SHORT   Dx;

	Dx = (SHORT)(pOm->Pos_x - G.Player.Pos_x);
	if (((Dx < 0) ? -Dx : Dx) < 900)
	{
		Om_fall_dir_z = (((SHORT)(pOm->Pos_z - G.Player.Pos_z)) < 0) ? -1 : 1;
		Om_fall_dir_x = 0;
	}
	else
	{
		Om_fall_dir_x = (Dx < 0) ? -1 : 1;
		Om_fall_dir_z = 0;
	}
	Om_fall_old_dir_x = pOm->Cdir_x;
	Om_fall_old_dir_z = pOm->Cdir_z;
}

/* Ob_test_load2 - 0x80036cf8
   Debug leftover: builds the test file names on the stack and does nothing else.
*/
/* DS 0x80036cf8 (Ob_test_load2) */
ULONG Ob_test_load2(void)
{
	/* DS 0x80036cf8: retail frame 0x50 keeps a 16-byte area below tim_file (0x10) and tmd_file (0x30) */
	LONG    Frame_pad[4];
	UCHAR   tim_file[31] = "d:/bio2/room/data/tetubox1.tim";
	UCHAR   tmd_file[31] = "d:/bio2/room/data/tetubox1.tm2";

	(void)Frame_pad;
	(void)tim_file;
	return tmd_file[30];
}

/* Oma_set_ofs - 0x80036e30
   Place an object's AT box; W/D and X/Z offsets swap when rotated 90 degrees.
*/
/* DS 0x80036e30 (Oma_set_ofs) */
void Oma_set_ofs(OBJ_MODEL_WORK *pOm)
{
	if ((pOm->Cdir_y & 0x400) == 0)
	{
		pOm->Atd[0].Atw_x = pOm->Atd[0].Ofs_x;
		pOm->Atd[0].Atw_z = pOm->Atd[0].Ofs_z;
		pOm->Atd[0].W     = pOm->Atd[0].At_w;
		pOm->Atd[0].D     = pOm->Atd[0].At_d;
	}
	else
	{
		pOm->Atd[0].Atw_x = pOm->Atd[0].Ofs_z;
		pOm->Atd[0].Atw_z = pOm->Atd[0].Ofs_x;
		pOm->Atd[0].W     = pOm->Atd[0].At_d;
		pOm->Atd[0].D     = pOm->Atd[0].At_w;
	}
	pOm->Atd[0].X = pOm->Pos_x + pOm->Atd[0].Atw_x;
	pOm->Atd[0].Y = pOm->Pos_y + pOm->Atd[0].Ofs_y;
	pOm->Atd[0].Z = pOm->Pos_z + pOm->Atd[0].Atw_z;
}

/* Om_explosion_set - 0x80036eb4 .. 0x80036ef7 (0x44 bytes)
   No Ghidra function here; body taken from the raw SLUS_007.48 words (re-checked 2026-10-13:
   every store, Free[1] = -10 from "li v0,-0xa", and the jr ra at 0x80036ef0 match).
*/
/* DS 0x80036eb4 (Om_explosion_set) */
void Om_explosion_set(OBJ_MODEL_WORK *pOm)
{
	pOm->Parts.Be_flg  |= 0x4a;
	pOm->Parts.Poly_rgb = 0x404040;
	pOm->Parts.Free[0]  = pOm->Cdir_y + 0x800;
	pOm->Parts.Free[1]  = -10;
	pOm->Parts.Free[2]  = 0;
	pOm->Parts.Free[3]  = 0;
	pOm->Parts.Free[4]  = 0x8000;
	pOm->Parts.Free[6]  = 0;
}
