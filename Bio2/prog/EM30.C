/* em30.c - reconstruction of D:\Bio2\prog\EM30.C (enemy 30).
 * Linked into two overlays from the same source: 0x15 (at 0x8010013c) and
 * 0x33 (at 0x8010d13c); only the enemy-number-specific entry names differ
 * (Em130/Em230, Pldamege_Em130/230, Pldie_Em130/230), selected by OVL_33.
 * Types come from the TU's retail SYM preamble (em30.h). */
#include "EM30.H"

/* Em30 work variables kept in the enemy's generic Free[] area */
#define EF	((EM30_FREE *)EM_FREE(pEm))
/* per-part work kept in the PARTS_W generic Free[] area */
#define PF	((EM30_PARTS_FREE *)pP_ptr->Free)

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;
extern long Ground_in_flg;
/* declared: retail SYM has no implicit-declaration block at their calls (Em30_pldm01, Em30_mv08) */
/* PsyQ libgte (declared: retail SYM has no implicit-declaration block at their calls) */

/* ---- functions of other translation units ----
 * EM30.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail 0xFA (Beta 2: 0x12C) */
static unsigned short Em2b_life_easy = 0xC8;	/* retail 0xC8 (Beta 2: 0x12C) */
/* retail-only second life set ([INFERRED] _flg40 names as in em10/em20/em25; image bytes,
 * retail .data +0x30..+0x5F of this overlay; no retail code reads it here) */
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = {
	0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E
};
static unsigned short Em2a_life_easy_flg40[10] = {
	0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C
};
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static void (*Em30_routine_0[8])() = {
	&Em30_Init,
	&Em30_Move,
	&Em30_Damage,
	&Em30_Die,
	&Em30_Scenario,
	0,
	0,
	&Em30_Dead
};
/* retail (image 0x80107524 slot 1 / 0x80114524 slot 0): [1][0] is -8800 (Beta 2 -14400) */
static short Em30_dest[2][2] = {
	{ -8800, -8000 }, { -8800, 1200 }
};
static short Em30_dest303[5][2] = { { -25000, -25400 }, { -12200, -25300 }, { -17800, -19700 }, { -23600, -14500 }, { -12300, -14500 } };
static short Em30_hani[2][4] = { { -12000, -17000, 6200, 19400 }, { -18100, -1400, 5200, 6300 } };
static short Em30_hani303[5][4] = { { -25500, -26500, 4100, 2700 }, { -14100, -26600, 3000, 2500 }, { -19200, -21400, 2700, 3800 }, { -25300, -15600, 3900, 2400 }, { -14500, -15600, 3400, 2700 } };
static char Em30_me_tate = 0;
static char Em30_me_yoko = 0;
static short Em30_me_tate_mv = 0;
static short Em30_me_yoko_mv = 0;
static short Em30_me_tate_a = 0;
static short Em30_me_yoko_a = 0;
static char Em30_me_tate_d = 0;
static char Em30_me_yoko_d = 0;
static void (*Em30_Move_type[6])() = {
	&Em30_Move_type00,
	&Em30_Move_type00,
	&Em30_Move_type00,
	&Em30_Move_type00,
	&Em30_Move_type00,
	&Em30_Move_type05
};
static void (*Em30_mv_br_type00[14])() = {
	&Em30_mv00_br,
	&Em30_mv01_br,
	&Em30_mv02_br,
	&Em30_mv03_br,
	&Em30_mv04_br,
	&Em30_mv05_br,
	&Em30_mv06_br,
	&Em30_mv02_br,
	&Em30_mv07_br,
	&Em30_mv08_br,
	&Em30_mv09_br,
	&Em30_mv0a_br,
	&Em30_mv0b_br,
	&Em30_mv0c_br
};
static void (*Em30_mv_type00[14])() = {
	&Em30_mv00,
	&Em30_mv01,
	&Em30_mv02,
	&Em30_mv03,
	&Em30_mv04,
	&Em30_mv05,
	&Em30_mv06,
	&Em30_mv02,
	&Em30_mv07,
	&Em30_mv08,
	&Em30_mv09,
	&Em30_mv0a,
	&Em30_mv0b,
	&Em30_mv0c
};
static void (*Em30_mv_br_type01[])() = { };
static void (*Em30_mv_type01[])() = { };
static short Em30_dir[2] = {
	0, 0
};
static short Em30_dir303[5] = { 3072, 3072, 0, 1024, 1024 };
static char Em30_iki[64] = { 12, 23, 35, 43, 51, 60, 66, 71, 75, 79, 82, 86, 89, 92, 94, 96, 97, 98, 99, 100, 99, 98, 97, 96, 94, 92, 89, 87, 85, 83, 80, 78, 75, 72, 68, 65, 62, 58, 54, 49, 44, 39, 34, 29, 24, 19, 13, 7, 0, -6, -15, -11, -26, -30, -32, -34, -35, -35, -34, -31, -25, -20, -10, 0 };
static unsigned char Em30_kuchi_mv09[12] = { 0, 5, 1, 4, 2, 0x28, 1, 4, 0, 2, 0xFF, 0 };
static unsigned char Em30_kuchi_mv09_01[12] = { 0, 5, 1, 4, 2, 0x40, 1, 4, 0, 2, 0xFF, 0 };
static void (*Em30_san_tbl[3])() = {
	&Em30_san_mv00,
	&Em30_san_mv01,
	&Em30_san_mv02
};
static void (*Em30_dm[][9])() = { };
static unsigned char Em30_Dm_cnt[18] = {
	1, 4, 4, 4, 0xA, 0x14, 6, 7, 0xA, 0xA, 0x14, 6, 4, 0x14, 1, 0, 0x14, 2
};
static void (*Em30_die[][9])() = { };
static void (*Em30_sce[])() = { };
static void (*Pldamage_Em30_tbl[2])() = {
	&Em30_pldm00,
	&Em30_pldm00
};
static void (*Pldie_Em30_tbl[1])() = {
	&Em30_pldie00
};
static void (*Em30_core_me_sa_tbl[3])() = {
	&Em30_core_mv00,
	&Em30_core_mv01,
	&Em30_core_mv02
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	short Dir;
	SVECTOR Sv;
	VECTOR Dv;
	PARTS_W *pP_ptr;

	Dv = *(VECTOR *)&G.Player.Pos_x;

	if (EF->Etc_flg & 8) {

		if (G.SaveData.Room_no != 3) {

			Dv.vx = Em30_dest[EF->Nige & 0xF][0];
			Dv.vz = Em30_dest[EF->Nige & 0xF][1];

		} else {

			Dv.vx = Em30_dest303[EF->Nige & 0xF][0];
			Dv.vz = Em30_dest303[EF->Nige & 0xF][1];
		}
	}

	Root_ck(pEm, &Dv, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (EF->Etc_flg & 2) {
			if (EF->Dm_timer0-- <= 0) {
				if (EF->Dm_cnt)
					EF->Dm_cnt--;
				EF->Dm_timer0 = 15;
				if (EF->Dm_cnt == 0)
					EF->Etc_flg &= 0xFFFD;
			}
		}

		if (EF->Etc_flg & 4)
			EF->Etc_flg |= 1;

		if (EF->At_timer)
			EF->At_timer--;

		if (EF->Fire_c)
			EF->Fire_c--;

		EF->Iki_timer++;
		EF->Iki_timer &= 0x3F;

		Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Em30_mgdm(pEm);

		pEm->pEnemy_ptr = (ENEMY_WORK *)&G.Player;
		Rot_neck_em_n(pEm, pEm->Cdir_y, &Dir);
		Dir &= 0xFFF;
		if (Dir > 0x800)
			Dir -= 0xFFF;
		pP_ptr = pEm->pSin_parts_ptr + 3;
		pP_ptr->Cdir_y += Dir;
		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
		pP_ptr->Cdir_y -= Dir;

		Em30_core_me_mv(pEm, pEm->pSin_parts_ptr + 4);
		Em30_core_me_sa(pEm, pEm->pSin_parts_ptr + 4);

		if (!(EF->Etc_flg & 0x40))
			Em_seq_sound(pEm);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);
	}

}

static void Em30_after(ENEMY_WORK *pEm)
{
}

