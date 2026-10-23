#ifndef EM_SUB_H
#define EM_SUB_H
/*
 * EM_SUB.H - Enemy subroutines (direction, position, shadow, texture, neck, joint)
 * Resident Evil 2 Dual Shock (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\EM_SUB.C
 */


#include <LIBGPU.H>
#include <LIBGTE.H>
#include "TYPES.H"
#include "JOINT.H"
#include "OMA.H"
#include "PLAYER.H"
#include "SCA.H"

/* ---- types migrated from data.h ---- */
/* Shadow ("kage") work for one enemy: an offset/half-size footprint around Pos, drawn as Prim[2]
 * (two POLY_FT4 quads). Be_flg is the work flag and Dir_y is the shadow's Y direction. */
struct tagKAGE_WORK {
	SHORT Off_x;
	SHORT Off_z;
	SHORT Half_x;
	SHORT Half_z;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	UCHAR Be_flg;
	SHORT Dir_y;
	VECTOR* pPos;
	POLY_FT4 Prim[2];
};

/* Shadow work for the 9-shadow variant: same footprint fields as tagKAGE_WORK, with pKw_top
 * pointing at a KAGE_WORK. */
struct tagKAGE_WORK9 {
	SHORT Off_x;
	SHORT Off_z;
	SHORT Half_x;
	SHORT Half_z;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	UCHAR Be_flg;
	SHORT Dir_y;
	VECTOR* pPos;
	KAGE_WORK* pKw_top;
};

struct tagENEMY_WORK {
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
	PARTS_W* pBow_hit_parts;	/* +0x200 bow parts that hit this enemy (pl_bow, DS 0x80046304) */
	VOID* pTbefore_func;
	VOID* pTafter_func;
	SHORT Spd_base_x;
	SHORT Spd_base_y;
	SHORT Spd_base_z;
	SHORT Kage_ofs;
	ULONG Free[12];
	/* retail stride is 0x248 (EM_WORK_SIZE); 0x244..0x247 are never read */
	UCHAR Stride_pad[4];
};

struct tagEM20_FREE {
	UCHAR Area_keep;
	UCHAR Jump_flg;
	SHORT Stress;
	UCHAR Spin_flg;
	UCHAR Fix_cnt;
	UCHAR Hit_type;
	UCHAR Blood;
	UCHAR Em_next;
	UCHAR Kage_routine;
	UCHAR Bang_ok;
	UCHAR Combo_flg;
	SHORT Dash_turn;
	UCHAR Kill_ok;
	UCHAR Target_pl;
	SHORT Yori_fx;
	SHORT Yori_fz;
	UCHAR No_hit;
	UCHAR House_flg;
	UCHAR Outside;
	UCHAR No_atck;
	UCHAR Stay;
	UCHAR No_voice;
	UCHAR No_fire;
};

struct tagEM20_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Dir;
	USHORT Zoom;
};

struct tagEM21F {
	UCHAR Tmp;
	UCHAR Cnt;
	UCHAR Cnt2;
	UCHAR Cnt3;
	UCHAR Na_cnt;
	UCHAR Sca_cnt;
	UCHAR Obj_cnt;
	UCHAR Idly_flg;
	UCHAR Idly_cnt;
	CHAR Pl_dead_cnt;
	USHORT Dummy1;
	SHORT Tmp_0;
	SHORT Tmp_1;
	SHORT Tmp_2;
	USHORT Flg;
	ULONG Padd1;
	ULONG Padd2;
	ULONG Padd3;
	ULONG Padd4;
};

struct tagBLOOD_DATA {
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Parts_no;
};

struct tagWEP_BLOOD {
	UCHAR Num;
	UCHAR Ofs_f;
	UCHAR Ofs_b;
	UCHAR Range;
	UCHAR Id;
	UCHAR Type;
	USHORT Zoom;
};

struct tagDM_ESP2 {
	SHORT X;
	SHORT Y;
	SHORT Z;
	SHORT Dir_y;
	USHORT Parts;
	USHORT Next_flg;
};

