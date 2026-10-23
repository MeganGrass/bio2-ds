/* em10.c - reconstruction of D:\Bio2\prog\EM10.C (enemy 10: zombie AI).
 * Linked into two overlays from the same source: 0x05 (EM10 at 0x801002a0)
 * and 0x23 (at 0x8010d2a0); only the enemy-number-specific entry names differ
 * (Em110/Em210, Pldamege_Em110/210, Pldie_Em110/210), selected by OVL_23.
 * Types come from the TU's retail SYM preamble (em10.h). */
#define EM10_C	/* em10_s0.c is #included below: one object, two SLD source files */
#include "EM10.H"
#include "SPL_TBL.H"

/* Em10 work variables kept in the enemy's generic Free[] area (byte offsets into it) */
#define FREE_UB(o)	(((unsigned char *)OVL_FREE(pEm))[o])
#define FREE_US(o)	(*(unsigned short *)((unsigned char *)OVL_FREE(pEm) + (o)))
#define FREE_SS(o)	(*(short *)((unsigned char *)OVL_FREE(pEm) + (o)))
#define FREE_SB(o)	(((signed char *)OVL_FREE(pEm))[o])
#define FREE_US_P(p,o)	(*(unsigned short *)((unsigned char *)OVL_FREE(p) + (o)))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of this translation unit ---- */

/* ---- functions of D:\Bio2\prog\EM10_S0.C (#included at the end of this file) ---- */

/* ---- functions of other translation units ---- */
/* PsyQ libgte (declared: retail SYM has no implicit-declaration blocks at their calls) */
/* used by the retail Em10_ude_down */

/* ---- data ---- */

static unsigned char Em10_pldm00_hajiki[4];
static unsigned char Em10_hajiki_no;
static unsigned short Em10_life[16] = { 0x46, 0x54, 0x76, 0x41, 0x32, 0x55, 0x30, 0x41, 0x28, 0x49, 0x45, 0x38, 0x46, 0x37, 0x48, 0x37 };	/* retail values (retail .data 0x8010C600) */
static unsigned short Em10_life_easy[16] = { 0x27, 0x0D, 0x1B, 0x27, 0x36, 0x1B, 0x27, 0x55, 0x1B, 0x27, 0x1B, 0x1B, 0x27, 0x1B, 0x27, 0x1B };	/* retail values (0x8010C620) */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail 0xFA (0x8010C66C) */
static unsigned short Em2b_life_easy = 0xC8;	/* retail 0xC8 */
/* retail-only second set, selected by G.System_flg & 0x40 ([INFERRED] _flg40 names as in em20/em25;
 * retail .data 0x8010C670..0x8010C6DF) */
static unsigned short Em10_life_flg40[16] = { 0x50, 0x5E, 0x80, 0x4B, 0x3C, 0x5F, 0x3A, 0x4B, 0x32, 0x53, 0x4F, 0x42, 0x50, 0x41, 0x52, 0x41 };
static unsigned short Em10_life_easy_flg40[16] = { 0x36, 0x1C, 0x36, 0x36, 0x46, 0x28, 0x36, 0x64, 0x28, 0x36, 0x46, 0x36, 0x36, 0x46, 0x36, 0x28 };
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static void (*Em10_routine_0[9])() = {
	&Em10_Init,
	&Em10_Move,
	&Em10_Damage,
	&Em10_Die,
	Spl_Scenario,
	0,
	0,
	&Em10_Dead,
	&Em10_Scenario
};
static void (*Em10_Move_type[14])() = {
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01,
	&Em10_Move_type00,
	&Em10_Move_type01
};
static void (*Em10_mv_br_type00[16])() = {
	&Em10_mv00_br,
	&Em10_mv01_br,
	&Em10_mv02_br,
	&Em10_mv03_br,
	&Em10_mv05_br,
	&Em10_mv06_br,
	&Em10_mv08_br,
	&Em10_mv07_br,
	&Em10_mv0a_br,
	&Em10_mv0b_br,
	&Em10_mv0c_br,
	&Em10_mv0d_br,
	&Em10_mv0e_br,
	&Em10_mv0f_br,
	&Em10_mv10_br,
	&Em10_mv11_br
};
static void (*Em10_mv_type00[16])() = {
	&Em10_mv00,
	&Em10_mv01,
	&Em10_mv02,
	&Em10_mv03,
	&Em10_mv05,
	&Em10_mv06,
	&Em10_mv08,
	&Em10_mv07,
	&Em10_mv0a,
	&Em10_mv0b,
	&Em10_mv0c,
	&Em10_mv0d,
	&Em10_mv0e,
	&Em10_mv0f,
	&Em10_mv10,
	&Em10_mv11
};
static void (*Em10_mv_br_type01[3])() = {
	&Em10_mv04_br,
	&Em10_mv03_br,
	&Em10_mv09_br
};
static void (*Em10_mv_type01[3])() = {
	&Em10_mv04,
	&Em10_mv03,
	&Em10_mv09
};
static unsigned short Em10_walk_rnd[32] = { 0xBE, 0x8032, 0x96, 0x8040, 0x6E, 0x8040, 0xB4, 0x8034, 0x8C, 0x8034, 0x78, 0x803A, 0x96, 0x804E, 0x8C, 0x8032, 0x96, 0x803E, 0x96, 0x8048, 0x6E, 0x8046, 0x78, 0x804A, 0xB4, 0x8022, 0x6E, 0x8034, 0xC8, 0x8042, 0xB4, 0x8042 };
static void (*Em10_dm[18][9])() = {
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	},
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	},
	{
		&Em10_dm05,
		&Em10_dm02,
		0,
		&Em10_dm05,
		&Em10_dm01,
		0,
		&Em10_dm05,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm05,
		&Em10_dm02,
		0,
		&Em10_dm05,
		&Em10_dm01,
		0,
		&Em10_dm05,
		&Em10_dm01,
		0
	},
	{
		&Em10_dm05,
		&Em10_dm01,
		&Em10_dm01,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00
	},
	{
		&Em10_dm01,
		&Em10_dm01,
		&Em10_dm01,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00
	},
	{
		&Em10_dm01,
		&Em10_dm01,
		&Em10_dm01,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00,
		&Em10_dm00
	},
	{
		&Em10_dm05,
		&Em10_dm04,
		0,
		&Em10_dm05,
		&Em10_dm04,
		0,
		&Em10_dm05,
		&Em10_dm04,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		&Em10_dm08,
		&Em10_dm08,
		0,
		&Em10_dm08,
		&Em10_dm08,
		0,
		&Em10_dm08,
		&Em10_dm08,
		0
	},
	{
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0,
		&Em10_dm00,
		&Em10_dm00,
		0
	},
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	},
	{
		&Em10_dm08,
		&Em10_dm08,
		0,
		&Em10_dm08,
		&Em10_dm08,
		0,
		&Em10_dm08,
		&Em10_dm08,
		0
	}
};
static void (*Em10_haidm[18])() = {
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06,
	&Em10_dm06
};
static unsigned char Em10_dm00_usonum[17] = { 9, 15, 15, 35, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0 };
static void (*Em10_die[18][9])() = {
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_dm05,
		&Em10_die03,
		0,
		&Em10_dm05,
		&Em10_die03,
		0,
		&Em10_dm05,
		&Em10_die03,
		0
	},
	{
		&Em10_dm05,
		&Em10_die03,
		0,
		&Em10_dm05,
		&Em10_die03,
		0,
		&Em10_dm05,
		&Em10_die03,
		0
	},
	{
		&Em10_dm05,
		&Em10_dm02,
		0,
		&Em10_dm05,
		&Em10_die00,
		0,
		&Em10_dm05,
		&Em10_die00,
		0
	},
	{
		&Em10_dm05,
		&Em10_die02,
		0,
		&Em10_dm05,
		&Em10_die04,
		0,
		&Em10_dm05,
		&Em10_die00,
		0
	},
	{
		&Em10_dm05,
		&Em10_die02,
		&Em10_die02,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00
	},
	{
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00
	},
	{
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00,
		&Em10_die00
	},
	{
		&Em10_die00,
		&Em10_die04,
		0,
		&Em10_die00,
		&Em10_die04,
		0,
		&Em10_die00,
		&Em10_die04,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	},
	/* rows 16/17: retail image words (em10 slot1 .data+0x888), resolved through em10_cdemd0_s1.symbols.txt */
	{
		&Em10_dm05,
		&Em10_die02,
		0,
		&Em10_dm05,
		&Em10_die04,
		0,
		&Em10_dm05,
		&Em10_die00,
		0
	},
	{
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0,
		&Em10_die00,
		&Em10_die00,
		0
	}
};
static void (*Em10_haidie[18])() = {
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01,
	&Em10_die01
};
static void (*Em10_sce[5])() = {
	&Em10_sce00,
	&Em10_sce01,
	&Em10_sce02,
	&Em10_sce03,
	&Em10_sce04
};
static void (*Pldamage_Em10_tbl[4])() = {
	&Em10_pldm00,
	&Em10_pldm00,
	&Em10_pldm02,
	&Em10_pldm02
};
static void (*Pldie_Em10_tbl[2])() = {
	&Em10_pldie00,
	&Em10_pldie00
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register VECTOR *pTv;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */

	pPl = (ENEMY_WORK *)&G.Player;
	pTv = (VECTOR *)&pPl->Pos_x;

	if (G.Status_flg & 0x10000000) {
		if ((pEm->L_spl & 0xc0000000) == 0x40000000)

			pTv = (VECTOR *)&G.pEnemy[1]->Pos_x;

		/* retail: the two Beta L_spl < L_pl choices are replaced by a guide-area test that
		 * falls back to the player (retail 0x80100320..0x80100344) */
		if (Guide_check(pTv->vx, pTv->vz) == 0xff)
			pTv = (VECTOR *)&pPl->Pos_x;
	}

	Root_ck(pEm, pTv, FREE_UB(0x1d), FREE_US(2) & 0x400);

	if (!(G.Stop_flg & 0x20000000)) {

		if ((pEm->Type & 1) || (FREE_US(2) & 2))
			pEm->Be_flg |= 0x4000000;

		if ((pEm->Be_flg & 0xc000000) == 0x4000000) {

			pEm->Be_flg |= 0x4000000;

			if ((pPl->Type & 0xfff) != 0xe && (pPl->Type & 0xfff) != 0x12) {	/* retail: + type 0x12 */

				if ((pPl->Status_flg & 0x8000) && pPl->Pos_y - 1500 < pEm->Pos_y)

					pEm->Be_flg &= ~0x4000000;

				if ((pPl->Status_flg & 0x4000) && pPl->Pos_y - 1000 < pEm->Pos_y)

					pEm->Be_flg &= ~0x4000000;
			}
		}

		if (FREE_UB(0x21))
			FREE_UB(0x21)--;
		if (FREE_UB(0x26))	/* retail-only timer (set to 60 by Em10_mv10) */
			FREE_UB(0x26)--;

		if (pEm->Damage_cnt & 0x7f)
			pEm->Damage_cnt--;

		if (FREE_UB(0x18)) {
			FREE_UB(0x18)--;
			G.Room_enemy_flg |= 0x100;	}

		Em10_routine_0[ pEm->Routine_0 ](pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);

		Em10_ude_down(pEm);

		FREE_US(0x1e)++;

		pEm->pEnemy_ptr = (unsigned long *)pPl;
		pEm->Neck_flg &= 0xfd;

		if ((pEm->Type & 0x4000) || (FREE_US(2) & 0x200))
			pEm->Neck_flg |= 2;

		Rot_neck_em(pEm, pEm->Cdir_y);

		Em10_Kahannsin_walk(pEm, (unsigned long)pEm->pSub0_kan_t_ptr, (unsigned long)pEm->pSub0_seq_t_ptr);

		if (pEm->Water && G.Player.Life >= 0) {
			pP_ptr = pEm->pSin_parts_ptr;
			if (pP_ptr->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 0xf) == 1 && pEm->Spd_x >= 11)

				Em10_water(pEm);

			if ((*pEm->pNow_seq & 0xf8000000) == 0x8000000)

				Em10_water(pEm);

			if ((*pEm->pNow_seq & 0xf8000000) == 0x48000000)

				Em10_water(pEm);
		}

		Em10_Kubigoro(pEm);

		OVL_FREE(pEm)[4] = *(unsigned long *)&pEm->Routine_0;

		Em_seq_sound(pEm);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);
	}

}

