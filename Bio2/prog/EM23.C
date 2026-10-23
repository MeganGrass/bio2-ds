/* em23.c - reconstruction of EM23.C, the EM23 enemy module.  One source,
 * linked into two runtime overlays: 0x09 (enemy bin slot 0, 0x80100190..)
 * and 0x27 (slot 1, 0x8010D190..).  The gate compiles it once per overlay
 * with -DOVL_09 / -DOVL_27; the only per-slot difference is the name of the
 * two exported entry points (Em1xx for slot 0, Em2xx for slot 1, see
 * EM_SET.C Em_move_tbl_set).  Types come from the TU's retail SYM preamble
 * (em23.h); the tables are the retail initialized data.  Engine routines are
 * called undeclared, as in the original: their implicit declarations open
 * the retail debug blocks. */
#include "EM23.H"

extern MATRIX GsIDMATRIX;
extern short Bomb_tbl[2][30];

/* the module's private work in ENEMY_WORK.Free[] */
/* retail's Free[] starts 4 bytes after the project's ENEMY_WORK.Free (pEm+0x218 against +0x214) */
#define EF(p) ((EM23_FREE *)((UCHAR *)(p)->Free + 4))

/* Byte fields that are plain char; the retail code reads them as signed bytes (lb) in these tests. */
#define S8(x)	(*(signed char *)&(x))

/* PsyQ libgte */

/* forward declarations: the retail SYM lists these objects first-declared here (defined below). */
static void (*Em23_routine_0[8])();
static void (*Em23_mv_br[8])();
static void (*Em23_mv[8])();
static void (*Em23_damage[5])();
static void (*Em23_die[])();
static void (*Em23_pldm_tbl[4])();
static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
/* Retail life tables (em23_cdemd0_s1 .data 0x801045BC..0x8010461B): the Em2b pair changed
 * relative to Beta 2; a second complete set (0x801045EC..) is selected by G.System_flg & 0x40
 * (Em23_Init @0x80100518). Second-set names are [INFERRED], following src-retail/prog/em20.c. */
