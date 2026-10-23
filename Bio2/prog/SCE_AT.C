/*
 * SCE_AT.C - AOT (at-trigger) event handlers: doors, items, saves
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE_AT.C
 */
#include "TYPES.H"
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <STRINGS.H>
#include "GAME.H"
#include "CD.H"
#include "ROOM.H"
#include "SCE_AT.H"
#include "DOOR.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "OMA.H"
#include "PAD.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "SUB.H"
#include "GAME.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCE2.H"
#include "flag.h"
#include "sce_defines.h"

/* DS 0x800a73c4 -- AOT handler table, indexed by SCE_AOT.Id */
SCE_AT_FUNC   Sce_at_jump_tbl[15] = {
	Nothing,        /* 0x00 SCE_NOTHING */
	Sce_Door,       /* 0x01 SCE_DOOR */
	Sce_Item,       /* 0x02 SCE_ITEM */
	Sce_Normal,     /* 0x03 SCE_NORMAL */
	Sce_Message,    /* 0x04 SCE_MESSAGE */
	Sce_Event,      /* 0x05 SCE_EVENT */
	Sce_Flg_chg,    /* 0x06 SCE_FLAG_CHG */
	Sce_Water,      /* 0x07 SCE_WATER */
	Sce_Move,       /* 0x08 SCE_MOVE */
	Sce_Save,       /* 0x09 SCE_SAVE */
	Sce_Itembox,    /* 0x0a SCE_ITEMBOX */
	Sce_Damage,     /* 0x0b SCE_DAMAGE */
	Sce_Status,     /* 0x0c SCE_STATUS */
	Sce_Hikidashi,  /* 0x0d SCE_HIKIDASHI */
	Sce_Windows,    /* 0x0e SCE_WINDOWS */
};

/*
 * AOT ("at" trigger) data layout used by this file (LOG types):
 *   SCE_AOT  : Id, Type, nFloor, Super, X, Z, W, D, Data[]   (header 0x0c)
 *   4-point AOTs (Type & SCE_POINT4) carry X1,Z1,X2,Z2 after D, so their data
 *   starts at +0x14 instead of +0x0c.
 * The AOT handlers receive a pointer to the data part.
 */

/* DS 0x800ead94 */
SCE_AOT      *pCAot;
/* DS 0x800eade0 */
ULONG         Old_Fatari;
/* DS 0x800eae30 */
ULONG         Old_Uatari;

/*
 * Sce_at_check  (DS 0x80051088)
 * Tests every registered AOT whose Type matches attribute against pEm.
 * Type SCE_UNDER: AOT tested at pEm's position  -> sets U_atari.
 * Type SCE_FRONT: AOT tested 620 units in front -> sets F_atari.
 * auto_flg is SCE_MANUAL (action button) or 0, compared with Type & SCE_MANUAL.
 * auto_flg == 0 : all hit handlers are called, returns 0.
 * auto_flg != 0 : the first hit with Id != 0 is called and its result
 *                 is returned.
 */
