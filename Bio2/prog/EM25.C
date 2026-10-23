/* em25.c - reconstruction of D:\Bio2\prog\EM25.C (enemy 25: licker AI - floor,
 * ceiling and wall movement).
 * Linked into two overlays from the same source: 0x0A (at 0x801000e8) and 0x28
 * (at 0x8010d0e8); only the enemy-number-specific entry name differs
 * (Em125 / Em225), selected by OVL_28.
 * Retail EM25.C and EM25_S0.C form ONE object (one SYM segment, shared static
 * tables): em25_s0.c is #included at the end of this file, after the last EM25.C
 * function.
 * Types come from the TU's retail SYM preamble (em25.h). */
#define EM25_C	/* em25_s0.c is #included below: one object, two SLD source files */
#include "EM25.H"

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of this translation unit ---- */

/* ---- functions of D:\Bio2\prog\EM25_S0.C (#included at the end of this file) ---- */

/* ---- functions of other translation units ---- */

/* ---- data (EM25.C) ---- */
/* Retail life tables (em25_cdemd0_s1 .data 0x801062C0..0x8010639F): Em25_life_easy and the
 * Em2b pair changed relative to Beta 2; a second complete set (0x80106330..) is selected by
 * G.System_flg & 0x40 (Em25_Init @0x8010022C). Second-set names are [INFERRED], following
 * src-retail/prog/em20.c. */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em25_life[16] = { 0x63, 0x63, 0x63, 0x63, 0x77, 0x63, 0x63, 0x77, 0x63, 0x63, 0x63, 0x77, 0x63, 0x59, 0x63, 0x63 };
static unsigned short Em25_life_easy[16] = { 0x4F, 0x59, 0x63, 0x4F, 0x31, 0x59, 0x4F, 0x45, 0x59, 0x45, 0x59, 0x59, 0x63, 0x45, 0x31, 0x4F };
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;
static unsigned short Em2b_life_easy = 0xC8;
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em25_life_flg40[16] = { 0x6D, 0x6D, 0x6D, 0x6D, 0x81, 0x6D, 0x6D, 0x81, 0x6D, 0x6D, 0x6D, 0x81, 0x6D, 0x63, 0x6D, 0x6D };
static unsigned short Em25_life_easy_flg40[16] = { 0x4F, 0x59, 0x63, 0x4F, 0x31, 0x59, 0x4F, 0x45, 0x59, 0x45, 0x59, 0x59, 0x63, 0x45, 0x31, 0x4F };
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static AT_DATA Em25_hit_tbl[4] = {	/* retail .data 0x801063A0: [0] Ofs_y/At_w/At_d/At_h and [1] At_w/At_d changed */
	{ 0, 0, 0, 800, 800, 0, 0, -1400, 1000, 1000, 1400, 0, 0 },
	{ 0, 0, 0, 30, 30, 0, 0, -100, 1000, 1000, 1400, 0, 0 },
	{ 0, 0, 0, 30, 30, 0, 0, 0, 1000, 1000, 1400, 0, 0 },
	{ 0, 0, 0, 5, 5, 0, 0, -1, 5, 5, 1, 0, 0 }
};
static void (*Em25_routine_0[8])() = {
	Em25_Init,
	Em25_Move,
	Em25_Damage,
	Em25_Die,
	Em25_Scenario,
	0,
	0,
	Em25_Dead
};
static void (*Em25_move_tbl[4])() = {
	Em25_Move_floor,
	Em25_Move_ceiling,
	Em25_Move_walldown,
	Em25_Move_wallset
};
static void (*Em25_floor_tbl[10])() = {
	Em25_mv000,
	Em25_mv01,
	Em25_mv01,
	Em25_mv01,
	Em25_mv04,
	Em25_mv05,
	Em25_mv05,
	Em25_mv07,
	Em25_mv08,
	Em25_mv09
};
/* retail: Em25_mv01's local table became file-scope data (D_80106478, between the floor and
   ceiling routine tables; lw/lh/lw %lo(D_80106478/7C/80) @0x8010092C, 0x80100974, 0x801009DC) */
static unsigned long Em25_mv01_tbl[3][3] = { { 2, 0x30, 0x64 }, { 3, 0x10, 0x3C }, { 3, 0x10, 0x14 } };
static void (*Em25_ceiling_tbl[12])() = {	/* retail: 12 entries (D_8010649C), + Em25_mv0b */
	Em25_mv001,
	Em25_mv011,
	Em25_mv011,
	Em25_mv011,
	Em25_mv04,
	Em25_mv05,
	Em25_mv05,
	Em25_mv071,
	Em25_mv08,
	Em25_mv09,
	Em25_mv0a,
	Em25_mv0b
};
static void (*move_wall_tbl[8])() = {
	Em25_Mv_wall000,
	Em25_Mv_wall10,
	Em25_Mv_wall200,
	Em25_Mv_wall30,
	Em25_Mv_wall40,
	Em25_Mv_wall50,
	Em25_Mv_wall60,
	Em25_Mv_wall70
};
static short ceil_turn_tbl[8] = { 64, -64, 64, -64, 32, -64, 64, -32 };
static void (*move_wallset_tbl[8])() = {
	Em25_Mv_wall001,
	Em25_Mv_wall10,
	Em25_Mv_wall201,
	Em25_Mv_wall30,
	Em25_Mv_wall40,
	Em25_Mv_wall50,
	Em25_Mv_wall60,
	Em25_Mv_wall70
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	long ground;	/* [INFERRED] name: Water_ck + 70 surface height (retail-only branch) */

	if (!(G.Stop_flg & 0x20000000)) {
		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (EF->No_fire)
			EF->No_fire--;

		Em25_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		pEm->nFloor = -pEm->Pos_y / 1800;

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x600);	/* retail: 0x400 -> 0x600, li a1 @0x8010017C */

		Em25_kage_move(pEm);
		Em25_spark_move(pEm);
	}
	else if (EF->Retail_27) {
		/* retail-only (0x80100198..0x801001D4): while stopped and Free byte +0x27 is set,
		   Pos_y = min( Water_ck( Pos_x, Pos_z ) + 70, Ground ) */
		ground = Water_ck(pEm->Pos_x, pEm->Pos_z) + 70;
		if (ground < pEm->Ground)
			pEm->Pos_y = ground;
		else
			pEm->Pos_y = pEm->Ground;
	}
}

