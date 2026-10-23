/* em22.c - reconstruction of D:\Bio2\prog\EM22.C (enemy 22: licker).
 * Linked into two overlays from the same source: 0x08 (EM22 at 0x8010024c) and
 * 0x26 (at 0x8010d24c); only the enemy-number-specific entry names differ
 * (Em122/Em222, Em122_Pldm/Em222_Pldm), selected by OVL_26.
 * Retail EM22.C #includes EM22_SUB.C ahead of its own functions: one object, one
 * SYM type preamble (em22.h), one data block, two SLD source files.
 * Data layout: the life tables, then EM22_SUB.C's tables, then the type sets are
 * defined at the top; Em22_mv07's local static Ck_pos is emitted with that
 * function, so the hit-length/blood tables and the routine tables that follow it
 * in the retail data block are defined at the end of the file (declared here).
 * statement lines follow the retail SLD by layout.
 * Retail (Dual Shock USA) copy: function definitions follow the retail text order
 * (Em22_Wall_escape..func_80102C40 precede Em122; evidenced by the text and by the
 * order of the functions' .rdata items); retail-only functions keep func_<slot-1 address>
 * names; changes against Beta 2 are commented "retail".
 * (retail ships no SLD). */
#define EM22_C	/* em22_sub.c is #included below */
#include "EM22.H"

/* ---- objects defined in other translation units ---- */
extern MATRIX GsIDMATRIX;

/* ---- objects of this file ---- */
static SVECTOR Add_dir;
static unsigned long Tongue_flg;
static void (*Em22_routine_0[8])();
static void (*Em22_mv[24])();
static void (*Em22_dm[8])();
static void (*Em22_die[6])();
static void (*Em22_pldm_tbl[])();

static unsigned short Em22_life[16] = { 87, 101, 81, 75, 110, 115, 99, 101, 101, 99, 81, 101, 115, 110, 87, 101 };	/* retail values (0x8010721C) */
static unsigned short Em22_life_easy[16] = { 65, 65, 70, 70, 75, 69, 62, 90, 80, 75, 82, 97, 89, 101, 90, 100 };
static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life = 250;	/* retail values (0x80107288) */
static unsigned short Em2b_life_easy = 200;
/* retail-only life tables (names unknown): Em22_life / Em22_life_easy + 10, used by Em22_Init when G.System_flg & 0x40 */
static unsigned short D_8010728C[16] = { 97, 111, 91, 85, 120, 125, 109, 111, 111, 109, 91, 111, 125, 120, 97, 111 };
static unsigned short D_801072AC[16] = { 75, 75, 80, 80, 85, 79, 72, 100, 90, 85, 92, 107, 99, 111, 100, 110 };
/* retail-only companions of the Em23/Em2a/Em2b life values above, same layout (names unknown;
 * EM22.C does not read them) */
static unsigned short D_801072CC = 240;
static unsigned short D_801072CE = 240;
static unsigned short D_801072D0[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short D_801072E4[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short D_801072F8 = 250;
static unsigned short D_801072FA = 200;

#include "EM22_SUB.C"

static unsigned short Tongue_hit_l[9] = { 2500, 3500, 4600, 4500, 4200, 3500, 3000, 2500, 2000 };	/* retail data order: before the type sets (0x8010750C) */
static unsigned char Normal_type_set[4] = { 0, 7, 11, 23 };	/* retail: entry 3 (Em22_mv[23], func_80102C40) added */
static unsigned char Scenario_type_set[4] = { 16, 17, 18, 22 };
static WEP_BLOOD Wep_blood_tbl[20];

static unsigned long Em22_Wall_escape(ENEMY_WORK *pEm, short Spd, unsigned long Flg)
{
	SVECTOR Ck_pos = { 1800, 0, 0, 0 };
	VECTOR Vec;

	Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);

	Vec.vx = pEm->Pos_x + Ck_pos.vx;
	Vec.vz = pEm->Pos_z + Ck_pos.vz;
	Sca_ck_hit(&Vec, 400, 1 << pEm->nFloor, 0x400);

	if (G.pSca_hit_data[0]) {
		register long Dir, Cdir;

		if (EF->Wall_at == 0) {

			Dir = Sca_get_dir((VECTOR *)&pEm->Pos_x, G.pSca_hit_data[0]);
			Dir = ((Dir << 10) + 0x800) & 0xFFF;
			Cdir = (pEm->Cdir_y & 0xFFF) - Dir;
			if (abs(Cdir) > 0x800) Cdir -= 0x1000;
			if (Cdir) {
				EF->Wall_at = Spd;
			}else
				EF->Wall_at = -Spd;

			if (Flg) EF->Wall_at = -EF->Wall_at;
		}
		pEm->Cdir_y += (signed char)EF->Wall_at;
		return 1;
	}
	EF->Wall_at = 0;
	return 0;
}
/* retail-only (retail 0x80101790 / 0x8010E790, 12 words; no Beta twin, name unknown):
 * sets bit 0x8000 in Be_flg of all 22 parts. Called once from Em22_Die when the death
 * sequence starts. */
static void func_80101790(ENEMY_WORK *pEm)
{
	register unsigned long i = 0;
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	for (; i < 22; i++, pParts++)
		pParts->Be_flg |= 0x8000;
}
static void Em22_no_br(ENEMY_WORK *pEm) {}
static void Em22_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	EF->Gun_area = 4;

	if (EF->pTarget = Em22_search_lv0(pEm)) {

		switch (EF->Sence_lv) {
		case 1:
			*(unsigned long *)&pEm->Routine_0 = 0xE01;
			break;

		case 2:

			*(unsigned long *)&pEm->Routine_0 = 0xF01;
			return;
		}
	}else
		switch (pEm->Routine_2) {
		case 0:
		case 3:
			EF->Wall_at = 0;
			*(unsigned long *)&pEm->Move_no = 0x2F0003;
			EF->Attribute |= 2;
			pEm->Routine_3++;
			if (pEm->Routine_3 >= 2) {
				*(unsigned short *)&pEm->Routine_2 = 4;
				goto walk;
			}
			pEm->Routine_2 = 1;
		case 1:
			Spd_get(pEm, Kan, Seq, 0);
			if (Joint_move2(pEm, Kan, Seq, 0x55)) pEm->Routine_2 = 3;
			Add_speedXZ(pEm, 0);
			break;

		case 2:
			pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x10400);
			break;
		walk:
			*(unsigned long *)&pEm->Move_no = 0x1F0001;

		case 4:
			Em22_Wall_escape(pEm, 0xC, 0);

			Spd_get(pEm, Kan, Seq, 0);
			if (Joint_move2(pEm, Kan, Seq, 0x80)) {
				if (!(Rnd() & 3)) *(unsigned short *)&pEm->Routine_2 = 0;
			}
			Add_speedXYZsuper(pEm, 0, 0);
		}
}
static void Em22_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0002;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		pEm->Routine_2 = 1;
	case 1:
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x60);
		Spd_get(pEm, Kan, Seq, 0);
		pEm->Spd_x /= 4;
		pEm->Spd_z /= 4;
		pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x100);
		Add_speedXYZsuper(pEm, 0, 0);
		break;

	case 2:
		*(unsigned long *)&pEm->Move_no = 0xF0004;
		*(unsigned short *)&pEm->Routine_2 = 3;
	case 3:
		pEm->Routine_3++;
		Joint_move2(pEm, Kan, Seq, 0x100);
		if (pEm->Routine_3 >= 5)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
static void Em22_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0006;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:
		if (EF->pTarget) {
			Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x20);

		}
		if (Joint_move2(pEm, Kan, Seq, 0x10100)) {
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			*(unsigned long *)&pEm->Routine_0 = 0x501;
		}
	}
}
static void Em22_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x100))
			*(unsigned long *)&pEm->Routine_0 = 1;

	}
}
static void Em22_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Add_dir_y;

	switch (pEm->Routine_2) {
	case 0:
		if (!EF->pTarget) {
			*(unsigned long *)&pEm->Move_no = 0xF0004;
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			pEm->Routine_2 = 1;
		}else {
			*(unsigned long *)&pEm->Move_no = 0xF0002;
			EF->Attribute &= ~2;
			EF->Gun_area = 3;
			pEm->Routine_2 = 2;
			goto rot;
		}

	case 1:
		EF->pTarget = Em22_search_lv0(pEm);
		pEm->Routine_3++;

		Joint_move2(pEm, Kan, Seq, 0x100);

		if (EF->pTarget) {
			*(unsigned long *)&pEm->Move_no = 0xF0002;
			pEm->Routine_2 = 2;
		}else {

			if (pEm->Routine_3 >= 0x79) {
				*(unsigned long *)&pEm->Routine_0 = 0x401;
				*(unsigned long *)&pEm->Move_no = 0xF0002;
			}

			break;
		}
	case 2:
	rot:
		if (!Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x200)) {

			if (SquareRoot0((EF->pTarget->Pos_x - pEm->Pos_x) * (EF->pTarget->Pos_x - pEm->Pos_x) + (EF->pTarget->Pos_z - pEm->Pos_z) * (EF->pTarget->Pos_z - pEm->Pos_z)) < 4000) {
				Add_dir_y = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);

			}else {
				Add_dir_y = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);

				pEm->Routine_3++;
			}
			pEm->Cdir_y += Add_dir_y;
			pEm->Spd_x = -40;
			pEm->Spd_z = 0;
			Add_speedXZ(pEm, 0);

		}else {

			*(unsigned long *)&pEm->Routine_0 = 0x1301;
		}

		Joint_move2(pEm, Kan, Seq, 0x100);

		if (!Add_dir_y)
			*(unsigned long *)&pEm->Routine_0 = 0x201;

		if (pEm->Routine_3 > 0x20)
			*(unsigned long *)&pEm->Routine_0 = 0x501;

	}
}
static void Em22_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0001;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
	case 1:

		if (!EF->At_cnt && pEm->L_pl < 2000) {
			if (Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x300))

				*(unsigned long *)&pEm->Routine_0 = 0x201;

			else
				*(unsigned long *)&pEm->Routine_0 = 0x901;	break;
		}

		if (pEm->L_pl > 4000) {	/* retail: random choice by the target's life (was 0x1401 only) */
			if (EF->pTarget->Life >= 100) {
				if ((Rnd() & 7) < 3)
					*(unsigned long *)&pEm->Routine_0 = 0xA01;
				else if (Rnd() & 2)
					*(unsigned long *)&pEm->Routine_0 = 0x1401;
				else
					*(unsigned long *)&pEm->Routine_0 = 0x801;
			}
			if (EF->pTarget->Life < 100) {
				if ((Rnd() & 7) < 2)
					*(unsigned long *)&pEm->Routine_0 = 0xA01;
				else
					*(unsigned long *)&pEm->Routine_0 = 0x1401;
			}
		}

		Spd_get(pEm, Kan, Seq, 1);

		Add_speedXZ(pEm, 0);

		Joint_move2(pEm, Kan, Seq, 0x10100);
		if (pEm->Move_cnt == 0x2E)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
