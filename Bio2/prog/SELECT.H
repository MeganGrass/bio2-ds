/*
 * SELECT.H - Scenario select screen work structures
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SELECT.H
 */
#ifndef SELECT_H
#define SELECT_H
#include "TYPES.H"
// D:\Bio2\prog\SELECT.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


/* ------------------------------------------------------------------ */
/* Work structures                                                    */
/* ------------------------------------------------------------------ */

/* Spot light, 16 bytes. Select_wk.Light[4] */
struct tagSELECT_LIGHT {
	LONG	Wait;		/* 0x00 state index into Select_light_proc_tbl */
	UCHAR	On;			/* 0x04 intensity multiplier */
	UCHAR	R;			/* 0x05 */
	UCHAR	G;			/* 0x06 */
	UCHAR	B;			/* 0x07 */
	SHORT	X;			/* 0x08 */
	SHORT	Y;			/* 0x0A */
	SHORT	Z;			/* 0x0C */
	USHORT	Range;		/* 0x0E */
};

/* Scenery sprite definition, 12 bytes. Select_sprt_def[12] */
struct tagSELECT_SPRT_DEF {
	SHORT	X;			/* 0x00 */
	SHORT	Y;			/* 0x02 */
	SHORT	W;			/* 0x04 */
	SHORT	H;			/* 0x06 */
	UCHAR	U;			/* 0x08 */
	UCHAR	V;			/* 0x09 */
	UCHAR	Abr;		/* 0x0A semi-transparency mode */
	UCHAR	Tp;			/* 0x0B texture page */
};

/* Scenario placement, 20 bytes. Select_pl_init[4] */
struct tagSELECT_PL_INIT {
	LONG	Pos_x;		/* 0x00 */
	LONG	Pos_y;		/* 0x04 */
	LONG	Pos_z;		/* 0x08 */
	SHORT	Cdir_x;		/* 0x0C */
	SHORT	Cdir_y;		/* 0x0E */
	SHORT	Cdir_z;		/* 0x10 */
	UCHAR	Id;			/* 0x12 character id */
	UCHAR	Pad13;
};

/* Name label glyph, 6 bytes. Select_name_uv[16] */
struct tagSELECT_NAME_UV {
	SHORT	W;			/* 0x00 */
	SHORT	H;			/* 0x02 */
	UCHAR	U;			/* 0x04 */
	UCHAR	V;			/* 0x05 */
};

/* Generic x/y pair, 4 bytes */
struct tagSELECT_XY {
	SHORT	X;
	SHORT	Y;
};

/* Message glyph record, 16 bytes. Rec[0] of SELECT_MOJI is the cursor,
 * Rec[1..140] are the text characters. Word0 = x (low) / y (high),
 * Word1 = w (low) / h (high), Word2 = u (low byte) / v / clut (high half). */
struct tagSELECT_MOJI_REC {
	ULONG	Word0;		/* 0x00 */
	ULONG	Word1;		/* 0x04 */
	ULONG	Word2;		/* 0x08 */
	UCHAR	Ot;			/* 0x0C ordering table slot */
	UCHAR	R;			/* 0x0D */
	UCHAR	G;			/* 0x0E */
	UCHAR	Vis;		/* 0x0F visible flag */
};

/* Message window work, 0x9F8 bytes. Pointed to by Select_wk.pMoji */
struct tagSELECT_MOJI {
	UCHAR	Col;					/* 0x000 next column, 0..19 */
	UCHAR	Row;					/* 0x001 next row */
	UCHAR	Pad002;
	UCHAR	Blink;					/* 0x003 cursor blink timer */
	UCHAR	Text[140];				/* 0x004 20 x 7 characters, 0x20 = blank */
	UCHAR	Attr[140];				/* 0x090 colour of each character */
	UCHAR	Wait;					/* 0x11C per-character delay counter */
	UCHAR	Color;					/* 0x11D current colour ('|' code) */
	UCHAR	Pad11e[2];
	UCHAR	*pStr;					/* 0x120 string being printed */
	UCHAR	*pCur;					/* 0x124 read position in pStr */
	SELECT_MOJI_REC Rec[141];		/* 0x128 */
};

