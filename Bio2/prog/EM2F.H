/* EM2F.H - types used only by the EM2F overlay (retail layout). Shared types are in the project headers. */
#ifndef EM2F_H
#define EM2F_H

#include "TYPES.H"
#include "MAIN.H"
#include "SPL.H"

/* ENEMY_WORK.Free[] starts 4 bytes later in the retail structure than in EM_SUB.H (two halfwords precede it), so the overlay addresses it through this offset. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares Direction_ck with long parameters; the retail call passes and returns 16-bit values. */
#define Direction_ck	((SHORT (*)(SHORT, SHORT, SHORT, SHORT))Direction_ck)

/* Retail-only copy of EM2F_FREE with the unnamed Free+0x18 byte (Retail_18, see retail em2f.h). */
typedef struct {
    unsigned char Hand_no;
    unsigned char Att;
    short Set_fx;
    short Set_fy;
    short Set_fz;
    short Set_dir;
    unsigned char No_atck;
    unsigned char Act_type;
    short Base_dir;
    short Set_dirz;
    unsigned char On_flg;
    unsigned char Hit_flg;
    unsigned char Col_flg;
    unsigned char Sick_flg;
    unsigned char Set60A;
    unsigned char Rno2_stk;
    unsigned char Rno3_stk;
    unsigned char Em_keep;
    unsigned char Retail_18;
} EM2F_FREE_R;

#ifdef OVL_32
#define EM_MAIN		Em22f
#else
#define EM_MAIN		Em12f
#endif

/* ---- functions of EM2F.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em2f_after(ENEMY_WORK *pEm);
static void Em2f_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Mv_dummy(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Mv_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_atck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void move_atck00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void move_atck10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void move_atck20(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void move_atck20_0(ENEMY_WORK *pEm);
static void move_atck30(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_neck(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_dmg(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_color(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_ivyfall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_gas(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_move_hole(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
/* Declared without prototypes: Em2f_move_hole calls Em2f_move_hole00 with no
 * arguments and Em2f_move_hole10 with pEm only (retail sets no argument
 * registers / only $a0); the callees read the caller's leftover $a1/$a2. */
static void Em2f_move_hole00();
static void Em2f_move_hole10();
static void Em2f_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_down(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_omake(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_omake00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_omake10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_oki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_dmg_H2SO4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_80101894(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void Em2f_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Die_nml(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Die_fire(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2f_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static PARTS_W *Get_parts_pos(ENEMY_WORK *pTg, SVECTOR *pSv, unsigned long no);
static void On_Ivyparts(ENEMY_WORK *pEm, unsigned long mode);
static long hit_ck_main(ENEMY_WORK *pEm, long pl, short dmg);
static long Atck_hit_ck(ENEMY_WORK *pEm, short dmg);
static void Em2f_Pl_to_dmg(PLAYER_WORK *pPl, unsigned char dir);
static void Em2f_Pl_to_die(PLAYER_WORK *pPl);
static void Set_req(ENEMY_WORK *pEm, unsigned char req);
static void Pcolor_dec(ENEMY_WORK *pEm);
static void Set_blood(ENEMY_WORK *pEm, unsigned char parts, unsigned char type);
static unsigned long Iv_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top);
static void Iv_Jh_move(ENEMY_WORK *pEm);

extern void Em_seq_sound(ENEMY_WORK *pEm);
extern void Em_dead_flg_on(ENEMY_WORK *pEm);

extern unsigned long Esp_call();
extern unsigned long Jh_set();
extern void MulJh();
extern void Aida_Jh_sort();
extern long Goto00_ck(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);
extern unsigned char Rnd(void);
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);
extern void Sca_ck_em(ENEMY_WORK *pEm, unsigned long Sc_id);
extern void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos);
extern void Oba_ck_em(ENEMY_WORK *pEm);
/* retail-only resident Dual Shock vibration helpers: Pad_vib_set0 0x8003947C, Pad_vib_set1 0x80039514
   (config/retail/main.symbols.txt; references/bio2-ds/Bio2/prog/PAD.C:309,325); old-style declarations. */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Flg_on();	/* retail-only call (Em2f_Die_nml); old-style declaration */

#endif /* EM2F_H */