static void Em30_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	PARTS_W *pO_ptr;
	SVECTOR Sv;
	unsigned long i; unsigned long *pTmp;
	unsigned char Me4[15] = { 0x0E, 0x13, 0x16, 0x18, 0x19, 0x1A, 0x1B, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2A, 0x32 };

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	/* retail: the "System_flg & 0x40" second life set (as Em10/Em28_Init), here with identical arms.
	 * Lever: the two-armed if/else ends the CSE path, so the G.pPldamage_Em/G.Player constants below
	 * are not rewritten relative to the &G.System_flg pseudo in a saved register; after reload the
	 * identical arms cross-jump and jump.c deletes the outer branch with its dead "& 0x40" test. */
	if (G.System_flg & 0x40) {
		pEm->Life = 500;
		if (G.System_flg & 0x20)
			pEm->Life = 400;
	} else {
		pEm->Life = 500;
		if (G.System_flg & 0x20)
			pEm->Life = 400;
	}

	pEm->D_life_u = 0x82;
	pEm->D_life_c = 0x82;
	pEm->D_life_d = 0x82;

	pP_ptr = pEm->pSin_parts_ptr + 3;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 3;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 0x20;
	pP_ptr->Free[3] = 0x20;
	pP_ptr->Free[4] = 0x264;
	pP_ptr->Free[5] = 0x138;

	EF->Dm_timer0 = 0;
	EF->Dm_cnt = 0;
	EF->At_timer = 0;
	EF->Etc_flg = 0;
	EF->Iki_timer = 0;
	EF->Me_r_0 = 0;
	EF->Me_r_1 = 0;
	EF->Mg_rout0 = 0;
	EF->Fire_c = 0;

	Em30_me_tate = 0;
	Em30_me_yoko = 0;
	Em30_me_tate_mv = 0;
	Em30_me_yoko_mv = 0;
	Em30_me_tate_a = 0;
	Em30_me_yoko_a = 0;
	Em30_me_tate_d = 0;
	Em30_me_yoko_d = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 800;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 800;
	pEm->Atd[0].W = 800;
	pEm->Atd[0].D = 800;

	pEm->Eff_at_r = 500;

	pEm->Be_flg |= 0xC000000;

	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x100);

	Kage_work_set(&pEm->pKage_work, 0, 0x2BC0320, 0, &pEm->pSin_parts_ptr->Workm.t[0]);

	G.pPldamage_Em[pEm->Id - 0x10] = PLDAMEGE_EM;
	G.pPldie_Em[pEm->Id - 0x10] = PLDIE_EM;

	pEm->Be_flg |= 4;

	pP_ptr = pEm->pSin_parts_ptr + 16;
	pP_ptr->Be_flg |= 0x4000;

	pP_ptr = pEm->pSin_parts_ptr + 17;
	pO_ptr = pEm->pSin_parts_ptr + 6;
	pP_ptr->Be_flg = 0;
	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = pO_ptr;
	pP_ptr->Pos_x = 12;
	pP_ptr->Pos_y = 0x86;
	pP_ptr->Pos_z = 0x1E;
	pP_ptr->Cdir_x = -200;
	pP_ptr->Cdir_y = 0;
	pP_ptr->Cdir_z = 0;
	RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);

	pP_ptr = pEm->pSin_parts_ptr + 18;
	pP_ptr->Be_flg = 0;

	if ((pEm->Type & 0xFFF) == 1) {
		pEm->Routine_1 = 1;
		return;
	}

	if ((pEm->Type & 0xFFF) == 3) {

		pEm->Routine_1 = 12; pEm->Type &= 0xF000;
		return;

	}

	pEm->Routine_1 = 10;
	pEm->Type |= 0x4000;
	pEm->Parts0_pos_y = 0xEB8;

	pP_ptr = pEm->pSin_parts_ptr + 1;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 2;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 4;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	((short *)pP_ptr->pSa_dat_head)[8] = 0;
	Sa_parts_allmv(pEm, pP_ptr);
	Sa_parts_offset_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 5;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	Sa_parts_allmv(pEm, pP_ptr);

	i = 16;
	pP_ptr = pEm->pSin_parts_ptr;
	do {
		pP_ptr->Be_flg |= 0x400;
		pP_ptr->Size_x = 0xC28;
		pP_ptr->Size_y = 0xEB8;
		pP_ptr->Size_z = 0xC28;
		pP_ptr++;
	} while (--i != 0);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	pP_ptr->Be_flg &= ~0x400;
	pP_ptr = pEm->pSin_parts_ptr + 2;
	pP_ptr->Be_flg &= ~0x400;
	pP_ptr = pEm->pSin_parts_ptr + 4;
	pP_ptr->Be_flg &= ~0x400;
	pP_ptr = pEm->pSin_parts_ptr + 5;
	pP_ptr->Be_flg &= ~0x400;
	pP_ptr = pEm->pSin_parts_ptr + 6;
	pP_ptr->Be_flg &= ~0x400;

	pP_ptr = pEm->pSin_parts_ptr + 16;
	pP_ptr->Be_flg |= 0x4000;

	pO_ptr = pEm->pSin_parts_ptr + 6;
	pTmp = pO_ptr->pTmd;
	pO_ptr->pTmd = pP_ptr->pTmd;
	pP_ptr->pTmd = pTmp;
	pTmp = pO_ptr->pTmd2;
	pO_ptr->pTmd2 = pP_ptr->pTmd2;
	pP_ptr->pTmd2 = pTmp;
	pTmp = pO_ptr->pPacket;
	pO_ptr->pPacket = pP_ptr->pPacket;
	pP_ptr->pPacket = pTmp;
	pTmp = pO_ptr->pPacket2;
	pO_ptr->pPacket2 = pP_ptr->pPacket2;
	pP_ptr->pPacket2 = pTmp;

	if (G.pEnemy[2]->Be_flg) {
		pP_ptr = pEm->pSin_parts_ptr + 18;
		pO_ptr = G.pEnemy[2]->pSin_parts_ptr + 8;
		pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
		pP_ptr->pOya_parts = pO_ptr;

		pP_ptr->Pos_x = 0xEB;
		pP_ptr->Pos_y = -0xB7;
		pP_ptr->Pos_z = 0;

		if (G.pEnemy[2]->Id == 0x44) {
			pP_ptr->Pos_x = 0xF6;
			pP_ptr->Pos_y = -0xFB;
			pP_ptr->Pos_z = 0;
		}
	}

	if ((pEm->Type & 0xFFF) == 2) {
		pP_ptr = pEm->pSin_parts_ptr + 17;
		pP_ptr->Be_flg = 1;
		pEm->Routine_1 = 11;
		pP_ptr->Be_flg |= 0x400;
		pP_ptr->Size_x = 0x1000;
		pP_ptr->Size_y = 0x1000;
		pP_ptr->Size_z = 0;

		pP_ptr = pEm->pSin_parts_ptr + 18;

		pP_ptr->Be_flg = 0x4401;
		pP_ptr->Size_x = 0x1000;
		pP_ptr->Size_y = 0x1000;
		pP_ptr->Size_z = 0x1000;

		pEm->Timer3 = 0;
		((short *)pP_ptr->pSa_dat_head)[6] = 0;
		((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;

		pEm->Type |= 0x4000;
		*(long *)&pEm->Move_no = 0x14;
		Joint_move(pEm, Kan, Seq, 0x100);
	}

	if ((pEm->Type & 0xFFF) == 5) {
		pP_ptr = pEm->pSin_parts_ptr + 18;
		pP_ptr->pSuper = (unsigned long *)&pEm->pSin_parts_ptr->Workm;
		pP_ptr->pOya_parts = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg = 1;

		((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
		((short *)pP_ptr->pSa_dat_head)[8] = 0;
		Sa_parts_allmv(pEm, pP_ptr);

		pP_ptr->Pos_x = 0;
		pP_ptr->Pos_y = 0;
		pP_ptr->Pos_z = 0;

		pEm->Routine_1 = 0;
		pEm->Be_flg |= 0xA;
		((unsigned char *)pEm->pKage_work)[14] = 0;

		i = 16;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		} while (--i != 0);
	}

	if ((pEm->Type & 0xFFF) == 4) {
		pEm->Routine_1 = 13;
		pEm->Type &= 0xF000;
	}

}

static void Em30_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em30_Move_type[pEm->Type & 0x3F](pEm, Kan, Seq); }

static void Em30_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em30_mv_br_type00[pEm->Routine_1](pEm);

	Em30_mv_type00[pEm->Routine_1](pEm, Kan, Seq); }

static void Em30_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em30_mv_br_type01[pEm->Routine_1](pEm);

	Em30_mv_type01[pEm->Routine_1](pEm, Kan, Seq); }

static void Em30_Move_type05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em30_syoku(pEm, pEm->pSin_parts_ptr + 18); }

static void Em30_mv00_br(ENEMY_WORK *pEm)
{

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x320);

}

static void Em30_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	unsigned long i;

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 1;
		pEm->Neck_flg &= 0xFD;

		pEm->Timer3 = 1;

		pEm->Timer2 = 1;

	case 1:	Joint_move(pEm, Kan, Seq, 0x80);

		i = 16;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Size_x += (signed char)pEm->Timer3 * 35;
			pP_ptr->Size_y += (signed char)pEm->Timer3 * 35;
			pP_ptr->Size_z += (signed char)pEm->Timer3 * 35;
			pP_ptr++;
		} while (--i != 0);

		pEm->Parts0_pos_y += (signed char)pEm->Timer3 * 16;

		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] -= (signed char)pEm->Timer3 * 200;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[6] -= (signed char)pEm->Timer3 * 200;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr + 5;
		((short *)pP_ptr->pSa_dat_head)[6] -= (signed char)pEm->Timer3 * 200;
		Sa_parts_allmv(pEm, pP_ptr);

		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[6] -= (signed char)pEm->Timer3 * 200;
		((short *)pP_ptr->pSa_dat_head)[8] += (signed char)pEm->Timer3 * 200;
		if (((short *)pP_ptr->pSa_dat_head)[6] < 0)
			pEm->Timer3 *= -1;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 0x1000)
			pEm->Timer3 *= -1;
		Sa_parts_allmv(pEm, pP_ptr);
		Sa_parts_offset_allmv(pEm, pP_ptr);

		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] -= (signed char)pEm->Timer3 * 200;
		Sa_parts_allmv(pEm, pP_ptr);

	}
}

static void Em30_mv01_br(ENEMY_WORK *pEm)
{
	short Dir1;
	short Dir2;
	unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;

	LL = pEm->L_pl;

	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x5F4);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x270);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338); /* retail 0x338 (Beta 0x400) */

	if (!Dir2 && LL < 1500)
	{	*(long *)&pEm->Routine_0 = 0x501;

		if (EF->At_timer < 10 && (Rnd() & 1))
			*(long *)&pEm->Routine_0 = 0x301;

	}

	if (EF->At_timer == 0 && G.Player.Damage_cnt == 0) {

		if (!Dir2 && LL >= 1801 && LL < 3000)
		{	*(long *)&pEm->Routine_0 = 0x201;

			if ((EF->Etc_flg & 1) && (Rnd() & 3))
				*(long *)&pEm->Routine_0 = 0x701;

			if (EF->Etc_flg & 0x20)
				*(long *)&pEm->Routine_0 = 0x701;

		}

		if (!Dir1 && LL >= 1801 && LL < 2500 && !(Rnd() & 3)) /* retail adds the Rnd test @0x80101054 */
			*(long *)&pEm->Routine_0 = 0x701;

		if (!Dir2 && LL < 1800)
			*(long *)&pEm->Routine_0 = 0x301;

	}

	if (G.Player.Damage_cnt && LL < 3000)
		*(long *)&pEm->Routine_0 = 0x401;

}

static void Em30_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0001;

		if (EF->Etc_flg & 1)
			*(long *)&pEm->Move_no = 0x1F000C;

		pEm->Routine_2 = 1;

	case 1:	Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
		if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x338))
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 32);

		Spd_get(pEm, Kan, Seq, 0);
		if (pEm->Hokan_flg >= 0x10)
			pEm->Spd_x >>= 1;

		if (!Hit_ck_point4(&pEm->Pos_x, G.Cccut))
			pEm->Spd_x += 40;

		if (Joint_move(pEm, Kan, Seq, 0x80))
		{	if (!(Rnd() & 3))
			{	Snd_se_enem(7, pEm);
			}
		}
		Add_speedXZ(pEm, 0);
	}
}

