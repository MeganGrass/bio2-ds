/* em24.c - retail-only enemy 24 (Dual Shock USA; no Beta 2 counterpart, original file name
 * unknown). A cut-down variant of the licker EM22.C: the source is the retail em22.c /
 * em22_sub.c reconstruction in one file, minus the moves/damage/death states EM24 lacks
 * (their routine-table entries are 0) and with EM24's own changes, each commented "EM24".
 * Linked into both enemy slots: em24_cdemd0_s1 (0x80100000, OVL_EM24_S1, entry names
 * Em124/Em124_Pldm) and em24_cdemd0_s0 (0x8010D000, OVL_EM24_S0, Em224/Em224_Pldm).
 * Function names: Em24_<role> where the EM22 twin gives the role, Beta 2 names for the
 * shared helpers (Wall_turn_ck .., Set_esp_w/Set_esp_s/Get_dir_pl from EM2B_SUB.C), all
 * [INFERRED]; func_<s1 address> otherwise. Order of definitions = retail text order.
 * Comments saying "retail:" describe the retail EM22 change the code shares. */
#include "EM24.H"

/* ---- objects defined in other translation units ---- */
extern MATRIX GsIDMATRIX;

/* ---- objects of this file ---- */
static SVECTOR Add_dir;
static unsigned long Tongue_flg;
static void (*Em24_routine_0[4])();
static void (*Em24_mv[24])();
static void (*Em24_dm[7])();
static void (*Em24_die[6])();
static void (*Em24_pldm_tbl[])();

/* EM24 data (retail .data from s1 0x80105944, names unknown): two life blocks of the EM22 shape
 * (Em23 pair, two 16-entry tables, Em2a pair of 10, Em2b pair); Em24_Init picks one table by
 * G.System_flg bits 0x40 / 0x20 */
static unsigned short D_80105944 = 300;
static unsigned short D_80105946 = 300;
static unsigned short D_80105948[16] = { 130, 90, 96, 116, 125, 102, 114, 125, 116, 114, 96, 125, 102, 125, 130, 90 };
static unsigned short D_80105968[16] = { 80, 80, 85, 85, 90, 84, 77, 105, 95, 90, 97, 112, 104, 116, 105, 115 };
static unsigned short D_80105988[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short D_8010599C[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short D_801059B0 = 250;
static unsigned short D_801059B2 = 200;
static unsigned short D_801059B4 = 240;
static unsigned short D_801059B6 = 240;
static unsigned short D_801059B8[16] = { 140, 100, 106, 126, 135, 112, 124, 126, 126, 124, 106, 126, 112, 135, 140, 100 };
static unsigned short D_801059D8[16] = { 90, 90, 95, 95, 100, 94, 87, 115, 105, 100, 107, 122, 114, 126, 115, 125 };
static unsigned short D_801059F8[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short D_80105A0C[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short D_80105A20 = 250;
static unsigned short D_80105A22 = 200;
static unsigned char Normal_type_set[4] = { 0, 0x17, 0xB, 0x17 };	/* 0x80105A24: EM24 has no Scenario_type_set */

#define EF	((EM24_FREE *)((char *)pEm->Free + 4))	/* enemy work kept in the generic Free[] area (retail Free[] is at +0x218, 4 bytes behind the project member) */

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
static long Em24_wall_hosei(ENEMY_WORK *pEm, SVECTOR *pFront_pos)
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
static unsigned long Em24_tongue_move(ENEMY_WORK *pEm)
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
static ENEMY_WORK *Em24_search_lv0(ENEMY_WORK *pEm)
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
static ENEMY_WORK *Em24_search_lv1(ENEMY_WORK *pEm)
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
static void Em24_Atd_set_1(ENEMY_WORK *pEm)
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
static void Em24_Atd_set_2(ENEMY_WORK *pEm)
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
static void Em24_Foot_set(ENEMY_WORK *pEm, unsigned long Foot_no)
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

static unsigned short Tongue_hit_l[9] = { 2500, 3500, 4600, 4500, 4200, 3500, 3000, 2500, 2000 };	/* 0x80105C38 */
static WEP_BLOOD Wep_blood_tbl[20];

static unsigned long Em24_Wall_escape(ENEMY_WORK *pEm, short Spd, unsigned long Flg)
{
	SVECTOR Ck_pos = { 1800, 0, 0, 0 };
	VECTOR Vec;

	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);

	Vec.vx = pEm->Pos_x + Ck_pos.vx;
	Vec.vz = pEm->Pos_z + Ck_pos.vz;
	Sca_ck_hit(&Vec, 400, 1 << pEm->nFloor, 0x400);

	if (G.pSca_hit_data[0]) {
		register long Dir, Cdir;

		if (EF->Wall_at == 0) {

			Dir = Sca_get_dir((VECTOR *)&pEm->Pos_x, G.pSca_hit_data[0]);
			Dir = ((Dir << 10) + 0x800) & 0xFFF;
			Cdir = (pEm->Cdir_y & 0xFFF) - Dir;
			if (abs(Cdir) > 0x800) Cdir -= 0x1000;
			if (Cdir) {
				EF->Wall_at = Spd;
			}else
				EF->Wall_at = -Spd;

			if (Flg) EF->Wall_at = -EF->Wall_at;
		}
		pEm->Cdir_y += (signed char)EF->Wall_at;
		return 1;
	}
	EF->Wall_at = 0;
	return 0;
}
/* retail-only (retail s1 0x80101704 / s0 0x8010E704, 12 words; no Beta twin, name unknown):
 * sets bit 0x8000 in Be_flg of all 22 parts. Called once from Em24_Die when the death
 * sequence starts. */
static void func_80101704(ENEMY_WORK *pEm)
{
	register unsigned long i = 0;
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	for (; i < 22; i++, pParts++)
		pParts->Be_flg |= 0x8000;
}
static void Em24_no_br(ENEMY_WORK *pEm) {}
static void Em24_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	EF->Gun_area = 4;

	if (EF->pTarget = Em24_search_lv0(pEm)) {

		switch (EF->Sence_lv) {
		case 1:
			*(unsigned long *)&pEm->Routine_0 = 0xE01;
			break;

		case 2:

			*(unsigned long *)&pEm->Routine_0 = 0xF01;
			return;
		}
	}else
		switch (pEm->Routine_2) {
		case 0:
		case 3:
			EF->Wall_at = 0;
			*(unsigned long *)&pEm->Move_no = 0x2F0003;
			EF->Attribute |= 2;
			pEm->Routine_3++;
			if (pEm->Routine_3 >= 2) {
				*(unsigned short *)&pEm->Routine_2 = 4;
				goto walk;
			}
			pEm->Routine_2 = 1;
		case 1:
			Spd_get(pEm, Kan, Seq, 0);
			if (Joint_move2(pEm, Kan, Seq, 0x55)) pEm->Routine_2 = 3;
			Add_speedXZ(pEm, 0);
			break;

		case 2:
			pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x10400);
			break;
		walk:
			*(unsigned long *)&pEm->Move_no = 0x1F0001;

		case 4:
			Em24_Wall_escape(pEm, 0xC, 0);

			Spd_get(pEm, Kan, Seq, 0);
			if (Joint_move2(pEm, Kan, Seq, 0x80)) {
				if (!(Rnd() & 3)) *(unsigned short *)&pEm->Routine_2 = 0;
			}
			Add_speedXYZsuper(pEm, 0, 0);
		}
}
static void Em24_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0002;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		pEm->Routine_2 = 1;
	case 1:
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x60);
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Spd_x /= 4;
		pEm->Spd_z /= 4;
		pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x100);
		Add_speedXYZsuper(pEm, 0, 0);
		break;

	case 2:
		*(unsigned long *)&pEm->Move_no = 0xF0004;
		*(unsigned short *)&pEm->Routine_2 = 3;
	case 3:
		pEm->Routine_3++;
		Joint_move2(pEm, Kan, Seq, 0x100);
		if (pEm->Routine_3 >= 5)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
