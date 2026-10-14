/*
 * DIEDEMO.C - Death demo overlay (camera / state machine)
 * D:\Bio2\prog\DIEDEMO.C
 * Overlay DIEDEMO:: 0x80190000 - 0x801938c7 (DS retail)
 *
 * Sprite work (Die_spr_*, Die_blood_*, Die_spr_load, Die_snd_trans) lives in DIESPR.C.
 * Die_demo and the Die_move_* state handlers are defined in Ghidra (DIEDEMO::
 * segment). Their DS addresses are read from Die_move_tbl (DS 0x801936d4) and from
 * MONITOR.C and checked against the overlay bytes.
 */

#include "TYPES.H"
#include "MONITOR.H"
#include "CD.H"
#include "ROOM.H"
#include <INLINE_C.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <STRINGS.H>
#include "DIEDEMO.H"
#include "DIESPR.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "JOINT.H"
#include "MAIN.H"
#include "SCE1.H"
#include "SND.H"
#include "SUB.H"
#include "flag.h"

/* ------------------------------------------------------------------ */
/* Die_move_tbl: state handlers, DS 0x801936d4 (7 entries, read from   */
/* DIEDEMO.BIN); Diedemo.Rno indexes it                                */
/* ------------------------------------------------------------------ */
static void (*Die_move_tbl[])(void) = {
	Die_move_fade_out,      /* 0x80190118 state 0 */
	Die_move_delay,         /* 0x80190344 state 1 */
	Die_move_wait,          /* 0x80190388 state 2 */
	Die_move_pause,         /* 0x80190eec state 3 */
	Die_move_main,          /* 0x80190fb0 state 4 */
	Die_move_out,           /* 0x801911a8 state 5 */
	Die_move_end,           /* 0x801912cc state 6 */
};

/* Critical attack ID table (DS address not verified) */
UCHAR Critical_id[16];

/* ------------------------------------------------------------------ */
/* Static data                                                         */
/* ------------------------------------------------------------------ */

/* Cut info table: 14 entries, DS 0x80193520 */
CUTINFO Cutinfo[14];

/* Enemy-type to Cutinfo lookup table, DS 0x801936c4 (15 bytes; index 14 searched first) */
static char Cutinfo_id[15];

/* Wait count table, DS 0x80193500 (USHORT, read from DIEDEMO.BIN). Index is the Cutinfo_id
 * index found by Die_move_fade_out (0..14), or 15 for the enemy '4' case with Type bit 0 clear.
 * Size 16 is the highest index the decompile can produce (15) plus one. The table's end is
 * not proven beyond that index; the Cutinfo label is not used as a bound. */
static USHORT Die_wait_tbl[16];

/* Death demo camera / state work, DS 0x80193870 (60 bytes; the beta debug info calls it
 * Diedemo, type DIEDEMO). One object: the retail Spiral_move addresses it through a single
 * base pointer (s1 = &Diedemo, s2 = &Diedemo.Rcut). Old names of the separate statics:
 *   Addpos    Cam_step          0x80193870  camera height step
 *   Rno       Diedemo_routine   0x80193872  state index, indexes Die_move_tbl
 *   Ctr       Diedemo_s_trans   0x80193874  frame counter (Die_move_pause counts it up to
 *                                           Cutinfo.Wait_ctr, Die_move_set_cut clears it);
 *                                           the sound transfer flag is S_trans_flg, 0x801938a5
 *   Rcut      View_p[] = Cam_lx/ly/lz 0x8019387c..84 (eye), View_r[] = Cam_tx/ty/tz
 *             0x80193888..90 (look-at); Set_view(View_p, View_r)
 *   Radius    Cam_radius        0x80193898
 *   Theta     Cam_theta         0x8019389c
 *   Prad      Cam_theta_add     0x8019389e
 *   P_radius  Cam_radius_add    0x801938a0
 *   Daddpos   Cam_step_add      0x801938a6
 * Die_phase (0x801938a4) is the phase counter below, kept as its own symbol because
 * DIEDEMO.H declares it extern for DIESPR.C. +0x38 (0x801938a8) is the SHORT member Wait_ctr, a
 * signed wait counter (Die_move_delay); the Cutinfo pointer is pCam_cutinfo, 0x801938ac. */
