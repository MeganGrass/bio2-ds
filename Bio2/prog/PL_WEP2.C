/*
 * PL_WEP2.C - Player weapon attack routines (per-weapon, bow gun)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PL_WEP2.C
 */

#include "TYPES.H"
#include "CD.H"
#include "MAIN.H"
#include <LIBCD.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "PL_WEP2.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCA.H"
#include "SCE_AT.H"
#include "SND.H"
#include "STATUS.H"
#include "GAME.H"
#include "SCE2.H"
#include "SUB.H"
#include "flag.h"
#include "DIVCHK.H"


/*
 * pl_mv05
 * line = 18, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043df8 (pl_mv05) */
/* Player weapon-motion dispatcher. Status_flg bits 0xc0 are set first (weapon in use), then the handler
 * for the weapon type (Type & 0xfff) runs. When the player is standing still (Spd_x == 0) a footstep is
 * played (Foot_set_pl); its third argument is bit 7 of Pl_weapon_kyan[Type - 1][Id & 1]. The low 7 bits
 * of the same bytes are the Move_cnt limits that the weapon handlers compare against. */
VOID pl_mv05(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
	G.Status_flg |= 0xc0;
	pl_mv05_w_tbl[(pEm->Type & 0xfff) - 1](pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, G.Key);
	if ((pEm->Spd_x == 0) && ((pEm->Routine_2 != 0 || (pEm->Hokan_flg == 0)))) {
		Foot_set_pl(pEm, 0, (ULONG)(Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][pEm->Id & 1] >> 7));
	}
	return;
}

/*
 * pl_w01_t2_init
 * line = 47, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043ecc (pl_w01_t2_init) */
VOID pl_w01_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w02_t2_init
 * line = 51, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043ed4 (pl_w02_t2_init) */
/* Init for weapon type 2: calls Tama_decrease_i for this weapon type. */
VOID pl_w02_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w03_t2_init
 * line = 57, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043efc (pl_w03_t2_init) */
VOID pl_w03_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w04_t2_init
 * line = 62, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043f24 (pl_w04_t2_init) */
VOID pl_w04_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w05_t2_init
 * line = 69, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043f2c (pl_w05_t2_init) */
VOID pl_w05_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w06_t2_init
 * line = 74, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043f54 (pl_w06_t2_init) */
VOID pl_w06_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w07_t2_init
 * line = 79, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043f7c (pl_w07_t2_init) */
VOID pl_w07_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w08_t2_init
 * line = 84, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043fa4 (pl_w08_t2_init) */
VOID pl_w08_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w09_t2_init
 * line = 89, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043fcc (pl_w09_t2_init) */
VOID pl_w09_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w0a_t2_init
 * line = 94, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80043ff4 (pl_w0a_t2_init) */
VOID pl_w0a_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w0b_t2_init
 * line = 99, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004401c (pl_w0b_t2_init) */
VOID pl_w0b_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w0c_t2_init
 * line = 104, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044044 (pl_w0c_t2_init) */
VOID pl_w0c_t2_init(ENEMY_WORK *pEm)
{
	G.Room_player_flg &= 0xfffe;
	pl_bow_init(pEm, 0);
	pl_bow_init(pEm, 1);
	pl_bow_init(pEm, 2);
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w0d_t2_init
 * line = 117, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800440cc (pl_w0d_t2_init) */
VOID pl_w0d_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w0e_t2_init
 * line = 122, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800440f4 (pl_w0e_t2_init) */
VOID pl_w0e_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w0f_t2_init
 * line = 127, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800440fc (pl_w0f_t2_init) */
VOID pl_w0f_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w10_t2_init
 * line = 132, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044104 (pl_w10_t2_init) */
VOID pl_w10_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w11_t2_init
 * line = 137, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004410c (pl_w11_t2_init) */
VOID pl_w11_t2_init(ENEMY_WORK *pEm)
{
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	return;
}

/*
 * pl_w12_t2_init
 * line = 142, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044134 (pl_w12_t2_init) */
VOID pl_w12_t2_init(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w01_t2_move
 * line = 148, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004413c (pl_w01_t2_move) */
/* Plays the sound for this motion frame when the frame word has bit 0x100000 set; the sound number is
 * the frame's 0xf0000 nibble on top of the 0x1000001 base. */
VOID pl_w01_t2_move(ENEMY_WORK *pEm)
{
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w02_t2_move
 * line = 157, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004418c (pl_w02_t2_move) */
VOID pl_w02_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x8a;
		esp_vec.vy = 0x267;
		esp_vec.vz = 8;
		Esp_call(0x30a0e00, 0, pMtx, &esp_vec);
		Esp_call(0x1000e00, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 3) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x15e;
		esp_vec.vy = 0x28;
		esp_vec.vz = -0xf;
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x2000700, (SHORT)((USHORT)G.Player.Cdir_y + 0x514), pMtx, &esp_vec);
	}
	return;
}

/*
 * pl_w03_t2_move
 * line = 180, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004429c (pl_w03_t2_move) */
VOID pl_w03_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x8a;
		esp_vec.vy = 0x267;
		esp_vec.vz = 8;
		Esp_call(0x30a0e00, 0, pMtx, &esp_vec);
		Esp_call(0x1000e00, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 3) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0xa0;
		esp_vec.vy = 200;
		esp_vec.vz = 0x3c;
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x2000700, (SHORT)((USHORT)G.Player.Cdir_y + 0x514), pMtx, &esp_vec);
	}
	return;
}

/*
 * pl_w04_t2_move
 * line = 203, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800443ac (pl_w04_t2_move) */
VOID pl_w04_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (((pEm->Move_cnt == 1) || (pEm->Move_cnt == 4)) || (pEm->Move_cnt == 7)) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x78;
		esp_vec.vy = 600;
		esp_vec.vz = -0x8a;
		Esp_call(0x30a0d00, 0, pMtx, &esp_vec);
		Esp_call(0x1000d00, 0, pMtx, &esp_vec);
	}
	if (((pEm->Move_cnt == 3) || (pEm->Move_cnt == 6)) || (pEm->Move_cnt == 10)) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0xa0;
		esp_vec.vy = 200;
		esp_vec.vz = 0x3c;
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x2000700, (SHORT)((USHORT)G.Player.Cdir_y + 0x514), pMtx, &esp_vec);
	}
	return;
}

/*
 * pl_w05_t2_move
 * line = 226, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800444e0 (pl_w05_t2_move) */
VOID pl_w05_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x6e;
		esp_vec.vy = 0x29e;
		esp_vec.vz = -10;
		Esp_call(0x30a1400, 0, pMtx, &esp_vec);
		Esp_call(0x1001d00, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 4) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x14a;
		esp_vec.vy = 0x32;
		esp_vec.vz = 0;
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x2000900, (SHORT)((USHORT)G.Player.Cdir_y + 0x694), pMtx, &esp_vec);
	}
	return;
}

/*
 * pl_w06_t2_move
 * line = 249, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800445ec (pl_w06_t2_move) */
VOID pl_w06_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x82;
		esp_vec.vy = 900;
		esp_vec.vz = -10;
		Esp_call(0x30a1800, 0, pMtx, &esp_vec);
		Esp_call(0x1002000, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x44c;
		Esp_call(0x1023000, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 4) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x14a;
		esp_vec.vy = 0x32;
		esp_vec.vz = 0;
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x3080900, 0, pMtx, &esp_vec);
		Esp_call(0x2000900, (SHORT)((USHORT)G.Player.Cdir_y + 0x694), pMtx, &esp_vec);
	}
	G.Status_flg &= ~STAT_BG_HOLD;
	return;
}

/*
 * pl_w07_t2_move
 * line = 275, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044730 (pl_w07_t2_move) */