static void Em24_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0006;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:
		if (EF->pTarget) {
			Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x20);

		}
		if (Joint_move2(pEm, Kan, Seq, 0x10100)) {
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			*(unsigned long *)&pEm->Routine_0 = 0x501;
		}
	}
}
static void Em24_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x100))
			*(unsigned long *)&pEm->Routine_0 = 1;

	}
}
static void Em24_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Add_dir_y;

	switch (pEm->Routine_2) {
	case 0:
		if (!EF->pTarget) {
			*(unsigned long *)&pEm->Move_no = 0xF0004;
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			pEm->Routine_2 = 1;
		}else {
			*(unsigned long *)&pEm->Move_no = 0xF0002;
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			pEm->Routine_2 = 2;
			goto rot;
		}

	case 1:
		EF->pTarget = Em24_search_lv0(pEm);
		pEm->Routine_3++;

		Joint_move2(pEm, Kan, Seq, 0x100);

		if (EF->pTarget) {
			*(unsigned long *)&pEm->Move_no = 0xF0002;
			pEm->Routine_2 = 2;
		}else {

			if (pEm->Routine_3 >= 0x79) {
				*(unsigned long *)&pEm->Routine_0 = 0x401;
				*(unsigned long *)&pEm->Move_no = 0xF0002;
			}

			break;
		}
	case 2:
	rot:
		if (!Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x200)) {

			if (SquareRoot0((EF->pTarget->Pos_x - pEm->Pos_x) * (EF->pTarget->Pos_x - pEm->Pos_x) + (EF->pTarget->Pos_z - pEm->Pos_z) * (EF->pTarget->Pos_z - pEm->Pos_z)) < 4000) {
				Add_dir_y = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);

			}else {
				Add_dir_y = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);

				pEm->Routine_3++;
			}
			pEm->Cdir_y += Add_dir_y;
			pEm->Spd_x = -40;
			pEm->Spd_z = 0;
			Add_speedXZ(pEm, 0);

		}else {

			*(unsigned long *)&pEm->Routine_0 = 0x1301;
		}

		Joint_move2(pEm, Kan, Seq, 0x100);

		if (!Add_dir_y)
			*(unsigned long *)&pEm->Routine_0 = 0x201;

		if (pEm->Routine_3 > 0x20)
			*(unsigned long *)&pEm->Routine_0 = 0x501;

	}
}
static void Em24_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0001;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:

		if (!EF->At_cnt && pEm->L_pl < 2000) {
			if (Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x300))

				*(unsigned long *)&pEm->Routine_0 = 0x201;

			else
				*(unsigned long *)&pEm->Routine_0 = 0x901;	break;
		}

		/* EM24: no long-range choice (EM22 retail picks 0xA01/0x1401/0x801 here) */

		Spd_get(pEm, Kan, Seq, 1);

		Add_speedXZ(pEm, 0);

		Joint_move2(pEm, Kan, Seq, 0x10100);
		if (pEm->Move_cnt == 0x2E)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
