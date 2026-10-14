/*
 * SUB.C - Shared utilities: memory, VRAM, view/light, flags, alloc
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SUB.C
 */
#include "TYPES.H"
#include "MONITOR.H"
#include "ROOM.H"
#include "SLD.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include "SUB.h"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "SELECT.H"
#include "flag.h"
#include "DIVCHK.H"

/* DS 0x800d5ba0 */
MEMORY_MANAGE Mem_man[8] = {
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
	{ (VOID *)0x00000000, 0x00000000 },
};


/* DS 0x800ab204 (pCmem_man) */
MEMORY_MANAGE *pCmem_man = Mem_man;	/* retail initial value = Mem_man */

/* DS 0x8018f040 (Prim_work_buf) */
UCHAR Prim_work_buf[0xfb0] RAM_SECTION(".buf_prim_work_buf");

/* DS 0x800769c4 (Vram_clr) */
VOID Vram_clr(register USHORT x, register USHORT y, register USHORT w, register USHORT h)
{
	auto RECT Rect;						// FFFFFFF0		// 8 bytes

	Rect.x = x;
	Rect.y = y;
	Rect.w = w;
	Rect.h = h;
	ClearImage(&Rect, 0, 0, 0);
}

/* DS 0x80076a00 (Memcpy) */
VOID Memcpy(register VOID *p, register VOID *q, register LONG r)
{
	register ULONG end_adr;

	end_adr = (ULONG)((int)p + r);
	do {
		*(UCHAR *)p = *(UCHAR *)q;
		p = (VOID *)((int)p + 1);
		q = (VOID *)((int)q + 1);
	} while ((ULONG)p < end_adr);
}

/* DS 0x80076a28 (Memclr) */
VOID Memclr(register VOID *p, register LONG r)
{
	do {
		*(ULONG *)p = 0;
		r--;
		p = (VOID *)((int)p + 4);
	} while (r > 0);
}

/* DS 0x80076a40 (Mapping_tim) */
VOID Mapping_tim(register ULONG *pTim_Adr)
{
	auto TIM_IMAGE Tim_image;				// FFFFFFD8		// 20 bytes
	register TIM_IMAGE *pTim;				// 20 bytes

	SHORT vram_x;
	INT img_w;
	INT tp_cnt;

	OpenTIM(pTim_Adr);
	pTim = ReadTIM(&Tim_image);
	vram_x = (USHORT)G.Ctpage * 0x40;
	if (G.Ctpage > 0x0f) {
		vram_x -= 0x400;
	}
	pTim->prect->x = vram_x;
	pTim->prect->y = (SHORT)((G.Ctpage >= 0x10) << 8);
	LoadImage(pTim->prect, pTim->paddr);
	img_w = (INT)pTim->prect->w;
	tp_cnt = img_w + 0x3f;
	if (tp_cnt < 0) {
		tp_cnt = img_w + 0x7e;
	}
	G.Ctpage += (CHAR)(tp_cnt >> 6);
	if (pTim->caddr != (ULONG *)0) {
		pTim->crect->y = G.Cclut + 0x1e0;
		LoadImage(pTim->crect, pTim->caddr);
		G.Cclut += (CHAR)pTim->crect->h;
	}
	DrawSync(0);
}

