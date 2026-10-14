/*
 * GAME.C - Main game loop and per-frame game control
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\GAME.C
 */
#include "TYPES.H"
#include "CONFIG.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "GAME.h"
#include "JOINT.H"
#include "DOOR.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "LINE_SCR.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "OBA.H"
#include "OM.H"
#include "PAD.H"
#include "PC_SYS.H"
#include "PLAYER.H"
#include "PSP.H"
#include "ROOM.H"
#include "SCA.H"
#include "SCE0.H"
#include "SCE_AT.H"
#include "SND.H"
#include "STAGE6.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "SUB.H"
#include "TRANS.H"
#include "TITLE.H"
#include "flag.h"

/* 80025794 */
/* Print14 takes a format and its arguments as consecutive words after pPtr (DS 0x8002fa28 forwards &pPtr to Print_main). */
typedef long (*PRINT14_VA)(ULONG x, ULONG y, ULONG c, ULONG p, UCHAR *pPtr, ...);

/* DS 0x80025794 (Game_loop) */
VOID Game_loop(VOID) {
	static INT Stop_bak;		/* 800B66C0 */
	static VECTOR V;			/* 800B66C4 */
	static VECTOR Kpos;			/* 800B66D4 */
	static SVECTOR Kofs;		/* 800B66E4 */
	USHORT Time_cnt;			/* reg 22 */
	ULONG Col;					/* reg 6  */
	ULONG Flg;					/* reg 2  */
	LONG x;						/* reg 2  */
	LONG z;						/* reg 2  */
	ENEMY_WORK * pEm;			/* reg 16 */
	ENEMY_WORK ** ppEm;			/* reg 17 */
	ENEMY_WORK * pPl;			/* reg 18 */
	KAGE_WORK * pKage;			/* reg 17 */

	Title_game_init();
	Task_sleep(1);
	Time_cnt = 0;
	G.Stop_flg = 0;
	Stop_bak = 0;
	Col = 8;
	if ((G.System_flg & SYS_EX_BATTLE) == 0) goto first;
	if (Flg_ck(G.SaveData.Common_flg, 0xfe) == 0) {
		Task_execute(1, (VOID *)Stage6_main_ptr);
		Flg_on(G.SaveData.Common_flg, 0xfe);
	}
first:
	G.pEm = (ENEMY_WORK *)&G.Player;
	G.Player.Routine_0 = 0;
	G.Player.Routine_1 = 0;
	G.Player.Routine_2 = 0;
	G.Player.Routine_3 = 0;
	Player_move((ENEMY_WORK *)&G.Player);
move:
	Flg = 7;
	Stop_bak |= G.Stop_flg;
	G.System_flg &= ~SYS_DOOR_INIT;
	G.Stop_flg |= 0xef000000;
	Fade_set(0x200, -0x1800, 7, 1);
	if ((G.System_flg & SYS_TIMER) != 0 && *(ULONG *)&G.SaveData.Stage_no == 0x1c0000) {
		/* countdown tick: decrement, wrap to 0x1d and drop a second on expiry */
		x = Time_cnt;
		Time_cnt = (USHORT)(Time_cnt - 1);
		Col = Time_cnt;
		if ((x & 0xffff) == 0) {
			G.SaveData.Count_down_timer += -1;
			Time_cnt = 0x1d;
			Col = Time_cnt;
		}
	}
	Game.Doordemo_flg = 5;
trans:
	for (;;) {
		if ((G.Status_flg & 0x1000000) == 0) {
			if ((G.Status_flg & STAT_TIMER) == 0) {
normal:
				V.vx = G.Status_flg;
				if (G.Front_func != NULL) {
					((CODE_PTR)(G.Front_func))();
				}
				if ((G.Status_flg & STAT_CAM_LOCK) == 0) {
					Cut_check(0);
				}
				Sce_scheduler();
				if (Task_status(1) == 0
						&& G.SaveData.Cut_no == (USHORT)G.Next_cut_no
						&& Main.Bg_chg_flg == 0
						&& Game.Doordemo_flg == 0
						&& (G.Status_flg & STAT_PL_BUSY) == 0
						&& (G.Stop_flg & 0x1000000) == 0
						&& G.Player.Damage_cnt == 0
						&& (G.System_flg & (SYS_GAME_OVER | 0x200)) == 0) {
					if ((G.Padd1_trg & 0x100) != 0 && (G.Status_flg & STAT_STATUS_SCREEN) == 0) {
						G.System_flg |= SYS_CONFIG;
						G.Status_flg |= STAT_STATUS_SCREEN;
					}
					if ((G.Key_trg & 0x4000) != 0 && (G.Status_flg & STAT_CINESCO) == 0) {
						G.Status_flg |= STAT_STATUS_SCREEN;
					}
					if ((G.Status_flg & STAT_STATUS_SCREEN) != 0) {
						Game.Doordemo_flg = 1;
						Game.Stop_bak = G.Stop_flg;
						G.Stop_flg |= 0xff000000;
						G.Status_flg |= STAT_PL_BUSY;
						if (St.St_type != 2) {
							Snd_se_stad(0x4060000, (VECTOR *)NULL);
						}
						goto render;
					}
				}
				ppEm = G.pEnemy + 1;
				SetSpadStack();
				pPl = (ENEMY_WORK *)(UINT)(UCHAR)G.Room_enemy_flg;
				*(ULONG *)&Game.Move_flg = *(ULONG *)&Game.Move_flg | 1;
				G.Room_enemy_flg &= 0xff;
				if (G.ppEnemy_end != G.pEnemy + 1) {
					do {
						pEm = *ppEm;
						if ((pEm->Be_flg & 1) != 0 && (pEm->Type & 0x8000) == 0) {
							x = pEm->Pos_x - G.Player.Pos_x;
							z = pEm->Pos_z - G.Player.Pos_z;
							G.pEm = pEm;
							pEm->L_pl = SquareRoot0(x * x + z * z);
							((VOID (*)(VOID))G.pEnemy[(UCHAR)*(LONG *)&pEm->Id + 0x12])();
							pPl = pEm;
						}
						ppEm++;
					} while (ppEm != G.ppEnemy_end);
				}
				G.pEm = (ENEMY_WORK *)&G.Player;
				if ((G.Player.Type & 0x8000) == 0) {
					Player_move((ENEMY_WORK *)&G.Player);
					Oba_ck_em2((ENEMY_WORK *)&G.Player);
					Sca_ck_em((ENEMY_WORK *)&G.Player, 0x8000);
					Set_front_pos((ENEMY_WORK *)&G.Player);
					V.vy = (LONG)G.Player.Fpos_x;
					V.pad = (LONG)G.Player.Fpos_z;
					Flg = 1 << (G.Player.nFloor & 0x1f);
					G.Player.Sca_info = G.Player.Sca_info | Sca_ck_hit((VECTOR *)&V.vy, 0x1c2, Flg, 0x8000) << 0x10;
					pPl = (ENEMY_WORK *)&V.vy;
					Om_move();
				} else {
					Om_move();
				}
				Sce_at();
				if (G.Back_func != NULL) {
					((CODE_PTR)(G.Back_func))();
				}
				ResetSpadStack();
				goto loop_end;
			}
			/* V.vx (Status_flg copy) & STAT_TIMER - countdown timer display */
			Flg = 1;
			if ((G.System_flg & SYS_TIMER) != 0) {
				if ((SHORT)Time_cnt > 0x1d || (V.vx & STAT_TIMER) == 0) {
					Time_cnt = 0;
					Col = 0;
					G.SaveData.Count_down_timer += 1;
				}
				Flg = 0;
				{
					SHORT timer = G.SaveData.Count_down_timer;
					/* DS 0x80025794 timer words: Print14 receives the format at 0x10(sp), the fields at 0x14/0x18/0x1c and two more words at 0x20/0x24.
					 * Print14 passes &pPtr to Print_main, which walks consecutive slots. PRINT14_VA makes the call pass those words on the stack. */
					((PRINT14_VA)Print14)(0x68, 0x20, 0, 2, (UCHAR *)"%2d:%02d:%02d", timer / 60, (timer % 60) / 10, (timer % 60) % 10, (SHORT)Time_cnt / 3, 0);
				}
				if (G.SaveData.Count_down_timer == 5999 && (SHORT)Time_cnt == 0x1b) {
					Col = 8;
				} else if (Game.Doordemo_flg == 0) {
					if (Fade_status(0) != 0 && *(ULONG *)&G.SaveData.Stage_no != 0x1c0000) {
						Time_cnt++;
						Col = Time_cnt;
					}
				}
				goto normal;
			}
			if ((SHORT)Time_cnt > 0x1d || (V.vx & STAT_TIMER) == 0) {
				Time_cnt = 0;
				Col = 8;
				G.SaveData.Count_down_timer += -1;
			}
			if (G.SaveData.Count_down_timer < 0x3c) {
				Flg = 2;
			}
			if (G.SaveData.Count_down_timer < 0) {
				Print14(0x68, 0x20, 2, 0, (UCHAR *)"0 00:00");
			} else {
				{
					SHORT timer = G.SaveData.Count_down_timer;
					/* DS 0x80025794 timer words: Print14 receives the format at 0x10(sp), the fields at 0x14/0x18/0x1c and two more words at 0x20/0x24.
					 * Print14 passes &pPtr to Print_main, which walks consecutive slots. PRINT14_VA makes the call pass those words on the stack. */
					((PRINT14_VA)Print14)(0x68, 0x20, Flg, 2, (UCHAR *)"%2d:%02d:%02d", timer / 60, (timer % 60) / 10, (timer % 60) % 10, 9 - (SHORT)Time_cnt / 3, (LONG)(SHORT)Col);
				}
			}
			if (G.SaveData.Count_down_timer >= 0) {
				if (Game.Doordemo_flg == 0) {
					if (Fade_status(0) != 0) {
						x = Col - 3;
						Time_cnt++;
						Col = (ULONG)x;
						if ((INT)(x << 0x10) < 1) {
							Col = 8;
						}
					}
				}
				goto normal;
			}
			if (Game.Doordemo_flg == 0 && Fade_status(0) != 0 && Task_status(1) == 0) {
				Time_cnt = 0;
				G.Status_flg &= ~STAT_TIMER;
				G.System_flg |= SYS_TIMER_EXPIRED;
				Col = 0;
			}
		} else {
loop_end:
			if ((G.System_flg & SYS_GAME_OVER) != 0 && Task_status(1) == 0
					&& Main.Bg_chg_flg == 0
					&& G.SaveData.Cut_no == (USHORT)G.Next_cut_no) {
				Task_lexecute(1, 2);
				G.System_flg &= ~SYS_GAME_OVER;
			}
		}
render:
		/* per-frame render section */
		if ((G.Status_flg & STAT_MIRROR_MATRIX) != 0) {
			Mirror_matrix_set();
		}
		if ((G.System_flg & SYS_PSP_TRANS) == 0) {
			Psp_trans();
		}
		SetSpadStack();
		if ((Game.Plight_flg & 1) != 0) {
			Gun_light_set();
		}
		ppEm = G.pEnemy;
		if (G.ppEnemy_end != G.pEnemy) {
			do {
				pEm = *ppEm;
				G.pEm = pEm;
				if ((pEm->Be_flg & 1) != 0) {
					pKage = (KAGE_WORK *)pEm->pKage_work;
					if (pEm == (ENEMY_WORK *)&G.Player) {
						Kofs.vz = 0;
						Kofs.vx = G.Player.Kage_ofs;
						Rot_vector(G.Player.Cdir_y, &Kofs, &Kofs);
						Kpos.vy = pKage->pPos->vx + (LONG)Kofs.vx;
						Kpos.vz = pKage->pPos->vy;
						Kpos.pad = pKage->pPos->vz + (LONG)Kofs.vz;
						G.Player.Ground = (SHORT)Sca_get_ground((VECTOR *)&Kpos.vy, 0, (UINT)G.Player.Sc_id << 8, 0);
					} else {
						pEm->Ground = (SHORT)Sca_get_ground(pKage->pPos, 0, (UINT)pEm->Sc_id << 8, 0);
					}
					if (pEm->pTbefore_func != NULL) {
						((CODE_PTR)(pEm->pTbefore_func))(pEm);
					}
					Joint_trans2(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, (VECTOR *)&pEm->Pos_x);
					if ((G.Status_flg & STAT_MIRROR_MATRIX) != 0) {
						Mirror_trans(pEm);
					}
					MulKage(pKage, pEm->Ground, pEm->Cdir_y, (pEm->Be_flg & 0x400) >> 9);
					if (pEm->pTafter_func != NULL) {
						((CODE_PTR)(pEm->pTafter_func))(pEm);
					}
					pEm->Old_x = (SHORT)pEm->Pos_x;
					pEm->Old_y = (SHORT)pEm->Pos_y;
					pEm->Old_z = (SHORT)pEm->Pos_z;
					pEm->Old_x2 = (SHORT)pEm->Atd[0].X;
					pEm->Old_y2 = (SHORT)pEm->Atd[0].Y;
					pEm->Old_z2 = (SHORT)pEm->Atd[0].Z;
				}
				ppEm++;
			} while (ppEm != G.ppEnemy_end);
		}
		if (Game.Plight_flg != 0) {
			Gun_light_reset();
		}
		Kage_work_sort();
		Kage_work9_sort();
		Om_trans();
		if (*(ULONG *)&Game.Move_flg != 0) {
			Esp_move();
		}
		Prim_trans();
		Scr_effect();
		ResetSpadStack();
post_render:
		if ((G.System_flg & SYS_ENDING) != 0) {
			Bg_set_mode(2, 0);
		}
		Task_sleep(1);
		if ((G.System_flg & SYS_ENDING) != 0) {
			Task_lchain(6);
		}
		if ((G.System_flg & SYS_IN_GAME) == 0) {
			if ((G.System_flg & SYS_TIMER) == 0
					&& (G.Status_flg & STAT_SCENARIO_B) == 0
					&& (G.System_flg & 0x20080) == 0x20000
					&& Flg_ck(G.SaveData.Common_flg, 6) == 0
					&& G.SaveData.Save_cnt == 0) {
				G.System_flg |= 0x80;
			}
pl_move:
			Pad_vib_init();
			Bg_set_mode(2, 0);
			if ((G.System_flg & SYS_DEMO_MODE) == 0 || Main.Padoff_stop_flg[1][1] != 0) {
				Main.Padoff_stop_flg[1][1] = 1;
				Init_global();
			} else {
				Init_global();
				Main.Padoff_stop_flg[0][1] = 0;
				G.System_flg |= SYS_INTRO_PENDING;
				Task_execute(1, Game_pad_wait_task);
			}
			Task_lchain(0);
			return;
		}
		*(ULONG *)&Game.Move_flg = 0;
		if (Main.Padoff_stop_flg[0][0] != 0) goto render;
		x = Time_cnt + 1;
		if ((G.System_flg & SYS_TIMER_EXPIRED) != 0) break;
		if ((G.System_flg & SYS_DEMO_MODE) != 0 && Game.Doordemo_flg == 0 && Demo.Pdemo_cnt < G.Pdemo_cnt) {
			G.SaveData.Key_idx = Key_idx_bak;
			G.Stop_flg |= 0xff000000;
			Fade_set(0x200, (Main.Padoff_stop_flg[1][1] == 0) ? 0x200 : 0x800, 7, 1);
			Game.Doordemo_flg = 2;
		}
		switch (Game.Doordemo_flg) {
		case 1:
			if ((Moji.Mess_flg & 0x80) != 0 || Fade_status(0) == 0) break;
			G.Stop_flg |= 0xff000000;
			if ((G.Status_flg & STAT_STATUS_SCREEN) == 0 && (G.System_flg & SYS_SAVE_GAME) == 0) {
				if ((G.System_flg & SYS_EX_BATTLE) == 0) {
					G.System_flg |= SYS_FADE_OUT;
					Bg_set_mode(1, 0);
				} else {
					if ((UCHAR)G.SaveData.Item_flg[7] == 0xff) {
						Flg_on(G.SaveData.Zapping_flg, 0x3f);
					}
					if (Flg_ck(G.SaveData.Zapping_flg, 0x3f) == 0) {
						G.System_flg |= SYS_FADE_OUT;
						Bg_set_mode(1, 0);
					}
				}
				G.Stop_flg = 0;
				Stop_bak = 0;
				G.System_flg |= SYS_DOOR_INIT;
				Set_door();
				goto move;
			}
			Fade_set(0x200, 0x1800, 7, 1);
			Game.Doordemo_flg = 2;
			/* fall through */
		case 2:
			if (Fade_status(0) == 0) goto trans;
			Fade_set(0x200, 0, 7, 1);
			Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)NULL);
			Task_sleep(2);
			if ((G.System_flg & SYS_DEMO_MODE) != 0 && Demo.Pdemo_cnt < G.Pdemo_cnt) {
				Snd_key_off_all();			/* was Snd_key_off_all */
				goto pl_move;
			}
			G.Stop_flg = 0;
			Stop_bak = 0;
			if ((G.Status_flg & STAT_STATUS_SCREEN) == 0) {
				if ((G.System_flg & SYS_SAVE_GAME) != 0) {
					G.System_flg = (G.System_flg & ~SYS_SAVE_GAME) | SYS_DOOR_INIT;
					Task_lexecute(1, 1);
					Stop_bak = *(ULONG *)&Game.Stop_bak;
					*(ULONG *)&Game.Stop_bak = 0;
					Task_sleep(1);
					G.System_flg &= ~SYS_CARD_LOAD;
					Pl_set_packet();
				} else {
					G.System_flg |= SYS_DOOR_INIT;
					Task_sleep(1);
				}
			} else {
				if ((G.System_flg & SYS_CONFIG) == 0) {
					G.System_flg |= SYS_DOOR_INIT;
					Task_execute(1, Status);
					Task_sleep(1);
				} else {
					G.System_flg = (G.System_flg & ~SYS_CONFIG) | SYS_DOOR_INIT;
					Task_lexecute(1, 4);
					Task_sleep(1);
					Pl_set_packet();
				}
				Stop_bak = *(ULONG *)&Game.Stop_bak;
				*(ULONG *)&Game.Stop_bak = 0;
				G.Status_flg &= ~0x8040;
				Task_sleep(1);
			}
			Game.Doordemo_flg = 3;
			goto first;
		case 3:
			goto first;
		case 4:
			Game.Doordemo_flg = 5;
			G.Stop_flg |= 0xff000000;
			break;
		case 5:
			if (Fade_status(0) == 0) {
				Game.Doordemo_flg = 0;
				G.Stop_flg = Stop_bak;
			}
			break;
		case 6:
			while (Game.Doordemo_flg == 6) {
				Task_sleep(1);
			}
			break;
		case 7:
			Task_execute(1, Windows);
			Game.Doordemo_flg = 0;
			goto post_render;
		default:
			break;
		}
	}
	Time_cnt = (USHORT)x;
	if ((SHORT)x == 0x10) {
		Task_lexecute(1, 2);
		Moji.Mess_flg = 0;
		Time_cnt = (USHORT)-0x7530;
	}
	goto render;
}

