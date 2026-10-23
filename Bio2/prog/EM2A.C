/* em2a.c - reconstruction of D:\Bio2\prog\EM2A.C, the EM2A enemy module.  One source,
 * linked into two runtime overlays: 0x0F (enemy bin slot 0, 0x80100180..) and 0x2D
 * (slot 1, 0x8010D180..); the gate compiles it once per overlay with -DOVL_0F /
 * -DOVL_2D, which only selects the exported names (em2a_sub.h).  Retail EM2A.C
 * #includes EM2A_SUB.C between its life tables and its own tables (one object, two
 * SLD files).  Types come from the object's retail SYM preamble (em2a.h); the tables
 * are the retail initialized data.  Engine routines are called undeclared, as in the
 * original: their implicit declarations open the retail debug blocks. */
#include "EM2A.H"

static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
/* Retail life tables (em2a_cdemd0_s1 .data 0x801047CC..0x8010482B): the Em2b pair changed
 * relative to Beta 2; a second complete set (0x801047FC..) is selected by G.System_flg & 0x40
 * (Em2a_Init @0x801013B4). Second-set names are [INFERRED], following src-retail/prog/em20.c. */
static unsigned short Em2b_life = 250;
static unsigned short Em2b_life_easy = 200;
static unsigned short Em23_life_flg40 = 240;
static unsigned short Em23_life_easy_flg40 = 240;
static unsigned short Em2a_life_flg40[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy_flg40[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life_flg40 = 250;
static unsigned short Em2b_life_easy_flg40 = 200;

/* forward declarations: the retail SYM lists these objects first-declared here (defined below). */
static void (*Em2a_routine_0[8])();
static void (*Em2a_mv_br[19])();
static void (*Em2a_mv[19])();
static void (*Em2a_damage[5])();
static void (*Em2a_die[])();
static void (*Em2a_pldm_tbl[3])();
static DM_ESP Blood_tbl[36];
static DM_ESP Flame_tbl[7];
static DM_ESP2 Spark_tbl[24];
#include "EM2A_SUB.C"

static unsigned char Em2a_sce_rtn[8] = { 9, 10, 11, 12, 13, 14, 15, 17 };
static void (*Em2a_routine_0[8])() = {
    Em2a_Init,
    Em2a_Move,
    Em2a_Damage,
    Em2a_Die,
    Em2a_Scenario,
    0,
    0,
    Em2a_Dead
};
static void (*Em2a_mv_br[19])() = {
    Em2a_taiki_br,
    Em2a_walk_br,
    Em2a_walkf_br,
    Em2a_att_c_br,
    Em2a_att2_r_br,
    Em2a_att2_l_br,
    Em2a_att_w_br,
    Em2a_att_t_br,
    Em2a_att_cf_br,
    Em2a_117_br,
    Em2a_30a_br,
    Em2a_602_br,
    Em2a_507_br,
    Em2a_604l_br,
    Em2a_604c_br,
    Em2a_20d_br,
    Em2a_cri_br,
    Em2a_612_br,
    Em2a_where_br	/* retail-only entry 18 (D_80104E58) */
};
static void (*Em2a_mv[19])() = {
    Em2a_taiki,
    Em2a_walk,
    Em2a_walkf,
    Em2a_att_c,
    Em2a_att2_r,
    Em2a_att2_l,
    Em2a_att_w,
    Em2a_att_t,
    Em2a_att_cf,
    Em2a_117,
    Em2a_30a,
    Em2a_602,
    Em2a_507,
    Em2a_604l,
    Em2a_604c,
    Em2a_20d,
    Em2a_cri,
    Em2a_612,
    Em2a_where	/* retail-only entry 18 (D_80104EA4) */
};
static void (*Em2a_damage[5])() = {
    Em2a_dm_f,
    Em2a_dm_b,
    0,
    Em2a_dm_down,
    Em2a_dm_no
};
static void (*Em2a_die[])() = {};
static void (*Em2a_pldm_tbl[3])() = {
    Em2a_pl_dm,
    Em2a_pl_dm_b,
    Em2a_pl_dm_c
};

void Em12a(ENEMY_WORK *pEm)
{

    Root_ck(pEm, &G.Player.Pos_x, 0, 0);

    if (G.Stop_flg & 0x20000000) return;

    if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

    Em2a_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

    if (EF(pEm)->LRpunch_wait) EF(pEm)->LRpunch_wait--;
    if (EF(pEm)->Cfpunch_wait) EF(pEm)->Cfpunch_wait--;
    if (EF(pEm)->Retail_14) EF(pEm)->Retail_14--; /* retail-only countdowns @0x80101238, 0x8010124C */
    if (EF(pEm)->Retail_15) EF(pEm)->Retail_15--;

    if (EF(pEm)->Dm_eff_func) ((void (*)())EF(pEm)->Dm_eff_func)(pEm);

    if (!(EF(pEm)->Attribute & 0x80)) Em_seq_sound(pEm);

    Oba_ck_em(pEm);
    Sca_ck_em(pEm, 0x400);

    EF(pEm)->Old_dis = SquareRoot0((pEm->Pos_x - pEm->Old_x) * (pEm->Pos_x - pEm->Old_x) + (pEm->Pos_z - pEm->Old_z) * (pEm->Pos_z - pEm->Old_z));

}

static void Em2a_after(ENEMY_WORK *pEm)
{
    Ty_Jh_move(pEm); }

static void Em2a_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long St_room;
    register unsigned short *pLife; /* [INFERRED] name (retail-only): the selected life table */
    register unsigned long *Bak; register PARTS_NECK_FREE_WORK *pNeck; /* pNeck: inferred name (cf. EM23.C), GCC folds it into the store offsets, retail SYM has no record */

    if ((pEm->Type & 0xF) != 7) {
        Bak = pEm->pSub0_kan_t_ptr;
        pEm->pSub0_kan_t_ptr = pEm->pKan_t_ptr;
        pEm->pKan_t_ptr = Bak;
        Kan = (unsigned long)Bak;
        Bak = pEm->pSub0_seq_t_ptr;
        pEm->pSub0_seq_t_ptr = pEm->pSeq_t_ptr;
        pEm->pSeq_t_ptr = Bak;
        Seq = (unsigned long)Bak;
    }

    G.pPldamage_Em[0x2A - 0x10] = Em12a_Pldamage;

    EF(pEm)->Attribute = 0;

    *(unsigned long *)&pEm->Routine_0 = 0x101;

    *(unsigned long *)&pEm->Move_no = 0xFF;

    if (pEm->Type & 0x4000)
        pEm->Routine_1 = Em2a_sce_rtn[pEm->Type & 0xF];

    /* retail (0x801013B4..0x80101400): the table is chosen once, incl. the second (0x40) set */
    if (G.System_flg & 0x40)
        pLife = (G.System_flg & 0x20) ? Em2a_life_easy_flg40 : Em2a_life_flg40;
    else
        pLife = (G.System_flg & 0x20) ? Em2a_life_easy : Em2a_life;

    switch ((pEm->Type & 0x300) >> 8) {
    case 0:
        pEm->Life = 20250;
        break;

    case 1:
        EF(pEm)->Attribute |= 6;
        pEm->Life = pLife[0] + 20000;
        break;

    case 2:
        EF(pEm)->Attribute |= 0x4B;
        pEm->Life = pLife[1] + 20000;
        break;

    case 3:
        EF(pEm)->Attribute |= 0x1A;
        pEm->Life = pLife[2] + 20000;
    }

    St_room = (G.SaveData.Stage_no + 1) * 256 + G.SaveData.Room_no;
    if (St_room == 0x117) {
        pEm->Life = pLife[3] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x100; /* retail: & ~0x300 (andi 0xFCFF) */
    }
    if (St_room == 0x10D) {
        pEm->Life = pLife[4] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x100; /* retail: & ~0x300 (andi 0xFCFF) */
    }
    if (St_room == 0x507) {
        pEm->Life = pLife[5] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x100; /* retail: & ~0x300 (andi 0xFCFF) */
    }
    if (St_room == 0x109) {
        pEm->Life = pLife[6] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x100; /* retail: & ~0x300 (andi 0xFCFF) */
    }
    if (St_room == 0x20D) {
        pEm->Life = pLife[7] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x100; /* retail: & ~0x300 (andi 0xFCFF) */
    }
    if (St_room == 0x20C) {
        pEm->Life = pLife[8] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x200; /* retail: & ~0x300 */
    }
    if (St_room == 0x602) {
        pEm->Life = pLife[9] + 20000;
        pEm->Type = (pEm->Type & ~0x300) | 0x200; /* retail: & ~0x300 */
    }

    pEm->Be_flg = 1;
    pEm->Damage_flg = 0;
    pEm->Sce_flg = 0;
    pEm->Down_cnt = 0;
    EF(pEm)->Pold_x = EF(pEm)->Pold_z = 0;
    EF(pEm)->Dm_eff_rtn = 0;

    EF(pEm)->Dm_eff_func = 0;
    EF(pEm)->LRpunch_wait = 0;
    EF(pEm)->Cfpunch_wait = 0;
    EF(pEm)->Retail_14 = 0; /* retail-only, sb @0x80101618 / 0x8010161C */
    EF(pEm)->Retail_15 = 0;

    G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
    G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

    pNeck = (PARTS_NECK_FREE_WORK *)pEm->pSin_parts_ptr[9].Free;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 9;
    pEm->pEnemy_neck = (unsigned long *)&G.Player;
    pNeck->N_rot_y = 0;
    pNeck->N_rot_z = 0;
    pNeck->Add_dir_y = 20;
    pNeck->Add_dir_z = 20;
    pNeck->L_rot_y = 0x230;
    pNeck->L_rot_z = 0x138;

    *(unsigned long *)&pEm->Move_no = 1;

    Joint_move(pEm, Kan, Seq, 0x100);

    pParts->Be_flg |= 0x4000;
    pEm->pTafter_func = Em2a_after;

    G.pFree_work = (unsigned char *)Ty_Jh_init(pEm, (unsigned long)G.pFree_work);

    pEm->nOba = 1;
    pEm->Atd[0].Ofs_x = 0;
    pEm->Atd[0].Ofs_y = -0x6A4;
    pEm->Atd[0].Ofs_z = 0;
    pEm->Atd[0].At_w = pEm->Atd[0].W = 0x2BC; /* retail: 0x320 -> 0x2BC (li @0x801016E0) */
    pEm->Atd[0].At_h = 0x6A4;
    pEm->Atd[0].At_d = pEm->Atd[0].D = 0x2BC;
    pEm->Eff_at_r = 0x320;

    pEm->Be_flg |= 0x1C000000;

    Kage_work_set(&pEm->pKage_work, 0, 0x2EE0226, 0, &pEm->Pos_x);

}

static void Em2a_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em2a_mv_br[pEm->Routine_1](pEm);
    Em2a_mv[pEm->Routine_1](pEm, Kan, Seq);

    Mot_play_init(pEm, EF(pEm)->Rno_bak);
    EF(pEm)->Rno_bak = *(unsigned long *)&pEm->Routine_0;
}

