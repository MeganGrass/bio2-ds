#ifndef MAIN_H
#define MAIN_H
/*
 * MAIN.H - Main game loop and system module
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */


#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "TYPES.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "OMA.H"
#include "SCA.H"
#include "SCE0.H"
#include "SCE_AT.H"
#include "STATUS.H"
#include "SUB.H"
#include "EM_SET.H"
#include "MEM_CARD.H"

/* PSX break instruction; Ghidra shows it as trap() */
#if defined(_MSC_VER)
#define trap(code) ((void)0)
#else
#define trap(code)	__asm__ volatile ("break %0" : : "i"(code))
#endif /* _MSC_VER */

/* cut / collision region record (Ccut_search, Hit_ck_point4, Dievcut, Ex_battle_vcut) */
struct tagVCUT {
	UCHAR Be_flg;
	UCHAR nFloor;
	UCHAR Fcut;
	UCHAR Tcut;
	SHORT Xz[4][2];
};

/* ---- types migrated from data.h ---- */
struct tagAT_TBL {
	SHORT Off_x;
	SHORT Off_y;
	SHORT Off_z;
	USHORT R;
	USHORT H;
	USHORT W;
};

struct tagAT_WORK {
	SHORT Off_x;
	SHORT Off_y;
	SHORT Off_z;
};

struct tagWOUND_WORK {
	UCHAR Be_flg;
	UCHAR Timer;
	UCHAR dummy00;
	UCHAR dummy01;
	DR_MOVE Prim;
};

struct tagBOM_WORK {
	UCHAR wk_no;
};

struct tagPARTS_BOMB_FREE_WORK {
	SHORT V0_x;
	SHORT V0_y;
	SHORT V0_z;
	SHORT G;
	USHORT Cnt0;
	SHORT Offset_y;
	SHORT Gxz;
};

struct tagPARTS_NECK_FREE_WORK {
	SHORT N_rot_y;
	SHORT N_rot_z;
	SHORT Add_dir_y;
	SHORT Add_dir_z;
	SHORT L_rot_y;
	SHORT L_rot_z;
	UCHAR Sce_add_dir_y;
	UCHAR Sce_add_dir_z;
};

struct tagPARTS_COLOR_CH_FREE_WORK {
	ULONG Color1;
	ULONG Color2;
};

struct tagPARTS_RYU_FREE_WORK {
	SHORT Gt3_y;
	SHORT Gt4_y;
	CHAR Gt3_gy;
	CHAR Gt4_gy;
	SHORT R_timer;
	UCHAR Down_flg;
};

