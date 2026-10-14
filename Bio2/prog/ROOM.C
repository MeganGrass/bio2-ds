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

/* Dummy enemy slot pointer used to fill G.pEnemy (DS 0x800d424c, the G.Em_default member; Ghidra xref from Set_room) */
#define ENEMY_DUMMY_SLOT	((ENEMY_WORK *)&G.Em_default)

/* DS string literals passed to Cd_read (no Ghidra label) */

/* Enemy routine: called as G.pEnemy[Id + 0x12](pEm) */
typedef void (*EM_ROUTINE)(ENEMY_WORK *pEm);

/* DS 0x80049e48 (Set_room) */
void Set_room(void)
{
	ULONG          i;
	ENEMY_WORK   **ppEm;
	ULONG        **pRcut;
	ULONG          ret_val;
	UINT           idx;
	UCHAR          old_pl_id;
	USHORT         saved_poison_timer;
	UCHAR          saved_poison_down;
	short          chg_player;

	G.Rbj_set_flg               = 0;
	G.Player.pOn_om             = (OBJ_MODEL_WORK *)0;
	/* Set_room clears every STAT_ bit except these (0x01104000: unnamed bits 24, 20, 14) */
	G.Status_flg                = G.Status_flg & (STAT_PLAYER | STAT_SCENARIO_B | STAT_ARRANGE_SAVED |
		STAT_SUB_PLAYER | STAT_TIMER | STAT_EASY_MODE | STAT_SPECIAL_RUN | STAT_INF_AMMO |
		STAT_DOOR_BG_CLEAR | STAT_INSPECT | 0x01104000);
	G.Player.Status_flg &= 63999;
	G.pFree_work                = G.pFree_work_head;
	G.pRoom                     = (ROOM_WORK *)G.pFree_work_head;
	Snd_bgm_set();
	ppEm                        = G.pEnemy;
	i                           = 0x21;
	G.ppEnemy_end               = G.pEnemy + 2;
	G.Enemy_num                 = 0;
	do {
		ppEm++;
		*ppEm = ENEMY_DUMMY_SLOT;
		i--;
	} while (i != 0);
	G.Em_bin_info[0].Use = 0;
	G.Em_bin_info[1].Use = 0;
	G.pObj_work_end      = G.Ob_model;
	idx = 0;
	do {
		idx++;
		G.Ob_model[0].Be_flg = 0;
	} while (idx < 0x20);

	if ((USHORT)G.Player.Id != G.SaveData.Change_player) {
		if (G.SaveData.Change_player < 0xc) {
			if ((G.SaveData.Change_player & 1U) == 0) {
				ret_val  = Flg_ck(G.SaveData.Zapping_flg, 5);
				if (ret_val != 0) {
					G.SaveData.Change_player = 8;
				}
				ret_val  = Flg_ck(G.SaveData.Zapping_flg, 0xf);
				chg_player  = 10;
			} else {
				ret_val  = Flg_ck(G.SaveData.Zapping_flg, 6);
				chg_player  = 9;
			}
			if (ret_val != 0) {
				G.SaveData.Change_player = chg_player;
			}
		}
		old_pl_id = G.Player.Id;
		G.Player.Id = (UCHAR)G.SaveData.Change_player;
		St_chenge_pl((int)G.SaveData.Change_player);
		Player_set((ENEMY_WORK *)&G.Player);
		saved_poison_down  = G.Pl_poison_down;
		saved_poison_timer  = G.Pl_poison_timer;
		G.Player.Routine_0 = 0;
		G.Player.Routine_1 = 0;
		G.Player.Routine_2 = 0;
		G.Player.Routine_3 = 0;
		if ((USHORT)G.SaveData.Change_player - 0xe < 2) {
			G.Pl_poison_down           = 0;
			G.Pl_poison_timer          = 0;
			G.SaveData.Pl_life         = G.Player.Life;
			G.SaveData.Pl_poison_down  = saved_poison_down;
			G.SaveData.Pl_poison_timer = saved_poison_timer;
			G.Player.Life              = G.Player.Max_life;
		} else if (0xb < old_pl_id) {
			G.Player.Life              = G.SaveData.Pl_life;
			G.Pl_poison_down           = G.SaveData.Pl_poison_down;
			G.Pl_poison_timer          = G.SaveData.Pl_poison_timer;
		}
		St.Pl_chenge_flg = 1;
		Snd_sys_core_set((UCHAR)G.SaveData.Change_player, 1);
	}

	UnknownSetRoomByte = 0;
	Task_sleep(1);

	if ((int)G.SaveData.Stage_no != (UINT)G.Dummy8[0]) {
		G.Dummy8[0] = (UCHAR)G.SaveData.Stage_no;
		Cd_read(((int)G.SaveData.Stage_no & 0xff) + ST1_FONT_TIM, Font_tim_buf, 1, (UCHAR *)"FONT TIM");
		G.Ctpage  = 29;
		G.Cclut   = 0;
		Mapping_tim(Font_tim_buf);
		Task_sleep(1);
		Pl_set_packet();
	}

	G.Ctpage = 5;
	G.Cclut  = 13;
	if (G.Font_type == 0) {
		ret_val = (ULONG)((USHORT *)Rdt_tbl[G.SaveData.Stage_no])[G.SaveData.Room_no];
	} else {
		ret_val = ((USHORT *)Rdt_tbl[G.SaveData.Stage_no])[G.SaveData.Room_no] + RDT_ALT_OFS;
	}
	ret_val = Cd_read(ret_val, (ULONG *)G.pRoom, 1, (UCHAR *)"RDT");

	/* relocate room-internal pointers (non-null entries) to absolute addresses */
	pRcut = &(G.pRoom)->pEdt0;
	do {
		if (*pRcut != (ULONG *)0) {
			*pRcut = (ULONG *)((int)*pRcut + (int)G.pRoom);
		}
		pRcut++;
	} while (pRcut < (ULONG **)(G.pRoom)->Cut);

	/* byte offsets in the RDT become absolute: DS adds G.pRoom itself (not 4 * G.pRoom) */
	idx = 0;
	if ((G.pRoom)->nCut != 0) {
		do {
			(G.pRoom)->pRcut[idx].pSp =
				(ULONG *)((int)(G.pRoom)->pRcut[idx].pSp + (int)G.pRoom);
			idx++;
		} while (idx < (G.pRoom)->nCut);
	}

	G.pFree_work += ret_val;

	idx = 0;
	if ((G.pRoom)->nOmodel != 0) {
		do {
			(G.pRoom)->pOmodel[idx].Tim_adr =
				(ULONG *)((int)(G.pRoom)->pOmodel[idx].Tim_adr + (int)G.pRoom);
			(G.pRoom)->pOmodel[idx].Tmd_adr =
				(ULONG *)((int)(G.pRoom)->pOmodel[idx].Tmd_adr + (int)G.pRoom);
			idx++;
		} while (idx < (G.pRoom)->nOmodel);
	}

	Cut_change((int)G.SaveData.Cut_no);
	Esp_init_R();
	Snd_sys_room_set();
	Sce_model_init();
	Snd_bgm_play_ck();

	if ((G.pRoom)->pVb0 != (ULONG *)0) {
		G.pFree_work = (UCHAR *)(G.pRoom)->pVb0;
	}

	G.pEm               = (ENEMY_WORK *)&G.Player;
	G.Player.Routine_0  = 0;
	G.Player.Routine_1  = 0;
	G.Player.Routine_2  = 0;
	G.Player.Routine_3  = 0;
	Player_move((ENEMY_WORK *)&G.Player);
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

/* DS 0x8004a3c0 (Set_stage) */
void Set_stage(void)
{
	ULONG Fsize_read;

	Fsize_read        = Task_lexecute(2, (int)G.SaveData.Stage_no + 8);
	G.pFree_work_head = Free_work_base + ((Fsize_read + 3) & 0xfffffffc);
	Task_sleep(1);
}

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
	void  *p;

	ULONG *pRbj;
	ULONG *pEnt;

	pRbj = (G.pRoom)->pRbj;
	if (pRbj != (ULONG *)0) {
		Rbj_num = pRbj[1];
		Dst_ofs = ((RBJ_ENTRY *)((int)pRbj + *pRbj))->Data_ofs;
		Keep_num = 0;
		if (Rbj_num != 0) {
			Ent_idx = 0;
			do {
				Ofs_tbl = *(G.pRoom)->pRbj;
				pEnt    = (ULONG *)((int)(G.pRoom)->pRbj + Ofs_tbl + Ent_idx);
				Ent_ofs = *pEnt;
				if (-1 < (int)G.Rbj_set_flg) {
					if (Dst_ofs != Ent_ofs) {
						Size = Ofs_tbl - Ent_ofs;
						if (Ent_idx != (Rbj_num - 1) * 8) {
							Size = pEnt[3] - Ent_ofs;
						}
						Memcpy((void *)((int)(G.pRoom)->pRbj + Dst_ofs),
							   (void *)((int)(G.pRoom)->pRbj + Ent_ofs), Size);
						pRbj = (G.pRoom)->pRbj;
						*(ULONG *)((int)pRbj + *pRbj + Keep_num * 8) =
							*(int *)((int)pRbj + *pRbj + Ent_idx) - (Ent_ofs - Dst_ofs);
						pRbj = (G.pRoom)->pRbj;
						*(ULONG *)((int)pRbj + *pRbj + Keep_num * 8 + 4) =
							*(int *)((int)pRbj + *pRbj + Ent_idx + 4) - (Ent_ofs - Dst_ofs);
					}
					if (Ent_idx == (Rbj_num - 1) * 8) {
						Adj = *(G.pRoom)->pRbj - Ent_ofs;
					} else {
						Adj = ((RBJ_ENTRY *)((int)(G.pRoom)->pRbj + *(G.pRoom)->pRbj + Ent_idx))[1].Data_ofs - Ent_ofs;
					}
					Dst_ofs += Adj;
					Keep_num++;
				}
				Ent_idx += 8;
				G.Rbj_set_flg <<= 1;
			} while (Ent_idx < Rbj_num << 3);
		}
		pRbj = (G.pRoom)->pRbj;
		Memcpy((void *)((int)pRbj + Dst_ofs), (void *)((int)pRbj + *pRbj), Keep_num * 8);
		*(G.pRoom)->pRbj   = Dst_ofs;
		(G.pRoom)->pRbj[1] = Keep_num;
		pEnt = (G.pRoom)->pEdt0;
		p    = (void *)((int)(G.pRoom)->pRbj + Dst_ofs + Keep_num * 8);
		Adj  = (int)pEnt - (int)p;
		Memcpy(p, pEnt, (int)(G.pRoom)->pVb0 - (int)pEnt);
		(G.pRoom)->pEdt0 = (ULONG *)((int)(G.pRoom)->pEdt0 - Adj);
		(G.pRoom)->pVh0  = (ULONG *)((int)(G.pRoom)->pVh0  - Adj);
		G.pFree_work     = (UCHAR *)((int)(G.pRoom)->pVb0  - Adj);
		Snd_sys_pset();
	}
}