static void Em2a_brain(ENEMY_WORK *pEm)
{
    register short Dir, Dir2;
    SVECTOR Svec;
    VECTOR Vec;

    /* retail: every attack choice also requires !G.Player.Damage_cnt (lbu G.Player+0x1D3), and the
       Retail_14/Retail_15 timers block re-attacks; the low-life threshold is 0x33 (Beta 0x29) */
    if (G.Player.Routine_0 == 5)
        if (G.Player.Routine_1 == 1) return;

    if (G.Player.Life < 0) return;

    Dir2 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

    Dir = Get_dir_pl(pEm);

    if (abs(Dir) < 0x2AA && pEm->L_pl < 0x7D0) {

        if (G.Player.Life >= 0x47) goto next;
        Svec.vx = G.Player.Atd[0].At_w + 0x12C;
        Svec.vz = 0;
        Rot_vector(Dir2, &Svec, &Svec);
        Vec.vx = Svec.vx + G.Player.Pos_x;
        Vec.vz = Svec.vz + G.Player.Pos_z;
        if (!G.Player.Damage_cnt && Sca_ck_hit(&Vec, 0, 1 << G.Player.nFloor, 0x8000)) {
            *(unsigned long *)&pEm->Routine_0 = 0x601;
            return;
        }
    }

next:
    if (G.Player.Life < 0x33 && !EF(pEm)->Retail_15)
        if (abs(Dir) < 0x2AA && pEm->L_pl < 0x898) {
            if (!G.Player.Damage_cnt) *(unsigned long *)&pEm->Routine_0 = 0x301;
            return;
        }

    if (EF(pEm)->LRpunch_wait && !EF(pEm)->Retail_14)
        if (abs(Dir) < 0x2AA && pEm->L_pl < 0x898) {
            if (!G.Player.Damage_cnt) *(unsigned long *)&pEm->Routine_0 = 0x601;
            return;
        }

    if (G.Player.Life >= 0x29) {

        if (!EF(pEm)->LRpunch_wait && pEm->L_pl < 0x898) {

            if (!G.Player.Damage_cnt && abs(Dir) < 0x320 && (abs(Dir) >= 0x24 || pEm->L_pl < 0x640)) {
                if ((EF(pEm)->Attribute & 0x40) && (Rnd() & 2))
                    EF(pEm)->Attribute &= ~0x20;
                else
                    EF(pEm)->Attribute |= 0x20;
                if (Dir > 0) {
                    *(unsigned long *)&pEm->Routine_0 = 0x401;
                    return;
                }

                *(unsigned long *)&pEm->Routine_0 = 0x501; return;

            }
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        } } else {
        if (!EF(pEm)->LRpunch_wait && pEm->L_pl < 0x7D0) {
            if (!G.Player.Damage_cnt && abs(Dir) < 0x1F4) {
                EF(pEm)->Attribute |= 0x20;
                if (Dir > 0) {

                    *(unsigned long *)&pEm->Routine_0 = 0x401; return;

                }

                *(unsigned long *)&pEm->Routine_0 = 0x501; return;

            } Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        }
    }

    if (!EF(pEm)->Cfpunch_wait && (pEm->Type & 0x300) != 0x100 && pEm->Routine_1 != 2 && G.Player.Life > 0x50 && (Rnd() & 0x1F) == 8 && !G.Player.Damage_cnt)

        if (pEm->L_pl > 0xC80 && pEm->L_pl < 0xE74 && abs(Dir) < 0xC8)
            *(unsigned long *)&pEm->Routine_0 = 0x801;

}
static void Em2a_taiki_br(ENEMY_WORK *pEm) {}

