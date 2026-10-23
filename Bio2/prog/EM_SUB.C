/*
 * EM_SUB.C - Enemy motion/direction/shadow/texture/body-part subroutines
 * Reconstructed from BIO2_BETA.LOG PSYQ symbol records + Ghidra decompilation
 */

#include "TYPES.H"
#include "CD.H"
#include "GTE_OPS.H"
#include "ROOM.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <MALLOC.H>
#include <STDIO.H>
#include <STRINGS.H>
#include <TYPES.H>
#include "EM_SUB.H"
#include "JOINT.H"
#include "LINE_SCR.H"
#include "MAIN.H"
#include "DIVCHK.H"
#include "MD_SUB.H"
#include "SCE1.H"
#include "SND.H"
#include "SPL_AUTO.H"
#include "SUB.H"
#include "ESP_R.H"
#include "PLAYER.H"
#include "SPL.H"
#include "flag.h"

/* Frame record at Bone_adr: null-position vector at +0x06 (retail Spd_get: lhu +0x6, lw +0x8, lhu +0xa) */
typedef struct tagKAN_BONE_REC {
	USHORT dummy00[3];	/* +0x00 .. +0x05 not read in this file */
	SVECTOR Null_pos;	/* +0x06 vx, +0x08 vy, +0x0a vz */
} KAN_BONE_REC;

/* DS stores the vx/vy pair of an SVECTOR as one 32-bit word (vy = high half) */
#define SVEC_XY32(sv, v)	((sv).vx = (SHORT)(v), (sv).vy = (SHORT)((LONG)(v) >> 16))

/* byte access into packed primitive words (UV / CLUT / tpage fields) */
#define PKT_GET8(p)		(*(char *)(p))
#define PKT_SET8(p, v)	(*(char *)(p) = (char)(v))

/* copy one SVECTOR as two 32-bit words (DS uses lw/sw pairs, not lwl/lwr) */
#define COPY_SV8(d, s)	(((ULONG *)(d))[0] = ((ULONG *)(s))[0], ((ULONG *)(d))[1] = ((ULONG *)(s))[1])

/* VECTOR.pad packs two 16-bit words: low = x, high = y */
#define PAD_XY(x, y)		((LONG)((ULONG)(USHORT)(x) | ((ULONG)(USHORT)(y) << 16)))

/* 32-bit field of a MATRIX at byte offset ofs (the decompile addresses MATRIX fields this way) */
#define MTX_L(m, ofs)		(*(LONG *)((UCHAR *)(m) + (ofs)))

/* ------------------------------------------------------------------ */
/* Hani_ck                                                             */
/* Check if vector pV is within the XZ rectangle defined by pPxzhw    */
/* ------------------------------------------------------------------ */
/* DS 0x80015280 (Hani_ck) */
ULONG Hani_ck(VECTOR *pV, short *pPxzhw)
{
	/* pPxzhw is four SHORTs: x origin, z origin, x extent, z extent. Each axis is one unsigned compare:
	   (ULONG)(pos - origin) > (ULONG)extent is true when pos lies below the origin (the difference wraps to a
	   large unsigned value) or beyond origin + extent. DS tests both axes in one || condition and keeps no
	   result local: each out-of-range test branches to the shared return with v0 cleared in its delay slot
	   (bne / clear v0), and only the in-range fall-through loads 1. */
	if ((ULONG)(pV->vx - pPxzhw[0]) > (ULONG)pPxzhw[2] || (ULONG)(pV->vz - pPxzhw[1]) > (ULONG)pPxzhw[3])
	{
		return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------ */
/* Add_speedXZ                                                         */
/* Apply enemy XZ speed in facing direction (muki offset)             */
/* ------------------------------------------------------------------ */
/* DS 0x800152c8 (Add_speedXZ) */
void Add_speedXZ(ENEMY_WORK *pEm, long muki)
{
	SVECTOR Sv;
	MATRIX M;

	Sv.vx = 0;
	Sv.vy = pEm->Cdir_y + (short)muki;
	Sv.vz = 0;
	RotMatrix(&Sv, &M);
	ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
	pEm->Pos_x = pEm->Pos_x + (int)Sv.vx;
	pEm->Pos_z = pEm->Pos_z + (int)Sv.vz;
}

/* ------------------------------------------------------------------ */
/* Add_speedXYZ                                                        */
/* Apply enemy XYZ speed with Y and Z direction offsets               */
/* ------------------------------------------------------------------ */
/* DS 0x80015350 (Add_speedXYZ) */
void Add_speedXYZ(ENEMY_WORK *pEm, long muki_y, long muki_z)
{
	SVECTOR Sv;
	MATRIX M;

	Sv.vx = 0;
	Sv.vy = pEm->Cdir_y + (short)muki_y;
	Sv.vz = pEm->Cdir_z + (short)muki_z;
	RotMatrix(&Sv, &M);
	ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
	pEm->Pos_x = pEm->Pos_x + (int)Sv.vx;
	pEm->Pos_y = pEm->Pos_y + (int)Sv.vy;
	pEm->Pos_z = pEm->Pos_z + (int)Sv.vz;
}

/* ------------------------------------------------------------------ */
/* Add_speedXYZsuper                                                   */
/* Apply enemy XYZ speed through super (parent) matrix                */
/* ------------------------------------------------------------------ */
/* DS 0x800153f4 (Add_speedXYZsuper) */
void Add_speedXYZsuper(ENEMY_WORK *pEm, long muki_y, long muki_z)
{
	SVECTOR Sv;
	MATRIX M;
	MATRIX Tm;

	Sv.vx = 0;
	Sv.vy = pEm->Cdir_y + (short)muki_y;
	Sv.vz = pEm->Cdir_z + (short)muki_z;
	RotMatrix(&Sv, (MATRIX *)&M);
	CompM((MATRIX *)pEm->pSuper, &M, &Tm);
	ApplyMatrixSV((MATRIX *)&Tm, (SVECTOR *)&pEm->Spd_x, &Sv);
	pEm->Pos_x = pEm->Pos_x + (int)Sv.vx;
	pEm->Pos_y = pEm->Pos_y + (int)Sv.vy;
	pEm->Pos_z = pEm->Pos_z + (int)Sv.vz;
}

/* ------------------------------------------------------------------ */
/* Direction_ck                                                        */
/* Return direction angle from (Hontai_x,Hontai_z) to (Target_x,Target_z) */
/* ------------------------------------------------------------------ */
/* DS 0x800154ac (Direction_ck) */
/* Returns long: DS callers that keep the result in a long use v0 directly (no sll/sra); callers that store it in a short narrow it themselves. */
long Direction_ck(long Hontai_x, long Hontai_z, long Target_x, long Target_z)
{
	short Kakudo;
	short Dir;
	int ZZ;
	int XX;
	int Dz;

	/* DS: each argument is narrowed to 16 bits where it is used (sll/sra 16); callers pass full values */
	Dir = 0x400;
	XX = (int)(short)Target_x - (int)(short)Hontai_x;
	Dz = (int)(short)Target_z - (int)(short)Hontai_z;
	if (0 < Dz)
	{
		Dir = 0xc00;
	}
	ZZ = Dz * 0x1000;
	if (XX != 0)
	{
		/* DS: signed divide with the zero and overflow checks */
		DIV_S(ZZ, ZZ, XX);
		ZZ = catan(ZZ);
		Kakudo = 0;
		if (XX < 0)
		{
			Kakudo = 0x800;
		}
		Dir = (Kakudo - (short)ZZ) & 0xfff;
	}
	return Dir;
}

/* ------------------------------------------------------------------ */
/* Goto00                                                              */
/* Turn enemy toward (Vec_x,Vec_z) by at most Add_dir per frame       */
/* ------------------------------------------------------------------ */
/* DS 0x80015558 (Goto00) */
void Goto00(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir)
{
	short Dir;
	short Cdir_sub;
	UINT Tgt_dir;
	UINT Ang_err;
	UINT Turn_spd;

	Turn_spd = (UINT)(USHORT)Add_dir;
	Dir = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)Vec_x, (short)Vec_z);
	Tgt_dir = Dir;
	if ((int)((UINT)Add_dir << 0x10) < 0)
	{
		Turn_spd = -Add_dir;
		Tgt_dir = (Tgt_dir + 0x800) & 0xfff;
	}
	Ang_err = (Turn_spd + (Tgt_dir - (USHORT)pEm->Cdir_y)) & 0xfff;
	if ((int)Ang_err < (int)(Turn_spd << 0x10) >> 0xf)
	{
		pEm->Cdir_y = (short)Tgt_dir;
	}
	else
	{
		Cdir_sub = pEm->Cdir_y - (short)Turn_spd;
		pEm->Cdir_y = Cdir_sub;
		if (Ang_err < 0x801)
		{
			pEm->Cdir_y = Cdir_sub + (short)Turn_spd * 2;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Goto00_ck                                                           */
/* Return remaining angular error for Goto00 target                   */
/* ------------------------------------------------------------------ */
/* DS 0x80015614 (Goto00_ck) */
long Goto00_ck(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir)
{
	short Tgt_dir;
	ULONG Ang_err;

	Tgt_dir = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)Vec_x, (short)Vec_z);
	/* Angle from the current heading to the target, biased by Add_dir and wrapped to 0..0xfff.
	 * DS adds the incoming Add_dir register before narrowing it and reads Cdir_y with lhu. */
	Ang_err = (USHORT)(Add_dir + (Tgt_dir - pEm->Cdir_y)) & 0xfff;
	/* Within twice the turn step of the target: no turn needed. */
	if ((long)Ang_err < Add_dir * 2)
	{
		return 0;
	}
	/* Past half a circle the target is on the other side: turn the other way. */
	if (Ang_err > 0x800)
	{
		return -Add_dir;
	}
	return Add_dir;
}

/* ------------------------------------------------------------------ */
/* Goto01                                                              */
/* Turn enemy toward explicit Dir angle by at most Add_dir per frame  */
/* ------------------------------------------------------------------ */
/* DS 0x8001569c (Goto01) */
void Goto01(ENEMY_WORK *pEm, short Dir, short Add_dir)
{
	/* retail reserves 8 more bytes of locals (Goto01 frame 8); never read */
	volatile int Rsv[2];
	short Cdir_sub;
	UINT Ang_err;
	UINT Turn_spd;
	UINT Tgt_dir;

	Turn_spd = (UINT)(USHORT)Add_dir;
	Tgt_dir = (UINT)(USHORT)Dir;
	if ((int)(Turn_spd << 0x10) < 0)
	{
		Turn_spd = -((int)(Turn_spd << 0x10) >> 0x10);
		Tgt_dir = (Tgt_dir + 0x800) & 0xfff;
	}
	Ang_err = (Turn_spd + (Tgt_dir - (USHORT)pEm->Cdir_y)) & 0xfff;
	Cdir_sub = pEm->Cdir_y - (short)Turn_spd;
	if ((int)Ang_err < (int)(Turn_spd << 0x10) >> 0xf)
	{
		pEm->Cdir_y = (short)Tgt_dir;
	}
	else
	{
		pEm->Cdir_y = Cdir_sub;
		if (Ang_err < 0x801)
		{
			pEm->Cdir_y = Cdir_sub + (short)Turn_spd * 2;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Goto01_ck                                                           */
/* Return remaining angular error for Goto01 target                   */
/* ------------------------------------------------------------------ */
/* DS 0x80015714 (Goto01_ck) */
long Goto01_ck(ENEMY_WORK *pEm, long Dir, long Add_dir)
{
	ULONG Rot;

	/* Angle from the current heading to Dir, offset by Add_dir and wrapped to 0..0xfff.
	   DS reads Cdir_y unsigned (lhu); only the low 12 bits of the sum are kept. */
	Rot = (Add_dir + (Dir - (USHORT)pEm->Cdir_y)) & 0xfff;
	/* Within Add_dir of Dir on the positive side: no turn needed. */
	if ((LONG)Rot < (short)Add_dir * 2)
	{
		return 0;
	}
	/* Past the half turn: turn the other way. */
	if (Rot > 0x800)
	{
		return -(short)Add_dir;
	}
	return (short)Add_dir;
}

/* ------------------------------------------------------------------ */
/* Dir_pos_ck                                                          */
/* Check if pP lies within Hani_dir of pV's heading Hed_dir           */
/* ------------------------------------------------------------------ */
/* DS 0x80015758 (Dir_pos_ck) */
long Dir_pos_ck(VECTOR *pV, VECTOR *pP, short Hed_dir, short Hani_dir)
{
	short Dir;

	Dir = Direction_ck((short)pV->vx, (short)pV->vz, (short)pP->vx, (short)pP->vz);
	return (((((int)Dir - (int)Hed_dir) + (int)Hani_dir) & 0xfffU) < (UINT)((int)Hani_dir << 1)) ^ 1;
}

/* ------------------------------------------------------------------ */
/* L_pos_ckXZ                                                          */
/* Return nonzero if XZ distance between pV and pP is less than Ll    */
/* ------------------------------------------------------------------ */
/* DS 0x800157d4 (L_pos_ckXZ) */
long L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll)
{
	int Dx;
	int Dz;
	UINT Dist;

	Dx = pV->vx - pP->vx;
	Dz = pV->vz - pP->vz;
	Dist = SquareRoot0(Dx * Dx + Dz * Dz);
	return (UINT)(Dist < Ll);
}

/* ------------------------------------------------------------------ */
/* L_pos_ckXZM                                                         */
/* Distance check from matrix-transformed point to pP (XZ only)       */
/* ------------------------------------------------------------------ */
/* DS 0x80015834 (L_pos_ckXZM) */
long L_pos_ckXZM(MATRIX *pM, VECTOR *pP, ULONG Ll, SVECTOR *pSv)
{
	int Dz;
	UINT Dist;
	int Dx;
	SVECTOR Sv;
	VECTOR V;

	/* DS copies vx and vy as one word (sw at +0x10, lw from pSv+0) and vz as a
	   halfword (sh at +0x14); vx and vy are written through one ULONG here. */
	if (pSv == (SVECTOR *)0x0)
	{
		*(ULONG *)&Sv.vx = 0;
		Sv.vz = 0;
	}
	else
	{
		*(ULONG *)&Sv.vx = *(ULONG *)&pSv->vx;
		Sv.vz = pSv->vz;
	}
	ApplyMatrix((MATRIX *)pM, &Sv, &V);
	V.vx += pM->t[0];
	V.vy += pM->t[1];
	V.vz += pM->t[2];
	Dx = V.vx - pP->vx;
	Dz = V.vz - pP->vz;
	Dist = SquareRoot0(Dx * Dx + Dz * Dz);
	return (UINT)(Dist < Ll);
}

/* ------------------------------------------------------------------ */
/* Cdir_ck                                                             */
/* Check if pT faces roughly the same direction as pEm (+/- 0x400)    */
/* ------------------------------------------------------------------ */
/* DS 0x80015910 (Cdir_ck) */
UCHAR Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
	return ((((int)pT->Cdir_y - (int)pEm->Cdir_y) + 0x400) & 0xfff) < 0x800;
}

/* ------------------------------------------------------------------ */
/* Cdir_ck2                                                            */
/* Check if pT faces toward pEm's position (+/- 0x400)               */
/* ------------------------------------------------------------------ */
/* DS 0x80015930 (Cdir_ck2) */
UCHAR Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
	short Dir;

	Dir = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)pT->Pos_x, (short)pT->Pos_z);
	return ((((int)pT->Cdir_y - (int)Dir) + 0x400) & 0xfff) < 0x800;
}

/* ------------------------------------------------------------------ */
/* Gacha_ch                                                            */
/* Return nonzero if any action button was just pressed               */
/* ------------------------------------------------------------------ */
/* DS 0x8001598c (Gacha_ch) */
ULONG Gacha_ch(void)
{
	return (UINT)((G.Key_trg & 0x34f) != 0);
}

