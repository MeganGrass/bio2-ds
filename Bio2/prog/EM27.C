/* em27.c - reconstruction of D:\Bio2\prog\EM27.C (enemy 27).
 * Linked into two overlays from the same source: 0x0C (at 0x801000e0) and
 * 0x2A (at 0x8010d0e0); only the enemy-number-specific entry names differ
 * (Em127/Em227, Pldamege_Em127/227, Pldie_Em127/227), selected by OVL_2A.
 * Types come from the TU's retail SYM preamble (em27.h). */
#include "EM27.H"

/* Em27 work variables kept in the enemy's generic Free[] area */
#define EF	((EM27_FREE *)OVL_FREE(pEm))

/* Timer2 and Timer3 are plain char; the retail code reads them as signed bytes (lb) in these tests. */
#define S8(x)	(*(signed char *)&(x))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of other translation units ----
 * EM27.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */

/* ApplyMatrixSV is declared: retail Em27_Init has no block for the Type 2 arm that first
 * calls it (an implicit declaration there would open one).  Declaration inferred from that
 * block structure; prototype as in PsyQ LIBGTE.H. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail 0xFA (0x801037BC) */
static unsigned short Em2b_life_easy = 0xC8;	/* retail 0xC8 */
/* retail-only second set, selected by G.System_flg & 0x40 ([INFERRED] _flg40 names as in em10/em20/em25;
 * retail .data 0x801037C0..0x801037EF); unreferenced by this TU's code */
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static void (*Em27_routine_0[8])() = {
	&Em27_Init,
	&Em27_Move,
	&Em27_Damage,
	&Em27_Die,
	&Em27_Scenario,
	0,
	0,
	&Em27_Dead
};
static void (*Em27_Move_type[])() = { };
static void (*Em27_mv_br_type00[8])() = {
	&Em27_mv00_br,
	&Em27_mv01_br,
	&Em27_mv02_br,
	&Em27_mv03_br,
	&Em27_mv04_br,
	&Em27_mv05_br,
	&Em27_mv06_br,
	&Em27_mv07_br
};
static void (*Em27_mv_type00[8])() = {
	&Em27_mv00,
	&Em27_mv01,
	&Em27_mv02,
	&Em27_mv03,
	&Em27_mv04,
	&Em27_mv05,
	&Em27_mv06,
	&Em27_mv07
};
static void (*Em27_die[18])() = {
	&Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00,
	&Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00,
	&Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00, &Em27_die00
};
static void (*Pldamage_Em27_tbl[1])() = {
	&Em27_pldm00
};
static void (*Pldie_Em27_tbl[])() = { };

void EM_MAIN(ENEMY_WORK *pEm)
{
	SVECTOR Sv;

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		Em27_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		EF->Drf_0 = EF->Drf_1;
		EF->Drf_1 = EF->Drf_2;
		EF->Drf_2 = EF->Drf_3;
		EF->Drf_3 = EF->Drf_4;
		EF->Drf_4 = EF->Drf_3;
		EF->Drf_3 = EF->Drf_6;
		EF->Drf_6 = EF->Drf_7;
		EF->Drf_7 = pEm->Cdir_y;

		Em_seq_sound(pEm);

		Oba_set_ofs(pEm, &pEm->Pos_x);
		Sca_ck_em(pEm, 0x400);

	}

}
static void Em27_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PARTS_W *pP_ptr; SVECTOR Sv, Sv1; register unsigned long i; MATRIX M;

	*(unsigned long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	pEm->Spd_x = 0x96;

	pEm->Life = (Rnd() & 1) + 1;

	pEm->Neck_flg = 0;
	pEm->Neck_no = 0;
	pEm->Spd_x = 0x50;

	pEm->Be_flg |= 0x400000;

	EF->Etc_flg = 0;

	pEm->Type |= 0x2000;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -200;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 200;
	pEm->Atd[0].At_h = 200;
	pEm->Atd[0].At_d = 200;
	pEm->Atd[0].W = 200;
	pEm->Atd[0].D = 200;
	pEm->nOba = 1;
	pEm->Be_flg |= 2;

	pEm->Eff_at_r = 300;

	pEm->Be_flg |= 0x4000000;

	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x100);

	Kage_work_set(&pEm->pKage_work, 0, 0x12C0190, 0, &pEm->pSin_parts_ptr->Workm.t[0]);

	if (!(pEm->Type & 0xFFF)) {
		pEm->Be_flg &= 0xFBFFFFFF;
		pEm->Type |= 0x4000;
		pEm->nOba = 0;
		((KAGE_WORK *)pEm->pKage_work)->Be_flg |= 8;

		i = 10;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		}while (--i);
	}

	if ((pEm->Type & 0xFFF) == 1) {
		pEm->Routine_1 = 3;
		pEm->Routine_2 = 0;
		G.pEnemy[2]->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.pEnemy[2]->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		pEm->Be_flg &= 0xFBFFFFFF;
		pEm->Type |= 0x4000;
		pEm->nOba = 0;
		((KAGE_WORK *)pEm->pKage_work)->Be_flg |= 8;
		pP_ptr = pEm->pSin_parts_ptr;
		i = 10;
		do {
			Texture_Move(&pP_ptr->Attribute, pEm->Tpage, 0x800000, pEm->Clut + 0x1E0);

			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		}while (--i);
	}

	if ((pEm->Type & 0xFFF) == 2) {
		pEm->Routine_1 = 6;
		pEm->Be_flg &= 0xFBFFFFFF;
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 7;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 8;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 1;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = G.pEnemy[2]->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 50;
		Sv.vz = 0;
		ApplyMatrixSV(&pP_ptr[4].Workm, &Sv, &Sv);
		pEm->Pos_x = Sv.vx + pP_ptr[4].Workm.t[0];
		pEm->Pos_y = Sv.vy + pP_ptr[4].Workm.t[1];
		pEm->Pos_z = Sv.vz + pP_ptr[4].Workm.t[2];
		pEm->Be_flg |= 0x800;
		pEm->Size = 0x800;
		pEm->Damage_cnt = 0x80;	/* retail-only */
	}

	if ((pEm->Type & 0xFFF) == 3) {
		pEm->Routine_1 = 1;
		pEm->Life = (Rnd() & 1) + 2;
	}

	G.pPldamage_Em[pEm->Id - 0x10] = PLDAMEGE_EM;
	G.pPldie_Em[pEm->Id - 0x10] = PLDIE_EM;

}
static void Em27_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em27_Move_type00(pEm, Kan, Seq); }
static void Em27_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em27_mv_br_type00[pEm->Routine_1](pEm);

	Em27_mv_type00[pEm->Routine_1](pEm, Kan, Seq); }