/* DS 0x80076b60 (Mapping_tmd) */
ULONG Mapping_tmd(register ULONG Mode, register ULONG *pTmd_adr, register ULONG Ctp, register ULONG Ccl)
{
	register ULONG *pPrim;
	register ULONG *pPrim2;
	register ULONG *pTmd;
	register ULONG i;
	register ULONG nObj;
	register ULONG nPoly;
	register ULONG Psize;
	ULONG tmd_id;
	INT size;
	ULONG Frame_pad[4];	/* unused; retail Mapping_tmd never references sp+0x10..0x1f (frame 0x40) */

	(void)Frame_pad;
	tmd_id = *pTmd_adr;
	mapModelingData(pTmd_adr + 1);
	size = 0;
	nObj = pTmd_adr[2];
	pPrim = (ULONG *)pTmd_adr[9];
	i = 0;
	if (nObj != 0) {
		pTmd = pTmd_adr + 8;
		do {
			nPoly  = *pTmd;
			Psize  = (i & 1) + 3;
			pPrim2 = (ULONG *)pTmd[1];
			size  += Psize * nPoly * 4;
			if (nPoly != 0) {
				ULONG *p1 = pPrim2 + 1;
				ULONG *p2 = pPrim2;
				do {
					nPoly--;
					*p1 += Ctp * 0x10000;
					p1  += Psize;
					*p2 += Ccl * 0x400000;
					p2  += Psize;
				} while (nPoly != 0);
			}
			i++;
			pTmd += 7;
		} while (i < nObj);
	}
	if (Mode == 0) {
		ULONG *pPrimDst;
		ULONG *pObj;
		ULONG parity;

		pPrimDst = (ULONG *)(Prim_work_buf + (0xfb0 - size));
		memcpy16(pPrimDst, pPrim, size);
		i = 0;
		if (nObj != 0) {
			pObj = pTmd_adr + 8;
			do {
				parity = i & 1;
				pObj[1] = (ULONG)pPrimDst;
				i++;
				pPrimDst  += (parity + 3) * *pObj;
				pObj  += 7;
			} while (i < nObj);
		}
	}
	return tmd_id;
}

/* DS 0x80076cb0 (Set_view) */
VOID Set_view(register LONG *pVp, register LONG *pVr)
{
	auto MATRIX Mat;						// FFFFFFA8		// 32 bytes
	register LONG F1;
	register LONG F2;
	register LONG Dx;
	register LONG Dy;
	register LONG Dz;
	auto VECTOR V;							// FFFFFFC8		// 16 bytes

	GsWSMATRIX = GsIDMATRIX;

	Dx = pVr[0] - pVp[0];
	Dy = pVr[1] - pVp[1];
	Dz = pVr[2] - pVp[2];

	F1 = SquareRoot0(Dx * Dx + Dy * Dy + Dz * Dz);
	F2 = SquareRoot0(Dx * Dx + Dz * Dz);

	Mat = GsIDMATRIX;
	DIV_S(Mat.m[2][2], F2 << 12, F1);	/* DS: div with break checks */
	DIV_S(Mat.m[2][1], Dy << 12, F1);
	Mat.m[1][2] = -Mat.m[2][1];
	Mat.m[1][1] = Mat.m[2][2];
	MulMatrix(&GsWSMATRIX, &Mat);

	if (F2 != 0) {
		Mat = GsIDMATRIX;
		DIV_S(Mat.m[0][0], Dz << 12, F2);
		Mat.m[2][2] = Mat.m[0][0];
		DIV_S(Mat.m[2][0], Dx << 12, F2);
		Mat.m[0][2] = -Mat.m[2][0];
		MulMatrix(&GsWSMATRIX, &Mat);
	}

	V.vx = -pVp[0];
	V.vy = -pVp[1];
	V.vz = -pVp[2];
	ApplyMatrixLV(&GsWSMATRIX, &V, (VECTOR *)&GsWSMATRIX.t);
}

