/* em26.c - reconstruction of D:\Bio2\prog\EM26.C (enemy 26).
 * Linked into two overlays from the same source: 0x0B (at 0x8010001c) and
 * 0x29 (at 0x8010d01c); only the enemy-number-specific entry names differ
 * (Em126/Em226, Pldamage_Em126/226), selected by OVL_29.
 * Types come from the TU's retail SYM preamble (em26.h). */
#include "EM26.H"

/* Em26 work variables kept in the enemy's generic Free[] area */
#define EF	((EM26_FREE *)EM_FREE_AREA(pEm))

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of other translation units ----
 * EM26.C includes no prototypes for them: every external routine is called
 * through an implicit declaration (arguments unnarrowed, implicit int).  The
 * implicit declarations are what open the record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static void (*Em26_routine_0[8])() = { Em26_Init, Em26_Move, Em26_Damage, Em26_Die, Em26_Scenario, 0, 0, Em26_Dead };
static unsigned char routine_tbl[6] = { 0, 0, 1, 4, 2, 2 };
static void (*em26_move_tbl[5])() = { Em26_Mv_dummy, Em26_Mv_walk, Em26_Mv_fall, Em26_Mv_jump, Em26_Mv_swim };
static void (*mv_walk_tbl[2])() = { Em26_Mv_walk00, Em26_Mv_walk10 };
static unsigned char dieblood_type_tbl[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0 };
static EM26_BLOOD_TBL blood_id_tbl[5] = { { 0, 25, 0, 4096 }, { 0, 1, 2048, 2048 }, { 26, 0, 0, 1024 }, { 5, 4, 0, 4096 }, { 4, 15, 0, 3072 } };	/* retail: entry 0 blood id 25 (Beta 9), retail .data 0x801010DC */