static DIEDEMO Diedemo;

/* Active cutinfo pointer, DS 0x801938ac */
static CUTINFO *pCam_cutinfo;

/* Phase counter, DS 0x801938a4 (extern in DIEDEMO.H). Die_move_wait steps it 0, 1, 2 and wraps;
 * state handlers test bit 0 (& 1) to alternate every frame. */
UCHAR Die_phase;

/* ------------------------------------------------------------------ */
/* Die_demo  DS 0x8019009c..0x801900d8 (16 instructions, frame 24 bytes) */
/* Entry task, started by MONITOR.C. Sets Diedemo.Rno = 0 and           */
/* Diedemo.Ctr = 0, then loops forever: Die_demo_move(); Task_sleep(1). */
/* The loop closes with j 0x801900b4 (delay slot 0x801900c8); the       */
/* epilogue at 0x801900cc..0x801900d8 (ra reload, jr ra) is unreachable */
/* because the loop has no exit, so the function never returns.         */
/* ------------------------------------------------------------------ */
void Die_demo(void)
{
	Diedemo.Rno = 0;
	Diedemo.Ctr = 0;
	for (;;) {
		Die_demo_move();
		Task_sleep(1);
	}
}

/* Death vcut, DS 0x801938b0 */
VCUT Dievcut;

/* ------------------------------------------------------------------ */
/* Die_demo_move  DS 0x801900dc                                        */
/* Per-frame dispatcher: calls current state handler                   */
/* ------------------------------------------------------------------ */
void Die_demo_move(void)
{
	(*Die_move_tbl[Diedemo.Rno])();
}

/* ------------------------------------------------------------------ */
/* Die_move_fade_out  DS 0x80190118  (state 0, 139 instructions, to 0x80190344) */
/* Cut setup, Diedemo.Rno == 0. Resets the camera work, takes Wait_ctr from     */
/* Die_wait_tbl by enemy type (only when SYS_DIE_BLOOD), sets STAT_PL_BUSY and   */
/* STAT_CAM_LOCK, then starts the timer-expired XA and pad vibration or clears   */
/* STAT_TIMER. Rno advances by 1 when 0x800 and SYS_DIE_BLOOD are set, else by 2. */
/* The name is from the state table order; the body is decoded from the bytes.   */
/* ------------------------------------------------------------------ */
void Die_move_fade_out(void)
{
	UCHAR       Enem_id;
	int         Id_ctr;
	int         Idx;
	ENEMY_WORK *pEnem;

	Diedemo.Free    = 0x400;
	Diedemo.Addpos  = 0;
	Diedemo.Ctr     = 0;
	Die_phase    = 0;
	Diedemo.Daddpos = 0;
	Diedemo.S_trans_flg = 0;

	if ((G.System_flg & SYS_DIE_BLOOD) != 0) {
		pEnem   = (ENEMY_WORK *)G.Player.pEnemy_ptr;
		Enem_id = pEnem->Id;
		if ((Enem_id == 52) && ((pEnem->Type & 1) == 0)) {
			Id_ctr = 15;
		} else if (Enem_id < 32) {
			Id_ctr = 1;
		} else {
			if (Enem_id == 57) {
				Enem_id = 46;
			}
			/* Search Cutinfo_id from index 14 downwards; falls back to 0 */
			Id_ctr = 14;
			if (Cutinfo_id[14] != Enem_id) {
				for (Idx = 14; Idx > 0; ) {
					Idx--;
					if (Cutinfo_id[Idx] == Enem_id) {
						break;
					}
				}
				Id_ctr = Idx;
			}
		}
		Diedemo.Wait_ctr = Die_wait_tbl[Id_ctr];
	}

	G.Stop_flg |= 0x02000000;
	G.Next_cut_no = (UCHAR)G.SaveData.Cut_no;

	if ((G.System_flg & SYS_TIMER_EXPIRED) == 0) {
		G.Status_flg = (G.Status_flg & ~STAT_TIMER) | STAT_PL_BUSY | STAT_CAM_LOCK;
	} else {
		G.Status_flg |= STAT_PL_BUSY | STAT_CAM_LOCK;
		Xa_play(1, 0);
		G.Status_flg |= STAT_XA_VOICE;
		Pad_vib_set0(5, 5);
		Pad_vib_set0(5, 15);
		Pad_vib_set0(10, 25);
		Pad_vib_fade_set(40, 200, 250, 5);
	}

	if (((G.System_flg & 0x800) != 0) && ((G.System_flg & SYS_DIE_BLOOD) != 0)) {
		Diedemo.Rno++;
	} else {
		Diedemo.Rno += 2;
	}
}

