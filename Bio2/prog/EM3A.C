/* em3a.c - reconstruction of D:\Bio2\prog\EM3A.C (enemy 3A).
 * Linked into two overlays from the same source: 0x1D (at 0x80100000) and
 * 0x3B (at 0x8010d000); only the enemy-number-specific entry names differ
 * (Em13a/Em23a, Pldamage_Em13a/23a), selected by OVL_3B.
 * Types come from the TU's retail SYM preamble (em3a.h). */
#include "EM3A.H"
/* Em3a work variables kept in the enemy's generic Free[] area */
#define EF	((EM3A_FREE *)ENEMY_FREE(pEm))
/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;
/* ---- functions of other translation units ----
 * Except where noted, EM3A.C calls them through implicit declarations
 * (arguments unnarrowed, implicit int); those open the record-less blocks of
 * the retail SYM. */
/* ---- functions of this translation unit ---- */
/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = {
	0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E
};
static unsigned short Em2a_life_easy[10] = {
	0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C
};
/* Retail life tables (em3a_cdemd0_s1 .data 0x80104B34..0x80104B93): Em2b values changed relative to
 * Beta 2 and a second complete set follows (same set as src-retail/prog/em20.c, selected there by
 * G.System_flg & 0x40); second-set names are [INFERRED], none is referenced by EM3A code. */
static unsigned short Em2b_life = 0xFA;
static unsigned short Em2b_life_easy = 0xC8;
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
static void (*Em3a_routine_0[8])() = {
	&Em3a_Init,
	&Em3a_Move,
	&Em3a_Damage,
	&Em3a_Die,
	&Em3a_Scenario,
	0,
	0,
	&Em3a_Dead
};
static void (*Em3a_move_tbl[13])() = {
	&Em3a_Move_taiki,
	&Em3a_Move_select,
	&Em3a_Move_fly,
	&Em3a_Move_atck,
	&Em3a_Move_turn,
	&Em3a_Move_walk,
	&Em3a_Move_escturn,
	&Em3a_Move_wall,
	&Em3a_Move_ikaku,
	&Em3a_Move_walk2,
	&Em3a_Move_wallset,
	&Em3a_Move_newfly,
	&func_801026FC	/* retail-only, retail data 0x80104BE4 */
};
static unsigned char atck_rnd00[8] = {
	5, 4, 5, 4, 5, 4, 5, 4
};
static unsigned char atck_rnd01[8] = {
	8, 4, 4, 4, 4, 4, 4, 4
};
static void (*move_fly_tbl[4])() = {
	&Em3a_Mv_fly00,
	&Em3a_Mv_fly10,
	&Em3a_Mv_fly20,
	&Em3a_Mv_fly30
};
static short fly_spd_tbl[16] = {
	500, -140, 10, 0, 360, -340, 30, 0, 180, -320, 40, 0,
	10, -200, 40, 0
};
static short body_rot_tbl[8] = {
	0, 12, 24, 12, 0, -12, -24, -12
};
static void (*move_atck_tbl[3])() = {
	&Em3a_Mv_atck00,
	&Em3a_Mv_atck10,
	&Em3a_Mv_atck20
};
static unsigned char atck_mset_tbl[4] = {
	0xC, 9, 9, 0xC
};
static void (*move_wall_tbl[3])() = {
	&Em3a_Mv_wall00,
	&Em3a_Mv_wall10,
	&Em3a_Mv_wall20
};
static char wall_time_tbl[8] = {
	0, 15, 0, -15, 0, 15, 0, -15
};
static void (*move_wallset_tbl[3])() = {
	&Em3a_Mv_wallset00,
	&Em3a_Mv_wallset10,
	&Em3a_Mv_wall20
};
static void (*move_newfly_tbl[4])() = {
	&Em3a_Mv_fly00,
	&Em3a_Mv_newfly10,
	&Em3a_Mv_fly20,
	&Em3a_Mv_newfly30
};
static void (*em3a_dmg_tbl[19])() = {
	&Em3a_dmg_nml,
	&Em3a_dmg_nml,
	&Em3a_dmg_nml,
	&Em3a_dmg_nml,
	&Em3a_dmg_ftl,
	&Em3a_dmg_ftl,
	&Em3a_dmg_ftl,
	&Em3a_dmg_ftl,
	&Em3a_dmg_gnml,
	&Em3a_dmg_fire,
	&Em3a_dmg_H2SO4,
	&Em3a_dmg_ftl,
	&Em3a_dmg_nml,
	&Em3a_dmg_spark,
	&Em3a_dmg_combo,
	&Em3a_dmg_fire2,
	&Em3a_dmg_gnml,
	&Em3a_dmg_combo,
	&Em3a_dmg_gnml
};
static unsigned char dieblood_type_tbl[20] = {	/* retail 0x80104CC4: entries 8, 16, 18 changed to 3 */
	0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 9, 1, 1, 0xB, 0, 3,
	3, 0, 3, 0
};
static unsigned char dieblood_ofs_tbl[20] = {
	0, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0, 0, 0, 0xC, 0, 0xC, 0,
	0xC, 0, 0, 0
};
static void (*dmg_ftl_tbl[4])() = {
	&Em3a_dmg_ftl00,
	&Em3a_Mv_fly10,
	&Em3a_Mv_fly20,
	&Em3a_Mv_fly30
};
static void (*dmg_fire_tbl[4])() = {
	&Em3a_dmg_fire00,
	&Em3a_dmg_fire10,
	&Em3a_dmg_fire20,
	&Em3a_dmg_fire30
};
static unsigned long wing_rgb_tbl[3] = {
	0x1F1F1F, 0x2F2F2F, 0x3F3F3F
};
static void (*dmg_fire2_tbl[4])() = {
	&Em3a_dmg_fire200,
	&Em3a_dmg_fire10,
	&Em3a_dmg_fire20,
	&Em3a_dmg_fire30
};
static void (*em3a_die_tbl[19])() = {	/* retail 0x80104D28: entry 5 changed to Em3a_Die_gnml */
	&Em3a_Die_nml,
	&Em3a_Die_nml,
	&Em3a_Die_nml,
	&Em3a_Die_nml,
	&Em3a_Die_gnml,
	&Em3a_Die_gnml,
	&Em3a_Die_gnml,
	&Em3a_Die_gnml,
	&Em3a_Die_gnml,
	&Em3a_Die_fire,
	&Em3a_Die_H2SO4,
	&Em3a_Die_nml,
	&Em3a_Die_nml,
	&Em3a_Die_spark,
	&Em3a_Die_nml,
	&Em3a_Die_fire,
	&Em3a_Die_gnml,
	&Em3a_Die_nml,
	&Em3a_Die_gnml
};
static void (*die_nml_tbl[3])() = {
	&Em3a_Die_nml00,
	&Em3a_Die_nml10,
	&Em3a_Die_nml20
};
static void (*die_fire_tbl[3])() = {
	&Em3a_Die_fire00,
	&Em3a_Die_nml10,
	&Em3a_Die_nml20
};
static unsigned char dmg_parts_rnd[4] = {
	8, 9, 8, 0xC
};
static void (*wing_move_tbl[3])() = {
	&Em3a_Wing_dmg,
	&Em3a_Wing_cont,
	&Em3a_Wing_right
};
static EM3A_BLOOD_TBL blood_id_tbl[14] = {	/* retail 0x80104D9C: entry 13 added (Set_blood type 13 in Em3a_Die_gnml) */
	{ 0, 1, 0x18, 0x2000 },
	{ 0, 1, 0x18, 0x2800 },
	{ 0, 1, 0, 0x1800 },
	{ 0x85, 4, 0, 0x1000 },
	{ 0x85, 1, 0, 0x1000 },
	{ 0x20, 0, 0, 0x2800 },
	{ 0x21, 0, 0, 0x2000 },
	{ 0x85, 4, 0, 0x1800 },
	{ 0x84, 7, 8, 0x1000 },
	{ 0x84, 7, 8, 0x1800 },
	{ 0x86, 0, 0, 0x1000 },
	{ 0x84, 4, 8, 0x2000 },
	{ 0, 1, 8, 0x2800 },
	{ 0x85, 1, 0, 0x1800 }
};
static short fire_ofs_tbl[72] = {
	3, 0, 0, 0, 3, -200, 0, -1400, 3, 400, 0, -800,
	4, 0, 0, -500, 5, 0, 0, 0, 5, 200, 0, 1300,
	5, -300, 0, 900, 6, 0, 0, 400, 3, -600, -200, -1300,
	3, 500, -100, -1400, 5, 700, -300, 1500, 5, -400, -200, 1600,
	0, 0, -300, 0, 0, 0, -290, 0, 0, -500, -300, 600,
	0, -750, 50, -450, 0, -900, -50, 200, 0, -1000, 300, -700
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	register EM3A_FREE *pFp;
	register unsigned long d0;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F) {

			pEm->Damage_cnt--; }else {

			pFp->Old_dmg = 0;
		}

		if (pFp->No_fire)
			pFp->No_fire--;

		Em3a_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Em3a_Wing(pEm);

		if (pFp->Biri_cnt) {
			d0 = Rnd() & 3;
			if (d0) {
				Set_blood(pEm, d0 - 1, 10);
			}else {

				Set_fire(pEm, 10, Rnd() & 3);
				Set_fire(pEm, 10, (Rnd() & 3) | 4);
			}
			pFp->Biri_cnt--;
		}

		pEm->Be_flg &= ~0x1C000000;
		if (pFp->Jump_flg || pFp->On_wall)
			pEm->Be_flg |= 0x18000000;

		else
			pEm->Be_flg |= 0xC000000;

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);

	}
}

