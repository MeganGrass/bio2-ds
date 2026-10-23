/*
 * OMA.H - Object model and atari (hit area) structures
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OMA.H
 */
#ifndef OMA_H
#define OMA_H
#include <LIBGTE.H>
#include "TYPES.H"
#include "JOINT.H"
#include "SCA.H"

/* ---- types migrated from data.h ---- */
/* Hit area ("atari") of an object or enemy: a centre X/Y/Z, the width and depth (W, D), the height, and the
 * offsets used to place it relative to the owner. At_w/At_d/At_h are the attack-box size. */
struct tagAT_DATA {
	LONG X;
	LONG Y;
	LONG Z;
	USHORT W;
	USHORT D;
	SHORT Ofs_x;
	SHORT Ofs_z;
	SHORT Ofs_y;
	USHORT At_w;
	USHORT At_d;
	USHORT At_h;
	SHORT Atw_x;
	SHORT Atw_z;
};

struct tagOM_PARTS_WORK {
	ULONG be_flg;
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
	UCHAR parts_no;
	CHAR timer;
	SHORT dummy03;
};

struct tagOBJ_MODEL_WORK {
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
	UCHAR Push_cnt;
	UCHAR Free0;
	OM_PARTS_WORK* pSin_parts_ptr;
	PARTS_W Parts;
};

ULONG   Oma_ck_oba(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm, ULONG Flg);
ULONG   Oma_ck_om(OBJ_MODEL_WORK *pOm, OBJ_MODEL_WORK *pOm2);
ULONG   Oma_ck_front(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm);
ULONG   Oma_pl_updown_ck(ULONG Id);
ULONG   Oma_Spl_updown_ck(ENEMY_WORK *pEm);
ULONG   Oma_front_ck(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm);
OBJ_MODEL_WORK *Oma_on_check(ENEMY_WORK *pEm);
void Oma_pull(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm);
void    Oma_pull_hosei_x(ENEMY_WORK *pEm, AT_DATA *pOat, long X2, long W);
void    Oma_pull_hosei_z(ENEMY_WORK *pEm, AT_DATA *pOat, long Z2, long D);
OBJ_MODEL_WORK *Oma_get_on_om(ENEMY_WORK *pEm, AT_DATA *pEat);
void    Oma_ob_pull2(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm, ULONG Id, ULONG Spd);
ULONG   Omd_in_check(VECTOR *pVec, OBJ_MODEL_WORK *pOm, long R, ULONG Flg);
ULONG   Oma_obj_ck_all(VECTOR *pPos, ENEMY_WORK *pEm);                  /* 0x800389e8 */
OBJ_MODEL_WORK *Oma_hit_ck_all(VECTOR *pV, ENEMY_WORK *pEm);
void    Om_Jump_ck(ENEMY_WORK *pEm);

/* Cross-module globals used by OMA.C */
/* DS 0x800a2594 */ extern SHORT   Up_add_tbl[];   /* short[], Id 0..; Id-4 used for Id >= 0x10 */
/* DS 0x800a25bc */ extern SHORT   Down_add_tbl[]; /* short[], Id+8 used for Id >= 0x10 */
/* DS 0x800a260c */ extern SHORT   Up_add_Spl_tbl[];   /* short[], index Id+0x10 */
/* DS 0x800a264c */ extern SHORT   Down_add_Spl_tbl[]; /* short[], index Id+0x10 */


#endif /* OMA_H_H */
