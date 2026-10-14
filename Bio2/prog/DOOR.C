/*
 * DOOR.C - Door rendering and scheduling module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\DOOR.C
 */

#include "TYPES.H"
#include "ROOM.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include <STDLIB.H>
#include <STRINGS.H>
#include "DOOR.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "OMA.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "SUB.H"
#include "CONFIG.H"
#include "GAME.H"
#include "FILECODE.H"
#include "flag.h"
#include "DIVCHK.H"

/* DS 0x80010b10 */
UCHAR StgNxt[] = "stg.nxt";

/* DS 0x8009a42c */
UCHAR door_msg[0x3c] = {
	44, 72, 65, 61, 79, 65, 0, 83, 61, 69, 80, 0, 61, 0, 73, 69,
	74, 81, 80, 65, 1, 254, 0, 48, 68, 65, 0, 79, 69, 64, 65, 0,
	76, 61, 63, 71, 0, 69, 79, 0, 62, 65, 69, 74, 67, 254, 0, 65,
	77, 81, 69, 76, 76, 65, 64, 1, 254, 0, 0, 0
};

/* DS 0x8009a468 */
USHORT door_msg_ptr[4] = {
	0, 23, 47, 0
};


/* DS 0x8009a4b0 */
USHORT Dtex_file[56] = {
	DOOR00_DO2, DOOR01_DO2, DOOR02_DO2, DOOR03_DO2,
	DOOR04_DO2, DOOR05_DO2, DOOR06_DO2, DOOR07_DO2,
	DOOR08_DO2, DOOR09_DO2, DOOR0A_DO2, DOOR0B_DO2,
	DOOR0C_DO2, DOOR0D_DO2, DOOR0E_DO2, DOOR0F_DO2,
	DOOR10_DO2, DOOR11_DO2, DOOR12_DO2, DOOR13_DO2,
	DOOR14_DO2, DOOR15_DO2, DOOR16_DO2, DOOR17_DO2,
	DOOR18_DO2, DOOR19_DO2, DOOR1A_DO2, DOOR1B_DO2,
	DOOR1C_DO2, DOOR1D_DO2, DOOR1E_DO2, DOOR1F_DO2,
	DOOR20_DO2, DOOR21_DO2, DOOR22_DO2, DOOR23_DO2,
	DOOR24_DO2, DOOR25_DO2, DOOR26_DO2, DOOR27_DO2,
	DOOR28_DO2, DOOR29_DO2, DOOR2A_DO2, DOOR2B_DO2,
	DOOR2C_DO2, DOOR2D_DO2, DOOR2E_DO2, DOOR2F_DO2,
	DOOR30_DO2, DOOR31_DO2, DOOR32_DO2, DOOR33_DO2,
	DOOR34_DO2, DOOR35_DO2, DOOR36_DO2, DOOR00_DO2,
};

/* DS 0x8009a520 */
DOOR_INFO Dinfo[55] = {
	{ 0x4B08, 0x8DD0, 0x0000000A, 0x9A, 0x42, 0x0000 },
	{ 0x5D78, 0xBA70, 0x0000000C, 0x2E, 0xD0, 0x0000 },
	{ 0x4B08, 0x94F0, 0x0000000A, 0x9A, 0xB3, 0x0000 },
	{ 0x4B08, 0x8E0C, 0x0000000A, 0x9A, 0xBC, 0x0000 },
	{ 0x5D78, 0xAFB8, 0x0000000C, 0x2E, 0xB2, 0x0000 },
	{ 0x4B08, 0xB7EC, 0x0000000A, 0x9A, 0x43, 0x0000 },
	{ 0x4C98, 0x908C, 0x0000000A, 0x85, 0x61, 0x0000 },
	{ 0x4C98, 0x8A64, 0x0000000A, 0x85, 0x36, 0x0000 },
	{ 0x4C98, 0x9528, 0x0000000A, 0x85, 0x95, 0x0000 },
	{ 0x5D78, 0x9510, 0x0000000C, 0x2E, 0xC3, 0x0000 },
	{ 0x59F8, 0xD06C, 0x0000000C, 0x52, 0x24, 0x0000 },
	{ 0x4B08, 0x8E70, 0x0000000A, 0x9A, 0x1D, 0x0000 },
	{ 0x5D48, 0x901C, 0x0000000C, 0x59, 0xB8, 0x0000 },
	{ 0x4B08, 0x91A4, 0x0000000A, 0x9A, 0xDB, 0x0000 },
	{ 0x3A28, 0xAA04, 0x00000008, 0x91, 0x0C, 0x0000 },
	{ 0x2A88, 0x8BCC, 0x00000006, 0xDB, 0x36, 0x0000 },
	{ 0x4B18, 0x8D98, 0x0000000A, 0x97, 0x12, 0x0000 },
	{ 0x5D78, 0x9690, 0x0000000C, 0x2E, 0x3E, 0x0000 },
	{ 0x3B08, 0x8BCC, 0x00000008, 0x9F, 0x4B, 0x0000 },
	{ 0x3DA8, 0x9504, 0x00000008, 0x96, 0xC1, 0x0000 },
	{ 0x5DD8, 0x98D8, 0x0000000C, 0x3D, 0x87, 0x0000 },
	{ 0x5338, 0x9CDC, 0x0000000B, 0xF5, 0xEC, 0x0000 },
	{ 0x2AF8, 0xB5EC, 0x00000006, 0xE8, 0x67, 0x0000 },
	{ 0x5A48, 0x9C50, 0x0000000C, 0x25, 0x5C, 0x0000 },
	{ 0x4B08, 0x8DCC, 0x0000000A, 0x9A, 0x8A, 0x0000 },
	{ 0x5C98, 0xB354, 0x0000000C, 0xAA, 0xA6, 0x0000 },
	{ 0x5338, 0x92FC, 0x0000000B, 0xF5, 0xF4, 0x0000 },
	{ 0x52B8, 0x9F28, 0x0000000B, 0x75, 0x46, 0x0000 },
	{ 0x5CB8, 0x94EC, 0x0000000C, 0x86, 0x13, 0x0000 },
	{ 0x5D78, 0x8D04, 0x0000000C, 0xEB, 0x48, 0x0000 },
	{ 0x5958, 0xBF0C, 0x0000000C, 0x28, 0xF3, 0x0000 },
	{ 0x5298, 0x9878, 0x0000000B, 0xBE, 0x7E, 0x0000 },
	{ 0x5C68, 0x8C0C, 0x0000000C, 0x56, 0x34, 0x0000 },
	{ 0x5C28, 0x8C0C, 0x0000000C, 0x81, 0x34, 0x0000 },
	{ 0x4C98, 0x8654, 0x0000000A, 0x85, 0xC1, 0x0000 },
	{ 0x5338, 0x9AFC, 0x0000000B, 0xF5, 0xDC, 0x0000 },
	{ 0x58C8, 0xA7F4, 0x0000000C, 0xAC, 0x11, 0x0000 },
	{ 0x44D8, 0x85FC, 0x00000009, 0x18, 0x22, 0x0000 },
	{ 0x5548, 0xE5B8, 0x0000000B, 0x17, 0x5A, 0x0000 },
	{ 0x5A88, 0xD2AC, 0x0000000C, 0x1E, 0x8F, 0x0000 },
	{ 0x5A08, 0xB2D4, 0x0000000C, 0xFC, 0x93, 0x0000 },
	{ 0x58C8, 0xBF9C, 0x0000000C, 0xAC, 0x77, 0x0000 },
	{ 0x4028, 0x8754, 0x00000009, 0x62, 0x3F, 0x0000 },
	{ 0x4848, 0x90C0, 0x0000000A, 0x69, 0x18, 0x0000 },
	{ 0x5EF8, 0x9698, 0x0000000C, 0xAF, 0x27, 0x0000 },
	{ 0x58B8, 0xD1A4, 0x0000000C, 0xE3, 0x84, 0x0000 },
	{ 0x4AE8, 0xD7FC, 0x0000000A, 0x4F, 0x9D, 0x0000 },
	{ 0x4C98, 0x8E44, 0x0000000A, 0x85, 0x9F, 0x0000 },
	{ 0x52B8, 0xA4EC, 0x0000000B, 0x75, 0x0B, 0x0000 },
	{ 0x5548, 0xE5B8, 0x0000000B, 0x17, 0xE6, 0x0000 },
	{ 0x5D88, 0x8C0C, 0x0000000C, 0xD1, 0x34, 0x0000 },
	{ 0x48E8, 0xBF3C, 0x0000000A, 0x67, 0x9E, 0x0000 },
	{ 0x5EF8, 0x8C0C, 0x0000000C, 0x70, 0x34, 0x0000 },
	{ 0x4D78, 0xBF18, 0x0000000A, 0x06, 0x4D, 0x0000 },
	{ 0x52F8, 0x8C1C, 0x0000000B, 0x58, 0x50, 0x0000 },
};


/* DS 0x800a23e7 */
UCHAR Door_ex_mess1a[0x1f] = {
	0x22, 0x45, 0x43, 0x44, 0x50, 0x00, 0x50, 0x4B, 0x00, 0x50, 0x44, 0x41, 0x00, 0x4C, 0x4B, 0x48,
	0x45, 0x3F, 0x41, 0x00, 0x4C, 0x4E, 0x41, 0x3F, 0x45, 0x4A, 0x3F, 0x50, 0x1A, 0xFE, 0x00,
};