static void Em3a_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Be_flg |= 0xC00000;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++)
		ENEMY_FREE(pEm)[i] = 0;

	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	pEm->Life = (G.System_flg & 0x20) ? 100 : 150;
	pEm->D_life_u = pEm->D_life_c = pEm->D_life_d = pEm->Life / 3;

	pEm->Be_flg |= 0xC000000;

	Kage_work_set(&pEm->pKage_work, 0, 0x57807D0, 0, &pEm->Pos_x);

	pEm->pEnemy_ptr = (ENEMY_WORK *)&G.Player;
	pEm->Neck_flg = 1;
	pEm->Neck_no = 0;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 8;
	pP_ptr->Free[3] = 0;
	pP_ptr->Free[4] = 0x100;
	pP_ptr->Free[5] = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -800;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 800;
	pEm->Atd[0].At_d = 1400;
	pEm->Atd[0].At_h = 1000;
	pEm->Atd[0].W = 800;
	pEm->Atd[0].D = 1400;
	pEm->Eff_at_r = 900;
	Set_schit(pEm, 0);

	pP_ptr = pEm->pSin_parts_ptr + 13;
	for (i = 0; i < 4; i++, pP_ptr++)
		pP_ptr->Be_flg |= 0x4000;

	EF->Wing_flg = 1;
	EF->Esc_limit = 2;
	EF->Burn_cnt = 30;

	if (pEm->Type & 0x10) {
		pEm->Routine_1 = 10;
		*(long *)&pEm->Move_no = 8;
		Joint_move(pEm, Kan, Seq, 0x100);

		EF->On_wall = 1;
		EF->Jump_flg = 1;
		pEm->Sce_free0 = 0;
	}else {

		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, Kan, Seq, 0x100);
	}
	G.pPldamage_Em[ 0x2A ] = PLDAMAGE_EM;

}

static void Em3a_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Get_target_pl(pEm);

	if (EF->No_catch)
		EF->No_catch--;

	Em3a_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	EF->Rno1_stk = pEm->Routine_1;
	EF->Rno2_stk = pEm->Routine_2;
	EF->Rno3_stk = pEm->Routine_3;
	EF->Spdx_stk = pEm->Spd_x;
	EF->Spdy_stk = pEm->Spd_y;
	EF->Spdz_stk = pEm->Spd_z;

	if (G.Room_enemy_flg & 0x20)
		EF->No_catch = 60;

}

static void Em3a_Move_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Timer0 = EF->Retail_2B ? 20 : (Rnd() & 0x1F) + 30;
		EF->Retail_2B = 0;
		*(long *)&pEm->Move_no = 0xF0000;
	}

	Joint_move(pEm, Kan, Seq, 0x100);

	if (!--pEm->Timer0) {
		pEm->Routine_1 = 1;
		*(short *)&pEm->Routine_2 = 0;
	}
}

static void Em3a_Move_select(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3A_FREE *pFp;
	register long dist;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	dist = pFp->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl;

	if (dist >= 10001) {
		pFp->Fly_type = 0;
		pEm->Routine_1 = atck_rnd00[ Rnd() & 7 ];
		if (!(pFp->Wing_flg & 1))
			pEm->Routine_1 = 9;

		pFp->Atck_ok = 1;
		pFp->Catch_ok = 0;
	}else

	if (dist >= 6001 || pFp->No_catch) {
		pFp->Fly_type = 1;
		pEm->Routine_1 = atck_rnd00[ Rnd() & 7 ];
		if (!(pFp->Wing_flg & 1))
			pEm->Routine_1 = 9;

		pFp->Atck_ok = 1;
		pFp->Catch_ok = 0;
	}else {

		EF->Fly_type = 2;
		pEm->Routine_1 = 4;
		if (pFp->Wing_flg & 1) {
			pEm->Routine_1 = atck_rnd01[ Rnd() & 7 ];
		}

		pFp->Atck_ok = 0;
		pFp->Catch_ok = 1;
	}

	*(short *)&pEm->Routine_2 = 0;
	pEm->Cdir_x = 0;

	Ck_esc_set(pEm);

	Joint_move(pEm, Kan, Seq, 0x100);
}

static void Ck_esc_set(ENEMY_WORK *pEm)
{
	if (!(pEm->Type & 1))
		if (EF->Dmg_set && (EF->Wing_flg & 1)) {
			*(long *)&pEm->Routine_0 = 0x601;

		} }

static void Em3a_Move_fly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_fly_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);

}

static void Em3a_Mv_fly00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	int Ft;	/* inferred name: Fly_type row index into fly_spd_tbl; gcc folds the *4 into every use, so the
		   variable itself is eliminated and retail has no SYM record for it (the raw byte stays in s1) */

	Ft = EF->Fly_type * 4;

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;

		pEm->Spd_x = 0;
		pEm->Spd_y = -20;
		pEm->Spd_z = 0;
		pEm->Timer0 = fly_spd_tbl[ Ft + 2 ];

		*(long *)&pEm->Move_no = 0x70001;

		pEm->Cdir_x = 0;

		EF->Jump_flg = 1;
		pEm->Timer3 = 0;
		EF->Retail_26 = pEm->Pos_x;
		EF->Retail_28 = pEm->Pos_z;

		Set_under_smoke(pEm);
	}

	pEm->Spd_y -= 20;
	pEm->Pos_y += (pEm->Spd_y >> 4) - 54;
	if (pEm->Spd_y < fly_spd_tbl[ Ft + 1 ]) {
		pEm->Spd_x = fly_spd_tbl[ Ft ];
		*(short *)&pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x70004;
	}
}

static void Set_under_smoke(ENEMY_WORK *pEm)
{
	Set_fire(pEm, 0x86, 1);
	Set_fire(pEm, 0x86, 8);
	Set_fire(pEm, 0x86, 3);
	Set_fire(pEm, 0x86, 10);
	Set_fire(pEm, 0x86, 6);
	Set_fire(pEm, 0x86, 7); }

static void Em3a_Mv_fly10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = EF->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!*(signed char *)&pEm->Timer3 && EF->Atck_ok) {
		pEm->Cdir_y += Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 4);
		if ((*(signed char *)&pEm->Timer3 = Bodyatck_ck(pEm))) {

			pEm->Cdir_y += (Rnd() & 1) ? 0x80 : -0x80;
		}
	}

	pEm->Spd_y += pEm->Timer0;
	Add_speedXYZ(pEm, 0, 0);

	if (pEm->Spd_y >= 0)

		if (pEm->Pos_y + 783 >= pEm->Ground) {
			*(short *)&pEm->Routine_2 = 2;
			return;
		}

	if (__builtin_abs(pEm->Spd_y) < pEm->Timer0 * 2) {

		if (EF->Wall_ok)
			if (pEm->Sca_info & 1) {
				pEm->Routine_1 = 7;
				*(short *)&pEm->Routine_2 = 0;
			}

		if (EF->Catch_ok)
			if (Touch_ck_main(pEm, 0, 0x73A, 0x180)) {
				pEm->Damage_cnt |= 0x80;
				pTg->Damage_cnt |= 0x80;
				pEm->Routine_1 = 3;
				*(short *)&pEm->Routine_2 = 0;
			}
	}

}