static void Em24_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Hit;

	SVECTOR Tmp_Svec = { 200, 0, 0, 0 };
	register PARTS_W *pParts = &pEm->pSin_parts_ptr[9];

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_3 = 0;
		if (!(EF->Attribute & 2))
			pEm->Routine_2++;
		else {
			Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);
			pEm->Routine_2 += Mot_play2(pEm, 6, 0xF, 1);
			break;
		}
	case 1:
		if (Tongue_flg) {	*(unsigned long *)&pEm->Routine_0 = 0xF01;	break;	}
		Tongue_flg = pEm->Work_no;
		EF->Tongue_Rno = 1;	/* retail: tongue started here; Wall_dir_new = 0 dropped (the hit flag is Timer0 now) */
		*(unsigned long *)&pEm->Move_no = 0x30007;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		pEm->Timer0 = 0;
		*(unsigned short *)&pEm->Routine_2 = 2;

	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 8);
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			pEm->Routine_2++;

		Add_speedXZ(pEm, 0);
		if (pEm->Move_cnt == 0x11) {
			Esp_call(0x141800, 0, &pParts->Workm, &Tmp_Svec);
			Snd_se_enem(5, pEm);
		}
		break;
	case 3:
		if (!EF->Tongue_Rno) *(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

	pEm->Routine_3++;
	if (pEm->Routine_3 >= 18 && pEm->Routine_3 < 26 && !pEm->Timer0) {	/* retail: was !EF->Wall_dir_new */

		if (!EF->pTarget->Damage_cnt) {
			Hit = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x100);

			if (!Hit && pEm->L_pl < Tongue_hit_l[ pEm->Routine_3 - 17 ]) {

				EF->pTarget->pEnemy_ptr = pEm;
				*(unsigned long *)&EF->pTarget->Routine_0 = 2;
				EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);

				Pad_vib_set0(3, 0);	/* retail: pad vibration */
				Pad_vib_fade_set(4, 0xFF, 0x32, 2);
				Pad_vib_fade_set(0xA, 0x8C, 0x32, 2);
				Hit = Pl_life_down(25, 0);

				if (Hit & 2)
					*(unsigned long *)&G.Player.Routine_0 = 3;

				EF->At_cnt = 40;
				pEm->Timer0 = 1;	/* retail: was EF->Wall_dir_new = 1 */
			}
		}
	}
}
static void Em24_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Hit;

	register PARTS_W *pParts = &pEm->pSin_parts_ptr[13];

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_3 = 0;
		if (EF->Attribute & 2) {
			pEm->Routine_2++;
		}else {
			pEm->Routine_2 += Mot_play2(pEm, 6, 0xF, 0);
			break;
		}
	case 1:
		*(unsigned long *)&pEm->Move_no = 0x3000F;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
		pEm->Timer0 = 0;
	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 8);
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		Add_speedXZ(pEm, 0);
		pEm->Routine_3++;

		if (!EF->pTarget->Damage_cnt && !pEm->Timer0)
		if (pEm->Routine_3 >= 2 && pEm->Routine_3 < 7) {
			Hit = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x154);
			if (!Hit && pEm->L_pl < 2800 && EF->pTarget->Life >= 0) {	/* retail: target must be alive */

				SVECTOR Bpos;

				EF->pTarget->pEnemy_ptr = pEm;
				*(unsigned long *)&EF->pTarget->Routine_0 = 2;
				EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);

				Bpos.vx = pParts->Workm.t[0];
				Bpos.vy = pParts->Workm.t[1] + 200;
				Bpos.vz = pParts->Workm.t[2];
				Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);
				Pad_vib_set0(5, 0);	/* retail: pad vibration */
				Pad_vib_fade_set(0xB, 200, 0x28, 3);

				Hit = Pl_life_down(15, 0);

				if (Hit & 2)
					*(unsigned long *)&G.Player.Routine_0 = 3;

				EF->At_cnt = 40;

				pEm->Timer0 = 1;
			}
		}

	}
}
static void Em24_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x30506;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned short *)&pEm->Routine_2 = 1;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x400)) {
			*(unsigned long *)&pEm->Move_no = 0;
			*(unsigned short *)&pEm->Routine_2 = 2;
		}
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);
		break;
	case 2:
		if (pEm->L_pl < 2500)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {

			if (pEm->L_pl > 8000) {
				EF->Sence_lv = 1;
				*(unsigned long *)&pEm->Routine_0 = 1;
				break;
			}

			if (pEm->L_pl < 2000) {

				*(unsigned long *)&pEm->Routine_0 = 0xF01;

			}else {
				if (!(Rnd() & 3))

					*(unsigned long *)&pEm->Move_no = 0;
				*(unsigned short *)&pEm->Routine_2 = 2;

			}
		}
		Add_speedXYZsuper(pEm, 0, 0);
		break;
	case 3:
		pEm->Routine_3++;
		if (pEm->Routine_3 >= 6) {
			*(unsigned long *)&pEm->Move_no = 0;
			*(unsigned short *)&pEm->Routine_2 = 2;
		}

	}
}
static void Em24_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0004;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		*(unsigned short *)&pEm->Routine_2 = 1;
		pEm->Timer0 = 0;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Move_no = 0x30004;
			pEm->Routine_2++;
		}
		break;
	case 2:
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			*(unsigned long *)&pEm->Routine_0 = 0x401;

	}

	pEm->Routine_3++;
	if (pEm->Routine_3 >= 11) {

		if (EF->pTarget = Em24_search_lv1(pEm))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

}
static void Em24_mv13(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned long *)&pEm->Move_no = 0xF0001;
		pEm->Routine_2++;
	case 1:
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);
		Spd_get(pEm, Kan, Seq, 0);
		Add_speedXZ(pEm, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x100))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		if (pEm->L_pl > 4000)
			*(unsigned long *)&pEm->Routine_0 = 0x1401;
		else

		if (!EF->At_cnt && pEm->L_pl < 2000)
			*(unsigned long *)&pEm->Routine_0 = 0x901;

	}
}
static void Em24_mv14(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned long *)&pEm->Move_no = 0xF0016;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		EF->Wall_at = 0;
		pEm->Routine_2++;
	case 1:

		if (pEm->Sca_info & 1)
			pEm->Timer1++;
		else
			pEm->Timer1 = 0;

		if (pEm->Timer1 >= 6) {
			pEm->Timer0++;

			if (pEm->Timer1 > 20) {
				if (pEm->Timer1 == 21) EF->Wall_at = 0;
				Em24_Wall_escape(pEm, 50, 0);
			}else
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x32);

		}else {
			if (Rnd() & 2)
				Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x32);
			else
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x32);
		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move2(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		if (++pEm->Timer0 >= 180)
			*(unsigned long *)&pEm->Routine_0 = 1;

		else
		if (pEm->L_pl < 2200) *(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
/* retail-only (retail s1 0x80102B10 / s0 0x8010FB10, 156 words; no Beta twin, name unknown):
 * Em24_mv[23] (Routine_1 0x17), a scenario drop like Em24_mv10: wait on Sce_free0, drop,
 * re-orient against the floor, land. */
static void func_80102B10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->pSca_dat = 0;
		pEm->Sce_free0 = 0;
		*(unsigned long *)&pEm->Move_no = 3;
		Joint_move2(pEm, Kan, Seq, 0x1000);
		EF->Gun_area = 0;
		pEm->Spd_y = 0;
		EF->Attribute |= 2;
		pEm->Type |= 0x4000;
		pEm->Routine_2++;
	case 1:
		if (pEm->Sce_free0) {
			EF->Gun_area = 5;
			*(unsigned long *)&pEm->Move_no = 0xC;
			pEm->Timer0 = -1;
			pEm->Type &= ~0x4000;
			pEm->Routine_2++;
		}
		break;
	case 2:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {
			*(unsigned long *)&pEm->Move_no = 0xD;
			pEm->Routine_2++;
		}
		break;
	case 3:
	{
		SVECTOR Ck_pos = { 2200, 0, 0, 0 };

		pEm->Pos_y += 0xA8C;
		Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);
		G.pEm->Cdir_y = (Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00;
		Em_init_super(pEm);
		pEm->Routine_2++;
	}
	case 4:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		*(unsigned long *)&pEm->Move_no = 0xD;
		Joint_move2(pEm, Kan, Seq, 0x1000);
		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			*(unsigned long *)&pEm->Move_no = 0xD;
			pEm->Routine_2++;
		}
		break;
	case 5:
		EF->Gun_area = 4;
		pEm->Pos_y = 0;
		if (Joint_move2(pEm, Kan, Seq, 0x1000))
			*(unsigned long *)&pEm->Routine_0 = 1;
	}
	pEm->Timer0++;
}
void EM_MAIN(ENEMY_WORK *pEm)
{

	register PARTS_W *pParts = (PARTS_W *)pEm->pSin_parts_ptr;
	VECTOR Vec;

	Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, 0, 0);

	if (G.Stop_flg & 0x20000000) return;

	*(unsigned long *)&Add_dir = 0;
	Add_dir.vz = 0;

	if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

	if (EF->At_cnt) EF->At_cnt--;

	Em24_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

	Gunarea_chg(pEm, EF->Gun_area);

	if (Add_dir.vx || Add_dir.vy || Add_dir.vz) {
		Rot_add_matrix(&pEm->Pos_x, &pEm->Super_vector, &pEm->Super_matrix, &Add_dir);

		pEm->Super_vector.vx = (pEm->Super_vector.vx + 0x40) & ~0x7F;
		pEm->Super_vector.vy = (pEm->Super_vector.vy + 0x40) & ~0x7F;
		pEm->Super_vector.vz = (pEm->Super_vector.vz + 0x40) & ~0x7F;
		RotMatrix(&pEm->Super_vector, (MATRIX *)pEm->pSuper);
	}

	Em24_tongue_move(pEm);

	if (!(EF->Attribute & 4)) Em_seq_sound(pEm);

	if (EF->Dm_eff_func) EF->Dm_eff_func(pEm);

	{
		register ENEMY_WORK *pEm2, **ppEm;

		Vec.vx = pParts->Workm.t[0];
		Vec.vy = pParts->Workm.t[1];
		Vec.vz = pParts->Workm.t[2];

		Oba_set_ofs(pEm, &Vec);

		pEm->At_em_no = 0xFF;

		for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++) {

			pEm2 = *ppEm;
			if (pEm == pEm2) continue;

			if (pEm2->Be_flg & 1) {
				Oba_ck_hit(pEm2, pEm);
			}

		}	}
	{
		VECTOR Pos;

		SVECTOR Svec = { EF->Sca_ofs, 0, 0 };

		Rot_vector(pEm->Cdir_y, &Svec, &Svec);
		Pos.vx = pParts[1].Workm.t[0] + Svec.vx;
		Pos.vy = pParts[1].Workm.t[1] + Svec.vy;
		Pos.vz = pParts[1].Workm.t[2] + Svec.vz;

		Oba_set_ofs(pEm, &Pos);

		if (EF->Attribute & 0x100) {
			EF->Attribute &= ~0x100;
			pEm->Sca_old_x = pEm->Atd[0].X;
			pEm->Sca_old_z = pEm->Atd[0].Z;
		}else
			Sca_ck_em(pEm, 0x400);
	}

	Oba_set_ofs(pEm, &Vec);
}
static void Em24_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP;

	*(unsigned long *)&pEm->Move_no = 1;
	Joint_move(pEm, Kan, Seq, 0x1000);	/* EM24: Joint_move (EM22: Joint_move2 0x100) */
	G.pPldamage_Em[20] = EM_PLDM;
	pEm->Be_flg |= 0x1C000000;

	pEm->Damage_flg = 0;
	pEm->Damage_cnt = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;

	if (G.System_flg & 0x40) {
		if (G.System_flg & 0x20) {
			pEm->Life = D_80105948[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 40;
		}else {
			pEm->Life = D_80105968[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 30;
		}
	}else {
		if (G.System_flg & 0x20) {
			pEm->Life = D_801059B8[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 40;
		}else {
			pEm->Life = D_801059D8[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 30;
		}
	}

	pEm->Be_flg |= 0x2000000;

	pP = &pEm->pSin_parts_ptr[8];
	pEm->Neck_flg = 0;
	pEm->Neck_no = 8;
	pP->Free[0] = 0;
	pP->Free[1] = 0;
	pP->Free[2] = 0x60;
	pP->Free[3] = 0x60;
	pP->Free[4] = 0x200;
	pP->Free[5] = 0x138;

	pP = &pEm->pSin_parts_ptr[22];
	pP->Be_flg = 0;
	pP->pSuper = (unsigned long *)&pEm->pSin_parts_ptr[8].Workm;
	pP->pOya_parts = (unsigned long *)&pEm->pSin_parts_ptr[8];
	pP->Pos_x = 0;
	pP->Pos_y = 0;
	pP->Pos_z = 0;
	pP->Cdir_x = 0x400;
	pP->Cdir_y = 0;
	pP->Cdir_z = 0x400;
	RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);

	Tongue_flg = 0;

	pEm->Super_vector.vx = pEm->Super_vector.vy = pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;

	EF->pTarget = (ENEMY_WORK *)&G.Player;

	pEm->Sce_free3 = pEm->Sce_free2 = pEm->Sce_free1 = pEm->Sce_free0 = 0;	/* EM24 */
	pEm->Timer0 = pEm->Timer1 = 0;
	EF->pSca_dat = 0;
	EF->Sence_lv = 0;
	EF->Attribute = 0;
	EF->At_cnt = 0;
	EF->Tongue_Rno = 0;
	EF->Kage_Rno = 0;
	EF->Kage_x = 0x44C;
	EF->Kage_z = 0x352;
	EF->Sca_ofs = -500;
	EF->Dm_eff_rtn = 0;
	EF->Dm_eff_func = 0;

	pEm->Be_flg &= ~0xA;

	*(unsigned long *)&pEm->Routine_0 = 1;
	pEm->Routine_1 = Normal_type_set[ pEm->Type & 0xF ];	/* EM24: one type table */

	Add_dir.vx = Add_dir.vy = Add_dir.vz = 0;
	switch ((pEm->Type >> 8) & 0xF) {
	case 0:

		Em24_Atd_set_1(pEm);	break;
	case 1:
		Add_dir.vz = 0x400;
		pEm->Timer3 = 0;
		pEm->Be_flg |= 8;
		Em24_Atd_set_2(pEm);
		break;
	case 2:
		Add_dir.vz = -0x400;
		pEm->Timer3 = 2;
		Em24_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 3:
		Add_dir.vx = -0x400;
		pEm->Timer3 = 3;
		Em24_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 4:
		Add_dir.vx = 0x400;
		pEm->Timer3 = 1;
		Em24_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 5:
		Add_dir.vx = 0x800;
		Add_dir.vy = 0x800;
		Em24_Atd_set_1(pEm);
		break;
	}

	pEm->Be_flg &= 0xE3FFFFFF;
	pEm->Be_flg |= 0xC000000;

	Kage_work_set((unsigned long *)&pEm->pKage_work, 0, EF->Kage_x | (EF->Kage_z << 16), 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);

	((KAGE_WORK *)pEm->pKage_work)->Off_x = 250;

	pP = &pEm->pSin_parts_ptr[0];	/* retail: seed the first two parts' positions */
	pP->Workm.t[0] = pEm->Pos_x;
	pP->Workm.t[1] = pEm->Pos_y;
	pP->Workm.t[2] = pEm->Pos_z;
	pP = &pEm->pSin_parts_ptr[1];
	pP->Workm.t[0] = pEm->Pos_x;
	pP->Workm.t[1] = pEm->Pos_y;
	pP->Workm.t[2] = pEm->Pos_z;

	pEm->pTbefore_func = Shadow_move;
}
static void Em24_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em24_mv[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em24_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	/* retail: rewritten leap attack (Beta 2: tongue lunge with player grab) */
	register long Dir;
	SVECTOR Unused = { 0, 400, 0, 0 };	/* retail copies this initializer to the frame and never reads it */

	switch (pEm->Routine_2) {
	case 0:
		pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		pEm->Spd_y = 0;
		EF->Retail_29 = 0;
		EF->Retail_2a = 0;
		pEm->Timer0 = 0;
		EF->Retail_2c = pEm->Pos_y;
		if (!(EF->Attribute & 2)) {
			pEm->Routine_2 = 2;
			goto jump;
		}
		*(unsigned long *)&pEm->Move_no = 0x30006;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x10400);
		break;
	case 2:
	jump:
		EF->Gun_area = 3;
		*(signed char *)&EF->Retail_29 = -10;
		EF->Retail_2a = 0;
		EF->Attribute &= ~2;
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
	case 3:
		pEm->Routine_2 += Mot_play2(pEm, 0x17, 0, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 6);
		break;
	case 4:
		pEm->Spd_x = 550;
		pEm->Spd_y = -245;
		*(signed char *)&EF->Retail_29 = -8;
		EF->Retail_2a = 35;
		pEm->Pos_y -= 0x72F;
		EF->Attribute |= 0x200;
		pEm->Routine_2++;
	case 5:
		pEm->Routine_2 += Mot_play2(pEm, 0x18, 0, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x20);

		if (!EF->pTarget->Damage_cnt)
		if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 15) {
			Dir = Get_dir_pl(pEm);
			if (Dir >= -199 && Dir < 712 && pEm->L_pl < 2200 && !pEm->Timer0) {
				EF->Dm_eff_func = func_801039B4;
				EF->Dm_eff_rtn = 0;
				pEm->Timer0 = 1;
				EF->pTarget->pEnemy_ptr = pEm;
				Pad_vib_set0(10, 0);
				Pad_vib_fade_set(0x16, 0xFF, 0x32, 3);
				Pad_vib_fade_set(0xC, 0x37, 0x28, 0x16);
				Pl_life_down(EF->pTarget->Life >= 111 ? 100 : 80, 0);
				Snd_se_enem(10, pEm);
				if (EF->pTarget->Life < 0) {
					*(unsigned long *)&G.Player.Routine_0 = 3;
					EF->pTarget->Life = -1;
					G.System_flg |= 0x4000100;
					G.Player.Timer0 = Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);
				}else {
					if (EF->pTarget->Life < 40) EF->pTarget->Life = 40;
					*(unsigned long *)&EF->pTarget->Routine_0 = 0x202;
					EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);
				}
			}
		}
		break;
	case 6:
		*(unsigned long *)&pEm->Move_no = 0x1C18;
		if (pEm->Pos_y <= EF->Retail_2c - 0x60F) break;
		*(signed char *)&EF->Retail_29 = -60;
		pEm->Spd_y = 0;
		EF->Retail_2a = 0;
		pEm->Pos_y = EF->Retail_2c;
		EF->Attribute &= ~0x200;
		pEm->Routine_2++;
	case 7:
		if (Mot_play2(pEm, 0x19, 0, 0))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

	pEm->Pos_y += pEm->Spd_y;
	Add_speedXZ(pEm, 0);
	pEm->Spd_x += (signed char)EF->Retail_29;
	pEm->Spd_y += EF->Retail_2a;
	if (pEm->Spd_x < 0) {
		EF->Retail_29 = 0;
		pEm->Spd_x = 0;
	}
}
/* retail-only (retail s1 0x801039B4 / s0 0x801109B4, 81 words; no Beta twin, name unknown):
 * Dm_eff_func installed by Em24_mv0a's hit: on even steps 0,2,4,6 rotates one of four
 * effect offsets by the player's Timer0 direction and spawns effect 0x2400 on the player
 * (Set_esp_s); clears itself after 7 steps. */
