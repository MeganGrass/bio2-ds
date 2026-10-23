/* em10_s0.c - reconstruction of D:\Bio2\prog\EM10_S0.C (enemy 10: death, scenario,
 * player-damage and miscellaneous routines).  Retail EM10.C #includes this file:
 * its static tables point at the static functions here and vice versa, and both
 * files' code and data form one object.  em10.c therefore #includes it at its end;
 * compiled on its own (the EM10_S0.C gate) the guarded block below supplies the
 * declarations em10.c would otherwise provide.  Linked into overlays 0x05 and 0x23;
 * the enemy-number-specific names are selected by OVL_23 (see em10.c). */
#ifndef EM10_C	/* standalone build */
#include "EM10.H"

#ifdef OVL_23
#define EM_MAIN		Em210
#define PLDAMEGE_EM	Pldamege_Em210
#define PLDIE_EM	Pldie_Em210
#else
#define EM_MAIN		Em110
#define PLDAMEGE_EM	Pldamege_Em110
#define PLDIE_EM	Pldie_Em110
#endif

extern GLOBAL G;

/* ---- functions of this file ---- */

/* ---- objects and functions of D:\Bio2\prog\EM10.C (static there) ---- */
#endif

static void Em10_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;

	if (!(FREE_US(2) & 0x4000)) {	/* retail-only: count each zombie death once (retail 0x80108260..0x80108298; */
		G.SaveData.Zonbi_cnt++;			/* the counter is G_Zonbi_cnt in references/SLUS_007.48/r3000a/Enemy/em10/Die/Em10Die.s:12) */
		FREE_US(2) |= 0x4000;	}
	FREE_US(2) &= 0xfdff;

	if (G.Player.Routine_0 == 5 && G.Player.pEnemy_ptr == (unsigned long *)pEm) {

		*(long *)&G.Player.Routine_0 = 1;
		G.Player.Damage_cnt &= 0x7f;
		G.Player.Neck_flg &= 0xed;

		pEm->Be_flg &= ~0x1004;
		G.Player.Be_flg &= ~0x1004;	}

	if (!pEm->Routine_2) {	/* retail-only (retail 0x80108328) */
		pP_ptr = &pEm->pSin_parts_ptr[3];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[4];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[6];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[7];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg |= 0x8000;	}

	if (pEm->Type & 1) {
		Em10_haidie[ pEm->Routine_1 - 1 ](pEm, Kan, Seq);
		return;	}

	if (FREE_US(2) & 0x10) {
		Em10_die05(pEm, Kan, Seq);

		return;	}

	if (FREE_US(2) & 2) {

		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x1f40, 0, &pP_ptr->Workm, 0);

		if (Rnd() & 1) {
			Snd_se_enem(13, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80))
			Em10_Firedm(pEm);

		if (pEm->Routine_1 == 11)
			Em10_Ryudm(pEm);

		if ((pEm->Routine_1 == 9 || pEm->Routine_1 == 17) && !(pEm->Type & 0x80))	/* retail: damage type 17 also */
			Em10_Tudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		*(long *)&pEm->Routine_0 = 0x907;

		return;
	}

	Em10_die[ pEm->Routine_1 - 1 ][ pEm->Damage_no ](pEm, Kan, Seq);

}

static void Em10_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv06_mot[2] = { 1, 2 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(signed char *)&pEm->Timer2 = Rnd() & 1;

		if (FREE_US(2) & 0x2000)
			*(signed char *)&pEm->Timer2 = 0;

		FREE_US(2) &= 0xfffb;
		if (*(signed char *)&pEm->Timer2)
			FREE_US(2) |= 4;

		*(long *)&pEm->Move_no = (signed char)pEm->Timer2 * 0xa00 + 0xf0000 + Em10_mv06_mot[ (signed char)pEm->Timer2 ];
		pEm->Routine_2 = 1;

		pEm->Neck_flg |= 1;

		pEm->Damage_cnt |= 0x80;

		if (pEm->Damage_no % 3) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = pP_ptr->Pos_x;
			Sv.vy = pP_ptr->Pos_y + 300;
			Sv.vz = pP_ptr->Pos_z;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call(0x1fa0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}else {

			pP_ptr = &pEm->pSin_parts_ptr[1];
			Sv.vx = 0;
			Sv.vy = 800;
			Sv.vz = 0;
			Esp_call(0x17d0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}

		if (Rnd() & 1) {
			Snd_se_enem(13, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		Em_dead_flg_on(pEm);

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80))
			Em10_Firedm(pEm);

		pEm->Timer3 = 0;

		if (pEm->Routine_1 == 11) {
			Em10_Ryudm(pEm);

			if (Rnd() & 1) {
				pP_ptr = &pEm->pSin_parts_ptr[8];
				pP_ptr->Be_flg |= 0x10;
				pP_ptr->Free[0] = 0;
				pP_ptr->Free[1] = 0;
				pP_ptr->Free[3] = 0x801e;
				((unsigned char *)pP_ptr->Free)[4] = 10;
				((unsigned char *)pP_ptr->Free)[5] = 10;
				pP_ptr->Poly_rgb = 0x304040;
				Esp_call(0x40f0fa0, pEm->Cdir_y, &pP_ptr->Workm, 0);

				pEm->Timer3 = 1;

			}
		}

		if ((pEm->Routine_1 == 9 || pEm->Routine_1 == 17) && !(pEm->Type & 0x80))	/* retail: damage type 17 also */
			Em10_Tudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		if (pEm->Routine_1 == 16)
			if (!(pEm->Type & 0x80))
				Em10_Firedm(pEm);

		pEm->Spd_x = 11;

	case 1:
		if (pEm->Damage_no % 3) {
			if (pEm->Move_cnt >= (signed char)pEm->Timer2 * 10 + 4 && pEm->Move_cnt <= (signed char)pEm->Timer2 * 10 + 16) {

				if (pEm->Move_cnt & 1) {

					pP_ptr = &pEm->pSin_parts_ptr[8];
					Sv.vx = pP_ptr->Pos_x;
					Sv.vy = pP_ptr->Pos_y + 500;
					Sv.vz = pP_ptr->Pos_z;
					pP_ptr = pEm->pSin_parts_ptr;
					Esp_call((Rnd() << 3) + 0xfd0, Rnd() << 4, &pP_ptr->Workm, &Sv);

				}
			}
		}

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);	}

		break;

	case 2:

		*(long *)&pEm->Routine_0 = 7;
		if (!(Rnd() & 3) && !(FREE_US(2) & 4) && !pEm->Water && !*(signed char *)&pEm->Timer3 && !(G.Status_flg & 0x10000000)) {	/* retail: + status flag test (retail 0x8010895C) */

			pEm->Life = 1;

			pEm->Be_flg &= 0xf3ffffff;
			pEm->Be_flg |= 0x4000000;

			pEm->Atd[0].At_w = 200;
			pEm->Atd[0].At_d = 200;
			pEm->Atd[0].W = 200;
			pEm->Atd[0].D = 200;
			pEm->Atd[0].Ofs_y = -350;
			pEm->Atd[0].At_h = 350;
			pEm->Type = 0x2001;	/* retail 0x2001 (Beta 1) */

			if (!(Rnd() & 7))

				*(long *)&pEm->Routine_0 = 1;
			else	*(long *)&pEm->Routine_0 = 0x201;

			pEm->Damage_cnt &= 0x7f;
		}

	}
}

