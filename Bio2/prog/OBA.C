/*
 * OBA.C - Enemy-vs-enemy and enemy-vs-stage hit checks
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OBA.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "MAIN.H"
#include "OBA.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "OMA.H"
#include "SCA.H"
#include "DIVCHK.H"

/* Oba_ck_hit - 0x80034d0c
   Cylinder-vs-cylinder hit check between every AT box of pEm1 and pEm2.
   Pushes pEm2 out of pEm1; returns 1 if any pair collided.
*/
/* DS stack frame: 0x38 bytes; s0..s7 saved at sp+0x10..0x2c, ra saved at sp+0x30. No local variable slots are
 * used. */
/* DS 0x80034d0c (Oba_ck_hit) */
ULONG Oba_ck_hit(ENEMY_WORK *pEm1, ENEMY_WORK *pEm2)
{
	static LONG Hit_flg;    /* DS 0x800c3af8 */
	static LONG nOb1;       /* DS 0x800c3afc */
	static LONG nOb2;       /* DS 0x800c3b00 */

	LONG        x, z, r, r2, h, y;
	LONG        w1;
	LONG        hosei_x, hosei_z;
	AT_DATA     *pAtd1;
	AT_DATA     *pAtd2;

	if (((pEm1->Be_flg | pEm2->Be_flg) & 2) != 0)
		return 0;
	if ((pEm1->Be_flg & pEm2->Be_flg & 0x1000) != 0)
		return 0;
	if ((pEm2->Be_flg & 4) != 0)
		return 0;

	nOb1    = pEm1->nOba;
	pAtd1   = pEm1->Atd;
	Hit_flg = 0;

	/* The loops are written with gotos and the counters are statics: DS reloads the counters from memory
	 * each pass and hoists no addresses or constants out of the loops. w1 is the first box radius. */
out_loop:
	if (--nOb1 < 0)
		goto out_end;
	pAtd2 = pEm2->Atd;
	w1    = pAtd1->At_w;
	nOb2  = pEm2->nOba;
in_loop:
	if (--nOb2 < 0)
		goto in_end;
	r = w1 + pAtd2->At_w;
	x = pAtd2->X - pAtd1->X;
	if ((ULONG)(x + r) <= (ULONG)(r * 2))
	{
		z = pAtd2->Z - pAtd1->Z;
		if ((ULONG)(z + r) <= (ULONG)(r * 2))
		{
			r2 = SquareRoot0(x * x + z * z);
			r -= r2;
			if (0 < r)
			{
				y = pAtd2->Y - pAtd1->Y;
				h = (ULONG)pAtd2->At_h + (ULONG)pAtd1->At_h;
				if ((-h < y) && (y < h))
				{
					r2++;
					DIV_S(hosei_x, x * r, r2);
					y       = ((LONG)pEm2->Old_y + (LONG)pAtd2->Ofs_y) - pAtd1->Y;
					DIV_S(hosei_z, z * r, r2);

					/* pEm2 was not overlapping vertically last frame:
					   if it crossed pEm1 this frame, push it back to the far side */
					if ((y <= -h) || (h <= y))
					{
						if (((pEm2->Old_x < pEm1->Pos_x) && (pEm1->Pos_x < pEm2->Pos_x)) || ((pEm1->Pos_x < pEm2->Old_x) && (pEm2->Pos_x < pEm1->Pos_x)))
						{
							DIV_S(hosei_x, -(x * r), r2);
							if (hosei_x > 0)
								hosei_x = w1 * 2 - hosei_x;
							else
								hosei_x = -(w1 * 2) - hosei_x;
						}
						if (((pEm2->Old_z < pEm1->Pos_z) && (pEm1->Pos_z < pEm2->Pos_z)) || ((pEm1->Pos_z < pEm2->Old_z) && (pEm2->Pos_z < pEm1->Pos_z)))
						{
							DIV_S(hosei_z, -(z * r), r2);
							if (hosei_z > 0)
								hosei_z = w1 * 2 - hosei_z;
							else
								hosei_z = -(w1 * 2) - hosei_z;
						}
					}

					if ((pEm1->Be_flg & 0x100000) != 0)
					{
						/* NOTE: binary clamps a positive hosei_x by writing hosei_z = 100 */
						if (100 < ((hosei_x < 0) ? -hosei_x : hosei_x))
						{
							if (hosei_x < 0)
								hosei_x = -100;
							else
								hosei_z = 100;
						}
						if (100 < ((hosei_z < 0) ? -hosei_z : hosei_z))
						{
							if (hosei_z < 0)
								hosei_z = -100;
							else
								hosei_z = 100;
						}
					}

					Hit_flg      = 1;
					pEm2->Pos_x += hosei_x;
					pEm2->Pos_z += hosei_z;
					pAtd2->X    += hosei_x;
					pAtd2->Z    += hosei_z;
					pEm2->At_em_no = pEm1->Work_no;
				}
			}
		}
	}
	pAtd2++;
	goto in_loop;
in_end:
	pAtd1++;
	goto out_loop;
out_end:
	return Hit_flg;
}