static void Em3a_Mv_fly20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Pos_y = pEm->Ground;

		*(long *)&pEm->Move_no = 0x70000;
		Set_under_smoke(pEm);
		pEm->Spd_y = -80;
		Snd_se_enem(7, pEm);
	}

	Add_speedXYZ(pEm, 0, 0);
	pEm->Spd_x -= 10;
	if (pEm->Spd_x < 11)
		pEm->Spd_x = 5;

	pEm->Spd_y += 30;
	if (pEm->Spd_y > 0 && pEm->Pos_y >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground;
		*(short *)&pEm->Routine_2 = 3;
		EF->Jump_flg = 0;
		Snd_se_enem(7, pEm);
	}
}

static void Em3a_Mv_fly30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;

		pEm->Timer0 = (Rnd() & 0xF) + 15;

	}

	if (!--pEm->Timer0) {
		*(long *)&pEm->Routine_0 = 0x101;
		if (G.Player.Life < 0)
			*(long *)&pEm->Routine_0 = 0x901;
	}

	Ck_esc_set(pEm);
}

static void Em3a_Move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_atck_tbl[ pEm->Routine_2 ](pEm, Kan, Seq); }

static void Em3a_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long dist, limit;

	*(short *)&pEm->Routine_2 = 1;

	pEm->Be_flg |= 2;
	pEm->Damage_cnt |= 0x80;

	G.Player.pEnemy_ptr = pEm;
	*(long *)&G.Player.Routine_0 = 5;
	G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
	G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
	G.Player.Damage_cnt |= 0x80;

	*(long *)&pEm->Move_no = 0xF0009;

	dist = EF->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl;
	limit = Cdir_ck(pEm, &G.Player) ? 500 : 1000;
	pEm->Spd_x = (dist - limit) / 26;
	pEm->Cdir_x = 0;
	EF->Retail_2A = 0;
}

static void Em3a_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_3 = 1;
			pEm->Timer0 = 20;
			pEm->Timer2 = 0;
			pEm->Timer3 = 20;
			*(long *)&pEm->Move_no = 10;
			Joint_move(pEm, Kan, Seq, 0x100);
			G.Player.Routine_2 = 1;
		}
		Add_speedXZ(pEm, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x18);
		if (!(EF->Wing_flg & 1))
			pEm->Pos_y -= 55;

	}else {

		pEm->Timer0 -= Gacha_ch();
		if (pEm->Timer0 <= 0 || (signed char)pEm->Timer3 <= 0) {
			*(short *)&pEm->Routine_2 = 2;
			G.Player.Routine_1 = 3;
			return;
		}

			if (!(Rnd() & 7))
				Set_blood(pEm, atck_mset_tbl[ Rnd() & 3 ], 2);

			Joint_move(pEm, Kan, Seq, 0x100);
			Em_seq_sound(pEm);
			pEm->Pos_y += (pEm->Timer2++ & 2) ? 32 : -32;

			if (!(pEm->Timer2 & 3)) {
				pEm->Timer3 -= 3;
				if (Pl_life_down(3, 0) == 2) {
					Em3a_Pl_to_die(0);
					*(short *)&pEm->Routine_2 = 2;
				}
			}
	}
}

static void Em3a_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		pEm->Damage_cnt &= 0x7F;

		pEm->Spd_x = -100;
		pEm->Spd_y = 40;

		pEm->Timer0 = pEm->Cdir_y;
		*(signed char *)&pEm->Timer2 = (Rnd() & 0x100) - 128;	/* only the low byte (-128) reaches the char Timer2 */

		*(long *)&pEm->Move_no = 0xF0004;

	case 1:
		if (pEm->Spd_y < 500)
			pEm->Spd_y += 40;

		pEm->Cdir_y += (signed char)pEm->Timer2;
		Add_speedXYZ(pEm, pEm->Timer0 - pEm->Cdir_y, 0);

		if (pEm->Spd_y >= 0) {

			if (pEm->Pos_y + 783 >= pEm->Ground) {
				pEm->Pos_y = pEm->Ground;
				pEm->Be_flg &= ~2;
				EF->Jump_flg = 0;
				Snd_se_enem(7, pEm);

				G.Room_enemy_flg &= ~0x20;

				if (EF->Wing_flg & 1) {
					pEm->Routine_1 = 1;
					*(short *)&pEm->Routine_2 = 0;
					EF->No_catch = 120;
					*(long *)&pEm->Move_no = 4;
				}else {

					pEm->Routine_3 = 2;
					pEm->Timer0 = 30;
					*(long *)&pEm->Move_no = 0;
				}
			}
		}

		Joint_move(pEm, Kan, Seq, 0x100); break;

	case 2:
		if (!--pEm->Timer0) {
			pEm->Routine_1 = 1;
			*(short *)&pEm->Routine_2 = 0;
			EF->No_catch = 60;
		}
		Joint_move(pEm, Kan, Seq, 0x100);

	}
}

static void Em3a_Move_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;
	register short dir;

	pTg = EF->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	dir = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x30);
	if (pTg->Life < 0)
		dir = -dir;

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		EF->Turn_rate = dir;
		pEm->Timer1 = 60;
		pEm->Cdir_x = 0;

	case 1:
		pEm->Routine_2 = 2;
		pEm->Timer0 = (Rnd() & 0xF) + 15;
		*(long *)&pEm->Move_no = 0xF0003;

	case 2:
		pEm->Cdir_y += (signed char)EF->Turn_rate;
		pEm->Cdir_x = body_rot_tbl[ pEm->Timer0 & 7 ];
		if (!--pEm->Timer0)

			pEm->Routine_2 = 3; break;

	case 3:
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Timer0 = (Rnd() & 0xF) + 15;
		pEm->Cdir_x = 0;

	case 4:
		if (!--pEm->Timer0)
			pEm->Routine_2 = 1;

	}

	if (!--pEm->Timer1 || !dir) {
		if ((EF->Wing_flg & 1) && G.Player.Life >= 0)
			pEm->Routine_1 = 11;

		else
			pEm->Routine_1 = 9;

		*(short *)&pEm->Routine_2 = 0;
	}

	if (!(EF->Wing_flg & 1) && !EF->No_catch)
		if (Touch_ck_main(pEm, 0, 0x73A, 0x180)) {
			pEm->Damage_cnt |= 0x80;
			pTg->Damage_cnt |= 0x80;
			pEm->Routine_1 = 3;
			*(short *)&pEm->Routine_2 = 0;
		}

	Ck_esc_set(pEm);

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);

}

/* retail-only (no Beta twin, name unknown): the steering/speed block of Beta Em3a_Move_walk,
 * moved into its own function in retail (retail 0x801013EC / 0x8010E3EC, called only from
 * Em3a_Move_walk with pEm, Kan, Seq; Kan and Seq are unused). */
static void func_801013EC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3A_FREE *pFp;
	register short dir;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);
	dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, (signed char)pFp->Turn_rate);
	pEm->Cdir_y += G.Player.Life >= 0 ? dir : -dir;
	pEm->Cdir_x = body_rot_tbl[ (pEm->Timer0 >> 1) & 7 ];

	pEm->Spd_x = (pFp->Wing_flg & 1) ? 50 : 75;
	if (!(*pEm->pNow_seq & 0x10000))
		pEm->Spd_x >>= 1;

	Add_speedXZ(pEm, 0);
}

static void Em3a_Move_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3A_FREE *pFp;
	register long dist;
	register short dir;	/* retail: never assigned after the steering block moved to func_801013EC;
				   the `!dir` test below reads the uninitialized s5 exactly as retail does */

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);
	dist = pFp->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0xF0002;
		pEm->Spd_x = 50;
		pEm->Timer0 = (dist >> 1) / 50;
		pFp->Turn_rate = 16;
		pEm->Spd_y = pEm->Spd_z = 0;
		pEm->Cdir_x = 0;
	}

	func_801013EC(pEm, Kan, Seq);

	if (!--pEm->Timer0 || !dir) {
		pEm->Routine_1 = 11;
		switch (pFp->Fly_type) {

		case 0:

			if (dist >= 10001)

				pEm->Routine_1 = 1; break;

		case 1:
			if (dist >= 6001) {

				pEm->Routine_1 = 1; }else {

				pFp->Atck_ok = 0;
				pFp->Catch_ok = 1;
			}
		}

		pFp->Fly_type++;
		*(short *)&pEm->Routine_2 = 0;
	}

	Ck_esc_set(pEm);

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);

}

