/*
 * MONITOR.C - Task (thread) scheduler
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */

#include "TYPES.H"
#include <ASM.H>
#include <KERNEL.H>
#include <LIBAPI.H>
#include <STRINGS.H>
#include "MONITOR.H"
#include "CD.H"
#include "DOOR.H"
#include "MAIN.H"
#include "FILECODE.H"

/* DS 0x80010ad0 / 0x80010ad8 - Cd_read debug message strings */
static char     ExecuteStr[] = "execute";

/* Overlay entry functions named in Ld_task_tbl (each is defined in its overlay .C file). */
extern VOID     Title(VOID);                        /* DS 0x80191808 (Title), TITLE.BIN */
extern VOID     Die_demo(VOID);                     /* DS 0x8019009c (Die_demo), DIEDEMO.BIN */
extern VOID     Select_main(VOID);                  /* DS 0x801bfaa8 (Select_main), SELECT.BIN */
extern VOID     Config(VOID);                       /* DS 0x801bfa84 (Config), CONFIG.BIN */
extern VOID     Opening(VOID);                      /* DS 0x801bfad8 (Opening), OPENING.BIN */
extern VOID     Result_task(VOID);                  /* DS 0x80158020 (Result_task), RESULT.BIN */
extern VOID     Stage1(VOID);                       /* DS 0x8011a004 (Stage1), STAGE1.BIN */
extern VOID     Stage2(VOID);                       /* DS 0x8011a004 (Stage2), STAGE2.BIN */
extern VOID     Stage3(VOID);                       /* DS 0x8011a004 (Stage3), STAGE3.BIN */
extern VOID     Stage4(VOID);                       /* DS 0x8011a004 (Stage4), STAGE4.BIN */
extern VOID     Stage5(VOID);                       /* DS 0x8011a004 (Stage5), STAGE5.BIN */
extern VOID     Stage6(VOID);                       /* DS 0x8011b180 (Stage6), STAGE6.BIN */
extern VOID     Stage7(VOID);                       /* DS 0x8011a004 (Stage7), STAGE7.BIN */

/* Kept as fixed placement (not data objects): the cells hold overlay load destinations
 * 0x80190000, 0x8011a000, 0x801bfa18 and 0x80158000, the bank bases the overlays must sit at. */
/* Load-address cells read through Ld_task_tbl[].Ld_adr (Ghidra data labels; the
 * SLUS image initialises each cell to its overlay bank base). */
/* DS 0x800107e8 (pTitle_adr) */
ULONG    *pTitle_adr = (ULONG *)0x80190000;
/* DS 0x800107ec (Diedemo_adr) */
ULONG    *Diedemo_adr = (ULONG *)0x80190000;
/* DS 0x800107f0 (pEnding_adr) */
ULONG    *pEnding_adr = (ULONG *)0x8011a000;
/* DS 0x800107f4 (pMemCard_adr) */
ULONG    *pMemCard_adr = (ULONG *)0x801bfa18;
/* DS 0x800107f8 (pSelect_adr) */
ULONG    *pSelect_adr = (ULONG *)0x801bfa18;
/* DS 0x800107fc (pConfig_adr) */
ULONG    *pConfig_adr = (ULONG *)0x801bfa18;
/* DS 0x80010800 (pOpening_adr) */
ULONG    *pOpening_adr = (ULONG *)0x801bfa18;
/* DS 0x8001080c (pResult_adr) */
ULONG    *pResult_adr = (ULONG *)0x80158000;
/* DS 0x80010810 (pStage_adr) */
ULONG    *pStage_adr = (ULONG *)0x8011a000;
static char     ChainStr[]   = "chain";

/* DS 0x800a20b8 - overlay task table, 15 x 12 bytes (ends at Movie_tbl, DS 0x800a216c).
 * Fields: File_no (FILECODE.H index), Func (overlay entry), Ld_adr (load-address cell).
 * Entries 1 and 6 have no project or Ghidra function at their Func address in the
 * overlay that holds File_no, so Func is the raw VA cast, naming the overlay block. */
