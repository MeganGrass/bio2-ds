/* em28.c - reconstruction of D:\Bio2\prog\EM28.C (enemy 28).
 * Linked into two overlays from the same source: 0x0D (at 0x801000fc) and
 * 0x2B (at 0x8010d0fc); only the enemy-number-specific entry names differ
 * (Em128/Em228, Pldamege_Em128/228, Pldie_Em128/228), selected by OVL_2B.
 * Types come from the TU's retail SYM preamble (em28.h). */
#include "EM28.H"

/* Em28 work variables kept in the enemy's generic Free[] area */
#define EF	((EM28_FREE *)OVL_FREE(pEm))

/* Byte fields that are plain char; the retail code reads them as signed bytes (lb) in these tests. */
#define S8(x)	(*(signed char *)&(x))
/* per-part work kept in the PARTS_W generic Free[] area (Em28_ude) */
#define PF	((EM28_P_FREE *)pP_ptr->Free)

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of other translation units ----
 * EM28.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail 0xFA (0x80106404) */
static unsigned short Em2b_life_easy = 0xC8;	/* retail 0xC8 */
/* retail-only second set, selected by G.System_flg & 0x40 ([INFERRED] _flg40 names as in em10/em20/em25;
 * retail .data 0x80106408..0x80106437); unreferenced by this TU's code */
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static void (*Em28_routine_0[8])() = {
	&Em28_Init,
	&Em28_Move,
	&Em28_Damage,
	&Em28_Die,
	&Em28_Scenario,
	0,
	0,
	&Em28_Dead
};
static char Em28_iki[64] = {
	12, 23, 35, 43, 51, 60, 66, 71, 75, 79, 82, 86, 89, 92, 94, 96, 97, 98, 99, 100, 99, 98, 97, 96, 94, 92, 89, 87, 85, 83, 80, 78, 75, 72, 68, 65, 62, 58, 54, 49, 44, 39, 34, 29, 24, 19, 13, 7, 0, -6, -15, -11, -26, -30, -32, -34, -35, -35, -34, -31, -25, -20, -10, 0
};
static char Em28_me_tate = 0;
static char Em28_me_yoko = 0;
static short Em28_me_tate_mv = 0;
static short Em28_me_yoko_mv = 0;
static short Em28_me_tate_a = 0;
static short Em28_me_yoko_a = 0;
static char Em28_me_tate_d = 0;
static char Em28_me_yoko_d = 0;
static void (*Em28_Move_type[2])() = {
	&Em28_Move_type00,
	&Em28_Move_type01
};
static void (*Em28_mv_br_type00[10])() = {
	&Em28_mv00_br,
	&Em28_mv01_br,
	&Em28_mv02_br,
	&Em28_mv03_br,
	&Em28_mv04_br,
	&Em28_mv05_br,
	&Em28_mv06_br,
	&Em28_mv07_br,
	&Em28_mv08_br,
	&Em28_mv09_br
};
static void (*Em28_mv_type00[10])() = {
	&Em28_mv00,
	&Em28_mv01,
	&Em28_mv02,
	&Em28_mv03,
	&Em28_mv04,
	&Em28_mv05,
	&Em28_mv06,
	&Em28_mv07,
	&Em28_mv08,
	&Em28_mv09
};
static void (*Em28_mv_br_type01[])() = { };
static void (*Em28_mv_type01[])() = { };
static void (*Em28_dm[18][9])() = {
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 },
	{ &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0, &Em28_dm02, &Em28_dm02, 0 },	/* retail: type 17 uses Em28_dm02 (Beta Em28_dm01) */
	{ &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0, &Em28_dm01, &Em28_dm01, 0 }
};
static unsigned char Em28_Dm_cnt[18][2] = { { 2, 3 }, { 2, 1 }, { 2, 1 }, { 2, 1 }, { 10, 3 }, { 15, 3 }, { 4, 2 }, { 6, 2 }, { 16, 0 }, { 16, 0 }, { 20, 0 }, { 6, 0 }, { 2, 1 }, { 16, 0 }, { 1, 0 }, { 4, 0 }, { 20, 0 }, { 2, 0 } };	/* retail values: [8] {16,0} (Beta {16,7}), [16] {20,0} (Beta {50,10}), [17] {2,0} (Beta {1,0}); retail .data 0x80106784 */
static void (*Em28_die[][9])() = { };
static void (*Em28_sce[])() = { };
static void (*Pldamage_Em28_tbl[2])() = {
	&Em28_pldm00,
	&Em28_pldm00
};
static void (*Pldie_Em28_tbl[])() = { };

void EM_MAIN(ENEMY_WORK *pEm)
{
	short Dir;
	register short Tmp;
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register ENEMY_WORK *pEm2, **ppEm;

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		if (EF->Dm_cnt & 0x7F)
			EF->Dm_cnt--;

		Em28_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		if (EF->Ko_timer)
			EF->Ko_timer--;

		if (EF->At_timer)
			EF->At_timer--;

		if (EF->Fire_c)
			EF->Fire_c--;

		if (*pEm->pNow_seq & 0x8000000) {
			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}
		Em28_core_me_mv(pEm, pEm->pSin_parts_ptr + 1);

		if (EF->Etc_flg & 0x100) {
			pP_ptr = pEm->pSin_parts_ptr + 1;
			Tmp = ((short *)pP_ptr->pSa_dat_head)[6];
			((short *)pP_ptr->pSa_dat_head)[6] = Tmp - (signed char)Em28_iki[ EF->Iki_timer ] * 5;

			EF->Iki_timer++;
			EF->Iki_timer &= 0x3F;
			Sa_parts_allmv(pEm, pP_ptr);
			Sa_parts_offset_allmv(pEm, pP_ptr);

			((short *)pP_ptr->pSa_dat_head)[6] = Tmp;
		}

		Em28_mgdm(pEm);

		Em28_ude(pEm, pEm->pSin_parts_ptr + 9);

		pEm->pEnemy_ptr = (ENEMY_WORK *)&G.Player;
		if (!(EF->Etc_flg & 0x4000)) {	/* retail: neck turn skipped while flagged 0x4000 */
		Rot_neck_em_n(pEm, pEm->Cdir_y, &Dir);
		Dir &= 0xFFF;
		if (Dir > 0x800)
			Dir -= 0xFFF;
		Dir /= 3;
		pP_ptr = pEm->pSin_parts_ptr + 2;
		pP_ptr[0].Cdir_y += Dir;
		RotMatrix(&pP_ptr[0].Cdir_x, pP_ptr[0].M);
		pP_ptr[0].Cdir_y -= Dir;

		pP_ptr[1].Cdir_y += Dir;
		RotMatrix(&pP_ptr[1].Cdir_x, pP_ptr[1].M);
		pP_ptr[1].Cdir_y -= Dir;

		pP_ptr[2].Cdir_y += Dir;
		RotMatrix(&pP_ptr[2].Cdir_x, pP_ptr[2].M);
		pP_ptr[2].Cdir_y -= Dir;
		}

		Em_seq_sound(pEm);

		Oba_set_ofs(pEm, &pEm->Pos_x);

		pP_ptr = pEm->pSin_parts_ptr + 3;
		pEm->Atd[1].X = pP_ptr->Workm.t[0];
		pEm->Atd[1].Z = pP_ptr->Workm.t[2];
		pP_ptr = pEm->pSin_parts_ptr + 18;
		pEm->Atd[2].X = pP_ptr->Workm.t[0];
		pEm->Atd[2].Z = pP_ptr->Workm.t[2];

		pEm->At_em_no = 0xFF;

		for (ppEm = &G.pEnemy[0]; ppEm != G.ppEnemy_end; ppEm++) {

			pEm2 = *ppEm;
			if (pEm != pEm2)

				if (pEm2->Be_flg & 1) {
					Oba_ck_hit(pEm2, pEm);
				}
		}
		Sca_ck_em(pEm, 0x400);
	}

}

static void Em28_after(ENEMY_WORK *pEm)
{
	Kg_Jh_move(pEm);
}

