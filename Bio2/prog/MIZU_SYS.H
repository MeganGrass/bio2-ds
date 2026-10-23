#ifndef MIZU_SYS_H
#define MIZU_SYS_H
/*
 * MIZU_SYS.H - Water surface / ripple system module
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */

#include "TYPES.H"


/* ------------------------------------------------------------------ */
/* Data                                                                 */
/* ------------------------------------------------------------------ */

/* DS 0x8009dca0 */ extern UCHAR  Mizu_size_tbl[15][2];     /* cells X, Z per water type   */
/* DS 0x8009dcc0 */ extern UCHAR  Mizu_tbl[15];           /* row index into struct tbl   */
/* DS 0x8009dcd0 */ extern USHORT Mizu_struct_tbl[125];   /* per-row cell presence bits  */
/* DS 0x8009ddcc */ extern UCHAR  Div_prim_coord[13][8];  /* subdivision quad coords     */

/* ------------------------------------------------------------------ */
/* Function prototypes                                                  */
/* ------------------------------------------------------------------ */

/* Water surface: Mizu_trans sorts and shows or hides the cells of one water object, Mizu_div_ck runs
 * the ripple subdivision for every active model, and Mizu_div_main does the subdivision for one model. */
void Mizu_trans(OBJ_MODEL_WORK *pOwork, ULONG toumei);
void Mizu_div_ck(OBJ_MODEL_WORK *pOwork, ULONG Size_x, ULONG Size_z);
void Mizu_div_main(OBJ_MODEL_WORK *pOwork, ULONG Size_x, ULONG Size_z, ENEMY_WORK *pEwork);


#endif /* MIZU_SYS_H_H */
