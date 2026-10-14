/*
 * SCE0.C - Scene task init, event execution and scheduler
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE0.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <MALLOC.H>
#include <STRINGS.H>
#include "ROOM.H"
#include "SLD.H"
#include "SCE0.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "OM.H"
#include "OMA.H"
#include "PL_WEP.H"
#include "SCE1.H"
#include "SCE2.H"
#include "STATUS.H"
#include "SUB.H"
#include "TITLE.H"
#include "GAME.H"
#include "flag.h"
#include "sce_defines.h"

/* DS 0x800a7400 */
UCHAR em_se_tbl[] = {
	3, 0, 5, 0, 6, 0, 36, 0, 46, 0, 47, 0, 12, 0, 13, 0,
	14, 0, 51, 0, 15, 0, 16, 0, 18, 0, 19, 0, 48, 0, 50, 0,
	20, 0, 22, 0, 23, 0, 24, 0, 28, 0, 33, 0, 35, 0, 34, 0,
	38, 0, 39, 0, 26, 0, 40, 0, 0, 29, 0, 30, 0, 31, 0, 32,
	1, 2, 3, 4, 3, 6, 3, 42, 3, 10, 3, 31, 3, 32, 4, 6,
	5, 6, 5, 10, 5, 17, 5, 31, 5, 32, 6, 43, 6, 31, 6, 32,
	7, 8, 7, 41, 9, 41, 9, 11, 9, 45, 37, 31, 37, 32, 42, 43,
	42, 44, 46, 32, 47, 32, 20, 21, 18, 32, 28, 32, 35, 31, 35, 32,
	49, 40, 16, 31, 16, 32, 22, 31, 22, 32, 23, 40, 19, 31, 19, 32,
	52, 0, 255, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0,
	1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1,
	0, 0, 1, 1, 255, 0, 0, 0
};

