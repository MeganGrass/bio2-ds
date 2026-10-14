#ifndef TITLE_H
#define TITLE_H
/*
 * TITLE.H - Title screen work structures, data and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\TITLE.H
 */
#include "TYPES.H"


// Ti (80197000). Offsets confirmed from the disassembly of Title_init, Title_ex_init,
// Title_main_select and Title_bg_disp (lui 0x8019 + 0x7000-based stores/loads).
struct TITLE_WORK {
	UCHAR Main_rtn;				// 00000000
	UCHAR Sub_rtn0;				// 00000001
	UCHAR Sub_rtn1;				// 00000002
	UCHAR Sub_rtn2;				// 00000003
	UCHAR Sub_rtn3;				// 00000004
	UCHAR Stereo_bak;			// 00000005
	UCHAR Sel_mode;				// 00000006
	UCHAR Cur;					// 00000007
	UCHAR Disp_cnt;				// 00000008
	UCHAR Disp_add;				// 00000009
	UCHAR Moji_move_cnt;		// 0000000A
	UCHAR Moji_kido;			// 0000000B
	UCHAR Moji_kido2;			// 0000000C
	UCHAR Rookie_cnt;			// 0000000D
	UCHAR Rookie_flg;			// 0000000E
	UCHAR Pad_0F;				// 0000000F
	USHORT Timer;				// 00000010
	SHORT Kido;					// 00000012
	SHORT Add;					// 00000014
	SHORT Tile_kido;			// 00000016 (Title_bg_disp, Title_moji_tile; Title_spy_init writes 0x16 at 80193450)
	SHORT Bg_y;				// 00000018 (Title_bg_disp, Title_bg_load)
	SHORT Pad_1A;			// 0000001A (never written; keeps Bg_cnt aligned)
	LONG Bg_cnt;			// 0000001C
	DR_MODE Moji0;				// 00000020
	DR_MODE Moji1;				// 0000002C
};

struct T_MOJI_TBL {
	SHORT x;					// 00000000
	SHORT y;					// 00000002
	SHORT w;					// 00000004
	SHORT h;					// 00000006
	UCHAR u;					// 00000008
	UCHAR v;					// 00000009
	UCHAR clut_x;				// 0000000A	(tpage x, used as byte << 6)
	UCHAR clut_y;				// 0000000B	(clut y, added to 0x1E0)
	UCHAR flag;					// 0000000C	(semi-trans flag)
	UCHAR rate;					// 0000000D	(tpage mode)
};

extern VOID Title_ex_init(VOID);
extern VOID Title_ex_main(VOID);
extern VOID Ex_battle_result(VOID);
extern VOID Title(VOID);
extern VOID Capcom_logo(VOID);
extern VOID Title_bg_load(VOID);
extern VOID Title_bg_reload(VOID);
extern VOID Title_init(VOID);
extern VOID Title_main(VOID);
extern VOID Title_main_game(VOID);
extern VOID Title_main_wait(VOID);
extern VOID Title_main_select(VOID);
extern VOID Moji_Sel(VOID);
extern VOID Title_sel_menu(VOID);
extern VOID Title_sel_mode(VOID);
extern VOID Title_sel_sound(VOID);
extern VOID Title_main_load(VOID);
extern VOID Title_main_option(VOID);
extern VOID Title_main_information(VOID);
extern VOID Title_spy_init(VOID);
extern VOID Title_spy_main(VOID);
extern VOID Title_game_init(VOID);
extern VOID Init_tbl_load(VOID);
extern VOID Moji_set_work(VOID);
extern VOID Moji_SelectMode(ULONG Mode, ULONG No, ULONG Dis, ULONG Flg);
extern VOID Moji_Game_mode(register ULONG Mode);
extern VOID Moji_Sound(register ULONG Mode);
extern VOID Moji_License(VOID);
extern VOID Moji_Sample(VOID);

extern TITLE_WORK Ti;					// 80197000
extern ULONG Padd2_mode;				// 800D9E60
extern DR_MODE Frame_d[2][4];			// 801933C0		// 96 bytes
extern ULONG Card_head[128];			// 80193008		// 512 bytes
extern T_MOJI_TBL Moji_work_tbl[62];	// 80196ADC		// 868 bytes (Moji_set_work reads 62 entries)
extern TILE Frame[2][4];				// 80193330		// 128 bytes

extern USHORT Title_fade_val;			// 8019703C
extern USHORT Title_fade_add;			// 8019703E
extern USHORT Title_fade_val2;			// 80197040
extern ULONG Ex_mode_sel_flg;			// 80196F08
extern ULONG pRoom_ex;				// 80196F0C
extern UCHAR Ex_room_cnt;			// 80196F12
extern ULONG Ex_room_snd_hdr;			// 80196F14
extern ULONG Ex_room_snd_a;			// 80196F18
extern ULONG Ex_room_snd_b;			// 80196F1C
extern ULONG Sel_mode_flg;			// 80196F90
extern ULONG Rookie_flg_bak;			// 80196F94
extern ULONG Game_mode_flg;			// 80196F98
extern ULONG Sound_sel_flg;			// 80196F9C

/* TITLE overlay data used by TITLE.C (addresses from the Ghidra dumps) */
extern UCHAR Ti_ex_rtn;						/* DS 0x80197042 */
extern UCHAR Ti_ex_cur;						/* DS 0x80197043 */
extern UCHAR Ex_room_snd_buf[0x9C4C];				/* DS 0x80168000 */
extern ULONG Title_bg_work_buf[];			/* DS 0x80132420 */
extern UCHAR Init_tbl_buf[];				/* DS 0x8018d270 */
extern USHORT Open_msg_ptr[6];				/* DS 0x80196a70 */
extern UCHAR Open_msg_00[];					/* DS 0x80196794 */
extern UCHAR Ex_battle_life[];				/* DS 0x80196a8c */
extern TILE Moji_tile_tbl[4];				/* DS 0x80196a9c */
extern TILE Info_tile[2][4];				/* DS 0x80196e84 */
extern DR_MODE Info_dr_mode[2][4];			/* DS 0x80196fa0 */
extern VOID * Title_info_tbl[7];			/* DS 0x801900ac */


#endif /* TITLE_H */
