static JH_TBL Ty_jh0_tbl[34] = {
    { 0, 4096 }, { 1, 4096 }, { 2, 4096 }, { 3, 4096 }, { 4, 4096 }, { 5, 4096 },
    { 6, 4096 }, { 7, 4096 }, { 8, 4096 }, { 9, 4096 }, { 10, 4096 }, { 11, 4096 },
    { 12, 4096 }, { 13, 4096 }, { 28, 4096 }, { 29, 4096 }, { 36, 4096 }, { 37, 4096 },
    { 40, 4096 }, { 41, 4096 }, { 46, 4096 }, { 47, 4096 }, { 50, 4096 }, { 55, 4096 },
    { 84, 4096 }, { 85, 4096 }, { 86, 4096 }, { 87, 4096 }, { 88, 4096 }, { 89, 4096 },
    { 90, 4096 }, { 91, 4096 }, { 24, 4096 }, { 42, 4096 }
};
static JH_HEAD Ty_jh0_head[1] = {
    { 0, 1, 34, 0, 0, 0, 0, { 0, 0, 0, 0 }, 0, 0, 0, 0, { 26, -42, 335, 0 }, Ty_jh0_tbl }
};
static JH_TBL Ty_jh1_tbl[38] = {
    { 21, 4096 }, { 22, 4096 }, { 23, 4096 }, { 31, 4096 }, { 34, 4096 }, { 48, 4096 },
    { 54, 4096 }, { 74, 4096 }, { 75, 4096 }, { 76, 4096 }, { 65, 2048 }, { 66, 2048 },
    { 67, 2048 }, { 68, 2048 }, { 69, 2048 }, { 70, 2048 }, { 71, 2048 }, { 72, 0 },
    { 73, 0 }, { 77, 0 }, { 14, 0 }, { 17, 0 }, { 19, 0 }, { 32, 0 },
    { 38, 0 }, { 49, 0 }, { 53, 0 }, { 92, 0 }, { 93, 0 }, { 94, 0 },
    { 95, 0 }, { 96, 0 }, { 97, 0 }, { 98, 0 }, { 99, 0 }, { 100, 0 },
    { 101, 0 }, { 102, 0 }
};
static JH_HEAD Ty_jh1_head[1] = {
    { 1, 2, 38, 0, 0, 0, 0, { 26, -42, 335, 0 }, 0, 0, 0, 0, { 17, -994, 315, 0 }, Ty_jh1_tbl }
};
static JH_TBL Ty_jh2_tbl[38] = {
    { 25, 4096 }, { 26, 4096 }, { 27, 4096 }, { 30, 4096 }, { 35, 4096 }, { 45, 4096 },
    { 51, 4096 }, { 78, 4096 }, { 79, 4096 }, { 80, 4096 }, { 58, 2048 }, { 59, 2048 },
    { 60, 2048 }, { 61, 2048 }, { 62, 2048 }, { 63, 2048 }, { 64, 2048 }, { 16, 0 },
    { 18, 0 }, { 20, 0 }, { 33, 0 }, { 39, 0 }, { 44, 0 }, { 52, 0 },
    { 81, 0 }, { 82, 0 }, { 83, 0 }, { 103, 0 }, { 104, 0 }, { 105, 0 },
    { 106, 0 }, { 107, 0 }, { 108, 0 }, { 109, 0 }, { 110, 0 }, { 111, 0 },
    { 112, 0 }, { 113, 0 }
};
static JH_HEAD Ty_jh2_head[1] = {
    { 4, 5, 38, 0, 0, 0, 0, { 26, -42, -335, 0 }, 0, 0, 0, 0, { 17, -994, -315, 0 }, Ty_jh2_tbl }
};
static JH_TBL Ty_jh3_tbl[2] = {
    { 56, 2048 }, { 57, 2048 }
};
static JH_HEAD Ty_jh3_head[1] = {
    { 1, 4, 2, 0, 0, 0, 0, { 26, -42, 335, 0 }, 0, 0, 0, 0, { 26, -42, -335, 0 }, Ty_jh3_tbl }
};
static JH_TBL Ty_jh4_tbl[2] = {
    { 15, 2048 }, { 43, 2048 }
};
static JH_HEAD Ty_jh4_head[1] = {
    { 2, 5, 2, 0, 0, 0, 0, { 17, -994, 315, 0 }, 0, 0, 0, 0, { 17, -994, -315, 0 }, Ty_jh4_tbl }
};
static DM_ESP Blood_tbl[36] = {
    { 200, 400, -200, 16384, 0 },
    { 200, 600, 300, 16384, 0 },
    { 200, 900, 0, 16384, 0 },
    { 200, 700, -100, 16384, 0 },
    { 200, 1100, 500, 16384, 0 },
    { 200, 1100, -300, 16384, 0 },
    { 200, 100, 200, 16384, 8 },
    { 200, 200, -100, 16384, 0 },
    { 200, 0, 100, 16384, 8 },
    { 200, 0, 100, 16384, 0 },
    { 200, -100, 400, 16000, 8 },
    { 300, 200, 100, 16405, 11 },
    { 200, -300, 200, 16384, 8 },
    { 200, -300, -100, 16384, 8 },
    { 200, -200, -300, 16384, 8 },
    { 200, 0, -100, 16384, 8 },
    { 100, 200, 200, 16000, 13 },
    { 300, -58, -59, 16406, 9 },
    { -300, 500, 200, 18500, 0 },
    { -300, 700, -200, 18500, 0 },
    { -300, 900, 0, 18500, 0 },
    { -300, 1100, -400, 18500, 0 },
    { -300, 600, -500, 18500, 0 },
    { -300, 1100, 400, 18500, 0 },
    { -300, -200, -100, 18500, 8 },
    { -300, 0, 200, 18500, 8 },
    { -300, 100, -100, 18500, 8 },
    { -300, -100, 100, 18500, 0 },
    { -300, 100, -300, 18500, 0 },
    { -300, 300, 0, 18500, 0 },
    { -200, -300, 200, 18300, 8 },
    { -150, -200, -100, 18300, 8 },
    { -200, 0, -200, 18300, 8 },
    { -120, -200, 300, 18500, 8 },
    { -100, 300, -100, 18500, 10 },
    { -150, -100, -100, 18300, 9 }
};
static DM_ESP Flame_tbl[7] = {
    { 330, 100, 200, 6834, 8 },
    { 290, -300, -300, 5734, 8 },
    { 300, 0, 0, 6476, 2 },
    { -400, 0, 0, 6476, 2 },
    { -300, -400, 0, 6476, 6 },
    { 300, -100, 0, 6476, 5 },
    { 100, 100, 0, 5256, 13 }
};
static DM_ESP2 Spark_tbl[24] = {
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { -500, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 300, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { -500, 100, 0, 0, 5, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { -400, -500, 100, 0, 7, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 350, -400, 100, 0, 8, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, -100, 0, 10, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 300, -200, 0, 0, 3, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 350, 500, 100, 0, 4, 0 },
    { 0, 0, 0, 0, 0, 2 }
};

static unsigned long Ty_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top)
{

    Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, Ty_jh0_head, Mem_top);
    Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, Ty_jh1_head, Mem_top);
    Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, Ty_jh2_head, Mem_top);
    Mem_top = Jh_set(pEm, pEm->pSin_parts_ptr, Ty_jh3_head, Mem_top);
    return Jh_set(pEm, pEm->pSin_parts_ptr, Ty_jh4_head, Mem_top); }