/* DS 0x800a2406 */
UCHAR Door_ex_mess1b[0x26] = {
	0x2B, 0x3E, 0x50, 0x3D, 0x45, 0x4A, 0x00, 0x50, 0x44, 0x41, 0x00, 0x10, 0x00, 0x4F, 0x4C, 0x41,
	0x3F, 0x45, 0x3D, 0x48, 0x00, 0x3E, 0x4B, 0x49, 0x3E, 0x4F, 0x1A, 0xFE, 0x00, 0x00, 0x00, 0x00,
	0x15, 0x00, 0x27, 0x00, 0x46, 0x00,
};

/* DS 0x8009a7b4 (pDoor_proc_tbl) */
DOOR_PROC pDoor_proc_tbl[4] = {
	Door_init,
	Door_move,
	Door_exit,
	NULL
};

/* 800a242c */
static DOOR_PROC pDoor_proc_tbl_ex[6] = {
	Door_proc_ex_init,
	Door_proc_ex_move,
	Door_proc_ex_end,
	Door_proc_ex_xa_wait,
	Door_proc_ex_result,
	NULL
};

/* DS 0x800c3a80 */
DOOR *pDoor[2] = { 0, 0 };

/* DS 0x8001082c (Door_camera) */
const RCUT Door_camera = {
	0x0000, 0x642D,
	{ 0x2710, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000 },
	0
};

/* DS 0x8009a470 (D_LIGHTMATRIX) */
MATRIX D_LIGHTMATRIX = {
	{ { 400, 800, -500 }, { -1800, -1000, -2700 }, { 3500, 6700, 1200 } },
	{ 0, 0, 0 }
};

/* DS 0x8009a490 (D_COLORMATRIX) */
MATRIX D_COLORMATRIX = {
	{ { 1600, 1600, 1600 }, { 1600, 1600, 1600 }, { 1600, 1600, 1600 } },
	{ 0, 0, 0 }
};

/* DS 0x800a23c0 (Door_ex_mess0) */
UCHAR Door_ex_mess0[0x15] = {
	0x1F, 0x28, 0x21, 0x1D, 0x2E, 0x00, 0x30, 0x25, 0x29, 0x21, 0x00, 0x0C, 0x0C, 0x16, 0x0C, 0x0C,
	0x16, 0x0C, 0x0C, 0xFE, 0x00,
};

/* DS 0x800a23d5 (Door_ex_mess_push) */
UCHAR Door_ex_mess_push[0x12] = {
	0x2C, 0x2E, 0x21, 0x2F, 0x2F, 0x00, 0x1D, 0x2A, 0x35, 0x00, 0x1E, 0x31, 0x30, 0x30, 0x2B, 0x2A,
	0xFE, 0x00,
};

/* DS 0x800dcbe0 (Ex_battle_rcut) */
RCUT Ex_battle_rcut = {
	0, 0, { 0, 0, 0 }, { 0, 0, 0 }, 0
};

/* DS 0x800dcc50 (Ex_battle_vcut) */
VCUT Ex_battle_vcut = {
	0, 0, 0, 0,
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }
};

/* DS 0x800a7fac (Snd_cd_watchdog_ctr) */
int Snd_cd_watchdog_ctr;

/* DS 0x800c3ac0 (Door_ex_buf0) */
UCHAR Door_ex_buf0[0x18];

/* DS 0x800c3ad8 (Door_ex_buf1) */
UCHAR Door_ex_buf1[0x18];

/* DS 0x80190000 (Door_ex_data) */
ULONG Door_ex_data[0x18EA] RAM_SECTION(".buf_door_ex_data");	/* 0x63A8 bytes: CD file STG_NXT (floc size) */


/* DS 0x801a1000 (Door_data) */
DOOR_DATA Door_data;

/* DS 0x801bd000 (Door_mem) */
DOOR Door_mem;

/* DS 0x801fb700 (Door_snd_head) */
DOOR_SND_HEAD Door_snd_head;


/* ------------------------------------------------------------------ */
/* Door_main                                                            */
/* Iterates the door process table and calls each registered handler.  */
/* ------------------------------------------------------------------ */
/* DS 0x80013bc4 (Door_main) */
void Door_main(void)
{
	void (**pProc_cur)(void);

	pProc_cur = (void (**)(void))pDoor_proc_tbl;	/* pDoor_proc_tbl (DS 0x8009a7b4) is the table itself */
	if (pDoor_proc_tbl[0] != NULL) {
		do {
			(*pProc_cur)();
			pProc_cur++;
		} while (*pProc_cur != NULL);
	}
}

/* ------------------------------------------------------------------ */
/* Door_init                                                            */
/* Initialises the door system: loads texture, sets up DOOR_WORK       */
/* array, maps TMD, configures view and saves door type in DSce_work.  */
/* ------------------------------------------------------------------ */
/* DS 0x80013c1c (Door_init) */
void Door_init(void)
{
	ULONG        *Data_adr;
	DOOR_WORK    *pDwork_slot;
	LONG          Slot_idx;
	ULONG         Tmd_tpage;
	LONG          Task_idx;
	DOOR_WORK   **ppDwork_slot;
	ULONG         Tmd_clut;
	SCE_TASK     *pTask;
	/* Camera cut copied from the rodata initialiser (Door_camera, 0x8001082c) */
	RCUT          Camera = Door_camera;

	pDoor[1] = (DOOR *)0x1;
	Door_texture_load();

	/* Initialise pDwork pointer array (slot 9 = last, slots step down by sizeof(DOOR_WORK)) */
	Slot_idx     = 9;
	ppDwork_slot = pDwork + 9;
	pDwork_slot  = DOOR_WORK_SLOT9;
	pDoor[0] = &Door_mem;

	G.System_flg |= SYS_DOOR_PROCESS;

	do {
		*ppDwork_slot = pDwork_slot;
		ppDwork_slot--;
		Slot_idx--;
		pDwork_slot--;
	} while (-1 < Slot_idx);

	/* Zero the 16-bit head of every DOOR_WORK slot up to pDwork[9]->Model_no */
	pDwork_slot = pDwork[0];
	if ((UCHAR *)pDwork_slot <= (UCHAR *)&pDwork[9]->Model_no) {
		do {
			*(USHORT *)&pDwork_slot->Be_flg = 0;
			pDwork_slot = (DOOR_WORK *)((USHORT *)&pDwork_slot->Be_flg + 1);
		} while ((UCHAR *)pDwork_slot <= (UCHAR *)&pDwork[9]->Model_no);
	}

	G.Ctpage = 0x15;
	G.Cclut  = 0x1f;

	/* Relocate the door data header (file-relative offsets) and set up pre-packet buffer pointers */
	Data_adr = (ULONG *)((UCHAR *)&Door_data + (ULONG)Door_data.pTim);
	Door_data.pTmd = (ULONG *)((UCHAR *)&Door_data + (ULONG)Door_data.pTmd);
	Door_data.pTim = Data_adr;

	pDoor[0]->pPrepacket = Data_adr;
	Mapping_tim(Data_adr);

	Data_adr = Door_data.pTmd;
	pDoor[0]->pTmd_adr = Door_data.pTmd;

	/* Select clut page based on texture type */
	if ((G.pSce_aot_on)->Dtex_type == 40) {
		Tmd_tpage = 0;
		Tmd_clut = 0;
	} else {
		Tmd_tpage = 0x15;
		Tmd_clut = 0x1f;
	}
	Mapping_tmd(1, Data_adr, Tmd_tpage, Tmd_clut);

	Task_idx = 10;
	Data_adr = pDoor[0]->pTmd_adr;

	pDoor[1] = (DOOR *)0x0;

	pDoor[0]->Divp.ndiv        = 3;
	pDoor[0]->Divp.pih         = 0x140;
	pDoor[0]->Divp.piv         = 0xf0;
	pDoor[0]->Ctr2             = 0;
	pDoor[0]->pTmd_adr         = Data_adr + 3;

	/* Initialise the four scene task slots 10..13 */
	for (pTask = &Sce.Sce_task[10]; Task_idx < 0xe; Task_idx++, pTask++) {
		pTask->Task_level   = (UCHAR)Task_idx;
		pTask->Status       = 0;
		pTask->Sub_ctr      = 0;
		pTask->Ifel_ctr[0]  = -1;
		pTask->Loop_ctr[0]  = -1;
	}

	Sce.pCScd = Door_data.Scd;
	Event_init(Sce.Sce_task + 10, 0);
	SetGeomScreen(0x122);
	Set_view(Camera.View_p, Camera.View_r);
	Bg_set_mode(2, 0);

	G.SaveData.DSce_work[1] = 1;
	G.SaveData.DSce_work[0] = (G.pSce_aot_on)->Door_type & 0xff7f;
	G.SaveData.DSce_work[2] = (G.pSce_aot_on)->Door_type & 0x80;
	G.SaveData.DSce_work[3] = (short)(G.pSce_aot_on)->Dtex_type;

	pDoor[0]->Sound_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Door_move                                                            */
/* Main door update loop: waits for vblank, drives scene scheduler,    */
/* renders door, optionally prints lock text, then sleeps one frame.   */
/* ------------------------------------------------------------------ */
/* DS 0x80013eb4 (Door_move) */
void Door_move(void)
{
	register UINT System_flg;
	register DOOR *pDr;

	/* Wait for any pending system mode transition */
	System_flg = G.System_flg & SYS_FADE_OUT;
	while (System_flg != 0) {
		Task_sleep(1);
		System_flg = G.System_flg & SYS_FADE_OUT;
	}

	if ((pDwork[0]->Free & 0x8000) == 0) {
		Main.Vloop = 0;
	}

	while (Sce.Sce_task[10].Status != 0) {
		if ((G.Status_flg & STAT_DOOR_LOAD) == 0) {
			G.SaveData.DSce_work[1] = 0;
		}
		Door_scheduler_main();
		Door_Trans();

		/* Print lock message overlays */
		pDr = pDoor[0];
		if ((ULONG)(pDr->Ctr2 - 0x29) < 0xdb) {
			if (G.SaveData.DSce_work[3] == 0x32) {
				Mess_print(0x20, 200, door_msg + door_msg_ptr[0], 0x4000);
			}
			if (G.SaveData.DSce_work[3] == 0x34) {
				Mess_print(0x20, 0xb8, door_msg + door_msg_ptr[1], 0x4000);
				Mess_print(0x20, 200,  door_msg + door_msg_ptr[2], 0x4000);
			}
		}

		pDr->Ctr2++;
		Task_sleep(1);
	}
}

/* ------------------------------------------------------------------ */
/* Door_scheduler_main                                                  */
/* Drives the door scene script interpreter for slots 10-13.           */
/* ------------------------------------------------------------------ */
/* DS 0x80014058 (Door_scheduler_main) */
void Door_scheduler_main(void)
{
	signed char Ifel_idx;
	ULONG     Op_result;
	SCE_TASK *pSce;
	LONG      Task_idx;

	Sce.Type  = 0x01;
	Sce.pCScd = Door_data.Scd;

	/* DS reloads the constants 1 and 2 and the lui of Sce per iteration (s0-s3 only); the for/continue form
	   keeps CCPSX from holding them in saved registers. */
	for (Task_idx = 10; Task_idx < 0xe; Task_idx++) {
		pSce = &Sce.Sce_task[Task_idx];

		if (pSce->Status == 0) {
			continue;
		}
		while (1) {
			Op_result = (*(int (*)(SCE_TASK *))Sce_jmp_tbl[*pSce->Data])(pSce);
			if (Op_result == 1) {
				continue;
			}
			if (Op_result == 2) {
				break;
			}
			if ((signed char)pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] < 0) {
				break;
			}
			Op_result = (ULONG)pSce->pS_SP;
			pSce->pS_SP = (ULONG *)(Op_result - 4);
			Ifel_idx = pSce->Sub_ctr;
			pSce->Data = (UCHAR *)*pSce->pS_SP;
			pSce->Ifel_ctr[Ifel_idx]--;
		}
	}
}