/* DS 0x80051088 (Sce_at_check) */
ULONG Sce_at_check(ENEMY_WORK *pEm, ULONG attribute, ULONG auto_flg)
{
	/* retail reserves 16 more bytes of locals (Sce_at_check frame 120); never read */
	volatile int Rsv[4];
	SVECTOR      Svec;
	VECTOR       Vec;
	ULONG        Hit_flg;
	ULONG        i;
	ULONG        D0;
	short        Work[10];   /* [0..1] base x/z, [2..9] X,Z,W,D,X1,Z1,X2,Z2 */
	SCE_AOT     *pAot;
	SCE_AOT    **ppAot;
	short        Aot_no;
	USHORT      *pData;

	Svec.vx = 0x26c;
	Svec.vz = 0;
	Rot_vector(pEm->Cdir_y, &Svec, &Svec);
	Vec.vx = Svec.vx + pEm->Pos_x;
	Vec.vz = Svec.vz + pEm->Pos_z;
	G.pEm  = pEm;

	D0 = G.nSce_aot;
	if (D0 == 0) {
		return 0;
	}
	i      = 0;
	Aot_no = 0;
	ppAot  = G.Sce_aot;
	do {
		pAot  = *ppAot++;
		pCAot = pAot;
		if (pAot == NULL) {
			goto next;
		}
		Old_Fatari = G.SaveData.F_atari;
		Old_Uatari = G.SaveData.U_atari;
		i++;
		if ((pAot->Type & attribute) == 0) {
			goto next;
		}
		if ((pAot->Type & SCE_MANUAL) != auto_flg) {
			goto next;
		}
		if ((pAot->nFloor & 0x80) == 0 && pEm->nFloor != pAot->nFloor) {
			goto next;
		}

		/* base position (AOT attached to an object model or an enemy) */
		if ((pAot->Super & 0x80) == 0) {
			Work[0] = 0;
			Work[1] = 0;
		} else if ((pAot->Super & 0x40) == 0) {
			Work[0] = (short)G.Ob_model[pAot->Super & 0x3f].Pos_x;
			Work[1] = (short)G.Ob_model[pAot->Super & 0x3f].Pos_z;
		} else {
			Work[0] = (short)G.pEnemy[pAot->Super & 0x3f]->Pos_x;
			Work[1] = (short)G.pEnemy[pAot->Super & 0x3f]->Pos_z;
		}

		pData = (USHORT *)&pAot->X;
		if ((pAot->Type & SCE_POINT4) != 0) {
			/* 4-point area: every point is relative to the base */
			Work[2] = Work[0] + pData[0];
			Work[3] = Work[1] + pData[1];
			Work[4] = Work[0] + pData[2];
			Work[5] = Work[1] + pData[3];
			Work[6] = Work[0] + pData[4];
			Work[7] = Work[1] + pData[5];
			Work[8] = Work[0] + pData[6];
			Work[9] = Work[1] + pData[7];
		} else {
			/* box: only X/Z are relative, W/D are sizes */
			Work[2] = Work[0] + pData[0];
			Work[3] = Work[1] + pData[1];
			Work[4] = pData[2];
			Work[5] = pData[3];
		}

		if ((pAot->Type & SCE_UNDER) != 0) {
			if ((pAot->Type & SCE_POINT4) != 0) {
				Hit_flg = Hit_ck_point4((VECTOR *)&pEm->Pos_x, (VCUT *)Work);
			} else {
				Hit_flg = Hit_ck_box((VECTOR *)&pEm->Pos_x, (BOX *)&Work[2]);
			}
			if (Hit_flg != 0) {
				G.SaveData.U_atari = Aot_no;
				goto hit;
			}
		}
		if ((pAot->Type & SCE_FRONT) == 0) {
			goto next;
		}
		if ((pAot->Type & SCE_POINT4) != 0) {
			Hit_flg = Hit_ck_point4(&Vec, (VCUT *)Work);
		} else {
			Hit_flg = Hit_ck_box(&Vec, (BOX *)&Work[2]);
		}
		if (Hit_flg == 0) {
			goto next;
		}
		G.SaveData.F_atari = Aot_no;

hit:
		if (auto_flg != 0) {
			if (pAot->Id == SCE_NOTHING) {
				goto next;
			}
			pEm->At_sce_no = (UCHAR)Aot_no;
			if ((pAot->Type & SCE_POINT4) != 0) {
				return Sce_at_jump_tbl[pAot->Id](&((SCE_AOT *)&pAot->Data)->W);
			}
			return Sce_at_jump_tbl[pAot->Id](&pAot->Data);
		}
		pEm->At_sce_no = (UCHAR)Aot_no;
		if ((pAot->Type & SCE_POINT4) != 0) {
			Sce_at_jump_tbl[pAot->Id](&((SCE_AOT *)&pAot->Data)->W);
		} else {
			Sce_at_jump_tbl[pAot->Id](&pAot->Data);
		}
next:
		Aot_no++;
	} while (i < D0);
	return 0;
}

/*
 * Nothing  (DS 0x800514ec)
 * AOT 0x00: undoes the atari values set by the hit test.
 */
/* DS 0x800514ec (Nothing) */
ULONG Nothing(void *pAot)
{
	G.SaveData.F_atari = (short)Old_Fatari;
	G.SaveData.U_atari = (short)Old_Uatari;
	return 0;
}

/*
 * Sce_Door  (DS 0x80051514)
 * AOT 0x01: door.  Starts the door demo if the door is unlocked, otherwise
 * tries to unlock it with the required key.
 */