static void Ty_Jh_move(ENEMY_WORK *pEm)
{

    VSync(1);
    MulJh(pEm, pEm->pSin_parts_ptr, Ty_jh0_head);
    MulJh(pEm, pEm->pSin_parts_ptr, Ty_jh1_head);
    MulJh(pEm, pEm->pSin_parts_ptr, Ty_jh2_head);
    MulJh(pEm, pEm->pSin_parts_ptr, Ty_jh3_head);
    MulJh(pEm, pEm->pSin_parts_ptr, Ty_jh4_head);
    VSync(1);

    Jh_sort(pEm, pEm->pSin_parts_ptr);

}

static unsigned long Em2a_Att_ck(ENEMY_WORK *pEm, unsigned long Parts_no, unsigned short Start, unsigned short End, unsigned long At_dis)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long Dis;

    if (G.Player.Life < 0) return 0; /* retail-only (bltz @0x801002F4) */
    Dis = SquareRoot0((G.Player.Pos_x - pParts[Parts_no].Workm.t[0]) * (G.Player.Pos_x - pParts[Parts_no].Workm.t[0]) + (G.Player.Pos_z - pParts[Parts_no].Workm.t[2]) * (G.Player.Pos_z - pParts[Parts_no].Workm.t[2]));
    EF(pEm)->Pold_x = pParts[Parts_no].Workm.t[0];
    EF(pEm)->Pold_z = pParts[Parts_no].Workm.t[2];
    if (pEm->Move_cnt >= Start && pEm->Move_cnt <= End) {

        if (!pEm->Timer0 && Dis <= At_dis) {
            pEm->Timer0 = 1;
            return 1;
        }
    }
    return 0;
}

