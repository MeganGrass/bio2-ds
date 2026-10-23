/* EM37.H - types used only by the EM37 overlay (retail layout). Shared types are in the project headers (MAIN.H, EM_SUB.H, PLAYER.H). */
#ifndef EM37_H
#define EM37_H

#include "TYPES.H"
#include "MAIN.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "SPL.H"

/* The project ENEMY_WORK places Free[] 4 bytes before the retail offset (+0x218) the overlays use. */
#define ENEMY_FREE(p)	((ULONG *)((UCHAR *)(p)->Free + 4))

/* EM_SUB.H declares Direction_ck with long parameters and Rnd, Cdir_ck with UCHAR results; this overlay calls them as the retail code does. */
#define Direction_ck	((SHORT (*)(SHORT, SHORT, SHORT, SHORT))Direction_ck)

#ifdef OVL_38
#define EM_MAIN		Em237
#define PLDAMEGE_EM	Pldamege_Em237
#define PLDIE_EM	Pldie_Em237
#else
#define EM_MAIN		Em137
#define PLDAMEGE_EM	Pldamege_Em137
#define PLDIE_EM	Pldie_Em137
#endif

/* ---- functions of EM37.C ---- */
/* retail-only resident Dual Shock vibration helpers (config/retail/main.symbols.txt Pad_vib_set0
 * 0x8003947C / Pad_vib_set1 0x80039514 / Pad_vib_fade_set 0x800395B8; references/bio2-ds/Bio2/prog/PLAYER.H:266-268). */
extern void Pad_vib_set0();
extern void Pad_vib_set1();
extern void Pad_vib_fade_set();

/* ---- functions of other translation units ----
 * Unlike EM28.C/EM3F.C, the retail SYM of EM37.C shows no record-less blocks
 * where these routines are first called inside nested statements, so they had
 * declarations in scope at file level.  Signatures are those of the defining
 * TUs' retail SYM records (libgte routines: PsyQ LIBGTE.H).
 * The exceptions, called through implicit declarations, are the routines whose
 * first call opens a record-less retail block: Pl_life_down() and Cdir_ck()
 * (Em37_mv03), Snd_se_enem() (Em37_mv08), Snd_se_stad() and Add_speedXZ()
 * (Em37_pldm01) and CompM() (Em37_trans).  Kage_work_set() is only called at
 * function-body level (as in EM3F.C, through an implicit declaration); for
 * Jh_sort(), MulJh(), Oba_ck_hit() and PartsWork_set() the SYM gives no
 * evidence either way (body-level calls in blocks that exist anyway). */
extern unsigned long Joint_move(ENEMY_WORK *pEm, unsigned long Kan_t_ptr, unsigned long Seq_t_ptr, unsigned long Late_flg);
extern unsigned char Rnd(void);
extern void Goto00(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);
extern unsigned long Jh_set(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh, unsigned long Mem_top);
extern void Jh_sort(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
extern void MulJh(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh);
extern unsigned long Oba_ck_hit(ENEMY_WORK *pEm1, ENEMY_WORK *pEm2);
extern unsigned long PartsWork_set(ENEMY_WORK *pEm, unsigned long work_top);

extern void Snd_se_enem(UCHAR SENo, ENEMY_WORK *pEm);
extern ULONG Pl_life_down(long Down_life, ULONG Mode);
extern UCHAR Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT);

void EM_MAIN(ENEMY_WORK *pEm);
static void Em37_after(ENEMY_WORK *pEm);
static void Em37_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv00_br(ENEMY_WORK *pEm);
static void Em37_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv01_br(ENEMY_WORK *pEm);
static void Em37_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv02_br(ENEMY_WORK *pEm);
static void Em37_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv03_br(ENEMY_WORK *pEm);
static void Em37_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv04_br(ENEMY_WORK *pEm);
static void Em37_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv05_br(ENEMY_WORK *pEm);
static void Em37_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv06_br(ENEMY_WORK *pEm);
static void Em37_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv07_br(ENEMY_WORK *pEm);
static void Em37_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv08_br(ENEMY_WORK *pEm);
static void Em37_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv09_br(ENEMY_WORK *pEm);
static void Em37_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv0a_br(ENEMY_WORK *pEm);
static void Em37_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv0b_br(ENEMY_WORK *pEm);
static void Em37_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv0c_br(ENEMY_WORK *pEm);
static void Em37_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv0d_br(ENEMY_WORK *pEm);
static void Em37_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_mv0e_br(ENEMY_WORK *pEm);
static void Em37_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_8010404C(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void func_80104054(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void Em37_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_dm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_dm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_801046AC(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void func_801048F4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em37_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em37_move(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static unsigned long G5s_Jh_init(ENEMY_WORK *pEm, unsigned long Mem_top);
static void G5s_Jh_move(ENEMY_WORK *pEm);
static void Goto00_x(ENEMY_WORK *pEm, long Vec_y, long Vec_z, short Add_dir);
static void Em37_ob_at(ENEMY_WORK *pEm);
static void Em37_ob_at1(ENEMY_WORK *pEm);
static ENEMY_WORK *EM37_Set_enemy_work(unsigned char Id, ENEMY_WORK *pMoto);
static void Em37_hari_quake(ENEMY_WORK *pEm);
static void func_80105560(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */

#endif /* EM37_H */
