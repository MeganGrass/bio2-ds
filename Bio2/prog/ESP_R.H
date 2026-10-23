#ifndef ESP_R_H
#define ESP_R_H
/*
 * ESP_R.H - Effect Sprite routines, per-state callbacks (Esp_00..Esp_5f)
 * D:\Bio2\prog\ESP_R.H
 */


#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagATTACK_WORK {
	SHORT Off_x;
	SHORT Off_z;
	SHORT R_x;
	SHORT Half_R_z;
};

/* Dispatch through the routine table: Esp_tblj[idx]() */
#define ESP_TBLJ_CALL(idx)	(((void (*)(void))Esp_tblj[(idx)])())

/* Ground height cached in the 4-byte word at ESP_WORK+0x14 (Ghidra pESP[5]). */
#define ESP_GROUND_Y(p)		(*(LONG *)&(p)->Free4)

/* Random value. DS Rnd (0x80015fe8) masks its result to 0..255 (andi v0,v0,0xff), so it is declared UCHAR.
 * Some DS callers (Esp_0e, Esp_30, Esp_45) divide it with signed multiply-high sequences; those call it
 * through Rnd_s32() in ESP_R.C. A global LONG declaration was tested and made 17 other functions worse. */
extern UCHAR Rnd(VOID);

/* Ground / water / collision helpers (declared in their owner headers). */

/* Hit-box constants read from ROM data (values not
 * readable through ghidramcp, so they stay external). */
extern const ATTACK_WORK Esp_aw_0e;	/* DS 0x800108d8 (Esp_0e) */
extern const ATTACK_WORK Esp_aw_0f;	/* DS 0x80010900 (Esp_0f) */
extern const ATTACK_WORK Esp_aw_18;	/* DS 0x80010908 (Esp_18) */
extern const ATTACK_WORK Esp_aw_28;	/* DS 0x80010910 (Esp_28) */
extern const ATTACK_WORK Esp_aw_2f[2];	/* DS 0x80010918 (Esp_2f); Esp_2f copies it to a stack array that Pos_em_at_ck updates */
extern const ATTACK_WORK Esp_aw_30[2];	/* DS 0x8001093c (Esp_30, Esp_31) */
extern const ATTACK_WORK Esp_aw_46;	/* DS 0x80010964 (Esp_46) */

/* OBA.C / SUB.H (retail 0x8003513c, Flg_ck) */
extern ULONG Oba_ck_em_at(SVECTOR *pPos, LONG R, LONG H);
extern ULONG Flg_ck(ULONG *p, ULONG x);

/* Functions defined in PAD.C */
extern VOID Pad_vib_set0(int Time, USHORT Attr);				/* DS 0x8003947c */
extern VOID Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);	/* DS 0x800395b8 */

/* ESP_R.C routines */
extern VOID Esp_00(VOID);
extern VOID Esp_01(VOID);
extern VOID Esp_02(VOID);
extern VOID Esp_03(VOID);
extern VOID Esp_04(VOID);
extern VOID Esp_05(VOID);
extern VOID Esp_06(VOID);
extern VOID Esp_07(VOID);
extern VOID Esp_08(VOID);
extern VOID Esp_09(VOID);
extern VOID Esp_0a(VOID);
extern VOID Esp_0b(VOID);
extern VOID Esp_0c(VOID);
extern VOID Esp_0d(VOID);
extern VOID Esp_0e(VOID);
extern VOID Esp_0f(VOID);
extern VOID Esp_10(VOID);
extern VOID Esp_11(VOID);
extern VOID Esp_12(VOID);
extern VOID Esp_13(VOID);
extern VOID Esp_14(VOID);
extern VOID Esp_15(VOID);
extern VOID Esp_16(VOID);
extern VOID Esp_17(VOID);
extern VOID Esp_18(VOID);
extern VOID Esp_19(VOID);
extern VOID Esp_1a(VOID);
extern VOID Esp_1b(VOID);
extern VOID Esp_1c(VOID);
extern VOID Esp_1d(VOID);
extern VOID Esp_1e(VOID);
extern VOID Esp_1f(VOID);
extern VOID Esp_20(VOID);
extern VOID Esp_21(VOID);
extern VOID Esp_22(VOID);
extern VOID Esp_23(VOID);
extern VOID Esp_24(VOID);
extern VOID Esp_25(VOID);
extern VOID Esp_26(VOID);
extern VOID Esp_27(VOID);
extern VOID Esp_28(VOID);
extern VOID Esp_29(VOID);
extern VOID Esp_2a(VOID);
extern VOID Esp_2b(VOID);
extern VOID Esp_2c(VOID);
extern VOID Esp_2d(VOID);
extern VOID Esp_2e(VOID);
extern VOID Esp_2f(VOID);
extern VOID Esp_30(VOID);
extern VOID Esp_31(VOID);
extern VOID Esp_32(VOID);
extern VOID Esp_33(VOID);
extern VOID Esp_34(VOID);
extern VOID Esp_35(VOID);
extern VOID Esp_36(VOID);
extern VOID Esp_37(VOID);
extern VOID Esp_38(VOID);
extern VOID Esp_39(VOID);
extern VOID Esp_3a(VOID);
extern VOID Esp_3b(VOID);
extern VOID Esp_3c(VOID);
extern VOID Esp_3d(VOID);
extern VOID Esp_3e(VOID);
extern VOID Esp_3f(VOID);
extern VOID Esp_40(VOID);
extern VOID Esp_41(VOID);
extern VOID Esp_42(VOID);
extern VOID Esp_43(VOID);
extern VOID Esp_44(VOID);
extern VOID Esp_45(VOID);
extern VOID Esp_46(VOID);
extern VOID Esp_47(VOID);
extern VOID Esp_48(VOID);
extern VOID Esp_49(VOID);
extern VOID Esp_4a(VOID);
extern VOID Esp_4b(VOID);
extern VOID Esp_4c(VOID);
extern VOID Esp_4d(VOID);
extern VOID Esp_4e(VOID);
extern VOID Esp_4f(VOID);
extern VOID Esp_50(VOID);
extern VOID Esp_51(VOID);
extern VOID Esp_52(VOID);
extern VOID Esp_53(VOID);
extern VOID Esp_54(VOID);
extern VOID Esp_55(VOID);
extern VOID Esp_56(VOID);
extern VOID Esp_57(VOID);
extern VOID Esp_58(VOID);
extern VOID Esp_59(VOID);
extern VOID Esp_5a(VOID);
extern VOID Esp_5b(VOID);
extern VOID Esp_5c(VOID);
extern VOID Esp_5d(VOID);
extern VOID Esp_5e(VOID);
extern VOID Esp_5f(VOID);


#endif /* ESP_R_H_H */
