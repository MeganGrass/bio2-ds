/* em22_sub.c - reconstruction of D:\Bio2\prog\EM22_SUB.C (enemy 22: licker -
 * wall/ceiling turning, tongue and shadow animation, target search, hit-area
 * placement, blood/spark effects).  Retail EM22.C #includes this file near its
 * top (all of its functions are static, EM22.C's statics are used here and the
 * two files form one object with one SYM preamble and one data block), so
 * em22.c #includes it; compiled on its own (the EM22_SUB.C gate) the guarded
 * block below supplies what em22.c would otherwise provide.  Linked into
 * overlays 0x08 and 0x26 from the same source.
 * statement lines follow the retail SLD by layout. */
#ifndef EM22_C	/* standalone build */
#include "EM22_SUB.H"
#endif

#define EF	((EM22_FREE *)&pEm->Free[1])	/* Em22 work kept in the enemy generic Free[] area; it starts 4 bytes after ENEMY_WORK.Free (retail +0x218) */

/* per axis (Get_axis2) and wall direction: the Add_dir step */
static short Axis_add_tbl[6][4][3] = {
	{ { 0, 0, -1024 }, { -1024, 0, 0 }, { 0, 0, 1024 }, { 1024, 0, 0 } },
	{ { 0, 0, 1024 }, { 1024, 0, 0 }, { 0, 0, -1024 }, { -1024, 0, 0 } },
	{ { 0, -1024, 0 }, { 0, 0, -1024 }, { 0, 1024, 0 }, { 0, 0, 1024 } },
	{ { 0, 1024, 0 }, { 0, 0, 1024 }, { 0, -1024, 0 }, { 0, 0, -1024 } },
	{ { -1024, 0, 0 }, { 0, -1024, 0 }, { 1024, 0, 0 }, { 0, 1024, 0 } },
	{ { 1024, 0, 0 }, { 0, 1024, 0 }, { -1024, 0, 0 }, { 0, -1024, 0 } }
};