static void Em30_mv02_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	SVECTOR Sv1;
	MATRIX M;
	unsigned char Ck;
	short Dir;
	PARTS_W *pP_ptr;
	unsigned char Em30_mv02_end[2] = { 30, 20 };

	unsigned short Em30_mv02_ikari[4][3] = { { 15, 0, 256 }, { 3, 33, 1024 }, { 15, 0, 256 }, { 3, 17, 1024 } };

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = (Em30_mv02_ikari[EF->Etc_flg & 1][0] << 16) + (Em30_mv02_ikari[EF->Etc_flg & 1][1] << 8) + 4;

		if (EF->Etc_flg & 1)
			Snd_se_enem(3, pEm);

		pEm->Routine_2 = 1;
		pEm->Timer2 = 0;

		if (pEm->Routine_1 == 7) {
			*(long *)&pEm->Move_no = (Em30_mv02_ikari[(EF->Etc_flg & 1) + 2][0] << 16) + (Em30_mv02_ikari[(EF->Etc_flg & 1) + 2][1] << 8) + 11;

			pEm->Timer2 = 1;
			EF->Etc_flg &= 0xFFDF;
		}
		Spd_get(pEm, Kan, Seq, 0);

		pEm->Timer3 = 0;

		pEm->Timer1 = 0;

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		if (pEm->Hokan_flg) {
			pEm->Spd_x >>= 1;
			pEm->Spd_z >>= 1;
		}
		if (Joint_move(pEm, Kan, Seq, Em30_mv02_ikari[EF->Etc_flg & 1][2])) {

			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = pEm->Timer1 + 30;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt >= 6 && pEm->Move_cnt < Em30_mv02_end[(signed char)pEm->Timer2] && !*(signed char *)&pEm->Timer3)

		{	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20); /* retail 0x20/0x52C (Beta 8/0x4C8) */
			if (pEm->L_pl > 3800 || Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x52C)) {

				*(long *)&pEm->Routine_0 = 0x101;
				EF->At_timer = 10;
				if (!(Rnd() & 3)) /* retail-only @0x801014FC */
					*(long *)&pEm->Routine_0 = 0x801;
			}
		}

		if (!*(signed char *)&pEm->Timer2) {

			if (pEm->Move_cnt == 46 && (!*(signed char *)&pEm->Timer3 || !(Rnd() & 3))) {
				*(long *)&pEm->Move_no = (Em30_mv02_ikari[EF->Etc_flg & 1][0] << 16) + 8; pEm->Timer3 = 1; }

		} else

		if (pEm->Move_cnt == 30 && !*(signed char *)&pEm->Timer3) {
			*(long *)&pEm->Move_no = (Em30_mv02_ikari[EF->Etc_flg & 1][0] << 16) + 8;

			pEm->Timer3 = 1;
		}

		if (*pEm->pNow_seq & 0x30000)

		{	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 32);

			if (*(signed char *)&pEm->Timer3 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400))

				pEm->Cdir_y += 0x80;

			Sv0.vx = 500;
			Sv0.vy = 0x183;
			Sv0.vz = 0;
			Sv1.vx = 0x44C;
			Sv1.vy = 0x183;
			Sv1.vz = 0;
			Em30_hand(pEm, &M);
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 900, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 900, &Sv1)) && !G.Player.Damage_cnt)

			{	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - 0x400;

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (*(signed char *)&pEm->Timer3) {
					Ck ^= 1;
					Dir -= 0x800;
				}

				Pl_life_down(30, 1);
				if (!*(signed char *)&pEm->Timer2) {

					*(long *)&G.Player.Routine_0 = ((Ck + 4) << 8) + 2;

					if (!Ck)
						G.Player.Cdir_y = Dir + 0x800;

				} else {

					*(long *)&G.Player.Routine_0 = ((Ck + 4) << 8) + 2;
					if (!Ck && !*(signed char *)&pEm->Timer3) {
						*(long *)&G.Player.Routine_0 = 0x10502;
						G.Player.Move_no = 5;
						G.Player.Move_cnt = 5;
						G.Player.Hokan_flg = 3;
						G.Player.Spd_x = 150;
						G.Player.Spd_y = 0;
						G.Player.Spd_z = 0;
					} else
					if (*(signed char *)&pEm->Timer3 && !Ck)
						G.Player.Cdir_y = Dir + 0x800;
				}

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				pEm->Timer3 = 1;

				EF->Etc_flg &= 0xFFFE;

				Snd_se_enem(6, pEm);

				pEm->Timer1 = 15;
				Pad_vib_set0(2, 0); /* retail-only vibration @0x80101840 */
				Pad_vib_set1(6, 0xFA, 0);

			}

			Sv0.vx = 800;
			Sv0.vy = 0x183;
			Sv0.vz = 0;
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			M.t[0] += Sv0.vx;
			M.t[1] += Sv0.vy + 150;
			M.t[2] += Sv0.vz;
			Sca_get_ground(&M.t[0], 400, 0x8000, 1);

			if (Ground_in_flg)
			{	*(long *)&pEm->Routine_0 = 0x601;
				EF->At_timer = 30;

				pP_ptr = pEm->pSin_parts_ptr + 6;
				Sv0.vx = 800;
				Sv0.vy = 0x183;
				Sv0.vz = 0;
				pP_ptr->Workm = M;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vx += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vy += 0x80 - (Rnd() & 0xFF);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vz += 0x40 - (Rnd() & 0x7F);
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
			}

		}
	}
}

static void Em30_mv03_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	SVECTOR Sv1;
	PARTS_W *pP_ptr;
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0xF0002;
		if (EF->Etc_flg & 1)
			*(long *)&pEm->Move_no = 0xF0A02;

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 = 1;

	case 1:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 15;
		}
		Add_speedXZ(pEm, 0);

		if (*pEm->pNow_seq & 0x30000)
		{	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 16);
			Sv0.vx = 500;
			Sv0.vy = 0x183;
			Sv0.vz = 0;
			Sv1.vx = 1000;
			Sv1.vy = 0x183;
			Sv1.vz = 0;
			pP_ptr = pEm->pSin_parts_ptr + 6;
			if ((L_pos_ckXZM(&pP_ptr->Workm, &G.Player.Pos_x, 800, &Sv0) || L_pos_ckXZM(&pP_ptr->Workm, &G.Player.Pos_x, 800, &Sv1)) && !G.Player.Damage_cnt)

			{	Pl_life_down(20, 1); /* retail 20 (Beta 15) */
				*(long *)&G.Player.Routine_0 = 0x10502;
				G.Player.Move_no = 5;
				G.Player.Move_cnt = 5;
				G.Player.Hokan_flg = 3;
				G.Player.Spd_x = 150;
				G.Player.Spd_y = 0;
				G.Player.Spd_z = 0;
				G.Player.Neck_flg |= 0x12;

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= 0xFFFE;

				Snd_se_enem(6, pEm);
				Pad_vib_set0(2, 0); /* retail-only vibration @0x80101C54 (implicit declarations, as all EM30 externs) */
				Pad_vib_set1(6, 0xFA, 0);

			}

		}
	}
}

static void Em30_mv04_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv0;
	SVECTOR Sv1;
	MATRIX M;
	unsigned long LL;
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0001;
		if (EF->Etc_flg & 1)
			*(long *)&pEm->Move_no = 0x1F000C;

		pEm->Routine_2 = 1;
		pEm->Timer0 = 60;

		pEm->Timer1 = 90;

		pEm->Timer3 = 0;

	case 1:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 32);
		Spd_get(pEm, Kan, Seq, 1);
		if (pEm->Hokan_flg >= 0x15)
			pEm->Spd_x >>= 1;

		Joint_move(pEm, Kan, Seq, 0x10080);

		LL = pEm->L_pl;
		if (LL > 2400)
			pEm->Spd_x >>= 1;

		if (pEm->Timer0)
			pEm->Timer0--;

		if (!pEm->Timer0 && LL > 2000 && LL < 3300 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100))

			pEm->Routine_2 = 2;

		if (LL > 2000 && LL < 3300 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100))

			pEm->Timer3 = 1;

		if ((unsigned short)pEm->Timer1-- == 0) {
			if (!*(signed char *)&pEm->Timer3) {
				*(long *)&pEm->Routine_0 = 0x101;
				if (!(Rnd() & 1)) /* retail-only @0x80101E68 */
					*(long *)&pEm->Routine_0 = 0x801;
			} else
				pEm->Routine_2 = 2;

			EF->At_timer = 10;
		}

		Add_speedXZ(pEm, 0); break;

	case 2:	*(long *)&pEm->Move_no = 0xF0006;

		if (EF->Etc_flg & 1)
			pEm->Move_cnt = 10;

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 = 3;

		pEm->Timer3 = 0;

	case 3:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_2 = 4;
			if (!*(signed char *)&pEm->Timer3) {
				*(long *)&pEm->Routine_0 = 0x101;
				EF->At_timer = 10; /* retail 10 (Beta 30) */
				if (!(Rnd() & 1)) /* retail-only @0x80101F24 */
					*(long *)&pEm->Routine_0 = 0x801;
			}
		}
		Add_speedXZ(pEm, 0);

		if (*pEm->pNow_seq & 0x30000)

		{	Em30_hand(pEm, &M);

			if (*pEm->pNow_seq & 0x10000)
			{	Sca_get_ground(&M.t[0], 200, 0x8000, 1);
				if (Ground_in_flg)
				{	*(long *)&pEm->Routine_0 = 0x601;
					EF->At_timer = 30;

					pP_ptr = pEm->pSin_parts_ptr + 6;
					Sv0.vx = 800;
					Sv0.vy = 0x183;
					Sv0.vz = 0;
					pP_ptr->Workm = M;
					Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
					Sv0.vx += 0x40 - (Rnd() & 0x7F);
					Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
					Sv0.vy += 0x80 - (Rnd() & 0xFF);
					Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
					Sv0.vz += 0x40 - (Rnd() & 0x7F);
					Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
					break;
				}
			}

			Sv0.vx = 300;
			Sv0.vy = 0x183;
			Sv0.vz = 0;
			Sv1.vx = 0x44C;
			Sv1.vy = 0x183;
			Sv1.vz = 0;
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 600, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 600, &Sv1)) && G.Player.Life >= 0)

			{	Pl_life_down(500, 0);
				G.System_flg |= 0x100;
				*(long *)&G.Player.Routine_0 = 6;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				EF->Etc_flg &= 0xFFFE;

				pEm->Timer3 = 1;

				Snd_se_enem(1, pEm);

			}

		}

		break;

	case 4:	*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Routine_2 = 5;
		pEm->Timer0 = 60;

	case 5:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 32);
		Spd_get(pEm, Kan, Seq, 1);
		if (pEm->Hokan_flg >= 0x15)
			pEm->Spd_x >>= 1;

		Joint_move(pEm, Kan, Seq, 0x10080);

		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 6;

		Add_speedXZ(pEm, 0); break;

	case 6:	*(long *)&pEm->Move_no = 0xF0006;
		pEm->Routine_2 = 7;
		pEm->Timer3 = 0;

	case 7:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100))
			pEm->Routine_2 = 8;

		if ((*pEm->pNow_seq & 0x20000) && !*(signed char *)&pEm->Timer3)
		{	Snd_se_enem(1, pEm);
			pEm->Timer3 = 1;

			pP_ptr = G.Player.pSin_parts_ptr;
			Esp_call(0x21000, pEm->Cdir_y, &pP_ptr->Workm, 0);
			pP_ptr = G.Player.pSin_parts_ptr + 8;
			Esp_call(0x21000, pEm->Cdir_y, &pP_ptr->Workm, 0);
		}

		Add_speedXZ(pEm, 0); break;

	case 8:	pEm->Routine_2 = 9;
		*(long *)&pEm->Move_no = 0x1F0013;

	case 9:	Spd_get(pEm, Kan, Seq, 0);
		pEm->Hokan_flg = 0x1C;
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);
		break;

	case 10:	*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Routine_2 = 11;

	case 11:	Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -32);
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

	}
}