struct tagSUBPLAY_WORK {
	ULONG Be_flg;
	UCHAR Routine_0;
	UCHAR Routine_1;
	UCHAR Routine_2;
	UCHAR Routine_3;
	UCHAR Id;
	UCHAR Sc_id;
	UCHAR At_obj_no;
	UCHAR At_sce_no;
	UCHAR Work_no;
	UCHAR At_em_no;
	USHORT At_em_flg;
	ULONG Attribute;
	ULONG* pTmd;
	ULONG* pPacket;
	ULONG* pTmd2;
	ULONG* pPacket2;
	SHORT M[3][3];
	LONG Pos_x;
	LONG Pos_y;
	LONG Pos_z;
	SHORT Old_x;
	SHORT Old_y;
	SHORT Old_z;
	SHORT Old_x2;
	SHORT Old_y2;
	SHORT Old_z2;
	USHORT dummy00;
	MATRIX Workm;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	USHORT dummy01;
	ULONG Poly_rgb;
	ULONG* pSuper;
	AT_DATA Atd[4];
	UCHAR Tpage;
	UCHAR Clut;
	UCHAR nFloor;
	UCHAR Parts_num;
	ULONG* pKan_t_ptr;
	SHORT Water;
	USHORT Type;
	ULONG Sca_info;
	SCA_DATA* pSca_hit_data;
	SHORT Sca_old_x;
	SHORT Sca_old_z;
	MATRIX Super_matrix;
	SVECTOR Super_vector;
	SHORT Spd_x;
	SHORT Spd_y;
	SHORT Spd_z;
	UCHAR In_screen;
	UCHAR Model_Tex_type;
	UCHAR Move_no;
	UCHAR Move_cnt;
	UCHAR Hokan_flg;
	UCHAR Mplay_flg;
	UCHAR Root_ck_cnt;
	CHAR D_life_u;
	CHAR D_life_c;
	CHAR D_life_d;
	USHORT Status_flg;
	SHORT Life;
	SHORT Timer0;
	SHORT Timer1;
	SHORT Fpos_x;
	SHORT Fpos_y;
	SHORT Fpos_z;
	SHORT Max_life;
	SHORT Base_pos_x;
	SHORT Base_pos_y;
	SHORT Base_pos_z;
	CHAR Timer2;
	CHAR Timer3;
	ULONG* pKage_work;
	SHORT Neck_point_x;
	SHORT Neck_point_y;
	SHORT Neck_point_z;
	SHORT Size;
	ULONG* pNow_seq;
	ULONG* pSeq_t_ptr;
	ULONG* pSub0_kan_t_ptr;
	ULONG* pSub0_seq_t_ptr;
	ULONG* pSub1_kan_t_ptr;
	ULONG* pSub1_seq_t_ptr;
	ULONG* pRoom_kan_t_ptr;
	ULONG* pRoom_seq_t_ptr;
	PARTS_W* pSin_parts_ptr;
	ULONG* pParts_tmd;
	ULONG* pParts_packet;
	ULONG* pM_char_ptr;
	ULONG* pM_option_tmd;
	ULONG* pM_option_packet;
	ULONG* pM_Kage_work;
	ULONG* pEnemy_ptr;
	ULONG* pEnemy_neck;
	ULONG* pSa_dat;
	UCHAR Neck_flg;
	UCHAR Neck_no;
	SHORT Ground;
	SHORT Dest_x;
	SHORT Dest_z;
	USHORT Down_cnt;
	UCHAR At_hit_flg;
	UCHAR Set_flg_no;
	USHORT Sce_flg;
	UCHAR Em_set_flg;
	UCHAR Model_type;
	USHORT Damage_flg;
	UCHAR Damage_no;
	UCHAR Damage_cnt;
	SHORT Sce_free0;
	SHORT Sce_free1;
	SHORT Sce_free2;
	SHORT Sce_free3;
	SHORT Spl_flg;
	SHORT Parts0_pos_y;
	SCA_DATA* pT_xz;
	OBJ_MODEL_WORK* pOn_om;
	ULONG nOba;
	UCHAR Attw_timer;
	UCHAR Attw_seq_no;
	SHORT Eff_at_r;
	ULONG L_pl;
	ULONG L_spl;
	SHORT Dir_spl;
	CHAR Se_type;
	UCHAR Area_no;
	ULONG Tmp_routine;
	ULONG* pDamage_work;
	VOID* pTbefore_func;
	VOID* pTafter_func;
	SHORT Spd_base_x;
	SHORT Spd_base_y;
	SHORT Spd_base_z;
	SHORT Kage_ofs;
};

struct tagROOM_HEAD {
	UCHAR nSprite;
	UCHAR nCut;
	UCHAR nOmodel;
	UCHAR nItem;
	UCHAR nDoor;
	UCHAR nRoom_at;
	UCHAR Reverb_lv;
	UCHAR nSprite_max;
	ULONG* pEdt0;
	ULONG* pVh0;
	ULONG* pVb0;
	ULONG padd0;
	ULONG padd1;
	ULONG* pRbj_end;
	ULONG* pSca;
	RCUT* pRcut;
	VCUT* pVcut;
	LIGHT_CUT_DATA* pLight;
	MODEL_LINK_INFO* pOmodel;
	SHORT* pFloor;
	SHORT* pBlock;
	UCHAR* pMessage;
	UCHAR* pMessage_sub;
	ULONG* pScrl;
	SHORT* pScdx;
	SHORT* pScd;
	ULONG* pEsp_hed;
	ULONG* pEsp_end;
	ULONG* pEsp_tim;
	ULONG* pEsp_tim_end;
	ULONG* pRbj;
};

struct tagDVIEW {
	LONG X;
	LONG Y;
	LONG Z;
};

struct tagSCE_AOT2 {
	UCHAR Id;
	UCHAR Type;
	UCHAR nFloor;
	UCHAR Super;
	SHORT Xz[4][2];
};