/* DS 0x800a74c8: 143 handlers, script opcodes 0x00..0x8e (all 143 words checked against the exe) */
SCE_JMP_FUNC Sce_jmp_tbl[143] = {
	(SCE_JMP_FUNC)Nop, (SCE_JMP_FUNC)Evt_end, (SCE_JMP_FUNC)Evt_next, (SCE_JMP_FUNC)Evt_chain,
	(SCE_JMP_FUNC)Evt_exec, (SCE_JMP_FUNC)Evt_kill, (SCE_JMP_FUNC)Ifel_ck, (SCE_JMP_FUNC)Else_ck,
	(SCE_JMP_FUNC)Endif, (SCE_JMP_FUNC)Sleep, (SCE_JMP_FUNC)Sleeping, (SCE_JMP_FUNC)Wsleep,
	(SCE_JMP_FUNC)Wsleeping, (SCE_JMP_FUNC)For, (SCE_JMP_FUNC)Next, (SCE_JMP_FUNC)While,
	(SCE_JMP_FUNC)Ewhile, (SCE_JMP_FUNC)Do, (SCE_JMP_FUNC)Edwhile, (SCE_JMP_FUNC)Switch,
	(SCE_JMP_FUNC)Case, (SCE_JMP_FUNC)Default, (SCE_JMP_FUNC)Eswitch, (SCE_JMP_FUNC)Goto,
	(SCE_JMP_FUNC)Gosub, (SCE_JMP_FUNC)Return, (SCE_JMP_FUNC)Break, (SCE_JMP_FUNC)For2,
	(SCE_JMP_FUNC)Break_point, (SCE_JMP_FUNC)Work_copy, (SCE_JMP_FUNC)Nop, (SCE_JMP_FUNC)Nop,
	(SCE_JMP_FUNC)Nop, (SCE_JMP_FUNC)Ck, (SCE_JMP_FUNC)Set, (SCE_JMP_FUNC)Cmp,
	(SCE_JMP_FUNC)Save, (SCE_JMP_FUNC)Copy, (SCE_JMP_FUNC)Calc, (SCE_JMP_FUNC)Calc2,
	(SCE_JMP_FUNC)Sce_rnd, (SCE_JMP_FUNC)Cut_chg, (SCE_JMP_FUNC)Cut_old, (SCE_JMP_FUNC)Message_on,
	(SCE_JMP_FUNC)Aot_set, (SCE_JMP_FUNC)Obj_model_set, (SCE_JMP_FUNC)Work_set, (SCE_JMP_FUNC)Speed_set,
	(SCE_JMP_FUNC)Add_speed, (SCE_JMP_FUNC)Add_aspeed, (SCE_JMP_FUNC)Pos_set, (SCE_JMP_FUNC)Dir_set,
	(SCE_JMP_FUNC)Member_set, (SCE_JMP_FUNC)Member_set2, (SCE_JMP_FUNC)Se_on, (SCE_JMP_FUNC)Sca_id_set,
	(SCE_JMP_FUNC)Flr_set, (SCE_JMP_FUNC)Dir_ck, (SCE_JMP_FUNC)Sce_espr_on, (SCE_JMP_FUNC)Door_aot_set,
	(SCE_JMP_FUNC)Cut_auto, (SCE_JMP_FUNC)Member_copy, (SCE_JMP_FUNC)Member_cmp, (SCE_JMP_FUNC)Plc_motion,
	(SCE_JMP_FUNC)Plc_dest, (SCE_JMP_FUNC)Plc_neck, (SCE_JMP_FUNC)Plc_ret, (SCE_JMP_FUNC)Plc_flg,
	(SCE_JMP_FUNC)Sce_em_set, (SCE_JMP_FUNC)Col_chg_set, (SCE_JMP_FUNC)Aot_reset, (SCE_JMP_FUNC)Aot_on,
	(SCE_JMP_FUNC)Super_set, (SCE_JMP_FUNC)Super_reset, (SCE_JMP_FUNC)Plc_gun, (SCE_JMP_FUNC)Cut_replace,
	(SCE_JMP_FUNC)Sce_espr_kill, (SCE_JMP_FUNC)Door_model_set, (SCE_JMP_FUNC)Item_aot_set, (SCE_JMP_FUNC)Sce_key_ck,
	(SCE_JMP_FUNC)Sce_trg_ck, (SCE_JMP_FUNC)Sce_bgm_control, (SCE_JMP_FUNC)Sce_espr_control, (SCE_JMP_FUNC)Sce_fade_set,
	(SCE_JMP_FUNC)Sce_espr3d_on, (SCE_JMP_FUNC)Member_calc, (SCE_JMP_FUNC)Member_calc2, (SCE_JMP_FUNC)Sce_bgmtbl_set,
	(SCE_JMP_FUNC)Plc_rot, (SCE_JMP_FUNC)Xa_on, (SCE_JMP_FUNC)Weapon_chg, (SCE_JMP_FUNC)Plc_cnt,
	(SCE_JMP_FUNC)Sce_shake_on, (SCE_JMP_FUNC)Mizu_div_set, (SCE_JMP_FUNC)Keep_Item_ck, (SCE_JMP_FUNC)Xa_vol,
	(SCE_JMP_FUNC)Kage_set, (SCE_JMP_FUNC)Cut_be_set, (SCE_JMP_FUNC)Sce_Item_lost, (SCE_JMP_FUNC)Plc_gun_eff,
	(SCE_JMP_FUNC)Sce_espr_on2, (SCE_JMP_FUNC)Sce_espr_kill2, (SCE_JMP_FUNC)Plc_stop, (SCE_JMP_FUNC)Aot_set_4p,
	(SCE_JMP_FUNC)Door_aot_set_4p, (SCE_JMP_FUNC)Item_aot_set_4p, (SCE_JMP_FUNC)Light_pos_set, (SCE_JMP_FUNC)Light_kido_set,
	(SCE_JMP_FUNC)Rbj_reset, (SCE_JMP_FUNC)Sce_scr_move, (SCE_JMP_FUNC)Parts_set, (SCE_JMP_FUNC)Movie_on,
	(SCE_JMP_FUNC)Splc_ret, (SCE_JMP_FUNC)Splc_sce, (SCE_JMP_FUNC)Super_on, (SCE_JMP_FUNC)Mirror_set,
	(SCE_JMP_FUNC)Sce_fade_adjust, (SCE_JMP_FUNC)Sce_espr3d_on2, (SCE_JMP_FUNC)Sce_Item_get, (SCE_JMP_FUNC)Sce_line_start,
	(SCE_JMP_FUNC)Sce_line_main, (SCE_JMP_FUNC)Sce_line_end, (SCE_JMP_FUNC)Sce_parts_bomb, (SCE_JMP_FUNC)Sce_parts_down,
	(SCE_JMP_FUNC)Light_color_set, (SCE_JMP_FUNC)Light_pos_set2, (SCE_JMP_FUNC)Light_kido_set2, (SCE_JMP_FUNC)Light_color_set2,
	(SCE_JMP_FUNC)Se_vol, (SCE_JMP_FUNC)Sce_Item_cmp, (SCE_JMP_FUNC)Sce_espr_task, (SCE_JMP_FUNC)Plc_heal,
	(SCE_JMP_FUNC)St_map_hint, (SCE_JMP_FUNC)Sce_em_pos_ck, (SCE_JMP_FUNC)Poison_ck, (SCE_JMP_FUNC)Poison_clr,
	(SCE_JMP_FUNC)Sce_Item_lost2, (SCE_JMP_FUNC)Evt_next2, (SCE_JMP_FUNC)Vib_set0, (SCE_JMP_FUNC)Vib_set1,
	(SCE_JMP_FUNC)Vib_fade_set, (SCE_JMP_FUNC)Item_aot_set2, (SCE_JMP_FUNC)Sce_em_set2
};