static void Em27_mv00_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em27_mv01_br(ENEMY_WORK *pEm)
{	register unsigned long LL; short Ck;	/* Ck: inferred name, no retail SYM record (single-use result is combined away) */

	LL = pEm->L_pl;

	Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);

	if (LL < 3000 && !Ck)
		*(unsigned long *)&pEm->Routine_0 = 0x201;

}
static void Em27_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register short Add; SVECTOR Sv, Sv1; MATRIX M;

	if (pEm->L_pl < 400 && (G.Room_player_flg & 6)) {

		*(unsigned long *)&pEm->Routine_0 = 0x10103;
		pEm->Damage_no = 0;
		pEm->Spd_x = 0;
		Esp_call(0x81FA0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 9;
		pEm->Timer0 = 0;
		Pad_vib_set0(1, 0);	/* retail-only (Dual Shock) */
		Pad_vib_set1(5, 80, 0);
		return;
	}

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Routine_2 = 1;

		pEm->Spd_y = 0;
		pEm->Spd_z = 0;

		pEm->Timer1 = 0;
		pEm->Timer2 = 0;
		pEm->Timer0 = (Rnd() & 0x1F) + 20;

	case 1:
		pEm->Spd_x -= 10;
		if (pEm->Spd_x < 30)
			pEm->Spd_x = 30;
		if (pEm->Spd_x > 120)
			pEm->Spd_x -= 30;

		if (!pEm->Timer2--) {
			pEm->Timer2 = (Rnd() & 0xF) + 10;
			pEm->Timer3 = (signed char)pEm->Timer2 >> 1;
		}

		if ((signed char)pEm->Timer2 > (signed char)pEm->Timer3) {
			pEm->Spd_x += (signed char)pEm->Timer2 * 3;
		}else
			pEm->Spd_x -= 20;

		Add = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		if (!(Rnd() & 3))
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, -((Rnd() & 1) << 5));

		if (Add > 0) {
			pEm->Timer1 += 4;
			if (pEm->Timer1 < -0x40)
				pEm->Timer1 += 8;

			if (pEm->Timer1 < -0x80)
				pEm->Timer1 += 16;

			if (pEm->Timer1 < -0x100)
				pEm->Timer1 += 16;
		}
		if (Add < 0) {
			pEm->Timer1 -= 4;
			if (pEm->Timer1 > -0x40)
				pEm->Timer1 -= 8;

			if (pEm->Timer1 > 0x80)
				pEm->Timer1 -= 16;

			if (pEm->Timer1 > 0x100)
				pEm->Timer1 -= 16;
		}

		pEm->Cdir_y += pEm->Timer1;

		Sv.vx = 0;
		Sv.vy = EF->Drf_0;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = pEm->Spd_x;
		Sv.vy = 0;
		Sv.vz = 0;
		ApplyMatrixSV(&M, &Sv, &Sv1);
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Pos_x += Sv1.vx;
		pEm->Pos_z += Sv1.vz;

		if (pEm->Timer0)
			pEm->Timer0--;

		if (!pEm->Timer0) {
			if (Rnd() & 1) {
				*(unsigned long *)&pEm->Routine_0 = 0x501;
				if (Rnd() & 1)
					*(unsigned long *)&pEm->Routine_0 = 0x701;
			}

			pEm->Timer0 = (Rnd() & 0x1F) + 20;
		}

	}
}
static void Em27_mv02_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x30002;
		pEm->Routine_2 = 1;
		pEm->Spd_y = -180;
		pEm->Spd_x += 200;
		if (pEm->Spd_x > 300)
			pEm->Spd_x = 300;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x28);
		Joint_move(pEm, Kan, Seq, 0x200);
		Add_speedXYZ(pEm, 0, 0);

		pEm->Spd_y += pEm->Timer0 * 15;
		if (pEm->Spd_y > 0 && !pEm->Timer1) {
			pEm->Timer1 = 1;
			*(long *)&pEm->Move_no = 0x70003;
		}
		pEm->Timer0++;
		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_x -= 150;
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			if (Rnd() & 1)
				*(unsigned long *)&pEm->Routine_0 = 0x701;

		}

		if (pEm->L_pl < 700 && !G.Player.Damage_cnt)
			*(unsigned long *)&pEm->Routine_0 = 0x401;

	}
}
static void Em27_mv03_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0, Sv, Sv1;
	unsigned char Em27_mv03_tbl[8] = { 4, 4, 5, 4, 5, 5, 4, 5 };
	register PARTS_W *pP_ptr;
	register unsigned long i;
	MATRIX M;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70004;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 5;

	case 1:
		Joint_move(pEm, Kan, Seq, 0x200);

		if (pEm->Sce_free0 & 1) {
			G.pEnemy[2]->Routine_0 = 8;
			G.pEnemy[2]->Routine_1 = 0;
			G.pEnemy[2]->Routine_2 = 0;
			pEm->Routine_2 = 2;

			pP_ptr = pEm->pSin_parts_ptr + 5;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 7;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 8;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 9;
			pP_ptr->Be_flg &= ~0x4000;
		}

		break;

	case 2:
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Move_no = Em27_mv03_tbl[Rnd() & 7];
			pEm->Hokan_flg = 7;
			pEm->Timer0 = (Rnd() & 7) + 8;

			pP_ptr = pEm->pSin_parts_ptr + 4;
			Esp_call(0x107D0, pEm->Cdir_y, &pP_ptr->Workm, 0);

		}
		Joint_move(pEm, Kan, Seq, 0x200);

		Sv0.vx = 0;
		Sv0.vy = -400;
		Sv0.vz = -200;
		ApplyMatrixSV(&G.pEnemy[2]->pSin_parts_ptr->Workm, &Sv0, &Sv0);

		pEm->Pos_x = Sv0.vx + G.pEnemy[2]->pSin_parts_ptr->Workm.t[0];
		pEm->Pos_y = Sv0.vy + G.pEnemy[2]->pSin_parts_ptr->Workm.t[1];
		pEm->Pos_z = Sv0.vz + G.pEnemy[2]->pSin_parts_ptr->Workm.t[2];

		pEm->Cdir_y = G.pEnemy[2]->Cdir_y;

		break;

	case 3:
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 7;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 8;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Be_flg |= 0x4000;
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, Kan, Seq, 0x200);

		break;

	case 5:
		pEm->Routine_2 = 6;
		*(long *)&pEm->Move_no = 0x70006;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		pP_ptr = pEm->pSin_parts_ptr;
		i = 10;
		do {
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr++;
		}while (--i);
		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x20FA0, pEm->Cdir_y, &pP_ptr->Workm, 0);

	case 6:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Move_no = 0x7000E;

			pEm->Routine_2 = 8;
		}

		pEm->Timer0 += pEm->Spd_x;
		pEm->Timer1 += pEm->Spd_y;
		Sv0.vx = pEm->Timer0 + 100;
		Sv0.vy = pEm->Timer1;
		Sv0.vz = -100;
		ApplyMatrixSV(&G.pEnemy[2]->pSin_parts_ptr->Workm, &Sv0, &Sv0);

		pEm->Pos_x = Sv0.vx + G.pEnemy[2]->pSin_parts_ptr->Workm.t[0];
		pEm->Pos_y = Sv0.vy + G.pEnemy[2]->pSin_parts_ptr->Workm.t[1];
		pEm->Pos_z = Sv0.vz + G.pEnemy[2]->pSin_parts_ptr->Workm.t[2];

		pEm->Cdir_y = G.pEnemy[2]->Cdir_y;

		break;

	case 7:
		Joint_move(pEm, Kan, Seq, 0x200);
		Sv0.vx = pEm->Timer0 + 100;
		Sv0.vy = pEm->Timer1;
		Sv0.vz = -100;
		ApplyMatrixSV(&G.pEnemy[2]->pSin_parts_ptr->Workm, &Sv0, &Sv0);

		pEm->Pos_x = Sv0.vx + G.pEnemy[2]->pSin_parts_ptr->Workm.t[0];
		pEm->Pos_y = Sv0.vy + G.pEnemy[2]->pSin_parts_ptr->Workm.t[1];
		pEm->Pos_z = Sv0.vz + G.pEnemy[2]->pSin_parts_ptr->Workm.t[2];

		pEm->Cdir_y = G.pEnemy[2]->Cdir_y;
		break;

	case 8:
		pEm->Spd_y = 100;
		pEm->Spd_x = 300;
		pEm->Spd_z = 0;
		pEm->Timer0 = 0;
		pEm->Routine_2 = 9;
		*(long *)&pEm->Move_no = 0x70003;

		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x10FA0, pEm->Cdir_y, &pP_ptr->Workm, 0);
		Esp_call(0x1388, pEm->Cdir_y, &pP_ptr->Workm, 0);

	case 9:
		Joint_move(pEm, Kan, Seq, 0x200);
		Add_speedXYZ(pEm, 0, 0);

		pEm->Spd_y += pEm->Timer0 * 15;
		pEm->Timer0++;
		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Routine_2 = 10;
		}
		break;

	case 10:
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Routine_2 = 11;

		pEm->Spd_x = 80;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;

		pEm->Timer1 = 0;
		pEm->Timer2 = 0;
		pEm->Timer0 = (Rnd() & 0x1F) + 20;

	case 11:
		Goto00(pEm, pEm->Sce_free0, pEm->Sce_free1, 0x10);

		if (pEm->Spd_x < 60)
			pEm->Spd_x = 60;
		if (pEm->Spd_x > 150)
			pEm->Spd_x -= 30;

		if (!pEm->Timer2--) {
			pEm->Timer2 = 10;
			pEm->Timer3 = 5;
		}
		if ((signed char)pEm->Timer2 > (signed char)pEm->Timer3) {
			pEm->Spd_x += (signed char)pEm->Timer2 << 2;
		}else
			pEm->Spd_x -= 40;

		Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);

		Sv.vx = 0;
		Sv.vy = EF->Drf_0;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = pEm->Spd_x;
		Sv.vy = 0;
		Sv.vz = 0;
		ApplyMatrixSV(&M, &Sv, &Sv1);
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Pos_x += Sv1.vx;
		pEm->Pos_z += Sv1.vz;

	}
}
static void Em27_mv04_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	MATRIX M;
	register unsigned short Rot;
	register long x, z, i;
	register PARTS_W *pP_ptr;

	unsigned char Em27_kam[4] = { 0, 8, 10, 11 };

	if (pEm->Routine_2 < 9 && (G.Room_enemy_flg & 1))
		pEm->Routine_2 = 9;

	if (pEm->Routine_2 < 9 && (G.Player.Damage_cnt & 0x80))
		pEm->Routine_2 = 9;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x30007;
		pEm->Routine_2 = 1;

		EF->Dir_h = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

		EF->Dir_h = (EF->Dir_h - G.Player.Cdir_y) & 0xFFF;

		pEm->Timer0 = 0x40 - ((Rnd() & 1) << 7);

		EF->Hajiki = (Rnd() & 0x3F) + 0x186;
		EF->Etc_flg &= 0xFFFE;
		pEm->Damage_cnt |= 0x80;
		Pad_vib_set1(8, 80, 0);	/* retail-only (Dual Shock) */

	case 1:
		Joint_move(pEm, Kan, Seq, 0x200);
		Em27_pl_on(pEm, 300, EF->Dir_h + G.Player.Cdir_y);

		EF->Dir_h += pEm->Timer0;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
		pEm->Pos_y -= 150;

		Rot = abs(G.Player.pSin_parts_ptr->Workm.t[1] + 300 - pEm->Pos_y);
		if (Rot < 300)
			pEm->Routine_2 = 2;

		break;

	case 2:
		pEm->Routine_2 = 3;

		pEm->Timer0 = (Rnd() & 0x1F) + 60;
		pEm->Timer1 = 4;
		pEm->Timer2 = 7;
		pEm->Timer3 = 8;

	case 3:
		Joint_move(pEm, Kan, Seq, 0x200);
		Em27_pl_on(pEm, 300, EF->Dir_h + G.Player.Cdir_y);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 4;

		pEm->Pos_y -= 3;
		EF->Dir_h += pEm->Timer1;
		pEm->Timer1 += (signed char)pEm->Timer3;
		if (!pEm->Timer2--) {
			pEm->Timer2 = 15;
			pEm->Timer3 = -S8(pEm->Timer3);
		}

		EF->Hajiki -= Em27_gacha_ch() * 16; if (EF->Hajiki < 0) { pEm->Routine_2 = 9; pEm->Cdir_y += 0x800; }
		break;

	case 4:
		pEm->Timer0 = (Rnd() & 0xF) + 15;
		pEm->Routine_2 = 5;

		if (Rnd() & 1)
			Snd_se_enem(1, pEm);
		pEm->Timer3 = 0;	/* retail-only: grab delay counter, see case 5 */
		Pad_vib_set1(8, 100, 0);	/* retail-only (Dual Shock) */

	case 5:
		Joint_move(pEm, Kan, Seq, 0x200);
		if ((unsigned short)pEm->Timer0-- == 0) {

			if (G.Player.Routine_0 != 5) {
				G.Player.Routine_0 = 5;
				G.Player.Routine_1 = 0;
				G.Player.Routine_2 = 0;
			}
			G.Player.pEnemy_ptr = (unsigned long *)pEm;
			G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
			G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
			pEm->Timer3 = 0x84;	/* retail: go to case 6 after 4 more frames (Beta: at once) */
		}
		if ((signed char)pEm->Timer3 & 0x80) {
			if (!(--pEm->Timer3 & 0x7f))
				pEm->Routine_2 = 6;	}

		Em27_pl_on(pEm, 300, EF->Dir_h + G.Player.Cdir_y);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
		EF->Hajiki -= Em27_gacha_ch() * 16; if (EF->Hajiki < 0) { pEm->Routine_2 = 9; pEm->Cdir_y += 0x800; }

		break;

	case 6:
		Rot = 0x340 - EF->Dir_h;
		pEm->Timer1 = 0x140;
		if ((Rot & 0xFFF) < 0x280) {
			pEm->Timer1 = 0;
			EF->Dir_h = 0x200;
		}
		EF->Dir_h -= pEm->Timer1;
		if ((Rot & 0xFFF) <= 0x800)
			EF->Dir_h += pEm->Timer1 << 1;

		pEm->Pos_y -= 200;
		Rot = abs(G.Player.pSin_parts_ptr[8].Workm.t[1] - pEm->Pos_y);

		if (Rot < 200)
			pEm->Routine_2 = 7;

		Em27_pl_on(pEm, 300, EF->Dir_h + G.Player.Cdir_y);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

		EF->Hajiki -= Em27_gacha_ch() * 16;
		if (EF->Hajiki < 0) {
			pEm->Routine_2 = 9;
			pEm->Cdir_y += 0x800;
		}
		break;

	case 7:
		*(long *)&pEm->Move_no = 0x70008;
		pEm->Routine_2 = 8;
		pEm->Timer0 = (Rnd() & 0xF) + 45;

		if (--pEm->Life < 0)
			EF->Etc_flg |= 1;

		if (Pl_life_down(10, 0) == 2) {
			pEm->Routine_2 = 11;
			G.System_flg |= 0x100;
			G.Player.Damage_cnt |= 0x80;
			break;
		}

	case 8:
		Joint_move(pEm, Kan, Seq, 0x200);

		x = G.Player.Pos_x;
		z = G.Player.Pos_z;
		G.Player.Pos_x = G.Player.pSin_parts_ptr[8].Workm.t[0];
		G.Player.Pos_z = G.Player.pSin_parts_ptr[8].Workm.t[2];
		Em27_pl_on(pEm, 350, EF->Dir_h + G.Player.Cdir_y);
		G.Player.Pos_x = x;
		G.Player.Pos_z = z;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 9;
			pEm->Cdir_y += 0x800;
		}

		if (!(pEm->Timer0 & 3) && pEm->Timer0 > 10) {

			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv0.vx = 100;
			Sv0.vy = 200;
			Sv0.vz = 0;
			Esp_call(((Rnd() << 1) + 0x5F4) | 0x10000, pEm->Cdir_y + 0xA00 - (Rnd() << 2), &pP_ptr->Workm, &Sv0);

		}

		if (!(pEm->Timer0 & 7))

		{	Snd_se_enem(Em27_kam[Rnd() % 4], pEm);
			Pad_vib_set0((Rnd() & 1) + 1, Rnd() & 3);	/* retail-only (Dual Shock) */
			Pad_vib_set1((Rnd() & 3) + 2, 150, Rnd() & 3);	}

		break;

	case 9:
		pEm->Spd_y = 100;
		pEm->Spd_x = 120;
		pEm->Spd_z = 0;
		pEm->Timer0 = 0;
		pEm->Routine_2 = 10;
		*(long *)&pEm->Move_no = 3;

		pEm->Cdir_y = pEm->Cdir_y - ((Rnd() << 1) - 0x900);

		if (EF->Etc_flg & 1) {
			pEm->Cdir_y = G.Player.Cdir_y - 0x100 - (Rnd() << 1);

			pEm->Spd_y = 300;
			pEm->Spd_x = 200;
		}

		if (!(G.Player.Damage_cnt & 0x80) && G.Player.Routine_0 == 5)

			G.Player.Routine_2 = 2;

		if (!(G.Player.Damage_cnt & 0x80) && G.Player.Routine_0 != 5) {

				G.Player.Routine_0 = 5;
				G.Player.Routine_1 = 0;
				G.Player.Routine_2 = 2;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		}

		G.Room_enemy_flg |= 1;
		pEm->Damage_cnt &= 0x7F;

	case 10:
		if (pEm->Move_cnt == 1)

			G.Room_enemy_flg &= 0xFFFE;

		Joint_move(pEm, Kan, Seq, 0x200);
		Add_speedXYZ(pEm, 0, 0);

		pEm->Spd_y += pEm->Timer0 * 15;
		pEm->Timer0++;
		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			*(unsigned long *)&pEm->Routine_0 = 0x101;

			if (EF->Etc_flg & 1) {
				*(unsigned long *)&pEm->Routine_0 = 0x103;
				pEm->Damage_no = 0;
			}
		}

		break;

	case 11:
		*(long *)&pEm->Move_no = 0xF000F;
		pEm->Routine_2 = 12;
		pEm->Timer0 = 120;
		pEm->Timer1 = 250;

		G.Player.Damage_cnt |= 0x80;
		G.Player.Life = -1;
		G.Player.Routine_2 = 4;

		pEm->Pos_y -= 100;

		pP_ptr = pEm->pSin_parts_ptr + 2;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 3;
		pP_ptr->Be_flg |= 0x4000;

		pEm->Be_flg |= 0x800;
		pEm->Size = 0x1000;
		/* retail-only (Dual Shock): the kill sequence */
		Pad_vib_set0(2, 0);	Pad_vib_set1(3, 150, 0);
		Pad_vib_set0(1, 3);	Pad_vib_set1(2, 150, 5);
		Pad_vib_set0(2, 7);	Pad_vib_set1(4, 150, 9);
		Pad_vib_set0(3, 12);	Pad_vib_set1(2, 150, 16);
		Pad_vib_set0(2, 18);	Pad_vib_set1(4, 150, 20);
		Pad_vib_set0(1, 22);	Pad_vib_set1(3, 150, 26);
		Pad_vib_set0(3, 22);	Pad_vib_set1(3, 150, 31);
		Pad_vib_set0(3, 29);
		Pad_vib_fade_set(20, 200, 100, 37);
		Pad_vib_set0(2, 37);
		Pad_vib_set0(3, 41);
		Pad_vib_set0(3, 48);
		Pad_vib_set0(2, 55);
		Pad_vib_set1(80, 140, 55);
		Pad_vib_set0(5, 70);
		Pad_vib_set1(15, 250, 75);
		Pad_vib_set0(3, 90);
		Pad_vib_set1(10, 250, 95);
		Pad_vib_fade_set(90, 200, 0, 135);

	case 12:
		pEm->Size -= 15;

		Joint_move(pEm, Kan, Seq, 0x100);

		x = G.Player.Pos_x;
		z = G.Player.Pos_z;
		G.Player.Pos_x = G.Player.pSin_parts_ptr[8].Workm.t[0];
		G.Player.Pos_z = G.Player.pSin_parts_ptr[8].Workm.t[2];
		Em27_pl_on(pEm, pEm->Timer1, EF->Dir_h + G.Player.Cdir_y);
		G.Player.Pos_x = x;
		G.Player.Pos_z = z;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 13;

		if (!(pEm->Timer0 & 3)) {

			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv0.vx = 100;
			Sv0.vy = 200;
			Sv0.vz = 0;
			Esp_call(((Rnd() << 1) + 0xDC4) | 0x10000, pEm->Cdir_y + 0xA00 - (Rnd() << 2), &pP_ptr->Workm, &Sv0);

			Esp_call(((Rnd() << 1) + 0x5F4) | 0x10000, pEm->Cdir_y + 0xA00 - (Rnd() << 2), &pP_ptr->Workm, &Sv0);

			Esp_call(((Rnd() << 1) + 0x5F4) | 0x10000, pEm->Cdir_y + 0xA00 - (Rnd() << 2), &pP_ptr->Workm, &Sv0);

			if (pEm->Timer0 < 30)

				Esp_call(((Rnd() << 1) + 0x9DC) | 0x30000, pEm->Cdir_y + 0xA00 - (Rnd() << 2), &pP_ptr->Workm, &Sv0);

		}

		if (!(pEm->Timer0 & 7))

			Snd_se_enem(Em27_kam[Rnd() % 4], pEm);

		if (pEm->Timer0 == 60) {
			G.Player.Routine_2 = 6;

			pP_ptr = pEm->pSin_parts_ptr + 1;
			pP_ptr->Be_flg |= 0x4000;

			Snd_se_enem(4, pEm);
		}

		if (pEm->Timer0 < 60) {
			pEm->Timer1 -= 2;
			pEm->Pos_y += 5;
		}

		if (pEm->Timer0 == 5) {
			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv0.vx = 100;
			Sv0.vy = 200;
			Sv0.vz = 0;
			Esp_call(0x11F40, pEm->Cdir_y + 0x800, &pP_ptr->Workm, &Sv0);

			i = 10;
			pP_ptr = pEm->pSin_parts_ptr;
			do {
				pP_ptr->Be_flg |= 0x4000;
				pP_ptr++;
			}while (--i);

			Snd_se_enem(4, pEm); }

		break;

	case 13:
		pEm->Routine_2 = 14;
		*(unsigned long *)&G.Player.Routine_0 = 0x80005;

	case 14:
		break;
	}

}
static unsigned long Em27_gacha_ch(void)
{
	register unsigned long R;

	if (G.Key_trg & 0x34F) R = 1; else R = 0;

	if (G.Room_player_flg & 2)

		R += 3;

	return R; }
