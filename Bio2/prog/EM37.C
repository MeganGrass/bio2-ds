/* em37.c - reconstruction of D:\Bio2\prog\EM37.C (enemy 37).
 * Linked into two overlays from the same source: 0x1A (at 0x8010012c) and
 * 0x38 (at 0x8010d12c); only the enemy-number-specific entry names differ
 * (Em137/Em237, Pldamege_Em137/237, Pldie_Em137/237), selected by OVL_38.
 * Types come from the TU's retail SYM preamble (em37.h).
 * Parts pointers written as `pSin_parts_ptr + N` (Em137, Em37_Init, Em37_mv02,
 * Em37_mv09, Em37_mv0b, Em37_ob_at/_ob_at1) are folded into the loaded base by
 * CSE; that is what puts the base in its own register across the following
 * call and leaves the variable's retail SYM record (or no record) as shipped. */
#include "EM37.H"

/* Em37 work variables kept in the enemy's generic Free[] area */
#define EF	((EM37_FREE *)ENEMY_FREE(pEm))

/* Timer2 and Timer3 are plain char; the retail code reads them as signed bytes (lb) in these tests. */
#define S8(x)	(*(signed char *)&(x))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
/* Retail life tables (em37_cdemd0_s1 .data 0x80105608..0x80105667): Em2b values changed relative to Beta 2 and a
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
static short Em37_pos_xyz[4][4] = {
	{ 965, -2243, -1933, 0 },
	{ 1615, -706, -2440, 0 },
	{ 1409, -683, 2495, 2048 },
	{ 957, -2426, 1958, 2048 }
};
static void (*Em37_routine_0[8])() = {
	&Em37_Init,
	&Em37_Move,
	&Em37_Damage,
	&Em37_Die,
	&Em37_Scenario,
	0,
	0,
	&Em37_Dead
};
static void (*Em37_Move_type[2])() = {	/* retail: both entries Em37_Move_type00 (.data 0x801056A8) */
	&Em37_Move_type00,
	&Em37_Move_type00
};
static void (*Em37_mv_br_type00[16])() = {	/* retail: 16 entries (.data 0x801056B0) */
	&Em37_mv00_br,
	&Em37_mv01_br,
	&Em37_mv02_br,
	&Em37_mv03_br,
	&Em37_mv04_br,
	&Em37_mv05_br,
	&Em37_mv06_br,
	&Em37_mv07_br,
	&Em37_mv08_br,
	&Em37_mv09_br,
	&Em37_mv0a_br,
	&Em37_mv0b_br,
	&Em37_mv0c_br,
	&Em37_mv0d_br,
	&Em37_mv0e_br,
	&func_8010404C
};
static void (*Em37_mv_type00[16])() = {	/* retail: 16 entries */
	&Em37_mv00,
	&Em37_mv01,
	&Em37_mv02,
	&Em37_mv03,
	&Em37_mv04,
	&Em37_mv05,
	&Em37_mv06,
	&Em37_mv07,
	&Em37_mv08,
	&Em37_mv09,
	&Em37_mv0a,
	&Em37_mv0b,
	&Em37_mv0c,
	&Em37_mv0d,
	&Em37_mv0e,
	&func_80104054
};
static void (*Em37_mv_br_type01[])() = { };
static void (*Em37_mv_type01[])() = { };
static unsigned char Em37_mv03_tbl[2][4] = { { 30, 55, 60, 70 }, { 38, 50, 52, 60 } };
static short Em37_mv03_dir_tbl[4] = { 896, 1088, 2976, 3200 };
static short Em37_mv08_tbl[4][6] = {
	{ 768, 0, 32, 2304, 0, 128 },
	{ 1536, 0, 32, 2816, 0, 128 },
	{ 2560, 0, 32, 64, 0, -64 },
	{ 3584, 0, 32, 1920, 0, 128 }
};
static short Em37_mv0b_tbl[16][4] = {
	{ 1920, 0, 128, 0 },
	{ 64, 0, -64, 0 },
	{ 64, 0, -64, 0 },
	{ 1536, 0, 128, 0 },
	{ 2048, 0, 128, 0 },
	{ 2560, 0, 128, 0 },
	{ 896, 0, 128, 0 },
	{ 2048, 0, 128, 0 },
	{ 2944, 0, 128, 0 },
	{ 3072, 0, 128, 0 },
	{ 896, 0, 128, 0 },
	{ 1280, 0, 128, 0 },
	{ 2304, 0, 128, 0 },
	{ 2816, 0, 128, 0 },
	{ 64, 0, -64, 0 },
	{ 1920, 0, 128, 0 }
};
/* retail-only (.data 0x801057F0, name [INFERRED]): Em37_mv0d move offset, start Cdir_z, spare per tentacle; placed before Em37_dm as in retail
 * (Em37_Damage's &Em37_dm[-1] = 0x801057E4, so the zero-size Em37_dm sits at 0x80105808, after this table) */
static short Em37_mv0d_tbl[4][3] = { { 0x17, -0x80, 0x22 }, { 0x15, -0x100, 0x20 }, { 0x15, -0x100, 0x20 }, { 0x17, -0x80, 0x22 } };
static void (*Em37_dm[][9])() = { };
static unsigned char Em37_Dm_cnt[18][2] = { { 1, 2 }, { 2, 1 }, { 2, 1 }, { 6, 1 }, { 8, 2 }, { 10, 2 }, { 2, 1 }, { 3, 1 }, { 2, 2 }, { 20, 3 }, { 20, 3 }, { 9, 0 }, { 2, 1 }, { 10, 0 }, { 1, 0 }, { 2, 0 }, { 20, 0 }, { 1, 0 } };
static void (*Em37_die[][9])() = { };
static void (*Em37_sce[])() = { };
static void (*Pldamage_Em37_tbl[5])() = {	/* retail: two more states (.data 0x80105838) */
	&Em37_pldm00,
	&Em37_pldm00,
	&Em37_pldm01,
	&func_801046AC,
	&func_801048F4
};
static void (*Pldie_Em37_tbl[])() = { };
static JH_TBL G5s_jh0_tbl[30] = {
	{ 1, 4096 }, { 13, 4096 }, { 25, 4096 }, { 37, 4096 }, { 49, 4096 }, { 2, 3296 },
	{ 14, 3296 }, { 26, 3296 }, { 38, 3296 }, { 50, 3296 }, { 3, 2496 }, { 15, 2496 },
	{ 27, 2496 }, { 39, 2496 }, { 51, 2496 }, { 4, 1600 }, { 16, 1600 }, { 28, 1600 },
	{ 40, 1600 }, { 52, 1600 }, { 5, 800 }, { 17, 800 }, { 29, 800 }, { 41, 800 },
	{ 53, 800 }, { 6, 0 }, { 18, 0 }, { 30, 0 }, { 42, 0 }, { 54, 0 }
};
static JH_HEAD G5s_jh0_head[1] = { { 0, 1, 30, 0, 0, 0, 0, { 0, 0, 0, 0 }, 0, 0, 0, 0, { -3750, 0, 0, 0 }, G5s_jh0_tbl } };
static JH_TBL G5s_jh1_tbl[20] = {
	{ 7, 3072 }, { 19, 3072 }, { 31, 3072 }, { 43, 3072 }, { 55, 3072 }, { 8, 2048 },
	{ 20, 2048 }, { 32, 2048 }, { 44, 2048 }, { 56, 2048 }, { 9, 1024 }, { 21, 1024 },
	{ 33, 1024 }, { 45, 1024 }, { 57, 1024 }, { 10, 0 }, { 22, 0 }, { 34, 0 },
	{ 46, 0 }, { 58, 0 }
};
static JH_HEAD G5s_jh1_head[1] = { { 1, 2, 20, 0, 0, 0, 0, { -3750, 0, 0, 0 }, 0, 0, 0, 0, { -6753, 0, 0, 0 }, G5s_jh1_tbl } };
static JH_TBL G5s_jh2_tbl[16] = {
	{ 11, 3072 }, { 23, 3072 }, { 35, 3072 }, { 47, 3072 }, { 59, 3072 }, { 12, 2048 },
	{ 24, 2048 }, { 36, 2048 }, { 48, 2048 }, { 60, 2048 }, { 61, 1024 }, { 62, 1024 },
	{ 63, 1024 }, { 64, 1024 }, { 65, 1024 }, { 0, 0 }
};
static JH_HEAD G5s_jh2_head[1] = { { 2, 3, 16, 0, 0, 0, 0, { -6753, 0, 0, 0 }, 0, 0, 0, 0, { -8261, 0, 0, 0 }, G5s_jh2_tbl } };

