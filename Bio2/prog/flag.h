#ifndef FLAG_H
#define FLAG_H
/*
 * flag.h - System flag (G.System_flg), status flag (G.Status_flg) and mode constants
 * Resident Evil 2 (SLUS-00748) decompilation
 * The first names came from a beta source (D:\Bio2\prog\flag.h). Each bit was then checked against its
 * retail usage in the MAIN and overlay dumps: wrong names were renamed (noted "was ..."), and bits with
 * no verified name are not defined here (see the notes after each group).
 */


/*
*	Preprocessor
*/
#define GAME_REGION					1			// 0=JP, 1=Other	// used only for gamepad configurations


/*
*	Snd_se_stad
*/
#define SOUND_CORE_CURSOR			0x04040000
#define SOUND_CORE_CANCEL			0x04050000
#define SOUND_CORE_CONFIRM			0x04060000


/*
*	Bg_set_mode
*/
#define BG_MODE_CLEAR				2			// Clear screen using TILE structure (Bg_set_mode() function sets this)


/*
*	G.System_flg (SYS_*)
*/
#define SYS_TOFU_SURVIVOR			0x00000001	// To-Fu Survivor On/Off
#define SYS_INTRO_PENDING			0x00000002	// Set at boot; Capcom_logo clears it when Padoff_stop_flg[1][1] == 2 (was SYS_BOOT_INTRO)
#define SYS_TIMER_EXPIRED			0x00000004	// Timer has expired (0:00)
#define SYS_TIMER					0x00000008	// Timer On/Off (also the 4th Survivor start selector in Title_game_init)
#define SYS_EASY_MODE				0x00000020	// Easy Mode On/Off
#define SYS_DIE_BLOOD				0x00000100	// Usage verified; the writer was not found in the dumps
#define SYS_CARD_LOAD				0x00000400	// Memory card load (dual use: input mode and success result)
#define SYS_RESET					0x00001000	// Soft Game Reset (set on demo divergence when SYS_IN_GAME is clear)
#define SYS_DEMO_MODE				0x00002000	// Demo Mode On/Off
#define SYS_DOOR_INIT				0x00004000	// Initialize Door Routine - Set_door() function
#define SYS_XA_MOVIE				0x00008000	// XA Movie is currently streaming
#define SYS_STR_MOVIE				0x00000200	// STR movie is streaming; Swap_Cbuff and the OPENING and Capcom_logo paths test it (verified use, not a wait in Title_init)
#define SYS_FADE_OUT				0x00010000	// Fade out (was SYS_BACKGROUND)
#define SYS_NEW_GAME				0x00020000	// New game (was SYS_LOAD_GAME)
#define SYS_SAVE_GAME				0x00040000	// Save Game (MEMCARD.BIN)
#define SYS_ARRANGE					0x00080000	// Arrange Mode On/Off
#define SYS_PSP_TRANS				0x00100000	// Camera Sprite On/Off - Psp_trans() function
#define SYS_ENDING					0x00200000	// End Game (ENDING.BIN)
#define SYS_EX_BATTLE				0x01000000	// Ex Battle (was SYS_ZAPPING)
#define SYS_DOOR_PROCESS			0x02000000	// Door_main() is parsing function table (Door_proc_tbl)
#define SYS_GAME_OVER				0x04000000	// Game Over (DIEDEMO.BIN)
#define SYS_CONFIG					0x08000000	// Configuration (CONFIG.BIN)
#define SYS_SPAD_STACK				0x10000000	// Re/Set SpadStack
#define SYS_BS_READ_SKIP			0x20000000	// Set: the BS file CD read is skipped in Bg_draw (MAIN 8002bdf4)
#define SYS_IN_GAME					0x40000000	// Set in Title_game_init; cleared by Init_global (soft reset, session end, teardown)
#define SYS_PAD_REPEAT				0x80000000	// Pad_set repeat logic, sign bit (was SYS_PLAYER)
/* G.System_flg bits with no verified name yet (beta names were SYS_4TH_SURVIVOR 0x10, SYS_LANG_SUB 0x40,
   SYS_FIRST_RUN 0x80, SYS_LANG_MAIN 0x800): left as raw masks until a retail reader or writer settles them.
   0x10: kept by Init_global, no other reader or writer in retail.
   0x40: set at boot (Init_system) and on card load; Mess_set, Mess_disp and name_ptr_set choose the alternate text table
         on (System_flg & 0x840) == 0x40; never cleared in retail.
   0x80: single latch in Game_loop (set on a new game only); no other reader.
   0x800: never set in retail, so the (0x840) branches see it clear. */


