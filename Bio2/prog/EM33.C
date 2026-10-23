/* em33.c - reconstruction of D:\Bio2\prog\EM33.C, the EM33 enemy module.  One source,
 * linked into two runtime overlays: 0x17 (enemy bin slot 0, 0x80100098..) and 0x35
 * (slot 1, 0x8010D098..).  The gate compiles it once per overlay with -DOVL_17 /
 * -DOVL_35; the only per-slot difference is the name of the three exported entry
 * points (Em1xx for slot 0, Em2xx for slot 1).  Types come from the TU's retail SYM
 * preamble (em33.h); the tables are the retail initialized data.  Engine routines are
 * called undeclared, as in the original: their implicit declarations open the retail
 * debug blocks. */
#include "EM33.H"

extern GLOBAL G;
extern MATRIX GsIDMATRIX;

/* the module's private work in ENEMY_WORK.Free[] */
#define EF(p) ((EM33_FREE *)ENEMY_FREE(p))

static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life = 250;	/* retail 250 (Beta 2: 300) */
static unsigned short Em2b_life_easy = 200;	/* retail 200 (Beta 2: 300) */
/* retail-only second life set ([INFERRED] _flg40 names as in em10/em20/em25; image bytes,
 * retail .data +0x30..+0x5F of this overlay; no retail code reads it here) */
static unsigned short Em23_life_flg40 = 240;
static unsigned short Em23_life_easy_flg40 = 240;
static unsigned short Em2a_life_flg40[10] = {
	250, 450, 300, 300, 220, 300, 400, 300, 400, 350
};
static unsigned short Em2a_life_easy_flg40[10] = {
	180, 340, 220, 240, 175, 240, 310, 240, 310, 300
};
static unsigned short Em2b_life_flg40 = 250;
static unsigned short Em2b_life_easy_flg40 = 200;
static char Em33_me_tate = 0;
static char Em33_me_yoko = 0;
static short Em33_me_tate_mv = 0;
static short Em33_me_yoko_mv = 0;
static short Em33_me_tate_a = 0;
static short Em33_me_yoko_a = 0;
static char Em33_me_tate_d = 0;
static char Em33_me_yoko_d = 0;
static short touch_tbl[10] = { 13, 1700, 19, 1500, 6, 800, 9, 1200, 6, 1200 };
/* retail: 12 entries (image 0x80106508 slot 1 / 0x80113508 slot 0; Beta 2 had 10, the next table moves +4) */
static short esc_rot_tbl[12] = {
	2048, 2560, 3072, 0, 1536, 1024, 3072, 3072, 1024, 1024, 3072, 0
};
static unsigned char dieblood_type_tbl[20] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0 };
static unsigned char blood_pos_tbl[4] = { 24, 1, 3, 0 };
static unsigned char blood_rnd[12] = { 23, 25, 26, 27, 2, 4, 17, 11, 2, 3, 2, 3 };
static unsigned char Mof_parts[8] = { 2, 3, 6, 9, 10, 17, 18, 19 };
static void (*Em33_routine_0[8])() = {
    Em33_Init,
    Em33_Move,
    Em33_Damage,
    Em33_Die,
    Em33_Scenario,
    0,
    0,
    Em33_Dead
};
static void (*Em33_Move_tbl[10])() = {
    Em33_Mv_tojo,
    Em33_Mv_taiki,
    Em33_Mv_walk,
    Em33_Mv_turn,
    Em33_Mv_atck,
    Em33_Mv_combo,
    Em33_Mv_critical,
    Em33_Mv_jump,
    Em33_Mv_yoro,
    Em33_Mv_ikaku
};
static void (*Mv_tojo_tbl[4])() = {
    Em33_Mv_tojo00,
    Em33_Mv_tojo10,
    Em33_Mv_tojo20,
    Em33_Mv_tojo30
};
static void (*Mv_atck_tbl[3])() = {
    Em33_Mv_atck00,
    Em33_Mv_atck10,
    Em33_Mv_atck20
};
static void (*Mv_combo_tbl[3])() = {
    Em33_Mv_combo00,
    Em33_Mv_combo10,
    Em33_Mv_combo20
};
static void (*Mv_critical_tbl[3])() = {
    Em33_Mv_critical00,
    Em33_Mv_critical10,
    Em33_Mv_critical20
};
static void (*Mv_jump_tbl[3])() = {
    Em33_Mv_jump00,
    Em33_Mv_jump10,
    Em33_Mv_jump20
};
/* retail .data 0x801065D4 / 0x801065E0: guide areas 10 -> 11 (area 10 new), values from the retail image */
static char down_dest_tbl[11] = { 0, 2, 2, 10, 5, 5, 6, 10, 8, 8, 10 };
static char jump_dest_tbl[242] = {
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1
};
static void (*em33_dmg_init[19])() = {
    Em33_dmg_nml,
    Em33_dmg_nml,
    Em33_dmg_nml,
    Em33_dmg_combo,
    Em33_dmg_mag,
    Em33_dmg_mag,
    Em33_dmg_nml,
    Em33_dmg_nml,
    Em33_dmg_fire,
    Em33_dmg_fire,
    Em33_dmg_H2SO4,
    Em33_dmg_nml,
    Em33_dmg_nml,
    Em33_dmg_spark,
    Em33_dmg_combo,
    Em33_dmg_fire,
    Em33_dmg_fire,	/* retail: entries 16 and 18 are Em33_dmg_fire (image 0x80106714 / 0x8010671C); Beta had Em33_dmg_nml */
    Em33_dmg_combo,
    Em33_dmg_fire
};
/* retail yoro_tbl00/yoro_tbl01 (image 0x80106720/0x80106734 slot 1, same in slot 0): entries [4] and [9] changed */
static unsigned char yoro_tbl00[20] = {
	10, 10, 10, 10, 20, 10, 10, 10, 10, 40, 10, 10, 10, 20, 4, 4, 40, 4, 10, 10
};
static unsigned char yoro_tbl01[20] = {
	10, 10, 10, 10, 20, 10, 20, 40, 20, 40, 40, 10, 10, 40, 10, 20, 40, 4, 10, 10
};
static void (*Em33_sce_tbl[5])() = {
    Em33_sce00,
    Em33_sce10,
    Em33_sce20,
    Em33_sce30,
    Em33_sce40
};
static void (*pldamage_tbl[4])() = {
    Em33_pldmg_push,
    Em33_pldmg_push,
    Em33_pldmg_combo1,
    Em33_pldmg_combo1
};
static unsigned char catch_parts_tbl[4] = { 9, 6, 21, 15 };
static void (*Mv_core_tbl[3])() = {
    Em33_Mv_core00,
    Em33_Mv_core10,
    Em33_Mv_core20
};
static EM33_BLOOD_TBL blood_id_tbl[10] = {
    { 0, 1, 0, 6144 },
    { 0, 1, 0, 8192 },
    { 133, 4, 0, 6144 },
    { 133, 1, 0, 4096 },
    { 132, 7, 8, 4096 },
    { 132, 7, 8, 6144 },
    { 1, 6, 0, 8192 },
    { 134, 0, 0, 4096 },
    { 132, 4, 8, 6144 },
    { 0, 5, 0, 8192 }
};

void Em133(ENEMY_WORK *pEm)
{
    register long xx, zz;
    register EM33_FREE *pFp; /* [INFERRED] retail keeps &ENEMY_FREE(pEm) in s1 from entry (0x801000A8) */

    pFp = EF(pEm);

    if (!(G.Stop_flg & 0x20000000)) {

        if (pEm->Damage_cnt & 0x7f)
            pEm->Damage_cnt--;

        if (EF(pEm)->No_atck[0])
            EF(pEm)->No_atck[0]--;

        if (EF(pEm)->No_atck[1])
            EF(pEm)->No_atck[1]--;

        if (EF(pEm)->No_jump)
            EF(pEm)->No_jump--;

        if (EF(pEm)->No_fire)
            EF(pEm)->No_fire--;

        Em33_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        Rot_neck_em(pEm, pEm->Cdir_y); /* retail-only */

        if (EF(pEm)->Biri_cnt) {
            EF(pEm)->Biri_cnt--;
            Set_blood(pEm, blood_rnd[(Rnd() & 7) + 2], 7);
            Set_blood(pEm, blood_rnd[(Rnd() & 7) + 2], 7);
        }

        Em33_Mv_core(pEm, pFp);
        Em33_core_me_mv(pEm, &pEm->pSin_parts_ptr[11]);
        Em33_Mv_heart(pEm, pFp);

        Em33_magdmg(pEm);

        Oba_ck_em(pEm);
        Sca_ck_em(pEm, 0x400);

        xx = abs(pEm->Pos_x - pEm->Old_x);
        zz = abs(pEm->Pos_z - pEm->Old_z);
        if (xx < 12 && zz < 12 && (pEm->Sca_info & 1)) {
            EF(pEm)->Stay++;
        } else {

            EF(pEm)->Stay = 0;
        }

    }
}

static void Em33_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register EM33_FREE *pFp;
    register unsigned long i;

    *(unsigned long *)&pEm->Routine_0 = 1;

    pFp = EF(pEm);

    pEm->Damage_flg = 0;
    pEm->Sce_flg = 0;
    pEm->Down_cnt = 0;
    for (i = 0; i < 12; i++) /* retail clears all 12 Free words */
        ENEMY_FREE(pEm)[i] = 0;

    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;

    /* retail: the "System_flg & 0x40" second life set (as Em10/Em28_Init), here with identical arms.
     * Lever: the two-armed if/else ends the CSE path, so the G.pPldamage_Em/G.Player constants below
     * are not rewritten relative to the &G.System_flg pseudo in a saved register; after reload the
     * identical arms cross-jump and jump.c deletes the outer branch with its dead "& 0x40" test. */
    if (G.System_flg & 0x40) {
        pEm->Life = (G.System_flg & 0x20) ? 750 : 900;
    } else {
        pEm->Life = (G.System_flg & 0x20) ? 750 : 900;
    }

    pFp->Dmg_stk = 40;

    pEm->Be_flg |= 0x1C000000;

    Kage_work9_set(&pEm->pKage_work, 0, 0x6400640, 0, &pEm->Pos_x);

    pEm->pEnemy_ptr = (unsigned long *)&G.Player;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 3;

    pP_ptr = &pEm->pSin_parts_ptr[3]; /* retail: neck part 3 (+0x29C @0x80100360), Beta part 0 */
    pP_ptr->Free[0] = 0;
    pP_ptr->Free[1] = 0;
    pP_ptr->Free[2] = 0x40;
    pP_ptr->Free[3] = 0x10;
    pP_ptr->Free[4] = 0x100;
    pP_ptr->Free[5] = 0x20;

    pP_ptr = pEm->pSin_parts_ptr;
    for (i = 0; i < 29; i++, pP_ptr++)
        pP_ptr->Be_flg |= 0x8000;

    pEm->nOba = 1;
    pEm->Atd[0].Ofs_x = 0;
    pEm->Atd[0].Ofs_y = -2000;
    pEm->Atd[0].Ofs_z = 0;
    pEm->Atd[0].At_w = 1100;
    pEm->Atd[0].At_d = 1100;
    pEm->Atd[0].At_h = 2000;
    pEm->Atd[0].W = 1200;
    pEm->Atd[0].D = 1200;
    pEm->Eff_at_r = 900;

    Em33_me_tate = 0;
    Em33_me_yoko = 0;
    Em33_me_tate_mv = 0;
    Em33_me_yoko_mv = 0;
    Em33_me_tate_a = 0;
    Em33_me_yoko_a = 0;
    Em33_me_tate_d = 0;
    Em33_me_yoko_d = 0;

    *(unsigned long *)&pEm->Move_no = 0;

    Joint_move(pEm, Kan, Seq, 0x100);

    G.pPldamage_Em[0x33 - 0x10] = Pldamage_Em133;
    G.pPldie_Em[0x33 - 0x10] = Pldie_Em133;

    switch (pEm->Type) {
    case 1:
        pEm->Routine_1 = 1;
        break;

    case 0x6002:
        *(unsigned long *)&pEm->Routine_0 = 4;
        break;
    }

    Set_sabun(pEm, 11, 0, 0);

}