static void func_801039B4(ENEMY_WORK *pEm)
{
	SVECTOR Pos[4] = { { 350, -400, 400, 0 }, { 350, -300, 250, 0 }, { 350, -200, 100, 0 }, { 350, -100, -50, 0 } };

	if (!(EF->Dm_eff_rtn & 1)) {
		Rot_vector(G.Player.Timer0 << 11, &Pos[ EF->Dm_eff_rtn >> 1 ], &Pos[ EF->Dm_eff_rtn >> 1 ]);
		Set_esp_s((ENEMY_WORK *)&G.Player, 0, 2, 0x2400, 0, 0, &Pos[ EF->Dm_eff_rtn >> 1 ]);
	}
	if (++EF->Dm_eff_rtn == 7) EF->Dm_eff_func = 0;
}
static void Em24_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->pSca_dat = 0;
		*(unsigned long *)&pEm->Move_no = 3;
		EF->Attribute |= 2;
		EF->Gun_area = 0;
		pEm->Spd_y = 0;
		pEm->Routine_2++;
	case 1:
		Joint_move2(pEm, Kan, Seq, 0x1000);

		if ((pEm->Type & 0xF) == 1) {	/* EM24: scenario type 1 waits for Sce_free0 */
			if (pEm->Sce_free0) {
				EF->Gun_area = 5;
				*(unsigned long *)&pEm->Move_no = 0xC;
				pEm->Timer0 = -1;
				pEm->Routine_2++;
			}
		}else if (EF->Sence_lv == 2 || (EF->pTarget = Em24_search_lv0(pEm))) {

			EF->Gun_area = 5;
			*(unsigned long *)&pEm->Move_no = 0xC;
			pEm->Timer0 = -1;
			pEm->Routine_2++;
		}
		break;
	case 2:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {

			pEm->Routine_2++;
			*(unsigned long *)&pEm->Move_no = 0xD;
		}	break;
	case 3:
	{
		SVECTOR Ck_pos = { 2200, 0, 0, 0 };

		pEm->Pos_y += 0xA8C;
		Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);
		G.pEm->Cdir_y = (Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00;
		Em_init_super(pEm);
		pEm->Routine_2++;
	}
	case 4:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		*(unsigned long *)&pEm->Move_no = 0xD;
		Joint_move2(pEm, Kan, Seq, 0x1000);

		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			*(unsigned long *)&pEm->Move_no = 0xD;
			pEm->Routine_2++;
		}
		break;

	case 5:
		EF->Gun_area = 4;
		pEm->Pos_y = 0;
		if (Joint_move2(pEm, Kan, Seq, 0x1000))
			*(unsigned long *)&pEm->Routine_0 = 1;

	}
	pEm->Timer0++;
}
static void Em24_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	/* EM24's move selector (cf. Em22_mv0f): no low-life escape, different choices */
	register short Dir;

	if (EF->pTarget->Life < 0 || pEm->Routine_0 == 7) {
		*(unsigned long *)&pEm->Routine_0 = 0xE01;	goto exit;
	}

	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, EF->pTarget->Pos_x, EF->pTarget->Pos_z);
	Dir = (Dir - pEm->Cdir_y) & 0xFFF;
	if (Dir > 0x800) Dir = 0x1000 - Dir;
	Dir = Branch(Dir, 0x100, 0x200, 0x600);

	switch (Branch(pEm->L_pl, 2500, 4500, 6500)) {
	case 0:
		switch (Dir) {
		case 0:
			if (!EF->At_cnt) {
				if (!(Rnd() & 1)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
		case 1:
			if (!EF->At_cnt) {
				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
		case 2:
		case 3:
			if (!EF->At_cnt) {
				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}	*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
		}
		break;
	case 1:
		switch (Dir) {
		case 0:
			if (!EF->At_cnt) {
				*(unsigned long *)&pEm->Routine_0 = 0x801;	goto exit;
			}	*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
		case 1:
			if (EF->At_cnt) {
				if (!(Rnd() & 1)) {
					*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
				}	*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x801;	goto exit;
		case 2:
		case 3:
			*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;
		}
		break;
	case 2:
		switch (Dir) {
		case 0:
			if (!(EF->Attribute & 1)) {
				if (Rnd() & 2) {
					if ((Rnd() & 7) < 6) {
						*(unsigned long *)&pEm->Routine_0 = 0xA01;	goto exit;
					}	*(unsigned long *)&pEm->Routine_0 = 0x801;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
		case 1:
			if (!(Rnd() & 3)) {
				*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;
			}
			if (Rnd() & 1) {
				if (EF->pTarget->Life <= 100) {
					*(unsigned long *)&pEm->Routine_0 = 0xA01;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
		case 2:
		case 3:	*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
		}
		break;
	case 3:
		if (pEm->L_pl < 10000 || EF->Sence_lv != 2) {
			EF->Sence_lv = 2;
			switch (Dir) {
			case 0:
			case 1:
			case 2:
				if ((Rnd() & 7) < 5) {
					*(unsigned long *)&pEm->Routine_0 = 0xA01;	goto exit;
				}	*(unsigned long *)&pEm->Routine_0 = 0x1401;	goto exit;
			case 3:
				*(unsigned long *)&pEm->Routine_0 = 0xC01;	goto exit;
			}
			break;
		}
		*(unsigned long *)&pEm->Routine_0 = 0xC01;
	}

exit:
	if (pEm->Routine_1 != 0xF)
		Em24_mv[ pEm->Routine_1 ](pEm, Kan, Seq);

	EF->Attribute &= ~1;
}
static void Em24_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (pEm->Routine_1 != 0xFF) {
		EF->pTarget = (ENEMY_WORK *)&G.Player;
		EF->Wep_no = pEm->Routine_1;

		if (Tongue_flg == pEm->Work_no) Tongue_flg = 0;
		pEm->Routine_1 = 0xFF;
		EF->Sence_lv = 2;
		Em24_damage_ck(pEm);
		if (pEm->Damage_no) {
			pEm->Be_flg &= ~0xE;
			EF->Attribute &= 0xFD0F;	/* retail: also clears 0x200 */
		}
	}

	Em24_dm[ pEm->Damage_no ](pEm, Kan, Seq);	}
static void Em24_damage_ck(ENEMY_WORK *pEm)
{
	register unsigned long Dir;	

	if (!(EF->Attribute & 0x40)) {

		if (EF->pSca_dat || pEm->Pos_y < pEm->Ground) {
			pEm->Damage_no = 5;	/* EM24: always (EM22 picks 5 or 2 by Attribute 0x80) */
		}else {

			Dir = Cdir_ck(pEm, EF->pTarget);

			switch (EF->Wep_no) {

			case 5: case 6: case 7: case 8: case 9:	/* retail: 9 moved here; random light flinch */
				if (Dir)

					pEm->Damage_no = 4;
				else
					pEm->Damage_no = 3;	break;

			case 10:
				if (Dir)
					pEm->Damage_flg |= 0x20;
				else
					pEm->Damage_flg &= ~0x20;
				pEm->Damage_no = 6;
				break;
			case 11:
				if (Dir)
					pEm->Damage_no = 4;
				else
					pEm->Damage_no = 3;
				break;

			case 4: case 12: case 14: case 15:

				pEm->Damage_no = 1;
				break;
			case 17:	/* retail: was Damage_no 0 */
				if (Dir)
					pEm->Damage_flg |= 0x20;
				else
					pEm->Damage_flg &= ~0x20;
				pEm->Damage_no = 6;
				break;
			case 1: case 2: case 3: case 13: case 16: case 18: case 19:
				pEm->Damage_no = 0;	}	}
	}else {

		pEm->Damage_no = 0;
	}

}
static void Em24_damage_after_ck(ENEMY_WORK *pEm)
{
	if (pEm->L_pl < 2500) {
		if (!Goto00_ck(EF->pTarget, pEm->Pos_x, pEm->Pos_z, 0x140)) {
			EF->Attribute |= 1;
			*(unsigned long *)&pEm->Routine_0 = 0x1301;
			return;
		}else {	if (Rnd() & 1)
				*(unsigned long *)&pEm->Routine_0 = 0xC01;
			else
				*(unsigned long *)&pEm->Routine_0 = 0xF01;	}

	}else {
		if (!Goto00_ck(EF->pTarget, pEm->Pos_x, pEm->Pos_z, 0x1E0))
			*(unsigned long *)&pEm->Routine_0 = 0x1401;
		else
			*(unsigned long *)&pEm->Routine_0 = 0xC01;
	}
}
static void Em24_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Life >= 0) {
		*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		if (pEm->Routine_1 != 0xFF) {
			EF->Attribute |= 1;
			Em24_mv[ pEm->Routine_1 ](pEm, Kan, Seq);
			EF->Attribute &= ~1;
		}
	}else
		*(unsigned long *)&pEm->Routine_0 = 3;

}
static void Em24_dm_s(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;

	if (pEm->Damage_flg & 0x20) {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0)
				Em24_damage_after_ck(pEm);
			else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}
	}else {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0) {
				Em24_damage_after_ck(pEm);
			}else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}

	}	}
