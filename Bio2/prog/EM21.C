/* em21.c - reconstruction of D:\Bio2\prog\EM21.C (enemy 21: crow).
 * Linked into two overlays from the same source: 0x07 (at 0x8010013c) and
 * 0x25 (at 0x8010d13c); only the enemy-number-specific entry names differ
 * (Em121/Em221, Pldamege_Em121/221), selected by OVL_25.
 * Types come from the TU's retail SYM preamble (em21.h). */
#include "EM21.H"

/* Em21 work variables kept in the enemy's generic Free[] area */
#define EF	((EM21F *)ENEMY_FREE(pEm))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of other translation units ----
 * EM21.C includes no prototypes for them: every external routine is called
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
static void (*Em21_routine_0[8])() = {
	&Em21_Init,
	&Em21_Move,
	&Em21_Damage,
	&Em21_Damage,
	&Em21_Scenario,
	0,
	0,
	&Em21_Dead
};
static void (*Em21_indoor_br[15])() = {
	&Em21_br_wait_i,
	&Em21_br_go_wait_i,
	&Em21_br_take_off_i,
	&Em21_br_glide_i,
	&Em21_br_flight_i,
	&Em21_br_up_i,
	&Em21_br_down_i,
	&Em21_br_landing_i,
	&Em21_br_quick_turn_i,
	&Em21_br_jump_i,
	&Em21_br_fall_i,
	&Em21_br_crash_f_i,
	&Em21_br_crash_i,
	&Em21_br_peck_f_i,
	&Em21_br_peck_i
};
static void (*Em21_indoor_mv[15])() = {
	&Em21_mv_wait_i,
	&Em21_mv_go_wait_i,
	&Em21_mv_take_off_i,
	&Em21_mv_glide_i,
	&Em21_mv_flight_i,
	&Em21_mv_up_i,
	&Em21_mv_down_i,
	&Em21_mv_landing_i,
	&Em21_mv_quick_turn_i,
	&Em21_mv_jump_i,
	&Em21_mv_fall_i,
	&Em21_mv_crash_f_i,
	&Em21_mv_crash_i,
	&Em21_mv_peck_f_i,
	&Em21_mv_peck_i
};
static void (*Em21_outdoor_br[15])() = {
	&Em21_br_wait_i,
	&Em21_br_go_wait_i,
	&Em21_br_take_off_i,
	&Em21_br_glide_i,
	&Em21_br_flight_i,
	&Em21_br_up_i,
	&Em21_br_down_i,
	&Em21_br_landing_i,
	&Em21_br_quick_turn_i,
	&Em21_br_jump_i,
	&Em21_br_fall_i,
	&Em21_br_crash_f_i,
	&Em21_br_crash_i,
	&Em21_br_peck_f_i,
	&Em21_br_peck_i
};
static void (*Em21_outdoor_mv[15])() = {
	&Em21_mv_wait_i,
	&Em21_mv_go_wait_i,
	&Em21_mv_take_off_i,
	&Em21_mv_glide_i,
	&Em21_mv_flight_i,
	&Em21_mv_up_i,
	&Em21_mv_down_i,
	&Em21_mv_landing_i,
	&Em21_mv_quick_turn_i,
	&Em21_mv_jump_i,
	&Em21_mv_fall_i,
	&Em21_mv_crash_f_i,
	&Em21_mv_crash_i,
	&Em21_mv_peck_f_i,
	&Em21_mv_peck_i
};
static void (*Em21_dm[19])() = {
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm01,
	&Em21_dm01,
	&Em21_dm03,
	&Em21_dm03,
	&Em21_dm01,
	&Em21_dm01,
	&Em21_dm01,
	&Em21_dm02,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm00,
	&Em21_dm01,
	&Em21_dm00
};
static void (*Em21_sce_mv[3])() = {
	&Em21_sce_wait,
	&Em21_sce_rush,
	&Em21_sce_crash
};
static void (*Em21_dead_mv[4])() = {
	&Em21_dead_normal,
	&Em21_dead_bomb,
	&Em21_dead_bow,
	&Em21_dead_instant
};
static void (*Pldamage_Em21_tbl[1])() = {
	&Em21_pldm00
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	static long D0;
	VECTOR Vec0, Vec1;

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7f) pEm->Damage_cnt--;

		Vec0.vx = pEm->Pos_x;
		Vec0.vy = pEm->Pos_y;
		Vec0.vz = pEm->Pos_z;
		Vec1.vx = G.Player.Pos_x;
		Vec1.vy = G.Player.Pos_y;
		Vec1.vz = G.Player.Pos_z;
		if (Sca_ck_line(&Vec0, &Vec1, 0x8400, 0)) {
			EF->Flg &= ~2;
		}else {
			EF->Flg |= 2;
		}

		if (pEm->L_pl < 900) {
			pEm->Atd[0].Ofs_y = -350;
		}else {
			pEm->Atd[0].Ofs_y = 0;
		}

		Em21_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Em21_kage_set(pEm);

		Em21_lock_off_ck(pEm);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);
	}

}

static void Em21_kage_set(ENEMY_WORK *pEm)
{
	register long D0;

	if (pEm->Routine_0 != 7) {

		D0 = pEm->Ground - pEm->Pos_y - 250;

		if (D0 > 0) {
			D0 = 0x57E40 / ((D0 + 0x708) >> 1);
			pEm->pKage_work[1] = D0 | (D0 << 16);

		}else
			pEm->pKage_work[1] = 0x1900190;
	}

}

static void Em21_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;
	pEm->Life = 10;

	ENEMY_FREE(pEm)[0] = 0;
	ENEMY_FREE(pEm)[1] = 0;
	ENEMY_FREE(pEm)[2] = 0;
	ENEMY_FREE(pEm)[3] = 0;
	ENEMY_FREE(pEm)[4] = 0;
	ENEMY_FREE(pEm)[5] = 0;
	ENEMY_FREE(pEm)[6] = 0;
	ENEMY_FREE(pEm)[7] = 0;
	pEm->Sce_free0 = 0;
	pEm->Sce_free1 = 0;
	pEm->Sce_free2 = 0;
	pEm->Sce_free3 = 0;
	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x200);

	pP_ptr = pEm->pSin_parts_ptr + 2;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 8;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 0x60;
	pP_ptr->Free[3] = 0x60;
	pP_ptr->Free[4] = 0x200;
	pP_ptr->Free[5] = 0x138;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -350;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_h = 0x212;
	Em21_atd_set(pEm, 350);

	pEm->Eff_at_r = 300;

	pEm->Ground = Sca_get_ground(&pEm->Pos_x, 250, 0x400, 0);

	Kage_work_set(&pEm->pKage_work, 0, 0xC800C8, 0, &pEm->Pos_x);

	pEm->Be_flg |= 0x8000000;

	if (pEm->Type & 0x4000) {
		pEm->Routine_0 = 4;
		pEm->Type &= ~0x4000;
		EF->Flg |= 1;
		pEm->Routine_1 = pEm->Type & 0xf;
		Em21_routine_0[ pEm->Routine_0 ](pEm, Kan, Seq);
	}

	if (pEm->Type & 0x40) {
		pEm->Be_flg |= 8;
		pEm->Damage_cnt = 0x80;
	}

	G.pPldamage_Em[ 0x21 - 0x10 ] = PLDAMEGE_EM;

}

