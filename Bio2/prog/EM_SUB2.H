#ifndef EM_SUB2_H
#define EM_SUB2_H
/*
 * EM_SUB2.H - Enemy subroutines 2
 * Resident Evil 2 Dual Shock (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\EM_SUB2.C
 */

#include "TYPES.H"


/* ------------------------------------------------------------------ */
/* Enemy motion playback, steering and parts-direction helpers from EM_SUB2.C. The entries are in
 * retail address order; the addresses in the trailing comments are the retail SLUS-00748 locations. */
/* Prototypes (DS address order)                                        */
/* ------------------------------------------------------------------ */

ULONG	Approach(VECTOR *pPos, VECTOR *pDest, SHORT Spd);						/* 8001a0e0 */
ULONG	Approach_one(LONG *pX, LONG Dx, SHORT Spd);								/* 8001a1e0 */
ULONG	Mot_play(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg);			/* 8001a240 */
ULONG	Mot_play2(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg);			/* 8001a2a4 */
ULONG	Mot_play_init(ENEMY_WORK *pEm, ULONG Routine_bak);						/* 8001a308 */
ULONG	Mot_play_sp(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg);			/* 8001a330 */
ULONG	Mot_play2_sp(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg);		/* 8001a3e8 */
ULONG	Mot_play2_spxz(ENEMY_WORK *pEm, ULONG No, ULONG Hokan, ULONG Flg);		/* 8001a4a0 */
void	Em_init_super(ENEMY_WORK *pEm);											/* 8001a558 */
ULONG	Branch(ULONG N, ULONG A, ULONG B, ULONG C);								/* 8001a58c */
ULONG	Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppWk, ULONG Id, ULONG Max);		/* 8001a5c0 */
void	Goto00_super(ENEMY_WORK *pEm, VECTOR *pVec, SHORT Add_dir);				/* 8001a644 */
void	Goto01_super(ENEMY_WORK *pEm, LONG Dir, SHORT Add_dir);					/* 8001a858 */
void	Get_parts_world_dir(ENEMY_WORK *pEm, PARTS_W *pParts, SVECTOR *pVec);	/* 8001a9bc */


#endif /* EM_SUB2_H_H */
