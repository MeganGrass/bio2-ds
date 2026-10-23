/* EM2E.H - types used only by the EM2E overlay (retail layout). */
#ifndef EM2E_H
#define EM2E_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
/* retail callers use the full word Cdir_ck returns; EM_SUB.H declares a UCHAR result, which adds a byte mask */
#define Cdir_ck ((long (*)())Cdir_ck)
#define Rot_neck_em ((void (*)())Rot_neck_em)
#define Goto00_ck ((long (*)())Goto00_ck)
#define Rnd ((long (*)())Rnd)

#ifdef OVL_31
#define EM_MAIN		Em22e
#define PLDAMEGE_EM	Pldamage_Em22e
#define PLDIE_EM	Pldie_Em22e
#else
#define EM_MAIN		Em12e
#define PLDAMEGE_EM	Pldamage_Em12e
#define PLDIE_EM	Pldie_Em12e
#endif

/* ---- functions of EM2E.C ---- */
/* retail-only resident Dual Shock vibration helpers at 0x8003947C / 0x80039514 / 0x800395B8
 * (Executable.map sub_8003947C/sub_80039514/sub_800395B8); names and parameter types from
 * references/bio2-ds/Bio2/prog/PLAYER.H:266-268. */
extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_set1(int Time, unsigned char Power, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);

void EM_MAIN(ENEMY_WORK *pEm);
static void Em2e_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Fire_die_ck(ENEMY_WORK *pEm);
static void Em2e_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Mv_walk00(ENEMY_WORK *pEm);
static void Em2e_atck_ck(ENEMY_WORK *pEm);
static void Em2e_Mv_reverse(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_rev00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Mv_rev00_0(ENEMY_WORK *pEm);
static void Em2e_Mv_rev10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_struck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_struck000(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_struck001(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Mv_struck001_0(ENEMY_WORK *pEm);
static void Em2e_Mv_struck002(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_poison(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_poison00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_poison10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_poison20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_poison30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_neck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_neck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_neck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_neck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_open(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_fall00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_fall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_fall20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_gas(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_catch(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_catch00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_catch10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Mv_catch20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_801030DC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_80103118(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_8010319C(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Set_dmg_fire(ENEMY_WORK *pEm, unsigned long type, long max);
static void Em2e_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_dmg_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Break_parts(ENEMY_WORK *pEm, unsigned long st, unsigned long ed);
static void Em2e_Die_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_80103E78(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Pcolor_dec(ENEMY_WORK *pEm);
static void func_801042AC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_dmg00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_dmg10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_dmg20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_die10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Pl_die20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2e_kage_move(ENEMY_WORK *pEm);
static void Em2e_Move_flower(ENEMY_WORK *pEm);
static void Em2e_Mv_flower00(ENEMY_WORK *pEm, EM2E_FREE *pFp);
static void Em2e_Mv_flower10(ENEMY_WORK *pEm, EM2E_FREE *pFp);
static void Em2e_Mv_flower20(ENEMY_WORK *pEm, EM2E_FREE *pFp);
static PLAYER_WORK *Get_target_pl(ENEMY_WORK *pEm);
static long Arm_ck_main(ENEMY_WORK *pEm, PLAYER_WORK *pTg, unsigned long No, long R);
static long Arm_hit_ck(ENEMY_WORK *pEm);
static void Em2e_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir);
static void Em2e_Pl_to_die(PLAYER_WORK *pPl);
static void Set_flower(ENEMY_WORK *pEm);
static unsigned long Head_open_ck(ENEMY_WORK *pEm);
static unsigned long Hand_atck_ck(ENEMY_WORK *pEm);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static long Cut_hand(ENEMY_WORK *pEm, unsigned long Id);
static long Ck_schit_parts(ENEMY_WORK *pEm, unsigned long no, long r);
static short Em2e_goto00_ck(ENEMY_WORK *pEm, long dx, long dz, short rot);
static void Get_workm(ENEMY_WORK *pEm);
static void Catch_pos_set(ENEMY_WORK *pEm, ENEMY_WORK *pTg, long Ofs);
static short Parts_to_pl(ENEMY_WORK *pEm, unsigned long No, short Rot, long *Dist);
static void Oba_ck_em2e(ENEMY_WORK *pEm);

/* Declared (not implicit): retail SYM has no binding block for Em2e_Init's
   hand-spawn loop body, which an implicit declaration there would create.
   Signature from EM_SET.C. */
extern ENEMY_WORK *Set_enemy_work(unsigned char Id);

/* Declared (not implicit): Arm_ck_main's "pTg->Life >= 0" body has no SYM
   binding block, which an implicit Spl_life_down there would create.
   Signature from SPL_AUTO.C. */
extern unsigned long Spl_life_down(ENEMY_WORK *pEm, long Down_life, unsigned long Mode);
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);
extern unsigned long Flg_ck(unsigned long *p, unsigned long x);	/* resident 0x80077360; signature as declared in src/prog/esp_r.c:13 */

#endif /* EM2E_H */