static void Em2a_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em2a_walk_br(ENEMY_WORK *pEm) {}

static void Em2a_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (G.Player.Life < 0) { /* retail-only (0x80101C48..0x80101C6C) */
        *(unsigned long *)&pEm->Routine_0 = 0x1201; return; }

    Em2a_brain(pEm);

    if ((pEm->Type & 0x300) == 0x100) {

        if (pEm->L_pl >= 0x1965) {

            *(unsigned long *)&pEm->Routine_0 = 0x201; return; }
    } else {

        if (pEm->L_pl >= 0x157D) {

            *(unsigned long *)&pEm->Routine_0 = 0x201; return; }
    }

    Get_dir_pl(pEm); /* retail-only, jal @0x80101CB4 (result unused) */
    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Timer1 = 0;
        pEm->Move_cnt = 0;
        pEm->Routine_2++;
    case 1:
        Mot_play_sp(pEm, 1, 0xF, 0);
        /* retail (0x80101D08..0x80101DBC): as in Em2a_walkf; thresholds 61/93 */
        if (pEm->pSca_hit_data) {
            if (!Goto01_ck(pEm, (short)((Sca_get_dir((VECTOR *)&pEm->Pos_x, pEm->pSca_hit_data) << 10) + 0x800), 0x50))
                pEm->Timer0++;
            else if (pEm->Timer0 < 0x10)
                pEm->Timer0 = 0;
        }

        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x30);

        if (pEm->Timer0 > 60) {
            pEm->Cdir_y += Sca_escape(pEm) << 5;
            if (pEm->Timer0 > 92) pEm->Timer0 = 0;
        }

    }
}

static void Em2a_walkf_br(ENEMY_WORK *pEm) {}

static void Em2a_walkf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em2a_brain(pEm);

    if (G.Player.Life >= 0)
        if (pEm->L_pl < 0xED8) {

            *(unsigned long *)&pEm->Routine_0 = 0x101; return; }

    Get_dir_pl(pEm); /* retail-only, jal @0x80101E20 (result unused) */
    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Timer1 = 0;
        pEm->Routine_2++;
    case 1:
        Mot_play_sp(pEm, 8, 0xF, 0);
        /* retail (0x80101E70..0x80101F24): blocked-wall counter from Sca_get_dir/Goto01_ck
           replaces the Old_dis / Sca_info test; thresholds 61/49, escape step << 5 */
        if (pEm->pSca_hit_data) {
            if (!Goto01_ck(pEm, (short)((Sca_get_dir((VECTOR *)&pEm->Pos_x, pEm->pSca_hit_data) << 10) + 0x800), 0x50))
                pEm->Timer0++;
            else if (pEm->Timer0 < 0x10)
                pEm->Timer0 = 0;
        }

        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);

        if (pEm->Timer0 > 60) {
            pEm->Cdir_y += Sca_escape(pEm) << 5;
            if (pEm->Timer0 > 48) pEm->Timer0 = 0;
        }

    }
}

static void Em2a_att_c_br(ENEMY_WORK *pEm) {}