/* ------------------------------------------------------------------ */
/* Die_move_delay  DS 0x80190344  (state 1)                            */
/* Leaf, no frame (17 instructions, 0x80190344..0x80190384, ends at jr  */
/* ra). Counts Diedemo.Wait_ctr down by one; when the 16-bit value is   */
/* negative afterwards, advances Diedemo.Rno to the next state. The     */
/* name is not in the beta list, so it is not verified.                 */
/* ------------------------------------------------------------------ */
void Die_move_delay(void)
{
	Diedemo.Wait_ctr--;
	if (Diedemo.Wait_ctr < 0) {
		Diedemo.Rno++;
	}
}

/* ------------------------------------------------------------------ */
/* Die_move_wait  DS 0x80190388  (state 2)                             */
/* Decompile FUN_DIEDEMO__80190388. Fade ramp while Free < 0x7fff; then  */
/* Die_spr_load, Bg_set_mode, Esp_die or Esp_kill_all, the Ob_model flag */
/* loop, the Ob_model[1] flag clear (guarded by the '#' enemy and        */
/* &GsIDMATRIX), Die_move_set_cut or Die_move_set_cut_usual, then Rno    */
/* advances (+1 with SYS_DIE_BLOOD, else +2) and Stop_flg is updated.    */
/* ------------------------------------------------------------------ */
void Die_move_wait(void)
{
	OBJ_MODEL_WORK *pOb;
	UCHAR           Omodel_num;
	UINT            Omodel_cnt;

	if (Diedemo.Free < 0x7fff) {
		/* Ramp: Ctr (speed) += Addpos, Addpos += 24 or 2, Free (position) += Ctr */
		Diedemo.Ctr = (SHORT)(Diedemo.Ctr + Diedemo.Addpos);
		Diedemo.Addpos = (SHORT)(Diedemo.Addpos + ((G.System_flg & SYS_DIE_BLOOD) ? 24 : 2));

		/* Three-frame phase counter in Die_phase (DIEDEMO.H: 0x801938a4) */
		Die_phase++;
		if (Die_phase < 3) {
			G.Stop_flg |= 0xff000000;
		} else {
			Die_phase = 0;
			G.Stop_flg ^= 0xfc000000;
			Snd_bgm_ctr(0x02000000);
			Snd_bgm_ctr(0x12000000);
			Snd_bgm_ctr(0x22000000);
		}

		Diedemo.Free = (SHORT)(Diedemo.Free + Diedemo.Ctr);
		if (Diedemo.Free >= 0x7fff) {
			Diedemo.Free = 0x7fff;
			if (G.System_flg & SYS_TIMER_EXPIRED) {
				if (G.Status_flg & STAT_XA_VOICE) {
					Pad_vib_set1(1, 250, 0);
					Diedemo.Ctr = 1;
					Diedemo.Addpos = 0;
					Diedemo.Free = 0x7ffe;
				} else {
					Pad_vib_fade_set(60, 250, 120, 0);
				}
			}
		}
		Fade_set(0x100, 0, 7, 3);
		Fade_adjust(0, Diedemo.Free, 0x00ffffff, NULL);
		return;
	}

	/* Fade finished: load the death sprites, then set the cut */
	Die_phase = 0;
	G.Dummy8_3[0] = 1;
	Die_spr_load();
	G.Dummy8_3[0] = 0;

	/* Camera sprite on, as the decompile writes it (flag.h SYS_PSP_TRANS = 0x00100000) */
	G.System_flg |= SYS_PSP_TRANS;
	Bg_set_mode(2, 0);
	if ((G.System_flg & SYS_TIMER_EXPIRED) == 0) {
		Esp_die();
	} else {
		Esp_kill_all();
	}

	G.Player.Be_flg |= 0x00000008;
	Omodel_num = G.pRoom->nOmodel;
	Omodel_cnt = 0;
	if (Omodel_num != 0) {
		pOb = G.Ob_model;
		do {
			Omodel_cnt++;
			pOb->Be_flg |= 0x00080000;
			pOb++;
		} while (Omodel_cnt < Omodel_num);
	}

	/* Be_flg 0x00080000 on Ob_model[1] is cleared when the enemy Id is '#' and
	 * Ob_model[1].pSuper is not &GsIDMATRIX (DS 0x800adb44) */
	if (((char)((ENEMY_WORK *)G.Player.pEnemy_ptr)->Id == 35) &&
	    ((MATRIX *)G.Ob_model[1].pSuper != &GsIDMATRIX)) {
		G.Ob_model[1].Be_flg &= 0xfff7ffff;
	}

	if ((G.System_flg & SYS_DIE_BLOOD) == 0) {
		Die_move_set_cut_usual();
	} else {
		Die_move_set_cut();
	}

	Task_sleep(1);
	Diedemo.Ctr = 0;
	Diedemo.Rno = (USHORT)(Diedemo.Rno + ((G.System_flg & SYS_DIE_BLOOD) ? 1 : 2));

	G.Stop_flg = (G.Stop_flg & 0x03ffffff) | ((Die_phase & 1) ? 0x80000000 : 0x40000000);
	Fade_set(0x100, -0xbbc, 7, 3);
}