static void Em33_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register EM33_FREE *pFp; /* inferred name: the compiler deletes it, retail SYM has no record */

    pFp = EF(pEm);

    Em33_Move_tbl[pEm->Routine_1](pEm, Kan, Seq);

    pFp->Rno1_stk = pEm->Routine_1;
    pFp->Rno2_stk = pEm->Routine_2;
    pFp->Rno3_stk = pEm->Routine_3;
    pFp->Spdx_stk = pEm->Spd_x;
    pFp->Spdy_stk = pEm->Spd_y;
    pFp->Spdz_stk = pEm->Spd_z;
    Em33_Yure_ck(pEm); /* retail @0x80100524 */

}

static void Em33_Mv_tojo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Mv_tojo_tbl[pEm->Routine_2](pEm, Kan, Seq); }

static void Em33_Mv_tojo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register unsigned long i;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;

        pEm->Sce_free1 = 0;

        *(unsigned long *)&pEm->Move_no = 0xF000A;
        pEm->Neck_flg |= 1; /* retail-only @0x801005C0 */
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);

        EF(pEm)->Heart_stop = 1;

        pEm->Be_flg |= 0x800;
        pEm->Size = 0xD98;

        for (i = 0; i < 8; i++)
            Set_sabun(pEm, Mof_parts[i], 0, 0xFFF);

        for (i = 20; i < 23; i++) {
            pP_ptr = &pEm->pSin_parts_ptr[i];
            pP_ptr->Be_flg |= 0x400;
            pP_ptr->Size_x = 0x1000;
            pP_ptr->Size_y = 0;
            pP_ptr->Size_z = 0x1000;
        }

        Set_sabun(pEm, 11, 0, 0xFFF);

        pEm->Damage_cnt |= 0x80;
    }

    if (pEm->Sce_free0 == 1)
        *(unsigned short *)&pEm->Routine_2 = 1;

}

static void Em33_Mv_tojo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Spd_get(pEm, Kan, Seq, 0);
    Joint_move(pEm, Kan, Seq, 0x100);
    Add_speedXZ(pEm, 0);

    if (pEm->Sce_free0 == 2)
        *(unsigned short *)&pEm->Routine_2 = 2;

    Em_seq_sound(pEm);

}

static unsigned long Mv_tojo20_0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq, short Rate)
{
    Joint_move(pEm, Kan, Seq, 0x100);
    pEm->Timer0 -= Rate;
    if (pEm->Timer0 < 0)
        pEm->Timer0 = 0;

    return *pEm->pNow_seq >> 16; }

static void Mv_tojo20_1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    pEm->Timer0 -= 0x200;
    if (pEm->Timer0 < 0)
        pEm->Timer0 = 0;

    Set_sabun(pEm, 9, 0, pEm->Timer0);
    Set_sabun(pEm, 6, 0, pEm->Timer0); }

static void Em33_Mv_tojo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register unsigned long i, chr_type;

    switch (pEm->Routine_3) {
    case 0:

        if (pEm->Sce_free0 == 3)
            pEm->Routine_3 = 1;

        break;

    case 1:

        pEm->Routine_3 = 2;
        *(unsigned long *)&pEm->Move_no = 0xF0005;
        EF(pEm)->Core_rno0 = 1;
        EF(pEm)->Core_rno1 = 1;
        pEm->Timer0 = 31;
        pEm->Timer2 = 0;

    case 2:

        if (++pEm->Timer2 & 1) {
            Set_sabun(pEm, 10, 0, 0xFFF);
            Set_sabun(pEm, 17, 0, 0xFFF);
        } else {

            Set_sabun(pEm, 10, 0, 0xF00);
            Set_sabun(pEm, 17, 0, 0xF00);
        }
        Joint_move(pEm, Kan, Seq, 0x100);
        pEm->Move_cnt = 0;
        if (--pEm->Timer0 == 0)
            pEm->Routine_3 = 3;

        break;

    case 3:

        if (++pEm->Timer2 & 1) {
            Set_sabun(pEm, 10, 0, 0xFFF);
            Set_sabun(pEm, 17, 0, 0xFFF);
        } else {

            Set_sabun(pEm, 10, 0, 0xF00);
            Set_sabun(pEm, 17, 0, 0xF00);
        }
        Joint_move(pEm, Kan, Seq, 0x200);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_3 = 4;
            pEm->Timer0 = 0xFFF;
        }
        break;

    case 4:

        chr_type = Mv_tojo20_0(pEm, Kan, Seq, 0x180);
        Set_sabun(pEm, 17, 0, pEm->Timer0);
        if (++pEm->Timer2 & 1) {
            Set_sabun(pEm, 10, 0, 0xFFF);
        } else {

            Set_sabun(pEm, 10, 0, 0xE00);
        }
        if (chr_type == 2) {
            pEm->Routine_3 = 5;
            pEm->Timer0 = 0xFFF;
        }
        break;

    case 5:

        chr_type = Mv_tojo20_0(pEm, Kan, Seq, 0x100);
        Set_sabun(pEm, 10, 0, pEm->Timer0);
        if (chr_type == 3) {
            pEm->Routine_3 = 6;
            pEm->Timer0 = 0xFFF;
            pEm->Timer1 = 0xFFF;
        }
        break;

    case 6:

        if (++pEm->Timer2 & 1) {
            Set_sabun(pEm, 9, 0, 0xFFF);
            Set_sabun(pEm, 6, 0, 0xFFF);
            Set_sabun(pEm, 2, 0, 0xFFF);
            Set_sabun(pEm, 3, 0, 0xFFF);
            Set_sabun(pEm, 18, 0, 0xFFF);
            Set_sabun(pEm, 19, 0, 0xFFF);
        } else {

            Set_sabun(pEm, 9, 0, 0xD00);
            Set_sabun(pEm, 6, 0, 0xD00);
            Set_sabun(pEm, 2, 0, 0xD00);
            Set_sabun(pEm, 3, 0, 0xD00);
            Set_sabun(pEm, 18, 0, 0xD00);
            Set_sabun(pEm, 19, 0, 0xD00);
        }

        pEm->Timer1 -= 0x100;
        if (pEm->Timer1 < 0) {
            pEm->Routine_3 = 7;
            pEm->Timer1 = 0xFFF;
            pEm->Timer0 = 0xFFF;
            pEm->Timer3 = 4;
            Set_sabun(pEm, 11, 0, 0);
            break;
        }
        Set_sabun(pEm, 11, 0, pEm->Timer1);

        break;

    case 7:

        pEm->Size += (++pEm->Timer2 & 1) ? 30 : -30;

        pEm->Timer1 -= 0x400;
        if (pEm->Timer1 < 0)
            pEm->Timer1 = 0;

        Set_sabun(pEm, 2, 0, pEm->Timer1);
        Set_sabun(pEm, 3, 0, pEm->Timer1);
        Set_sabun(pEm, 18, 0, pEm->Timer1);
        Set_sabun(pEm, 19, 0, pEm->Timer1);

        Mv_tojo20_1(pEm, Kan, Seq);
        Joint_move(pEm, Kan, Seq, 0x100);
        if (--*(signed char *)&pEm->Timer3 == 0)
            pEm->Routine_3 = 8;

        break;

    case 8:

        pEm->Size += (++pEm->Timer2 & 1) ? 30 : -30;
        Mv_tojo20_1(pEm, Kan, Seq);
        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x40000)
            pEm->Routine_3 = 9;

        break;

    case 9:

        pEm->Size += 500;
        if (pEm->Size >= 0x1180) {
            pEm->Size = 0x11FF;
            pEm->Routine_3 = 10;
        }
        Mv_tojo20_1(pEm, Kan, Seq);
        Joint_move(pEm, Kan, Seq, 0x100);
        break;

    case 10:

        pEm->Size -= 5;
        if (pEm->Size < 0xFFF)
            pEm->Size = 0xFFF;

        Mv_tojo20_1(pEm, Kan, Seq);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_3 = 11;
            EF(pEm)->Heart_stop = 0;
            *(unsigned long *)&pEm->Move_no = 0xF0006;
            Snd_se_enem(11, pEm);
        }
        break;

    case 11:

        for (i = 20; i < 23; i++) {
            pP_ptr = &pEm->pSin_parts_ptr[i];
            pP_ptr->Size_y += 0x100;
            if (pP_ptr->Size_y >= 0x1000)
                pP_ptr->Size_y = 0xFFF;
        }

        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_3 = 12;
            pEm->Timer0 = 30;
        }

        break;

    case 12:

        if (--pEm->Timer0 == 0) {
            pEm->Sce_free1 = 1;
            *(unsigned short *)&pEm->Routine_2 = 3;
        }
    }

    Em_seq_sound(pEm);
}

static void Em33_Mv_tojo30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register unsigned long i;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        return;
    }

    pEm->Type &= 0xFF;

    pEm->Be_flg &= ~0x800;
    pEm->Neck_flg &= 0xFE; /* retail-only */

    for (i = 0; i < 8; i++)
        Set_sabun(pEm, Mof_parts[i], 0, 0);

    for (i = 20; i < 23; i++) {
        pP_ptr = &pEm->pSin_parts_ptr[i];
        pP_ptr->Be_flg &= ~0x400;
    }

    EF(pEm)->Core_rno0 = 0;
    EF(pEm)->Core_rno1 = 1;

    pEm->Routine_1 = 1;
    *(unsigned short *)&pEm->Routine_2 = 0;

    pEm->Damage_cnt &= 0x7F;
}

static void Em33_Mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0000;
        pEm->Timer0 = 60;
        Snd_se_enem(10, pEm);
        pEm->Neck_flg &= 0xFE; /* retail-only @0x8010104C */
    }

    Joint_move(pEm, Kan, Seq, 0x100);

    if (--pEm->Timer0 == 0) {
        pEm->Routine_1 = 2;
        *(unsigned short *)&pEm->Routine_2 = 0;
    }
}

