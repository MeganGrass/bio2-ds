#ifndef SCE1_H
#define SCE1_H
/*
 * SCE1.H - Event script opcode prototypes and AOT struct
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE1.H
 */
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagSCE_AOT {
	UCHAR Id;
	UCHAR Type;
	UCHAR nFloor;
	UCHAR Super;
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	//USHORT Data[0];
	USHORT Data;
};


/* cross-module globals */
/* Flg_addr: pointer table, DS 0x800a78c8 (4-byte entries), defined in SCE1.C */
extern ULONG *Flg_addr[];

void   Replace_rdt(void);   /* ROOM.C */
int    Stage_room_to_mapno(int Stage_no, int Room_no); /* SCE0.C */

/* function prototypes */
ULONG  Break_point(SCE_TASK *pSce);
ULONG  Nop(SCE_TASK *pSce);
ULONG  Evt_end(SCE_TASK *pSce);
ULONG  Evt_next(SCE_TASK *pSce);
ULONG  Evt_chain(SCE_TASK *pSce);
ULONG  Evt_exec(SCE_TASK *pSce);
ULONG  Evt_kill(SCE_TASK *pSce);
ULONG  Ifel_ck(SCE_TASK *pSce);
ULONG  Else_ck(SCE_TASK *pSce);
ULONG  Endif(SCE_TASK *pSce);
ULONG  Sleep(SCE_TASK *pSce);
ULONG  Sleeping(SCE_TASK *pSce);
ULONG  Wsleep(SCE_TASK *pSce);
ULONG  Wsleeping(SCE_TASK *pSce);
ULONG  For(SCE_TASK *pSce);
ULONG  For2(SCE_TASK *pSce);
ULONG  Next(SCE_TASK *pSce);
ULONG  While(SCE_TASK *pSce);
ULONG  Ewhile(SCE_TASK *pSce);
ULONG  Do(SCE_TASK *pSce);
ULONG  Edwhile(SCE_TASK *pSce);
ULONG  While_main(SCE_TASK *pSce, UCHAR *pData, UCHAR Ofs);
ULONG  Switch(SCE_TASK *pSce);
ULONG  Case(SCE_TASK *pSce);
ULONG  Default(SCE_TASK *pSce);
ULONG  Eswitch(SCE_TASK *pSce);
ULONG  Goto(SCE_TASK *pSce);
ULONG  Gosub(SCE_TASK *pSce);
ULONG  Return(SCE_TASK *pSce);
ULONG  Break(SCE_TASK *pSce);
ULONG  Work_copy(SCE_TASK *pSce);
ULONG  Rbj_reset(SCE_TASK *pSce);
ULONG  Ck(SCE_TASK *pSce);
ULONG  Set(SCE_TASK *pSce);
ULONG  Cmp(SCE_TASK *pSce);
ULONG  Save(SCE_TASK *pSce);
ULONG  Copy(SCE_TASK *pSce);
ULONG  Calc(SCE_TASK *pSce);
ULONG  Calc2(SCE_TASK *pSce);
void   Calc_branch(ULONG Exp, short *Ans, long Num);
ULONG  Sce_rnd(SCE_TASK *pSce);
ULONG  Cut_chg(SCE_TASK *pSce);
ULONG  Cut_old(SCE_TASK *pSce);
void   Cut_chg_main(ULONG Cut_no);
ULONG  Cut_auto(SCE_TASK *pSce);
ULONG  Cut_replace(SCE_TASK *pSce);
ULONG  Cut_be_set(SCE_TASK *pSce);
ULONG  Message_on(SCE_TASK *pSce);
ULONG  Aot_set(SCE_TASK *pSce);
ULONG  Aot_set_4p(SCE_TASK *pSce);
ULONG  Door_aot_set(SCE_TASK *pSce);
ULONG  Door_aot_set_4p(SCE_TASK *pSce);
ULONG  Item_aot_set(SCE_TASK *pSce);
ULONG  Item_aot_set2(SCE_TASK *pSce);
ULONG  Item_aot_set_4p(SCE_TASK *pSce);
ULONG  Aot_reset(SCE_TASK *pSce);
ULONG  Aot_on(SCE_TASK *pSce);
ULONG  Obj_model_set(SCE_TASK *pSce);
ULONG  Super_set(SCE_TASK *pSce);
ULONG  Super_on(SCE_TASK *pSce);
ULONG  Super_reset(SCE_TASK *pSce);
ULONG  Work_set(SCE_TASK *pSce);
ULONG  Parts_set(SCE_TASK *pSce);
ULONG  Speed_set(SCE_TASK *pSce);
ULONG  Add_speed(SCE_TASK *pSce);
ULONG  Add_aspeed(SCE_TASK *pSce);
ULONG  Pos_set(SCE_TASK *pSce);
ULONG  Dir_set(SCE_TASK *pSce);
ULONG  Member_set(SCE_TASK *pSce);
ULONG  Member_set2(SCE_TASK *pSce);
void   Member_set_branch(ENEMY_WORK *pW, ULONG Member, long D0);
ULONG  Member_copy(SCE_TASK *pSce);
ULONG  Member_cmp(SCE_TASK *pSce);
long   Load_member_branch(ENEMY_WORK *pW, ULONG Member);
ULONG  Member_calc(SCE_TASK *pSce);
ULONG  Member_calc2(SCE_TASK *pSce);
short *Load_member_addr_branch(ENEMY_WORK *pW, ULONG Member);
ULONG  Dir_ck(SCE_TASK *pSce);


#endif /* SCE1_H_H */
