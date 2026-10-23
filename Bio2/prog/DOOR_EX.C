/*
 * DOOR_EX.C - Extreme Battle door sequence (Resident Evil 2 DS, SLUS_00748)
 * "Ex" in these names refers to Extreme Battle, the bonus mode unlocked by completing the game.
 * Set_door (GAME.C) starts Door_main_ex instead of Door_main when SYS_EX_BATTLE is set and
 * flag 0x3f of Zapping_flg is set.
 * DS links this code as its own object between MOVIE and OBA: Door_main_ex and the twelve
 * Door_proc_ex_* functions (0x800335c0-0x80034ba8), with its .rdata (0x80010af8-0x80010b17),
 * .data (0x800a23c0-0x800a2443) and static .bss (0x800c3ac0-0x800c3af7) at the same link
 * position in each section. The beta has none of this code, so the file name is a project choice.
 * Addresses in the comments are DS addresses from Ghidra.
 */

#include "TYPES.H"
#include "ROOM.H"
#include <ASM.H>
#include <INLINE_C.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
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

/* DS 0x80190000 (Door_ex_data) */
ULONG *Door_ex_data = (ULONG *)0x80190000;

/* DS 0x80010b10 (StgNxt) */
UCHAR StgNxt[8] = "stg.nxt";

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

/* DS 0x800a242c (pDoor_proc_tbl_ex) */
static DOOR_PROC pDoor_proc_tbl_ex[6] = {
	Door_proc_ex_init,
	Door_proc_ex_move,
	Door_proc_ex_end,
	Door_proc_ex_xa_wait,
	Door_proc_ex_result,
	NULL
};


/* DS 0x800c3ac0 (Door_ex_buf0): result text being revealed. Static: DS keeps it and Door_ex_buf1 in this
 * object's own .bss (0x800c3ac0-0x800c3af7). */
static UCHAR Door_ex_buf0[0x18];

/* DS 0x800c3ad8 (Door_ex_buf1): result text being revealed */
static UCHAR Door_ex_buf1[0x20];

/* ================================================================== */
/* Extreme Battle door sequence: clear time, clear logo and result     */
/* ================================================================== */

/* DS 0x800dcbe0 (Door_ex): state of the clear sequence, see DOOR.H. Zero at start, so DS keeps it in .bss.
 * Its address is a named object of 0x4c bytes, DS reaches its fields through one base register. */
DOOR_EX_WORK Door_ex;

/* DS 0x800dcc50 (Ex_battle_vcut): zero at start; DS keeps it in .bss, not .data */
VCUT Ex_battle_vcut;

/* DS 0x800dcc78 (Door_ex_logo): "clear" logo sprite and its zoom, one 0x2c byte object */
typedef struct {
	POLY_FT4 Ft4;                   /* 000  sprite                 */
	SHORT    Dx;                    /* 028  zoom (x)               */
	SHORT    Dy;                    /* 02a  zoom (y)               */
} DOOR_EX_LOGO;
DOOR_EX_LOGO Door_ex_logo;

/* DS 0x800ead80 and 0x800ead88 (Door_ex_line0, Door_ex_line1): the two result text lines, two separate objects.
 * Declared without a size first: DS addresses them through full address registers, not gp-relative. */
extern DOOR_EX_LINE Door_ex_line0[];
extern DOOR_EX_LINE Door_ex_line1[];
DOOR_EX_LINE        Door_ex_line0[1];
DOOR_EX_LINE        Door_ex_line1[1];

