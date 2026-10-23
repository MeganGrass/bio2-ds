/* em3b.c - reconstruction of D:\Bio2\prog\EM3B.C (enemy 3B).
 * Linked into two overlays from the same source: 0x1E (at 0x80100000) and
 * 0x3C (at 0x8010d000); only the enemy-number-specific entry name differs
 * (Em13b/Em23b), selected by OVL_3C.
 * Types come from the TU's retail SYM preamble (em3b.h). */
#include "EM3B.H"
/* Em3b work variables kept in the enemy's generic Free[] area */
#define EF	((EM3B_FREE *)EM_FREE(pEm))
/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;
/* ---- functions of other translation units ----
 * EM3B.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */
/* ---- functions of this translation unit ---- */
/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
/* Retail life tables (em3b_cdemd0_s1 .data 0x801013C8..0x80101427): Em2b values changed relative to Beta 2 and a
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
static void (*Em3b_routine_0[8])() = {
	&Em3b_Init,
	&Em3b_Move,
	&Em3b_Damage,
	&Em3b_Die,
	&Em3b_Scenario,
	0,
	0,
	&Em3b_Dead
};
static unsigned char init_routine_tbl[12] = { 0, 1, 2, 3, 3, 3, 3, 4, 5, 0, 0, 0 };
static unsigned char set_max_tbl[4] = { 24, 20, 13, 0 };
static void (*em3b_move_tbl[6])() = {
	&Em3b_Move_move,
	&Em3b_Move_fall,
	&Em3b_Move_oya,
	&Em3b_Move_wall,
	&Em3b_Move_oya2,
	&Em3b_Move_loop
};
static void (*move_oya_tbl[3])() = {
	&Em3b_mv_oya00,
	&Em3b_mv_oya10,
	&Em3b_mv_oya20
};
static unsigned char set_int_tbl[8] = { 0, 2, 0, 0, 0, 1, 2, 0 };
static void (*move_oya2_tbl[3])() = {
	&Em3b_mv_oya200,
	&Em3b_mv_oya10,
	&Em3b_mv_oya220
};
static unsigned char dieblood_type_tbl[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0 };
static unsigned char die_time_tbl[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 5, 15, 5, 45, 1, 15, 1, 15, 1, 1, 1, 5 };
static EM3B_BLOOD_TBL blood_id_tbl[5] = {
	{ 0, 1, 8, 0x1800 },
	{ 0, 1, 0, 0x1000 },
	{ 0, 1, 8, 0x2000 },
	{ 5, 4, 0, 0x1000 },
	{ 4, 15, 0, 0xC00 }
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		Em3b_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);
	}
}

static void Em3b_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3B_FREE *pFp;
	register unsigned long i;

	*(long *)&pEm->Routine_0 = (init_routine_tbl[ pEm->Type & 0xF ] << 8) + 1;

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++)
		EM_FREE(pEm)[i] = 0;

	pEm->Life = 1;
	pEm->D_life_u = 13;
	pEm->D_life_c = 13;
	pEm->D_life_d = 13;

	pEm->Be_flg |= 0x4400002;

	Kage_work_set(&pEm->pKage_work, 0, 0xC801F4, 0, &pEm->Pos_x);

	pEm->pEnemy_ptr = (ENEMY_WORK *)&G.Player;
	pEm->Neck_flg = 1;
	pEm->Neck_no = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -200;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 100;
	pEm->Atd[0].At_d = 100;
	pEm->Atd[0].At_h = 200;
	pEm->Atd[0].W = 200;
	pEm->Atd[0].D = 200;
	pEm->Eff_at_r = 200;
	pEm->nFloor = 0;	/* retail-only (0x801001A4) */

	pFp->Set_cnt = 0;
	pFp->Ck_cnt = 15;
	pFp->No_atck = 0;

	*(long *)&pEm->Move_no = (Rnd() & 0xF) << 8;
	Joint_move(pEm, Kan, Seq, 0x80);

	pEm->Be_flg |= 0x400000;
	switch (pEm->Type) {

	case 0x6001:	/* retail: child type 0x2001 -> 0x6001 (Set_child callers) */
		pEm->Be_flg |= 0x400;
		pEm->Old_x = pEm->Spd_x;
		pEm->Old_z = pEm->Spd_z;
		pEm->Cdir_x = pEm->Cdir_z = 0;

		pEm->Cdir_y = Rnd() << 4; break;

	case 0x6002:

		pEm->Be_flg |= 0x80400;
		pFp->Set_max = set_max_tbl[ Ck_em_id(pEm, 0, 0x3A, 0) ];
		break;

	case 0x2003:

		Em3b_Init_wall(pEm, Rnd() << 4, 0, 0x400);
		break;

	case 0x2004:

		Em3b_Init_wall(pEm, 0x400, (Rnd() << 4) + 0x400, 0);
		break;

	case 0x2005:

		Em3b_Init_wall(pEm, Rnd() << 4, 0, -0x400);
		break;

	case 0x2006:

		Em3b_Init_wall(pEm, -0x400, (Rnd() << 4) + 0x400, 0);
		break;

	case 0x2008:

		EF->Flgset = 1;

	default:

		pEm->Cdir_y = Rnd() << 4;

	}
	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

}

