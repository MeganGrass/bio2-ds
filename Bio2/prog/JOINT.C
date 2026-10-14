/*
 * JOINT.C - Enemy joint, parts transform and joint motion module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\JOINT.C
 */
#include "TYPES.H"
#include "GTE_OPS.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBAPI.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <GTEMAC.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSPU.H>
#include <MALLOC.H>
#include <STDIO.H>
#include <TYPES.H>
#include "JOINT.h"
#include "DOOR.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "SCA.H"
#include "ST_SUB00.H"
#include "SUB.H"

/* Frame record head at a frame address (retail Joint_move_p 0x80029e10: lh 0x2 at the frame, scaled by Parts0_pos_y) */
typedef struct tagKAN_FRAME {
	SHORT Pos_x;		/* +0x00 low half of the first frame word (J_seq, J_seq2: Pos_x) */
	SHORT Pos_y;		/* +0x02 high half of the first frame word */
	SHORT Pos_z;		/* +0x04 low half of the second frame word (J_seq, J_seq2: Pos_z) */
	SHORT Unk_06;		/* +0x06 high half of the second frame word (not read) */
	UINT  Unk_08;		/* +0x08 third frame word; Rot_adr = Fr+8 (retail 0x80029ef8: addiu s2,s0,-0x4) */
	UINT  Unk_0c;		/* +0x0c first packed rotation word, pRot_w (retail 0x80029ee8: addiu s0,s0,0xc) */
} KAN_FRAME;

/* Byte lanes of one packed rotation word. The group is 9 words holding 8 parts of 12-bit x, y, z fields.
   The field meanings are not in the retail names, so the bytes are Unk_xx (byte offset in the word). */
typedef struct tagKAN_ROT_LANES {
	UCHAR Unk_00;		/* +0 bits 0-7 of the word */
	UCHAR Unk_01;		/* +1 bits 8-15 */
	UCHAR Unk_02;		/* +2 bits 16-23 */
	UCHAR Unk_03;		/* +3 bits 24-31, low 8 bits of a 12-bit Z field (J_seq). Retail 0x80029f6c: lbu v1,0x3(s0), s0 = pRot_w */
} KAN_ROT_LANES;

/* One rotation group as Joint_move_p walks it. Rot_adr is the frame word before the group,
   so Unk_04 is the group's first rotation word (pRot_w[0]) and Unk_24 is pRot_w[8]. */
typedef struct tagKAN_ROT_GRP {
	UINT Unk_00;		/* +0x00 frame word before the group (Rot_adr itself; not read) */
	UINT Unk_04;		/* +0x04 rotation word 0 (pRot_w[0]) */
	UINT Unk_08;		/* +0x08 rotation word 1 (pRot_w[1]). Retail 0x80029f68: lw v0,0x8(s2), s2 = Rot_adr */
	UINT Unk_0c;		/* +0x0c rotation word 2 (pRot_w[2]). Retail 0x8002a018: lbu v0,0x8(s2), s2 = Rot_adr+4 (lane 0) */
	UINT Unk_10;		/* +0x10 rotation word 3 (pRot_w[3]). Retail 0x8002a070: lw v0,0x8(s2), s2 = Rot_adr+8 */
	UINT Unk_14;		/* +0x14 rotation word 4 (pRot_w[4]). Retail 0x8002a0b8: lw v0,0x8(s2), s2 = Rot_adr+12 */
	UINT Unk_18;		/* +0x18 rotation word 5 (pRot_w[5]). Retail 0x8002a138: lw v0,0x8(s2), s2 = Rot_adr+16 (lane 0: 0x8002a120 lbu) */
	UINT Unk_1c;		/* +0x1c rotation word 6 (pRot_w[6]). Retail 0x8002a174: lw v0,0x8(s2), s2 = Rot_adr+20; lane 3 (+0x1f) is 0x8002a1cc: lbu a0,0x3(s2), s2 = Rot_adr+28 */
	UINT Unk_20;		/* +0x20 rotation word 7 (pRot_w[7]). Retail 0x8002a228: lw v1,0x0(s2), s2 = Rot_adr+32 */
	UINT Unk_24;		/* +0x24 rotation word 8 (pRot_w[8]) */
} KAN_ROT_GRP;

/* Joint_trans2 - 0x80027160
   Transforms enemy object parts using rotation/scale matrices.
   Handles super-matrix composition, light setup, and dispatches
   per-part rendering (ko_joint_trans2 / ko_joint_trans3).
   Be_flg and pPos are the 3rd/4th arguments (Ghidra a2/a3).
*/
/* DS 0x80027160 (Joint_trans2) */
void Joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, VECTOR *pPos)
{
	MATRIX M;
	VECTOR v;
	UCHAR i;

	if ((Be_flg & 0x2000000) == 0) {
		RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	}
	else {
		RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
		if ((Be_flg & 0x1000000) == 0) {
			gte_SetRotMatrix((MATRIX *)pEm->pSuper);
			gte_ldclmv(&M);
			gte_rtir();
			gte_stclmv((MATRIX *)pEm->M);
			/* columns 2 and 3 */
			gte_ldclmv((MATRIX *)&M.m[0][1]);
			gte_rtir();
			gte_stclmv((MATRIX *)&pEm->M[0][1]);
			gte_ldclmv((MATRIX *)&M.m[0][2]);
			gte_rtir();
			gte_stclmv((MATRIX *)&pEm->M[0][2]);
		}
		else {
			M.t[0] = pEm->Pos_x;
			M.t[1] = pEm->Pos_y;
			M.t[2] = pEm->Pos_z;
			CompM((MATRIX *)pEm->pSuper, &M, (MATRIX *)pEm->M);
		}
	}

	if ((Be_flg & 0x800) != 0) {
		v.vx = (LONG)pEm->Size;
		v.vy = (LONG)pEm->Size;
		v.vz = (LONG)pEm->Size;
		ScaleMatrix((MATRIX *)pEm->M, &v);
	}

	Set_light((VECTOR *)pP_ptr->Workm.t);
	SetColorMatrix((MATRIX *)&COLORMATRIX);

	i = pEm->Parts_num;
	if (Hit_ck_point4(pPos, G.Cccut) != 0 && (Be_flg & 0x80000) == 0) {
		do {
			if ((pP_ptr->Be_flg & 1) != 0) {
				ko_joint_trans2(pEm, pP_ptr, pP_ptr->Be_flg, &pP_ptr->Workm);
			}
			i--;
			pP_ptr++;
		} while (i != 0);
		return;
	}

	do {
		ULONG be = pP_ptr->Be_flg;
		if ((be & 0x9000) == 0) {
			if ((be & 1) != 0) {
				ko_joint_trans3(pEm, pP_ptr, be, &pP_ptr->Workm);
			}
		}
		else if ((be & 1) != 0) {
			ko_joint_trans2(pEm, pP_ptr, be, &pP_ptr->Workm);
		}
		i--;
		pP_ptr++;
	} while (i != 0);
}

/* A MATRIX rotation part (3x3 shorts plus pad) viewed as five words, as DS copies it. */
typedef struct { ULONG w[5]; } MAT_ROT_W;