/* ------------------------------------------------------------------ */
/* Die_move_set_cut  DS 0x8019070c                                     */
/* Set up camera cut info by enemy type                                */
/* ------------------------------------------------------------------ */
void Die_move_set_cut(void)
{
	short        Theta_step;
	UCHAR       Cdir_flg;
	int          Idx;
	int          Id_ctr;
	char         Enem_id;
	MATRIX      *pMx;
	ENEMY_WORK  *pLook_enem;
	short        Obj_ptr;
	CUTINFO     *pCut;

	Diedemo.Ctr  = 0;
	Diedemo.Prad = 0;

	Enem_id = (char)((ENEMY_WORK *)G.Player.pEnemy_ptr)->Id;
	if ((Enem_id == 52) && ((((ENEMY_WORK *)G.Player.pEnemy_ptr)->Type & 1) == 0)) {
		Enem_id = 51;
	}
	if (Enem_id == 57) {
		Enem_id = 46;
	}

	/* Search Cutinfo_id from index 14 downwards; falls back to 0 */
	Id_ctr = 14;
	if (Cutinfo_id[14] != Enem_id) {
		for (Idx = 13; Idx >= 0; Idx--) {
			if (Cutinfo_id[Idx] == Enem_id) {
				break;
			}
		}
		Id_ctr = (Idx < 0) ? 0 : Idx;
	}

	pCut             = &Cutinfo[Id_ctr];
	pCam_cutinfo     = pCut;
	Diedemo.Radius   = pCut->Radius;
	Diedemo.P_radius = pCut->P_radius;
	Diedemo.Theta    = G.Player.Cdir_y & 0xfff;

	Cdir_flg = Cdir_ck((ENEMY_WORK *)&G.Player, (ENEMY_WORK *)G.Player.pEnemy_ptr);
	if (Cdir_flg == 0) {
		Theta_step   = pCut->M_theta;
		Diedemo.Prad = pCut->Mrad;
	} else {
		Theta_step   = pCut->P_theta;
		Diedemo.Prad = pCut->Prad;
	}
	Diedemo.Theta += Theta_step;

	if (pCut->R_obj == 0) {
		if ((Die_phase & 1) == 0) {
			pMx = &G.Player.pSin_parts_ptr[pCut->R_ptr].Workm;
			goto set_campos;
		}
		Obj_ptr = pCut->R_ptr;
		pLook_enem  = G.pEnemy[1];
	} else {
		/* R_obj other than 0/1: retail leaves the look-at matrix unset */
		if (pCut->R_obj != 1) goto set_campos;
		Obj_ptr = pCut->R_ptr;
		pLook_enem  = (ENEMY_WORK *)G.Player.pEnemy_ptr;
	}
	pMx = &pLook_enem->pSin_parts_ptr[Obj_ptr].Workm;

set_campos:
	Diedemo.Rcut.View_p[1] = pMx->t[1] + pCut->P_viewp;
	Diedemo.Addpos         = pCut->Addpos;
	Diedemo.Daddpos        = 0;
	Diedemo.Rcut.View_r[0] = pMx->t[0];
	Diedemo.Rcut.View_r[2] = pMx->t[2];
	Diedemo.Rcut.View_r[1] = pMx->t[1] - 300;

	Die_vcut_set();
	Die_enemy_set();
	Die_light_set();
	Spiral_move();
}