static void Em33_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PLAYER_WORK *pTg;
    register short rot;

    pTg = &G.Player;

    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = EF(pEm)->Status + 0xF0001;
        pEm->Spd_x = pEm->Spd_z = pEm->Spd_y = 0;
        EF(pEm)->Turn_on = 0;
        EF(pEm)->Stay = 0;
        pEm->Neck_flg &= 0xFE; /* retail-only @0x8010112C */

    case 1:

        Root_ck(pEm, &pTg->Pos_x, 0, 0);
        rot = 0x18; if (pEm->L_pl > 5000) rot = 0x20;
        if (pTg->Life >= 0) {
            register short Dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, rot); /* Dir: inferred name, the compiler deletes it, retail SYM has no record */
            pEm->Cdir_y += Dir;
        }
        Spd_get(pEm, Kan, Seq, 0);
        if (EF(pEm)->Stay > 20)
            pEm->Routine_2 = 2;

        break;

    case 2:

        pEm->Routine_2 = 3;
        EF(pEm)->Em_keep = esc_rot_tbl[Guide_check(pEm->Pos_x, pEm->Pos_z)];
        pEm->Timer0 = 45;

    case 3:
        Spd_get(pEm, Kan, Seq, 0);
        if (Die_move_rot(pEm, 16) || --pEm->Timer0 == 0) {
            pEm->Routine_2 = 1;
            EF(pEm)->Stay = 0;
        }

    }

    Joint_move(pEm, Kan, Seq, 0x100);
    Add_speedXZ(pEm, 0);
    Em_seq_sound(pEm);

    if (Mv_atck_ck(pEm) == -1) {
        Snd_se_enem(10, pEm);
    }
    Mv_turn_ck(pEm);
    Mv_jump_ck(pEm);
}

static long Mv_atck_ck(ENEMY_WORK *pEm)
{
    register PLAYER_WORK *pTg;
    register EM33_FREE *pFp;
    register short dir, range, cnt;

    pTg = &G.Player;
    pFp = EF(pEm);

    dir = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x300);

    if (pTg->Life < 0) return 0;

    if (pTg->Life < 20 && Cdir_ck(pEm, pTg) == 0) {
        if (pFp->No_atck[0] == 0 && dir == 0 && pEm->L_pl < 3500) {
            pEm->Routine_1 = 6;
            *(unsigned short *)&pEm->Routine_2 = 0;
            return 1;
        }
        return 0;
    }

    if (pFp->Status == 0) {
        if (pFp->No_atck[0] && pFp->No_atck[1]) {
            pFp->No_atck[Rnd() & 1] = 0;
            return -1;
        }

        if (Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 1) >= 0) {

            range = 4000; cnt = pFp->No_atck[0];
        } else {

            range = 3000;
            cnt = pFp->No_atck[1];
        }

        if (cnt == 0 && dir == 0 && pEm->L_pl < range) {
            pEm->Routine_1 = 4;
            *(unsigned short *)&pEm->Routine_2 = 0;
            return 1;
        }
    } else {

        if (pFp->No_atck[0]) return 0; /* retail @0x80101438 (Beta -1) */

        if (dir == 0 && pEm->L_pl < 3000) {
            pEm->Routine_1 = 5;
            *(unsigned short *)&pEm->Routine_2 = 0;
            return 1;
        }
    }

    return 0;
}

static void Em33_Mv_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PLAYER_WORK *pTg;
    register EM33_FREE *pFp;
    register short dir;
    register short d2; /* d2: inferred name, the compiler deletes it, retail SYM has no record */
    pTg = &G.Player;
    pFp = EF(pEm);

    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = pFp->Status + 0xF0001;
        pFp->Em_keep = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
        dir = -1; if (pFp->Em_keep > 0) dir = 1;
        if ((signed char)pFp->Turn_keep == dir) {
            if (++pFp->Turn_rev >= 3) {
                pFp->Em_keep = -pFp->Em_keep;
                pFp->Turn_rev = 0;
            }
        }
        pFp->Turn_keep = dir;
        pEm->Timer0 = 32;
        pFp->Turn_on = 0;
        pEm->Neck_flg &= 0xFE; /* retail-only @0x80101574 */
    }

    pEm->Cdir_y += pFp->Em_keep;
    d2 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
    if (--pEm->Timer0 == 0 || d2 == 0 || EF(pEm)->Em_keep == 0) {
        pEm->Routine_1 = 2;
        *(unsigned short *)&pEm->Routine_2 = 0;
        Mv_atck_ck(pEm);
        Mv_jump_ck(pEm);
    }

    Joint_move(pEm, Kan, Seq, 0x100);
    Em_seq_sound(pEm);
    Joint_move(pEm, Kan, Seq, 0x100);
    Em_seq_sound(pEm);

}

static void Mv_turn_ck(ENEMY_WORK *pEm)
{
    if (pEm->L_pl < 4500 && G.Player.Life >= 0) {
        if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400)) {
            if (((0x1558 >> (Rnd() & 15)) & 1) || EF(pEm)->Turn_on) {
                pEm->Routine_1 = 3;
                *(unsigned short *)&pEm->Routine_2 = 0;
            }
        }
    }
}

static void Em33_Mv_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long d0;

    Mv_atck_tbl[pEm->Routine_2](pEm, Kan, Seq);

    d0 = touch_tbl[EF(pEm)->Em_keep * 2] + 1;
    if (*(signed char *)&pEm->Timer3) {
        Set_blood(pEm, d0, 1);
        Set_blood(pEm, d0 + 2, 1);
        pEm->Timer3--;
    }

}

static void Em33_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register EM33_FREE *pFp;

    pFp = EF(pEm);

    pEm->Routine_2 = 1;
    if (pEm->Routine_3 == 0) {
        if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1) >= 0)

            EF(pEm)->Em_keep = 0;
        else
            EF(pEm)->Em_keep = 1;

        pEm->Timer1 = (1 << (Rnd() & 15)) & 0x5A0;
    }
    *(unsigned long *)&pEm->Move_no = pFp->Em_keep + 0xF0007;
    Spd_get(pEm, Kan, Seq, 0);
    Joint_move(pEm, Kan, Seq, 0x100);

    pEm->Timer2 = 0;
    pEm->Timer3 = 0;
    pEm->Neck_flg |= 1; /* retail-only @0x80101864 */

    pFp->Turn_rev = 0;
}

static void Em33_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register EM33_FREE *pFp;
    register PLAYER_WORK *pTg;
    register unsigned long chr_type, d0;

    pTg = &G.Player;
    pFp = EF(pEm);

    Spd_get(pEm, Kan, Seq, 0);
    if (Joint_move(pEm, Kan, Seq, 0x100)) {
        chr_type = *pEm->pNow_seq;
        pEm->Routine_1 = 2;
        *(unsigned short *)&pEm->Routine_2 = 0;
        if (EF(pEm)->Turn_on == 0) {
            pFp->No_atck[EF(pEm)->Em_keep] = (signed char)pEm->Timer2 + 10 + (Rnd() & 7);
        }
        Mv_turn_ck(pEm);
        Mv_kill_ck(pEm);
        EF(pEm)->Turn_on = 0;
        EF(pEm)->No_jump = (pEm->Timer2 << 2) + 60;
    } else {

        chr_type = *pEm->pNow_seq;

        if (pEm->Routine_3 == 0 && pEm->Timer1 != 0 && (chr_type & 0x20000)) {
            if (Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x440) && !((0x5A0 >> (Rnd() & 15)) & 1)) {

                pEm->Routine_3 = 1;
                EF(pEm)->Turn_on = 1;
            } else {

                Goto00(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
                *(unsigned short *)&pEm->Routine_2 = 0x100;
                pFp->Em_keep ^= 1;
            }
        }
    }

    Add_speedXZ(pEm, 0);
    Em_seq_sound(pEm);

    if ((chr_type & 0x10000) && *(signed char *)&pEm->Timer2 == 0)
        if (Arm_atck_ck(pEm, pFp->Em_keep)) {
        Snd_se_enem(3, pEm);
        pEm->Timer2 = 15;
        pEm->Timer3 = 3;
        Set_blood((ENEMY_WORK *)&G.Player, 8, 0);
    }

    d0 = touch_tbl[pFp->Em_keep * 2] + 1;
    Arm_spark_ck(pEm, d0);

}

static long Arm_spark_ck(ENEMY_WORK *pEm, unsigned char no)
{
    register PARTS_W *pP_ptr;
    register short x, z; /* x, z: inferred names, the compiler deletes them, retail SYM has no record */

    if (*pEm->pNow_seq & 0x1000000) {
        if (Ck_schit(pEm, no, 100)) {
            pP_ptr = &pEm->pSin_parts_ptr[no];
            x = pP_ptr->Workm.t[0];
            z = pP_ptr->Workm.t[2];

            if (x > -3700 && x < 15400)
                if (z > -22000 && z < -11000) {
                    Snd_se_enem(5, pEm);
                    Set_blood(pEm, no - 1, 6);
                    Set_blood(pEm, no, 6);
                    Set_blood(pEm, no + 1, 6);
                    return 1;
                }
        }
    }

    return 0;
}

static void Em33_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register EM33_FREE *pFp;
    register unsigned long d0;

    pFp = EF(pEm);

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        d0 = EF(pEm)->Em_keep;
        if (d0 == 0) {
            *(unsigned long *)&pEm->Move_no = 0xF2E07;
        } else {

            *(unsigned long *)&pEm->Move_no = 0xF2608;
        }
        d0 = touch_tbl[d0 * 2] + 1;
        Set_blood(pEm, d0, 6);
        Set_blood(pEm, d0 + 1, 6);
        Set_blood(pEm, d0 + 2, 6);
    }

    if (Joint_move(pEm, Kan, Seq, 0x100)) {
        pEm->Routine_1 = 2;
        *(unsigned short *)&pEm->Routine_2 = 0;
        pFp->No_atck[pFp->Em_keep] = 30;
        Mv_turn_ck(pEm);
        pFp->Turn_on = 0;
    }
}

static void Mv_kill_ck(ENEMY_WORK *pEm)
{
    if (G.Player.Life < 0) {
        pEm->Routine_1 = 1;
        *(unsigned short *)&pEm->Routine_2 = 0;
        pEm->Be_flg |= 0x4A;
    } }

static void Em33_Mv_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Mv_combo_tbl[pEm->Routine_2](pEm, Kan, Seq); }

static void Em33_Mv_combo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0009;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        EF(pEm)->Em_keep = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
        EF(pEm)->Turn_rev = 0;
        pEm->Neck_flg |= 1; /* retail-only @0x80101DEC */
        break;

    case 1:

        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Add_speedXZ(pEm, 0);
        pEm->Cdir_y += EF(pEm)->Em_keep;

        if (*pEm->pNow_seq & 0x70000)
            if (Combo_atck_ck(pEm, 3)) {
                Snd_se_enem(13, pEm);
                *(unsigned short *)&pEm->Routine_2 = 1;
                break;
            }

        if (*pEm->pNow_seq & 0x80000) {
            pEm->Routine_3 = 2;
            *(unsigned long *)&pEm->Move_no = 0xF000C;
            Spd_get(pEm, Kan, Seq, 0);
            Joint_move(pEm, Kan, Seq, 0x100);
        }
        break;

    case 2:

        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_1 = 2;
            *(unsigned short *)&pEm->Routine_2 = 0;
            EF(pEm)->Status = 0;
            EF(pEm)->No_atck[0] = 30;
            Snd_se_enem(10, pEm);
        }
        Add_speedXZ(pEm, 0);

    }

    Em_seq_sound(pEm);
}