static void Em10_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long Tex;

	unsigned char Em10_walk_no[8] = { 0, 2, 0, 2, 0, 2, 2, 0 };

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	if (G.System_flg & 0x40) {	/* retail: second life set (retail 0x801006C8..0x8010075C) */
		pEm->Life = Em10_life_flg40[ (Rnd() >> (Rnd() & 3)) & 0xf ];
		if (G.System_flg & 0x20)
			pEm->Life = Em10_life_easy_flg40[ (Rnd() >> (Rnd() & 3)) & 0xf ];
	}else {
		pEm->Life = Em10_life[ (Rnd() >> (Rnd() & 3)) & 0xf ];
		if (G.System_flg & 0x20)
			pEm->Life = Em10_life_easy[ (Rnd() >> (Rnd() & 3)) & 0xf ];
	}

	if (!(G.System_flg & 0x20) && G.Enemy_num < 4)	/* retail: not on easy */
		pEm->Life += 15;

	pEm->D_life_u = 13;
	pEm->D_life_c = 13;
	pEm->D_life_d = 13;

	pP_ptr = &pEm->pSin_parts_ptr[8];
	pEm->Neck_flg = 0;
	pEm->Neck_no = 8;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 0x60;
	pP_ptr->Free[3] = 0x60;
	pP_ptr->Free[4] = 0x200;
	pP_ptr->Free[5] = 0x138;

	FREE_UB(0) = Em10_walk_no[ (Rnd() >> (Rnd() & 3)) & 7 ];	/* retail: two draws */
	FREE_US(2) = 0;
	FREE_UB(1) = 0;
	FREE_UB(0xa) = 0;
	FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;
	FREE_UB(0x18) = 0;
	FREE_UB(0x19) = 0;
	FREE_US(0x1e) = 0;
	FREE_UB(0x21) = 0;
	FREE_UB(0x22) = 0;
	FREE_UB(0x23) = 0;
	FREE_UB(0x24) = 0;

	/* retail-only (retail 0x80100868..0x80100950): Id 0x11 zombies are tougher and always
	 * use the 0x8000 walk; with System_flg 0x40 one in three does too */
	if (pEm->Id == 0x11) {
		pEm->Life = 250;
		FREE_US(2) |= 0x8000;	}

	if (G.System_flg & 0x40) {
		if (!((Rnd() >> (Rnd() & 3)) % 3)) {
			FREE_US(2) |= 0x8000;
			FREE_UB(0xb) = (Rnd() & 0xf) + 32;	}	}

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 500;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 500;
	pEm->Atd[0].W = 500;
	pEm->Atd[0].D = 500;

	pEm->Eff_at_r = 500;

	pEm->Be_flg |= 0xc000000;

	if (pEm->Id == 0x13)
		pEm->Parts0_pos_y = 0xe66;
	if (pEm->Id == 0x1e)	/* retail-only, as in Em10_mv0f */
		pEm->Parts0_pos_y = 0xe66;

	*(long *)&pEm->Move_no = 1;
	if ((pEm->Type & 0x3f) == 2) {
		*(long *)&pEm->Move_no = 0x16;
		*(long *)&pEm->Routine_0 = 0x701;
		FREE_US(2) |= 4;	}

	if ((pEm->Type & 0x3f) == 4) {
		*(long *)&pEm->Move_no = 0x17;
		*(long *)&pEm->Routine_0 = 0x701;	}

	if ((pEm->Type & 0x3f) == 5) {
		*(long *)&pEm->Move_no = 0x16;
		*(long *)&pEm->Routine_0 = 0x701;
		FREE_US(2) |= 4;
		pEm->Type = 0x4002;
		pEm->Life = -1;
		pEm->pSin_parts_ptr[8].Be_flg |= 0x4000;	}

	if ((pEm->Type & 0x3f) == 7) {
		*(long *)&pEm->Move_no = 0x17;
		*(long *)&pEm->Routine_0 = 0x701;
		pEm->Type = 0x4004;
		pEm->Life = -1;
		pEm->pSin_parts_ptr[8].Be_flg |= 0x4000;	}

	if ((pEm->Type & 0x3f) == 6)
		*(long *)&pEm->Routine_0 = 0x101;

	if ((pEm->Type & 0x3f) == 8) {
		*(long *)&pEm->Move_no = 0x12;
		*(long *)&pEm->Routine_0 = 0x801;	}

	if (pEm->Type & 1) {
		*(long *)&pEm->Move_no = 0x17;
		pEm->Type |= 0x2000;
		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Atd[0].At_w = 200;
		pEm->Atd[0].At_d = 200;
		pEm->Atd[0].W = 200;
		pEm->Atd[0].D = 200;

		pEm->Atd[0].Ofs_y = -350;
		pEm->Atd[0].At_h = 350;

		pEm->Life /= 2;	}

	if ((pEm->Type & 0x3f) == 1)
		*(long *)&pEm->Routine_0 = 0x201;

	if ((pEm->Type & 0x3f) == 3)
		*(long *)&pEm->Routine_0 = 1;

	Joint_move(pEm, Kan, Seq, 0x100);

	if ((pEm->Type & 0x3f) == 0xa) {

		pEm->Damage_cnt |= 0x80;
		FREE_UB(0xa) = 0;
		pEm->Atd[0].At_w = 0;
		pEm->Atd[0].At_d = 0;
		pEm->Atd[0].W = 0;
		pEm->Atd[0].D = 0;
		pEm->Atd[0].Ofs_y = -350;
		pEm->Atd[0].At_h = 350;
		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;
		FREE_US(2) &= 0xfffb;
		FREE_US(2) |= 2;
		*(long *)&pEm->Routine_0 = 0xf01;
		pEm->Be_flg |= 8;
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, (unsigned long)pEm->pRoom_kan_t_ptr, (unsigned long)pEm->pRoom_seq_t_ptr, 0x100);	}

	Kage_work_set(&pEm->pKage_work, 0, 0x2580190, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);

	if (pEm->Id == 0x12) {

		if (Rnd() & 1) {
			pP_ptr = &pEm->pSin_parts_ptr[8];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);

		}

		if (!(Rnd() & 1)) {
			pP_ptr = &pEm->pSin_parts_ptr[0];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[16];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[1];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[2];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[3];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[4];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[5];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[6];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[7];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[9];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[10];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[11];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);

			pP_ptr = &pEm->pSin_parts_ptr[12];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[13];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 3, 0, pEm->Clut + 0x1e3);

			pP_ptr = &pEm->pSin_parts_ptr[14];
			Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);

		}
	}

	if (pEm->Id == 0x1f) {

		if (!(pEm->Model_type & 0xc0))
			Tex = (Rnd() + (Rnd() << (Rnd() & 3))) % 3 + 1;

		if (pEm->Model_type & 0x3f)
			Tex = pEm->Model_type & 0x3f;

		pP_ptr = &pEm->pSin_parts_ptr[0];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[1];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[2];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[3];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[4];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[5];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[6];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[7];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[9];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[10];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[12];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[13];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[16];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

	}

	if (pEm->Type & 0x80)
		Em10_Firedm(pEm);

	pP_ptr = &pEm->pSin_parts_ptr[15];
	pP_ptr->Be_flg = 0;

	pP_ptr = &pEm->pSin_parts_ptr[16];
	pP_ptr->Be_flg = 0;

	G.pPldamage_Em[ pEm->Id - 16 ] = PLDAMEGE_EM;
	G.pPldie_Em[ pEm->Id - 16 ] = PLDIE_EM;

}

static void Em10_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em10_Move_type[ pEm->Type & 0x3f ](pEm, Kan, Seq);	}

static void Em10_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em10_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em10_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em10_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em10_mv_br_type01[ pEm->Routine_1 ](pEm);

	Em10_mv_type01[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em10_mv00_br(ENEMY_WORK *pEm)
{
	register short Dir0;
	register short Dir1;
	register unsigned long LL;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name (as Em10_mv01_br) */
	LL = pEm->L_pl;
	pPl = (ENEMY_WORK *)&G.Player;	/* retail lever: G.Player reached through a pointer local (not G.Player.x) so CSE folds each
	 * pPl->field to its own absolute %hi/%lo address; no forced address pseudo lives across Goto00_ck */
	Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
	Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);

	if (LL < 5000 && !Dir0 && (pEm->Status_flg & 0x800))
		*(long *)&pEm->Routine_0 = 0x101;

	if (LL < 3000 && Dir1 && (G.Room_player_flg & 0x15) && (pEm->Status_flg & 0x800) && !(Rnd() & 1))

		*(long *)&pEm->Routine_0 = 0xc01;

	if (LL < 2000 && Dir0 && (pEm->Status_flg & 0x800) && (G.Room_player_flg & 0x17) && (Rnd() & 1))

		*(long *)&pEm->Routine_0 = 0xc01;

}

static void Em10_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = ((Rnd() & 0x1f) << 8) + 0xf0000;
		pEm->Routine_2 = 1;

		pEm->Timer0 = Rnd() + 150;

		pEm->Timer1 = Rnd() + 300;

	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);

		if ((unsigned short)pEm->Timer1-- == 0) {
			if (!FREE_UB(0x21) && (Rnd() & 1)) {
				Snd_se_enem(0xb, pEm);
				FREE_UB(0x21) = 150;
			}
			pEm->Timer1 = Rnd() + 300;	}

		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);
		}

		if (!pEm->Timer1 && !(Rnd() & 1))
			pEm->Routine_2 = 2;

		if (!pEm->Timer0 && pEm->L_pl < 7500 && (pEm->Status_flg & 0x800))

			*(long *)&pEm->Routine_0 = 0x101;

		if (pEm->Timer0)
			pEm->Timer0--;

		break;

	case 2:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 3;
		pEm->Timer0 = Rnd() + 150;

		FREE_UB(0x1d) = Rnd_area();
		FREE_US(2) |= 0x400;

	case 3:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		Add_speedXZ(pEm, 0);

		pEm->Timer0--;
		if (!pEm->Timer0) {
			pEm->Routine_2 = 0;
			FREE_US(2) &= 0xfbff;	}

		if (pEm->L_pl < 3000) {
			*(long *)&pEm->Routine_0 = 0x101;
			FREE_US(2) &= 0xfbff;	}

		if (pEm->L_pl < 7500 && (pEm->Status_flg & 0x800)) {

			*(long *)&pEm->Routine_0 = 0x101;
			FREE_US(2) &= 0xfbff;	}

	}

}

/* retail-only (retail 0x801016C8 / 0x8010E6C8, 19 words; no Beta twin, name unknown):
 * plays the enemy SE whose number sits in the top nibble of the current sequence word when its
 * 0x8000000 flag is set and the number is 0 or 1. Called from Em10_mv01's slow-walk step. */
static void func_801016C8(ENEMY_WORK *pEm)
{
	if ((*pEm->pNow_seq & 0x8000000) && (*pEm->pNow_seq >> 28) < 2)
		Snd_se_enem(*pEm->pNow_seq >> 28, pEm);
}

static void Em10_mv01_br(ENEMY_WORK *pEm)
{
	register short Dir0;
	register short Dir1;
	register unsigned long LL;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */

	LL = pEm->L_pl;

	pPl = (ENEMY_WORK *)&G.Player;	/* retail lever: G.Player reached through a pointer local (not G.Player.x) so CSE folds each
	 * pPl->field to its own absolute %hi/%lo address; no forced address pseudo lives across Goto00_ck */
	Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
	Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
	Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);

	/* retail: the two "turn away" tests are skipped while Free[0x26] runs (set by Em10_mv10);
	 * the Move_cnt / Rnd conditions of the Beta 0xe01 tests are gone (retail 0x80101798..) */
	if (!FREE_UB(0x26)) {
		if ((pEm->L_spl & 0xc0000000) && (pEm->L_spl & 0x3fffffff) < 2000 && !Goto01_ck(pEm, pEm->Dir_spl, 0x100)) {

			*(long *)&pEm->Routine_0 = 0xe01;
			return;	}

		if (!FREE_UB(0x26) && !Dir1 && LL < 2000 && pEm->nFloor != pPl->nFloor && !pPl->Damage_cnt)
			*(long *)&pEm->Routine_0 = 0xe01;
	}

	if ((pEm->Sce_free0 & 0xc000) && (pEm->Sca_info & 1))

		*(long *)&pEm->Routine_0 = 0xa01;

	if (LL < 3500 && Dir0 && (G.Room_player_flg & 0x15) && !(Rnd() & 3))

		*(long *)&pEm->Routine_0 = 0xc01;

	if (LL < 2500 && Dir0 && (G.Room_player_flg & 0x17) && !(Rnd() & 1))

		*(long *)&pEm->Routine_0 = 0xc01;

	if (pPl->Id != 0xf) {

		if (LL < 1200 && !(pPl->Damage_cnt & 0x80) && pEm->nFloor == pPl->nFloor) {

			if (!(FREE_US(2) & 0x20)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x100, 0x100)) {

					*(long *)&pEm->Routine_0 = 0x301;
					pPl->Damage_cnt |= 0x80;	}

			}

			if (!(FREE_US(2) & 0x40)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y - 0x100, 0x100)) {

					*(long *)&pEm->Routine_0 = 0x301;
					pPl->Damage_cnt |= 0x80;	}	}

		}

	}else {

		if (!FREE_UB(0x26) && !Dir1 && LL < 2000 && !pPl->Damage_cnt)

			*(long *)&pEm->Routine_0 = 0xe01;	}

	/* retail-only: grab a player lying at Life -0x8000 (retail 0x80101AA8..) */
	if (pPl->Life == -0x8000 && !Dir1 && LL < 1000) {
		*(long *)&pEm->Routine_0 = 0x60801;
		pEm->Type |= 0x4000;	}
}