/* ------------------------------------------------------------------ */
/* Door_main_ex                                                  800335c0 */
/* Extreme Battle door sequence driver: walks pDoor_proc_tbl_ex        */
/* until a NULL entry and then exits the task.                          */
/* ------------------------------------------------------------------ */
/* DS 0x800335c0 (Door_main_ex) */
void Door_main_ex(void)
{
	while (pDoor_proc_tbl_ex[Door_ex.rno] != NULL) {
		(*pDoor_proc_tbl_ex[Door_ex.rno])();
		Door_ex.rno++;
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
	DOOR_EX_WORK *pEx = &Door_ex;
	SHORT       Stage_no;
	ULONG       File_no;
	UCHAR      *Mess;

	Door_ex.flg = 0;
	Door_ex.clear_time = (G.SaveData.Game_play_time - G.SaveData.Front_play_time)
					   + VSync(-1) - G.Game_time_offset;
	pEm   = G.pEnemy[0];
	G.pEm = pEm;

	if ((UCHAR)G.SaveData.Item_flg[7] == 0xff) {
		/* zapping hand-over: no clear screen, just the exit motion */
		Door_ex.flg |= 0x40;
		*(UINT *)&pEm->Move_no = 0x70001;		/* Move_no 1, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
		pEx->cnt_max = 0x78;
		pEx->cnt     = 0x78;
		*(ULONG *)&G.ExData[60]   = (Door_ex.clear_time / 0x3c) * 0x3c;
		G.SaveData.ExData3[0x16]  = *(ULONG *)&G.ExData[60];
	} else {
		Snd_bgm_fade_ON(90, 0x10);
		Door_ex.flg = (G.SaveData.Stage_no != 5);
		if (Door_ex.flg != 0) {
			File_no = CLEAR1_PIX;
			Mess    = (UCHAR *)"clear1.tim";
			*(ULONG *)&G.ExData[56]  = (Door_ex.clear_time / 0x3c) * 0x3c;
			G.SaveData.ExData3[0x15] = *(ULONG *)&G.ExData[56];
		} else {
			File_no = CLEAR2_PIX;
			Mess    = (UCHAR *)"clear2.tim";
			*(ULONG *)&G.ExData[52]  = (Door_ex.clear_time / 0x3c) * 0x3c;
			G.SaveData.ExData3[0x14] = *(ULONG *)&G.ExData[52];
		}
		Cd_read(File_no, Bg_img_buf, 0, Mess);

		*(UINT *)&pEm->Move_no = 0x2;		/* Move_no 2, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
		Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);

		Cd_read(STG_NXT, Door_ex_data, 0, StgNxt);
		Door_proc_ex_snd_load();

		Stage_no = G.SaveData.Stage_no;
		G.SaveData.Stage_no = 7;
		Xa_play(0, 0);
		/* retail stores Ctpage (low byte) and Cclut (high byte) as one halfword */
		*(USHORT *)&G.Ctpage = 0x1f15;
		G.SaveData.Stage_no = Stage_no & 0xff;
		Mapping_tim(Door_ex_data);

		SetPolyFT4(&Door_ex_logo.Ft4);
		Door_ex_logo.Ft4.v2    = 0x28;
		Door_ex_logo.Ft4.v3    = 0x28;
		Door_ex_logo.Ft4.tpage = 0x95;
		Door_ex_logo.Ft4.clut  = 0x7fc0;
		Door_ex_logo.Dx       = 0x36;
		Door_ex_logo.Ft4.u0    = 0;
		Door_ex_logo.Ft4.v0    = 0;
		Door_ex_logo.Ft4.u1    = 0xd4;
		Door_ex_logo.Ft4.v1    = 0;
		Door_ex_logo.Ft4.u2    = 0;
		Door_ex_logo.Ft4.u3    = 0xd4;
		*(ULONG *)&Door_ex_logo.Ft4.r0 = 0x2e808080;	/* r0, g0, b0, code as one word store */
		pEx->cnt_max   = 0x3c;
		pEx->cnt       = 0x3c;
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
	DOOR_EX_WORK *pEx = &Door_ex;
	UCHAR *pCnt = &Door_ex.cnt;

	pEm   = G.pEnemy[0];
	G.pEm = pEm;

	if (Door_ex.cnt != 0) {
		do {
			if ((pEx->flg & 0x40) != 0) {
				Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
			} else {
				if (pEx->cnt == 10) {
					*(UINT *)&pEm->Move_no = 0x70001;		/* Move_no 1, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
					Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
				}
				if (pEx->cnt < 10) {
					Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x200);
				} else {
					Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);
				}
			}

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
			(*pCnt)--;
		} while (pEx->cnt != 0);
	}
	pEx->cnt = 0x3c;
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
	DOOR_EX_WORK *pEx = &Door_ex;
	pEm   = G.pEnemy[0];
	G.pEm = pEm;

	while (pEx->cnt != 0) {
		if (pEx->cnt == 0xf) {
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
		pEx->cnt--;
	}

	if ((pEx->flg & 0x40) != 0) {
		Door_proc_ex_next_set();
		G.System_flg &= ~SYS_DOOR_PROCESS;
		Flg_off(G.SaveData.Zapping_flg, 0x3f);
		pEx->rno = 4;                          /* -> table[5] == NULL: exit */
		G.SaveData.Item_flg[7] ^= 0xff;
		Bg_set_mode(2, 0);
	} else {
		Door_ex_line0[0].X   = 0x36;
		Door_ex_line0[0].Y   = 0x5f;
		Door_ex_line0[0].Cnt = 0x15;
		Door_ex_line1[0].X   = 0x26;
		Door_ex_line1[0].Y   = 0x7b;
		Door_ex_line0[0].Idx = 0;
		Door_ex_line1[0].Idx = 0;
		Door_ex_line1[0].Cnt = 0x1f;
		Bg_set_mode(1, 0);
	}
	Fade_off(3);
}

/* ------------------------------------------------------------------ */
/* Door_proc_ex_xa_wait                                          80033d08 */
/* Phase 3: shows the zooming clear logo until the XA track ends.      */
/* ------------------------------------------------------------------ */
/* DS 0x80033d08 (Door_proc_ex_xa_wait) */
void Door_proc_ex_xa_wait(void)
{
	while ((G.Status_flg & STAT_XA_VOICE) != 0) {
		Door_proc_ex_ft4_trans(&Door_ex_logo.Ft4);
		Task_sleep(1);
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
	ULONG  Digit;
	SHORT  Wait;
	SHORT  i;
	Wait = 0x10;
	/* Cnt is USHORT: Cnt - 1 is compared as an int (no 16-bit sign extension) */
	for (i = 0; i < Door_ex_line0[0].Cnt - 1; i++) {
		Door_ex_buf0[i] = 0;
	}
	for (i = 0; i < Door_ex_line1[0].Cnt - 1; i++) {
		Door_ex_buf1[i] = 0;
	}
	Door_ex_buf1[0] = 0xfe;
	Door_ex_buf0[0] = 0xfe;

	/* clear time -> "HH:MM:SS" digit glyphs (glyph index = digit + 0xc) */
	Time = Door_ex.clear_time;
	if (0x14996c4 < Time) {                       /* cap at 99:59:59 */
		Time = 0x14996c4;
	}
	Door_ex_mess0[0x0c] = Door_ex_mess0[0x0e] = Door_ex_mess0[0x0f] = Door_ex_mess0[0x11] = Door_ex_mess0[0x12] = 0xc;
	Digit = Time / 0x20f580;                               /* 10 hours */
	Door_ex_mess0[0x0b] = Digit + 0xc;
	Door_ex_mess0[0x0c] = Time / 0x34bc0 - Digit * 10 + 0xc;          /* 1 hour  */
	Digit = (Time % 0x34bc0) / 36000;                      /* 10 min   */
	Door_ex_mess0[0x0e] = Digit + 0xc;
	Door_ex_mess0[0x0f] = (Time % 0x34bc0) / 0xe10 - Digit * 10 + 0xc; /* 1 min   */
	Digit = (Time % 0xe10) / 600;                          /* 10 sec   */
	Door_ex_mess0[0x11] = Digit + 0xc;
	Door_ex_mess0[0x12] = (Time % 0xe10) / 0x3c - Digit * 10 + 0xc;   /* 1 sec   */

	for (;;) {
		Door_proc_ex_ft4_trans(&Door_ex_logo.Ft4);

		if (Door_ex_line0[0].Cnt != 0) {
			if (Wait == 0) {
				Door_ex_buf0[Door_ex_line0[0].Idx] = Door_ex_mess0[Door_ex_line0[0].Idx];
				if (Door_ex_buf0[Door_ex_line0[0].Idx] == 0xfe) {
					Snd_se_stad(0x10000, NULL);
					Wait = 6;
				} else {
					Door_ex_buf0[Door_ex_line0[0].Idx + 1] = 0xfe;
					if (Door_ex_buf0[Door_ex_line0[0].Idx] == 0) {
						Snd_se_stad(0x20000, NULL);
						Wait = 5;
					} else {
						Snd_se_stad(0, NULL);
						Wait = 4;
					}
				}
				Door_ex_line0[0].Idx++;
				Door_ex_line0[0].Cnt--;
			} else {
				Wait--;
			}
		} else {
			if ((Door_ex.flg & 0x80) == 0) {
				Bg_set_mode(0, 0);
				Fade_set(0x203, -0x100, 7, 3);
				Door_ex.flg |= 0x80;
			}
			if (Door_ex_line1[0].Cnt != 0) {
				if (Wait != 0) {
					Wait--;
				} else {
					if ((Door_ex.flg & 0x1f) == 0) {
						Door_ex_buf1[Door_ex_line1[0].Idx] = Door_ex_mess1a[Door_ex_line1[0].Idx];
					} else {
						Door_ex_buf1[Door_ex_line1[0].Idx] = Door_ex_mess1b[Door_ex_line1[0].Idx];
					}
					if (Door_ex_buf1[Door_ex_line1[0].Idx] == 0xfe) {
						Snd_se_stad(0x10000, NULL);
						Wait = 6;
						Door_ex_line1[0].Cnt = 1;
					} else {
						Door_ex_buf1[Door_ex_line1[0].Idx + 1] = 0xfe;
						if (Door_ex_buf1[Door_ex_line1[0].Idx] == 0) {
							Snd_se_stad(0x20000, NULL);
							Wait = 5;
						} else {
							Snd_se_stad(0, NULL);
							Wait = 4;
						}
					}
					Door_ex_line1[0].Idx++;
					Door_ex_line1[0].Cnt--;
				}
			}
		}

		Mess_print(Door_ex_line0[0].X, Door_ex_line0[0].Y, Door_ex_buf0, 0x4000);
		Mess_print(Door_ex_line1[0].X, Door_ex_line1[0].Y, Door_ex_buf1, 0x4000);

		if (Door_ex_line0[0].Cnt == 0 && Door_ex_line1[0].Cnt == 0) {
			Wait++;
			if ((Wait & 0x10) == 0) {
				Mess_print(0x3e, 0xb9, Door_ex_mess_push, 0x4000);
			}
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


/* ------------------------------------------------------------------ */
/* Door_proc_ex_rcut                                             8003432c */
/* Copies the current room cut into Door_ex.rcut and computes the    */
/* orbit parameters for the clear cinematic.                            */
/* ------------------------------------------------------------------ */
/* DS 0x8003432c (Door_proc_ex_rcut) */
void Door_proc_ex_rcut(void)
{
	RCUT    *pRcut;
	PARTS_W *pParts;
	LONG     View_py;
	LONG     View_ry;
	DOOR_EX_WORK *pEx;
	RCUT    *pCut;

	pRcut  = G.pRoom->pRcut + G.SaveData.Cut_no;
	pParts = G.pEnemy[0]->pSin_parts_ptr;

	Door_ex.rcut = *pRcut;
	pEx  = &Door_ex;
	pCut = &Door_ex.rcut;

	View_py = Door_ex.rcut.View_p[1];
	View_ry = Door_ex.rcut.View_r[1];

	Door_ex.dir     = (USHORT)ratan2(Door_ex.rcut.View_p[2] - Door_ex.rcut.View_r[2], Door_ex.rcut.View_p[0] - Door_ex.rcut.View_r[0]) & 0xfff;
	Door_ex.dir_add = (Door_ex.dir + G.pEnemy[0]->Cdir_y - 300) & 0xfff;
	if ((Door_ex.flg & 0x40) != 0) {
		Door_ex.dir_add += 0x1000;
	}

	Door_ex.rcut.View_p[1] = 0;
	Door_ex.rcut.View_r[1] = 0;
	Door_ex.cx   = Door_ex.rcut.View_r[0];
	Door_ex.cz   = Door_ex.rcut.View_r[2];
	Door_ex.dist = SquareRoot0((Door_ex.rcut.View_p[0] - Door_ex.rcut.View_r[0]) * (Door_ex.rcut.View_p[0] - Door_ex.rcut.View_r[0])
	                             + (Door_ex.rcut.View_p[2] - Door_ex.rcut.View_r[2]) * (Door_ex.rcut.View_p[2] - Door_ex.rcut.View_r[2]));
	Door_ex.rcut.View_p[1] = View_py;
	Door_ex.rcut.View_r[1] = View_ry;

	if ((Door_ex.flg & 0x40) != 0) {
		Door_ex.dist_step = (Door_ex.dist - 2500) >> 1;
	} else {
		DIV_S(Door_ex.dist_step, Door_ex.dist - 2500, (LONG)Door_ex.cnt_max);
	}

	pEx->py     = pCut->View_p[1];
	pEx->ry     = pCut->View_r[1];
	pEx->py_add = pParts->Workm.t[1] + 400;
	pEx->py_add -= pCut->View_p[1];
	pEx->ry_add = pParts->Workm.t[1] - 200;
	pEx->ry_add -= pCut->View_r[1];
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
	MATRIX  *pWork;
	RCUT    *pRcut;
	DOOR_EX_WORK *pEx;
	LONG     Ang;
	LONG     Rate;
	ULONG    Dir;
	LONG     Cs;

	DIV_S(Ang, (LONG)(Door_ex.cnt_max - Door_ex.cnt) * 0x800, (LONG)Door_ex.cnt_max);
	Ang -= 0x400;
	pParts  = G.pEnemy[0]->pSin_parts_ptr;
	pWork   = &pParts->Workm;
	Rate   = rsin(Ang) + 0x1000;               /* 0..0x2000 ease curve */
	pRcut   = &Door_ex.rcut;
	pEx     = &Door_ex;

	Dir = (Door_ex.dir - ((LONG)(Door_ex.dir_add * Rate) >> 13)) & 0xfff;

	if ((Door_ex.flg & 0x40) != 0) {
		if (Door_ex.cnt == 0x50) {
			Door_ex.dist_step /= 2;
		}
		Cs = rcos(Ang * 3 + 0xc00);
		Door_ex.dist = (Door_ex.dist_step * (Cs + 0x1000) >> 12) + 2500;
	} else {
		Door_ex.dist -= Door_ex.dist_step;
	}

	pRcut->View_p[0] = pEx->cx + (pEx->dist * rcos(Dir) >> 12);
	pRcut->View_p[2] = pEx->cz + (pEx->dist * rsin(Dir) >> 12);
	pRcut->View_p[1] = pEx->py + (pEx->py_add * Rate >> 13);

	pEx->cx += (pWork->t[0] - pEx->cx) / 30;
	pEx->cz += (pWork->t[2] - pEx->cz) / 30;

	pRcut->View_r[0] += (pWork->t[0] - pRcut->View_r[0]) / 30;
	pRcut->View_r[1]  = pEx->ry + (pEx->ry_add * Rate >> 13);
	pRcut->View_r[2] += (pWork->t[2] - pRcut->View_r[2]) / 30;

	Set_view(pRcut->View_p, pRcut->View_r);

	if (0xd0 < (pRcut->ViewR >> 7)) {
		pRcut->ViewR -= 0x80;
	}
	SetGeomScreen(pRcut->ViewR >> 7);

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
	VCUT *pVcut = &Ex_battle_vcut;
	SHORT i;

	for (i = 0; i < 4; i++) {
		pVcut->Xz[i][0] = 0;
		pVcut->Xz[i][1] = 0;
	}
	G.Cccut = pVcut;
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
	MATRIX         *pHead;
	SHORT           i;
	SHORT           j;

	pHead = &G.pEnemy[0]->pSin_parts_ptr[8].Workm;
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
	pLit->Pos[0][0] = (SHORT)pHead->t[0];
	pLit->Pos[0][1] = (SHORT)pHead->t[1] - 400;
	pLit->Pos[0][2] = (SHORT)pHead->t[0];  /* sic: binary stores t[0] (x) here too */
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
	SHORT  Id;
	CHAR  *pVab;

	pVab = &Vab_id[0];
	*Door_snd_head = ((DOOR_EX_FILE *)Door_ex_data)->Snd_head;
	pVh_tbl[0] = (ULONG *)((UCHAR *)Door_snd_head + Door_snd_head->Vh_offset);

	if (-1 < (signed char)*pVab) {
		SsVabClose((SHORT)(signed char)*pVab);
		*pVab = -1;
	}
	EDT_SLOT_PTR(2) = (ULONG *)Door_snd_head;

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
	/* Retail reserves 8 bytes of frame at sp+0x10 that no path reads or writes. */
	volatile UCHAR Frame_pad[8];
	if (Door_ex_logo.Dx >= 0) {
		Door_ex_logo.Dx -= 2;
		Door_ex_logo.Dy = Door_ex_logo.Dx >> 2;
		Fade_set(0x200, 0, 6, 1);
		Fade_adjust(0, 0x400, 0xffffff, NULL);
		pFt4->x0 = 0x36  - Door_ex_logo.Dx;
		pFt4->y0 = 0x30  - Door_ex_logo.Dy;
		pFt4->x1 = 0x109 + Door_ex_logo.Dx;
		pFt4->y1 = 0x30  - Door_ex_logo.Dy;
		pFt4->x2 = 0x36  - Door_ex_logo.Dx;
		pFt4->y2 = 0x57  + Door_ex_logo.Dy;
		pFt4->x3 = 0x109 + Door_ex_logo.Dx;
		pFt4->y3 = 0x57  + Door_ex_logo.Dy;
	} else {
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x400, 0xffffff, NULL);
		*(ULONG *)&pFt4->r0 = 0x2c808080;
	}
	AddPrim(G.pOt0 + 5, pFt4);
}