static void Em10_die01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv06_mot[2] = { 1, 2 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		FREE_US(2) &= 0xfffb;
		*(long *)&pEm->Move_no = 0xf0007;
		pEm->Routine_2 = 1;

		pEm->Neck_flg |= 1;

		pEm->Damage_cnt |= 0x80;

		pP_ptr = pEm->pSin_parts_ptr;
		Sv.vx = 0;
		Sv.vy = 300;
		Sv.vz = 0;
		Esp_call(0x1fa0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		if (Rnd() & 1) {
			Snd_se_enem(13, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		Em_dead_flg_on(pEm);

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80))
			Em10_Firedm(pEm);

		if (pEm->Routine_1 == 11 && !(FREE_US(2) & 0x1000))

			Em10_Ryudm(pEm);

		if ((pEm->Routine_1 == 9 || pEm->Routine_1 == 17) && !(pEm->Type & 0x80))	/* retail: damage type 17 also */
			Em10_Tudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 2:
		*(long *)&pEm->Routine_0 = 7;

	}
}

static void Em10_die02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Spd[2] = { -450, 250 };
	register PARTS_W *pP_ptr;
	register PARTS_W *pO;
	register PARTS_W *pS_ptr;	/* retail-only local, [INFERRED] name: parts base for the 9..13 blocks */
	SVECTOR Sv;
	unsigned char Em10_die02_mot[2] = { 2, 1 };

	if (FREE_UB(0x19) == 1) {
		Em10_die04(pEm, Kan, Seq);
		return;	}

	switch (pEm->Routine_2) {
	case 0:

		if ((Rnd() & 3) && !(G.Player.Be_flg & 0x80000000) && pEm->Routine_1 != 9 && pEm->Routine_1 != 17) {	/* retail: + type 17 */

			FREE_UB(0x19) = 1;
			Em10_die04(pEm, Kan, Seq);
			return;	}

		if ((pEm->Routine_1 == 9 || pEm->Routine_1 == 17) && !(pEm->Type & 0x80))	/* retail: damage type 17 also */
			Em10_Tudm(pEm);

		if (((Rnd() >> (Rnd() & 7)) & 3) && pEm->Routine_1 == 9) {
			FREE_UB(0x19) = 1;
			Em10_die04(pEm, Kan, Seq);
			return;	}

		*(signed char *)&pEm->Timer3 = (pEm->Damage_flg & 0x20) >> 5;
		FREE_US(2) &= 0xfffb;
		if (!*(signed char *)&pEm->Timer3)
			FREE_US(2) |= 4;
		*(long *)&pEm->Move_no = 0x30004 - (signed char)pEm->Timer3;
		pEm->Routine_2 = 1;
		pEm->Timer1 = 10;

		pEm->Spd_x = Spd[ (signed char)pEm->Timer3 ];
		pEm->Spd_z = 0;
		Add_speedXZ(pEm, 0);

		pEm->Neck_flg = 1;

		pEm->Damage_cnt |= 0x80;

		pP_ptr = &pEm->pSin_parts_ptr[8];

		pP_ptr->Be_flg |= 0x4a;
		pP_ptr->Free[0] = pEm->Cdir_y + 0x800;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[2] = 400;
		pP_ptr->Free[1] = -100;
		pP_ptr->Free[3] = 10;
		pP_ptr->Free[6] = -50;
		pP_ptr->Poly_rgb = 0x101030;
		Esp_call(0xbb8, Rnd() << 4, &pP_ptr->Workm, 0);
		Esp_kill(5, 3, &pP_ptr->Workm);	/* retail-only */

		/* retail: parts 9/10 only while Free flag 0x20 is clear, otherwise parts 9-11 are
		 * switched off and their effects killed; parts 12/13 get the same treatment as 9/10
		 * at random unless Free flag 0x40 (retail 0x80108E5C..0x80108F74) */
		pS_ptr = pEm->pSin_parts_ptr;
		if (!(FREE_US(2) & 0x20)) {
			pS_ptr[9].Be_flg |= 0x1062;
			pS_ptr[9].Old_x = pEm->Cdir_y + 0x800;
			pS_ptr[9].Timer3 = 0;
			pS_ptr[9].Old_z = 100;
			pS_ptr[9].Old_y = 0;
			pS_ptr[9].Timer1 = 30;
			pS_ptr[9].Old_x2 = 0;
			pS_ptr[9].Old_y2 = pEm->Cdir_y;
			pS_ptr[9].Old_z2 = 0x400;

			pP_ptr = &pEm->pSin_parts_ptr[10];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x40;
			*(long *)&pP_ptr->Free[2] = 0x101010;
		}else {
			pS_ptr[9].Be_flg = 0;
			pS_ptr[10].Be_flg = 0;
			Esp_kill(5, 3, &pS_ptr[10].Workm);
			pS_ptr[11].Be_flg = 0;
			Esp_kill(5, 3, &pS_ptr[11].Workm);
		}

		pS_ptr = pEm->pSin_parts_ptr;
		if (!(FREE_US(2) & 0x40) && !(Rnd() & 1)) {
			pS_ptr[12].Be_flg |= 0x1062;
			pS_ptr[12].Old_x = pEm->Cdir_y + 0x800;
			pS_ptr[12].Timer3 = 0;
			pS_ptr[12].Old_z = 100;
			pS_ptr[12].Old_y = 0;
			pS_ptr[12].Timer1 = 30;
			pS_ptr[12].Old_x2 = 0;
			pS_ptr[12].Old_y2 = pEm->Cdir_y;
			pS_ptr[12].Old_z2 = 0x400;

			pS_ptr[13].Be_flg |= 0x80;
			*(long *)&pS_ptr[13].Free[0] = 0x40;
			*(long *)&pS_ptr[13].Free[2] = 0x101010;
		}

		pP_ptr = &pEm->pSin_parts_ptr[11];
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101010;

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x20;
		*(long *)&pP_ptr->Free[2] = 0x20;

		pO = pEm->pSin_parts_ptr;

		pO->pTmd = pO[16].pTmd;
		pO->pTmd2 = pO[16].pTmd2;
		pO->pPacket = pO[16].pPacket;
		pO->pPacket2 = pO[16].pPacket2;

		pP_ptr = pEm->pSin_parts_ptr;

		Sv.vx = 0;
		Sv.vy = -500;
		Sv.vz = 0;
		Esp_call(0x1f40, pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4), &pP_ptr->Workm, &Sv);

		Esp_call(((Rnd() + 0xfa0) << 2) | 0x8000000, Rnd() << 4, &pP_ptr->Workm, &Sv);

		Snd_se_enem(2, pEm);

		Em_dead_flg_on(pEm);

		pEm->Spd_x = 11;

	case 1:
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x400);
		Add_speedXZ(pEm, 0);

		if (pEm->Timer1) {
			if (!(pEm->Timer1 & 1)) {

				pP_ptr = pEm->pSin_parts_ptr;
				Esp_call(0x1f40, pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4), &pP_ptr->Workm, &pEm->Spd_x);

				Esp_call(0x1f40, pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4), &pP_ptr->Workm, &pEm->Spd_x);
			}

			pEm->Timer1--;
		}

		break;

	case 2:
		if (Rnd() & 1) {
			pEm->Routine_2 = 5;
			break;	}

		*(long *)&pEm->Move_no = (((signed char)pEm->Timer3 * 15 + 10) << 8) + 0x70000 + Em10_die02_mot[ (signed char)pEm->Timer3 ];
		pEm->Routine_2 = 3;

	case 3:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);
		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);	}

		break;

	case 4:
		*(long *)&pEm->Routine_0 = 7;
		break;

	case 5:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 6;

		pEm->Timer0 = (Rnd() & 0x1f) + 15;

	case 6:
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 3;
			*(long *)&pEm->Move_no = 0x70001;
			FREE_US(2) &= 0xfffb;	}

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		Add_speedXZ(pEm, 0);

	}

}