static void Em2a_att_c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;
    register short Dir; /* inferred name: the compiler deletes it, retail SYM has no record */
    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
    case 1:
        Dir = Get_dir_pl(pEm);
        if (pEm->Move_cnt && pEm->Move_cnt < 0x34)
            if (abs(Dir) >= 0x353 || pEm->L_pl >= 0x1069) {
                pEm->Routine_2 = 3;
                EF(pEm)->Retail_15 = 30; /* retail-only, sb @0x80102018 */
                *(unsigned long *)&pEm->Routine_0 = 0x101;
                break;
            }

        pEm->Routine_2 += Mot_play_sp(pEm, 2, 0xF, 0);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 4);

        Svec.vx = EF(pEm)->Pold_x;
        Svec.vz = EF(pEm)->Pold_z;
        if (Em2a_Att_ck(pEm, 12, 0x38, 0x3B, 0x352)) { { SVECTOR Bpos;

            Snd_se_enem(2, pEm);
            Pad_vib_set0(3, 0); /* retail-only vibration (0x80102094..0x801020BC) */
            Pad_vib_set1(8, 255, 0);
            Pad_vib_fade_set(4, 255, 40, 8);

            Pl_dm_set(pEm, &Svec, 12);

            Bpos.vx = pParts[12].Workm.t[0];
            Bpos.vy = pParts[12].Workm.t[1];
            Bpos.vz = pParts[12].Workm.t[2];
            if (Bpos.vy < pEm->Ground - 0xBB8) Bpos.vy = pEm->Ground - 0xBB8;
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos); }

            /* retail (0x80102130..0x8010217C): normal damage; the grab only when it kills */
            Em2a_add_dm(50);
            if (G.Player.Life < 0) {
                G.Player.Life = -1;

                G.System_flg |= 0x4000000;
                G.System_flg |= 0x100;

                *(unsigned long *)&pPl->Routine_0 = 0x205;

                *(unsigned long *)&pEm->Routine_0 = 0x1001;
            }
            break;
        }
        break;

    case 3:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 4);

        pEm->Move_cnt = 0;
        Mot_play(pEm, 2, 0xF);

        if (--pEm->Timer0) break; case 2:
        *(unsigned long *)&pEm->Routine_0 = 0x101;

    }
}

static void Em2a_att2_r_br(ENEMY_WORK *pEm) {}

static void Em2a_att2_r(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;
    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Routine_2++;
    case 1:
        if (Mot_play_sp(pEm, 6, 3, 0)) {
            EF(pEm)->LRpunch_wait = 0x23;
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);

        /* retail: the Get_dir_pl turn-into-grab test of Beta 2 is removed */

        Svec.vx = EF(pEm)->Pold_x;
        Svec.vz = EF(pEm)->Pold_z;
        if (Att_ck2(pEm, 12, 5, 14, 0x258, pPl->Atd[0].At_w * 2)) { SVECTOR Bpos;

            Snd_se_enem(2, pEm);
            Pad_vib_set0(3, 0); /* retail-only vibration */
            Pad_vib_set1(5, 220, 0);
            Pad_vib_fade_set(4, 220, 40, 5);

            Pl_dm_set(pEm, &Svec, 12);

            Bpos.vx = pParts[12].Workm.t[0];
            Bpos.vy = pParts[12].Workm.t[1];
            Bpos.vz = pParts[12].Workm.t[2];
            if (Bpos.vy < pEm->Ground - 0xBB8) Bpos.vy = pEm->Ground - 0xBB8;
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);

            Em2a_add_dm(0x14);
        }
        pEm->Spd_z = 100;

    }
}

static void Em2a_att2_l_br(ENEMY_WORK *pEm) {}

static void Em2a_att2_l(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;
    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Routine_2++;
    case 1:
        if (Mot_play_sp(pEm, 7, 3, 0)) {
            EF(pEm)->LRpunch_wait = 0x23;
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);

        /* retail: the Get_dir_pl turn-into-grab test of Beta 2 is removed */

        Svec.vx = EF(pEm)->Pold_x;
        Svec.vz = EF(pEm)->Pold_z;
        if (Att_ck2(pEm, 15, 5, 14, 0x258, pPl->Atd[0].At_w * 2)) { SVECTOR Bpos;

            Snd_se_enem(2, pEm);
            Pad_vib_set0(3, 0); /* retail-only vibration */
            Pad_vib_set1(5, 220, 0);
            Pad_vib_fade_set(4, 220, 40, 5);

            Pl_dm_set(pEm, &Svec, 15);

            Bpos.vx = pParts[15].Workm.t[0];
            Bpos.vy = pParts[15].Workm.t[1];
            Bpos.vz = pParts[15].Workm.t[2];
            if (Bpos.vy < pEm->Ground - 0xBB8) Bpos.vy = pEm->Ground - 0xBB8;
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);

            Em2a_add_dm(0x14);
        }

    }
}

static void Em2a_att_w_br(ENEMY_WORK *pEm) {}

static void Em2a_att_w(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Routine_2++;
        pEm->Be_flg |= 4;
    case 1:
        pEm->Routine_2 += Mot_play_sp(pEm, 12, 15, 0);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);

        if (Em2a_Att_ck(pEm, 12, 0x2F, 0x32, 0x226)) { SVECTOR Bpos;

            Snd_se_enem(2, pEm);

            *(unsigned long *)&G.Player.Routine_0 = 2;
            G.Player.Routine_1 += Cdir_ck(pEm, &G.Player);

            Bpos.vx = pParts[12].Workm.t[0];
            Bpos.vy = pParts[12].Workm.t[1];
            Bpos.vz = pParts[12].Workm.t[2];
            if (Bpos.vy < pEm->Ground - 0xBB8) Bpos.vy = pEm->Ground - 0xBB8;
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);
            Bpos.vy += 100;
            Esp_call(0x2B00, 0, &GsIDMATRIX, &Bpos);
            Pad_vib_set0(3, 0); /* retail-only vibration (0x801026C8..0x801026F0) */
            Pad_vib_set1(8, 255, 0);
            Pad_vib_fade_set(4, 255, 40, 8);

            Em2a_add_dm(0x28);
        }
        break;

    case 2:
        EF(pEm)->Retail_14 = 50; /* retail-only, sb @0x80102708 */
        pEm->Be_flg &= ~4;
        *(unsigned long *)&pEm->Routine_0 = 0x101;

    }
}

static void Em2a_att_t_br(ENEMY_WORK *pEm) {}