static void Em33_Mv_combo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long d0;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(signed char *)&pEm->Timer2 = 4;
        pEm->Timer3 = 4;
    }

    Spd_get(pEm, Kan, Seq, 0);
    if (Joint_move(pEm, Kan, Seq, 0x100))
        *(unsigned short *)&pEm->Routine_2 = 2;

    Add_speedXZ(pEm, 0);
    Em_seq_sound(pEm);

    if (d0 = (*pEm->pNow_seq >> 16) & 7)
        if ((signed char)pEm->Timer2 != d0)
            if (Combo_atck_ck(pEm, d0 - 1)) {
                Snd_se_enem(d0 < 3 ? 3 : 13, pEm);
                *(signed char *)&pEm->Timer2 = d0;
                pEm->Timer3 = 4;
            }

    if (*(signed char *)&pEm->Timer3) {
        d0 = touch_tbl[((signed char)pEm->Timer2 - 1) << 1];
        if ((signed char)pEm->Timer2 < 3) {
            Set_blood(pEm, d0 + 1, 1);
            Set_blood(pEm, d0 + 3, 1);
        } else {

            Set_blood(pEm, d0, 1);
        }
        pEm->Timer3--;
    }

    Arm_spark_ck(pEm, touch_tbl[(signed char)pEm->Timer2 * 2] + 1);

}

static void Em33_Mv_combo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0006;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Snd_se_enem(11, pEm);
        break;

    case 1:
        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_3 = 2;
            pEm->Timer0 = 31;
            *(unsigned long *)&pEm->Move_no = 0x3F0000;
            Joint_move(pEm, Kan, Seq, 0x40);
        }
        Add_speedXZ(pEm, 0);
        break;

    case 2:
        Joint_move(pEm, Kan, Seq, 0x40);
        if (--pEm->Timer0 == 0) {
            pEm->Routine_1 = 2;
            *(unsigned short *)&pEm->Routine_2 = 0;
            EF(pEm)->Status = 0;
            EF(pEm)->No_atck[0] = 60;
            Mv_kill_ck(pEm);
        }

    }
}

static void Em33_Mv_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Mv_critical_tbl[pEm->Routine_2](pEm, Kan, Seq); }

static void Em33_Mv_critical00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PLAYER_WORK *pTg;

    pTg = &G.Player;

    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0006;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Add_speedXZ(pEm, 0);
        pEm->Neck_flg |= 1; /* retail-only @0x80102348 */
        break;

    case 1:

        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_3 = 2;
            EF(pEm)->Em_keep = 0;
            if (Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40))
                EF(pEm)->Em_keep = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x10); }

        break;

    case 2:

        pEm->Routine_3 = 3;
        *(unsigned long *)&pEm->Move_no = 0xF000B;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        break;

    case 3:

        pEm->Cdir_y += EF(pEm)->Em_keep;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x10000)
            if (Arm_touch_ck(pEm, 4)) {
                if (pTg->Life < 20 && Cdir_ck(pEm, pTg) == 0) {
                    *(unsigned short *)&pEm->Routine_2 = 1;
                    Snd_se_enem(4, pEm);
                    break;
                } else {

                    if (Pl_life_down(10, 0) != 2) { /* retail 10 (Beta 5) */
                        Em33_Pl_to_dmg(Cdir_ck(pEm, &G.Player));
                    } else {

                        Em33_Pl_to_die();
                    }
                    Snd_se_enem(13, pEm);
                    Set_blood(pEm, 6, 1);
                }
            }

        if (*pEm->pNow_seq & 0x20000)
            pEm->Routine_3 = 4;

        break;

    case 4:

        pEm->Routine_3 = 5;
        *(unsigned long *)&pEm->Move_no = 0xF000C;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        break;

    case 5:

        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_1 = 2;
            *(unsigned short *)&pEm->Routine_2 = 0;
            EF(pEm)->Status = 0;
            EF(pEm)->No_atck[0] = 60;
            Snd_se_enem(10, pEm);
            Mv_kill_ck(pEm);
        }
        Add_speedXZ(pEm, 0);
    }

}

static void Em33_Mv_critical10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register PLAYER_WORK *pTg;
    register unsigned long chr_type, i;

    if (pEm->Routine_3 == 0) {

        pEm->Routine_3 = 1;
        pTg = &G.Player;

        pEm->Be_flg |= 2;
        pEm->Damage_cnt |= 0x80; /* retail-only @0x80102644 */

        pTg->pEnemy_ptr = (unsigned long *)pEm;
        *(unsigned long *)&pTg->Routine_0 = 6;
        pTg->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pTg->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pTg->Damage_cnt |= 0x80;
        Set_base_pos(pTg, Kan, Seq, 0);
        pTg->Cdir_y = pEm->Cdir_y + 0x800;
        pTg->Be_flg |= 0x4A;

        EF(pEm)->Core_rno0 = 1;
        EF(pEm)->Core_rno1 = 1;

        pP_ptr = &pEm->pSin_parts_ptr[6];
        pP_ptr->Poly_rgb = 0xF0F3F;

        G.System_flg |= 0x100;
        G.System_flg |= 0x4000000;
    }

    Spd_get(pEm, Kan, Seq, 0);
    if (Joint_move(pEm, Kan, Seq, 0x100))
        *(unsigned short *)&pEm->Routine_2 = 2;

    Add_speedXZ(pEm, 0);
    Em_seq_sound(pEm);

    chr_type = *pEm->pNow_seq;
    if (chr_type & 0x100000) {
        if ((0x5FFA >> (Rnd() & 15)) & 1)
            Set_blood(pEm, 6, Rnd() & 1);

        if ((0x17E4 >> (Rnd() & 15)) & 1)
            Set_blood(pEm, 6, 9);

    }

    if (chr_type & 0x400000) {
        for (i = 14; i < 17; i++) {
            pP_ptr = &pEm->pSin_parts_ptr[i];
            pP_ptr->Poly_rgb = 0xF0F3F;
        }
        for (i = 20; i < 23; i++) {
            pP_ptr = &pEm->pSin_parts_ptr[i];
            pP_ptr->Poly_rgb = 0xF0F3F;
        }
    }

    if (chr_type & 0x200000) {
        Set_blood(pEm, 15, 1);
        Set_blood(pEm, 21, 1);
        if ((0x17E4 >> (Rnd() & 15)) & 1) {
            Set_blood(pEm, 14, 0);
            Set_blood(pEm, 16, 0);
            Set_blood(pEm, 20, 0);
            Set_blood(pEm, 22, 0);
        }
    }
}

static void Em33_Mv_critical20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0006;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Snd_se_enem(10, pEm);
        break;

    case 1:
        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_3 = 2;
            *(unsigned long *)&pEm->Move_no = 0xFF0000;
        }
        Add_speedXZ(pEm, 0);
        break;

    case 2:
        Joint_move(pEm, Kan, Seq, 15);

    }

}

static void Em33_Mv_jump(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Mv_jump_tbl[pEm->Routine_2](pEm, Kan, Seq); }

static void Em33_Mv_jump00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF000D;
        pEm->Spd_x = SquareRoot0((5000 - pEm->Pos_x) * (5000 - pEm->Pos_x) + (-16600 - pEm->Pos_z) * (-16600 - pEm->Pos_z)) / 17;

        pEm->Spd_y = -1354;
        pEm->Spd_z = 0;

        EF(pEm)->Em_keep = Direction_ck(pEm->Pos_x, pEm->Pos_z, 5000, -16600);
        EF(pEm)->Turn_rev = 0;
        pEm->Neck_flg |= 1; /* retail-only @0x80102B14 */

    case 1:

        Die_move_rot(pEm, 0x80);
        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_3 = 2;
            pEm->Timer3 = 0;
            pEm->Be_flg |= 8;
            EF(pEm)->Jump_flg = 1;
            pEm->Damage_cnt |= 0x80;
            Snd_se_enem(6, pEm);
            Pad_vib_set1(8, 0x64, 0); /* retail-only vibration (shared jal @0x80102C64) */
        }
        break;

    case 2:

        Add_speedXYZ(pEm, 0, 0);
        if ((signed char)pEm->Timer3 == 0)
            if (Joint_move(pEm, Kan, Seq, 0x100)) {
                pEm->Timer3 = 1;
                pEm->Be_flg |= 0x400;
            }

        pEm->Spd_y += 80;
        if (pEm->Spd_y >= 0 && pEm->Pos_y >= -6000) {
            *(unsigned short *)&pEm->Routine_2 = 1;
            pEm->Pos_x = 5000;
            pEm->Pos_y = -6000;
            pEm->Pos_z = -16600;
            Snd_se_enem(7, pEm);
            EF(pEm)->Yure_type = 28;
            EF(pEm)->Yure_time = 7;
            Pad_vib_set1(8, 0x64, 0); /* retail-only vibration */
        }

    }

    pEm->nFloor = pEm->Pos_y / 1800;

}

static void Em33_Mv_jump10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0001;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        pEm->Timer0 = SquareRoot0((5000 - G.Player.Pos_x) * (5000 - G.Player.Pos_x) + (-16600 - G.Player.Pos_z) * (-16600 - G.Player.Pos_z)) >> 4;

        pEm->Type |= 0x4400;
    }

    Spd_get(pEm, Kan, Seq, 0);
    Joint_move(pEm, Kan, Seq, 0x100);
    pEm->Timer0 -= abs(pEm->Spd_x);
    if (pEm->Timer0 < 0)
        *(unsigned short *)&pEm->Routine_2 = 2;

}