/* DS 0x800a7704 */
UCHAR Em_kind_tbl[116] = {
	0x10, 0x10, 0x11, 0x10, 0x12, 0x10, 0x13, 0x10, 0x15, 0x10, 0x16, 0x10, 0x17, 0x10, 0x18, 0x10,
	0x1E, 0x10, 0x1F, 0x10, 0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x25, 0x25, 0x26, 0x26,
	0x27, 0x27, 0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E,
	0x2F, 0x2F, 0x30, 0x30, 0x31, 0x31, 0x33, 0x33, 0x34, 0x34, 0x36, 0x36, 0x37, 0x37, 0x39, 0x39,
	0x3A, 0x3A, 0x3B, 0x3B, 0x3E, 0x3E, 0x3F, 0x3F, 0x40, 0x40, 0x41, 0x51, 0x42, 0x50, 0x43, 0x51,
	0x44, 0x50, 0x45, 0x45, 0x46, 0x50, 0x47, 0x51, 0x48, 0x50, 0x49, 0x51, 0x4A, 0x10, 0x4B, 0x51,
	0x4F, 0x45, 0x50, 0x50, 0x51, 0x51, 0x54, 0x50, 0x55, 0x51, 0x58, 0x50, 0x59, 0x51, 0x5A, 0x50,
	0x5B, 0x51, 0xFF, 0x00,
};


/* DS 0x800d7860 (Sce): scenario state, sizeof(SCENARIO) 5236; the file bytes are all zero */
SCENARIO Sce;


/*
 * Sce_test_init  (DS 0x800529b4)
 * Initialises all 14 SCE task slots and sets the script random seed.
 * Note: the retail loop stores a descending level (13..0) into
 * Sce_task[0..13].Task_level.
 */
/* DS 0x800529b4 (Sce_test_init) */
void Sce_test_init(void)
{
	ULONG     i;
	SCE_TASK *pTask;

	i     = 14;
	pTask = Sce.Sce_task;
	do {
		i--;
		pTask->Task_level  = (UCHAR)i;
		pTask->Status      = 0;
		pTask->Sub_ctr     = 0;
		pTask->Ifel_ctr[0] = -1;
		pTask->Loop_ctr[0] = -1;
		pTask++;
	} while (i != 0);
	Sce.Random_base = 0x138201c3;
}

/*
 * Sce_rnd_item_set  (DS 0x800529fc)  -- retail-only
 * Seeds Rnd() from the VSync counter, then builds a random 8-bit mask with
 * exactly four bits set (four "hits" out of eight).  The mask is stored in
 * the low byte of Item_flg[7] and its complement in the low byte of ExData0[7].
 * Called from Title_game_init.
 */