static void Em10_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 1;

		pEm->Timer3 = (Rnd() & 0xf) << 1;
		pEm->Timer0 = (Em10_walk_rnd[ (signed char)pEm->Timer3 ] & 0x7fff) + Rnd();

		pEm->Timer2 = (Rnd() & 0x1f) + 30;

		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		pEm->Move_cnt = Rnd() & 0x1f;
		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);

		pEm->Timer1 = (Rnd() >> 3) + 100;

	case 1:

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Timer3++;
			if ((signed char)pEm->Timer3 >= 32)
				pEm->Timer3 = 0;
			pEm->Timer0 = (Em10_walk_rnd[ (signed char)pEm->Timer3 ] & 0x7fff) + (Rnd() & 0x1f);

			pEm->Cdir_y -= 0x10 - ((Rnd() & 1) << 5);	}

		if (pEm->L_pl > 5000) {
			Dir = 8 - ((Em10_walk_rnd[ (signed char)pEm->Timer3 ] >> 15) << 4);
		}else
			Dir = 8;

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir);

		if ((signed char)pEm->Timer2-- == 0) {
			pEm->Timer2 = (Rnd() & 0xf) + 30;
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

			if (!FREE_UB(0x21)) {
				if (!(Rnd() & 0x1f)) {
					Snd_se_enem(10, pEm);	FREE_UB(0x21) = 150;	}

				else if (!(Rnd() & 0x1f)) {
					Snd_se_enem(11, pEm);
					FREE_UB(0x21) = 150;
				}
			}
		}

		if (pEm->L_pl < 3000)
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		if ((pEm->Type & 0x80) || (FREE_US(2) & 0x8000)) {	/* retail: also on Free flag 0x8000 */
			if (pEm->Move_cnt % 3 == 2) {
				func_801016C8(pEm);	/* retail-only */
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
				Joint_move_skip(pEm, (unsigned long)pEm->pSeq_t_ptr);	}	}	/* retail: replaces Beta's Joint_move here */

		Add_speedXZ(pEm, 0);

		pEm->Timer1--;
		if (!pEm->Timer1) {

			pEm->Timer1 = (Rnd() >> 3) + 100;
			if (!(Rnd() & 3) && pEm->L_pl < 5000)
				*(long *)&pEm->Routine_0 = 0x201;	}

		if ((pEm->Type & 0x80) || (FREE_US(2) & 0x1000)) {
			if (!(FREE_US(0x1e) & 7) && (FREE_US(2) & 0x800)) {
				pEm->Cdir_y -= -0x40 + (((Rnd() >> (Rnd() & 3)) & 1) << 7);
				pEm->Life--;

				if (pEm->Life < 0) {
					*(long *)&pEm->Routine_0 = 0xa03;
					if (FREE_US(2) & 0x1000)
						*(long *)&pEm->Routine_0 = 0xb03;
					pEm->Damage_no = 4;
					pEm->Damage_cnt |= 0x80;
					FREE_US(2) |= 0x2000;
					return;	}

			}

			if (FREE_US(2) & 0x1000)
				pEm->Cdir_y -= -0x40 + (Rnd() >> 1);	}

		/* retail: the Beta `if( !( pEm->Type & 0x40 ) )` guard around these two tests is gone */
		if (pEm->L_pl < 2000 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200) && !(G.Player.Damage_cnt & 0x80))

			*(long *)&pEm->Routine_0 = 0x201;

		if ((G.Room_player_flg & 4) && pEm->L_pl < 2500 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x52c))

			*(long *)&pEm->Routine_0 = 0x201;

	}

}

static void Em10_mv02_br(ENEMY_WORK *pEm)
{
	register short Dir0;
	register short Dir1;
	register unsigned long LL;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */
	LL = pEm->L_pl;

	pPl = (ENEMY_WORK *)&G.Player;	/* retail lever: G.Player reached through a pointer local (not G.Player.x) so CSE folds each
	 * pPl->field to its own absolute %hi/%lo address; no forced address pseudo lives across Goto00_ck */
	Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
	Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
	Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);

	/* retail: as in Em10_mv01_br (Free[0x26] guard, Beta Move_cnt/Rnd conditions removed) */
	if (!FREE_UB(0x26)) {
		if ((pEm->L_spl & 0xc0000000) && (pEm->L_spl & 0x3fffffff) < 2000 && !Goto01_ck(pEm, pEm->Dir_spl, 0x100)) {

			*(long *)&pEm->Routine_0 = 0xe01;
			return;	}

		if (!FREE_UB(0x26) && !Dir1 && LL < 2000 && pEm->nFloor != pPl->nFloor && !pPl->Damage_cnt)
			*(long *)&pEm->Routine_0 = 0xe01;
	}

	if ((pEm->Sce_free0 & 0xc000) && (pEm->Sca_info & 1))

		*(long *)&pEm->Routine_0 = 0xa01;

	if (LL < 3000 && Dir0 && (G.Room_player_flg & 0x17) && !(Rnd() & 1))

		*(long *)&pEm->Routine_0 = 0xc01;

	if (pPl->Id != 0xf) {

		if (LL < 1200 && !(pPl->Damage_cnt & 0x80) && pEm->nFloor == pPl->nFloor) {

			if (!(FREE_US(2) & 0x20)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x100, 0x100)) {

					*(long *)&pEm->Routine_0 = 0x301;
					pPl->Damage_cnt |= 0x80;	}

			}

			if (!(FREE_US(2) & 0x40)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y - 0x100, 0x100)) {

					*(long *)&pEm->Routine_0 = 0x301;
					pPl->Damage_cnt |= 0x80;	}	}

		}

	}else {

		if (!FREE_UB(0x26) && !Dir1 && LL < 2000 && !pPl->Damage_cnt)

			*(long *)&pEm->Routine_0 = 0xe01;	}

	if (pPl->Life == -0x8000 && !Dir1 && LL < 1000) {
		*(long *)&pEm->Routine_0 = 0x60801;
		pEm->Type |= 0x4000;	}

}

static void Em10_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0004;
		pEm->Routine_2 = 1;

		pEm->Timer0 = (Rnd() & 0x3f) + 180;

		if (!FREE_UB(0x21)) {
			if (!(Rnd() & 1)) {
				Snd_se_enem(10, pEm);	}
			else {
				Snd_se_enem(11, pEm);
			}
			FREE_UB(0x21) = 150;	}

		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		pEm->Move_cnt = Rnd() & 0x1f;
		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		pEm->Timer1 = (Rnd() & 0x1f) + 45;	/* retail-only: sway timer/counter */
		pEm->Timer2 = 0;

	case 1:
		if (!*(signed char *)&pEm->Timer2)	/* retail: sways the other way while Timer2 runs (retail 0x80102344..) */
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
		else {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x10);
			pEm->Timer2--;	}

		if (pEm->Sca_info & 1) {
			if ((unsigned short)pEm->Timer1-- == 0) {
				pEm->Timer1 = (Rnd() & 0x1f) + 45;
				pEm->Timer2 = (Rnd() & 7) + 5;	}	}

		Spd_get(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		if ((pEm->Type & 0x80) || (FREE_US(2) & 0x8000)) {	/* retail: also on Free flag 0x8000 */
			if (pEm->Move_cnt % 3 == 2) {
				func_801016C8(pEm);	/* retail-only */
				Joint_move_skip(pEm, (unsigned long)pEm->pSeq_t_ptr);	}	}	/* retail: replaces Beta's Joint_move here */

		Add_speedXZ(pEm, 0);

		if ((pEm->Type & 0x80) || (FREE_US(2) & 0x1000)) {
			if (!(FREE_US(0x1e) & 7) && (FREE_US(2) & 0x800)) {
				pEm->Life--;

				if (pEm->Life < 0) {
					*(long *)&pEm->Routine_0 = 0xa03;
					if (FREE_US(2) & 0x1000)
						*(long *)&pEm->Routine_0 = 0xb03;
					pEm->Damage_no = 4;
					pEm->Damage_cnt |= 0x80;
					FREE_US(2) |= 0x2000;
					return;	}

			}

			if (FREE_US(2) & 0x1000)
				pEm->Cdir_y -= -0x40 + (Rnd() >> 1);	}

		if ((unsigned short)pEm->Timer0-- == 0 || (G.Player.Damage_cnt & 0x80))
			*(long *)&pEm->Routine_0 = 0x101;

		if ((pEm->L_pl > 5000 || Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400)) && pEm->Timer0 < 120)

			*(long *)&pEm->Routine_0 = 0x101;

	}

}

static void Em10_mv03_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned char Em10_mv03_type[4][2] = { { 11, 11 }, { 14, 14 }, { 11, 11 }, { 14, 14 } };	/* retail: 4 rows (Id 0x11/0x17 use rows 2/3) */
	SVECTOR Sv;
	register PARTS_W *pP_ptr;
	MATRIX M;

	unsigned char Em10_mv03_atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } };	/* retail: rows 2/3 bite harder */
	register ENEMY_WORK *pT;
	register unsigned long Flg;
	register unsigned long Life_ck;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */
	register unsigned long Up;	/* retail-only local, [INFERRED] name: get-up decision in case 8 */

	pPl = (ENEMY_WORK *)&G.Player;
	Flg = pEm->Type & 1;
	if (pEm->Id == 0x17 || pEm->Id == 0x11)	/* retail-only */
		Flg += 2;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = Em10_mv03_type[ Flg ][0] + 0xf0000;
		pEm->Routine_2 = 1;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);

		pPl->pEnemy_ptr = (unsigned long *)pEm;
		Set_base_pos(pPl, Kan, Seq, 0);
		*(long *)&pPl->Routine_0 = (Cdir_ck(pEm, pPl) << 8) | 5;

		if (Flg & 1)
			pPl->Routine_1 += 2;

		pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

		pPl->Damage_cnt |= 0x80;

		pEm->Damage_cnt = 0xf;

		pEm->Be_flg |= 0x1000;
		pPl->Be_flg |= 0x1000;

		if (!FREE_UB(0x21)) {	/* retail: == 0 and the timer is set (Beta tested != 0) */
			if (Rnd() & 1) {
				Snd_se_enem(10, pEm);	}
			else {
				Snd_se_enem(11, pEm);
			}
			FREE_UB(0x21) = 150;
		}

		Pad_vib_fade_set(0x14, 0, 0xfa, 0);	/* retail-only (Dual Shock) */

	case 1:

		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 2:
		*(long *)&pEm->Move_no = Em10_mv03_type[ Flg ][0] + 1;
		pEm->Routine_2 = 3;
		pEm->Timer0 = 148;	/* retail: always 148 (Beta 74/37) */

	case 3:

		Null_pos_set(pEm, Kan, Seq, 0);

		Joint_move(pEm, Kan, Seq, 0x100);

		pEm->Timer0 = pEm->Timer0 - 2 - Gacha_ch() * 5;	/* retail: - 2 - 5 per press (Beta - 1 - 1) */
		if (pEm->Timer0 < 0) {
			pEm->Routine_2 = 4;

			pPl->Routine_2 = 4;	}

		if (pEm->Move_cnt == Em10_mv03_atck[ Flg ][0]) {

			Pad_vib_set1(6, 0xfa, 0);	/* retail-only (Dual Shock) */
			Pad_vib_set0(0xa, 0);
			Pad_vib_set1(4, 0xfa, 0xf);
			Pad_vib_set0(4, 0xd);

			Snd_se_enem(3, pEm);

			Life_ck = Pl_life_down(Em10_mv03_atck[ Flg ][1], Flg & 1);

			if (Life_ck & 1) {
				pEm->Routine_2 = 4;

				pPl->Routine_2 = 4;

				pPl->Timer2 = 1;	}

			if (Life_ck & 2) {
				*(long *)&pEm->Routine_0 = 0x601;
				*(long *)&G.Player.Routine_0 = (Cdir_ck(pEm, pPl) << 8) | 6;

				G.System_flg |= 0x100;	}

		}

		break;

	case 4:
		*(long *)&pEm->Move_no = Em10_mv03_type[ Flg ][0] + 0x70002;
		pEm->Routine_2 = 5;

		pEm->Timer0 = 0;
		pEm->Timer2 = 0;
		Pad_vib_set1(4, 0xfa, 0xa);	/* retail-only (Dual Shock) */

	case 5:
		/* retail: the arm-tear (Em10_ude_down) starts only at random and unless an arm is
		 * already gone; Free[0x25] bit 1 picks the arm (retail 0x801027A4..0x80102844) */
		if (!(FREE_US(2) & 0x60) && pEm->Move_cnt == 3 && !(Flg & 1) && !((Rnd() >> (Rnd() & 3)) & 0xf)) {
			FREE_UB(0x24) = 1;
			FREE_UB(0x25) = Cdir_ck(pEm, pPl);
			FREE_UB(0x25) |= Rnd() & 2;
			FREE_US(2) |= 0x40;
			if (FREE_UB(0x25) & 2) {
				FREE_US(2) &= 0xffbf;
				FREE_US(2) |= 0x20;	}	}

		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		if (Flg & 1) {

			if ((!(pPl->Id & 1) && pPl->Id < 11) || pPl->Id == 12 || pPl->Id == 13 || pPl->Id == 11) {	/* retail: more player ids */
				if (pEm->Move_cnt == 0x16) {

					pP_ptr = &pEm->pSin_parts_ptr[8];
					pP_ptr->Be_flg |= 0x4a;
					pP_ptr->Free[0] = pEm->Cdir_y;
					pP_ptr->Free[4] = 0;
					pP_ptr->Free[2] = 0;
					pP_ptr->Free[1] = 0;
					pP_ptr->Free[3] = 0;
					pP_ptr->Free[6] = 0;
					pP_ptr->Poly_rgb = 0x101040;

					pP_ptr = pEm->pSin_parts_ptr;
					Sv.vx = 0;
					Sv.vy = -600;
					Sv.vz = 0;

					Esp_call(0x8001000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

					Snd_se_enem(2, pEm);
					Pad_vib_set1(4, 0xfa, 0);	/* retail-only (Dual Shock) */
				}

			}else {
				if (pEm->Move_cnt == 0x1b) {
					FREE_US(2) |= 0x80;

					Snd_se_enem(8, pEm);
					Pad_vib_set1(4, 0xfa, 0);	/* retail-only (Dual Shock) */

				}	}

			if ((unsigned)(pEm->Move_cnt - 23) < 19) {
				if (pEm->Move_cnt & 1) {

					pP_ptr = &pEm->pSin_parts_ptr[8];
					Sv.vx = 0;
					Sv.vy = 100;

					Sv.vz = 0;	Esp_call(0x1000, pEm->Cdir_y + (Rnd() << 2) - 0x200, &pP_ptr->Workm, &Sv);

				}	}

		}

		if (!(Flg & 1)) {
			if (pEm->Move_cnt == 7)

				pEm->Routine_2 = 7;	}	break;

	case 6:
		*(long *)&pEm->Routine_0 = 7;
		pEm->Life = -1;
		FREE_US(2) &= 0xfffb;
		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;
		pEm->Damage_cnt |= 0x80;

		break;

	case 7:
		Spd_get(pEm, Kan, Seq, 0);
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 = 8;

	case 8:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		pEm->Spd_x -= 30;

		Add_speedXZ(pEm, 0);

		/* retail: on Sce_timer 3 (bio2-ds PLAYER.C:2441 names it G.SaveData.Sce_timer) the zombie
		 * gets up again with a 1/4 chance instead of the 15/16 one (retail 0x80102C9C..0x80102D20) */
		Up = 0;
		if (pEm->Move_cnt == 0x16) {
			if (G.SaveData.Sce_timer != 3)
				Up = (((Rnd() >> (Rnd() & 3)) & 0xf) ^ 2) != 0;	/* != 2 spelled as ^ 2: retail computes the xor in place */
			if (G.SaveData.Sce_timer == 3 && !(Rnd() & 3))
				Up = 1;	}
		if (Up) {
			*(long *)&pEm->Routine_0 = 0x501;
			FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;

			FREE_US(2) &= 0xfff7;

			pEm->Timer2 = 1;
			FREE_US(2) &= 0xffff;
			FREE_US(2) |= 4;
			*(long *)&pEm->Move_no = (signed char)pEm->Timer2 * 0xa00 + 0xa0a02;
			pEm->Routine_2 = 1;
			FREE_US(2) &= 0xffff;

			FREE_US(2) |= 0x202;
			pEm->Be_flg &= 0xf3ffffff;
			pEm->Be_flg |= 0x4000000;
			pEm->Damage_cnt |= 0x80;
			FREE_UB(0xa) = 0;
			pEm->Timer3 = 0;
			pEm->Type |= 0x2000;
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(13, pEm);
			}
			pEm->Timer0 = 1;
			break;	}

		if ((unsigned)(pEm->Move_cnt - 7) < 18 && pEm->At_em_no != 0xff) {

			pT = G.pEnemy[ pEm->At_em_no ];

			if ((pT->Id & 0x10) && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt && !(FREE_US_P(pT,2) & 8) && !(pT->Type & 0x2000)) {

				*(long *)&pT->Routine_0 = 0x901;

				pT->Timer3 = Cdir_ck(pEm, pT);

				Snd_se_enem(4, pEm);	}

		}

		break;

	case 9:
		*(long *)&pEm->Routine_0 = 1;

	}

}

