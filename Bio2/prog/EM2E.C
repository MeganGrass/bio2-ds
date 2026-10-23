/* em2e.c - reconstruction of D:\Bio2\prog\EM2E.C (enemy 2e: Ivy, the plant enemy).
 * Linked into two overlays from the same source: 0x13 (at 0x80100000) and 0x31
 * (at 0x8010D000); only the enemy-number-specific entry names differ
 * (Em12e/Em22e, Pldamage_Em12e/22e, Pldie_Em12e/22e), selected by OVL_31.
 * Types come from the TU's retail SYM preamble (em2e.h). */
#include "EM2E.H"

/* the module's private work in ENEMY_WORK.Free[] (macro spelling inferred; retail SYM has no local for it) */
/* retail's Free[] starts 4 bytes after the project's ENEMY_WORK.Free (pEm+0x218 against +0x214) */
#define EF(p)	((EM2E_FREE *)((UCHAR *)(p)->Free + 4))

/* ---- objects defined in other translation units ---- */

/* ---- functions of this translation unit ---- */

/* ---- functions of other translation units ---- */
/* Declared (not implicit): Em2e_Mv_fall20's switch has no SYM binding block,
   which an implicit Joint_move there would create.  Signature from JOINT.C. */
/* Declared (not implicit): Set_flower's if-body has no SYM binding block,
   which an implicit RotMatrix there would create.  PsyQ libgte signature. */
extern MATRIX GsIDMATRIX;	/* PsyQ libgs */

/* ---- data ---- */

static unsigned short Em23_life = 0x12C;
static unsigned short Em23_life_easy = 0x12C;
static unsigned short Em2a_life[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life = 0xFA;	/* retail: 0x12C -> 0xFA (data 0x801057A0) */
static unsigned short Em2b_life_easy = 0xC8;	/* retail: 0x12C -> 0xC8 */
/* Retail life tables (em2e_cdemd0_s1 .data 0x80105774..0x80105853): Em2b and Em2e values changed
 * relative to Beta 2; a second complete set (0x801057E4..) is selected by G.System_flg & 0x40
 * (Em2e_Init @0x80100158). Second-set names follow src-retail/prog/em20.c ([INFERRED]). */
static unsigned short Em2e_life[16] = { 0x61, 0x77, 0x61, 0x5A, 0x61, 0x5A, 0x77, 0x61, 0x5A, 0x5A, 0x59, 0x77, 0x45, 0x5A, 0x59, 0x77 };
static unsigned short Em2e_life_easy[16] = { 0x59, 0x45, 0x59, 0x4F, 0x63, 0x45, 0x4F, 0x3B, 0x45, 0x3B, 0x3B, 0x45, 0x3B, 0x3B, 0x3B, 0x45 };
static unsigned short Em23_life_flg40 = 0xF0;
static unsigned short Em23_life_easy_flg40 = 0xF0;
static unsigned short Em2a_life_flg40[10] = { 0xFA, 0x1C2, 0x12C, 0x12C, 0xDC, 0x12C, 0x190, 0x12C, 0x190, 0x15E };
static unsigned short Em2a_life_easy_flg40[10] = { 0xB4, 0x154, 0xDC, 0xF0, 0xAF, 0xF0, 0x136, 0xF0, 0x136, 0x12C };
static unsigned short Em2b_life_flg40 = 0xFA;
static unsigned short Em2b_life_easy_flg40 = 0xC8;
static unsigned short Em2e_life_flg40[16] = { 0x6B, 0x64, 0x6B, 0x81, 0x6B, 0x64, 0x81, 0x64, 0x81, 0x64, 0x63, 0x81, 0x4F, 0x64, 0x63, 0x81 };
static unsigned short Em2e_life_easy_flg40[16] = { 0x63, 0x4F, 0x63, 0x59, 0x6D, 0x4F, 0x59, 0x6D, 0x6D, 0x45, 0x6D, 0x4F, 0x45, 0x45, 0x45, 0x4F };
static EM2E_HITSIZE Arm_hit_size[4] = {
	{ 0x15, 0x3E8 },
	{ 0x17, 0x320 },
	{ 0xF, 0x3E8 },
	{ 0x11, 0x320 }
};
static EM2E_HAND Hand_tbl[8] = {
	{ 14, 2, 0, 0xC8, 0 },
	{ 15, 2, 0, 0, 0 },
	{ 20, 1, 0, 0x12C, 0 },
	{ 21, 1, 0, 0, 0 },
	{ 1, 2, 0, 0, 0 },
	{ 5, 1, 0, 0, 0 },
	{ 9, 2, 0, -0xA, -0x64 },
	{ 10, 1, 0, -0x14, 0x96 }
};
static unsigned char Hand_set_tbl[16] = { 2, 3, 5, 6, 0, 1, 4, 7, 0, 3, 6, 7, 1, 2, 6, 7 };
static unsigned char fire_rnd_tbl[16] = { 0, 1, 9, 14, 5, 10, 20, 17, 2, 21, 10, 0, 7, 15, 3, 9 };
static void (*Em2e_routine_0[8])() = {
	Em2e_Init,
	Em2e_Move,
	Em2e_Damage,
	Em2e_Die,
	Em2e_Scenario,
	0,
	0,
	Em2e_Dead
};
static unsigned char Type_rno_tbl[8] = { 0, 1, 0, 1, 8, 8, 9, 11 };	/* retail last entry 11 (image byte at .data+0x177; Beta 2: 0) */
static void (*em2e_move_tbl[12])() = {	/* retail: 12th entry func_801030DC (data 0x80105918) */
	Em2e_Mv_taiki,
	Em2e_Mv_walk,
	Em2e_Mv_reverse,
	Em2e_Mv_turn,
	Em2e_Mv_struck,
	Em2e_Mv_poison,
	Em2e_Mv_neck,
	Em2e_Mv_open,
	Em2e_Mv_fall,
	Em2e_Mv_gas,
	Em2e_Mv_catch,
	func_801030DC
};
static unsigned char no_atck_rnd[8] = { 15, 30, 60, 45, 15, 30, 60, 45 };
static void (*Mv_struck00_tbl[3])() = {
	Em2e_Mv_struck000,
	Em2e_Mv_struck001,
	Em2e_Mv_struck002
};
static void (*mv_poison_tbl[4])() = {
	Em2e_Mv_poison00,
	Em2e_Mv_poison10,
	Em2e_Mv_poison20,
	Em2e_Mv_poison30
};
static char poison_time_tbl[8] = { 30, 15, 0, 15, 15, 0, 15, 30 };
static void (*mv_neck_tbl[3])() = {
	Em2e_Mv_neck00,
	Em2e_Mv_neck10,
	Em2e_Mv_neck20
};
static void (*mv_fall_tbl[3])() = {
	Em2e_Mv_fall00,
	Em2e_Mv_fall10,
	Em2e_Mv_fall20
};
static void (*move_catch_tbl[3])() = {
	Em2e_Mv_catch00,
	Em2e_Mv_catch10,
	Em2e_Mv_catch20
};
static void (*mv_0b_tbl[2])() = {	/* retail-only table 0x8010596C (name inferred), Routine_1 11 */
	func_80103118,
	func_8010319C
};
static void (*em2e_dmg_tbl[19])() = {	/* retail image words (em2e slot1 .data+0x200), resolved through em2e_cdemd0_s1.symbols.txt */
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_combo,
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_gnml,
	Em2e_dmg_fire,
	Em2e_dmg_H2SO4,
	Em2e_dmg_nml,
	Em2e_dmg_nml,
	Em2e_dmg_spark,
	Em2e_dmg_combo,
	Em2e_dmg_fire2,
	Em2e_dmg_ftl,
	Em2e_dmg_combo,
	Em2e_dmg_ftl
};
static unsigned char combo_mark_rnd[8] = { 11, 10, 14, 20, 0, 1, 5, 21 };	/* retail: last entry 10 -> 21 (data 0x801059C7) */
static void (*em2e_die_tbl[19])() = {
	/* retail table 0x801059C8: entries 5-6 nml -> ftl, entries 17/19 gnml -> func_801042AC */
	Em2e_Die_nml,
	Em2e_Die_nml,
	Em2e_Die_nml,
	Em2e_Die_nml,
	Em2e_Die_ftl,
	Em2e_Die_ftl,
	Em2e_Die_ftl,
	Em2e_Die_ftl,
	Em2e_Die_gnml,
	Em2e_Die_fire,
	Em2e_Die_H2SO4,
	Em2e_Die_nml,
	Em2e_Die_nml,
	Em2e_Die_spark,
	Em2e_Die_nml,
	Em2e_Die_fire,
	func_801042AC,
	Em2e_Die_nml,
	func_801042AC
};
static void (*pldmg_tbl[3])() = {
	Em2e_Pl_dmg00,
	Em2e_Pl_dmg10,
	Em2e_Pl_dmg20
};
static void (*pldie_tbl[3])() = {
	Em2e_Pl_die00,
	Em2e_Pl_die10,
	Em2e_Pl_die20
};
static void (*move_flower_tbl[3])() = {
	Em2e_Mv_flower00,
	Em2e_Mv_flower10,
	Em2e_Mv_flower20
};
static EM2E_BLOOD_TBL blood_id_tbl[11] = {
	{ 0, 1, 0x10, 0x1800 },
	{ 0, 1, 0x10, 0x1000 },
	{ 133, 4, 0, 0x1000 },
	{ 133, 1, 0, 0x1000 },
	{ 0, 5, 0x18, 0x1000 },
	{ 0, 5, 0, 0x800 },
	{ 0, 5, 0x18, 0x1800 },
	{ 132, 7, 8, 0xC00 },
	{ 132, 7, 8, 0x1400 },
	{ 134, 0, 0, 0x1000 },
	{ 132, 12, 0, 0x1800 }
};

void EM_MAIN(ENEMY_WORK *pEm)
{

	if (!(G.Stop_flg & 0x20000000)) {

		if (EF(pEm)->Hand_req) {
			EF(pEm)->Hand_req = 0;
			EF(pEm)->Req_att = 0;
		}

		if (pEm->Damage_cnt & 0x7f)
			pEm->Damage_cnt--;

		if (EF(pEm)->No_fire)
			EF(pEm)->No_fire--;

		Em2e_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		if (EF(pEm)->Biri_cnt) {
			EF(pEm)->Biri_cnt--;
			Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 9);
		}

		if (EF(pEm)->Nomove_flg) {
			Oba_ck_em2e(pEm);
		}else {

			Oba_ck_em(pEm);
		}

		Sca_ck_em(pEm, 0x400);

		Get_workm(pEm);

		if (pEm->Pos_x == pEm->Old_x && pEm->Pos_z == pEm->Old_z) {
			EF(pEm)->Stay++;
		}else

			EF(pEm)->Stay = 0;

	}
}

