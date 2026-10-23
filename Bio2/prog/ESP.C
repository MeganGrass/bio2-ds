/*
 * ESP.C - Effect Sprite (ESP) management
 * D:\Bio2\prog\ESP.C
 *
 * All 18 function names and local variable names sourced from BIO2_BETA.LOG
 * PSYQ symbol records (8c/8e/94/96). No overlay; all functions are retail-linked.
 *
 * Functions (beta log order, addresses are retail):
 *   Esp_init_C   @ 8001babc
 *   Esp_init_R   @ 8001bba4
 *   Esp_data_set0 @ 8001bca0
 *   Esp_data_set1 @ 8001bd38
 *   Esp_call     @ 8001bf10
 *   Esp_call3D   @ 8001c224
 *   Esp_call3D2  @ 8001c56c
 *   Esp_call2    @ 8001c8c4
 *   Esp_call3    @ 8001cbe8
 *   Esp_kill     @ 8001cefc
 *   Esp_kill2    @ 8001cfbc
 *   Esp_die      @ 8001cffc
 *   Esp_ctrl     @ 8001d0ac
 *   Esp_ctrl2    @ 8001d15c
 *   Esp_ctrl3    @ 8001d180
 *   Esp_ctrl4    @ 8001d240
 *   Esp_move     @ 8001d300
 *   Esp_pos      @ 8001d68c
 */

#include "ESP.H"
#include "TYPES.H"
#include <GTEMAC.H>
#include <INLINE_C.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <STRINGS.H>
#include "game.h"
#include "CD.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "ESP_R.H"
#include "ROOM.H"
#include "SCE1.H"
#include "SELECT.H"
#include "SCE2.H"
#include "FILECODE.H"

/* DS 0x801fc400: CORE00.ESP load buffer */
#define ESP_CORE_BUF ((UCHAR *)0x801fc400)


/* DS 0x8009d868 */
VOID *Esp_tblj[96] = {
	(VOID *)Esp_00, (VOID *)Esp_01, (VOID *)Esp_02, (VOID *)Esp_03,
	(VOID *)Esp_04, (VOID *)Esp_05, (VOID *)Esp_06, (VOID *)Esp_07,
	(VOID *)Esp_08, (VOID *)Esp_09, (VOID *)Esp_0a, (VOID *)Esp_0b,
	(VOID *)Esp_0c, (VOID *)Esp_0d, (VOID *)Esp_0e, (VOID *)Esp_0f,
	(VOID *)Esp_10, (VOID *)Esp_11, (VOID *)Esp_12, (VOID *)Esp_13,
	(VOID *)Esp_14, (VOID *)Esp_15, (VOID *)Esp_16, (VOID *)Esp_17,
	(VOID *)Esp_18, (VOID *)Esp_19, (VOID *)Esp_1a, (VOID *)Esp_1b,
	(VOID *)Esp_1c, (VOID *)Esp_1d, (VOID *)Esp_1e, (VOID *)Esp_1f,
	(VOID *)Esp_20, (VOID *)Esp_21, (VOID *)Esp_22, (VOID *)Esp_23,
	(VOID *)Esp_24, (VOID *)Esp_25, (VOID *)Esp_26, (VOID *)Esp_27,
	(VOID *)Esp_28, (VOID *)Esp_29, (VOID *)Esp_2a, (VOID *)Esp_2b,
	(VOID *)Esp_2c, (VOID *)Esp_2d, (VOID *)Esp_2e, (VOID *)Esp_2f,
	(VOID *)Esp_30, (VOID *)Esp_31, (VOID *)Esp_32, (VOID *)Esp_33,
	(VOID *)Esp_34, (VOID *)Esp_35, (VOID *)Esp_36, (VOID *)Esp_37,
	(VOID *)Esp_38, (VOID *)Esp_39, (VOID *)Esp_3a, (VOID *)Esp_3b,
	(VOID *)Esp_3c, (VOID *)Esp_3d, (VOID *)Esp_3e, (VOID *)Esp_3f,
	(VOID *)Esp_40, (VOID *)Esp_41, (VOID *)Esp_42, (VOID *)Esp_43,
	(VOID *)Esp_44, (VOID *)Esp_45, (VOID *)Esp_46, (VOID *)Esp_47,
	(VOID *)Esp_48, (VOID *)Esp_49, (VOID *)Esp_4a, (VOID *)Esp_4b,
	(VOID *)Esp_4c, (VOID *)Esp_4d, (VOID *)Esp_4e, (VOID *)Esp_4f,
	(VOID *)Esp_50, (VOID *)Esp_51, (VOID *)Esp_52, (VOID *)Esp_53,
	(VOID *)Esp_54, (VOID *)Esp_55, (VOID *)Esp_56, (VOID *)Esp_57,
	(VOID *)Esp_58, (VOID *)Esp_59, (VOID *)Esp_5a, (VOID *)Esp_5b,
	(VOID *)Esp_5c, (VOID *)Esp_5d, (VOID *)Esp_5e, (VOID *)Esp_5f,
};

/* pEspdt / pEspmv: 64 entries each. Esp_init_C (DS 0x8001bb08-0x8001bb34) clears 0x40 words of
 * each table, counting down from 0x800d4dd8 and 0x800d4f18 (the end of each table). */
/* DS 0x800d4cd8 */
ULONG *pEspdt[64];

/* DS 0x800d4e18 */
ULONG *pEspmv[64];

/* Local to ESP.C (static in the beta ESP.C); declared here because Esp_init_C and Esp_init_R call them first. */
UCHAR Esp_data_set0(UCHAR *ptr8, ULONG *ptr32, ULONG addr, ULONG type);
void Esp_data_set1(ULONG *plong, ULONG *ptr);


/* DS 0x800d8cf0 (Esp): effect pool, 96 slots of 0x7C bytes; the file bytes are all zero */
ESP_WORK Esp[96];

/* DS 0x800dcbd0 (pESP) */
ESP_WORK *pESP;



/* DS 0x800eae48 */
UCHAR Espid[16];

/* Dispatch through the routine table (same as ESP_R.H ESP_TBLJ_CALL). */
#ifndef ESP_TBLJ_CALL
#define ESP_TBLJ_CALL(idx)	(((void (*)(void))Esp_tblj[(idx)])())
#endif

/* ------------------------------------------------------------------ */
/* Esp_init_C                                                           */
/* Initialize ESP system from core (CORE00.ESP) CD file.               */
/* locals: D0, pCore_end                                                */
/* ------------------------------------------------------------------ */
/* DS 0x8001babc (Esp_init_C) */
UCHAR Esp_init_C(void)
{
	ULONG D0;
	UCHAR *pCore_end;
	int cnt0;
	int cnt1;

	for (cnt0 = 96; cnt0 != 0; ) {
		Esp[--cnt0].Be_flg = 0;
	}

	for (cnt1 = 16; cnt1 != 0; ) {
		Espid[--cnt1] = 0xff;
	}

	/* DS clears all 64 entries of both tables, from the last entry down */
	for (cnt0 = 64; cnt0 != 0; ) {
		cnt0--;
		pEspdt[cnt0] = (ULONG *)0xffffffff;
		pEspmv[cnt0] = (ULONG *)0xffffffff;
	}

	/* Cd_read returns the byte count; the end pointer is rounded up to a whole word, and the
	 * table walk in Esp_data_set0 starts at the last word and goes backward. */
	D0 = Cd_read(CORE00_ESP, (ULONG *)ESP_CORE_BUF, 1, (UCHAR *)"CORE00.ESP");
	pCore_end = ESP_CORE_BUF + ((D0 >> 2) << 2);
	if ((D0 & 3) != 0) {
		pCore_end += 4;
	}

	return Esp_data_set0(ESP_CORE_BUF, (ULONG *)(pCore_end + -4), (ULONG)ESP_CORE_BUF, 0);
}