static void Em10_mv04_br(ENEMY_WORK *pEm)
{

	register unsigned long LL;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name (as Em10_mv01_br) */
	LL = pEm->L_pl;
	pPl = (ENEMY_WORK *)&G.Player;	/* retail lever: G.Player reached through a pointer local (not G.Player.x) so CSE folds each
	 * pPl->field to its own absolute %hi/%lo address; no forced address pseudo lives across Goto00_ck */
	Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
	Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
	if (LL < 1300 && !(pPl->Damage_cnt & 0x80) && pEm->nFloor == pPl->nFloor) {
		if (!(FREE_US(2) & 0x20)) {
			if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x100, 0x100)) {
				*(long *)&pEm->Routine_0 = 0x101;
				pPl->Damage_cnt |= 0x80;	}
		}
		if (!(FREE_US(2) & 0x40)) {
			if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y - 0x100, 0x100)) {
				*(long *)&pEm->Routine_0 = 0x101;
				pPl->Damage_cnt |= 0x80;	}

		}
	}

}

static void Em10_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf0005;
		pEm->Routine_2 = 1;
		pEm->Timer0 = (Rnd() & 7) + 7;

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Move_cnt = Rnd() & 0xf;
		Spd_get(pEm, Kan, Seq, 0);

	case 1:
		if (pEm->Spd_x > 20) {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
			if ((unsigned short)pEm->Timer0-- == 0) {
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
				pEm->Timer0 = (Rnd() & 7) + 7;	}

		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

	}

}

static void Em10_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em10_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned char Em10_mv06_mot[3][2] = { { 1, 2 }, { 23, 22 }, { 8, 9 } };

	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	unsigned short Em10_mv06_timer[16] = { 90, 30, 170, 160, 10, 40, 100, 50, 110, 20, 35, 70, 30, 180, 30, 60 };	/* retail .rdata template (em10 slot1 .rdata+0x48, image halfwords) */

	switch (pEm->Routine_2) {
	case 0:
		*(signed char *)&pEm->Timer2 = Rnd() & 1;

		FREE_US(2) &= 0xfffb;
		if (*(signed char *)&pEm->Timer2)
			FREE_US(2) |= 4;

		*(long *)&pEm->Move_no = (((signed char)pEm->Timer2 * 5 + 10) << 8) + 0xf0000 + Em10_mv06_mot[0][ (signed char)pEm->Timer2 ];
		pEm->Routine_2 = 1;

		FREE_US(2) |= 0x200;

		FREE_US(2) |= 2;

		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Damage_cnt |= 0x80;
		FREE_UB(0xa) = 0;

		pEm->Timer3 = 0;

		pEm->Type |= 0x2000;

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(13, pEm);
			}
			FREE_UB(0x21) = 150;
		}
		pEm->Timer0 = 0;

		pEm->Spd_x = 11;

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		if (pEm->Atd[0].At_w > 20) {
			pEm->Atd[0].At_w -= 10;
			pEm->Atd[0].At_d -= 10;
			if (pEm->Atd[0].W > 200) {
				pEm->Atd[0].W -= 10;
				pEm->Atd[0].D -= 10;	}
		}

		if (!pEm->Hokan_flg || pEm->Timer0) {

			Foot_set_pl(pEm, 0, 1);	}

		break;

	case 2:
		pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = Em10_mv06_mot[1][ (signed char)pEm->Timer2 ] + 0xf0000;

		pEm->Timer1 = Em10_mv06_timer[ (Rnd() >> (Rnd() & 3)) & 0xf ] + (Rnd() & 0xf);	/* retail 0xf (Beta 0x1f) */

		pEm->Damage_cnt &= 0x7f;

		pEm->Atd[0].At_w = 0;
		pEm->Atd[0].At_d = 0;
		pEm->Atd[0].W = 200;
		pEm->Atd[0].D = 200;

		pEm->Atd[0].Ofs_y = -350;
		pEm->Atd[0].At_h = 350;

		pEm->Spd_x = Rnd() & 3;

		if (pEm->Type & 0x80) {
			pEm->Spd_x = 0;
			pEm->Timer1 = Em10_mv06_timer[ (Rnd() >> (Rnd() & 3)) & 0xf ] >> 1;	}

	case 3:
		if (!pEm->Spd_x)
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		goto wait_countdown; { /* [INFERRED] case3 skips initialization; case4/5 dispatch still enters this block. */

	case 4:
		pEm->Timer0 = (Rnd() & 0xf) + 3;
		pEm->Routine_2 = 5;

	case 5:
		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 3;
		} wait_countdown: switch (0) default: ; /* [INFERRED] shared countdown entry, separate from its following test; no added debug label. */
		if ((unsigned short)pEm->Timer1-- == 0) {
			pEm->Routine_2 = 6;

			break;	}
		break;
	case 6:
		*(long *)&pEm->Move_no = Em10_mv06_mot[2][ (signed char)pEm->Timer2 ] + 0xf0000;
		pEm->Routine_2 = 7;

		FREE_US(2) |= 0x10;

		FREE_UB(0x1c) = 1;
		if (FREE_US(2) & 4)
			FREE_UB(0x1c) = 0;

		if ((signed char)pEm->Timer3 == 1) {
			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vx = 0;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x1f40, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}

		if (!FREE_UB(0x21) && !(Rnd() & 3)) {
			Snd_se_enem(11, pEm);
			FREE_UB(0x21) = 150;
		}

		FREE_US(2) &= 0xfdff;

		pEm->Spd_x = 11;

	case 7:
		if (pEm->Atd[0].At_w < 500) {
			pEm->Atd[0].At_w += 10;
			pEm->Atd[0].At_d += 10;
			if (pEm->Atd[0].W < 500) {
				pEm->Atd[0].W += 10;
				pEm->Atd[0].D += 10;	}
		}
		if (pEm->Atd[0].Ofs_y > -1500) {	/* retail: own guard, step 10 (Beta -30/+30 inside the At_w test) */
			pEm->Atd[0].Ofs_y -= 10;
			pEm->Atd[0].At_h += 10;	}

		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			FREE_US(2) &= 0xffef;
			pEm->Routine_2 = 8;
			FREE_US(2) &= 0xfffd;	}

		if (pEm->Move_cnt == 0x37)
			pEm->Be_flg |= 0xc000000;

		break;

	case 8:
		pEm->Damage_cnt &= 0x7f;
		*(long *)&pEm->Routine_0 = 0x101;

		pEm->Be_flg |= 0xc000000;

		pEm->Type &= 0xdfff;

		FREE_US(2) &= 0xffef;
		FREE_US(2) &= 0xfffd;

		pEm->Atd[0].At_w = 500;
		pEm->Atd[0].At_d = 500;
		pEm->Atd[0].W = 500;
		pEm->Atd[0].D = 500;

		pEm->Atd[0].Ofs_y = -1500;
		pEm->Atd[0].At_h = 1500;

	}

}

static void Em10_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x17;
		if (FREE_US(2) & 4)
			*(long *)&pEm->Move_no = 0x16;

		Joint_move(pEm, Kan, Seq, 0x100);

		pEm->Routine_2 = 1;

		pEm->Damage_cnt |= 0x80;

		pEm->Neck_flg |= 1;

	case 1:
		if (!(pEm->Type & 0x4000))

			pEm->Routine_2 = 2;

		break;

	case 2:
		*(long *)&pEm->Move_no = ((Rnd() & 0xf) << 8) + 0xf0008;
		if (FREE_US(2) & 4)
			*(long *)&pEm->Move_no = ((Rnd() & 0xf) << 8) + 0xf0009;

		pEm->Routine_2 = 3;

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);
			}else if (Rnd() & 1) {
				Snd_se_enem(11, pEm);	}
			else {
				Snd_se_enem(10, pEm);
			}
			FREE_UB(0x21) = 150;
		}

	case 3:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 4:
		pEm->Damage_cnt &= 0x7f;
		*(long *)&pEm->Routine_0 = 0x101;

		pEm->Be_flg |= 0xc000000;

		pEm->Neck_flg &= 0xfe;

	}

}

static void Em10_mv08_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf0018;
		pEm->Routine_2 = 1;

		Set_base_pos((ENEMY_WORK *)&G.Player, Kan, Seq, 0);

		if (Rnd() & 1) {
			Snd_se_enem(10, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		pEm->Damage_cnt |= 0x80;

	case 1:
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x801;
		pEm->Type |= 0x4000;

	}

}

static void Em10_mv09_br(ENEMY_WORK *pEm)
{
	register short Dir0;
	register short Dir1;
	register unsigned long LL;
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name (as Em10_mv01_br) */
	LL = pEm->L_pl;
	pPl = (ENEMY_WORK *)&G.Player;	/* retail lever: G.Player reached through a pointer local (not G.Player.x) so CSE folds each
	 * pPl->field to its own absolute %hi/%lo address; no forced address pseudo lives across Goto00_ck */
	Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
	Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
	if (!Dir1 && LL < 1300 && !(pPl->Damage_cnt & 0x80) && (pEm->Status_flg & 0x800) && pEm->nFloor == pPl->nFloor) {
		*(long *)&pEm->Routine_0 = 0x101;
		pPl->Damage_cnt |= 0x80;	}

}

static void Em10_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x17;

	}

}

static void Em10_mv0a_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv0a_mot[8] = { 18, 19, 20, 18, 19, 20, 18, 19 };

	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = Em10_mv0a_mot[ Rnd() & 7 ] + 0x70000;

		pEm->Damage_cnt |= 0x80;

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);
		}

		if (*pEm->pNow_seq & 0x10000) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = 200;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x13d0, Rnd() << 4, &pP_ptr->Workm, &Sv);
		}

		if (!(pEm->Type & 0x4000))
			pEm->Routine_2 = 3;

		break;

	case 2:
		*(long *)&pEm->Move_no = Em10_mv0a_mot[ Rnd() & 7 ] + 0x300;
		pEm->Routine_2 = 1;
		break;

	case 3:
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0xf0015;
		pEm->Damage_cnt &= 0x7f;

		FREE_US(2) |= 0x10;
		FREE_UB(0x1c) = 2;

		if (!FREE_UB(0x21)) {
			if (!(Rnd() & 1)) {

				if (Rnd() & 1) {
					Snd_se_enem(11, pEm);
				}else if (Rnd() & 1) {
					Snd_se_enem(10, pEm);
				}	}
			FREE_UB(0x21) = 150;
		}

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			FREE_US(2) &= 0xffef;
			pEm->Routine_2 = 5;	}

		break;

	case 5:
		pEm->Damage_cnt &= 0x7f;
		*(long *)&pEm->Routine_0 = 0x101;
		FREE_US(2) &= 0xffef;

		break;

	case 6:
		pEm->Routine_2 = 7;
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0xf0015;

	case 7:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x10100);
		break;

	case 8:
		pEm->Routine_2 = 0;

	}

}