static unsigned long Wall_turn_ck(ENEMY_WORK *pEm, SVECTOR *pSv)
{	VECTOR Vec;	SVECTOR Svec;	register short Gnd;

	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, pSv, &Svec);
	Vec.vx = pEm->Pos_x + Svec.vx;
	Vec.vy = pEm->Pos_y + Svec.vy;
	Vec.vz = pEm->Pos_z + Svec.vz;

	if (!Get_axis((MATRIX *)pEm->pSuper)) {

		{	Sca_ck_hit(&Vec, 10, 1 << pEm->nFloor, 0x200);
			return G.pSca_hit_data[0] != 0;
		}
	}else {
		{	G.pSca_hit_data[0] = 0;
			Gnd = Sca_get_ground(&Vec, 0, 0x600, 0);

			if (Ground_in_flg) Gnd = Ground_in_pos;
			if (Gnd < Vec.vy) {

				EF->Dest_y = Gnd;	return 1;
			}

			if (Ceiling_in_pos[0] > Ceiling) Ceiling = Ceiling_in_pos[0];
			if (Vec.vy < Ceiling) {
				EF->Dest_y = Ceiling;
				return 1;
			}
		}
	}	return 0;
}
static void Wall_turn_init(ENEMY_WORK *pEm)
{	register short Dir;

	SVECTOR Add_vec = { 0x8A2, 0, 0, 0 };	VECTOR Vec;

	if (G.pSca_hit_data[0]) {

		EF->pSca_dat = G.pSca_hit_data[0];

		Vec.vx = pEm->Pos_x;
		Vec.vy = pEm->Pos_y;
		Vec.vz = pEm->Pos_z;
		Sca_get_pos_super(&pEm->Pos_x, EF->pSca_dat, pEm->pSuper, &Vec);

		Dir = Sca_get_dir_super(&pEm->Pos_x, EF->pSca_dat, pEm->pSuper, 0);
		Rot_vector(Dir << 10, &Add_vec, &Add_vec);
		EF->Dest_x = Add_vec.vx + Vec.vx;
		EF->Dest_z = Add_vec.vz + Vec.vz;
		EF->Attribute &= 0xFFEF;
		EF->Attribute |= 0x20;

	}else {

		{	EF->Dest_x = pEm->Pos_x;
			EF->Dest_z = pEm->Pos_z;

			switch (Get_axis((MATRIX *)pEm->pSuper)) {
			case 1:
				if (EF->Dest_y > pEm->Pos_y) {
					Dir = 1;

					EF->Dest_y -= 0x898;	}else {

					Dir = 3;	EF->Dest_y += 0x898;	}	break;

			case 2:	if (EF->Dest_y > pEm->Pos_y) {
					Dir = 2;

					EF->Dest_y -= 0x898;	}else {
					Dir = 0;
					EF->Dest_y += 0x898;	}
			}

			EF->Attribute &= 0xFFDF;
			EF->Attribute |= 0x10;

		}
	}	EF->Wall_dir_new = Dir ^ 2;
	pEm->Be_flg |= 8;
}
static void Wall_turning(ENEMY_WORK *pEm)
{	VECTOR Vec;

	Vec.vx = EF->Dest_x;
	Vec.vz = EF->Dest_z;

	if (!(EF->Attribute & 0x10)) {
		{	if (Approach(&pEm->Pos_x, &Vec, 0x30))
				EF->Attribute |= 0x10;
		}	}
	if (!(EF->Attribute & 0x20)) {
		{	if (Approach_one(&pEm->Pos_y, EF->Dest_y, 0x30))
				EF->Attribute |= 0x10;
		}	}

	Goto01_super(pEm, EF->Wall_dir_new << 10, 0x20);

}
static void Wall_turn_end(ENEMY_WORK *pEm)
{
	SVECTOR Svec = { 0x898, 0xF963, 0, 0 };	long Axis;

	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Svec, &Svec);
	pEm->Pos_x = EF->Dest_x + Svec.vx;
	pEm->Pos_y += Svec.vy;
	pEm->Pos_z = EF->Dest_z + Svec.vz;

	Axis = Get_axis2((MATRIX *)pEm->pSuper);
	Add_dir.vx = Axis_add_tbl[Axis][EF->Wall_dir_new][0];
	Add_dir.vy = Axis_add_tbl[Axis][EF->Wall_dir_new][1];
	Add_dir.vz = Axis_add_tbl[Axis][EF->Wall_dir_new][2];
	pEm->Be_flg &= ~8;

}
static long Em22_wall_hosei(ENEMY_WORK *pEm, SVECTOR *pFront_pos)
{	register SCA_DATA **ppSca_data;	SVECTOR Ck_pos;	VECTOR Ck_posV;	register unsigned char Wall_dir;	register unsigned long Floor, Hit_flg;

	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, pFront_pos, &Ck_pos);

	pEm->Timer2 = ((Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00) >> 10;

	Ck_posV.vx = Ck_pos.vx + pEm->Pos_x;
	Ck_posV.vy = Ck_pos.vy;
	Ck_posV.vz = Ck_pos.vz + pEm->Pos_z;

	Floor = 1 << (pEm->Pos_y / -1800);

	Sca_ck_hit(&Ck_posV, 10, Floor, 0x200);

	ppSca_data = G.pSca_hit_data;
	Hit_flg = 0x80000000;

	if (!*ppSca_data) return 0x40000000;

	Sca_ck_hit(&Ck_posV, 10, Floor, 0x600);

	do {	if ((*ppSca_data)->Id & 0x200) {	{	/* [INFERRED] inner compound: retail SYM block (Sca_get_dir implicit decl) ends at line 270, before the continue */

			Wall_dir = Sca_get_dir(&Ck_posV, *ppSca_data);

			if (*ppSca_data != EF->pSca_dat) {
				EF->pSca_dat = *ppSca_data;

				if ((Wall_dir ^ (unsigned char)pEm->Timer3) & 1) {
					EF->pSca_dat = *ppSca_data;
					return Wall_dir;
				}

			}else {

				if ((unsigned char)pEm->Timer3 & 1) {
					{	pEm->Pos_z = Sca_get_pos((unsigned char)pEm->Timer3, *ppSca_data);	}

				}else {
					{	pEm->Pos_x = Sca_get_pos((unsigned char)pEm->Timer3, *ppSca_data);	}	}	}	}

		}else

			Hit_flg |= 0x20000000;

		ppSca_data++;	}while (*ppSca_data);

	return Hit_flg;	}
