/* EM20.H - types used only by the EM20 overlay (retail layout). */
#ifndef EM20_H
#define EM20_H

#include "TYPES.H"

/* Retail calls these through unprototyped declarations (int result, int arguments, no narrowing), so the
 * prototypes of the owner headers are renamed while they are read. */
#define Rnd Rnd_owner
#define Pad_vib_set0 Pad_vib_set0_owner
#define Pad_vib_set1 Pad_vib_set1_owner
#define Pad_vib_fade_set Pad_vib_fade_set_owner
#define Cdir_ck Cdir_ck_owner
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "OMA.H"
#include "SCA.H"

#undef Rnd
#undef Pad_vib_set0
#undef Pad_vib_set1
#undef Pad_vib_fade_set
#undef Cdir_ck

extern void Pad_vib_set0(long Time, long Attr);
extern void Pad_vib_set1(long Time, long Power, long Attr);
extern void Pad_vib_fade_set(long Time, long Pow_st, long Pow_ed, long Attr);
#ifdef OVL_24
#define EM_MAIN		Em220
#define PLDIE_EM	Pldie_Em220
#else
#define EM_MAIN		Em120
#define PLDIE_EM	Pldie_Em120
#endif

/* ---- functions of EM20.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em20_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static unsigned long Em20_rndwalk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_yori(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_run00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_run10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_run20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_run30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Jump_set(ENEMY_WORK *pEm, long spd_x, long spd_y);
static void Em20_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static long Em20_Mv_jump10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static long Em20_Mv_jump20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_atck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_sleep(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_sleep00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_sleep10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_sleep20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_sleep30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_back(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_critical00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_critical10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_escape(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_floorout(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_walk2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_died(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_eat(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_mv_eat00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_house(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_mv_house00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_mv_house10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_rush(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_rush00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_rush10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_rush20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_bodyatck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_bodyatck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_bodyatck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_bodyatck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Move_jumpin(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_jumpin00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Mv_jumpin20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_combo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_combo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_combo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_combo30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dmg_nml2_00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Set_dmghit(ENEMY_WORK *pEm, unsigned long Type);
static void Em20_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_bang(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_bang00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Set_head_bomb(ENEMY_WORK *pEm);
static void Em20_Die_bang10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_gnml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Die_spark00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em20_Blood_move(ENEMY_WORK *pEm);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Dog_rot_neck(ENEMY_WORK *pEm);
static void Rate_speed_up(ENEMY_WORK *pEm, long g, long limit);
static void Rate_speed_down(ENEMY_WORK *pEm, long g, long limit);
static void Set_run_motion(ENEMY_WORK *pEm, short d_tmp);
static long Atck_range_ck(ENEMY_WORK *pEm);
static unsigned long hit_ck_main(ENEMY_WORK *pEm, long pl);
static unsigned long Atck_hit_ck(ENEMY_WORK *pEm);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static void Em20_Pl_to_dmg(unsigned long pl, unsigned char dir);
static void Em20_Pl_to_die(unsigned long pl);
static void Ck_Floor(ENEMY_WORK *pEm);
static void Ck_Died(ENEMY_WORK *pEm);
static ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm);

extern void Em_dead_flg_on(ENEMY_WORK *pEm);
extern void Em_seq_sound(ENEMY_WORK *pEm);
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);
extern void Sca_ck_em(ENEMY_WORK *pEm, unsigned long Sc_id);

/* retail overlay work area is ENEMY_WORK+0x218; project Free is at +0x214 */
#define OVL_FREE(p) ((ULONG *)((UCHAR *)(p)->Free + 4))

#endif /* EM20_H */