static void Em10_mv0b_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv0b_mot[2][2] = { { 3, 3 }, { 4, 13 } };

	unsigned char Em10_mv06_mot[3][2] = { { 1, 2 }, { 23, 22 }, { 8, 9 } };

	switch (pEm->Routine_2) {
	case 0:
		*(signed char *)&pEm->Timer2 = Rnd() & 1;
		*(long *)&pEm->Move_no = ((Rnd() & 3) << 8) + Em10_mv0b_mot[ (signed char)pEm->Timer3 ][ (signed char)pEm->Timer2 ];
		if (*(signed char *)&pEm->Timer3 && *(signed char *)&pEm->Timer2)
			pEm->Move_cnt = (Rnd() & 3) + 0x14;

		pEm->Routine_2 = 1;

		pEm->Spd_x = 400;
		pEm->Spd_z = 0;

		pEm->Timer0 = (signed char)pEm->Timer3 * 0x800;

		FREE_US(2) &= 0xffef;
		FREE_US(2) |= 8;

		pEm->Timer1 = 0;

		if (!FREE_UB(0x21) && (Rnd() & 1)) {
			Snd_se_enem(12, pEm);
			FREE_UB(0x21) = 150;
		}

	case 1:
		Add_speedXZ(pEm, pEm->Timer0);

		pEm->Spd_x -= 30;
		if (pEm->Spd_x < 0) {
			pEm->Spd_x = 0;
			pEm->Timer1++;	}

		if (pEm->Timer1 == 1 && ((Rnd() >> (Rnd() & 3)) & 7)) {
			*(long *)&pEm->Routine_0 = 0x501;
			FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;

			FREE_US(2) &= 0xfff7;

			*(signed char *)&pEm->Timer2 = pEm->Timer3;

			FREE_US(2) &= 0xfffb;
			if (*(signed char *)&pEm->Timer2)
				FREE_US(2) |= 4;
			*(long *)&pEm->Move_no = (((signed char)pEm->Timer2 * 5 + 10) << 8) + 0xf0000 + Em10_mv06_mot[0][ (signed char)pEm->Timer2 ];
			pEm->Routine_2 = 1;

			FREE_US(2) |= 0x200;

			FREE_US(2) |= 2;

			pEm->Be_flg &= 0xf3ffffff;
			pEm->Be_flg |= 0x4000000;

			pEm->Damage_cnt |= 0x80;
			FREE_UB(0xa) = 0;

			pEm->Timer3 = 0;

			pEm->Type |= 0x2000;

			if (!FREE_UB(0x21)) {
				if (Rnd() & 1) {
					Snd_se_enem(12, pEm);	}
				else {
					Snd_se_enem(13, pEm);
				}
				FREE_UB(0x21) = 150;
			}
			pEm->Timer0 = 0;
			break;
		}

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x101;

		FREE_US(2) &= 0xfff7;

	}

}

static void Em10_mv0c_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv0c_mot[3] = { 0, 2, 4 };
	register short Dir;

	pEm->Sce_free1 |= 0x4000;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = ((Rnd() & 3) << 8) + 0xf0000 + FREE_UB(0);
		pEm->Routine_2 = 1;

		pEm->Timer2 = 0;
		if (pEm->Sce_free0 & 0x4000)
			pEm->Timer2 = (Rnd() & 1) + 1;

	case 1:
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		Dir = Goto01_ck(pEm, pEm->Sce_free0 & 0xfff, 0x20);
		pEm->Cdir_y += Dir;
		if (!Dir)
			pEm->Routine_2 = 2;

		break;

	case 2:
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0xf0000 + (Em10_mv0c_mot[ (signed char)pEm->Timer2 ] + (Rnd() & 1));
		pEm->Routine_2 = 3;
		pEm->Timer0 = (Rnd() & 0x7f) + 60;

	case 3:
		if (Joint_move(pEm, (unsigned long)pEm->pRoom_kan_t_ptr, (unsigned long)pEm->pRoom_seq_t_ptr, 0x100))

			*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0xf0000 + (Em10_mv0c_mot[ (signed char)pEm->Timer2 ] + (Rnd() & 1));

		pEm->Sce_free1 &= 0x7fff;
		if (*pEm->pNow_seq & 0x10000)
			pEm->Sce_free1 |= 0x8000;

		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 4;

		break;

	case 4:
		*(long *)&pEm->Routine_0 = 0x101;

	}

}

static void Em10_mv0d_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf000a;
		pEm->Routine_2 = 1;
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);

		pEm->Timer0 = 0;

		FREE_US(2) &= 0xfffb;

		FREE_US(2) |= 2;

		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Damage_cnt |= 0x80;
		FREE_UB(0xa) = 0;

		pEm->Timer3 = 0;

		pEm->Type |= 0x2000;

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(10, pEm);	}
			else {
				Snd_se_enem(11, pEm);
			}
			FREE_UB(0x21) = 150;
		}

	case 1:
		if (++pEm->Timer0 & 1) {
			Spd_get(pEm, Kan, Seq, 0);
		}else
			pEm->Spd_x >>= 2;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt >= 18) {
			Dir = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);

			if (!Dir && pEm->L_pl < 1800 && !(G.Player.Damage_cnt & 0x80) && pEm->nFloor == G.Player.nFloor) {

				pEm->Type &= 0xffc0;
				pEm->Type |= 1;
				*(long *)&pEm->Routine_0 = 0x101;
				G.Player.Damage_cnt |= 0x80;	}
			break;

		}
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x20501;
		FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;
		pEm->Timer2 = 0;

	}

}

static void Em10_mv0e_br(ENEMY_WORK *pEm)
{
	/* retail: the empty Beta branch routine now starts the bite (retail 0x8010460C, 79 words;
	 * same shape as Em10_mv04_br) */
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */
	register long Flg;	/* retail-only local, [INFERRED] name: the && chain is materialized */
	register unsigned long LL;

	pPl = (ENEMY_WORK *)&G.Player;
	LL = pEm->L_pl;

	Flg = pEm->Routine_2 == 3 && LL < 1300 && !(G.Player.Damage_cnt & 0x80) && pEm->Move_cnt < 11 && pEm->nFloor == G.Player.nFloor && G.Player.Id != 15;
	if (Flg) {
		if (!(FREE_US(2) & 0x20)) {
			if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x100, 0x100)) {
				*(long *)&pEm->Routine_0 = 0x301;
				pPl->Damage_cnt |= 0x80;	}
		}

		if (!(FREE_US(2) & 0x40)) {
			if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y - 0x100, 0x100)) {
				*(long *)&pEm->Routine_0 = 0x301;
				pPl->Damage_cnt |= 0x80;	}
		}
	}
}

static void Em10_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	/* retail: the Beta bite test at the end of case 3 moved into Em10_mv0e_br; cases 1 and 3
	 * read the player through a pointer to G.Player (retail 0x80104748, 118 words) */
	register ENEMY_WORK *pPl;	/* retail-only local, [INFERRED] name */

	pPl = (ENEMY_WORK *)&G.Player;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70019;
		pEm->Routine_2 = 1;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		pEm->Timer0 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0xbe);

		if (!FREE_UB(0x21)) {
			Snd_se_enem(10, pEm);
			FREE_UB(0x21) = 150;
		}

		pEm->Spd_x = 11;

	case 1:
		if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x140)) {
			pEm->Timer0 = 0;
			pEm->Routine_2 = 2;	}

		pEm->Cdir_y += pEm->Timer0;

		if (Joint_move(pEm, Kan, Seq, 0x200))

			pEm->Routine_2 = 2;

		break;

	case 2:
		*(long *)&pEm->Move_no = 0x7001b;
		pEm->Routine_2 = 3;

	case 3:
		Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200))
			*(long *)&pEm->Routine_0 = 0x101;

		Add_speedXZ(pEm, 0);
		if (pEm->Move_cnt == 0x19)
			*(long *)&pEm->Routine_0 = 0x101;

	}

}

static void Em10_mv0f_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long Tex;
	register unsigned long i;
	unsigned char Em10_walk_no[8] = { 0, 2, 0, 2, 0, 2, 0, 2 };

	*(long *)&pEm->Routine_0 = 1;
	Parts_work_kan_init(pEm, pEm->pKan_t_ptr);	/* retail-only call (retail 0x80104984) */

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	pEm->Life = Em10_life[ Rnd() & 0xf ];
	pEm->D_life_u = 13;
	pEm->D_life_c = 13;
	pEm->D_life_d = 13;

	FREE_UB(0) = Em10_walk_no[ Rnd() & 7 ];
	FREE_US(2) = 0;
	FREE_UB(1) = 0;
	FREE_UB(0xa) = 0;
	FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;
	FREE_UB(0x18) = 0;
	FREE_UB(0x19) = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 500;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 500;
	pEm->Atd[0].W = 500;
	pEm->Atd[0].D = 500;

	pEm->Be_flg = 0xc000001;

	if (pEm->Id == 0x13)
		pEm->Parts0_pos_y = 0xe66;
	if (pEm->Id == 0x1e)	/* retail-only (retail 0x80104A54) */
		pEm->Parts0_pos_y = 0xe66;

	*(long *)&pEm->Move_no = 1;
	Joint_move(pEm, Kan, Seq, 0x100);

	if ((pEm->Type & 0x3f) == 6)
		*(long *)&pEm->Routine_0 = 0x101;

	if (pEm->Id == 0x1f) {

		if (!(pEm->Model_type & 0xc0))
			Tex = Rnd() % 3 + 1;

		if (pEm->Model_type & 0x3f)
			Tex = pEm->Model_type & 0x3f;

		pP_ptr = &pEm->pSin_parts_ptr[0];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[1];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[2];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[3];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[4];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[5];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[6];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[7];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[9];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[10];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[12];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[13];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

		pP_ptr = &pEm->pSin_parts_ptr[16];
		Texture_Move((MODEL_WORK *)&pP_ptr->Attribute, pEm->Tpage + Tex, 0, pEm->Clut + 0x1e0 + Tex);

	}

	i = pEm->Parts_num;
	pP_ptr = pEm->pSin_parts_ptr;
	do {

		*(long *)&pP_ptr->Workm.m[0][0] = 0x1000;
		*(long *)&pP_ptr->Workm.m[0][2] = 0;
		*(long *)&pP_ptr->Workm.m[1][1] = 0x1000;
		*(long *)&pP_ptr->Workm.m[2][0] = 0;
		*(long *)&pP_ptr->Workm.m[2][2] = 0x1000;
		pP_ptr->Workm.t[0] = 0;
		pP_ptr->Workm.t[1] = 0;
		pP_ptr->Workm.t[2] = 0;
		*(long *)&pP_ptr->M[0][0] = 0x1000;
		*(long *)&pP_ptr->M[0][2] = 0;
		*(long *)&pP_ptr->M[1][1] = 0x1000;
		*(long *)&pP_ptr->M[2][0] = 0;
		*(long *)&pP_ptr->M[2][2] = 0x1000;
		pP_ptr->Attribute = 0;
		*(long *)&pP_ptr->Cdir_x = 0;
		pP_ptr->Cdir_z = 0;
		pP_ptr->Be_flg = 1;
		pP_ptr->Poly_rgb = 0x808080;

		pP_ptr->Free[0] = 0;
		pP_ptr->Free[1] = 0;
		pP_ptr->Free[2] = 0;
		pP_ptr->Free[3] = 0;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[5] = 0;

		pP_ptr++;

	}while (--i);

	pP_ptr = &pEm->pSin_parts_ptr[15];
	pP_ptr->Be_flg = 0;

	pP_ptr = &pEm->pSin_parts_ptr[16];
	pP_ptr->Be_flg = 0;

}

static void Em10_mv10_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf0011;	/* retail 0xf0011 (Beta 0x70011) */
		pEm->Routine_2 = 1;

		pEm->Spd_x = 11;

	case 1:
		if (pEm->Move_cnt == 10) {
			pP_ptr = &pEm->pSin_parts_ptr[8];
			Esp_call(0xa001000, pEm->Cdir_y, &pP_ptr->Workm, 0);
			Snd_se_enem(5, pEm);
		}

		if (Joint_move(pEm, Kan, Seq, 0x100)) {	/* retail 0x100 (Beta 0x200) */
			*(long *)&pEm->Routine_0 = 1;
			FREE_UB(0x26) = 60;	/* retail-only (sb 0x23e at retail 0x80104E28) */
		}

	}

}

static void Em10_mv11_br(ENEMY_WORK *pEm)
{

}

static void Em10_mv11(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, (unsigned long)pEm->pRoom_kan_t_ptr, (unsigned long)pEm->pRoom_seq_t_ptr, 0x100);

		if (pEm->Sce_free0 & 1) {
			pEm->Routine_2 = 1;

			*(long *)&pEm->Move_no = 0;	}
		break;

	case 1:
		Spd_get(pEm, (unsigned long)pEm->pRoom_kan_t_ptr, (unsigned long)pEm->pRoom_seq_t_ptr, 0);

		if (Joint_move(pEm, (unsigned long)pEm->pRoom_kan_t_ptr, (unsigned long)pEm->pRoom_seq_t_ptr, 0x100)) {

			pEm->Damage_cnt &= 0x7f;

			pEm->Timer3 = 0;
			pEm->Timer2 = 0;
			*(long *)&pEm->Routine_0 = 0x60501;
			pEm->Type &= 0xbfff;
			pEm->Be_flg &= ~8;	}

		Add_speedXZ(pEm, 0);
	}

}

