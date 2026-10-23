/* em31.c - reconstruction of D:\Bio2\prog\EM31.C (enemy 31: the plant core of
 * the B4 boss room).  Linked into two overlays from the same source: 0x16 (at
 * 0x801000c4) and 0x34 (at 0x8010d0c4); only the enemy-number-specific entry
 * names differ (Em131/Em231, Pldamege_Em131/231, Pldie_Em131/231), selected by
 * OVL_34.  statement lines follow the retail SLD by layout. */
#include "EM31.H"

#define EF	((EM31_FREE *)EM_FREE(pEm))	/* Em31 work kept in the enemy's generic Free[] area */

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern long Ground_in_flg;
extern MATRIX GsIDMATRIX;
/* libgte */
/* libgte */

/* ---- functions of this file ---- */

/* ---- data (retail .data order) ---- */
static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life = 250;	/* retail 250 (Beta 2: 300) */
static unsigned short Em2b_life_easy = 200;	/* retail 200 (Beta 2: 300) */
/* retail-only second life set ([INFERRED] _flg40 names as in em10/em20/em25; image bytes,
 * retail .data +0x30..+0x5F of this overlay; no retail code reads it here) */
static unsigned short Em23_life_flg40 = 240;
static unsigned short Em23_life_easy_flg40 = 240;
static unsigned short Em2a_life_flg40[10] = {
	250, 450, 300, 300, 220, 300, 400, 300, 400, 350
};
static unsigned short Em2a_life_easy_flg40[10] = {
	180, 340, 220, 240, 175, 240, 310, 240, 310, 300
};
static unsigned short Em2b_life_flg40 = 250;
static unsigned short Em2b_life_easy_flg40 = 200;
static void (*Em31_routine_0[8])() = { Em31_Init, Em31_Move, Em31_Damage, Em31_Die, Em31_Scenario, 0, 0, Em31_Dead };
/* breathing curve (Iki_timer 0..63) */
static char Em31_iki[64] = {
	12, 23, 35, 43, 51, 60, 66, 71, 75, 79, 82, 86, 89, 92, 94, 96,
	97, 98, 99, 100, 99, 98, 97, 96, 94, 92, 89, 87, 85, 83, 80, 78,
	75, 72, 68, 65, 62, 58, 54, 49, 44, 39, 34, 29, 24, 19, 13, 7,
	0, -6, -15, -11, -26, -30, -32, -34, -35, -35, -34, -31, -25, -20, -10, 0
};
static char Em31_me_tate = 0;
static char Em31_me_yoko = 0;
static short Em31_me_tate_mv = 0;
static short Em31_me_yoko_mv = 0;
static short Em31_me_tate_a = 0;
static short Em31_me_yoko_a = 0;
static char Em31_me_tate_d = 0;
static char Em31_me_yoko_d = 0;
static void (*Em31_Move_type[4])() = { Em31_Move_type00, Em31_Move_type01, Em31_Move_type02, Em31_Move_type00 };
static void (*Em31_mv_br_type00[10])() = { Em31_mv00_br, Em31_mv01_br, Em31_mv02_br, Em31_mv03_br, Em31_mv04_br, Em31_mv05_br, Em31_mv07_br, Em31_mv0a_br, Em31_mv0b_br, Em31_mv0c_br };
static void (*Em31_mv_type00[10])() = { Em31_mv00, Em31_mv01, Em31_mv02, Em31_mv03, Em31_mv04, Em31_mv05, Em31_mv07, Em31_mv0a, Em31_mv0b, Em31_mv0c };
static void (*Em31_mv_br_type01[7])() = { Em31_mv00_br, Em31_mv01_br1, Em31_mv06_br, 0, Em31_mv06_br, Em31_mv05_br1, Em31_mv07_br };	/* retail: entry 4 = Em31_mv06_br (image 0x80107CD4), Beta had 0 */
static void (*Em31_mv_type01[7])() = { Em31_mv00, Em31_mv01, Em31_mv06, 0, Em31_mv06, Em31_mv05, Em31_mv07 };	/* retail: entry 4 = Em31_mv06 (image 0x80107CF0), Beta had 0 */
static void (*Em31_mv_br_type02[2])() = { Em31_mv08_br, Em31_mv09_br };
static void (*Em31_mv_type02[2])() = { Em31_mv08, Em31_mv09 };
static short Em31_mv08_pod[6][2] = {
	{ -25056, -22878 }, { -20574, -23130 }, { -16020, -23040 },
	{ -25020, -25344 }, { -22014, -25200 }, { -15948, -25596 }
};
static unsigned char Em31_mv08_no[2][4] = { { 0, 1, 2, 3 }, { 0, 2, 1, 3 } };
static void (*Em31_dm[][9])() = { };
static unsigned char Em31_Dm_cnt[18] = {
	1, 4, 4, 3, 10, 20, 6, 10, 8, 20, 8, 3, 2, 20, 1, 0, 20, 2
};
static void (*Em31_die[][9])() = { };
static void (*Em31_sce[])() = { };
static void (*Pldamage_Em31_tbl[2])() = { Em31_pldm00, Em31_pldm00 };
static void (*Pldie_Em31_tbl[1])() = { Em31_pldie00 };
static void (*Em31_core_me_sa_tbl[3])() = { Em31_core_mv00, Em31_core_mv01, Em31_core_mv02 };