static void Em30_mv05_br(ENEMY_WORK *pEm)
{
	unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;
	short Dir2;	/* [inferred] no SYM record (copy-propagated into the test); name from Em30_mv01_br */

	LL = pEm->L_pl;

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x5F4);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!Dir2 && LL > 2500)
		*(long *)&pEm->Routine_0 = 0x101;

}

static void Em30_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0001;
		if (EF->Etc_flg & 1)
			*(long *)&pEm->Move_no = 0x1F000C;

		pEm->Routine_2 = 1;

		pEm->Timer0 = 60;

	case 1:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 32);
		Spd_get(pEm, Kan, Seq, 1);
		if (pEm->Hokan_flg >= 0x15)
			pEm->Spd_x >>= 1;

		Joint_move(pEm, Kan, Seq, 0x10080);
		Add_speedXZ(pEm, 0);

		if ((unsigned short)pEm->Timer0-- == 0)
			*(long *)&pEm->Routine_0 = 0x101;

	}
}

static void Em30_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x20007;
		pEm->Routine_2 = 1;

		EF->Etc_flg |= 0x20;

	case 1:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 16);
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400))
		{	*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 40;
			if (!(Rnd() & 1)) /* retail-only @0x801026B0 */
				*(long *)&pEm->Routine_0 = 0x801;

			if (!(Rnd() & 1) && !G.Player.Damage_cnt && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) /* retail &1, 0x100 (Beta &3, 0x270) */

				*(long *)&pEm->Routine_0 = 0x20401;

		}
		Add_speedXZ(pEm, 0);

	}
}

static void Em30_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x70011;
		pEm->Routine_2 = 1;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 64);
		EF->Etc_flg |= 1; /* retail-only @0x801027B0 */

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->At_timer = 10;
			EF->Etc_flg |= 1;
		}
		Add_speedXZ(pEm, 0);
	}

}

static void Em30_mv08_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	long x, z, i;
	unsigned long L, LL;

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F000E;
		pEm->Routine_2 = 1;

		if (G.SaveData.Room_no != 3) {

			i = 2;
			L = 0x7FFFFFFF;
			while (i) {
				i = ~(-i); /* [INFERRED] countdown identity for the bounded nonnegative count. */ x = pEm->Pos_x - Em30_dest[i][0];
				z = pEm->Pos_z - Em30_dest[i][1];
				LL = SquareRoot0(x * x + z * z);
				if (LL < L) {
					L = LL;
					EF->Nige = i;
				}
			}

		} else {

			i = 5;
			L = 0x7FFFFFFF;
			while (i) {
				i = ~(-i); /* [INFERRED] countdown identity for the bounded nonnegative count. */ x = pEm->Pos_x - Em30_dest303[i][0];
				z = pEm->Pos_z - Em30_dest303[i][1];
				LL = SquareRoot0(x * x + z * z);
				if (LL < L) {
					L = LL;
					EF->Nige = i;
				}
			}
		}

		EF->Etc_flg |= 8;

	case 1:	if (G.SaveData.Room_no != 3) {

			i = 2;
			do
			{	if (Hani_ck(&pEm->Pos_x, Em30_hani[i - 1])) {
					pEm->Routine_2 = 2;

					pEm->Timer1 = Em30_dir[i - 1];
				}
			} while (--i != 0);

		} else {

			i = 5;
			do
			{	if (Hani_ck(&pEm->Pos_x, Em30_hani303[i - 1])) {
					pEm->Routine_2 = 2;

					pEm->Timer1 = Em30_dir303[i - 1];
				}
			} while (--i != 0);
		}

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);
		pEm->Life = 0; break;

	case 2:	Goto01(pEm, pEm->Timer1, 32);

		if (G.SaveData.Room_no == 3) { /* retail: room split @0x80102AA8 */
			if (!Goto01_ck(pEm, pEm->Timer1, 16) && (pEm->Sca_info & 1)) {
				pEm->Routine_2 = 3;
				pEm->Be_flg |= 8;
			}
		} else {
			if (!Goto01_ck(pEm, pEm->Timer1, 16) && (pEm->Sca_info & 1) && pEm->Pos_x >= -8499) {
				pEm->Routine_2 = 3;
				pEm->Be_flg |= 8;
			}
		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		pEm->Life = 0; break;

	case 3:	*(long *)&pEm->Move_no = 0x1F0010;
		pEm->Routine_2 = 4;

		pEm->Life = -1;

	case 4:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80))
			pEm->Routine_2 = 5;

		Add_speedXZ(pEm, 0);

	case 5:
		break;

	}

}