static unsigned short Em2b_life = 250;
static unsigned short Em2b_life_easy = 200;
static unsigned short Em23_life_flg40 = 240;
static unsigned short Em23_life_easy_flg40 = 240;
static unsigned short Em2a_life_flg40[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy_flg40[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life_flg40 = 250;
static unsigned short Em2b_life_easy_flg40 = 200;
static void (*Em23_routine_0[8])() = {
    Em23_Init,
    Em23_Move,
    Em23_Damage,
    Em23_Die,
    0,
    0,
    0,
    Em23_Dead
};
static void (*Em23_mv_br[8])() = {
    Em23_ent_br,
    Em23_walk_br,
    Em23_walk_br,
    Em23_turn_br,
    Em23_Chit_br,
    Em23_haziki_br,
    Em23_taiki_br,
    Em23_last_br
};
static void (*Em23_mv[8])() = {
    Em23_ent,
    Em23_walk,
    Em23_run,
    Em23_turn,
    Em23_Chit,
    Em23_haziki,
    Em23_taiki,
    Em23_last
};
static void (*Em23_damage[5])() = {
    Em23_dm_no,
    Em23_explosion,
    Em23_dm_escape,
    Em23_died,
    Em23_dm
};
static void (*Em23_die[])() = {};
static void (*Em23_pldm_tbl[4])() = {
    Em23_Pl_die,
    Em23_Pl_stumble,
    Em2a_pl_dm_f,
    Em2a_pl_dm_b
};
static EM23_ESP Blood_tbl[24] = {
    { 2300, -300, 400, -2500, 7 },
    { 1900, -500, 400, -2500, 7 },
    { 1200, -600, 800, -2500, 7 },
    { 500, -200, 1200, -2500, 7 },
    { 1600, -600, 100, -2500, 7 },
    { 1800, -400, -300, -2500, 7 },
    { 2600, 0, -300, -2500, 7 },
    { 800, -400, -900, -2500, 7 },
    { 1700, 500, -900, -2500, 6 },
    { 200, 100, 500, -2500, 4 },
    { 600, 300, 0, -2500, 10 },
    { 700, 0, 1300, -2500, 6 },
    { 1000, 700, 800, -2500, 6 },
    { 2000, 900, -100, -2500, 6 },
    { 2000, 200, 0, -2500, 6 },
    { 3800, 500, 0, -2500, 6 },
    { 3200, 300, 500, -2500, 6 },
    { 3300, 300, 200, -2500, 6 },
    { 3100, 300, -300, -2500, 6 },
    { 900, 500, -1000, -2500, 5 },
    { 600, 1200, 800, -2500, 3 },
    { -400, 800, 200, -2500, 8 },
    { 700, -400, 1200, -2500, 5 },
    { 1300, 0, 1300, -2500, 6 }
};
static DM_ESP2 Spark_tbl[21] = {
    { 3100, -800, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 2500, -100, -200, 0, 7, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 2000, -100, -1400, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 900, 600, 800, 0, 5, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { -300, 600, 200, 0, 8, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 400, 0, 0, 0, 3, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 1000, -1000, 1000, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 0, 0, 0, 0, 0, 2 },
    { 1700, -900, 600, 0, 0, 0 }
};

void Em123(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;

    if (!(G.Stop_flg & 0x20000000)) {

        pEm->Neck_no = 5;

        if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

        { register ENEMY_WORK *pEm2, **ppEm; VECTOR Vec; SVECTOR Dir_bak; Em23_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

        Mot_play_init(pEm, EF(pEm)->Rno_bak);
        EF(pEm)->Rno_bak = *(unsigned long *)&pEm->Routine_0;

        pEm->pEnemy_ptr = (unsigned long *)&G.Player;

        Vec.vx = pParts[7].Workm.t[0];
        Vec.vy = pParts[7].Workm.t[1];
        Vec.vz = pParts[7].Workm.t[2];

        pEm->Be_flg |= 0x02000000;
        pEm->pSuper = (unsigned long *)&pParts[7].Workm;
        Dir_bak.vx = pEm->Cdir_x;
        Dir_bak.vy = pEm->Cdir_y;
        Dir_bak.vz = pEm->Cdir_z;

        pEm->Cdir_x = 0;
        pEm->Cdir_y = 0;
        pEm->Cdir_z = 0;
        Oba_set_ofs(pEm, &Vec);

        pEm->Cdir_x = Dir_bak.vx;
        pEm->Cdir_y = Dir_bak.vy;
        pEm->Cdir_z = Dir_bak.vz;
        pEm->Be_flg &= ~0x02000000;
        pEm->pSuper = (unsigned long *)&GsIDMATRIX;

        pEm->At_em_no = 0xFF;

        for (ppEm = &G.pEnemy[0]; ppEm != G.ppEnemy_end; ppEm++) {

            pEm2 = *ppEm;
            if (pEm == pEm2) continue;

            if (pEm2->Be_flg & 1) {
                Oba_ck_hit(pEm2, pEm);
            }

        } }

        Gas_get(pEm);

        Em23_mouse_move(pEm);

        if (EF(pEm)->Attribute & 4) {
            if (pEm->Neck_flg & 4)
                Rot_neck(pEm, pEm->Cdir_y);
            else
                Rot_neck_em(pEm, pEm->Cdir_y);
        }

        pParts[6].Cdir_z += EF(pEm)->Neck_ctr;
        RotMatrix(&pParts[6].Cdir_x, &pParts[6].M);
        pParts[6].Cdir_z -= EF(pEm)->Neck_ctr;

        pParts[7].Cdir_z += EF(pEm)->Mouse_ctr;
        RotMatrix(&pParts[7].Cdir_x, &pParts[7].M);
        pParts[7].Cdir_z -= EF(pEm)->Mouse_ctr;

        if (!(EF(pEm)->Attribute & 2)) Player_stumble_ck(pEm);

        if ((*pEm->pNow_seq & 0xF8000000) == 0x28000000) EF(pEm)->Earthquake_ctr = 1;
        Earthquake_move(pEm);
        if ((*pEm->pNow_seq & 0xF8000000) == 0x48000000) EF(pEm)->Roar_cnt = 0x37;
        Em_seq_sound(pEm);
        if (EF(pEm)->Roar_cnt) EF(pEm)->Roar_cnt--;

        if (EF(pEm)->Stumble_cnt) EF(pEm)->Stumble_cnt--;

        if (EF(pEm)->Dm_eff_func) ((void (*)())EF(pEm)->Dm_eff_func)(pEm);

        if (EF(pEm)->Rensha_cnt) EF(pEm)->Rensha_cnt--;

        if (EF(pEm)->Dm_timer) EF(pEm)->Dm_timer--;
        if (EF(pEm)->Retail_20) EF(pEm)->Retail_20--; /* retail-only @0x801004A8 */
        pEm->Neck_no = 6;
    } }

static void Em23_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long i;
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register PARTS_NECK_FREE_WORK *pNeck; /* inferred name: GCC folds it into the store offsets, retail SYM has no record */
    G.pPldamage_Em[0x23 - 0x10] = Em123_Pldm;

    *(unsigned long *)&pEm->Routine_0 = 1;

    pEm->Damage_flg = 0;
    pEm->Sce_flg = 0;
    pEm->Down_cnt = 0;
    if (G.System_flg & 0x40) { /* retail-only second life set (0x80100518..0x8010057C) */
        if (G.System_flg & 0x20)
            pEm->Life = Em23_life_flg40 + 25000;
        else
            pEm->Life = Em23_life_easy_flg40 + 25000;
    } else if (G.System_flg & 0x20)
        pEm->Life = Em23_life + 25000;
    else
        pEm->Life = Em23_life_easy + 25000;

    pNeck = (PARTS_NECK_FREE_WORK *)pEm->pSin_parts_ptr[5].Free;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 5;
    pEm->pEnemy_neck = (unsigned long *)&G.Player;
    pNeck->N_rot_y = 0;
    pNeck->N_rot_z = 0;
    pNeck->Add_dir_y = 20;
    pNeck->Add_dir_z = 20;
    pNeck->L_rot_y = 0x230;
    pNeck->L_rot_z = 0x138;

    for (i = 0; i < 22; i++)
        pParts[i].Be_flg |= 0x9000;

    G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
    G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

    *(unsigned long *)&pEm->Move_no = 1;
    Joint_move(pEm, Kan, Seq, 0x100);

    EF(pEm)->Attribute = 0;
    EF(pEm)->Turn_cnt = 0;
    EF(pEm)->Def_x = EF(pEm)->Def_z = 0;
    EF(pEm)->Rno_bak = 0;
    EF(pEm)->Mouse_rtn = 0;
    EF(pEm)->Turn_flg = 0;
    EF(pEm)->Gget_rtn = 0;
    EF(pEm)->Earthquake_ctr = 0;
    EF(pEm)->Mouse_ctr = 0;
    EF(pEm)->Neck_ctr = 0;
    EF(pEm)->Stumble_cnt = 0;
    EF(pEm)->Head_rtn = 0;
    EF(pEm)->Dm_timer = 0;
    EF(pEm)->Dm_flg = 0;
    EF(pEm)->Dm_rtn_type = 0;
    EF(pEm)->Dm_eff_rtn = 0;
    EF(pEm)->Dm_eff_func = 0;
    EF(pEm)->Rensha_cnt = 0;
    EF(pEm)->Retail_20 = 0; /* retail-only, sb @0x8010069C */

    pEm->Sce_free0 = 0;

    pEm->Be_flg |= 0x4C;

    pEm->nOba = 3;
    pEm->Atd[1].Ofs_x = 450;
    pEm->Atd[1].Ofs_y = 0;
    pEm->Atd[1].Ofs_z = 0;
    pEm->Atd[1].At_w = pEm->Atd[1].W = 0x5AA;
    pEm->Atd[1].At_h = 0x7D0;
    pEm->Atd[1].At_d = pEm->Atd[1].D = 0x5AA;

    pEm->Atd[3].Ofs_x = -0x898;
    pEm->Atd[3].Ofs_y = 0;
    pEm->Atd[3].Ofs_z = 0;
    pEm->Atd[3].At_w = pEm->Atd[3].W = 0xAF0;
    pEm->Atd[3].At_h = 0x7D0;
    pEm->Atd[3].At_d = pEm->Atd[3].D = 0xAF0;

    pEm->Atd[2].Ofs_x = 0x76C;
    pEm->Atd[2].Ofs_y = 0;
    pEm->Atd[2].Ofs_z = 0;
    pEm->Atd[2].At_w = pEm->Atd[2].W = 0x3B6;
    pEm->Atd[2].At_h = 0x7D0;
    pEm->Atd[2].At_d = pEm->Atd[2].D = 0x3B6;

    pEm->Atd[0].Ofs_x = 0xBB8;
    pEm->Atd[0].Ofs_y = 0;
    pEm->Atd[0].Ofs_z = 0;
    pEm->Atd[0].At_w = pEm->Atd[0].W = 0x258;
    pEm->Atd[0].At_h = 0x7D0;
    pEm->Atd[0].At_d = pEm->Atd[0].D = 0x258;

    pEm->Eff_at_r = 0x44C; /* retail: 0xBB8 -> 0x44C (li @0x8010070C) */

    pEm->Be_flg |= 0x1C000000;

    Kage_work_set(&pEm->pKage_work, 0, 0, 0, &pEm->Pos_x);

}

static void Em23_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em23_mv_br[pEm->Routine_1](pEm);

    Mot_play_init(pEm, EF(pEm)->Rno_bak);
    EF(pEm)->Rno_bak = *(unsigned long *)&pEm->Routine_0;

    Em23_mv[pEm->Routine_1](pEm, Kan, Seq);
}

static void Em23_ent_br(ENEMY_WORK *pEm) {}

static void Em23_ent(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Sce_free0)
            pEm->Routine_2++; break;

    case 1:
        G.Status_flg |= 0x10;
        G.Status_flg |= 0x100;
        EF(pEm)->Attribute |= 2;
        if (pEm->Move_cnt == 0x46) Cut_change(0);
        if (Mot_play_sp(pEm, 6, 0, 0)) {
            Cut_change(1);
            pEm->Pos_z = -0x52DC;
            pEm->Routine_2++;
        }
        break;
    case 2:
        if (Mot_play_sp(pEm, 3, 0, 0)) {
            G.Status_flg &= ~0x10;
            G.Status_flg &= ~0x100;
            EF(pEm)->Attribute &= ~2;
            EF(pEm)->Retail_20 = 110; /* retail-only, sb @0x80100920 */
            pEm->Cdir_y += 0x400;
            EF(pEm)->Attribute |= 4;
            *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
    }
}

static void Em23_walk_br(ENEMY_WORK *pEm)
{
    register OBJ_MODEL_WORK *pOm = &G.Ob_model[1];

    Em23_Bite(pEm);

    if (!EF(pEm)->Turn_cnt) {

        if (pEm->Pos_x < -0x45D5) {
            EF(pEm)->Turn_cnt = 1;
            EF(pEm)->Def_x = -0x45D5 - pEm->Pos_x;
            EF(pEm)->Def_z = 0;
            *(unsigned long *)&pEm->Routine_0 = 0x301;
            return;
        }
    }

    if (EF(pEm)->Turn_cnt == 1) {

        if (pEm->Pos_z > -0x1C9B) {
            EF(pEm)->Turn_cnt = 2;
            EF(pEm)->Def_x = 0;
            EF(pEm)->Def_z = pEm->Pos_z + 0x1C9B;
            *(unsigned long *)&pEm->Routine_0 = 0x301;
            return;
        }

        if (EF(pEm)->Stumble_cnt) return;

        if (Flg_ck(G.SaveData.Room_flg, 1) && EF(pEm)->Gget_rtn < 2) {
            pEm->Neck_flg |= 4;
            pEm->Neck_point_x = pEm->Pos_x;
            pEm->Neck_point_y = pEm->Pos_y - 0x320;
            pEm->Neck_point_z = pOm->Pos_z;
        }

        if (G.Ob_model[1].Pos_z - pEm->Pos_z - 0x2328 < -0x78 && !EF(pEm)->Gget_rtn)

            pEm->Neck_flg &= ~4;

        if (Flg_ck(G.SaveData.Room_flg, 0)) {
            if (G.Ob_model[1].Pos_z - pEm->Pos_z > 0x2198 && G.Ob_model[1].Pos_z - pEm->Pos_z < 0x2328 && !EF(pEm)->Gget_rtn) {

                EF(pEm)->Gget_rtn = 1;
                if (pEm->Routine_1 == 2)

                    *(unsigned long *)&pEm->Routine_0 = 0x101; } return;
        }
    }

    if (EF(pEm)->Gget_rtn == 0 || EF(pEm)->Gget_rtn == 7) {

        if (pEm->Routine_1 == 1) {

            if (EF(pEm)->Stumble_cnt)
                *(unsigned long *)&pEm->Routine_0 = 0x201;

            /* retail (0x80100B5C..0x80100BB4): skipped while Retail_20 runs; both distances 100
               (Beta 0x2AF8 / 0x4650) */
            if (EF(pEm)->Retail_20) return;
            if (EF(pEm)->Attribute & 0x20) {
                if (pEm->L_pl > 100)
                    *(unsigned long *)&pEm->Routine_0 = 0x201; } else
            if (pEm->L_pl > 100) {

                *(unsigned long *)&pEm->Routine_0 = 0x201; } } else

        if (!EF(pEm)->Stumble_cnt) {

            if (EF(pEm)->Gget_rtn != 0 && EF(pEm)->Gget_rtn != 7)
                *(unsigned long *)&pEm->Routine_0 = 0x101;

            /* retail (0x80100BDC..0x80100C04): as in Em23_run_br */
            if (pEm->L_pl < 90) *(unsigned long *)&pEm->Routine_0 = 0x101;

            if (EF(pEm)->Retail_20) *(unsigned long *)&pEm->Routine_0 = 0x101;
        }
    }
}

