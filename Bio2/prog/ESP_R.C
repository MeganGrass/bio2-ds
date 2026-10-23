/*
 * ESP_R.C - Effect Sprite (ESP) per-state routines (Esp_00..Esp_5f)
 * D:\Bio2\prog\ESP_R.C
 *
 * Function names and addresses from the Ghidra SLUS_00748 symbol table
 * (DS addresses). Each body Esp_00..Esp_5f follows the retail decompile of
 * that function. Field offsets are the ESP_WORK
 * members; Ghidra "pESP + n" (pointer-sized) steps are mapped to members.
 *
 * Functions (DS address order):
 *   Esp_00 @ 8001dc28   Esp_01 @ 8001dc30   Esp_02 @ 8001dd2c
 *   Esp_03 @ 8001de88   Esp_04 @ 8001df50   Esp_05 @ 8001df90
 *   Esp_06 @ 8001e11c   Esp_07 @ 8001e154   Esp_08 @ 8001e200
 *   Esp_09 @ 8001e390   Esp_0a @ 8001e3c4   Esp_0b @ 8001e4b4
 *   Esp_0c @ 8001e638   Esp_0d @ 8001e898   Esp_0e @ 8001e8b0
 *   Esp_0f @ 8001ed9c   Esp_10 @ 8001f128   Esp_11 @ 8001f198
 *   Esp_12 @ 8001f2a0   Esp_13 @ 8001f2c0   Esp_14 @ 8001f530
 *   Esp_15 @ 8001f5a4   Esp_16 @ 8001f634   Esp_17 @ 8001f6a4
 *   Esp_18 @ 8001f6e0   Esp_19 @ 8001fa08   Esp_1a @ 8001fa6c
 *   Esp_1b @ 8001fa9c   Esp_1c @ 8001fbd0   Esp_1d @ 8001fd5c
 *   Esp_1e @ 8001fecc   Esp_1f @ 8001ff98   Esp_20 @ 800200b0
 *   Esp_21 @ 80020298   Esp_22 @ 80020364   Esp_23 @ 800204f8
 *   Esp_24 @ 80020510   Esp_25 @ 80020660   Esp_26 @ 800206e0
 *   Esp_27 @ 800206fc   Esp_28 @ 80020758   Esp_29 @ 800207e4
 *   Esp_2a @ 800208b8   Esp_2b @ 80020950   Esp_2c @ 800209cc
 *   Esp_2d @ 80020abc   Esp_2e @ 80020b60   Esp_2f @ 80020c3c
 *   Esp_30 @ 80020f3c   Esp_31 @ 800215c8   Esp_32 @ 80021970
 *   Esp_33 @ 800219b4   Esp_34 @ 80021a48   Esp_35 @ 80021b00
 *   Esp_36 @ 80021ce4   Esp_37 @ 80021d24   Esp_38 @ 80021e14
 *   Esp_39 @ 80022094   Esp_3a @ 80022254   Esp_3b @ 800223f8
 *   Esp_3c @ 80022454   Esp_3d @ 80022474   Esp_3e @ 8002256c
 *   Esp_3f @ 800226ac   Esp_40 @ 80022728   Esp_41 @ 80022774
 *   Esp_42 @ 800228d8   Esp_43 @ 800229e4   Esp_44 @ 80022abc
 *   Esp_45 @ 80022ebc   Esp_46 @ 80023204   Esp_47 @ 80023554
 *   Esp_48 @ 8002373c   Esp_49 @ 8002387c   Esp_4a @ 800238e4
 *   Esp_4b @ 800239c4   Esp_4c @ 80023b94   Esp_4d @ 80023c70
 *   Esp_4e @ 800241c8   Esp_4f @ 8002434c   Esp_50 @ 8002453c
 *   Esp_51 @ 800246e8   Esp_52 @ 80024818   Esp_53 @ 800252b8
 *   Esp_54 @ 80025348   Esp_55 @ 8002539c   Esp_56 @ 80025408
 *   Esp_57 @ 8002563c   Esp_58 @ 80025754   Esp_59 @ 8002575c
 *   Esp_5a @ 80025764   Esp_5b @ 8002576c   Esp_5c @ 80025774
 *   Esp_5d @ 8002577c   Esp_5e @ 80025784   Esp_5f @ 8002578c
 */

#include "TYPES.H"
#include "CD.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <STRINGS.H>
#include "game.h"
#include "ESP_R.H"
#include "DIEDEMO.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "MAIN.H"
#include "PL_WEP2.H"
#include "SCA.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "OBA.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCE2.H"
#include "SUB.H"
#include "DIVCHK.H"

/* DS divides and takes remainders of Rnd() with the signed multiply-high sequences (0x55555556, 0x88888889, 0x66666667 and sra),
 * so these callers see Rnd() as a signed int, not as the UCHAR it is declared as. Rnd_s32() calls it through a LONG-returning type. */
#define Rnd_s32()	(((LONG (*)(VOID))Rnd)())

/* ------------------------------------------------------------------ */
/* Esp_00 - no-op (DS 8001dc28)                                         */
/* ------------------------------------------------------------------ */
/* DS 0x8001dc28 (Esp_00) */
void Esp_00(void)
{
	return;
}

/* ------------------------------------------------------------------ */
/* Esp_01 - Load next move-sequence step, animation time from frame    */
/*          (DS 8001dc30)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001dc30 (Esp_01) */
void Esp_01(void)
{
	SSEQUENCE *pAnm;

	pESP->Be_flg = pESP->Free3;
	pESP->Tpage |= pESP->Free4;
	pESP->Anm_cnt = pESP->Free0;
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	pESP->Mov_cnt += pESP->Free2;
	*(MSEQUENCE *)pESP = pESP->pMoveseq[pESP->Mov_cnt];
}

/* ------------------------------------------------------------------ */
/* Esp_02 - As Esp_01, with the animation frame picked at random        */
/*          from Free0 .. Free0 + Free5 (DS 8001dd2c)                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001dd2c (Esp_02) */
void Esp_02(void)
{
	LONG Anm_off;
	UCHAR *pTime;

	pESP->Be_flg = pESP->Free3;
	pESP->Tpage |= pESP->Free4;
	/* DS: signed remainder with the zero and overflow checks. DS divides the
	 * Rnd() return register directly, with no andi 0xff after the jal, so
	 * the call goes through Rnd_s32() (Rnd seen as LONG-returning). */
	MOD_S(Anm_off, Rnd_s32(), (LONG)pESP->Free5 + 1);
	pESP->Anm_cnt = (UCHAR)(pESP->Free0 + (CHAR)Anm_off);
	/* DS addresses the Time byte first, then copies it. */
	pTime = &pESP->pAnmseq[pESP->Anm_cnt].Time;
	pESP->Anm_time = *pTime;
	pESP->Mov_cnt += pESP->Free2;
	*(MSEQUENCE *)pESP = pESP->pMoveseq[pESP->Mov_cnt];
}

/* ------------------------------------------------------------------ */
/* Esp_03 - Step move sequence when Free1 counter reaches 0 (DS 8001de88) */
/* ------------------------------------------------------------------ */
/* DS 0x8001de88 (Esp_03) */
void Esp_03(void)
{
	if (pESP->Free1 != 0) {
		pESP->Free1--;
	} else {
		pESP->Mov_cnt += pESP->Free2;
		*(MSEQUENCE *)pESP = pESP->pMoveseq[pESP->Mov_cnt];
	}
}

/* ------------------------------------------------------------------ */
/* Esp_04 - Advance via Esp_01 when Free1 counter reaches 0 (DS 8001df50) */
/* ------------------------------------------------------------------ */
/* DS 0x8001df50 (Esp_04) */
void Esp_04(void)
{
	if (pESP->Free1 != 0) {
		pESP->Free1--;
	} else {
		Esp_01();
	}
}

