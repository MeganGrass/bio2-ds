/* DIVCHK.H - DS divide sequences.
 *
 * DS divides with variable divisors by the 3-operand div/divu macro of its assembler, which expands to
 * the divide, a zero check and (signed only) an overflow check, followed by mflo/mfhi:
 *     div  num,den ; bne den,zero,L1 ; nop ; break 0x1c00 ; L1: li at,-1 ; bne den,at,L2 ; lui at,0x8000 ;
 *     bne  num,at,L2 ; nop ; break 0x1800 ; L2: mflo/mfhi res
 *     divu num,den ; bne den,zero,L1 ; nop ; break 0x1c00 ; L1: mflo/mfhi res
 * The compiler here emits its own shorter check with different break encodings (break 7 = 0x1cd), so the
 * DS sequence is written out. `break 0x1c00` and `break 0x1800` assemble to 0x0007000d and 0x0006000d,
 * the same words as in DS. Divides by constants are not affected (they use multiply-high).
 * Use DIV_S / MOD_S for signed operands, DIV_U / MOD_U for unsigned ones. res must be an lvalue.
 * Host builds fall back to the C operators. */
#ifndef DIVCHK_H
#define DIVCHK_H

#if defined(__mips__) || defined(__psx__) || defined(__R3000__)
#define DIV_S_ASM(res, num, den, get) \
	__asm__ volatile (".set noreorder\n\t.set noat\n\t" \
		"div $0,%1,%2\n\tbne %2,$0,1f\n\tnop\n\tbreak 0x1c00\n" \
		"1:\tli $1,-1\n\tbne %2,$1,2f\n\tlui $1,0x8000\n\tbne %1,$1,2f\n\tnop\n\tbreak 0x1800\n" \
		"2:\t" get " %0\n\t.set at\n\t.set reorder" \
		: "=r"(res) : "r"(num), "r"(den) : "hi", "lo")
#define DIV_U_ASM(res, num, den, get) \
	__asm__ volatile (".set noreorder\n\t.set noat\n\t" \
		"divu $0,%1,%2\n\tbne %2,$0,1f\n\tnop\n\tbreak 0x1c00\n" \
		"1:\t" get " %0\n\t.set at\n\t.set reorder" \
		: "=r"(res) : "r"(num), "r"(den) : "hi", "lo")
#define DIV_S(res, num, den)	DIV_S_ASM(res, num, den, "mflo")
#define MOD_S(res, num, den)	DIV_S_ASM(res, num, den, "mfhi")
#define DIV_U(res, num, den)	DIV_U_ASM(res, num, den, "mflo")
#define MOD_U(res, num, den)	DIV_U_ASM(res, num, den, "mfhi")
#else
#define DIV_S(res, num, den)	((res) = (num) / (den))
#define MOD_S(res, num, den)	((res) = (num) % (den))
#define DIV_U(res, num, den)	((res) = (num) / (den))
#define MOD_U(res, num, den)	((res) = (num) % (den))
#endif

#endif /* DIVCHK_H */