/* ------------------------------------------------------------------ */
/* Die_move_set_cut_usual  DS 0x801909c4                               */
/* Set up default camera cut (no specific enemy type)                  */
/* ------------------------------------------------------------------ */
void Die_move_set_cut_usual(void)
{
	Diedemo.Radius   = 3000;
	Diedemo.Addpos   = 8;
	Diedemo.Daddpos  = 1;
	Diedemo.Prad     = 0x10;
	Diedemo.Ctr      = 0;
	Diedemo.P_radius = 0;
	pCam_cutinfo     = &Cutinfo[0];
	Diedemo.Theta    = G.Player.Cdir_y;
	Diedemo.Rcut.View_p[1] = ((G.Player.pSin_parts_ptr)->Workm).t[1] - 1000;
	Diedemo.Rcut.View_r[0] = ((G.Player.pSin_parts_ptr)->Workm).t[0];
	Diedemo.Rcut.View_r[2] = ((G.Player.pSin_parts_ptr)->Workm).t[2];
	Diedemo.Rcut.View_r[1] = ((G.Player.pSin_parts_ptr)->Workm).t[1] - 300;

	Die_vcut_set();
	Die_enemy_set();
	Die_light_set();
	Spiral_move();
}

/* ------------------------------------------------------------------ */
/* Die_enemy_set  DS 0x80190aa4                                        */
/* Mark enemies near death-point as visible; set Be_flg                */
/* ------------------------------------------------------------------ */
void Die_enemy_set(void)
{
	ULONG      Be_flg_new;
	int         Dx;
	int         Dz;
	long        Dist;
	UINT        Be_flg_cur;
	ENEMY_WORK *pEm;
	ENEMY_WORK **ppEm;

	ppEm = G.pEnemy + 1;
	if ((G.System_flg & SYS_TIMER_EXPIRED) == 0) {
		if (G.ppEnemy_end != G.pEnemy + 1) {
			do {
				pEm = *ppEm;
				if ((pEm->Be_flg & 1) != 0) {
					Be_flg_new = pEm->Be_flg | 0x80000;
					if ((pEm->Type & 0xc400) == 0) {
						if ((Die_phase & 1) == 0) {
							Dx = G.Player.Pos_x - pEm->Pos_x;
							Dz = G.Player.Pos_z - pEm->Pos_z;
							Dist = SquareRoot0(Dx * Dx + Dz * Dz);
							if (Dist < 0x1c21) {
								Be_flg_new = pEm->Be_flg | 8;
							} else {
								if (pEm->Id == 41) goto next_enemy;
								Be_flg_new = pEm->Be_flg | 0x80000;
							}
						} else {
							Dx = G.pEnemy[1]->Pos_x - pEm->Pos_x;
							Dz = G.pEnemy[1]->Pos_z - pEm->Pos_z;
							Dist = SquareRoot0(Dx * Dx + Dz * Dz);
							if (Dist < 0x1c21) {
								Be_flg_new = pEm->Be_flg | 8;
							} else {
								if (pEm->Id == 41) goto next_enemy;
								Be_flg_new = pEm->Be_flg | 0x80000;
							}
						}
					}
					pEm->Be_flg = Be_flg_new;
				}
next_enemy:
				ppEm++;
			} while (ppEm != G.ppEnemy_end);
		}
		if ((Die_phase & 1) == 0) {
			*G.Player.pEnemy_ptr = *G.Player.pEnemy_ptr & 0xfff7ffff;
			*G.Player.pEnemy_ptr = *G.Player.pEnemy_ptr | 8;
		} else {
			*G.pEnemy[1]->pEnemy_ptr = *G.pEnemy[1]->pEnemy_ptr & 0xfff7ffff;
			*G.pEnemy[1]->pEnemy_ptr = *G.pEnemy[1]->pEnemy_ptr | 8;
			G.Player.Be_flg |= 0x80000;
			G.pEnemy[1]->Be_flg &= 0xfff7ffff;
		}
	} else {
		if (G.ppEnemy_end != G.pEnemy + 1) {
			do {
				Be_flg_cur = (*ppEm)->Be_flg;
				if ((Be_flg_cur & 1) != 0) {
					(*ppEm)->Be_flg = Be_flg_cur | 0x80000;
				}
				ppEm++;
			} while (ppEm != G.ppEnemy_end);
		}
		G.Player.Be_flg |= 0x80000;
	}
}

