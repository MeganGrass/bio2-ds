/* em3f.c - reconstruction of D:\Bio2\prog\EM3F.C (enemy 3F).
 * Linked into two overlays from the same source: 0x20 (at 0x80100004) and
 * 0x3E (at 0x8010d004); only the enemy-number-specific entry name differs
 * (Em13f/Em23f), selected by OVL_3E.
 * Types come from the TU's retail SYM preamble (em3f.h). */
#include "EM3F.H"

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of other translation units ----
 * EM3F.C includes no prototypes for them: Joint_move and Kage_work_set are
 * called through implicit declarations (arguments unnarrowed, implicit int);
 * those declarations open the record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static void (*Em3f_routine_0[4])() = {	/* retail: 4th entry func_801002C8 (data 0x80100378) */
	&Em3f_init,
	&Em3f_main,
	&Em3f_wait,
	&func_801002C8
};

void EM_MAIN(ENEMY_WORK *pEm)
{
	if (!(G.Stop_flg & 0x20000000)) {

	Em3f_routine_0[ pEm->Routine_0 ](pEm);
	if (pEm->Sce_free0 == 1) {	/* retail-only switch to routine 3 (@0x80100004) */
		*(long *)&pEm->Routine_0 = 3;
		pEm->Sce_free0 = 0;
	}
}	}

static void Em3f_init(ENEMY_WORK *pEm)
{

*(long *)&pEm->Routine_0 = 2;
*(long *)&pEm->Move_no = 0;

Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);

pEm->Neck_no = 0;

pEm->nOba = 0;

Kage_work_set(&pEm->pKage_work, 0, 0, 0, &pEm->Pos_x);

((KAGE_WORK9 *)pEm->pKage_work)->Be_flg = 0;

pEm->Be_flg |= 10;

pEm->Type |= 0x4000;

}

static void Em3f_main(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

switch (pEm->Routine_1) {
case 0:
	*(long *)&pEm->Move_no = 0;
		pEm->Routine_1++;
case 1:
	pP_ptr = pEm->pSin_parts_ptr;
		if (pEm->Move_cnt < 10)
			pP_ptr[1].Pos_y += 11;

	if (pEm->Move_cnt >= 10 && pEm->Move_cnt < 23)
			pP_ptr[1].Pos_y += 12;

	if (pEm->Move_cnt >= 23 && pEm->Move_cnt < 30)
			pP_ptr[1].Pos_y += 11;

	if (pEm->Move_cnt == 40)
			pP_ptr[2].Pos_y -= 12;

	if (pEm->Move_cnt >= 41 && pEm->Move_cnt < 48)
			pP_ptr[2].Pos_y -= 13;

	if (pEm->Move_cnt >= 47 && pEm->Move_cnt < 54) {
			pP_ptr[4].Pos_y += 12;
			pP_ptr[6].Pos_y += 12;
			pP_ptr[8].Pos_y += 12;
			pP_ptr[10].Pos_y += 12;
		}
	if (pEm->Move_cnt >= 54 && pEm->Move_cnt < 60) {
			pP_ptr[4].Pos_y += 11;
			pP_ptr[6].Pos_y += 11;
			pP_ptr[8].Pos_y += 11;
			pP_ptr[10].Pos_y += 11;
		}
	if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0))
			*(long *)&pEm->Routine_0 = 2;	/* retail: word store (sw @0x801002A8) */

}
}

static void Em3f_wait(ENEMY_WORK *pEm)
{

}

/* retail-only (0x801002C8 in slot 1, 0x8010D2C8 in slot 0), name unknown: routine 3, entered from
 * EM_MAIN when Sce_free0 is 1; sets the parts' final heights and plays motion 0x3B00 */
static void func_801002C8(ENEMY_WORK *pEm)
{
	register PARTS_W *pP_ptr;

	switch (pEm->Routine_1) {
	case 0:
		*(long *)&pEm->Move_no = 0x3b00;
		pP_ptr = pEm->pSin_parts_ptr;
		pP_ptr[1].Pos_y = 0x5e;
		pP_ptr[2].Pos_y = -0x67;
		pP_ptr[4].Pos_y = 0x70;
		pP_ptr[6].Pos_y = 0x70;
		pP_ptr[8].Pos_y = 0x70;
		pP_ptr[10].Pos_y = 0x70;
		pEm->Routine_1++;
	case 1:
		if (Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0))
			*(long *)&pEm->Routine_0 = 2;
	}
}
