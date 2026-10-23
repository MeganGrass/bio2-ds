/*
 * OPENING.H - Opening sequence sprite and fade structures
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OPENING.H
 */
#ifndef OPENING_H
#define OPENING_H
#include "TYPES.H"
// D:\Bio2\prog\OPENING.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


// Sprite definition (10 bytes) - text / picture tiles of the opening TIMs
struct OP_MOJI_TBL {
	USHORT tpage;				// 00000000
	USHORT clut;				// 00000002
	UCHAR u;					// 00000004
	UCHAR v;					// 00000005
	UCHAR uw;					// 00000006
	UCHAR vh;					// 00000007
	UCHAR w;					// 00000008
	UCHAR h;					// 00000009
};

// Sprite work entry (12 bytes) - one per displayed sprite of a scene
struct OP_WORK {
	UCHAR flg;					// 00000000	bit0 disp, bit2 fade in, bit3 fade out, bit4 scroll up, bit5 zoom out, bit6 zoom in
	UCHAR type;					// 00000001	hi nibble 0 moji, 1 picture, 2 zoom moji; bit4 set = always cleared
	UCHAR ot;					// 00000002	OT priority
	UCHAR no;					// 00000003	index into Op_moji_tbl / Op_pict_tbl
	SHORT x;					// 00000004
	SHORT y;					// 00000006
	LONG rgb;					// 00000008	r,g,b,0 packed
};

// Local view of one Main.Fade[] entry (76 bytes) - Ghidra's FADE type is misaligned
struct OP_FADE {
	SHORT Kido;					// 00000000
	SHORT Add;					// 00000002
	UCHAR Mask;					// 00000004
	UCHAR Rgb_mask[3];			// 00000005
	UCHAR Pri;					// 00000008
	UCHAR Dummy[3];				// 00000009
	TILE T[2];					// 0000000C
	DR_MODE Dr_mode[2];			// 0000002C
	RECT Rect;					// 00000044
};

/* ------------------------------------------------------------------ */
/* Overlay data (OPENING overlay, DS addresses 801bfa18-801c2eeb)     */
/* ------------------------------------------------------------------ */

/* Ctpage per TIM load (index 4 = Ctpage used by the scene 3 init path) */
extern UCHAR Opening_ctpage[5];		// 801BFA24		// 5 bytes
/* Cd_read message string "OP_TIM_LD" (10 bytes with the terminator, OPENING.BIN) */
extern CHAR Opening_str_op_tim_ld[10];	// 801BFA2C		// 10 bytes
/* The words at 801BFA3C-801BFAD4 are the compiler's switch jump tables of the
 * scene timelines (.rdata), not source data. */

/* TIM buffers used by Opening_init and the timelines */
extern ULONG *Opening_scene_tim_buf;	// 8013F814		// 0x25800 bytes: OPEN11/OPEN14 (floc size)
extern ULONG *Opening_tim_load_buf;	// 80165028		// 0x25C20 bytes: OPEN01_TIM, the largest bank TIM (floc size)

/* Cd_read file numbers. [0..3] TIM loads at Opening_init, [4..9] scene loads */
extern USHORT Opening_cd_no[10];		// 801C24AC		// 20 bytes
/* Overlay function tables: [draw/fade mode] and [scene no]; the entries are the
 * words stored in OPENING.BIN at 801C2068 and 801C2078. */
extern VOID *Opening_draw_tbl[4];		// 801C2068		// 16 bytes
extern VOID *Opening_scene_tbl[5];		// 801C2078		// 20 bytes

extern UCHAR Opening_fade_no;			// 801C24C0		// 1 byte
extern USHORT Opening_seq_no;			// 801C24C2		// 2 bytes
extern ULONG Opening_frame_cnt;			// 801C24C4		// 4 bytes
extern USHORT Opening_scene_saved;		// 801C24C8		// 2 bytes
extern UCHAR Opening_skip_ok;			// 801C24CA		// 1 byte
extern SHORT Opening_sprite_num;		// 801C24CC		// 2 bytes
extern UCHAR Opening_draw_mode;			// 801C24CE		// 1 byte
extern USHORT Opening_timer;			// 801C24D0		// 2 bytes
extern USHORT Opening_scene_no;			// 801C24D2		// 2 bytes
extern UCHAR Opening_trans_phase;		// 801C24D4		// 1 byte

/* Double-buffered primitive list: [Cbuf_id][32] POLY_FT4 */
extern POLY_FT4 (*Opening_prim)[32];		// 801C24D8		// 2560 bytes

/* Moji / picture sprite tables (10 bytes each, indexed by OP_WORK.no) */
extern OP_MOJI_TBL Opening_moji_tbl[14];	// 801C208C		// 140 bytes
extern OP_MOJI_TBL Opening_pict_tbl[22];	// 801C2118		// 220 bytes

/* Sprite lists of the three scenes (12 bytes each) */
extern OP_WORK Opening_sprite_s1[28];		// 801C21F4		// 336 bytes
extern OP_WORK Opening_sprite_s2[12];		// 801C2344		// 144 bytes
extern OP_WORK Opening_sprite_s3[18];		// 801C23D4		// 216 bytes

extern OP_WORK *Opening_sprite_cur;		// 801C2ED8		// 4 bytes
extern SHORT Opening_scroll_dy;			// 801C2EDC		// 2 bytes
extern SHORT Opening_blink_dy;			// 801C2EDE		// 2 bytes
extern USHORT Opening_scroll_cnt;		// 801C2EE0		// 2 bytes
extern USHORT Opening_blink_cnt;		// 801C2EE2		// 2 bytes
extern UCHAR Opening_bg_flg;			// 801C2EE4		// 1 byte
extern ULONG Opening_vis_mask;			// 801C2EE8		// 4 bytes

/* Functions */
extern VOID Opening(VOID);
extern VOID Opening_trans_wait(VOID);
extern VOID Opening_init(VOID);
extern VOID Opening_exit(VOID);
extern VOID Opening_bg_fade_in(VOID);
extern VOID Opening_bg_fade_out(VOID);
extern VOID Opening_draw_idle(VOID);
extern VOID Opening_timeline_a(VOID);
extern VOID Opening_timeline_b(VOID);
extern VOID Opening_timeline_c(VOID);
extern VOID Opening_sprite_draw(VOID);
extern VOID Opening_vis_set(UCHAR Flg, CHAR Clr_sw);
extern VOID Opening_fade_set(INT Fade_no, USHORT Ctrl, ULONG Col, ULONG *pPos);

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* OPENING_H_H */
