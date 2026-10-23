/* em36.c - reconstruction of D:\Bio2\prog\EM36.C (enemy 36).
 * Linked into two overlays from the same source: 0x19 (at 0x80100000) and
 * 0x37 (at 0x8010d000); only the enemy-number-specific entry names differ
 * (Em136/Em236, Pldamege_Em136/Pldamege_Em236, Pldie_Em136/Pldie_Em236),
 * selected by OVL_37.
 * Types come from the TU's retail SYM preamble (em36.h).
 * Pointer locals that the compiler folded away entirely (no hard register) leave
 * no SYM record; such locals are named here by inference (marked [INFERRED]).
 * Several byte fields are written together as one word (*(long *)&pEm->Routine_0,
 * *(long *)&pEm->Move_no), as the retail word stores show. */
#include "EM36.H"

/* Em36 work variables kept in the enemy's generic Free[] area */
#define EF	((EM36_FREE *)ENEMY_FREE(pEm))
/* T1 read as a signed byte (lb) where the retail code does so */
#define EF_T1S	(*(signed char *)&EF->T1)

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static short Em36_pos_y;
static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = {
	250, 450, 300, 300, 220, 300, 400, 300, 400, 350
};
static unsigned short Em2a_life_easy[10] = {
	180, 340, 220, 240, 175, 240, 310, 240, 310, 300
};
/* Retail life tables (em36_cdemd0_s1 .data 0x8010556C..0x801055CB): Em2b values changed relative to Beta 2 and a
 * second complete set follows (same set as src-retail/prog/em20.c, selected there by
 * G.System_flg & 0x40); second-set names are [INFERRED], none is referenced by this unit. */
static unsigned short Em2b_life = 0xFA;
static unsigned short Em2b_life_easy = 0xC8;
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static void (*Em36_routine_0[8])() = { &Em36_Init, &Em36_Move, &Em36_Damage, &Em36_Die, &Em36_Scenario, 0, 0, &Em36_Dead };
static unsigned char Em36_Me3_0[10] = {
	9, 3, 4, 122, 123, 133, 150, 194, 201, 205
};
static unsigned char Em36_Me4_0[3] = {
	2, 218, 219
};
static unsigned char Em36_Me3_1[9] = {
	8, 0, 1, 2, 59, 60, 97, 189, 190
};
static unsigned char Em36_Me4_1[5] = {
	4, 39, 53, 91, 92
};
static MEDAMA Em36_Me[2] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, Em36_Me3_0, Em36_Me4_0, 2, 15, -15, 15, -15 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, Em36_Me3_1, Em36_Me4_1, 2, 15, -15, 15, -15 }
};
static void (*Em36_Move_type[1])() = { &Em36_Move_type00 };
static void (*Em36_mv_br_type00[5])() = { &Em36_mv00_br, &Em36_mv01_br, &Em36_mv02_br, &Em36_mv03_br, &Em36_mv04_br };
static void (*Em36_mv_type00[5])() = { &Em36_mv00, &Em36_mv01, &Em36_mv02, &Em36_mv03, &Em36_mv04 };
/* retail-only (.data 0x80105674, name [INFERRED]): Em36_set_em37_1set mode per Timer3 row and random column (Em36_mv03 0x80101A38) */
static unsigned long Em36_mv03_set_tbl[8][2] = {
	{ 0x301, 0x301 }, { 0xD01, 0xD01 }, { 0x301, 0x301 }, { 0xD01, 0x301 },
	{ 0xD01, 0x301 }, { 0xD01, 0xD01 }, { 0xD01, 0xD01 }, { 0x301, 0xD01 }
};
static unsigned char Em36_Dm_cnt[18] = {
	5, 5, 5, 5, 14, 20, 14, 20, 14, 14, 14, 5, 5, 20, 1, 1,
	20, 1
};
/* retail-only (.data 0x801056C8, name [INFERRED]): Em36_die00 effect spline points (x, y, z, pad) */
static short Em36_die_pos_tbl[4][4] = {
	{ 1903, -1046, -2310, 0 }, { 1703, -2646, -750, 0 }, { 2393, -546, 1410, 0 }, { 1443, -2266, 1410, 0 }
};
static void (*Em36_sce[])() = { };
static void (*Pldamage_Em36_tbl[1])() = { &Em36_pldm00 };
static void (*Pldie_Em36_tbl[1])() = { &Em36_pldie00 };
static JH_TBL G5a_jh0_tbl[186] = {
	{ 43, 4096 }, { 44, 4096 }, { 161, 4096 }, { 183, 4096 }, { 157, 4096 }, { 158, 4096 },
	{ 159, 4096 }, { 180, 4096 }, { 178, 4096 }, { 179, 4096 }, { 177, 4096 }, { 176, 4096 },
	{ 175, 4096 }, { 122, 4096 }, { 123, 4096 }, { 124, 4096 }, { 181, 4096 }, { 125, 4096 },
	{ 182, 4096 }, { 142, 4096 }, { 141, 4096 }, { 140, 4096 }, { 191, 4096 }, { 187, 4096 },
	{ 186, 4096 }, { 189, 4096 }, { 190, 4096 }, { 188, 4096 }, { 184, 4096 }, { 185, 4096 },
	{ 121, 3414 }, { 156, 3414 }, { 42, 3414 }, { 174, 3414 }, { 139, 3414 }, { 120, 2730 },
	{ 155, 2730 }, { 41, 2730 }, { 173, 2730 }, { 138, 2730 }, { 85, 2048 }, { 119, 2048 },
	{ 154, 2048 }, { 40, 2048 }, { 172, 2048 }, { 137, 2048 }, { 103, 2048 }, { 84, 1366 },
	{ 118, 1366 }, { 153, 1366 }, { 39, 1366 }, { 171, 1366 }, { 136, 1366 }, { 102, 1366 },
	{ 196, 1026 }, { 195, 1026 }, { 194, 1026 }, { 83, 682 }, { 117, 682 }, { 152, 682 },
	{ 38, 682 }, { 170, 682 }, { 135, 682 }, { 101, 682 }, { 0, 0 }, { 1, 0 },
	{ 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 },
	{ 8, 0 }, { 9, 0 }, { 10, 0 }, { 11, 0 }, { 12, 0 }, { 13, 0 },
	{ 14, 0 }, { 15, 0 }, { 16, 0 }, { 17, 0 }, { 18, 0 }, { 19, 0 },
	{ 20, 0 }, { 21, 0 }, { 22, 0 }, { 23, 0 }, { 24, 0 }, { 25, 0 },
	{ 26, 0 }, { 27, 0 }, { 28, 0 }, { 29, 0 }, { 30, 0 }, { 31, 0 },
	{ 32, 0 }, { 33, 0 }, { 34, 0 }, { 35, 0 }, { 36, 0 }, { 37, 0 },
	{ 45, 0 }, { 46, 0 }, { 47, 0 }, { 48, 0 }, { 49, 0 }, { 50, 0 },
	{ 51, 0 }, { 52, 0 }, { 53, 0 }, { 54, 0 }, { 55, 0 }, { 56, 0 },
	{ 57, 0 }, { 58, 0 }, { 59, 0 }, { 60, 0 }, { 61, 0 }, { 62, 0 },
	{ 63, 0 }, { 64, 0 }, { 65, 0 }, { 66, 0 }, { 67, 0 }, { 68, 0 },
	{ 69, 0 }, { 70, 0 }, { 71, 0 }, { 72, 0 }, { 73, 0 }, { 74, 0 },
	{ 75, 0 }, { 76, 0 }, { 77, 0 }, { 78, 0 }, { 79, 0 }, { 80, 0 },
	{ 81, 0 }, { 82, 0 }, { 92, 0 }, { 93, 0 }, { 94, 0 }, { 95, 0 },
	{ 96, 0 }, { 97, 0 }, { 98, 0 }, { 99, 0 }, { 100, 0 }, { 109, 0 },
	{ 110, 0 }, { 111, 0 }, { 112, 0 }, { 113, 0 }, { 114, 0 }, { 115, 0 },
	{ 116, 0 }, { 126, 0 }, { 127, 0 }, { 128, 0 }, { 129, 0 }, { 130, 0 },
	{ 131, 0 }, { 132, 0 }, { 133, 0 }, { 134, 0 }, { 143, 0 }, { 144, 0 },
	{ 145, 0 }, { 146, 0 }, { 147, 0 }, { 148, 0 }, { 149, 0 }, { 150, 0 },
	{ 151, 0 }, { 160, 0 }, { 162, 0 }, { 163, 0 }, { 164, 0 }, { 165, 0 },
	{ 166, 0 }, { 167, 0 }, { 168, 0 }, { 169, 0 }, { 197, 0 }, { 198, 0 }
};
static JH_HEAD G5a_jh0_head[1] = {
	{ 1, 0, 186, 0, 0, 0, 0, { -2146, -807, 0, 0 }, 0, 0, 0, 0, { 0, 0, 0, 0 }, G5a_jh0_tbl }
};
static JH_TBL G5a_jh1_tbl[13] = {
	{ 86, 0 }, { 87, 0 }, { 88, 0 }, { 89, 0 }, { 90, 0 }, { 91, 0 },
	{ 104, 0 }, { 105, 0 }, { 106, 0 }, { 107, 0 }, { 108, 0 }, { 193, 0 },
	{ 192, 0 }
};
static JH_HEAD G5a_jh1_head[1] = {
	{ 1, 3, 13, 0, 0, 0, 0, { -2146, -807, 0, 0 }, 0, 0, 0, 0, { -800, -500, 0, 0 }, G5a_jh1_tbl }
};
static char Em36_iki[64] = {
	12, 23, 35, 43, 51, 60, 66, 71, 75, 79, 82, 86, 89, 92, 94, 96,
	97, 98, 99, 100, 99, 98, 97, 96, 94, 92, 89, 87, 85, 83, 80, 78,
	75, 72, 68, 65, 62, 58, 54, 49, 44, 39, 34, 29, 24, 19, 13, 7,
	0, -6, -15, -11, -26, -30, -32, -34, -35, -35, -34, -31, -25, -20, -10, 0
};
static void (*Em37_K_tbl[6])() = { &Em37_K_mv00, &Em37_K_mv01, &Em37_K_mv02, &Em37_K_mv03, &Em37_K_mv04, &func_80104B94 };	/* retail: sixth state (.data 0x80105AD0) */
static short Em37_K_mv03_tbl[110] = {
	-15, -30, -50, -70, -60, -30, 0, 31, 64, 99, 136, 175,
	215, 257, 301, 341, 384, 414, 440, 460, 467, 473, 478, 482,
	478, 490, 483, 497, 488, 503, 492, 506, 493, 508, 493, 509,
	494, 511, 496, 512, 497, 511, 497, 511, 496, 510, 495, 510,
	495, 509, 494, 508, 493, 507, 492, 506, 491, 505, 490, 504,
	489, 503, 488, 502, 487, 500, 485, 473, 461, 449, 437, 425,
	410, 400, 390, 380, 370, 358, 344, 330, 315, 300, 285, 270,
	256, 240, 222, 202, 183, 168, 150, 135, 110, 95, 80, 65,
	50, 38, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0
};

