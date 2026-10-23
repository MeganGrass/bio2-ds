/* em3e.c - reconstruction of D:\Bio2\prog\EM3E.C (enemy 3E).
 * Linked into two overlays from the same source: 0x1F (at 0x80100000) and
 * 0x3D (at 0x8010d000); only the enemy-number-specific entry name differs
 * (Em13e/Em23e), selected by OVL_3D.
 * Types come from the TU's retail SYM preamble (em3e.h).
 * Routine_0..3 and Move_no..Mplay_flg are written together as one word
 * (*(long *)&...), as the retail word stores show. */
#include "EM3E.H"

/* ---- objects defined in other translation units ---- */
extern GLOBAL G;

/* ---- functions of other translation units ----
 * Joint_move and Kage_work_set are called through an implicit declaration
 * (arguments unnarrowed, implicit int); the implicit declarations open the
 * record-less blocks of the retail SYM. */

/* ---- functions of this translation unit ---- */

/* ---- data ---- */
static void (*Em3e_routine_0[4])() = { &Em3e_init, &Em3e_move, &Em3e_wait, &Em3e_sce };

void EM_MAIN(ENEMY_WORK *pEm)
{
	if (G.Stop_flg & 0x20000000) return;

	Em3e_routine_0[ pEm->Routine_0 ](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

}

static void Em3e_init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	*(long *)&pEm->Routine_0 = 2;
	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);

	pEm->Neck_no = 0;

	pEm->nOba = 0;

	Kage_work_set(&pEm->pKage_work, 0, 0, 0, &pEm->Pos_x);

	((KAGE_WORK *)pEm->pKage_work)->Be_flg = 0;

	pEm->Be_flg |= 0xA;

	pEm->Type |= 0x4000;

	OVL_FREE(pEm)[0] = 0;
	OVL_FREE(pEm)[1] = 0;
	OVL_FREE(pEm)[2] = 0;
	OVL_FREE(pEm)[3] = 0;
	OVL_FREE(pEm)[4] = 0;
	OVL_FREE(pEm)[5] = 0;
	OVL_FREE(pEm)[6] = 0;
	OVL_FREE(pEm)[7] = 0;
	OVL_FREE(pEm)[8] = 0;
	OVL_FREE(pEm)[9] = 0;
}

static void Em3e_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	switch (pEm->Routine_1) {
	case 10:
		OVL_FREE(pEm)[0] = 0;
		OVL_FREE(pEm)[1] = 1;
		*(long *)&pEm->Move_no = 0x70000;
		pEm->Routine_1 = 12;
		goto move2;
	case 11:
		OVL_FREE(pEm)[0] = 1;
		OVL_FREE(pEm)[1] = 0;
		*(long *)&pEm->Move_no = 0x70001;
		pEm->Routine_1 = 12;
	case 12:
	move2:

		if (!Joint_move(pEm, Kan, Seq, (OVL_FREE(pEm)[0] << 16) | 0x200)) break;
		*(long *)&pEm->Move_no = OVL_FREE(pEm)[1] + 0x70000;
		pEm->Routine_1 = 13;
	case 13:
	move:

		if (Joint_move(pEm, Kan, Seq, (OVL_FREE(pEm)[0] << 16) | 0x200)) {

			pEm->Routine_0 = 2;	}	break;
	default:
		OVL_FREE(pEm)[0] = pEm->Routine_1 & 1;
		*(long *)&pEm->Move_no = (pEm->Routine_1 >> 1) + 0x70000;
		pEm->Routine_1 = 13;
		goto move;
	}
}

static void Em3e_wait(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}

static void Em3e_sce(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

}