/* Sprite slot, 0x40 bytes: one SPRT and one DR_MODE per draw buffer. Select_wk.Prim[30];
 * DS reaches the SPRT at slot * 0x40 + 4 + Cbuf_id * 0x14 and the DR_MODE at +0x28 + Cbuf_id * 0x0C */
typedef struct tagSELECT_PRIM SELECT_PRIM;
struct tagSELECT_PRIM {
	SPRT	Sp[2];		/* 0x00 */
	DR_MODE	Dr[2];		/* 0x28 */
};

/* Heap base used for the model/texture loads (DS 0x80100000, not a typed global) */
#define SELECT_HEAP_BASE	((ULONG *)0x80100000)

/* Overlay work block at DS 0x801C3B90 (0x7D0 bytes) */
struct tagSELECT_WORK {
	UCHAR	Main_no;		/* 0x000 DS 801C3B90 main state, index into Select_main_proc_tbl */
	UCHAR	Pad001;			/* 0x001 DS 801C3B91 cleared by Select_main, otherwise unused */
	UCHAR	Moji_no;		/* 0x002 DS 801C3B92 message script state */
	UCHAR	Pad003;			/* 0x003 DS 801C3B93 cleared by Select_main, otherwise unused */
	SELECT_PRIM Prim[30];	/* 0x004 DS 801C3B94 sprite slots (0..17 used: scenery, cursor, names) */
	signed char	Cursor_no;	/* 0x784 DS 801C4314 selected scenario 0..3, signed byte (lb) */
	UCHAR	Blink_cnt;		/* 0x785 DS 801C4315 cursor blink brightness */
	UCHAR	Blink_dir;		/* 0x786 DS 801C4316 cursor blink direction */
	UCHAR	Flg;			/* 0x787 DS 801C4317 bit0 = rotating, bit1/bit2 = cursor lock */
	UCHAR	*pHeap;			/* 0x788 DS 801C4318 bump pointer for loaded model data */
	SELECT_MOJI *pMoji;		/* 0x78C DS 801C431C message window work */
	SELECT_LIGHT Light[4];	/* 0x790 DS 801C4320 */
};

/* ------------------------------------------------------------------ */
/* Types missing from the shared headers                              */
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/* Functions                                                          */
/* ------------------------------------------------------------------ */

extern VOID Select_main(VOID);
extern VOID Select_init(VOID);
extern VOID Select_pl_hide(ENEMY_WORK *pEm);
extern VOID Select_view_init(VOID);
extern VOID Select_sprt_init(VOID);
extern VOID Select_pl_tim_load(ENEMY_WORK *pEm, ULONG *pWork);
extern ULONG *Select_pl_load(ENEMY_WORK *pEm, ULONG *pWork, UCHAR Tpage, UCHAR Clut);
extern VOID Select_mapping_tim(ULONG *pTim);
extern VOID Select_bg_load(VOID);
extern VOID Select_claire_set(ENEMY_WORK *pEm);
extern VOID Select_ada_set(ENEMY_WORK *pEm);
extern VOID Select_pl_set(ENEMY_WORK *pEm, ULONG No);
extern VOID Select_cursor_move(VOID);
extern VOID Select_decide(VOID);
extern VOID Select_sprt_disp(VOID);
extern VOID Select_name_disp(SELECT_WORK *pSel);
extern VOID Select_exit(VOID);
extern VOID Select_ada_disp(ENEMY_WORK *pEm);
extern VOID Select_ada_sort(ENEMY_WORK *pEm);
extern VOID Select_pl_sort(ENEMY_WORK *pEm);
extern VOID Select_light_ctrl(ENEMY_WORK *pEm);
extern VOID Select_light_wait(ENEMY_WORK *pEm, ULONG *pLight);
extern VOID Select_light_up(ENEMY_WORK *pEm, ULONG *pLight);
extern VOID Select_light_on(ENEMY_WORK *pEm, ULONG *pLight);
extern VOID Select_light_down(ENEMY_WORK *pEm, ULONG *pLight);
extern VOID Select_light_init(ENEMY_WORK *pEm);
extern VOID Select_light_calc(LONG *pPos, ENEMY_WORK *pEm);
extern VOID Select_light_set(ULONG No, VECTOR *pVec, UCHAR *pCol);
extern VOID Select_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos);
extern VOID Select_pl_main(VOID);
extern VOID Select_pl_move(VOID);
extern VOID Select_fade_in(VOID);
extern VOID Select_fade_out(VOID);
extern VOID Select_pl_dir_set(VOID);
extern VOID Select_pl_rot(VOID);
extern VOID Select_moji_set(ULONG *pDst, ULONG *pSrc);
extern VOID Select_moji_dm_set(VOID);
extern VOID Select_moji_init(VOID);
extern ULONG Select_moji_print(LONG Wait, UCHAR *pStr, ULONG *pEnd);
extern VOID Select_moji_ctrl(VOID);
extern VOID Select_moji_clear(VOID);
extern VOID Select_moji_update(LONG Disp);
extern VOID Select_moji_trans(LONG Num);