/* ------------------------------------------------------------------ */
/* Sikai_ck                                                            */
/* Line-of-sight check: can pEm see pTa from position pAi?            */
/* ------------------------------------------------------------------ */
/* DS 0x800159a4 (Sikai_ck) */
ULONG Sikai_ck(ENEMY_WORK *pEm, VECTOR *pTa, long Han, VECTOR *pAi)
{
	LONG Dir;
	ULONG x;
	ULONG z;
	ULONG L;
	SVECTOR Sv;
	MATRIX M;
	MATRIX M1;
	SVECTOR Sck0;
	SVECTOR Sck1;
	SVECTOR Tp0;
	SVECTOR Tp1;
	SVECTOR Tp2;

	Dir = (short)Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)pAi->vx, (short)pAi->vz);
	x = pEm->Pos_x - pAi->vx;
	z = pEm->Pos_z - pAi->vz;
	L = SquareRoot0(x * x + z * z);

	/* rotation for the sight direction; Sck0 = (L,0,0) rotated, Sck1 = (0,0,Han) rotated */
	*(LONG *)&Sv = Dir << 16;
	Sv.vz = 0;
	RotMatrix(&Sv, &M);
	*(LONG *)&Sv = L;
	ApplyMatrixSV(&M, &Sv, &Sck0);
	*(LONG *)&Sv = 0;
	Sv.vz = Han;
	ApplyMatrixSV(&M, &Sv, &Sck1);

	*(LONG *)&Tp0 = pTa->vx - (pEm->Pos_x + Sck1.vx);
	Tp0.vz = pTa->vz - (Sck1.vz + pEm->Pos_z);
	*(LONG *)&Tp1 = pTa->vx - (pEm->Pos_x - Sck1.vx);
	Tp1.vz = pTa->vz - (pEm->Pos_z - Sck1.vz);
	*(LONG *)&Tp2 = pTa->vx - (Tp0.vx + Sck0.vx);
	Tp2.vz = pTa->vz - (Tp0.vz + Sck0.vz);

	*(LONG *)&M.m[0][0] = Sck0.vx;
	M.m[0][2] = Sck0.vz;
	M.m[1][0] = Sck1.vx;
	*(LONG *)&M.m[1][1] = (USHORT)Sck1.vz << 16;

	*(LONG *)&M1.m[1][1] = 0;
	M1.m[0][0] = Tp0.vx;
	M1.m[0][1] = Tp2.vx;
	*(LONG *)&M1.m[0][2] = Tp1.vx;
	M1.m[2][0] = Tp0.vz;
	M1.m[2][1] = Tp2.vz;
	M1.m[2][2] = Tp1.vz;

	MulMatrix0(&M, &M1, &M);
	if ((M.m[0][0] & 0x8000) != (M.m[0][1] & 0x8000))
	{
		return (M.m[1][0] & 0x8000) != (M.m[1][2] & 0x8000);
	}
	return 0;
}

/* ------------------------------------------------------------------ */
/* Set_base_pos                                                        */
/* Record enemy's base position from null-frame bone data             */
/* ------------------------------------------------------------------ */
/* DS 0x80015b94 (Set_base_pos) */
void Set_base_pos(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
	MATRIX m;
	SVECTOR sv;
	SVECTOR *pSv;
	MATRIX *pM;
	ENEMY_WORK *pPm;

	pPm = G.pEm;
	pSv = &sv;
	pM = &m;
	get_null_pos(pSv, R_flg, Kan, Seq);
	m = GsIDMATRIX;
	RotMatrixY((int)pPm->Cdir_y, pM);
	ApplyMatrixSV(pM, pSv, pSv);
	pPm->Base_pos_x = (short)pPm->Pos_x - pSv->vx;
	pPm->Base_pos_y = (short)pPm->Pos_y - pSv->vy;
	pPm->Base_pos_z = (short)pPm->Pos_z - pSv->vz;
	pEm->Base_pos_x = pPm->Base_pos_x;
	pEm->Base_pos_y = pPm->Base_pos_y;
	pEm->Base_pos_z = pPm->Base_pos_z;
}

/* ------------------------------------------------------------------ */
/* Null_pos_set                                                        */
/* Update enemy position from base pos + rotated bone offset          */
/* ------------------------------------------------------------------ */
/* DS 0x80015cb8 (Null_pos_set) */
void Null_pos_set(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
	MATRIX m;
	SVECTOR sv;
	SVECTOR *pSv;
	MATRIX *pM;

	pSv = &sv;
	pM = &m;
	get_null_pos(pSv, R_flg, Kan, Seq);
	m = GsIDMATRIX;
	RotMatrixY((int)pEm->Cdir_y, pM);
	ApplyMatrixSV(pM, pSv, pSv);
	pEm->Pos_x = (int)pEm->Base_pos_x + (int)sv.vx;
	pEm->Pos_y = (int)pEm->Base_pos_y + (int)sv.vy;
	pEm->Pos_z = (int)pEm->Base_pos_z + (int)sv.vz;
}

/* ------------------------------------------------------------------ */
/* get_null_pos                                                        */
/* Read the null-frame bone position from the animation data          */
/* ------------------------------------------------------------------ */
/* DS 0x80015db0 (get_null_pos) */
void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq)
{
	ENEMY_WORK *pEm;
	UINT no;
	UINT f_num;
	UINT Rec_hi;
	int Bone_adr;
	UINT *pSeq_ptr;

	pEm = G.pEm;
	pSeq_ptr = (UINT *)(Seq + (UINT)pEm->Move_no * 4);
	no = *pSeq_ptr;
	if (R_flg != 0)
	{
		f_num = ((no & 0xffff) - (UINT)pEm->Move_cnt) - 1;
	}
	else
	{
		f_num = (UINT)pEm->Move_cnt;
	}
	pSeq_ptr = (UINT *)(Seq + (no >> 0x10) + f_num * 4);
	Bone_adr = Kan + ((KAN_HEAD *)Kan)->Bone_ofs;
	Rec_hi = ((KAN_HEAD *)Kan)->Rec_hi;
	no = *pSeq_ptr;
	while ((no & 0x8000) != 0)
	{
		pSeq_ptr++;
		no = *pSeq_ptr;
	}
	Bone_adr += (Rec_hi >> 2) * (*pSeq_ptr & 0xfff) * 4;
	pSv->vx = ((KAN_BONE_REC *)Bone_adr)->Null_pos.vx;
	pSv->vy = (short)*(int *)&((KAN_BONE_REC *)Bone_adr)->Null_pos.vy;
	pSv->vz = ((KAN_BONE_REC *)Bone_adr)->Null_pos.vz;
}

/* ------------------------------------------------------------------ */
/* Spd_get                                                             */
/* Read speed components for current move frame from animation data   */
/* ------------------------------------------------------------------ */
/* DS 0x80015e7c (Spd_get) */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
	UINT Seq_w;
	UINT Fr_no;
	UINT Rec_sz;
	int Bone_adr;
	short Spd_x;
	short Spd_y;
	short Spd_z;

	Seq_w = *(UINT *)(Seq + (UINT)pEm->Move_no * 4);
	if (R_flg == 0)
	{
		Fr_no = (UINT)pEm->Move_cnt;
	}
	else
	{
		Fr_no = ((Seq_w & 0xffff) - (UINT)pEm->Move_cnt) - 1;
	}
	Rec_sz = *(UINT *)&((KAN_HEAD *)Kan)->Parts_cnt >> 0x12;
	Bone_adr = Kan + ((KAN_HEAD *)Kan)->Bone_ofs +
			Rec_sz * (*(UINT *)(Seq + (Seq_w >> 0x10) + Fr_no * 4) & 0xfff) * 4;
	Spd_x = ((KAN_BONE_REC *)Bone_adr)->Null_pos.vx;
	Spd_y = (short)*(int *)&((KAN_BONE_REC *)Bone_adr)->Null_pos.vy;
	Spd_z = ((KAN_BONE_REC *)Bone_adr)->Null_pos.vz;
	if (pEm->Move_cnt == 0)
	{
		pEm->Spd_base_x = 0;
		pEm->Spd_base_y = 0;
		pEm->Spd_base_z = 0;
		if (R_flg != 0)
		{
			pEm->Spd_base_x = Spd_x;
			pEm->Spd_base_y = Spd_y;
			pEm->Spd_base_z = Spd_z;
			Bone_adr = Kan + ((KAN_HEAD *)Kan)->Bone_ofs +
					Rec_sz * (*(UINT *)(Seq + *(USHORT *)(Seq + (UINT)pEm->Move_no * 4 + 2)) & 0xfff) * 4;
			pEm->Spd_base_x = pEm->Spd_base_x + ((KAN_BONE_REC *)Bone_adr)->Null_pos.vx;
			pEm->Spd_base_y = pEm->Spd_base_y + (short)*(int *)&((KAN_BONE_REC *)Bone_adr)->Null_pos.vy;
			pEm->Spd_base_z = pEm->Spd_base_z + ((KAN_BONE_REC *)Bone_adr)->Null_pos.vz;
		}
	}
	pEm->Spd_x = Spd_x - pEm->Spd_base_x;
	pEm->Spd_y = Spd_y - pEm->Spd_base_y;
	pEm->Spd_z = Spd_z - pEm->Spd_base_z;
	pEm->Spd_base_x = Spd_x;
	pEm->Spd_base_y = Spd_y;
	pEm->Spd_base_z = Spd_z;
}

/* ------------------------------------------------------------------ */
/* Rnd                                                                 */
/* Simple 8-bit LFSR random number                                     */
/* ------------------------------------------------------------------ */
/* DS 0x80015fe8 (Rnd) */
UCHAR Rnd(void)
{
	/* retail reserves 8 more bytes of locals (Rnd frame 8); never read */
	volatile int Rsv[2];
	UINT Rnd_sum;
	UINT Seed_sh;

	Seed_sh = G.Random >> 7 & 0xff;
	Rnd_sum = ((Seed_sh + G.Random) & 0xff) + (Seed_sh << 8);
	G.Random = (USHORT)Rnd_sum;
	return Rnd_sum;
}

/* ------------------------------------------------------------------ */
/* Em_seq_sound                                                        */
/* Trigger enemy sequence sound if flagged in current seq word        */
/* ------------------------------------------------------------------ */
/* DS 0x80016028 (Em_seq_sound) */
void Em_seq_sound(ENEMY_WORK *pEm)
{

	if ((*pEm->pNow_seq & 0x8000000) != 0)
	{
		Snd_se_enem((UCHAR)(*pEm->pNow_seq >> 0x1c), pEm);
	}
}

/* ------------------------------------------------------------------ */
/* move_kuchi                                                          */
/* Mouth/expression texture animation helper                          */
/* ------------------------------------------------------------------ */
#if 0	/* move_kuchi: beta-only, not in the DS dump; an older header declared a conflicting INT move_kuchi(VOID) */
void move_kuchi(ENEMY_WORK *pEm)
{
	/* function body not recovered in retail binary (may be inlined) */
}
#endif

/* ------------------------------------------------------------------ */
/* Tex_Ani_move                                                        */
/* Advance texture animation state machine                            */
/* ------------------------------------------------------------------ */
/* DS 0x80016068 (Tex_Ani_move) */
void Tex_Ani_move(TEX_MOVE_DATA *T_data, TEX_MOVE_SEQ *T_seq, DR_MOVE *Prim, UCHAR *Set_timer, UCHAR *Set_seq_no)
{
	UCHAR Timer_val;
	TEX_MOVE_DATA *Td;
	UINT Tpage;
	UINT Off_v;
	RECT Rect;

	Timer_val = *Set_timer;
	if (Timer_val == 0)
	{
		Timer_val = *Set_seq_no;
		*Set_seq_no = Timer_val + 1;
		if (T_seq[(UCHAR)(Timer_val + 1)].Tbl_no == 0xff)
		{
			*Set_seq_no = 0;
		}
		*Set_timer = T_seq[*Set_seq_no].Timer;
		Tpage = (UINT)(G.pEm)->Tpage;
		Td = T_data + T_seq[*Set_seq_no].Tbl_no;
		Off_v = 0;
		if (0xf < Tpage)
		{
			Tpage -= 0x10;
			Off_v = 0x100;
		}
		Rect.x = (USHORT)Td->Mo_u + (short)(Tpage * 0x40) + (USHORT)Td->Moto_tpage * 0x40;
		Rect.y = (USHORT)Td->Mo_v + (short)Off_v;
		Rect.w = (short)Td->Mo_w;
		Rect.h = (short)Td->Mo_h;
		SetDrawMove((DR_MOVE *)Prim, &Rect, Tpage * 0x40 + (UINT)Td->Saki_tpage * 0x40 + (UINT)Td->Sa_u, Td->Sa_v | Off_v);
		Prim->tag = (Prim->tag & 0xff000000) | (G.Ot2[G.Cbuf_id][0] & 0xffffff);
		G.Ot2[G.Cbuf_id][0] = (G.Ot2[G.Cbuf_id][0] & 0xff000000) | ((UINT)Prim & 0xffffff);
		Timer_val = *Set_timer;
	}
	*Set_timer = Timer_val + 0xff;
}

/* ------------------------------------------------------------------ */
/* Foot_set_pl                                                         */
/* Set position or speed from foot bone world position                */
/* ------------------------------------------------------------------ */
/* DS 0x80016200 (Foot_set_pl) */
void Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no)
{
	PARTS_W *pP_ptr;
	MATRIX Mat;
	long x;
	long z;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &Mat);
	CompM(&Mat, (MATRIX *)pP_ptr[1].M, &Mat);
	CompM(&Mat, (MATRIX *)pP_ptr[Foot_no * 3 + 2].M, &Mat);
	CompM(&Mat, (MATRIX *)pP_ptr[Foot_no * 3 + 3].M, &Mat);
	CompM(&Mat, (MATRIX *)pP_ptr[Foot_no * 3 + 4].M, &Mat);
	if (Mode == 0)
	{
		pEm->Pos_x = pEm->Pos_x - (Mat.t[0] - pP_ptr[Foot_no * 3 + 4].Workm.t[0]);
		pEm->Pos_z = pEm->Pos_z - (Mat.t[2] - pP_ptr[Foot_no * 3 + 4].Workm.t[2]);
	}
	else
	{
		x = Mat.t[0] - pP_ptr[Foot_no * 3 + 4].Workm.t[0];
		z = Mat.t[2] - pP_ptr[Foot_no * 3 + 4].Workm.t[2];
		pEm->Spd_x = (short)SquareRoot0(x * x + z * z);
	}
}

/* ------------------------------------------------------------------ */
/* Kage_work_init                                                      */
/* Initialize shadow work array (POLY_FT4 x 50)                      */
/* ------------------------------------------------------------------ */
/* DS 0x8001633c (Kage_work_init) */
void Kage_work_init(void)
{
	KAGE_WORK	*pK;
	UCHAR		*pTpage;
	ULONG		i;
	POLY_FT4	*p;
	USHORT		Vram_id;

	/* One KAGE_WORK per shadow: Prim[0] is the template, Prim[1] gets a copy */
	pK = G.Kage;
	i = 0x32;
	/* DS keeps the address of Player.Tpage in one register: Tpage at [0], Clut at [1] */
	pTpage = &G.Player.Tpage;
	do
	{
		p = &pK->Prim[0];
		pK->Be_flg = 0;
		*(ULONG *)&p->r0 = 0x808080;		/* r0,g0,b0 */
		SetPolyFT4(p);
		SetSemiTrans(p, 1);
		Vram_id = GetTPage(1, 2, (pTpage[0] - 0xe) * 0x40, 0x100);
		p->tpage = Vram_id;
		Vram_id = GetClut(0, pTpage[1] + 0x1e2);
		p->clut = Vram_id;
		p->u0 = 0x62;  p->v0 = 0xe1;			/* u0,v0 */
		p->u1 = 0x7e;  p->v1 = 0xe1;			/* u1,v1 */
		p->u2 = 0x62;  p->v2 = 0xff;			/* u2,v2 */
		p->u3 = 0x7e;  p->v3 = 0xff;			/* u3,v3 */
		/* DS addresses Prim[1] from pK, not from a second running pointer */
		Memcpy(&pK->Prim[1], p, 0x28);
		pK++;
		i--;
	} while (i != 0);
}

/* ------------------------------------------------------------------ */
/* Kage_work9_init                                                   */
/* Initialize shadow work9 array (5 entries)                          */
/* ------------------------------------------------------------------ */
/* DS 0x8001645c (Kage_work9_init) */
void Kage_work9_init(void)
{
	KAGE_WORK9	*pK;
	ULONG		i;

	pK = G.Kage9;
	i = 5;
	do
	{
		pK->Be_flg = 0;
		i--;
		pK++;
	} while (i != 0);
}