static void Em25_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register EM25_FREE *pFp;
	register unsigned long i;
	register PARTS_W *pP_ptr;	/* inferred name: no retail record (gcc folds it into the loaded base) */
	pEm->Be_flg |= 0xC00000;	/* retail: 0x400000 -> 0xC00000 (lui 0xC0 @0x80100204) */

	*(unsigned long *)&pEm->Routine_0 = 1;

	if (G.System_flg & 0x40)	/* retail-only second life set (0x8010022C..0x80100304) */
		pEm->Life = ((G.System_flg & 0x20) ? Em25_life_easy_flg40[ Rnd() & 0xF ] : Em25_life_flg40[ Rnd() & 0xF ]) + (Rnd() & 3);
	else
		pEm->Life = ((G.System_flg & 0x20) ? Em25_life_easy[ Rnd() & 0xF ] : Em25_life[ Rnd() & 0xF ]) + (Rnd() & 3);

	pEm->D_life_u = 6;
	pEm->D_life_c = 6;
	pEm->D_life_d = 6;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++)	/* retail: 10 -> 12 words (sltiu 0xC @0x80100338) */
		pEm->Free[ i + 1 ] = 0;

	*(unsigned long *)&pEm->Move_no = 1;
	Joint_move(pEm, Kan, Seq, 0x100);

	pP_ptr = &pEm->pSin_parts_ptr[ 8 ];
	pEm->Neck_flg = 0;
	pEm->Neck_no = 8;
	pP_ptr->Free[ 0 ] = 0;
	pP_ptr->Free[ 1 ] = 0;
	pP_ptr->Free[ 2 ] = 0x60;
	pP_ptr->Free[ 3 ] = 0x60;
	pP_ptr->Free[ 4 ] = 0x200;
	pP_ptr->Free[ 5 ] = 0x138;

	Kage_work_set((unsigned long *)&pEm->pKage_work, 0, 0x3200320, 0, (VECTOR *)&pEm->Pos_x);

	EF->Foot_cnt = 8;

	pEm->nOba = 1;
	pEm->Atd[ 0 ] = Em25_hit_tbl[ 0 ];
	pEm->Eff_at_r = 800;

	pFp = EF;
	if (!Ck_em_id(pEm, &pFp->Retail_28, 0x25, 1))	/* retail-only (jal @0x8010041C, sw 0 @0x8010042C) */
		pFp->Retail_28 = 0;
	switch (pEm->Type & 0xF) {
		case 0:
			pFp->Place = 0;
			pFp->Flip = 0;
			pEm->Be_flg |= 0xC000000;
			break;

		case 2:	case 3:
			pFp->Place = 1;
			pFp->Flip = 0x800;
			pEm->Cdir_z = 0x800;
			pEm->Atd[ 0 ].Ofs_y = 0;
			pEm->Be_flg |= 0x10000000;
			pFp->Down_point = pEm->Pos_y + 1250;
			break;

		case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
			pFp->Place = 3;
			pFp->Flip = 0x800;
			pFp->Wall_pos = ((unsigned char)pEm->Type - 4) >> 1;
			pEm->Cdir_y = pFp->Wall_pos << 10;
			pEm->Atd[ 0 ] = Em25_hit_tbl[ 2 ];
			Sca_ck_hit((VECTOR *)&pEm->Pos_x, 800, 1 << pEm->nFloor, 0x400);
			Ck_lock_wall(pEm);
			if (!(pEm->Type & 0xF0))

				pFp->Down_point = -3960;
			else
				pFp->Down_point = ((pEm->Type & 0xF0) >> 4) * -1800 + 1250;

			pFp->Kage_routine = 1;
			break;
	}

}

static void Em25_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Get_target_pl(pEm);

	Em25_move_tbl[ EF->Place ](pEm, Kan, Seq);

	Set_gunrange(pEm);

	if (EF->at_cnt)
		EF->at_cnt--;

	if (EF->Nowall_cnt)
		EF->Nowall_cnt--;

	if (EF->sch_flg) {
		EF->sc_timer++;
	}else {

		EF->sc_timer = 0;

		EF->Rno1_bak = pEm->Routine_1;
		EF->Rno2_bak = pEm->Routine_2;
		EF->Rno3_bak = pEm->Routine_3;
	}
}

