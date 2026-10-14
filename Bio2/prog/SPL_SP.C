/*
 * SPL_SP.C - Spl enemy special-state dispatch and move routines
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_SP.C
 */
#include "TYPES.H"
#include "MAIN.H"
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <MEMORY.H>
#include "SPL_SP.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "SABUN.H"

/* SA frame head at PARTS_W.pSa_dat_head (ULONG words). Words 0..2 are not accessed by these functions.
 * Words 3 and up are Blk[k]: low half Weight, high half Hi. Retail stores the low halves of Blk[0] and
 * Blk[1] with sh at 0x80067b64 (0xc(v0), in Spl_sp44_00) and sh at 0x80067224 (0x10(v0), in Spl_sp40_00). */
typedef struct {
	ULONG Hdr[3];
	struct {
		SHORT Weight;
		SHORT Hi;
	} Blk[2];
} SA_HEAD;

/* file-local prototypes */
static VOID Spl_sp40_00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);
static VOID Spl_sp44_00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* forward declarations (definitions are in retail address order) */
static VOID *Spl_sp[24][8];

/* DS 0x80067004 (Spl_Sp_Move) */
VOID Spl_Sp_Move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	((CODE_PTR)(Spl_sp[pEm->Id - 0x40][pEm->Routine_1]))(pEm, Kan, Seq);
}

