/*
 * SLD.C - Desld streamed decompressor and memcpy16 helper
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SLD.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBPRESS.H>
#include <LIBSPU.H>
#include "SLD.H"
#include "MAIN.H"

/* Scratchpad input window (DS 0x1f800000, 0x400 bytes) and the header skip applied
 * by Desld_init to the first chunk. */
#define DESLD_SCRATCH		((UCHAR *)0x1F800000)
#define DESLD_SCRATCH_END	((UCHAR *)0x1F800400)
#define DESLD_HEADER_SKIP	6
/* forward declarations (definitions are in retail address order) */
static VOID Desld_copy(UCHAR *src, UCHAR *dst, ULONG count);


DESLD_STATE Desld_st;		/* DS 0x80010738 (Desld_st) */

/*
 * DS 0x80010000 (Desld_init)
 * Stores the destination, sets the input cursor to scratchpad + 6 (skips the
 * header of the first chunk), the end pointer, the copy base and state 0.
 */
VOID Desld_init(ULONG *pBuf)
{
	Desld_st.dst = (UCHAR *)pBuf;
	Desld_st.src = DESLD_SCRATCH + DESLD_HEADER_SKIP;	/* skip the 6-byte header of the first chunk */
	Desld_st.end = DESLD_SCRATCH_END;
	Desld_st.state = DESLD_ST_START;
}

/*
 * DS 0x80010084 (Desld_proc) + FUN_80010184 (0x354 bytes, the token decoder)
 *
 * Resumable decoder. The asm dispatches through the 5-word table at 0x80010170
 * indexed by Desld_st.state. The switch below mirrors that dispatch, with the
 * targets inferred from the code (see the header). Returns 0 when the chunk is
 * used up and the caller must refill; 0xffff on the 0xff end marker.
 *
 * Token formats:
 *   c & 0x10 == 0       match: length = (c & 0xf) + 3 (+ extra byte if nibble == 15),
 *                       offset = dst - 2048 + (((c & 0xe0) << 3) | next byte)
 *   c & 0x10 != 0       literal run: length = 16 - (c & 0xf), or 16 + next byte
 *                       when (c & 0xf) == 0
 *   c == 0xff           end of stream
 *
 * DS 0x800104d8 and 0x800104e8 are not C functions. They are the register epilogue:
 * 0x800104d8 reloads s1 and falls into 0x800104e8, which sets t7 = s6 (the return
 * continuation used by the decoder's jr t7) and reloads s0, s2..s6 from
 * Desld_st.saved_regs (+0x20..+0x38, written by DS 0x80010078). In C the compiler
 * restores the callee-saved registers and each exit is a plain return, so the
 * calls are dropped at the decoder's exit points.
 */
