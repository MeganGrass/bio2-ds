/*
 * PL_WEP.H - Player weapon attack structs, tables and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PL_WEP.H
 */
#ifndef PL_WEP_H
#define PL_WEP_H
#include "TYPES.H"
#include "PLAYER.H"
#include "MAIN.H"
#include "PL_WEP2.H"

/* ---- types migrated from data.h ---- */
struct tagATTACK_WEAPON_EM {
	ULONG Damage;
	ULONG SubDamage;
	SHORT Jyo_ue;
	SHORT Jyo_ge;
	SHORT Tyu_ue;
	SHORT Tyu_ge;
	SHORT Ge_ue;
	SHORT Ge_ge;
};

void Pl_weapon_ch(ENEMY_WORK *pEm);
void Non_init(ENEMY_WORK *pEm);
void Plw02_init(ENEMY_WORK *pEm);
void Plw09_init(ENEMY_WORK *pEm);
void Plw0c_init(ENEMY_WORK *pEm);
void Plw0f_init(ENEMY_WORK *pEm);
void Plw12_init(ENEMY_WORK *pEm);
void Weapon_cls(ENEMY_WORK *pEm);
void Init_W_Tool(void);
void W_Tool(void);
void init_Weapon_at(ENEMY_WORK *pEm, ATTACK_WEAPON_TBL *At_tbl_top);
ULONG Weapon_at_ck(ENEMY_WORK *pEm, ULONG W_no, ATTACK_WEAPON_TBL *At_tbl_top);
long Set_MinMax(ATTACK_WEAPON_EM *pAwe, UCHAR Ck_flg, long *pMax, long *pMin);
ULONG Attack_r_ck0(MODEL_WORK *pOm, MODEL_WORK *pPm, ATTACK_WORK *pAt);
ULONG Attack_r_ck1(VECTOR *pH, short Cdir_y, VECTOR *pT, ATTACK_WORK *pAt);
ULONG Em_ck(ENEMY_WORK *pEm, ULONG LL);
void Hand_ck(ENEMY_WORK *pEm, long Ck_pos_y, short Type);
void Mag_set(ENEMY_WORK *pEm);
void Mag_down(ENEMY_WORK *pEm);
void G_rot_st(ENEMY_WORK *pEm);
void G_rot(ENEMY_WORK *pEm);
void Gat_lp_st(ENEMY_WORK *pEm);
void Gat_rot(ENEMY_WORK *pEm);
void Pl_water(ENEMY_WORK *pEm);
void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv05_w0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w0_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w0_t6(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w1_t0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w1_t1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w1_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);

extern UCHAR Pl_vib_tbl[18][6];	/* DS 0x800a6ed0, row (Type & 0xfff) - 2 */
/* partial-field access at byte offset 'off' (decompiler _N_M_ form) */
#define PL_AT(T, p, off)	(*(T *)((char *)(p) + (off)))
typedef void (*PL_WEP_FN)(ENEMY_WORK *pEm);
/* Tables already declared in GLOBAL.H (Plw_no, Pl_weapon_kyan, Gun_jtg_bit, W_at_ck,
   Ghand_len, Mw_ck, Pl_mag, G_rot_data_z, Pl_weapon_tbl) are not repeated here. */
extern UCHAR  Pl_weapon_at_base[];	/* DS 0x800a68e8, UCHAR base: Type*0x18 + Gun_jtg_bit[..]*8 */
extern PL_WEP_FN Pl_weapon_init[20];	/* DS 0x800a6d28, index Type & 0xfff */
extern PL_WEP_FN pl_mv05_w_tbl[19];	/* DS 0x800a6f90, index Type & 0xfff (entry 0 is the word 0x00000002, not a routine) */
extern PL_WEP_FN pl_mv05_t2_init[];	/* DS 0x800a6f94 base, index (Type & 0xfff) + 0x12 (w0_t2 / w1_t2) */
extern char   s_WEP_DATA_80010fe4[];	/* DS 0x80010fe4, Cd_read file name string */

/* Literal words copied into local structs: 0x80011010.. in pl_mv05_w0_t2 / pl_mv05_w1_t2,
   0x80011024.. in pl_mv05_w1_t1 */
extern ULONG  Pl_mvtbl0_w0;	/* DS 0x80011010 */
extern ULONG  Pl_mvtbl0_w1;	/* DS 0x80011014 */
extern USHORT Pl_mvtbl0_w2;	/* DS 0x80011018 */
extern ULONG  Pl_mvtbl1_w0;	/* DS 0x8001101c */
extern ULONG  Pl_mvtbl1_w1;	/* DS 0x80011020 */
extern ULONG  Pl_mvtbl2_w0;	/* DS 0x80011024 */
extern ULONG  Pl_mvtbl2_w1;	/* DS 0x80011028 */
extern USHORT Pl_mvtbl2_w2;	/* DS 0x8001102c */
void Pl_view_esp_set(ENEMY_WORK *pEm, PARTS_W *pParts);	/* PLAYER.C 80040350 */
void Pad_vib_set0(int Time, USHORT Attr);	/* PAD.C 8003947c */
void Pad_vib_set1(int Time, UCHAR Power, USHORT Attr);	/* PAD.C 80039514 */
void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);	/* PAD.C 800395b8 */
void Pl_weapon_vib(ENEMY_WORK *pEm);



/* Globals defined in PL_WEP.C */
	extern ATTACK_WORK3 Pl_w010[5];						// 0x800a64e0
	extern USHORT Plw_no[16];							// 0x800a6d08	// PLxxWxx_PLW names (FILECODE.H), by enemy Id
	extern UCHAR W_at_ck[3][9];							// 0x800a6db4
	extern SHORT Ghand_len[19][4];						// 0x800a6dd0
	extern CHAR Mw_ck[16];								// 0x800a6e68
	extern PL_MAG_TBL Pl_mag[4];						// 0x800a6e90
	extern SHORT G_rot_data_z[20];						// 0x800a6ea8
#endif /* PL_WEP_H_H */