/* ko_joint_trans2 - 0x80027434
   Per-part transform with lighting, optional scaling, attribute-driven
   sort-dispatch (SortGT3_neo / SortGT4_neo / Bomb / Ryu variants).
*/
/* DS 0x80027434 (ko_joint_trans2) */
void ko_joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG be_flg, MATRIX *wm)
{
	MATRIX *pSuper;
	MATRIX *pM;
	MATRIX lm;
	ULONG  Att;
	ULONG  Poly_rgb;
	POLY_GT3 *pPacket;
	POLY_GT4 *pPacket2;
	TMD_MEM *tmd;
	TMD_MEM *tmd2;
	VECTOR sv;

	pSuper = (MATRIX *)pP_ptr->pSuper;
	pM     = (MATRIX *)pP_ptr->M;

	if ((*pP_ptr->pOya_parts & 0x21) == 0x20) {
		pP_ptr->Be_flg = 0x20;
		return;
	}

	if ((be_flg & 0x40) == 0) {
		gte_SetRotMatrix(pSuper);
		gte_SetTransMatrix(pSuper);
		gte_ldclmv(pM);
		gte_rtir();
		gte_stclmv(wm);
		gte_ldclmv((MATRIX *)&pP_ptr->M[0][1]);
		gte_rtir();
		gte_stclmv((MATRIX *)&wm->m[0][1]);
		gte_ldclmv((MATRIX *)&pP_ptr->M[0][2]);
		gte_rtir();
		gte_stclmv((MATRIX *)&wm->m[0][2]);
		gte_ldlv0((VECTOR *)&pP_ptr->Pos_x);
		gte_rt();
		gte_stlvnl((VECTOR *)wm->t);
	}

	if ((be_flg & 0x800) != 0) {
		/* parent scale: Size_x, Size_y, Size_z of the parent part (retail offsets 0x8c, 0x8e, 0x90); DS reloads pOya_parts for each product */
		/* DS copies the part rotation (3x3 shorts plus pad) as five words */
		*(MAT_ROT_W *)lm.m = *(MAT_ROT_W *)pP_ptr->M;
		lm.t[0] = pP_ptr->Pos_x * ((PARTS_W *)pP_ptr->pOya_parts)->Size_x >> 12;
		lm.t[1] = pP_ptr->Pos_y * ((PARTS_W *)pP_ptr->pOya_parts)->Size_y >> 12;
		lm.t[2] = pP_ptr->Pos_z * ((PARTS_W *)pP_ptr->pOya_parts)->Size_z >> 12;
		CompM(pSuper, &lm, wm);
	}

	if ((be_flg & 0x20) != 0)
		Parts_down(pP_ptr, wm);

	/* lighting */
	gte_SetRotMatrix((MATRIX *)&LIGHTMATRIX);
	gte_ldclmv(wm);
	gte_rtir();
	gte_stclmv(&lm);
	gte_ldclmv((MATRIX *)&wm->m[0][1]);
	gte_rtir();
	gte_stclmv((MATRIX *)&lm.m[0][1]);
	gte_ldclmv((MATRIX *)&wm->m[0][2]);
	gte_rtir();
	gte_stclmv((MATRIX *)&lm.m[0][2]);
	gte_SetLightMatrix(&lm);

	if ((be_flg & 0x1000) != 0) {
		Set_light((VECTOR *)wm->t);
		MulMatrix0((MATRIX *)&LIGHTMATRIX, wm, &lm);
		SetLightMatrix(&lm);
		Set_light((VECTOR *)pEm->pSin_parts_ptr->Workm.t);
	}

	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
	gte_ldclmv(wm);
	gte_rtir();
	tmd = (TMD_MEM *)pP_ptr->pTmd;
	gte_stclmv(&lm);
	gte_ldclmv((MATRIX *)&wm->m[0][1]);
	gte_rtir();
	tmd2 = (TMD_MEM *)pP_ptr->pTmd2;
	gte_stclmv((MATRIX *)&lm.m[0][1]);
	gte_ldclmv((MATRIX *)&wm->m[0][2]);
	gte_rtir();
	Poly_rgb = pP_ptr->Poly_rgb;
	Att = pP_ptr->Attribute;
	gte_stclmv((MATRIX *)&lm.m[0][2]);
	gte_ldlv0((VECTOR *)wm->t);
	gte_rt();
	pPacket  = (POLY_GT3 *)(pP_ptr->pPacket  + (UINT)G.Cbuf_id * 10);
	pPacket2 = (POLY_GT4 *)(pP_ptr->pPacket2 + (UINT)G.Cbuf_id * 0xd);
	gte_stlvnl((VECTOR *)lm.t);

	if ((be_flg & 0x400) != 0) {
		sv.vx = pP_ptr->Size_x;
		sv.vy = pP_ptr->Size_y;
		sv.vz = pP_ptr->Size_z;
		ScaleMatrix(&lm, &sv);
	}

	if ((be_flg & 0x2000) != 0) {
		sv.vx = pP_ptr->Size_x;
		sv.vy = pP_ptr->Size_y;
		sv.vz = pP_ptr->Size_z;
		/* identity, stored as five words (m00|m01, m02|m10, m11|m12, m20|m21, m22|pad) */
		((MAT_ROT_W *)lm.m)->w[0] = 0x1000;
		((MAT_ROT_W *)lm.m)->w[1] = 0;
		((MAT_ROT_W *)lm.m)->w[2] = 0x1000;
		((MAT_ROT_W *)lm.m)->w[3] = 0;
		((MAT_ROT_W *)lm.m)->w[4] = 0x1000;
		ScaleMatrix(&lm, &sv);
		MulMatrix0(&lm, wm, &lm);
		/* DS copies the translation as a 16-byte VECTOR (the fourth word lands on the dead sv) */
		*(VECTOR *)lm.t = *(VECTOR *)wm->t;
		CompM(&GsWSMATRIX, &lm, &lm);
	}

	if ((be_flg & 0x8000) != 0) {
		if (Hit_ck_point4((VECTOR *)pP_ptr->Workm.t, G.Cccut) == 0)
			be_flg |= 0x4000;
	}

	gte_SetRotMatrix(&lm);
	gte_SetTransMatrix(&lm);

	if ((be_flg & 0x4000) == 0) {
		if ((be_flg & 0x18) == 0) {
			SortGT3_neo(tmd,  pPacket,  Poly_rgb, (Att & 2) >> 1);
			SortGT4_neo(tmd2, pPacket2, Poly_rgb, (Att & 2) >> 1);
			if ((be_flg & 0x80) != 0)
				Model_color_plus((MODEL_WORK *)&pP_ptr->Attribute,
								 *(ULONG *)pP_ptr->Free,
								 *(ULONG *)&pP_ptr->Free[2]);
			return;
		}
		if ((be_flg & 8) != 0) {
			Bomb_Parts_SortGT3(tmd,  pPacket,  pP_ptr);
			Bomb_Parts_SortGT4(tmd2, pPacket2, pP_ptr);
		}
		if ((be_flg & 0x10) != 0) {
			Ryu_Parts_SortGT3(tmd,  pPacket,  pP_ptr, (Att & 2) >> 1);
			Ryu_Parts_SortGT4(tmd2, pPacket2, pP_ptr, (Att & 2) >> 1);
		}
	}

	if ((be_flg & 8) != 0)
		Parts_bomb(pP_ptr);
	else if ((be_flg & 0x10) != 0)
		Parts_ryu(pP_ptr);
}

/* SortGT3_neo - 0x80027bec
   Sorts textured gouraud-shaded triangle primitives into the OT.
   Control flow mirrors the DS loop: s1/s2 are re-read from the next
   primitive after every rejected/accepted triangle.
*/
/* DS 0x80027bec (SortGT3_neo) */
void SortGT3_neo(TMD_MEM *pT_head, POLY_GT3 *bp, ULONG Cv, ULONG D0)
{
	ULONG *prim_p;
	ULONG *ver_p;
	ULONG *nor_p;
	ULONG n_poly;
	ULONG sp[2];	/* sp[1] doubles as the rgbc word (gte_ldrgb source) */
	ULONG *ot;
	SVECTOR *s0, *s1, *s2, *s3;
	SHORT v1, v2;
	ULONG *p_prim;

	prim_p = pT_head->prim_top;
	ver_p  = pT_head->ver_top;
	sp[1] = (Cv & 0xffffff) | ((ULONG)(UCHAR)(D0 << 1) << 24) | 0x34000000;
	n_poly = pT_head->n_poly;
	ot = G.Ot1[G.Cbuf_id];
	gte_ldrgb((CVECTOR *)&sp[1]);
	s0 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 2) * 2);
	v1 = *(SHORT *)((int)prim_p + 6);
	v2 = *(SHORT *)((int)prim_p + 10);

	while (n_poly != 0) {
		s1 = (SVECTOR *)(ver_p + v1 * 2);
		s2 = (SVECTOR *)(ver_p + v2 * 2);
		p_prim = prim_p;

		gte_ldv3(s0, s1, s2);
		prim_p = p_prim + 3;
		n_poly--;
		gte_rtpt_b();
		nor_p = pT_head->nor_top;
		s3 = (SVECTOR *)(nor_p + (SHORT)p_prim[1] * 2);
		s1 = (SVECTOR *)(nor_p + (SHORT)p_prim[2] * 2);
		s2 = (SVECTOR *)(nor_p + (SHORT)p_prim[0] * 2);
		gte_nclip_b();
		s0 = (SVECTOR *)(ver_p + *(SHORT *)((int)p_prim + 0xe) * 2);
		gte_stopz((long *)sp);
		if ((int)sp[0] < 1) {
			bp += 2;
			v1 = *(SHORT *)((int)prim_p + 6);
			v2 = *(SHORT *)((int)prim_p + 10);
			continue;
		}
		gte_avsz3_b();
		gte_stsxy3_gt3(&bp->tag);
		gte_stotz((long *)sp);
		gte_ldv3(s2, s3, s1);
		gte_ncct();
		v1 = *(SHORT *)((int)prim_p + 6);
		v2 = *(SHORT *)((int)prim_p + 10);
		if ((int)sp[0] < 8) {
			bp += 2;
			continue;
		}
		if (0x1fff < (int)sp[0]) sp[0] = 0x1fff;
		{
			ULONG *pOT = (ULONG *)((UCHAR *)ot + (sp[0] >> 1 & 0xffc));
			sp[0] = *pOT;
			*pOT  = (ULONG)bp & 0xffffff;
			bp->tag = (sp[0] & 0xffffff) | 0x9000000;
			gte_strgb3_gt3(&bp->tag);
			bp += 2;
		}
	}
}

/* SortGT4_neo - 0x80027dbc
   Sorts textured gouraud-shaded quad primitives into the OT.
*/
/* DS 0x80027dbc (SortGT4_neo) */
void SortGT4_neo(TMD_MEM *pT_head, POLY_GT4 *bp, ULONG Cv, ULONG D0)
{
	ULONG *prim_p;
	ULONG *ver_p;
	ULONG *nor_p;
	ULONG n_poly;
	ULONG sp[2];	/* sp[1] doubles as the rgbc word (gte_ldrgb source) */
	ULONG *ot;
	SVECTOR *s0, *s1, *s2, *r1;

	prim_p = pT_head->prim_top;
	ver_p  = pT_head->ver_top;
	sp[1] = (Cv & 0xffffff) | ((ULONG)(UCHAR)(D0 << 1) << 24) | 0x3c000000;
	n_poly = pT_head->n_poly;
	ot = G.Ot1[G.Cbuf_id];
	s0 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 2) * 2);
	s1 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 6) * 2);
	s2 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 10) * 2);
	gte_ldrgb((CVECTOR *)&sp[1]);

	while (n_poly != 0) {
		ULONG *next = prim_p + 4;
		gte_ldv3(s0, s1, s2);
		n_poly--;
		gte_rtpt_b();
		s0 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 0x12) * 2);
		gte_nclip_b();
		s2 = (SVECTOR *)(ver_p + *(SHORT *)((int)prim_p + 0xe) * 2);
		gte_stopz((long *)sp);
		if ((int)sp[0] < 0) {
			bp += 2;
			s1 = (SVECTOR *)(ver_p + *(SHORT *)((int)next + 6) * 2);
			s2 = (SVECTOR *)(ver_p + *(SHORT *)((int)next + 10) * 2);
			prim_p = next;
			continue;
		}
		gte_stsxy3_gt3(&bp->tag);
		gte_ldv0(s2);
		gte_rtps();
		nor_p = pT_head->nor_top;
		gte_ldv0((SVECTOR *)(nor_p + (SHORT)next[-1] * 2));
		gte_nccs();
		s2 = (SVECTOR *)(nor_p + (SHORT)next[-4] * 2);
		r1 = (SVECTOR *)(nor_p + (SHORT)next[-3] * 2);
		s1 = (SVECTOR *)(nor_p + (SHORT)next[-2] * 2);
		gte_avsz4_b();
		gte_stsxy((long *)&bp->x3);
		gte_strgb((CVECTOR *)&bp->r3);
		gte_stotz((long *)sp);
		gte_ldv3(s2, r1, s1);
		gte_ncct();
		s1 = (SVECTOR *)(ver_p + *(SHORT *)((int)next + 6) * 2);
		s2 = (SVECTOR *)(ver_p + *(SHORT *)((int)next + 10) * 2);
		if ((int)sp[0] < 8) {
			bp += 2;
			prim_p = next;
			continue;
		}
		if (0x1fff < (int)sp[0]) sp[0] = 0x1fff;
		{
			ULONG *pOT = (ULONG *)((UCHAR *)ot + (sp[0] >> 1 & 0xffc));
			sp[0] = *pOT;
			*pOT  = (ULONG)bp & 0xffffff;
			bp->tag = (sp[0] & 0xffffff) | 0xc000000;
			gte_strgb3_gt3(&bp->tag);
			bp += 2;
			prim_p = next;
		}
	}
}

