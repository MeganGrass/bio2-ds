/*
 * SCE1.C - Event script (SCE) command handlers
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE1.C
 */
#include "TYPES.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "MAIN.H"
#include "CONFIG.H"
#include "SCE1.H"
#include "DOOR.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOJIDISP.H"
#include "OM.H"
#include "OMA.H"
#include "PL_WEP2.H"
#include "ROOM.H"
#include "SCE0.H"
#include "SCE_AT.H"
#include "sce_defines.h"
#include "STATUS.H"
#include "SUB.H"
#include "GAME.H"
#include "flag.h"
#include "DIVCHK.H"
#include "SPL_TBL.H"

/* DS takes Rnd() as a signed int (no andi after the jal); Rnd_s32() calls it through a LONG-returning type (see ESP_R.C). */
#define Rnd_s32()	(((LONG (*)(VOID))Rnd)())

/* DS 0x800537cc (Break_point) */
ULONG Break_point(SCE_TASK *pSce)
{
	pSce->Data++;
	return 1;
}

/* DS 0x800537e4 (Nop) */
ULONG Nop(SCE_TASK *pSce)
{
	pSce->Data++;
	return 1;
}

/* DS 0x800537fc (Evt_end) */
ULONG Evt_end(SCE_TASK *pSce)
{
	ULONG Ret;
	UCHAR *D0;
	ULONG Sub_ctr;
	signed char Ifel_ctr;

	Sub_ctr = (ULONG)pSce->Sub_ctr;
	if (Sub_ctr == 0) {
		pSce->Status = 0;
		Ret = 2;
	}
	else {
		D0 = pSce->Ret_addr[Sub_ctr - 1];
		Ret = 1;
		Ifel_ctr = pSce->Ifel_ctr[Sub_ctr - 1];
		pSce->Sub_ctr = pSce->Sub_ctr + -1;
		pSce->Data = D0;
		pSce->pS_SP = pSce->Stack[Sub_ctr - 1] + Ifel_ctr + 1;
	}
	return Ret;
}

/* DS 0x80053860 (Evt_next) */
ULONG Evt_next(SCE_TASK *pSce)
{
	pSce->Data++;
	return 2;
}

/* DS 0x80053878 (Evt_chain) */
ULONG Evt_chain(SCE_TASK *pSce)
{
	Event_init(pSce, (UINT)pSce->Data[3]);
	return 1;
}

/* DS 0x800538a4 (Evt_exec) */
ULONG Evt_exec(SCE_TASK *pSce)
{
	UCHAR *pData;
	UCHAR Evt_no;
	UCHAR Task_level;

	pData = pSce->Data;
	Task_level = pData[1];
	Evt_no = pData[3];
	pSce->Data = pData + 4;
	Event_exec((UINT)Task_level, (UINT)Evt_no);
	return 1;
}

/* DS 0x800538dc (Evt_kill) */
ULONG Evt_kill(SCE_TASK *pSce)
{
	Sce.Sce_task[pSce->Data[1]].Status = 0;
	pSce->Data += 2;
	return 1;
}

/* Forward-jump record of Ifel_ck and Else_ck: the USHORT jump offset is at +2
 * (retail Ifel_ck 0x80053924: lhu a2,0x2(a2)); +0 opcode and +1 are not read here. */
typedef struct {
	UCHAR Dummy[2];
	USHORT Ofs;
} SCE_JMP_REC;

/* DS 0x80053924 (Ifel_ck) */
ULONG Ifel_ck(SCE_TASK *pSce)
{
	UCHAR *pData;
	ULONG D0;

	pData = pSce->Data;
	pSce->Data = pData + 4;
	/* The jump target (the record end plus the USHORT offset) is read before the Ifel counter is
	   raised: DS loads the offset (lhu a2,0x2(a2)) ahead of the counter store (sb v0,0x4(v1)) and
	   adds it to the advanced Data value it has just stored (addu a1,a1,a2). */
	D0 = (ULONG)pSce->Data + ((SCE_JMP_REC *)pData)->Ofs;
	pSce->Ifel_ctr[(signed char)pSce->Sub_ctr]++;
	*pSce->pS_SP++ = D0;
	return 1;
}

/* DS 0x80053964 (Else_ck) */
ULONG Else_ck(SCE_TASK *pSce)
{
	pSce->pS_SP = pSce->pS_SP + -1;
	pSce->Data = pSce->Data + ((SCE_JMP_REC *)pSce->Data)->Ofs;
	pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] = pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] - 1;
	return 1;
}

/* DS 0x800539a0 (Endif) */
ULONG Endif(SCE_TASK *pSce)
{
	pSce->pS_SP = pSce->pS_SP + -1;
	pSce->Data += 2;
	pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] = pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] - 1;
	return 1;
}

/* Sleep record: the USHORT count is at +2 (retail Sleep 0x800539dc: lhu a0,0x2(a2)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Sleep_cnt;
} SCE_SLEEP_REC;

/* DS 0x800539dc (Sleep) */
ULONG Sleep(SCE_TASK *pSce)
{
	/* Sub_idx is a full-width signed index: retail reads Sub_ctr with lb a0+2 and uses it as is */
	LONG Sub_idx;
	UCHAR Loop_cnt;
	UCHAR *pData;

	pData = pSce->Data;
	Sub_idx = (signed char)pSce->Sub_ctr;
	pSce->Data = pData + 1;
	Loop_cnt = pSce->Loop_ctr[Sub_idx] + 1;
	pSce->Loop_ctr[Sub_idx] = Loop_cnt;
	/* Lcnt[Sub_idx] row at pSce + 0xa0 + Sub_idx * 8, column is the new loop level as a signed byte */
	pSce->Lcnt[Sub_idx][(signed char)Loop_cnt] = ((SCE_SLEEP_REC *)pData)->Sleep_cnt;
	return 1;
}

/* Sleep: push a timed-sleep level. The count from the record goes into the level's Lcnt slot and the
 * script continues on the next command (return 1). Sleeping then counts that slot down each frame. */
/* DS 0x80053a24 (Sleeping) */
ULONG Sleeping(SCE_TASK *pSce)
{
	LONG Sub_idx;
	short Sleep_cnt;
	short *pSleep_cnt;

	Sub_idx = (signed char)pSce->Sub_ctr;
	pSleep_cnt = pSce->Lcnt[Sub_idx] + (signed char)pSce->Loop_ctr[Sub_idx];
	Sleep_cnt = *pSleep_cnt;
	*pSleep_cnt = Sleep_cnt + -1;
	if (Sleep_cnt == 1) {
		pSce->Data += 3;
		pSce->Loop_ctr[Sub_idx] = pSce->Loop_ctr[Sub_idx] - 1;
	}
	return 2;
}

/* Wsleep: wait one frame at this point in the script. The loop counter moves on and the opcode is stepped
 * past, so the scheduler re-enters the same opcode on the next frame; Wsleeping (below) ends the wait. */
/* DS 0x80053a8c (Wsleep) */
ULONG Wsleep(SCE_TASK *pSce)
{
	pSce->Loop_ctr[(signed char)pSce->Sub_ctr] = pSce->Loop_ctr[(signed char)pSce->Sub_ctr] + 1;
	pSce->Data++;
	return 1;
}

/* DS 0x80053ac0 (Wsleeping) */
ULONG Wsleeping(SCE_TASK *pSce)
{
	if (((G.Status_flg & STAT_XA_VOICE) == 0) && ((Moji.Mess_flg & 0x80) == 0)) {
		pSce->Data++;
		pSce->Loop_ctr[(signed char)pSce->Sub_ctr] = pSce->Loop_ctr[(signed char)pSce->Sub_ctr] - 1;
	}
	return 2;
}

/* Loop-header record of For: SHORT break offset at +2 and SHORT loop count at +4
 * (retail For 0x80053b1c: lh t2,0x2(t0) and lh v0,0x4(t0)). */
typedef struct {
	UCHAR Dummy[2];
	SHORT Brk_ofs;
	SHORT Lcnt;
} SCE_FOR_REC;

/* Loop-header record of For2 and While and Do: SHORT break offset at +2 (retail For2 0x80053bd4: lh t2,0x2(t0);
 * While 0x80053d3c: lh a3,0x2(a1); Do 0x80053e50: lh t2,0x2(a2)). */
typedef struct {
	UCHAR Dummy[2];
	SHORT Brk_ofs;
} SCE_BRK_REC;

/* DS 0x80053b1c (For) */
ULONG For(SCE_TASK *pSce)
{
	LONG Sub_idx;
	LONG Brk_ofs;
	signed char Loop_idx;
	UCHAR *pCmd;
	UCHAR *pBody;

	pCmd = pSce->Data;
	Sub_idx = pSce->Sub_ctr;
	if (((SCE_FOR_REC *)pCmd)->Lcnt != 0) {
		Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
		pSce->Loop_ctr[Sub_idx] = Loop_idx;
		Brk_ofs = ((SCE_FOR_REC *)pCmd)->Brk_ofs;
		pBody = pCmd + 6;
		pSce->Lcnt[Sub_idx][Loop_idx] = ((SCE_FOR_REC *)pCmd)->Lcnt;
		pSce->Data = pBody;
		pSce->Lbreak[Sub_idx][Loop_idx] = pBody + Brk_ofs;
		pSce->Lstack[Sub_idx][Loop_idx] = pBody;
		pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
	}
	else {
		pSce->Data = pCmd + ((SCE_FOR_REC *)pCmd)->Brk_ofs + 6;
	}
	return 1;
}

/* For: a counted loop. A zero count skips the body (jumps past the break offset). Otherwise a new loop
 * level records the count, the body start, the break target and the current if/else depth, so the
 * matching Next can return to the body or leave by the break target. */
/* DS 0x80053bd4 (For2) */
ULONG For2(SCE_TASK *pSce)
{
	/* retail reserves 8 more bytes of locals (For2 frame 8); never read */
	volatile int Rsv[1];
	LONG Sub_idx;
	LONG Brk_ofs;
	signed char Loop_idx;
	UCHAR *pCmd;
	UCHAR *pBody;

	pCmd = pSce->Data;
	Sub_idx = pSce->Sub_ctr;
	if (G.SaveData.Sce_work[pCmd[5] - 4] != 0) {
		Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
		pSce->Loop_ctr[Sub_idx] = Loop_idx;
		Brk_ofs = ((SCE_BRK_REC *)pCmd)->Brk_ofs;
		pBody = pCmd + 6;
		pSce->Lcnt[Sub_idx][Loop_idx] = G.SaveData.Sce_work[pCmd[5] - 4];
		pSce->Data = pBody;
		pSce->Lbreak[Sub_idx][Loop_idx] = pBody + Brk_ofs;
		pSce->Lstack[Sub_idx][Loop_idx] = pBody;
		pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
	}
	else {
		pSce->Data = pCmd + ((SCE_BRK_REC *)pCmd)->Brk_ofs + 6;
	}
	return 1;
}

/* Next: close a For loop. While the count is above one, control returns to the saved body start; on the
 * last pass the loop level is popped and the script moves past the Next record. */
/* DS 0x80053cbc (Next) */
ULONG Next(SCE_TASK *pSce)
{
	LONG Sub_idx;
	LONG Loop_idx;
	short *pLp_cnt;

	Sub_idx = pSce->Sub_ctr;
	Loop_idx = pSce->Loop_ctr[Sub_idx];
	pLp_cnt = pSce->Lcnt[Sub_idx] + Loop_idx;
	(*pLp_cnt)--;
	if (*pLp_cnt != 0) {
		pSce->Data = pSce->Lstack[Sub_idx][Loop_idx];
	}
	else {
		pSce->Data += 2;
		pSce->Loop_ctr[Sub_idx] = pSce->Loop_ctr[Sub_idx] + -1;
	}
	return 1;
}