static void Em28_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;

	if (G.System_flg & 0x40) {	/* retail-only second life set */
		pEm->Life = 600;
		if (G.System_flg & 0x20)
			pEm->Life = 450;
	}
	else {
		pEm->Life = 600;
		if (G.System_flg & 0x20)
			pEm->Life = 400;
	}

	pEm->D_life_u = 0x82;
	pEm->D_life_c = 0x82;
	pEm->D_life_d = 0x82;

	pEm->Be_flg |= 0x400000;

	pP_ptr = pEm->pSin_parts_ptr + 4;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 4;
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 0x20;
	pP_ptr->Free[3] = 0x20;
	pP_ptr->Free[4] = 0x32C;
	pP_ptr->Free[5] = 0x138;

	EF->Ko_timer = 10;
	EF->Re_timer = 0;
	EF->Kudure_t = (Rnd() >> 1) + 0x78;
	EF->At_timer = 0;
	EF->Etc_flg = 0;
	EF->Ude_life = 6;
	EF->Iki_timer = 0;
	EF->Dm_cnt = 0;
	EF->Mg_rout0 = 0;
	EF->Fire_c = 0;

	Em28_me_tate = 0;
	Em28_me_yoko = 0;
	Em28_me_tate_mv = 0;
	Em28_me_yoko_mv = 0;
	Em28_me_tate_a = 0;
	Em28_me_yoko_a = 0;
	Em28_me_tate_d = 0;
	Em28_me_yoko_d = 0;

	pEm->nOba = 3;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 1300;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].At_d = 1300;
	pEm->Atd[0].W = 1300;
	pEm->Atd[0].D = 1300;

	pEm->Atd[1].At_w = 800;
	pEm->Atd[1].At_h = 1500;
	pEm->Atd[1].At_d = 800;
	pEm->Atd[1].W = 800;
	pEm->Atd[1].D = 800;
	pEm->Atd[1].Ofs_x = 0;
	pEm->Atd[1].Ofs_y = -1500;
	pEm->Atd[1].Ofs_z = 0;

	pEm->Atd[2].At_w = 800;
	pEm->Atd[2].At_h = 1500;
	pEm->Atd[2].At_d = 800;
	pEm->Atd[2].W = 800;
	pEm->Atd[2].D = 800;
	pEm->Atd[2].Ofs_x = 0;
	pEm->Atd[2].Ofs_y = -1500;
	pEm->Atd[2].Ofs_z = 0;

	pEm->Eff_at_r = 800;

	pEm->Be_flg |= 0xC000000;

	*(long *)&pEm->Move_no = 1;

	Joint_move(pEm, Kan, Seq, 0x100);

	Kage_work_set(&pEm->pKage_work, 0, 0x5140514, 0, &pEm->Pos_x);	/* retail: shadow follows Pos (Beta: parts[0] Workm.t) */

	G.pPldamage_Em[ pEm->Id - 0x10 ] = PLDAMEGE_EM;
	G.pPldie_Em[ pEm->Id - 0x10 ] = PLDIE_EM;

	pEm->Be_flg |= 4;

	pP_ptr = pEm->pSin_parts_ptr + 9;
	pP_ptr[0].Be_flg &= ~0x40;
	pP_ptr[0].Be_flg &= ~0x2000;
	pP_ptr[0].Poly_rgb = 0x808080;
	pP_ptr[0].Size_x = 0x1000;
	pP_ptr[0].Size_y = 0x1000;
	pP_ptr[0].Size_z = 0x1000;

	pP_ptr[1].Be_flg &= ~0x200;
	pP_ptr[1].Be_flg &= ~0x2000;
	pP_ptr[1].Poly_rgb = 0x808080;
	pP_ptr[1].Size_x = 0x1000;
	pP_ptr[1].Size_y = 0x1000;
	pP_ptr[1].Size_z = 0x1000;

	pP_ptr[2].Be_flg &= ~0x200;
	pP_ptr[2].Be_flg &= ~0x2000;
	pP_ptr[2].Poly_rgb = 0x808080;
	pP_ptr[2].Size_x = 0x1000;
	pP_ptr[2].Size_y = 0x1000;
	pP_ptr[2].Size_z = 0x1000;

	pP_ptr[3].Be_flg &= ~0x200;
	pP_ptr[3].Be_flg &= ~0x2000;
	pP_ptr[3].Poly_rgb = 0x808080;
	pP_ptr[3].Size_x = 0x1000;
	pP_ptr[3].Size_y = 0x1000;
	pP_ptr[3].Size_z = 0x1000;

	pEm->pSin_parts_ptr[2].Be_flg |= 0x4000;
	G.pFree_work = Kg_Jh_init(pEm, G.pFree_work);
	pEm->pTafter_func = Em28_after;

	if ((pEm->Type & 0xFFF) == 1) {
		pEm->Routine_1 = 1;
		pEm->Type = 0;
		EF->Etc_flg |= 0x100;
		return;
	}

	pEm->Type |= 0x4000;
	pP_ptr = pEm->pSin_parts_ptr + 1;
	((short *)pP_ptr->pSa_dat_head)[6] = 0x1000;
	((short *)pP_ptr->pSa_dat_head)[8] = 0;
	Sa_parts_allmv(pEm, pP_ptr);
	Sa_parts_offset_allmv(pEm, pP_ptr);
	*(long *)&pEm->Move_no = 12;

	Joint_move(pEm, Kan, Seq, 0x10100);
	pEm->Routine_1 = 8;

	pP_ptr = pEm->pSin_parts_ptr + 2;
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0;
	pP_ptr->Size_y = 0x1000;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 3;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0;
	pP_ptr->Size_y = 0x1000;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 4;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0;
	pP_ptr->Size_y = 0x1000;
	pP_ptr->Size_z = 0x1000;

	pP_ptr = pEm->pSin_parts_ptr + 9;
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 10;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 11;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 12;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;

	pP_ptr = pEm->pSin_parts_ptr + 13;
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 14;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;
	pP_ptr = pEm->pSin_parts_ptr + 15;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x1000;
	pP_ptr->Size_y = 0;
	pP_ptr->Size_z = 0x1000;

	pP_ptr = pEm->pSin_parts_ptr + 16;
	pP_ptr->Be_flg |= 0x400;
	pP_ptr->Size_x = 0x400;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x400;
	pP_ptr = pEm->pSin_parts_ptr + 17;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x400;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x400;

	pP_ptr = pEm->pSin_parts_ptr + 18;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x800;
	pP_ptr = pEm->pSin_parts_ptr + 19;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x800;
	pP_ptr = pEm->pSin_parts_ptr + 20;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x800;
	pP_ptr = pEm->pSin_parts_ptr + 21;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x800;
	pP_ptr = pEm->pSin_parts_ptr + 22;
	pP_ptr->Be_flg |= 0xC00;
	pP_ptr->Size_x = 0x800;
	pP_ptr->Size_y = 0xC00;
	pP_ptr->Size_z = 0x800;

	pEm->Neck_flg |= 2;

}

static void Em28_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em28_Move_type[ pEm->Type & 0x3F ](pEm, Kan, Seq); }

static void Em28_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em28_mv_br_type00[ pEm->Routine_1 ](pEm);

	Em28_mv_type00[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em28_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em28_mv_br_type01[ pEm->Routine_1 ](pEm);

	Em28_mv_type01[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em28_mv00_br(ENEMY_WORK *pEm)
{

	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
	Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x320);

	*(long *)&pEm->Routine_0 = 0x101;

}

static void Em28_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = ((Rnd() & 0x1F) << 8) + 0x3F0000;
		pEm->Routine_2 = 1;

		pEm->Neck_flg &= ~2;

	case 1:
		Joint_move(pEm, Kan, Seq, 0x40);
	}
}

static void Em28_mv01_br(ENEMY_WORK *pEm)
{
	register short Dir1;
	register unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;
	short Ck0, Ck1;	/* inferred names: no retail SYM records (single-use results are combined away) */

	LL = pEm->L_pl;

	Ck0 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x4C8);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338);
	Ck1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (Ck1)
		EF->Re_timer++;

	if (EF->Kudure_t)
		EF->Kudure_t--;

	if (Ck0 && LL < 6000 && EF->Re_timer > 60)
		*(long *)&pEm->Routine_0 = 0x201;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
	Sv.vx = 2000;
	Sv.vy = 0;
	Sv.vz = 0;
	ApplyMatrix(&M, &Sv, &V);
	V.vx += pEm->Pos_x;
	V.vz += pEm->Pos_z;
	if (G.Enemy_num < 5 && !EF->Ko_timer && !Sca_ck_hit(&V, 400, 1 << (pEm->Ground / -1800), 0x400))

		*(long *)&pEm->Routine_0 = 0x601;

	if (!Dir1 && LL < 3500 && !G.Player.Damage_cnt && !EF->At_timer)
		*(long *)&pEm->Routine_0 = 0x301;

	if (!Dir1 && LL < 2000)
		*(long *)&pEm->Routine_0 = 0x901;

}

static void Em28_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0001;
		pEm->Routine_2 = 1;

		pEm->Neck_flg &= ~2;

	case 1:
		if (pEm->Spd_x >= 16) {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
		}
		if (pEm->Spd_x >= 11) {
			Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);

		if (!Hit_ck_point4(&pEm->Pos_x, G.Cccut))
			pEm->Spd_x += 50;

		Add_speedXZ(pEm, 0);

	}
}

static void Em28_mv02_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if ((signed char)EF->Dm_cnt < 3)
		EF->Dm_cnt |= 0x82;

	if ((signed char)EF->Ude_life < 2)
		EF->Ude_life = 1;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0008;
		pEm->Routine_2 = 1;
		EF->Re_timer = 0;

		pEm->Neck_flg |= 2;
		EF->Etc_flg |= 0x2000;	/* retail: dying now restores the routine (Em28_Die) */

	case 1:
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, -32);
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {

			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Cdir_y += 0x800;
			EF->Kudure_t = (Rnd() >> 1) + 0x78;

			EF->Dm_cnt &= 0x7F;
			EF->Etc_flg &= ~0x2000;	/* retail */

			break;
		}
		Add_speedXZ(pEm, 0);
	}
}

