/* em25_s0.c - reconstruction of D:\Bio2\prog\EM25_S0.C (enemy 25: damage, death,
 * scenario and helper routines).  Retail EM25.C #includes this file: its static
 * tables point at the static functions here and vice versa, and both files' code
 * and data form one object.  em25.c therefore #includes it at its end; compiled on
 * its own (the EM25_S0.C gate) the guarded block below supplies the declarations
 * em25.c would otherwise provide.  Linked into overlays 0x0A and 0x28. */
#ifndef EM25_C	/* standalone build */
#include "EM25.H"

extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* ---- functions of this file ---- */

/* ---- functions of D:\Bio2\prog\EM25.C (static there) ---- */
#endif

/* ---- data (EM25_S0.C) ---- */
static void (*Em25_dm[19])() = {
	Em25_dm_nml,
	Em25_dm_nml,
	Em25_dm_nml,
	Em25_dm_combo,
	Em25_dm_ftl,
	Em25_dm_ftl,
	Em25_dm_ftl,
	Em25_dm_ftl,
	Em25_dm_gnml,
	Em25_dm_fire,
	Em25_dm_H2SO4,
	Em25_dm_nml,
	Em25_dm_nml,
	Em25_dm_spark,
	Em25_dm_combo,
	Em25_dm_fire2,
	Em25_dm_ftl,
	Em25_dm_combo,
	Em25_dm_nml
};
static void (*dmg_nml_tbl[4])() = {
	Em25_dm_nml00,
	Em25_dm_nml10,
	Em25_dm_nml20,
	Em25_dm_nml20
};
static unsigned char next_rtn_tbl[8] = { 0x1, 0x9, 0x1, 0x9, 0x1, 0x5, 0x1, 0x5 };
static void (*dmg_ftl_tbl[4])() = {
	Em25_dm_ftl00,
	Em25_dm_ftl10,
	Em25_dm_ftl20,
	Em25_dm_ftl20
};
static void (*dm_ftl11_tbl[3])() = {
	Em25_dm_ftl110,
	Em25_dm_ftl111,
	Em25_dm_ftl112
};
static void (*dm_ftl21_tbl[3])() = {
	Em25_dm_ftl210,
	Em25_dm_ftl211,
	Em25_dm_ftl112
};
static unsigned char hitmark_tbl00[4] = { 0x0, 0x1, 0x2, 0x13 };
static void (*em25_die_tbl[19])() = {
	Em25_Die_nml,
	Em25_Die_nml,
	Em25_Die_nml,
	Em25_Die_nml,
	Em25_Die_ftl,
	Em25_Die_ftl,
	Em25_Die_ftl,
	Em25_Die_ftl,
	Em25_Die_gnml,
	Em25_Die_fire,
	Em25_Die_H2SO4,
	Em25_Die_nml,
	Em25_Die_nml,
	Em25_Die_spark,
	Em25_Die_nml,
	Em25_Die_fire,
	Em25_Die_ftl,
	Em25_Die_nml,
	Em25_Die_nml
};
static void (*die_nml_tbl[4])() = {
	Em25_Die_nml00,
	Em25_Die_nml10,
	Em25_Die_nml20,
	Em25_Die_nml20
};
static void (*die_nml10_tbl[3])() = {
	Em25_Die_nml100,
	Em25_Die_nml101,
	Em25_Die_nml102
};
static SVECTOR shibuki_tbl[8] = {
	{ 0, 0, 0, 0 },
	{ 250, 0, 500, 0 },
	{ 250, 0, -500, 0 },
	{ 500, 0, 1000, 0 },
	{ 500, 0, -1000, 0 },
	{ 750, 0, 500, 0 },
	{ 750, 0, -500, 0 },
	{ 1000, 0, 0, 0 }
};
static void (*die_nml20_tbl[3])() = {
	Em25_Die_nml200,
	Em25_Die_nml201,
	Em25_Die_nml102
};
static void (*die_ftl_tbl[4])() = {
	Em25_Die_ftl00,
	Em25_Die_ftl10,
	Em25_Die_ftl20,
	Em25_Die_ftl20
};
static void (*die_ftl20_tbl[3])() = {
	Em25_Die_ftl200,
	Em25_Die_nml201,
	Em25_Die_nml102
};
static unsigned char float_fy[8] = { 0x0, 0x8, 0x10, 0x20, 0x20, 0x10, 0x8, 0x0 };
static EM25_BLOOD_TBL blood_id_tbl[11] = {
	{ 0, 25, 0, 8192 },
	{ 0, 25, 0, 10240 },
	{ 9, 10, 512, 2048 },
	{ 0, 9, 0, 4096 },
	{ 133, 3, 0, 5120 },
	{ 133, 4, 0, 4096 },
	{ 132, 15, 0, 3072 },
	{ 132, 15, 0, 5120 },
	{ 26, 0, 0, 6144 },
	{ 134, 0, 0, 4096 },
	{ 132, 12, 0, 4096 }
};
static unsigned char foot_se_tbl[4] = { 0x0, 0x8, 0x9, 0x8 };
static unsigned char biripos_tbl[8] = { 0x0, 0x3, 0x6, 0x9, 0xB, 0x0, 0x10, 0x13 };

static void Em25_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2)
		EF->Dmg_type = pEm->Routine_1;

	if (!pEm->Water)
		pEm->Water = Water_ck(pEm->Pos_x, pEm->Pos_z);

	Em25_dm[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);	}

static void Em25_dm_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_nml_tbl[ EF->Place ](pEm, Kan, Seq);	}

static void Em25_dm_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned long next;	/* [INFERRED] name: next Routine_0..3 word (retail-only) */

	switch (pEm->Routine_2) {
		case 0:
			pEm->Routine_2 = 1;
			*(unsigned long *)&pEm->Move_no = 0x30001;
			if (EF->Dmg_type != 1)
				Set_blood(pEm, 0, 0);

		case 1:

			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:
			/* retail: 0x80102E00..0x80102E58 */
			if (pEm->Routine_1 == 1 || !(unsigned char)(pEm->Damage_no / 3))
				next = 0x901;
			else
				next = (next_rtn_tbl[ Rnd() & 7 ] << 8) + 1;
			*(unsigned long *)&pEm->Routine_0 = next;
	}
}