static void Em23_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (!pEm->Routine_2)
    {   ; /* retail SLD: block opens on 481, its code is on 482 (a code-less token follows the brace) */
        Mot_play_sp(pEm, 0, 0x20, 0);
    }
}

static void Em23_run_br(ENEMY_WORK *pEm)
{

    Em23_Bite(pEm);

    if (EF(pEm)->Turn_cnt == 1 && G.Ob_model[1].Pos_z - pEm->Pos_z < 0x251C)

        *(unsigned long *)&pEm->Routine_0 = 0x101;

    /* retail (0x80100C9C..0x80100CC4): fixed distance 90 and the Retail_20 timer replace the
       Attribute-dependent 0x2134 / 0x2EE0 distances */
    if (pEm->L_pl < 90) *(unsigned long *)&pEm->Routine_0 = 0x101;

    if (EF(pEm)->Retail_20) *(unsigned long *)&pEm->Routine_0 = 0x101;
    }

static void Em23_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    if (!pEm->Routine_2)
    {   ; /* retail SLD: block opens on 510, its code is on 511 (a code-less token follows the brace) */
        if (Mot_play_sp(pEm, 1, 4, 0))
            *(unsigned long *)&pEm->Routine_0 = 0x201;

    }
}

static void Em23_turn_br(ENEMY_WORK *pEm) {}

static void Em23_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long Tmp;

    Em23_Bite(pEm);

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(4, pEm);
        pEm->Routine_2++;
    case 1:

        if (EF(pEm)->Stumble_cnt) {
            { Tmp = Mot_play_sp(pEm, 2, 0x10, 0) + Mot_play_sp(pEm, 2, 0x10, 0); } /* inner braces: retail block 541..541 */

        } else {
            Tmp = Mot_play_sp(pEm, 2, 0x10, 0);
        }
        pEm->Routine_2 += (Tmp != 0);
        EF(pEm)->Turn_flg = 1;
        break;
    case 2:
        if (EF(pEm)->Stumble_cnt)
            Tmp = Mot_play_sp(pEm, 3, 0, 0) + Mot_play_sp(pEm, 3, 0, 0);
        else

            Tmp = Mot_play_sp(pEm, 3, 0, 0);

        if (EF(pEm)->Turn_cnt == 2 && pEm->Move_no == 3 && pEm->Move_cnt >= 0x72)

            EF(pEm)->Attribute |= 0x10;

        if (Tmp) {
            pEm->Cdir_y += 0x400;
            EF(pEm)->Turn_flg = 0;

            if (EF(pEm)->Turn_cnt == 2) {
                *(unsigned long *)&pEm->Routine_0 = 0x701;
                EF(pEm)->Dm_timer = 0xFF;
            } else
                *(unsigned long *)&pEm->Routine_0 = 0x101;
        }

    }

    if (EF(pEm)->Def_x > 0) {
        pEm->Pos_x += 6;
        EF(pEm)->Def_x -= 6;
    }
    if (EF(pEm)->Def_z > 0) {
        pEm->Pos_z -= 6;
        EF(pEm)->Def_z -= 6;
    }
}

static void Em23_Chit_br(ENEMY_WORK *pEm) {}

static void Em23_Chit(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:

        G.Status_flg |= 0x100;
        /* retail (0x80100FD4..0x80100FF4): cut 14 or 7, as in Em23_died */
        Cut_change(((G.Status_flg & 0x40000000) && Flg_ck(G.SaveData.Zapping_flg, 16)) ? 14 : 7);
        Main.Bg_chg_flg = 1; /* retail-only, sb @0x80101004 */

        G.Ob_model[1].Be_flg = 0;
        EF(pEm)->Attribute |= 2;
        G.Player.Routine_0 = 5;
        G.Player.Routine_1 = 0;
        *(unsigned short *)&G.Player.Routine_2 = 0;
        G.Player.Life = -1; /* retail-only, sh G.Player+0x156 @0x80101040 */

        pEm->Pos_x = -0x5A26;
        EF(pEm)->Attribute &= ~4;
        EF(pEm)->Neck_ctr = 0;
        EF(pEm)->Mouse_ctr = 0;
        EF(pEm)->Mouse_rtn = 0;
        G.Player.pEnemy_ptr = (unsigned long *)pEm;
        G.Player.Pos_x = pEm->Pos_x + 0x2993;
        G.Player.Pos_z = -0x393;

        G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

        pEm->Be_flg |= 0x100A;
        G.Player.Be_flg |= 0x1008;
        G.Player.Damage_cnt |= 0x80;

        pEm->Routine_2++;
    case 1:

        Mot_play_sp(pEm, 4, 0, 0);
        if (pEm->Move_cnt == 0xD) {
            G.System_flg |= 0x4000000;
            G.System_flg |= 0x100;
            Set_base_pos(&G.Player, Kan, Seq, 0);
            pEm->Routine_2++;
            G.Player.Timer0 = 1;
            G.Player.Cdir_y = pEm->Cdir_y;
        }

        break;
    case 2:

        Null_pos_set(pEm, Kan, Seq, 0);
        if (Mot_play_sp(pEm, 5, 0, 0)) {
            G.Player.Be_flg |= 0x80000;
            pEm->Routine_2++;
        }
        break;
    case 3:

        Null_pos_set(pEm, Kan, Seq, 0);
        Mot_play(pEm, 0xB, 0, 0);
    }
}

static void Em23_haziki_br(ENEMY_WORK *pEm) {}

static void Em23_haziki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em23_taiki_br(ENEMY_WORK *pEm) {}

static void Em23_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em23_last_br(ENEMY_WORK *pEm) {}

static void Em23_last(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        Em23_Bite(pEm);
        EF(pEm)->Dm_timer = 0xFF;
        Mot_play_sp(pEm, 0, 0x20, 0);
        EF(pEm)->Attribute |= 0x10;
        if (pEm->Move_cnt > 0x42) pEm->Routine_2++;
        break;
    case 1:

        EF(pEm)->Attribute |= 2;
        EF(pEm)->Attribute &= ~0x10;

        if (G.Player.Life < 0)
            *(unsigned long *)&pEm->Routine_0 = 0x601;
        else {
            EF(pEm)->Attribute |= 4;
            *(unsigned long *)&pEm->Routine_0 = 0x401;
        }

    }
}

static void Em23_Bite(ENEMY_WORK *pEm)
{
    if (G.Player.At_em_flg && !(G.Status_flg & 0x10) && !G.Player.Damage_cnt &&
        !S8(EF(pEm)->Mouse_rtn) && !(pEm->Neck_flg & 4) &&
        (EF(pEm)->Gget_rtn == 0 || EF(pEm)->Gget_rtn == 7)) {

        EF(pEm)->Mouse_rtn = 1;
}   }

