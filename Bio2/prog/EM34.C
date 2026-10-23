/* em34.c - reconstruction of D:\Bio2\prog\EM34.C (enemy 34).
 * Linked into two overlays from the same source: 0x18 (at 0x8010012c) and 0x36
 * (at 0x8010d12c); only the enemy-number-specific entry names differ
 * (Em134/Em234, Pldamege_Em134/234, Pldie_Em134/234), selected by OVL_36.
 * Types come from the TU's retail SYM preamble (em34.h).
 * The function-pointer tables carry no retail SYM records; their names are
 * inferred (project symbol list) and they are placed by address order. */
#include "EM34.H"

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;
extern ATTACK_WEAPON_EM Em34_damage_tbl0[19];	/* PL_WEP.H: ATTACK_WEAPON_EM[19] */
extern ATTACK_WEAPON_EM Em34_damage_tbl1[19];	/* PL_WEP.H: ATTACK_WEAPON_EM[19] */
extern unsigned char *pEnemy_damage_tbl[];	/* PL_WEP.C: ATTACK_WEAPON_EM *[48] */

/* ---- functions of this translation unit referenced by its tables ---- */

/* ---- functions of other translation units ---- */
/* LIBGTE.H prototypes (no implicit-declaration blocks at their calls) */
/* LIBGTE.H prototype (retail: no implicit-declaration block at its call) */
extern long Ground_in_flg, Ground_in_pos;

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
static SVECTOR Em34_T_PL_POS = { 0, 0, 0, 0 };
static short Em34_pl_dir = 0;
/* @80109322..80109324: 2 unrecorded bytes (padding/unknown): 0000 */
static short Em34_hosei_pos[9][2] = {
	{ 0, 0 },
	{ 700, 700 },
	{ 700, -700 },
	{ -700, 700 },
	{ -700, 700 },
	{ 1400, 1400 },
	{ 1400, -1400 },
	{ -1400, 1400 },
	{ -1400, -1400 }
};
static short Em34_mv10_pos[8][2] = {
	{ -18000, -11000 },
	{ -18000, -14500 },
	{ -18000, -18000 },
	{ -18000, -21300 },
	{ -9000, -11000 },
	{ -9000, -14500 },
	{ -9000, -18000 },
	{ -9000, -21300 }
};
static unsigned char Em34_uppos = 0x0;
/* @80109369..8010936C: 3 unrecorded bytes (padding/unknown): 000000 */
static unsigned char Em34_uppos_no[4] = { 0x0, 0x0, 0x0, 0x0 };
static short Em34_mv10_downpos[16][2] = {
	{ -21800, -7400 },
	{ -21800, -16200 },
	{ -21800, -25100 },
	{ -18000, -25200 },
	{ -13200, -25200 },
	{ -13200, -16100 },
	{ -13200, -7300 },
	{ -17900, -7300 },
	{ -13800, -7300 },
	{ -13900, -16200 },
	{ -13900, -25400 },
	{ -9200, -25400 },
	{ -5100, -25400 },
	{ -5100, -16700 },
	{ -5100, -7200 },
	{ -9100, -7200 }
};
static unsigned char Em34_pl_downpos_no = 0x0;
/* @801093B1..801093B2: 1 unrecorded bytes (padding/unknown): 00 */
static short Em34_back_pos_x = 0;
static short Em34_back_pos_z = 0;
static short Em34_sake_cnt = 0;
/* @801093B8: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_routine_0[8])() = {
	Em34_Init,
	Em34_Move,
	Em34_Damage,
	Em34_Die,
	Em34_Scenario,
	0,
	0,
	Em34_Dead
};
static unsigned char Em34_Me3_0[4] = { 0x3, 0x2E, 0x36, 0x3A };
static unsigned char Em34_Me4_0[15] = { 0xE, 0x1A, 0x1D, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x34 };
/* @801093EB..801093EC: 1 unrecorded bytes (padding/unknown): 00 */
static unsigned char Em34_Me3_1[9] = { 0x8, 0x0, 0x18, 0x1C, 0x21, 0x28, 0x29, 0x2A, 0x2B };
/* @801093F5..801093F8: 3 unrecorded bytes (padding/unknown): 000000 */
static unsigned char Em34_Me4_1[1] = { 0x0 };
/* @801093F9..801093FC: 3 unrecorded bytes (padding/unknown): 000000 */
static unsigned char Em34_Me3_2[13] = { 0xC, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x39, 0xBA, 0xBD, 0xBE };
/* @80109409..8010940C: 3 unrecorded bytes (padding/unknown): 000000 */
static unsigned char Em34_Me4_2[6] = { 0x5, 0x17, 0x18, 0x19, 0x1A, 0x44 };
/* @80109412..80109414: 2 unrecorded bytes (padding/unknown): 0000 */
static MEDAMA Em34_Me[3] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, Em34_Me3_0, Em34_Me4_0, 0x10, 18, -17, 15, -12 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, Em34_Me3_1, Em34_Me4_1, 0x5, 5, -5, 5, -5 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, Em34_Me3_2, Em34_Me4_2, 0xA, 18, -17, 15, -12 }
};
/* Retail .data has no Em34_init_flg (Beta 2: byte after Em34_Me): the MEDAMA tail word 0x000000F4 at
 * .data+0x1AC is followed directly by Em34_Move_type, and no retail code reads or writes the flag.
 * Em34_Move_type has four retail entries (image words at .data+0x1B0: Em34_Move_type00, then
 * Em34_Move_type01 x3, resolved through em34_cdemd0_s1.symbols.txt). */
/* @8010946C: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_Move_type[4])() = {
	Em34_Move_type00,
	Em34_Move_type01,
	Em34_Move_type01,
	Em34_Move_type01
};
/* @80109474: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_mv_br_type00[18])() = {
	Em34_mv00_br,
	Em34_mv01_br,
	Em34_mv02_br,
	Em34_mv03_br,
	Em34_mv04_br,
	Em34_mv05_br,
	Em34_mv06_br,
	Em34_mv07_br,
	Em34_mv08_br,
	Em34_mv09_br,
	Em34_mv0a_br,
	Em34_mv0b_br,
	Em34_mv0c_br,
	Em34_mv0d_br,
	Em34_mv0e_br,
	Em34_mv0f_br,
	Em34_mv10_br,
	Em34_mv10_br
};
/* @801094BC: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_mv_type00[18])() = {
	Em34_mv00,
	Em34_mv01,
	Em34_mv02,
	Em34_mv03,
	Em34_mv04,
	Em34_mv05,
	Em34_mv06,
	Em34_mv07,
	Em34_mv08,
	Em34_mv09,
	Em34_mv0a,
	Em34_mv0b,
	Em34_mv0c,
	Em34_mv0d,
	Em34_mv0e,
	Em34_mv0f,
	Em34_mv10,
	Em34_mv10
};
/* Original SYM preserves both empty header-owned dispatch arrays at Em34_iki. */
static void (*Em34_mv_br_type01[0])() = {};
static void (*Em34_mv_type01[0])() = {};
static char Em34_iki[64] = {
	12,
	23,
	35,
	43,
	51,
	60,
	66,
	71,
	75,
	79,
	82,
	86,
	89,
	92,
	94,
	96,
	97,
	98,
	99,
	100,
	99,
	98,
	97,
	96,
	94,
	92,
	89,
	87,
	85,
	83,
	80,
	78,
	75,
	72,
	68,
	65,
	62,
	58,
	54,
	49,
	44,
	39,
	34,
	29,
	24,
	19,
	13,
	7,
	0,
	-6,
	-15,
	-11,
	-26,
	-30,
	-32,
	-34,
	-35,
	-35,
	-34,
	-31,
	-25,
	-20,
	-10,
	0
};
static unsigned char Em34_mv02_tbl[2][2] = {
	{ 0x1E, 0x7 },
	{ 0x13, 0x14 }
};
static short Em34_mv05_dir[4] = { 3072, 0, 1024, 2048 };
static unsigned char Em34_Dm_cnt[18] = {
	1, 3, 3, 3, 10, 15, 5, 8, 10, 8, 8, 3, 2, 20, 2, 2, 20, 4
};
/* @80109562..80109564: 2 unrecorded bytes (padding/unknown): 0000 */
/* @80109564: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_Damage_type[2])() = {
	Em34_Damage_33,
	Em34_Damage_34
};
static unsigned long D_8010B118 = 0;	/* retail-only EM34.C word (name unknown, data 0x8010B118): Em34_die00 shadow fade colour for Kage_work9_color_set */
/* Original SYM records an empty Em34_sce followed by Pldamage_Em34_tbl. */
static void (*Em34_sce[0])() = {};
static void (*Pldamage_Em34_tbl[2])() = {
	Em34_pldm00,
	Em34_pldm00
};
/* @80109574: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Pldie_Em34_tbl[3])() = {
	Em34_pldie00,
	Em34_pldie00,
	Em34_pldie01
};
/* @80109580: no retail SYM record; name from config/overlays/ovl_18.symbols.txt (inferred names) */
static void (*Em34_kuchi_move_tbl[7])() = {
	Em34_kuchi_mv0,
	Em34_kuchi_mv1,
	Em34_kuchi_mv2,
	Em34_kuchi_mv3,
	Em34_kuchi_mv4,
	Em34_kuchi_mv5,
	Em34_kuchi_mv6
};
static short Em34_kuchi_mv1_tbl[3] = { 256, -128, -128 };
/* @801095A2..801095A4: 2 unrecorded bytes (padding/unknown): 0000 */
static short Em34_kuchi_mv2_tbl[32] = {
	812,
	1900,
	63,
	900,
	1500,
	963,
	100,
	697,
	1320,
	1597,
	32,
	530,
	1435,
	349,
	130,
	59,
	456,
	1912,
	756,
	692,
	1287,
	1592,
	896,
	1400,
	30,
	478,
	597,
	960,
	687,
	1364,
	974,
	43
};
static long Em34_m03_sabun[4] = { -128, -358, -412, -512 };
static JH_TBL Em34_jh0_tbl[61] = {
	{ 0x37, 0x1000 },
	{ 0x33, 0x1000 },
	{ 0x32, 0x1000 },
	{ 0x29, 0x1000 },
	{ 0x27, 0x1000 },
	{ 0x26, 0x1000 },
	{ 0x19, 0x1000 },
	{ 0x2, 0x1000 },
	{ 0x1, 0x1000 },
	{ 0x14, 0x1000 },
	{ 0x13, 0x1000 },
	{ 0x22, 0x1000 },
	{ 0x23, 0x1000 },
	{ 0x36, 0x1000 },
	{ 0x0, 0x60C },
	{ 0x28, 0x60C },
	{ 0x31, 0x60C },
	{ 0x2F, 0x60C },
	{ 0x39, 0x60C },
	{ 0x3B, 0x60C },
	{ 0x35, 0x60C },
	{ 0x24, 0x60C },
	{ 0x34, 0x60C },
	{ 0x30, 0x200 },
	{ 0x3C, 0x200 },
	{ 0x38, 0x200 },
	{ 0x1E, 0x200 },
	{ 0x1F, 0x200 },
	{ 0xE, 0x200 },
	{ 0xF, 0x200 },
	{ 0x21, 0x200 },
	{ 0x20, 0x200 },
	{ 0x25, 0x200 },
	{ 0x3, 0x0 },
	{ 0x4, 0x0 },
	{ 0x5, 0x0 },
	{ 0x6, 0x0 },
	{ 0x7, 0x0 },
	{ 0x8, 0x0 },
	{ 0x9, 0x0 },
	{ 0xA, 0x0 },
	{ 0xB, 0x0 },
	{ 0xC, 0x0 },
	{ 0xD, 0x0 },
	{ 0x10, 0x0 },
	{ 0x11, 0x0 },
	{ 0x12, 0x0 },
	{ 0x15, 0x0 },
	{ 0x16, 0x0 },
	{ 0x17, 0x0 },
	{ 0x18, 0x0 },
	{ 0x1A, 0x0 },
	{ 0x1B, 0x0 },
	{ 0x1C, 0x0 },
	{ 0x1D, 0x0 },
	{ 0x2A, 0x0 },
	{ 0x2B, 0x0 },
	{ 0x2C, 0x0 },
	{ 0x2D, 0x0 },
	{ 0x2E, 0x0 },
	{ 0x3A, 0x0 }
};
static JH_HEAD Em34_jh0_head[1] = {
	{ 0xA, 0x1A, 0x3D, 0, 0, 0, 0, { 1160, 127, 0, 0 }, 0, 0, 0, 0, { 0, 0, 0, 0 }, Em34_jh0_tbl }
};
static unsigned short Chg_t; /* Original BSS at 80109720 / 80116720, not initialized data. */
/* end 80109722, image end 80109724 */

