/* em2f.c - reconstruction of D:\Bio2\prog\EM2F.C (enemy 2F: ivy).
 * Linked into two overlays from the same source: 0x14 (at 0x80100004) and
 * 0x32 (at 0x8010d004); only the enemy-number-specific entry name differs
 * (Em12f/Em22f), selected by OVL_32.
 * Types come from the TU's retail SYM preamble (em2f.h).
 * Kage_work_set, Oba_ck_em, L_pos_ckXZ, Pl_life_down, Spl_life_down, Cdir_ck
 * and Snd_se_enem are called without a declaration (implicit int): those
 * implicit declarations are what open the record-less blocks of the retail
 * SYM (Em2f_Init, Em12f, hit_ck_main).  The other callees are declared below,
 * because retail opens no block at their calls. */
#include "EM2F.H"

/* The plant body (EM2E) that owns this ivy part: pEm->pEnemy_ptr points at its
 * ENEMY_WORK, whose Free[] area holds an EM2E_FREE. */
#define PARENT_FREE(p)	((EM2E_FREE *)ENEMY_FREE((ENEMY_WORK *)(p)->pEnemy_ptr))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of this translation unit ---- */

/* ---- functions of other translation units ---- */

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
static unsigned long D_801022A8 = 0;	/* retail-only EM2F.C word (name unknown): normal-death counter; Em2f_Init clears it (sw @0x8010026C), Em2f_Die_nml counts to 4 then sets Room_flg bit 0x14 (@0x801019B0) */
static void (*Em2f_routine_0[8])() = {
	Em2f_Init,
	Em2f_Move,
	Em2f_Damage,
	Em2f_Die,
	Em2f_Scenario,
	0,
	0,
	Em2f_Dead
};
static void (*Mv_move_tbl[10])() = {
	Em2f_move_taiki,
	Em2f_move_taiki,
	Em2f_move_taiki,
	Em2f_move_atck,
	Em2f_move_neck,
	Em2f_move_dmg,
	Em2f_move_color,
	Em2f_move_ivyfall,
	Em2f_move_gas,
	Em2f_move_hole
};
static void (*move_atck_tbl[3])() = {
	move_atck00,
	move_atck10,
	move_atck20
};
static unsigned char atck_wait_tbl[8] = { 8, 15, 8, 4, 8, 15, 8, 4 };
static void (*Em2f_dmg_tbl[6])() = {
	Em2f_dmg_down,
	Em2f_dmg_fall,
	Em2f_dmg_omake,
	Em2f_dmg_oki,
	Em2f_dmg_H2SO4,
	func_80101894	/* retail-only entry (data .word @0x8010231C) */
};
static void (*dmg_omake_tbl[2])() = {
	Em2f_dmg_omake00,
	Em2f_dmg_omake10
};
static void (*em2f_die_tbl[19])() = {
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_fire,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_fire,
	Em2f_Die_nml,
	Em2f_Die_nml,
	Em2f_Die_nml
};
static EM2F_BLOOD_TBL blood_id_tbl[6] = {
	{ 0, 17, 0, 0x1000 },
	{ 0, 17, 0, 0x800 },
	{ 4, 15, 8, 0xC00 },
	{ 6, 0, 0, 0x1000 },
	{ 5, 6, 0, 0x1000 },
	{ 5, 6, 0, 0x800 }
};
static JH_TBL Iv_jh0_tbl[10] = {
	{ 0, 0x1000 },
	{ 1, 0x1000 },
	{ 3, 0x1000 },
	{ 5, 0x1000 },
	{ 2, 0x834 },
	{ 4, 0x834 },
	{ 6, 0x834 },
	{ 0x12, 0x2BC },
	{ 0xE, 0x2BC },
	{ 0xA, 0x2BC }
};
static JH_HEAD Iv_jh0_head[1] = {
	{ 0, 1, 0xA, 0, 0, 0, 0, { 0, 0, 0, 0 }, 0, 0, 0, 0, { 0, -0x373, 0, 0 }, Iv_jh0_tbl }
};
static JH_TBL Iv_jh1_tbl[13] = {
	{ 0x11, 0xE00 },
	{ 0xD, 0xE00 },
	{ 9, 0xE00 },
	{ 0x13, 0xA00 },
	{ 0xF, 0xA00 },
	{ 0xB, 0xA00 },
	{ 0x10, 0x800 },
	{ 0xC, 0x800 },
	{ 8, 0x800 },
	{ 0x15, 0x200 },
	{ 0x14, 0x200 },
	{ 0x16, 0x200 },
	{ 7, 0 }
};
static JH_HEAD Iv_jh1_head[1] = {
	{ 1, 2, 0xD, 0, 0, 0, 0, { 0, -0x373, 0, 0 }, 0, 0, 0, 0, { 0, -0x6E6, 0, 0 }, Iv_jh1_tbl }
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	unsigned short pPm;

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7f)
			pEm->Damage_cnt--;

		if (((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck)
			((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck--;

		Em2f_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Set60A) {
			pPm = (unsigned long)pEm->pEnemy_ptr;
			if (((EM2F_FREE *)ENEMY_FREE(pEm))->Col_flg)
				if (PARENT_FREE(pEm)->Die_cnt)
					Pcolor_dec(pEm);
		}

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);

	}
}