/* DS 0x80053d3c (While) */
ULONG While(SCE_TASK *pSce)
{
	signed char Sub_idx;
	short Brk_ofs;
	ULONG Ret;
	UCHAR *pCmd;
	UCHAR *pData;
	signed char Loop_idx;
	UCHAR **ppBrk;

	Sub_idx = pSce->Sub_ctr;
	pCmd = pSce->Data;
	Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
	pSce->Loop_ctr[Sub_idx] = Loop_idx;
	Brk_ofs = ((SCE_BRK_REC *)pCmd)->Brk_ofs;
	pSce->Lstack[Sub_idx][Loop_idx] = pCmd;
	pData = pCmd + 4;
	ppBrk = pSce->Lbreak[Sub_idx] + Loop_idx;
	pSce->Data = pData;
	*ppBrk = pData + Brk_ofs;
	pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
	Ret = While_main(pSce, pData, pCmd[1]);
	if (Ret == 0) {
		pSce->Data = *ppBrk;
		pSce->Loop_ctr[Sub_idx] = pSce->Loop_ctr[Sub_idx] - 1;
	}
	return 1;
}

/* DS 0x80053e0c (Ewhile) */
ULONG Ewhile(SCE_TASK *pSce)
{
	signed char Sub_idx;

	Sub_idx = pSce->Sub_ctr;
	pSce->Data = pSce->Lstack[Sub_idx][(signed char)pSce->Loop_ctr[Sub_idx]];
	pSce->Loop_ctr[Sub_idx] = pSce->Loop_ctr[Sub_idx] - 1;
	return 1;
}

/* DS 0x80053e50 (Do) */
ULONG Do(SCE_TASK *pSce)
{
	signed char Sub_idx;
	short Brk_ofs;
	UCHAR *pBody;
	signed char Loop_idx;

	Sub_idx = pSce->Sub_ctr;
	pBody = pSce->Data;
	Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
	pSce->Loop_ctr[Sub_idx] = Loop_idx;
	Brk_ofs = ((SCE_BRK_REC *)pBody)->Brk_ofs;
	pBody += 4;
	pSce->Data = pBody;
	pSce->Lstack[Sub_idx][Loop_idx] = pBody;
	pSce->Lbreak[Sub_idx][Loop_idx] = pBody + Brk_ofs;
	pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
	return 1;
}

/* DS 0x80053ec4 (Edwhile) */
ULONG Edwhile(SCE_TASK *pSce)
{
	ULONG Sub_ctr;
	UCHAR *pCmd;
	ULONG Ret;
	UCHAR *pData;

	pCmd = pSce->Data;
	pData = pCmd + 2;
	pSce->Data = pData;
	/* Sub_ctr is read as a signed byte (DS lb s1,0x2(s0)) and kept sign-extended in a ULONG for indexing. */
	Sub_ctr = (signed char)pSce->Sub_ctr;
	Ret = While_main(pSce, pData, pCmd[1]);
	/* Each branch returns 1 itself: DS loads v0 = 1 in both paths (li v0,0x1 before the j and before the
	 * final sw), and places the decrement path first as the fall-through after bne v0,zero. */
	if (Ret != 0) {
		pSce->Data = pSce->Lstack[Sub_ctr][(signed char)pSce->Loop_ctr[Sub_ctr]];
		return 1;
	}
	else {
		pSce->Loop_ctr[Sub_ctr] = pSce->Loop_ctr[Sub_ctr] + -1;
		return 1;
	}
}

/* Condition term of While_main: SHORT logic operator at +0 (retail While_main 0x80053f50: lhu s0,0x0(a1))
 * and the opcode byte of the next term at +2 (retail lbu v0,0x0(a1) after a1 += 2; dispatched via Sce_jmp_tbl). */
typedef struct {
	SHORT Logic_op;
	UCHAR Op;
} SCE_COND_REC;

/* DS 0x80053f50 (While_main) */
ULONG While_main(SCE_TASK *pSce, UCHAR *pData, UCHAR Ofs)
{
	short Logic_op;
	UINT Ret;
	UINT Cmd_ret;
	short *pCond;
	UCHAR *pEnd;

	pEnd = pData + Ofs;
	Ret = ((SCE_JMP_FUNC)(Sce_jmp_tbl[*pData]))(pSce);
	pCond = (short *)pSce->Data;
	while (pCond != (short *)pEnd) {
		Logic_op = *pCond;
		pSce->Data = (UCHAR *)(pCond + 1);
		Cmd_ret = ((SCE_JMP_FUNC)(Sce_jmp_tbl[((SCE_COND_REC *)pCond)->Op]))(pSce);
		pCond = (short *)pSce->Data;
		if (Logic_op != 0) {
			Ret |= Cmd_ret;
		}
		else {
			Ret &= Cmd_ret;
		}
	}
	return Ret;
}

/* Switch head record: USHORT break offset at +2 (retail Switch 0x80054020: lhu t0,0x2(a3)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Brk_ofs;
} SCE_SWITCH_REC;

/* Case record: USHORT offset to the next case at +2 (retail lhu t0,0x2(a3)), SHORT case value at +4
 * (retail lh v0,0x4(a3)); +0 is the case marker byte. */
typedef struct {
	UCHAR Dummy[2];
	USHORT Ofs;
	SHORT Val;
} SCE_CASE_REC;

/* Switch: open a switch block. The case value is compared against the saved expression and control moves
 * to the matching Case label, or to the Default/Eswitch exit when none matches. */
/* DS 0x80054020 (Switch) */
ULONG Switch(SCE_TASK *pSce)
{
	LONG Sub_idx;
	UCHAR Var_no;
	USHORT Brk_ofs;
	signed char Loop_idx;
	UCHAR *pCase;
	UCHAR *pNext;

	Sub_idx = (signed char)pSce->Sub_ctr;
	pCase = pSce->Data;
	Brk_ofs = ((SCE_SWITCH_REC *)pCase)->Brk_ofs;
	Var_no = pCase[1];
	pCase += 4;
	Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
	pSce->Loop_ctr[Sub_idx] = Loop_idx;
	pSce->Lbreak[Sub_idx][Loop_idx] = pCase + Brk_ofs;
	pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
	do {
		if (*pCase == 21) {
			pNext = pCase + 2;
			pSce->Data = pNext;
			return 1;
		}
		if (*pCase == 22) {
			pNext = pCase + 2;
			pSce->Loop_ctr[Sub_idx] = pSce->Loop_ctr[Sub_idx] + -1;
			pSce->Data = pNext;
			return 1;
		}
		pNext = pCase + 6;
		if (G.SaveData.Sce_work[Var_no - 4] == ((SCE_CASE_REC *)pCase)->Val) {
			pSce->Data = pNext;
			return 1;
		}
		pCase = pNext + ((SCE_CASE_REC *)pCase)->Ofs;
	} while (1);
}

/* DS 0x800540f8 (Case) */
ULONG Case(SCE_TASK *pSce)
{
	pSce->Data += 6;
	return 1;
}

/* DS 0x80054110 (Default) */
ULONG Default(SCE_TASK *pSce)
{
	pSce->Data += 2;
	return 1;
}

/* DS 0x80054128 (Eswitch) */
ULONG Eswitch(SCE_TASK *pSce)
{
	pSce->Loop_ctr[(signed char)pSce->Sub_ctr] = pSce->Loop_ctr[(signed char)pSce->Sub_ctr] - 1;
	pSce->Data += 2;
	return 1;
}

/* Goto record: SHORT jump offset at +4 (retail Goto 0x8005415c: lh t0,0x4(a1)); +1 and +2 are read as bytes */
typedef struct {
	UCHAR Dummy[4];
	SHORT Jmp_ofs;
} SCE_GOTO_REC;

/* Goto: jump to a label in the script. The offset in the record is relative to the current command. */
/* The task level index is a ULONG holding the sign-extended Sub_ctr byte (lb), as in the prototype's ULONG Sub_ctr local;
 * the jump offset is a LONG local loaded with lh, and the stack slot is Stack[level] + (Ifel_ctr + 1). */
/* DS 0x8005415c (Goto) */
ULONG Goto(SCE_TASK *pSce)
{
	ULONG Sub_idx;
	UCHAR Ifel_val;
	UCHAR Loop_val;
	LONG Jmp_ofs;
	UCHAR *pData;

	Sub_idx = (signed char)pSce->Sub_ctr;
	pData = pSce->Data;
	Ifel_val = pData[1];
	Loop_val = pData[2];
	Jmp_ofs = ((SCE_GOTO_REC *)pData)->Jmp_ofs;
	pSce->Ifel_ctr[Sub_idx] = Ifel_val;
	pSce->pS_SP = pSce->Stack[Sub_idx] + (Ifel_val + 1);
	pSce->Loop_ctr[Sub_idx] = Loop_val;
	pSce->Data = pData + Jmp_ofs;
	return 1;
}

/* Gosub: call a script subroutine. The return address (the command after this one) is saved in Ret_addr
 * for this task level and control jumps to the subroutine entry from the table. */
/* DS 0x800541a8 (Gosub) */
ULONG Gosub(SCE_TASK *pSce)
{
	LONG Sub_idx;
	int Next_idx;
	UCHAR Sub_no;
	USHORT Sub_ofs;
	UCHAR *pSub_tbl;

	Sub_idx = (signed char)pSce->Sub_ctr;
	Next_idx = Sub_idx + 1;
	Sub_no = pSce->Data[1];
	pSce->Ret_addr[Sub_idx] = pSce->Data + 2;
	pSce->Ifel_ctr[Next_idx] = -1;
	pSce->Loop_ctr[Next_idx] = -1;
	pSce->pS_SP = pSce->Stack[Next_idx];
	pSub_tbl = Sce.pCScd;
	Sub_ofs = *(USHORT *)(Sce.pCScd + (UINT)Sub_no * 2);
	pSce->Sub_ctr = Next_idx;
	pSce->Data = pSub_tbl + Sub_ofs;
	return 1;
}

/* DS 0x80054210 (Return) */
ULONG Return(SCE_TASK *pSce)
{
	signed char Sub_idx;

	Sub_idx = (signed char)(pSce->Sub_ctr - 1);
	pSce->Sub_ctr = Sub_idx;
	pSce->Data = pSce->Ret_addr[Sub_idx];
	pSce->pS_SP = pSce->Stack[Sub_idx] + ((signed char)pSce->Ifel_ctr[Sub_idx] + 1);
	return 1;
}

/* DS 0x80054268 (Break) */
ULONG Break(SCE_TASK *pSce)
{
	int Sub_idx;
	int Loop_idx;

	Sub_idx = (signed char)pSce->Sub_ctr;
	Loop_idx = (signed char)pSce->Loop_ctr[Sub_idx];
	pSce->Data = pSce->Lbreak[Sub_idx][Loop_idx];
	pSce->Ifel_ctr[Sub_idx] = pSce->Loop_if_class[Sub_idx][Loop_idx];
	pSce->Loop_ctr[Sub_idx] = Loop_idx - 1;
	return 1;
}

/* DS 0x800542b4 (Work_copy) */
ULONG Work_copy(SCE_TASK *pSce)
{
	ULONG Dst_ofs;
	UCHAR *pData;
	UCHAR D0;
	UCHAR D2;

	pData = pSce->Data;
	D0 = pData[1];
	Dst_ofs = pData[2];
	D2 = pData[3];
	pData += 4;
	pSce->Data = pData;
	if (D2 != 0) {
		*(short *)(pData + Dst_ofs) = G.SaveData.Sce_work[D0 - 4];
	}
	else {
		pData[Dst_ofs] = (UCHAR)G.SaveData.Sce_work[D0 - 4];
	}
	return 1;
}