static void Em23_mouse_move(ENEMY_WORK *pEm)
{
    SVECTOR Svec;

    switch (S8(EF(pEm)->Mouse_rtn)) {
    case 0:
        break;

    case 1:
        pEm->Timer0 = 0;
        EF(pEm)->Mouse_rtn++;

    case 2:

        pEm->Timer0++;
        EF(pEm)->Neck_ctr += Em23_get_spd(4, pEm->Timer0, 0x14);
        EF(pEm)->Mouse_ctr += Em23_get_spd(4, pEm->Timer0, -0x64);
        if (pEm->Timer0 == 4) {
            pEm->Timer0 = 0;
            EF(pEm)->Mouse_rtn++;
        }
        break;

    case 3:
        pEm->Timer0++;
        EF(pEm)->Neck_ctr += Em23_get_spd_r(4, pEm->Timer0, 0x14);
        EF(pEm)->Mouse_ctr += Em23_get_spd_r(4, pEm->Timer0, -0x64);
        if (pEm->Timer0 == 4) {
            pEm->Timer0 = 0;
            EF(pEm)->Mouse_rtn++;
        }
        break;

    case 4:
        pEm->Timer0++;
        EF(pEm)->Neck_ctr += Em23_get_spd(3, pEm->Timer0, -0x14);
        EF(pEm)->Mouse_ctr += Em23_get_spd(3, pEm->Timer0, 0x64);
        if (pEm->Timer0 == 3) {
            pEm->Timer0 = 0;
            EF(pEm)->Mouse_rtn++;
        }

        Pl_bite_dm(pEm); break;

    case 5:
        pEm->Timer0++;
        EF(pEm)->Neck_ctr += Em23_get_spd_r(3, pEm->Timer0, -0x14);
        EF(pEm)->Mouse_ctr += Em23_get_spd_r(3, pEm->Timer0, 0x64);
        if (pEm->Timer0 == 3) {
            pEm->Timer0 = 0;
            EF(pEm)->Mouse_rtn = 0;
        }
        Pl_bite_dm(pEm);
        break;
    }
}

static unsigned long Pl_bite_dm(ENEMY_WORK *pEm)
{

    if (G.Player.At_em_flg && !G.Player.Damage_cnt) {

        G.Player.pEnemy_ptr = (unsigned long *)pEm;
        *(unsigned long *)&G.Player.Routine_0 = 0x205;

        if (Cdir_ck(pEm, &G.Player)) G.Player.Routine_1++;

        if (pEm->Pos_z > -0x2328) {

            if (Cdir_ck(pEm, &G.Player))
                Goto01((ENEMY_WORK *)&G.Player, 0, 0x600);
            else
                Goto01((ENEMY_WORK *)&G.Player, 0x800, 0x600);

        } else {

            if (pEm->Pos_x > -0x3CF0) {

                if (Cdir_ck(pEm, &G.Player))
                    Goto01((ENEMY_WORK *)&G.Player, 0x800, 0x600);
                else
                    Goto01((ENEMY_WORK *)&G.Player, 0, 0x600);
            } else {

                if (Cdir_ck(pEm, &G.Player))
                    Goto01((ENEMY_WORK *)&G.Player, 0xC00, 0x600);
                else
                    Goto01((ENEMY_WORK *)&G.Player, 0x400, 0x600);
            }
        }

        Em23_add_dm((EF(pEm)->Attribute & 0x20) ? 30 : 60); /* retail: 0x41 -> 30 / 60 (@0x80101694) */
        EF(pEm)->Retail_20 = 70; /* retail-only, sb @0x801016B4 */
        EF(pEm)->Dm_eff_rtn = 0;
        Bite_blood(pEm);
    }
}

static unsigned long Player_stumble_ck(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register long x, z;

    if (!G.Player.Damage_cnt) {

        x = G.Player.Pos_x - pParts->Workm.t[0];
        z = G.Player.Pos_z - pParts->Workm.t[2];

        if (SquareRoot0(x * x + z * z) < 0x1450) {

            EF(pEm)->Attribute |= 2;
            EF(pEm)->Stumble_cnt = 8;

            G.Player.pEnemy_ptr = (unsigned long *)pEm;
            *(unsigned long *)&G.Player.Routine_0 = 0x105;
            pEm->Be_flg |= 0x100A;
            G.Player.Be_flg |= 0x1008;
            G.Player.Damage_cnt |= 0x80;
            G.Player.Life = -1;
    }   }
}

static unsigned long Gas_get(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register OBJ_MODEL_WORK *pOm = &G.Ob_model[1];
    MATRIX Loc_m;
    VECTOR Vec;
    SVECTOR Svec1 = {0, 0, 0, 0};
    SVECTOR Svec2;
    switch (EF(pEm)->Gget_rtn) {
    case 0:
        break;

    case 1:
        pEm->Timer1 = 0; Flg_on(G.SaveData.Room_flg, 0x1D);
        EF(pEm)->Gget_rtn++;

    case 2:
        EF(pEm)->Neck_ctr += Em23_get_spd(0xD, pEm->Timer1, 0xF0);
        EF(pEm)->Mouse_ctr += Em23_get_spd(0xD, pEm->Timer1, -0x140);
        pEm->Timer1++;
        if (pOm->Pos_z - pEm->Pos_z < 0x1FA4)
            pOm->Pos_z = pEm->Pos_z + 0x1FA4;

        if (pEm->Timer1 == 0xD) {
            pEm->Timer1 = 0;
            pOm->pSuper = (unsigned long *)&pParts[6].Workm;

            pEm->Neck_flg &= ~4;
            MulLMatrix(&pParts[6].Workm, &pOm->Workm, &Loc_m);
            pOm->Pos_x = Loc_m.t[0];
            pOm->Pos_y = Loc_m.t[1];
            pOm->Pos_z = Loc_m.t[2];
            pOm->Workm = Loc_m;
            Rot_add_matrix(&pOm->Pos_x, &pOm->Cdir_x, &pOm->Workm, &Svec1);

            EF(pEm)->Gget_rtn++;
        }
        break;

    case 3:
        EF(pEm)->Neck_ctr += Em23_get_spd(6, pEm->Timer1, -0x8C);
        EF(pEm)->Mouse_ctr += Em23_get_spd(6, pEm->Timer1, -0x3C);
        pEm->Timer1++;
        if (pOm->Pos_x != 0x6D6) Approach_one(&pOm->Pos_x, 0x6D6, 0x46);
        if (pEm->Timer1 == 6) {
            pEm->Timer1 = 0;
            EF(pEm)->Gget_rtn++;
        }
        break;

    case 4:
        EF(pEm)->Neck_ctr += Em23_get_spd_r(6, pEm->Timer1, -0x8C);
        EF(pEm)->Mouse_ctr += Em23_get_spd_r(6, pEm->Timer1, -0x3C);
        pEm->Timer1++;
        if (pOm->Pos_x != 0x6D6) Approach_one(&pOm->Pos_x, 0x6D6, 0x91);
        if (pOm->Pos_y != -0xD2) Approach_one(&pOm->Pos_y, -0xD2, 0x19);
        if (pEm->Timer1 == 6) {
            pEm->Timer1 = 0;
            EF(pEm)->Gget_rtn++;
        }
        break;

    case 5:
        EF(pEm)->Neck_ctr += Em23_get_spd(7, pEm->Timer1, 0x4B);
        EF(pEm)->Mouse_ctr += Em23_get_spd(7, pEm->Timer1, 0x64);
        pEm->Timer1++;
        if (pOm->Pos_x != 0x6D6) Approach_one(&pOm->Pos_x, 0x6D6, 0x64);
        if (pOm->Pos_y != -0xD2) Approach_one(&pOm->Pos_y, -0xD2, 0x37);
        if (pEm->Timer1 == 7) {
            pEm->Timer1 = 0;
            EF(pEm)->Gget_rtn++;
        }
        break;

    case 6:
        EF(pEm)->Neck_ctr += Em23_get_spd_r(7, pEm->Timer1, 0x4B);
        EF(pEm)->Mouse_ctr += Em23_get_spd_r(7, pEm->Timer1, 0x64);
        pEm->Timer1++;
        if (pEm->Timer1 == 7) {
            pEm->Timer1 = 0;
            EF(pEm)->Gget_rtn++;
        }
        break;

    case 7:
        if (pOm->Pos_x != 0x6D6) Approach_one(&pOm->Pos_x, 0x6D6, 0x64);
        if (pOm->Pos_y != -0xD2) Approach_one(&pOm->Pos_y, -0xD2, 0x1E);
        break;

    }
}