static void Em25_dm_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM25_FREE *pFp;	/* inferred name: gcc folds it into each use (no retail SYM record) */

	pFp = EF;
	if (pFp->Cont_flg) {
		*(unsigned long *)&pEm->Routine_0 = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
		if (pFp->Dmg_type != 1)
			Set_blood(pEm, 0, 0);

		Em25_Move(pEm, Kan, Seq);
	}else {

		Em25_dm_nml00(pEm, Kan, Seq);
	}	}

static void Em25_dm_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM25_FREE *pFp;	/* inferred name: gcc folds it into each use (no retail SYM record) */

	pFp = EF;
	if (pFp->Dmg_type != 1)
		Set_blood(pEm, 1, 0);

	if (pFp->Cont_flg) {
		*(unsigned long *)&pEm->Routine_0 = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
		Em25_Move(pEm, Kan, Seq);
	}else {

		pFp->Em_next = 1;
		*(unsigned long *)&pEm->Routine_0 = 0x401;
	}

}

static void Em25_dm_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	dmg_ftl_tbl[ EF->Place ](pEm, Kan, Seq);	}

static void Em25_dm_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
		case 0:
			pEm->Routine_2 = 1;
			*(unsigned long *)&pEm->Move_no = 0x30001;

			Foot_break_set(pEm);

		case 1:
			pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
			break;

		case 2:
			*(unsigned long *)&pEm->Routine_0 = 0x901;

	}

}

static void Em25_dm_ftl10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (EF->Cont_flg) {
		Foot_break_set(pEm);
		*(unsigned long *)&pEm->Routine_0 = (EF->Rno3_bak << 24) + ((EF->Rno2_bak << 16) + (EF->Rno1_bak << 8) + 1);
		Em25_Move(pEm, Kan, Seq);
	}else

		if (!(pEm->Type & 1)) {
			dm_ftl11_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
		}else

			Em25_dm_ftl12(pEm, Kan, Seq);

}

static void Em25_dm_ftl110(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	pEm->Routine_2 = 1;

	pEm->Spd_x = 0;
	pEm->Spd_y = 40;
	pEm->Spd_z = 0;

	pEm->Timer0 = 0x800;
	*(unsigned long *)&pEm->Move_no = 0x30001;
	EF->Jump_flg = 1;
	EF->Flip = 0;

	pEm->Be_flg &= 0xE3FFFFFF;
	pEm->Damage_cnt |= 0x80;

	Foot_break_set(pEm);

	Em25_Set_child(pEm, 0x2004, (Rnd() & 1) + 1);
}

static void Em25_dm_ftl111(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Add_speedXYZ(pEm, 0, pEm->Timer0);
	pEm->Spd_y += 40;

	if (pEm->Timer0) {
		pEm->Cdir_z += 0x200;
		pEm->Timer0 -= 0x200;
	}
	if (pEm->Pos_y >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground;
		pEm->Be_flg &= 0xEFFFFFFF;
		pEm->Be_flg |= 0xC000000;
		if (!pEm->Timer0) {
			pEm->Routine_2 = 2;
			*(unsigned long *)&pEm->Move_no = 0x30009;
		}
		Snd_se_enem(((signed char)EF->Hitline < 0) ? 6 : 5, pEm);
	}

	Joint_move(pEm, Kan, Seq, 0x400);

}

static void Em25_dm_ftl112(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (Joint_move(pEm, Kan, Seq, 0x400)) {
		EF->Jump_flg = 0;
		EF->Place = 0;
		pEm->Be_flg |= 0xC000000;
		pEm->Damage_cnt &= 0x7F;
		*(unsigned long *)&pEm->Routine_0 = 1;
	}
}

static void Em25_dm_ftl12(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		Em25_Set_child(pEm, 0x2004, (Rnd() & 1) + 1);
	}
	Em25_dm_ftl00(pEm, Kan, Seq);
}

static void Em25_dm_ftl20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (EF->Cont_flg) {
		Foot_break_set(pEm);
		*(unsigned long *)&pEm->Routine_0 = (EF->Rno3_bak << 24) + ((EF->Rno2_bak << 16) + (EF->Rno1_bak << 8) + 1);
		Em25_Move(pEm, Kan, Seq);
	}else

		if (!(pEm->Type & 1)) {
			dm_ftl21_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);
		}else {

			Foot_break_set(pEm);

			Em25_Set_child(pEm, 0x2004, (Rnd() & 1) + 1);

			EF->Em_next = 1;
			*(unsigned long *)&pEm->Routine_0 = 0x401;
		}
}

static void Em25_dm_ftl210(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em25_dm_ftl110(pEm, Kan, Seq);

	Joint_move(pEm, Kan, Seq, 0x400);

	pEm->Spd_x = 480;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	if (EF->Wall_pos & 1) {
		pEm->Timer0 = 0;
		pEm->Timer1 = (EF->Wall_pos << 10) - pEm->Cdir_y;
	}else {

		pEm->Timer0 = -pEm->Cdir_x;
		pEm->Timer1 = 0;
	}

	pEm->Be_flg &= 0xE3FFFFFF;

}

static void Em25_dm_ftl211(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em25_add_speed(pEm, pEm->Timer0, pEm->Timer1, 0);
	pEm->Spd_x += 40;

	if (pEm->Pos_y + 400 >= pEm->Ground) {
		pEm->Cdir_x = 0;
		pEm->Cdir_z = 0;
		pEm->Cdir_y = -(EF->Sc_dir + (EF->Wall_pos << 10) * (EF->Wall_pos & 1));

		pEm->Pos_y = pEm->Ground;
		pEm->Be_flg &= 0xEFFFFFFF;
		pEm->Be_flg |= 0xC000000;
		pEm->Routine_2 = 2;
		*(unsigned long *)&pEm->Move_no = 0x30009;
		Joint_move(pEm, Kan, Seq, 0x400);

		pEm->Atd[ 0 ] = Em25_hit_tbl[ 0 ];
		EF->Kage_routine = 0;
		Snd_se_enem(((signed char)EF->Hitline < 0) ? 6 : 5, pEm);
	}
}

