/*
 * ROOM.C - Room and stage loading (RDT) module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ROOM.C
 */

#include "TYPES.H"
#include "GAME.H"
#include <STRINGS.H>
#include "ROOM.H"
#include "CD.H"
#include "EM_SET.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MONITOR.H"
#include "OMA.H"
#include "PLAYER.H"
#include "PSP.H"
#include "SCE0.H"
#include "SND.H"
#include "STATUS.H"
#include "SUB.H"
#include "DOOR.H"
#include "GAME.H"
#include "SCE1.H"
#include "FILECODE.H"
#include "flag.h"

/* RDT list index: +125 selects the alternate block (Font_type != 0) */
#define RDT_ALT_OFS	0x7d


/* DS 0x800A70FC (Rdt_tbl1): ROOMxxxx_RDT names (FILECODE.H), room list 1 */
USHORT Rdt_tbl1[30] = {
	ROOM1000_RDT, ROOM1010_RDT, ROOM1020_RDT, ROOM1030_RDT,
	ROOM1040_RDT, ROOM1050_RDT, ROOM1060_RDT, ROOM1070_RDT,
	ROOM1080_RDT, ROOM1090_RDT, ROOM10A0_RDT, ROOM10B0_RDT,
	ROOM10C0_RDT, ROOM10D0_RDT, ROOM10E0_RDT, ROOM10F0_RDT,
	ROOM1100_RDT, ROOM1110_RDT, ROOM1120_RDT, ROOM1130_RDT,
	ROOM1140_RDT, ROOM1150_RDT, ROOM1160_RDT, ROOM1170_RDT,
	ROOM1180_RDT, ROOM1190_RDT, ROOM11A0_RDT, ROOM11B0_RDT,
	ROOM11C0_RDT, ROOM11D0_RDT,
};

/* DS 0x800A7138 (Rdt_tbl2): ROOMxxxx_RDT names (FILECODE.H), room list 2 */
USHORT Rdt_tbl2[28] = {
	ROOM2000_RDT, ROOM2010_RDT, ROOM2020_RDT, ROOM2030_RDT,
	ROOM2040_RDT, ROOM2050_RDT, ROOM2060_RDT, ROOM2070_RDT,
	ROOM2080_RDT, ROOM2090_RDT, ROOM20A0_RDT, ROOM20B0_RDT,
	ROOM20C0_RDT, ROOM20D0_RDT, ROOM20E0_RDT, ROOM20F0_RDT,
	ROOM2100_RDT, ROOM2110_RDT, ROOM2120_RDT, ROOM2130_RDT,
	ROOM2140_RDT, ROOM2150_RDT, ROOM2160_RDT, CONFIG_BIN,
	CONFIG_BIN, ROOM2190_RDT, ROOM21A0_RDT, ROOM21B0_RDT,
};

/* DS 0x800A7170 (Rdt_tbl3): ROOMxxxx_RDT names (FILECODE.H), room list 3 */
USHORT Rdt_tbl3[16] = {
	ROOM3000_RDT, ROOM3010_RDT, ROOM3020_RDT, ROOM3030_RDT,
	ROOM3040_RDT, ROOM3050_RDT, ROOM3060_RDT, ROOM3070_RDT,
	ROOM3080_RDT, ROOM3090_RDT, ROOM30A0_RDT, ROOM30B0_RDT,
	ROOM30C0_RDT, ROOM30D0_RDT, CONFIG_BIN, CONFIG_BIN,
};

/* DS 0x800A7190 (Rdt_tbl4): ROOMxxxx_RDT names (FILECODE.H), room list 4 */
USHORT Rdt_tbl4[20] = {
	ROOM4000_RDT, ROOM4010_RDT, ROOM4020_RDT, ROOM4030_RDT,
	ROOM4040_RDT, ROOM4050_RDT, CONFIG_BIN, ROOM4070_RDT,
	ROOM4080_RDT, ROOM4090_RDT, ROOM40A0_RDT, ROOM40B0_RDT,
	ROOM40C0_RDT, ROOM40D0_RDT, ROOM40E0_RDT, ROOM40F0_RDT,
	ROOM4100_RDT, CONFIG_BIN, CONFIG_BIN, CONFIG_BIN,
};