void EM_MAIN(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;

if (!(G.Stop_flg & 0x20000000)) {

	if (pEm->Type == 8) {

		Em37_ob_at1(pEm);
		pEm->Be_flg &= ~1;
		return;
	}

	if (pEm->Damage_cnt & 0x7F)
		pEm->Damage_cnt--;

	if (pEm->Routine_0) {
		switch (EF->Nemoto_no) {
		case 0:
			pP_ptr = G.pEnemy[2]->pSin_parts_ptr + 2;
			Sv = *(SVECTOR *)Em37_pos_xyz[ EF->No ];
			ApplyMatrixSV(&pP_ptr->Workm, &Sv, &Sv);
			pEm->Pos_x = pP_ptr->Workm.t[0] + Sv.vx;
			pEm->Pos_y = pP_ptr->Workm.t[1] + Sv.vy;
			pEm->Pos_z = pP_ptr->Workm.t[2] + Sv.vz;
			break;

		case 1:
			break;

		case 2:
			pP_ptr = G.pEnemy[2]->pSin_parts_ptr + 2;
			Sv = *(SVECTOR *)Em37_pos_xyz[ EF->No ];
			Sv.vz /= 2;
			Sv.vy /= 2;
			Sv.vy -= 1300;
			if (EF->No == 1 || EF->No == 2)
				Sv.vy -= 200;

			ApplyMatrixSV(&pP_ptr->Workm, &Sv, &Sv);
			pEm->Pos_x = pP_ptr->Workm.t[0] + Sv.vx;
			pEm->Pos_y = pP_ptr->Workm.t[1] + Sv.vy;
			pEm->Pos_z = pP_ptr->Workm.t[2] + Sv.vz;
			break;
		}
	}

	Em37_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

	Em37_hari_quake(pEm);

	if (!(EF->etc_flg & 1)) {
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr[1].Size_x = pP_ptr->Size_x;
		pP_ptr[2].Size_x = pP_ptr->Size_x;
		pP_ptr[3].Size_x = pP_ptr->Size_x;
	}

	Em_seq_sound(pEm);	/* retail-only (Em137 +0x278, 0x801003F0) */

	Em37_ob_at(pEm);
	G.pEnemy[ EF->No + 7 ]->Be_flg |= 1;
	G.pEnemy[ EF->No + 7 ]->Be_flg &= ~0x100000;	/* retail-only (0x801002CC..0x80100320) */
	if (pEm->Be_flg & 0x100000)
		G.pEnemy[ EF->No + 7 ]->Be_flg |= 0x100000;
}

}

static void Em37_after(ENEMY_WORK *pEm)
{
if (pEm->pSin_parts_ptr->Be_flg & 1) {
	G5s_Jh_move(pEm);
}	}

static void Em37_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long i;
	MATRIX M;
	register GLOBAL *pG = &G;
	register ENEMY_WORK *pKem;

*(long *)&pEm->Routine_0 = 1;

pEm->Damage_flg = 0;
pEm->Sce_flg = 0;
pEm->Down_cnt = 0;
pEm->Damage_cnt = 0;

pEm->Life = -1;
pEm->D_life_u = -126;
pEm->D_life_c = -126;
pEm->D_life_d = -126;

pEm->Neck_flg = 0;
pEm->Neck_no = 3;

EF->Nemoto_no = 0;
EF->etc_flg = 0;
EF->Q_timer = 0;
EF->Retail_0C = 0;	/* retail-only (0x80100578) */
EF->Retail_0B = 0;

pEm->Be_flg |= 8;

pEm->nOba = 0;
pEm->Atd[0].Ofs_x = 0;
pEm->Atd[0].Ofs_y = -500;
pEm->Atd[0].Ofs_z = 0;
pEm->Atd[0].At_w = 500;
pEm->Atd[0].At_h = 500;
pEm->Atd[0].At_d = 500;
pEm->Atd[0].W = 500;
pEm->Atd[0].D = 500;

pEm->Atd[1].Ofs_x = 0;
pEm->Atd[1].Ofs_y = -500;
pEm->Atd[1].Ofs_z = 0;
pEm->Atd[1].At_w = 1200;
pEm->Atd[1].At_h = 500;
pEm->Atd[1].At_d = 1200;
pEm->Atd[1].W = 1200;
pEm->Atd[1].D = 1200;

pEm->Atd[2].Ofs_x = 0;
pEm->Atd[2].Ofs_y = -500;
pEm->Atd[2].Ofs_z = 0;
pEm->Atd[2].At_w = 600;
pEm->Atd[2].At_h = 500;
pEm->Atd[2].At_d = 600;
pEm->Atd[2].W = 600;
pEm->Atd[2].D = 600;

pEm->Atd[3].Ofs_x = 0;
pEm->Atd[3].Ofs_y = -500;
pEm->Atd[3].Ofs_z = 0;
pEm->Atd[3].At_w = 700;
pEm->Atd[3].At_h = 500;
pEm->Atd[3].At_d = 700;
pEm->Atd[3].W = 700;
pEm->Atd[3].D = 700;

Kage_work_set(&pEm->pKage_work, 0, 0, 0, pEm->pSin_parts_ptr->Workm.t);

i = 3;
while (pEm != pG->pEnemy[ i ]) {
	i++;
}
EF->No = i - 3;

if (pEm->Type == 8) {
	pEm->nOba = 4;
	pEm->Atd[0].At_w = 800;	/* retail: 800 (0x8010066C) */
	pEm->Atd[0].At_d = 800;
	pEm->Atd[0].W = 800;
	pEm->Atd[0].D = 800;

	pEm->Atd[1].At_w = 600;
	pEm->Atd[1].At_d = 600;
	pEm->Atd[1].W = 600;
	pEm->Atd[1].D = 600;

	pEm->Atd[2].At_w = 600;
	pEm->Atd[2].At_d = 600;
	pEm->Atd[2].W = 600;
	pEm->Atd[2].D = 600;

	pEm->Atd[3].At_w = 600;
	pEm->Atd[3].At_d = 600;
	pEm->Atd[3].W = 600;
	pEm->Atd[3].D = 600;

	return;
}

	pEm->Eff_at_r = 800;

*(long *)&pEm->Move_no = 0;

Joint_move(pEm, Kan, Seq, 0x100);

G.pPldamage_Em[ pEm->Id - 0x10 ] = PLDAMEGE_EM;
G.pPldie_Em[ pEm->Id - 0x10 ] = PLDIE_EM;

pEm->Be_flg |= 0xC;	/* retail: 0xC (0x80100728) */

pP_ptr = pEm->pSin_parts_ptr;
pP_ptr->Be_flg |= 0x4400;

pP_ptr->Size_x = 0;
pP_ptr->Size_y = 0x1000;
pP_ptr->Size_z = 0x1000;

pP_ptr[1].Be_flg |= 0x4800;

pP_ptr[1].Size_x = 0;
pP_ptr[1].Size_y = 0x1000;
pP_ptr[1].Size_z = 0x1000;

pP_ptr[2].Be_flg |= 0x4800;

pP_ptr[2].Size_x = 0;
pP_ptr[2].Size_y = 0x1000;
pP_ptr[2].Size_z = 0x1000;

pP_ptr[3].Be_flg |= 0x4800;

pP_ptr[3].Size_x = 0;
pP_ptr[3].Size_y = 0x1000;
pP_ptr[3].Size_z = 0x1000;

if (!pEm->Pos_y)
	G.pFree_work = (unsigned char *)G5s_Jh_init(pEm, (unsigned long)G.pFree_work);

pEm->pTafter_func = Em37_after;

if (!(pEm->Type & 0xFFF)) {	/* retail: only for the base tentacle type (0x801007C4) */
	Sv = *(SVECTOR *)Em37_pos_xyz[ i - 3 ];

	RotMatrix((SVECTOR *)&pG->pEnemy[2]->Cdir_x, &M);
	ApplyMatrixSV(&M, &Sv, &Sv);
	pEm->Pos_x = pG->pEnemy[2]->Pos_x + Sv.vx;
	pEm->Pos_y = pG->pEnemy[2]->Pos_y + Sv.vy;
	pEm->Pos_z = pG->pEnemy[2]->Pos_z + Sv.vz;
}

/* retail: the part 4 hook setup of Beta 2 is gone */

pEm->Routine_1 = 8;

pKem = EM37_Set_enemy_work(0x34, pEm);
pKem->Be_flg = 0;
pKem->Type = 8;

Em37_Init(pKem, Kan, Seq);

if ((pEm->Type & 0xFFF) == 1) {	/* retail-only (0x801008B0) */
	pEm->Routine_1 = 15;
	EF->Nemoto_no = 1;
}

}
static void Em37_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Em37_Move_type[ pEm->Type & 0x3F ](pEm, Kan, Seq); }

static void Em37_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Em37_mv_br_type00[ pEm->Routine_1 ](pEm);

Em37_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em37_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Em37_mv_br_type01[ pEm->Routine_1 ](pEm);

Em37_mv_type01[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em37_mv00_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0x3F0000;
	pEm->Routine_2 = 1;
	pEm->Neck_flg &= ~2;

	pEm->Timer0 = (Rnd() & 0x3F) + 60;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg &= ~1;
}

}

static void Em37_mv01_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr, *pTa;
	register long X, Z, L;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 9;
	pEm->Routine_2 = 1;
	pEm->Neck_flg &= ~2;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg |= 1;

	pTa = G.Player.pSin_parts_ptr;
	X = pTa->Workm.t[1] - pEm->Pos_y;
	Z = pTa->Workm.t[2] - pEm->Pos_z;
	X = SquareRoot0(X * X + Z * Z);
	L = pTa->Workm.t[0] - pEm->Pos_x;

	pEm->Cdir_z = catan((X << 12) / L);

	pTa = G.Player.pSin_parts_ptr;
	pEm->Cdir_x = Direction_ck(pEm->Pos_y, -pEm->Pos_z, pTa->Workm.t[1], -pTa->Workm.t[2]);

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x100;
	if (pP_ptr->Size_x > 0x800) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0xF0000;
	}

	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x100);

	pP_ptr->Size_x += 0x100;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;
		pEm->Timer0 = (Rnd() & 0xF) + 10;
	}
	break;