struct tagEM22_FREE {
	UCHAR Wall_dir_new;
	UCHAR Sence_lv;
	UCHAR Tongue_Rno;
	UCHAR Kage_Rno;
	USHORT Kage_x;
	USHORT Kage_z;
	SCA_DATA* pSca_dat;
	SCA_DATA* pDest_wall;
	ENEMY_WORK* pTarget;
	USHORT Attribute;
	UCHAR At_cnt;
	UCHAR Wep_no;
	UCHAR Wall_at;
	UCHAR Wep_bak;
	UCHAR Gun_area;
	UCHAR Dm_eff_rtn;
	SHORT Dest_x;
	SHORT Dest_y;
	SHORT Dest_z;
	SHORT Sca_ofs;
	VOID* Dm_eff_func;
	UCHAR Retail_28;	/* retail-only, name unknown: byte at pEm+0x240 */
	CHAR Retail_29;	/* retail-only: signed byte at pEm+0x241 (Spd_x step of Em22_mv0a) */
	SHORT Retail_2a;	/* retail-only: halfword at pEm+0x242 (Spd_y step of Em22_mv0a) */
	SHORT Retail_2c;	/* retail-only: halfword at pEm+0x244 (saved Pos_y) */
};

struct tagEM23_FREE {
	USHORT Attribute;
	UCHAR Turn_cnt;
	UCHAR Blood_rtn;
	SHORT Def_x;
	SHORT Def_z;
	ULONG Rno_bak;
	CHAR Mouse_rtn;
	CHAR Turn_flg;
	UCHAR Gget_rtn;
	UCHAR Earthquake_ctr;
	SHORT Mouse_ctr;
	SHORT Neck_ctr;
	UCHAR Stumble_cnt;
	UCHAR Head_rtn;
	UCHAR Dm_timer;
	UCHAR Dm_flg;
	UCHAR Dm_rtn_type;
	UCHAR Dm_eff_rtn;
	UCHAR Roar_cnt;
	UCHAR Rensha_cnt;
	VOID* Dm_eff_func;
	UCHAR Retail_20;	/* retail-only: countdown byte at pEm+0x238 */
};

struct tagEM23_ESP {
	SHORT X;
	SHORT Y;
	SHORT Z;
	SHORT Dir_y;
	ULONG Parts;
};

struct tagEM23_WEP_ESP {
	UCHAR Id;
	UCHAR Type;
	USHORT Zoom;
};

struct tagEM25_FREE {
	SHORT at_cnt;
	SHORT Flip;
	SHORT sch_flg;
	SHORT sc_timer;
	UCHAR Foot_flg;
	UCHAR Foot_cnt;
	UCHAR Place;
	UCHAR Jump_flg;
	UCHAR Die_weak;
	UCHAR Kage_routine;
	SHORT Sc_dir;
	SCA_DATA* pWall;
	LONG Down_point;
	UCHAR Wall_pos;
	UCHAR Em_next;
	UCHAR Nowall_cnt;
	UCHAR Rno1_bak;
	UCHAR Rno2_bak;
	UCHAR Rno3_bak;
	UCHAR Cont_flg;
	UCHAR Target_pl;
	UCHAR Foot_se;
	UCHAR No_redie;
	CHAR Hitline;
	UCHAR Spark_cnt;
	UCHAR Dmg_type;
	UCHAR No_fire;
	UCHAR Water_rno;
	UCHAR Retail_27;	/* retail-only, name unknown: byte at pEm+0x23F */
	ENEMY_WORK* Retail_28;	/* retail-only, name unknown: another enemy at pEm+0x240 */
	SHORT Retail_2C;	/* retail-only, name unknown: float offset at pEm+0x244 */
};

struct tagEM25_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Dir;
	USHORT Zoom;
};

struct tagEM26_FREE {
	PLAYER_WORK* pTarget_pl;
	SHORT No_catch_cnt;
	CHAR kasakasarot;
	CHAR Catch_dir;
	UCHAR First_catch;
	UCHAR Atck_flg;
	UCHAR Jump_flg;
	CHAR Chase_rot;
};

struct tagEM26_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Dir;
	USHORT Zoom;
};