static unsigned long Att_ck2(ENEMY_WORK *pEm, unsigned long Parts_no, unsigned short Start, unsigned short End, unsigned long At_dis, unsigned long At_w)
{

    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long Dis;
    register long Dir0, Dir1, Dir2;
    register short Old_x, Old_z;

    if (G.Player.Life < 0) return 0; /* retail-only (bltz @0x80100424) */
    Old_x = EF(pEm)->Pold_x;
    Old_z = EF(pEm)->Pold_z;
    EF(pEm)->Pold_x = pParts[Parts_no].Workm.t[0];
    EF(pEm)->Pold_z = pParts[Parts_no].Workm.t[2];
    if (pEm->Move_cnt < Start || pEm->Move_cnt > End) return 0;

        Dis = SquareRoot0((pEm->Pos_x - pParts[Parts_no].Workm.t[0]) * (pEm->Pos_x - pParts[Parts_no].Workm.t[0]) + (pEm->Pos_z - pParts[Parts_no].Workm.t[2]) * (pEm->Pos_z - pParts[Parts_no].Workm.t[2]));
        Dis += SquareRoot0((pEm->Pos_x - Old_x) * (pEm->Pos_x - Old_x) + (pEm->Pos_z - Old_z) * (pEm->Pos_z - Old_z)) - -(At_w + 500); Dis /= 2; /* written as - -(...): GCC then keeps At_w + 500 as one operand, as retail does */

        if (!pEm->Timer0 && pEm->L_pl < Dis) {

            Dir0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, Old_x, Old_z);
            Dir1 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pParts[Parts_no].Workm.t[0], pParts[Parts_no].Workm.t[2]);

            if (Dir1 < Dir0) {
                Dir2 = Dir0; Dir0 = Dir1; Dir1 = Dir2;
            }

            Dir1 = (Dir1 - Dir0) & 0xFFF;
            if (Dir1 >= 0x800) {
                Dir0 = Dir1 + Dir0 - 0x1000;
                Dir1 = 0x1000 - Dir1;
            }
            Dir0 -= At_dis;
            Dir1 += At_dis * 2;

            Dir2 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
            Dir2 = (Dir2 - Dir0) & 0xFFF;

            if (Dir2 > 0 && Dir2 < Dir1) {

                pEm->Timer0 = 1;
                return 1;
            }
        }
    return 0;
}