void EM_MAIN(ENEMY_WORK *pEm)
{

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F)
			pEm->Damage_cnt--;

		Em26_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		pEm->nFloor = -pEm->Pos_y / 1800;

		if (pEm->Be_flg) {
			Oba_ck_em(pEm);
			Sca_ck_em(pEm, 0x400);
		}
	}

}
static void Em26_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register unsigned long i;

	pEm->Life = 1;

	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	for (i = 0; i < 12; i++)	/* retail clears all 12 words */
		((ULONG *)EM_FREE_AREA(pEm))[i] = 0;

	*(long *)&pEm->Move_no = 0xF0000;

	pEm->Be_flg |= 0x4c00000;	/* retail 0x4c00000 (Beta 0x4000000) */
	pEm->Neck_flg = 0;	/* retail-only */
	pEm->Neck_no = 0;

	EF->kasakasarot = 0x78;
	EF->No_catch_cnt = Rnd() & 0x1F;
	EF->pTarget_pl = &G.Player;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -10;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 10;
	pEm->Atd[0].At_h = 10;
	pEm->Atd[0].At_d = 10;
	pEm->Atd[0].W = 10;
	pEm->Atd[0].D = 10;
	pEm->Eff_at_r = 20;

	pEm->Spd_x = 0;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	pEm->Cdir_x = 0;
	pEm->Cdir_z = 0;

	pEm->Damage_cnt = 6;
	pEm->Timer0 = 0;

	*(unsigned long *)&pEm->Routine_0 = (routine_tbl[(unsigned char)pEm->Type] << 8) + 1;

	switch ((unsigned char)pEm->Type) {
	case 0:
		pEm->Be_flg |= 0x8044A;

		pEm->Life = -1;
		pEm->Type |= 0x4000;	/* retail-only */
		break;

	case 1: case 2: case 3:
		break;

	case 4: case 5:

		pEm->Be_flg &= 0xE3FFFFFF;
		pEm->Be_flg |= 2;
		pEm->Spd_x = 200;
		break;
	}

	Kage_work_set(&pEm->pKage_work, 0, 0xC800C8, 0, &pEm->Pos_x);

	if (Water_ck(pEm->Pos_x, pEm->Pos_z))
		pEm->Be_flg |= 0x400;

}
static void Em26_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	em26_move_tbl[pEm->Routine_1](pEm, Kan, Seq); }
static void Em26_Mv_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Joint_move(pEm, Kan, Seq, 0x400);

}
static void Em26_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PLAYER_WORK *pTg; register long Dir0, dist;

	mv_walk_tbl[pEm->Routine_2](pEm, Kan, Seq);

	pTg = EF->pTarget_pl;

	Dir0 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);

	if (EF->No_catch_cnt)
		EF->No_catch_cnt--;

	dist = (pTg == &G.Player) ? pEm->L_pl : pEm->L_spl;
	if (dist < 2000 && !Dir0)
		if (!EF->No_catch_cnt && !(pTg->Damage_cnt & 0x80)) {
			pEm->Routine_1 = 3;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}

	if (pEm->L_pl < 500 && (G.Room_player_flg & 6) && pEm->nFloor == G.Player.nFloor) {
		if (*(unsigned short *)&G.Player.Routine_0 == 0x101 || *(unsigned short *)&G.Player.Routine_0 == 0x201 || *(unsigned short *)&G.Player.Routine_0 == 0x301 || *(unsigned short *)&G.Player.Routine_0 == 0x401) {

			*(unsigned long *)&pEm->Routine_0 = 3;
			Pad_vib_set0(1, 0);	/* retail-only (Dual Shock) */
			Pad_vib_set1(3, 80, 0);	}
	}

	if (G.Status_flg & 0x10000000) {
		pTg = (PLAYER_WORK *)G.pEnemy[1];
		if (pEm->nFloor == pTg->nFloor)
			if ((pEm->L_spl & 0x3FFFFFFF) < 500)
				if (*(unsigned short *)&pTg->Routine_0 == 0x101 || *(unsigned short *)&pTg->Routine_0 == 0x201 || *(unsigned short *)&pTg->Routine_0 == 0x301 || *(unsigned short *)&pTg->Routine_0 == 0x401)

					*(unsigned long *)&pEm->Routine_0 = 3;
	}

	Joint_move(pEm, Kan, Seq, 0x400);
}
static void Em26_Mv_walk00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;

		pEm->Timer0 = ((*(int (*)(void))Rnd)() + 0x1E) & (EF->Atck_flg * 30 + 0x3F);
		EF->Atck_flg = 0;
		*(signed char *)&EF->Chase_rot = ((0x1111 >> (Rnd() & 0xF)) & 1) ? -0x40 : 0x40;

		if (G.Status_flg & 0x10000000) {
			if (pEm->L_pl < (pEm->L_spl & 0x3FFFFFFF))
				EF->pTarget_pl = &G.Player;

			else
				EF->pTarget_pl = (PLAYER_WORK *)G.pEnemy[1];
		}

	case 1:
		if ((unsigned short)pEm->Timer0-- == 0)
			*(unsigned short *)&pEm->Routine_2 = 1;

	}

	if (!Get_sc_hit(pEm)) {
		pEm->Routine_1 = 2;
		*(unsigned short *)&pEm->Routine_2 = 0;
	}

}
static void Em26_Mv_walk10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PLAYER_WORK *pTg;

	pTg = EF->pTarget_pl;

	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;

		pEm->Spd_x = (Rnd() & 0x3F) + 0x5A;

		pEm->Spd_z = 0;

		pEm->Timer0 = Rnd() & 0x4E;

	case 1:
		Goto00(pEm, pTg->Pos_x, pTg->Pos_z, (signed char)EF->Chase_rot);

		pEm->Cdir_y += (signed char)EF->kasakasarot;
		*(signed char *)&EF->kasakasarot *= -1;

		if ((unsigned short)pEm->Timer0-- == 0)
			*(unsigned short *)&pEm->Routine_2 = 0;

	}

	Add_speedXZ(pEm, (signed char)EF->kasakasarot);

	if (Water_ck(pEm->Pos_x, pEm->Pos_z)) {
		pEm->Pos_x = pEm->Old_x;
		pEm->Pos_y = pEm->Old_y;
		pEm->Pos_z = pEm->Old_z;
	}else {

		if (!Get_sc_hit(pEm)) {
			pEm->Routine_1 = 2;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}
	}
}
static void Em26_Mv_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register long hit;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Spd_y = 40;
		pEm->Be_flg |= 2;
		EF->Jump_flg = 1;
	}

	Add_speedXYZ(pEm, 0, 0);
	if (pEm->Spd_y < 800)
		pEm->Spd_y += 40;

	else
		pEm->Spd_y = 800;

	pEm->Damage_cnt = 0xFF;

	hit = Get_sc_hit(pEm); if (hit) {
		pEm->Damage_cnt = 0;
		pEm->Be_flg &= ~2;
		pEm->Be_flg |= 0x4000000;
		EF->Jump_flg = 0;

		if (hit < 0) {
			pEm->Pos_y = hit + 10;
			pEm->Routine_1 = 4;
			Set_blood(pEm, 0, 0x82);
			pEm->Be_flg |= 0x400;
		}else {

			pEm->Pos_y = pEm->Ground;
			pEm->Routine_1 = 1;
			pEm->Be_flg &= ~0x400;
		}
		*(unsigned short *)&pEm->Routine_2 = 0;
	}

	Joint_move(pEm, Kan, Seq, 0x400);

}
static void Em26_Mv_jump(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register long hit; register EM26_FREE *pFp;

	pFp = (EM26_FREE *)EM_FREE_AREA(pEm);

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;

		pEm->Spd_x = (Rnd() & 0x3F) + 0x5A;
		pEm->Spd_y = -300;
		pEm->Be_flg |= 0x8000000;

		pFp->Atck_flg = 0;
		pFp->Jump_flg = 1;

	}

	Add_speedXYZ(pEm, 0, 0);
	pEm->Spd_y += 40;

	if (!Em26_atck_ck(pEm, 0))
		if (G.Status_flg & 0x10000000)
			Em26_atck_ck(pEm, 1);

	hit = Get_sc_hit(pEm); if (hit) {
		pEm->Be_flg &= 0xF7FFFFFF;
		pFp->Jump_flg = 0;
		if (hit < 0) {
			Set_blood(pEm, 0, 0x82);
			pEm->Life = -1;
			pEm->Pos_y = hit + 10;
			*(unsigned long *)&pEm->Routine_0 = 3;
			pEm->Be_flg |= 0x400;
		}else {

			pEm->Routine_1 = 1;
			*(unsigned short *)&pEm->Routine_2 = 0;
			pFp->No_catch_cnt = (Rnd() & 0x1F) + 0x1E;
			pEm->Be_flg &= ~0x400;
			pEm->Pos_y = pEm->Ground;
		}
	}
}
static void Em26_Mv_swim(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register PLAYER_WORK *pTg; register long Dir0, dist;

	pTg = EF->pTarget_pl;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pEm->Timer0 = 450;
		pEm->Timer1 = (Rnd() & 1) ? 0x20 : -0x20;
		pEm->Timer2 = Rnd() & 0x1F;
		pEm->Be_flg |= 0xC000000;
	}

	Dir0 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);

	pEm->Cdir_y += (signed char)EF->kasakasarot + pEm->Timer1;
	*(signed char *)&EF->kasakasarot *= -1;

	Joint_move(pEm, Kan, Seq, 0x400);
	if (!(++pEm->Timer2 & 0xF)) {
		Set_blood(pEm, 0, 0x82);
		pEm->Timer2 += Rnd() & 7;
	}

	dist = (EF->pTarget_pl == &G.Player) ? pEm->L_pl : pEm->L_spl;
	if (dist < 2000 && !Dir0)
		if (!(pTg->Damage_cnt & 0x80)) {
			pEm->Routine_1 = 3;
			*(unsigned short *)&pEm->Routine_2 = 0;
		}

	pEm->Pos_y = pEm->Water + 10;

	if (Get_sc_hit(pEm) >= 0) {
		pEm->Routine_1 = 1;

		*(unsigned short *)&pEm->Routine_2 = 0; }
	else {

		if (!--pEm->Timer0) {
			pEm->Life = -1;
			*(unsigned long *)&pEm->Routine_0 = 3;
		}
	}
}
/* [INFERRED] Retail has no Catch_plparts body between Em26_Mv_swim and
 * Em26_Damage. The unreferenced Beta-only helper emitted 360 unwanted bytes
 * here in each bank. Keep Beta source unchanged; do not invent retail code. */