static void Em3a_Move_escturn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = EF->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Cdir_y &= 0xFFF;
		pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);
		pEm->Timer0 += 0x800;
		pEm->Timer0 = (pEm->Timer0 & 0xC00) + ((pEm->Timer0 & 0x200) << 1);
		pEm->Timer1 = __builtin_abs(pEm->Cdir_y - pEm->Timer0);
		*(signed char *)&pEm->Timer2 = (pEm->Cdir_y < pEm->Timer0) ? 48 : -48;
		if (pEm->Timer1 & 0x800) {
			pEm->Timer1 = 0xFFF - pEm->Timer1;
			pEm->Timer2 = -pEm->Timer2;
		}
		pEm->Timer3 = 0;
		*(long *)&pEm->Move_no = 0xF0003;
	}

	pEm->Cdir_y += (signed char)pEm->Timer2;
	pEm->Cdir_x = body_rot_tbl[ pEm->Timer3++ & 7 ];
	pEm->Timer1 -= 0x30;
	if (pEm->Timer1 <= 0) {
		pEm->Cdir_y = pEm->Timer0;
		EF->Wall_ok = 1;
		pEm->Routine_1 = 11;
		*(short *)&pEm->Routine_2 = 0;
		EF->Fly_type = 1;
	}

	Joint_move(pEm, Kan, Seq, 0x100);

}

static void Em3a_Move_wall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_wall_tbl[ pEm->Routine_2 ](pEm, Kan, Seq); }

static void Em3a_Mv_wall00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0xF0005;
		Set_schit(pEm, 1);
		EF->On_wall = 1;
		pEm->Cdir_x = 0;
	}

	if (Joint_move(pEm, Kan, Seq, 0x100)) {
		*(short *)&pEm->Routine_2 = 1;

		pEm->Be_flg |= 4;
		Set_schit(pEm, 1);

		pEm->Spd_x = 783;
		Add_speedXZ(pEm, 0);
		pEm->Cdir_z -= 0x400;
		*(long *)&pEm->Move_no = 8;
		Joint_move(pEm, Kan, Seq, 0x100);
	}

}

static void Em3a_Mv_wall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Timer0 = (signed char)wall_time_tbl[ Rnd() & 7 ] + 150;
		EF->Dmg_set = 0;
	}

	if (!--pEm->Timer0 || EF->Dmg_set)
		*(short *)&pEm->Routine_2 = 2;

	Joint_move(pEm, Kan, Seq, 0x100);
}

static void Em3a_Mv_wall20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short d0;

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Spd_x = -70;
		pEm->Spd_y = 0;
		pEm->Timer0 = pEm->Cdir_y;
		*(signed char *)&pEm->Timer2 = (Rnd() & 1) ? 64 : -64;
		*(long *)&pEm->Move_no = 0xF0004;
	}

	if (pEm->Cdir_z < 0) {
		pEm->Cdir_z += 0x80;
	}else {

		pEm->Cdir_z = 0;
		EF->On_wall = 0;
	}
	pEm->Cdir_y += (signed char)pEm->Timer2;

	if (pEm->Spd_y < 500)
		pEm->Spd_y += 10;

	d0 = pEm->Cdir_z;
	pEm->Cdir_z = 0;
	Add_speedXYZ(pEm, pEm->Timer0 - pEm->Cdir_y, 0);
	pEm->Cdir_z = d0;

	if (pEm->Pos_y + 783 >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground - 783;
		EF->Jump_flg = 0;
		Snd_se_enem(7, pEm);
		if (pEm->Cdir_z >= 0) {
			if (pEm->Routine_0 == 1) {
				pEm->Routine_1 = 0;
				*(short *)&pEm->Routine_2 = 0;
			}else {

				*(short *)&pEm->Routine_2 = 2;
			}

			pEm->Be_flg &= ~4;
			Set_schit(pEm, 0);
			EF->Wall_ok = 0;
			EF->Dmg_set = 0;
			pEm->Pos_y = pEm->Ground;
			*(long *)&pEm->Move_no = 0xF0000;
		}
	}

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);
}

static void Em3a_Move_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv, Sv2;
	MATRIX M;

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;

		pEm->Spd_y = 0;
		EF->Jump_flg = 1;
		pEm->Timer1 = 6;
		pEm->Timer2 = 15;
		*(signed char *)&pEm->Timer3 = -8;
		*(long *)&pEm->Move_no = 0xF0004;

		pEm->Cdir_x = 0;
		Set_under_smoke(pEm);

	case 1:
		if (!pEm->Routine_3) {
			pEm->Pos_y -= 111;
			if (pEm->Pos_y <= pEm->Ground - 783)
				pEm->Routine_3 = 1;

		}else {

			pEm->Pos_y += (signed char)pEm->Timer3;
			if ((signed char)pEm->Timer2 == 15 || (signed char)pEm->Timer2 == 7) {
				Snd_se_enem(5, pEm);
			}
			if (!--*(signed char *)&pEm->Timer2) {
				pEm->Timer2 = 15;
				*(signed char *)&pEm->Timer3 *= -1;
				if (!--pEm->Timer1) {
					pEm->Routine_2 = 2;
					*(long *)&pEm->Move_no = 0xF0000;
				}
			}
		}
		Joint_move(pEm, Kan, Seq, 0x100);

		if (pEm->Timer1 < 6) {

			Sv.vx = pEm->Pos_x;
			Sv.vy = pEm->Pos_y;
			Sv.vz = pEm->Pos_z;
			Sv2.vx = 0x400 - (Rnd() << 4);
			Sv2.vy = pEm->Ground - 50 - (Rnd() & 0x1F) * 8;
			Sv2.vz = (Rnd() << 4) - 0x600;
			Set_smoke(pEm, 5, &Sv, &Sv2);
		}
		break;

	case 2:
		if (pEm->Pos_y < pEm->Ground)
			pEm->Pos_y += 111;

		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Pos_y = pEm->Ground;
			EF->Jump_flg = 0;
			pEm->Routine_1 = 1;
			*(short *)&pEm->Routine_2 = 0;
		}

	}
}

static void Em3a_Move_walk2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;
	register EM3A_FREE *pFp;
	register long dist;
	register short dir;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);
	pTg = &G.Player;
	dist = pFp->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl;

	dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, (signed char)pFp->Turn_rate);

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0xF0003;
		pEm->Spd_x = 75;
		pEm->Timer0 = dist / 75;
		pEm->Timer1 = (Rnd() & 0xF) + 31;
		pFp->Turn_rate = 24;
		pEm->Spd_y = pEm->Spd_z = 0;
		pEm->Cdir_x = 0;

	case 1:
		Root_ck(pEm, &pTg->Pos_x, 0, 0);
		pEm->Cdir_y += dir;
		pEm->Cdir_x = body_rot_tbl[ (pEm->Timer0 >> 1) & 7 ];

		pEm->Spd_x = 75;
		if (!(*pEm->pNow_seq & 0x10000))
			pEm->Spd_x = 37;

		Add_speedXZ(pEm, 0);

		if (!--pEm->Timer1) {
			*(short *)&pEm->Routine_2 = 2;
			pEm->Timer1 = (Rnd() & 0x1F) + 15;
			pEm->Cdir_x = 0;
			*(long *)&pEm->Move_no = 0xF0000;
		}

		if (!--pEm->Timer0) {
			pEm->Routine_1 = 4;
			*(short *)&pEm->Routine_2 = 0;
		}
		break;

	case 2:
		if (!--pEm->Timer1 || dist < 3000) {
			*(short *)&pEm->Routine_2 = 1;
			pEm->Timer1 = (Rnd() & 0x1F) + 15;
			*(long *)&pEm->Move_no = 0xF0003;
		}

	}

	if (!pFp->No_catch && !(G.Room_enemy_flg & 0x20)) {
		if (Touch_ck_main(pEm, 0, 0x73A, 0x180)) {
			pEm->Damage_cnt |= 0x80;
			pTg->Damage_cnt |= 0x80;
			pEm->Routine_1 = 3;
			*(short *)&pEm->Routine_2 = 0;
			G.Room_enemy_flg |= 0x20;
		}
	}

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);

}

