/*
 * GAME.H - Shared game data types (items, maps, status)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\GAME.H
 */
#ifndef GAME_H
#define GAME_H

#include <LIBGPU.H>
#include "TYPES.H"
#include "MD_SUB.H"

/* ---- types migrated from data.h ---- */
struct tagECG_POINT {
	UCHAR Y;
	UCHAR H;
};

struct tagITEM_MIX_DATA {
	UCHAR Object_item_id;
	UCHAR Mix_type;
	UCHAR Result_item;
	CHAR Mixed_pix_no;
};

struct tagITEM_DATA {
	UCHAR Item_max;
	UCHAR Item_attribute;
	UCHAR Item_num_col;
	UCHAR Item_mix_num;
	ITEM_MIX_DATA* Item_mix_data;
};

struct tagFILE_DATA {
	UCHAR Page_max;
	UCHAR Back_h;
};

struct tagMAP_DATA {
	UCHAR U;
	UCHAR V;
	UCHAR W;
	UCHAR H;
	SHORT C_pos_x;
	SHORT C_pos_y;
	SHORT B_pos_x;
	SHORT B_pos_y;
	UCHAR Map_flg;
	UCHAR Change_flag;
	UCHAR Pl0_flg;
	UCHAR Pl1_flg;
};

struct tagMAP_STAGE_INFO {
	MAP_DATA* pMap_data;
	UCHAR Room_num;
	UCHAR Map;
	USHORT File_no;
};

struct tagSTATUS {
	UCHAR Main_rtn;
	UCHAR Sub_rtn0;
	UCHAR Sub_rtn1;
	UCHAR Sub_rtn2;
	UCHAR Sub_rtn3;
	UCHAR Sub_rtn4;
	UCHAR Sub_rtn5;
	UCHAR Sub_rtn6;
	UCHAR Equip_item_no;
	UCHAR Equip_item_no_old;
	UCHAR Equip_item_id;
	UCHAR Equip_item_id_old;
	UCHAR Icursol_main;
	UCHAR Icursol_sub;
	UCHAR Mcursol0;
	UCHAR Mcursol1;
	UCHAR St_type;
	CHAR F_sel_book;
	CHAR F_sel_file;
	UCHAR F_page_no;
	UCHAR Itembox_tpage;
	UCHAR Fread_char;
	UCHAR Exit_flg;
	UCHAR Same_id_work_no;
	UCHAR Using_item_no;
	UCHAR File_flg;
	UCHAR Map_no;
	UCHAR Pl_chenge_flg;
	CHAR Add_cp_x0;
	CHAR Add_cp_y0;
	CHAR X0;
	CHAR Y0;
	CHAR Add_cp_x1;
	CHAR Add_cp_y1;
	CHAR X1;
	CHAR Y1;
	UCHAR Ibox_sel;
	CHAR Ibox_moxe;
	UCHAR Mixed_id;
	UCHAR Mixed_pix_no;
	UCHAR blink_timer0;
	UCHAR blink_sw0;
	UCHAR blink_timer1;
	UCHAR blink_sw1;
	SHORT Machinegun_trg;
	SHORT Goffset_add;
	SHORT F_move_x;
	SHORT F_move_y;
	SHORT Itemlist_pos_x;
	SHORT Itemlist_pos_y;
	SHORT Message_pos_x;
	SHORT Message_pos_y;
	SHORT Status_pos_x;
	SHORT Status_pos_y;
	SHORT Sub_pos_x;
	SHORT Sub_pos_y;
	SHORT Menu0_pos_x;
	SHORT Menu0_pos_y;
	SHORT Menu1_pos_x;
	SHORT Menu1_pos_y;
	SHORT Frame_pos_x;
	SHORT Frame_pos_y;
	SHORT Ibox_pos_x;
	SHORT Ibox_pos_y;
	SHORT Map_pos_x;
	SHORT Map_pos_y;
	SHORT Map_offset_x;
	SHORT Map_offset_y;
	SHORT File_pos_x;
	SHORT File_pos_y;
	SHORT Ecg_x;
	CHAR Recover_type;
	CHAR Ecg_rtn;
	SHORT Heal_x;
	SHORT Cure_y;
	MODEL_WORK Fmodel[30];
	USHORT Add_life;
	USHORT dm00;
	USHORT Clut[22];
	DR_MODE Dmode[24][2];
};
/*
 * GAME.H - Main game loop, door transition and gun light module
 * Resident Evil 2 Dual Shock (SLUS-00748) reconstruction
 */



void    Game_loop(void);                    /* 80025794 */
void    Game_pad_wait_task(void);           /* 80026b2c */
void    Set_door(void);                     /* 80026b7c */
void    Gun_light_set(void);                /* 80026ed8 */
void    Gun_light_reset(void);              /* 800270ec */



#endif /* GAME_H */