struct tagSEARCH_AOT {
	UCHAR Id;
	UCHAR Type;
	USHORT Dummy_data;
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	USHORT Data0;
	USHORT Data1;
	USHORT Data2;
	USHORT Data3;
};

struct tagIN_DOOR_WORK2 {
	UCHAR Id;
	UCHAR Type;
	UCHAR nFloor;
	UCHAR Super;
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	IN_DOOR_WORK in_door;
};

struct tagSCE_AT_DATA {
	SHORT Data0;
	SHORT Data1;
	SHORT Data2;
};

struct tagCORPSE_SAVE_WORK {
	UCHAR Id;
	UCHAR Size;
	UCHAR Ksize_x;
	UCHAR Ksize_z;
	UCHAR Cdir_y;
	CHAR Pos_x;
	CHAR Pos_y;
	CHAR Pos_z;
};

struct tagCORPSE_WORK {
	ULONG Attribute;
	ULONG* pTmd;
	ULONG* pPacket;
	ULONG* pTmd2;
	ULONG* pPacket2;
	SHORT M[3][3];
	LONG Pos_x;
	LONG Pos_y;
	LONG Pos_z;
	SHORT Old_x;
	SHORT Old_y;
	SHORT Old_z;
	SHORT Old_x2;
	SHORT Old_y2;
	SHORT Old_z2;
	USHORT dummy00;
	MATRIX Workm;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
	USHORT dummy01;
	ULONG Poly_rgb;
	ULONG* pSuper;
	ULONG Size;
};

struct tagAF_MOVE_TBL {
	VOID* pFunc;
	ENEMY_WORK* pEwork;
};

struct tagFADE {
	SHORT Kido;
	SHORT Add;
	UCHAR Hrate;
	UCHAR Mask_r;
	UCHAR Mask_g;
	UCHAR Mask_b;
	UCHAR Pri;
	TILE T[2];
	DR_MODE Dr_mode[2];
	RECT Rect;	/* +0x44 screen rect {0, 0, 0x140, 0xf0} written by Fade_set and Fade_adjust */
};

struct tagDBUF {
	DRAWENV Draw;
	DISPENV Disp;
	DR_MODE Curtain_Dm;
	DR_MODE Pause_Dm;
	TILE Bg_clr;
};

struct tagMAIN {
	DBUF Db[2];
	DBUF* pDb;
	ULONG Pad_rmask;
	UCHAR Pad_wait_time;
	UCHAR Pad_repeat_time;
	UCHAR Vloop;
	UCHAR Bg_chg_flg;
	FADE Fade[4];
	RECT Rect;
	UCHAR Bg_mode;
	UCHAR Curtain_kido;
	UCHAR Key_idx;
	UCHAR Scrl_type;
	UCHAR Padoff_stop_flg[4][2];
	UCHAR Pollhost_flg;
};

struct tagGAME {
	UCHAR Doordemo_flg;
	UCHAR Plight_flg;
	UCHAR Plight_cnt;
	UCHAR Plight_col[3];
	ULONG Plight_bak[10];
	VECTOR Plight_pos;
	UCHAR Move_flg;
	UCHAR Rookie_cnt;
	ULONG Stop_bak;
};

struct tagPDEMO {
	UCHAR Stage_no;
	UCHAR Room_no;
	UCHAR Cut_no;
	UCHAR Equip_id;
	UCHAR Equip_no;
	UCHAR Key_idx;
	UCHAR Player_id;
	UCHAR UNKNOWN;
	ITEM_WORK Item_wk[11];
	USHORT Pdemo_cnt;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Cdir_y;
	USHORT Padd[900];
};

struct tagFLAG_DISP_TBL {
	USHORT X;
	UCHAR Y;
	UCHAR Num;
	ULONG* Adr;
};

struct tagROOM_POS {
	USHORT Be_flg;
	SHORT X;
	SHORT Z;
	USHORT nFloor;
	UCHAR Name[17];
};