static void Em10_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register PARTS_W *pS_ptr;
	register long Lim;	/* retail-only local, [INFERRED] name: Free[0xb] damage threshold */

	if (G.Player.Routine_0 == 5 && G.Player.pEnemy_ptr == (unsigned long *)pEm) {

		*(long *)&G.Player.Routine_0 = 1;
		G.Player.Damage_cnt &= 0x7f;
		G.Player.Neck_flg &= 0xed;

		pEm->Be_flg &= ~0x1004;
		G.Player.Be_flg &= ~0x1004;	}

	if (pEm->Type & 1) {
		Em10_haidm[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
		return;	}

	if (FREE_US(2) & 0x10) {
		Em10_dm07(pEm, Kan, Seq);
		return;	}

	Lim = 0;	/* retail: threshold 0x17 for Type 0x40 unless Routine_1 == 1 (retail 0x8010503C..0x8010505C) */
	if (pEm->Type & 0x40)
		Lim = (pEm->Routine_1 != 1) ? 0x17 : 0;
	if (!pEm->Routine_2 && FREE_SB(0xb) <= Lim) {

		if (FREE_UB(0xa) == 1 || pEm->Routine_1 == 1) {
			FREE_UB(0xa) = 1;
			*(long *)&pEm->Routine_0 = 0x501;
			FREE_UB(0xb) = (Rnd() & 0xf) + 0x10;
			if (!(pEm->Damage_no % 3)) {

				pP_ptr = &pEm->pSin_parts_ptr[1];
				Sv.vx = 0;
				Sv.vy = 800;
				Sv.vz = 0;
				Esp_call(0x17d0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			}else {

				pP_ptr = &pEm->pSin_parts_ptr[8];
				Sv.vx = pP_ptr->Pos_x;
				Sv.vy = pP_ptr->Pos_y + 500;
				Sv.vz = pP_ptr->Pos_z;
				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
			}
			return;
		}

		FREE_UB(0xa) = 1;
	}

	if (FREE_US(2) & 2) {
		pEm->Timer3 = 1;
		*(long *)&pEm->Routine_0 = 0x60501;

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80)) {
			Em10_Firedm(pEm);
			FREE_US(2) |= 0x800;	}

		if (pEm->Routine_1 == 11)
			Em10_Ryudm(pEm);

		if (pEm->Routine_1 == 9 && !(pEm->Type & 0x80))
			Em10_Tudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		if (pEm->Routine_1 == 16) {
			if (!(pEm->Type & 0x80) && FREE_UB(0x22) >= 9) {
				Em10_Firedm(pEm);
				FREE_US(2) |= 0x800;	}

			FREE_UB(0x22)++;	}

		return;	}

	if (!(FREE_US(2) & 0x60) && (signed char)pEm->D_life_c < 0 && (pEm->Damage_flg & 0xc0)) {

		Snd_se_enem(9, pEm);	/* retail-only (retail 0x80105374) */

		if (pEm->Damage_flg & 0x80) {
			pP_ptr = &pEm->pSin_parts_ptr[12];
			FREE_US(2) |= 0x40;
		}else {
			pP_ptr = &pEm->pSin_parts_ptr[9];
			FREE_US(2) |= 0x20;	}

		if ((Rnd() & 1) && pEm->Id != 30) {	/* retail: not for Id 30 */
			pS_ptr = &pEm->pSin_parts_ptr[15];
			pP_ptr->pTmd = pS_ptr->pTmd;
			pP_ptr->pTmd2 = pS_ptr->pTmd2;
			pP_ptr->pPacket = pS_ptr->pPacket;
			pP_ptr->pPacket2 = pS_ptr->pPacket2;	}

		Esp_call(0x1f40, Rnd() << 4, &pP_ptr->Workm, 0);

		pP_ptr++;
		pP_ptr->Be_flg |= 0x1062;
		pP_ptr->Old_x = pEm->Cdir_y + 0x800;
		pP_ptr->Timer3 = 0;
		pP_ptr->Old_z = 10;
		pP_ptr->Old_y = -200;
		pP_ptr->Timer1 = 50;
		pP_ptr->Old_x2 = 0;
		pP_ptr->Old_y2 = pEm->Cdir_y;
		pP_ptr->Old_z2 = 0x400;
		Esp_call(0x1b58, Rnd() << 4, &pP_ptr->Workm, 0);

	}

	Em10_dm[ pEm->Routine_1 - 1 ][ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em10_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	MATRIX M;
	VECTOR V;
	register short Dir;
	register unsigned char Cnt;
	register unsigned long LL;

	LL = pEm->L_pl;

	if (FREE_UB(0xa) == 1) {
		Em10_dm01(pEm, Kan, Seq);
		return;	}

	switch (pEm->Routine_2) {
	case 0:
		if (FREE_UB(0) != pEm->Move_no)
			*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;

		if (!FREE_UB(0x21)) {
			if (!(Rnd() & 1)) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(11, pEm);
			}
			FREE_UB(0x21) = 150;
		}

		pEm->Timer3 = 24;

		if (pEm->Routine_1 == 16) {
			if (!(pEm->Type & 0x80) && FREE_UB(0x22) >= 9) {
				Em10_Firedm(pEm);
				FREE_US(2) |= 0x800;	}

			FREE_UB(0x22)++;
			pEm->Timer3 = 2;	}

		Cnt = pEm->Move_cnt;
		pEm->Move_cnt = 0;
		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
		pEm->Move_cnt = Cnt;
		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);

		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;
		pEm->Timer1 = 1;
		Dir = 0;

		FREE_UB(0xb) -= Em10_dm00_usonum[ pEm->Routine_1 - 1 ];
		if (!(pEm->Type & 0x40) && FREE_SB(0xb) <= 0 && pEm->Life > 80)	/* retail-only (retail 0x801055E8..0x80105620) */
			FREE_UB(0xb) = (Rnd() & 0xf) + 16;

		if (pEm->Damage_flg & 0x20) {
			pEm->Timer1 = -1;
			Dir = 0x800;	}

		if (pEm->Routine_1 != 16) {
			if (pEm->Damage_no % 3) {

				pP_ptr = &pEm->pSin_parts_ptr[8];
				Sv.vx = pP_ptr->Pos_x - pEm->Timer1 * 100;
				Sv.vy = pP_ptr->Pos_y + 300;
				Sv.vz = pP_ptr->Pos_z;
				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(0x17d0, pEm->Cdir_y - Dir, &pP_ptr->Workm, &Sv);
			}else {

				pP_ptr = &pEm->pSin_parts_ptr[1];
				Sv.vx = 0;
				Sv.vy = 800;
				Sv.vz = 0;
				Esp_call(0x17d0, pEm->Cdir_y - Dir, &pP_ptr->Workm, &Sv);
			}
		}
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

	case 1:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);

		pEm->Spd_x >>= 1;
		pEm->Spd_z >>= 1;

		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = -(pEm->Timer0 * (signed char)pEm->Timer3 * 8) * pEm->Timer1;
		RotMatrix(&Sv, &M);
		MulMatrix(pP_ptr->M, &M);

		pP_ptr++;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = pEm->Timer0 * (signed char)pEm->Timer3 * (pEm->Timer1 << 3);
		RotMatrix(&Sv, &M);
		MulMatrix2(&M, pP_ptr->M);

		if (pEm->Timer0++ >= 3) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = 16;

			if (pEm->Routine_1 == 14)
				Em10_Spdm(pEm);
		}

		break;

	case 2:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);

		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = -(pEm->Timer0 * (signed char)pEm->Timer3) * pEm->Timer1;
		RotMatrix(&Sv, &M);
		MulMatrix(pP_ptr->M, &M);

		pP_ptr++;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = pEm->Timer0 * (signed char)pEm->Timer3 * pEm->Timer1;
		RotMatrix(&Sv, &M);
		MulMatrix2(&M, pP_ptr->M);

		if ((unsigned short)pEm->Timer0-- == 0)
			pEm->Routine_2 = 3;

		if (LL < 1300 && !(G.Player.Damage_cnt & 0x80) && pEm->nFloor == G.Player.nFloor && !(G.pEnemy[1]->Be_flg & 1)) {

			if (!(FREE_US(2) & 0x20)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&G.Player.Pos_x, pEm->Cdir_y + 0x100, 0x120))

					*(long *)&pEm->Routine_0 = 0x301;

			}

			if (!(FREE_US(2) & 0x40)) {
				if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&G.Player.Pos_x, pEm->Cdir_y - 0x100, 0x120))

					*(long *)&pEm->Routine_0 = 0x301;
			}
		}

		break;

	case 3:
		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200) && pEm->L_pl < 4000 && !(G.Player.Damage_cnt & 0x80) && !(Rnd() & 0xf)) {

			*(long *)&pEm->Routine_0 = 0xb01;
			return;	}

		if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400) && (Rnd() & 1)) {

			*(long *)&pEm->Routine_0 = 0xc01;
			return;	}

		if (Rnd() & 1) {
			*(long *)&pEm->Routine_0 = 0x201;
			return;	}

		*(long *)&pEm->Routine_0 = 0x101;

		if (FREE_UB(0) == pEm->Move_no) {
			*(long *)&pEm->Routine_0 = 0x10101;

			pEm->Timer3 = (Rnd() & 0xf) * 2;
			pEm->Timer0 = 30;
			pEm->Timer2 = (Rnd() & 0x1f) + 30;

			pEm->Timer1 = Rnd() + 300;	}

	}

	if (FREE_UB(0) == pEm->Move_no) {
		Add_speedXZ(pEm, 0);
	}

}

static void Em10_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Spd[2] = { -450, 0 };
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:
		Dir = pEm->Damage_flg & 0x20;	*(long *)&pEm->Move_no = 0x30004 - (Dir = Dir != 0);
		pEm->Routine_2 = 1;
		pEm->Timer1 = 10;

		pEm->Spd_x = Spd[ (pEm->Damage_flg >> 5) & 1 ];
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(13, pEm);
			}
			FREE_UB(0x21) = 150;
		}

		Dir = 0;
		pEm->Timer2 = 1;

		if (pEm->Damage_flg & 0x20) {
			Dir = 0x800;
			*(signed char *)&pEm->Timer2 = -1;	}

		if (pEm->Damage_no % 3) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = pP_ptr->Pos_x - (signed char)pEm->Timer2 * 100;
			Sv.vy = pP_ptr->Pos_y + 300;
			Sv.vz = pP_ptr->Pos_z;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(0x17d0, pEm->Cdir_y - Dir, &pP_ptr->Workm, &Sv);
		}else {

			pP_ptr = &pEm->pSin_parts_ptr[1];
			Sv.vx = 0;
			Sv.vy = 800;
			Sv.vz = 0;
			Esp_call(0x17d0, pEm->Cdir_y - Dir, &pP_ptr->Workm, &Sv);
			pEm->Timer2 = 0;
		}

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80)) {
			Em10_Firedm(pEm);
			FREE_US(2) |= 0x800;	}

		if (pEm->Routine_1 == 11) {

			if (!(FREE_US(2) & 0x1000) && !(Rnd() & 1)) {

				Dir = Rnd() >> (Rnd() & 3);
				FREE_US(2) |= 0x20;
				if (Dir & 1) {
					FREE_US(2) &= 0xffdf;
					FREE_US(2) |= 0x40;	}

				pP_ptr = &pEm->pSin_parts_ptr[ (Dir & 1) * 3 + 10 ];
				pP_ptr->Be_flg |= 0x10;
				pP_ptr->Free[0] = 0;
				pP_ptr->Free[1] = 0;
				pP_ptr->Free[3] = 0x8032;
				((unsigned char *)pP_ptr->Free)[4] = 5;
				((unsigned char *)pP_ptr->Free)[5] = 5;
				pP_ptr->Poly_rgb = 0x304040;
				Esp_call(0x40f0bb8, pEm->Cdir_y, &pP_ptr->Workm, 0);
				pP_ptr++;
				pP_ptr->Be_flg |= 0x10;
				pP_ptr->Free[0] = 0;
				pP_ptr->Free[1] = 0;
				pP_ptr->Free[3] = 0x8032;
				((unsigned char *)pP_ptr->Free)[4] = 5;
				((unsigned char *)pP_ptr->Free)[5] = 5;
				pP_ptr->Poly_rgb = 0x304040;
				Esp_call(0x40f07d0, pEm->Cdir_y, &pP_ptr->Workm, 0);
			}

			Em10_Ryudm(pEm);

		}

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0);

		if (pEm->Timer1 && *(signed char *)&pEm->Timer2) {
			if (!(pEm->Timer1 & 1)) {

				pP_ptr = pEm->pSin_parts_ptr;

				if (!FREE_UB(0xa)) {
					Esp_call(0x1f40, pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4), &pP_ptr->Workm, &pEm->Spd_x);

				}else {
					pEm->Spd_y -= 500;
					Esp_call(0x13e8, pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4), &pP_ptr->Workm, &pEm->Spd_x);

				}
			}
			pEm->Timer1--;
		}

		break;

	case 2:

		*(long *)&pEm->Routine_0 = 0x101;

		if (FREE_SB(0xb) <= 0) {	/* retail: <= 0 (bgtz at retail 0x80106018); Beta tested == 0 */
			FREE_UB(0xb) = (Rnd() & 0x10) + 0xf;
			FREE_UB(0xa) = 0;
		}

		if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200) && pEm->L_pl < 4000 && !(G.Player.Damage_cnt & 0x80) && !(Rnd() & 3)) {

			*(long *)&pEm->Routine_0 = 0xb01;

		}else if (pEm->L_pl < 3000 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 900) && (Rnd() & 1)) {

			*(long *)&pEm->Routine_0 = 0xc01;

		}else if (Rnd() & 1)
			*(long *)&pEm->Routine_0 = 0x201;

	}

}