static void Em2e_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register EM2E_FREE *pFp;
	register EM2E_HAND *handset;
	register ENEMY_WORK *pSet;
	register unsigned char i, *tbl, act;
	register short dec;	/* [INFERRED] used carrier: life reduction for Common_flg 0xa4 */

	*(long *)&pEm->Routine_0 = (Type_rno_tbl[ pEm->Type & 0xf ] << 8) + 1;

	pFp = EF(pEm);

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++)	/* retail clears all 12 words */
		((ULONG *)EF(pEm))[i] = 0;

	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	if (G.System_flg & 0x40) {
		if (G.System_flg & 0x20) {
			dec = 20;
			pEm->Life = Em2e_life_easy_flg40[ Rnd() & 0xf ];
		}else {
			dec = 50;
			pEm->Life = Em2e_life_flg40[ Rnd() & 0xf ];
		}
	}else {
		if (G.System_flg & 0x20) {
			dec = 20;
			pEm->Life = Em2e_life_easy[ Rnd() & 0xf ];
		}else {
			dec = 50;
			pEm->Life = Em2e_life[ Rnd() & 0xf ];
		}
	}
	pEm->Life += Rnd() & 3;
	do { }while (0);	/* [INFERRED] compiled-out macro; Em2e_Init: keeps the Life store ahead of the Flg_ck setup (@0x801002B4), removes 8 diffs */
	if (Flg_ck(G.SaveData.Common_flg, 0xa4))	/* retail-only reduction (@0x80100158 Flg_ck call) */
		pEm->Life -= dec;

	pEm->D_life_u = 13;
	pEm->D_life_c = 13;
	pEm->D_life_d = 13;
	pFp->Life_half = pEm->Life >> 1;

	pEm->Be_flg |= 0xc000000;

	pEm->pEnemy_ptr = (unsigned long *)&G.Player;
	pEm->Neck_flg = 0;
	pEm->Neck_no = 9;

	pP_ptr = &pEm->pSin_parts_ptr[9];
	pP_ptr->Free[0] = 0;
	pP_ptr->Free[1] = 0;
	pP_ptr->Free[2] = 12;
	pP_ptr->Free[3] = 0;
	pP_ptr->Free[4] = 0x200;
	pP_ptr->Free[5] = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 800;
	pEm->Atd[0].Ofs_y = -1500;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 700;
	pEm->Atd[0].At_d = 600;
	pEm->Atd[0].At_h = 1500;
	pEm->Atd[0].W = 800;
	pEm->Atd[0].D = 800;
	pEm->Eff_at_r = 800;

	for (i = 0; i < 26; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[i];
		pP_ptr->Be_flg |= 0x8000;
	}

	pFp->Twins_flg = Ck_em_id(pEm, &pFp->pTg_em, 0x2e, 1);
	pFp->Stick_cnt = 90;

	tbl = &Hand_set_tbl[ (Rnd() & 3) * 4 ];
	act = Rnd() & 3;

	for (i = 0; i < ((pEm->Type & 0x10) ? 3 : 4); i++) {
		handset = &Hand_tbl[ *tbl++ ];
		if ((pSet = Set_enemy_work(0x2f))) {
			pSet->Type = handset->No + 1;
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Att = handset->Att;
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Set_fx = handset->Fx;
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Set_fy = handset->Fy;
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Set_fz = handset->Fz;
			pSet->pEnemy_ptr = (unsigned long *)pEm;

			pSet->nFloor = pEm->nFloor;	/* retail-only store (sb 0x106 @0x801004BC) */
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Act_type = (act == i);
			((EM2F_FREE *)((UCHAR *)pSet->Free + 4))->Hand_no = i;
			pFp->Hand_rest |= 1 << i;
			Kage_work_set(&pSet->pKage_work, 0, 0x320190, 0, (VECTOR *)&pSet->Pos_x);

		}
	}

	pEm->Type &= 0xff0f;
	if (pEm->Id == 0x2e) {
		G.pPldamage_Em[30] = PLDAMEGE_EM;
		G.pPldie_Em[30] = PLDIE_EM;
	}else {

		G.pPldamage_Em[41] = PLDAMEGE_EM;
		G.pPldie_Em[41] = PLDIE_EM;
	}

	switch (pEm->Type) {
	case 0x6004:
	case 0x6005:
		*(long *)&pEm->Move_no = 0x5a0c;
		Joint_move(pEm, Kan, Seq, 0x40);
		pEm->Be_flg |= 0x408;
		pEm->Damage_cnt |= 0x80;

		pP_ptr = &pEm->pSin_parts_ptr[10];	/* retail: part 11 -> 10 (addiu 0x714) */
		Kage_work_set(&pEm->pKage_work, 0xfe0c, 0x3840708, 0, (VECTOR *)pP_ptr->Workm.t);

		break;

	case 6:
		pEm->Be_flg &= 0xe7ffffff;

		Kage_work_set(&pEm->pKage_work, 0xfd440000, 0x7080384, 0, (VECTOR *)&pEm->Pos_x);

		pEm->Atd[0].Ofs_x = 100;
		pEm->Atd[0].Ofs_z = -300;
		pEm->Atd[0].At_w = 1000;
		pEm->Atd[0].At_d = 500;
		break;

	default:
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, Kan, Seq, 0x40);

		pP_ptr = &pEm->pSin_parts_ptr[10];	/* retail: part 11 -> 10 */
		Kage_work_set(&pEm->pKage_work, 0xfe70, 0x3e80898, 0, (VECTOR *)pP_ptr->Workm.t);

	}

	pEm->Be_flg |= 0x800000;

}

static void Em2e_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;

	pFp = EF(pEm);

	if (pFp->No_atck)
		pFp->No_atck--;

	if (pFp->No_catch)
		pFp->No_catch--;

	if (pFp->No_poison)
		pFp->No_poison--;

	if (pFp->No_atck2[0])
		pFp->No_atck2[0]--;

	if (pFp->No_atck2[1])
		pFp->No_atck2[1]--;

	if (pFp->No_rev)
		pFp->No_rev--;

	Get_target_pl(pEm);

	em2e_move_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);
	Hand_atck_ck(pEm);
	Em2e_Move_flower(pEm);
	/* retail drops the Beta Die_cnt countdown / Pcolor_dec block here (0x801006B8 goes straight to Rot_neck_em) */

	Rot_neck_em(pEm, pEm->Cdir_y + (EF(pEm)->Rev_flg << 11));
	Set_flower(pEm);

	EF(pEm)->Rno1_stk = pEm->Routine_1;
	EF(pEm)->Rno2_stk = pEm->Routine_2;
	EF(pEm)->Rno3_stk = pEm->Routine_3;

	if (pFp->Twins_flg)
		if (pFp->pTg_em->Life < 0)
			pFp->Twins_flg = 0;

	if ((G.Room_enemy_flg & 0x20) && G.Player.Life > 0)
		G.Room_enemy_flg &= 0xffdf;

}

static void Em2e_Mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	switch (pEm->Routine_2) {
	case 0:

		if (pEm->Type == 0x6002) {
			if (!pEm->Sce_free0) return;

			pEm->Type &= 0xfff;
		}

		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0000;
		pEm->Neck_flg |= 2;
		EF(pEm)->Hand_req = 1;
		EF(pEm)->Req_att = 3;
		G.Room_enemy_flg &= 0xffbf;

		EF(pEm)->Nomove_flg = 1;

	case 1:

		if (((EF(pEm)->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl) < 10000 && pTg->Life >= 0) {
			pEm->Routine_2 = 2;
			pEm->Timer0 = 30;
			pEm->Neck_flg &= 0xfd;
			EF(pEm)->Hand_req = 3;
			EF(pEm)->Req_att = 3;
		}
		break;

	case 2:

		if (!--pEm->Timer0) {
			pEm->Routine_1 = 1;
			*(short *)&pEm->Routine_2 = 0;
			EF(pEm)->Stay = 0;
		}
	}

	Joint_move(pEm, Kan, Seq, 0x40);

	Head_open_ck(pEm);

	Fire_die_ck(pEm);

}

static void Fire_die_ck(ENEMY_WORK *pEm)
{
	if (EF(pEm)->Die_cnt == 0x8000 && G.Player.Life >= 0) {	/* retail adds the Life guard: lh G+0x3B66, bltz @0x80100A1C */
		if (!EF(pEm)->Sick_flg)

			*(long *)&pEm->Routine_0 = 0xa03;
		else
			*(long *)&pEm->Routine_0 = 7;

	}
}

static void Em2e_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;
	register EM2E_FREE *pFp;
	register short rot;	/* not in retail SYM (combined away): goto00 turn result */

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pFp = EF(pEm);

	/* retail: no distance tests (Beta dist local gone); Stick_cnt and a Timer0 phase decide instead (@0x80100A44) */
	switch (pEm->Routine_2) {

	case 0:

		if (pEm->Type == 0x6002) {
			if (!pEm->Sce_free0) return;

			pEm->Type &= 0xfff;
		}

		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0001;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);
		pEm->Neck_flg &= 0xfd;

		pFp->Hand_req = 3;
		pFp->Req_att = 3;
		G.Room_enemy_flg &= 0xffbf;
		pEm->Be_flg &= 0xfffffffb;

		pFp->Nomove_flg = 0;	EF(pEm)->Stay = 0;	break;

	case 1:
		Root_ck(pEm, &pTg->Pos_x, 0, 0);
		rot = Em2e_goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 6);
		pEm->Cdir_y += rot;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);
		Add_speedXZ(pEm, 0);
		if (!pFp->Stick_cnt)
			pEm->Routine_2 = 2;

		if (pFp->Stay > 120) {
			pEm->Routine_1 = 3;

			*(short *)&pEm->Routine_2 = 0;	}	break;

	case 2:
		pEm->Routine_2 = 3;
		pEm->Timer0 = 90;

	case 3:
		Root_ck(pEm, &pTg->Pos_x, 0, 0);
		rot = Em2e_goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 6);
		pEm->Cdir_y += rot;

		pFp->No_atck = 0;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);
		if (!--pEm->Timer0) {
			pEm->Routine_2 = 1;
			EF(pEm)->Stay = 0;
		}

	}

	Mv_walk00(pEm);
}

