/* DIVCHK.H - DS divide sequences.
 *
 * DS divides with variable divisors by the 3-operand div/divu macro of its assembler, which expands to
 * the divide, a zero check and (signed only) an overflow check, followed by mflo/mfhi:
 *     div  num,den ; bne den,zero,L1 ; nop ; break 0x1c00 ; L1: li at,-1 ; bne den,at,L2 ; lui at,0x8000 ;
 *     bne  num,at,L2 ; nop ; break 0x1800 ; L2: mflo/mfhi res
 *     divu num,den ; bne den,zero,L1 ; nop ; break 0x1c00 ; L1: mflo/mfhi res
 * `break 0x1c00` and `break 0x1800` assemble to 0x0007000d and 0x0006000d, the words in DS.
 * The PSY-Q 4.0 compiler (GNU C 2.7.2.SN32.3.7) writes a C division as that 3-operand macro, `div res,num,den`,
 * and ASPSX 2.56 expands it to exactly the sequence above, so the plain C operators give the DS words.
 * (GNU C 2.95.2, which the CCPSX driver picks up through PSYQ_PATH when it is not pointed at the 4.0 compiler,
 * emits its own shorter check with break 7 instead; an inline-asm copy of the DS sequence used to live here for
 * that compiler, and 2.7.2 rejects it with "fixed or forbidden register was spilled".)
 * Divides by constants are not affected (they use multiply-high).
 * Use DIV_S / MOD_S for signed operands, DIV_U / MOD_U for unsigned ones. res must be an lvalue. */
#ifndef DIVCHK_H
#define DIVCHK_H

#define DIV_S(res, num, den)	((res) = (num) / (den))
#define MOD_S(res, num, den)	((res) = (num) % (den))
#define DIV_U(res, num, den)	((res) = (num) / (den))
#define MOD_U(res, num, den)	((res) = (num) % (den))

#endif /* DIVCHK_H */