static void Em21_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long Flg;

	Flg = 0;
	if (EF->Idly_cnt == 0) {
		if (!(Rnd() & 0x1f)) {
			EF->Idly_cnt = Rnd() & 0x7f;
			EF->Idly_flg = Rnd_area();
		}
	}
	if (G.Player.Life < 0) {
		if (EF->Pl_dead_cnt) {
			if (EF->Idly_cnt) Flg = 1;
			EF->Pl_dead_cnt--;
		}
	}else {
		if (!(EF->Flg & 0x40))
			if (EF->Idly_cnt) Flg = 1;
	}

	if (Flg) {
		Root_ck(pEm, &G.Player.Pos_x, EF->Idly_flg, 1);
		if (EF->Idly_cnt-- <= 0) EF->Idly_flg = 0;
	}else {
		Root_ck(pEm, &G.Player.Pos_x, 0, 0);
	}

	if (pEm->Type & 0x80) {
		Em21_outdoor_br[ pEm->Routine_1 ](pEm);
		Em21_outdoor_mv[ pEm->Routine_1 ](pEm, Kan, Seq);
	}else {
		Em21_indoor_br[ pEm->Routine_1 ](pEm);
		Em21_indoor_mv[ pEm->Routine_1 ](pEm, Kan, Seq);
	}
}

static void Em21_br_wait_i(ENEMY_WORK *pEm)
{	register long D0;
	D0 = 0;

	if (G.Player.Life < 0 && (signed char)EF->Pl_dead_cnt <= 0 && pEm->L_pl < 1800) return;

	if (pEm->L_pl <= 1800) D0 = 1;

	if ((G.Room_player_flg & 4) && pEm->L_pl <= 5400)
		D0 = 1;

	if (G.Room_player_flg & 1) D0 = 1;

	if (EF->Flg & 1)
		if ((unsigned short)pEm->Timer0-- <= 0) D0 = 1;

	if (D0)
		Em21_r1_set(pEm, 1);

}

static void Em21_mv_wait_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Cnt = Rnd() % 100;
		pEm->Routine_2 = 7;
		break;
	case 1:
		*(long *)&pEm->Move_no = 0x70000 + ((Rnd() % 5) << 8);
		EF->Cnt = 5;
		pEm->Routine_2 = 10;
		break;
	case 2:
		*(long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 8;
		break;
	case 3:
		*(long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 9;
		break;
	case 4:
		*(long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 11;
		break;
	case 5:
		*(long *)&pEm->Move_no = 0x70002 + ((Rnd() % 7) << 8);
		pEm->Routine_2 = 8;
		break;
	case 6:
		*(long *)&pEm->Move_no = 0x7000A;
		EF->Cnt = Rnd() % 5 + 15;
		EF->Tmp_0 = pEm->Pos_x;
		EF->Tmp_1 = pEm->Pos_z;
		pEm->Routine_2 = 13;
		break;

	case 7:
		if (EF->Cnt == 0) {
			EF->Tmp = Rnd() % 7;
			if (EF->Tmp == 6) {
				if (Rnd() & 0xf)

					pEm->Routine_2 = 0;
				else
					pEm->Routine_2 = 6;
			}else

				pEm->Routine_2 = EF->Tmp; } EF->Cnt--; break;

	case 9: if (Joint_move(pEm, Kan, Seq, 0x10200))
			pEm->Routine_2 = 0; break;

	case 10: if (EF->Cnt-- <= 0) {
			if (!(EF->Flg & 0x20) && (Rnd() & 1))
				Snd_se_enem(6, pEm);
		}
	case 8: if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 0; break;

	case 11: if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_2 = 12;
			EF->Cnt = Rnd() % 100;
		}
		break;
	case 12:
		if (EF->Cnt == 0) pEm->Routine_2 = 3;
		EF->Cnt--;
		break;
	case 13:
		Joint_move(pEm, Kan, Seq, 0x10200);
		Em21_foot_set(pEm);
		if (pEm->Move_cnt == EF->Cnt) {
			*(long *)&pEm->Move_no = 0x7000A + ((0x27 - pEm->Move_cnt) << 8);
			pEm->Routine_2 = 14;
		}
		break;
	case 14:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_2 = 0;
			pEm->Pos_x = EF->Tmp_0;
			pEm->Pos_z = EF->Tmp_1;
		}else

			Em21_foot_set(pEm);

	}
}

static void Em21_br_go_wait_i(ENEMY_WORK *pEm)
{
}

static void Em21_mv_go_wait_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Cnt = Rnd() & 7;
	case 1:
		if (EF->Cnt-- <= 0) {
			if (pEm->Type & 0x40)

				Em21_r1_set(pEm, 3);
			else Em21_r1_set(pEm, 2);
		}

	}
}

static void Em21_br_take_off_i(ENEMY_WORK *pEm)
{
}

static void Em21_mv_take_off_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:
		D0 = Rnd() & 7;
		if (Rnd() & 1) Snd_se_enem(1, pEm);
		if (!(Rnd() & 7))
			Em_21_feather_call(pEm, 1);

		*(long *)&pEm->Move_no = 0x70007 + (D0 << 8);
		EF->Sca_cnt = 0;
		pEm->Spd_x = (Rnd() & 0x1f) + 100;
		pEm->Spd_y = -10;
		EF->Flg |= 1;
		EF->Cnt = 9 - D0;
		EF->Tmp = 1;
		pEm->Routine_2 = 5;
		goto move;
	case 1:
		pEm->Spd_y = -80;
		EF->Cnt = 6;
		EF->Tmp = 2;
		pEm->Routine_2 = 5;
		goto move;
	case 2:
		if (Rnd() & 1) Snd_se_enem(1, pEm);
		pEm->Spd_y = -60;
		EF->Cnt = 4;
		EF->Tmp = 3;
		pEm->Routine_2 = 5;
		goto move;
	case 3:
		pEm->Spd_y = -100;
		EF->Cnt = 9;
		EF->Tmp = 4;
		pEm->Routine_2 = 5;
		goto move;
	case 4:
		if (!(Rnd() & 7))
			Em_21_feather_call(pEm, 1);

		pEm->Spd_y = -80;
		EF->Cnt = 0xff;
		pEm->Routine_2 = 5;

	case 5:
move:
		EF->Cnt--;
		if (EF->Cnt == 0) pEm->Routine_2 = EF->Tmp;
		if (Rnd() & 1) {
			Goto00(pEm, -pEm->Pos_x + G.Player.Pos_x, -pEm->Pos_z + G.Player.Pos_z, 0x20);

		}else {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		}
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Spd_x < 300) pEm->Spd_x++;
		if (Joint_move(pEm, Kan, Seq, 0x200))
			Em21_r1_set(pEm, 4);

	}
}