/* ko_joint_trans3 - 0x80027ff0
   Simplified per-part transform (no lighting/scaling path).
   Handles parts_bomb / parts_ryu dispatch only.
*/
/* DS 0x80027ff0 (ko_joint_trans3) */
void ko_joint_trans3(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG be_flg, MATRIX *wm)
{
	MATRIX *pSuper;
	MATRIX *pM;
	MATRIX lm;

	pSuper = (MATRIX *)pP_ptr->pSuper;
	pM     = (MATRIX *)pP_ptr->M;

	if ((*pP_ptr->pOya_parts & 0x21) == 0x20) {
		pP_ptr->Be_flg = 0x20;
		return;
	}

	if ((be_flg & 0x40) == 0) {
		gte_SetRotMatrix(pSuper);
		gte_SetTransMatrix(pSuper);
		gte_ldclmv(pM);
		gte_rtir();
		gte_stclmv(wm);
		gte_ldclmv((MATRIX *)&pP_ptr->M[0][1]);
		gte_rtir();
		gte_stclmv((MATRIX *)&wm->m[0][1]);
		gte_ldclmv((MATRIX *)&pP_ptr->M[0][2]);
		gte_rtir();
		gte_stclmv((MATRIX *)&wm->m[0][2]);
		gte_ldlv0((VECTOR *)&pP_ptr->Pos_x);
		gte_rt();
		gte_stlvnl((VECTOR *)wm->t);
	}

	if ((be_flg & 0x20) != 0)
		Parts_down(pP_ptr, wm);

	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
	gte_ldclmv(wm);
	gte_rtir();
	gte_stclmv(&lm);
	gte_ldclmv((MATRIX *)&wm->m[0][1]);
	gte_rtir();
	gte_stclmv((MATRIX *)&lm.m[0][1]);
	gte_ldclmv((MATRIX *)&wm->m[0][2]);
	gte_rtir();
	gte_stclmv((MATRIX *)&lm.m[0][2]);
	gte_ldlv0((VECTOR *)wm->t);
	gte_rt();
	gte_stlvnl((VECTOR *)lm.t);

	if ((be_flg & 8) != 0)
		Parts_bomb(pP_ptr);
	else if ((be_flg & 0x10) != 0)
		Parts_ryu(pP_ptr);
}

/* PartsWork_set - 0x80028324
   Records the part count and the parts-work pointer, and returns the end of
   the parts work area (one 0xac-byte PARTS_W per part).
*/
/* DS 0x80028324 (PartsWork_set) */
ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top)
{
	pEm->Parts_num = (UCHAR)(pEm->pTmd[-1] >> 1);
	pEm->pSin_parts_ptr = (PARTS_W *)work_top;
	return work_top + (UINT)pEm->Parts_num * 0xac;
}

/* PartsWork_link - 0x80028368
   Initialises every part of pEm from the kan data (position triples and
   parent/child link table), presets the GT3/GT4 packets and returns the
   next free packet address.
   Layout note: pKan_t_ptr[0] low half = link-table offset (& ~3),
   [1] low half = link entry count, position triples start at kan+8.
*/
/* DS 0x80028368 (PartsWork_link) */
ULONG PartsWork_link(ENEMY_WORK *pEm, ULONG Packet_top, ULONG *pKan_t_ptr, ULONG Flg)
{
	PARTS_W *pTop;
	PARTS_W *pCur;
	USHORT  *pPos;
	UINT    *pLink;
	UCHAR   *pLinkBase;
	UCHAR   *pChild;
	ULONG   *pTmd_cur;
	ULONG   *pG_packet;
	UINT     link_cnt;
	UINT     n;
	UINT     cnt;
	UCHAR    part_no;
	ULONG    Frame_pad[38];		/* retail reserves 0xd8 bytes and uses only the top (spills at 0x88-0xa8) */

	(void)Frame_pad;
	part_no   = 0;
	pPos      = &((USHORT *)pKan_t_ptr)[4];
	n         = pEm->Parts_num;
	pCur      = pEm->pSin_parts_ptr;
	pTmd_cur  = pEm->pTmd;
	link_cnt  = ((KAN_HEAD *)pKan_t_ptr)->Parts_cnt;
	pLinkBase = (UCHAR *)pKan_t_ptr + (((USHORT *)pKan_t_ptr)[0] & 0xfffc);
	pLink     = (UINT *)pLinkBase;

	/* part 0 hangs off pEm (M, Be_flg); the ENEMY_WORK super-matrix/vector are reset */
	pCur->pSuper     = (ULONG *)pEm->M;
	pCur->pOya_parts = &pEm->Be_flg;
	pEm->Super_vector.vx = 0;
	pEm->Super_vector.vy = 0;
	pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;
	pEm->pSuper = (ULONG *)&pEm->Super_matrix;

	pTop = pCur;
	for (; n != 0; n--) {
		pCur->Poly_rgb = 0x808080;
		/* identity working matrix (Workm) and identity rotation (M) */
		pCur->Workm.m[0][0] = 0x1000;
		pCur->Workm.m[0][1] = 0;
		pCur->Workm.m[0][2] = 0;
		pCur->Workm.m[1][0] = 0;
		pCur->Workm.m[1][1] = 0x1000;
		pCur->Workm.m[1][2] = 0;
		pCur->Workm.m[2][0] = 0;
		pCur->Workm.m[2][1] = 0;
		pCur->Workm.m[2][2] = 0x1000;
		pCur->Workm.t[0] = 0;
		pCur->Workm.t[1] = 0;
		pCur->Workm.t[2] = 0;
		pCur->M[0][0] = 0x1000;
		pCur->M[0][1] = 0;
		pCur->M[0][2] = 0;
		pCur->M[1][0] = 0;
		pCur->M[1][1] = 0x1000;
		pCur->M[1][2] = 0;
		pCur->M[2][0] = 0;
		pCur->M[2][1] = 0;
		pCur->M[2][2] = 0x1000;
		pCur->Attribute = 0;
		pCur->Cdir_x = 0;
		pCur->Cdir_y = 0;
		pCur->Cdir_z = 0;
		pCur->Free[0] = 0;
		pCur->Free[1] = 0;
		pCur->Free[2] = 0;
		pCur->Free[3] = 0;
		pCur->Free[4] = 0;
		pCur->Free[5] = 0;
		pCur->Be_flg = 1;
		pCur->Parts_no = part_no;
		pCur->pTmd  = pTmd_cur;
		pCur->pTmd2 = (ULONG *)((TMD_MEM *)pTmd_cur + 1);

		pG_packet = (ULONG *)PresetObjectGT3_neo((MODEL_WORK *)&pCur->Attribute, (ULONG *)Packet_top, Flg);
		Packet_top = (ULONG)PresetObjectGT4_neo((MODEL_WORK *)&pCur->Attribute, pG_packet, Flg);

		/* position triple (x,y,z) from kan data */
		pCur->Pos_x = (SHORT)pPos[0];
		pCur->Old_x = (SHORT)pPos[0];
		pCur->Pos_y = (SHORT)pPos[1];
		pCur->Old_y = (SHORT)pPos[1];
		pCur->Pos_z = (SHORT)pPos[2];
		pCur->Old_z = (SHORT)pPos[2];
		pPos += 3;

		if (link_cnt == 0) {
			pCur->pSuper     = (ULONG *)&GsIDMATRIX;
			pCur->pOya_parts = NULL;
		} else {
			/* parent link: list of child indices for this part.
			   The child offset is relative to the start of the link table. */
			pChild = pLinkBase + (*pLink >> 16);
			cnt = *pLink & 0xffff;
			for (; cnt != 0; cnt--) {
				pTop[*pChild].pSuper     = (ULONG *)&pCur->Workm;
				pTop[*pChild].pOya_parts = &pCur->Be_flg;
				pChild++;
			}
			pLink++;
			link_cnt--;
		}
		pCur++;
		pTmd_cur = (ULONG *)((TMD_MEM *)pTmd_cur + 2);
		part_no++;
	}
	return Packet_top;
}

/* Init_Parts_work - 0x80028648
   Same initialisation as PartsWork_link without the packet preset and link
   table: sets identity matrices, zero rotation and the kan position triples.
*/
/* DS 0x80028648 (Init_Parts_work) */
void Init_Parts_work(ENEMY_WORK *pEm)
{
	USHORT *pPos;
	PARTS_W *pCur;
	UINT n;
	UINT k;
	UINT r, c;
	UCHAR Frame_pad[120];

	(void)Frame_pad;

	n      = pEm->Parts_num;
	pPos   = &((USHORT *)pEm->pKan_t_ptr)[4];
	pCur   = pEm->pSin_parts_ptr;

	pCur->pSuper     = (ULONG *)pEm->M;
	pCur->pOya_parts = &pEm->Be_flg;
	pEm->Super_vector.vx = 0;
	pEm->Super_vector.vy = 0;
	pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;
	pEm->pSuper = (ULONG *)&pEm->Super_matrix;

	for (k = 0; k < n; k++) {
		for (r = 0; r < 3; r++) {
			for (c = 0; c < 3; c++) {
				pCur->M[r][c]       = (r == c) ? 0x1000 : 0;
				pCur->Workm.m[r][c] = (r == c) ? 0x1000 : 0;
			}
		}
		pCur->Workm.t[0] = 0;
		pCur->Workm.t[1] = 0;
		pCur->Workm.t[2] = 0;
		pCur->Attribute  = 0;
		pCur->Poly_rgb   = 0x808080;
		for (r = 0; r < 6; r++) {
			pCur->Free[r] = 0;
		}
		pCur->Cdir_x = 0;
		pCur->Cdir_y = 0;
		pCur->Cdir_z = 0;
		pCur->Be_flg = 1;

		pCur->Pos_x = (LONG)(SHORT)pPos[0];
		pCur->Pos_y = (LONG)(SHORT)pPos[1];
		pCur->Pos_z = (LONG)(SHORT)pPos[2];
		pCur->Old_x = (SHORT)pCur->Pos_x;
		pCur->Old_y = (SHORT)pCur->Pos_y;
		pCur->Old_z = (SHORT)pCur->Pos_z;
		pPos += 3;
		pCur++;
	}
}