/* Oba_ck_em_at - 0x8003513c  (retail-only)
   Point (cylinder of radius R / half-height H) vs. every live enemy AT box.
   Returns 2 if it hit the player, 1 if it hit another enemy, 0 if nothing.
*/
/* DS stack frame: 0x38 bytes; s0..s7 saved at sp+0x10..0x2c, ra saved at sp+0x30. No local variable slots are
 * used. */
/* DS 0x8003513c (Oba_ck_em_at) */
ULONG Oba_ck_em_at(SVECTOR *pPos, LONG R, LONG H)
{
	static LONG nOb;        /* DS 0x800c3b08 */

	LONG        x, z, r, h, y;
	AT_DATA     *pAtd;
	ENEMY_WORK  *pEm;
	ENEMY_WORK  **ppEm;

	for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++)
	{
		pEm = *ppEm;
		/* skip enemies that are not active (Be_flg bit 0 clear) and those with bit 1 set */
		if (((pEm->Be_flg & 1) == 0) || ((pEm->Be_flg & 2) != 0))
			continue;

		pAtd = pEm->Atd;
		nOb  = pEm->nOba;
		while (--nOb >= 0)
		{
			r = R + pAtd->At_w;
			x = pAtd->X - pPos->vx;
			if ((ULONG)(x + r) <= (ULONG)(r * 2))
			{
				z = pAtd->Z - pPos->vz;
				if (((ULONG)(z + r) <= (ULONG)(r * 2)) && (0 < r - SquareRoot0(x * x + z * z)))
				{
					h = (ULONG)pAtd->At_h + H;
					y = pAtd->Y - pPos->vy;
					if ((-h < y) && (y < h))
					{
						if (pEm != (ENEMY_WORK *)&G.Player)
							return 1;
						return 2;
					}
				}
			}
			pAtd++;
		}
	}
	return 0;
}

/* Oba_ck_only - 0x800352d8
   Check whether pEm1 placed at pV would touch pEm2 (first AT box only).
*/
/* DS stack frame: 0x28 bytes; s0..s3 saved at sp+0x10..0x1c, ra saved at sp+0x20. No local variable slots are
 * used. */
/* DS 0x800352d8 (Oba_ck_only) */
ULONG Oba_ck_only(VECTOR *pV, ENEMY_WORK *pEm1, ENEMY_WORK *pEm2)
{
	LONG    x, z, r, h, y;

	if (((pEm1->Be_flg | pEm2->Be_flg) & 2) != 0)
		return 0;
	if ((pEm1->Be_flg & pEm2->Be_flg & 0x1000) != 0)
		return 0;

	r = (ULONG)pEm1->Atd[0].At_w + (ULONG)pEm2->Atd[0].At_w;
	x = pEm2->Atd[0].X - (pV->vx + pEm1->Atd[0].Atw_x);
	z = pEm2->Atd[0].Z - (pV->vz + pEm1->Atd[0].Atw_z);
	if ((ULONG)(x + r) > (ULONG)(r * 2))
		return 0;
	if ((ULONG)(z + r) > (ULONG)(r * 2))
		return 0;
	if (r - SquareRoot0(x * x + z * z) <= 0)
		return 0;

	h = (ULONG)pEm2->Atd[0].At_h + (ULONG)pEm1->Atd[0].At_h;
	y = pEm2->Atd[0].Y - (pV->vy + pEm1->Atd[0].Ofs_y);
	if ((y <= -h) || (h <= y))
		return 0;
	return 1;
}

/* Oba_set_ofs - 0x80035408
   Rotate every AT box offset by the body Y direction (and super matrix)
   and place the boxes around pPos.
*/
/* DS stack frame: 0x68 bytes; s0..s5 saved at sp+0x48..0x5c, ra saved at sp+0x60. Locals: ATsvec: sp+0x10, 8
 * bytes; ATvec: sp+0x18, 16 bytes; ATmp_m: sp+0x28, 32 bytes. */