static void Em21_br_glide_i(ENEMY_WORK *pEm)
{
}

static void Em21_mv_glide_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register long D0;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Type &= ~0x40;
		if (Rnd() & 1) Snd_se_enem(1, pEm);

		D0 = Rnd() & 7;
		*(long *)&pEm->Move_no = 0x70006 + (D0 << 8);
		EF->Sca_cnt = 0;
		pEm->Spd_x = (Rnd() & 7) + 60;
		pEm->Spd_y = 0;
		EF->Flg |= 1;
		EF->Cnt = 12;
		EF->Cnt2 = 0;
		EF->Tmp = 1;
		pEm->Routine_2 = 2;
		goto move;
	case 1:
		pEm->Be_flg &= ~8;
		pEm->Damage_cnt &= 0x7f;
		pEm->Spd_x = (Rnd() & 7) + 80;
		pEm->Spd_y = 80;
		EF->Cnt = 0xff;
		EF->Cnt2 = 0x20;
		pEm->Routine_2 = 2;

	case 2:
move:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, EF->Cnt2);
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Spd_x < 300) pEm->Spd_x++;
		if (Joint_move(pEm, Kan, Seq, 0x200)) pEm->Routine_2 = 3;
		if (EF->Cnt-- <= 0) pEm->Routine_2 = EF->Tmp;
		break;
	case 3:
		Em21_r1_set(pEm, 4);

	}
}

static void Em21_br_flight_i(ENEMY_WORK *pEm)
{

	if (pEm->L_pl > 7200) {
		if (Rnd() & 1)

			Em21_r1_set(pEm, 7);
		else Em21_r1_set(pEm, 8);

	}

	if (!(Rnd() & 0xf)) {
		pEm->Timer0 = Rnd() & 0x3f;
		if (G.Player.Ground - 3600 < pEm->Pos_y)

			Em21_r1_set(pEm, 5);
		else Em21_r1_set(pEm, 6);
	}

	Em21_sca_avoid_ck(pEm);
	if (G.Player.Life < 0 && (signed char)EF->Pl_dead_cnt <= 0) {
		Em21_pl_dead_ck(pEm);
	}else {
		Em21_attack_ck(pEm);
		Em21_run_away_ck(pEm);
	}
}

static void Em21_mv_flight_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Svec;

	switch (pEm->Routine_2) {
	case 0:
		if (Rnd() & 1) {
			*(long *)&pEm->Move_no = 0x70003;
			EF->Tmp = 2;
		}else {
			*(long *)&pEm->Move_no = 0x70004;
			if (!(Rnd() & 7))
				Em_21_feather_call(pEm, 1);

			EF->Tmp = 3;
		}
		EF->Cnt = Rnd() & 0x1f;
		pEm->Spd_y = 0;
		pEm->Routine_2 = 1;
	case 1:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Spd_x < 300) pEm->Spd_x += EF->Tmp;
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0) pEm->Routine_2 = 0;

	}
}

static void Em21_br_up_i(ENEMY_WORK *pEm)
{

	if (pEm->Pos_y < G.Player.Ground - 4800)
		Em21_r1_set(pEm, 4);

	if ((unsigned short)pEm->Timer0-- <= 0)
		Em21_r1_set(pEm, 4);

	Em21_sca_avoid_ck(pEm);
	if (G.Player.Life < 0 && (signed char)EF->Pl_dead_cnt <= 0) {
		Em21_pl_dead_ck(pEm);
	}else {
		Em21_attack_ck(pEm);
		Em21_run_away_ck(pEm);
	} }

static void Em21_mv_up_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Cnt = 0;
		*(long *)&pEm->Move_no = 0x70004;
		pEm->Spd_y = -40;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0)
			EF->Cnt = 9;

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Spd_x < 300) pEm->Spd_x += 2;
		Joint_move(pEm, Kan, Seq, 0x200);

	}
}

static void Em21_br_down_i(ENEMY_WORK *pEm)
{

	if (G.Player.Ground - 1800 < pEm->Pos_y)
		Em21_r1_set(pEm, 4);

	if ((unsigned short)pEm->Timer0-- <= 0)
		Em21_r1_set(pEm, 4);

	Em21_sca_avoid_ck(pEm);
	if (G.Player.Life < 0 && (signed char)EF->Pl_dead_cnt <= 0) {
		Em21_pl_dead_ck(pEm);
	}else {
		Em21_attack_ck(pEm);
		Em21_run_away_ck(pEm);
	} }

static void Em21_mv_down_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Cnt = 0;
		*(long *)&pEm->Move_no = 0x70003;
		pEm->Spd_y = 40;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0)
			EF->Cnt = 15;

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
		Add_speedXYZ(pEm, 0, 0);
		if (pEm->Spd_x < 300) pEm->Spd_x++;
		Joint_move(pEm, Kan, Seq, 0x200);

	}
}

static void Em21_br_landing_i(ENEMY_WORK *pEm)
{

}

static void Em21_mv_landing_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long D0;
	SVECTOR Svec;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70003;
		EF->Sca_cnt = 0;
		EF->Cnt = 0;
		pEm->Spd_x = 50;
		pEm->Spd_y = 120;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0) {
			if (Rnd() & 1) Snd_se_enem(1, pEm);
			EF->Cnt = 15;
		}
		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		Add_speedXYZ(pEm, 0, 0);
		D0 = pEm->Ground - 250;
		if (pEm->Pos_y - D0 > -300)
			pEm->Routine_2 = 2;

		Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 2:
		*(long *)&pEm->Move_no = 0x7000A;
		if (Rnd() & 1) Snd_se_enem(1, pEm);
		pEm->Spd_x = 50;
		pEm->Spd_y = 15;
		EF->Cnt = 15;
		EF->Tmp = 3;
		pEm->Routine_2 = 5;
		goto move;
	case 3:
		if (Rnd() & 1) Snd_se_enem(1, pEm);
		EF->Cnt = 5;
		EF->Tmp = 4;
		pEm->Routine_2 = 5;
		goto move;
	case 4:
		pEm->Spd_x = 10;
		pEm->Spd_y = 0;
		pEm->Pos_y = pEm->Ground - 250;
		EF->Cnt = 0xff;
		pEm->Routine_2 = 5;

	case 5:
move:
		EF->Cnt--;
		if (EF->Cnt == 0) pEm->Routine_2 = EF->Tmp;
		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		Add_speedXYZ(pEm, 0, 0);
		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) pEm->Pos_y = D0;
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Spd_x = 10;
			pEm->Routine_2 = 6;
			*(long *)&pEm->Move_no = 0x70008;
		}
		break;
	case 6:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);
		Add_speedXYZ(pEm, 0, 0);
		pEm->Pos_y = pEm->Ground - 250;
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			if (G.Player.Life < 0 && (signed char)EF->Pl_dead_cnt <= 0)

				Em21_r1_set(pEm, 0);
			else {	pEm->Timer0 = Rnd() << 1;
				Em21_r1_set(pEm, 2);	}
		}

	}
}

