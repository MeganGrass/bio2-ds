/* EM25.H - types used only by the EM25 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM25_H
#define EM25_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"

/* This overlay calls Rnd() as a plain int-returning function (no andi after the jal), while EM_SUB.H declares it UCHAR;
 * calling it through a LONG-returning type reproduces the call without changing the project prototype (see Rnd_s32 in ESP_R.C). */
#define Rnd()	(((LONG (*)(VOID))Rnd)())

/* The overlay calls these project functions without a prototype (implicit int declarations, arguments passed unnarrowed),
 * so each name is called through a K&R function type; the prototypes in the project headers stay untouched. */
#define Sca_get_dir	(*(int (*)())Sca_get_dir)

/* Em25 work kept in the enemy generic Free[] area; it starts 4 bytes after ENEMY_WORK.Free (retail +0x218). */
#define EF_OF(pE)	((EM25_FREE *)&(pE)->Free[1])
#define EF		EF_OF(pEm)

#ifdef OVL_28
#define EM_MAIN		Em225
#else
#define EM_MAIN		Em125
#endif

/* ---- functions of EM25.C ---- */
/* retail-only resident Dual Shock vibration helpers Pad_vib_set0 @0x8003947C / Pad_vib_set1
 * @0x80039514 (config/retail/main.symbols.txt; references/bio2-ds/Bio2/prog/PAD.C:309,325).
 * Called unprototyped. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em25_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Move_floor(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Move_ceiling(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv011(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv071(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only */
static void Em25_Move_walldown(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_wall_init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall50(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall60(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall70(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static short Get_wallshot(ENEMY_WORK *pEm);
static void Em25_Move_wallset(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Mv_wall201(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em25_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl110(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl111(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl112(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl210(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_ftl211(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_dm_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml100(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml101(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml1010(ENEMY_WORK *pEm);
static void Em25_Die_nml102(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml201(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_nml30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_ftl30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Die_break_set(ENEMY_WORK *pEm);
static void Em25_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_float_move(ENEMY_WORK *pEm);
static void Em25_foot_set(ENEMY_WORK *pEm, unsigned char Flg);
static short Em25_goto00(ENEMY_WORK *pEm, short rr);
static unsigned long Ck_lock_wall(ENEMY_WORK *pEm);
static void Em25_wall_rot(ENEMY_WORK *pEm, short rot);
static void Em25_add_speed(ENEMY_WORK *pEm, long rx, long ry, long rz);
static long Ck_wall_edge(ENEMY_WORK *pEm, short Scdir);
static long Ck_near_wall(ENEMY_WORK *pEm, SCA_DATA *pSc, short dir);
static long Ck_wall_other(ENEMY_WORK *pEm, short r);
static short Ck_wall_to_pl(ENEMY_WORK *pEm, short rot);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static void Set_poison(ENEMY_WORK *pEm, unsigned long Type, short Dir);
static unsigned long atck_ck_main(ENEMY_WORK *pEm, unsigned long pl, long R);
static unsigned long Em25_atck_ck(ENEMY_WORK *pEm, long R, long H);
static long Foot_break_set(ENEMY_WORK *pEm);
static void Em25_Set_child(ENEMY_WORK *pEm, unsigned short type, unsigned char cnt);
static void Em25_Pl_to_dmg(unsigned long pl, unsigned char dir);
static void Em25_Pl_to_die(unsigned long pl);
static void Set_foot_se(ENEMY_WORK *pEm);
static void Chg_modelcol(ENEMY_WORK *pEm, unsigned long Rgb);
static long Ck_land(ENEMY_WORK *pEm, long Fe, long Fg, long Fw);

/* Called WITHOUT a prototype in scope (implicit int declarations): Rnd(), Joint_move(), Oba_ck_em(),
   Sca_ck_hit(), Sca_get_dir(), Sca_get_pos(), Snd_se_enem(), Direction_ck(), Goto00_ck(), Cdir_ck(),
   Ck_em_id(), Spl_life_down(), Pl_life_down(), Esp_call(), RotMatrix(), CompM(),
   ApplyMatrixSV(), Add_speedXZ(), Add_speedXYZ(), Em_dead_flg_on(), abs() (gcc builtin, expanded inline),
   and the EM25_S0.C functions Em25_kage_move(), Em25_spark_move(), Get_target_pl(), Set_gunrange()
   (used before their definition).  gcc pushes an implicit declaration into the innermost block where the
   callee is first used, which is what produces the retail SYM blocks that record no locals (e.g. the
   then/else blocks of Ck_wall_edge, Set_blood, Em25_atck_ck); the arguments are passed unnarrowed. */
extern void Kage_work_set(unsigned long *pK, unsigned long Off, unsigned long Half, unsigned long Color, VECTOR *pPos);
extern void Sca_ck_em(ENEMY_WORK *pEm, unsigned long Sc_id);
extern ENEMY_WORK *Set_enemy_work();	/* declared: retail Em25_Set_child's loop body has no implicit-declaration block */
extern void Goto00(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);
extern void Em_seq_sound(ENEMY_WORK *pEm);
extern long Water_ck(long Pos_x, long Pos_z);

/* ---- functions of EM25_S0.C ---- */

static void Em25_dm_ftl12(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em25_kage_move(ENEMY_WORK *pEm);
static ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm);
static void Em25_spark_move(ENEMY_WORK *pEm);
static void Set_gunrange(ENEMY_WORK *pEm);

static long Em25_mv08sp(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);

#endif /* EM25_H */