static unsigned long Em22_tongue_move(ENEMY_WORK *pEm)
{	register PARTS_W *pP_ptr;

	pP_ptr = &pEm->pSin_parts_ptr[22];

	switch (EF->Tongue_Rno) {	/* retail: two new leading steps (delay), old cases 1..4 become 3..6, 5/6 removed */
	case 0:	break;
	case 1:
		EF->Retail_28 = 0;
		EF->Tongue_Rno++;
	case 2:
		if (++EF->Retail_28 == 0x11) EF->Tongue_Rno++;
		break;
	case 3:

		pP_ptr->Be_flg = 1;
		((short *)pP_ptr->pSa_dat_head)[6] += 0x800;
		Sa_parts_allmv(pEm, pP_ptr);
		if (((short *)pP_ptr->pSa_dat_head)[6] >= 0x1000)

			EF->Tongue_Rno++;	break;
	case 4:
		((short *)pP_ptr->pSa_dat_head)[8] += 0x400;
		Sa_parts_allmv(pEm, pP_ptr);
		if (((short *)pP_ptr->pSa_dat_head)[8] >= 0x1000) {
			EF->Tongue_Rno++;
			EF->Retail_28 = 4;	/* retail: was pEm->Timer1 */
		}
		break;
	case 5:
		((short *)pP_ptr->pSa_dat_head)[EF->Retail_28 * 2 + 2] += 0x1000;
		Sa_parts_allmv(pEm, pP_ptr);
		if (((short *)pP_ptr->pSa_dat_head)[EF->Retail_28 * 2 + 2] >= 0x1000) {
			EF->Retail_28++;
			if (EF->Retail_28 == 11) {
				EF->Tongue_Rno++;
				((short *)pP_ptr->pSa_dat_head)[6] = 0;
				((short *)pP_ptr->pSa_dat_head)[8] = 0;
				((short *)pP_ptr->pSa_dat_head)[10] = 0;
				((short *)pP_ptr->pSa_dat_head)[12] = 0;
				((short *)pP_ptr->pSa_dat_head)[14] = 0;
				((short *)pP_ptr->pSa_dat_head)[16] = 0;
				((short *)pP_ptr->pSa_dat_head)[18] = 0;
				((short *)pP_ptr->pSa_dat_head)[20] = 0;
			}
		}
		break;
	case 6:
		((short *)pP_ptr->pSa_dat_head)[22] -= 0x1000;
		Sa_parts_allmv(pEm, pP_ptr);
		if (((short *)pP_ptr->pSa_dat_head)[22] <= 0) {
			EF->Tongue_Rno = 0;
			Tongue_flg = 0;
			pP_ptr->Be_flg = 0;
		}
		break;
	}

	return 0;
}
static void Shadow_move(ENEMY_WORK *pEm)
{	register long Kage_mag;

	register KAGE_WORK *pKage = (KAGE_WORK *)pEm->pKage_work;
	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	SVECTOR Svec = { 0, 0, 0, 0 };
	switch (EF->Kage_Rno) {
	case 0:

		Kage_mag = 9000 - (pEm->Ground - pParts->Workm.t[1]);
		if (Kage_mag < 0) Kage_mag = 0;
		pKage->Half_x = EF->Kage_x * Kage_mag / 9000;
		pKage->Half_z = EF->Kage_z * Kage_mag / 9000;
		break;

	case 1:

		pKage->pPos = (VECTOR *)pParts[2].Workm.t;

		pEm->Be_flg |= 0xA;
		pEm->Timer1 = 0;
		EF->Kage_Rno++;

	case 2:
		if (pEm->Timer1 < 0x70) {
			pEm->Timer1++;

			if (pEm->Timer1 < 0x10) {
				*(unsigned long *)&pKage->Prim[0].r0 += 0x40400;
				*(unsigned long *)&pKage->Prim[1].r0 += 0x40400;
				*(unsigned long *)&pKage->Prim[0].r0 -= 7;
				*(unsigned long *)&pKage->Prim[1].r0 -= 7;
			}
			pKage->Half_x += 4;
			pKage->Half_z += 4;
		}
	}
	if (pEm->Life > 0 && !(Rnd() & 0x1F))
		Esp_call(0x150800, 0, &pParts[9].Workm, &Svec);
}
static ENEMY_WORK *Em22_search_lv0(ENEMY_WORK *pEm)
{
	if (G.Player.Life < 0) return 0;

	if (pEm->Routine_0 == 7) {

		EF->Sence_lv = 0;	return 0;
	}

	if (EF->Attribute & 1) return (ENEMY_WORK *)&G.Player;

	if (pEm->At_em_no < 2) {
		EF->Sence_lv = 1;
		return G.pEnemy[pEm->At_em_no];
	}

	switch (Branch(pEm->L_pl, 3000, 9000, 9100)) {
	case 0:

		if (*(unsigned long *)&G.Room_enemy_flg & 0x170000) {

			EF->Sence_lv = 1;	return (ENEMY_WORK *)&G.Player;
		}
		break;

	case 1:

		if (G.Room_player_flg & 5) {

			EF->Sence_lv = 1;	return (ENEMY_WORK *)&G.Player;
		}
		break;

	case 2:
	case 3:

		if (G.Room_player_flg & 5) {
			EF->Sence_lv = 1;
			return (ENEMY_WORK *)&G.Player;
		}
		break;
	}

	return 0;
}
static ENEMY_WORK *Em22_search_lv1(ENEMY_WORK *pEm)
{
	if (G.Player.Life < 0) return 0;

	if (pEm->Routine_0 == 7) {

		EF->Sence_lv = 0;	return 0;
	}

	if (EF->Attribute & 1) {
		EF->Sence_lv = 1;	return (ENEMY_WORK *)&G.Player;
	}

	if (pEm->At_em_no < 2) {
		EF->Sence_lv = 2;
		return G.pEnemy[pEm->At_em_no];
	}

	switch (Branch(pEm->L_pl, 3000, 5000, 7000)) {
	case 0:
		if (G.Player.Routine_1) {

			EF->Sence_lv = 2;	return (ENEMY_WORK *)&G.Player;
		}
		break;

	case 1:

		if (*(unsigned long *)&G.Room_enemy_flg & 0x170000) {
			EF->Sence_lv = 2;
			return (ENEMY_WORK *)&G.Player;
		}
		break;

	case 2:
	case 3:

		if (G.Room_player_flg & 5) {
			EF->Sence_lv = 2;
			return (ENEMY_WORK *)&G.Player;
		}
		break;
	}

	return 0;
}
static void Em22_Atd_set_1(ENEMY_WORK *pEm)
{
	pEm->nOba = 2;

	pEm->Atd[0].Ofs_x = 0x2A0;	pEm->Eff_at_r = 0x2BC;	/* retail: was 0x250 beside At_w */
	pEm->Atd[0].Ofs_y = 0;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = 0x250;

	pEm->Atd[0].At_h = 0x258;
	pEm->Atd[0].At_d = 0x250;
	pEm->Atd[0].D = 1000;
	pEm->Atd[0].W = 1000;

	pEm->Atd[1].Ofs_x = -0x60;
	pEm->Atd[1].Ofs_y = -0x180;
	pEm->Atd[1].Ofs_z = 0;
	pEm->Atd[1].W = 0x180;	pEm->Atd[1].At_w = 0x180;
	pEm->Atd[1].At_h = 0x180;
	pEm->Atd[1].D = 0x180;	pEm->Atd[1].At_d = 0x180;
	}	/* retail: Be_flg &= ~8 moved to the callers */