static void Em33_Mv_jump20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PLAYER_WORK *pTg;
    register unsigned long area;
    register short *pB; /* inferred name: the compiler deletes it, retail SYM has no record */

    pTg = &G.Player;

    switch (pEm->Routine_3) {

    case 0:
        pEm->Routine_3 = 1;

        *(unsigned long *)&pEm->Move_no = 0xF000D;
        area = down_dest_tbl[Guide_check(pTg->Pos_x, pTg->Pos_z)];
        pB = G.pRoom->pBlock + area * 6 + 2; /* + 2 folded into the four loads: GCC deletes pB, as retail */
        pEm->Dest_x = (pB[0] + pB[2]) >> 1;
        pEm->Dest_z = (pB[1] + pB[3]) >> 1;

        pEm->Spd_x = SquareRoot0((pEm->Dest_x - pEm->Pos_x) * (pEm->Dest_x - pEm->Pos_x) + (pEm->Dest_z - pEm->Pos_z) * (pEm->Dest_z - pEm->Pos_z)) / 22;

        pEm->Spd_y = -852;
        pEm->Spd_z = 0;
        EF(pEm)->Em_keep = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);

        pEm->Cdir_y = EF(pEm)->Em_keep;
        pEm->Be_flg |= 4;

    case 1:

        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_3 = 2;
            pEm->Timer3 = 0;
            Snd_se_enem(6, pEm);
        }
        break;

    case 2:

        Add_speedXYZ(pEm, EF(pEm)->Em_keep - pEm->Cdir_y, 0);
        if ((signed char)pEm->Timer3 == 0)
            if (Joint_move(pEm, Kan, Seq, 0x100))
                pEm->Timer3 = 1;

        pEm->nFloor = pEm->Pos_y / 1800;

        pEm->Spd_y += 80;
        if (pEm->Spd_y >= 852 && pEm->Spd_x != 0) {
            pEm->Be_flg &= ~0x40C;
            pEm->Spd_x = 0;
            pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);

            pEm->Type &= 0xBBFF;
        }

        if (pEm->Pos_y + 200 >= pEm->Ground) {
            pEm->Routine_3 = 3;

            *(unsigned long *)&pEm->Move_no = 0x3000E;
            pEm->Timer2 = 1;
            EF(pEm)->Jump_flg = 0;

            Joint_move(pEm, Kan, Seq, 0x400);
            pEm->Pos_x = pEm->Dest_x;
            pEm->Pos_y = pEm->Ground;
            pEm->Pos_z = pEm->Dest_z;
            pEm->nFloor = 0;
            pEm->Timer3 = 0;
            pEm->Damage_cnt &= 0x7F;
            pEm->Type &= 0xBBFF;
            Pad_vib_set1(8, 0x8C, 0); /* retail-only vibration @0x801030E8 */
        }

        break;

    case 3:

        if (Joint_move(pEm, Kan, Seq, 0x400)) {

            EF(pEm)->Jump_flg = 0;

            pEm->Routine_1 = 2;
            *(unsigned short *)&pEm->Routine_2 = 0;

            EF(pEm)->No_jump = 60;
            if ((0x57EA >> (Rnd() & 15)) & 1)
                EF(pEm)->No_jump += 60;

            Mv_kill_ck(pEm);
        }
        Em_seq_sound(pEm);

        if ((*pEm->pNow_seq & 0x10000) && *(signed char *)&pEm->Timer2 == 0)
            if (Arm_atck_ck(pEm, 0)) {
                pEm->Timer2 = 1;
                pEm->Timer3 = 3;
                Snd_se_enem(3, pEm);
                Set_blood((ENEMY_WORK *)&G.Player, 8, 0);
            }

        if (*pEm->pNow_seq & 0x8000000) {
            EF(pEm)->Yure_type = 29;
            EF(pEm)->Yure_time = 7;
        }

        Arm_spark_ck(pEm, 14);

        if (*(signed char *)&pEm->Timer3) {
            Set_blood(pEm, 14, 1);
            Set_blood(pEm, 16, 1);
            pEm->Timer3--;
        }

    }

}

static long jump_ck_main(ENEMY_WORK *pEm, long Type)
{
    register unsigned long Pa, Ea;
    register char *pTbl;

    Pa = Guide_check(G.Player.Pos_x, G.Player.Pos_z);
    Ea = Guide_check(pEm->Pos_x, pEm->Pos_z);

    if (Type == 0)
        pTbl = jump_dest_tbl;
    else

        pTbl = &jump_dest_tbl[121];

    return (signed char)*(pTbl + Ea * 11 + Pa); /* retail 11x11 @0x801032AC */
}

static void Mv_jump_ck(ENEMY_WORK *pEm)
{

    register unsigned long lim; /* [INFERRED] carrier: retail difficulty-dependent range @0x801032FC */

    if (G.Player.Life < 0) return;

    lim = (G.System_flg & 0x20) ? 375 : 450;

    if (EF(pEm)->No_jump == 0)
        if (jump_ck_main(pEm, (unsigned long)(pEm->Life + 0) < lim) == 0) { /* "+ 0": inferred spelling; GCC then compares the sign-extended Life unsigned (lh + sltiu), as retail does */
            if ((0x1A0 >> (Rnd() & 15)) & 1) {
                pEm->Routine_1 = 7;
                *(unsigned short *)&pEm->Routine_2 = 0;
            }
        }

}

static void Em33_Mv_yoro(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0004;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Snd_se_enem(10, pEm);
        break;

    case 1:

        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Damage_cnt &= 0x7F;
            if (jump_ck_main(pEm, 1) == 0) {
                EF(pEm)->No_yoro = 0;
                *(unsigned long *)&pEm->Routine_0 = 0x701;
            } else {
                *(unsigned short *)&pEm->Routine_2 = 2;
            }
        }

        Add_speedXZ(pEm, 0);
        break;

    case 2:
        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            *(unsigned long *)&pEm->Routine_0 = 0x201;
            EF(pEm)->No_yoro = 0;
        }
        Add_speedXZ(pEm, 0);

    }
}

static void Em33_Mv_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;

        EF(pEm)->Status = 2;
        *(unsigned long *)&pEm->Move_no = 0xF0006;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Snd_se_enem(11, pEm);
    } else {

        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Damage_cnt &= 0x7F;
            *(unsigned long *)&pEm->Routine_0 = 0x201;
            EF(pEm)->No_yoro = 0;
        }
        Add_speedXZ(pEm, 0);
    }
}

static void Em33_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_2 == 0) {
        Em33_dmg_nml00(pEm, Kan, Seq);
    } else {

        Em33_dmg_nml10(pEm, Kan, Seq);
    }
    Em33_Yure_ck(pEm); /* retail @0x80103610 */
}

static void Em33_dmg_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register EM33_FREE *pFp;
    register long lim; /* [INFERRED] carrier for the retail difficulty threshold */

    pFp = EF(pEm);

    em33_dmg_init[pEm->Routine_1 - 1](pEm, Kan, Seq);

    EF(pEm)->Core_rno0 = 2;
    EF(pEm)->Core_rno1 = 0;

    if (pEm->Routine_1 != 16) {
        Snd_se_enem(2, pEm);
    }

    if (pEm->Routine_1 != 1) {
        /* retail @0x801036B0: two offset blood sprays replace Beta's single Set_blood */
        SVECTOR Ofs;
        register unsigned char parts, type; /* [INFERRED] carriers (retail s0/s1) */

        parts = blood_pos_tbl[pEm->Damage_no % 3];
        type = dieblood_type_tbl[pEm->Routine_1 - 1];
        Ofs.vx = 300;
        Ofs.vy = 0;
        Ofs.vz = 0;
        Set_blood_ofs(pEm, parts, type, &Ofs);
        Ofs.vx = -100;
        Set_blood_ofs(pEm, parts, type, &Ofs);
    }

    if (G.Player.Life < 20 || pFp->Jump_flg || pFp->No_yoro) {
        *(unsigned long *)&pEm->Routine_0 = (pFp->Rno3_stk << 24) + ((pFp->Rno2_stk << 16) + (pFp->Rno1_stk << 8) + 1);
        Em33_Move(pEm, Kan, Seq);
        return;
    }

    lim = (G.System_flg & 0x20) ? 500 : 600; /* retail: easy 500 (Beta 600) @0x80103784 */
    if (pEm->Life >= lim) {
        EF(pEm)->Dmg_stk -= yoro_tbl00[pEm->Routine_1 - 1];
        if (EF(pEm)->Dmg_stk <= 0) {

            EF(pEm)->Turn_on = 0;
            EF(pEm)->Dmg_stk = 40;
            EF(pEm)->No_yoro = 1;
            *(unsigned long *)&pEm->Routine_0 = 0x801;
            pEm->Spd_x = EF(pEm)->Spdx_stk;
            pEm->Spd_y = EF(pEm)->Spdy_stk;
            pEm->Spd_z = EF(pEm)->Spdz_stk;
            EF(pEm)->Rno1_stk = pEm->Routine_1;
            EF(pEm)->Rno2_stk = pEm->Routine_2;
            EF(pEm)->Rno3_stk = pEm->Routine_3;
            return;
        } } else {
        EF(pEm)->Dmg_stk -= yoro_tbl01[pEm->Routine_1 - 1];
        if (EF(pEm)->Dmg_stk <= 0) {

            EF(pEm)->Turn_on = 0;
            EF(pEm)->Dmg_stk = 40;

            if (EF(pEm)->Status == 0) {
                EF(pEm)->No_yoro = 1;
                *(unsigned long *)&pEm->Routine_0 = 0x901;
                pEm->Spd_x = EF(pEm)->Spdx_stk;
                pEm->Spd_y = EF(pEm)->Spdy_stk;
                pEm->Spd_z = EF(pEm)->Spdz_stk;
                EF(pEm)->Rno1_stk = pEm->Routine_1;
                EF(pEm)->Rno2_stk = pEm->Routine_2;
                EF(pEm)->Rno3_stk = pEm->Routine_3;
                return;
    } } }
    *(unsigned long *)&pEm->Routine_0 = (EF(pEm)->Rno3_stk << 24) + ((EF(pEm)->Rno2_stk << 16) + (EF(pEm)->Rno1_stk << 8) + 1);

    Em33_Move(pEm, Kan, Seq);

}

static void Em33_dmg_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

    Spd_get(pEm, Kan, Seq, 0);
    if (Joint_move(pEm, Kan, Seq, 0x100)) {
        pEm->Damage_cnt &= 0x7F;
        *(unsigned long *)&pEm->Routine_0 = 0x201;
        EF(pEm)->Rno1_stk = pEm->Routine_1;
        EF(pEm)->Rno2_stk = pEm->Routine_2;
        EF(pEm)->Rno3_stk = pEm->Routine_3;
    }
    Add_speedXZ(pEm, 0);

}

static void Em33_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em33_dmg_mag(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    EF(pEm)->Mg_r0 = 1; }

static void Em33_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Set_blood(pEm, blood_rnd[((pEm->Damage_no % 3) << 2) + (Rnd() & 3)], dieblood_type_tbl[pEm->Routine_1 - 1]); /* retail: Beta shifted by (Rnd&3)+2 */

}

static void Em33_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long i, max, add;
    register unsigned long rno; /* [INFERRED] carrier: retail keeps Routine_1 in a1 for both tests */

    rno = pEm->Routine_1;
    if (rno == 16) {
        if (EF(pEm)->No_fire) return;

        EF(pEm)->No_fire = 30;
    }

    add = (pEm->Damage_no % 3) ? 4 : 0;

    max = (rno == 10 || rno == 16) ? 6 : 3; /* retail @0x80103A88 (Beta == 9 ? 3 : 6) */
    for (i = 0; i < max; i++) {
        Set_blood(pEm, blood_rnd[(add + Rnd()) & 7], 2);
    }
}

static void Em33_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long i, add;
    add = (pEm->Damage_no % 3) ? 4 : 0;

    if (pEm->Routine_0 == 2) {
        for (i = 0; i < 3; i++) {
            Set_blood(pEm, blood_rnd[add + (Rnd() & 7)], 4);
        }
    } else {

        for (i = 0; i < 4; i++) {
            Set_blood(pEm, blood_rnd[add + (Rnd() & 7)], 5);
        }
    }
}

static void Em33_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_0 == 2) {
        EF(pEm)->Biri_cnt = 15;
        Set_blood(pEm, blood_rnd[(Rnd() & 3) + 8], 4);
        Set_blood(pEm, blood_rnd[(Rnd() & 3) + 8], 4);
    } else {

        EF(pEm)->Biri_cnt = 45;
        Set_blood(pEm, blood_rnd[(Rnd() & 3) + 8], 4);
        Set_blood(pEm, blood_rnd[(Rnd() & 3) + 8], 4);
        Set_blood(pEm, blood_rnd[(Rnd() & 3) + 8], 8);
    }
}