/* DS 0x80051514 (Sce_Door) */
ULONG Sce_Door(void *pAot)
{
	IN_DOOR_WORK *pDoor;
	ULONG         Key_type;
	long          Key_work;
	ULONG         Mess_no;

	pDoor = (IN_DOOR_WORK *)pAot;
	if (G.Player.pOn_om != NULL) {
		return 0;
	}
	if ((G.Status_flg & (STAT_PLAYER | STAT_SUB_PLAYER)) == (STAT_PLAYER | STAT_SUB_PLAYER) && (G.pEnemy[1]->Be_flg & 1) != 0 && (G.pEnemy[1]->Free[2] & 0x2000) != 0) {
		Mess_no = 8;
	} else {
		if ((pDoor->Key_id & 0x80) == 0 || Flg_ck(G.SaveData.Key_flg, pDoor->Key_id & 0x3f) != 0) {
			/* unlocked: enter */
			G.pSce_aot_on     = pDoor;
			G.Stop_flg       |= 0xff000000;
			Game.Doordemo_flg = 1;
			return 0;
		}
		Key_type = pDoor->Key_type;
		if (Key_type == DOOR_KEY_TYPE_UNLOCK) {
			/* unlocked from this side */
			Mess_set(0, 0x100, 10, 0xff000000);
			Snd_se_stad(0x2260000, NULL);
			goto door_in;
		}
		if (Key_type == DOOR_KEY_TYPE_LOCKED) {
			Snd_se_stad(0x2160000, NULL);
			Mess_no = 11;
		} else {
			Key_work = Search_item_id(Key_type);
			if (Key_work >= 0) {
				/* key in inventory: use it */
				Moji.Mess_item = (UCHAR)Key_type;
				Mess_set(0, 0x100, 5, 0xff000000);
				Snd_se_stad(0x2250000, NULL);
				G.Front_func     = Key_lost;
				G.Front_func_rtn = (UCHAR)(Key_work + 1);
door_in:
				Flg_on(G.SaveData.Key_flg, pDoor->Key_id & 0x3f);
				return 0;
			}
			Snd_se_stad(0x2160000, NULL);
			Mess_no = Key_type - 0x4c;
		}
	}
	Mess_set(0, 0x100, Mess_no, 0xff000000);
	return 0;
}

/*
 * Key_lost  (DS 0x80051718)
 * Front function: decrements the used key and, when it runs out, asks
 * whether to discard it.
 */
/* DS 0x80051718 (Key_lost) */
void Key_lost(void)
{
	/* DS 0x800c3b98 */
	static long Key_work;
	tagGLOBAL *pG = &G;

	if (pG->Front_func_rtn == 0) {
		if ((Moji.Mess_flg & 0x80) == 0) {
			if ((Moji.Mess_flg & 1) == 0) {
				pG->SaveData.Item_work[Key_work].Id   = 0;
				pG->SaveData.Item_work[Key_work].Size = 0;
				Sort_item();
			} else {
				pG->SaveData.Item_work[Key_work].Num = 1;
			}
			pG->Front_func = NULL;
		}
	} else {
		Key_work = pG->Front_func_rtn - 1;
		if ((Moji.Mess_flg & 0x80) == 0) {
			if (--pG->SaveData.Item_work[Key_work].Num == 0) {
				Mess_set(0, 0x100, 9, 0xff000000);
				pG->Stop_flg |= 0xff000000;
			} else {
				pG->Front_func = NULL;
			}
			pG->Front_func_rtn = 0;
		}
	}
}

/*
 * Sce_Item  (DS 0x80051884)
 * AOT 0x02: item pickup.  Returns 1 if the player picks it up with an
 * animation (Free bit 0), 0 if the status screen is opened directly.
 */
/* DS 0x80051884 (Sce_Item) */
ULONG Sce_Item(void *pAot)
{
	SCE_ITEM_DATA *pItem;

	pItem          = (SCE_ITEM_DATA *)pAot;
	G.Get_item_id  = (UCHAR)pItem->Data0;
	G.pAot_data    = pCAot;
	Moji.Mess_item = G.Get_item_id;
	if ((G.System_flg & 0x40) != 0 && G.Get_item_id == 0x1e) {
		pItem->Data1 = 2;
	}
	if ((pItem->Free & ITEM_ACTION_KNEEL) != 0) {
		G.Player.Routine_1 = 6;
		G.Player.Routine_2 = 0;
		return 1;
	}
	St.St_type        = 2;
	Game.Doordemo_flg = 1;
	G.Status_flg     |= STAT_STATUS_SCREEN;
	Game.Stop_bak     = G.Stop_flg;
	return 0;
}

/*
 * Sce_Normal  (DS 0x80051940)
 * AOT 0x03: no action.
 */
/* DS 0x80051940 (Sce_Normal) */
ULONG Sce_Normal(void *pAot)
{
	return 0;
}

/*
 * Sce_Message  (DS 0x80051948)
 * AOT 0x04: Data0 = message no, Data1 = attribute (+0x300), Data2 = mode.
 */