static long Em23_get_spd(long Rtime, long Ntime, long Dir)
{
    register long Ntime2;

    if (!Ntime)
        return 0;

    Ntime2 = Ntime - 1;
    return Dir * Ntime * Ntime / (Rtime * Rtime) - Dir * Ntime2 * Ntime2 / (Rtime * Rtime);

}

static long Em23_get_spd_r(long Rtime, long Ntime, long Dir)
{
    register long Ntime2; /* inferred name (cf. Em23_get_spd): retail opens a function block here but keeps no record, i.e. a deleted local */

    if (!Ntime)
        return Dir;

    Ntime = Rtime - Ntime + 1;

    return Dir * Ntime * Ntime / (Rtime * Rtime) - Dir * (Ntime - 1) * (Ntime - 1) / (Rtime * Rtime);

}

static void Earthquake_move(ENEMY_WORK *pEm)
{
    switch (EF(pEm)->Earthquake_ctr) {
    case 0:
        break;

    case 1:
        Pad_vib_fade_set(10, 120, 50, 0); /* retail-only, jal @0x80101D8C */
        pEm->Timer2 = 2;
        EF(pEm)->Earthquake_ctr++;
    case 2:
        if (--S8(pEm->Timer2) == 0) {
            EF(pEm)->Earthquake_ctr++;
        }

        break;

    case 3:
        pEm->Timer2 = 5;
        G.Status_flg |= 4;
        EF(pEm)->Earthquake_ctr++;

    case 4:
        if (--S8(pEm->Timer2) == 0) {
            pEm->Timer2 = 10;
            Pad_vib_fade_set(8, 85, 40, 0); /* retail-only, jal @0x80101E24 */
            G.Status_flg &= ~4;
            G.Status_flg |= 8;
            EF(pEm)->Earthquake_ctr++;
        }
        break;

    case 5:
        if (--S8(pEm->Timer2) == 0) {
            G.Status_flg &= ~8;
            EF(pEm)->Earthquake_ctr = 0;
        }

    } }

static void Em23_damage_ck(ENEMY_WORK *pEm)
{
    if (pEm->Life < 25000) EF(pEm)->Attribute |= 0x20;

    if (G.Player.Life >= 0) {
        /* retail (0x80101EF0..0x80101F18): the Gget_rtn == 7 test comes first and excludes the rest */
        if (EF(pEm)->Gget_rtn == 7) {
            if (EF(pEm)->Dm_rtn_type != 1) {
                pEm->Damage_no = 1; return;
            }
        } else {

        if ((EF(pEm)->Attribute & 0x10) && G.SaveData.Cut_no == 8) {

          if (pEm->Life < 25000) {

            if (G.Status_flg & 0x40000000) {

                EF(pEm)->Mouse_ctr = 0;
                EF(pEm)->Neck_ctr = 0;
                EF(pEm)->Mouse_rtn = 0;
                pEm->Damage_no = 3; } else {

                EF(pEm)->Mouse_rtn = 0;
                pEm->Damage_no = 2;
            }
            return;
        } }

        if (!S8(EF(pEm)->Turn_flg) && !EF(pEm)->Dm_timer && !(Rnd() & 3)) {

            pEm->Damage_no = 4; EF(pEm)->Dm_timer = (Rnd() & 0x3F) + 0x78;
            return;
    }   } } pEm->Damage_no = 0;

}

static void Em23_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_1 != 0xFF) {
        EF(pEm)->Dm_rtn_type = pEm->Routine_1;
        pEm->Routine_1 = 0xFF;
        Em23_damage_ck(pEm);
        if (pEm->Move_cnt > 1) pEm->Move_cnt -= 2;
    }

    Em23_damage[pEm->Damage_no](pEm, Kan, Seq); }

static void Em23_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (!(*(unsigned long *)&EF(pEm)->Dm_rtn_type & 0xFFFF0000))
        Snd_se_enem(5, pEm);
    Dm_eff(pEm);

    *(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
    if (pEm->Routine_0 == 1) {
        Em23_mv_br[pEm->Routine_1](pEm);
        Em23_mv[pEm->Routine_1](pEm, Kan, Seq);
    }

}

static void Em23_explosion(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register OBJ_MODEL_WORK *pOm = &G.Ob_model[1];
    SVECTOR Svec = {0, 0, 0, 0};
    static int Dir;
    static short Spd_add;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(4, pEm);
        Em_dead_flg_on(pEm);
        G.Status_flg |= 0x100;

        /* retail (0x80102204..0x80102294): cut 6 or 13, new cut 5, Attribute 0x40 remembers the change */
        if ((G.SaveData.Cut_no == 6 || G.SaveData.Cut_no == 13) && pEm->Pos_z < -0x33F4) {
            EF(pEm)->Attribute |= 0x40;
            G.Player.Be_flg |= 0x80400;
            Cut_change(5);
            Main.Bg_chg_flg = 1;
        } else
            EF(pEm)->Attribute &= ~0x40;
        G.Status_flg |= 4;
        EF(pEm)->Earthquake_ctr = 1;
        EF(pEm)->Gget_rtn = 0xD;
        EF(pEm)->Attribute |= 2;
        EF(pEm)->Neck_ctr = 0;
        EF(pEm)->Mouse_ctr = 0;
        EF(pEm)->Mouse_rtn = 0;
        pEm->Be_flg |= 0x80A;
        pEm->Size = 0xE5E;
        pEm->Damage_cnt |= 0x80;

        pOm->pSuper = (unsigned long *)&GsIDMATRIX;
        pOm->Pos_x = pOm->Workm.t[0];
        pOm->Pos_y = pOm->Workm.t[1];
        pOm->Pos_z = pOm->Workm.t[2];
        Rot_add_matrix(&pOm->Pos_x, &pOm->Cdir_x, &pOm->Workm, &Svec);
        Pad_vib_set0(8, 3); /* retail-only, jal @0x8010232C */
        Pad_vib_fade_set(50, 255, 0, 0); /* retail-only, jal @0x80102340 */

        Om_explosion_set(pOm);
        Em_explosion_set(pEm, &pParts[7]);

        Main.Vloop = 4;
        Explosion_blood(pEm);
        Gas_explosion(pEm, 0);
        Snd_se_stad(0x20F0001, &pEm->Pos_x); /* retail: (Pos_x | 0x20F0100, 0) -> (0x20F0001, &Pos_x) */

        pEm->Timer3 = 9;
        EF(pEm)->Head_rtn = 1;
        Dir = pEm->Cdir_y + 0xA0;
        if (pEm->Cdir_y == 0xC00) {
            if (pEm->Pos_z > -0xBB8) pEm->Pos_z = -0xBB8;
            if (pEm->Move_no == 2)
                pEm->Cdir_y += (pEm->Move_cnt << 9) / 150;

            if (pEm->Move_no == 3)
                pEm->Cdir_y += 0x200 + (pEm->Move_cnt << 9) / 150;
        }

        if (pEm->Pos_x > -0x5AFF) pEm->Pos_z -= 1000;
        pEm->Routine_2++;
        Spd_add = 0x28;
        pEm->Spd_x = -0x1F4;

    case 1:
        if (Mot_play_sp(pEm, 9, 0, 0)) {
            if (EF(pEm)->Attribute & 0x40) { /* retail (0x801024A4..0x80102518) */
                EF(pEm)->Attribute &= ~0x40;
                Cut_change(((G.Status_flg & 0x40000000) && Flg_ck(G.SaveData.Zapping_flg, 16)) ? 13 : 6);
                Main.Bg_chg_flg = 1;
                G.Player.Be_flg &= ~0x80400;
            }
            pEm->Routine_2++;
        }
        if (pEm->Move_cnt == 0x1F) {
            G.Status_flg |= 4;
            EF(pEm)->Earthquake_ctr = 1;
        }

        if ((signed char)pEm->Timer3 > 0) {
            pEm->Timer3--;
        } else
            pOm->Be_flg = 0;

        pEm->Spd_x += Spd_add;
        if (Spd_add) Spd_add -= 2;
        Add_speedXZ(pEm, 0x800);

        if (Dir == 0xCA0) {

            if (pEm->Pos_x != -0x64C8) Approach_one(&pEm->Pos_x, -0x64C8, 0xC8);
        } else {

            if (pEm->Pos_z != 0x258) Approach_one(&pEm->Pos_z, 0x258, 0xC8);

        }
        Goto01(pEm, Dir, 0x4B);
        if (pEm->Size > 0xD96)
            pEm->Size -= 0x14;
        else

            pEm->Size = 0xD96; break;

    case 2:
        EF(pEm)->Attribute &= ~4;

        pEm->Routine_2++;
        break;

    case 3:
        pEm->Type |= 0x4000; /* retail-only, sh @0x80102664 */
        Flg_on(G.SaveData.Room_flg, 0x1E);
        EF(pEm)->Neck_ctr = 0;
        EF(pEm)->Mouse_ctr = 0;
        EF(pEm)->Mouse_rtn = 0;
        Main.Vloop = 2;
        G.Status_flg &= ~0x100;
    }

}