void EM_MAIN(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register KAGE_WORK9 *pK9;
	register PARTS_W *pP;	/* inferred name: gcc folds it into its uses, so retail has no record */

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	if (Guide_check(pEm->Pos_x, pEm->Pos_z) == 0xFF) {
		pEm->Dest_x = G.Player.Pos_x;
		pEm->Dest_z = G.Player.Pos_z;
	}

	if (G.Stop_flg & 0x20000000)
		return;
	{
		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 1) {
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0-- <= 0) {	/* unsigned, so <= 0 is == 0; this spelling keeps retail's old-value test */
				if (((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt)
					((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt--;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0 = 15;
				if (!((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt)
					((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFE;
			}
		}

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer)
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer--;

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0)
			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0--;

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer1)
			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer1--;

		if (EM34_RETAIL_24(pEm))	/* retail-only timer */
			EM34_RETAIL_24(pEm)--;

		if (pEm->Type & 1) {
			pK9 = (KAGE_WORK9 *)pEm->pKage_work;
			pK9->Half_x = (pEm->Pos_y * 23) / 100 + 2300;
			pK9->Half_z = (pEm->Pos_y * 13) / 100 + 1300;
		}

		Em34_pos_set(pEm);

		Em34_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		Em34_mgdm(pEm);

		Em34_kuchi_move(pEm, &pEm->pSin_parts_ptr[ 10 ]);

		Em34_kubi(pEm);

		if (!(pEm->Move_cnt & 0x1F) && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x800)) {
			Em34_core_set_dest(0, 15 - Rnd() % 30, 15 - Rnd() % 30);
			Em34_core_set_dest(1, 10 - Rnd() % 20, 10 - Rnd() % 20);
			Em34_core_set_dest(2, 15 - Rnd() % 30, 15 - Rnd() % 30);
		}
		Em34_core_me_mv(pEm, 0);
		Em34_core_me_mv(pEm, 1);
		Em34_core_me_mv(pEm, 2);

		Em_seq_sound(pEm);

		Oba_set_ofs(pEm, &pEm->Pos_x);
		pP = &pEm->pSin_parts_ptr[ 26 ];
		pEm->Atd[ 1 ].X = pP->Workm.t[ 0 ];
		pEm->Atd[ 1 ].Z = pP->Workm.t[ 2 ];
		pEm->At_em_no = 0xFF;

		Oba_ck_hit(*G.pEnemy, pEm);	/* pointer form (not [0]): non-struct fixed-address load, so the Atd store may sink past it, as retail */

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x40) {
			Sca_ck_em(pEm, 0x600);
		}else {
			Sca_ck_em(pEm, 0x400);
		}

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFBF;

		if (!(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x1000) && pEm->Atd[ 0 ].At_w < 1400) {
			pEm->Atd[ 0 ].At_w += 10;
			pEm->Atd[ 0 ].At_d += 10;
			pEm->Atd[ 0 ].W += 10;
			pEm->Atd[ 0 ].D += 10;
		}

		pEm->Be_flg &= 0xFFFFFBFF;	/* retail-only */
		if (pEm->Ground)
			pEm->Be_flg |= 0x400;

	}

}

static void Em34_after(ENEMY_WORK *pEm)
{
	if (pEm->Type & 1) {
		Em34_Jh_move(pEm);
	}	}

static void Em34_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long i;
	register PARTS_W *pP;	/* inferred name: no retail record */
	register KAGE_WORK9 *pK9;	/* retail-only, [INFERRED]: same carrier as EM_MAIN's pK9 */

	*(unsigned long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	if (G.System_flg & 0x40) {	/* retail: life depends on System_flg bit 0x40 */
		pEm->Life = 1000;
		if (G.System_flg & 0x20)
			pEm->Life = 900;
	}else {
		pEm->Life = 1100;
		if (G.System_flg & 0x20)
			pEm->Life = 1000;
	}

	pEm->D_life_u = -126;
	pEm->D_life_c = -126;
	pEm->D_life_d = -126;

	pP = &pEm->pSin_parts_ptr[ 26 ];
	pEm->Neck_flg = 0;
	pEm->Neck_no = 26;
	pP->Free[ 0 ] = 0;
	pP->Free[ 1 ] = 0;
	pP->Free[ 2 ] = 0x10;
	pP->Free[ 3 ] = 0x20;
	pP->Free[ 4 ] = 0xE4;
	pP->Free[ 5 ] = 0xD4;

	((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0 = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Kubi_dir_z = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Iki = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer1 = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt = 0;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Kin_cnt = 0;
	EM34_RETAIL_24(pEm) = 0;

	pEm->nOba = 2;
	pEm->Atd[ 0 ].Ofs_x = 400;
	pEm->Atd[ 0 ].Ofs_y = -2000;
	pEm->Atd[ 0 ].Ofs_z = 0;
	pEm->Atd[ 0 ].At_w = 1100;
	pEm->Atd[ 0 ].At_h = 2000;
	pEm->Atd[ 0 ].At_d = 1100;
	pEm->Atd[ 0 ].W = 1100;
	pEm->Atd[ 0 ].D = 1100;

	pEm->Atd[ 1 ].Ofs_x = 0;
	pEm->Atd[ 1 ].Ofs_y = -2000;
	pEm->Atd[ 1 ].Ofs_z = 0;
	pEm->Atd[ 1 ].At_w = 1000;
	pEm->Atd[ 1 ].At_h = 2000;
	pEm->Atd[ 1 ].At_d = 1000;
	pEm->Atd[ 1 ].W = 1000;
	pEm->Atd[ 1 ].D = 1000;

	Em34_Me[ 0 ].Tate = 0;
	Em34_Me[ 0 ].Yoko = 0;
	Em34_Me[ 0 ].Tate_d = 0;
	Em34_Me[ 0 ].Yoko_d = 0;
	Em34_Me[ 0 ].Tate_mv = 0;
	Em34_Me[ 0 ].Yoko_mv = 0;
	Em34_Me[ 0 ].Tate_a = 0;
	Em34_Me[ 0 ].Yoko_a = 0;

	Em34_Me[ 1 ].Tate = 0;
	Em34_Me[ 1 ].Yoko = 0;
	Em34_Me[ 1 ].Tate_d = 0;
	Em34_Me[ 1 ].Yoko_d = 0;
	Em34_Me[ 1 ].Tate_mv = 0;
	Em34_Me[ 1 ].Yoko_mv = 0;
	Em34_Me[ 1 ].Tate_a = 0;
	Em34_Me[ 1 ].Yoko_a = 0;

	Em34_Me[ 2 ].Tate = 0;
	Em34_Me[ 2 ].Yoko = 0;
	Em34_Me[ 2 ].Tate_d = 0;
	Em34_Me[ 2 ].Yoko_d = 0;
	Em34_Me[ 2 ].Tate_mv = 0;
	Em34_Me[ 2 ].Yoko_mv = 0;
	Em34_Me[ 2 ].Tate_a = 0;
	Em34_Me[ 2 ].Yoko_a = 0;

	pEm->Eff_at_r = 800;

	pEm->Be_flg |= 0x1C000000;

	*(unsigned long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x100);

	Kage_work9_set(&pEm->pKage_work, 0, 0x70805DC, 0, pEm->pSin_parts_ptr->Workm.t);

	G.pPldamage_Em[ pEm->Id - 16 ] = PLDAMEGE_EM;
	G.pPldie_Em[ pEm->Id - 16 ] = PLDIE_EM;

	pEm->Be_flg |= 4;

	pEm->Be_flg &= 0xFFFFDFFF;

	G.pFree_work = (unsigned char *)Em34_Jh_init(pEm, G.pFree_work);
	pEm->pTafter_func = Em34_after;	/* retail no longer sets Em34_init_flg here */

	G.Player.pEnemy_ptr = (unsigned long *)pEm;
	G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
	G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

	if (!pEm->Type) {
		pP_ptr = pEm->pSin_parts_ptr;
		i = 27;
		while (i) {
			((short *)pP_ptr->pSa_dat_head)[ 6 ] = 0x1000;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr++;
			i--;	}

		Sa_parts_offset_allmv(pEm, pEm->pSin_parts_ptr);

		pEm->Routine_1 = 1;
		pEm->pSin_parts_ptr[ 26 ].Be_flg &= 0xFFFFBFFF;
		Chg_t = 100;
		pEm->Type = 0;
		pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
		*(short *)&pP_ptr->pSa_dat_head[ 4 ] = 0;	/* spelling inferred: a cast address (not a PLUS) is not MEM_IN_STRUCT, so the Chg_t store schedules before it as in retail */
		*(short *)&pP_ptr->pSa_dat_head[ 5 ] = 0;
		*(short *)&pP_ptr->pSa_dat_head[ 6 ] = 0;
		*(short *)&pP_ptr->pSa_dat_head[ 7 ] = 0;
		Sa_parts_allmv(pEm, pP_ptr);
		pEm->Routine_1 = 13;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x1000;

		pEnemy_damage_tbl[ pEm->Id - 16 ] = Em34_damage_tbl0;
		((KAGE_WORK9 *)pEm->pKage_work)->Be_flg = 0;
		pEm->Type |= 0x4000;
	}else {	/* retail-only variant (Type 3): headless body from the start */
		pEm->Type = 3;
		pEm->Life += 100;
		pEm->pSin_parts_ptr[ 26 ].Be_flg |= 0x4000;
		pP_ptr = pEm->pSin_parts_ptr;
		i = 27;
		while (i) {
			((short *)pP_ptr->pSa_dat_head)[ 6 ] = 0;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr++;
			i--;	}

		Sa_parts_offset_allmv(pEm, pEm->pSin_parts_ptr);
		pEm->Life = 700;
		if (G.System_flg & 0x20)
			pEm->Life = 500;
		pEm->Atd[ 0 ].Ofs_y = -1500;
		pEm->Atd[ 0 ].At_h = 1500;
		pEm->Atd[ 1 ].Ofs_y = -1500;
		pEm->Atd[ 1 ].At_h = 1500;
		pEm->Sce_free0 |= 2;
		pK9 = (KAGE_WORK9 *)pEm->pKage_work;
		pK9->Half_x = 2300;
		pK9->Half_z = 1300;
		pEm->Be_flg |= 0x2000;
		pEnemy_damage_tbl[ pEm->Id - 16 ] = Em34_damage_tbl1;
		*(unsigned long *)&pEm->Routine_0 = 0x401;
		((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 90;
		EM34_RETAIL_24(pEm) = 90;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 0;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Be_flg &= 0xEFFFFFFF;
		pEm->Damage_cnt &= 0x7F;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xEFFF;
		pEm->Atd[ 0 ].At_w -= 50;
		pEm->Atd[ 0 ].At_d -= 50;
		pEm->Atd[ 0 ].W -= 50;
		pEm->Atd[ 0 ].D -= 50;
	}

}

static void Em34_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em34_Move_type[ pEm->Type & 0x3F ](pEm, Kan, Seq);

	if ((((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x180) == 0x100) {
		*(unsigned long *)&pEm->Routine_0 = 3;
	}	}

static void Em34_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em34_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em34_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em34_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em34_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em34_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em34_mv00_br(ENEMY_WORK *pEm)
{

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 800);

}
static void Em34_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;
	register KAGE_WORK9 *pK9;
	SVECTOR Sv0;
	register PARTS_W *pP;	/* inferred name: gcc folds it into its uses, so retail has no record */

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF000B;
		pEm->Routine_2 = 1;
		pEm->Neck_flg |= 2;
		pEm->Damage_cnt |= 0x80;
		pEm->Life = 700;
		if (G.System_flg & 0x20)
			pEm->Life = 500;

		pEm->Sce_free0 |= 2;

		break;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);
		break;

	case 2:
		pEm->Timer0 = 150;
		pEm->Routine_2 = 3;
		pEm->Timer1 = 0x1000;

	case 3:
		pP = &pEm->pSin_parts_ptr[ 10 ];
		((short *)pP->pSa_dat_head)[ 6 ] = pEm->Timer1 + Em34_iki[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Iki ] * 3;

		if (pEm->Timer0 < 80) {
			((short *)pP->pSa_dat_head)[ 6 ] += (Rnd() & 0x3F) * (1 - ((pEm->Timer0 & 1) << 1));
			if (!(pEm->Timer0 & 1)) {	/* retail-only effect burst; pP stays on part 15 below, as retail */
				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x830), Rnd() << 4, &pP_ptr[ 11 ].Workm, 0);
				pP_ptr = pEm->pSin_parts_ptr;
				pP = &pP_ptr[ 15 ];
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x830), Rnd() << 4, &pP_ptr[ 15 ].Workm, 0);
			}
		}

		if (pEm->Timer0 < 30 && !(pEm->Timer0 & 1)) {	/* retail-only effect burst */
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP_ptr[ 11 ].Workm, 0);
			pP_ptr = pEm->pSin_parts_ptr;
			pP = &pP_ptr[ 15 ];
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP_ptr[ 15 ].Workm, 0);
		}

		if (pEm->Timer0 < 30) {
			((short *)pP->pSa_dat_head)[ 6 ] += (Rnd() & 0x3F) * (1 - ((pEm->Timer0 & 1) << 1));

			((EM34_FREE *)EM_FREE_AREA(pEm))->Iki++;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Iki &= 0x3F;

			pP = &pEm->pSin_parts_ptr[ 26 ];
			((short *)pP->pSa_dat_head)[ 6 ] = pEm->Timer1 + Em34_iki[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Iki ] * 3;

			Sa_parts_allmv(pEm, pP);
			pEm->Timer1 -= 0x30;

		}

		if ((unsigned short)pEm->Timer0-- == 0)	/* cast keeps the old-value test, as retail */
			pEm->Routine_2 = 4;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Iki++;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Iki &= 0x3F;

		pP = &pEm->pSin_parts_ptr[ 10 ];
		Sa_parts_allmv(pEm, pP);
		break;

	case 4:
		*(unsigned long *)&pEm->Move_no = 0xF000C;
		pEm->Routine_2 = 5;

		pEm->Atd[ 0 ].Ofs_y = -1500;
		pEm->Atd[ 0 ].At_h = 1500;
		pEm->Atd[ 1 ].Ofs_y = -1500;
		pEm->Atd[ 1 ].At_h = 1500;

		pEm->Timer1 = 0;

	case 5:

		pK9 = (KAGE_WORK9 *)pEm->pKage_work;
		pK9->Half_x += 100;
		if (pK9->Half_x > 2300)
			pK9->Half_x = 2300;

		pK9->Half_z -= 10;
		if (pK9->Half_z < 1300)
			pK9->Half_z = 1300;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		pP = pEm->pSin_parts_ptr;
		Kan = 0;	/* [INFERRED reuse] Kan (dead after the Joint_move above) is the loop counter: only then is i block-local below, as retail's register map requires */
		while (Kan < 27) {	/* [INFERRED reuse] Kan */
			if (Kan == 10 || Kan == 26) { Kan++; pP++; continue; }	/* [INFERRED reuse] Kan */

				((short *)pP->pSa_dat_head)[ 6 ] -= 0x40;
				if (((short *)pP->pSa_dat_head)[ 6 ] < 0)
					((short *)pP->pSa_dat_head)[ 6 ] = 0;

				Sa_parts_allmv(pEm, pP);

			Kan++; pP++;	}	/* [INFERRED reuse] Kan */

		if (pEm->Move_cnt && !(pEm->Type & 1)) {
			pP = &pEm->pSin_parts_ptr[ 10 ];
			((short *)pP->pSa_dat_head)[ 6 ] -= pEm->Timer1;
			((short *)pP->pSa_dat_head)[ 6 ] += Rnd() * (1 - ((pEm->Timer0 & 1) << 1));

			if (((short *)pP->pSa_dat_head)[ 6 ] < 0) {
				((short *)pP->pSa_dat_head)[ 6 ] = 0;
				pEm->Type |= 1;
				pEm->pSin_parts_ptr[ 26 ].Be_flg |= 0x4000;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 4;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
			}

			pP = &pEm->pSin_parts_ptr[ 26 ];
			((short *)pP->pSa_dat_head)[ 6 ] -= pEm->Timer1;
			if (((short *)pP->pSa_dat_head)[ 6 ] < 0)
				((short *)pP->pSa_dat_head)[ 6 ] = 0;

			Sa_parts_allmv(pEm, pP);
		}
		pEm->Timer1 += 2;

		pP = &pEm->pSin_parts_ptr[ 10 ];
		i = Rnd() & 0x3F;	i *= 1 - ((pEm->Timer0 & 1) << 1);	/* two statements: retail computes the masked Rnd() in i's register */
		((short *)pP->pSa_dat_head)[ 6 ] += i;
		Sa_parts_allmv(pEm, pP);
		pP = &pEm->pSin_parts_ptr[ 26 ];
		((short *)pP->pSa_dat_head)[ 6 ] += i;
		Sa_parts_allmv(pEm, pP);

		pEm->Timer0++;

		if (pEm->Move_cnt == 120) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 3;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		}
		if (pEm->Move_cnt == 140) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 4;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;

			pEm->Be_flg |= 0x2000;
		}

		if ((pEm->Move_cnt & 1) && pEm->Move_cnt < 45) {	/* retail-only effect burst */
			pP = &pEm->pSin_parts_ptr[ 11 ];
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP->Workm, 0);
			pP = &pEm->pSin_parts_ptr[ 15 ];
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0xC18), Rnd() << 4, &pP->Workm, 0);
			pP = &pEm->pSin_parts_ptr[ 16 ];
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP->Workm, 0);
		}
		break;
	case 6:
		*(unsigned long *)&pEm->Move_no = 0x7000A;
		pEm->Routine_2 = 7;
		pEm->Type = 1;

		pEnemy_damage_tbl[ pEm->Id - 16 ] = Em34_damage_tbl1;
		pEm->Be_flg &= 0xEFFFFFFF;

		pEm->pSin_parts_ptr[ 26 ].Be_flg |= 0x4000;
		pP = &pEm->pSin_parts_ptr[ 26 ];
		((short *)pP->pSa_dat_head)[ 6 ] = 0;
		pP = &pEm->pSin_parts_ptr[ 10 ];
		((short *)pP->pSa_dat_head)[ 6 ] = 0;

	case 7:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_2 = 8;
			pEm->Sce_free0 |= 8;
		}

		Add_speedXZ(pEm, 0);
		break;

	case 8:	/* retail-only: wait for the scenario release (Sce_free0 & 0x10) */
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		pEm->Routine_2 = 9;
		pEm->Timer0 = 0;
		pEm->Timer2 = (Rnd() & 0xF) + 1;
		Em34_sake_cnt = 0;
		Snd_se_enem(0, pEm);

	case 9:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100))
			Snd_se_enem(11, pEm);

		if (pEm->Sce_free0 & 0x10) {
			*(unsigned long *)&pEm->Routine_0 = 0x301;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 5;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 90;
			EM34_RETAIL_24(pEm) = 90;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 0;
			pEm->Spd_x = 0;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
			pEm->Neck_flg &= 0xFD;
			pEm->Damage_cnt &= 0x7F;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xEFFF;
			pEm->Atd[ 0 ].At_w -= 50;
			pEm->Atd[ 0 ].At_d -= 50;
			pEm->Atd[ 0 ].W -= 50;
			pEm->Atd[ 0 ].D -= 50;
		}

		Add_speedXZ(pEm, 0);
	}
}