static void Em30_mv09_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr; unsigned long i; unsigned long *pTmp; SVECTOR Sv;

	if (pEm->Routine_2 < 4) {
		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
		Sa_parts_allmv(pEm, pP_ptr);
	}

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F000E;

		pEm->Timer0 = 120;

		pEm->Neck_flg |= 2;

		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;

		break;

	case 1:	Spd_get(pEm, Kan, Seq, 0);

		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0x1F0012;

			pEm->Timer3 = 0;
			pEm->Timer0 = 80;

			pEm->Timer1 = 5;
			pEm->Timer2 = 1;
		}

		break;

	case 2:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			pEm->Routine_2 = 3;
			*(long *)&pEm->Move_no = 0x1F000F;

			pEm->Timer0 = 90;

			pEm->Timer1 = 5;
			pEm->Timer2 = 1;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt == 0x76) {
			pP_ptr = pEm->pSin_parts_ptr + 6;	/* swap parts 6 and 16 (pP_ptr[10]); base spelling inferred from codegen */

			pTmp = pP_ptr->pTmd;
			pP_ptr->pTmd = pP_ptr[10].pTmd;
			pP_ptr[10].pTmd = pTmp;
			pTmp = pP_ptr->pTmd2;
			pP_ptr->pTmd2 = pP_ptr[10].pTmd2;
			pP_ptr[10].pTmd2 = pTmp;
			pTmp = pP_ptr->pPacket;
			pP_ptr->pPacket = pP_ptr[10].pPacket;
			pP_ptr[10].pPacket = pTmp;
			pTmp = pP_ptr->pPacket2;
			pP_ptr->pPacket2 = pP_ptr[10].pPacket2;
			pP_ptr[10].pPacket2 = pTmp;
		}

		break;

	case 3:	if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 4;

			EF->K_timer = 0;
			EF->K_seq = 0;
			EF->K_zan = 0;
		}

		if (pEm->Timer0 < 40) {
			pP_ptr = pEm->pSin_parts_ptr + 2;
			((short *)pP_ptr->pSa_dat_head)[6] += pEm->Timer1 * (signed char)pEm->Timer2;

			pEm->Timer1 += 5;
			pEm->Timer2 *= -1;
			Sa_parts_allmv(pEm, pP_ptr);
		}
		break;

	case 4:	pEm->Parts0_pos_y = 0x1000;
		i = 16;

		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Size_x += 30;
			pP_ptr->Size_y += 10;
			pP_ptr->Size_z += 30;
			if (pP_ptr->Size_z > 0x1000)
				pP_ptr->Be_flg &= ~0x400;

			pP_ptr++;
		} while (--i != 0);

		if (pEm->Move_cnt == 15) {
			pP_ptr = pEm->pSin_parts_ptr + 2;
			Sv.vx = 100;
			Sv.vy = 0;
			Sv.vz = -200;
			Esp_call(0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			Sv.vy = -200;
			Esp_call(0x11000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			Sv.vy = -400;
			Esp_call(0xC18, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			Sv.vy = -600;
			Esp_call(0x830, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			Sv.vy = 200;
			Esp_call(0x10830, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}

		if (pEm->Move_cnt >= 21) {
			pP_ptr = pEm->pSin_parts_ptr + 1;
			((short *)pP_ptr->pSa_dat_head)[6] -= 200;
			if (((short *)pP_ptr->pSa_dat_head)[6] < 0)
				((short *)pP_ptr->pSa_dat_head)[6] = 0;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr + 2;
			((short *)pP_ptr->pSa_dat_head)[6] -= 200;
			if (((short *)pP_ptr->pSa_dat_head)[6] < 0)
				((short *)pP_ptr->pSa_dat_head)[6] = 0;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr + 4;
			((short *)pP_ptr->pSa_dat_head)[6] -= 200;
			((short *)pP_ptr->pSa_dat_head)[8] += 200;
			if (((short *)pP_ptr->pSa_dat_head)[6] < 0) {
				((short *)pP_ptr->pSa_dat_head)[6] = 0;
				((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
			}
			Sa_parts_allmv(pEm, pP_ptr);
		}

		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[6] += pEm->Timer1 * (signed char)pEm->Timer2;

		pEm->Timer2 *= -1;
		Sa_parts_allmv(pEm, pP_ptr);

		pP_ptr = pEm->pSin_parts_ptr + 5;
		((short *)pP_ptr->pSa_dat_head)[6] -= 100;
		if (((short *)pP_ptr->pSa_dat_head)[6] < 0)
			((short *)pP_ptr->pSa_dat_head)[6] = 0;
		Sa_parts_allmv(pEm, pP_ptr);
		Sa_parts_offset_allmv(pEm, pP_ptr);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			pEm->Routine_2 = 5;
			*(long *)&pEm->Move_no = 0x1F0013;
			pEm->Timer0 = 90;
			pEm->Timer3 = 10;
			EF->Iki_timer = 0;

			pEm->Neck_flg &= ~2;
		}

		Add_speedXZ(pEm, 0);

		Em30_kuchi_mv(pEm, pEm->pSin_parts_ptr + 3, Em30_kuchi_mv09_01);

		break;

	case 5:	if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 6;

			Em30_me_tate_d = 0;
			Em30_me_yoko_d = 0;

			EF->K_timer = 0;
			EF->K_seq = 0;
			EF->K_zan = 0;

			EF->Etc_flg |= 0x40;
		}

		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] = -(signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[6] = -(signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
		Sa_parts_allmv(pEm, pP_ptr);

		if (pEm->Timer1 > 0) {
			pP_ptr = pEm->pSin_parts_ptr + 2;
			((short *)pP_ptr->pSa_dat_head)[6] += pEm->Timer1 * (signed char)pEm->Timer2;

			pEm->Timer1 -= 10;
			pEm->Timer2 *= -1;
			Sa_parts_allmv(pEm, pP_ptr);
		}

		if (pEm->Timer0 < 70) {
			pP_ptr = pEm->pSin_parts_ptr + 4;
			((short *)pP_ptr->pSa_dat_head)[8] -= (signed char)pEm->Timer3 * 10;

			pEm->Timer3++;
			if (((short *)pP_ptr->pSa_dat_head)[8] < 0)
				((short *)pP_ptr->pSa_dat_head)[8] = 0;
			Sa_parts_allmv(pEm, pP_ptr);
		}

		if (pEm->Timer0 == 40) {
			Em30_me_tate_d = -10;
			Em30_me_yoko_d = 14;
		}

		if (pEm->Timer0 == 10) {
			Em30_me_tate_d = -10;
			Em30_me_yoko_d = -14;
		}

		break;

	case 6:	Spd_get(pEm, Kan, Seq, 0);
		pEm->Hokan_flg = 0x1C;
		Joint_move(pEm, Kan, Seq, 0x80);

		if (pEm->Move_cnt == 50) {
			pEm->Routine_2 = 7;
			*(long *)&pEm->Move_no = 0x1F0013;

			EF->K_timer = 0;
			EF->K_seq = 0;
		}

		Add_speedXZ(pEm, 0); pP_ptr = pEm->pSin_parts_ptr + 5; Sa_parts_offset_allmv(pEm, pP_ptr); Em30_kuchi_mv(pEm, pEm->pSin_parts_ptr + 3, Em30_kuchi_mv09); break;

	case 7:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Type &= ~0x4000;

			EF->Etc_flg &= ~0x40;
		}

		Add_speedXZ(pEm, 0);

		pP_ptr = pEm->pSin_parts_ptr + 5;
		Sa_parts_offset_allmv(pEm, pP_ptr);

		Em30_kuchi_mv(pEm, pEm->pSin_parts_ptr + 3, Em30_kuchi_mv09);

	}

}

static void Em30_mv0a_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr; unsigned long i;
	long lim; /* [INFERRED] carrier: retail ground limit in case 7 */

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x14;
		pEm->Timer2 = 60;
		pEm->Routine_2 = 1;

		pP_ptr = pEm->pSin_parts_ptr + 18;
		EF->Dm_timer0 = 0;
		EF->Dm_cnt = 0;
		pP_ptr->Timer0 = 0;
		pP_ptr->Timer1 = 0;

		i = 16;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		} while (--i != 0);
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 10;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 11;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 12;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 13;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 14;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 15;
		pP_ptr->Be_flg &= ~0x4000;

		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[6] = 0;
		((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
		Sa_parts_allmv(pEm, pP_ptr);

	case 1:	pEm->Spd_x = (signed char)pEm->Timer2 >> 2;
		pEm->Spd_z = 0;

		if (pEm->Sce_free0 & 1) {
			pEm->Routine_2 = 2;
			pEm->Timer3 = 1;
		}

		break;

	case 2:	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		pP_ptr = pEm->pSin_parts_ptr + 17;
		pP_ptr->Size_z += 0x199;

		((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)pEm->Timer3 * 0x1000;
		((short *)pP_ptr->pSa_dat_head)[8] = (signed char)pEm->Timer3 * 0x1000;
		Sa_parts_allmv(pEm, pP_ptr);

		pEm->Timer3++;
		pEm->Timer3 &= 1;

		break;

	case 3:	pEm->Timer0 = 45;
		pEm->Routine_2 = 4;

		pEm->Timer1 = 0x80;
		pEm->Timer2 = 6;

		pEm->Timer0 = 0;

	case 4:	pP_ptr = pEm->pSin_parts_ptr + 17;

		((short *)pP_ptr->pSa_dat_head)[6] -= pEm->Timer1;
		Sa_parts_allmv(pEm, pP_ptr);
		if (pEm->Timer2-- == 0) {
			pEm->Timer2 = 6;
			pEm->Timer1 *= -1;
		}

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 5;
			pEm->Timer0 = 0;
		}

		break;

	case 5:	pP_ptr = pEm->pSin_parts_ptr + 17;

		if (pEm->Timer0 >= 16) {
			pP_ptr->Size_z += 20;
			((short *)pP_ptr->pSa_dat_head)[6] -= 0x200;
			Sa_parts_allmv(pEm, pP_ptr);
		} else {
			((short *)pP_ptr->pSa_dat_head)[6] += 0x100;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr->Size_z -= 20;
		}
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 6;
			pEm->Timer0 = 30;
			pEm->Timer1 = 0;

			pP_ptr = pEm->pSin_parts_ptr + 18;
			EF->Dm_timer0 = 1;
			EF->Dm_cnt = 0;
			pP_ptr->Timer0 = 0;
			pP_ptr->Timer1 = 0;
		}

		break;

	case 6:	pP_ptr = pEm->pSin_parts_ptr + 17;
		((short *)pP_ptr->pSa_dat_head)[8] += 0x100;
		Sa_parts_allmv(pEm, pP_ptr);

		pP_ptr->Cdir_x += pEm->Timer1;
		pEm->Timer1 += 3;
		if (pEm->Timer0 < 5)
			pEm->Timer1 -= 6;

		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);

		if ((unsigned short)pEm->Timer0-- == 0) {
			pP_ptr = pEm->pSin_parts_ptr + 17;
			pP_ptr->Be_flg |= 0x8040; /* retail 0x8040 (Beta 0x40) */

			pP_ptr->Cdir_x = 600;
			pP_ptr->Cdir_y = 0;
			pP_ptr->Cdir_z = 0x400;
			RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
			MulMatrix(&pP_ptr->Workm, (MATRIX *)pP_ptr->M);

			pP_ptr->Workm.t[0] = -0x3543; /* retail (Beta -0x3705/-0x4130) */
			pP_ptr->Workm.t[2] = -0x4450;
			pEm->Timer3 = 1; /* retail-only */

			if (G.pEnemy[2]->Id == 0x44) {
				pP_ptr->Workm.t[0] = -0x51BA;
				pP_ptr->Workm.t[2] = -0x61BC;
				pEm->Timer3 = 0; /* retail-only */
			}

			pEm->Routine_2 = 7;
			pEm->Timer0 = 30;
			pEm->Timer2 = 3;

			*(long *)&pEm->Move_no = 0x1F0000;
			if ((signed char)pEm->Timer3) /* retail-only @0x801039CC */
				pP_ptr->Workm.t[1] -= 2000;
		}

		break;

	case 7:	Joint_move(pEm, Kan, Seq, 0x80);

		pP_ptr = pEm->pSin_parts_ptr + 17;
		pP_ptr->Cdir_x = pEm->Timer1;
		pP_ptr->Cdir_y = 0;
		pP_ptr->Cdir_z = 0;
		pEm->Timer1 -= 2;
		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
		MulMatrix2((MATRIX *)pP_ptr->M, &pP_ptr->Workm);

		pP_ptr->Workm.t[1] += pEm->Timer0;
		pEm->Timer0 += 9;
		lim = (signed char)pEm->Timer3 ? 150 : 0; /* retail: raised floor when Timer3 (Beta 0) */
		if (pP_ptr->Workm.t[1] > lim) {
			pP_ptr->Workm.t[1] = lim;
			pEm->Timer0 >>= 3;
			pEm->Timer0 *= -1;
			if (pEm->Timer2-- == 0) {
				pEm->Routine_2 = 8;
				pEm->Timer0 = 8;

				*(long *)&pEm->Move_no = 0x1F0001;

				pEm->Timer3 = 0;
				pEm->Timer2 = 0;
			}

			((short *)pP_ptr->pSa_dat_head)[8] -= 0x100;

		}
		((short *)pP_ptr->pSa_dat_head)[8] -= 0x100;
		Sa_parts_allmv(pEm, pP_ptr);

		pEm->Spd_x = -20;
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);

		break;

	case 8:	pP_ptr = pEm->pSin_parts_ptr + 17;
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 9;
			pEm->Timer0 = 8;
		}
		((short *)pP_ptr->pSa_dat_head)[8] += 0x40;
		Sa_parts_allmv(pEm, pP_ptr);

		if (pEm->Sce_free0 & 2) {
			pP_ptr = pEm->pSin_parts_ptr + 4;
			((short *)pP_ptr->pSa_dat_head)[8] -= (signed char)pEm->Timer3 * 10;
			if ((signed char)pEm->Timer3 < 90)
				pEm->Timer3++;
			if (((short *)pP_ptr->pSa_dat_head)[8] < 0)
				((short *)pP_ptr->pSa_dat_head)[8] = 0;

			Sa_parts_allmv(pEm, pP_ptr);
		}

		if (pEm->Sce_free0 & 4) { Spd_get(pEm, Kan, Seq, 0); Joint_move(pEm, Kan, Seq, 0x80); Add_speedXZ(pEm, 0); }

		break;

	case 9:	pP_ptr = pEm->pSin_parts_ptr + 17;
		((short *)pP_ptr->pSa_dat_head)[8] -= 0x40;
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 8;
			pEm->Timer0 = 8;
		}
		Sa_parts_allmv(pEm, pP_ptr);

		if (pEm->Sce_free0 & 2) {
			pP_ptr = pEm->pSin_parts_ptr + 4;
			((short *)pP_ptr->pSa_dat_head)[8] -= (signed char)pEm->Timer3 * 10;
			if ((signed char)pEm->Timer3 < 90)
				pEm->Timer3++;
			if (((short *)pP_ptr->pSa_dat_head)[8] < 0)
				((short *)pP_ptr->pSa_dat_head)[8] = 0;

			Sa_parts_allmv(pEm, pP_ptr);
		}

		if (pEm->Sce_free0 & 4) {
			Spd_get(pEm, Kan, Seq, 0);
			Joint_move(pEm, Kan, Seq, 0x80);
			Add_speedXZ(pEm, 0);
		}

	}

	if (pEm->Sce_free0 & 0x20) {
		Em30_me_tate_d = -10;
		Em30_me_yoko_d = 14;
	}
	if (pEm->Sce_free0 & 0x40) {
		Em30_me_tate_d = -10;
		Em30_me_yoko_d = -14;
	}
	if (pEm->Sce_free0 & 0x80) {
		Em30_me_tate_d = 0;
		Em30_me_yoko_d = 0;
	}

	if (pEm->Sce_free0 & 0x100) {
		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[8] += (signed char)pEm->Timer2 * 10;
		if ((signed char)pEm->Timer2 < 90)
			pEm->Timer2++;
		if (((short *)pP_ptr->pSa_dat_head)[8] > 0x1000)
			((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;

		Sa_parts_allmv(pEm, pP_ptr);
	}

	Em30_san(pEm, pEm->pSin_parts_ptr + 18);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 2;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em30_san(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	Em30_san_tbl[EF->Dm_timer0](pEm, pP_ptr); }

static void Em30_san_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	pP_ptr->Be_flg |= 0x4000; }

static void Em30_san_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (EF->Dm_cnt)
	{ case 0:
		pP_ptr->Timer0 = 0x1000;
		EF->Dm_cnt = 1;
		pP_ptr->Timer2 = 1;
		pP_ptr->Timer3 = (Rnd() & 3) + 1;

		pP_ptr->Be_flg &= ~0x4000;

		pP_ptr->Timer1 = 30;

	case 1:	if (pP_ptr->Timer1-- == 0) {
			EF->Dm_cnt = 2;
			pP_ptr->Timer1 = 60;
		}

		break;

	case 2:	pP_ptr->Size_x -= 20;
		if (pP_ptr->Timer1-- == 0)
			EF->Dm_cnt = 3;

		break;

	case 3:	pP_ptr->Size_x -= 0x200;
		if (pP_ptr->Size_x < 0) {
			EF->Dm_cnt = 4;
			pP_ptr->Be_flg |= 0x4000;
		}

		break;
	case 4:
		break;
	}

	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - pP_ptr->Timer0;
	((short *)pP_ptr->pSa_dat_head)[8] = pP_ptr->Timer0;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr->Timer0 += pP_ptr->Timer2 * 0x400;

	Sv.vx = 16;
	Sv.vy = 0x70;
	Sv.vz = 0;

	if (EF->Dm_cnt < 4) {

		if ((unsigned short)pP_ptr->Timer3-- == 0) {
			pP_ptr->Timer2 *= -1;
			pP_ptr->Timer3 = (Rnd() & 3) + 1;
			Esp_call((Rnd() + 0xDDC) | 0x140000, 0, &pP_ptr->Workm, &Sv);

		}

		if (pP_ptr->Timer0 < 0) {
			pP_ptr->Timer2 = 1;
			pP_ptr->Timer3 = (Rnd() & 3) + 1;
			pP_ptr->Timer0 += pP_ptr->Timer2 * 0x400;
			Esp_call((Rnd() + 0xDDC) | 0x140000, 0, &pP_ptr->Workm, &Sv);
		}

		if (pP_ptr->Timer0 > 0x1000) {
			pP_ptr->Timer2 = -1;
			pP_ptr->Timer3 = (Rnd() & 3) + 1;
			pP_ptr->Timer0 += pP_ptr->Timer2 * 0x400;
			Esp_call((Rnd() + 0xDDC) | 0x140000, 0, &pP_ptr->Workm, &Sv);
		}
	}

}