static void Em27_mv05_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	long Ck;	/* inferred name: no retail SYM record (single-use results are combined away) */

	if (pEm->L_pl < 400 && (G.Room_player_flg & 6)) {

		*(unsigned long *)&pEm->Routine_0 = 0x10103;
		pEm->Damage_no = 0;
		pEm->Spd_x = 0;
		Esp_call(0x81FA0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 9;
		pEm->Timer0 = 0;
		Pad_vib_set0(1, 0);	/* retail-only (Dual Shock) */
		Pad_vib_set1(5, 80, 0);
		return;
	}

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0001;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);
		pEm->Spd_x -= pEm->Timer0 * 2;
		pEm->Timer0++;

		if (pEm->Spd_x < 0) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = (Rnd() & 0x1F) + 30;
			pEm->Timer2 = 0;
		}

		break;

	case 2:
		pEm->Timer2 = 0;
		Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200); if (Ck)
			pEm->Timer2 = 1;

		Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20); if (!Ck)
			pEm->Timer2 = 0;

		if (S8(pEm->Timer2))
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);

		Joint_move(pEm, Kan, Seq, 0x100);

		if ((unsigned short)pEm->Timer0-- == 0) {

			pEm->Routine_2 = 3;

			if (Rnd() & 1) {
				pEm->Routine_1 = 1;
				pEm->Routine_2 = 0;
			}
		}

		break;

	case 3:
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Routine_2 = 4;
		pEm->Timer1 = Rnd() + 30;

	case 4:
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, -8);

		pEm->Spd_x += 10;
		if (pEm->Spd_x > 100) {
			pEm->Spd_x = 100;
			*(unsigned long *)&pEm->Routine_0 = 0x101;
		}

	}
}
static void Em27_mv06_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long i;
	PARTS_W *pP;	/* inferred name: G.pEnemy[2] parts[4]; gcc folds the +4 into each use and drops the variable (no retail SYM record) */
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 12;
		pEm->Routine_2 = 1;

		pEm->Timer0 = 30;
		pEm->Timer2 = 90;

		i = 10;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Poly_rgb = 0xC0C0C0;
			pP_ptr++;
		}while (--i);

		Snd_se_enem(7, pEm);

	case 1:
		if ((signed char)pEm->Timer2 == 60) {
			pP_ptr = pEm->pSin_parts_ptr + 1;
			pP_ptr->Be_flg &= ~0x4000;
		}

		pP = G.pEnemy[2]->pSin_parts_ptr + 4;
		Sv.vx = pEm->Timer0;
		Sv.vy = pEm->Timer0 / 5 + 60;
		Sv.vz = 0x40 - (Rnd() & 0x7F);
		ApplyMatrixSV(&pP->Workm, &Sv, &Sv);
		pEm->Pos_x = Sv.vx + pP->Workm.t[0];
		pEm->Pos_y = Sv.vy + pP->Workm.t[1];
		pEm->Pos_z = Sv.vz + pP->Workm.t[2];
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Timer0 += 5;

		if (!pEm->Timer2--)
			pEm->Routine_2 = 2;

		break;

	case 2:
		pEm->Timer0 = 0;
		pEm->Routine_2 = 3;
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 7;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 8;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Be_flg &= ~0x4000;

		Snd_se_enem(7, pEm);

	case 3:
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Pos_y += pEm->Timer0;
		pEm->Timer0 += 30;

		if (pEm->Pos_y > pEm->Ground) {
			pEm->Routine_2 = 4;
			pEm->Pos_y = pEm->Ground;

			pEm->nFloor = 3;
			pEm->Damage_cnt &= 0x7f;	/* retail-only */
		}

		break;

	case 4:
		*(long *)&pEm->Move_no = 0x3000B;
		pEm->Timer0 = (Rnd() & 0x1F) + 120;
		pEm->Routine_2 = 5;
		pEm->Be_flg |= 0x4000000;

		pEm->Timer1 = 1;

		Snd_se_enem(5, pEm);

	case 5:
		pEm->Size += 0x12 + pEm->Timer1 * 30;
		pEm->Timer1 *= -1;
		if (pEm->Size > 0x1000)
			pEm->Size = 0x1000;

		if (pEm->Timer0 == 60)
			*(long *)&pEm->Move_no = 0x30009;

		if ((pEm->Timer0 & 0x3F) && !(Rnd() & 0x1F))

			Snd_se_enem(5, pEm);

		pEm->Spd_x = 0x10 - (Rnd() & 0x1F);
		pEm->Spd_z = 0x10 - (Rnd() & 0x1F);
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Cdir_y = pEm->Cdir_y - (Rnd() - 0x80);

		if (pEm->Timer0 < 90) {
			i = 10;
			pP_ptr = pEm->pSin_parts_ptr;
			do {
				pP_ptr->Poly_rgb -= 0x10101;
				pP_ptr++;

			}while (--i);
		}

		Joint_move(pEm, Kan, Seq, 0x400);
		if ((unsigned short)pEm->Timer0-- == 0) {
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			pEm->Spd_x = 80;

			pEm->Be_flg &= ~0x800;
		}

		if (pEm->L_pl < 400 && (G.Room_player_flg & 6)) {

			*(unsigned long *)&pEm->Routine_0 = 0x10103;
			pEm->Damage_no = 0;
			pEm->Spd_x = 0;
			Esp_call(0x81FA0, Rnd() << 4, (MATRIX *)pEm->M, 0);
			Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
			Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
			*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 9;
			pEm->Timer0 = 0;
			Pad_vib_set0(1, 0);	/* retail-only (Dual Shock) */
			Pad_vib_set1(5, 80, 0);
		}
	}
}
static void Em27_mv07_br(ENEMY_WORK *pEm)
{

}
static void Em27_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (pEm->L_pl < 400 && (G.Room_player_flg & 6)) {

		*(unsigned long *)&pEm->Routine_0 = 0x10103;
		pEm->Damage_no = 0;
		pEm->Spd_x = 0;
		Esp_call(0x81FA0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		Esp_call(0x817D0, Rnd() << 4, (MATRIX *)pEm->M, 0);
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 9;
		pEm->Timer0 = 0;
		Pad_vib_set0(1, 0);	/* retail-only (Dual Shock) */
		Pad_vib_set1(5, 80, 0);
		return;
	}

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x40);
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		pEm->Spd_x -= pEm->Timer0 * 5;
		pEm->Timer0++;
		if (pEm->Spd_x < 0) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = (Rnd() & 0x1F) + 60;
			pEm->Timer2 = 0;
		}

		break;

	case 2:
		if (pEm->L_pl < 2500) {
			pEm->Routine_2 = 3;
			*(long *)&pEm->Move_no = 0x7000D;

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);

			break;
		}
		Joint_move(pEm, Kan, Seq, 0x100);

		if ((unsigned short)pEm->Timer0-- == 0) {
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			pEm->Spd_x = 80; }
		break;

	case 3:
		Spd_get(pEm, Kan, Seq, 0);

		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			pEm->Spd_x = 100;
			pEm->Spd_z = 0;
			break;
		}
		Add_speedXZ(pEm, 0);

	}
}
static void Em27_pl_on(ENEMY_WORK *pEm, unsigned long L, short Dir)
{	SVECTOR Sv0; MATRIX M;

	Sv0.vx = 0;
	Sv0.vy = Dir;
	Sv0.vz = 0;
	RotMatrix(&Sv0, &M);
	Sv0.vx = L;
	Sv0.vy = 0;
	Sv0.vz = 0;
	ApplyMatrixSV(&M, &Sv0, &Sv0);
	pEm->Pos_x = Sv0.vx + G.Player.Pos_x;
	pEm->Pos_z = Sv0.vz + G.Player.Pos_z;
}
static void Em27_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em27_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern void (*Em27_die[])();	/* [INFERRED] block-scope declaration of the table used here: opens retail's record-less block */

	Em27_die[pEm->Routine_1 - 1](pEm, Kan, Seq);

}
static void Em27_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;
	register KAGE_WORK *pKage;
	SVECTOR Sv;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0xF0009;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;

		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x817D0, 0, &pP_ptr->Workm, 0);

		pEm->Life = -1;

		pEm->Timer3 = 0;

		if (pEm->Routine_1 == 9 || pEm->Routine_1 == 10 || pEm->Routine_1 == 16 || pEm->Routine_1 == 17) {	/* retail: + type 17 */

			Esp_call(((Rnd() << 2) + 0xC18) | 0x5040000, pEm->Cdir_y, (MATRIX *)pEm->M, 0);

			pEm->Timer3 = 1;
		}
		if (pEm->Routine_1 == 11 || pEm->Routine_1 == 14) {
			Esp_call(((Rnd() << 2) + 0xC18) | 0x40F0000, pEm->Cdir_y, (MATRIX *)pEm->M, 0);

			pEm->Timer3 = 1;
		}
		if (pEm->Routine_1 == 12) {	/* retail-only */
			pEm->Spd_x = 0;
			pEm->Spd_z = 0;	}

		Snd_se_enem(3, pEm);

	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);
		pEm->Spd_x -= pEm->Timer0 * 20;
		pEm->Timer0++;
		if (pEm->Spd_x < 0) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = Rnd() & 7;

			i = 10;
			pP_ptr = pEm->pSin_parts_ptr;
			do {
				pP_ptr->Be_flg |= 0x2000;
				pP_ptr->Size_x = 0x1000;
				pP_ptr->Size_y = 0x1000;
				pP_ptr->Size_z = 0x1000;
				pP_ptr++;
			}while (--i);
		}

		break;

	case 2:
		if (pEm->Timer0 == 30 || pEm->Timer0 == 60) {
			pEm->Move_no++;
			pEm->Hokan_flg = 7;
		}

		pEm->Timer0++;
		if (!(pEm->Timer0 & 0x1F) && !S8(pEm->Timer3)) {
			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vx = (Rnd() >> 1) + 0x40;
			Sv.vy = 200;
			Sv.vz = 0x40 - (Rnd() >> 1);
			Esp_call(0x805F4, Rnd() << 4, &pP_ptr->Workm, &Sv);
		}

		i = 10;
		pP_ptr = pEm->pSin_parts_ptr;
		do {	/* retail: the Beta Be_flg |= 0x2000 here is gone (set once in case 1) */
			pP_ptr->Size_x += 10;
			pP_ptr->Size_y -= 40;
			pP_ptr->Size_z += 10;

			pP_ptr++;
		}while (--i);

		pP_ptr = pEm->pSin_parts_ptr;
		i = 10;
		if ((pP_ptr->Poly_rgb & 0xFF) != 0x20) {
			do {
				pP_ptr->Poly_rgb -= 0x10001;
				pP_ptr++;
			}while (--i);
			pP_ptr--;
		}

		if ((*(unsigned long *)&pKage->Prim[0].r0 & 0xFF00) != 0x2000) {
			*(unsigned long *)&pKage->Prim[0].r0 -= 0x400;
			*(unsigned long *)&pKage->Prim[1].r0 -= 0x400;
			*(unsigned long *)&pKage->Prim[0].r0 += 0x30003;
			*(unsigned long *)&pKage->Prim[1].r0 += 0x30003;

			pKage->Half_x += 10;
			pKage->Half_z += 10;
		}

		if (pP_ptr->Size_y < 0)
			pEm->Routine_2 = 3;

		Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 3:
		*(unsigned long *)&pEm->Routine_0 = 7;
	}

	if (pEm->Pos_y < pEm->Ground) {
		pEm->Pos_y += 100;
		if (pEm->Pos_y >= pEm->Ground)
			pEm->Pos_y = pEm->Ground;
	}

}
static void Em27_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em27_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;
	register KAGE_WORK *pKage;
	SVECTOR Sv;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_1) {
	case 0:
		pEm->Life = -1;
		*(long *)&pEm->Move_no = 9;
		pEm->Routine_1 = 1;
		pEm->Timer0 = 0;
		Joint_move(pEm, Kan, Seq, 0x100);

	case 1:
		pEm->Timer0++;

		if (pEm->Timer0 == 31) {
			i = 10;
			pP_ptr = pEm->pSin_parts_ptr;
			do {
				Late_set(&pP_ptr->Attribute, 1);
				pP_ptr->Attribute |= 2;
				pP_ptr->Poly_rgb -= 0x103010;
				pP_ptr++;
			}while (--i);
		}

		if (pEm->Timer0 >= 31) {

			pP_ptr = pEm->pSin_parts_ptr;
			i = 10; if (pP_ptr->Poly_rgb & 0xFF) {
				do {
					pP_ptr->Poly_rgb -= 0x10001;
					pP_ptr++;
				}while (--i);
			}

			pP_ptr = pEm->pSin_parts_ptr;

			i = 10; if (pP_ptr->Poly_rgb & 0xFF00) {
				do {
					pP_ptr->Poly_rgb -= 0x100;
					pP_ptr++;
				}while (--i);
			}
		}

		pP_ptr = pEm->pSin_parts_ptr;
		if (!(pP_ptr->Poly_rgb & 0xFFFFFF))
			pEm->Routine_1 = 2;

		if (*(unsigned long *)&pKage->Prim[0].r0 & 0xFF00) {
			*(unsigned long *)&pKage->Prim[0].r0 -= 0x100;
			*(unsigned long *)&pKage->Prim[1].r0 -= 0x100;
		}

		if (*(unsigned long *)&pKage->Prim[0].r0 & 0xFF) {
			*(unsigned long *)&pKage->Prim[0].r0 -= 0x20002;
			*(unsigned long *)&pKage->Prim[1].r0 -= 0x20002;
		}
		break;

	case 2:
		Kill_enemy_work(pEm);
		pKage->Be_flg = 0;
	}

}
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em27_tbl[pEm->Routine_1](pEm, Kan, Seq); }
static void Em27_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70000;	/* retail 0x70000 (Beta 0xF0000) */
		pEm->Routine_2 = 1;

		Snd_se_stad(0x4000001, &pEm->Pos_x);

	case 1:
		Joint_move(pEm, Kan, Seq, 0x200); break;	/* retail 0x200 */

	case 2:
		*(long *)&pEm->Move_no = 0x70004;
		pEm->Routine_2 = 3;
		pEm->Damage_cnt &= 0x7F;

	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x100))

			*(unsigned long *)&pEm->Routine_0 = 1;
		break;

	case 4:
		*(long *)&pEm->Move_no = 0xF0001;
		pEm->Routine_2 = 5;

		pEm->Neck_flg = 1;

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101010;

	case 5:
		Joint_move(pEm, Kan, Seq, 0x100); break;

	case 6:
		*(long *)&pEm->Move_no = 0xF0002;
		pEm->Routine_2 = 7;
		pP_ptr = pEm->pSin_parts_ptr + 8;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101010;

	case 7:
		Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 8:
		*(long *)&pEm->Move_no = 0xF0003;
		pEm->Routine_2 = 9;

		pP_ptr = pEm->pSin_parts_ptr + 8;
		Esp_call(0x213E8, pEm->Cdir_y, &pP_ptr->Workm, 0);
		Esp_call(0x21000, pEm->Cdir_y + 0x800, &pP_ptr->Workm, 0);

	case 9:
		pP_ptr = pEm->pSin_parts_ptr + 8;
		if (pEm->Move_cnt < 15 && !(pEm->Move_cnt & 1)) {
			Esp_call((Rnd() << 3) + 0x400, pEm->Cdir_y, &pP_ptr->Workm, 0);
		}
		if (pEm->Move_cnt == 45 || pEm->Move_cnt == 48 || pEm->Move_cnt == 5)

			Esp_call(0x21000, pEm->Cdir_y, &pP_ptr->Workm, 0);

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		Add_speedXZ(pEm, 0);
		break;

	case 10:
		*(unsigned long *)&pEm->Routine_0 = 7;

	}

}
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em27_tbl[pEm->Routine_1](pEm, Kan, Seq); }