static void Em3a_Move_wallset(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_wallset_tbl[ pEm->Routine_2 ](pEm, Kan, Seq); }

static void Em3a_Mv_wallset00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(short *)&pEm->Routine_2 = 1;

	pEm->Cdir_x = 0;

	pEm->Be_flg |= 4;
	Set_schit(pEm, 1);

	pEm->Spd_x = 783;
	pEm->Spd_z = 0;
	Add_speedXZ(pEm, 0);
	pEm->Cdir_z = -0x400;
	*(long *)&pEm->Move_no = 8;
	Joint_move(pEm, Kan, Seq, 0x100);
}

static void Em3a_Mv_wallset10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		EF->Dmg_set = 0;
	}

	if (pEm->Sce_free0 || EF->Dmg_set)
		*(short *)&pEm->Routine_2 = 2;

	Joint_move(pEm, Kan, Seq, 0x100); }

static void Em3a_Move_newfly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_newfly_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Joint_move(pEm, Kan, Seq, 0x200);
	Em_seq_sound(pEm);

}

static void Em3a_Mv_newfly10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = EF->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!*(signed char *)&pEm->Timer3 && EF->Atck_ok) {
		pEm->Cdir_y += Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 4);
		if ((*(signed char *)&pEm->Timer3 = Bodyatck_ck(pEm))) {

			pEm->Cdir_y += (Rnd() & 1) ? 0x80 : -0x80;
		}
	}

	pEm->Spd_y += pEm->Timer0;
	Add_speedXYZ(pEm, 0, 0);

	switch (pEm->Routine_3) {

	case 0:
		if (pEm->Spd_y >= 0) {
			pEm->Routine_3 = 1;
			EF->Em_keep = (pEm->Pos_y - pEm->Ground) >> 1;
		}
		break;

	case 1:
		if (pEm->Pos_y >= EF->Em_keep) {
			pEm->Routine_3 = 2;
			pEm->Timer0 = -pEm->Timer0;
		}
		break;

	case 2:
		if (pEm->Pos_y + 195 >= pEm->Ground) {
			*(short *)&pEm->Routine_2 = 2;
			return;
		}
		if (pEm->Spd_y <= pEm->Timer0) {
			pEm->Routine_3 = 3;
			pEm->Spd_y = __builtin_abs(pEm->Timer0);
			pEm->Timer0 = 0;
		}
		break;

	case 3:
		if (pEm->Pos_y + 195 >= pEm->Ground) {
			*(short *)&pEm->Routine_2 = 2;
			return;
		}

	}

	if (__builtin_abs(pEm->Spd_y) < pEm->Timer0 * 2 && pEm->Routine_3 < 2) {

		if (EF->Wall_ok)
			if (pEm->Sca_info & 1) {
				pEm->Routine_1 = 7;
				*(short *)&pEm->Routine_2 = 0;
			}

		if (EF->Catch_ok)
			if (Touch_ck_main(pEm, 0, 0x73A, 0x180)) {
				pEm->Damage_cnt |= 0x80;
				pTg->Damage_cnt |= 0x80;
				pEm->Routine_1 = 3;
				*(short *)&pEm->Routine_2 = 0;
			}
	}

}

static void Em3a_Mv_newfly30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		pEm->Timer0 = 15;
		*(long *)&pEm->Move_no = 0x70003;
		pEm->Spd_z = 0;
		if (__builtin_abs(pEm->Pos_x - EF->Retail_26) < 800 && __builtin_abs(pEm->Pos_z - EF->Retail_28) < 800) {
			if (++EF->Retail_2A >= 3) {
				*(long *)&pEm->Routine_0 = 0xC01;
				return;
			}
		}else {
			EF->Retail_2A = 0;
		}

	case 1:
		pEm->Spd_x = 75;
		if (!(*pEm->pNow_seq & 0x10000))
			pEm->Spd_x = 37;

		Add_speedXZ(pEm, 0);
		if (!--pEm->Timer0 || (pEm->Sca_info & 1)) {
			pEm->Routine_3 = 2;
			*(long *)&pEm->Move_no = 0x70000;
			pEm->Timer0 = (Rnd() & 0xF) + 15;
		}
		Em_seq_sound(pEm);
		break;

	case 2:
		if (!--pEm->Timer0) {
			*(long *)&pEm->Routine_0 = 0x101;
			if (G.Player.Life < 0)
				*(long *)&pEm->Routine_0 = 0x901;
		}

	}

	Ck_esc_set(pEm);
}

/* retail-only (no Beta twin, name unknown): Em3a_move_tbl[12] (retail data 0x80104BE4), entered
 * from Em3a_Mv_newfly30 (Routine 0xC01) after three landings within 800 of the take-off point;
 * a long walk (Timer0 450) using the retail steering block func_801013EC. Retail 0x801026FC / 0x8010F6FC. */
static void func_801026FC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3A_FREE *pFp;
	register long dist;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);
	dist = pFp->Target_pl ? (pEm->L_spl & 0x3FFFFFFF) : pEm->L_pl;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0xF0002;
		pEm->Spd_x = 50;
		pEm->Timer0 = 450;
		pFp->Turn_rate = 16;
		pEm->Spd_y = pEm->Spd_z = 0;
		pEm->Cdir_x = 0;
		pFp->Retail_2A = 0;
	}

	func_801013EC(pEm, Kan, Seq);

	if (!--pEm->Timer0 || dist < 1700) {
		pEm->Routine_1 = 1;
		*(short *)&pEm->Routine_2 = 0;
	}

	Ck_esc_set(pEm);

	Joint_move(pEm, Kan, Seq, 0x100);
	Em_seq_sound(pEm);
}

static void Em3a_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Cdir_x = 0;

	em3a_dmg_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq); }

static void Em3a_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3A_FREE *pFp;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	if (dieblood_ofs_tbl[ pEm->Routine_1 - 1 ] && !EF->On_wall) {
		Set_fire(pEm, dieblood_type_tbl[ pEm->Routine_1 - 1 ], dieblood_ofs_tbl[ pEm->Routine_1 - 1 ]);
	}else {

		Set_blood(pEm, 0, dieblood_type_tbl[ pEm->Routine_1 - 1 ]);
	}

	if (!pFp->No_snd) {
		Snd_se_enem(3, pEm);
	}

	pFp->No_snd = 0;
	Esc_dmg_ck(pEm);
	EF->Old_dmg = pEm->Routine_1;

	*(long *)&pEm->Routine_0 = (pFp->Rno3_stk << 24) + ((pFp->Rno2_stk << 16) + (pFp->Rno1_stk << 8) + 1);
	pEm->Spd_x = pFp->Spdx_stk;
	pEm->Spd_y = pFp->Spdy_stk;
	pEm->Spd_z = pFp->Spdz_stk;

	Em3a_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	pFp->Rno1_stk = pEm->Routine_1;
	pFp->Rno2_stk = pEm->Routine_2;
	pFp->Rno3_stk = pEm->Routine_3;
	pFp->Spdx_stk = pEm->Spd_x;
	pFp->Spdy_stk = pEm->Spd_y;
	pFp->Spdz_stk = pEm->Spd_z;

}

static void Esc_dmg_ck(ENEMY_WORK *pEm)
{
	register unsigned long max;

	if (EF->On_wall)
		EF->Dmg_set = 1;

	max = (G.System_flg & 0x20) ? 100 : 150;

	if (EF->Esc_limit && !EF->Dmg_set && (EF->Wing_flg & 1))
		if (pEm->Life < (EF->Esc_limit * max) / 3) {
			EF->Dmg_set = 1;
			EF->Esc_limit--;

		} }

static void Em3a_dmg_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_ftl_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	if (pEm->Routine_2)
		Joint_move(pEm, Kan, Seq, 0x100);

	EF->Spdx_stk = pEm->Spd_x;
	EF->Spdy_stk = pEm->Spd_y;
	EF->Spdz_stk = pEm->Spd_z; }