static void Em22_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Hit;

	SVECTOR Tmp_Svec = { 200, 0, 0, 0 };
	register PARTS_W *pParts = &pEm->pSin_parts_ptr[9];

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_3 = 0;
		if (!(EF->Attribute & 2))
			pEm->Routine_2++;
		else {
			Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);
			pEm->Routine_2 += Mot_play2(pEm, 6, 0xF, 1);
			break;
		}
	case 1:
		if (Tongue_flg) {	*(unsigned long *)&pEm->Routine_0 = 0xF01;	break;	}
		Tongue_flg = pEm->Work_no;
		EF->Tongue_Rno = 1;	/* retail: tongue started here; Wall_dir_new = 0 dropped (the hit flag is Timer0 now) */
		*(unsigned long *)&pEm->Move_no = 0x30007;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		pEm->Timer0 = 0;
		*(unsigned short *)&pEm->Routine_2 = 2;

	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 8);
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			pEm->Routine_2++;

		Add_speedXZ(pEm, 0);
		if (pEm->Move_cnt == 0x11) {
			Esp_call(0x141800, 0, &pParts->Workm, &Tmp_Svec);
			Snd_se_enem(5, pEm);
		}
		break;
	case 3:
		if (!EF->Tongue_Rno) *(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

	pEm->Routine_3++;
	if (pEm->Routine_3 >= 18 && pEm->Routine_3 < 26 && !pEm->Timer0) {	/* retail: was !EF->Wall_dir_new */

		if (!EF->pTarget->Damage_cnt) {
			Hit = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x100);

			if (!Hit && pEm->L_pl < Tongue_hit_l[ pEm->Routine_3 - 17 ]) {

				EF->pTarget->pEnemy_ptr = pEm;
				*(unsigned long *)&EF->pTarget->Routine_0 = 2;
				EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);

				Pad_vib_set0(3, 0);	/* retail: pad vibration */
				Pad_vib_fade_set(4, 0xFF, 0x32, 2);
				Pad_vib_fade_set(0xA, 0x8C, 0x32, 2);
				Hit = Pl_life_down(25, 0);

				if (Hit & 2)
					*(unsigned long *)&G.Player.Routine_0 = 3;

				EF->At_cnt = 40;
				pEm->Timer0 = 1;	/* retail: was EF->Wall_dir_new = 1 */
			}
		}
	}
}
static void Em22_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	short Hit;

	register PARTS_W *pParts = &pEm->pSin_parts_ptr[13];

	switch (pEm->Routine_2) {
	case 0:
		pEm->Routine_3 = 0;
		if (EF->Attribute & 2) {
			pEm->Routine_2++;
		}else {
			pEm->Routine_2 += Mot_play2(pEm, 6, 0xF, 0);
			break;
		}
	case 1:
		*(unsigned long *)&pEm->Move_no = 0x30015;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2++;
		pEm->Timer0 = 0;
	case 2:
		Spd_get(pEm, Kan, Seq, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 8);
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		Add_speedXZ(pEm, 0);
		pEm->Routine_3++;

		if (!EF->pTarget->Damage_cnt && !pEm->Timer0)
		if (pEm->Routine_3 >= 2 && pEm->Routine_3 < 7) {
			Hit = Goto00_ck(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x154);
			if (!Hit && pEm->L_pl < 2800 && EF->pTarget->Life >= 0) {	/* retail: target must be alive */

				SVECTOR Bpos;

				EF->pTarget->pEnemy_ptr = pEm;
				*(unsigned long *)&EF->pTarget->Routine_0 = 2;
				EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);

				Bpos.vx = pParts->Workm.t[0];
				Bpos.vy = pParts->Workm.t[1] + 200;
				Bpos.vz = pParts->Workm.t[2];
				Esp_call(0x2400, 0, &GsIDMATRIX, &Bpos);
				Pad_vib_set0(5, 0);	/* retail: pad vibration */
				Pad_vib_fade_set(0xB, 200, 0x28, 3);

				Hit = Pl_life_down(15, 0);

				if (Hit & 2)
					*(unsigned long *)&G.Player.Routine_0 = 3;

				EF->At_cnt = 40;

				pEm->Timer0 = 1;
			}
		}

	}
}
static void Em22_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0x30506;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned short *)&pEm->Routine_2 = 1;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x400)) {
			*(unsigned long *)&pEm->Move_no = 0;
			*(unsigned short *)&pEm->Routine_2 = 2;
		}
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);
		break;
	case 2:
		if (pEm->L_pl < 2500)
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x40);

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {

			if (pEm->L_pl > 8000) {
				EF->Sence_lv = 1;
				*(unsigned long *)&pEm->Routine_0 = 1;
				break;
			}

			if (pEm->L_pl < 2000) {

				*(unsigned long *)&pEm->Routine_0 = 0xF01;

			}else {
				if (!(Rnd() & 3))

					*(unsigned long *)&pEm->Move_no = 0;
				*(unsigned short *)&pEm->Routine_2 = 2;

			}
		}
		Add_speedXYZsuper(pEm, 0, 0);
		break;
	case 3:
		pEm->Routine_3++;
		if (pEm->Routine_3 >= 6) {
			*(unsigned long *)&pEm->Move_no = 0;
			*(unsigned short *)&pEm->Routine_2 = 2;
		}

	}
}
static void Em22_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0004;
		EF->Attribute &= ~2;
		EF->Gun_area = 3;
		*(unsigned short *)&pEm->Routine_2 = 1;
		pEm->Timer0 = 0;
	case 1:
		if (Joint_move2(pEm, Kan, Seq, 0x100)) {
			*(unsigned long *)&pEm->Move_no = 0x30004;
			pEm->Routine_2++;
		}
		break;
	case 2:
		if (Joint_move2(pEm, Kan, Seq, 0x400))
			*(unsigned long *)&pEm->Routine_0 = 0x401;

	}

	pEm->Routine_3++;
	if (pEm->Routine_3 >= 11) {

		if (EF->pTarget = Em22_search_lv1(pEm))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

}
static void Em22_mv13(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned long *)&pEm->Move_no = 0xF0001;
		pEm->Routine_2++;
	case 1:
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x10);
		Spd_get(pEm, Kan, Seq, 0);
		Add_speedXZ(pEm, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x100))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;

		if (pEm->L_pl > 4000)
			*(unsigned long *)&pEm->Routine_0 = 0x1401;
		else

		if (!EF->At_cnt && pEm->L_pl < 2000)
			*(unsigned long *)&pEm->Routine_0 = 0x901;

	}
}
static void Em22_mv14(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_2) {
	case 0:
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		*(unsigned long *)&pEm->Move_no = 0xF001E;
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		EF->Wall_at = 0;
		pEm->Routine_2++;
	case 1:

		if (pEm->Sca_info & 1)
			pEm->Timer1++;
		else
			pEm->Timer1 = 0;

		if (pEm->Timer1 >= 6) {
			pEm->Timer0++;

			if (pEm->Timer1 > 20) {
				if (pEm->Timer1 == 21) EF->Wall_at = 0;
				Em22_Wall_escape(pEm, 50, 0);
			}else
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x32);

		}else {
			if (Rnd() & 2)
				Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x32);
			else
				Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x32);
		}

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move2(pEm, Kan, Seq, 0x100);
		Add_speedXZ(pEm, 0);

		if (++pEm->Timer0 >= 180)
			*(unsigned long *)&pEm->Routine_0 = 1;

		else
		if (pEm->L_pl < 2200) *(unsigned long *)&pEm->Routine_0 = 0xF01;

	}
}
/* retail-only (retail 0x80102C40 / 0x8010FC40, 156 words; no Beta twin, name unknown):
 * Em22_mv[23] (Routine_1 0x17), a scenario drop like Em22_mv10: wait on Sce_free0, drop,
 * re-orient against the floor, land. */