static void Em28_mv03_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned short Ck;
	register PARTS_W *pP;	/* inferred name: no retail SYM record (CSE folds it into the parts base) */

	if ((signed char)EF->Dm_cnt < 3)
		EF->Dm_cnt |= 0x82;

	if ((signed char)EF->Ude_life < 2)
		EF->Ude_life = 1;

	EF->At_timer = 15;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0004;
		pEm->Routine_2 = 1;

		pEm->Timer0 = 0;
		pEm->Timer1 = 0;

		pEm->Neck_flg |= 2;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40)) {

			pEm->Cdir_y += 0x400;
			*(long *)&pEm->Routine_0 = 0x101;

			EF->Dm_cnt &= 0x7F;

			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 16);

			EF->Kudure_t = (Rnd() >> 1) + 0x78;
		}
		Add_speedXZ(pEm, 0);

		if (pEm->Move_cnt > 5 && pEm->Move_cnt < 30) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
			if (pEm->L_pl > 4500 || Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x4C8)) {

				*(long *)&pEm->Routine_0 = 0x101;

				EF->Dm_cnt &= 0x7F;

				EF->At_timer = 15;
			}
		}

		pEm->Cdir_y += 0x200;
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 4);
		pEm->Cdir_y -= 0x200;

		Ck = pEm->Move_cnt - 54;
		if (Ck < 22) {
			pEm->Cdir_y += 0x200;
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 32);
			pEm->Cdir_y -= 0x200;

			if (!pEm->Timer0) {
				pP = pEm->pSin_parts_ptr + 10;

				if ((L_pos_ckXZ(&pP[2].Workm.t[0], &G.Player.Pos_x, 800) || L_pos_ckXZ(&pP->Workm.t[0], &G.Player.Pos_x, 1000)) && !G.Player.Damage_cnt) {

					if (Pl_life_down(30, 0) == 2) {
						*(long *)&G.Player.Routine_0 = 3;
					}else {
						*(long *)&G.Player.Routine_0 = 5;
						Ck = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z) - 0x400;

						if ((((short)Ck - G.Player.Cdir_y + 0x400) & 0xFFF) < 0x800)
							G.Player.Routine_1++;
					}
					G.Player.Damage_cnt |= 0x80;
					G.Player.pEnemy_ptr = pEm;
					G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
					G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
					pEm->Timer0 = 1;
					pEm->Timer1 = 4;
					Pad_vib_set0(3, 0);	/* retail-only (Dual Shock) */
					Pad_vib_set1(10, 200, 0);

					Snd_se_enem(6, pEm);

				}
			}
		}

		if (pEm->Timer0) {
			if (pEm->Timer1) {
				pEm->Timer1--;

				pP = pEm->pSin_parts_ptr + 12;
				Esp_call((pEm->Timer1 * 200 + 0x818) | 0x10000, 0, &pP->Workm, 0);
			}
		}

		break;

	case 2:
		EF->At_timer = 30;

	}
}

static void Em28_mv04_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0002;
		pEm->Routine_2 = 1;

		pEm->Neck_flg |= 2;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40))

			*(long *)&pEm->Routine_0 = 0x501;

		Add_speedXZ(pEm, 0);
	}
}

static void Em28_mv05_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0003;
		pEm->Routine_2 = 1;

		pEm->Neck_flg &= ~2;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40))

			*(long *)&pEm->Routine_0 = 0x101;

		Add_speedXZ(pEm, 0);
	}
}

static void Em28_mv06_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pKo;
	SVECTOR Sv;
	register PARTS_W *pP;	/* inferred name: no retail SYM record (CSE folds it into the parts base) */

	if ((signed char)EF->Dm_cnt < 3)
		EF->Dm_cnt |= 0x82;

	if ((signed char)EF->Ude_life < 2)
		EF->Ude_life = 1;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0005;
		pEm->Routine_2 = 1;

		if (!(EF->Etc_flg & 0x400)) {
			G.pEnemy[3]->Be_flg = 0;
			EF->Etc_flg |= 0x400;
			G.Enemy_num--;
		}

		pKo = Set_enemy_work(0x27);
		pP = pEm->pSin_parts_ptr + 4;
		pKo->Pos_x = pP->Workm.t[0];
		pKo->Pos_y = pEm->Pos_y;
		pKo->Pos_z = pP->Workm.t[2];
		pKo->Cdir_y = pEm->Cdir_y - (short)(Rnd() - 0x80);
		pKo->Type = 2;

		pEm->Timer0 = 120;

		pEm->Neck_flg |= 2;

	case 1:
		if (pEm->Timer0 == 100 && G.Enemy_num < 5) {
			pKo = Set_enemy_work(0x27);
			pP = pEm->pSin_parts_ptr + 4;
			pKo->Pos_x = pP->Workm.t[0];
			pKo->Pos_y = pEm->Pos_y;
			pKo->Pos_z = pP->Workm.t[2];
			pKo->Cdir_y = pEm->Cdir_y - Rnd() + 0x880;
			pKo->Type = 2;
		}

		if (pEm->Timer0 == 90 && G.Enemy_num < 5 && (Rnd() & 1)) {
			pKo = Set_enemy_work(0x27);
			pP = pEm->pSin_parts_ptr + 4;
			pKo->Pos_x = pP->Workm.t[0];
			pKo->Pos_y = pEm->Pos_y;
			pKo->Pos_z = pP->Workm.t[2];
			pKo->Cdir_y = pEm->Cdir_y - Rnd() + 0x480;
			pKo->Type = 2;
		}

		if (Joint_move(pEm, Kan, Seq, 0x40)) {

			pEm->Hokan_flg = 0x3F;

			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}

		if (pEm->Timer0 < 60 && !(pEm->Timer0 & 0xF)) {
			pP = pEm->pSin_parts_ptr + 4;
			Sv.vx = 0x80 - Rnd();
			Sv.vy = 0;
			Sv.vz = 0x80 - Rnd();
			Esp_call(0x151000, pEm->Cdir_y, &pP->Workm, &Sv);
		}

		if ((unsigned short)pEm->Timer0-- <= 0) {
			*(long *)&pEm->Routine_0 = 0x101;

			EF->Dm_cnt &= 0x7F;

			EF->Ko_timer = Rnd() + 300;
			if (G.Enemy_num < 4)
				EF->Ko_timer >>= 1;

			if (pEm->L_pl < 4000 && !G.Player.Damage_cnt)
				*(long *)&pEm->Routine_0 = 0x301;
		}

	case 2:

	}
}

static void Em28_mv07_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F000A;
		pEm->Routine_2 = 1;

		EF->Kudure_t = (Rnd() >> 1) + 0x78;

		pEm->Neck_flg &= ~2;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40))

			*(long *)&pEm->Routine_0 = 0x101;

		Add_speedXZ(pEm, 0);
	}
}

static void Em28_mv08_br(ENEMY_WORK *pEm)
{

}