static void Em3a_dmg_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Snd_se_enem(3, pEm);
	EF->Retail_2A = 0;

	if (!EF->Jump_flg) {
		EF->Wroutine_0 = 0;
		EF->Wroutine_1 = 0;
		*(long *)&pEm->Move_no = 3;
		EF->Wing_flg |= 0x80;
		Joint_move(pEm, Kan, Seq, 0x100);
		*(long *)&pEm->Routine_0 = 1;
		EF->Retail_2B = 1;
		EF->No_catch = 0;
		EF->Rno1_stk = pEm->Routine_1;
		EF->Rno2_stk = pEm->Routine_2;
		EF->Rno3_stk = pEm->Routine_3;
	}else

	if (EF->On_wall) {
		Em3a_dmg_nml(pEm, Kan, Seq);
	}else {

		pEm->Routine_2 = 1;
		if (!EF->No_snd)
			Snd_se_enem(3, pEm);

		EF->No_snd = 0;
		Set_blood(pEm, 0, dieblood_type_tbl[ pEm->Routine_1 - 1 ]);

		Dmg_ftl00_0(pEm, Kan, Seq);
	}
}

static void Dmg_ftl00_0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = EF->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pEm->Spd_x = 0;
	pEm->Spd_y = EF->Spdy_stk;
	pEm->Spd_z = EF->Spdz_stk;
	if (pEm->Spd_y < 0)
		pEm->Spd_y = 40;

	pEm->Timer0 = 40;
	Goto00(pEm, pTg->Pos_x, pTg->Pos_z, -0x100);

	EF->Atck_ok = 0;
	EF->Catch_ok = 0;
	EF->Wall_ok = 0;

	Esc_dmg_ck(pEm);
	EF->Old_dmg = pEm->Routine_1;

	if (pEm->Move_no != 4)
		*(long *)&pEm->Move_no = 0xF0004;

	Joint_move(pEm, Kan, Seq, 0x100);
	EF->No_snd = 0;

}

static void Em3a_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Set_blood(pEm, (Rnd() + 1) & 1, 0);
	Em3a_dmg_nml(pEm, Kan, Seq);
}

static void Em3a_dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Set_blood(pEm, 0, 7);
		Em3a_dmg_ftl00(pEm, Kan, Seq);
	}else {

		dmg_ftl_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
		Joint_move(pEm, Kan, Seq, 0x100);
	} }

static void Em3a_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_fire_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	if (pEm->Routine_2 != 3)
		Joint_move(pEm, Kan, Seq, 0x100);

	EF->Spdx_stk = pEm->Spd_x;
	EF->Spdy_stk = pEm->Spd_y;
	EF->Spdz_stk = pEm->Spd_z; }

static void Em3a_dmg_fire00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;
	register PARTS_W *pP_ptr;

	Snd_se_enem(3, pEm);
	EF->Old_dmg = pEm->Routine_1;

	if (!(EF->Wing_flg & 1)) {
		pEm->Routine_1 = 7;
		Set_blood(pEm, 0, 3);
	}else {

		pEm->Damage_cnt |= 0x80;

		if ((signed char)(EF->Burn_cnt -= 15) < 0)
			EF->Burn_cnt = 0;

		if (!*(signed char *)&EF->Burn_cnt) {
			for (i = 3; i < 7; i++) {
				pP_ptr = pEm->pSin_parts_ptr + i;
				pP_ptr->Poly_rgb = wing_rgb_tbl[ (signed char)EF->Burn_cnt / 10 ];
			}
		}

		if (!EF->Jump_flg) {
			*(short *)&pEm->Routine_2 = 2;
		}else

		if (EF->On_wall) {
			*(short *)&pEm->Routine_2 = 3;
		}else {

			*(short *)&pEm->Routine_2 = 1;

			Dmg_ftl00_0(pEm, Kan, Seq);
		} } }

static void Em3a_dmg_fire10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em3a_Mv_fly10(pEm, Kan, Seq);
	Set_fire(pEm, 3, Rnd() & 7);
}

static void Em3a_dmg_fire20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;
	register PARTS_W *pP_ptr, *pQ_ptr;
	register EM3A_FREE *pFp;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0;

		pEm->Pos_y = pEm->Ground;

		EF->Wroutine_0 = 1;
		EF->Wroutine_1 = 0;
		EF->Wing_flg |= 0x80;

		if (!*(signed char *)&EF->Burn_cnt) {
			for (i = 3; i < 7; i++) {
				pP_ptr = pEm->pSin_parts_ptr + i;
				pP_ptr->Be_flg |= 0x400;
				pP_ptr->Size_x = pP_ptr->Size_y = pP_ptr->Size_z = 0xFFF;
			}
			pFp->Fire_cnt = 32;
		}

	case 1:
		for (i = 3; i < 7; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Size_x -= 0x80;
			pP_ptr->Size_y -= 0x80;
			pP_ptr->Size_z -= 0x80;
		}

		if (pP_ptr->Size_x <= 0x400)
			pEm->Routine_3 = 2;

		break;

	case 2:
		if (!*(signed char *)&EF->Burn_cnt) {
			pP_ptr = pEm->pSin_parts_ptr + 13;	/* source = parts 13-16 (SYM: pQ_ptr is the a3 destination) */
			pQ_ptr = pEm->pSin_parts_ptr + 3;
			for (i = 0; i < 4; i++, pP_ptr++, pQ_ptr++) {
				pQ_ptr->Be_flg &= ~0x400;
				pQ_ptr->pTmd = pP_ptr->pTmd;
				pQ_ptr->pTmd2 = pP_ptr->pTmd2;
				pQ_ptr->pPacket = pP_ptr->pPacket;
				pQ_ptr->pPacket2 = pP_ptr->pPacket2;
				pQ_ptr->Poly_rgb = pP_ptr->Poly_rgb;
			}
			EF->Wing_flg = 0;
		}else

			EF->Fire_cnt = 16;

		EF->Wing_flg &= 0x7F;
		pEm->Damage_cnt &= 0x7F;
		*(long *)&pEm->Routine_0 = 0x101;
		pFp->Rno1_stk = pEm->Routine_1;
		pFp->Rno2_stk = pEm->Routine_2;
		pFp->Rno3_stk = pEm->Routine_3;

	}

}

static void Em3a_dmg_fire30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em3a_Mv_wall20(pEm, Kan, Seq);
	Set_fire(pEm, 3, Rnd() & 7);
}

static void Em3a_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Set_fire(pEm, 8, Rnd() & 3);
		Set_fire(pEm, 8, (Rnd() & 3) | 4);
		Em3a_dmg_ftl00(pEm, Kan, Seq);
	}else {

		dmg_ftl_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
		Joint_move(pEm, Kan, Seq, 0x100);
	}

}

static void Em3a_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em3a_dmg_ftl00(pEm, Kan, Seq);
		EF->Biri_cnt = 20;
	}else {

		dmg_ftl_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
		Joint_move(pEm, Kan, Seq, 0x100);
	} }

static void Em3a_dmg_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_fire2_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	if (pEm->Routine_2 != 3)
		Joint_move(pEm, Kan, Seq, 0x100);

	EF->Spdx_stk = pEm->Spd_x;
	EF->Spdy_stk = pEm->Spd_y;
	EF->Spdz_stk = pEm->Spd_z; }

static void Em3a_dmg_fire200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;
	register PARTS_W *pP_ptr;

	if (pEm->Routine_1 == EF->Old_dmg)
		EF->No_snd = 1;

	if ((signed char)(EF->Burn_cnt -= 6) < 0)
		EF->Burn_cnt = 0;

	if (!(EF->Wing_flg & 1) || *(signed char *)&EF->Burn_cnt) {
		if (!EF->No_fire) {

			Set_fire(pEm, 3, Rnd() & 7);
			EF->No_fire = 15;
		}
		Em3a_dmg_nml(pEm, Kan, Seq);
	}else {

		pEm->Damage_cnt |= 0x80;
		EF->Old_dmg = pEm->Routine_1;

		if (!*(signed char *)&EF->Burn_cnt) {
			for (i = 3; i < 7; i++) {
				pP_ptr = pEm->pSin_parts_ptr + i;
				pP_ptr->Poly_rgb = wing_rgb_tbl[ (signed char)EF->Burn_cnt / 10 ];
			}
		}

		if (!EF->No_snd) {
			Snd_se_enem(3, pEm);
		}

		EF->No_snd = 0;
		if (!EF->Jump_flg) {
			*(short *)&pEm->Routine_2 = 2;
		}else

		if (EF->On_wall) {
			*(short *)&pEm->Routine_2 = 3;
		}else {

			*(short *)&pEm->Routine_2 = 1;

			Dmg_ftl00_0(pEm, Kan, Seq);
		} } }

