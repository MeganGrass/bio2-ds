#ifndef SCE2_H
#define SCE2_H
#include "TYPES.H"


/* SCE2.C - SCD opcode implementations (effect/enemy/audio/light/misc)
 * DS retail range 0x80056428 .. 0x80059437 */

/* --------------------------------------------------------------------------
 *	Globals owned by SCE2.C
 * ------------------------------------------------------------------------ */
/* DS 0x800a7ab8 */ extern UCHAR            Itp_check_sum[100];
/* DS 0x800d5be8 */ extern SCE_EM_SET_WORK *pSce_em;

/* --------------------------------------------------------------------------
 *	External retail-only functions referenced by SCE2.C
 * ------------------------------------------------------------------------ */
void Esp_kill_all(void);											/* ESP.C 8001d07c */
void Pad_vib_set0(int Time, USHORT Attr);							/* PAD.C 8003947c */
void Pad_vib_set1(int Time, UCHAR Power, USHORT Attr);				/* PAD.C 80039514 */
void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);	/* PAD.C 800395b8 */

/* --------------------------------------------------------------------------
 *	Prototypes (DS address order)
 *	Opcode handlers for effects (ESP and 3D sprites), player and enemy control, sound, lights, items,
 *	screen effects, and vibration. The trailing addresses are the retail SLUS-00748 locations.
 * ------------------------------------------------------------------------ */
/* --------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */
ULONG   Se_on(SCE_TASK *pSce);				/* 80056428 */
ULONG   Sca_id_set(SCE_TASK *pSce);			/* 8005655c */
ULONG   Sce_espr_on(SCE_TASK *pSce);		/* 800565a4 */
ULONG   Sce_espr_on2(SCE_TASK *pSce);		/* 80056644 */
ULONG   Sce_espr3d_on(SCE_TASK *pSce);		/* 800566f0 */
ULONG   Sce_espr3d_on2(SCE_TASK *pSce);		/* 800567b4 */
ULONG   Sce_espr_kill(SCE_TASK *pSce);		/* 80056884 */
ULONG   Sce_espr_kill2(SCE_TASK *pSce);		/* 800568e8 */
ULONG   Sce_espr_task(SCE_TASK *pSce);		/* 80056930 */
ULONG   Sce_espr_control(SCE_TASK *pSce);	/* 800569d4 */
MATRIX *Get_matrix(long Work_kind, long Work_no);	/* 80056a38 */
ULONG   Plc_motion(SCE_TASK *pSce);			/* 80056b8c */
ULONG   Plc_dest(SCE_TASK *pSce);			/* 80056bd8 */
ULONG   Plc_gun(SCE_TASK *pSce);			/* 80056dc0 */
ULONG   Plc_gun_eff(SCE_TASK *pSce);		/* 80056df8 */
ULONG   Plc_neck(SCE_TASK *pSce);			/* 80056e5c */
ULONG   Plc_ret(SCE_TASK *pSce);			/* 80056f9c */
ULONG   Plc_stop(SCE_TASK *pSce);			/* 80056ff0 */
ULONG   Plc_flg(SCE_TASK *pSce);			/* 80057038 */
ULONG   Plc_rot(SCE_TASK *pSce);			/* 800570bc */
ULONG   Plc_cnt(SCE_TASK *pSce);			/* 80057120 */
ULONG   Sce_em_set(SCE_TASK *pSce);			/* 8005714c */
ULONG   Sce_em_set2(SCE_TASK *pSce);		/* 80057714 */
ULONG   Sce_key_ck(SCE_TASK *pSce);			/* 80057da8 */
ULONG   Sce_trg_ck(SCE_TASK *pSce);			/* 80057de4 */
ULONG   Sce_bgm_control(SCE_TASK *pSce);	/* 80057e20 */
ULONG   Sce_bgmtbl_set(SCE_TASK *pSce);		/* 80057e98 */
ULONG   Sce_fade_set(SCE_TASK *pSce);		/* 80057ef0 */
ULONG   Sce_fade_adjust(SCE_TASK *pSce);	/* 80057fd8 */
ULONG   Xa_on(SCE_TASK *pSce);				/* 80058028 */
ULONG   Xa_vol(SCE_TASK *pSce);				/* 80058090 */
ULONG   Se_vol(SCE_TASK *pSce);				/* 800580cc */
ULONG   Sce_shake_on(SCE_TASK *pSce);		/* 80058164 */
ULONG   Sce_line_start(SCE_TASK *pSce);		/* 80058214 */
ULONG   Sce_line_main(SCE_TASK *pSce);		/* 80058260 */
ULONG   Sce_line_end(SCE_TASK *pSce);		/* 800582b8 */
ULONG   Mizu_div_set(SCE_TASK *pSce);		/* 800582f0 */
ULONG   Keep_Item_ck(SCE_TASK *pSce);		/* 800584f0 */
ULONG   Sce_Item_cmp(SCE_TASK *pSce);		/* 80058528 */
ULONG   Sce_Item_lost(SCE_TASK *pSce);		/* 800585e4 */
ULONG   Sce_Item_lost2(SCE_TASK *pSce);		/* 80058660 */
ULONG   Weapon_chg(SCE_TASK *pSce);			/* 80058704 */
ULONG   Sce_Item_get(SCE_TASK *pSce);		/* 800587b8 */
ULONG   Kage_set(SCE_TASK *pSce);			/* 80058898 */
ULONG   Light_pos_set(SCE_TASK *pSce);		/* 800589d4 */
ULONG   Light_kido_set(SCE_TASK *pSce);		/* 80058a8c */
ULONG   Light_color_set(SCE_TASK *pSce);	/* 80058ad8 */
ULONG   Light_pos_set2(SCE_TASK *pSce);		/* 80058b40 */
ULONG   Light_kido_set2(SCE_TASK *pSce);	/* 80058bfc */
ULONG   Light_color_set2(SCE_TASK *pSce);	/* 80058c4c */
ULONG   Sce_scr_move(SCE_TASK *pSce);		/* 80058cb8 */
ULONG   Flr_set(SCE_TASK *pSce);			/* 80058ce4 */
ULONG   Movie_on(SCE_TASK *pSce);			/* 80058d64 */
ULONG   Splc_ret(SCE_TASK *pSce);			/* 80058dac */
ULONG   Splc_sce(SCE_TASK *pSce);			/* 80058e48 */
ULONG   Evt_next2(SCE_TASK *pSce);			/* 80058eb0 */
ULONG   Mirror_set(SCE_TASK *pSce);			/* 80058ef8 */
ULONG   Sce_parts_bomb(SCE_TASK *pSce);		/* 80058f78 */
ULONG   Sce_parts_down(SCE_TASK *pSce);		/* 80059030 */
ULONG   Col_chg_set(SCE_TASK *pSce);		/* 800590e0 */
ULONG   Plc_heal(SCE_TASK *pSce);			/* 8005918c */
ULONG   St_map_hint(SCE_TASK *pSce);		/* 800591c4 */
ULONG   Sce_em_pos_ck(SCE_TASK *pSce);		/* 8005922c */
ULONG   Poison_ck(SCE_TASK *pSce);			/* 800592f8 */
ULONG   Poison_clr(SCE_TASK *pSce);			/* 80059318 */
ULONG   Vib_set0(SCE_TASK *pSce);			/* 80059348 */
ULONG   Vib_set1(SCE_TASK *pSce);			/* 80059394 */
ULONG   Vib_fade_set(SCE_TASK *pSce);		/* 800593e4 */


#endif /* SCE2_H_H */