/* DS 0x800529fc (Sce_rnd_item_set) */
void Sce_rnd_item_set(void)
{
	int   Cnt;
	ULONG Bit;
	ULONG Hit;
	ULONG Mask;

	Cnt = VSync(-1) % 512;
	while (Cnt-- != 0) {
		Rnd();
	}

	Mask = 0;
	Hit  = 0;
	Bit  = 0;
	while (1) {
		if ((Rnd() & 1) != 0) {
			Hit++;
			Mask |= 1 << Bit;
			if (Hit == 4) {
				break;
			}
		}
		Bit++;
		if (Bit - Hit >= 4) {
			/* four misses: the remaining bits must all be set */
			Mask |= 0xf0 << Hit;
			break;
		}
		if (Bit >= 8) {
			break;
		}
	}
	*(UCHAR *)&G.SaveData.Item_flg[7] = (UCHAR)Mask;
	*(UCHAR *)&G.SaveData.ExData0[7]  = (UCHAR)~Mask;
}

/*
 * Em_kind_search  (DS 0x80052af4)
 * Searches Em_kind_tbl (Id, kind byte pairs) for Id and returns the kind.
 * Returns 0x80 if not found.
 * Note: the retail loop only continues while the next Id byte is 0xff, so
 * effectively only the first entry is tested (faithful to the binary).
 */
/* DS 0x80052af4 (Em_kind_search) */
ULONG Em_kind_search(ULONG Id)
{
	UCHAR *p;

	p = Em_kind_tbl;
	do {
		if ((ULONG)p[0] == Id) {
			return (ULONG)p[1];
		}
		p += 2;
	} while (p[0] == 0xff);
	return 0x80;
}

/*
 * Sce_se_set  (DS 0x80052b38)
 * Looks up the Se_tmp0/Se_tmp1 pair in em_se_tbl.  If found, the index is
 * stored in G.Em_se_set_no and BE_FLG 0x2000 is set on the live enemies
 * whose Se_type matches the second member of the pair.
 */
/* DS 0x80052b38 (Sce_se_set) */
void Sce_se_set(void)
{
	UCHAR        *A0;
	ULONG         Flg;
	ENEMY_WORK   *pEm;
	ENEMY_WORK  **ppEm;

	if (Sce.Se_tmp0 != 0 || Sce.Se_tmp1 != 0) {
		Flg            = 0;
		A0             = em_se_tbl;
		G.Em_se_set_no = 0;
		while (A0[0] != 0xff) {
			if (A0[0] == Sce.Se_tmp0 &&
				(Sce.Se_tmp1 == 0 || Sce.Se_tmp1 == A0[1])) {
				goto step1;
			}
			if (A0[1] == Sce.Se_tmp0 &&
				(Sce.Se_tmp1 == 0 || Sce.Se_tmp1 == A0[0])) {
				Flg = 1;
				goto step1;
			}
			A0 += 2;
			G.Em_se_set_no++;
		}
	}
	G.Em_se_set_no = 0xff;
	return;

step1:
	for (ppEm = G.pEnemy + 1; ppEm != G.ppEnemy_end; ppEm++) {
		pEm = *ppEm;
		if ((pEm->Be_flg & 1) == 0) {
			continue;
		}
		if ((int)(signed char)pEm->Se_type == (int)Sce.Se_tmp0) {
			if (Flg) {
				pEm->Be_flg |= 0x2000;
			}
		} else if ((int)(signed char)pEm->Se_type == (int)Sce.Se_tmp1 && !Flg) {
			pEm->Be_flg |= 0x2000;
		}
	}
}

/*
 * Sce_rnd_set  (DS 0x80052ca0)
 * Updates the script random number each frame.
 * Uses Rnd() in debug mode (System_flg 0x1000000), otherwise steps the
 * Random_base generator.
 */
/* DS 0x80052ca0 (Sce_rnd_set) */
void Sce_rnd_set(void)
{
	ULONG D0;
	ULONG D1;

	if ((G.System_flg & SYS_EX_BATTLE) == 0) {
		D1 = Sce.Random_base << 1;
		D0 = ((Sce.Random_base + (D1 >> 16)) & 0xffff) + (D1 & 0xffff0000);
		Sce.Random_base = D0;
	} else {
		D0 = Rnd();
	}
	G.SaveData.Sce_random = (short)D0;
}