/* DS 0x8004a694 (Em_init_move) */
void Em_init_move(void)
{
	BOOL        Stop_save;
	USHORT      Type_save;
	short       Timer0_save;
	UINT        Routine;
	ENEMY_WORK *pEm;
	ENEMY_WORK **ppEm;
	ENEMY_WORK **ppEm_cur;

	ppEm_cur  = &G.pEm;
	Stop_save = (G.Stop_flg & 0x20000000) != 0;
	if (Stop_save) {
		G.Stop_flg &= 0xdfffffff;
	}

	ppEm = G.pEnemy + 1;
	if (G.ppEnemy_end != G.pEnemy + 1) {
		do {
			pEm    = *ppEm;
			*ppEm_cur = pEm;
			if ((pEm->Be_flg & 1) != 0) {
				Type_save    = pEm->Type;
				pEm->Type    = Type_save & 0x7fff;
				Timer0_save  = pEm->Timer0;
				((EM_ROUTINE)G.pEnemy[pEm->Id + 0x12])(pEm);
				if ((int)Timer0_save != 0) {
					Routine        = (((int)Timer0_save & 0xff00U) - 0x100) | 4;
					pEm->Routine_0 = (char)Routine;
					pEm->Routine_1 = (char)(Routine >> 8);
					pEm->Routine_2 = (char)(Routine >> 0x10);
					pEm->Routine_3 = (char)(Routine >> 0x18);
					pEm->Move_no   = (UCHAR)Timer0_save;
					pEm->Sce_flg   = pEm->Timer1;
					((EM_ROUTINE)G.pEnemy[pEm->Id + 0x12])(pEm);
				}
				pEm->Type |= (Type_save & 0x8000);
			}
			ppEm++;
		} while (ppEm != G.ppEnemy_end);
	}

	if (Stop_save) {
		G.Stop_flg |= 0x20000000;
	}
}

/* DS 0x800dfad7 (UnknownSetRoomByte) */
UCHAR UnknownSetRoomByte = 0;
