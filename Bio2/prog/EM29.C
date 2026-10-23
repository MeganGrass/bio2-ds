/* em29.c - reconstruction of D:\Bio2\prog\EM29.C (enemy 29).
 * Linked into two overlays from the same source: 0x0E (at 0x80100078) and
 * 0x2C (at 0x8010d078); only the enemy-number-specific entry names differ
 * (Em129/Em229, Em129_Pldm/Em229_Pldm), selected by OVL_2C.
 * Types come from the TU's retail SYM preamble (em29.h). */
#include "EM29.H"

/* Em29 work variables kept in the enemy's generic Free[] area */
#define EF	((EM29_FREE *)&pEm->Free[1])	/* starts 4 bytes after ENEMY_WORK.Free (retail +0x218) */

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;
extern long Ground_in_pos;	/* SCA.C */

/* ---- functions of other translation units ----
 * EM29.C includes no prototypes for most of them: those external routines are
 * called through an implicit declaration (arguments unnarrowed, implicit int).
 * The implicit declarations are what open the record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static unsigned long Hold_info;
static unsigned long Hold_num;
static unsigned long Critical_flg;
static unsigned long Hold_time;
static unsigned long Em29_vib_cnt;	/* retail-only: frames of the hold vibration after the player's death ([INFERRED] name) */
/* forward declarations: the retail SYM lists these objects first-declared here (defined below). */
static HOLD_TBL Hold_tbl[32];
static void (*Em29_routine_0[8])();
static void (*Em29_mv_br[5])();
static void (*Em29_mv[5])();
static void (*Em29_die[2])();
/* retail-only (Dual Shock) hold vibration table, retail .data 0x80101C4C..0x80101CCF ([INFERRED] name):
 * [0..30] motor-1 power per number of holding roaches, [31] fade enable, [32] the fading power
 * after the player's death.  One array, not three scalars: EM_MAIN keeps &[32] in a register
 * (a constant-offset address), which separate scalars do not reproduce. */
