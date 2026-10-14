/*
 * EM_SUB2.C - Enemy subroutines 2 (approach / motion play / super-matrix turning)
 * Resident Evil 2 Dual Shock (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\EM_SUB2.C
 *
 * DS retail range: 8001a0e0 (Approach) .. 8001aa37 (Get_parts_world_dir)
 */

#include "TYPES.H"
#include "MAIN.H"
#include <LIBAPI.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include "EM_SUB2.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "SCA.H"
#include "SUB.H"
#include "DIVCHK.H"

/* Forward unit vector used by the super-matrix turning helpers        */
/* (DS 0x8001088c: { 0x7fff, 0, 0, 0 })                                */
static const SVECTOR Super_front = { 0x7fff, 0, 0, 0 };

/* ------------------------------------------------------------------ */
/* Approach                                          DS 0x8001a0e0     */
/* Moves pPos toward pDest (XZ) by Spd. When the sign of either delta  */
/* flips the destination was passed: snap to it and return 1.          */
/* ------------------------------------------------------------------ */
/* DS 0x8001a0e0 (Approach) */
ULONG Approach(VECTOR *pPos, VECTOR *pDest, SHORT Spd)
{
	SVECTOR	Add;
	ULONG	Pos_info;

	Add.vx = Spd;
	Add.vz = 0;
	Add.vy = 0;

	Pos_info = ((pPos->vx - pDest->vx) & 0x80000000)
			 | (((pPos->vz - pDest->vz) & 0x80000000) >> 1);

	Rot_vector(Direction_ck(pPos->vx, pPos->vz, pDest->vx, pDest->vz), &Add, &Add);
	pPos->vx += Add.vx;
	pPos->vz += Add.vz;

	Pos_info ^= (pPos->vx - pDest->vx) & 0x80000000;
	Pos_info ^= ((pPos->vz - pDest->vz) & 0x80000000) >> 1;
	if (Pos_info == 0)
		return 0;

	pPos->vx = pDest->vx;
	pPos->vz = pDest->vz;
	return 1;
}

/* ------------------------------------------------------------------ */
/* Approach_one                                      DS 0x8001a1e0     */
/* One-axis version of Approach.                                       */
/* ------------------------------------------------------------------ */
/* DS 0x8001a1e0 (Approach_one) */
ULONG Approach_one(LONG *pX, LONG Dx, SHORT Spd)
{
	ULONG	Pos_info;

	Pos_info = (*pX - Dx) & 0x80000000;
	if (Pos_info == 0)
		*pX -= Spd;
	else
		*pX += Spd;

	if (Pos_info == ((*pX - Dx) & 0x80000000))
		return 0;

	*pX = Dx;
	return 1;
}

/* ------------------------------------------------------------------ */
/* Mot_play                                          DS 0x8001a240     */
/* Plays motion No (Joint_move). Returns 1 when the motion has ended.  */
/* ------------------------------------------------------------------ */
/* DS 0x8001a240 (Mot_play) */
ULONG Mot_play(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg)
{
	ULONG	Div;

	if (pEm->Move_no != No)
	{
		pEm->Move_no   = (UCHAR)No;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = (UCHAR)Hokan;
	}
	DIV_U(Div, 0x1000, Hokan + 1);	/* DS: unsigned divide with the zero check */
	return Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr,
					  (Div) | (Flg << 16)) != 0;
}

/* ------------------------------------------------------------------ */
/* Mot_play2                                         DS 0x8001a2a4     */
/* As Mot_play, using Joint_move2.                                     */
/* ------------------------------------------------------------------ */
/* DS 0x8001a2a4 (Mot_play2) */
ULONG Mot_play2(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg)
{
	ULONG	Div;

	if (pEm->Move_no != No)
	{
		pEm->Move_no   = (UCHAR)No;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = (UCHAR)Hokan;
	}
	DIV_U(Div, 0x1000, Hokan + 1);	/* DS: unsigned divide with the zero check */
	return Joint_move2(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr,
					   (Div) | (Flg << 16)) != 0;
}

/* ------------------------------------------------------------------ */
/* Mot_play_init                                     DS 0x8001a308     */
/* Forces a motion restart (Move_no = 0xff) when Routine_0..2 differ   */
/* from the saved routine word. Always returns 0xff.                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001a308 (Mot_play_init) */
ULONG Mot_play_init(ENEMY_WORK *pEm, ULONG Routine_bak)
{
	/* Only Routine_0..Routine_2 take part (low three bytes, Routine_0 in the lowest) */
	if (((ULONG)pEm->Routine_0 | ((ULONG)pEm->Routine_1 << 8) | ((ULONG)pEm->Routine_2 << 16)) != (Routine_bak & 0xffffff))
		pEm->Move_no = 0xff;
	return 0xff;
}