/* Parts_work_kan_init - 0x80028794
   PARTS_W setup from kan data: per-part defaults, position triples, and the
   parent/child pointer links. The DS pointer-slot writes at part-array words
   0x1d/0x25 are PARTS_W.pSuper (byte 0x74) and PARTS_W.pOya_parts (byte 0x94).
*/
/* DS 0x80028794 (Parts_work_kan_init) */
void Parts_work_kan_init(ENEMY_WORK *pEm, USHORT *pKan)
{
	PARTS_W *pTop;
	PARTS_W *pCur;
	USHORT  *pPos;
	UINT    *pLink;
	UCHAR   *pLinkBase;
	UCHAR   *pChild;
	UINT     link_cnt;
	UINT     n;
	UINT     cnt;
	UCHAR    part_no;
	UCHAR    Frame_pad[120];	/* retail reserves sp+0x00..0x77 and never reads it (as in Init_Parts_work) */

	(void)Frame_pad;
	part_no   = 0;
	pPos      = &pKan[4];
	n         = pEm->Parts_num;
	pCur      = pEm->pSin_parts_ptr;
	link_cnt  = ((KAN_HEAD *)pKan)->Parts_cnt;
	pLinkBase = (UCHAR *)pKan + (pKan[0] & 0xfffc);
	pLink     = (UINT *)pLinkBase;

	/* part 0 hangs off pEm (M, Be_flg); the ENEMY_WORK super-matrix/vector are reset */
	pCur->pSuper     = (ULONG *)pEm->M;
	pCur->pOya_parts = &pEm->Be_flg;
	pEm->Super_vector.vx = 0;
	pEm->Super_vector.vy = 0;
	pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;
	pEm->pSuper = (ULONG *)&pEm->Super_matrix;

	pTop = pCur;
	for (; n != 0; n--) {
		pCur->Poly_rgb = 0x808080;
		/* identity working matrix (Workm) and identity rotation (M) */
		pCur->Workm.m[0][0] = 0x1000;
		pCur->Workm.m[0][1] = 0;
		pCur->Workm.m[0][2] = 0;
		pCur->Workm.m[1][0] = 0;
		pCur->Workm.m[1][1] = 0x1000;
		pCur->Workm.m[1][2] = 0;
		pCur->Workm.m[2][0] = 0;
		pCur->Workm.m[2][1] = 0;
		pCur->Workm.m[2][2] = 0x1000;
		pCur->Workm.t[0] = 0;
		pCur->Workm.t[1] = 0;
		pCur->Workm.t[2] = 0;
		pCur->M[0][0] = 0x1000;
		pCur->M[0][1] = 0;
		pCur->M[0][2] = 0;
		pCur->M[1][0] = 0;
		pCur->M[1][1] = 0x1000;
		pCur->M[1][2] = 0;
		pCur->M[2][0] = 0;
		pCur->M[2][1] = 0;
		pCur->M[2][2] = 0x1000;
		pCur->Attribute = 0;
		pCur->Cdir_x = 0;
		pCur->Cdir_y = 0;
		pCur->Cdir_z = 0;
		pCur->Free[0] = 0;
		pCur->Free[1] = 0;
		pCur->Free[2] = 0;
		pCur->Free[3] = 0;
		pCur->Free[4] = 0;
		pCur->Free[5] = 0;
		pCur->Be_flg = 1;
		pCur->Parts_no = part_no;
		pCur->Pos_x = (SHORT)pPos[0];
		pCur->Old_x = (SHORT)pPos[0];
		pCur->Pos_y = (SHORT)pPos[1];
		pCur->Old_y = (SHORT)pPos[1];
		pCur->Pos_z = (SHORT)pPos[2];
		pCur->Old_z = (SHORT)pPos[2];
		pPos += 3;

		if (link_cnt == 0) {
			pCur->pSuper     = (ULONG *)&GsIDMATRIX;
			pCur->pOya_parts = NULL;
		} else {
			/* children point their pSuper at this part's Workm and pOya_parts at this part */
			pChild = pLinkBase + (*pLink >> 16);
			cnt = *pLink & 0xffff;
			for (; cnt != 0; cnt--) {
				pTop[*pChild].pSuper     = (ULONG *)&pCur->Workm;
				pTop[*pChild].pOya_parts = &pCur->Be_flg;
				pChild++;
			}
			pLink++;
			link_cnt--;
		}
		pCur++;
		part_no++;
	}
}

/* Mem_ck_Parts_work - 0x800289b4
   Returns the memory needed for the parts' TMD-derived packet buffers.
*/
/* DS 0x800289b4 (Mem_ck_Parts_work) */
ULONG Mem_ck_Parts_work(ENEMY_WORK *pEm)
{
	ULONG sum = 0;
	UINT  k;
	PARTS_W *pP = pEm->pSin_parts_ptr;

	for (k = 0; k < pEm->Parts_num; k++) {
		sum += 0xc + ((TMD_MEM *)pP[k].pTmd)->n_poly * 0x14 + ((TMD_MEM *)pP[k].pTmd2)->n_poly * 0x1a;
	}
	return sum;
}

/* Joint_pos_trans - 0x80028a24
   Rebuilds the root rotation of pEm and runs ko_joint_trans3 on every part.
*/
/* DS 0x80028a24 (Joint_pos_trans) */
void Joint_pos_trans(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	VECTOR v;
	UCHAR i;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	if ((pEm->Be_flg & 0x800) != 0) {
		v.vx = (LONG)pEm->Size;
		v.vy = (LONG)pEm->Size;
		v.vz = (LONG)pEm->Size;
		ScaleMatrix((MATRIX *)pEm->M, &v);
	}
	i = pEm->Parts_num;
	do {
		ko_joint_trans3(pEm, pP_ptr, pP_ptr->Be_flg, &pP_ptr->Workm);
		i--;
		pP_ptr++;
	} while (i != 0);
}

/* Parts_down - 0x80028ad8
   Physics: bounces a detached part off the floor, applies friction.
*/
/* DS 0x80028ad8 (Parts_down) */
void Parts_down(PARTS_W *pP_ptr, MATRIX *wm)
{
	ENEMY_WORK *pEm;
	MATRIX M;
	SVECTOR Sv;
	MATRIX M0;
	LONG floor_y;
	ULONG wall_hit;
	UCHAR rnd;
	MATRIX Frame_pad[2];

	(void)Frame_pad;

	pEm = G.pEm;
	if (pP_ptr->Timer3 == 0) {
		Sv.vy = pP_ptr->Old_x;
		Sv.vz = 0;
		Sv.vx = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = pP_ptr->Old_z;
		Sv.vy = pP_ptr->Old_y;
		ApplyMatrixSV(&M, &Sv, (SVECTOR *)&pP_ptr->Old_x);
		pP_ptr->Timer3 = 3;
		pP_ptr->Timer2 = 0x0f;
	}

	if (pP_ptr->Timer3 > 1) {
		floor_y = Sca_get_ground((VECTOR *)pP_ptr->Workm.t, 100, 0x2000, 1);
		wm->t[0] += (LONG)pP_ptr->Old_x;
		pP_ptr->Old_y = pP_ptr->Old_y + (SHORT)(signed char)pP_ptr->Timer1;
		wm->t[1] += (LONG)pP_ptr->Old_y;
		wm->t[2] += (LONG)pP_ptr->Old_z;

		if (floor_y < wm->t[1]) {
			pP_ptr->Timer3--;
			wm->t[1] = floor_y;
			pP_ptr->Old_y = (SHORT)(-pP_ptr->Old_y) >> 2;
			pP_ptr->Old_x >>= 2;
			pP_ptr->Old_z >>= 2;
			rnd = Rnd();
			Esp_call((pEm->Be_flg >> 0x13 & 0x18) << 0x10 | 2000,
					 (SHORT)rnd << 4,
					 &pP_ptr->Workm, NULL);
			if ((pP_ptr->Be_flg & 0x10) != 0) {
				pP_ptr->Be_flg |= 0x2000;
				pP_ptr->Size_x = 7000;
				pP_ptr->Size_z = 7000;
				pP_ptr->Size_y = 0;
				pP_ptr->Timer3 = 1;
			}
			if (pP_ptr->Timer3 == 1) {
				wm->t[1] = floor_y;
				pP_ptr->Be_flg &= 0xfffffffe;
			}
		}

		wall_hit = Sca_ck_hit((VECTOR *)pP_ptr->Workm.t, 100,
							  1 << (-(pP_ptr->Workm.t[1] / 0x708) & 0x1f),
							  0x2000);
		if (wall_hit != 0) {
			pP_ptr->Old_x >>= 2;
			pP_ptr->Old_z >>= 2;
			pP_ptr->Old_x = -pP_ptr->Old_x;
			pP_ptr->Old_z = -pP_ptr->Old_z;
		}
	}

	if ((signed char)pP_ptr->Timer2 >= 0) {
		Sv.vx = pP_ptr->Old_x2;
		Sv.vy = pP_ptr->Old_y2;
		Sv.vz = pP_ptr->Old_z2;
		RotMatrix(&Sv, &M0);
		HMatrix(&pP_ptr->Workm, &M0, &pP_ptr->Workm, (signed char)pP_ptr->Timer2 * -0x100 + 0x1000);
		pP_ptr->Timer2--;
	}
}

/* Parts_bomb - 0x80028dac
   Advances bomb-flying part physics each frame.
*/
/* DS 0x80028dac (Parts_bomb) */
void Parts_bomb(PARTS_W *pP_ptr)
{
	MATRIX M;
	SVECTOR Sv, Sv1;
	SHORT new_cnt;
	USHORT r;

	if ((pP_ptr->Free[4] & 0x7fff) < 0x1d)
		pP_ptr->Free[4]++;
	else
		pP_ptr->Be_flg = 0;

	Sv.vy = pP_ptr->Free[0];
	new_cnt = pP_ptr->Free[1] + pP_ptr->Free[3];
	pP_ptr->Free[1] = new_cnt;
	pP_ptr->Workm.t[1] += (LONG)new_cnt;
	Sv.vz = 0;
	Sv.vx = 0;
	RotMatrix(&Sv, &M);
	r = pP_ptr->Free[2] + pP_ptr->Free[6];
	pP_ptr->Free[2] = r;
	if ((int)((UINT)r << 0x10) < 0)
		pP_ptr->Free[2] = 0;
	Sv.vx = pP_ptr->Free[2];
	Sv.vy = 0;
	ApplyMatrixSV(&M, &Sv, &Sv1);
	pP_ptr->Workm.t[0] += (LONG)Sv1.vx;
	pP_ptr->Workm.t[2] += (LONG)Sv1.vz;
}

/* Parts_ryu - 0x80028ea4
   Counts down a flowing part timer and triggers its fall sequence.
*/
/* DS 0x80028ea4 (Parts_ryu) */
void Parts_ryu(PARTS_W *pP_ptr)
{
	SHORT spd, accel;

	if ((pP_ptr->Free[3] & 0x7fff) != 0) {
		pP_ptr->Free[3]--;
		spd = pP_ptr->Free[0];
		pP_ptr->Free[1] = pP_ptr->Free[1] + (SHORT)*(signed char *)((int)pP_ptr->Free + 5);
		accel = pP_ptr->Free[3];
		pP_ptr->Free[0] = spd + (signed char)pP_ptr->Free[2];
		if (accel == (SHORT)0x8000) {
			pP_ptr->Old_x  = 0;
			pP_ptr->Timer3 = 0;
			pP_ptr->Old_z  = 0;
			pP_ptr->Old_y  = 0;
			pP_ptr->Timer1 = 0x1e;
			pP_ptr->Old_x2 = 0;
			pP_ptr->Be_flg |= 0x1062;
			pP_ptr->Old_z2 = 0x400;
			pP_ptr->Old_y2 = (G.pEm)->Cdir_y;
		}
	}
}