case 3:
	if ((unsigned short)pEm->Timer0-- == 0) {
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0xF0000;
	}
	break;

case 4:
	Joint_move(pEm, Kan, Seq, 0x7D);
	pP_ptr->Size_x -= 0x200;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;
	}
}
}
static void Em37_mv02_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr, *pTa;
	register long X;
	SVECTOR Sv0;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;
	pEm->Neck_flg &= ~2;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg |= 1;

	pTa = G.Player.pSin_parts_ptr;
	Goto00(pEm, pTa->Workm.t[0], pTa->Workm.t[2], 0x800);

	pEm->Cdir_z = -0x40;
	pEm->Timer1 = 0x28;
	pEm->Timer2 = 0;
	pEm->Timer0 = 0;
	pEm->Cdir_x = 0;

	pEm->Timer3 = 0;

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += pEm->Timer0;
	pEm->Timer0 += 2;
	if (pP_ptr->Size_x > 0xDDC) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0x3F0001;
	}

	if (pP_ptr->Size_x > 0x800 && !S8(pEm->Timer3)) {
		*(long *)&pEm->Move_no = 0x3F0009;
		pEm->Timer3 = 1;
	}

	if (!S8(pEm->Timer2)) {
		pEm->Cdir_z -= pEm->Timer1;
		pEm->Timer1 -= 1;
	}
	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += pEm->Timer0;

	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;
		pEm->Timer0 = (Rnd() & 0xF) + 10;
	}

	if (!S8(pEm->Timer2)) {
		pEm->Cdir_z -= pEm->Timer1;
		pEm->Timer1 -= 8;
	}

	Em37_trans(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 3;
	Sv0.vx = 1500;
	Sv0.vy = 0;
	Sv0.vz = 0;
	ApplyMatrixSV(&pP_ptr->Workm, &Sv0, &Sv0);
	Sv0.vy += pP_ptr->Workm.t[1];
	Sv0.vz += pP_ptr->Workm.t[2];

	if (Sv0.vy > -400 && !S8(pEm->Timer2)) {
		pEm->Routine_2 = 4;
		pEm->Timer0 = (Rnd() & 0xF) + 30;
		pEm->Timer2 = 1;
		pEm->Timer1 = 0x40;
	}

	if ((EF->etc_flg & 2) && !G.Player.Damage_cnt) {
		*(long *)&G.Player.Routine_0 = 0x202;
		G.Player.Damage_cnt |= 0x80;
		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(10, 250, 0);
	}

	if (Sv0.vz < -27000)
		pEm->Cdir_y -= 0x20;

	if (Sv0.vz > -19800)
		pEm->Cdir_y += 0x20;

	break;

case 3:
	if (!S8(pEm->Timer2)) {
		pEm->Cdir_z -= pEm->Timer1;
		pEm->Timer1 -= 8;
	}

	Em37_trans(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 3;
	Sv0.vx = 1500;
	Sv0.vy = 0;
	Sv0.vz = 0;
	ApplyMatrixSV(&pP_ptr->Workm, &Sv0, &Sv0);
	Sv0.vy += pP_ptr->Workm.t[1];
	Sv0.vz += pP_ptr->Workm.t[2];

	if (Sv0.vz < -27000)
		pEm->Cdir_y -= 0x20;

	if (Sv0.vz > -19800)
		pEm->Cdir_y += 0x20;

	if (Sv0.vy > -400 && !S8(pEm->Timer2)) {
		pEm->Timer2 = 1;
		pEm->Timer1 = 0x40;
		pEm->Timer0 = (Rnd() & 0xF) + 20;
		pEm->Routine_2 = 4;
	}

	if ((EF->etc_flg & 2) && !G.Player.Damage_cnt) {
		*(long *)&G.Player.Routine_0 = 0x202;
		G.Player.Damage_cnt |= 0x80;
		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(10, 250, 0);
	}

	break;

case 4:
	if ((unsigned short)pEm->Timer0-- == 0) {
		pEm->Routine_2 = 5;
		*(long *)&pEm->Move_no = 0x3F0000;
		pEm->Timer0 = 8;
	}

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Cdir_z += pEm->Timer1;
	pP_ptr[1].Cdir_z -= pEm->Timer1;
	pP_ptr[2].Cdir_z -= pEm->Timer1;
	pP_ptr[3].Cdir_z -= pEm->Timer1 >> 1;
	pEm->Timer1 -= (signed char)pEm->Timer2 * 2;

	Em37_move(pEm, pEm->pSin_parts_ptr);

	pP_ptr = pEm->pSin_parts_ptr;

	X = pEm->Cdir_z + pP_ptr->Cdir_z + pP_ptr[1].Cdir_z + pP_ptr[2].Cdir_z + pP_ptr[3].Cdir_z;

	X &= 0xFFF;
	if (X > 0x800 && X < 0x1000) {
		pP_ptr->Cdir_z -= pEm->Timer1;
		pP_ptr[1].Cdir_z += pEm->Timer1;
		pP_ptr[2].Cdir_z += pEm->Timer1;
		pP_ptr[3].Cdir_z += pEm->Timer1 >> 1;

		Em37_move(pEm, pEm->pSin_parts_ptr);

		pEm->Timer1 = 0;
		pEm->Timer2 = 0;

		pEm->Timer0 = 5;
	}

	break;

case 5:
	Joint_move(pEm, Kan, Seq, 0x40);
	pP_ptr->Size_x -= pEm->Timer0;
	pEm->Timer0 += 8;
	pEm->Cdir_z += 4;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;
	}
}

}

static void Em37_mv03_br(ENEMY_WORK *pEm)
{

}
static void Em37_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;
	pEm->Neck_flg &= ~2;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg |= 1;

	pEm->Cdir_z = -0x240;	/* retail: -0x240 (Em37_mv03 +0x88) */
	pEm->Cdir_y = 0;

	pEm->Cdir_x = Em37_mv03_dir_tbl[ EF->No ];

	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg |= 1 << (EF->No + 4);
	Snd_se_enem(0, pEm);	/* retail-only */

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0xC00) {	/* retail: 0xC00 */

		pEm->Routine_2 = 2;
		pEm->Timer3 = Rnd() & 1;

		*(long *)&pEm->Move_no = pEm->Timer3 + 0x1F0007;
	}

	if (pP_ptr->Size_x > 2024) {	/* retail-only swing-up (Em37_mv03 +0x100) */
		pEm->Cdir_z += 32;
		if (pEm->Cdir_z > 32)
			pEm->Cdir_z = 32;
	}

	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x80);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;
		pEm->Timer0 = (Rnd() & 0xF) + 10;
	}

	if (Em37_mv03_tbl[ (signed char)pEm->Timer3 ][0] < pEm->Move_cnt && pEm->Move_cnt < Em37_mv03_tbl[ (signed char)pEm->Timer3 ][1]) {

		pEm->Cdir_z += 8;
	}

	break;

case 3:
	if (Joint_move(pEm, Kan, Seq, 0x80)) {
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = 0x1F0000;
	}

	if (Em37_mv03_tbl[ (signed char)pEm->Timer3 ][0] < pEm->Move_cnt && pEm->Move_cnt < Em37_mv03_tbl[ (signed char)pEm->Timer3 ][1]) {

		pEm->Cdir_z += 8;
	}

	if (Em37_mv03_tbl[ (signed char)pEm->Timer3 ][2] < pEm->Move_cnt && pEm->Move_cnt < Em37_mv03_tbl[ (signed char)pEm->Timer3 ][3]) {

		pEm->Cdir_z += 4;

		if ((EF->etc_flg & 4) && !G.Player.Damage_cnt && G.pEnemy[2]->Life >= 0) {	/* retail: core alive */

			if (Pl_life_down(15, 0) == 2) {
				*(long *)&G.Player.Routine_0 = 3;
			}else {
/* inferred: the dead Kan parameter carries the Cdir_ck() result (no local of its own in the SYM) */
				Kan = Cdir_ck(pEm, &G.Player);
				*(long *)&G.Player.Routine_0 = ((~Kan & 1) << 8) + 5;

				if (pEm->L_pl > 6000) {
					*(long *)&G.Player.Routine_0 = 0x205;
					G.Player.Cdir_y = 0xD80;
					if (EF->No < 2)
						G.Player.Cdir_y = 0x280;
				}

			}

			G.Player.Damage_cnt |= 0x80;
			G.Player.pEnemy_ptr = (unsigned long *)pEm;
			G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
			G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
			Snd_se_enem(3, pEm);	/* retail-only */
			Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
			Pad_vib_set1(10, 250, 0);
		}
	}

	break;

case 4:
	Joint_move(pEm, Kan, Seq, 0x80);
	pP_ptr->Size_x -= 0x40;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;

		((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg &= ~(1 << (EF->No + 4));
	}
}

}