/*
 * Sce_model_init  (DS 0x80052d14)
 * Clears the 32 object model slots and assigns tpage/clut for each room
 * object model, mapping its TIM if it differs from the previous one.
 */
/* DS 0x80052d14 (Sce_model_init) */
void Sce_model_init(void)
{
	ULONG            i;
	ULONG           *pTim;
	ULONG            S0;
	OBJ_MODEL_WORK  *pOm;
	MODEL_LINK_INFO *pMi;
	UCHAR           *pCl;
	tagGLOBAL       *pG;

	pG = &G;
	pG->pOld_tim = NULL;
	pG->pOld_tmd = NULL;
	pOm = pG->Ob_model;
	for (i = 32; i != 0; i--) {
		pOm->Be_flg = 0;
		pOm->Free0  = 0;
		pOm++;
	}

	S0  = (ULONG)pG->pRoom->nOmodel;
	pMi = pG->pRoom->pOmodel;
	pOm = pG->Ob_model;
	pCl = &pG->Ob_model[0].Clut;	/* same as pOm->Clut, stepped on its own */
	for (; S0 != 0; S0--) {
		pTim      = (ULONG *)pMi->Tim_adr;
		pCl[-1]   = pG->Ctpage;
		*pCl      = pG->Cclut;
		if (pG->pOld_tim == pTim) {
			/* same TIM as the previous model: share its tpage/clut */
			pCl[-1] = pCl[-505];
			*pCl    = pCl[-504];
		} else {
			pG->pOld_tim = pTim;
			if (pTim == (ULONG *)pG->pRoom) {
				pOm->Be_flg = 0x80000000;
			} else {
				Mapping_tim(pTim);
			}
		}
		pOm++;
		pCl += 504;
		pMi++;
	}
}

/*
 * Sce_work_clr  (DS 0x80052e20)
 * Clears per-frame work variables (atari flags, use flags, mizu counter).
 */
/* DS 0x80052e20 (Sce_work_clr) */
void Sce_work_clr(void)
{
	G.SaveData.F_atari     = -1;
	G.SaveData.U_atari     = -1;
	G.SaveData.Use_id      = -1;
	G.SaveData.Get_id      = -1;
	G.SaveData.Room_flg[1] = 0;
	G.Use_flg[0]           = 0;
	G.Use_flg[1]           = 0;
	G.Use_flg[2]           = 0;
	G.Use_flg[3]           = 0;
	Sce.Mizu_div_ctr       = 0;
}

/*
 * Sce_work_clr_at  (DS 0x80052e7c)
 * Resets At_obj_no / At_sce_no to 0xff for the player, all live enemies and
 * all live object models.  DS writes both bytes with one halfword store of
 * 0xffff; this code uses two byte stores to the same two bytes.
 */
/* DS 0x80052e7c (Sce_work_clr_at) */
void Sce_work_clr_at(void)
{
	ENEMY_WORK     *pEw;
	ENEMY_WORK    **ppEm;
	OBJ_MODEL_WORK *pOm;
	OBJ_MODEL_WORK *pOm2;

	G.Player.At_obj_no = 0xff;
	G.Player.At_sce_no = 0xff;
	for (ppEm = G.pEnemy + 2; ppEm != G.ppEnemy_end; ppEm++) {
		pEw = *ppEm;
		if ((pEw->Be_flg & 1) != 0) {
			pEw->At_obj_no = 0xff;
			pEw->At_sce_no = 0xff;
		}
	}
	pOm2 = G.Ob_model + G.pRoom->nOmodel;
	for (pOm = G.Ob_model; pOm < pOm2; pOm++) {
		if ((pOm->Be_flg & OM_ENABLED) != 0) {
			pOm->At_obj_no = 0xff;
			pOm->At_sce_no = 0xff;
		}
	}
}

/*
 * Sce_work_clr_set  (DS 0x80052f3c)
 * Full room-change work reset: task slots 0..9, AOT list, flags,
 * enemy/model state, priority Be_flg table.
 */