static void Em21_br_quick_turn_i(ENEMY_WORK *pEm)
{

	Em21_run_away_ck(pEm); }

static void Em21_mv_quick_turn_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Svec;

	switch (pEm->Routine_2) {
	case 0:
		if (!(Rnd() & 7))
			Em_21_feather_call(pEm, 1);

		*(long *)&pEm->Move_no = 0x70004;
		EF->Cnt = Rnd() & 0x3f;
		EF->Cnt2 = 0;
		EF->Sca_cnt = 0;
		pEm->Spd_y = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt2-- <= 0)
			EF->Cnt2 = 14;

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);
		pEm->Cdir_y += (signed char)EF->Tmp;
		Add_speedXYZ(pEm, 0, 0);

		Joint_move(pEm, Kan, Seq, 0x200);
		if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x60) || EF->Cnt-- <= 0)

			Em21_r1_set(pEm, 4);

	}
}

static void Em21_br_jump_i(ENEMY_WORK *pEm)
{
}

static void Em21_mv_jump_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70008;
		pEm->Routine_2 = 1;
	case 1:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Timer0 = Rnd() & 0xf;
			Em21_r1_set(pEm, 0);
		}

	}
}

static void Em21_br_fall_i(ENEMY_WORK *pEm)
{
}

static void Em21_mv_fall_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(3, pEm);
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Spd_y = 0;
		pEm->Spd_x = 0;
		EF->Tmp_0 = 0;
		EF->Sca_cnt = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 2;
	case 2:

		EF->Tmp_0 += 20;
		pEm->Pos_y += EF->Tmp_0;

		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Snd_se_enem(5, pEm);
			pEm->Pos_y = D0;
			*(long *)&pEm->Move_no = 0x7000B;
			EF->Cnt = Rnd() & 0xf;
			pEm->Cdir_z = 0;
			pEm->Routine_2 = 3;
		}
		break;
	case 3:
		if (EF->Cnt-- <= 0) pEm->Routine_2 = 4;
		Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Move_cnt--;
		break;
	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			Em21_r0_set(pEm, 1, 9);

	}
}

static void Em21_br_crash_f_i(ENEMY_WORK *pEm)
{
	if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x60) || !(EF->Flg & 2))

		Em21_r1_set(pEm, 4);

	Em21_run_away_ck(pEm);
}

static void Em21_mv_crash_f_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long D0;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70004;
		EF->Tmp_0 = G.Player.Ground - 2700;
		D0 = EF->Tmp_0 - pEm->Pos_y;
		if (D0 < 0)

			EF->Tmp = 0;
		else EF->Tmp = 1;

		pEm->Spd_y = 0;
		EF->Cnt = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0) {
			Snd_se_enem(1, pEm);
			EF->Cnt2 = 14;
		}
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x60);
		if (pEm->Spd_x < 300) pEm->Spd_x += 5;
		Joint_move(pEm, Kan, Seq, 0x200);

		if (EF->Tmp == 0) {
			if (EF->Tmp_0 < pEm->Pos_y)
				pEm->Pos_y -= 40; }else
		if (pEm->Pos_y < EF->Tmp_0) pEm->Pos_y += 40;

		if (pEm->At_em_no == 0) {
			D0 = EF->Tmp_0 - pEm->Pos_y;
			if (D0 > -360 && D0 < 360) {
				if (pEm->Spd_x > 230) {
					Em21_r1_set(pEm, 12);
				}else {
					EF->Idly_cnt = Rnd() & 0x7f;
					EF->Idly_flg = Rnd_area();
					Em21_r1_set(pEm, 8);
				}
			}
		}else {
			Add_speedXYZ(pEm, 0, 0);
		}

	}
}

static void Em21_br_crash_i(ENEMY_WORK *pEm)
{
	Em21_run_away_ck(pEm); }

static void Em21_mv_crash_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:

		if (Cdir_ck(pEm, (ENEMY_WORK *)&G.Player)) {
			Em21_r0_set((ENEMY_WORK *)&G.Player, 2, 1);
		}else {
			Em21_r0_set((ENEMY_WORK *)&G.Player, 2, 0);
		}
		/* retail-only Dual Shock vibration: Pad_vib_set0 0x8003947C / Pad_vib_set1 0x80039514 (config/retail/main.symbols.txt;
		 * references/bio2-ds/Bio2/prog/PAD.C:309,325); implicit declarations as in this TU. */
		Pad_vib_set0(4, 0);
		Pad_vib_set1(10, 150, 0);
		Snd_se_enem(4, pEm);
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Spd_y = 0;
		pEm->Spd_x = 0;
		EF->Obj_cnt = 0;
		EF->Tmp_0 = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 2;

	case 2:

		EF->Tmp_0 += 20;
		pEm->Pos_y += EF->Tmp_0;

		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Snd_se_enem(5, pEm);
			pEm->Pos_y = D0;
			*(long *)&pEm->Move_no = 0x7000B;
			EF->Cnt = Rnd() & 0xf;
			pEm->Routine_2 = 3;
		}
		break;
	case 3:
		if (EF->Cnt-- <= 0) pEm->Routine_2 = 4;
		Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Move_cnt--;
		break;
	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400))

				Em21_r1_set(pEm, 9);
			else {	pEm->Timer0 = Rnd() & 0xf;
				Em21_r1_set(pEm, 0);
			}
		}

	}
}

static void Em21_br_peck_f_i(ENEMY_WORK *pEm)
{
	if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0xc0) || !(EF->Flg & 2)) {

		Em21_atd_set(pEm, 350);
		Em21_r1_set(pEm, 4);
	}
	Em21_run_away_ck(pEm);
}

static void Em21_mv_peck_f_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long D0, D1;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70004;
		EF->Tmp_0 = G.Player.Ground - 2880;
		D0 = EF->Tmp_0 - pEm->Pos_y;
		if (D0 < 0)

			EF->Tmp = 0;
		else EF->Tmp = 1;

		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Cdir_x = 0;
		pEm->Cdir_z = 0;
		EF->Cnt = 0;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0) {
			Snd_se_enem(1, pEm);
			EF->Cnt2 = 14;
		}
		if (EF->Flg & 4) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x60);
			if (pEm->Spd_x < 300) pEm->Spd_x += 4;
		}else {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x180);
			if (pEm->Spd_x < 300) pEm->Spd_x += 12;
		}
		Joint_move(pEm, Kan, Seq, 0x200);

		D1 = EF->Tmp_0 - pEm->Pos_y;
		if (D1 < -60 || D1 > 60) {
			if (D1 < -60) pEm->Pos_y -= 60;
			else
			if (D1 > 60)
				pEm->Pos_y += 60;
		}

		if (pEm->L_pl < 900)
			Em21_atd_set(pEm, 100);

		if (pEm->L_pl < 650) {
			D0 = EF->Tmp_0 - pEm->Pos_y;
			if (D0 > -100 && D0 < 100)
				Em21_r1_set(pEm, 14);

		}else {
			Add_speedXYZ(pEm, 0, 0);
		}

	}
}