static void Em33_magdmg(ENEMY_WORK *pEm)
{
    SVECTOR Sv0, Sv1, Sv2;
    MATRIX M;
    short Dir; /* inferred name: copy of the stored value, propagated; retail SYM has no record (as EM31.C) */
    PARTS_W *pP_ptr; /* inferred name: folded into the address; retail SYM has no record (as EM31.C) */

    switch (EF(pEm)->Mg_r0) {
    case 0:
        break;

    case 1:
        EF(pEm)->Mg_r0 = 2;
        EF(pEm)->Mg_t0 = 0;
        EF(pEm)->Mg_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

    case 2:

        Sv0.vx = 0;
        Dir = Sv0.vy = EF(pEm)->Mg_dir;
        Sv0.vz = 0;
        RotMatrix(&Sv0, &M);

        Sv0.vx = -(EF(pEm)->Mg_t0 * 100);
        Sv0.vy = 200;
        ApplyMatrixSV(&M, &Sv0, &Sv1);

        Sv0.vx = -100;
        Sv0.vy = -500;
        Sv0.vz = 0;

        if (!(pEm->Damage_no % 3))
            Sv0.vy = 500;

        ApplyMatrixSV(&M, &Sv0, &Sv2);

        pP_ptr = pEm->pSin_parts_ptr + 9;
        Sv0.vx = Sv1.vx + pEm->Pos_x;
        Sv0.vy = Sv1.vy + pP_ptr->Workm.t[1];
        Sv0.vz = Sv1.vz + pEm->Pos_z;
        Esp_call((0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx + 0x80 - Rnd();
        Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz + 0x80 - Rnd();
        Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

        if (EF(pEm)->Mg_t0 == 1) {
            Esp_call(0x217D0, Dir + 0x800, &GsIDMATRIX, &Sv0);
            Esp_call(0x20C18, Dir + 0x800, &GsIDMATRIX, &Sv0);
        }

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 1);
        Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 1);
        Esp_call((0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 3 + 0x80 - Rnd();
        Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 3 + 0x80 - Rnd();
        Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + (Sv2.vx << 2);
        Sv0.vz = Sv1.vz + pEm->Pos_z + (Sv2.vz << 2);
        Esp_call((0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 5 + 0x80 - Rnd();
        Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 5 + 0x80 - Rnd();
        Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 6;
        Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 6;
        Esp_call((0x65C - EF(pEm)->Mg_t0 * 36 - Rnd()) | 0x10000, Dir, &GsIDMATRIX, &Sv0);

        EF(pEm)->Mg_t0++;
        Sv0.vx = Sv1.vx + pEm->Pos_x + Sv2.vx * 7 + 0x80 - Rnd();
        Sv0.vz = Sv1.vz + pEm->Pos_z + Sv2.vz * 7 + 0x80 - Rnd();
        Esp_call(((1 - (Rnd() & 1)) << 16) | (0x65C - EF(pEm)->Mg_t0 * 30 - Rnd()), Dir, &GsIDMATRIX, &Sv0);

        if (EF(pEm)->Mg_t0++ >= 17)
            EF(pEm)->Mg_r0 = 0;

    }

}

static long Die_move_rot(ENEMY_WORK *pEm, short rot)
{
    register short d0;

    d0 = EF(pEm)->Em_keep - (pEm->Cdir_y & 0xFFF);
    if (abs(d0) >= rot) {
        if (abs(d0) >= 0x800)
            d0 = -d0;

        pEm->Cdir_y += (d0 > 0) ? rot : -rot;
        return 0;
    }
    return 1;
}

static void Em33_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register long d0;

    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0004;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        EF(pEm)->Em_keep = esc_rot_tbl[Guide_check(pEm->Pos_x, pEm->Pos_z)];
        pEm->Timer2 = (EF(pEm)->Em_keep & 0x3FF) != 0;
        pEm->Timer0 = 45;
        pEm->Timer3 = 2;

        em33_dmg_init[pEm->Routine_1 - 1](pEm, Kan, Seq);
        EF(pEm)->Jump_flg = 0;
        Snd_se_enem(10, pEm);
        pEm->Neck_flg |= 1; /* retail-only @0x801044AC */

        break;

    case 1:

        Spd_get(pEm, Kan, Seq, 0);
        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_2 = 2;
            *(unsigned long *)&pEm->Move_no = 0xF0002;
            Spd_get(pEm, Kan, Seq, 0);
            Joint_move(pEm, Kan, Seq, 0x100);
            pEm->Be_flg |= 4;
        } else
            Add_speedXZ(pEm, 0);

        break;

    case 2:

        Die_move_rot(pEm, 16);
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        d0 = EF(pEm)->Em_keep - (pEm->Cdir_y & 0xFFF);
        if ((pEm->Sca_info & 1) && abs(d0) < 0x400) {
            pEm->Routine_2 = 3;
            *(unsigned long *)&pEm->Move_no = 0xF0011; /* retail (Beta 0xF0012) */
            Spd_get(pEm, Kan, Seq, 0);
            Joint_move(pEm, Kan, Seq, 0x100);
            Snd_se_enem(12, pEm);
            break;
        }
        Em_seq_sound(pEm);
        Add_speedXZ(pEm, 0);
        break;

    case 3:

        Die_move_rot(pEm, 24);
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_2 = 4;
            *(unsigned long *)&pEm->Move_no = 0xF0012; /* retail (Beta 0xF0013) */
        }
        break;

    case 4:

        if (Joint_move(pEm, Kan, Seq, 0x100))
            pEm->Hokan_flg = 15;

        if (*pEm->pNow_seq & 0x10000)
            if (--*(signed char *)&pEm->Timer3 == 0) {
                pEm->Routine_2 = 5;
                pEm->Be_flg |= 0x4A;
                *(unsigned long *)&pEm->Move_no = 0xF0013; /* retail (Beta 0xF0014) */
            }

        break;

    case 5:

        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_2 = 6;
            pEm->Timer3 = 0;
            pEm->Spd_x = 220;
            pEm->Spd_y = -1000;

            EF(pEm)->Jump_flg = 1;
            pEm->Be_flg |= 0x400;
        }
        break;

    case 6:

        if ((signed char)pEm->Timer3 == 0)
            if (Joint_move(pEm, Kan, Seq, 0x100))
                pEm->Timer3 = 1;

        Add_speedXYZ(pEm, 0, 0);
        pEm->Spd_y += 160;

        d0 = -1600; if (*(signed char *)&pEm->Timer2) d0 = -1500;
        if (pEm->Spd_y > 0 && d0 < pEm->Pos_y) {
            pEm->Routine_2 = 7;
            pEm->Pos_y = d0;
            *(unsigned long *)&pEm->Move_no = 0x1000F;
            Snd_se_enem(*(signed char *)&pEm->Timer2 ? 9 : 8, pEm);

            break;
        }
        break;

    case 7:
        if (Joint_move(pEm, Kan, Seq, 0x800)) {
            pEm->Routine_2 = 8;
            pEm->Spd_y = -900; /* retail (Beta -1100) */
            *(unsigned long *)&pEm->Move_no = 0x30014; /* retail (Beta 0x30015) */
            Snd_se_enem(6, pEm);
            Em_dead_flg_on(pEm); /* retail @0x80104840: moved here from case 11 */
        }
        break;

    case 8:

        Joint_move(pEm, Kan, Seq, 0x400);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Spd_y += 160;
        if (pEm->Spd_y >= 0) {
            pEm->Routine_2 = 9;
            Snd_se_enem(14, pEm);
        }
        break;

    case 9:

        if (Joint_move(pEm, Kan, Seq, 0x400))
            pEm->Routine_2 = 10;

        break;

    case 10:

        pEm->Pos_y -= 180;
        if (pEm->Pos_y < -13000)
            pEm->Routine_2 = 11;

        break;

    case 11:
        pEm->Routine_2 = 12;

    }

    if (pEm->Routine_2 < 12) {
        if (EF(pEm)->Jump_flg == 0) {
            if ((0x8000 >> (Rnd() & 15)) & 1)
                Set_blood(pEm, blood_pos_tbl[pEm->Damage_no % 3], 0);

        } else {

            if (pEm->Pos_y >= -9999) {
                if ((0x17E4 >> (Rnd() & 15)) & 1) {
                    Set_blood(pEm, 1, 9);
                    if ((0x17E4 >> (Rnd() & 15)) & 1)
                        Set_blood(pEm, 2, 9);
                }

            }
        }
    }
    Em33_Yure_ck(pEm); /* retail @0x801049E0 */
}

static void Em33_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    Em33_sce_tbl[pEm->Routine_1](pEm, Kan, Seq); }

static void Em33_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        pEm->Sce_free1 = 0;
        *(unsigned long *)&pEm->Move_no = 0xF1310; /* retail 0xF1310 (Beta 0xF1311) */
        EF(pEm)->Core_rno0 = 1;
        EF(pEm)->Core_rno1 = 1;

        pEm->Be_flg |= 0x400;
        pEm->Neck_flg |= 1; /* retail-only */
    }

    if (pEm->Sce_free0 == 1) {
        pEm->Routine_1 = 1;
        *(unsigned short *)&pEm->Routine_2 = 0;
    } }

static void Em33_sce10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        pEm->Spd_x = pEm->Spd_z = 0;
        pEm->Spd_y = 750;
        pEm->Timer3 = 0;
        pEm->Be_flg &= ~0x400;

    case 1:

        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Pos_y + 950 >= pEm->Ground && *(signed char *)&pEm->Timer3 == 0) {
            Snd_se_enem(8, pEm);
            pEm->Timer3 = 1;
            EF(pEm)->Yure_type = 29; /* retail 29/10 (Beta 28/7) */
            EF(pEm)->Yure_time = 10;
        }

        if (pEm->Pos_y + 200 >= pEm->Ground) {
            pEm->Routine_2 = 2;
            pEm->Pos_y = pEm->Ground;
            *(unsigned long *)&pEm->Move_no = 0xE;
            Joint_move(pEm, Kan, Seq, 0x100);
        }

        break;

    case 2:

        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Routine_2 = 3;
            *(unsigned long *)&pEm->Move_no = 0xF0000;
        }
        break;

    case 3:

        Joint_move(pEm, Kan, Seq, 0x100);
        if (pEm->Sce_free0 == 2) {
            pEm->Routine_1 = 2;
            *(unsigned short *)&pEm->Routine_2 = 0;
        }

    }
    Em33_Yure_ck(pEm); /* retail @0x80104C24 */

}