/* HMatrix - 0x80028f48
   Interpolates between two rotation matrices pM0 and pM1 into pM2
   using GTE fixed-point lerp (Late = 0..0x1000).
   The DS build does the 3x3 weighted average in one LoadAverageShort12 step
   (verified by disassembly: weights 0x1000-Late / Late, then MatrixNormal_0);
   that step is written here as a plain loop over the nine shorts.
*/
/* DS 0x80028f48 (HMatrix) */
void HMatrix(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, ULONG Late)
{
	SVECTOR Sv0;
	/* DS: LoadAverageShort12 on m[0][0..2] (GPF12/GPL12 weighted average, writes only those three shorts) */
	gte_LoadAverageShort12((SVECTOR *)pM0->m, (SVECTOR *)pM1->m, 0x1000 - Late, Late, (SVECTOR *)pM2->m);

	gte_ldIR0((0x1000 - Late));
	gte_ldsv((SVECTOR *)&pM0->m[1][1]);
	gte_gpf12();
	gte_ldIR0(Late);
	gte_ldsv((SVECTOR *)&pM1->m[1][1]);
	gte_gpl12();
	gte_stsv((SVECTOR *)&pM2->m[1][1]);

	Sv0.vx = pM0->m[1][0];
	Sv0.vy = pM0->m[2][1];
	Sv0.vz = pM0->m[2][2];
	gte_ldIR0((0x1000 - Late));
	gte_ldsv(&Sv0);
	gte_gpf12();
	Sv0.vx = pM1->m[1][0];
	Sv0.vy = pM1->m[2][1];
	Sv0.vz = pM1->m[2][2];
	gte_ldIR0(Late);
	gte_ldsv(&Sv0);
	gte_gpl12();
	gte_stsv(&Sv0);
	pM2->m[1][0] = Sv0.vx;
	pM2->m[2][1] = Sv0.vy;
	pM2->m[2][2] = Sv0.vz;
	MatrixNormal_0(pM2, pM2);
}

/* HMatrix1 - 0x80029120
   Variant of HMatrix that also lerps the translation vector.
   Used by J_seq2 for smooth matrix interpolation.
   Translation is t[0..2] (offset 0x14), each lerped as a signed short.
*/
/* DS 0x80029120 (HMatrix1) */
void HMatrix1(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, ULONG Late)
{
	SVECTOR Sv0;
	/* DS: LoadAverageShort12 on m[0][0..2] (GPF12/GPL12 weighted average, writes only those three shorts) */
	gte_LoadAverageShort12((SVECTOR *)pM0->m, (SVECTOR *)pM1->m, 0x1000 - Late, Late, (SVECTOR *)pM2->m);

	gte_ldIR0((0x1000 - Late));
	gte_ldsv((SVECTOR *)&pM0->m[1][1]);
	gte_gpf12();
	gte_ldIR0(Late);
	gte_ldsv((SVECTOR *)&pM1->m[1][1]);
	gte_gpl12();
	gte_stsv((SVECTOR *)&pM2->m[1][1]);

	Sv0.vx = pM0->m[1][0];
	Sv0.vy = pM0->m[2][1];
	Sv0.vz = pM0->m[2][2];
	gte_ldIR0((0x1000 - Late));
	gte_ldsv(&Sv0);
	gte_gpf12();
	Sv0.vx = pM1->m[1][0];
	Sv0.vy = pM1->m[2][1];
	Sv0.vz = pM1->m[2][2];
	gte_ldIR0(Late);
	gte_ldsv(&Sv0);
	gte_gpl12();
	gte_stsv(&Sv0);
	pM2->m[1][0] = Sv0.vx;
	pM2->m[2][1] = Sv0.vy;
	pM2->m[2][2] = Sv0.vz;
	MatrixNormal_2(pM2, pM2);

	/* lerp translation (low 16 bits of each t[] word) */
	Sv0.vx = (SHORT)pM0->t[0];
	Sv0.vy = (SHORT)pM0->t[1];
	Sv0.vz = (SHORT)pM0->t[2];
	gte_ldIR0((0x1000 - Late));
	gte_ldsv(&Sv0);
	gte_gpf12();
	Sv0.vx = (SHORT)pM1->t[0];
	Sv0.vy = (SHORT)pM1->t[1];
	Sv0.vz = (SHORT)pM1->t[2];
	gte_ldIR0(Late);
	gte_ldsv(&Sv0);
	gte_gpl12();
	gte_stsv(&Sv0);
	pM2->t[0] = (LONG)Sv0.vx;
	pM2->t[1] = (LONG)Sv0.vy;
	pM2->t[2] = (LONG)Sv0.vz;
}

/* Model_color_plus - 0x8002940c
   Adds tinted highlight colour to GT3/GT4 primitive packets in-place.
   pMw = model work block, Pluscolor_1 = base colour, Pluscolor_2 = blend colour.
*/
/* DS 0x8002940c (Model_color_plus) */
void Model_color_plus(MODEL_WORK *pMw, ULONG Pluscolor_1, ULONG Pluscolor_2)
{
	ULONG n;
	UCHAR *pgt3, *pgt4;
	UCHAR g = (UCHAR)(Pluscolor_1 >> 8);
	UCHAR b = (UCHAR)(Pluscolor_1 >> 16);

	n = ((TMD_MEM *)pMw->pTmd)->n_poly;
	if (n != 0) {
		pgt3 = (UCHAR *)((int)pMw->pPacket + (UINT)G.Cbuf_id * 0x28 + 0x1e);
		do {
			n--;
			pgt3[-0xe] = (UCHAR)Pluscolor_1;
			pgt3[-0xd] = g;
			pgt3[-0xc] = b;
			pgt3[-2]   = (UCHAR)(((UINT)pgt3[-2]   + (Pluscolor_2 & 0xff))      >> 1);
			pgt3[-1]   = (UCHAR)(((UINT)pgt3[-1]   + (Pluscolor_2 >> 8 & 0xff)) >> 1);
			pgt3[0]    = (UCHAR)(((UINT)pgt3[0]    + (Pluscolor_2 >> 16 & 0xff))>> 1);
			pgt3 += 0x50;
		} while (n != 0);
	}

	n = ((TMD_MEM *)pMw->pTmd2)->n_poly;
	if (n != 0) {
		pgt4 = (UCHAR *)((int)pMw->pPacket2 + (UINT)G.Cbuf_id * 0x34 + 0x2a);
		do {
			n--;
			pgt4[-2]    = (UCHAR)Pluscolor_1;
			pgt4[-1]    = g;
			pgt4[0]     = b;
			pgt4[-0x1a] = (UCHAR)(((UINT)pgt4[-0x1a] + (Pluscolor_1 & 0xff))       >> 1);
			pgt4[-0x19] = (UCHAR)(((UINT)pgt4[-0x19] + (Pluscolor_1 >> 8 & 0xff))  >> 1);
			pgt4[-0x18] = (UCHAR)(((UINT)pgt4[-0x18] + (Pluscolor_1 >> 16 & 0xff)) >> 1);
			pgt4[-0xe]  = (UCHAR)(((UINT)pgt4[-0xe]  + (Pluscolor_2 & 0xff))        >> 1);
			pgt4[-0xd]  = (UCHAR)(((UINT)pgt4[-0xd]  + (Pluscolor_2 >> 8 & 0xff))  >> 1);
			pgt4[-0xc]  = (UCHAR)(((UINT)pgt4[-0xc]  + (Pluscolor_2 >> 16 & 0xff)) >> 1);
			pgt4 += 0x68;
		} while (n != 0);
	}
}

/* Joint_move - 0x8002959c
   Looks up the current animation frame pointer for pEm and calls J_seq.
*/
/* DS 0x8002959c (Joint_move) */
ULONG Joint_move(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
	ULONG f_num;
	ULONG *seq_ptr;
	UINT move_no;

	f_num = *(UINT *)(Seq_t_ptr + (UINT)pEm->Move_no * 4);
	if ((Late_flg & 0xffff0000) == 0)
		move_no = (UINT)pEm->Move_cnt;
	else
		move_no = ((f_num & 0xffff) - (UINT)pEm->Move_cnt) - 1;

	seq_ptr = (ULONG *)(Seq_t_ptr + (f_num >> 0x10) + move_no * 4);
	pEm->pNow_seq = seq_ptr;
	return J_seq(Kan_t_ptr, Seq_t_ptr, seq_ptr, Late_flg);
}