static void Mv_walk00(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register long dist;
	register short rot;	/* not in retail SYM (its sign-extension is combined into the test) */

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	dist = (EF(pEm)->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl;

	Em_seq_sound(pEm);

	pEm->Cdir_y += (EF(pEm)->Rev_flg << 11) + 0x800;
	rot = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x200);
	pEm->Cdir_y -= (EF(pEm)->Rev_flg << 11) + 0x800;
	if (dist < 3000 && !rot && !EF(pEm)->No_rev) {
		pEm->Routine_1 = 2;
		*(short *)&pEm->Routine_2 = 0;
	}

	if (pTg->Life < 0) {
		pEm->Routine_1 = 0;
		*(short *)&pEm->Routine_2 = 0;
	}

	Em2e_atck_ck(pEm);

	Head_open_ck(pEm);

	Fire_die_ck(pEm);
}

static void Em2e_atck_ck(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register PARTS_W *pP_ptr;
	register EM2E_FREE *pFp;
	register short neck, sa, sb;
	register long dist;
	long range;	register EM2E_FREE *pTfp;	/* pTfp: inferred; not in retail SYM (combined into its one use) */
	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	pFp = EF(pEm);
	dist = (EF(pEm)->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl;

	if (pTg->Life < 0) return;

	if (G.Room_enemy_flg & 0x80) {
		pFp->No_catch = 30;
		pFp->No_atck = 30;

		pFp->No_catch = 30;	return;
	}

	pP_ptr = &pEm->pSin_parts_ptr[ pEm->Neck_no ];
	neck = pP_ptr->Cdir_y;

	if (!pFp->Target_pl && (G.Room_enemy_flg & 0x20) && !(EF(pEm)->Die_cnt & 0x8000) && !pFp->Rev_flg) {

		sa = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, (EF(pEm)->Rev_flg << 11) | 0x100);
		if (dist <= 2700 && !sa && !pTg->Life) {
			pEm->Routine_1 = 6;
			*(short *)&pEm->Routine_2 = 0;
			pTg->Life = -1;
			pEm->Damage_cnt |= 0x80;	/* retail-only stores (0x80100E00 neck-grab arm) */
			pTg->Damage_cnt |= 0x80;
		}
		return;
	}

	if (pFp->Twins_flg) {
		pTfp = EF(pFp->pTg_em);
		if (!pTfp->Rev_flg && (G.Room_enemy_flg & 0x20)) return;
	}

	if (!pFp->No_catch) {
		sb = Parts_to_pl(pEm, 11, 1, &range);
		sa = Parts_to_pl(pEm, 11, 0x200, &range);

		if (range <= 1500 && !sa)	/* retail: 1300 -> 1500 */
			if ((0x800 - neck) * sb >= 0)
				if (!(EF(pEm)->Die_cnt & 0x8000) && !pFp->Target_pl && !(pTg->Damage_cnt & 0x80)) {

					pEm->Routine_1 = 10;
					*(short *)&pEm->Routine_2 = 0;
					G.Room_enemy_flg |= 0x80;
					pEm->Damage_cnt |= 0x80;	/* retail-only stores and vibration (catch arm) */
					pTg->Damage_cnt |= 0x80;
					Pad_vib_set0(3, 0);
					Pad_vib_set1(6, 0x96, 0);
					return;
				}
	}

	pEm->Cdir_y += neck;
	sa = Em2e_goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
	pEm->Cdir_y -= neck;
	if (dist <= 4000 && !sa && !pFp->No_atck && !(G.Room_enemy_flg & 0x40))

		if (pTg->Routine_0 == 1 && pTg->Routine_1 != 2) {
			pEm->Routine_1 = 4;

			*(short *)&pEm->Routine_2 = 0;	return;
		}

	pEm->Cdir_y += neck;
	sa = Em2e_goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
	pEm->Cdir_y -= neck;
	if (dist >= 4000 && dist <= 9000 && !sa && !pFp->No_poison && !(EF(pEm)->Die_cnt & 0x8000)) {

		pEm->Routine_1 = 5;
		*(short *)&pEm->Routine_2 = 0;
		pFp->Poison_type = 0;
	}
}

static void Em2e_Mv_reverse(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em2e_Mv_rev00(pEm, Kan, Seq);
	}else {

		Em2e_Mv_rev10(pEm, Kan, Seq);
	}
}

static void Em2e_Mv_rev00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Neck_flg |= 2;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0007;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);

		G.Room_enemy_flg &= 0xffbf;
		pEm->Be_flg &= 0xfffffffb;
		EF(pEm)->Nomove_flg = 1;
	}else {

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x40)) {
			Mv_rev00_0(pEm);
			return;
		}
		Add_speedXZ(pEm, 0);
	}

	pEm->Atd[0].Ofs_x -= pEm->Spd_x;

}

static void Mv_rev00_0(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register EM2E_FREE *pFp;

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	pFp = EF(pEm);

	EF(pEm)->Rev_flg ^= 1;
	pEm->Atd[0].Ofs_x = (!EF(pEm)->Rev_flg) ? 800 : -800;

	EF(pEm)->No_rev = 60;
	pEm->Routine_1 = 1;
	*(short *)&pEm->Routine_2 = 0;
	pEm->Neck_flg &= 0xfd;
	Head_open_ck(pEm);

	if (*(unsigned short *)&pTg->Routine_0 == 0x201 && ((0xbd0 >> (Rnd() & 0xf)) & 1)) {
		pEm->Routine_1 = 5;
		EF(pEm)->Poison_type = 1;
	}

	pFp->No_poison = 0;
	pFp->No_atck = 0;
	pFp->No_catch = 0;
}

static void Em2e_Mv_rev10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Neck_flg |= 2;

		G.Room_enemy_flg &= 0xffbf;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 5;
	}

	pEm->Atd[0].Ofs_x += (EF(pEm)->Rev_flg) ? 43 : -43;

	if (Joint_move(pEm, Kan, Seq, 0x40)) {
		EF(pEm)->No_atck = no_atck_rnd[ Rnd() & 7 ];
		Mv_rev00_0(pEm);
	}
}

static void Em2e_Mv_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Timer0 = -Em2e_goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 1);
		if (!pEm->Timer0) {
			pEm->Timer0 = (Rnd() & 1) ? 1 : -1;
		}
		pEm->Timer0 *= 6;
		pEm->Timer1 = 0x400;
	}

	pEm->Cdir_y += pEm->Timer0;
	pEm->Timer1 -= 6;
	Spd_get(pEm, Kan, Seq, 0);
	Joint_move(pEm, Kan, Seq, 0x40);

	if (pEm->Timer1 < 0) {
		pEm->Routine_1 = 1;
		*(short *)&pEm->Routine_2 = 0;
	}

	Mv_walk00(pEm);

}

static void Em2e_Mv_struck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Mv_struck00_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	Fire_die_ck(pEm);	}

static void Em2e_Mv_struck000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Routine_2 = 1;

	*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0002;
	pEm->Neck_flg &= 0xfd;
	EF(pEm)->Dmg_point = 15;	/* retail: 14 -> 15 (li @0x801016CC) */
	Spd_get(pEm, Kan, Seq, 0);
	Joint_move(pEm, Kan, Seq, 0x40);
	pEm->Spd_x = pEm->Spd_z = 0;

	G.Room_enemy_flg |= 0x40;

	pEm->Timer2 = 0;
	EF(pEm)->Stick_cnt = 90;

	EF(pEm)->Nomove_flg = 0;
}

static void Em2e_Mv_struck001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;
	register short eos;
	register long chr_type;

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
	Em2e_goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x300);

	Spd_get(pEm, Kan, Seq, 0);
	eos = Joint_move(pEm, Kan, Seq, 0x40);
	Add_speedXZ(pEm, 0);
	pEm->Atd[0].Ofs_x -= pEm->Spd_x;
	chr_type = *pEm->pNow_seq;

	Em_seq_sound(pEm);

	if ((chr_type & 0x10000) && !*(signed char *)&pEm->Timer2)
		pEm->Timer2 = Arm_hit_ck(pEm);

	if (chr_type & 0x20000) {
		G.Room_enemy_flg &= 0xffbf;
		EF(pEm)->No_atck2[0]++;
		EF(pEm)->No_atck2[1]++;
		if (Ck_schit_parts(pEm, 0x11, 0x28) || Ck_schit_parts(pEm, 0x17, 0x28))

			*(short *)&pEm->Routine_2 = 2;
	}

	if (eos)
		Mv_struck001_0(pEm);

}

static void Mv_struck001_0(ENEMY_WORK *pEm)
{

	EF(pEm)->No_atck = no_atck_rnd[ Rnd() & 7 ];
	EF(pEm)->No_poison = no_atck_rnd[ Rnd() & 7 ] + 30;
	EF(pEm)->No_catch = (*(signed char *)&pEm->Timer2) ? 15 : 0;	/* retail: 30 -> 15 (andi 0xF @0x80101934) */

	pEm->Atd[0].Ofs_x = (!EF(pEm)->Rev_flg) ? 800 : -800;
	pEm->Routine_1 = 1;
	*(short *)&pEm->Routine_2 = 0;
	Head_open_ck(pEm);
}

static void Em2e_Mv_struck002(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0006;
	}
	Spd_get(pEm, Kan, Seq, 0);
	if (Joint_move(pEm, Kan, Seq, 0x40))
		Mv_struck001_0(pEm);

	Add_speedXZ(pEm, 0);
	if ((*pEm->pNow_seq & 0x10000) && !*(signed char *)&pEm->Timer2)
		pEm->Timer2 = Arm_hit_ck(pEm);

}