static void Em10_Firedm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	pEm->Type |= 0x80;

	Sv.vx = 0;
	Sv.vy = 200;
	Sv.vz = 0;
	pP_ptr = pEm->pSin_parts_ptr;
	if (Rnd() & 1) {
		Esp_call(0x5032710, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		pP_ptr = &pEm->pSin_parts_ptr[8];
		if (pP_ptr->Be_flg & 1)	/* retail: part-present guards; the Beta parts[10]/[13] 0x5030fa0 calls are gone */
			Esp_call(0x5031388, pEm->Cdir_y, &pP_ptr->Workm, 0);
		pP_ptr = &pEm->pSin_parts_ptr[3];
		if (pP_ptr->Be_flg & 1)
			Esp_call(0x50313e8, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	}else {
		Esp_call(0x5032710, pEm->Cdir_y, &pP_ptr->Workm, 0);
		pP_ptr = &pEm->pSin_parts_ptr[3];
		if (pP_ptr->Be_flg & 1)
			Esp_call(0x5031388, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		pP_ptr = &pEm->pSin_parts_ptr[6];
		Sv.vy = -200;
		if (pP_ptr->Be_flg & 1)
			Esp_call(0x50313e8, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	}

	if (pEm->Routine_1 != 16) {

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Poly_rgb = 0x404040;
		pP_ptr = &pEm->pSin_parts_ptr[1];
		pP_ptr->Poly_rgb = 0x484848;
		pP_ptr = &pEm->pSin_parts_ptr[2];
		pP_ptr->Poly_rgb = 0x707070;
		pP_ptr = &pEm->pSin_parts_ptr[5];
		pP_ptr->Poly_rgb = 0x707070;
		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Poly_rgb = 0x505050;
		pP_ptr = &pEm->pSin_parts_ptr[9];
		pP_ptr->Poly_rgb = 0x505050;
		pP_ptr = &pEm->pSin_parts_ptr[10];
		pP_ptr->Poly_rgb = 0x484848;
		pP_ptr = &pEm->pSin_parts_ptr[11];
		pP_ptr->Poly_rgb = 0x404040;
		pP_ptr = &pEm->pSin_parts_ptr[12];
		pP_ptr->Poly_rgb = 0x505050;
		pP_ptr = &pEm->pSin_parts_ptr[13];
		pP_ptr->Poly_rgb = 0x404040;
		pP_ptr = &pEm->pSin_parts_ptr[14];
		pP_ptr->Poly_rgb = 0x404040;
	}

}

static void Em10_Ryudm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	FREE_US(2) |= 0x1800;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Poly_rgb = 0x304040;
	pP_ptr = &pEm->pSin_parts_ptr[1];
	pP_ptr->Poly_rgb = 0x304040;
	pP_ptr = &pEm->pSin_parts_ptr[2];
	pP_ptr->Poly_rgb = 0x405050;
	pP_ptr = &pEm->pSin_parts_ptr[5];
	pP_ptr->Poly_rgb = 0x405050;
	pP_ptr = &pEm->pSin_parts_ptr[8];
	pP_ptr->Poly_rgb = 0x304040;
	pP_ptr = &pEm->pSin_parts_ptr[9];
	pP_ptr->Poly_rgb = 0x506060;
	pP_ptr = &pEm->pSin_parts_ptr[12];
	pP_ptr->Poly_rgb = 0x506060;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(0x40f1770, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = &pEm->pSin_parts_ptr[12];
	if (pP_ptr->Be_flg & 1)	/* retail-only guard */
		Esp_call(0x40f0fa0, pEm->Cdir_y, &pP_ptr->Workm, 0);
	pP_ptr = &pEm->pSin_parts_ptr[3];
	if (pP_ptr->Be_flg & 1)	/* retail-only guard */
		Esp_call(0x40f0fa0, pEm->Cdir_y, &pP_ptr->Workm, 0);

}

static void Em10_Tudm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	Sv.vx = 0;
	Sv.vy = 200;
	Sv.vz = 0;
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(0x5032710, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = &pEm->pSin_parts_ptr[3];
	if (pP_ptr->Be_flg & 1)	/* retail-only guard (retail 0x80106450) */
		Esp_call(0x50313e8, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Poly_rgb = 0x404040;
	pP_ptr = &pEm->pSin_parts_ptr[1];
	pP_ptr->Poly_rgb = 0x484848;
	pP_ptr = &pEm->pSin_parts_ptr[2];
	pP_ptr->Poly_rgb = 0x707070;
	pP_ptr = &pEm->pSin_parts_ptr[5];
	pP_ptr->Poly_rgb = 0x707070;
	pP_ptr = &pEm->pSin_parts_ptr[9];
	pP_ptr->Poly_rgb = 0x505050;
	pP_ptr = &pEm->pSin_parts_ptr[10];
	pP_ptr->Poly_rgb = 0x484848;
	pP_ptr = &pEm->pSin_parts_ptr[11];
	pP_ptr->Poly_rgb = 0x404040;
	pP_ptr = &pEm->pSin_parts_ptr[12];
	pP_ptr->Poly_rgb = 0x505050;
	pP_ptr = &pEm->pSin_parts_ptr[13];
	pP_ptr->Poly_rgb = 0x404040;
	pP_ptr = &pEm->pSin_parts_ptr[14];
	pP_ptr->Poly_rgb = 0x404040;

}

static void Em10_Spdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13e8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1000) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1000) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0xc18) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 3) + 0x1000) | 0x40c0000, pEm->Cdir_y, &pP_ptr->Workm, 0);

	pP_ptr = &pEm->pSin_parts_ptr[9];
	Esp_call(((Rnd() << 3) + 0xc18) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = &pEm->pSin_parts_ptr[3];
	Esp_call(((Rnd() << 3) + 0xc18) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

}

static void Em10_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	short Spd[2] = { -250, 100 };
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	MATRIX M, M1;
	register unsigned long i;
	register short Dir;
	register long Tmp;
	if (FREE_UB(0x19) == 1) {
		Em10_die04(pEm, Kan, Seq);
		return;	}

	if (FREE_UB(0x19) == 2) {
		Em10_die03(pEm, Kan, Seq);
		return;	}

	switch (pEm->Routine_2) {
	case 0:

		if (G.Player.Be_flg & 0x80000000) {
			FREE_UB(0x19) = 2;
			Em10_die03(pEm, Kan, Seq);
			return;	}

		if (pEm->Water || (G.Status_flg & 0x10000000)) {	/* retail: also on the 0x10000000 status flag (retail 0x801067D8) */
			FREE_UB(0x19) = 1;
			Em10_die04(pEm, Kan, Seq);
			return;	}

		if ((Rnd() & 3) && pEm->Routine_0 == 3) {
			FREE_UB(0x19) = 1;
			Em10_die04(pEm, Kan, Seq);
			return;	}

		Dir = pEm->Damage_flg & 0x20;	*(long *)&pEm->Move_no = 0x30004 - (Dir = Dir != 0);
		pEm->Routine_2 = 1;
		pEm->Super_vector.vx = 0;
		pEm->Super_vector.vy = 0;
		pEm->Super_vector.vz = 0;

		pEm->Timer0 = 0;
		pEm->Timer3 = 0;

		pEm->Spd_x = Spd[ (pEm->Damage_flg >> 5) & 1 ];
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101030;
		Esp_call(0x1770, Rnd() << 4, &pP_ptr->Workm, 0);

		Esp_call(((Rnd() + 0x7d0) << 1) | 0x8000000, Rnd() << 4, &pP_ptr->Workm, 0);

		i = (Rnd() & 3) + 2;
		do {
			Esp_call((Rnd() + 0xc00) | 0x9020000, Rnd() << 4, &pP_ptr->Workm, 0);

		}while (--i);

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(13, pEm);
			}
			FREE_UB(0x21) = 150;
		}
		Snd_se_enem(9, pEm);	/* retail-only (retail 0x80106988) */

		FREE_SS(0x1a) = pEm->Ground;

		Em_dead_flg_on(pEm);

		pEm->Damage_cnt |= 0x80;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0);

		if (pEm->Routine_2 == 2) {
			pP_ptr = pEm->pSin_parts_ptr;
			RotMatrix(&pEm->Super_vector, &M);
			MulMatrix(pP_ptr->M, &M);
			pP_ptr++;
			TransposeMatrix(&M, &M1);
			MulMatrix2(&M1, pP_ptr->M);
		}

		if (pEm->Move_cnt) {
			if (!(pEm->Move_cnt & 3)) {

				pP_ptr = pEm->pSin_parts_ptr;

				Esp_call((Rnd() << 3) + 0x1770, Rnd() << 4, &pP_ptr->Workm, &pEm->Spd_x);

			}
		}

		break;

	case 2:
		FREE_US(2) &= 0xfffb;

		pEm->Damage_cnt &= 0x7f;

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Old_y = -200;
		pP_ptr->Cdir_x = 0;
		pP_ptr->Cdir_y = 0;
		pP_ptr->Cdir_z = 0x400;
		pEm->Pos_y = FREE_SS(0x1a);

		FREE_US(2) |= 1;

		if (pEm->Routine_0 == 3)
			pEm->Life = 10;

		pEm->Type = 0x2001;	/* retail 0x2001 (Beta 1) */
		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Atd[0].At_w = 200;
		pEm->Atd[0].At_d = 200;
		pEm->Atd[0].W = 200;
		pEm->Atd[0].D = 200;
		pEm->Atd[0].Ofs_y = -350;
		pEm->Atd[0].At_h = 350;

		*(long *)&pEm->Routine_0 = 1;
		*(long *)&pEm->Move_no = 0xf0005;
		Joint_move(pEm, Kan, Seq, 0x100);
		return;

	}

	if (pEm->Routine_2 < 2) {
		pP_ptr = pEm->pSin_parts_ptr;
		if (pEm->Move_cnt < 10) {
			pEm->Super_vector.vx -= (signed char)pEm->Timer3 * 16;
			pEm->Super_vector.vy = 0;
			pEm->Super_vector.vz += (signed char)pEm->Timer3 * 24;
			pEm->Timer3++;	}

		if ((unsigned)(pEm->Move_cnt - 11) < 19) {
			pEm->Super_vector.vx += (signed char)pEm->Timer3 * 8;
			pEm->Super_vector.vy -= pEm->Timer0 * 2;
			pEm->Super_vector.vz -= (signed char)pEm->Timer3 * 2;

			pEm->Timer3--;
			pEm->Timer0++;	}

		if ((unsigned)(pEm->Move_cnt - 31) < 19) {
			pEm->Super_vector.vx -= (signed char)pEm->Timer3 * 4;
			pEm->Super_vector.vy += pEm->Timer0 * 2;
			pEm->Super_vector.vz -= (signed char)pEm->Timer3 * 2;

			pEm->Timer3++;
			pEm->Timer0--;	}

		RotMatrix(&pEm->Super_vector, &M);
		MulMatrix(pP_ptr->M, &M);
		pP_ptr++;
		TransposeMatrix(&M, &M1);
		MulMatrix2(&M1, pP_ptr->M);
	}

	if (pEm->Move_cnt == 20 && pEm->Routine_2 == 1) {
		pP_ptr = &pEm->pSin_parts_ptr[1];
		pP_ptr->pSuper = (unsigned long *)&pEm->Super_matrix;
		pEm->Super_matrix = pP_ptr[-1].Workm;

		pEm->Super_matrix.t[0] = pEm->Pos_x;
		pEm->Super_matrix.t[2] = pEm->Pos_z;

		pEm->Timer1 = 10;
		pEm->Timer2 = 2;

		pP_ptr = &pEm->pSin_parts_ptr[1];

		Esp_call(0x31000, Rnd() << 4, &pP_ptr->Workm, 0);
		Snd_se_enem(9, pEm);	/* retail-only (retail 0x80106D84) */

	}

	if (pEm->Move_cnt >= 20 && pEm->Routine_2 == 1) {
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr[1].Be_flg |= 0x9000;
		pP_ptr[2].Be_flg |= 0x9000;
		pP_ptr[3].Be_flg |= 0x9000;
		pP_ptr[4].Be_flg |= 0x9000;
		pP_ptr[5].Be_flg |= 0x9000;
		pP_ptr[6].Be_flg |= 0x9000;
		pP_ptr[7].Be_flg |= 0x9000;
		RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
		CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &M);
		*(long *)&pEm->Super_matrix.m[0][0] = *(long *)&M.m[0][0];
		*(long *)&pEm->Super_matrix.m[0][2] = *(long *)&M.m[0][2];
		*(long *)&pEm->Super_matrix.m[1][1] = *(long *)&M.m[1][1];
		*(long *)&pEm->Super_matrix.m[2][0] = *(long *)&M.m[2][0];
		*(long *)&pEm->Super_matrix.m[2][2] = *(long *)&M.m[2][2];

		pEm->Super_matrix.t[1] = pP_ptr->Pos_y + FREE_SS(0x1a);

		if (*(signed char *)&pEm->Timer2) {
			pEm->Pos_y += pEm->Timer1;
			pEm->Timer1 += 5;
			if (pEm->Move_cnt >= 35)
				pEm->Timer1 += 50;	}

		if (FREE_SS(0x1a) - (200 + pP_ptr->Pos_y) < pEm->Pos_y && *(signed char *)&pEm->Timer2) {
			Tmp = pEm->Timer1;
			pEm->Timer1 = -(Tmp >> 3);
			pEm->Pos_y = FREE_SS(0x1a) - (300 + pP_ptr->Pos_y);

			pEm->Timer2--;

			Snd_se_enem(4, pEm);

		}

	}

}

static void Em10_dm03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Spd[2] = { -450, 250 };
	SVECTOR Sv;
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:
		Dir = pEm->Damage_flg & 0x20;	*(long *)&pEm->Move_no = 0x30004 - (Dir = Dir != 0);
		pEm->Routine_2 = 1;
		pEm->Timer1 = 10;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0);
		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x101;
	}

}

