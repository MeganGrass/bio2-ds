/*
 * SPL_SCE.H - Subpl scenario handler prototypes and speed tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_SCE.H
 */
#ifndef SPL_SCE_H
#define SPL_SCE_H
// D:\Bio2\prog\SPL_SCE.H
#include "TYPES.H"


/* Scenario-driven step of a sub-player (Spl) character: runs the entry of Spl_sce selected by the scenario
 * state, using the motion data Kan and Seq. */
extern VOID Spl_Scenario(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* Spl_sce[33] (DS 0x800a8d90) is defined static in SPL_SCE.C, the only file that uses it. */

/* Subpl_walk/back/ato/run are static tables in SPL_TBL.H; SPL_SCE.C has its own copy (DS 0x800a8c40) */


#endif /* SPL_SCE_H_H */