static void Em21_br_peck_i(ENEMY_WORK *pEm)
{
	Em21_run_away_ck(pEm); }

static void Em21_mv_peck_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long D0;
	SVECTOR Svec;

	if (G.Player.Life <= 0)

		EF->Tmp = 0;
	else EF->Tmp = 1;

	switch (pEm->Routine_2) {
	case 0:
		if (G.Player.Life < 0) goto end;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Cdir_x = 0;
		pEm->Cdir_z = 0;
		if (EF->Flg & 4) {
			Em21_set_flg_all(8);
			Em21_set_flg_all(0x40);

			Em21_r0_set((ENEMY_WORK *)&G.Player, 5, 0);
			G.Player.pEnemy_ptr = (unsigned long *)pEm;
			G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
			G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		}

		pEm->Pos_y = EF->Tmp_0;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
		*(long *)&pEm->Move_no = 0x70005;
		EF->Cnt = 100;
		EF->Cnt2 = 0;
		pEm->Spd_x = 300;
		pEm->Routine_2 = 1;

	case 1:
		if (EF->Flg & 4) {
			Pad_vib_set0(6, 0); /* retail-only Dual Shock vibration, see Em21_mv_crash_i */
			Pad_vib_set1(4, 150, 1);
			Svec.vx = pEm->Pos_x;
			Svec.vy = pEm->Pos_y + 500;
			Svec.vz = pEm->Pos_z;
			Esp_call(0x11000, pEm->Cdir_y, &GsIDMATRIX, &Svec);
			if (!(Rnd() & 3))
				Em_21_feather_call(pEm, 1);

			Snd_se_enem(2, pEm);
			D0 = Pl_life_down(5, EF->Tmp);
			if (D0 == 1) goto end;
			if (D0 == 2) {
				Em21_set_flg_all(0x80);
				goto end;
			}
		}else {
			if (!(Rnd() & 7))
				Em_21_feather_call(pEm, 1);

		}
		EF->Cnt3 = 4;
		pEm->Routine_2 = 2;
	case 2:
		Joint_move(pEm, Kan, Seq, 0x200);
		EF->Cnt -= Gacha_ch() * 3 + 1;
		if ((signed char)EF->Cnt < 0) goto end;
		if (EF->Cnt3-- <= 0) {
			EF->Tmp_0 = 150;
			EF->Cnt3 = EF->Cnt2 = (Rnd() & 7) + 2;
			pEm->Routine_2 = 3;
		}
		break;
	case 3:
		Joint_move(pEm, Kan, Seq, 0x200);
		EF->Cnt -= Gacha_ch() * 3 + 1;
		if ((signed char)EF->Cnt < 0) goto end;
		pEm->Pos_y -= EF->Tmp_0;
		EF->Tmp_0 -= 15;
		if (EF->Cnt2-- <= 0) pEm->Routine_2 = 4;
		break;
	case 4:
		Joint_move(pEm, Kan, Seq, 0x200);
		EF->Cnt -= Gacha_ch() * 3 + 1;
		if ((signed char)EF->Cnt < 0) goto end;
		EF->Tmp_0 += 15;
		pEm->Pos_y += EF->Tmp_0;
		if (EF->Cnt3-- <= 0) {
			*(long *)&pEm->Move_no = 0x70005;
			pEm->Routine_2 = 1;
			EF->Cnt2 = 7;
		}
		break;
	case 5:
end:
		EF->Idly_cnt = Rnd() & 0x7f;
		EF->Idly_flg = Rnd_area();
		Em21_r1_set(pEm, 4);
		if (EF->Flg & 4) {
			Em21_atd_set(pEm, 350);
			Em21_set_flg_all(0x10);
			G.Player.Routine_2 = 3;
		}

	}
}

static void Em21_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	Em21_dm[ pEm->Routine_1 ](pEm, Kan, Seq);
}

static void Em21_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(3, pEm);
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Spd_y = 0;
		pEm->Spd_x = 0;
		EF->Sca_cnt = 0;

		EF->Tmp_0 = 0;

		D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

		EF->Tmp_1 = D0 - pEm->Cdir_y;
		G.pEm->Spd_x = 100;

		if (Rnd() & 1)
			EF->Tmp_2 = 200;
		else
			EF->Tmp_2 = -200;

		Em_21_blood_call(pEm);
		Em_21_feather_call(pEm, (Rnd() & 1) | 2);

		EF->Cnt = 3;
		EF->Cnt2 = 3;

		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 2;

	case 2:

		if (EF->Cnt-- <= 0) Em_21_feather_call(pEm, 1);
		if (EF->Cnt2-- <= 0) Em_21_feather_call(pEm, 1);

		EF->Tmp_0 += 20;
		pEm->Pos_y += EF->Tmp_0;

		Add_speedXZ(G.pEm, EF->Tmp_1);

		if (EF->Tmp_2 < 0) {
			if (pEm->Cdir_z > -2048) pEm->Cdir_z += EF->Tmp_2;
		}else {
			if (pEm->Cdir_z < 2048) pEm->Cdir_z += EF->Tmp_2;
		}

		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Snd_se_enem(5, pEm);
			pEm->Pos_y = D0;
			*(long *)&pEm->Move_no = 0x7000B;
			EF->Cnt = Rnd() & 0xf;
			pEm->Cdir_z = 0;
			if (pEm->Life < 0) {
				EF->Cnt = 11;
				Em_dead_flg_on(pEm);
				pEm->Routine_2 = 5;
			}else {
				pEm->Routine_2 = 3;
			}
		}
		break;
	case 3:
		if (EF->Cnt-- <= 0) pEm->Routine_2 = 4;
		Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Move_cnt--;
		break;
	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400)) {
				Em21_r0_set(pEm, 1, 9);
			}else {
				pEm->Timer0 = Rnd() & 0xf;
				Em21_r0_set(pEm, 1, 0);
			}
		}
		break;
	case 5:
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0)
			Em21_r0_set(pEm, 7, 0);

	}
}

static void Em21_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	register unsigned long Rnd0;

	pEm->Spd_y = 0;

	D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

	Rnd0 = Rnd();

	Em21_parts_bobm_set(pEm->pSin_parts_ptr, D0);

	if (Rnd0 & 1) {
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 1, D0);
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 2, D0);
	}else {
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 1, D0);
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 2, D0);
	}

	if (Rnd0 & 2) {
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 3, D0);
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 4, D0);
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 5, D0);
	}else {
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 3, D0);
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 4, D0);
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 5, D0);
	}

	if (Rnd0 & 4) {
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 6, D0);
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 7, D0);
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 8, D0);
	}else {
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 6, D0);
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 7, D0);
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 8, D0);
	}

	Em21_parts_bobm_set(pEm->pSin_parts_ptr + 9, D0);
	Em21_parts_bobm_set(pEm->pSin_parts_ptr + 10, D0);

	if (Rnd0 & 8) {
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 11, D0);
	}else {
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 11, D0);
	}

	if (Rnd0 & 0x10) {
		Em21_parts_bobm_set(pEm->pSin_parts_ptr + 12, D0);
	}else {
		Em21_parts_fall_set(pEm->pSin_parts_ptr + 12, D0);
	}

	Em_21_blood_call(pEm);
	Em_21_blood_call(pEm);
	Em_21_blood_call(pEm);
	Em_21_feather_call(pEm, (Rnd() & 3) | 8);

	Em_dead_flg_on(pEm);
	Em21_r0_set(pEm, 7, 1);
}