/* DS 0x80076f88 (Set_light) */
VOID Set_light(register VECTOR *pV)
{
	auto VECTOR Vec;						// FFFFFFC8		// 16 bytes
	auto UCHAR Col[4];						// FFFFFFD8		// 4 bytes
	ULONG No;
	INT modeOfs;
	INT colOfs;
	INT atten;
	INT scaled0;
	INT scaled1;
	LONG dist;
	UINT lightRng2;
	UINT lightRng;
	UCHAR *pRgb;
	LIGHT_CUT_DATA *pLightPos;
	LIGHT_CUT_DATA *pLightCut;
	LIGHT_CUT_DATA *pLum;

	No = 0;
	modeOfs = 4;
	colOfs = 0;
	pLightCut = (G.pRoom)->pLight + G.SaveData.Cut_no;
	pLightPos = pLightCut;
	pLum = pLightCut;

	do {
		if (pLightCut->Mode[No] == 0) {
			Vec.vx = (pV->vx - (INT)pLightPos->Pos[0][0]) >> 4;
			Vec.vy = (pV->vy - (INT)pLightPos->Pos[0][1]) >> 4;
			Vec.vz = (pV->vz - (INT)pLightPos->Pos[0][2]) >> 4;
			dist = SquareRoot0(Vec.vx * Vec.vx + Vec.vz * Vec.vz);
			lightRng = (UINT)(pLum->L[0] >> 4);
			atten = (INT)lightRng - (INT)dist;
			if (atten < 0) {
				atten = 0;
			}
			if (pLum->L[0] == 0) {
				Col[0] = 0;
				Col[1] = 0;
				Col[2] = 0;
				Col[3] = 0;
			} else {
				scaled0 = (UINT)pLightCut->Col[0][colOfs] * atten;
				scaled1 = (UINT)pLightCut->Col[0][colOfs + 1] * atten;
				lightRng2 = (UINT)(pLum->L[0] >> 4);
				DIV_S(Col[1], (INT)scaled1, (INT)lightRng2);	/* DS: signed div */
				DIV_S(Col[0], (INT)scaled0, (INT)lightRng);
				atten = (UINT)pLightCut->Col[0][colOfs + 2] * atten;
				lightRng = (UINT)(pLum->L[0] >> 4);
				DIV_S(Col[2], (INT)atten, (INT)lightRng);
			}
			pRgb = Col;
		} else {
			Vec.vx = (LONG)pLightPos->Pos[0][0];
			Vec.vy = (LONG)pLightPos->Pos[0][1];
			Vec.vz = (LONG)pLightPos->Pos[0][2];
			pRgb = pLightCut->Mode + modeOfs - 1;
		}
		modeOfs += 3;
		Set_Light_data(No, &Vec, pRgb);
		pLightPos = (LIGHT_CUT_DATA *)((UCHAR *)pLightPos + sizeof(pLightPos->Pos[0]));
		colOfs += 3;
		No++;
		pLum = (LIGHT_CUT_DATA *)((UCHAR *)pLum + sizeof(pLum->L[0]));
	} while ((INT)No < 3);

	SetBackColor((UINT)pLightCut->Ambient[0], (UINT)pLightCut->Ambient[1], (UINT)pLightCut->Ambient[2]);
}

/* DS 0x80077200 (Set_Light_data) */
VOID Set_Light_data(register ULONG No, register VECTOR *pLig, register UCHAR *pRgb)
{
	auto VECTOR normVec;
	LIGHT_CUT_DATA *pLight;
	SHORT *pLm;
	SHORT *pCm;

	pLight = (G.pRoom)->pLight + G.SaveData.Cut_no;
	pLm = &LIGHTMATRIX.m[No][0];
	pCm = &COLORMATRIX.m[0][No];
	VectorNormal(pLig, &normVec);
	pLm[0] = (SHORT)normVec.vx * -(USHORT)pLight->Mag;
	pLm[1] = (SHORT)normVec.vy * -(USHORT)pLight->Mag;
	pLm[2] = (SHORT)normVec.vz * -(USHORT)pLight->Mag;
	pCm[0] = (USHORT)*pRgb << 4;
	pCm[3] = (USHORT)pRgb[1] << 4;
	pCm[6] = (USHORT)pRgb[2] << 4;
}

/* DS 0x8007730c (Flg_on) */
VOID Flg_on(register ULONG *p, register ULONG x)
{
	p[x >> 5] |= 0x80000000U >> (x & 0x1f);
}

/* DS 0x80077334 (Flg_off) */
VOID Flg_off(register ULONG *p, register ULONG x)
{
	p[x >> 5] &= ~(0x80000000U >> (x & 0x1f));
}

/* DS 0x80077360 (Flg_ck) */
ULONG Flg_ck(register ULONG *p, register ULONG x)
{
	return p[x >> 5] & (0x80000000U >> (x & 0x1f));
}