/* DS 0x80051948 (Sce_Message) */
ULONG Sce_Message(void *pAot)
{
	USHORT *pDat;

	pDat = (USHORT *)pAot;
	Mess_set(0, pDat[1] + 0x300, pDat[0], (ULONG)pDat[2] << 16);
	return 0;
}

/*
 * Sce_Event  (DS 0x80051980)
 * AOT 0x05: starts script event (byte 3) on task level Data0.
 */
/* DS 0x80051980 (Sce_Event) */
ULONG Sce_Event(void *pAot)
{
	if ((G.Stop_flg & 0x2000000) == 0) {
		Event_exec(((USHORT *)pAot)[0], ((UCHAR *)pAot)[3]);
	}
	return 0;
}

/*
 * Sce_Flg_chg  (DS 0x800519c0)
 * AOT 0x06: Data0 = flag table, Data1 = bit number, Data2 = on/off.
 */
/* DS 0x800519c0 (Sce_Flg_chg) */
ULONG Sce_Flg_chg(void *pAot)
{
	USHORT *pDat;
	ULONG  *pFlg;
	ULONG   Bit;

	pDat = (USHORT *)pAot;
	Bit  = pDat[1];
	/* DS forms the word offset as srl 3 / andi 0x1c, which is the word index taken from the low byte of
	   the bit number ((Bit & 0xff) >> 5): bit numbers above 255 wrap into the first eight words of the
	   table. The bit inside the word still uses all of Bit (andi 0x1f). */
	pFlg = Flg_addr[pDat[0]] + ((UCHAR)Bit >> 5);
	if (pDat[2] != 0) {
		*pFlg |= 0x80000000U >> (Bit & 0x1f);
	} else {
		*pFlg &= ~(0x80000000U >> (Bit & 0x1f));
	}
	return 0;
}

/*
 * Sce_Water  (DS 0x80051a2c)
 * AOT 0x07: sets the water depth of the checked work.
 */
/* DS 0x80051a2c (Sce_Water) */
ULONG Sce_Water(void *pAot)
{
	G.pEm->Water = ((short *)pAot)[0];
	return 0;
}

/*
 * Sce_Move  (DS 0x80051a48)
 * AOT 0x08: moves the checked work by (Data0, Data1, Data2).
 */
/* DS 0x80051a48 (Sce_Move) */
ULONG Sce_Move(void *pAot)
{
	ENEMY_WORK *pEm;
	short      *pDat;

	pDat = (short *)pAot;
	pEm  = G.pEm;
	pEm->Pos_x  += pDat[0];
	pEm->Pos_y  += pDat[1];
	pEm->Pos_z  += pDat[2];
	pEm->Be_flg |= 0x20000;
	return 0;
}

/*
 * Sce_Save  (DS 0x80051ab0)
 * AOT 0x09: typewriter.  Starts Save_entrance as back function.
 */
/* DS 0x80051ab0 (Sce_Save) */
ULONG Sce_Save(void *pAot)
{
	SCE_AOT *pAot2;

	pAot2 = pCAot;
	G.Back_func          = Save_entrance;
	G.Back_func_rtn      = 0;
	/* DS reads the save-area number from the AOT data before it copies
	 * Stop_flg to Game.Stop_bak, so the store is written ahead of that copy. */
	G.SaveData.Save_area = (UCHAR)pAot2->Data;
	Game.Stop_bak        = G.Stop_flg;
	G.Stop_flg          |= 0xff000000;
	return 0;
}

/*
 * Save_entrance  (DS 0x80051b04)
 * Back function: asks to save if an ink ribbon (item 0x1e) is carried.
 */
/* DS 0x80051b04 (Save_entrance) */
void Save_entrance(void)
{
	tagGLOBAL *pG = &G;

	if (pG->Back_func_rtn == 0) {
		if (Search_item_id(0x1e) < 0) {
			pG->Stop_flg = Game.Stop_bak;
			Mess_set(0, 0x100, 0, 0xff000000);
			pG->Back_func     = NULL;
			pG->Back_func_rtn = 0;
		} else {
			Mess_set(0, 0x100, 1, 0xff000000);
			pG->Back_func_rtn = 1;
		}
	} else if (pG->Back_func_rtn == 1 && (Moji.Mess_flg & 0x80) == 0) {
		if ((Moji.Mess_flg & 1) == 0) {
			Game.Doordemo_flg = 1;
			pG->System_flg     |= SYS_SAVE_GAME;
		} else {
			pG->Stop_flg = Game.Stop_bak;
		}
		pG->Back_func     = NULL;
		pG->Back_func_rtn = 0;
	}
}

