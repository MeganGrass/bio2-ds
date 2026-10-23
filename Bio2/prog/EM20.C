/* em20.c - reconstruction of D:\Bio2\prog\EM20.C (enemy 20: zombie dog AI).
 * Linked into two overlays from the same source: 0x06 (EM20 at 0x80100004)
 * and 0x24 (at 0x8010d004); only the enemy-number-specific entry names differ
 * (Em120/Em220, Pldie_Em120/220), selected by OVL_24.
 * Types come from the TU's retail SYM preamble (em20.h). */
#include "EM20.H"

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of this translation unit ---- */

/* ---- functions of other translation units ---- */
extern MATRIX GsIDMATRIX;

/* ---- data ---- */

/* Retail life tables (em20_cdemd0_s1 .data 0x80105340..0x8010541F): the first set changed
 * values relative to Beta 2; a second complete set (0x801053B0..) is selected by
 * G.System_flg & 0x40 (Em20_Init @0x80100160). Second-set names are [INFERRED]. */
static unsigned short Em20_life[16] = { 0x77, 0x55, 0x55, 0x55, 0x77, 0x46, 0x55, 0x55, 0x46, 0x55, 0x3B, 0x46, 0x3B, 0x55, 0x3B, 0x46 };
static unsigned short Em20_life_easy[16] = { 0x39, 0x39, 0x52, 0x41, 0x52, 0x41, 0x39, 0x3B, 0x41, 0x39, 0x1E, 0x39, 0x1E, 0x39, 0x1E, 0x45 };
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;
static unsigned short Em2b_life_easy = 0xC8;
static unsigned short Em20_life_flg40[16] = { 0x81, 0x5F, 0x5F, 0x5F, 0x81, 0x50, 0x5F, 0x5F, 0x50, 0x5F, 0x45, 0x50, 0x45, 0x5F, 0x45, 0x50 };
static unsigned short Em20_life_easy_flg40[16] = { 0x43, 0x43, 0x5C, 0x4B, 0x5C, 0x4B, 0x43, 0x45, 0x4B, 0x43, 0x28, 0x43, 0x28, 0x43, 0x28, 0x4F };
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static unsigned char em20_time_tbl00[8] = { 7, 31, 15, 15, 15, 15, 7, 31 };
static unsigned char em20_time_tbl01[8] = { 10, 20, 30, 20, 30, 20, 10, 20 };
static unsigned char em20_time_tbl02[8] = { 15, 30, 45, 60, 60, 45, 30, 15 };
static void (*Em20_routine_0[8])() = {
	&Em20_Init,
	&Em20_Move,
	&Em20_Damage,
	&Em20_Die,
	&Em20_Scenario,
	0,
	0,
	&Em20_Dead
};
static unsigned char em20_routine_tbl[12] = { 0, 1, 2, 4, 10, 12, 14, 13, 15, 0, 0, 0 };
static void (*em20_move_tbl[17])() = {
	&Em20_Move_walk,
	&Em20_Move_yori,
	&Em20_Move_run,
	&Em20_Move_atck,
	&Em20_Move_sleep,
	&Em20_Move_ikaku,
	&Em20_Move_back,
	&Em20_Move_critical,
	&Em20_Move_escape,
	&Em20_Move_floorout,
	&Em20_Move_walk2,
	&Em20_Move_died,
	&Em20_Move_eat,
	&Em20_Move_house,
	&Em20_Move_rush,
	&Em20_Move_bodyatck,
	&Em20_Move_jumpin
};
static long yori_ofs[8] = { 0x400, 0, -0x200, 0, -0x400, 0, 0x200, 0 };
static void (*move_run_tbl[4])() = {
	&Em20_Mv_run00,
	&Em20_Mv_run10,
	&Em20_Mv_run20,
	&Em20_Mv_run30
};
static unsigned short dash_turn_rnd[8] = { 0x20, 0x40, 0x30, 0x40, 0x20, 0x40, 0x30, 0x40 };
static void (*move_atck_tbl[4])() = {
	&Em20_Mv_atck00,
	&Em20_Mv_atck10,
	&Em20_Mv_atck20,
	&Em20_Mv_atck30
};
static void (*move_sleep_tbl[4])() = {
	&Em20_Mv_sleep00,
	&Em20_Mv_sleep10,
	&Em20_Mv_sleep20,
	&Em20_Mv_sleep30
};
static unsigned char pl_chi_parts[4] = { 8, 0, 9, 12 };
static unsigned char house_mot_rnd[8] = { 11, 26, 11, 26, 26, 26, 11, 26 };
static void (*move_rush_tbl[3])() = {
	&Em20_Mv_rush00,
	&Em20_Mv_rush10,
	&Em20_Mv_rush20
};
static void (*move_bodyatck_tbl[4])() = {
	&Em20_Mv_rush00,
	&Em20_Mv_bodyatck10,
	&Em20_Mv_bodyatck20,
	&Em20_Mv_bodyatck30
};
static void (*move_jumpin_tbl[3])() = {
	&Em20_Mv_jumpin00,
	&Em20_Mv_rush10,
	&Em20_Mv_jumpin20
};
/* retail em20_cdemd0_s1 .data 0x8010553C: entries 3/14/17 -> Em20_Dmg_nml2, 16/18 -> Em20_Dmg_gnml */
static void (*em20_dmg_tbl[19])() = {
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml2,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_gnml,
	&Em20_Dmg_fire,
	&Em20_Dmg_H2SO4,
	&Em20_Dmg_nml,
	&Em20_Dmg_nml,
	&Em20_Dmg_spark,
	&Em20_Dmg_nml2,
	&Em20_Dmg_fire,
	&Em20_Dmg_gnml,
	&Em20_Dmg_nml2,
	&Em20_Dmg_gnml
};
static void (*dmg_nml_tbl[5])() = {
	&Em20_Dmg_nml00,
	&Em20_Dmg_nml10,
	&Em20_Dmg_nml20,
	&Em20_Dmg_nml30,
	&Em20_Dmg_nml40
};
static short dir_hosei[8] = { 0x80, -0xC0, -0x80, 0xC0, -0x100, 0x100, 0x100, -0x100 };
static void (*dmg_combo_tbl[4])() = {
	&Em20_Dmg_combo00,
	&Em20_Dmg_combo10,
	&Em20_Dmg_combo20,
	&Em20_Dmg_combo30
};
/* retail-only (.data 0x801055BC, 5 entries): Em20_Dmg_nml2 steps; name [INFERRED] */
static void (*dmg_nml2_tbl[5])() = {
	&Em20_Dmg_nml2_00,
	&Em20_Dmg_nml10,
	&Em20_Dmg_nml20,
	&Em20_Dmg_nml30,
	&Em20_Dmg_nml40
};
static void (*em20_die_tbl[19])() = {
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_gnml,
	&Em20_Die_gnml,
	&Em20_Die_bang,
	&Em20_Die_bang,
	&Em20_Die_gnml,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_spark,
	&Em20_Die_nml,
	&Em20_Die_nml,
	&Em20_Die_gnml,	/* retail image word 0x80104610 = Em20_Die_gnml (Beta 2: Em20_Die_bang) */
	&Em20_Die_nml,
	&Em20_Die_gnml	/* retail image word 0x80104610 = Em20_Die_gnml (Beta 2: Em20_Die_bang) */
};
static void (*die_nml_init[19])() = {
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_gnml,
	&Em20_Die_gnml,
	&Em20_Die_bang,
	&Em20_Die_bang,
	&Em20_Die_gnml,
	&Em20_Die_fire,
	&Em20_Die_H2SO4,
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_nml00,
	&Em20_Die_fire,
	&Em20_Die_gnml,
	&Em20_Die_nml00,
	&Em20_Die_gnml
};
static void (*die_nml_tbl[4])() = {
	&Em20_Die_nml00,
	&Em20_Dmg_nml10,
	&Em20_Die_nml20,
	&Em20_Dmg_nml40
};
static void (*die_bang_tbl[2])() = {
	&Em20_Die_bang00,
	&Em20_Die_bang10
};
static unsigned char bang_parts_no[8] = { 2, 3, 4, 7, 8, 9, 10, 0 };
static void (*die_gnml_tbl[4])() = {
	&Em20_Die_gnml00,
	&Em20_Dmg_nml10,
	&Em20_Die_nml20,
	&Em20_Dmg_nml40
};
static void (*die_spark_tbl[4])() = {
	&Em20_Die_spark00,
	&Em20_Dmg_nml10,
	&Em20_Die_nml20,
	&Em20_Dmg_nml40
};
static unsigned char mot_tbl[4] = { 13, 2, 12, 0 };
static EM20_BLOOD_TBL blood_id_tbl[13] = {
	{ 0, 1, 0, 0x1000 },
	{ 9, 0, 0x200, 0x800 },
	{ 9, 1, 0x200, 0x800 },
	{ 9, 2, 0x200, 0x800 },
	{ 0, 1, 0x400, 0x1000 },
	{ 0, 1, 0x200, 0x1000 },
	{ 0, 21, 0, 0x400 },	/* retail Zoom 0x400 (image halfword at .data+0x394; Beta 2: 0x800) */
	{ 133, 3, 0, 0x1000 },
	{ 133, 4, 0, 0x1000 },
	{ 132, 15, 0, 0x1400 },
	{ 132, 15, 0, 0xC00 },
	{ 134, 0, 0, 0x1000 },
	{ 132, 12, 0, 0x1000 }
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7f)
			pEm->Damage_cnt--;

		if (((EM20_FREE *)OVL_FREE(pEm))->No_fire)
			((EM20_FREE *)OVL_FREE(pEm))->No_fire--;

		Em20_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);

		if ((pEm->Pos_x == pEm->Old_x && pEm->Pos_z == pEm->Old_z) || pEm->At_em_no != 0xff) {

			((EM20_FREE *)OVL_FREE(pEm))->Stay++;
		}else

			((EM20_FREE *)OVL_FREE(pEm))->Stay = 0;

	}

}