static void Em30_san_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

}

static void Em30_mv0b_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;	/* folded away by the compiler: retail SYM has no record */

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Timer0 = 60;
		pEm->Neck_flg = 0;

		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;

		break;

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		if (pEm->Sce_free0 & 2)
			pEm->Routine_2 = 2;

		break;

	case 2:	*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 3;

	case 3:	Kan = (unsigned long)pEm->pRoom_kan_t_ptr;
		Seq = (unsigned long)pEm->pRoom_seq_t_ptr;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		if (pEm->Sce_free0 & 4)
			pEm->Routine_2 = 4;

		break;

	case 4:	*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Timer0 = 60;
		pEm->Routine_2 = 5;

	case 5:	Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);
	}

	pP_ptr = pEm->pSin_parts_ptr + 1;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 2;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em30_mv0c_br(ENEMY_WORK *pEm)
{

}

static void Em30_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr; unsigned long i;

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Timer0 = 60;
		pEm->Neck_flg = 0;

		pEm->Timer3 = 0;
		pEm->Timer2 = 0;

		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;

		i = 16;
		pP_ptr = pEm->pSin_parts_ptr;
		do {
			pP_ptr->Be_flg |= 0x4000;
			pP_ptr++;
		} while (--i != 0);
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 5;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 6;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 10;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 11;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 12;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 13;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 14;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr = pEm->pSin_parts_ptr + 15;
		pP_ptr->Be_flg &= ~0x4000;

		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[6] = 0;
		((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
		Sa_parts_allmv(pEm, pP_ptr);

		break;

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		if (pEm->Sce_free0 & 2)
			pEm->Routine_2 = 2;

		break;

	case 2:	*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 3;

	case 3:	Kan = (unsigned long)pEm->pRoom_kan_t_ptr;
		Seq = (unsigned long)pEm->pRoom_seq_t_ptr;
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		break;

	case 4:	*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Timer0 = 60;

		if (pEm->Sce_free0 & 4)
			pEm->Routine_2 = 5;

		break;

	case 5:	Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		if (pEm->Sce_free0 & 0x10) {
			pEm->Routine_2 = 6;
			*(long *)&pEm->Move_no = 0;
			Joint_move(pEm, Kan, Seq, 0x80);
		}

		break;

	case 6:
		break;

	}

	if (pEm->Sce_free0 & 8) {
		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[8] -= (signed char)pEm->Timer3 * 10;
		if ((signed char)pEm->Timer3 < 90)
			pEm->Timer3++;
		if (((short *)pP_ptr->pSa_dat_head)[8] < 0)
			((short *)pP_ptr->pSa_dat_head)[8] = 0;

		Sa_parts_allmv(pEm, pP_ptr);
	}

	if (pEm->Sce_free0 & 0x20) {
		Em30_me_tate_d = -10;
		Em30_me_yoko_d = 14;
	}
	if (pEm->Sce_free0 & 0x40) {
		Em30_me_tate_d = -10;
		Em30_me_yoko_d = -14;
	}
	if (pEm->Sce_free0 & 0x80) {
		Em30_me_tate_d = 0;
		Em30_me_yoko_d = 0;
	}

	if (pEm->Sce_free0 & 0x100) {
		pP_ptr = pEm->pSin_parts_ptr + 4;
		((short *)pP_ptr->pSa_dat_head)[8] += (signed char)pEm->Timer2 * 10;
		if ((signed char)pEm->Timer2 < 90)
			pEm->Timer2++;
		if (((short *)pP_ptr->pSa_dat_head)[8] > 0x1000)
			((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;

		Sa_parts_allmv(pEm, pP_ptr);
	}

	pP_ptr = pEm->pSin_parts_ptr + 1;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 2;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000 - (signed char)Em30_iki[(signed char)EF->Iki_timer] * 5;
	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em30_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv; MATRIX M; PARTS_W *pP_ptr; short Dir;	/* pP_ptr, Dir: inferred; folded by the compiler (no retail SYM record) */

	if (pEm->Routine_2 == 0) {

		pP_ptr = pEm->pSin_parts_ptr + 1;
		Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Dir;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 100;
		Sv.vy = 0x100 - Rnd() * 2;
		Sv.vz = 0x80 - Rnd();
		if (pEm->Damage_no % 3 == 0) {
			Sv.vx = 100;
			Sv.vy = 0x3BC - Rnd() * 2;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP_ptr->Workm.t[0];
		Sv.vy += pP_ptr->Workm.t[1];
		Sv.vz += pP_ptr->Workm.t[2];

		if (pEm->Routine_1 != 16) {
			Esp_call(((Rnd() * 8) + 0x17D0) | 0x10000, Dir, &GsIDMATRIX, &Sv);
			Sv.vy += 0x40 - (Rnd() & 0x3F);
			Esp_call((Rnd() * 8) + 0x17D0, Dir, &GsIDMATRIX, &Sv);
			if (pEm->Routine_1 != 1) /* retail-only @0x801049AC */
				Snd_se_enem(8, pEm);
		}

		EF->Me_r_0 = 1;
		EF->Me_r_1 = 0;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em30_bomdm(pEm);

			if (pEm->Routine_1 == 17) /* retail-only damage type 17 */
				Em30_dm_type17(pEm);

			if (pEm->Routine_1 == 10)
				Em30_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em30_ryudm(pEm);
		}

		if (pEm->Routine_1 == 16 && !EF->Fire_c) {
			Em30_firedm(pEm);
			EF->Fire_c = 45;
		}

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6) /* retail adds 6 */
			EF->Mg_rout0 = 1;

		if (pEm->Routine_1 == 14)
			Em30_spdm(pEm);

		if (!(EF->Etc_flg & 4) && pEm->Life < 200) {
			EF->Dm_cnt = 50;
			EF->Etc_flg |= 4;
		}

		if (EF->Etc_flg & 0x80) { /* retail-only @0x80104AE0: restore the routine, then the dm00 tail */
			*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em30_dm00(pEm, Kan, Seq);
			return;
		}

		if (!(EF->Etc_flg & 1)) {

			if (pEm->Routine_1 == 9) {
				EF->Dm_cnt += Em30_Dm_cnt[pEm->Routine_1 - 1] >> 1;
				if (pEm->Damage_no < 3)
					EF->Dm_cnt += Em30_Dm_cnt[pEm->Routine_1 - 1] >> 1;

			} else
				EF->Dm_cnt += Em30_Dm_cnt[pEm->Routine_1 - 1];

			if (!(EF->Etc_flg & 2)) {
				EF->Etc_flg |= 2;
				EF->Dm_timer0 = 15;
			}
		}

		if (EF->Dm_cnt < 12) {
			*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em30_Move(pEm, Kan, Seq);
			return;
		}
		EF->Dm_cnt = 0;
		EF->Dm_timer0 = 0;
		EF->Etc_flg &= ~2;
	}

	Em30_dm00(pEm, Kan, Seq);

}

static void Em30_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x3000A - ((long (*)())Cdir_ck)(pEm, &G.Player);
		pEm->Routine_2 = 1;

		EF->Etc_flg |= 0x80; /* retail @0x80104C7C (Beta: Damage_cnt |= 0x80) */
		if (pEm->Routine_1 == 11) /* retail-only */
			Snd_se_enem(0, pEm);

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Routine_0 = 0x801;
			pEm->Damage_cnt &= 0x7F;
			EF->Etc_flg &= 0xFF7F; /* retail-only */
			Snd_se_enem(7, pEm); /* retail-only */
		}
		Add_speedXZ(pEm, 0);
	}
}

