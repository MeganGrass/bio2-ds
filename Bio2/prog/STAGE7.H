/*
 * STAGE7.H - Stage 7 entry point and boot/XA sector table externs
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE7.H
 */
#ifndef STAGE7_H
#define STAGE7_H
#include "TYPES.H"
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


extern VOID Stage7(VOID);
extern USHORT Bs_size7[5][16];		// 8011A054		// 160 bytes
extern UCHAR Bs_check_sum7[5][16];		// 8011A0F4		// 80 bytes
extern USHORT Xa_sector07[16][3];		// 8011A144		// 96 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE7_H_H */