static void Em2a_att_t(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;
    SVECTOR Svec;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Routine_2++;
    case 1:

        if (Mot_play_sp(pEm, 15, 15, 0))
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);

        Svec.vx = EF(pEm)->Pold_x;
        Svec.vz = EF(pEm)->Pold_z;
        if (Em2a_Att_ck(pEm, 15, 14, 19, 0x3B6)) { SVECTOR Bpos;

            Snd_se_enem(2, pEm);

            Goto00(pPl, pEm->Pos_x, pEm->Pos_z, 0x100);

            *(unsigned long *)&pPl->Routine_0 = 2;

            Bpos.vx = pParts[15].Workm.t[0];
            Bpos.vy = pParts[15].Workm.t[1];
            Bpos.vz = pParts[15].Workm.t[2];
            if (Bpos.vy < pEm->Ground - 0xBB8) Bpos.vy = pEm->Ground - 0xBB8;
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);
            Pad_vib_set0(3, 0); /* retail-only vibration (0x801028A0..0x801028C8) */
            Pad_vib_set1(8, 220, 0);
            Pad_vib_fade_set(6, 220, 40, 8);

            Em2a_add_dm(10);

            EF(pEm)->Attribute |= 0x20;
        }

        break;

    case 2:
        pEm->Move_no = 0xF;
        if (Mot_play_sp(pEm, 15, 0, 1))
            *(unsigned long *)&pEm->Routine_0 = 0x101;

        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);

        if (pEm->Move_cnt == 0x16) {
            SVECTOR Bpos;

            Snd_se_enem(2, pEm);

            Goto00(pPl, pEm->Pos_x, pEm->Pos_z, 0x100);
            Pl_dm_set_normal(pEm);

            Bpos.vx = pParts[15].Workm.t[0];
            Bpos.vy = pParts[15].Workm.t[1];
            Bpos.vz = pParts[15].Workm.t[2];
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);
            Pad_vib_set0(3, 0); /* retail-only vibration (0x80102990..0x801029A8) */
            Pad_vib_fade_set(12, 160, 40, 0);

            Em2a_add_dm(10);
        }

    }
}

static void Em2a_att_cf_br(ENEMY_WORK *pEm) {}

static void Em2a_att_cf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EF(pEm)->Cfpunch_wait = 200;
        pEm->Timer0 = 0;
    case 1:
        pEm->Routine_2 += Mot_play_sp(pEm, 3, 15, 0);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x18);

        Svec.vx = EF(pEm)->Pold_x;
        Svec.vz = EF(pEm)->Pold_z;
        if (Em2a_Att_ck(pEm, 12, 0x15, 0x19, 0x2EE)) {
            SVECTOR Bpos;

            Snd_se_enem(2, pEm);
            Pad_vib_set0(3, 0); /* retail-only vibration (0x80102AB8..0x80102AE0) */
            Pad_vib_set1(8, 220, 0);
            Pad_vib_fade_set(4, 220, 40, 8);

            Pl_dm_set(pEm, &Svec, 12);

            Bpos.vx = pParts[12].Workm.t[0];
            Bpos.vy = pParts[12].Workm.t[1];
            Bpos.vz = pParts[12].Workm.t[2];
            Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);

            Em2a_add_dm(0x1E);
        }
        break;
    case 2:
        *(unsigned long *)&pEm->Routine_0 = 0x101;

    }
}

static void Em2a_117_br(ENEMY_WORK *pEm) {}

static void Em2a_117(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Routine_2++;
        pEm->Timer0 = 10;
        pEm->Be_flg |= 0x408;
    case 1:
        if (pEm->Sce_free0) {
            G.Status_flg |= 0x100;
            pEm->Pos_x = -0x5D3B;
            pEm->Pos_y = -0x2C88;
            pEm->Pos_z = 0x1F8;
            pEm->Cdir_y = 0x800;
            pEm->Routine_2++;
        }
        break;

    case 2:
        if (Mot_play_sp(pEm, 14, 0, 0)) {
            pEm->Be_flg &= ~0x408;
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
        if (pEm->Move_cnt == 0x33) {
            if (pEm->Timer0) {
                pEm->Timer0--;
                pEm->Pos_y -= 1000;
                if (!pEm->Timer0) pEm->Sce_free1 = 1;
                pEm->Move_cnt = 0x32;
            } else {
                pEm->Pos_x = -0x6432;
                pEm->Pos_y = -0x3841;
                pEm->Pos_z = -0x1068;
                pEm->Cdir_y = 0xC20;
                pEm->Type &= ~0x4000;
            }
        }

    }
}

static void Em2a_30a_br(ENEMY_WORK *pEm) {}

static void Em2a_30a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Neck_flg |= 4;
        pEm->Routine_2++;
        pEm->Be_flg |= 8;

    case 1:
        if (!(pEm->Sce_free0 & 1)) break;

        pEm->Routine_2++;
    case 2:
        Mot_play_sp(pEm, 1, 0, 0);

        if (!(pEm->Sce_free0 & 2)) break;

        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 0x1F0000;
        break;

    case 3:
        pEm->Move_no = 0;
        Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x80);
        if (pEm->Sce_free0 & 4) pEm->Routine_2++;
        break;

    case 4:
        Mot_play_sp(pEm, 1, 0x1F, 0);

    }
}

static void Em2a_602_br(ENEMY_WORK *pEm) {}

static void Em2a_602(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Sce_free1 = 0;
        pEm->Routine_2++;
    case 1:
        if (!pEm->Sce_free0) break;

        pEm->Type &= ~0x4000;
        pEm->Pos_y = -0xFA0;
        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 0x330E;
    case 2:
        pEm->Move_no = 0xE;
        pEm->Move_cnt = 0x33;
        Joint_move(pEm, Kan, Seq, 0x1000);
        pEm->Pos_y += 1000;
        if (!pEm->Pos_y) pEm->Routine_2++;
        break;

    case 3:
        pEm->Move_no = 0xE;
        pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x1000);
        break;

    case 4:
        if (pEm->Sce_free1)

            *(unsigned long *)&pEm->Routine_0 = 0x101;
    }
}

static void Em2a_507_br(ENEMY_WORK *pEm) {}