static void Em2a_add_dm(unsigned long Att)
{
    register long Pl_life; /* inferred name (cf. EM23.C): the compiler deletes it, retail SYM has no record */

    Pl_life = Pl_life_down(Att, 0);

    if (Pl_life & 2)
        *(unsigned long *)&G.Player.Routine_0 = 3;

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

static long Get_dir_pl(ENEMY_WORK *pEm)
{
    register short Dir;

    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
    Dir = (Dir - pEm->Cdir_y) & 0xFFF;
    if (Dir > 0x800) Dir |= 0xF000;

    return Dir;
}

static long Pl_dm_set_normal(ENEMY_WORK *pEm)
{
    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player; /* inferred name (cf. Pl_dm_set): the compiler deletes it, retail SYM has no record */

    *(unsigned long *)&G.Player.Routine_0 = 2;

    G.Player.Routine_1 += Cdir_ck(pPl, pEm);
    G.Player.pEnemy_ptr = (unsigned long *)pEm;
}

static long Pl_dm_set(ENEMY_WORK *pEm, SVECTOR *pSv, unsigned long Parts_no)
{
    register PARTS_W *pParts = &pEm->pSin_parts_ptr[Parts_no];

    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;
    register short Dir; /* inferred name: the compiler deletes it, retail SYM has no record */

    *(unsigned long *)&G.Player.Routine_0 = 5;

    Dir = Direction_ck(pSv->vx, pSv->vz, G.Player.Old_x, G.Player.Old_z);

    if (((Dir - G.Player.Cdir_y + 0x400) & 0xFFF) < 0x800) {

        Goto01(pPl, pEm->Cdir_y, 0x200);
        pSv->vx = G.Player.Pos_x + (pParts->Workm.t[0] - pSv->vx);

        pSv->vz = G.Player.Pos_z + (pParts->Workm.t[2] - pSv->vz);

        G.Player.Routine_1++;
    } else {

        Goto01(pPl, pEm->Cdir_y - 0x800, 0x200);
        pSv->vx = G.Player.Pos_x - (pParts->Workm.t[0] - pSv->vx);

        pSv->vz = G.Player.Pos_z - (pParts->Workm.t[2] - pSv->vz);
    }

    pPl->pEnemy_ptr = (unsigned long *)pEm;
    pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
    pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
}

static unsigned long Dm_eff(ENEMY_WORK *pEm)
{

    /* retail (jtbl_80100004): new 0x1600 blood for weapons 5/6, other sizes/zooms changed */
    EF(pEm)->Dm_eff_rtn = 0; switch (EF(pEm)->Dm_wep) {
    case 1:
    case 12:
    case 17:
    case 19:
        break;

    case 2: case 3: case 4:
        Dm_eff_blood(pEm, 1, 0, 6, 0x1000, 3);
        break;
    case 5: case 6:
        Dm_eff_blood(pEm, 1, 0, 6, 0x1600, 3);
        break;
    case 7:
        Dm_eff_blood(pEm, 4, 0, 6, 0x1500, 3);
        Dm_eff_blood(pEm, 2, 0, 6, 0x1400, 2);
        break;
    case 8:
        Dm_eff_blood(pEm, 4, 0, 6, 0x1500, 3);
        Dm_eff_blood(pEm, 4, 0, 6, 0x1400, 2);
        break;
    case 9:
        Dm_eff_blood(pEm, 5, 5, 4, 0x1000, 3);
        break;
    case 10:
        Dm_eff_flame(pEm);

        break;
    case 11:
        Dm_eff_blood(pEm, 5, 4, 9, 0x1500, 3);
        break;

    case 13:
        Dm_eff_blood(pEm, 1, 0, 6, 0x1000, 3);
        break;
    case 14:
        Dm_eff_spark(pEm);
        break;

    case 16:
        Dm_eff_blood(pEm, 5, 5, 4, 0x1600, 3);
        break;

    case 15: case 18:
        Dm_eff_0f(pEm);

    } }

static void Dm_eff_blood(ENEMY_WORK *pEm, unsigned long No, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Atr)
{
    register DM_ESP *pEsp;
    SVECTOR Svec;
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long Ofs, Mask, i; register MATRIX *pMat; /* pMat: inferred name (cf. Dm_eff_spark), the compiler deletes it, retail SYM has no record */
    Ofs = 0;

    Mask = 36;

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
        pMat = &pParts[pEsp->Parts].Workm;

        Esp_call(Id << 24 | Type << 16 | Zoom, pEm->Cdir_y + pEsp->Dir_y, pMat, pEsp);
    }
}

