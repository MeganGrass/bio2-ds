/* EM10.H - types used only by the EM10 overlay (retail layout). Shared types come from the project headers. */
#ifndef EM10_H
#define EM10_H

#include "TYPES.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "CONFIG.H"
#include "ROOM.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "SCA.H"
#include "OMA.H"
#include "JOINT.H"

/* ENEMY_WORK.Free[] starts 4 bytes after the end of the Kage_ofs member (+0x218); the project struct has no
 * Retail_214/Retail_216 halfwords in front of it, so the overlays address it with this offset. */
#define OVL_FREE(p) ((ULONG *)((UCHAR *)(p)->Free + 4))

struct SPL_FREE_WORK {
    short Dest_x;
    short Dest_z;
    unsigned short Move_flg;
    unsigned char Spl_dummy000;
    unsigned char Spl_dummy001;
    unsigned long ll;
    unsigned long em_ll;
    unsigned long Spl_dummy00;
    unsigned long Spl_dummy01;
    unsigned long Spl_dummy02;
    unsigned long Spl_dummy03;
};

#ifdef OVL_23
#define EM_MAIN		Em210
#define PLDAMEGE_EM	Pldamege_Em210
#define PLDIE_EM	Pldie_Em210
#else
#define EM_MAIN		Em110
#define PLDAMEGE_EM	Pldamege_Em110
#define PLDIE_EM	Pldie_Em110
#endif

/* ---- functions of EM10.C ---- */
/* retail-only resident function (no Beta twin; Executable.map label sub_80028794; references/bio2-ds/
 * Bio2/prog/JOINT.C:696-702 calls it Parts_work_kan_init(ENEMY_WORK *, USHORT *), an inferred name).
 * [INFERRED] prototype from the call in Em10_mv0f: a0 = pEm, a1 = pEm->pKan_t_ptr. */
/* retail-only resident function JOINT.C Joint_move_skip (DS 0x8002a9c8; no Beta or mockup twin, name in the
 * JOINT.C convention): advances the motion one frame without posing the parts and returns 1 at the end of
 * the cycle. Em10_mv01 and Em10_mv02 call it where Beta called Joint_move: a0 = pEm, a1 = pEm->pSeq_t_ptr. JOINT.H owns
 * the prototype; the overlay cannot include it (SDK headers). */
extern unsigned long Joint_move_skip(ENEMY_WORK *pEm, unsigned long Seq_t_ptr);
/* retail-only resident Dual Shock vibration helpers: Pad_vib_set0 0x8003947C, Pad_vib_set1 0x80039514,
 * Pad_vib_fade_set 0x800395B8 (names from references/bio2-ds/Bio2/prog/PLAYER.H:265-268, as in
 * config/retail/main.symbols.txt; inferred, not original). Declared without a prototype: the retail
 * calls pass plain ints. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em10_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv00_br(ENEMY_WORK *pEm);
static void Em10_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_801016C8(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void Em10_mv01_br(ENEMY_WORK *pEm);
static void Em10_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv02_br(ENEMY_WORK *pEm);
static void Em10_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv03_br(ENEMY_WORK *pEm);
static void Em10_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv04_br(ENEMY_WORK *pEm);
static void Em10_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv05_br(ENEMY_WORK *pEm);
static void Em10_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv06_br(ENEMY_WORK *pEm);
static void Em10_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv07_br(ENEMY_WORK *pEm);
static void Em10_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv08_br(ENEMY_WORK *pEm);
static void Em10_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv09_br(ENEMY_WORK *pEm);
static void Em10_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0a_br(ENEMY_WORK *pEm);
static void Em10_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0b_br(ENEMY_WORK *pEm);
static void Em10_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0c_br(ENEMY_WORK *pEm);
static void Em10_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0d_br(ENEMY_WORK *pEm);
static void Em10_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0e_br(ENEMY_WORK *pEm);
static void Em10_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv0f_br(ENEMY_WORK *pEm);
static void Em10_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv10_br(ENEMY_WORK *pEm);
static void Em10_mv10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_mv11_br(ENEMY_WORK *pEm);
static void Em10_mv11(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Firedm(ENEMY_WORK *pEm);
static void Em10_Ryudm(ENEMY_WORK *pEm);
static void Em10_Tudm(ENEMY_WORK *pEm);
static void Em10_Spdm(ENEMY_WORK *pEm);
static void Em10_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_dm08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em10_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_die05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_sce01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_sce02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_sce03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_sce04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_pldm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Kahannsin_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em10_Kubigoro(ENEMY_WORK *pEm);
static void Em10_water(ENEMY_WORK *pEm);
static void Em10_ude_down(ENEMY_WORK *pEm);

/* Called WITHOUT a prototype in scope (implicit int declarations): Rnd(), Snd_se_enem(), Foot_set_pl(),
   Goto00(), Goto00_ck(), Spd_get(), Add_speedXZ(), Dir_pos_ck(), Null_pos_set(), Set_base_pos(), Cdir_ck(),
   Esp_call(), Texture_Move(), Kage_work_set(), Oba_ck_em(), Em_dead_flg_on(), CompM(), Direction_ck(), HMatrix(), Cdir_ck2(), Print8(), Snd_se_stad(),
   Joint_move_p(), MulKage(), Sca_get_ground(), Sca_ck_hit().
   gcc pushes an implicit declaration into the innermost block where the callee is first used, which is what
   produces the empty retail SYM blocks (e.g. EM_MAIN 79-193, Em10_Init's Texture_Move blocks); the
   arguments are passed unnarrowed (e.g. the dir_y argument of Esp_call). */