static void Em2a_507(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Routine_2++;
    case 1:
        if (pEm->Sce_free0 & 8) { /* retail-only shortcut to case 5 (@0x80102F84) */
            pEm->Routine_2 = 5; break;
        }
        if (!pEm->Sce_free0) break;

        pEm->Routine_2++;
    case 2:
        Mot_play_sp(pEm, 1, 15, 0);

        if (!(pEm->Sce_free0 & 2)) break;

        pEm->Routine_2++;
    case 3:
        Mot_play(pEm, 12, 0x1F, 0);
        pEm->Move_cnt = 1;

        if (!(pEm->Sce_free0 & 4)) break;

        pEm->Move_cnt = 0;
        pEm->Routine_2++;
    case 4:
        pEm->Routine_2 += Mot_play_sp(pEm, 12, 15, 0);
        break;

    case 5:
        if (pEm->Sce_free0 & 8) {
            pEm->Type &= ~0x4000;
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
    }
}

static void Em2a_604l_br(ENEMY_WORK *pEm) {}

static void Em2a_604l(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Timer0 = 0;
        pEm->Pos_y = -0x61A8;
        pEm->Be_flg |= 0xA;
        pEm->Routine_2++;
    case 1:
        if (pEm->Sce_free0 != 1) break;

        pEm->Pos_y = -0x5DC0;
        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 0x330E;

    case 2:
        pEm->Move_no = 0xE;
        pEm->Move_cnt = 0x33;
        Joint_move(pEm, Kan, Seq, 0x1000);
        pEm->Pos_y += 1000;
        if (pEm->Pos_y == -0x4650)
            pEm->Routine_2++; break;

    case 3:
        pEm->Move_no = 0xE;
        pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x1000);
        break;

    case 4:
        if (pEm->Sce_free0 != 2) break;

        pEm->Routine_2++;

    case 5:
        Mot_play_sp(pEm, 1, 15, 0);
        if (pEm->Sce_free0 == 3)
            pEm->Routine_2++;

        break;

    case 6:
        pEm->Routine_2 += Mot_play_sp(pEm, 9, 15, 0);
        break;

    case 7:
        if (pEm->Sce_free0 != 4) break;

        *(unsigned long *)&pEm->Move_no = 0xF0001;
        pEm->Routine_2++;

    case 8:
        pEm->Move_no = 1;
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);

        break;

    case 9:
        if (pEm->Sce_free0 != 5) break;

        pEm->Cdir_y += 0x800;
        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 1;
        Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000);
        break;

    case 10:
        Mot_play_sp(pEm, 8, 3, 0);
        if (pEm->Sce_free0 != 6) break;

        *(unsigned long *)&pEm->Move_no = 0x30002;
        pEm->Routine_2++;

    case 11:
        pEm->Move_no = 2;

        if (pEm->Sce_free0 == 7) {
            pEm->Routine_2++;
            *(unsigned long *)&pEm->Move_no = 0x30003;
        }
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400))

            pEm->Move_cnt = 0x19;
        break;

    case 12:
        pEm->Move_no = 3;
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400))
            pEm->Routine_2++;

        Add_speedXZ(pEm, 0); break;

    case 13:
        if (pEm->Sce_free0 != 8) break;

        pEm->Cdir_y += 0x800;
        *(unsigned long *)&pEm->Move_no = 4;
        pEm->Routine_2++;

    case 14:
        Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000);
        if (pEm->Sce_free0 != 8) break;

        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 4;
        break;

    case 15:
        pEm->Routine_2++;
        break;

    case 16:
        pEm->Move_no = 4;
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000)) {
            *(unsigned long *)&pEm->Move_no = 0x30000;
            pEm->Routine_2++;
        }
        Add_speedXZ(pEm, 0);
        break;

    case 17:
        pEm->Move_no = 0; Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

        Add_speedXZ(pEm, 0);
    }
}

static void Em2a_604c_br(ENEMY_WORK *pEm) {}

static void Em2a_604c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Be_flg |= 0xA;
        pEm->Routine_2++;
    case 1:
        if (pEm->Sce_free0 != 1) break;

        pEm->Routine_2++;

    case 2:
        Mot_play_sp(pEm, 1, 15, 0);

        if (pEm->Sce_free0 != 2) break;

        pEm->Routine_2++;

    case 3:
        pEm->Routine_2 += Mot_play_sp(pEm, 2, 15, 0);
        break;

    /* retail (jtbl_801000E4, 12 cases): new cases 4/5 wait for Sce_free0 6, then 3 */
    case 4:
        if (pEm->Sce_free0 != 6) break;

        pEm->Routine_2++;

    case 5:
        Mot_play_sp(pEm, 1, 15, 0);

        if (pEm->Sce_free0 != 3) break;

        *(unsigned long *)&pEm->Move_no = 0x70001;
        pEm->Routine_2++;
        break;

    case 6:
        pEm->Move_no = 1;
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x200); /* retail: 0x1000 -> 0x200 */

        Add_speedXZ(pEm, 0);

    case 7:
        if (pEm->Sce_free0 != 4) break;

        pEm->Cdir_y += 0x800;
        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 1;
        Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000);
        break;

    case 8:
        Mot_play_sp(pEm, 1, 15, 0);

        if (pEm->Sce_free0 != 5) break;

        pEm->Routine_2++;
        *(unsigned long *)&pEm->Move_no = 4;

    case 9:
        pEm->Move_no = 4;
        Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000)) {
            *(unsigned long *)&pEm->Move_no = 0xA00;
            Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
            pEm->Routine_2++;
            break;
        }

        Add_speedXZ(pEm, 0); break;

    case 10:
        pEm->Cdir_y += 0x400;
        pEm->Routine_2++;

    case 11:
        pEm->Move_no = 0; Spd_get(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x1000);

        Add_speedXZ(pEm, 0);
    }
}

static void Em2a_20d_br(ENEMY_WORK *pEm) {}

static void Em2a_20d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Sce_free0 = 0;
        pEm->Be_flg |= 8;
        pEm->Routine_2++;
        pEm->Move_cnt = 0;

    case 1:
        if (pEm->Sce_free0 != 1) break;

        pEm->Move_cnt = 0x1A;
        pEm->Move_no = 0x10;
        Spd_get(pEm, Kan, Seq, 0);
        pEm->Routine_2++;
    case 2:
        pEm->Move_no = 0x10;
        pEm->Routine_2 += Mot_play_sp(pEm, 0x10, 7, 0);
        break;

    case 3:
        pEm->Be_flg &= ~8;
        pEm->Type &= ~0x4000;
        *(unsigned long *)&pEm->Routine_0 = 0x101;
    }
}