/* ------------------------------------------------------------------ */
/* Die_light_set  DS 0x80190d7c                                        */
/* Set up lighting for the current cut                                 */
/* ------------------------------------------------------------------ */
void Die_light_set(void)
{
	PARTS_W        *pPlayerParts;
	short           Cut_idx;
	LIGHT_CUT_DATA *pLc;
	long            Z_pos;

	Cut_idx  = G.SaveData.Cut_no;
	pPlayerParts = G.Player.pSin_parts_ptr;
	pLc    = (G.pRoom)->pLight;

	pLc[Cut_idx].Pos[2][0] = (short)Diedemo.Rcut.View_r[0] + 500;
	pLc[Cut_idx].Pos[2][1] = (short)Diedemo.Rcut.View_r[1] + 3000;
	Z_pos = Diedemo.Rcut.View_r[2];
	pLc[Cut_idx].Col[2][0] = 0x84;
	pLc[Cut_idx].Col[2][1] = 0x84;
	pLc[Cut_idx].Col[2][2] = 0x84;
	pLc[Cut_idx].L[2]      = 8000;
	pLc[Cut_idx].Pos[2][2] = (short)Z_pos + 500;

	pLc[Cut_idx].Pos[0][0] = (short)(pPlayerParts->Workm).t[0];
	pLc[Cut_idx].Pos[0][1] = (short)(pPlayerParts->Workm).t[1] - 300;
	Z_pos = (pPlayerParts->Workm).t[2];
	pLc[Cut_idx].Col[0][0] = 120;
	pLc[Cut_idx].Col[0][1] = 120;
	pLc[Cut_idx].Col[0][2] = 120;
	pLc[Cut_idx].L[0]      = 4000;
	pLc[Cut_idx].Mode[0]   = 0;
	pLc[Cut_idx].Col[1][0] = 0;
	pLc[Cut_idx].Col[1][1] = 0;
	pLc[Cut_idx].Col[1][2] = 0;
	pLc[Cut_idx].Pos[0][2] = (short)Z_pos;
}

/* ------------------------------------------------------------------ */
/* Die_vcut_set  DS 0x80190e44                                         */
/* Initialise G.Cccut vcut data from system flags                      */
/* Dievcut.Xz[i][0] and Dievcut.Xz[i][1] are the eight words, i = 0-3.  */
/* ------------------------------------------------------------------ */
void Die_vcut_set(void)
{
	USHORT *pWord;
	int     i;

	if ((G.System_flg & 0x800) == 0) {
		pWord = (USHORT *)Dievcut.Xz;
		for (i = 0; i < 4; i++) {
			pWord[i * 2]     = 0;
			pWord[i * 2 + 1] = 0;
		}
	} else {
		pWord = (USHORT *)Dievcut.Xz;
		pWord[0] = 0xffff;
		pWord[1] = 1;
		pWord[2] = 0xffff;
		pWord[3] = 0xffff;
		pWord[4] = 1;
		pWord[5] = 1;
		pWord[6] = 1;
		pWord[7] = 0xffff;
	}
	G.Cccut = (VCUT *)&Dievcut;
}