static void Em26_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em26_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
	case 1:
		break;
	case 2:		/* empty third case: retail's decision tree is gcc's three-node split (root 1) */
		break;
	}

	return;
}
static void Em26_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em26_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em26_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (Water_ck(pEm->Pos_x, pEm->Pos_z) >= pEm->Ground || EF->Jump_flg) {
		Em26_Die00(pEm, Kan, Seq);
	}else

		Em26_Die10(pEm, Kan, Seq);

}
static void Em26_Die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	register KAGE_WORK *pKage;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		pKage = pEm->pKage_work;

		if (!EF->Jump_flg) {
			*(unsigned long *)&pKage->Prim[0].r0 &= 0xFF000000;
			*(unsigned long *)&pKage->Prim[0].r0 |= 0xBF10BF;
			*(unsigned long *)&pKage->Prim[1].r0 &= 0xFF000000;
			*(unsigned long *)&pKage->Prim[1].r0 |= 0xBF10BF;
		}else

			pEm->Be_flg |= 0x400;

		Snd_se_enem(7, pEm);
		Set_blood(pEm, 0, dieblood_type_tbl[pEm->Routine_1 - 1]);
	}else

		Kill_enemy_work(pEm);

}
static void Em26_Die10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Be_flg |= 2;

	pEm->Pos_y += 30;
	pEm->Cdir_y += 0x20;
	pEm->Cdir_z += 0x40;
	Joint_move(pEm, Kan, Seq, 0x400);

	if (pEm->Pos_y >= pEm->Ground || pEm->Pos_y < pEm->Water)
		Kill_enemy_work(pEm);

}
static void Em26_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
static void Em26_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
void PLDAMAGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}
/* [INFERRED] Get_plparts was used only by the absent Catch_plparts helper.
 * Retail Em26_atck_ck follows PLDAMAGE_EM directly, without this 76-byte
 * Beta-only body. Its declaration/definition must not emit extra code. */