struct tagEM26_CATCH_TBL {
	USHORT Id;
	SHORT fx;
	SHORT fy;
	SHORT fz;
};

struct tagEM27_FREE {
	SHORT Drf_0;
	SHORT Drf_1;
	SHORT Drf_2;
	SHORT Drf_3;
	SHORT Drf_4;
	SHORT Drf_5;
	SHORT Drf_6;
	SHORT Drf_7;
	SHORT Dir_h;
	SHORT Hajiki;
	USHORT Etc_flg;
};

struct tagEM28_FREE {
	USHORT Ko_timer;
	USHORT Re_timer;
	UCHAR Kudure_t;
	UCHAR At_timer;
	USHORT Etc_flg;
	CHAR Ude_life;
	UCHAR Iki_timer;
	CHAR Dm_cnt;
	UCHAR Mg_rout0;
	CHAR Mg_timer0;
	SHORT Mg_dir;
	UCHAR Fire_c;
};

struct tagEM28_P_FREE {
	UCHAR _R0;
};

struct tagEM29_FREE {
	SHORT R;
	SHORT Spd_r;
	SHORT Dest_dir;
	USHORT Attribute;
	SHORT Dest_x;
	SHORT Dest_y;
	SHORT Dest_z;
	UCHAR Target_no;
	UCHAR Foot_die_flg;
	UCHAR Spark_timer;
};

struct tagHOLD_TBL {
	USHORT Parts_no;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Dir_x;
	SHORT Dir_y;
	SHORT Dir_z;
};

struct tagEM2A_FREE {
	ULONG Rno_bak;
	SHORT Pold_x;
	SHORT Pold_z;
	VOID* Dm_eff_func;
	USHORT Attribute;
	USHORT Old_dis;
	UCHAR Dm_eff_rtn;
	UCHAR Dm_wep;
	UCHAR LRpunch_wait;
	UCHAR Cfpunch_wait;
	UCHAR Retail_14;
	UCHAR Retail_15;
	UCHAR Damage_no;
	UCHAR Damage_old;
	DM_ESP2* pEsp2;
	ULONG Buf0;
	ULONG Buf1;
};

struct tagDM_ESP {
	SHORT X;
	SHORT Y;
	SHORT Z;
	SHORT Dir_y;
	ULONG Parts;
};

struct tagDM_ESP3 {
	UCHAR Id;
	UCHAR Type;
	UCHAR Parts;
	UCHAR Next_flg;
	SHORT X;
	SHORT Y;
	SHORT Z;
	SHORT Dir_y;
};

struct tagDM_WEP_ESP {
	UCHAR Id;
	UCHAR Type;
	USHORT Zoom;
};

struct tagEM2B_FREE {
	ULONG Rno_bak;
	USHORT Attribute;
	CHAR Breast_Rno;
	CHAR Dm_wep;
	VOID* Dm_eff_func;
	VOID* At_eff_func;
	UCHAR Dm_eff_rtn;
	CHAR At_eff_rtn;
	CHAR At_wait;
	CHAR Old_dir;
	SHORT Pold_x;
	SHORT Pold_z;
	SHORT Bak_x;
	SHORT Bak_z;
	USHORT Escape_cnt;
	UCHAR Retail_1e;
};

struct tagPARTS_FLY {
	USHORT Parts_no;
	SHORT Dir_y;
	SHORT Spd;
	SHORT Add;
	SHORT Spd_y;
	SHORT Timer;
	SHORT Cdirspd;
	SHORT Cdiradd;
	SHORT Cflg;
	SHORT Smoke_flg;
};

struct tagEM2d_FREE {
	SHORT Dir_y;
	LONG Pos_x;
	LONG Pos_z;
	SHORT Se_first;
};

struct tagEM2F_FREE {
	UCHAR Hand_no;
	UCHAR Att;
	SHORT Set_fx;
	SHORT Set_fy;
	SHORT Set_fz;
	SHORT Set_dir;
	UCHAR No_atck;
	UCHAR Act_type;
	SHORT Base_dir;
	SHORT Set_dirz;
	UCHAR On_flg;
	UCHAR Hit_flg;
	UCHAR Col_flg;
	UCHAR Sick_flg;
	UCHAR Set60A;
	UCHAR Rno2_stk;
	UCHAR Rno3_stk;
	UCHAR Em_keep;
};