static void Em37_mv04_br(ENEMY_WORK *pEm)
{

}
static void Em37_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pTa;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0x3F0009 + ((Rnd() % 90) << 8) + (Rnd() & 3);
	pEm->Routine_2 = 1;

	pEm->Timer1 = (Rnd() & 0xF) + 60;
	pEm->Timer2 = 3;

case 1:
	pTa = G.Player.pSin_parts_ptr;
	Goto00_x(pEm, pTa->Workm.t[1], pTa->Workm.t[2], 8);

	Joint_move(pEm, Kan, Seq, 0x40);

	if ((unsigned short)pEm->Timer1-- == 0) {
		*(long *)&pEm->Move_no = 0x3F0009 + ((Rnd() % 90) << 8) + (Rnd() % 3);
		pEm->Timer0 = (Rnd() & 0xF) + 60;
	}
}

}

static void Em37_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr, *pTa;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;

	pP_ptr->Be_flg |= 1;

	pEm->Cdir_z = -0x20;
	pEm->Cdir_y = 0;
	pTa = G.Player.pSin_parts_ptr;
	pEm->Cdir_x = Direction_ck(pEm->Pos_y, -pEm->Pos_z, pTa->Workm.t[1], -pTa->Workm.t[2]);

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x800) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0x1F0009;
	}

	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x80);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		*(long *)&pEm->Routine_0 = 0x401;
	}
}

}

static void Em37_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0x1F0000;
	pEm->Routine_2 = 1;

case 1:
	Joint_move(pEm, Kan, Seq, 0x80);
	pP_ptr->Size_x -= 0x40;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;
	}
}
}

static void Em37_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pTa;

switch (pEm->Routine_2) {
case 0:
	pEm->Routine_2 = 1;
	*(long *)&pEm->Move_no = 0x1F0007;

case 1:
	pTa = G.Player.pSin_parts_ptr;
	Goto00_x(pEm, pTa->Workm.t[1] - 500, pTa->Workm.t[2], 0x40);

	if (Joint_move(pEm, Kan, Seq, 0x80))
		*(long *)&pEm->Routine_0 = 0x401;
}

}
static void Em37_mv08_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;

unsigned char Mot_tbl[4] = { 14, 14, 16, 15 };

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = Mot_tbl[ EF->No ];
	pP_ptr->Be_flg |= 1;

	if (EF->No == 1) {
		pP_ptr->Be_flg |= 0x4000;
		EF->Nemoto_no = 2;
	}

	pEm->Cdir_x = 0;
	pEm->Cdir_y = 0;
	pEm->Cdir_z = 0;
	pP_ptr->Size_x = 0;
	break;

case 1:
	pEm->Routine_2 = 2;
	EF->Nemoto_no = 1;

	Snd_se_enem(0, pEm);

case 2:
	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

	pP_ptr->Size_x += 0x100;
	if (pP_ptr->Size_x > 0x1000)
		pP_ptr->Size_x = 0x1000;

	break;

case 3:
	*(long *)&pEm->Move_no = 0x70000;
	pEm->Timer0 = 0;
	pEm->Routine_2 = 4;

	Snd_se_enem(2, pEm);

	EF->Q_timer = 7;

case 4:

	if (pEm->Timer0 < 5)
		pEm->Pos_x -= pEm->Timer0 * 10;

	if (pEm->Timer0 >= 6 && pEm->Timer0 < 35)
		pEm->Pos_x += 10;

	if (pEm->Timer0 == 36)
		pEm->Routine_2 = 5;

	pEm->Timer0++;

	break;

case 5:
	pEm->Timer0 = 0;
	break;

case 6:

	if (pEm->Timer0 < 20)
		pEm->Pos_x -= pEm->Timer0;

	if (pEm->Timer0 >= 16 && pEm->Timer0 < 180) {
		pEm->Pos_x += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_y += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_z += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
	}
	if (pEm->Timer0 > 160)
		pEm->Pos_x += 5;

	if (pEm->Timer0 == 180)
		pEm->Routine_2 = 7;

	pEm->Timer0++;

case 7:

	break;

case 8:
	Joint_move(pEm, Kan, Seq, 0x200);
	pP_ptr->Size_x -= 0x40;

	if (EF->No >= 2) {
		pEm->Cdir_y += 0x10;
	}else {
		pEm->Cdir_y -= 0x10;
	}
	if (pP_ptr->Size_x < 0x800) {
		pP_ptr->Size_x = 0;
		pEm->Routine_2 = 9;

		Snd_se_enem(0, pEm);
	}

case 9:
	break;

}

}
static void Em37_mv09_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register long X;
	SVECTOR Sv0;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Cdir_x = Em37_mv08_tbl[ EF->No ][3];
	pEm->Cdir_y = Em37_mv08_tbl[ EF->No ][4];
	pEm->Cdir_z = Em37_mv08_tbl[ EF->No ][5];
	pEm->Timer1 = 0x40;
	pEm->Timer2 = 0;
	pEm->Timer0 = 0;
	EF->Nemoto_no = 2;
	pEm->Routine_2 = 1;
	pP_ptr->Be_flg |= 1;
	pP_ptr->Size_x = 0;

	pEm->nOba = 4;

	Snd_se_enem(0, pEm);

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += pEm->Timer0;
	pEm->Timer0 += 8;
	if (pP_ptr->Size_x > 0xC00) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0x1F0002;
	}

	pEm->Cdir_z -= pEm->Timer1;
	pEm->Timer1 -= 4;
	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x80);

	pP_ptr->Size_x += pEm->Timer0;
	pEm->Timer0 += 0x10;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;
		Snd_se_enem(15, pEm);	/* retail-only (Em37_mv09 +0x1D0) */
	}

case 3:

	pEm->Cdir_z -= pEm->Timer1;

	if (pEm->Routine_2 == 3)
		pEm->Timer1 -= 4;

	Em37_trans(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 3;
	Sv0.vx = 1500;
	Sv0.vy = 0;
	Sv0.vz = 0;
	ApplyMatrixSV(&pP_ptr->Workm, &Sv0, &Sv0);
	Sv0.vy += pP_ptr->Workm.t[1];
	Sv0.vz += pP_ptr->Workm.t[2];

	if (Sv0.vy > 0 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vy < -5000 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vz < -27000 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vz > -19800 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	break;

case 4:
	pEm->Timer1 = 0x40;
	pEm->Timer0 = 60;
	pEm->Routine_2 = 5;
	pEm->Timer2 = 1;
	EF->Nemoto_no = 1;

	Snd_se_enem(2, pEm);

	EF->Q_timer = 7;

case 5:
	if ((unsigned short)pEm->Timer0-- == 0) {
		pEm->Routine_2 = 6;
		*(long *)&pEm->Move_no = 0x3F0000;
		pEm->Timer0 = 8;

		EF->Pos_x = pP_ptr[3].Workm.t[0] - pP_ptr->Workm.t[0];
		EF->Size_x = pP_ptr->Size_x;
		EF->Px = pP_ptr->Workm.t[0];
	}

	pP_ptr->Cdir_z += pEm->Timer1;
	pP_ptr[1].Cdir_z -= pEm->Timer1;
	pP_ptr[2].Cdir_z -= pEm->Timer1;
	pP_ptr[3].Cdir_z -= pEm->Timer1 >> 1;
	pEm->Timer1 -= (signed char)pEm->Timer2 * 2;

	if (!pEm->Timer1)
		pEm->Timer0 = 0;

	Em37_move(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr;

	X = pEm->Cdir_z + pP_ptr->Cdir_z + pP_ptr[1].Cdir_z + pP_ptr[2].Cdir_z + pP_ptr[3].Cdir_z;

	X &= 0xFFF;
	if (X > 0x800 && X < 0x1000) {
		pP_ptr->Cdir_z -= pEm->Timer1;
		pP_ptr[1].Cdir_z += pEm->Timer1;
		pP_ptr[2].Cdir_z += pEm->Timer1;
		pP_ptr[3].Cdir_z += pEm->Timer1 >> 1;

		Em37_move(pEm, pEm->pSin_parts_ptr);
		pEm->Timer1 = 0;
		pEm->Timer2 = 0;
	}

	break;

case 6:
	pEm->Timer0 = 0;
	break;

case 7:
	if (pEm->Timer0 < 10) {
		pP_ptr->Cdir_z -= pEm->Timer0 * 2;
		pP_ptr[1].Cdir_z += pEm->Timer0 * 2;
		pP_ptr[2].Cdir_z += pEm->Timer0 * 2;
		pP_ptr[3].Cdir_z += pEm->Timer0;
		Em37_move(pEm, pEm->pSin_parts_ptr);
	}

	if (pEm->Timer0 >= 8 && pEm->Timer0 < 175) {
		pEm->Pos_x += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_y += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_z += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
	}

	if (pEm->Timer0 > 165) {
		pP_ptr->Cdir_z += 4;
		pP_ptr[1].Cdir_z -= 4;
		pP_ptr[2].Cdir_z -= 4;
		pP_ptr[3].Cdir_z -= 2;
		Em37_move(pEm, pEm->pSin_parts_ptr);
	}
	if (pEm->Timer0 == 180)
		pEm->Routine_2 = 8;

	pEm->Timer0++;

case 8:

	break;

case 9:
	Joint_move(pEm, Kan, Seq, 0x40);
	pP_ptr->Size_x -= pEm->Timer0;
	pEm->Timer0 += 4;
	pEm->Cdir_z += 8;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		pEm->Routine_2 = 10;

		Snd_se_enem(0, pEm);
	}
}
}