static void Em22_Atd_set_2(ENEMY_WORK *pEm)
{
	pEm->nOba = 3;

	pEm->Atd[0].Ofs_x = 0x2A0;
	pEm->Atd[0].Ofs_y = -200;	pEm->Eff_at_r = 0x1E4;	/* retail: was 0x180 beside At_w */
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].W = 0x180;	pEm->Atd[0].At_w = 0x180;

	pEm->Atd[0].At_h = 0x258;
	pEm->Atd[0].D = 0x180;	pEm->Atd[0].At_d = 0x180;

	pEm->Atd[1].Ofs_x = -0x60;
	pEm->Atd[1].Ofs_y = -0x180;
	pEm->Atd[1].Ofs_z = 0;
	pEm->Atd[1].W = 0x180;	pEm->Atd[1].At_w = 0x180;
	pEm->Atd[1].At_h = 0x180;
	pEm->Atd[1].D = 0x180;	pEm->Atd[1].At_d = 0x180;

	pEm->Atd[2].Ofs_x = 0x400;
	pEm->Atd[2].Ofs_y = 0x70;
	pEm->Atd[2].Ofs_z = 0;
	pEm->Atd[2].W = 0x380;	pEm->Atd[2].At_w = 0x380;
	pEm->Atd[2].At_h = 0x180;
	pEm->Atd[2].D = 0x380;	pEm->Atd[2].At_d = 0x380;	}