static void Em25_Move_floor(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em25_floor_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em25_mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = 0x30000;
			pEm->Routine_2 = 1;

			pEm->Timer0 = (Rnd() + 10) & 0x1F;

		case 1:
			Joint_move(pEm, Kan, Seq, 0x400);

			if ((unsigned short)pEm->Timer0-- == 0) {
				*(unsigned long *)&pEm->Routine_0 = 0x101;
				if (Rnd() & 1)
					*(unsigned long *)&pEm->Routine_0 = 0x401;
			}
			break;
	}

}

static void Em25_mv000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir0;
	register unsigned short LL;
	register short Dir1;	/* inferred name: no retail record (combine folds it into its single test) */
	Em25_mv_taiki(pEm, Kan, Seq);

	LL = EF->Target_pl ? pEm->L_spl : pEm->L_pl;

	Dir0 = Em25_goto00(pEm, 0x80);
	Em25_goto00(pEm, 0x100);
	Dir1 = Em25_goto00(pEm, 0x400);

	if (Dir1 && LL < 8000)
		*(unsigned long *)&pEm->Routine_0 = 0x501;

	if (G.Room_player_flg & 1) {
		*(unsigned long *)&pEm->Routine_0 = 0x901;
		if (Rnd() & 1)
			*(unsigned long *)&pEm->Routine_0 = 0x501;

	}

	if (EF->at_cnt == 0) {

		if (LL < 7500 && !Dir0 && LL > 3000)
			*(unsigned long *)&pEm->Routine_0 = 0x701;

		if (LL < 6000 && !Dir0 && LL > 1000) {
			if ((0x340 >> (Rnd() & 0xF)) & 1)
				*(unsigned long *)&pEm->Routine_0 = 0x801;

			else
				*(unsigned long *)&pEm->Routine_0 = 0x701;

		}
	}
}

static void Em25_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = Em25_mv01_tbl[ pEm->Routine_1 - 1 ][ 0 ] + 0x30000;
			pEm->Routine_2 = 1;

			pEm->Spd_z = 0;

			pEm->Timer0 = Rnd() & 0x2E;

		case 1:
			pEm->Cdir_y += Em25_goto00(pEm, Em25_mv01_tbl[ pEm->Routine_1 - 1 ][ 1 ]);

			Joint_move(pEm, Kan, Seq, 0x400);
			Set_foot_se(pEm);

			Em25_foot_set(pEm, 1);
			pEm->Timer1 = pEm->Spd_x;

			pEm->Spd_x += Em25_mv01_tbl[ pEm->Routine_1 - 1 ][ 2 ];

			if ((unsigned short)pEm->Timer0-- == 0
			 || (!EF->Place	/* retail-only: stop near the target (0x801009F8..0x80100A48) */
			   && (EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 1500))
				*(unsigned long *)&pEm->Routine_0 = 1;
			break;

	}

	Add_speedXZ(pEm, EF->Flip);

}

static void Em25_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = 0x30003;
			pEm->Routine_2 = 1;

			pEm->Timer0 = (Rnd() + 10) & 0x3F;
			pEm->Timer1 = 0x18;
			if (Rnd() & 1)
				pEm->Timer1 = -0x18;

			if (EF->Place)
				pEm->Timer1 = -pEm->Timer1;

		case 1:

			pEm->Cdir_y += pEm->Timer1;

			Joint_move(pEm, Kan, Seq, 0x400);
			Set_foot_se(pEm);

			if ((unsigned short)pEm->Timer0-- == 0)
				*(unsigned long *)&pEm->Routine_0 = 1;
			break;

	}

}

static void Em25_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register short Add_dir;

	unsigned long Em25_mv03_tbl[2][2] = { { 2, 0x80 }, { 3, 0x20 } };

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = Em25_mv03_tbl[ pEm->Routine_1 - 5 ][ 0 ] + 0x30000;
			pEm->Routine_2 = 1;

			pEm->Timer1 = (Rnd() & 0x1F) + 0x3C;

		case 1:
			Add_dir = Em25_goto00(pEm, Em25_mv03_tbl[ pEm->Routine_1 - 5 ][ 1 ]);

			pEm->Cdir_y += Add_dir;

			if ((unsigned short)pEm->Timer1-- == 0 || !Add_dir)
				pEm->Routine_2 = 2;

			Joint_move(pEm, Kan, Seq, 0x400);
			Set_foot_se(pEm);

			break;

		case 2:

			*(unsigned long *)&pEm->Routine_0 = 1;

	}

}

