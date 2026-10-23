#ifndef CONFIG_H
#define CONFIG_H

/*
 * D:\Bio2\prog\CONFIG.H
 *
 * Options / configuration screen overlay (SLUS_00748).
 * Overlay range: CONFIG::801bfa18 -- CONFIG::801c6683.
 *
 * All addresses in comments are retail DS addresses (Ghidra).  Layouts and
 * field offsets follow the decompiled / disassembled overlay.
 */

#include <libgs.h>
#include <LIBSND.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagMODE {
	UCHAR Moji_tbl_top;
	UCHAR Moji_Nobj;
};

struct tagP_XY {
	UCHAR Hot_x;
	UCHAR Hot_y;
	UCHAR W;
	UCHAR H;
};

struct tagCNF_SPR {
	UCHAR Be_flg;
	UCHAR Id;
	UCHAR Otz;
	UCHAR Chars;
	SHORT Ofs_x;
	SHORT Ofs_y;
};

/* EDT table slot, 4 bytes; pEdt_adr[6] at DS 800dbb70. Slot 0 holds the BGM
 * fade flag bytes. Defined here, ahead of the extern pEdt_adr declaration. */
struct tagEDT_TABLE_WORK {
	UCHAR id_check;
	UCHAR pan_prog;
	UCHAR tone_pri;
	UCHAR monopoly;
};
typedef struct tagEDT_TABLE_WORK EDT_TABLE_WORK;

struct tagSEQCTR {
	UCHAR Flg;
	UCHAR Ctrl;
	signed char Seq_no;
	signed char Vab_id;
	SndVolume Vol;
};

struct tagBGMCTR {
	UCHAR Main;
	UCHAR Sub;
};

struct tagSEENTRY {
	UCHAR Pri;
	UCHAR Vch;
};

struct tagREQUEST {
	USHORT Be_flg;
	USHORT SENo;
	SHORT Vid;
	SHORT Pno;
	SHORT Tno;
	SHORT Not;
	SHORT Fin;
	SHORT Vol;
	SHORT Vor;
	SHORT flg_3D;
	LONG Pos_x;
	LONG Pos_y;
	LONG Pos_z;
};

/* ---------------------------------------------------------------------------
 * CNF_ST  --  per-row record table, 6 rows, stride 2.  DS 801c3798.
 *   base   first sprite index of the row (index into Cnf_char).
 *   count  number of sprites in the row (the rows are contiguous: base + count
 *          is the next row's base). Not read by the retail code; the name
 *          follows the beta tagMODE (Moji_tbl_top, Moji_Nobj).
 * The current selection of each row is NOT in this record; it lives in the
 * separate Cnf_cur[] array (see below).
 * ---------------------------------------------------------------------------*/
struct tagCNF_ST {
	unsigned char base;
	unsigned char count;
};

/* ---------------------------------------------------------------------------
 * CNF_CHAR  --  sprite / glyph descriptor, 8 bytes, 92 entries.  DS 801c38c4.
 *   flags  bit0 visible, bit1|bit5 selected highlight, bit2 alt colour,
 *          bit5 pulse colour (B_rgb), bit6 red colour.
 *   row    option row that owns this sprite.
 *   otz    ordering table depth.
 *   glyph  index of the first glyph sequence in Mojiretu[].
 *   x, y   screen position (x is also animated for the scroll rows).
 * ---------------------------------------------------------------------------*/
struct tagCNF_CHAR {
	unsigned char flags;
	unsigned char row;
	unsigned char otz;
	unsigned char glyph;
	short         x;
	short         y;
};

#define CNF_CHAR_COUNT   92
#define CNF_ROW_COUNT     6

/* Sprite of row r at offset off from the row base. */
#define CNF_SPR(r, off)  (Cnf_char[Cnf_st[(r)].base + (off)])

/* ---------------------------------------------------------------------------
 * Overlay data (all definitions live in CONFIG.C)
 * ---------------------------------------------------------------------------*/