INT Desld_proc(VOID)
{
	UCHAR	*src;		/* t1 */
	UCHAR	*dst;		/* t3 */
	UCHAR	*end;		/* t2 */
	UCHAR	*msrc;		/* s5 */
	ULONG	c;		/* s4: control byte, then nibble */
	ULONG	len;		/* s4: copy count */
	ULONG	avail;
	ULONG	off;

	/* DS 0x80010138..0x80010144 : end, dst, src (+0x18, +0x00, +0x08) */
	end = Desld_st.end;
	dst = Desld_st.dst;
	src = Desld_st.src;

	switch (Desld_st.state) {
	case DESLD_ST_SHORT_TAIL:	/* DS 0x80010264: control UCHAR parked in +0x14 */
		c = Desld_st.pending;
		src = DESLD_SCRATCH;
		goto match;

	case DESLD_ST_LIT_TAIL:		/* DS 0x80010380: remaining literal count in +0x14 */
		len = Desld_st.pending;
		src = DESLD_SCRATCH;
		goto literal_copy;

	case DESLD_ST_MATCH_LEN:	/* DS 0x80010234: match source saved in +0x08 */
		msrc = Desld_st.src;
		src = DESLD_SCRATCH;
		goto match_extra;

	case DESLD_ST_LONG_LEN:		/* DS 0x8001035c: literal count was 16, extra UCHAR pending */
		src = DESLD_SCRATCH;
		len = 16;
		goto long_extra;

	case DESLD_ST_START:		/* DS 0x80010184 */
	default:
		break;
	}

loop:						/* DS 0x800101a4 */
	c = *src++;
	if ((c & 0x10) == 0) {
		goto short_token;		/* DS 0x800101b0 */
	}

	if (c == 0xff) {			/* DS 0x80010200 : end of stream */
		return 0xffff;
	}

	len = 16 - (c & 0x0f);		/* DS 0x800103ac..0x800103b4 : (~c & 0x1f) + 1 */
	if (len == 16) {			/* DS 0x800103b8 : nibble == 0 */
		if (src == end) {		/* DS 0x800103c4 : extra UCHAR in next chunk */
			Desld_st.dst = dst;	/* DS 0x800103d4 */
			Desld_st.state = DESLD_ST_LONG_LEN;	/* DS 0x800103e0 */
			return 0;
		}
long_extra:
		len += *src++;			/* DS 0x800103fc : count = 16 + extra */
	}

	/* DS 0x80010408 */
	avail = (ULONG)(end - src);
	if (len > avail) {			/* DS 0x8001040c : run crosses the chunk end */
		Desld_st.pending = len - avail;	/* DS 0x80010414 */
		if (avail != 0) {		/* DS 0x8001042c */
			{ UCHAR *cs = src; UCHAR *cd = dst; ULONG cn = avail; while (cn != 0) { *cd++ = *cs++; cn--; } }	/* inline copy: a leaf keeps no frame (retail Desld_proc has frame 0) */
			dst += avail;
		}
		Desld_st.dst = dst;		/* DS 0x80010460 */
		Desld_st.state = DESLD_ST_LIT_TAIL;	/* DS 0x80010450 */
		return 0;
	}

literal_copy:					/* DS 0x8001046c */
	{ UCHAR *cs = src; UCHAR *cd = dst; ULONG cn = len; while (cn != 0) { *cd++ = *cs++; cn--; } }
	dst += len;
	src += len;
	if (src != end) {			/* DS 0x80010478 */
		Desld_st.state = 0;		/* DS 0x80010488 */
		goto loop;				/* DS 0x8001048c */
	}
	/* DS 0x800104a4 : exact end of chunk. The asm does not reset +0x10 on this path
	 * (kept as found). */
	Desld_st.dst = dst;
	Desld_st.src = DESLD_SCRATCH;
	return 0;

short_token:					/* DS 0x800101b8 */
	if (src == end) {			/* control UCHAR was the last UCHAR of the chunk */
		Desld_st.pending = c;	/* DS 0x800101c0 */
		Desld_st.state = DESLD_ST_SHORT_TAIL;	/* DS 0x800101d4 */
		Desld_st.dst = dst;		/* DS 0x800101f4 */
		return 0;
	}

match:							/* DS 0x8001028c */
	off = ((c & 0xe0) << 3) | src[0];
	msrc = dst + ((LONG)off - 2048);	/* DS 0x800102a0 : s5 = t3 + (off11 - 2048) */
	c &= 0x0f;					/* DS 0x800102a4 : nibble */
	src++;						/* DS 0x800102b0 */
	if (c < 15) {				/* DS 0x800102a8 */
		len = c + 3;
		goto match_copy;
	}
	if (src == end) {			/* DS 0x800102b4 : length UCHAR in next chunk */
		Desld_st.src = msrc;	/* DS 0x800102c0 */
		Desld_st.state = DESLD_ST_MATCH_LEN;	/* DS 0x800102d0 */
		Desld_st.dst = dst;		/* DS 0x800102f4 */
		return 0;
	}
match_extra:
	len = 15 + *src++;			/* DS 0x800102fc */
	len += 3;					/* DS 0x8001030c */

match_copy:						/* DS 0x80010308 */
	{ UCHAR *cs = msrc; UCHAR *cd = dst; ULONG cn = len; while (cn != 0) { *cd++ = *cs++; cn--; } }
	dst += len;
	if (src != end) {			/* DS 0x80010310 */
		goto loop;
	}
	Desld_st.state = 0;			/* DS 0x80010324 */
	Desld_st.dst = dst;			/* DS 0x80010334 */
	Desld_st.src = DESLD_SCRATCH;	/* DS 0x8001034c */
	return 0;					/* DS 0x80010354 */
}

/*
 * DS 0x80010560 (entry) and 0x80010598 (Desld_copy). The retail entry at 0x80010560
 * computes the first block count and jumps into the unrolled block at 0x80010598
 * (retail base in Desld_st.copy_base, which this C loop does not need).
 * Copies `count` bytes forward from src to dst. The asm does this with a 32-byte
 * unrolled block (Duff's device entered at 12*k in 0x80010598) and advances both
 * cursors; the byte order is identical to this loop, so overlapping back-references
 * behave the same.
 */
static VOID Desld_copy(UCHAR *src, UCHAR *dst, ULONG count)
{
	while (count != 0) {
		*dst++ = *src++;
		count--;
	}
}

/*
 * DS 0x80010778 (memcpy16)
 * Copies 16 bytes per iteration (4 words). Loop is do/while on source < source+size,
 * so at least one 16-byte block is always copied.
 */
VOID memcpy16(ULONG *dest, ULONG *source, INT size)
{
	ULONG	*end;
	ULONG	t0, t1, t2, t3;

	end = (ULONG *)((UCHAR *)source + size);
	do {
		t0 = source[0];
		t1 = source[1];
		t2 = source[2];
		t3 = source[3];
		dest[0] = t0;
		dest[1] = t1;
		dest[2] = t2;
		dest[3] = t3;
		source += 4;
		dest += 4;
	} while ((ULONG)source < (ULONG)end);
}

/*
 * DS 0x800107b4 (setexmask)
 * PSX: a single `break 0x406` trap (exception-mask hook).
 */
VOID setexmask(VOID)
{
	trap(0x406);
}