static void func_80102C40(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->pSca_dat = 0;
		pEm->Sce_free0 = 0;
		*(unsigned long *)&pEm->Move_no = 3;
		Joint_move2(pEm, Kan, Seq, 0x1000);
		EF->Gun_area = 0;
		pEm->Spd_y = 0;
		EF->Attribute |= 2;
		pEm->Type |= 0x4000;
		pEm->Routine_2++;
	case 1:
		if (pEm->Sce_free0) {
			EF->Gun_area = 5;
			*(unsigned long *)&pEm->Move_no = 0x12;
			pEm->Timer0 = -1;
			pEm->Type &= ~0x4000;
			pEm->Routine_2++;
		}
		break;
	case 2:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {
			*(unsigned long *)&pEm->Move_no = 0x13;
			pEm->Routine_2++;
		}
		break;
	case 3:
	{
		SVECTOR Ck_pos = { 2200, 0, 0, 0 };

		pEm->Pos_y += 0xA8C;
		Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);
		G.pEm->Cdir_y = (Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00;
		Em_init_super(pEm);
		pEm->Routine_2++;
	}
	case 4:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		*(unsigned long *)&pEm->Move_no = 0x13;
		Joint_move2(pEm, Kan, Seq, 0x1000);
		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			*(unsigned long *)&pEm->Move_no = 0x13;
			pEm->Routine_2++;
		}
		break;
	case 5:
		EF->Gun_area = 4;
		pEm->Pos_y = 0;
		if (Joint_move2(pEm, Kan, Seq, 0x1000))
			*(unsigned long *)&pEm->Routine_0 = 1;
	}
	pEm->Timer0++;
}
void EM_MAIN(ENEMY_WORK *pEm)
{

	register PARTS_W *pParts = (PARTS_W *)pEm->pSin_parts_ptr;
	VECTOR Vec;

	Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, 0, 0);

	if (G.Stop_flg & 0x20000000) return;

	*(unsigned long *)&Add_dir = 0;
	Add_dir.vz = 0;

	if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

	if (EF->At_cnt) EF->At_cnt--;

	Em22_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

	Gunarea_chg(pEm, EF->Gun_area);

	if (Add_dir.vx || Add_dir.vy || Add_dir.vz) {
		Rot_add_matrix(&pEm->Pos_x, &pEm->Super_vector, &pEm->Super_matrix, &Add_dir);

		pEm->Super_vector.vx = (pEm->Super_vector.vx + 0x40) & ~0x7F;
		pEm->Super_vector.vy = (pEm->Super_vector.vy + 0x40) & ~0x7F;
		pEm->Super_vector.vz = (pEm->Super_vector.vz + 0x40) & ~0x7F;
		RotMatrix(&pEm->Super_vector, (MATRIX *)pEm->pSuper);
	}

	Em22_tongue_move(pEm);

	if (!(EF->Attribute & 4)) Em_seq_sound(pEm);

	if (EF->Dm_eff_func) ((CODE_PTR)EF->Dm_eff_func)(pEm);

	{
		register ENEMY_WORK *pEm2, **ppEm;

		Vec.vx = pParts->Workm.t[0];
		Vec.vy = pParts->Workm.t[1];
		Vec.vz = pParts->Workm.t[2];

		Oba_set_ofs(pEm, &Vec);

		pEm->At_em_no = 0xFF;

		for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++) {

			pEm2 = *ppEm;
			if (pEm == pEm2) continue;

			if (pEm2->Be_flg & 1) {
				Oba_ck_hit(pEm2, pEm);
			}

		}	}
	{
		VECTOR Pos;

		SVECTOR Svec = { EF->Sca_ofs, 0, 0 };

		Rot_vector(pEm->Cdir_y, &Svec, &Svec);
		Pos.vx = pParts[1].Workm.t[0] + Svec.vx;
		Pos.vy = pParts[1].Workm.t[1] + Svec.vy;
		Pos.vz = pParts[1].Workm.t[2] + Svec.vz;

		Oba_set_ofs(pEm, &Pos);

		if (EF->Attribute & 0x100) {
			EF->Attribute &= ~0x100;
			pEm->Sca_old_x = pEm->Atd[0].X;
			pEm->Sca_old_z = pEm->Atd[0].Z;
		}else
			Sca_ck_em(pEm, 0x400);
	}

	Oba_set_ofs(pEm, &Vec);
}
static void Em22_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	PARTS_W *pP;

	pEm->Damage_flg = 0;
	pEm->Damage_cnt = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;

	G.pPldamage_Em[18] = EM_PLDM;

	*(unsigned long *)&pEm->Move_no = 1;
	Joint_move2(pEm, Kan, Seq, 0x100);

	pEm->Be_flg |= 0x2000000;

	pP = &pEm->pSin_parts_ptr[8];
	pEm->Neck_flg = 0;
	pEm->Neck_no = 8;
	pP->Free[0] = 0;
	pP->Free[1] = 0;
	pP->Free[2] = 0x60;
	pP->Free[3] = 0x60;
	pP->Free[4] = 0x200;
	pP->Free[5] = 0x138;

	pP = &pEm->pSin_parts_ptr[22];
	pP->Be_flg = 0;
	pP->pSuper = (unsigned long *)&pEm->pSin_parts_ptr[8].Workm;
	pP->pOya_parts = (unsigned long *)&pEm->pSin_parts_ptr[8];
	pP->Pos_x = 0;
	pP->Pos_y = 0;
	pP->Pos_z = 0;
	pP->Cdir_x = 0x400;
	pP->Cdir_y = 0;
	pP->Cdir_z = 0x400;
	RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);

	Tongue_flg = 0;

	if (G.System_flg & 0x40) {	/* retail: +10 life tables when System_flg bit 0x40, minus 20/10 once Common_flg 0xA4 is set */
		if (G.System_flg & 0x20) {
			pEm->Life = D_801072AC[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 20;
		}else {
			pEm->Life = D_8010728C[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 10;
		}
	}else {
		if (G.System_flg & 0x20) {
			pEm->Life = Em22_life_easy[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 20;
		}else {
			pEm->Life = Em22_life[ Rnd() & 0xF ];
			if (Flg_ck(G.SaveData.Common_flg, 0xA4)) pEm->Life -= 10;
		}
	}

	pEm->Super_vector.vx = pEm->Super_vector.vy = pEm->Super_vector.vz = 0;
	pEm->Super_matrix = GsIDMATRIX;

	EF->pTarget = (ENEMY_WORK *)&G.Player;

	pEm->Timer0 = pEm->Timer1 = 0;
	EF->pSca_dat = 0;
	EF->Sence_lv = 0;
	EF->Attribute = 0;
	EF->At_cnt = 0;
	EF->Tongue_Rno = 0;
	EF->Kage_Rno = 0;
	EF->Kage_x = 0x44C;
	EF->Kage_z = 0x352;
	EF->Sca_ofs = -500;
	EF->Dm_eff_rtn = 0;
	EF->Dm_eff_func = 0;

	pEm->Be_flg &= ~0xA;

	*(unsigned long *)&pEm->Routine_0 = 1;
	if (pEm->Type & 0x4000) {

		pEm->Routine_1 = Scenario_type_set[ pEm->Type & 0xF ];
	}else {

		pEm->Routine_1 = Normal_type_set[ pEm->Type & 0xF ];
	}

	Add_dir.vx = Add_dir.vy = Add_dir.vz = 0;
	switch ((pEm->Type >> 8) & 0xF) {
	case 0:

		Em22_Atd_set_1(pEm);	break;
	case 1:
		Add_dir.vz = 0x400;
		pEm->Timer3 = 0;
		pEm->Be_flg |= 8;
		Em22_Atd_set_2(pEm);
		break;
	case 2:
		Add_dir.vz = -0x400;
		pEm->Timer3 = 2;
		Em22_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 3:
		Add_dir.vx = -0x400;
		pEm->Timer3 = 3;
		Em22_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 4:
		Add_dir.vx = 0x400;
		pEm->Timer3 = 1;
		Em22_Atd_set_2(pEm);
		pEm->Be_flg |= 8;
		break;
	case 5:
		Add_dir.vx = 0x800;
		Add_dir.vy = 0x800;
		Em22_Atd_set_1(pEm);
		break;
	}

	pEm->Be_flg &= 0xE3FFFFFF;
	pEm->Be_flg |= 0xC000000;

	Kage_work_set((unsigned long *)&pEm->pKage_work, 0, EF->Kage_x | (EF->Kage_z << 16), 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);

	((KAGE_WORK *)pEm->pKage_work)->Off_x = 250;

	pP = &pEm->pSin_parts_ptr[0];	/* retail: seed the first two parts' positions */
	pP->Workm.t[0] = pEm->Pos_x;
	pP->Workm.t[1] = pEm->Pos_y;
	pP->Workm.t[2] = pEm->Pos_z;
	pP = &pEm->pSin_parts_ptr[1];
	pP->Workm.t[0] = pEm->Pos_x;
	pP->Workm.t[1] = pEm->Pos_y;
	pP->Workm.t[2] = pEm->Pos_z;

	pEm->pTbefore_func = Shadow_move;
}
static void Em22_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em22_mv[ pEm->Routine_1 ](pEm, Kan, Seq);	}
static void Em22_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Ck_pos = { 2200, 0, 0, 0 };
	VECTOR Vec;
	register SCA_DATA **ppSca_data;	register long Dir, Dis;
	register long Best_dis = 0x7FFFFFFF;
	register GLOBAL *pG = &G;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_no = 1;
		pEm->Routine_2++;
	case 1:
		if (EF->Attribute & 2)
			pEm->Routine_2++;
		else {
			pEm->Routine_2 += Mot_play2(pEm, 6, 0xF, 0);
			break;
		}
	case 2:
		EF->Attribute |= 2;
		EF->Gun_area = 4;

		Sca_search(0xF, 0x200, -1);

		ppSca_data = pG->pSca_hit_data;
		do {

			Dir = Sca_get_dir((VECTOR *)&pEm->Pos_x, *ppSca_data);
			switch (Dir) {
			case 0:
				Dis = abs(pEm->Pos_x - ((*ppSca_data)->X + (*ppSca_data)->W));
				break;
			case 1:
				Dis = abs((*ppSca_data)->Z - pEm->Pos_z);
				break;
			case 2:
				Dis = abs((*ppSca_data)->X - pEm->Pos_x);
				break;
			case 3:
				Dis = abs(pEm->Pos_z - ((*ppSca_data)->Z + (*ppSca_data)->D));
			}

			if (Dis < Best_dis) {
				Best_dis = Dis;
				EF->pDest_wall = *ppSca_data;
			}
		}while (*++ppSca_data);

		EF->Dest_x = EF->pDest_wall->X + (EF->pDest_wall->W >> 1);
		EF->Dest_z = EF->pDest_wall->Z + (EF->pDest_wall->D >> 1);

		pEm->Move_cnt = 1;
		pEm->Timer0 = 0;
		pEm->Routine_2++;

	case 3:

		Vec.vx = EF->Dest_x;
		Vec.vy = pEm->Pos_y;
		Vec.vz = EF->Dest_z;
		Goto00_super(pEm, &Vec, 0x40);

		if (pEm->Sca_info & 1) pEm->Timer1++;

		if (pEm->Timer0 >= 0xB4)
			*(unsigned long *)&pEm->Routine_0 = 1;

		if (Wall_turn_ck(pEm, &Ck_pos)) {

			Wall_turn_init(pEm);
			EF->Attribute |= 0xC0;
			pEm->Be_flg |= 4;
			pEm->Routine_2++;
		}

		Mot_play2_sp(pEm, 0, 0xF, 0);	break;

	case 4:
		if (Mot_play2(pEm, 0x11, 0xF, 0)) {
			Wall_turn_end(pEm);
			EF->Attribute &= ~0x40;
			pEm->Routine_2++;
			break;
		}
		Wall_turning(pEm);
		break;

	case 5:
		if (Wall_turn_ck(pEm, &Ck_pos)) {

			pEm->Be_flg &= ~4;
			EF->Attribute &= ~2;
			EF->Gun_area = 1;
			Wall_turn_init(pEm);
			pEm->Routine_2++;
		}

		Mot_play2_sp(pEm, 0, 0xF, 0);
		break;
	case 6:
		if (Mot_play2(pEm, 0x11, 0xF, 0)) {
			Wall_turn_end(pEm);
			EF->Attribute &= ~0x80;
			*(unsigned long *)&pEm->Routine_0 = 0x1501;
			break;
		}	Wall_turning(pEm);

	}
}
static void Em22_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	static SVECTOR Ck_pos = { 2200, 0, 0, 0 };
	register long Wall_dir;
	SVECTOR Svec;

	switch (pEm->Routine_2) {
	case 0:
		*(unsigned long *)&pEm->Move_no = 0xF0001;
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Routine_2 = 1;
	case 1:

		Wall_dir = Em22_wall_hosei(pEm, &Ck_pos);

		if (Em22_search_lv0(pEm)) {
			*(unsigned long *)&pEm->Routine_0 = 0xD01;
			break;	/* retail */
		}

		if (Wall_dir & 0x20000000) {
			*(unsigned long *)&pEm->Routine_0 = 0xD01;
			break;
		}

		if (Wall_dir < 0) goto next;

		pEm->Be_flg |= 4;
		if (Wall_dir & 0x40000000) {
			pEm->Routine_2 = 5;
			goto deko;
		}
		pEm->Routine_2 = 3;
		EF->Wall_dir_new = Wall_dir;
		goto boko;

	next:

		Spd_get(pEm, Kan, Seq, 0);
		Joint_move2(pEm, Kan, Seq, 0x100);
		Add_speedXYZsuper(pEm, 0, 0);

		break;

	case 3:
	boko:

		*(unsigned long *)&pEm->Move_no = 0xF0011;
		pEm->Routine_2 = 4;
	case 4:
		if (Joint_move2(pEm, Kan, Seq, 0x100)) {
			Add_dir.vy = ((unsigned char)pEm->Timer2 - (unsigned char)pEm->Timer3) << 10;
			Add_dir.vy += ((unsigned char)pEm->Timer2 - EF->Wall_dir_new) << 10;

			Svec.vx = 0x69D;
			Svec.vz = 0;
			Rot_vector(((unsigned char)pEm->Timer2 << 10) + Add_dir.vy, &Svec, &Svec);

			if (EF->Wall_dir_new & 1) {
				pEm->Pos_x += Svec.vx;
				pEm->Pos_z = Sca_get_pos(EF->Wall_dir_new, EF->pSca_dat);
			}else {
				pEm->Pos_z += Svec.vz;
				pEm->Pos_x = Sca_get_pos(EF->Wall_dir_new, EF->pSca_dat);
			}

			pEm->Timer3 = EF->Wall_dir_new;
			EF->Attribute |= 0x100;

			pEm->Be_flg &= ~4;
			pEm->Routine_2 = 0;

		}	break;

	case 5:
	deko:
		*(unsigned long *)&pEm->Move_no = 0xF0010;
		pEm->Routine_2 = 6;
	case 6:
		if (Joint_move2(pEm, Kan, Seq, 0x100)) {
			Add_dir.vy = ((unsigned char)pEm->Timer2 << 10) - ((unsigned char)pEm->Timer3 << 10);

			Svec.vx = 0x670;
			Svec.vz = 0;
			Rot_vector(((unsigned char)pEm->Timer2 << 10) + Add_dir.vy, &Svec, &Svec);

			pEm->Timer3 = ((unsigned char)pEm->Timer3 + (Add_dir.vy >> 10)) & 3;

			if (pEm->Timer3 & 1) {
				pEm->Pos_x += Svec.vx;
				pEm->Pos_z = Sca_get_pos((unsigned char)pEm->Timer3, EF->pSca_dat);
			}else {
				pEm->Pos_z += Svec.vz;
				pEm->Pos_x = Sca_get_pos((unsigned char)pEm->Timer3, EF->pSca_dat);
			}

			pEm->Be_flg &= ~4;
			pEm->Routine_2 = 0;
		}

	}

}
static void Em22_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	/* retail: rewritten leap attack (Beta 2: tongue lunge with player grab) */
	register long Dir;
	SVECTOR Unused = { 0, 400, 0, 0 };	/* retail copies this initializer to the frame and never reads it */

	switch (pEm->Routine_2) {
	case 0:
		pEm->Spd_x = 0;
		pEm->Spd_z = 0;
		pEm->Spd_y = 0;
		EF->Retail_29 = 0;
		EF->Retail_2a = 0;
		pEm->Timer0 = 0;
		EF->Retail_2c = pEm->Pos_y;
		if (!(EF->Attribute & 2)) {
			pEm->Routine_2 = 2;
			goto jump;
		}
		*(unsigned long *)&pEm->Move_no = 0x30006;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Joint_move2(pEm, Kan, Seq, 0x10400);
		break;
	case 2:
	jump:
		EF->Gun_area = 3;
		*(signed char *)&EF->Retail_29 = -10;
		EF->Retail_2a = 0;
		EF->Attribute &= ~2;
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
	case 3:
		pEm->Routine_2 += Mot_play2(pEm, 0x1F, 0, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 6);
		break;
	case 4:
		pEm->Spd_x = 550;
		pEm->Spd_y = -245;
		*(signed char *)&EF->Retail_29 = -8;
		EF->Retail_2a = 35;
		pEm->Pos_y -= 0x72F;
		EF->Attribute |= 0x200;
		pEm->Routine_2++;
	case 5:
		pEm->Routine_2 += Mot_play2(pEm, 0x20, 0, 0);
		Goto00(pEm, EF->pTarget->Pos_x, EF->pTarget->Pos_z, 0x20);

		if (!EF->pTarget->Damage_cnt)
		if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 15) {
			Dir = Get_dir_pl(pEm);
			if (Dir >= -199 && Dir < 712 && pEm->L_pl < 2200 && !pEm->Timer0) {
				EF->Dm_eff_func = func_80104204;
				EF->Dm_eff_rtn = 0;
				pEm->Timer0 = 1;
				EF->pTarget->pEnemy_ptr = pEm;
				Pad_vib_set0(10, 0);
				Pad_vib_fade_set(0x16, 0xFF, 0x32, 3);
				Pad_vib_fade_set(0xC, 0x37, 0x28, 0x16);
				Pl_life_down(EF->pTarget->Life >= 111 ? 100 : 80, 0);
				Snd_se_enem(10, pEm);
				if (EF->pTarget->Life < 0) {
					*(unsigned long *)&G.Player.Routine_0 = 3;
					EF->pTarget->Life = -1;
					G.System_flg |= 0x4000100;
					G.Player.Timer0 = Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);
				}else {
					if (EF->pTarget->Life < 40) EF->pTarget->Life = 40;
					*(unsigned long *)&EF->pTarget->Routine_0 = 0x202;
					EF->pTarget->Routine_1 += Cdir_ck(pEm, (ENEMY_WORK *)&G.Player);
				}
			}
		}
		break;
	case 6:
		*(unsigned long *)&pEm->Move_no = 0x1C20;
		if (pEm->Pos_y <= EF->Retail_2c - 0x60F) break;
		*(signed char *)&EF->Retail_29 = -60;
		pEm->Spd_y = 0;
		EF->Retail_2a = 0;
		pEm->Pos_y = EF->Retail_2c;
		EF->Attribute &= ~0x200;
		pEm->Routine_2++;
	case 7:
		if (Mot_play2(pEm, 0x21, 0, 0))
			*(unsigned long *)&pEm->Routine_0 = 0xF01;
	}

	pEm->Pos_y += pEm->Spd_y;
	Add_speedXZ(pEm, 0);
	pEm->Spd_x += (signed char)EF->Retail_29;
	pEm->Spd_y += EF->Retail_2a;
	if (pEm->Spd_x < 0) {
		EF->Retail_29 = 0;
		pEm->Spd_x = 0;
	}
}
/* retail-only (retail 0x80104204 / 0x80111204, 81 words; no Beta twin, name unknown):
 * Dm_eff_func installed by Em22_mv0a's hit: on even steps 0,2,4,6 rotates one of four
 * effect offsets by the player's Timer0 direction and spawns effect 0x2400 on the player
 * (Set_esp_s); clears itself after 7 steps. */