/* DS 0x80054310 (Rbj_reset) */
ULONG Rbj_reset(SCE_TASK *pSce)
{
	/* DS calls Replace_rdt as a function that returns a value (as an implicit int
	 * declaration would), so the jal counts as a write of v0 even though the value is
	 * not used. With v0 written twice in the function, the "return 1" is no longer
	 * kept at the end of the block: DS increments the Data pointer in v1 and puts
	 * li v0,1 in the load delay slot after lw v1,0x1c(s0). Called through the void
	 * prototype, the pointer is incremented in v0 and a nop fills that slot. */
	((LONG (*)(void))Replace_rdt)();
	Sce.Rbj_reset_flg = 1;
	pSce->Data++;
	return 1;
}


/* DS 0x800a78c8 - ULONG *[40]: addresses of GLOBAL (G) flag words (4-byte entries) */
/* entry 11 (+0x2c): Moji.Mess_flg (DS 0x800e873c, MOJIDISP+0x5c8c) */
ULONG *Flg_addr[40] = {
	/* entry 12 is a 32-bit word: Room_enemy_flg (low half) + Room_player_flg (high half) */
	(ULONG *)&G.System_flg, (ULONG *)&G.Status_flg, (ULONG *)&G.Stop_flg, (ULONG *)&G.SaveData.Scenario_flg[0],	/* +0x0 */
	(ULONG *)&G.SaveData.Common_flg[0], (ULONG *)&G.SaveData.Room_flg[0], (ULONG *)&G.SaveData.Enemy_flg[0], (ULONG *)&G.SaveData.Enemy_flg2[0],	/* +0x10 */
	(ULONG *)&G.SaveData.Item_flg[0], (ULONG *)&G.SaveData.Map_flg[0], (ULONG *)&G.Use_flg[0], (ULONG *)&Moji.Mess_flg,	/* +0x20 */
	(ULONG *)&G.Room_enemy_flg, (ULONG *)&G.SaveData.Pri_be_flg[0][0], (ULONG *)&G.SaveData.Pri_be_flg[1][0], (ULONG *)&G.SaveData.Pri_be_flg[2][0],	/* +0x30 */
	(ULONG *)&G.SaveData.Pri_be_flg[3][0], (ULONG *)&G.SaveData.Pri_be_flg[4][0], (ULONG *)&G.SaveData.Pri_be_flg[5][0], (ULONG *)&G.SaveData.Pri_be_flg[6][0],	/* +0x40 */
	(ULONG *)&G.SaveData.Pri_be_flg[7][0], (ULONG *)&G.SaveData.Pri_be_flg[8][0], (ULONG *)&G.SaveData.Pri_be_flg[9][0], (ULONG *)&G.SaveData.Pri_be_flg[10][0],	/* +0x50 */
	(ULONG *)&G.SaveData.Pri_be_flg[11][0], (ULONG *)&G.SaveData.Pri_be_flg[12][0], (ULONG *)&G.SaveData.Pri_be_flg[13][0], (ULONG *)&G.SaveData.Pri_be_flg[14][0],	/* +0x60 */
	(ULONG *)&G.SaveData.Pri_be_flg[15][0], (ULONG *)&G.SaveData.Zapping_flg[0], (ULONG *)&G.Rbj_set_flg, (ULONG *)&G.SaveData.Key_flg[0],	/* +0x70 */
	(ULONG *)&G.SaveData.Map_c_flg[1], (ULONG *)&G.SaveData.Map_i_flg, (ULONG *)&G.SaveData.Item_flg2[0], (ULONG *)&G.SaveData.Map_o_flg,	/* +0x80 */
	(ULONG *)&G.SaveData.ExData0[0], (ULONG *)&G.SaveData.ExData1[0], (ULONG *)&G.SaveData.ExData2[0], (ULONG *)&G.SaveData.ExData3[0],	/* +0x90 */
};

/* Ck record: USHORT flag bit selector at +2 (retail Ck 0x80054354: lhu a1,0x2(v0)). Selector layout (Ghidra Ck): bits 0-4 = bit in the ULONG, bits 5-7 = word index (byte offset (Bit >> 3) & 0x1c) inside Flg_addr[Flg_no], bits 8+ = 0 means invert */
typedef struct {
	UCHAR Dummy[2];
	USHORT Bit;
} SCE_CK_REC;

/* DS 0x80054354 (Ck) */
ULONG Ck(SCE_TASK *pSce)
{
	UCHAR *pData;
	LONG   D0;
	LONG   D1;
	LONG   D2;
	ULONG *pFlg;

	pData = pSce->Data;
	D0 = pData[1];						/* Flg_addr index */
	D1 = ((SCE_CK_REC *)pData)->Bit;	/* bit selector (DS lhu); LONG so that D1 >> 8 is DS sra */
	pSce->Data = pData + 4;
	/* word (D1 >> 5) & 7 of the flag array; DS folds the index and the * 4 scale into srl 3, andi 0x1c */
	pFlg = Flg_addr[D0] + ((D1 >> 5) & 7);
	/* the if/else form keeps the masked word in its own register (DS and v1,v1,v0; sltu v0,zero,v1) */
	if (*pFlg & (0x80000000U >> (D1 & 0x1f)))
	{
		D2 = 1;
	}
	else
	{
		D2 = 0;
	}
	/* high byte of the selector 0: the result is inverted (test for a clear bit) */
	if ((D1 >> 8) == 0)
	{
		D2 ^= 1;
	}
	return D2;
}

/* DS 0x800543b4 (Set) */
ULONG Set(SCE_TASK *pSce)
{
	UCHAR  *pData;
	UCHAR   D0;
	LONG    D1;
	LONG    D2;
	ULONG  *pFlg;
	ULONG   Top;
	LONG    Bit;

	pData = pSce->Data;
	D0 = pData[1];
	D1 = pData[2];
	D2 = pData[3];
	pSce->Data = pData + 4;
	pFlg = Flg_addr[D0] + (D1 >> 5);
	Bit = D1 & 0x1f;
	Top = 0x80000000U;
	/* retail returns 1 from each case (the constant 1 of the compare is reused as the return value) */
	switch (D2)
	{
	case 0:
		*pFlg &= ~(Top >> Bit);
		return 1;
	case 1:
		*pFlg |= Top >> Bit;
		return 1;
	case 7:
		*pFlg ^= Top >> Bit;
		return 1;
	}
	return 1;
}

/* Compare record: USHORT comparison word at +2 (retail Cmp 0x80054474: lhu v1,0x2(v0)) and USHORT
 * value at +4 (retail reads it with lh v0,0x4 instead; the port keeps lhu so the code is unchanged). */
typedef struct {
	UCHAR Dummy[2];
	USHORT Cmp_word;
	SHORT Cmp_val;
} SCE_CMP_REC;

/* DS 0x80054474 (Cmp) */
ULONG Cmp(SCE_TASK *pSce)
{
	LONG Cmp_val;
	LONG Var_val;
	UINT Cmp_word;
	UCHAR *pData;
	UINT Var_no;

	pData = pSce->Data;
	Cmp_word = ((SCE_CMP_REC *)pData)->Cmp_word;
	Cmp_val = ((SCE_CMP_REC *)pData)->Cmp_val;
	pSce->Data = pData + 6;
	Var_no = Cmp_word & 0xff;
	Var_val = G.SaveData.Sce_work[Var_no - 4];
	switch (Cmp_word >> 8) {
	case CMP_EQ:
		return (ULONG)(Var_val == Cmp_val);
	case CMP_GT:
		return (ULONG)(Cmp_val < Var_val);
	case CMP_GE:
		return (Var_val < Cmp_val) ^ 1;
	case CMP_LT:
		return (ULONG)(Var_val < Cmp_val);
	case CMP_LE:
		return (Cmp_val < Var_val) ^ 1;
	case CMP_NE:
		return (ULONG)(Var_val != Cmp_val);
	case CMP_AND:
		return (ULONG)((Var_val & Cmp_val) != 0);
	}
}

/* Save record: SHORT value at +2 (retail Save 0x8005451c: lh a1,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Set_val;
} SCE_SAVE_REC;

/* DS 0x8005451c (Save) */
ULONG Save(SCE_TASK *pSce)
{
	UCHAR Var_no;
	LONG Set_val;
	UCHAR *pData;

	pData = pSce->Data;
	Var_no = pData[1];
	Set_val = ((SCE_SAVE_REC *)pData)->Set_val;
	pSce->Data = pData + 4;
	G.SaveData.Sce_work[Var_no - 4] = Set_val;
	return 1;
}

/* DS 0x8005454c (Copy) */
ULONG Copy(SCE_TASK *pSce)
{
	UCHAR Dst_no;
	UCHAR Src_no;
	UCHAR *pData;

	pData = pSce->Data;
	Dst_no = pData[1];
	Src_no = pData[2];
	pSce->Data = pData + 3;
	G.SaveData.Sce_work[Dst_no - 4] = G.SaveData.Sce_work[Src_no - 4];
	return 1;
}

/* Calc record (also used by Member_calc): USHORT command word at +2 (retail Calc 0x8005458c: lhu v1,0x2(v0))
 * and SHORT immediate at +4 (retail lh a2,0x4(v0)). */
typedef struct {
	UCHAR Dummy[2];
	USHORT Cmd_word;
	SHORT Imm;
} SCE_CALC_REC;

/* DS 0x8005458c (Calc) */
ULONG Calc(SCE_TASK *pSce)
{
	LONG Imm;
	USHORT Cmd_word;
	UCHAR *pData;

	pData = pSce->Data;
	Cmd_word = ((SCE_CALC_REC *)pData)->Cmd_word;
	Imm = ((SCE_CALC_REC *)pData)->Imm;
	pSce->Data = pData + 6;
	Calc_branch(Cmd_word & 0xff, G.SaveData.Sce_work + ((Cmd_word >> 8) - 4), (int)Imm);
	return 1;
}

/* DS 0x800545d8 (Calc2) */
ULONG Calc2(SCE_TASK *pSce)
{
	/* The Oct 97 mockup declares three LONG locals (D0, D1, D2) here. With LONG locals the Sce_work read is a
	   single lh (DS 0x80054600: lh a2,0x47ec(at)) passed straight to Calc_branch, and the frame is DS's 32 bytes
	   with no extra stack slots. */
	LONG Op_no;
	LONG Dst_no;
	LONG Src_val;
	UCHAR *pData;

	pData = pSce->Data;
	Op_no = pData[1];
	Dst_no = pData[2];
	Src_val = G.SaveData.Sce_work[pData[3] - 4];
	pSce->Data = pData + 4;
	Calc_branch(Op_no, G.SaveData.Sce_work + (Dst_no - 4), Src_val);
	return 1;
}

/* DS 0x80054634 (Calc_branch) */
void Calc_branch(ULONG Exp, short *Ans, long Num)
{
	USHORT Value;

	Value = (USHORT)Num;
	switch (Exp) {
	case CALC_ADD:
		Value = *Ans + Value;
		break;
	case CALC_SUB:
		Value = *Ans - Value;
		break;
	case CALC_MUL:
		*Ans = *Ans * Value;
		return;
	case CALC_DIV:
		{ int q; DIV_S(q, (int)*Ans, Num); *Ans = (short)q; }
		return;
	case CALC_MOD:
		{ int r; MOD_S(r, (int)*Ans, Num); *Ans = (short)r; }
		return;
	case CALC_OR:
		Value = *Ans | Value;
		break;
	case CALC_AND:
		Value = *Ans & Value;
		break;
	case CALC_XOR:
		Value = *Ans ^ Value;
		break;
	case CALC_NOT:
		Value = ~*Ans;
		break;
	case CALC_SL:
		Value = *Ans << (Num & 0x1fU);
		break;
	case CALC_SR_16BIT:
		Value = (USHORT)((int)(UINT)(USHORT)*Ans >> (Num & 0x1fU));
		break;
	case CALC_SR_32BIT:
		Value = (USHORT)((int)*Ans >> (Num & 0x1fU));
		break;
	default:
		return;
	}
	*Ans = Value;
}

