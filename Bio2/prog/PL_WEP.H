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
	ULONG Damage;		/* three 10-bit damage values at bit n * 10; value n applies when attack box n of the weapon (ATTACK_WORK3 Off0_x..Off2_x, box 0 nearest) hit */
	ULONG SubDamage;	/* three 3-bit part damage values at bit n * 3, and three 7-bit Damage_cnt values at bit 9 + n * 7; n is the attack box as for Damage */
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
extern PL_WEP_FN Pl_weapon_init[20];	/* DS 0x800a6d28, index Type & 0xfff */
extern PL_WEP_FN pl_mv05_t2_init[19];	/* DS 0x800a6f94, index (Type & 0xfff) - 1 */
extern PL_WEP_FN pl_mv05_t2_spr[19];	/* DS 0x800a6fe0, index (Type & 0xfff) - 1 */
extern char   s_WEP_DATA_80010fe4[];	/* DS 0x80010fe4, Cd_read file name string */

void Pl_view_esp_set(ENEMY_WORK *pEm, PARTS_W *pParts);	/* PLAYER.C 80040350 */
void Pad_vib_set0(int Time, USHORT Attr);	/* PAD.C 8003947c */
void Pad_vib_set1(int Time, UCHAR Power, USHORT Attr);	/* PAD.C 80039514 */
void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);	/* PAD.C 800395b8 */
void Pl_weapon_vib(ENEMY_WORK *pEm);



/* Globals defined in PL_WEP.C */
	extern ATTACK_WEAPON_EM Em10_damage_tbl[19];			// 0x800a412c
	extern ATTACK_WEAPON_EM Em15_damage_tbl[19];			// 0x800a42a8
	extern ATTACK_WEAPON_EM Em20_damage_tbl[19];			// 0x800a4424
	extern ATTACK_WEAPON_EM Em21_damage_tbl[19];			// 0x800a45a0
	extern ATTACK_WEAPON_EM Em22_damage_tbl[19];			// 0x800a471c
	extern ATTACK_WEAPON_EM Em24_damage_tbl[19];			// 0x800a4898
	extern ATTACK_WEAPON_EM Em23_damage_tbl[19];			// 0x800a4a14
	extern ATTACK_WEAPON_EM Em25_damage_tbl[19];			// 0x800a4b90
	extern ATTACK_WEAPON_EM Em27_damage_tbl[19];			// 0x800a4d0c
	extern ATTACK_WEAPON_EM Em28_damage_tbl[19];			// 0x800a4e88
	extern ATTACK_WEAPON_EM Em29_damage_tbl[19];			// 0x800a5004
	extern ATTACK_WEAPON_EM Em2d_damage_tbl[19];			// 0x800a5180
	extern ATTACK_WEAPON_EM Em2a_damage_tbl[19];			// 0x800a52fc
	extern ATTACK_WEAPON_EM Em2b_damage_tbl[19];			// 0x800a5478
	extern ATTACK_WEAPON_EM Em2e_damage_tbl[19];			// 0x800a55f4
	extern ATTACK_WEAPON_EM Em30_damage_tbl[19];			// 0x800a5770
	extern ATTACK_WEAPON_EM Em31_damage_tbl[19];			// 0x800a58ec
	extern ATTACK_WEAPON_EM Em33_damage_tbl[19];			// 0x800a5a68
	extern ATTACK_WEAPON_EM Em34_damage_tbl0[19];			// 0x800a5be4
	extern ATTACK_WEAPON_EM Em34_damage_tbl1[19];			// 0x800a5d60
	extern ATTACK_WEAPON_EM Em36_damage_tbl[19];			// 0x800a5edc
	extern ATTACK_WEAPON_EM Em3a_damage_tbl[19];			// 0x800a6058
	extern ATTACK_WEAPON_EM Em3b_damage_tbl[19];			// 0x800a61d4
	extern ATTACK_WORK3 PLW00_W[3];						// 0x800a6350
	extern ATTACK_SEQ PLW00_S[2];						// 0x800a63a4
	extern ATTACK_WORK3 Pl_w0111[5];					// 0x800a6444
	extern ATTACK_SEQ Pl_w0111_s1[7];					// 0x800a64d0
	extern ATTACK_WORK3 PLW02_W[3];						// 0x800a6618
	extern ATTACK_SEQ PLW02_S[2];						// 0x800a666c
	extern ATTACK_WORK3 PLW05_W[3];						// 0x800a6670
	extern ATTACK_SEQ PLW05_S[2];						// 0x800a66c4
	extern ATTACK_WORK3 PLW13_W[3];						// 0x800a66c8
	extern ATTACK_SEQ PLW13_S[4];						// 0x800a671c
	extern ATTACK_WORK3 PLW07_W[3];						// 0x800a6724
	extern ATTACK_SEQ PLW07_S[7];						// 0x800a6778
	extern ATTACK_WORK3 PLW08_W[3];						// 0x800a6788
	extern ATTACK_SEQ PLW08_S[9];						// 0x800a67dc
	extern ATTACK_WORK3 PLW0E_W[3];						// 0x800a67f0
	extern ATTACK_SEQ PLW0E_S[2];						// 0x800a6844
	extern ATTACK_WORK3 PLW0F_W[3];						// 0x800a6848
	extern ATTACK_SEQ PLW0F_S[3];						// 0x800a689c
	extern ATTACK_WORK3 PLW12_W[3];						// 0x800a68a4
	extern ATTACK_SEQ PLW12_S[3];						// 0x800a68f8
	extern ATTACK_WEAPON_TBL Pl_weapon_tbl[19][3];		// 0x800a6900	// index (Type & 0xfff) - 1, then Gun_jtg_bit[..]
	extern ATTACK_WEAPON_EM *pEnemy_damage_tbl[48];		// 0x800a6ac8	// index Id - 0x10
	extern ATTACK_WORK3 Pl_w010[5];						// 0x800a64e0
	extern USHORT Plw_no[16];							// 0x800a6d08	// PLxxWxx_PLW names (FILECODE.H), by enemy Id
	extern UCHAR W_at_ck[3][9];							// 0x800a6db4
	extern SHORT Ghand_len[19][4];						// 0x800a6dd0
	extern CHAR Mw_ck[16];								// 0x800a6e68
	extern UCHAR Pl_wep_mag_type[21];					// 0x800a6e78	// magazine kind per weapon, index (Type & 0xfff) - 1
	extern PL_MAG_TBL Pl_mag[4];						// 0x800a6e90
	extern SHORT G_rot_data_z[20];						// 0x800a6ea8
#endif /* PL_WEP_H_H */