static void Em20_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register EM20_FREE *pFp;
	register unsigned long i;

	*(long *)&pEm->Routine_0 = 1;

	pFp = (EM20_FREE *)OVL_FREE(pEm);

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++) /* retail clears 12 words @0x80100144 */
		OVL_FREE(pEm)[ i ] = 0;

	if (G.System_flg & 0x40)
		pEm->Life = ((G.System_flg & 0x20) ? Em20_life_easy_flg40[ Rnd() & 0xf ] : Em20_life_flg40[ Rnd() & 0xf ]) + (Rnd() & 3);
	else
		pEm->Life = ((G.System_flg & 0x20) ? Em20_life_easy[ Rnd() & 0xf ] : Em20_life[ Rnd() & 0xf ]) + (Rnd() & 3);

	pEm->D_life_u = pEm->D_life_c = pEm->D_life_d = 13;

	Kage_work_set(&pEm->pKage_work, 0, 0x25804b0, 0, &pEm->Pos_x);

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 500;
	pEm->Atd[0].Ofs_y = -1000;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[1].At_w = 600;
	pEm->Atd[0].At_d = 600;
	pEm->Atd[0].At_h = 1000;
	pEm->Atd[0].W = 600;
	pEm->Atd[0].D = 600;
	pEm->Eff_at_r = 600;

	pEm->pEnemy_ptr = (unsigned long *)&G.Player;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 2;

	for (i = 2; i < 4; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		pP_ptr->Free[0] = 0;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[2] = 0x20;
		pP_ptr->Free[3] = 0;
		pP_ptr->Free[4] = 0x180;
		pP_ptr->Free[5] = 0x80;
	}

	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	pEm->Routine_1 = em20_routine_tbl[ pEm->Type & 0x7f ];
	switch (pEm->Type) {
	case 0: case 1: case 4:

		*(long *)&pEm->Move_no = ((Rnd() & 0x3f) << 8) + 0xf0001;

		pEm->Be_flg |= 0xc000000;	break;

	case 2: case 6:

		*(long *)&pEm->Move_no = ((Rnd() & 0xf) << 8) + 0xf0002;

		pEm->Be_flg |= 0xc000000;	break;

	case 0x2003:
		*(long *)&pEm->Move_no = 0xf0006;
		pEm->Be_flg |= 0x4000000;
		break;

	case 7:
		*(long *)&pEm->Move_no = ((Rnd() & 0x1f) << 8) + 0xf000b;
		pEm->Timer2 = (Rnd() & 3) + 1;
		pEm->Neck_flg &= 0xfd;
		pFp->House_flg = 1;
		pEm->Routine_3 = 1;

	case 8:
		*(long *)&pEm->Move_no = 0xf0000;
		pFp->House_flg = 1;
		pEm->Be_flg |= 0x8000000;
		break;

	case 9:
		*(long *)&pEm->Move_no = ((Rnd() & 0x3f) << 8) + 0xf0001;
		pFp->Outside = 1;

	default:
		pEm->Be_flg |= 0xc000000;
	}

	Joint_move(pEm, Kan, Seq, 0x100);

	G.pPldie_Em[16] = PLDIE_EM;

}

static void Em20_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	em20_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	if (*pEm->pNow_seq & 0x40000) {
		Snd_se_enem(8, pEm);

	}

}

static unsigned long Em20_rndwalk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;
	register unsigned long ck = 0;

	pFp = (EM20_FREE *)OVL_FREE(pEm);

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		pEm->Spd_x = 30;
		pEm->Spd_z = 0;
		pEm->Neck_flg |= 2;
		pEm->Be_flg &= 0xefffffff;
		pEm->Be_flg |= 0x4000000;
		pFp->Jump_flg = 0;
		pFp->House_flg = 0;
		pEm->Damage_cnt &= 0x7f;
		break;

	case 1:
		Add_speedXZ(pEm, 0);
		if (pEm->Sca_info & 1) {

			pEm->Routine_2 = !(Rnd() & 0x1f) ? 2 : 3;

			pFp->Area_keep = Rnd_area();
			if (pEm->Routine_2 == 2) {
				pEm->Timer1 = 90;
				*(long *)&pEm->Move_no = 0xf0000;
			}
		}
		break;

	case 2:
		if (--pEm->Timer1 == 0) {
			pEm->Routine_2 = 3;
			*(long *)&pEm->Move_no = 0xf0001;
		}
		break;

	case 3:
		ck = Root_ck(pEm, 0, pFp->Area_keep, 1);
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
		Add_speedXZ(pEm, 0);
		if (!(pEm->Sca_info & 1))
			pEm->Routine_2 = 1;

	}

	Joint_move(pEm, Kan, Seq, 0x100);
	Dog_rot_neck(pEm);

	if (!pFp->Blood && !(Rnd() & 0x3f))
		pFp->Blood = em20_time_tbl00[ Rnd() & 7 ];

	Set_blood(pEm, 4, 6);

	return ck;
}

static void Em20_Move_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long ck;

	ck = Em20_rndwalk(pEm, Kan, Seq);

	if (G.Player.nFloor == pEm->nFloor) {
		if (pEm->L_pl < 4000 || ck == 1 || (G.Room_enemy_flg & 0x20)) {
			pEm->Routine_1 = 1;
			*(unsigned short *)&pEm->Routine_2 = 0;

		}else if (G.Room_player_flg & 1) {
			pEm->Routine_1 = 2;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}
	}

	Ck_Died(pEm);
}

static void Em20_Move_yori(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pTg;
	VECTOR targ;
	short d;

	if (pEm->Routine_2 == 0) {
		pEm->Routine_2 = 1;
		pEm->Spd_x = 30;
		pEm->Spd_z = 0;
		pEm->Neck_flg &= 0xfd;
		pEm->Timer2 = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		((EM20_FREE *)OVL_FREE(pEm))->House_flg = 0;
		pEm->Damage_cnt &= 0x7f;

		pEm->Be_flg &= 0xefffffff;
		pEm->Be_flg |= 0x4000000;

		((EM20_FREE *)OVL_FREE(pEm))->Yori_fx = yori_ofs[ Rnd() & 7 ];
		((EM20_FREE *)OVL_FREE(pEm))->Yori_fz = yori_ofs[ Rnd() & 7 ];
	}

	pTg = Get_target_pl(pEm);
	targ.vx = pTg->Pos_x + ((EM20_FREE *)OVL_FREE(pEm))->Yori_fx;
	targ.vy = pTg->Pos_y;
	targ.vz = pTg->Pos_z + ((EM20_FREE *)OVL_FREE(pEm))->Yori_fz;
	Root_ck(pEm, &targ, 0, 0);
	d = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
	pEm->Cdir_y += d;
	Add_speedXZ(pEm, 0);

	if (pEm->L_pl < 3000) {
		pEm->Timer2++;

		if (G.Room_player_flg & 4)
			pEm->Timer2 += 20;

		if (!Dir_pos_ck(&pEm->Pos_x, &pTg->Pos_x, pEm->Cdir_y, 0x80) && pEm->L_pl > 600) {

			pEm->Routine_1 = 3;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}
	}else {

		if (*(signed char *)&pEm->Timer2)
			pEm->Timer2--;

		if (!Dir_pos_ck(&pEm->Pos_x, &pTg->Pos_x, pEm->Cdir_y, 0x80) && (G.Room_player_flg & 4))

			pEm->Timer2 += 20;

	}

	if ((signed char)pEm->Timer2 > 64 || (G.Room_player_flg & 1) || (G.Room_enemy_flg & 0x20)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		pEm->Timer2 = 0;
	}

	Joint_move(pEm, Kan, Seq, 0x100);
	Dog_rot_neck(pEm);

	if (!((EM20_FREE *)OVL_FREE(pEm))->Blood && !(Rnd() & 0x3f))
		((EM20_FREE *)OVL_FREE(pEm))->Blood = em20_time_tbl00[ Rnd() & 7 ];

	Set_blood(pEm, 4, 6);

	Ck_Floor(pEm);
	Ck_Died(pEm);
}

static void Em20_Move_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_run_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Joint_move(pEm, Kan, Seq, 0x100);

	Dog_rot_neck(pEm);

	Ck_Floor(pEm);
	Ck_Died(pEm);	}