/* DS 0x80054760 (Sce_rnd) */
ULONG Sce_rnd(SCE_TASK *pSce)
{
	pSce->Data++;
	Sce_rnd_set();
	return 1;
}

/* DS 0x8005478c (Cut_chg) */
ULONG Cut_chg(SCE_TASK *pSce)
{
	ULONG D0;
	ULONG *pStatus_flg;

	/* DS keeps the address of G.Status_flg in a saved register (s1) across the
	   Cut_chg_main call and reads and writes the flag word through it at offset 0;
	   the empty asm keeps cc1psx from folding the address back into lui/lw pairs. */
	__asm__ ("" : "=r"(pStatus_flg) : "0"(&G.Status_flg));
	*pStatus_flg |= STAT_CAM_LOCK;
	D0 = pSce->Data[1];
	Cut_chg_main(D0 & 0x7f);
	*pStatus_flg |= 0x100000;
	if ((D0 & 0x80) != 0) {
		Main.Bg_chg_flg = 0;
	}
	pSce->Data += 2;
	return 1;
}

/* DS 0x8005481c (Cut_old) */
ULONG Cut_old(SCE_TASK *pSce)
{
	Cut_chg_main((UINT)Sce.Cut_old);
	G.Status_flg = (G.Status_flg | 0x100000) & ~STAT_CAM_LOCK;
	pSce->Data++;
	return 1;
}

/* DS 0x80054884 (Cut_chg_main) */
void Cut_chg_main(ULONG Cut_no)
{
	/* The previous cut is copied straight from the SHORT field: DS loads the whole halfword (lhu) before the new
	   cut is stored and keeps only its low byte in Sce.Cut_old (sb). */
	Sce.Cut_old = G.SaveData.Cut_no;
	G.SaveData.Cut_no = Cut_no;
	Main.Bg_chg_flg = 1;
	Cut_change(Cut_no);
}

/* DS 0x800548c8 (Cut_auto) */
ULONG Cut_auto(SCE_TASK *pSce)
{
	if (pSce->Data[1] == 1) {
		G.Status_flg &= ~STAT_CAM_LOCK;
	}
	else {
		G.Status_flg |= STAT_CAM_LOCK;
	}
	pSce->Data += 2;
	return 1;
}

/* DS 0x80054930 (Cut_replace) */
ULONG Cut_replace(SCE_TASK *pSce)
{
	UINT Cut_a;
	UINT Cut_b;
	UCHAR *pCut;
	UINT cur;

	Cut_a = pSce->Data[1];
	Cut_b = pSce->Data[2];
	pCut = &(G.pRoom)->pVcut->Fcut;
	cur = *pCut;
	while (cur != 0xff) {
		if (cur == Cut_a) {
			*pCut = Cut_b;
		}
		else if (cur == Cut_b) {
			*pCut = Cut_a;
		}
		if (pCut[1] == Cut_a) {
			pCut[1] = Cut_b;
		}
		else if (pCut[1] == Cut_b) {
			pCut[1] = Cut_a;
		}
		pCut += 0x14;
		cur = *pCut;
	}
	if ((G.Cccut_next)->Fcut == Cut_a) {
		Cut_change(Cut_b);
	}
	pSce->Data += 3;
	return 1;
}

/* DS 0x80054a14 (Cut_be_set) */
ULONG Cut_be_set(SCE_TASK *pSce)
{
	UCHAR *pCmd;
	VCUT *pVc;

	pCmd = pSce->Data;
	pVc = (G.pRoom)->pVcut;
	if (pVc->Fcut != pCmd[1]) {
		do {
			pVc++;
		} while (pVc->Fcut != pCmd[1]);
		pCmd = pSce->Data;
	}
	pVc[pCmd[2]].Be_flg = pCmd[3];
	pSce->Data += 4;
	return 1;
}

/* Message record: USHORT stop data at +4 (retail Message_on 0x80054a8c: lhu s0,0x4(v0)) */
typedef struct {
	UCHAR Dummy[4];
	USHORT Stop_hi;
} SCE_MESSAGE_REC;

/* DS 0x80054a8c (Message_on) */
ULONG Message_on(SCE_TASK *pSce)
{
	UCHAR Mess_no;
	UCHAR Attr_b;
	USHORT Stop_hi;
	UCHAR *pData;

	pData = pSce->Data;
	Mess_no = pData[2];
	Attr_b = pData[3];
	Stop_hi = ((SCE_MESSAGE_REC *)pData)->Stop_hi;
	pSce->Data = pData + 6;
	Mess_set(0, Attr_b | 0x300, (UINT)Mess_no, (UINT)Stop_hi << 0x10);
	G.Stop_flg |= (UINT)Stop_hi << 0x10;
	return 1;
}

/* DS 0x80054af4 (Aot_set) */
ULONG Aot_set(SCE_TASK *pSce)
{
	SCE_AOT **ppAot;

	ppAot = &G.Sce_aot[pSce->Data[1]];
	if (*ppAot == (SCE_AOT *)0x0) {
		G.nSce_aot++;
	}
	*ppAot = (SCE_AOT *)(pSce->Data + 2);
	pSce->Data += 0x14;
	return 1;
}

/* DS 0x80054b60 (Aot_set_4p) */
ULONG Aot_set_4p(SCE_TASK *pSce)
{
	SCE_AOT **ppAot;

	ppAot = &G.Sce_aot[pSce->Data[1]];
	if (*ppAot == (SCE_AOT *)0x0) {
		G.nSce_aot++;
	}
	*ppAot = (SCE_AOT *)(pSce->Data + 2);
	pSce->Data[3] |= SCE_POINT4;
	pSce->Data += 0x1c;
	return 1;
}

/* DS 0x80054be4 (Door_aot_set) */
ULONG Door_aot_set(SCE_TASK *pSce)
{
	SCE_AOT **ppAot;

	/* DS forms the address of the Sce_aot slot once and keeps it in a register for both the
	 * empty-slot test and the store of the new AOT pointer. */
	ppAot = &G.Sce_aot[pSce->Data[1]];
	if (*ppAot == (SCE_AOT *)0x0) {
		G.nSce_aot++;
	}
	*ppAot = (SCE_AOT *)(pSce->Data + 2);
	pSce->Data += 0x20;
	return 1;
}

/* DS 0x80054c50 (Door_aot_set_4p) */
ULONG Door_aot_set_4p(SCE_TASK *pSce)
{
	SCE_AOT **ppAot;

	/* DS forms the address of the Sce_aot slot once (v1) and uses it for both the NULL test and the store. */
	ppAot = &G.Sce_aot[pSce->Data[1]];
	if (*ppAot == (SCE_AOT *)0x0) {
		G.nSce_aot++;
	}
	*ppAot = (SCE_AOT *)(pSce->Data + 2);
	pSce->Data[3] |= SCE_POINT4;
	pSce->Data += 0x28;
	return 1;
}

/* Item_aot record (Item_aot_set and Item_aot_set2): USHORT flag word at +18 (retail Item_aot_set 0x80054cd4:
 * lhu a1,0x12(s0)); the other bytes of the record are read as bytes by the handler. */
typedef struct {
	UCHAR Dummy[18];
	USHORT Flg_bit;
} SCE_ITEM_AOT_REC;

/* DS 0x80054cd4 (Item_aot_set) */
ULONG Item_aot_set(SCE_TASK *pSce)
{
	/* retail reserves 8 more bytes of locals (Item_aot_set frame 48); never read */
	volatile int Rsv[2];
	UCHAR Work_a;
	USHORT Flg_bit;
	ULONG Ret;
	ULONG *p;
	UCHAR *pData;
	UINT Ob_no;
	tagGLOBAL *pG;


	pG = &G;
	pData = pSce->Data;
	Flg_bit = ((SCE_ITEM_AOT_REC *)pData)->Flg_bit;
	Ob_no = (UINT)pData[0x14];
	pSce->Data = pData + 0x16;
	Work_a = pData[1];
	if (pG->Sce_aot[Work_a] == (SCE_AOT *)0x0) {
		pG->nSce_aot++;
	}
	pG->Sce_aot[Work_a] = (SCE_AOT *)(pData + 2);
	if (3 < pG->SaveData.Stage_no) {
		p = Flg_addr[FLAG_ITEM_2];
	}
	else {
		p = Flg_addr[FLAG_ITEM];
	}
	Ret = Flg_ck((ULONG *)p, (UINT)Flg_bit);
	if (Ret != 0) {
		pG->Sce_aot[Work_a]->Id = SCE_NOTHING;
		if (Ob_no < 0x20) {
			pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			pG->Ob_model[Ob_no].Free0 = 0;
		}
	}
	else {
		if (Ob_no < 0x20) {
			Work_a = pData[0x15];
			pG->Ob_model[Ob_no].Free0 = Work_a;
			if ((Work_a & ITEM_ACTION_BE_FLG) != 0) {
				pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			}
		}
	}
	return 1;
}

/* DS 0x80054e14 (Item_aot_set2) */
ULONG Item_aot_set2(SCE_TASK *pSce)
{
	/* retail reserves 8 more bytes of locals (Item_aot_set2 frame 64); never read */
	volatile int Rsv[2];
	UCHAR Work_a;
	UCHAR Chance;
	USHORT Flg_bit;
	ULONG Ret;
	int Rnd_val;
	ULONG *p;
	UCHAR *pData;
	ULONG *pFlg_nrm;
	UINT Ob_no;
	tagGLOBAL *pG;
	ULONG *pFlg_alt;

	pG = &G;
	pData = pSce->Data;
	Flg_bit = ((SCE_ITEM_AOT_REC *)pData)->Flg_bit;
	Ob_no = (UINT)pData[0x14];
	pSce->Data = pData + 0x18;
	Work_a = pData[1];
	if (pG->Sce_aot[Work_a] == (SCE_AOT *)0x0) {
		pG->nSce_aot++;
	}
	pG->Sce_aot[Work_a] = (SCE_AOT *)(pData + 2);
	if (3 < pG->SaveData.Stage_no) {
		pFlg_nrm = Flg_addr[FLAG_ITEM_2];
		pFlg_alt = Flg_addr[FLAG_EX_1];
	}
	else {
		pFlg_nrm = Flg_addr[FLAG_ITEM];
		pFlg_alt = Flg_addr[FLAG_EX_0];
	}
	Ret = Stage_room_to_mapno((int)pG->SaveData.Stage_no, (int)pG->SaveData.Room_no);
	Ret = Flg_ck(pG->SaveData.Map_flg, Ret);
	if (Ret == 0) {
		Chance = pData[0x16];
		Rnd_val = Rnd_s32() * 100;
		if (Rnd_val < 0) {
			Rnd_val += 0xff;
		}
		p = pFlg_nrm;
		if ((UINT)(Rnd_val >> 8) < (UINT)Chance) {
			p = pFlg_alt;
		}
		Flg_on((ULONG *)p, (UINT)Flg_bit);
	}
	Ret = Flg_ck((ULONG *)pFlg_nrm, (UINT)Flg_bit);
	if (Ret != 0) {
		pG->Sce_aot[Work_a]->Id = SCE_NOTHING;
		if (Ob_no < 0x20) {
			pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			pG->Ob_model[Ob_no].Free0 = 0;
		}
	}
	else {
		if (Ob_no < 0x20) {
			Work_a = pData[0x15];
			pG->Ob_model[Ob_no].Free0 = Work_a;
			if ((Work_a & ITEM_ACTION_BE_FLG) != 0) {
				pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			}
		}
	}
	return 1;
}

/* Item_aot_set_4p record: USHORT flag word at +26 (retail Item_aot_set_4p 0x80055008: lhu a1,0x1a(s0)) */
typedef struct {
	UCHAR Dummy[26];
	USHORT Flg_bit;
} SCE_ITEM_AOT4_REC;