static void func_80104204(ENEMY_WORK *pEm)
{
	SVECTOR Pos[4] = { { 350, -400, 400, 0 }, { 350, -300, 250, 0 }, { 350, -200, 100, 0 }, { 350, -100, -50, 0 } };

	if (!(EF->Dm_eff_rtn & 1)) {
		Rot_vector(G.Player.Timer0 << 11, &Pos[ EF->Dm_eff_rtn >> 1 ], &Pos[ EF->Dm_eff_rtn >> 1 ]);
		Set_esp_s((ENEMY_WORK *)&G.Player, 0, 2, 0x2400, 0, 0, &Pos[ EF->Dm_eff_rtn >> 1 ]);
	}
	if (++EF->Dm_eff_rtn == 7) EF->Dm_eff_func = 0;
}
static void Em22_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->pSca_dat = 0;
		*(unsigned long *)&pEm->Move_no = 3;
		EF->Attribute |= 2;
		EF->Gun_area = 0;
		pEm->Spd_y = 0;
		pEm->Routine_2++;
	case 1:
		Joint_move2(pEm, Kan, Seq, 0x1000);

		if (EF->Sence_lv == 2 || (EF->pTarget = Em22_search_lv0(pEm))) {

			EF->Gun_area = 5;
			*(unsigned long *)&pEm->Move_no = 0x12;
			pEm->Timer0 = -1;
			pEm->Routine_2++;
		}
		break;
	case 2:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		if (Joint_move2(pEm, Kan, Seq, 0x1000)) {

			pEm->Routine_2++;
			*(unsigned long *)&pEm->Move_no = 0x13;
		}	break;
	case 3:
	{
		SVECTOR Ck_pos = { 2200, 0, 0, 0 };

		pEm->Pos_y += 0xA8C;
		Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);
		G.pEm->Cdir_y = (Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00;
		Em_init_super(pEm);
		pEm->Routine_2++;
	}
	case 4:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		*(unsigned long *)&pEm->Move_no = 0x13;
		Joint_move2(pEm, Kan, Seq, 0x1000);

		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			*(unsigned long *)&pEm->Move_no = 0x13;
			pEm->Routine_2++;
		}
		break;

	case 5:
		EF->Gun_area = 4;
		pEm->Pos_y = 0;
		if (Joint_move2(pEm, Kan, Seq, 0x1000))
			*(unsigned long *)&pEm->Routine_0 = 1;

	}
	pEm->Timer0++;
}
static void Em22_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register long Tmp;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		pEm->Be_flg |= 0xC;	/* retail: was 4 */
		pEm->Damage_cnt |= 0x80;	/* retail */

		Tmp = (unsigned char)pEm->Timer3 - (unsigned char)pEm->Timer2;
		if (abs(Tmp) > 1) Tmp /= -3;

		if (pEm->Pos_y >= pEm->Ground - 0x903) {

			if (pEm->Pos_y > pEm->Ground - 0x631 || pEm->Pos_y < pEm->Ground - 0x66D) {

				pEm->Timer1 = -(pEm->Pos_y + 0x64F) + pEm->Ground;
				pEm->Spd_y = pEm->Timer1 / 22;
			}else {
				pEm->Spd_y = 0;
				pEm->Pos_y = pEm->Ground - 0x64F;
			}
			if (Tmp < 0) {
				*(unsigned long *)&pEm->Move_no = 0xF001A;
			}else {
				*(unsigned long *)&pEm->Move_no = 0xF000C;
			}
			pEm->Routine_2++;
		}else {
			*(unsigned long *)&pEm->Move_no = 0xF001B;
			if (Tmp < 0) pEm->Timer1 = 1;
			pEm->Routine_2 = 3;
			Em22_Atd_set_1(pEm);
			goto side;
		}
	case 1:
		if (pEm->Timer1 < -20 || pEm->Timer1 > 20) {
			pEm->Timer1 -= pEm->Spd_y;
			pEm->Pos_y += pEm->Spd_y;
		}

		if (Joint_move2(pEm, Kan, Seq, 0x100)) {
			SVECTOR Vec = { 0x64F, 0, 0, 0 };

			Rot_vector((unsigned char)pEm->Timer3 << 10, &Vec, &Vec);
			pEm->Pos_x += Vec.vx;
			pEm->Pos_z += Vec.vz;
			pEm->Pos_y = 0;
			Em_init_super(pEm);
			pEm->Cdir_y = (unsigned char)pEm->Timer2 << 10;
			EF->pSca_dat = 0;
			pEm->Routine_2++;
		}
		break;

	case 2:
		pEm->Move_cnt = 0;
		Mot_play2_spxz(pEm, 3, 3, 0);
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		pEm->Timer0++;
		pEm->Be_flg &= ~0xC;	/* retail: one clear after the call (was ~8 before it, ~4 here) */
		pEm->Damage_cnt &= 0x7F;	/* retail */
		if (pEm->Timer0 >= 4) *(unsigned long *)&pEm->Routine_0 = 0xE01;
		break;

	case 3:
	side:
		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, (pEm->Timer1 << 16) | 0x100))
			pEm->Routine_2 = 0;

		if (pEm->Timer1)
			Add_speedXYZsuper(pEm, 0x800, 0);
		else
			Add_speedXYZsuper(pEm, 0, 0);
	}
}
static void Em22_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;

	if (EF->pTarget->Life < 0 || pEm->Routine_0 == 7) {

		*(unsigned long *)&pEm->Routine_0 = 0xE01;	goto exit;
	}

	if ((pEm->Type & 0x10) && pEm->Life < 50) {
		if (!(Rnd() & 3)) {
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			goto exit;
		}
	}

	Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, EF->pTarget->Pos_x, EF->pTarget->Pos_z);

	Dir = (Dir - pEm->Cdir_y) & 0xFFF;
	if (Dir > 0x800) Dir = 0x1000 - Dir;
	Dir = Branch(Dir, 0x100, 0x200, 0x600);

	switch (Branch(pEm->L_pl, 2500, 4500, 6500)) {

	case 0:

		switch (Dir) {
		case 0:
			if (!EF->At_cnt) {

				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;

		case 1:
			if (!EF->At_cnt) {

				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;

		case 2:
		case 3:
			if (!EF->At_cnt) {

				if (!(Rnd() & 3)) {
					*(unsigned long *)&pEm->Routine_0 = 0x901;	goto exit;
				}	*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;

		}
		break;

	case 1:

		switch (Dir) {
		case 0:
			if (!EF->At_cnt) {	/* retail: random choice by the target's life (was life ratio <= 1/2) */
				if (EF->pTarget->Life >= 100) {
					if ((Rnd() & 7) < 3)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else if (Rnd() & 2)
						*(unsigned long *)&pEm->Routine_0 = 0x1301;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x801;
				}
				if (EF->pTarget->Life < 100) {
					if ((Rnd() & 7) < 2)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x801;
				}
				goto exit;
			}	*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
		case 1:
			if (EF->At_cnt) {
				if (!(Rnd() & 1)) {
					*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;
				}	*(unsigned long *)&pEm->Routine_0 = 0x601;	goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x801;	goto exit;

		case 2:
		case 3:
			*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;

		}
		break;

	case 2:

		switch (Dir) {
		case 0:
			if (!(EF->Attribute & 1)) {	/* retail: random choice by the target's life */
				if (EF->pTarget->Life >= 100) {
					if ((Rnd() & 7) < 5)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x1301;
				}
				if (EF->pTarget->Life < 100) {
					if ((Rnd() & 7) < 2)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x1301;
				}
				goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;

		case 1:
			if (!(Rnd() & 3)) {
				*(unsigned long *)&pEm->Routine_0 = 0x201;	goto exit;
			}
			*(unsigned long *)&pEm->Routine_0 = 0x1301;	goto exit;

		case 2:
		case 3:	*(unsigned long *)&pEm->Routine_0 = 0x1401;	goto exit;
		}

		break;

	case 3:

		if (pEm->L_pl < 10000 || EF->Sence_lv != 2) {

			EF->Sence_lv = 2;
			switch (Dir) {
			case 0:
			case 1:
			case 2:	/* retail: random choice by the target's life (was 0x1401 only) */
				if (EF->pTarget->Life >= 100) {
					if ((Rnd() & 7) < 5)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x1401;
				}
				if (EF->pTarget->Life < 100) {
					if ((Rnd() & 7) < 2)
						*(unsigned long *)&pEm->Routine_0 = 0xA01;
					else
						*(unsigned long *)&pEm->Routine_0 = 0x1401;
				}
				goto exit;

			case 3:
				*(unsigned long *)&pEm->Routine_0 = 0xC01;	goto exit;

			}
			break;

		}
		*(unsigned long *)&pEm->Routine_0 = 0xC01;

	}

exit:
	if (pEm->Routine_1 != 0xF)
		Em22_mv[ pEm->Routine_1 ](pEm, Kan, Seq);

	EF->Attribute &= ~1;
}
static void Em22_mv10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_cnt = 0;
		Mot_play2_spxz(pEm, 3, 0, 0);
		if (pEm->Sce_free0) {
			pEm->Sce_free0 = 0;
			pEm->Type &= ~0x4000;
			pEm->Routine_2++;
			pEm->Spd_y = 0;
		}
		break;

	case 1:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Routine_2 += Mot_play2(pEm, 0x12, 0, 0);
		break;

	case 2:
	{
		SVECTOR Ck_pos = { 2200, 0, 0, 0 };

		pEm->Pos_y += 0xA8C;
		Rot_vector_super(pEm->Cdir_y, (MATRIX *)pEm->pSuper, &Ck_pos, &Ck_pos);
		G.pEm->Cdir_y = (Direction_ck(0, 0, Ck_pos.vx, Ck_pos.vz) + 0x100) & 0xC00;
		Em_init_super(pEm);
		*(unsigned long *)&pEm->Move_no = 0x13;
		pEm->Routine_2++;
	}
	case 3:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Move_cnt = 0;
		Joint_move2(pEm, Kan, Seq, 0x100);

		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			EF->Gun_area = 3;
			Em22_Atd_set_1(pEm);
			pEm->Be_flg &= ~8;	/* retail: moved out of Em22_Atd_set_1 */
			pEm->Move_no = 0;
			pEm->Routine_2++;
		}
		break;

	case 4:
		pEm->Pos_y = 0;
		pEm->Routine_2 += Mot_play2(pEm, 0x13, 0, 0);
		break;

	case 5:
		Mot_play2_spxz(pEm, 3, 3, 0);
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		if (pEm->Sce_free0) *(unsigned long *)&pEm->Routine_0 = 0xE01;

	}
}
static void Em22_mv11(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_cnt = 0;
		Mot_play2_spxz(pEm, 3, 0, 0);
		if (pEm->Sce_free0) {
			pEm->Sce_free0 = 0;
			pEm->Type &= ~0x4000;
			pEm->Routine_2++;
			pEm->Spd_y = 0;
			*(unsigned long *)&pEm->Move_no = 0xF0013;
		}
		break;

	case 1:
		pEm->Spd_y += 0x48;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Move_cnt = 0;
		Joint_move2(pEm, Kan, Seq, 0x100);

		if (pEm->Pos_y + pEm->Spd_y + 0x48 >= 0) {
			EF->Gun_area = 3;
			Snd_se_enem(2, pEm);
			Em22_Atd_set_1(pEm);
			pEm->Be_flg &= ~8;	/* retail: moved out of Em22_Atd_set_1 */
			pEm->Move_no = 0;
			pEm->Routine_2++;
		}
		break;

	case 2:
		pEm->Pos_y = 0;
		pEm->Routine_2 += Mot_play2(pEm, 0x13, 0, 0);
		break;

	case 3:
		Mot_play2_spxz(pEm, 3, 3, 0);
		EF->Attribute |= 2;
		EF->Gun_area = 4;
		if (pEm->Sce_free0) *(unsigned long *)&pEm->Routine_0 = 0xE01;

	}
}
static void Em22_mv12(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		EF->Attribute |= 4;
		pEm->Move_cnt = 0;
		Mot_play2(pEm, 0, 0, 0);
		if (pEm->Sce_free0) {
			pEm->Sce_free0 = 0;
			pEm->Move_no = 0xFF;
			pEm->Routine_2++;
		}
		break;

	case 1:
		Mot_play2_sp(pEm, 0, 0, 0);
		if (pEm->Sce_free0)
			pEm->Routine_2++;
		break;
	case 2:	break;

	}
}
static void Em22_mv15(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_no = 1;
		pEm->Routine_2++;
		pEm->Timer0 = 0;
		EF->Wall_at = 0;
	case 1:
		pEm->Timer0++;

		Goto00_super(pEm, (VECTOR *)&EF->pTarget->Pos_x, 0x40);
		Mot_play2_sp(pEm, 0, 0xF, 0);

		if (pEm->L_pl < 2500 || pEm->Timer0 >= 0x100) {
			EF->Sence_lv = 2;
			*(unsigned long *)&pEm->Routine_0 = 0xB01;
		}

	}
}
static void Em22_mv16(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:

		pEm->Be_flg |= 8;
		Mot_play2(pEm, 0xE, 0, 0);
		pEm->Move_no = 0;
		pEm->Move_cnt = 0;
		if (pEm->Sce_free0) pEm->Routine_2++;

		break;

	case 1:
		if (pEm->Move_cnt >= 5) Approach_one(&pEm->Pos_y, 0, 100);
		if (Mot_play2_sp(pEm, 0xE, 0, 0)) {
			pEm->Be_flg &= ~8;
			pEm->Type &= 0xFFF;
			*(unsigned long *)&pEm->Routine_0 = 0x501;
		}
		break;
	case 2:	break;

	}
}
static void Em22_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	if (pEm->Routine_1 != 0xFF) {
		EF->pTarget = (ENEMY_WORK *)&G.Player;
		EF->Wep_no = pEm->Routine_1;

		if (Tongue_flg == pEm->Work_no) Tongue_flg = 0;
		EF->Sence_lv = 2;	/* retail */
		pEm->Routine_1 = 0xFF;
		Em22_damage_ck(pEm);
		if (pEm->Damage_no) {
			pEm->Be_flg &= ~0xE;
			EF->Attribute &= 0xFD0F;	/* retail: also clears 0x200 */
		}
	}

	Em22_dm[ pEm->Damage_no ](pEm, Kan, Seq);	}