static void Em3b_Init_wall(ENEMY_WORK *pEm, short rx, short ry, short rz)
{
	pEm->Cdir_x = rx;
	pEm->Cdir_y = ry;
	pEm->Cdir_z = rz;
	pEm->Be_flg |= 0x800040C;
	pEm->Atd[0].Ofs_y = 0;
	pEm->Eff_at_r = 400;
	EF->Flgset = 1;

	if (G.Player.Pos_y - 3600 < pEm->Pos_y) {
		pEm->Be_flg |= 0x4000000;
	} }

static void Em3b_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3B_FREE *pFp;

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	if (pFp->No_atck) {
		if (!--pFp->No_atck)
			G.Room_enemy_flg &= ~0x10;
	}

	if (pFp->No_set)
		pFp->No_set--;

	em3b_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);
}

static void Em3b_Move_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Flg_ck(G.SaveData.Room_flg, 0x1F))	/* retail-only room-flag guard (0x801004A8) */
		return;
	if (!pEm->Routine_2) {
		Em3b_mv_move00(pEm, Kan, Seq);
	}else {

		Em3b_mv_move10(pEm, Kan, Seq);
	} }

static void Em3b_mv_move00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short dir;
	register EM3B_FREE *pFp;
	VECTOR goal;

	goal.vx = G.Player.Pos_x;
	goal.vz = G.Player.Pos_z;
	pFp = (EM3B_FREE *)EM_FREE(pEm);
	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		pEm->Spd_x = 2;
		pEm->Timer2 = (Rnd() + 1) & 7;

	case 1:

		if (!*(signed char *)&pEm->Timer2) {
			dir = Goto00_ck(pEm, goal.vx, goal.vz, 2);
			pEm->Timer2 = 4;

		}else dir = 0; break;

	case 2:

		goal.vy = 0; Root_ck(pEm, &goal, 0, 0);
		dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 2);
		if (!--pEm->Timer0)
			pEm->Routine_3 = 1;

	}

	pEm->Timer2--;
	pEm->Cdir_y += !pFp->No_atck ? dir : -dir;
	Add_speedXZ(pEm, 0);

	if (!EF->No_atck && !(G.Room_enemy_flg & 0x10)) {
		if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {
			if (pEm->L_pl < 1800 && ((0x27E4 >> (Rnd() & 0xF)) & 1)) {
				G.Room_enemy_flg |= 0x10;
				pFp->No_atck = 90;
				*(short *)&pEm->Routine_2 = 1;
			}
		}
	}

	Joint_move(pEm, Kan, Seq, 0x80);
}

static void Em3b_mv_move10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Ofs, Rot;
	register PARTS_W *pP;	/* inferred name: no retail SYM record (CSE folds it into the parts base) */

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0x3F0001;
		pEm->Timer0 = 30;

	case 1:
		Joint_move(pEm, Kan, Seq, 0x40);
		if (!--pEm->Timer0)
			pEm->Routine_3 = 2;

		break;

	case 2:
		pEm->Routine_3 = 3;
		pP = pEm->pSin_parts_ptr + 2;
		Ofs.vx = 0;
		Ofs.vy = 0;
		Ofs.vz = 0;
		Rot.vx = 0;
		Rot.vy = 0;
		Rot.vz = 0;
		Esp_call3D(0x2B001000, pEm->Cdir_y, &pP->Workm, &Ofs, &Rot);
		*(long *)&pEm->Move_no = 0x3F0000;
		Snd_se_enem(0, pEm);

	case 3:
		Joint_move(pEm, Kan, Seq, 0x40);
		if (!pEm->Hokan_flg)
			*(short *)&pEm->Routine_2 = 0;

	}

}