static void Em10_die03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv06_mot[2] = { 1, 2 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;
	register short Dir;

	switch (pEm->Routine_2) {
	case 0:
		*(signed char *)&pEm->Timer2 = Rnd() & 1;

		FREE_US(2) &= 0xfffb;
		if (*(signed char *)&pEm->Timer2)
			FREE_US(2) |= 4;

		*(long *)&pEm->Move_no = (signed char)pEm->Timer2 * 0xa00 + 0x30000 + Em10_mv06_mot[ (signed char)pEm->Timer2 ];
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		Sv.vx = pP_ptr->Pos_x;
		Sv.vy = pP_ptr->Pos_y + 300;
		Sv.vz = pP_ptr->Pos_z;
		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x1f40, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		Sv.vy -= 400;
		Esp_call(0x8001b58, Dir, &pP_ptr->Workm, &Sv);
		Esp_call(0x21f40, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		pEm->Neck_flg = 1;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg |= 0x4a;
		pP_ptr->Free[0] = Dir + 0x800;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[2] = 400;
		pP_ptr->Free[1] = -100;
		pP_ptr->Free[3] = 10;
		pP_ptr->Free[6] = -50;
		pP_ptr->Poly_rgb = 0x101040;
		Esp_kill(5, 3, &pP_ptr->Workm);	/* retail-only */

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101010;

		Snd_se_enem(2, pEm);

		Em_dead_flg_on(pEm);

		pEm->Spd_x = 11;

		if (Rnd() & 1) {
			pEm->Routine_2 = 3;
			break;	}

	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);	}

		break;

	case 2:
		*(long *)&pEm->Routine_0 = 7;
		break;

	case 3:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 4;

		pEm->Timer0 = (Rnd() & 0x3f) + 30;

	case 4:
		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_2 = 1;
			*(long *)&pEm->Move_no = 0xf0001;
			FREE_US(2) &= 0xfffb;	}

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		Add_speedXZ(pEm, 0);

	}
}

static void Em10_die04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv06_mot[2] = { 1, 2 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;
	register unsigned long i;

	switch (pEm->Routine_2) {
	case 0:

		pEm->Neck_flg |= 1;

		pEm->Damage_cnt |= 0x80;

		pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);

		i = (Rnd() & 3) + 1;

		if (pEm->Routine_1 != 12) {

			pP_ptr = pEm->pSin_parts_ptr;
			Sv.vx = 0;
			Sv.vy = -300;
			Sv.vz = 0;
			Esp_call(0x1fa0, pEm->Timer0, &pP_ptr->Workm, &Sv);
			Sv.vx = 0;
			Sv.vy = -100;
			Sv.vz = 0;
			Esp_call(0x17d0, pEm->Timer0, &pP_ptr->Workm, &Sv);
			while (1) {	i--;
				Sv.vx = 0x100 - (Rnd() << 1);
				Sv.vy = 0x38 - (Rnd() << 1);
				Sv.vz = 0x100 - (Rnd() << 1);
				Esp_call(((Rnd() << 2) + 0x600) | 0x9020000, 0x200 + pEm->Timer0 - (Rnd() << 2), &pP_ptr->Workm, &Sv);

				if (!i) break;

			}	}else {

			pP_ptr = (PARTS_W *)pEm->pBow_hit_parts;
			pEm->Timer2 = ((unsigned char *)pP_ptr->Free)[1];
			pP_ptr = &pEm->pSin_parts_ptr[ (signed char)pEm->Timer2 ];
			Esp_call(0x17d0, pEm->Timer0, &pP_ptr->Workm, 0);
		}

		pEm->Timer2 = ~Cdir_ck(pEm, (ENEMY_WORK *)&G.Player) & 1;

		FREE_US(2) &= 0xfffb;
		if (*(signed char *)&pEm->Timer2)
			FREE_US(2) |= 4;

		*(long *)&pEm->Move_no = Em10_mv06_mot[ (signed char)pEm->Timer2 ] + 0xf0f00;
		pEm->Routine_2 = 1;

		if (Rnd() & 1) {
			Snd_se_enem(13, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		Em_dead_flg_on(pEm);

		pEm->Spd_x = 400;
		pEm->Spd_z = 0;

	case 1:
		if (pEm->Routine_1 != 12) {
			if (pEm->Move_cnt >= (signed char)pEm->Timer2 * 10 + 4 && pEm->Move_cnt <= (signed char)pEm->Timer2 * 10 + 16) {

				if (pEm->Move_cnt & 1) {

					pP_ptr = &pEm->pSin_parts_ptr[8];
					Sv.vx = pP_ptr->Pos_x;
					Sv.vy = pP_ptr->Pos_y + 500;
					Sv.vz = pP_ptr->Pos_z;
					pP_ptr = pEm->pSin_parts_ptr;
					Esp_call((Rnd() << 3) + 0xfd0, Rnd() << 4, &pP_ptr->Workm, &Sv);

				}
			}
		}

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		Add_speedXZ(pEm, (signed char)pEm->Timer2 << 11);

		pEm->Spd_x -= 50;
		if (pEm->Spd_x < 0)
			pEm->Spd_x = 0;

		if (!pEm->Spd_x) {

			Foot_set_pl(pEm, 0, 1);	}

		break;

	case 2:
		*(long *)&pEm->Routine_0 = 7;

	}
}

static void Em10_die05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_die05_tbl[3][3] = { { 55, 22, 1 }, { 40, 23, 1 }, { 0, 1, 1 } };

	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:

		FREE_US(2) &= 0xfffb;
		FREE_US(2) |= 4;

		if (FREE_UB(0x1c) == 1)
			FREE_US(2) &= 0xfffb;

		if (pEm->Move_cnt < Em10_die05_tbl[ FREE_UB(0x1c) ][0]) {
			*(long *)&pEm->Move_no = Em10_die05_tbl[ FREE_UB(0x1c) ][1] + 0x1f0000;
			pEm->Timer3 = 0;
		}else {
			FREE_US(2) &= 0xfffb;
			*(long *)&pEm->Move_no = Em10_die05_tbl[ FREE_UB(0x1c) ][2] + 0x1f1e00;
			pEm->Timer3 = 1;	}

		pEm->Routine_2 = 1;

		pEm->Neck_flg |= 1;

		pEm->Damage_cnt |= 0x80;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		Sv.vx = pP_ptr->Pos_x;
		Sv.vy = pP_ptr->Pos_y + 300;
		Sv.vz = pP_ptr->Pos_z;
		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x1fa0, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		if (Rnd() & 1) {
			Snd_se_enem(13, pEm);	}
		else {
			Snd_se_enem(11, pEm);
		}

		Em_dead_flg_on(pEm);

		if (pEm->Routine_1 == 10 && !(pEm->Type & 0x80))
			Em10_Firedm(pEm);

		if (pEm->Routine_1 == 11) {
			Em10_Ryudm(pEm);
			pP_ptr = &pEm->pSin_parts_ptr[8];
			pP_ptr->Be_flg |= 0x10;
			pP_ptr->Free[0] = 0;
			pP_ptr->Free[1] = 0;
			pP_ptr->Free[3] = 0x801e;
			((unsigned char *)pP_ptr->Free)[4] = 10;
			((unsigned char *)pP_ptr->Free)[5] = 10;
			pP_ptr->Poly_rgb = 0x304040;
			Esp_call(0x40f0fa0, pEm->Cdir_y, &pP_ptr->Workm, 0);
		}

		if ((pEm->Routine_1 == 9 || pEm->Routine_1 == 17) && !(pEm->Type & 0x80))	/* retail: damage type 17 also */
			Em10_Tudm(pEm);

		if (pEm->Routine_1 == 14)
			Em10_Spdm(pEm);

		pEm->Spd_x = 11;

	case 1:
		if (Joint_move(pEm, Kan, Seq, 0x80)) {
			if (!*(signed char *)&pEm->Timer3) {

				pEm->Routine_2 = 2;

				break;	}
			pEm->Routine_2 = 3;	}	break;
	case 2:
		Joint_move(pEm, Kan, Seq, 0x80);
		if (!pEm->Hokan_flg)

			pEm->Routine_2 = 3;
		break;

	case 3:
		*(long *)&pEm->Routine_0 = 7;

	}
}