/* The functions below were called through implicit int declarations; the project headers prototype them, so the calls
 * go through an unprototyped function pointer to keep the argument passing and result type. */
#define Rnd ((int (*)())Rnd)
#define Foot_set_pl ((int (*)())Foot_set_pl)
#define Goto00 ((int (*)())Goto00)
#define Goto00_ck ((int (*)())Goto00_ck)
#define Spd_get ((int (*)())Spd_get)
#define Add_speedXZ ((int (*)())Add_speedXZ)
#define Dir_pos_ck ((int (*)())Dir_pos_ck)
#define Null_pos_set ((int (*)())Null_pos_set)
#define Set_base_pos ((int (*)())Set_base_pos)
#define Cdir_ck ((int (*)())Cdir_ck)
#define Texture_Move ((int (*)())Texture_Move)
#define Kage_work_set ((int (*)())Kage_work_set)
#define Em_dead_flg_on ((int (*)())Em_dead_flg_on)
#define CompM ((int (*)())CompM)
#define Direction_ck ((int (*)())Direction_ck)
#define HMatrix ((int (*)())HMatrix)
#define Cdir_ck2 ((int (*)())Cdir_ck2)
#define Joint_move_p ((int (*)())Joint_move_p)
#define MulKage ((int (*)())MulKage)
#define Sca_get_ground ((int (*)())Sca_get_ground)
#define Sca_ck_hit ((int (*)())Sca_ck_hit)
extern unsigned long Pl_life_down(long Down_life, unsigned long Mode);
extern unsigned long Gacha_ch(void);
extern unsigned char Rnd_area(void);
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);
extern unsigned char Root_ck();	/* unprototyped: retail passes Mode without narrowing */
extern void Rot_neck_em(ENEMY_WORK *pPm, short Cdir_y);
extern void Em_seq_sound(ENEMY_WORK *pEm);
extern void Sca_ck_em(ENEMY_WORK *pEm, unsigned long Sc_id);
extern void Spl_Scenario();
extern unsigned char Guide_check();	/* ROOT_CK.C; unprototyped: retail passes the long coordinates unnarrowed */
extern void MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm);	/* MD_SUB.C; used by the retail Em10_ude_down */

/* ---- functions of EM10_S0.C ---- */
/* retail-only resident Dual Shock vibration helpers, see em10.c (bio2-ds PLAYER.H:266-268 names them
 * Pad_vib_set0 / Pad_vib_set1 / Pad_vib_fade_set). Declared without a prototype: plain int arguments. */

#endif /* EM10_H */