static void Em34_mv01_br(ENEMY_WORK *pEm)
{
	register short Dir1;	/* inferred name: no retail record */
	register short Dir2;
	register unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;

	LL = pEm->L_pl;

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1524);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!Dir1 && LL < 4000 && !((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer)
		*(unsigned long *)&pEm->Routine_0 = 0x201;

	if (!Dir2 && LL < 2000)
		*(unsigned long *)&pEm->Routine_0 = 0x801;

	if (pEm->Life < 700) {
		*(unsigned long *)&pEm->Routine_0 = 1;
		pEm->Damage_cnt |= 0x80;
	}

}

static void Em34_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	VECTOR Tv;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0;

		Em34_sake_cnt = 0;

		Snd_se_enem(0, pEm);

	case 1:
		if (Em34_sake_cnt) {
			Em34_sake_cnt--;
			Goto00(pEm, Em34_back_pos_x, Em34_back_pos_z, -16);
		}else {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
			if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x338))
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
		}

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Spd_x += 10;

		if (pEm->Sca_info & 1) {
			pEm->Timer0++;
			if (pEm->Timer0 > 90) {

				Tv.vx = pEm->Atd[ 0 ].X;
				Tv.vy = pEm->Atd[ 0 ].Y;
				Tv.vz = pEm->Atd[ 0 ].Z;
				Sca_ck_hit(&Tv, 0x6A4, 1, 0x500);
				if (*G.pSca_hit_data) {	/* pointer form: non-struct load, so gcc keeps it in one register (as retail) */

					Em34_back_pos_x = (*G.pSca_hit_data)->X + ((*G.pSca_hit_data)->W >> 1);
					Em34_back_pos_z = (*G.pSca_hit_data)->Z + ((*G.pSca_hit_data)->D >> 1);
					Em34_sake_cnt = 8;
				}
			}
		}else
			pEm->Timer0 = 0;

		Joint_move(pEm, Kan, Seq, 0x80);
		Add_speedXZ(pEm, 0);
	}
}

