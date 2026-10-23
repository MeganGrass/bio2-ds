/* EM21.H - types used only by the EM21 overlay (retail layout). Shared types are in the project headers (MAIN.H, EM_SUB.H, PLAYER.H). */
#ifndef EM21_H
#define EM21_H

#include "TYPES.H"
#include "MAIN.H"
#include "PLAYER.H"
#include "ROOM.H"

/* The project ENEMY_WORK places Free[] 4 bytes before the retail offset (+0x218) the overlays use. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares Rnd returning UCHAR; this overlay calls it as the retail code does, with an int result. */
#define Rnd	((LONG (*)())Rnd)
#define Cdir_ck	((LONG (*)())Cdir_ck)
#define Gacha_ch	((LONG (*)())Gacha_ch)
#define Sca_get_dir	((LONG (*)())Sca_get_dir)

#ifdef OVL_25
#define EM_MAIN		Em221
#define PLDAMEGE_EM	Pldamege_Em221
#else
#define EM_MAIN		Em121
#define PLDAMEGE_EM	Pldamege_Em121
#endif

/* ---- functions of EM21.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em21_kage_set(ENEMY_WORK *pEm);
static void Em21_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_wait_i(ENEMY_WORK *pEm);
static void Em21_mv_wait_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_go_wait_i(ENEMY_WORK *pEm);
static void Em21_mv_go_wait_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_take_off_i(ENEMY_WORK *pEm);
static void Em21_mv_take_off_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_glide_i(ENEMY_WORK *pEm);
static void Em21_mv_glide_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_flight_i(ENEMY_WORK *pEm);
static void Em21_mv_flight_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_up_i(ENEMY_WORK *pEm);
static void Em21_mv_up_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_down_i(ENEMY_WORK *pEm);
static void Em21_mv_down_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_landing_i(ENEMY_WORK *pEm);
static void Em21_mv_landing_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_quick_turn_i(ENEMY_WORK *pEm);
static void Em21_mv_quick_turn_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_jump_i(ENEMY_WORK *pEm);
static void Em21_mv_jump_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_fall_i(ENEMY_WORK *pEm);
static void Em21_mv_fall_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_crash_f_i(ENEMY_WORK *pEm);
static void Em21_mv_crash_f_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_crash_i(ENEMY_WORK *pEm);
static void Em21_mv_crash_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_peck_f_i(ENEMY_WORK *pEm);
static void Em21_mv_peck_f_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_br_peck_i(ENEMY_WORK *pEm);
static void Em21_mv_peck_i(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_parts_bobm_set(PARTS_W *pP_ptr, long B_y);
static void Em21_parts_fall_set(PARTS_W *pP_ptr, long D_y);
static void Em21_dm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dm03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_sce_wait(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_sce_rush(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_sce_crash(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dead_normal(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dead_bomb(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dead_bow(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dead_instant(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_foot_set(ENEMY_WORK *pEm);
static void Em21_r0_set(ENEMY_WORK *pEm, unsigned long Rno0, unsigned long Rno1);
static void Em21_r1_set(ENEMY_WORK *pEm, unsigned long Rno1);
static void Em21_sca_avoid_ck(ENEMY_WORK *pEm);
static void Em21_attack_ck(ENEMY_WORK *pEm);
static void Em21_run_away_ck(ENEMY_WORK *pEm);
static void Em21_pl_dead_ck(ENEMY_WORK *pEm);
static void Em21_lock_off_ck(ENEMY_WORK *pEm);
static void Em21_set_flg_all(unsigned long Mask);
static void Em_21_blood_call(ENEMY_WORK *pEm);
static void Em_21_blood_call2(ENEMY_WORK *pEm);
static void Em_21_feather_call(ENEMY_WORK *pEm, long Num);
static void Em21_atd_set(ENEMY_WORK *pEm, long Size);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em21_dummy_br(ENEMY_WORK *pEm);
static void Em21_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

#endif /* EM21_H */