struct tagEM2E_FREE {
	UCHAR Target_pl;
	UCHAR Twins_flg;
	UCHAR Nomove_flg;
	UCHAR Atck_type;
	UCHAR Flower_flg;
	UCHAR Dmg_point;
	SHORT Flower_rate;
	UCHAR No_atck;
	UCHAR No_catch;
	UCHAR No_poison;
	UCHAR Stick_cnt;
	UCHAR Em_next;
	UCHAR Hand_req;
	UCHAR Hand_rest;
	UCHAR Req_att;
	SHORT Life_half;
	UCHAR Flower_r0;
	UCHAR Flower_r1;
	UCHAR Flower_free;
	UCHAR Stay;
	UCHAR Rno1_stk;
	UCHAR Rno2_stk;
	UCHAR Rno3_stk;
	UCHAR Rev_flg;
	UCHAR No_rev;
	UCHAR Kage_routine;
	ENEMY_WORK* pTg_em;
	USHORT Die_cnt;
	UCHAR No_atck2[2];
	UCHAR Biri_cnt;
	UCHAR Sick_flg;
	UCHAR Poison_type;
	UCHAR No_fire;
	UCHAR Retail_28;
};

struct tagEM2E_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Pal;
	USHORT Zoom;
};

struct tagEM2E_HITSIZE {
	USHORT No;
	SHORT R;
};

struct tagEM2E_HAND {
	UCHAR No;
	UCHAR Att;
	SHORT Fx;
	SHORT Fy;
	SHORT Fz;
};

struct tagEM2F_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Dir;
	USHORT Zoom;
};

struct tagEM30_FREE {
	UCHAR Dm_timer0;
	UCHAR Dm_cnt;
	UCHAR At_timer;
	UCHAR Nige;
	USHORT Etc_flg;
	CHAR Iki_timer;
	CHAR Me_timer1;
	UCHAR Me_r_0;
	UCHAR Me_r_1;
	SHORT Me_timer0;
	UCHAR K_timer;
	UCHAR K_seq;
	UCHAR K_zan;
	UCHAR Mg_rout0;
	CHAR Mg_timer0;
	SHORT Mg_dir;
	UCHAR Fire_c;
};

struct tagEM30_PARTS_FREE {
	UCHAR r0;
	UCHAR r1;
};

struct tagEM31_FREE {
	UCHAR Dm_timer0;
	UCHAR Dm_cnt;
	UCHAR At_timer;
	UCHAR plus_life;
	USHORT Etc_flg;
	CHAR Iki_timer;
	CHAR Me_timer1;
	UCHAR Me_r_0;
	UCHAR Me_r_1;
	SHORT Me_timer0;
	USHORT life_timer;
	UCHAR Yane_cnt;
	UCHAR Yane_no;
	UCHAR ikaku_num;
	UCHAR Mg_rout0;
	CHAR Mg_timer0;
	SHORT Mg_dir;
	UCHAR Fire_c;
};

struct tagEM33_FREE {
	UCHAR Status;
	UCHAR Biri_cnt;
	SHORT Em_keep;
	UCHAR No_atck[2];
	UCHAR Rno1_stk;
	UCHAR Rno2_stk;
	UCHAR Rno3_stk;
	UCHAR Jump_flg;
	UCHAR Jump_on;
	UCHAR No_fire;
	UCHAR Turn_on;
	UCHAR Stay;
	SHORT Dmg_stk;
	UCHAR Core_rno0;
	UCHAR Core_rno1;
	UCHAR Core_free;
	UCHAR No_jump;
	UCHAR Heart_rno0;
	UCHAR Heart_stop;
	SHORT Heart_size;
	CHAR Turn_keep;
	UCHAR Turn_rev;
	UCHAR Yure_time;
	UCHAR Yure_type;
	UCHAR No_yoro;
	UCHAR Padd00;
	SHORT Spdx_stk;
	SHORT Spdy_stk;
	SHORT Spdz_stk;
	UCHAR Mg_r0;
	UCHAR Mg_t0;
	SHORT Mg_dir;
};