static void Em28_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;

	short Em28_mv08_hen[32] = { 5, 14, 26, 37, 44, 48, 50, 48, 45, 40, 33, 26, 19, 14, 7, 0, -5, -10, -15, -19, -23, -26, -28, -30, -29, -27, -23, -19, -16, -11, -6, -1 };

	switch (pEm->Routine_2) {
	case 0:

		*(long *)&pEm->Move_no = 0x7000C;

		pEm->Timer0 = 0;
		pEm->Timer1 = 1;
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Size_x = 0x200;
		pEm->Timer3 = (Rnd() & 7) + 7;
		pEm->Timer2 = 5;

		pP_ptr = pEm->pSin_parts_ptr + 13;
		pP_ptr->Be_flg |= 0x4000;
		pP_ptr[1].Be_flg |= 0x4000;
		pP_ptr[2].Be_flg |= 0x4000;

		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Be_flg |= 0x4000;

		if (pEm->Sce_free0 & 1)
			pEm->Routine_2 = 1;

		pP_ptr = pEm->pSin_parts_ptr + 2;

		pP_ptr->Poly_rgb = 0x608060;
		pP_ptr = pEm->pSin_parts_ptr + 4;
		pP_ptr->Poly_rgb = 0x608060;

		pP_ptr = pEm->pSin_parts_ptr + 9;
		pP_ptr->Poly_rgb = 0x608060;
		pP_ptr[1].Poly_rgb = 0x608060;
		pP_ptr[2].Poly_rgb = 0x608060;
		pP_ptr[3].Poly_rgb = 0x608060;

		pP_ptr = pEm->pSin_parts_ptr + 13;
		pP_ptr->Poly_rgb = 0x608060;
		pP_ptr[1].Poly_rgb = 0x608060;
		pP_ptr[2].Poly_rgb = 0x608060;

		pP_ptr = pEm->pSin_parts_ptr + 16;
		pP_ptr->Poly_rgb = 0x608060;
		pP_ptr[1].Poly_rgb = 0x608060;

		break;

	case 1:
		if ((unsigned char)pEm->Timer3-- <= 0) {
			pEm->Routine_2 = 2;

			Snd_se_enem(8, pEm);

			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}

		break;

	case 2:
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_2 = 1;
			pEm->Timer3 = (Rnd() & 7) + 7;
			if ((unsigned char)pEm->Timer2-- <= 0) {
				pEm->Routine_2 = 3;
				pEm->Timer3 = 120;

				pEm->Timer2 = 0;

				Em28_me_tate_d = 0;
				Em28_me_yoko_d = 0;

				Snd_se_enem(9, pEm);
			}
		}

		break;

	case 3:
		pEm->Timer0++;

		pEm->Hokan_flg = 0x30;
		Joint_move(pEm, Kan, Seq, 0x40);
		if ((unsigned char)pEm->Timer3-- <= 0) {
			pEm->Routine_2 = 4;
			*(long *)&pEm->Move_no = 0x3F000D;
		}
		break;

	case 4:
		pEm->Timer0++;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x40);

		if (pEm->Timer0 & 0x10) {
			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}

		break;

	case 5:
		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] = 0;

		*(long *)&pEm->Routine_0 = 0x101;

		pEm->Type &= ~0x4000;

		pP_ptr = pEm->pSin_parts_ptr + 16;
		pP_ptr->Be_flg &= ~0xC00;
		pP_ptr[1].Be_flg &= ~0xC00;
		pP_ptr[2].Be_flg &= ~0xC00;
		pP_ptr[3].Be_flg &= ~0xC00;
		pP_ptr[4].Be_flg &= ~0xC00;
		pP_ptr[5].Be_flg &= ~0xC00;
		pP_ptr[6].Be_flg &= ~0xC00;

		Em28_me_tate_d = 0;
		Em28_me_yoko_d = 0;

	}

	pEm->Timer1 *= -1;

	if (pEm->Timer0 >= 2 && pEm->Timer0 < 15) {
		pP_ptr = pEm->pSin_parts_ptr + 1;
		Sv.vx = 0x3F4 - (Rnd() << 2);
		Sv.vy = -200;
		Sv.vz = 0x200 - (Rnd() << 2);
		Esp_call(((Rnd() << 3) + 0x17D0) | 0x80000, Rnd() << 4, &pP_ptr->Workm, &Sv);
		Sv.vx = 0x3F4 - (Rnd() << 2);
		Sv.vy = -200;
		Sv.vz = 0x200 - (Rnd() << 2);
		Esp_call(((Rnd() << 3) + 0x17D0) | 0x100000, Rnd() << 4, &pP_ptr->Workm, &Sv);
	}

	if (pEm->Timer0 >= 2 && !(EF->Etc_flg & 8)) {
		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[6] -= Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
		pEm->Timer2++;
		pEm->Timer2 &= 0x1F;
		if (((short *)pP_ptr->pSa_dat_head)[6] < 0) {
			((short *)pP_ptr->pSa_dat_head)[6] = 0;
			EF->Etc_flg |= 0x108;
		}
		Sa_parts_allmv(pEm, pP_ptr);
		Sa_parts_offset_allmv(pEm, pP_ptr);

		if (!((signed char)pEm->Timer2 & 1)) {
			Sv.vx = 0x3F4 - (Rnd() << 2);
			Sv.vy = -200;
			Sv.vz = 0x200 - (Rnd() << 2);
			Esp_call(((((Rnd() & 1) << 3) + 8) << 16) | ((Rnd() << 3) + 0xC18), Rnd() << 4, &pP_ptr->Workm, &Sv);

		}
	}

	if (pEm->Timer0 >= 0xE7) {

		pP_ptr = pEm->pSin_parts_ptr + 4;
		if (pP_ptr->Size_x < 0x1000) {
			pP_ptr->Size_x += (pEm->Timer0 & 1) * 100 - 10;
			if ((signed char)pEm->Timer2 < 5) {
				Sv.vx = 0x40 - (Rnd() & 0x7F);
				Sv.vy = 0x40 - (Rnd() & 0x7F);
				Sv.vz = 0x40 - (Rnd() & 0x7F);

			}

			pP_ptr = pEm->pSin_parts_ptr + 2;
			if ((pP_ptr->Poly_rgb & 0xFF) != 0x80) {
				pP_ptr->Poly_rgb += 0x10001;
				pP_ptr = pEm->pSin_parts_ptr + 4;
				pP_ptr->Poly_rgb += 0x10001;
			}

		}else {
			pP_ptr->Be_flg &= ~0xC00;
		}
	}

	if (pEm->Timer0 >= 0xA1) {

		pP_ptr = pEm->pSin_parts_ptr + 2;

		pP_ptr[2].Be_flg &= ~0x4000;
		if (pP_ptr->Size_x < 0x1000) {
			pP_ptr->Size_x += (pEm->Timer0 & 1) * 70 + 10;
			pP_ptr[1].Size_x += (pEm->Timer0 & 1) * 70 + 10;

			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40;
			Sv.vz = 0x40 - (Rnd() & 0x7F);
			Esp_call(((Rnd() << 2) + 0x1000) | 0x80000, Rnd() << 4, &pP_ptr->Workm, &Sv);

			Sv.vy = -0x40;
			Esp_call(((((Rnd() & 1) << 3) + 8) << 16) | ((Rnd() << 2) + 0xC18), Rnd() << 4, &pP_ptr->Workm, &Sv);

		}else {
			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
			if (!(pEm->Timer0 & 7)) {
				Sv.vx = 0x40 - (Rnd() & 0x7F);
				Sv.vy = 0x40 - (Rnd() & 0x7F);
				Sv.vz = 0x40 - (Rnd() & 0x7F);
				Esp_call(((((Rnd() & 1) << 3) + 8) << 16) | ((Rnd() << 2) + 0xC18), Rnd() << 4, &pP_ptr->Workm, &Sv);

			}
		}
	}

	if (pEm->Timer0 >= 0x65) {

		pP_ptr = pEm->pSin_parts_ptr + 9;
		if (pP_ptr->Size_y < 0x1000) {
			pP_ptr->Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[1].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[2].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[3].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;

		}else {
			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
			pP_ptr[2].Be_flg &= ~0xC00;
			pP_ptr[3].Be_flg &= ~0xC00;

			if ((pP_ptr->Poly_rgb & 0xFF) != 0x80) {
				pP_ptr->Poly_rgb += 0x10001;
				pP_ptr[1].Poly_rgb += 0x10001;
				pP_ptr[2].Poly_rgb += 0x10001;
				pP_ptr[3].Poly_rgb += 0x10001;
			}
		}

		if ((signed char)pEm->Timer2 < 15 || !(pEm->Timer0 & 7)) {
			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);
			Esp_call(((Rnd() << 3) + 0x830) | 0x80000, Rnd() << 4, &pP_ptr->Workm, &Sv);

		}
	}

	if (pEm->Timer0 >= 0x3D) {
		pP_ptr = pEm->pSin_parts_ptr + 13;
		pP_ptr->Be_flg &= ~0x4000;
		pP_ptr[1].Be_flg &= ~0x4000;
		pP_ptr[2].Be_flg &= ~0x4000;
		if (pP_ptr->Size_y < 0x1000) {
			pP_ptr->Size_y += (pEm->Timer0 & 1) * 80 + 20;
			pP_ptr[1].Size_y += (pEm->Timer0 & 1) * 80 + 20;
			pP_ptr[2].Size_y += (pEm->Timer0 & 1) * 80 + 20;
			if (pEm->Timer0 & 1) {
				Sv.vx = 0x40 - (Rnd() & 0x7F);
				Sv.vy = 0x40 - (Rnd() & 0x7F);
				Sv.vz = 0x40 - (Rnd() & 0x7F);
			}

		}else {
			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
			pP_ptr[2].Be_flg &= ~0xC00;

			if ((pP_ptr->Poly_rgb & 0xFF) != 0x80) {
				pP_ptr->Poly_rgb += 0x10001;
				pP_ptr[1].Poly_rgb += 0x10001;
				pP_ptr[2].Poly_rgb += 0x10001;
			}
		}

		if ((signed char)pEm->Timer2 < 15) {
			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);

		}
	}

	if (pEm->Timer0 >= 2) {
		pP_ptr = pEm->pSin_parts_ptr + 16;
		if (pP_ptr->Size_y < 0x1000 && !(EF->Etc_flg & 0x10)) {
			pP_ptr->Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[1].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;

			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);
			Esp_call(((((Rnd() & 1) << 3) + 8) << 16) | ((Rnd() << 3) + 0xC18), Rnd() << 4, &pP_ptr->Workm, &Sv);

		}else {
			pP_ptr->Size_y = 0x1000;
			pP_ptr[1].Size_y = 0x1000;
			EF->Etc_flg |= 0x10;

			if (pEm->Timer0 >= 0xC9 && (pP_ptr->Poly_rgb & 0xFF) != 0x80) {
				pP_ptr->Poly_rgb += 0x10001;
				pP_ptr[1].Poly_rgb += 0x10001;
			}
		}

		if ((signed char)pEm->Timer2 < 15 || !(pEm->Timer0 & 7)) {
			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);

		}

		if (pP_ptr->Size_x < 0x1000 && !(EF->Etc_flg & 0x20)) {
			pP_ptr->Size_x += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[1].Size_x += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr->Size_z += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[1].Size_z += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
		}else {
			pP_ptr->Size_x = 0x1000;
			pP_ptr[1].Size_x = 0x1000;
			pP_ptr->Size_z = 0x1000;
			pP_ptr[1].Size_z = 0x1000;
			EF->Etc_flg |= 0x20;

			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
		}
	}

	if (pEm->Timer0 >= 2) {
		pP_ptr = pEm->pSin_parts_ptr + 18;
		if (pP_ptr->Size_y < 0x1000 && !(EF->Etc_flg & 0x40)) {
			pP_ptr->Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[1].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[2].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[3].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;
			pP_ptr[4].Size_y += Em28_mv08_hen[ (signed char)pEm->Timer2 ] * 3;

			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);

		}else {
			pP_ptr->Size_y = 0x1000;
			pP_ptr[1].Size_y = 0x1000;
			pP_ptr[2].Size_y = 0x1000;
			pP_ptr[3].Size_y = 0x1000;
			pP_ptr[4].Size_y = 0x1000;

			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
			pP_ptr[2].Be_flg &= ~0xC00;
			pP_ptr[3].Be_flg &= ~0xC00;
			pP_ptr[4].Be_flg &= ~0xC00;

			EF->Etc_flg |= 0x40;
		}

		if ((signed char)pEm->Timer2 < 15 || !(pEm->Timer0 & 7)) {
			Sv.vx = 0x40 - (Rnd() & 0x7F);
			Sv.vy = 0x40 - (Rnd() & 0x7F);
			Sv.vz = 0x40 - (Rnd() & 0x7F);

		}

		if (pP_ptr->Size_x < 0x1000 && !(EF->Etc_flg & 0x80)) {
			pP_ptr->Size_x += (pEm->Timer0 & 1) * 20 + 10;
			pP_ptr[1].Size_x += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[2].Size_x += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[3].Size_x += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[4].Size_x += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr->Size_z += (pEm->Timer0 & 1) * 20 + 10;
			pP_ptr[1].Size_z += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[2].Size_z += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[3].Size_z += (pEm->Timer0 & 1) * 10 + 10;
			pP_ptr[4].Size_z += (pEm->Timer0 & 1) * 10 + 10;
		}else {
			pP_ptr->Size_x = 0x1000;
			pP_ptr[1].Size_x = 0x1000;
			pP_ptr[2].Size_x = 0x1000;
			pP_ptr[3].Size_x = 0x1000;
			pP_ptr[4].Size_x = 0x1000;
			pP_ptr->Size_z = 0x1000;
			pP_ptr[1].Size_z = 0x1000;
			pP_ptr[2].Size_z = 0x1000;
			pP_ptr[3].Size_z = 0x1000;
			pP_ptr[4].Size_z = 0x1000;
			EF->Etc_flg |= 0x80;

			pP_ptr->Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;
			pP_ptr[2].Be_flg &= ~0xC00;
			pP_ptr[3].Be_flg &= ~0xC00;
			pP_ptr[4].Be_flg &= ~0xC00;
		}
	}

}