static void Em3a_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Cdir_x = 0;

	em3a_die_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);

	Em_dead_flg_on(pEm);
}

static void Em3a_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq); }

static void Em3a_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Set_blood(pEm, 0, dieblood_type_tbl[ pEm->Routine_1 - 1 ]);
	Snd_se_enem(6, pEm);

	if (!EF->Jump_flg) {
		*(short *)&pEm->Routine_2 = 2;
		Em3a_Die_nml20(pEm, Kan, Seq);
	}else {

		if (EF->On_wall) {
			pEm->Cdir_z = 0;
			Set_schit(pEm, 0);
		}

		*(short *)&pEm->Routine_2 = 1;

		Dmg_ftl00_0(pEm, Kan, Seq);
	} }

static void Em3a_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em3a_Mv_fly10(pEm, Kan, Seq);
	Joint_move(pEm, Kan, Seq, 0x100); }

static void Em3a_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Pos_y = pEm->Ground;
		*(long *)&pEm->Move_no = 0xF0007;
	}

	if (Joint_move(pEm, Kan, Seq, 0x100)) {
		*(long *)&pEm->Routine_0 = 7;
	} }

static void Em3a_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, Dmg;

	Dmg = pEm->Routine_1;

	if (!pEm->Routine_2) {
		if (Dmg == 9)
			Set_blood(pEm, 0, 4);

		Set_blood(pEm, 2, 1);
		if (pEm->Damage_no < 3 || Dmg != 9) {
			if (Dmg == 9 || Dmg == 17 || Dmg == 19) {
				for (i = 0; i < 13; i++) {
					pP_ptr = pEm->pSin_parts_ptr + i;
					pP_ptr->Poly_rgb = 0x502040;
				}
				Set_blood(pEm, 0, 13);
			}

			pP_ptr = pEm->pSin_parts_ptr + 2;
			pP_ptr->Be_flg |= 0x44A;
			pP_ptr->Free[0] = pEm->Cdir_y;
			pP_ptr->Free[4] = 0;
			pP_ptr->Free[2] = -30;
			pP_ptr->Free[1] = -20;
			pP_ptr->Free[3] = 2;
			pP_ptr->Free[6] = -5;
			pP_ptr->Poly_rgb = 0x101010;
			pP_ptr->Size_x = 0xC00;
			pP_ptr->Size_y = 0xC00;
			pP_ptr->Size_z = 0xC00;

			for (i = 14; i < 18; i++) {
				Set_fire(pEm, 1, i);
				Set_fire(pEm, 12, i);
			}
		}
		Em3a_Die_nml00(pEm, Kan, Seq);
	}else {

		die_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
	}

}

static void Em3a_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_fire_tbl[ pEm->Routine_2 ](pEm, Kan, Seq); }

static void Em3a_Die_fire00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	if (EF->Wing_flg & 1) {
		for (i = 0; i < 4; i++) {
			Set_fire(pEm, 4, Rnd() & 7);
			Set_fire(pEm, 3, Rnd() & 7);
		}
	}

	for (i = 0; i < 4; i++)
		Set_fire(pEm, 4, i + 12);

	for (i = 0; i < 13; i++) {
		pP_ptr = pEm->pSin_parts_ptr + i;
		pP_ptr->Poly_rgb = 0x3F3F3F;
	}

	Em3a_Die_nml00(pEm, Kan, Seq);
}

static void Em3a_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	if (!pEm->Routine_2) {
		Set_fire(pEm, 8, Rnd() & 3);
		Set_fire(pEm, 8, (Rnd() & 3) | 4);
		Set_fire(pEm, 9, Rnd() & 3);
		Set_fire(pEm, 9, (Rnd() & 3) | 4);

		for (i = 0; i < 13; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Be_flg |= 0x400;
			pP_ptr->Poly_rgb = 0x301040;
			pP_ptr->Size_x = 0x1000;
			pP_ptr->Size_y = 0x1000;
			pP_ptr->Size_z = 0x1000;
		}

		pP_ptr = pEm->pSin_parts_ptr + 1;
		pP_ptr->Be_flg |= 0x10;
		pP_ptr->Free[0] = 0;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[3] = 30;
		((unsigned char *)pP_ptr->Free)[4] = 30;	/* byte view of Free[2] (two byte stores in retail) */
		((unsigned char *)pP_ptr->Free)[5] = 30;

		Em3a_Die_nml00(pEm, Kan, Seq);
	}else {

		die_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
	}

	if (EF->Wing_flg) {
		for (i = 3; i < 7; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			if (pP_ptr->Size_x > 0xC00) {
				pP_ptr->Size_x -= 0x40;
				pP_ptr->Size_y -= 0x40;
				pP_ptr->Size_z -= 0x40;
			}
		}
	}
}

static void Em3a_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	if (!pEm->Routine_2) {
		Set_blood(pEm, 2, 11);
		EF->Biri_cnt = 45;
		for (i = 0; i < 13; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Poly_rgb = 0x3F3F4F;
		}
		Em3a_Die_nml00(pEm, Kan, Seq);
	}else {

		die_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
	}
}

static void Em3a_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em3a_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_1) {
		pEm->Routine_1 = 1;
		pEm->Be_flg |= 0x42;
		G.Room_enemy_flg &= ~0x20;
	}
	Em3a_kage_move(pEm); }

static void Em3a_kage_move(ENEMY_WORK *pEm)
{
	register KAGE_WORK *pKage;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (EF->Kage_routine) {

	case 0:
		EF->Kage_routine = 1;
		*(long *)&pKage->Prim[0].r0 &= 0xFF000000;
		*(long *)&pKage->Prim[0].r0 |= 0xAF4FAF;
		*(long *)&pKage->Prim[1].r0 &= 0xFF000000;
		*(long *)&pKage->Prim[1].r0 |= 0xAF4FAF;
		pEm->Timer3 = 90;

	case 1:
		pKage->Half_x += 10;
		pKage->Half_z += 14;
		if (!--*(signed char *)&pEm->Timer3)
			EF->Kage_routine = 2;
		break;

	case 2:	/* inferred: retail's case-dispatch tree tests a third (empty) case value */
		break;
	}

}

void PLDAMAGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_1) {
	case 0:
		pEm->Routine_1 = 1;
		*(long *)&pEm->Move_no = 0xF0001;
		pEm->Neck_flg |= 1;
		Pad_vib_set0(10, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(30, 150, 0);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_1 = 2;
			*(long *)&pEm->Move_no = 0xF0002;
		}
		break;

	case 2:
		Joint_move(pEm, Kan, Seq, 0x100);
		if (!(Rnd() & 7)) {
			Set_blood(pEm, dmg_parts_rnd[ Rnd() & 3 ], 2);
			Pad_vib_set0((Rnd() & 3) + 3, Rnd() & 3);		/* retail-only Dual Shock vibration */
			Pad_vib_set1((Rnd() & 3) + 2, 250, Rnd() & 3);
			Pad_vib_set0((Rnd() & 3) + 3, (Rnd() & 3) + 3);
			Pad_vib_set1((Rnd() & 3) + 2, 250, (Rnd() & 3) + 3);
		}
		break;

	case 3:
		pEm->Routine_1 = 4;
		*(long *)&pEm->Move_no = 0xF0003;
		Pad_vib_set1(3, 120, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(10, 120, 0, 3);

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Neck_flg &= ~1;
			pEm->Damage_cnt &= 0x7F;
			pEm->Be_flg &= ~0x42;

			*(long *)&pEm->Routine_0 = 1;
		}

	}

	if (pEm->Routine_2) {
		pEm->Routine_2 = 0;
		Snd_se_stad(0x4020001, &pEm->Pos_x);
	}
}

static void Wing_set(ENEMY_WORK *pEm, unsigned long No, short Rx, short Ry)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr + (No & 0x3F);
	if (!(No & 0x40)) {
		pP_ptr->Cdir_x = -Rx;
		pP_ptr->Cdir_y = -Ry;
		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
	}

	pP_ptr += 2;

	if (!(No & 0x80)) {
		pP_ptr->Cdir_x = Rx;
		pP_ptr->Cdir_y = Ry;
		RotMatrix(&pP_ptr->Cdir_x, pP_ptr->M);
	}
}