/* DS 0x800A71B8 (Rdt_tbl5): ROOMxxxx_RDT names (FILECODE.H), room list 5 */
USHORT Rdt_tbl5[12] = {
	ROOM5000_RDT, ROOM5010_RDT, ROOM5020_RDT, ROOM5030_RDT,
	ROOM5040_RDT, ROOM5050_RDT, ROOM5060_RDT, ROOM5070_RDT,
	ROOM5080_RDT, ROOM5090_RDT, CONFIG_BIN, CONFIG_BIN,
};

/* DS 0x800A71D0 (Rdt_tbl6): ROOMxxxx_RDT names (FILECODE.H), room list 6 */
USHORT Rdt_tbl6[24] = {
	ROOM6000_RDT, ROOM6010_RDT, ROOM6020_RDT, ROOM6030_RDT,
	ROOM6040_RDT, ROOM6050_RDT, ROOM6060_RDT, ROOM6070_RDT,
	ROOM6080_RDT, ROOM6090_RDT, ROOM60A0_RDT, ROOM60B0_RDT,
	ROOM60C0_RDT, ROOM60D0_RDT, ROOM60E0_RDT, ROOM60F0_RDT,
	ROOM6100_RDT, ROOM6110_RDT, ROOM6120_RDT, ROOM6130_RDT,
	ROOM6140_RDT, ROOM6150_RDT, ROOM6160_RDT, ROOM6170_RDT,
};

/* DS 0x800A7200 (Rdt_tbl7): ROOMxxxx_RDT names (FILECODE.H), room list 7 */
USHORT Rdt_tbl7[8] = {
	ROOM7000_RDT, ROOM7010_RDT, ROOM7020_RDT, ROOM7030_RDT,
	ROOM7040_RDT, CONFIG_BIN, CONFIG_BIN, CONFIG_BIN,
};

/* DS 0x800a7210 (Rdt_tbl): RDT table pointers, indexed by stage */
ULONG *Rdt_tbl[7] = {
	(ULONG *)Rdt_tbl1,
	(ULONG *)Rdt_tbl2,
	(ULONG *)Rdt_tbl3,
	(ULONG *)Rdt_tbl4,
	(ULONG *)Rdt_tbl5,
	(ULONG *)Rdt_tbl6,
	(ULONG *)Rdt_tbl7,
};

/* DS string literals passed to Cd_read */

/* Enemy routine: called as G.pEnemy[Id + 0x12](pEm) */
typedef void (*EM_ROUTINE)(ENEMY_WORK *pEm);

/* DS stack frame: 0x40 bytes; s0..s2 saved at sp+0x30..0x38, ra saved at sp+0x3c. No local variable slots are
 * used. */