/* ------------------------------------------------------------------ */
/* Esp_05 - Ground/water check; dispatch by ground contact (DS 8001df90) */
/* ------------------------------------------------------------------ */
/* DS 0x8001df90 (Esp_05) */
void Esp_05(void)
{
	LONG water;
	LONG ground;
	UCHAR idx;
	VECTOR tvec;
	ESP_WORK *pEsp;

	water = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (water != 0 && water < pESP->Pos_y) {
		Esp_call3(pESP->Mag | 0x1a010000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		pEsp = pESP;
		pEsp->Routine1 = 0;
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		if (pESP->Id == 0x1d) {
			ESP_TBLJ_CALL(pESP->Free0);
		}
	} else {
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
		if (ground < tvec.vy) {
			ESP_TBLJ_CALL(pESP->Free0);
		} else if (Ground_in_flg != 0) {
			idx = (ground != ESP_GROUND_Y(pESP)) ? pESP->Free0 : pESP->Free1;
			ESP_TBLJ_CALL(idx);
		} else {
			ESP_GROUND_Y(pESP) = ground;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_06 - Count Free0 down; at zero, kill (DS 8001e11c)               */
/* ------------------------------------------------------------------ */
/* DS 0x8001e11c (Esp_06) */
void Esp_06(void)
{
	ESP_WORK *p = pESP;

	if (p->Free0 != 0) {
		p->Free0--;
	} else {
		p->Routine1 = 0;
		p->Routine0 = 0;
		p->Be_flg = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_07 - Start sound (plus light flag when Padd0 is 0), then Esp_01 */
/*          (DS 8001e154)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001e154 (Esp_07) */
void Esp_07(void)
{
	ULONG bits;
	VECTOR vec;

	pESP->Type += pESP->Free1;
	/* DS loads Free5 (lhu at DS 0x8001e188) between the Pos_x load and its store,
	   then shifts it into a separate register (sll v0,a0,16 at DS 0x8001e1a8) and
	   adds 1 in the branch delay slot (DS 0x8001e1b8). Loading Free5 into bits
	   before the vec copies and forming the sound number after them gives that
	   order. */
	bits = pESP->Free5;
	vec.vx = pESP->Pos_x;
	vec.vy = pESP->Pos_y;
	vec.vz = pESP->Pos_z;
	bits = (bits << 16) + 1;
	/* DS reads Padd0 with lb (DS 0x8001e1ac) and branches with beq to the Free5
	   sound, so the 0x31f0001 call is the fall-through arm. */
	if ((signed char)pESP->Padd0 != 0) {
		Snd_se_stad(0x31f0001, &vec);
	} else {
		Snd_se_stad(bits, &vec);
		Game.Plight_flg = 1;
	}
	Esp_01();
}

/* ------------------------------------------------------------------ */
/* Esp_08 - Random scatter of animation, position and hot-spot          */
/*          (DS 8001e200)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001e200 (Esp_08) */
void Esp_08(void)
{
	SSEQUENCE *pAnm;

	Esp_01();
	pESP->Transx = (USHORT)(pESP->Transx + (Rnd_s32() % 5) * 0x200);
	pESP->Anm_cnt = (UCHAR)(pESP->Anm_cnt + (Rnd_s32() % 2));
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	pESP->Hot_x += (signed char)pESP->Free0;
	pESP->Hot_y += (signed char)pESP->Free1;
	pESP->Hot_z += (signed char)pESP->Free2;
	pESP->Cdir_x = (SHORT)(pESP->Free3 + (Rnd_s32() % 5) * 3);
	pESP->Cdir_y = (SHORT)(pESP->Free4 + (Rnd_s32() % 5) * 3);
}

/* ------------------------------------------------------------------ */
/* Esp_09 - Transx += 0x100, then Esp_14 (DS 8001e390)                  */
/* ------------------------------------------------------------------ */
/* DS 0x8001e390 (Esp_09) */
void Esp_09(void)
{
	pESP->Transx += 0x100;
	Esp_14();
}

/* ------------------------------------------------------------------ */
/* Esp_0a - Random speed kick (not for Type 6) (DS 8001e3c4)            */
/* ------------------------------------------------------------------ */
/* DS 0x8001e3c4 (Esp_0a) */
void Esp_0a(void)
{
	SSEQUENCE *pAnm;

	pESP->Be_flg = 0x8003;
	pESP->Anm_cnt = (UCHAR)pESP->Free3;
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	if (pESP->Type != 6) {
		pESP->Speed_z = (SHORT)((Rnd_s32() % 4) * 5);
		pESP->Speed_y = (SHORT)(pESP->Speed_y + (Rnd_s32() % 3) * -10);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_0b - Scale Free0, Add_y/z and speeds by Mag/3000 (DS 8001e4b4)   */
/* ------------------------------------------------------------------ */
/* DS 0x8001e4b4 (Esp_0b) */
void Esp_0b(void)
{
	Esp_01();
	pESP->Add_x = (CHAR)((LONG)(signed char)pESP->Add_x * pESP->Mag / 3000);
	pESP->Add_y = (CHAR)((LONG)(signed char)pESP->Add_y * pESP->Mag / 3000);
	pESP->Add_z = (CHAR)((LONG)(signed char)pESP->Add_z * pESP->Mag / 3000);
	pESP->Speed_x = (SHORT)((LONG)pESP->Speed_x * pESP->Mag / 3000);
	pESP->Speed_y = (SHORT)((LONG)pESP->Speed_y * pESP->Mag / 3000);
	pESP->Speed_z = (SHORT)((LONG)pESP->Speed_z * pESP->Mag / 3000);
}

/* ------------------------------------------------------------------ */
/* Esp_0c - Spawn a child effect per frame until Padd2 runs out         */
/*          (DS 8001e638)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001e638 (Esp_0c) */
void Esp_0c(void)
{
	ULONG uRet;
	ESP_WORK *pNew;
	ESP_WORK *pEsp;

	if (pESP->Free2 != 0) {
		if ((signed char)pESP->Padd0 < 10 && pESP->Anm_cnt == pESP->Free0) {
			pESP->Padd0++;
			/* DS sets up the Esp_call3 arguments in each Padd0 branch and shares one jal; Mag * 800 / 1000 is a signed divide. */
			if ((signed char)pESP->Padd0 > 5) {
				uRet = Esp_call3((ULONG)pESP->Id << 24 | (ULONG)pESP->Type << 16 |
					(pESP->Mag * 800) / 1000, pESP->DirY, pESP->pMat, (SVECTOR *)&pESP->Hot_x);
			} else {
				uRet = Esp_call3((ULONG)pESP->Id << 24 | (ULONG)pESP->Type << 16 | pESP->Mag, pESP->DirY, pESP->pMat, (SVECTOR *)&pESP->Hot_x);
			}
			if (uRet < 0xff) {
				pNew = &Esp[uRet];	/* DS forms &Esp[uRet] once for both stores */
				pNew->Id2 = pESP->Id2;
				pNew->Padd0 = pESP->Padd0;
			}
		}
		pEsp = pESP;	/* DS loads pESP once for the Anm_cnt > 0x12 block */
		if (pEsp->Anm_cnt > 0x12) {
			pEsp->Add_y = 0;
			pEsp->Mag = (USHORT)((pEsp->Mag * 0x3d4) / 1000);
		}
		if (pESP->Anm_cnt > 0x14) {
			pESP->Speed_y = 0;
		}
	} else {
		if (pESP->Anm_cnt == pESP->Free0) {
			uRet = Esp_call3((ULONG)pESP->Id << 24 | (ULONG)pESP->Type << 16 | pESP->Mag, pESP->DirY, pESP->pMat, (SVECTOR *)&pESP->Hot_x);
			if (uRet < 0xff) {
				pNew = &Esp[uRet];
				pNew->Id2 = pESP->Id2;
				pNew->Padd0 = pESP->Padd0;
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_0d - Set Be_flg 0xa003, Routine0 = 0 (DS 8001e898)               */
/* ------------------------------------------------------------------ */
/* DS 0x8001e898 (Esp_0d) */
void Esp_0d(void)
{
	pESP->Be_flg = 0xa003;
	pESP->Routine0 = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_0e - Seven-state sequence on Free0: debris burst on the player     */
/*          and scene fades with a timed Kido ramp (DS 8001e8b0)        */
/* ------------------------------------------------------------------ */
/* DS 0x800108d8 (Esp_aw_0e) */
const ATTACK_WORK Esp_aw_0e = { -200, 0, 100, 50 };

/* DS 0x8001e8b0 (Esp_0e) */
void Esp_0e(void)
{
	SVECTOR svec;
	VECTOR tvec;
	ATTACK_WORK Aw;
	ENEMY_WORK *pHit;
	ESP_WORK *pEsp;
	LONG dist;
	LONG extra_z;
	LONG dir;
	SHORT cdir_z;
	ULONG count;
	ULONG step_x;
	ULONG bRet;
	ULONG Kido;

	Aw = Esp_aw_0e;
	switch (pESP->Free0) {
	case 0:
		pESP->Be_flg = pESP->Free3;
		Game.Plight_flg = 1;
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		Snd_se_stad(0x1100001, &tvec);
		pHit = PL_Hit_em(&G.Player);
		Pos_em_at_ck((VECTOR *)&pHit->Atd[0], pESP->DirY, &Aw, 0x3000e);
		extra_z = 0;
		if (pHit->Atd[0].Y > tvec.vy) {
			extra_z = (pHit->Atd[0].Y - tvec.vy) / 5;
		} else if (tvec.vy > pHit->Atd[0].Y) {
			extra_z = (tvec.vy - pHit->Atd[0].Y) / 5;
		}
		/* DS reuses tvec for the absolute distances on each axis */
		tvec.vx = pHit->Atd[0].X - pESP->Pos_x;
		if (tvec.vx < 0) {
			tvec.vx = -tvec.vx;
		}
		tvec.vy = pHit->Atd[0].Y - pESP->Pos_y;
		if (tvec.vy < 0) {
			tvec.vy = -tvec.vy;
		}
		tvec.vz = pHit->Atd[0].Z - pESP->Pos_z;
		if (tvec.vz < 0) {
			tvec.vz = -tvec.vz;
		}
		dist = SquareRoot0(tvec.vx * tvec.vx + tvec.vz * tvec.vz);
		dist = SquareRoot0(tvec.vy * tvec.vy + dist * SquareRoot0(tvec.vx * tvec.vx + tvec.vz * tvec.vz));
		dir = Snd_se_dir_ck(pESP->Pos_x, pESP->Pos_z, pHit->Atd[0].X, pHit->Atd[0].Z) - pESP->DirY;
		count = (ULONG)dist / 0x1b0;
		svec.vx = pESP->Hot_x;
		svec.vy = pESP->Hot_y;
		svec.vz = pESP->Hot_z;
		if (count == 0) {
			count = 1;
		}
		DIV_U(step_x, (ULONG)(pHit->Atd[0].Y - pESP->Pos_y), count);	/* DS: unsigned divide with the zero check */
		do {
			count--;
			bRet = Esp_call3(0x6012600, (SHORT)dir, (MATRIX *)pESP->m, &svec);
			if (bRet < 0xff) {
				pEsp = &Esp[bRet];
				pEsp->Cdir_x = (SHORT)((Rnd_s32() % 30) * 10 - 150);
				pEsp->Cdir_y = (SHORT)((Rnd_s32() % 50) * 40 - 1000);
				cdir_z = (Rnd_s32() % 20) * 30 + 0x338;
				pEsp->Cdir_z = (SHORT)(cdir_z + (((Rnd() & 2) != 0) ? extra_z + 0x800 : extra_z));
			} else {
				count = 0;
			}
			svec.vy += 0x1b0;
			svec.vx -= (SHORT)step_x;
		} while (count != 0);
		Fade_set(0x100, 0, 7, 1);
		Kido = 0x2000;
		break;
	case 1:
		Kido = 0x1800;
		break;
	case 2:
		Kido = 0x1600;
		break;
	case 3:
		Fade_off(0);
		goto next;
	case 4:
		Fade_set(0x100, 0, 7, 1);
		Kido = 0x1400;
		break;
	case 5:
		Kido = 0x1200;
		break;
	case 6:
		Fade_off(0);
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
	case 7:
	default:
		goto next;
	}
	Fade_adjust(0, Kido, 0x808080, (RECT *)0);
next:
	pESP->Free0++;
}

/* ------------------------------------------------------------------ */
/* Esp_0f - Ground/water test; bounce or hit, then dispatch (DS 8001ed9c) */
/* ------------------------------------------------------------------ */
/* DS 0x80010900 (Esp_aw_0f) */
const ATTACK_WORK Esp_aw_0f = { -1400, 0, 350, 250 };

/* DS 0x8001ed9c (Esp_0f) */
void Esp_0f(void)
{
	/* retail reserves 8 more bytes of locals (Esp_0f frame 64); never read */
	volatile int Rsv[2];
	ATTACK_WORK Aw;
	VECTOR tvec;
	LONG lWater;
	LONG ground;
	ULONG hitA;
	ULONG hitB;
	UCHAR idx;

	Aw = Esp_aw_0f;
	lWater = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (lWater == 0 || pESP->Pos_y <= lWater) {
		pESP->Free2--;
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
		ESP_GROUND_Y(pESP) = ground - 10;
		tvec.vy += 1000;
		hitA = Pos_em_at_ck(&tvec, pESP->DirY, &Aw, (LONG)(signed char)pESP->Padd0 + 0x30009);
		tvec.vy -= 2000;
		hitB = Pos_em_at_ck(&tvec, pESP->DirY, &Aw, (LONG)(signed char)pESP->Padd0 + 0x30009);
		if (hitA + hitB == 0 && pESP->Free2 != 0) {
			if (Ground_in_flg == 0) {
				return;
			}
			{
				SHORT sx;
				SHORT sy;
				SHORT sz;

				pESP->Speed_x -= (signed char)pESP->Add_x;
				pESP->Speed_y -= (signed char)pESP->Add_y;
				sx = pESP->Speed_x;
				pESP->Speed_z -= (signed char)pESP->Add_z;
				pESP->Speed_x -= (signed char)pESP->Add_x;
				pESP->Mov_x -= sx;
				sy = pESP->Speed_y;
				pESP->Speed_y -= (signed char)pESP->Add_y;
				pESP->Mov_y -= sy;
				sz = pESP->Speed_z;
				pESP->Speed_z -= (signed char)pESP->Add_z;
				pESP->Mov_z -= sz;
				pESP->Mov_x -= pESP->Speed_x / 3;
				pESP->Mov_y -= pESP->Speed_y / 3;
				pESP->Mov_z -= pESP->Speed_z / 3;
				Esp_pos_matrix();
			}
			idx = pESP->Free1;
		} else {
			G.Wep_hit_pos.vx = pESP->Pos_x;
			G.Wep_hit_pos.vy = pESP->Pos_y;
			G.Wep_hit_pos.vz = pESP->Pos_z;
			pESP->Be_flg |= 0x80;
			idx = pESP->Free0;
		}
	} else {
		pESP->Pos_y += 500;
		Esp_call3(0x1a051c00, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		idx = pESP->Free0;
	}
	ESP_TBLJ_CALL(idx + (signed char)pESP->Padd0);
}

/* ------------------------------------------------------------------ */
/* Esp_10 - Snap to ground, store ground height, set Routine state      */
/*          (DS 8001f128)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001f128 (Esp_10) */
void Esp_10(void)
{
	VECTOR tvec;
	LONG ground;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
	ESP_GROUND_Y(pESP) = ground;
	pESP->Routine0 = pESP->Free2;
	pESP->Routine1 = 5;
}

/* ------------------------------------------------------------------ */
/* Esp_11 - Player-type dependent setup, then Esp_0f (DS 8001f198)      */
/* ------------------------------------------------------------------ */
/* DS 0x8001f198 (Esp_11) */
void Esp_11(void)
{
	/* retail reserves 24 more bytes of locals (Esp_11 frame 48); never read */
	volatile int Rsv[6];
	SSEQUENCE *pAnm;
	LONG r;

	pESP->Padd0 = (CHAR)(G.Player.Type - 9);
	pESP->Routine0 = 0x16;
	pESP->Routine1 = 0x0f;
	/* DS stores Be_flg (sh 0x18), Anm_cnt (sb 0x21) and Tpage (lhu/ori/sh 0x2a) through one
	 * load of pESP, then reloads pESP (lw a0) only for the animation-time line. This statement
	 * order gives that shape; the three stores go to different fields, so the order does not
	 * change what is stored. Anm_time is read from the entry address pAnmseq + Anm_cnt * 8 (v1). */
	pESP->Be_flg = 0xb403;
	pESP->Tpage |= 0x20;
	pESP->Anm_cnt = 0x12;
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	if ((G.Player.Type & 0x1f) == 9) {
		/* DS uses the Rnd result in v0 directly (no andi 0xff before the divide by 3), so the
		 * call goes through Rnd_s32() (Rnd seen as LONG-returning). */
		r = Rnd_s32();
		pESP->Free2 = (UCHAR)(r % 3 + 10);
	} else {
		pESP->Free2 = 15;
	}
	Esp_0f();
}

/* ------------------------------------------------------------------ */
/* Esp_12 - Transx += 0x100 (DS 8001f2a0)                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001f2a0 (Esp_12) */
void Esp_12(void)
{
	pESP->Transx += 0x100;
}

/* ------------------------------------------------------------------ */
/* Esp_13 - Per-frame timer, then fade/scale sub-states by Padd0        */
/*          (DS 8001f2c0)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001f2c0 (Esp_13) */
void Esp_13(void)
{
	ESP_WORK *pEsp;
	SHORT padd1;

	if (pESP->Padd2 != 0) {
		if (pESP->Free5 > 0xf && pESP->Transx > 0x1000) {
			Esp_28();
		}
		pESP->Free5++;
	}
	pEsp = pESP;
	switch ((signed char)pEsp->Padd0) {
	case 0:
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		break;
	case 1:
		padd1 = pEsp->Padd1;
		if (padd1 != 0) {
			pEsp->Padd1 = padd1 - 1;
			pEsp->Transx = (LONG)pEsp->Transx * 0x3de / 1000;
			pEsp->Transy = (LONG)pEsp->Transy * 0x3d4 / 1000;
		} else {
			pEsp->Padd0 = 0;
		}
		break;
	case 2:
		padd1 = pEsp->Padd1;
		if (padd1 != 0) {
			pEsp->Padd1 = padd1 - 1;
			pEsp->Transx = (LONG)pEsp->Transx * 0x3f1 / 1000;
			pEsp->Transy = (LONG)pEsp->Transy * 0x3ea / 1000;
		} else {
			pEsp->Padd0 = 1;
			pESP->Padd1 = Rnd_s32() % 11 + 0x5a;
		}
		break;
	case 3:
		padd1 = pEsp->Padd1;
		if (padd1 != 0) {
			pEsp->Padd1 = padd1 - 1;
		} else {
			pEsp->Padd0 = 2;
			pESP->Padd1 = Rnd_s32() % 8 + 0x1e;
		}
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_14 - Ground probe; on ground contact clear Routine0 (DS 8001f530) */
/* ------------------------------------------------------------------ */
/* DS 0x8001f530 (Esp_14) */
void Esp_14(void)
{
	VECTOR tvec;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	Sca_get_ground(&tvec, 2, 0x2000, 0);
	if (Ground_in_flg != 0) {
		ESP_WORK *pEsp = pESP;	/* pESP is loaded once for both stores below (DS reuses the pointer) */
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_15 - Count Free0 down, emitting a call every other frame        */
/*          (DS 8001f5a4)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001f5a4 (Esp_15) */
void Esp_15(void)
{
	if (pESP->Free0 != 0) {
		if ((pESP->Free0 & 1) == 0 && (pESP->Pos_x + pESP->Pos_y + pESP->Pos_z) != 0) {
			Esp_call3(((ULONG)pESP->Mag + 0xb00) | 0x3090000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		}
		pESP->Free0--;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_16 - Emit one call scaled by Mag (DS 8001f634)                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001f634 (Esp_16) */
void Esp_16(void)
{
	/* Mag is read with lhu and the product Mag * 150 is divided by 100 as a signed value: DS multiplies by
	   0x51eb851f, shifts the high word right by 5 and subtracts the sign (sra v0,v0,0x1f; subu a0,a0,v0)
	   at 0x8001f664-0x8001f684, so the product is a LONG here. */
	Esp_call3((((LONG)pESP->Mag * 150) / 100) | 0x30b0000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
}

/* ------------------------------------------------------------------ */
/* Esp_17 - Emit one fixed call (DS 8001f6a4)                           */
/* ------------------------------------------------------------------ */
/* DS 0x8001f6a4 (Esp_17) */
void Esp_17(void)
{
	Esp_call3(0x30a1800, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
}

/* ------------------------------------------------------------------ */
/* Esp_18 - Bounds check, enemy hit test and ground bounce (DS 8001f6e0) */
/* ------------------------------------------------------------------ */
/* DS 0x80010908 (Esp_aw_18) */
const ATTACK_WORK Esp_aw_18 = { -800, 0, 400, 200 };

/* DS 0x8001f6e0 (Esp_18) */
void Esp_18(void)
{
	VECTOR tvec;
	ATTACK_WORK Aw;
	ULONG hit;
	ESP_WORK *pEsp;

	Aw = Esp_aw_18;
	pEsp = pESP;
	if (pEsp->Pos_x > 32000 || pEsp->Pos_z > 32000) {
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
	}
	pEsp = pESP;
	if (pEsp->Pos_x < -32000 || pEsp->Pos_z < -32000) {
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
	}
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	hit = Pos_em_at_ck(&tvec, pESP->DirY, &Aw, 0x30011);
	if (hit != 0) {
		G.Wep_hit_pos.vx = pESP->Pos_x;
		G.Wep_hit_pos.vy = pESP->Pos_y;
		G.Wep_hit_pos.vz = pESP->Pos_z;
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		tvec.vy = Sca_get_ground(&tvec, 2, 0x2000, 0);
		ESP_GROUND_Y(pESP) = tvec.vy - 10;
		hit = Pos_em_at_ck(&tvec, pESP->DirY, &Aw, 0x20011);
		if (hit != 0) {
			ESP_TBLJ_CALL(pESP->Free0);
		} else {
			if (Ground_in_flg == 0) {
				return;
			}
			{
				SHORT sx;
				SHORT sy;
				SHORT sz;

				pESP->Speed_x -= (signed char)pESP->Add_x;
				pESP->Speed_y -= (signed char)pESP->Add_y;
				sx = pESP->Speed_x;
				pESP->Speed_z -= (signed char)pESP->Add_z;
				pESP->Speed_x -= (signed char)pESP->Add_x;
				pESP->Mov_x -= sx;
				sy = pESP->Speed_y;
				pESP->Speed_y -= (signed char)pESP->Add_y;
				pESP->Mov_y -= sy;
				sz = pESP->Speed_z;
				pESP->Speed_z -= (signed char)pESP->Add_z;
				pESP->Mov_z -= sz;
				pESP->Mov_x -= pESP->Speed_x / 3;
				pESP->Mov_y -= pESP->Speed_y / 3;
				pESP->Mov_z -= pESP->Speed_z / 3;
				Esp_pos_matrix();
			}
			ESP_TBLJ_CALL(pESP->Free1);
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_19 - Water surface: kill when under the water line (DS 8001fa08) */
/* ------------------------------------------------------------------ */
/* DS 0x8001fa08 (Esp_19) */
void Esp_19(void)
{
	ESP_WORK *pE;
	LONG water;

	water = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (water != 0) {
		/* DS reads pESP once here, after the Water_ck call, and uses that
		 * copy for the Pos_y compare and both stores; the byte store to
		 * Routine0 does not reload pESP before the Be_flg store. */
		pE = pESP;
		if (water < pE->Pos_y) {
			pE->Routine0 = 0;
			pE->Be_flg = 0;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_1a - Integrate Transx and Transy by Free3 / Free4 (DS 8001fa6c)  */
/* ------------------------------------------------------------------ */
/* DS 0x8001fa6c (Esp_1a) */
void Esp_1a(void)
{
	pESP->Transx = (USHORT)(pESP->Transx + pESP->Free3);
	pESP->Transy = (USHORT)(pESP->Transy + pESP->Free4);
}

/* ------------------------------------------------------------------ */
/* Esp_1b - Switch to sprite set 0x3a/0x1c with random frame (DS 8001fa9c) */
/* ------------------------------------------------------------------ */
/* DS 0x8001fa9c (Esp_1b) */
void Esp_1b(void)
{
	VECTOR tvec;
	LONG ground;
	SSEQUENCE *pAnm;
	ESP_WORK *pEsp;
	ESP_WORK *pEsp2;

	pEsp = pESP;
	pEsp->Be_flg = 0xb003;
	pEsp->Tpage |= 0x20;
	pESP->Anm_cnt = (UCHAR)(Rnd_s32() % 3);
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
	pEsp2 = pESP;
	ESP_GROUND_Y(pEsp2) = ground;
	pEsp2->Routine0 = 0x3a;
	pESP->Routine1 = 0x1c;
	pESP->Padd0 = 2;
	pESP->Padd1 = (SHORT)(Rnd_s32() % 3 + 8);
}

/* ------------------------------------------------------------------ */
/* Esp_1c - Ground / water / fall checks, then dispatch (DS 8001fbd0)   */
/* ------------------------------------------------------------------ */
/* DS 0x8001fbd0 (Esp_1c) */
void Esp_1c(void)
{
	VECTOR tvec;
	LONG ground;
	UCHAR idx;
	ESP_WORK *pEsp;

	ground = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (ground != 0 && ground < pESP->Pos_y) {
		pEsp = pESP;
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
	}
	pEsp = pESP;
	if (pEsp->Speed_x < 0) {
		pEsp->Add_x = 0;
		pEsp->Speed_x = 0;
	}
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y - 900;
	tvec.vz = pESP->Pos_z;
	ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
	tvec.vy += 900;
	if (ground < tvec.vy) {
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		if (Ground_in_flg != 0) {
			idx = (ground != ESP_GROUND_Y(pESP)) ? pESP->Free0 : pESP->Free1;
			ESP_TBLJ_CALL(idx);
		}
		ESP_GROUND_Y(pESP) = ground;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_1d - Emit a trail call every 15 frames while moving; ground test */
/*          (DS 8001fd5c)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001fd5c (Esp_1d) */
void Esp_1d(void)
{
	VECTOR tvec;
	UCHAR bRet;

	if (pESP->Speed_x < 1) {
		ESP_WORK *pW = pESP;

		pW->Add_x = 0;
		pW->Speed_x = 0;
	} else if (pESP->Speed_x > 0x3c && pESP->Free0 % 0xf == 0) {
		/* Signed int division as in retail (mult with 0x66666667, sra 31 and subu); Mag is a USHORT, so the operand is widened to LONG. */
		bRet = Esp_call3((((LONG)pESP->Mag << 2) / 5) | 0x5040000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		if (bRet < 0xff) {
			Esp[bRet].Padd2 = 1;
		}
	}
	pESP->Free0++;
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y - 100;
	tvec.vz = pESP->Pos_z;
	Sca_get_ground(&tvec, 2, 0x2000, 0);
	if (Ground_in_flg != 0) {
		ESP_TBLJ_CALL(pESP->Free1);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_1e - Step Esp_02, then pick a random life (DS 8001fecc)          */
/* ------------------------------------------------------------------ */
/* DS 0x8001fecc (Esp_1e) */
void Esp_1e(void)
{
	Esp_02();
	if (pESP->Type == 4) {
		pESP->Padd0 = 2;
		pESP->Padd1 = (SHORT)(Rnd_s32() % 8 + 2);
	} else {
		pESP->Padd0 = 3;
		pESP->Padd1 = (SHORT)((Rnd_s32() % 6) * 50 + 700);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_1f - Countdown Padd1, then emit hit calls and move to Routine1 0x35 */
/*          (DS 8001ff98)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8001ff98 (Esp_1f) */
void Esp_1f(void)
{
	UCHAR bRet;

	if (pESP->Padd1 >= 4) {
		if ((signed char)pESP->Padd0 != 0) {
			if ((signed char)pESP->Padd0 < 3) {
				bRet = Esp_call3(pESP->Mag | 0xa000000, pESP->DirY, pESP->pMat, (SVECTOR *)&pESP->Hot_x);
				if (bRet < 0xff) {
					Esp[bRet].Padd0 = pESP->Padd0 + 1;
				}
			}
		} else {
			bRet = Esp_call3(pESP->Mag | 0xa000000, pESP->DirY, pESP->pMat, (SVECTOR *)&pESP->Hot_x);
			if (bRet < 0xff) {
				Esp[bRet].Padd0 = 1;
				Esp[bRet].Padd1 = 1;
			}
		}
		Esp_10();
		pESP->Routine1 = 0x35;
	} else {
		pESP->Padd1++;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_20 - Player proximity: damage/poison/vibration, or drift          */
/*          (DS 800200b0)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800200b0 (Esp_20) */
void Esp_20(void)
{
	VECTOR tvec;
	ENEMY_WORK *pl;
	USHORT tx;
	USHORT ty;
	LONG iy;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 800, 0x14);
	if (pl != 0) {
		if (pl->Id < 0x10) {
			if ((pESP->Type & 8) != 0) {
				G.Player.Damage_cnt |= 0x80;
				G.Pl_poison_timer = 0x1518;
				G.Pl_poison_down = 0x1e;
			}
			Pad_vib_set0(8, 0);
			Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
		} else if ((pESP->Type & 8) != 0) {
			G.SaveData.Dummy1[1] |= 1;
		}
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		if ((signed char)pESP->Padd0 < 0x14) {
			tx = pESP->Transx;
			pESP->Transx = tx + (LONG)tx * 0x26 / 1000;
			pESP->Padd0++;
		} else {
			pESP->Transx = pESP->Transx;
		}
		ty = pESP->Transy;
		iy = ty;
		pESP->Transy = ty + iy / 200;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_21 - Countdown Free1 (random re-arm at 0); Anm_cnt 0x12 -> 0x0b  */
/*          (DS 80020298)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80020298 (Esp_21) */
void Esp_21(void)
{
	SSEQUENCE *pAnm;
	ESP_WORK *pEsp;

	if (pESP->Free1 != 0) {
		pESP->Free1--;
		pEsp = pESP;
		if (pEsp->Anm_cnt == 0x12) {
			pEsp->Anm_cnt = 0x0b;
			pEsp->Be_flg = 0x8000;
			pAnm = pESP->pAnmseq + pESP->Anm_cnt;
			pESP->Anm_time = pAnm->Time;
		}
	} else {
		pESP->Be_flg = 0xb803;
		pESP->Free1 = (UCHAR)(Rnd_s32() % 30 + 60);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_22 - Wait for Free3 to arm, then launch random velocity (DS 80020364) */
/* ------------------------------------------------------------------ */
/* DS 0x80020364 (Esp_22) */
void Esp_22(void)
{
	LONG spd;
	LONG padd1;
	if (pESP->Free3 != 0) {
		if (pESP->Speed_y < pESP->Padd2) {
			pESP->Speed_x = (SHORT)((signed char)pESP->Padd1 * (Rnd_s32() % 70 + 50));
			pESP->Speed_z = (SHORT)((signed char)pESP->Padd1 * (Rnd_s32() % 50));
			spd = Rnd_s32() % 20 + 40;
			pESP->Free3 = 0;
			pESP->Speed_y = (SHORT)spd;
			padd1 = (signed char)pESP->Padd1;	/* DS reads Padd1 with lb, negates it and stores the sign-extended byte with sh */
			pESP->Padd1 = (signed char)(-padd1);
			pESP->Padd2 = (SHORT)(((SHORT)spd * -3) / 10);
		}
	} else if (pESP->Speed_y < 1) {
		pESP->Free3 = 1;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_23 - Kill: clear routine and flag (DS 800204f8)                  */
/* ------------------------------------------------------------------ */
/* DS 0x800204f8 (Esp_23) */
void Esp_23(void)
{
	pESP->Routine0 = 0;
	pESP->Be_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_24 - Randomize speed XZ, restore position from Old_*, set Padd2 */
/*          (DS 80020510)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80020510 (Esp_24) */
void Esp_24(void)
{
	LONG spd;
	LONG padd1;
	pESP->Speed_x = (SHORT)((signed char)pESP->Padd1 * (Rnd_s32() % 70 + 50));
	pESP->Speed_z = (SHORT)((signed char)pESP->Padd1 * (Rnd_s32() % 50));
	spd = Rnd_s32() % 20 + 40;
	pESP->Speed_y = (SHORT)spd;
	padd1 = (signed char)pESP->Padd1;	/* DS reads Padd1 with lb, negates it and stores the sign-extended byte with sh */
	pESP->Padd1 = (signed char)(-padd1);
	*(ULONG *)&pESP->Pos_x = *(ULONG *)&pESP->Old_x;
	pESP->Pos_z = pESP->Old_z;
	pESP->Padd2 = (SHORT)(((SHORT)spd * -3) / 10);
}

/* ------------------------------------------------------------------ */
/* Esp_25 - Reset sequence to index 6; reload animation time; mask flg  */
/*          (DS 80020660)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80020660 (Esp_25) */
void Esp_25(void)
{
	SSEQUENCE *pAnm;		/* register pointer local, as in the beta record (v1 in DS) */

	*(ULONG *)&pESP->Routine0 = 0;
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Add_y = 0;
		pEsp->Add_x = 0;
	}
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Anm_cnt = 6;
		pEsp->Speed_y = 0;
		pEsp->Speed_x = 0;
	}
	/* DS forms the entry address as pAnmseq + Anm_cnt * 8 in v1, then
	 * reads Time from offset 2 of that entry. */
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	pESP->Be_flg &= 0xa001;
}

/* ------------------------------------------------------------------ */
/* Esp_26 - Clear Add_x, Speed_x and Free1 (DS 800206e0)                */
/* ------------------------------------------------------------------ */
/* DS 0x800206e0 (Esp_26) */
void Esp_26(void)
{
	pESP->Add_x = 0;
	pESP->Speed_x = 0;
	pESP->Free1 = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_27 - Spawn sub-effect with identity matrix (DS 800206fc)         */
/* ------------------------------------------------------------------ */
/* DS 0x800206fc (Esp_27) */
void Esp_27(void)
{
	pESP->Routine0 = 0x54;
	pESP->Routine1 = 0;
	Esp_call3((ULONG)pESP->Mag | 0x14000000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
}

/* ------------------------------------------------------------------ */
/* Esp_28 - Attack check at Pos_y - 100; call Esp_32 on hit (DS 80020758) */
/* ------------------------------------------------------------------ */
/* DS 0x80010910 (Esp_aw_28) */
const ATTACK_WORK Esp_aw_28 = { -600, 0, 300, 150 };

/* DS 0x80020758 (Esp_28) */
void Esp_28(void)
{
	VECTOR tvec;
	ATTACK_WORK Aw;

	Aw = Esp_aw_28;
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y - 100;
	tvec.vz = pESP->Pos_z;
	if (Pos_em_at_ck(&tvec, pESP->DirY, &Aw, 0x2002000a) != 0) {
		Esp_32();
	}
}

/* ------------------------------------------------------------------ */
/* Esp_29 - Spawn three sub-effects at random angles (DS 800207e4)      */
/* ------------------------------------------------------------------ */
/* DS 0x800207e4 (Esp_29) */
void Esp_29(void)
{
	/* DS shifts the Rnd() return register straight into the halfword angle (sll v0,v0,20 /
	 * sra a1,v0,16) with no andi 0xff after each jal, the same form as Esp_2a, so each call
	 * goes through Rnd_s32() (Rnd seen as LONG-returning). Rnd masks its own result to 0..255
	 * (andi v0,v0,0xff before its jr ra), so the angle is still 0..4080 in steps of 16. The
	 * mockup declares no locals here, so the calls are written inline. */
	Esp_call3(((ULONG)pESP->Free0 << 16) | 0x9000800, (SHORT)(Rnd_s32() << 4), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	Esp_call3((((ULONG)pESP->Free0 + 1) << 16) | 0x9001000, (SHORT)(Rnd_s32() << 4), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	Esp_call3((((ULONG)pESP->Free0 + 2) << 16) | 0x9000c00, (SHORT)(Rnd_s32() << 4), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	pESP->Routine0 = 0x2a;
}

/* ------------------------------------------------------------------ */
/* Esp_2a - Spawn two more sub-effects (DS 800208b8)                    */
/* ------------------------------------------------------------------ */
/* DS 0x800208b8 (Esp_2a) */
void Esp_2a(void)
{
	/* DS shifts the Rnd() return register straight into the halfword angle (sll v0,v0,20 /
	 * sra a1,v0,16) with no andi 0xff after the jal, so each call goes through Rnd_s32()
	 * (Rnd seen as LONG-returning). Rnd masks its own result to 0..255 (andi v0,v0,0xff before
	 * its jr ra), so the angle is still 0..4080 in steps of 16. The mockup declares no locals
	 * here, so the calls are written inline. */
	Esp_call3((((ULONG)pESP->Free0 + 3) << 16) | 0x9000c00, (SHORT)(Rnd_s32() << 4), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	Esp_call3((((ULONG)pESP->Free0 + 4) << 16) | 0x9000a00, (SHORT)(Rnd_s32() << 4), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	pESP->Routine0 = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_2b - Reset to sequence 0x1e, Pos_y from ground; mask flg (DS 80020950) */
/* ------------------------------------------------------------------ */
/* DS 0x80020950 (Esp_2b) */
void Esp_2b(void)
{
	/* The mockup declares one local, SSEQUENCE *pAnm, for the current animation step. Pos_y is
	 * written before Anm_cnt here: DS loads pESP once (lw v1) for the Anm_cnt store and the
	 * Free4 read, stores Anm_cnt (sb v0,0x21(v1)) ahead of Pos_y (sh v0,0x36(v1)) after
	 * scheduling, and reloads pESP (lw a0) only for the pAnm line. The compiler gives that shape from
	 * this statement order; the two stores go to different fields, so the order does not change
	 * what is stored. Pos_y takes the low half of the 4-byte ground height word at Free4
	 * (lw 0x14, sh 0x36). */
	SSEQUENCE *pAnm;

	pESP->Routine0 = 0;
	pESP->Routine1 = 0;
	pESP->Pos_y = (SHORT)ESP_GROUND_Y(pESP);
	pESP->Anm_cnt = 0x1e;
	pAnm = &pESP->pAnmseq[pESP->Anm_cnt];
	pESP->Anm_time = pAnm->Time;
	pESP->Be_flg &= 0xfffd;
}

/* ------------------------------------------------------------------ */
/* Esp_2c - Bounce: scale speeds, random Speed_z, play sound (DS 800209cc) */
/* ------------------------------------------------------------------ */
/* DS 0x800209cc (Esp_2c) */
void Esp_2c(void)
{
	VECTOR vec;
	ULONG code;

	pESP->Free0 = 0x23;
	pESP->Speed_x = (SHORT)((pESP->Speed_x * 0x3c) / 100);
	pESP->Speed_y = (SHORT)((pESP->Speed_y * -0x28) / 100);
	pESP->Speed_z = (SHORT)((Rnd_s32() % 4) * 3);
	code = ((ULONG)pESP->Free3 << 16) | 1;
	vec.vx = pESP->Pos_x;
	vec.vy = pESP->Pos_y;
	vec.vz = pESP->Pos_z;
	Snd_se_stad(code, &vec);
}

/* ------------------------------------------------------------------ */
/* Esp_2d - Bounce (mirror): negate Speed_x, random Speed_z, play sound */
/*          (DS 80020abc)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80020abc (Esp_2d) */
void Esp_2d(void)
{
	VECTOR vec;
	ULONG code;
	LONG spd;

	pESP->Free1 = 0x23;
	spd = pESP->Speed_x;	/* DS reads Speed_x with lh before negating it */
	pESP->Speed_x = -spd;
	pESP->Speed_z = (SHORT)((Rnd_s32() % 4) * 3);
	code = ((ULONG)pESP->Free3 << 16) | 1;
	vec.vx = pESP->Pos_x;
	vec.vy = pESP->Pos_y;
	vec.vz = pESP->Pos_z;
	Snd_se_stad(code, &vec);
}

/* ------------------------------------------------------------------ */
/* Esp_2e - Reset state, random Add_x / Padd1, clear Free3 bit 0        */
/*          (DS 80020b60)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80020b60 (Esp_2e) */
void Esp_2e(void)
{
	ESP_WORK *pEsp;

	pEsp = pESP;
	pEsp->Add_y = 0;
	pEsp->Free0 = 0;
	pEsp->Speed_y = 0;
	pESP->Routine0 = 0x13;
	pESP->Routine1 = 0x1d;
	pESP->Speed_x = 0xb4;
	pESP->Add_x = (CHAR)(-10 - (Rnd_s32() % 11));
	pESP->Padd1 = (SHORT)((Rnd_s32() % 8) + 0x26);
	pESP->Free3 &= 0xfffe;
}

/* ------------------------------------------------------------------ */
/* Esp_2f - Multi-step explosion: sound, attack checks, sub-effects,    */
/*          steps Free3 1..4 then ends (DS 80020c3c)                    */
/* ------------------------------------------------------------------ */
typedef struct { ATTACK_WORK Tbl[2]; } ATTACK_WORK_PAIR;   /* a two-element ATTACK_WORK table as one 16-byte block, copied whole as in DS */

/* DS 0x80010918 - ATTACK_WORK[2] { Off_x, Off_z, R_x, Half_R_z }; Esp_2f copies it to a stack array that Pos_em_at_ck updates */
const ATTACK_WORK Esp_aw_2f[2] = {
	{ -2000, 0, 1000, 500 },
	{ -1000, 0, 500, 250 },
};

/* DS 0x80020c3c (Esp_2f) */
void Esp_2f(void)
{
	VECTOR tvec;
	ULONG type_off;
	ULONG call_base;
	ATTACK_WORK_PAIR Aw;	/* DS copies the whole 16-byte table to the stack at entry */
	MATRIX *id_mtx = (MATRIX *)&GsIDMATRIX;	/* held in a saved register across the calls */

	Aw = *(ATTACK_WORK_PAIR *)Esp_aw_2f;
	switch (pESP->Free3) {
	case 0:
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		pESP->Be_flg = 0x8400;
		pESP->Routine0 = 0;
		pESP->Routine1 = 0x2f;
		if (pESP->Type == 0x0c) {
			Snd_se_stad(0x1110001, &tvec);
			Pos_em_at_ck(&tvec, pESP->DirY, &Aw.Tbl[pESP->Type - 12], 0x10020009);
			tvec.vy += 900;
			Pos_em_at_ck(&tvec, pESP->DirY, &Aw.Tbl[pESP->Type - 12], 0x10020009);
		} else {
			Snd_se_stad(0x1140001, &tvec);
		}
		type_off = (ULONG)((LONG)(pESP->Type - 0x0c) * 0xa00) & 0xffff;
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -0x1d6;
		}
		call_base = type_off + 0x1700;
		Esp_call3(call_base | 0x31f0000, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
		Esp_call3((type_off + 0x1f00) | 0x40c0000, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
		Esp_call3(call_base | 0x41d0000, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
		pESP->Free3 = 1;
		break;
	case 1:
		pESP->Free3 = 2;
		break;
	case 2:
		pESP->Free3 = 3;
		break;
	case 3:
		pESP->Free3 = 4;
		break;
	case 4:
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -600;
		}
		Esp_call3(0x4152700, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_30 - Large explosion: sparks, spawn calls, Pos_em checks, restore  */
/*          of the saved t[] vector (DS 80020f3c)                       */
/* ------------------------------------------------------------------ */

/* DS 0x8001093c - ATTACK_WORK[2] { Off_x, Off_z, R_x, Half_R_z } */
const ATTACK_WORK Esp_aw_30[2] = {
	{ -1200, 0, 600, 300 },
	{ -600, 0, 300, 150 },
};

/* DS 0x80020f3c (Esp_30) */
void Esp_30(void)
{
	LONG saved[3];
	ATTACK_WORK_PAIR Aw;   /* DS copies the whole 16-byte table to the stack at entry; Pos_em_at_ck takes element 0 or 1 */
	ULONG Arg0;   /* first spawn argument, formed before the second Rnd call as in DS */

	Aw = *(ATTACK_WORK_PAIR *)Esp_aw_30;
	switch (pESP->Free3) {
	case 0:
		saved[0] = pESP->t[0];
		saved[1] = pESP->t[1];
		saved[2] = pESP->t[2];
		pESP->Be_flg = 0x8000;
		pESP->Routine0 = 0;
		pESP->t[0] = pESP->Pos_x;
		pESP->t[1] = pESP->Pos_y;
		pESP->t[2] = pESP->Pos_z;
		pESP->Free3 = 1;
		pESP->Routine1 = 0x30;
		Snd_se_stad(0x1120001, (VECTOR *)pESP->t);
		if ((pESP->Be_flg & 0x80) == 0) {
			Pos_em_at_ck((VECTOR *)pESP->t, pESP->DirY, &Aw.Tbl[0], 0x2000a);
			saved[1] += 1800;
			Pos_em_at_ck((VECTOR *)pESP->t, pESP->DirY, &Aw.Tbl[0], 0x2000a);
			saved[1] += 1800;
			Esp_call3(0x40c2800, 0, (MATRIX *)pESP->m, NULL);
			Esp_call3(0x41d2700, 0, (MATRIX *)pESP->m, NULL);

			{
				UCHAR ret;

				Arg0 = (((ULONG)(Rnd_s32() % 8)) * 0x300 + 0x1c00) | 0x5050000;
				ret = Esp_call3(Arg0, (SHORT)(pESP->DirY + Rnd_s32() % 40), (MATRIX *)pESP->m, NULL);
				if (ret < 0xff) {
					Esp[ret].Speed_x += (SHORT)(Rnd_s32() % 25);
					Esp[ret].Padd2 = 1;
					Esp[ret].Add_y += (CHAR)(Rnd_s32() % 8);
				}
			}
			{
				UCHAR ret;

				Arg0 = (((ULONG)(Rnd_s32() % 8)) * 0x300 + 0x1c00) | 0x5050000;
				ret = Esp_call3(Arg0, (SHORT)(pESP->DirY + Rnd_s32() % 80 + 400), (MATRIX *)pESP->m, NULL);
				if (ret < 0xff) {
					Esp[ret].Speed_x += (SHORT)(Rnd_s32() % 25);
					Esp[ret].Padd2 = 1;
					Esp[ret].Add_y += (CHAR)(Rnd_s32() % 8);
				}
			}
			{
				UCHAR ret;

				Arg0 = (((ULONG)(Rnd_s32() % 8)) * 0x300 + 0x1c00) | 0x5050000;
				ret = Esp_call3(Arg0, (SHORT)(pESP->DirY + Rnd_s32() % 80 - 400), (MATRIX *)pESP->m, NULL);
				if (ret < 0xff) {
					Esp[ret].Speed_x += (SHORT)(Rnd_s32() % 25);
					Esp[ret].Padd2 = 1;
					Esp[ret].Add_y += (CHAR)(Rnd_s32() % 8);
				}
			}
		} else {
			Pos_em_at_ck((VECTOR *)pESP->t, pESP->DirY, &Aw.Tbl[1], 0x2000a);
			saved[1] -= 1800;
			Pos_em_at_ck((VECTOR *)pESP->t, pESP->DirY, &Aw.Tbl[1], 0x2000a);
			saved[1] += 1800;
			Esp_call3(0x40c2800, 0, (MATRIX *)pESP->m, NULL);
			Esp_call3(0x41d2700, 0, (MATRIX *)pESP->m, NULL);
		}
		pESP->t[0] = saved[0];
		pESP->t[1] = saved[1];
		pESP->t[2] = saved[2];
		break;
	case 1:
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_31 - Staged burst: sounds, two attack checks, timed sub-effects  */
/*          (DS 800215c8)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800215c8 (Esp_31) */
void Esp_31(void)
{
	VECTOR tvec;
	ATTACK_WORK_PAIR Aw;	/* DS copies the whole 16-byte table to the stack at entry, as in Esp_30 */
	ATTACK_WORK *pAw;
	ULONG bit;

	Aw = *(ATTACK_WORK_PAIR *)Esp_aw_30;
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	switch (pESP->Free3) {
	case 0:
		pESP->Free3 = 1;
		pESP->Be_flg = 0x8403;
		pESP->Routine0 = 0;
		pESP->Routine1 = 0x31;
		Snd_se_stad(0x1130001, &tvec);
		pAw = ((pESP->Be_flg & 0x80) == 0) ? &Aw.Tbl[0] : &Aw.Tbl[1];
		Pos_em_at_ck(&tvec, pESP->DirY, pAw, 0x1002000b);
		tvec.vy += 0x708;
		Pos_em_at_ck(&tvec, pESP->DirY, pAw, 0x1002000b);
		if (pESP->Free2 == 0xff) {
			pESP->Speed_y = 0xf0;
			pESP->Pos_y += -0x1d6;
			pESP->Add_x = 0;
			pESP->Speed_x = 0;
		} else {
			pESP->Add_x = (CHAR)0xe9;
			pESP->Speed_x = 0;
			pESP->Speed_y = 0xf0;
		}
		pESP->Add_y = 0;
		Esp_call3(0x30f2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		Esp_call3(0x40c2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		bit = 0x41d1800;
		break;
	case 1:
		pESP->Free3 = 2;
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -600;
		}
		Esp_call3(0x31f2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -0x1d6;
		}
		return;
	case 2:
		pESP->Free3 = 3;
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -600;
		}
		bit = 0x3142000;
		break;
	case 3:
		pESP->Free3 = 4;
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -600;
		}
		bit = 0x40d2800;
		break;
	case 4:
		if (pESP->Free2 == 0xff) {
			pESP->Pos_y += -600;
		}
		Esp_call3(0x30f2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
		return;
	default:
		return;
	}
	Esp_call3(bit, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
}

/* ------------------------------------------------------------------ */
/* Esp_32 - Hit reaction: Free0 = 0x40, clear Add_x / Free1 / Speed_x,   */
/*          clear Free3 bit 1 (DS 80021970)                             */
/* ------------------------------------------------------------------ */
/* DS 0x80021970 (Esp_32) */
void Esp_32(void)
{
	ESP_WORK *pEsp;

	pESP->Free0 = 0x40;
	pEsp = pESP;
	pEsp->Add_x = 0;
	pEsp->Free1 = 0;
	pEsp->Speed_x = 0;
	pESP->Free3 &= 0xfffd;
}

/* ------------------------------------------------------------------ */
/* Esp_33 - Sound and one spawn call, then reset motion and routine    */
/*          (DS 800219b4)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800219b4 (Esp_33) */
void Esp_33(void)
{
	VECTOR tvec;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	Snd_se_stad(0x3060001, &tvec);
	Esp_call3(0x30f1400, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Add_y = 0;
		pEsp->Add_x = 0;
	}
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Speed_y = 0;
		pEsp->Speed_x = 0;
		/* Routine0, Routine1, Free0 and Free1 are one word at +0x00 */
		*(ULONG *)&pEsp->Routine0 = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_34 - Sound, anim 0x0d with Anm_time 2, spawn call, then idle    */
/*          (DS 80021a48)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80021a48 (Esp_34) */
void Esp_34(void)
{
	VECTOR tvec;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	Snd_se_stad(0x3040001, &tvec);
	pESP->Anm_cnt = 0x0d;
	pESP->Anm_time = 2;
	Esp_call3(0x30f2800, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
	/* DS reads pESP once for the two Add stores (sb, sb) and once more for the Speed, Be_flg
	   and Routine stores (sh, sh, sh, sw), so each group goes through its own local pointer,
	   as in Esp_33. */
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Add_y = 0;
		pEsp->Add_x = 0;
	}
	{
		ESP_WORK *pEsp = pESP;

		pEsp->Speed_y = 0;
		pEsp->Speed_x = 0;
		pEsp->Be_flg = 0xb001;
		/* Routine0, Routine1, Free0 and Free1 are one word at +0x00 */
		*(ULONG *)&pEsp->Routine0 = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_35 - Ground / water / player-contact check; vibrate on player    */
/*          contact, else dispatch by ground state (DS 80021b00)        */
/* ------------------------------------------------------------------ */
/* DS 0x80021b00 (Esp_35) */
void Esp_35(void)
{
	VECTOR tvec;
	LONG ground;
	ENEMY_WORK *pl;
	ULONG code;
	UCHAR idx;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	ground = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (ground != 0 && ground < pESP->Pos_y) {
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
		pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 500, 5);
		if (pl != 0) {
			code = 0x3060000;
			if (pl->Be_flg & 0x2000) {
				code = 0x3160000;
			}
			Snd_se_stad(code | 1, &tvec);
			if (pl->Id <= 0xf) {
				Pad_vib_set0(8, 0);
				Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
			}
		} else if (tvec.vy > ground) {
			ESP_TBLJ_CALL(pESP->Free0);
		} else {
			if (Ground_in_flg != 0) {
				idx = (ground != ESP_GROUND_Y(pESP)) ? pESP->Free0 : pESP->Free1;
				ESP_TBLJ_CALL(idx);
			}
			ESP_GROUND_Y(pESP) = ground;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_36 - Snap to ground, then Routine1 0x38 and Cdir_y from Free3    */
/*          (DS 80021ce4)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80021ce4 (Esp_36) */
void Esp_36(void)
{
	Esp_10();
	pESP->Routine1 = 0x38;
	pESP->Cdir_y = (SHORT)pESP->Free3;
}

/* ------------------------------------------------------------------ */
/* Esp_37 - Fade Transx / grow Transy; first 20 steps count Padd0       */
/*          (DS 80021d24)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80021d24 (Esp_37) */
void Esp_37(void)
{
	if ((signed char)pESP->Padd0 < 0x14) {
		pESP->Padd0++;
		pESP->Transx = (USHORT)(pESP->Transx - pESP->Transx / 0x14);
		pESP->Transy = (USHORT)(pESP->Transy + pESP->Transy * 0x5a / 1000);
	} else {
		pESP->Transx = (USHORT)(pESP->Transx - pESP->Transx / 200);
		pESP->Transy = (USHORT)(pESP->Transy + pESP->Transy / 100);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_38 - Player contact (vibrate / poison), else ground probe and    */
/*          dispatch (DS 80021e14)                                      */
/* ------------------------------------------------------------------ */
/* DS 0x80021e14 (Esp_38) */
void Esp_38(void)
{
	VECTOR tvec;
	LONG ground;
	ENEMY_WORK *pl;
	UCHAR idx;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 600, 0x14);
	if (pl != 0) {
		if (pl->Id < 0x10) {
			if ((pESP->Type & 8) != 0) {
				G.Player.Damage_cnt |= 0x80;
				G.Pl_poison_timer = 0x1518;
				G.Pl_poison_down = 0x1e;
			}
			Pad_vib_set0(8, 0);
			Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
		} else if ((pESP->Type & 8) != 0) {
			G.SaveData.Dummy1[1] |= 1;
		}
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		/* ground holds the water height first, then the ground probe result */
		ground = Water_ck(pESP->Pos_x, pESP->Pos_z);
		if (ground != 0 && ground < pESP->Pos_y) {
			Esp_call3(0x30f2800, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
			ESP_TBLJ_CALL(pESP->Free0);
		} else {
			tvec.vx = pESP->Pos_x;
			tvec.vy = pESP->Pos_y + 700;
			tvec.vz = pESP->Pos_z;
			ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
			if (tvec.vy > ground) {
				Esp_call3(0x30f2800, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
				ESP_TBLJ_CALL(pESP->Free0);
			} else {
				if (Ground_in_flg != 0) {
					idx = (ground != ESP_GROUND_Y(pESP)) ? pESP->Free0 : pESP->Free1;
					ESP_TBLJ_CALL(idx);
				}
				ESP_GROUND_Y(pESP) = ground;
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_39 - Water / ground clamp; while Transy is low, boost and call   */
/*          Esp_34, else decay (DS 80022094)                            */
/* ------------------------------------------------------------------ */
/* DS 0x80022094 (Esp_39) */
void Esp_39(void)
{
	VECTOR tvec;
	LONG water;
	LONG ground;
	USHORT mag;
	LONG trans_x;		/* Transx widened to int for the signed "/ 10" below */

	water = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (water != 0 && water < pESP->Pos_y) {
		pESP->Add_y = 0;
		pESP->Speed_y = 0;
		pESP->Pos_y = (SHORT)water;
	}
	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
	if (ground < tvec.vy) {
		pESP->Add_y = 0;
		pESP->Speed_y = 0;
	}
	if (pESP->Transy > 0x1000) {
		pESP->Routine0 = 0;
		pESP->Routine1 = 0x39;
		/* DS divides Transx as a signed int: mult by 0x66666667, sra 2.
		   The value is a zero-extended halfword, so the compiler drops the
		   sign correction. A USHORT operand would make the C front end use
		   an unsigned division (multu 0xcccccccd), which DS does not do. */
		trans_x = pESP->Transx;
		pESP->Transx = (USHORT)(pESP->Transx + (trans_x & 0xffff) / 10);
		/* Signed int division: mult by 0x10624dd3 with the sra 31 sign word. */
		pESP->Transy = (USHORT)(pESP->Transy - (pESP->Transy * 300) / 1000);
	} else {
		mag = pESP->Mag;
		pESP->Transx = 0x1100;
		pESP->Transy = 0xa00;
		/* Signed int division: DS multiplies by 0x51eb851f with mult and
		   subtracts the sign word (sra 31) of the product. */
		pESP->Mag = (USHORT)(mag + (mag * 0x1e) / 100);
		Esp_34();
	}
}

/* ------------------------------------------------------------------ */
/* Esp_3a - Two-phase fade of Transx / Transy, Padd1 frame counter      */
/*          (DS 80022254)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022254 (Esp_3a) */
void Esp_3a(void)
{
	LONG r;
	ESP_WORK *pEsp;

	pEsp = pESP;
	switch ((signed char)pESP->Padd0) {
	case 0:
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		break;
	case 1:
		if (pESP->Padd1 != 0) {
			pESP->Padd1--;
			pESP->Transx = (USHORT)(((LONG)pESP->Transx * 0x370) / 1000);
			pESP->Transy = (USHORT)(((LONG)pESP->Transy * 800) / 1000);
		} else {
			pESP->Padd0 = 0;
		}
		break;
	case 2:
		if (pESP->Padd1 != 0) {
			pESP->Padd1--;
			pESP->Transx = (USHORT)(((LONG)pESP->Transx * 0x3f2) / 1000);
			pESP->Transy = (USHORT)(((LONG)pESP->Transy * 0x3ef) / 1000);
		} else {
			pESP->Padd0 = 1;
			r = Rnd_s32();
			pESP->Padd1 = (SHORT)((r % 2) + 2);
		}
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_3b - Esp_02 step, Padd0 = 2, random Padd1 in 10..13 (DS 800223f8) */
/* ------------------------------------------------------------------ */
/* DS 0x800223f8 (Esp_3b) */
void Esp_3b(void)
{
	LONG r;

	Esp_02();
	pESP->Padd0 = 2;
	/* DS takes Rnd's v0 as a signed int: no andi 0xff after the jal, and r % 4 is the
	   signed form (bgez, addiu 3, sra 2, sll 2, subu) at DS 0x80022428..0x8002243c.
	   The call therefore goes through Rnd_s32(). */
	r = Rnd_s32();
	pESP->Padd1 = (SHORT)((r % 4) + 10);
}

/* ------------------------------------------------------------------ */
/* Esp_3c - Mag += 0x87 (DS 80022454)                                   */
/* ------------------------------------------------------------------ */
/* DS 0x80022454 (Esp_3c) */
void Esp_3c(void)
{
	pESP->Mag = (USHORT)(pESP->Mag + 0x87);
}

/* ------------------------------------------------------------------ */
/* Esp_3d - After frame 5, sound + spawn + vibrate when player is near  */
/*          (DS 80022474)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022474 (Esp_3d) */
void Esp_3d(void)
{
	VECTOR tvec;
	ATTACK_WORK Aw;	/* DS copies Esp_aw_18 to the stack at entry; not read in this routine */
	ENEMY_WORK *pl;

	Aw = Esp_aw_18;
	if (pESP->Anm_cnt > 5) {
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 500, 5);
		if (pl != 0) {
			Snd_se_stad(0x31d0001, &tvec);
			Esp_call3(0x30f2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
			if (pl->Id < 0x10) {
				Pad_vib_set0(8, 0);
				Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_3e - Fan of 19 spawn calls along Hot_* with random Cdir_y/z      */
/*          (DS 8002256c)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8002256c (Esp_3e) */
void Esp_3e(void)
{
	SVECTOR svec[3];	/* only [0] is used; the other two entries are unused */
	LONG count;
	UCHAR ret;
	ESP_WORK *pNew;
	ESP_WORK *self;

	Game.Plight_flg = 1;
	svec[0].vx = pESP->Hot_x;
	svec[0].vy = pESP->Hot_y;
	svec[0].vz = pESP->Hot_z;
	count = 0x13;
	do {
		count--;
		ret = Esp_call3(0x6042000, pESP->DirY, (MATRIX *)pESP->m, &svec[0]);
		if (ret < 0xff) {
			/* DS computes &Esp[ret] once (s1) before the Rnd calls, and Rnd goes through
			 * Rnd_s32() (no andi 0xff after the jal). */
			pNew = &Esp[ret];
			pNew->Cdir_y = (SHORT)((Rnd_s32() % 50) * 40 - 1000);
			pNew->Cdir_z = (SHORT)((Rnd_s32() & 2) != 0 ? 0x400 : 0xc00);
		} else {
			count = 0;
		}
		svec[0].vy += 0x1b0;
	} while (count != 0);
	/* DS loads pESP once for the three stores. */
	self = pESP;
	self->Routine1 = 0;
	self->Routine0 = 0;
	self->Be_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_3f - Ground probe (0x80 flag); clear Be_flg bit 13 when grounded  */
/*          (DS 800226ac)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800226ac (Esp_3f) */
void Esp_3f(void)
{
	VECTOR tvec;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	Sca_get_ground(&tvec, 2, 0x80, 1);
	if (Ground_in_flg != 0) {
		pESP->Be_flg &= 0xdfff;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_40 - Stop vertical motion, snap Pos_y to ground, clear Free3 bit 0 */
/*          (DS 80022728)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022728 (Esp_40) */
void Esp_40(void)
{
	ESP_WORK *p = pESP;

	/* DS loads pESP once for the first four stores (Add_y, Free0,
	 * Speed_y, Pos_y) and loads it again from memory for Routine1 and
	 * again for Free3, so the first group goes through a local pointer
	 * and the last two statements use pESP directly. */
	p->Add_y = 0;
	p->Free0 = 0;
	p->Speed_y = 0;
	p->Pos_y = (SHORT)ESP_GROUND_Y(p);
	pESP->Routine1 = 0;
	pESP->Free3 &= 0xfffe;
}

/* ------------------------------------------------------------------ */
/* Esp_41 - Anim countdown in Free1; on expiry set flags and random fade */
/*          (DS 80022774)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022774 (Esp_41) */
void Esp_41(void)
{
	SHORT v;
	SSEQUENCE *pAnm;

	if (pESP->Free1 != 0) {
		pESP->Free1--;
		if (pESP->Free1 == 0) {
			pESP->Be_flg |= 0x2003;
			v = (SHORT)(pESP->Mag - (Rnd_s32() % 10) * 10);
			pESP->Transy = (USHORT)v;
			pESP->Transx = (USHORT)v;
		}
	} else if (pESP->Anm_cnt == pESP->Free0) {
		pESP->Anm_cnt = pESP->Free2;
		pAnm = pESP->pAnmseq + pESP->Anm_cnt;
		pESP->Anm_time = pAnm->Time;
		pESP->Be_flg &= 0xdffc;
		pESP->Free1 = (UCHAR)(Rnd_s32() % 50 + 50);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_42 - Dispatch Free0 while far below the player; else grow Transx  */
/*          and Transy (DS 800228d8)                                    */
/* ------------------------------------------------------------------ */
/* DS 0x800228d8 (Esp_42) */
void Esp_42(void)
{
	USHORT tx;
	USHORT ty;
	LONG iy;

	if ((LONG)G.Player.Pos_y + G.Player.Atd[0].Ofs_y - 1000 < (LONG)pESP->Pos_y) {
		ESP_TBLJ_CALL(pESP->Free0);
	} else {
		if ((signed char)pESP->Padd0 < 0x14) {
			tx = pESP->Transx;
			pESP->Transx = tx + (LONG)tx * 0x26 / 1000;
			pESP->Padd0++;
		} else {
			pESP->Transx = pESP->Transx;
		}
		ty = pESP->Transy;
		iy = ty;
		pESP->Transy = ty + iy / 200;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_43 - Below water line: reset routine and spawn one call (DS 800229e4) */
/* ------------------------------------------------------------------ */
/* DS 0x800229e4 (Esp_43) */
void Esp_43(void)
{
	LONG water;
	LONG ret;
	ESP_WORK *pEsp;

	water = Water_ck(pESP->Pos_x, pESP->Pos_z);
	pEsp = pESP;
	if (pEsp->Pos_y < water) {
		pEsp->Routine1 = 0;
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		ret = Esp_call3(((LONG)pESP->Mag * 0x8c / 100) | 0xf010000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		if (ret < 0xff) {
			Esp[ret].Free0 = 3;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_44 - Enemy-attack proximity (Oba_ck_em_at) with vibration and    */
/*          ground/water checks; otherwise two random spawn calls        */
/*          (DS 80022abc)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022abc (Esp_44) */
void Esp_44(void)
{
	VECTOR tvec;
	LONG water;
	LONG ground;
	LONG hit;
	LONG a;
	LONG b;
	ULONG mag;

	if (pESP->Anm_cnt == 10) {
		pESP->Anm_cnt = 6;
		pESP->Anm_time = pESP->pAnmseq[pESP->Anm_cnt].Time;
	}
	hit = Oba_ck_em_at((SVECTOR *)&pESP->Pos_x, 0x96, 1 << ((-((LONG)pESP->Pos_y / 0x708)) & 0x1f));
	if (hit != 0) {
		if (hit == 2) {
			Pad_vib_fade_set(4, 0x78, 0x14, 0);
		}
		pESP->Anm_cnt = 0xb;
		pESP->Anm_time = pESP->pAnmseq[pESP->Anm_cnt].Time;
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Transy = 0x1000;
		pESP->Speed_y = 0;
		pESP->Add_y = 0;

		a = Rnd();
		mag = pESP->Mag;
		b = Rnd();
		Esp_call3((mag + (a % 8) * 0x80) | 0x1a000000, (SHORT)(pESP->DirY - ((b % 10) * 12 - 624)), (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);

		a = Rnd();
		mag = pESP->Mag;
		b = Rnd();
		Esp_call3((mag + (a % 8) * 0x80) | 0x1a000000, (SHORT)(pESP->DirY + (b % 10) * 12 + 0xd90), (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
	} else {
		pESP->Transy = (USHORT)(pESP->Transy + 0x78);
		water = Water_ck(pESP->Pos_x, pESP->Pos_z);
		if (water != 0 && water < pESP->Pos_y) {
			pESP->Anm_cnt = 0xb;
			pESP->Anm_time = pESP->pAnmseq[pESP->Anm_cnt].Time;
			pESP->Routine1 = 0;
			pESP->Routine0 = 0;
			pESP->Transy = 0x1000;
			pESP->Speed_y = 0;
			pESP->Add_y = 0;
		}
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		ground = Sca_get_ground(&tvec, 2, 0x2000, 1);
		if (ground < pESP->Pos_y) {
			pESP->Anm_cnt = 8;
			pESP->Anm_time = pESP->pAnmseq[pESP->Anm_cnt].Time;
			pESP->Routine1 = 0;
			pESP->Routine0 = 0;
			pESP->Add_y = 0;
			pESP->Speed_y = 0;
			pESP->Pos_y = (SHORT)ground;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_45 - Three-shot spray with fading Mag; Free0 / Free1 countdown    */
/*          (DS 80022ebc)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80022ebc (Esp_45) */
void Esp_45(void)
{
	LONG tp;
	LONG r;

	tp = pESP->Type & 0xf8;
	if (pESP->Free0 != 0) {
		if (pESP->Free0 != 1) {
			return;
		}
	} else {
		pESP->Be_flg = 0x8002;
		pESP->Free0 = 1;
		pESP->Free1 = 0x5a;
	}
	if (pESP->Free1 != 0) {
		/* Three sparks (three calls in DS). Each spark draws its type random first,
		   then its direction random. */
		r = Rnd_s32();
		Esp_call3(((r % 3 + tp) << 16) | pESP->Mag | 0x26000000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		r = Rnd_s32();
		Esp_call3(((r % 3 + tp) << 16) | pESP->Mag | 0x26000000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		r = Rnd_s32();
		Esp_call3(((r % 3 + tp) << 16) | pESP->Mag | 0x26000000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		if (pESP->Free1 < 0x41) {
			pESP->Mag = (USHORT)((pESP->Mag * 0x3bb) / 1000);
		}
		pESP->Free1--;
	} else {
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_46 - Countdown Free0; hit / ground / water test sets Free0 = 6    */
/*          and Free1 = 1, then a spark call at Free0 == 3; the second   */
/*          spark call depends on an unset kind, as in retail           */
/*          (DS 80023204)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x80010964 (Esp_aw_46) */
const ATTACK_WORK Esp_aw_46 = { -1400, 0, 400, 200 };

/* DS 0x80023204 (Esp_46) */
void Esp_46(void)
{
	VECTOR tvec;
	ATTACK_WORK Aw;
	LONG water;
	LONG ground;
	ULONG hit;
	LONG kind;
	SSEQUENCE *pAnm;
	ESP_WORK *pEsp;
	MATRIX *id_mtx;	/* held in a saved register across the two calls of the Free0 == 3 path */

	Aw = Esp_aw_46;
	pESP->Free0--;
	if (pESP->Free0 == 0) {
		pESP->Add_y = (CHAR)-10;
	}
	if (pESP->Free1 != 0) {
		if (pESP->Free0 == 3) {
			/* Retail ignores the Esp_call3 result here and tests kind (s0) against 2
			   (0x800232bc: li v0,2 / bne s0,v0). kind is not set on this path, so the
			   test reads whatever the caller left in that register. Kept as in retail. */
			id_mtx = (MATRIX *)&GsIDMATRIX;
			Esp_call3(0x40c2000, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
			if (kind == 2) {
				Esp_call3((ULONG)((Rnd_s32() % 5) * 100 + 0x2100) | 0x5060000, 0, id_mtx, (SVECTOR *)&pESP->Pos_x);
			}
		}
	} else {
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		kind = 0;
		/* Retail passes Pos_x as both arguments of Water_ck here. */
		water = Water_ck(pESP->Pos_x, pESP->Pos_x);
		if (water != 0 && water < pESP->Pos_y) {
			/* Signed int division as in retail (mult / sra at 0x800233a8); Mag * 65 is never
			   negative, so the quotient equals the unsigned one and the compiler drops the
			   sign fix-up. CCPSX also leaves an unused 8-byte stack slot for this division
			   (sp+0x28..0x2f), which retail's 64-byte frame has and never reads. */
			pESP->Mag = (USHORT)(pESP->Mag * 0x41 / 100);
			Esp_call3(0x30f2000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
			kind = 3;
		}
		hit = Pos_em_at_ck(&tvec, pESP->DirY, &Aw, 0x20010);
		if (hit != 0) {
			G.Wep_hit_pos.vx = pESP->Pos_x;
			G.Wep_hit_pos.vy = pESP->Pos_y;
			G.Wep_hit_pos.vz = pESP->Pos_z;
			kind = 2;
		}
		ground = Sca_get_ground(&tvec, 2, 0x2000, 0);
		if (ground < tvec.vy) {
			kind = 1;
		}
		if (Ground_in_flg != 0) {
			kind = 1;
			/* Signed int division as in retail (mult, sra 31 and subu at 0x80023498..0x800234ac). */
			pESP->Mag = (USHORT)(pESP->Mag * 0x46 / 100);
		}
		if (kind != 0) {
			pEsp = pESP;
			pEsp->Add_x = 0;
			pEsp->Speed_x = 0;
			pESP->Add_y = 0;
			pESP->Anm_cnt = 0x0c;
			pAnm = pESP->pAnmseq + pESP->Anm_cnt;
			pESP->Anm_time = pAnm->Time;
			pESP->Speed_y = 0x14;
			pESP->Free0 = 6;
			pESP->Free1 = 1;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_47 - Spawns child Esp entries every 4th frame, each advanced by   */
/*          the frame count, until the child hits ground or water;      */
/*          then sets self to Routine0 0x55 (DS 80023554)               */
/* ------------------------------------------------------------------ */
/* DS 0x80023554 (Esp_47) */
void Esp_47(void)
{
	ESP_WORK *pSelf;
	LONG below;
	ESP_WORK *pCur;	/* pESP as loaded by the depth test; DS reuses that register for Anm_cnt */
	ESP_WORK *pNew;	/* pESP reloaded after the Anm_cnt store, shared by Routine1, Routine0 and Transy */
	VECTOR tvec;
	LONG water;
	LONG ground;
	UCHAR ret;
	ULONG n;
	ULONG k;
	UCHAR stop_anm;	/* held in a saved register across the calls */

	Esp_01();
	n = 1;
	stop_anm = 10;
	pSelf = pESP;
	do {
		n++;
		if ((n & 3) == 0) {
			ret = Esp_call3(((ULONG)pSelf->Mag) | 0x1c020000, pSelf->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pSelf->Hot_x);
			if (ret < 0xff) {
				pESP = &Esp[ret];
				/* DS has no zero test on the count (n is at least 4 here), so the loop is a do-while. */
				k = n;
				do {
					Esp_pos(1);
					pESP->Free3 = 0;
				} while (--k != 0);
				pESP->Be_flg = 0xb003;
				pESP->Free3 = 6;
				pESP->Routine0 = 0;
				pESP->Routine1 = 0x44;
				tvec.vx = pESP->Pos_x;
				tvec.vy = pESP->Pos_y;
				tvec.vz = pESP->Pos_z;
				water = Water_ck(pESP->Pos_x, pESP->Pos_z);
				ground = Sca_get_ground(&tvec, 2, 0x2000, 1);
				/* DS loads pESP separately in each arm of the depth test, into one register that the
				 * Anm_cnt store below then reuses; the water arm comes first (beq water, zero). */
				if (water != 0) {
					pCur = pESP;
					below = water < pCur->Pos_y;
				} else {
					pCur = pESP;
					below = ground < pCur->Pos_y;
				}
				if (below) {
					pCur->Anm_cnt = stop_anm;
					n = 0;
					/* DS reloads pESP here for the first three stores and again for Speed_y/Add_y. */
					pNew = pESP;
					pNew->Routine1 = 0;
					pNew->Routine0 = 0;
					pNew->Transy = 0x1000;
					pESP->Speed_y = 0;
					pESP->Add_y = 0;
				}
			} else {
				n = 0;
			}
		}
	} while (n != 0);
	pESP = pSelf;
	pSelf->Routine0 = 0x55;
	pSelf->Be_flg = 0x8000;
	pESP->Routine1 = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_48 - Esp_01 step, then random Transx, Anm_cnt (+0/1), Cdir_x/y    */
/*          (DS 8002373c)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8002373c (Esp_48) */
void Esp_48(void)
{
	SSEQUENCE *pAnm;

	Esp_01();
	pESP->Transx = (USHORT)(pESP->Transx + (Rnd_s32() % 5) * 0x200);
	pESP->Anm_cnt = (UCHAR)(pESP->Anm_cnt + Rnd_s32() % 2);
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	pESP->Cdir_x = (SHORT)(pESP->Free3 + (Rnd_s32() % 5) * 3);
	pESP->Cdir_y = (SHORT)(pESP->Free4 + (Rnd_s32() % 5) * 3);
	pESP->Free0 = 4;
}

/* ------------------------------------------------------------------ */
/* Esp_49 - When animation reaches Free0: Add_y -1, Add_x -3, halve Speed_x */
/*          and clear Routine0 (DS 8002387c)                            */
/* ------------------------------------------------------------------ */
/* DS 0x8002387c (Esp_49) */
void Esp_49(void)
{
	if (pESP->Anm_cnt == pESP->Free0) {
		pESP->Add_y = -1;
		pESP->Add_x = -3;
		pESP->Speed_x = (SHORT)(pESP->Speed_x / 2);
		pESP->Routine0 = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_4a - Ten spawn calls with Type-derived bits and random direction, */
/*          then clear routine (DS 800238e4)                            */
/* ------------------------------------------------------------------ */
/* DS 0x800238e4 (Esp_4a) */
void Esp_4a(void)
{
	UCHAR type;
	LONG n;
	ESP_WORK *pEsp;

	type = pESP->Type;
	n = 10;
	do {
		n--;
		Esp_call3((((ULONG)type & 0xf8) << 16) | pESP->Mag | 0x1a000000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pEsp = pESP;
	} while (n != 0);
	pEsp->Routine1 = 0;
	pEsp->Routine0 = 0;
	pEsp->Be_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_4b - Flag-gated emitter: 5-frame spawn cadence, Transx decay and  */
/*          Free4 countdown (DS 800239c4)                               */
/* ------------------------------------------------------------------ */
/* DS 0x800239c4 (Esp_4b) */
void Esp_4b(void)
{
	ULONG flg;
	UCHAR ret;
	ESP_WORK *self;
	ESP_WORK *pNew;

	flg = Flg_ck(G.SaveData.Common_flg, 0x76);
	/* DS tests the flag with beq (flag clear jumps to the later block), so the flag-set
	 * block comes first; its Free4 >= 0x5b reset block precedes the decay block. */
	if (flg != 0) {
		pESP->Free4++;
		if (pESP->Free4 >= 0x5b) {
			/* DS keeps pESP in one register for the three stores. */
			self = pESP;
			self->Routine1 = 0;
			self->Routine0 = 0;
			self->Be_flg = 0;
		} else {
			if (pESP->Free3 % 5 == 0) {
				/* Transx decays to 80%. The product is int (USHORT promoted):
				 * retail divides with a signed mult / sra 5, the sign fix-up
				 * folded away because Transx * 0x50 is never negative. */
				pESP->Transx = (USHORT)((pESP->Transx * 0x50) / 100);
				ret = Esp_call3((((ULONG)pESP->Id) << 24) | 0x10000 | pESP->Mag, pESP->DirY, (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
				if (ret < 0xff) {
					/* DS forms &Esp[ret] once, then reloads pESP for the Transx copy. */
					pNew = &Esp[ret];
					pNew->Be_flg = 0xb803;
					pNew->Transx = pESP->Transx;
				}
			}
		}
	} else {
		pESP->Be_flg = 0x8000;
		if (pESP->Free3 % 5 == 0) {
			ret = Esp_call3((((ULONG)pESP->Id) << 24) | 0x10000 | pESP->Mag, pESP->DirY, (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
			if (ret < 0xff) {
				Esp[ret].Be_flg = 0xb803;
			}
		}
	}
	pESP->Free3++;
}

/* ------------------------------------------------------------------ */
/* Esp_4c - Player proximity (300): poison / vibration or flag, then one  */
/*          spawn call (DS 80023b94)                                    */
/* ------------------------------------------------------------------ */
/* DS 0x80023b94 (Esp_4c) */
void Esp_4c(void)
{
	VECTOR tvec;
	ENEMY_WORK *pl;

	tvec.vx = pESP->Pos_x;
	tvec.vy = pESP->Pos_y;
	tvec.vz = pESP->Pos_z;
	pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 300, 5);
	if (pl != 0) {
		if (pl->Id < 0x10) {
			G.Pl_poison_timer = 0x1518;
			G.Pl_poison_down = 0x1e;
			Pad_vib_set0(8, 0);
			Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
		} else {
			G.SaveData.Dummy1[1] |= 1;
		}
		Esp_call3(0x30f2000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
	}
}

/* ------------------------------------------------------------------ */
/* Esp_4d - Six-shot spray from Hot_* with Mag-scaled bits and random   */
/*          angles; Pos-side vy steps by Mag/15 (DS 80023c70)            */
/* ------------------------------------------------------------------ */
/* DS 0x80023c70 (Esp_4d) */
void Esp_4d(void)
{
	SVECTOR svec[3];	/* only [0] is used; the other two entries are unused */
	ULONG tbits;
	ULONG code;
	LONG a;
	SHORT dir;
	USHORT mag;

	svec[0].vx = pESP->Hot_x;
	svec[0].vy = pESP->Hot_y;
	svec[0].vz = pESP->Hot_z;
	tbits = ((ULONG)pESP->Type & 0xf8) << 16;

	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)(mag * 0x3c) / 100 + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY - ((Rnd_s32() % 10) * 12 - 624));
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);
	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)(mag * 0x3c) / 100 + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY + (Rnd_s32() % 10) * 12 + 0xd90);
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);
	svec[0].vy += (SHORT)(pESP->Mag / 15);

	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)(mag * 0x50) / 100 + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY - ((Rnd_s32() % 10) * 12 - 624));
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);
	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)(mag * 0x50) / 100 + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY + (Rnd_s32() % 10) * 12 + 0xd90);
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);
	svec[0].vy += (SHORT)(pESP->Mag / 15);

	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)mag + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY - ((Rnd_s32() % 10) * 12 - 624));
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);
	a = Rnd_s32();
	mag = pESP->Mag;
	code = tbits | ((ULONG)((LONG)mag + (a % 8) * 0x80) | 0x1a000000);
	dir = (SHORT)(pESP->DirY + (Rnd_s32() % 10) * 12 + 0xd90);
	Esp_call3(code, dir, (MATRIX *)pESP->pMat, &svec[0]);

	pESP->Routine1 = 0;
	pESP->Routine0 = 0;
	pESP->Be_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_4e - Esp_02 step, then scale Add_* and Speed_* by Mag / 3000     */
/*          (DS 800241c8)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800241c8 (Esp_4e) */
void Esp_4e(void)
{
	Esp_02();
	pESP->Add_x = (CHAR)((LONG)(signed char)pESP->Add_x * pESP->Mag / 3000);
	pESP->Add_y = (CHAR)((LONG)(signed char)pESP->Add_y * pESP->Mag / 3000);
	pESP->Add_z = (CHAR)((LONG)(signed char)pESP->Add_z * pESP->Mag / 3000);
	pESP->Speed_x = (SHORT)((LONG)pESP->Speed_x * pESP->Mag / 3000);
	pESP->Speed_y = (SHORT)((LONG)pESP->Speed_y * pESP->Mag / 3000);
	pESP->Speed_z = (SHORT)((LONG)pESP->Speed_z * pESP->Mag / 3000);
}

/* ------------------------------------------------------------------ */
/* Esp_4f - Decay Transx/Transy; when the player is within 600, set     */
/*          Padd0 and fire a spark, then poison / vibrate (DS 8002434c)  */
/* ------------------------------------------------------------------ */
/* DS 0x8002434c (Esp_4f) */
void Esp_4f(void)
{
	VECTOR tvec;
	SVECTOR svec[3];		/* only [0] is used; the other two entries are unused */
	ENEMY_WORK *pl;
	ESP_WORK *pEsp;

	if ((signed char)pESP->Padd0 == 3) {
		pESP->Transx = (USHORT)((pESP->Transx * 0x5d) / 100);
		pESP->Transy = (USHORT)((pESP->Transy * 0x62) / 100);
	} else {
		pESP->Transx = (USHORT)((pESP->Transx * 0x67) / 100);
		tvec.vx = pESP->Old_x;
		tvec.vy = pESP->Old_y;
		tvec.vz = pESP->Old_z;
		pl = (ENEMY_WORK *)Pos_pl_at_ck(&tvec, 600, 5);
		if (pl != 0) {
			if ((signed char)pESP->Padd0 == 0) {
				pESP->Padd0 |= 0x80;
				tvec.vx = pESP->Pos_x;
				tvec.vy = pESP->Pos_y;
				tvec.vz = pESP->Pos_z;
				Snd_se_stad(0x31d0001, &tvec);
				svec[0].vx = (SHORT)((SHORT)tvec.vx - (SHORT)pl->Pos_x);
				svec[0].vy = (SHORT)((SHORT)tvec.vy - (SHORT)pl->Pos_y);
				svec[0].vz = (SHORT)((SHORT)tvec.vz - (SHORT)pl->Pos_z);
				Esp_call3(0x30f1000, 0, (MATRIX *)&pl->M, &svec[0]);
			}
			pEsp = pESP;	/* DS loads pESP once for the three stores */
			pEsp->Routine1 = 0;
			pEsp->Routine0 = 0;
			pEsp->Be_flg = 0;
			if (pl->Id < 0x10) {
				Pad_vib_set0(8, 0);
				Pad_vib_fade_set(0xe, 0xb4, 0x3c, 0);
			}
		}
	}
}

/* ------------------------------------------------------------------ */
/* Esp_50 - Reset to anim 10 with Transx/Transy 0x1000; Padd0 selects a  */
/*          launch (Cdir_x 0x400 / 0xc00) or a one-shot sound + spawn   */
/*          (DS 8002453c)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x8002453c (Esp_50) */
void Esp_50(void)
{
	VECTOR tvec;
	VECTOR mov;
	SSEQUENCE *pAnm;

	pESP->Transx = 0x1000;
	pESP->Transy = 0x1000;
	pESP->Anm_cnt = 10;
	pAnm = pESP->pAnmseq + pESP->Anm_cnt;
	pESP->Anm_time = pAnm->Time;
	pESP->Speed_x = 0;
	pESP->Add_x = 0;
	pESP->Speed_y = 0;
	pESP->Add_y = 0;
	pESP->Routine1 = 0;

	switch (pESP->Padd0 & 0x7f) {
	case 0:
		pESP->Padd0 = 3;
		pESP->Cdir_z = 0;
		if ((signed char)pESP->Padd0 & 0x80) {
			return;
		}
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		/* DS also copies Mov_x/y/z into mov and never reads them. Kept so the frame and stores match DS. */
		mov.vx = pESP->Mov_x;
		mov.vy = pESP->Mov_y;
		mov.vz = pESP->Mov_z;
		Snd_se_stad(0x31d0001, &tvec);
		Esp_call3(0x4091800, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
		return;
	case 1:
		pESP->Padd0 = 3;
		pESP->Cdir_x = 0x400;
		pESP->Cdir_y = 0x800;
		pESP->Cdir_z = 0;
		break;
	case 2:
		pESP->Padd0 = 3;
		pESP->Cdir_x = 0xc00;
		pESP->Cdir_y = 0x800;
		pESP->Cdir_z = 0;
		break;
	default:
		return;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_51 - Esp_01 step, then two spawns with Padd0 1 / 2 and fixed        */
/*          Cdir values (DS 800246e8)                                   */
/* ------------------------------------------------------------------ */
/* DS 0x800246e8 (Esp_51) */
void Esp_51(void)
{
	UCHAR ret;
	ESP_WORK *pNew;

	Esp_01();
	pESP->Cdir_y = 0x800;
	pESP->Cdir_z = 0x82;
	ret = Esp_call3(((ULONG)pESP->Id << 24) | ((ULONG)pESP->Type << 16) | pESP->Mag, pESP->DirY, (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	if (ret < 0xff) {
		pNew = &Esp[ret];
		pNew->Routine0 = 1;
		pNew->Padd0 = 1;
		pNew->Cdir_x = 0x400;
		pNew->Cdir_y = 0x768;
		pNew->Cdir_z = 0xfc4;
	}
	ret = Esp_call3(((ULONG)pESP->Id << 24) | ((ULONG)pESP->Type << 16) | pESP->Mag, pESP->DirY, (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
	if (ret < 0xff) {
		pNew = &Esp[ret];
		pNew->Routine0 = 1;
		pNew->Padd0 = 2;
		pNew->Cdir_x = 0xc00;
		pNew->Cdir_y = 0x898;
		pNew->Cdir_z = 0xfc4;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_52 - Five-state burst sequence on Free0 (0..4): Hot_y steps and  */
/*          spawn calls keyed on Type & 0xf8 (DS 80024818)              */
/* ------------------------------------------------------------------ */
/* DS 0x80024818 (Esp_52) */
void Esp_52(void)
{
	ULONG tp;
	ULONG tp4;
	LONG r;

	tp = (ULONG)pESP->Type & 0xf8;
	switch (pESP->Free0) {
	case 0:
		pESP->Free0 = 1;
		pESP->Be_flg = 0x8002;
		pESP->Free3 = pESP->Hot_y;
		tp = (tp | 1) << 16;
		r = Rnd_s32();
		Esp_call3(tp | (((LONG)pESP->Mag * 0xb4) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y - (SHORT)(((LONG)pESP->Mag * 300) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp | (((LONG)pESP->Mag * 0x96) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y - (SHORT)(((LONG)pESP->Mag * 0x122) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp | (((LONG)pESP->Mag * 0x78) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 1:
		pESP->Free0 = 2;
		pESP->Hot_y = (SHORT)(pESP->Hot_y - (SHORT)(((LONG)pESP->Mag * 0x104) / 0x1000));
		r = Rnd_s32();
		tp = (tp | 1) << 16;
		Esp_call3(tp | (((LONG)pESP->Mag * 0x73) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y - (SHORT)(((LONG)pESP->Mag * 0xe6) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp | (ULONG)pESP->Mag | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 2:
		pESP->Free0 = 3;
		pESP->Hot_y = (SHORT)(pESP->Hot_y - (SHORT)(((LONG)pESP->Mag * 0xbe) / 0x1000));
		r = Rnd_s32();
		Esp_call3(((tp | 1) << 16) | (((LONG)pESP->Mag * 0x5a) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 3:
		pESP->Free0 = 4;
		pESP->Hot_y = (SHORT)(pESP->Hot_y + (SHORT)(((LONG)pESP->Mag * 0xbe) / 0x1000));
		r = Rnd_s32();
		tp = (tp | 1) << 16;
		Esp_call3(tp | (((LONG)pESP->Mag * 0x5a) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y + (SHORT)(((LONG)pESP->Mag * 0xe6) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp | (((LONG)pESP->Mag * 0x69) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 4:
		pESP->Hot_y = (SHORT)(pESP->Hot_y + (SHORT)(((LONG)pESP->Mag * 0x104) / 0x1000));
		r = Rnd_s32();
		tp4 = (tp + 1) * 0x10000;
		Esp_call3(tp4 | (((LONG)pESP->Mag * 0x6e) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y + (SHORT)(((LONG)pESP->Mag * 0x122) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp4 | (((LONG)pESP->Mag * 0x8c) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Hot_y = (SHORT)(pESP->Hot_y + (SHORT)(((LONG)pESP->Mag * 300) / 0x1000));
		r = Rnd_s32();
		Esp_call3(tp4 | (((LONG)pESP->Mag * 0xaa) / 100) | 0x1a000000, (SHORT)(pESP->DirY + (r % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		Esp_call3(((tp + 4) << 16) | (ULONG)pESP->Mag | 0x1a000000, pESP->DirY, (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pESP->Routine1 = 0;
		pESP->Routine0 = 0;
		pESP->Be_flg = 0;
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Esp_53 - Underwater spawn when below the water line, then idle       */
/*          (DS 800252b8)                                               */
/* ------------------------------------------------------------------ */
/* DS 0x800252b8 (Esp_53) */
void Esp_53(void)
{
	ESP_WORK *self;
	LONG water;

	water = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (water != 0 && water < pESP->Pos_y) {
		Esp_call3((ULONG)pESP->Mag | 0x1a050000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
	}
	self = pESP;
	self->Routine1 = 0;
	self->Routine0 = 0;
	self->Be_flg = 0;
}

/* ------------------------------------------------------------------ */
/* Esp_54 - Clear routine and flag, then one spawn call (DS 80025348)   */
/* ------------------------------------------------------------------ */
/* DS 0x80025348 (Esp_54) */
void Esp_54(void)
{
	ESP_WORK *self;

	self = pESP;
	self->Routine1 = 0;
	self->Routine0 = 0;
	self->Be_flg = 0;
	Esp_call3((ULONG)pESP->Mag | 0x14000000, 0, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Pos_x);
}

/* ------------------------------------------------------------------ */
/* Esp_55 - Every 4th frame (Free3 & 3 == 0) spawn from Hot_*; advance  */
/*          Free3 (DS 8002539c)                                         */
/* ------------------------------------------------------------------ */
/* DS 0x8002539c (Esp_55) */
void Esp_55(void)
{
	if ((pESP->Free3 & 3) == 0) {
		Esp_call3((ULONG)pESP->Mag | 0x1c020000, pESP->DirY, (MATRIX *)&GsIDMATRIX, (SVECTOR *)&pESP->Hot_x);
	}
	pESP->Free3++;
}

/* ------------------------------------------------------------------ */
/* Esp_56 - Spawn from Free0 state with random direction; Free0 0/1/3   */
/*          fire twice, 6 fires once and resets routine (DS 80025408)   */
/* ------------------------------------------------------------------ */
/* DS 0x80025408 (Esp_56) */
void Esp_56(void)
{
	ESP_WORK *pEsp;

	switch (pESP->Free0) {
	case 0:
		Esp_call3((ULONG)pESP->Mag | 0x2060000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 1:
	case 3:
		Esp_call3((ULONG)pESP->Mag | 0x2060000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		Esp_call3((ULONG)pESP->Mag | 0x2060000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		break;
	case 6:
		Esp_call3((ULONG)pESP->Mag | 0x2060000, (SHORT)(pESP->DirY + (Rnd_s32() % 30) * 400), (MATRIX *)pESP->pMat, (SVECTOR *)&pESP->Hot_x);
		pEsp = pESP;
		pEsp->Routine1 = 0;
		pEsp->Routine0 = 0;
		pEsp->Be_flg = 0;
		break;
	case 2:
	case 4:
	case 5:
	default:
		break;
	}
	pESP->Free0++;
}

/* ------------------------------------------------------------------ */
/* Esp_57 - Ground / water landing: set animation 8, clamp Pos_y to the   */
/*          surface, clear routine (DS 8002563c)                        */
/* ------------------------------------------------------------------ */
/* DS 0x8002563c (Esp_57) */
void Esp_57(void)
{
	VECTOR tvec;
	LONG level;
	ESP_WORK *pEsp;
	ESP_WORK *pEsp2;

	level = Water_ck(pESP->Pos_x, pESP->Pos_z);
	if (level != 0 && level < pESP->Pos_y) {
		pESP->Anm_cnt = 8;
	} else {
		tvec.vx = pESP->Pos_x;
		tvec.vy = pESP->Pos_y;
		tvec.vz = pESP->Pos_z;
		level = Sca_get_ground(&tvec, 2, 0x2000, 1);
		if (pESP->Pos_y <= level) {
			return;
		}
		pESP->Anm_cnt = 8;
	}
	pESP->Anm_time = pESP->pAnmseq[pESP->Anm_cnt].Time;
	pEsp = pESP;
	pEsp->Routine1 = 0;
	pEsp->Routine0 = 0;
	pEsp2 = pESP;
	pEsp2->Add_y = 0;
	pEsp2->Speed_y = 0;
	pESP->Pos_y = (SHORT)level;
}

/* ------------------------------------------------------------------ */
/* Esp_58 - Empty (vibration stub in retail) (DS 80025754)              */
/* ------------------------------------------------------------------ */
/* DS 0x80025754 (Esp_58) */
void Esp_58(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_59 - Empty (vibration stub in retail) (DS 8002575c)              */
/* ------------------------------------------------------------------ */
/* DS 0x8002575c (Esp_59) */
void Esp_59(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5a - Empty (vibration stub in retail) (DS 80025764)              */
/* ------------------------------------------------------------------ */
/* DS 0x80025764 (Esp_5a) */
void Esp_5a(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5b - Empty (vibration stub in retail) (DS 8002576c)              */
/* ------------------------------------------------------------------ */
/* DS 0x8002576c (Esp_5b) */
void Esp_5b(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5c - Empty (vibration stub in retail) (DS 80025774)              */
/* ------------------------------------------------------------------ */
/* DS 0x80025774 (Esp_5c) */
void Esp_5c(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5d - Empty (vibration stub in retail) (DS 8002577c)              */
/* ------------------------------------------------------------------ */
/* DS 0x8002577c (Esp_5d) */
void Esp_5d(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5e - Empty (vibration stub in retail) (DS 80025784)              */
/* ------------------------------------------------------------------ */
/* DS 0x80025784 (Esp_5e) */
void Esp_5e(void)
{
}

/* ------------------------------------------------------------------ */
/* Esp_5f - Empty (vibration stub in retail) (DS 8002578c)              */
/* ------------------------------------------------------------------ */
/* DS 0x8002578c (Esp_5f) */
void Esp_5f(void)
{
}