/* ------------------------------------------------------------------ */
/* Door_exit                                                            */
/* Tears down the door task: waits for fade, triggers fade-out,        */
/* plays door-close sound, signals main loop, clears system flag.      */
/* ------------------------------------------------------------------ */
/* DS 0x8001417c (Door_exit) */
void Door_exit(void)
{
	ULONG Fade_stat;

	/* goto loop: CCPSX does not hoist the lui of pDoor/Main/G into saved registers across it, as in DS (frame [ra] only) */
Door_exit_wait:
	Fade_stat = Fade_status(0);
	if (Fade_stat == 0) {
		Task_sleep(1);
		goto Door_exit_wait;
	}

	Fade_set(0x200, 0, 7, 1);
	Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0x0);

	if (pDoor[0]->Sound_flg != 0) {
		Snd_se_stad(0x10000, (VECTOR *)&G.Player.Pos_x);
	}

	Main.Vloop   = 0x02;
	G.System_flg &= ~SYS_DOOR_PROCESS;
	Task_exit();
}

/* ------------------------------------------------------------------ */
/* Door_Trans                                                           */
/* GTE lighting pass and polygon sort for all active door work slots.  */
/* ------------------------------------------------------------------ */
/* Double-buffered prepacket area: the second half starts 0x1400 ULONGs (0x5000 bytes) into pPrepacket (retail Door_Trans) */
typedef ULONG DOOR_PACKET_HALF[0x1400];

/* DS 0x80014234 (Door_Trans) */
void Door_Trans(void)
{
	USHORT      Anim_val;
	UINT        Anim_col;
	MATRIX     *pMtx_tmp;
	POLY_GT3   *pPacket_buf;
	MATRIX     *pWorkm_mtx;
	DOOR_WORK  *pCdwork;
	LONG        Slot_idx;
	DOOR       *pDoor_tmp;
	MATRIX      Temp_matrix;	/* DS frame (0x40) reserves this 32-byte light matrix at sp+0x10 (stclmv target) */

	/* Select packet buffer half */
	pDoor_tmp = pDoor[0];   /* DS loads pDoor once before the branch, then reloads it for Ctr1 */
	if (G.Cbuf_id == 0) {
		pPacket_buf = (POLY_GT3 *)pDoor_tmp->pPrepacket;
	} else {
		pPacket_buf = (POLY_GT3 *)((DOOR_PACKET_HALF *)pDoor_tmp->pPrepacket)[1];
	}

	pDoor_tmp->pPackettop = pPacket_buf;
	pDoor[0]->Ctr1 = 0;

	Slot_idx      = 0;
	pCdwork  = pDwork[0];

	/* DS re-forms the constants (0x44/0x88, D_COLORMATRIX, ...) and the lui of each global inside the
	   loop and keeps only pCdwork, the Attribute pointer and the counter in s0-s2. A goto loop has no
	   loop-invariant hoisting in CCPSX, which reproduces that frame (s0-s2). */
Door_Trans_loop:
	{
		if (pCdwork->Be_flg != 0) {
			RotMatrix((SVECTOR *)&pCdwork->Cdir_z, (MATRIX *)pCdwork->M);

			/* Set ambient light colour */
			if ((pCdwork->Attribute2 & 0x1000) == 0) {
				gte_SetBackColor(0x44, 0x44, 0x44);
			} else {
				gte_SetBackColor(0x88, 0x88, 0x88);
			}

			gte_SetColorMatrix((MATRIX *)&D_COLORMATRIX);
			gte_SetRotMatrix((MATRIX *)&D_LIGHTMATRIX);

			pWorkm_mtx = &pCdwork->Workm;
			gte_ldclmv(&pCdwork->Workm);
			gte_rtir();
			pMtx_tmp = &Temp_matrix;		/* light matrix block: retail frame (0x40) reserves this 32-byte MATRIX at sp+0x10 */
			gte_stclmv(&Temp_matrix);

			gte_ldclmv((MATRIX *)&pCdwork->Workm.m[1]);
			gte_rtir();
			gte_stclmv((MATRIX *)&Temp_matrix.m[0][1]);

			gte_ldclmv((MATRIX *)&pCdwork->Workm.m[1][1]);
			gte_rtir();
			gte_stclmv((MATRIX *)&Temp_matrix.m[0][2]);

			gte_SetLightMatrix(&Temp_matrix);

			/* Animate door frame counter if animated flag set */
			if ((pCdwork->Attribute2 & 0x100) != 0) {
				Anim_val = (SHORT)pCdwork->Free + 8;
				pCdwork->Free = Anim_val & 0x1ff;
				if ((Anim_val & 0x100) == 0) {
					Anim_val &= 0xff;
				} else {
					Anim_val = 0x100 - (Anim_val & 0xff);
				}
				pCdwork->Free2 = Anim_val;
				Anim_col = (UINT)pCdwork->Free2;
				pCdwork->Poly_rgb = Anim_col << 0x10 | Anim_col << 8 | Anim_col;
			}

			/* Build world-space transform and sort polygons */
			pMtx_tmp = (MATRIX *)pCdwork->pSuper;
			gte_SetRotMatrix(pMtx_tmp);
			gte_ldclmv((MATRIX *)pCdwork->M);
			gte_rtir();
			gte_stclmv(&pCdwork->Workm);
			gte_ldclmv((MATRIX *)&pCdwork->M[0][1]);
			gte_rtir();
			gte_stclmv((MATRIX *)&pCdwork->Workm.m[0][1]);
			gte_ldclmv((MATRIX *)&pCdwork->M[0][2]);
			gte_rtir();
			gte_stclmv((MATRIX *)&pCdwork->Workm.m[0][2]);

			gte_SetTransMatrix(pMtx_tmp);
			gte_ldlv0((VECTOR *)((SHORT *)&pCdwork->Pos_x + 1));
			gte_rt();
			gte_stlvl((VECTOR *)pCdwork->Workm.t);
			gte_SetRotMatrix(&pCdwork->Workm);
			gte_SetTransMatrix(&pCdwork->Workm);

			DoorSort((MODEL_WORK *)&pCdwork->Attribute, pCdwork, (TMD_MEM *)pCdwork->pTmd);
		}

		Slot_idx++;
		pCdwork++;
	}
	if (Slot_idx < 10) {
		goto Door_Trans_loop;
	}
}