static void Em25_dm_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long no;

	if (!pEm->Routine_2) {
		Set_blood(pEm, hitmark_tbl00[ Rnd() & 3 ], 3);
		no = Rnd() & 7;
		if (!((EF->Foot_flg >> no) & 1))
			Set_blood(pEm, no * 2 + 3, 3);

		if ((signed char)pEm->D_life_u < 0 || (signed char)pEm->D_life_c < 0 || (signed char)pEm->D_life_d < 0) {
			Foot_break_set(pEm);
			pEm->D_life_u = 6;
			pEm->D_life_c = 6;
			pEm->D_life_d = 6;
		}
	}

	Em25_dm_nml(pEm, Kan, Seq);
}

static void Em25_dm_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, d0;

	pEm->Routine_1 = 7;
	Em25_dm_ftl(pEm, Kan, Seq);

	for (i = 0; i < 2 - pEm->Damage_no / 3; i++) {
		d0 = (Rnd() & 0xF) + 3;
		pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
		if ((pP_ptr->Be_flg & 0x4B) == 1)
			Set_blood(pEm, d0, 5);

	}

}

static void Em25_dm_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, d0;

	for (i = 0; i < 5; i++) {
		d0 = (Rnd() & 0xF) + 3;
		pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
		if ((pP_ptr->Be_flg & 0x4B) == 1)
			Set_blood(pEm, d0, 5);

	}

	pEm->Routine_1 = 2;
	Em25_dm_nml(pEm, Kan, Seq);
}

static void Em25_dm_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long base, no;

	no = Rnd() & 7;
	base = no * 2 + 3;
	if (EF->Foot_cnt >= 3 && !((EF->Foot_flg >> no) & 1)) {
		EF->Foot_flg |= 1 << no;
		EF->Foot_cnt--;

		pP_ptr = &pEm->pSin_parts_ptr[ base ] + 1;
		pP_ptr->Be_flg |= 0x1062;
		pP_ptr->Poly_rgb = 0x101F3F;
		pP_ptr->Old_x = pEm->Cdir_y + 0x800;
		pP_ptr->Timer3 = 0;
		pP_ptr->Old_z = 100;
		pP_ptr->Old_y = 0;
		pP_ptr->Timer1 = 30;
		pP_ptr->Old_x2 = 0;
		pP_ptr->Old_y2 = pEm->Cdir_y;
		pP_ptr->Old_z2 = 0x400;
	}
	Set_blood(pEm, base, 7);
	pP_ptr = &pEm->pSin_parts_ptr[ base ];
	pP_ptr->Poly_rgb = 0x102F4F;

	pEm->Routine_1 = 2;
	Em25_dm_nml(pEm, Kan, Seq);
}

static void Em25_dm_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	EF->Spark_cnt = 20;
	Set_blood(pEm, 0, 10);
	pEm->Routine_1 = 7;
	Em25_dm_ftl(pEm, Kan, Seq);	}

static void Em25_dm_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register EM25_FREE *pFp;
	register unsigned long i, d0;

	pFp = EF;
	if (!pFp->No_fire) {
		for (i = 0; i < 5; i++) {
			d0 = (Rnd() & 0xF) + 3;
			pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
			if ((pP_ptr->Be_flg & 0x4B) == 1)
				Set_blood(pEm, d0, 5);

		}
		pFp->No_fire = 30;
	}

	*(unsigned long *)&pEm->Routine_0 = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
	Em25_Move(pEm, Kan, Seq);
}

static void Em25_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2)
		EF->Dmg_type = pEm->Routine_1;

	if (!pEm->Water)
		pEm->Water = Water_ck(pEm->Pos_x, pEm->Pos_z);

	em25_die_tbl[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);

	Em25_float_move(pEm);

	Em_dead_flg_on(pEm);
}

static void Em25_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!EF->Die_weak) {
		die_nml_tbl[ EF->Place ](pEm, Kan, Seq);
	}else {

		Em25_Die_nml30(pEm, Kan, Seq);
	}	}

static void Em25_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register EM25_FREE *pFp;

	pFp = EF;

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;

		*(unsigned long *)&pEm->Move_no = 0x7000C;
		if (pFp->Dmg_type != 1)
			Set_blood(pEm, 0, 0);

		pEm->Be_flg |= 0x42;
	}

	Joint_move(pEm, Kan, Seq, 0x200);
	if (*pEm->pNow_seq & 0x80000) {
		pEm->Be_flg &= 0xE3FFFFFF;
		if (pEm->Water) {	/* retail-only block 0x80103E34..0x80103E94 */
			switch (EF->Dmg_type) {
				case 9:
				case 10:
				case 16:
					Set_blood(pEm, 0, 7);
			}
			*(signed char *)&EF->Hitline = -1;
			if (!EF->Water_rno) {
				EF->Water_rno = 3;
				EF->Retail_27 = 1;
			}
		}
		*(unsigned long *)&pEm->Routine_0 = 7;
		if (!pFp->No_redie) {
			pEm->Life = 1;
			pFp->Die_weak = 1;
		}
	}

}

static void Em25_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_nml10_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em25_Die_nml100(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Routine_2 = 1;
	pEm->Spd_x = 0;
	pEm->Spd_y = 40;
	pEm->Spd_z = 0;
	pEm->Cdir_z = 0;

	pEm->Pos_y += 800;
	*(unsigned long *)&pEm->Move_no = 0x90C;

	if (EF->Dmg_type != 1) {
		Set_blood(pEm, 0, 0);
	}	}

static void Em25_Die_nml101(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Joint_move(pEm, Kan, Seq, 0x200);
	Add_speedXYZ(pEm, 0, 0);
	pEm->Spd_y += 40;

	if (Ck_land(pEm, 0, 0, 0)) {
		pEm->Routine_2 = 2;
		pEm->Be_flg &= 0xE7FFFFFF;
		pEm->Be_flg |= 0x4000000;
		EF->Place = 0;
		pEm->Be_flg |= 0x42;
		Snd_se_enem(((signed char)EF->Hitline < 0) ? 6 : 5, pEm);
		Em25_Die_nml1010(pEm);

		if ((signed char)EF->Hitline < 0)
			switch (EF->Dmg_type) {
				case 9: case 10: case 16:

					Set_blood(pEm, 0, 7);
					break;

			}
	}
}

