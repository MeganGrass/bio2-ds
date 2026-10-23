/* EM3A.H - types used only by the EM3A overlay (retail layout). Shared types are in the project headers (MAIN.H, EM_SUB.H, PLAYER.H). */
#ifndef EM3A_H
#define EM3A_H

#include "TYPES.H"
#include "MAIN.H"
#include "PLAYER.H"
#include "ROOM.H"

/* The project ENEMY_WORK places Free[] 4 bytes before the retail offset (+0x218) the overlays use. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares these routines with narrower or unsigned results; this overlay calls them as the retail code does, with an int result. */
#define Cdir_ck	((LONG (*)())Cdir_ck)
#define Rnd	((LONG (*)())Rnd)

#ifdef OVL_3B
#define EM_MAIN		Em23a
#define PLDAMAGE_EM	Pldamage_Em23a
#else
#define EM_MAIN		Em13a
#define PLDAMAGE_EM	Pldamage_Em13a
#endif

/* ---- functions of EM3A.C ---- */
/* retail-only resident Dual Shock vibration helpers (config/retail/main.symbols.txt Pad_vib_set0
 * 0x8003947C / Pad_vib_set1 0x80039514; references/bio2-ds/Bio2/prog/PLAYER.H:266-267). */
extern void Pad_vib_set0();
extern void Pad_vib_set1();

void EM_MAIN(ENEMY_WORK *pEm);
static void Em3a_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_select(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Ck_esc_set(ENEMY_WORK *pEm);
static void Em3a_Move_fly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_fly00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Set_under_smoke(ENEMY_WORK *pEm);
static void Em3a_Mv_fly10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_fly20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_fly30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_801013EC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void Em3a_Move_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_escturn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_wall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_wall00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_wall10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_wall20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_walk2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_wallset(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_wallset00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_wallset10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Move_newfly(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_newfly10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Mv_newfly30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_801026FC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void Em3a_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Esc_dmg_ck(ENEMY_WORK *pEm);
static void Em3a_dmg_ftl(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_ftl00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Dmg_ftl00_0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_dmg_fire200(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_nml20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_gnml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_fire00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Die_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3a_kage_move(ENEMY_WORK *pEm);
void PLDAMAGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Wing_set(ENEMY_WORK *pEm, unsigned long No, short Rx, short Ry);
static void Em3a_Wing(ENEMY_WORK *pEm);
static void Em3a_Wing_dmg(ENEMY_WORK *pEm, EM3A_FREE *pFp);
static void Em3a_Wing_cont(ENEMY_WORK *pEm, EM3A_FREE *pFp);
static void Em3a_Wing_right(ENEMY_WORK *pEm, EM3A_FREE *pFp);
static void Wing_fast_main(ENEMY_WORK *pEm, EM3A_FREE *pFp);
/* Get_target_pl has no prototype: Em3a_Move calls it through an implicit declaration
   (which opens the record-less outer block of Em3a_Move in the retail SYM). */
static unsigned long Touch_ck_main(ENEMY_WORK *pEm, unsigned long Pl, long Range, short Dir);
static unsigned long Bodyatck_ck(ENEMY_WORK *pEm);
static void Em3a_Pl_to_dmg(unsigned long pl, unsigned char dir);
static void Em3a_Pl_to_die(unsigned long pl);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static void Set_fire(ENEMY_WORK *pEm, unsigned long Type, unsigned long Tbl);
static void Set_smoke(ENEMY_WORK *pEm, unsigned long Type, SVECTOR *pBs, SVECTOR *pOf);
static void Set_schit(ENEMY_WORK *pEm, unsigned long Type);

#endif /* EM3A_H */
