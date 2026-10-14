/*
 * SABUN.H - Enemy vertex-delta (sabun) struct and morph prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SABUN.H
 */
#ifndef SABUN_H
#define SABUN_H
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
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
void    Sa_dat_set(ENEMY_WORK *pEm, ULONG *pSa);
void    Sa_parts_mod(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_allmv2(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_parts_offset_allmv(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
void    Sa_morph_trans(ENEMY_WORK *pEm, ULONG *pTmd, ULONG *pSa_head);	/* DS 0x8004bf90 */


#endif /* SABUN_H_H */
