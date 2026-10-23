/* em2d.c - reconstruction of D:\Bio2\prog\EM2D.C (enemy 2D).
 * Linked into two overlays from the same source: 0x12 (at 0x80100000) and
 * 0x30 (at 0x8010d000); only the enemy-number-specific entry names differ
 * (Em12d/Em22d, Pldie_Em12d/22d, Pldamege_Em12d/22d), selected by OVL_30.
 * Types come from the TU's retail SYM preamble (em2d.h). */
#include "EM2D.H"
/* Em2d work variables kept in the enemy's generic Free[] area */
#define EF	((EM2d_FREE *)EM_FREE_AREA(pEm))
/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of this translation unit ---- */
/* ---- data ---- */
static unsigned char Se_call;
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail life tables (em2d_cdemd0_s1 .data 0x801013B4..0x80101412): Em2b 0x12C -> 0xFA/0xC8, */
static unsigned short Em2b_life_easy = 0xC8;	/* plus a second set selected elsewhere by G.System_flg & 0x40; names follow src-retail/prog/em20.c ([INFERRED]) */
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static unsigned char P_hand[2][7] = { { 1, 2, 3, 4, 5, 6, 7 }, { 8, 9, 10, 11, 12, 13, 14 } };
static void (*Em2d_routine_0[8])() = {
	&Em2d_Init,
	&Em2d_Move,
	&Em2d_Damage,
	&Em2d_Die,
	&Em2d_Scenario,
	0,
	0,
	&Em2d_Dead
};
static void (*Em2d_Move_type[2])() = {
	&Em2d_Move_type00,
	&Em2d_Move_type01
};
static void (*Em2d_mv_br_type00[8])() = {
	&Em2d_mv00_br, &Em2d_mv01_br, &Em2d_mv02_br, &Em2d_mv03_br,
	&Em2d_mv04_br, &Em2d_mv05_br, &Em2d_mv06_br, &Em2d_mv07_br
};
static void (*Em2d_mv_type00[8])() = {
	&Em2d_mv00, &Em2d_mv01, &Em2d_mv02, &Em2d_mv03,
	&Em2d_mv04, &Em2d_mv05, &Em2d_mv06, &Em2d_mv07
};
static void (*Em2d_mv_br_type01[8])() = {
	&Em2d_mv00_br, &Em2d_mv01_br, &Em2d_mv02_br, &Em2d_mv03_br,
	&Em2d_mv04_br, &Em2d_mv05_br, &Em2d_mv06_br, &Em2d_mv07_br
};
static void (*Em2d_mv_type01[8])() = {
	&Em2d_mv00, &Em2d_mv01, &Em2d_mv02, &Em2d_mv03,
	&Em2d_mv04, &Em2d_mv05, &Em2d_mv06, &Em2d_mv07
};
static void (*Em2d_dm[9])() = { &Em2d_dm00, &Em2d_dm00, 0, &Em2d_dm00, &Em2d_dm00, 0, &Em2d_dm00, &Em2d_dm00, 0 };
static void (*Em2d_die[9])() = { &Em2d_die00, &Em2d_die00, 0, &Em2d_die00, &Em2d_die00, 0, &Em2d_die00, &Em2d_die00, 0 };
static void (*Em2d_sce[])() = { };
static void (*Pldie_Em2d_tbl[])() = { };
static void (*Pldamage_Em2d_tbl[4])() = {
	&Em2d_pldm00,
	&Em2d_pldm00,
	&Em2d_pldm00,
	&Em2d_pldm00
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	SVECTOR Sv;

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (G.Room_enemy_flg && (pEm->Type & 0x10))
			G.Room_enemy_flg--;

		if (Se_call && (pEm->Type & 0x10))
			Se_call--;

		Em2d_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Oba_ck_em(pEm);

	}

}