static void Em2e_Mv_poison(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	mv_poison_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em2e_Mv_poison00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;

	pFp = EF(pEm);

	pFp->Em_next = pFp->Poison_type + ((pEm->Id == 0x2e) ? 0 : 8);

	pFp->Stick_cnt = 90;

	pEm->Be_flg &= 0xfffffffb;
	pFp->Nomove_flg = 0;

	*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0000;
	*(short *)&pEm->Routine_2 = 1;
	if (!pFp->Flower_flg) {
		*(short *)&pEm->Routine_2 = 2;
		pFp->Flower_r0 = 1;
		pFp->Flower_r1 = 0;
		pFp->Flower_free = 0x20;
	}
}

static void Em2e_Mv_poison10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register long chr_type;
	SVECTOR sv;

	register long dir;	/* inferred name; not in retail SYM (combined into the Esp_call argument) */
	Joint_move(pEm, Kan, Seq, 0x40);
	chr_type = *pEm->pNow_seq;
	if (!pEm->Routine_3) {
		if (chr_type & 0x10000) {
			pEm->Routine_3 = 1;
			pP_ptr = &pEm->pSin_parts_ptr[ pEm->Neck_no ];
			dir = pEm->Cdir_y + pP_ptr->Cdir_y + (EF(pEm)->Rev_flg << 11);
			pP_ptr = &pEm->pSin_parts_ptr[11];
			sv.vx = sv.vy = sv.vz = 0;
			Esp_call((EF(pEm)->Em_next << 16) | 0x1d000c00, dir, &pP_ptr->Workm, &sv);
			Snd_se_enem(3, pEm);
		}
	}else {

		if (chr_type & 0x20000)
			Em2e_Mv_poison30(pEm, Kan, Seq);
	}

}

static void Em2e_Mv_poison20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;
	SVECTOR sv;
	register PARTS_W *pP_ptr;	/* inferred; not in retail SYM (its one use is combined into the Esp_call argument) */
	register short dir;	/* inferred; not in retail SYM (extension combined into the call) */

	pFp = EF(pEm);

	switch (pEm->Routine_3) {

	case 0:
		if (!EF(pEm)->Flower_r1) break;

	case 1:

		pEm->Routine_3 = 2;
		pFp->Flower_r0 = 2;
		pFp->Flower_r1 = 0;
		pFp->Flower_free = 0x20;
		pP_ptr = &pEm->pSin_parts_ptr[11];
		dir = pEm->Cdir_y + pEm->pSin_parts_ptr[9].Cdir_y + (EF(pEm)->Rev_flg << 11);

		sv.vx = sv.vy = sv.vz = 0;
		Esp_call((EF(pEm)->Em_next << 16) | 0x1d000c00, dir, &pP_ptr->Workm, &sv);
		Snd_se_enem(3, pEm);
		break;

	case 2:

		if (EF(pEm)->Flower_r1)
			Em2e_Mv_poison30(pEm, Kan, Seq);
		break;

	}

	Joint_move(pEm, Kan, Seq, 0x40);
}

static void Em2e_Mv_poison30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	EF(pEm)->No_poison = poison_time_tbl[ Rnd() & 7 ] + 60;
	pEm->Routine_1 = 1;
	*(short *)&pEm->Routine_2 = 0;
	pEm->Spd_x = pEm->Spd_z = 0;
}

static void Em2e_Mv_neck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	mv_neck_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	EF(pEm)->No_atck2[0] = 1;
	EF(pEm)->No_atck2[1] = 1;	}

static void Em2e_Mv_neck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register short d2;
	register short rot, dir;	/* inferred; not in retail SYM (extensions combined into their uses) */
	pPl = &G.Player;
	d2 = 0;

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		EF(pEm)->Hand_req = 5;
		EF(pEm)->Req_att = 3;
		pEm->Be_flg |= 2;
		pEm->Neck_flg |= 2;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0000;
		G.Room_enemy_flg |= 0x40;
		pEm->Be_flg &= 0xfffffffb;

		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		*(long *)&G.Player.Routine_0 = 6;
		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		G.Player.Damage_cnt |= 0x80;
		pPl->Be_flg |= 0x42;

		G.Player.Spd_x = 100;
		G.Player.Spd_z = 0;

		Snd_se_enem(12, pEm);
	}

	Joint_move(pEm, Kan, Seq, 0x40);

	if ((rot = Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 0x100))) {
		G.Player.Cdir_y += rot;
	}else

		d2 = 1;

	if (pEm->L_pl > 1500) {
		dir = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z);
		Add_speedXZ(pPl, dir - pPl->Cdir_y);
	}else

		d2 |= 2;

	if (d2 == 3) {
		*(short *)&pEm->Routine_2 = 1;
		EF(pEm)->Req_att = 0xff;
	}
}

static void Em2e_Mv_neck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;

		*(long *)&pEm->Move_no = 0x3f0008;
		EF(pEm)->Flower_flg = 0;
		pEm->Cdir_y += EF(pEm)->Rev_flg << 11;

		Set_base_pos(&G.Player, Kan, Seq, 0);
		G.Player.Cdir_y = pEm->Cdir_y + 0x800;
		G.Player.Routine_1 = 1;
		G.Player.Routine_2 = 0;
		G.Player.Routine_3 = 0;

		G.System_flg |= 0x100;
		G.System_flg |= 0x4000000;
		Pad_vib_set0(3, 0);		/* retail-only vibration calls @0x80102114..0x80102150 */
		Pad_vib_set0(4, 8);
		Pad_vib_set0(3, 0x14);
		Pad_vib_set1(0x14, 0xc8, 0);
		Pad_vib_fade_set(0x1e, 0xc8, 0x32, 0x14);

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x40)) {
			pEm->Routine_3 = 2;
			*(long *)&pEm->Move_no = 0x3f0009;
		}
		break;

	case 2:

		if (Joint_move(pEm, Kan, Seq, 0x40))
			*(short *)&pEm->Routine_2 = 2;

	}
}

static void Em2e_Mv_neck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register PARTS_W *pP_ptr;
	register unsigned long chr_type, i;

	pPl = &G.Player;

	switch (pEm->Routine_3) {

	case 0:
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 0x3f000a;
		Pad_vib_set1(0x14, 0x78, 0);		/* retail-only vibration (0x801021C4 case 0) */
		Pad_vib_fade_set(0x1e, 0x78, 0x28, 0x14);

	case 1:

		if (Joint_move(pEm, Kan, Seq, 0x40)) {
			pEm->Routine_3 = 2;
			*(long *)&pEm->Move_no = 0x3f0000;
		}

		chr_type = *pEm->pNow_seq;
		if (chr_type & 0x10000) {
			Pad_vib_set0(0x1e, 0);		/* retail-only vibration before the part hide */
			Pad_vib_set1(3, 0xc8, 5);
			Pad_vib_set1(3, 0xc8, 0xf);
			Pad_vib_set1(8, 0xc8, 0x19);
			Pad_vib_fade_set(0x3c, 0xc8, 0, 0x21);
			pP_ptr = &pPl->pSin_parts_ptr[8];
			pP_ptr->Be_flg &= 0xfffffffe;
			if (pPl->Id & 1) {
				for (i = 15; i < 19; i++) {
					pP_ptr = &pPl->pSin_parts_ptr[i];
					pP_ptr->Be_flg &= 0xfffffffe;
				}
			}
			Snd_se_enem(7, pEm);
			Set_blood((ENEMY_WORK *)pPl, 8, 8);
		}

		if (chr_type & 0x20000) {
			Set_blood(pEm, Rnd() % 3 + 11, 4);
			if (!(Rnd() & 3))
				Set_blood(pEm, Rnd() % 3 + 11, 5);

		}

		if (chr_type & 0x40000)
			Set_blood((ENEMY_WORK *)pPl, 8, 8);

		break;

	case 2:

		EF(pEm)->Flower_flg = 1;
		EF(pEm)->Flower_rate = 0x100;
		if (!(Rnd() & 0xf))
			Set_blood(pEm, Rnd() % 3 + 11, 4);

		Joint_move(pEm, Kan, Seq, 0x40);

	}
}

static void Em2e_Mv_open(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0000;
		pEm->Neck_flg |= 2;

		EF(pEm)->Flower_r0 = 1;
		EF(pEm)->Flower_r1 = 0;
		EF(pEm)->Flower_free = 8;

		G.Room_enemy_flg &= 0xffbf;

		EF(pEm)->Nomove_flg = 0;	}else {

		if (!(Rnd() & 7))
			Set_blood(pEm, Rnd() % 3 + 11, 4);

		if (EF(pEm)->Flower_r1) {
			*(long *)&pEm->Routine_0 = 0x101;
			pEm->Damage_cnt &= 0x7f;
		}
	}

	Joint_move(pEm, Kan, Seq, 0x40);
}

static void Em2e_Mv_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	mv_fall_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	EF(pEm)->No_atck2[0] = 1;
	EF(pEm)->No_atck2[1] = 1;	}

static void Em2e_Mv_fall00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pTg;

	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pEm->Neck_flg = 1;
	pEm->Neck_no = 0;

	if (pEm->Type == 0x6005) {
		if (pEm->Sce_free0)
			*(short *)&pEm->Routine_2 = 1;

	}else

		if (((EF(pEm)->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl) < 4000 && pTg->Life >= 0)
			*(short *)&pEm->Routine_2 = 1;

	pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);

	EF(pEm)->Hand_req = 8;
	EF(pEm)->Req_att = 3;
}

static void Em2e_Mv_fall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		pEm->Spd_y = 0xf0;
		pEm->Be_flg &= 0xfffffbff;
		pEm->Be_flg |= 4;
		EF(pEm)->Nomove_flg = 0;
	}

	pEm->Spd_y += 0x50;
	if (pEm->Spd_y > 0x280)
		pEm->Spd_y = 0x280;

	Add_speedXYZ(pEm, 0, 0);

	if (pEm->Pos_y >= pEm->Ground) {
		*(short *)&pEm->Routine_2 = 2;
		pEm->Be_flg &= 0xfffffff7;
		pEm->Type &= 0xfff;
		pEm->Pos_y = pEm->Ground;
		Snd_se_enem(10, pEm);
	}
}