void EM_MAIN(ENEMY_WORK *pEm)
{	short Dir; SVECTOR Sv; PARTS_W *pP_ptr;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (EF->Etc_flg & 1) {
			if (EF->Dm_timer0-- <= 0) {	/* [INFERRED] spelling: retail tests the pre-decrement value (== 0 for this unsigned byte) */
				if (EF->Dm_cnt)
					EF->Dm_cnt--;
				EF->Dm_timer0 = 15;
				if (!EF->Dm_cnt)
					EF->Etc_flg &= 0xFFFE;
			}
		}

		Em36_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		if (EF->Kuchi_at_timer)
			EF->Kuchi_at_timer--;

		if (!(EF->Etc_flg & 8)) {	/* retail-only guard (0x801001A8) */
			if (!(pEm->Move_cnt & 0xF))
				Em36_core_set_dest(0, 15 - Rnd() % 30, 15 - Rnd() % 30);

			if ((pEm->Move_cnt & 0xF) == 7)
				Em36_core_set_dest(1, 15 - Rnd() % 30, 15 - Rnd() % 30);
		}

		Em36_core_me_mv(pEm, 0);
		Em36_core_me_mv(pEm, 1);

		Em36_mv_quake(pEm);

		Em36_die_wait(pEm);

		Em_seq_sound(pEm);

		Em37_K(pEm, &pEm->pSin_parts_ptr[2]);

		Dir = 0;
		pEm->pEnemy_ptr = (unsigned long *)&G.Player;
		Rot_neck_em_n(pEm, pEm->Cdir_y, &Dir);
		Dir &= 0xFFF;
		if (Dir > 0x800)
			Dir -= 0xFFF;
		pP_ptr = pEm->pSin_parts_ptr + 1;
		pP_ptr->Cdir_y += Dir;
		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
		pP_ptr->Cdir_y -= Dir;

		Oba_set_ofs(pEm, &pEm->Pos_x);

		pEm->At_em_no = 0xFF;

		Oba_ck_hit(G.pEnemy[0], pEm);

	}

}

static void Em36_after(ENEMY_WORK *pEm)
{
	G5a_Jh_move(pEm); }

static void Em36_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	PARTS_W *pP_ptr, *pO_ptr; SVECTOR Sv; MATRIX M;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	/* retail: 600, 400 in easy mode (0x801003FC), inside the "System_flg & 0x40" second life set with
	 * identical arms (Retail lever 2, docs/RETAIL_MATCHING.md): the two-armed if/else ends the CSE path
	 * so no G-relative base register survives into the body; cross-jumping later merges the arms. */
	if (G.System_flg & 0x40) {
		pEm->Life = 600;
		if (G.System_flg & 0x20)
			pEm->Life = 400;
	}else {
		pEm->Life = 600;
		if (G.System_flg & 0x20)
			pEm->Life = 400;
	}
	pEm->D_life_u = -126;
	pEm->D_life_c = -126;
	pEm->D_life_d = -126;

	pEm->Cdir_y = 0;

	pP_ptr = pEm->pSin_parts_ptr + 1;
	pEm->Neck_flg = 1;
	pEm->Neck_no = 1;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 8;
	pP_ptr->Free[3] = 8;
	pP_ptr->Free[4] = 0xD4;
	pP_ptr->Free[5] = 0xD4;

	EF->_R0 = 0;
	EF->_R1 = 0;
	EF->_R2 = 0;
	EF->T0 = 0;
	EF->T1 = 0;
	EF->Q0_t = 0;
	EF->Q1_t = 0;
	EF->Q_timer = 0;
	EF->Syoku_flg = 0;
	EF->Kuchi_at_timer = 0;
	pEm->L_pl = 30000;	/* retail-only (0x8010049C) */

	Em36_Me[0].Tate = 0;
	Em36_Me[0].Yoko = 0;
	Em36_Me[0].Tate_d = 0;
	Em36_Me[0].Yoko_d = 0;
	Em36_Me[0].Tate_mv = 0;
	Em36_Me[0].Yoko_mv = 0;
	Em36_Me[0].Tate_a = 0;
	Em36_Me[0].Yoko_a = 0;

	Em36_Me[1].Tate = 0;
	Em36_Me[1].Yoko = 0;
	Em36_Me[1].Tate_d = 0;
	Em36_Me[1].Yoko_d = 0;
	Em36_Me[1].Tate_mv = 0;
	Em36_Me[1].Yoko_mv = 0;
	Em36_Me[1].Tate_a = 0;
	Em36_Me[1].Yoko_a = 0;

	pEm->Be_flg |= 8;

	pEm->nOba = 2;
	pEm->Atd[0].Ofs_x = -2000;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 6000;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 6000;
	pEm->Atd[0].W = 6000;
	pEm->Atd[0].D = 6000;

	pEm->Atd[1].Ofs_x = 2200;
	pEm->Atd[1].Ofs_y = -1500;
	pEm->Atd[1].Ofs_z = 0;
	pEm->Atd[1].At_w = 1300;
	pEm->Atd[1].At_h = 1500;
	pEm->Atd[1].At_d = 1300;
	pEm->Atd[1].W = 1300;
	pEm->Atd[1].D = 1300;

	pEm->Be_flg |= 2;

	pEm->Eff_at_r = 0x1644;

	pEm->Be_flg |= 0xC000000;

	*(long *)&pEm->Move_no = 0;
	Joint_move(pEm, Kan, Seq, 0x100);

	G.pPldamage_Em[ pEm->Id - 0x10 ] = EM_PLDM;
	G.pPldie_Em[ pEm->Id - 0x10 ] = EM_PLDIE;

	pEm->Be_flg |= 4;

	Kage_work_set(&pEm->pKage_work, 0, 0xFA00FA0, 0, &pEm->pSin_parts_ptr->Workm.t[0]);

	pEm->pSin_parts_ptr->Be_flg |= 0x4000;
	if (!G.pEnemy[3]->Pos_y)
		G.pFree_work = (unsigned char *)G5a_Jh_init(pEm, (unsigned long)G.pFree_work);

	pEm->pTafter_func = Em36_after;

	pP_ptr = pEm->pSin_parts_ptr + 2;
	pO_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg = 1;

	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = (unsigned long *)pO_ptr;
	pP_ptr->Pos_x = 1800;
	pP_ptr->Pos_y = 4500;
	pP_ptr->Pos_z = 0;

	pP_ptr = pEm->pSin_parts_ptr + 3;
	pO_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg = 1;
	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = (unsigned long *)pO_ptr;
	pP_ptr->Pos_x = 800;
	pP_ptr->Pos_y = 500;
	pP_ptr->Pos_z = 0;

	pP_ptr = pEm->pSin_parts_ptr + 4;
	pO_ptr = pEm->pSin_parts_ptr + 2;
	pP_ptr->Be_flg = 1;
	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = (unsigned long *)pO_ptr;
	pP_ptr->Pos_x = 1698;
	pP_ptr->Pos_y = -1398;
	pP_ptr->Pos_z = 0;

	pP_ptr = pEm->pSin_parts_ptr + 5;
	pP_ptr->Be_flg = 1;
	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = (unsigned long *)pO_ptr;
	pP_ptr->Pos_x = 1432;
	pP_ptr->Pos_y = -2691;
	pP_ptr->Pos_z = 1023;

	pP_ptr = pEm->pSin_parts_ptr + 6;
	pP_ptr->Be_flg = 1;
	pP_ptr->pSuper = (unsigned long *)&pO_ptr->Workm;
	pP_ptr->pOya_parts = (unsigned long *)pO_ptr;
	pP_ptr->Pos_x = 1430;
	pP_ptr->Pos_y = -2284;
	pP_ptr->Pos_z = -2078;

	pP_ptr = pEm->pSin_parts_ptr + 2;
	((short *)pP_ptr->pSa_dat_head)[6] = 0;
	((short *)pP_ptr->pSa_dat_head)[8] = 0;
	Sa_parts_allmv(pEm, pP_ptr);

	pEm->Routine_1 = 2;

}

static void Em36_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em36_Move_type[ pEm->Type & 0x3F ](pEm, Kan, Seq); }

static void Em36_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if ((EF->Etc_flg & 6) == 4) {
		*(long *)&pEm->Routine_0 = 3;
		return;
	}

	Em36_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em36_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq);

}