/* DS 0x80049e48 (Set_room) */
void Set_room(void)
{
	/* retail reserves 8 more bytes of locals (Set_room frame 64); never read or written */
	volatile int Rsv[2];
	tagGLOBAL      *pG = &G;
	PLAYER_WORK    *pPl = &pG->Player;
	ULONG          i;
	ENEMY_WORK   **ppEm;
	ULONG        **pRcut;
	ULONG          ret_val;
	LONG           stage_no;
	UINT           idx;
	UCHAR          old_pl_id;

	G.Rbj_set_flg               = 0;
	G.Player.pOn_om             = (OBJ_MODEL_WORK *)0;
	/* Set_room clears every STAT_ bit except these (0x01104000: unnamed bits 24, 20, 14) */
	G.Status_flg                = G.Status_flg & (STAT_PLAYER | STAT_SCENARIO_B | STAT_ARRANGE_SAVED | STAT_SUB_PLAYER | STAT_TIMER | STAT_EASY_MODE | STAT_SPECIAL_RUN | STAT_INF_AMMO | STAT_DOOR_BG_CLEAR | STAT_INSPECT | 0x01104000);
	G.Player.Status_flg &= 63999;
	G.pFree_work                = G.pFree_work_head;
	G.pRoom                     = (ROOM_WORK *)G.pFree_work_head;
	Snd_bgm_set();
	ppEm                        = pG->pEnemy + 1;
	i                           = 0x21;
	pG->ppEnemy_end               = pG->pEnemy + 2;
	pG->Enemy_num                 = 0;
	do {
		/* every slot starts as the dummy enemy G.Em_default (DS 0x800d424c) */
		*ppEm = (ENEMY_WORK *)&pG->Em_default;
		ppEm++;
		i--;
	} while (i != 0);
	pG->Em_bin_info[0].Use = 0;
	pG->Em_bin_info[1].Use = 0;
	pG->pObj_work_end      = pG->Ob_model;
	idx = 0;
	do {
		idx++;
		pG->Ob_model[0].Be_flg = 0;
	} while (idx < 0x20);

	if ((USHORT)pPl->Id != pG->SaveData.Change_player) {
		if (pG->SaveData.Change_player < 0xc) {
			if ((pG->SaveData.Change_player & 1U) != 0) {
				if (Flg_ck(pG->SaveData.Zapping_flg, 6) != 0) {
					pG->SaveData.Change_player = 9;
				}
			} else {
				if (Flg_ck(pG->SaveData.Zapping_flg, 5) != 0) {
					pG->SaveData.Change_player = 8;
				}
				if (Flg_ck(pG->SaveData.Zapping_flg, 0xf) != 0) {
					pG->SaveData.Change_player = 10;
				}
			}
		}
		old_pl_id = pPl->Id;
		pPl->Id = (UCHAR)pG->SaveData.Change_player;
		St_chenge_pl((int)pG->SaveData.Change_player);
		Player_set((ENEMY_WORK *)pPl);
		*(long *)&pPl->Routine_0 = 0;
		if ((UINT)((USHORT)pG->SaveData.Change_player - 0xe) < 2) {
			pG->SaveData.Pl_life         = pG->Player.Life;
			pG->SaveData.Pl_poison_down  = pG->Pl_poison_down;
			pG->SaveData.Pl_poison_timer = pG->Pl_poison_timer;
			pG->Pl_poison_down           = 0;
			pG->Pl_poison_timer          = 0;
			pG->Player.Life              = pG->Player.Max_life;
		} else if (0xb < old_pl_id) {
			pG->Player.Life              = pG->SaveData.Pl_life;
			pG->Pl_poison_down           = pG->SaveData.Pl_poison_down;
			pG->Pl_poison_timer          = pG->SaveData.Pl_poison_timer;
		}
		St.Pl_chenge_flg = 1;
		Snd_sys_core_set((UCHAR)pG->SaveData.Change_player, 1);
	}

	UnknownSetRoomByte = 0;
	Task_sleep(1);

	stage_no = pG->SaveData.Stage_no;
	if (stage_no != (UINT)pG->Dummy8[0]) {
		pG->Dummy8[0] = (UCHAR)stage_no;
		Cd_read((stage_no & 0xff) + ST1_FONT_TIM, Font_tim_buf, 1, (UCHAR *)"FONT TIM");
		/* retail stores Ctpage (low byte) and Cclut (high byte) as one halfword */
		*(USHORT *)&G.Ctpage = 0x001d;
		Mapping_tim(Font_tim_buf);
		Task_sleep(1);
		Pl_set_packet();
	}

	*(USHORT *)&G.Ctpage = 0x0d05;
	if (pG->Font_type != 0) {
		ret_val = Cd_read((ULONG)((USHORT *)Rdt_tbl[pG->SaveData.Stage_no])[pG->SaveData.Room_no] + RDT_ALT_OFS, (ULONG *)pG->pRoom, 1, (UCHAR *)"RDT");
	} else {
		ret_val = Cd_read((ULONG)((USHORT *)Rdt_tbl[pG->SaveData.Stage_no])[pG->SaveData.Room_no], (ULONG *)pG->pRoom, 1, (UCHAR *)"RDT");
	}

	/* relocate room-internal pointers (non-null entries) to absolute addresses */
	pRcut = &(pG->pRoom)->pEdt0;
	do {
		if (*pRcut != (ULONG *)0) {
			*pRcut = (ULONG *)((int)*pRcut + (int)pG->pRoom);
		}
		pRcut++;
	} while (pRcut < (ULONG **)(pG->pRoom)->Cut);

	/* byte offsets in the RDT become absolute: DS adds pG->pRoom itself (not 4 * pG->pRoom) */
	idx = 0;
	while (idx < (pG->pRoom)->nCut) {
		(pG->pRoom)->pRcut[idx].pSp =
			(ULONG *)((int)(pG->pRoom)->pRcut[idx].pSp + (int)pG->pRoom);
		idx++;
	}

	pG->pFree_work += ret_val;

	idx = 0;
	while (idx < (pG->pRoom)->nOmodel) {
		(pG->pRoom)->pOmodel[idx].Tim_adr =
			(ULONG *)((int)(pG->pRoom)->pOmodel[idx].Tim_adr + (int)pG->pRoom);
		(pG->pRoom)->pOmodel[idx].Tmd_adr =
			(ULONG *)((int)(pG->pRoom)->pOmodel[idx].Tmd_adr + (int)pG->pRoom);
		idx++;
	}

	Cut_change((int)pG->SaveData.Cut_no);
	Esp_init_R();
	Snd_sys_room_set();
	Sce_model_init();
	Snd_bgm_play_ck();

	if ((pG->pRoom)->pVb0 != (ULONG *)0) {
		pG->pFree_work = (UCHAR *)(pG->pRoom)->pVb0;
	}

	pG->pEm               = (ENEMY_WORK *)pPl;
	*(long *)&pG->Player.Routine_0 = 0;
	Player_move((ENEMY_WORK *)pPl);
	Sce_scheduler_set();
	Snd_sys_enem_set();
	Rbj_set();
	Psp_init0();
	Em_set();
	Em_init_move();
	Psp_init1();

	while (UnknownSetRoomByte != 0) {
		Task_sleep(1);
	}
	St_room_set();
}