static void Em10_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em10_sce[ pEm->Routine_1 ](pEm, Kan, Seq);
}

static void Em10_sce00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf0000;
		pEm->Routine_2 = 1;
	case 1:
		Joint_move(pEm, Kan, Seq, 0x100);
		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);
		}
	}
}

static void Em10_sce01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = FREE_UB(0) + 0xf0000;
		pEm->Routine_2 = 1;
	case 1:
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x24);
		Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
		Joint_move(pEm, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x100);

		Add_speedXZ(pEm, 0);
	}
}

static void Em10_sce02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pAt;

	pAt = G.pEnemy[ pEm->Sce_free3 ];

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0xf000a;
		pEm->Routine_2 = 1;
		Goto00(pEm, pAt->Pos_x, pAt->Pos_z, 0x400);

		pEm->Timer0 = 0;
		Snd_se_enem(10, pEm);
		pEm->Damage_cnt |= 0x80;
	case 1:
		if (++pEm->Timer0 & 1) {
			Spd_get(pEm, Kan, Seq, 0);
		}else
			pEm->Spd_x >>= 2;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);
		if (pEm->Move_cnt == 17)
			pEm->Routine_2 = 2;

		break;
	case 2:
		*(long *)&pEm->Move_no = 0xf0018;
		pEm->Routine_2 = 3;

		Set_base_pos(pAt, Kan, Seq, 0);
		*(long *)&pAt->Routine_0 = 0x2004;

		pAt->pEnemy_ptr = (unsigned long *)pEm;

		pAt->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
		pAt->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;

		pEm->Be_flg |= 0x1000;
		pAt->Be_flg |= 0x1000;

	case 3:
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;
	case 4:
		*(long *)&pEm->Routine_0 = 0x20308;
	}
}

static void Em10_sce03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	unsigned char Em10_mv0a_mot[8] = { 18, 19, 20, 18, 19, 20, 18, 19 };

	SVECTOR Sv;
	register ENEMY_WORK *pAt;
	register PARTS_W *pP_ptr;

	pAt = G.pEnemy[ pEm->Sce_free3 ];
	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_2 = 1;
		*(long *)&pEm->Move_no = ((Rnd() & 7) << 8) + 0xf0015;
		pEm->Damage_cnt |= 0x80;
	case 1:
		Goto00(pEm, pAt->Pos_x, pAt->Pos_z, 0x10);
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x10100);
		break;
	case 2:
		pEm->Routine_2 = 3;
		*(long *)&pEm->Move_no = Em10_mv0a_mot[ Rnd() & 7 ] + 0x70000;
	case 3:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);
		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);
		}

		if (*pEm->pNow_seq & 0x10000) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = 200;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x17b8, Rnd() << 4, &pP_ptr->Workm, &Sv);
		}
		break;
	case 4:
		*(long *)&pEm->Move_no = Em10_mv0a_mot[ Rnd() & 7 ] + 0x300;
		pEm->Routine_2 = 1;
	}
}

static void Em10_sce04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	unsigned char Em10_mv06_mot[2] = { 1, 2 };
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer2 = 1;

		FREE_US(2) &= 0xfffb;
		FREE_US(2) |= 4;
		*(long *)&pEm->Move_no = 0x30a02;
		pEm->Routine_2 = 1;
		pEm->Damage_cnt |= 0x80;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		Sv.vx = pP_ptr->Pos_x;
		Sv.vy = pP_ptr->Pos_y + 300;
		Sv.vz = pP_ptr->Pos_z;
		pP_ptr = pEm->pSin_parts_ptr;
		Esp_call(0x1f40, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		Sv.vy -= 400;
		Esp_call(0x8001b58, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		Esp_call(0x21f40, pEm->Cdir_y, &pP_ptr->Workm, &Sv);

		pEm->Neck_flg = 1;

		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg |= 0x4a;
		pP_ptr->Free[0] = 0;
		pP_ptr->Free[4] = 0;
		pP_ptr->Free[2] = 0;
		pP_ptr->Free[1] = -100;
		pP_ptr->Free[3] = 10;
		pP_ptr->Free[6] = 0;
		pP_ptr->Poly_rgb = 0x101040;

		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr->Be_flg |= 0x80;
		*(long *)&pP_ptr->Free[0] = 0x40;
		*(long *)&pP_ptr->Free[2] = 0x101010;

		Snd_se_enem(2, pEm);
		Em_dead_flg_on(pEm);
	case 1:
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		if (!pEm->Hokan_flg) {

			Foot_set_pl(pEm, 0, 1);	}

		break;
	case 2:
		*(long *)&pEm->Routine_0 = 7;

	}
}