static void Em37_mv0a_br(ENEMY_WORK *pEm)
{

}
static void Em37_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg |= 1;

	pEm->Cdir_x = Em37_mv08_tbl[ EF->No ][0];
	pEm->Cdir_y = Em37_mv08_tbl[ EF->No ][1];
	pEm->Cdir_z = Em37_mv08_tbl[ EF->No ][2];

	EF->Nemoto_no = 0;

	Snd_se_enem(0, pEm);

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x800) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = (EF->No % 3) + 0x1F0009;
	}

	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x80);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;
		pEm->Timer0 = (Rnd() & 0xF) + 30;
	}
	break;

case 3:
	Joint_move(pEm, Kan, Seq, 0x40);

	if ((unsigned short)pEm->Timer0-- == 0) {
		*(long *)&pEm->Move_no = 0x3F0009 + ((Rnd() % 90) << 8) + (Rnd() % 3);
		pEm->Timer0 = (Rnd() & 0xF) + 60;
	}
	break;

case 4:
	pEm->Routine_2 = 5;
	*(long *)&pEm->Move_no = (Rnd() & 1) + 0x1F0007;

case 5:
	/* retail: no Snd_se_enem( 4 ) on frame 10 (0x8010299C) */

	if (Joint_move(pEm, Kan, Seq, 0x80)) {
		pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = (Rnd() % 3) + 0x3F0009;
		pEm->Timer0 = (Rnd() & 0xF) + 30;
	}
}
}

static void Em37_mv0b_br(ENEMY_WORK *pEm)
{

}
static void Em37_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register long X, Ck;
	SVECTOR Sv0;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:

	*(long *)&pEm->Move_no = 0;
	Ck = (Rnd() & 3) << 2;

	pEm->Cdir_x = Em37_mv0b_tbl[ EF->No + Ck ][0];
	pEm->Cdir_y = Em37_mv0b_tbl[ EF->No + Ck ][1];
	pEm->Cdir_z = Em37_mv0b_tbl[ EF->No + Ck ][2];

	pEm->Timer1 = 0x40;
	pEm->Timer2 = 0;
	pEm->Timer0 = 0;

	EF->Nemoto_no = 0;
	pEm->Routine_2 = 1;
	pP_ptr->Be_flg |= 1;
	pP_ptr->Size_x = 0;

	Snd_se_enem(0, pEm);

	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg &= ~(1 << (EF->No + 4));

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += pEm->Timer0;
	pEm->Timer0 += 8;
	if (pP_ptr->Size_x > 0xC00) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0x1F0002;
	}

	pEm->Cdir_z -= pEm->Timer1;
	pEm->Timer1 -= 4;
	break;

case 2:
	Joint_move(pEm, Kan, Seq, 0x80);

	pP_ptr->Size_x += pEm->Timer0;
	pEm->Timer0 += 0x10;
	if (pP_ptr->Size_x > 0x1000) {
		pP_ptr->Size_x = 0x1000;
		pEm->Routine_2 = 3;

		Snd_se_enem(15, pEm);	/* retail: sound 15 (Em37_mv0b +0x1DC) */
	}

case 3:
	pEm->Cdir_z -= pEm->Timer1;

	if (pEm->Routine_2 == 3)
		pEm->Timer1 -= 4;

	Em37_trans(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr + 3;
	Sv0.vx = 1500;
	Sv0.vy = 0;
	Sv0.vz = 0;
	ApplyMatrixSV(&pP_ptr->Workm, &Sv0, &Sv0);
	Sv0.vy += pP_ptr->Workm.t[1];
	Sv0.vz += pP_ptr->Workm.t[2];

	if (Sv0.vy > 0 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vy < -5000 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vz < -27000 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if (Sv0.vz > -19800 && !S8(pEm->Timer2))
		pEm->Routine_2 = 4;

	if ((EF->etc_flg & 2) && !G.Player.Damage_cnt) {

/* inferred: the dead Kan parameter carries the Cdir_ck() result (no local of its own in the SYM) */
		Pl_life_down(15, 1);	/* retail-only (Em37_mv0b +0x340) */
		Kan = Cdir_ck(pEm, &G.Player);
		*(long *)&G.Player.Routine_0 = ((~Kan & 1) << 8) + 5;

		G.Player.Damage_cnt |= 0x80;
		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(5, 250, 0);
	}

	break;

case 4:
	pEm->Timer1 = 0x40;
	pEm->Timer0 = 60;
	pEm->Routine_2 = 5;
	pEm->Timer2 = 1;

	Snd_se_enem(2, pEm);

	EF->Q_timer = 7;
	EF->Retail_0B = 1;	/* retail-only: starts func_80105560's toggle state (Em37_mv0b +0x3F0) */

case 5:
	if ((unsigned short)pEm->Timer0-- == 0) {
		pEm->Routine_2 = 6;
		*(long *)&pEm->Move_no = 0x3F0000;
		pEm->Timer0 = 8;

		EF->Pos_x = pEm->Pos_x + 0x2000;
		EF->Size_x = pP_ptr->Size_x;
	}

	pP_ptr->Cdir_z += pEm->Timer1;
	pP_ptr[1].Cdir_z -= pEm->Timer1;
	pP_ptr[2].Cdir_z -= pEm->Timer1;
	pP_ptr[3].Cdir_z -= pEm->Timer1 >> 1;
	pEm->Timer1 -= (signed char)pEm->Timer2 * 2;

	if (!pEm->Timer1)
		pEm->Timer0 = 0;

	Em37_move(pEm, pEm->pSin_parts_ptr);
	pP_ptr = pEm->pSin_parts_ptr;

	X = pEm->Cdir_z + pP_ptr->Cdir_z + pP_ptr[1].Cdir_z + pP_ptr[2].Cdir_z + pP_ptr[3].Cdir_z;

	X &= 0xFFF;
	if (X > 0x800 && X < 0x1000) {
		pP_ptr->Cdir_z -= pEm->Timer1;
		pP_ptr[1].Cdir_z += pEm->Timer1;
		pP_ptr[2].Cdir_z += pEm->Timer1;
		pP_ptr[3].Cdir_z += pEm->Timer1 >> 1;

		Em37_move(pEm, pEm->pSin_parts_ptr);
		pEm->Timer1 = 0;
		pEm->Timer2 = 0;
	}

	break;

case 6:
	EF->etc_flg |= 1;

	pP_ptr->Be_flg &= ~0x400;
	pP_ptr->Be_flg |= 0x2000;
	pP_ptr[1].Be_flg &= ~0x800;

	X = EF->Pos_x - pEm->Pos_x;
	pP_ptr->Size_x = ((X << 8) / 0x2000 * EF->Size_x) >> 8;

	pEm->Timer0 = 0;
	break;

case 7:
	if (pEm->Timer0 < 10) {
		pP_ptr->Cdir_z -= pEm->Timer0 * 2;
		pP_ptr[1].Cdir_z += pEm->Timer0 * 2;
		pP_ptr[2].Cdir_z += pEm->Timer0 * 2;
		pP_ptr[3].Cdir_z += pEm->Timer0;
		Em37_move(pEm, pEm->pSin_parts_ptr);
	}

	if (pEm->Timer0 >= 6 && pEm->Timer0 < 80) {
		pEm->Pos_x += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_y += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
		pEm->Pos_z += (Rnd() & 0xF) * (1 - (pEm->Timer0 & 1) * 2);
	}

	if (pEm->Timer0 > 80) {
		pP_ptr->Cdir_z += 4;
		pP_ptr[1].Cdir_z -= 4;
		pP_ptr[2].Cdir_z -= 4;
		pP_ptr[3].Cdir_z -= 2;
		Em37_move(pEm, pEm->pSin_parts_ptr);
	}
	if (pEm->Timer0 == 90)
		pEm->Routine_2 = 8;

	X = EF->Pos_x - pEm->Pos_x;
	pP_ptr->Size_x = ((X << 8) / 0x2000 * EF->Size_x) >> 8;

	pEm->Timer0++;

case 8:
	pEm->Timer2 = 0;
	*(long *)&pEm->Move_no = 0x3F0000;

	X = EF->Pos_x - pEm->Pos_x;
	pP_ptr->Size_x = ((X << 8) / 0x2000 * EF->Size_x) >> 8;

	break;

case 9:
	pEm->Cdir_z--;
	pP_ptr->Size_x += 0x80;
	if (pP_ptr->Size_x > EF->Size_x) {
		pP_ptr->Size_x = EF->Size_x;

		pP_ptr->Be_flg |= 0x400;
		pP_ptr->Be_flg &= ~0x2000;
		pP_ptr[1].Be_flg |= 0x800;
		EF->etc_flg &= ~1;

		pEm->Routine_2 = 10;

		Snd_se_enem(0, pEm);
	}

	Joint_move(pEm, Kan, Seq, 0x40);
	break;

case 10:
	Joint_move(pEm, Kan, Seq, 0x40);
	pP_ptr->Size_x -= pEm->Timer0;
	pEm->Timer0++;
	pEm->Cdir_z--;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;
	}

}
}