void EM_MAIN(ENEMY_WORK *pEm)
{
	short Dir;
	SVECTOR Sv;
	VECTOR Dv;

	short Max_life[2] = { 500, 0 };
	PARTS_W *pP;
	Dv = *(VECTOR *)&G.Player.Pos_x;

	Root_ck(pEm, &Dv, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (EF->Etc_flg & 2) {
			if (EF->Dm_timer0-- <= 0) {
				if (EF->Dm_cnt)
					EF->Dm_cnt--;
				EF->Dm_timer0 = 15;
				if (!EF->Dm_cnt)
					EF->Etc_flg &= ~2;
			}
		}

		if (pEm->Life <= 500 && EF->life_timer)
			EF->life_timer--;

		if (!(G.System_flg & 0x20)) {
			if (EF->plus_life-- <= 0 && EF->life_timer) {
				EF->plus_life = 7;
				if (pEm->Life > 0 && pEm->Life < Max_life[ pEm->Type & 1 ])
					pEm->Life++;
			}
		}

		if (EF->At_timer)
			EF->At_timer--;

		if (EF->Fire_c)
			EF->Fire_c--;

		if (EF->Etc_flg & 0x20) {
			pP = &pEm->pSin_parts_ptr[1];
			((short *)pP->pSa_dat_head)[6] = 0x1000 - *(signed char *)&Em31_iki[ *(signed char *)&EF->Iki_timer ] * 16;

			Sa_parts_allmv(pEm, pP);

			pP = &pEm->pSin_parts_ptr[2];
			((short *)pP->pSa_dat_head)[6] -= (1 - ((EF->Iki_timer & 1) << 1)) << 7;

			((short *)pP->pSa_dat_head)[8] += (1 - ((EF->Iki_timer & 1) << 1)) << 7;

			Sa_parts_allmv(pEm, pP);

			EF->Iki_timer++;
			EF->Iki_timer &= 0x3F;

			if (EF->Iki_timer & 0x10) {
				Em31_me_tate_d = 15 - Rnd() % 30;
				Em31_me_yoko_d = 15 - Rnd() % 30;
			}
		}

		Em31_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Em31_mgdm(pEm);

		pEm->pEnemy_ptr = (ENEMY_WORK *)&G.Player;
		Rot_neck_em_n(pEm, pEm->Cdir_y, &Dir);
		Dir &= 0xFFF;
		if (Dir > 0x800)
			Dir -= 0xFFF;
		pP = &pEm->pSin_parts_ptr[3];
		pP->Cdir_y += Dir;
		RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
		pP->Cdir_y -= Dir;

		Em31_core_me_mv(pEm, &pEm->pSin_parts_ptr[4]);
		Em31_core_me_sa(pEm, &pEm->pSin_parts_ptr[4]);

		Em_seq_sound(pEm);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);
	}

}
static void Em31_after(ENEMY_WORK *pEm)
{
}
static void Em31_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long i;

	unsigned char Me4[15] = { 14, 19, 22, 24, 25, 26, 27, 35, 36, 37, 38, 39, 40, 42, 50 };

	*(unsigned long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	/* retail: the "System_flg & 0x40" second life set (as Em10/Em28_Init), here with identical arms.
	 * Lever: the two-armed if/else ends the CSE path, so the G.pPldamage_Em/G.Player constants below
	 * are not rewritten relative to the &G.System_flg pseudo in a saved register; after reload the
	 * identical arms cross-jump and jump.c deletes the outer branch with its dead "& 0x40" test.	 */
	if (G.System_flg & 0x40) {
		pEm->Life = 700;
		if (G.System_flg & 0x20)
			pEm->Life = 600;
	}else {
		pEm->Life = 700;
		if (G.System_flg & 0x20)
			pEm->Life = 600;
	}

	pEm->D_life_u = 0;
	pEm->D_life_c = 0;
	pEm->D_life_d = 0;

	pP_ptr = pEm->pSin_parts_ptr;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 3;
	pP_ptr[3].Free[0] = 0;
	pP_ptr[3].Free[1] = 0;
	pP_ptr[3].Free[2] = 0x20;
	pP_ptr[3].Free[3] = 0x20;
	pP_ptr[3].Free[4] = 0x264;
	pP_ptr[3].Free[5] = 0x138;

	EF->Dm_timer0 = 0;
	EF->Dm_cnt = 0;
	EF->At_timer = 0;
	EF->Etc_flg = 0;
	EF->Iki_timer = 0;
	EF->Me_r_0 = 0;
	EF->Me_r_1 = 0;
	EF->plus_life = 5;
	EF->life_timer = 2000;
	EF->Yane_cnt = 0;
	EF->Yane_no = 0;
	EF->ikaku_num = 0;

	Em31_me_tate = 0;
	Em31_me_yoko = 0;
	Em31_me_tate_mv = 0;
	Em31_me_yoko_mv = 0;
	Em31_me_tate_a = 0;
	Em31_me_yoko_a = 0;
	Em31_me_tate_d = 0;
	Em31_me_yoko_d = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 900;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 900;
	pEm->Atd[0].W = 900;
	pEm->Atd[0].D = 900;

	pEm->Eff_at_r = 800;

	pEm->Be_flg |= 0xC000000;

	*(unsigned long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x100);

	Kage_work_set((unsigned long *)&pEm->pKage_work, 0, 0x3200384, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);

	G.pPldamage_Em[ pEm->Id - 16 ] = EM_PLDAMEGE;
	G.pPldie_Em[ pEm->Id - 16 ] = EM_PLDIE;

	pEm->Be_flg |= 4;

	i = 18;
	pP_ptr = pEm->pSin_parts_ptr;
	do {
		pP_ptr->Poly_rgb = 0x808080;
		pP_ptr++;
	}while (--i);

	pP_ptr = &pEm->pSin_parts_ptr[1];
	((short *)pP_ptr->pSa_dat_head)[6] = 0;
	Sa_parts_allmv(pEm, pP_ptr);

	pP_ptr = &pEm->pSin_parts_ptr[2];
	((short *)pP_ptr->pSa_dat_head)[6] = 0;
	((short *)pP_ptr->pSa_dat_head)[8] = 0;
	Sa_parts_allmv(pEm, pP_ptr);

	pEm->Routine_1 = 7;

	*(unsigned long *)&pEm->Move_no = 0x13;
	Joint_move(pEm, Kan, Seq, 0x100);

	pP_ptr = &pEm->pSin_parts_ptr[2];
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	((short *)pP_ptr->pSa_dat_head)[8] = 0;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = &pEm->pSin_parts_ptr[4];
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	Sa_parts_allmv(pEm, pP_ptr);

	pP_ptr = &pEm->pSin_parts_ptr[6];
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0x200;
	pP_ptr->Size_z = 0x800;
	pP_ptr = &pEm->pSin_parts_ptr[7];
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0x200;
	pP_ptr->Size_z = 0x800;
	pP_ptr = &pEm->pSin_parts_ptr[8];
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0x200;
	pP_ptr->Size_z = 0x800;

	pP_ptr = &pEm->pSin_parts_ptr[3];
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0x200;
	pP_ptr->Size_z = 0x1000;

	if ((pEm->Type & 0xFFF) == 2) {

		pEm->Timer0 = 60;

		pEm->Life = 5000;
		pEm->Be_flg |= 2;

		EF->Yane_no |= (Rnd() & 1) << 4;

		((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;

		pEm->Routine_1 = 0;
		pEm->Routine_2 = 3;

		i = 18;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		}while (--i);

		pP_ptr = &pEm->pSin_parts_ptr[6];
		pP_ptr->Be_flg &= ~0x400;
		pP_ptr = &pEm->pSin_parts_ptr[7];
		pP_ptr->Be_flg &= ~0x400;
		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg &= ~0x400;
	}

	if ((pEm->Type & 0xFFF) == 3) {
		pEm->Life = 5000;
		pEm->Be_flg |= 0xA;

		((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;

		pEm->Routine_1 = 9;
		pEm->Routine_2 = 0;

		pP_ptr = pEm->pSin_parts_ptr;
		i = 18;
		do {	pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		}while (--i);

		pP_ptr = &pEm->pSin_parts_ptr[6];
		pP_ptr->Be_flg &= ~0x400;
		pP_ptr = &pEm->pSin_parts_ptr[7];
		pP_ptr->Be_flg &= ~0x400;
		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg &= ~0x400;
	}

}
static void Em31_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em31_Move_type[ pEm->Type & 0x3F ](pEm, Kan, Seq);	}
static void Em31_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em31_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em31_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em31_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em31_mv_br_type01[ pEm->Routine_1 ](pEm);

	Em31_mv_type01[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em31_Move_type02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em31_mv_br_type02[ pEm->Routine_1 ](pEm);

	Em31_mv_type02[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em31_mv00_br(ENEMY_WORK *pEm)
{

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x320);

	pEm->Routine_1 = 1;

}
static void Em31_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned long Joint_move();

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 1;
		pEm->Neck_flg &= ~2;
		pEm->Timer3 = 1;
		pEm->Timer2 = 1;
	case 1:

		Joint_move(pEm, Kan, Seq, 0x80);
	}

}
static void Em31_mv01_br(ENEMY_WORK *pEm)
{
	register short Dir0, Dir1, Dir2;
	register unsigned long LL, Ck;
	SVECTOR Sv;	VECTOR V;	MATRIX M;	Ck = 0;

	LL = pEm->L_pl;

	Dir0 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x4C8);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x270);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!EF->life_timer) {
		pEm->Type = 1;
		*(unsigned long *)&pEm->Routine_0 = 0x601;
		return;
	}

	if (pEm->Life < 100) {
		pEm->Type = 1;
		*(unsigned long *)&pEm->Routine_0 = 0x601;
		return;
	}

	if (!Dir2 && LL < 1500) {
		*(unsigned long *)&pEm->Routine_0 = 0x501;
		pEm->Timer0 = 40;
		return;
	}

	if (!EF->At_timer && !G.Player.Damage_cnt)

		if (!Dir2 && LL > 2500 && LL < 3400) {
			*(unsigned long *)&pEm->Routine_0 = 0x201;
			Ck = 1;
		}

	if (LL < 2500 && !Dir0 && !G.Player.Damage_cnt && EF->At_timer < 10) {
		*(unsigned long *)&pEm->Routine_0 = 0x401;
		Ck = 1;
	}

	if (EF->At_timer && !G.Player.Damage_cnt) {

		if (!Dir1 && LL < 3400 && (G.Room_player_flg & 4) && !(Rnd() & 3)) {

			*(unsigned long *)&pEm->Routine_0 = 0x801;
			Ck = 1;
		}
	}

	if (G.Player.Life && G.Player.Life < 60 && Ck == 1) {
		EF->ikaku_num++;

		if (EF->ikaku_num < 3)

			*(unsigned long *)&pEm->Routine_0 = 0x301;
		else	EF->ikaku_num = 0;
	}

}
static void Em31_mv01_br1(ENEMY_WORK *pEm)
{
	register short Dir1;
	register unsigned long LL;
	SVECTOR Sv;	VECTOR V;	MATRIX M;
	short Dir2;

	LL = pEm->L_pl;

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x5F4);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x270);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!Dir2 && LL < 1500)
		*(unsigned long *)&pEm->Routine_0 = 0x501;

	if (!EF->At_timer && !G.Player.Damage_cnt) {

		if (!Dir1 && LL < 3000)
			*(unsigned long *)&pEm->Routine_0 = 0x201;
	}

}
static void Em31_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x1F0001;

		if (pEm->Type & 1)
			*(unsigned long *)&pEm->Move_no = 0x1F0002;

		pEm->Routine_2 = 1;

		pEm->Timer0 = 0;

	case 1:
		if (!(pEm->Type & 1)) {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
			if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x338))
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x30);

		}else {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
			if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x338))
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		}
		Spd_get(pEm, Kan, Seq, 0);
		if (pEm->Hokan_flg >= 16)
			pEm->Spd_x >>= 1;

		if (!Hit_ck_point4(&pEm->Pos_x, G.Cccut))
			pEm->Spd_x += 20;

		if (Joint_move(pEm, Kan, Seq, 0x80)) {

			pEm->Timer0++;

			if ((pEm->Type & 1) && (Rnd() & 1)) {
				Snd_se_enem(9, pEm);
			}
			if (!(pEm->Type & 1) && !(pEm->Timer0 & 1)) {
				Snd_se_enem(0xD, pEm);
			}
		}
		Add_speedXZ(pEm, 0);
	}
}
static void Em31_mv02_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0, Sv1;
	PARTS_W *pP_ptr;
	MATRIX M;
	unsigned char Ck;
	short Dir;	/* inferred: no SYM record (value lives only in a temporary) */

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0006;
		pEm->Routine_2 = 1;
		Spd_get(pEm, Kan, Seq, 0);

		pEm->Timer3 = 0;

		pEm->Timer2 = 0;

		pEm->Timer1 = 0;

		if (G.Player.Life >= 61) {
			pEm->Move_cnt = 15;
			Spd_get(pEm, Kan, Seq, 0);
		}

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (pEm->Hokan_flg) {
			pEm->Spd_x >>= 1;
			pEm->Spd_z >>= 1;
		}
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 20;
			EF->Etc_flg &= ~0x80;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt >= 6 && pEm->Move_cnt < 19 && !*(signed char *)&pEm->Timer3) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
			if (pEm->L_pl >= 3801 || Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x590)) {

				*(long *)&pEm->Routine_0 = 0x101;
				EF->At_timer = 10;
			}
		}

		if (pEm->Move_cnt == 40 && pEm->L_pl < 3000 && !*(signed char *)&pEm->Timer3) {
			*(unsigned long *)&pEm->Move_no = 0xF0007;
			pEm->Timer3 = 1;

			if ((G.Player.Damage_cnt & 0x80) && G.Player.Life > 0)
				G.Player.Damage_cnt = 0;
		}

		if (*(unsigned long *)pEm->pNow_seq & 0x30000)
		{
			EF->Etc_flg |= 0x80;

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

			if (*(signed char *)&pEm->Timer3 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400))

				pEm->Cdir_y += 0x80;

			Sv0.vx = 0;
			Sv0.vy = -300;
			Sv0.vz = 0;
			Sv1.vx = 0;
			Sv1.vy = 400;
			Sv1.vz = 0;
			Em31_hand(pEm, &M);
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 1200, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 1200, &Sv1)) && !G.Player.Damage_cnt)

			{
				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - 0x400;

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (*(signed char *)&pEm->Timer3)
					Ck ^= 1;

				Pl_life_down(20, 1);	/* retail: 15 -> 20 */

				*(long *)&G.Player.Routine_0 = ((Ck | 2) << 8) + 2;

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= ~1;

				pEm->Timer2 = 6;

				Snd_se_enem(3, pEm);
				Em31_Tume_color(pEm);
				Pad_vib_set0(8, 0);		/* retail-only vibration @0x80101568/0x80101578 */
				Pad_vib_set1(8, 0xfa, 4);
			}

			Sv0.vx = 0;
			Sv0.vy = 1000;
			Sv0.vz = 0;
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			M.t[0] += Sv0.vx;
			M.t[1] += Sv0.vy;
			M.t[2] += Sv0.vz;
			Sca_get_ground(M.t, 600, 0x8000, 1);
			if (Ground_in_flg) {
				pP_ptr = pEm->pSin_parts_ptr + 6;
				Sv0.vx = 0;
				Sv0.vy = 1000;
				Sv0.vz = 0;
				pP_ptr->Workm = M;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vx += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vy += 0x80 - (Rnd() & 0xFF);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vz += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

				if (!pEm->Timer1) {
					Snd_se_enem(7, pEm);
					pEm->Timer1 = 7;
				}
			}
		}

		G.Status_flg &= ~8;
		if (pEm->Timer1) {
			pEm->Timer1--;
			G.Status_flg |= 8;
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x4E8 - Rnd() * 2;
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0;
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}

	}
}
static void Em31_mv03_br(ENEMY_WORK *pEm)
{
	short Dir2;
	unsigned long LL;

	if (!EF->life_timer) {
		pEm->Type = 1;
		*(long *)&pEm->Routine_0 = 0x601;
		return;
	}
	if (pEm->Life < 100) {
		pEm->Type = 1;
		*(long *)&pEm->Routine_0 = 0x601;
		return;
	}

	LL = pEm->L_pl;

	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!G.Player.Damage_cnt && pEm->Move_cnt >= 11) {

		if (!Dir2 && LL < 1500) {
			if (G.Player.Spd_x > 0 && !((unsigned long (*)())Cdir_ck)(pEm, &G.Player)) {
				*(long *)&pEm->Routine_0 = 0x401;
				EF->ikaku_num = 0;
			}
			if (G.Player.Spd_x < 0 && ((unsigned long (*)())Cdir_ck)(pEm, &G.Player)) {
				*(long *)&pEm->Routine_0 = 0x401;
				EF->ikaku_num = 0;
			}
		}
	}

	if ((Dir2 || LL > 5000) && pEm->Move_cnt >= 26) {
		*(long *)&pEm->Routine_0 = 0x101;
		EF->At_timer = 10;
	}

}
static void Em31_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0, Sv1;	/* unused in the retail body (stack slots only) */

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0017;
		pEm->Routine_2 = 1;

		pEm->Timer0 = 60;

	case 1:
		if (pEm->Hokan_flg) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {

			if (pEm->L_pl < 3400) {
				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Move_no = 0xF0006;
					pEm->Routine_2 = 3;
					return;
				}

				*(long *)&pEm->Routine_0 = 0x201; return;
			}

			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 15;
		}

		Add_speedXZ(pEm, 0);
		break;

	case 3:
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt == 18) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 15;
		}
	}
}
static void Em31_mv04_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv0, Sv1;
	MATRIX M;

	EF->Etc_flg |= 0x80;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 = 1;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		pEm->Timer3 = 0;
		pEm->Timer2 = 0;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			G.Player.Damage_cnt &= 0x7F;
			pEm->Routine_2 = 2;

			if (G.Player.Life >= 21) {
				pEm->Routine_2 = 5;

				pEm->Timer2 = 0;

				if (!((Rnd() >> (Rnd() & 3)) & 3))
					*(long *)&pEm->Routine_0 = 0x201;

			}

			if (!*(signed char *)&pEm->Timer3) {
				*(long *)&pEm->Routine_0 = 0x101;
				EF->At_timer = 15;
			}
		}
		Add_speedXZ(pEm, 0);

		Em31_hand_left(pEm, &M);
		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Workm = M;

		if ((*(unsigned long *)pEm->pNow_seq & 0x30000) && !*(signed char *)&pEm->Timer3) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;

			if (L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv0) && !G.Player.Damage_cnt)

			{
				*(long *)&G.Player.Routine_0 = 5;
				G.Player.Neck_flg |= 0x12;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= ~1;

				pEm->Timer3 = 1;

				Snd_se_enem(8, pEm);
				Pad_vib_set0(2, 0);		/* retail-only vibration @0x80101E34/0x80101E44 */
				Pad_vib_set1(0xf, 0x78, 0);
			}

		}

		if (*(signed char *)&pEm->Timer3 && (*(unsigned long *)pEm->pNow_seq & 0x30000)) {
			G.Player.Pos_x = M.t[0];
			G.Player.Pos_z = M.t[2];

			if (!(pEm->Move_cnt & 3)) {
				pP_ptr = G.Player.pSin_parts_ptr + 4;
				Esp_call(0x30B17D0, pEm->Cdir_y, &pP_ptr->Workm, 0);
				pP_ptr = G.Player.pSin_parts_ptr + 7;
				Esp_call(0x30B17D0, pEm->Cdir_y, &pP_ptr->Workm, 0);
			}
		}

		if (!(*(unsigned long *)pEm->pNow_seq & 0x30000) && *(signed char *)&pEm->Timer3 && !*(signed char *)&pEm->Timer2) {

			*(long *)&G.Player.Routine_0 = 1;
			G.Player.Damage_cnt &= 0x7F;
			pEm->Be_flg &= ~0x1000;
			G.Player.Be_flg &= ~0x1000;
		}

		if (!(*(unsigned long *)pEm->pNow_seq & 0x30000) && *(signed char *)&pEm->Timer3)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x40);

		break;

	case 2:
		*(unsigned long *)&pEm->Move_no = 0x70009;
		pEm->Routine_2 = 3;

		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);

		G.Player.pEnemy_ptr = pEm;
		Set_base_pos(&G.Player, Kan, Seq, 0);
		*(long *)&G.Player.Routine_0 = 6;
		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

		G.Player.Damage_cnt |= 0x80;
		pEm->Damage_cnt |= 0x80;

		pEm->Be_flg |= 0x1000;
		G.Player.Be_flg |= 0x1000;
		G.Player.Cdir_y = pEm->Cdir_y + 0x800;

		G.System_flg |= 0x100;
		Pl_life_down(500, 0);

	case 3:
		Null_pos_set(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(unsigned long *)&pEm->Move_no = 0xF0017;
			pEm->Routine_2 = 4;
		}

		if (pEm->Move_cnt == 6) {
			Snd_se_enem(5, pEm);
			Em31_Tume_color(pEm);
		}

		Em31_hand(pEm, &M);
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Workm = M;

		if (pEm->Move_cnt >= 7 && !(pEm->Move_cnt & 3)) {
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x148 - Rnd();
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
		}

		break;

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x100))

			pEm->Hokan_flg = 15;

		break;

	case 5:
		*(unsigned long *)&pEm->Move_no = 0x70109;
		pEm->Routine_2 = 6;
		pEm->Timer3 = 0;
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		pEm->Timer2 = 0;
		pEm->Timer1 = 0;

	case 6:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {

			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 30;
			EF->Etc_flg &= ~0x80;

			if (*(signed char *)&pEm->Timer3) {
				EF->At_timer = (Rnd() & 7) + 10;
			}
			pEm->Damage_cnt &= 0x7F;
		}
		Add_speedXZ(pEm, 0);

		Em31_hand(pEm, &M);

		if (*(unsigned long *)pEm->pNow_seq & 0x30000) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			Sv1.vx = 0;
			Sv1.vy = 500;
			Sv1.vz = 0;
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv1)) && !G.Player.Damage_cnt)

			{
				Pl_life_down(15, 1);

				*(long *)&G.Player.Routine_0 = 0x202;
				G.Player.Neck_flg |= 0x12;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= ~1;

				pEm->Timer2 = 5;

				Snd_se_enem(4, pEm);
				Em31_Tume_color(pEm);

				pEm->Timer3 = 1;
				Pad_vib_set0(8, 0);		/* retail-only vibration @0x801023E4/0x801023F4 */
				Pad_vib_set1(8, 0xfa, 4);

			}

			Sv0.vx = 0;
			Sv0.vy = 1000;
			Sv0.vz = 0;
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			M.t[0] += Sv0.vx;
			M.t[1] += Sv0.vy;
			M.t[2] += Sv0.vz;
			Sca_get_ground(M.t, 600, 0x8000, 1);
			if (Ground_in_flg) {
				pP_ptr = pEm->pSin_parts_ptr + 6;
				Sv0.vx = 0;
				Sv0.vy = 1000;
				Sv0.vz = 0;
				pP_ptr->Workm = M;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vx += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vy += 0x80 - (Rnd() & 0xFF);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vz += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

				if (!pEm->Timer1) {
					Snd_se_enem(7, pEm);
					pEm->Timer1 = 7;
				}
			}
		}

		G.Status_flg &= ~8;
		if (pEm->Timer1) {
			pEm->Timer1--;
			G.Status_flg |= 8;
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x4E8 - Rnd() * 2;
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0;
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}

		if (pEm->Move_cnt == 10 && !*(signed char *)&pEm->Timer3) {
			*(unsigned long *)&pEm->Move_no = 0x7000B;
			pEm->Timer3 = 1;
		}

	}
}
static void Em31_mv05_br(ENEMY_WORK *pEm)
{
	register short Dir2;
	register unsigned long LL;
	SVECTOR Sv;	VECTOR V;	MATRIX M;

	LL = pEm->L_pl;

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x5F4);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!EF->life_timer) {
		pEm->Type = 1;
		*(long *)&pEm->Routine_0 = 0x601;
		return;
	}

	if (pEm->Life < 100) {
		pEm->Type = 1;
		*(long *)&pEm->Routine_0 = 0x601;
		return;
	}

	if (!Dir2 && LL > 2500)
		*(long *)&pEm->Routine_0 = 0x101;

	if (!G.Player.Damage_cnt && pEm->Timer0 < 10) {

		if (!Dir2 && LL < 2500 && (G.Room_player_flg & 4))
			*(long *)&pEm->Routine_0 = 0x401;
	}

}
static void Em31_mv05_br1(ENEMY_WORK *pEm)
{
	register unsigned long LL;
	SVECTOR Sv;	VECTOR V;	MATRIX M;
	short Dir2;

	LL = pEm->L_pl;

	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!Dir2 && LL > 2500)
		*(long *)&pEm->Routine_0 = 0x101;

}
static void Em31_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x1F0001;

		if (pEm->Type & 1)
			*(unsigned long *)&pEm->Move_no = 0x1F0002;

		pEm->Routine_2 = 1;
		pEm->Timer0 = 40;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		Spd_get(pEm, Kan, Seq, 1);
		if (pEm->Hokan_flg >= 21)
			pEm->Spd_x >>= 1;

		Joint_move(pEm, Kan, Seq, 0x10080);
		Add_speedXZ(pEm, 0);

		if ((unsigned short)pEm->Timer0-- <= 0) {	/* unsigned test of the old value (retail lhu + bnez old) */
			*(long *)&pEm->Routine_0 = 0x101;
			/* retail-only follow-up attack choice (@0x801028C8) */
			if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400) && pEm->L_pl < 3000) {
				*(long *)&pEm->Routine_0 = 0x201;
				if ((Rnd() & 1) || G.Player.Life <= 0)
					*(long *)&pEm->Routine_0 = 0x401;
			}
		}

	}
}
static void Em31_mv06_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv0, Sv1;
	MATRIX M;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF000C + (Rnd() & 1);
		pEm->Routine_2 = 1;
		pEm->Timer2 = 0;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 20;

			EF->Etc_flg &= ~0x80;
		}

		Add_speedXZ(pEm, 0);

		if (*(unsigned long *)pEm->pNow_seq & 0x30000) {

			EF->Etc_flg |= 0x80;

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			Sv1.vx = 0;
			Sv1.vy = 500;
			Sv1.vz = 0;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			if ((L_pos_ckXZM(&pP_ptr->Workm, &G.Player.Pos_x, 800, &Sv0) || L_pos_ckXZM(&pP_ptr->Workm, &G.Player.Pos_x, 800, &Sv1)) && !G.Player.Damage_cnt)

			{
				Pl_life_down(5, 0);

				if (Pl_life_down(5, 0) == 2) {
					*(long *)&G.Player.Routine_0 = 3;
				}else {
					*(long *)&G.Player.Routine_0 = 2;
				}
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				pEm->Timer2 = 2;

				Snd_se_enem(6, pEm);
				Em31_Tume_color(pEm);
				Pad_vib_set0(6, 0);		/* retail-only vibration @0x80102C68/0x80102C78 */
				Pad_vib_set1(8, 0x96, 4);

			}
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x274 - Rnd();
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0xC18, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0;
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}

	}

}
static void Em31_mv07_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv;

	EF->Dm_cnt = 0;
	EF->Etc_flg |= 0x80;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF000E;
		pEm->Routine_2 = 1;

		pP_ptr = pEm->pSin_parts_ptr + 3;
		Sv.vx = 400;
		Sv.vy = 100;
		Sv.vz = 0;
		Esp_call(0x13D0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		pEm->Neck_flg |= 2;

		pEm->Timer1 = pEm->Life;

		Snd_se_enem(12, pEm);

		G.Status_flg &= ~8;

	case 1:

		if (pEm->Move_cnt == 30 || pEm->Move_cnt == 35 || pEm->Move_cnt == 50) {

			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv.vx = 300;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x13D0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}

		if (pEm->Move_cnt >= 6 && pEm->Move_cnt < 25) {
			pP_ptr = pEm->pSin_parts_ptr + 2;
			Sv.vx = 400;
			Sv.vy = -44 - Rnd() * 2;
			Sv.vz = 128 - Rnd();
			Esp_call(0x17B8, Rnd() << 4, &pP_ptr->Workm, &Sv);
			Sv.vy = -44 - Rnd() * 2;
			Sv.vz = 128 - Rnd();
			Esp_call(0x117B8, Rnd() << 4, &pP_ptr->Workm, &Sv);

			if (pEm->Move_cnt == 15) {
				pP_ptr->Be_flg |= 0x80;
				*(unsigned long *)&pP_ptr->Free[0] = 0x101030;
				*(unsigned long *)&pP_ptr->Free[2] = 0x101020;
			}

		}

		if (pEm->Atd[0].At_w < 1100) {
			pEm->Atd[0].At_w += 5;
			pEm->Atd[0].At_d += 5;
			pEm->Atd[0].W += 5;
			pEm->Atd[0].D += 5;
		}

		pP_ptr = pEm->pSin_parts_ptr + 2;

		((short *)pP_ptr->pSa_dat_head)[8] -= (pEm->Move_cnt & 1) << 7;
		if (pEm->Move_cnt >= 61 && pEm->Move_cnt < 92) {
			((short *)pP_ptr->pSa_dat_head)[8] += 0x40;

			((short *)pP_ptr->pSa_dat_head)[6] += 0x80;
		}

		Sa_parts_allmv(pEm, pP_ptr);

		((short *)pP_ptr->pSa_dat_head)[8] += (pEm->Move_cnt & 1) << 7;

		if (pEm->Move_cnt >= 21) {
			pP_ptr = pEm->pSin_parts_ptr + 1;
			((short *)pP_ptr->pSa_dat_head)[6] += 0x40;
			if (((short *)pP_ptr->pSa_dat_head)[6] > 0x1000) {
				((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
				EF->Etc_flg |= 0x20;
			}
			Sa_parts_allmv(pEm, pP_ptr);
		}

		if (pEm->Move_cnt >= 21 && pEm->Move_cnt < 62 && !(pEm->Move_cnt & 3)) {

			pP_ptr = pEm->pSin_parts_ptr + 2;
			Sv.vx = 364 - (Rnd() & 0x7F);
			Sv.vy = 200;
			Sv.vz = 364 - (Rnd() & 0x7F);
			Esp_call(0x13D0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			Sv.vz = -236 - (Rnd() & 0x7F);
			Esp_call(0x13D0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		}

		if (pEm->Move_cnt == 40) {
			pP_ptr = pEm->pSin_parts_ptr + 1;
			pP_ptr->Be_flg |= 0x80;
			*(unsigned long *)&pP_ptr->Free[0] = 0x101040;
			*(unsigned long *)&pP_ptr->Free[2] = 0x101020;
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 10;
			pEm->Neck_flg &= ~2;
			EF->Etc_flg &= ~0x80;
			EF->Etc_flg |= 0x100;

			pEm->Life = pEm->Timer1;
			if (pEm->Life > 100)
				pEm->Life = 100;

			return;
		}
		Add_speedXZ(pEm, 0);
	}

	pEm->Life = 9000;

}
static void Em31_mv08_br(ENEMY_WORK *pEm)
{
	short No;	/* inferred: no SYM record (combined into its use) */

	if (pEm->Sce_free0 & 0x10) {
		*(long *)&pEm->Routine_0 = 0x101;

		No = pEm->Sce_free0 & 0xF;
		pEm->Pos_x = Em31_mv08_pod[ No ][0];
		pEm->Pos_z = Em31_mv08_pod[ No ][1];
	}

}
static void Em31_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short No;	/* inferred: no SYM record (combined into its single use) */
	unsigned long Joint_move();	/* block-scope declaration (retail has no switch-body block) */
	pEm->Damage_cnt |= 0x80;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x10;
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Routine_2 = 1;

		EF->Etc_flg |= 1;
		pEm->Timer1 = 5;

		pEm->Type &= ~0x4000;

	case 1:
		if ((unsigned short)pEm->Timer1-- <= 0) {	/* unsigned test of the old value (retail lhu + bnez old) */
			No = Rnd() % 3;
			pEm->Pos_x = Em31_mv08_pod[ No ][0];
			pEm->Pos_z = Em31_mv08_pod[ No ][1];
			pEm->Timer1 = (Rnd() & 0xF) + 15;

			pEm->Cdir_y = Rnd() << 4;
		}

		break;

	case 2:
		pEm->Type |= 0x4000;
		pEm->Life = -1;
		break;

	case 3:
		pEm->Timer0 = 10;
	}

}
static void Em31_mv09_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	MATRIX M;
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into each address) */

	if ((pEm->Damage_cnt & 0x7F) && pEm->Routine_2 >= 2 && pEm->Routine_2 < 4)
		pEm->Routine_2 = 4;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x10;
		pEm->Routine_2 = 1;

		pEm->Damage_cnt &= 0x7F;

		pEm->Timer2 = 0;

		Snd_se_enem(3, pEm);

		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Poly_rgb = 0x404040;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Poly_rgb = 0x606060;

	case 1:
		pEm->Cdir_y -= 0x800;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
		pEm->Cdir_y += 0x800;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		if (pEm->Move_cnt == 4) {

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
		}

		if (pEm->Move_cnt >= 5 && pEm->Move_cnt < 10)
			pEm->Pos_y += 300;

		if (pEm->Move_cnt == 8)
			Snd_se_enem(1, pEm);

		Em31_hand(pEm, &M);
		if (*(unsigned long *)pEm->pNow_seq & 0x30000) {
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv0) && !G.Player.Damage_cnt)

			{
				Pl_life_down(5, 0);

				if (Pl_life_down(5, 0) == 2)

					*(long *)&G.Player.Routine_0 = 3;
				else *(long *)&G.Player.Routine_0 = 0x202;

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				pEm->Timer2 = 5;

				Em31_Tume_color(pEm);
				Snd_se_enem(2, pEm);
				Pad_vib_set0(8, 0);		/* retail-only vibration */
				Pad_vib_set1(6, 0x96, 4);

			}
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x274 - Rnd();
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0xC18, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr + 8;
			Sv0.vx = 0x40 - (Rnd() >> 1);
			Sv0.vy = 0x40 - (Rnd() >> 1);
			Sv0.vz = 0x40 - (Rnd() >> 1);
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}
		break;

	case 2:
		pEm->Timer0 = 20;
		pEm->Routine_2 = 3;
		*(unsigned long *)&pEm->Move_no = 0x12;

		pEm->Timer2 = 0;

	case 3:
		if (pEm->Move_cnt >= 21 && pEm->Move_cnt < 34) {
			pEm->Cdir_y -= 0x800;
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
			pEm->Cdir_y += 0x800;
		}

		if (Joint_move(pEm, Kan, Seq, 0x100)) {	/* retail: Sce_free0 0x20 branches to the new routine 7 */
			pEm->Routine_2 = 4;
			if (pEm->Sce_free0 & 0x20)
				pEm->Routine_2 = 7;
		}

		Em31_hand(pEm, &M);
		if (*(unsigned long *)pEm->pNow_seq & 0x30000) {
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&M, &G.Player.Pos_x, 500, &Sv0) && !G.Player.Damage_cnt)

			{
				Pl_life_down(5, 0);

				if (Pl_life_down(2, 0) == 2)

					*(long *)&G.Player.Routine_0 = 3;
				else *(long *)&G.Player.Routine_0 = 0x202;

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				pEm->Timer2 = 5;

				Em31_Tume_color(pEm);
				Snd_se_enem(2, pEm);
				Pad_vib_set0(8, 0);		/* retail-only vibration */
				Pad_vib_set1(6, 0x96, 4);

			}
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x274 - Rnd();
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0xC18, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr + 8;
			Sv0.vx = 0x40 - (Rnd() >> 1);
			Sv0.vy = 0x40 - (Rnd() >> 1);
			Sv0.vz = 0x40 - (Rnd() >> 1);
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}
		break;

	case 4:
		*(unsigned long *)&pEm->Move_no = 0x11;
		pEm->Routine_2 = 5;
		pEm->Damage_cnt |= 0x80;
		pEm->Timer3 = 5;

	case 5:
		if (*(signed char *)&pEm->Timer3) {
			(*(signed char *)&pEm->Timer3)--;
			if (!(pEm->Sce_free0 & 0x20))	/* retail: Sce_free0 0x20 sinks slowly instead */
				pEm->Pos_y -= 300;
		}
		if (pEm->Sce_free0 & 0x20)
			pEm->Pos_y -= 10;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 6:
		*(long *)&pEm->Routine_0 = 1;

		pEm->Sce_free0 &= ~0x10;

		if (pEm->Sce_free0 & 0x20)
			*(long *)&pEm->Routine_0 = 0x20001;

		pEm->Timer0 = 60;

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
		break;

	case 7:	/* retail-only routines 7/8 (@0x80103AD8) */
		pEm->Routine_2 = 8;
		pEm->Timer0 = 20;

	case 8:
		Joint_move(pEm, Kan, Seq, 0x100);
		if ((unsigned short)pEm->Timer0-- <= 0)
			pEm->Routine_2 = 4;

	}

}
static void Em31_mv0a_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x13;
		pEm->Routine_2 = 1;

		pEm->Timer3 = 0;
		pEm->Neck_flg |= 2;

		pP_ptr = pEm->pSin_parts_ptr + 2;
		pP_ptr->Be_flg |= 0xC00;
		pP_ptr->Size_x = 0x1000;
		pP_ptr->Size_y = 0x1000;
		pP_ptr->Size_z = 0x1000;
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg |= 0x800;
		pP_ptr = pEm->pSin_parts_ptr + 3;
		pP_ptr->Be_flg |= 0x4800;
		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Be_flg |= 0x800;
		pP_ptr = pEm->pSin_parts_ptr + 1;
		pP_ptr->Be_flg |= 0xC00;
		pP_ptr->Size_x = 0x1000;
		pP_ptr->Size_y = 0x1000;
		pP_ptr->Size_z = 0x1000;
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x400;
		pP_ptr->Size_x = 0x1000;
		pP_ptr->Size_y = 0x1000;
		pP_ptr->Size_z = 0x1000;

		pEm->Timer0 = 150;
		pEm->Timer2 = 0;

	case 1:
		if (pEm->Sce_free0 & 1)

			pEm->Routine_2 = 2;

		break;

	case 2:
		pP_ptr = pEm->pSin_parts_ptr + 2;
		if (pEm->Move_cnt >= 6 && pEm->Move_cnt < 35) {
			((short *)pP_ptr->pSa_dat_head)[6] -= (pEm->Move_cnt - 10) * 8;

			((short *)pP_ptr->pSa_dat_head)[8] += (pEm->Move_cnt - 10) * 8;
		}

		if (pEm->Move_cnt >= 36 && pEm->Move_cnt < 60) {
			((short *)pP_ptr->pSa_dat_head)[6] += (pEm->Move_cnt - 35) * 4;

			((short *)pP_ptr->pSa_dat_head)[8] -= (pEm->Move_cnt - 35) * 4;
		}

		if (pEm->Move_cnt >= 61 && pEm->Move_cnt < 75) {
			((short *)pP_ptr->pSa_dat_head)[6] -= (pEm->Move_cnt - 60) * 8;

			((short *)pP_ptr->pSa_dat_head)[8] += (pEm->Move_cnt - 60) * 8;
		}

		if (pEm->Move_cnt >= 76) {
			((short *)pP_ptr->pSa_dat_head)[6] += (pEm->Move_cnt - 75) * 2;

			((short *)pP_ptr->pSa_dat_head)[8] -= (pEm->Move_cnt - 75) * 2;
		}

		((short *)pP_ptr->pSa_dat_head)[6] -= ((pEm->Move_cnt & 1) * 2 - 1) * 60;

		((short *)pP_ptr->pSa_dat_head)[8] += ((pEm->Move_cnt & 1) * 2 - 1) * 60;

		Sa_parts_allmv(pEm, pP_ptr);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Move_no = 0xF0014;
			pEm->Routine_2 = 3;

			pEm->Timer3 = 0;

			pEm->Timer0 = 0;

		}

		Add_speedXZ(pEm, 0); break;

	case 3:
		Spd_get(pEm, Kan, Seq, 0);
		if (!pEm->Move_cnt) {
			pEm->Move_cnt++;
			Spd_get(pEm, Kan, Seq, 0);
			pEm->Move_cnt--;
			pEm->Hokan_flg = 15;
		}
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		pP_ptr = pEm->pSin_parts_ptr + 2;
		if (pEm->Timer0 > 30 && !((signed char)pEm->Timer3 & 1)) {
			((short *)pP_ptr->pSa_dat_head)[6] -= (pEm->Timer0 - 30) * 4;

			((short *)pP_ptr->pSa_dat_head)[8] += (pEm->Timer0 - 30) * 4;

			if (((short *)pP_ptr->pSa_dat_head)[8] > 0x1000) {
				((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
				((short *)pP_ptr->pSa_dat_head)[6] = 0;

				pEm->Timer3 |= 1;
			}
		}

		if (((signed char)pEm->Timer3 & 1) && !(pEm->Timer3 & 4)) {
			((short *)pP_ptr->pSa_dat_head)[8] -= 0x80;
			if (((short *)pP_ptr->pSa_dat_head)[8] < 0) {
				((short *)pP_ptr->pSa_dat_head)[8] = 0;
				pEm->Timer3 |= 4;
			}
		}

		((short *)pP_ptr->pSa_dat_head)[6] += ((pEm->Move_cnt & 1) * 2 - 1) * 120;

		((short *)pP_ptr->pSa_dat_head)[8] -= ((pEm->Move_cnt & 1) * 2 - 1) * 120;

		Sa_parts_allmv(pEm, pP_ptr);

		if (pEm->Timer0 == 41) {
			pP_ptr = pEm->pSin_parts_ptr + 3;
			Esp_call(0x31FA0, pEm->Cdir_y, &pP_ptr->Workm, 0);
		}

		if (pEm->Timer0 >= 46) {
			pP_ptr = pEm->pSin_parts_ptr + 3;
			if (pP_ptr->Be_flg & 0x400) {
				pP_ptr->Be_flg &= ~0x4000;
				pP_ptr->Size_y += (pEm->Timer0 - 45) * 2;
				if (pP_ptr->Size_y >= 0x1000) {
					pP_ptr->Be_flg &= ~0x400;

					*(unsigned long *)&pEm->Move_no = 0xF0015;
					pEm->Routine_2 = 4;

					pEm->Timer3 = 0;
				}
			}
		}

		pEm->Timer0++;

		break;

	case 4:
		Spd_get(pEm, Kan, Seq, 0);
		if (!pEm->Move_cnt) {
			pEm->Move_cnt++;
			Spd_get(pEm, Kan, Seq, 0);
			pEm->Move_cnt--;
		}
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Move_no = 0xF0016;
			pEm->Routine_2 = 5;

			pEm->Timer3 = 0;

			pEm->Timer0 = 0;
			pEm->Neck_flg &= ~2;
		}

		Add_speedXZ(pEm, 0); break;

	case 5:
		if (pEm->Timer0++ - 45 == 0) {	/* compares the old value (retail: sll/sra of the pre-increment halfword) */

			pEm->Timer3 = 0;
			pEm->Routine_2 = 6;
			pEm->Neck_flg |= 2;

			Em31_me_tate_d = 0;
			Em31_me_yoko_d = 0;
		}

		if (pEm->Timer0 == 20) {
			*(signed char *)&Em31_me_tate_d = -15;
			Em31_me_yoko_d = 0;
		}
		if (pEm->Timer0 == 30) {
			*(signed char *)&Em31_me_tate_d = -15;
			Em31_me_yoko_d = 8;
		}

		pP_ptr = pEm->pSin_parts_ptr + 4;
		if (pEm->Timer0 > 0 && !(pEm->Timer3 & 1)) {
			((short *)pP_ptr->pSa_dat_head)[6] -= pEm->Timer0 * 16;

			if (((short *)pP_ptr->pSa_dat_head)[6] < 0) {
				((short *)pP_ptr->pSa_dat_head)[6] = 0;
				pEm->Timer3 |= 1;
			}
		}
		Sa_parts_allmv(pEm, pP_ptr);

		break;

	case 6:
		Sv.vx = -100;
		Sv.vy = 100;
		Sv.vz = 0;

		pP_ptr = pEm->pSin_parts_ptr + 8;
		if (pEm->Move_cnt >= 16 && (pP_ptr->Be_flg & 0x400)) {
			pP_ptr->Size_y += (pEm->Move_cnt - 15) << 5;
			if (pP_ptr->Size_y >= 0x1000)
				pP_ptr->Be_flg &= ~0x400;

			pP_ptr->Size_x += (pEm->Move_cnt - 15) << 5;
			pP_ptr->Size_z += (pEm->Move_cnt - 15) << 5;
			if (pP_ptr->Size_y >= 0x1000) {
				pP_ptr->Size_x = 0x1000;
				pP_ptr->Size_z = 0x1000;
			}

			if (pEm->Move_cnt & 1) {
				Esp_call((Rnd() * 8 + 0x800) | 0x140000, 0, &pP_ptr->Workm, &Sv);
			}
		}

		pP_ptr = pEm->pSin_parts_ptr + 6;
		if (pEm->Move_cnt >= 21 && (pP_ptr->Be_flg & 0x400)) {
			pP_ptr->Size_y += (pEm->Move_cnt - 20) << 5;
			if (pP_ptr->Size_y >= 0x1000)
				pP_ptr->Be_flg &= ~0x400;

			pP_ptr->Size_x += (pEm->Move_cnt - 20) << 5;
			pP_ptr->Size_z += (pEm->Move_cnt - 20) << 5;
			if (pP_ptr->Size_y >= 0x1000) {
				pP_ptr->Size_x = 0x1000;
				pP_ptr->Size_z = 0x1000;
			}
			if (pEm->Move_cnt & 1) {
				Esp_call((Rnd() * 8 + 0x800) | 0x140000, 0, &pP_ptr->Workm, &Sv);
			}
		}

		pP_ptr = pEm->pSin_parts_ptr + 7;
		if (pEm->Move_cnt >= 41 && (pP_ptr->Be_flg & 0x400)) {
			pP_ptr->Size_y += (pEm->Move_cnt - 40) << 5;
			if (pP_ptr->Size_y >= 0x1000)
				pP_ptr->Be_flg &= ~0x400;

			pP_ptr->Size_x += (pEm->Move_cnt - 40) << 5;
			pP_ptr->Size_z += (pEm->Move_cnt - 40) << 5;
			if (pP_ptr->Size_y >= 0x1000) {
				pP_ptr->Size_x = 0x1000;
				pP_ptr->Size_z = 0x1000;
			}
			if (pEm->Move_cnt & 1) {
				Esp_call((Rnd() * 8 + 0x800) | 0x140000, 0, &pP_ptr->Workm, &Sv);
			}
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {

			pEm->Routine_2 = 7;
			pEm->Timer0 = 0;

			pEm->Sce_free0 |= 2;

			*(unsigned long *)&pEm->Move_no = 0xFF0000;
		}

		Add_speedXZ(pEm, 0); break;

	case 7:
		if (pEm->Timer0 > 30)
			Joint_move(pEm, Kan, Seq, 0x10);

		pEm->Timer0++;

		if (pEm->Sce_free0 & 4) {
			pEm->Routine_2 = 8;

			pEm->Spd_x = 200;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;

			pEm->Timer0 = 0;

			*(unsigned long *)&pEm->Move_no = 0xF;
			Joint_move(pEm, Kan, Seq, 0x100);

			pP_ptr = pEm->pSin_parts_ptr + 2;
			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr = pEm->pSin_parts_ptr + 4;
			pP_ptr->Be_flg &= ~0x800;
			pP_ptr = pEm->pSin_parts_ptr + 3;
			pP_ptr->Be_flg &= ~0x4800;
			pP_ptr = pEm->pSin_parts_ptr + 9;
			pP_ptr->Be_flg &= ~0x800;
			pP_ptr = pEm->pSin_parts_ptr + 1;
			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr = pEm->pSin_parts_ptr;
			pP_ptr->Be_flg &= ~0x400;

			Snd_se_enem(9, pEm);
		}

		break;

	case 8:
		pEm->Pos_y += pEm->Timer0 * 30;
		pEm->Timer0++;
		if (pEm->Pos_y >= -1999)
			pEm->Routine_2 = 9;

		Add_speedXZ(pEm, 0); break;

	case 9:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_2 = 10;
			pEm->Neck_flg &= ~2;
		}

		pEm->Pos_y += pEm->Timer0 * 30;
		pEm->Timer0++;
		if (pEm->Pos_y >= -399)
			pEm->Pos_y = 0;

		if (pEm->Move_cnt < 12)
			Add_speedXZ(pEm, 0);

		break;

	case 10:
		*(long *)&pEm->Routine_0 = 0x101;
		EF->At_timer = 10;
		pEm->Neck_flg &= ~2;

	}

	pP_ptr = pEm->pSin_parts_ptr + 2;
	pP_ptr->Size_x = 0x1000 - (signed char)Em31_iki[ (signed char)pEm->Timer2 ];
	pP_ptr->Size_y = 0x1000 - (signed char)Em31_iki[ (signed char)pEm->Timer2 ];
	pP_ptr->Size_z = 0x1000 - (signed char)Em31_iki[ (signed char)pEm->Timer2 ];
	pP_ptr = pEm->pSin_parts_ptr + 1;
	pP_ptr->Size_x = 0x1000 - (signed char)Em31_iki[ (signed char)pEm->Timer2 ];
	pP_ptr->Size_y = 0x1000 - (signed char)Em31_iki[ (signed char)pEm->Timer2 ];
	pP_ptr->Size_z = 0x1000 - (signed char)Em31_iki[ *(signed char *)&pEm->Timer2 ];
	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Size_x = 0x1000 - (signed char)Em31_iki[ *(signed char *)&pEm->Timer2 ];
	pP_ptr->Size_y = 0x1000 - (signed char)Em31_iki[ *(signed char *)&pEm->Timer2 ];
	pP_ptr->Size_z = 0x1000 - (signed char)Em31_iki[ *(signed char *)&pEm->Timer2 ];
	pEm->Timer2++;
	pEm->Timer2 &= 0x3F;

}
static void Em31_mv0b_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv0, Sv1;
	MATRIX M;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70109;
		pEm->Routine_2 = 1;
		pEm->Timer3 = 0;
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		pEm->Timer2 = 0;
		pEm->Timer1 = 0;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 10;

			EF->Etc_flg &= ~0x80;
		}

		Add_speedXZ(pEm, 0);

		if (!*(signed char *)&pEm->Timer3)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		Em31_hand(pEm, &M);

		if (*(unsigned long *)pEm->pNow_seq & 0x30000)
		{
			EF->Etc_flg |= 0x80;

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			Sv1.vx = 0;
			Sv1.vy = 500;
			Sv1.vz = 0;
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 800, &Sv1)) && !G.Player.Damage_cnt)

			{
				Pl_life_down(30, 1);	/* retail: 10 -> 30 */

				*(long *)&G.Player.Routine_0 = 0x202;
				G.Player.Neck_flg |= 0x12;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= ~1;

				pEm->Timer2 = 5;

				Snd_se_enem(4, pEm);
				Pad_vib_set0(8, 0);		/* retail-only vibration @0x80104B6C/0x80104B7C */
				Pad_vib_set1(8, 0xfa, 4);

			}

			Sv0.vx = 0;
			Sv0.vy = 1000;
			Sv0.vz = 0;
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			M.t[0] += Sv0.vx;
			M.t[1] += Sv0.vy;
			M.t[2] += Sv0.vz;
			Sca_get_ground(M.t, 600, 0x8000, 1);
			if (Ground_in_flg) {
				pP_ptr = pEm->pSin_parts_ptr + 6;
				Sv0.vx = 0;
				Sv0.vy = 1000;
				Sv0.vz = 0;
				pP_ptr->Workm = M;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vx += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vy += 0x80 - (Rnd() & 0xFF);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vz += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

				if (!pEm->Timer1) {
					Snd_se_enem(7, pEm);
					pEm->Timer1 = 7;
				}

			}
		}

		G.Status_flg &= ~8;
		if (pEm->Timer1) {
			pEm->Timer1--;
			G.Status_flg |= 8;
		}

		if (*(signed char *)&pEm->Timer2) {
			(*(signed char *)&pEm->Timer2)--;
			Em31_hand(pEm, &M);
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0x4E8 - Rnd() * 2;
			Sv0.vz = 0x100 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Workm = M;
			Esp_call(Rnd() * 4 + 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 0x100 - Rnd() * 2;
			Sv0.vy = 0;
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, 0, &pP_ptr->Workm, &Sv0);

		}

		if (pEm->Move_cnt == 10 && !*(signed char *)&pEm->Timer3) {
			*(unsigned long *)&pEm->Move_no = 0x7000B;
			pEm->Timer3 = 1;
		}

	}
}
static void Em31_mv0c_br(ENEMY_WORK *pEm)
{

}
static void Em31_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0, Sv1;	/* unused in the retail body (stack slots only) */
	MATRIX M;
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into each address) */

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70000;

		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Poly_rgb = 0x404040;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Poly_rgb = 0x606060;

		pEm->Spd_x = 0;
		pEm->Spd_z = 0;

		pEm->Timer1 = 5;

		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;

		break;

	case 1:
		if (pEm->Move_cnt == 17) {

			pP_ptr = pEm->pSin_parts_ptr + 4;
			pP_ptr->Be_flg &= ~0x4000;
		}
		if (pEm->Move_cnt == 14) {

			pP_ptr = pEm->pSin_parts_ptr + 5;
			pP_ptr->Be_flg &= ~0x4000;
		}
		if (pEm->Move_cnt == 11) {

			pP_ptr = pEm->pSin_parts_ptr + 6;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 7;
			pP_ptr->Be_flg &= ~0x4000;
			pP_ptr = pEm->pSin_parts_ptr + 8;
			pP_ptr->Be_flg &= ~0x4000;
		}

		if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x200)) {

			pEm->Routine_2 = 2;
			*(unsigned long *)&pEm->Move_no = 0x70001;
			pEm->Timer0 = 10;
			pEm->Timer1 = 5;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt >= 11)
			pEm->Spd_x += 70;

		break;

	case 2:
		if ((unsigned short)pEm->Timer0-- <= 0) {	/* unsigned test of the old value (retail lhu + bnez old) */
			pEm->Routine_2 = 3;
			pEm->Spd_x = 0;
		}

		pEm->Pos_x += 16 - (Rnd() & 0x1F);
		pEm->Pos_z += 16 - (Rnd() & 0x1F);

		break;

	case 3:
		if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x200)) {

			pEm->Routine_2 = 4;
			*(unsigned long *)&pEm->Move_no = 0x70001;
		}
		Add_speedXZ(pEm, 0);
		pEm->Spd_x -= 4;

		if (pEm->Move_cnt == 25) {
			pP_ptr = pEm->pSin_parts_ptr + 4;
			pP_ptr->Be_flg |= 0x4000;
		}
		if (pEm->Move_cnt == 30) {
			pP_ptr = pEm->pSin_parts_ptr + 5;
			pP_ptr->Be_flg |= 0x4000;
		}
		break;

	case 4:
		pEm->Routine_2 = 0;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 7;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 8;
		pP_ptr->Be_flg |= 0x4000;

	}
}
static int Em31_Tume_color(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into each address) */

	if (!(EF->Etc_flg & 0x40)) {

		pP_ptr = pEm->pSin_parts_ptr + 6;

		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x30;
		pP_ptr++;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x30;
		pP_ptr++;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x30;

		EF->Etc_flg |= 0x40;
	}
}
static void Em31_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv, Sv0;
	short Ck;
	MATRIX M;
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into each address) */

	if (!pEm->Routine_2) {

		pP_ptr = pEm->pSin_parts_ptr + 1;
		Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		if (pEm->Type != 2) {
			Sv.vx = 0;
			Sv.vy = Ck;
			Sv.vz = 0;
			RotMatrix(&Sv, &M);
			Sv.vx = 200;
			Sv.vy = -200;
			Sv0.vx = (pEm->Spd_x << 2) + 300;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (!(pEm->Damage_no % 3)) {
				Sv.vx = 150;
				Sv.vy = 700;
				Sv0.vx = (pEm->Spd_x << 1) + 150;
				Sv0.vy = 700;
			}
			ApplyMatrixSV(&M, &Sv, &Sv);
			Sv.vx += pP_ptr->Workm.t[0];
			Sv.vy += pP_ptr->Workm.t[1];
			Sv.vz += pP_ptr->Workm.t[2];
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			Sv0.vx += pP_ptr->Workm.t[0];
			Sv0.vy += pP_ptr->Workm.t[1];
			Sv0.vz += pP_ptr->Workm.t[2];

			if (pEm->Routine_1 != 16 && pEm->Routine_1 != 12) {
				Esp_call((Rnd() * 8 + 0x17D0) | 0x10000, Ck, &GsIDMATRIX, &Sv);
				Sv.vy += 0x40 - (Rnd() & 0x3F);
				Esp_call(Rnd() * 8 + 0x17D0, Ck, &GsIDMATRIX, &Sv);

				Esp_call(Rnd() * 8 + 0x17D0, Ck, &GsIDMATRIX, &Sv0);
				Sv.vy += 0x40 - (Rnd() & 0x3F);
				Esp_call(Rnd() * 8 + 0x17D0, Ck, &GsIDMATRIX, &Sv0);

				if (pEm->Routine_1 != 1) {
					Snd_se_enem(2, pEm); } }

		}else {
			pP_ptr = pEm->pSin_parts_ptr + 5;
			Sv.vx = 0x40 - (Rnd() >> 1);
			Sv.vy = 0;
			Sv.vz = 0x40 - (Rnd() >> 1);
			Esp_call((Rnd() * 8 + 0x17D0) | 0x10000, Ck, &pP_ptr->Workm, &Sv);
			Sv.vy += 0x40 - (Rnd() & 0x3F);
			Esp_call(Rnd() * 8 + 0x17D0, Ck, &pP_ptr->Workm, &Sv);
			pP_ptr = pEm->pSin_parts_ptr + 6;
			Esp_call(Rnd() * 8 + 0x17D0, Ck, &pP_ptr->Workm, &Sv);

			Snd_se_enem(4, pEm);

		}

		EF->Me_r_0 = 1;
		EF->Me_r_1 = 0;

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6)	/* retail adds 6 (sltiu Routine_1-5 < 2 @0x80105588) */
			EF->Mg_rout0 = 1;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em31_bomdm(pEm);

			if (pEm->Routine_1 == 17)	/* retail-only (@0x801055D4) */
				func_80105B7C(pEm);

			if (pEm->Routine_1 == 10)
				Em31_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em31_ryudm(pEm);

		}

		if (pEm->Routine_1 == 14)
			Em31_spdm(pEm);

		if (pEm->Routine_1 == 16 && !EF->Fire_c) {
			Em31_firedm(pEm);
			EF->Fire_c = 45;
		}

		if (!(EF->Etc_flg & 4) && pEm->Life < 200) {
			EF->Dm_cnt = 50;
			EF->Etc_flg |= 4;
		}

		if (EF->Etc_flg & 0x200) {	/* retail-only: restore the routine and go straight to Em31_dm00 (@0x80105694) */
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em31_dm00(pEm, Kan, Seq);
			return;
		}

		if (!(EF->Etc_flg & 1)) {
			EF->Dm_cnt += Em31_Dm_cnt[ pEm->Routine_1 - 1 ];

			if (!(EF->Etc_flg & 2)) {
				EF->Etc_flg |= 2;
				EF->Dm_timer0 = 15;
			}
		}

		if (EF->Etc_flg & 0x80) {
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em31_Move(pEm, Kan, Seq);
			return;
		}

		if (EF->Dm_cnt < 12) {
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em31_Move(pEm, Kan, Seq);
			return;
		}
		EF->Dm_cnt = 0;
		EF->Dm_timer0 = 0;
		EF->Etc_flg &= ~2;
	}

	Em31_dm00(pEm, Kan, Seq);

}
static void Em31_firedm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	/* retail: seven effects with new ids/parts (@0x80105794) */
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x3AF8) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[13].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[10].Workm, 0);
	{
		PARTS_W *pP_ptr2;	/* [INFERRED] used carrier: retail holds this part base in s1, not pP_ptr's s2 */

		pP_ptr2 = pEm->pSin_parts_ptr;
		Esp_call((Rnd() * 4 + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr2[3].Workm, 0);
	}

}
static void Em31_ryudm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[4].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[10].Workm, 0);

}
static void Em31_bomdm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1BB8) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[4].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x830) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x448) | 0x40F0000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x448) | 0x40F0000, pEm->Cdir_y, &pP_ptr[4].Workm, 0);
}
/* retail-only (0x80105B7C in slot 1, 0x80112B7C in slot 0), name unknown: damage effect set called
 * between Em31_bomdm and Em31_firedm in Em31_Damage (@0x801055D4) and Em31_die00 (@0x80106A30) */
