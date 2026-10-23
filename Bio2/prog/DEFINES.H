/*
 * DEFINES.H - Common constants and fixed RAM buffer addresses
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\DEFINES.H
 */
#ifndef DEFINES_H
#define DEFINES_H
#include "TYPES.H"


#define TRUE		1
#define FALSE		0

#define ON			1
#define OFF			0

#define ENABLED		1
#define DISABLED	0

/* Fixed RAM addresses of the shared load buffers: RDTFile holds room (RDT) data, BSSFile and TIMFile
 * are the 320x240 background and texture buffers, LZin is the compressed TIM input (same address as
 * TIMFile), and LZout is where a decompressed TIM is written. PLDFile and VBFile share 0x801BFA14. */
#define SCRATCHPAD					0x1F800000	// Scratchpad Memory (0x400 bytes)

#define RDTFile						0x8011A000	// RDT Buffer

#define BSSFile						0x80190000	// 320x240 BS Buffer

#define TIMFile						0x80198000	// 320x240 TIM Buffer

#define LZin						0x80198000	// Compressed TIM Buffer (custom, non-vanilla/retail)

#define LZout						0x801A5800	// Decompressed TIM Buffer

#define PLDFile						0x801BFA14	// PLD Buffer

#define VBFile						0x801BFA14	// VB Buffer


#endif /* DEFINES_H_H */