/* DS 0x8006704c (Spl_sp40_00) */
static VOID Spl_sp40_00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;
	UCHAR Kuchi3[33];
	UCHAR Kuchi4[12];
	UCHAR Kubi3[14];
	UCHAR Kubi4[5];
	ULONG *Kan_t_ptr;
	ULONG *Seq_t_ptr;

	memcpy(Kuchi3, Spl_sp40_kuchi3_rom, sizeof(Kuchi3));
	memcpy(Kuchi4, Spl_sp40_kuchi4_rom, sizeof(Kuchi4));
	memcpy(Kubi3, Spl_sp40_kubi3_rom, sizeof(Kubi3));
	memcpy(Kubi4, Spl_sp40_kubi4_rom, sizeof(Kubi4));

	Kan_t_ptr = pEm->pRoom_kan_t_ptr;
	Seq_t_ptr = pEm->pRoom_seq_t_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 0xf;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->pSin_parts_ptr[0x10].Be_flg |= 1;
		pP_ptr = pEm->pSin_parts_ptr;
		((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight = 0x200;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr;
		((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight = 0x200;
		Sa_parts_allmv(pEm, pP_ptr + 0x10);
		Sv0.vx = 200;
		Sv0.vy = -400;
		Sv0.vz = -200;
		Esp_call(6000, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, &Sv0);
		Sv0.vx = 0x96;
		Sv0.vy = -0x15e;
		Sv0.vz = -200;
		Esp_call(6000, pEm->Cdir_y + 0x800, &pEm->pSin_parts_ptr->Workm, &Sv0);
		pEm->Timer0 = 0;
		pEm->Timer2 = 5;
		pEm->Timer3 = 0;
		pEm->pSin_parts_ptr[9].Be_flg |= 0x8000;
		pEm->pSin_parts_ptr[10].Be_flg |= 0x8000;
		pEm->pSin_parts_ptr[0xb].Be_flg |= 0x8000;
		/* fall through */
	case 1:
		if (pEm->Move_cnt < 0x2a) {
			if ((pEm->Move_cnt & 3) == 0) {
				Sv0.vx = 200;
				Sv0.vy = pEm->Timer0 * 0x1e - 400;
				Sv0.vz = pEm->Timer0 * 0xf - 200;
				Esp_call(0x10bb8, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, &Sv0);
				Sv0.vx = 100;
				Sv0.vy = pEm->Timer0 * 0x19 - 0x15e;
				Sv0.vz = pEm->Timer0 * 0xf - 200;
				Esp_call(5000, pEm->Cdir_y + 0x800, &pEm->pSin_parts_ptr->Workm, &Sv0);
				pEm->Timer0++;
			}
			if (pEm->Move_cnt < 0x2a) {
				if (pEm->Timer2 < 4) {
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight += -0x50;
					Sa_parts_allmv(pEm, pP_ptr);
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight += -0x50;
				} else {
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight += 200;
					Sa_parts_allmv(pEm, pP_ptr);
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight += 200;
				}
				Sa_parts_allmv(pEm, pP_ptr + 0x10);
				if (pEm->Timer2-- == 0) {
					pEm->Timer2 = 5;
				}
			}
		}
		if (pEm->Move_cnt == 0x28) {
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		}
		if (pEm->Move_cnt == 0x32) {
			Texture_Move_GT3((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, Kuchi3);
			Texture_Move_GT4((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0xc0fffa, pEm->Clut + 0x1e2, Kuchi4);
			Texture_Move_GT3((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x2f0000, pEm->Clut + 0x1e2, Kubi3);
			Texture_Move_GT4((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x2f0000, pEm->Clut + 0x1e2, Kubi4);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[10].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xd].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		}
		if ((0x32 < pEm->Move_cnt) && (pEm->Timer3 == 0)) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight += 200;
			if (0x1000 < ((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight) {
				((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight = 0x1000;
				pEm->Timer3 = 1;
			}
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight += 200;
			if (0x1000 < ((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight) {
				((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight = 0x1000;
			}
			Sa_parts_allmv(pEm, pP_ptr + 0x10);
		}
		if (((UINT)(pEm->Move_cnt - 0x24) < 0x13) && ((pEm->Move_cnt & 3) == 0)) {
			Esp_call(0x21770, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, (SVECTOR *)0);
		}
		if (pEm->Move_cnt == 70) {
			G.pEnemy[3]->Routine_2 = 5;
		}
		pEm->Routine_2 += (CHAR)Joint_move(pEm, (ULONG)Kan_t_ptr, (ULONG)Seq_t_ptr, 0x100);
		break;
	case 2:
		pEm->Timer0 = 0x96;
		pEm->Routine_2 = 3;
		/* fall through */
	case 3:
		if (pEm->Timer0 == 0x82) {
			G.pEnemy[3]->Routine_2++;
		}
		if ((UINT)(USHORT)pEm->Timer0 - 0x78 < 10) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight += 100;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight += 100;
			Sa_parts_allmv(pEm, pP_ptr + 0x10);
		}
		if ((UINT)(USHORT)pEm->Timer0 - 0x72 < 5) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight -= 200;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight -= 200;
			Sa_parts_allmv(pEm, pP_ptr + 0x10);
		}
		if ((UINT)(USHORT)pEm->Timer0 - 0x67 < 10) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight += 0x50;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight += 0x50;
			Sa_parts_allmv(pEm, pP_ptr + 0x10);
		}
		if ((UINT)(USHORT)pEm->Timer0 - 0x61 < 5) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[1].Weight -= 0xa0;
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0x10].pSa_dat_head)->Blk[1].Weight -= 0xa0;
			Sa_parts_allmv(pEm, pP_ptr + 0x10);
		}
		if ((UINT)(USHORT)pEm->Timer0 - 0x74 < 0x18) {
			if ((pEm->Timer0 & 7) == 0) {
				Esp_call(0x20fa0, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, (SVECTOR *)0);
			}
		}
		if (pEm->Timer0-- == 0) {
			pEm->Routine_2 = 4;
			G.pEnemy[3]->Routine_2++;
		}
		break;
	default:
		break;
	}
}

/* DS 0x80011a58: count 0x20, then 32 poly ids (copied to local Kuchi3 in Spl_sp40_00) */
const UCHAR Spl_sp40_kuchi3_rom[33] = {
	0x20, 0x01, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x15,
	0x16, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x25, 0x26, 0x27, 0x28, 0x29,
	0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
	0x43
};

/* DS 0x80011a7c: count 0x0b, then 11 poly ids (copied to local Kuchi4 in Spl_sp40_00) */
const UCHAR Spl_sp40_kuchi4_rom[12] = {
	0x0b, 0x08, 0x0a, 0x0e, 0x10, 0x11, 0x12, 0x13,
	0x15, 0x16, 0x1e, 0x25
};

/* DS 0x80011a88: count 0x0d, then 13 poly ids (copied to local Kubi3 in Spl_sp40_00) */
const UCHAR Spl_sp40_kubi3_rom[14] = {
	0x0d, 0x06, 0x0b, 0x0e, 0x0f, 0x10, 0x17, 0x18,
	0x24, 0x36, 0x37, 0x3d, 0x3e, 0x42
};

/* DS 0x80011a98: count 0x04, then 4 poly ids (copied to local Kubi4 in Spl_sp40_00) */
const UCHAR Spl_sp40_kubi4_rom[5] = {
	0x04, 0x03, 0x05, 0x0f, 0x23
};

/* DS 0x80011ab4: count 0x19, then 25 poly ids (copied to local Kuchi3 in Spl_sp44_00) */
const UCHAR Spl_sp44_kuchi3_rom[26] = {
	0x19, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
	0x0b, 0x10, 0x12, 0x13, 0x16, 0x21, 0x22, 0x23,
	0x25, 0x28, 0x29, 0x2c, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39
};

/* DS 0x80011ad0: count 0x05, then 5 poly ids (copied to local Kuchi4 in Spl_sp44_00) */
const UCHAR Spl_sp44_kuchi4_rom[6] = {
	0x05, 0x12, 0x13, 0x14, 0x15, 0x2d
};

/* DS 0x80011ad8: count 0x03, then 3 poly ids (copied to local Kubi3 in Spl_sp44_00) */
const UCHAR Spl_sp44_kubi3_rom[4] = {
	0x03, 0x0d, 0x0e, 0x17
};

/* DS 0x80011adc: count 0x04, then 4 poly ids (copied to local Kubi4 in Spl_sp44_00) */
const UCHAR Spl_sp44_kubi4_rom[5] = {
	0x04, 0x08, 0x19, 0x24, 0x32
};

/* DS 0x80011ae4: count 0x08, then 8 poly ids (copied to local Me3 in Spl_sp44_00) */
const UCHAR Spl_sp44_me3_rom[9] = {
	0x08, 0x11, 0x14, 0x1c, 0x27, 0x2a, 0x31, 0x32,
	0x33
};

/* DS 0x80011af0: count 0x0d, then 13 poly ids (copied to local Me4 in Spl_sp44_00) */
const UCHAR Spl_sp44_me4_rom[14] = {
	0x0d, 0x05, 0x0c, 0x0d, 0x0e, 0x18, 0x28, 0x29,
	0x2f, 0x30, 0x33, 0x34, 0x36, 0x37
};

/* DS 0x800a8f64 (Spl_sp), indexed [Id - 0x40][Routine_1] */
static VOID *Spl_sp[24][8] = {
	{ (VOID *)Spl_sp40_00, 0, 0, 0, 0, 0, 0, 0 },	/* Id 0x40 */
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ (VOID *)Spl_sp44_00, 0, 0, 0, 0, 0, 0, 0 },	/* Id 0x44 */
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ (VOID *)Spl_sp44_00, 0, 0, 0, 0, 0, 0, 0 },	/* Id 0x46 */
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* DS 0x80067984 (Spl_sp44_00) */
static VOID Spl_sp44_00(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv0;
	UCHAR Kuchi3[26];
	UCHAR Kuchi4[6];
	UCHAR Kubi3[4];
	UCHAR Kubi4[5];
	UCHAR Me3[9];
	UCHAR Me4[14];
	ULONG *Kan_t_ptr;
	ULONG *Seq_t_ptr;

	memcpy(Kuchi3, Spl_sp44_kuchi3_rom, sizeof(Kuchi3));
	memcpy(Kuchi4, Spl_sp44_kuchi4_rom, sizeof(Kuchi4));
	memcpy(Kubi3, Spl_sp44_kubi3_rom, sizeof(Kubi3));
	memcpy(Kubi4, Spl_sp44_kubi4_rom, sizeof(Kubi4));
	memcpy(Me3, Spl_sp44_me3_rom, sizeof(Me3));
	memcpy(Me4, Spl_sp44_me4_rom, sizeof(Me4));

	Kan_t_ptr = pEm->pRoom_kan_t_ptr;
	Seq_t_ptr = pEm->pRoom_seq_t_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		pEm->Hokan_flg = 0xf;
		pEm->Mplay_flg = 0;
		pEm->Routine_2 = 1;
		pEm->pSin_parts_ptr[0xf].Be_flg |= 1;
		pP_ptr = pEm->pSin_parts_ptr;
		((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight = 0x200;
		Sa_parts_allmv(pEm, pP_ptr);
		pP_ptr = pEm->pSin_parts_ptr;
		((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight = 0x200;
		Sa_parts_allmv(pEm, pP_ptr + 0xf);
		Sv0.vx = 200;
		Sv0.vy = -400;
		Sv0.vz = -200;
		Esp_call(6000, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, &Sv0);
		Sv0.vx = 0x96;
		Sv0.vy = -0x15e;
		Sv0.vz = -200;
		Esp_call(6000, pEm->Cdir_y + 0x800, &pEm->pSin_parts_ptr->Workm, &Sv0);
		pEm->Timer0 = 0;
		pEm->Timer2 = 5;
		pEm->Timer3 = 0;
		/* fall through */
	case 1:
		if (pEm->Move_cnt < 0x2a) {
			if ((pEm->Move_cnt & 3) == 0) {
				Sv0.vx = 200;
				Sv0.vy = pEm->Timer0 * 0x1e - 400;
				Sv0.vz = pEm->Timer0 * 0xf - 200;
				Esp_call(0x10bb8, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, &Sv0);
				Sv0.vx = 100;
				Sv0.vy = pEm->Timer0 * 0x19 - 0x15e;
				Sv0.vz = pEm->Timer0 * 0xf - 200;
				Esp_call(5000, pEm->Cdir_y + 0x800, &pEm->pSin_parts_ptr->Workm, &Sv0);
				pEm->Timer0++;
			}
			if (pEm->Move_cnt < 0x2a) {
				if (pEm->Timer2 < 4) {
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight += -0x50;
					Sa_parts_allmv(pEm, pP_ptr);
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight += -0x50;
				} else {
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight += 200;
					Sa_parts_allmv(pEm, pP_ptr);
					pP_ptr = pEm->pSin_parts_ptr;
					((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight += 200;
				}
				Sa_parts_allmv(pEm, pP_ptr + 0xf);
				if (pEm->Timer2-- == 0) {
					pEm->Timer2 = 5;
				}
			}
		}
		if (pEm->Move_cnt == 0x28) {
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[9].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr->Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		}
		if (pEm->Move_cnt == 0x32) {
			Texture_Move_GT3((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x850000, pEm->Clut + 0x1e2, Kuchi3);
			Texture_Move_GT4((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x850000, pEm->Clut + 0x1e2, Kuchi4);
			Texture_Move_GT3((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x50, pEm->Clut + 0x1e2, Kubi3);
			Texture_Move_GT4((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x50, pEm->Clut + 0x1e2, Kubi4);
			Texture_Move_GT3((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x380044, pEm->Clut + 0x1e2, Me3);
			Texture_Move_GT4((MODEL_WORK *)&pEm->pSin_parts_ptr[8].Attribute,
							 pEm->Tpage + 2, 0x380044, pEm->Clut + 0x1e2, Me4);
			Texture_Move((MODEL_WORK *)&pEm->pSin_parts_ptr[0xc].Attribute,
						 pEm->Tpage + 2, 0, pEm->Clut + 0x1e2);
		}
		if ((0x32 < pEm->Move_cnt) && (pEm->Timer3 == 0)) {
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight += 200;
			if (0x1000 < ((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight) {
				((SA_HEAD *)pP_ptr->pSa_dat_head)->Blk[0].Weight = 0x1000;
				pEm->Timer3 = 1;
			}
			Sa_parts_allmv(pEm, pP_ptr);
			pP_ptr = pEm->pSin_parts_ptr;
			((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight += 200;
			if (0x1000 < ((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight) {
				((SA_HEAD *)pP_ptr[0xf].pSa_dat_head)->Blk[0].Weight = 0x1000;
			}
			Sa_parts_allmv(pEm, pP_ptr + 0xf);
		}
		if (((UINT)(pEm->Move_cnt - 0x24) < 0x13) && ((pEm->Move_cnt & 3) == 0)) {
			Esp_call(0x21770, pEm->Cdir_y, &pEm->pSin_parts_ptr->Workm, (SVECTOR *)0);
		}
		if (pEm->Move_cnt == 70) {
			G.pEnemy[3]->Routine_2 = 5;
		}
		pEm->Routine_2 += (CHAR)Joint_move(pEm, (ULONG)Kan_t_ptr, (ULONG)Seq_t_ptr, 0x100);
		break;
	case 2:
		pEm->Timer0 = 0x96;
		pEm->Routine_2 = 3;
		break;
	default:
		break;
	}
}