static void Em25_Die_nml1010(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned long i;	/* inferred name: no retail record (strength-reduced to the table offset) */

	if (EF->Cont_flg)
		G.Room_enemy_flg &= 0xFFDF;

	if ((signed char)EF->Hitline <= 0) {

		pP_ptr = pEm->pSin_parts_ptr;
		for (i = 0; i < 8; i++) {
			Sv.vx = shibuki_tbl[ i ].vx + pP_ptr->Workm.t[ 0 ];
			Sv.vy = pEm->Water;
			Sv.vz = shibuki_tbl[ i ].vz + pP_ptr->Workm.t[ 2 ];
			Esp_call(0x1A002000, pEm->Cdir_y, &GsIDMATRIX, &Sv);
		}

		EF->Water_rno = 1;
		EF->Retail_27 = 1; /* retail-only store, sb @0x80104134 */
	}	}

static void Em25_Die_nml102(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Joint_move(pEm, Kan, Seq, 0x400);
	if (*pEm->pNow_seq & 0x80000) {
		*(unsigned long *)&pEm->Routine_0 = 7;
		pEm->Be_flg &= 0xE3FFFFFF;
		if (!EF->No_redie) {
			pEm->Life = 1;
			EF->Die_weak = 1;
		}
	}
}

static void Em25_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_nml20_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em25_Die_nml200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Routine_2 = 1;

	pEm->Timer0 = 0x800;
	*(unsigned long *)&pEm->Move_no = 0x901;
	Joint_move(pEm, Kan, Seq, 0x400);

	EF->Jump_flg = 1;
	EF->Flip = 0;

	pEm->Spd_x = 480;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	if (EF->Wall_pos & 1) {
		pEm->Timer0 = 0;
		pEm->Timer1 = (EF->Wall_pos << 10) - pEm->Cdir_y;
	}else {

		pEm->Timer0 = -pEm->Cdir_x;
		pEm->Timer1 = 0;
	}

	if (EF->Dmg_type != 1)
		Set_blood(pEm, 0, 0);

}

static void Em25_Die_nml201(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em25_add_speed(pEm, pEm->Timer0, pEm->Timer1, 0);
	pEm->Spd_x += 40;

	if (Ck_land(pEm, 400, 0, 0)) {
		pEm->Cdir_x = 0;
		pEm->Cdir_z = 0;
		pEm->Cdir_y = -(EF->Sc_dir + (EF->Wall_pos << 10) * (EF->Wall_pos & 1));

		pEm->Routine_2 = 2;

		EF->Place = 0;
		pEm->Be_flg |= 0x42;
		*(unsigned long *)&pEm->Move_no = 0x90C;
		Joint_move(pEm, Kan, Seq, 0x200);

		pEm->Atd[ 0 ] = Em25_hit_tbl[ 0 ];
		EF->Kage_routine = 0;
		Snd_se_enem(((signed char)EF->Hitline < 0) ? 6 : 5, pEm);
		Em25_Die_nml1010(pEm);
		if ((signed char)EF->Hitline < 0)
			switch (EF->Dmg_type) {
				case 9: case 10: case 16:

					Set_blood(pEm, 0, 7);
					break;

			}
	}

}

static void Em25_Die_nml30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(unsigned long *)&pEm->Move_no = 0x7000D;
	Joint_move(pEm, Kan, Seq, 0x200);

	Em25_Set_child(pEm, 0x2002, (Rnd() & 3) + 1);

	Set_blood(pEm, 0, 0);

	*(unsigned long *)&pEm->Routine_0 = 7;
	pEm->Be_flg |= 0x42;

	if ((signed char)EF->Hitline < 0)
		pEm->Pos_y = pEm->Water + 70;

}

static void Em25_Die_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!EF->Die_weak) {
		die_ftl_tbl[ EF->Place ](pEm, Kan, Seq);
	}else {

		Em25_Die_ftl30(pEm, Kan, Seq);
	}	}

static void Em25_Die_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		EF->Hitline = 1;	/* retail-only, sb @0x80104574 */
		pEm->Be_flg |= 2;
		*(unsigned long *)&pEm->Move_no = 0x7000C;
		Set_blood(pEm, 19, 1);
		Die_break_set(pEm);

		Em25_Set_child(pEm, 0x2002, (Rnd() & 3) + 6);
	}

	Joint_move(pEm, Kan, Seq, 0x200);
	if (*pEm->pNow_seq & 0x40000) {
		if (pEm->Water) {	/* retail-only block, as in Em25_Die_nml00 */
			switch (EF->Dmg_type) {
				case 9:
				case 10:
				case 16:
					Set_blood(pEm, 0, 7);
			}
			*(signed char *)&EF->Hitline = -1;
			if (!EF->Water_rno) {
				EF->Water_rno = 3;
				EF->Retail_27 = 1;
			}
		}
		*(unsigned long *)&pEm->Routine_0 = 7;
	}

}

static void Em25_Die_ftl10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {

		Set_blood(pEm, 19, 1);
		Die_break_set(pEm);

		Em25_Set_child(pEm, 0x2004, (Rnd() & 3) + 6);
	}

	Em25_Die_nml10(pEm, Kan, Seq);

}

static void Em25_Die_ftl20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	die_ftl20_tbl[ pEm->Routine_2 ](pEm, Kan, Seq);	}

static void Em25_Die_ftl200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	pEm->Routine_2 = 1;

	pEm->Timer0 = 0x800;
	*(unsigned long *)&pEm->Move_no = 0x901;
	Joint_move(pEm, Kan, Seq, 0x400);

	EF->Jump_flg = 1;
	EF->Flip = 0;

	pEm->Spd_x = 480;
	pEm->Spd_y = 0;
	pEm->Spd_z = 0;

	if (EF->Wall_pos & 1) {
		pEm->Timer0 = 0;
		pEm->Timer1 = (EF->Wall_pos << 10) - pEm->Cdir_y;
	}else {

		pEm->Timer0 = -pEm->Cdir_x;
		pEm->Timer1 = 0;
	}

	Die_break_set(pEm);
	Set_blood(pEm, 19, 1);

	Em25_Set_child(pEm, 0x2004, (Rnd() & 3) + 6);

}