static void Em3a_Wing(ENEMY_WORK *pEm)
{
	register EM3A_FREE *pFp;

	pFp = (EM3A_FREE *)ENEMY_FREE(pEm);

	if (pFp->Fire_cnt) {
		pFp->Fire_cnt--;
		if ((pFp->Wing_flg & 1) && (pFp->Fire_cnt & 1)) {
			Set_fire(pEm, 3, Rnd() & 7);
		}
	}

	if (pFp->Wing_flg & 0x80)

		wing_move_tbl[ pFp->Wroutine_0 ](pEm, pFp);
}

static void Em3a_Wing_dmg(ENEMY_WORK *pEm, EM3A_FREE *pFp)
{
	if (!pFp->Wroutine_1)
		pFp->Wing_free = 12;

	Wing_fast_main(pEm, pFp);

	if (!--*(signed char *)&pFp->Wing_free)
		pFp->Wing_flg &= 0x7F;

	Wing_set(pEm, 3, pFp->Fwing_ang, 0);
	Wing_set(pEm, 4, pFp->Bwing_ang, 0); }

static void Em3a_Wing_cont(ENEMY_WORK *pEm, EM3A_FREE *pFp)
{
	Wing_fast_main(pEm, pFp);

	Wing_set(pEm, 3, pFp->Fwing_ang, 0);
	Wing_set(pEm, 4, pFp->Bwing_ang, 0); }

static void Em3a_Wing_right(ENEMY_WORK *pEm, EM3A_FREE *pFp)
{
	if (!pFp->Wroutine_1)
		pFp->Wing_free = 0;

	Wing_fast_main(pEm, pFp);

	if (!*(signed char *)&pFp->Wing_free) {
		if (pFp->Wroutine_1 == 2)
			pFp->Wing_free = 1;

	}else

	if (pFp->Wroutine_1 == 1)
		pFp->Wing_flg &= 0x7F;

	Wing_set(pEm, 0x83, pFp->Fwing_ang, 0);
	Wing_set(pEm, 0x84, pFp->Bwing_ang, 0); }

static void Wing_fast_main(ENEMY_WORK *pEm, EM3A_FREE *pFp)
{

	switch (pFp->Wroutine_1) {

	case 0:
		pFp->Wroutine_1 = 1;
		pFp->Bwing_ang = pFp->Fwing_ang = -0x100;

	case 1:
		pFp->Fwing_ang += 0x200;

		if (pFp->Fwing_ang >= 0x300) {
			pFp->Wroutine_1 = 2;

			pFp->Fwing_ang = 0x300; break;
		}
		break;
	case 2:
		pFp->Fwing_ang -= 0x200;

		if (pFp->Fwing_ang < -0xFF) {
			pFp->Wroutine_1 = 1;
			pFp->Fwing_ang = -0x100;
		}

	}

	pFp->Bwing_ang = pFp->Fwing_ang; }

static ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
	register ENEMY_WORK *pTg;

	pTg = (ENEMY_WORK *)&G.Player;
	EF->Target_pl = 0;

	if (G.Status_flg & 0x10000000) {
		if ((pEm->L_spl & 0xC0000000) == 0x40000000 && (pEm->L_spl & 0x3FFFFFFF) < pEm->L_pl) {

			pTg = G.pEnemy[1];
			EF->Target_pl = 1;
		}
	}

	return pTg; }

static unsigned long Touch_ck_main(ENEMY_WORK *pEm, unsigned long Pl, long Range, short Dir)
{
	register PLAYER_WORK *pTg;
	register long dist, ck;

	if (Pl) {
		pTg = (PLAYER_WORK *)G.pEnemy[1];
		dist = pEm->L_spl & 0x3FFFFFFF;
	}else {

		pTg = &G.Player;
		dist = pEm->L_pl;
	}

	ck = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, Dir);

	if (dist < Range && !ck && pTg->Life >= 0)
		if (!(pTg->Damage_cnt & 0x80))
			return 1;

	return 0;

}

static unsigned long Bodyatck_ck(ENEMY_WORK *pEm)
{

	if (Touch_ck_main(pEm, 0, 0x73A, 0x180)) {
		Snd_se_enem(4, pEm);
		if (Pl_life_down(20, 0) != 2) {
			Em3a_Pl_to_dmg(0, Cdir_ck(pEm, &G.Player));
			Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
			Pad_vib_set1(8, 250, 0);
			return 1;
		}

		Em3a_Pl_to_die(0);
		G.Player.Be_flg |= 2;
		return 2;

	}

	if (G.Status_flg & 0x10000000)
		if (Touch_ck_main(pEm, 1, 0x73A, 0x180)) {
			Snd_se_enem(4, pEm);
			if (Spl_life_down(20, 0) != 2) {
				Em3a_Pl_to_dmg(1, Cdir_ck(pEm, G.pEnemy[1]));
				return 1;
			}

			Em3a_Pl_to_die(1);
			return 2;

		}

	return 0;
}

static void Em3a_Pl_to_dmg(unsigned long pl, unsigned char dir)
{
	register PLAYER_WORK *pPl;

	pPl = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir + 2;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
}

static void Em3a_Pl_to_die(unsigned long pl)
{
	register PLAYER_WORK *pPl;

	pPl = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 3;
	pPl->Routine_1 = 0;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	register ENEMY_WORK *pTg;
	register PLAYER_WORK *pPl;
	SVECTOR Sv;
	register long dir, Id;
	register EM3A_BLOOD_TBL *tbl;

	tbl = &blood_id_tbl[ type ];

	pTg = pEm;

	pPl = ((EM3A_FREE *)ENEMY_FREE(pTg))->Target_pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);

	pP_ptr = pTg->pSin_parts_ptr + parts;

	Id = tbl->Id & 0x7F;

	if (!(tbl->Id & 0x80)) {
		Sv.vx = pP_ptr->Workm.t[0];
		Sv.vy = pP_ptr->Workm.t[1];
		Sv.vz = pP_ptr->Workm.t[2];
		Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
	}else {

		Sv.vx = Sv.vy = Sv.vz = 0;
		Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
	}
}

static void Set_fire(ENEMY_WORK *pEm, unsigned long Type, unsigned long Tbl)
{
	register short *pTbl;
	register EM3A_BLOOD_TBL *pTbl2;
	register PARTS_W *pP_ptr;
	SVECTOR Sv, Sv2;

	pTbl = &fire_ofs_tbl[ Tbl * 4 ];

	pTbl2 = &blood_id_tbl[ Type & 0x7F ];

	pP_ptr = pEm->pSin_parts_ptr + *pTbl++;

	if (!(Type & 0x80)) {
		Sv.vx = *pTbl++;
		Sv.vy = *pTbl++;
		Sv.vz = *pTbl++;
		Esp_call(((pTbl2->Id & 0x7F) << 24) | ((pTbl2->Type + pTbl2->Pal) << 16) | pTbl2->Zoom, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	}else {

		Sv.vx = pP_ptr->Workm.t[0];
		Sv.vy = pEm->Ground;
		Sv.vz = pP_ptr->Workm.t[2];
		Sv2.vx = pTbl[0];
		Sv2.vy = 0;
		Sv2.vz = pTbl[2];
		Set_smoke(pEm, Type & 0x7F, &Sv, &Sv2);
	}
}

static void Set_smoke(ENEMY_WORK *pEm, unsigned long Type, SVECTOR *pBs, SVECTOR *pOf)
{
	register EM3A_BLOOD_TBL *pTbl;
	SVECTOR Sv;
	MATRIX M;

	pTbl = &blood_id_tbl[ Type & 0x7F ];

	RotMatrix(&pEm->Cdir_x, &M);
	ApplyMatrixSV(&M, pOf, &Sv);
	Sv.vx += pBs->vx;
	Sv.vy += pBs->vy;
	Sv.vz += pBs->vz;
	Esp_call(((pTbl->Id & 0x7F) << 24) | ((pTbl->Type + pTbl->Pal) << 16) | pTbl->Zoom, pEm->Cdir_y, &GsIDMATRIX, &Sv);

}

static void Set_schit(ENEMY_WORK *pEm, unsigned long Type)
{
	if (!Type) {
		pEm->Atd[0].Ofs_y = -800;
		pEm->Atd[0].At_h = 1000;
		pEm->Atd[0].W = 800;
		pEm->Atd[0].D = 1400;
	}else {
		pEm->Atd[0].At_h = 2000;
		pEm->Atd[0].Ofs_y = 0;
		pEm->Atd[0].W = 4;
		pEm->Atd[0].D = 4;
	} }