static void Em22_Foot_set(ENEMY_WORK *pEm, unsigned long Foot_no)
{	register PARTS_W *pP_ptr;	MATRIX M;

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &M);

	pP_ptr += Foot_no * 4 + 16;
	CompM(&M, (MATRIX *)pP_ptr[-2].M, &M);
	CompM(&M, (MATRIX *)pP_ptr[-1].M, &M);
	CompM(&M, (MATRIX *)pP_ptr->M, &M);

	pEm->Pos_x -= M.t[0] - pP_ptr->Workm.t[0];
	pEm->Pos_z -= M.t[2] - pP_ptr->Workm.t[2];
}
/* hit points for the blood effect: offset from parts Parts_no (Wep_blood_tbl Ofs_f/Ofs_b select the row) */
static BLOOD_DATA Blood_point_tbl[24] = {
	{ -300, 0, 0, 7 },
	{ 0, -200, 0, 4 },
	{ 0, 200, 200, 6 },
	{ 0, -300, 0, 8 },
	{ -300, 0, 0, 10 },
	{ 300, 100, 0, 11 },
	{ 0, 200, 200, 6 },
	{ 0, -300, 0, 13 },
	{ 300, -100, 0, 5 },
	{ -200, -600, 0, 6 },
	{ 300, 400, 0, 12 },
	{ 0, 0, -100, 13 },
	{ 0, 0, -400, 0 },
	{ -400, 0, -300, 0 },
	{ -300, 0, 200, 2 },
	{ -300, 0, -200, 2 },
	{ -500, -300, 200, 2 },
	{ -200, -600, -400, 2 },
	{ -200, 600, -200, 14 },
	{ 0, 600, 200, 18 },
	{ 0, 0, 0, 16 },
	{ 0, 0, 0, 20 },
	{ -400, 200, 0, 18 },
	{ -100, 400, -200, 18 }
};
static int Blood_move(ENEMY_WORK *pEm, WEP_BLOOD *pWb, unsigned long Front_flg)
{
	register unsigned char Bp_no;
	register PARTS_W *pParts = (PARTS_W *)pEm->pSin_parts_ptr;
	register BLOOD_DATA *pBd;
	register unsigned long No, Range, Ofs;

	No = pWb->Num;
	Range = pWb->Range;
	if (Front_flg)
		Ofs = pWb->Ofs_b;
	else
		Ofs = pWb->Ofs_f;

	while (No) {

		No--;	Bp_no = Rnd() & (Range - 1);
		pBd = &Blood_point_tbl[ Ofs + Bp_no ];

		Esp_call((pWb->Id << 24) | (pWb->Type << 16) | pWb->Zoom, 0, &pParts[ pBd->Parts_no ].Workm, pBd);

	}

	EF->Dm_eff_rtn = 0;
	switch (EF->Wep_no) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 12:
	case 13:
	case 15:
	case 18:
		break;
	case 11:	if ((unsigned char)pParts->Poly_rgb != 0x48)
			Color_chg(pEm, 0x16, 0x58, 0x58, 0x58);
		break;

	case 14:	Dm_eff_spark(pEm);

	case 10: case 16: case 17: case 19:
		Color_chg(pEm, 0x16, 0x48, 0x48, 0x48);
		break;

	}
}
/* spark points (parts + offset); Next_flg bit 0 = another point follows in this frame, bit 1 = skip */
static DM_ESP2 Spark_tbl[16] = {
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 100, 100, 0, 6, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -100, 0, -100, 0, 15, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -200, 0, 200, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -300, -600, -200, 0, 2, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 20, 0 }
};
static void Dm_eff_spark(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts = (PARTS_W *)pEm->pSin_parts_ptr;
	register MATRIX *pMat;
	static DM_ESP2 *pEsp2;
	__complex__ unsigned long next_flag;	/* [INFERRED] consumed next-entry flag; compiler-deleted carrier leaves no SYM local. */

	if (EF->Dm_eff_rtn == 0) {

		pEsp2 = Spark_tbl;
		EF->Dm_eff_func = Dm_eff_spark;
		EF->Dm_eff_rtn++;
		return;
	}

	do {
		if (!(pEsp2->Next_flg & 2)) {
			pMat = &pParts[ pEsp2->Parts ].Workm;
			Esp_call(0x6000000 | (Rnd() * 2 + 0x1000), 0, pMat, pEsp2);
		}
		__real__ next_flag = pEsp2->Next_flg & 1;

		pEsp2++;
	}while (__real__ next_flag);
	if (EF->Dm_eff_rtn == 16) EF->Dm_eff_func = 0;
	EF->Dm_eff_rtn++;

}
static void Gunarea_chg(ENEMY_WORK *pEm, unsigned long Id)
{
	switch (Id) {
	case 0:
		pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x10000000;
		break;
	case 1:
		pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x18000000;
		break;
	case 2:
		pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x08000000;
		break;
	case 3:
		pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x0C000000;
		break;
	case 4:
		if (G.Player.Type == 7 || G.Player.Type == 8)
			pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x0C000000;
		else
			pEm->Be_flg = (pEm->Be_flg & 0xE3FFFFFF) | 0x04000000;
		break;
	case 5:
		pEm->Be_flg &= 0xE3FFFFFF;
		break;
	}	}
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b)
{
	register PARTS_W *pParts = (PARTS_W *)pEm->pSin_parts_ptr;
	register unsigned long i;

	for (i = 0; i < Num; i++)
		pParts[i].Poly_rgb = (pParts[i].Poly_rgb & 0xFF000000) | (b << 16 | g << 8 | r);

}