static void Em2e_Mv_fall20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:

		pEm->Routine_3 = 1;
		pEm->Timer0 = 15;
		pEm->Neck_no = 9;
		pEm->Neck_flg &= 0xfe;

	case 1:

		if (!Joint_move(pEm, Kan, Seq, 0x40))
			if (!Joint_move(pEm, Kan, Seq, 0x40)) break;

		pEm->Routine_3 = 2;
		*(long *)&pEm->Move_no = 12;
		Joint_move(pEm, Kan, Seq, 0x10040);
		EF(pEm)->Hand_req = 0x44;
		EF(pEm)->Req_att = 3;
		break;

	case 2:

		Joint_move(pEm, Kan, Seq, 0x10040);
		Joint_move(pEm, Kan, Seq, 0x10040);
		if (*pEm->pNow_seq & 0x20000) {
			pEm->Routine_1 = 1;
			*(short *)&pEm->Routine_2 = 0;
			pEm->Damage_cnt &= 0x7f;
		}

	}
}

static void Em2e_Mv_gas(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;

	pFp = EF(pEm);

	switch (pEm->Routine_2) {	case 0:	/* label line: a free layout choice (no code); on its own line ASPSX 2.56 starts its second
					   code chunk exactly at Set_blood line 3119 and mis-relocates that SLD record */

		pEm->Routine_2 = 1;
		pEm->Neck_flg |= 2;
		pEm->Be_flg |= 0x804;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x760c;
		Joint_move(pEm, Kan, Seq, 0x100);

		EF(pEm)->Hand_req = 9;
		EF(pEm)->Req_att = 3;

		EF(pEm)->Flower_flg = 1;
		EF(pEm)->Flower_rate = 0x60;

		EF(pEm)->Sick_flg = 1;	pEm->Size = 0x1000;
		break;

	case 1:

		pEm->Size += 4;
		if (pEm->Size > 0x1100) {
			pEm->Routine_2 = 2;

			pEm->Size = 0x1100;	}	break;

	case 2:
		pEm->Size -= 4;
		if (pEm->Size <= 0x1000) {
			pEm->Routine_2 = 1;
			pEm->Size = 0x1000;
		}

	}

	pFp->No_atck2[0] = 1;
	pFp->No_atck2[1] = 1;

	Fire_die_ck(pEm);

}

static void Em2e_Mv_catch(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	move_catch_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);

	EF(pEm)->No_atck2[0] = 1;
	EF(pEm)->No_atck2[1] = 1;	}

static void Em2e_Mv_catch00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register short rot;	/* inferred; not in retail SYM (extension combined into the test) */

	pPl = &G.Player;

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;

		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0008;
		EF(pEm)->Em_next = EF(pEm)->Flower_flg;
		EF(pEm)->Flower_flg = 0;
		pEm->Neck_flg |= 2;

		EF(pEm)->Nomove_flg = 1;

		pEm->Timer0 = 30;
		pEm->Timer1 = 45;	/* retail: 60 -> 45 */
		EF(pEm)->Retail_28 = 0x83;	/* retail-only store, sb 0x240 @0x80102B80 */
		EF(pEm)->Hand_req = 5;
		EF(pEm)->Req_att = 3;
		EF(pEm)->Stick_cnt = 90;

		G.Player.pEnemy_ptr = (unsigned long *)pEm;
		*(long *)&G.Player.Routine_0 = 5;
		G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
		G.Player.Damage_cnt |= 0x80;
		pPl->Be_flg |= 0x42;

		Set_base_pos(pPl, Kan, Seq, 0);

		Snd_se_enem(13, pEm);
	}
	if ((rot = Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 0x100)))
		G.Player.Cdir_y += rot;

	if (Joint_move(pEm, Kan, Seq, 0x40)) {
		*(short *)&pEm->Routine_2 = 1;
		G.Player.Cdir_y = pEm->Cdir_y + ((EF(pEm)->Rev_flg ^ 1) << 11);
	}
}

static void Em2e_Mv_catch10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;
	register unsigned long d0;
	SVECTOR sv;
	register PARTS_W *pP_ptr;	/* inferred; not in retail SYM (its one use is combined into the Esp_call argument) */

	pPl = &G.Player;

	switch (pEm->Routine_3) {
	case 0:

		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f0009;
		EF(pEm)->Hand_req = 3;
		EF(pEm)->Req_att = 3;

	case 1:

		if (!--pEm->Timer1) {
			pEm->Routine_3 = 2;
			EF(pEm)->Retail_28--;	/* retail-only @0x80102D7C..0x80102D90 */
		}
		break;

	case 2:

		if ((G.Room_enemy_flg & 0x20) && !EF(pEm)->Rev_flg) {
			pEm->Routine_1 = 6;
			*(short *)&pEm->Routine_2 = 2;
			Set_base_pos(pPl, Kan, Seq, 0);
			Em2e_Mv_neck20(pEm, Kan, Seq);
			G.Player.Routine_0 = 6;
			G.Player.Routine_1 = 2;
			G.Player.Routine_2 = 0;
			G.Player.Routine_3 = 0;
			G.System_flg |= 0x100;
			G.System_flg |= 0x4000000;
			return;
		}else {

			pEm->Routine_3 = 3;
			pEm->Timer2 = 3;
			pEm->Timer3 = 1;
			Snd_se_enem(3, pEm);
		}

	case 3:

		if (!--*(signed char *)&pEm->Timer3) {
			pP_ptr = &pEm->pSin_parts_ptr[11];
			sv.vx = (EF(pEm)->Rev_flg) ? -200 : 200;	/* retail: was 0 / -400 */
			sv.vy = -450;
			sv.vz = 0;
			Esp_call(0x1d030c00, pEm->Cdir_y + (EF(pEm)->Rev_flg << 11), &pP_ptr->Workm, &sv);
			pEm->Timer3 = 4;

			if (!--*(signed char *)&pEm->Timer2) {
				pEm->Routine_3 = 1;
				pEm->Timer1 = 25;	/* retail: 30 -> 25 */

				if ((d0 = Pl_life_down(20, 0)) != 2) {	/* retail: 10 -> 20 */
					pPl->Routine_2 = 2;
					pPl->Routine_3 = 0;
					if (d0)
						G.Room_enemy_flg |= 0x20;

				}else {

					pPl->Be_flg &= 0xffffffbd;
					Em2e_Pl_to_die(pPl);
					pEm->Atd[0].At_w = 300;
					pEm->Atd[0].At_d = 300;
					pEm->Be_flg |= 4;
					pEm->Be_flg &= 0xfffffffd;
					*(short *)&pEm->Routine_2 = 2;
				}
			}
		}

	}

	if (Joint_move(pEm, Kan, Seq, 0x40))
		pEm->Hokan_flg = 0x3f;

	pEm->Timer0 -= Gacha_ch();
	if ((pEm->Timer0 < 0 && pPl->Life >= 0) || !(EF(pEm)->Retail_28 & 0x7f)) {	/* retail adds the Retail_28 test and clear (@0x80102FC0..0x80102FE8) */
		*(short *)&pEm->Routine_2 = 2;
		pPl->Routine_1 = 2;
		pPl->Routine_2 = 0;
		pPl->Routine_3 = 0;
		EF(pEm)->Retail_28 = 0;
	}
}

static void Em2e_Mv_catch20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long eos;	/* inferred; not in retail SYM (combined into the test) */

	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f000b;
		EF(pEm)->Flower_flg = EF(pEm)->Em_next;
	}

	if ((eos = Joint_move(pEm, Kan, Seq, 0x40))) {
		pEm->Routine_1 = 1;
		*(short *)&pEm->Routine_2 = 0;
		EF(pEm)->No_atck = 30;
		EF(pEm)->No_poison = 30;
		EF(pEm)->No_catch = 45;
		G.Room_enemy_flg &= 0xff7f;
		pEm->Be_flg &= 0xffffffbd;

		EF(pEm)->Nomove_flg = 0;
		pEm->Damage_cnt &= 0x7f;	/* retail-only store: lbu/andi 0x7F/sb 0x1D3 @0x801030AC..0x801030C4 */
	}
}

/* retail-only move routine 11 (0x801030DC in slot 1, 0x801100DC in slot 0), names unknown */
static void func_801030DC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	mv_0b_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
}

static void func_80103118(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_3) {
		pEm->Routine_3 = 1;
		*(long *)&pEm->Move_no = 12;
		pEm->Neck_flg |= 2;
		Joint_move(pEm, Kan, Seq, 0x10040);
		EF(pEm)->Hand_req = 8;
		EF(pEm)->Req_att = 3;
	}
	if (Flg_ck(G.SaveData.Common_flg, 0x5f))
		*(short *)&pEm->Routine_2 = 1;
}

static void func_8010319C(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;

	pFp = EF(pEm);

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Timer0 = 0x33;

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x10040)) {
			*(long *)&pEm->Move_no = 0x7f0000;
			pEm->Routine_3 = 2;
		}
		if (pEm->Timer0) {
			pEm->Timer0--;
			pFp->Hand_req = 4;
			pFp->Req_att = 3;
		}
		break;

	case 2:
		Joint_move(pEm, Kan, Seq, 0x20);
		if (pEm->Sce_free0) {
			pEm->Routine_1 = 1;
			*(short *)&pEm->Routine_2 = 0;
			EF(pEm)->Stay = 0;
			EF(pEm)->No_poison = 45;
			pEm->Type &= 0xfff;
		}
	}
}

static void Em2e_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	em2e_dmg_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
	Hand_atck_ck(pEm);
	Em2e_Move_flower(pEm);

	Rot_neck_em(pEm, pEm->Cdir_y + (EF(pEm)->Rev_flg << 11));
	Set_flower(pEm);

}

static void Em2e_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!EF(pEm)->Sick_flg) {
		EF(pEm)->Hand_req = 6;
		EF(pEm)->Req_att = 3;
	}

	if (pEm->Routine_1 != 1) {
		if ((pEm->Damage_no % 3) || EF(pEm)->Sick_flg) {
			Set_blood(pEm, 10, 1);
		}else {

			Set_blood(pEm, ((Rnd() & 1) << 2) | 3, 1);
		}
	}

	if (pEm->Routine_1 != 16) {
		Snd_se_enem(2, pEm);
	}

	Spd_get(pEm, Kan, Seq, 0);
	if (EF(pEm)->Rno1_stk) {
		*(long *)&pEm->Routine_0 = (EF(pEm)->Rno3_stk << 24) + (1 + ((EF(pEm)->Rno2_stk << 16) + (EF(pEm)->Rno1_stk << 8)));	/* grouping inferred from retail's add order */
	}else {

		*(long *)&pEm->Routine_0 = 0x101;
		pEm->Neck_flg &= 0xfd;
		EF(pEm)->Hand_req = 3;
		EF(pEm)->Req_att = 3;
	}
}