static void Em24_dm_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;
	register long Up_dir;
	unsigned char next_routine; /* [INFERRED] consumed case-3 increment, not an original retail SYM name. */

	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Spd_y = pEm->Spd_x = pEm->Spd_z = 0;
		switch ((unsigned char)pEm->Timer3) {
		case 0:
			pEm->Spd_x = 150;	break;
		case 1:
			pEm->Spd_z = -150;	break;
		case 2:
			pEm->Spd_x = -150;	break;
		case 3:
			pEm->Spd_z = 150;
		}	Up_dir = 4;
		if (*(unsigned char *)&pEm->Timer2)
			Up_dir = *(unsigned char *)&pEm->Timer2;

		if ((unsigned char)pEm->Timer3 - Up_dir < 0)

			*(unsigned long *)&pEm->Move_no = 0x11;
		else
			*(unsigned long *)&pEm->Move_no = 0x12;

		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
		pEm->Timer0 = 4;
		do { }while (0);	/* [INFERRED] code-less retail line (compiled-out macro): its loop notes are a scheduling barrier */

		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);	break;

	case 2:
		pEm->Pos_y += 0x4F5;
		pEm->Cdir_y = ((unsigned char)pEm->Timer2 << 10) - 0x800;
		Em_init_super(pEm);
		pEm->Routine_3++;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0x13;
		Joint_move2(pEm, Kan, Seq, 0x400);

		if (pEm->Pos_y >= EF->Retail_2c) {	/* EM24: was >= 0 */

			EF->pSca_dat = 0;
			Em24_Atd_set_1(pEm);
			/* retail: moved out of Em24_Atd_set_1. Capture the byte increment before the flag update;
			 * the final byte store preserves wraparound for all 256 input values. */
			next_routine = pEm->Routine_3 + 1;
			pEm->Be_flg &= ~8;
			*(unsigned long *)&pEm->Move_no = 0x13;
			/* [INFERRED] Four one-pass macro-style levels weight this real store: GCC/GDB gives the
			 * increment quantity 8 refs/14 lifetime, ahead of flags' 4/6. This preserves v0 for the
			 * shared case-5 tail and the original load/AND schedule (4 diffs -> 0 in both banks).
			 * Three levels give 7 refs and do not match. Exact original macro spelling is unknown. */
			do { do { do { do {
				pEm->Routine_3 = next_routine;
			} while (0); } while (0); } while (0); } while (0);

			do { }while (0);	/* [INFERRED] code-less retail line: compiled-out macro; its loop notes are a scheduling barrier (keeps the Joint_move2 setup below Routine_3++, so the tail cross-jumps into case 5 as in retail) */

			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);	}	break;
	case 5:
		if (pEm->Life < 0) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			pEm->Be_flg &= ~0xC;
			pEm->Routine_2++;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0x10;
		pEm->Routine_3++;
	case 1: case 4: case 6:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		break;
	case 7:
		pEm->Damage_cnt &= 0x7F;
		EF->Gun_area = 3;
		pEm->Be_flg &= ~0xC;
		Em24_damage_after_ck(pEm);
	}

	if (pEm->Pos_y + pEm->Spd_y + 0x48 < 0) {
		pEm->Spd_y += 0x30;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
	}else
		pEm->Pos_y = pEm->Ground;	/* EM24: was 0 */

}
static void Em24_dm_lf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	VECTOR TmpV1, TmpV2;

	switch (pEm->Routine_3) {
	case 0:

		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Spd_x = 50;
		pEm->Spd_y = pEm->Spd_z = 0;
		Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);
		pEm->Timer0 = Rnd() & 0x11;	/* retail: random hold time (was 0) */

		pEm->Atd[2].Ofs_x = -0x2A0;
		pEm->Atd[2].Ofs_z = 0;

		*(unsigned long *)&pEm->Move_no = 9;
		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
	case 1:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		if (EF->Sca_ofs > -800) EF->Sca_ofs -= 40;

		EF->Gun_area = 3;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		break;

	case 2:
		if (pEm->Life < 0) {
			pEm->Routine_2++;
			pEm->Routine_3 = 0;
			break;
		}
		if (--pEm->Timer0 > 0) break;	/* retail: holds for Timer0 frames */
		*(unsigned long *)&pEm->Move_no = 0x30010;

		pEm->Timer0 = 0;	/* retail: no fall-through into case 3 */
		pEm->Routine_3 = 3;
		break;
	case 3:
		pEm->Timer0++;
		if (EF->Sca_ofs < -500) EF->Sca_ofs += 80;

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x400)) {
			pEm->Damage_cnt &= 0x7F;
			EF->Sca_ofs = -500;
			EF->Gun_area = 3;
			Em24_damage_after_ck(pEm);
		}
		Add_speedXZ(pEm, 0);
	}
}
static void Em24_dm_lb(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
		pEm->Spd_x = 100;
		pEm->Spd_y = pEm->Spd_z = 0;
		Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		Goto01(pEm, EF->pTarget->Cdir_y, 0x80);
		pEm->Timer0 = 0;

		*(unsigned long *)&pEm->Move_no = 0x3000A;
		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
	case 1:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
		pEm->Timer0++;

		if (pEm->Timer0 == 4) {
			pEm->Spd_x = 50;
			pEm->Spd_y = pEm->Spd_z = 0;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		}

		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		break;

	case 2:
		if (pEm->Life < 0) {
			pEm->Routine_2++;
			pEm->Routine_3 = 0;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		EF->Attribute |= 2;
		pEm->Routine_3 = 3;
	case 3:
		if (Joint_move2(pEm, Kan, Seq, 0x10100)) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			Em24_damage_after_ck(pEm);
		}
	}
}
static void Em24_dm_flame(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;

	if (pEm->Damage_flg & 0x20) {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0)
				Em24_damage_after_ck(pEm);
			else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}
	}else {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0) {
				Em24_damage_after_ck(pEm);
			}else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}

	}	}
