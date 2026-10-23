#ifndef JOINT_H
#define JOINT_H
/*
 * JOINT.H - Joint and parts work structure definitions
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\JOINT.H
 */
#include <LIBGTE.H>
#include "TYPES.H"

/* Animation table header at Kan (retail Spd_get 0x80015e7c reads lhu +0x2, lw +0x4) */
typedef struct tagKAN_HEAD {
	USHORT dummy00;		/* +0x00 not read in this file */
	USHORT Bone_ofs;	/* +0x02 frame data offset (frame = Kan + Bone_ofs + n * Rec_sz * 4) */
	USHORT Parts_cnt;	/* +0x04 low half of the UINT at +0x04 */
	USHORT Rec_hi;		/* +0x06 high half of the UINT at +0x04 (Rec_sz << 2) */
} KAN_HEAD;

/* ---- types migrated from data.h ---- */
struct tagPARTS_W {
	ULONG Be_flg;
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
	UCHAR Parts_no;
	CHAR Timer1;
	CHAR Timer2;
	UCHAR Sabun_flg;
	SHORT Rot_x;
	SHORT Rot_y;
	SHORT Rot_z;
	SHORT Sabun_cnt0;
	SHORT Timer0;
	SHORT Timer3;
	ULONG* pSa_dat_head;
	SHORT Size_x;
	SHORT Size_y;
	SHORT Size_z;
	SHORT dummy03;
	ULONG* pOya_parts;
	SHORT Free[10];
};
/*
 * JOINT.H - Joint / parts transform and motion sequencer module
 * Resident Evil 2 Dual Shock (SLUS-00748) reconstruction
 *
 * Structures used here (PARTS_W, ENEMY_WORK, MATRIX, ...) come from their owner headers.
 */



/*
 * JOINT_P - player-side joint motion state used by Joint_move_p (DS 0x80029e10).
 * Offsets follow the DS: lbu +0x0 (Move_no), lbu +0x1 (Move_cnt),
 * lbu +0x2 (Hokan_flg), lbu +0x3 (P0_parts_no), lw +0x4 (On_parts_flg),
 * P0_m at +0x8 (t[1] stored at +0x20), P0_sv at +0x28. No other file uses JOINT_P.
 */
struct tagJOINT_P {
	UCHAR   Move_no;        /* +0x00 index into the sequence table (x4)    */
	UCHAR   Move_cnt;       /* +0x01 current frame within the move         */
	UCHAR   Hokan_flg;      /* +0x02 hokan (interpolation) strength        */
	UCHAR   P0_parts_no;    /* +0x03 part 0 (root) selector                */
	UINT    On_parts_flg;   /* +0x04 bit per part: decode this part's rotation */
	MATRIX  P0_m;           /* +0x08 root rotation matrix                  */
	SVECTOR P0_sv;          /* +0x28 root rotation as short vector         */
};

/* ------------------------------------------------------------------ */
/* Parts work setup (DS 0x80028324 .. 0x800289b4)                       */
/* ------------------------------------------------------------------ */
ULONG   PartsWork_set(ENEMY_WORK *pEm, ULONG work_top);								/* 80028324 */
ULONG PartsWork_link(ENEMY_WORK *pEm, ULONG Packet_top, ULONG *pKan_t_ptr, ULONG Flg); 	/* 80028368 */
void    Init_Parts_work(ENEMY_WORK *pEm);											/* 80028648 */
void    Parts_work_kan_init(ENEMY_WORK *pEm, USHORT *pKan);							/* 80028794 */
ULONG   Mem_ck_Parts_work(ENEMY_WORK *pEm);											/* 800289b4 */

/* ------------------------------------------------------------------ */
/* Transform                                                            */
/* ------------------------------------------------------------------ */
void Joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, VECTOR *pPos); 	/* 80027160 */
void    ko_joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG be_flg, MATRIX *wm);	/* 80027434 */
void SortGT3_neo(TMD_MEM *pT_head, POLY_GT3 *bp, ULONG Cv, ULONG D0); 		/* 80027bec */
void SortGT4_neo(TMD_MEM *pT_head, POLY_GT4 *bp, ULONG Cv, ULONG D0); 		/* 80027dbc */
void    ko_joint_trans3(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG be_flg, MATRIX *wm);	/* 80027ff0 */
void    Joint_pos_trans(ENEMY_WORK *pEm);										/* 80028a24 */

/* ------------------------------------------------------------------ */
/* Detached-part physics                                                */
/* ------------------------------------------------------------------ */
void    Parts_down(PARTS_W *pP_ptr, MATRIX *wm);								/* 80028ad8 */
void    Parts_bomb(PARTS_W *pP_ptr);											/* 80028dac */
void    Parts_ryu(PARTS_W *pP_ptr);												/* 80028ea4 */

/* ------------------------------------------------------------------ */
/* Matrix / vector helpers                                              */
/* ------------------------------------------------------------------ */
void    HMatrix(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, ULONG Late);				/* 80028f48 */
void    HMatrix1(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, ULONG Late);			/* 80029120 */
void    Model_color_plus(MODEL_WORK *pMw, ULONG Pluscolor_1, ULONG Pluscolor_2);	/* 8002940c */

/* ------------------------------------------------------------------ */
/* Motion sequencer                                                     */
/* ------------------------------------------------------------------ */
ULONG   Joint_move(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg);	/* 8002959c */
ULONG   J_seq(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG *pSeq_ptr, ULONG Late_flg);		/* 80029614 */
void    hokan_svec(SVECTOR *sv0, SVECTOR *sv1, SVECTOR *sv2, ULONG late);		/* 80029b70 */
void    Pl_set_packet(void);											/* 80029c94 */
ULONG   Joint_move_p(JOINT_P *pJp, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg);	/* 80029e10 */
ULONG   Joint_move2(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg);	/* 8002a490 */
ULONG   J_seq2(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG *pSeq_ptr, ULONG Late_flg);		/* 8002a508 */
ULONG   Joint_move_skip(ENEMY_WORK *pEm, ULONG Seq_t_ptr);							/* 8002a9c8 */


#endif /* JOINT_H_H */