static void Em3b_Move_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Flg_ck(G.SaveData.Room_flg, 0x1F))	/* retail-only room-flag guard (0x801008B4) */
		return;
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Spd_x = pEm->Spd_z = 0;
		pEm->Spd_y = 40;

		pEm->Cdir_z = 0x200;
		pEm->Damage_cnt |= 0x80;
		pEm->Type &= ~0x4000;
	}

	if (pEm->Spd_y < 600)
		pEm->Spd_y += 40;

	Add_speedXYZ(pEm, 0, -pEm->Cdir_z);

	if (pEm->Pos_y >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground;
		pEm->Cdir_z = 0;
		pEm->Routine_1 = 0;
		*(short *)&pEm->Routine_2 = 0;
		pEm->Damage_cnt &= 0x7F;
		Snd_se_enem(1, pEm);
	}
	Joint_move(pEm, Kan, Seq, 0x80);
}

static void Em3b_Move_oya(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	move_oya_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Ck_total(pEm); }

static void Em3b_mv_oya00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;
	register EM3B_FREE *pFp;

	pTg = &G.Player;

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Timer0 = 300 - (Rnd() & 0x1F);
	}

	if (pEm->Sce_free0) {

		if (*(unsigned short *)&G.Player.Routine_0 == 0x101 || *(unsigned short *)&G.Player.Routine_0 == 0x301) {
			pEm->Timer0 -= 10;
		}else

		if (*(unsigned short *)&G.Player.Routine_0 == 0x201)
			pEm->Timer0 -= 15;

		if (--pEm->Timer0 <= 0 || pTg->Life < 0) {
			if ((0x5335 >> (Rnd() & 0xF)) & 1) {
				pFp->Em_keep = ((0x990 >> (Rnd() & 0xF)) & 1) ? 5 : 2;
				pFp->Keep_x = pTg->Pos_x;
				pFp->Keep_z = pTg->Pos_z;
				*(short *)&pEm->Routine_2 = 1;
			}
			pEm->Timer0 = 0;
		}
	} }

static void Em3b_mv_oya10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pWk;
	register EM3B_FREE *pFp;
	register long x;
	long z;	/* inferred name: no retail SYM record (single-use value is combined into its use) */

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	if (!pEm->Routine_3) {
		while (pFp->Em_keep-- > 0) {
			x = pFp->Keep_x + (((Rnd() & 0xF) - 7) << 4);
			z = pFp->Keep_z + (((Rnd() & 0xF) - 7) << 4);
			if ((pWk = Set_child(pEm, 0x6001, x, z))) {
				pWk->Spd_x = pFp->Keep_x;
				pWk->Spd_z = pFp->Keep_z;
			}else {

				pEm->Routine_2++;
				pEm->Routine_3 = 0;
				return;
			}

			pEm->Timer0 = set_int_tbl[ Rnd() & 7 ];
			if (pEm->Timer0) {
				pEm->Routine_3 = 1;
				return;
			}
		}
		pEm->Routine_2++;
		pEm->Routine_3 = 0;
	}else {

		if (!--pEm->Timer0)
			pEm->Routine_3 = 0;
	}

}

static void Em3b_mv_oya20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(short *)&pEm->Routine_2 = 0; }

static void Ck_total(ENEMY_WORK *pEm)
{

	if (!EF->Ck_cnt) {
		EF->Set_cnt = Ck_em_id(pEm, 0, 0x3B, 0);

		EF->Ck_cnt = 15; }
	else
		EF->Ck_cnt--;

}

static void Em3b_Move_wall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Joint_move(pEm, Kan, Seq, 0x80); }

static void Em3b_Move_loop(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3B_FREE *pFp;

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Spd_x = 2;
		pEm->Spd_y = pEm->Spd_z = 0;
		pFp->Keep_x = (Rnd() & 1) ? 2 : -2;
		pEm->Timer0 = 0x1000;
	}

	pEm->Cdir_y += pFp->Keep_x;
	Add_speedXZ(pEm, 0);
	Joint_move(pEm, Kan, Seq, 0x80);

	pEm->Timer0 -= 2;
	if (pEm->Timer0 < 0) {
		pFp->Keep_x *= -1;
		pEm->Timer0 = 0x1000;
	}

	if (G.Player.Type == 0x11) {	/* retail-only hit-box adjustment (0x80100DCC) */
		pEm->Atd[0].Ofs_y = pEm->Pos_y;
		pEm->Atd[0].At_h = -pEm->Pos_y;
	}else {
		pEm->Atd[0].Ofs_y = -200;
		pEm->Atd[0].At_h = 200;
	}

}

static void Em3b_Move_oya2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_oya2_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Ck_total(pEm); }