static void Em22_damage_ck(ENEMY_WORK *pEm)
{
	register unsigned long Dir;

	if (!(EF->Attribute & 0x40)) {
		if (EF->Attribute & 0x200)	/* retail: new damage type 7 (func_80106764) */
			pEm->Damage_no = 7;
		else

		if (EF->pSca_dat || pEm->Pos_y < pEm->Ground) {
			if (EF->Attribute & 0x80)
				pEm->Damage_no = 5;
			else
				pEm->Damage_no = 2;
		}else {

			Dir = Cdir_ck(pEm, EF->pTarget);

			switch (EF->Wep_no) {

			case 5: case 6: case 7: case 8: case 9:	/* retail: 9 moved here; random light flinch */
				if ((Rnd() & 0xF) < 5) {
					pEm->Damage_no = 1;
					break;
				}
				if (Dir)

					pEm->Damage_no = 4;
				else
					pEm->Damage_no = 3;	break;

			case 10:
				if (Dir)
					pEm->Damage_flg |= 0x20;
				else
					pEm->Damage_flg &= ~0x20;
				pEm->Damage_no = 6;
				break;
			case 11:
				if ((Rnd() & 0xF) < 5) {	/* retail */
					pEm->Damage_no = 1;
					break;
				}
				if (Dir)
					pEm->Damage_no = 4;
				else
					pEm->Damage_no = 3;
				break;

			case 4: case 12: case 14: case 15:

				pEm->Damage_no = 1;
				break;
			case 17:	/* retail: was Damage_no 0 */
				if (Dir)
					pEm->Damage_flg |= 0x20;
				else
					pEm->Damage_flg &= ~0x20;
				pEm->Damage_no = 6;
				break;
			case 1: case 2: case 3: case 13: case 16: case 18: case 19:
				pEm->Damage_no = 0;	}	}
	}else {

		pEm->Damage_no = 0;
	}

}
static void Em22_damage_after_ck(ENEMY_WORK *pEm)
{
	if (pEm->L_pl < 2500) {
		if (!Goto00_ck(EF->pTarget, pEm->Pos_x, pEm->Pos_z, 0x140)) {
			EF->Attribute |= 1;
			*(unsigned long *)&pEm->Routine_0 = 0x1301;
			return;
		}else {	if (Rnd() & 1)
				*(unsigned long *)&pEm->Routine_0 = 0xC01;
			else
				*(unsigned long *)&pEm->Routine_0 = 0xF01;	}

	}else {
		if (!Goto00_ck(EF->pTarget, pEm->Pos_x, pEm->Pos_z, 0x1E0))
			*(unsigned long *)&pEm->Routine_0 = 0x1401;
		else
			*(unsigned long *)&pEm->Routine_0 = 0xC01;
	}
}
static void Em22_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Life >= 0) {
		*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		if (pEm->Routine_1 != 0xFF) {
			EF->Attribute |= 1;
			Em22_mv[ pEm->Routine_1 ](pEm, Kan, Seq);
			EF->Attribute &= ~1;
		}
	}else
		*(unsigned long *)&pEm->Routine_0 = 3;

}
static void Em22_dm_s(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;

	if (pEm->Damage_flg & 0x20) {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0)
				Em22_damage_after_ck(pEm);
			else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}
	}else {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0) {
				Em22_damage_after_ck(pEm);
			}else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}

	}	}