struct tagDB_WORK {
	UCHAR Menu_flg;
	UCHAR Menu_no;
	UCHAR J_stage;
	UCHAR Xa_no;
	UCHAR Xa_cnt_no;
	UCHAR Disp_at_flg;
	UCHAR Disp_shori_flg;
	UCHAR Disp_monosashi_flg;
	UCHAR Est_test_flg;
	UCHAR Est_test_num;
	USHORT Xa_cnt[8];
	UCHAR J_room[7];
	USHORT Wnd_col;
	USHORT Wnd_x;
	USHORT Wnd_y;
	USHORT Wnd_w;
	USHORT Wnd_h;
	TILE Fade[2];
	DR_MODE Dm[2];
	ULONG Mem[16];
	USHORT S_cnt[16];
	UCHAR R_no;
};

struct tagUTILS {
	UCHAR Menu_flg;
	UCHAR R_no;
	UCHAR Menu_no;
	UCHAR Dummy;
	TILE Fade[2];
	DR_MODE Dm[2];
};

struct tagFLG_VIEW {
	UCHAR CLine;
	UCHAR R_no;
	CHAR Pos_x;
	CHAR Pos_y;
	CHAR Flg_no;
	UCHAR Wait_ctr;
	UCHAR Repeat_ctr;
};

struct tagVIEW_CTR {
	UCHAR R_no;
	CHAR Mode;
	UCHAR Mode_change;
	UCHAR Lock_on;
	UCHAR Col_chg_on;
	UCHAR Col_chg_mode;
	ENEMY_WORK* pE;
	RCUT Rcut_wk;
};

struct tagATTACK_WEAPON_EM_REN {
	SHORT Tyu_ue;
	SHORT Tyu_ge;
};

struct tagPL_MAG_TBL {
	UCHAR Parts_no;
	CHAR V_y;
	UCHAR V_xz;
	UCHAR Flg;
	SHORT Dir;
};

struct tagATTACK_PRIM_F4 {
	PRIM_F4 Pac0[2];
	PRIM_F4 Pac1[2];
	PRIM_F4 Pac2[2];
};

struct tagPARTS_BOW_FREE_WORK {
	UCHAR R_0;
	UCHAR Hit_no;
	SHORT Dir_y;
	ENEMY_WORK* pAe;
};

struct tagEM10_FREE {
	UCHAR Walk_no;
	UCHAR Kahan_r0;
	USHORT Etc_flg;
	UCHAR K_move_no;
	UCHAR K_move_cnt;
	UCHAR K_hokan_flg;
	UCHAR K_timer0;
	SHORT K_dir_y;
	UCHAR Dm_r0;
	CHAR Dm_cnt;
	ULONG* pK_kage;
	ULONG Old_r;
	ULONG Next_r;
	UCHAR Hanare_cnt;
	UCHAR Die_r0;
	SHORT Ground;
	UCHAR Mot_dm;
	UCHAR Rnd_area;
	USHORT A_timer;
	UCHAR W_timer;
	UCHAR Dam_s;
	UCHAR Fire_c;
	UCHAR Ude_down;
	UCHAR Ude_down_r0;
	UCHAR Ude_flg;
	SHORT Dummy;
	SHORT U_pos_x;
	SHORT U_pos_y;
	SHORT U_pos_z;
};



struct tagVIEW_ENV {
	DR_MODE Tex_mode;
	SPRT Sprt;
	TILE View_mask[6];
	LINE_F4 Waku4[4];
	LINE_F2 Waku2[19];
	TILE Cedit_tile[9];
	TILE Cedit_col[256];
	POLY_G4 Cedit_bar[3];
	LINE_F2 Cedit_line2[34];
	LINE_F2 Cedit_waku[6];
	LINE_F2 Cedit_cur2[2];
	LINE_F4 Cedit_cur4[2];
	LINE_F2 Net_line2[48];
	DR_MOVE Clut_move;
};

struct tagVIEW_WORK {
	UCHAR r_no_0;
	UCHAR r_no_1;
	UCHAR r_no_2;
	UCHAR r_no_3;
	CHAR Col_mode;
	UCHAR Net_mode;
	UCHAR Tpage;
	UCHAR Pos_y;
	USHORT Clt_x;
	USHORT Clt_y;
	UCHAR Clut_no;
	UCHAR Bar_no;
	USHORT dummy;
	USHORT Clut_wk[256];
	RECT Clut_get;
};

struct tagVRAMVIEW {
	VIEW_ENV View_env[2];
	VIEW_WORK View_work;
	UCHAR mode;
};





