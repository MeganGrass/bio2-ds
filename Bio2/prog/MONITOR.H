/*
 * MONITOR.H - Task scheduler (MONITOR) task slot and loader structs
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MONITOR.H
 */
#ifndef MONITOR_H
#define MONITOR_H

#include <KERNEL.H>

/* Task slot (0x80 bytes) */
struct tagTASK {
	USHORT      Tcb_status;         /* 0x00  0 free, 1 sleep, 2 start, 4 resume, 0x7f run, |0x40 suspended */
	SHORT       Tcb_sleep_ctr;      /* 0x02 */
	LONG        (*pTcb_pc)(void);   /* 0x04 */
	ULONG       Tcb_th;             /* 0x08 */
	struct TCB  *pTcb;              /* 0x0c */
	ULONG       Tcb_init_sp;        /* 0x10 */
	struct EXEC Tcb_exec;           /* 0x14 */
	ULONG       *pPc;               /* 0x50 */
	USHORT      R_no[6];            /* 0x54 */
	SHORT       Ctr[8];             /* 0x60 */
	UCHAR       Free[16];           /* 0x70 */
};

struct tagMONITOR {
	ULONG       Gp;
	TASK        Task[3];            /* DS 0x800d76a4 */
	TASK        *pCtcb;             /* DS 0x800d7824 */
};

struct tagLOADTASK {
	ULONG       File_no;
	void        (*Func)(void);
	ULONG       **Ld_adr;
};

/* Initial stack tops of the three task threads. Kept: fixed thread-stack placement, not data objects. */
#define TASK0_STACK_TOP     0x801ffb00
#define TASK1_STACK_TOP     0x801ff400
#define TASK2_STACK_TOP     0x801fed00

/* PLACEHOLDER - caller return address capture (Ghidra: Task_sleep stores $ra,
 * i.e. its caller's return address, into DS 0x800c3ab8 "pPc").
 * PSX/MIPS builds: the real retail sequence (inline asm "sw $31,0(&dst)").
 * Host builds (MSVC/GCC): the compiler intrinsic, which yields the return
 * address of the function that invokes STORE_RA (Task_sleep's caller), so the
 * value is only meaningful when the code is inlined into Task_sleep.
 * Do not treat host-build results as retail data. */
#if defined(__mips__) || defined(__psx__) || defined(__R3000__)
#define STORE_RA(dst)   __asm__ volatile ("sw $31, 0(%0)" : : "r"(&(dst)) : "memory")
#elif defined(_MSC_VER)
#include <intrin.h>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
#define STORE_RA(dst)   ((dst) = (ULONG *)_ReturnAddress())   /* PLACEHOLDER (host build) */
#else
#define STORE_RA(dst)   ((dst) = (ULONG *)__builtin_return_address(0))   /* PLACEHOLDER (host build) */
#endif

/* DS 0x800d76a0 */ extern MONITOR  Monitor;

void    Init_scheduler(void);                           /* 80031cc0 */
void Scheduler(void);                                 /* 80031d8c */
ULONG   Task_lexecute(ULONG Level, ULONG Task_no);      /* 80031e80 */
void    Task_lchain(ULONG Task_no);                     /* 80031f00 */
void    Task_execute(ULONG Level, void *pFuncall);      /* 80031f6c */
void    Task_sleep(ULONG Sleep_counter);                /* 80031f94 */
void    Task_exit(void);                                /* 80031fe4 */
void    Task_kill(ULONG Level);                         /* 80032030 */
void    Task_chain(void *pFuncall);                     /* 8003208c */
void    Task_suspend(ULONG Level);                      /* 800320e0 */
void    Task_signal(ULONG Level);                       /* 8003210c */
ULONG   Task_status(ULONG Level);                       /* 80032138 */


#endif /* MONITOR_H_H */