/* DS 0x80055008 (Item_aot_set_4p) */
ULONG Item_aot_set_4p(SCE_TASK *pSce)
{
	/* retail reserves 8 more bytes of locals (Item_aot_set_4p frame 48); never read */
	volatile int Rsv[2];
	UCHAR Work_a;
	USHORT Flg_bit;
	ULONG Ret;
	ULONG *p;
	UCHAR *pData;
	UINT Ob_no;
	tagGLOBAL *pG;


	pG = &G;
	pData = pSce->Data;
	Flg_bit = ((SCE_ITEM_AOT4_REC *)pData)->Flg_bit;
	Ob_no = (UINT)pData[0x1c];
	pData[3] |= SCE_POINT4;
	pSce->Data = pData + 0x1e;
	Work_a = pData[1];
	if (pG->Sce_aot[Work_a] == (SCE_AOT *)0x0) {
		pG->nSce_aot++;
	}
	pG->Sce_aot[Work_a] = (SCE_AOT *)(pData + 2);
	if (3 < pG->SaveData.Stage_no) {
		p = Flg_addr[FLAG_ITEM_2];
	}
	else {
		p = Flg_addr[FLAG_ITEM];
	}
	Ret = Flg_ck((ULONG *)p, (UINT)Flg_bit);
	if (Ret != 0) {
		pG->Sce_aot[Work_a]->Id = SCE_NOTHING;
		if (Ob_no < 0x20) {
			pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			pG->Ob_model[Ob_no].Free0 = 0;
		}
	}
	else {
		if (Ob_no < 0x20) {
			Work_a = pData[0x1d];
			pG->Ob_model[Ob_no].Free0 = Work_a;
			if ((Work_a & ITEM_ACTION_BE_FLG) != 0) {
				pG->Ob_model[Ob_no].Be_flg = 0x80000000;
			}
		}
	}
	return 1;
}

/* Aot_reset record: USHORT Id_type at +4 and USHORT nFloor at +6 (stored halfword-wise into pSce_aot->nFloor,
 * i.e. nFloor and Super), SHORT X_s at +8 (retail Aot_reset 0x80055154: lh v0,0x6(a2); lh a3,0x4(a2); lh v1,0x8(a2)). */
typedef struct {
	UCHAR Dummy[4];
	USHORT Id_type;
	USHORT nFloor;
	SHORT X_s;
} SCE_AOT_RESET_REC;

/* DS 0x80055154 (Aot_reset) */
ULONG Aot_reset(SCE_TASK *pSce)
{
	UCHAR Old_type;
	USHORT Id_type;
	short X_s;
	SCE_AOT *pSce_aot;
	UCHAR *pData;

	pData = pSce->Data;
	pSce_aot = G.Sce_aot[(signed char)pData[1]];
	Old_type = pSce_aot->Type;
	pSce_aot->Id = pData[2];
	pSce_aot->Type = pData[3] | (Old_type & SCE_POINT4);
	if ((Old_type & SCE_POINT4) == 0) {
		/* data part at retail +0xc (SCE_AOT is 0xe bytes, so +1 would be wrong) */
		pSce_aot = (SCE_AOT *)&pSce_aot->Data;
	}
	else {
		/* retail +0x14: W field of the data part */
		pSce_aot = (SCE_AOT *)&((SCE_AOT *)&pSce_aot->Data)->W;
	}
	Id_type = ((SCE_AOT_RESET_REC *)pData)->Id_type;
	X_s = ((SCE_AOT_RESET_REC *)pData)->X_s;
	*(USHORT *)&pSce_aot->nFloor = ((SCE_AOT_RESET_REC *)pData)->nFloor;
	pSce_aot->Id = (char)Id_type;
	pSce_aot->Type = (char)(Id_type >> 8);
	pSce_aot->X = X_s;
	pSce->Data = pData + 10;
	return 1;
}

/* DS 0x800551c8 (Aot_on) */
ULONG Aot_on(SCE_TASK *pSce)
{
	SCE_AOT *pArg;

	pCAot = G.Sce_aot[pSce->Data[1]];
	/* retail +0x14 (W field of the data part) */
	pArg = (SCE_AOT *)&((SCE_AOT *)&pCAot->Data)->W;
	if ((pCAot->Type & SCE_POINT4) == 0) {
		/* retail +0xc (data part) */
		pArg = (SCE_AOT *)&pCAot->Data;
	}
	/* Called through the table's own SCE_AT_FUNC type (ULONG return, result unused), as SCE_AT.C does. DS
	   (0x8005523c) then reloads pSce->Data into v1 and sets the return value 1 in the load delay slot. */
	Sce_at_jump_tbl[pCAot->Id](pArg);
	pSce->Data += 2;
	return 1;
}

/* Object-model record of Obj_model_set (0x26 bytes). Bytes 0..7 are still read as bytes by the handler.
 * Retail Obj_model_set 0x80055260: lhu a3,0x8(s2); lhu v0,0xa(s2); lh a1,0xc(s2); lh a2,0xe(s2); lh a3,0x10(s2);
 * lh v0,0x12(s2); lh a1,0x14(s2)...; lh a1,0x1a(s2); lh a1,0x20(s2) (retail reads +0x20..+0x24 with lh, the port with lhu). */
typedef struct {
	UCHAR Dummy[8];
	USHORT Work_d;		/* 0x08 */
	USHORT Be_flg;		/* 0x0a */
	SHORT Work_a;		/* 0x0c */
	SHORT Work_b;		/* 0x0e */
	SHORT Pos_y_s;		/* 0x10 */
	SHORT Pos_z;		/* 0x12 */
	SHORT Cdir_x;		/* 0x14 */
	SHORT Cdir_y;		/* 0x16 */
	SHORT Cdir_z;		/* 0x18 */
	SHORT Ofs_x;		/* 0x1a */
	SHORT Ofs_y;		/* 0x1c */
	SHORT Ofs_z;		/* 0x1e */
	SHORT At_w;		/* 0x20 */
	SHORT Box_h;		/* 0x22 */
	SHORT Box_d;		/* 0x24 */
} SCE_OBJ_SET_REC;

/* DS 0x80055260 (Obj_model_set) */
ULONG Obj_model_set(SCE_TASK *pSce)
{
	UCHAR Work_c;
	UCHAR Sup_flg;
	UCHAR Cc_n;
	LONG Work_a;
	LONG Work_b;
	LONG Pos_y_s;
	LONG Box_h;
	LONG Box_d;
	ULONG Work_d;
	ULONG Old_be_flg;
	short (*pMtx)[3];
	UINT Sup_mode;
	ROOM_WORK *pTmd_adr;
	UINT Work_e;
	UINT Om_no;
	OBJ_MODEL_WORK *pOm;
	UCHAR *pData;
	CC_PARTS_WORK *pCc;
	tagGLOBAL *pG;

	pG = &G;
	pData = pSce->Data;
	Work_c = pData[6];
	Om_no = (UINT)pData[1];
	Work_d = ((SCE_OBJ_SET_REC *)pData)->Work_d;
	pOm = pG->Ob_model + Om_no;
	Old_be_flg = pOm->Be_flg;
	pOm->Id = pData[2];
	pOm->nFloor = Work_c;
	pOm->Type = Work_d;
	Sup_flg = pData[7];
	if (Old_be_flg != 0) {
		pOm->Be_flg = 0;
	}
	else {
		pOm->Be_flg = (int)(short)(((SCE_OBJ_SET_REC *)pData)->Be_flg | OM_ENABLED);
	}
	/* retail reads pObj_work_end through its own address (lui+addiu), the other fields through the G base register */
	if (G.pObj_work_end < pOm + 1) {
		G.pObj_work_end = pOm + 1;
	}
	Work_a = ((SCE_OBJ_SET_REC *)pData)->Work_a;
	Work_b = ((SCE_OBJ_SET_REC *)pData)->Work_b;
	Pos_y_s = ((SCE_OBJ_SET_REC *)pData)->Pos_y_s;
	pOm->Pos_z = (int)((SCE_OBJ_SET_REC *)pData)->Pos_z;
	pOm->Attribute = (int)Work_a;
	pOm->Pos_x = (int)Work_b;
	pOm->Pos_y = (int)Pos_y_s;
	if (((int)Work_a & OM_DISABLED_2) != 0) {
		pOm->Be_flg &= ~OM_ENABLED;
	}
	Work_a = ((SCE_OBJ_SET_REC *)pData)->Cdir_y;
	Work_b = ((SCE_OBJ_SET_REC *)pData)->Cdir_z;
	pOm->Cdir_x = ((SCE_OBJ_SET_REC *)pData)->Cdir_x;
	pOm->Cdir_y = Work_a;
	pOm->Cdir_z = Work_b;
	Work_a = ((SCE_OBJ_SET_REC *)pData)->Ofs_y;
	Work_b = ((SCE_OBJ_SET_REC *)pData)->Ofs_z;
	pOm->Atd[0].Ofs_x = ((SCE_OBJ_SET_REC *)pData)->Ofs_x;
	pOm->Atd[0].Ofs_y = Work_a;
	pOm->Atd[0].Ofs_z = Work_b;
	Work_d = ((SCE_OBJ_SET_REC *)pData)->At_w;
	Box_h = ((SCE_OBJ_SET_REC *)pData)->Box_h;
	Box_d = ((SCE_OBJ_SET_REC *)pData)->Box_d;
	Sup_mode = Sup_flg & 0xc0;
	pOm->Parts.Be_flg = 1;
	pOm->Atd[0].At_w = Work_d;
	pOm->Atd[0].At_h = Box_h;
	pOm->Atd[0].At_d = Box_d;
	/* retail: a switch on the two high bits of the super flag (decision tree 0x40, then 0, 0x80, 0xc0) */
	switch (Sup_mode) {
	case 0:
		pOm->pSuper = (ULONG *)&GsIDMATRIX;
		break;
	case SUPER_WORK_TYPE_PLAYER:
		pOm->pSuper = (ULONG *)pG->Player.M;
		break;
	case 0x80:
		pOm->pSuper = (ULONG *)pG->pEnemy[Sup_flg & 0x3f]->M;
		break;
	case 0xc0:
		pOm->pSuper = (ULONG *)&pG->Ob_model[Sup_flg & 0x3f].Workm;
		break;
	}
	pTmd_adr = (ROOM_WORK *)(pG->pRoom)->pOmodel[Om_no].Tmd_adr;
	if (pTmd_adr != pG->pRoom) {
		Om_init(Om_no, (ULONG *)pTmd_adr);
	}
	else {
		pOm->Be_flg = 0;
	}
	pMtx = pOm->M;
	RotMatrix((SVECTOR *)&pOm->Cdir_x, (MATRIX *)pMtx);
	CompM((MATRIX *)pOm->pSuper, (MATRIX *)pMtx, &pOm->Workm);
	Work_c = pData[4];
	Cc_n = pData[5];
	if ((pData[3] & 0xc0) != 0) {
		Work_e = (UINT)Cc_work.Ccol_no;
		pCc = &Cc_work.Cc_parts[Work_e];
		pCc->Cc_ctr = pData[3];
		pCc->Cc_wait = Work_c;
		pCc->Cc_num = Cc_n;
		pCc->Cc_cnt = 0;
		pCc->pOm = pOm;
		Cc_work.Ccol_no++;
	}
	Oma_set_ofs(pOm);
	pSce->Data = pData + 0x26;
	return 1;
}

/* Super_set record: SHORT position at +4..+8 and SHORT direction at +10..+14
 * (retail Super_set 0x8005550c: lh v0,0x4(a3); lh v1,0x6(a3); lh a0,0x8(a3); lh v0,0xa(a3); lh v1,0xc(a3); lh a0,0xe(a3)). */
typedef struct {
	UCHAR Dummy[4];
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Cdir_x;
	SHORT Cdir_y;
	SHORT Cdir_z;
} SCE_SUPER_SET_REC;