static void Em2f_after(ENEMY_WORK *pEm)
{
	Iv_Jh_move(pEm);	}

static void Em2f_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	*(long *)&pEm->Routine_0 = 1;

	pEm->Be_flg |= 0x44a;

	pEm->Life = -1;
	pEm->Neck_flg = 1;
	pEm->Neck_no = 0;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;

	((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck = 0;
	((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = 0;
	((EM2F_FREE *)ENEMY_FREE(pEm))->On_flg = 0;
	((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = 0;
	((EM2F_FREE *)ENEMY_FREE(pEm))->Col_flg = 0;
	((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg = 0;

	for (i = 1; i < 3; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		pP_ptr->Be_flg |= 0x8000;
	}

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 500;
	pEm->Atd[0].At_d = 500;
	pEm->Atd[0].At_h = 500;
	pEm->Atd[0].W = 500;
	pEm->Atd[0].D = 500;
	pEm->Eff_at_r = 1200;

	*(long *)&pEm->Move_no = ((Rnd() & 0xf) << 8) + 0x70000;

	pEm->Type |= 0x2000;

	if (!(pEm->Type & 0xff)) {
		Kage_work_set(&pEm->pKage_work, 0, 0x320190, 0, pP_ptr->Workm.t);

		G.pFree_work = Iv_Jh_init(pEm, G.pFree_work);
		D_801022A8 = 0;	/* retail-only */
		pEm->Be_flg |= 0x80400;
		pEm->Type |= 0x4000;	/* retail-only */
		return;
	}else {

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x4000;
		pEm->pTafter_func = (void (*)())Em2f_after;

		((EM2F_FREE *)ENEMY_FREE(pEm))->Set60A = 0;
		((EM2F_FREE_R *)ENEMY_FREE(pEm))->Retail_18 = 0;	/* retail-only */

		if ((unsigned char)pEm->Type < 0x20) {
			pEm->Type |= 0x4000;	/* retail-only */
			((EM2F_FREE *)ENEMY_FREE(pEm))->Base_dir = pEm->Cdir_y = (Rnd() & 0xff) << 1;	/* retail: moved here from the common path */
			On_Ivyparts(pEm, 0);
		}else {

			pEm->Cdir_x = 0x400;
			pEm->Routine_1 = 9;
			((EM2F_FREE *)ENEMY_FREE(pEm))->Set60A = 1;
			pEm->Be_flg |= 0x18010000;
			Kage_work_set(&pEm->pKage_work, 0, 0x320190, 0, pP_ptr->Workm.t);
			if (pEm->Type == 0x2021)	/* retail-only */
				pEm->Life = -1;
			else {
				pEm->Life = 1;
				pEm->Cdir_y = 0x800;
			}

		}
	}
}

static void Em2f_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!(unsigned char)pEm->Type) {
		Em2f_Mv_dummy(pEm, Kan, Seq);
	}else {

		Em2f_Mv_move(pEm, Kan, Seq);
	}	}

static void Em2f_Mv_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em2f_Mv_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned char req;
	unsigned short pPm;

	Mv_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Set60A) {

		pPm = (unsigned long)pEm->pEnemy_ptr;
		if (PARENT_FREE(pEm)->Hand_req) {
			req = (PARENT_FREE(pEm)->Hand_req & 0x7f) - 1;
			if (pEm->Routine_1 != req) {
				if (PARENT_FREE(pEm)->Hand_req & 0x80) {
					if (PARENT_FREE(pEm)->Req_att == ((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no)
						Set_req(pEm, req);

				}else

				if (((EM2F_FREE *)ENEMY_FREE(pEm))->Att & PARENT_FREE(pEm)->Req_att)
					Set_req(pEm, req);
			}
		}
	}

}

static void Em2f_move_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = ((EM2F_FREE *)ENEMY_FREE(pEm))->Base_dir;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = 0;
		*(long *)&pEm->Move_no = (7 << 16) + ((((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no << (pEm->Routine_1 + 2)) << 8) + pEm->Routine_1;
	}

	Joint_move(pEm, Kan, Seq, 0x200);
	On_Ivyparts(pEm, 0);	}

static void Em2f_move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pIv;
	register PLAYER_WORK *pTg;

	move_atck_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	pIv = (ENEMY_WORK *)pEm->pEnemy_ptr;

	pTg = (((EM2E_FREE *)ENEMY_FREE(pIv))->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (pTg->Damage_cnt & 0x80)
		((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = 1;

	On_Ivyparts(pEm, 0);
}

static void move_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Routine_2++;

	pEm->Timer0 = ((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no * 2 + ((Rnd() & 3) + 1);

	Joint_move(pEm, Kan, Seq, 0x200);	}

static void move_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pIv;
	register PLAYER_WORK *pTg;

	if (!--pEm->Timer0) {
		pEm->Routine_2++;

		pIv = (ENEMY_WORK *)pEm->pEnemy_ptr;

		((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = 0;
		*(long *)&pEm->Move_no = 0x70004;

		pTg = &G.Player;	if (((EM2E_FREE *)ENEMY_FREE(pIv))->Target_pl) pTg = (PLAYER_WORK *)G.pEnemy[1];

		pEm->Timer1 = pIv->Cdir_y;
		if (((EM2F_FREE *)ENEMY_FREE(pEm))->Att == 3) {
			pEm->Timer1 += 0x400 + ((Rnd() & 1) << 11);
		}else

			pEm->Timer1 += (((EM2F_FREE *)ENEMY_FREE(pEm))->Att & 2) ? 0x400 : -0x400;

		pEm->Timer1 += Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
	}

	Joint_move(pEm, Kan, Seq, 0x200);
}

static void move_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pIv;
	register unsigned long chr_type;
	register unsigned long eos;

	pIv = (ENEMY_WORK *)pEm->pEnemy_ptr;

	((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = pEm->Timer1 - pIv->Cdir_y;
	eos = Joint_move(pEm, Kan, Seq, 0x200);
	Em_seq_sound(pEm);
	chr_type = *pEm->pNow_seq;

	if (chr_type & 0x20000)
		if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg)
			((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = Atck_hit_ck(pEm, 5);

	if (!(chr_type & 0x10000)) {
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += (((EM2F_FREE *)ENEMY_FREE(pEm))->Att & 1) ? -20 : 20;
	}else {

		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += (((EM2F_FREE *)ENEMY_FREE(pEm))->Att & 1) ? 20 : -20;
	}
	if (eos)
		move_atck20_0(pEm);

}

static void move_atck20_0(ENEMY_WORK *pEm)
{
	if (pEm->Routine_0 == 1) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Act_type &= 0x7f;

	}	}

static void move_atck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em2f_move_neck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pIv;
	register EM2E_FREE *pIvf;
	register PLAYER_WORK *pTg;

	pIv = (ENEMY_WORK *)pEm->pEnemy_ptr;
	pIvf = (EM2E_FREE *)ENEMY_FREE(pIv);

	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;

		*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 7) << 8) + ((Rnd() & 1) + 5);

		pTg = (pIvf->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
		pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);

	case 1:
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = pEm->Timer0 - pIv->Cdir_y;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz -= (Rnd() & 0x1f) | 0x100;
		if (((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz < -0x3ff)
			pEm->Routine_2 = 2;

		break;

	case 2:
		if (pIvf->Req_att) {
			pEm->Routine_2 = 3;
			*(long *)&pEm->Move_no = (((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no << 11) + 0x70001;
		}
		break;

	case 3:
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = pEm->Timer0 - pIv->Cdir_y;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += (Rnd() & 0x1f) | 0x100;
		Joint_move(pEm, Kan, Seq, 0x200);
		if (((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz >= 0)
			pEm->Routine_2 = 4;

	case 4:
		break;

	}

	Joint_move(pEm, Kan, Seq, 0x200);
	On_Ivyparts(pEm, 0);
}

static void Em2f_move_dmg(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 7) << 8) + 5;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = ((EM2F_FREE *)ENEMY_FREE(pEm))->Base_dir;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = 0;
	}

	if (Joint_move(pEm, Kan, Seq, 0x200)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
	}
	On_Ivyparts(pEm, 0);	}

static void Em2f_move_color(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);	/* [INFERRED] block-scope declaration of the function called here: opens the record-less SYM block */

	((EM2F_FREE *)ENEMY_FREE(pEm))->Col_flg = 1;

	Joint_move(pEm, Kan, Seq, 0x200);

	((EM2F_FREE *)ENEMY_FREE(pEm))->Act_type &= 0x7f;

	if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg)

		pEm->Routine_1 = 2;
	else
		pEm->Routine_1 = 8;

	*(unsigned short *)&pEm->Routine_2 = 0;

}

static void Em2f_move_ivyfall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);	/* [INFERRED] block-scope declaration of the function called here: opens the record-less SYM block */

	Joint_move(pEm, Kan, Seq, 0x200);

	((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = 0x400;
	On_Ivyparts(pEm, 0);
}

static void Em2f_move_gas(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;

		((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg = 1;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = 0x400;
		*(long *)&pEm->Move_no = (Rnd() & 7) << 8;

		Joint_move(pEm, Kan, Seq, 0x200);

		if (((EM2F_FREE *)ENEMY_FREE(pEm))->Act_type == 1 && !((EM2F_FREE *)ENEMY_FREE(pEm))->Col_flg) {
			*(long *)&pEm->Routine_0 = 0x202;
			((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck = 0;
			pEm->Timer2 = 1;
		}
	}

	On_Ivyparts(pEm, 0);	}

static void Em2f_move_hole(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em2f_move_hole00();
	}else {

		Em2f_move_hole10(pEm);
	}

	((EM2F_FREE *)ENEMY_FREE(pEm))->Rno2_stk = pEm->Routine_2;
	((EM2F_FREE *)ENEMY_FREE(pEm))->Rno3_stk = pEm->Routine_3;

	if (pEm->Sce_free0 == 1 || G.Room_enemy_flg & 0x10) {
		pEm->Life = -1;	/* retail-only */
		*(long *)&pEm->Routine_0 = 0xa03;
	}	}

static void Em2f_move_hole00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 0xf) << 8) + ((Rnd() & 1) + 1);
	}

	if (Joint_move(pEm, Kan, Seq, 0x200))
		*(long *)&pEm->Move_no = (Rnd() & 1) + 0x70001;

	if (pEm->Type != 0x2021 && !((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck && pEm->L_pl < 2000) {	/* retail-only Type test */
		*(unsigned short *)&pEm->Routine_2 = 1;
	}
}

static void Em2f_move_hole10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);	/* [INFERRED] block-scope declaration of the function called here: opens the record-less SYM block */
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0x70004;
		pEm->Cdir_y = 0x400;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = 0;
	}

	if (Joint_move(pEm, Kan, Seq, 0x200)) {
		*(unsigned short *)&pEm->Routine_2 = 0;
		((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck = (Rnd() & 0xf) + 30;
		pEm->Cdir_y = 0x800;
	}
	Em_seq_sound(pEm);

	if (*pEm->pNow_seq & 0x20000)
		if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg)
			((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = Atck_hit_ck(pEm, 1);

}

static void Em2f_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em2f_dmg_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em2f_dmg_down(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pIv;
	unsigned short pPp;	/* reconstruction device, inferred name: no retail SYM record; its dead narrowing store keeps the line-725 load */

	switch (pEm->Routine_2) {
	case 0:

		pEm->Routine_2 = 1;
		pEm->Timer0 = (Rnd() & 1) ? 24 : -24;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = ((EM2F_FREE *)ENEMY_FREE(pEm))->Base_dir;
		*(long *)&pEm->Move_no = (0xf << 16) + ((((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no << 3) << 8) + 1;

	case 1:

		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir += 24;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += pEm->Timer0;
		if (abs(((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz) >= 0x400) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = (pEm->Timer0 > 0) ? 8 : -8;
		}

		Joint_move(pEm, Kan, Seq, 0x100);	break;

	case 2:
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir -= 8;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz -= pEm->Timer0;
		pPp = (unsigned long)pEm->pSin_parts_ptr;
		pIv = (ENEMY_WORK *)pEm->pEnemy_ptr;
		if (pEm->pSin_parts_ptr[2].Workm.t[1] >= pIv->Ground) {
			pEm->Routine_2 = 3;
			pEm->Timer0 = (Rnd() & 7) + 15;
		}
		Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 3:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			pEm->Routine_2 = 4;
			*(long *)&pEm->Move_no = (0xf << 16) + (((EM2F_FREE *)ENEMY_FREE(pEm))->Hand_no << 8) + 3;
		}
		break;

	case 4:
		if (Joint_move(pEm, Kan, Seq, 0x100)) {
			if (((EM2F_FREE *)ENEMY_FREE(pEm))->Act_type == 1 && !((EM2F_FREE *)ENEMY_FREE(pEm))->Col_flg) {

				pEm->Routine_1 = 2;
				*(unsigned short *)&pEm->Routine_2 = 0;
				((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck = 0;
				((EM2F_FREE *)ENEMY_FREE(pEm))->Em_keep = 2;
				break;
			}
			pEm->Routine_2 = 5;
		}
		break;

	case 5:
		break;
	}

	On_Ivyparts(pEm, 0);
}

static void Em2f_dmg_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:

		pEm->Routine_2 = 1;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir = ((EM2F_FREE *)ENEMY_FREE(pEm))->Base_dir + 0x400;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = (Rnd() & 1) ? 0x400 : -0x400;
		pEm->Spd_y = 40;
		*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 7) << 8) + 5;
		Set_blood(pEm, 0, 1);

	case 1:
		pEm->Pos_y += pEm->Spd_y;
		pEm->Spd_y += 40;
		if (pEm->Pos_y >= pEm->Ground) {
			pEm->Routine_2 = 2;
			pEm->Pos_y = pEm->Ground;
			*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 0x1f) << 8) + 2;
		}
		break;
	}

	Joint_move(pEm, Kan, Seq, 0x200);	}

static void Em2f_dmg_omake(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pIvf;	/* inferred name (sibling Em2f_dmg_omake00); retail records none: combine folds its +0x214 into the single use */

	pIvf = PARENT_FREE(pEm);

	if (pEm->Routine_2 < 2)
		dmg_omake_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	if (((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg) {
		if (pIvf->Hand_req == 7) {
			Em2f_move_color(pEm, Kan, Seq);
			pEm->Routine_0 = 1;

			return;
		}

		if (pIvf->Hand_req == 0x45) {
			Em2f_move_color(pEm, Kan, Seq);
			pEm->Routine_1 = 4;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}
	}

	On_Ivyparts(pEm, 0);
}

static void Em2f_dmg_omake00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pIvf;

	pIvf = PARENT_FREE(pEm);

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Timer0 = (Rnd() & 0x1f) + 30;

	case 1:
		if (!--pEm->Timer0) {
			pEm->Routine_3 = 2;
			*(long *)&pEm->Move_no = 0x73c03;
		}
		break;

	case 2:
		if (Joint_move(pEm, Kan, Seq, 0x200))
			pEm->Routine_3 = 0;
		break;
	}

	if ((pIvf->Target_pl ? (pEm->L_spl & 0x3fffffff) < 1000 : pEm->L_pl < 1000) && !((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck)
		*(unsigned short *)&pEm->Routine_2 = 1;

}

static void Em2f_dmg_omake10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register unsigned long chr_type;

	switch (pEm->Routine_3) {

	short Dir;	case 0:	/* Dir: inferred name; the retail SYM has no row for it, but its declaration is what opens the record-less switch-body block */
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0x70004;
		pEm->Timer0 = ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir;
		pEm->Timer1 = ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz;
		pEm->Timer2 = 0;
		pEm->Timer3 = 0;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz &= 0xfff;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = 0;

	case 1:

		Dir = 0x800 - ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz;
		if (abs(Dir) > 0x100) {
			((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += (Dir > 0) ? 0x100 : -0x100;
			pEm->Timer2++;
		}
		Joint_move(pEm, Kan, Seq, 0x200);
		Em_seq_sound(pEm);

		chr_type = *pEm->pNow_seq;
		if (chr_type & 0x20000)
			if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg)
				((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg = Atck_hit_ck(pEm, 5);

		if (chr_type & 0x10000)

			pEm->Routine_3 = 2;

		break;

	case 2:

		if (!*(signed char *)&pEm->Timer3)
			if (Joint_move(pEm, Kan, Seq, 0x200))
				pEm->Timer3 = 1;

		if (*(signed char *)&pEm->Timer2) {
			((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz += (pEm->Timer1 > 0) ? -0x100 : 0x100;
			pEm->Timer2--;
			break;
		}
		pEm->Routine_3 = 3;
		((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz = pEm->Timer1;
		*(long *)&pEm->Move_no = 0x73c03;

		break;

	case 3:

		Joint_move(pEm, Kan, Seq, 0x200);
		if (!pEm->Hokan_flg) {
			((EM2F_FREE *)ENEMY_FREE(pEm))->Em_keep = ((EM2F_FREE *)ENEMY_FREE(pEm))->Hit_flg ? 0 : ((EM2F_FREE *)ENEMY_FREE(pEm))->Em_keep - 1;
			if (((EM2F_FREE *)ENEMY_FREE(pEm))->Em_keep) {
				*(unsigned short *)&pEm->Routine_2 = 0;
				((EM2F_FREE *)ENEMY_FREE(pEm))->No_atck = 60;
			}else

				*(unsigned short *)&pEm->Routine_2 = 2;

		}
		break;
	}
}

static void Em2f_dmg_oki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register EM2F_FREE *pFp;

	pFp = (EM2F_FREE *)ENEMY_FREE(pEm);

	switch (pEm->Routine_2) {
	case 0:

		pEm->Routine_2 = 1;
		pFp->Set_dirz = 0x400;
		*(long *)&pEm->Move_no = (0xf << 16) + ((pFp->Hand_no << 3) << 8);

	case 1:

		pFp->Set_dirz -= 32;
		if (pFp->Set_dirz < 0) {
			pFp->Set_dirz = 0;
			*(long *)&pEm->Routine_0 = 1;
		}
		break;
	}

	Joint_move(pEm, Kan, Seq, 0x100);
	On_Ivyparts(pEm, 0);
}

static void Em2f_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	switch (pEm->Routine_2) {
	case 0:

		pEm->Routine_2 = 1;
		pEm->Be_flg |= 0x800;
		pEm->Size = 0x1000;
		for (i = 0; i < 3; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[ i ];
			pP_ptr->Poly_rgb = 0x3f2f5f;
		}
		Set_blood(pEm, 0, 2);
		if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg)
			*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 7) << 8) + 5;

	case 1:

		pEm->Size -= 32;
		if (pEm->Size < 0xa00) {
			pEm->Routine_2 = 2;
			pEm->Size = 0xa00;

			for (i = 0; i < 3; i++) {
				pP_ptr = &pEm->pSin_parts_ptr[ i ];
				pP_ptr->Be_flg |= 0x10;
				pP_ptr->Free[0] = 0;
				pP_ptr->Free[1] = 0;
				pP_ptr->Free[3] = 30;
				((unsigned char *)pP_ptr->Free)[4] = 20;
				((unsigned char *)pP_ptr->Free)[5] = 20;
			}
		}
		if (!((EM2F_FREE *)ENEMY_FREE(pEm))->Sick_flg)
			Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 2:
		break;
	}

	On_Ivyparts(pEm, 0);

}

/* retail-only (retail 0x80101894 / 0x8010E894, 52 words; no Beta twin, name unknown): Em2f_dmg_tbl[5].
   Sets Retail_18 (Iv_Jh_move then skips its joint work) and restyles the three ivy parts. */
static void func_80101894(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		((EM2F_FREE_R *)ENEMY_FREE(pEm))->Retail_18 = 1;
		for (i = 0; i < 3; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[ i ];
			pP_ptr->Be_flg |= 0x4A;
			pP_ptr->Be_flg &= 0xFFFF7FFF;
			pP_ptr->Free[0] = pEm->Cdir_y;
			pP_ptr->Free[2] = 100;
			pP_ptr->Free[1] = -200;
			pP_ptr->Free[3] = 5;
			pP_ptr->Free[4] = 0;
			pP_ptr->Free[6] = -30;
			pP_ptr->Poly_rgb = 0x104010;
			Set_blood(pEm, i, 0);
		}
	}
}

static void Em2f_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (((EM2F_FREE *)ENEMY_FREE(pEm))->Set60A) {
		em2f_die_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
	}	}

static void Em2f_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (D_801022A8 < 4)	/* retail-only */
		D_801022A8++;
	else
		Flg_on(G.SaveData.Room_flg, 0x14);
	pEm->Life = 1;
	*(long *)&pEm->Routine_0 = (((EM2F_FREE *)ENEMY_FREE(pEm))->Rno3_stk << 24) + ((((EM2F_FREE *)ENEMY_FREE(pEm))->Rno2_stk << 16) + 0x901);
	Em2f_move_hole(pEm, Kan, Seq);	}

static void Em2f_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long i;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Be_flg |= 0x800;
		pEm->Size = 0x1000;
		*(long *)&pEm->Move_no = (7 << 16) + ((Rnd() & 7) << 8) + ((Rnd() & 1) + 5);
		pEm->Timer0 = 1;
		pEm->Cdir_y = 0x800;
		Em_dead_flg_on(pEm);
		if (!(G.Room_enemy_flg & 0x10))	/* retail: one-shot sound, flag 0x10 replaces Beta's 0x80 */
			Snd_se_enem(8, pEm);
		G.Room_enemy_flg |= 0x10;
	}

	Joint_move(pEm, Kan, Seq, 0x100);

	if (!--pEm->Timer0) {
		pEm->Be_flg &= ~0x800;
		for (i = 0; i < 3; i++)
			Set_blood(pEm, i, (pEm->Size > 0x800) ? 4 : 5);

		pEm->Timer0 = 12;
		pEm->Be_flg |= 0x800;
	}

	Pcolor_dec(pEm);

	pEm->Size -= 32;
	if (pEm->Size < 0) {
		pEm->Size = 0;
		*(long *)&pEm->Routine_0 = 7;
		pEm->Type |= 0x4000;	/* retail-only */
	}

}

static void Em2f_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em2f_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Type |= 0x4400;	}

static PARTS_W *Get_parts_pos(ENEMY_WORK *pTg, SVECTOR *pSv, unsigned long no)
{
	register PARTS_W *pP_ptr;

	pP_ptr = &pTg->pSin_parts_ptr[ no ];

	pSv->vx = pP_ptr->Workm.t[0];
	pSv->vy = pP_ptr->Workm.t[1];
	pSv->vz = pP_ptr->Workm.t[2];

	return pP_ptr;
}

static void On_Ivyparts(ENEMY_WORK *pEm, unsigned long mode)
{
	register PARTS_W *pP_ptr;
	register ENEMY_WORK *pTg;
	SVECTOR sv, sv2;
	MATRIX M;

	pTg = (ENEMY_WORK *)pEm->pEnemy_ptr;

	pP_ptr = Get_parts_pos(pTg, &sv, (unsigned char)pEm->Type - 1);
	pEm->Pos_x = sv.vx;
	pEm->Pos_y = sv.vy;
	pEm->Pos_z = sv.vz;

	sv.vx = pTg->Cdir_x + pP_ptr->Cdir_x;
	sv.vy = pTg->Cdir_y + pP_ptr->Cdir_y;
	sv.vz = pTg->Cdir_z + pP_ptr->Cdir_z;
	RotMatrix(&sv, &M);

	sv2.vx = ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_fx;
	sv2.vy = ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_fy;
	sv2.vz = ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_fz;
	ApplyMatrixSV(&M, &sv2, &sv);

	pEm->Cdir_x = pTg->Cdir_x;
	pEm->Cdir_y = pTg->Cdir_y + ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dir;
	pEm->Cdir_z = pTg->Cdir_z + ((EM2F_FREE *)ENEMY_FREE(pEm))->Set_dirz;
	if (mode) {
		pEm->Cdir_x += pP_ptr->Cdir_x;
		pEm->Cdir_y += pP_ptr->Cdir_y;
		pEm->Cdir_z += pP_ptr->Cdir_z;
	}

}

static long hit_ck_main(ENEMY_WORK *pEm, long pl, short dmg)
{
	register PARTS_W *pPt;
	register PLAYER_WORK *pTg;
	register long ck, i;

	pTg = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	for (i = 1; i < 3; i++) {
		pPt = &pEm->pSin_parts_ptr[ i ];
		if (pTg->Life >= 0) {
			if (L_pos_ckXZ(&pTg->Pos_x, pPt->Workm.t, 700))

				if (pEm->nFloor == pTg->nFloor) {
					if (pTg->Damage_cnt & 0x80)
						return -1;

					if (!pl) {
						ck = Pl_life_down(dmg, 1);
					}else {

						ck = Spl_life_down(pEm, dmg, 1);
					}

					switch (ck) {
					case 1:
						G.Room_enemy_flg |= 0x20;

					case 0:
						Em2f_Pl_to_dmg(pTg, Cdir_ck(pEm, pTg));
						break;

					case 2:
						Em2f_Pl_to_die(pTg);
						break;
					}
					Snd_se_enem(0, pEm);
					return 1;
				}

		}
	}

	return 0;

}

static long Atck_hit_ck(ENEMY_WORK *pEm, short dmg)
{
	register long rep;

	if (rep = hit_ck_main(pEm, 0, dmg))
		return rep;

	if (G.Status_flg & 0x10000000)
		if (rep = hit_ck_main(pEm, 1, dmg))
			return rep;

	return 0;
}

static void Em2f_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir)
{
	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
	Pad_vib_set0(3, 0);	/* retail-only Dual Shock vibration */
	Pad_vib_set1(4, 200, 3);	}

static void Em2f_Pl_to_die(PLAYER_WORK *pPl)
{
	pPl->Routine_0 = 3;
	pPl->Routine_1 = 0;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;	}

static void Set_req(ENEMY_WORK *pEm, unsigned char req)
{
	if (!(req & 0x40)) {
		pEm->Routine_1 = req;
		*(unsigned short *)&pEm->Routine_2 = 0;
	}else {

		*(long *)&pEm->Routine_0 = ((req & 0xf) << 8) + 2;
	}	}

static void Pcolor_dec(ENEMY_WORK *pEm)
{
	register unsigned long p0, p1, p2, i;
	register PARTS_W *pP_ptr;

	for (i = 0; i < 3; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		p0 = pP_ptr->Poly_rgb & 0xff;
		if (p0 & 0xff)
			p0--;
		p1 = pP_ptr->Poly_rgb & 0xff00;

		if (p1 & 0xff00)
			p1 -= 0x100;
		p2 = pP_ptr->Poly_rgb & 0xff0000;

		if (p2 & 0xff0000)
			p2 -= 0x10000;

		pP_ptr->Poly_rgb = p2 | p1 | p0;
	}

}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register long dir;
	register EM2F_BLOOD_TBL *tbl;

	tbl = &blood_id_tbl[ type ];

	dir = pEm->Cdir_y;

	pP_ptr = &pEm->pSin_parts_ptr[ parts ];

	Sv.vx = 0;
	Sv.vy = 0;
	Sv.vz = 0;
	Esp_call((tbl->Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
}

static unsigned long Iv_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top)
{

	Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, Iv_jh0_head, Mem_top);
	return Jh_set(pEm, pEm->pSin_parts_ptr, Iv_jh1_head, Mem_top);	}

static void Iv_Jh_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	if (((EM2F_FREE_R *)ENEMY_FREE(pEm))->Retail_18) return;	/* retail-only */
	pP_ptr = pEm->pSin_parts_ptr;

	MulJh(pEm, pP_ptr, Iv_jh0_head);
	MulJh(pEm, pP_ptr, Iv_jh1_head);

	Aida_Jh_sort(pEm);

}