struct tagEM33_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Pal;
	USHORT Zoom;
};

struct tagEM34_FREE {
	SHORT Kabe_dir;
	USHORT Etc_flg;
	UCHAR Dm_timer0;
	UCHAR Dm_cnt;
	UCHAR At_timer;
	UCHAR Kabe_no;
	UCHAR Mg_rout0;
	CHAR Mg_timer0;
	SHORT Mg_dir;
	SHORT Jyu;
	UCHAR Tj_r;
	UCHAR Iki;
	UCHAR Kuchi_r0;
	UCHAR Kuchi_r1;
	SHORT Kuchi_t0;
	SHORT Kuchi_t1;
	CHAR Kuchi_t2;
	CHAR Kuchi_t3;
	SHORT Kubi_dir_z;
	UCHAR J_timer0;
	UCHAR J_timer1;
	UCHAR Kuchi_no;
	UCHAR Kuchi_no1;
	UCHAR Jump_num;
	UCHAR Kin_cnt;
	USHORT Etc_timer;
	UCHAR Cu_cnt;
	UCHAR Up_cnt;
};

struct tagMEDAMA {
	CHAR Tate;
	CHAR Yoko;
	CHAR Tate_d;
	CHAR Yoko_d;
	SHORT Tate_mv;
	SHORT Yoko_mv;
	SHORT Tate_a;
	SHORT Yoko_a;
	UCHAR* pMe3;
	UCHAR* pMe4;
	UCHAR Parts_no;
	CHAR T_max;
	CHAR T_min;
	CHAR Y_max;
	CHAR Y_min;
};

struct tagEM36_FREE {
	UCHAR _R0;
	UCHAR _R1;
	UCHAR _R2;
	CHAR T1;
	SHORT T0;
	SHORT T2;
	CHAR T3;
	UCHAR Dm_timer0;
	UCHAR Dm_cnt;
	UCHAR Q0_t;
	UCHAR Q1_t;
	UCHAR Q_timer;
	USHORT Etc_flg;
	UCHAR Syoku_flg;
	UCHAR Kuchi_at_timer;
};

struct tagEM37_FREE {
	UCHAR No;
	UCHAR Nemoto_no;
	SHORT Pos_x;
	SHORT Size_x;
	SHORT Px;
	SHORT etc_flg;
	UCHAR Q_timer;
	UCHAR Retail_0B;
	USHORT Retail_0C;
};

struct tagEM38_FREE {
};

struct tagEM3A_FREE {
	UCHAR Target_pl;
	CHAR Wing_free;
	SHORT Fwing_ang;
	SHORT Bwing_ang;
	UCHAR Wroutine_0;
	UCHAR Wroutine_1;
	UCHAR Jump_flg;
	UCHAR Wing_flg;
	UCHAR Fly_type;
	CHAR Turn_rate;
	UCHAR Rno1_stk;
	UCHAR Rno2_stk;
	UCHAR Rno3_stk;
	UCHAR Dmg_set;
	UCHAR Wall_ok;
	UCHAR On_wall;
	UCHAR Catch_ok;
	UCHAR Atck_ok;
	UCHAR No_catch;
	UCHAR Esc_limit;
	UCHAR Dmg_free;
	CHAR Burn_cnt;
	UCHAR Fire_cnt;
	UCHAR Kage_routine;
	UCHAR No_fire;
	UCHAR Biri_cnt;
	SHORT Spdx_stk;
	SHORT Spdy_stk;
	SHORT Spdz_stk;
	UCHAR Old_dmg;
	UCHAR No_snd;
	SHORT Em_keep;
	SHORT Retail_26;
	SHORT Retail_28;
	UCHAR Retail_2A;
	UCHAR Retail_2B;
};

struct tagEM3A_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Pal;
	USHORT Zoom;
};

