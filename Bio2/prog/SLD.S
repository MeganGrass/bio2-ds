# SLD.S - Desld streamed decompressor, memcpy16 and setexmask (SLUS-00748)
# Resident Evil 2 (SLUS-00748) decompilation
# Source: D:\Bio2\prog\SLD.S
# Assembled with ASPSX (-G0) and linked at 0x80010000 as the first object of the main image.
# The code and the overlay table below are in .rdata, which Makefile.link places before .text (retail order).
# Source of truth: retail bytes of SLUS_007.48 (file offset = vaddr - 0x80010000 + 0x800).
# Instructions are written with their MIPS mnemonics. li/move pseudo-ops are emitted as .word
# so the exact encoding is kept. Data inside the code (jump table, Desld_st) is emitted as .word/.space.

#
# Desld is a streamed LZ-style decompressor. Desld_init starts a stream. Desld_proc decodes the
# 0x400-byte scratchpad window at 0x1f800000 into main RAM and returns 0 when it needs the next
# window, or 0xffff at the 0xff end marker. Its state is kept in Desld_st (DS 0x80010738, struct
# DesldState in SLD.H).
# STRICT: the code at 0x80010000-0x800107bf is byte-identical to retail (1984 bytes).
	.set	noreorder
	.set	noat

	.rdata


# DS 0x80010000 (Desld_init)
# Starts a stream. dst (a0) goes to Desld_st+0x00, src is set to scratchpad + 6 (skipping the 6-byte
# header of the first chunk), end to scratchpad + 0x400, the copy base to DS 0x80010598 (+0x1c), and
# state to 0 (DESLD_ST_START).
	.globl	Desld_init
Desld_init:
	lui $8,0x8001
	ori $8,$8,0x738
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $4,0x0($1)
	lui $9,0x1f80
	ori $9,$9,0x0
	addiu $10,$9,0x6
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	sw $10,0x0($1)
	addiu $10,$9,0x400
	lui $1,0x0
	addiu $1,$1,0x18
	addu $1,$1,$8
	sw $10,0x0($1)
	lui $10,0x8001
	addiu $10,$10,0x598
	lui $1,0x0
	addiu $1,$1,0x1c
	addu $1,$1,$8
	sw $10,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $0,0x0($1)
	jr $31
	lui $1,0x0

# DS 0x80010078 (pre_desld)
# Second entry point with no caller in this tree. Expects $8 to hold the Desld_st base and clears
# Desld_st+0x0c, which the decoder does not read.
	.globl	pre_desld
pre_desld:
	addiu $1,$1,0xc
	addu $1,$1,$8
	sw $0,0x0($1)

# DS 0x80010084 (Desld_proc)
# Decoder entry, called by MAIN.C after each 0x400-byte window is copied to the scratchpad. Saves s0 through
# s7 into Desld_st.saved_regs (+0x20 to +0x3c), keeps the return address in s6, loads s3 from Desld_st+0x1c
# (the copy base), sets s2 to 32 (the block size), loads the dst, src and end cursors, and dispatches on
# Desld_st.state through the table at DS 0x80010170.
	.globl	Desld_proc
Desld_proc:
	lui $8,0x8001
	ori $8,$8,0x738
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x20
	addu $1,$1,$8
	sw $16,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x24
	addu $1,$1,$8
	sw $17,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x28
	addu $1,$1,$8
	sw $18,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x2c
	addu $1,$1,$8
	sw $19,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x30
	addu $1,$1,$8
	sw $20,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x34
	addu $1,$1,$8
	sw $21,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x38
	addu $1,$1,$8
	sw $22,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x3c
	addu $1,$1,$8
	sw $23,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x1c
	addu $1,$1,$8
	lw $19,0x0($1)
	addu $22,$0,$31
	addiu $18,$0,0x20
	lui $1,0x0
	addiu $1,$1,0x18
	addu $1,$1,$8
	lw $10,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	lw $12,0x0($1)
	lui $13,0x8001
	addiu $13,$13,0x170
	addu $13,$12,$13
	lw $12,0x0($13)
	nop
	jr $12
	addiu $17,$0,-256
# Resume table at DS 0x80010170, indexed by Desld_st.state (0, 4, 8, 12, 16). The entries are, in order:
# DESLD_ST_START, DESLD_ST_SHORT_TAIL, DESLD_ST_LIT_TAIL, DESLD_ST_MATCH_LEN, DESLD_ST_LONG_LEN (SLD.H).
	.word	0x80010184
	.word	0x80010264
	.word	0x80010380
	.word	0x80010234
	.word	0x8001035C