/* DS 0x8005550c (Super_set) */
ULONG Super_set(SCE_TASK *pSce)
{
	UCHAR Sup_no;
	short Work_a;
	short Work_b;
	MATRIX *pMat;
	UINT Sup_val;
	UINT Sup_mode;
	ENEMY_WORK *pEm;
	UCHAR *pData;

	pData = pSce->Data;
	pEm = pSce->pWork;
	Sup_val = (UINT)pData[2];
	Sup_no = pData[3];
	Sup_mode = Sup_val & 0xc0;
	if (Sup_mode == SUPER_WORK_TYPE_PLAYER) {
		if ((Sup_no & 0x80) == 0) {
			pMat = (MATRIX *)G.Player.M;
		}
		else {
			pMat = &G.Player.pSin_parts_ptr[Sup_no & 0x7f].Workm;
		}
	}
	else {
		if (Sup_mode < 0x41) {
			if ((pData[2] & 0xc0) == 0) {
				pEm->pSuper = (ULONG *)&GsIDMATRIX;
			}
			goto work_params;
		}
		/* Sup_mode is 0x80 in this branch, so Sup_val - 0x80 is the enemy number (Sup_val & 0x3f) */
		if (Sup_mode == 0x80) {
			if ((Sup_no & 0x80) != 0) {
				pEm->pSuper = (ULONG *)&G.pEnemy[Sup_val - 0x80]->pSin_parts_ptr[Sup_no & 0x7f].Workm;
				goto work_params;
			}
			pMat = (MATRIX *)&G.pEnemy[Sup_val - 0x80]->M;
		}
		else {
			if (Sup_mode != 0xc0) goto work_params;
			pMat = &G.Ob_model[Sup_val - 0xc0].Workm;
		}
	}
	pEm->pSuper = (ULONG *)pMat;
work_params:
	Work_a = ((SCE_SUPER_SET_REC *)pData)->Pos_y;
	Work_b = ((SCE_SUPER_SET_REC *)pData)->Pos_z;
	pEm->Pos_x = (int)((SCE_SUPER_SET_REC *)pData)->Pos_x;
	pEm->Pos_y = (int)Work_a;
	pEm->Pos_z = (int)Work_b;
	Work_a = ((SCE_SUPER_SET_REC *)pData)->Cdir_y;
	Work_b = ((SCE_SUPER_SET_REC *)pData)->Cdir_z;
	pEm->Cdir_x = ((SCE_SUPER_SET_REC *)pData)->Cdir_x;
	pEm->Cdir_y = Work_a;
	pEm->Cdir_z = Work_b;
	pSce->Data = pData + 0x10;
	return 1;
}

/* Super_on record: SHORT location t[] at +4..+8 and SHORT rotation vx, vy, vz at +10..+14
 * (retail Super_on 0x80055680: lh v0,0x4(s3); lh v1,0x6(s3); lh a2,0x8(s3); lh v0,0xa(s3); lh v1,0xc(s3); lh a2,0xe(s3)). */
typedef struct {
	UCHAR Dummy[4];
	SHORT t[3];
	SHORT vx;
	SHORT vy;
	SHORT vz;
} SCE_SUPER_ON_REC;

/* DS 0x80055680 (Super_on) */
ULONG Super_on(SCE_TASK *pSce)
{
	UCHAR Sup_no;
	UINT Sup_mode;
	UINT Sup_val;
	MATRIX *pMat;
	ENEMY_WORK *pEm;
	UCHAR *pData;
	MATRIX Loc_mtx;
	MATRIX World_mtx;
	SVECTOR Rot_v;
	SVECTOR Dir_w;

	pData = pSce->Data;
	Sup_val = (UINT)pData[2];
	Sup_no = pData[3];
	Sup_mode = Sup_val & 0xc0;
	if (Sup_mode == SUPER_WORK_TYPE_PLAYER) {
		if ((Sup_no & 0x80) == 0) {
			pMat = (MATRIX *)G.Player.M;
		}
		else {
			pMat = (MATRIX *)&G.Player.pSin_parts_ptr[Sup_no & 0x7f].Workm;
		}
	}
	else if (Sup_mode == SUPER_WORK_TYPE_IDMATRIX) {
		pMat = &GsIDMATRIX;
	}
	else if (Sup_mode == 0x80) {
		if ((Sup_no & 0x80) == 0) {
			pMat = (MATRIX *)&G.pEnemy[Sup_val & 0x3f]->M;
		}
		else {
			pMat = (MATRIX *)&G.pEnemy[Sup_val & 0x3f]->pSin_parts_ptr[Sup_no & 0x7f].Workm;
		}
	}
	else if (Sup_mode == 0xc0) {
		pMat = &G.Ob_model[Sup_val - 0xc0].Workm;
	}
	Loc_mtx.t[0] = ((SCE_SUPER_ON_REC *)pData)->t[0];
	Loc_mtx.t[1] = ((SCE_SUPER_ON_REC *)pData)->t[1];
	Loc_mtx.t[2] = ((SCE_SUPER_ON_REC *)pData)->t[2];
	Rot_v.vx = ((SCE_SUPER_ON_REC *)pData)->vx;
	Rot_v.vy = ((SCE_SUPER_ON_REC *)pData)->vy;
	Rot_v.vz = ((SCE_SUPER_ON_REC *)pData)->vz;
	RotMatrix(&Rot_v, (MATRIX *)&Loc_mtx);
	CompM(pMat, &Loc_mtx, &World_mtx);
	pEm = pSce->pWork;
	pEm->Pos_x = World_mtx.t[0];
	pEm->Pos_y = World_mtx.t[1];
	pEm->Pos_z = World_mtx.t[2];
	Rot_v.vx = 1000;
	Rot_v.vy = 0;
	Rot_v.vz = 0;
	ApplyMatrixSV((MATRIX *)&World_mtx, &Rot_v, (SVECTOR *)&Dir_w);
	Bow_3D(&Dir_w, (SVECTOR *)&Rot_v);
	pEm->Cdir_x = Rot_v.vx;
	pEm->Cdir_y = Rot_v.vy;
	pEm->Cdir_z = Rot_v.vz + 0x400;
	pSce->Data = pData + 0x10;
	return 1;
}

/* Super_reset record: SHORT Cdir_x at +2 and SHORT Dir_y_s, Dir_z_s at +4, +6
 * (retail Super_reset 0x800558b0: lh v1,0x2(a1); lh a2,0x4(a1); lh a3,0x6(a1)). */
typedef struct {
	UCHAR Dummy[2];
	SHORT Cdir_x;
	SHORT Dir_y_s;
	SHORT Dir_z_s;
} SCE_SUPER_RESET_REC;

/* DS 0x800558b0 (Super_reset) */
ULONG Super_reset(SCE_TASK *pSce)
{
	LONG Cdir_x;		/* Cdir_x read from the record (kept signed; a direct copy loads it unsigned) */
	LONG Dir_y_s;		/* Cdir_y source, read from the record */
	LONG Dir_z_s;		/* Cdir_z source, read from the record */
	ENEMY_WORK *pEm;
	UCHAR *pData;
	long Wm_x;
	long Wm_y;
	long Wm_z;

	pEm = pSce->pWork;
	pData = pSce->Data;
	Wm_x = pEm->Workm.t[0];
	Wm_y = pEm->Workm.t[1];
	Wm_z = pEm->Workm.t[2];
	pEm->pSuper = (ULONG *)&GsIDMATRIX;
	pEm->Pos_x = Wm_x;
	pEm->Pos_y = Wm_y;
	pEm->Pos_z = Wm_z;
	Dir_y_s = ((SCE_SUPER_RESET_REC *)pData)->Dir_y_s;
	Dir_z_s = ((SCE_SUPER_RESET_REC *)pData)->Dir_z_s;
	Cdir_x = ((SCE_SUPER_RESET_REC *)pData)->Cdir_x;
	pEm->Cdir_x = Cdir_x;
	pEm->Cdir_y = Dir_y_s;
	pEm->Cdir_z = Dir_z_s;
	pSce->Data = pData + 8;
	return 1;
}

/* DS 0x80055904 (Work_set) */
ULONG Work_set(SCE_TASK *pSce)
{
	UCHAR Kind;
	UCHAR *pData;
	ENEMY_WORK *pEm;
	int Idx;

	pData = pSce->Data;
	/* Retail clears the twelve speed fields as six word stores (sw zero at +0x158 to +0x16c); each ULONG store below clears two adjacent SHORT fields. */
	*(ULONG *)&pSce->Spd_x = 0;
	*(ULONG *)&pSce->Spd_z = 0;
	*(ULONG *)&pSce->Dspd_y = 0;
	*(ULONG *)&pSce->Aspd_x = 0;
	*(ULONG *)&pSce->Aspd_z = 0;
	*(ULONG *)&pSce->Adspd_y = 0;
	Kind = pData[1];
	Idx = (int)(signed char)pData[2];
	pSce->Data = pData + 3;
	switch (Kind) {
	case WORK_TYPE_PLAYER:
		pSce->pWork = G.pEnemy[0];
		break;
	case WORK_TYPE_SUB_PLAYER:
		pSce->pWork = G.pEnemy[1];
		break;
	case WORK_TYPE_ENEMY:
		pSce->pWork = G.pEnemy[Idx + 2];
		break;
	case WORK_TYPE_OBJECT:
		pEm = (ENEMY_WORK *)(G.Ob_model + Idx);
		pSce->pWork = pEm;
		break;
	case WORK_TYPE_DOOR:
		pSce->pWork = (ENEMY_WORK *)pDwork[Idx];
		break;
	}
	return 1;
}

/* Parts record: SHORT value at +4 (retail Parts_set 0x800559d0: lhu a0,0x4(a0)) */
typedef struct {
	UCHAR Dummy[4];
	SHORT Parts_v;
} SCE_PARTS_REC;

/* DS 0x800559d0 (Parts_set) */
ULONG Parts_set(SCE_TASK *pSce)
{
	short Parts_v;
	PARTS_W *pParts;
	UCHAR *pData;

	pData = pSce->Data;
	Parts_v = ((SCE_PARTS_REC *)pData)->Parts_v;
	pParts = pSce->pWork->pSin_parts_ptr + (signed char)pData[2];
	switch ((signed char)pData[3]) {
	case PARTS_BE_FLAG0:
		*(short *)&pParts->Be_flg = Parts_v;
		break;
	case PARTS_BE_FLAG1:
		*((short *)&pParts->Be_flg + 1) = Parts_v;
		break;
	case PARTS_POLY_RGB0:
		*(short *)&pParts->Poly_rgb = Parts_v;
		break;
	case PARTS_POLY_RGB1:
		((CVECTOR *)&pParts->Poly_rgb)->b = (char)Parts_v;
		break;
	case PARTS_CDIR_X:
		pParts->Cdir_x = Parts_v;
		break;
	case PARTS_CDIR_Y:
		pParts->Cdir_y = Parts_v;
		break;
	case PARTS_CDIR_Z:
		pParts->Cdir_z = Parts_v;
		break;
	}
	pSce->Data += 6;
	return 1;
}

/* Speed record: USHORT value at +2 (retail Speed_set 0x80055a84: lhu a1,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Spd_val;
} SCE_SPEED_REC;

/* DS 0x80055a84 (Speed_set) */
ULONG Speed_set(SCE_TASK *pSce)
{
	UCHAR Spd_idx;
	USHORT Spd_val;
	UCHAR *pData;

	pData = pSce->Data;
	Spd_idx = pData[1];
	Spd_val = ((SCE_SPEED_REC *)pData)->Spd_val;
	pSce->Data = pData + 4;
	/* Spd_x, Spd_y, Spd_z, Dspd_x ... Adspd_z are consecutive SHORTs in SCE_TASK */
	((SHORT *)&pSce->Spd_x)[Spd_idx] = (SHORT)Spd_val;
	return 1;
}