/* 80026b2c */
/* DS 0x80026b2c (Game_pad_wait_task) */
VOID Game_pad_wait_task(VOID) {
	while (Main.Padoff_stop_flg[0][1] == 0) {
		Task_sleep(1);
	}
	Main.Padoff_stop_flg[0][1] = 0;
	Task_exit();
}

/* 80026b7c */
/* DS 0x80026b7c (Set_door) */
VOID Set_door(VOID) {
	Pad_vib_init();
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		Bgm_unlock_ck();
	}
	G.System_flg |= SYS_DOOR_PROCESS;
	if ((G.System_flg & SYS_EX_BATTLE) == 0 || Flg_ck(G.SaveData.Zapping_flg, 0x3f) == 0) {
		Bg_set_mode(2, 0);
		Task_execute(1, Door_main);
		Task_sleep(1);
		Door_snd_trans();
		Task_sleep(1);
	} else {
		Door_ex_rno = 0;
		G.Next_cut_no = (UCHAR)G.SaveData.Cut_no;
		Task_execute(1, Door_main_ex);
		while ((G.System_flg & SYS_DOOR_PROCESS) != 0 && Door_ex_rno != 4) {
			Task_sleep(1);
		}
	}
	G.Player.Old_x = G.pSce_aot_on->Next_pos_x;
	G.Player.Pos_x = (LONG)G.Player.Old_x;
	G.Player.Old_y = G.pSce_aot_on->Next_pos_y;
	G.Player.Pos_y = (LONG)G.Player.Old_y;
	G.Player.Old_z = G.pSce_aot_on->Next_pos_z;
	G.Player.Pos_z = (LONG)G.Player.Old_z;
	G.Player.Cdir_y = G.pSce_aot_on->Next_cdir_y;
	G.Player.nFloor = G.pSce_aot_on->Next_nfloor;
	G.Player.Ground = (SHORT)((USHORT)G.Player.nFloor * (SHORT)-0x708);
	G.Next_cut_no = G.pSce_aot_on->Next_cut;
	G.SaveData.Room_no_old = G.SaveData.Room_no + (G.SaveData.Stage_no + 1) * 0x100;
	G.SaveData.Cut_no = (SHORT)G.Next_cut_no;
	G.SaveData.Room_no = (SHORT)G.pSce_aot_on->Next_room;
	G.Player.Sca_old_x = G.Player.Old_x;
	G.Player.Sca_old_z = G.Player.Old_z;
	if ((INT)G.SaveData.Stage_no != (INT)((UINT)G.pSce_aot_on->Next_stage % 9)) {
		G.SaveData.Stage_no = (SHORT)((UINT)G.pSce_aot_on->Next_stage % 9);
		Set_stage();
	}
	while ((G.Status_flg & STAT_DOOR_LOAD) != 0) {
		Task_sleep(1);
	}
	Snd_bgm_ck();
	Kage_work_init();
	Kage_work9_init();
	Set_room();
	while ((G.System_flg & SYS_DOOR_PROCESS) != 0) {
		Task_sleep(1);
	}
	Bg_set_mode(0, 0);
	Cut_check(1);
	Main.Bg_chg_flg = 1;
	Task_sleep(1);
	if ((G.Status_flg & STAT_DOOR_BG_CLEAR) != 0) {
		Bg_set_mode(2, 0);
		G.Status_flg &= ~STAT_DOOR_BG_CLEAR;
	}
}