static void func_80105B7C(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x3AF8) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[1].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[3].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[10].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call((Rnd() * 4 + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);

}
static void Em31_spdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = -472 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[2].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[2].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr[0].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr[1].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[12].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[13].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[10].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[4].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call((Rnd() * 8 + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[16].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr + 12;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr + 4;
	Esp_call((Rnd() * 4 + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);

}
static void Em31_mgdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv0, Sv1, Sv2;
	MATRIX M;
	short Dir;	/* inferred name: no SYM record (copy of the stored value, propagated) */
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into the address) */

	switch (EF->Mg_rout0) {
	case 0:
		break;

	case 1:
		EF->Mg_rout0 = 2;
		EF->Mg_timer0 = 0;
		EF->Mg_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

	case 2:
		Sv0.vx = 0;
		Dir = Sv0.vy = EF->Mg_dir;
		Sv0.vz = 0;
		RotMatrix(&Sv0, &M);

		Sv0.vx = -(*(signed char *)&EF->Mg_timer0 * 100);
		Sv0.vy = 200;
		ApplyMatrixSV(&M, &Sv0, &Sv1);

		Sv0.vx = -100;
		Sv0.vy = -500;
		Sv0.vz = 0;

		if (!(pEm->Damage_no % 3))
			Sv0.vy = 500;

		ApplyMatrixSV(&M, &Sv0, &Sv2);

		pP_ptr = pEm->pSin_parts_ptr + 2;
		Sv0.vx = Sv1.vx + pEm->Pos_x;
		Sv0.vy = Sv1.vy + pP_ptr->Workm.t[1];
		Sv0.vz = Sv1.vz + pEm->Pos_z;
		Esp_call((0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx + 0x80 - Rnd();
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz + 0x80 - Rnd();
		Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		if ((signed char)EF->Mg_timer0 == 1) {
			Esp_call(0x217D0, Dir + 0x800, &GsIDMATRIX, &Sv0);
			Esp_call(0x20C18, Dir + 0x800, &GsIDMATRIX, &Sv0);
		}

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 1);
		Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 1);
		Esp_call((0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 3 + 0x80 - Rnd();
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 3 + 0x80 - Rnd();
		Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 2);
		Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 2);
		Esp_call((0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 5 + 0x80 - Rnd();
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 5 + 0x80 - Rnd();
		Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 6;
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 6;
		Esp_call((0x65C - (signed char)EF->Mg_timer0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 7 + 0x80 - Rnd();
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 7 + 0x80 - Rnd();
		Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - (signed char)EF->Mg_timer0 * 30 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		if ((*(signed char *)&EF->Mg_timer0)++ >= 17)
			EF->Mg_rout0 = 0;

	}

}
static void Em31_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x30004 - ((unsigned long (*)())Cdir_ck)(pEm, &G.Player);
		pEm->Routine_2 = 1;
		/* retail drops the Damage_cnt |= 0x80 here and sets Etc_flg 0x200 instead (@0x801066B0) */
		Snd_se_enem(9, pEm);

		EF->ikaku_num++;
		EF->Etc_flg |= 0x200;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Damage_cnt &= 0x7F;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt == 15) {

			if (pEm->Routine_1 == 14)
				Em31_spdm(pEm);

			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Damage_cnt &= 0x7F;
			EF->Etc_flg &= 0xfdff;
		}
	}
}
static void Em31_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em31_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;	/* unused in the retail body (stack slots only) */
	MATRIX M;

	Em31_die00(pEm, Kan, Seq);
}
static void Em31_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0, Sv1;	/* Sv0/Sv1 unused in the retail body (stack slots only) */
	MATRIX M;
	unsigned char Ck;
	KAGE_WORK *pKage;
	SVECTOR Sv;
	PARTS_W *pP_ptr;	/* inferred name: no SYM record (folded into each address) */

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_2) {
	case 0:
		/* retail: the hit effects now come before the Etc_flg 0x100 test (@0x80106884) */
		pP_ptr = pEm->pSin_parts_ptr + 1;
		Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Ck;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 100;
		Sv.vy = 0;
		if (!(pEm->Damage_no % 3)) {
			Sv.vx = 100;
			Sv.vy = 700;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP_ptr->Workm.t[0];
		Sv.vy += pP_ptr->Workm.t[1];
		Sv.vz += pP_ptr->Workm.t[2];

		if (pEm->Routine_1 != 16) {
			Esp_call((Rnd() * 8 + 0x17D0) | 0x10000, Ck, &GsIDMATRIX, &Sv);
			Sv.vy += 0x40 - (Rnd() & 0x3F);
			Esp_call(Rnd() * 8 + 0x17D0, Ck, &GsIDMATRIX, &Sv);
		}

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6)	/* retail adds 6 */
			EF->Mg_rout0 = 1;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em31_bomdm(pEm);

			if (pEm->Routine_1 == 17)	/* retail-only (@0x80106A30) */
				func_80105B7C(pEm);

			if (pEm->Routine_1 == 10)
				Em31_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em31_ryudm(pEm);

		}

		if (pEm->Routine_1 == 14)
			Em31_spdm(pEm);

		if (pEm->Routine_1 == 16)
			Em31_firedm(pEm);

		if (!(EF->Etc_flg & 0x100)) {
			pEm->Life = 10;
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			return;
		}

		*(unsigned long *)&pEm->Move_no = 0x3000A;
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;

		EF->Etc_flg |= 0x11;

		EF->Me_r_0 = 2;
		EF->Me_r_1 = 0;

		Em_dead_flg_on(pEm);
		pEm->Neck_flg |= 2;

	case 1:

		if (pEm->Move_cnt < 15) {

			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv.vx = 0x40 - (Rnd() >> 1);
			Sv.vy = 0;
			Sv.vz = 0x40 - (Rnd() >> 1);
			Esp_call(((Rnd() & 1) << 16) | 0xC00, Rnd() << 4, &pP_ptr->Workm, &Sv);
		}

		if (pEm->Move_cnt >= 81 && pEm->Move_cnt < 109)
		{
			if (pEm->Move_cnt == 81)
				Snd_se_enem(9, pEm);

			pP_ptr = pEm->pSin_parts_ptr + 3;
			Sv.vx = 0x40 - (Rnd() >> 1);
			Sv.vy = -200;
			Sv.vz = 0x40 - (Rnd() >> 1);
			Esp_call(((Rnd() & 1) << 16) | 0xC00, Rnd() << 4, &pP_ptr->Workm, &Sv);

			pP_ptr = pEm->pSin_parts_ptr + 2;
			Sv.vx = 0x100 - Rnd() * 2;
			Sv.vy = 0xD4 - Rnd() * 4;
			Sv.vz = 0x100 - Rnd() * 2;
			Esp_call(((Rnd() & 1) << 16) | 0x17B8, Rnd() << 4, &pP_ptr->Workm, &Sv);
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			pEm->Routine_2 = 2;
			EF->Etc_flg &= ~0x20;
			pEm->Be_flg |= 2;

			EF->Me_r_0 = 0;
			EF->Me_r_1 = 0;

			pEm->Timer0 = 210;
		}

		Add_speedXZ(pEm, 0);
		break;

	case 2:
		if (pKage->Prim[0].r0 != 0x10) {
			pKage->Prim[0].r0--;
			pKage->Prim[1].r0--;
		}
		if (*(unsigned long *)&pKage->Prim[1].r0 & 1) {
			*(unsigned long *)&pKage->Prim[0].r0 += 0x10100;
			*(unsigned long *)&pKage->Prim[1].r0 += 0x10100;
		}

		if (pEm->Timer0) {
			pKage->Half_x += 5;
			pKage->Half_z += 5;
			pEm->Timer0--;
		}

	}
}
static void Em31_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em31_sce[ pEm->Routine_1 ](pEm, Kan, Seq); }
static void Em31_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em31_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
void EM_PLDAMEGE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em31_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }
static void Em31_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x30000;

		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;

		pEm->Neck_flg = 1;
		Snd_se_stad(0x4000001, &pEm->Pos_x);

	case 1:
		pEm->Move_cnt = 0;
		Joint_move(pEm, Kan, Seq, 0x400);
		Goto00(pEm, G.pEnemy[2]->Pos_x, G.pEnemy[2]->Pos_z, 0x100);

	}
}
void EM_PLDIE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em31_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }
static void Em31_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv0;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70000;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		pEm->Routine_2 = 1;
		Pad_vib_set0(0x1e, 0);		/* retail-only vibration (pldie00 case 0) */
		Pad_vib_set1(0xf, 0x78, 0xf);

	case 1:

		if (pEm->Move_cnt == 10) {
			Snd_se_stad(0x4030001, &pEm->Pos_x);

			pP_ptr = pEm->pSin_parts_ptr + 8;
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x30;
			*(long *)&pP_ptr->Free[2] = 0x101030;

			pP_ptr = pEm->pSin_parts_ptr;
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x60;
			*(long *)&pP_ptr->Free[2] = 0x101030;
			Pad_vib_set1(0xa, 0xfa, 0);	/* retail-only vibration @0x80107020..0x8010705C */
			Pad_vib_fade_set(0x1e, 0xfa, 0x6e, 0xa);
			Pad_vib_set1(0x28, 0x6e, 0x28);
			Pad_vib_set0(8, 0xa);
			Pad_vib_set0(3, 0x14);
			Pad_vib_set0(2, 0x19);
			Pad_vib_set0(3, 0x1e);
			Pad_vib_set0(3, 0x23);
			Pad_vib_set0(5, 0x28);
			Pad_vib_set0(3, 0x37);
			Pad_vib_set0(3, 0x46);

		}

		G.Player.Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		G.Player.Cdir_y -= 0x800;
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		if (pEm->Move_cnt >= 7 && !(pEm->Move_cnt & 3)) {
			pP_ptr = pEm->pSin_parts_ptr;
			Sv0.vx = 200;
			Sv0.vy = 0x148 - Rnd();
			Sv0.vz = 0x100 - Rnd() * 2;
			Esp_call(Rnd() * 4 + 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		}

		if (pEm->Move_cnt == 89 || pEm->Move_cnt == 98) {
			Snd_se_walk(2, 0, pEm);

			pP_ptr = pEm->pSin_parts_ptr;
			Sv0.vx = 0x80 - Rnd();
			Sv0.vy = 0x148 - Rnd();
			Sv0.vz = 0x80 - Rnd();
			Esp_call(Rnd() * 4 + 0x1000, Rnd() << 4, &pP_ptr->Workm, &Sv0);

			Sv0.vx = 0x80 - Rnd();
			Sv0.vy = 0x148 - Rnd();
			Sv0.vz = 0x80 - Rnd();
			Esp_call((Rnd() * 4 + 0x1000) | 0x10000, Rnd() << 4, &pP_ptr->Workm, &Sv0);
			Pad_vib_set0(3, 0);		/* retail-only vibration @0x80107238.. */
			Pad_vib_set1(6, 0xfa, 0);
			if (pEm->Move_cnt == 98)
				Pad_vib_fade_set(0x3c, 0xfa, 0, 6);
		}

		break;

	case 2:
		pEm->Timer0 = 30;
		pEm->Routine_2 = 3;

	case 3:
		if ((unsigned short)pEm->Timer0-- <= 0)	/* unsigned test of the old value (retail lhu + bnez old) */
			pEm->Routine_2 = 4;

	case 4:
		break;

	}
}
static void Em31_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	unsigned char Me3[3] = { 2, 46, 54 };

	unsigned char Me4[15] = { 14, 23, 26, 28, 29, 30, 31, 32, 41, 42, 43, 44, 45, 46, 48 };
	register short Tmp_t, Tmp_y;
	short Dt;	/* inferred name: no retail SYM record (narrowed temporary combined away; as EM28.C) */

	Tmp_t = Em31_me_tate_a;
	Tmp_y = Em31_me_yoko_a;
	if ((signed char)Em31_me_tate_d < (signed char)Em31_me_tate)
		Em31_me_tate_a = -1;

	if ((signed char)Em31_me_tate < (signed char)Em31_me_tate_d)
		Em31_me_tate_a = 1;

	if ((signed char)Em31_me_yoko > (signed char)Em31_me_yoko_d)
		Em31_me_yoko_a = -1;

	if ((signed char)Em31_me_yoko < (signed char)Em31_me_yoko_d)
		Em31_me_yoko_a = 1;

	if (-Em31_me_tate_a == Tmp_t)
		Em31_me_tate_mv = Em31_me_tate_mv * 2 / 3;

	if (-Em31_me_yoko_a == Tmp_y)
		Em31_me_yoko_mv = Em31_me_yoko_mv * 2 / 3;

	if ((signed char)Em31_me_tate == (signed char)Em31_me_tate_d && __builtin_abs(Em31_me_tate_mv) < 3) {
		Em31_me_tate = Em31_me_tate_d;
		Em31_me_tate_a = 0;
		Em31_me_tate_mv = 0;
	}
	if ((signed char)Em31_me_yoko == (signed char)Em31_me_yoko_d && __builtin_abs(Em31_me_yoko_mv) < 3) {
		Em31_me_yoko = Em31_me_yoko_d;
		Em31_me_yoko_a = 0;
		Em31_me_yoko_mv = 0;
	}

	Em31_me_tate_mv += Em31_me_tate_a;
	Em31_me_yoko_mv += Em31_me_yoko_a;

	Tmp_t = (signed char)Em31_me_tate;
	Tmp_y = (signed char)Em31_me_yoko;

	Em31_me_tate += Em31_me_tate_mv;
	Em31_me_yoko += Em31_me_yoko_mv;

	if ((signed char)Em31_me_tate > 20)
		Em31_me_tate = 20;

	if ((signed char)Em31_me_tate < -17)
		Em31_me_tate = -17;

	if ((signed char)Em31_me_yoko > 20)
		Em31_me_yoko = 20;

	if ((signed char)Em31_me_yoko < -17)
		Em31_me_yoko = -17;

	Dt = (signed char)Em31_me_tate - Tmp_t;
	Tmp_y = (signed char)Em31_me_yoko - Tmp_y;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me3);

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me4);

}
static void Em31_core_me_sa(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	Em31_core_me_sa_tbl[ EF->Me_r_0 ](pEm, pP_ptr); }
static void Em31_core_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

}
static void Em31_core_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	switch (EF->Me_r_1) {
	case 0:
		((short *)pP_ptr->pSa_dat_head)[6] += 0x400;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 0x1000) {
			((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
			EF->Me_r_1 = 1;
			EF->Me_timer0 = 0;

			Em31_me_yoko_d = 14 - Rnd() % 28;
			Em31_me_tate_d = 14 - Rnd() % 28;
		}

		break;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[6] -= EF->Me_timer0;
		EF->Me_timer0 += 100;
		if (((short *)pP_ptr->pSa_dat_head)[6] < 0) {
			((short *)pP_ptr->pSa_dat_head)[6] = 0;
			EF->Me_r_0 = 0;
			EF->Me_r_1 = 0;

			Em31_me_yoko_d = 0;
			Em31_me_tate_d = 0;
		}
	}

	Sa_parts_allmv(pEm, pP_ptr);
}
static void Em31_core_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	register unsigned long Me_t;

	switch (EF->Me_r_1) {
	case 0:
		((short *)pP_ptr->pSa_dat_head)[6] += 0x400;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 0xC00) {
			((short *)pP_ptr->pSa_dat_head)[6] = 0xC00;
			EF->Me_r_1 = 1;
			EF->Me_timer0 = 0;
			EF->Me_timer1 = 3;
		}
		break;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[6] -= 500;
		EF->Me_timer0 += 100;
		Me_t = EF->Me_timer1;
		EF->Me_timer1 = Me_t - 1;
		if (!Me_t) {
			EF->Me_timer1 = 3;

			EF->Me_r_1 = 2; } break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[6] += 500;
		EF->Me_timer0 -= 100;
		Me_t = EF->Me_timer1;
		EF->Me_timer1 = Me_t - 1;
		if (!Me_t) {
			EF->Me_timer1 = 3;
			EF->Me_r_1 = 1;
		}

	}

	Em31_me_yoko_d = 14 - Rnd() % 28;
	Em31_me_tate_d = 14 - Rnd() % 28;

	Sa_parts_allmv(pEm, pP_ptr);

}
static void Em31_hand(ENEMY_WORK *pEm, MATRIX *pM)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, pM);
	CompM(pM, (MATRIX *)pP_ptr[1].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[2].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[4].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[5].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[6].M, pM);
}
static void Em31_hand_left(ENEMY_WORK *pEm, MATRIX *pM)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, pM);
	CompM(pM, (MATRIX *)pP_ptr[1].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[2].M, pM);

	CompM(pM, (MATRIX *)pP_ptr[9].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[10].M, pM);
	CompM(pM, (MATRIX *)pP_ptr[11].M, pM);
}