/* retail-only (retail s1 0x80105140 / s0 0x80112140, 150 words; no Beta twin, name unknown):
 * Em24_dm[7], chosen by Em24_damage_ck when Attribute bit 0x200 (set during Em24_mv0a's
 * leap) is up: the licker drops from the leap height to the saved ground height
 * EF->Retail_2c, then gets up (cf. Em24_dm_fall). */
static void func_80105140(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;	/* unused, as in Em24_dm_fall / Em24_dm_fall_v: retail reserves its frame slot */

	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		*(unsigned long *)&pEm->Move_no = 0x13;
		pEm->Spd_y = pEm->Spd_x = pEm->Spd_z = 0;
		EF->Attribute |= 2;
		pEm->Pos_y += 0x546;
		pEm->Routine_3++;
	case 1:
		Joint_move2(pEm, Kan, Seq, 0x1000);
		pEm->Move_cnt = 0;
		if (pEm->Pos_y < EF->Retail_2c) break;
		EF->pSca_dat = 0;
		Em24_Atd_set_1(pEm);
		*(unsigned long *)&pEm->Move_no = 0x13;
		pEm->Be_flg &= ~8;
		pEm->Routine_3++;
	case 2:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
		break;
	case 3:
		if (pEm->Life < 0) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			pEm->Be_flg &= ~0xC;
			pEm->Routine_2++;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0x10;
		pEm->Routine_3++;
	case 4:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		break;
	case 5:
		pEm->Damage_cnt &= 0x7F;
		EF->Gun_area = 3;
		pEm->Be_flg &= ~0xC;
		Em24_damage_after_ck(pEm);
	}

	if (pEm->Pos_y + pEm->Spd_y + 0x48 < EF->Retail_2c) {
		pEm->Spd_y += 0x30;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
	}else
		pEm->Pos_y = EF->Retail_2c;
}
static void Em24_die_ck(ENEMY_WORK *pEm)
{	/* EM24: no Attribute 0x40 / 0x200 / 0x80 cases (die types 3, 4 and 2 are not in Em24_die) */
	

	if (EF->pSca_dat || pEm->Pos_y < pEm->Ground)
		pEm->Damage_no = 5;
	else if (Cdir_ck(pEm, (ENEMY_WORK *)&G.Player))
		pEm->Damage_no = 1;
	else
		pEm->Damage_no = 0;
}
static void Em24_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Routine_1 != 0xFF) {
		EF->Wep_no = pEm->Routine_1;
		pEm->Routine_1 = 0xFF;
		EF->pTarget = (ENEMY_WORK *)&G.Player;
		Em_dead_flg_on(pEm);
		func_80101704(pEm);
		Em24_die_ck(pEm);
	}

	Em24_die[ pEm->Damage_no ](pEm, Kan, Seq);
}
static void Em24_die_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Tongue_flg = 0;	/* EM24 */
		Snd_se_enem(8, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Be_flg |= 2;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Mot_play2_sp(pEm, 0x14, 0, 0);
		if (EF->Sca_ofs > -1000) EF->Sca_ofs -= 50;
		break;
	case 2:
		pEm->Routine_2++;
	case 3:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}
		break;
	case 4: case 5:	break;
	}
}
static void Em24_die_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(8, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
		pEm->Be_flg |= 2;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Mot_play2_sp(pEm, 0x15, 0xF, 0);
		if (EF->Sca_ofs < -100) EF->Sca_ofs += 25;
		break;
	case 2:
		EF->Kage_Rno = 1;
		pEm->Routine_2++;

	}
}
/* retail: Em24_Pl00 is gone (no code for it in the retail overlay); Em24_pldm_tbl is empty and
 * shares its address with Dm_eff_spark's pEsp2 (Em124_Pldm indexes 0x80105D90). In its place the
 * retail text has Em24_die[5]: */