/* 80026ed8 */
/* DS 0x80026ed8 (Gun_light_set) */
VOID Gun_light_set(VOID) {
	LIGHT_CUT_DATA * pLight;	/* reg 17 - 40 bytes */
	SVECTOR * pV;

	pLight = G.pRoom->pLight + G.SaveData.Cut_no;
	Memcpy((VOID *)((INT)Game.Plight_bak + 2), pLight, 0x28);
	pLight->Mode[2] = 0;
	pLight->L[2] = 1;
	pV = ESP_PM_TMP;
	pV->vx = 0x4b0;		/* DS stores the 16-bit value (0xb0 | 4 << 8) in one sh */
	ESP_PM_TMP->vz = 0;
	Rot_vector(G.Player.Cdir_y, pV, pV);
	Game.Plight_pos.vy = -0x960;
	Game.Plight_col[0] = 0xff;
	Game.Plight_col[1] = 0xe6;
	Game.Plight_col[2] = 0xa0;
	Game.Plight_pos.vx = G.Player.Pos_x + pV->vx;
	Game.Plight_pos.vz = G.Player.Pos_z + ESP_PM_TMP->vz;
	switch (Game.Plight_cnt) {	/* DS tests 0, then 1, then leaves */
	case 0:
		if (pLight->Col[2][0] < 0xff) {	/* DS: sltiu 0xff */
			pLight->Col[2][0] = 0xff;
		}
		if (pLight->Col[2][1] < Game.Plight_col[1]) {
			pLight->Col[2][1] = Game.Plight_col[1];
		}
		if (pLight->Col[2][2] < Game.Plight_col[2]) {
			pLight->Col[2][2] = Game.Plight_col[2];
		}
		Game.Plight_cnt++;
		break;
	case 1:
		if (pLight->Col[2][0] < 0x7f) {
			pLight->Col[2][0] = 0x7f;
		}
		if (pLight->Col[2][1] < Game.Plight_col[1] >> 1) {
			pLight->Col[2][1] = Game.Plight_col[1] >> 1;
		}
		if (pLight->Col[2][2] < Game.Plight_col[2] >> 1) {
			pLight->Col[2][2] = Game.Plight_col[2] >> 1;
		}
		Game.Plight_cnt++;
		break;
	}
	pLight->Pos[2][0] = Game.Plight_pos.vx;
	pLight->Pos[2][1] = Game.Plight_pos.vy;
	pLight->Pos[2][2] = Game.Plight_pos.vz;
	pLight->L[2] = 6000;
}

/* 800270ec */
/* DS 0x800270ec (Gun_light_reset) */
VOID Gun_light_reset(VOID) {
	Memcpy(G.pRoom->pLight + G.SaveData.Cut_no, (VOID *)((INT)Game.Plight_bak + 2), 0x28);
	if (Game.Plight_cnt > 1) {
		Game.Plight_flg = 0;
		Game.Plight_cnt = 0;
	}
}

/* DS 0x800df348 (Game) */
tagGAME Game;

/* DS 0x800df390 (Demo) */
PDEMO Demo;

/* DS 0x800dfad6 (Key_idx_bak) */
UCHAR Key_idx_bak = 0;