static void Em2a_cri_br(ENEMY_WORK *pEm) {}

static void Em2a_cri(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;
    SVECTOR Bpos;
    VECTOR Vec;
    register short Dir; register short Front; /* Front: inferred name, the compiler deletes it, retail SYM has no record */
    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

    Front = Cdir_ck(&G.Player, pEm);

    if (Front) {

        Svec.vx = 0x640;
        Svec.vz = 0x12C;
    } else {

        Svec.vx = 0x7D0;
        Svec.vz = 0x10E;
        Approach_one(&pEm->Pos_y, G.Player.Pos_y + 0x82, 1);
    }

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Be_flg |= 0xA;
        pEm->Routine_2++;
    case 1:
        pEm->Move_no = 2;
        pEm->Routine_2 += Mot_play_sp(pEm, 2, 3, 0);

        Rot_vector(pEm->Cdir_y, &Svec, &Svec);
        Vec.vx = pEm->Pos_x + Svec.vx;
        Vec.vz = pEm->Pos_z + Svec.vz;
        Approach(&G.Player.Pos_x, &Vec, 0x12C);

        pEm->Pos_y += 25; /* retail-only (sw @0x801039FC) */
        Goto01(pEm, Dir + 0x60, 0x30);

        if (++pEm->Timer0 >= 7) {
            pEm->Move_cnt = 9;
            pEm->Routine_2++;
        }
        break;

    case 2:
        pEm->Move_no = 0xC;
        pEm->Routine_2 += Mot_play(pEm, 12, 3, 0);
        if (pEm->Move_cnt == 0x3F) pEm->Routine_2++;
        /* retail: no Goto01 here */

        Rot_vector(pEm->Cdir_y, &Svec, &Svec);
        Vec.vx = pEm->Pos_x + Svec.vx;
        Vec.vz = pEm->Pos_z + Svec.vz;
        Approach(&G.Player.Pos_x, &Vec, 0xFA);

        if (pEm->Move_cnt == 0x32) {
            Snd_se_enem(2, pEm); /* retail-only */
            Bpos.vx = pParts[12].Workm.t[0];
            Bpos.vy = pParts[12].Workm.t[1];
            Bpos.vz = pParts[12].Workm.t[2];
            Esp_call(0x2800, 0, &GsIDMATRIX, &Bpos);
        }
        if (pEm->Move_cnt == 0x2F) Snd_se_enem(2, pEm); /* retail-only (@0x80103B18) */
        if (pEm->Move_cnt == 0x2D) { /* retail-only vibration sequence (0x80103B30..0x80103B88) */
            Pad_vib_set0(4, 0);
            Pad_vib_set1(10, 255, 0);
            Pad_vib_fade_set(8, 255, 40, 10);
            Pad_vib_set0(3, 46);
            Pad_vib_fade_set(16, 200, 90, 46);
            Pad_vib_fade_set(40, 100, 40, 62);
        }
        break;

    case 3:
        Goto01(pEm, Dir + 0x60, 0x30);
        pEm->Routine_2 += Mot_play(pEm, 13, 3, 0);
        if (pEm->Move_cnt == 0x95) pEm->Move_cnt = 0x74;

    }
}

static void Em2a_612_br(ENEMY_WORK *pEm) {}

static void Em2a_612(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Attribute |= 0x80;
        pEm->Sce_free0 = 0;
        pEm->Routine_2++;
        Mot_play_sp(pEm, 0, 0, 0);
        pEm->Be_flg |= 8;

    case 1:
        if (!pEm->Sce_free0) break;

        pEm->Routine_2++;
    case 2:
        Mot_play_sp(pEm, 0, 0, 0);
        if (pEm->Sce_free0 == 2) pEm->Routine_2++;

    }
}

/* Retail-only routine 18 (em2a_cdemd0_s1 0x80103CF4 / 0x80103CFC, s0 +0xD000): byte-identical
 * (masked, except one Mot_play_sp argument) to Beta EM2B's Em2a_where_br / Em2a_where
 * (src/prog/em2b.c:673-681), whose names it takes; the retail source line is unknown (source line not recorded). */
static void Em2a_where_br(ENEMY_WORK *pEm) {}

static void Em2a_where(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register short Dir;

    Dir = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

    if (!pEm->Routine_2) {
        if (G.Cbuf_id) Mot_play_sp(pEm, 1, 0xF, 0);	/* EM2B passes 3; retail EM2A 0xF (li a2 @0x80103D58) */
        Goto01(pEm, Dir, 8);
    }
}

static void Em2a_pl_dm_c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register ENEMY_WORK *pEnemy = (ENEMY_WORK *)pEm->pEnemy_ptr;
    register short Dir;

    Dir = Cdir_ck(pEm, pEnemy);

    switch (pEm->Routine_2) {

    case 0:

        *(unsigned long *)&pEm->Move_no = 0x30003 + (pEm->Routine_1 & 1);
        pEm->Be_flg |= 0xA;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;

        Snd_se_stad((Dir << 16) + 0x10000 | 0x4000001, &pEm->Pos_x);

        pEm->Neck_flg |= 0x12;

    case 1:
        if (Dir)
            Goto00_r(pEm, pEnemy->Pos_x, pEnemy->Pos_z, 0x40);
        else
            Goto00(pEm, pEnemy->Pos_x, pEnemy->Pos_z, 0x40);

        if (pEm->Move_cnt >= 6)
            { if (G.Cbuf_id) Joint_move(pEm, Kan, Seq, 0x400); }
        else
            Joint_move(pEm, Kan, Seq, 0x400);

        if (pEm->Move_cnt == 0x1A) pEm->Routine_2++;
        break;

    case 2:
        Snd_se_stad((Dir << 16) + 0x10000 | 0x4000001, &pEm->Pos_x);
        *(unsigned long *)&pEm->Move_no = 0x31000;
        pEm->Routine_2++;
    case 3:
        if (Dir)
            Goto00_r(pEm, pEnemy->Pos_x, pEnemy->Pos_z, 0x10);
        else
            Goto00(pEm, pEnemy->Pos_x, pEnemy->Pos_z, 0x10);
        pEm->Routine_2 += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        break;

    case 4:
        if (++pEm->Timer0 >= 0x24) pEm->Routine_2++;
        break;

    case 5:
        Snd_se_stad((Dir << 16) + 0x10000 | 0x4000001, &pEm->Pos_x);
        *(unsigned long *)&pEm->Move_no = 0x30005;
        pEm->Routine_2++;

    case 6:
        pEm->Routine_2 += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        if (pEm->Move_cnt == 0xC) pEm->Routine_2++;
    }
}

