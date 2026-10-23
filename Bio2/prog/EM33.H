/* EM33.H - types used only by the EM33 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM33_H
#define EM33_H

#include "TYPES.H"
#include "MAIN.H"
#include "ROOM.H"

/* ENEMY_WORK.Free[] starts 4 bytes later in the retail structure than in EM_SUB.H (two halfwords precede it), so the overlay addresses it through this offset. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares Direction_ck with long parameters and Cdir_ck and Rnd returning UCHAR; this overlay calls them through the narrower old-style types the retail code uses. */
#define Direction_ck	((LONG (*)())Direction_ck)
#define Cdir_ck	((LONG (*)())Cdir_ck)
#define Rnd	((LONG (*)())Rnd)

#ifdef OVL_35
#define Em133 Em233
#define Pldamage_Em133 Pldamage_Em233
#define Pldie_Em133 Pldie_Em233
#endif

/* ---- functions of EM33.C ---- */
/* declared: retail SYM has no implicit-declaration block at their calls */
extern unsigned long Joint_move();
extern void Rot_neck_em(ENEMY_WORK *pPm, short Cdir_y); /* retail Em133 @0x80100158 */
/* retail-only resident Dual Shock helpers (labels as in EM10/EM20; references/bio2-ds PAD.C:309,325,341
 * name them Pad_vib_set0/Pad_vib_set1/Pad_vib_fade_set). Declared without prototypes like EM10. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();

void Em133(ENEMY_WORK *pEm);
static void Em33_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_tojo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_tojo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_tojo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static unsigned long Mv_tojo20_0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq, short Rate);
static void Mv_tojo20_1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_tojo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_tojo30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static long Mv_atck_ck(ENEMY_WORK *pEm);
static void Em33_Mv_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Mv_turn_ck(ENEMY_WORK *pEm);
static void Em33_Mv_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static long Arm_spark_ck(ENEMY_WORK *pEm, unsigned char no);
static void Em33_Mv_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Mv_kill_ck(ENEMY_WORK *pEm);
static void Em33_Mv_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_combo00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_combo10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_combo20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_critical00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_critical10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_critical20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_jump(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_jump00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_jump10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_jump20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static long jump_ck_main(ENEMY_WORK *pEm, long Type);
static void Mv_jump_ck(ENEMY_WORK *pEm);
static void Em33_Mv_yoro(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_ikaku(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_nml00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_nml10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_mag(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_combo(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_dmg_spark(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_magdmg(ENEMY_WORK *pEm);
static long Die_move_rot(ENEMY_WORK *pEm, short rot);
static void Em33_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_sce10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_sce20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_sce30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_sce40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void Pldamage_Em133(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_pldmg_push(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_pldmg_combo1(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void Pldie_Em133(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em33_Mv_core(ENEMY_WORK *pEm, EM33_FREE *pFp);
static void Em33_Mv_core00(ENEMY_WORK *pEm, EM33_FREE *pFp);
static void Em33_Mv_core10(ENEMY_WORK *pEm, EM33_FREE *pFp);
static void Em33_Mv_core20(ENEMY_WORK *pEm, EM33_FREE *pFp);
static long Ck_core_tate(short Add);
static long Ck_core_yoko(short Add);
static void Em33_Mv_heart(ENEMY_WORK *pEm, EM33_FREE *pFp);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static void Em33_Pl_to_dmg(unsigned char dir);
static void Em33_Pl_to_dmg2(ENEMY_WORK *pEm, unsigned char Type, unsigned char Dir);
static void Em33_Pl_to_die(void);
static long Arm_touch_ck(ENEMY_WORK *pEm, unsigned long Type);
static long Arm_atck_ck(ENEMY_WORK *pEm, unsigned long Type);
static long Combo_atck_ck(ENEMY_WORK *pEm, unsigned long Type);
static long Ck_schit(ENEMY_WORK *pEm, unsigned long no, long r);
static void Em33_core_me_mv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Set_sabun(ENEMY_WORK *pEm, unsigned long Parts, unsigned long Ofs, short Rate);
static void Em33_Yure_ck(ENEMY_WORK *pEm);
static void Set_blood_ofs(ENEMY_WORK *pEm, unsigned char parts, unsigned char type, SVECTOR *pOfs);

#endif /* EM33_H */