/* ------------------------------------------------------------------ */
/* Kage_work_set                                                       */
/* Allocate and configure a shadow work entry                         */
/* ------------------------------------------------------------------ */
/* DS 0x80016480 (Kage_work_set) */
void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
{
	KAGE_WORK *pKw;
	int Left_cnt;

	/* first free slot: scan up to 50 entries, Left_cnt reaches 0 when all are in use */
	pKw = G.Kage;
	Left_cnt = 0x32;
	while (pKw->Be_flg != 0)
	{
		Left_cnt--;
		if (Left_cnt == 0)
		{
			break;
		}
		pKw++;
	}
	if (Left_cnt == 0)
	{
		*pK = 0;
	}
	else
	{
		*pK = (ULONG)pKw;
		pKw->Be_flg = 5;
		pKw->pPos = pPos;
		if (Color != 0)
		{
			*(UINT *)&pKw->Prim[0].r0 = (*(UINT *)&pKw->Prim[0].r0 & 0xff000000) | Color;
			*(UINT *)&pKw->Prim[1].r0 = (*(UINT *)&pKw->Prim[1].r0 & 0xff000000) | Color;
		}
		else
		{
			*(UINT *)&pKw->Prim[0].r0 = (*(UINT *)&pKw->Prim[0].r0 & 0xff000000) | 0x808080;
			*(UINT *)&pKw->Prim[1].r0 = (*(UINT *)&pKw->Prim[1].r0 & 0xff000000) | 0x808080;
		}
		/* Off_x and Off_z, Half_x and Half_z are each written as one 32-bit word */
		*(ULONG *)&pKw->Off_x = Off;
		*(ULONG *)&pKw->Half_x = Half;
	}
}

/* DS unrolls the 9x8 (u,v) table into immediate byte stores: each KAGE_WORK
   element receives the same 4 (u,v) pairs on Prim[0] and on Prim[1]. */
#define KAGE_UV_PRIM(w, p, A, B, C, D, E, F, G, H) (w).Prim[p].u0 = (A); (w).Prim[p].v0 = (B); (w).Prim[p].u1 = (C); (w).Prim[p].v1 = (D); (w).Prim[p].u2 = (E); (w).Prim[p].v2 = (F); (w).Prim[p].u3 = (G); (w).Prim[p].v3 = (H)
#define KAGE_UV(w, A, B, C, D, E, F, G, H) KAGE_UV_PRIM(w, 0, A, B, C, D, E, F, G, H); KAGE_UV_PRIM(w, 1, A, B, C, D, E, F, G, H)

/* ------------------------------------------------------------------ */
/* Kage_work9_set                                                      */
/* Allocate and configure a shadow work9 entry                        */
/* ------------------------------------------------------------------ */
/* DS 0x8001653c (Kage_work9_set) */
void Kage_work9_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
{
	KAGE_WORK9 *pK9;
	KAGE_WORK *pKw;
	int i;

	/* DS walks pointers with a down-counter (5 and 41 steps) in both searches and keeps
	   walking pKw through the 9-element loop, then steps it back by 9 for the (u,v) stores */
	/* first free KAGE_WORK9 entry (0..4) */
	pK9 = G.Kage9;
	for (i = 5; i != 0; i--)
	{
		if (pK9->Be_flg == 0)
		{
			break;
		}
		pK9++;
	}
	if (i == 0)
	{
		return;
	}
	pK9->Be_flg = 7;

	/* first free KAGE_WORK entry (0..40); on failure release the KAGE_WORK9 entry */
	pKw = G.Kage;
	for (i = 41; i != 0; i--)
	{
		if (pKw->Be_flg == 0)
		{
			break;
		}
		pKw++;
	}
	if (i == 0)
	{
		pK9->Be_flg = 0;
		*pK = 0;
		return;
	}
	*pK = (ULONG)pK9;
	pK9->pKw_top = pKw;
	pK9->Off_x  = (short)Off;
	pK9->Off_z  = (short)(Off >> 16);
	pK9->Half_x = (short)Half;
	pK9->Half_z = (short)(Half >> 16);
	pK9->pPos = pPos;

	/* the 9 consecutive KAGE_WORK elements that make up the shadow */
	i = 9;
	do
	{
		pKw->Be_flg = 2;
		if (Color != 0)
		{
			*(UINT *)&pKw->Prim[0].r0 = (*(UINT *)&pKw->Prim[0].r0 & 0xff000000) | Color;
			*(UINT *)&pKw->Prim[1].r0 = (*(UINT *)&pKw->Prim[1].r0 & 0xff000000) | Color;
		}
		pKw++;
	}
	while (--i != 0);

	pKw -= 9;	/* back to the first of the 9 elements (DS: t0 += 9*0x68 in the loop, then -= 9*0x68) */

	KAGE_UV(*pKw, 0x62,0xe1, 0x6b,0xe1, 0x62,0xeb, 0x6b,0xeb);
	pKw++;
	KAGE_UV(*pKw, 0x6b,0xe1, 0x74,0xe1, 0x6b,0xeb, 0x74,0xeb);
	pKw++;
	KAGE_UV(*pKw, 0x74,0xe1, 0x7e,0xe1, 0x74,0xeb, 0x7e,0xeb);
	pKw++;
	KAGE_UV(*pKw, 0x62,0xeb, 0x6b,0xeb, 0x62,0xf5, 0x6b,0xf5);
	pKw++;
	KAGE_UV(*pKw, 0x6b,0xeb, 0x74,0xeb, 0x6b,0xf5, 0x74,0xf5);
	pKw++;
	KAGE_UV(*pKw, 0x74,0xeb, 0x7e,0xeb, 0x74,0xf5, 0x7e,0xf5);
	pKw++;
	KAGE_UV(*pKw, 0x62,0xf5, 0x6b,0xf5, 0x62,0xff, 0x6b,0xff);
	pKw++;
	KAGE_UV(*pKw, 0x6b,0xf5, 0x74,0xf5, 0x6b,0xff, 0x74,0xff);
	pKw++;
	KAGE_UV(*pKw, 0x74,0xf5, 0x7e,0xf5, 0x74,0xff, 0x7e,0xff);
	pKw++;
}