struct tagCAPTION_TBL {
	USHORT Start_frame;
	USHORT Disp_count;
	USHORT No;
	USHORT Height;
	USHORT Pos_x;
	USHORT Pos_y;
};

struct tagCAPTION_ID {
	CAPTION_TBL* pTbl_ptr;
	ULONG Filecode;
	ULONG CaptionBuffId;
};

struct tagCAPTION {
	CAPTION_TBL* pTbl_ptr;
	UCHAR* pBuff;
	UCHAR* pSuper_buff;
	ULONG* pAdr;
	USHORT End_of_jimaku;
	USHORT Decomp_end;
	ULONG Decomp_size;
};

enum {
	CMOJI_00,
	CMOJI_01,
	CMOJI_02,
	CMOJI_03,
	CMOJI_04,
	CMOJI_05,
	CMOJI_06,
	CMOJI_07,
	CMOJI_08,
	CMOJI_09,
	CMOJI_10,
	CMOJI_11,
	CMOJI_12,
	CMOJI_13,
	CMOJI_14,
	CMOJI_15,
	CMOJI_16,
	CMOJI_17,
	EMESS_0,
	EMESS_1,
	EMESS_2,
	EMESS_3,
	EMESS_4,
	EMESS_5,
	EMESS_6,
	EMESS_7,
	EMESS_8,
	MESS_LEON,
	MESS_CLAIRE,
	MESS_LEON_R,
	MESS_CLAIRE_R,
	MESS_SPY,
	MESS_SAVE_CNT,
	AREA_0,
	AREA_1,
	AREA_2,
	AREA_3,
	AREA_4,
	AREA_5,
	AREA_6,
	AREA_7,
	AREA_8,
	AREA_9,
	AREA_10,
	AREA_11,
	AREA_12,
	AREA_13,
	AREA_14,
	AREA_15,
	AREA_16,
	AREA_17,
};

enum {
	SLOT_CHECK,
	USE_CARD,
	SEL_SLOT,
	ERR_MESS,
	NOW_CHECKING,
	CARD_CHECK,
	FILE_SEL,
	READY,
	SAVE,
	SAVE_DEMO,
	LOAD,
	LOAD2,
	FORMAT_CHECK,
	FORMAT,
	ERR_MESS2,
	OVERWRITE,
	SCRL_UP,
	SCRL_DOWN,
	PAGE_CHANGE,
};

enum {
	MES_A00,
	MES_A10,
	MES_A11,
	MES_A20,
	MES_A21,
	MES_CARD00,
	MES_CARD01,
	MES_CARD10,
	MES_CARD20,
	MES_PRESS,
};

enum {
	DUMMY0,
	DUMMY1,
	MES_613_0,
	MES_613_1,
	MES_613_2,
	MES_613_3,
	MES_613_4,
	MES_613_5,
	MES_613_6,
	MES_613_7,
	MES_613_8,
	MES_613_9,
};

enum {
	E1_INIT,
	E1_A1,
	E1_A3,
	E1_A8,
	E1_A9,
	E1_A10,
	E1_A11,
	E1_A12,
	E1_A13,
	E1_PRESS_KEY,
	E1_PRESS_KEY2,
	E1_EXIT,
	E1_EXIT2,
	E1_EXIT3,
};

enum {
	E2_INIT,
	E2_SET_UP,
	E2_SET_UP2,
	E2_SET_UP3,
	E2_INPUT1,
	E2_INPUT2,
	E2_NO_GUEST,
	E2_GUEST,
	E2_ENTRY_END,
	E2_ENTRY_END2,
	E2_ENTRY_END3,
	E2_EXIT,
	E2_EXIT2,
};

/* ------------------------------------------------------------------ */
/* Globals used by MAIN.C (defined in their owning modules)            */
/* ------------------------------------------------------------------ */

/* DS 0x800109e8: constant clear rect used by main() on restart */
extern const RECT ClearImageRect;

/* DS 0x8009dc10: 2 x RECT. Rect[0].x (low 32 bits) holds the scroll offset,
 * Rect[0].w/h and Rect[1].x/y form the RECT passed to LoadImage in Swap_Cbuff. */
extern RECT Rect[2];

