/* em2c.c - reconstruction of D:\Bio2\prog\EM2C.C (enemy 2C).
 * Linked into two overlays from the same source: 0x11 (at 0x80100004) and
 * 0x2F (at 0x8010d004); only the enemy-number-specific entry name differs
 * (Em12c/Em22c), selected by OVL_2F.
 * Types come from the TU's retail SYM preamble (em2c.h).
 * Joint_move() and Kage_work_set() are called without a prototype in scope
 * (implicit int): the implicit declaration in Em2c_Move's switch body is what
 * produces its empty retail SYM block (63-79). */
#include "EM2C.H"

/* ---- functions of this translation unit ---- */

void EM_MAIN(ENEMY_WORK *pEm)
{
	switch (pEm->Routine_0) {
	case 0:
		Em2c_Init(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
		break;
	case 1:
		Em2c_Move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

	}	}

static void Em2c_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(long *)&pEm->Routine_0 = 1;
	Joint_move(pEm, Kan, Seq, 0x1000);

	pEm->Damage_cnt = 0x80;
	pEm->Be_flg |= 0x44a;

	Kage_work_set(&pEm->pKage_work, 0, 0, 0, &pEm->Pos_x);
	func_8010014C(pEm);	/* retail-only call (@0x80100064) */
}

static void Em2c_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_1) {

	case 0:

		if (!pEm->Sce_free0) break;

		pEm->Routine_1++;
		*(long *)&pEm->Move_no = 0;

	case 1:

		pEm->Routine_1 += Joint_move(pEm, Kan, Seq, 0x1000);

	case 2:
		break;
	}
}

/* retail-only (0x8010014C in slot 1, 0x8010D14C in slot 0), name unknown: three effects on parts
 * 0-2, called at the end of Em2c_Init. The first vector setup is overwritten before use; retail
 * keeps its stack stores (0x8010014C). */
static void func_8010014C(ENEMY_WORK *pEm)
{
	PARTS_W *pP_ptr;
	SVECTOR Sv;

	pP_ptr = pEm->pSin_parts_ptr;
	Sv.vx = 0;
	Sv.vy = 0;
	Sv.vz = 0;
	Esp_call(0x5011600, 0, &pP_ptr[0].Workm, &Sv);
	Sv.vx = 0;
	Sv.vy = 500;
	Sv.vz = 0;
	Sv.vx = 0;
	Sv.vy = 200;
	Sv.vz = 0;
	Esp_call(0x5011600, 0, &pP_ptr[1].Workm, &Sv);
	Sv.vx = 0;
	Sv.vy = 0;
	Sv.vz = -100;
	Esp_call(0x5011200, 0, &pP_ptr[2].Workm, &Sv);
}
