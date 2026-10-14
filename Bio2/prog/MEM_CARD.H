/*
 * MEM_CARD.H - Memory card save data, work block and message tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MEM_CARD.H
 */
#ifndef MEM_CARD_H
#define MEM_CARD_H
#include <LIBGPU.H>
#include <LIBSPU.H>
#include "TYPES.H"
#include "STATUS.H"

/* ---- types migrated from data.h ---- */
struct tagCFILE_INFO {
	UCHAR title[15][22];
	UCHAR num;
	ULONG total_size;
	ULONG size[15];
};

struct tagSAVE_DATA {
	//ULONG Save_data_head[0];
	_CARD Card_Head;
	//ULONG Save_data_head;
	ULONG Game_play_time;
	ULONG Front_play_time;
	UCHAR Max_item_num;
	UCHAR Stereo;
	UCHAR Pl_id;
	UCHAR Save_area;
	UCHAR Vol_Se;
	UCHAR Vol_Bgm;
	UCHAR Key_idx;
	UCHAR Pl_poison_down;
	USHORT Pl_poison_timer;
	USHORT Dummy16;
	ULONG Status_bak;
	UCHAR Equip_id;
	UCHAR Equip_no;
	SHORT Pl_life;
	USHORT Zonbi_cnt;
	USHORT Cure_cnt;
	USHORT Save_cnt;
	SHORT Pl_pos_x;
	SHORT Pl_pos_y;
	SHORT Pl_pos_z;
	ULONG Weapon_mugen;
	USHORT Bgm_tbl[142];
	SHORT F_atari;
	SHORT U_atari;
	SHORT Use_id;
	SHORT Get_id;
	SHORT Sce_work[8];
	SHORT DSce_work[4];
	SHORT Sce_tmp_work[8];
	SHORT Stage_no;
	SHORT Room_no;
	SHORT Cut_no;
	SHORT Room_no_old;
	SHORT Cut_no_old;
	SHORT Sce_random;
	SHORT Player_life;
	SHORT Sce_timer;
	SHORT Change_player;
	SHORT Def_em_set;
	SHORT Def_aot_set;
	SHORT Count_down_timer;
	ULONG Scenario_flg[8];
	ULONG Common_flg[8];
	ULONG Room_flg[2];
	ULONG Enemy_flg[8];
	ULONG Enemy_flg2[8];
	ULONG Item_flg[8];
	ULONG Item_flg2[4];
	ULONG Item_c_flg[7];
	ULONG Map_o_flg;
	ULONG Map_flg[4];
	ULONG Map_c_flg[2];
	ULONG Map_i_flg;
	ULONG Pri_be_flg[16][4];
	ULONG File_flg;
	ULONG Zapping_flg[2];
	ULONG Key_flg[2];
	ITEM_WORK Item_work[11];
	ITEM_WORK Item_boxwork[64];
	UCHAR File_tag_bk[24];
	USHORT Dummy0;
	UCHAR Dummy1[2];
	USHORT Dummy[2];
	ULONG ExData0[8];
	ULONG ExData1[4];
	ULONG ExData2[8];
	ULONG ExData3[25];
	//ULONG Zero_clr_end[0];
	//ULONG Zero_clr_end;
};

/* One 0x80-byte save block as stored in Save_info: the first 0x80 bytes of SAVE_DATA, from Game_play_time (+0x200). */
typedef struct {
	ULONG Game_play_time;
	ULONG Front_play_time;
	UCHAR Max_item_num;
	UCHAR Stereo;
	UCHAR Pl_id;
	UCHAR Save_area;
	UCHAR Vol_Se;
	UCHAR Vol_Bgm;
	UCHAR Key_idx;
	UCHAR Pl_poison_down;
	USHORT Pl_poison_timer;
	UCHAR Dummy16_lo;			/* low byte of SAVE_DATA.Dummy16, the only byte MEM_CARD reads */
	UCHAR Dummy16_hi;
	ULONG Status_bak;
	UCHAR Equip_id;
	UCHAR Equip_no;
	SHORT Pl_life;
	USHORT Zonbi_cnt;
	USHORT Cure_cnt;
	USHORT Save_cnt;
	UCHAR Rest[0x80 - 0x22];	/* rest of the block: copied with the 0x80 transfer, not read by MEM_CARD */
} SAVE_BLOCK;

