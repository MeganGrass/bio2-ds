/*
 * PLAYER.H - Player work struct, weapon table and player externs
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PLAYER.H
 */
#ifndef PLAYER_H
#define PLAYER_H
#include <LIBGTE.H>
#include "TYPES.H"
#include "JOINT.H"
#include "OMA.H"
#include "SCA.H"

/* Attack structs used by the weapon tables declared in PL_WEP.H and PL_WEP2.H */
struct tagATTACK_SEQ {
	UCHAR Tbl_no;
	UCHAR Timer;
};

struct tagATTACK_WEAPON_TBL {
	ATTACK_WORK3* At_work_top;
	ATTACK_SEQ* At_seq_top;
};

struct tagATTACK_WORK3 {
	UCHAR Status;
	UCHAR Ck_kin;
	UCHAR Ck_tyu;
	UCHAR Ck_en;
	SHORT Off0_x;
	SHORT Off0_z;
	SHORT R0_x;
	SHORT Half0_R_z;
	SHORT Off1_x;
	SHORT Off1_z;
	SHORT R1_x;
	SHORT Half1_R_z;
	SHORT Off2_x;
	SHORT Off2_z;
	SHORT R2_x;
	SHORT Half2_R_z;
};

/* ---- types migrated from data.h ---- */
struct tagPLAYER_WORK {
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
	SHORT Sce_free2;	/* 0x1d8: Sce_free2 (low word) + Sce_free3 (high word) hold a 32-bit ENEMY_WORK* (see PL_Hit_em) */
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
	USHORT Poison_timer;
	UCHAR Poison_down;
};
/* D:\Bio2\prog\PLAYER.H */

/* Hit-enemy pointer kept in the Sce_free2/Sce_free3 pair (G.Player offset 0x1d8).
 * Pos_em_at_ck() returns (ulong)pEnemy for the enemy it hits; PL_WEP.C and PL_WEP2.C
 * store that 32-bit value into Sce_free2 (low) / Sce_free3 (high). DS 0x8001e8b0 (Esp_0e)
 * reads it as the enemy pointer and uses its Atd[0] X/Y/Z (DS +0x84/+0x88/+0x8c).
 * Use the pair as one pointer: PL_Hit_em(&G.Player). */
#define PL_Hit_em(pPl)	(*(ENEMY_WORK **)&(pPl)->Sce_free2)



/* Player routine / damage / die dispatch. All Pl_* dispatch tables hold 32-bit code
   addresses and are called with (pEm, Kan, Seq) (or (pEm, Key, Key_trg) for the *_pad tables).
   The owner headers declare them as pointer-to-array; PL_TBL_FN takes the address of the
   symbol and indexes it as an array of function pointers. */