static void Em21_parts_bobm_set(PARTS_W *pP_ptr, long B_y)
{
	pP_ptr->Be_flg |= 0x4a;
	pP_ptr->Free[0] = B_y;
	pP_ptr->Free[4] = 0;
	pP_ptr->Free[2] = 800;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[3] = 10;
	pP_ptr->Free[6] = -100;
	pP_ptr->Poly_rgb = 0x101040; }

static void Em21_parts_fall_set(PARTS_W *pP_ptr, long D_y)
{
	pP_ptr->Be_flg |= 0x1062;
	pP_ptr->Old_x = D_y + (Rnd() << 2);
	pP_ptr->Timer3 = 0;
	pP_ptr->Old_z = (Rnd() & 0x7f) + 100;
	pP_ptr->Old_y = 0;
	pP_ptr->Timer1 = 20;
	pP_ptr->Old_x2 = 0;
	pP_ptr->Old_y2 = Rnd() << 4;
	pP_ptr->Old_z2 = 0;
}

static void Em21_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:
		Em_dead_flg_on(pEm);
		Snd_se_enem(3, pEm);
		Em21_atd_set(pEm, 100);
		pEm->Life = -1;

		Em_21_blood_call(pEm);
		Em_21_feather_call(pEm, (Rnd() & 3) + 3);

		pEm->Spd_x >>= 1;
		pEm->Spd_z >>= 1;
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 2;
	case 2:
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Spd_y += 20;

		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			pEm->Pos_y = D0;
			pEm->Cdir_z = 0;
			EF->Cnt = 8;
			pEm->Routine_2 = 3;
		}

		if (pEm->Sca_info & 1) {
			if (pEm->Be_flg & 0x4000) {
				pEm->Cdir_x = ((Rnd() & 1) << 11) + 0x400;
				pEm->Cdir_y = Rnd() << 4;
				pEm->Cdir_z = 0;
			}else {
				pEm->Cdir_z = ((Rnd() & 1) << 11) + 0x400;
				pEm->Cdir_x = Rnd() << 4;
				pEm->Cdir_y = 0;
			}
			*(long *)&pEm->Move_no = 4;
			pEm->Move_cnt = 3;
			Joint_move(pEm, Kan, Seq, 0x200);
			Em21_r0_set(pEm, 7, 2);
		}
		break;
	case 3:

		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Spd_x >>= 1;
		pEm->Spd_z >>= 1;
		if (EF->Cnt-- <= 0) {
			*(long *)&pEm->Move_no = 0x7000B;
			EF->Cnt = 11;
			pEm->Routine_2 = 4;
		}
		break;
	case 4:
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0)
			Em21_r0_set(pEm, 7, 0);

	case 5:

	}
}

static void Em21_dm03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register long D0;
	switch (pEm->Routine_2) {
	case 0:

		Em_dead_flg_on(pEm);
		Snd_se_enem(3, pEm);
		Em21_atd_set(pEm, 10);
		pEm->Life = -1;

		Em_21_blood_call(pEm);
		Em_21_feather_call(pEm, (Rnd() & 3) + 5);

		pEm->Cdir_y = G.Player.Cdir_y;
		pEm->Spd_x = 600;
		*(long *)&pEm->Move_no = 0x70009;
		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_2 = 2;
	case 2:
		Add_speedXZ(pEm, 0, 0);
		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 20;

		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Snd_se_enem(5, pEm);
			pEm->Pos_y = D0;
			pEm->Spd_y = 0;
			EF->Cnt = 16;
			pEm->Routine_2 = 3;
		}

		if (pEm->Sca_info & 1) {
			Snd_se_enem(3, pEm);
			Em_21_blood_call(pEm);
			pEm->Spd_y = 0;
			pEm->Spd_x = 20;
			pEm->Cdir_y += 0x800;
			pEm->Routine_2 = 4;
		}
		break;
	case 3:
		Add_speedXZ(pEm, 0, 0);
		pEm->Spd_x >>= 1;
		if (EF->Cnt-- <= 0)
			Em21_r0_set(pEm, 7, 3);

		break;
	case 4:
		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 20;
		Add_speedXZ(pEm, 0, 0);

		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Em21_atd_set(pEm, 350);
			Snd_se_enem(5, pEm);
			pEm->Pos_y = D0;
			pEm->Spd_y = 0;
			Em21_r0_set(pEm, 7, 3);
		}

	}

}

static void Em21_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em21_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);
	Em21_sce_mv[ pEm->Routine_1 ](pEm, Kan, Seq);
}

static void Em21_sce_wait(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em21_mv_wait_i(pEm, Kan, Seq);

	switch (pEm->Routine_3) {
	case 0:
		EF->Flg |= 0x20;
		pEm->Routine_3++;
	case 1:
		if (pEm->Sce_free0 & 1) {
			pEm->Sce_free0 = 0;
			Em21_r0_set(pEm, 1, 1);
		}
		break;
	case 2:
		EF->Flg &= ~0x20;
		pEm->Routine_3++;
	case 3:
		if (pEm->Sce_free0 & 1 || G.Room_player_flg & 1) {

			pEm->Sce_free0 = 0;
			Em21_r0_set(pEm, 1, 1);
		}
} }

static void Em21_sce_rush(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		if (pEm->Sce_free0 & 2) {
			pEm->Sce_free0 = 0;

			Em21_atd_set(pEm, 50);

			*(long *)&pEm->Move_no = 0x70004;
			pEm->Spd_x = 300;
			pEm->Spd_y = -20;
			EF->Cnt = 3;
			pEm->Routine_2 = 1;
			Snd_se_enem(1, pEm);
		}
		break;
	case 1:
		Goto00(pEm, -0x30D4, -0x3234, 0x60);
		Add_speedXYZ(pEm, 0, 0);
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0) {
			Snd_se_enem(1, pEm);
			EF->Cnt = 60;
			EF->Cnt2 = 5;
			pEm->Routine_2 = 2;
		}
		break;
	case 2:
		Goto00(pEm, -0xF4F, -0x3000, 0x60);
		Add_speedXYZ(pEm, 0, 0);
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0) {
			EF->Idly_cnt = 0xFF;
			EF->Idly_flg = 0;
			Em21_atd_set(pEm, 350);

			Em21_r0_set(pEm, 1, 4);
		}

	}
}