/*
 * Sce_Itembox  (DS 0x80051c20)
 * AOT 0x0a: item box.  Data0 = lid object model, Data1 = tpage.
 */
/* DS 0x80051c20 (Sce_Itembox) */
ULONG Sce_Itembox(void *pAot)
{
	SCE_AOT *pAot2;
	USHORT  *pDat;

	pAot2 = pCAot;
	pDat  = &pAot2->Data;
	G.Ibox_Futa_no   = (UCHAR)pDat[0];
	St.Itembox_tpage = (UCHAR)pDat[1];
	G.Back_func      = Item_box_entrance;
	G.Back_func_rtn  = 0;
	return 0;
}

/*
 * Item_box_entrance  (DS 0x80051c64)
 * Back function: opens the item box lid (rotation Cdir_z of the lid
 * object model), then starts the status screen in item box mode.
 */
/* DS 0x80051c64 (Item_box_entrance) */
void Item_box_entrance(void)
{
	/* DS 0x800c3b9c */
	static short           Dir_spd;
	/* DS 0x800c3ba0 */
	static short           Spd_add;
	/* DS 0x800c3ba4: pointer to the item box OBJ_MODEL_WORK (G.Ob_model[Ibox_Futa_no]);
	 * the lid rotation is its Cdir_z (+0x78). */
	static OBJ_MODEL_WORK *pOwork;
	short                  Old_spd;
	tagGLOBAL             *pG = &G;

	switch (pG->Back_func_rtn) {
	case 0:
		Dir_spd       = 1;
		Spd_add       = 3;
		Game.Stop_bak = pG->Stop_flg;
		pG->Stop_flg   |= 0x7f000000;
		Snd_se_stad(0x2150000, NULL);
		pG->Back_func_rtn = 1;
		pOwork          = &pG->Ob_model[pG->Ibox_Futa_no];
		/* fall through */
	case 1:
		/* lid opening: accelerate */
		pOwork->Cdir_z -= Dir_spd;
		Dir_spd        += Spd_add;
		if (pOwork->Cdir_z < -399) {
			pG->Back_func_rtn = 2;
			/* reverse the acceleration; DS reloads Spd_add sign-extended (lh) before
			 * negating it, which the multiply by -1 gives and a unary minus does not. */
			Spd_add         *= -1;
		}
		break;

	case 2:
		/* lid opening: decelerate until the speed reaches 0 */
		pOwork->Cdir_z -= Dir_spd;
		Dir_spd        += Spd_add;
		if (Dir_spd > 0) {
			break;
		}
		St.St_type        = 1;
		Game.Doordemo_flg = 1;
		Dir_spd           = 0;
		pG->Back_func_rtn   = 3;
		pG->Status_flg     |= STAT_STATUS_SCREEN;
		/* fall through */
	case 3:
		/* wait a few frames, then close the lid */
		Dir_spd++;
		if (Dir_spd < 5) {
			break;
		}
		pOwork->Cdir_z = 0;
		pG->Back_func_rtn = 0;
		pG->Back_func     = NULL;
		break;

	default:
		break;
	}
}

/*
 * Sce_Damage  (DS 0x80051e74)
 * AOT 0x0b: damages the checked work by Data1; Data0 is the damage type.
 * Knocks the work away from the AOT centre and rumbles the pad.
 */
/* DS 0x80051e74 (Sce_Damage) */
ULONG Sce_Damage(void *pAot)
{
	USHORT *pDat;

	pDat = (USHORT *)pAot;
	if ((G.pEm->Damage_cnt & 0x80) != 0) {
		return 1;
	}
	if ((G.pEm->Status_flg & 2) != 0) {
		G.pEm->Status_flg &= ~2;
		return 1;
	}
	G.pEm->Damage_cnt |= 0x80;
	G.pEm->Life       -= pDat[1];
	if (G.pEm->Life < 0) {
		if (G.pEm->Routine_0 != 3) {
			G.pEm->Routine_0 = 3;
			G.pEm->Routine_1 = 0;
			G.pEm->Routine_2 = 0;
			G.pEm->Routine_3 = 0;
		}
	} else {
		G.pEm->Routine_0 = 2;
		G.pEm->Routine_1 = (UCHAR)pDat[0];
		G.pEm->Routine_2 = 0;
		G.pEm->Routine_3 = 0;
		G.pEm->Spd_x     = 200;
		Add_speedXZ(G.pEm, Direction_ck(pCAot->X + (pCAot->W >> 1), pCAot->Z + (pCAot->D >> 1), G.pEm->Pos_x, G.pEm->Pos_z) - G.pEm->Cdir_y);
		Pad_vib_set0(4, 0);
		Pad_vib_set1(10, 0x96, 0);
	}
	return 1;
}