static void Em20_Mv_run00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	pEm->Routine_2 = 1;

	*(long *)&pEm->Move_no = 0xf0002;

	((EM20_FREE *)OVL_FREE(pEm))->No_hit = 0;
	((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
	((EM20_FREE *)OVL_FREE(pEm))->House_flg = 0;
	pEm->Damage_cnt &= 0x7f;

	pEm->Neck_flg &= 0xfd;

	pEm->Be_flg &= 0xefffffff;
	pEm->Be_flg |= 0xc000000;

	((EM20_FREE *)OVL_FREE(pEm))->Dash_turn = 0x60;
	if (Ck_em_id(pEm, 0, 0x20, 0)) {
		((EM20_FREE *)OVL_FREE(pEm))->Dash_turn = dash_turn_rnd[ Rnd() & 7 ];
	}

	if (((EM20_FREE *)OVL_FREE(pEm))->Outside) {
		*(unsigned short *)&pEm->Routine_2 = 3;
		((EM20_FREE *)OVL_FREE(pEm))->Area_keep = Rnd_area();
		((EM20_FREE *)OVL_FREE(pEm))->No_hit = 1;
		((EM20_FREE *)OVL_FREE(pEm))->Stay = 0;
		pEm->Timer1 = 0;
	}
}

static void Em20_Mv_run10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;
	register ENEMY_WORK *pTg;
	register long ck, d_tmp = 0;

	Rate_speed_up(pEm, 40, 240);
	pFp = (EM20_FREE *)OVL_FREE(pEm);
	if (!pFp->Fix_cnt) {
		pTg = Get_target_pl(pEm);
		Root_ck(pEm, &pTg->Pos_x, 0, 0);
		d_tmp = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, pFp->Dash_turn);
		if (pFp->Stay >= 16 && !d_tmp)
			d_tmp = pFp->Dash_turn;

		pEm->Cdir_y += d_tmp;
	}else {

		if (!--pFp->Fix_cnt || (pEm->Sca_info & 1)) {
			pFp->Fix_cnt = 0;
			pFp->Em_next = 0;
		}
	}
	Add_speedXZ(pEm, 0);
	Set_run_motion(pEm, d_tmp);

	if (ck = Atck_range_ck(pEm)) {
		if (ck < 0) {
			pFp->Stress += 10;
			if (ck == -2) {

				pEm->Routine_1 = 8;	*(unsigned short *)&pEm->Routine_2 = 0;	return;
			}
			if (ck == -4)
				pFp->Spin_flg = 1;

		}else {

			pEm->Routine_1 = 3;

			*(unsigned short *)&pEm->Routine_2 = 0;	return;
		}
	}

	if (pFp->Stress > 450) {
		if (pTg->Life > 20)
			pFp->Stress = 0;

		pFp->Spin_flg = 1;
	}

	if (pFp->Spin_flg) {
		pFp->Spin_flg = 0;
		*(unsigned short *)&pEm->Routine_2 = 2;
	}
}

static void Em20_Mv_run20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned short dir;
	ENEMY_WORK *pTg;

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Timer0 = 0;
	}

	pTg = Get_target_pl(pEm);
	Root_ck(pEm, &pTg->Pos_x, 0, 0);
	dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);

	Add_speedXZ(pEm, -pEm->Timer0);
	Set_run_motion(pEm, dir);

	if (pEm->Routine_3 == 1) {
		Rate_speed_down(pEm, 6, 10);
		if (pEm->Spd_x < 11)
			pEm->Routine_3++;

	}else

		Rate_speed_up(pEm, 6, 240);

	pEm->Cdir_y += dir;
	pEm->Timer0 += dir;
	if ((pEm->Timer0 >= 0 ? pEm->Timer0 : -pEm->Timer0) >= 0x800 || !dir)
		pEm->Routine_2 = 1;

}

static void Em20_Mv_run30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;
	register PLAYER_WORK *pTg;
	register long ck, d0, d1;

	pFp = (EM20_FREE *)OVL_FREE(pEm);
	pTg = (pFp->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (pFp->No_atck)
		pFp->No_atck--;

	Rate_speed_up(pEm, 40, 240);

	ck = Root_ck(pEm, 0, pFp->Area_keep, 1);

	d0 = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
	d1 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_y, 0x80);

	if (pEm->Timer1) {
		d0 = (signed char)pEm->Timer2;
		pEm->Timer1--;
	}else

	if (((EM20_FREE *)OVL_FREE(pEm))->Stay >= 16) {
		pEm->Timer1 = 30;
		if (d0)

			pEm->Timer2 = -d0;
		else
			pEm->Timer2 = -(d1 >> 1);
	}

	pEm->Cdir_y += d0;

	Add_speedXZ(pEm, 0);
	Set_run_motion(pEm, d0);

	if (!pEm->Routine_3 || (ck == 1 && SquareRoot0((pEm->Pos_x - pEm->Dest_x) * (pEm->Pos_x - pEm->Dest_x) + (pEm->Pos_z - pEm->Dest_z) * (pEm->Pos_z - pEm->Dest_z)) < 1000)) {

		pEm->Routine_3 = 1;
		ck = pFp->Area_keep;
		pFp->Area_keep = Rnd_area();
		if (ck == pFp->Area_keep)
			pFp->Area_keep++;

		pFp->Area_keep %= *G.pRoom->pBlock;
	}

	if (!pFp->No_atck && (Atck_range_ck(pEm) || !d1)) {
		pEm->Routine_1 = 3;
		*(unsigned short *)&pEm->Routine_2 = 0;
		pFp->No_atck = em20_time_tbl02[ Rnd() & 7 ] + 30;

	}
}

static void Em20_Move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_atck_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Dog_rot_neck(pEm);

	Set_blood(pEm, 4, 0);	}

static void Em20_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;

		Jump_set(pEm, 0x41, -0x118);

		pEm->Neck_flg |= 2;

		if (pEm->Move_no == 12) {
			pEm->Cdir_y -= 0x20;
		}else {

		if (pEm->Move_no == 13)
			pEm->Cdir_y += 0x20;	}
	}

	Add_speedXZ(pEm, 0);

	Joint_move(pEm, Kan, Seq, 0x200);

	if (!--pEm->Timer0) {
		*(unsigned short *)&pEm->Routine_2 = 1;

		pEm->Spd_x = 0x118;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 1;
		((EM20_FREE *)OVL_FREE(pEm))->Bang_ok = 1;

		pEm->Be_flg |= 0x10000000;
		pEm->Be_flg &= 0xfbffffff;

		Snd_se_enem(3, pEm);
		G.Room_enemy_flg |= 0x20;
	}
}

static void Jump_set(ENEMY_WORK *pEm, long spd_x, long spd_y)
{
	pEm->Spd_x = spd_x;
	pEm->Spd_y = spd_y;
	pEm->Timer0 = 8;

	*(long *)&pEm->Move_no = 0x70014;

	pEm->Be_flg &= 0xefffffff;
	pEm->Be_flg |= 0x4000000;

	((EM20_FREE *)OVL_FREE(pEm))->Hit_type = 0;
	((EM20_FREE *)OVL_FREE(pEm))->Blood = 0;	}

static void Em20_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long ck;

	ck = Em20_Mv_jump10(pEm, Kan, Seq);

	if (ck > 0) {
		/* retail @0x801013E8: vibration on the leap hit */
		Pad_vib_set0(5, 0);
		Pad_vib_set1(5, 200, 0);
	}else {

		if (ck < 0) {
			if (((EM20_FREE *)OVL_FREE(pEm))->Hit_type == 2) {
				pEm->Routine_1 = 7;
				*(unsigned short *)&pEm->Routine_2 = 0;
				return;
			}
			*(unsigned short *)&pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0xf0015;
			Joint_move(pEm, Kan, Seq, 0x100);
			((EM20_FREE *)OVL_FREE(pEm))->Stress += 20;
			((EM20_FREE *)OVL_FREE(pEm))->Bang_ok = 0;
		}
	}

}

static long Em20_Mv_jump10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long d0;

	Add_speedXYZ(pEm, 0, 0);
	pEm->Spd_y += 40;

	d0 = Atck_hit_ck(pEm) == 1;

	if (pEm->Spd_y >= 0 && !d0) {

		d0 = -1;	}else

	if (pEm->Move_cnt)
		Joint_move(pEm, Kan, Seq, 0x200);

	return d0;
}

static void Em20_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Em20_Mv_jump20(pEm, Kan, Seq)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Fix_cnt = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Hit_type = 0;
	}	}

static long Em20_Mv_jump20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		Add_speedXYZ(pEm, 0, 0);
		pEm->Spd_y += 40;

		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Routine_3 = 1;

			pEm->Pos_y = pEm->Ground;
			((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;

			pEm->Be_flg &= 0xefffffff;
			pEm->Be_flg |= 0x4000000;

			Snd_se_enem(10, pEm);
		}

		if (pEm->Move_cnt != 8)
			Joint_move(pEm, Kan, Seq, 0x100);

	}else {

		Add_speedXZ(pEm, 0);
		Rate_speed_down(pEm, 0x40, 240);

		if (Joint_move(pEm, Kan, Seq, 0x100))
			return 1;

	}

	return 0;
}