static void Em34_mv02_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	SVECTOR Sv1;
	register PARTS_W *pP_ptr;
	MATRIX M;
	register short Ck;	register short Dir;	register PARTS_W *pP;	/* Dir, pP: inferred names, no retail record */
	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer2 = (Rnd() >> (Rnd() & 7)) & 1;

		if (pEm->L_pl < 2500)
			pEm->Timer2 = 1;

		pEm->Timer3 = 0;

		*(unsigned long *)&pEm->Move_no = (signed char)pEm->Timer2 + 0xF0003;
		pEm->Routine_2 = 1;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		pEm->Timer1 = 0;
		pEm->Timer0 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFD;

		Snd_se_enem(0, pEm);

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {

			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 2) goto rout2;

			if (G.Player.Life < 0) {
rout2:			pEm->Routine_2 = 2;	/* inferred label: retail branches from the flag test straight into this statement */
				break;
			}

			*(unsigned long *)&pEm->Routine_0 = 0x101;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 10;

		}	if (!pEm->Timer1 && pEm->L_pl < 4000 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200) && pEm->Move_cnt == Em34_mv02_tbl[ (signed char)pEm->Timer2 ][ 0 ] && G.Player.Life >= 0) {

			pEm->Timer1 = 1;
			pEm->Timer2 = ~pEm->Timer2 & 1;
			*(unsigned long *)&pEm->Move_no = (Em34_mv02_tbl[ pEm->Timer2 ^ 1 ][ 1 ] << 8) + 0xF0003 + pEm->Timer2;

			Spd_get(pEm, Kan, Seq, 0);
			Spd_get(pEm, Kan, Seq, 0);
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

			if (G.Player.Life >= 0)
				G.Player.Damage_cnt &= 0x7F;
		}

		if (*pEm->pNow_seq & 0x10000) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
			Sv0.vx = 0;
			Sv0.vy = 0;
			Sv0.vz = 0;
			Sv1.vx = 0;
			Sv1.vy = 600;
			Sv1.vz = 0;

			if (!pEm->Timer2) {
				Em34_hand_right(pEm, &M);
			}else {
				Em34_hand_left(pEm, &M);
			}

			if ((L_pos_ckXZM(&M, &G.Player.Pos_x, 1700, &Sv0) || L_pos_ckXZM(&M, &G.Player.Pos_x, 1500, &Sv1)) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - 0x400;

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (pEm->Timer2)
					Ck ^= 1;

				if (Pl_life_down(15, 0) != 2) {

					*(unsigned long *)&G.Player.Routine_0 = ((Ck | 2) << 8) + 2;
				}else {

					*(unsigned long *)&G.Player.Routine_0 = ((signed char)pEm->Timer2 << 8) + 6;
					pEm->Damage_cnt |= 0x80;
					G.System_flg |= 0x100;
				}

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				pEm->Timer3 = 6;
				pEm->Timer0 = (signed char)pEm->Timer2;

				Snd_se_enem(3, pEm);
				Em34_Tume_color(pEm, (signed char)pEm->Timer2);
				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 2;
				Pad_vib_set0(8, 0); /* retail-only Dual Shock vibration */
				Pad_vib_set1(8, 250, 4);
			}

			Sv0.vx = 0;
			Sv0.vy = -500;
			Sv0.vz = 0;
			ApplyMatrixSV(&M, &Sv0, &Sv0);
			M.t[ 0 ] += Sv0.vx;
			M.t[ 1 ] += Sv0.vy;
			M.t[ 2 ] += Sv0.vz;
			Sca_get_ground(&M.t[ 0 ], 600, 0x8000, 1);
			if (Ground_in_flg) {
				pP_ptr = &pEm->pSin_parts_ptr[ 19 - (signed char)pEm->Timer2 * 5 ];
				Sv0.vx = 0;
				Sv0.vy = -500;
				Sv0.vz = 0;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vx -= (Rnd() & 0x7F) - 0x40;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vy -= Rnd() - 0x80;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
				Sv0.vz -= (Rnd() & 0x7F) - 0x40;
				Esp_call(0x1061FA0, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

				if (!((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0) {
					Snd_se_enem(4, pEm);
					((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 7;
				}

			}
		}

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0)
			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0--;

		if (*(signed char *)&pEm->Timer3) {
			(*(signed char *)&pEm->Timer3)--;
			if (!pEm->Timer0) {
				Em34_hand_right(pEm, &M);
			}else {
				Em34_hand_left(pEm, &M);
			}
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = 1256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP_ptr = &pEm->pSin_parts_ptr[ 19 - pEm->Timer0 * 5 ];
			pP_ptr->Workm = M;
			Esp_call((Rnd() << 2) | 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = -200;
			Sv0.vz = 256 - Rnd() * 2;
			Esp_call((Rnd() << 2) | 0x1000, 0, &pP_ptr->Workm, &Sv0);

			pP = &G.Player.pSin_parts_ptr[ 8 ];
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = 0;
			Sv0.vz = 256 - Rnd() * 2;
			Esp_call((Rnd() << 2) | 0x1000, 0, &pP->Workm, &Sv0);
		}

		Add_speedXZ(pEm, 0);
		break;

	case 2:
		*(unsigned long *)&pEm->Move_no = 0xF0009;
		pEm->Routine_2 = 3;

	case 3:
		if (pEm->Hokan_flg)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		if (pEm->Move_cnt >= 26)
		if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338) || pEm->L_pl > 5000) {

			if (G.Player.Life < 0) {
				pEm->Routine_2 = 4;
				break;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 5;
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {

			if (G.Player.Life < 0) {
				pEm->Routine_2 = 4;
				break;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 10;
		}
		Add_speedXZ(pEm, 0);
		break;
	case 4:
		break;

	}
}
static void Em34_mv03_br(ENEMY_WORK *pEm)
{
	register short Dir0;
	register short Dir1;
	register short Dir2;
	register short Dir3;
	register unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;

	LL = pEm->L_pl;

	Dir0 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1524);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 712);
	Dir2 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338);
	Dir3 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);

	if (!(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20)) {

		if (!G.Player.Damage_cnt) {

			if (!Dir2 && LL < 3300 && !((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer) {
				*(unsigned long *)&pEm->Routine_0 = 0xE01;
				if (((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt)
					((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt--;
			}

			if (!EM34_RETAIL_24(pEm) && !Dir3 && LL < 3800 && (!((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt || G.Player.Life < 30)) {
				*(unsigned long *)&pEm->Routine_0 = 0xF01;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt = (Rnd() & 3) + 3;
				if ((pEm->Type & 0xFFF) == 3)	/* retail-only */
					((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt = (Rnd() & 1) + 2;
				return;
			}
		}

		if (Dir2)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer++;

		if ((pEm->Type & 0xFFF) != 3 && Em34_uppos >= 3 && Dir2 && !((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0) {
			*(unsigned long *)&pEm->Routine_0 = 0x1001;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num = (Rnd() & 1) + 1;
		}

		if ((pEm->Type & 0xFFF) != 3 && Em34_uppos && Dir0 && ((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 < 45 && ((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer > 100) {
			*(unsigned long *)&pEm->Routine_0 = 0x1001;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num = (Rnd() & 1) + 1;
		}

		if (!Dir1 && LL > 3000 && !((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 && (pEm->Status_flg & 0x800)) {

			*(unsigned long *)&pEm->Routine_0 = 0x501;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Up_cnt++;

			if (G.Player.Routine_1 == 5)
				*(unsigned long *)&pEm->Routine_0 = 0xC01;
		}

		if ((pEm->Type & 0xFFF) != 3 && ((EM34_FREE *)EM_FREE_AREA(pEm))->Up_cnt >= 5 && Em34_uppos) {
			*(unsigned long *)&pEm->Routine_0 = 0x1001;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num = (Rnd() & 3) + 1;
		}

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer > 200) {
			if (pEm->Status_flg & 0x800)
				*(unsigned long *)&pEm->Routine_0 = 0x501;
			else
				*(unsigned long *)&pEm->Routine_0 = 0x401;

			if ((pEm->Type & 0xFFF) != 3 && (Rnd() & 3) && Em34_uppos) {
				*(unsigned long *)&pEm->Routine_0 = 0x1001;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num = (Rnd() & 1) + 1;
			}
		}

		if (LL > 8000 && !((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 && !(pEm->Status_flg & 0x800))
			*(unsigned long *)&pEm->Routine_0 = 0x401;

		if (!Dir2 && LL < 2500 && G.Player.Life < 0)	/* retail-only */
			*(unsigned long *)&pEm->Routine_0 = 0xA01;

	}else {
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer++;

		if (Em34_uppos >= 3) {
			*(unsigned long *)&pEm->Routine_0 = 0x1101;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;

			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num)
				((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num--;
		}

		if (!((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num && !Dir1) {
			*(unsigned long *)&pEm->Routine_0 = 0x501;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;

			if (LL > 8000 || !(Rnd() & 3)) {

				*(unsigned long *)&pEm->Routine_0 = 0xB01;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 5;
			}
		}

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer > 150) {
			*(unsigned long *)&pEm->Routine_0 = 0x501;
			if (LL > 8000) {

				*(unsigned long *)&pEm->Routine_0 = 0xB01;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 5;
			}
			if (Em34_uppos) {

				*(unsigned long *)&pEm->Routine_0 = 0x1101;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;
				if (((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num)
					((EM34_FREE *)EM_FREE_AREA(pEm))->Jump_num--;
			}
		}
	}

}
static void Em34_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	VECTOR Tv;

	((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x40;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		pEm->Routine_2 = 1;

		pEm->Timer0 = 0;

		pEm->Timer2 = (Rnd() & 0xF) + 1;

		Em34_sake_cnt = 0;

		Snd_se_enem(0, pEm);

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (pEm->Spd_x > 20)
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 4);

		if (pEm->Spd_x > 30)
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20) {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
			if (pEm->Timer2)
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Timer2);
		}

		if (pEm->L_pl < 3500 && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20)) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kin_cnt++;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kin_cnt > 150 && ((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt >= 2) {
				*(unsigned long *)&pEm->Routine_0 = 0xB01;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 4;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kin_cnt = 0;
			}
		}

		if (Joint_move(pEm, Kan, Seq, 0x100))
			Snd_se_enem(11, pEm);

		Add_speedXZ(pEm, 0);
	}
}

static void Em34_mv04_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	char Em34_mv04_z[ 8 ] = { 50, -50, 40, -40, 0, 0, 80, -80 };

	register short Do_x;
	register short Do_z;
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;
	VECTOR Tv;
	register PARTS_W *pP;	register short Dir;	register short Ck;	/* inferred names: no retail record */

	((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x40;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70000;
		pEm->Routine_2 = 1;

		pEm->Timer2 = 0;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);

		pEm->Timer3 = Em34_mv04_z[ (Rnd() >> (Rnd() & 3)) & 7 ];

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFD;

		Em34_sake_cnt = 0;

		if (Rnd() & 1) {
			Snd_se_enem(10, pEm);
		}

	case 1:

		if (!(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 2)) {
			if (Em34_sake_cnt) {
				Em34_sake_cnt--;
				Goto00(pEm, Em34_back_pos_x, Em34_back_pos_z, -32);
			}else
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
		}

		Spd_get(pEm, Kan, Seq, 0);

		if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x200) || (pEm->Timer2 & 0x40)) {

			Do_x = ((pEm->Spd_x << 4) / 3) >> 4;
			Do_z = ((pEm->Spd_z << 4) / 3) >> 4;
			pEm->Spd_x -= Do_x;
			pEm->Spd_z -= Do_z;
		}
		if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400) || (pEm->Timer2 & 0x40)) {

			pEm->Spd_x -= Do_x;
			pEm->Spd_z -= Do_z;
		}

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x200) {
			pEm->Timer2 = -64;
			Snd_se_enem(11, pEm);
		}

		if (Joint_move(pEm, Kan, Seq, 0x100)) {

			pEm->Timer3 = Em34_mv04_z[ (Rnd() >> (Rnd() & 3)) & 7 ];

			if ((signed char)pEm->Timer2 & 0x80) {
				if (!(pEm->Timer2 & 0xF)) {
					*(unsigned long *)&pEm->Routine_0 = 0x301;

					if (Rnd() & 1) {
						*(unsigned long *)&pEm->Routine_0 = 0xB01;
						((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 4;
					}
					((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 90;
					if (pEm->Timer2 & 0x40) {
						((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 30;
						if (G.Player.Life < 30)
							((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 0;
					}
				}

				pEm->Timer2--;
			}
		}

		pEm->Spd_x += 10;
		if (pEm->Sca_info & 1) {
			pEm->Timer0++;
			if (pEm->Timer0 > 90) {

				Tv.vx = pEm->Atd[ 0 ].X;
				Tv.vy = pEm->Atd[ 0 ].Y;
				Tv.vz = pEm->Atd[ 0 ].Z;
				Sca_ck_hit(&Tv, 0x6A4, 1, 0x500);
				if (*G.pSca_hit_data) {	/* pointer form: non-struct load, kept in one register (as retail) */

					Em34_back_pos_x = (*G.pSca_hit_data)->X + ((*G.pSca_hit_data)->W >> 1);
					Em34_back_pos_z = (*G.pSca_hit_data)->Z + ((*G.pSca_hit_data)->D >> 1);
					Em34_sake_cnt = 4;
				}
			}
		}else
			pEm->Timer0 = 0;

		if (pEm->Spd_x > 200 && !(pEm->Timer2 & 0x40)) {
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 1400;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&pP->Workm, &G.Player.Pos_x, 1500, &Sv0) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				Pl_life_down(20, 1);
				*(unsigned long *)&G.Player.Routine_0 = ((Ck | 2) << 8) + 2;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				pEm->Timer1 = 6;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 2;
				Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, -128);
				Snd_se_enem(7, pEm);
				Pad_vib_set0(3, 0); /* retail-only Dual Shock vibration */
				Pad_vib_set1(15, 250, 0);
			}
		}

		if (pEm->L_pl < 4500 && !((signed char)pEm->Timer2 & 0x80))
			pEm->Timer2 = -127;

		Add_speedXZ(pEm, 0);
	}

	if (pEm->Timer1) {
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, -16);
		pEm->Timer1--;

		Sv0.vx = 1900;
		Sv0.vy = 256 - Rnd() * 2;
		Sv0.vz = 256 - Rnd() * 2;
		pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
		Esp_call((Rnd() << 2) + 0xC18, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		Sv0.vx = 2000;
		Esp_call(((Rnd() << 2) + 0xC18) | 0x10000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		pP_ptr = G.Player.pSin_parts_ptr;
		Sv0.vx = 256 - Rnd() * 2;
		Sv0.vy = -200;
		Sv0.vz = 256 - Rnd() * 2;
		Esp_call((Rnd() << 2) + 0xC18, 0, &pP_ptr->Workm, &Sv0);
	}

}
static void Em34_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em34_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Ck;
	VECTOR Tv;
	SVECTOR Sv;
	SVECTOR Sv0;
	register PARTS_W *pP_ptr;	register unsigned long L;
	register PARTS_W *pP;	register short Dir;	register short Ck1;	/* inferred names: no retail record */
	pEm->Timer0++;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70006;
		pEm->Routine_2 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu = 0;

		pEm->Timer3 = 0;
		pEm->Timer2 = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFD;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;

		if (Rnd() & 1)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 4;
		Pad_vib_set1(8, 100, 15);	/* retail-only Dual Shock vibration */

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
		Joint_move(pEm, Kan, Seq, 0x200);

		if (pEm->Move_cnt == 19) {
			pEm->Routine_2 = 2;

			L = Em34_pl_pos(pEm, &Em34_T_PL_POS, 1500, ((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20);

			Sv.vx = L >> 4;
			if (Sv.vx < 400 && (Rnd() & 1))
				Sv.vx = 400;

			if (Sv.vx > 600)
				Sv.vx = 600;

			if (pEm->Pos_x < -20900 || pEm->Pos_x > -6200 || pEm->Pos_z < -24500 || pEm->Pos_z > -7700) {

				if (!(Rnd() & 3)) {
					Sv.vx += 300;
					Goto00(pEm, Em34_T_PL_POS.vx, Em34_T_PL_POS.vz, -64);
				}
			}

			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20)
				Sv.vx = L / 21;

			Sv.vz = 0;
			Sv.vy = -900;

			Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, Em34_T_PL_POS.vx, Em34_T_PL_POS.vz);

			Em34_dir_spd(pEm, Ck, &Sv);

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x80;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 4;

			Goto00(pEm, Em34_T_PL_POS.vx, Em34_T_PL_POS.vz, 0x20);

			pEm->Timer0 = 0;
			Snd_se_enem(2, pEm);

		}

		if (pEm->Move_cnt >= 16 && pEm->Pos_y > -1500) {
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 1400;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&pP->Workm, &G.Player.Pos_x, 1500, &Sv0) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

				Ck1 = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				Pl_life_down(15, 1);
				*(unsigned long *)&G.Player.Routine_0 = (Ck1 << 8) + 2;
				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				pEm->Timer2 = 6;
				Snd_se_enem(7, pEm);
				Pad_vib_set0(10, 0);	/* retail-only Dual Shock vibration */
				Pad_vib_set1(5, 200, 4);
			}
		}

		break;

	case 2:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 120 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;
		break;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0xF000D;
		pEm->Routine_2 = 4;

	case 4:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x10)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);

		Joint_move(pEm, Kan, Seq, 0x100);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 120 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;

		if (pEm->Spd_y > 0 && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x10)) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x10;
			*(unsigned long *)&pEm->Move_no = 0xF000E;
		}

		pEm->Pos_x += pEm->Spd_x;	/* retail-only: wall check at the next position */
		pEm->Pos_z += pEm->Spd_z;
		Sca_ck_em(pEm, 0x400);
		pEm->Pos_x -= pEm->Spd_x;
		pEm->Pos_z -= pEm->Spd_z;

		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			*(unsigned long *)&pEm->Routine_0 = 0x601;
		}

		Tv.vx = pEm->Atd[ 0 ].X;
		Tv.vy = pEm->Atd[ 0 ].Y;
		Tv.vz = pEm->Atd[ 0 ].Z;
		Sca_ck_hit(&Tv, 1700, 1 << pEm->nFloor, 0x1100);

		if (G.pSca_hit_data[ 0 ] && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 8) && pEm->Pos_y < -3200 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x40)) {

			*(unsigned long *)&pEm->Routine_0 = 0x701;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 8;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_no = Sca_get_dir(&pEm->Pos_x, G.pSca_hit_data[ 0 ]);

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_dir = Em34_kabe_dir(pEm, ((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_no & 1);

		}

		if ((((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x10) && pEm->Pos_y > -2000) {

			pP = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 1000;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&pP->Workm, &G.Player.Pos_x, 2000, &Sv0) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

				Ck1 = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (Pl_life_down(30, 0) != 2) {

					*(unsigned long *)&G.Player.Routine_0 = ((Ck1 | 2) << 8) + 2;
				}else {

					*(unsigned long *)&G.Player.Routine_0 = 3;
					pEm->Damage_cnt |= 0x80;
				}

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 2;

				pEm->Timer2 = 6;
				Snd_se_enem(7, pEm);
				Pad_vib_set0(8, 0);	/* retail-only Dual Shock vibration */
				Pad_vib_set1(13, 250, 2);

			}
		}

		Ck = Sca_get_ground(&Tv, 1400, 0x400, 0);
		if (Ground_in_flg)
			Ck = Ground_in_pos;

		if (!Ck)
			pEm->nFloor = 0;

		if (pEm->Pos_y > -3600)
			pEm->nFloor = 0;

	}

	if (*(signed char *)&pEm->Timer2) {
		(*(signed char *)&pEm->Timer2)--;

		Sv0.vx = 1900;
		Sv0.vy = 256 - Rnd() * 2;
		Sv0.vz = 256 - Rnd() * 2;
		pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
		Esp_call((Rnd() << 2) | 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		Sv0.vx = 2000;
		Esp_call((Rnd() << 2) | 0x11000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		pP_ptr = G.Player.pSin_parts_ptr;
		Sv0.vx = 256 - Rnd() * 2;
		Sv0.vy = -200;
		Sv0.vz = 256 - Rnd() * 2;
		Esp_call((Rnd() << 2) | 0x1000, 0, &pP_ptr->Workm, &Sv0);
	}

	pEm->Pos_x += pEm->Spd_x;
	pEm->Pos_z += pEm->Spd_z;

}

static void Em34_mv06_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x3020F;
		pEm->Routine_2 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFB;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFF7;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFEF;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFDF;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFF7F;
		if (pEm->Ground == -3600) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x20;

			pEm->Atd[ 0 ].At_w = 500;
			pEm->Atd[ 0 ].At_d = 500;
			pEm->Atd[ 0 ].W = 500;
			pEm->Atd[ 0 ].D = 500;

			Snd_se_enem(4, pEm);
		}else {
			Snd_se_enem(3, pEm);
		}
		Pad_vib_set1(8, 100, 0); /* retail-only Dual Shock vibration: Pad_vib_set0/Pad_vib_set1/Pad_vib_fade_set 0x8003947C/0x80039514/0x800395B8 (config/retail/main.symbols.txt; references/bio2-ds/Bio2/prog/PAD.C:309,325,341); implicit declarations as in this TU */
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x400))

			pEm->Routine_2 = 2;
		break;

	case 2:
		*(unsigned long *)&pEm->Routine_0 = 0x301;
		((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 90;

		if ((((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 2) && !(Rnd() & 3))
			*(unsigned long *)&pEm->Routine_0 = 0xA01;
	}

}

static void Em34_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em34_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Ck;
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;
	register PARTS_W *pP;	register short Dir;	register short Ck1;	/* inferred names: no retail record */
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0007;
		pEm->Timer3 = 0;
		pEm->Routine_2 = 1;
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_dir == Em34_mv05_dir[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_no ]) {
			*(unsigned long *)&pEm->Move_no = 0xF0010;
			pEm->Timer3 = 1;
		}
		pEm->Spd_x = 400;
		pEm->Spd_z = 0;
		pEm->nFloor = 0;

		pEm->Timer2 = 0;

	case 1:
		if (pEm->Move_cnt < 15) {
			Goto01(pEm, ((EM34_FREE *)EM_FREE_AREA(pEm))->Kabe_dir, 0x80);
			Add_speedXZ(pEm, 0);
			pEm->Spd_x -= 40;
			if (pEm->Spd_x < 0)
				pEm->Spd_x = 0;
		}

		if (pEm->Move_cnt >= 14 && pEm->Move_cnt < 22)
			pEm->Pos_y += 20 + (Rnd() & 0x7F);

		if (pEm->Move_cnt >= 22) {
			Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
			if (pEm->Timer3 && Ck > 0)
				pEm->Cdir_y += Ck;

			if (!pEm->Timer3 && Ck < 0)
				pEm->Cdir_y += Ck;

			pEm->Spd_x += 100;
		}

		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_2 = 2;
			pEm->Spd_y = 100;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFEF;
			*(unsigned long *)&pEm->Move_no = 0x3000E;
		}
		break;

	case 2:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		Joint_move(pEm, Kan, Seq, 0x400);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 120 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;

		pEm->Pos_x += pEm->Spd_x;	/* retail-only: wall check at the next position */
		pEm->Pos_z += pEm->Spd_z;
		Sca_ck_em(pEm, 0x400);
		pEm->Pos_x -= pEm->Spd_x;
		pEm->Pos_z -= pEm->Spd_z;

		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			*(unsigned long *)&pEm->Routine_0 = 0x601;
		}

		if (pEm->Pos_y > -1500) {
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 1000;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&pP->Workm, &G.Player.Pos_x, 2000, &Sv0) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

				Ck1 = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (Pl_life_down(30, 0) != 2) {

					*(unsigned long *)&G.Player.Routine_0 = ((Ck1 | 2) << 8) + 2;
				}else {

					*(unsigned long *)&G.Player.Routine_0 = 3;
					pEm->Damage_cnt |= 0x80;
				}

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				pEm->Timer2 = 6;
				Snd_se_enem(6, pEm);

				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 2;
				Pad_vib_set0(8, 0);	/* retail-only Dual Shock vibration */
				Pad_vib_set1(13, 250, 2);
			}
		}

	}

	if (*(signed char *)&pEm->Timer2) {
		(*(signed char *)&pEm->Timer2)--;

		Sv0.vx = 1900;
		Sv0.vy = 256 - Rnd() * 2;
		Sv0.vz = 256 - Rnd() * 2;
		pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
		Esp_call((Rnd() << 2) | 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		Sv0.vx = 2000;
		Esp_call((Rnd() << 2) | 0x11000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		pP_ptr = G.Player.pSin_parts_ptr;
		Sv0.vx = 256 - Rnd() * 2;
		Sv0.vy = -200;
		Sv0.vz = 256 - Rnd() * 2;
		Esp_call((Rnd() << 2) | 0x1000, 0, &pP_ptr->Workm, &Sv0);
	}

}
static void Em34_mv08_br(ENEMY_WORK *pEm)
{
	register short Dir1;	/* inferred name: no retail record */
	register unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;

	LL = pEm->L_pl;

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1524);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (!Dir1 && LL > 2500 && LL < 4000 && !((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer)
		*(unsigned long *)&pEm->Routine_0 = 0x201;

	if (pEm->Life < 700) {
		*(unsigned long *)&pEm->Routine_0 = 1;
		pEm->Damage_cnt |= 0x80;
	}

}

static void Em34_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 40;
	case 1:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
		if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x338))
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);

		Spd_get(pEm, Kan, Seq, 1);
		if (pEm->Hokan_flg > 20)
			pEm->Spd_x >>= 1;

		Joint_move(pEm, Kan, Seq, 0x10080);
		Add_speedXZ(pEm, 0);

		if ((unsigned short)pEm->Timer0-- == 0)	/* cast keeps the old-value test, as retail */
			*(unsigned long *)&pEm->Routine_0 = 0x101;

	}
}

static void Em34_mv09_br(ENEMY_WORK *pEm)
{

}

static void Em34_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF000A;
		pEm->Routine_2 = 1;
	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100))
			*(unsigned long *)&pEm->Routine_0 = 0x301;

		Add_speedXZ(pEm, 0);

	}
}