/*
 * Sce_Status  (DS 0x80052004)
 * AOT 0x0c: opens the status screen.
 */
/* DS 0x80052004 (Sce_Status) */
ULONG Sce_Status(void *pAot)
{
	Game.Doordemo_flg = 1;
	St.St_type        = 0;
	Game.Stop_bak     = G.Stop_flg;
	G.Status_flg     |= STAT_STATUS_SCREEN;
	return 0;
}

/*
 * Sce_Hikidashi  (DS 0x80052044)
 * AOT 0x0d: locked drawer / desk.
 *   Data0 = Key_flg bit (unlocked), Data1 = index of the item AOT inside,
 *   Data2 = close-up camera cut.
 */
/* DS 0x80052044 (Sce_Hikidashi) */
ULONG Sce_Hikidashi(void *pAot)
{
	USHORT        *pDat;
	ULONG         *Flg_addr;
	SCE_ITEM_DATA *pItem;
	ULONG          Mess_no;

	pDat = (USHORT *)pAot;
	if (G.Front_func != NULL) {
		return 0;
	}
	if (G.Player.Damage_cnt != 0) {
		return 0;
	}
	if ((G.Status_flg & (STAT_STATUS_SCREEN | 0x40)) != 0) {
		return 0;
	}
	if (G.Next_cut_no != G.SaveData.Cut_no) {
		return 0;
	}

	/* item already taken?  Item_flg (stage 0..3, decompile G+0x86d4) or
	   Item_flg2 (stage 4+, decompile G+0x86f4) */
	Flg_addr = G.SaveData.Item_flg;
	if (G.SaveData.Stage_no >= 4) {
		Flg_addr = G.SaveData.Item_flg2;
	}
	pItem = (SCE_ITEM_DATA *)&G.Sce_aot[pDat[1]]->Data;
	if (Flg_ck(Flg_addr, pItem->Data2) != 0) {
		Mess_no = 4;
	} else if (Flg_ck(G.SaveData.Key_flg, pDat[0]) != 0) {
		/* already unlocked: open the drawer */
		G.pAot_data = pCAot;
		Snd_se_stad(0x2280000, NULL);
		pItem = (SCE_ITEM_DATA *)&G.Sce_aot[pDat[1]]->Data;
		G.Ob_model[pItem->Om_no].Be_flg |= 0x11;
		Cut_change(pDat[2]);
		Game.Stop_bak  = G.Stop_flg;
		G.Stop_flg    |= 0xff000000;
		G.Status_flg  |= STAT_PL_BUSY;
		G.Cccut_next->Xz[0][0] = -0x7fbc;
		G.Cccut_next->Xz[1][0] = -0x7fbc;
		G.Cccut_next->Xz[2][0] = -0x7f58;
		G.Cccut_next->Xz[3][0] = -0x7f58;
		G.Cccut_next->Xz[0][1] = -0x7fbc;
		G.Cccut_next->Xz[1][1] = -0x7f58;
		G.Cccut_next->Xz[2][1] = -0x7f58;
		G.Cccut_next->Xz[3][1] = -0x7fbc;
		G.Front_func     = Hikidashi_entrance;
		G.Front_func_rtn = 0x23;
		return 0;
	} else if (Search_item_id(0x1f) >= 0 || G.SaveData.Item_work[10].Id == 0x30) {
		/* small key (0x1f) carried, or special item in slot 10 */
		G.Front_func     = Hikidashi_entrance;
		G.pAot_data      = pCAot;
		G.Front_func_rtn = 1;
		return 0;
	} else {
		Mess_no = 2;
	}
	Mess_set(0, 0x100, Mess_no, 0xff000000);
	return 0;
}

/*
 * Hikidashi_entrance  (DS 0x800522cc)
 * Front function for Sce_Hikidashi.
 *   1-2   : "use key?" question, unlock and consume the key.
 *   0x23..: camera close-up delay, then 5 opens the status screen on the
 *           item inside, 4 closes the drawer again.
 */