static void Em25_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register PLAYER_WORK *pTg;
	register short Dir;	/* inferred name: no retail record (shares pTg's register) */
	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = 0x30004;
			pEm->Routine_2 = 1;

			pEm->Spd_z = 0;

		case 1:
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);

			pEm->Spd_x = 100;
			Add_speedXZ(pEm, 0);
			break;

		case 2:
			*(unsigned long *)&pEm->Move_no = 0x30005;
			pEm->Timer0 = (Rnd() & 0x1F) + 0x14;
			pEm->Routine_2 = 3;

		case 3:
			Dir = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
			pEm->Cdir_y += Dir;

			Joint_move(pEm, Kan, Seq, 0x400);

			if (!Dir)
				pEm->Routine_2 = 4;

			if ((unsigned short)pEm->Timer0-- == 0)

				pEm->Routine_2 = 6;	break;

		case 4:
			*(unsigned long *)&pEm->Move_no = 0x30005;
			pEm->Routine_2 = 5;

			pEm->Timer0 = (Rnd() & 0x1F) + 10;
			pEm->Spd_x = 300;

		case 5:
			Goto00(pEm, pTg->Pos_x, pTg->Pos_z, 0x10);

			Add_speedXZ(pEm, EF->Flip);

			Joint_move(pEm, Kan, Seq, 0x400);

			if ((unsigned short)pEm->Timer0-- == 0 || pEm->At_em_no != 0xFF || (pEm->Sca_info & 1) || Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x200))

				pEm->Routine_2 = 6;

			if (Em25_atck_ck(pEm, 1500, 4300)) /* retail: 1300 -> 1500, li a1 @0x80100EE4 */

				pEm->Routine_2 = 6;	break;

		case 6:
			*(unsigned long *)&pEm->Move_no = 0x30007;
			pEm->Spd_x = 100;
			pEm->Routine_2 = 7;

		case 7:
			Add_speedXZ(pEm, EF->Flip);
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 8:

			*(unsigned long *)&pEm->Routine_0 = 1;

			EF->at_cnt = 15;

	}
	Set_foot_se(pEm);

}

static void Em25_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Off_sv;

	switch (pEm->Routine_2) {

		case 0:

			*(unsigned long *)&pEm->Move_no = 0x3000B;
			pEm->Routine_2 = 1;

			Set_poison(pEm, EF->Place + 8, pEm->Cdir_y + EF->Flip);

		case 1:

			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:
			*(unsigned long *)&pEm->Routine_0 = 1;
			EF->at_cnt = 15;
			break;

		case 3:
			if (Em25_mv08sp(pEm, Kan, Seq)) {
				*(unsigned long *)&pEm->Routine_0 = 1;
				EF->at_cnt = 15;
			}

	}
	Set_foot_se(pEm);
	Em_seq_sound(pEm);
}

static long Em25_mv08sp(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long type;

	switch (pEm->Routine_3) {

		case 0:
			pEm->Routine_3 = 1;
			*(unsigned long *)&pEm->Move_no = 0x3000A;

		case 1:

			Joint_move(pEm, Kan, Seq, 0x400);
			if (*pEm->pNow_seq & 0x10000) {
				pEm->Routine_3 = 2;
				Snd_se_enem(3, pEm);

				if (EF->Place < 2) {
					Set_poison(pEm, EF->Place + 8, pEm->Cdir_y + EF->Flip);
					return 0;
				}
				type = ((EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 3000) ? 0x89 : 0x88;

			}else break;	Set_poison(pEm, type, Get_wallshot(pEm));

			return 0;

		case 2:
			if (Joint_move(pEm, Kan, Seq, 0x400))

				pEm->Routine_3 = 3;	break;

		case 3:
			return 1;
	}

	return 0;
}

static void Em25_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	MATRIX Tm;
	SVECTOR Tsv;
	register PLAYER_WORK *pTg;

	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = 0x30008;
			pEm->Routine_2 = 1;

			Tsv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);

			Tsv.vx = 0;
			Tsv.vy += Rnd() + 0x380 + (Rnd() & 1) * 0x800;	/* retail: 0x1000 -> 0x800 (sll 11 @0x801012DC) */
			Tsv.vz = 0;
			pEm->Timer0 = Tsv.vy;	/* retail-only, sh @0x801012FC */
			RotMatrix(&Tsv, &Tm);

			Tsv.vx = (Rnd() & 0x3F) + 400;
			Tsv.vy = 0;
			Tsv.vz = 0;
			ApplyMatrixSV(&Tm, &Tsv, (SVECTOR *)&pEm->Spd_x);

		case 1:
			pEm->Cdir_y += Em25_goto00(pEm, 0x60);
			Add_speedXZ(pEm, EF->Flip + pEm->Timer0);	/* retail: + Timer0 (lh 0x158 @0x80101340) */
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:

			*(unsigned long *)&pEm->Routine_0 = 1;

			EF->sc_timer = 0;

	}
}

static void Em25_Move_ceiling(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em25_ceiling_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em25_mv001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir0;
	register unsigned short LL;

	Em25_mv_taiki(pEm, Kan, Seq);

	LL = EF->Target_pl ? pEm->L_spl : pEm->L_pl;

	Dir0 = Em25_goto00(pEm, 0x80);
	Em25_goto00(pEm, 0x100);
	Em25_goto00(pEm, 0x400);

	if (G.Room_player_flg & 1) {
		*(unsigned long *)&pEm->Routine_0 = 0x901;
		if (Rnd() & 1)
			*(unsigned long *)&pEm->Routine_0 = 0x501;

	}

	if (EF->at_cnt == 0) {

		if (LL > 3000 && LL < 7500) {
			if (!Dir0 && ((0x340 >> (Rnd() & 0xF)) & 1))
				*(unsigned long *)&pEm->Routine_0 = 0x701;

			else
				*(unsigned long *)&pEm->Routine_0 = 0x101;

		}

		if (LL < 3000 && !Dir0) {
			if ((0x340 >> (Rnd() & 0xF)) & 1)
				*(unsigned long *)&pEm->Routine_0 = 0x801;

			else
				*(unsigned long *)&pEm->Routine_0 = 0x701;

		}
	}
}