/* DS 0x80052f3c (Sce_work_clr_set) */
void Sce_work_clr_set(void)
{
	ULONG     i;
	ULONG    *A0;
	SCE_TASK *pTask;

	pTask = Sce.Sce_task;
	for (i = 0; i < 10; i++) {
		pTask->Task_level  = (UCHAR)i;
		pTask->Status      = 0;
		pTask->Sub_ctr     = 0;
		pTask->Ifel_ctr[0] = -1;
		pTask->Loop_ctr[0] = -1;
		pTask++;
	}

	Sce_aot_init();
	G.SaveData.Room_flg[0]  = 0;
	Sce.Mizu_div_max        = 0;
	Sce.Mizu_div_ctr        = 0;
	Sce.Rbj_reset_flg       = 0;
	G.SaveData.Def_em_set   = -1;
	G.SaveData.Def_aot_set  = -1;
	G.Room_enemy_flg        = 0;
	G.Room_player_flg       = 0;
	Cc_work.Ctex_old        = 0xff;
	Sce.C_id                = 0xff;
	Sce.C_model_type        = 0xff;
	Sce.C_kind              = 0xff;
	Sce.Se_vol              = 0xff;
	Cc_work.Ccol_no         = 0;
	Sce.Se_tmp0             = 0;
	Sce.Se_tmp1             = 0;
	CD.Vol                  = 0x78;
	G.Status_flg           &= 0xfffff3ff;
	Sce.pC_em               = G.pEnemy[2];

	A0 = &G.SaveData.Pri_be_flg[0][0];
	for (i = 0x40; i != 0; i--) {
		*A0++ = 0xffffffff;
	}
	if ((G.Player.Be_flg & 0x400) != 0) {
		G.Player.Be_flg &= ~0x400;
	}
	G.Player.Type &= 0xfff;
}

/*
 * Sce_aot_init  (DS 0x800530c0)
 * Clears the G.Sce_aot pointer array and resets G.nSce_aot.
 */
/* DS 0x800530c0 (Sce_aot_init) */
void Sce_aot_init(void)
{
	ULONG     i;
	SCE_AOT **pAot;

	pAot = G.Sce_aot;
	for (i = 32; i != 0; i--) {
		*pAot++ = NULL;
	}
	G.nSce_aot = 0;
}

/*
 * Event_init  (DS 0x800530ec)
 * Initialises a SCE task to run event Evt_no of the current script.
 */
/* DS 0x800530ec (Event_init) */
void Event_init(SCE_TASK *pSce, ULONG Evt_no)
{
	UCHAR *pScd;
	ULONG  D0;

	pScd               = Sce.pCScd;
	D0                 = ((USHORT *)pScd)[Evt_no];
	pSce->pS_SP        = pSce->Stack[(signed char)pSce->Sub_ctr];
	pSce->Status       = 1;
	pSce->Routine0     = 0;
	pSce->Ifel_ctr[0]  = -1;
	pSce->Loop_ctr[0]  = -1;
	pSce->Data         = pScd + D0;
}

/*
 * Event_exec  (DS 0x80053138)
 * Starts event Evt_no on task Task_level.  A Task_level outside the
 * normal range selects the first free task (2..9 for room scripts,
 * 10..13 otherwise); if all are busy the last one is reused.
 */
/* DS 0x80053138 (Event_exec) */
void Event_exec(ULONG Task_level, ULONG Evt_no)
{
	ULONG     i;
	ULONG    *pSpd;
	SCE_TASK *pTask;

	if (Sce.Type == 0) {
		if (Task_level > 9) {
			Task_level = 2;
			pTask      = &Sce.Sce_task[2];
			while (pTask->Status != 0) {
				if (Task_level == 9) {
					break;
				}
				Task_level++;
				if (Task_level > 9) {
					return;
				}
				pTask++;
			}
		}
	} else if (Task_level > 13) {
		Task_level = 10;
		pTask      = &Sce.Sce_task[10];
		while (pTask->Status != 0) {
			if (Task_level == 13) {
				break;
			}
			Task_level++;
			if (Task_level > 13) {
				return;
			}
			pTask++;
		}
	}

	pTask          = &Sce.Sce_task[Task_level];
	pTask->Sub_ctr = 0;
	/* clear Spd_x .. Adspd_z (12 shorts) */
	pSpd = (ULONG *)&pTask->Spd_x;
	for (i = 6; i != 0; i--) {
		*pSpd++ = 0;
	}
	Event_init(pTask, Evt_no);
}