/* background-scroll RECT (Bg_draw); DS address not yet verified */
extern RECT Rect_bg;

/* DS 0x8011a000: TIM buffer for TEX.TIM (Init_main) */
extern ULONG Tex_tim_buf[];

/* DS 0x80190000: BS (background stream) load buffer */
extern ULONG Bs_buf[0x3E39];

/* DS 0x80198000: decoded background image / scroll strip buffer */
extern ULONG Bg_img_buf[0xD800];

/* DS 0x80199e00: MDEC input buffer for DecDCTin */
#define Dct_in_buf ((ULONG *)Bs_buf + 0x2780)	/* view into Bs_buf at +0x9e00 */

/* DS 0x801a5800: background TIM buffer (Bg_draw) */
extern ULONG Bg_tim_buf[0x1E00];

/* One entry of the per-(room,cut) Bs_info table (Bs_info_tbl below, 8 bytes each) */
typedef struct tagBS_INFO {
	int ofs;		/* +0 offset of the BS chunk in Bs_buf */
	int flag;		/* +4 non-zero when the BS chunk is used */
} BS_INFO;

/* DS 0x8018fff8: per-(room,cut) table of 8-byte entries.
 * entry base = Bs_info_tbl + ofs, flag = *(int *)(Bs_info_tbl + ofs + 4) */
#define Bs_info_tbl ((UCHAR *)Bs_buf - 8)	/* view 8 bytes before Bs_buf */

/* SLD decompressor (engine code, not PsyQ). Unpacks compressed TIM data for the
 * OT depth textures that are overlaid on the prerendered backgrounds (faux 3D).
 * DS 0x80010000 (Desld_init) / DS 0x80010084 (Desld_proc), region 0x80010000-0x80010778.
 * Owner file not yet assigned. */
void Desld_init(ULONG *pBuf);
int  Desld_proc(void);

/* ------------------------------------------------------------------ */
/* Function prototypes                                                  */
/* ------------------------------------------------------------------ */

void main(void);
void Init_system(void);
void InitGeom_func(void);
void Init_main(void);
void Swap_Cbuff(void);
void Bg_set_mode(ULONG Mode, ULONG Rgb);
void Bg_draw(void);
void Fade_set(USHORT A0, short Add, ULONG Mask, ULONG Pri);
void Fade_adjust(ULONG No, ULONG Kido, ULONG Rgb, RECT *Rect);
void Fade_off(ULONG No);
ULONG Fade_status(ULONG No);
void System_trans(void);
void Init_global(void);
ULONG Cut_check(ULONG Flg);
void Cut_change(ULONG Fc);
VCUT *Ccut_search(ULONG Fc);
ULONG Hit_ck_point4(VECTOR *p, VCUT *cp);
ULONG Hit_ck_box(VECTOR *p, BOX *q);
void Card_event_set(void);
void Logo(void);



/* Globals defined in MAIN.C */
	extern MATRIX LIGHTMATRIX;							// 0x8009db64
	extern MATRIX COLORMATRIX;							// 0x8009db84
	extern TILE Curtain[2][2];							// 0x8009dbac
	extern TILE Pause[2];								// 0x8009dbac
	extern LONG Scrl_h;									// 0x8009dc0c
	extern ULONG Ev0;									// 0x800d7698
	extern ULONG Ev1;									// 0x800d7828
	extern ULONG Ev2;									// 0x800d782c
	extern ULONG Ev3;									// 0x800d7830
	extern ULONG Ev10;									// 0x800d8cd4
	extern ULONG Ev11;									// 0x800d8cdc
	extern ULONG Ev13;									// 0x800d8ce0