static void Em25_mv011(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pOther;	/* [INFERRED] name (retail-only): EM25_FREE Retail_28 */
	register long dist;	/* [INFERRED] name (retail-only) */

	Em25_mv01(pEm, Kan, Seq);

	/* retail-only (0x80101548..0x801015B4): XZ distance to the enemy in Retail_28 */
	pOther = EF->Retail_28;
	dist = 9999;
	if (pOther && pOther->Routine_0 != 7) {
		dist = SquareRoot0((pEm->Pos_x - pOther->Pos_x) * (pEm->Pos_x - pOther->Pos_x)
		                  + (pEm->Pos_z - pOther->Pos_z) * (pEm->Pos_z - pOther->Pos_z));
		if (EF_OF(pOther)->Place < 2)
			dist = 9999;
	}

	if (Sca_ck_hit((VECTOR *)&pEm->Pos_x, 800, 1 << pEm->nFloor, 0x200))	/* retail-only first test */
		return;
	if (Sca_ck_hit((VECTOR *)&pEm->Pos_x, 800, 1 << pEm->nFloor, 0x400)) {
		if (dist < 2500) {	/* retail-only */
			if (pOther)
				pEm->Routine_1 = 11;
			else
				pEm->Routine_1 = 4;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}else if (!EF->Nowall_cnt && !(G.Room_enemy_flg & 0x20)) {
			if ((EF->Wall_pos = Ck_lock_wall(pEm))) {
				EF->Wall_pos--;
				if (!Ck_wall_edge(pEm, 0x400) && !Ck_wall_edge(pEm, -0x400))
					if (Ck_near_wall(pEm, EF->pWall, EF->Wall_pos)) {
						pEm->Routine_1 = 10;
						*(unsigned short *)&pEm->Routine_2 = 0;
						EF->Cont_flg = 1;
						G.Room_enemy_flg |= 0x20;
					}
			}

		}else {

			pEm->Routine_1 = 4;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}
	}
}

static void Em25_mv071(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Add_dir;

	switch (pEm->Routine_2) {
		case 0:
			*(unsigned long *)&pEm->Move_no = 0x30004;
			pEm->Routine_2 = 1;

			pEm->Spd_z = 0;

		case 1:
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);

			pEm->Spd_x = 100;
			Add_speedXZ(pEm, EF->Flip);

			break;

		case 2:
			*(unsigned long *)&pEm->Move_no = 0x30005;
			pEm->Timer0 = (Rnd() & 0x1F) + 0x2D;
			pEm->Routine_2 = 3;

		case 3:
			Add_dir = Em25_goto00(pEm, 0x40);
			pEm->Cdir_y += Add_dir;

			Joint_move(pEm, Kan, Seq, 0x400);

			if (!Add_dir)
				pEm->Routine_2 = 4;

			if ((unsigned short)pEm->Timer0-- == 0)

				pEm->Routine_2 = 6;	break;

		case 4:
			*(unsigned long *)&pEm->Move_no = 0x30005;
			pEm->Routine_2 = 5;

			pEm->Timer0 = (Rnd() & 0x1F) + 10;
			pEm->Timer2 = 1;
			pEm->Spd_x = 300;

		case 5:
			pEm->Cdir_y += Em25_goto00(pEm, 0x10);

			Add_speedXZ(pEm, EF->Flip);

			Joint_move(pEm, Kan, Seq, 0x400);

			if (!--*(signed char *)&pEm->Timer2) {
				Snd_se_enem(3, pEm);
				Set_poison(pEm, 0x8A, pEm->Cdir_y);
				pEm->Timer2 = 12;
			}

			if ((unsigned short)pEm->Timer0-- == 0 || Em25_goto00(pEm, 0x200) || (pEm->Sca_info & 1) || pEm->At_em_no != 0xFF)

				pEm->Routine_2 = 6;

			break;

		case 6:
			*(unsigned long *)&pEm->Move_no = 0x30007;
			pEm->Spd_x = 100;
			pEm->Routine_2 = 7;

		case 7:
			Add_speedXZ(pEm, EF->Flip);
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 8:

			*(unsigned long *)&pEm->Routine_0 = 1;

			EF->at_cnt = 60;

	}
	Set_foot_se(pEm);

}