/* J_seq - 0x80029614
   Core joint animation sequencer. Unpacks 12-bit packed rotation data
   into PARTS_W rotation fields, optionally lerps with previous frame
   (Hokan interpolation), builds rotation matrices, advances Move_cnt.
   Returns 1 when the animation cycle completes.
*/
/* DS 0x80029614 (J_seq) */
ULONG J_seq(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG *pSeq_ptr, ULONG Late_flg)
{
	ENEMY_WORK *pEm;
	PARTS_W *pP_base;
	PARTS_W *pP_walk;
	int *pFrame;
	UINT *pRot_w;
	UINT *pRot_w1;
	UINT *pRot_w2;
	UINT Pk_w;
	UINT Dif_x;
	UINT Hk_flg;
	UINT Parts_cnt;
	UINT Parts_left;
	UINT Mv_frames;
	INT Ipol_w;
	SHORT Step_lim;
	UCHAR Mv_cnt_nx;
	BOOL Mv_end;
	SHORT *r0_00;
	VECTOR *r0;
	VECTOR TV0l;
	MATRIX Frame_pad[2];

	(void)Frame_pad;
	pEm = G.pEm;
	pFrame = (int *)(Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs +
					 (*(UINT *)&((KAN_HEAD *)Kan_t_ptr)->Parts_cnt >> 0x12) * (*pSeq_ptr & 0xfff) * 4);
	Ipol_w = ((KAN_FRAME *)pFrame)->Pos_y * (int)pEm->Parts0_pos_y;
	pP_base = pEm->pSin_parts_ptr;
	Mv_frames = (UINT)*(USHORT *)((UINT)pEm->Move_no * 4 + Seq_t_ptr);
	pP_base->Pos_x = (int)((KAN_FRAME *)pFrame)->Pos_x;
	pP_base->Pos_y = Ipol_w >> 0xc;
	pP_base->Pos_z = (int)((KAN_FRAME *)pFrame)->Pos_z;
	Parts_cnt = (UINT)((KAN_HEAD *)Kan_t_ptr)->Parts_cnt;
	Hk_flg = (UINT)pEm->Hokan_flg;
	pP_walk = pP_base;
	Parts_left = Parts_cnt;
	if (Hk_flg != 0) {
		Ipol_w = (Late_flg & 0xffff) * Hk_flg;
		TV0l.vx = (LONG)pP_base->Old_x;
		TV0l.vy = (LONG)pP_base->Old_y;
		TV0l.vz = (LONG)pP_base->Old_z;
		gte_ldIR0(0x1000 - Ipol_w);
		r0 = (VECTOR *)&pP_base->Pos_x;
		gte_ldlvl(r0);
		gte_gpf12();
		gte_ldIR0(Ipol_w);
		gte_ldlvl(&TV0l);
		gte_gpl12();
		gte_stlvl(r0);
	}
	pRot_w = (UINT *)&((KAN_FRAME *)pFrame)->Unk_0c;
	pP_walk->Old_x = (SHORT)pP_walk->Pos_x;
	pP_walk->Old_y = (SHORT)pP_walk->Pos_y;
	pP_walk->Old_z = (SHORT)pP_walk->Pos_z;
	do {
		Pk_w = (*pRot_w & 0xfff) | ((*pRot_w & 0xfff000) << 4);
		pP_walk->Rot_x = (SHORT)Pk_w;
		pP_walk->Rot_y = (SHORT)(Pk_w >> 0x10);
		pRot_w1 = pRot_w + 1;
		pP_walk->Rot_z = (USHORT)((KAN_ROT_LANES *)pRot_w)->Unk_03 | (USHORT)((*pRot_w1 & 0xf) << 8);
		if (Parts_left == 1) {
			break;
		}
		Pk_w = ((*pRot_w1 & 0xfff0) >> 4) | (*pRot_w1 & 0xfff0000);
		pP_walk[1].Rot_x = (SHORT)Pk_w;
		pP_walk[1].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[1].Rot_z = (USHORT)(*pRot_w1 >> 0x1c) | (USHORT)(UCHAR)pRot_w[2] << 4;
		if (Parts_left == 2) {
			break;
		}
		Pk_w = pRot_w[2];
		Pk_w = ((Pk_w & 0xfff00) >> 8) | ((Pk_w & 0xfff00000) >> 4);
		pP_walk[2].Rot_x = (SHORT)Pk_w;
		pP_walk[2].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[2].Rot_z = (USHORT)pRot_w[3] & 0xfff;
		if (Parts_left == 3) {
			break;
		}
		Pk_w = pRot_w[3];
		pRot_w1 = pRot_w + 4;
		Pk_w = ((Pk_w & 0xfff000) >> 0xc) | ((Pk_w & 0xff000000) >> 8) | ((*pRot_w1 & 0xf) << 0x18);
		pP_walk[3].Rot_x = (SHORT)Pk_w;
		pP_walk[3].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[3].Rot_z = (SHORT)((*pRot_w1 & 0xfff0) >> 4);
		if (Parts_left == 4) {
			break;
		}
		pRot_w2 = pRot_w + 5;
		Pk_w = ((*pRot_w1 & 0xfff0000) >> 0x10) | ((*pRot_w1 & 0xf0000000) >> 0xc) | ((UINT)(UCHAR)*pRot_w2 << 0x14);
		pP_walk[4].Rot_x = (SHORT)Pk_w;
		pP_walk[4].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[4].Rot_z = (USHORT)(*pRot_w2 >> 8) & 0xfff;
		if (Parts_left == 5) {
			break;
		}
		Pk_w = (*pRot_w2 >> 0x14) | ((pRot_w[6] & 0xfff) << 0x10);
		pP_walk[5].Rot_x = (SHORT)Pk_w;
		pP_walk[5].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[5].Rot_z = (SHORT)((pRot_w[6] & 0xfff000) >> 0xc);
		if (Parts_left == 6) {
			break;
		}
		pRot_w1 = pRot_w + 7;
		Pk_w = (UINT)((KAN_ROT_LANES *)&pRot_w[6])->Unk_03 | ((*pRot_w1 & 0xf) << 8) | ((*pRot_w1 & 0xfff0) << 0xc);
		pP_walk[6].Rot_x = (SHORT)Pk_w;
		pP_walk[6].Rot_y = (SHORT)(Pk_w >> 0x10);
		pP_walk[6].Rot_z = (USHORT)(*pRot_w1 >> 0x10) & 0xfff;
		if (Parts_left == 7) {
			break;
		}
		pRot_w2 = pRot_w + 8;
		Parts_left -= 8;
		Pk_w = *pRot_w2;
		Pk_w = (*pRot_w1 >> 0x1c) | ((Pk_w & 0xff) << 4) | ((Pk_w & 0xfff00) << 8);
		pP_walk[7].Rot_x = (SHORT)Pk_w;
		pP_walk[7].Rot_y = (SHORT)(Pk_w >> 0x10);
		pRot_w += 9;
		pP_walk[7].Rot_z = (USHORT)(*pRot_w2 >> 0x14);
		pP_walk += 8;
	} while (Parts_left != 0);

	pP_walk = pP_base;
	Parts_left = Parts_cnt;
	if (Hk_flg == 0) {
		do {
			Parts_cnt--;
			pP_base->Cdir_x = pP_base->Rot_x;
			pP_base->Cdir_y = pP_base->Rot_y;
			pP_base->Cdir_z = pP_base->Rot_z;
			pP_base++;
		} while (Parts_cnt != 0);
	} else {
		pEm->Hokan_flg += 0xff;
		do {
			Hk_flg = (USHORT)pP_base->Cdir_x | ((UINT)(USHORT)pP_base->Cdir_y << 16);
			Pk_w = (USHORT)pP_base->Cdir_z & 0xfff;
			pP_base->Cdir_x = (SHORT)(Hk_flg & 0xfff0fff);
			pP_base->Cdir_y = (SHORT)((Hk_flg & 0xfff0fff) >> 0x10);
			pP_base->Cdir_z = (SHORT)Pk_w;
			gte_ldIR0(Ipol_w);
			gte_ldsv((SVECTOR *)&pP_base->Cdir_x);
			Dif_x = ((int)pP_base->Rot_x - (int)pP_base->Cdir_x) + 0x800;
			Hk_flg = ((int)pP_base->Rot_y - (int)pP_base->Cdir_y) + 0x800;
			Pk_w = ((int)pP_base->Rot_z - Pk_w) + 0x800;
			gte_gpf12_b();
			if (0x1000 < Dif_x) {
				Step_lim = 0x1000;
				if ((Dif_x & 0x8000) == 0) {
					Step_lim = -0x1000;
				}
				pP_base->Rot_x += Step_lim;
			}
			if (0x1000 < Hk_flg) {
				Step_lim = 0x1000;
				if ((Hk_flg & 0x8000) == 0) {
					Step_lim = -0x1000;
				}
				pP_base->Rot_y += Step_lim;
			}
			if (0x1000 < Pk_w) {
				Step_lim = 0x1000;
				if ((Pk_w & 0x8000) == 0) {
					Step_lim = -0x1000;
				}
				pP_base->Rot_z += Step_lim;
			}
			gte_ldIR0(0x1000 - Ipol_w);
			gte_ldsv((SVECTOR *)&pP_base->Rot_x);
			r0_00 = &pP_base->Cdir_x;
			Parts_cnt--;
			gte_gpl12_b();
			pP_base++;
			gte_stsv((SVECTOR *)r0_00);
		} while (Parts_cnt != 0);
	}
	do {
		if ((pP_walk->Be_flg & 0x200) == 0) {
			RotMatrix((SVECTOR *)&pP_walk->Cdir_x, (MATRIX *)pP_walk->M);
		}
		Parts_left--;
		pP_walk++;
	} while (Parts_left != 0);
	Mv_cnt_nx = pEm->Move_cnt + 1;
	pEm->Move_cnt = Mv_cnt_nx;
	Mv_end = (Mv_frames <= Mv_cnt_nx);
	if (Mv_end) {
		pEm->Move_cnt = 0;
	}
	return (UINT)Mv_end;
}

/* hokan_svec - 0x80029b70
   GTE-assisted short-vector interpolation: lerps sv0 toward sv1,
   writes result into sv2, with clamped max step of 0x1000.
*/
/* DS 0x80029b70 (hokan_svec) */
void hokan_svec(SVECTOR *sv0, SVECTOR *sv1, SVECTOR *sv2, ULONG late)
{
	ULONG vx, vy, vz;
	SHORT clamp;

	/* mask sv0 to 12-bit signed */
	{
		ULONG tmp = (USHORT)sv0->vx | ((UINT)(USHORT)sv0->vy << 16);
		ULONG tmpz = (USHORT)sv0->vz & 0xfff;
		sv0->vx = (SHORT)(tmp & 0xfff0fff);
		sv0->vy = (SHORT)((tmp & 0xfff0fff) >> 0x10);
		sv0->vz = (SHORT)tmpz;
		vx = ((int)sv1->vx - (int)sv0->vx) + 0x800;
		vz = ((int)sv1->vz - tmpz) + 0x800;
		vy = ((int)sv1->vy - (int)sv0->vy) + 0x800;
	}

	if (0x1000 < vx) {
		clamp = (vx & 0x8000) == 0 ? -0x1000 : 0x1000;
		sv1->vx += clamp;
	}
	if (0x1000 < vy) {
		clamp = (vy & 0x8000) == 0 ? -0x1000 : 0x1000;
		sv1->vy += clamp;
	}
	if (0x1000 < vz) {
		clamp = (vz & 0x8000) == 0 ? -0x1000 : 0x1000;
		sv1->vz += clamp;
	}

	gte_ldIR0(0x1000 - late);
	gte_ldsv(sv0);
	gte_gpf12_b();
	gte_ldIR0(late);
	gte_ldsv(sv1);
	gte_gpl12();
	gte_stsv(sv2);
}

