/*
 * STAGE1.H - Stage 1 data table declarations (Bs size, checksum, XA)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE1.H
 */
#ifndef STAGE1_H
#define STAGE1_H
#include "TYPES.H"
// D:\Bio2\prog\STAGE1.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


extern VOID Stage1(VOID);
extern USHORT Bs_size1[30][16];		// 8011A054		// 960 bytes
extern UCHAR Bs_check_sum1[30][16];		// 8011A414		// 480 bytes
extern USHORT Xa_sector01[29][3];		// 8011A5F4		// 174 bytes

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


#endif /* STAGE1_H_H */