static unsigned long Em29_vib_pow[33] = { 0, 60, 90, 120, 150, 170, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190 };
static void (*Em29_pldm_tbl[2])() = { &Em29_Pl00, &Em29_Pl0f };
static void (*Em29_routine_0[8])() = { &Em29_Init, &Em29_Move, &Em29_Die, &Em29_Die, &Em29_Scenario, 0, 0, &Em29_Dead };
static void (*Em29_mv_br[5])() = { &Em29_taiki_br, &Em29_run_br, &Em29_fly_br, &Em29_hold_br, &Em29_fall_br };
static void (*Em29_mv[5])() = { &Em29_taiki, &Em29_run, &Em29_fly, &Em29_hold, &Em29_fall };
static void (*Em29_die[2])() = { &Em29_die0, &Em29_die1 };
static HOLD_TBL Hold_tbl[32] = {
	{ 8, 245, -306, 10, 0, 2048, -1024 },
	{ 8, 97, -316, -106, 9017, -546, -4165 },
	{ 8, -43, -253, 50, 7998, -41, -4835 },
	{ 1, 150, -569, -102, 460, 2000, 3426 },
	{ 1, 207, -169, 90, 3906, 2160, 2976 },
	{ 1, 134, -168, -194, 3546, 2160, 2846 },
	{ 1, 182, 0, -40, 284, 2286, 3036 },
	{ 2, 125, 353, 90, 143, 1460, 3046 },
	{ 3, 60, 279, -30, 29, 2100, 3044 },
	{ 5, 120, 256, 80, 269, 1580, 2882 },
	{ 6, 47, 136, -100, 69, 2522, 3052 },
	{ 9, -23, 230, -116, 3515, 3416, 2876 },
	{ 10, 80, -30, -50, 285, 3356, 3486 },
	{ 11, -30, -30, -50, 133, 3576, 3197 },
	{ 12, -14, -20, -70, 0, 2866, 190 },
	{ 13, 19, -4, 40, 1280, 1126, 1876 },
	{ 13, 82, 206, 37, 1849, 36, 960 },
	{ 0, -208, -674, 65, 2319, 1842, 1139 },
	{ 0, -202, -314, -74, 1769, 1842, 999 },
	{ 1, -177, -14, 162, 2689, 1842, 959 },
	{ 2, 163, -14, -58, 3992, 3468, 3095 },
	{ 3, -180, -194, -8, 4, 162, 2945 },
	{ 4, -1, -354, -23, 64, 3678, 3088 },
	{ 6, -213, -354, 47, 3851, 4058, 3049 },
	{ 7, -23, -354, 47, 226, 4058, 3039 },
	{ 10, 33, -354, 47, 4031, 4058, 3379 },
	{ 13, -34, -214, 35, 55, 4058, 3119 },
	{ 1, -153, -214, -237, 405, 3378, 3409 },
	{ 1, 8, -214, 226, 545, 798, 2529 },
	{ 8, 245, -306, 10, 0, 2048, -1024 },
	{ 8, 97, -316, -106, 9017, -546, -4165 },
	{ 8, -43, -253, 50, 7998, -41, -4835 }
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	register HOLD_TBL *pH;

	pH = &Hold_tbl[ EF->Target_no ];
	/* retail-only (Dual Shock): the roach in enemy slot 2 drives the hold vibration */
	if (pEm == G.pEnemy[2] && Hold_num) {
		if (G.Player.Life > 0 && !(G.System_flg & 0x4000000)) {
			Pad_vib_set1(1, Em29_vib_pow[ Hold_num ], 0);
			Em29_vib_pow[32] = 190;
		}
		else {
			Pad_vib_set1(1, Em29_vib_pow[32], 0);
			if (G.Cbuf_id && Em29_vib_pow[31])
				Em29_vib_pow[32]--;
			Em29_vib_cnt++;
		}
	}
	/* retail-only: a holding roach is pinned to its hold point */
	if ((*(long *)&pEm->Routine_0 & 0xFFFFFF) == 0x10301) {
		pEm->Pos_x = pH->Pos_x;
		pEm->Pos_y = pH->Pos_y;
		pEm->Pos_z = pH->Pos_z;
	}

	if (G.Stop_flg & 0x20000000) return;

	if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

	Em29_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

	Em_seq_sound(pEm);

	if (EF->Attribute & 8) {
		if (pEm->Pos_x > -7300) pEm->Pos_x = -7300;
		if (pEm->Pos_x < -27000) pEm->Pos_x = -27000;

		if (pEm->L_pl < 550 && G.Player.Pos_y - 32 < pEm->Pos_y && !EF->Foot_die_flg && G.Player.Routine_1 && G.Player.Routine_1 < 5) {

			EF->Foot_die_flg = 1;
			pEm->Life = -1;
			EF->Attribute |= 4;
			pEm->Be_flg |= 0xA;
			if (Em29_vib_cnt < 160)
				Pad_vib_set0(2, 0);	/* retail-only (Dual Shock) */
			*(long *)&pEm->Routine_0 = 3;
		}
	}

	if (Hold_num < 3) Hold_time = 0;

	if (Critical_flg)
		if (!(EF->Attribute & 1)) *(long *)&pEm->Routine_0 = 0x201;

	if (EF->Spark_timer) {
		if (EF->Spark_timer == 1) Dm_eff_disp(pEm, 6, 0, 0xC00);
		EF->Spark_timer--;
	}

	Oba_set_ofs(pEm, &G.pEm->Pos_x);
}