static void Em30_firedm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);

}

static void Em30_ryudm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	/* retail-only: four more sprays (retail 0x80104F00..) */
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[10].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[3].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[3].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[7].Workm, 0);

}

static void Em30_bomdm(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x13E8) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x830) | 0x5040000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x830) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x448) | 0x40F0000, pEm->Cdir_y, &pP_ptr[15].Workm, 0);
}

/* retail-only (em30_cdemd0_s1 0x8010509C): spray effect for damage type 17 (Em30_Damage
 * @0x80104A04, Em30_Die, Em30_die00). Name [INFERRED] after the Beta *dm effect helpers. */
static void Em30_dm_type17(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x3AF8) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[1].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[3].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[8].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() * 4) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
}

static void Em30_spdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv; PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = -0x1D8 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[2].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[2].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr[1].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[13].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[7].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() * 8) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[4].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr + 10;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr + 4;
	Esp_call(((Rnd() * 4) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);

}

static void Em30_mgdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv0; SVECTOR Sv1; SVECTOR Sv2; MATRIX M; PARTS_W *pP_ptr; short Dir;	/* pP_ptr, Dir: inferred; folded by the compiler (no retail SYM record) */

	switch (EF->Mg_rout0)
	{ case 1:

		EF->Mg_rout0 = 2;
		EF->Mg_timer0 = 0;
		EF->Mg_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

	case 2:	Sv0.vx = 0;
		Dir = Sv0.vy = EF->Mg_dir;
		Sv0.vz = 0;
		RotMatrix(&Sv0, &M);

		Sv0.vx = -((signed char)EF->Mg_timer0 * 100);
		Sv0.vy = 200;
		ApplyMatrixSV(&M, &Sv0, &Sv1);

		Sv0.vx = -100;
		Sv0.vy = -500;
		Sv0.vz = 0;

		if (pEm->Damage_no % 3 == 0)
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

		if (EF->Mg_timer0 == 1) {
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

		if ((signed char)EF->Mg_timer0++ > 16)
			EF->Mg_rout0 = 0;
	case 0:
		break; }

}

static void Em30_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em30_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv; MATRIX M; PARTS_W *pP_ptr; short Dir;	/* pP_ptr, Dir: inferred; folded by the compiler (no retail SYM record) */

	if ((EF->Etc_flg & 0x10) && pEm->Routine_2 == 0) {

		pP_ptr = pEm->pSin_parts_ptr + 1;
		Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Dir;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 100;
		Sv.vy = 0x100 - Rnd() * 2;
		Sv.vz = 0x80 - Rnd();
		if (pEm->Damage_no % 3 == 0) {
			Sv.vx = 100;
			Sv.vy = 0x3BC - Rnd() * 2;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP_ptr->Workm.t[0];
		Sv.vy += pP_ptr->Workm.t[1];
		Sv.vz += pP_ptr->Workm.t[2];

		if (pEm->Routine_1 != 16) {
			Esp_call(((Rnd() * 8) + 0x17D0) | 0x10000, Dir, &GsIDMATRIX, &Sv);
			Sv.vy += 0x40 - (Rnd() & 0x3F);
			Esp_call((Rnd() * 8) + 0x17D0, Dir, &GsIDMATRIX, &Sv);
			if (pEm->Routine_1 != 1) /* retail-only */
				Snd_se_enem(8, pEm);
		}

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em30_bomdm(pEm);

			if (pEm->Routine_1 == 17) /* retail-only damage type 17 */
				Em30_dm_type17(pEm);

			if (pEm->Routine_1 == 10)
				Em30_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em30_ryudm(pEm);
		}

		if (pEm->Routine_1 == 14)
			Em30_spdm(pEm);

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6) /* retail adds 6 */
			EF->Mg_rout0 = 1;

		if (pEm->Routine_1 == 16)
			Em30_firedm(pEm);

		*(long *)&pEm->Routine_0 = pEm->Tmp_routine;

		if (*(unsigned short *)&pEm->Routine_0 == 0x901) {
			Em30_mv08(pEm, Kan, Seq);
			return;
		}
	}

	Em30_die00(pEm, Kan, Seq);

}

static void Em30_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0; SVECTOR Sv1; MATRIX M; short Dir; unsigned char Ck;

	pEm->Life = 0;

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x3000A - ((long (*)())Cdir_ck)(pEm, &G.Player);
		pEm->Routine_2 = 1;

		pEm->Damage_cnt |= 0x80;

		EF->Etc_flg |= 0x11;

		EF->Me_r_0 = 2;
		EF->Me_r_1 = 0;

		Em_dead_flg_on(pEm);

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em30_bomdm(pEm);

			if (pEm->Routine_1 == 17) /* retail-only damage type 17 */
				Em30_dm_type17(pEm);

			if (pEm->Routine_1 == 10)
				Em30_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em30_ryudm(pEm);
		}

		if (pEm->Routine_1 == 14)
			Em30_spdm(pEm);

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6) /* retail adds 6 */
			EF->Mg_rout0 = 1;

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Move_no = 0x70011;
			pEm->Routine_2 = 2;
			pEm->Damage_cnt &= 0x7F;
		}
		Add_speedXZ(pEm, 0);
		break;

	case 2:	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 16);
		Spd_get(pEm, Kan, Seq, 0);
		Add_speedXZ(pEm, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Move_no = 0x32104;
			pEm->Routine_2 = 3;
			Spd_get(pEm, Kan, Seq, 0);

			pEm->Timer3 = 0;
			Snd_se_enem(3, pEm);
		}

		break;

	case 3:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400))
			*(long *)&pEm->Routine_0 = 0x901;

		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt == 0x2E && !*(signed char *)&pEm->Timer3) {
			*(long *)&pEm->Move_no = 0x30008;
			pEm->Timer3 = 1;
		}

		if (*pEm->pNow_seq & 0x30000) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 16);

			if (*(signed char *)&pEm->Timer3 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400))

				pEm->Cdir_y += 0x80;

			Sv0.vx = 500;
			Sv0.vy = 0x183;
			Sv0.vz = 0;
			Sv1.vx = 0x44C;
			Sv1.vy = 0x183;
			Sv1.vz = 0;
			Em30_hand(pEm, &M);
			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 900, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 900, &Sv1)) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - 0x400;

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (*(signed char *)&pEm->Timer3) {
					Ck ^= 1;
					Dir -= 0x800;
				}

				Pl_life_down(30, 1);

				*(long *)&G.Player.Routine_0 = ((Ck + 4) << 8) + 2;

				if (!Ck)
					G.Player.Cdir_y = Dir + 0x800;

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				Snd_se_enem(6, pEm);

			}
		}
	}
}

static void Em30_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em30_sce[pEm->Routine_1](pEm, Kan, Seq); }

static void Em30_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em30_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em30_tbl[pEm->Routine_1](pEm, Kan, Seq); }

static void Em30_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = pEm->Routine_1 + 0x30000;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		if (pEm->Routine_1 == 0) {
			Snd_se_stad(0x4000001, &pEm->Pos_x); }
		else {
			Snd_se_stad(0x4010001, &pEm->Pos_x);
		}

	case 1:	Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Routine_0 = 1;

			pEm->Damage_cnt &= 0x7F;
			pEm->Neck_flg = 0;

			G.Status_flg &= ~0x40;
		}
		Add_speedXZ(pEm, 0);

	}
}

static void Em30_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x30601;
		pEm->Routine_2 = 1;

		pEm->Spd_x = 800;
		pEm->Spd_z = 0;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;
		Snd_se_stad(0x4010001, &pEm->Pos_x); /* retail-only @0x80106538 */

	case 1:	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Spd_x -= 150;
		if (pEm->Spd_x < 300)
			pEm->Spd_x = 300;

		break;

	case 2:	*(long *)&pEm->Move_no = 0x30002;
		pEm->Routine_2 = 3;

	case 3:	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Spd_x -= 20;
		if (pEm->Spd_x < 0)
			pEm->Spd_x = 0;

		break;

	case 4:	*(long *)&pEm->Routine_0 = 1;

		pEm->Damage_cnt &= 0x7F;
		pEm->Neck_flg = 1;

		G.Status_flg &= ~0x40;
		break;
	}
	Add_speedXZ(pEm, 0x800);

}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em30_tbl[pEm->Routine_1](pEm, Kan, Seq); }