/* ------------------------------------------------------------------ */
/* Mot_play_sp                                       DS 0x8001a330     */
/* Mot_play with root-motion speed: moves pEm in XZ while playing.     */
/* ------------------------------------------------------------------ */
/* DS 0x8001a330 (Mot_play_sp) */
ULONG Mot_play_sp(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg)
{
	ULONG	Div;
	ULONG	End;

	if (pEm->Move_no != No)
	{
		pEm->Move_no   = (UCHAR)No;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = (UCHAR)Hokan;
	}
	Spd_get(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0);
	DIV_U(Div, 0x1000, Hokan + 1);	/* DS: unsigned divide with the zero check */
	End = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr,
					 (Div) | (Flg << 16));
	if (End == 0)
		Add_speedXZ(pEm, 0);
	return End != 0;
}

/* ------------------------------------------------------------------ */
/* Mot_play2_sp                                      DS 0x8001a3e8     */
/* Mot_play2 with root-motion speed through the super matrix.          */
/* ------------------------------------------------------------------ */
/* DS 0x8001a3e8 (Mot_play2_sp) */
ULONG Mot_play2_sp(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg)
{
	ULONG	Div;
	ULONG	End;

	if (pEm->Move_no != No)
	{
		pEm->Move_no   = (UCHAR)No;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = (UCHAR)Hokan;
	}
	Spd_get(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0);
	DIV_U(Div, 0x1000, Hokan + 1);	/* DS: unsigned divide with the zero check */
	End = Joint_move2(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr,
					  (Div) | (Flg << 16));
	if (End == 0)
		Add_speedXYZsuper(pEm, 0, 0);
	return End != 0;
}

/* ------------------------------------------------------------------ */
/* Mot_play2_spxz                                    DS 0x8001a4a0     */
/* Mot_play2 with XZ root-motion speed.                                */
/* ------------------------------------------------------------------ */
/* DS 0x8001a4a0 (Mot_play2_spxz) */
ULONG Mot_play2_spxz(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg)
{
	ULONG	Div;
	ULONG	End;

	if (pEm->Move_no != No)
	{
		pEm->Move_no   = (UCHAR)No;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = (UCHAR)Hokan;
	}
	Spd_get(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0);
	DIV_U(Div, 0x1000, Hokan + 1);	/* DS: unsigned divide with the zero check */
	End = Joint_move2(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr,
					  (Div) | (Flg << 16));
	if (End == 0)
		Add_speedXZ(pEm, 0);
	return End != 0;
}

/* ------------------------------------------------------------------ */
/* Em_init_super                                     DS 0x8001a558     */
/* ------------------------------------------------------------------ */
/* DS 0x8001a558 (Em_init_super) */
void Em_init_super(ENEMY_WORK *pEm)
{
	pEm->Super_vector.vx = 0;
	pEm->Super_vector.vy = 0;
	pEm->Super_vector.vz = 0;
	RotMatrix(&pEm->Super_vector, &pEm->Super_matrix);
}

/* ------------------------------------------------------------------ */
/* Branch                                            DS 0x8001a58c     */
/* 0: N < A   1: A <= N < B   2: B <= N < C   3: N >= C                */
/* ------------------------------------------------------------------ */
/* DS 0x8001a58c (Branch) */
ULONG Branch(ULONG N, ULONG A, ULONG B, ULONG C)
{
	if (N < A)
		return 0;
	if (N < B)
		return 1;
	if (N < C)
		return 2;
	return 3;
}

/* ------------------------------------------------------------------ */
/* Ck_em_id                                          DS 0x8001a5c0     */
/* Counts active enemies of type Id other than pEm; stores up to Max   */
/* of them into ppWk (if not NULL). Returns the total count.           */
/* ------------------------------------------------------------------ */
/* DS 0x8001a5c0 (Ck_em_id) */
ULONG Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppWk, ULONG Id, ULONG Max)
{
	ULONG		total;
	ENEMY_WORK	*pWk;
	ENEMY_WORK	**ppEm;

	total = 0;
	for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++)
	{
		pWk = *ppEm;
		if ((pWk->Be_flg & 1) && pWk->Id == Id && pWk != pEm)
		{
			total++;
			if (ppWk != NULL && Max != 0)
			{
				*ppWk++ = pWk;
				Max--;
			}
		}
	}
	return total;
}