/* DS 0x80077384 (Rot_vector) */
VOID Rot_vector(register SHORT dir, register SVECTOR *p, register SVECTOR *q)
{
	static SVECTOR Svec;					// 800AB208		// 8 bytes
	register MATRIX *pMat;

	Svec.vy = dir;
	pMat = &Tmp_matrix[0];
	RotMatrix(&Svec, pMat);
	ApplyMatrixSV(pMat, p, q);
}

/* DS 0x800773ec (Rot_vector_super) */
VOID Rot_vector_super(register SHORT dir, register MATRIX *pM, register SVECTOR *p, register SVECTOR *q)
{
	static SVECTOR Svec;					// 800AB210		// 8 bytes
	register MATRIX *pMat;

	Svec.vy = dir;
	pMat = &Tmp_matrix[1];
	RotMatrix(&Svec, pMat);
	MulMatrix2(pM, pMat);
	ApplyMatrixSV(pMat, p, q);
}

/* DS 0x8007746c (Rot_add_matrix) */
VOID Rot_add_matrix(register LONG *pPos, register SHORT *pDir, register MATRIX *pM, register SVECTOR *Add_dir)
{
	auto MATRIX Mat;						// FFFFFF98		// 32 bytes
	auto MATRIX Mat2;						// FFFFFFB8		// 32 bytes
	auto SVECTOR Svec;						// FFFFFFD8		// 8 bytes
	auto VECTOR Vec;						// FFFFFFE0		// 16 bytes
	LONG ang;
	INT m02;
	INT m12;
	INT lenSq;
	INT ySq;
	INT x;
	INT y;

	Mat.t[0] = pPos[0];
	Mat.t[1] = pPos[1];
	Mat.t[2] = pPos[2];
	RotMatrix((SVECTOR *)Add_dir, &Mat);
	MulMatrix2(&Mat, (MATRIX *)pM);
	MatrixNormal((MATRIX *)pM, (MATRIX *)pM);

	m02 = (INT)pM->m[0][2];
	m12 = (INT)pM->m[1][2];
	x = (INT)pM->m[2][2];
	lenSq = m02 * m02 + m12 * m12 + x * x;
	if (lenSq < 0) {
		lenSq += 0xfff;
	}
	lenSq >>= 0xc;
	DIV_S(y, m02 << 0xc, lenSq);	/* DS: div with break checks */

	ang = ratan2(-m12, x);
	*pDir = (SHORT)ang;

	ySq = y * y;
	if (ySq < 0) {
		ySq += 0xfff;
	}
	ang = SquareRoot12(0x1000 - (ySq >> 0xc));
	ang = ratan2(y, ang);
	pDir[1] = (SHORT)ang;
	pDir[2] = 0;

	Mat.t[0] = 0;
	Mat.t[1] = 0;
	Mat.t[2] = 0;
	RotMatrix((SVECTOR *)pDir, &Mat);
	TransposeMatrix(&Mat, &Mat2);

	Svec.vx = pM->m[0][0];
	Svec.vy = pM->m[1][0];
	Svec.vz = pM->m[2][0];
	ApplyMatrix(&Mat2, &Svec, &Vec);
	ang = ratan2(Vec.vy, Vec.vx);
	pDir[2] = (SHORT)ang;
}

/* DS 0x80077600 (Set_front_pos) */
VOID Set_front_pos(register ENEMY_WORK *pEm)
{
	static SVECTOR Svec;					// 800C3C20		// 8 bytes
	register SVECTOR *pSvec;

	pSvec = &Svec;
	pSvec->vz = 0;
	pSvec->vx = (SHORT)((INT)((UINT)pEm->Atd[0].W * 0xb3) >> 7);
	Rot_vector(pEm->Cdir_y, pSvec, pSvec);
	pEm->Fpos_x = pSvec->vx + (SHORT)pEm->Pos_x;
	pEm->Fpos_z = pSvec->vz + (SHORT)pEm->Pos_z;
}