static void Em2d_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned char i;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;
	pEm->Spd_x = 0;
	pEm->Spd_z = 0;
	pEm->Spd_y = 0;

	G.pPldamage_Em[ 0x1D ] = PLDAMEGE_EM;

	pEm->Timer0 = Rnd() & 0xF;

	EF->Dir_y = pEm->Cdir_y;

	EF->Pos_x = pEm->Pos_x;
	EF->Pos_z = pEm->Pos_z;

	EF->Se_first = 0;

	pEm->Life = 250;
	*(signed char *)&pEm->D_life_u = 0x82;
	*(signed char *)&pEm->D_life_c = 0x82;
	*(signed char *)&pEm->D_life_d = 0x82;

	if (pEm->Type & 1) {

		pP_ptr = pEm->pSin_parts_ptr + P_hand[1][0];
		pEm->Neck_flg = 0;
		pEm->Neck_no = P_hand[1][0];
		pP_ptr->Free[0] = 0;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[2] = 0x60;
		pP_ptr->Free[3] = 0x60;
		pP_ptr->Free[4] = 0x200;
		pP_ptr->Free[5] = 0x138;
		i = 7;
		do {
			pP_ptr = pEm->pSin_parts_ptr + P_hand[0][ i - 1 ]; pP_ptr->Be_flg = 0;

		}while (--i);
		Hand_display(pEm, 1, 0);
	}else {

		pP_ptr = pEm->pSin_parts_ptr + P_hand[0][0];
		pEm->Neck_flg = 0;
		pEm->Neck_no = P_hand[0][0];
		pP_ptr->Free[0] = 0;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[2] = 0x60;
		pP_ptr->Free[3] = 0x60;
		pP_ptr->Free[4] = 0x200;
		pP_ptr->Free[5] = 0x138;
		i = 7;
		do {
			pP_ptr = pEm->pSin_parts_ptr + P_hand[1][ i - 1 ]; pP_ptr->Be_flg = 0;

		}while (--i);
		Hand_display(pEm, 0, 0);
	}

	G.Room_enemy_flg = 0;

	Se_call = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = 0;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 800;
	pEm->Atd[0].At_h = 500;
	pEm->Atd[0].At_d = 800;
	pEm->Atd[0].W = 800;
	pEm->Atd[0].D = 800;

	pEm->Eff_at_r = 800;

	pEm->Be_flg |= 0xC000000;

	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x100);

	pEm->Be_flg |= 0x1404;

	Kage_work_set(&pEm->pKage_work, 0, 0x2BC0258, 0, &pEm->pSin_parts_ptr->Workm.t[0]);

}

static void Em2d_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em2d_Move_type[ pEm->Type & 1 ](pEm, Kan, Seq); }

static void Em2d_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em2d_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em2d_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em2d_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em2d_mv_br_type01[ pEm->Routine_1 ](pEm);

	Em2d_mv_type01[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em2d_mv00_br(ENEMY_WORK *pEm)
{
	register short Dir1;
	register unsigned long LL;
	short Ck;	/* inferred name: no retail SYM record (single-use result is combined away) */
	LL = pEm->L_pl;

	Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x2F8);

	if (LL < 2500 && !Ck && pEm->Routine_2 >= 2) {
		*(long *)&pEm->Routine_0 = 0x101;
		Hand_display(pEm, pEm->Type & 1, 1);

		pEm->Damage_cnt = 0;
		pEm->Pos_x = EF->Pos_x;
		pEm->Pos_z = EF->Pos_z;

		if (EF->Se_first == 1) {
			if (!Se_call) {
				Snd_se_enem(4, pEm);
				Se_call = 60;
			}

		}else {
			EF->Se_first = 1; } }else

	if (LL < 3000 && !Dir1 && pEm->Routine_2 == 2) {
		pEm->Routine_2 = 3;
		Hand_display(pEm, pEm->Type & 1, 1);

		pEm->Damage_cnt = 0;

		if (EF->Se_first == 1) {
			if (!Se_call) {
				Snd_se_enem(4, pEm);
				Se_call = 60;
			}
		}else {
			EF->Se_first = 1;
		}
	}

}