# DS 0x80010184 (FUN_80010184)
# Token decoder, the body of Desld_proc (0x354 bytes in retail). Registers: t1 input cursor, t2 chunk
# end, t3 output cursor, s4 control byte or count, s5 match source. Token formats:
#   bit 4 clear: back-reference. Length is (c & 0xf) + 3, plus 15 + one extra byte when the nibble is 15.
#                Source is the output cursor + (off - 2048), where off = ((c & 0xe0) << 3) | next byte.
#   bit 4 set, c != 0xff: literal run of 16 - (c & 0xf) bytes; when the nibble is 0 the count is
#                16 + one extra byte.
#   c == 0xff: end of stream, returns 0xffff.
# When the chunk ends inside a token, the position is saved in Desld_st, the state is set, and 0 is returned.
	.globl	FUN_80010184
FUN_80010184:
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	lw $9,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
L_800101A4:
	lbu $20,0x0($9)
	addiu $9,$9,0x1
	andi $13,$20,0x10
	bne $13,$0,L_80010200
	nop
	bne $9,$10,L_8001028C
	nop
	lui $1,0x0
	addiu $1,$1,0x14
	addu $1,$1,$8
	sw $20,0x0($1)
	jal FUN_800104d8
	addiu $13,$0,0x4
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $13,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	jr $15
	addiu $2,$0,0x0
L_80010200:
	addiu $12,$0,0xFF
	bne $20,$12,L_800103AC
	nop
	ori $1,$0,0xffff
	add $2,$0,$1
	jal FUN_800104e8
	lui $1,0x0
	addiu $1,$1,0x24
	addu $1,$1,$8
	lw $17,0x0($1)
	jr $15
	ori $1,$0,0xffff
	add $2,$0,$1
	lui $9,0x1f80
	ori $9,$9,0x0
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	lw $21,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
	j L_800102FC
	addiu $20,$0,0xF
	lui $9,0x1f80
	ori $9,$9,0x0
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x14
	addu $1,$1,$8
	lw $20,0x0($1)
L_8001028C:
	lbu $13,0x0($9)
	andi $12,$20,0xe0
	or $12,$12,$17
	sll $12,$12,0x3
	or $13,$12,$13
	addu $21,$11,$13
	andi $20,$20,0xf
	sltiu $15,$20,0xf
	bne $0,$15,L_80010308
	addiu $9,$9,0x1
	bne $9,$10,L_800102FC
	nop
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	sw $21,0x0($1)
	jal FUN_800104d8
	addiu $13,$0,0xC
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $13,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	jr $15
	addiu $2,$0,0x0
L_800102FC:
	lbu $13,0x0($9)
	addiu $9,$9,0x1
	addu $20,$13,$20
L_80010308:
	jal FUN_80010560
	addiu $20,$20,0x3
	bne $9,$10,L_800101A4
	nop
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $0,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	jal FUN_800104d8
	lui $9,0x1f80
	ori $9,$9,0x0
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	sw $9,0x0($1)
	jr $15
	addiu $2,$0,0x0
	lui $9,0x1f80
	ori $9,$9,0x0
	addiu $21,$9,0x1
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
	j L_800103FC
	addiu $20,$0,0x10
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	lw $11,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x14
	addu $1,$1,$8
	lw $20,0x0($1)
	lui $9,0x1f80
	j L_8001046C
	ori $9,$9,0x0
L_800103AC:
	ori $20,$20,0xffe0
	xori $20,$20,0xffff
	addiu $20,$20,0x1
	addiu $12,$20,-0x10
	bne $12,$0,L_80010408
	nop
	bne $9,$10,L_800103FC
	nop
	jal FUN_800104d8
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	addiu $11,$0,0x10
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $11,0x0($1)
	jr $15
	addiu $2,$0,0x0
L_800103FC:
	lbu $13,0x0($9)
	addiu $9,$9,0x1
	addu $20,$20,$13
L_80010408:
	addu $12,$20,$9
	subu $13,$12,$10
	blez $13,L_8001046C
	subu $12,$12,$10
	lui $1,0x0
	addiu $1,$1,0x14
	addu $1,$1,$8
	sw $12,0x0($1)
	subu $20,$10,$9
	beq $10,$9,L_8001043C
	add $21,$9,$0
	jal FUN_80010560
	nop
L_8001043C:
	jal FUN_800104d8
	addiu $12,$0,0x8
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $12,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	jr $15
	addiu $2,$0,0x0
L_8001046C:
	jal FUN_80010560
	add $21,$9,$0
	addu $9,$9,$20
	beq $9,$10,L_800104A4
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $0,0x0($1)
	j L_800101A4
	nop
	lui $1,0x0
	addiu $1,$1,0x10
	addu $1,$1,$8
	sw $0,0x0($1)