/* ------------------------------------------------------------------ */
/* DoorSort                                                             */
/* GTE polygon transform, back-face cull, lighting and OT sort for     */
/* one door model. Handles subdivision (DivideGT3) when flag set.      */
/* ------------------------------------------------------------------ */
/* DS 0x8001468c (DoorSort) */
/* Entry index of the 0x80 near band in G.Ot1: DS 0x7fc byte offset / 4 = 511 (DS 0x8001468c DoorSort). */
#define DOOR_OT_NEAR_ENTRY	511
void DoorSort(MODEL_WORK *mw, DOOR_WORK *pDoorwork, TMD_MEM *Tmd)
{
	USHORT     Free_new;
	USHORT     Free_flg;
	SVECTOR   *s2;
	ULONG      Poly_left;
	ULONG     *pNor_top;
	ULONG      Addr_mask;
	SHORT     *pZ_scale;
	UINT      *pPacket_cur;
	SVECTOR   *s1;
	SHORT     *pPrim_cur;
	ULONG     *pOt_base;
	ULONG     *pVer_top;
	SVECTOR   *s0;
	ULONG     *pPrim_iter;         /* prim_top iteration pointer */

	Free_flg      = pDoorwork->Free;

	/* Clamp animation counter */
	if (((Free_flg & 0x400) != 0) &&
		(pDoor[0]->Ctr2 == pDoorwork->Sca_info_hi)) {
		Free_new = Free_flg | 0x80;
		if ((Free_flg & 0xc0) == 0xc0) {
			Free_new = Free_flg & 0xff7f;
		}
		pDoorwork->Free = Free_new;
	}

	pPrim_iter = Tmd->prim_top;
	Poly_left  = Tmd->n_poly;
	pVer_top = Tmd->ver_top;
	pNor_top      = Tmd->nor_top;
	pPacket_cur      = (UINT *)mw->pPacket + (UINT)G.Cbuf_id * 10;

	((CVECTOR *)&mw->pSuper)->cd =
		(UCHAR)((pDoorwork->Free >> 0xd) & 2) | 0x34;
	gte_ldrgb((CVECTOR *)&mw->pSuper);

	Addr_mask = 0xffffff;
	s0      = (SVECTOR *)(pVer_top + *(SHORT *)((int)pPrim_iter + 2) * 2);
	s1      = (SVECTOR *)(pVer_top + *(SHORT *)((int)pPrim_iter + 6) * 2);
	s2      = (SVECTOR *)(pVer_top + *(SHORT *)((int)pPrim_iter + 10) * 2);

	if (Poly_left != 0) {
		pZ_scale  = &G.SaveData.Sce_work[0];
		pPrim_cur = (SHORT *)((int)pPrim_iter + 10);

		do {
			UINT      *pPrim_code;
			POLY_GT3  *pPoly_gt3;
			ULONG      Otz_avg;
			ULONG      Addr_tmp;
			SHORT     *pPrim_next;
			SVECTOR   *s_nor;
			UINT      *pOt_entry;
			LONG       Nclip_result;
			ULONG      Ot_tmp;

			/* Optional Z-scale for perspective warp */
			if ((pDoorwork->Free & 0x2000) != 0) {
				s1->vz = (SHORT)((UINT)((int)s1->vz * (int)*pZ_scale) >> 8);
				s2->vz = (SHORT)((UINT)((int)s2->vz * (int)*pZ_scale) >> 8);
				s0->vz = (SHORT)((UINT)((int)s0->vz * (int)*pZ_scale) >> 8);
			}

			gte_ldv3(s0, s1, s2);
			gte_rtpt();
			Poly_left--;

			/* Save verts for DivideGT3 if subdivision active */
			if ((pDoorwork->Free & 0x20) != 0) {
				SHORT Vtx_y, Vtx_z, Vtx_pad;
				DOOR *pDoor_cur;

				Vtx_y = s0->vy; Vtx_z = s0->vz; Vtx_pad = s0->pad;
				pDoor_cur = pDoor[0];
				pDoor_cur->V0.vx  = s0->vx;
				pDoor_cur->V0.vy  = Vtx_y;
				pDoor_cur->V0.vz  = Vtx_z;
				pDoor_cur->V0.pad = Vtx_pad;

				Vtx_y = s1->vy; Vtx_z = s1->vz; Vtx_pad = s1->pad;
				pDoor_cur = pDoor[0];
				pDoor_cur->V1.vx  = s1->vx;
				pDoor_cur->V1.vy  = Vtx_y;
				pDoor_cur->V1.vz  = Vtx_z;
				pDoor_cur->V1.pad = Vtx_pad;

				Vtx_y = s2->vy; Vtx_z = s2->vz; Vtx_pad = s2->pad;
				pDoor_cur = pDoor[0];
				pDoor_cur->V2.vx  = s2->vx;
				pDoor_cur->V2.vy  = Vtx_y;
				pDoor_cur->V2.vz  = Vtx_z;
				pDoor_cur->V2.pad = Vtx_pad;
			}

			s_nor = (SVECTOR *)(pNor_top + pPrim_cur[-3] * 2);
			s1 = (SVECTOR *)(pNor_top + pPrim_cur[-1] * 2);
			s2 = (SVECTOR *)(pNor_top + (SHORT)*pPrim_iter * 2);

			gte_nclip();

			pPrim_next = pPrim_cur + 6;
			pPrim_iter += 3;
			s0      = (SVECTOR *)(pVer_top + pPrim_cur[2] * 2);

			gte_stopz((long *)&Nclip_result);

			if (Nclip_result < 0) {
				/* Back face: skip lighting, advance vertex pointers */
				pDoor[0]->Ctr1++;
				s1 = (SVECTOR *)(pVer_top + pPrim_next[-2] * 2);
				s2 = (SVECTOR *)(pVer_top + *pPrim_next * 2);
			} else {
				/* Front face: light, project, OT-link */
				gte_ldv3(s2, s_nor, s1);
				gte_ncct();

				s1 = (SVECTOR *)(pVer_top + pPrim_next[-2] * 2);
				s2 = (SVECTOR *)(pVer_top + *pPrim_next * 2);

				gte_stsxy3_gt3(pPacket_cur);
				gte_strgb3_gt3(pPacket_cur);
				gte_avsz3();
				gte_stotz((long *)&Ot_tmp);

				Otz_avg = Ot_tmp;
				if (Otz_avg >> 6 != 0) {
					Free_flg      = pDoorwork->Free & 0xc0;
					pOt_base = G.pOt1;

					if (Free_flg == 0x80) {
						pOt_entry = (UINT *)(pOt_base + DOOR_OT_NEAR_ENTRY + (Otz_avg >> 7));
					} else if (Free_flg == 0x40) {
						Addr_tmp = (Otz_avg >> 0xc) << 2;
						pOt_base = G.pOt2;
						pOt_entry = (UINT *)((int)pOt_base + Addr_tmp);
					} else if (Free_flg == 0xc0) {
						Addr_tmp = (Otz_avg >> 7) << 2;
						pOt_entry = (UINT *)((int)pOt_base + Addr_tmp);
					}

					if ((pDoorwork->Free & 0x20) == 0) {
						/* Standard OT link */
						Ot_tmp = *pOt_entry;
						*pOt_entry = (UINT)pPacket_cur & Addr_mask;
						*pPacket_cur = (Ot_tmp & Addr_mask) | 0x9000000;
					} else {
						/* Subdivided polygon via DivideGT3 */
						pPrim_code = pPacket_cur + 1;
						pPoly_gt3 = pDoor[0]->pPackettop;

						pPoly_gt3 = (POLY_GT3 *)DivideGT3(
							&pDoor[0]->V0, &pDoor[0]->V1,
							&pDoor[0]->V2, (u_long *)(pPacket_cur + 3),
							(u_long *)(pPacket_cur + 6),
							(u_long *)(pPacket_cur + 9),
							(CVECTOR *)pPrim_code,
							(CVECTOR *)pPrim_code,
							(CVECTOR *)pPrim_code,
							pPoly_gt3,
							(u_long *)pOt_entry,
							&pDoor[0]->Divp);

						pDoor[0]->pPackettop = pPoly_gt3;
					}
				}

				pDoor[0]->Ctr1++;
			}

			pPacket_cur += 0x14;
			pPrim_cur = pPrim_next;
		} while (Poly_left != 0);
	}
}

/* ------------------------------------------------------------------ */
/* Door_model_init                                                      */
/* Binds a DOOR_WORK slot to a TMD model and allocates packet memory.  */
/* ------------------------------------------------------------------ */
/* DS 0x80014b40 (Door_model_init) */
void Door_model_init(DOOR_WORK *pTmpDwork, ULONG Model_no)
{
	DOOR  *pDoor_cur;
	ULONG *Tmd_adr;

	pDoor_cur  = pDoor[0];
	Tmd_adr = pDoor[0]->pTmd_adr;

	pTmpDwork->Poly_rgb = 0x808080;
	pTmpDwork->pTmd   = Tmd_adr + Model_no * 0xe;
	pTmpDwork->pTmd2  = Tmd_adr + Model_no * 0xe + 7;

	Tmd_adr = (ULONG *)PresetObjectGT3_neo(
		(MODEL_WORK *)&pTmpDwork->Attribute, pDoor_cur->pPrepacket, 0);
	pDoor[0]->pPrepacket = Tmd_adr;
}

/* ------------------------------------------------------------------ */
/* Door_model_set                                                       */
/* Scene-script opcode handler: reads model parameters from script     */
/* data and configures the corresponding DOOR_WORK slot.               */
/* ------------------------------------------------------------------ */
/* Door_model_set script record (22 bytes), read in place from pSce->Data. Field names are the DOOR_WORK fields the
 * bytes are copied into; Unk_00 is not used and Unk_01 selects the pDwork slot. Retail reads in Door_model_set:
 * 0x80014bb8 lbu +1, 0x80014bd0 lbu +2, 0x80014bdc lbu +3, 0x80014be8 lbu +4, 0x80014bf4 lbu +5, 0x80014c00 lhu +6,
 * 0x80014c0c lh +8, 0x80014c18 lh +0xa, 0x80014c24 lh +0xc, 0x80014c30 lh +0xe, 0x80014c3c lhu +0x10 (the port keeps
 * Cdir_x signed), 0x80014c48 lhu +0x12, 0x80014c58 lhu +0x14; 0x80014cac addiu v0,a1,0x16 is the record size. */