static void Em25_Die_ftl30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(unsigned long *)&pEm->Move_no = 0x7000D;
	Joint_move(pEm, Kan, Seq, 0x200);

	Em25_Set_child(pEm, 0x2002, (Rnd() & 3) + 6);

	Set_blood(pEm, 19, 1);
	Die_break_set(pEm);

	pEm->Be_flg |= 0x42;
	*(unsigned long *)&pEm->Routine_0 = 7;

	if ((signed char)EF->Hitline < 0)
		pEm->Pos_y = pEm->Water + 70;

}

static void Die_break_set(ENEMY_WORK *pEm)
{
	register unsigned long i;
	register PARTS_W *pP_ptr;	/* inferred name: no retail record */

	pEm->Neck_flg |= 1;

	pP_ptr = &pEm->pSin_parts_ptr[ 19 ];
	pP_ptr->Be_flg |= 0x4A;
	pP_ptr->Free[ 0 ] = pEm->Cdir_y + 0x800;
	pP_ptr->Free[ 4 ] = 0;
	pP_ptr->Free[ 2 ] = 0;
	pP_ptr->Free[ 1 ] = -240;
	pP_ptr->Free[ 3 ] = 15;
	pP_ptr->Free[ 6 ] = 0;
	pP_ptr->Poly_rgb = 0x2F2F2F;

	Set_blood(pEm, 19, 1);
	for (i = 0; i < 4; i++)
		Set_blood(pEm, 19, 2);

}

static void Em25_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, d0, d1;

	if (!pEm->Routine_2 && !EF->Die_weak) {

		d1 = pEm->Water ? 5 : 4;	/* retail: Water test (lh 0x10C @0x80104984) replaces ( Type & 1 ) + 4 */
		Set_blood(pEm, 0, d1);
		for (i = 0; i < 3; i++) {	/* retail: 4 -> 3 iterations (sltiu @0x80104A18) */
			d0 = (Rnd() & 0xF) + 3;
			pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
			if ((pP_ptr->Be_flg & 0x4B) == 1) {
				Set_blood(pEm, d0, d1);
				Set_blood(pEm, d0, 5);
			}
		}
	}

	Em25_Die_ftl(pEm, Kan, Seq);
}

static void Em25_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, d0, d1;

	if (!pEm->Routine_2 && !EF->Die_weak) {

		d1 = pEm->Water ? 5 : 4;
		Set_blood(pEm, 0, d1);
		for (i = 0; i < 4; i++) {
			d0 = (Rnd() & 0xF) + 3;
			pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
			if ((pP_ptr->Be_flg & 0x4B) == 1) {
				Set_blood(pEm, d0, d1);
				Set_blood(pEm, d0, 5);
			}
		}

		Chg_modelcol(pEm, 0x202F2F);
	}

	Em25_Die_nml(pEm, Kan, Seq);
}

static void Em25_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;	/* inferred name: no retail record */

	if (!pEm->Routine_2 && !EF->Die_weak) {

		pP_ptr = &pEm->pSin_parts_ptr[ 19 ];
		pP_ptr->Be_flg |= 0x10;
		pP_ptr->Free[ 0 ] = 0;
		pP_ptr->Free[ 1 ] = 0;
		pP_ptr->Free[ 3 ] = 90;
		((unsigned char *)pP_ptr->Free)[ 4 ] = 100;
		((unsigned char *)pP_ptr->Free)[ 5 ] = 100;

		Chg_modelcol(pEm, 0x101F3F);

		Set_blood(pEm, 19, 7);
		Set_blood(pEm, 0, 6);
		Set_blood(pEm, 1, 6);

		EF->No_redie = 1;
	}

	Em25_Die_nml(pEm, Kan, Seq);

}

static void Em25_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!EF->Die_weak) {
		Chg_modelcol(pEm, 0x3F3F3F);
		EF->Spark_cnt = 30;
		Set_blood(pEm, 19, 10);
		Set_blood(pEm, 0, 10);
	}

	pEm->Routine_1 = 2;
	Em25_Die_nml(pEm, Kan, Seq);	}

static void Em25_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em25_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (!pEm->Routine_1) {
		if (Joint_move(pEm, Kan, Seq, 0x200)) {
			pEm->Routine_1 = 1;
			pEm->Timer2 = (Rnd() & 0x1F) + 30;
		}
	}else {

		if (!--*(signed char *)&pEm->Timer2) {
			*(unsigned long *)&pEm->Move_no = 0x7000D;
			pEm->Routine_1 = 0;
		}
	}

	if (!pEm->Routine_2) {
		pEm->Routine_2 = 1;
		EF->Kage_routine = 2;
		pEm->Timer1 = 0;
	}

	if ((signed char)EF->Hitline < 0)
		Em25_float_move(pEm);

	pEm->Be_flg |= 0x4000000;
	if (pEm->Pos_y < G.Player.Pos_y)
		pEm->Be_flg |= 0x8000000;

}

static void Em25_kage_move(ENEMY_WORK *pEm)
{
	register KAGE_WORK *pKage;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (EF->Kage_routine) {

		case 0:
			if (!Water_ck(pEm->Pos_x, pEm->Pos_z)) {
				pEm->Be_flg &= 0xFFFFFBFF;
				break;
			}

		case 1:
			pEm->Be_flg |= 0x400;
			break;

		case 2:
			if (Water_ck(pEm->Pos_x, pEm->Pos_z))
				break;

			EF->Kage_routine = 3;
			*(unsigned long *)&pKage->Prim[ 0 ].r0 &= 0xFF000000;
			*(unsigned long *)&pKage->Prim[ 0 ].r0 |= 0xBF10BF;
			*(unsigned long *)&pKage->Prim[ 1 ].r0 &= 0xFF000000;
			*(unsigned long *)&pKage->Prim[ 1 ].r0 |= 0xBF10BF;
			pEm->Be_flg &= 0xFFFFFBFF;
			pEm->Timer3 = 90;

		case 3:
			pKage->Half_x += 8;
			pKage->Half_z += 8;
			if (!--*(signed char *)&pEm->Timer3)
				EF->Kage_routine = 4;

		case 4:

	}

}