static void Em33_sce20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long d0;

    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0007;
        Spd_get(pEm, Kan, Seq, 0);
        Joint_move(pEm, Kan, Seq, 0x100);
        pEm->Timer3 = 0;
        return;
    }

    Spd_get(pEm, Kan, Seq, 0);
    if (Joint_move(pEm, Kan, Seq, 0x100)) {
        pEm->Routine_1 = 3;
        *(unsigned short *)&pEm->Routine_2 = 0;
    }

    Add_speedXZ(pEm, 0);
    Em_seq_sound(pEm);

    if ((*pEm->pNow_seq & 0x10000) && pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        Snd_se_enem(3, pEm);
        pEm->Timer3 = 15;
    }

    d0 = touch_tbl[0] + 1;
    if (*(signed char *)&pEm->Timer3) {
        Set_blood(pEm, d0, 1);
        Set_blood(pEm, d0 + 2, 1);
        pEm->Timer3--;
    }
    Em33_Yure_ck(pEm); /* retail @0x80104D8C */
}

static void Em33_sce30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0000;
        pEm->Spd_y = -750;

    case 1:

        Joint_move(pEm, Kan, Seq, 0x100);
        if (pEm->Sce_free0 == 3) {
            pEm->Routine_2 = 2;
            *(unsigned long *)&pEm->Move_no = 0xF000D;
        }
        break;

    case 2:

        Joint_move(pEm, Kan, Seq, 0x100);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_2 = 3;
            pEm->Timer3 = 0;
            EF(pEm)->Jump_flg = 1;
            Snd_se_enem(6, pEm);
        }
        break;

    case 3:

        Add_speedXYZ(pEm, 0, 0);
        if ((signed char)pEm->Timer3 == 0)
            if (Joint_move(pEm, Kan, Seq, 0x100)) {
                pEm->Timer3 = 1;
                pEm->Be_flg |= 0x400;
            }

        if (pEm->Pos_y < -9000) {
            pEm->Routine_1 = 4;
            *(unsigned short *)&pEm->Routine_2 = 0;
        }

    }
    Em33_Yure_ck(pEm); /* retail @0x80104F14 */
}

static void Em33_sce40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        *(unsigned long *)&pEm->Move_no = 0xF0000;
    }

    Joint_move(pEm, Kan, Seq, 0x100);
    pEm->Sce_free1 = 1; }

void Pldamage_Em133(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    pldamage_tbl[pEm->Routine_1](pEm, Seq, Kan); }

static void Em33_pldmg_push(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register unsigned long dir;

    Kan = (unsigned long)pEm->pKan_t_ptr;
    Seq = (unsigned long)pEm->pSeq_t_ptr;

    switch (pEm->Routine_2) {

    case 0:
        pEm->Routine_2 = 1;
        dir = pEm->Routine_1;
        pEm->Neck_flg |= 1;
        *(unsigned long *)&pEm->Move_no = dir + 0xF0003;
        pEm->Spd_x = 600;
        pEm->Spd_y = pEm->Spd_z = 0;
        if (dir == 0) pEm->Timer0 = 0x800; else pEm->Timer0 = 0;

        Snd_se_stad((((dir & 1) + 1) << 16) | 0x4000001, &pEm->Pos_x);

    case 1:

        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Neck_flg &= 0xFE;
            pEm->Damage_cnt &= 0x7F;
            *(unsigned long *)&pEm->Routine_0 = 1;
        }

        Add_speedXZ(pEm, pEm->Timer0 - pEm->Cdir_y);

        pEm->Spd_x -= 90;
        if (pEm->Spd_x < 0)
            pEm->Spd_x = 0;

    }

}

static void Em33_pldmg_combo1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register ENEMY_WORK *pG3;
    register PARTS_W *pP_ptr;
    register unsigned long d0; __complex__ unsigned long move_selector; /* [INFERRED] consumed routine selector; no emitted SYM local. */

    Kan = (unsigned long)pEm->pKan_t_ptr;
    Seq = (unsigned long)pEm->pSeq_t_ptr;

    if (pEm->Routine_2 == 0) {
        pEm->Routine_2 = 1;
        __real__ move_selector = pEm->Routine_1;
        pEm->Neck_flg |= 1;
        *(unsigned long *)&pEm->Move_no = __real__ move_selector + 0xF0001;
        Joint_move(pEm, Kan, Seq, 0x100);
        Snd_se_stad(((pEm->Timer0 + 1) << 16) | 0x4000001, &pEm->Pos_x);
    } else {

        if (Joint_move(pEm, Kan, Seq, 0x100)) {
            pEm->Neck_flg &= 0xFE;
            pEm->Damage_cnt &= 0x7F;
            *(unsigned long *)&pEm->Routine_0 = 1;
            return;
        }
    }

    pG3 = (ENEMY_WORK *)pEm->pEnemy_ptr;
    d0 = (*pG3->pNow_seq >> 20) & 0xF;
    if (d0) {
        pP_ptr = &pG3->pSin_parts_ptr[catch_parts_tbl[d0 - 1]];
        pEm->Pos_x = pP_ptr->Workm.t[0];
        pEm->Pos_z = pP_ptr->Workm.t[2];
    }

}

void Pldie_Em133(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
    register PARTS_W *pP_ptr;
    register ENEMY_WORK *pTg;
    register unsigned long chr_type, i;
    register unsigned char Ofs; /* retail-only local, [INFERRED] name (as EM10_S0.C): Dual Shock strength offset */

    pTg = (ENEMY_WORK *)pEm->pEnemy_ptr;

    switch (pEm->Routine_1) {

    case 0:
        pEm->Routine_1 = 1;
        pEm->Neck_flg |= 1;
        pEm->Life = -1;
        *(unsigned long *)&pEm->Move_no = ((pTg->Move_cnt - 1) << 8) + 0xF0006;
        /* retail-only (Dual Shock): vibration script for the kill (retail 0x80105290..0x80105390) */
        Ofs = (G.System_flg & 0x800) ? 15 : 0;
        Pad_vib_set0(8, 0);
        Pad_vib_set1(0x14, 0xFA, 4);
        Pad_vib_fade_set(0x23, 0xFA, 0x3C, 0x18);
        Pad_vib_set0(3, 0x1E);
        Pad_vib_set0(2, 0x28);
        Pad_vib_set0(3, 0x3C - Ofs);
        Pad_vib_set1(8, 0xFA, 0x3C - Ofs);
        Pad_vib_set1(0x23, 0x50, 0x44 - Ofs);
        Pad_vib_set0(2, 0x44 - Ofs);
        Pad_vib_set0(2, 0x4B - Ofs);
        Pad_vib_set0(3, 0x50 - Ofs);
        Pad_vib_set0(3, 0x64 - Ofs);
        Pad_vib_set1(6, 0xFA, 0x64 - Ofs);
        Pad_vib_set0(2, 0x6A - Ofs);
        Pad_vib_set1(8, 0xFA, 0x6E);
        Pad_vib_fade_set(0x3C, 0xFA, 0, 0x76);

    case 1:
        pEm->Cdir_y += 0x800;
        Null_pos_set(pEm, Kan, Seq, 0);
        pEm->Cdir_y -= 0x800;
        pEm->Routine_1 += Joint_move(pEm, Kan, Seq, 0x100);

    }

    chr_type = *pEm->pNow_seq;
    if (chr_type & 0x10000) {
        Set_blood(pEm, 8, 0);
        pP_ptr = &pEm->pSin_parts_ptr[8];
        pP_ptr->Be_flg &= ~1;
        if (pEm->Id & 1) {
            for (i = 15; i < 19; i++) {
                pP_ptr = &pEm->pSin_parts_ptr[i];
                pP_ptr->Be_flg &= ~1;
            }
        }

        pP_ptr = pEm->pSin_parts_ptr;
        pP_ptr->Be_flg |= 0x80;
        *(unsigned long *)&pP_ptr->Free[0] = 0x40;
        *(unsigned long *)&pP_ptr->Free[2] = 0x101020;

        pP_ptr = &pEm->pSin_parts_ptr[9];
        pP_ptr->Be_flg |= 0x80;
        *(unsigned long *)&pP_ptr->Free[0] = 0x40;
        *(unsigned long *)&pP_ptr->Free[2] = 0x101020;

        pP_ptr = &pEm->pSin_parts_ptr[13];
        pP_ptr->Be_flg |= 0x80;
        *(unsigned long *)&pP_ptr->Free[0] = 0x40;
        *(unsigned long *)&pP_ptr->Free[2] = 0x101020;
    }

}

static void Em33_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}

static void Em33_Mv_core(ENEMY_WORK *pEm, EM33_FREE *pFp)
{
    Mv_core_tbl[pFp->Core_rno0](pEm, pFp); }

static void Em33_Mv_core00(ENEMY_WORK *pEm, EM33_FREE *pFp)
{
    switch (pFp->Core_rno1) {
    case 0:
        pFp->Core_rno1 = 1;
        *(signed char *)&Em33_me_tate_d = -15;

    case 1:
        if (Ck_core_tate(Rnd() & 3) > 0)
            pFp->Core_rno1 = 2;

        break;

    case 2:
        if (Ck_core_tate(-(Rnd() & 3)) < 0) {
            pFp->Core_rno1 = 3;
            pFp->Core_free = (Rnd() & 3) * 30 + 1;
        }
        break;

    case 3:
        if (--pFp->Core_free == 0)
            pFp->Core_rno1 = 1;

    }

    Ck_core_yoko(Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 1) > 0 ? 2 : -2);
}

static void Em33_Mv_core10(ENEMY_WORK *pEm, EM33_FREE *pFp)
{
    register short x, y; /* x, y: inferred names, the compiler deletes them, retail SYM has no record */

    if (pFp->Core_free == 0) {
        x = (Rnd() & 15) - 7;
        y = Rnd();
        if (y & 1)
            Ck_core_tate(x);

        if (y & 2)
            Ck_core_yoko(x);

        pFp->Core_free = (Rnd() & 7) * pFp->Core_rno1;
    } else {

        pFp->Core_free--;
    }
}

static void Em33_Mv_core20(ENEMY_WORK *pEm, EM33_FREE *pFp)
{
    register PARTS_W *pP_ptr; /* inferred name: the compiler deletes it, retail SYM has no record */

    switch (pFp->Core_rno1) {

    case 0:
        pFp->Core_rno1 = 1;
        pFp->Core_free = 0;

    case 1:

        if (++pFp->Core_free > 8) {
            pFp->Core_rno1 = 2;
            pFp->Core_free = 8;
        }
        break;

    case 2:

        if (--pFp->Core_free == 0) {
            pFp->Core_rno0 = 0;
            pFp->Core_rno1 = 0;
            pFp->Core_free = 0;
        }
    }

    pP_ptr = &pEm->pSin_parts_ptr[11];
    ((short *)pP_ptr->pSa_dat_head)[6] = pFp->Core_free << 9;
    Sa_parts_allmv(pEm, pP_ptr);
}

static long Ck_core_tate(short Add)
{
    Em33_me_tate_d += Add;

    if ((signed char)Em33_me_tate_d > 15) {
        Em33_me_tate_d = 15;
        return 1;
    }

    if ((signed char)Em33_me_tate_d < -15) {
        *(signed char *)&Em33_me_tate_d = -15;
        return -1;
    }

    return 0; }

static long Ck_core_yoko(short Add)
{
    Em33_me_yoko_d += Add;

    if ((signed char)Em33_me_yoko_d > 15) {
        Em33_me_yoko_d = 15;
        return 1;
    }

    if ((signed char)Em33_me_yoko_d < -15) {
        *(signed char *)&Em33_me_yoko_d = -15;
        return -1;
    }

    return 0; }