static void Em36_mv00_br(ENEMY_WORK *pEm)
{	unsigned long LL; short Ck;	/* Ck [INFERRED]: no SYM record */

	LL = pEm->L_pl;

	Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);

	if (pEm->Pos_x > 9000 && LL < 6000 && !Ck && !G.Player.Damage_cnt && G.Player.Life <= 0 && !(EF->Etc_flg & 4)) {

		*(long *)&pEm->Routine_0 = 0x401;
		Pl_life_down(500, 0);
		G.Player.Damage_cnt |= 0x80;
		EF->_R0 = 4;
		EF->_R1 = 0;
		G.System_flg |= 0x100;

	}

}

static void Em36_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	unsigned long i, j; unsigned char Ok_no[4]; unsigned char Ok_num;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 1;

		pEm->Timer0 = (Rnd() & 0x1F) + 120;
		pEm->Timer3 = 0;

		if (pEm->Pos_x < 8000 || pEm->L_pl > 11000) {	/* retail: also when far away (0x801009D4) */

			i = 4;
			j = 0;
			Ok_num = 0;
			do {

				if (!((EF->Syoku_flg >> j) & 1)) {

					if (!((EF->Syoku_flg >> (j + 4)) & 1)) {
						Ok_no[ Ok_num ] = j;
						Ok_num++;
					}
				}

				j++;	}while (--i);

			if (Ok_num == 4) {
				*(long *)&pEm->Routine_0 = 0x101;
				break;
			}
			pEm->Timer3 = 1;	/* retail-only (0x80100A48) */
			pEm->Timer0 = 10;
		}

		pEm->Timer2 = (Rnd() >> (Rnd() & 3)) & 7;	/* retail-only set row (0x80100A54) */
		if (pEm->L_pl < 7000)
			pEm->Timer2 = 0;

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x80))
			if (Rnd() & 3) {
				Snd_se_enem(9, pEm);
			}

		if (!(pEm->Timer0 & 0xF) && !(pEm->Timer3 & 1)) {

			i = 4;
			j = 0;
			Ok_num = 0;
			do {

				if (!((EF->Syoku_flg >> j) & 1)) {

					if (!((EF->Syoku_flg >> (j + 4)) & 1)) {
						Ok_no[ Ok_num ] = j;
						Ok_num++;
					}
				}

				j++;	}while (--i);

			if (Ok_num) {
				if (pEm->L_pl < 7000)
					pEm->Timer2 = 0;
				Em36_set_em37_1set(Ok_no[ Rnd() % Ok_num ], Em36_mv03_set_tbl[ (signed char)pEm->Timer2 ][ (Rnd() >> (Rnd() & 3)) & 1 ]);
			}

		}

		if (pEm->Timer0) {
			pEm->Timer0--;

			if (pEm->Timer0) break;
		}
		pEm->Timer3 |= 1;

		i = 4;
		j = 0;
		Ok_num = 0;
		do {

			if (!((EF->Syoku_flg >> j) & 1)) {

				if ((EF->Syoku_flg >> (j + 4)) & 1)
					Ok_num++;
			}

			j++;	}while (--i);

		if (!Ok_num) {

			*(long *)&pEm->Routine_0 = 0x301;

			if ((Rnd() & 3) && pEm->Pos_x < 10000)
				*(long *)&pEm->Routine_0 = 0x101;

			if (pEm->L_pl > 10000)	/* retail-only (0x80100C74) */
				*(long *)&pEm->Routine_0 = 0x101;

			if (pEm->Pos_x > 9000 && !(Rnd() & 1))
				*(long *)&pEm->Routine_0 = 0x301;

			if (pEm->Pos_x < 7000 && !(Rnd() & 1))
				*(long *)&pEm->Routine_0 = 0x101;
		}

	}

}

static void Em36_mv01_br(ENEMY_WORK *pEm)
{
	/* retail: empty (0x80100D08) */
}

static void Em36_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned char Em36_mv01[4][4] = { { 0, 2, 1, 3 }, { 1, 2, 3, 0 }, { 3, 0, 2, 1 }, { 3, 1, 2, 0 } };

	EF->Etc_flg |= 2;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0005;
		pEm->Routine_2 = 1;

		Em36_set_em37_all(0xB01);

		Snd_se_enem(10, pEm);

		pEm->Timer3 = (Rnd() >> (Rnd() & 3)) & 3;

	case 1:

		if (!pEm->Move_cnt)
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][0], 0xB01);

		if (pEm->Move_cnt == 7)
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][1], 0xB01);

		if (pEm->Move_cnt == 10)
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][2], 0xB01);

		if (pEm->Move_cnt == 15)
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][3], 0xB01);

		if (pEm->Move_cnt == 55) {
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][0], 0x70B01);

			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][2], 0x70B01);

			Snd_se_enem(9, pEm);
		}

		if (pEm->Move_cnt == 61) {
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][1], 0x70B01);

			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][3], 0x70B01);

		}

		if (pEm->Move_cnt == 90)
			Snd_se_enem(11, pEm);

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Spd_x += pEm->Spd_x >> 1;
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);

		if (pEm->Pos_x < 12000)
			Add_speedXZ(pEm, 0);

		break;

	case 2:
		*(long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 3;

		Snd_se_enem(10, pEm);

	case 3:
		Joint_move(pEm, Kan, Seq, 0x80);
		if (pEm->Move_cnt == 85) {	/* retail: end on frame 85, not on the Joint_move result (0x80101024) */
			*(long *)&pEm->Routine_0 = 1;

			EF->Etc_flg &= 0xFFFD;
		}

		if (pEm->Move_cnt == 25) {
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][0], 0x90B01);

			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][2], 0x90B01);

			Snd_se_enem(9, pEm);
		}
		if (pEm->Move_cnt == 15) {
			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][1], 0x90B01);

			Em36_set_em37_1set(Em36_mv01[ (signed char)pEm->Timer3 ][3], 0x90B01);
		}

	}

}

static void Em36_mv02_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	PARTS_W *pP_ptr; ENEMY_WORK *pKem; GLOBAL *pG; long Ck;

	pG = &G;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0001;
		pEm->Neck_flg &= 0xFD;
		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[6] = 0;
		((short *)pP_ptr->pSa_dat_head)[8] = 0;
		((short *)pP_ptr->pSa_dat_head)[10] = 0;
		((short *)pP_ptr->pSa_dat_head)[12] = 0;

		if (pEm->Sce_free0 & 1) {
			pEm->Routine_2 = 1;
			pEm->Be_flg &= ~2;

			pEm->Pos_x = -9000;
			pEm->Pos_z = -23400;
			pEm->Timer0 = 0;

			pP_ptr = pEm->pSin_parts_ptr;
			Em36_pos_y = pP_ptr->Pos_y;

			EF->_R0 = 3;
			EF->_R1 = 0;

			Snd_se_enem(10, pEm);
		}

		break;

	case 1:
		pEm->Timer0++;

		if (pEm->Timer0 == 10) {

			pKem = pG->pEnemy[3];
			pKem->Routine_2 = 1;
			pP_ptr = pKem->pSin_parts_ptr;
			pP_ptr->Size_x = 0;
		}
		if (pEm->Timer0 == 30) {

			pKem = pG->pEnemy[5];
			pKem->Routine_2 = 1;
			pP_ptr = pKem->pSin_parts_ptr;
			pP_ptr->Size_x = 0;
		}
		if (pEm->Timer0 == 40) {

			pKem = pG->pEnemy[6];
			pKem->Routine_2 = 1;
			pP_ptr = pKem->pSin_parts_ptr;
			pP_ptr->Size_x = 0;

			Snd_se_enem(9, pEm);

		}

		if (pEm->Timer0 == 90) {
			pEm->Routine_2 = 2;

			pKem = pG->pEnemy[3];
			pKem->Routine_2 = 6;
			pKem = pG->pEnemy[4];
			pKem->Routine_2 = 6;
			pKem = pG->pEnemy[5];
			pKem->Routine_2 = 6;
			pKem = pG->pEnemy[6];
			pKem->Routine_2 = 6;

			Snd_se_enem(11, pEm);

			Snd_se_enem(10, pEm);	}	break;

	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		pP_ptr = pEm->pSin_parts_ptr;
		Ck = pP_ptr->Pos_y - Em36_pos_y;

		Ck = (unsigned long)(Ck * 8830) >> 12;	/* [INFERRED] unsigned shift: retail emits srl here, sra in case 6 */

		((short *)pP_ptr[2].pSa_dat_head)[8] = Ck;

		break;

	case 3:
		pEm->Timer0 = 0;
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0x1F0003;

		Snd_se_enem(9, pEm);

	case 4:
		Joint_move(pEm, Kan, Seq, 0x80);

		if (pEm->Timer0 == 30) {

			pKem = pG->pEnemy[3];
			pKem->Routine_2 = 8;
			pKem = pG->pEnemy[4];
			pKem->Routine_2 = 8;
			pKem = pG->pEnemy[5];
			pKem->Routine_2 = 8;
			pKem = pG->pEnemy[6];
			pKem->Routine_2 = 8;

			Snd_se_enem(0, pEm);
		}

		pEm->Timer0++;

		if (pEm->Timer0 == 90) {
			pEm->Routine_2 = 5;
			pEm->Timer0 = 0;

			Snd_se_enem(10, pEm);
		}
		break;

	case 5:
		Joint_move(pEm, Kan, Seq, 0x80);

		if (pEm->Timer0 == 20) {

			pKem = pG->pEnemy[3];
			*(long *)&pKem->Routine_0 = 0x901;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 40) {
			pKem = pG->pEnemy[4];
			*(long *)&pKem->Routine_0 = 0x901;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 45) {
			pKem = pG->pEnemy[5];
			*(long *)&pKem->Routine_0 = 0x901;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 56) {
			pKem = pG->pEnemy[6];
			*(long *)&pKem->Routine_0 = 0x901;

			Snd_se_enem(0, pEm);
		}

		pEm->Timer0++;

		if (pEm->Timer0 == 110) {
			pEm->Routine_2 = 6;
			pEm->Timer0 = 0;
			*(long *)&pEm->Move_no = 0x1F0004;

			pKem = pG->pEnemy[3];
			pKem->Routine_2 = 7;
			pKem = pG->pEnemy[4];
			pKem->Routine_2 = 7;
			pKem = pG->pEnemy[5];
			pKem->Routine_2 = 7;
			pKem = pG->pEnemy[6];
			pKem->Routine_2 = 7;

			Snd_se_enem(11, pEm);
		}

		break;

	case 6:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);

		pP_ptr = pEm->pSin_parts_ptr;
		Ck = pP_ptr->Pos_y - Em36_pos_y;

		if (Ck)
			Ck = (Ck * 8830) >> 12;

		((short *)pP_ptr[2].pSa_dat_head)[8] = Ck;

		break;

	case 7:
		pP_ptr = pEm->pSin_parts_ptr + 2;
		((short *)pP_ptr->pSa_dat_head)[8] = 0;

		*(long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 8;

		pKem = pG->pEnemy[3];
		pKem->Routine_2 = 9;
		pKem = pG->pEnemy[4];
		pKem->Routine_2 = 9;
		pKem = pG->pEnemy[5];
		pKem->Routine_2 = 9;
		pKem = pG->pEnemy[6];
		pKem->Routine_2 = 9;

		Snd_se_enem(10, pEm);

	case 8:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);	break;

	case 9:
		*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 10;
		pEm->Timer0 = 0;

		Snd_se_enem(9, pEm);

	case 10:
		Joint_move(pEm, Kan, Seq, 0x80);

		if (pEm->Timer0 == 20) {

			pKem = pG->pEnemy[3];
			*(long *)&pKem->Routine_0 = 0xA01;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 35) {
			pKem = pG->pEnemy[5];
			*(long *)&pKem->Routine_0 = 0xA01;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 40) {
			pKem = pG->pEnemy[4];
			*(long *)&pKem->Routine_0 = 0xA01;

			Snd_se_enem(0, pEm);
		}

		if (pEm->Timer0 == 50) {
			pKem = pG->pEnemy[6];
			*(long *)&pKem->Routine_0 = 0xA01;

			Snd_se_enem(0, pEm);
		}

		pEm->Timer0++;

		if (pEm->Timer0 == 110)

			pEm->Routine_2 = 11;	break;

	case 11:
		*(long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 12;

		pKem = pG->pEnemy[3];
		*(long *)&pKem->Routine_0 = 0x40A01;

		pKem = pG->pEnemy[5];
		*(long *)&pKem->Routine_0 = 0x40A01;

		Snd_se_enem(10, pEm);

	case 12:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
		break;

	case 13:
		*(long *)&pEm->Move_no = 0x1F0000;
		pEm->Routine_2 = 14;

		EF->_R0 = 0;
		EF->_R1 = 0;

		pEm->Timer0 = 60;

		Snd_se_enem(9, pEm);

		Em36_set_em37_all(0x601);

	case 14:
		Joint_move(pEm, Kan, Seq, 0x80);

		if ((unsigned short)pEm->Timer0-- != 0) break;	/* [INFERRED] cast: retail tests the pre-decrement value */

		*(long *)&pEm->Routine_0 = 0x101;

		pEm->Neck_flg = 0;

		Snd_se_enem(10, pEm);

		Snd_se_enem(9, pEm);

	}

}

