#ifndef SLD_H
#define SLD_H
// D:\Bio2\prog\SLD.H
#include "TYPES.H"


/*
 * SLD decompressor. Unpacks compressed TIM data into
 * Bg_tim_buf. Input is read 0x400 bytes at a time from the scratchpad window
 * DS 0x1f800000 (refilled by the caller via memcpy16), output goes to main RAM.
 *
 * Desld_proc returns 0 when it needs the next 0x400-byte chunk, and 0xffff on
 * the 0xff end-of-stream marker.
 */

/* Resume states stored in DESLD_STATE.state (byte offsets into the DS jump table
 * at 0x80010170; targets inferred from the code, table itself not readable). */
#define DESLD_ST_START       0x00	/* DS 0x80010184 */
#define DESLD_ST_SHORT_TAIL  0x04	/* DS 0x80010264 */
#define DESLD_ST_LIT_TAIL    0x08	/* DS 0x80010380 */
#define DESLD_ST_MATCH_LEN   0x0c	/* DS 0x80010234 */
#define DESLD_ST_LONG_LEN    0x10	/* DS 0x8001035c */

/* Decompressor state block. DS 0x80010738 (Ghidra: desld_buf, T0).
 * Offsets are the DS offsets from that base. Pointers are 32-bit (PSX). */
struct DesldState {
	UCHAR	*dst;		/* +0x00 output cursor in main RAM (set by Desld_init) */
	ULONG	reserved04;	/* +0x04 not referenced */
	UCHAR	*src;		/* +0x08 input cursor in scratchpad; reused as the saved
	                     *       match source while state is DESLD_ST_MATCH_LEN */
	ULONG	reserved0c;	/* +0x0c written by DS 0x80010078 only (no callers) */
	ULONG	state;		/* +0x10 resume state (see DESLD_ST_*) */
	ULONG	pending;	/* +0x14 parked control byte (SHORT_TAIL) or remaining
	                     *       literal count (LIT_TAIL) */
	UCHAR	*end;		/* +0x18 end of the input chunk (0x1f800400) */
	ULONG	copy_base;	/* +0x1c retail jump base of the copy routine (DS 0x80010598); the C loop does not use it, so it is not set */
	ULONG	saved_regs[8];	/* +0x20..+0x3c saved s0..s7 (asm register save only) */
};

extern DESLD_STATE Desld_st;	/* DS 0x80010738 */

/* DS 0x80010000 */
VOID Desld_init(ULONG *pBuf);

/* DS 0x80010084 (called from Bg_draw, see MAIN.C) */
INT Desld_proc(VOID);

/* DS 0x800107b4 */
VOID setexmask(VOID);



VOID memcpy16(ULONG *dest, ULONG *source, INT size);

#endif /* SLD_H */
