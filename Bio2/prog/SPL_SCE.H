/*
 * SPL_SCE.H - Subpl scenario handler prototypes and speed tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_SCE.H
 */
#ifndef SPL_SCE_H
#define SPL_SCE_H
// D:\Bio2\prog\SPL_SCE.H
#include "TYPES.H"


extern VOID Spl_Scenario(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* Spl_sce[33] (DS 0x800a8d90) is defined static in SPL_SCE.C, the only file that uses it. */

/* Subpl walk/back/ato/run speed tables, defined in SPL.C */
extern UCHAR Subpl_walk[32][2];		/* DS 0x800a8268 */
extern UCHAR Subpl_back[32][2];		/* DS 0x800a82a8 */
extern UCHAR Subpl_ato[32][2];		/* DS 0x800a8328 */
extern UCHAR Subpl_run[32][2];		/* DS 0x800a8368 */


#endif /* SPL_SCE_H_H */