/* Scratchpad stack switch (DS 0x8007769c..0x80077748). The DS functions move
 *  itself, which CCPSX cannot express in C, so the sp moves are inline asm.
 * Each asm sits in a function with no C locals and no calls: a local array
 * would give the function a frame that the epilogue reloads from the
 * scratchpad sp. pStack_bak is the DS slot 0x800c3c28 and G.Sp_bak is DS
 * 0x800ce554 (G+0x236c); both hold raw sp values, not arrays, in DS.
 * Host (MSVC) builds have no  to switch, so the macros are no-ops there. */
#if defined(__mips__) || defined(__psx__) || defined(__R3000__)
/* sw  to dst, then sp = 0x1f8003fc (scratchpad top) */
#define SPAD_SAVE_SP_TOP(dst)	__asm__ volatile ("sw $29, %0\n\tli $29, 0x1f8003fc" : "=m"(dst))
/* sw  to dst, then sp = src */
#define SPAD_SAVE_SP_LOAD(dst, src)	__asm__ volatile ("sw $29, %0\n\tlw $29, %1" : "=m"(dst) : "m"(src))
/* sp = src */
#define SPAD_LOAD_SP(src)	__asm__ volatile ("lw $29, %0" : : "m"(src))
#else
#define SPAD_SAVE_SP_TOP(dst)	((void)0)
#define SPAD_SAVE_SP_LOAD(dst, src)	((void)0)
#define SPAD_LOAD_SP(src)	((void)0)
#endif

/* DS 0x8007769c (SetSpadStack): flag on, pStack_bak = sp, sp = 0x1f8003fc */
VOID SetSpadStack(VOID)
{
	G.System_flg |= SYS_SPAD_STACK;
	SPAD_SAVE_SP_TOP(pStack_bak);
}

/* DS 0x800776d8 (ResetSpadStack): flag off, sp = pStack_bak */
VOID ResetSpadStack(VOID)
{
	G.System_flg &= ~SYS_SPAD_STACK;
	SPAD_LOAD_SP(pStack_bak);
}

/* DS 0x80077708 (SetSpadStack2): flag off, G.Sp_bak = sp, sp = pStack_bak */
VOID SetSpadStack2(VOID)
{
	G.System_flg &= ~SYS_SPAD_STACK;
	SPAD_SAVE_SP_LOAD(G.Sp_bak, pStack_bak);
}

/* DS 0x8007774c (ResetSpadStack2): flag on, sp = G.Sp_bak */
VOID ResetSpadStack2(VOID)
{
	G.System_flg |= SYS_SPAD_STACK;
	SPAD_LOAD_SP(G.Sp_bak);
}

/* DS 0x8007777c (Get_work) */
ULONG Get_work(register VOID **p, register ULONG Size)
{
	VOID *pFree;
	ULONG Ret;

	Ret = 1;
	pFree = G.pFree;
	if ((UCHAR *)G.pFree + Size < (UCHAR *)TASK2_STACK_TOP) {
		*p = pFree;
		pCmem_man->pAddress = pFree;
		G.pFree = (UCHAR *)G.pFree + Size;
		pCmem_man->Size = Size;
		do {
			pCmem_man++;
			Ret = (ULONG)pCmem_man;
		} while (pCmem_man->pAddress != (VOID *)0);
	}
	return Ret;
}

/* DS 0x800777f4 (Free_work) */
ULONG Free_work(register VOID **p)
{
	register MEMORY_MANAGE *pMem;
	MEMORY_MANAGE *pEnd;
	BOOL isBefore;
	ULONG Ret;

	pEnd = &Mem_man[8];
	pMem = Mem_man;
	while ((pMem != pEnd) && (pMem->pAddress != *p)) {
		pMem++;
	}
	if (pMem == pEnd) {
		Ret = 1;
	} else {
		*p = (VOID *)0;
		isBefore = (pMem < pCmem_man);
		pMem->pAddress = (VOID *)0;
		if (isBefore) {
			pCmem_man = pMem;
		}
		Ret = 0;
	}
	return Ret;
}

/* DS 0x800c3c28 (pStack_bak) */
ULONG *pStack_bak = 0;

/* DS 0x800c3be0 (Tmp_matrix) */
MATRIX Tmp_matrix[2];