typedef struct {
	UCHAR Unk_00;
	UCHAR Unk_01;
	UCHAR Id;
	UCHAR Unk_03;	/* +3, stored at retail 0x80014be4 (sh 0x10e); meaning not known */
	UCHAR Be_flg;
	UCHAR Unk_05;	/* +5, stored at retail 0x80014bfc (sh 0x146); meaning not known */
	USHORT Unk_06;	/* +6, stored at retail 0x80014c08 (sh 0x144); meaning not known */
	SHORT Attribute;
	SHORT Pos_x;
	SHORT Pos_y;
	SHORT Pos_z;
	SHORT Cdir_x;
	USHORT Cdir_y;
	USHORT Cdir_z;
} DOOR_MODEL_SET_REC;

/* DS 0x80014ba4 (Door_model_set) */
ULONG Door_model_set(SCE_TASK *pSce)
{
	ULONG      Free_flg;
	MATRIX    *pMat;
	DOOR_WORK *pDwork_slot;
	DOOR_MODEL_SET_REC *pData;
	ULONG      Frame_pad[2];

	(void)Frame_pad;
	pData = (DOOR_MODEL_SET_REC *)pSce->Data;
	pDwork_slot    = pDwork[pData->Unk_01];

	pDwork_slot->Id                              = pData->Id;
	pDwork_slot->Sca_info_hi                      = (USHORT)pData->Unk_03;
	pDwork_slot->Be_flg                          = (UINT)pData->Be_flg;
	pDwork_slot->Free2                           = (USHORT)pData->Unk_05;
	Free_flg = pData->Unk_06;
	pDwork_slot->Free                            = Free_flg;
	pDwork_slot->Attribute                       = (int)pData->Attribute;
	pDwork_slot->Pos_x                             = (int)pData->Pos_x;
	pDwork_slot->Pos_y                             = (int)pData->Pos_y;
	pDwork_slot->Pos_z                             = (int)pData->Pos_z;
	pDwork_slot->Cdir_x                          = pData->Cdir_x;
	pDwork_slot->Cdir_y                          = pData->Cdir_y;
	pDwork_slot->Cdir_z                          = pData->Cdir_z;

	/* Choose reference matrix: world or parent door slot */
	if ((Free_flg & 0x10) == 0) {
		pMat = &GsWSMATRIX;
	} else {
		pMat = &pDwork[Free_flg & 0xf]->Workm;
	}
	pDwork_slot->pSuper = (ULONG *)pMat;

	if ((Free_flg & 0x800) != 0) {
		pDoor[0]->Sound_flg = 1;
	}

	pSce->Data = (UCHAR *)(pData + 1);
	Door_model_init(pDwork_slot, (UINT)pDwork_slot->Free2);
	return 1;
}

/* ------------------------------------------------------------------ */
/* Door_snd_trans                                                       */
/* Loads door sound bank from CD into SPU and opens it as a VAB.       */
/* Retries on timeout; handles Bg_set_mode deferral until data ready.  */
/* ------------------------------------------------------------------ */
/* DS 0x80014cd0 (Door_snd_trans) */
void Door_snd_trans(void)
{
	SHORT        Bg_clr_sw;
	ULONG        Sound_size;
	DOOR_INFO   *pDoor_info;
	UCHAR        Door_no;
	ULONG        Sector;
	ULONG        File_no;
	SHORT        Vab_ret;
	ULONG        Hdr_word1;
	ULONG        Hdr_word2;
	ULONG        Hdr_word3;
	ULONG       *pHdr_dst;
	ULONG       *Address;

	Bg_clr_sw = 0;
	Door_no   = (G.pSce_aot_on)->Dtex_type;

retry:
	do {
		SpuSetTransferMode(0);
		pDoor_info = &Dinfo[(G.pSce_aot_on)->Dtex_type];
		G.Status_flg |= STAT_DOOR_LOAD;

		/* Wait for previous texture DMA */
		while (pDoor[1] != (DOOR *)0x0) {
			Task_sleep(1);
		}

		File_no    = (ULONG)Dtex_file[Door_no];
		Sound_size = (ULONG)pDoor_info->Sound_size;

		CD.Fsum    = pDoor_info->Sound_sum;
		Sector     = -(int)Sound_size;
		Address    = (ULONG *)((UCHAR *)&Door_data + Sector);

		CD.Fsector = (UINT)floc[File_no].sector +
					 (UINT)floc[File_no].sec_high * 0x10000;
		CD.Fsize   = Sound_size;

		Cd_read(File_no, Address, 3, (UCHAR *)"DOOR_SOUND");

		/* Copy VAB header to staging area */
		pHdr_dst = (ULONG *)&Door_snd_head;
		do {
			Hdr_word1 = Address[1];
			Hdr_word2 = Address[2];
			Hdr_word3 = Address[3];
			*pHdr_dst   = *Address;
			pHdr_dst[1] = Hdr_word1;
			pHdr_dst[2] = Hdr_word2;
			pHdr_dst[3] = Hdr_word3;
			Address += 4;
			pHdr_dst += 4;
		} while (Address != (ULONG *)&((DOOR_SND_HEAD *)((UCHAR *)&Door_data + Sector))->Vh_offset);

		Hdr_word1 = Address[1];
		*pHdr_dst   = *Address;
		pHdr_dst[1] = Hdr_word1;

		EDT_SLOT_PTR(2) = (ULONG *)&Door_snd_head;
		pVh_tbl[0]  = (ULONG *)((UCHAR *)&Door_snd_head + Door_snd_head.Vh_offset);

		/* Close previous VAB if open */
		if ((signed char)Vab_id[0] >= 0) {
			SsVabClose((SHORT)(signed char)Vab_id[0]);
			Vab_id[0] = -1;
		}

		/* Wait for any pending SPU transfer to complete */
		File_no = 0;
		while ((Vab_ret = SsVabTransCompleted(0)) == 0) {
			Task_sleep(1);
		}

		/* Open VH with timeout retry */
		do {
			Vab_ret = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[0], 0, 0x3dc50);
			Vab_id[0] = (char)Vab_ret;

			if (((G.System_flg & SYS_FADE_OUT) == 0) && (!Bg_clr_sw)) {
				Bg_set_mode(2, 0);
				Bg_clr_sw = 1;
			}

			Task_sleep(1);
			File_no++;
			if (0xb4 < File_no) goto retry;
		} while ((signed char)Vab_id[0] == -1);

		/* Transfer VB body with timeout retry */
		File_no = 0;
		while (1) {
			Task_sleep(1);
			File_no++;
			if (0xb4 < File_no) break;

			Vab_ret = SsVabTransBody(
				(UCHAR *)((UCHAR *)&Door_data + sizeof(DOOR_SND_HEAD) - Sound_size), (SHORT)(signed char)Vab_id[0]);

			if (Vab_ret != -1) {
				Snd_cd_watchdog_ctr = 0;
				do {
					Snd_cd_watchdog();
					if (((G.System_flg & SYS_FADE_OUT) == 0) && (!Bg_clr_sw)) {
						Bg_set_mode(2, 0);
						Bg_clr_sw = 1;
					}
					Task_sleep(1);
					Vab_ret = SsVabTransCompleted(0);
				} while (Vab_ret == 0);

				G.Status_flg &= ~STAT_DOOR_LOAD;
				return;
			}
		}
	} while (1); /* outer retry loop */
}

/* ------------------------------------------------------------------ */
/* Door_texture_load                                                    */
/* Issues a CD read for the door texture data into the work buffer.    */
/* ------------------------------------------------------------------ */
/* DS 0x80015064 (Door_texture_load) */
void Door_texture_load(void)
{
	ULONG       Sector;
	ULONG       File_no;
	DOOR_INFO  *pDoor_info;
	UCHAR       Door_no;

	Door_no    = (G.pSce_aot_on)->Dtex_type;
	pDoor_info = &Dinfo[Door_no];
	File_no    = (ULONG)Dtex_file[Door_no];

	CD.Fsum    = pDoor_info->Data_sum;
	Sector     = (UINT)floc[File_no].sector +
				 (UINT)floc[File_no].sec_high * 0x10000 +
				 pDoor_info->Sector_offset;
	CD.Fsector = Sector;
	CD.Fsize   = (ULONG)pDoor_info->Data_size;

	G.Status_flg |= STAT_DOOR_LOAD;
	Cd_read(File_no, (ULONG *)&Door_data, 3, (UCHAR *)"DOOR_TEXTURE");
}

/* ------------------------------------------------------------------ */
/* Bgm_unlock_ck                                                 80015128 */
/* Retail addition. Once 7 or more of the item flags 0xf8..0xff are    */
/* set, sets Common_flg 0xfc and unlocks the extra BGM table entries.  */
/* ------------------------------------------------------------------ */
/* DS 0x80015128 (Bgm_unlock_ck) */
void Bgm_unlock_ck(void)
{
	ULONG Flg_cnt;
	ULONG Flg_no;
	ULONG Bgm_no;

	Flg_cnt = 0;
	if (Flg_ck(G.SaveData.Common_flg, 0xfc) == 0) {
		for (Flg_no = 0xf8; Flg_no < 0x100; Flg_no++) {
			if (Flg_ck(G.SaveData.Item_flg, Flg_no) != 0) {
				Flg_cnt++;
			}
		}
		if (6 < Flg_cnt) {
			Flg_on(G.SaveData.Common_flg, 0xfc);
			Snd_bgm_tbl_set(0x105ff34);
			for (Bgm_no = 7; Bgm_no < 0x18; Bgm_no++) {
				if (Bgm_no == 0xf) {
					Bgm_no = 0x10;
				}
				Snd_bgm_tbl_set(Bgm_no << 0x10 | 0x100ff34);
			}
			for (Bgm_no = 0; Bgm_no < 0x1c; Bgm_no++) {
				if (Bgm_no == 8) {
					Bgm_no = 9;
				}
				if (Bgm_no == 0x17) {
					Bgm_no = 0x19;
				}
				Snd_bgm_tbl_set(Bgm_no << 0x10 | 0x200ff34);
			}
			for (Bgm_no = 0; Bgm_no < 0xc; Bgm_no++) {
				if (Bgm_no == 5) {
					Bgm_no = 7;
				}
				Snd_bgm_tbl_set(Bgm_no << 0x10 | 0x300ff34);
			}
		}
	}
}

