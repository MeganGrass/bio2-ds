#ifndef STAGE5_H
#define STAGE5_H
#include "TYPES.H"
/*
 * STAGE5.H - Stage 5 entry point and data tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE5.H
 */
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


extern VOID Stage5(VOID);
extern USHORT Bs_size5[10][16];		// 8011A054		// 320 bytes
extern UCHAR Bs_check_sum5[10][16];		// 8011A194		// 160 bytes
extern USHORT Xa_sector05[19][3];		// 8011A234		// 114 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE5_H_H */