static void Em23_dm_escape(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        G.Status_flg |= 0x10;
        G.Stop_flg |= 0x1000000;
        G.Status_flg |= 0x100;
        pEm->Damage_cnt |= 0x80;
        EF(pEm)->Attribute |= 2;

        pEm->Routine_2++;
        break;

    case 1:
        if (G.Status_flg & 0x80) break;

        G.Player.Be_flg |= 0x80400; /* retail: 0x80000 -> 0x80400 (@0x80102764) */
        Cut_change(9);
        Main.Bg_chg_flg = 1; /* retail-only, sb Main+0x13B @0x80102794 */

        if (S8(EF(pEm)->Turn_flg)) {

            pEm->Pos_x = -0x4E9D;
            pEm->Pos_z = -0x388;

            pEm->Cdir_y = 0xC00; } else
            pEm->Cdir_y -= 0x400;

        pEm->Routine_2++;
    case 2:
        if (Mot_play_sp(pEm, 7, 0, 0)) {
            pEm->Routine_2++;
            pEm->Timer3 = 0;
            Snd_se_stad(0x20E0001, &pEm->Pos_x); /* retail: moved here from Timer3 == 4 (jal @0x8010280C) */
        }
        break;

    case 3:
        pEm->Timer3 += Mot_play_sp(pEm, 1, 7, 0); /* retail: 3rd arg 0 -> 7 (li a2 @0x80102824) */

        if ((signed char)pEm->Timer3 == 4) {
            EF(pEm)->Attribute |= 8;
        }
        if (*(signed char *)&pEm->Timer3 == 5) {
            pEm->Routine_2++;
            Cut_change(8);
            Main.Bg_chg_flg = 1; /* retail-only, sb @0x801028B4 */
            G.Status_flg &= ~0x100;
            G.Status_flg &= ~0x10;
            G.Stop_flg &= ~0x1000000;
            pEm->Type |= 0x4000; /* retail-only, sh @0x801028EC */
            G.Player.Be_flg &= ~0x80400; /* retail: ~0x80000 -> ~0x80400 */
            Mot_play_sp(pEm, 8, 0, 0);
        }
        break;
    case 4:
        break;
    }
}

static void Em23_died(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        Em_dead_flg_on(pEm);

        G.Status_flg |= 0x10;
        G.Stop_flg |= 0x1000000;
        G.Status_flg |= 0x100;
        pEm->Damage_cnt |= 0x80;
        EF(pEm)->Attribute |= 2;
        pEm->Routine_3 = 0;
        pEm->Neck_flg |= 4;
        pEm->Neck_point_x = -0x2420;
        pEm->Neck_point_y = -0x708;
        pEm->Neck_point_z = 0x3CA;
        pEm->Routine_2++;

    case 1:
        if (G.Status_flg & 0x80) break;

        if (S8(EF(pEm)->Turn_flg)) {

            pEm->Pos_x = -0x4E9D;
            pEm->Pos_z = -0x388;
            pEm->Cdir_y = 0x1000;
        }

        G.Player.Be_flg |= 0x80400; /* retail: 0x80000 -> 0x80400 */
        /* retail (0x80102A54..0x80102A7C): cut 14 or 7 instead of 9 */
        Cut_change(((G.Status_flg & 0x40000000) && Flg_ck(G.SaveData.Zapping_flg, 16)) ? 14 : 7);
        Main.Bg_chg_flg = 1; /* retail-only, sb @0x80102A8C */
        pEm->Routine_2++;

    case 2:
        if (!pEm->Routine_3)
            pEm->Routine_3 = Approach_one(&pEm->Pos_z, 0, 0x14);
        if (Mot_play_sp(pEm, 8, 0, 0)) { /* retail: 3rd arg 4 -> 0 */
            pEm->Type |= 0x4000; /* retail-only, sh @0x80102AF0 */
            Cut_change(8);
            Main.Bg_chg_flg = 1; /* retail-only, sb @0x80102B14 */
            G.Status_flg &= ~0x100;
            G.Status_flg &= ~0x10;
            G.Stop_flg &= ~0x1000000;
            G.Player.Be_flg &= ~0x80400; /* retail: ~0x80000 -> ~0x80400 */

            *(unsigned long *)&pEm->Routine_0 = 0x601;
            pEm->Atd[3].Ofs_x = -0x7D0;
            pEm->Atd[3].Ofs_y = 0;
            pEm->Atd[3].Ofs_z = 0x1B58;
            pEm->Atd[3].At_w = pEm->Atd[3].W = 0x1BEE;
            pEm->Atd[3].At_h = 0x7D0;
            pEm->Atd[3].At_d = pEm->Atd[3].D = 0x1BEE;

            pEm->Atd[1].Ofs_x = -0x96;
            pEm->Atd[1].Ofs_y = 0;
            pEm->Atd[1].Ofs_z = 0x12C0;
            pEm->Atd[1].At_w = pEm->Atd[1].W = 0x157C;
            pEm->Atd[1].At_h = 0x7D0;
            pEm->Atd[1].At_d = pEm->Atd[1].D = 0x157C;
        }

    }
}

static void Em23_dm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        if (!EF(pEm)->Roar_cnt) Snd_se_enem(0, pEm);
        pEm->Damage_cnt |= 0x80;
        Dm_eff(pEm);
        pEm->Routine_2++;

    case 1:
        if (Mot_play_sp(pEm, 0xA, 3, 0)) {
            *(unsigned long *)&pEm->Routine_0 = 0x101;
            pEm->Damage_cnt &= 0x7F;
        }
    }
}

static void Em23_die_ck(ENEMY_WORK *pEm)
{
    pEm->Damage_no = 0; }

static void Em23_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_1 != 0xFF) {
        pEm->Routine_1 = 0xFF;
        Em23_die_ck(pEm);
    }

    Em23_die[pEm->Damage_no](pEm, Kan, Seq); }

static void Em23_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

void Em123_Pldm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em23_pldm_tbl[pEm->Routine_1](pEm, Kan, Seq); }

static void Em23_Pl_die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        /* retail-only vibration sequence (0x80102DA8..0x80102E4C) */
        Pad_vib_fade_set(10, 40, 150, 11);
        Pad_vib_set0(3, 22);
        Pad_vib_set0(3, 26);
        Pad_vib_fade_set(65, 240, 120, 21);
        Pad_vib_fade_set(10, 245, 70, 86);
        Pad_vib_fade_set(5, 70, 140, 106);
        Pad_vib_set0(3, 110);
        Pad_vib_fade_set(20, 255, 120, 108);
        Pad_vib_fade_set(20, 190, 100, 128);
        Pad_vib_fade_set(145, 160, 30, 148);

        pEm->Timer0 = 0;
        G.Stop_flg |= 0x1000000;
        pEm->Routine_2++;
    case 1:
        pEm->Move_cnt = 0;
        *(unsigned long *)&pEm->Move_no = 0x30002;
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
        if (pEm->Timer0) {
            *(unsigned long *)&pEm->Move_no = 1;
            pEm->Routine_2++;
        }
        break;

    case 2:
        Null_pos_set(pEm, Kan, Seq, 0);
        if (pEm->Move_cnt == 3)
            Snd_se_stad(0x4020001, &pEm->Pos_x);
        pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x1000);
        break;

    case 3:
        G.Player.Damage_cnt = 0x80;
    }
}