/* ------------------------------------------------------------------ */
/* Goto00_super                                      DS 0x8001a644     */
/* Turns pEm (Cdir_y) toward pVec, in the plane selected by the        */
/* dominant axis of the super matrix (Get_axis2: 0..5).                */
/* NOTE (faithful to DS): for an axis code > 5, Dir is uninitialised.  */
/* ------------------------------------------------------------------ */
/* DS 0x8001a644 (Goto00_super) */
void Goto00_super(ENEMY_WORK *pEm, VECTOR *pVec, SHORT Add_dir)
{
	LONG	Dir;
	LONG	Rot;
	SVECTOR	Svec;

	Svec = Super_front;
	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Svec, &Svec);

	switch (Get_axis2((MATRIX *)pEm->pSuper))
	{
	case 0:
		Dir  = Direction_ck(pEm->Pos_x, pEm->Pos_z, pVec->vx, pVec->vz);
		Dir -= Direction_ck(0, 0, Svec.vx, Svec.vz);
		break;
	case 1:
		Dir  = -Direction_ck(pEm->Pos_x, pEm->Pos_z, pVec->vx, pVec->vz);
		Dir += Direction_ck(0, 0, Svec.vx, Svec.vz);
		break;
	case 2:
		Dir  = Direction_ck(pEm->Pos_z, pEm->Pos_y, pVec->vz, pVec->vy);
		Dir -= Direction_ck(0, 0, Svec.vz, Svec.vy);
		break;
	case 3:
		Dir  = -Direction_ck(pEm->Pos_z, pEm->Pos_y, pVec->vz, pVec->vy);
		Dir += Direction_ck(0, 0, Svec.vz, Svec.vy);
		break;
	case 4:
		Dir  = Direction_ck(pEm->Pos_y, pEm->Pos_x, pVec->vy, pVec->vx);
		Dir -= Direction_ck(0, 0, Svec.vy, Svec.vx);
		break;
	case 5:
		Dir  = -Direction_ck(pEm->Pos_y, pEm->Pos_x, pVec->vy, pVec->vx);
		Dir += Direction_ck(0, 0, Svec.vy, Svec.vx);
		break;
	}

	Rot = (Dir + Add_dir) & 0xfff;
	if (Rot >= Add_dir * 2)
	{
		if (Rot <= 0x800)
			pEm->Cdir_y += Add_dir;
		else
			pEm->Cdir_y -= Add_dir;
	}
}

/* ------------------------------------------------------------------ */
/* Goto01_super                                      DS 0x8001a858     */
/* Turns pEm (Cdir_y) toward the absolute direction Dir in the plane   */
/* selected by the super matrix (Get_axis2: 0..5).                     */
/* ------------------------------------------------------------------ */
/* DS 0x8001a858 (Goto01_super) */
void Goto01_super(ENEMY_WORK *pEm, LONG Dir, SHORT Add_dir)
{
	LONG	Rot;
	SVECTOR	Svec;

	Svec = Super_front;
	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Svec, &Svec);

	switch (Get_axis2((MATRIX *)pEm->pSuper))
	{
	case 0:
		Dir = Dir - Direction_ck(0, 0, Svec.vx, Svec.vz);
		break;
	case 1:
		Dir = Direction_ck(0, 0, Svec.vx, Svec.vz) - Dir;
		break;
	case 2:
		Dir = Dir - Direction_ck(0, 0, Svec.vz, Svec.vy);
		break;
	case 3:
		Dir = Direction_ck(0, 0, Svec.vz, Svec.vy) - Dir;
		break;
	case 4:
		Dir = Dir - Direction_ck(0, 0, Svec.vy, Svec.vx);
		break;
	case 5:
		Dir = Direction_ck(0, 0, Svec.vy, Svec.vx) - Dir;
		break;
	}

	Rot = (Dir + Add_dir) & 0xfff;
	if (Rot >= Add_dir * 2)
	{
		if (Rot <= 0x800)
			pEm->Cdir_y += Add_dir;
		else
			pEm->Cdir_y -= Add_dir;
	}
}

/* ------------------------------------------------------------------ */
/* Get_parts_world_dir                               DS 0x8001a9bc     */
/* Sums the Cdir of pParts and all its parents up to the root part,    */
/* plus the body Cdir, into pVec.                                      */
/* ------------------------------------------------------------------ */
/* DS 0x8001a9bc (Get_parts_world_dir) */
void Get_parts_world_dir(ENEMY_WORK *pEm, PARTS_W *pParts, SVECTOR *pVec)
{
	pVec->vx = pEm->Cdir_x;
	pVec->vy = pEm->Cdir_y;
	pVec->vz = pEm->Cdir_z;
	for (;;)
	{
		pVec->vx += pParts->Cdir_x;
		pVec->vy += pParts->Cdir_y;
		pVec->vz += pParts->Cdir_z;
		if (pParts == pEm->pSin_parts_ptr)
			break;
		pParts = (PARTS_W *)pParts->pOya_parts;
	}
}