static void Em25_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			pEm->Cdir_y &= 0xFFF;
			pEm->Timer0 = (EF->Sc_dir - pEm->Cdir_y) & 0xFFF;
			pEm->Timer1 = 0x18;
			if (pEm->Timer0 & 0x800) {
				pEm->Timer0 = (pEm->Cdir_y - EF->Sc_dir) & 0xFFF;
				pEm->Timer1 = -pEm->Timer1;
			}
			*(unsigned long *)&pEm->Move_no = 0x30003;

		case 1:

			if (pEm->Timer0 > 0) {
				pEm->Cdir_y += pEm->Timer1;
				pEm->Timer0 -= 0x18;
			}else {

				pEm->Routine_2 = 2;
				pEm->Cdir_y = EF->Sc_dir;
			}
			Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:

			pEm->Routine_2 = 3;
			pEm->Spd_x = 25;
			pEm->Spd_z = 0;

			pEm->Atd[ 0 ] = Em25_hit_tbl[ 1 ];

			*(unsigned long *)&pEm->Move_no = 0x3000E;

		case 3:
			Add_speedXZ(pEm, EF->Flip);
			if (Joint_move(pEm, Kan, Seq, 0x400)) {
				*(unsigned long *)&pEm->Routine_0 = 1;
				EF->Place = 2;

				pEm->Spd_x = 450;

				Add_speedXZ(pEm, EF->Flip);
				pEm->Pos_y += 1250;
				EF->Down_point = pEm->Pos_y;
				EF->Cont_flg = 0;
				G.Room_enemy_flg &= 0xFFDF;
				Em25_Mv_wall000(pEm, Kan, Seq);
			}

	}

	Set_foot_se(pEm);

}

/* Retail-only ceiling routine 11 (em25_cdemd0_s1 0x80101BC8 / s0 0x8010EBC8; Em25_ceiling_tbl[11],
 * entered from Em25_mv011's Routine_1 = 11 when the Retail_28 enemy is within 2500).  The name
 * is [INFERRED] from the table-index naming of its neighbours (Em25_mv0a = entry 10); the
 * original name and source line are unknown (source line not recorded). Turns toward the other enemy for
 * 60 frames while accelerating like Em25_mv01 (Em25_mv01_tbl[0][2]). */
static void Em25_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pOther;	/* [INFERRED] name: EM25_FREE Retail_28 */

	pOther = EF->Retail_28;
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(unsigned long *)&pEm->Move_no = 0x30002;
		pEm->Timer0 = 60;
		pEm->Timer1 = Goto00_ck(pEm, pOther->Pos_x, pOther->Pos_z, 0x40);
	}
	pEm->Cdir_y += pEm->Timer1;
	Joint_move(pEm, Kan, Seq, 0x400);
	Set_foot_se(pEm);
	Em25_foot_set(pEm, 1);
	pEm->Spd_x += Em25_mv01_tbl[ 0 ][ 2 ];
	if (!--pEm->Timer0)
		*(unsigned long *)&pEm->Routine_0 = 1;
	if (!(pEm->Sca_info & 1))
		Add_speedXZ(pEm, EF->Flip);
}

static void Em25_Move_walldown(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	extern void (*move_wall_tbl[])();	/* [INFERRED] block-scope declaration of the table used here: opens retail's record-less block */

	move_wall_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

}

static void Em25_wall_init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (EF->Wall_pos & 1) {
		pEm->Cdir_x = pEm->Cdir_y;
		pEm->Cdir_z = 0;
	}else {

		pEm->Cdir_x = 0;
		pEm->Cdir_z = 0x400;
	}

	pEm->Atd[ 0 ] = Em25_hit_tbl[ 2 ];

	EF->Sc_dir = 0;

	EF->Kage_routine = 1;

	*(unsigned long *)&pEm->Move_no = 0x30000;
	Joint_move(pEm, Kan, Seq, 0x400);
}

static void Em25_Mv_wall000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long dist, water;
	register PLAYER_WORK *pTg;

	Em25_wall_init(pEm, Kan, Seq);

	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	dist = pTg->Pos_y - pEm->Pos_y;
	if (dist > 3500) {
		water = Water_ck(pEm->Pos_x, pEm->Pos_z);
		if (water >= pEm->Ground) {
			pEm->Timer0 = (dist - 3500) / 100;
		}else {

			if ((pEm->Timer0 = (-(pEm->Pos_y + 2200) + water) / 100) < 0)

				pEm->Timer0 = 0;
		}

		if (pEm->Timer0) {
			pEm->Timer0 += Rnd() & 3;
			pEm->Routine_1 = 1;
			*(unsigned short *)&pEm->Routine_2 = 0;
			EF->Em_next = 2;
			return;
		}
	}

	pEm->Routine_1 = 2;
	*(unsigned short *)&pEm->Routine_2 = 1;
	pEm->Timer0 = (Rnd() & 0xF) + 0x1E;

}

static void Em25_Mv_wall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Spd_x = 100;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		*(unsigned long *)&pEm->Move_no = 0x30002;
	}

	if (pEm->Timer0) {
		Em25_add_speed(pEm, 0, 0, 0);
		pEm->Timer0--;
	}
	Joint_move(pEm, Kan, Seq, 0x400);
	Set_foot_se(pEm);

	if (!pEm->Timer0) {
		pEm->Routine_1 = EF->Em_next;
		*(unsigned short *)&pEm->Routine_2 = 0;
		pEm->Timer0 = (Rnd() & 0xF) + 0xF;
	}
}

static void Em25_Mv_wall200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(unsigned long *)&pEm->Move_no = 0x30000;
	}

	Joint_move(pEm, Kan, Seq, 0x400);

	if (!--pEm->Timer0) {
		if (((EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 3000) && ((0x340 >> (Rnd() & 0xF)) & 1))
			pEm->Routine_1 = 7;

		else
			pEm->Routine_1 = 3;

		*(unsigned short *)&pEm->Routine_2 = 0;
	}
}