VOID pl_w07_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x96;
		esp_vec.vy = 0x514;
		esp_vec.vz = 0;
		Esp_call(0x1001d00, 0, pMtx, &esp_vec);
		Esp_call(0x1041000, 0, pMtx, &esp_vec);
		Esp_call(0x30c1800, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x708;
		Esp_call(0x30e1000, 0, pMtx, &esp_vec);
		Esp_call(0x1025000, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x8fc;
		Esp_call(0x30e1d00, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 26) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0xb8;
		esp_vec.vy = 0x22e;
		esp_vec.vz = -0x50;
		Esp_call(0x3080d00, 0, pMtx, &esp_vec);
		Esp_call(0x2020900, (SHORT)((USHORT)G.Player.Cdir_y + 0x514), pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 27) {
		esp_vec.vx = 0xb8;
		esp_vec.vy = 0x21a;
		esp_vec.vz = -0x50;
		Esp_call(0x3080d00, 0, &pEm->pSin_parts_ptr[0xb].Workm, &esp_vec);
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w08_t2_move
 * line = 319, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044908 (pl_w08_t2_move) */
VOID pl_w08_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x30c;
		esp_vec.vy = 0x532;
		esp_vec.vz = 0;
		Esp_call(0x1002900, 0, pMtx, &esp_vec);
		Esp_call(0x1041600, 0, pMtx, &esp_vec);
		Esp_call(0x1054000, 0, pMtx, &esp_vec);
		Esp_call(0x30c1600, 0, pMtx, &esp_vec);
		esp_vec.vx = 0x3bf;
		esp_vec.vy = 0x65e;
		Esp_call(0x30e0e00, 0, pMtx, &esp_vec);
		Esp_call(0x1041800, 0, pMtx, &esp_vec);
		esp_vec.vx = 0x4e5;
		esp_vec.vy = 0x852;
		Esp_call(0x30e1600, 0, pMtx, &esp_vec);
		Esp_call(0x1035900, 0, pMtx, &esp_vec);
		Esp_call(0x1041a00, 0, pMtx, &esp_vec);
		esp_vec.vx = 0x646;
		esp_vec.vy = 0xaaa;
		Esp_call(0x30e2000, 0, pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 5) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0xb8;
		esp_vec.vy = 0x22e;
		esp_vec.vz = -0x50;
		Esp_call(0x3080d00, 0, pMtx, &esp_vec);
		Esp_call(0x2020900, (SHORT)((USHORT)G.Player.Cdir_y + 0x514), pMtx, &esp_vec);
	}
	if (pEm->Move_cnt == 6) {
		esp_vec.vx = 0xb8;
		esp_vec.vy = 0x21a;
		esp_vec.vz = -0x50;
		Esp_call(0x3080d00, 0, &pEm->pSin_parts_ptr[0xb].Workm, &esp_vec);
	}
	G.Status_flg &= ~STAT_BG_HOLD;
	return;
}

/*
 * pl_w09_t2_move
 * line = 362 (approx), file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044b44 (pl_w09_t2_move) */
VOID pl_w09_t2_move(ENEMY_WORK *pEm)
{
	SHORT spd_x;
	UCHAR esp_idx;
	ULONG tama_ck;
	UCHAR mv_cnt;
	MATRIX *pMtx;
	ULONG esp_no;
	ULONG Pl_idx;
	SVECTOR esp_vec;
	/* DS 0x80011030: 12-byte initialiser template in .rdata, block-copied into the local (lwl/lwr pairs) */
	SHORT mv_tbl[6] = { 0, -120, -240, 250, 200, 150 };
	SHORT *pTbl;

	/* table base kept in a saved register, as in the DS routine */
	pTbl = mv_tbl;
	mv_cnt = pEm->Move_cnt;
	if (mv_cnt == 1) {
		esp_vec.vx = 0x78;
		esp_vec.vy = 0x4b0;
		esp_vec.vz = 0;
		Pl_idx = (ULONG)(G.Player.Status_flg >> 0xf);
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		Esp_call(0x1002000, 0, pMtx, &esp_vec);
		esp_idx = Esp_call(0x20c0a00, pEm->Cdir_y, pMtx, &esp_vec);
		esp_no = (ULONG)esp_idx;
		if (esp_no < 0xff) {
			Esp[esp_no].Add_x = -10;
			spd_x = (SHORT)pTbl[Pl_idx * 3];
			Esp[esp_no].Speed_y = 600;
			Esp[esp_no].Speed_z = 0;
			Esp[esp_no].Speed_x = spd_x;
		}
		esp_idx = Esp_call(0x20c0a00, pEm->Cdir_y, pMtx, &esp_vec);
		esp_no = (ULONG)esp_idx;
		if (esp_no < 0xff) {
			Esp[esp_no].Add_x = -0x14;
			spd_x = (SHORT)pTbl[Pl_idx * 3 + 1];
			Esp[esp_no].Speed_y = 400;
			Esp[esp_no].Speed_z = 100;
			Esp[esp_no].Speed_x = spd_x;
		}
		esp_idx = Esp_call(0x20c0a00, pEm->Cdir_y, pMtx, &esp_vec);
		esp_no = (ULONG)esp_idx;
		if (esp_no < 0xff) {
			Esp[esp_no].Add_x = -0x14;
			spd_x = (SHORT)pTbl[Pl_idx * 3 + 1];
			Esp[esp_no].Speed_y = 400;
			Esp[esp_no].Speed_z = -100;
			Esp[esp_no].Speed_x = spd_x;
		}
		esp_idx = Esp_call(0x20c0a00, pEm->Cdir_y, pMtx, &esp_vec);
		esp_no = (ULONG)esp_idx;
		if (esp_no < 0xff) {
			Esp[esp_no].Add_x = -0x28;
			spd_x = (SHORT)pTbl[Pl_idx * 3 + 2];
			Esp[esp_no].Speed_y = 0xfa;
			Esp[esp_no].Speed_z = 0x32;
			Esp[esp_no].Speed_x = spd_x;
		}
		esp_idx = Esp_call(0x20c0a00, pEm->Cdir_y, pMtx, &esp_vec);
		esp_no = (ULONG)esp_idx;
		if (esp_no < 0xff) {
			Esp[esp_no].Add_x = -0x28;
			spd_x = (SHORT)pTbl[Pl_idx * 3 + 2];
			Esp[esp_no].Speed_y = 0xfa;
			Esp[esp_no].Speed_z = -0x32;
			Esp[esp_no].Speed_x = spd_x;
		}
		mv_cnt = pEm->Move_cnt;
	}
	if ((mv_cnt == 8) && (tama_ck = Tama_ck_i(pEm->Type & 0xfff), tama_ck == 0)) {
		*(USHORT *)&pEm->Routine_2 = 0x1;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
	}
	mv_cnt = 10;
	if (pEm->Move_no == 12) {
		mv_cnt = 9;
	}
	if (pEm->Move_cnt == mv_cnt) {
		G_rot_st(pEm);
		Pad_vib_set0(3, 0);
		Pad_vib_set1(5, 0x96, 0x11);
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w0a_t2_move
 * line = (approx 430), file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80044f44 (pl_w0a_t2_move) */
VOID pl_w0a_t2_move(ENEMY_WORK *pEm)
{
	UCHAR mv_cnt;
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x78;
		esp_vec.vy = 0x4b0;
		esp_vec.vz = 0;
		Esp_call(0x1002000, 0, pMtx, &esp_vec);
		Esp_call(0x20c1000, pEm->Cdir_y, pMtx, &esp_vec);
		Esp_call(0x3081200, 0, pMtx, &esp_vec);
	}
	if ((pEm->Move_cnt == 8) && Tama_ck_i(pEm->Type & 0xfff) == 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		*(ULONG *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y, one sw at +0x144 (DS: sw zero) */
		pEm->Spd_z = 0;
	}
	mv_cnt = 10;
	if (pEm->Move_no == 12) {
		mv_cnt = 9;
	}
	if (pEm->Move_cnt == mv_cnt) {
		G_rot_st(pEm);
		Pad_vib_set0(3, 0);
		Pad_vib_set1(5, 0x96, 0x11);
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w0b_t2_move
 * line = (approx 455), file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045090 (pl_w0b_t2_move) */
VOID pl_w0b_t2_move(ENEMY_WORK *pEm)
{
	UCHAR mv_cnt;
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x78;
		esp_vec.vy = 0x4b0;
		esp_vec.vz = 0;
		Esp_call(0x1002000, 0, pMtx, &esp_vec);
		Esp_call(0x20c1000, pEm->Cdir_y, pMtx, &esp_vec);
		Esp_call(0x3081200, 0, pMtx, &esp_vec);
	}
	if ((pEm->Move_cnt == 8) && (Tama_ck_i(pEm->Type & 0xfff) == 0)) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		*(ULONG *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y, one sw at +0x144 (DS: sw zero) */
		pEm->Spd_z = 0;
	}
	mv_cnt = 10;
	if (pEm->Move_no == 12) {
		mv_cnt = 9;
	}
	if (pEm->Move_cnt == mv_cnt) {
		G_rot_st(pEm);
		Pad_vib_set0(3, 0);
		Pad_vib_set1(5, 0x96, 0x11);
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w0c_t2_move
 * line = 469, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800451dc (pl_w0c_t2_move) */
VOID pl_w0c_t2_move(ENEMY_WORK *pEm)
{
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	return;
}

/*
 * pl_w0d_t2_move
 * line = 478, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004522c (pl_w0d_t2_move) */
VOID pl_w0d_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;
	SVECTOR esp_rot;

	if (pEm->Move_cnt == 2) {
		/* DS loads pSin_parts_ptr before it stores esp_vec, so the matrix pointer is set first */
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x82;
		esp_vec.vy = 0x2da;
		esp_vec.vz = 0;
		Esp_call(0x1001000, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x118;
		Esp_call(0x1041000, 0, pMtx, &esp_vec);
		/* DS narrows the direction to 16 bits at the call (lhu, addiu, sll 16, sra 16) */
		Esp_call(0x1061000, (SHORT)(pEm->Cdir_y + 0x800), pMtx, &esp_vec);
		Esp_call(0x30a0c00, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x33e;
		Esp_call(0x30c1000, 0, pMtx, &esp_vec);
		esp_vec.vy = 0x38e;
		esp_rot.vz = 0x400;
		esp_rot.vx = 0;
		esp_rot.vy = 0;
		Esp_call3D(0x1073000, 0, pMtx, &esp_vec, &esp_rot);
	}
	return;
}

/*
 * pl_w0e_t2_move
 * line = 488, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004534c (pl_w0e_t2_move) */
VOID pl_w0e_t2_move(ENEMY_WORK *pEm)
{
	return;
}

/*
 * pl_w0f_t2_move
 * line = 493, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045354 (pl_w0f_t2_move) */
VOID pl_w0f_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMat;
	SVECTOR svec;

	if ((ULONG)pEm->Move_cnt % 3 == 1) {
		svec.vx = 0x46;
		svec.vy = 1000;
		if (G.Player.Id >= 4) {
			svec.vz = -100;
		} else {
			svec.vz = -0x3c;
		}
		pMat = &pEm->pSin_parts_ptr[0xb].Workm;
		Esp_call(0x1011000, 0, pMat, &svec);
		Esp_call(0x30c1000, 0, pMat, &svec);
		Pl_weapon_vib(pEm);
	}
	if ((ULONG)pEm->Move_cnt % 6 == 1) {
		svec.vx = 0xaa;
		svec.vy = 0x118;
		svec.vz = 0x50;
		Esp_call(0x2010900, (SHORT)((USHORT)G.Player.Cdir_y + 0x4b0), &pEm->pSin_parts_ptr[0xb].Workm, &svec);
	}
	return;
}

/*
 * pl_w10_t2_move
 * line = 522, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800454a0 (pl_w10_t2_move) */
VOID pl_w10_t2_move(ENEMY_WORK *pEm)
{
	SVECTOR esp_vec;

	if ((ULONG)pEm->Move_cnt % 3 == 1) {
		MATRIX *pMtx;

		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0x96;
		esp_vec.vy = 0x4b0;
		esp_vec.vz = 0;
		Esp_call(0x31d1200, G.Player.Cdir_y, pMtx, &esp_vec);
		Game.Plight_flg = 1;
	}
	if (pEm->Move_cnt == 1) {
		Snd_se_stad(0x1000001, (VECTOR *)&pEm->pSin_parts_ptr[0xb].Pos_x);
		Pl_weapon_vib(pEm);
	}
	if (pEm->Move_cnt == 11) {
		Snd_se_stad(0x10b0001, (VECTOR *)&pEm->pSin_parts_ptr[0xb].Pos_x);
	}
	return;
}

/*
 * pl_w11_t2_move
 * line = 543, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045588 (pl_w11_t2_move) */
VOID pl_w11_t2_move(ENEMY_WORK *pEm)
{
	MATRIX *pMtx;
	SVECTOR esp_vec;

	if (pEm->Move_cnt == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = 0;
		esp_vec.vy = 0x44c;
		esp_vec.vz = 0;
		Esp_call(0x1002000, 0, pMtx, &esp_vec);
		Esp_call(0x20d1000, pEm->Cdir_y, pMtx, &esp_vec);
		Esp_call(0x30a1a00, 0, pMtx, &esp_vec);
		esp_vec.vx = 300;
		esp_vec.vy = -900;
		Esp_call(0x30a1500, 0, pMtx, &esp_vec);
	}
	return;
}

/*
 * pl_w12_t2_move
 * line = 562, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045648 (pl_w12_t2_move) */
VOID pl_w12_t2_move(ENEMY_WORK *pEm)
{
	LONG rnd_r;
	MATRIX *pMtx;
	PARTS_W *pP_ptr;
	SVECTOR esp_vec;
	SVECTOR esp_rot;

	if ((ULONG)pEm->Move_cnt % 3 == 1) {
		pP_ptr = pEm->pSin_parts_ptr;
		pMtx = &pP_ptr[0xb].Workm;
		esp_vec.vx = -200;
		esp_vec.vy = 0x4ce;
		esp_vec.vz = 0;
		esp_rot.vx = 0;
		esp_rot.vy = -200;
		esp_rot.vz = 0x400;
		Esp_call(0x1001000, 0, pMtx, &esp_vec);
		Esp_call(0x30c0a00, 0, pMtx, &esp_vec);
		esp_vec.vx = -0x96;
		esp_vec.vy = 0x5d2;
		Esp_call3D(0x1075000, 0, pMtx, &esp_vec, &esp_rot);
		Pl_weapon_vib(pEm);
	}
	if ((ULONG)pEm->Move_cnt % 6 == 1) {
		pMtx = &pEm->pSin_parts_ptr[0xb].Workm;
		esp_vec.vx = -200;
		esp_vec.vy = 400;
		esp_vec.vz = 0;
		Esp_call(0x30c1200, 0, pMtx, &esp_vec);
		if ((ULONG)pEm->Move_cnt % 6 == 1) {
			pP_ptr = pEm->pSin_parts_ptr;
			esp_vec.vy = 0x82;
			esp_vec.vx = -200;
			esp_vec.vz = 0x14;
			/* Rnd() is read through a LONG-returning type: the result is not masked after the call, and % 10
			 * below is a signed divide. */
			rnd_r = ((LONG (*)(VOID))Rnd)();
			/* DS adds 0xa00 to the random step (rnd_r % 10 * 0x14) first and then adds the lhu of G.Player.Cdir_y;
			 * only the low 16 bits reach Esp_call, sign-extended by the (SHORT) cast. */
			Esp_call(0x2010a00, (SHORT)(G.Player.Cdir_y + 0xa00 + (rnd_r % 10) * 0x14), &pP_ptr[0xb].Workm, &esp_vec);
		}
	}
	return;
}

/*
 * pl_mv05_w1_t3
 * line = 600, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045850 (pl_mv05_w1_t3) */
VOID pl_mv05_w1_t3(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	LONG rnd_r;
	ULONG ck_ret;
	PARTS_W *pP_ptr;
	SVECTOR esp_vec;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(UINT *)&pEm->Move_no = 0xF0109;		/* Move_no 9, Move_cnt 1, Hokan_flg 15, Mplay_flg 0 (DS: one sw at +0x14c) */
	}
	if (pEm->Water != 0) {
		pP_ptr = pEm->pSin_parts_ptr;
		esp_vec.vx = 0;
		esp_vec.vy = 200;
		esp_vec.vz = 0;
		if ((int)pEm->Water < pP_ptr[0xb].Workm.t[1] + 300) {
			/* DS takes Rnd()'s result straight from v0 with no andi 0xff after the call: the call is made through a
			 * LONG-returning type, as in pl_mv05_w1_t2 (Rnd already returns a value in 0..255). */
			rnd_r = ((LONG (*)(VOID))Rnd)();
			Esp_call((rnd_r * 4 + 0x60c) | 0x1a000000, pEm->Cdir_y, &pP_ptr[0xb].Workm, &esp_vec);
		}
		pP_ptr = pEm->pSin_parts_ptr;
		if ((int)pEm->Water < pP_ptr[0xe].Workm.t[1] + 300) {
			rnd_r = ((LONG (*)(VOID))Rnd)();
			Esp_call((rnd_r * 4 + 0x60c) | 0x1a000000, pEm->Cdir_y, &pP_ptr[0xe].Workm, &esp_vec);
		}
	}
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -0x40;
	}
	if ((Key & 2) != 0) {
		pEm->Cdir_y += 0x40;
	}
	ck_ret = Joint_move(pEm, Kan, Seq, 0x10100);
	if (ck_ret != 0) {
		*(UINT *)&pEm->Routine_0 = 0x1;		/* Routine_0 1, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
		pEm->Neck_flg &= 0xed;
		G.Status_flg &= ~STAT_BG_HOLD;
	}
	return;
}

/*
 * pl_mv05_w1_t4
 * line = 664, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800459e4 (pl_mv05_w1_t4) */
VOID pl_mv05_w1_t4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG ck_ret;
	PARTS_W *pP_ptr;
	SVECTOR esp_vec;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(UINT *)&pEm->Move_no = 0x70010;		/* Move_no 16, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
	}
	ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
	if (ck_ret != 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	if (((*pEm->pNow_seq & 0x200000) != 0) && ((pEm->Status_flg & 1) != 0)) {
		if ((pEm->Type & 0xfff) == 0xd) {
			/* DS reads pSin_parts_ptr into a2 before the three esp_vec stores and adds the
			   Workm offset of part 11 in the Esp_call delay slot. */
			pP_ptr = pEm->pSin_parts_ptr;
			esp_vec.vx = 100;
			esp_vec.vy = 0xdc;
			esp_vec.vz = 0;
			Esp_call(0x2070800, pEm->Cdir_y, &pP_ptr[0xb].Workm, &esp_vec);
		} else {
			Mag_set(pEm);
		}
		pEm->Status_flg &= 0xfffe;
	}
	if ((*pEm->pNow_seq & 0x400000) != 0) {
		Tama_reload();
		pEm->Status_flg |= 1;
	}
	return;
}

/*
 * pl_mv05_w1_t5
 * line = 696, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045b38 (pl_mv05_w1_t5) */
VOID pl_mv05_w1_t5(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	CHAR tmr_old;
	LONG ck_ret;
	ENEMY_WORK *pTgt;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		pl_mv05_sub(pEm);
		if (pEm->pEnemy_ptr == (ULONG *)pEm) goto end_state;
		*(UINT *)&pEm->Move_no = 0xF0309;		/* Move_no 9, Move_cnt 3, Hokan_flg 15, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Timer3 = 12;
	} else if (pEm->Routine_3 != 1) {
		return;
	}
	pTgt = (ENEMY_WORK *)pEm->pEnemy_ptr;
	if (pTgt != pEm) {
		Goto00(pEm, pTgt->Pos_x, pTgt->Pos_z, 0xd8);
	}
	ck_ret = Goto00_ck(pEm, pTgt->Pos_x, pTgt->Pos_z, 0x10);
	if (ck_ret == 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	}
	tmr_old = pEm->Timer3;
	pEm->Timer3 = tmr_old + -1;
	if (tmr_old != 0) {
		return;
	}
end_state:
	*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	return;
}

/*
 * pl_mv05_sub
 * line = 735, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045c10 (pl_mv05_sub) */
VOID pl_mv05_sub(ENEMY_WORK *pEm)
{
	BOOL at_end;
	LONG ck_ret;
	ULONG sca_ret;
	ULONG alive_cnt;
	ENEMY_WORK *pTgt;
	ULONG rest_cnt;
	ULONG scan_cnt;
	INT idx_nxt;
	INT idx_cur;
	ENEMY_WORK **ppTgt;
	INT prev_idx;
	INT best_idx;
	INT alt_idx;
	VECTOR tgt_pt;

	idx_nxt = 2;
	rest_cnt = (ULONG)G.Enemy_num;
	alive_cnt = 0;
	alt_idx = 0;
	if (rest_cnt != 0) {
		scan_cnt = rest_cnt;
		idx_cur = idx_nxt;
		if ((G.pEnemy[2]->Be_flg & 1) == 0) goto scan_loop;
		alive_cnt = 1;
		pTgt = G.pEnemy[2];
		while (scan_cnt = scan_cnt - 1, idx_cur = idx_nxt, pEm->pEnemy_ptr != (ULONG *)pTgt) {
scan_loop:
			do {
				idx_nxt = idx_cur + 1;
				if (scan_cnt == 0) goto scan_done;
				pTgt = G.pEnemy[idx_cur + 1];
				idx_cur = idx_nxt;
			} while ((pTgt->Be_flg & 1) == 0);
			alive_cnt++;
		}
scan_done:
		idx_cur = idx_nxt + 1;
		if (rest_cnt == alive_cnt) {
			idx_cur = 2;
		}
		ppTgt = G.pEnemy + idx_cur;
		prev_idx = 0;
		do {
			pTgt = *ppTgt;
			best_idx = prev_idx;
			if ((pTgt->Be_flg & 1) != 0) {
				tgt_pt.vx = pTgt->Atd[0].X;
				tgt_pt.vy = pTgt->pSin_parts_ptr[pEm->Neck_no].Workm.t[1];
				tgt_pt.vz = pTgt->Atd[0].Z;
				rest_cnt--;
				if (-1 < pTgt->Life) {
					ck_ret = Goto00_ck(pEm, pTgt->Pos_x, pTgt->Pos_z, 0x400);
					if (((ck_ret == 0) && (sca_ret = Sca_ck_line((VECTOR *)((int)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t), &tgt_pt, 0x80, 1), sca_ret == 0)) && ((pTgt->Type & 0xc000) == 0)) {
						best_idx = idx_cur;
						alt_idx = prev_idx;
					}
				}
			}
			at_end = ppTgt == G.ppEnemy_end;
			ppTgt++;
			if (at_end) {
				ppTgt = G.pEnemy + 2;
				idx_cur = 2;
			} else {
				idx_cur++;
			}
			prev_idx = best_idx;
		} while (rest_cnt != 0);
		if (best_idx != 0) {
			idx_cur = best_idx << 2;
			if ((idx_nxt == best_idx) && (alt_idx != 0)) {
				idx_cur = alt_idx << 2;
			}
			pEm->pEnemy_ptr = (ULONG *)*(ENEMY_WORK **)((int)G.pEnemy + idx_cur);
			return;
		}
	}
	pEm->pEnemy_ptr = (ULONG *)pEm;
	return;
}

/*
 * pl_bow_init
 * line = 823, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80045e78 (pl_bow_init) */
UCHAR pl_bow_init(ENEMY_WORK *pEm, ULONG No)
{
	PARTS_W *pP_base;
	PARTS_W *pO;
	PARTS_W *pP_ptr;
	PARTS_W *pHand;
	MATRIX *m0;
	INT loop_cnt;
	SHORT bow_idx;

	loop_cnt = 6;
	pP_base = pEm->pSin_parts_ptr;
	pP_ptr = pP_base;
	pHand = pP_base + 0xb;
	pO = pP_ptr + 0x14;
	if (pEm->Id == 14) {
		pO = pP_ptr + 0x11;
	}
	bow_idx = 0;
	do {
		if ((UCHAR)pO->Free[0] == 0xff) {
			*(UCHAR *)&pO->Free[0] = 0;
			pO->Be_flg = 1;
			pO->Pos_x = 0xc8;
			(pO + 1)->Be_flg = 1;
			pO->Pos_y = 0x390;
			pO->Pos_z = 0;
			if (No == 1) {
				pO->Pos_z = 0x64;
			}
			if (No == 2) {
				pO->Pos_z = -100;
			}
			m0 = (MATRIX *)pO->pSuper;
			*(ULONG *)&pO->M[0][0] = 0x1000;
			*(ULONG *)&pO->M[0][2] = 0;
			*(ULONG *)&pO->M[1][1] = 0x1000;
			*(ULONG *)&pO->M[2][0] = 0;
			*(ULONG *)&pO->M[2][2] = 0x1000;
			CompM(m0, (MATRIX *)pO->M, &pO->Workm);
			pO->Rot_y = pEm->Cdir_y;
			pO->Be_flg = 1;
			pO->pSuper = (ULONG *)&pP_base[0xb].Workm;
			pO->pOya_parts = &pP_base[0xb].Be_flg;
			pHand->Timer0 = bow_idx;
			return 1;
		}
		pO += 2;
		loop_cnt = loop_cnt + -1;
		bow_idx++;
	} while (loop_cnt != 0);
	bow_idx = pHand->Timer0 + 1;
	pHand->Timer0 = bow_idx;
	if (2 < bow_idx) {
		pHand->Timer0 = 0;
	}
	pP_ptr = pEm->pSin_parts_ptr + pHand->Timer0 * 2 + 0x14;
	if (pEm->Id == 14) {
		pP_ptr = pP_ptr + -3;
	}
	*(UCHAR *)pP_ptr->Free = 0;
	pP_ptr->Be_flg = 1;
	pP_ptr[1].Be_flg = 1;
	pP_ptr->Rot_y = pEm->Cdir_y;
	pO = pEm->pSin_parts_ptr;
	pP_ptr->M[0][0] = 0x1000;
	pP_ptr->M[0][1] = 0;
	pP_ptr->M[1][1] = 0x1000;
	pP_ptr->M[1][2] = 0;
	*(ULONG *)&pP_ptr->M[2][2] = 0x1000;
	pP_ptr->Pos_x = 200;
	pP_ptr->Be_flg = 1;
	pP_ptr->Pos_y = 0x390;
	*(ULONG *)&pP_ptr->M[0][2] = 0;
	pP_ptr->M[2][0] = 0;
	pP_ptr->M[2][1] = 0;
	pP_ptr->Pos_z = 0;
	pP_ptr->pSuper = (ULONG *)&pO[0xb].Workm;
	pP_ptr->pOya_parts = &pO[0xb].Be_flg;
	pP_ptr->Be_flg |= 0x40;
	if (No == 1) {
		pP_ptr->Pos_z = 100;
	}
	if (No == 2) {
		pP_ptr->Pos_z = -100;
	}
	CompM((MATRIX *)pP_ptr->pSuper, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
	return 0;
}

/*
 * pl_bow_clr
 * line = 896, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800460ac (pl_bow_clr) */
UCHAR pl_bow_clr(ENEMY_WORK *pEm)
{
	PARTS_W *pP_base;
	MATRIX *m0;
	ULONG **pO;
	PARTS_W *pP_ptr;
	INT loop_cnt;

	loop_cnt = 6;
	pP_ptr = pEm->pSin_parts_ptr + 0x14;
	if (pEm->Id == 14) {
		pP_ptr = pEm->pSin_parts_ptr + 0x11;
	}
	pO = &pP_ptr->pOya_parts;
	do {
		*(UCHAR *)&pP_ptr->Free[0] = 0xff;
		pP_ptr->Be_flg = 0;
		m0 = (MATRIX *)pP_ptr->pSuper;
		pP_ptr[1].Be_flg = 0;
		*(ULONG *)&pP_ptr->M[0][0] = 0x1000;
		*(ULONG *)&pP_ptr->M[0][2] = 0;
		*(ULONG *)&pP_ptr->M[1][1] = 0x1000;
		*(ULONG *)&pP_ptr->M[2][0] = 0;
		*(ULONG *)&pP_ptr->M[2][2] = 0x1000;
		CompM(m0, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);
		pP_ptr += 2;
		pP_ptr[-2].Rot_y = pEm->Cdir_y;
		pP_base = pEm->pSin_parts_ptr;
		loop_cnt = loop_cnt + -1;
		pP_ptr[-2].pSuper = (ULONG *)&pP_base[0xb].Workm;
		*pO = &pP_base[0xb].Be_flg;
		pO += 0x56;
	} while (loop_cnt != 0);
	pEm->pSin_parts_ptr[0xb].Timer0 = 0;
	return 0;
}

/*
 * Bow_Lim_dir_ck
 * line = 930, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004618c (Bow_Lim_dir_ck) */
ENEMY_WORK *Bow_Lim_dir_ck(VECTOR *pV, SHORT Cdir_y, ULONG Lim, SHORT Dir)
{
	SHORT Tdir;
	INT dz;
	ULONG dist;
	LONG ret;
	ENEMY_WORK *pTgt;
	ENEMY_WORK **ppTgt;
	ENEMY_WORK *pBest;
	INT dif_xz[2];
	tagGLOBAL *pG = &G;

	pBest = (ENEMY_WORK *)0x0;
	if (G.Enemy_num != 0) {
		ppTgt = pG->pEnemy + 2;
		do {
			pTgt = *ppTgt;
			if (((pTgt->Be_flg & 1) != 0) && ((pTgt->Type & 0xe000) == 0)) {
				dif_xz[0] = pV->vx - pTgt->Pos_x;
				dz = pV->vz - pTgt->Pos_z;
				dist = SquareRoot0(dif_xz[0] * dif_xz[0] + dz * dz);
				Tdir = Direction_ck(pV->vx, pV->vz, pTgt->Pos_x, pTgt->Pos_z);
				if (-1 < pTgt->Life) {
					ret = Goto_pro(Tdir, Cdir_y, Dir, (SHORT *)dif_xz);
					if ((ret == 0) && (dist < Lim)) {
						Lim = dist;
						pBest = pTgt;
					}
				}
			}
			ppTgt++;
		} while (ppTgt != pG->ppEnemy_end);
		if (pBest != (ENEMY_WORK *)0x0) {
			return pBest;
		}
	}
	return G.pEm;
}

/* The fields of the bow part other than Be_flg and M are addressed from pBow_w (the Free area of the same part), as DS does. */
#define PB (*(PARTS_W *)((char *)pBow_w - (int)&((PARTS_W *)0)->Free[0]))

/*
 * pl_bow
 * line = 1023, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80046304 (pl_bow) */
VOID pl_bow(ENEMY_WORK *pEm)
{
	SHORT old_cnt;
	UCHAR rnd_b;
	UCHAR rnd_r;
	ENEMY_WORK *pTgt;
	ULONG rng_no;
	PARTS_W *pP_tgt;
	LONG water_y;
	ULONG part_idx;
	INT wk_iv;
	MATRIX *lwm;
	ULONG *pTgt_part;
	SHORT *pBow_w;
	PARTS_W *pP_bow;
	MATRIX *pBow_wm;
	VECTOR *pBow_pos;
	SVECTOR spd_vec;
	SVECTOR dpos_vec;
	MATRIX mtx_rot;
	SVECTOR dir_vec[2];
	SVECTOR ang_tgt;
	SVECTOR ang_cur;
	/* DS 0x8001103c: 8-byte initialiser template in .rdata (Off_x -500, Off_z 0, R_x 375, Half_R_z 25), block-copied into the local */
	ATTACK_WORK atk_wk = { -500, 0, 375, 25 };
	SHORT goto_out[4];
	INT loop_cnt;
	CHAR add_flg;
	SVECTOR *pDpos;

	loop_cnt = 6;
	add_flg = 1;
	pP_bow = pEm->pSin_parts_ptr + 0x14;
	if (pEm->Id == 14) {
		pP_bow = pEm->pSin_parts_ptr + 0x11;
	}
	pDpos = &dpos_vec;
	pBow_w = pP_bow->Free;
	pBow_pos = (VECTOR *)((int)(pP_bow->Workm).t);
	pBow_wm = &pP_bow->Workm;
	do {
		switch((CHAR)pBow_w[0]) {
		case 0:
			pP_bow->Be_flg |= 0x8040;
			((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 1;
			*(ULONG *)(pBow_w + 10) = *(ULONG *)(pBow_w + 10) | 0x8000;
			pP_bow->Be_flg |= 0x400;
			pBow_w[-5] = 0x2000;
			pBow_w[81] = 0x2000;
			pBow_w[-6] = 0x1000;
			pBow_w[-4] = 0x1000;
			pBow_w[80] = 0x1000;
			pBow_w[82] = 0x1000;
			PB.Timer0 = 10;
			PB.Timer1 = 0;
			PB.Timer2 = 0;
			*(ULONG *)(pBow_w + 10) = *(ULONG *)(pBow_w + 10) | 0x400;
			spd_vec.vx = 0;
			rnd_b = Rnd();
			spd_vec.vz = (rnd_b & 1) * -0x28 + 0x14;
			wk_iv = PB.Pos_z;
			if (wk_iv == -100) {
				spd_vec.vx = 100;
				rnd_b = Rnd();
				spd_vec.vz = (rnd_b & 1) * -0x3c + 0x1e;
				wk_iv = PB.Pos_z;
			}
			if (wk_iv == 100) {
				spd_vec.vx = -100;
				rnd_b = Rnd();
				spd_vec.vz = (rnd_b & 1) * -0x3c + 0x1e;
			}
			spd_vec.vy = 0;
			RotMatrix(&spd_vec, &mtx_rot);
			MulMatrix((MATRIX *)pBow_wm, &mtx_rot);
			Sca_get_ground(pBow_pos, 200, 0x2000, 0);
			if (Ground_in_flg != 0) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
				return;
			}
			add_flg = 0;
		case 1:
			spd_vec.vx = 0;
			spd_vec.vy = 800;
			spd_vec.vz = 0;
			ApplyMatrixSV((MATRIX *)pBow_wm, &spd_vec, pDpos);
			PB.Workm.t[0] = PB.Workm.t[0] + (INT)dpos_vec.vx;
			PB.Workm.t[1] = PB.Workm.t[1] + (INT)dpos_vec.vy;
			PB.Workm.t[2] = PB.Workm.t[2] + (INT)dpos_vec.vz;
			if (add_flg == 0) {
				PB.Workm.t[0] = PB.Workm.t[0] - (INT)dpos_vec.vx;
				PB.Workm.t[1] = PB.Workm.t[1] - (INT)dpos_vec.vy;
				PB.Workm.t[2] = PB.Workm.t[2] - (INT)dpos_vec.vz;
			}
			Bow_3D((SVECTOR *)pDpos, &ang_cur);
			pBow_w[1] = ang_cur.vy;
			if (PB.Pos_z == 0) {
				rng_no = 10000;
			} else {
				rng_no = 5000;
			}
			pTgt = Bow_Lim_dir_ck(pBow_pos, ang_cur.vy, rng_no, 800);
			rng_no = Bow_goto_parts_no(pTgt, pBow_wm, Pl_bow_hit_parts);
			part_idx = rng_no & 0xffff;
			pP_tgt = pTgt->pSin_parts_ptr;
			dir_vec[0].vx = (SHORT)((*(INT *)((int)pP_tgt[part_idx].Workm.t) - PB.Workm.t[0]) >> 3);
			dir_vec[0].vy = (SHORT)((pP_tgt[part_idx].Workm.t[1] - PB.Workm.t[1]) >> 3);
			dir_vec[0].vz = (SHORT)((pP_tgt[part_idx].Workm.t[2] - PB.Workm.t[2]) >> 3);
			Bow_3D(dir_vec, &ang_tgt);
			if ((pTgt != pEm) && (PB.Timer0 != 0)) {
				Goto_pro(ang_tgt.vy, ang_cur.vy, 0x20, goto_out);
				ang_cur.vy = goto_out[0];
				Goto_pro(ang_tgt.vz, ang_cur.vz, 0x58, goto_out);
				ang_cur.vz = goto_out[0];
				RotMatrix((SVECTOR *)&ang_cur, (MATRIX *)pBow_wm);
				PB.Timer0 = PB.Timer0 + -1;
			}
			Sca_get_ground(pBow_pos, 100, 0x2000, 0);
			if (Ground_in_flg != 0) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
			}
			if (PB.Workm.t[1] < Ceiling) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
			}
			if (60000 < PB.Workm.t[0] + 30000U) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
				PB.Timer1 = 1;
			}
			if (60000 < PB.Workm.t[1] + 30000U) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
				PB.Timer1 = 1;
			}
			if (60000 < PB.Workm.t[2] + 30000U) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 2;
				PB.Timer1 = 1;
			}
			pTgt = (ENEMY_WORK *)Pos_em_at_ck(pBow_pos, pBow_w[-13], &atk_wk, 0xc);
			*(ENEMY_WORK **)(pBow_w + 2) = pTgt;
			if (pTgt != (ENEMY_WORK *)0x0) {
				rng_no = Bow_goto_parts_no(pTgt, pBow_wm, Pl_bow_hit_parts);
				part_idx = rng_no & 0xffff;
			}
			water_y = Water_ck(PB.Workm.t[0], PB.Workm.t[2]);
			wk_iv = (INT)(SHORT)water_y;
			if (wk_iv != 0) {
				if (wk_iv < PB.Workm.t[1]) {
					rnd_r = Rnd();
					Esp_call((rnd_r * 2 + 0x418) | 0x1a000000, ang_cur.vy + 0x800, pBow_wm, (SVECTOR *)0x0);
					spd_vec.vy = 200;
					rnd_r = Rnd();
					Esp_call((rnd_r * 2 + 0x418) | 0x1a000000, ang_cur.vy + 0x800, pBow_wm, (SVECTOR *)&spd_vec);
					spd_vec.vy = 400;
					rnd_r = Rnd();
					Esp_call((rnd_r * 2 + 0x418) | 0x1a000000, ang_cur.vy + 0x800, pBow_wm, (SVECTOR *)&spd_vec);
					spd_vec.vy = 600;
					rnd_r = Rnd();
					Esp_call((rnd_r * 2 + 0x418) | 0x1a000000, ang_cur.vy + 0x800, pBow_wm, (SVECTOR *)&spd_vec);
				}
				if (((signed char)PB.Timer2 == 0) && (wk_iv < PB.Workm.t[1])) {
					PB.Timer2 = 1;
					spd_vec.vx = (SHORT)PB.Workm.t[0];
					spd_vec.vz = (SHORT)PB.Workm.t[2];
					spd_vec.vy = (SHORT)water_y;
					rnd_r = Rnd();
					Esp_call((rnd_r * 2 + 0x418) | 0x1a050000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&spd_vec);
				}
			}
			if (((PARTS_BOW_FREE_WORK *)pBow_w)->pAe != 0) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 5;
				((PARTS_BOW_FREE_WORK *)pBow_w)->Hit_no = (UCHAR)part_idx;
				((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->pBow_hit_parts = pP_bow;
				if (*(CHAR *)&((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Id == 33) {
					((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Spd_x = dpos_vec.vx;
					((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Spd_y = dpos_vec.vy;
					((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Spd_z = dpos_vec.vz;
				}
			}
			break;
		case 2:
			pP_bow->Be_flg &= 0xfffffbff;
			*(ULONG *)(pBow_w + 10) = *(ULONG *)(pBow_w + 10) & 0xfffffbff;
			if ((signed char)PB.Timer1 == 0) {
				Snd_se_stad(0x10a0001, pBow_pos);
			}
			PB.Timer0 = 0x14;
			((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 3;
			PB.Timer1 = -1;
			pBow_w[10] = 0;
			pBow_w[11] = 0;
			PB.Timer3 = (SHORT)PB.Workm.t[1];
			spd_vec.vx = 0;
			spd_vec.vy = 400;
			spd_vec.vz = 0;
			ApplyMatrixSV((MATRIX *)pBow_wm, &spd_vec, &dpos_vec);
			PB.Workm.t[0] = PB.Workm.t[0] + (INT)dpos_vec.vx;
			PB.Workm.t[1] = PB.Workm.t[1] + (INT)dpos_vec.vy;
			PB.Workm.t[2] = PB.Workm.t[2] + (INT)dpos_vec.vz;
		case 3:
			spd_vec.vx = 0;
			spd_vec.vy = PB.Timer0 * (SHORT)((INT)PB.Timer1 << 3);
			spd_vec.vz = PB.Timer0 * (SHORT)((INT)PB.Timer1 << 3);
			RotMatrix(&spd_vec, &mtx_rot);
			MulMatrix0((MATRIX *)pBow_wm, &mtx_rot, (MATRIX *)pBow_wm);
			old_cnt = PB.Timer0;
			PB.Timer1 = -PB.Timer1;
			PB.Timer0 = old_cnt + -1;
			if (old_cnt == 0) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 4;
			}
			if ((*(int *)&G.SaveData.Stage_no == 0x90004) && (rng_no = Hani_ck(pBow_pos, Bow_Room509), rng_no == 0)) {
				PB.Workm.t[1] = PB.Workm.t[1] + -0xb4;
			}
			break;
		case 4:
			pP_bow->Be_flg = 0;
			pBow_w[10] = 0;
			pBow_w[11] = 0;
			((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 0xff;
			pP_tgt = pEm->pSin_parts_ptr;
			*(PARTS_W **)(pBow_w + -2) = pP_tgt + 0xb;
			*(MATRIX **)(pBow_w + -18) = &pP_tgt[0xb].Workm;
			pP_bow->Be_flg |= 0x40;
			PB.Pos_x = 200;
			PB.Pos_y = 0x390;
			PB.Pos_z = 0;
			break;
		case 5:
			pP_bow->Be_flg &= 0xfffffbff;
			PB.Timer0 = 0x4b;
			*(ULONG *)(pBow_w + 10) = *(ULONG *)(pBow_w + 10) & 0xfffffbff;
			if ((*(CHAR *)&((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Id == 0x36) || (*(CHAR *)&((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Id == 35)) {
				PB.Timer0 = 10;
			}
			if (*(CHAR *)&((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Id == 38) {
				PB.Timer0 = 0;
			}
			((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 6;
			PB.Timer1 = -1;
			PB.Timer3 = (SHORT)PB.Workm.t[1];
			wk_iv = (INT)((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->pSin_parts_ptr + (ULONG)*(UCHAR *)((int)pBow_w + 1) * 0xac;
			dir_vec[0].vx = (SHORT)((PARTS_W *)wk_iv)->Workm.t[0] - (SHORT)PB.Workm.t[0];
			dir_vec[0].vy = (SHORT)((PARTS_W *)wk_iv)->Workm.t[1] - (SHORT)PB.Workm.t[1];
			dir_vec[0].vz = (SHORT)((PARTS_W *)wk_iv)->Workm.t[2] - (SHORT)PB.Workm.t[2];
			Bow_3D(dir_vec, &ang_tgt);
			RotMatrix((SVECTOR *)&ang_tgt, (MATRIX *)pBow_wm);
			water_y = SquareRoot0((INT)dir_vec[0].vx * (INT)dir_vec[0].vx + (INT)dir_vec[0].vz * (INT)dir_vec[0].vz);
			part_idx = SquareRoot0(water_y * water_y + (INT)dir_vec[0].vy * (INT)dir_vec[0].vy);
			spd_vec.vy = (SHORT)(part_idx >> 2) * 3;
			spd_vec.vx = 0;
			spd_vec.vz = 0;
			ApplyMatrixSV((MATRIX *)pBow_wm, &spd_vec, &dpos_vec);
			PB.Workm.t[0] = PB.Workm.t[0] + (INT)dpos_vec.vx;
			PB.Workm.t[1] = PB.Workm.t[1] + (INT)dpos_vec.vy;
			PB.Workm.t[2] = PB.Workm.t[2] + (INT)dpos_vec.vz;
			Snd_se_stad(0x10f0001, pBow_pos);
			pTgt_part = (ULONG *)((INT)((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->pSin_parts_ptr + (ULONG)*(UCHAR *)((int)pBow_w + 1) * 0xac);
			lwm = &((PARTS_W *)pTgt_part)->Workm;
			MulLMatrix(lwm, pBow_wm, (MATRIX *)pP_bow->M);
			*(MATRIX **)(pBow_w + -18) = lwm;
			*(ULONG **)(pBow_w + -2) = pTgt_part;
			pP_bow->Be_flg &= 0xffffffbf;
			if ((((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Be_flg & 0x10000) == 0) {
				spd_vec.vx = (SHORT)PB.Pos_x;
				spd_vec.vy = (SHORT)PB.Pos_y;
				spd_vec.vz = (SHORT)PB.Pos_z;
				Esp_call((((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Be_flg >> 0x13 & 0x18) << 0x10 | 0x1bb8, pBow_w[1] + -0x800,
						 (MATRIX *)((ULONG)((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->pSin_parts_ptr + (ULONG)*(UCHAR *)((int)pBow_w + 1) * 0xac + 0x48),
						 (SVECTOR *)&spd_vec);
			}
		case 6:
			old_cnt = PB.Timer0;
			PB.Timer0 = old_cnt + -1;
			if (old_cnt == 0) {
				((PARTS_BOW_FREE_WORK *)pBow_w)->R_0 = 4;
			}
			if (((PB.Timer0 & 3U) == 0) && ((((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Be_flg & 0x10000) == 0)) {
				rnd_b = *(UCHAR *)((int)pBow_w + 1);
				part_idx = (ULONG)((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->pSin_parts_ptr;
				spd_vec.vx = (SHORT)PB.Pos_x;
				spd_vec.vy = (SHORT)PB.Pos_y;
				spd_vec.vz = (SHORT)PB.Pos_z;
				rnd_r = Rnd();
				Esp_call((((PARTS_BOW_FREE_WORK *)pBow_w)->pAe->Be_flg >> 0x13 & 0x18) << 0x10 | (rnd_r + 0x200) * 2, pBow_w[1] + -0x800, (MATRIX *)&((PARTS_W *)part_idx)[rnd_b].Workm, (SVECTOR *)&spd_vec);
			}
		}
		pBow_w += 0xac;
		pBow_pos = (VECTOR *)&pBow_pos[0x15].vz;
		pBow_wm = (MATRIX *)&pBow_wm[10].t[1];
		pP_bow += 2;
		loop_cnt = loop_cnt + -1;
		if (loop_cnt == 0) {
			return;
		}
	} while (1);
}

#undef PB

/*
 * Goto_pro
 * line = 1523, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80046f44 (Goto_pro) */
LONG Goto_pro(LONG Tdir, LONG Pdir, LONG Add_dir, SHORT *pDir)
{
	LONG ang_diff;
	SHORT new_dir;

	/* DS copies Tdir into its own register (v1) before the difference is
	   formed in the Tdir argument register; new_dir is that SHORT copy and
	   later carries the stepped direction that is stored through pDir. */
	new_dir = Tdir;
	ang_diff = (Add_dir + (Tdir - Pdir)) & 0xfff;
	if (ang_diff < ((SHORT)Add_dir << 1)) {
		/* close enough: snap straight to the target direction */
		*pDir = new_dir;
		return 0;
	}
	/* step one Add_dir: Pdir - Add_dir by default, Pdir + Add_dir (written as
	   Pdir - Add_dir + Add_dir * 2, as DS computes it) when the offset
	   difference is 0x800 or less */
	Pdir = Pdir - Add_dir;
	new_dir = Pdir;
	if ((ULONG)ang_diff < 0x801) {
		new_dir = Pdir + Add_dir * 2;
	}
	*pDir = new_dir;
	return 1;
}

/*
 * Bow_3D
 * line = 1550, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80046f98 (Bow_3D) */
VOID Bow_3D(SVECTOR *pTs, SVECTOR *pRot)
{
	SHORT rot_y;
	SHORT rot_x;
	LONG Vx;

	rot_y = Rot_3D((INT)pTs->vx, (INT)pTs->vz);
	Vx = SquareRoot0((INT)pTs->vx * (INT)pTs->vx + (INT)pTs->vz * (INT)pTs->vz);
	rot_x = Rot_3D(Vx, -(INT)pTs->vy);
	pRot->vx = 0;
	pRot->vy = rot_y;
	pRot->vz = (rot_x - 0x400U) & 0xfff;
	return;
}

/*
 * Rot_3D
 * line = 1570, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80047030 (Rot_3D) */
SHORT Rot_3D(LONG Vx, LONG Vz)
{
	LONG Kakudo;

	Kakudo = 0x400;
	if (0 < Vz) {
		Kakudo = 0xc00;
	}
	if (Vx != 0) {
		Kakudo = catan((Vz << 0xc) / Vx);
		/* DS selects 0x1000 or 0x800 as the base and masks the difference to
		 * 12 bits; 0x1000 & 0xfff is 0, so a non-negative Vx gives -atan. */
		return (((Vx < 0) ? 0x800 : 0x1000) - Kakudo) & 0xfff;
	}
	return Kakudo;
}

/*
 * Pos_em_at_ck
 * line = 1598, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800470c0 (Pos_em_at_ck) */
ULONG Pos_em_at_ck(VECTOR *pP, SHORT Cdir_y, ATTACK_WORK *pAtw, ULONG W_no)
{
	LONG ang_dif;
	ULONG ck_ret;
	ULONG at_h;
	UCHAR dmg_keep;
	INT ofs_h;
	ATTACK_WEAPON_EM *pAwe;
	ENEMY_WORK *pTgt;
	ENEMY_WORK *pHit;
	ENEMY_WORK **ppTgt;
	ULONG dmg_lv;
	tagGLOBAL *pG = &G;

	pHit = (ENEMY_WORK *)0x0;
	if (G.Enemy_num != 0) {
		dmg_lv = W_no >> 0x1c;
		ppTgt = pG->pEnemy + 2;
		do {
			pTgt = *ppTgt;
			if (((((pTgt->Be_flg & 1) != 0) && (pTgt->Damage_cnt == 0)) && (-1 < pTgt->Life)) &&
			   (((pTgt->Type & 0xc000) == 0 &&
				(ang_dif = pTgt->Atd[0].Ofs_y, pTgt->Damage_flg = pTgt->Damage_flg & 0xff00,
				at_h = (ULONG)pTgt->Atd[0].At_h,
				(pTgt->Pos_y + (INT)ang_dif + 100 + at_h) - pP->vy < (at_h + 100) * 2)))) {
				pAtw->Half_R_z += (pTgt->Eff_at_r >> 2);
				pAtw->R_x += (pTgt->Eff_at_r >> 2);
				ck_ret = Attack_r_ck1(pP, Cdir_y, (VECTOR *)pTgt->Atd, pAtw);
				if (ck_ret != 0) {
					pHit = pTgt;
					pHit->Damage_flg |= 1;
					if ((W_no & 0x10000) == 0) goto apply_dmg;
					pAwe = pEnemy_damage_tbl[pHit->Id - 0x10] + ((W_no & 0xffff) - 1);
					ck_ret = *(ULONG *)&pHit->Routine_0;
					pHit->Life = pHit->Life - ((USHORT)(pAwe->Damage >> (dmg_lv * 10)) & 0x3ff);
					if (ck_ret != 0xc02) {
						pHit->Tmp_routine = ck_ret;
					}
					*(UINT *)&pHit->Routine_0 = 0x2;		/* Routine_0 2, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
					if (pHit->Life < 0) {
						*(UINT *)&pHit->Routine_0 = 0x3;		/* Routine_0 3, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
					}
					pHit->Damage_no = 1;
					if ((W_no & 0x20000) != 0) {
						ofs_h = (INT)((ULONG)(USHORT)pHit->Atd[0].Ofs_y << 0x10) >> 0x11;
						if (pHit->Pos_y + ofs_h < pP->vy) {
							pHit->Damage_no = 0;
						}
						if (((pHit->Be_flg & 0x10000000) != 0) && (pP->vy < pHit->Pos_y + ofs_h * 3)) {
							pHit->Damage_no = 2;
						}
					}
					pHit->Routine_1 = (UCHAR)W_no;
					dmg_keep = pHit->Damage_cnt & 0x80;
					pHit->Damage_no = pHit->Damage_no + dmg_lv * 3;
					pHit->Damage_cnt = dmg_keep;
					pHit->Damage_cnt = dmg_keep | ((UCHAR)(pAwe->SubDamage >> 9) & 0x7f);
					ang_dif = Direction_ck(pP->vx, pP->vz, pHit->Pos_x, pHit->Pos_z);
					ang_dif = ang_dif - pHit->Cdir_y;
					if (((ang_dif + 0x400) & 0xfff) < 0x800) {
						pHit->Damage_flg |= 0x20;
					}
					if (((ang_dif + 0x600) & 0xfff) < 0x400) {
						pHit->Damage_flg |= 0x40;
					}
					if (((ang_dif - 0x200) & 0xfff) < 0x400) {
						pHit->Damage_flg |= 0x80;
					}
				} else {
					pAtw->Half_R_z = pAtw->Half_R_z - (pTgt->Eff_at_r >> 2);
					pAtw->R_x = pAtw->R_x - (pTgt->Eff_at_r >> 2);
				}
			}
			ppTgt++;
		} while (ppTgt != pG->ppEnemy_end);
		if ((W_no & 0x10000) != 0) {
			return (ULONG)pHit;
		}
apply_dmg:
		if (pHit != (ENEMY_WORK *)0x0) {
			pAwe = pEnemy_damage_tbl[pHit->Id - 0x10] + ((W_no & 0xffff) - 1);
			pHit->Life = pHit->Life - ((USHORT)(pAwe->Damage >> (dmg_lv * 10)) & 0x3ff);
			if (1 < *(ULONG *)&pHit->Routine_0 - 0xc02) {
				pHit->Tmp_routine = *(ULONG *)&pHit->Routine_0;
			}
			*(UINT *)&pHit->Routine_0 = 0x2;		/* Routine_0 2, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
			if (pHit->Life < 0) {
				*(UINT *)&pHit->Routine_0 = 0x3;		/* Routine_0 3, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
			}
			pHit->Damage_no = 1;
			if ((W_no & 0x20000) != 0) {
				ofs_h = (INT)((ULONG)(USHORT)pHit->Atd[0].Ofs_y << 0x10) >> 0x11;
				if (pHit->Pos_y + ofs_h < pP->vy) {
					pHit->Damage_no = 0;
				}
				if (((pHit->Be_flg & 0x10000000) != 0) && (pP->vy < pHit->Pos_y + ofs_h * 3)) {
					pHit->Damage_no = 2;
				}
			}
			if (((W_no & 0x40000) != 0) && (pHit->nFloor == (G.pEm)->nFloor)) {
				pHit->Damage_no = 0;
			}
			pHit->Routine_1 = (UCHAR)W_no;
			dmg_keep = pHit->Damage_cnt & 0x80;
			pHit->Damage_no = pHit->Damage_no + dmg_lv * 3;
			pHit->Damage_cnt = dmg_keep;
			pHit->Damage_cnt = dmg_keep | ((UCHAR)(pAwe->SubDamage >> 9) & 0x7f);
			ang_dif = Direction_ck(pP->vx, pP->vz, pHit->Pos_x, pHit->Pos_z);
			ang_dif = ang_dif - pHit->Cdir_y;
			if (((ang_dif + 0x400) & 0xfff) < 0x800) {
				pHit->Damage_flg |= 0x20;
			}
			if (((ang_dif + 0x600) & 0xfff) < 0x400) {
				pHit->Damage_flg |= 0x40;
			}
			if (0x3ff < ((ang_dif - 0x200) & 0xfff)) {
				return (ULONG)pHit;
			}
			pHit->Damage_flg |= 0x80;
			return (ULONG)pHit;
		}
	}
	return 0;
}

/*
 * Pos_pl_at_ck
 * line = 1829, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80047664 (Pos_pl_at_ck) */
ULONG Pos_pl_at_ck(VECTOR *pP, ULONG Len, LONG Down_life)
{
	SHORT ofs_y;
	INT dx;
	INT dz;
	ULONG life_ret;
	ULONG at_h;
	ENEMY_WORK *pTgt;
	ENEMY_WORK **ppTgt;
	ENEMY_WORK *pHit;
	INT loop_cnt;
	INT idx;

	pHit = (ENEMY_WORK *)0x0;
	loop_cnt = 1;
	idx = 0;
	if ((G.Status_flg & STAT_SUB_PLAYER) != 0) {
		loop_cnt = 2;
	}
	ppTgt = G.pEnemy;
	do {
		pTgt = *ppTgt;
		if (((((pTgt->Be_flg & 1) != 0) && (pTgt->Damage_cnt == 0)) && (-1 < pTgt->Life)) &&
		   ((((pTgt->Type & 0xc000) == 0 &&
			 (pTgt->Damage_flg = pTgt->Damage_flg & 0xff00, ofs_y = pTgt->Atd[0].Ofs_y,
			 at_h = (ULONG)pTgt->Atd[0].At_h,
			 (pTgt->Pos_y + (INT)ofs_y + at_h) - pP->vy < at_h << 1)) &&
			(dx = pP->vx - pTgt->Pos_x, dz = pP->vz - pTgt->Pos_z,
			at_h = SquareRoot0(dx * dx + dz * dz), at_h < Len)))) {
			pHit = pTgt;
			*(UINT *)&pHit->Routine_0 = 0x2;		/* Routine_0 2, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
			if (idx == 0) {
				life_ret = Pl_life_down(Down_life, 0);
				if (life_ret == 2) {
					*(UINT *)&pHit->Routine_0 = 0x3;		/* Routine_0 3, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
				}
			} else {
				pHit->Life = pHit->Life - (SHORT)Down_life;
				G.SaveData.Dummy[0] = G.SaveData.Dummy[0] - (SHORT)Down_life;
				if (pHit->Life < 0) {
					*(UINT *)&pHit->Routine_0 = 0x3;		/* Routine_0 3, Routine_1 0, Routine_2 0, Routine_3 0 (DS: one sw at +0x4) */
				}
			}
		}
		idx++;
		ppTgt++;
		loop_cnt = loop_cnt + -1;
	} while (loop_cnt != 0);
	return (ULONG)pHit;
}

/*
 * Bow_goto_parts_no
 * line = 1913, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80047858 (Bow_goto_parts_no) */
ULONG Bow_goto_parts_no(ENEMY_WORK *pTa, MATRIX *pM, BOW_HIT_PARTS *pBow_t)
{
	UCHAR part_no;
	INT dx;
	INT dz;
	INT dy;
	LONG dist;
	PARTS_W *pP_tgt;
	BOW_HIT_PARTS *pBow_hit;
	INT hit_idx;
	INT loop_cnt;
	ULONG best_dist;
	ULONG Hit_no;

	best_dist = 0x7fffffff;
	if (pTa->Id < 0x10) {
		return 0;
	}
	pBow_hit = pBow_t + (pTa->Id - 0x10);
	loop_cnt = 4;
	hit_idx = 0;
	do {
		part_no = pBow_hit->Hit0_parts;
		pP_tgt = pTa->pSin_parts_ptr;
		dx = pP_tgt[part_no].Workm.t[0] - pM->t[0];
		dz = pP_tgt[part_no].Workm.t[2] - pM->t[2];
		dist = dx * dx + dz * dz;
		dy = pP_tgt[part_no].Workm.t[1] - pM->t[1];
		if (dy < 0) {
			dy = -dy;
		}
		dy += SquareRoot0(dist);
		if ((ULONG)dy < best_dist) {
			Hit_no = (ULONG)pBow_hit->Hit0_parts | hit_idx << 0x10;
			best_dist = dy;
		}
		pBow_hit = (BOW_HIT_PARTS *)&pBow_hit->Hit1_parts;
		loop_cnt = loop_cnt + -1;
		hit_idx++;
	} while (loop_cnt != 0);
	return Hit_no;
}

/*
 * pl_mv05_w2
 * line = 1974, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004799c (pl_mv05_w2) */
VOID pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w2_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
	return;
}

/*
 * pl_mv05_w2_t1
 * line = 1983, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800479d8 (pl_mv05_w2_t1) */
VOID pl_mv05_w2_t1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG reload_ck;
	ULONG tama_ck;
	USHORT wep_type;
	/* DS 0x80011060: 10-byte initialiser template in .rdata, block-copied into the local (lwl/lwr, lh, swl/swr, sh) */
	USHORT mv_tbl[5] = { 0, 18, 12, 0, 15 };

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(INT *)&pEm->Move_no = (SHORT)mv_tbl[(ULONG)(pEm->Status_flg >> 0xd)] + 0x70000;
		G.Room_player_flg |= 0x18;
	}
	if ((Key & 0x100) == 0) {
		*(USHORT *)&pEm->Routine_2 = 0x3;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
		return;
	}
	wep_type = pEm->Type & 0xfff;
	if ((wep_type != 0xe) && (wep_type != 0x12)) {
		if (((Key & 0x10) != 0) && ((pEm->Status_flg & 0x8000) == 0)) {
			pEm->Routine_3 = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			return;
		}
		if (((Key & 0x20) != 0) && ((pEm->Status_flg & 0x2000) == 0)) {
			pEm->Routine_3 = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			return;
		}
		if (((Key & 0x30) == 0) && ((pEm->Status_flg & 0x4000) == 0)) {
			pEm->Routine_3 = 0;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
			return;
		}
	}
	if ((((G.SaveData.Weapon_mugen & 1) == 0) && ((G.System_flg & 0x840) != 0x840)) && ((G.Padd1_trg & 4) != 0)) {
		*(USHORT *)&pEm->Routine_2 = 0x5;		/* Routine_2 5, Routine_3 0 (DS: one sh at +0x6) */
	}
	if ((Key & 0x40) != 0) {
		tama_ck = Tama_ck_i(pEm->Type & 0xfff);
		if (tama_ck != 0) {
			*(USHORT *)&pEm->Routine_2 = 0x2;		/* Routine_2 2, Routine_3 0 (DS: one sh at +0x6) */
			return;
		}
		if ((G.Key_trg & 0x40) != 0) {
			reload_ck = Tama_reload_ck();
			if (reload_ck == 0) {
				Snd_se_stad(0x1010001, (VECTOR *)&pEm->Pos_x);
			} else {
				*(USHORT *)&pEm->Routine_2 = 0x4;		/* Routine_2 4, Routine_3 0 (DS: one sh at +0x6) */
			}
		}
	}
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -0x40;
	}
	if ((Key & 2) != 0) {
		pEm->Cdir_y += 0x40;
	}
	Joint_move(pEm, Kan, Seq, 0x200);
	return;
}

/*
 * pl_mv05_w2_t2
 * line = 2122, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80047c6c (pl_mv05_w2_t2) */
VOID pl_mv05_w2_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	USHORT mv_no_tp;
	ULONG reload_ck;
	ULONG ck_ret;
	ULONG wep_type;
	ULONG wep_no;
	UCHAR jtg_idx;
	/* DS 0x8001106c: 10-byte initialiser template in .rdata, block-copied into the local */
	USHORT mv_tbl[5] = { 0, 16, 10, 0, 13 };
	/* DS 0x80011078: second 10-byte template, block-copied into the second local */
	SHORT mv_tbl2[5] = { 0, 17, 11, 0, 14 };

	Hand_ck(pEm, *(LONG *)&pEm->pSin_parts_ptr[0xe].Workm.t[1], pEm->Type & 0xfff);
	jtg_idx = pEm->Be_flg >> 0x1d;
	switch(pEm->Routine_3) {
	case 0:
		*(INT *)&pEm->Move_no = (SHORT)mv_tbl[(ULONG)(pEm->Status_flg >> 0xd)] + 0x70000;
		pl_mv05_t2_init[(pEm->Type & 0xfff) - 1](pEm);
		Pad_vib_fade_set(10, 0, Pl_vib_tbl[(pEm->Type & 0xfff) - 2][2], 0);
	case 1:
		mv_no_tp = mv_tbl[(ULONG)(pEm->Status_flg >> 0xd)];
		pEm->Hokan_flg = 7;
		pEm->Move_no = (UCHAR)mv_no_tp;
		/* &Pl_weapon_tbl[(Type & 0xfff) - 1][Gun_jtg_bit[..]]. DS builds the address as row + (column + base): the row offset
		 * (Type * 3 << 3) first, then the column offset (Gun_jtg_bit[..] << 3) added to the table base less one row
		 * (0x800a68e8; the compiler folds the - 1 of the row index into the base), and the row is added last (here in the
		 * delay slot of the jal). The other callers in this file use the same form. */
		init_Weapon_at(pEm, &Pl_weapon_tbl[(pEm->Type & 0xfff) - 1][Gun_jtg_bit[jtg_idx]]);
		pEm->Routine_3 = 2;
		pEm->Timer3 = 9;
	case 2:
		wep_type = pEm->Type & 0xfff;
		if (wep_type == 0xf) {
			if ((ULONG)pEm->Move_cnt % 3 == 0) {
				ck_ret = Tama_decrease_i(0xf, 0);
				if (ck_ret == 0) {
					pEm->Routine_3 = 3;
					return;
				}
			}
		} else {
			ck_ret = Tama_decrease_i(wep_type, 0);
			if (ck_ret == 0) {
				pEm->Routine_3 = 3;
				return;
			}
		}
		G.Room_player_flg |= 1;
		wep_no = pEm->Type & 0xfff;
		Weapon_at_ck(pEm, (wep_no - 1) * 0x10000 | jtg_idx, &Pl_weapon_tbl[wep_no - 1][Gun_jtg_bit[jtg_idx]]);
		Joint_move(pEm, Kan, Seq, 0x200);
		/* per-frame routine of the weapon: pl_mv05_t2_spr[Type - 1], DS table base 0x800a6fdc + Type * 4 */
		pl_mv05_t2_spr[(pEm->Type & 0xfff) - 1](pEm);
		if ((Key & 0x100) == 0) {
			*(USHORT *)&pEm->Routine_2 = 0x3;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
			return;
		}
		if (((Key & 0x10) != 0) && ((pEm->Status_flg & 0x8000) == 0)) {
			pEm->Routine_3 = 1;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x8000;
			if ((pEm->Type & 0xfff) == 0x10) {
				Snd_se_stad(0x10d0001, (VECTOR *)&pEm->Pos_x);
				Pad_vib_set0(4, 0);
				Pad_vib_set1(5, 0xfa, 0);
			}
		}
		else if (((Key & 0x20) != 0) && ((pEm->Status_flg & 0x2000) == 0)) {
			pEm->Routine_3 = 1;
			pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x2000;
			if ((pEm->Type & 0xfff) == 0x10) {
				Snd_se_stad(0x10d0001, (VECTOR *)&pEm->Pos_x);
				Pad_vib_set0(4, 0);
				Pad_vib_set1(5, 0xfa, 0);
			}
		}
		else {
			if (((Key & 0x30) == 0) && ((pEm->Status_flg & 0x4000) == 0)) {
				pEm->Routine_3 = 1;
				pEm->Status_flg = (pEm->Status_flg & 0x1fff) | 0x4000;
				if ((pEm->Type & 0xfff) == 0x10) {
					Snd_se_stad(0x10d0001, (VECTOR *)&pEm->Pos_x);
				}
				return;
			}
			if ((Key & 0x40) == 0) {
				pEm->Routine_3 = 3;
				return;
			}
			if (((pEm->Water != 0) && (pEm->Water < pEm->Ground)) && ((pEm->Move_cnt & 3) == 0)) {
				Pl_water(pEm);
			}
			break;
		}
		return;
	case 3:
		pEm->Routine_3 = 4;
		*(INT *)&pEm->Move_no = mv_tbl2[(ULONG)(pEm->Status_flg >> 0xd)] + 0x70000;
	case 4:
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (CHAR)ck_ret;
		if ((Key & 0x40) != 0) {
			ck_ret = Tama_ck_i(pEm->Type & 0xfff);
			if (ck_ret != 0) {
				*(USHORT *)&pEm->Routine_2 = 0x102;		/* Routine_2 2, Routine_3 1 (DS: one sh at +0x6) */
				return;
			}
			if ((G.Key_trg & 0x40) != 0) {
				reload_ck = Tama_reload_ck();
				if (reload_ck == 0) {
					Snd_se_stad(0x1010001, (VECTOR *)&pEm->Pos_x);
				} else {
					*(USHORT *)&pEm->Routine_2 = 0x4;		/* Routine_2 4, Routine_3 0 (DS: one sh at +0x6) */
				}
			}
		}
		break;
	case 5:
		*(USHORT *)&pEm->Routine_2 = 0x1;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		break;
	}
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -0x20;
	}
	if ((Key & 2) == 0) {
		return;
	}
	pEm->Cdir_y += 0x20;
	return;
}


/*
 * pl_mv05_w2_t4
 * line = 2310, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800481cc (pl_mv05_w2_t4) */
VOID pl_mv05_w2_t4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG ck_ret;

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(UINT *)&pEm->Move_no = 0x70013;		/* Move_no 19, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
	}
	ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
	if (ck_ret != 0) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	}
	if ((*pEm->pNow_seq & 0x100000) != 0) {
		Snd_se_stad((*pEm->pNow_seq & 0xf0000) | 0x1000001, (VECTOR *)&pEm->Pos_x);
	}
	if (((*pEm->pNow_seq & 0x200000) != 0) && ((pEm->Status_flg & 1) != 0)) {
		Mag_set(pEm);
		pEm->Status_flg &= 0xfffe;
	}
	if ((*pEm->pNow_seq & 0x400000) != 0) {
		Tama_reload();
		pEm->Status_flg |= 1;
	}
	return;
}

/*
 * pl_mv05_w3
 * line = 2359, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800482d8 (pl_mv05_w3) */
VOID pl_mv05_w3(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w3_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
	return;
}

/*
 * pl_mv05_w3_t2
 * line = 2367, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80048314 (pl_mv05_w3_t2) */
VOID pl_mv05_w3_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	UCHAR sub_st;
	ULONG ck_ret;
	INT hit_res;
	INT hit_res2;
	ATTACK_WEAPON_TBL *At_tbl_top;
	SVECTOR esp_vec;
	/* DS 0x8001109c: 3-byte initialiser template in .rdata, byte-copied (lb/sb) into the local; indexed by sub_st */
	UCHAR jtg_tbl[3] = { 4, 1, 2 };
	UCHAR jtg_sel;

	switch (pEm->Routine_3) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x7000A;		/* Move_no 10, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_3 = 1;
		G.Room_player_flg |= 1;
		sub_st = 2;
		pl_mv05_t2_init[(pEm->Type & 0xfff) - 1](pEm);
		do {
			jtg_sel = jtg_tbl[sub_st];
			At_tbl_top = &Pl_weapon_tbl[(pEm->Type & 0xfff) - 1][Gun_jtg_bit[jtg_sel]];
			init_Weapon_at(pEm, At_tbl_top);
			ck_ret = Weapon_at_ck(pEm, ((pEm->Type & 0xfff) - 1) * 0x10000 | (ULONG)jtg_sel | 0x8000, At_tbl_top);
			*(ULONG *)&pEm->Sce_free2 = ck_ret;
			if (ck_ret != 0) break;
		} while (--sub_st != 0);
		Tama_decrease_i(pEm->Type & 0xfff, 0);
		Pad_vib_set0(3, 0);
		Pad_vib_set1(5, 0x96, 0);
	case 1:
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		}
		if ((pEm->Move_cnt == 4) && (hit_res = *(INT *)&pEm->Sce_free2, hit_res != 0)) {
			pEm->Routine_3 = 2;
			*(UINT *)&pEm->Move_no = 0xB;		/* Move_no 11, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
			Tama_decrease_i(pEm->Type & 0xfff, 1);
			Pl_weapon_vib(pEm);
		}
		if (pEm->Move_cnt == 1) {
			esp_vec.vx = 0x82;
			esp_vec.vy = 0x514;
			esp_vec.vz = 0;
			Esp_call(0x1281800, 0, &pEm->pSin_parts_ptr[0xb].Workm, &esp_vec);
			if ((pEm->Move_cnt == 1) && (hit_res2 = *(INT *)&pEm->Sce_free2, hit_res2 == 0)) {
				esp_vec.vx = 0x82;
				esp_vec.vy = 0x6b8;
				esp_vec.vz = 0;
				Esp_call(0x6031000, pEm->Cdir_y, &pEm->pSin_parts_ptr[0xb].Workm, &esp_vec);
			}
		}
		if ((((pEm->Water != 0) && (pEm->Water < pEm->Ground)) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		break;
	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
			*(UINT *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y (DS: one sw at +0x144) */
			pEm->Spd_z = 0;
		}
		Add_speedXZ(pEm, 0);
		if ((0x14 < pEm->Move_cnt) && ((Key & 0x100) == 0)) {
			*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
			*(UINT *)&pEm->Spd_x = 0;		/* Spd_x and Spd_y (DS: one sw at +0x144) */
			pEm->Spd_z = 0;
			return;
		}
		if ((((pEm->Water != 0) && (pEm->Water < pEm->Ground)) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		if (pEm->Move_cnt == 1) {
			esp_vec.vx = 0x82;
			esp_vec.vy = 0x6b8;
			esp_vec.vz = 0;
			Esp_call(0x6021000, pEm->Cdir_y, &pEm->pSin_parts_ptr[0xb].Workm, &esp_vec);
		}
		G.Status_flg &= ~STAT_BG_HOLD;
		break;
	}
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -8;
	}
	if ((Key & 2) != 0) {
		pEm->Cdir_y += 8;
	}
	if (((Key & 0x100) == 0) && ((Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][2] & 0x7f) < pEm->Move_cnt)) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
	}
	return;
}


/*
 * pl_mv05_w4
 * line = 2534, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x8004879c (pl_mv05_w4) */
VOID pl_mv05_w4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w4_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
	return;
}

/*
 * pl_mv05_w4_t2
 * line = 2545, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x800487d8 (pl_mv05_w4_t2) */
VOID pl_mv05_w4_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG ck_ret;
	ULONG wep_type;
	UCHAR mv_cnt;
	UCHAR jtg_idx;
	/* DS 0x80011010: 10-byte initialiser template in .rdata, block-copied into the local (lwl/lwr, lh, swl/swr, sh) */
	USHORT mv_tbl[5] = { 0, 14, 10, 0, 12 };

	if (pEm->Routine_3 == 0) {
		pEm->Routine_3 = 1;
		*(INT *)&pEm->Move_no =
			 *(SHORT *)((int)mv_tbl + (ULONG)(pEm->Status_flg >> 0xd) * 2) + 0x70000;
		G.Room_player_flg |= 1;
		pl_mv05_t2_init[(pEm->Type & 0xfff) - 1](pEm);
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
	}
	Hand_ck(pEm, pEm->pSin_parts_ptr[0xe].Workm.t[1], pEm->Type & 0xfff);
	jtg_idx = pEm->Be_flg >> 0x1d;
	if (Flg_ck(G.SaveData.Common_flg, 0x7e) != 0 && pEm->Move_cnt == 3) {
		pEm->Move_cnt = 10;
		pEm->Hokan_flg = 3;
	}
	mv_cnt = pEm->Move_cnt;
	if (mv_cnt != 0) {
		if ((mv_cnt != 3) && (mv_cnt != 6)) goto skip_shot_init;
		if (((Key & 0x40) == 0) || (ck_ret = Tama_ck_i(pEm->Type & 0xfff), ck_ret == 0)) {
			pEm->Move_cnt = 10;
			pEm->Hokan_flg = 3;
			goto skip_shot_init;
		}
	}
	init_Weapon_at(pEm, &Pl_weapon_tbl[(pEm->Type & 0xfff) - 1][Gun_jtg_bit[jtg_idx]]);
	Tama_decrease_i(pEm->Type & 0xfff, 0);
	Pl_view_esp_set(pEm, pEm->pSin_parts_ptr + 0xb);
	Pl_weapon_vib(pEm);
skip_shot_init:
	wep_type = pEm->Type & 0xfff;
	Weapon_at_ck(pEm, (wep_type - 1) * 0x10000 | jtg_idx, &Pl_weapon_tbl[wep_type - 1][Gun_jtg_bit[jtg_idx]]);
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -8;
	}
	if ((Key & 2) != 0) {
		pEm->Cdir_y += 8;
	}
	if (((Key & 0x100) == 0) && ((Pl_weapon_kyan[(pEm->Type & 0xfff) - 1][2] & 0x7f) < pEm->Move_cnt)) {
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		pEm->Spd_x = 0;
		pEm->Spd_y = 0;
		pEm->Spd_z = 0;
	} else {
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
			pEm->Spd_x = 0;
			pEm->Spd_y = 0;
			pEm->Spd_z = 0;
		}
		if ((((pEm->Water != 0) && (pEm->Water < pEm->Ground)) && (pEm->Move_cnt < 6)) && ((pEm->Move_cnt & 1) != 0)) {
			Pl_water(pEm);
		}
		/* per-frame routine of the weapon: pl_mv05_t2_spr[Type - 1], DS table base 0x800a6fdc + Type * 4 */
		pl_mv05_t2_spr[(pEm->Type & 0xfff) - 1](pEm);
	}
	return;
}

/*
 * pl_mv05_w5
 * line = 2702, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80048b50 (pl_mv05_w5) */
VOID pl_mv05_w5(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	pl_mv05_w5_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
	return;
}

/*
 * pl_mv05_w5_t2
 * line = 2710, file = D:\Bio2\prog\PL_WEP2.C
 */
/* DS 0x80048b8c (pl_mv05_w5_t2) */
VOID pl_mv05_w5_t2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
	ULONG reload_ck;
	ULONG wep_type;
	ULONG ck_ret;
	UCHAR jtg_idx;

	jtg_idx = 2;
	switch(pEm->Routine_3) {
	case 0:
		*(UINT *)&pEm->Move_no = 0x7000a;		/* Move_no 10, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Status_flg |= 0x4000;
		pl_mv05_t2_init[(pEm->Type & 0xfff) - 1](pEm);
		init_Weapon_at(pEm, &Pl_weapon_tbl[(pEm->Type & 0xfff) - 1][Gun_jtg_bit[jtg_idx]]);
		pEm->Routine_3 = 1;
		Snd_se_stad(0x10b0001, (VECTOR *)&pEm->Pos_x);
		Pad_vib_fade_set(0xf, 0, Pl_vib_tbl[(pEm->Type & 0xfff) - 2][2], 0);
	case 1:
		Gat_lp_st(pEm);
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		if (ck_ret != 0) {
			*(UINT *)&pEm->Move_no = 0x7000b;		/* Move_no 11, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
			pEm->Routine_3 = 2;
		}
		break;
	case 2:
		Gat_lp_st(pEm);
		G.Room_player_flg |= 1;
		wep_type = pEm->Type & 0xfff;
		Weapon_at_ck(pEm, (wep_type - 1) * 0x10000 | jtg_idx, &Pl_weapon_tbl[wep_type - 1][Gun_jtg_bit[jtg_idx]]);
		Joint_move(pEm, Kan, Seq, 0x200);
		/* per-frame routine of the weapon: pl_mv05_t2_spr[Type - 1], DS table base 0x800a6fdc + Type * 4 */
		pl_mv05_t2_spr[(pEm->Type & 0xfff) - 1](pEm);
		if ((Key & 0x100) == 0) {
			*(USHORT *)&pEm->Routine_2 = 0x0003;		/* Routine_2 3, Routine_3 0 (DS: one sh at +0x6) */
			Snd_se_stad(0x10c0001, (VECTOR *)&pEm->Pos_x);
			return;
		}
		if ((Key & 0x40) == 0) {
			pEm->Routine_3 = 3;
			return;
		}
		if (((pEm->Water != 0) && (pEm->Water < pEm->Ground)) && ((pEm->Move_cnt & 3) == 0)) {
			Pl_water(pEm);
		}
		break;
	case 3:
		*(UINT *)&pEm->Move_no = 0x7000d;		/* Move_no 13, Move_cnt 0, Hokan_flg 7, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_3 = 4;
		pEm->Spd_x = 0;
		Snd_se_stad(0x10c0001, (VECTOR *)&pEm->Pos_x);
	case 4:
		ck_ret = Joint_move(pEm, Kan, Seq, 0x200);
		pEm->Routine_3 = pEm->Routine_3 + (CHAR)ck_ret;
		if ((Key & 0x40) != 0) {
			ck_ret = Tama_ck_i(pEm->Type & 0xfff);
			if (ck_ret != 0) {
				*(USHORT *)&pEm->Routine_2 = 0x0102;		/* Routine_2 2, Routine_3 1 (DS: one sh at +0x6) */
				return;
			}
			if ((G.Key_trg & 0x40) != 0) {
				reload_ck = Tama_reload_ck();
				if (reload_ck == 0) {
					Snd_se_stad(0x1010001, (VECTOR *)&pEm->Pos_x);
				} else {
					*(USHORT *)&pEm->Routine_2 = 0x0004;		/* Routine_2 4, Routine_3 0 (DS: one sh at +0x6) */
					break;
				}
			}
		}
		break;
	case 5:
		*(USHORT *)&pEm->Routine_2 = 0x0001;		/* Routine_2 1, Routine_3 0 (DS: one sh at +0x6) */
		break;
	}
	if ((Key & 8) != 0) {
		pEm->Cdir_y = pEm->Cdir_y + -8;
	}
	if ((Key & 2) != 0) {
		pEm->Cdir_y += 8;
	}
	return;
}