/* DS 0x80055ab0 (Add_speed) */
ULONG Add_speed(SCE_TASK *pSce)
{
	ULONG S0;
	ULONG S1;
	ULONG S2;
	ENEMY_WORK *pEm;

	pEm = pSce->pWork;
	S0 = pSce->Spd_x;
	S1 = pSce->Spd_y;
	S2 = pSce->Spd_z;
	pEm->Pos_x += S0;
	pEm->Pos_y += S1;
	pEm->Pos_z += S2;
	S0 = pSce->Dspd_x;
	S1 = pSce->Dspd_y;
	S2 = pSce->Dspd_z;
	pEm->Cdir_x += S0;
	pEm->Cdir_y += S1;
	pEm->Cdir_z += S2;
	pSce->Data++;
	return 1;
}

/* DS 0x80055b2c (Add_aspeed) */
ULONG Add_aspeed(SCE_TASK *pSce)
{
	pSce->Spd_x += pSce->Aspd_x;
	pSce->Spd_y += pSce->Aspd_y;
	pSce->Spd_z += pSce->Aspd_z;
	pSce->Dspd_x += pSce->Adspd_x;
	pSce->Dspd_y += pSce->Adspd_y;
	pSce->Dspd_z += pSce->Adspd_z;
	pSce->Data++;
	return 1;
}

/* Pos_set record: SHORT Pos_x at +2, SHORT Pos_y_s and Pos_z_s at +4, +6
 * (retail Pos_set 0x80055ba0: lh a1,0x2(v0); lh a2,0x4(v0); lh a3,0x6(v0)). */
typedef struct {
	UCHAR Dummy[2];
	SHORT Pos_x;
	SHORT Pos_y_s;
	SHORT Pos_z_s;
} SCE_POS_REC;

/* DS 0x80055ba0 (Pos_set) */
ULONG Pos_set(SCE_TASK *pSce)
{
	LONG S0;
	LONG S1;
	LONG S2;
	UCHAR *pData;
	ENEMY_WORK *pEm;

	pData = pSce->Data;
	pEm = pSce->pWork;
	S0 = ((SCE_POS_REC *)pData)->Pos_x;
	S1 = ((SCE_POS_REC *)pData)->Pos_y_s;
	S2 = ((SCE_POS_REC *)pData)->Pos_z_s;
	pData += 8;
	pEm->Pos_x = S0;
	pEm->Pos_y = S1;
	pEm->Pos_z = S2;
	pSce->Data = pData;
	return 1;
}

/* Dir_set record: SHORT Cdir_x at +2, SHORT Dir_y_s and Dir_z_s at +4, +6
 * (retail Dir_set 0x80055bd0: lh a1,0x2(v0); lh a2,0x4(v0); lh a3,0x6(v0)). */
typedef struct {
	UCHAR Dummy[2];
	SHORT Cdir_x;
	SHORT Dir_y_s;
	SHORT Dir_z_s;
} SCE_DIR_REC;

/* DS 0x80055bd0 (Dir_set) */
ULONG Dir_set(SCE_TASK *pSce)
{
	LONG Cdir_x;		/* Cdir_x read from the record (kept signed; a direct copy loads it unsigned) */
	LONG Dir_y_s;		/* Cdir_y source, read from the record */
	LONG Dir_z_s;		/* Cdir_z source, read from the record */
	UCHAR *pData;
	ENEMY_WORK *pEm;

	pData = pSce->Data;
	pEm = pSce->pWork;
	Dir_y_s = ((SCE_DIR_REC *)pData)->Dir_y_s;
	Dir_z_s = ((SCE_DIR_REC *)pData)->Dir_z_s;
	Cdir_x = ((SCE_DIR_REC *)pData)->Cdir_x;
	pEm->Cdir_x = Cdir_x;
	pEm->Cdir_y = Dir_y_s;
	pEm->Cdir_z = Dir_z_s;
	pSce->Data = pData + 8;
	return 1;
}

/* Member_set record: SHORT value at +2 (retail Member_set 0x80055c00: lh a2,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT D0;
} SCE_MEMBER_SET_REC;

/* DS 0x80055c00 (Member_set) */
ULONG Member_set(SCE_TASK *pSce)
{
	Member_set_branch(pSce->pWork, (UINT)pSce->Data[1], (int)((SCE_MEMBER_SET_REC *)pSce->Data)->D0);
	pSce->Data += 4;
	return 1;
}

/* DS 0x80055c50 (Member_set2) */
ULONG Member_set2(SCE_TASK *pSce)
{
	Member_set_branch(pSce->pWork, (UINT)pSce->Data[1], (int)G.SaveData.Sce_work[pSce->Data[2] - 4]);
	pSce->Data += 3;
	return 1;
}

/* DS 0x80055cb0 (Member_set_branch) */
void Member_set_branch(ENEMY_WORK *pW, ULONG Member, long D0)
{
	UCHAR Val_b;
	USHORT Val_h;

	Val_b = (UCHAR)D0;
	Val_h = (USHORT)D0;
	switch (Member) {
	case MEMBER_BE_FLAG0:		*(USHORT *)&pW->Be_flg = Val_h; break;
	case MEMBER_BE_FLAG1:		*((USHORT *)&pW->Be_flg + 1) = Val_h; break;
	case MEMBER_ROUTINE0:		pW->Routine_0 = Val_b; break;
	case MEMBER_ROUTINE1:		pW->Routine_1 = Val_b; break;
	case MEMBER_ROUTINE2:		pW->Routine_2 = Val_b; break;
	case MEMBER_ROUTINE3:		pW->Routine_3 = Val_b; break;
	case MEMBER_ID:				pW->Id = Val_b; break;
	case MEMBER_TYPE:			pW->Type = Val_h; break;
	case MEMBER_AT_OBJ_NO:		pW->At_obj_no = Val_b; break;
	case MEMBER_AT_SCE_NO:		pW->At_sce_no = Val_b; break;
	case MEMBER_ATTRIBUTE:		pW->Attribute = D0; break;
	case MEMBER_POS_X:			pW->Pos_x = D0; break;
	case MEMBER_POS_Y:			pW->Pos_y = D0; break;
	case MEMBER_POS_Z:			pW->Pos_z = D0; break;
	case MEMBER_CDIR_X:			pW->Cdir_x = Val_h; break;
	case MEMBER_CDIR_Y:			pW->Cdir_y = Val_h; break;
	case MEMBER_CDIR_Z:			pW->Cdir_z = Val_h; break;
	case MEMBER_NFLOOR:			pW->nFloor = Val_b; break;
	case MEMBER_STATUS_FLAG:	pW->Status_flg = Val_h; break;
	case MEMBER_GROUND:			pW->Ground = Val_h; break;
	case MEMBER_DEST_X:			pW->Dest_x = Val_h; break;
	case MEMBER_DEST_Z:			pW->Dest_z = Val_h; break;
	case MEMBER_SCE_FLAG:		pW->Sce_flg = Val_h; break;
	case MEMBER_SCE_FREE0:		pW->Sce_free0 = Val_h; break;
	case MEMBER_SCE_FREE1:		pW->Sce_free1 = Val_h; break;
	case MEMBER_SCE_FREE2:		pW->Sce_free2 = Val_h; break;
	case MEMBER_SCE_FREE3:		pW->Sce_free3 = Val_h; break;
	case MEMBER_SPD_X_BYTE:		*(UCHAR *)&pW->Spd_x = Val_b; break;
	case MEMBER_SPD_X:			pW->Spd_x = Val_h; break;
	case MEMBER_SPD_Y:			pW->Spd_y = Val_h; break;
	case MEMBER_SPD_Z:			pW->Spd_z = Val_h; break;
	case MEMBER_HOKAN_FLAG:		pW->Hokan_flg = Val_b; break;
	case MEMBER_OFS_X:			pW->Atd[0].Ofs_x = Val_h; break;
	case MEMBER_OFS_Y:			pW->Atd[0].Ofs_y = Val_h; break;
	case MEMBER_OFS_Z:			pW->Atd[0].Ofs_z = Val_h; break;
	case MEMBER_AT_W:			pW->Atd[0].At_w = Val_h; break;
	case MEMBER_AT_H:			pW->Atd[0].At_h = Val_h; break;
	case MEMBER_AT_D:			pW->Atd[0].At_d = Val_h; break;
	case MEMBER_PARTS0_POS_Y:	pW->Parts0_pos_y = Val_h; break;
	case MEMBER_SCA_OLD_X:		pW->Sca_old_x = Val_h; break;
	case MEMBER_SCA_OLD_Z:		pW->Sca_old_z = Val_h; break;
	case MEMBER_FREE1:			*(USHORT *)&pW->Free[1] = Val_h; break;
	case MEMBER_FREE2:			*((USHORT *)&pW->Free[1] + 1) = Val_h; break;
	case MEMBER_DAMAGE_CNT:		pW->Damage_cnt = Val_b; break;
	}
}

/* DS 0x80055e38 (Member_copy) */
ULONG Member_copy(SCE_TASK *pSce)
{
	/* The SCE record is { opcode, destination, member }. DS reads both operand bytes with lb (signed); plain
	   char is unsigned in this build, so the record pointer is signed char. The destination selects
	   Sce_work[Dst_no - 4] (DS shifts it once and folds the -4 into the Sce_work base address). */
	ULONG Dst_no;
	ULONG Mem_no;
	signed char *pData;

	pData = (signed char *)pSce->Data;
	Dst_no = pData[1];
	Mem_no = pData[2];
	pSce->Data = (UCHAR *)(pData + 3);
	G.SaveData.Sce_work[Dst_no - 4] = (SHORT)Load_member_branch(pSce->pWork, Mem_no);
	return 1;
}

/* Member_cmp record: USHORT comparison word at +2 (retail Member_cmp 0x80055e8c: lhu s0,0x2(v0)) and SHORT value
 * at +4 (retail lh s1,0x4(v0)). */
typedef struct {
	UCHAR Dummy[2];
	USHORT Cmp_word;
	SHORT Cmp_val;
} SCE_MEMBER_CMP_REC;

/* DS 0x80055e8c (Member_cmp) */
ULONG Member_cmp(SCE_TASK *pSce)
{
	USHORT Cmp_word;
	UCHAR *pData;
	UINT Mem_val;
	ULONG Ret;
	int Cmp_op;
	UINT Cmp_val;

	pData = pSce->Data;
	Cmp_word = ((SCE_MEMBER_CMP_REC *)pData)->Cmp_word;
	Cmp_val = (UINT)((SCE_MEMBER_CMP_REC *)pData)->Cmp_val;
	pSce->Data = pData + 6;
	Mem_val = Load_member_branch(pSce->pWork, Cmp_word & 0xff);
	Cmp_op = Cmp_word >> 8;
	Ret = (ULONG)((UINT)Cmp_op < 7);
	if (Ret != 0) {
		switch (Cmp_op) {
		case CMP_EQ:  return (ULONG)(Mem_val == Cmp_val);
		case CMP_GT:  return (ULONG)((int)Cmp_val < (int)Mem_val);
		case CMP_GE:  return ((int)Mem_val < (int)Cmp_val) ^ 1;
		case CMP_LT:  return (ULONG)((int)Mem_val < (int)Cmp_val);
		case CMP_LE:  return ((int)Cmp_val < (int)Mem_val) ^ 1;
		case CMP_NE:  Ret = (ULONG)(Mem_val != Cmp_val); break;
		case CMP_AND: Ret = (ULONG)((Mem_val & Cmp_val) != 0); break;
		}
	}
	return Ret;
}