static void Em34_mv0a_br(ENEMY_WORK *pEm)
{
}

static void Em34_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0012;
		pEm->Routine_2 = 1;
	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Routine_0 = 0x301;
			if (G.Player.Life < 0) *(unsigned long *)&pEm->Routine_0 = 0xA01;
		}
		Add_speedXZ(pEm, 0);

	}
}

static void Em34_mv0b_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;
	register short Ck;
	SVECTOR Sv;
	register unsigned long X;
	register unsigned long Z;
	VECTOR Tv;
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0013;
		pEm->Routine_2 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 4;

		pEm->Timer3 = 0;
		Pad_vib_set1(8, 80, 15);	/* retail-only Dual Shock vibration */
	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);

		if (pEm->Move_cnt == 18) {

			Snd_se_enem(2, pEm);

			pEm->Routine_2 = 2;

			Sv.vx = 550;
			Sv.vy = -400;
			Sv.vz = 0;

			Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

			switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r) {	/* the (short) casts below keep retail's ( Dir + K ) - n association */
			case 0:
				break;

			case 1:
				Dir = (short)(Dir + 0x200) - (((Rnd() >> (Rnd() & 3)) & 1) << 10);
				break;

			case 2:
				Dir = (short)(Dir + 0x400) - (((Rnd() >> (Rnd() & 3)) & 1) << 11);
				break;

			case 3:
				Dir = (short)(Dir + 0xA00) - (((Rnd() >> (Rnd() & 3)) & 1) << 10);
				break;

			case 4:
				Dir += 0x800;	break;

			case 5:
				pEm->Timer3 = (Em34_pl_downpos_no + (Rnd() % 3 + 3)) & 7;

				if (Em34_pl_downpos_no >= 8)
					pEm->Timer3 += 8;

				X = pEm->Pos_x - Em34_mv10_downpos[ pEm->Timer3 ][ 0 ];
				Z = pEm->Pos_z - Em34_mv10_downpos[ pEm->Timer3 ][ 1 ];
				X = SquareRoot0(X * X + Z * Z);
				Sv.vx = X / 5;
				Sv.vy = -600;
				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, Em34_mv10_downpos[ pEm->Timer3 ][ 0 ], Em34_mv10_downpos[ pEm->Timer3 ][ 1 ]);
				break;
			}

			Em34_dir_spd(pEm, Dir, &Sv);

			pEm->Timer0 = (short)(pEm->Spd_x / 5) * -1;	/* "* -1" on the short quotient: retail sign-extends it before negating */
			pEm->Timer1 = (short)(pEm->Spd_z / 5) * -1;

			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x80;
		}

		break;

	case 2:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

	case 3:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 140 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;

		pEm->Pos_x += pEm->Spd_x;	/* retail-only: wall check at the next position */
		pEm->Pos_z += pEm->Spd_z;
		Sca_ck_em(pEm, 0x400);
		pEm->Pos_x -= pEm->Spd_x;
		pEm->Pos_z -= pEm->Spd_z;

		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			pEm->Routine_2 = 4;

			*(unsigned long *)&pEm->Move_no = 0x80614;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFF7F;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFB;

			if (!pEm->Pos_y) {
				Snd_se_enem(3, pEm);
			}else {
				Snd_se_enem(4, pEm);
			}
			Pad_vib_set1(8, 100, 0);	/* retail-only Dual Shock vibration */
		}

		Tv.vx = pEm->Atd[ 0 ].X;
		Tv.vy = pEm->Atd[ 0 ].Y;
		Tv.vz = pEm->Atd[ 0 ].Z;
		Ck = Sca_get_ground(&Tv, 1400, 0x400, 0);
		if (Ground_in_flg)
			Ck = Ground_in_pos;

		if (!Ck) {
			pEm->nFloor = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFDF;
		}

		if (pEm->Pos_y > -3600) {
			pEm->nFloor = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFDF;
		}

		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		break;

	case 4:
		pEm->Spd_x += pEm->Timer0;
		if ((pEm->Spd_x & 0x8000) == (pEm->Timer0 & 0x8000))
			pEm->Spd_x = 0;

		pEm->Spd_z += pEm->Timer1;
		if ((pEm->Spd_z & 0x8000) == (pEm->Timer1 & 0x8000))
			pEm->Spd_z = 0;

		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(unsigned long *)&pEm->Routine_0 = 0x301;

			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer1 = 60;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 0;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r == 5)
				*(unsigned long *)&pEm->Routine_0 = 0x401;
		}

		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
	}
}

static void Em34_mv0c_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0015;
		pEm->Routine_2 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFD;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_2 = 2;
			*(unsigned long *)&pEm->Move_no = 0xF0016;
			pEm->Timer0 = 30;
		}
		Add_speedXZ(pEm, 0);
		break;

	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		if ((unsigned short)pEm->Timer0-- == 0) {	/* cast keeps the old-value test, as retail */
			pEm->Routine_2 = 3;
			if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400)) {
				*(unsigned long *)&pEm->Routine_0 = 0xB01;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 4;
			}
		}
		break;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0x70806;
		Spd_get(pEm, Kan, Seq, 0);
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu = 0;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
		Joint_move(pEm, Kan, Seq, 0x200);
		*(unsigned long *)&pEm->Routine_0 = 0x10501;
		pEm->Timer2 = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x80;

		Snd_se_enem(2, pEm);
		Pad_vib_set1(8, 80, 0); /* retail-only Dual Shock vibration */
	}
}

static void Em34_mv0d_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x17;
		Joint_move(pEm, Kan, Seq, 0x100);

		if (pEm->Sce_free0 & 1) {
			pEm->Routine_2 = 1;
			((KAGE_WORK9 *)pEm->pKage_work)->Be_flg = 7;
			pEm->Type &= ~0x4000;
		}

		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
		pEm->Timer0 = 0;
		pEm->nFloor = 0;
		break;

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

	case 2:
		pEm->Spd_y += 30 - pEm->Timer0;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Timer0--;
		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Routine_2 = 3;
			*(unsigned long *)&pEm->Move_no = 0x30519;
			Spd_get(pEm, Kan, Seq, 0);
		}
		break;

	case 3:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0);

		break;

	case 4:
		*(unsigned long *)&pEm->Move_no = 0x70009;
		pEm->Routine_2 = 5;

	case 5:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		Add_speedXZ(pEm, 0);
		break;

	case 6:
		*(unsigned long *)&pEm->Routine_0 = 0x101;
		((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 30;
	}
}

static void Em34_mv0e_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;
	register short Ck;
	register PARTS_W *pP;	register short Dir;	/* inferred names: no retail record */
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF001A;
		pEm->Routine_2 = 1;

		pEm->Timer3 = 0;
		pEm->Timer2 = 0;	pEm->Timer0 = 0;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Routine_0 = 0x301;

			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 15;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 2) {
				((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = (Rnd() & 0xF) + 15;
				((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 90;

				if ((Rnd() & 1) && pEm->L_pl < 3600 && ((EM34_FREE *)EM_FREE_AREA(pEm))->Cu_cnt >= 2) {
					*(unsigned long *)&pEm->Routine_0 = 0xB01;
					((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = (Rnd() & 1) + 3;
				}
			}
		}

		if (*pEm->pNow_seq & 0x10000) {

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);

			pP = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 1400;
			Sv0.vy = 0;
			Sv0.vz = 0;
			if (L_pos_ckXZM(&pP->Workm, &G.Player.Pos_x, 1500, &Sv0) && !G.Player.Damage_cnt) {

				Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - 0x400;

				Ck = ((G.Player.Cdir_y - Dir + 0x400) & 0xFFF) < 0x800;

				if (pEm->Timer2)
					Ck ^= 1;

				if (Pl_life_down(20, 0) != 2) {

					*(unsigned long *)&G.Player.Routine_0 = ((Ck | 2) << 8) + 2;
				}else {

					*(unsigned long *)&G.Player.Routine_0 = 3;
					pEm->Damage_cnt |= 0x80;
				}

				G.Player.Damage_cnt |= 0x80;
				G.Player.pEnemy_ptr = (unsigned long *)pEm;

				pEm->Timer3 = 6;
				pEm->Timer0 = (signed char)pEm->Timer2;

				Snd_se_enem(6, pEm);
				Pad_vib_set0(8, 0); /* retail-only Dual Shock vibration */
				Pad_vib_set1(5, 150, 4);
			}
		}

		if (*(signed char *)&pEm->Timer3) {
			(*(signed char *)&pEm->Timer3)--;

			Sv0.vx = 1900;
			Sv0.vy = 256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			Esp_call((Rnd() << 2) | 0x1000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			Sv0.vx = 2000;
			Esp_call((Rnd() << 2) | 0x11000, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			pP_ptr = G.Player.pSin_parts_ptr;
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = -200;
			Sv0.vz = 256 - Rnd() * 2;
			Esp_call((Rnd() << 2) | 0x1000, 0, &pP_ptr->Workm, &Sv0);
		}

		Add_speedXZ(pEm, 0);

	}
}

static void Em34_mv0f_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv0;
	register PARTS_W *pP;	/* inferred name: no retail record */
	register long Dm;	/* retail-only damage carrier, [INFERRED] name (no retail SYM); the direct conditional argument changes the base-register web */

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x7001B;
		pEm->Routine_2 = 1;

		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);

		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		Set_base_pos(&G.Player, Kan, Seq, 0);

		*(unsigned long *)&G.Player.Routine_0 = 5;
		pEm->Timer3 = 0;	/* retail-only: set when this bite kills */
		Dm = 70;
		if (G.Player.Life > 170) Dm = G.Player.Life - 100;
		if (Pl_life_down(Dm, 0) == 2) {
			*(unsigned long *)&G.Player.Routine_0 = 0x206;
			pEm->Damage_cnt |= 0x80;
			G.System_flg |= 0x100;
			pEm->Timer3 = 1;
		}

		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

		G.Player.Damage_cnt |= 0x80;
		pEm->Damage_cnt |= 0x80;

		pEm->Be_flg |= 0x1000;
		G.Player.Be_flg |= 0x1000;

		G.Player.Timer1 = pEm->Cdir_y + 0x800;
		Pad_vib_set1(20, 120, 0);	/* retail-only Dual Shock vibration */
	case 1:
		G.Player.Base_pos_x = pEm->Base_pos_x;
		G.Player.Base_pos_z = pEm->Base_pos_z;

		Null_pos_set(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(unsigned long *)&pEm->Routine_0 = 0x301;
			((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 150;
			((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 150;
			EM34_RETAIL_24(pEm) = 150;
			if (pEm->Timer3 & 1) {
				((EM34_FREE *)EM_FREE_AREA(pEm))->At_timer = 250;
				((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer0 = 250;
			}
			pEm->Damage_cnt &= 0x7F;
			pEm->Be_flg &= 0xFFFFEFFF;
		}

		if (pEm->Move_cnt == 8) {
			Sv0.vx = 1900;
			Sv0.vy = 256;
			Sv0.vz = 0;
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Esp_call(0x1BB8, pEm->Cdir_y, &pP->Workm, &Sv0);
			Sv0.vx = 2000;
			Sv0.vy = 0;
			Esp_call(0x11FA0, pEm->Cdir_y, &pP->Workm, &Sv0);
			Pad_vib_set0(6, 0);	/* retail-only Dual Shock vibration sequence */
			Pad_vib_set1(8, 250, 3);
			Pad_vib_set0(3, 15);
			Pad_vib_set0(2, 20);
			Pad_vib_set1(70, 100, 11);
			Pad_vib_set0(3, 25);
			Pad_vib_set1(13, 250, 25);
			Pad_vib_set0(3, 40);
			Pad_vib_set0(3, 45);
			Pad_vib_set0(3, 50);
			Pad_vib_set1(13, 250, 50);
			Pad_vib_set0(3, 65);
			Pad_vib_set0(10, 70);
			Pad_vib_set1(8, 250, 70);
		}

		if ((pEm->Timer3 & 1) && pEm->Move_cnt == 30) {	/* retail-only */
			pP = &pEm->pSin_parts_ptr[ 10 ];
			pP->Be_flg |= 0x80;
			*(long *)&pP->Free[ 0 ] = 0x30;
			*(long *)&pP->Free[ 2 ] = 0x202020;
			Sv0.vx = 2156 - Rnd() * 2;
			Sv0.vy = 256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Esp_call(Rnd() * 2 + 0x1FA0, pEm->Cdir_y, &pP->Workm, &Sv0);
		}

		if (pEm->Move_cnt >= 9) {
			Sv0.vx = 2156 - Rnd() * 2;
			Sv0.vy = 256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP = &pEm->pSin_parts_ptr[ 10 ];
			Esp_call(Rnd() * 2 + 0x830, pEm->Cdir_y, &pP->Workm, &Sv0);

			Sv0.vx = 2256 - Rnd() * 2;
			Esp_call((Rnd() * 2 + 0x830) | 0x10000, pEm->Cdir_y, &pP->Workm, &Sv0);

		}
	}
}

static void Em34_mv10_br(ENEMY_WORK *pEm)
{

}
static void Em34_mv10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	VECTOR Tv;
	SVECTOR Sv;
	SVECTOR Sv0;
	register unsigned long X;
	register unsigned long Z;
	register short D_x;	register short D_z;	register short Dir;	/* Dir: inferred name, no retail record */
	if (!pEm->Routine_2)

		pEm->Timer2 = Em34_uppos_no[ Rnd() % Em34_uppos ];

	D_x = Em34_mv10_pos[ pEm->Timer2 ][ 0 ];
	D_z = Em34_mv10_pos[ pEm->Timer2 ][ 1 ];

	pEm->Timer0++;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70006;
		pEm->Routine_2 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu = 0;

		pEm->Timer3 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Up_cnt = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFD;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer = 0;
		Pad_vib_set1(8, 80, 15);	/* retail-only Dual Shock vibration */
	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, D_x, D_z, 0x10);
		Joint_move(pEm, Kan, Seq, 0x200);

		if (pEm->Move_cnt == 19) {
			pEm->Routine_2 = 2;

			pEm->Spd_y = -1100;
			pEm->Timer3 = 18;

			if (pEm->Routine_1 == 17) {
				pEm->Spd_y = -800;
				pEm->Timer3 = 16;
			}
			pEm->Spd_z = 0;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 4;
			pEm->Timer0 = 0;
			Goto00(pEm, D_x, D_z, 0x20);

			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x80;

			Snd_se_enem(2, pEm);
		}
		break;

	case 2:
		Goto00(pEm, D_x, D_z, 0x40);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 120 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;

		break;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0xF000D;
		pEm->Routine_2 = 4;

	case 4:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x10)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);

		Joint_move(pEm, Kan, Seq, 0x100);

		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 120 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Jyu += 2;

		if (pEm->Spd_y > 0 && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x10)) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x10;
			*(unsigned long *)&pEm->Move_no = 0xF000E;
		}

		pEm->Pos_x += pEm->Spd_x;	/* retail-only: wall check at the next position */
		pEm->Pos_z += pEm->Spd_z;
		Sca_ck_em(pEm, 0x400);
		pEm->Pos_x -= pEm->Spd_x;
		pEm->Pos_z -= pEm->Spd_z;

		if (pEm->Pos_y > pEm->Ground) {
			pEm->Pos_y = pEm->Ground;
			pEm->Spd_y = 0;
			*(unsigned long *)&pEm->Routine_0 = 0x601;

			if (pEm->Ground == -3600)
				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x20;
			Pad_vib_set1(8, 100, 0);	/* retail-only Dual Shock vibration */
		}

		if (pEm->Pos_y < -3800)
			pEm->nFloor = 2;

		if (pEm->Pos_y > -3600)
			pEm->nFloor = 0;

	}

	if (*(signed char *)&pEm->Timer3) {
		X = D_x - pEm->Pos_x;
		Z = D_z - pEm->Pos_z;
		X = SquareRoot0(X * X + Z * Z);

		Sv.vx = X / pEm->Timer3;
		Sv.vy = pEm->Spd_y;
		Sv.vz = 0;

		Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, D_x, D_z);

		Em34_dir_spd(pEm, Dir, &Sv);
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		(*(signed char *)&pEm->Timer3)--;
	}

}
static int Em34_Tume_color(ENEMY_WORK *pEm, unsigned long Flg)
{
	register PARTS_W *pP_ptr;

	pP_ptr = &pEm->pSin_parts_ptr[ 19 - Flg * 5 ];
	pP_ptr->Be_flg |= 0x80;
	*(long *)&pP_ptr->Free[ 0 ] = 0x40;
	*(long *)&pP_ptr->Free[ 2 ] = 0x30;

}