/* DS 0x80035408 (Oba_set_ofs) */
void Oba_set_ofs(ENEMY_WORK *pEm, VECTOR *pPos)
{
	SVECTOR ATsvec;
	VECTOR  ATvec;
	MATRIX  ATmp_m;
	ULONG   nOba;
	AT_DATA *pAtd;

	pAtd = &pEm->Atd[0];
	nOba = pEm->nOba;
	if (nOba == 0)
		return;
	do {
		ATsvec.vy = pEm->Cdir_y;
		ATsvec.vz = 0;
		ATsvec.vx = 0;
		RotMatrix(&ATsvec, &ATmp_m);
		if ((pEm->Be_flg & 0x2000000) != 0)
			MulMatrix2((MATRIX *)pEm->pSuper, &ATmp_m);

		ATsvec.vx = pAtd->Ofs_x;
		ATsvec.vy = pAtd->Ofs_y;
		ATsvec.vz = pAtd->Ofs_z;
		ApplyMatrix(&ATmp_m, &ATsvec, &ATvec);
		pAtd->Atw_x = (SHORT)ATvec.vx;
		pAtd->Atw_z = (SHORT)ATvec.vz;
		pAtd->X     = pPos->vx + ATvec.vx;
		pAtd->Y     = pPos->vy + ATvec.vy;
		pAtd->Z     = pPos->vz + ATvec.vz;
		pAtd++;
		nOba--;
	} while (nOba != 0);
}

/* Oba_ck_em - 0x80035530
   Push pEm out of every other live enemy.
*/
/* DS stack frame: 0x20 bytes; s0..s1 saved at sp+0x10..0x14, ra saved at sp+0x18. No local variable slots are
 * used. */
/* DS 0x80035530 (Oba_ck_em) */
void Oba_ck_em(ENEMY_WORK *pEm)
{
	ENEMY_WORK  *pEm2;
	ENEMY_WORK  **ppEm;

	Oba_set_ofs(pEm, (VECTOR *)&pEm->Pos_x);
	pEm->At_em_no = 0xff;
	/* goto loop instead of for: DS does not treat this as a loop, so G.ppEnemy_end is re-read via its own
	   lui each pass instead of &G being held in a saved register across the Oba_ck_hit call */
	ppEm = G.pEnemy;
	if (ppEm != G.ppEnemy_end)
	{
em_loop:
		pEm2 = *ppEm;
		if ((pEm != pEm2) && ((pEm2->Be_flg & 1) != 0))
			Oba_ck_hit(pEm2, pEm);
		ppEm++;
		if (ppEm != G.ppEnemy_end)
			goto em_loop;
	}
}

/* Oba_ck_em2 - 0x800355c4
   As Oba_ck_em, but also records which enemy slots were hit in At_em_flg
   (bit 15 = slot 0, bit 14 = slot 1, ...).
*/
/* DS stack frame: 0x28 bytes; s0..s3 saved at sp+0x10..0x1c, ra saved at sp+0x20. No local variable slots are
 * used. */
/* DS 0x800355c4 (Oba_ck_em2) */
void Oba_ck_em2(ENEMY_WORK *pEm)
{
	ENEMY_WORK  *pEm2;
	ENEMY_WORK  **ppEm;
	ULONG       Set_bit;
	ULONG       Ck_bit;

	Oba_set_ofs(pEm, (VECTOR *)&pEm->Pos_x);
	Set_bit = 0x8000;
	pEm->At_em_no = 0xff;
	Ck_bit = 0;
	/* goto loop instead of for: DS does not treat this as a loop, so &G is not held in a saved register
	   across the Oba_ck_hit call */
	ppEm = G.pEnemy;
	if (ppEm != G.ppEnemy_end)
	{
em2_loop:
		pEm2 = *ppEm;
		if ((pEm != pEm2) && ((pEm2->Be_flg & 1) != 0) && (Oba_ck_hit(pEm2, pEm) != 0))
			Ck_bit |= Set_bit;
		Set_bit >>= 1;
		ppEm++;
		if (ppEm != G.ppEnemy_end)
			goto em2_loop;
	}
	pEm->At_em_flg = (USHORT)Ck_bit;
}

/* Sca_ck_em - 0x8003567c
   Collision against the room's SCA (static collision) data.
*/
/* DS stack frame: 0x18 bytes; s0 saved at sp+0x10, ra saved at sp+0x14. No local variable slots are used. */
/* DS 0x8003567c (Sca_ck_em) */
void Sca_ck_em(ENEMY_WORK *pEm, ULONG Sc_id)
{
	pEm->Sca_info = 0;
	if (((pEm->Be_flg & 8) == 0) && (pEm->pOn_om == NULL))
	{
		pEm->Be_flg &= ~0x4000;
		pEm->Sca_info = Sca_ck_hit((VECTOR *)pEm->Atd, (ULONG)pEm->Atd[0].W, 1 << pEm->nFloor, Sc_id | 0x1000000);
		pEm->pSca_hit_data = G.pSca_hit_data[0];
	}
	pEm->Sca_old_x = (SHORT)pEm->Atd[0].X;
	pEm->Sca_old_z = (SHORT)pEm->Atd[0].Z;
}
