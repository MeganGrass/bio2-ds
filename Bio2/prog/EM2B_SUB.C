static PARTS_FLY Parts_fly[15] = {
	{ 1, 90, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 2, 90, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 4, 90, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 6, 90, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 8, 90, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 9, 90, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 10, 90, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 11, 90, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 12, 90, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 14, 80, 200, 0, -200, 0, 0, 0, 0, 1 },
	{ 15, 80, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 16, 80, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 17, 80, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 18, 80, 200, 0, -200, 0, 0, 0, 0, 0 },
	{ 19, 80, 200, 0, -200, 0, 0, 0, 0, 0 }
};

static void Em2b_parts_fly_init(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts;
	register PARTS_FLY *pP_fly;
	SVECTOR Svec;
	register unsigned long i;

	G.Random = 0x2DE4;

	for (i = 0; i < 15; i++) {
		pP_fly = &Parts_fly[i];
		pParts = pEm->pSin_parts_ptr + pP_fly->Parts_no;

		pParts->Be_flg |= 0x40;

		pP_fly->Dir_y = Rnd() << 4;
		pP_fly->Spd += (signed char)Rnd() / 2;
		pP_fly->Spd_y += (signed char)Rnd();
		pP_fly->Cdirspd = (signed char)Rnd() * 2;
		pP_fly->Cdiradd = 0;

		Svec.vx = Svec.vy = Svec.vz = 0;
		Rot_add_matrix(&pParts->Pos_x, &pParts->Cdir_x, &pParts->Workm, &Svec);

		pParts->Cdir_z &= 0xFFF;
		pP_fly->Cflg = pParts->Cdir_z >= 0x400 && pParts->Cdir_z < 0xC00;
		pP_fly->Timer = 0;
	}

}
static void Em2b_parts_fly_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts;
	register PARTS_FLY *pP_fly;
	SVECTOR Svec;
	register unsigned long i;
	char Ck;	/* inferred name: no retail SYM record (combined away) */
	for (i = 0; i < 15; i++) {
		pP_fly = &Parts_fly[i];
		pParts = pEm->pSin_parts_ptr + pP_fly->Parts_no;

		pP_fly->Timer++;

		pP_fly->Spd_y += 98 * pP_fly->Timer * pP_fly->Timer / 20 - 98 * (pP_fly->Timer - 1) * (pP_fly->Timer - 1) / 20;

		if (pParts->Workm.t[1] >= 0) {
			pP_fly->Spd /= 20;
			pP_fly->Spd_y /= -28;
			pParts->Workm.t[1] = 0;
			pP_fly->Timer = 0;
			pParts->Cdir_z &= 0xFFF;
			if (pParts->Cdir_z < 0x400 || pParts->Cdir_z >= 0xC00) {
				if (pParts->Cdir_z < 0x400)
					pP_fly->Cdiradd = 10;
				else
					pP_fly->Cdiradd = -10;
			}else {
				if (pParts->Cdir_z > 0x800)
					pP_fly->Cdiradd = 10;
				else
					pP_fly->Cdiradd = -10;
			}
		}

		pParts->Cdir_z += pP_fly->Cdirspd;
		pP_fly->Cdirspd += pP_fly->Cdiradd;
		pParts->Cdir_z &= 0xFFF;
		Ck = pParts->Cdir_z >= 0x400 && pParts->Cdir_z < 0xC00;

		if (pP_fly->Cdiradd) {
			if (Ck != pP_fly->Cflg)
				pP_fly->Cdirspd /= -20;

		}else
			pP_fly->Cflg = Ck;

		if (pP_fly->Cdirspd < 10 && pParts->Cdir_z > 0x400 - 10 && pParts->Cdir_z < 0x400 + 10) {

			pP_fly->Cdirspd = 0; pP_fly->Cdiradd = 0;
		}
		if (pP_fly->Cdirspd < 10 && pParts->Cdir_z > 0xC00 - 10 && pParts->Cdir_z < 0xC00 + 10) {

			pP_fly->Cdirspd = 0; pP_fly->Cdiradd = 0;
		}

		Svec.vx = pP_fly->Spd;
		Svec.vz = 0;
		Rot_vector(pEm->Cdir_y + pP_fly->Dir_y, &Svec, &Svec);
		pParts->Workm.t[0] += Svec.vx;
		pParts->Workm.t[1] += pP_fly->Spd_y;
		pParts->Workm.t[2] += Svec.vz;
		RotMatrix((SVECTOR *)&pParts->Cdir_x, &pParts->Workm);

		pP_fly->Spd += pP_fly->Add;
	}
}
static void Em2b_parts_smoke(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts;
	register PARTS_FLY *pP_fly;
	SVECTOR Svec;
	register unsigned long i;

	for (i = 0; i < 15; i++) {
		pP_fly = &Parts_fly[i];
		pParts = pEm->pSin_parts_ptr + pP_fly->Parts_no;

		if (pP_fly->Smoke_flg) {
			Svec.vx = Svec.vy = Svec.vz = 0;
			Esp_call(0x40E2000, Rnd() << 4, &pParts->Workm, &Svec);
		}
	}
}
static void Em2b_parts_fly_end(ENEMY_WORK *pEm, short x, short z)
{
	register PARTS_W *pParts;
	register PARTS_FLY *pP_fly;
	SVECTOR Svec;
	VECTOR Vec;
	register unsigned long i;

	for (i = 0; i < 15; i++) {
		pP_fly = &Parts_fly[i];
		pParts = pEm->pSin_parts_ptr + pP_fly->Parts_no;
	/* re-centre each part: x + (t - Pos), Em2b_dead parked the enemy at (-0x38D3, -0x392C) */
		pParts->Workm.t[0] = x + (pParts->Workm.t[0] + 0x38D3);
		pParts->Workm.t[2] = z + (pParts->Workm.t[2] + 0x392C);
		Vec.vx = pParts->Workm.t[0];
		Vec.vy = 0;
		Vec.vz = pParts->Workm.t[2];

		if (Sca_ck_hit(&Vec, 0, 1 << pEm->nFloor, 0x400))
			pParts->Be_flg = 0;
		Color_chg(pEm, pP_fly->Parts_no, 0x40, 0x40, 0x40);
	}
}
static unsigned long Em2b_att_ck2(ENEMY_WORK *pEm, unsigned long Parts_no, unsigned short Start, unsigned short End, unsigned long At_dis, unsigned long At_w)
{

	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	register unsigned long Dis;
	register long Dir0, Dir1, Dir2;
	register short Old_x, Old_z;
	SVECTOR Svec;

	Old_x = EF(pEm)->Pold_x;
	Old_z = EF(pEm)->Pold_z;
	EF(pEm)->Pold_x = pParts[Parts_no].Workm.t[0];
	EF(pEm)->Pold_z = pParts[Parts_no].Workm.t[2];
	if (pEm->Move_cnt < Start || pEm->Move_cnt > End) return 0;

	Dis = SquareRoot0((pEm->Pos_x - pParts[Parts_no].Workm.t[0]) * (pEm->Pos_x - pParts[Parts_no].Workm.t[0]) + (pEm->Pos_z - pParts[Parts_no].Workm.t[2]) * (pEm->Pos_z - pParts[Parts_no].Workm.t[2]));
	Dis += SquareRoot0((pEm->Pos_x - Old_x) * (pEm->Pos_x - Old_x) + (pEm->Pos_z - Old_z) * (pEm->Pos_z - Old_z));
	Dis += 500 + At_w; Dis /= 2;

	if (!pEm->Timer0 && pEm->L_pl < Dis) {

		Dir0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, Old_x, Old_z);
		Dir1 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pParts[Parts_no].Workm.t[0], pParts[Parts_no].Workm.t[2]);

		if (Dir1 < Dir0) {
			Dir2 = Dir0;
			Dir0 = Dir1;
			Dir1 = Dir2;
		}

		Dir1 = (Dir1 - Dir0) & 0xFFF;
		if (Dir1 >= 0x800) {
			Dir0 = Dir1 + Dir0 - 0x1000;
			Dir1 = 0x1000 - Dir1;
		}
		Dir0 -= At_dis;
		Dir1 += At_dis * 2;

		Dir2 = Direction_ck(pEm->Pos_x, pEm->Pos_z, ((ENEMY_WORK *)&G.Player)->Pos_x, G.Player.Pos_z);	/* player read as ENEMY_WORK: keeps retail's per-use address (no CSE across the call) */
		Dir2 = (Dir2 - Dir0) & 0xFFF;

		if (Dir2 > 0 && Dir2 < Dir1) {

			Svec.vx = G.Player.Pos_x;
			Svec.vy = pParts[Parts_no].Workm.t[1];
			Svec.vz = G.Player.Pos_z;
			Esp_call(0x62800, pEm->Cdir_y, &GsIDMATRIX, &Svec);
			pEm->Timer0 = 1;
			return 1;
		}
	}
	return 0;
}
static long Get_dir_pl(ENEMY_WORK *pEm)
{
	register short Dir;

	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
	Dir = (Dir - pEm->Cdir_y) & 0xFFF;
	if (Dir > 0x800) Dir |= 0xF000;

	return Dir;
}
static void Set_tri(SVECTOR *pP1, SVECTOR *pP2, SVECTOR *pP3)
{
	static POLY_F3 Tri[2] = {
		{ 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0 }
	};
	register POLY_F3 *pTri;
	long p, flg;

	pTri = &Tri[G.Cbuf_id];
	SetPolyF3(pTri);

	RotAverage3(pP1, pP2, pP3, (long *)&pTri->x0, (long *)&pTri->x1, (long *)&pTri->x2, &p, &flg);

	AddPrim(G.pOt1, pTri);
}
static DM_ESP Blood_tbl[36] = {
	{ 200, 600, 100, 0, 1 },
	{ 100, 400, 0, 300, 2 },
	{ 200, 600, 200, -100, 18 },
	{ 200, 200, 0, 400, 19 },
	{ 100, 700, 100, -100, 19 },
	{ 100, 700, 100, -100, 2 },
	{ 200, 0, 300, -200, 0 },
	{ 200, 100, 0, 200, 18 },
	{ 400, -200, 100, 0, 4 },
	{ 500, -400, -100, 200, 4 },
	{ 300, 600, 100, -400, 1 },
	{ 300, 400, 200, 100, 18 },
	{ 500, -100, 300, 0, 1 },
	{ 100, 0, 100, -400, 7 },
	{ 300, -200, 50, 400, 13 },
	{ 300, -300, 150, -200, 4 },
	{ 500, -200, -250, 300, 5 },
	{ 200, 100, 0, -400, 6 },
	{ -200, 500, -150, 1901, 2 },
	{ -200, 100, 50, 2601, 2 },
	{ -200, 500, -200, 2000, 1 },
	{ -300, 300, 0, 1601, 18 },
	{ -200, 100, -100, 1601, 18 },
	{ -200, 600, 0, 2301, 19 },
	{ -300, 400, -300, 2001, 1 },
	{ -400, 100, -400, 1901, 0 },
	{ -300, -200, 200, 2501, 4 },
	{ -300, 100, -300, 1701, 5 },
	{ -200, 400, 100, -1499, 7 },
	{ -200, 500, -200, -2099, 13 },
	{ -200, 100, -100, -2299, 5 },
	{ -200, -100, 100, -1799, 5 },
	{ -300, -400, -400, -2399, 5 },
	{ -300, -400, 400, -1699, 5 },
	{ -200, -200, -100, -1999, 7 },
	{ -100, -200, -100, -2199, 13 }
};
static DM_ESP2 Spark_tbl[24] = {
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 600, -500, -400, 0, 5, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 100, -200, 0, 0, 13, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 100, -300, 0, 0, 2, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -400, 0, -200, 0, 5, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 500, 0, 0, 19, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 300, 0, 6, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -100, -100, -200, 0, 14, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 0, 2 },
	{ 300, -300, 200, 0, 4, 0 },
	{ 0, 0, 0, 0, 0, 2 },
	{ -400, -200, 100, 0, 8, 0 },
	{ 0, 0, 0, 0, 0, 2 }
};
static DM_ESP Flame_tbl[14] = {
	{ 0, -100, 0, 12288, 5 },
	{ -350, -300, 0, 7297, 4 },
	{ 390, -200, 400, 5097, 5 },
	{ 300, -100, -70, 7197, 4 },
	{ 100, 0, 0, 5545, 13 },
	{ 600, -200, -250, 9661, 5 },
	{ -300, -200, -200, 7515, 5 },
	{ -300, -100, 330, 5346, 5 },
	{ -150, 300, 280, 5946, 6 },
	{ 0, 300, -300, 5946, 12 },
	{ 300, 600, 0, 6246, 13 },
	{ -70, 800, -200, 6346, 13 },
	{ 0, 0, 0, 6137, 8 },
	{ -400, -300, 0, 6137, 8 }
};
static void Foot_set(ENEMY_WORK *pEm, unsigned long Foot_no)
{
	register PARTS_W *pP_ptr;
	MATRIX M;

	pP_ptr = pEm->pSin_parts_ptr;

	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &M);

	pP_ptr += Foot_no * 17 + 3;

	CompM(&M, (MATRIX *)pP_ptr[-2].M, &M);
	CompM(&M, (MATRIX *)pP_ptr[-1].M, &M);
	CompM(&M, (MATRIX *)pP_ptr->M, &M);

	pEm->Pos_x -= M.t[0] - pP_ptr->Workm.t[0];
	pEm->Pos_z -= M.t[2] - pP_ptr->Workm.t[2];
}
static unsigned long Em2b_add_dm(unsigned long Att)
{
	register long Pl_life;	/* inferred name (cf. EM23.C): the compiler deletes it, retail SYM has no record */

	Pl_life = Pl_life_down(Att, 0);

	if (Pl_life & 2) *(unsigned long *)&G.Player.Routine_0 = 3;
}
static void Breast_move(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr + 5;
	static unsigned long Time;

	switch (EF(pEm)->Breast_Rno) {
	case 0:
		Time = 0;
		EF(pEm)->Breast_Rno++;
	case 1:
		((short *)pParts->pSa_dat_head)[6] = 0x800 * Time * Time / 100;

		Sa_parts_allmv(pEm, pParts);

		Time++;
		if (Time == 10) EF(pEm)->Breast_Rno++;
		break;
	case 2:
		Time--;
		if (!Time) EF(pEm)->Breast_Rno++;
		((short *)pParts->pSa_dat_head)[6] = 0x1000 - 0x800 * Time * Time / 100;

		Sa_parts_allmv(pEm, pParts);
		break;
	case 3:
		((short *)pParts->pSa_dat_head)[6] = 0x1000 - 0x800 * Time * Time / 100;

		Sa_parts_allmv(pEm, pParts);

		Time++;
		if (Time == 10) EF(pEm)->Breast_Rno++;
		break;
	case 4:
		Time--;
		if (!Time) EF(pEm)->Breast_Rno = 1;
		((short *)pParts->pSa_dat_head)[6] = 0x800 * Time * Time / 100;

		Sa_parts_allmv(pEm, pParts);

	}

}
static unsigned long S_rnd(unsigned char N, unsigned long *pBuf, unsigned long *pBuf2)
{
	register long Ck;

	if (*pBuf > 32 || !--*pBuf) {

		*pBuf = N;
		*pBuf2 = 0x80000000 >> (Rnd() % N);
	}
	Ck = *pBuf2;
	*pBuf2 <<= 1;
	return Ck < 0;

}
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
static void Dm_eff_blood(ENEMY_WORK *pEm, unsigned long No, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Atr)
{
	register DM_ESP *pEsp;	register unsigned long Ofs, Mask, i;
	Ofs = 0; Mask = 36;

	if (Atr & 2) {
		if (Cdir_ck(pEm, &G.Player)) Ofs = 18;
		Mask = 18;
	}

	if (Atr & 1) {
		Ofs += (pEm->Damage_no % 3) * 6;
		Mask = 6;
	}

	while (No-- > 0) {

		i = Ofs + Rnd() % Mask;
		pEsp = &Blood_tbl[i];

		if (Atr & 4) {
			Set_esp_s(pEm, Id, Type, Zoom, pEm->Cdir_y + pEsp->Dir_y, pEsp->Parts, (SVECTOR *)pEsp);

		}else
			Set_esp_w(pEm, Id, Type, Zoom, pEm->Cdir_y + pEsp->Dir_y, pEsp->Parts, (SVECTOR *)pEsp);

	}
}
static void Dm_eff_0f(ENEMY_WORK *pEm)
{
	static int Damage_no, Damage_old;

	switch (EF(pEm)->Dm_eff_rtn) {
	case 0:
		Damage_no = pEm->Damage_no;
		Dm_eff_blood(pEm, 1, 0, 6, 0x2000, 3);
		EF(pEm)->Dm_eff_func = Dm_eff_0f;
		EF(pEm)->Dm_eff_rtn++;
		return;

	case 4: case 7: case 10:
		Damage_old = pEm->Damage_no;
		pEm->Damage_no = Damage_no;
		if (G.Key & 0x40)
			Dm_eff_blood(pEm, 1, 0, 6, 0x1800, 3);
		pEm->Damage_no = Damage_old;
		if (EF(pEm)->Dm_eff_rtn == 10) EF(pEm)->Dm_eff_func = 0;
	}
	EF(pEm)->Dm_eff_rtn++;

}
static void Pl_dm_set(ENEMY_WORK *pEm)
{
	register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;	/* inferred name (cf. Pl_dm_set_push): the compiler deletes it, retail SYM has no record */

	G.Player.pEnemy_ptr = (unsigned long *)pEm;

	*(unsigned long *)&G.Player.Routine_0 = 2;
	G.Player.Routine_1 += Cdir_ck(pEm, pPl);
}
static long Pl_dm_set_push(ENEMY_WORK *pEm, short Old_x, short Old_z, unsigned long Parts_no)
{
	register ENEMY_WORK *pPl;

	pPl = (ENEMY_WORK *)&G.Player;

	*(unsigned long *)&G.Player.Routine_0 = 0x105;

	if (Cdir_ck(pEm, pPl)) {

		Goto01(pPl, pEm->Cdir_y, 0x200);
		G.Player.Routine_1++;
	}else {

		Goto01(pPl, pEm->Cdir_y - 0x800, 0x200);
	}

	pPl->pEnemy_ptr = (unsigned long *)pEm;
	pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
	pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
}
static void Dm_eff_spark(ENEMY_WORK *pEm)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	register MATRIX *pMat;
	static DM_ESP2 *pEsp2;
	register unsigned long Next;	/* inferred name (cf. EM23.C): jump threading removes it, retail SYM has no record */

	if (!EF(pEm)->Dm_eff_rtn) {

		pEsp2 = Spark_tbl;
		EF(pEm)->Dm_eff_func = Dm_eff_spark;
		EF(pEm)->Dm_eff_rtn++;
	}else {

		do {	if (!(pEsp2->Next_flg & 2)) {
				pMat = &pParts[pEsp2->Parts].Workm;
				Esp_call(0x6001400 | Rnd() << 1, 0, pMat, pEsp2);
			}
			if (pEsp2->Next_flg & 1) Next = 1; else Next = 0;

			pEsp2++;
		}while (Next);
		if (EF(pEm)->Dm_eff_rtn == 21) EF(pEm)->Dm_eff_func = 0;
		EF(pEm)->Dm_eff_rtn++;
	}
}
static void Set_flame(ENEMY_WORK *pEm)
{
	register unsigned long i;
	register DM_ESP *pEsp;	register PARTS_W *pParts;
	pParts = pEm->pSin_parts_ptr;

	for (i = 0; i < 14; i++) {
		pEsp = &Flame_tbl[i];
		Esp_call2(pEsp->Dir_y | 0x5030000, pEm->Cdir_y << 16 | (i + 0x90), &pParts[pEsp->Parts].Workm, pEsp);
	}
}
static void Em_explosion_set(ENEMY_WORK *pEm, unsigned long Parts_no)
{
	register PARTS_W *pP_ptr = &pEm->pSin_parts_ptr[Parts_no];

	pP_ptr->Be_flg |= 0x4A;
	pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
	*(unsigned short *)&pP_ptr->Free[4] = 0x8000;	/* stored as unsigned short (ori, not addiu) */
	pP_ptr->Free[2] = 0;
	pP_ptr->Free[1] = -10;
	pP_ptr->Free[3] = 0;
	pP_ptr->Free[6] = 0;
	pP_ptr->Poly_rgb = 0x202038;
}
static void Bomb_tbl_set(void)
{
	register unsigned long i;

	for (i = 0; i < 30; i++)
		Bomb_tbl[0][i] *= 6;

}
static void Bomb_tbl_reset(void)
{
	register unsigned long i;

	for (i = 0; i < 30; i++)
		Bomb_tbl[0][i] /= 6;

}
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	pParts[Num].Poly_rgb = (pParts[Num].Poly_rgb & 0xFF000000) | (b << 16 | g << 8 | r);
}
static void Pl_launcher_set(ENEMY_WORK *pEm, unsigned long Dis)
{
	SVECTOR Svec;

	Svec.vx = Dis;
	Svec.vy = 0;
	Svec.vz = 0;
	Rot_vector(pEm->Cdir_y, &Svec, &Svec);
	Svec.vx += pEm->Pos_x;
	Svec.vz += pEm->Pos_z;

	G.Player.Pos_x = Svec.vx;
	G.Player.Pos_z = Svec.vz;
	G.Player.Cdir_y = Direction_ck(Svec.vx, Svec.vz, pEm->Pos_x, pEm->Pos_z) - 50;

}
static void Add_spdXZ(ENEMY_WORK *pEm, long Dir)
{
	SVECTOR Sv;
	MATRIX M;

	Sv.vx = 0;
	Sv.vy = Dir;
	Sv.vz = 0;
	RotMatrix(&Sv, &M);
	ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
	pEm->Pos_x += Sv.vx;
	pEm->Pos_z += Sv.vz;
}