static void Em34_firedm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 10 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 5 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 1 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 15 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 12 ].Workm, 0);

}

static void Em34_ryudm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 10 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 5 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 26 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 16 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 1 ].Workm, 0);

}

static void Em34_bomdm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 10 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1BB8) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 15 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 5 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 2 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x830) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x448) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 15 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x448) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 4 ].Workm, 0);
}

/* retail-only (retail 0x80106004 / 0x80113004, 167 words; no Beta twin, name unknown): effect
   burst of the damage arm Routine_1 == 17 in Em34_Damage_34, same shape as Em34_bomdm. */
static void func_80106004(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x46B0) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 10 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x3AF8) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2388) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 15 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 5 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 1 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 11 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x3AF8) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 26 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 23 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[ 20 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 2 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 15 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 4 ].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[ 26 ].Workm, 0);
}
static void Em34_spdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;	register PARTS_W *pP;	/* pP: inferred name, no retail record */

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = -472 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 26 ].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 26 ].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 0 ].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 1 ].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 12 ].Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 13 ].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 10 ].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 4 ].Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 128 - Rnd();
	Sv.vy = 128 - Rnd();
	Sv.vz = 128 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr[ 16 ].Workm, &Sv);

	pP = &pEm->pSin_parts_ptr[ 1 ];
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);
	pP = &pEm->pSin_parts_ptr[ 12 ];
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);
	pP = &pEm->pSin_parts_ptr[ 4 ];
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);

}
static void Em34_mgdm(ENEMY_WORK *pEm)
{
	SVECTOR Sv0;
	SVECTOR Sv1;
	SVECTOR Sv2;
	MATRIX M;
	register short Dir;	register PARTS_W *pP;	/* inferred names: no retail record */
	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0) {
	case 0:	break;	case 1:	/* inferred empty case 0: retail's compare tree tests < 2 */

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0 = 2;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

	case 2:

		Sv0.vx = 0;
		Dir = Sv0.vy = ((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_dir;
		Sv0.vz = 0;
		RotMatrix(&Sv0, &M);

		Sv0.vx = -((signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 100);
		Sv0.vy = 200;
		ApplyMatrixSV(&M, &Sv0, &Sv1);

		Sv0.vx = -100;
		Sv0.vy = -500;
		Sv0.vz = 0;

		if (!(pEm->Damage_no % 3))
			Sv0.vy = 500;

		ApplyMatrixSV(&M, &Sv0, &Sv2);

		pP = &pEm->pSin_parts_ptr[ 9 ];
		Sv0.vx = Sv1.vx + pEm->Pos_x;
		Sv0.vy = Sv1.vy + pP->Workm.t[ 1 ];
		Sv0.vz = Sv1.vz + pEm->Pos_z;
		Esp_call(((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx - (Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz - (Rnd() - 0x80);
		Esp_call(((1 - (Rnd() & 1)) << 16) | ((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36), Dir, &GsIDMATRIX, &Sv0);

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 == 1) {
			Esp_call(0x217D0, Dir + 0x800, &GsIDMATRIX, &Sv0);
			Esp_call(0x20C18, Dir + 0x800, &GsIDMATRIX, &Sv0);
		}

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 1);
		Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 1);
		Esp_call(((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 3 - (Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 3 - (Rnd() - 0x80);
		Esp_call(((1 - (Rnd() & 1)) << 16) | ((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36), Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 2);
		Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 2);
		Esp_call(((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 5 - (Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 5 - (Rnd() - 0x80);
		Esp_call(((1 - (Rnd() & 1)) << 16) | ((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36), Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 6;
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 6;
		Esp_call(((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 36) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 7 - (Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 7 - (Rnd() - 0x80);
		Esp_call(((1 - (Rnd() & 1)) << 16) | ((short)(0x65C - Rnd()) - (signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0 * 30), Dir, &GsIDMATRIX, &Sv0);

		if ((signed char)((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_timer0++ > 16)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0 = 0;

	}

}
static void Em34_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em34_Damage_type[ pEm->Type & 1 ](pEm, Kan, Seq);	}

static void Em34_Damage_33(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;
	SVECTOR Sv0;
	register short Ck;
	MATRIX M;
	register PARTS_W *pP;	/* inferred name: no retail record */

	if (!pEm->Routine_2) {

		pP = &pEm->pSin_parts_ptr[ 9 ];
		Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Ck;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 200;
		Sv.vy = -200;
		Sv0.vx = (pEm->Spd_x << 2) + 300;
		Sv0.vy = 0;
		Sv0.vz = 0;
		if (!(pEm->Damage_no % 3)) {
			Sv.vx = 150;
			Sv.vy = 800;
			Sv0.vx = (pEm->Spd_x << 1) + 150;
			Sv0.vy = 700;
		}
		if ((pEm->Damage_no % 3) == 2) {
			Sv.vx = 200;
			Sv.vy = -700;
			Sv0.vx = (pEm->Spd_x << 1) + 200;
			Sv0.vy = -700;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP->Workm.t[ 0 ];
		Sv.vy += pP->Workm.t[ 1 ];
		Sv.vz += pP->Workm.t[ 2 ];
		ApplyMatrixSV(&M, &Sv0, &Sv0);
		Sv0.vx += pP->Workm.t[ 0 ];
		Sv0.vy += pP->Workm.t[ 1 ];
		Sv0.vz += pP->Workm.t[ 2 ];

		if (pEm->Routine_1 != 16 && pEm->Routine_1 != 12) {
			Esp_call(((Rnd() << 3) + 0x17D0) | 0x10000, Ck, &GsIDMATRIX, &Sv);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv);

			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);

			if (pEm->Routine_1 != 1) {
				Snd_se_enem(2, pEm);
			}
		}

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0 = 1;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em34_bomdm(pEm);

			if (pEm->Routine_1 == 17)	/* retail-only damage arm */
				func_80106004(pEm);

			if (pEm->Routine_1 == 10)
				Em34_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em34_ryudm(pEm);
		}

		if (pEm->Routine_1 == 14)
			Em34_spdm(pEm);

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x400) {	/* retail-only gate (Em34_dm00 sets 0x400) */
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em34_dm00(pEm, Kan, Seq);
			return;
		}else {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt += Em34_Dm_cnt[ pEm->Routine_1 - 1 ];

			if (!(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 1)) {
				((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 1;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0 = 15;
			}

			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt < 12) {
				*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
				Em34_Move(pEm, Kan, Seq);
				return;
			}
			((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFFFE;
		}
	}

	Em34_dm00(pEm, Kan, Seq);

}

static void Em34_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70508;
		pEm->Routine_2 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x400;

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			pEm->Damage_cnt &= 0x7F;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= ~0x400;
		}

	}	}
static void Em34_Damage_34(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;
	SVECTOR Sv0;
	register short Ck;
	MATRIX M;
	register PARTS_W *pP;	/* inferred name: no retail record */

	if (!pEm->Routine_2) {

		pP = &pEm->pSin_parts_ptr[ 10 ];
		Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Ck;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 200;
		Sv.vy = -200;
		Sv0.vx = (pEm->Spd_x << 2) + 300;
		Sv0.vy = 0;
		Sv0.vz = 0;
		if (!(pEm->Damage_no % 3)) {
			Sv.vx = 150;
			Sv.vy = 800;
			Sv0.vx = (pEm->Spd_x << 1) + 150;
			Sv0.vy = 700;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP->Workm.t[ 0 ];
		Sv.vy += pP->Workm.t[ 1 ];
		Sv.vz += pP->Workm.t[ 2 ];
		ApplyMatrixSV(&M, &Sv0, &Sv0);
		Sv0.vx += pP->Workm.t[ 0 ];
		Sv0.vy += pP->Workm.t[ 1 ];
		Sv0.vz += pP->Workm.t[ 2 ];

		if (pEm->Routine_1 != 16 && pEm->Routine_1 != 12) {
			Esp_call(((Rnd() << 3) + 0x17D0) | 0x10000, Ck, &GsIDMATRIX, &Sv);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv);

			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);

			if (pEm->Routine_1 != 1) {
				Snd_se_enem(13, pEm);
			}
		}

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0 = 1;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em34_bomdm(pEm);

			if (pEm->Routine_1 == 17)	/* retail-only damage arm */
				func_80106004(pEm);

			if (pEm->Routine_1 == 10)
				Em34_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em34_ryudm(pEm);
		}

		if (pEm->Routine_1 == 14)
			Em34_spdm(pEm);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_cnt += Em34_Dm_cnt[ pEm->Routine_1 - 1 ];

		if (!(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 1)) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 1;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Dm_timer0 = 15;
		}

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_timer += 20;

		*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;

		if (!((EM34_FREE *)EM_FREE_AREA(pEm))->J_timer1 && !pEm->Pos_y && !(Rnd() & 3) && !(((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 4)) {
			*(unsigned long *)&pEm->Routine_0 = 0xB01;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 1;
			if (!(Rnd() & 3))
				((EM34_FREE *)EM_FREE_AREA(pEm))->Tj_r = 4;

			if (!(Rnd() & 3))
				*(unsigned long *)&pEm->Routine_0 = 0x501;
		}

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x200;

		Em34_Move(pEm, Kan, Seq);

		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg &= 0xFDFF;
	}

}

static void Em34_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em34_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em34_die00(pEm, Kan, Seq);	}

static void Em34_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	short Em34_die00_hen[ 8 ] = { 31, 40, 22, -20, -30, 10, 22, 0 };
	SVECTOR Sv;	/* retail-only locals of the replayed hit effect (Em34_Damage_34 names); frame order hen/Sv/Sv0/M as retail */
	SVECTOR Sv0;
	register short Ck;
	MATRIX M;
	register PARTS_W *pP;	/* inferred name: no retail record */

	((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x40;

	switch (pEm->Routine_2) {
	case 0:
		Em_dead_flg_on(pEm);	/* retail-only: the death start replays the Em34_Damage_34 hit effect */

		pP = &pEm->pSin_parts_ptr[ 10 ];
		Ck = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vx = 0;
		Sv.vy = Ck;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv.vx = 200;
		Sv.vy = -200;
		Sv0.vx = (pEm->Spd_x << 2) + 300;
		Sv0.vy = 0;
		Sv0.vz = 0;
		if (!(pEm->Damage_no % 3)) {
			Sv.vx = 150;
			Sv.vy = 800;
			Sv0.vx = (pEm->Spd_x << 1) + 150;
			Sv0.vy = 700;
		}
		ApplyMatrixSV(&M, &Sv, &Sv);
		Sv.vx += pP->Workm.t[ 0 ];
		Sv.vy += pP->Workm.t[ 1 ];
		Sv.vz += pP->Workm.t[ 2 ];
		ApplyMatrixSV(&M, &Sv0, &Sv0);
		Sv0.vx += pP->Workm.t[ 0 ];
		Sv0.vy += pP->Workm.t[ 1 ];
		Sv0.vz += pP->Workm.t[ 2 ];

		if (pEm->Routine_1 != 16 && pEm->Routine_1 != 12) {
			Esp_call(((Rnd() << 3) + 0x17D0) | 0x10000, Ck, &GsIDMATRIX, &Sv);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv);

			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);
			Sv.vy -= (unsigned char)(Rnd() & 0x3F) - 0x40;
			Esp_call((Rnd() << 3) + 0x17D0, Ck, &GsIDMATRIX, &Sv0);

			if (pEm->Routine_1 != 1) {
				Snd_se_enem(13, pEm);
			}
		}

		if (pEm->Routine_1 == 5 || pEm->Routine_1 == 6)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Mg_rout0 = 1;

		if (pEm->Damage_no < 3) {
			if (pEm->Routine_1 == 9)
				Em34_bomdm(pEm);

			if (pEm->Routine_1 == 17)
				func_80106004(pEm);

			if (pEm->Routine_1 == 10)
				Em34_firedm(pEm);

			if (pEm->Routine_1 == 11)
				Em34_ryudm(pEm);
		}

		if (pEm->Routine_1 == 14)
			Em34_spdm(pEm);

		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x80) {
			*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
			Em34_Move(pEm, Kan, Seq);
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x100;
			break;
		}

		pEm->Sce_free0 |= 4;

		*(unsigned long *)&pEm->Move_no = 0x70011;
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x1000;
		D_8010B118 = 0x808080;
		pEm->Be_flg |= 2;
		pEm->Neck_flg |= 2;

		pP_ptr = &pEm->pSin_parts_ptr[ 0 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 9 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 5 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 6 ];
		pP_ptr->Be_flg |= 0x400; pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000;
		pP_ptr = &pEm->pSin_parts_ptr[ 7 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 1 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 2 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;
		pP_ptr = &pEm->pSin_parts_ptr[ 3 ];
		pP_ptr->Size_x = 0x1000; pP_ptr->Size_y = 0x1000; pP_ptr->Size_z = 0x1000; pP_ptr->Be_flg |= 0x400;

	case 1:
		if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 90) {
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 925 - Rnd() * 2;
			Sv0.vy = 125 - Rnd() * 2;
			Sv0.vz = 125 - Rnd() * 2;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x13E8), Rnd() << 4, &pP_ptr->Workm, &Sv0);
			if (!G.SaveData.Sce_timer) {
				Sv0.vx = 925 - Rnd() * 2;
				Sv0.vy = 125 - Rnd() * 2;
				Sv0.vz = 125 - Rnd() * 2;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0xC18), Rnd() << 4, &pP_ptr->Workm, &Sv0);
			}
		}

		if (pEm->Move_cnt >= 91 && !(pEm->Move_cnt & 3)) {
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 925 - Rnd() * 2;
			Sv0.vy = 125 - Rnd() * 2;
			Sv0.vz = 125 - Rnd() * 2;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1000), Rnd() << 4, &pP_ptr->Workm, &Sv0);
			if (!G.SaveData.Sce_timer) {
				Sv0.vx = 925 - Rnd() * 2;
				Sv0.vy = 125 - Rnd() * 2;
				Sv0.vz = 125 - Rnd() * 2;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x830), Rnd() << 4, &pP_ptr->Workm, &Sv0);
			}
		}

		if ((pEm->Move_cnt >= 6 && pEm->Move_cnt < 35) || (pEm->Move_cnt >= 61 && pEm->Move_cnt < 90)) {
			pP_ptr = &pEm->pSin_parts_ptr[ 26 ];
			Sv0.vx = 300;
			Sv0.vy = 414 - Rnd();
			Sv0.vz = 64 - Rnd();
			Esp_call((((Rnd() & 1) + 16) << 16) | ((Rnd() << 2) + 0x17D0), Rnd() << 4, &pP_ptr->Workm, &Sv0);
		}

		if (pEm->Move_cnt >= 6 && !(pEm->Move_cnt & 3) && !G.SaveData.Sce_timer) {
			pP_ptr = &pEm->pSin_parts_ptr[ 26 ];
			Sv0.vx = 200;
			Sv0.vy = 414 - Rnd();
			Sv0.vz = 64 - Rnd();
			Esp_call((((Rnd() & 1) + 16) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP_ptr->Workm, &Sv0);
		}

		if (pEm->Move_cnt == 50) {
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
		}

		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_2 = 2;
			pEm->Parts0_pos_y = 0x1000;

			pEm->Timer2 = 0;
			pEm->Timer3 = (Rnd() & 0x1F) + 10;

			*(unsigned long *)&pEm->Move_no = 0x7001C;
			pEm->Timer1 = 0;
		}

		if (pEm->Move_cnt >= 61) {
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			((short *)pP_ptr->pSa_dat_head)[ 14 ] += 4;
			pEm->Parts0_pos_y -= 4;
			if (pEm->Parts0_pos_y < 0x800)
				pEm->Parts0_pos_y = 0x800;

			if (((short *)pP_ptr->pSa_dat_head)[ 14 ] > 0x800)
				((short *)pP_ptr->pSa_dat_head)[ 14 ] = 0x800;

			Sa_parts_allmv(pEm, pP_ptr);
			Sa_parts_offset_allmv(pEm, pP_ptr);
		}
		break;

	case 2:
		D_8010B118 += 0x100FF;	/* retail-only shadow fade */
		if ((D_8010B118 & 0xFF00) > 0xBF00)
			D_8010B118 = (D_8010B118 & 0xFF0000FF) | 0xBFBF00;
		if ((unsigned char)D_8010B118 < 0x10)
			D_8010B118 = (D_8010B118 & 0xFFFFFF00) | 0x10;
		Kage_work9_color_set(pEm->pKage_work, D_8010B118);

		pP_ptr = &pEm->pSin_parts_ptr[ 0 ];
		if (pP_ptr->Size_x > 128) {
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 9 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 5 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 6 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 7 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 1 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 2 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
			pP_ptr = &pEm->pSin_parts_ptr[ 3 ];
			pP_ptr->Size_x -= 8; pP_ptr->Size_y -= 8; pP_ptr->Size_z -= 8;
		}

		if (!(pEm->Timer1 & 7)) {
			pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
			Sv0.vx = 925 - Rnd() * 2;
			Sv0.vy = 125 - Rnd() * 2;
			Sv0.vz = 125 - Rnd() * 2;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1000), Rnd() << 4, &pP_ptr->Workm, &Sv0);
			Sv0.vx = 325 - Rnd() * 2;
			Sv0.vy = 125 - Rnd() * 2;
			Sv0.vz = 125 - Rnd() * 2;
			Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x1000), Rnd() << 4, &pP_ptr->Workm, &Sv0);
		}

		if (pEm->Timer1 < 30 && !(pEm->Timer1 & 3)) {
			pP_ptr = &pEm->pSin_parts_ptr[ 26 ];
			Sv0.vx = 200;
			Sv0.vy = 364 - Rnd();
			Sv0.vz = 64 - Rnd();
			Esp_call((((Rnd() & 1) + 16) << 16) | ((Rnd() << 2) + 0x1000), Rnd() << 4, &pP_ptr->Workm, &Sv0);
		}
		pEm->Timer1++;

		Joint_move(pEm, Kan, Seq, 0x200);

		pP_ptr = &pEm->pSin_parts_ptr[ 10 ];
		((short *)pP_ptr->pSa_dat_head)[ 14 ] += 2;

		if (pEm->Parts0_pos_y < 0x1000)
			pEm->Parts0_pos_y += 0x10;

		if (*(signed char *)&pEm->Timer3) {
			(*(signed char *)&pEm->Timer3)--;
		}else {
			((short *)pP_ptr->pSa_dat_head)[ 14 ] += Em34_die00_hen[ (signed char)pEm->Timer2 ] * 2;

			pEm->Timer2++;
			if ((signed char)pEm->Timer2 >= 8) {
				pEm->Timer2 = 0;
				pEm->Timer3 = (Rnd() & 0x1F) + 10;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[ 14 ] > 5096) {
			pEm->Routine_2 = 3;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg |= 0x800;
			Em34_core_set_dest(0, 5, 0);
			Em34_core_set_dest(1, 0, 5);
			Em34_core_set_dest(2, 7, -8);
		}

		Sa_parts_allmv(pEm, pP_ptr);
		Sa_parts_offset_allmv(pEm, pP_ptr);
		break;

	case 3:
		pEm->Move_cnt = 1;
		break;
	}
}

static void Em34_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em34_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em34_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em34_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em34_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em34_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

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

		break;

	case 3:
		*(unsigned long *)&pEm->Move_no = 2;
		pEm->Routine_2 = 4;

		pEm->Cdir_y += 300;

		pEm->Be_flg &= 0xFFFFFFF7;

	case 4:
		if (pEm->Move_cnt < 10) {
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = 256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | (Rnd() * 2 + 0x830), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		}

		Spd_get(pEm, Kan, Seq, 0);
		pEm->Spd_x += pEm->Spd_x >> 1;
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0x800);

		if (pEm->Move_cnt == 3) {
			Snd_se_stad(0x4020001, &pEm->Pos_x);
		}

		if (*pEm->pNow_seq & 0x10000) {
			Snd_se_walk(2, 0, pEm);
			Pad_vib_set0(3, 0); /* retail-only Dual Shock vibration */
			Pad_vib_set1(6, 250, 0);
		}

		break;

	case 5:
		pEm->Routine_2 = 6;
		*(unsigned long *)&pEm->Move_no = 0x70006;

	case 6:
		if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10200)) {

			*(unsigned long *)&pEm->Routine_0 = 1;

			pEm->Damage_cnt &= 0x7F;
			pEm->Neck_flg = 0;

			G.Status_flg &= 0xFFFFFFBF;
		}
	}
}