static void Em29_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PARTS_W *pParts;
	pParts = pEm->pSin_parts_ptr;

	G.pPldamage_Em[ 0x29 - 0x10 ] = EM_PLDM;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Neck_no = 0;	/* retail */
	pEm->Type |= 0x2000;
	pEm->Life = 10;
	pEm->Be_flg |= 0x800000;	/* retail 0x800000 (Beta 0xC00000) */

	Hold_info = 0;
	Hold_num = 0;
	Hold_time = 0;
	Critical_flg = 0;

	EF->R = 0;
	EF->Spd_r = 0;
	EF->Dest_dir = 0;
	EF->Attribute = 0;
	EF->Target_no = 0;
	EF->Dest_x = EF->Dest_y = EF->Dest_z = 0;

	EF->Foot_die_flg = 0;
	EF->Spark_timer = 0;

	*(long *)&pEm->Move_no = 1;
	Joint_move(pEm, Kan, Seq, 0x100);

	pParts[2].Be_flg = 0;
	pParts[3].Be_flg = 0;

	pEm->Be_flg |= 0x2000000;
	pEm->Super_vector.vx = 0;
	pEm->Super_vector.vy = 0xC00;
	pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = 0;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = pEm->Atd[0].W = 1;
	pEm->Atd[0].At_h = 100;
	pEm->Atd[0].At_d = pEm->Atd[0].D = 1;
	pEm->Eff_at_r = 800;

	Em_init_super(pEm);
	pEm->Cdir_y = Rnd() << 4;
	pEm->Cdir_z = 0;
	Em29_Dir_set(pEm);

	pEm->Be_flg |= 0x1C000000;

	pEm->Be_flg |= 2;

	Kage_work_set(&pEm->pKage_work, 0, 0, 0, &pEm->Pos_x);
	Em29_vib_cnt = 0;	/* retail-only */

}

static void Em29_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em29_mv_br[ pEm->Routine_1 ](pEm);
	Em29_mv[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em29_taiki_br(ENEMY_WORK *pEm) {}

static void Em29_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	EF->Attribute |= 8;

	switch (pEm->Routine_2) {
	case 0:
		if (Mot_play(pEm, 1, 0, 0)) {
			if (!(Rnd() & 2)) {
				*(long *)&pEm->Routine_0 = 0x101;
				break;
			} pEm->Routine_2++;

		} break;

	case 1:
		if (pEm->Routine_2 -= Mot_play(pEm, 0, 0, 0)) {
			if (!(Rnd() & 2))
				*(long *)&pEm->Routine_0 = 0x101;
			else
				pEm->Routine_2--;
		}

	}
}

static void Em29_run_br(ENEMY_WORK *pEm) {}

static void Em29_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Spd;
	short Ck;	/* inferred name: no retail SYM record (combined away) */

	EF->Attribute |= 8;

	switch (pEm->Routine_2) {
	case 0:
		EF->R = Direction_ck(-0x6270, -0x708, pEm->Pos_z, pEm->Pos_y);
		pEm->Spd_x = 0;
		pEm->Routine_2++;

	case 1:

		if (!(Rnd() & 3)) {

			if (pEm->Pos_y < -1800 && !(Rnd() & 3)) {
				*(long *)&pEm->Routine_0 = 0x201; break; }

			*(long *)&pEm->Routine_0 = 1; break;
		}else {
			if ((Rnd() & 3) == 2) Snd_se_enem(0, pEm);
			EF->Dest_dir = (short)(pEm->Cdir_y - 0x400) + (Rnd() << 3);
			pEm->Routine_2++;
		}

	case 2:

		if (pEm->Spd_x < 140) pEm->Spd_x += 20;
		Spd.vz = pEm->Spd_x;
		Spd.vy = Spd.vx = 0;

		Ck = Goto01_ck(pEm, EF->Dest_dir, 32);
		if (!Ck) pEm->Routine_2 = 1;
		pEm->Cdir_y += Ck;
		Rot_vector(pEm->Cdir_y, &Spd, &Spd);
		EF->Spd_r = Spd.vz * 409600 / 1130400;

		EF->R = (EF->R + EF->Spd_r) & 0xFFF;
		pEm->Pos_x += Spd.vx;

		pEm->Pos_z = rcos(EF->R) * 1800 / 4096 - 25200;
		pEm->Pos_y = -(rsin(EF->R) * 1800) / 4096 - 1800;

		pEm->Super_vector.vy = 0xC00;
		pEm->Super_vector.vz = 0xC00 - EF->R;
		RotMatrix(&pEm->Super_vector, pEm->pSuper);
	}
}