static void Em25_Mv_wall30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			pEm->Timer1 = Ck_wall_to_pl(pEm, 0x20);

			if (!pEm->Timer1) {

				pEm->Routine_1 = 7;	*(unsigned short *)&pEm->Routine_2 = 0;	break;

			}	pEm->Timer0 = (pEm->Timer1 > 0) ? 0x400 : -0x400;
			if (!EF->Wall_pos)
				pEm->Timer0 = -pEm->Timer0;

		case 1:

			pEm->Routine_2 = 2;
			pEm->Timer2 = (Rnd() & 0xF) + 0xF;
			*(unsigned long *)&pEm->Move_no = 0x30002;

		case 2:

			Em25_wall_rot(pEm, pEm->Timer1);
			if ((EF->Sc_dir & 0xFFF) == (pEm->Timer0 & 0xFFF)) {
				pEm->Routine_1 = 6;

				*(unsigned short *)&pEm->Routine_2 = 0;	break;
			}
			if (!--*(signed char *)&pEm->Timer2) {
				pEm->Routine_2 = 3;
				pEm->Timer2 = (Rnd() & 0xF) + 1;
				*(unsigned long *)&pEm->Move_no = 0x30000;
			}
			break;

		case 3:

			if (!--*(signed char *)&pEm->Timer2)
				pEm->Routine_2 = 1;

	}

	Joint_move(pEm, Kan, Seq, 0x400);
	Set_foot_se(pEm);
}

static void Em25_Mv_wall40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned short dd;

	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			pEm->Timer1 = (!EF->Em_next) ? ceil_turn_tbl[ Rnd() & 7 ] : 0x40;
			EF->Cont_flg = 1;

		case 1:

			pEm->Routine_2 = 2;
			if (!EF->Em_next) {
				pEm->Timer2 = (Rnd() & 0xF) + 0xF;
			}else

				*(signed char *)&pEm->Timer2 = -1;

			if (abs(pEm->Timer1) == 0x20)

				*(unsigned long *)&pEm->Move_no = 0x30002;
			else
				*(unsigned long *)&pEm->Move_no = 0x30001;

		case 2:

			Em25_wall_rot(pEm, pEm->Timer1);

			dd = abs(pEm->Timer1);
			if ((((EF->Sc_dir & 0xFFF) - 0x800 + dd) & 0xFFF) <= dd * 2) {
				if (G.Room_enemy_flg & 0x20) {
					pEm->Routine_2 = 4;
					*(unsigned long *)&pEm->Move_no = 0x30000;
				}else

					pEm->Routine_2 = 5;
			}

			if (!--*(signed char *)&pEm->Timer2) {
				pEm->Routine_2 = 3;
				pEm->Timer2 = (Rnd() & 0xF) + 0xF;
				*(unsigned long *)&pEm->Move_no = 0x30000;
			}
			break;

		case 3:

			if (!--*(signed char *)&pEm->Timer2)
				pEm->Routine_2 = 1;

			break;

		case 4:

			if (G.Room_enemy_flg & 0x20)
				break;

		case 5:
			/* retail (0x8010243C..0x80102484): start a vertical descent instead of handing
			   over to Routine_1 = 1 with a Timer0 step count */
			pEm->Routine_2 = 6;
			EF->Sc_dir = 0x800;
			EF->Cont_flg = 1;
			G.Room_enemy_flg |= 0x20;
			pEm->Spd_x = 100;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
			*(unsigned long *)&pEm->Move_no = 0x30002;
			break;

		case 6:
			/* retail-only (0x80102488..0x801024BC) */
			Em25_add_speed(pEm, 0, 0, 0);
			if (pEm->Pos_y <= EF->Down_point) {
				pEm->Pos_y = EF->Down_point;
				pEm->Routine_1 = 5;
				*(unsigned short *)&pEm->Routine_2 = 0;
			}

	}

	Joint_move(pEm, Kan, Seq, 0x400);
	Set_foot_se(pEm);

}