static void Em36_mv03_br(ENEMY_WORK *pEm)
{
	/* retail: empty (0x80101808) */
}

static void Em36_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	unsigned long i, j; unsigned char Ok_no[4]; unsigned char Ok_num;

	EF->Etc_flg |= 2;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 1;

		pEm->Timer0 = 0;
		pEm->Timer2 = 0;

		Snd_se_enem(11, pEm);

		pEm->Timer1 = 0;
		pEm->Timer3 = (Rnd() >> (Rnd() & 3)) & 7;	/* retail-only (0x80101884) */

	case 1:

		if (!*(signed char *)&pEm->Timer2) {
			pEm->Timer0++;
			if (pEm->Timer0 == 50) {
				pEm->Timer2 |= 1;
				Snd_se_enem(11, pEm);
			}
		}
		if ((signed char)pEm->Timer2 == 1) {
			pEm->Timer0--;
			if (!pEm->Timer0) {
				pEm->Timer2 = 2;
				Snd_se_enem(10, pEm);
			}
		}

		pEm->Spd_x = pEm->Timer0;

		if (pEm->Pos_x > 4000)
			pEm->Pos_x -= pEm->Timer0;

		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(long *)&pEm->Routine_0 = 1;

			EF->Etc_flg &= 0xFFFD;
		}

		if (!(pEm->Timer1 & 0x1F)) {

			i = 4;
			j = 0;
			Ok_num = 0;
			do {

				if (!((EF->Syoku_flg >> j) & 1)) {

					if (!((EF->Syoku_flg >> (j + 4)) & 1)) {
						Ok_no[ Ok_num ] = j;
						Ok_num++;
					}
				}

				j++;	}while (--i);

			if (Ok_num) {
				if (pEm->L_pl < 7000)	/* retail-only (0x801019E4) */
					pEm->Timer3 = 0;
				Em36_set_em37_1set(Ok_no[ Rnd() % Ok_num ], Em36_mv03_set_tbl[ (signed char)pEm->Timer3 ][ (Rnd() >> (Rnd() & 3)) & 1 ]);
			}

		}

		pEm->Timer1++;

	}

}

static void Em36_mv04_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Sv0; PARTS_W *pP_ptr;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Routine_2 = 1;

		/* retail: no Goto00 turn toward the player here (0x80101B14) */

		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		Set_base_pos(&G.Player, Kan, Seq, 0);

		*(long *)&G.Player.Routine_0 = 6;
		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

		G.Player.Damage_cnt |= 0x80;
		pEm->Damage_cnt |= 0x80;

		pEm->Be_flg |= 0x1000;
		G.Player.Be_flg |= 0x1002;

		G.Player.Timer1 = pEm->Cdir_y + 0x800;

		Snd_se_enem(6, pEm);

	case 1:
		if (pEm->Move_cnt == 10)
			Snd_se_enem(7, pEm);

		if (pEm->Move_cnt >= 16 && pEm->Move_cnt < 30) {
			pP_ptr = pEm->pSin_parts_ptr + 4;
			Sv0.vx = 0x100 - (Rnd() << 1);
			Sv0.vy = 0x100 - (Rnd() << 1);
			Sv0.vz = 0x100 - (Rnd() << 1);
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x17D0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		}

		Null_pos_set(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200))

			pEm->Routine_2 = 2;	break;

	case 2:
		*(long *)&pEm->Move_no = 0x70002;
		pEm->Routine_2 = 3;

		Snd_se_enem(10, pEm);

		pEm->Timer1 = 150;

	case 3:
		Joint_move(pEm, Kan, Seq, 0x200);

		if (pEm->Timer1)
			pEm->Timer1--;

		pP_ptr = pEm->pSin_parts_ptr + 2;
		if (!pEm->Timer1 && ((short *)pP_ptr->pSa_dat_head)[6] < 6000) {
			EF->_R0 = 0;
			EF->_R1 = 0;
		}

	}

}

static void Em36_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_mv08_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_mv09_br(ENEMY_WORK *pEm)
{

}

static void Em36_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_firedm(ENEMY_WORK *pEm)
{	SVECTOR Sv0; PARTS_W *pP_ptr;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	pP_ptr = pEm->pSin_parts_ptr + 2;
	Sv0.vx = G.Wep_hit_pos.vx - pP_ptr->Workm.t[0];
	Sv0.vy = G.Wep_hit_pos.vy - pP_ptr->Workm.t[1];
	Sv0.vz = G.Wep_hit_pos.vz - pP_ptr->Workm.t[2];
	Esp_call(0x5042710, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	Esp_call(0x40F1000 | (Rnd() << 2), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vx += 800;
	Esp_call(0x5042710, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vx -= 800;
	Esp_call(((Rnd() << 2) + 8000) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vz -= 812;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 8000) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vz -= 412;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 4000) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vz += 2036;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 8000) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vz += 412;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 4000) | 0x5040000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

}