/* DS stack frame: 0x18 bytes; ra saved at sp+0x10. No local variable slots are used. */
/* DS 0x8004a3c0 (Set_stage) */
void Set_stage(void)
{
	ULONG Fsize_read;

	Fsize_read        = Task_lexecute(2, (int)G.SaveData.Stage_no + 8);
	G.pFree_work_head = Free_work_base + ((Fsize_read + 3) & 0xfffffffc);
	Task_sleep(1);
}

/* DS stack frame: 0x38 bytes; s0..s6 saved at sp+0x18..0x30, ra saved at sp+0x34. No local variable slots are
 * used. */
/* DS 0x8004a414 (Replace_rdt) */
void Replace_rdt(void)
{
	ULONG  Ofs_tbl;		/* offset of the entry table inside the RBJ block (*pRbj) */
	ULONG  Rbj_num;		/* number of RBJ entries (pRbj[1]) */
	ULONG  Keep_num;	/* entries kept after the flag filter */
	ULONG  Dst_ofs;		/* running destination offset for kept data */
	ULONG  Ent_ofs;		/* start offset of the entry being examined */
	ULONG  Ent_idx;		/* UCHAR index into the entry table */
	long   Size;
	int    Adj;
	int    Diff;
	void  *p;

	ULONG *pRbj;
	ULONG *pEnt;
	tagGLOBAL *pG;

	pG = &G;
	pRbj = (pG->pRoom)->pRbj;
	if (pRbj != (ULONG *)0) {
		Rbj_num = pRbj[1];
		Dst_ofs = ((RBJ_ENTRY *)((int)pRbj + *pRbj))->Data_ofs;
		Keep_num = 0;
		if (Rbj_num != 0) {
			Ent_idx = 0;
			do {
				Ofs_tbl = *(pG->pRoom)->pRbj;
				pEnt    = (ULONG *)((int)(pG->pRoom)->pRbj + Ofs_tbl + Ent_idx);
				Ent_ofs = *pEnt;
				if (-1 < (int)pG->Rbj_set_flg) {
					if (Dst_ofs != Ent_ofs) {
						Size = Ofs_tbl - Ent_ofs;
						if (Ent_idx != (Rbj_num - 1) * 8) {
							Size = pEnt[3] - Ent_ofs;
						}
						Memcpy((void *)((int)(pG->pRoom)->pRbj + Dst_ofs), (void *)((int)(pG->pRoom)->pRbj + Ent_ofs), Size);
						pEnt = (pG->pRoom)->pRbj;
						Diff = Ent_ofs - Dst_ofs;
						*(ULONG *)((int)pEnt + Keep_num * 8 + *pEnt) =
							*(int *)((int)pEnt + Ent_idx + *pEnt) - Diff;
						pEnt = (pG->pRoom)->pRbj;
						*(ULONG *)((int)pEnt + Keep_num * 8 + *pEnt + 4) =
							*(int *)((int)pEnt + Ent_idx + *pEnt + 4) - Diff;
					}
					if (Ent_idx != (Rbj_num - 1) * 8) {
						Adj = ((ULONG *)((int)(pG->pRoom)->pRbj + Ent_idx + *(pG->pRoom)->pRbj))[2] - Ent_ofs;
					} else {
						Adj = *(pG->pRoom)->pRbj - Ent_ofs;
					}
					Dst_ofs += Adj;
					Keep_num++;
				}
				Ent_idx += 8;
				pG->Rbj_set_flg <<= 1;
			} while (Ent_idx < Rbj_num << 3);
		}
		pEnt = (pG->pRoom)->pRbj;
		Memcpy((void *)((int)pEnt + Dst_ofs), (void *)((int)pEnt + *pEnt), Keep_num * 8);
		*(pG->pRoom)->pRbj   = Dst_ofs;
		(pG->pRoom)->pRbj[1] = Keep_num;
		pEnt = (pG->pRoom)->pEdt0;
		p    = (void *)((int)(pG->pRoom)->pRbj + Dst_ofs + Keep_num * 8);
		Adj  = (int)pEnt - (int)p;
		Memcpy(p, pEnt, (int)(pG->pRoom)->pVb0 - (int)pEnt);
		(pG->pRoom)->pEdt0 = (ULONG *)((int)(pG->pRoom)->pEdt0 - Adj);
		(pG->pRoom)->pVh0  = (ULONG *)((int)(pG->pRoom)->pVh0  - Adj);
		pG->pFree_work     = (UCHAR *)((int)(pG->pRoom)->pVb0  - Adj);
		Snd_sys_pset();
	}
}