static void Em10_dm04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	MATRIX M;
	SVECTOR Rot;
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:

		pP_ptr = (PARTS_W *)pEm->pBow_hit_parts;
		pEm->Timer2 = ((unsigned char *)pP_ptr->Free)[1];
		if (((unsigned char *)pP_ptr->Free)[1] == 3 || ((unsigned char *)pP_ptr->Free)[1] == 6) {
			pEm->Damage_no = 3;
			break;	}

		pEm->Timer0 = pP_ptr->Free[1];

		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 1;
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg &= ~0x40;

		if (!FREE_UB(0x21)) {
			if (Rnd() & 1) {
				Snd_se_enem(12, pEm);	}
			else {
				Snd_se_enem(13, pEm);
			}
			FREE_UB(0x21) = 150;
		}

		pEm->Spd_x = 300;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;

		pEm->Timer1 = 0;
		pEm->Timer3 = 50;

		Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		pP_ptr = &pEm->pSin_parts_ptr[ (signed char)pEm->Timer2 ];
		Esp_call((Rnd() << 3) + 0x1770, Dir, &pP_ptr->Workm, 0);

	case 1:
		Rot.vx = 0;
		Rot.vy = pEm->Timer0;
		Rot.vz = 0;
		RotMatrix(&Rot, &M);

		ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Rot);

		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		pEm->Pos_x += Rot.vx;
		pEm->Pos_z += Rot.vz;

		pEm->Spd_x -= pEm->Timer1;
		pEm->Timer1 += (signed char)pEm->Timer3;

		if (pEm->Spd_x < 0)
			pEm->Routine_2 = 6;
		/* retail: the Beta Sca_info & 1 wall-hit block (Routine_2 = 2, Esp_call, SE) is gone */

		break;

	case 2:
		pP_ptr = pEm->pSin_parts_ptr;
		pEm->Routine_2 = 3;
		pEm->Timer3 = (Rnd() & 0x3f) + 30;
		pEm->Timer1 = pP_ptr->Pos_y;
		pEm->Timer0 = 0x1000;

	case 3:
		if ((unsigned char)pEm->Timer3-- == 0)
			pEm->Routine_2 = 4;

		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		pP_ptr = pEm->pSin_parts_ptr;
		M = *(MATRIX *)pP_ptr->M;
		HMatrix(&GsIDMATRIX, &M, (MATRIX *)pP_ptr->M, pEm->Timer0);
		pP_ptr->Pos_y = pEm->Timer1;

		if (pEm->Timer0 > 0x400)

			pEm->Timer0 -= 0x200;

		break;

	case 4:
		pEm->Routine_2 = 5;
		pEm->Timer3 = 20;

	case 5:
		if ((unsigned char)pEm->Timer3-- == 0)
			pEm->Routine_2 = 6;

		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		pP_ptr = pEm->pSin_parts_ptr;
		M = *(MATRIX *)pP_ptr->M;
		HMatrix(&GsIDMATRIX, &M, (MATRIX *)pP_ptr->M, pEm->Timer0);
		pP_ptr->Pos_y = pEm->Timer1;

		pEm->Timer0 += 0x99;

		break;

	case 6:
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg &= ~0x40;

		*(long *)&pEm->Routine_0 = 0x101;

		if (Rnd() & 1)
			*(long *)&pEm->Routine_0 = 0x201;

	}

}

static void Em10_dm05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Spd[2] = { -450, 250 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf0001;
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Move_cnt = Rnd() & 7;
		Spd_get(pEm, Kan, Seq, 0);

		pEm->Routine_2 = 1;

		pEm->Damage_cnt |= 0x80;

		if (pEm->Routine_1 != 12) {

			pEm->Timer3 = Rnd() & 1;
			if (pEm->Timer3) {

				pP_ptr = &pEm->pSin_parts_ptr[3];
				pP_ptr->Be_flg |= 0x4a;
				pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
				pP_ptr->Free[4] = 0;
				pP_ptr->Free[2] = 0;
				pP_ptr->Free[1] = -10;
				pP_ptr->Free[3] = 0;
				pP_ptr->Free[6] = 0;
				pP_ptr->Poly_rgb = 0x10104f;
				Esp_kill(5, 3, &pP_ptr->Workm);	/* retail-only */

				pP_ptr = &pEm->pSin_parts_ptr[4];
				pP_ptr->Be_flg = 0;

				Snd_se_enem(2, pEm);

				if (Rnd() & 1) {
					pP_ptr = &pEm->pSin_parts_ptr[5];
					pP_ptr->Be_flg |= 0x4a;
					pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
					pP_ptr->Free[4] = 0;
					pP_ptr->Free[2] = 0;
					pP_ptr->Free[1] = -10;
					pP_ptr->Free[3] = 0;
					pP_ptr->Free[6] = 0;
					pP_ptr->Poly_rgb = 0x10104f;
					Esp_call(0x1770, 0, &pP_ptr->Workm, 0);
					Esp_call(0x8001000, Rnd() << 4, &pP_ptr->Workm, 0);
					Esp_kill(5, 3, &pP_ptr->Workm);	/* retail-only */
					pP_ptr = &pEm->pSin_parts_ptr[6];
					pP_ptr->Be_flg |= 0x4a;
					pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
					pP_ptr->Free[4] = 0;
					pP_ptr->Free[2] = 0;
					pP_ptr->Free[1] = -10;
					pP_ptr->Free[3] = 0;
					pP_ptr->Free[6] = 0;
					pP_ptr->Poly_rgb = 0x10104f;
					Esp_call(0x1770, 0, &pP_ptr->Workm, 0);
					Esp_kill(5, 3, &pP_ptr->Workm);	/* retail-only */
					pP_ptr = &pEm->pSin_parts_ptr[7];
					pP_ptr->Be_flg = 0;
				}
			}

			pP_ptr = &pEm->pSin_parts_ptr[3];
			Sv.vx = 0;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x1388, 0, &pP_ptr->Workm, &Sv);

			Sv.vy = 400;
			Esp_call(0x1770, Rnd() << 2, &pP_ptr->Workm, &Sv);

			Sv.vy = 800;
			Esp_call(0xfa0, Rnd() << 4, &pP_ptr->Workm, &Sv);

			Esp_call(0x8001000, Rnd() << 4, &pP_ptr->Workm, 0);

		}

		if (!FREE_UB(0x21)) {
			Snd_se_enem(12, pEm);
			FREE_UB(0x21) = 150;
		}

		Em_dead_flg_on(pEm);

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		break;

	case 2:

		if (pEm->Routine_0 == 3) {

			if ((Rnd() & 3) || !*(signed char *)&pEm->Timer3)
				pEm->Life = 10;
			else {
				FREE_US(2) &= 0xfffb;
				*(long *)&pEm->Routine_0 = 7;
				break;	}
		}

		if (pEm->Water || (G.Status_flg & 0x10000000)) {	/* retail: + status flag */
			FREE_US(2) &= 0xfffb;
			*(long *)&pEm->Routine_0 = 7;
			break;	}

		pEm->Be_flg &= 0xf3ffffff;
		pEm->Be_flg |= 0x4000000;

		pEm->Atd[0].At_w = 200;
		pEm->Atd[0].At_d = 200;
		pEm->Atd[0].W = 200;
		pEm->Atd[0].D = 200;
		pEm->Atd[0].Ofs_y = -350;
		pEm->Atd[0].At_h = 350;
		pEm->Type = 0x2001;	/* retail 0x2001 (Beta 1) */

		if (Rnd() & 1)
			*(long *)&pEm->Routine_0 = 1;
		else
			*(long *)&pEm->Routine_0 = 0x201;

		pEm->Damage_cnt &= 0x7f;

	}

}

static void Em10_dm06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Spd[2] = { -450, 250 };
	register PARTS_W *pP_ptr;
	SVECTOR Sv;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0x30006;
		pEm->Routine_2 = 1;

		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = 0;
		Esp_call(0x1770, 0, &pP_ptr->Workm, &Sv);

		if (!FREE_UB(0x21)) {
			Snd_se_enem(12, pEm);
			FREE_UB(0x21) = 150;
		}

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80)) {
			Em10_Firedm(pEm);
			FREE_US(2) |= 0x800;	}

		if (pEm->Routine_1 == 11)
			Em10_Ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		if (pEm->Routine_1 == 16) {
			if (!(pEm->Type & 0x80) && FREE_UB(0x22) >= 9) {
				Em10_Firedm(pEm);
				FREE_US(2) |= 0x800;	}

			FREE_UB(0x22)++;	}

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);

		break;

	case 2:
		pEm->Type = 1;
		*(long *)&pEm->Routine_0 = 1;
	}

}

static void Em10_dm07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	MATRIX M, M1;
	VECTOR V;

	Sv.vx = 0;
	Sv.vy = 0;
	Sv.vz = 0;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;
		pEm->Timer1 = 1;
		pEm->Timer2 = 0;

		if (pEm->Damage_flg & 0x20)
			pEm->Timer1 = -1;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		Sv.vy = 300;
		Esp_call(0x17d0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		pEm->Hokan_flg = 15;

		if ((OVL_FREE(pEm)[4] & 0xff) == 1) {
			OVL_FREE(pEm)[5] = OVL_FREE(pEm)[4];

			if ((OVL_FREE(pEm)[5] & 0xff0000) == 2) {
				Print8(0x20, 0x84, 0, 0, "BUG", 0);
			}
		}

		if (pEm->Life < 0) {
			pEm->Life = 0;
			pEm->Timer2 = 1;

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Esp_call(0x31f40, Rnd() << 4, &pP_ptr->Workm, &Sv);
			pEm->Hokan_flg = 15;
		}

		if (!FREE_UB(0x21)) {
			Snd_se_enem(12, pEm);
			FREE_UB(0x21) = 150;
		}

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80) && pEm->Damage_no < 3) {

			Em10_Firedm(pEm);
			FREE_US(2) |= 0x800;	}

		pEm->Timer3 = 24;

		if (pEm->Routine_1 == 16) {
			if (!(pEm->Type & 0x80) && FREE_UB(0x22) >= 9) {
				Em10_Firedm(pEm);
				FREE_US(2) |= 0x800;	}

			FREE_UB(0x22)++;
			pEm->Timer3 = 2;	}

		if (pEm->Routine_1 == 15 || pEm->Routine_1 == 18)	/* retail: type 18 also */
			pEm->Timer3 = 2;

		if (pEm->Routine_1 == 11 && pEm->Damage_no < 3)

			Em10_Ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

	case 1:
		if (!Joint_move(pEm, Kan, Seq, 0x100)) {

			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vz = -(pEm->Timer0 * (signed char)pEm->Timer3 * 8) * pEm->Timer1;
			RotMatrix(&Sv, &M);
			MulMatrix(pP_ptr->M, &M);

			pP_ptr++;

			TransposeMatrix(&M, &M1);
			MulMatrix2(&M1, pP_ptr->M);

			if (pEm->Timer0++ >= 3) {
				pEm->Routine_2 = 2;
				pEm->Timer0 = 16;	}

			break;	}
		else {	*(long *)&pEm->Routine_0 = OVL_FREE(pEm)[5]; pEm->Routine_2++; FREE_US(2) &= 0xffef; FREE_US(2) &= 0xfffd; pEm->Be_flg |= 0xc000000; pEm->Type &= 0xdfff; pEm->Atd[0].At_w = 500; pEm->Atd[0].At_d = 500; pEm->Atd[0].W = 500; pEm->Atd[0].D = 500; pEm->Atd[0].Ofs_y = -1500; pEm->Atd[0].At_h = 1500;	}
		break;
	case 2:
		if (!Joint_move(pEm, Kan, Seq, 0x100)) {

			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vz = -(pEm->Timer0 * (signed char)pEm->Timer3) * pEm->Timer1;
			RotMatrix(&Sv, &M);
			MulMatrix(pP_ptr->M, &M);

			pP_ptr++;

			TransposeMatrix(&M, &M1);
			MulMatrix2(&M1, pP_ptr->M);

			if ((unsigned short)pEm->Timer0-- == 0)
				pEm->Routine_2 = 3;

			break;	}
		else {	*(long *)&pEm->Routine_0 = OVL_FREE(pEm)[5]; pEm->Routine_2++; FREE_US(2) &= 0xffef; FREE_US(2) &= 0xfffd; pEm->Be_flg |= 0xc000000; pEm->Type &= 0xdfff; pEm->Atd[0].At_w = 500; pEm->Atd[0].At_d = 500; pEm->Atd[0].W = 500; pEm->Atd[0].D = 500; pEm->Atd[0].Ofs_y = -1500; pEm->Atd[0].At_h = 1500;
		}	break;
	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Routine_0 = OVL_FREE(pEm)[5];
			pEm->Routine_2++;
			FREE_US(2) &= 0xffef;

			FREE_US(2) &= 0xfffd;

			pEm->Be_flg |= 0xc000000;
			pEm->Type &= 0xdfff;
			pEm->Atd[0].At_w = 500;
			pEm->Atd[0].At_d = 500;
			pEm->Atd[0].W = 500;
			pEm->Atd[0].D = 500;
			pEm->Atd[0].Ofs_y = -1500;
			pEm->Atd[0].At_h = 1500;
		}

	}

}

static void Em10_dm08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	unsigned char Em10_dm08_mot[8] = { 29, 28, 30, 28, 28, 29, 28, 30 };

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = Em10_dm08_mot[ Rnd() & 7 ] + 0x70000;
		pEm->Routine_2 = 1;

		pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		if (pEm->Damage_no % 3) {
			/* retail: one blood effect with a random 0x10000 flag; the Beta extra spray and the
			 * Routine_1 == 18 effect are gone (retail 0x80107FF4..0x80108104) */
			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vx = 0x100 - (Rnd() << 1);
			Sv.vy = 0x138 - (Rnd() << 2);
			Sv.vz = 0x100 - (Rnd() << 1);
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1b58), pEm->Timer0, &pP_ptr->Workm, &Sv);

		}else {

			pP_ptr = &pEm->pSin_parts_ptr[1];
			Sv.vx = 0x7f - Rnd();
			Sv.vy = 0x39f - Rnd();
			Sv.vz = 0x7f - Rnd();
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x17d0), pEm->Timer0, &pP_ptr->Workm, &Sv);

		}

		if (!FREE_UB(0x21)) {
			Snd_se_enem(12, pEm);
			FREE_UB(0x21) = 150;
		}

		pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		pEm->Spd_x = (Rnd() & 0x3f) + 20;
		pEm->Spd_z = 0x40 - (Rnd() & 0x7f);
		Add_speedXZ(pEm, pEm->Timer0 - pEm->Cdir_y + 0x800);
		pEm->Cdir_y += 0x40 - (Rnd() & 0x7f);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);
		break;

	case 2:

		*(long *)&pEm->Routine_0 = 0x101;
		if (Rnd() & 1)
			*(long *)&pEm->Routine_0 = 0x201;

		if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200) && pEm->L_pl < 5000 && !(G.Player.Damage_cnt & 0x80) && !(Rnd() & 7))

			*(long *)&pEm->Routine_0 = 0xb01;

	}

}

#include "EM10_S0.C"