static void Em20_Mv_atck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		*(long *)&pEm->Move_no = 0xf0016;
		pEm->Spd_x >>= 2;
		pEm->Timer0 = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Hit_type = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Blood = 4;

	case 1:
		Add_speedXYZ(pEm, 0x800, 0);
		pEm->Spd_y += 40;
		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Routine_3 = 2;
			pEm->Pos_y = pEm->Ground;
			((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;

			pEm->Be_flg &= 0xefffffff;
			pEm->Be_flg |= 0x4000000;

			Snd_se_enem(10, pEm);
		}

		if (pEm->Move_cnt <= 16) {
			Joint_move(pEm, Kan, Seq, 0x100);
			break;
		}

		pEm->Spd_x = 0;	break;

	case 2:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_1 = 2;
			*(unsigned short *)&pEm->Routine_2 = 0;
			((EM20_FREE *)OVL_FREE(pEm))->Fix_cnt = em20_time_tbl00[ Rnd() & 7 ];
		}

	}

	if (pEm->Timer0 < 0x800) {
		pEm->Cdir_y += 0x80;
		pEm->Timer0 += 0x80;
	}
}

static void Em20_Move_sleep(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_sleep_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em20_Mv_sleep00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register unsigned long range;

	pPl = (((EM20_FREE *)OVL_FREE(pEm))->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	range = (*(unsigned short *)&pPl->Routine_0 == 0x201) ? 5000 : 3500;

	if (pEm->L_pl <= range) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0xf0008;
	}else

	if ((G.Room_player_flg & 1) || (G.Room_enemy_flg & 0x20)) {
		if (!(G.Room_enemy_flg & 0x80)) {
			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0xf0008;
			G.Room_enemy_flg |= 0x80;
		}else {

			pEm->Timer0 = em20_time_tbl02[ Rnd() & 7 ];
			pEm->Routine_2 = 1;
		}
	}

	Joint_move(pEm, Kan, Seq, 0x100);

}

static void Em20_Mv_sleep10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!--pEm->Timer0) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0xf0008;
	}
	Joint_move(pEm, Kan, Seq, 0x100);	}

static void Em20_Mv_sleep20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Joint_move(pEm, Kan, Seq, 0x100)) {
		pEm->Be_flg |= 0x8000000;
		pEm->Type &= 0xdfff;
		*(unsigned short *)&pEm->Routine_2 = 3;
	}	}

static void Em20_Mv_sleep30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Timer0 = em20_time_tbl01[ Rnd() & 7 ];
		*(long *)&pEm->Move_no = 0x1f0000;
	}

	if (!--pEm->Timer0) {

		*(long *)&pEm->Move_no = 0xf0001;
		pEm->Routine_1 = 1;
		*(unsigned short *)&pEm->Routine_2 = 0;
	}

	Joint_move(pEm, Kan, Seq, 0x80);
	Dog_rot_neck(pEm);
}

static void Em20_Move_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x30003;
		pEm->Neck_flg &= 0xfd;
		((EM20_FREE *)OVL_FREE(pEm))->Blood = 0x20;
	}

	if (Joint_move(pEm, Kan, Seq, 0x400)) {
		if (pEm->Move_no != 4) {
			*(long *)&pEm->Move_no = pEm->Move_no + 0x30001;
			if (pEm->Move_no == 4)
				{ Snd_se_enem(4, pEm); }

		}else {

			*(long *)&pEm->Routine_0 = 0x201;
		}
	}

	Dog_rot_neck(pEm);

	Set_blood(pEm, 4, 6);
}

static void Em20_Move_back(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = (((EM20_FREE *)OVL_FREE(pEm))->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Spd_x = 50;
		*(long *)&pEm->Move_no = 0x30009;
		pEm->Neck_flg &= 0xfd;
	}

	if (pEm->Move_cnt < 15) {
		if (pEm->Move_cnt >= 8)
			Rate_speed_down(pEm, 32, 30);

		Add_speedXZ(pEm, 0x800);
		Goto00(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
	}

	if (Joint_move(pEm, Kan, Seq, 0x400))
		*(long *)&pEm->Routine_0 = 0x201;

	Dog_rot_neck(pEm);

}

static void Em20_Move_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em20_Mv_critical00(pEm, Kan, Seq);
	}else {

		Em20_Mv_critical10(pEm, Kan, Seq);
	}	}

static void Em20_Mv_critical00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register EM20_FREE *pFp;
	register PARTS_W *pP_ptr;
	register unsigned long i;
	register unsigned char Ofs;	/* retail-only local, [INFERRED] name (as EM10_S0.C): Dual Shock strength offset */

	pPl = &G.Player;

	pFp = (EM20_FREE *)OVL_FREE(pEm);
	if (!pEm->Routine_3) {

		pEm->Routine_3 = 1;
		((EM20_FREE *)OVL_FREE(pEm))->Em_next = 0;
		pEm->Spd_x = 0;
		pEm->Timer2 = 12;
		*(long *)&pEm->Move_no = 0x1f0017;
		Joint_move(pEm, Kan, Seq, 0x80);
		((EM20_FREE *)OVL_FREE(pEm))->Blood = 0;

		pPl->pEnemy_ptr = (unsigned long *)pEm;
		Set_base_pos(pPl, Kan, Seq, 0);
		*(long *)&pPl->Routine_0 = 6;
		pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		pPl->Damage_cnt |= 0x80;
		pPl->Cdir_y = pEm->Cdir_y + 0x800;
		/* retail-only (Dual Shock): vibration pattern for the critical bite (retail 0x80101DA8..0x80101EB8) */
		Ofs = (G.System_flg & 0x800) ? 15 : 0;
		Pad_vib_fade_set(0x1e, 0x32, 0x96, 0);
		Pad_vib_set1(8, 0xfa, 0x1e - Ofs);
		Pad_vib_set0(5, 0x32 - Ofs);
		Pad_vib_set1(0x3c, 0x78, 0x32 - Ofs);
		Pad_vib_set0(5, 0x46 - Ofs);
		Pad_vib_set0(5, 0x55 - Ofs);
		Pad_vib_set0(0x14, 0x64 - Ofs);
		Pad_vib_fade_set(8, 0x64, 0xfa, 0x71 - Ofs);
		Pad_vib_fade_set(0x3c, 0xfa, 0, 0x79 - Ofs);
	}else {

		Set_base_pos(pPl, Kan, Seq, 0);

		Null_pos_set(pEm, Kan, Seq, 0);

		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(unsigned short *)&pEm->Routine_2 = 1;
			*(long *)&pEm->Move_no = 0x1f0018;
			Spd_get(pEm, Kan, Seq, 0);
			Joint_move(pEm, Kan, Seq, 0x80);

			((EM20_FREE *)OVL_FREE(pEm))->Blood = 0;	return;
		}

		if (pEm->Move_cnt == 0x62) {
			((EM20_FREE *)OVL_FREE(pEm))->Blood = 0x12;

			pP_ptr = &pEm->pSin_parts_ptr[4];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x60;
			*(long *)&pP_ptr->Free[2] = 0x101010;
		}

		if (pEm->Move_cnt == 0x66) {

			for (i = 0; i < 4; i++) {
				pP_ptr = &G.Player.pSin_parts_ptr[ pl_chi_parts[ i ] ];
				pP_ptr->Be_flg |= 0x80;
				*(long *)&pP_ptr->Free[0] = 0x60;
				*(long *)&pP_ptr->Free[2] = 0x101010;
			}
		}

	}

	if (!*(signed char *)&pEm->Timer2 || (pPl->Id & 1)) {
		if (!pFp->Em_next) {
			pFp->Em_next = 1;
			G.System_flg |= 0x100;
			G.System_flg |= 0x4000000;
			G.Room_enemy_flg |= 0x40;
		}
		Add_speedXYZ(pEm, 0, 0);
		pEm->Spd_y += 40;

		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		}

		if (pPl->Id & 1)
			pEm->Timer2--;

	}else

		pEm->Timer2--;

	Set_blood(pEm, 4, 0);
	Set_blood(pEm, 8, 4);
	Set_blood(pEm, 8, 4);

	Em_seq_sound(pEm);
}

static void Em20_Mv_critical10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long eos;

	if (!pEm->Routine_3) {
		Spd_get(pEm, Kan, Seq, 0);
		eos = Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);
		if (eos) {
			pEm->Routine_3 = 1;
			pEm->Timer0 = 2;
			*(long *)&pEm->Move_no = 0x1f0019;
			Spd_get(pEm, Kan, Seq, 0);
			Joint_move(pEm, Kan, Seq, 0x80);
		}
	}else {

		Spd_get(pEm, Kan, Seq, 0);
		eos = Joint_move(pEm, Kan, Seq, 0x80);

		if (eos)
			if (!--pEm->Timer0) {
				pEm->Routine_1 = 12;
				*(unsigned short *)&pEm->Routine_2 = 0;
			}

	}

	Em_seq_sound(pEm);

}

static void Em20_Move_escape(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long d_tmp;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		((EM20_FREE *)OVL_FREE(pEm))->Area_keep = Rnd_area();
		pEm->Timer0 = em20_time_tbl01[ Rnd() & 7 ] + 30;
		((EM20_FREE *)OVL_FREE(pEm))->Stress += 300;
	}

	Get_target_pl(pEm);
	Root_ck(pEm, 0, ((EM20_FREE *)OVL_FREE(pEm))->Area_keep, 1);
	d_tmp = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, ((EM20_FREE *)OVL_FREE(pEm))->Dash_turn);
	pEm->Cdir_y += d_tmp;
	Add_speedXZ(pEm, 0);

	if (!--pEm->Timer0) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Fix_cnt = 0;
	}

	Set_run_motion(pEm, d_tmp);
	Joint_move(pEm, Kan, Seq, 0x100);

	Dog_rot_neck(pEm);

	Ck_Floor(pEm);
}