static void Em28_mv09_br(ENEMY_WORK *pEm)
{
	register short Dir1;
	register unsigned long LL;
	SVECTOR Sv;
	VECTOR V;
	MATRIX M;
	short Ck0, Ck1;	/* inferred names: no retail SYM records (single-use results are combined away) */

	LL = pEm->L_pl;

	Ck0 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x5F4);
	Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x338);
	Ck1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);

	if (Ck1)
		EF->Re_timer += 2;

	if (Ck0 && LL < 5000 && EF->Re_timer > 90)
		*(long *)&pEm->Routine_0 = 0x201;

	if (!Dir1 && LL < 3500 && !G.Player.Damage_cnt && !EF->At_timer && pEm->Timer0 < 30)

		*(long *)&pEm->Routine_0 = 0x301;

	if (LL > 3500)
		*(long *)&pEm->Routine_0 = 0x101;

}

static void Em28_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x3F0001;
		pEm->Routine_2 = 1;

		pEm->Neck_flg &= ~2;

		pEm->Timer0 = (Rnd() & 0x1F) + 130;	/* retail 130 (Beta 100) */

		pEm->Spd_x = 10;
		pEm->Spd_z = 0;

	case 1:
		if (pEm->Spd_x >= 16) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 4);
		}
		if (pEm->Spd_x >= 11) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 4);
		}

		Joint_move(pEm, Kan, Seq, 0x10040);
		Add_speedXZ(pEm, 0x800);

		if ((unsigned short)pEm->Timer0-- <= 0)
			*(long *)&pEm->Routine_0 = 0x101;

	}
}

static void Em28_firedm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[16].Workm, 0);

}
static void Em28_ryudm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[16].Workm, 0);

}
static void Em28_bomdm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1FA0) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x13E8) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x5040000, pEm->Cdir_y, &pP_ptr[16].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0xC18) | 0x40F0000, pEm->Cdir_y, &pP_ptr[13].Workm, 0);
}

/* retail-only (0x80103524): effects for the retail damage type 17, Em28_bomdm's layout with
 * larger bursts.  [INFERRED] name (no Beta/SYM record). */
static void Em28_bom2dm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x3328) | 0x5040000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x3E58) | 0x5040000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x3328) | 0x5040000, pEm->Cdir_y, &pP_ptr[16].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[10].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x5040000, pEm->Cdir_y, &pP_ptr[3].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[9].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x2770) | 0x5040000, pEm->Cdir_y, &pP_ptr[5].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[2].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[0].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[13].Workm, 0);
	pP_ptr = pEm->pSin_parts_ptr;
	Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr[9].Workm, 0);
}

static void Em28_spdm(ENEMY_WORK *pEm, unsigned char Flg)
{
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = -0x368 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x17D0) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 1;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = -0x368 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
	pP_ptr = pEm->pSin_parts_ptr + 1;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x1BB8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 9;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 5;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	pP_ptr = pEm->pSin_parts_ptr + 3;
	Sv.vx = 0x80 - Rnd();
	Sv.vy = 0x80 - Rnd();
	Sv.vz = 0x80 - Rnd();
	Esp_call(((Rnd() << 3) + 0x13E8) | 0x6000000, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

	if (Flg) {
		pP_ptr = pEm->pSin_parts_ptr + 2;
		Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP_ptr->Workm, 0);
	}

}

static void Em28_mgdm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0, Sv1, Sv2;
	MATRIX M;	short Dir;

	switch (EF->Mg_rout0) {
	case 1:

		EF->Mg_rout0 = 2;
		EF->Mg_timer0 = 0;
		EF->Mg_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

	case 2:
		Sv0.vx = 0;
		Dir = Sv0.vy = EF->Mg_dir;
		Sv0.vz = 0;
		RotMatrix(&Sv0, &M);

		Sv0.vx = -((signed char)EF->Mg_timer0 * 200) - 200;
		Sv0.vy = 200;
		ApplyMatrixSV(&M, &Sv0, &Sv1);

		Sv0.vx = -200;
		Sv0.vy = 0;
		Sv0.vz = 0;
		ApplyMatrixSV(&M, &Sv0, &Sv2);

		pP_ptr = pEm->pSin_parts_ptr;
		Sv0.vx = Sv1.vx + pEm->Pos_x;
		Sv0.vy = pP_ptr->Workm.t[1];
		Sv0.vz = Sv1.vz + pEm->Pos_z;
		Esp_call((0x1080 - (signed char)EF->Mg_timer0 * 160 - Rnd()) | 0x90000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx - (short)(Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz - (short)(Rnd() - 0x80);
		Esp_call(((9 - (Rnd() & 1)) << 16) | (0x1080 - (signed char)EF->Mg_timer0 * 160 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		if ((signed char)EF->Mg_timer0 == 1)
			Esp_call(0xA1000, Dir + 0x800, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 1);
		Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 1);
		Esp_call((0x1080 - (signed char)EF->Mg_timer0 * 160 - Rnd()) | 0x90000, Dir, &GsIDMATRIX, &Sv0);

		EF->Mg_timer0++;
		Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 3 - (short)(Rnd() - 0x80);
		Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 3 - (short)(Rnd() - 0x80);
		Esp_call(((9 - (Rnd() & 1)) << 16) | (0x1080 - (signed char)EF->Mg_timer0 * 160 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

		if ((signed char)EF->Mg_timer0++ > 12)
			EF->Mg_rout0 = 0;
		break;
	case 0:	}

}

static void Em28_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;
	MATRIX M;
	register PARTS_W *pP;	/* inferred names: no retail SYM records (pP is folded into the */
	short Dir, Ck; long Ofs;	/* parts base; Dir is copy-propagated; Ck is combined; Ofs is a constant
				   carrier that keeps fold from reassociating Dir - (x - 0x200)) */

	if (!pEm->Routine_2) {
		Em28_me_tate_d = 3 - Rnd() % 6;
		Em28_me_yoko_d = 15 - Rnd() % 30;

		if (pEm->Routine_1 != 16) {

			pP = pEm->pSin_parts_ptr + 1;

			Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

			Sv.vx = 0;
			Sv.vy = Dir;
			Sv.vz = 0x80 - Rnd();
			RotMatrix(&Sv, &M);
			Sv.vx = 800;
			Sv.vy = 0;
			if (!pEm->Damage_no) {
				Sv.vx = 500;
				Sv.vy = 200;
			}
			ApplyMatrixSV(&M, &Sv, &Sv);
			Sv.vx += pP->Workm.t[0];
			Sv.vy += pP->Workm.t[1];
			Sv.vz += pP->Workm.t[2];
			Esp_call(((Rnd() << 3) + 0x1FA0) | 0x90000, Dir, &GsIDMATRIX, &Sv);
			if (Em28_Dm_cnt[ pEm->Routine_1 - 1 ][0] > 3) {
				Sv.vy += 0x40 - (Rnd() & 0x3F);
				Esp_call(((Rnd() << 3) + 0x1FA0) | 0x80000, Dir, &GsIDMATRIX, &Sv);
			}

			if (!(Rnd() & 3)) {
				Esp_call(0x1017D0, Dir, &GsIDMATRIX, &Sv);
				Ofs = 0x200; Esp_call(0x1F0817D0, Dir - ((Rnd() << 2) - Ofs), &GsIDMATRIX, &Sv);
			}
		}

		if (S8(EF->Ude_life)) {
			EF->Ude_life -= Em28_Dm_cnt[ pEm->Routine_1 - 1 ][1];
			if (S8(EF->Ude_life) <= 0)
				EF->Ude_life = 1;
			pEm->Cdir_y -= 0x23E;
			Ck = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 450);
			pEm->Cdir_y += 0x23E;
			if (!Ck && S8(EF->Ude_life) == 1)
				EF->Ude_life = 0;

		}

		if (pEm->Routine_1 == 9)
			Em28_bomdm(pEm);

		if (pEm->Routine_1 == 17)	/* retail-only damage type 17 */
			Em28_bom2dm(pEm);

		if (pEm->Routine_1 == 10)
			Em28_firedm(pEm);

		if (pEm->Routine_1 == 11)
			Em28_ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em28_spdm(pEm, 1);

		if (pEm->Routine_1 == 16 && !EF->Fire_c) {
			Em28_firedm(pEm);
			EF->Fire_c = 45;
		}

		if ((pEm->Routine_1 == 5 || pEm->Routine_1 == 6) && !EF->Mg_rout0)	/* retail: + type 6 */
			EF->Mg_rout0 = 1;
		EF->Etc_flg &= ~0x2000;	/* retail */

		if (S8(EF->Dm_cnt)) {
			*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
			if (pEm->Routine_0 != 2) {
				Em28_Move(pEm, Kan, Seq);
				return;
			}

			if ((EF->Etc_flg & 0x1000) && !S8(EF->Ude_life))
				EF->Ude_life = 1;
		}

	}

	if (!S8(EF->Ude_life)) {
		Em28_dm00(pEm, Kan, Seq);
		return;
	}

	Em28_dm[ pEm->Routine_1 - 1 ][ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em28_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Sv;
	register PARTS_W *pP;	/* inferred name: no retail SYM record (CSE folds it into the parts base) */

	EF->Dm_cnt |= 0x52;	/* retail: unconditional (Beta: only while Dm_cnt < 3) */

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x1F0002;
		pEm->Routine_2 = 1;

		EF->Etc_flg |= 1;
		pP = pEm->pSin_parts_ptr + 9;
		pP->Be_flg |= 0x40;

		pEm->Neck_flg &= ~2;

		EF->Etc_flg |= 0x200;

	case 1:
		if (pEm->Move_cnt == 35)
			Pad_vib_set1(8, 150, 0);	/* retail-only (Dual Shock) */
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x80)) {

			pEm->Routine_2 = 2;

			pP = pEm->pSin_parts_ptr + 10;
			pP[0].Be_flg |= 0x200;

			pP[1].Be_flg |= 0x200;

			pP[2].Be_flg |= 0x200;

			*(long *)&pEm->Move_no = 0x1F0009;

			pEm->Timer0 = 180;

			pEm->Timer3 = 0;
		}

		Add_speedXZ(pEm, 0);

		pP = pEm->pSin_parts_ptr + 1;

		MulMatrix0(&pP->Workm, pP[8].M, &pP[8].Workm);
		break;

	case 2:
		if ((unsigned char)pEm->Timer3-- <= 0) {
			pEm->Timer3 = (Rnd() & 1) + 1;
			pEm->Timer1 = 32 - (Rnd() & 0x3F);

			pEm->Spd_x = 16 - (Rnd() & 0x1F);
			pEm->Spd_z = 16 - (Rnd() & 0x1F);

		}
		pEm->Cdir_y += pEm->Timer1;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;

		Joint_move(pEm, Kan, Seq, 0x80);

		if ((unsigned short)pEm->Timer0-- <= 0) {
			pEm->Routine_2 = 3;

			pEm->pSin_parts_ptr[9].Timer0 = 0xFF;
		}

		if (!(pEm->Timer0 & 3)) {
			pP = pEm->pSin_parts_ptr + 1;

			Sv.vx = pP[8].Pos_x - (short)((Rnd() & 0x7F) - 0x40);
			Sv.vy = pP[8].Pos_y - (short)((Rnd() & 0x7F) - 0x40);
			Sv.vz = pP[8].Pos_z - (short)((Rnd() & 0x7F) - 0x40);
			Esp_call(((Rnd() << 3) + 0x1000) | 0x90000, pEm->Cdir_y - 0x400, &pP->Workm, &Sv);
		}

		break;

	case 3:
		*(long *)&pEm->Move_no = 0x1F000B;
		pEm->Routine_2 = 4;

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			pEm->Routine_2 = 5;
			*(long *)&pEm->Move_no = 0x1F0003;
		}
		break;

	case 5:
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			*(long *)&pEm->Routine_0 = 0x101;
			EF->Ude_life = 6;

			EF->Dm_cnt &= 0x7F;

			EF->Etc_flg &= ~0x200;
		}

	}

	if (!(pEm->Move_cnt & 0xF)) {
		Em28_me_tate_d = 3 - Rnd() % 6;
		Em28_me_yoko_d = 15 - Rnd() % 30;
	}
}