static void Em29_fly_br(ENEMY_WORK *pEm) {}

static void Em29_fly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PARTS_W *pParts;	register long z, y, r;	MATRIX Tmp_m2;
	pParts = pEm->pSin_parts_ptr;

	EF->Attribute |= 8;

	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(3, pEm);
		pParts[2].Be_flg = 1;
		pParts[3].Be_flg = 1;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		pEm->Timer2 = 0;
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;

		EF->Target_no = Em29_sel_hold();
		EF->Attribute |= 1;

		pEm->Routine_2++;

	case 1:
		if (Mot_play(pEm, 2, 0, 0)) {

			Em_init_super(pEm);
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);

			pEm->Be_flg |= 0x2000;

			pEm->Timer0 = 0;
			pEm->Routine_2++;
		}
		break;

	case 2:
		Mot_play(pEm, 3, 0, 0);

		z = -25200 - pEm->Pos_z;
		y = -1800 - pEm->Pos_y;
		r = SquareRoot0(z * z + y * y);

		pEm->Timer2++;
		if ((signed char)pEm->Timer2 == 16) {
			pEm->Timer2 = 0;
			Snd_se_enem(3, pEm);
		}

		if (r > 1800) {
			if ((EF->Attribute & 2) || pEm->Timer0 >= 90) {

				EF->Attribute &= ~2;
				Em29_Dir_set(pEm);
				pParts[2].Be_flg = 0;
				pParts[3].Be_flg = 0;
				EF->Attribute &= ~1;
				Hold_info &= ~(1 << EF->Target_no);

				pEm->Be_flg &= ~0x2000;

				*(long *)&pEm->Routine_0 = 1; break;
			}

			pEm->Pos_z = -25200 - z * 1800 / r;
			pEm->Pos_y = -1800 - y * 1800 / r;
			pEm->Spd_y = 0;
		}

		if (pEm->Timer0 < 90) pEm->Timer0++;

		{	MATRIX Tmp_m;
			VECTOR Vec;	register HOLD_TBL *pH;	register PARTS_W *pPl_Parts;
			pH = &Hold_tbl[ EF->Target_no ];
			pPl_Parts = G.Player.pSin_parts_ptr;

			RotMatrix((SVECTOR *)&pH->Dir_x, &Tmp_m);
			Tmp_m.t[0] = pH->Pos_x;
			Tmp_m.t[1] = pH->Pos_y;
			Tmp_m.t[2] = pH->Pos_z;
			CompM(&pPl_Parts[ pH->Parts_no ].Workm, &Tmp_m, &Tmp_m2);
			EF->Dest_x = Tmp_m2.t[0];
			EF->Dest_y = Tmp_m2.t[1];
			EF->Dest_z = Tmp_m2.t[2];
		}

		if (SquareRoot0((pEm->Pos_x - Tmp_m2.t[0]) * (pEm->Pos_x - Tmp_m2.t[0]) + (pEm->Pos_z - Tmp_m2.t[2]) * (pEm->Pos_z - Tmp_m2.t[2])) < 200 && __builtin_abs(pEm->Pos_y - EF->Dest_y) < 200) {

			EF->Attribute |= 2;
			EF->Attribute &= ~8;

			Hold_num++;

			if (!G.Player.Damage_cnt) {

				G.Player.Routine_0 = 5;
				G.Player.Routine_1 = 0;
				*(short *)&G.Player.Routine_2 = 0;

				G.Player.pEnemy_ptr = pEm;
				G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
				G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

				if (Hold_time >= 226) {
					G.System_flg |= 0x4000000;
					*(long *)&G.Player.Routine_0 = 3;
					G.Player.Life = -1;
					G.Player.Damage_cnt = 0x80;
					Critical_flg = 1;
				}
			}

			pEm->Be_flg &= ~0x2000;

			*(long *)&pEm->Routine_0 = 0x301;
			if (Em29_vib_cnt < 160) {	/* retail-only (Dual Shock) */
				Pad_vib_set0(4, 0);
				Pad_vib_fade_set(12, 140, 50, 1);
			}
			break;
		}

		if (!(EF->Attribute & 2) && pEm->Timer0 < 90) {
			Goto00(pEm, EF->Dest_x, EF->Dest_z, 64);

			y = EF->Dest_y - (pEm->Pos_y + pEm->Spd_y);
			if (y > 100) y = 100;
			if (y < -100) y = -100;
			pEm->Spd_y = y;

		}else {
			pEm->Spd_y++;
		}

		pEm->Pos_y += pEm->Spd_y;
		if (pEm->Spd_x < 200) pEm->Spd_x += pEm->Timer1++;
		Add_speedXZ(pEm, 0, 0);

	}
}