static void Em33_Mv_heart(ENEMY_WORK *pEm, EM33_FREE *pFp)
{
    register PARTS_W *pP_ptr;

    if (pFp->Heart_stop) return;

    switch (pFp->Heart_rno0) {
    case 0:
        pFp->Heart_rno0 = 1;
        pFp->Heart_size = 0;
        break;

    case 1:

        pFp->Heart_size += 0x200;
        if (pFp->Heart_size > 0x2800) {
            pFp->Heart_size = 0x2800;
            pFp->Heart_rno0 = 2;
        }
        break;

    case 2:

        pFp->Heart_size -= 0x800;
        if (pFp->Heart_size < 0) {
            pFp->Heart_size = 0;
            pFp->Heart_rno0 = 1;
        }
    }

    pP_ptr = &pEm->pSin_parts_ptr[2];
    if (pFp->Heart_size < 0x1000) {
        ((short *)pP_ptr->pSa_dat_head)[8] = 0;
        ((short *)pP_ptr->pSa_dat_head)[10] = 0x1000 - pFp->Heart_size;
    } else {

        ((short *)pP_ptr->pSa_dat_head)[8] = pFp->Heart_size - 0x1000;
        ((short *)pP_ptr->pSa_dat_head)[10] = 0;
    }
    Sa_parts_allmv(pEm, pP_ptr);
}

static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type)
{
    register PARTS_W *pP_ptr;
    SVECTOR Sv;
    register long dir, Id;
    register EM33_BLOOD_TBL *tbl;

    tbl = &blood_id_tbl[type];

    dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

    pP_ptr = &pEm->pSin_parts_ptr[parts];

    Id = tbl->Id & 0x7F;

    if (!(tbl->Id & 0x80)) {
        Sv.vx = pP_ptr->Workm.t[0];
        Sv.vy = pP_ptr->Workm.t[1];
        Sv.vz = pP_ptr->Workm.t[2];
        Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
    } else {

        Sv.vy = Sv.vz = 0;
        Sv.vx = 300;
        Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &pP_ptr->Workm, &Sv);
    }
}

/* retail-only (em33_cdemd0_s1 0x80105B30): Set_blood with a caller-supplied offset, used
 * twice by Em33_dmg_nml00. Name [INFERRED]; no reference names it. */
static void Set_blood_ofs(ENEMY_WORK *pEm, unsigned char parts, unsigned char type, SVECTOR *pOfs)
{
    register PARTS_W *pP_ptr;
    SVECTOR Sv;
    register long dir, Id;
    register EM33_BLOOD_TBL *tbl;

    tbl = &blood_id_tbl[type];

    dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

    pP_ptr = &pEm->pSin_parts_ptr[parts];

    Id = tbl->Id & 0x7F;

    if (!(tbl->Id & 0x80)) {
        Sv.vx = pOfs->vx + pP_ptr->Workm.t[0];
        Sv.vy = pOfs->vy + pP_ptr->Workm.t[1];
        Sv.vz = pOfs->vz + pP_ptr->Workm.t[2];
        Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &GsIDMATRIX, &Sv);
    } else {

        Esp_call((Id << 24) | ((tbl->Type + tbl->Pal) << 16) | tbl->Zoom, dir, &pP_ptr->Workm, pOfs);
    }
}

static void Em33_Pl_to_dmg(unsigned char dir)
{
    register PLAYER_WORK *pPl = &G.Player; /* inferred name: the compiler deletes it, retail SYM has no record */

    pPl->Routine_0 = 2;
    pPl->Routine_1 = dir + 2;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;

}

static void Em33_Pl_to_dmg2(ENEMY_WORK *pEm, unsigned char Type, unsigned char Dir)
{
    register PLAYER_WORK *pPl = &G.Player; /* inferred name: the compiler deletes it, retail SYM has no record */

    pPl->pEnemy_ptr = (unsigned long *)pEm;
    *(unsigned long *)&pPl->Routine_0 = 5;
    pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
    pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
    pPl->Routine_1 = Type + Dir;

}

static void Em33_Pl_to_die(void)
{
    register PLAYER_WORK *pPl = &G.Player; /* inferred name: the compiler deletes it, retail SYM has no record */

    pPl->Routine_0 = 3;
    pPl->Routine_1 = 0;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
}

static long Arm_touch_ck(ENEMY_WORK *pEm, unsigned long Type)
{
    register PARTS_W *pPt;
    register PLAYER_WORK *pTg;

    pTg = &G.Player;

    pPt = &pEm->pSin_parts_ptr[touch_tbl[Type * 2]];

    if (!(pTg->Damage_cnt & 0x80) && pTg->Life >= 0) {
        if (L_pos_ckXZ(&pTg->Pos_x, pPt->Workm.t, touch_tbl[Type * 2 + 1])) {

            if (abs(pEm->Pos_y - pPt->Workm.t[1]) < 3300)
                return 1;
        }

    }

    return 0;
}

static long Arm_atck_ck(ENEMY_WORK *pEm, unsigned long Type)
{
    register PLAYER_WORK *pPl;
    register PARTS_W *pP_ptr; /* inferred name: the compiler deletes it, retail SYM has no record */

    if (Arm_touch_ck(pEm, Type)) {

        if (Pl_life_down(20, 0) != 2) {
            pPl = &G.Player;
            pP_ptr = &pEm->pSin_parts_ptr[3];
            pPl->Timer0 = Direction_ck(pP_ptr->Workm.t[0], pP_ptr->Workm.t[2], pPl->Pos_x, pPl->Pos_z);

            if (EF(pEm)->Jump_flg == 0) {
                Em33_Pl_to_dmg2(pEm, 0, Cdir_ck(pEm, pPl));
            } else {

                Em33_Pl_to_dmg(Cdir_ck(pEm, pPl));
            }
            Pad_vib_set0(8, 0); /* retail-only vibration @0x80105E84 */
            Pad_vib_set1(8, 0xFA, 4);
            return 1;
        }

        Em33_Pl_to_die();
        return -1;

    }

    return 0;
}

static long Combo_atck_ck(ENEMY_WORK *pEm, unsigned long Type)
{
    register PLAYER_WORK *pPl;

    if (Arm_touch_ck(pEm, Type)) {

        if (Pl_life_down((Type & 2) ? 10 : 20, 0) != 2) { /* retail 10/20 (Beta 5/15) */
            pPl = &G.Player;
            if (Type) {
                Em33_Pl_to_dmg2(pEm, 2, Cdir_ck(pEm, pPl));
            } else {

                Em33_Pl_to_dmg(Cdir_ck(pEm, pPl) + 2);
            }
            Pad_vib_set0(8, 0); /* retail-only vibration @0x80105F58 */
            Pad_vib_set1(8, 0xFA, 4);
            pPl->Timer0 = Type & 1;
            return 1;
        }

        Em33_Pl_to_die();
        return -1;

    }

    return 0;

}

static long Ck_schit(ENEMY_WORK *pEm, unsigned long no, long r)
{
    register PARTS_W *pP_ptr;

    pP_ptr = &pEm->pSin_parts_ptr[no];

    return Sca_ck_hit((VECTOR *)pP_ptr->Workm.t, r, 1 << pEm->nFloor, 0x400);

}

static void Em33_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr)
{
    unsigned char Me3[3] = { 2, 46, 54 };

    unsigned char Me4[15] = { 14, 26, 29, 31, 32, 33, 34, 35, 44, 45, 46, 47, 48, 49, 52 };
    register short Tmp_t, Tmp_y;
    short Dt; /* inferred name: narrowed temporary combined away, retail SYM has no record (as EM28.C / EM31.C) */

    Tmp_t = Em33_me_tate_a;
    Tmp_y = Em33_me_yoko_a;
    if ((signed char)Em33_me_tate_d < (signed char)Em33_me_tate)
        Em33_me_tate_a = -1;

    if ((signed char)Em33_me_tate < (signed char)Em33_me_tate_d)
        Em33_me_tate_a = 1;

    if ((signed char)Em33_me_yoko > (signed char)Em33_me_yoko_d)
        Em33_me_yoko_a = -1;

    if ((signed char)Em33_me_yoko < (signed char)Em33_me_yoko_d)
        Em33_me_yoko_a = 1;

    if (-Em33_me_tate_a == Tmp_t)
        Em33_me_tate_mv = Em33_me_tate_mv * 2 / 3;

    if (-Em33_me_yoko_a == Tmp_y)
        Em33_me_yoko_mv = Em33_me_yoko_mv * 2 / 3;

    if ((signed char)Em33_me_tate == (signed char)Em33_me_tate_d && abs(Em33_me_tate_mv) < 3) {
        Em33_me_tate = Em33_me_tate_d;
        Em33_me_tate_a = 0;
        Em33_me_tate_mv = 0;
    }
    if (Em33_me_yoko == (signed char)Em33_me_yoko_d && abs(Em33_me_yoko_mv) < 3) {
        Em33_me_yoko = Em33_me_yoko_d;
        Em33_me_yoko_a = 0;
        Em33_me_yoko_mv = 0;
    }

    Em33_me_tate_mv += Em33_me_tate_a;
    Em33_me_yoko_mv += Em33_me_yoko_a;

    Tmp_t = (signed char)Em33_me_tate;
    Tmp_y = (signed char)Em33_me_yoko;

    Em33_me_tate += Em33_me_tate_mv;
    Em33_me_yoko += Em33_me_yoko_mv;

    if ((signed char)Em33_me_tate > 20)
        Em33_me_tate = 20;

    if ((signed char)Em33_me_tate < -17)
        Em33_me_tate = -17;

    if ((signed char)Em33_me_yoko > 20)
        Em33_me_yoko = 20;

    if ((signed char)Em33_me_yoko < -17)
        Em33_me_yoko = -17;

    Dt = (signed char)Em33_me_tate - Tmp_t;
    Tmp_y = (signed char)Em33_me_yoko - Tmp_y;

    Texture_Move_GT3(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me3);

    Texture_Move_GT4(&pP_ptr->Attribute, pEm->Tpage + 2, (Dt << 16) | (unsigned short)Tmp_y, pEm->Clut + 0x1E2, Me4);

}

static void Set_sabun(ENEMY_WORK *pEm, unsigned long Parts, unsigned long Ofs, short Rate)
{
    register PARTS_W *pP_ptr;

    pP_ptr = &pEm->pSin_parts_ptr[Parts];
    ((short *)pP_ptr->pSa_dat_head)[Ofs * 2 + 6] = Rate;
    Sa_parts_allmv(pEm, pP_ptr);
}

/* retail-only (em33_cdemd0_s1 0x8010641C, last function): the screen-shake countdown that
 * Beta 2 ran inline in Em133, now called at the end of the Move/Damage/Die/scenario
 * handlers. Name [INFERRED] from the Yure_time/Yure_type fields it consumes. */
static void Em33_Yure_ck(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Yure_time) {
        G.Status_flg |= 0x80000000 >> EF(pEm)->Yure_type;
        if (!--EF(pEm)->Yure_time) {
            G.Status_flg &= 0xfffffff7;
            G.Status_flg &= 0xfffffffb;
        }
    }
}