static void Em37_mv0c_br(ENEMY_WORK *pEm)
{

}
static void Em37_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr->Be_flg |= 1;

	pEm->Cdir_z = 0x40;
	pEm->Cdir_y = 0;

	pEm->Cdir_x = Em37_mv03_dir_tbl[ EF->No ];
	pEm->Cdir_x -= 0x400;

	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg |= 1 << (EF->No + 4);

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x1000) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = 0x1F0016;
		pP_ptr->Size_x = 0x1000;
	}

	break;

case 2:
	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
	pEm->Cdir_z += 8;
	break;

case 3:
	pEm->Routine_2 = 4;
	*(long *)&pEm->Move_no = 0x3F0000;

case 4:
	Joint_move(pEm, Kan, Seq, 0x40);
	pP_ptr->Size_x -= 0x80;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;

		((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg &= ~(1 << (EF->No + 4));
	}

	pEm->Cdir_z -= 4;
}

}

static void Em37_mv0d_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	/* retail rewrite (0x801034EC, 269 words): the tentacle tracks the player while it rises and
	   grabs (Routine 0x305) or re-grabs a held player (0x405) on frames 26..32 */
pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	pP_ptr = pEm->pSin_parts_ptr;
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;
	pP_ptr->Be_flg |= 1;

	pEm->Cdir_z = Em37_mv0d_tbl[ EF->No ][1];
	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
	pEm->Cdir_x = 0;

	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg |= 1 << (EF->No + 4);
	Snd_se_enem(0, pEm);
	pEm->Be_flg |= 0x100000;

case 1:
	Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0xDF4) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = Em37_mv0d_tbl[ EF->No ][0] + 0x1F0000;
		pEm->Timer1 = 0;
	}

	pEm->Cdir_z -= 8;
	break;

case 2:
	pP_ptr->Size_x += 0x40;
	if (pP_ptr->Size_x > 0x1000) {
		pEm->Routine_2 = 3;
		pP_ptr->Size_x = 0x1000;
	}
	pEm->Timer3 = 0;

case 3:
	if (pEm->Move_cnt > 20) {
		pEm->Cdir_z += pEm->Timer1;
		pEm->Timer1 += 8;
		if (pEm->Cdir_z > 0)
			pEm->Cdir_z = 0;
	}

	if (pEm->Move_cnt > 25 && pEm->Move_cnt < 33) {
		if (EF->etc_flg & 4) {
			if (!G.Player.Damage_cnt) {
				Pl_life_down(15, 1);
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				*(long *)&G.Player.Routine_0 = 0x305;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
				Snd_se_enem(3, pEm);
				pEm->Timer3 = 1;
				Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
				Pad_vib_set1(10, 250, 0);
			}

			if ((EF->etc_flg & 4) && G.Player.Damage_cnt && !S8(pEm->Timer3) && G.Player.Life >= 0) {
				Pl_life_down(10, 1);
				G.Player.pEnemy_ptr = (unsigned long *)pEm;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				*(long *)&G.Player.Routine_0 = 0x405;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
				Snd_se_enem(3, pEm);
				pEm->Timer3 = 1;
				Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
				Pad_vib_set1(10, 250, 0);
			}
		}
	}

	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
	break;

case 4:
	pEm->Routine_2 = 5;
	*(long *)&pEm->Move_no = 0x3F0000;

case 5:
	Joint_move(pEm, Kan, Seq, 0x40);
	pP_ptr->Size_x -= 0x80;
	if (pP_ptr->Size_x < 0) {
		pP_ptr->Size_x = 0;
		*(long *)&pEm->Routine_0 = 1;

		((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg &= ~(1 << (EF->No + 4));
		pEm->Be_flg &= ~0x100000;
	}
}

}

static void Em37_mv0e_br(ENEMY_WORK *pEm)
{

}

static void Em37_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;

	/* retail rewrite (0x80103928, 457 words): rise with spray effects, hold, drop and bounce
	   on the floor (vibration), then darken and shrink away (states 0..7) */
pP_ptr = pEm->pSin_parts_ptr;

switch (pEm->Routine_2) {
case 0:
	pP_ptr = pEm->pSin_parts_ptr;
	*(long *)&pEm->Move_no = 0;
	pEm->Routine_2 = 1;
	pP_ptr->Be_flg |= 1;

	pEm->Cdir_x = pEm->Cdir_y = pEm->Cdir_z = 0;

	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Syoku_flg |= 1 << (EF->No + 4);

	pEm->Be_flg |= 2;
	G.pEnemy[ EF->No + 7 ]->Be_flg |= 2;

case 1:
	Joint_move(pEm, Kan, Seq, 0x40);

	pP_ptr->Size_x += 0x100;
	if (pP_ptr->Size_x > 0x800) {
		pEm->Routine_2 = 2;
		*(long *)&pEm->Move_no = (Rnd() & 3) + 0x1F0011;
	}

	if (!(pEm->Move_cnt & 1)) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();
		Esp_call((((Rnd() & 1) + 0x10) << 16) | ((Rnd() << 3) + 0x2770), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	}
	break;

case 2:
	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x80);
	pP_ptr->Size_x += 0x100;

	if (pP_ptr->Size_x > 0x1000) {
		pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0x1F0011;
		EF->Nemoto_no = 1;
		pEm->Timer0 = (Rnd() & 0x1F) + 100;
		pEm->Timer1 = 0;
		pEm->Timer2 = 3;
	}

	if (!(pEm->Move_cnt & 3)) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();
		Esp_call((((Rnd() & 1) + 0x10) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	}
	break;

case 3:
	if (Joint_move(pEm, Kan, Seq, 0x80))
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0x1F0011;

	if ((unsigned short)pEm->Timer0-- == 0) {
		pEm->Routine_2 = 4;
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0x3F0013;
		pEm->Timer2 = 3;
	}

	if (!(pEm->Move_cnt & 7)) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	}
	break;

case 4:
	Joint_move(pEm, Kan, Seq, 0x40);
	pEm->Pos_y += pEm->Timer0;
	if ((pEm->Timer0 += 2) > 60) {
		pEm->Timer0 += 30;
	}else {
		if (!(pEm->Move_cnt & 1))
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, 0);

		if (!(pEm->Move_cnt & 7)) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();
			Esp_call((((Rnd() & 1) + 0x10) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
		}
	}

	if (pEm->Pos_y > -50) {
		pEm->Pos_y -= pEm->Timer0;
		pEm->Timer0 = (short)(pEm->Timer0 * -1) >> 4;

		if (!pEm->Timer2--) {
			pEm->Routine_2 = 5;
			pEm->Timer0 = 150;
		}
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, 0);
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr[1].Workm, 0);
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr[2].Workm, 0);
		Snd_se_enem(1, pEm);
		Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(10, 250, 0);
	}
	break;

case 5:
	if (Joint_move(pEm, Kan, Seq, 0x40))
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0x3F0013;

	if ((unsigned short)pEm->Timer0-- == 0) {
		*(long *)&pEm->Move_no = ((Rnd() + Rnd()) % 3) + 0x7F0009;
		pEm->Routine_2 = 6;
	}

	if (!(pEm->Timer0 & 3))
		pP_ptr->Poly_rgb -= 0x10101;

	if (!(pEm->Move_cnt & 1)) {
		Sv0.vx = 0x80 - Rnd();
		Sv0.vy = 0x80 - Rnd();
		Sv0.vz = 0x80 - Rnd();
		Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1FA0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
	}
	break;

case 6:
	Joint_move(pEm, Kan, Seq, 0x20);
	pP_ptr->Size_x -= 6;
	pP_ptr->Size_y -= 0x20;
	pP_ptr->Size_z += 0x10;
	if ((pP_ptr->Poly_rgb & 0xFF) != 0x20)
		pP_ptr->Poly_rgb--;
	if (pP_ptr->Poly_rgb & 0xFF00)
		pP_ptr->Poly_rgb -= 0x100;
	if ((pP_ptr->Poly_rgb & 0xFF0000) != 0x300000)
		pP_ptr->Poly_rgb -= 0x10000;
	if (pEm->Hokan_flg == 0x40)
		pEm->Routine_2 = 7;
	break;

case 7:
	break;
}
}

/* retail-only (no Beta twin, name unknown): Em37_mv_br_type00[15] / Em37_mv_type00[15]
 * (retail .data 0x801056EC / 0x8010572C), an empty branch check and a hidden-wait move that
 * shows the tentacle once Sce_free0 bit 0 is set. Retail 0x8010404C, 0x80104054. */
static void func_8010404C(ENEMY_WORK *pEm)
{

}

static void func_80104054(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 13;
		pEm->Be_flg |= 2;
		pEm->Cdir_x = pEm->Cdir_y = pEm->Cdir_z = 0;
		G.pEnemy[ EF->No + 7 ]->Be_flg |= 2;
		pP_ptr->Size_x = 0x1000;
		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;
		break;

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x40);
		break;

	case 2:
		*(long *)&pEm->Routine_0 = 0x801;
		EF->Nemoto_no = 0;
		pP_ptr->Size_x = 0;
		pEm->Be_flg &= ~2;
		G.pEnemy[ EF->No + 7 ]->Be_flg &= ~2;
	}
}

static void Em37_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Em37_dm[ pEm->Routine_1 - 1 ][ pEm->Damage_no ](pEm, Kan, Seq); }