static void Em25_float_move(ENEMY_WORK *pEm)
{
	register EM25_FREE *pFp;

	pFp = EF;

	if (!pFp->Water_rno)
		return;

	if (!pEm->Water) {	/* retail-only (0x80104F2C..0x80104F50): no water here */
		pFp->Hitline = 1;
		pFp->Retail_27 = 0;
		pEm->Pos_y = pEm->Ground;
		return;
	}

	switch (pFp->Water_rno) {
		case 1:
			pFp->Water_rno = 2;
			pEm->Spd_y = 140;
			pFp->Retail_2C = 0;	/* retail-only */

		case 2:
			/* retail (0x80104F98..0x80105030): bob around the surface instead of
			   Pos_y += Spd_y, Spd_y -= 20 */
			pFp->Retail_2C += pEm->Spd_y;
			pEm->Pos_y = pEm->Water + 70 + pFp->Retail_2C;
			pEm->Spd_y -= 15;
			if (pEm->Spd_y <= 0) {
				if (pEm->Spd_y < -70)
					pEm->Spd_y = -70;
				if (pEm->Pos_y <= pEm->Water) {
					pEm->Pos_y = pEm->Water;
					pFp->Water_rno = 3;
				}
			}else if (pEm->Pos_y >= pEm->Ground) {
				pEm->Pos_y = pEm->Ground;
				pEm->Spd_y = 0;
			}
			break;

		case 3:
			pEm->Pos_y = pEm->Water + 70 + float_fy[ (pEm->Timer1++ & 0x3F) >> 3 ];

	}

}

static void Em25_foot_set(ENEMY_WORK *pEm, unsigned char Flg)
{
	register PARTS_W *pP_ptr;
	register unsigned char no;
	MATRIX Tm;
	register short dx, dz;	/* inferred names: no retail record */

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);

	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &Tm);

	pP_ptr += (Flg << 3) + 4;
	no = Flg << 2;

	if ((EF->Foot_flg >> no) & 1) {

		pEm->Spd_x = 0;	}else {

		CompM(&Tm, (MATRIX *)pP_ptr[ -1 ].M, &Tm);
		CompM(&Tm, (MATRIX *)pP_ptr->M, &Tm);

		dx = Tm.t[ 0 ] - pP_ptr->Workm.t[ 0 ];
		dz = Tm.t[ 2 ] - pP_ptr->Workm.t[ 2 ];

		pEm->Spd_x = SquareRoot0(dx * dx + dz * dz);
	}	}

static short Em25_goto00(ENEMY_WORK *pEm, short rr)
{
	register PLAYER_WORK *pTg;
	register long x, z;

	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	if (!EF->Place) {
		return Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, rr);
	}else {

		x = pTg->Pos_x - pEm->Pos_x;
		z = pTg->Pos_z - pEm->Pos_z;
		return Goto00_ck(pEm, pEm->Pos_x - x, pEm->Pos_z - z, rr);
	}
}

static unsigned long Ck_lock_wall(ENEMY_WORK *pEm)
{
	register SCA_DATA **ppScd;
	register EM25_FREE *pFp;
	register unsigned short pos;
	register unsigned short dir;	/* inferred name: no retail record */

	ppScd = G.pSca_hit_data;
	pFp = EF;

	while (*ppScd) {
		pos = Sca_get_dir((VECTOR *)&pEm->Pos_x, *ppScd);
		pFp->pWall = *ppScd++;
		pFp->Sc_dir = pos << 10;
		dir = (pEm->Cdir_y + 0x200) & 0xC00;
		if (pFp->Sc_dir == dir && pFp->pWall->Floor >= -pEm->Pos_y / 1800)
			return pos + 1;

	}

	return 0;

}

static void Em25_wall_rot(ENEMY_WORK *pEm, short rot)
{
	register EM25_FREE *pFp;

	pFp = EF;

	if (pFp->Wall_pos & 1) {
		pEm->Cdir_y += rot;
	}else {

		if (!(pFp->Wall_pos & 2))
			rot = -rot;

		pEm->Cdir_x += rot;
	}

	pFp->Sc_dir += rot;

}

static void Em25_add_speed(ENEMY_WORK *pEm, long rx, long ry, long rz)
{
	SVECTOR Sv;
	MATRIX M;

	Sv.vx = pEm->Cdir_x + rx;
	Sv.vy = pEm->Cdir_y + ry;
	Sv.vz = pEm->Cdir_z + rz;
	RotMatrix(&Sv, &M);
	ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
	pEm->Pos_x += Sv.vx;
	pEm->Pos_y += Sv.vy;
	pEm->Pos_z += Sv.vz;

}

static long Ck_wall_edge(ENEMY_WORK *pEm, short Scdir)
{
	register SCA_DATA *pSc;
	register short pos;
	register long d;	/* inferred name: no retail record */

	pSc = EF->pWall;

	pos = EF->Wall_pos;

	if (pos & 1) {
		Scdir += (pos << 10) - 0x400;
		if (Scdir == 0x400) {
			d = Sca_get_pos(2, pSc);
			return pEm->Pos_x < d + 2500;
		}else {

			d = Sca_get_pos(0, pSc);
			return d - 2500 < pEm->Pos_x;
		}
	}else {

		if (Scdir == 0x400) {
			d = Sca_get_pos(3, pSc);
			return d - 2500 < pEm->Pos_z;
		}else {

			d = Sca_get_pos(1, pSc);
			return pEm->Pos_z < d + 2500;
		}
	}

}

static long Ck_near_wall(ENEMY_WORK *pEm, SCA_DATA *pSc, short dir)
{
	register long edge;
	register PLAYER_WORK *pTg;

	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	edge = Sca_get_pos(dir, pSc);
	if (dir & 1) {
		if (abs(pTg->Pos_z - edge) < 3000)
			return 1;
	}else {

		if (abs(pTg->Pos_x - edge) < 3000) return 1;
	}

	return 0;
}