static void Em20_Move_floorout(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Timer0 = 60;
		*(long *)&pEm->Move_no = 0xf0000;
	}

	if (!--pEm->Timer0) {
		pEm->Routine_1 = 0;
		*(unsigned short *)&pEm->Routine_2 = 0;
		*(long *)&pEm->Move_no = 0xf0001;
	}

	Joint_move(pEm, Kan, Seq, 0x100);

	Dog_rot_neck(pEm);	}

static void Em20_Move_walk2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em20_rndwalk(pEm, Kan, Seq);

	if (G.Player.nFloor == pEm->nFloor)
		if (pEm->L_pl < 4000 || (G.Room_enemy_flg & 0x20) || (G.Room_player_flg & 1)) {

			pEm->Routine_1 = 2;
			*(unsigned short *)&pEm->Routine_2 = 0;

		}	}

static void Em20_Move_died(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pTg;

	pTg = G.pEnemy[1];
	if (!(G.Status_flg & 0x10000000) || pTg->Life)
		pTg = (ENEMY_WORK *)&G.Player;

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		pEm->Spd_x = 30;
		pEm->Spd_z = 0;
		pEm->Neck_flg &= 0xfd;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		((EM20_FREE *)OVL_FREE(pEm))->House_flg = 0;
		*(long *)&pEm->Move_no = ((Rnd() & 0x3f) << 8) + 0xf0001;

	case 1:
		Root_ck(pEm, &pTg->Pos_x, 0, 0);
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
		Add_speedXZ(pEm, 0);
		if ((!Dir_pos_ck(&pEm->Pos_x, &pTg->Pos_x, pEm->Cdir_y, 0x100) && pEm->L_pl < 2500) || pEm->At_em_no != 0xff) {

			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0xf0000;
		}

	case 2:
		break;

	}

	Joint_move(pEm, Kan, Seq, 0x100);
	Dog_rot_neck(pEm);

}

static void Em20_Move_eat(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em20_mv_eat00(pEm, Kan, Seq);
	}else {

		Em20_Mv_sleep30(pEm, Kan, Seq);
	}	}

static void Em20_mv_eat00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long chr_type;
	register unsigned long range;

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		((EM20_FREE *)OVL_FREE(pEm))->House_flg = 0;
		*(long *)&pEm->Move_no = ((Rnd() & 0x3f) << 8) + 0x1f000e;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x80);
		break;

	case 1:
		if (G.Player.Life >= 0) {

			range = (*(unsigned short *)&G.Player.Routine_0 == 0x201) ? 5000 : 3500;

			if (pEm->L_pl <= range) {
				*(unsigned short *)&pEm->Routine_2 = 1;
			}else

			if (G.Room_player_flg & 1) {
				if (!(G.Room_enemy_flg & 0x80)) {
					*(unsigned short *)&pEm->Routine_2 = 1;
					G.Room_enemy_flg |= 0x80;
				}else {

					pEm->Routine_3 = 2;
					pEm->Timer0 = em20_time_tbl02[ Rnd() & 7 ];
				}
			}
		}
		break;

	case 2:
		if (!--pEm->Timer0)
			*(unsigned short *)&pEm->Routine_2 = 1;

	}

	chr_type = *pEm->pNow_seq;
	if (chr_type & 0x30000) {
		((EM20_FREE *)OVL_FREE(pEm))->Blood = 1;
		Set_blood(pEm, 4, 0);
		if ((chr_type & 0x20000) && !(Rnd() & 3)) {
			((EM20_FREE *)OVL_FREE(pEm))->Blood = 1;
			Set_blood(pEm, 4, (Rnd() & 1) + 1);
		}

	}
	Em_seq_sound(pEm);

	Spd_get(pEm, Kan, Seq, 0);
	Joint_move(pEm, Kan, Seq, 0x80);
	Add_speedXZ(pEm, 0);

}

static void Em20_Move_house(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em20_mv_house00(pEm, Kan, Seq);
	}else {

		Em20_mv_house10(pEm, Kan, Seq);
	}

	Dog_rot_neck(pEm);	}

static void Em20_mv_house00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;
		pEm->Neck_flg &= 0xfd;
		*(long *)&pEm->Move_no = house_mot_rnd[ Rnd() & 7 ] + 0xf0000;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Timer2 = (Rnd() & 3) + 1;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			if (!--*(signed char *)&pEm->Timer2) {
				*(unsigned short *)&pEm->Routine_2 = 1;
			}else

				*(long *)&pEm->Move_no = house_mot_rnd[ Rnd() & 7 ] + 0xf0000;
		}

		if (pEm->Move_no == 11) {
			Add_speedXZ(pEm, 0);
		}

	}

	Em_seq_sound(pEm);

}

static void Em20_mv_house10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		pEm->Spd_x = 240;
		pEm->Timer0 = em20_time_tbl01[ Rnd() & 7 ];
		pEm->Timer1 = (Rnd() & 1) ? 0x7f : -0x7f;
		if (Rnd() & 1)
			pEm->Timer1 = -pEm->Timer1;

		*(long *)&pEm->Move_no = 0xf0002;

	case 1:
		pEm->Cdir_y += pEm->Timer1;
		if (!--pEm->Timer0)
			pEm->Routine_3 = 2;

		break;

	case 2:
		pEm->Cdir_y += pEm->Timer1;
		pEm->Cdir_y &= 0xfff;
		if (pEm->Cdir_y >= 0x780 && pEm->Cdir_y <= 0x880) {
			if ((0x1b68 >> (Rnd() & 0xf)) & 1) {
				pEm->Routine_1 = 15;
				*(unsigned short *)&pEm->Routine_2 = 0x100;
			}else {

				pEm->Routine_3 = 3;

				pEm->Timer1 = 0;	}
		}
		break;

	case 3:
		if (pEm->Sca_info & 1)
			*(unsigned short *)&pEm->Routine_2 = 0;

	}

	Add_speedXZ(pEm, 0);
	Set_run_motion(pEm, pEm->Timer1);
	Joint_move(pEm, Kan, Seq, 0x100);
}

static void Em20_Move_rush(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_rush_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Dog_rot_neck(pEm);	}

static void Em20_Mv_rush00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 2;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		((EM20_FREE *)OVL_FREE(pEm))->House_flg = 0; /* retail: sb $zero,0x22D($s0) @0x80102CE0 (Free+0x15) */
		break;

	case 1:
		pEm->Routine_3 = 2;
		Jump_set(pEm, 0x41, -0x140);
		pEm->Neck_flg &= 0xfd;

	case 2:
		Add_speedXZ(pEm, 0);

		Joint_move(pEm, Kan, Seq, 0x200);

		if (!--pEm->Timer0) {
			*(unsigned short *)&pEm->Routine_2 = 1;

			pEm->Spd_x = 0x118;
			((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 1;
			Snd_se_enem(3, pEm);
		}

	}
}

static void Em20_Mv_rush10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	((EM20_FREE *)OVL_FREE(pEm))->No_hit = 1;
	if (Em20_Mv_jump10(pEm, Kan, Seq) < 0) {
		*(unsigned short *)&pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0xf0015;
		Joint_move(pEm, Kan, Seq, 0x100);
	}	}

static void Em20_Mv_rush20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Em20_Mv_jump20(pEm, Kan, Seq)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		pEm->Damage_cnt &= 0x7f;
		((EM20_FREE *)OVL_FREE(pEm))->Fix_cnt = 0;
		((EM20_FREE *)OVL_FREE(pEm))->No_hit = 0;
	}	}

static void Em20_Move_bodyatck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_bodyatck_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
	Dog_rot_neck(pEm);	}

static void Em20_Mv_bodyatck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	((EM20_FREE *)OVL_FREE(pEm))->No_hit = 1;
	if (Em20_Mv_jump10(pEm, Kan, Seq) < 0) {
		*(unsigned short *)&pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0xf0015;
		Joint_move(pEm, Kan, Seq, 0x100);
	}

	if (pEm->Sca_info & 1) {
		*(unsigned short *)&pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = 0xf0016;
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Spd_x = -(pEm->Spd_x >> 1);
		pEm->Timer0 = 0;
		pEm->Sce_free0 = 1;
	}	}

static void Em20_Mv_bodyatck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Em20_Mv_jump20(pEm, Kan, Seq)) {
		pEm->Routine_1 = 13;
		*(unsigned short *)&pEm->Routine_2 = 1;
		pEm->Damage_cnt &= 0x7f;
	}	}

static void Em20_Mv_bodyatck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		Add_speedXYZ(pEm, 0x800, 0);
		pEm->Spd_y += 40;
		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Routine_3 = 1;
			pEm->Pos_y = pEm->Ground;
			((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;

			pEm->Be_flg &= 0xefffffff;
			pEm->Be_flg |= 0x4000000;
			Snd_se_enem(10, pEm);
		}

		if (pEm->Move_cnt <= 16) {
			Joint_move(pEm, Kan, Seq, 0x100);
		}else

			pEm->Spd_x = 0;

	}else {

		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_1 = 13;
			*(unsigned short *)&pEm->Routine_2 = 1;
			pEm->Damage_cnt &= 0x7f;
		}
	}

	if (pEm->Timer0 < 0x400) {
		pEm->Cdir_y += 0x100;
		pEm->Timer0 += 0x100;
	}
}