static void Em2e_dmg_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (EF(pEm)->Sick_flg) {
		Em2e_dmg_nml(pEm, Kan, Seq);
		return;
	}

	if (!pEm->Routine_2) {
		*(short *)&pEm->Routine_2 = 1;
		G.Room_enemy_flg &= 0xffbf;

		*(long *)&pEm->Move_no = Cdir_ck(pEm, &G.Player) + 3 + EF(pEm)->Rev_flg * 13;
		Spd_get(pEm, Kan, Seq, 0);	/* retail-only call before Joint_move (@0x80103468 first arm) */
		Joint_move(pEm, Kan, Seq, 0x400);
		*(long *)&pEm->Spd_base_x = 0;	/* retail: sw 0x20C / sh 0x210 instead of Spd_x/Spd_z (@0x80103524/0x8010352C) */
		pEm->Spd_base_z = 0;
		if (!EF(pEm)->Sick_flg) {
			EF(pEm)->Hand_req = 6;
			EF(pEm)->Req_att = 3;
		}
		if ((pEm->Damage_no % 3) || EF(pEm)->Sick_flg) {
			Set_blood(pEm, 10, 0);
		}else {

			Set_blood(pEm, ((Rnd() & 1) << 2) | 3, 0);
		}
		Snd_se_enem(11, pEm);
		pEm->Be_flg &= 0xfffffffb;
	}else {

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move(pEm, Kan, Seq, 0x400)) {
			if (EF(pEm)->Sick_flg)
				*(long *)&pEm->Routine_0 = 0x901;
			else

				*(long *)&pEm->Routine_0 = 0x101;

			pEm->Atd[0].Ofs_x = (!EF(pEm)->Rev_flg) ? 800 : -800;
		}else

			Add_speedXZ(pEm, 0);
	}
}

static void Em2e_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!EF(pEm)->Sick_flg) {
		EF(pEm)->Hand_req = 6;
		EF(pEm)->Req_att = 3;
	}

	Set_blood(pEm, combo_mark_rnd[ Rnd() & 7 ], 0);	/* retail: mark 0, plus a second mark 1 for Routine_1 0x12 (@0x80103698..0x801036CC) */
	if (pEm->Routine_1 == 0x12)
		Set_blood(pEm, combo_mark_rnd[ Rnd() & 7 ], 1);

	Snd_se_enem(2, pEm);

	*(long *)&pEm->Routine_0 = (EF(pEm)->Rno3_stk << 24) + (1 + ((EF(pEm)->Rno2_stk << 16) + (EF(pEm)->Rno1_stk << 8)));	/* grouping inferred from retail's add order */
}

static void Em2e_dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		if ((pEm->Damage_no % 3) || EF(pEm)->Sick_flg) {
			Set_blood(pEm, 11, 2);
		}else {

			Set_blood(pEm, 5, 2);
		}
	}

	Em2e_dmg_nml(pEm, Kan, Seq);	/* retail: dmg_nml (jal @0x801037A8) instead of Beta dmg_ftl */	}

static void Set_dmg_fire(ENEMY_WORK *pEm, unsigned long type, long max)
{
	register unsigned long i, d0;

	d0 = Rnd() & 7;

	for (i = 0; i < max; i++)
		Set_blood(pEm, fire_rnd_tbl[ d0 + i ], type);

}

static void Em2e_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2)
		Set_dmg_fire(pEm, 2, 6);

	Em2e_dmg_nml(pEm, Kan, Seq);	/* retail: dmg_nml (jal @0x801038A0) instead of Beta dmg_ftl */	}

static void Em2e_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em2e_dmg_nml(pEm, Kan, Seq);	/* retail: dmg_nml (jal @0x801038D0) instead of dmg_ftl, and no Routine_3/Sick_flg gate */

	EF(pEm)->Hand_req = 0xc5;
	EF(pEm)->Req_att = Rnd() & 3;
	Set_dmg_fire(pEm, 7, 2);
}

static void Em2e_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 10);
	Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 10);

	EF(pEm)->Biri_cnt = 15;

	Em2e_dmg_nml(pEm, Kan, Seq);

}

static void Em2e_dmg_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2)
		if (!EF(pEm)->No_fire) {
			EF(pEm)->No_fire = 60;
			Set_dmg_fire(pEm, 2, 6);
		}

	Em2e_dmg_nml(pEm, Kan, Seq);	}

static void Em2e_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	em2e_die_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
	Rot_neck_em(pEm, pEm->Cdir_y + (EF(pEm)->Rev_flg << 11));	/* retail-only call @0x80103A64 */
	Em_dead_flg_on(pEm);
}

static void Em2e_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em2e_Die_nml00(pEm, Kan, Seq);
		if (pEm->Routine_1 != 1)
			Set_blood(pEm, 10, 1);

	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}
}

static void Em2e_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PLAYER_WORK *pPl;	/* [INFERRED] used carrier, as in Em2e_Mv_neck00 */
	pPl = &G.Player;

	if (EF(pEm)->Retail_28) {	/* retail-only block @0x80103AF4..0x80103B1C */
		pPl->Routine_1 = 2;
		pPl->Routine_2 = 0;
		pPl->Routine_3 = 0;
		EF(pEm)->Retail_28 = 0;
	}
	if (EF(pEm)->Sick_flg) {

		*(long *)&pEm->Routine_0 = 7;	}else {

		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = EF(pEm)->Rev_flg * 13 + 0x3f000c;
		G.Room_enemy_flg &= 0xffbf;
		pEm->Neck_flg |= 2;
		pEm->Be_flg |= 4;
	}
}

static void Em2e_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Spd_get(pEm, Kan, Seq, 0);
	if (Joint_move(pEm, Kan, Seq, 0x40))
		*(long *)&pEm->Routine_0 = 7;

	Add_speedXZ(pEm, 0);
	Em_seq_sound(pEm);
	if (!EF(pEm)->Sick_flg) {
		EF(pEm)->Hand_req = 6;
		EF(pEm)->Req_att = 3;
	}
	if (*pEm->pNow_seq & 0x10000) {
		EF(pEm)->Hand_req = 0x41;
		EF(pEm)->Req_att = 3;
	}
}

static void Break_parts(ENEMY_WORK *pEm, unsigned long st, unsigned long ed)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	for (i = st; i < ed; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[i];
		pP_ptr->Be_flg |= 0x4a;
		pP_ptr->Be_flg &= 0xffff7fff;	/* retail-only: and 0xFFFF7FFF @0x80103CB4 */
		pP_ptr->Free[0] = pEm->Cdir_y;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[2] = 300;
		pP_ptr->Free[1] = -150;
		pP_ptr->Free[3] = 10;
		pP_ptr->Free[6] = -100;
		pP_ptr->Poly_rgb = 0x104010;
	}
}

static void Em2e_Die_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long d0, d1;
	register unsigned long rno;	/* [INFERRED] used carrier: retail reads Routine_1 on entry (lbu s3 @0x80103D10) */

	rno = pEm->Routine_1;
	if (!pEm->Routine_2) {
		Em2e_Die_nml00(pEm, Kan, Seq);
		pEm->Neck_flg |= 1;

		if ((pEm->Damage_no % 3) || EF(pEm)->Sick_flg) {
			d0 = 12;
			d1 = 14;
		}else {

			d0 = 6;
			d1 = 9;
		}
		Set_blood(pEm, d0 - 1, 0);

		if (pEm->Damage_no < 3 || rno == 6 || rno == 8)	/* retail adds Routine_1 6 (@0x80103DAC) */
			Break_parts(pEm, d0, d1);

	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}
}

static void Em2e_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	/* retail: the Beta body moved into func_80103E78, followed by a new hand request (@0x80103DF8) */
	if (!pEm->Routine_2) {
		func_80103E78(pEm, Kan, Seq);
		EF(pEm)->Hand_req = 0x46;
		EF(pEm)->Req_att = (Rnd() % 3) | 0x80;
	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}
}

/* retail-only (0x80103E78 in slot 1, 0x80110E78 in slot 0), name unknown: the Beta Em2e_Die_gnml
 * body, shared by Em2e_Die_gnml and func_801042AC */
static void func_80103E78(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long d0, d1;

	Em2e_Die_nml00(pEm, Kan, Seq);
	pEm->Neck_flg |= 1;

	if ((pEm->Damage_no % 3) || EF(pEm)->Sick_flg) {
		d0 = 12;
		d1 = 14;
	}else {

		d0 = 6;
		d1 = 9;
	}

	Set_blood(pEm, d0 - 1, 0);
	Set_blood(pEm, d0 - 1, 3);

	Break_parts(pEm, d0, d1);
}

static void Em2e_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	/* retail rewrite (@0x80103F4C): normal die with fire marks, or a timed fade when Sick_flg */
	if (!EF(pEm)->Sick_flg) {
		if (!pEm->Routine_2) {
			Set_dmg_fire(pEm, 3, 6);
			Set_dmg_fire(pEm, 3, 6);
			pEm->Be_flg &= 0xfffffffb;
			Em2e_Die_nml00(pEm, Kan, Seq);
			G.Room_enemy_flg &= 0xffbf;
			EF(pEm)->Hand_req = 7;
			EF(pEm)->Req_att = 3;
			EF(pEm)->Die_cnt = 1;
		}else {

			Em2e_Die_nml10(pEm, Kan, Seq);
		}
	}else {

		if (!pEm->Routine_2) {
			pEm->Routine_2 = 1;
			pEm->Timer0 = 120;
			Set_dmg_fire(pEm, 3, 6);
			Set_dmg_fire(pEm, 3, 6);
			EF(pEm)->Hand_req = 7;
			EF(pEm)->Req_att = 3;
			EF(pEm)->Die_cnt = 1;
		}else {

			if (!--pEm->Timer0)
				*(long *)&pEm->Routine_0 = 7;
		}
	}
	Pcolor_dec(pEm);
}