static void Em22_dm_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;
	register long Up_dir;
	unsigned char next_routine; /* [INFERRED] consumed case-3 increment; retail original SYM unavailable. */

	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Spd_y = pEm->Spd_x = pEm->Spd_z = 0;
		switch ((unsigned char)pEm->Timer3) {
		case 0:
			pEm->Spd_x = 150;	break;
		case 1:
			pEm->Spd_z = -150;	break;
		case 2:
			pEm->Spd_x = -150;	break;
		case 3:
			pEm->Spd_z = 150;
		}	Up_dir = 4;
		if (*(unsigned char *)&pEm->Timer2)
			Up_dir = *(unsigned char *)&pEm->Timer2;

		if ((unsigned char)pEm->Timer3 - Up_dir < 0)

			*(unsigned long *)&pEm->Move_no = 0x17;
		else
			*(unsigned long *)&pEm->Move_no = 0x18;

		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
		pEm->Timer0 = 4;
		do { }while (0);	/* [INFERRED] code-less retail line (compiled-out macro): its loop notes are a scheduling barrier */

		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);	break;

	case 2:
		pEm->Pos_y += 0x4F5;
		pEm->Cdir_y = ((unsigned char)pEm->Timer2 << 10) - 0x800;
		Em_init_super(pEm);
		pEm->Routine_3++;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0x19;
		Joint_move2(pEm, Kan, Seq, 0x400);

		if (pEm->Pos_y >= 0) {

			EF->pSca_dat = 0;
			Em22_Atd_set_1(pEm);
			/* retail: moved out of Em22_Atd_set_1; capture the real increment before the disjoint stores. */
			next_routine = pEm->Routine_3 + 1;
			pEm->Be_flg &= ~8;
			*(unsigned long *)&pEm->Move_no = 0x19;
			/* [INFERRED] Reused Em24_dm_fall's real-store weighting lever, independently verified here:
			 * four one-pass levels give the byte quantity 8 refs/14 lifetime, above flags' 4/6, so
			 * it takes v0 and shares case 5's tail (4 diffs -> 0 in both banks). Original spelling unknown. */
			do { do { do { do {
				pEm->Routine_3 = next_routine;
			} while (0); } while (0); } while (0); } while (0);

			do { }while (0);	/* [INFERRED] code-less retail line: compiled-out macro; its loop notes are a scheduling barrier (keeps the Joint_move2 setup below Routine_3++, so the tail cross-jumps into case 5 as in retail) */

			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);	}	break;
	case 5:
		if (pEm->Life < 0) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			pEm->Be_flg &= ~0xC;
			pEm->Routine_2++;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0x16;
		pEm->Routine_3++;
	case 1: case 4: case 6:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		break;
	case 7:
		pEm->Damage_cnt &= 0x7F;
		EF->Gun_area = 3;
		pEm->Be_flg &= ~0xC;
		Em22_damage_after_ck(pEm);
	}

	if (pEm->Pos_y + pEm->Spd_y + 0x48 < 0) {
		pEm->Spd_y += 0x30;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
	}else
		pEm->Pos_y = 0;

}
static void Em22_dm_lf(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	VECTOR TmpV1, TmpV2;

	switch (pEm->Routine_3) {
	case 0:

		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Spd_x = 50;
		pEm->Spd_y = pEm->Spd_z = 0;
		Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);
		pEm->Timer0 = Rnd() & 0x11;	/* retail: random hold time (was 0) */

		pEm->Atd[2].Ofs_x = -0x2A0;
		pEm->Atd[2].Ofs_z = 0;

		*(unsigned long *)&pEm->Move_no = 9;
		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
	case 1:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		if (EF->Sca_ofs > -800) EF->Sca_ofs -= 40;

		EF->Gun_area = 3;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		break;

	case 2:
		if (pEm->Life < 0) {
			pEm->Routine_2++;
			pEm->Routine_3 = 0;
			break;
		}
		if (--pEm->Timer0 > 0) break;	/* retail: holds for Timer0 frames */
		*(unsigned long *)&pEm->Move_no = 0x30016;

		pEm->Timer0 = 0;	/* retail: no fall-through into case 3 */
		pEm->Routine_3 = 3;
		break;
	case 3:
		pEm->Timer0++;
		if (EF->Sca_ofs < -500) EF->Sca_ofs += 80;

		Spd_get(pEm, Kan, Seq, 0);
		if (Joint_move2(pEm, Kan, Seq, 0x400)) {
			pEm->Damage_cnt &= 0x7F;
			EF->Sca_ofs = -500;
			EF->Gun_area = 3;
			Em22_damage_after_ck(pEm);
		}
		Add_speedXZ(pEm, 0);
	}
}
static void Em22_dm_lb(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
		pEm->Spd_x = 100;
		pEm->Spd_y = pEm->Spd_z = 0;
		Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		Goto01(pEm, EF->pTarget->Cdir_y, 0x80);
		pEm->Timer0 = 0;

		*(unsigned long *)&pEm->Move_no = 0x3000A;
		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
	case 1:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
		pEm->Timer0++;

		if (pEm->Timer0 == 4) {
			pEm->Spd_x = 50;
			pEm->Spd_y = pEm->Spd_z = 0;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
		}

		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		break;

	case 2:
		if (pEm->Life < 0) {
			pEm->Routine_2++;
			pEm->Routine_3 = 0;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0xF0005;
		EF->Attribute |= 2;
		pEm->Routine_3 = 3;
	case 3:
		if (Joint_move2(pEm, Kan, Seq, 0x10100)) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			Em22_damage_after_ck(pEm);
		}
	}
}
static void Em22_dm_fall_v(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;
	unsigned char next_routine; /* [INFERRED] consumed case-3 increment; retail original SYM unavailable. */

	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Spd_y = pEm->Spd_x = pEm->Spd_z = 0;
		*(unsigned long *)&pEm->Move_no = 0x22;
		EF->Attribute |= 2;
		pEm->Routine_3 = 1;
		pEm->Timer0 = 4;
	case 1:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		if (pEm->Move_cnt < 6)
			pEm->Spd_y = 0;	break;

	case 2:	{
		register unsigned long Axis;
		pEm->Pos_y += 0x4F5;

		Axis = Get_axis2((MATRIX *)pEm->pSuper);

		switch (Axis) {
		case 0:
		case 1:
			break;
		case 2:
			pEm->Pos_x -= 600;
			break;
		case 3:
			pEm->Pos_x += 600;
			break;
		case 4:
			pEm->Pos_z -= 600;
			break;
		case 5:
			pEm->Pos_z += 600;
		}

	}
		Em_init_super(pEm);
		pEm->Routine_3++;

	case 3:
		*(unsigned long *)&pEm->Move_no = 0x19;
		Joint_move2(pEm, Kan, Seq, 0x400);

		if (pEm->Pos_y >= 0) {

			EF->pSca_dat = 0;
			Em22_Atd_set_1(pEm);
			/* retail: moved out of Em22_Atd_set_1; capture the real increment before the disjoint stores. */
			next_routine = pEm->Routine_3 + 1;
			pEm->Be_flg &= ~8;
			*(unsigned long *)&pEm->Move_no = 0x19;
			/* [INFERRED] Four one-pass store levels independently verified here (4 diffs -> 0,
			 * both banks): byte quantity 8 refs/14 lifetime beats flags' 4/6 and takes v0 for the
			 * shared case-5 tail. This removes the old Axis reuse; original macro spelling unknown. */
			do { do { do { do {
				pEm->Routine_3 = next_routine;
			} while (0); } while (0); } while (0); } while (0);

			do { }while (0);	/* [INFERRED] code-less retail line: compiled-out macro; its loop notes are a scheduling barrier (keeps the Joint_move2 setup below Routine_3++, so the tail cross-jumps into case 5 as in retail) */

			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);	}	break;
	case 5:
		if (pEm->Life < 0) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			pEm->Be_flg &= ~0xC;
			pEm->Routine_2++;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0x16;
		pEm->Routine_3++;
	case 4: case 6:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		break;
	case 7:
		pEm->Damage_cnt &= 0x7F;
		EF->Gun_area = 3;
		pEm->Be_flg &= ~0xC;
		Em22_damage_after_ck(pEm);
	}

	if (pEm->Pos_y + pEm->Spd_y + 0x48 < 0) {
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
		pEm->Spd_y += 0x30;
	}else
		pEm->Pos_y = pEm->Ground;	/* retail: was 0 */

}
static void Em22_dm_flame(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;

	if (pEm->Damage_flg & 0x20) {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0)
				Em22_damage_after_ck(pEm);
			else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}
	}else {

		switch (pEm->Routine_3) {
		case 0:
			Snd_se_enem(7, pEm);
			Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
			pEm->Spd_x = 100;
			pEm->Spd_y = pEm->Spd_z = 0;
			Goto01(pEm, EF->pTarget->Cdir_y + 0x800, 0x80);

			*(unsigned long *)&pEm->Move_no = 0x30008;
			EF->Attribute &= ~2;
			pEm->Routine_3 = 1;

		case 1:
			if (pEm->Spd_x > 0) pEm->Spd_x -= 6;
			Rot_vector(EF->pTarget->Cdir_y, (SVECTOR *)&pEm->Spd_x, &L_spd);
			pEm->Pos_x += L_spd.vx;
			pEm->Pos_z += L_spd.vz;
			pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
			break;
		case 2:
			if (pEm->Life >= 0) {
				Em22_damage_after_ck(pEm);
			}else
				*(unsigned long *)&pEm->Routine_0 = 3;
		}

	}	}