static void Em2d_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0000;
		Joint_move(pEm, Kan, Seq, 0x100);
		pEm->Routine_2 = 1;

		pEm->Cdir_y = EF->Dir_y;

		pEm->Pos_x = EF->Pos_x;
		pEm->Pos_z = EF->Pos_z;

		pEm->Damage_cnt |= 0x80;
		pEm->Spd_x = 400;
		pEm->Spd_z = 0;
		pEm->Spd_y = 0;
		Add_speedXZ(pEm, 0x800);

	case 1:
		if ((unsigned short)pEm->Timer0-- <= 0) {
			pEm->Routine_2 = 2;
		}
		break;

	case 2: break;
	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Pos_x = EF->Pos_x;
			pEm->Pos_z = EF->Pos_z;
			break;
		}
		if (pEm->Move_cnt < 11) {
			pEm->Spd_x = 50;
			pEm->Spd_z = 0;
			pEm->Spd_y = 0;
			Add_speedXZ(pEm, 0);
		}
	}
}

static void Em2d_mv01_br(ENEMY_WORK *pEm)
{
	register PARTS_W *pPt;
	SVECTOR Sv;
	short Ck;	/* inferred name: no retail SYM record (single-use result is combined away) */

	pPt = pEm->pSin_parts_ptr + P_hand[ pEm->Type & 1 ][2];
	Ck = L_pos_ckXZ(&G.Player.Pos_x, &pPt->Workm.t[0], 900);

	if (Ck)
		*(long *)&pEm->Routine_0 = 0x301;

}

static void Em2d_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0xF0001;
		pEm->Routine_2 = 1;
		pEm->Timer0 = (Rnd() & 0x1F) + 60;

		if (Rnd() & 1) {
			if (Rnd() & 1) {
				Snd_se_enem(0, pEm); }
			else {
				Snd_se_enem(1, pEm);
			}
		}

	case 1:
		pEm->Cdir_y += Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		if (pEm->Cdir_y > EF->Dir_y + 0x180)
			pEm->Cdir_y = EF->Dir_y + 0x180;

		if (pEm->Cdir_y < EF->Dir_y - 0x180)
			pEm->Cdir_y = EF->Dir_y - 0x180;

		Joint_move(pEm, Kan, Seq, 0x100);

		if ((unsigned short)pEm->Timer0-- <= 0)
			*(long *)&pEm->Routine_0 = 0x501;

	}
}

static void Em2d_mv02_br(ENEMY_WORK *pEm)
{

}

static void Em2d_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em2d_mv03_br(ENEMY_WORK *pEm)
{
	register PARTS_W *pPt;
	SVECTOR Sv;
	short Ck;	/* inferred name: no retail SYM record (single-use result is combined away) */

	pPt = pEm->pSin_parts_ptr + P_hand[ pEm->Type & 1 ][2];
	Ck = L_pos_ckXZ(&G.Player.Pos_x, &pPt->Workm.t[0], 600);

	if (Ck && pEm->Move_cnt >= 5 && !G.Room_enemy_flg) {

		if (!(G.Player.Damage_cnt & 0x80)) {
			*(long *)&pEm->Routine_0 = 0x401;

			G.Player.Damage_cnt |= 0x80;
		}else {

			*(long *)&pEm->Routine_0 = 0x601;
		}
	}

}

static void Em2d_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0003;
		pEm->Routine_2 = 1;

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x30);

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x501;
	}
}

static void Em2d_mv04_br(ENEMY_WORK *pEm)
{

}

static void Em2d_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pPt;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0005;
		pEm->Routine_2 = 1;

		pPt = pEm->pSin_parts_ptr + P_hand[ pEm->Type & 1 ][2];
		G.Player.Pos_x = pPt->Workm.t[0];
		G.Player.Pos_z = pPt->Workm.t[2];

		G.Player.pEnemy_ptr = pEm;
		*(long *)&G.Player.Routine_0 = 5;

		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		Pad_vib_set0(4, 0);		/* retail-only vibration @0x80100C60..0x80100C84 */
		Pad_vib_set1(4, 0xfa, 0);
		Pad_vib_fade_set(0x64, 0x32, 0x96, 0);

		pEm->Timer1 = 150;

		if (G.Player.Life > 100)

			pEm->Timer1 = 150; break;
	case 1:
		if (!pEm->Move_cnt) {
			Pad_vib_set0(2, 0);		/* retail-only vibration @0x80100CC4..0x80100CE4 */
			Pad_vib_set1(1, 0xb4, 0);
			Pad_vib_set1(2, 0xfa, 1);
			if (Rnd() & 1) {
				Snd_se_enem(5, pEm); }

			else {
				Snd_se_enem(7, pEm);
			}
		}

		Joint_move(pEm, Kan, Seq, 0x100);

		pEm->Timer1 -= (int)Gacha_ch() * 2 + 1;
		if (pEm->Timer1 < 0) {
			pEm->Routine_2 = 2;

			G.Player.Routine_2 = 2;
		}
		break;

	case 2:
		G.Room_enemy_flg = 120;

		*(long *)&pEm->Routine_0 = 0x501;

	}
}