static void Em10_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register KAGE_WORK *pKage;
	register PARTS_W *pP_ptr;
	register unsigned long i;

	pKage = (KAGE_WORK *)pEm->pKage_work;

	switch (pEm->Routine_1) {
	case 0:

		pEm->Neck_flg |= 1;

		*(long *)&pEm->Move_no = 0x70017;
		if (FREE_US(2) & 4)
			*(long *)&pEm->Move_no = 0x70016;

		pEm->Be_flg |= 2;
		pEm->Life = -1;

		pEm->Routine_1 = 1;
		*(unsigned long *)&pKage->Prim[0].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[0].r0 |= 0xbfbf10;
		*(unsigned long *)&pKage->Prim[1].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[1].r0 |= 0xbfbf10;

		pEm->Timer2 = 120;
		pEm->Timer0 = 40;

		FREE_UB(0x20) = 0;

		pEm->Spd_x = 0;

		Em_dead_flg_on(pEm);
		/* retail-only: the same part flags are set at once (retail 0x8010A528..0x8010A5A0) */
		pP_ptr = &pEm->pSin_parts_ptr[3];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[4];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[6];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[7];
		pP_ptr->Be_flg |= 0x8000;
		pP_ptr = &pEm->pSin_parts_ptr[8];
		pP_ptr->Be_flg |= 0x8000;

		if (Rnd() % 3) {
			pEm->Routine_1 = 8;
			return;	}

		pEm->Timer1 = 0;

	case 1:

		if (*(signed char *)&pEm->Timer2) {
			pEm->Timer2--;
			pKage->Half_x += 5;
			pKage->Half_z += 5;	}

		pEm->Routine_1 += Joint_move(pEm, Kan, Seq, 0x200);

		break;

	case 2:
		pEm->Timer3 = (Rnd() & 0xf) + 1;
		pEm->Routine_1 = 3;

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_1 = 4;
			pEm->Timer0 = 30;
			break;	}

	case 3:
		if (*(signed char *)&pEm->Timer2) {
			pEm->Timer2--;
			pKage->Half_x += 5;
			pKage->Half_z += 5;	}

		if ((unsigned char)pEm->Timer3-- == 0)
			pEm->Routine_1 = 1;

		break;

	case 4:
		if ((unsigned char)pEm->Timer3-- == 0)
			pEm->Routine_1 = 5;

		break;

	case 6:
		pEm->Timer3 = (Rnd() & 0x3f) + 60;
		pEm->Routine_1 = 4;

		if ((unsigned short)pEm->Timer0-- == 0) {
			pEm->Routine_1 = 7;

			/* retail: parts 3,4,6,7,8 only (Beta also set parts 2 and 5) */
			pP_ptr = &pEm->pSin_parts_ptr[3];
			pP_ptr->Be_flg |= 0x8000;
			pP_ptr = &pEm->pSin_parts_ptr[4];
			pP_ptr->Be_flg |= 0x8000;
			pP_ptr = &pEm->pSin_parts_ptr[6];
			pP_ptr->Be_flg |= 0x8000;
			pP_ptr = &pEm->pSin_parts_ptr[7];
			pP_ptr->Be_flg |= 0x8000;
			pP_ptr = &pEm->pSin_parts_ptr[8];
			pP_ptr->Be_flg |= 0x8000;	}

		break;

	case 8:

		if (*(signed char *)&pEm->Timer2) {
			pEm->Timer2--;
			pKage->Half_x += 5;
			pKage->Half_z += 5;	}

		if (!*(signed char *)&pEm->Timer2)
			pEm->Routine_1 = 7;

		break;

	case 9:
		*(long *)&pEm->Move_no = 0x70007;
		if (FREE_US(2) & 4)
			*(long *)&pEm->Move_no = 0x70016;

		pEm->Routine_1 = 10;

	case 5:	case 10:
		pEm->Routine_1 += Joint_move(pEm, Kan, Seq, 0x200);
		break;

	case 11:
		pEm->Routine_1 = 0;

	}

	pEm->Timer1++;

	if (pEm->Type & 0x80) {

		pP_ptr = pEm->pSin_parts_ptr;	i = 15;
		if (*(unsigned char *)&pP_ptr->Poly_rgb != 0x10 && !(pEm->Timer1 & 3)) {
			do {
				pP_ptr->Poly_rgb -= 0x10101;
				pP_ptr++;
			}while (--i);
		}
	}

	if (pEm->Water) {
		if (!(FREE_UB(0x20) & 0x80)) {
			pEm->Pos_y -= 5;
			if (pEm->Pos_y < pEm->Water + 300) {

				pEm->Pos_y = pEm->Water + 300;
				FREE_UB(0x20) = 0x80;	}

		}else {

			if (FREE_UB(0x20) < 0xa0) {
				pEm->Pos_y -= 2;
			}else {
				pEm->Pos_y += 2;
				if (FREE_UB(0x20) == 0xbf)
					FREE_UB(0x20) = 0x80;
			}
		}
		FREE_UB(0x20)++;
	}

}

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register PARTS_W *pP_ptr;

	Pldamage_Em10_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	if (pEm->Water) {
		pP_ptr = pEm->pSin_parts_ptr;
		if (pP_ptr->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 0xf) == 1) {

			Em10_water(pEm);
			/* retail: the Beta Snd_se_stad( 0x2020001, ... ) splash call is gone (retail 0x8010A99C) */

		}
	}

}

static void Em10_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pAi;
	register ENEMY_WORK *pT;
	SVECTOR Sv;
	register unsigned long i;
	register short Ck_flg;
	register PARTS_W *pP_ptr;

	unsigned char Em10_pldm00_mot[2] = { 0, 3 };

	short Em10_pldm00_blo[2][3] = { { 0, 200, 100 }, { -100, 100, -200 } };

	pAi = (ENEMY_WORK *)pEm->pEnemy_ptr;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = Em10_pldm00_mot[ pEm->Routine_1 ] + 0xf0000;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x800;

		pEm->Timer2 = 0;

		Goto00(pEm, pAi->Pos_x, pAi->Pos_z, 0x800);
		if (pEm->Routine_1 & 1) {
			pEm->Cdir_y += 0x800;
			pEm->Timer0 = 0;	}

		Em10_pldm00_hajiki[0] = 0xff;
		Em10_pldm00_hajiki[1] = 0xff;
		Em10_pldm00_hajiki[2] = 0xff;
		Em10_pldm00_hajiki[3] = 0xff;
		Em10_hajiki_no = 0;

		Snd_se_stad(((Rnd() & 1) << 17) | 0x4000001, (VECTOR *)&pEm->Pos_x);

		pEm->Neck_flg |= 0x12;

		pEm->Timer1 = 0;

	case 1:
		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 2:
		*(long *)&pEm->Move_no = Em10_pldm00_mot[ pEm->Routine_1 ] + 1;
		pEm->Routine_2 = 3;

	case 3:
		if (pEm->Move_cnt == 15) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][0];
			Sv.vy = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][1];
			Sv.vz = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][2];
			Esp_call(0x17d0, -0x100 + (pEm->Cdir_y + (Rnd() << 1)) + ((pEm->Routine_1 & 1) << 11), &pP_ptr->Workm, &Sv);

			pEm->Timer2 = 0;
		}

		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 4:
		*(long *)&pEm->Move_no = Em10_pldm00_mot[ pEm->Routine_1 ] + 0x70002;
		pEm->Routine_2 = 5;

		if (*(signed char *)&pEm->Timer2) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][0];
			Sv.vy = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][1];
			Sv.vz = Em10_pldm00_blo[ pEm->Routine_1 & 1 ][2];
			Esp_call(0x17d0, -0x100 + (pEm->Cdir_y + (Rnd() << 1)) + ((pEm->Routine_1 & 1) << 11), &pP_ptr->Workm, &Sv);

		}

	case 5:
		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		if ((unsigned)(pEm->Move_cnt - 9) < 5) {
			Ck_flg = pEm->At_em_flg;
			Ck_flg <<= 2;

			i = 2;	while (Ck_flg) {

				if ((Ck_flg & 0x8000) && !((pEm->Timer1 >> i) & 1)) {

					pT = G.pEnemy[i];
					if ((pT->Id & 0x10) && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt && !(FREE_US_P(pT,2) & 8) && !(pT->Type & 0x2000)) {

						*(long *)&pT->Routine_0 = 0x901;

						pT->Timer3 = ~Cdir_ck2(pEm, pT) & 1;
					}
				}
				i++;
				Ck_flg <<= 1;
			}

			pEm->Timer1 |= pEm->At_em_flg;
		}

		break;

	case 6:
		*(long *)&pEm->Routine_0 = 1;

		G.Player.Damage_cnt &= 0x7f;
		pEm->Neck_flg &= 0xed;

		pEm->Be_flg &= ~0x1004;
		pAi->Be_flg &= ~0x1004;

	}

}