extern unsigned char  Cnf_status;        /* DS 801c40e8  state index (0-5)        */
extern unsigned char  Cnf_pad_mode;      /* DS 801c40e9  copy of Pad_type1     */
extern unsigned char  Cnf_mode_next;     /* DS 801c40ea  pending option / mode    */
extern unsigned long  B_rgb;             /* DS 801c40ec  pulse colour (RGBC)      */
extern void          *pBak_light;        /* DS 801c40f0  saved pRoom->pLight      */
extern unsigned char  Cnf_err;           /* DS 801c40f4  load error bits (byte)   */
extern POLY_FT4 (*Cnf_Oprim)[120]; /* DS 801c40f8  prim double buffer       */
extern unsigned char  Suuji[6];          /* DS 801c6678  volume digit glyph ids   */
extern void          *pOprim;            /* DS 801c6680  current prim pointer     */
extern unsigned char  Pad_type1;         /* DS 800e8984  port-1 pad type from the pad driver
                                          *              (0 = not read yet, 4 = digital, 7 = analog) */

extern unsigned char  Cnf_mode_naw;      /* DS 801c3cd0  mode flags               */
extern unsigned char  Bak_cut;           /* DS 801c3cd1  Cut_no saved on entry    */
extern unsigned char  Cnf_cd2;           /* DS 801c3cd2  fade toggle              */
extern unsigned char  Cnf_cd3;           /* DS 801c3cd3  volume step              */
extern unsigned char  Cnf_cd4;           /* DS 801c3cd4  volume repeat counter    */
extern unsigned char  Cnf_cd5;           /* DS 801c3cd5  LRGB file index          */
extern unsigned char  Cnf_cur[6];        /* DS 801c3cd8  per-row selection        */
extern unsigned char  Cnf_r_no;          /* DS 801c3cde  current option row       */
extern unsigned char  Cnf_cdf;           /* DS 801c3cdf  cheat counter            */
extern unsigned char  Cnf_scrl_bak;      /* DS 801c3ce0  saved Main.Scrl_type     */
extern unsigned char  Cnf_ce1;           /* DS 801c3ce1  animation counter        */
extern unsigned short *Cnf_eclt;

/* Cnf_cur[5] (DS 801c3cdd) is also the key-config working copy: same byte. */
#define Cnf_cdd          Cnf_cur[5]

extern CNF_ST         Cnf_st[CNF_ROW_COUNT];          /* DS 801c3798            */
extern unsigned char  Cnf_uv[72][4];                  /* DS 801c37a4 u,v,w,h    */
extern CNF_CHAR       Cnf_char[CNF_CHAR_COUNT];       /* DS 801c38c4            */
extern unsigned char  Mojiretu[235];                  /* DS 801c3ba4 glyph seqs */
extern unsigned long  Botan_offset[3];                /* DS 801c3c90 key bitmask */
extern void          (*Config_tblj[6])(void);         /* DS 801c3c9c state table */
extern void          (*Config_padd[6])(void);         /* DS 801c3cb4 row table   */

/* ---------------------------------------------------------------------------
 * Function prototypes (DS address order)
 * ---------------------------------------------------------------------------*/
void Config(void);                /* DS 801bfa84 task entry / main loop        */
void Config_main(void);           /* DS 801bfb28 state 1: calls padd dispatch  */
void Config_fade_in_wait(void);   /* DS 801bfb48 state 0                       */
void Config_fade_out_set(void);   /* DS 801bfba0 state 2                       */
void Config_fade_out_wait(void);  /* DS 801bfca4 state 3                       */
void Config_init(void);           /* DS 801bfedc overlay init                  */
void Config_exit(void);           /* DS 801c01ec state 4: restore + exit       */
void Config_padd_main(void);      /* DS 801c0414 dispatch to row handler       */
void Config_padd_opt(void);       /* DS 801c0498 row 0                         */
void Config_padd_con(void);       /* DS 801c0a34 row 1                         */
void Config_padd_vib(void);       /* DS 801c14e4 row 2                         */
void Config_padd_snd(void);       /* DS 801c1954 row 3                         */
void Config_padd_clr(void);       /* DS 801c2848 row 4                         */
void Config_padd_res(void);       /* DS 801c28d8 row 5                         */
void Config_move(void);           /* DS 801c2ccc per-frame sprite update       */
void Config_main_trans(void);     /* DS 801c2f44 build POLY_FT4 prims          */
void Config_suuji_trans(void);    /* DS 801c339c volume digit strip            */
void Config_lrgb(void);           /* DS 801c3598 state 5: load colour TIM      */
void Config_suuji(int channel, unsigned int vol, int option);  /* DS 801c362c     */



/* Defined in SND.C; declared here because the struct type is defined in this header */
	extern tagEDT_TABLE_WORK pEdt_adr[6];				// 0x800dbb70
#endif /* CONFIG_H_H */