/* Pl_set_packet - 0x80029c94
   Re-presets the player's part packets. Part 11 is borrowed to point at the
   gpPl_* buffers while every part is re-presetted, then restored.
   Player Id 0/2 also gets a Texture_Move when Common_flg bits 0x3e are set.
   Globals used (gpPl_tmd_ptr, gpPl_tmd2_ptr, gpPl_pac_ptr, gpPl_pac2_ptr) are
   owned by the player module and are not declared here.
*/
/* DS 0x80029c94 (Pl_set_packet) */
void Pl_set_packet(void)
{
	PARTS_W *pPart;
	MODEL_WORK *pMw;
	ULONG *Save_tmd, *Save_tmd2, *Save_pac, *Save_pac2;
	UINT i;

	if (G.Player.pTmd != NULL) {
		pPart = G.Player.pSin_parts_ptr;
		Save_tmd  = pPart[0xb].pTmd;
		Save_tmd2  = pPart[0xb].pTmd2;
		Save_pac = pPart[0xb].pPacket;
		Save_pac2 = pPart[0xb].pPacket2;
		pPart[0xb].pTmd     = (ULONG *)gpPl_tmd_ptr;
		pPart[0xb].pTmd2    = (ULONG *)gpPl_tmd2_ptr;
		pPart[0xb].pPacket  = (ULONG *)gpPl_pac_ptr;
		pPart[0xb].pPacket2 = (ULONG *)gpPl_pac2_ptr;
		for (i = 0; i < G.Player.Parts_num; i++) {
			pMw = (MODEL_WORK *)&pPart[i].Attribute;
			PresetObjectGT3_neo(pMw, pPart[i].pPacket, 1);
			PresetObjectGT4_neo(pMw, pPart[i].pPacket2, 1);
		}
		pPart = G.Player.pSin_parts_ptr;
		pPart[0xb].pTmd     = Save_tmd;
		pPart[0xb].pTmd2    = Save_tmd2;
		pPart[0xb].pPacket  = Save_pac;
		pPart[0xb].pPacket2 = Save_pac2;
		if ((G.Player.Id == 0 || G.Player.Id == 2) && Flg_ck(G.SaveData.Common_flg, 0x3e) != 0) {
			Texture_Move((MODEL_WORK *)&G.Player.pSin_parts_ptr->Attribute,
						 G.Player.Tpage + 2, 0, G.Player.Clut + 0x1e2);
		}
	}
}

/* Joint_move_p - 0x80029e10
   Player-side motion sequencer (JOINT_P variant). Decodes the frame's
   rotation data for the parts selected by On_parts_flg, plus the root
   position into P0_m / P0_sv, interpolating against the previous frame
   when Hokan_flg is set. Returns 1 when the cycle completes.
   JOINT_P field offsets are inferred from the DS code (see JOINT.H).
*/
/* DS 0x80029e10 (Joint_move_p) */
ULONG Joint_move_p(JOINT_P *pJp, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
	UINT   f_num;
	UINT   sel;
	UINT   mask;
	UINT   seq_word;
	UINT  *pRot_w;
	UINT   Prt_mask, Pk_w, Parts_left, On_bits, Parts_cnt;
	int    Fr_adr, Rot_adr, Ipol_inv;
	ULONG  Ipol_w;
	UINT   Hk_flg;
	UINT   Mv_frames;
	UCHAR  cnt;
	PARTS_W *pP_cur;
	PARTS_W *pGrp;
	MATRIX *pMtx;
	SVECTOR *r0;
	SHORT   Cur_v, Step_z;
	UINT    flg;
	UINT    cdir_y;
	UINT    dx, dy, dz;
	ULONG   Frame_pad[6];		/* retail never reads 0x10-0x1f, 0x24, 0x2c, 0x34 (its spills are 0x20, 0x28, 0x30, 0x38, 0x3c) */

	(void)Frame_pad;
	f_num = *(UINT *)(Seq_t_ptr + (UINT)pJp->Move_no * 4);
	if ((Late_flg & 0xffff0000) == 0)
		sel = (UINT)pJp->Move_cnt;
	else
		sel = ((f_num & 0xffff) - (UINT)pJp->Move_cnt) - 1;

	seq_word = *(UINT *)(Seq_t_ptr + (f_num >> 0x10) + sel * 4);
	Fr_adr = Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs +
			(*(UINT *)&((KAN_HEAD *)Kan_t_ptr)->Parts_cnt >> 0x12) * (seq_word & 0xfff) * 4;

	mask    = 0xfff0000;
	pRot_w  = (UINT *)&((KAN_FRAME *)Fr_adr)->Unk_0c;
	Hk_flg = (UINT)pJp->Hokan_flg;
	Rot_adr  = (int)&((KAN_FRAME *)Fr_adr)->Unk_08;
	Prt_mask   = (UINT)pJp->P0_parts_no;
	On_bits  = pJp->On_parts_flg;
	pP_cur = G.pEm->pSin_parts_ptr;
	pGrp = pP_cur;
	Mv_frames = (UINT)*(USHORT *)((UINT)pJp->Move_no * 4 + Seq_t_ptr);
	pJp->P0_m.t[1] = (LONG)((int)((KAN_FRAME *)Fr_adr)->Pos_y * (int)G.pEm->Parts0_pos_y >> 12);
	Parts_cnt = (UINT)((KAN_HEAD *)Kan_t_ptr)->Parts_cnt;
	Ipol_w = Late_flg & 0xffff;
	Parts_left = Parts_cnt;

	do {
		if ((On_bits & 1) != 0) {
			*(UINT *)&pGrp[0].Rot_x = (*pRot_w & 0xfff) | (*pRot_w & 0xfff000) << 4;
			pGrp[0].Rot_z = (USHORT)((KAN_ROT_LANES *)pRot_w)->Unk_03 | (USHORT)((((KAN_ROT_GRP *)Rot_adr)->Unk_08 & 0xf) << 8);
		}
		if (Prt_mask == 0) {
			Prt_mask = (*pRot_w & 0xfff) | (*pRot_w & 0xfff000) << 4;
			pJp->P0_sv.vx = (SHORT)Prt_mask;
			pJp->P0_sv.vy = (SHORT)(Prt_mask >> 0x10);
			pJp->P0_sv.vz = (USHORT)((KAN_ROT_LANES *)pRot_w)->Unk_03 | (USHORT)((((KAN_ROT_GRP *)Rot_adr)->Unk_08 & 0xf) << 8);
			flg = 0xffffffff;
			RotMatrix(&pJp->P0_sv, &pJp->P0_m);
			Prt_mask = flg;
		}
		if (Parts_left == 1) break;
		if ((On_bits >> 1 & 1) != 0) {
			Pk_w = pRot_w[1];
			*(UINT *)&pGrp[1].Rot_x = (Pk_w & 0xfff0) >> 4 | (Pk_w & mask);
			pGrp[1].Rot_z = (USHORT)(pRot_w[1] >> 0x1c) | (USHORT)((KAN_ROT_LANES *)&((KAN_ROT_GRP *)Rot_adr)->Unk_0c)->Unk_00 << 4;
		}
		if (Parts_left == 2) break;
		if ((On_bits >> 2 & 1) != 0) {
			*(UINT *)&pGrp[2].Rot_x = ((pRot_w[2] << 0xc) >> 0x14) | ((pRot_w[2] & 0xfff00000) >> 4);
			pGrp[2].Rot_z = (USHORT)((KAN_ROT_GRP *)Rot_adr)->Unk_10 & 0xfff;
		}
		if (Parts_left == 3) break;
		if ((On_bits >> 3 & 1) != 0) {
			*(UINT *)&pGrp[3].Rot_x =
				((pRot_w[3] & 0xfff000) >> 0xc) |
				((pRot_w[3] & 0xff000000) >> 8) | ((((KAN_ROT_GRP *)Rot_adr)->Unk_14 & 0xf) << 0x18);
			pGrp[3].Rot_z = (SHORT)((((KAN_ROT_GRP *)Rot_adr)->Unk_14 & 0xfff0) >> 4);
		}
		if (Parts_left == 4) break;
		if ((On_bits >> 4 & 1) != 0) {
			*(UINT *)&pGrp[4].Rot_x =
				((int)(pRot_w[4] & mask) >> 0x10) |
				((pRot_w[4] & 0xf0000000) >> 0xc) | ((UINT)((KAN_ROT_LANES *)&((KAN_ROT_GRP *)Rot_adr)->Unk_18)->Unk_00 << 0x14);
			pGrp[4].Rot_z = (USHORT)(((UINT)(((KAN_ROT_GRP *)Rot_adr)->Unk_18 << 0xc)) >> 0x14);
		}
		if (Parts_left == 5) break;
		if ((On_bits >> 5 & 1) != 0) {
			*(UINT *)&pGrp[5].Rot_x = (pRot_w[5] >> 0x14) | ((((KAN_ROT_GRP *)Rot_adr)->Unk_1c & 0xfff) << 0x10);
			pGrp[5].Rot_z = (SHORT)((((KAN_ROT_GRP *)Rot_adr)->Unk_1c & 0xfff000) >> 0xc);
		}
		if (Parts_left == 6) break;
		if ((On_bits >> 6 & 1) != 0) {
			Pk_w = pRot_w[7];
			*(UINT *)&pGrp[6].Rot_x = ((UINT)((KAN_ROT_LANES *)&((KAN_ROT_GRP *)Rot_adr)->Unk_1c)->Unk_03) | ((Pk_w & 0xf) << 8) | ((Pk_w & 0xfff0) << 0xc);
			pGrp[6].Rot_z = (USHORT)(pRot_w[7] >> 0x10) & (USHORT)(mask >> 0x10);
		}
		if (Parts_left == 7) break;
		if ((On_bits >> 7 & 1) != 0) {
			Pk_w = pRot_w[8];
			*(UINT *)&pGrp[7].Rot_x = (((KAN_ROT_GRP *)Rot_adr)->Unk_20 >> 0x1c) | ((Pk_w & 0xff) << 4) | ((Pk_w & 0xfff00) << 8);
			pGrp[7].Rot_z = (USHORT)(pRot_w[8] >> 0x14);
		}
		On_bits >>= 8;
		Rot_adr = Rot_adr + 9 * sizeof(UINT);
		pRot_w += 9;
		Parts_left -= 8;
		pGrp += 8;
	} while (Parts_left != 0);

	Prt_mask = pJp->On_parts_flg;
	if (Hk_flg == 0) {
		pMtx = (MATRIX *)pP_cur->M;
		do {
			if ((Prt_mask & 1) != 0) {
				/* carry the current values into pMtx[2] before rebuilding the matrix */
				SHORT cur_y = pMtx[3].m[1][1];
				*(UINT *)&pMtx[2].m[2][2] = *(UINT *)&pMtx[3].m[0][2];
				*(SHORT *)pMtx[2].t = cur_y;
				RotMatrix((SVECTOR *)&pP_cur->Rot_x, pMtx);
			}
			Prt_mask >>= 1;
			pMtx = (MATRIX *)pMtx[5].m[2];
			Parts_cnt--;
			pP_cur++;
		} while (Parts_cnt != 0);
	}
	else {
		pMtx = (MATRIX *)pP_cur->M;
		r0 = (SVECTOR *)&pP_cur->Cdir_x;
		Ipol_inv = 0x1000 - (int)Ipol_w;
		do {
			if ((Prt_mask & 1) != 0) {
				/* hokan: interpolate the stored Cdir toward the new Rot.
				   The stored Cdir fields are 12-bit: mask them before taking deltas. */
				cdir_y = (USHORT)pMtx[2].t[0] & 0xfff;
				*(UINT *)&pMtx[2].m[2][2] &= 0xfff0fff;
				*(SHORT *)pMtx[2].t = (SHORT)cdir_y;
				gte_ldIR0((int)Ipol_w);
				gte_ldsv(r0);
				Cur_v = pMtx[3].m[0][2];
				dz = ((int)Cur_v - (int)pMtx[2].m[2][2]) + 0x800;
				dy = ((int)pMtx[3].m[1][1] - (int)*(short *)pMtx[2].t) + 0x800;
				dx = ((int)pMtx[3].m[1][0] - (int)pP_cur->Cdir_y) + 0x800;
				if (0x1000 < dz) {
					Step_z = (dz & 0x8000) == 0 ? -0x1000 : 0x1000;
					pMtx[3].m[0][2] = Cur_v + Step_z;
				}
				if (0x1000 < dx) {
					Cur_v = (dx & 0x8000) == 0 ? -0x1000 : 0x1000;
					pMtx[3].m[1][0] += Cur_v;
				}
				if (0x1000 < dy) {
					Cur_v = (dy & 0x8000) == 0 ? -0x1000 : 0x1000;
					pMtx[3].m[1][1] += Cur_v;
				}
				gte_gpf12_b();
				gte_ldIR0(Ipol_inv);
				gte_ldsv((SVECTOR *)&pP_cur->Rot_x);
				gte_gpl12();
				gte_stsv(r0);
				RotMatrix(r0, pMtx);
			}
			Prt_mask >>= 1;
			pMtx = (MATRIX *)pMtx[5].m[2];
			r0 = (SVECTOR *)&r0[0x15].vz;
			pP_cur++;
			Parts_cnt--;
			pJp->Hokan_flg += 0xff;
		} while (Parts_cnt != 0);
	}

	cnt = pJp->Move_cnt + 1;
	pJp->Move_cnt = cnt;
	if (Mv_frames <= (UINT)cnt) {
		pJp->Move_cnt = 0;
		return 1;
	}
	return 0;
}