static void Em10_pldm01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em10_pldm02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register ENEMY_WORK *pAi;
	SVECTOR Sv;
	register PARTS_W *pP_ptr;

	unsigned char Em10_pldm02_mot[2] = { 6, 10 };

	short Em10_pldm02_blo[2][3] = { { -400, -100, 0 }, { 200, -100, 0 } };

	pAi = (ENEMY_WORK *)pEm->pEnemy_ptr;
	pEm->Be_flg |= 4;	/* retail-only (retail 0x8010AFC4..0x8010AFD4) */

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = Em10_pldm02_mot[ pEm->Routine_1 - 2 ] + 0xf0000;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x800;

		Goto00(pEm, pAi->Pos_x, pAi->Pos_z, 0x800);
		if (pEm->Routine_1 & 1) {
			pEm->Cdir_y += 0x800;
			pEm->Timer0 = 0;	}

		Snd_se_stad(0x4000001, (VECTOR *)&pEm->Pos_x);

		pEm->Neck_flg |= 0x12;

	case 1:
		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 2:
		*(long *)&pEm->Move_no = Em10_pldm02_mot[ pEm->Routine_1 - 2 ] + 1;
		pEm->Routine_2 = 3;

	case 3:
		if (!pEm->Move_cnt) {

			pP_ptr = &pEm->pSin_parts_ptr[3];
			Sv.vx = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][0];
			Sv.vy = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][1];
			Sv.vz = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][2];
			Esp_call(0xfd0, -0x100 + (pEm->Cdir_y + (Rnd() << 1)) + ((pEm->Routine_1 & 1) << 11), &pP_ptr->Workm, &Sv);

		}

		if (pEm->Move_cnt == 3) {

			pP_ptr = &pEm->pSin_parts_ptr[3];
			Sv.vx = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][0];
			Sv.vy = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][1];
			Sv.vz = Em10_pldm02_blo[ pEm->Routine_1 & 1 ][2];
			Esp_call(0x17d0, -0x100 + (pEm->Cdir_y + (Rnd() << 1)) + ((pEm->Routine_1 & 1) << 11), &pP_ptr->Workm, &Sv);

		}

		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		Joint_move(pEm, Kan, Seq, 0x100);

		break;

	case 4:
		*(long *)&pEm->Move_no = Em10_pldm02_mot[ pEm->Routine_1 - 2 ] + 0x70002;
		pEm->Routine_2 = 5;

		goto damage_continue; { /* [INFERRED] skip case6 initialization on this path; direct case6 dispatch remains reachable. */

	case 6:
		*(long *)&pEm->Move_no = 9;
		pEm->Routine_2 = 7;

		if (pEm->Routine_1 & 1) {
			pEm->Cdir_y += 0x800;
			pEm->Timer0 = 0x800;	}
		}

	case 5:	case 7: damage_continue: switch (0) default: ; /* [INFERRED] common case continuation without an added debug label. */
		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x200);

		break;

	case 8:
		*(long *)&pEm->Routine_0 = 1;

		G.Player.Damage_cnt &= 0x7f;
		pEm->Neck_flg &= 0xed;

		pEm->Be_flg &= ~0x1004;
		pAi->Be_flg &= ~0x1004;

	}

}

void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register PARTS_W *pP_ptr;

	Pldie_Em10_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);

	if (pEm->Water) {
		pP_ptr = pEm->pSin_parts_ptr;
		if (pP_ptr->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 0xf) == 1) {

			Em10_water(pEm);

			Snd_se_stad(0x2020001, (VECTOR *)&pEm->Pos_x);
		}
	}

}

static void Em10_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register PARTS_W *pP_ptr;
	SVECTOR Sv;
	register unsigned char Ofs;	/* retail-only local, [INFERRED] name: Dual Shock strength offset */

	unsigned char Em10_pldie00_mot[2] = { 13, 15 };

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = Em10_pldie00_mot[ pEm->Routine_1 ] + 0xf0000;
		pEm->Routine_2 = 1;
		pEm->Timer0 = 0x800;

		if (pEm->Routine_1 & 1)
			pEm->Timer0 = 0;

		pEm->Neck_flg |= 1;

		Snd_se_stad(0x4030001, (VECTOR *)&pEm->Pos_x);

		pEm->Neck_flg |= 0x12;

		/* retail-only (Dual Shock): vibration pattern for the bite (retail 0x8010B518..0x8010B5C8) */
		Ofs = (G.System_flg & 0x800) ? 15 : 0;
		Pad_vib_fade_set(0x21, 0x32, 0x96, 0);
		Pad_vib_set1(8, 0xfa, 0x21 - Ofs);
		Pad_vib_set0(0x32, 0x41 - Ofs);
		Pad_vib_set1(6, 0xfa, 0x4b - Ofs);
		Pad_vib_set1(6, 0xfa, 0x56 - Ofs);
		Pad_vib_set1(6, 0xfa, 0x73 - Ofs);
		Pad_vib_fade_set(0x5a, 0xfa, 0, 0x79 - Ofs);

	case 1:
		pEm->Cdir_y += pEm->Timer0;
		Null_pos_set(pEm, Kan, Seq, 0);

		pEm->Cdir_y -= pEm->Timer0;

		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);

		if ((unsigned)(pEm->Move_cnt - 41) < 58) {
			if (!(pEm->Move_cnt & 1)) {

				pP_ptr = &pEm->pSin_parts_ptr[8];
				Sv.vx = 0;
				Sv.vy = 0;
				Sv.vz = 0;
				Esp_call((Rnd() << 3) + 0xbd0, Rnd() << 4, &pP_ptr->Workm, &Sv);

			}
		}

		if ((unsigned)(pEm->Move_cnt - 100) < 5) {

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = 0;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x17d0, Rnd() << 4, &pP_ptr->Workm, &Sv);

			pP_ptr = &pEm->pSin_parts_ptr[8];
			Sv.vx = 0;
			Sv.vy = 0;
			Sv.vz = 0;
			Esp_call(0x1bd0, Rnd() << 4, &pP_ptr->Workm, &Sv);

			pP_ptr = &pEm->pSin_parts_ptr[8];
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x40;
			*(long *)&pP_ptr->Free[2] = 0x202030;

			pP_ptr = pEm->pSin_parts_ptr;
			pP_ptr->Be_flg |= 0x80;
			*(long *)&pP_ptr->Free[0] = 0x80;
			*(long *)&pP_ptr->Free[2] = 0x101030;
		}

		break;

	case 2:
		pEm->Routine_2 = 3;
		pEm->Move_no++;
		pEm->Move_cnt = 0;
		pEm->Life = -0x8000;

	case 3:
		pEm->Hokan_flg = 10;
		pEm->Routine_2 += Joint_move(pEm, Kan, Seq, 0x100);
		break;

	case 4:
		pEm->Routine_2 = 5;
		pEm->Timer3 = (Rnd() & 0x1f) + 3;

	case 5:
		if ((unsigned char)pEm->Timer3-- == 0)
			pEm->Routine_2 = 3;

		break;

	case 6:
		*(long *)&pEm->Routine_0 = 7;

	}

}

