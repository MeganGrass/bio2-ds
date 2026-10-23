/*
 * SABUN.H - Enemy vertex-delta (sabun) struct and morph prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SABUN.H
 */
#ifndef SABUN_H
#define SABUN_H
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
/* One morph vertex: org_* is the base vertex and sa_* is its morph delta (the "sabun" difference applied
 * to the model each frame). */
struct tagSAB {
	SHORT org_vx;
	SHORT org_vy;
	SHORT org_vz;
	SHORT sa_vx;
	SHORT sa_vy;
	SHORT sa_vz;
};
/* D:\Bio2\prog\SABUN.H */



/* Function prototypes */
/* Morph helpers: Sa_dat_set loads the delta table for an enemy, the Sa_parts_* functions apply the deltas
 * to the parts (all parts, or with an offset), and Sa_morph_trans runs the morph for one TMD. */
void    Sa_dat_set(ENEMY_WORK *pEm, ULONG *pSa);
void    Sa_parts_mod(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_allmv2(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_offset_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_morph_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG *pTmd, ULONG *pSa_head);	/* DS 0x8004bf90 */


#endif /* SABUN_H_H */