struct tagEM3B_FREE {
	UCHAR Target_pl;
	UCHAR No_set;
	UCHAR Set_cnt;
	UCHAR Ck_cnt;
	UCHAR No_atck;
	UCHAR Em_keep;
	SHORT Keep_x;
	SHORT Keep_z;
	UCHAR Set_max;
	UCHAR Area_type;
	UCHAR Flgset;
	UCHAR Em_next;
};

struct tagEM3B_BLOOD_TBL {
	UCHAR Id;
	UCHAR Type;
	USHORT Pal;
	USHORT Zoom;
};

struct tagTEX_MOVE_SEQ {
	UCHAR Tbl_no;
	UCHAR Timer;
};




/* ------------------------------------------------------------------ */
/* Externs                                                              */
/* ------------------------------------------------------------------ */

/* Player jelly-hair head (used by Pl_jh_init / Pl_jh_move)           */
/* DS 0x8009adbc */
extern JH_HEAD   Pl_jh_head;

/* ------------------------------------------------------------------ */
/* Prototypes (DS address order)                                        */
/* ------------------------------------------------------------------ */

ULONG   Hani_ck(VECTOR *pV, short *pPxzhw);											/* 80015280 */
void    Add_speedXZ();  /* two or three arguments: see Mot_play_sp */									/* 800152c8 */
void    Add_speedXYZ(ENEMY_WORK *pEm, long muki_y, long muki_z);					/* 80015350 */
void    Add_speedXYZsuper(ENEMY_WORK *pEm, long muki_y, long muki_z);				/* 800153f4 */
long   Direction_ck(long Hontai_x, long Hontai_z, long Target_x, long Target_z);	/* 800154ac */
void    Goto00(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);				/* 80015558 */
long    Goto00_ck(ENEMY_WORK *pEm, long Vec_x, long Vec_z, short Add_dir);			/* 80015614 */
void    Goto01(ENEMY_WORK *pEm, short Dir, short Add_dir);							/* 8001569c */
long    Goto01_ck(ENEMY_WORK *pEm, long Dir, long Add_dir);						/* 80015714 */
long    Dir_pos_ck(VECTOR *pV, VECTOR *pP, short Hed_dir, short Hani_dir);			/* 80015758 */
long    L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll);								/* 800157d4 */
long    L_pos_ckXZM(MATRIX *pM, VECTOR *pP, ULONG Ll, SVECTOR *pSv);				/* 80015834 */
UCHAR   Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT);									/* 80015910 */
UCHAR   Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT);									/* 80015930 */
ULONG   Gacha_ch(void);																/* 8001598c */
ULONG   Sikai_ck(ENEMY_WORK *pEm, VECTOR *pTa, long Han, VECTOR *pAi);				/* 800159a4 */
void    Set_base_pos(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);			/* 80015b94 */
void    Null_pos_set(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);			/* 80015cb8 */
void    get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq);				/* 80015db0 */
void    Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);				/* 80015e7c */
UCHAR   Rnd(void);																	/* 80015fe8 */
void    Em_seq_sound(ENEMY_WORK *pEm);												/* 80016028 */
#if 0	/* beta-only prototype, see EM_SUB.C */
void    move_kuchi(ENEMY_WORK *pEm);
#endif												/* beta-only: not present in DS binary */
void    Tex_Ani_move(TEX_MOVE_DATA *T_data, TEX_MOVE_SEQ *T_seq, DR_MOVE *Prim,
					 UCHAR *Set_timer, UCHAR *Set_seq_no);								/* 80016068 */