static void Em2d_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em2d_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0004;
		pEm->Routine_2 = 1;

		pEm->Damage_cnt |= 0x80;

		Snd_se_enem(6, pEm);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Timer0 = (Rnd() & 0x1F) + 30;
			Hand_display(pEm, pEm->Type & 1, 0);

			*(long *)&pEm->Routine_0 = 0x701;

			pEm->Life = -1;

			pEm->Damage_cnt |= 0x80;
		}
		pEm->Spd_x = 30;
		pEm->Spd_z = 0;
		pEm->Spd_y = 0;
		Add_speedXZ(pEm, 0x800);
	}
}

static void Em2d_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em2d_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern unsigned long Joint_move();	/* [INFERRED] block-scope declaration of the function called in this body: opens the record-less SYM block */

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0005;
		pEm->Routine_2 = 1;

		break;
	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);

		if (!(G.Player.Damage_cnt & 0x80))

			pEm->Routine_2 = 2; break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x101;
	}

}

static void Em2d_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em2d_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em2d_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;

	Em2d_dm[ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em2d_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;
	MATRIX M;
	VECTOR V;

	pEm->Damage_cnt |= 0x80;

	*(long *)&pEm->Routine_0 = 0x501;

	if (Rnd() & 1) {
		Snd_se_enem(2, pEm); }
	else {
		Snd_se_enem(3, pEm);
	}

}

static void Em2d_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern void (*Em2d_die[])();	/* [INFERRED] block-scope declaration of the table used here: opens the record-less SYM block */

	Em2d_die[ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em2d_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pPt;
	SVECTOR Sv;

	pPt = pEm->pSin_parts_ptr + P_hand[ pEm->Type & 1 ][0];

	Sv.vx = 0;
	Sv.vy = 0;
	Sv.vz = 0;
	Esp_call(0x21000, pEm->Cdir_y, &pPt->Workm, &Sv);

	pEm->Life = 250;

	pEm->Damage_cnt |= 0x80;

	*(long *)&pEm->Routine_0 = 0x501;

	if (Rnd() & 1) {
		Snd_se_enem(2, pEm); }
	else {
		Snd_se_enem(3, pEm);
	}

}

static void Em2d_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em2d_sce[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em2d_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em2d_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em2d_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Hand_display(ENEMY_WORK *pEm, unsigned char L_r, unsigned char On_off)
{
	register PARTS_W *pP_ptr;
	register unsigned char i;

	i = 7;
	do {
		pP_ptr = pEm->pSin_parts_ptr + P_hand[L_r][ i - 1 ];

		if (On_off) {

			pP_ptr->Be_flg |= 1;
		}else {

			pP_ptr->Be_flg &= ~1;
		}
	}while (--i);

	if (On_off) {

		pEm->Be_flg &= ~2;
	}else {

		pEm->Be_flg |= 2;
	}

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em2d_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em2d_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pAi;
	register unsigned char Cdir_ck_flg;

	pAi = pEm->pEnemy_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xF0000;
		pEm->Routine_2 = 1;

		Snd_se_stad((((Rnd() & 1) + 1) << 16) | 0x4000001, &pEm->Pos_x);

		Cdir_ck_flg = Cdir_ck(pEm, pAi);

		Goto00(pEm, pAi->Pos_x, pAi->Pos_z, 0x800);
		if (Cdir_ck_flg)
			pEm->Cdir_y += 0x800;

	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 2:
		*(long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 3;
	case 3:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 4:
		*(long *)&pEm->Routine_0 = 1;

		G.Player.Damage_cnt &= 0x7F;

	}
}