static void Em20_Move_jumpin(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_jumpin_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em20_Mv_jumpin00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!(pEm->Routine_3 & 0xf)) {

		pEm->Routine_3 = 1;

		Jump_set(pEm, 0x41, -0x140);
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg &= 0xfd;
		pEm->Be_flg |= 8;
		((EM20_FREE *)OVL_FREE(pEm))->Outside = 0;
	}

	Add_speedXZ(pEm, 0);
	Joint_move(pEm, Kan, Seq, 0x200);

	if (!--pEm->Timer0) {
		*(unsigned short *)&pEm->Routine_2 = 1;
		pEm->Spd_x = 0x118;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 1;
		Snd_se_enem(3, pEm);
	}
}

static void Em20_Mv_jumpin20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;

	if (pEm->Pos_y >= pEm->Pos_y + pEm->Atd[0].Ofs_y)
		pEm->Be_flg &= ~8;

	if (Em20_Mv_jump20(pEm, Kan, Seq)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		pEm->Damage_cnt &= 0x7f;
		((EM20_FREE *)OVL_FREE(pEm))->Fix_cnt = 0;
		((EM20_FREE *)OVL_FREE(pEm))->No_hit = 0;
		pEm->nFloor = G.Player.nFloor;

	}	}

static void Em20_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!(((EM20_FREE *)OVL_FREE(pEm))->Combo_flg & 0x80)) {
		em20_dmg_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
	}else {

		Em20_Dmg_combo(pEm, Kan, Seq);
	}	}

static void Em20_Dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em20_Dmg_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;

	pEm->Timer0 = (G.Player.Cdir_y & 0xfff) - (pEm->Cdir_y & 0xfff);
	pEm->Timer0 += dir_hosei[ Rnd() & 7 ];

	pEm->Neck_flg |= 2;
	pEm->Type &= 0xdfff;

	pFp = (EM20_FREE *)OVL_FREE(pEm);	if (pEm->Routine_0 == 2)
		{ Snd_se_enem(1, pEm);
		}

	if (pFp->Combo_flg && !pFp->House_flg) {
		pFp->Combo_flg |= 0x80;
		Em20_Dmg_combo00(pEm, Kan, Seq);
		return;
	}

	if (pFp->Jump_flg) {
		*(unsigned short *)&pEm->Routine_2 = 1;
		pEm->Spd_x >>= 1;
	}else {

		*(unsigned short *)&pEm->Routine_2 = 2;
		pEm->Spd_x = 240;
		Set_dmghit(pEm, 0);
	}
	pFp->Em_next = 0;
	((EM20_FREE *)OVL_FREE(pEm))->Combo_flg = 1;
	*(long *)&pEm->Move_no = 0x70011;
	pEm->Timer1 = 0x200;

	Joint_move(pEm, Kan, Seq, 0x200);

	pFp->Blood = 1;
	Set_blood(pEm, 0, 0);

}

static void Em20_Dmg_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Add_speedXYZ(pEm, pEm->Timer0, 0);
	pEm->Spd_y += 40;

	if (pEm->Pos_y >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground;
		pEm->Timer1 = 0x200;
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 0;
		((EM20_FREE *)OVL_FREE(pEm))->Em_next = 1;
		Set_dmghit(pEm, 0);
		if (((0x404 >> (Rnd() & 0xf)) & 1) && !(((EM20_FREE *)OVL_FREE(pEm))->Combo_flg & 0x80) && pEm->Life >= 0 && !((EM20_FREE *)OVL_FREE(pEm))->House_flg) {

			{ Snd_se_enem(10, pEm); }

			*(unsigned short *)&pEm->Routine_2 = 3;	}else {

			Snd_se_enem(11, pEm);
			*(unsigned short *)&pEm->Routine_2 = 2;
		}
	}

	if (pEm->Move_cnt != 3)
		Joint_move(pEm, Kan, Seq, 0x200);

}

static void Em20_Dmg_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;

	pPl = (((EM20_FREE *)OVL_FREE(pEm))->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!((EM20_FREE *)OVL_FREE(pEm))->Em_next) {
		Add_speedXZ(pEm, pEm->Timer0);
		Rate_speed_down(pEm, 10, 0);
	}

	if (Joint_move(pEm, Kan, Seq, pEm->Timer1)) {
		switch (pEm->Routine_3) {
		case 0:
			pEm->Routine_3 = 1;
			((EM20_FREE *)OVL_FREE(pEm))->Combo_flg &= 0xc0;
			((EM20_FREE *)OVL_FREE(pEm))->Combo_flg |= 2;
			*(long *)&pEm->Move_no = (pEm->Hokan_flg << 16) + 0x12;
			break;

		case 1:
			pEm->Routine_3 = 2;
			*(long *)&pEm->Move_no = 0xf0007;
			pEm->Timer1 = 0x100;
			break;

		case 2:
			Set_dmghit(pEm, 1);
			((EM20_FREE *)OVL_FREE(pEm))->Combo_flg = 0;
			pEm->Type &= 0xdfff;
			pEm->Damage_cnt &= 0x7f;
			if (((EM20_FREE *)OVL_FREE(pEm))->House_flg) {
				*(long *)&pEm->Routine_0 = 0x10d01;
			}else

			if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200))

				*(long *)&pEm->Routine_0 = 0x501;
			else
				*(long *)&pEm->Routine_0 = 0x601;

		}
	}
}

static void Em20_Dmg_nml30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0x30f16;
		pEm->Damage_cnt &= 0x7f;
	}

	if (Joint_move(pEm, Kan, Seq, 0x400)) {
		Set_dmghit(pEm, 1);
		((EM20_FREE *)OVL_FREE(pEm))->Combo_flg = 0;
		*(long *)&pEm->Routine_0 = 0x201;
	}	}

static void Em20_Dmg_nml40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Life = ((G.System_flg & 0x20) ? Em20_life_easy[ Rnd() & 0xf ] : Em20_life[ Rnd() & 0xf ]) + (Rnd() & 3);

		pEm->D_life_u = pEm->D_life_c = pEm->D_life_d = 13;
		pEm->Be_flg &= 0xe3ffffff;
		pEm->Damage_cnt |= 0x80;
	}
}

static void Em20_Dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_combo_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Set_blood(pEm, Rnd() & 0xf, 0);
	Set_blood(pEm, Rnd() & 0xf, 0);
	Set_blood(pEm, Rnd() & 0xf, 5);
}

static void Em20_Dmg_combo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;

	pFp = (EM20_FREE *)OVL_FREE(pEm);	if (pEm->Routine_1 == 15 || pEm->Routine_1 == 18) {
		pFp->Blood += 3;
		if (!(Rnd() & 3)) {
			pFp->Blood++;
			Set_blood(pEm, Rnd() & 0xf, (Rnd() & 1) | 2);
		}
	}else {

		pFp->Blood = 1;
		Set_blood(pEm, 0, 0);
		Snd_se_enem(1, pEm);
	}

	if ((pFp->Combo_flg & 0x7f) == 1) {
		if (pFp->Jump_flg) {

			*(unsigned short *)&pEm->Routine_2 = 1;	}else {

			*(unsigned short *)&pEm->Routine_2 = 2;

		}	}else {

		*(unsigned short *)&pEm->Routine_2 = 0x103;
		if (!(pFp->Combo_flg & 0x40)) {
			*(long *)&pEm->Move_no = (pEm->Hokan_flg << 16) + 0x12;
			Snd_se_enem(1, pEm);
			pFp->Combo_flg |= 0x40;
		}

		pEm->Timer2 = 0;
		pFp->Em_next = 1;
	}
	pEm->Timer1 = 0;
}

static void Em20_Dmg_combo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		Add_speedXZ(pEm, pEm->Timer0 + (Rnd() << 1) - 0x100, 0);
		pEm->Cdir_y += (Rnd() & 0xff) - 0x80;
	}

	Em20_Dmg_nml10(pEm, Kan, Seq);
}

static void Em20_Dmg_combo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;

	pFp = (EM20_FREE *)OVL_FREE(pEm);
	if (!pFp->Em_next) {
		Add_speedXZ(pEm, pEm->Timer0);
		Rate_speed_down(pEm, 10, 0);
		if (!pEm->Routine_3) {
			pEm->Routine_3 = 1;
			pEm->Cdir_y += (Rnd() & 0xff) - 0x80;
		}
	}

	if (Joint_move(pEm, Kan, Seq, pEm->Timer1)) {
		*(unsigned short *)&pEm->Routine_2 = 3;
		pFp->Combo_flg &= 0xc0;
		pFp->Combo_flg |= 2;
		pEm->Timer2 = 0;
		*(long *)&pEm->Move_no = (pEm->Hokan_flg << 16) + 0x12;
	}
}

static void Em20_Dmg_combo30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!*(signed char *)&pEm->Timer2) {
		pEm->Timer2 = 1;
		if (pEm->Routine_1 != 12) {
			pEm->Spd_x = 40;
			Add_speedXZ(pEm, pEm->Timer0);
			pEm->Cdir_y += (Rnd() & 0xff) - 0x80;
		}
	}

	Em20_Dmg_nml20(pEm, Kan, Seq);

	if (pEm->Routine_3 >= 2)
		((EM20_FREE *)OVL_FREE(pEm))->Combo_flg &= 0xbf;

}