static void Em2e_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	if (!pEm->Routine_2) {
		Em2e_Die_nml00(pEm, Kan, Seq);
		EF(pEm)->Hand_req = 0x45;
		EF(pEm)->Req_att = 3;
		Set_dmg_fire(pEm, 8, 4);
		for (i = 0; i < 26; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[i];
			pP_ptr->Poly_rgb = 0x3f2f5f;
		}
	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}

}

static void Em2e_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM2E_FREE *pFp;
	register PARTS_W *pP_ptr;
	register unsigned long i;

	pFp = EF(pEm);

	if (!pEm->Routine_2) {
		Em2e_Die_nml00(pEm, Kan, Seq);
		Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 10);
		Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 10);
		Set_blood(pEm, fire_rnd_tbl[ Rnd() & 0xf ], 10);

		pEm->Neck_no = 0;

		EF(pEm)->Biri_cnt = 45;
		EF(pEm)->Hand_req = 7;
		EF(pEm)->Req_att = 3;
		for (i = 0; i < 26; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[i];
			pP_ptr->Poly_rgb = 0x3f3f3f;
		}
	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}
	pFp->Die_cnt = 1;
}

static void Pcolor_dec(ENEMY_WORK *pEm)
{
	register unsigned long p0, p1, p2, i;
	register PARTS_W *pP_ptr;

	for (i = 0; i < 26; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[i];
		p0 = pP_ptr->Poly_rgb & 0xff;
		if (p0)
			p0--;

		p1 = pP_ptr->Poly_rgb & 0xff00;
		if (p1)
			p1 -= 0x100;

		p2 = pP_ptr->Poly_rgb & 0xff0000;
		if (p2 & 0xff0000)
			p2 -= 0x10000;

		pP_ptr->Poly_rgb = p2 | p1 | p0;
	}
}

/* retail-only (0x801042AC in slot 1, 0x801112AC in slot 0), name unknown: die-table entries 17 and 19
 * (Beta Em2e_Die_gnml) */
static void func_801042AC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		func_80103E78(pEm, Kan, Seq);
		EF(pEm)->Hand_req = 0x46;
		EF(pEm)->Req_att = 3;
		if (Rnd() & 1) {
			Break_parts(pEm, 14, 20);
			Set_blood(pEm, 14, 0);
		}else {
			Break_parts(pEm, 20, 26);
			Set_blood(pEm, 20, 0);
		}
	}else {

		Em2e_Die_nml10(pEm, Kan, Seq);
	}
}

static void Em2e_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pldmg_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em2e_Pl_dmg00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pTg;
	register long Ofs;

	pTg = (ENEMY_WORK *)pEm->pEnemy_ptr;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x3f0000;
		pEm->Neck_flg |= 1;
		pEm->Timer1 = SquareRoot0((pEm->Pos_x - pTg->Pos_x) * (pEm->Pos_x - pTg->Pos_x) + (pEm->Pos_z - pTg->Pos_z) * (pEm->Pos_z - pTg->Pos_z)) - 1471;
		*(signed char *)&pEm->Timer2 = 7;
		Snd_se_stad(0x4010001, &pEm->Pos_x);
	}

	if (*(signed char *)&pEm->Timer2) {
		Ofs = *(signed char *)&pEm->Timer2 * (pEm->Timer1 >> 3);
		(*(signed char *)&pEm->Timer2)--;
	}else

		Ofs = 0;

	Catch_pos_set(pEm, pTg, Ofs + 1471);
	if (Joint_move(pEm, Kan, Seq, 0x40)) {
		pEm->Routine_1 = 1;
		*(short *)&pEm->Routine_2 = 0;
	}

}

static void Em2e_Pl_dmg10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pTg;
	register long Ofs;

	pTg = (ENEMY_WORK *)pEm->pEnemy_ptr;

	switch (pEm->Routine_2) {
	case 0:

		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x1f0001;
		*(signed char *)&pEm->Timer2 = 31;
		Pad_vib_set1(0x14, 0x78, 0);		/* retail-only vibration @0x80104544.. */
		Pad_vib_fade_set(0x1e, 0x78, 0x28, 0x14);
		break;
	case 1:
		break;
	case 2:
		if (!pEm->Routine_3) {
			pEm->Routine_3 = 1;
			pEm->Timer3 = 1;
		}
		if (!--*(signed char *)&pEm->Timer3) {
			*(short *)&pEm->Routine_2 = 1;
			Snd_se_stad(0x4020001, &pEm->Pos_x);
			Set_blood(pEm, 0, 8);
			Pad_vib_set0(0xa, 0);		/* retail-only vibration @0x801045BC.. */
			Pad_vib_set1(6, 0xfa, 2);
			Pad_vib_set1(0x14, 0x78, 8);
			Pad_vib_fade_set(0x14, 0x78, 0x28, 0x1c);
		}
	}

	if (*(signed char *)&pEm->Timer2) {
		Ofs = *(signed char *)&pEm->Timer2 * 284 / 31;
		(*(signed char *)&pEm->Timer2)--;
	}else

		Ofs = 0;

	Catch_pos_set(pEm, pTg, Ofs + 1187);
	if (Joint_move(pEm, Kan, Seq, 0x80))
		pEm->Hokan_flg = 31;

}

static void Em2e_Pl_dmg20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x1f0004;
		Spd_get(pEm, Kan, Seq, 0);
		Joint_move(pEm, Kan, Seq, 0x40);
		pEm->Timer0 = 1187;
		return;
	}

	Spd_get(pEm, Kan, Seq, 0);
	if (Joint_move(pEm, Kan, Seq, 0x40)) {
		*(long *)&pEm->Routine_0 = 1;
		pEm->Damage_cnt &= 0x7f;
		pEm->Be_flg &= 0xffffffbd;
	}
	Add_speedXZ(pEm, 0x800);
	pEm->Timer0 += pEm->Spd_x;
	if (pEm->Timer0 >= 0x73a)
		pEm->Be_flg &= 0xffffffbd;

}

static void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pldie_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}

static void Em2e_Pl_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x3f0003;
		pEm->Neck_flg |= 1;
	}

	Joint_move(pEm, Kan, Seq, 0x40);	}

static void Em2e_Pl_die10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = 0x3f0000;
		pEm->Neck_flg |= 1;

	case 1:

		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;
		if (Joint_move(pEm, Kan, Seq, 0x40)) {
			pEm->Routine_2 = 2;
			*(long *)&pEm->Move_no = 0x3f0001;
		}
		break;

	case 2:

		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;
		if (Joint_move(pEm, Kan, Seq, 0x40)) {
			pEm->Routine_1 = 2;
			*(short *)&pEm->Routine_2 = 0;
		}

	}

}

static void Em2e_Pl_die20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {

	case 0:
		pEm->Routine_2 = 1;
		pEm->Be_flg |= 8;
		*(long *)&pEm->Move_no = 0x3f0002;

	case 1:

		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;
		pEm->Routine_1 += Joint_move(pEm, Kan, Seq, 0x40);
		break;

	case 2:	/* inferred: an explicit idle case; retail dispatch is the 3-case tree */
		break;

	}
}

static void Em2e_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Be_flg |= 0x42;
	EF(pEm)->Die_cnt = 0;	/* retail-only store: sh zero,0x238 (Free+0x20 = Die_cnt) @0x80104A58 */
	Em2e_kage_move(pEm);	}

static void Em2e_kage_move(ENEMY_WORK *pEm)
{
	register KAGE_WORK *pKage;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (EF(pEm)->Kage_routine) {
	case 0:

		EF(pEm)->Kage_routine = 1;
		*(unsigned long *)&pKage->Prim[0].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[0].r0 |= 0x3f3f6f;
		*(unsigned long *)&pKage->Prim[1].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[1].r0 |= 0x3f3f6f;
		pEm->Timer3 = 90;

	case 1:
		pKage->Half_x += 8;
		pKage->Half_z += 8;
		if (!--*(signed char *)&pEm->Timer3)
			EF(pEm)->Kage_routine = 2;
		break;

	case 2:	/* inferred: an explicit idle case; retail dispatch is the 3-case tree */
		break;
	}

}

static void Em2e_Move_flower(ENEMY_WORK *pEm)
{
	move_flower_tbl[ EF(pEm)->Flower_r0 ](pEm, EF(pEm));	}

static void Em2e_Mv_flower00(ENEMY_WORK *pEm, EM2E_FREE *pFp)
{
}

static void Em2e_Mv_flower10(ENEMY_WORK *pEm, EM2E_FREE *pFp)
{
	if (!pFp->Flower_r1) {
		pFp->Flower_flg = 1;
		pFp->Flower_rate += pFp->Flower_free;
		if (pFp->Flower_rate >= 0x100) {
			pFp->Flower_r1 = 1;
			pFp->Flower_rate = 0x100;
		}
	}
}

static void Em2e_Mv_flower20(ENEMY_WORK *pEm, EM2E_FREE *pFp)
{
	if (!pFp->Flower_r1) {
		pFp->Flower_flg = 1;
		pFp->Flower_rate -= pFp->Flower_free;
		if (pFp->Flower_rate <= 0) {
			pFp->Flower_r1 = 1;
			pFp->Flower_rate = 0;
			pFp->Flower_flg = 0;
		}
	}
}

static PLAYER_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register long dst0, dst1;

	pTg = &G.Player;
	EF(pEm)->Target_pl = 0;

	dst0 = pEm->L_pl;
	dst1 = pEm->L_spl & 0x3fffffff;

	if (G.Status_flg & 0x10000000) {
		if (dst0 > 6000 || dst1 > 6000) {
			if ((pEm->L_spl & 0xc0000000) == 0x40000000 && dst1 < dst0) {

				pTg = (PLAYER_WORK *)G.pEnemy[1];
				EF(pEm)->Target_pl = 1;
			}
		}
	}

	return pTg;
}