/* Memory card work block (retail pMwork, base 0x801c45a8). Offsets are the retail ones from the MEMCARD dumps. */
struct tagMWORK {
	UCHAR Port;				/* 0x000 card port 0/1 */
	UCHAR Blink_phase;		/* 0x001 blink phase of the text: 0 = visible half */
	UCHAR Anim_wait;		/* 0x002 frame countdown to the next animation step */
	UCHAR Msg_idx;			/* 0x003 reveal index into the save message line (0xff at start) */
	UCHAR F_num[16];		/* 0x004 card file index per save slot, 0x0f = none; index 15 (0x013) is never used by retail */
	UCHAR Anim_step;		/* 0x014 index into Mc_wait_tbl */
	UCHAR Scrl_ofs;			/* 0x015 top visible slot of the save list, 0..10 */
	UCHAR Arrow_tmr;		/* 0x016 0..31 timer for the scroll arrows */
	UCHAR Pad_stop;			/* 0x017 1 = pad input gated */
	SHORT Bg_kido;			/* 0x018 fade level 0..30 of Bg_rect */
	SHORT Scrl_slide_y;		/* 0x01a list slide offset, 0 at rest */
	UCHAR Card_err_flg;		/* 0x01c card status error seen */
	UCHAR Card_poll_req;	/* 0x01d card status poll requested */
	UCHAR Card_ready;		/* 0x01e card ready result */
	UCHAR Sel_row_bk;		/* 0x01f saved highlight row of the list */
	ULONG Card_total_size;	/* 0x020 copy of CFILE_INFO.total_size from a card search (0x1e000 = empty card) */
	RECT Bg_rect;			/* 0x024 background panel rect, Fade_adjust(3) */
	CFILE_INFO File_info;	/* 0x02c card directory, 0x18c bytes */
	UCHAR Unk_1b8[60];		/* 0x1b8 no retail access; the gap up to the card image at 0x1f4 */
	SAVE_DATA Card_save;	/* 0x1f4 2 KB card image; its Game_play_time (the save summary) is at 0x3f4 */
	UCHAR Card_tail[104];	/* 0x98c rest of the 0x800-byte card read; written by the transfer, not read */
	DR_MOVE Scrl_mask[2][2];	/* 0x9f4 four MoveImage records of the list scroll mask, row-major */
	SAVE_BLOCK Save_info[15];	/* 0xa54 save blocks, one per slot */
	UCHAR Ident_sel;		/* 0x11d4 0 = BASLUS-00421, nonzero = BASLUS-00748 */
	UCHAR Unk_11d5;			/* 0x11d5 written in states 3 and 7, cleared on exit; never read */
	UCHAR Mark_pulse;		/* 0x11d6 pulse phase 0..15 of the slot marker */
	UCHAR Mark_step;		/* 0x11d7 pulse step, +1 or -1 (signed char in use) */
	UCHAR Win_show_flg;		/* 0x11d8 selects the state 0x18 window (1) or bypasses it (0) */
	UCHAR Unk_11d9;			/* 0x11d9 never accessed; alignment byte before Win_kido */
	SHORT Win_kido;			/* 0x11da fade level of the window rects */
	RECT Win_rect[2];		/* 0x11dc Fade_adjust(1) and (2) rects */
};
/*
 * MEM_CARD.H - Memory card access module
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */



/* ------------------------------------------------------------------ */
/* Overlay data (MEMCARD overlay; Ghidra label address in comment)      */
/* ------------------------------------------------------------------ */
extern MWORK *pMwork;               /* 801c431c: current card work block   */
extern UCHAR Mess_text[];           /* 801c3e1c: message string pool       */
extern USHORT Mess_offs[];          /* 801c4260: per-message pool offsets  */
extern UCHAR Mess_ctrl[6];          /* 801c4584: string control codes, [0] = end */
extern UCHAR Card_font[];           /* 801bfad8: font argument to Print14  */
extern UCHAR Mc_search_pat[];       /* 801bfa1c: Card_search name pattern string */
extern UCHAR Mc_name_tmpl[15];      /* 801bfa40: save-slot file-name template, copied by Card_access */
extern UCHAR Mc_wait_tbl[34];        /* 801c42f8: per-step wait counts (halved) for the slot animation; byte table, 1-byte stride (retail lbu) */
extern UCHAR Mc_slot_ofs[];         /* 801c458c: slot index -> digit offset table */
extern UCHAR Mc_prev_slot;          /* 801c45a0: last slot index (pMwork+3) */
extern UCHAR Mc_fname_tmpl[6];      /* 801bfd0c: memory-card file name head "bu00"+2 */
extern UCHAR Mc_area_name[][29];    /* 801c4320: save area names, 29-byte rows */
extern UCHAR Mc_icon_801bfb40[];    /* card icon header templates (38 bytes each) */
extern UCHAR Mc_icon_801bfb68[];
extern UCHAR Mc_icon_801bfb90[];
extern UCHAR Mc_icon_801bfbb8[];
extern UCHAR Mc_icon_801bfbe0[];
extern UCHAR Mc_icon_801bfc08[];
extern UCHAR Mc_icon_801bfc30[];
extern UCHAR Mc_icon_801bfc58[];
extern UCHAR Mc_icon_801bfc80[];
extern UCHAR Mc_icon_801bfca8[];
extern UCHAR Mc_icon_801bfcd0[];
extern UCHAR Mc_icon_801bfcf8[];    /* 2-byte icon fragments */
extern UCHAR Mc_icon_801bfcfc[];
extern UCHAR Mc_icon_801bfd00[];
extern UCHAR Mc_icon_801bfd04[];
extern UCHAR Mc_icon_801bfd08[];

/* Message index n -> pointer into Mess_text */
#define MESS_STR(n)        (&Mess_text[Mess_offs[n]])

/* Libcard wrappers used by this module (BIOS jump stubs) */

/* ------------------------------------------------------------------ */
/* Function prototypes                                                  */
/* ------------------------------------------------------------------ */

void          Card_access(void);
void Cardaccess_init(char Mode);
void Cardaccess_exit(int Param);
void          Card_mess_disp(int Rno, int Mode, UINT Cursor, int Err_code);
void          Cursor_disp(ULONG X, ULONG Y);
void          Search_f_num(UCHAR *F_num);
int Check_card(UCHAR *F_num);
void          Card_blink_head(void);
BOOL          Card_poll(int Port);
UINT Wslot_check(void);
void Card_mess_make(void *Mess, ULONG Save_data, char Cursor);
void          Save_push(void);
void          Load_pop(void);
void          Clear_event_wait(void);
void          Clear_event_wait2(void);
ULONG Card_write(void *Head, char Port, char *Name, char *Title, UINT Cap);
ULONG Card_read(void *Head, char *Name, size_t Cap, off_t Offset);
void Card_search(char Port, CFILE_INFO *Buff, char *Pattern);
UINT Card_check(int Port);
int Card_format_check(int Port);
UINT Card_format(int Port);
ULONG         Get_card_event(void);
ULONG         Get_card_event2(void);
int Card_clear(int Port);


#endif /* MEM_CARD_H_H */