static void Em20_Dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;

	((EM20_FREE *)OVL_FREE(pEm))->Blood = 2 - pEm->Damage_no / 3;
	for (i = 0; i < ((EM20_FREE *)OVL_FREE(pEm))->Blood; i++)
		{ Set_blood(pEm, Rnd() & 0xf, 8); }

	pEm->Routine_1 = 1;
	Em20_Dmg_nml(pEm, Kan, Seq);
}

static void Em20_Dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;

	((EM20_FREE *)OVL_FREE(pEm))->Blood = 4 - pEm->Damage_no / 3;
	for (i = 0; i < ((EM20_FREE *)OVL_FREE(pEm))->Blood; i++)
		{ Set_blood(pEm, Rnd() & 0xf, 8); }

	pEm->Damage_cnt |= 0x80;

	pEm->Routine_1 = 1;
	Em20_Dmg_nml(pEm, Kan, Seq);
}

static void Em20_Dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long d0;

	d0 = Rnd() & 0xf;

	pEm->pSin_parts_ptr[ d0 ].Poly_rgb = 0x3f2f;

	((EM20_FREE *)OVL_FREE(pEm))->Blood = 1;
	Set_blood(pEm, d0, 9);

	pEm->Routine_1 = 1;
	Em20_Dmg_nml(pEm, Kan, Seq);
}

static void Em20_Dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_2) {
		Em20_Dmg_nml00(pEm, Kan, Seq);

		((EM20_FREE *)OVL_FREE(pEm))->Blood = 8;
		Set_blood(pEm, Rnd() & 0xf, 11);
		pEm->Timer3 = 0;
	}

	Em20_Dmg_nml(pEm, Kan, Seq);
	if (!(++*(signed char *)&pEm->Timer3 % 3)) {
		Set_blood(pEm, Rnd() & 0xf, 11);
		Set_blood(pEm, Rnd() & 0xf, 11);
	}
}

/* retail-only (em20_cdemd0_s1 0x80103FF4 / s0 0x80110FF4): a damage type dispatched like
 * Em20_Dmg_nml; names [INFERRED] from dmg_nml2_tbl reusing Em20_Dmg_nml10..40. */
static void Em20_Dmg_nml2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_nml2_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

/* retail-only (s1 0x80104030 / s0 0x80111030): airborne hit adds a knock-back hop, then normal damage */
static void Em20_Dmg_nml2_00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if ((pEm->Be_flg & 0x18000000) && !((EM20_FREE *)OVL_FREE(pEm))->Jump_flg) {
		((EM20_FREE *)OVL_FREE(pEm))->Jump_flg = 1;
		pEm->Spd_y = -0x8c;
		pEm->Spd_x = 0xf0;
	}
	Em20_Dmg_nml00(pEm, Kan, Seq);
}

static void Set_dmghit(ENEMY_WORK *pEm, unsigned long Type)
{
	if (!Type) {
		pEm->Be_flg &= 0xe7ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Atd[0].Ofs_y = -500;
		pEm->Atd[0].At_h = 500;
	}else {

		pEm->Be_flg |= 0xc000000;
		pEm->Atd[0].Ofs_y = -1000;
		pEm->Atd[0].At_h = 1000;
	}	}

static void Em20_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	em20_die_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);	}

static void Em20_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		die_nml_init[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
	}else {

		die_nml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
	}	}

static void Em20_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long tmp;

	Em_dead_flg_on(pEm);

	tmp = ((EM20_FREE *)OVL_FREE(pEm))->Combo_flg & 0x3f;
	((EM20_FREE *)OVL_FREE(pEm))->Combo_flg = 0;
	Em20_Dmg_nml00(pEm, Kan, Seq);

	if (!((EM20_FREE *)OVL_FREE(pEm))->No_voice) {
		Snd_se_enem(7, pEm);
	}

	if (tmp == 2)
		pEm->Spd_x = 0;

}

static void Em20_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!((EM20_FREE *)OVL_FREE(pEm))->Em_next) {
		Add_speedXZ(pEm, pEm->Timer0);
		Rate_speed_down(pEm, 10, 0);
	}

	if (Joint_move(pEm, Kan, Seq, pEm->Timer1)) {
		if (!pEm->Routine_3) {
			pEm->Routine_3 = 1;
			*(long *)&pEm->Move_no = (pEm->Hokan_flg << 16) + 0x12;
		}else

			*(long *)&pEm->Routine_0 = 7;
	}

}

static void Em20_Die_bang(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_bang_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Set_blood(pEm, 3, 0);
	Set_blood(pEm, 2, 0);
	Set_blood(pEm, 2, (Rnd() & 1) + 1);
}

static void Em20_Die_bang00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!((EM20_FREE *)OVL_FREE(pEm))->Jump_flg || !((EM20_FREE *)OVL_FREE(pEm))->Bang_ok || !(Rnd() & 5) || pEm->Damage_no >= 3) {

		pEm->Routine_1 = 1;
		Em20_Die_nml00(pEm, Kan, Seq);
	}else {

		Em_dead_flg_on(pEm);
		*(unsigned short *)&pEm->Routine_2 = 1;

		Set_head_bomb(pEm);

		if (!((EM20_FREE *)OVL_FREE(pEm))->Target_pl) {
			Em20_Pl_to_dmg(0, Cdir_ck(pEm, &G.Player));

		}

		((EM20_FREE *)OVL_FREE(pEm))->Blood = 0x12;
		*(long *)&pEm->Move_no = 0x70011;
		Joint_move(pEm, Kan, Seq, 0x200);
	}	}

static void Set_head_bomb(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	pEm->Neck_flg |= 1;

	for (i = 0; i < 7; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ bang_parts_no[ i ] ];
		pP_ptr->Be_flg |= 0x4a;
		pP_ptr->Free[0] = pEm->Cdir_y;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[2] = 800;
		pP_ptr->Free[1] = -150;
		pP_ptr->Free[3] = 10;
		pP_ptr->Free[6] = -100;
		pP_ptr->Poly_rgb = 0x101040;
	}

}

static void Em20_Die_bang10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM20_FREE *pFp;

	pFp = (EM20_FREE *)OVL_FREE(pEm);

	Add_speedXYZ(pEm, 0, 0);

	if (pFp->Jump_flg) {
		if (!pEm->Routine_3)
			if (pEm->L_pl < 1400) {
			pEm->Spd_x = 0;
			pEm->Routine_3 = 1;
		}

		pEm->Spd_y += 40;
		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			pFp->Jump_flg = 0;
			pEm->Routine_3 = 0;
		}

		if (pEm->Move_cnt != 3)
			Joint_move(pEm, Kan, Seq, 0x200);

	}else

	if (Joint_move(pEm, Kan, Seq, 0x200)) {
		if (!pEm->Routine_3) {
			pEm->Routine_3 = 1;
			*(long *)&pEm->Move_no = (pEm->Hokan_flg << 16) + 0x12;
		}else

			*(long *)&pEm->Routine_0 = 7;
	}

}

static void Em20_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_gnml_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Set_blood(pEm, 3, 0);
	Set_blood(pEm, 2, 0);
	Set_blood(pEm, 2, (Rnd() & 1) + 1);
}

static void Em20_Die_gnml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long d0;

	if (pEm->Routine_1 == 9 && pEm->Damage_no >= 3) {
		Em20_Die_nml00(pEm, Kan, Seq);
	}else {

		((EM20_FREE *)OVL_FREE(pEm))->No_voice = 1;
		Em20_Die_nml00(pEm, Kan, Seq);

		Set_head_bomb(pEm);

		if (pEm->Routine_1 == 9) {
			((EM20_FREE *)OVL_FREE(pEm))->Blood = 1;
			d0 = Rnd() & 0xf;
			pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
			if (!(pP_ptr->Be_flg & 0x4a))
				Set_blood(pEm, d0, 7);

		}

		((EM20_FREE *)OVL_FREE(pEm))->Blood = 0x12;
	}	}

static void Em20_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	Em20_Die_nml00(pEm, Kan, Seq);

	if (pEm->Damage_no < 3 || pEm->Routine_1 == 16) {

		((EM20_FREE *)OVL_FREE(pEm))->Blood = 6;
		for (i = 0; i < 6; i++)
			{ Set_blood(pEm, Rnd() & 0xf, 7); }

		for (i = 0; i < 17; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[ i ];
			pP_ptr->Poly_rgb = 0x202020;
		}
	}	}

static void Em20_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	Em20_Die_nml00(pEm, Kan, Seq);

	if (pEm->Damage_no >= 3) return;

	for (i = 0; i < 17; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		pP_ptr->Poly_rgb = 0x3f2f;
	}

	((EM20_FREE *)OVL_FREE(pEm))->Blood = 2;
	Set_blood(pEm, Rnd() & 7, 9);
	Set_blood(pEm, (Rnd() & 7) | 8, 10);

}

static void Em20_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_spark_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	if (++pEm->Timer3 & 1) {
		Set_blood(pEm, Rnd() & 0xf, 11);
		Set_blood(pEm, Rnd() & 0xf, 11);
	}

}

static void Em20_Die_spark00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	Em20_Die_nml00(pEm, Kan, Seq);

	for (i = 0; i < 17; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		pP_ptr->Poly_rgb = 0x3f203f;
	}

	((EM20_FREE *)OVL_FREE(pEm))->Blood = 0x14;
	pEm->Timer3 = 0;
	Set_blood(pEm, Rnd() & 0xf, 11);
	Set_blood(pEm, Rnd() & 0xf, 12);
	Set_blood(pEm, 1, 12);
}

