/*
 * STAGE2.H - Stage 2 entry point and data declarations
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE2.H
 */
#ifndef STAGE2_H
#define STAGE2_H
#include "TYPES.H"
// D:\Bio2\prog\STAGE2.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


extern VOID Stage2(VOID);
extern USHORT Bs_size2[28][16];		// 8011A054		// 896 bytes
extern UCHAR Bs_check_sum2[28][16];		// 8011A3D4		// 448 bytes
extern USHORT Xa_sector02[33][3];		// 8011A594		// 198 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE2_H_H */