L_800104A4:
	lui $1,0x0
	addiu $1,$1,0x0
	addu $1,$1,$8
	sw $11,0x0($1)
	jal FUN_800104d8
	lui $9,0x1f80
	ori $9,$9,0x0
	lui $1,0x0
	addiu $1,$1,0x8
	addu $1,$1,$8
	sw $9,0x0($1)
	jr $15
	addiu $2,$0,0x0

# DS 0x800104d8 (FUN_800104d8)
# Exit path of the decoder. Loads s1 from Desld_st.saved_regs[1] (+0x24) and falls into FUN_800104e8.
	.globl	FUN_800104d8
FUN_800104d8:
	lui $1,0x0
	addiu $1,$1,0x24
	addu $1,$1,$8
	lw $17,0x0($1)

# DS 0x800104e8 (FUN_800104e8)
# Restore and return. Sets t7 to the current s6, which holds the return address that Desld_proc saved
# from ra. The decoder's exits use "jr t7" to return. Then restores s0 and s2 through s6 from
# Desld_st.saved_regs (+0x20 to +0x38) and returns through ra.
	.globl	FUN_800104e8
FUN_800104e8:
	addu $15,$0,$22
	lui $1,0x0
	addiu $1,$1,0x20
	addu $1,$1,$8
	lw $16,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x28
	addu $1,$1,$8
	lw $18,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x2c
	addu $1,$1,$8
	lw $19,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x30
	addu $1,$1,$8
	lw $20,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x34
	addu $1,$1,$8
	lw $21,0x0($1)
	lui $1,0x0
	addiu $1,$1,0x38
	addu $1,$1,$8
	lw $22,0x0($1)
	jr $31
	lui $1,0x0
# The next three words are not disassembled by Ghidra. They are a restore of s7 from Desld_st+0x3c
# (addiu, addu, lw). They follow the return of FUN_800104e8, so no return path runs them. Emitted as .word.
	.word	0x2421003C	# no Ghidra instruction (data)
	.word	0x00280821	# no Ghidra instruction (data)
	.word	0x8C370000	# no Ghidra instruction (data)

# DS 0x80010560 (FUN_80010560)
# Loop head for Desld_copy. s0 is the number of bytes still to copy (set from s4 on entry), s2 is the
# block size (32 bytes), and s3 is the jump base of the unrolled copy. Each pass works out how many
# bytes it copies, then jumps into Desld_copy at the matching byte. Desld_copy comes back to the loop
# head while s0 is not zero.
	.globl	FUN_80010560
FUN_80010560:
	addu $16,$20,$0
L_80010564:
	addu $12,$16,$0
	subu $14,$12,$18
	bltz $14,L_8001057C
	subu $14,$18,$12
	addiu $14,$0,0x0
	addiu $12,$18,0x0
L_8001057C:
	sll $15,$14,0x3
	sll $13,$14,0x2
	addu $15,$15,$13
	addu $15,$19,$15
	subu $21,$21,$14
	jr $15
	subu $11,$11,$14

# DS 0x80010598 (Desld_copy)
# Unrolled byte copy: 32 lbu/sb pairs from s5 (source) to t3 (destination). Each byte takes 12 bytes of
# code, so the entry point selects the first byte to copy. Afterwards the source and destination advance
# by the bytes copied and s0 drops by the block size. The copy is forward, one byte at a time, so an
# overlapping back-reference gives the same result as a byte loop.
	.globl	Desld_copy
Desld_copy:
	lbu $13,0x0($21)
	nop
	sb $13,0x0($11)
	lbu $13,0x1($21)
	nop
	sb $13,0x1($11)
	lbu $13,0x2($21)
	nop
	sb $13,0x2($11)
	lbu $13,0x3($21)
	nop
	sb $13,0x3($11)
	lbu $13,0x4($21)
	nop
	sb $13,0x4($11)
	lbu $13,0x5($21)
	nop
	sb $13,0x5($11)
	lbu $13,0x6($21)
	nop
	sb $13,0x6($11)
	lbu $13,0x7($21)
	nop
	sb $13,0x7($11)
	lbu $13,0x8($21)
	nop
	sb $13,0x8($11)
	lbu $13,0x9($21)
	nop
	sb $13,0x9($11)
	lbu $13,0xa($21)
	nop
	sb $13,0xa($11)
	lbu $13,0xb($21)
	nop
	sb $13,0xb($11)
	lbu $13,0xc($21)
	nop
	sb $13,0xc($11)
	lbu $13,0xd($21)
	nop
	sb $13,0xd($11)
	lbu $13,0xe($21)
	nop
	sb $13,0xe($11)
	lbu $13,0xf($21)
	nop
	sb $13,0xf($11)
	lbu $13,0x10($21)
	nop
	sb $13,0x10($11)
	lbu $13,0x11($21)
	nop
	sb $13,0x11($11)
	lbu $13,0x12($21)
	nop
	sb $13,0x12($11)
	lbu $13,0x13($21)
	nop
	sb $13,0x13($11)
	lbu $13,0x14($21)
	nop
	sb $13,0x14($11)
	lbu $13,0x15($21)
	nop
	sb $13,0x15($11)
	lbu $13,0x16($21)
	nop
	sb $13,0x16($11)
	lbu $13,0x17($21)
	nop
	sb $13,0x17($11)
	lbu $13,0x18($21)
	nop
	sb $13,0x18($11)
	lbu $13,0x19($21)
	nop
	sb $13,0x19($11)
	lbu $13,0x1a($21)
	nop
	sb $13,0x1a($11)
	lbu $13,0x1b($21)
	nop
	sb $13,0x1b($11)
	lbu $13,0x1c($21)
	nop
	sb $13,0x1c($11)
	lbu $13,0x1d($21)
	nop
	sb $13,0x1d($11)
	lbu $13,0x1e($21)
	nop
	sb $13,0x1e($11)
	lbu $13,0x1f($21)
	nop
	sb $13,0x1f($11)
	addu $21,$21,$14
	addu $21,$21,$12
	addu $11,$11,$12
	subu $16,$16,$12
	bne $16,$0,L_80010564
	addu $11,$11,$14
	jr $31
	nop
	.space	64	# Desld_st, zero in retail (DS 0x80010738)

