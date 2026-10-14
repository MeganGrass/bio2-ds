/*
 * STAGE3.H - Stage 3 entry point and data table externs
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE3.H
 */
#ifndef STAGE3_H
#define STAGE3_H
#include "TYPES.H"
// D:\Bio2\prog\STAGE3.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


extern VOID Stage3(VOID);
extern USHORT Bs_size3[14][16];		// 8011A054		// 448 bytes
extern UCHAR Bs_check_sum3[14][16];		// 8011A214		// 224 bytes
extern USHORT Xa_sector03[70][3];		// 8011A2F4		// 420 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE3_H_H */