static LOADTASK Ld_task_tbl[15] = {
	{ TITLE_BIN,    Title,                      &pTitle_adr   },  /* [0]  TITLE.BIN, Title (DS 0x80191808) */
	{ MEM_CARD_BIN, (void (*)(void))0x801bfd14, &pMemCard_adr },  /* [1]  MEM_CARD.BIN block 0x801BFD14, raw VA */
	{ DIEDEMO_BIN,  Die_demo,                   &Diedemo_adr  },  /* [2]  DIEDEMO.BIN, Die_demo (DS 0x8019009c) */
	{ SELECT_BIN,   Select_main,                &pSelect_adr  },  /* [3]  SELECT.BIN, Select_main (DS 0x801bfaa8) */
	{ CONFIG_BIN,   Config,                     &pConfig_adr  },  /* [4]  CONFIG.BIN, Config (DS 0x801bfa84) */
	{ OPENING_BIN,  Opening,                    &pOpening_adr },  /* [5]  OPENING.BIN, Opening (DS 0x801bfad8) */
	{ ENDING_BIN,   (void (*)(void))0x8011a0fc, &pEnding_adr  },  /* [6]  ENDING.BIN block 0x8011A0FC, raw VA */
	{ RESULT_BIN,   Result_task,                &pResult_adr  },  /* [7]  RESULT.BIN, Result_task (DS 0x80158020) */
	{ STAGE1_BIN,   Stage1,                     &pStage_adr   },  /* [8]  STAGE1.BIN, Stage1 (DS 0x8011a004) */
	{ STAGE2_BIN,   Stage2,                     &pStage_adr   },  /* [9]  STAGE2.BIN, Stage2 (DS 0x8011a004) */
	{ STAGE3_BIN,   Stage3,                     &pStage_adr   },  /* [10] STAGE3.BIN, Stage3 (DS 0x8011a004) */
	{ STAGE4_BIN,   Stage4,                     &pStage_adr   },  /* [11] STAGE4.BIN, Stage4 (DS 0x8011a004) */
	{ STAGE5_BIN,   Stage5,                     &pStage_adr   },  /* [12] STAGE5.BIN, Stage5 (DS 0x8011a004) */
	{ STAGE6_BIN,   Stage6,                     &pStage_adr   },  /* [13] STAGE6.BIN, Stage6 (DS 0x8011b180) */
	{ STAGE7_BIN,   Stage7,                     &pStage_adr   },  /* [14] STAGE7.BIN, Stage7 (DS 0x8011a004) */
};

/* DS 0x800d76a0 (Monitor) */
MONITOR     Monitor = { 0 };

/* DS 0x800c3ab8 - return address of the last Task_sleep caller */
static ULONG    *pPc;

/* Kernel table of tables (address 0x100), its TCB-base entry index,
 * the TCB register index holding SR, and the SR value given to new TCBs (IEc|IM2). */
#define KERNEL_TOT_ADDR     0x100
#define KERNEL_TCB_BASE_IDX 4
#define TCB_SR_REG_IDX      35
#define TCB_SR_INIT         0x404
#define TCB_SIZE            0xc0

/* ------------------------------------------------------------------ */
/* Init_scheduler  (DS 0x80031cc0)                                      */
/* Bind each task slot to kernel TCB i+1 and set its stack/GP.         */
/* ------------------------------------------------------------------ */
/* DS 0x80031cc0 (Init_scheduler) */
void Init_scheduler(void)
{
	LONG        i;
	ULONG       *pToT;                      /* kernel Table of Tables at 0x100 */
	TASK        *pT;

	pToT = (ULONG *)KERNEL_TOT_ADDR;
	for (i = 0, pT = Monitor.Task; i < 3; i++, pT++)
	{
		pT->Tcb_status = 0;
		pT->pTcb       = (struct TCB *)(pToT[KERNEL_TCB_BASE_IDX] + TCB_SIZE * (i + 1));  /* TCB array base = word at 0x110 */
		pT->pTcb->reg[TCB_SR_REG_IDX] = TCB_SR_INIT;  /* SR: IEc|IM2 */
		pT->Tcb_exec.gp = GetGp();
	}

	Monitor.Task[0].Tcb_init_sp = TASK0_STACK_TOP;
	Monitor.Task[1].Tcb_init_sp = TASK1_STACK_TOP;
	Monitor.Task[2].Tcb_init_sp = TASK2_STACK_TOP;
}