# DS 0x80010778 (memcpy16)
# memcpy16(dest $4, source $5, size $6 bytes). Copies four words per pass and stops when the source
# pointer reaches source + size. The test follows the copy, so at least one block is copied. size is
# expected to be a non-zero multiple of 16.
	.globl	memcpy16
memcpy16:
	addu $6,$5,$6
L_8001077C:
	lw $8,0x0($5)
	lw $9,0x4($5)
	lw $10,0x8($5)
	lw $11,0xc($5)
	sw $8,0x0($4)
	addiu $5,$5,0x10
	sw $9,0x4($4)
	sw $10,0x8($4)
	sw $11,0xc($4)
	sltu $2,$5,$6
	bne $2,$0,L_8001077C
	addiu $4,$4,0x10
	jr $31
	nop

# DS 0x800107B4 (setexmask)
# Exception-mask hook: break 0x406, then return.
	.globl	setexmask
setexmask:
	break 0x406
	jr $31
	nop

# Overlay load-address table, DS 0x800107c0-0x80010817 (22 words). It follows the SLD code in
# the image, which is where retail's .rdata starts. Words 0-9 and 18 are read by no code in
# this tree; word 21 holds the start of .sbss, which this link does not name.
# Words 10-17 and 19-20 are the overlay load-address cells read by MONITOR.C and STAGE6.C.
	.globl	Block_adr
Block_adr:		# DS 0x800107c0
	.word	0x80200000
	.word	0x80200000	# DS 0x800107c4, Floor_adr
	.word	0x80200000	# DS 0x800107c8, T_Psp_adr
	.word	0x80200000	# DS 0x800107cc, Light_adr
	.word	0x80200000	# DS 0x800107d0, Sc_atari_adr
	.word	0x80200000	# DS 0x800107d4, T_Snd_adr
	.word	0x80200000	# DS 0x800107d8, T_Esp_adr
	.word	0x80200000	# DS 0x800107dc, T_em_set_adr
	.word	0x80200000	# DS 0x800107e0, T_sce_at_adr
	.word	0x80200000	# DS 0x800107e4, no name

	.globl	pTitle_adr
pTitle_adr:
	.word	0x80190000	# DS 0x800107e8
	.globl	Diedemo_adr
Diedemo_adr:
	.word	0x80190000	# DS 0x800107ec
	.globl	pEnding_adr
pEnding_adr:
	.word	0x8011a000	# DS 0x800107f0
	.globl	pMemCard_adr
pMemCard_adr:
	.word	0x801bfa18	# DS 0x800107f4
	.globl	pSelect_adr
pSelect_adr:
	.word	0x801bfa18	# DS 0x800107f8
	.globl	pConfig_adr
pConfig_adr:
	.word	0x801bfa18	# DS 0x800107fc
	.globl	pOpening_adr
pOpening_adr:
	.word	0x801bfa18	# DS 0x80010800
	.globl	Stage6_main_ptr
Stage6_main_ptr:
	.word	0x00000000	# DS 0x80010804, written by STAGE6.C at run time
	.word	0x80703b50	# DS 0x80010808, no name
	.globl	pResult_adr
pResult_adr:
	.word	0x80158000	# DS 0x8001080c
	.globl	pStage_adr
pStage_adr:
	.word	0x8011a000	# DS 0x80010810
	.word	0x800c3a48	# DS 0x80010814, start of .sbss in retail