static void Em25_Mv_wall50(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pOther;	/* [INFERRED] name (retail-only): EM25_FREE Retail_28 */
	register long dist;	/* [INFERRED] name (retail-only) */

	dist = 9999;	/* retail-only (li a2 @0x80102528) */
	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			if (EF->Wall_pos & 1) {
				pEm->Spd_x = 0;
				pEm->Spd_y = 25;
			}else {

				pEm->Spd_x = 25;
				pEm->Spd_y = 0;
			}
			pEm->Spd_z = 0;	/* retail-only, sh @0x8010258C */

			pEm->Atd[ 0 ] = Em25_hit_tbl[ 1 ];

			*(unsigned long *)&pEm->Move_no = 0x30000;	/* retail: 0x3000E -> 0x30000 (lui @0x801025D8) */

		case 1:
			/* retail-only (0x801025E0..0x80102684): wait while the Retail_28 enemy is near */
			pOther = EF->Retail_28;
			if (pOther && pOther->Routine_0 != 7) {
				dist = SquareRoot0((pEm->Pos_x - pOther->Pos_x) * (pEm->Pos_x - pOther->Pos_x)
				                  + (pEm->Pos_z - pOther->Pos_z) * (pEm->Pos_z - pOther->Pos_z));
				if (EF_OF(pOther)->Place != 1)
					dist = 9999;
			}
			if (dist > 2500) {
				pEm->Routine_2 = 2;
				*(unsigned long *)&pEm->Move_no = 0x3000E;
			}
			Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:

			Add_speedXYZ(pEm, 0, EF->Sc_dir);
			if (Joint_move(pEm, Kan, Seq, 0x400)) {
				EF->Place = 1;
				EF->Flip = 0x800;
				if (EF->Wall_pos & 1) {
					pEm->Cdir_y = pEm->Cdir_x + 0x800;
				}else {

					pEm->Cdir_y += 0x800;
				}

				pEm->Cdir_x = 0;
				pEm->Cdir_z = 0x800;

				EF->Nowall_cnt = 150;

				pEm->Pos_y -= 475;	/* retail: 350 -> 475 (addiu -0x1DB @0x8010270C) */
				pEm->Spd_x = 1225;
				Add_speedXZ(pEm, EF->Flip);

				pEm->Atd[ 0 ] = Em25_hit_tbl[ 0 ];
				pEm->Atd[ 0 ].Ofs_y = 0;

				*(unsigned long *)&pEm->Routine_0 = 1;
				EF->Cont_flg = 0;
				G.Room_enemy_flg &= 0xFFDF;

				EF->Kage_routine = 0;
			}

	}

	Set_foot_se(pEm);

}

static void Em25_Mv_wall60(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			pEm->Timer0 = (Rnd() & 0xF) + 0x1E;
			pEm->Spd_x = 300;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
			*(unsigned long *)&pEm->Move_no = 0x30004;

		case 1:

			if (Joint_move(pEm, Kan, Seq, 0x400)) {
				pEm->Routine_2 = 2;
				*(unsigned long *)&pEm->Move_no = 0x30005;

			}	break;

		case 2:

			Em25_add_speed(pEm, 0, 0, 0);
			Joint_move(pEm, Kan, Seq, 0x400);

			if (Ck_wall_edge(pEm, EF->Sc_dir) || pEm->At_em_no != 0xFF || (pEm->Sca_info & 1)) {

				pEm->Routine_2 = 3;
				*(unsigned long *)&pEm->Move_no = 0x30007;
			}

			if (Em25_atck_ck(pEm, 2000, 4300)) {
				pEm->Routine_2 = 3;
				*(unsigned long *)&pEm->Move_no = 0x30007;
			}
			break;

		case 3:

			if (Joint_move(pEm, Kan, Seq, 0x400)) {
				pEm->Routine_1 = 4;
				*(unsigned short *)&pEm->Routine_2 = 0;
				EF->Em_next = 0;
			}

	}
	Set_foot_se(pEm);

}

static void Em25_Mv_wall70(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short type;
	register short Dir;	/* inferred name: no retail record (gcc keeps only the call-value copy) */
	switch (pEm->Routine_2) {

		case 0:
			pEm->Routine_2 = 1;
			pEm->Timer0 = 0x10;
			*(unsigned long *)&pEm->Move_no = 0x30001;

		case 1:

			Dir = Ck_wall_to_pl(pEm, 0x40);
			Em25_wall_rot(pEm, Dir);
			if (!Dir || !--pEm->Timer0) {

				pEm->Routine_2 = 2;
				*(unsigned long *)&pEm->Move_no = 0x3000B;
				type = ((EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 3000) ? 0x89 : 0x88;

				Set_poison(pEm, type, Get_wallshot(pEm));
			}

			Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:

			if (Joint_move(pEm, Kan, Seq, 0x400)) {
				pEm->Routine_1 = 4;
				*(unsigned short *)&pEm->Routine_2 = 0;
				EF->Em_next = 0;
			}
			break;

		case 3:

			if (Em25_mv08sp(pEm, Kan, Seq)) {
				pEm->Routine_1 = 4;
				*(unsigned short *)&pEm->Routine_2 = 0;
				EF->Em_next = 0;
			}

	}
	Em_seq_sound(pEm);
}

static short Get_wallshot(ENEMY_WORK *pEm)
{
	register short dir;

	dir = EF->Sc_dir;
	if (!EF->Wall_pos)
		dir = -dir;

	return ((EF->Wall_pos << 10) + (dir - (dir >> 2))) & 0xFFF;
}

static void Em25_Move_wallset(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	extern void (*move_wallset_tbl[])();	/* [INFERRED] block-scope declaration of the table used here: opens retail's record-less block */

	move_wallset_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

}

static void Em25_Mv_wall001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em25_wall_init(pEm, Kan, Seq);

	pEm->Routine_1 = 2;
	*(unsigned short *)&pEm->Routine_2 = 0;	}

static void Em25_Mv_wall201(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Joint_move(pEm, Kan, Seq, 0x400);

	if (((EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 7200) || Ck_near_wall(pEm, EF->pWall, EF->Wall_pos)) {

		if ((EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl) < 3000)
			pEm->Routine_1 = 7;

		else
			pEm->Routine_1 = 3;

		*(unsigned short *)&pEm->Routine_2 = 0;
	}

}

#include "EM25_S0.C"