static void Em28_ude(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	register VECTOR *pPos;
	register PARTS_W *pO;
	SVECTOR Sv;
	register unsigned long i;

	if (EF->Etc_flg & 1) {
		pP_ptr->Be_flg |= 0x40;
		EF->Etc_flg |= 2;
		EF->Etc_flg &= ~1;

		PF->_R0 = 0;
	}

	if (EF->Etc_flg & 2) {

		pO = pEm->pSin_parts_ptr + 1;
		pPos = (VECTOR *)pP_ptr->Workm.t;
		switch (PF->_R0) {
		case 0:
			PF->_R0 = 1;

			pP_ptr->Timer0 = 100;
			pP_ptr->Timer2 = 4;
			pP_ptr->Timer1 = 0;

			Sv.vx = pP_ptr->Pos_x - (short)((Rnd() & 0x7F) - 0x40);
			Sv.vy = pP_ptr->Pos_y - (short)((Rnd() & 0x7F) - 0x40);
			Sv.vz = pP_ptr->Pos_z - (short)((Rnd() & 0x7F) - 0x40);
			Esp_call(0x91F40, pEm->Cdir_y - 0x400, &pO->Workm, &Sv);

		case 1:
			pP_ptr->Timer1++;

			if ((signed char)pP_ptr->Timer1 < 90)
				if (!(pP_ptr->Timer1 & 3)) {
					Sv.vx = pP_ptr->Pos_x - (short)((Rnd() & 0x7F) - 0x40);
					Sv.vy = pP_ptr->Pos_y - (short)((Rnd() & 0x7F) - 0x40);
					Sv.vz = pP_ptr->Pos_z - (short)((Rnd() & 0x7F) - 0x40);
					Esp_call(((Rnd() << 3) + 0x1000) | 0x80000, pEm->Cdir_y - 0x400, &pO->Workm, &Sv);

					Sv.vx = 0x40 - (Rnd() & 0x7F);
					Sv.vy = 0x40 - (Rnd() & 0x7F);
					Sv.vz = 0x40 - (Rnd() & 0x7F);
					Esp_call(((Rnd() << 3) + 0x1000) | 0x80000, 0, &pP_ptr->Workm, &Sv);
				}

			pP_ptr->Timer0 += 30;
			pPos->vy += pP_ptr->Timer0;

			if (pEm->Ground - 200 < pPos->vy) {
				pPos->vy = pEm->Ground - 200;
				pP_ptr->Timer2--;
				pP_ptr->Timer0 *= -1;
				pP_ptr->Timer0 >>= 1;

				Sv.vx = 0x40 - (Rnd() & 0x7F);
				Sv.vy = 0x40 - (Rnd() & 0x7F);
				Sv.vz = 0x40 - (Rnd() & 0x7F);
				Esp_call(((Rnd() << 3) + 0x1000) | 0x80000, 0, &pP_ptr->Workm, &Sv);

				if (!S8(pP_ptr->Timer2))
					PF->_R0 = 2;
			}

			break;

		case 2:
			PF->_R0 = 3;
			i = 4;
			do {
				pO = &pP_ptr[ i ] - 1;
				pO->Be_flg |= 0x2000;
				pO->Size_x = 0x1000;
				pO->Size_y = 0x1000;
				pO->Size_z = 0x1000;
			}while (--i);

			pP_ptr->Timer0 = 0;

		case 3:
			pP_ptr->Timer0++;

			pPos->vy += 2;
			i = 4;
			do {
				pO = &pP_ptr[ i ] - 1;
				pO->Be_flg |= 0x2000;
				pO->Size_x += 20;
				pO->Size_y -= 40;
				pO->Size_z += 20;
				pO->Poly_rgb -= 0x10001;
			}while (--i);

			if (!(pP_ptr->Timer0 & 1)) {
				Sv.vx = 0x80 - Rnd();
				Sv.vy = 0x80 - Rnd();
				Sv.vz = 0x80 - Rnd();
				Esp_call(((Rnd() << 2) + 0x448) | 0x80000, 0, &pP_ptr[ (Rnd() & 1) + 1 ].Workm, &Sv);
			}

			if (pP_ptr->Size_y < 40)
				PF->_R0 = 4;

			break;

		case 4:
			i = 4;
			do {
				pO = &pP_ptr[ i ] - 1;
				Late_set(&pO->Attribute, 1);
				pO->Attribute |= 2;
				pO->Poly_rgb -= 0x2000;
			}while (--i);

			PF->_R0 = 5;

		case 5:
			if (pP_ptr->Poly_rgb & 0xFF) {
				i = 4;
				do {

					pO = &pP_ptr[ i ] - 1; pO->Poly_rgb -= 0x10001;
				}while (--i);
			}
			if (pP_ptr->Poly_rgb & 0xFF00) {
				i = 4;
				do {

					pO = &pP_ptr[ i ] - 1; pO->Poly_rgb -= 0x100;
				}while (--i);
				break;
			}

			PF->_R0 = 6; break;

		case 6:
			i = 4;
			do {
				pO = &pP_ptr[ i ] - 1;
				pO->Attribute &= ~2;
				pO->Be_flg &= ~0x2000;
				pO->Be_flg |= 0x400;
				pO->Be_flg &= ~0x40;
				pO->Be_flg &= ~0x200;
				if (i != 1)
					pO->Be_flg |= 0x800;

				pO->Poly_rgb = 0x508050;
				pO->Size_x = 0x1000;
				pO->Size_y = 0;
				pO->Size_z = 0x1000;
			}while (--i);
			PF->_R0 = 7;

			pP_ptr->Timer0 = 0;

		case 7:

			if (pP_ptr->Timer0 == 255) {
				PF->_R0 = 8;
				pP_ptr->Timer2 = 4;

				pP_ptr->Timer0 = 0;

				pP_ptr->Timer1 = 1;

				pP_ptr->Timer3 = 0;
			}

			break;

		case 8:
			pP_ptr->Timer0++;
			if (!(pP_ptr->Timer0 & 3)) {

				Sv.vx = 0x80 - Rnd();
				Sv.vy = 0x80 - Rnd();
				Sv.vz = 0x80 - Rnd();
				Esp_call(((Rnd() << 3) + 0xC18) | 0x80000, pEm->Cdir_y - 0x400, &pP_ptr->Workm, &Sv);

				Sv.vx = 0x80 - Rnd();
				Sv.vy = 0x80 - Rnd();
				Sv.vz = 0x80 - Rnd();
				Esp_call(((Rnd() << 3) + 0x830) | 0x110000, pEm->Cdir_y - 0x400, &pP_ptr->Workm, &Sv);
			}

			if (pP_ptr->Timer0 > 60 && (pP_ptr->Poly_rgb & 0xFF) != 0x80) {

				pO = pP_ptr; i = 4;
				do {
					pO->Poly_rgb += 0x80008;
					pO++;
				}while (--i);
			}

			pO = &pP_ptr[ (signed char)pP_ptr->Timer2 ] - 1;
			pO->Size_y += 0xF0 + (signed char)pP_ptr->Timer1 * 120;

			pP_ptr->Timer1 *= -1;
			if (pO->Size_y > 0x1000) {
				pO->Size_y = 0x1000;
				pO->Be_flg &= ~0x400;
				pO[1].Be_flg &= ~0x800;
				pP_ptr->Timer2--;
				pEm->Timer3 = 0;
				if (!S8(pP_ptr->Timer2))
					PF->_R0 = 9;
			}

		case 9:
		}
	}
}