/* DS 0x800522cc (Hikidashi_entrance) */
void Hikidashi_entrance(void)
{
	tagGLOBAL *pG = &G;
	/* DS 0x800c3ba8 */
	static long           Key_work;
	/* DS 0x800c3bac */
	static SCE_ITEM_DATA *pItem_dat;
	/* DS 0x800c3bb0 */
	static SCE_AOT       *pAot;
	USHORT               *pDat;

	pDat = &pG->pAot_data->Data;
	switch (pG->Front_func_rtn) {
	case 0:
		break;

	case 1:
		if (pG->SaveData.Item_work[10].Id == 0x30) {
			Moji.Mess_item = 0x30;
		} else {
			Key_work       = Search_item_id(0x1f);
			Moji.Mess_item = 0x1f;
		}
		Mess_set(0, 0x100, 3, 0xff000000);
		pG->Front_func_rtn = 2;
		break;

	case 2:
		if ((Moji.Mess_flg & 0x80) != 0) {
			break;
		}
		if ((Moji.Mess_flg & 1) == 0) {
			Flg_on(pG->SaveData.Key_flg, pDat[0]);
			Snd_se_stad(0x2270000, NULL);
			if (pG->SaveData.Item_work[10].Id == 0x30) {
				Moji.Mess_item = 0x30;
			} else {
				Moji.Mess_item = 0x1f;
				if (--pG->SaveData.Item_work[Key_work].Num == 0) {
					pG->SaveData.Item_work[Key_work].Size = 0;
					pG->SaveData.Item_work[Key_work].Id   = 0;
				}
				Sort_item();
			}
			Mess_set(0, 0x100, 5, 0xff000000);
		}
		pG->Front_func     = NULL;
		pG->Front_func_rtn = 0;
		break;

	case 4:
		if ((pG->Status_flg & STAT_STATUS_SCREEN) != 0) {
			break;
		}
		pG->Status_flg &= ~STAT_CAM_LOCK;
		pG->Ob_model[pItem_dat->Om_no].Be_flg &= ~0x11;
		pG->Front_func     = NULL;
		pG->Front_func_rtn = 0;
		break;

	case 5:
		pAot              = pG->pAot_data;
		pG->pAot_data       = pG->Sce_aot[pDat[1]];
		St.St_type        = 2;
		pItem_dat         = (SCE_ITEM_DATA *)&pG->Sce_aot[pDat[1]]->Data;
		pG->Status_flg      = (pG->Status_flg & ~STAT_PL_BUSY) | STAT_STATUS_SCREEN;
		Game.Doordemo_flg = 1;
		Cut_change(pG->SaveData.Cut_no_old);
		pG->SaveData.Cut_no = pG->SaveData.Cut_no_old;
		pG->Status_flg     |= STAT_CAM_LOCK;
		pG->Front_func_rtn--;
		pG->Get_item_id     = (UCHAR)pItem_dat->Data0;
		Moji.Mess_item    = pG->Get_item_id;
		break;

	default:
		pG->Front_func_rtn--;
		break;
	}
}

/*
 * Sce_Windows  (DS 0x80052620)
 * AOT 0x0e: opens a window (Window_no = byte2 * 16 + byte0).
 */
/* DS 0x80052620 (Sce_Windows) */
ULONG Sce_Windows(void *pAot)
{
	ULONG b2;
	ULONG b0;

	/* both script bytes are read before Doordemo_flg is set (DS order) */
	b2 = ((UCHAR *)pAot)[2];
	b0 = ((UCHAR *)pAot)[0];
	Game.Doordemo_flg = 7;
	G.Window_no       = b2 * 16 + b0;
	return 0;
}

/*
 * Sce_at  (DS 0x8005264c)
 * Per-frame AOT check for the player (SCE_PLAYER), enemies (SCE_ENEMY),
 * object models (SCE_04) and the partner (SCE_08).
 */
/* DS 0x8005264c (Sce_at) */
/*
 * Sce_at  (per frame)
 * Runs the action-object (AOT) checks for the player, every live enemy and every active object model.
 * The player gets an extra manual check (action button) while the inspect status is on. Water is reset
 * for each owner first, so a stale water flag cannot survive a frame in which the owner left the water.
 */