static void Em37_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em37_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em37_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	extern void (*Em37_die[][9])();	/* [INFERRED] block-scope declaration of the table called through in this block: retail's record-less body block */

	Em37_die[ pEm->Routine_1 - 1 ][ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em37_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em37_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Em37_sce[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em37_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em37_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Pldamage_Em37_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }
static void Em37_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	MATRIX M;
	SVECTOR Sv0;

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30003;
	pEm->Routine_2 = 1;
	pEm->Damage_cnt |= 0x80;

	Snd_se_stad(((pEm->Routine_1 & 1) + 1) << 16 | 0x4000001, &pEm->Pos_x);

	pEm->Neck_flg |= 0x12;

	Sv0.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.pEnemy[2]->Pos_x, G.pEnemy[2]->Pos_z);

	Sv0.vx = 0;
	Sv0.vz = 0;
	RotMatrix(&Sv0, &M);
	Sv0.vx = 800;
	Sv0.vy = 0;
	ApplyMatrixSV(&M, &Sv0, (SVECTOR *)&pEm->Spd_x);

case 1:
	pEm->Routine_2 += Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);

	pEm->Pos_x += pEm->Spd_x;
	pEm->Pos_z += pEm->Spd_z;

	pEm->Spd_x >>= 1;
	pEm->Spd_z >>= 1;

	break;

case 2:
	*(long *)&pEm->Routine_0 = 1;
	pEm->Damage_cnt = 0;

	pEm->Neck_flg &= ~0x12;
}
}

static void Em37_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

switch (pEm->Routine_2) {
case 0:
	*(long *)&pEm->Move_no = 0x30602;
	pEm->Routine_2 = 1;

	pEm->Spd_x = 800;
	pEm->Spd_z = 0;

	G.Status_flg |= 0x40;
	pEm->Damage_cnt |= 0x80;
	pEm->Neck_flg = 1;

	pEm->Be_flg |= 2;

	Snd_se_stad(0x4010001, &pEm->Pos_x);
	pEm->Timer2 = 5;	/* retail-only (0x80104504) */

case 1:
	pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);

case 2:
	pEm->Pos_x -= 100;
	if (S8(pEm->Timer2))	/* retail-only delay before the wall hit (0x80104538) */
		S8(pEm->Timer2)--;

	if ((pEm->Sca_info & 1) && !S8(pEm->Timer2)) {
		pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = 0x30000;
		pEm->Spd_x = 0;
		Snd_se_stad(0x4020001, &pEm->Pos_x);
		Snd_se_stad(0x2290001, &pEm->Pos_x);	/* retail-only */
		Pad_vib_set0(3, 0);		/* retail-only Dual Shock vibration */
		Pad_vib_set1(6, 250, 0);
	}
	break;

case 3:
	if (Joint_move(pEm, Kan, Seq, 0x400)) {
		*(long *)&pEm->Move_no = 0x30001;
		pEm->Routine_2 = 4;
	}
	break;

case 4:
	if (Joint_move(pEm, Kan, Seq, 0x400)) {
		*(long *)&pEm->Move_no = 0x70006;
		pEm->Routine_2 = 5;

		pEm->Be_flg &= ~2;

		pEm->Damage_cnt &= 0x7F;
		pEm->Neck_flg = 0;

		G.Status_flg &= ~0x40;
	}
	break;

case 5:
	if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10200))

		*(long *)&pEm->Routine_0 = 1;

}

Add_speedXZ(pEm, 0x800);
}
/* retail-only (no Beta twin, name unknown): Pldamage_Em37_tbl[3] (retail .data 0x80105838); the player is
 * knocked away from the core (G.pEnemy[2]) with a 600-unit push, then a stand-up sequence like
 * Em37_pldm01. Retail 0x801046AC / 0x801116AC. */
static void func_801046AC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	MATRIX M;
	SVECTOR Sv;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70704;
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(0x4020001, &pEm->Pos_x);
		pEm->Neck_flg |= 0x12;

		Sv.vy = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)G.pEnemy[2]->Pos_x, (short)G.pEnemy[2]->Pos_z);
		Sv.vx = 0;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 600;
		Sv.vy = 0;
		ApplyMatrixSV(&M, &Sv, (SVECTOR *)&pEm->Spd_x);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Move_no = 0x30003;
			pEm->Routine_2 = 2;
			pEm->Timer0 = 15;
		}
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Spd_x >>= 1;
		pEm->Spd_z >>= 1;
		break;

	case 2:
		Joint_move(pEm, Kan, Seq, 0x400);
		if ((unsigned short)pEm->Timer0-- == 0) {
			*(long *)&pEm->Move_no = 0x30001;
			pEm->Routine_2 = 3;
		}
		break;

	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Move_no = 0x70006;
			pEm->Routine_2 = 4;

			pEm->Be_flg &= ~2;

			pEm->Damage_cnt &= 0x7F;
			pEm->Neck_flg = 0;

			G.Status_flg &= ~0x40;
		}
		break;

	case 4:
		if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10200))
			*(long *)&pEm->Routine_0 = 1;
	}
}

/* retail-only (no Beta twin, name unknown): Pldamage_Em37_tbl[4] (retail .data 0x8010583C); the player is
 * knocked away from the core (G.pEnemy[2]) with a 600-unit push, then a stand-up sequence like
 * Em37_pldm01. Retail 0x801048F4 / 0x801118F4. */
static void func_801048F4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	MATRIX M;
	SVECTOR Sv;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70005;
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;
		Snd_se_stad(0x4020001, &pEm->Pos_x);
		pEm->Neck_flg |= 0x12;

		Sv.vy = Direction_ck((short)pEm->Pos_x, (short)pEm->Pos_z, (short)G.pEnemy[2]->Pos_x, (short)G.pEnemy[2]->Pos_z);
		Sv.vx = 0;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 600;
		Sv.vy = 0;
		ApplyMatrixSV(&M, &Sv, (SVECTOR *)&pEm->Spd_x);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(long *)&pEm->Move_no = 0x30003;
			pEm->Routine_2 = 2;
			pEm->Timer0 = 15;
		}
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Spd_x >>= 1;
		pEm->Spd_z >>= 1;
		if (pEm->Move_cnt == 3 || pEm->Move_cnt == 11)
			Snd_se_walk(2, 0, pEm);
		break;

	case 2:
		Joint_move(pEm, Kan, Seq, 0x400);
		if ((unsigned short)pEm->Timer0-- == 0) {
			*(long *)&pEm->Move_no = 0x30001;
			pEm->Routine_2 = 3;
		}
		break;

	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Move_no = 0x70006;
			pEm->Routine_2 = 4;

			pEm->Be_flg &= ~2;

			pEm->Damage_cnt &= 0x7F;
			pEm->Neck_flg = 0;

			G.Status_flg &= ~0x40;
		}
		break;

	case 4:
		if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10200))
			*(long *)&pEm->Routine_0 = 1;
	}
}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

Pldie_Em37_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em37_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
CompM(pEm->M, pP_ptr->M, &pP_ptr->Workm);
CompM(&pP_ptr->Workm, pP_ptr[1].M, &pP_ptr[1].Workm);
CompM(&pP_ptr[1].Workm, pP_ptr[2].M, &pP_ptr[2].Workm);
CompM(&pP_ptr[2].Workm, pP_ptr[3].M, &pP_ptr[3].Workm);
}

static void Em37_move(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
RotMatrix((SVECTOR *)&pP_ptr[1].Cdir_x, (MATRIX *)pP_ptr[1].M);
RotMatrix((SVECTOR *)&pP_ptr[2].Cdir_x, (MATRIX *)pP_ptr[2].M);
RotMatrix((SVECTOR *)&pP_ptr[3].Cdir_x, (MATRIX *)pP_ptr[3].M); }

static unsigned long G5s_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top)
{

Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, G5s_jh0_head, Mem_top);
Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, G5s_jh1_head, Mem_top);
return Jh_set(pEm, pEm->pSin_parts_ptr, G5s_jh2_head, Mem_top); }

static void G5s_Jh_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

pP_ptr = pEm->pSin_parts_ptr;
MulJh(pEm, pP_ptr, G5s_jh0_head);
MulJh(pEm, pP_ptr, G5s_jh1_head);
MulJh(pEm, pP_ptr, G5s_jh2_head);
Jh_sort(pEm, pP_ptr);

}

static void Goto00_x(ENEMY_WORK *pEm, long Vec_y, long Vec_z, short Add_dir)
{
	register short Dir;
	unsigned short Sa;	/* inferred name (as EM_SUB.C Goto00): no retail SYM record,
				   combine folds the narrowed copy into the 0xfff mask */

	Dir = Direction_ck(pEm->Pos_y, -pEm->Pos_z, Vec_y, -Vec_z);

Sa = Add_dir + (Dir - pEm->Cdir_x);
if ((Sa & 0xfff) < Add_dir * 2) {
	pEm->Cdir_x = Dir;
}else {

	pEm->Cdir_x -= Add_dir;
	if ((Sa & 0xfff) < 0x801)
		pEm->Cdir_x += Add_dir << 1;
}

}