static void Em28_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	MATRIX M;

	EF->Dm_cnt |= 0x82;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70006;
		pEm->Routine_2 = 1;
		pEm->Neck_flg &= ~2;

		pEm->Timer0 = 0;

		pP_ptr = pEm->pSin_parts_ptr;
		pEm->Cdir_y += pP_ptr->Cdir_y;
		pP_ptr->Cdir_y = 0;

		EF->Etc_flg |= 0x1000;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {

			*(long *)&pEm->Routine_0 = 0x101;

			EF->Dm_cnt &= 0x7F;

			pP_ptr = pEm->pSin_parts_ptr + 2;
			pP_ptr[0].Be_flg &= ~0xC00;
			pP_ptr[1].Be_flg &= ~0xC00;

			EF->Etc_flg &= ~0x1000;
		}

		Add_speedXZ(pEm, 0);
	}

	if (!(pEm->Move_cnt & 0xF)) {
		Em28_me_tate_d = 3 - Rnd() % 6;
		Em28_me_yoko_d = 15 - Rnd() % 30;
	}

}

static void Em28_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register PARTS_W *pP;	/* inferred name: no retail SYM record (CSE folds it into the parts base) */
	EF->Dm_cnt |= 0x82;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x70006;
		pEm->Routine_2 = 1;
		pEm->Neck_flg &= ~2;

		pP_ptr = pEm->pSin_parts_ptr;
		pEm->Cdir_y += pP_ptr->Cdir_y;
		pP_ptr->Cdir_y = 0;
		if (pEm->Routine_1 == 17)	/* retail-only damage type 17 */
			Em28_ryudm(pEm);
		EF->Etc_flg |= 0x1000;	/* retail */

	case 1:
		if (pEm->Routine_1 == 14 && !(pEm->Move_cnt & 3))
			Em28_spdm(pEm, 0);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			*(long *)&pEm->Move_no = 0x3F000A;
			pEm->Routine_2 = 2;
			pEm->Neck_flg &= ~2;

			pP = pEm->pSin_parts_ptr + 9;
			Esp_call(((Rnd() << 2) + 0x17D0) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);

			pP = pEm->pSin_parts_ptr + 13;
			Esp_call(((Rnd() << 2) + 0x1FA0) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);

			pP = pEm->pSin_parts_ptr + 3;
			Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);

			pP = pEm->pSin_parts_ptr + 1;
			Esp_call(((Rnd() << 2) + 0x1000) | 0x40F0000, pEm->Cdir_y, &pP->Workm, 0);

			if (pEm->Routine_1 == 14)
				Em28_spdm(pEm, 1);

		}
		Add_speedXZ(pEm, 0);
		break;

	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40)) {

			*(long *)&pEm->Routine_0 = 0x101;

			EF->Dm_cnt &= 0x7F;
		}
		Add_speedXZ(pEm, 0);
	}

	if (!(pEm->Move_cnt & 3)) {
		Em28_me_tate_d = 3 - Rnd() % 6;
		Em28_me_yoko_d = 15 - Rnd() % 30;
	}

}

static void Em28_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	/* retail: a death while flagged 0x2000 restores the saved routine instead of dying */
	if (EF->Etc_flg & 0x2000) {
		pEm->Life = 0;
		*(long *)&pEm->Routine_0 = pEm->Tmp_routine;
		Em28_Move(pEm, Kan, Seq);
		return;
	}
	Em28_die00(pEm, Kan, Seq);
}