static void Em36_ryudm(ENEMY_WORK *pEm)
{	SVECTOR Sv0; PARTS_W *pP_ptr;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	pP_ptr = pEm->pSin_parts_ptr + 2;
	Sv0.vx = G.Wep_hit_pos.vx - pP_ptr->Workm.t[0];
	Sv0.vy = G.Wep_hit_pos.vy - pP_ptr->Workm.t[1];
	Sv0.vz = G.Wep_hit_pos.vz - pP_ptr->Workm.t[2];
	Esp_call(((Rnd() << 2) + 10096) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	Sv0.vx += 800;
	Esp_call(((Rnd() << 2) + 10000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vx -= 800;
	Sv0.vz -= 612;
	Esp_call(((Rnd() << 2) + 8000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	Sv0.vz += 1224;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 8000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

	Sv0.vz += (Rnd() << 1) - 868;
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 6000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	Sv0.vz += 0x100 - (Rnd() << 1);
	Sv0.vy += 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 6000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

}

static void Em36_bomdm(ENEMY_WORK *pEm)
{	SVECTOR Sv0; PARTS_W *pP_ptr;	/* pP_ptr [INFERRED]: folded away, no SYM record */

	pP_ptr = pEm->pSin_parts_ptr + 2;
	Sv0.vx = G.Wep_hit_pos.vx - pP_ptr->Workm.t[0];
	Sv0.vy = G.Wep_hit_pos.vy - pP_ptr->Workm.t[1];
	Sv0.vz = G.Wep_hit_pos.vz - pP_ptr->Workm.t[2];
	Esp_call(0x5042710, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	/* retail: the remaining six effects of the Beta version are gone (0x8010221C, 45 words) */
}

static void Em36_spdm(ENEMY_WORK *pEm)
{	SVECTOR Sv; PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x1091 - (Rnd() << 1);
	Sv.vy = 0x934 - (Rnd() << 1);
	Sv.vz = 0x100 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 0x2838) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Esp_call(((Rnd() << 2) + 6000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Sv.vx = 0x1091 - (Rnd() << 1);
	Sv.vy = 0xEAC - (Rnd() << 1);
	Sv.vz = 0x510 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 0x2838) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Sv.vx = 0xD71 - (Rnd() << 1);
	Sv.vy = 0xB28 - (Rnd() << 1);
	Sv.vz = -0x450 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 0x2838) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Esp_call(((Rnd() << 2) + 6000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Sv.vx = 0x6CD - (Rnd() << 1);
	Sv.vy = 0x2F4 - (Rnd() << 1);
	Sv.vz = 0xC4 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 0x2838) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	Sv.vx = 0x6B8 - (Rnd() << 1);
	Sv.vy = 0x2F4 - (Rnd() << 1);
	Sv.vz = 0xC4 - (Rnd() << 1);
	Esp_call(((Rnd() << 2) + 0x1A28) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	Esp_call(((Rnd() << 2) + 4000) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

}

static void Em36_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Sv, Sv0; short Ck; MATRIX M; unsigned long Hit;	/* Hit [INFERRED] */

	/* retail rewrite of the hit-effect block (0x801025BC..0x80102948): the hit point is projected
	   along the player's facing onto the creature's front, and the bomb damage no longer requires
	   Damage_no < 3 */
	if (!pEm->Routine_2) {
		Hit = 0;	/* [INFERRED] hit-in-mouth flag */

		Sv.vx = 0;
		Sv.vy = G.Player.Cdir_y;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 0x1000;
		Sv.vy = 0;
		ApplyMatrixSV(&M, &Sv, &Sv);

		if (Sv.vx < -64 && pEm->Routine_1 != 16 && (pEm->Routine_1 < 9 || pEm->Routine_1 > 12)) {
			Sv0.vz = (Sv.vz * (pEm->Pos_x - -(-G.Player.Pos_x + 3183))) / Sv.vx;
			Sv0.vx = pEm->Pos_x + 3183;
			Sv0.vy = -1400;
			if (!(pEm->Damage_no % 3))
				Sv.vy = -700;
			if (pEm->Damage_no % 3 == 2)
				Sv.vy = -2000;

			Sv0.vz += G.Player.Pos_z;
			if (Sv0.vz > -20500)
				Sv0.vz = -20500;
			if (Sv0.vz < -26000)
				Sv0.vz = -26000;

			if (pEm->Routine_1 == 1)
				Sv0.vz = G.Player.pSin_parts_ptr[11].Workm.t[2];

			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), 0, &GsIDMATRIX, &Sv0);
			Sv0.vx += 500;
			Sv0.vz -= Rnd() - 0x80;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), 0, &GsIDMATRIX, &Sv0);

			if (Sv0.vz > -24000 && Sv0.vz < -22500)
				Hit = 1;
		}

		Ck = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)G.Player.Pos_x, (short)G.Player.Pos_z);

		if (Hit == 1) {
			Sv0.vx = pEm->Atd[1].X + 500;
			Sv0.vy = pEm->Atd[1].Y;
			Sv0.vz = pEm->Atd[1].Z;
			Esp_call((Rnd() << 3) + 0x1FA0, Ck, &GsIDMATRIX, &Sv0);
			Sv.vx += 500;
			Esp_call(((Rnd() << 3) + 0x1FA0) | 0x10000, Ck, &GsIDMATRIX, &Sv0);

			if (pEm->Routine_1 != Hit) {
				Snd_se_enem(8, pEm);
			}
		}

		if (pEm->Routine_1 == 9 || pEm->Routine_1 == 17)
			Em36_bomdm(pEm);

		if (pEm->Routine_1 == 10)
			Em36_firedm(pEm);

		if (pEm->Routine_1 == 11)
			Em36_ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em36_spdm(pEm);

		if (Em36_Dm_cnt[ pEm->Routine_1 - 1 ] >= 11)
			EF->Q_timer = 7;

		if (EF->Etc_flg & 2) {
			*(long *)&pEm->Routine_0 = pEm->Tmp_routine;

			if (pEm->Routine_0 == 1) {
				Em36_Move(pEm, Kan, Seq);
				return;
			}
		}else {

			EF->Dm_cnt += Em36_Dm_cnt[ pEm->Routine_1 - 1 ];

			if (!(EF->Etc_flg & 1)) {
				EF->Etc_flg |= 1;
				EF->Dm_timer0 = 15;
			}

			if (EF->Dm_cnt < 15) {
				*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
				Em36_Move(pEm, Kan, Seq);
				return;
			}
			EF->Dm_cnt = 0;
			EF->Dm_timer0 = 0;
			EF->Etc_flg &= 0xFFFE;
		}
	}

	Em36_dm00(pEm, Kan, Seq);

}

static void Em36_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	EF->Etc_flg |= 2;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0008;
		pEm->Routine_2 = 1;
		Snd_se_enem(12, pEm);

	case 1:
		if (pEm->Move_cnt == 15)
			Snd_se_enem(9, pEm);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(long *)&pEm->Routine_0 = 1;

			EF->Etc_flg &= 0xFFFD;
		}
		Add_speedXZ(pEm, 0);

	}

}

static void Em36_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em36_die00(pEm, Kan, Seq); }