static long Ck_wall_other(ENEMY_WORK *pEm, short r)
{
	register SCA_DATA **ppScd;

	if (!Sca_ck_hit((VECTOR *)&pEm->Pos_x, r, 1 << pEm->nFloor, 0x400)) return 0;
	ppScd = G.pSca_hit_data;

	while (*ppScd) {
		if (*ppScd != EF->pWall) {	if ((*ppScd++)->Floor >= -pEm->Pos_y / 1800)
				return 1;
		}else
			ppScd++;
	}

	return 0;
}

static short Ck_wall_to_pl(ENEMY_WORK *pEm, short rot)
{
	register short xx, yy, zz;
	register PLAYER_WORK *pTg;
	register long d;	/* inferred name: no retail record */

	pTg = (EF->Target_pl) ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	xx = pEm->Cdir_x;
	yy = pEm->Cdir_y;
	zz = pEm->Cdir_z;
	pEm->Cdir_x = 0;
	pEm->Cdir_y = (EF->Wall_pos << 10) & 0xFFF;
	pEm->Cdir_z = 0;

	d = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, rot);

	pEm->Cdir_x = xx;
	pEm->Cdir_y = yy;
	pEm->Cdir_z = zz;

	return d;
}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv, Sv2;
	register EM25_BLOOD_TBL *tbl;
	register long dir, Id;
	MATRIX M;

	tbl = &blood_id_tbl[ type & 0x7F ];

	dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Pos_x, pEm->Pos_z);

	pP_ptr = pEm->pSin_parts_ptr;
	pP_ptr += parts;
	Id = tbl->Id & 0x7F;

	if (tbl->Id & 0x80) {
		Sv.vx = 0;
		Sv.vy = -100;
		Sv.vz = 0;
		Esp_call((Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
	}else {

		Sv.vx = 0;
		Sv.vy = dir;
		Sv.vz = 0;
		RotMatrix(&Sv, &M);
		Sv2.vx = Sv2.vy = Sv2.vz = 0;
		switch (EF->Place) {
			case 1:
				Sv2.vy = 800;
				break;

			case 2:
			case 3:
				Sv2.vx = 400; break;	/* explicit break: required for retail's case layout */
		}

		ApplyMatrixSV(&M, &Sv2, &Sv);
		Sv.vx += pP_ptr->Workm.t[ 0 ];
		Sv.vy += pP_ptr->Workm.t[ 1 ];
		Sv.vz += pP_ptr->Workm.t[ 2 ];
		if (type & 0x80)
			Sv.vy = pEm->Water;

		Esp_call((Id << 24) | (tbl->Type << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
	}
}

static void Set_poison(ENEMY_WORK *pEm, unsigned long Type, short Dir)
{
	register PARTS_W *pP_ptr;
	SVECTOR sv, sv2;
	MATRIX M;

	pP_ptr = pEm->pSin_parts_ptr;
	Sca_get_ground(&pEm->Pos_x, 100, 0x400, 0);	/* retail-only, jal @0x801058DC (result unused) */

	if (!(Type & 0x80)) {
		sv.vx = 1000;
		sv.vy = 0;
		sv.vz = -100;
		Esp_call((Type << 16) | 0x1D001000, Dir, &pP_ptr->Workm, &sv);
	}else {

		sv.vx = pEm->Cdir_x;
		sv.vy = pEm->Cdir_y;
		sv.vz = pEm->Cdir_z;
		RotMatrix(&sv, &M);
		sv2.vx = 1000;
		sv2.vy = 0;
		sv2.vz = -100;
		ApplyMatrixSV(&M, &sv2, &sv);
		sv.vx += pP_ptr->Workm.t[ 0 ];
		sv.vy += pP_ptr->Workm.t[ 1 ];
		sv.vz += pP_ptr->Workm.t[ 2 ];

		Esp_call(((Type & 0x7F) << 16) | 0x1D001000, Dir, &GsIDMATRIX, &sv);
	}

}

static unsigned long atck_ck_main(ENEMY_WORK *pEm, unsigned long pl, long R)
{
	register PLAYER_WORK *pTg;
	register long dist, ck;

	if (pl) {
		pTg = (PLAYER_WORK *)G.pEnemy[1];
		dist = pEm->L_spl & 0x3FFFFFFF;
	}else {

		pTg = &G.Player;
		dist = pEm->L_pl;
	}

	if (dist < R) {
		if (!(pTg->Damage_cnt & 0x80)) {

			ck = pl ? Spl_life_down(pEm, 20, 0) : Pl_life_down(20, 0);

			if (ck != 2) {
				Em25_Pl_to_dmg(pl, Cdir_ck(pEm, pTg));
			}else {

				Em25_Pl_to_die(pl);
			}
			return 1;
		}
	}

	return 0;
}

static unsigned long Em25_atck_ck(ENEMY_WORK *pEm, long R, long H)
{
	if (abs(pEm->Pos_y - G.Player.Pos_y) < H)
		if (atck_ck_main(pEm, 0, R)) {
			Snd_se_enem(1, pEm);
			Pad_vib_set0(2, 0);	/* retail-only vibration, jal @0x80105B44 */
			Pad_vib_set1(8, 200, 0);	/* retail-only vibration, jal @0x80105B54 */
			return 1;
		}

	if (G.Status_flg & 0x10000000)
		if (abs(pEm->Pos_y - G.pEnemy[1]->Pos_y) < H)
			if (atck_ck_main(pEm, 1, R)) {
				Snd_se_enem(1, pEm);
				return 1;
			}

	return 0;
}

static long Foot_break_set(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, base, no;

	if (EF->Dmg_type == 14)
		return 1;

	no = Rnd() & 7;
	if (EF->Foot_cnt >= 3 && !((EF->Foot_flg >> no) & 1)) {
		EF->Foot_flg |= 1 << no;
		EF->Foot_cnt--;

		base = no * 2 + 3;

		for (i = base; i <= base + 1; i++) {
			pP_ptr = &pEm->pSin_parts_ptr[ i ];
			pP_ptr->Be_flg |= 0x4A;
			pP_ptr->Free[ 0 ] = pEm->Cdir_y + 0x800;
			pP_ptr->Free[ 4 ] = 0;
			pP_ptr->Free[ 2 ] = 600;
			pP_ptr->Free[ 1 ] = -120;
			pP_ptr->Free[ 3 ] = 10;
			pP_ptr->Free[ 6 ] = -100;
			pP_ptr->Poly_rgb = 0x2F2F2F;
		}

		Set_blood(pEm, base, 1);
		return 0;
	}

	Set_blood(pEm, 19, 1);
	return 1;
}

static void Em25_Set_child(ENEMY_WORK *pEm, unsigned short type, unsigned char cnt)
{
	register ENEMY_WORK *pWk;
	register unsigned long total;

	if ((G.System_flg & 0x1000000) && (Rnd() & 3))	/* retail-only early exit (0x80105D54..0x80105D7C) */
		return;

	if (EF->Dmg_type == 14)
		return;

	total = Ck_em_id(pEm, 0, 0x26, 0);
	if (total + cnt >= 18)
		if (!(cnt = 17 - total)) return;

	while (cnt-- > 0) {	/* inferred spelling: tests the old count (retail's duplicated top exit test) */
		if (!(pWk = (ENEMY_WORK *)Set_enemy_work(0x26))) return;
		pWk->Type = type | (pEm->Water ? 1 : 0);	/* inferred spelling (retail hoists zero-extended type) */
		pWk->Pos_x = pEm->Pos_x;
		pWk->Pos_y = pEm->Pos_y;
		pWk->Pos_z = pEm->Pos_z;
		if (EF->Place < 2) {
			pWk->Cdir_y = Rnd() << 4;
		}else {

			pWk->Cdir_y = (EF->Wall_pos << 10) - (0x200 - (Rnd() << 2));	/* inferred spelling: retail subtracts 0x200 from Wall_pos<<10 first */
		}
		pWk->nOba = 1;
		pWk->Atd[ 0 ] = Em25_hit_tbl[ 3 ];
		((CODE_PTR)G.Em_move_tbl[ pWk->Id - 0x10 ])(pWk);	/* retail-only: run the child once (jalr @0x80105EE8, G+0x3C74 = Em_move_tbl - 0x40) */

	}
}

static void Em25_Pl_to_dmg(unsigned long pl, unsigned char dir)
{
	register PLAYER_WORK *pPl;

	pPl = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 2;
	pPl->Routine_1 = dir + 2;
	pPl->Routine_2 = 0;
	pPl->Routine_3 = 0;

	pPl->Damage_cnt |= 0x80;
}

static void Em25_Pl_to_die(unsigned long pl)
{
	register PLAYER_WORK *pPl;

	pPl = pl ? (PLAYER_WORK *)G.pEnemy[1] : &G.Player;

	pPl->Routine_0 = 3;
	pPl->Routine_1 = 0;
	pPl->Routine_2 = 0;

	pPl->Routine_3 = 0;	}

static ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
	register ENEMY_WORK *pTg;

	pTg = (ENEMY_WORK *)&G.Player;
	EF->Target_pl = 0;

	if (G.Status_flg & 0x10000000) {
		if ((pEm->L_spl & 0xC0000000) == 0x40000000 && (pEm->L_spl & 0x3FFFFFFF) < pEm->L_pl) {

			pTg = G.pEnemy[1];
			EF->Target_pl = 1;
		}
	}

	return pTg;	}

static void Set_foot_se(ENEMY_WORK *pEm)
{
	if (*pEm->pNow_seq & 0x100000) {
		if (!pEm->Water) {
			Snd_se_enem(foot_se_tbl[ EF->Foot_se ], pEm);
		}else {

			Snd_se_enem(10, pEm);

		}
		EF->Foot_se = (EF->Foot_se + 1) % 3;
	}
}

static void Chg_modelcol(ENEMY_WORK *pEm, unsigned long Rgb)
{
	register PARTS_W *pP_ptr;
	register unsigned long i;

	for (i = 0; i < 20; i++) {
		pP_ptr = &pEm->pSin_parts_ptr[ i ];
		pP_ptr->Poly_rgb = Rgb;
	}
}

static long Ck_land(ENEMY_WORK *pEm, long Fe, long Fg, long Fw)
{
	register EM25_FREE *pFp;	/* inferred name: gcc folds it into the Hitline stores (no retail SYM record) */

	if (pEm->Water < pEm->Ground) {
		if (pEm->Pos_y + Fe < pEm->Water) return 0;
		pEm->Pos_y = pEm->Water + Fw;
		pFp = EF; *(signed char *)&pFp->Hitline = -1;
		return -1;
	}else {

		if (pEm->Pos_y + Fe >= pEm->Ground) {
		pEm->Pos_y = pEm->Ground + Fg;
		pFp = EF; pFp->Hitline = 1;
		return 1; }
	}

	return 0;

}

static void Em25_spark_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;
	register unsigned long i, d0;

	if (!EF->Spark_cnt)
		return;

	if (--EF->Spark_cnt & 1) {
		for (i = 0; i < 3; i++) {
			d0 = biripos_tbl[ Rnd() & 7 ];
			pP_ptr = &pEm->pSin_parts_ptr[ d0 ];
			if ((pP_ptr->Be_flg & 0x4B) == 1)
				Set_blood(pEm, d0, 9);

		}
	}
}

static void Set_gunrange(ENEMY_WORK *pEm)
{
	register PLAYER_WORK *pTg;
	register long dh;

	pTg = &G.Player;

	pEm->Be_flg &= 0xE3FFFFFF;

	switch (EF->Place) {

		case 0:
			pEm->Be_flg |= 0xC000000;	break;

		case 1:
			pEm->Be_flg |= 0x10000000;
			break;

		case 2:
		case 3:
			dh = pTg->Pos_y - pEm->Pos_y;
			if (dh > 4200) {
				pEm->Be_flg |= 0x10000000;
			}else {

				pEm->Be_flg |= 0x18000000;
				if (dh < 2100)
					pEm->Be_flg |= 0x4000000;
			}

	}

}