/* retail-only (retail 0x80106764 / 0x80113764, 150 words; no Beta twin, name unknown):
 * Em22_dm[7], chosen by Em22_damage_ck when Attribute bit 0x200 (set during Em22_mv0a's
 * leap) is up: the licker drops from the leap height to the saved ground height
 * EF->Retail_2c, then gets up (cf. Em22_dm_fall). */
static void func_80106764(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR L_spd;	/* unused, as in Em22_dm_fall / Em22_dm_fall_v: retail reserves its frame slot */

	switch (pEm->Routine_3) {
	case 0:
		pEm->Damage_cnt |= 0x80;
		Snd_se_enem(7, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		*(unsigned long *)&pEm->Move_no = 0x19;
		pEm->Spd_y = pEm->Spd_x = pEm->Spd_z = 0;
		EF->Attribute |= 2;
		pEm->Pos_y += 0x546;
		pEm->Routine_3++;
	case 1:
		Joint_move2(pEm, Kan, Seq, 0x1000);
		pEm->Move_cnt = 0;
		if (pEm->Pos_y < EF->Retail_2c) break;
		EF->pSca_dat = 0;
		Em22_Atd_set_1(pEm);
		*(unsigned long *)&pEm->Move_no = 0x19;
		pEm->Be_flg &= ~8;
		pEm->Routine_3++;
	case 2:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x400);
		break;
	case 3:
		if (pEm->Life < 0) {
			pEm->Damage_cnt &= 0x7F;
			EF->Gun_area = 3;
			pEm->Be_flg &= ~0xC;
			pEm->Routine_2++;
			break;
		}
		*(unsigned long *)&pEm->Move_no = 0x16;
		pEm->Routine_3++;
	case 4:
		pEm->Routine_3 += Joint_move2(pEm, Kan, Seq, 0x1000);
		break;
	case 5:
		pEm->Damage_cnt &= 0x7F;
		EF->Gun_area = 3;
		pEm->Be_flg &= ~0xC;
		Em22_damage_after_ck(pEm);
	}

	if (pEm->Pos_y + pEm->Spd_y + 0x48 < EF->Retail_2c) {
		pEm->Spd_y += 0x30;
		pEm->Pos_y += pEm->Spd_y;
		pEm->Pos_x += pEm->Spd_x;
		pEm->Pos_z += pEm->Spd_z;
	}else
		pEm->Pos_y = EF->Retail_2c;
}
static void Em22_die_ck(ENEMY_WORK *pEm)
{

	if (EF->Attribute & 0x40)

		pEm->Damage_no = 3;
	else
	if (EF->Attribute & 0x200)	/* retail: new die type 5 (func_80106FC4) */
		pEm->Damage_no = 5;
	else
	if (EF->pSca_dat || pEm->Pos_y < pEm->Ground) {
		if (EF->Attribute & 0x80)
			pEm->Damage_no = 4;
		else
			pEm->Damage_no = 2;
	}else {

		if (Cdir_ck(pEm, (ENEMY_WORK *)&G.Player))
			pEm->Damage_no = 1;
		else
			pEm->Damage_no = 0;
	}	}