static void Em36_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	PARTS_W *pP_ptr; SVECTOR Sv0, Sv; short Ck; MATRIX M; unsigned long i, j; unsigned char Ok_no[4]; unsigned char Ok_num;
	unsigned long Hit;	/* [INFERRED] hit-in-mouth flag (as Em36_Damage) */
	long k;	/* [INFERRED] spline segment */
	PARTS_W *pQ_ptr, *pR_ptr;	/* [INFERRED] */

	/* retail rewrite (0x80102BDC, 790 words): the hit block of Em36_Damage, then a six-state death:
	   1 effects along a 4-point spline (Em36_die_pos_tbl), 2 struggle for 250 frames, 3/4 sink while
	   the body darkens and the mouth closes, 5 done (0x80102C34..0x80103808) */
	switch (pEm->Routine_2) {
	case 0:
		Hit = 0;

		Sv.vx = 0;
		Sv.vy = G.Player.Cdir_y;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 0x1000;
		Sv.vy = 0;
		ApplyMatrixSV(&M, &Sv, &Sv);

		if (Sv.vx < -64 && pEm->Routine_1 != 16 && (pEm->Routine_1 < 9 || pEm->Routine_1 > 12)) {
			Sv0.vz = (Sv.vz * (pEm->Pos_x - -(-G.Player.Pos_x + 3183))) / Sv.vx;
			Sv0.vx = pEm->Pos_x + 3183;
			Sv0.vy = -1400;

			if (!(pEm->Damage_no % 3))
				Sv.vy = -700;
			if (pEm->Damage_no % 3 == 2)
				Sv.vy = -2000;

			Sv0.vz += G.Player.Pos_z;
			if (Sv0.vz > -20500)
				Sv0.vz = -20500;
			if (Sv0.vz < -26000)
				Sv0.vz = -26000;

			if (pEm->Routine_1 == 1)
				Sv0.vz = G.Player.pSin_parts_ptr[11].Workm.t[2];

			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), 0, &GsIDMATRIX, &Sv0);
			Sv0.vx += 500;
			Sv0.vz -= Rnd() - 0x80;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), 0, &GsIDMATRIX, &Sv0);

			if (Sv0.vz > -24000 && Sv0.vz < -22500)
				Hit = 1;
		}

		Ck = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)G.Player.Pos_x, (short)G.Player.Pos_z);

		if (Hit == 1) {
			Sv0.vx = pEm->Atd[1].X + 500;
			Sv0.vy = pEm->Atd[1].Y;
			Sv0.vz = pEm->Atd[1].Z;
			Esp_call((Rnd() << 3) + 0x1FA0, Ck, &GsIDMATRIX, &Sv0);
			Sv.vx += 500;
			Esp_call(((Rnd() << 3) + 0x1FA0) | 0x10000, Ck, &GsIDMATRIX, &Sv0);

			if (pEm->Routine_1 != Hit) {
				Snd_se_enem(8, pEm);
			}
		}

		if (pEm->Routine_1 == 9 || pEm->Routine_1 == 17)
			Em36_bomdm(pEm);

		if (pEm->Routine_1 == 10)
			Em36_firedm(pEm);

		if (pEm->Routine_1 == 11)
			Em36_ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em36_spdm(pEm);

		EF->Q_timer = 240;

		if (EF->Etc_flg & 2) {
			*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
			EF->Etc_flg |= 4;
			pEm->Life = 0;

			if (pEm->Routine_0 == 1) {
				Em36_Move(pEm, Kan, Seq);
			}else {
				Em36_dm00(pEm, Kan, Seq);
			}
			break;
		}

		pEm->Life = -1;
		pEm->Sce_free0 |= 2;

		*(long *)&pEm->Move_no = 0x1F0006;
		pEm->Routine_2 = 1;

		Snd_se_enem(10, pEm);

		pEm->Damage_cnt |= 0x80;

		pP_ptr = pEm->pSin_parts_ptr;
		i = 7;
		do {
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, 0);
			pP_ptr++;

		}while (--i);

		EF->_R0 = 5;
		EF->_R1 = 0;
		pEm->Timer3 = 0;
		pEm->Neck_flg |= 2;
		Snd_se_enem(13, pEm);
		Pad_vib_set1(250, 180, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_fade_set(150, 180, 0, 250);
		Pad_vib_fade_set(10, 180, 220, 20);
		Pad_vib_fade_set(10, 220, 180, 30);
		Pad_vib_fade_set(10, 180, 220, 40);
		Pad_vib_fade_set(10, 220, 180, 50);
		Pad_vib_fade_set(10, 180, 220, 65);
		Pad_vib_fade_set(10, 220, 180, 75);
		Pad_vib_fade_set(10, 180, 220, 230);
		Pad_vib_fade_set(10, 220, 180, 240);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0x1F0007;
			pEm->Timer0 = 250;		}

		k = (signed char)pEm->Timer3 >> 4;
		if (k < 3) {
			Sv0.vx = Em36_die_pos_tbl[ k ][0];
			Sv0.vy = Em36_die_pos_tbl[ k ][1];
			Sv0.vz = Em36_die_pos_tbl[ k ][2];
			Sv.vx = Em36_die_pos_tbl[ k + 1 ][0] - Em36_die_pos_tbl[ k ][0];
			Sv.vy = Em36_die_pos_tbl[ k + 1 ][1] - Em36_die_pos_tbl[ k ][1];
			Sv.vz = Em36_die_pos_tbl[ k + 1 ][2] - Em36_die_pos_tbl[ k ][2];
			Sv.vx = (Sv.vx / 16) * ((signed char)pEm->Timer3 & 0xF);
			Sv.vy = (Sv.vy / 16) * ((signed char)pEm->Timer3 & 0xF);
			Sv.vz = (Sv.vz / 16) * ((signed char)pEm->Timer3 & 0xF);
			Sv0.vx += Sv.vx;
			Sv0.vy += Sv.vy;
			Sv0.vz += Sv.vz;

			pP_ptr = pEm->pSin_parts_ptr + 2;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);			pEm->Timer3++;
		}

		if (pEm->Move_cnt == 60)
			pEm->Timer3 = 0;

		if (!(pEm->Move_cnt & 3)) {
			i = 4;
			j = 0;
			Ok_num = 0;
			do {

				if (!((EF->Syoku_flg >> j) & 1)) {

					if (!((EF->Syoku_flg >> (j + 4)) & 1)) {
						Ok_no[ Ok_num ] = j;
						Ok_num++;
					}
				}

				j++;	}while (--i);

			if (Ok_num) {
				Em36_set_em37_1set(Ok_no[ Rnd() % Ok_num ], 0xE01);
				Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
				Pad_vib_set1(15, 250, 0);
			}
		}
		break;

	case 2:
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(long *)&pEm->Move_no = 0x1F0006;
			if (!(Rnd() & 3))
				*(long *)&pEm->Move_no = 0x1F0008;
		}

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 3;
			EF->Etc_flg |= 8;
			Em36_core_set_dest(0, 0, 0);
			Em36_core_set_dest(1, 0, 0);
			pQ_ptr = pEm->pSin_parts_ptr;
			pQ_ptr[2].Size_x = 0x1000;
			pQ_ptr[2].Size_y = 0x1000;
			pQ_ptr[2].Size_z = 0x1000;
			pQ_ptr[2].Be_flg |= 0x400;
			*(long *)&pEm->Move_no = 0x1F000A;
			Pad_vib_set1(5, 200, 0);		/* retail-only Dual Shock vibration */
			Pad_vib_fade_set(200, 200, 0, 5);
			Pad_vib_set0(3, 0);
			Pad_vib_set0(3, 5);
			Pad_vib_set0(2, 15);
			Pad_vib_set0(3, 60);
			Pad_vib_set0(3, 65);
			Pad_vib_set0(2, 75);
			Pad_vib_set0(3, 90);
			Pad_vib_set0(3, 95);
			Pad_vib_set0(2, 105);
			Pad_vib_set0(2, 110);
			Pad_vib_set0(2, 115);
			Pad_vib_set0(1, 120);
		}

		if (!(pEm->Move_cnt & 3)) {
			i = 4;
			j = 0;
			Ok_num = 0;
			do {

				if (!((EF->Syoku_flg >> j) & 1)) {

					if (!((EF->Syoku_flg >> (j + 4)) & 1)) {
						Ok_no[ Ok_num ] = j;
						Ok_num++;
					}
				}

				j++;	}while (--i);

			if (Ok_num) {
				Em36_set_em37_1set(Ok_no[ Rnd() % Ok_num ], 0xE01);
				Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
				Pad_vib_set1(15, 250, 0);
			}
		}
		break;

	case 3:
		if (!(pEm->Timer0 & 1))
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);

	case 4:
		if (++pEm->Timer0 & 1) {
			pP_ptr = pEm->pSin_parts_ptr;
			if ((pP_ptr->Poly_rgb & 0xFF) != 0x30)
				pP_ptr->Poly_rgb--;
			if ((pP_ptr->Poly_rgb & 0xFF00) != 0x3000)
				pP_ptr->Poly_rgb -= 0x100;
			if ((pP_ptr->Poly_rgb & 0xFF0000) != 0x400000)
				pP_ptr->Poly_rgb -= 0x10000;

			pR_ptr = pEm->pSin_parts_ptr;
			if ((pR_ptr[2].Poly_rgb & 0xFF) != 0x30)
				pR_ptr[2].Poly_rgb--;
			if ((pR_ptr[2].Poly_rgb & 0xFF00) != 0x3000)
				pR_ptr[2].Poly_rgb -= 0x100;
			if ((pR_ptr[2].Poly_rgb & 0xFF0000) != 0x400000)
				pR_ptr[2].Poly_rgb -= 0x10000;
		}

		pP_ptr = pEm->pSin_parts_ptr + 2;
		Ck = 16;	/* [INFERRED reuse] Ck (dead here) as the morph step */
		if (((short *)pP_ptr->pSa_dat_head)[10] > 0xC00)
			Ck = 4;
		else if (((short *)pP_ptr->pSa_dat_head)[10] > 0x800)
			Ck = 8;
		else if (((short *)pP_ptr->pSa_dat_head)[10] > 0x400)
			Ck = 12;

		((short *)pP_ptr->pSa_dat_head)[10] += Ck;
		if (((short *)pP_ptr->pSa_dat_head)[10] > 0x1000)
			pEm->Routine_2 = 5;

		pP_ptr->Pos_y = 4500 - (((short *)pP_ptr->pSa_dat_head)[10] * 2950) / 4096;
		pEm->Pos_y = 4500 - pP_ptr->Pos_y;
		break;

	case 5:
		break;
	}
}

static void Em36_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em36_sce[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em36_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em36_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em36_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em36_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP_ptr;	/* [INFERRED] unused declaration: no SYM record, opens the empty block */

}

void EM_PLDIE(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em36_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em36_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Sv0;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70000;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		Snd_se_stad(0x4010001, &pEm->Pos_x);

	case 1:
		pEm->Pos_x += (pEm->Base_pos_x - pEm->Pos_x) >> 1;
		pEm->Pos_z += (pEm->Base_pos_z - pEm->Pos_z) >> 1;

		Goto01(pEm, pEm->Timer1, 0x100);

		Joint_move(pEm, Kan, Seq, 0x200);
		if (pEm->Move_cnt == 8) {
			pEm->Routine_2 = 2;
			pEm->Cdir_y = pEm->Timer1;
			Snd_se_stad(0x4010001, &pEm->Pos_x);
		}
		break;

	case 2:

		G.Player.Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		G.Player.Cdir_y -= 0x800;

		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 3;

	}
}

static unsigned long G5a_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top)
{

	Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, G5a_jh0_head, Mem_top);
	return Jh_set(pEm, pEm->pSin_parts_ptr, G5a_jh1_head, Mem_top); }