static void Em23_Pl_stumble(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register ENEMY_WORK *pWani = (ENEMY_WORK *)pEm->pEnemy_ptr;
    register short Dir; /* inferred name: the compiler deletes it, retail SYM has no record */

    switch (pEm->Routine_2) {
    case 0:

        Snd_se_stad(0x4030001, &pEm->Pos_x);
        Pad_vib_fade_set(53, 240, 40, 0); /* retail-only, jal @0x80102F9C */

        *(unsigned long *)&pEm->Move_no = 0x300;
        G.System_flg |= 0x4000000;
        pEm->Routine_3 = 0;
        pEm->Routine_2++;
    case 1:
        if (pEm->Routine_3 < 3) {
            Pl_stumble_blood(pEm->Routine_3);
            pEm->Routine_3++;
        }
        Dir = Direction_ck(pWani->Pos_x, pWani->Pos_z, pEm->Pos_x, pEm->Pos_z);

        Goto01(pEm, Dir, 0x80);
        pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
        break;
    case 2:
        break;
    }
}

static void Em2a_pl_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long Tmp;

    switch (pEm->Routine_2) {
    case 0:

        Snd_se_stad(0x4010001, &pEm->Pos_x);

        *(unsigned long *)&pEm->Move_no = 0x30012;
        pEm->Routine_2 = 1;
        pEm->Spd_x = -0x320;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;

        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;
    case 1:
        if (!pEm->Hokan_flg)
            pEm->Routine_2 = 2;

        Add_speedXZ(pEm, 0);
        Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

        pEm->Spd_x += 0x96;
        break;

    case 2:
        *(unsigned long *)&pEm->Move_no = 0x30013;
        pEm->Routine_2 = 3;
    case 3:
        Add_speedXZ(pEm, 0);
        Tmp = Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

        Tmp += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

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

        Snd_se_stad(0x4020001, &pEm->Pos_x);

        *(unsigned long *)&pEm->Move_no = 0x30014;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x320;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;

        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;

    case 1:
        Add_speedXZ(pEm, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

        if (pEm->Hokan_flg) /* both arms equal here (EM2A.C uses 0x1E / 0x3C); GCC merges them */
            pEm->Spd_x -= 0x78;
        else

            pEm->Spd_x -= 0x78;

        if (pEm->Spd_x < 0)

            pEm->Spd_x = 0; break;

    case 2:
    case 4:

        *(unsigned long *)&pEm->Move_no = 0x30011;
        pEm->Routine_2 = 5;
        pEm->Damage_cnt = 0;

    case 5:
        Tmp = Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

        Tmp += Joint_move(pEm, pEm->pRoom_kan_t_ptr, pEm->pRoom_seq_t_ptr, 0x400);

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

static void Gas_explosion(ENEMY_WORK *pEm, unsigned long Rtn)
{
    register OBJ_MODEL_WORK *pOm = &G.Ob_model[1];

    SVECTOR Ob_vec[6] = {
        { 0, -2800, 0, 0 }, { 300, -2100, 0, 0 }, { 0, -700, 0, 0 },
        { 0, 0, 300, 0 }, { 0, -1400, -200, 0 }, { -500, -400, 200, 0 }
    };

    Esp_call(0x30F2000, 0, &pOm->Workm, &Ob_vec[0]);
    Esp_call(0x30F3000, 0, &pOm->Workm, &Ob_vec[1]);
    Esp_call(0x30F3000, 0, &pOm->Workm, &Ob_vec[2]);
    Esp_call(0x30F2000, 0, &pOm->Workm, &Ob_vec[3]);
    Esp_call(0x30F2000, 0, &pOm->Workm, &Ob_vec[4]);
    Esp_call(0x30F3000, 0, &pOm->Workm, &Ob_vec[5]);
}

static void Pl_stumble_blood(unsigned long Rtn)
{
    register PARTS_W *pParts;
    EM23_ESP Pos_tbl[9] = {
        { 0, 0, 0, 2128, 4 }, { 0, 0, 0, 1920, 7 }, { 0, 200, 0, 1334, 9 },
        { 0, 200, 0, 1334, 3 }, { 0, 300, 0, 1834, 2 }, { 0, 200, 0, 4034, 5 },
        { -300, -500, 200, 2534, 1 }, { 0, 0, 200, 3634, 9 }, { -200, 600, 400, 1534, 2 }
    };

    pParts = G.Player.pSin_parts_ptr + Pos_tbl[Rtn * 3].Parts;
    Esp_call(0x11800, Pos_tbl[Rtn * 3].Dir_y, &pParts[0].Workm, &Pos_tbl[Rtn * 3]);

    Esp_call(0x21000, Pos_tbl[Rtn * 3].Dir_y, &pParts[1].Workm, &Pos_tbl[Rtn * 3 + 1]);

    Esp_call(0x11800, Pos_tbl[Rtn * 3].Dir_y, &pParts[2].Workm, &Pos_tbl[Rtn * 3 + 2]);

    Esp_call(0x22000, Pos_tbl[Rtn * 3].Dir_y + 0x400, &pParts[0].Workm, &Pos_tbl[Rtn * 3]);

    Esp_call(0x1800, Pos_tbl[Rtn * 3].Dir_y + 0x400, &pParts[1].Workm, &Pos_tbl[Rtn * 3 + 1]);

    Esp_call(0x1000, Pos_tbl[Rtn * 3].Dir_y + 0x400, &pParts[2].Workm, &Pos_tbl[Rtn * 3 + 2]);

}

static void Em23_Head_flying(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = &pEm->pSin_parts_ptr[7];
    static short Spd_z, Spd_y, Sp_add_y, Cspd_z, Csp_add_z;
    SVECTOR Svec;
    register short Dir; /* inferred name: the compiler deletes it, retail SYM has no record */

    switch (EF(pEm)->Head_rtn) {
    case 0:
        break;

    case 1:
        pParts->pSuper = (unsigned long *)&GsIDMATRIX;
        pParts->Pos_x = pParts->Workm.t[0];
        pParts->Pos_y = pParts->Workm.t[1];
        pParts->Pos_z = pParts->Workm.t[2];
        Svec.vx = Svec.vy = Svec.vz = 0;
        Rot_add_matrix(&pParts->Pos_x, &pParts->Cdir_x, &pParts->Workm, &Svec);
        Spd_z = -0x2BC;
        Spd_y = -0x1F4;
        Sp_add_y = 0x46;
        Cspd_z = -0xD2;
        Csp_add_z = 0xA;
        EF(pEm)->Head_rtn++;

    case 2:
        Dir = Direction_ck(pParts->Pos_x, pParts->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

        Svec.vx = Spd_z;
        Svec.vz = 0;
        Rot_vector(Dir, &Svec, &Svec);
        pParts->Pos_x += Svec.vx;
        pParts->Pos_z += Svec.vz;
        pParts->Pos_y += Spd_y;
        pParts->Cdir_z += Cspd_z;

        Spd_y += Sp_add_y;
        if (pParts->Pos_y >= 0) {
            pParts->Pos_y = 0;
            EF(pEm)->Head_rtn = 0;
        }
        Cspd_z += Csp_add_z;
        if (Cspd_z >= 0) {
            Cspd_z = 0;
            Csp_add_z = 0;
        }
    }
}

static void Em23_Bomb_tbl_set(void)
{
    register unsigned long i;

    for (i = 0; i < 30; i++)
        Bomb_tbl[0][i] *= 6;

}

static void Em23_Bomb_tbl_reset(void)
{
    register unsigned long i;

    for (i = 0; i < 30; i++)
        Bomb_tbl[0][i] /= 6;

}

static void Em23_add_dm(unsigned long Att)
{
    register long Pl_life;

    Pad_vib_set0(3, 0); /* retail-only, jal @0x801038F8 */
    Pad_vib_fade_set(14, 210, 40, 2); /* retail-only, jal @0x8010390C */
    Pl_life = Pl_life_down(Att, 0);
    if (Pl_life & 1)

        G.Player.Routine_2 = 4;

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

static unsigned long Dm_eff(ENEMY_WORK *pEm)
{

    EF(pEm)->Dm_eff_rtn = 0; switch (EF(pEm)->Dm_rtn_type) {
    case 1:
    case 12:
        break;
    case 2: case 3:
        Dm_eff_blood(pEm, 1, 0, 6, 0x2800, 0);
        break;
    case 4:
        Dm_eff_3handgun(pEm);
        break;

    case 5: case 6:
        Dm_eff_blood(pEm, 1, 0, 6, 0x2800, 0);
        break;
    case 7:
        Dm_eff_shotgun(pEm, 4);

        break;
    case 8:
        Dm_eff_shotgun(pEm, 6);

        break;

    case 11:
        Color_chg(pEm, 0x17, 0x60, 0x60, 0x60);
        Dm_eff_Frame(pEm, 4, 9); /* retail: 3 -> 9 (li a2 @0x80103AA8) */
        break;

    case 13:
        Dm_eff_blood(pEm, 1, 0, 6, 0x2800, 0);
        break;
    case 14:
        Color_chg(pEm, 0x17, 0x60, 0x60, 0x60);
        Dm_eff_spark(pEm);
        break;
    case 15:
        Dm_eff_blood(pEm, 1, 0, 6, 0x2800, 0);
        break;

    case 18:
        Dm_eff_blood(pEm, 4, 0, 6, 0x2800, 0);
        break;
    case 9: case 10: case 16: case 17: case 19:
        Dm_eff_Frame(pEm, 5, 4);
        Color_chg(pEm, 0x17, 0x60, 0x60, 0x60);
    } }

static unsigned long Dm_eff_blood(ENEMY_WORK *pEm, unsigned long No, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Flg)
{
    register EM23_ESP *pEsp;

    while (No-- > 0) {

        if (Flg)

            pEsp = &Blood_tbl[Rnd() % 24];
        else

            pEsp = &Blood_tbl[Rnd() & 7];

        Set_esp_w(pEm, Id, Type, Zoom, pEsp->Dir_y + ((signed char)Rnd() << 4), pEsp->Parts, (SVECTOR *)pEsp);

    }
}

static void Dm_eff_3handgun(ENEMY_WORK *pEm)
{
    static unsigned char Cnt;
    register EM23_ESP *pEsp;
    SVECTOR Svec;
    static unsigned long Tbl_no;

    if (!EF(pEm)->Rensha_cnt) Tbl_no = Rnd() & 7;
    EF(pEm)->Rensha_cnt = 4;

    pEsp = &Blood_tbl[Tbl_no];
    Svec.vx = pEsp->X + (Rnd() * 2 - 0x7F);
    Svec.vy = pEsp->Y + (Rnd() * 2 - 0x7F);
    Svec.vz = pEsp->Z + (Rnd() * 2 - 0x7F);
    Set_esp_w(pEm, 0, 6, 0x2800, pEsp->Dir_y, pEsp->Parts, &Svec);
}

static void Bite_blood(ENEMY_WORK *pEm)
{
    register short Dir;
    SVECTOR Svec;
    register PLAYER_WORK *pPl = &G.Player;

    switch (EF(pEm)->Dm_eff_rtn) {
    case 0:
        EF(pEm)->Dm_eff_func = Bite_blood;
        EF(pEm)->Dm_eff_rtn++;
        break;

    case 1:
    case 2:
    case 3:
        Dir = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z);

        Svec.vx = (signed char)Rnd() + G.Player.Pos_x;
        Svec.vy = G.Player.Pos_y - 0x320 + (signed char)Rnd();
        Svec.vz = (signed char)Rnd() + G.Player.Pos_z;
        Esp_call(0x12000, Dir + ((signed char)Rnd() << 4), &GsIDMATRIX, &Svec);

        if (EF(pEm)->Dm_eff_rtn == 3) EF(pEm)->Dm_eff_func = 0;
        EF(pEm)->Dm_eff_rtn++;
    }
}

static void Explosion_blood(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;

    switch (EF(pEm)->Dm_eff_rtn) {
    case 0:
        EF(pEm)->Dm_eff_func = Explosion_blood;
        EF(pEm)->Dm_eff_rtn++;
        break;

    case 1:
        Svec.vx = ((signed char)Rnd() << 1) + 0x12C;
        Svec.vy = (signed char)Rnd() << 1;
        Svec.vz = ((signed char)Rnd() << 2) + 0xC8;
        Esp_call(0x26032000, 0, &pParts[6].Workm, &Svec);

    case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        Svec.vx = ((signed char)Rnd() << 2) + 0x12C;
        Svec.vy = (signed char)Rnd() << 1;
        Svec.vz = ((signed char)Rnd() << 1) + 0xC8;
        Esp_call(0x64000, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);
        Svec.vx = ((signed char)Rnd() << 2) + 0x12C;
        Svec.vy = (signed char)Rnd() << 1;
        Svec.vz = ((signed char)Rnd() << 1) + 0xC8;
        Esp_call(0x34000, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);

        Svec.vx = ((signed char)Rnd() << 1) + 0x12C;
        Svec.vy = (signed char)Rnd() << 1;
        Svec.vz = ((signed char)Rnd() << 2) + 0xC8;
        Esp_call(0x8024000, (signed char)Rnd() << 4, &pParts[6].Workm, &Svec);
        Esp_call(0x8024000, (signed char)Rnd() << 4, &pParts[6].Workm, &Svec);

    default:
        if (EF(pEm)->Dm_eff_rtn == 0x40) EF(pEm)->Dm_eff_func = 0;
        EF(pEm)->Dm_eff_rtn++;
    }
}

static void Dm_eff_Frame(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    SVECTOR Svec;

    Svec.vx = (signed char)Rnd() << 1;
    Svec.vy = -0x190;
    Svec.vz = 0;
    Esp_call(Id << 24 | Type << 16 | 0x2000, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);

    Svec.vx = ((signed char)Rnd() << 1) + 0x12C;
    Svec.vy = -0x12C;
    Svec.vz = 0x384;
    Esp_call(Id << 24 | Type << 16 | 0x2000, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);

    Svec.vx = ((signed char)Rnd() << 1) - 0x258;
    Svec.vy = -0xC8;
    Svec.vz = -0x3E8;
    Esp_call(Id << 24 | Type << 16 | 0x2000, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);

    Svec.vx = ((signed char)Rnd() << 1) - 0x190;
    Svec.vy = -0x190;
    Svec.vz = 0xC8;
    Esp_call(Id << 24 | Type << 16 | 0x2800, (signed char)Rnd() << 4, &pParts[7].Workm, &Svec);
}

static void Dm_eff_shotgun(ENEMY_WORK *pEm, unsigned long Num)
{
    static unsigned long Limit;

    if (!EF(pEm)->Dm_eff_rtn) {

        EF(pEm)->Dm_eff_func = Dm_eff_shotgun;
        EF(pEm)->Dm_eff_rtn++;
        Limit = Num;
    } else {

        Dm_eff_blood(pEm, 1, 0, 6, 0x3000, 1);
        Dm_eff_blood(pEm, 1, 0, 6, 0x3000, 1);
        if (EF(pEm)->Dm_eff_rtn == Limit) EF(pEm)->Dm_eff_func = 0;
        EF(pEm)->Dm_eff_rtn++;
    }
}

static void Em_explosion_set(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
    pP_ptr->Be_flg |= 0x4A;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->V0_x = pEm->Cdir_y + 0x800;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->Cnt0 = 0x8000;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->V0_z = 0;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->V0_y = -0xA;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->G = 0;
    ((PARTS_BOMB_FREE_WORK *)pP_ptr->Free)->Gxz = 0;
    pP_ptr->Poly_rgb = 0x202038; }

static void Dm_eff_spark(ENEMY_WORK *pEm)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register MATRIX *pMat;
    static DM_ESP2 *pEsp2;
    register unsigned long Next; /* inferred name: jump threading removes it, retail SYM has no record */

    if (!EF(pEm)->Dm_eff_rtn) {

        pEsp2 = Spark_tbl;
        EF(pEm)->Dm_eff_func = Dm_eff_spark;
        EF(pEm)->Dm_eff_rtn++;
    } else {

        do {
            if (!(pEsp2->Next_flg & 2)) {
                pMat = &pParts[pEsp2->Parts].Workm;
                Esp_call(0x6001800 | Rnd() << 1, 0, pMat, pEsp2);
            }
            if (pEsp2->Next_flg & 1) Next = 1; else Next = 0;

            pEsp2++;
        } while (Next);
        if (EF(pEm)->Dm_eff_rtn == 0x18) EF(pEm)->Dm_eff_func = 0;
        EF(pEm)->Dm_eff_rtn++;
    }
}

static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b)
{
    register PARTS_W *pParts = pEm->pSin_parts_ptr;
    register unsigned long i;

    for (i = 0; i < Num; i++)
        pParts[i].Poly_rgb = (pParts[i].Poly_rgb & 0xFF000000) | (b << 16 | g << 8 | r);

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