/* retail-only (retail s1 0x801056B0 / s0 0x801126B0, 44 words; no Beta twin, name unknown):
 * Em24_die[5] (Damage_no 5 from Em24_die_ck, Attribute bit 0x200): Em24_die_wall with the
 * leap-fall damage func_80105140 in place of Em24_dm_fall. */
static void func_801056B0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		func_80105140(pEm, Kan, Seq);
		break;
	case 1:
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
		break;
	case 2:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}

	}
}
/* [INFERRED] Native retail order puts this dispatcher after the die[5]
 * handler above. Both bodies are unchanged; source order, not an internal
 * linker origin, establishes their correct offsets in each bank. */
void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em24_pldm_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}
/* ---- data emitted after Em24_mv07's local static (retail data order) ---- */
static WEP_BLOOD Wep_blood_tbl[20] = {
	{ 0, 0, 0, 0, 0, 6, 8192 },
	{ 0, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 5, 0, 0, 24, 0, 6, 8192 },
	{ 8, 0, 0, 24, 0, 6, 8192 },
	{ 4, 0, 12, 12, 5, 4, 5120 },
	{ 6, 0, 0, 24, 5, 4, 6144 },
	{ 4, 0, 0, 24, 4, 9, 10240 },	/* retail: 7 -> 9 */
	{ 0, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 0, 0, 12, 12, 6, 0, 4096 },
	{ 1, 0, 0, 24, 0, 6, 8192 },
	{ 6, 0, 0, 24, 5, 4, 6144 },
	{ 1, 0, 8, 12, 5, 6, 8192 },	/* retail: 0 -> 5 */
	{ 1, 0, 8, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 }	/* EM24: 8 -> 12 */
};
static void (*Em24_routine_0[4])() = { Em24_Init, Em24_Move, Em24_Damage, Em24_Die };	/* 0x80105CEC */
static void (*Em24_mv[24])() = { Em24_mv00, 0, Em24_mv02, Em24_mv03, Em24_mv04, Em24_mv05, Em24_mv06, 0, Em24_mv08, Em24_mv09, Em24_mv0a, Em24_mv0b, Em24_mv0c, 0, Em24_mv0e, Em24_mv0f, 0, 0, 0, Em24_mv13, Em24_mv14, 0, 0, func_80102B10 };	/* 0x80105CFC */
static void (*Em24_dm[7])() = { Em24_dm_no, Em24_dm_s, Em24_dm_fall, Em24_dm_lf, Em24_dm_lb, func_80105140, Em24_dm_flame };	/* 0x80105D5C */
static void (*Em24_die[6])() = { Em24_die_f, Em24_die_b, 0, 0, 0, func_801056B0 };	/* 0x80105D78 */
static void (*Em24_pldm_tbl[])() = { };	/* retail: empty (Em24_Pl00 removed) */
/* Set_esp_w, Set_esp_s, Get_dir_pl: the same code as Beta 2 EM2B_SUB.C (names and bodies from there),
 * linked at the end of the EM24 text (s1 0x8010579C..0x80105944). */
static void Set_esp_w(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	SVECTOR Svec;

	ApplyMatrixSV(&pParts[Parts_no].Workm, pSvec, &Svec);
	Svec.vx += pParts[Parts_no].Workm.t[0];
	Svec.vy += pParts[Parts_no].Workm.t[1];
	Svec.vz += pParts[Parts_no].Workm.t[2];
	Esp_call(Id << 24 | Type << 16 | Zoom, Cdir_y, &GsIDMATRIX, &Svec);
}
static void Set_esp_s(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	Esp_call(Id << 24 | Type << 16 | Zoom, Cdir_y, &pParts[Parts_no].Workm, pSvec);
}
static long Get_dir_pl(ENEMY_WORK *pEm)
{
	register short Dir;

	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
	Dir = (Dir - pEm->Cdir_y) & 0xFFF;
	if (Dir > 0x800) Dir |= 0xF000;

	return Dir;
}