static void Em34_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em34_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em34_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;	register PARTS_W *pO;
	SVECTOR Sv0;
	register PARTS_W *pP;	/* inferred name: no retail record */
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70000;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		if (!pEm->Routine_1) {

			pP = &pEm->pSin_parts_ptr[ 8 ];
			Sv0.vx = pP->Pos_x;
			Sv0.vy = pP->Pos_y;
			Sv0.vz = pP->Pos_z;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(0x1F40, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
			Esp_call(0x21F40, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
			Esp_call(0x21F40, pEm->Cdir_y + 0x800, &pP_ptr->Workm, &Sv0);
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x40;
			*(long *)&pP_ptr->Free[ 2 ] = 0x30;

			pO = &G.pEnemy[ 2 ]->pSin_parts_ptr[ 18 - pEm->Routine_1 * 5 ];
			pP = &pEm->pSin_parts_ptr[ 8 ];
			pP->Be_flg |= 0x4000;
			pP->pSuper = (unsigned long *)&pO->Workm;
			pP->pOya_parts = (unsigned long *)pO;
			pP->Pos_x = 0;
			pP->Pos_y = 600;
			pP->Pos_z = 300 - pEm->Routine_1 * 600;
		}else {

			pP_ptr = &pEm->pSin_parts_ptr[ 1 ];
			Sv0.vx = 0;
			Sv0.vy = -300;
			Sv0.vz = 0;
			Esp_call(0x1F40, pEm->Cdir_y, &pP_ptr->Workm, &Sv0);
			Esp_call(0x21F40, pEm->Cdir_y, &pP_ptr->Workm, 0);
			Esp_call(0x21F40, pEm->Cdir_y + 0x800, &pP_ptr->Workm, 0);
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x40;
			*(long *)&pP_ptr->Free[ 2 ] = 0x30;
			pP = &pEm->pSin_parts_ptr[ 8 ];
			Esp_call(0x1F40, pEm->Cdir_y, &pP->Workm, 0);
			pP = &pEm->pSin_parts_ptr[ 9 ];
			Esp_call(0x1F40, pEm->Cdir_y, &pP->Workm, 0);
			pP = &pEm->pSin_parts_ptr[ 12 ];
			Esp_call(0x1F40, pEm->Cdir_y, &pP->Workm, 0);

		}
		/* retail-only Dual Shock vibration sequence (Pad_vib_set0/Pad_vib_set1/Pad_vib_fade_set;
		   references/bio2-ds/Bio2/prog/PAD.C:309,325,341 Pad_vib_set0/Pad_vib_set1/Pad_vib_fade_set) */
		Pad_vib_set0(2, 0);
		Pad_vib_set0(6, 10);
		Pad_vib_set0(10, 23);
		Pad_vib_set0(6, 40);
		Pad_vib_set1(30, 250, 0);
		Pad_vib_fade_set(120, 250, 0, 30);
		Pad_vib_set0(3, 72);
		Pad_vib_set1(8, 250, 72);

	case 1:
		if (!pEm->Routine_1) {
			if (pEm->Move_cnt < 60 && (pEm->Move_cnt & 1)) {

				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 2) + 0x7D0), 0, &pP_ptr[ 8 ].Workm, 0);

				Sv0.vx = 0;
				Sv0.vy = -600;
				Sv0.vz = 0;
				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0x7D0), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

			}
		}else {
			if (pEm->Move_cnt < 60 && (pEm->Move_cnt & 1)) {

				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(((Rnd() & 1) << 16) | ((Rnd() << 3) + 0xFA0), pEm->Cdir_y, &pP_ptr[ 1 ].Workm, 0);

			}
		}

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200))

			pEm->Routine_2 = 2;

		Add_speedXZ(pEm, 0);

		break;
	case 2:	/* inferred empty case: retail's compare tree tests < 2 */
		break;
	}
}
static void Em34_pldie01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x70001;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		Snd_se_stad(0x4010001, &pEm->Pos_x);
		pEm->Be_flg |= 2;	/* retail-only */
		Pad_vib_set1(30, 120, 5);	/* retail-only Dual Shock vibration */
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
		if (pEm->Move_cnt == 30) {	/* retail-only */
			Snd_se_stad(0x4000001, &pEm->Pos_x);
			pP_ptr = &pEm->pSin_parts_ptr[ 0 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 1 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 2 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 5 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 8 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 9 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
			pP_ptr = &pEm->pSin_parts_ptr[ 12 ];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[ 0 ] = 0x30;
			*(long *)&pP_ptr->Free[ 2 ] = 0x202020;
		}
		break;

	case 3:
		*(unsigned long *)&pEm->Move_no = 2;
		pEm->Routine_2 = 4;

		pEm->Cdir_y += 100;

		pEm->Be_flg &= 0xFFFFFFF7;

	case 4:
		if (pEm->Move_cnt < 10) {
			Sv0.vx = 256 - Rnd() * 2;
			Sv0.vy = 256 - Rnd() * 2;
			Sv0.vz = 256 - Rnd() * 2;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(((Rnd() & 1) << 16) | (Rnd() * 2 + 0x830), pEm->Cdir_y, &pP_ptr->Workm, &Sv0);

		}

		if (pEm->Move_cnt == 3)
			Snd_se_stad(0x4020001, &pEm->Pos_x);

		if (*pEm->pNow_seq & 0x10000) {
			Snd_se_walk(2, 0, pEm);
			Pad_vib_set0(3, 0);	/* retail-only Dual Shock vibration */
			Pad_vib_set1(6, 250, 0);
		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0x800);

		if (pEm->Move_cnt == 35) {
			pEm->Routine_2 = 5;
			*(unsigned long *)&pEm->Move_no = 0xF5502;

			Pad_vib_set0(5, 0);	/* retail: Dual Shock vibration replaces Snd_se_stad( 0x4030001 ) */
			Pad_vib_set1(5, 120, 0);
			Pad_vib_fade_set(60, 250, 0, 5);
		}

		break;

	case 5:
		pEm->Routine_2 += Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
		break;

	case 6:
		break;

	}
}
static unsigned long Em34_kabe_dir(ENEMY_WORK *pEm, short Ck)
{

	register unsigned short Cc;

	Cc = (~Ck & 1) * 0x400;	/* spelling inferred: "* 0x400" (not "<< 10") keeps retail's sign-extension of Ck */

	Ck = Cc - pEm->Cdir_y + 0x400;
	if ((Ck & 0xFFF) >= 0x800U)

		return Cc + 0x800;	/* Cc is 0 or 0x400, so this is Cc | 0x800; spelling inferred: an unshortened '+' keeps Cc (not the folded temp) in retail's ori */

	return Cc;
}

static void Em34_hand_right(ENEMY_WORK *pEm, MATRIX *pM)
{
	register PARTS_W *pP_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);

	pP_ptr = pEm->pSin_parts_ptr;
	CompM(pEm->M, pP_ptr->M, pM);

	CompM(pM, pP_ptr[ 9 ].M, pM);
	CompM(pM, pP_ptr[ 10 ].M, pM);
	CompM(pM, pP_ptr[ 15 ].M, pM);
	CompM(pM, pP_ptr[ 16 ].M, pM);
	CompM(pM, pP_ptr[ 17 ].M, pM);
	CompM(pM, pP_ptr[ 18 ].M, pM);

}

static void Em34_hand_left(ENEMY_WORK *pEm, MATRIX *pM)
{
	register PARTS_W *pP_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);

	pP_ptr = pEm->pSin_parts_ptr;
	CompM(pEm->M, pP_ptr->M, pM);

	CompM(pM, pP_ptr[ 9 ].M, pM);
	CompM(pM, pP_ptr[ 10 ].M, pM);

	CompM(pM, pP_ptr[ 11 ].M, pM);
	CompM(pM, pP_ptr[ 12 ].M, pM);
	CompM(pM, pP_ptr[ 13 ].M, pM);

}