/* ================================================================== */
/* Extended door / clear-time sequence (separate code block in DS)     */
/* ================================================================== */

static UCHAR        Door_ex_cnt;          /* 800dcc1c  frames left in current phase   */
static UCHAR        Door_ex_cnt_max;      /* 800dcc1d  frames in current phase        */
UCHAR               Door_ex_rno;          /* 800dcc1e  index into pDoor_proc_tbl_ex   */
static UCHAR        Door_ex_flg;          /* 800dcc1f  0x40 zapping, 0x80 fade done   */
static ULONG        Door_ex_clear_time;   /* 800dcc28  clear time in frames           */
static POLY_FT4     Door_ex_ft4;          /* 800dcc78  "clear" logo sprite            */
static SHORT        Door_ex_ft4_dx;       /* 800dcca0  logo zoom (x)                  */
static SHORT        Door_ex_ft4_dy;       /* 800dcca2  logo zoom (y)                  */
static DOOR_EX_LINE Door_ex_line[2];      /* 800ead80  clear-time text lines          */


/* DS 0x800d4dd8, DOOR_WORK slot pointers (all zero in the EXE image) */
DOOR_WORK *pDwork[10];

/* ------------------------------------------------------------------ */
/* Door_main_ex                                                  800335c0 */
/* Extended (zapping) door sequence driver: walks pDoor_proc_tbl_ex    */
/* until a NULL entry and then exits the task.                          */
/* ------------------------------------------------------------------ */
/* DS 0x800335c0 (Door_main_ex) */
void Door_main_ex(void)
{
	ULONG Frame_pad[2];

	UCHAR *pRno = &Door_ex_rno;

	(void)Frame_pad;
	while (pDoor_proc_tbl_ex[*pRno] != NULL) {
		(*pDoor_proc_tbl_ex[*pRno])();
		(*pRno)++;
	}
	Task_exit();
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_init                                             80033674 */
/* Phase 0: records the clear time, starts the player's end motion,    */
/* loads the clear logo / sound data (scenario clear only) and sets up */
/* the cinematic camera and light.                                      */
/* ------------------------------------------------------------------ */
/* DS 0x80033674 (Door_proc_ex_init) */
void Door_proc_ex_init(void)
{
	ENEMY_WORK *pEm;
	SHORT       Stage_no;
	ULONG       File_no;
	UCHAR      *Mess;

	Door_ex_flg = 0;
	Door_ex_clear_time = (G.SaveData.Game_play_time - G.SaveData.Front_play_time)
					   + VSync(-1) - G.Game_time_offset;
	pEm   = G.pEnemy[0];
	G.pEm = pEm;

	if (G.SaveData.Item_flg[7] == 0xff) {
		/* zapping hand-over: no clear screen, just the exit motion */
		Door_ex_flg |= 0x40;
		pEm->Move_no   = 1;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = 7;
		pEm->Mplay_flg = 0;
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		Door_ex_cnt_max = 0x78;
		Door_ex_cnt     = 0x78;
		*(ULONG *)&G.ExData[60]   = (Door_ex_clear_time / 0x3c) * 0x3c;
		G.SaveData.ExData3[0x16]  = *(ULONG *)&G.ExData[60];
	} else {
		Snd_bgm_fade_ON(90, 0x10);
		Door_ex_flg = (G.SaveData.Stage_no != 5);
		if (Door_ex_flg != 0) {
			File_no = CLEAR1_PIX;
			Mess    = (UCHAR *)"clear1.tim";
			*(ULONG *)&G.ExData[56]  = (Door_ex_clear_time / 0x3c) * 0x3c;
			G.SaveData.ExData3[0x15] = *(ULONG *)&G.ExData[56];
		} else {
			File_no = CLEAR2_PIX;
			Mess    = (UCHAR *)"clear2.tim";
			*(ULONG *)&G.ExData[52]  = (Door_ex_clear_time / 0x3c) * 0x3c;
			G.SaveData.ExData3[0x14] = *(ULONG *)&G.ExData[52];
		}
		Cd_read(File_no, Bg_img_buf, 0, Mess);

		pEm->Move_no   = 2;
		pEm->Move_cnt  = 0;
		pEm->Hokan_flg = 0;
		pEm->Mplay_flg = 0;
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);

		Cd_read(STG_NXT, Door_ex_data, 0, StgNxt);
		Door_proc_ex_snd_load();

		Stage_no = G.SaveData.Stage_no;
		G.SaveData.Stage_no = 7;
		Xa_play(0, 0);
		G.Ctpage = 0x15;
		G.Cclut  = 0x1f;
		G.SaveData.Stage_no = Stage_no & 0xff;
		Mapping_tim(Door_ex_data);

		SetPolyFT4(&Door_ex_ft4);
		Door_ex_ft4.v2    = 0x28;
		Door_ex_ft4.v3    = 0x28;
		Door_ex_ft4.tpage = 0x95;
		Door_ex_ft4.clut  = 0x7fc0;
		Door_ex_ft4_dx    = 0x36;
		Door_ex_ft4.u0    = 0;
		Door_ex_ft4.v0    = 0;
		Door_ex_ft4.u1    = 0xd4;
		Door_ex_ft4.v1    = 0;
		Door_ex_ft4.u2    = 0;
		Door_ex_ft4.u3    = 0xd4;
		Door_ex_ft4.r0    = 0x80;
		Door_ex_ft4.g0    = 0x80;
		Door_ex_ft4.b0    = 0x80;
		Door_ex_ft4.code  = 0x2e;
		Door_ex_cnt_max   = 0x3c;
		Door_ex_cnt       = 0x3c;
	}

	Door_proc_ex_rcut();
	Door_proc_ex_vcut();
	Door_proc_ex_Set_View();
	Door_proc_ex_lit();
	Bg_set_mode(1, 0);
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_move                                             800339a4 */
/* Phase 1: plays the exit motion while the camera orbits, fading in.  */
/* ------------------------------------------------------------------ */
/* DS 0x800339a4 (Door_proc_ex_move) */
void Door_proc_ex_move(void)
{
	register ENEMY_WORK *pEm;
	register ULONG       *pKan;
	register ULONG       *pSeq;
	ULONG       Late_flg;

	pEm   = G.pEnemy[0];
	G.pEm = pEm;

Door_ex_move_loop:
	if (Door_ex_cnt != 0) {
		pKan = pEm->pSub0_kan_t_ptr;
		pSeq = pEm->pSub0_seq_t_ptr;
		if ((Door_ex_flg & 0x40) == 0) {
			if (Door_ex_cnt == 10) {
				pEm->Move_no   = 1;
				pEm->Move_cnt  = 0;
				pEm->Hokan_flg = 7;
				pEm->Mplay_flg = 0;
				Joint_move(pEm, (ULONG)pKan, (ULONG)pSeq, 0x200);
			}
			Late_flg = (Door_ex_cnt < 10) ? 0x200 : 0x100;
		} else {
			Late_flg = 0x200;
		}
		Joint_move(pEm, (ULONG)pKan, (ULONG)pSeq, Late_flg);

		Fade_set(0x200, 0, 6, 1);
		Fade_adjust(0, 0x400, 0xffffff, NULL);
		Fade_set(0x203, 0, 7, 3);
		Fade_adjust(3, 0x400, 0xffffff, NULL);
		Door_proc_ex_Set_View();

		if (pEm->pTbefore_func != NULL) {
			(*(EM_TRANS_FUNC)pEm->pTbefore_func)(pEm);
		}
		Joint_trans2(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, (VECTOR *)&pEm->Pos_x);
		if (pEm->pTafter_func != NULL) {
			(*(EM_TRANS_FUNC)pEm->pTafter_func)(pEm);
		}
		Task_sleep(1);
		Door_ex_cnt--;
		goto Door_ex_move_loop;
	}
	Door_ex_cnt = 0x3c;
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_end                                              80033b58 */
/* Phase 2: holds the final pose and fades out. Scenario clear sets up */
/* the two result text lines; zapping jumps straight to the next stage.*/
/* ------------------------------------------------------------------ */
/* DS 0x80033b58 (Door_proc_ex_end) */
void Door_proc_ex_end(void)
{
	ENEMY_WORK *pEm;
	ULONG       Mode;
	UCHAR      *pCnt;
	UCHAR      *pFlg;

	pCnt  = &Door_ex_cnt;
	pFlg  = &Door_ex_flg;
	pEm   = G.pEnemy[0];
	G.pEm = pEm;

	while (*pCnt != 0) {
		if (*pCnt == 0xf) {
			Fade_set(0x200, 0x800, 7, 3);
		}
		if (pEm->pTbefore_func != NULL) {
			(*(EM_TRANS_FUNC)pEm->pTbefore_func)(pEm);
		}
		Joint_trans2(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, (VECTOR *)&pEm->Pos_x);
		if (pEm->pTafter_func != NULL) {
			(*(EM_TRANS_FUNC)pEm->pTafter_func)(pEm);
		}
		Task_sleep(1);
		(*pCnt)--;
	}

	if ((*pFlg & 0x40) == 0) {
		Mode = 1;
		Door_ex_line[0].X   = 0x36;
		Door_ex_line[0].Y   = 0x5f;
		Door_ex_line[0].Cnt = 0x15;
		Door_ex_line[1].X   = 0x26;
		Door_ex_line[1].Y   = 0x7b;
		Door_ex_line[0].Idx = 0;
		Door_ex_line[1].Idx = 0;
		Door_ex_line[1].Cnt = 0x1f;
	} else {
		Door_proc_ex_next_set();
		G.System_flg &= ~SYS_DOOR_PROCESS;
		Flg_off(G.SaveData.Zapping_flg, 0x3f);
		Mode = 2;
		Door_ex_rno = 4;                          /* -> table[5] == NULL: exit */
		G.SaveData.Item_flg[7] ^= 0xff;
	}
	Bg_set_mode(Mode, 0);
	Fade_off(3);
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_xa_wait                                          80033d08 */
/* Phase 3: shows the zooming clear logo until the XA track ends.      */
/* ------------------------------------------------------------------ */
/* DS 0x80033d08 (Door_proc_ex_xa_wait) */
void Door_proc_ex_xa_wait(void)
{
Door_xa_wait_loop:
	if ((G.Status_flg & STAT_XA_VOICE) != 0) {
		Door_proc_ex_ft4_trans(&Door_ex_ft4);
		Task_sleep(1);
		goto Door_xa_wait_loop;
	}
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_result                                           80033d60 */
/* Phase 4: types out the clear time and rank lines character by      */
/* character, then waits for a button press.                           */
/* ------------------------------------------------------------------ */
/* DS 0x80033d60 (Door_proc_ex_result) */
void Door_proc_ex_result(void)
{
	ULONG  Time;
	UCHAR  Digit;
	USHORT Wait;
	SHORT  i;
	UCHAR  c;

	Wait = 0x10;
	for (i = 0; i < (SHORT)(Door_ex_line[0].Cnt - 1); i++) {
		Door_ex_buf0[i] = 0;
	}
	for (i = 0; i < (SHORT)(Door_ex_line[1].Cnt - 1); i++) {
		Door_ex_buf1[i] = 0;
	}
	Door_ex_buf1[0] = 0xfe;
	Door_ex_buf0[0] = 0xfe;

	/* clear time -> "HH:MM:SS" digit glyphs (glyph index = digit + 0xc) */
	Time = Door_ex_clear_time;
	if (0x14996c4 < Time) {                       /* cap at 99:59:59 */
		Time = 0x14996c4;
	}
	Digit = (UCHAR)(Time / 0x20f580);                               /* 10 hours */
	Door_ex_mess0[0x0b] = Digit + 0xc;
	Door_ex_mess0[0x0c] = (UCHAR)(Time / 0x34bc0) - Digit * 10 + 0xc;          /* 1 hour  */
	Digit = (UCHAR)((Time % 0x34bc0) / 36000);                      /* 10 min   */
	Door_ex_mess0[0x0e] = Digit + 0xc;
	Door_ex_mess0[0x0f] = (UCHAR)((Time % 0x34bc0) / 0xe10) - Digit * 10 + 0xc; /* 1 min   */
	Digit = (UCHAR)((Time % 0xe10) / 600);                          /* 10 sec   */
	Door_ex_mess0[0x11] = Digit + 0xc;
	Door_ex_mess0[0x12] = (UCHAR)((Time % 0xe10) / 0x3c) - Digit * 10 + 0xc;   /* 1 sec   */

	for (;;) {
		Door_proc_ex_ft4_trans(&Door_ex_ft4);

		if (Door_ex_line[0].Cnt == 0) {
			if ((Door_ex_flg & 0x80) == 0) {
				Bg_set_mode(0, 0);
				Fade_set(0x203, -0x100, 7, 3);
				Door_ex_flg |= 0x80;
			}
			if (Door_ex_line[1].Cnt != 0) {
				if (Wait != 0) {
					Wait--;
				} else {
					if ((Door_ex_flg & 0x1f) == 0) {
						c = Door_ex_mess1a[Door_ex_line[1].Idx];
					} else {
						c = Door_ex_mess1b[Door_ex_line[1].Idx];
					}
					Door_ex_buf1[Door_ex_line[1].Idx] = c;
					if (c == 0xfe) {
						Snd_se_stad(0x10000, NULL);
						Wait = 6;
						Door_ex_line[1].Cnt = 1;
					} else {
						Door_ex_buf1[Door_ex_line[1].Idx + 1] = 0xfe;
						if (c == 0) {
							Snd_se_stad(0x20000, NULL);
							Wait = 5;
						} else {
							Snd_se_stad(0, NULL);
							Wait = 4;
						}
					}
					Door_ex_line[1].Idx++;
					Door_ex_line[1].Cnt--;
				}
			}
		} else if (Wait == 0) {
			c = Door_ex_mess0[Door_ex_line[0].Idx];
			Door_ex_buf0[Door_ex_line[0].Idx] = c;
			if (c == 0xfe) {
				Snd_se_stad(0x10000, NULL);
				Wait = 6;
			} else {
				Door_ex_buf0[Door_ex_line[0].Idx + 1] = 0xfe;
				if (c == 0) {
					Snd_se_stad(0x20000, NULL);
					Wait = 5;
				} else {
					Snd_se_stad(0, NULL);
					Wait = 4;
				}
			}
			Door_ex_line[0].Idx++;
			Door_ex_line[0].Cnt--;
		} else {
			Wait--;
		}

		Mess_print(Door_ex_line[0].X, Door_ex_line[0].Y, Door_ex_buf0, 0x4000);
		Mess_print(Door_ex_line[1].X, Door_ex_line[1].Y, Door_ex_buf1, 0x4000);

		if (Door_ex_line[0].Cnt == 0 && Door_ex_line[1].Cnt == 0) {
			if (((Wait + 1) & 0x10) == 0) {
				Mess_print(0x3e, 0xb9, Door_ex_mess_push, 0x4000);
			}
			Wait++;
			if (G.Padd1_trg != 0) {
				G.Game_time_offset = VSync(-1);
				G.SaveData.Front_play_time = G.SaveData.Game_play_time;
				G.System_flg &= ~SYS_DOOR_PROCESS;
				Flg_off(G.SaveData.Zapping_flg, 0x3f);
				return;
			}
		}
		Task_sleep(1);
	}
}

/* Clear-sequence camera orbit state (800dcc00..800dcc27) */
static LONG   Door_ex_dist;           /* 800dcc00  camera-target distance (XZ)    */
static USHORT Door_ex_dir;            /* 800dcc04  camera direction (start)       */
static USHORT Door_ex_dir_add;        /* 800dcc06  orbit sweep                    */
static LONG   Door_ex_dist_step;      /* 800dcc08  per-frame distance change      */
static LONG   Door_ex_py_add;         /* 800dcc0c  eye height delta               */
static LONG   Door_ex_ry_add;         /* 800dcc10  target height delta            */
static LONG   Door_ex_py;             /* 800dcc14  eye height (start)             */
static LONG   Door_ex_ry;             /* 800dcc18  target height (start)          */
static LONG   Door_ex_cx;             /* 800dcc20  orbit centre x                 */
static LONG   Door_ex_cz;             /* 800dcc24  orbit centre z                 */

/* ------------------------------------------------------------------ */
/* Door_proc_ex_rcut                                             8003432c */
/* Copies the current room cut into Ex_battle_rcut and computes the    */
/* orbit parameters for the clear cinematic.                            */
/* ------------------------------------------------------------------ */
/* DS 0x8003432c (Door_proc_ex_rcut) */
void Door_proc_ex_rcut(void)
{
	RCUT    *pRcut;
	PARTS_W *pParts;
	LONG     View_py;
	LONG     View_ry;
	LONG     Dx;
	LONG     Dz;

	pRcut  = G.pRoom->pRcut + G.SaveData.Cut_no;
	pParts = G.pEnemy[0]->pSin_parts_ptr;

	Ex_battle_rcut.end_flg   = pRcut->end_flg;
	Ex_battle_rcut.ViewR     = pRcut->ViewR;
	Ex_battle_rcut.View_p[0] = pRcut->View_p[0];
	Ex_battle_rcut.View_p[1] = pRcut->View_p[1];
	Ex_battle_rcut.View_p[2] = pRcut->View_p[2];
	Ex_battle_rcut.View_r[0] = pRcut->View_r[0];
	Ex_battle_rcut.View_r[1] = pRcut->View_r[1];
	Ex_battle_rcut.View_r[2] = pRcut->View_r[2];
	Ex_battle_rcut.pSp       = pRcut->pSp;

	Dx = Ex_battle_rcut.View_p[0] - Ex_battle_rcut.View_r[0];
	Dz = Ex_battle_rcut.View_p[2] - Ex_battle_rcut.View_r[2];
	View_py = Ex_battle_rcut.View_p[1];
	View_ry = Ex_battle_rcut.View_r[1];

	Door_ex_dir     = (USHORT)ratan2(Dz, Dx) & 0xfff;
	Door_ex_dir_add = (Door_ex_dir + G.pEnemy[0]->Cdir_y - 300) & 0xfff;
	if ((Door_ex_flg & 0x40) != 0) {
		Door_ex_dir_add += 0x1000;
	}

	Ex_battle_rcut.View_p[1] = 0;
	Ex_battle_rcut.View_r[1] = 0;
	Door_ex_cx   = Ex_battle_rcut.View_r[0];
	Door_ex_cz   = Ex_battle_rcut.View_r[2];
	Door_ex_dist = SquareRoot0(Dx * Dx + Dz * Dz);
	Ex_battle_rcut.View_p[1] = View_py;
	Ex_battle_rcut.View_r[1] = View_ry;

	if ((Door_ex_flg & 0x40) == 0) {
		DIV_S(Door_ex_dist_step, Door_ex_dist - 2500, (LONG)Door_ex_cnt_max);
	} else {
		Door_ex_dist_step = (Door_ex_dist - 2500) >> 1;
	}

	Door_ex_py     = View_py;
	Door_ex_ry     = View_ry;
	Door_ex_py_add = (pParts->Workm.t[1] + 400) - View_py;
	Door_ex_ry_add = (pParts->Workm.t[1] - 200) - View_ry;
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_Set_View                                         8003458c */
/* Per-frame camera update: orbits around the player while easing the  */
/* orbit centre / target towards the player's root part.               */
/* ------------------------------------------------------------------ */
/* DS 0x8003458c (Door_proc_ex_Set_View) */
void Door_proc_ex_Set_View(void)
{
	PARTS_W *pParts;
	LONG     Ang;
	LONG     Rate;
	ULONG    Dir;
	LONG     Cs;

	DIV_S(Ang, (LONG)(Door_ex_cnt_max - Door_ex_cnt) * 0x800, (LONG)Door_ex_cnt_max);
	Ang -= 0x400;
	pParts = G.pEnemy[0]->pSin_parts_ptr;
	Rate   = rsin(Ang) + 0x1000;               /* 0..0x2000 ease curve */

	Dir = (Door_ex_dir - ((LONG)(Door_ex_dir_add * Rate) >> 13)) & 0xfff;

	if ((Door_ex_flg & 0x40) == 0) {
		Door_ex_dist -= Door_ex_dist_step;
	} else {
		if (Door_ex_cnt == 0x50) {
			Door_ex_dist_step /= 2;
		}
		Cs = rcos(Ang * 3 + 0xc00);
		Door_ex_dist = (Door_ex_dist_step * (Cs + 0x1000) >> 12) + 2500;
	}

	Ex_battle_rcut.View_p[0] = Door_ex_cx + (Door_ex_dist * rcos(Dir) >> 12);
	Ex_battle_rcut.View_p[2] = Door_ex_cz + (Door_ex_dist * rsin(Dir) >> 12);
	Ex_battle_rcut.View_p[1] = Door_ex_py + (Door_ex_py_add * Rate >> 13);

	Door_ex_cx += (pParts->Workm.t[0] - Door_ex_cx) / 30;
	Door_ex_cz += (pParts->Workm.t[2] - Door_ex_cz) / 30;

	Ex_battle_rcut.View_r[0] += (pParts->Workm.t[0] - Ex_battle_rcut.View_r[0]) / 30;
	Ex_battle_rcut.View_r[1]  = Door_ex_ry + (Door_ex_ry_add * Rate >> 13);
	Ex_battle_rcut.View_r[2] += (pParts->Workm.t[2] - Ex_battle_rcut.View_r[2]) / 30;

	Set_view(Ex_battle_rcut.View_p, Ex_battle_rcut.View_r);

	if (0xd0 < (Ex_battle_rcut.ViewR >> 7)) {
		Ex_battle_rcut.ViewR -= 0x80;
	}
	SetGeomScreen(Ex_battle_rcut.ViewR >> 7);

	gte_SetRotMatrix(&GsWSMATRIX);
	gte_SetTransMatrix(&GsWSMATRIX);
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_vcut                                             800348d0 */
/* Clears the four switch-zone points of Ex_battle_vcut and makes it   */
/* the current camera-switch cut.                                       */
/* ------------------------------------------------------------------ */
/* DS 0x800348d0 (Door_proc_ex_vcut) */
void Door_proc_ex_vcut(void)
{
	SHORT i;

	for (i = 0; i < 4; i++) {
		Ex_battle_vcut.Xz[i][0] = 0;
		Ex_battle_vcut.Xz[i][1] = 0;
	}
	G.Cccut = &Ex_battle_vcut;
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_lit                                              80034918 */
/* Rebuilds the current cut's lighting: white ambient, lights 1-2 off, */
/* light 0 white (L 4000) just above the player's head (parts[8]).     */
/* ------------------------------------------------------------------ */
/* DS 0x80034918 (Door_proc_ex_lit) */
void Door_proc_ex_lit(void)
{
	LIGHT_CUT_DATA *pLit;
	PARTS_W        *pHead;
	SHORT           i;
	SHORT           j;

	pHead = &G.pEnemy[0]->pSin_parts_ptr[8];
	pLit  = &G.pRoom->pLight[G.SaveData.Cut_no];

	pLit->Ambient[2] = 0x80;
	pLit->Ambient[1] = 0x80;
	pLit->Ambient[0] = 0x80;

	for (i = 1; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			pLit->Col[i][j] = 0;
		}
	}

	pLit->Col[0][0] = 0x78;
	pLit->Col[0][1] = 0x78;
	pLit->Col[0][2] = 0x78;
	pLit->L[0]      = 4000;
	pLit->Mode[0]   = 0;
	pLit->Pos[0][0] = (SHORT)pHead->Workm.t[0];
	pLit->Pos[0][1] = (SHORT)pHead->Workm.t[1] - 400;
	pLit->Pos[0][2] = (SHORT)pHead->Workm.t[0];  /* sic: binary stores t[0] (x) here too */
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_next_set                                         80034a04 */
/* Zapping hand-over: points the pending AOT at stage 6 room 0 start.  */
/* ------------------------------------------------------------------ */
/* DS 0x80034a04 (Door_proc_ex_next_set) */
void Door_proc_ex_next_set(void)
{
	G.pSce_aot_on->Next_pos_x  = 0x17f4;
	G.pSce_aot_on->Next_pos_y  = 0;
	G.pSce_aot_on->Next_pos_z  = 0x6db5;
	G.pSce_aot_on->Next_cdir_y = 0x400;
	G.pSce_aot_on->Next_nfloor = 0;
	G.pSce_aot_on->Next_cut    = 0;
	G.pSce_aot_on->Next_stage  = 6;
	G.pSce_aot_on->Next_room   = 0;
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_snd_load                                         80034a84 */
/* Copies the EDT/VH block out of the clear-screen data (file 0xdc,    */
/* offset 0x2a20) to Door_snd_head and opens it as VAB slot 0.         */
/* ------------------------------------------------------------------ */
/* Clear-screen file (Door_ex_data, file 0xdc at DS 0x80190000). The EDT/VH block copied to Door_snd_head starts at file
 * offset 0x2a20 (retail Door_proc_ex_snd_load 0x80034a90: lui a2,0x8019; 0x80034a94: ori a2,a2,0x2a20). The bytes
 * before it are not decoded. */
typedef struct {
	UCHAR Unk_0000[0x2a20];
	DOOR_SND_HEAD Snd_head;
} DOOR_EX_FILE;

/* DS 0x80034a84 (Door_proc_ex_snd_load) */
void Door_proc_ex_snd_load(void)
{
	ULONG *pSrc;
	ULONG *pDst;
	ULONG  i;
	SHORT  Id;
	CHAR  *pVab;

	pVab = &Vab_id[0];
	pSrc = (ULONG *)&((DOOR_EX_FILE *)Door_ex_data)->Snd_head;
	pDst = (ULONG *)&Door_snd_head;
	for (i = 0; i < sizeof(DOOR_SND_HEAD) / 4; i++) {
		*pDst++ = *pSrc++;
	}
	pVh_tbl[0] = (ULONG *)((UCHAR *)&Door_snd_head + Door_snd_head.Vh_offset);

	if (-1 < (signed char)*pVab) {
		SsVabClose((SHORT)(signed char)*pVab);
		*pVab = -1;
	}
	EDT_SLOT_PTR(2) = (ULONG *)&Door_snd_head;

	do {
		Id = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[0], 0, 0x3dc50);
		*pVab = (CHAR)Id;
	} while ((signed char)*pVab == -1);
	SsVabTransBody((UCHAR *)Door_ex_data + sizeof(DOOR_EX_FILE), (SHORT)(signed char)*pVab);
	SsVabTransCompleted(1);
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_ft4_trans                                        80034ba8 */
/* Zooms the clear logo sprite in by 2 px per frame, then holds it     */
/* opaque; always adds it to OT level 5.                               */
/* ------------------------------------------------------------------ */
/* DS 0x80034ba8 (Door_proc_ex_ft4_trans) */
void Door_proc_ex_ft4_trans(POLY_FT4 *pFt4)
{
	if (Door_ex_ft4_dx < 0) {
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x400, 0xffffff, NULL);
		pFt4->r0   = 0x80;
		pFt4->g0   = 0x80;
		pFt4->b0   = 0x80;
		pFt4->code = 0x2c;
	} else {
		Door_ex_ft4_dx -= 2;
		Door_ex_ft4_dy  = Door_ex_ft4_dx >> 2;
		Fade_set(0x200, 0, 6, 1);
		Fade_adjust(0, 0x400, 0xffffff, NULL);
		pFt4->x0 = 0x36  - Door_ex_ft4_dx;
		pFt4->y0 = 0x30  - Door_ex_ft4_dy;
		pFt4->x1 = 0x109 + Door_ex_ft4_dx;
		pFt4->y1 = 0x30  - Door_ex_ft4_dy;
		pFt4->x2 = 0x36  - Door_ex_ft4_dx;
		pFt4->y2 = 0x57  + Door_ex_ft4_dy;
		pFt4->x3 = 0x109 + Door_ex_ft4_dx;
		pFt4->y3 = 0x57  + Door_ex_ft4_dy;
	}
	AddPrim(G.pOt0 + 5, pFt4);
}