/* DS stack frame: 0x30 bytes; s0..s6 saved at sp+0x10..0x28, ra saved at sp+0x2c. No local variable slots are
 * used. */
/* DS 0x8004a694 (Em_init_move) */
void Em_init_move(void)
{
	BOOL        Stop_save;
	USHORT      Type_save;
	USHORT      Type_hi;
	LONG        Timer0_save;
	UINT        Routine;
	ENEMY_WORK *pEm;
	ENEMY_WORK **ppEm;
	ENEMY_WORK **ppEm_cur;

	if ((G.Stop_flg & 0x20000000) != 0) {
		G.Stop_flg &= 0xdfffffff;
		Stop_save = 1;
	} else {
		Stop_save = 0;
	}

	ppEm = G.pEnemy + 1;
	if (G.ppEnemy_end != G.pEnemy + 1) {
		ppEm_cur = &G.pEm;
		do {
			pEm    = *ppEm;
			*ppEm_cur = pEm;
			if ((pEm->Be_flg & 1) != 0) {
				Type_save    = pEm->Type;
				pEm->Type    = Type_save & 0x7fff;
				Type_hi      = Type_save & 0x8000;
				Timer0_save  = pEm->Timer0;
				((EM_ROUTINE)G.pEnemy[pEm->Id + 0x12])(pEm);
				if ((int)Timer0_save != 0) {
					Routine        = (((int)Timer0_save & 0xff00U) - 0x100) | 4;
					*(UINT *)&pEm->Routine_0 = Routine;		/* Routine_0 to Routine_3 are the four bytes of Routine (DS: one sw at +0x4) */
					pEm->Move_no   = (UCHAR)Timer0_save;
					pEm->Sce_flg   = pEm->Timer1;
					((EM_ROUTINE)G.pEnemy[pEm->Id + 0x12])(pEm);
				}
				pEm->Type |= Type_hi;
			}
			ppEm++;
		} while (ppEm != G.ppEnemy_end);
	}

	if (Stop_save) {
		G.Stop_flg |= 0x20000000;
	}
}

/* DS 0x800dfad7 (UnknownSetRoomByte) */
UCHAR UnknownSetRoomByte;