static void Em29_hold_br(ENEMY_WORK *pEm) {}

static void Em29_hold(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PARTS_W *pParts;	register HOLD_TBL *pH;
	pParts = G.Player.pSin_parts_ptr;
	pH = &Hold_tbl[ EF->Target_no ];

	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(3, pEm);
		pEm->Be_flg |= 0x3000000;
		pEm->Cdir_x = pH->Dir_x;
		pEm->Cdir_y = pH->Dir_y;
		pEm->Cdir_z = pH->Dir_z;
		pEm->pSuper = (unsigned long *)&pParts[ pH->Parts_no ].Workm;
		pEm->Timer0 = Rnd() / 16 + 90;
		pEm->Routine_2++;
	case 1:
		pEm->Pos_x = pH->Pos_x;
		pEm->Pos_y = pH->Pos_y;
		pEm->Pos_z = pH->Pos_z;
		Mot_play(pEm, 2, 0, 0);

		if (G.Padd1_trg)
			pEm->Timer0 -= 8;
		else
			pEm->Timer0--;
		if (pEm->Timer0 < 0) {

			if (!Critical_flg) {

				*(long *)&pEm->Routine_0 = 0x401;
				Hold_num--;

				if (!Hold_num) {
					G.Player.Routine_0 = 1;
					G.Player.Routine_1 = 0;
					*(short *)&G.Player.Routine_2 = 0;
				}
			}
		}

	}
	if (Hold_num >= 3) Hold_time++;
}

static void Em29_fall_br(ENEMY_WORK *pEm) {}