/* Global work area G (struct tagGLOBAL) */
struct tagGLOBAL {
	ULONG Ot0[2][8];
	ULONG* pOt0;
	ULONG Ot1[2][1024];
	ULONG* pOt1;
	ULONG Ot2[2][16];
	ULONG* pOt2;
	UCHAR Padd_data[2][36];
	ULONG Padd;
	ULONG Padd_trg;
	USHORT Padd1;
	USHORT Padd1_trg;
	USHORT Padd2;
	USHORT Padd2_trg;
	ULONG Key;
	ULONG Key_trg;
	ULONG Key_old;
	ULONG Random;
	ULONG* pPl_emd_top;
	ULONG* pPl_pac_top;
	ROOM_WORK* pRoom;
	UCHAR* pFree_work_head;
	UCHAR* pFree_work;
	ENEMY_WORK* pEm;
	ENEMY_WORK** ppEnemy_end;
	VCUT* Cccut;
	VCUT* Cccut_next;
	VOID* pPldamage_Em[64];
	VOID* pPldie_Em[64];
	//UCHAR* pBs_check_sum[16];
	UCHAR* pBs_check_sum;
	USHORT* pXa_sector;
	ULONG* pOld_tim;
	ULONG* pOld_tmd;
	IN_DOOR_WORK* pSce_aot_on;
	ULONG* Sp_bak;
	SCE_AOT* Sce_aot[32];
	SCE_AOT* pAot_data;
	ULONG Game_time_offset;
	UCHAR Cbuf_id;
	UCHAR Sound_mode;
	UCHAR Bk_equip_no;
	UCHAR Bk_item_num;
	ITEM_WORK Bk_item_work[10];
	KAGE_WORK9 Kage9[5];
	KAGE_WORK Kage[50];
	//USHORT* pBs_size[16];
	USHORT* pBs_size;
	VOID* pFree;
	SCA_DATA* pSca_hit_data[32];
	ULONG Rbj_set_flg;
	ULONG System_flg;
	ULONG Unknown_Flg;
	UCHAR Window_no;
	UCHAR Mirror_flg;
	SHORT Mirror_pos;
	SHORT Mirror_max;
	SHORT Mirror_min;
	UCHAR Save_cursor;
	UCHAR Dummy8[3];
	VECTOR Wep_hit_pos;
	UCHAR ExData[64];
	//ULONG Zero_clr_head[0];
	ULONG Status_flg;
	ULONG Stop_flg;
	ULONG Use_flg[4];
	UCHAR Ctpage;
	UCHAR Cclut;
	UCHAR Next_cut_no;
	UCHAR Enemy_num;
	USHORT Room_enemy_flg;
	USHORT Room_player_flg;
	PLAYER_WORK Player;
	USHORT Pl_poison_timer;
	UCHAR Pl_poison_down;
	UCHAR unknown_byte;			// unreferenced?
	ENEMY_WORK* pEnemy[34];
	VOID* Em_move_tbl[96];
	VOID* Em_ov_mtbl[2][96];
	EM_BIN_INFO Em_bin_info[2];
	OBJ_MODEL_WORK Ob_model[32];
	OBJ_MODEL_WORK* pObj_work_end;
	SHORT Root_rot;
	SHORT Root_area_no;
	SHORT Root_dest_x;
	SHORT Root_dest_z;
	UCHAR nSce_aot;
	UCHAR Get_item_id;
	USHORT Pdemo_cnt;
	UCHAR Up_event_no;
	UCHAR Up_eve_rtn;
	UCHAR Up_key_rtn;
	UCHAR Up_wnd_rtn;
	USHORT Movie_Isrgb24;
	UCHAR Movie_id;
	UCHAR Ibox_Futa_no;
	SHORT Light3_pos_x;
	SHORT Light3_pos_y;
	SHORT Light3_pos_z;
	USHORT Light3_L;
	UCHAR Light3_col[4];
	UCHAR Memcard_rtn;
	UCHAR Front_func_rtn;
	UCHAR Back_func_rtn;
	UCHAR Em_se_set_no;
	ULONG Em_default;
	ULONG Unknown_Buffer[145];	// never referenced?
	SHORT Scrl_y;
	SHORT Scrl_flg;
	VOID* Front_func;
	VOID* Back_func;
	UCHAR Font_type;
	UCHAR Dummy8_3[3];
	SAVE_DATA SaveData;
};

	extern tagGLOBAL G;									// 0x800cc1e8

/* Game and demo globals defined in GAME.C */
	extern tagGAME Game;								// 0x800df348
	extern PDEMO Demo;									// 0x800df390
	extern UCHAR Key_idx_bak;							// 0x800dfad6

/* Main work area (struct tagMAIN above); defined in CONFIG.C */
	extern tagMAIN Main;								// 0x800dfae0
#endif /* MAIN_H_H */
