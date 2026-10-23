/*
 * STAGE4.H - Stage 4 entry point and background/XA data tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE4.H
 */
#ifndef STAGE4_H
#define STAGE4_H
#include "TYPES.H"
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


/* Stage 4 setup routine and its data tables (defined in STAGE4.C). Sizes in the trailing comments are bytes. */
extern VOID Stage4(VOID);
extern USHORT Bs_size4[17][16];		// 8011A054		// 544 bytes
extern UCHAR Bs_check_sum4[17][16];		// 8011A274		// 272 bytes
extern USHORT Xa_sector04[42][3];		// 8011A384		// 252 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE4_H_H */