/* ------------------------------------------------------------------ */
/* Scheduler  (DS 0x80031d8c)                                           */
/* ------------------------------------------------------------------ */
void Scheduler(void)
{
	TASK    *pCt;

	for (pCt = Monitor.Task; pCt < &Monitor.Task[3]; pCt++)
	{
		Monitor.pCtcb = pCt;
		switch (pCt->Tcb_status)
		{
		case 2:                             /* start new thread */
			EnterCriticalSection();
			pCt->Tcb_th = OpenTh(pCt->pTcb_pc, pCt->Tcb_init_sp, pCt->Tcb_exec.gp);
			ExitCriticalSection();
			pCt->Tcb_status = 0x7f;
			ChangeTh(pCt->Tcb_th);
			break;

		case 1:                             /* sleeping */
			if (--pCt->Tcb_sleep_ctr > 0)
				break;
			/* fall through */
		case 4:                             /* resume */
			pCt->Tcb_status = 0x7f;
			ChangeTh(pCt->Tcb_th);
			break;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Task_lexecute  (DS 0x80031e80)                                       */
/* ------------------------------------------------------------------ */
ULONG Task_lexecute(ULONG Level, ULONG Task_no)
{
	ULONG   Size;

	Size = Cd_read(Ld_task_tbl[Task_no].File_no, *Ld_task_tbl[Task_no].Ld_adr, 0,
				   (UCHAR *)ExecuteStr);
	Task_execute(Level, (&Ld_task_tbl[Task_no])->Func);   /* table entry re-addressed after the call, as retail does */
	return Size;
}

/* ------------------------------------------------------------------ */
/* Task_lchain  (DS 0x80031f00)                                         */
/* ------------------------------------------------------------------ */
void Task_lchain(ULONG Task_no)
{
	LOADTASK    *pLd;

	pLd = &Ld_task_tbl[Task_no];
	Cd_read(pLd->File_no, *pLd->Ld_adr, 0, (UCHAR *)ChainStr);
	Task_chain(pLd->Func);
}

/* ------------------------------------------------------------------ */
/* Task_execute  (DS 0x80031f6c)                                        */
/* ------------------------------------------------------------------ */
void Task_execute(ULONG Level, void *pFuncall)
{
	Monitor.Task[Level].pTcb_pc    = (LONG (*)(void))pFuncall;
	Monitor.Task[Level].Tcb_status = 2;
}

/* ------------------------------------------------------------------ */
/* Task_sleep  (DS 0x80031f94)                                          */
/* ------------------------------------------------------------------ */
void Task_sleep(ULONG Sleep_counter)
{
	TASK    *pCt;

	pCt = Monitor.pCtcb;
	pCt->Tcb_sleep_ctr = (SHORT)Sleep_counter;
	pCt->Tcb_status    = 1;
	STORE_RA(pPc);                          /* pPc = caller's $ra */
	pCt->pPc = pPc;
	ChangeTh(0xff000000);                   /* back to the main thread */
}

/* ------------------------------------------------------------------ */
/* Task_exit  (DS 0x80031fe4)                                           */
/* ------------------------------------------------------------------ */
void Task_exit(void)
{
	TASK    *pCt;

	pCt = Monitor.pCtcb;
	pCt->Tcb_status = 0;
	EnterCriticalSection();
	CloseTh(pCt->Tcb_th);
	ExitCriticalSection();
	ChangeTh(0xff000000);
}

/* ------------------------------------------------------------------ */
/* Task_kill  (DS 0x80032030)                                           */
/* ------------------------------------------------------------------ */
void Task_kill(ULONG Level)
{
	TASK    *pT;

	pT = &Monitor.Task[Level];
	if (pT->Tcb_status != 0)
	{
		pT->Tcb_status = 0;
		EnterCriticalSection();
		CloseTh(pT->Tcb_th);
		ExitCriticalSection();
	}
}

/* ------------------------------------------------------------------ */
/* Task_chain  (DS 0x8003208c)                                          */
/* ------------------------------------------------------------------ */
void Task_chain(void *pFuncall)
{
	TASK    *pCt;

	pCt = Monitor.pCtcb;
	pCt->Tcb_status = 2;
	pCt->pTcb_pc    = (LONG (*)(void))pFuncall;
	EnterCriticalSection();
	CloseTh(pCt->Tcb_th);
	ExitCriticalSection();
	ChangeTh(0xff000000);
}

/* ------------------------------------------------------------------ */
/* Task_suspend  (DS 0x800320e0)                                        */
/* ------------------------------------------------------------------ */
void Task_suspend(ULONG Level)
{
	Monitor.Task[Level].Tcb_status |= 0x40;
}

/* ------------------------------------------------------------------ */
/* Task_signal  (DS 0x8003210c)                                         */
/* ------------------------------------------------------------------ */
void Task_signal(ULONG Level)
{
	Monitor.Task[Level].Tcb_status &= ~0x40;
}

/* ------------------------------------------------------------------ */
/* Task_status  (DS 0x80032138)                                         */
/* ------------------------------------------------------------------ */
ULONG Task_status(ULONG Level)
{
	return Monitor.Task[Level].Tcb_status;
}