/* ------------------------------------------------------------------ */
/* DS 0x80190eec (Die_move_pause)                                      */
/* State 3, Diedemo.Rno == 3. Starts the death sound transfer once when  */
/* the CD read is idle, runs the spiral camera, and counts Diedemo.Ctr  */
/* up. When Ctr passes pCam_cutinfo->Wait_ctr the camera work is reset  */
/* (Addpos, Prad, P_radius, Ctr) and the state advances to Rno 4.       */
/* Die_snd_trans(0xc68): same size as the Die_move_main call.           */
/* ------------------------------------------------------------------ */
void Die_move_pause(void)
{
	if (CD.Cd_rtn_no == 0 && Diedemo.S_trans_flg == 0) {
		Die_snd_trans(0xc68);
		Diedemo.S_trans_flg = 1;
	}
	Spiral_move();
	Diedemo.Ctr++;
	if ((int)(USHORT)pCam_cutinfo->Wait_ctr < (int)Diedemo.Ctr) {
		Diedemo.P_radius = 0;
		Diedemo.Prad = 0;
		Diedemo.Addpos = 0;
		Diedemo.Ctr = 0;
		Diedemo.Rno++;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x80190fb0 (Die_move_main)                                       */
/* State 4, Diedemo.Rno == 4. Per-frame light fade of the current cut   */
/* (G.pRoom->pLight[G.SaveData.Cut_no]) while G.System_flg SYS_DIE_BLOOD */
/* is set, sprite and camera update, and the Rno 4 -> 5 change at       */
/* Ctr == 0xaa. Sets DIESPR Blood_flg each frame.                       */
/* Die_snd_trans(0xc68): same size as the Die_move_pause call.          */
/* ------------------------------------------------------------------ */
void Die_move_main(void)
{
	SHORT          Cut_idx;
	LIGHT_CUT_DATA *pCut;

	Cut_idx = G.SaveData.Cut_no;
	pCut = &G.pRoom->pLight[Cut_idx];
	if ((G.System_flg & SYS_DIE_BLOOD) != 0) {
		if (Diedemo.Ctr - 6 < 4) {
			pCut->Col[2][0] = 0xff;
			pCut->Col[2][1] = 0;
			pCut->Col[2][2] = 0;
			pCut->L[2] = 20000;
		} else if (Diedemo.Ctr - 10 < 0xb) {
			pCut->Col[2][0] += 0xf4;
			pCut->Col[2][1] += 0x0d;
			pCut->Col[2][2] += 0x0d;
			pCut->L[2] -= 0x4b0;
		}
	}
	if (Diedemo.Ctr == 0) {
		while (CD.Cd_rtn_no != 0) {
			Task_sleep(1);
		}
		if ((G.System_flg & SYS_DIE_BLOOD) != 0 && Diedemo.S_trans_flg == 0) {
			Die_snd_trans(0xc68);
			Diedemo.S_trans_flg = 1;
		}
		Die_spr_init();
	}
	Die_spr_move();
	Spiral_move();
	Diedemo.Ctr++;
	if (Diedemo.Ctr == 0xaa) {
		Diedemo.Addpos = 10;
		Diedemo.Daddpos = 2;
		Diedemo.Prad = 0x10;
		Diedemo.Ctr = 0;
		Diedemo.Rno++;
	}
	((DIESPR *)Die_spr_work)->Blood_flg = 1;
	pCut->Col[0][0] = 60;
	pCut->Col[0][1] = 60;
	pCut->Col[0][2] = 60;
}

/* ------------------------------------------------------------------ */
/* DS 0x801911a8 (Die_move_out)                                        */
/* State 5, Diedemo.Rno == 5. Runs the sprite and spiral camera, starts   */
/* the screen fade-in at Ctr == 0x6e, advances Rno after Ctr 0xa2, and    */
/* darkens the current cut's first light entry: each colour byte loses 1  */
/* (saturating at 0) and L[0] loses 100 (not below 0).                    */
/* ------------------------------------------------------------------ */
void Die_move_out(void)
{
	SHORT          Cut_idx;
	LIGHT_CUT_DATA *pLight;

	Cut_idx = G.SaveData.Cut_no;
	pLight = G.pRoom->pLight;
	Die_spr_move();
	Spiral_move();
	Diedemo.Ctr++;
	if (Diedemo.Ctr == 0x6e) {
		Fade_set(0x200, 0x200, 7, 3);
	}
	if (Diedemo.Ctr > 0xa2) {
		Diedemo.Rno++;
	}
	if (pLight[Cut_idx].Col[0][0] != 0) {
		pLight[Cut_idx].Col[0][0]--;
	}
	if (pLight[Cut_idx].Col[0][1] != 0) {
		pLight[Cut_idx].Col[0][1]--;
	}
	if (pLight[Cut_idx].Col[0][2] != 0) {
		pLight[Cut_idx].Col[0][2]--;
	}
	if (pLight[Cut_idx].L[0] > 100) {
		pLight[Cut_idx].L[0] -= 100;
	}
}

/* ------------------------------------------------------------------ */
/* DS 0x801912cc (Die_move_end)                                        */
/* State 6, end of the death demo. Diedemo.Rno = 0, clears SYS_IN_GAME,   */
/* SYS_PSP_TRANS and SYS_DIE_BLOOD in G.System_flg (mask 0xbfeffeff),    */
/* then Snd_sys_init2 and Task_exit.                                      */
/* ------------------------------------------------------------------ */
void Die_move_end(void)
{
	Diedemo.Rno = 0;
	G.System_flg &= ~(SYS_IN_GAME | SYS_PSP_TRANS | SYS_DIE_BLOOD);
	Snd_sys_init2();
	Task_exit();
}

/* ------------------------------------------------------------------ */
/* Spiral_move  DS 0x80191328                                          */
/* Compute spiral camera position and install view matrix              */
/* ------------------------------------------------------------------ */
void Spiral_move(void)
{
	short        Parts_idx;
	int          Trig_val;
	ENEMY_WORK  *pLook_enem;
	MATRIX      *pMx;
	DIEDEMO     *pDiedemo;
	RCUT        *pRcut;
	MATRIX      *pWs;

	pDiedemo = &Diedemo;
	pRcut = &pDiedemo->Rcut;
	pDiedemo->Radius -= pDiedemo->P_radius;
	if (pCam_cutinfo->R_obj == 0) {
		if ((Die_phase & 1) == 0) {
			pMx = &G.Player.pSin_parts_ptr[pCam_cutinfo->R_ptr].Workm;
			goto set_view;
		}
		Parts_idx  = pCam_cutinfo->R_ptr;
		pLook_enem = G.pEnemy[1];
	} else {
		if (pCam_cutinfo->R_obj != 1) goto set_view;
		Parts_idx  = pCam_cutinfo->R_ptr;
		pLook_enem = (ENEMY_WORK *)G.Player.pEnemy_ptr;
	}
	pMx = &pLook_enem->pSin_parts_ptr[Parts_idx].Workm;

set_view:
	pRcut->View_p[1] -= pDiedemo->Addpos;
	pDiedemo->Addpos += pDiedemo->Daddpos;

	Trig_val = rcos((int)(short)pDiedemo->Theta);
	pRcut->View_p[0] = pMx->t[0] + (pDiedemo->Radius * Trig_val >> 0xc);

	Trig_val = rsin((int)(short)pDiedemo->Theta);
	pDiedemo->Theta = (pDiedemo->Theta - pDiedemo->Prad) & 0xfff;
	pRcut->View_p[2] = pMx->t[2] - (pDiedemo->Radius * Trig_val >> 0xc);

	pRcut->View_r[0] = pRcut->View_r[0] + (pMx->t[0] - pRcut->View_r[0]) / 0x1e;
	pRcut->View_r[2] = pRcut->View_r[2] + (pMx->t[2] - pRcut->View_r[2]) / 0x1e;
	pRcut->View_r[1] = pRcut->View_r[1] + (pMx->t[1] - (pRcut->View_r[1] + 300)) / 5;

	Set_view(pRcut->View_p, pRcut->View_r);
	SetGeomScreen(0xd0);
	pWs = &GsWSMATRIX;
	gte_SetRotMatrix((MATRIX *)&GsWSMATRIX);
	gte_SetTransMatrix((MATRIX *)pWs);
}