static void Em20_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em20_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_1) {
		pEm->Routine_1 = 1;
		pEm->Be_flg |= 2;
	}

	Em20_Blood_move(pEm);	}

static void Em20_Blood_move(ENEMY_WORK *pEm)
{
	register KAGE_WORK *pKage;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (((EM20_FREE *)OVL_FREE(pEm))->Kage_routine) {

	case 0:
		((EM20_FREE *)OVL_FREE(pEm))->Kage_routine = 1;
		*(unsigned long *)&pKage->Prim[0].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[0].r0 |= 0xbfbf10;
		*(unsigned long *)&pKage->Prim[1].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[1].r0 |= 0xbfbf10;
		pEm->Timer3 = 90;

	case 1:
		pKage->Half_x += 8;
		pKage->Half_z += 8;
		if (!--*(signed char *)&pEm->Timer3)
			((EM20_FREE *)OVL_FREE(pEm))->Kage_routine = 2;
		break;

	case 2:
		break;
	}

}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_1) {

	case 0:
		pEm->Routine_1 = 1;
		*(long *)&pEm->Move_no = 0x1f0000;
		pEm->Neck_flg |= 1;
		break;

	case 1:
		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;
		pEm->Pos_y = pEm->Ground;

		if (Joint_move(pEm, Kan, Seq, 0x80))
			pEm->Routine_1 = 2;

	}

}

static void Dog_rot_neck(ENEMY_WORK *pEm)
{

	pEm->Neck_no = 2;
	Rot_neck_em(pEm, pEm->Cdir_y);

	pEm->Neck_no = 3;
	Rot_neck_em(pEm, pEm->Cdir_y);

	pEm->Neck_no = 2;

}

static void Rate_speed_up(ENEMY_WORK *pEm, long g, long limit)
{
	if (pEm->Spd_x < limit) {
		pEm->Spd_x += g;
		if (pEm->Spd_x > limit)
			pEm->Spd_x = limit;

	}	}

static void Rate_speed_down(ENEMY_WORK *pEm, long g, long limit)
{
	if (pEm->Spd_x > limit) {
		pEm->Spd_x -= g;
		if (pEm->Spd_x < limit)
			pEm->Spd_x = limit;

	}	}

static void Set_run_motion(ENEMY_WORK *pEm, short d_tmp)
{
	register unsigned char t_tmp;

	t_tmp = mot_tbl[ d_tmp ? ((d_tmp > 0) ? 2 : 0) : 1 ];

	if (pEm->Move_no != t_tmp) {

		pEm->Move_no = t_tmp;
		if (pEm->Hokan_flg < 7)
			pEm->Hokan_flg = 15;
	}

}

static long Atck_range_ck(ENEMY_WORK *pEm)
{
	register EM20_FREE *pFp;
	register ENEMY_WORK *pTg;
	register long dist;

	pFp = (EM20_FREE *)OVL_FREE(pEm);	pTg = (((EM20_FREE *)OVL_FREE(pEm))->Target_pl) ? G.pEnemy[1] : (ENEMY_WORK *)&G.Player;	/* layout: kept on one line (retail likely split it like 532/1184/2266/3236); with the split ASPSX 2.56's record-buffer flush lands on Atck_hit_ck's line-3190 SLD record and PSYLINK relocates it twice */

	if (pTg->Life < 0) return -2;

	dist = (pFp->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl;
	if (dist < 4000 && pTg->nFloor == pEm->nFloor) {
		if (!Dir_pos_ck(&pEm->Pos_x, &pTg->Pos_x, pEm->Cdir_y, 0x100)) {

			if (pTg->Damage_cnt & 0x80) return -4;

			if (pTg->Life < 21 && Cdir_ck(pEm, pTg) && pFp->Stress < 600) return -2;

			return 1;
		}

		return -1;

	}
	return 0;
}

static unsigned long hit_ck_main(ENEMY_WORK *pEm, long pl)
{
	register EM20_FREE *pFp;
	register PARTS_W *pPt;
	register PLAYER_WORK *pPl;
	register long range, life, ck;

	pFp = (EM20_FREE *)OVL_FREE(pEm);

	pPt = pEm->pSin_parts_ptr;	pPt += 4;

	pPl = (pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	life = pPl->Life;
	range = (life >= 21) ? 1000 : 700;
	if (!pFp->Hit_type && !(pPl->Damage_cnt & 0x80)) {
		if (L_pos_ckXZ((VECTOR *)&pPl->Pos_x, (VECTOR *)pPt->Workm.t, range) && pEm->Ground - 1800 < pEm->Pos_y && pPl->nFloor == pEm->nFloor) {

			pFp->Stress = 0;

			if (!pl) {
				{ ck = Pl_life_down(20, 0); }
			}else

				ck = Spl_life_down(pEm, 20, 0);

			if (ck == 2 && !Cdir_ck(pEm, pPl) && !pFp->Target_pl && pPl->Id != 15) { /* retail: Id test @0x80104F10 */

				pFp->Hit_type = 2;
				pEm->Spd_x = 0;

				pPl->Be_flg |= 0xa;
				pPl->Damage_cnt = 0xff;
				pPl->Cdir_y = pEm->Cdir_y + 0x800;
				G.System_flg &= 0xfbffffff;
				pEm->Damage_cnt |= 0x80; /* retail @0x80104F64 */
			}else {

				*(unsigned short *)&pEm->Routine_2 = 3;
				pFp->Hit_type = 1;

				if (pPl->Life >= 0) {
					Em20_Pl_to_dmg(pl, Cdir_ck(pEm, pPl));
				}else {

					Em20_Pl_to_die(pl);
					pEm->Be_flg |= 2;
				}
			}
		}
	}

	return pFp->Hit_type;

}

static unsigned long Atck_hit_ck(ENEMY_WORK *pEm)
{

	if (((EM20_FREE *)OVL_FREE(pEm))->No_hit) return 0;

	if (hit_ck_main(pEm, 0))
		return ((EM20_FREE *)OVL_FREE(pEm))->Hit_type;

	if (G.Status_flg & 0x10000000) {
		if (hit_ck_main(pEm, 1))
			return ((EM20_FREE *)OVL_FREE(pEm))->Hit_type;
	}

	return 0;	}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	register EM20_FREE *pFp;
	register ENEMY_WORK *pTg;
	SVECTOR Sv;
	register long dir, Id;
	register EM20_BLOOD_TBL *tbl;

	pFp = (EM20_FREE *)OVL_FREE(pEm);
	if (pFp->Blood) {

		tbl = &blood_id_tbl[ type ];
		pTg = (ENEMY_WORK *)&G.Player;	if (type != 4) pTg = pEm;
		dir = pTg->Cdir_y + (Rnd() & (tbl->Dir * 2 - 1)) - tbl->Dir;

		pP_ptr = pTg->pSin_parts_ptr;
		pP_ptr += parts & 0x7f;

		Id = tbl->Id & 0x7f;

		if (tbl->Id & 0x80) {
			Sv.vx = 0;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call((Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
		}else {

			Sv.vx = pP_ptr->Workm.t[0];
			Sv.vy = pP_ptr->Workm.t[1];
			Sv.vz = pP_ptr->Workm.t[2];
			Esp_call((Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
		}
		pFp->Blood--;
	}	}

static void Em20_Pl_to_dmg(unsigned long pl, unsigned char dir)
{
	register PLAYER_WORK *pPl;

	pPl = (pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir + 2;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
}

static void Em20_Pl_to_die(unsigned long pl)
{
	register PLAYER_WORK *pPl;

	pPl = &G.Player;	if (pl) pPl = (PLAYER_WORK *)G.pEnemy[1];

	pPl->Routine_0 = 3;
	pPl->Routine_1 = 0;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;
}

static void Ck_Floor(ENEMY_WORK *pEm)
{
	if (G.Player.nFloor != pEm->nFloor) {
		pEm->Routine_1 = 9;
		*(unsigned short *)&pEm->Routine_2 = 0;
		G.Room_enemy_flg &= 0xffdf;
	}	}

static void Ck_Died(ENEMY_WORK *pEm)
{
	if (G.Room_enemy_flg & 0x40) {
		pEm->Routine_1 = 11;
		*(unsigned short *)&pEm->Routine_2 = 0;
	}	}

static ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
	register ENEMY_WORK *pTg;

	pTg = (ENEMY_WORK *)&G.Player;
	((EM20_FREE *)OVL_FREE(pEm))->Target_pl = 0;

	if (G.Status_flg & 0x10000000) {
		if ((pEm->L_spl & 0xc0000000) == 0x40000000) {

			pTg = G.pEnemy[1];
			((EM20_FREE *)OVL_FREE(pEm))->Target_pl = 1;
		}
		if ((pEm->L_spl & 0xc0000000) == 0x80000000 && (pEm->L_spl & 0x3fffffff) < pEm->L_pl) {

			if (G.pEnemy[1]->Life >= 21) {
				pTg = G.pEnemy[1];
				((EM20_FREE *)OVL_FREE(pEm))->Target_pl = 1;
			}
		}
	}

	return pTg;
}