static void Em29_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Gnd;
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	SVECTOR Vec = { 20, 0, 0, 0 };
	short Dir;	/* inferred name: no retail SYM record (combined away) */
	switch (pEm->Routine_2) {
	case 0:
	{	MATRIX Tmp_m, Tmp_m2;	VECTOR Vec;	register HOLD_TBL *pH;	register PARTS_W *pPl_Parts;	pEm->Be_flg &= ~0x1000000;

		pH = &Hold_tbl[ EF->Target_no ];
		pPl_Parts = G.Player.pSin_parts_ptr;

		RotMatrix((SVECTOR *)&pH->Dir_x, &Tmp_m);
		Tmp_m.t[0] = pH->Pos_x;
		Tmp_m.t[1] = pH->Pos_y;
		Tmp_m.t[2] = pH->Pos_z;
		CompM(&pPl_Parts[ pH->Parts_no ].Workm, &Tmp_m, &Tmp_m2);
		pEm->Pos_x = Tmp_m2.t[0];
		pEm->Pos_y = Tmp_m2.t[1];
		pEm->Pos_z = Tmp_m2.t[2]; }

		Dir = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

		pEm->Spd_x = 50;
		pEm->Spd_z = 0;
		Rot_vector(Dir, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);

		pEm->Cdir_x = 0;
		pEm->Cdir_y = 0;
		pEm->Cdir_z = 0;
		pEm->pSuper = (unsigned long *)&pEm->Super_matrix;
		Em_init_super(pEm);
		pEm->Timer0 = 0;
		pEm->Spd_y = 20;
		pEm->Routine_2++;
	case 1:
		Add_speedXZ(pEm, 0, 0);
		pEm->Spd_y += pEm->Timer0 += 2;
		pEm->Pos_y += pEm->Spd_y;

		if (__builtin_abs(pEm->Super_vector.vz - 0xC00) > 16) {
			if (pEm->Super_vector.vz < 0xC00)
				pEm->Super_vector.vz -= 16;
			else
				pEm->Super_vector.vz += 16;
			RotMatrix(&pEm->Super_vector, pEm->pSuper);
		}

		Gnd = Sca_get_ground(&pEm->Pos_x, 1, 0x400, 0);
		if (Ground_in_pos < Gnd) Gnd = Ground_in_pos;
		if (pEm->Pos_y >= Gnd) {
			Snd_se_enem(1, pEm);
			pEm->Pos_y = Gnd;
			Em29_Dir_set(pEm);
			pParts[2].Be_flg = 0;
			pParts[3].Be_flg = 0;
			EF->Attribute &= ~1;
			Hold_info &= ~(1 << EF->Target_no);
			*(long *)&pEm->Routine_0 = 1;
		}

	}
}

static void Em29_die_ck(ENEMY_WORK *pEm)
{
	if (EF->Attribute & 4) {
		pEm->Damage_no = 1;
	}else {

		pEm->Damage_no = 0;

	} }

static void Em29_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Routine_1 != 0xFF) {

		Dm_eff(pEm, pEm->Routine_1);
		pEm->Routine_1 = 0xFF;
		Em29_die_ck(pEm);
	}

	Em29_die[ pEm->Damage_no ](pEm, Kan, Seq); }

static void Em29_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em29_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em29_die0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PARTS_W *pParts;	register short Gnd;
	pParts = pEm->pSin_parts_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Spd_y = 50;
		pEm->Routine_2++;
	case 1:
		pEm->Spd_y += pEm->Timer0 += 2;
		pEm->Pos_y += pEm->Spd_y;

		if (__builtin_abs(pEm->Super_vector.vz - 0xC00) > 16) {
			if (pEm->Super_vector.vz < 0xC00)
				pEm->Super_vector.vz -= 16;
			else
				pEm->Super_vector.vz += 16;
			RotMatrix(&pEm->Super_vector, pEm->pSuper);
		}

		Gnd = Sca_get_ground(&pEm->Pos_x, 1, 0x400, 0);
		if (Ground_in_pos < Gnd) Gnd = Ground_in_pos;
		if (Gnd > -7200 && pEm->Pos_y >= Gnd) {
			pEm->Pos_y = Gnd;
			pParts[2].Be_flg = 1;
			pParts[3].Be_flg = 1;
			pEm->Spd_y = 20;
			pEm->Routine_2++;
		}

		break;

	case 2:
		pEm->Routine_2 += Mot_play(pEm, 4, 0, 0);
	case 3:
		pEm->Pos_y += pEm->Spd_y;
		Em29_Dir_set(pEm);
		if (pEm->Pos_z == pEm->Old_z) pEm->Routine_2++;

	case 4:
	}
}

static void Em29_die1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	SVECTOR Svec;	register PARTS_W *pParts;

	pParts = pEm->pSin_parts_ptr;

	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(2, pEm);
		Svec.vx = pEm->Pos_x;
		Svec.vy = pEm->Pos_y;
		Svec.vz = pEm->Pos_z;
		pParts[2].Be_flg = 1;
		pParts[3].Be_flg = 1;
		Esp_call(0x100C00, 0, &GsIDMATRIX, &Svec);
		pEm->Routine_2++;
	case 1:
		pEm->Move_cnt = 0;
		Mot_play(pEm, 4, 0, 0);

	}
}