static void Em37_ob_at(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP;	/* inferred name; no SYM record: CSE folds base + 1 away */

pP = pEm->pSin_parts_ptr + 1;
pEm->Atd[0].X = pP->Workm.t[0];
pEm->Atd[0].Y = pP->Workm.t[1];
pEm->Atd[0].Z = pP->Workm.t[2];
	pP--;
pEm->Atd[1].X = pP->Workm.t[0];
pEm->Atd[1].Y = pP->Workm.t[1];
pEm->Atd[1].Z = pP->Workm.t[2];

Sv.vx = pEm->Atd[1].X - pEm->Atd[0].X;
Sv.vy = pEm->Atd[1].Y - pEm->Atd[0].Y;
Sv.vz = pEm->Atd[1].Z - pEm->Atd[0].Z;

pEm->Atd[2].X = pEm->Atd[0].X + (Sv.vx >> 2);
pEm->Atd[2].Y = pEm->Atd[0].Y + (Sv.vy >> 2);
pEm->Atd[2].Z = pEm->Atd[0].Z + (Sv.vz >> 2);

pEm->Atd[3].X = pEm->Atd[0].X + (Sv.vx >> 1);
pEm->Atd[3].Y = pEm->Atd[0].Y + (Sv.vy >> 1);
pEm->Atd[3].Z = pEm->Atd[0].Z + (Sv.vz >> 1);

pEm->At_em_no = 0xFF;

EF->etc_flg &= ~6;

EF->etc_flg |= Oba_ck_hit((ENEMY_WORK *)&G.Player, pEm) << 1;

}

static void Em37_ob_at1(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register ENEMY_WORK *pO;
	register PARTS_W *pP;	/* inferred name; no SYM record: CSE folds base + 3 away */

	pO = G.pEnemy[ EF->No - 1 ];

pP = pO->pSin_parts_ptr + 3;
pEm->Atd[0].X = pP->Workm.t[0];
pEm->Atd[0].Y = pP->Workm.t[1];
pEm->Atd[0].Z = pP->Workm.t[2];

	pP--;
pEm->Atd[1].X = pP->Workm.t[0];
pEm->Atd[1].Y = pP->Workm.t[1];
pEm->Atd[1].Z = pP->Workm.t[2];
	pP--;
pEm->Atd[2].X = pP->Workm.t[0];
pEm->Atd[2].Y = pP->Workm.t[1];
pEm->Atd[2].Z = pP->Workm.t[2];

Sv.vx = pEm->Atd[1].X - pEm->Atd[2].X;
Sv.vy = pEm->Atd[1].Y - pEm->Atd[2].Y;
Sv.vz = pEm->Atd[1].Z - pEm->Atd[2].Z;

pEm->Atd[3].X = pEm->Atd[2].X + (Sv.vx >> 1);
pEm->Atd[3].Y = pEm->Atd[2].Y + (Sv.vy >> 1);
pEm->Atd[3].Z = pEm->Atd[2].Z + (Sv.vz >> 1);

pEm->At_em_no = 0xFF;

((EM37_FREE *)ENEMY_FREE(pO))->etc_flg &= ~6;

((EM37_FREE *)ENEMY_FREE(pO))->etc_flg |= Oba_ck_hit(pEm, (ENEMY_WORK *)&G.Player) * 6;

}

static ENEMY_WORK *EM37_Set_enemy_work(unsigned char Id, ENEMY_WORK *pMoto)
{
register long i, Flg = 0;
	register ENEMY_WORK *pEm, **ppEm, **pTmp;
register GLOBAL *pG = &G;
	register unsigned long Mem_top;

i = 0x20;

ppEm = &pG->pEnemy[2];
do {
	if (!((*ppEm)->Be_flg & 1)) break;

	ppEm++;

}while (--i);

if (ppEm >= pG->ppEnemy_end) {
	pTmp = pG->ppEnemy_end;
	pG->ppEnemy_end = ppEm + 1;
	Flg = 1;
}

Mem_top = (unsigned long)pG->pFree_work;

*ppEm = (ENEMY_WORK *)Mem_top;
pEm = (ENEMY_WORK *)Mem_top;
Mem_top += sizeof(ENEMY_WORK);

if (Mem_top > 0x8018FFFF) {
	pEm->Be_flg = 0;
	if (Flg)
		pG->ppEnemy_end = pTmp;

	return 0;
}

pG->Enemy_num++;

pEm->pSa_dat = pMoto->pSa_dat;
pEm->pSeq_t_ptr = pMoto->pSeq_t_ptr;
pEm->pKan_t_ptr = pMoto->pKan_t_ptr;
pEm->pSub0_seq_t_ptr = pMoto->pSub0_seq_t_ptr;
pEm->pSub0_kan_t_ptr = pMoto->pSub0_kan_t_ptr;
pEm->pSub1_seq_t_ptr = pMoto->pSub1_seq_t_ptr;
pEm->pSub1_kan_t_ptr = pMoto->pSub1_kan_t_ptr;
pEm->pRoom_seq_t_ptr = pMoto->pRoom_seq_t_ptr;
pEm->pRoom_kan_t_ptr = pMoto->pRoom_kan_t_ptr;
pEm->pTmd = pMoto->pTmd;
pEm->Poly_rgb = 0x808080;
pEm->Model_type = pMoto->Model_type;
pEm->Clut = pMoto->Clut;
pEm->Tpage = pMoto->Tpage;
pEm->pTmd2 = pEm->pTmd + 7;
pEm->Id = pMoto->Id;
pEm->Type = pMoto->Type;
pEm->nFloor = pMoto->nFloor;
pEm->Ground = pMoto->Ground;
pEm->pTbefore_func = pMoto->pTbefore_func;
pEm->pTafter_func = pMoto->pTafter_func;
pEm->Sc_id = pMoto->Sc_id;
pEm->Atd[0] = pMoto->Atd[0];
pEm->Atd[1] = pMoto->Atd[1];
pEm->Atd[2] = pMoto->Atd[2];
pEm->Super_matrix = pMoto->Super_matrix;
pEm->Super_vector = pMoto->Super_vector;
pEm->pSuper = pMoto->pSuper;
pEm->Old_x = pMoto->Old_x;
pEm->Old_y = pMoto->Old_y;
pEm->Old_z = pMoto->Old_z;
pEm->Size = pMoto->Size;
pEm->Em_set_flg = pMoto->Em_set_flg;
pEm->nOba = pMoto->nOba;

pEm->Pos_x = pMoto->Pos_x;
pEm->Pos_y = pMoto->Pos_y;
pEm->Pos_z = pMoto->Pos_z;
pEm->Cdir_x = 0;
pEm->Cdir_y = 0;
pEm->Cdir_z = 0;
*(long *)&pEm->M[0][0] = 0x1000;
*(long *)&pEm->M[0][2] = 0;
*(long *)&pEm->M[1][1] = 0x1000;
*(long *)&pEm->M[2][0] = 0;
*(long *)&pEm->M[2][2] = 0x1000;
pEm->Be_flg = 1;
pEm->Work_no = i;
*(long *)&pEm->Routine_0 = 0;
pEm->Damage_flg = 0;
pEm->Damage_cnt = 0;
pEm->Status_flg = 0;
pEm->In_screen = 0;
pEm->Sce_flg = 0;
pEm->Spl_flg = 0;
pEm->Parts0_pos_y = 0x1000;
pEm->Sce_free0 = 0;
pEm->Sce_free1 = 0;
pEm->Sce_free2 = 0;
pEm->Sce_free3 = 0;
pEm->Timer0 = 0;
pEm->Timer1 = 0;
pEm->Root_ck_cnt = pG->Enemy_num & 7;

pEm->pTbefore_func = 0;
pEm->pTafter_func = 0;
pEm->pOn_om = 0;

Mem_top = PartsWork_set(pEm, Mem_top);

pG->pFree_work = (unsigned char *)Mem_top;

return pEm;
}

static void Em37_hari_quake(ENEMY_WORK *pEm)
{

((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Q0_t &= ~(2 << EF->No);

if (EF->Q_timer) {
	EF->Q_timer--;
	((EM36_FREE *)ENEMY_FREE(G.pEnemy[2]))->Q0_t |= 2 << EF->No;
	Pad_vib_set1(3, 130, 0);		/* retail-only Dual Shock vibration (0x80105534) */
	Pad_vib_fade_set(25, 130, 0, 3);
}	}

/* retail-only (no Beta twin, name unknown), never referenced in retail (retail 0x80105560 /
 * 0x80112560): a three-step toggle of part 4's Be_flg bits 0x40/0x4000 driven by
 * EM37_FREE Retail_0B (state) and Retail_0C (150-frame timer). */
static void func_80105560(ENEMY_WORK *pEm)
{
	switch (EF->Retail_0B) {
	case 1:
		pEm->pSin_parts_ptr[4].Be_flg = (pEm->pSin_parts_ptr[4].Be_flg & ~0x4000) | 0x40;
		EF->Retail_0B = 2;
		EF->Retail_0C = 150;

	case 2:
		if ((short)EF->Retail_0C-- == 0)
			EF->Retail_0B = 3;
		break;

	case 3:
		pEm->pSin_parts_ptr[4].Be_flg = (pEm->pSin_parts_ptr[4].Be_flg | 0x4000) & ~0x40;
		EF->Retail_0B = 0;
		break;
	case 0:
		break;
	}
}