/* ------------------------------------------------------------------ */
/* Overlay data, DS addresses below                                   */
/* ------------------------------------------------------------------ */

/* Gap before Select_main (801BFA18-801BFAA7): CD message strings and
 * Cd_read message buffers. Ghidra does not define their contents.
 * Compiler jump tables at 801BFA3C (pl_move, Id 0), 801BFA54 (pl_move, Id 1),
 * 801BFA6C (pl_move, Id 0x0B) and 801BFA84 (moji_ctrl, 9 entries) are
 * replaced by C switch statements. */
extern UCHAR Select_mess_tim[];		// 801BFA1C		// Cd_read message, TIM load (contents not defined)
extern UCHAR Select_mess_pl[];		// 801BFA20		// Cd_read message, model load (contents not defined)
extern CHAR Select_str_bg[];		// 801BFA24		// "Select_BG"
extern CHAR Select_str_sel[];		// 801BFA30		// "Select"

extern SELECT_SPRT_DEF Select_sprt_def[12];	// 801C2AF0	// 144 bytes
extern SELECT_PL_INIT Select_pl_init[4];		// 801C2B80	// 80 bytes
extern ULONG Select_moji_tbl[141][4];			// 801C2BD0	// 2256 bytes, glyph template per character
extern UCHAR Select_str[4][7][30];				// 801C35E0	// 840 bytes, 7 message strings per scenario
extern ULONG Select_dir_L[4][4];				// 801C3A28	// 64 bytes, [scenario][pl] target L
extern SHORT Select_dir_fpos[4][4];				// 801C3A68	// 32 bytes, [scenario][pl] target Fpos_y
extern SELECT_NAME_UV Select_name_uv[16];		// 801C3A88	// 96 bytes
extern SELECT_XY Select_sprt_pos[6];			// 801C3AE8	// 24 bytes, name label positions
extern USHORT Select_cur_x[4][2];				// 801C3B00	// 16 bytes, cursor sprite x
extern UCHAR Select_name_idx[4][6];				// 801C3B10	// 24 bytes, name glyph index per scenario, 0xFF = none
extern USHORT Select_pl_cd[4];					// 801C3B28	// 8 bytes, CD file of each model
extern USHORT Select_tim_cd[4];					// 801C3B30	// 8 bytes, CD file of each texture
extern VOID *Select_main_proc_tbl[6];			// 801C3B38	// 24 bytes, main state routines
extern VOID *Select_light_proc_tbl[4];			// 801C3B50	// 16 bytes, light state routines
extern ULONG Select_moji_end;					// 801C3B60	// message print finished flag
extern ULONG Select_view_unk;					// 801C3B68	// zeroed by Select_view_init, not otherwise used
extern LONG Select_view_eye[3];					// 801C3B6C	// 12 bytes, Set_view position
extern LONG Select_view_ref[3];					// 801C3B7C	// 12 bytes, Set_view reference
extern SELECT_WORK Select_wk;					// 801C3B90	// 0x7D0 bytes
extern MATRIX Select_light_mtx[4];				// 801C3928	// 128 bytes, light direction matrix
extern MATRIX Select_light_col[4];				// 801C39A8	// 128 bytes, light colour matrix

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* SELECT_H_H */