void Sce_at(void)
{
	ENEMY_WORK     *pEw;
	ENEMY_WORK    **ppEw;
	OBJ_MODEL_WORK *pOm;
	OBJ_MODEL_WORK *pOm_end;

	if (G.Player.Life < 0) {
		return;
	}
	G.Player.Water = 0;
	if ((G.Status_flg & STAT_INSPECT) != 0) {
		Sce_at_check((ENEMY_WORK *)&G.Player, SCE_PLAYER, SCE_MANUAL);
	}
	Sce_at_check((ENEMY_WORK *)&G.Player, SCE_PLAYER, 0);

	for (ppEw = G.pEnemy + 2; ppEw != G.ppEnemy_end; ppEw++) {
		pEw = *ppEw;
		if ((pEw->Be_flg & 1) != 0) {
			pEw->Water = 0;
			Sce_at_check(pEw, SCE_ENEMY, 0);
		}
	}

	pOm_end = G.Ob_model + G.pRoom->nOmodel;
	for (pOm = G.Ob_model; pOm < pOm_end; pOm++) {
		if ((pOm->Be_flg & OM_ENABLED) != 0) {
			pOm->Water = 0;
			Sce_at_check((ENEMY_WORK *)pOm, SCE_04, 0);
		}
	}

	pEw = G.pEnemy[1];
	if ((pEw->Be_flg & 1) != 0) {
		pEw->Water = 0;
		Sce_at_check(G.pEnemy[1], SCE_08, 0);
	}
}

/*
 * Water_ck  (DS 0x800527b4)
 * Returns the water depth of the first water AOT (Id SCE_WATER) containing
 * (Pos_x, Pos_z), or 0.
 */
/* DS 0x800527b4 (Water_ck) */
long Water_ck(long Pos_x, long Pos_z)
{
	/* retail reserves 8 more bytes of locals (Water_ck frame 88); never read */
	volatile int Rsv[2];
	VECTOR    Vec;
	ULONG     i;
	ULONG     D0;
	SCE_AOT  *pAot;
	SCE_AOT **ppAot;
	short     Work[10];   /* [0..1] unused (uninitialised), [2..9] points */
	short    *pData;

	D0     = G.nSce_aot;
	i      = 0;
	Vec.vx = Pos_x;
	Vec.vy = 0;
	Vec.vz = Pos_z;
	if (D0 == 0) {
		return 0;
	}
	ppAot = G.Sce_aot;
	do {
		pAot = *ppAot++;
		if (pAot == NULL) {
			continue;
		}
		i++;
		if (pAot->Id != SCE_WATER) {
			continue;
		}
		pData = &pAot->X;
		if ((pAot->Type & SCE_POINT4) != 0) {
			Work[2] = pData[0];
			Work[3] = pData[1];
			Work[4] = pData[2];
			Work[5] = pData[3];
			Work[6] = pData[4];
			Work[7] = pData[5];
			Work[8] = pData[6];
			Work[9] = pData[7];
			if (Hit_ck_point4(&Vec, (VCUT *)Work) != 0) {
				return pData[8];        /* +0x14 */
			}
		} else {
			if (Hit_ck_box(&Vec, (BOX *)&pAot->X) != 0) {
				return pData[4];        /* +0x0c */
			}
		}
	} while (i < D0);
	return 0;
}

/* Floor table record, 12 bytes: BOX (X, Z, W, D), UCHAR Type at +8, SHORT Floor at +10.
 * Retail Floor_check (DS 0x800528e8): "lbu v0,0x0(s0)" (DS 0x80052978) and "lh v1,0x2(s0)"
 * (DS 0x80052944) with s0 = record + 8. */
typedef struct {
	BOX   Box;
	UCHAR Type;
	UCHAR Dummy;
	SHORT Floor;
} FLOOR_REC;

/*
 * Floor_check  (DS 0x800528e8)
 * Returns the floor (sound) type at (check_x, check_z) on floor nfloor,
 * scanning the room floor table backwards.  Default 0x1d.
 * Floor record (12 bytes): BOX (X,Z,W,D), UCHAR type @+8, short floor @+10.
 */
/* DS 0x800528e8 (Floor_check) */
UCHAR Floor_check(long check_x, long check_z, ULONG nfloor)
{
	short  *A0;
	UCHAR  *pType;
	ULONG   D0;
	ULONG   D1;
	VECTOR  Vec;

	A0     = G.pRoom->pFloor;
	D0     = *A0++;
	A0    += D0 * 6;          /* end of the record table */
	pType  = &((FLOOR_REC *)A0)->Type; /* type byte (+8) of the record below the end, stepped 12 bytes per record */
	Vec.vx = check_x;
	Vec.vz = check_z;
	while (D0-- != 0) {
		pType -= 12;
		A0 -= 6;              /* previous 12-byte record */
		D1  = ((FLOOR_REC *)A0)->Floor;
		if ((D1 & 0x80) == 0 && (D1 & 0x7f) == nfloor && Hit_ck_box(&Vec, (BOX *)A0) != 0) {
			return *pType;
		}
	}
	return 0x1d;
}