/* Joint_move2 - 0x8002a490
   Same as Joint_move but calls J_seq2 (uses MatrixNormal_2 branch).
*/
/* DS 0x8002a490 (Joint_move2) */
ULONG Joint_move2(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
	ULONG f_num;
	ULONG *seq_ptr;
	UINT move_no;

	f_num = *(UINT *)(Seq_t_ptr + (UINT)pEm->Move_no * 4);
	if ((Late_flg & 0xffff0000) == 0)
		move_no = (UINT)pEm->Move_cnt;
	else
		move_no = ((f_num & 0xffff) - (UINT)pEm->Move_cnt) - 1;

	seq_ptr = (ULONG *)(Seq_t_ptr + (f_num >> 0x10) + move_no * 4);
	pEm->pNow_seq = seq_ptr;
	return J_seq2(Kan_t_ptr, Seq_t_ptr, seq_ptr, Late_flg);
}

/* J_seq2 - 0x8002a508
   Variant of J_seq for player-type models. Unpacks 12-bit rotations
   directly into PARTS_W->Rot fields and calls MatrixNormal_2. When
   hokan is active, uses HMatrix for matrix-space interpolation.
   Returns 1 on animation cycle completion.
   Parts are 0xac bytes apart, so M advances with pP_ptr.
*/
/* 20-byte copy of the rotation part (3x3 shorts plus pad) of a MATRIX */
typedef struct { LONG w[5]; } MAT3_COPY;

/* DS 0x8002a508 (J_seq2) */
ULONG J_seq2(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG *pSeq_ptr, ULONG Late_flg)
{
	ENEMY_WORK *pEm;
	PARTS_W *pP_ptr;
	int *pFrame;
	UINT *pRot_w;
	short *pRot_s;
	int Parts_left, Ipol_w;
	ULONG Pk_w, Hk_flg, Parts_cnt, Mv_frames;
	MATRIX *pM2;
	ULONG TV0_old[3];   /* Old_x/y/z save */
	MATRIX M0;
	MATRIX M1;
	SHORT *pRot;
	ULONG Frame_pad[30];		/* retail reserves 0x100 bytes; only the low locals are used */

	(void)Frame_pad;
	pEm = G.pEm;
	pFrame = (int *)(Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs +
					 (*(UINT *)&((KAN_HEAD *)Kan_t_ptr)->Parts_cnt >> 0x12) * (*pSeq_ptr & 0xfff) * 4);
	Ipol_w = ((KAN_FRAME *)pFrame)->Pos_y * (int)pEm->Parts0_pos_y;
	pP_ptr = pEm->pSin_parts_ptr;
	Mv_frames = (UINT)*(USHORT *)((UINT)pEm->Move_no * 4 + Seq_t_ptr);
	pP_ptr->Pos_x = (int)((KAN_FRAME *)pFrame)->Pos_x;
	pP_ptr->Pos_y = Ipol_w >> 0xc;
	pP_ptr->Pos_z = (int)((KAN_FRAME *)pFrame)->Pos_z;
	Hk_flg = (UINT)pEm->Hokan_flg;
	Parts_cnt = (UINT)((KAN_HEAD *)Kan_t_ptr)->Parts_cnt;

	if (Hk_flg != 0) {
		Ipol_w = (Late_flg & 0xffff) * Hk_flg;
		TV0_old[0] = (int)pP_ptr->Old_x;
		TV0_old[1] = (int)pP_ptr->Old_y;
		TV0_old[2] = (int)pP_ptr->Old_z;
		gte_ldIR0(0x1000 - Ipol_w);
		gte_ldlvl((VECTOR *)&pP_ptr->Pos_x);
		gte_gpf12();
		gte_ldIR0(Ipol_w);
		gte_ldlvl((VECTOR *)TV0_old);
		gte_gpl12();
		gte_stlvl((VECTOR *)&pP_ptr->Pos_x);
	}

	pRot_s = &pP_ptr[1].Rot_z;
	pP_ptr->Old_x = (SHORT)pP_ptr->Pos_x;
	pP_ptr->Old_y = (SHORT)pP_ptr->Pos_y;
	pP_ptr->Old_z = (SHORT)pP_ptr->Pos_z;

	/* first part rotation from the KAN_FRAME Unk_0c word */
	Pk_w = (((KAN_FRAME *)pFrame)->Unk_0c & 0xfffU) | (((KAN_FRAME *)pFrame)->Unk_0c & 0xfff000U) << 4;
	pP_ptr->Rot_x = (SHORT)Pk_w;
	pP_ptr->Rot_y = (SHORT)(Pk_w >> 0x10);
	pRot_w = (UINT *)&pFrame[4];
	pP_ptr->Rot_z = (USHORT)((KAN_ROT_LANES *)&((KAN_FRAME *)pFrame)->Unk_0c)->Unk_03 | (USHORT)((*pRot_w & 0xf) << 8);

	/* DS unpacks 8 parts (9 words) per pass: pRot_w advances word by word and pRot_s by 0x56 shorts per part;
	   Parts_left is counted down after every part. */
	for (Parts_left = Parts_cnt - 1; Parts_left != 0; ) {
		*(UINT *)(pRot_s - 2) = (*pRot_w & 0xfff0) >> 4 | (*pRot_w & 0xfff0000);
		Pk_w = *pRot_w++;
		*pRot_s = (USHORT)(Pk_w >> 0x1c) | (USHORT)(UCHAR)*pRot_w << 4;
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = *pRot_w++;
		*(UINT *)(pRot_s - 2) = (Pk_w & 0xfff00) >> 8 | (Pk_w & 0xfff00000) >> 4;
		*pRot_s = (USHORT)*pRot_w & 0xfff;
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = *pRot_w++;
		*(UINT *)(pRot_s - 2) = (Pk_w & 0xfff000) >> 0xc | (Pk_w & 0xff000000) >> 8 | (*pRot_w & 0xf) << 0x18;
		*pRot_s = (SHORT)((*pRot_w & 0xfff0) >> 4);
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = *pRot_w++;
		*(UINT *)(pRot_s - 2) = (int)(Pk_w & 0xfff0000) >> 0x10 | (Pk_w & 0xf0000000) >> 0xc |
								(UINT)(UCHAR)*pRot_w << 0x14;
		*pRot_s = (USHORT)((*pRot_w & 0xfff00) >> 8);
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = *pRot_w++;
		*(UINT *)(pRot_s - 2) = Pk_w >> 0x14 | (*pRot_w & 0xfff) << 0x10;
		*pRot_s = (SHORT)((*pRot_w & 0xfff000) >> 0xc);
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = (UINT)((KAN_ROT_LANES *)pRot_w)->Unk_03;
		pRot_w++;
		*(UINT *)(pRot_s - 2) = Pk_w | (*pRot_w & 0xf) << 8 | (*pRot_w & 0xfff0) << 0xc;
		*pRot_s = (USHORT)((*pRot_w & 0xfff0000) >> 0x10);
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		Pk_w = *pRot_w++;
		*(UINT *)(pRot_s - 2) = Pk_w >> 0x1c | (*pRot_w & 0xff) << 4 | (*pRot_w & 0xfff00) << 8;
		*pRot_s = (USHORT)(*pRot_w++ >> 0x14);
		pRot_s += 0x56;
		if (--Parts_left == 0) break;
		*(UINT *)(pRot_s - 2) = (*pRot_w & 0xfff) | (*pRot_w & 0xfff000) << 4;
		Pk_w = (UINT)((KAN_ROT_LANES *)pRot_w)->Unk_03;
		pRot_w++;
		*pRot_s = (USHORT)(Pk_w | (*pRot_w & 0xf) << 8);
		pRot_s += 0x56;
		Parts_left--;
	}

	if (Hk_flg == 0) {
		do {
			RotMatrix((SVECTOR *)&pP_ptr->Rot_x, (MATRIX *)pP_ptr->M);
			MatrixNormal_2((MATRIX *)pP_ptr->M, (MATRIX *)pP_ptr->M);
			Parts_cnt--;
			pP_ptr++;
		} while (Parts_cnt != 0);
	}
	else {
		pM2 = (MATRIX *)pP_ptr->M;
		pEm->Hokan_flg += 0xff;
		do {
			/* M0 = previous rotation: DS copies the 3x3 shorts as five words (lw/sw), not a MATRIX copy */
			*(MAT3_COPY *)&M0 = *(MAT3_COPY *)pM2;
			pRot = &pP_ptr->Rot_x;
			pP_ptr++;
			Parts_cnt--;
			/* M1 = new rotation for the same part */
			RotMatrix((SVECTOR *)pRot, &M1);
			HMatrix(&M0, &M1, pM2, 0xfff - Ipol_w);
			pM2 = (MATRIX *)pP_ptr->M;
		} while (Parts_cnt != 0);
	}

	{
		UCHAR cnt = pEm->Move_cnt + 1;
		pEm->Move_cnt = cnt;
		if (Mv_frames <= cnt) {
			pEm->Move_cnt = 0;
			return 1;
		}
		return 0;
	}
}