static void Em22_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Routine_1 != 0xFF) {
		EF->Wep_no = pEm->Routine_1;
		pEm->Routine_1 = 0xFF;
		EF->pTarget = (ENEMY_WORK *)&G.Player;
		Em_dead_flg_on(pEm);
		func_80101790(pEm);
		Em22_die_ck(pEm);
	}

	Em22_die[ pEm->Damage_no ](pEm, Kan, Seq);
}
static void Em22_die_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(8, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Be_flg |= 2;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Mot_play2_sp(pEm, 0x1C, 0, 0);
		if (EF->Sca_ofs > -1000) EF->Sca_ofs -= 50;
		break;
	case 2:
		pEm->Routine_2++;
	case 3:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}
		break;
	case 4: case 5:	break;
	}
}
static void Em22_die_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Snd_se_enem(8, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 1);
		pEm->Be_flg |= 2;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Mot_play2_sp(pEm, 0x1D, 0xF, 0);
		if (EF->Sca_ofs < -100) EF->Sca_ofs += 25;
		break;
	case 2:
		EF->Kage_Rno = 1;
		pEm->Routine_2++;

	}
}
static void Em22_die_wall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Em22_dm_fall(pEm, Kan, Seq);
		break;
	case 1:
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
		break;
	case 2:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}

	}
}
static void Em22_die_climb(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Em_init_super(pEm);
		Snd_se_enem(8, pEm);
		Blood_move(pEm, &Wep_blood_tbl[ EF->Wep_no ], 0);
		pEm->Move_no = 0;
		pEm->Routine_2++;
	case 1:
		pEm->Routine_2 += Mot_play2(pEm, 9, 0, 0);
		break;

	case 2:
		pEm->Routine_2++;	break;
	case 3:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}

	}
}
static void Em22_die_wall_v(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		Em22_dm_fall_v(pEm, Kan, Seq);
		break;
	case 1:
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
		break;
	case 2:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}

	}
}
static void Em22_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}
static void Em22_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}
void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em22_pldm_tbl[ pEm->Routine_1 ](pEm, Kan, Seq);	}
/* retail: Em22_Pl00 is gone (no code for it in the retail overlay); Em22_pldm_tbl is empty and
 * shares its address with Dm_eff_spark's pEsp2 (Em122_Pldm indexes 0x80107688). In its place the
 * retail text has Em22_die[5]: */
/* retail-only (retail 0x80106FC4 / 0x80113FC4, 44 words; no Beta twin, name unknown):
 * Em22_die[5] (Damage_no 5 from Em22_die_ck, Attribute bit 0x200): Em22_die_wall with the
 * leap-fall damage func_80106764 in place of Em22_dm_fall. */
static void func_80106FC4(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		func_80106764(pEm, Kan, Seq);
		break;
	case 1:
		Snd_se_enem(8, pEm);
		pEm->Routine_2++;
		break;
	case 2:
		if (Mot_play2(pEm, 0xB, 0xF, 0)) {
			EF->Kage_Rno = 1;
			pEm->Routine_2++;
		}

	}
}
/* ---- data emitted after Em22_mv07's local static (retail data order) ---- */
static WEP_BLOOD Wep_blood_tbl[20] = {
	{ 0, 0, 0, 0, 0, 6, 8192 },
	{ 0, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 5, 0, 0, 24, 0, 6, 8192 },
	{ 8, 0, 0, 24, 0, 6, 8192 },
	{ 4, 0, 12, 12, 5, 4, 5120 },
	{ 6, 0, 0, 24, 5, 4, 6144 },
	{ 4, 0, 0, 24, 4, 9, 10240 },	/* retail: 7 -> 9 */
	{ 0, 0, 12, 12, 0, 6, 8192 },
	{ 1, 0, 12, 12, 0, 6, 8192 },
	{ 0, 0, 12, 12, 6, 0, 4096 },
	{ 1, 0, 0, 24, 0, 6, 8192 },
	{ 6, 0, 0, 24, 5, 4, 6144 },
	{ 1, 0, 8, 12, 5, 6, 8192 },	/* retail: 0 -> 5 */
	{ 1, 0, 8, 12, 0, 6, 8192 },
	{ 1, 0, 8, 12, 0, 6, 8192 }
};
static void (*Em22_routine_0[8])() = { Em22_Init, Em22_Move, Em22_Damage, Em22_Die, Em22_Move, 0, 0, Em22_Dead };
static void (*Em22_mv[24])() = { Em22_mv00, Em22_mv01, Em22_mv02, Em22_mv03, Em22_mv04, Em22_mv05, Em22_mv06, Em22_mv07, Em22_mv08, Em22_mv09, Em22_mv0a, Em22_mv0b, Em22_mv0c, Em22_mv0d, Em22_mv0e, Em22_mv0f, Em22_mv10, Em22_mv11, Em22_mv12, Em22_mv13, Em22_mv14, Em22_mv15, Em22_mv16, func_80102C40 };	/* retail: [23] added (0x8010764C) */
static void (*Em22_dm[8])() = { Em22_dm_no, Em22_dm_s, Em22_dm_fall, Em22_dm_lf, Em22_dm_lb, Em22_dm_fall_v, Em22_dm_flame, func_80106764 };	/* retail: [7] added (0x8010766C) */
static void (*Em22_die[6])() = { Em22_die_f, Em22_die_b, Em22_die_wall, Em22_die_climb, Em22_die_wall_v, func_80106FC4 };	/* retail: [5] added (0x80107684) */
static void (*Em22_pldm_tbl[])() = { };	/* retail: empty (Em22_Pl00 removed) */
/* retail 0x80107074 / 0x80114074, 55 words: Beta 2 EM2B_SUB.C Set_esp_w (twin: EM23.C Set_esp_w) */
static void Set_esp_w(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;
	SVECTOR Svec;

	ApplyMatrixSV(&pParts[Parts_no].Workm, pSvec, &Svec);
	Svec.vx += pParts[Parts_no].Workm.t[0];
	Svec.vy += pParts[Parts_no].Workm.t[1];
	Svec.vz += pParts[Parts_no].Workm.t[2];
	Esp_call(Id << 24 | Type << 16 | Zoom, Cdir_y, &GsIDMATRIX, &Svec);
}
/* retail 0x80107150 / 0x80114150, 24 words: Beta 2 EM2B_SUB.C Set_esp_s */
static void Set_esp_s(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec)
{
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	Esp_call(Id << 24 | Type << 16 | Zoom, Cdir_y, &pParts[Parts_no].Workm, pSvec);
}
/* retail 0x801071B0 / 0x801141B0, 27 words: the player's direction relative to the enemy's
 * facing, -0x7FF..0x800; the same code as Beta 2 EM2B_SUB.C Get_dir_pl (name from there).
 * Follows Set_esp_w / Set_esp_s at the end of the retail overlay text; called from Em22_mv0a. */
static long Get_dir_pl(ENEMY_WORK *pEm)
{
	short Dir;

	Dir = (Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y) & 0xFFF;
	if (Dir > 0x800) Dir |= 0xF000;
	return Dir;
}