/* DS 0x80055f50 (Load_member_branch) */
long Load_member_branch(ENEMY_WORK *pW, ULONG Member)
{
	UINT Ret;

	Ret = (UINT)(Member < 0x2c);
	if (Ret != 0) {
		switch (Member) {
		case MEMBER_BE_FLAG0:		Ret = (UINT)(USHORT)pW->Be_flg; break;
		case MEMBER_BE_FLAG1:		Ret = (UINT)*((USHORT *)&pW->Be_flg + 1); break;
		case MEMBER_ROUTINE0:		Ret = (UINT)pW->Routine_0; break;
		case MEMBER_ROUTINE1:		Ret = (UINT)pW->Routine_1; break;
		case MEMBER_ROUTINE2:		Ret = (UINT)pW->Routine_2; break;
		case MEMBER_ROUTINE3:		Ret = (UINT)pW->Routine_3; break;
		case MEMBER_ID:				Ret = (UINT)pW->Id; break;
		case MEMBER_TYPE:			Ret = (UINT)pW->Type; break;
		case MEMBER_AT_OBJ_NO:		Ret = (UINT)pW->At_obj_no; break;
		case MEMBER_AT_SCE_NO:		Ret = (UINT)pW->At_sce_no; break;
		case MEMBER_ATTRIBUTE:		Ret = pW->Attribute; break;
		case MEMBER_POS_X:			Ret = pW->Pos_x; break;
		case MEMBER_POS_Y:			Ret = pW->Pos_y; break;
		case MEMBER_POS_Z:			Ret = pW->Pos_z; break;
		case MEMBER_CDIR_X:			Ret = (UINT)pW->Cdir_x; break;
		case MEMBER_CDIR_Y:			Ret = (UINT)pW->Cdir_y; break;
		case MEMBER_CDIR_Z:			Ret = (UINT)pW->Cdir_z; break;
		case MEMBER_NFLOOR:			Ret = (UINT)pW->nFloor; break;
		case MEMBER_STATUS_FLAG:	Ret = (UINT)pW->Status_flg; break;
		case MEMBER_GROUND:			Ret = (UINT)pW->Ground; break;
		case MEMBER_DEST_X:			Ret = (UINT)pW->Dest_x; break;
		case MEMBER_DEST_Z:			Ret = (UINT)pW->Dest_z; break;
		case MEMBER_SCE_FLAG:		Ret = (UINT)pW->Sce_flg; break;
		case MEMBER_SCE_FREE0:		Ret = (UINT)pW->Sce_free0; break;
		case MEMBER_SCE_FREE1:		Ret = (UINT)pW->Sce_free1; break;
		case MEMBER_SCE_FREE2:		Ret = (UINT)pW->Sce_free2; break;
		case MEMBER_SCE_FREE3:		Ret = (UINT)pW->Sce_free3; break;
		case MEMBER_SPD_X_BYTE:		Ret = (UINT)(UCHAR)pW->Spd_x; break;
		case MEMBER_SPD_X:			Ret = (UINT)pW->Spd_x; break;
		case MEMBER_SPD_Y:			Ret = (UINT)pW->Spd_y; break;
		case MEMBER_SPD_Z:			Ret = (UINT)pW->Spd_z; break;
		case MEMBER_HOKAN_FLAG:		Ret = (UINT)pW->Hokan_flg; break;
		case MEMBER_OFS_X:			Ret = (UINT)pW->Atd[0].Ofs_x; break;
		case MEMBER_OFS_Y:			Ret = (UINT)pW->Atd[0].Ofs_y; break;
		case MEMBER_OFS_Z:			Ret = (UINT)pW->Atd[0].Ofs_z; break;
		case MEMBER_AT_W:			Ret = (UINT)pW->Atd[0].At_w; break;
		case MEMBER_AT_H:			Ret = (UINT)pW->Atd[0].At_h; break;
		case MEMBER_AT_D:			Ret = (UINT)pW->Atd[0].At_d; break;
		case MEMBER_PARTS0_POS_Y:	Ret = (UINT)pW->Parts0_pos_y; break;
		case MEMBER_SCA_OLD_X:		Ret = (UINT)pW->Sca_old_x; break;
		case MEMBER_SCA_OLD_Z:		Ret = (UINT)pW->Sca_old_z; break;
		case MEMBER_FREE1:			Ret = (UINT)(USHORT)pW->Free[1]; break;
		case MEMBER_FREE2:			Ret = (UINT)*((USHORT *)&pW->Free[1] + 1); break;
		case MEMBER_DAMAGE_CNT:		Ret = (UINT)pW->Damage_cnt; break;
		}
	}
	return Ret;
}

/* DS 0x80056188 (Member_calc) */
ULONG Member_calc(SCE_TASK *pSce)
{
	LONG Imm;
	USHORT Cmd_word;
	short *Ans;

	Cmd_word = ((SCE_CALC_REC *)pSce->Data)->Cmd_word;
	Imm = ((SCE_CALC_REC *)pSce->Data)->Imm;
	Ans = Load_member_addr_branch(pSce->pWork, (UINT)(Cmd_word >> 8));
	Calc_branch(Cmd_word & 0xff, Ans, (int)Imm);
	pSce->Data += 6;
	return 1;
}

/* DS 0x800561f4 (Member_calc2) */
ULONG Member_calc2(SCE_TASK *pSce)
{
	short *Ans;

	Ans = Load_member_addr_branch(pSce->pWork, (UINT)pSce->Data[2]);
	Calc_branch((UINT)pSce->Data[1], Ans, (int)G.SaveData.Sce_work[pSce->Data[3] - 4]);
	pSce->Data += 4;
	return 1;
}

/* DS 0x80056264 (Load_member_addr_branch) */
short *Load_member_addr_branch(ENEMY_WORK *pW, ULONG Member)
{
	ENEMY_WORK *pAddr;

	pAddr = (ENEMY_WORK *)(UINT)(Member < 0x2c);
	if ((ENEMY_WORK *)(UINT)(Member < 0x2c) != (ENEMY_WORK *)0x0) {
		switch (Member) {
		case MEMBER_BE_FLAG0:		pAddr = pW; break;
		case MEMBER_BE_FLAG1:		pAddr = (ENEMY_WORK *)((USHORT *)&pW->Be_flg + 1); break;
		case MEMBER_ROUTINE0:		pAddr = (ENEMY_WORK *)&pW->Routine_0; break;
		case MEMBER_ROUTINE1:		pAddr = (ENEMY_WORK *)&pW->Routine_1; break;
		case MEMBER_ROUTINE2:		pAddr = (ENEMY_WORK *)&pW->Routine_2; break;
		case MEMBER_ROUTINE3:		pAddr = (ENEMY_WORK *)&pW->Routine_3; break;
		case MEMBER_ID:				pAddr = (ENEMY_WORK *)&pW->Id; break;
		case MEMBER_TYPE:			pAddr = (ENEMY_WORK *)&pW->Type; break;
		case MEMBER_AT_OBJ_NO:		pAddr = (ENEMY_WORK *)&pW->At_obj_no; break;
		case MEMBER_AT_SCE_NO:		pAddr = (ENEMY_WORK *)&pW->At_sce_no; break;
		case MEMBER_ATTRIBUTE:		pAddr = (ENEMY_WORK *)&pW->Attribute; break;
		case MEMBER_POS_X:			pAddr = (ENEMY_WORK *)&pW->Pos_x; break;
		case MEMBER_POS_Y:			pAddr = (ENEMY_WORK *)&pW->Pos_y; break;
		case MEMBER_POS_Z:			pAddr = (ENEMY_WORK *)&pW->Pos_z; break;
		case MEMBER_CDIR_X:			pAddr = (ENEMY_WORK *)&pW->Cdir_x; break;
		case MEMBER_CDIR_Y:			pAddr = (ENEMY_WORK *)&pW->Cdir_y; break;
		case MEMBER_CDIR_Z:			pAddr = (ENEMY_WORK *)&pW->Cdir_z; break;
		case MEMBER_NFLOOR:			pAddr = (ENEMY_WORK *)&pW->nFloor; break;
		case MEMBER_STATUS_FLAG:	pAddr = (ENEMY_WORK *)&pW->Status_flg; break;
		case MEMBER_GROUND:			pAddr = (ENEMY_WORK *)&pW->Ground; break;
		case MEMBER_DEST_X:			pAddr = (ENEMY_WORK *)&pW->Dest_x; break;
		case MEMBER_DEST_Z:			pAddr = (ENEMY_WORK *)&pW->Dest_z; break;
		case MEMBER_SCE_FLAG:		pAddr = (ENEMY_WORK *)&pW->Sce_flg; break;
		case MEMBER_SCE_FREE0:		pAddr = (ENEMY_WORK *)&pW->Sce_free0; break;
		case MEMBER_SCE_FREE1:		pAddr = (ENEMY_WORK *)&pW->Sce_free1; break;
		case MEMBER_SCE_FREE2:		pAddr = (ENEMY_WORK *)&pW->Sce_free2; break;
		case MEMBER_SCE_FREE3:		pAddr = (ENEMY_WORK *)&pW->Sce_free3; break;
		case MEMBER_SPD_X_BYTE:
		case MEMBER_SPD_X:
		default:					pAddr = (ENEMY_WORK *)&pW->Spd_x; break;
		case MEMBER_SPD_Y:			pAddr = (ENEMY_WORK *)&pW->Spd_y; break;
		case MEMBER_SPD_Z:			pAddr = (ENEMY_WORK *)&pW->Spd_z; break;
		case MEMBER_HOKAN_FLAG:		pAddr = (ENEMY_WORK *)&pW->Hokan_flg; break;
		case MEMBER_OFS_X:			pAddr = (ENEMY_WORK *)&pW->Atd[0].Ofs_x; break;
		case MEMBER_OFS_Y:			pAddr = (ENEMY_WORK *)&pW->Atd[0].Ofs_y; break;
		case MEMBER_OFS_Z:			pAddr = (ENEMY_WORK *)&pW->Atd[0].Ofs_z; break;
		case MEMBER_AT_W:			pAddr = (ENEMY_WORK *)&pW->Atd[0].At_w; break;
		case MEMBER_AT_H:			pAddr = (ENEMY_WORK *)&pW->Atd[0].At_h; break;
		case MEMBER_AT_D:			pAddr = (ENEMY_WORK *)&pW->Atd[0].At_d; break;
		case MEMBER_PARTS0_POS_Y:	pAddr = (ENEMY_WORK *)&pW->Parts0_pos_y; break;
		case MEMBER_SCA_OLD_X:		pAddr = (ENEMY_WORK *)&pW->Sca_old_x; break;
		case MEMBER_SCA_OLD_Z:		pAddr = (ENEMY_WORK *)&pW->Sca_old_z; break;
		case MEMBER_FREE1:			pAddr = (ENEMY_WORK *)&pW->Free[1]; break;
		case MEMBER_FREE2:			pAddr = (ENEMY_WORK *)((USHORT *)&pW->Free[1] + 1); break;
		case MEMBER_DAMAGE_CNT:		pAddr = (ENEMY_WORK *)&pW->Damage_cnt; break;
		}
	}
	return (short *)pAddr;
}

/* Dir_ck record: SHORT Vec_x at +2, SHORT Vec_z at +4 and SHORT Add_dir at +6 (retail Dir_ck 0x800563e8:
 * lh a1,0x2(v1); lh a2,0x4(v1); lh a3,0x6(v1)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Vec_x;
	SHORT Vec_z;
	SHORT Add_dir;
} SCE_DIR_CK_REC;

/* DS 0x800563e8 (Dir_ck) */
ULONG Dir_ck(SCE_TASK *pSce)
{
	long Ret;
	UCHAR *pData;

	pData = pSce->Data;
	pSce->Data = pData + 8;
	Ret = Goto00_ck(pSce->pWork, (int)((SCE_DIR_CK_REC *)pData)->Vec_x, (int)((SCE_DIR_CK_REC *)pData)->Vec_z, ((SCE_DIR_CK_REC *)pData)->Add_dir);
	return (UINT)(Ret == 0);
}