static long Arm_ck_main(ENEMY_WORK *pEm, PLAYER_WORK *pTg, unsigned long No, long R)
{
	register PARTS_W *pPt;
	register long ck, flg;

	pPt = &pEm->pSin_parts_ptr[No];
	flg = (EF(pEm)->Die_cnt & 0x8000) != 0;

	if (L_pos_ckXZ(&pTg->Pos_x, pPt->Workm.t, R)) {
		if (!(pTg->Damage_cnt & 0x80) && pEm->nFloor == pTg->nFloor) {
			if (pTg->Life >= 0) {
				if (pTg == &G.Player) {	/* retail: was !Target_pl; bne pTg,&G.Player @0x80104D18 */
					ck = Pl_life_down(EF(pEm)->Dmg_point, flg);
				}else

					ck = Spl_life_down(pEm, EF(pEm)->Dmg_point, flg);

				switch (ck) {
				case 1:
					G.Room_enemy_flg |= 0x20;

				case 0:
					Em2e_Pl_to_dmg(pTg, EF(pEm)->Rev_flg ^ Cdir_ck(pEm, pTg));
					break;

				case 2:
					Em2e_Pl_to_die(pTg);

				}
			}
			Snd_se_enem(1, pEm);
			return 1;
		}
	}

	return 0;
}

static long Arm_hit_ck(ENEMY_WORK *pEm)
{
	register EM2E_HITSIZE *pAt;
	register unsigned long i;

	pAt = Arm_hit_size;

	for (i = 0; i < 4; i++) {
		if (Arm_ck_main(pEm, &G.Player, pAt->No, pAt->R)) return 1;

		if (G.Status_flg & 0x10000000)
			if (Arm_ck_main(pEm, (PLAYER_WORK *)G.pEnemy[1], pAt->No, pAt->R)) return 1;

		pAt++;
	}

	return 0;

}

static void Em2e_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir)
{
	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir + 2;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
	Pad_vib_set0(3, 0);		/* retail-only vibration calls @0x80104EF0/0x80104F00 */
	Pad_vib_set1(8, 200, 0);
}

static void Em2e_Pl_to_die(PLAYER_WORK *pPl)
{
	pPl->Routine_0 = 3;
	pPl->Routine_1 = 0;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;
}

static void Set_flower(ENEMY_WORK *pEm)
{
	register short rot;
	register PARTS_W *pP_ptr;	/* inferred; not in retail SYM (folded into each access by cse) */

	if (EF(pEm)->Flower_flg) {

		pP_ptr = &pEm->pSin_parts_ptr[11];
		rot = EF(pEm)->Flower_rate;

		pP_ptr->Cdir_z = -rot;
		RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);

		pP_ptr++;

		pP_ptr->Cdir_y = rot;
		pP_ptr->Cdir_z = rot << 1;
		RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);

		pP_ptr++;

		pP_ptr->Cdir_y = -rot;
		pP_ptr->Cdir_z = rot << 1;
		RotMatrix((SVECTOR *)&pP_ptr->Cdir_x, (MATRIX *)pP_ptr->M);
	}
}

static unsigned long Head_open_ck(ENEMY_WORK *pEm)
{
	if (pEm->Life <= EF(pEm)->Life_half && pEm->Life >= 0 && !EF(pEm)->Flower_flg
	 && G.Player.Life >= 0 && !(G.Room_enemy_flg & 0x80)) {	/* retail adds the last two tests (G+0x3B66, G+0x3A0C @0x80104FEC..0x8010500C) */
		pEm->Routine_1 = 7;
		*(short *)&pEm->Routine_2 = 0;

		return 1;
	}

	return 0;
}

static unsigned long Hand_atck_ck(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register EM2E_FREE *pFp;
	register long range;
	register short att;
	register short rot;	/* inferred; not in retail SYM (extension combined into the test) */
	pFp = EF(pEm);
	pTg = (EF(pEm)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	range = (EF(pEm)->Target_pl) ? (pEm->L_spl & 0x3fffffff) : pEm->L_pl;

	rot = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
	Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x700);

	if (pTg->Life >= 0) {

		if (range > 1500) {

			pFp->Stick_cnt = 90;	return 0;
		}

		if (pFp->Stick_cnt) {

			pFp->Stick_cnt--;	return 0;
		}

		att = (rot < 0) ? 1 : 2;
		if (!pFp->No_atck2[ att - 1 ] && !(G.Room_enemy_flg & 0x80)) {
			pFp->Hand_req = 4;
			pFp->Req_att = att;
			pFp->No_atck2[ pFp->Req_att - 1 ] = 15;
			return 1;
		}
		/* retail drops the Beta "No_atck2[att - 1] = 7" fallback (no store after the test @0x80105038) */
	}

	return 0;
}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	register ENEMY_WORK *pTg;
	register PLAYER_WORK *pPl;
	SVECTOR Sv;
	register long dir, Id;
	register EM2E_BLOOD_TBL *tbl;

	tbl = &blood_id_tbl[ type ];

	pTg = pEm;

	pPl = (EF(pTg)->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;
	dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);

	pP_ptr = &pTg->pSin_parts_ptr[ parts ];

	Id = tbl->Id & 0x7f;

	if (!(tbl->Id & 0x80)) {
		Sv.vx = pP_ptr->Workm.t[0];
		Sv.vy = pP_ptr->Workm.t[1];
		Sv.vz = pP_ptr->Workm.t[2];
		Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
	}else {

		Sv.vx = Sv.vy = Sv.vz = 0;
		Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
	}
}

static long Cut_hand(ENEMY_WORK *pEm, unsigned long Id)
{
	register EM2E_FREE *pFp;	/* [INFERRED] gcc folds it into the field addresses (no retail SYM record) */

	pFp = EF(pEm);

	if ((pFp->Hand_rest >> Id) & 1) {
		pFp->Hand_req = 0xc2;
		pFp->Req_att = Id;
		pFp->Hand_rest &= ~(1 << Id);
		return 0;
	}

	return 1;
}

static long Ck_schit_parts(ENEMY_WORK *pEm, unsigned long no, long r)
{
	register PARTS_W *pP_ptr;

	pP_ptr = &pEm->pSin_parts_ptr[ no ];

	return Sca_ck_hit(pP_ptr->Workm.t, r, 1 << pEm->nFloor, 0x400);

}

static short Em2e_goto00_ck(ENEMY_WORK *pEm, long dx, long dz, short rot)
{
	register short rr;

	rr = pEm->Cdir_y;
	pEm->Cdir_y = rr + (EF(pEm)->Rev_flg << 11);
	rot = Goto00_ck(pEm, dx, dz, rot);
	pEm->Cdir_y = rr;

	return rot;
}

static void Get_workm(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr[0].M, &pP_ptr[0].Workm);

	CompM(&pP_ptr[0].Workm, (MATRIX *)pP_ptr[1].M, &pP_ptr[1].Workm);

	CompM(&pP_ptr[0].Workm, (MATRIX *)pP_ptr[5].M, &pP_ptr[5].Workm);

	CompM(&pP_ptr[0].Workm, (MATRIX *)pP_ptr[9].M, &pP_ptr[9].Workm);
	CompM(&pP_ptr[9].Workm, (MATRIX *)pP_ptr[10].M, &pP_ptr[10].Workm);

	CompM(&pP_ptr[10].Workm, (MATRIX *)pP_ptr[14].M, &pP_ptr[14].Workm);
	CompM(&pP_ptr[14].Workm, (MATRIX *)pP_ptr[15].M, &pP_ptr[15].Workm);

	CompM(&pP_ptr[10].Workm, (MATRIX *)pP_ptr[20].M, &pP_ptr[20].Workm);
	CompM(&pP_ptr[20].Workm, (MATRIX *)pP_ptr[21].M, &pP_ptr[21].Workm);
}

static void Catch_pos_set(ENEMY_WORK *pEm, ENEMY_WORK *pTg, long Ofs)
{
	register PARTS_W *pP_ptr;
	SVECTOR v0, v1;
	MATRIX M;

	pP_ptr = &pTg->pSin_parts_ptr[ pTg->Neck_no ];

	v0.vx = pTg->Cdir_x;
	v0.vy = (pTg->Cdir_y + pP_ptr->Cdir_y + pP_ptr->Free[0] + (EF(pTg)->Rev_flg << 11)) & 0xfff;

	v0.vz = pTg->Cdir_z;
	RotMatrix(&v0, &M);

	v1.vx = Ofs;
	v1.vy = v1.vz = 0;
	ApplyMatrixSV(&M, &v1, &v0);
	pEm->Pos_x = pTg->Pos_x + v0.vx;
	pEm->Pos_z = pTg->Pos_z + v0.vz;

}

static short Parts_to_pl(ENEMY_WORK *pEm, unsigned long No, short Rot, long *Dist)
{
	register PARTS_W *pP_ptr;
	register long xx, zz, rr;	register PLAYER_WORK *pPl;	/* pPl: inferred; not in retail SYM (folded into its accesses) */
	register long dir;	/* inferred; not in retail SYM (combined into the return) */

	xx = pEm->Pos_x;
	zz = pEm->Pos_z;
	rr = pEm->Cdir_y;

	pP_ptr = &pEm->pSin_parts_ptr[ No ];
	pEm->Pos_x = pP_ptr->Workm.t[0];
	pEm->Pos_z = pP_ptr->Workm.t[2];

	if (No > 8) {
		pP_ptr = &pEm->pSin_parts_ptr[ pEm->Neck_no ];
		pEm->Cdir_y += pP_ptr->Free[0];	/* retail drops the part's Cdir_y term (only lhu 0x98 @0x80105630) */
	}
	pPl = &G.Player;
	*Dist = SquareRoot0((pEm->Pos_x - G.Player.Pos_x) * (pEm->Pos_x - G.Player.Pos_x) + (pEm->Pos_z - G.Player.Pos_z) * (pEm->Pos_z - G.Player.Pos_z));
	dir = Em2e_goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, Rot);

	pEm->Pos_x = xx;
	pEm->Pos_z = zz;
	pEm->Cdir_y = rr;

	return dir;
}

static void Oba_ck_em2e(ENEMY_WORK *pEm)
{
	register ENEMY_WORK *pEm2;
	register ENEMY_WORK **ppEm;

	Oba_set_ofs(pEm, &pEm->Pos_x);

	pEm->At_em_no = 0xff;

	for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {

		pEm2 = *ppEm;
		if (pEm != pEm2)

			if (pEm2->Be_flg & 1) {
				Oba_ck_hit(pEm2, pEm);
			}
	}

}