static void G5a_Jh_move(ENEMY_WORK *pEm)
{	PARTS_W *pP_ptr;
	MATRIX Wm;	/* [INFERRED] name; unused (user ruling: unused source locals allowed when the object passes): retail's frame is 0x20 larger than the Beta body needs and no instruction touches the extra 0x20 bytes (both slots, 0x80103AF4 / 0x80110AF4) */

	pP_ptr = pEm->pSin_parts_ptr;
	/* retail: the body and first-row part matrices are rebuilt here (0x80103B18..0x80103B5C) */
	RotMatrix(&pEm->Cdir_x, pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
	CompM(&pP_ptr->Workm, (MATRIX *)pP_ptr[1].M, &pP_ptr[1].Workm);
	CompM(&pP_ptr->Workm, (MATRIX *)pP_ptr[2].M, &pP_ptr[2].Workm);
	CompM(&pP_ptr->Workm, (MATRIX *)pP_ptr[3].M, &pP_ptr[3].Workm);
	MulJh(pEm, pP_ptr, G5a_jh0_head);
	MulJh(pEm, pP_ptr, G5a_jh1_head);
	Jh_sort(pEm, pP_ptr);

}

static void Em37_K(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	Em37_K_tbl[ EF->_R0 ](pEm, pP_ptr);

	Sa_morph_trans(pEm, pP_ptr, pP_ptr->pTmd, pP_ptr->pSa_dat_head);

}

static void Em37_K_mv00(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	switch (EF->_R1) {
	case 0:

		EF->_R1 = 1;
		EF->T0 = (Rnd() & 0x1F) + 180;
		EF->T2 = 6;
		EF->T1 = 0;

	case 1:
		if (((short *)pP_ptr->pSa_dat_head)[12])
			((short *)pP_ptr->pSa_dat_head)[12] >>= 1;

		((short *)pP_ptr->pSa_dat_head)[6] += EF->T2;

		if (EF->T2 < 162 && !EF_T1S) {
			EF->T2 += 6;
			if (EF->T2 >= 162) {
				EF->T1 = 1;
				EF->T2 = 162;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[6] > 8000) {
			EF->T2 -= 6;
			if (EF->T2 < 0) {
				EF->_R1 = 2;

				EF->T2 = 8;
				EF->T1 = 0;
			}
		}

		break;

	case 2:

		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T2;

		if (EF->T2 < 192 && !EF_T1S) {
			EF->T2 += 8;
			if (EF->T2 >= 192) {
				EF->T1 = 1;
				EF->T2 = 192;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[6] < -548) {
			EF->T2 -= 8;
			if (EF->T2 < 0) {
				EF->_R1 = 1;

				EF->T2 = 6;
				EF->T1 = 0;
			}
		}
		break;

	default:
		return;
	}

	/* retail: the bite check runs after both swim phases, with a facing test and the
	   Etc_flg bit 2 guard, and no longer drains life first (0x80103E00..0x80103EE0) */
	if (pEm->L_pl < 6000 && !EF->Kuchi_at_timer && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80)
	 && !G.Player.Damage_cnt && !(EF->Etc_flg & 4)) {

		EF->_R0 = 1;
		EF->_R1 = 0;

		if (G.Player.Life <= 0 && pEm->Life >= 0) {

			*(long *)&pEm->Routine_0 = 0x401;
			Pl_life_down(500, 0);
			G.Player.Damage_cnt |= 0x80;
			EF->_R0 = 4;
			EF->_R1 = 0;
			G.System_flg |= 0x100;
		}
	}

}

static void Em37_K_mv01(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{	SVECTOR Sv0;

	/* retail rewrite (0x80103EFC, 337 words): faster pull-back, a grab check that also catches a
	   player already being bitten (Routine 5/<2), a shaking state 3 and the old release as state 4 */
	switch (EF->_R1) {
	case 0:
		EF->_R1 = 1;
		EF->T0 = 0;

		EF->T2 = 0;

		EF->T1 = 0;

	case 1:
		if (((short *)pP_ptr->pSa_dat_head)[12])
			((short *)pP_ptr->pSa_dat_head)[12] >>= 1;

		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] < -4048)
			EF->_R1 = 2;

		((short *)pP_ptr->pSa_dat_head)[12] += EF->T0 >> 1;

		EF->T0 += 32;
		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[6] += EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 0)
			((short *)pP_ptr->pSa_dat_head)[6] = 0;

		((short *)pP_ptr->pSa_dat_head)[12] += EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[12] > 5572) {
			EF->_R1 = 3;
			EF->T2 = 15;

			Snd_se_enem(6, pEm);

		}

		pEm->Pos_x += EF->T0 >> 3;
		if (pEm->Pos_x > 12000)
			pEm->Pos_x -= EF->T0 >> 3;

		if ((EF->T0 >> 3) > 10)
			EF->Q_timer = 4;

		pEm->Atd[1].Ofs_x += 500;
		if (pEm->Atd[1].Ofs_x > 3700)
			pEm->Atd[1].Ofs_x = 3700;

		pEm->Atd[1].X += 900;

		if (L_pos_ckXZ(&pEm->Atd[1], &G.Player.Pos_x, 1500)
		 && (!G.Player.Damage_cnt
		   || (!EF_T1S && G.Player.Life >= 0 && G.Player.Routine_0 == 5 && G.Player.Routine_1 < 2))) {

			Pl_life_down(40, 1);
			*(long *)&G.Player.Routine_0 = 0x202;
			G.Player.Damage_cnt |= 0x80;
			G.Player.pEnemy_ptr = (unsigned long *)pEm;

			EF->T1 = 10;
			Snd_se_enem(14, pEm);
			Pad_vib_set0(8, 0);		/* retail-only Dual Shock vibration */
			Pad_vib_set1(10, 250, 4);
		}

		pEm->Atd[1].X -= 900;

		EF->T0 += 0x100;

		break;

	case 3:
		if ((unsigned short)EF->T2-- == 0)
			EF->_R1 = 4;
		((short *)pP_ptr->pSa_dat_head)[12] += 0x40 - ((EF->T2 & 1) << 8);		break;

	case 4:
		((short *)pP_ptr->pSa_dat_head)[12] -= 64;
		pEm->Atd[1].X += 900;
		if (L_pos_ckXZ(&pEm->Atd[1], &G.Player.Pos_x, 3000))
			((short *)pP_ptr->pSa_dat_head)[12] -= 32;
		pEm->Atd[1].X -= 900;

		if (((short *)pP_ptr->pSa_dat_head)[12] < 0) {
			((short *)pP_ptr->pSa_dat_head)[12] = 0;
			EF->_R0 = 0;
			EF->_R1 = 0;

			EF->Kuchi_at_timer = 5;
		}

		pEm->Atd[1].Ofs_x -= 15;
		if (pEm->Atd[1].Ofs_x < 2200)
			pEm->Atd[1].Ofs_x = 2200;

	}

	if (EF_T1S) {
		EF_T1S--;

		pP_ptr = pEm->pSin_parts_ptr + 4;
		Sv0.vx = 1200;
		Sv0.vy = (Rnd() << 1) + 0x38;
		Sv0.vz = Rnd() + 0x80;
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x2770), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		Sv0.vx = 1400;
		Sv0.vy = (Rnd() << 1) + 0x38;
		Sv0.vz = Rnd() + 0x80;
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x2770), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		pP_ptr = G.Player.pSin_parts_ptr;
		Sv0.vx = 0x100 - (Rnd() << 1);
		Sv0.vy = 0;
		Sv0.vz = 0x100 - (Rnd() << 1);
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x17D0), 0, &pP_ptr->Workm, &Sv0);
	}

}