/*
*	G.Status_flg (STAT_*)
*/
#define STAT_QUAKE_RASTA			0x00000002	// Screen quake with Scr_rasta (Scr_quake, MAIN 8002aa6c)
#define STAT_QUAKE_LARGE			0x00000004	// Screen quake, large amplitude (was STAT_SHAKE_FAST)
#define STAT_QUAKE_SMALL			0x00000008	// Screen quake, small amplitude (was STAT_SHAKE_SLOW)
#define STAT_CINESCO				0x00000010	// Letterbox Mode On/Off (For Cutscenes)
#define STAT_XA_VOICE				0x00000020	// XA Voice is currently streaming
#define STAT_CAM_LOCK				0x00000100	// Camera Lock On/Off
#define STAT_STATUS_SCREEN			0x00008000	// Status Screen On/Off
#define STAT_MIRROR_MATRIX			0x00010000	// Mirror Matrix On/Off (Mirror Reflection)
#define STAT_ROOM_FADE				0x00080000	// Room fade (Sce_fade_chk, SCE0.C)
#define STAT_INSPECT				0x00200000	// Player pressed Check/Inspect button On/Off
#define STAT_DOOR_BG_CLEAR			0x00400000	// Set_door: background clear (Bg_set_mode BG_MODE_CLEAR)
#define STAT_INF_AMMO				0x00800000	// Infinite Ammo On/Off
#define STAT_SPECIAL_RUN			0x02000000	// Set with SYS_TIMER on the Title path (was STAT_4TH_SURVIVOR; broad label)
#define STAT_EASY_MODE				0x04000000	// Easy Mode On/Off
#define STAT_TIMER					0x08000000	// Timer On/Off
#define STAT_SUB_PLAYER				0x10000000	// Partner On/Off
#define STAT_ARRANGE_SAVED			0x20000000	// Set in Save_push (MEM_CARD 801c3228) when SYS_ARRANGE is set; Load_pop reads it back
#define STAT_SCENARIO_B				0x40000000	// OFF=A, ON=B
#define STAT_PLAYER					0x80000000	// OFF=PL0, ON=PL1
#define STAT_PL_BUSY			0x00000040	// Player busy or locked (damage, death, stairs, drawer, timed actions, armed, item get); Sce_Hikidashi and the Start menu gate on it
#define STAT_BG_HOLD			0x00000080	// Holds a pending background change: Swap_Cbuff sets Bg_chg_flg only while this is clear
#define STAT_GUEST_REG		0x00000200	// Guest fingerprint registration pending (Computer613, cases 0, 7 and 10)
#define STAT_PUSH_OBJ		0x00002000	// Pushing a movable object (set by Om_move; read by the push routine and its exit)
#define STAT_EX_SAVE		0x00004000	// Save holds Ex-mode data (Save_push sets it; ENDING takes the Ex branch on it)
#define STAT_DOOR_LOAD		0x00020000	// Door load pending (Door_* routines)
/* G.Status_flg bits with no verified name yet: 0x400, 0x800, 0x1000, 0x40000, 0x100000, 0x1000000 (beta names STAT_PAUSE,
   STAT_AUTO_AIM, STAT_AIM_ROCKET, STAT_RELOAD were not supported by retail usage and are removed).
   0x100000: set by Swap_Cbuff (background change) and SCE1 (cut change); cleared in MAIN; main's pause test
             (STAT_CINESCO | 0x100000) blocks pausing while it is set. Two setters, so no single name yet.
   0x1000000: tested only in Game_loop (the game-over branch); never set in retail.
   0x400, 0x800, 0x1000: cleared in Set_room and Sce_work_clr_set, never tested. 0x40000: set in pl_mv05_w1_t1 only. */


#endif /* FLAG_H */
