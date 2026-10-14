#ifndef RESULT_H
#define RESULT_H
#include "TYPES.H"
// D:\Bio2\prog\RESULT.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


/* RESULT overlay: post-game result / ranking screen (DS 0x80158000-0x8015C3CF) */

#define RESULT_MOJI_USE		0x01	/* entry in use                                   */
#define RESULT_MOJI_F1		0x02	/* set from mode bit 0; draw uses it as semi-trans */
#define RESULT_MOJI_FADE_IN	0x04	/* colour ramps up towards 0x80                   */
#define RESULT_MOJI_FADE_OUT	0x08	/* colour ramps down, entry freed at zero         */
#define RESULT_MOJI_FAST	0x10	/* fade step is faster (x3 for fade-out)          */
#define RESULT_MOJI_HOLD	0x80	/* fully lit, colour held                         */

/* One text entry of the overlay, 12 bytes. Mode bits passed to Result_moji_set():
 *   bit 0 -> RESULT_MOJI_F1, bit 1 -> FADE_IN with colour 0, bit 2 -> FADE_OUT,
 *   bit 3 -> FADE_IN|FAST with colour 0, bits 8-15 -> OT layer, bit 7 -> static. */
struct tagRESULT_MOJI {
	UCHAR Flg;		/* +0x00 RESULT_MOJI_* bits                         */
	UCHAR Ot_no;		/* +0x01 OT layer offset (mode >> 8)                */
	UCHAR Dummy;		/* +0x02 mode >> 16, never read by draw             */
	UCHAR Str_no;		/* +0x03 string index into Result_str_ofs           */
	SHORT X;		/* +0x04                                            */
	SHORT Y;		/* +0x06                                            */
	ULONG Rgb;		/* +0x08 R,G,B in bytes 0-2 (0x80 = full)           */
};

/* One ranking record, 16 bytes, at Result_ex.Rec[n] (G.ExData + 4 + 16*n). */
struct tagRESULT_REC {
	UCHAR Rank;		/* +0x00 stored from Result_ex.Mode                 */
	UCHAR Dummy;		/* +0x01                                            */
	USHORT Recorded;	/* +0x02 1 once a run has been stored               */
	ULONG Time[3];		/* +0x04 three 60 Hz frame counters                 */
};

/* Result data kept in G.ExData[64] (ExData is a plain byte array in GLOBAL). */
struct tagRESULT_EX {
	UCHAR Valid;		/* ExData[0] nonzero: result data pending           */
	UCHAR Variant;		/* ExData[1] layout 0-2                             */
	UCHAR Cur_rec;		/* ExData[2] 1-based index of current record        */
	UCHAR Mode;		/* ExData[3] 0-3, shown as text 0x0c-0x0f           */
	RESULT_REC Rec[3];	/* ExData[4]                                        */
	ULONG Run[3];		/* ExData[52] current run times                     */
};

extern UCHAR Result_mess_bg_tim[];	/* DS 0x80158000	// Cd_read message, file 0xc9 */
extern UCHAR Result_mess_moji_tim[];	/* DS 0x80158008	// Cd_read message, file 0xca */
extern UCHAR Result_mess_init_tbl[];	/* DS 0x80158014	// Cd_read message, file 0xa8 */
extern USHORT Result_str_ofs[28];	/* DS 0x801598C0	// 56 bytes, offset into Result_str_buf */
extern UCHAR Result_str_buf[164];	/* DS 0x8015981C	// 164 bytes, 0xFF-terminated glyph strings */
extern USHORT Result_row_y[3];		/* DS 0x801598F8	// 6 bytes, Y of row 0 by Variant */
extern USHORT Result_row_y2[3];		/* DS 0x80159900	// 6 bytes, Y of row 1 by Variant */
extern USHORT Result_time_ofs[3][4];	/* DS 0x80159908	// 24 bytes, X offsets [Variant][col] */
extern UCHAR Result_glyph[66][4];	/* DS 0x80159920	// 264 bytes, {u, v, w, h} per glyph */
extern VOID *Result_state_tbl[4];	/* DS 0x80159A28	// 16 bytes, state handlers (Result_state) */
extern VOID *Result_sel_tbl[4];		/* DS 0x80159A38	// 16 bytes, selector handlers (Result_sel) */
extern POLY_FT4 Result_prim[2][128];	/* DS 0x80159A48	// 10240 bytes, one buffer per G.Cbuf_id */
extern SHORT Result_timer;		/* DS 0x8015C248	// 2 bytes, frame counter for the timeline */
extern UCHAR Result_state;		/* DS 0x8015C24A	// 1 byte, 0 init 1 fade-in 2 main 3 exit 4 done */
extern UCHAR Result_sel;		/* DS 0x8015C24B	// 1 byte, index into Result_sel_tbl */
extern RESULT_MOJI Result_moji[31];	/* DS 0x8015C250	// 372 bytes, text entries (0x8015C3C4 is end) */

/* Addresses used directly by the overlay (no typed global in MAIN.H) */
#define RESULT_FREE_WORK_BASE	((UCHAR *)0x80160000)	/* G.pFree_work reset in Result_bgm_end */
#define RESULT_SAVE_INIT_SRC	((ULONG *)0x8011C5F8)	/* 0x798-byte save template copied into G.SaveData */

/* Table contents verified from the dispatch code: Result_state_tbl[0] = Result_init,
 * Result_sel_tbl[0] = Result_sel_rtn. Entries 1..3 of both tables are not readable
 * with the available tools (no overlay binary, no memory read). */

extern VOID Result_task(VOID);			/* DS 0x80158020 */
extern VOID Result_init(VOID);			/* DS 0x801580CC */
extern VOID Result_fade_wait(VOID);		/* DS 0x801581AC */
extern VOID Result_main(VOID);			/* DS 0x801581DC */
extern VOID Result_sel_rtn(VOID);		/* DS 0x80158218 */
extern VOID Result_rank_main(VOID);		/* DS 0x80158240 */
extern VOID Result_score_main(VOID);		/* DS 0x8015829C */
extern VOID Result_bgm_end(VOID);		/* DS 0x80158310 */
extern VOID Result_exit(VOID);			/* DS 0x801583E0 */
extern VOID Result_moji_set(SHORT X, SHORT Y, UCHAR Str_no, ULONG Mode);	/* DS 0x8015855C */
extern VOID Result_moji_draw(VOID);		/* DS 0x80158634 */
extern ULONG Result_rank_timeline(VOID);	/* DS 0x801589E4 */
extern VOID Result_rank_name_set(VOID);		/* DS 0x80158A9C */
extern VOID Result_rank_time_set(VOID);		/* DS 0x80158BFC */
extern ULONG Result_score_timeline(VOID);	/* DS 0x80158EF4 */
extern ULONG Result_time_moji_set(ULONG Time, ULONG Slot);	/* DS 0x8015934C */
extern VOID Result_inittbl_load(VOID);		/* DS 0x801597D0 */

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* RESULT_H_H */