/* ------------------------------------------------------------------ */
/* Esp_init_R                                                           */
/* Initialize ESP system from room data (pRoom->pEsp_hed).             */
/* locals: cnt0, cnt1                                                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001bba4 (Esp_init_R) */
UCHAR Esp_init_R(void)
{
	UCHAR cnt0;
	UINT cnt1;
	UCHAR Ret;
	ULONG *ptr8;

	/* DS clears Be_flg of all 96 Esp entries from the last one down, counting in the same register
	 * (a0) that the Espid walk below reuses, so one counter serves both loops. */
	for (cnt1 = 96; cnt1 != 0; ) {
		cnt1--;
		Esp[cnt1].Be_flg = 0;
	}

	cnt1 = 8;
	do {
		cnt0 = (UINT)Espid[cnt1];
		if (cnt0 == 0xff) {
			cnt1 = 0x10;
		} else {
			pEspdt[cnt0] = (ULONG *)0xffffffff;
			pEspmv[cnt0] = (ULONG *)0xffffffff;
			Espid[cnt1] = 0xff;
			cnt1++;
		}
		Ret = cnt1 < 0x10;
	} while ((BOOL)Ret);

	/* Load the room ESP tables only when the room has a head (pEsp_hed != 0) and its first word is not
	 * the -1 end mark. DS has no return statement here: v0 is whatever is left in it (0 from the loop
	 * test when the head is 0, -1 when the first word is -1, otherwise what Esp_data_set1 leaves).
	 * The only caller (ROOM.C) discards the value. */
	ptr8 = (G.pRoom)->pEsp_hed;
	if (ptr8 != (ULONG *)0x0 && *ptr8 != 0xffffffff) {
		Esp_data_set0((UCHAR *)ptr8, (G.pRoom)->pEsp_end, (ULONG)ptr8, 8);
		Esp_data_set1((G.pRoom)->pEsp_tim_end, (G.pRoom)->pEsp_tim);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_data_set0                                                        */
/* Load ESP data table: map IDs to data/move pointers.                 */
/* params: ptr8, ptr32, addr, type                                      */
/* locals: D0, D1, id, cnt0, addres                                    */
/* ------------------------------------------------------------------ */
/* DS 0x8001bca0 (Esp_data_set0) */
UCHAR Esp_data_set0(UCHAR *ptr8, ULONG *ptr32, ULONG addr, ULONG type)
{
	ULONG D0;
	ULONG *addres;
	UINT id;
	UINT cnt0;
	UINT nwords;

	/* Each pass reads one id byte from the list at ptr8 (0xff ends it), records it in Espid, and
	 * stores the entry's data and move pointers at pEspdt[id] / pEspmv[id], relative to addr. */
	for (cnt0 = 0; cnt0 < 8; cnt0++) {
		id = (UINT)*ptr8;
		Espid[cnt0 + type] = *ptr8;
		if (id == 0xff) {
			break;
		}
		D0 = *ptr32;
		ptr32 = ptr32 + -1;
		ptr8++;
		addres = (ULONG *)(D0 + addr);
		pEspdt[id] = addres;
		nwords = (*addres >> 0x10) + (*addres & 0xffff) * 2 + 2;
		pEspmv[id] = addres + nwords;
	}

	return (UCHAR)cnt0;
}

/* ESP data block header: the first words of a pEspdt[] entry (read as ULONG words, so the struct starts at the
 * entry address). Clut (+4) and Tpage (+6) are the values Esp_call* copy into ESP_WORK.Clut/Tpage. Anmseq[0] starts
 * at +8, so its Time byte is at +10, which Esp_call* copy into ESP_WORK.Anm_time (retail Esp_call 8001bfc4:
 * lbu v0,0xa(t1); 8001bfd4: lhu v0,0x6(t1); 8001c008: lhu v1,0x4(t1)). Esp_data_set1 writes Clut with sh at
 * 8001be70 (sh v0,0x4(s0)), reads Unk_02 at 8001be8c (lhu a2,0x2(s0)) and stores the low byte of Tpage at
 * 8001be94 (sb v0,0x6(s0)). Ndat and Unk_02 are not decoded. */
typedef struct {
	USHORT Ndat;
	USHORT Unk_02;
	USHORT Clut;
	USHORT Tpage;
	SSEQUENCE Anmseq[1];
} ESP_DATA_HDR;

/* ------------------------------------------------------------------ */
/* Esp_data_set1                                                       */
/* Load TIM textures for ESP sprites into VRAM, patch clut/tpage.      */
/* params: plong, ptr                                                   */
/* locals: D0, D1, tpage, vramy, cluty, pU32, pSpgp, Tim, pTimadr     */
/* ------------------------------------------------------------------ */
/* DS 0x8001bd38 (Esp_data_set1) */
void Esp_data_set1(ULONG *plong, ULONG *ptr)
{
	USHORT tpage;
	int vramy;
	int cluty;
	int colSlot;
	ULONG *pU32;
	TIM_IMAGE Tim;
	RECT *pTimadr[8];
	UINT D0;
	char *pVcoord;
	UINT cnt;
	UCHAR *pSlot;

	/* pTimadr[i] = ptr + plong[-(i+1)] (the 8 TIM offsets, read backwards) */
	for (D0 = 0; D0 < 8; D0++) {
		plong = plong + -1;
		pTimadr[D0] = (RECT *)((int)ptr + *plong);
	}

	/* colSlot (column slot, starts at 15, decremented on wrap) is separate from
	 * cluty (CLUT / rect y, starts at 0x1e0, advanced by crect.h). */
	colSlot = 0xf;
	vramy = 0;
	cluty = 0x1e0;
	pSlot = Espid + 8;
	for (D0 = 0; D0 < 8; D0++) {
		if (*pSlot == 0xff) {
			return;
		}
		OpenTIM((ULONG *)pTimadr[D0]);
		ReadTIM(&Tim);
		/* when this TIM would run past the 256-line column, start the next column to the left */
		if (0x100 < (UINT)(vramy + (Tim.prect)->h)) {
			colSlot = colSlot + -1;
			vramy = 0;
		}
		*(int *)Tim.prect = (vramy << 0x10) + colSlot * 0x40;
		LoadImage(Tim.prect, Tim.paddr);
		DrawSync(0);
		if (Tim.caddr != (ULONG *)0x0) {
			*(int *)Tim.crect = cluty * 0x10000 + 0x120;
			LoadImage(Tim.crect, Tim.caddr);
			DrawSync(0);
		}
		pU32 = pEspdt[*pSlot];
		tpage = GetClut(0x120, cluty);
		((ESP_DATA_HDR *)pU32)->Clut = tpage;
		tpage = GetTPage(0, 0, (int)(Tim.prect)->x, (int)(Tim.prect)->y);
		cnt = (UINT)((ESP_DATA_HDR *)pU32)->Unk_02;
		*(char *)&((ESP_DATA_HDR *)pU32)->Tpage = (char)tpage;
		pVcoord = (char *)((int)pU32 + (UINT)(USHORT)*pU32 * 8 + 9);
		do {
			cnt--;
			*pVcoord = *pVcoord + (char)vramy;
			pVcoord += 4;
		} while (cnt != 0);
		pSlot++;
		vramy = vramy + (Tim.prect)->h;
		cluty = cluty + (Tim.crect)->h;
	}

	return;
}

/* ------------------------------------------------------------------ */
/* Esp_call - Spawn with Be_flg 0xa003 (DS 8001bf10)                    */
/*            bit = Id << 24 | Type << 16 | Mag                         */
/* ------------------------------------------------------------------ */
/* DS writes ESP_WORK with word stores and word copies; ESP_WORD() views the named member and the next one as one
 * ULONG (the DS stores Id/Id2/Type/Mov_cnt, Mov_x/Mov_y, Hot_x/Hot_y ... as single sw). ESP_BODY_W is the part of
 * ESP_WORK after the MSEQUENCE (Be_flg on); assigning it copies the 0x64 bytes as one block, which is how a child
 * slot is made from its parent. The MATRIX at ESP_WORK+0x4c (m and t) is likewise copied by structure assignment. */
#define ESP_WORD(m) (*(ULONG *)&(m))
typedef struct { ULONG w[(sizeof(ESP_WORK) - sizeof(MSEQUENCE)) / 4]; } ESP_BODY_W;

/* Copies the 24-byte move entry at pMoveseq into the slot word by word; DS reloads pMoveseq for every word. */
#define ESP_MSEQ_COPY(e) do { ((ULONG *)(e))[0] = ((ULONG *)(e)->pMoveseq)[0]; ((ULONG *)(e))[1] = ((ULONG *)(e)->pMoveseq)[1]; ((ULONG *)(e))[2] = ((ULONG *)(e)->pMoveseq)[2]; ((ULONG *)(e))[3] = ((ULONG *)(e)->pMoveseq)[3]; ((ULONG *)(e))[4] = ((ULONG *)(e)->pMoveseq)[4]; ((ULONG *)(e))[5] = ((ULONG *)(e)->pMoveseq)[5]; } while (0)

/* DS 0x8001bf10 (Esp_call) */
UCHAR Esp_call(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec)
{
	ULONG id = bit >> 24;
	ULONG tp = (bit >> 16) & 0xff;
	ULONG *pData;
	ULONG *pMove;
	ULONG nseq;
	ULONG sprg_ofs;
	ESP_WORK *pParent;
	ESP_WORK *pChild;
	ULONG *pGrp;
	LONG slot;
	LONG n;
	ULONG j;

	pData = pEspdt[id];
	pMove = pEspmv[id];
	pMove = (ULONG *)((UCHAR *)pMove + 4 * (*(USHORT *)((UCHAR *)pMove + 2 * (tp & 7))));
	nseq = *pMove & 0xffff;
	pMove = pMove + 1;				/* DS steps past the group count word at once */
	sprg_ofs = (*pData & 0xffff) * 4 + 4;	/* the sprite group list follows the header at pData + sprg_ofs halfwords */

	/* Free-slot search from the top: n counts down from 96 and the first slot with Be_flg 0 is taken (0xff when none). */
	n = 96;
	while (n != 0) {
		n--;
		if (Esp[n].Be_flg == 0) {
			goto found;
		}
	}
	return 0xff;
found:
	slot = n;
	pParent = &Esp[slot];
	pParent->Be_flg = 0xa003;
	pParent->Wk_no = 0;
	pParent->Padd0 = 0;
	ESP_WORD(pParent->Id) = (tp << 16) + id;	/* Id, Id2 = 0, Type, Mov_cnt = 0 */
	*(USHORT *)&pParent->Anm_time = ((ESP_DATA_HDR *)pData)->Anmseq[0].Time;
	pParent->DirY = dir_y;
	ESP_WORD(pParent->Mov_x) = 0;
	ESP_WORD(pParent->Mov_z) = (ULONG)((ESP_DATA_HDR *)pData)->Tpage << 16;
	if (svec != NULL) {
		ESP_WORD(pParent->Hot_x) = ((ULONG *)svec)[0];
		ESP_WORD(pParent->Hot_z) = ((ULONG *)svec)[1];
	} else {
		ESP_WORD(pParent->Hot_x) = 0;
		ESP_WORD(pParent->Hot_z) = 0;
	}
	ESP_WORD(pParent->Pos_x) = 0;
	ESP_WORD(pParent->Pos_z) = bit << 16;	/* Pos_z = 0, Mag = low half of bit */
	ESP_WORD(pParent->Old_x) = 0;
	ESP_WORD(pParent->Old_z) = 0x10000;	/* Old_z = 0, Padd1 = 1 */
	ESP_WORD(pParent->Cdir_x) = 0;
	ESP_WORD(pParent->Cdir_z) = 0;		/* Cdir_z = 0, Padd2 = 0 */
	pParent->Clut = (USHORT)(((ESP_DATA_HDR *)pData)->Clut + (tp >> 3) * 0x40);
	*(MATRIX *)pParent->m = *pmat;
	pParent->pMat = pmat;
	pParent->pAnmseq = (SSEQUENCE *)((ESP_DATA_HDR *)pData)->Anmseq;
	pParent->pSprg = (SPRITEGp *)((UCHAR *)pData + sprg_ofs * 2);
	pParent->pMoveseq = (MSEQUENCE *)(pMove + 1);
	ESP_MSEQ_COPY(pParent);

	/* Children: one per remaining move group; the free-slot search continues downward from the parent's slot (0xfe when
	 * the pool runs out). Be_flg and Padd0 are stored first and overwritten by the body copy, as in DS. */
	while (--nseq != 0) {
		while (n != 0) {
			n--;
			if (Esp[n].Be_flg == 0) {
				goto found_child;
			}
		}
		return 0xfe;
found_child:
		pChild = &Esp[n];
		pChild->Be_flg = 0xa003;
		pChild->Padd0 = 0;
		*(ESP_BODY_W *)&pChild->Be_flg = *(ESP_BODY_W *)&Esp[slot].Be_flg;
		pChild->Wk_no = (UCHAR)slot;
		pGrp = pMove;
		j = nseq;
		do {
			j--;
			pGrp += (*pGrp & 0xffff) * 6 + 1;
		} while (j != 0);
		pGrp++;
		pChild->pMoveseq = (MSEQUENCE *)pGrp;
		ESP_MSEQ_COPY(pChild);
	}
	return (UCHAR)slot;
}

/* ------------------------------------------------------------------ */
/* Esp_call3D - Esp_call plus Cdir_x/y/z from dsv (DS 8001c224)          */
/* ------------------------------------------------------------------ */
/* DS 0x8001c224 (Esp_call3D) */
UCHAR Esp_call3D(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec, SVECTOR *dsv)
{
	ULONG id = bit >> 24;
	ULONG tp = (bit >> 16) & 0xff;
	ULONG *pData;
	ULONG *pMove;
	ULONG nseq;
	ULONG sprg_ofs;
	ESP_WORK *pParent;
	ESP_WORK *pChild;
	ULONG *pGrp;
	LONG slot;
	LONG n;
	ULONG j;

	pData = pEspdt[id];
	pMove = pEspmv[id];
	pMove = (ULONG *)((UCHAR *)pMove + 4 * (*(USHORT *)((UCHAR *)pMove + 2 * (tp & 7))));
	nseq = *pMove & 0xffff;
	pMove = pMove + 1;				/* DS steps past the group count word at once */
	sprg_ofs = (*pData & 0xffff) * 4 + 4;	/* the sprite group list follows the header at pData + sprg_ofs halfwords */

	/* Free-slot search from the top: n counts down from 96 and the first slot with Be_flg 0 is taken (0xff when none). */
	n = 96;
	while (n != 0) {
		n--;
		if (Esp[n].Be_flg == 0) {
			goto found;
		}
	}
	return 0xff;
found:
	slot = n;
	pParent = &Esp[slot];
	pParent->Be_flg = 0xa003;
	pParent->Wk_no = 0;
	pParent->Padd0 = 0;
	ESP_WORD(pParent->Id) = (tp << 16) + id;	/* Id, Id2 = 0, Type, Mov_cnt = 0 */
	*(USHORT *)&pParent->Anm_time = ((ESP_DATA_HDR *)pData)->Anmseq[0].Time;
	pParent->DirY = dir_y;
	ESP_WORD(pParent->Mov_x) = 0;
	ESP_WORD(pParent->Mov_z) = (ULONG)((ESP_DATA_HDR *)pData)->Tpage << 16;
	if (svec != NULL) {
		ESP_WORD(pParent->Hot_x) = ((ULONG *)svec)[0];
		ESP_WORD(pParent->Hot_z) = ((ULONG *)svec)[1];
	} else {
		ESP_WORD(pParent->Hot_x) = 0;
		ESP_WORD(pParent->Hot_z) = 0;
	}
	ESP_WORD(pParent->Pos_x) = 0;
	ESP_WORD(pParent->Pos_z) = bit << 16;	/* Pos_z = 0, Mag = low half of bit */
	ESP_WORD(pParent->Old_x) = 0;
	ESP_WORD(pParent->Old_z) = 0x10000;	/* Old_z = 0, Padd1 = 1 */
	ESP_WORD(pParent->Cdir_x) = 0;
	ESP_WORD(pParent->Cdir_z) = 0;		/* Cdir_z = 0, Padd2 = 0 */
	pParent->Clut = (USHORT)(((ESP_DATA_HDR *)pData)->Clut + (tp >> 3) * 0x40);
	*(MATRIX *)pParent->m = *pmat;
	pParent->pMat = pmat;
	pParent->pAnmseq = (SSEQUENCE *)((ESP_DATA_HDR *)pData)->Anmseq;
	pParent->pSprg = (SPRITEGp *)((UCHAR *)pData + sprg_ofs * 2);
	pParent->pMoveseq = (MSEQUENCE *)(pMove + 1);
	ESP_MSEQ_COPY(pParent);
	ESP_WORD(pParent->Cdir_x) = *(ULONG *)dsv;
	pParent->Cdir_z = ((USHORT *)dsv)[2];

	/* Children: one per remaining move group; the free-slot search continues downward from the parent's slot (0xfe when
	 * the pool runs out). Be_flg and Padd0 are stored first and overwritten by the body copy, as in DS. */
	while (--nseq != 0) {
		while (n != 0) {
			n--;
			if (Esp[n].Be_flg == 0) {
				goto found_child;
			}
		}
		return 0xfe;
found_child:
		pChild = &Esp[n];
		pChild->Be_flg = 0xa003;
		pChild->Padd0 = 0;
		*(ESP_BODY_W *)&pChild->Be_flg = *(ESP_BODY_W *)&Esp[slot].Be_flg;
		pChild->Wk_no = (UCHAR)slot;
		pGrp = pMove;
		j = nseq;
		do {
			j--;
			pGrp += (*pGrp & 0xffff) * 6 + 1;
		} while (j != 0);
		pGrp++;
		pChild->pMoveseq = (MSEQUENCE *)pGrp;
		ESP_MSEQ_COPY(pChild);
		ESP_WORD(pChild->Cdir_x) = *(ULONG *)dsv;
		pChild->Cdir_z = ((USHORT *)dsv)[2];
	}
	return (UCHAR)slot;
}


/* ------------------------------------------------------------------ */
/* Esp_call3D2 - Esp_call3D with DirY / Id2 packed in dir_y_id2 (DS 8001c56c) */
/*   DirY = high 16 bits, Id2 = low 8 bits                              */
/* ------------------------------------------------------------------ */
/* DS 0x8001c56c (Esp_call3D2) */
UCHAR Esp_call3D2(ULONG bit, ULONG dir_y_id2, MATRIX *pmat, SVECTOR *svec, SVECTOR *dsv)
{
	ULONG id = bit >> 24;
	ULONG tp = (bit >> 16) & 0xff;
	ULONG *pData;
	ULONG *pMove;
	ULONG nseq;
	ULONG sprg_ofs;
	ESP_WORK *pParent;
	ESP_WORK *pChild;
	ULONG *pGrp;
	LONG slot;
	LONG n;
	ULONG j;

	pData = pEspdt[id];
	pMove = pEspmv[id];
	pMove = (ULONG *)((UCHAR *)pMove + 4 * (*(USHORT *)((UCHAR *)pMove + 2 * (tp & 7))));
	nseq = *pMove & 0xffff;
	pMove = pMove + 1;				/* DS steps past the group count word at once */
	sprg_ofs = (*pData & 0xffff) * 4 + 4;	/* the sprite group list follows the header at pData + sprg_ofs halfwords */

	/* Free-slot search from the top: n counts down from 96 and the first slot with Be_flg 0 is taken (0xff when none). */
	n = 96;
	while (n != 0) {
		n--;
		if (Esp[n].Be_flg == 0) {
			goto found;
		}
	}
	return 0xff;
found:
	slot = n;
	pParent = &Esp[slot];
	pParent->Be_flg = 0xa003;
	pParent->Wk_no = 0;
	pParent->Padd0 = 0;
	ESP_WORD(pParent->Id) = id + ((dir_y_id2 << 8) & 0xffff) + (tp << 16);	/* Id, Id2 = 0, Type, Mov_cnt = 0 */
	*(USHORT *)&pParent->Anm_time = ((ESP_DATA_HDR *)pData)->Anmseq[0].Time;
	pParent->DirY = dir_y_id2 >> 16;
	ESP_WORD(pParent->Mov_x) = 0;
	ESP_WORD(pParent->Mov_z) = (ULONG)((ESP_DATA_HDR *)pData)->Tpage << 16;
	if (svec != NULL) {
		ESP_WORD(pParent->Hot_x) = ((ULONG *)svec)[0];
		ESP_WORD(pParent->Hot_z) = ((ULONG *)svec)[1];
	} else {
		ESP_WORD(pParent->Hot_x) = 0;
		ESP_WORD(pParent->Hot_z) = 0;
	}
	ESP_WORD(pParent->Pos_x) = 0;
	ESP_WORD(pParent->Pos_z) = bit << 16;	/* Pos_z = 0, Mag = low half of bit */
	ESP_WORD(pParent->Old_x) = 0;
	ESP_WORD(pParent->Old_z) = 0x10000;	/* Old_z = 0, Padd1 = 1 */
	ESP_WORD(pParent->Cdir_x) = 0;
	ESP_WORD(pParent->Cdir_z) = 0;		/* Cdir_z = 0, Padd2 = 0 */
	pParent->Clut = (USHORT)(((ESP_DATA_HDR *)pData)->Clut + (tp >> 3) * 0x40);
	*(MATRIX *)pParent->m = *pmat;
	pParent->pMat = pmat;
	pParent->pAnmseq = (SSEQUENCE *)((ESP_DATA_HDR *)pData)->Anmseq;
	pParent->pSprg = (SPRITEGp *)((UCHAR *)pData + sprg_ofs * 2);
	pParent->pMoveseq = (MSEQUENCE *)(pMove + 1);
	ESP_MSEQ_COPY(pParent);
	ESP_WORD(pParent->Cdir_x) = *(ULONG *)dsv;
	pParent->Cdir_z = ((USHORT *)dsv)[2];

	/* Children: one per remaining move group; the free-slot search continues downward from the parent's slot (0xfe when
	 * the pool runs out). Be_flg and Padd0 are stored first and overwritten by the body copy, as in DS. */
	while (--nseq != 0) {
		while (n != 0) {
			n--;
			if (Esp[n].Be_flg == 0) {
				goto found_child;
			}
		}
		return 0xfe;
found_child:
		pChild = &Esp[n];
		pChild->Be_flg = 0xa003;
		pChild->Padd0 = 0;
		*(ESP_BODY_W *)&pChild->Be_flg = *(ESP_BODY_W *)&Esp[slot].Be_flg;
		pChild->Wk_no = (UCHAR)slot;
		pGrp = pMove;
		j = nseq;
		do {
			j--;
			pGrp += (*pGrp & 0xffff) * 6 + 1;
		} while (j != 0);
		pGrp++;
		pChild->pMoveseq = (MSEQUENCE *)pGrp;
		ESP_MSEQ_COPY(pChild);
		ESP_WORD(pChild->Cdir_x) = *(ULONG *)dsv;
		pChild->Cdir_z = ((USHORT *)dsv)[2];
	}
	return (UCHAR)slot;
}


/* ------------------------------------------------------------------ */
/* Esp_call2 - Esp_call with DirY / Id2 packed in dir_y_id2 (DS 8001c8c4) */
/* ------------------------------------------------------------------ */
/* DS 0x8001c8c4 (Esp_call2) */
UCHAR Esp_call2(ULONG bit, ULONG dir_y_id2, MATRIX *pmat, SVECTOR *svec)
{
	ULONG id = bit >> 24;
	ULONG tp = (bit >> 16) & 0xff;
	ULONG *pData;
	ULONG *pMove;
	ULONG nseq;
	ULONG sprg_ofs;
	ESP_WORK *pParent;
	ESP_WORK *pChild;
	ULONG *pGrp;
	LONG slot;
	LONG n;
	ULONG j;

	pData = pEspdt[id];
	pMove = pEspmv[id];
	pMove = (ULONG *)((UCHAR *)pMove + 4 * (*(USHORT *)((UCHAR *)pMove + 2 * (tp & 7))));
	nseq = *pMove & 0xffff;
	pMove = pMove + 1;				/* DS steps past the group count word at once */
	sprg_ofs = (*pData & 0xffff) * 4 + 4;	/* the sprite group list follows the header at pData + sprg_ofs halfwords */

	/* Free-slot search from the top: n counts down from 96 and the first slot with Be_flg 0 is taken (0xff when none). */
	n = 96;
	while (n != 0) {
		n--;
		if (Esp[n].Be_flg == 0) {
			goto found;
		}
	}
	return 0xff;
found:
	slot = n;
	pParent = &Esp[slot];
	pParent->Be_flg = 0xa003;
	pParent->Wk_no = 0;
	pParent->Padd0 = 0;
	ESP_WORD(pParent->Id) = id + ((dir_y_id2 << 8) & 0xffff) + (tp << 16);	/* Id, Id2 = 0, Type, Mov_cnt = 0 */
	*(USHORT *)&pParent->Anm_time = ((ESP_DATA_HDR *)pData)->Anmseq[0].Time;
	pParent->DirY = dir_y_id2 >> 16;
	ESP_WORD(pParent->Mov_x) = 0;
	ESP_WORD(pParent->Mov_z) = (ULONG)((ESP_DATA_HDR *)pData)->Tpage << 16;
	if (svec != NULL) {
		ESP_WORD(pParent->Hot_x) = ((ULONG *)svec)[0];
		ESP_WORD(pParent->Hot_z) = ((ULONG *)svec)[1];
	} else {
		ESP_WORD(pParent->Hot_x) = 0;
		ESP_WORD(pParent->Hot_z) = 0;
	}
	ESP_WORD(pParent->Pos_x) = 0;
	ESP_WORD(pParent->Pos_z) = bit << 16;	/* Pos_z = 0, Mag = low half of bit */
	ESP_WORD(pParent->Old_x) = 0;
	ESP_WORD(pParent->Old_z) = 0x10000;	/* Old_z = 0, Padd1 = 1 */
	ESP_WORD(pParent->Cdir_x) = 0;
	ESP_WORD(pParent->Cdir_z) = 0;		/* Cdir_z = 0, Padd2 = 0 */
	pParent->Clut = (USHORT)(((ESP_DATA_HDR *)pData)->Clut + (tp >> 3) * 0x40);
	*(MATRIX *)pParent->m = *pmat;
	pParent->pMat = pmat;
	pParent->pAnmseq = (SSEQUENCE *)((ESP_DATA_HDR *)pData)->Anmseq;
	pParent->pSprg = (SPRITEGp *)((UCHAR *)pData + sprg_ofs * 2);
	pParent->pMoveseq = (MSEQUENCE *)(pMove + 1);
	ESP_MSEQ_COPY(pParent);

	/* Children: one per remaining move group; the free-slot search continues downward from the parent's slot (0xfe when
	 * the pool runs out). Be_flg and Padd0 are stored first and overwritten by the body copy, as in DS. */
	while (--nseq != 0) {
		while (n != 0) {
			n--;
			if (Esp[n].Be_flg == 0) {
				goto found_child;
			}
		}
		return 0xfe;
found_child:
		pChild = &Esp[n];
		pChild->Be_flg = 0xa003;
		pChild->Padd0 = 0;
		*(ESP_BODY_W *)&pChild->Be_flg = *(ESP_BODY_W *)&Esp[slot].Be_flg;
		pChild->Wk_no = (UCHAR)slot;
		pGrp = pMove;
		j = nseq;
		do {
			j--;
			pGrp += (*pGrp & 0xffff) * 6 + 1;
		} while (j != 0);
		pGrp++;
		pChild->pMoveseq = (MSEQUENCE *)pGrp;
		ESP_MSEQ_COPY(pChild);
	}
	return (UCHAR)slot;
}


/* ------------------------------------------------------------------ */
/* Esp_call3 - Spawn with Be_flg 0x4000 (DS 8001cbe8)                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001cbe8 (Esp_call3) */
UCHAR Esp_call3(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec)
{
	ULONG id = bit >> 24;
	ULONG tp = (bit >> 16) & 0xff;
	ULONG *pData;
	ULONG *pMove;
	ULONG nseq;
	ULONG sprg_ofs;
	ESP_WORK *pParent;
	ESP_WORK *pChild;
	ULONG *pGrp;
	LONG slot;
	LONG n;
	ULONG j;

	pData = pEspdt[id];
	pMove = pEspmv[id];
	pMove = (ULONG *)((UCHAR *)pMove + 4 * (*(USHORT *)((UCHAR *)pMove + 2 * (tp & 7))));
	nseq = *pMove & 0xffff;
	pMove = pMove + 1;				/* DS steps past the group count word at once */
	sprg_ofs = (*pData & 0xffff) * 4 + 4;	/* the sprite group list follows the header at pData + sprg_ofs halfwords */

	/* Free-slot search from the top: n counts down from 96 and the first slot with Be_flg 0 is taken (0xff when none). */
	n = 96;
	while (n != 0) {
		n--;
		if (Esp[n].Be_flg == 0) {
			goto found;
		}
	}
	return 0xff;
found:
	slot = n;
	pParent = &Esp[slot];
	pParent->Be_flg = 0x4000;
	pParent->Wk_no = 0;
	pParent->Padd0 = 0;
	ESP_WORD(pParent->Id) = (tp << 16) + id;	/* Id, Id2 = 0, Type, Mov_cnt = 0 */
	*(USHORT *)&pParent->Anm_time = ((ESP_DATA_HDR *)pData)->Anmseq[0].Time;
	pParent->DirY = dir_y;
	ESP_WORD(pParent->Mov_x) = 0;
	ESP_WORD(pParent->Mov_z) = (ULONG)((ESP_DATA_HDR *)pData)->Tpage << 16;
	if (svec != NULL) {
		ESP_WORD(pParent->Hot_x) = ((ULONG *)svec)[0];
		ESP_WORD(pParent->Hot_z) = ((ULONG *)svec)[1];
	} else {
		ESP_WORD(pParent->Hot_x) = 0;
		ESP_WORD(pParent->Hot_z) = 0;
	}
	ESP_WORD(pParent->Pos_x) = 0;
	ESP_WORD(pParent->Pos_z) = bit << 16;	/* Pos_z = 0, Mag = low half of bit */
	ESP_WORD(pParent->Old_x) = 0;
	ESP_WORD(pParent->Old_z) = 0x10000;	/* Old_z = 0, Padd1 = 1 */
	ESP_WORD(pParent->Cdir_x) = 0;
	ESP_WORD(pParent->Cdir_z) = 0;		/* Cdir_z = 0, Padd2 = 0 */
	pParent->Clut = (USHORT)(((ESP_DATA_HDR *)pData)->Clut + (tp >> 3) * 0x40);
	*(MATRIX *)pParent->m = *pmat;
	pParent->pMat = pmat;
	pParent->pAnmseq = (SSEQUENCE *)((ESP_DATA_HDR *)pData)->Anmseq;
	pParent->pSprg = (SPRITEGp *)((UCHAR *)pData + sprg_ofs * 2);
	pParent->pMoveseq = (MSEQUENCE *)(pMove + 1);
	ESP_MSEQ_COPY(pParent);

	/* Children: one per remaining move group; the free-slot search continues downward from the parent's slot (0xfe when
	 * the pool runs out). Be_flg and Padd0 are stored first and overwritten by the body copy, as in DS. */
	while (--nseq != 0) {
		while (n != 0) {
			n--;
			if (Esp[n].Be_flg == 0) {
				goto found_child;
			}
		}
		return 0xfe;
found_child:
		pChild = &Esp[n];
		pChild->Be_flg = 0x4000;
		pChild->Padd0 = 0;
		*(ESP_BODY_W *)&pChild->Be_flg = *(ESP_BODY_W *)&Esp[slot].Be_flg;
		pChild->Wk_no = (UCHAR)slot;
		pGrp = pMove;
		j = nseq;
		do {
			j--;
			pGrp += (*pGrp & 0xffff) * 6 + 1;
		} while (j != 0);
		pGrp++;
		pChild->pMoveseq = (MSEQUENCE *)pGrp;
		ESP_MSEQ_COPY(pChild);
	}
	return (UCHAR)slot;
}



/* ------------------------------------------------------------------ */
/* Esp_kill                                                             */
/* Kill all ESP slots matching id, tp (and optionally pmat).           */
/* params: id, tp (unnamed second UCHAR in beta log), pmat (Ghidra)    */
/* ------------------------------------------------------------------ */
/* DS 0x8001cefc (Esp_kill) */
UCHAR Esp_kill(UCHAR id, UCHAR tp, MATRIX *pmat)
{
	ULONG i;
	ESP_WORK *pEsp;

	/* pEsp is taken as &Esp[i] each pass, as in DS. DS has no return statement: v0 holds the last loop test (0), and no
	 * caller uses the value. */
	if (pmat != NULL) {
		for (i = 0; i < 96; i++) {
			pEsp = &Esp[i];
			if (pEsp->Id == id && pEsp->Type == tp && pEsp->pMat == pmat) {
				pEsp->Routine1 = 0;
				pEsp->Routine0 = 0;
				pEsp->Be_flg = 0;
			}
		}
	} else {
		for (i = 0; i < 96; i++) {
			pEsp = &Esp[i];
			if (pEsp->Id == id && pEsp->Type == tp) {
				pEsp->Routine1 = 0;
				pEsp->Routine0 = 0;
				pEsp->Be_flg = 0;
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_kill2                                                            */
/* Kill all ESP slots whose ID byte matches id2.                       */
/* params: id2                                                          */
/* ------------------------------------------------------------------ */
/* DS 0x8001cfbc (Esp_kill2) */
UCHAR Esp_kill2(ULONG id2)
{
	UCHAR last;
	ULONG i;
	ESP_WORK *pEsp;

	/* The return value is the last Id2 read (DS leaves it in v0). */
	for (i = 0; i < 96; i++) {
		pEsp = &Esp[i];
		last = pEsp->Id2;
		if (last == id2) {
			pEsp->Routine1 = 0;
			pEsp->Routine0 = 0;
			pEsp->Be_flg = 0;
		}
	}
	return last;
}

/* ------------------------------------------------------------------ */
/* Esp_die                                                              */
/* Kill all active ESP slots that are not in a "dying" state.          */
/* ------------------------------------------------------------------ */
/* DS 0x8001cffc (Esp_die) */
void Esp_die(void)
{
	ULONG i;
	ESP_WORK *pEsp;
	BOOL kill;

	for (i = 0; i < 96; i++) {
		pEsp = &Esp[i];
		/* Id 0 is never killed; Id 5 keeps Type 2 and Type 3 alive. */
		kill = 1;
		if (pEsp->Id == 0) {
			kill = 0;
		} else if (pEsp->Id == 5) {
			if (pEsp->Type == 2) {
				kill = 0;
			}
			if (pEsp->Type == 3) {
				kill = 0;
			}
		}
		if (kill) {
			pEsp->Routine1 = 0;
			pEsp->Routine0 = 0;
			pEsp->Be_flg = 0;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_kill_all - Clear Routine0/Routine1/Be_flg of all 96 slots         */
/*                (DS 8001d07c)                                         */
/* ------------------------------------------------------------------ */
/* DS 0x8001d07c (Esp_kill_all) */
void Esp_kill_all(void)
{
	ESP_WORK *pEsp = Esp;
	LONG i;

	for (i = 0; i < 96; i++) {
		pEsp->Routine1 = 0;
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		pEsp++;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_ctrl                                                             */
/* Set Routine0 for all ESP slots matching id, tp (and optionally      */
/* pmat).                                                              */
/* params: id, tp, rtn, pmat                                           */
/* ------------------------------------------------------------------ */
/* DS 0x8001d0ac (Esp_ctrl) */
UCHAR Esp_ctrl(UCHAR id, UCHAR tp, USHORT rtn, MATRIX *pmat)
{
	ULONG i;
	ESP_WORK *pEsp;

	/* pEsp is taken as &Esp[i] each pass, as in DS. DS has no return statement: v0 holds the last loop test (0), and no
	 * caller uses the value. */
	if (pmat != NULL) {
		for (i = 0; i < 96; i++) {
			pEsp = &Esp[i];
			if (pEsp->Id == id && pEsp->Type == tp && pEsp->pMat == pmat) {
				pEsp->Routine0 = (UCHAR)rtn;
			}
		}
	} else {
		for (i = 0; i < 96; i++) {
			pEsp = &Esp[i];
			if (pEsp->Id == id && pEsp->Type == tp) {
				pEsp->Routine0 = (UCHAR)rtn;
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_ctrl2                                                            */
/* Set Routine0 for a single ESP slot by work-number index.            */
/* params: wk_no, rtn                                                  */
/* ------------------------------------------------------------------ */
/* DS 0x8001d15c (Esp_ctrl2) */
ULONG Esp_ctrl2(USHORT wk_no, USHORT rtn)
{
	Esp[wk_no].Routine0 = (UCHAR)rtn;
	return wk_no * sizeof(ESP_WORK);	/* DS leaves wk_no * 0x7c in v0 without truncation (no caller in the project) */
}

/* ------------------------------------------------------------------ */
/* Esp_ctrl3                                                            */
/* Set Transx/Free2 (X magnitude/sine) for all matching ESP slots.    */
/* params: id, tp, magx, sinx, pmat                                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001d180 (Esp_ctrl3) */
UCHAR Esp_ctrl3(UCHAR id, UCHAR tp, USHORT magx, USHORT sinx, MATRIX *pmat)
{
	BOOL moreSlots;
	ESP_WORK *pEsp;
	UINT idx;

	idx = 0;
	if (pmat != (MATRIX *)0x0) {
		pEsp = Esp;
		do {
			if (((pEsp->Id == id) && (pEsp->Type == tp)) && (pEsp->pMat == pmat)) {
				pEsp->Transx = magx;
				pEsp->Free2 = (UCHAR)sinx;
			}
			idx++;
			moreSlots = idx < 0x60;
			pEsp++;
		} while (moreSlots);
	} else {
		pEsp = Esp;
		do {
			if ((pEsp->Id == id) && (pEsp->Type == tp)) {
				pEsp->Transx = magx;
				pEsp->Free2 = (UCHAR)sinx;
			}
			idx++;
			moreSlots = idx < 0x60;
			pEsp++;
		} while (moreSlots);
	}
	return moreSlots;
}

/* ------------------------------------------------------------------ */
/* Esp_ctrl4                                                            */
/* Set Transy/Free3 (Y magnitude/sine) for all matching ESP slots.    */
/* params: id, tp, magy, siny, pmat                                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001d240 (Esp_ctrl4) */
UCHAR Esp_ctrl4(UCHAR id, UCHAR tp, USHORT magy, USHORT siny, MATRIX *pmat)
{
	BOOL moreSlots;
	ESP_WORK *pEsp;
	UINT idx;

	idx = 0;
	if (pmat != (MATRIX *)0x0) {
		pEsp = Esp;
		do {
			if (((pEsp->Id == id) && (pEsp->Type == tp)) && (pEsp->pMat == pmat)) {
				pEsp->Transy = magy;
				pEsp->Free3 = siny;
			}
			idx++;
			moreSlots = idx < 0x60;
			pEsp++;
		} while (moreSlots);
	} else {
		pEsp = Esp;
		do {
			if ((pEsp->Id == id) && (pEsp->Type == tp)) {
				pEsp->Transy = magy;
				pEsp->Free3 = siny;
			}
			idx++;
			moreSlots = idx < 0x60;
			pEsp++;
		} while (moreSlots);
	}
	return moreSlots;
}

/* ------------------------------------------------------------------ */
/* Esp_move                                                             */
/* Per-frame ESP update: tick all active slots via Esp_tblj dispatch. */
/* Respects G.Stop_flg to restrict which types are updated when        */
/* the game is paused.                                                 */
/* ------------------------------------------------------------------ */
/* DS 0x8001d300 (Esp_move) */
void Esp_move(void)
{
	/* DS keeps &Esp[0] and &Esp[96] in saved registers for the four slot loops (do-while loops, no entry test), and
	 * uses pEsp as a slot pointer read once, so stores through it do not make DS reload pESP. */
	ESP_WORK *pEsp;
	ESP_WORK *pBeg = Esp;
	ESP_WORK *pEnd = &Esp[96];

	if (G.Stop_flg & 0x10000000) {
		/* Paused: only the Id classes 0x1a, 0x1c, 0x28, 0x15, 0x16 (and 1 in pass 1) run. */
		pESP = pBeg;
		do {
			if (pESP->Id == 0x1a || pESP->Id == 0x1c || pESP->Id == 0x28 || pESP->Id == 0x15 || pESP->Id == 0x16 || pESP->Id == 1) {
				pEsp = pESP;
				if ((pEsp->Be_flg & 0x8000) != 0) {
					if (pEsp->Wk_no != 0 && Esp[pEsp->Wk_no].Be_flg == 0) {
						pEsp->Routine1 = 0;
						pEsp->Routine0 = 0;
						pEsp->Be_flg = 0;
					}
					ESP_TBLJ_CALL(pESP->Routine0);
				}
			}
		} while (++pESP < pEnd);
		pESP = pBeg;
		do {
			if (pESP->Id == 0x1a || pESP->Id == 0x1c || pESP->Id == 0x28 || pESP->Id == 0x15 || pESP->Id == 0x16) {
				pEsp = pESP;
				if ((pEsp->Be_flg & 0x4000) != 0) {
					pEsp->Be_flg = 0xa003;
					ESP_TBLJ_CALL(pEsp->Routine0);
				}
				if ((pESP->Be_flg & 0x8000) != 0) {
					Esp_pos(0);
				}
			}
		} while (++pESP < pEnd);
	} else {
		/* Pass 1: slots with Be_flg bit 15 run Routine0 (clear if the parent is dead). */
		pESP = pBeg;
		do {
			pEsp = pESP;
			if ((pEsp->Be_flg & 0x8000) != 0) {
				if (pEsp->Wk_no != 0 && Esp[pEsp->Wk_no].Be_flg == 0) {
					pEsp->Routine1 = 0;
					pEsp->Routine0 = 0;
					pEsp->Be_flg = 0;
				}
				ESP_TBLJ_CALL(pESP->Routine0);
			}
		} while (++pESP < pEnd);
		/* Pass 2: bit 14 runs Routine0 with Be_flg = 0xa003; bit 15 runs Esp_pos. */
		pESP = pBeg;
		do {
			if ((pESP->Be_flg & 0x4000) != 0) {
				pESP->Be_flg = 0xa003;
				ESP_TBLJ_CALL(pESP->Routine0);
			}
			if ((pESP->Be_flg & 0x8000) != 0) {
				Esp_pos(0);
			}
		} while (++pESP < pEnd);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_pos                                                              */
/* Update position/frame for the current pESP slot.                   */
/* Dispatches move and draw routines, advances velocity, ticks frame   */
/* counter, and handles move-sequence branching.                       */
/* ------------------------------------------------------------------ */
/* DS 0x8001d68c (Esp_pos) */
void Esp_pos(LONG Run_rtn0)
{
	ESP_WORK *pEsp;
	SSEQUENCE *ent;

	if (Run_rtn0 != 0) {
		ESP_TBLJ_CALL(pESP->Routine0);
	}
	Esp_pos_matrix();
	ESP_TBLJ_CALL(pESP->Routine1);

	if ((pESP->Be_flg & 2) != 0) {
		pESP->Mov_x = (SHORT)(pESP->Mov_x + pESP->Speed_x);
		pESP->Mov_y = (SHORT)(pESP->Mov_y + pESP->Speed_y);
		pESP->Mov_z = (SHORT)(pESP->Mov_z + pESP->Speed_z);
		pESP->Speed_x = (SHORT)(pESP->Speed_x + (signed char)pESP->Add_x);
		pESP->Speed_y = (SHORT)(pESP->Speed_y + (signed char)pESP->Add_y);
		pESP->Speed_z = (SHORT)(pESP->Speed_z + (signed char)pESP->Add_z);
	}

	if ((pESP->Be_flg & 1) != 0) {
		if (pESP->Anm_time == 0) {
			pESP->Anm_cnt = (UCHAR)(pESP->Anm_cnt + (pESP->Be_flg & 1));
			pEsp = pESP;
			ent = &pEsp->pAnmseq[pEsp->Anm_cnt];
			if (ent->Time == 0) {
				if (ent->pGp == 0) {
					pEsp->Routine1 = 0;
					pEsp->Routine0 = 0;
					pEsp->Be_flg = 0;
				}
			} else if (ent->Time == 0xff) {
				pESP->Anm_cnt = ent->pGp;
				ent = &pESP->pAnmseq[pESP->Anm_cnt];
			}
			pESP->Anm_time = ent->Time;
		}
		pESP->Anm_time = (UCHAR)(pESP->Anm_time - 1);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_pos_matrix - Save Pos to Old, copy pMat when Be_flg & 0x800, then */
/*                  place the effect from Mov / Hot / t[] through the    */
/*                  GTE (Be_flg & 0x400 selects the unrotated branch)  */
/*                  (DS 8001d894)                                      */
/* ------------------------------------------------------------------ */
/* DS 0x8001d894 (Esp_pos_matrix) */

void Esp_pos_matrix(void)
{
	if ((pESP->Be_flg & 0x800) != 0) {
		*(MATRIX *)pESP->m = *pESP->pMat;
	}

	ESP_WORD(pESP->Old_x) = ESP_WORD(pESP->Pos_x);
	pESP->Old_z = pESP->Pos_z;

	if (pESP->Be_flg & 0x400) {
		*ESP_PM_MATW = GsIDMATRIX;
		gte_SetRotMatrix(ESP_PM_MATW);
		gte_ldv0((SVECTOR *)&pESP->Mov_x);
		gte_rtv0();
		gte_stlvnl(ESP_PM_VRES);
		ESP_PM_TMP->vx = (SHORT)(pESP->Hot_x + ESP_PM_VRES->vx);
		ESP_PM_TMP->vy = (SHORT)(pESP->Hot_y + ESP_PM_VRES->vy);
		ESP_PM_TMP->vz = (SHORT)(pESP->Hot_z + ESP_PM_VRES->vz);

		gte_SetRotMatrix((MATRIX *)pESP->m);
		gte_ldv0(ESP_PM_TMP);
		gte_rtv0();
		gte_stlvnl(ESP_PM_VRES);
		pESP->Pos_x = (SHORT)(pESP->t[0] + ESP_PM_VRES->vx);
		pESP->Pos_y = (SHORT)(pESP->t[1] + ESP_PM_VRES->vy);
		pESP->Pos_z = (SHORT)(pESP->t[2] + ESP_PM_VRES->vz);
	} else {
		RotMatrixY(pESP->DirY, ESP_PM_MATW);
		gte_SetRotMatrix(ESP_PM_MATW);
		gte_ldv0((SVECTOR *)&pESP->Mov_x);
		gte_rtv0();
		gte_stlvnl(ESP_PM_VRES);
		pESP->Pos_x = (SHORT)ESP_PM_VRES->vx;
		pESP->Pos_y = (SHORT)ESP_PM_VRES->vy;
		pESP->Pos_z = (SHORT)ESP_PM_VRES->vz;

		gte_SetRotMatrix((MATRIX *)pESP->m);
		gte_ApplyRotMatrix((SVECTOR *)&pESP->Hot_x, ESP_PM_VRES);
		pESP->Pos_x = (SHORT)(pESP->Pos_x + (pESP->t[0] + ESP_PM_VRES->vx));
		pESP->Pos_y = (SHORT)(pESP->Pos_y + (pESP->t[1] + ESP_PM_VRES->vy));
		pESP->Pos_z = (SHORT)(pESP->Pos_z + (pESP->t[2] + ESP_PM_VRES->vz));
	}
}