/*
 * Sce_col_chg_init  (DS 0x80053244)
 * Initialises colour/texture change work for all active CC_WORK entries.
 */
/* DS 0x80053244 (Sce_col_chg_init) */
void Sce_col_chg_init(void)
{
	ULONG    D0;
	ULONG    i;
	CC_WORK *pCcp;
	ULONG    Pad[2];

	(void)Pad;
	D0 = (ULONG)Cc_work.Ccol_no;
	if (D0 != 0) {
		pCcp = &Cc_work;
		for (i = 0; i < D0; i++) {
			if ((pCcp->Cc_parts[0].Cc_ctr & 0x80) != 0) {
				Col_chg_init(i, pCcp->Cc_parts);
			} else if ((pCcp->Cc_parts[0].Cc_ctr & 0x40) != 0) {
				Tex_chg_init(i, pCcp->Cc_parts);
			}
			pCcp = (CC_WORK *)&pCcp->Cc_parts[1];
		}
	}
}

/*
 * Sce_mirror_init  (DS 0x800532d8)
 * Builds the mirror (reflected) player model if the mirror flag is set.
 */
/* DS 0x800532d8 (Sce_mirror_init) */
void Sce_mirror_init(void)
{
	ENEMY_WORK *pEm;
	PARTS_W    *pParts;

	if ((G.Status_flg & STAT_MIRROR_MATRIX) != 0) {
		pEm = (ENEMY_WORK *)&G.Player;
		Weapon_cls(pEm);
		pParts               = G.Player.pSin_parts_ptr;
		pParts[11].pTmd      = (ULONG *)gpPl_tmd_ptr;
		pParts[11].pTmd2     = (ULONG *)gpPl_tmd2_ptr;
		pParts[11].pPacket   = (ULONG *)gpPl_pac_ptr;
		pParts[11].pPacket2  = (ULONG *)gpPl_pac2_ptr;
		G.pFree_work = (UCHAR *)Mirror_model_cp(pEm, (ULONG)G.pFree_work);
		Pl_weapon_ch(pEm);
		G.pFree_work = (UCHAR *)Init_mirror_weapon(pEm, (ULONG)G.pFree_work);
		Mirror_weapon_cp(pEm);
	}
}

/*
 * Sce_kirakira_set  (DS 0x80053394)
 * Spawns the sparkle (kirakira) effect on live object models whose Free0
 * (the Item_aot_set action byte) has bit 0x80 set; bits 0x60 select the
 * effect size (0 -> 1).
 */
/* DS 0x80053394 (Sce_kirakira_set) */
void Sce_kirakira_set(void)
{
	ULONG           Cnt;
	ULONG           i;
	ULONG           D0;
	SVECTOR         Svec[2];	/* retail reserves two SVECTORs; only Svec[0] is used */
	OBJ_MODEL_WORK *pOm;

	Cnt = G.pRoom->nOmodel;
	for (i = 0; i < Cnt; i++) {
		pOm = &G.Ob_model[i];
		if ((pOm->Be_flg & OM_ENABLED) == 0 || (pOm->Free0 & ITEM_ACTION_80) == 0) {
			continue;
		}
		D0 = (pOm->Free0 & 0x60) >> 5;
		Svec[0].vx = 0;
		Svec[0].vy = -90;
		Svec[0].vz = 0;
		if (D0 == 0) {
			D0 = 1;
		}
		Esp_call(((D0 * 8 + 7) << 16) | 0x5002800, 0, (MATRIX *)pOm->M, &Svec[0]);
	}
}

/*
 * Sce_fade_chk  (DS 0x80053470)
 * Re-applies the room fade while Status_flg 0x80000 is set.
 */
void Sce_fade_chk(void)
{
	if ((G.Status_flg & STAT_ROOM_FADE) != 0) {
		Fade_set(0x102, -0x8000, 2, 7);
		Fade_adjust(2, 0x800, 0, NULL);
	}
}