static unsigned long Em26_atck_ck(ENEMY_WORK *pEm, unsigned long pl)
{	register unsigned long dist; register PLAYER_WORK *pPl; register EM26_FREE *pFp;

	pFp = (EM26_FREE *)EM_FREE_AREA(pEm);

	if (!pl) {
		pPl = &G.Player;
		dist = pEm->L_pl;
	}else {

		pPl = (PLAYER_WORK *)G.pEnemy[1];
		dist = pEm->L_spl & 0x3FFFFFFF;
	}

	if (dist < pPl->Atd[0].At_w + 10 && (unsigned long)(pPl->Pos_y - pEm->Pos_y) < 3000 && !pFp->Atck_flg) {

		if (!(pPl->Damage_cnt & 0x80)) {
			if (!pl) {
				Em26_Pl_to_dmg(pPl, Cdir_ck(pEm, pPl));
				Pl_life_down(1, 1);
				Pad_vib_set0(3, 0);	/* retail-only (Dual Shock) */
			}else

				Spl_life_down(pEm, 1, 1);

			Set_blood(pEm, 0, 1);
			pFp->Atck_flg = 1;
			pEm->Spd_x = 0x5A;
			pEm->Cdir_y += 0x800;
			return 1;
		}
	}

	return 0;
}
static void Set_blood(ENEMY_WORK *pTg, unsigned char parts, unsigned char type)
{	register PARTS_W *pP_ptr; SVECTOR Sv; register long dir; register EM26_BLOOD_TBL *tbl;

	tbl = &blood_id_tbl[type & 0x7F];

	dir = pTg->Cdir_y + tbl->Dir;

	pP_ptr = pTg->pSin_parts_ptr;
	pP_ptr += parts;

	Sv.vx = pP_ptr->Workm.t[0];
	Sv.vy = pP_ptr->Workm.t[1];
	if ((type & 0x80) && pTg->Water < pTg->Ground)
		Sv.vy = pTg->Water + 10;

	Sv.vz = pP_ptr->Workm.t[2];
	Esp_call((tbl->Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
}
static void Em26_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir)
{
	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;
	pPl->Damage_cnt |= 0x80; }
static long Get_sc_hit(ENEMY_WORK *pEm)
{	register long wat;

	wat = Water_ck(pEm->Pos_x, pEm->Pos_z);

	if (pEm->Pos_y >= pEm->Ground)
		return 1;

	if (pEm->Pos_y - 10 >= wat)
		return wat;

	return 0;
}
