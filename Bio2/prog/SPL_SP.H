#ifndef SPL_SP_H
#define SPL_SP_H
/*
 * SPL_SP.H - Special enemy (Spl_sp) move prototypes and ROM tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_SP.H
 */
#include "TYPES.H"


extern VOID Spl_Sp_Move(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);


/* Spl_sp[24][8] (DS 0x800a8f64) is defined in SPL_SP.C, the only file that uses it. */

/* ROM texture-coordinate tables copied into the local arrays of Spl_sp40_00 / Spl_sp44_00. The kuchi
 * (mouth), kubi (neck) and me (eye) names give the body part. */
extern const UCHAR Spl_sp40_kuchi3_rom[33];	/* DS 0x80011a58 */
extern const UCHAR Spl_sp40_kuchi4_rom[12];	/* DS 0x80011a7c */
extern const UCHAR Spl_sp40_kubi3_rom[14];	/* DS 0x80011a88 */
extern const UCHAR Spl_sp40_kubi4_rom[5];	/* DS 0x80011a98 */
extern const UCHAR Spl_sp44_kuchi3_rom[26];	/* DS 0x80011ab4 */
extern const UCHAR Spl_sp44_kuchi4_rom[6];	/* DS 0x80011ad0 */
extern const UCHAR Spl_sp44_kubi3_rom[4];	/* DS 0x80011ad8 */
extern const UCHAR Spl_sp44_kubi4_rom[5];	/* DS 0x80011adc */
extern const UCHAR Spl_sp44_me3_rom[9];		/* DS 0x80011ae4 */
extern const UCHAR Spl_sp44_me4_rom[14];	/* DS 0x80011af0 */


#endif /* SPL_SP_H_H */
