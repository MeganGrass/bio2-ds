#ifndef OBA_H
#define OBA_H
/*
 * OBA.H - Object vs enemy hit-test prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OBA.H
 */
#include "TYPES.H"

ULONG   Oba_ck_hit(ENEMY_WORK *pEm1, ENEMY_WORK *pEm2);             /* 0x80034d0c */
ULONG   Oba_ck_em_at(SVECTOR *pPos, LONG R, LONG H);                /* 0x8003513c (retail-only) */
ULONG   Oba_ck_only(VECTOR *pV, ENEMY_WORK *pEm1, ENEMY_WORK *pEm2);/* 0x800352d8 */
void    Oba_set_ofs(ENEMY_WORK *pEm, VECTOR *pPos);                 /* 0x80035408 */
void    Oba_ck_em(ENEMY_WORK *pEm);                                 /* 0x80035530 */
void    Oba_ck_em2(ENEMY_WORK *pEm);                                /* 0x800355c4 */
void    Sca_ck_em(ENEMY_WORK *pEm, ULONG Sc_id);                    /* 0x8003567c */


#endif /* OBA_H_H */