static void Em21_sce_crash(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		pEm->Be_flg |= 0x80008;
		pEm->Type |= 0x4000;
		pEm->Routine_2 = 1;
	case 1:
		if (pEm->Sce_free0 & 4) {
			pEm->Sce_free0 = 0;

			*(long *)&pEm->Move_no = 0x70004;
			pEm->Spd_x = 300;
			pEm->Spd_y = 0;
			pEm->Be_flg &= ~0x80000;
			pEm->Type &= ~0x4000;
			EF->Cnt = 6;
			pEm->Routine_2 = 2;
		}
		break;
	case 2:
		Add_speedXYZ(pEm, 0, 0);
		Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Spd_x -= 10;
		if (EF->Cnt-- <= 0) {
			pEm->Be_flg &= ~8;
			Em21_r0_set(pEm, 1, 4);
		}
	}
}

static void Em21_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em21_dead_mv[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em21_dead_normal(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register unsigned long D0;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Be_flg |= 2;
		pEm->Damage_cnt = 0x80;

		*(long *)&pEm->Move_no = 0x7000B;
		EF->Cnt = (Rnd() & 0x1F) + 0x19; pEm->Timer0 = EF->Cnt;

		Kage_work_color_set(pEm->pKage_work, 0xBFBF10);
		((KAGE_WORK *)pEm->pKage_work)->Half_x = 400;
		((KAGE_WORK *)pEm->pKage_work)->Half_z = 400;
		pEm->Timer1 = 60;

		pEm->Routine_2 = 1;

	case 1:
		if (EF->Cnt-- <= 0) {
			if (pEm->Timer0 > 0x5A) {
				pEm->Move_cnt = 11;
				EF->Cnt = 6;
			}else {
				if (Rnd() & 1) {
					pEm->Move_cnt = 0;
					EF->Cnt = 12;
				}else {
					pEm->Move_cnt = 11;
					EF->Cnt = 7;
				}
			}
			pEm->Routine_2 = 2;
		}

		if (pEm->Timer1) {
			pEm->Timer1--;
			((KAGE_WORK *)pEm->pKage_work)->Half_x += 10;
			((KAGE_WORK *)pEm->pKage_work)->Half_z += 10;
		}
		break;

	case 2:
		D0 = 1; if (pEm->Timer0 > 0x5A) {
			EF->Cnt2++;
			if (EF->Cnt2 & 1) D0 = 0;
		}

		if (D0) {
			if (!pEm->Move_cnt && (Rnd() & 1) && pEm->Timer0 < 0x5A) {
				Snd_se_enem(3, pEm);
			}
			Joint_move(pEm, Kan, Seq, 0x200);
			EF->Cnt--;
		}
		if (!EF->Cnt) {
			pEm->Timer0 += (Rnd() & 0xF) + 10;
			if (pEm->Timer0 > 0x96)

				pEm->Routine_2 = 3;
			else {	EF->Cnt = pEm->Timer0;
				pEm->Routine_2 = 1; }
		}

	}

}

static void Em21_dead_bomb(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {

		pEm->Be_flg |= 2;
		pEm->Damage_cnt = 0x80;
		((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;
		pEm->Routine_2 = 1;

	} }

static void Em21_dead_bow(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register long D0;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Be_flg |= 2;
		pEm->Damage_cnt = 0x80;
		pEm->Routine_2 = 1;
		EF->Cnt = (Rnd() & 0x1F) + 0x19;
		pEm->Timer0 = EF->Cnt;

		((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;

		pEm->Timer1 = 60;
		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt-- <= 0) {
			*(long *)&pEm->Move_no = 0x70004;
			if (pEm->Timer0 > 0x78) {
				EF->Cnt = (Rnd() & 1) * 5 + 10;
			}else {
				EF->Cnt = (Rnd() & 7) * 5 + 5;
			}

			pEm->Routine_2 = 2; } break;

	case 2:
		D0 = 1; if (pEm->Timer0 > 0x64) {
			EF->Cnt2++;
			if (EF->Cnt2 & 1) D0 = 0;
		}

		if (D0) {

			Joint_move(pEm, Kan, Seq, 0x200);
			if (pEm->Timer0 < 0x64) pEm->Move_cnt++;
			EF->Cnt--;
		}
		if (!EF->Cnt) {
			pEm->Timer0 += (Rnd() & 0xF) + 10;
			if (pEm->Timer0 > 0x96) {

				pEm->Spd_y = 0; pEm->Routine_2 = 3;
				break;
			}else {	EF->Cnt = pEm->Timer0;

				pEm->Routine_2 = 1; break; }
		}
		break;

	case 3:
		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 20;

		D0 = pEm->Ground - 250;
		if (D0 < pEm->Pos_y) {
			Em21_atd_set(pEm, 350);
			Snd_se_enem(5, pEm);
			*(long *)&pEm->Move_no = 0x7000B;
			pEm->Cdir_z = 0;
			pEm->Cdir_x = 0;
			pEm->Pos_y = D0;
			EF->Cnt = 11;
			pEm->Routine_2 = 4;
		}
		break;
	case 4:
		Joint_move(pEm, Kan, Seq, 0x200);
		if (EF->Cnt-- <= 0)
			pEm->Routine_2 = 5;
		break;
	case 5:

	}
}

static void Em21_dead_instant(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		pEm->Be_flg |= 2;
		pEm->Damage_cnt = 0x80;

		*(long *)&pEm->Move_no = 0x7000B;

		Kage_work_color_set(pEm->pKage_work, 0xBFBF10);
		((KAGE_WORK *)pEm->pKage_work)->Half_x = 400;
		((KAGE_WORK *)pEm->pKage_work)->Half_z = 400;
		pEm->Timer1 = 60;
		EF->Cnt = 11;

		pEm->Routine_2 = 1;
	case 1:
		if (EF->Cnt) {
			EF->Cnt--;
			Joint_move(pEm, Kan, Seq, 0x200);
		}
		if (pEm->Timer1) {
			pEm->Timer1--;
			((KAGE_WORK *)pEm->pKage_work)->Half_x += 10;
			((KAGE_WORK *)pEm->pKage_work)->Half_z += 10;
		}else {
			pEm->Routine_2 = 2;
		}
		break;
	case 2:

	}
}

static void Em21_foot_set(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	MATRIX M;

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, &pEm->M);
	CompM(&pEm->M, &pP_ptr->M, &M);
	CompM(&M, &pP_ptr[11].M, &M);

	pEm->Pos_x -= M.t[0] - pP_ptr[11].Workm.t[0];
	pEm->Pos_z -= M.t[2] - pP_ptr[11].Workm.t[2];
}

static void Em21_r0_set(ENEMY_WORK *pEm, unsigned long Rno0, unsigned long Rno1)
{
	pEm->Routine_0 = Rno0;
	pEm->Routine_1 = Rno1;
	*(unsigned short *)&pEm->Routine_2 = 0; }