static void Em3b_mv_oya200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM3B_FREE *pFp;	/* no retail SYM record: CSE folds it into EM_FREE(pEm) */

	pFp = (EM3B_FREE *)EM_FREE(pEm);

	if (pEm->Sce_free0) {
		pEm->Sce_free0 = 0;
		pFp->Em_keep = 5;
		pFp->Keep_x = pEm->Pos_x;
		pFp->Keep_z = pEm->Pos_z;
		*(short *)&pEm->Routine_2 = 1;
	}
}

static void Em3b_mv_oya220(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(short *)&pEm->Routine_2 = 0; }

static void Em3b_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em3b_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register KAGE_WORK *pKage;
	register PARTS_W *pP_ptr;
	register unsigned long i;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, Kan, Seq, 0x80);
		Set_blood(pEm, 0, dieblood_type_tbl[ pEm->Routine_1 - 1 ]);

		*(long *)&pKage->Prim[0].r0 &= 0xFF000000;
		*(long *)&pKage->Prim[0].r0 |= 0xAF4FAF;
		*(long *)&pKage->Prim[1].r0 &= 0xFF000000;
		*(long *)&pKage->Prim[1].r0 |= 0xAF4FAF;

		if (EF->Flgset) {
			Em_dead_flg_on(pEm);
		}

		pEm->Timer0 = 0x1000;

		for (i = 0; i < 3; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Be_flg |= 0x480;
			pP_ptr->Size_x = 0x1000;
			pP_ptr->Size_y = 0x1000;
			pP_ptr->Size_z = 0x1000;
			*(long *)&pP_ptr->Free[0] = 0x4000;
			*(long *)&pP_ptr->Free[2] = 0x101010;
		}
	}else {

		for (i = 0; i < 3; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Size_y = pEm->Timer0;
		}
		pEm->Timer0 -= 0x200;
		pKage->Half_z += 0x20;
		pKage->Half_x += 0x10;
		if (pEm->Timer0 < 0x100) {
			*(long *)&pEm->Routine_0 = (pEm->Routine_1 << 8) + 7;
			pEm->Timer0 = 0x1000;
		}
	}
}

static void Em3b_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em3b_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register KAGE_WORK *pKage;
	register PARTS_W *pP_ptr;
	register unsigned long i;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		pEm->Timer0 = die_time_tbl[ pEm->Routine_1 - 1 ];

	case 1:
		if (G.Stop_flg & 0x80000000)	/* retail-only pause guard (0x801010BC) */
			break;
		if (!--pEm->Timer0) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = 0x1000;
		}
		break;

	case 2:
		if (G.Stop_flg & 0x80000000)	/* retail-only pause guard (0x801010FC) */
			break;
		for (i = 0; i < 3; i++) {
			pP_ptr = pEm->pSin_parts_ptr + i;
			pP_ptr->Size_x = pP_ptr->Size_z = pEm->Timer0;
		}
		pEm->Timer0 -= 0x80;
		if (pEm->Timer0 < 0)
			pEm->Timer0 = 0;

		if (pKage->Half_z) {
			pKage->Half_z -= 4;
			if (pKage->Half_z <= 0)
				pKage->Half_z = 0;
		}

		if (pKage->Half_x) {
			pKage->Half_x -= 12;
			if (pKage->Half_x <= 0) {
				if (EF->No_atck)
					G.Room_enemy_flg &= ~0x10;

				pKage->Be_flg = 0;
				Kill_enemy_work(pEm);
			}
		}
	}

}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register long Id;
	register EM3B_BLOOD_TBL *tbl;
	long Dir;	/* inferred name: no retail SYM record (single-use result is combined into its use) */

	tbl = &blood_id_tbl[ type ];

	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

	pP_ptr = pEm->pSin_parts_ptr + parts;

	Id = tbl->Id & 0x7F;

	Sv.vx = pP_ptr->Workm.t[0];
	Sv.vy = pP_ptr->Workm.t[1];
	Sv.vz = pP_ptr->Workm.t[2];
	Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, Dir, &GsIDMATRIX, &Sv);
}

static void Em3b_Pl_to_dmg(unsigned long pl, unsigned char dir)
{
	register PLAYER_WORK *pPl;

	pPl = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
}

static ENEMY_WORK *Set_child(ENEMY_WORK *pEm, unsigned long Type, long Px, long Pz)
{
	register ENEMY_WORK *pWk;

	if (EF->Set_cnt < EF->Set_max) {
		if (!(pWk = Set_enemy_work(0x3B))) return 0;
		if (pEm->Be_flg & 0x2000)
			pWk->Be_flg |= 0x2000;

		pWk->Type = Type;
		pWk->Pos_x = Px;
		pWk->Pos_y = pEm->Pos_y;
		pWk->Pos_z = Pz;
		EF->Set_cnt++;
		return pWk;
	}

	return 0;

}