static void Em28_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, j;
	SVECTOR Sv;
	register KAGE_WORK *pKage;
	short n;	/* retail-only: spawn count (case 3) */
	ENEMY_WORK *pWk;

	short Em28_die00_hen[8] = { 31, 40, 22, -20, -30, 10, 22, 0 };

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_2) {
	case 0:

		Em_dead_flg_on(pEm);

		*(long *)&pEm->Move_no = 0x3F0007;

		if (EF->Etc_flg & 0x200) {
			pEm->Move_cnt = 50;
			Spd_get(pEm, Kan, Seq, 0);
		}

		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;

		pEm->Timer3 = 0;
		if (EF->Etc_flg & 2) {
			EF->Etc_flg &= ~2;
			pEm->Timer3 = 1;
		}

		if (pEm->Routine_1 == 9)
			Em28_bomdm(pEm);

		if (pEm->Routine_1 == 17)	/* retail-only damage type 17 */
			Em28_bom2dm(pEm);

		if (pEm->Routine_1 == 10)
			Em28_firedm(pEm);

		if (pEm->Routine_1 == 11)
			Em28_ryudm(pEm);

		if (pEm->Routine_1 == 14)
			Em28_spdm(pEm, 1);

		if ((pEm->Routine_1 == 5 || pEm->Routine_1 == 6) && !EF->Mg_rout0)	/* retail: + type 6 */
			EF->Mg_rout0 = 1;

		/* retail: the Beta's parts[0] Cdir_y fold into Cdir_y is gone */

		if (pEm->Routine_1 == 16)
			Em28_firedm(pEm);
		EF->Etc_flg |= 0x4000;	/* retail: stops the EM_MAIN neck turn */

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x40)) {	/* retail: Joint_move2 */
			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0x3F000E;

			pEm->Timer0 = 0;

			pEm->Timer2 = (Rnd() & 0x1F) + 8;

			pP_ptr = pEm->pSin_parts_ptr;
			i = 23;
			j = 0;
			do {
				if (!*(signed char *)&pEm->Timer3 || j < 9 || j > 12) {
					pP_ptr->Be_flg |= 0x2000;
					pP_ptr->Size_x = 0x1000;
					pP_ptr->Size_y = 0x1000;
					pP_ptr->Size_z = 0x1000;
				}
				j++;
				pP_ptr++;
			}while (--i);

			pP_ptr = pEm->pSin_parts_ptr + 1;
			pP_ptr->Be_flg &= ~0x2000;

			pEm->Timer3 = -1;

			Snd_se_enem(12, pEm);

		}
		Add_speedXZ(pEm, 0);

		if (!(pEm->Move_cnt & 0xF)) {
			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}
		if (pEm->Move_cnt == 49)
			Pad_vib_set1(8, 150, 5);	/* retail-only (Dual Shock) */

		break;

	case 2:
		Joint_move2(pEm, Kan, Seq, 0x40);	/* retail: Joint_move2 */

		if (pEm->Atd[0].At_w > 300) {
			pEm->Atd[0].At_w -= 6;
			pEm->Atd[0].At_d -= 6;
			pEm->Atd[0].W -= 6;
			pEm->Atd[0].D -= 6;
			pEm->Atd[1].At_w -= 3;
			pEm->Atd[1].At_d -= 3;
			pEm->Atd[1].W -= 3;
			pEm->Atd[1].D -= 3;
			pEm->Atd[2].At_w -= 3;
			pEm->Atd[2].At_d -= 3;
			pEm->Atd[2].W -= 3;
			pEm->Atd[2].D -= 3;
		}

		if (pEm->Timer0 == 60)
			*(long *)&pEm->Move_no = 0x3F000F;

		if (pEm->Timer0 == 120)
			*(long *)&pEm->Move_no = 0x3F0010;

		pP_ptr = pEm->pSin_parts_ptr;
		if (pP_ptr->Size_y > 20) {
			i = 23;
			do {
				if (pP_ptr->Size_y > 20) {
					pP_ptr->Size_y -= 7;
					pP_ptr->Size_x -= 4;
					pP_ptr->Size_z -= 4;
				}
				pP_ptr++;
			}while (--i);
		}

		pP_ptr = pEm->pSin_parts_ptr + 1;
		((short *)pP_ptr->pSa_dat_head)[8] += 10;

		if (!*(signed char *)&pEm->Timer2) {
			pEm->Timer3 = 0;
			pEm->Timer2 = (Rnd() & 0x1F) + 12;
		}else {
			pEm->Timer2--;
		}
		if ((unsigned char)pEm->Timer3 != 0xFF) {
			((short *)pP_ptr->pSa_dat_head)[8] -= Em28_die00_hen[ (signed char)pEm->Timer3 ] * 2;

			if ((signed char)++pEm->Timer3 >= 8)
				pEm->Timer3 = -1;
		}

		if ((*(long *)&pKage->Prim[0].r0 & 0xFF00) != 0x1000) {
			*(long *)&pKage->Prim[0].r0 -= 0x100;
			*(long *)&pKage->Prim[1].r0 -= 0x100;
			*(long *)&pKage->Prim[0].r0 += 0x10001;
			*(long *)&pKage->Prim[1].r0 += 0x10001;

			pKage->Half_x += 5;
			pKage->Half_z += 5;
		}

		if (((short *)pP_ptr->pSa_dat_head)[8] > 0x1000) {
			((short *)pP_ptr->pSa_dat_head)[8] = 0x1000;
			pEm->Routine_2 = 3;
		}

		if (!(pEm->Timer0 % 3)) {
			if ((pP_ptr[0].Poly_rgb & 0xFF) != 0x40)
				pP_ptr[0].Poly_rgb -= 0x10001;

			if ((pP_ptr[0].Poly_rgb & 0xFF00) != 0x6000)
				pP_ptr[0].Poly_rgb -= 0x100;

			if ((pP_ptr[1].Poly_rgb & 0xFF) != 0x40)
				pP_ptr[1].Poly_rgb -= 0x10001;

			if ((pP_ptr[1].Poly_rgb & 0xFF00) != 0x6000)
				pP_ptr[1].Poly_rgb -= 0x100;

			if ((pP_ptr[3].Poly_rgb & 0xFF) != 0x40)
				pP_ptr[3].Poly_rgb -= 0x10001;

			if ((pP_ptr[3].Poly_rgb & 0xFF00) != 0x6000)
				pP_ptr[3].Poly_rgb -= 0x100;

			Sv.vx = 0x400 - (Rnd() << 3);
			Sv.vy = -200;
			Sv.vz = 0x400 - (Rnd() << 3);
			Esp_call(((Rnd() << 3) + 0x830) | 0x90000, Rnd() << 4, &pP_ptr->Workm, &Sv);

		}
		pEm->Timer0++;
		/* inferred dead guard (exact original condition unknown): Move_cnt is unsigned char, so bit 8 is never set */
		if (pEm->Move_cnt & 0x100) {
			/* combine folds this branch after flow: retail's empty block at 2790 and the block split before 2794 */
			Rnd(); }

		if (!(pEm->Move_cnt & 7)) {
			Em28_me_tate_d = 3 - Rnd() % 6;
			Em28_me_yoko_d = 15 - Rnd() % 30;
		}

		break;

	case 3:
		pEm->Be_flg |= 2;

		pP_ptr = pEm->pSin_parts_ptr;
		i = 23;
		do {
			pP_ptr->Be_flg = 0;
			pP_ptr++;
		}while (--i);
		pP_ptr = pEm->pSin_parts_ptr + 1;
		pP_ptr->Be_flg = 1;

		/* retail-only: spawn small spiders (enemy 0x27) up to 4 enemies */
		n = 4 - G.Enemy_num;
		if (n < 0)
			n = 0;
		while ((unsigned short)n-- > 0) {	/* inferred spelling: an ordered test keeps fold from turning it into --n != -1 (retail tests the old count) */
			pWk = Set_enemy_work(0x27);
			pWk->Pos_x = pEm->Pos_x;
			pWk->Pos_y = pEm->Pos_y;
			pWk->Pos_z = pEm->Pos_z;
			pWk->Cdir_y = Rnd() << 4;
			pWk->Type = 3;
		}

		pEm->Routine_2 = 4;

		pEm->Timer0 = 500;

		pP_ptr = pEm->pSin_parts_ptr + 1;
		pEm->Timer1 = *(short *)(pP_ptr->pSa_dat_head + 4);

		Em28_me_tate_d = 0;
		Em28_me_yoko_d = 10;

	case 4:
		pP_ptr = pEm->pSin_parts_ptr + 1;
		if (!*(signed char *)&pEm->Timer2) {
			pEm->Timer3 = 0;
			pEm->Timer2 = (Rnd() & 0x3F) + 16;
		}else {
			pEm->Timer2--;
		}
		if ((unsigned char)pEm->Timer3 != 0xFF) {
			((short *)pP_ptr->pSa_dat_head)[8] = pEm->Timer1 + Em28_die00_hen[ (signed char)pEm->Timer3 ] * 3;

			if ((signed char)++pEm->Timer3 >= 8)
				pEm->Timer3 = -1;
		}

		if ((unsigned short)pEm->Timer0-- <= 0) {
			pEm->Routine_2 = 5;

			EF->Etc_flg &= ~0x100;
		}

	case 5:
	}

}

static void Em28_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em28_sce[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em28_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em28_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldamage_Em28_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em28_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = pEm->Routine_1 + 0x30000;
		pEm->Routine_2 = 1;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

		if (!pEm->Routine_1) {
			Snd_se_stad(0x4000001, &pEm->Pos_x);
		}else {
			Snd_se_stad(0x4010001, &pEm->Pos_x);
		}

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			*(long *)&pEm->Routine_0 = 1;

			pEm->Damage_cnt &= 0x7F;
			pEm->Neck_flg = 0;

			G.Status_flg &= ~0x40;
		}
		Add_speedXZ(pEm, 0);

	}
}

static void Em28_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x30601;
		pEm->Routine_2 = 1;

		pEm->Spd_x = 800;
		pEm->Spd_z = 0;

		G.Status_flg |= 0x40;
		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg = 1;

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Spd_x -= 150;
		if (pEm->Spd_x < 300)
			pEm->Spd_x = 300;

		break;

	case 2:
		*(long *)&pEm->Move_no = 0x30002;
		pEm->Routine_2 = 3;

	case 3:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		pEm->Spd_x -= 20;
		if (pEm->Spd_x < 0)
			pEm->Spd_x = 0;

		break;

	case 4:
		*(long *)&pEm->Routine_0 = 1;

		pEm->Damage_cnt &= 0x7F;
		pEm->Neck_flg = 1;

		G.Status_flg &= ~0x40;
	}

	Add_speedXZ(pEm, 0x800);

}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Pldie_Em28_tbl[ pEm->Routine_1 ](pEm, Kan, Seq); }

static void Em28_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
	unsigned char Me3[17] = { 16, 12, 48, 52, 53, 54, 55, 56, 57, 58, 59, 60, 63, 146, 147, 154, 158 };

	unsigned char Me4[13] = { 12, 1, 9, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
	register short Tmp_t, Tmp_y;
	short Dt;	/* inferred name: no retail SYM record (narrowed temporary combined away) */

	Tmp_t = Em28_me_tate_a;
	Tmp_y = Em28_me_yoko_a;
	if ((signed char)Em28_me_tate_d < (signed char)Em28_me_tate)
		Em28_me_tate_a = -1;

	if ((signed char)Em28_me_tate < (signed char)Em28_me_tate_d)
		Em28_me_tate_a = 1;

	if ((signed char)Em28_me_yoko > (signed char)Em28_me_yoko_d)
		Em28_me_yoko_a = -1;

	if ((signed char)Em28_me_yoko < (signed char)Em28_me_yoko_d)
		Em28_me_yoko_a = 1;

	if (-Em28_me_tate_a == Tmp_t)
		Em28_me_tate_mv = Em28_me_tate_mv * 2 / 3;

	if (-Em28_me_yoko_a == Tmp_y)
		Em28_me_yoko_mv = Em28_me_yoko_mv * 2 / 3;

	if ((signed char)Em28_me_tate == (signed char)Em28_me_tate_d && __builtin_abs(Em28_me_tate_mv) < 3) {
		Em28_me_tate = (signed char)Em28_me_tate_d;
		Em28_me_tate_a = 0;
		Em28_me_tate_mv = 0;
	}
	if ((signed char)Em28_me_yoko == (signed char)Em28_me_yoko_d && __builtin_abs(Em28_me_yoko_mv) < 3) {
		Em28_me_yoko = (signed char)Em28_me_yoko_d;
		Em28_me_yoko_a = 0;
		Em28_me_yoko_mv = 0;
	}

	Em28_me_tate_mv += Em28_me_tate_a;
	Em28_me_yoko_mv += Em28_me_yoko_a;

	Tmp_t = (signed char)Em28_me_tate;
	Tmp_y = (signed char)Em28_me_yoko;

	Em28_me_tate += Em28_me_tate_mv;
	Em28_me_yoko += Em28_me_yoko_mv;

	if ((signed char)Em28_me_tate > 3)
		Em28_me_tate = 3;

	if ((signed char)Em28_me_tate < -3)
		Em28_me_tate = -3;

	if ((signed char)Em28_me_yoko > 15)
		Em28_me_yoko = 15;

	if ((signed char)Em28_me_yoko < -15)
		Em28_me_yoko = -15;

	Dt = (signed char)Em28_me_tate - Tmp_t;
	Tmp_y = (signed char)Em28_me_yoko - Tmp_y;

	Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me3);

	Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me4);

}