static void Em37_K_mv02(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{	register EM36_FREE *pFp;	/* inferred name: gcc folds it into every use (no retail SYM record) */

	switch ((pFp = EF)->_R1) {
	case 0:
		EF->_R1 = 1;
		EF->T0 = 36;

		EF->T1 = 10;
		EF->T3 = 10;

		EF->T2 = 1;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[8] += EF->T0;

		EF->T0 -= EF->T2 * 4;	/* multiply (not shift) keeps the sign-extending lh of T2 */

		if ((unsigned char)EF->T1-- != 0) break;	/* [INFERRED] cast: retail tests the pre-decrement value with lbu */
		EF->T1 = EF->T3;
		EF->T3--;
		EF->T2 *= -1;	/* *= -1 (not unary minus) keeps the lh of T2 */

		if (!EF_T1S) {
			EF->_R0 = 0;
			EF->_R1 = 0;
		}
	}

}

static void Em37_K_mv03(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	switch (EF->_R1) {
	case 0:
		EF->_R1 = 1;
		EF->T0 = (Rnd() & 0x1F) + 30;

		EF->T2 = 6;
		EF->T1 = 0;

	case 1:
		if (((short *)pP_ptr->pSa_dat_head)[12])
			((short *)pP_ptr->pSa_dat_head)[12] >>= 1;

		((short *)pP_ptr->pSa_dat_head)[6] += EF->T2;

		if (EF->T2 < 162 && !EF_T1S) {
			EF->T2 += 6;
			if (EF->T2 >= 162) {
				EF->T1 = 1;
				EF->T2 = 162;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[6] > 8500) {
			EF->T2 -= 6;
			if (EF->T2 < 0) {
				EF->_R1 = 2;

				EF->T2 = 8;
				EF->T1 = 0;
			}
		}

		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T2;

		if (EF->T2 < 192 && !EF_T1S) {
			EF->T2 += 8;
			if (EF->T2 >= 192) {
				EF->T1 = 1;
				EF->T2 = 192;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[6] < -1048) {
			EF->T2 -= 8;
			if (EF->T2 < 0) {
				EF->_R1 = 1;

				EF->T2 = 6;
				EF->T1 = 0;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[6] > 0 && ((short *)pP_ptr->pSa_dat_head)[6] < 2048)

			EF->_R1 = 6;

		if (EF->T0)
			EF->T0--;

		break;

	case 3:
		EF->T1 = (Rnd() & 3) + 7;
		EF->_R1 = 4;

		EF->T2 = 0;

	case 4:
		((short *)pP_ptr->pSa_dat_head)[6] -= 2136;

		if ((unsigned short)EF->T2-- != 0) break;	/* [INFERRED] cast: pre-decrement test */
		EF->T2 = 2;

		EF->_R1 = 5;	break;

	case 5:
		((short *)pP_ptr->pSa_dat_head)[6] += 712;

		if ((unsigned short)EF->T2-- != 0) break;
		EF->_R1 = 4;
		EF->T2 = 0;

		if ((unsigned char)EF->T1-- != 0) break;	/* [INFERRED] cast: pre-decrement test */

		EF->_R1 = 0;	break;

	case 6:
		EF->T3 = 0;
		EF->_R1 = 7;

	case 7:
		((short *)pP_ptr->pSa_dat_head)[6] = 8000 - Em37_K_mv03_tbl[ *(signed char *)&EF->T3 ] * 24;

		EF->T3++;
		if ((signed char)EF->T3 == 110)
			EF->_R1 = 6;

	}	}

static void Em37_K_mv04(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{	SVECTOR Sv0;

	switch (EF->_R1) {
	case 0:
		EF->_R1 = 1;
		EF->T0 = 0;

		EF->T2 = 0;

		EF->T1 = 0;
		Pad_vib_set0(30, 0);		/* retail-only Dual Shock vibration (0x801048C4) */
		Pad_vib_set1(35, 250, 15);

	case 1:
		if (((short *)pP_ptr->pSa_dat_head)[12]) {
			((short *)pP_ptr->pSa_dat_head)[12] >>= 1;
			EF->T0 = 0x400;
		}

		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] < -4048)
			EF->_R1 = 2;

		EF->T0 += 32;

		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[6] += EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 10000) {
			EF->_R1 = 3;
			EF->T1 = (Rnd() & 0x3F) + 64;

			EF->T0 = 0;
			EF->T2 = Rnd() + (Rnd() << 1) + (Rnd() << 2) + (Rnd() << 3) + (Rnd() << 4);

			Snd_se_enem((Rnd() & 1) ? 6 : 14, pEm);	/* retail: random cry (0x80104A0C) */

			pP_ptr = pEm->pSin_parts_ptr + 4;
			Sv0.vx = 0;
			Sv0.vy = 0x100 - (Rnd() << 1);
			Sv0.vz = 0x100 - (Rnd() << 1);
			Esp_call((Rnd() << 2) + 0x1FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			Sv0.vx = 0;
			Sv0.vy = 0x100 - (Rnd() << 1);
			Sv0.vz = 0x100 - (Rnd() << 1);
			Esp_call(((Rnd() << 2) + 0x1FA0) | 0x10000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
			Pad_vib_set0((Rnd() & 3) + 2, Rnd() & 3);		/* retail-only Dual Shock vibration */
			Pad_vib_set1((Rnd() & 3) + 3, 250, Rnd() & 3);
		}

		break;

	case 3:
		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] < EF->T2) {
			EF->_R1 = 2;
			EF->T0 = (Rnd() << 2) + 0x800;
		}
		EF->T0 += EF_T1S;

	}
}
/* retail-only (no Beta twin, name unknown): Em37_K_tbl[5] (retail .data 0x80105AD0); a jaw sequence
 * (retract, shake with effects, open, close), retail 0x80104B94 / 0x80111B94. */
static void func_80104B94(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	register unsigned long Ef;	/* [INFERRED] effect id base */
	PARTS_W *pQ_ptr;	/* [INFERRED] */
	SVECTOR Sv0;

	switch (EF->_R1) {
	case 0:
		EF->_R1 = 1;
		EF->T0 = 0;
		EF->T2 = 0;
		EF->T1 = 0;

	case 1:
		if (((short *)pP_ptr->pSa_dat_head)[12])
			((short *)pP_ptr->pSa_dat_head)[12] >>= 1;

		((short *)pP_ptr->pSa_dat_head)[6] -= EF->T0;
		if (((short *)pP_ptr->pSa_dat_head)[6] < -4048)
			EF->_R1 = 2;
		EF->T0 += 32;
		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[6] += 32 + (Rnd() << 2) * (1 - ((EF->T2 & 1) << 1));
		if (((short *)pP_ptr->pSa_dat_head)[6] > 8000)
			((short *)pP_ptr->pSa_dat_head)[6] = 8000;
		if (((short *)pP_ptr->pSa_dat_head)[6] < -5000)
			((short *)pP_ptr->pSa_dat_head)[6] = -5000;

		if (++EF->T2 > 300)
			EF->_R1 = 3;

		Ef = 0x1FA0;
		if (EF->T2 > 150)
			Ef = 0x17D0;
		if (EF->T2 > 250)
			Ef = 0xFD0;

		if (EF->T2 & 1) {
			pQ_ptr = pEm->pSin_parts_ptr;
			Sv0.vx = 0x80 - Rnd();
			Sv0.vy = 0x80 - Rnd();
			Sv0.vz = 0x80 - Rnd();
			Esp_call((((Rnd() & 1) + (Rnd() & 0x10)) << 16) | (Ef + (Rnd() << 2)), pEm->Cdir_y, &pQ_ptr[4].Workm, &Sv0);
		}
		break;

	case 3:
		((short *)pP_ptr->pSa_dat_head)[6] += 64;
		if (((short *)pP_ptr->pSa_dat_head)[6] > 12000)
			EF->_R1 = 4;
		break;

	case 4:
		((short *)pP_ptr->pSa_dat_head)[6] -= 128;
		if (((short *)pP_ptr->pSa_dat_head)[6] < 0)
			EF->_R1 = 5;
		break;

	case 5:
		break;
	}
}

static void Em36_set_em37_all(unsigned long R)
{
	ENEMY_WORK *pKem;

	pKem = G.pEnemy[3];
	*(long *)&pKem->Routine_0 = R;
	pKem = G.pEnemy[4];
	*(long *)&pKem->Routine_0 = R;
	pKem = G.pEnemy[5];
	*(long *)&pKem->Routine_0 = R;
	pKem = G.pEnemy[6];
	*(long *)&pKem->Routine_0 = R;

}

static void Em36_set_em37_1set(unsigned long No, unsigned long R)
{
	ENEMY_WORK *pKem;

	pKem = G.pEnemy[ No + 3 ];
	*(long *)&pKem->Routine_0 = R;

}

static int Em36_mv_quake(ENEMY_WORK *pEm)
{

	EF->Q0_t &= 0xFE;
	EF->Q1_t &= 0xFE;

	if (EF->Q_timer) {
		EF->Q_timer--;
		EF->Q0_t |= 1;
		Pad_vib_set1(3, 100, 0);	/* retail-only Dual Shock vibration (0x80104F04) */
		Pad_vib_fade_set(20, 100, 0, 3);
	}

	if (pEm->Spd_x > 120) {
		EF->Q1_t |= 1;
		Pad_vib_set1(3, 150, 0);
		Pad_vib_fade_set(30, 150, 0, 3);
	}

	if (pEm->Spd_x > 20) {
		EF->Q0_t |= 1;
		Pad_vib_set1(3, 100, 0);
		Pad_vib_fade_set(20, 100, 0, 3);
	}

	if (!EF->Q0_t) {
		G.Status_flg &= ~8;
	}else {
		G.Status_flg |= 8;
	}

	if (!EF->Q1_t) {
		G.Status_flg &= ~4;
	}else {
		G.Status_flg |= 4;
	}	}

static void Em36_core_set_dest(unsigned char No, short D_x, short D_y)
{
	MEDAMA *pMe;

	pMe = &Em36_Me[ No ];

	pMe->Tate_d = D_x;
	pMe->Yoko_d = D_y;

}

static void Em36_core_me_mv(ENEMY_WORK *pEm, unsigned char No)
{	PARTS_W *pP_ptr; short Tmp_t, Tmp_y; MEDAMA *pMe;

	pMe = &Em36_Me[ No ];

	pP_ptr = pEm->pSin_parts_ptr + pMe->Parts_no;

	Tmp_t = pMe->Tate_a;
	Tmp_y = pMe->Yoko_a;

	if ((signed char)pMe->Tate > (signed char)pMe->Tate_d)
		pMe->Tate_a = -1;

	if ((signed char)pMe->Tate < (signed char)pMe->Tate_d)
		pMe->Tate_a = 1;

	if ((signed char)pMe->Yoko > (signed char)pMe->Yoko_d)
		pMe->Yoko_a = -1;

	if ((signed char)pMe->Yoko < (signed char)pMe->Yoko_d)
		pMe->Yoko_a = 1;

	if (-pMe->Tate_a == Tmp_t)
		pMe->Tate_mv = pMe->Tate_mv * 2 / 3;

	if (-pMe->Yoko_a == Tmp_y)
		pMe->Yoko_mv = pMe->Yoko_mv * 2 / 3;

	if ((signed char)pMe->Tate == (signed char)pMe->Tate_d && __builtin_abs(pMe->Tate_mv) < 3) {
		pMe->Tate = pMe->Tate_d;
		pMe->Tate_a = 0;
		pMe->Tate_mv = 0;
	}
	if ((signed char)pMe->Yoko == (signed char)pMe->Yoko_d && __builtin_abs(pMe->Yoko_mv) < 3) {
		pMe->Yoko = pMe->Yoko_d;
		pMe->Yoko_a = 0;
		pMe->Yoko_mv = 0;
	}

	pMe->Tate_mv += pMe->Tate_a;
	pMe->Yoko_mv += pMe->Yoko_a;

	Tmp_t = (signed char)pMe->Tate;
	Tmp_y = (signed char)pMe->Yoko;

	pMe->Tate += pMe->Tate_mv;
	pMe->Yoko += pMe->Yoko_mv;

	if ((signed char)pMe->T_max < (signed char)pMe->Tate)
		pMe->Tate = pMe->T_max;

	if ((signed char)pMe->Tate < (signed char)pMe->T_min)
		pMe->Tate = pMe->T_min;

	if ((signed char)pMe->Y_max < (signed char)pMe->Yoko)
		pMe->Yoko = pMe->Y_max;

	if ((signed char)pMe->Yoko < (signed char)pMe->Y_min)
		pMe->Yoko = pMe->Y_min;

	Tmp_t = (signed char)pMe->Tate - Tmp_t;
	Tmp_y = (signed char)pMe->Yoko - Tmp_y;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 3, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, pMe->pMe3);	/* [INFERRED] packed offset and first-argument spelling; the (unsigned short) cast reproduces retail's Tmp_t/Tmp_y register homes */

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 3, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, pMe->pMe4);

}

static void Em36_die_wait(ENEMY_WORK *pEm)
{	PARTS_W *pP_ptr; SVECTOR Sv0;

	/* retail: one random offset per frame and fixed parts 0/4/5 (every 8th frame) and 6/1 (frame 4)
	   instead of the Beta loop over parts 0..6 (0x801053B4..0x8010554C) */
	if ((EF->Etc_flg & 6) == 6) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();

		if (!(pEm->Move_cnt & 7)) {
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr[0].Workm, &Sv0);
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr[4].Workm, &Sv0);
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr[5].Workm, &Sv0);
		}

		if ((pEm->Move_cnt & 7) == 4) {
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr[6].Workm, &Sv0);
			pP_ptr = pEm->pSin_parts_ptr;
			Sv0.vx += 1200;
			Sv0.vy += 400;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1FA0), pEm->Cdir_y, &pP_ptr[1].Workm, &Sv0);
		}
	}
}