static void Dm_eff_0f(ENEMY_WORK *pEm)
{
    /* retail: Damage_no/Damage_old are per-enemy EM2A_FREE bytes, not function statics */
    switch (EF(pEm)->Dm_eff_rtn) {
    case 0:
        EF(pEm)->Damage_no = pEm->Damage_no;
        Dm_eff_blood(pEm, 1, 0, 6, 0x1000, 3);
        EF(pEm)->Dm_eff_func = Dm_eff_0f;
        EF(pEm)->Dm_eff_rtn++;
        return;

    case 4: case 7: case 10:
        EF(pEm)->Damage_old = pEm->Damage_no;
        pEm->Damage_no = EF(pEm)->Damage_no;
        if (G.Key & 0x40)
            Dm_eff_blood(pEm, 1, 0, 6, 0x1000, 3);
        pEm->Damage_no = EF(pEm)->Damage_old;
        if (EF(pEm)->Dm_eff_rtn == 10) EF(pEm)->Dm_eff_func = 0;
    }
    EF(pEm)->Dm_eff_rtn++;

}

static void Dm_eff_disp(ENEMY_WORK *pEm, unsigned long No, unsigned long Id, unsigned long Type, unsigned long Zoom, DM_ESP *pEsp)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register MATRIX *pMat;

    while (No-- > 0) {
        pMat = &pParts[pEsp->Parts].Workm;
        Esp_call(Id << 24 | Type << 16 | (Zoom + Rnd() * 2), pEm->Cdir_y + pEsp->Dir_y, pMat, pEsp);
        pEsp++;
    }
}

static void Dm_eff_flame(ENEMY_WORK *pEm)
{
    register DM_ESP *pEsp;
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long No;
    register MATRIX *pMat; /* inferred name (cf. Dm_eff_disp): the compiler deletes it, retail SYM has no record */

    pEsp = Flame_tbl;
    for (No = 0; No < 7; No++) {
        pMat = &pParts[pEsp->Parts].Workm;
        Esp_call(0x5040000 | pEsp->Dir_y, pEm->Cdir_y, pMat, pEsp);
        pEsp++;
    }
}

static void Dm_eff_spark(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register MATRIX *pMat;
    register unsigned long Next; /* inferred name (cf. EM23.C): jump threading removes it, retail SYM has no record */

    if (!EF(pEm)->Dm_eff_rtn) {

        EF(pEm)->pEsp2 = Spark_tbl; /* retail: per-enemy pointer, not a static */
        EF(pEm)->Dm_eff_func = Dm_eff_spark;
        EF(pEm)->Dm_eff_rtn++;
    } else {

        do {
            if (!(EF(pEm)->pEsp2->Next_flg & 2)) {
                pMat = &pParts[EF(pEm)->pEsp2->Parts].Workm;
                Esp_call(0x6001000 | Rnd() << 1, 0, pMat, EF(pEm)->pEsp2);
            }
            if (EF(pEm)->pEsp2->Next_flg & 1) Next = 1; else Next = 0;

            EF(pEm)->pEsp2++;
        } while (Next);
        if (EF(pEm)->Dm_eff_rtn == 0x18) EF(pEm)->Dm_eff_func = 0;
        EF(pEm)->Dm_eff_rtn++;
    }
}

static void Goto00_r(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir)
{
    register short Dir;
    register unsigned short Hani;

    Dir = (Direction_ck(pEm->Pos_x, pEm->Pos_z, Vec_x, Vec_z) + 0x800) & 0xFFF;

    if (Add_dir < 0) {
        Add_dir = -Add_dir;
        Dir = (Dir + 0x800) & 0xFFF;
    }

    Hani = Dir - pEm->Cdir_y + Add_dir;
    if ((Hani & 0xFFF) < Add_dir * 2)

        pEm->Cdir_y = Dir;
    else {

        pEm->Cdir_y -= Add_dir;

        if ((Hani & 0xFFF) <= 0x800)
            pEm->Cdir_y += Add_dir * 2;
    }

}

static long Sca_escape(ENEMY_WORK *pEm)
{
    register short Dir;
    register SCA_DATA *pSca = pEm->pSca_hit_data;

    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pSca->X + (pSca->W >> 1), pSca->Z + (pSca->D >> 1));

    Dir = (Dir - pEm->Cdir_y) & 0xFFF;
    if (Dir > 0x800) Dir |= 0xF000;

    if (Dir > 0) return -1;

    return 1;
}