void    Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no);					/* 80016200 */
void    Kage_work_init(void);														/* 8001633c */
void    Kage_work9_init(void);														/* 8001645c */
void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos); 	/* 80016480 */
void Kage_work9_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos); 	/* 8001653c */
void    MulKage(KAGE_WORK *pKw, short Grand, short Dir_y, ULONG Flg);				/* 800168b4 */
void    Kage_work_sort(void);														/* 8001699c */
void    Kage_work9_sort(void);														/* 80016c14 */
void    Kage_work_color_set(ULONG *pK, ULONG Color);								/* 80016fe4 */
void    Kage_work9_color_set(ULONG *pK, ULONG Color);								/* 8001700c */
void    Texture_Move(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus);	/* 80017054 */
void Texture_Move_GT3(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus, UCHAR *pPoly_no); 	/* 800171d0 */
void Texture_Move_GT4(MODEL_WORK *pMw, USHORT tex_page, ULONG mvmu, short clut_plus, UCHAR *pPoly_no); 	/* 800172f8 */
void Texture_Move_GT4_up(MODEL_WORK *pMw, ULONG Late, ULONG Ouv, UCHAR *pPoly_no);	/* 8001745c */
ULONG   Neck_Dir_ZY(VECTOR *pSv0, VECTOR *pSv1, SVECTOR *pRot, short Front_dir_y);	/* 8001820c */
void    Rot_neck(ENEMY_WORK *pPm, short Cdir_y);									/* 800177c0 */
void    Rot_neck_em(ENEMY_WORK *pPm, short Cdir_y);									/* 80017d98 */
void    Rot_neck_em_n(ENEMY_WORK *pPm, short Cdir_y, short *pDir);					/* 80017fdc */
void    Op_PartsSort00(ENEMY_WORK *pEm);											/* 80018378 */
void    Op_PartsSort05(ENEMY_WORK *pEm);											/* 8001844c */
void    Op_PartsSort06(ENEMY_WORK *pEm);											/* 80018648 */
void    Kami_rot05(PARTS_W *pP_ptr, MATRIX *opw,
				   short jyu, short mg, int ow, int air_u, int z_under,
				   int jyu_x, int mg_x, int ow_x, int air_u_x, int x_under, int now_y);	/* 80018728 */
short   W_rot(short rxz, short ry);													/* 80018cc8 */
short   W_rot_x(short rxz, short ry);												/* 80018d64 */
void    Rbj_set(void);																/* 80018e08 */
void    Em_dead_flg_on(ENEMY_WORK *pEm);											/* 80018fb0 */
ULONG   Aida_Jh_init(ENEMY_WORK *pEm, ULONG Mem_top);								/* 80019024 */
void    Aida_Jh_move(ENEMY_WORK *pEm);												/* 800190c0 */
ULONG   Kg_Jh_init(ENEMY_WORK *pEm, ULONG Mem_top);									/* 800193c0 */
void    Kg_Jh_move(ENEMY_WORK *pEm);												/* 800193f0 */
ULONG   Pl_jh_init(ENEMY_WORK *pEm, ULONG Mode);									/* 80019440 */
void    Pl_jh_move(ENEMY_WORK *pEm);												/* 80019470 */
ULONG   Jh_set(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh, ULONG Mem_top);	/* 80019628 */
void    MulJh(ENEMY_WORK *pEm, PARTS_W *pH_parts, JH_HEAD *pJhh);					/* 800197f4 */
void    Aida_Jh_sort(ENEMY_WORK *pEm);												/* 80019b3c */
void    Jh_sort(ENEMY_WORK *pEm, PARTS_W *pP_ptr);									/* 80019cd0 */



/* Globals defined in EM_SUB.C */
	extern JH_TBL Aida_jh0_tbl[73];						// 0x8009a7c4
	extern JH_HEAD Aida_jh0_head;						// 0x8009a8e8
	extern JH_TBL Aida_jh1_tbl[23];						// 0x8009a920
	extern JH_HEAD Aida_jh1_head;						// 0x8009a97c
	extern JH_TBL Aida_jh2_tbl[23];						// 0x8009a9b4
	extern JH_HEAD Aida_jh2_head;						// 0x8009aa10
	extern JH_TBL Aida_jh4_tbl[18];						// 0x8009aa90
	extern JH_HEAD Aida_jh4_head;						// 0x8009aad8
	extern JH_TBL Aida_jh5_tbl[18];						// 0x8009ab10
	extern JH_HEAD Aida_jh5_head;						// 0x8009aad8
	extern JH_TBL Kg_jh0_tbl[52];						// 0x8009ab90
	extern JH_HEAD Kg_jh0_head;							// 0x8009ac60
#endif /* EM_SUB_H_H */