/* ------------------------------------------------------------------ */
/* MulKage                                                             */
/* Update shadow position/flags for a KAGE_WORK entry                */
/* ------------------------------------------------------------------ */
/* DS 0x800168b4 (MulKage) */
void MulKage(KAGE_WORK *pKw, short Grand, short Dir_y, ULONG Flg)
{
	MATRIX Rot_m;
	SVECTOR Off_sv;
	SVECTOR Rot_sv;

	if ((pKw != (KAGE_WORK *)0x0) && (pKw->Be_flg != 0))
	{
		/* identity matrix: DS stores it as five 32-bit words (m[0][0..1], m[0][2]..m[1][0], ...) */
		*(int *)&Rot_m.m[0][0] = 0x1000;
		*(int *)&Rot_m.m[0][2] = 0;
		*(int *)&Rot_m.m[1][1] = 0x1000;
		*(int *)&Rot_m.m[2][0] = 0;
		*(int *)&Rot_m.m[2][2] = 0x1000;
		RotMatrixY((int)Dir_y, &Rot_m);
		/* DS stores Off_x as a 32-bit word (vx = Off_x, vy = its sign) */
		*(int *)&Off_sv = pKw->Off_x;
		Off_sv.vz = pKw->Off_z;
		ApplyMatrixSV(&Rot_m, &Off_sv, &Rot_sv);
		pKw->Dir_y = Dir_y;
		pKw->Pos_y = Grand;
		pKw->Pos_x = Rot_sv.vx + (short)pKw->pPos->vx;
		pKw->Pos_z = Rot_sv.vz + (short)pKw->pPos->vz;
		pKw->Be_flg |= 4;
		if (Flg != 0)
		{
			pKw->Be_flg &= 0xfb;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Kage_work_sort                                                      */
/* Sort/render all active KAGE_WORK shadow entries                    */
/* ------------------------------------------------------------------ */
/* DS 0x8001699c (Kage_work_sort) */
void Kage_work_sort(void)
{
	KAGE_WORK *pKw;
	UINT *pTag;
	UINT *pOt;
	UINT *Ot_base;
	UINT Adr_mask;
	LONG Unit;
	MATRIX Mx;
	MATRIX M2;
	SVECTOR Sv[4];
	VECTOR Pos;
	ULONG Otz;
	int i;
	LONG Hx;

	pKw = G.Kage;
	Unit = 0x1000;
	Adr_mask = 0xffffff;
	Ot_base = (UINT *)G.Ot1[G.Cbuf_id];
	for (i = 50; i != 0; i--, pKw++)
	{
		if ((pKw->Be_flg & 4) == 0)
		{
			continue;
		}
		if ((pKw->Be_flg & 8) == 0)
		{
			/* local matrix: RotY(Dir_y) with the shadow position as translation */
			*(int *)&Mx.m[0][0] = Unit;
			*(int *)&Mx.m[0][2] = 0;
			*(int *)&Mx.m[1][1] = Unit;
			*(int *)&Mx.m[2][0] = 0;
			*(int *)&Mx.m[2][2] = Unit;
			RotMatrixY((int)pKw->Dir_y, &Mx);
			Mx.t[0] = pKw->Pos_x;
			Mx.t[1] = pKw->Pos_y;
			Mx.t[2] = pKw->Pos_z;
			CompM(&GsWSMATRIX, &Mx, &M2);
			SetRotMatrix(&M2);
			SetTransMatrix(&M2);

			/* quad corners; DS stores the x corners as 32-bit words (vy = sign of vx) */
			Hx = pKw->Half_x;
			Sv[0].vz = pKw->Half_z;
			Sv[1].vz = pKw->Half_z;
			*(long *)&Sv[0] = ~Hx - 1;
			*(long *)&Sv[1] = Hx;
			*(long *)&Sv[2] = ~Hx - 1;
			*(long *)&Sv[3] = Hx;
			Sv[2].vz = ~pKw->Half_z - 1;
			Sv[3].vz = ~pKw->Half_z - 1;

			Pos = *(VECTOR *)Mx.t;
			pTag = (UINT *)&pKw->Prim[G.Cbuf_id];
			if (Hit_ck_point4(&Pos, G.Cccut) != 0)
			{
				/* project the 4 corners into this shadow's primitive */
				gte_ldv3(&Sv[0], &Sv[1], &Sv[2]);
				gte_rtpt_b();
				gte_stsxy3((long *)&((POLY_FT4 *)pTag)->x0, (long *)&((POLY_FT4 *)pTag)->x1, (long *)&((POLY_FT4 *)pTag)->x2);
				gte_ldv0(&Sv[3]);
				gte_rtps();
				gte_stsxy((long *)&((POLY_FT4 *)pTag)->x3);
				gte_avsz4();
				gte_stotz((long *)&Otz);
				if (Otz > 0x1fff)
				{
					Otz = 0x1fff;
				}

				/* link the primitive into ordering table of the current buffer (G.Ot1[Cbuf_id]) */
				pOt = Ot_base + (Otz >> 3);
				*pTag = (*pTag & 0xff000000) | (*pOt & Adr_mask);
				*pOt  = (*pOt & 0xff000000) | ((UINT)pTag & Adr_mask);
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Kage_work9_sort                                                     */
/* Sort/render all active KAGE_WORK9 shadow entries                   */
/* ------------------------------------------------------------------ */
/* DS 0x80016c14 (Kage_work9_sort) */
void Kage_work9_sort(void)
{
	USHORT Hx;
	short Gx0;
	UINT Otz_p;
	UINT Ot_idx;
	UINT *pOt;
	short Gx2;
	short Gx1;
	int *pK9_top;
	int Cell_cnt;
	short Grid_stp;
	short Gz1;	short Bot_z;	/* bottom edge Z (~Hz - 1), restored in case 3 */
	MATRIX Cell_lm;
	MATRIX Cell_wm;
	SVECTOR Qv_lt;
	SVECTOR Qv_rt;
	SVECTOR Qv_lb;
	SVECTOR Qv_rb;
	VECTOR Cell_pos;
	long Avg_out[2];
	UINT *Ot_base;
	int Kg9_cnt;
	UINT Adr_mask;
	int Kw_cur;
	short Hz;

	Kg9_cnt = 5;
	pK9_top = (int *)&G.Kage9[0].pKw_top;
	Adr_mask = 0xffffff;
	Ot_base = (UINT *)G.Ot1[G.Cbuf_id];
	do
	{
		/* KAGE_WORK9 Be_flg (element +0x0e = pK9_top - 0x0a bytes), bit 2 = active */
		if ((((UCHAR *)pK9_top)[-0xa] & 0x4) != 0)
		{
			*(int *)&Cell_lm.m[0][0] = 0x1000;
			*(int *)&Cell_lm.m[0][2] = 0;
			*(int *)&Cell_lm.m[1][1] = 0x1000;
			*(int *)&Cell_lm.m[2][0] = 0;
			*(int *)&Cell_lm.m[2][2] = 0x1000;
			RotMatrixY((int)(short)pK9_top[-2], (MATRIX *)&Cell_lm);
			Cell_lm.t[0] = *(short *)(pK9_top + -4);
			Cell_lm.t[1] = *(short *)((int)pK9_top + -0xe);
			Cell_lm.t[2] = *(short *)(pK9_top + -3);
			CompM(&GsWSMATRIX, &Cell_lm, &Cell_wm);
			SetRotMatrix((MATRIX *)&Cell_wm);
			SetTransMatrix((MATRIX *)&Cell_wm);
			Hx = *(USHORT *)(pK9_top + -5);
			SVEC_XY32(Qv_lt, (USHORT)~Hx - 1);
			Gx0 = Qv_lt.vx;
			SVEC_XY32(Qv_rt, (SHORT)Hx);
			Qv_lt.vz = *(short *)((int)pK9_top + -0x12);
			Hz = (int)Qv_lt.vz;
			Qv_rt.vz = *(short *)((int)pK9_top + -0x12);
			Qv_lb.vz = ~*(short *)((int)pK9_top + -0x12) - 1;
			Cell_pos.vx = Cell_lm.t[0];
			Cell_pos.vy = Cell_lm.t[1];
			Cell_pos.vz = Cell_lm.t[2];
			Cell_pos.pad = (USHORT)Cell_wm.m[0][0] | ((LONG)(USHORT)Cell_wm.m[0][1] << 16);
			Grid_stp = (short)(((int)(short)Hx - (int)Qv_lt.vx) / 3);
			Gx1 = Grid_stp + Qv_lt.vx;
			Kw_cur = *pK9_top;
			Gx2 = Grid_stp * 2 + Qv_lt.vx;
			Grid_stp = (short)(((int)Qv_lb.vz - Hz) / 3);
			Gz1 = Grid_stp + Qv_lt.vz;
			Grid_stp = Grid_stp * 2 + Qv_lt.vz;
			Bot_z = Qv_lb.vz; Qv_lb.vx = Qv_lt.vx;
			Qv_lb.vy = Qv_lt.vy;
			Qv_rb.vx = Qv_rt.vx;
			Qv_rb.vy = Qv_rt.vy;
			Qv_rb.vz = Qv_lb.vz;
			if (Hit_ck_point4(&Cell_pos, G.Cccut) != 0)
			{
				Cell_cnt = 9;
				do
				{
					switch (Cell_cnt)
					{
					case 1: case 4: case 7:
						Qv_lt.vx = Gx2;  Qv_rt.vx = Hx;
						Qv_rb.vx = Hx;  Qv_lb.vx = Gx2;
						break;
					case 2: case 5: case 8:
						Qv_lt.vx = Gx1; Qv_rt.vx = Gx2;
						Qv_rb.vx = Gx2;  Qv_lb.vx = Gx1;
						break;
					case 3:
						Qv_lt.vx = Gx0;
						Qv_rt.vx = Gx1; Qv_rb.vx = Gx1;
						Qv_lb.vx = Gx0;
						Qv_lt.vz = Grid_stp; Qv_rt.vz = Grid_stp; Qv_lb.vz = Bot_z;
						Qv_rb.vz = Qv_lb.vz;
						break;
					case 6:
						Qv_lt.vx = Gx0;
						Qv_rt.vx = Gx1; Qv_rb.vx = Gx1;
						Qv_lb.vx = Gx0;
						Qv_lt.vz = Gz1; Qv_rt.vz = Gz1;
						Qv_lb.vz = Grid_stp; Qv_rb.vz = Grid_stp;
						break;
					case 9:
						Qv_lt.vx = Gx0;
						Qv_rt.vx = Gx1; Qv_rb.vx = Gx1;
						Qv_lb.vx = Gx0;
						Qv_lt.vz = (short)Hz; Qv_rt.vz = (short)Hz;
						Qv_lb.vz = Gz1; Qv_rb.vz = Gz1;
					}
					/* Tag address is recomputed from Kw_cur instead of being held in a local.
					   Kw_cur is only advanced at the end of the body, so every use below
					   sees the same address the old pTag local held. */
					Otz_p = RotAverage4(&Qv_lt, &Qv_rt, &Qv_lb, &Qv_rb, (long *)&((POLY_FT4 *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18))->x0, (long *)&((POLY_FT4 *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18))->x1, (long *)&((POLY_FT4 *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18))->x2, (long *)&((POLY_FT4 *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18))->x3, Avg_out, Avg_out);
					if (0x1f < Otz_p) Otz_p = Otz_p - 0x20;
					Ot_idx = Otz_p >> 3;
					if (0x1fff < Otz_p) Ot_idx = 0x3ff;
					pOt = Ot_base + Ot_idx;
					*(UINT *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18) = (*(UINT *)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18) & 0xff000000) | (*pOt & Adr_mask);
					Cell_cnt = Cell_cnt + -1;
					*pOt = (*pOt & 0xff000000) | ((UINT)(Kw_cur + (UINT)G.Cbuf_id * 0x28 + 0x18) & Adr_mask);
					Kw_cur += 0x68;
				} while (Cell_cnt != 0);
			}
		}
		pK9_top += 7;
		Kg9_cnt = Kg9_cnt + -1;
	} while (Kg9_cnt != 0);
}

/* ------------------------------------------------------------------ */
/* Kage_work_color_set                                                 */
/* Set color in both POLY_FT4 primitives of a KAGE_WORK               */
/* ------------------------------------------------------------------ */
/* DS 0x80016fe4 (Kage_work_color_set) */
void Kage_work_color_set(ULONG *pK, ULONG Color)
{
	*(UINT *)&((KAGE_WORK *)pK)->Prim[0].r0 = (*(UINT *)&((KAGE_WORK *)pK)->Prim[0].r0 & 0xff000000) | Color;
	*(UINT *)&((KAGE_WORK *)pK)->Prim[1].r0 = (*(UINT *)&((KAGE_WORK *)pK)->Prim[1].r0 & 0xff000000) | Color;
}

/* ------------------------------------------------------------------ */
/* Kage_work9_color_set                                                */
/* Set color in both POLY_FT4 primitives of each of the 9 KAGE_WORK    */
/* ------------------------------------------------------------------ */
/* DS 0x8001700c (Kage_work9_color_set) */
void Kage_work9_color_set(ULONG *pK, ULONG Color)
{
	KAGE_WORK *pPrim;
	char cnt;

	cnt = 9;
	pPrim = (KAGE_WORK *)pK[6];
	do
	{
		cnt = cnt + -1;
		*(UINT *)&pPrim->Prim[0].r0 = (*(UINT *)&pPrim->Prim[0].r0 & 0xff000000) | Color;
		*(UINT *)&pPrim->Prim[1].r0 = (*(UINT *)&pPrim->Prim[1].r0 & 0xff000000) | Color;
		pPrim++;
	} while (cnt != 0);
}

/* ------------------------------------------------------------------ */
/* Texture_Move                                                        */
/* Slide UV coordinates for all GT3/GT4 primitives in a model         */
/* ------------------------------------------------------------------ */
/* DS 0x80017054 (Texture_Move) */
void Texture_Move(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus)
{
	POLY_GT3 *pGt3;
	POLY_GT4 *pGt4;
	UINT Poly_cnt;
	UINT Prev_cnt;
	UINT Poly_n;

	pGt3 = (POLY_GT3 *)pMw->pPacket;
	pGt4 = (POLY_GT4 *)pMw->pPacket2;
	Poly_cnt = pMw->pTmd[5] * 2;
	Poly_n = Poly_cnt;
	Poly_cnt--;
	if (Poly_n != 0)
	{
		do
		{
			pGt3->u0 += mvmu;
			pGt3->v0 += mvmu >> 16;
			pGt3->u1 += mvmu;
			pGt3->v1 += mvmu >> 16;
			pGt3->u2 += mvmu;
			pGt3->v2 += mvmu >> 16;
			pGt3->tpage = (pGt3->tpage & 0xffe0) | (tex_page & 0x1f);
			pGt3->clut = (pGt3->clut & 0x803f) | clut_plus << 6;
			pGt3++;
			Prev_cnt = Poly_cnt;
			Poly_cnt--;
		} while (Prev_cnt != 0);
	}
	Poly_cnt = pMw->pTmd2[5] * 2;
	Poly_n = Poly_cnt;
	Poly_cnt--;
	if (Poly_n != 0)
	{
		do
		{
			pGt4->u0 += mvmu;
			pGt4->v0 += mvmu >> 16;
			pGt4->u1 += mvmu;
			pGt4->v1 += mvmu >> 16;
			pGt4->u2 += mvmu;
			pGt4->v2 += mvmu >> 16;
			pGt4->u3 += mvmu;
			pGt4->v3 += mvmu >> 16;
			pGt4->tpage = (pGt4->tpage & 0xffe0) | (tex_page & 0x1f);
			pGt4->clut = (pGt4->clut & 0x803f) | clut_plus << 6;
			pGt4++;
			Prev_cnt = Poly_cnt;
			Poly_cnt--;
		} while (Prev_cnt != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Texture_Move_GT3                                                    */
/* Slide UV coords for selected GT3 polys (by index list pPoly_no)   */
/* ------------------------------------------------------------------ */
/* DS 0x800171d0 (Texture_Move_GT3) */
void Texture_Move_GT3(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus, UCHAR *pPoly_no)
{
	POLY_GT3 *gt3;
	ULONG *pPkt;
	UINT Poly_cnt;
	UINT Prev_cnt;
	UINT Poly_n;

	pPkt = pMw->pPacket;
	Poly_cnt = *pPoly_no++;
	Poly_n = Poly_cnt;
	Poly_cnt--;
	if (Poly_n != 0)
	{
		do
		{
			/* two GT3 primitives (0x28 bytes each) per index in the list */
			gt3 = (POLY_GT3 *)((int)pPkt + (UINT)*pPoly_no * 0x50);
			pPoly_no++;
			gt3->u0 += mvmu;
			gt3->v0 += mvmu >> 16;
			gt3->u1 += mvmu;
			gt3->v1 += mvmu >> 16;
			gt3->u2 += mvmu;
			gt3->v2 += mvmu >> 16;
			gt3->tpage = (gt3->tpage & 0xffe0) | (tex_page & 0x1f);
			gt3->clut = (gt3->clut & 0x803f) | clut_plus << 6;
			gt3++;
			gt3->u0 += mvmu;
			gt3->v0 += mvmu >> 16;
			gt3->u1 += mvmu;
			gt3->v1 += mvmu >> 16;
			gt3->u2 += mvmu;
			gt3->v2 += mvmu >> 16;
			gt3->tpage = (gt3->tpage & 0xffe0) | (tex_page & 0x1f);
			gt3->clut = (gt3->clut & 0x803f) | clut_plus << 6;
			Prev_cnt = Poly_cnt;
			Poly_cnt--;
		} while (Prev_cnt != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Texture_Move_GT4                                                    */
/* Slide UV coords for selected GT4 polys (by index list pPoly_no)   */
/* ------------------------------------------------------------------ */
/* DS 0x800172f8 (Texture_Move_GT4) */
void Texture_Move_GT4(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus, UCHAR *pPoly_no)
{
	char Du;
	char Dv;
	UINT Poly_cnt;
	UINT Prev_cnt;
	POLY_GT4 *pGt4;
	ULONG *pPkt;

	Poly_cnt = (UINT)*pPoly_no;
	pPkt = pMw->pPacket2;
	if (Poly_cnt != 0)
	{
		pPoly_no++;
		Poly_cnt--;
		do
		{
			/* Each model polygon owns two consecutive GT4 packets (double buffer) */
			pGt4 = (POLY_GT4 *)((int)pPkt + (UINT)*pPoly_no * 0x68);
			pPoly_no++;
			Prev_cnt = Poly_cnt;
			Poly_cnt--;
			Du = (char)mvmu;
			Dv = (char)(mvmu >> 0x10);
			PKT_SET8(&pGt4->u0, (char)pGt4->u0 + Du);
			PKT_SET8(&pGt4->v0, PKT_GET8(&pGt4->v0) + Dv);
			PKT_SET8(&pGt4->u1, (char)pGt4->u1 + Du);
			PKT_SET8(&pGt4->v1, PKT_GET8(&pGt4->v1) + Dv);
			PKT_SET8(&pGt4->u2, (char)pGt4->u2 + Du);
			PKT_SET8(&pGt4->v2, PKT_GET8(&pGt4->v2) + Dv);
			PKT_SET8(&pGt4->u3, (char)pGt4->u3 + Du);
			PKT_SET8(&pGt4->v3, PKT_GET8(&pGt4->v3) + Dv);
			pGt4->tpage = (pGt4->tpage & 0xffe0) | (tex_page & 0x1f);
			pGt4->clut = (pGt4->clut & 0x803f) | clut_plus << 6;
			pGt4++;
			PKT_SET8(&pGt4->u0, (char)pGt4->u0 + Du);
			PKT_SET8(&pGt4->v0, PKT_GET8(&pGt4->v0) + Dv);
			PKT_SET8(&pGt4->u1, (char)pGt4->u1 + Du);
			PKT_SET8(&pGt4->v1, PKT_GET8(&pGt4->v1) + Dv);
			PKT_SET8(&pGt4->u2, (char)pGt4->u2 + Du);
			PKT_SET8(&pGt4->v2, PKT_GET8(&pGt4->v2) + Dv);
			PKT_SET8(&pGt4->u3, (char)pGt4->u3 + Du);
			PKT_SET8(&pGt4->v3, PKT_GET8(&pGt4->v3) + Dv);
			pGt4->tpage = (pGt4->tpage & 0xffe0) | (tex_page & 0x1f);
			pGt4->clut = (pGt4->clut & 0x803f) | clut_plus << 6;
		} while (Prev_cnt != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Texture_Move_GT4_up                                                 */
/* Scale the UV coords of selected GT4 polys (by index list pPoly_no)  */
/* by 1/Late around the offset Ouv (u in the low half, v in the high)  */
/* ------------------------------------------------------------------ */
/* DS 0x8001745c (Texture_Move_GT4_up) */
void Texture_Move_GT4_up(MODEL_WORK *pMw, ULONG Late, ULONG Ouv, UCHAR *pPoly_no)
{
	ULONG Ov;
	ULONG Ovm;
	ULONG Ou;
	ULONG Oum;
	ULONG Du;
	ULONG Dv;
	UINT Poly_cnt;
	UINT Poly_n;
	UINT Prev_cnt;
	POLY_GT4 *gt4;
	ULONG *pPkt;

	/* Du/Dv: rounding remainder of the original UV after the 1/Late scale (12-bit fixed point) */
	Ov = Ouv >> 16;
	Ovm = (Ov << 12) / Late;
	Ou = Ouv & 0xffff;
	Oum = (Ou << 12) / Late;
	Poly_cnt = *pPoly_no++;
	pPkt = pMw->pPacket2;
	Dv = Ovm >> 12;
	Dv = Ov - Dv;
	Du = Oum >> 12;
	Du = Ou - Du;
	Poly_n = Poly_cnt;
	Poly_cnt--;
	if (Poly_n != 0)
	{
		do
		{
			/* two GT4 primitives (0x34 bytes each) per index in the list */
			gt4 = (POLY_GT4 *)((int)pPkt + (UINT)*pPoly_no * 0x68);
			pPoly_no++;
			gt4->u0 = ((gt4->u0 << 12) / Late >> 12) + Du;
			gt4->v0 = ((gt4->v0 << 12) / Late >> 12) + Dv;
			gt4->u1 = ((gt4->u1 << 12) / Late >> 12) + Du;
			gt4->v1 = ((gt4->v1 << 12) / Late >> 12) + Dv;
			gt4->u2 = ((gt4->u2 << 12) / Late >> 12) + Du;
			gt4->v2 = ((gt4->v2 << 12) / Late >> 12) + Dv;
			gt4->u3 = ((gt4->u3 << 12) / Late >> 12) + Du;
			gt4->v3 = ((gt4->v3 << 12) / Late >> 12) + Dv;
			gt4++;
			gt4->u0 = ((gt4->u0 << 12) / Late >> 12) + Du;
			gt4->v0 = ((gt4->v0 << 12) / Late >> 12) + Dv;
			gt4->u1 = ((gt4->u1 << 12) / Late >> 12) + Du;
			gt4->v1 = ((gt4->v1 << 12) / Late >> 12) + Dv;
			gt4->u2 = ((gt4->u2 << 12) / Late >> 12) + Du;
			gt4->v2 = ((gt4->v2 << 12) / Late >> 12) + Dv;
			gt4->u3 = ((gt4->u3 << 12) / Late >> 12) + Du;
			gt4->v3 = ((gt4->v3 << 12) / Late >> 12) + Dv;
			Prev_cnt = Poly_cnt;
			Poly_cnt--;
		} while (Prev_cnt != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Rot_neck                                                            */
/* Rotate neck bone toward target with clamped angular velocity       */
/* ------------------------------------------------------------------ */
/* DS 0x800177c0 (Rot_neck) */
void Rot_neck(ENEMY_WORK *pPm, short Cdir_y)
{
	UCHAR Enemy_neck_no;
	UCHAR Own_neck_no;
	USHORT Body_dir_y;
	PARTS_W *pNeck_parts;
	PARTS_W *pOwn_parts;
	PARTS_W *pOwn;
	SHORT *pFree;
	UINT Spd_lim;
	short Vel;
	UINT Ang_dif;
	ENEMY_WORK *pNeck_enemy;
	SVECTOR Neck_rot;
	VECTOR Neck_tgt;
	SVECTOR Unused_stack;

	if ((pPm->Neck_flg & 1) == 0)
	{
		pNeck_enemy = (ENEMY_WORK *)pPm->pEnemy_neck;
		pOwn_parts = pPm->pSin_parts_ptr;
		Enemy_neck_no = pNeck_enemy->Neck_no;
		Body_dir_y = pOwn_parts->Cdir_y;
		pNeck_parts = pNeck_enemy->pSin_parts_ptr;
		Own_neck_no = pPm->Neck_no;
		pOwn = &pOwn_parts[Own_neck_no];
		Neck_tgt = *(VECTOR *)pNeck_parts[Enemy_neck_no].Workm.t;
		if ((pPm->Neck_flg & 4) != 0)
		{
			Neck_tgt.vy = (long)pPm->Neck_point_y;
			Neck_tgt.vx = (long)pPm->Neck_point_x;
			Neck_tgt.vz = (long)pPm->Neck_point_z;
			if ((USHORT)Neck_tgt.vy == 0x8000)
			{
				pNeck_parts = G.pEnemy[Neck_tgt.vx]->pSin_parts_ptr + G.pEnemy[Neck_tgt.vx]->Neck_no;
				if (Neck_tgt.vx == 0x15)
				{
					pNeck_parts = pPm->pSin_parts_ptr + pPm->Neck_no;
				}
				Neck_tgt = *(VECTOR *)pNeck_parts->Workm.t;
			}
		}
		Neck_Dir_ZY((VECTOR *)pOwn->Workm.t, &Neck_tgt, &Neck_rot, (short)(Cdir_y + Body_dir_y));
		if ((pPm->Neck_flg & 8) != 0)
		{
			Neck_rot.vx = 0;
			Neck_rot.vy = Body_dir_y + (pPm->Neck_point_y + Cdir_y);
			Neck_rot.vz = pPm->Neck_point_z;
		}
		if ((pPm->Neck_flg & 0x40) != 0)
		{
			if (pPm->Neck_point_z == 0)
			{
				pPm->Neck_point_z = 1;
				pPm->Neck_point_y = Body_dir_y + (pOwn->Free[4] + (Cdir_y + -0x80));
			}
			Neck_rot.vy = pPm->Neck_point_y;
		}
		if ((pPm->Neck_flg & 2) != 0)
		{
			Neck_rot.vy = Body_dir_y + (Cdir_y + pOwn->Cdir_y);
		}
		if ((pPm->Neck_flg & 0x10) != 0)
		{
			Neck_rot.vz = pOwn->Cdir_z;
		}
		if ((pPm == pNeck_enemy) && ((pPm->Neck_flg & 0x80) == 0))
		{
			Neck_rot.vy = Body_dir_y + (Cdir_y + pOwn->Cdir_y);
			Neck_rot.vz = pOwn->Cdir_z;
		}
		Spd_lim = (UINT)(USHORT)pOwn->Free[2];
		Vel = pOwn->Free[4];
		if ((pPm->Neck_flg & 0x80) != 0)
		{
			Spd_lim = (UINT)(UCHAR)pOwn->Free[6];
		}
		Ang_dif = ((int)Neck_rot.vy - (int)Cdir_y) - (int)(short)Body_dir_y;
		if ((int)((Ang_dif + (int)Vel) & 0xfff) > ((int)Vel << 1))
		{
			if ((Ang_dif & 0x800) != 0)
			{
				Neck_rot.vy = (Cdir_y + Body_dir_y) - Vel;
			}
			else
			{
				Neck_rot.vy = (Cdir_y + Body_dir_y) + Vel;
			}
		}
		pFree = &pOwn->Free[0];
		Vel = pOwn->Free[0] - (short)Spd_lim;
		Ang_dif = Spd_lim + ((UINT)(USHORT)Neck_rot.vy - ((UINT)(USHORT)pOwn->Free[0] + (UINT)Body_dir_y + (UINT)(USHORT)Cdir_y + (UINT)(USHORT)pOwn->Cdir_y));
		pOwn->Free[0] = Vel;
		if ((Ang_dif & 0x800) == 0)
		{
			pOwn->Free[0] = Vel + (short)Spd_lim * 2;
		}
		if (((int)(Ang_dif & 0xfff) < (int)(Spd_lim << 0x10) >> 0xf) && (pOwn->Free[0] = Neck_rot.vy - (Body_dir_y + (Cdir_y + pOwn->Cdir_y)), (pPm->Neck_flg & 0x40) != 0))
		{
			if (--pPm->Neck_point_x == 0)
			{
				pPm->Neck_flg = 18;
			}
			pPm->Neck_point_y = Body_dir_y + (Cdir_y + ((short)Body_dir_y - (pPm->Neck_point_y - (Cdir_y + 0x80))));
		}
		pOwn->Cdir_y = pOwn->Cdir_y + *pFree;
		if ((pPm->Neck_flg & 0x20) != 0)
		{
			if (pPm->Neck_point_z == 0)
			{
				pPm->Neck_point_z = 1;
				pPm->Neck_point_y = pOwn->Free[5];
			}
			Neck_rot.vz = pPm->Neck_point_y;
		}
		pFree = &pOwn->Free[1];
		Spd_lim = (UINT)(USHORT)pOwn->Free[3];
		Body_dir_y = pOwn->Free[5];
		if ((pPm->Neck_flg & 0x80) != 0)
		{
			Spd_lim = (UINT)((UCHAR *)&pOwn->Free[6])[1];
		}
		Ang_dif = (UINT)Neck_rot.vz;
		if ((int)((Ang_dif + (int)(short)Body_dir_y) & 0xfff) > ((int)(short)Body_dir_y << 1))
		{
			if ((Ang_dif & 0x800) != 0)
			{
				Neck_rot.vz = -Body_dir_y;
			}
			else
			{
				Neck_rot.vz = Body_dir_y;
			}
		}
		Vel = pOwn->Free[1] - (short)Spd_lim;
		Ang_dif = Spd_lim + ((UINT)(USHORT)Neck_rot.vz - ((UINT)(USHORT)pOwn->Cdir_z + (UINT)(USHORT)pOwn->Free[1]));
		pOwn->Free[1] = Vel;
		if ((Ang_dif & 0x800) == 0)
		{
			pOwn->Free[1] = Vel + (short)Spd_lim * 2;
		}
		if (((int)(Ang_dif & 0xfff) < (int)(Spd_lim << 0x10) >> 0xf) && (pOwn->Free[1] = Neck_rot.vz - pOwn->Cdir_z, (pPm->Neck_flg & 0x20) != 0))
		{
			if (Neck_rot.vz != 0)
			{
				if (--pPm->Neck_point_x == 0)
				{
					pPm->Neck_flg = 18;
				}
				pPm->Neck_point_y = 0;
			}
			if (((pPm->Neck_flg & 0x20) != 0) && (Neck_rot.vz == 0))
			{
				pPm->Neck_point_y = pOwn->Free[5];
			}
		}
		pOwn->Cdir_z = pOwn->Cdir_z + *pFree;
		RotMatrix((SVECTOR *)&pOwn->Cdir_x, (MATRIX *)pOwn->M);
		pOwn->Cdir_y = pOwn->Cdir_y - pOwn->Free[0];
		pOwn->Cdir_z = pOwn->Cdir_z - *pFree;
	}
}

/* ------------------------------------------------------------------ */
/* Rot_neck_em                                                         */
/* Rotate neck bone (enemy-to-enemy variant)                          */
/* ------------------------------------------------------------------ */
/* DS 0x80017d98 (Rot_neck_em) */
void Rot_neck_em(ENEMY_WORK *pPm, short Cdir_y)
{
	UINT Own_neck_no;
	UINT Enemy_neck_no;
	USHORT Body_dir_y;
	PARTS_W *pEn_parts;
	PARTS_W *pEn_p;
	short Vel;
	PARTS_W *pOwn_parts;
	PARTS_W *pOwn_p;
	UINT Ang_dif;
	short Free_dif;
	UINT Free_dif12;
	UINT Spd_lim;
	ENEMY_WORK *pNeck_enemy;
	short Dir_sum;
	SVECTOR Neck_rot;
	VECTOR Neck_tgt;

	if ((pPm->Neck_flg & 1) == 0)
	{
		Own_neck_no = pPm->Neck_no;
		pOwn_parts = pPm->pSin_parts_ptr;
		pOwn_p = &pOwn_parts[Own_neck_no];
		pNeck_enemy = (ENEMY_WORK *)pPm->pEnemy_ptr;
		Enemy_neck_no = pNeck_enemy->Neck_no;
		Body_dir_y = pOwn_parts->Cdir_y;
		pEn_parts = pNeck_enemy->pSin_parts_ptr;
		pEn_p = &pEn_parts[Enemy_neck_no];
		Neck_tgt = *(VECTOR *)pEn_p->Workm.t;
		Dir_sum = Cdir_y + Body_dir_y;
		Neck_Dir_ZY((VECTOR *)pOwn_p->Workm.t, &Neck_tgt, &Neck_rot, Dir_sum);
		if ((pPm->Neck_flg & 2) != 0)
		{
			Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
		}
		if (pPm == pNeck_enemy)
		{
			Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
			Neck_rot.vz = 0;
		}
		Spd_lim = (USHORT)pOwn_p->Free[2];
		Vel = pOwn_p->Free[4];
		if ((pPm->Neck_flg & 0x80) != 0)
		{
			Spd_lim = (UCHAR)pOwn_p->Free[6];
		}
		Ang_dif = (Neck_rot.vy - Cdir_y) - (short)Body_dir_y;
		if ((int)((Ang_dif + Vel) & 0xfff) > Vel * 2)
		{
			if ((Ang_dif & 0x800) == 0)
			{
				Neck_rot.vy = Vel + Dir_sum;
			}
			else
			{
				Neck_rot.vy = Dir_sum - Vel;
			}
		}
		Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y) + (USHORT)pOwn_p->Free[0]));
		Vel = pOwn_p->Free[0] - Spd_lim;
		Free_dif12 = Free_dif & 0xfff;
		pOwn_p->Free[0] = Vel;
		if ((Free_dif & 0x800) == 0)
		{
			pOwn_p->Free[0] = Vel + Spd_lim * 2;
		}
		if ((int)Free_dif12 < (int)(Spd_lim << 0x10) >> 0xf)
		{
			pOwn_p->Free[0] = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
		}
		pOwn_p->Cdir_y += pOwn_p->Free[0];
		RotMatrix((SVECTOR *)&pOwn_p->Cdir_x, (MATRIX *)pOwn_p->M);
		pOwn_p->Cdir_y = pOwn_p->Cdir_y - pOwn_p->Free[0];
	}
}

/* ------------------------------------------------------------------ */
/* Rot_neck_em_n                                                       */
/* Rotate neck bone and write result to *pDir                         */
/* ------------------------------------------------------------------ */
/* DS 0x80017fdc (Rot_neck_em_n) */
void Rot_neck_em_n(ENEMY_WORK *pPm, short Cdir_y, short *pDir)
{
	UINT Own_neck_no;
	UINT Enemy_neck_no;
	USHORT Body_dir_y;
	PARTS_W *pEn_parts;
	PARTS_W *pEn_p;
	short Vel;
	PARTS_W *pOwn_parts;
	PARTS_W *pOwn_p;
	UINT Ang_dif;
	short Free_dif;
	UINT Free_dif12;
	UINT Spd_lim;
	ENEMY_WORK *pNeck_enemy;
	short Dir_sum;
	SVECTOR Neck_rot;
	VECTOR Neck_tgt;

	if ((pPm->Neck_flg & 1) == 0)
	{
		Own_neck_no = pPm->Neck_no;
		pOwn_parts = pPm->pSin_parts_ptr;
		pOwn_p = &pOwn_parts[Own_neck_no];
		pNeck_enemy = (ENEMY_WORK *)pPm->pEnemy_ptr;
		Enemy_neck_no = pNeck_enemy->Neck_no;
		Body_dir_y = pOwn_parts->Cdir_y;
		pEn_parts = pNeck_enemy->pSin_parts_ptr;
		pEn_p = &pEn_parts[Enemy_neck_no];
		Neck_tgt = *(VECTOR *)pEn_p->Workm.t;
		Dir_sum = Cdir_y + Body_dir_y;
		Neck_Dir_ZY((VECTOR *)pOwn_p->Workm.t, &Neck_tgt, &Neck_rot, Dir_sum);
		if ((pPm->Neck_flg & 2) != 0)
		{
			Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
		}
		if (pPm == pNeck_enemy)
		{
			Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
			Neck_rot.vz = 0;
		}
		Spd_lim = (USHORT)pOwn_p->Free[2];
		Vel = pOwn_p->Free[4];
		if ((pPm->Neck_flg & 0x80) != 0)
		{
			Spd_lim = (UCHAR)pOwn_p->Free[6];
		}
		Ang_dif = (Neck_rot.vy - Cdir_y) - (short)Body_dir_y;
		if ((int)((Ang_dif + Vel) & 0xfff) > Vel * 2)
		{
			if ((Ang_dif & 0x800) == 0)
			{
				Neck_rot.vy = Vel + Dir_sum;
			}
			else
			{
				Neck_rot.vy = Dir_sum - Vel;
			}
		}
		Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((USHORT)pOwn_p->Free[0] + (Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y)));
		Vel = pOwn_p->Free[0] - Spd_lim;
		Free_dif12 = Free_dif & 0xfff;
		pOwn_p->Free[0] = Vel;
		if ((Free_dif & 0x800) == 0)
		{
			pOwn_p->Free[0] = Vel + Spd_lim * 2;
		}
		if ((int)Free_dif12 < (int)(Spd_lim << 0x10) >> 0xf)
		{
			pOwn_p->Free[0] = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
		}
		*pDir = pOwn_p->Free[0];
	}
}


/* ------------------------------------------------------------------ */
/* Neck_Dir_ZY                                                         */
/* Compute Z/Y rotation angles to face pSv0 toward pSv1              */
/* ------------------------------------------------------------------ */
/* DS 0x8001820c (Neck_Dir_ZY) */
ULONG Neck_Dir_ZY(VECTOR *pSv0, VECTOR *pSv1, SVECTOR *pRot, short Front_dir_y)
{
	long Dist;
	int Ang_y;
	ULONG Ang_z;
	UINT Dz;
	UINT Dy;
	UINT Side;
	long x;

	x = pSv1->vx - pSv0->vx;
	Dz = pSv1->vz - pSv0->vz;
	Dy = pSv0->vy - pSv1->vy;
	Dist = SquareRoot0(x * x + Dz * Dz);
	if (Dz != 0)
	{
		pRot->vy = (USHORT)((Dz & 0x80000000) == 0) * 0x800 + 0x400;
	}
	if (x != 0)
	{
		/* DS: signed divide with the zero and overflow checks */
		DIV_S(Ang_y, (int)(Dz * 0x1000), x);
		Ang_y = catan(Ang_y);
		pRot->vy = ((0x1000 - (short)Ang_y) - ((USHORT)((UINT)x >> 0x14) & 0x800)) & 0xfff;
	}
	Ang_z = 0x80000000;
	if (Dy != 0)
	{
		if ((Dy & 0x80000000) != 0)
		{
			Side = 0;
		}
		else
		{
			Side = 1;
		}
		Ang_z = Side * 0x800 + 0xc00;
		pRot->vz = (short)Ang_z;
	}
	if (Dist != 0)
	{
		DIV_S(Ang_z, (int)(Dy * 0x1000), Dist);
		Ang_z = catan((int)Ang_z);
		pRot->vz = (0x1000U - Ang_z) & 0xfff;
	}
	return Ang_z;
}

/* ------------------------------------------------------------------ */
/* Op_PartsSort00                                                      */
/* Parts sort for enemy type 0 (Op = Osamu/operator)                  */
/* ------------------------------------------------------------------ */
/* DS 0x80018378 (Op_PartsSort00) */
void Op_PartsSort00(ENEMY_WORK *pEm)
{
	/* retail frame is 208 (Op_PartsSort00); this 32-byte reservation and pTmp_m fill it; never read */
	volatile int Rsv[8];
	USHORT Em_dir_y;
	PARTS_W *pP_ptr;
	MATRIX tmp_m[3];
	MATRIX *pTmp_m;

	pP_ptr = pEm->pSin_parts_ptr;
	pTmp_m = tmp_m;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, pTmp_m);
	Em_dir_y = pEm->Cdir_y;
	pP_ptr->Cdir_z &= 0xfff;
	Kami_rot05(pP_ptr + 9, pTmp_m, 400, 0, 0x28, 4, 0, 400, 0, 0x18, 4, 0, (int)(((UINT)Em_dir_y + (UINT)(USHORT)pP_ptr->Cdir_y) * 0x10000) >> 0x10);
}

/* ------------------------------------------------------------------ */
/* Op_PartsSort05                                                      */
/* Parts sort for enemy type 5                                         */
/* ------------------------------------------------------------------ */
/* DS 0x8001844c (Op_PartsSort05) */
void Op_PartsSort05(ENEMY_WORK *pEm)
{
	/* retail reserves 32 more bytes of locals (Op_PartsSort05 frame 224); never read */
	volatile int Rsv[8];
	short rot_y;
	int now_y;
	PARTS_W *pP_ptr;
	PARTS_W *pKami_parts;
	MATRIX tmp_m[3];

	pP_ptr = pEm->pSin_parts_ptr;
	pKami_parts = pP_ptr + 0xf;
	if ((pEm->Be_flg & 0x40000) == 0)
	{
		RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	}
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, tmp_m);
	CompM(tmp_m, (MATRIX *)pP_ptr[8].M, &pP_ptr[8].Workm);
	rot_y = pP_ptr[8].Cdir_y + pEm->Cdir_y + pP_ptr->Cdir_y;
	pP_ptr->Cdir_z &= 0xfff;
	if ((pEm->Neck_flg & 1) == 0)
	{
		rot_y = pP_ptr[8].Free[0] + rot_y;
	}
	now_y = (int)rot_y;
	Kami_rot05(pKami_parts, &pP_ptr[8].Workm,  400, 100, 0x28, 2, 0, 400, 100, 0x18, 4, 0, now_y);
	Kami_rot05(pKami_parts + 1, &pKami_parts->Workm, 0x15e, 0, 0x30, 4, 0x96, 0x96, 0, 0x18, 6, 0, now_y);
	Kami_rot05(pKami_parts + 2, &pKami_parts[1].Workm, 300, 0, 0x30, 4, 0, 0x96, 0, 0x18, 6, 0, now_y);
	Kami_rot05(pKami_parts + 3, &pKami_parts[2].Workm, 0xfa, 0, 0x30, 4, 0, 100, 0, 0x18, 6, 0, now_y);
}

/* ------------------------------------------------------------------ */
/* Op_PartsSort06                                                      */
/* Parts sort for enemy type 6                                         */
/* ------------------------------------------------------------------ */
/* DS 0x80018648 (Op_PartsSort06) */
void Op_PartsSort06(ENEMY_WORK *pEm)
{
	/* retail reserves 128 more bytes of locals (Op_PartsSort06 frame 208); never read */
	volatile int Rsv[32];
	USHORT Em_dir_y;
	PARTS_W *pP_ptr;
	PARTS_W *pKami_parts;

	pP_ptr = pEm->pSin_parts_ptr;
	pKami_parts = pP_ptr + 0xf;	/* retail computes this in the Be_flg bne delay slot (0x8001867c), so it runs on both paths */
	if ((pEm->Be_flg & 0x40000) == 0)
	{
		RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	}
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
	Em_dir_y = pEm->Cdir_y;
	pP_ptr->Cdir_z &= 0xfff;
	Kami_rot05(pKami_parts, &pP_ptr->Workm, 400, 100, 0x30, 4, 0, 0x32, 0, 0x30, 6, 0, (int)(((UINT)Em_dir_y + (UINT)(USHORT)pP_ptr->Cdir_y) * 0x10000) >> 0x10);
}

/* ------------------------------------------------------------------ */
/* Kami_rot05                                                          */
/* Hair/cloth spring physics rotation for parts type 5                */
/* ------------------------------------------------------------------ */
/* DS 0x80018728 (Kami_rot05) */
void Kami_rot05(PARTS_W *pP_ptr, MATRIX *opw, short jyu, short mg, int ow, int air_u, int z_under, int jyu_x, int mg_x, int ow_x, int air_u_x, int x_under, int now_y)
{
	/* retail reserves 68 more bytes of locals (Kami_rot05 frame 288); never read */
	volatile int Rsv[17];
	MATRIX M1;
	MATRIX Mtmp;
	MATRIX Mchk;
	SVECTOR Sv;
	VECTOR Pa;
	VECTOR Pb;
	VECTOR Vc;
	VECTOR *pWt;
	VECTOR *pOt;
	PARTS_W *pPl;
	UINT W;
	UINT Uv;
	int Ang;
	int Tmp;
	int Late;
	short Rz;
	USHORT Rx;
	short Air_u;
	short Mg_x;

	Air_u = (short)air_u;
	Mg_x  = (short)mg_x;
	pWt   = (VECTOR *)pP_ptr->Workm.t;
	pOt   = (VECTOR *)opw->t;

	/* upper joint offset: (0, jyu, 0) through RotY(Cdir_y) */
	Sv.vx = (short)(pP_ptr->Cdir_x + pP_ptr->Free[1]);
	Sv.vy = 0;
	Sv.vz = pP_ptr->Cdir_z + pP_ptr->Free[0];
	RotMatrixYXZ(&Sv, &M1);
	RotMatrixY((int)pP_ptr->Cdir_y, &M1);
	Sv.vx = 0;
	Sv.vy = jyu;
	Sv.vz = 0;
	gte_SetRotMatrix(&M1);
	gte_ldv0(&Sv);
	gte_rtv0();
	gte_stlvnl(&Pa);
	Pa.vx += pWt->vx;
	Pa.vy = Pa.vy + pWt->vy + (int)mg;
	Pa.vz += pWt->vz;

	/* lower joint offset: (0, jyu_x, 0) through the same rotation */
	Sv.vy = jyu_x;
	gte_ldv0(&Sv);
	gte_rtv0();
	gte_stlvnl(&Pb);
	Pb.vx += pWt->vx;
	Pb.vy = Pb.vy + pWt->vy + (int)Mg_x;
	Pb.vz += pWt->vz;

	/* pSin_parts_ptr[8].Workm = (Player.M * pSin.M) * pSin[8].M */
	pPl = G.Player.pSin_parts_ptr;
	CompM((MATRIX *)G.Player.M, (MATRIX *)pPl->M, &Mtmp);
	CompM(&Mtmp, (MATRIX *)pPl[8].M, &pPl[8].Workm);
	/* DS: Mtmp is reloaded from GsIDMATRIX with the lower joint position as translation
	   and run through MulLMatrix into Mchk; Mchk is never read (dead result, DS performs it) */
	Mtmp = GsIDMATRIX;
	Mtmp.t[0] = Pa.vx;
	Mtmp.t[1] = Pa.vy;
	Mtmp.t[2] = Pa.vz;
	MulLMatrix(&pPl[8].Workm, &Mtmp, &Mchk);

	/* translation of this part: parent translation + (opw * position) / 16 */
	Sv.vx = (short)(pP_ptr->Pos_x << 4);
	Sv.vy = (short)(pP_ptr->Pos_y << 4);
	Sv.vz = (short)(pP_ptr->Pos_z << 4);
	ApplyMatrix(opw, &Sv, &Vc);
	pWt->vx = (Vc.vx >> 4) + pOt->vx;
	pWt->vy = (Vc.vy >> 4) + pOt->vy;
	pWt->vz = (Vc.vz >> 4) + pOt->vz;

	/* Z angle from the upper joint lag */
	M1 = GsIDMATRIX;
	RotMatrixY(-(int)pP_ptr->Cdir_y, &M1);
	W = ((UINT)(pWt->vy - Pa.vy) * 0x40000) | ((((UINT)(pWt->vx - Pa.vx)) & 0xffff) << 2);
	Sv.vx = (short)W;
	Sv.vy = (short)(W >> 16);
	Sv.vz = (short)(((short)pWt->vz - (short)Pa.vz) * 4);
	ApplyMatrix(&M1, &Sv, &Vc);
	Rz = W_rot((short)Vc.vx, (short)Vc.vy);

	/* X angle from the lower joint lag */
	W = ((UINT)(pWt->vy - Pb.vy) * 0x40000) | ((((UINT)(pWt->vx - Pb.vx)) & 0xffff) << 2);
	Sv.vx = (short)W;
	Sv.vy = (short)(W >> 16);
	Sv.vz = (short)(((short)pWt->vz - (short)Pb.vz) * 4);
	RotMatrixZ(-(pP_ptr->Cdir_z + pP_ptr->Free[0]), &M1);
	ApplyMatrix(&M1, &Sv, &Vc);
	Rx = (USHORT)W_rot_x(-(short)Vc.vz, (short)Vc.vy);

	/* spring the free angles (damped toward zero, driven by the sin of the lag) */
	Ang = rsin((int)Rz - (int)(short)z_under);
	Tmp = (int)pP_ptr->Free[0];
	DIV_S(Late, Tmp, (int)Air_u);	/* DS: signed divide with the zero and overflow checks */
	Uv  = (UINT)(Tmp - Late) - (UINT)(((int)(short)ow * Ang) >> 12);
	pP_ptr->Free[0] = (USHORT)(Uv & 0xfff);
	if ((Uv & 0x800) != 0)
	{
		pP_ptr->Free[0] = (USHORT)((Uv & 0xfff) - 0x1000);
	}

	Ang = rsin((int)(short)Rx - (int)(short)x_under);
	Tmp = (int)pP_ptr->Free[1];
	DIV_S(Late, Tmp, (int)(short)air_u_x);
	Uv  = (UINT)(Tmp - Late) - (UINT)(((int)(short)ow_x * Ang) >> 12);
	pP_ptr->Free[1] = (USHORT)(Uv & 0xfff);
	if ((Uv & 0x800) != 0)
	{
		pP_ptr->Free[1] = (USHORT)((Uv & 0xfff) - 0x1000);
	}

	/* write back the new angles and rebuild the part matrix */
	pP_ptr->Cdir_z = Rz;
	pP_ptr->Cdir_x = (short)Rx;
	pP_ptr->Cdir_y = (short)((UINT)Rx >> 16);
	RotMatrixYXZ((SVECTOR *)&pP_ptr->Cdir_x, &pP_ptr->Workm);
	pP_ptr->Cdir_y = (short)now_y;
	RotMatrixY((int)(short)now_y, &pP_ptr->Workm);
}

/* ------------------------------------------------------------------ */
/* W_rot                                                               */
/* Convert XZ and Y displacements to a rotation angle (Y axis)        */
/* ------------------------------------------------------------------ */
/* DS 0x80018cc8 (W_rot) */
short W_rot(short rxz, short ry)
{
	UINT Pos_side;
	long x;
	long y;
	long z;
	short rot;

	if (((int)rxz & 0x80000000U) != 0)
	{
		Pos_side = 0;
	}
	else
	{
		Pos_side = 1;
	}
	rot = Pos_side * 0x800 + 0xc00;
	y = -(int)ry;
	if (y != 0)
	{
		x = (int)rxz << 0xc;
		DIV_S(z, (int)x, y);	/* DS: signed divide with the zero and overflow checks */
		z = catan(z);
		rot = (short)z;
		if (y < 0)
		{
			rot += 0x800;
		}
	}
	return rot;
}

/* ------------------------------------------------------------------ */
/* W_rot_x                                                             */
/* Convert XZ and Y displacements to a rotation angle (X axis)        */
/* ------------------------------------------------------------------ */
/* DS 0x80018d64 (W_rot_x) */
short W_rot_x(short rxz, short ry)
{
	UINT Pos_side;
	long x;
	long y;
	long z;
	short rot;

	if (((int)rxz & 0x80000000U) != 0)
	{
		Pos_side = 0;
	}
	else
	{
		Pos_side = 1;
	}
	rot = Pos_side << 0xb | 0x400;
	y = -(int)ry;
	if (y != 0)
	{
		x = (int)rxz << 0xc;
		DIV_S(z, (int)x, y);	/* DS: signed divide with the zero and overflow checks */
		z = catan(z);
		rot = (short)z;
		if (y < 0)
		{
			rot = -rot;
		}
	}
	return rot;
}

/* ------------------------------------------------------------------ */
/* Rbj_set                                                             */
/* Set room-branch-jump pointers for player and enemies               */
/* ------------------------------------------------------------------ */
/* DS 0x80018e08 (Rbj_set) */
void Rbj_set(void)
{
	ULONG *pRbj_rec;
	ENEMY_WORK *pEm;
	UINT Flg_bits;
	ULONG *pRbj;
	int *pSeq_ofs;
	UINT Em_no;
	int *pKan_ofs;
	char Rec_cnt;

	pRbj = (G.pRoom)->pRbj;
	if (pRbj != (ULONG *)0x0)
	{
		Em_no = 0;
		Rec_cnt = (char)pRbj[1];
		pKan_ofs = (int *)((int)pRbj + *pRbj);
		pSeq_ofs = pKan_ofs + 1;
		do
		{
			pRbj_rec = (ULONG *)((int)pRbj + *pKan_ofs);
			Flg_bits = (UINT)*pRbj_rec;
			if ((G.System_flg & SYS_EX_BATTLE) != 0)
			{
				/* life 0 and 3 take the first record bit, life 1 and 2 the second */
				switch (G.SaveData.Player_life)
				{
				case 0:
				case 3:
					if ((Flg_bits & 1) != 0)
					{
												G.Player.pRoom_kan_t_ptr = pRbj_rec + 1;
						G.Player.pRoom_seq_t_ptr = (ULONG *)((int)pRbj + *pSeq_ofs);
					}
					break;
				case 1:
				case 2:
					if ((Flg_bits & 2) != 0)
					{
												G.Player.pRoom_kan_t_ptr = pRbj_rec + 1;
						G.Player.pRoom_seq_t_ptr = (ULONG *)((int)pRbj + *pSeq_ofs);
					}
					break;
				}
				Em_no++;
				Flg_bits >>= 1;
			}
			else if ((G.Status_flg & STAT_SPECIAL_RUN) != 0)
			{
				if ((int)Flg_bits < 0)
				{
					G.Player.pRoom_kan_t_ptr = pRbj_rec + 1;
					G.Player.pRoom_seq_t_ptr = (ULONG *)((int)pRbj + *pSeq_ofs);
				}
			}
			else if ((Flg_bits & 1) != 0)
			{
				G.Player.pRoom_kan_t_ptr = pRbj_rec + 1;
				G.Player.pRoom_seq_t_ptr = (ULONG *)((int)pRbj + *pSeq_ofs);
			}
			Flg_bits >>= 1;
			if (Flg_bits != 0)
			{
				do
				{
					if ((Flg_bits & 1) != 0)
					{
						pEm = G.pEnemy[(Em_no & 0xff) + 1];
						if ((pEm->Be_flg & 1) != 0)
						{
							pEm->pRoom_kan_t_ptr = pRbj_rec + 1;
							G.pEnemy[(Em_no & 0xff) + 1]->pRoom_seq_t_ptr = (ULONG *)((int)pRbj + *pSeq_ofs);
						}
					}
					Flg_bits >>= 1;
					Em_no++;
				} while (Flg_bits != 0);
			}
			pSeq_ofs += 2;
			pKan_ofs += 2;
			Em_no = 0;
		} while (--Rec_cnt != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Em_dead_flg_on                                                      */
/* Set the enemy-dead bit in the appropriate save flag array           */
/* ------------------------------------------------------------------ */
/* DS 0x80018fb0 (Em_dead_flg_on) */
void Em_dead_flg_on(ENEMY_WORK *pEm)
{
	/* Stage_no 3 and above, or a run with STAT_SPECIAL_RUN set, records the kill in Enemy_flg2;
	   otherwise the kill goes to Enemy_flg. The bit number is the enemy's Em_set_flg.
	   DS sets up both arguments separately in each of the three branches (lbu a1 from Em_set_flg,
	   then a0 = Enemy_flg2 or Enemy_flg) and all three branches reach one shared jal Flg_on, so the
	   source calls Flg_on once per branch and the compiler merges the calls into that one call site.
	   A single call fed by pointer and bit locals lets CCPSX fold the two identical Enemy_flg2
	   branches into one block, which DS keeps apart. The Oct 97 mockup lists no locals here. */
	if (G.SaveData.Stage_no >= 3)
	{
		Flg_on(G.SaveData.Enemy_flg2, pEm->Em_set_flg);
	}
	else if ((G.Status_flg & STAT_SPECIAL_RUN) != 0)
	{
		Flg_on(G.SaveData.Enemy_flg2, pEm->Em_set_flg);
	}
	else
	{
		Flg_on(G.SaveData.Enemy_flg, pEm->Em_set_flg);
	}
}

/* ------------------------------------------------------------------ */
/* Aida_Jh_init                                                        */
/* Initialize Aida jelly-hair segments                                 */
/* ------------------------------------------------------------------ */
/* DS 0x80019024 (Aida_Jh_init) */
ULONG Aida_Jh_init(ENEMY_WORK *pEm, ULONG Mem_top)
{
	ULONG Mem_nxt;

	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr, &Aida_jh0_head, Mem_top);
	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr, &Aida_jh1_head, Mem_nxt);
	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr, &Aida_jh2_head, Mem_nxt);
	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr, &Aida_jh4_head, Mem_nxt);
	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr, &Aida_jh5_head, Mem_nxt);
	return Mem_nxt;
}

/* ------------------------------------------------------------------ */
/* Aida_Jh_move                                                        */
/* Animate Aida jelly-hair and sort for rendering                     */
/* ------------------------------------------------------------------ */
/* DS 0x800190c0 (Aida_Jh_move) */
void Aida_Jh_move(ENEMY_WORK *pEm)
{
	MATRIX *m2;
	MATRIX *m2_00;
	PARTS_W *pTmp;
	PARTS_W *pP_ptr;
	MATRIX Save_ws;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	m2_00 = &pP_ptr->Workm;
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, m2_00);
	m2 = &pP_ptr[1].Workm;
	CompM(m2_00, (MATRIX *)pP_ptr[1].M, m2);
	CompM(m2,    (MATRIX *)pP_ptr[2].M, &pP_ptr[2].Workm);
	CompM(m2,    (MATRIX *)pP_ptr[5].M, &pP_ptr[5].Workm);
	CompM(m2_00, (MATRIX *)pP_ptr[9].M, &pP_ptr[9].Workm);
	CompM(m2_00, (MATRIX *)pP_ptr[0xc].M, &pP_ptr[0xc].Workm);
	VSync(1);
	MulJh(pEm, pP_ptr, &Aida_jh0_head);
	MulJh(pEm, pP_ptr, &Aida_jh1_head);
	MulJh(pEm, pP_ptr, &Aida_jh2_head);
	MulJh(pEm, pP_ptr, &Aida_jh4_head);
	MulJh(pEm, pP_ptr, &Aida_jh5_head);
	VSync(1);
	Aida_Jh_sort(pEm);
	if (((G.Status_flg & STAT_MIRROR_MATRIX) != 0) && ((G.Mirror_flg == 2 || (Mirror_in_view((G.pRoom)->pRcut + G.SaveData.Cut_no, G.Mirror_flg, (VECTOR *)&pEm->Pos_x) != 0))))
	{
		Parts_work_cp(pEm);
		pTmp = pEm->pSin_parts_ptr;
		pEm->pSin_parts_ptr = (PARTS_W *)pEm->pM_char_ptr;
		MulJh(pEm, pP_ptr, &Aida_jh0_head);
		MulJh(pEm, pP_ptr, &Aida_jh1_head);
		MulJh(pEm, pP_ptr, &Aida_jh2_head);
		MulJh(pEm, pP_ptr, &Aida_jh4_head);
		MulJh(pEm, pP_ptr, &Aida_jh5_head);
		Save_ws = GsWSMATRIX;
		GsWSMATRIX = GsMWSMATRIX;
		Aida_Jh_sort(pEm);
		GsWSMATRIX = Save_ws;
		pEm->pSin_parts_ptr = pTmp;
	}
}

/* ------------------------------------------------------------------ */
/* Kg_Jh_init                                                          */
/* Initialize Kg (Krauser/creature) jelly-hair segments               */
/* ------------------------------------------------------------------ */
/* DS 0x800193c0 (Kg_Jh_init) */
ULONG Kg_Jh_init(ENEMY_WORK *pEm, ULONG Mem_top)
{
	ULONG Mem_nxt;

	Mem_nxt = Jh_set(pEm, pEm->pSin_parts_ptr + 2, &Kg_jh0_head, Mem_top);
	return Mem_nxt;
}

/* ------------------------------------------------------------------ */
/* Kg_Jh_move                                                          */
/* Animate Kg jelly-hair and sort for rendering                       */
/* ------------------------------------------------------------------ */
/* DS 0x800193f0 (Kg_Jh_move) */
void Kg_Jh_move(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	MulJh(pEm, pP_ptr + 2, &Kg_jh0_head);
	Jh_sort(pEm, pP_ptr + 2);
}

/* DS 0x8009a7c4 (Aida_jh0_tbl) */
JH_TBL Aida_jh0_tbl[73] = {
	{ 0x0019, 0x1000 }, { 0x001a, 0x1000 }, { 0x0011, 0x1000 }, { 0x001c, 0x1000 },
	{ 0x0025, 0x1000 }, { 0x0024, 0x1000 }, { 0x0026, 0x1000 }, { 0x0003, 0x1000 },
	{ 0x0008, 0x1000 }, { 0x0009, 0x1000 }, { 0x0006, 0x1000 }, { 0x0007, 0x1000 },
	{ 0x0016, 0x1000 }, { 0x000a, 0x1000 }, { 0x0022, 0x1000 }, { 0x0067, 0x1000 },
	{ 0x0023, 0x1000 }, { 0x000d, 0x1000 }, { 0x0004, 0x1000 }, { 0x001d, 0x1000 },
	{ 0x0021, 0x1000 }, { 0x0012, 0x1000 }, { 0x001f, 0x1000 }, { 0x0000, 0x1000 },
	{ 0x000e, 0x1000 }, { 0x0005, 0x1000 }, { 0x0001, 0x1000 }, { 0x001b, 0x1000 },
	{ 0x000c, 0x1000 }, { 0x001e, 0x1000 }, { 0x0018, 0x1000 }, { 0x0014, 0x1000 },
	{ 0x0017, 0x1000 }, { 0x0068, 0x1000 }, { 0x0028, 0x1000 }, { 0x0029, 0x1000 },
	{ 0x000b, 0x1000 }, { 0x0013, 0x1000 }, { 0x0027, 0x1000 }, { 0x000f, 0x1000 },
	{ 0x002f, 0x0c00 }, { 0x0036, 0x0c00 }, { 0x0031, 0x0c00 }, { 0x0069, 0x0c00 },
	{ 0x0037, 0x0c00 }, { 0x0035, 0x0c00 }, { 0x0030, 0x0c00 }, { 0x0041, 0x0800 },
	{ 0x0044, 0x0800 }, { 0x0045, 0x0800 }, { 0x006a, 0x0800 }, { 0x0046, 0x0800 },
	{ 0x0042, 0x0800 }, { 0x0043, 0x0800 }, { 0x004d, 0x0400 }, { 0x004f, 0x0400 },
	{ 0x0040, 0x0400 }, { 0x003b, 0x0400 }, { 0x004b, 0x0400 }, { 0x004c, 0x0400 },
	{ 0x004e, 0x0400 }, { 0x004a, 0x0400 }, { 0x003f, 0x0400 }, { 0x003a, 0x0000 },
	{ 0x0053, 0x0000 }, { 0x0052, 0x0000 }, { 0x003e, 0x0000 }, { 0x006e, 0x0000 },
	{ 0x0075, 0x0000 }, { 0x0057, 0x0000 }, { 0x003d, 0x0000 }, { 0x0055, 0x0000 },
	{ 0x0056, 0x0000 }
};

/* DS 0x8009a8e8 (Aida_jh0_head) */
JH_HEAD Aida_jh0_head = { 0x00, 0x01, 0x0049, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, Aida_jh0_tbl };

/* ------------------------------------------------------------------ */
/* Pl_jh_init                                                          */
/* Player jelly-hair init: Jh_set on the player body parts            */
/* DS 0x80019440 (Pl_jh_init) */
/* ------------------------------------------------------------------ */
ULONG Pl_jh_init(ENEMY_WORK *pEm, ULONG Mode)
{
	return Jh_set(pEm, pEm->pSin_parts_ptr, &Pl_jh_head, Mode);
}

/* ------------------------------------------------------------------ */
/* Pl_jh_move                                                          */
/* Player jelly-hair update: MulJh + Jh_sort, then the mirror pass    */
/* DS 0x80019470 (Pl_jh_move) */
/* ------------------------------------------------------------------ */
void Pl_jh_move(ENEMY_WORK *pEm)
{
	PARTS_W *pH_parts;
	PARTS_W *pPl;
	MATRIX Save_ws;

	pH_parts = pEm->pSin_parts_ptr;
	MulJh(pEm, pH_parts, &Pl_jh_head);
	Jh_sort(pEm, pH_parts);
	if ((G.Status_flg & STAT_MIRROR_MATRIX) != 0)
	{
		if ((G.Mirror_flg == 2) || (Mirror_in_view((G.pRoom)->pRcut + G.SaveData.Cut_no, G.Mirror_flg, (VECTOR *)&pEm->Pos_x) != 0))
		{
			Parts_work_cp(pEm);
			pPl = pEm->pSin_parts_ptr;
			pEm->pSin_parts_ptr = (PARTS_W *)pEm->pM_char_ptr;
			MulJh(pEm, pH_parts, &Pl_jh_head);
			Save_ws = GsWSMATRIX;
			GsWSMATRIX = GsMWSMATRIX;
			Jh_sort(pEm, pEm->pSin_parts_ptr);
			GsWSMATRIX = Save_ws;
			pEm->pSin_parts_ptr = pPl;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Jh_set                                                              */
/* Allocate and initialise a JH_HEAD work buffer from animation data  */
/* ------------------------------------------------------------------ */
/* DS 0x80019628 (Jh_set) */
ULONG Jh_set(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh, ULONG Mem_top)
{
	USHORT *pPt_idx;
	short *pSv_vz;
	JH_TBL *pJt;
	ULONG Tmd_base;
	SVECTOR *pSv0;
	SVECTOR *pSv1;
	UINT Num;
	ULONG *pTmd;

	pJt = pJhh->pJh;
	pTmd = pH_parts->pTmd;
	Num = (UINT)pJhh->Vn_num;
	pJhh->p0Vec = (SVECTOR *)Mem_top;
	pSv0 = (SVECTOR *)(Mem_top + Num * 8);
	pJhh->p0Nor = pSv0;
	pSv0 += Num;
	pJhh->p1Vec = pSv0;
	pJhh->p1Nor = pSv0 + Num;
	pSv0 += Num + Num;
	pJhh->p0Ovec = pSv0;
	pSv1 = pSv0 + Num;
	pSv_vz = &pSv0->vz;
	Tmd_base = *pTmd;
	do
	{
		Num--;
		pSv0->vx = *(short *)((UINT)pJt->Point * 8 + Tmd_base) + (pJhh->Parts0_off).vx;
		pSv0++;
		pSv_vz[-1] = *(short *)((UINT)pJt->Point * 8 + Tmd_base + 2) + (pJhh->Parts0_off).vy;
		pPt_idx = &pJt->Point;
		pJt++;
		*pSv_vz = *(short *)((UINT)*pPt_idx * 8 + Tmd_base + 4) + (pJhh->Parts0_off).vz;
		pSv_vz += 4;
	} while (Num != 0);
	pJhh->p1Ovec = pSv1;
	pSv0 = pSv1 + (UINT)pJhh->Vn_num;
	pSv_vz = &pSv1->vz;
	Tmd_base = *pTmd;
	pJt = pJhh->pJh;
	Num = (UINT)pJhh->Vn_num;
	do
	{
		Num--;
		pSv1->vx = *(short *)((UINT)pJt->Point * 8 + Tmd_base) + (pJhh->Parts1_off).vx;
		pSv1++;
		pSv_vz[-1] = *(short *)((UINT)pJt->Point * 8 + Tmd_base + 2) + (pJhh->Parts1_off).vy;
		pPt_idx = &pJt->Point;
		pJt++;
		*pSv_vz = *(short *)((UINT)*pPt_idx * 8 + Tmd_base + 4) + (pJhh->Parts1_off).vz;
		pSv_vz += 4;
	} while (Num != 0);
	pJhh->p0Onor = pSv0;
	pSv1 = pSv0 + (UINT)pJhh->Vn_num;
	pSv_vz = &pSv0->vz;
	Tmd_base = pTmd[2];
	pJt = pJhh->pJh;
	Num = (UINT)pJhh->Vn_num;
	do
	{
		pSv0->vx = *(short *)((UINT)pJt->Point * 8 + Tmd_base);
		Num--;
		pSv_vz[-1] = *(short *)((UINT)pJt->Point * 8 + Tmd_base + 2);
		pPt_idx = &pJt->Point;
		pSv0++;
		pJt++;
		*pSv_vz = *(short *)((UINT)*pPt_idx * 8 + Tmd_base + 4);
		pSv_vz += 4;
	} while (Num != 0);
	return (ULONG)pSv1;
}

/* ------------------------------------------------------------------ */
/* MulJh                                                               */
/* Transform jelly-hair vertices for both source parts                */
/* ------------------------------------------------------------------ */
/* DS 0x800197f4 (MulJh) */
void MulJh(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh)
{
	/* retail reserves 8 more bytes of locals (MulJh frame 16); never read */
	volatile int Rsv[2];
	PARTS_W *pWm;
	SVECTOR *pSrc;
	SVECTOR *pDst;
	SVECTOR *pNsrc;
	SVECTOR *pNdst;
	SVECTOR *pV0;
	SVECTOR *pV1;
	SVECTOR *pN0;
	SVECTOR *pN1;
	JH_TBL *pJt;
	ULONG *pTmd;
	ULONG Vert_top;
	ULONG Nor_top;
	long Tx;
	long Ty;
	long Tz;
	UINT Vn;
	UINT Cnt;
	UINT Pass;
	UINT j;
	UINT Per;
	UINT Pt;
	int Ir;

	Vn   = (UINT)pJhh->Vn_num;
	pTmd = pH_parts->pTmd;

	pWm   = pEm->pSin_parts_ptr + pJhh->Parts0_no;
	pSrc  = (SVECTOR *)pJhh->p0Ovec;
	pDst  = (SVECTOR *)pJhh->p0Vec;
	pNdst = (SVECTOR *)pJhh->p0Nor;
	pNsrc = (SVECTOR *)pJhh->p0Onor;
	gte_SetRotMatrix((MATRIX *)&pWm->Workm);
	Pass = 0;
	goto Vert_loop;

Pass1:
	pWm   = pEm->pSin_parts_ptr + pJhh->Parts1_no;
	pSrc  = (SVECTOR *)pJhh->p1Ovec;
	pDst  = (SVECTOR *)pJhh->p1Vec;
	pNdst = (SVECTOR *)pJhh->p1Nor;
	pNsrc = (SVECTOR *)pJhh->p0Onor;
	gte_SetRotMatrix((MATRIX *)&pWm->Workm);
	Pass = 1;

Vert_loop:
	Tx = pWm->Workm.t[0];
	Ty = pWm->Workm.t[1];
	Tz = pWm->Workm.t[2];
	j = Vn;
	do
	{
		gte_ldv0(pSrc);
		gte_rtv0_b();
		gte_stsv(pDst);
		gte_ldv0(pNsrc);
		pDst->vx = pDst->vx + (short)Tx;
		gte_rtv0_b();
		pDst->vy = pDst->vy + (short)Ty;
		pDst->vz = pDst->vz + (short)Tz;
		gte_stsv(pNdst);
		pSrc++;
		pNsrc++;
		pDst++;
		pNdst++;
	} while (--j != 0);
	if (Pass == 0)
	{
		goto Pass1;
	}


	/* blend the transformed vertices/normals into the TMD by joint table */
	pV0 = (SVECTOR *)pJhh->p0Vec;
	pV1 = (SVECTOR *)pJhh->p1Vec;
	pN0 = (SVECTOR *)pJhh->p0Nor;
	pN1 = (SVECTOR *)pJhh->p1Nor;
	Vert_top = pTmd[0];
	Nor_top  = pTmd[2];
	pJt = pJhh->pJh;
	Cnt = Vn;

	do
	{
		Pt  = (UINT)pJt->Point;
		Per = (UINT)pJt->Per;
		if (Per == 0x1000)
		{
			COPY_SV8(Vert_top + Pt * 8, pV0);
			COPY_SV8(Nor_top + Pt * 8, pN0);
		}
		else if (Per == 0)
		{
			COPY_SV8(Vert_top + Pt * 8, pV1);
			COPY_SV8(Nor_top + Pt * 8, pN1);
		}
		else
		{
			Ir = 0x1000 - (int)Per;
			gte_ldIR0(Per);
			gte_ldsv(pV0);
			gte_gpf12_b();
			gte_ldIR0(Ir);
			gte_ldsv(pV1);
			gte_gpl12_b();
			gte_stsv((SVECTOR *)(Vert_top + Pt * 8));
			gte_ldIR0(Per);
			gte_ldsv(pN0);
			gte_gpf12_b();
			gte_ldIR0(Ir);
			gte_ldsv(pN1);
			gte_gpl12_b();
			gte_stsv((SVECTOR *)(Nor_top + Pt * 8));
		}
		pJt++;
		pV0++;
		pV1++;
		pN0++;
		pN1++;
	} while (--Cnt != 0);
}

/* ------------------------------------------------------------------ */
/* Aida_Jh_sort                                                        */
/* Sort/render Aida jelly-hair model primitives                        */
/* ------------------------------------------------------------------ */
/* DS 0x80019b3c (Aida_Jh_sort) */
void Aida_Jh_sort(ENEMY_WORK *pEm)
{
	POLY_GT4 *p4;
	ULONG Hit_flg;
	MATRIX *r0;
	PARTS_W *pP_ptr;
	MATRIX *m2;
	ULONG *tmd2;
	TMD_MEM *pT_head;
	ULONG rgb;
	ULONG Att;
	MATRIX M;
	MATRIX M1;

	pP_ptr = pEm->pSin_parts_ptr;
	Set_light((VECTOR *)pP_ptr->Workm.t);
	SetColorMatrix((MATRIX *)&COLORMATRIX);
	CompM(&GsWSMATRIX, (MATRIX *)&GsIDMATRIX, &M);
	m2 = &M1;
	CompM(&LIGHTMATRIX, (MATRIX *)&GsIDMATRIX, m2);
	Hit_flg = Hit_ck_point4((VECTOR *)&pEm->Pos_x, G.Cccut);
	if ((Hit_flg != 0) && (pEm->Be_flg & 0x80000) == 0)
	{
		gte_SetRotMatrix((MATRIX *)&M);
		gte_SetTransMatrix((MATRIX *)&M);
		gte_SetLightMatrix((MATRIX *)m2);
		pP_ptr = pEm->pSin_parts_ptr;
		rgb   = pP_ptr->Poly_rgb;
		Att = (pP_ptr->Attribute & 2) >> 1;
		tmd2  = pP_ptr->pPacket2;
		pT_head = (TMD_MEM *)pP_ptr->pTmd2;
		p4 = (POLY_GT4 *)(tmd2 + (UINT)G.Cbuf_id * 0xd);
		SortGT3_neo((TMD_MEM *)pP_ptr->pTmd,  (POLY_GT3 *)(pP_ptr->pPacket + (UINT)G.Cbuf_id * 10), rgb, Att);
		SortGT4_neo(pT_head, p4, rgb, Att);
	}
}

/* ------------------------------------------------------------------ */
/* Jh_sort                                                             */
/* Sort/render a single jelly-hair parts entry                        */
/* ------------------------------------------------------------------ */
/* DS 0x80019cd0 (Jh_sort) */
void Jh_sort(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	ULONG Hit_flg;
	POLY_GT4 *p4;
	MATRIX *r0;
	UINT Cbuf_no;
	ULONG *tmd2;
	TMD_MEM *pT_head;
	ULONG rgb;
	ULONG Att;
	MATRIX *m2;
	MATRIX M;
	MATRIX M1;
	MATRIX M2;
	MATRIX M3;
	MATRIX M4;
	MATRIX M5;
	MATRIX M6;
	VECTOR V;
	(void)M5;		/* unused; retail Jh_sort keeps this slot (vector at 0xf0) */
	(void)M6;

	if ((pP_ptr->Be_flg & 1) != 0)
	{
		Set_light((VECTOR *)(pEm->pSin_parts_ptr->Workm).t);
		SetColorMatrix((MATRIX *)&COLORMATRIX);
		CompM(&GsWSMATRIX, (MATRIX *)&GsIDMATRIX, &M);
		m2 = &M1;
		CompM(&LIGHTMATRIX, (MATRIX *)&GsIDMATRIX, m2);
		if ((pP_ptr->Be_flg & 0x400) != 0)
		{
			/* M2 = copy of Workm (the translation words come with the matrix) */
			M2 = pP_ptr->Workm;
			V.vx = (long)pP_ptr->Size_x;
			V.vy = (long)pP_ptr->Size_y;
			V.vz = (long)pP_ptr->Size_z;
			ScaleMatrix((MATRIX *)&M2, &V);
			TransposeMatrix((MATRIX *)&pP_ptr->Workm, (MATRIX *)&M3);
			M3.t[0] = 0;
			M3.t[1] = 0;
			M3.t[2] = 0;
			M4 = GsIDMATRIX;
			M4.t[0] = -M2.t[0];
			M4.t[1] = -M2.t[1];
			M4.t[2] = -M2.t[2];
			CompM(&M3, &M4, &M3);
			CompM(&M2, &M3, &M2);
			CompM(&GsWSMATRIX, &M2, &M);
		}
		if ((pP_ptr->Be_flg & 0x2000) != 0)
		{
			M2 = GsIDMATRIX;
			M2.t[0] = pP_ptr->Workm.t[0];
			M2.t[1] = pP_ptr->Workm.t[1];
			M2.t[2] = pP_ptr->Workm.t[2];
			V.vx = (long)pP_ptr->Size_x;
			V.vy = (long)pP_ptr->Size_y;
			V.vz = (long)pP_ptr->Size_z;
			ScaleMatrix((MATRIX *)&M2, &V);
			M3 = GsIDMATRIX;
			M3.t[0] = -M2.t[0];
			M3.t[1] = -M2.t[1];
			M3.t[2] = -M2.t[2];
			CompM(&M2, &M3, &M2);
			CompM(&GsWSMATRIX, &M2, &M);
		}
		Hit_flg = Hit_ck_point4((VECTOR *)&pEm->Pos_x, G.Cccut);
		if ((Hit_flg != 0) && (pEm->Be_flg & 0x80000) == 0)
		{
			gte_SetRotMatrix((MATRIX *)&M);
			gte_SetTransMatrix((MATRIX *)&M);
			gte_SetLightMatrix((MATRIX *)m2);
			rgb = pP_ptr->Poly_rgb;
			tmd2 = pP_ptr->pPacket2;
			pT_head = (TMD_MEM *)pP_ptr->pTmd2;
			Att = (pP_ptr->Attribute & 2) >> 1;
			p4 = (POLY_GT4 *)(tmd2 + (UINT)G.Cbuf_id * 0xd);
			SortGT3_neo((TMD_MEM *)pP_ptr->pTmd, (POLY_GT3 *)(pP_ptr->pPacket + (UINT)G.Cbuf_id * 10), rgb, Att);
			SortGT4_neo(pT_head, p4, rgb, Att);
		}
	}
}

/* DS 0x8009a920 (Aida_jh1_tbl) */
JH_TBL Aida_jh1_tbl[23] = {
	{ 0x0061, 0x0000 }, { 0x0064, 0x0000 }, { 0x0049, 0x0000 }, { 0x0063, 0x0000 },
	{ 0x0078, 0x0000 }, { 0x006d, 0x0000 }, { 0x0076, 0x0000 }, { 0x007a, 0x0000 },
	{ 0x006c, 0x0000 }, { 0x0070, 0x0000 }, { 0x005b, 0x0000 }, { 0x005a, 0x0000 },
	{ 0x0058, 0x0000 }, { 0x005c, 0x0000 }, { 0x005e, 0x0000 }, { 0x0054, 0x0000 },
	{ 0x0062, 0x0000 }, { 0x0060, 0x0000 }, { 0x005d, 0x0000 }, { 0x005f, 0x0000 },
	{ 0x0051, 0x0000 }, { 0x0066, 0x0000 }, { 0x0065, 0x0000 }
};

/* DS 0x8009a97c (Aida_jh1_head) */
JH_HEAD Aida_jh1_head = { 0x01, 0x02, 0x0017, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { 0x0019, -199, 0x008c, 0x0000 }, Aida_jh1_tbl };

/* DS 0x8009a9b4 (Aida_jh2_tbl) */
JH_TBL Aida_jh2_tbl[23] = {
	{ 0x0034, 0x0000 }, { 0x0032, 0x0000 }, { 0x0020, 0x0000 }, { 0x0010, 0x0000 },
	{ 0x0079, 0x0000 }, { 0x0073, 0x0000 }, { 0x0077, 0x0000 }, { 0x007b, 0x0000 },
	{ 0x006b, 0x0000 }, { 0x006f, 0x0000 }, { 0x0048, 0x0000 }, { 0x0050, 0x0000 },
	{ 0x0047, 0x0000 }, { 0x0059, 0x0000 }, { 0x002a, 0x0000 }, { 0x0002, 0x0000 },
	{ 0x0039, 0x0000 }, { 0x002c, 0x0000 }, { 0x0015, 0x0000 }, { 0x002e, 0x0000 },
	{ 0x002b, 0x0000 }, { 0x0033, 0x0000 }, { 0x002d, 0x0000 }
};

/* DS 0x8009aa10 (Aida_jh2_head) */
JH_HEAD Aida_jh2_head = { 0x01, 0x05, 0x0017, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { 0x0019, -199, -139, 0x0000 }, Aida_jh2_tbl };

/* DS 0x8009aa48 (Aida_jh3_tbl) */
JH_TBL Aida_jh3_tbl[4] = {
	{ 0x003c, 0x0800 },
	{ 0x0075, 0x0800 },
	{ 0x003c, 0x0800 },
	{ 0x0075, 0x0800 }
};

/* DS 0x8009aa58 (Aida_jh3_head) */
JH_HEAD Aida_jh3_head = { 0x02, 0x05, 0x0002, 0, 0, 0, 0, { 0x0019, -199, 0x008c, 0x0000 }, 0, 0, 0, 0, { 0x0019, -199, -139, 0x0000 }, Aida_jh3_tbl };

/* DS 0x8009aa90 (Aida_jh4_tbl) */
JH_TBL Aida_jh4_tbl[18] = {
	{ 0x0009, 0x0df4 }, { 0x0008, 0x0df4 }, { 0x008c, 0x0800 }, { 0x0086, 0x0800 },
	{ 0x007d, 0x0800 }, { 0x007c, 0x0800 }, { 0x008b, 0x0000 }, { 0x0085, 0x0000 },
	{ 0x0094, 0x0000 }, { 0x0093, 0x0000 }, { 0x0084, 0x0000 }, { 0x008d, 0x0000 },
	{ 0x0087, 0x0000 }, { 0x008a, 0x0000 }, { 0x0081, 0x0000 }, { 0x0080, 0x0000 },
	{ 0x0082, 0x0000 }, { 0x0083, 0x0000 }
};

/* DS 0x8009aad8 (Aida_jh4_head) */
JH_HEAD Aida_jh4_head = { 0x00, 0x09, 0x0012, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { 0x0028, 0x023b, 0x011d, 0x0000 }, Aida_jh4_tbl };

/* DS 0x8009ab10 (Aida_jh5_tbl) */
JH_TBL Aida_jh5_tbl[18] = {
	{ 0x0006, 0x0df4 }, { 0x0007, 0x0df4 }, { 0x0038, 0x0800 }, { 0x0092, 0x0800 },
	{ 0x007e, 0x0800 }, { 0x007f, 0x0800 }, { 0x0088, 0x0000 }, { 0x0074, 0x0000 },
	{ 0x0095, 0x0000 }, { 0x0096, 0x0000 }, { 0x0071, 0x0000 }, { 0x0072, 0x0000 },
	{ 0x0089, 0x0000 }, { 0x003c, 0x0000 }, { 0x0090, 0x0000 }, { 0x0091, 0x0000 },
	{ 0x008f, 0x0000 }, { 0x008e, 0x0000 }
};

/* DS 0x8009ab58 (Aida_jh5_head) */
JH_HEAD Aida_jh5_head = { 0x00, 0x0c, 0x0012, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { 0x0028, 0x023b, -285, 0x0000 }, Aida_jh5_tbl };

/* DS 0x8009ab90 (Kg_jh0_tbl) */
JH_TBL Kg_jh0_tbl[52] = {
	{ 0x0002, 0x1000 }, { 0x0004, 0x1000 }, { 0x0005, 0x1000 }, { 0x000c, 0x1000 },
	{ 0x000f, 0x1000 }, { 0x0000, 0x0c00 }, { 0x0001, 0x0c00 }, { 0x0003, 0x0c00 },
	{ 0x0006, 0x0c00 }, { 0x0007, 0x0c00 }, { 0x000d, 0x0c00 }, { 0x001a, 0x0c00 },
	{ 0x0016, 0x0800 }, { 0x001f, 0x0800 }, { 0x0017, 0x0800 }, { 0x0008, 0x0800 },
	{ 0x0018, 0x0800 }, { 0x001b, 0x0800 }, { 0x0019, 0x0800 }, { 0x0020, 0x0400 },
	{ 0x0021, 0x0400 }, { 0x001e, 0x0400 }, { 0x0009, 0x0400 }, { 0x001c, 0x0400 },
	{ 0x001d, 0x0400 }, { 0x000b, 0x0400 }, { 0x0013, 0x0000 }, { 0x0015, 0x0000 },
	{ 0x0012, 0x0000 }, { 0x000e, 0x0000 }, { 0x0010, 0x0000 }, { 0x0011, 0x0000 },
	{ 0x000a, 0x0000 }, { 0x0014, 0x0000 }, { 0x002b, 0x0e00 }, { 0x002e, 0x0e00 },
	{ 0x0028, 0x0e00 }, { 0x0031, 0x0e00 }, { 0x0025, 0x0e00 }, { 0x0022, 0x0e00 },
	{ 0x002c, 0x0800 }, { 0x002f, 0x0800 }, { 0x0029, 0x0800 }, { 0x0032, 0x0800 },
	{ 0x0026, 0x0800 }, { 0x0023, 0x0800 }, { 0x002d, 0x0000 }, { 0x0030, 0x0000 },
	{ 0x002a, 0x0000 }, { 0x0033, 0x0000 }, { 0x0027, 0x0000 }, { 0x0024, 0x0000 }
};

/* DS 0x8009ac60 (Kg_jh0_head) */
JH_HEAD Kg_jh0_head = { 0x02, 0x03, 0x0034, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000, 0x0000 }, 0, 0, 0, 0, { -504, 0x0000, 0x0000, 0x0000 }, Kg_jh0_tbl };

/* ------------------------------------------------------------------ */
/* Player jelly-hair data (exe ROM initialisers)                       */
/* Jh_set fills the pointer fields of Pl_jh_head at run time           */
/* ------------------------------------------------------------------ */
/* DS 0x8009ac98 (Pl_jh_tbl) */
JH_TBL Pl_jh_tbl[73] = {
	{ 5, 0x1000 }, { 7, 0x1000 }, { 9, 0x1000 }, { 6, 0x1000 },
	{ 4, 0x1000 }, { 8, 0x1000 }, { 11, 0x1000 }, { 14, 0x1000 },
	{ 13, 0x1000 }, { 28, 0x1000 }, { 21, 0x1000 }, { 22, 0x1000 },
	{ 23, 0x1000 }, { 20, 0x1000 }, { 39, 0x1000 }, { 38, 0x1000 },
	{ 34, 0x1000 }, { 43, 0x1000 }, { 44, 0x1000 }, { 46, 0x1000 },
	{ 42, 0x1000 }, { 41, 0x1000 }, { 45, 0x1000 }, { 40, 0x1000 },
	{ 65, 0x1000 }, { 69, 0x1000 }, { 66, 0x1000 }, { 67, 0x1000 },
	{ 68, 0x1000 }, { 70, 0x1000 }, { 71, 0x1000 }, { 72, 0x1000 },
	{ 16, 0x0e00 }, { 10, 0x0e00 }, { 17, 0x0e00 }, { 3, 0x0c00 },
	{ 36, 0x0c00 }, { 33, 0x0c00 }, { 24, 0x0c00 }, { 26, 0x0c00 },
	{ 32, 0x0c00 }, { 35, 0x0c00 }, { 31, 0x0800 }, { 12, 0x0800 },
	{ 30, 0x0800 }, { 18, 0x0800 }, { 37, 0x0800 }, { 15, 0x0800 },
	{ 19, 0x0800 }, { 0, 0x0400 }, { 1, 0x0400 }, { 25, 0x0400 },
	{ 2, 0x0400 }, { 27, 0x0400 }, { 29, 0x0400 }, { 56, 0x0400 },
	{ 49, 0x0400 }, { 48, 0x0400 }, { 50, 0x0400 }, { 57, 0x0400 },
	{ 47, 0x0400 }, { 51, 0x0200 }, { 58, 0x0200 }, { 53, 0x0000 },
	{ 61, 0x0000 }, { 62, 0x0000 }, { 54, 0x0000 }, { 55, 0x0000 },
	{ 60, 0x0000 }, { 59, 0x0000 }, { 52, 0x0000 }, { 64, 0x0000 },
	{ 63, 0x0000 },
};

/* DS 0x8009adbc (Pl_jh_head) */
JH_HEAD Pl_jh_head = {
	0,						/* Parts0_no */
	1,						/* Parts1_no */
	73,						/* Vn_num */
	0, 0, 0, 0,				/* p0Vec, p0Nor, p0Ovec, p0Onor (runtime pointers, zero in ROM) */
	{ 0, 0, 0 },				/* Parts0_off */
	0, 0, 0, 0,				/* p1Vec, p1Nor, p1Ovec, p1Onor (runtime pointers, zero in ROM) */
	{ 29, 19, 0 },				/* Parts1_off */
	Pl_jh_tbl					/* pJh (DS 0x8009ac98) */
};