static void Em30_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;
	extern void Pad_vib_set0();
	extern void Pad_vib_set1();
	extern void Pad_vib_fade_set();
	register unsigned char Ofs; /* retail-only local, [INFERRED] name (as EM10_S0.C): Dual Shock strength offset */

	switch (pEm->Routine_2)
	{ case 0:
		*(long *)&pEm->Move_no = 0x30007;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		G.Player.Life = -1;

		pP_ptr = pEm->pSin_parts_ptr + 8;
		Esp_call(0x21000, pEm->Cdir_y, &pP_ptr->Workm, 0);
		/* retail-only (Dual Shock): sound and vibration script for the kill (retail 0x801067E0..) */
		Snd_se_stad(0x4020001, &pEm->Pos_x);
		Pad_vib_set0(2, 0);
		Pad_vib_set1(10, 0xFA, 0);
		Pad_vib_fade_set(0x1E, 0xFA, 0x50, 10);
		Ofs = (G.System_flg & 0x800) ? 15 : 0;
		Pad_vib_set0(2, 0x37 - Ofs);
		Pad_vib_set0(3, 0x3C - Ofs);
		Pad_vib_set0(3, 0x42 - Ofs);
		Pad_vib_set0(8, 0x4B - Ofs);
		Pad_vib_set1(0x1E, 0x78, 0x2D - Ofs);
		Pad_vib_fade_set(0x1E, 0x78, 0, 0x4B - Ofs);
		Pad_vib_set1(5, 0xFA, 0xAA);
		Pad_vib_fade_set(0x3C, 0xFA, 0, 0xAF);

	case 1:	if (pEm->Move_cnt == 0x17) /* retail-only */
			Snd_se_stad(0x4030001, &pEm->Pos_x);
		if (pEm->Move_cnt < 10) {
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | 0x1000, pEm->Cdir_y, &pP_ptr->Workm, 0);
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | 0x1000, pEm->Cdir_y, &pP_ptr[8].Workm, 0);
		}

		if (pEm->Move_cnt == 5) {
			pP_ptr = pEm->pSin_parts_ptr + 8;
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x40;
			*(long *)&pP_ptr->Free[2] = 0x202030;

			pP_ptr = pEm->pSin_parts_ptr;
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x80;
			*(long *)&pP_ptr->Free[2] = 0x101030;
		}

		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			pEm->Routine_2 = 2;
			pEm->Be_flg |= 2;
		}

		break;
	case 2:
		break;
	}
}

static void Em30_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	unsigned char Me3[7] = { 6, 48, 55, 59, 62, 64, 79 };

	unsigned char Me4[15] = { 14, 19, 22, 24, 25, 26, 27, 35, 36, 37, 38, 39, 40, 42, 50 };
	register short Tmp_t, Tmp_y;
	short Dt;	/* inferred name: no retail SYM record (narrowed temporary combined away) */

	Tmp_t = Em30_me_tate_a;
	Tmp_y = Em30_me_yoko_a;
	if ((signed char)Em30_me_tate_d < (signed char)Em30_me_tate)
		Em30_me_tate_a = -1;

	if ((signed char)Em30_me_tate < (signed char)Em30_me_tate_d)
		Em30_me_tate_a = 1;

	if ((signed char)Em30_me_yoko > (signed char)Em30_me_yoko_d)
		Em30_me_yoko_a = -1;

	if ((signed char)Em30_me_yoko < (signed char)Em30_me_yoko_d)
		Em30_me_yoko_a = 1;

	if (-Em30_me_tate_a == Tmp_t)
		Em30_me_tate_mv = Em30_me_tate_mv * 2 / 3;

	if (-Em30_me_yoko_a == Tmp_y)
		Em30_me_yoko_mv = Em30_me_yoko_mv * 2 / 3;

	if ((signed char)Em30_me_tate == (signed char)Em30_me_tate_d && __builtin_abs(Em30_me_tate_mv) < 3) {
		Em30_me_tate = Em30_me_tate_d;
		Em30_me_tate_a = 0;
		Em30_me_tate_mv = 0;
	}
	if (Em30_me_yoko == (signed char)Em30_me_yoko_d && __builtin_abs(Em30_me_yoko_mv) < 3) {
		Em30_me_yoko = Em30_me_yoko_d;
		Em30_me_yoko_a = 0;
		Em30_me_yoko_mv = 0;
	}

	Em30_me_tate_mv += Em30_me_tate_a;
	Em30_me_yoko_mv += Em30_me_yoko_a;

	Tmp_t = (signed char)Em30_me_tate;
	Tmp_y = (signed char)Em30_me_yoko;

	Em30_me_tate += Em30_me_tate_mv;
	Em30_me_yoko += Em30_me_yoko_mv;

	if ((signed char)Em30_me_tate > 20)
		Em30_me_tate = 20;

	if ((signed char)Em30_me_tate < -17)
		Em30_me_tate = -17;

	if ((signed char)Em30_me_yoko > 20)
		Em30_me_yoko = 20;

	if ((signed char)Em30_me_yoko < -17)
		Em30_me_yoko = -17;

	Dt = (signed char)Em30_me_tate - Tmp_t;
	Tmp_y = (signed char)Em30_me_yoko - Tmp_y;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me3);

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me4);

}

static void Em30_kuchi_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr, unsigned char *pSeq)
{
	unsigned char Me3[5] = { 4, 32, 85, 92, 104 };

	unsigned char Me4[3] = { 2, 9, 22 };

	unsigned char Za[3][2] = { { 0, 47 }, { 62, 67 }, { 95, 67 } };
	short Tmp_t, Tmp_y;
	unsigned char Zan;

	Zan = pSeq[EF->K_seq * 2];
	if (Zan == 0xFF)
		return;

	Tmp_t = pSeq[EF->K_seq * 2 + 1];
	if (Tmp_t == 0xFF) {
		EF->K_timer = 0;
		EF->K_seq = 0;
		return;
	}

	if (EF->K_timer) {
		EF->K_timer--;
		return;
	}

	EF->K_timer = Tmp_t;
	EF->K_seq++;

	Tmp_t = Za[Zan][1] - Za[EF->K_zan][1];
	Tmp_y = Za[Zan][0] - Za[EF->K_zan][0];
	EF->K_zan = Zan;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E0, Me3);

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E0, Me4);

}

static void Em30_core_me_sa(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	Em30_core_me_sa_tbl[EF->Me_r_0](pEm, pP_ptr); }

static void Em30_core_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

}

static void Em30_core_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	switch (EF->Me_r_1) {
	case 0:
		((short *)pP_ptr->pSa_dat_head)[8] += 0x400;
		if (((short *)pP_ptr->pSa_dat_head)[8] > 0x1000) {
			((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
			EF->Me_r_1 = 1;
			EF->Me_timer0 = 0;

			Em30_me_yoko_d = 14 - Rnd() % 28;
			Em30_me_tate_d = 14 - Rnd() % 28;
		}

		break;

	case 1:	((short *)pP_ptr->pSa_dat_head)[8] -= EF->Me_timer0;
		EF->Me_timer0 += 100;
		if (((short *)pP_ptr->pSa_dat_head)[8] < 0) {
			((short *)pP_ptr->pSa_dat_head)[8] = 0;
			EF->Me_r_0 = 0;
			EF->Me_r_1 = 0;

			Em30_me_yoko_d = 0;
			Em30_me_tate_d = 0;
		}
	}

	Sa_parts_allmv(pEm, pP_ptr);
}

static void Em30_core_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	switch (EF->Me_r_1) {
	case 0:
		((short *)pP_ptr->pSa_dat_head)[8] += 0x400;
		if (((short *)pP_ptr->pSa_dat_head)[8] > 0xC00) {
			((short *)pP_ptr->pSa_dat_head)[8] = 0xC00;
			EF->Me_r_1 = 1;
			EF->Me_timer0 = 0;
			EF->Me_timer1 = 3;
		}
		break;

	case 1:	((short *)pP_ptr->pSa_dat_head)[8] -= 500;
		EF->Me_timer0 += 100;
		if (!EF->Me_timer1--) {
			EF->Me_timer1 = 3;

			EF->Me_r_1 = 2; } break;

	case 2:	((short *)pP_ptr->pSa_dat_head)[8] += 500;
		EF->Me_timer0 -= 100;
		if (!EF->Me_timer1--) {
			EF->Me_timer1 = 3;
			EF->Me_r_1 = 1;
		}

	}

	Em30_me_yoko_d = 14 - Rnd() % 28;
	Em30_me_tate_d = 14 - Rnd() % 28;

	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em30_syoku(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	switch (pEm->Routine_1) {
	case 0:
		pEm->Timer0 = 0x200;
		pEm->Routine_1 = 1;

	case 1:	((short *)pP_ptr->pSa_dat_head)[6] -= pEm->Timer0;
		((short *)pP_ptr->pSa_dat_head)[8] += pEm->Timer0;
		if (((short *)pP_ptr->pSa_dat_head)[8] > 0x624) {
			pEm->Timer0 -= 0x40;
			if (pEm->Timer0 < 0xC0) {
				pEm->Routine_1 = 2;
				pEm->Timer0 = 0xC0;
			}
		} else {
			pEm->Timer0 += 0x40;
			if (pEm->Timer0 > 0x180)
				pEm->Timer0 = 0x180;
		}

		break;

	case 2:	((short *)pP_ptr->pSa_dat_head)[6] += pEm->Timer0;
		((short *)pP_ptr->pSa_dat_head)[8] -= pEm->Timer0;
		if (((short *)pP_ptr->pSa_dat_head)[8] < 0x200) {
			pEm->Timer0 -= 0x30;
			if (pEm->Timer0 < 0x90) {
				pEm->Routine_1 = 1;
				pEm->Timer0 = 0x100;
			}
		} else {
			pEm->Timer0 += 0x30;
			if (pEm->Timer0 > 0x100)
				pEm->Timer0 = 0x100;
		}

	}

	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em30_hand(ENEMY_WORK *pEm, MATRIX *pM)
{
	PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	RotMatrix(&pEm->Cdir_x, pEm->M);
	CompM(pEm->M, pP_ptr->M, pM);
	CompM(pM, pP_ptr[1].M, pM);
	CompM(pM, pP_ptr[2].M, pM);
	CompM(pM, pP_ptr[4].M, pM);
	CompM(pM, pP_ptr[5].M, pM);
	CompM(pM, pP_ptr[6].M, pM);
}