static void Foot_set_Em34(ENEMY_WORK *pEm, unsigned long Mode, unsigned long Foot_no)
{
	register PARTS_W *pP_ptr;
	MATRIX M;
	register long x;
	register long z;

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM(pEm->M, pP_ptr->M, &M);
	CompM(&M, pP_ptr[ 9 ].M, &M);
	CompM(&M, pP_ptr[ 10 ].M, &M);

	if (!Foot_no) {
		pP_ptr += 19;	/* spelling inferred: retail addresses both feet relative to the last part */
		CompM(&M, pP_ptr[ -4 ].M, &M);

	}else
		pP_ptr += 14;
	CompM(&M, pP_ptr[ -3 ].M, &M);
	CompM(&M, pP_ptr[ -2 ].M, &M);
	CompM(&M, pP_ptr[ -1 ].M, &M);
	CompM(&M, pP_ptr->M, &M);

	if (!Mode) {
		pEm->Pos_x -= M.t[ 0 ] - pP_ptr->Workm.t[ 0 ];
		pEm->Pos_z -= M.t[ 2 ] - pP_ptr->Workm.t[ 2 ];
	}else {

		x = M.t[ 0 ] - pP_ptr->Workm.t[ 0 ];
		z = M.t[ 2 ] - pP_ptr->Workm.t[ 2 ];
		pEm->Spd_x = SquareRoot0(x * x + z * z);
	}	}
static void Em34_kuchi_move(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	if (!(pEm->Type & 1)) return;

	Em34_kuchi_move_tbl[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 ](pEm, pP_ptr);

	Sa_parts_allmv(pEm, pP_ptr);

}

static void Em34_kuchi_mv0(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Iki = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 120;

	case 1:
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Iki == 63)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 2;

		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		((short *)pP_ptr->pSa_dat_head)[ 10 ] >>= 1;
		((short *)pP_ptr->pSa_dat_head)[ 12 ] = Em34_iki[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Iki ] * 16;
		break;

	case 2:
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Iki == 63)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 3;

		((short *)pP_ptr->pSa_dat_head)[ 8 ] = Em34_iki[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Iki ] * 16;
		break;

	case 3:
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Iki == 63)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;

		((short *)pP_ptr->pSa_dat_head)[ 10 ] = Em34_iki[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Iki ] * 16;
		break;

	}

	((EM34_FREE *)EM_FREE_AREA(pEm))->Iki++;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Iki &= 0x3F;	}
static void Em34_kuchi_mv1(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ 8 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ 10 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 / 4;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 4;

		((short *)pP_ptr->pSa_dat_head)[ 12 ] >>= 1;

		if (((short *)pP_ptr->pSa_dat_head)[ 10 ] > 0x400)
			((short *)pP_ptr->pSa_dat_head)[ 10 ] = 0x400;

		if (((short *)pP_ptr->pSa_dat_head)[ 8 ] > 0xC00) {
			((short *)pP_ptr->pSa_dat_head)[ 8 ] = 0xC00;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 2;
		}
		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[ 8 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ 10 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 8;
		if (((short *)pP_ptr->pSa_dat_head)[ 10 ] > 0x1000) {
			((short *)pP_ptr->pSa_dat_head)[ 10 ] = 0x1000;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 3;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t2 = 10;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = Rnd() * 2 + 0x400 + Rnd() + Rnd();
		}

		break;

	case 3:
		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		((short *)pP_ptr->pSa_dat_head)[ 10 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ 12 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 32;
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 > 256)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 256;

		if (((short *)pP_ptr->pSa_dat_head)[ 12 ] > ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1) {
			((short *)pP_ptr->pSa_dat_head)[ 12 ] = ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;
			((short *)pP_ptr->pSa_dat_head)[ 10 ] = 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 4;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 -= Rnd() + 0x264;

			if ((short)((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t2-- == 0)	/* cast keeps the old-value test, as retail */
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 5;
		}

		break;

	case 4:
		((short *)pP_ptr->pSa_dat_head)[ 10 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ 12 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 8;
		if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 > 64)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 64;

		if (((short *)pP_ptr->pSa_dat_head)[ 12 ] < ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1) {
			((short *)pP_ptr->pSa_dat_head)[ 10 ] = 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;
			((short *)pP_ptr->pSa_dat_head)[ 12 ] = ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 3;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 += Rnd() + 0x2C8;
		}
		break;

	case 5:
		((short *)pP_ptr->pSa_dat_head)[ 10 ] -= 32;
		((short *)pP_ptr->pSa_dat_head)[ 12 ] -= 32;
		if (((short *)pP_ptr->pSa_dat_head)[ 12 ] < 0)
			((short *)pP_ptr->pSa_dat_head)[ 12 ] = 0;

		if (((short *)pP_ptr->pSa_dat_head)[ 10 ] < 0)
			((short *)pP_ptr->pSa_dat_head)[ 10 ] = 0;

		if (!((short *)pP_ptr->pSa_dat_head)[ 10 ] && !((short *)pP_ptr->pSa_dat_head)[ 12 ]) {

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		}

	}
}
static void Em34_kuchi_mv2(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no = ((Rnd() >> (Rnd() & 3)) & 1) + 5;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 = 11 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = Em34_kuchi_mv2_tbl[ Rnd() & 0x1F ] + 0x800;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0++;

		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 / 3;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] > 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] = 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;

		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] > ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1) {
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] = ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 6;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		}
	}

}

static void Em34_kuchi_mv3(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 8;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] < 0)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 *= 2;
		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] < -824)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] = -824;

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] == -824 && !((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ]) {

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 2;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
		}

		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0++;

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] > 0) {
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 3;
		}
		break;

	case 3:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
	}

}
static void Em34_kuchi_mv4(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no = ((Rnd() >> (Rnd() & 3)) & 1) + 5;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 = 11 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = Em34_kuchi_mv2_tbl[ Rnd() & 0x1F ] + 0x800;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 8;

		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 / 3;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] > 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] = 0x1000 - ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;

		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] > ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1) {
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] = ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1;

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 6;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		}
	}

}

static void Em34_kuchi_mv5(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	SVECTOR Sv;

	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] < 0)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] = 0;

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] < 0)
			((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] = 0;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 2;

		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		if (!((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] && !((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ]) {

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r0 = 0;
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 0;
		}
	}
}
static void Em34_kuchi_mv6(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	switch (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1) {
	case 0:
		((EM34_FREE *)EM_FREE_AREA(pEm))->Iki = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t2 = 120;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = (Rnd() >> 1) + 128;

		((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3 = 0;

	case 1:
		((short *)pP_ptr->pSa_dat_head)[ 8 ] >>= 1;
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;

		if (!((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 4;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 > 256)
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 256;
		}else {

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 -= 8;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 < 0) {
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 2;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = ((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] - ((Rnd() >> 1) + 312);

				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3 = 0;
				break;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] > ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 && !((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3)
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3 = 1;

		break;

	case 2:
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no * 2 ] += ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;
		((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] -= ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0;

		if (!((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3) {
			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 += 2;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 > 256)
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 256;
		}else {

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 -= 4;
			if (((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 < 0) {
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_r1 = 1;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t0 = 0;
				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 = ((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] + ((Rnd() >> 1) + 376);

				((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3 = 0;
				break;
			}
		}

		if (((short *)pP_ptr->pSa_dat_head)[ ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_no1 * 2 ] < ((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t1 && !((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3)

			((EM34_FREE *)EM_FREE_AREA(pEm))->Kuchi_t3 = 1;

	}

	((EM34_FREE *)EM_FREE_AREA(pEm))->Iki++;
	((EM34_FREE *)EM_FREE_AREA(pEm))->Iki &= 0x3F;	}
static void Em34_kubi(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	register long i;
	register long Ck;
	register long Tmp;	short Dir;
	register PARTS_W *pP;	/* inferred name: no retail record */
	pEm->pEnemy_ptr = (unsigned long *)&G.Player;
	Rot_neck_em_n(pEm, pEm->Cdir_y, &Dir);
	Dir = Ck = Dir & 0xFFF;
	if (Ck > 0x800)
		Dir = Ck - 0xFFF;

	i = 0;
	Tmp = 0;

	if (pEm->Type & 1) {
		pP = &pEm->pSin_parts_ptr[ 10 ];
		while (i < 4) {
			Ck = ((short *)pP->pSa_dat_head)[ 6 + i * 2 ];
			Tmp += ((Em34_m03_sabun[ i ] << 4) * Ck) >> 16;
			i++;	}
		if (Tmp < -712)
			Tmp = -712;

		if (Tmp > 0)
			Tmp = 0;
	}

	pP = &pEm->pSin_parts_ptr[ 26 ];
	Sv.vx = pP->Cdir_x;
	Sv.vy = pP->Cdir_y + Dir;
	Sv.vz = pP->Cdir_z + Tmp;

	RotMatrix(&Sv, (MATRIX *)pP->M);

}

static void Em34_dir_spd(ENEMY_WORK *pEm, short Dir, SVECTOR *pSpd)
{
	SVECTOR S_dir;
	MATRIX M;

	S_dir.vx = 0;
	S_dir.vy = Dir;
	S_dir.vz = 0;

	RotMatrix(&S_dir, &M);
	ApplyMatrixSV(&M, pSpd, (SVECTOR *)&pEm->Spd_x);

}

static unsigned long Em34_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top)
{

	return Jh_set(pEm, &pEm->pSin_parts_ptr[ 26 ], Em34_jh0_head, Mem_top);	}

static void Em34_Jh_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = &pEm->pSin_parts_ptr[ 26 ];
	MulJh(pEm, pP_ptr, Em34_jh0_head);
	Jh_sort(pEm, pP_ptr);

}
static void Em34_core_set_dest(No, D_x, D_y)	/* K&R (old-style) definition: the unnarrowed call sites and the SYM line base need it */
unsigned char No; short D_x, D_y; {
	MEDAMA *pMe;

	pMe = &Em34_Me[ No ];

	pMe->Tate_d = D_x;
	pMe->Yoko_d = D_y;

}

static void Em34_core_me_mv(ENEMY_WORK *pEm, unsigned char No)
{
	register PARTS_W *pP_ptr;
	register short Tmp_t;
	register short Tmp_y;
	register MEDAMA *pMe;

	pMe = &Em34_Me[ No ];

	pP_ptr = &pEm->pSin_parts_ptr[ pMe->Parts_no ];

	Tmp_t = pMe->Tate_a;
	Tmp_y = pMe->Yoko_a;

	if ((signed char)pMe->Tate > (signed char)pMe->Tate_d)
		pMe->Tate_a = -1;

	if ((signed char)pMe->Tate < (signed char)pMe->Tate_d)
		pMe->Tate_a = 1;

	if (pMe->Yoko > pMe->Yoko_d)
		pMe->Yoko_a = -1;

	if (pMe->Yoko < pMe->Yoko_d)
		pMe->Yoko_a = 1;

	if (-pMe->Tate_a == Tmp_t)
		pMe->Tate_mv = pMe->Tate_mv * 2 / 3;

	if (-pMe->Yoko_a == Tmp_y)
		pMe->Yoko_mv = pMe->Yoko_mv * 2 / 3;

	if (pMe->Tate == (signed char)pMe->Tate_d && abs(pMe->Tate_mv) < 3) {
		pMe->Tate = pMe->Tate_d;
		pMe->Tate_a = 0;
		pMe->Tate_mv = 0;
	}
	if (pMe->Yoko == (signed char)pMe->Yoko_d && abs(pMe->Yoko_mv) < 3) {
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

	if ((signed char)pMe->T_max < pMe->Tate)
		pMe->Tate = pMe->T_max;

	if (pMe->Tate < (signed char)pMe->T_min)
		pMe->Tate = pMe->T_min;

	if ((signed char)pMe->Y_max < pMe->Yoko)
		pMe->Yoko = pMe->Y_max;

	if (pMe->Yoko < (signed char)pMe->Y_min)
		pMe->Yoko = pMe->Y_min;

	Tmp_t = (signed char)pMe->Tate - Tmp_t;
	Tmp_y = (signed char)pMe->Yoko - Tmp_y;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 2, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, pMe->pMe3);

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 2, (Tmp_t << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, pMe->pMe4);

}
static unsigned long Em34_pl_pos(ENEMY_WORK *pEm, SVECTOR *pSv, unsigned long L, unsigned short Flg)
{
	register short i;
	SVECTOR Sv0;
	MATRIX M;
	register unsigned long X;
	register unsigned long Z;
	VECTOR Tv;

	Sv0.vy = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

	Sv0.vx = 0;
	Sv0.vz = 0;

	RotMatrix(&Sv0, &M);
	Sv0.vx = L;
	Sv0.vy = 0;
	ApplyMatrix(&M, &Sv0, &Tv);

	Tv.vx += G.Player.Pos_x;
	Tv.vz += G.Player.Pos_z;

	if (Flg) {

		if (Sca_ck_hit(&Tv, 1400, 1, 0x500)) {
			i = 0;
			while (i < 9) {
				Tv.vx = G.Player.Pos_x + Em34_hosei_pos[ i ][ 0 ];
				Tv.vz = G.Player.Pos_z + Em34_hosei_pos[ i ][ 1 ];

				if (!Sca_ck_hit(&Tv, 1400, 1, 0x500))
					break;

				i++;	}
		}
	}

	X = Tv.vx - pEm->Pos_x;
	Z = Tv.vz - pEm->Pos_z;
	pSv->vx = Tv.vx;
	pSv->vz = Tv.vz;

	return SquareRoot0(X * X + Z * Z);

}

static void Em34_pos_set(ENEMY_WORK *pEm)
{
	register unsigned char Area_no;	register unsigned char i;	register unsigned char j;	register unsigned char k;
	register unsigned long LL = 0x7FFFFFFF;
	register unsigned long L;	register unsigned long X;	register unsigned long Z;

	Area_no = (Rnd() >> (Rnd() & 3)) & 1;
	if (pEm->Pos_x < -15000)
		Area_no = 0;

	if (pEm->Pos_x > -11800)
		Area_no = 1;

	i = 0;

	j = Area_no * 4;	/* spelling inferred: a multiply (not a shift) keeps retail's zero-extended Area_no */

	Em34_uppos = 0;
	k = 0;
	if (((EM34_FREE *)EM_FREE_AREA(pEm))->Etc_flg & 0x20)
		j = (Area_no != 1) * 4;	/* the other area (Area_no is 0 or 1); spelling inferred from retail's unshortened xori */

	do {

		if (!Goto00_ck(pEm, Em34_mv10_pos[ j + i ][ 0 ], Em34_mv10_pos[ j + i ][ 1 ], 0x270)) {

			Em34_uppos++;
			Em34_uppos_no[ k ] = j + i;
			k++;
		}
	}while (++i < 4);

	i = 0;
	j = Area_no << 3;
	Em34_pl_downpos_no = 0;
	do {
		X = G.Player.Pos_x - Em34_mv10_downpos[ j + i ][ 0 ];
		Z = G.Player.Pos_z - Em34_mv10_downpos[ j + i ][ 1 ];
		L = SquareRoot0(X * X + Z * Z);
		if (L < LL) {
			Em34_pl_downpos_no = j + i;
			LL = L;
		}
	}while (++i < 8);

}