/*
 * Stage_room_to_mapno  (DS 0x800534c4)
 * Returns the Map_flg bit index for (Stage_no, Room_no).
 * Note: for Stage_no > 6 the DS code returns an uninitialised base
 * (register v1) + Room_no; 0 is used here.
 */
/* DS 0x800534c4 (Stage_room_to_mapno) */
int Stage_room_to_mapno(int Stage_no, int Room_no)
{
	int Base;

	switch (Stage_no) {
	case 0:  Base = 0;    break;
	case 1:  Base = 0x1e; break;
	case 2:  Base = 0x3a; break;
	case 3:  Base = 0x48; break;
	case 4:  Base = 0x59; break;
	case 5:  Base = 0x63; break;
	case 6:  Base = 0x7b; break;
	default: Base = 0;    break;
	}
	return Base + Room_no;
}

/*
 * Sce_scheduler_set  (DS 0x80053528)
 * Room-init scheduler: resets work, runs the init (Scdx) and main (Scd)
 * scripts once, then runs the post-init helpers.
 */
/* DS 0x80053528 (Sce_scheduler_set) */
void Sce_scheduler_set(void)
{
	ULONG D0;

	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		D0 = Stage_room_to_mapno((int)G.SaveData.Stage_no, (int)G.SaveData.Room_no);
		if (Flg_ck(G.SaveData.Map_flg, D0) == 0) {
			Flg_on(G.SaveData.Common_flg, 0xff);
		} else {
			Flg_off(G.SaveData.Common_flg, 0xff);
		}
	}
	memcpy16((ULONG *)ScratchPad, (ULONG *)Sce_jmp_tbl, 0x400);
	Sce_rnd_set();
	Sce_work_clr();
	Sce_work_clr_set();
	Sce.Type  = 0;
	Sce.pCScd = (UCHAR *)G.pRoom->pScdx;
	Event_exec(0, 0);
	Sce.pCScd = (UCHAR *)G.pRoom->pScd;
	Event_exec(1, 0);
	Sce_scheduler_main();
	Sce_se_set();
	Sce_col_chg_init();
	Sce_mirror_init();
	Sce_kirakira_set();
}

/*
 * Sce_scheduler  (DS 0x80053644)
 * Per-frame scheduler: runs event 1 of the room script and all live tasks.
 */
/* DS 0x80053644 (Sce_scheduler) */
void Sce_scheduler(void)
{
	Sce_fade_chk();
	if ((G.Stop_flg & 0x2000000) == 0) {
		memcpy16((ULONG *)ScratchPad, (ULONG *)Sce_jmp_tbl, 0x400);
		Sce_rnd_set();
		Sce.Type  = 0;
		Sce.pCScd = (UCHAR *)G.pRoom->pScd;
		Event_exec(0, 1);
		Sce_scheduler_main();
	}
}

/*
 * Sce_scheduler_main  (DS 0x800536c4)
 * Runs tasks 0..9 through the opcode jump table copied to the ScratchPad.
 * An opcode returns 1 to continue, 2 to yield; any other value pops the
 * script return address (end of a nested if/else block) and continues.
 */
/* DS 0x800536c4 (Sce_scheduler_main) */
void Sce_scheduler_main(void)
{
	ULONG     D0;
	ULONG     Level;
	SCE_TASK *pSce;

	/* DS rebuilds pSce from the Sce base each iteration (indexed form) and keeps only
	   pSce/offset/Level in s0-s2; the continue/break layout reproduces its frame. */
	for (Level = 0; Level < 10; Level++) {
		pSce = &Sce.Sce_task[Level];
		if (pSce->Status == 0) {
			continue;
		}
		while (1) {
			D0 = ((SCE_JMP_FUNC *)ScratchPad)[*pSce->Data](pSce);
			if (D0 == 1) {
				continue;
			}
			if (D0 == 2) {
				break;
			}
			if ((signed char)pSce->Ifel_ctr[(signed char)pSce->Sub_ctr] < 0) {
				break;
			}
			pSce->pS_SP--;
			pSce->Data = (UCHAR *)*pSce->pS_SP;
			pSce->Ifel_ctr[(signed char)pSce->Sub_ctr]--;
		}
	}
	Sce_work_clr();
	Sce_work_clr_at();
}