typedef void (*PL_FUNC)(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
#define PL_TBL_FN(tbl, idx)	(((PL_FUNC *)&(tbl))[idx])

/* Function prototypes */
void    Player_set(ENEMY_WORK *pEm);
void    Pl_before(ENEMY_WORK *pEm);
void    Pl_after(ENEMY_WORK *pEm);
void    Player_move(ENEMY_WORK *pEm);
void    Pl_init(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv00_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv01_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv03_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv03(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
ULONG   Lim_dir_ck(ENEMY_WORK *pEm, ULONG Lim, short Dir);
void    Pl_neck(ENEMY_WORK *pEm, ULONG Lim, short Dir);
void    pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv04(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv06(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv07(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv08(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv09_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv09(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv0a_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv0a(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void    pl_mv0b(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_dm00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_dm01(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_dm02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_dm03(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    pl_die00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_Em_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_Em_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void    Pl_dead(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
ULONG   Pl_life_down(long Down_life, ULONG Mode);
ULONG   Pl_life_up(long Up_life);
void    Pl_view_esp_set(ENEMY_WORK *pEm, PARTS_W *pParts);	/* DS 0x80040350 */

/* Model / TIM / TMD loader (DS 0x8003b9e0); returns the data address past the TIM block */
ULONG   Pl_model_set(ENEMY_WORK *pEm, int *pData, UCHAR Tpage, UCHAR Clut);

/* Camera-view Esp table records (DS 0x800a4114, 8 bytes each) */
struct tagPL_VIEW_OFS {
	short Ofs_dist;		/* +0 */
	short Dir_vx;		/* +2 */
	short Dir_vy;		/* +4 */
	short Ofs_height;	/* +6 */
};

extern UCHAR       Pl_view_no_tbl[];	/* DS 0x800a4100, indexed by Type & 0xfff */
extern PL_VIEW_OFS Pl_view_ofs_tbl[];	/* DS 0x800a4114 */

/* Cross-module (owners: EM_SUB.C, PAD.C) */
ULONG   Pl_jh_init(ENEMY_WORK *pEm, ULONG Mode);	/* DS 0x80019440 */
void    Pl_jh_move(ENEMY_WORK *pEm);				/* DS 0x80019470 */
void    Pad_vib_set0(int Time, USHORT Attr);		/* DS 0x8003947c */
void    Pad_vib_set1(int Time, UCHAR Power, USHORT Attr);	/* DS 0x80039514 */
void    Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);	/* DS 0x800395b8 */
void    Pad_vib_init(void);							/* DS 0x80039694 */

/* Player move tables (DS 0x80010ecc..0x80010ef2, 0x80010f14). Entries are indexed by D_life_u
   (0 = healthy, 1 = caution, 2 = danger) or by Routine_2 for pl_mv07. */
extern const USHORT Pl_taiki_mot[3];		/* DS 0x80010ecc, pl_mv00 */
extern const UCHAR  Pl_walk_spd[3];			/* DS 0x80010ed4, pl_mv01_pad */
extern const SHORT  Pl_walk_mot[3];			/* DS 0x80010ed8, pl_mv01 / pl_mv04 / pl_mv0a */
extern const UCHAR  Pl_run_spd[3];			/* DS 0x80010ee0, pl_mv02_pad */
extern const UCHAR  Pl_run_mot[3];			/* DS 0x80010ee4, pl_mv02 */
extern const UCHAR  Pl_backwalk_spd[3];		/* DS 0x80010ee8, pl_mv03_pad */
extern const UCHAR  Pl_rot_spd[3];			/* DS 0x80010ef0, pl_mv04_pad */
extern const SVECTOR Pl_mv07_init_sv;			/* DS 0x80010f14 (8 bytes), initial Sv for pl_mv07 */

/* Player-side work / flags addressed directly in DS */
extern UCHAR  Pl_poison_tint;					/* DS 0x800a4005, poison tint level (0..0x40) */
extern UCHAR  Pad_act_flg;					/* DS 0x800e898c, pad flag written by PAD.C */

/* Model load buffer (Player_set): read to DS 0x801bfa14, copied to DS 0x801cd414 */
extern ULONG *Font_tim_buf;				/* DS 0x801bfa14 */
/* Word table at Font_tim_buf + Font_tim_buf[0] (Player_set). Offsets: Seq/Kan/Tmd from
 * the Pl_model_copy_buf base, Tim from the Font_tim_buf base. Evidence: Player_set (DS 0x8003baf0). */
struct tagPL_DATA_TBL {
	int Seq_ofs;
	int Kan_ofs;
	int Tmd_ofs;
	int Tim_ofs;
};
typedef struct tagPL_DATA_TBL PL_DATA_TBL;
/* DS 0x801cd414: view into Font_tim_buf (the model copy sits inside the player-file buffer, +0xDA00) */
#define Pl_model_copy_buf ((UCHAR *)Font_tim_buf + 0xDA00)
extern UCHAR  s_Pl_cd_msg_80010ec8[4];				/* DS 0x80010ec8, Cd_read message buffer */



/* Globals defined in PLAYER.C */
	extern USHORT Pld_no[16];							// 0x800a3fe4	// PLxx_PLD names (FILECODE.H), by enemy Id
	extern UCHAR Pl_neck_timer;							// 0x800a4004
	extern UCHAR Pld_name[40];							// 0x800a4008
	extern PL_FUNC Pl_routine[9];						// 0x800a4030
	extern PL_FUNC Pl_mv_pad[12];						// 0x800a4054
	extern PL_FUNC Pl_mv[12];							// 0x800a4084
	extern VECTOR Pl_kage_pos;							// 0x800a40b4
	extern PL_FUNC Pl_dm[6];							// 0x800a40e4
	extern PL_FUNC Pl_die_t[1];							// 0x800a40fc
	extern ULONG(*gpPl_pac_ptr)[1];						// 0x800d7660
	extern ULONG(*gpPl_tmd_ptr)[1];						// 0x800d7840
	extern ULONG(*gpPl_pac2_ptr)[1];					// 0x800d7858
	extern CHAR UnkMvPadFlg;							// 0x800d785c
	extern ULONG(*gpPl_tmd2_ptr)[1];					// 0x800d8cec
#endif /* PLAYER_H_H */