static void Em10_Kahannsin_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	JOINT_P Em10_Jp0 = { 0, 0, 0, 0, 0x7e };
	register short Tmp;
	SVECTOR Spd;
	SVECTOR Sv;
	MATRIX M;
	register KAGE_WORK *pKage;
	register PARTS_W *pP_ptr;

	if (!(FREE_US(2) & 1))
		return;

	switch (FREE_UB(1)) {
	case 0:
		FREE_UB(4) = FREE_UB(0);
		FREE_UB(5) = 0;
		FREE_UB(6) = 7;

		pP_ptr = &pEm->pSin_parts_ptr[1];
		pEm->Super_matrix.t[0] = pP_ptr->Workm.t[0];
		pEm->Super_matrix.t[2] = pP_ptr->Workm.t[2];

		FREE_UB(1) = 1;

		FREE_US(8) = pEm->Cdir_y;

		FREE_UB(7) = (Rnd() & 0x1f) + 10;

		Kage_work_set(&OVL_FREE(pEm)[3], 0, 0x2bc01f4, 0, (VECTOR *)pEm->Super_matrix.t);

	case 1:

		Em10_Jp0.Move_no = FREE_UB(4);
		Em10_Jp0.Move_cnt = FREE_UB(5);
		Em10_Jp0.Hokan_flg = FREE_UB(6);

		Joint_move_p(&Em10_Jp0, (unsigned long)pEm->pKan_t_ptr, (unsigned long)pEm->pSeq_t_ptr, 0x200);

		FREE_UB(4) = Em10_Jp0.Move_no;
		FREE_UB(5) = Em10_Jp0.Move_cnt;
		FREE_UB(6) = Em10_Jp0.Hokan_flg;

		Spd.vx = 0;
		Spd.vy = FREE_US(8);
		Spd.vz = 0;
		RotMatrix(&Spd, &M);
		Spd.vx = (Rnd() & 7) + 10;
		Spd.vy = 0;
		ApplyMatrixSV(&M, &Spd, &Sv);

		pEm->Super_matrix.t[0] += Sv.vx;
		pEm->Super_matrix.t[2] += Sv.vz;

		if ((signed char)FREE_UB(7)-- == 0)
			FREE_UB(1) = 2;

		break;

	case 2:
		FREE_UB(4) = 1;
		FREE_UB(5) = 0;
		FREE_UB(6) = 7;
		FREE_UB(1) = 3;

	case 3:
		Em10_Jp0.Move_no = FREE_UB(4);
		Em10_Jp0.Move_cnt = FREE_UB(5);
		Em10_Jp0.Hokan_flg = FREE_UB(6);

		FREE_UB(1) += Joint_move_p(&Em10_Jp0, Kan, Seq, 0x200);

		FREE_UB(4) = Em10_Jp0.Move_no;
		FREE_UB(5) = Em10_Jp0.Move_cnt;
		FREE_UB(6) = Em10_Jp0.Hokan_flg;

		Spd.vx = 0;
		Spd.vy = FREE_US(8);
		Spd.vz = 0;
		RotMatrix(&Spd, &M);
		Spd.vx = 30;
		Spd.vy = 0;
		ApplyMatrixSV(&M, &Spd, &Sv);

		pEm->Super_matrix.t[0] += Sv.vx;
		pEm->Super_matrix.t[2] += Sv.vz;

		break;

	case 4:
		pP_ptr = &pEm->pSin_parts_ptr[1];
		pP_ptr->Be_flg |= 0x40;
		FREE_UB(1) = 5;

		FREE_UB(4) = 22;
		FREE_UB(5) = 0;
		FREE_UB(6) = 7;

		FREE_UB(7) = (Rnd() & 0x1f) + 10;

		pKage = (KAGE_WORK *)OVL_FREE(pEm)[3];
		*(unsigned long *)&pKage->Prim[0].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[0].r0 |= 0xbfbf10;
		*(unsigned long *)&pKage->Prim[1].r0 &= 0xff000000;
		*(unsigned long *)&pKage->Prim[1].r0 |= 0xbfbf10;

		if (Rnd() & 1) {
			FREE_UB(1) = 8;
			return;	}

	case 5:
		Em10_Jp0.Move_no = FREE_UB(4);
		Em10_Jp0.Move_cnt = FREE_UB(5);
		Em10_Jp0.Hokan_flg = FREE_UB(6);

		FREE_UB(1) += Joint_move_p(&Em10_Jp0, Kan, Seq, 0x200);

		FREE_UB(4) = Em10_Jp0.Move_no;
		FREE_UB(5) = Em10_Jp0.Move_cnt;
		FREE_UB(6) = Em10_Jp0.Hokan_flg;

		break;

	case 6:
		FREE_UB(7) = (Rnd() & 0x1f) + 60;
		FREE_UB(1) = 7;

		if (!(Rnd() & 0xf)) {
			FREE_UB(1) = 8;
			return;	}

	case 7:

		Em10_Jp0.Move_no = FREE_UB(4);
		Em10_Jp0.Move_cnt = 0;
		Em10_Jp0.Hokan_flg = 0;
		Joint_move_p(&Em10_Jp0, Kan, Seq, 0x200);
		if ((signed char)FREE_UB(7)-- == 0)
			FREE_UB(1) = 5;

		break;

	case 8:

		Em10_Jp0.Move_no = FREE_UB(4);
		Em10_Jp0.Move_cnt = 0;
		Em10_Jp0.Hokan_flg = 0;
		Joint_move_p(&Em10_Jp0, Kan, Seq, 0x200);

	}

	if (FREE_UB(1) < 4) {
		;
		Tmp = pEm->Cdir_y;
		pEm->Cdir_y = FREE_US(8);
		RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
		pEm->Cdir_y = Tmp;
		CompM((MATRIX *)pEm->M, &Em10_Jp0.P0_m, &M);
		*(long *)&pEm->Super_matrix.m[0][0] = *(long *)&M.m[0][0];
		*(long *)&pEm->Super_matrix.m[0][2] = *(long *)&M.m[0][2];
		*(long *)&pEm->Super_matrix.m[1][1] = *(long *)&M.m[1][1];
		*(long *)&pEm->Super_matrix.m[2][0] = *(long *)&M.m[2][0];
		*(long *)&pEm->Super_matrix.m[2][2] = *(long *)&M.m[2][2];

		pEm->Super_matrix.t[1] = Em10_Jp0.P0_m.t[1] + FREE_SS(0x1a);
	}

	MulKage((KAGE_WORK *)OVL_FREE(pEm)[3], pEm->Ground, FREE_SS(8), (pEm->Be_flg & 0x400) >> 9);

}