static void Em21_r1_set(ENEMY_WORK *pEm, unsigned long Rno1)
{
	pEm->Routine_1 = Rno1;
	*(unsigned short *)&pEm->Routine_2 = 0; }

static void Em21_sca_avoid_ck(ENEMY_WORK *pEm)
{
	register unsigned long D0;
	SVECTOR Svec;
	register unsigned long Cdir_y;

	Cdir_y = pEm->Cdir_y & 0xFFF;

	if (pEm->Sca_info & 1) {
		if (!EF->Sca_cnt) {
			if (pEm->Spd_x > 0xE6) {
				Svec.vx = pEm->Pos_x;
				Svec.vy = pEm->Pos_y;
				Svec.vz = pEm->Pos_z;
				D0 = Sca_get_dir(&Svec, pEm->pSca_hit_data);
				if (D0 == 2) {
					if (Cdir_y < 0x80 || Cdir_y > 0xF80)
						Em21_r1_set(pEm, 10);

				}else {
					D0 <<= 10;
					if (((D0 + 0x780) & 0xFFF) < Cdir_y &&
						Cdir_y < ((D0 + 0x880) & 0xFFF))
						Em21_r1_set(pEm, 10);
				}

			}
		}
		if (EF->Sca_cnt > 10) {
			if (!(Rnd() & 0x7F)) {
				Em21_r1_set(pEm, 7);
			}else {
				Em21_r1_set(pEm, 8);
			}
		}
		if (pEm->Spd_x > 100) pEm->Spd_x -= 4;
		EF->Sca_cnt++;
	}else {
		EF->Sca_cnt = 0;
	}
}

static void Em21_attack_ck(ENEMY_WORK *pEm)
{

	if (G.Player.Life < 0) return;

	if ((ENEMY_FREE(pEm)[4] & 0xA0000) == 0xA0000) {

		Em21_r1_set(pEm, 13);
		EF->Flg &= ~0x40;
		EF->Flg &= ~0x08;
		EF->Na_cnt = 0;
		return;
	}

	if (EF->Na_cnt) {
		EF->Na_cnt--;
		return;
	}

	if (pEm->L_pl > 900 && pEm->L_pl < 7200 &&
		!(G.Room_enemy_flg & 1) &&
		(EF->Flg & 2) &&
		!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {

		G.Room_enemy_flg |= 1;
		EF->Flg |= 4;
		if (!(Rnd() & 0xF)) {
			Em21_r1_set(pEm, 11);
		}else {
			Em21_r1_set(pEm, 13);
		}
		return;
	}

	if (!pEm->At_em_no) {

		if (!EF->Obj_cnt && pEm->Spd_x > 0xE6 && !(G.Room_enemy_flg & 1))
			if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100))
				Em21_r1_set(pEm, 12);

		if (EF->Obj_cnt > 10) {
			Em21_r1_set(pEm, 8);
			EF->Idly_cnt = Rnd() & 0x7F;
			EF->Idly_flg = Rnd_area();
		}
		if (pEm->Spd_x > 100) pEm->Spd_x -= 4;
		EF->Obj_cnt++;
	}else {
		EF->Obj_cnt = 0;
	}
}

static void Em21_run_away_ck(ENEMY_WORK *pEm)
{

	if (EF->Flg & 0x10) {
		Em21_atd_set(pEm, 350);

		EF->Idly_cnt = Rnd() & 0x9D;
		EF->Idly_flg = Rnd_area();
		EF->Na_cnt = 30;
		Em21_r1_set(pEm, 4);
		EF->Flg &= ~0x58;
	}
}

static void Em21_pl_dead_ck(ENEMY_WORK *pEm)
{
	if (pEm->L_pl < 1800) {
		Em21_r1_set(pEm, 7);
	} }

static void Em21_lock_off_ck(ENEMY_WORK *pEm)
{
	register unsigned long D0;
	if (EF->Flg & 4) {
		D0 = pEm->Routine_1;
		if (D0 < 11 || D0 > 14) {
			G.Room_enemy_flg &= ~1;
			EF->Flg &= ~4;
		}
	}

	if (EF->Flg & 0x80) {
		EF->Flg &= ~0x80;
		EF->Pl_dead_cnt = 120;
	}
}

static void Em21_set_flg_all(unsigned long Mask)
{
	register ENEMY_WORK *pEm, **ppEm;

	for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
		pEm = *ppEm;
		if (pEm->Id == 0x21 && !(EF->Flg & 4)) {
			EF->Flg |= Mask;
			EF->Idly_cnt = 0;
			EF->Idly_flg = 0;
		}
	}
}

static void Em_21_blood_call(ENEMY_WORK *pEm)
{
	SVECTOR Svec;
	Svec.vx = (signed char)Rnd() + pEm->Pos_x;
	Svec.vy = (signed char)Rnd() + pEm->Pos_y;
	Svec.vz = (signed char)Rnd() + pEm->Pos_z;
	Esp_call(0x12800, Rnd() << 4, &GsIDMATRIX, &Svec);
}

static void Em_21_blood_call2(ENEMY_WORK *pEm)
{
	SVECTOR Svec;
	Svec.vx = pEm->Pos_x;
	Svec.vy = pEm->Pos_y;
	Svec.vz = pEm->Pos_z;
	Esp_call(0x50C00, pEm->Cdir_y, &GsIDMATRIX, &Svec);
}

static void Em_21_feather_call(ENEMY_WORK *pEm, long Num)
{
	SVECTOR Svec;
	while (Num-- > 0U) {	/* unsigned test: retail keeps the copy-then-decrement loop (no -1 rewrite) */
		Svec.vx = pEm->Pos_x + ((signed char)Rnd() << 2);
		Svec.vy = pEm->Pos_y + ((signed char)Rnd() << 2);
		Svec.vz = pEm->Pos_z + ((signed char)Rnd() << 2);
		Esp_call(0xC000800, Rnd() << 4, &GsIDMATRIX, &Svec);
	}
}

static void Em21_atd_set(ENEMY_WORK *pEm, long Size)
{
	pEm->Atd[0].At_w = Size;
	pEm->Atd[0].At_d = Size;
	pEm->Atd[0].W = Size;
	pEm->Atd[0].D = Size; }

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em21_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em21_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70000;
		pEm->Routine_2 = 1;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Move_no = 0x70001;
			pEm->Routine_2 = 2;
		}
		break;
	case 2:
		Joint_move(pEm, Kan, Seq, 0x200);
		break;
	case 3:
		if (G.Player.Life < 0) {
			G.Player.Routine_0 = 3;
			G.Player.Routine_1 = 0;
			G.Player.Routine_2 = 0;
			G.Player.Routine_3 = 0;
			break;
		}
		*(long *)&pEm->Move_no = 0x70002;
		pEm->Routine_2 = 4;

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Move_no = 0x70001;
			Em21_r0_set((ENEMY_WORK *)&G.Player, 1, 0);
		}
		break;
	case 5:
	}
}

static void Em21_dummy_br(ENEMY_WORK *pEm) {}
static void Em21_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq) {}
