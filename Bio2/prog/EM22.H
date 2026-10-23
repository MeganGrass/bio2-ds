/* EM22.H - types used only by the EM22 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM22_H
#define EM22_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"

/* This overlay calls Rnd() as a plain int-returning function (no andi after the jal), while EM_SUB.H declares it UCHAR;
 * calling it through a LONG-returning type reproduces the call without changing the project prototype (see Rnd_s32 in ESP_R.C). */
#define Rnd()	(((LONG (*)(VOID))Rnd)())

/* The overlay calls these project functions without a prototype (implicit int declarations, arguments passed unnarrowed),
 * so each name is called through a K&R function type; the prototypes in the project headers stay untouched. */
#define Cdir_ck	(*(int (*)())Cdir_ck)
#define Goto01	(*(int (*)())Goto01)
#define Rot_vector	(*(int (*)())Rot_vector)
#define Sca_get_dir	(*(int (*)())Sca_get_dir)

#ifdef OVL_26
#define EM_MAIN		Em222
#define EM_PLDM		Em222_Pldm
#else
#define EM_MAIN		Em122
#define EM_PLDM		Em122_Pldm
#endif

/* ---- functions of EM22.C ---- */
/* retail-only pad vibration helpers (PAD.C), as named in config/retail/main.symbols.txt;
 * prototypes from references/bio2-ds/Bio2/prog/PAD.H:33,35 (Pad_vib_set0 @0x8003947C,
 * Pad_vib_fade_set @0x800395B8) */
extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);
extern unsigned long Flg_ck(unsigned long *p, unsigned long x);	/* retail: used by Em22_Init */

void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em22_damage_ck(ENEMY_WORK *pEm);
static void Em22_die_ck(ENEMY_WORK *pEm);
static void Em22_dm_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em22_dm_fall_v(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_80101790(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void func_80102C40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void func_80104204(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void func_80106764(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void func_80106FC4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static long Get_dir_pl(ENEMY_WORK *pEm);	/* Beta 2 EM2B_SUB.C Get_dir_pl, also linked here in retail */
/* retail links EM2B_SUB.C's Set_esp_w / Set_esp_s / Get_dir_pl trio at the end of this overlay
 * (0x80107074 / 0x80107150 / 0x801071B0 in s1); defined at the end of this file in that order. */
static void Set_esp_w(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec);
static void Set_esp_s(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec);

/* ---- functions of EM22_SUB.C ---- */
static void Dm_eff_spark(ENEMY_WORK *pEm);
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b);

#endif /* EM22_H */