static void Em10_Kubigoro(ENEMY_WORK *pEm)
{
	MATRIX M;

	SVECTOR TMP_SV = { 300, -300, 0 };
	register short Ground;
	VECTOR Tv0;
	register PARTS_W *pP_ptr;
	register long Tmp;

	if (!(FREE_US(2) & 0x180))
		return;

	pP_ptr = &pEm->pSin_parts_ptr[8];

	if (FREE_US(2) & 0x80) {
		pEm->Neck_flg = 1;
		pP_ptr->Be_flg |= 0x8040;
		FREE_US(2) &= 0xff7f;
		FREE_US(2) |= 0x100;

		M = GsIDMATRIX;
		RotMatrixY(pEm->Cdir_y + 0x800, &M);
		ApplyMatrixSV(&M, &TMP_SV, (SVECTOR *)&pP_ptr->Old_x);
		pP_ptr->Timer1 = 2;	}

	M = GsIDMATRIX;
	RotMatrixZ(0x80, &M);
	RotMatrixX(0x40, &M);
	CompM(&pP_ptr->Workm, &M, &pP_ptr->Workm);
	Tv0 = *(VECTOR *)pP_ptr->Workm.t;
	pP_ptr->Workm.t[0] += pP_ptr->Old_x;
	pP_ptr->Workm.t[1] += pP_ptr->Old_y;
	pP_ptr->Workm.t[2] += pP_ptr->Old_z;

	Ground = Sca_get_ground(&Tv0, 100, 0x2000, 1);

	if (Sca_ck_hit((VECTOR *)pP_ptr->Workm.t, 100, 1 << (pP_ptr->Workm.t[1] / -1800), 0x2000)) {

		*(VECTOR *)pP_ptr->Workm.t = Tv0;
		Ground = Sca_get_ground((VECTOR *)pP_ptr->Workm.t, 100, 0x2000, 1);

		pP_ptr->Old_x = pP_ptr->Old_x * -1;
		pP_ptr->Old_z = pP_ptr->Old_z * -1;
		Esp_call(1000, Rnd() << 4, &pP_ptr->Workm, 0);

		/* retail-only: the head is removed after its last bounce or when it is still inside
		 * a wall (retail 0x8010BF8C..0x8010BFF8) */
		if ((unsigned char)pP_ptr->Timer1-- == 0) {
			FREE_US(2) &= 0xfeff;
			pP_ptr->Be_flg = 0;
			Esp_kill(5, 3, &pP_ptr->Workm);
			return;	}

		if (Sca_ck_hit((VECTOR *)pP_ptr->Workm.t, 100, 1 << (pP_ptr->Workm.t[1] / -1800), 0x2000)) {
			FREE_US(2) &= 0xfeff;
			pP_ptr->Be_flg = 0;
			Esp_kill(5, 3, &pP_ptr->Workm);
			return;	}
	}

	pP_ptr->Old_y += 50;
	if (Ground - 200 < pP_ptr->Workm.t[1]) {
		pP_ptr->Workm.t[1] = Ground - 200;
		pP_ptr->Old_x >>= 1;
		pP_ptr->Old_z >>= 1;
		Tmp = pP_ptr->Old_y;
		pP_ptr->Old_y = -(Tmp >> 2);
		Tv0.vx = Tv0.vy = Tv0.vz = 0;
		Esp_call(1000, Rnd() << 4, &pP_ptr->Workm, 0);

		Snd_se_enem(7, pEm);

		if ((unsigned char)pP_ptr->Timer1-- == 0) {
			FREE_US(2) &= 0xfeff;
			pP_ptr->Be_flg = 0;
			Esp_kill(5, 3, &pP_ptr->Workm);	}	/* retail-only Esp_kill */

	}

}

static void Em10_water(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	SVECTOR Sv1;
	register PARTS_W *pP_ptr;
	pP_ptr = pEm->pSin_parts_ptr;
	CompM((MATRIX *)pEm->M, (MATRIX *)pP_ptr->M, &pP_ptr->Workm);

	Sv.vx = pP_ptr->Workm.t[0];
	Sv.vy = pEm->Water;
	Sv.vz = pP_ptr->Workm.t[2];

	Sv1.vx = 300;
	Sv1.vy = 0;
	Sv1.vz = 0;
	ApplyMatrixSV((MATRIX *)pEm->M, &Sv1, &Sv1);
	Sv1.vx += Sv.vx;
	Sv1.vy += Sv.vy;
	Sv1.vz += Sv.vz;

	Esp_call(((Rnd() << 2) + 0xc00) | 0x1a000000, pEm->Cdir_y - (Rnd() << 2), &GsIDMATRIX, &Sv1);

	Esp_call(((Rnd() << 2) + 0xc00) | 0x1a000000, 0x400 + pEm->Cdir_y - (Rnd() << 2), &GsIDMATRIX, &Sv1);

	Esp_call(((Rnd() << 2) + 0xc00) | 0x1a000000, 0x800 + pEm->Cdir_y - (Rnd() << 2), &GsIDMATRIX, &Sv);

	Esp_call(((Rnd() << 2) + 0xc00) | 0x1a000000, 0xc00 + pEm->Cdir_y - (Rnd() << 2), &GsIDMATRIX, &Sv);

}

/* retail: the Beta body was dead code behind an early return; retail fills it in (retail
 * 0x8010C2A4 / 0x801192A4, 215 words): an arm (parts 9..11 or 12..14, chosen by Free[0x25])
 * is torn off, carried on the player's matrix for 60 frames and then dropped. */
static void Em10_ude_down(ENEMY_WORK *pEm)
{
	SVECTOR Sv;
	MATRIX M;
	register PARTS_W *pP_ptr;

	pP_ptr = &pEm->pSin_parts_ptr[ (FREE_UB(0x25) >> 1) * 3 + 9 ];

	switch (FREE_UB(0x24)) {
	case 1:
		*(long *)&pP_ptr->Free[0] = 0x30;
		*(long *)&pP_ptr->Free[2] = 0x101020;
		*(long *)&pP_ptr->Free[0] = 0x30;
		*(long *)&pP_ptr->Free[2] = 0x101020;
		pP_ptr->Be_flg |= 0x9280;
		pP_ptr[1].Be_flg |= 0x9200;
		pP_ptr[2].Be_flg |= 0x9200;
		pP_ptr->Be_flg |= 0x80;
		pP_ptr->pSuper = (unsigned long *)&G.Player.pSin_parts_ptr->Workm;
		pP_ptr->pOya_parts = (unsigned long *)G.Player.pSin_parts_ptr;
		MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP_ptr->Workm, (MATRIX *)pP_ptr->M);
		MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP_ptr[2].Workm, &M);
		FREE_SS(0x28) = M.t[0] >> 1;
		FREE_SS(0x2a) = M.t[1] + 200;
		FREE_SS(0x2c) = M.t[2] >> 1;
		FREE_UB(0x24) = 2;
		FREE_UB(0x23) = 60;
		Snd_se_enem(9, pEm);
		if (SquareRoot0(FREE_SS(0x28) * FREE_SS(0x28) + FREE_SS(0x2c) * FREE_SS(0x2c)) > 800) {
			FREE_UB(0x24) = 3;
			break;	}

	case 2:
		if ((signed char)FREE_UB(0x23)-- == 0)	/* [INFERRED] cast form: retail decrements by +255 and tests the old byte */
			FREE_UB(0x24) = 3;
		MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP_ptr[2].Workm, &M);
		pP_ptr->Pos_x += FREE_SS(0x28) - M.t[0];
		pP_ptr->Pos_y += FREE_SS(0x2a) - M.t[1];
		pP_ptr->Pos_z += FREE_SS(0x2c) - M.t[2];
		Sv.vx = 0;
		Sv.vy = 0;
		Sv.vz = (Rnd() & 0xf) * (1 - ((FREE_UB(0x23) & 1) << 1)) - 8;
		RotMatrix(&Sv, &M);
		MulMatrix((MATRIX *)pP_ptr->M, &M);
		if (!(FREE_UB(0x23) & 3)) {
			Esp_call(((Rnd() & 1) << 16) | (Rnd() + 0xc18), pEm->Cdir_y, &pP_ptr->Workm, 0);
			Sv.vx = 0;
			Sv.vy = -500;
			Sv.vz = (FREE_UB(0x25) >> 1) * 500 - 250;
			pP_ptr = pEm->pSin_parts_ptr;
			Esp_call((Rnd() & 1) | 0xc18, pEm->Cdir_y, &pP_ptr->Workm, &Sv);
		}
		break;

	case 3:
		pP_ptr->Be_flg |= 0x1062;
		pP_ptr->Old_x = pEm->Cdir_y + 0x800;
		pP_ptr->Timer3 = 0;
		pP_ptr->Old_z = 0;
		pP_ptr->Old_y = -20;
		pP_ptr->Timer1 = 30;
		pP_ptr->Old_x2 = 0;
		pP_ptr->Old_y2 = G.Player.Cdir_y + ((~FREE_UB(0x25) & 1) << 11);
		pP_ptr->Old_z2 = -0x400;
		FREE_UB(0x24) = 4;
		break;

	case 0:	case 4:	/* retail jump table has 5 entries (0..4) */
		break;
	}
}