static void Em2a_damage_ck(ENEMY_WORK *pEm)
{
    /* retail: Buf0/Buf1 are per-enemy EM2A_FREE members, not function statics (@0x80104058) */
    if (pEm->Life >= 20000) {
        if (S_rnd(4, &EF(pEm)->Buf0, &EF(pEm)->Buf1)) {
            if (!(pEm->Damage_flg & 0x20)) {
                pEm->Damage_no = 0; return;
            }
            pEm->Damage_no = 1;
        } else
            pEm->Damage_no = 4;
    } else
        pEm->Damage_no = 3;
}

static void Em2a_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_1 != 0xFF) {
        EF(pEm)->Dm_wep = pEm->Routine_1;
        Dm_eff(pEm);
        pEm->Routine_1 = 0xFF;
        Em2a_damage_ck(pEm);
    }

    Em2a_damage[pEm->Damage_no](pEm, Kan, Seq); }

static void Em2a_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Timer0 = Rnd() & 1;
    case 1:
        pEm->Routine_3 += Mot_play_sp(pEm, pEm->Timer0 + 4, 15, 0);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
        break;

    case 2:
        *(unsigned long *)&pEm->Routine_0 = 0x101;

    }
}

static void Em2a_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 += Mot_play_sp(pEm, 9, 15, 0);
        break;

    case 1:
        *(unsigned long *)&pEm->Routine_0 = 0x101;

    }
}

static void Em2a_dm_down(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {
    case 0:
        Em_dead_flg_on(pEm);
        pEm->Damage_cnt |= 0x80;
        pEm->Sce_free0 = 0;
        pEm->Type |= 0x4000;	/* retail: replaces Be_flg |= 2 here (sh @0x801042BC) */
        pEm->Routine_3++;

    case 1:
        pEm->Routine_3 += Mot_play_sp(pEm, 10, 15, 0);
        break;	/* retail: no fall-through into case 2 */

    case 2:
        pEm->Be_flg |= 2;	/* retail: moved here (sw @0x801042F0) */
        if (pEm->Sce_free0) {
            pEm->Sce_free0 = 0;
            pEm->Routine_3 = 3;
        }
        break;

    case 3:
        pEm->Move_cnt = 0;
        Mot_play(pEm, 11, 0, 0);
        if (pEm->Sce_free0) pEm->Routine_3++;
        break;

    case 4:
        pEm->Routine_3 += Mot_play(pEm, 11, 0, 0);
        break;
    case 5:
        break;
    }
}

static void Em2a_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    *(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
    if (pEm->Routine_0 == 1)
        Em2a_mv[pEm->Routine_1](pEm, Kan, Seq);
    else
        *(unsigned long *)&pEm->Routine_0 = 0x101; }

static void Em2a_die_ck(ENEMY_WORK *pEm)
{
    pEm->Damage_no = 0; }

static void Em2a_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_1 != 0xFF) {
        pEm->Routine_1 = 0xFF;
        Em2a_die_ck(pEm);
    }

    Em2a_die[pEm->Damage_no](pEm, Kan, Seq); }

void Em12a_Pldamage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em2a_pldm_tbl[pEm->Routine_1](pEm, Kan, Seq); }

static void Em2a_pl_dm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long Tmp;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_stad(0x4010001, &pEm->Pos_x); /* retail-only, jal @0x801044E4 */
        *(unsigned long *)&pEm->Move_no = 0x30002;
        pEm->Routine_2 = 1;
        pEm->Spd_x = -0x258;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;

        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;
    case 1:
        if (!pEm->Hokan_flg)
            pEm->Routine_2 = 2;

        Add_speedXZ(pEm, 0);
        Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        pEm->Spd_x += 0x70;
        break;

    case 2:
        *(unsigned long *)&pEm->Move_no = 0x30003;
        pEm->Routine_2 = 3;
    case 3:
        Add_speedXZ(pEm, 0);
        Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        Tmp += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        pEm->Routine_2 += Tmp;

        pEm->Spd_x += 0xF;
        if (pEm->Spd_x > 0)

            pEm->Spd_x = 0; break;

    case 4:
        *(unsigned long *)&pEm->Routine_0 = 1;
        pEm->Damage_cnt = 0;
        pEm->Neck_flg &= ~0x12;

    }
}

static void Em2a_pl_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long Tmp;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_stad(0x4020001, &pEm->Pos_x); /* retail-only, jal @0x80104660 */
        *(unsigned long *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x1F4;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;

        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;

    case 1:
        Add_speedXZ(pEm, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        if (pEm->Hokan_flg) {
            pEm->Spd_x -= 0x1E;
        } else
            pEm->Spd_x -= 0x3C;

        if (pEm->Spd_x < 0)

            pEm->Spd_x = 0; break;

    case 2:
    case 4:

        *(unsigned long *)&pEm->Move_no = 0x30001;
        pEm->Routine_2 = 5;
        pEm->Damage_cnt = 0;

    case 5:
        Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        Tmp += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

        if (Tmp) {
            pEm->Routine_2 = 6;
            *(unsigned long *)&pEm->Move_no = 0x30006;
        }
        break;

    case 6:
        if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400)) {

            *(unsigned long *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Neck_flg &= ~0x12;
        }

    }
}

static void Em2a_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em2a_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}