void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register unsigned long D0;	/* inferred name: no retail SYM record (combined into its use) */

	D0 = G.Player.Id == 15;

	Em29_pldm_tbl[ D0 ](pEm, Kan, Seq);
}

static void Em29_Pl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0;
		Snd_se_stad(0x4000001, &pEm->Pos_x);	/* retail-only */
		pEm->Routine_2++;
	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Move_no = 1;
			pEm->Routine_2++;
		}
		break;
	case 2:
		Joint_move(pEm, Kan, Seq, 0x400);

	}
}

static void Em29_Pl0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 15;
		Snd_se_stad(0x4000001, &pEm->Pos_x);	/* retail-only */
		pEm->Routine_2++;
	case 1:
		Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

	}
}

static void Em29_Dir_set(ENEMY_WORK *pEm)
{
	EF->R = Direction_ck(-0x6270, -0x708, pEm->Pos_z, pEm->Pos_y);

	pEm->Pos_z = rcos(EF->R) * 1800 / 4096 - 25200;
	pEm->Pos_y = -(rsin(EF->R) * 1800) / 4096 - 1800;
	pEm->Super_vector.vy = 0xC00;
	pEm->Super_vector.vz = 0xC00 - EF->R;
	RotMatrix(&pEm->Super_vector, pEm->pSuper);
}

static unsigned long Em29_sel_hold(void)
{
	register unsigned long Randam, Flg, i;

	for (i = 0; i < 32; i++) {
		Randam = Rnd() >> 3;
		Flg = 1 << Randam;
		if (Hold_info & Flg) continue;
		Hold_info |= Flg;
		return Randam;

	}

	Flg = 1;
	for (Randam = 0; Randam < 32; Randam++, Flg <<= 1) {
		if (Hold_info & Flg) continue;
		Hold_info |= Randam;	/* retail ORs the index here, not the bit */
		return Randam;

	}

}

static unsigned long Dm_eff(ENEMY_WORK *pEm, unsigned long Wep_no)
{	extern unsigned long Dm_eff_disp(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Z);	/* [INFERRED] block-scope declaration of the function called here: opens the record-less SYM block */

	switch (Wep_no) {
	case 1:
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		Dm_eff_disp(pEm, 0, 16, 0x1000);
		break;
	case 9:
	case 10:
		Dm_eff_disp(pEm, 5, 4, 0x1300);
		break;

	case 11:
		Dm_eff_disp(pEm, 4, 9, 0x800);
		break;
	case 12:
		break;
	case 13:
		Dm_eff_disp(pEm, 0, 16, 0x1000);
		break;
	case 14:
		EF->Spark_timer = 7;
		break;
	case 15:
		Dm_eff_disp(pEm, 0, 16, 0x1000);
		break;
	case 16:
	case 17:
		Dm_eff_disp(pEm, 5, 4, 0x1300);
		break;
	case 18:
		Dm_eff_disp(pEm, 0, 16, 0x1000);
		break;
	case 19:
		Dm_eff_disp(pEm, 5, 4, 0x1300);
	}
}

static unsigned long Dm_eff_disp(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Z)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	SVECTOR Svec = { 0, 0, 0, 0 };

	Esp_call((Id << 24) | (Type << 16) | Z, (signed char)(char)Rnd() << 4, &pParts->Workm, &Svec);
}

static unsigned long Dm_eff_explosion(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	pP_ptr->Be_flg |= 0x4A;
	pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
	pP_ptr->Free[4] = 0;
	pP_ptr->Free[2] = 0;
	pP_ptr->Free[1] = -10;
	pP_ptr->Free[3] = 0;
	pP_ptr->Free[6] = 0; }
