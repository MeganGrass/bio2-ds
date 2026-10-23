/*
 * SPL_AUTO.H - Spl/Aida/Sherry AI helper externs and motion tables
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL_AUTO.H
 */
#ifndef SPL_AUTO_H
#define SPL_AUTO_H

#include "TYPES.H"

extern VOID Spl_Auto(register ENEMY_WORK *pEm, register ULONG Kan, register ULONG Seq);

/* ---- types migrated from data.h ---- */

/* Packed routine word: Routine_0 = bits 0-7, Routine_1 = 8-15, Routine_2 = 16-23, Routine_3 = 24-31 */
#define SPL_SET_ROUTINE(p, v)	(*(ULONG *)&(p)->Routine_0 = (ULONG)(v))

/* Oma / Aida helpers used by the Spl AI */
extern ULONG Oma_Spl_updown_ck(register ENEMY_WORK *pEm);
extern VOID Aida_Jh_move(register ENEMY_WORK *pEm);
extern SHORT Up_add_Spl_tbl[];		/* index Id+0x10 */
extern SHORT Down_add_Spl_tbl[];	/* index Id+0x10 */
extern SHORT Down_add_tbl[];		/* index Id+8 */

/* Motion ROM tables copied into local arrays by the Aida / Sherry routines */
extern const USHORT Spl_mv_taiki_rom[3];	/* DS 0x8001195c */
extern const SHORT Spl_mv01_rom[3];		/* DS 0x80011964 */
extern const UCHAR Spl_mv02_rom[3];		/* DS 0x8001196c */
extern const UCHAR Spl_mv03_rom[3];		/* DS 0x80011970 */
extern const SHORT Spl_mv05_t1_rom[5];		/* DS 0x80011974 */
extern const SHORT Spl_mv05_t2_rom[5];		/* DS 0x80011980 */

/* Dispatch tables (RAM, filled by the game); declared here, defined elsewhere. Aida_Move_type and
 * Sherry_Move_type index the per-move-type branch tables (mv_br_type00..0a), which pick the motion for
 * each AI state. The St_pos tables give the starting position for each character. */
extern VOID *Aida_Move_type[11];
extern VOID *Aida_mv_br_type00[];
extern VOID *Aida_mv_br_type01[];
extern VOID *Aida_mv_br_type02[];
extern VOID *Aida_mv_br_type03[];
extern VOID *Aida_mv_br_type04[];
extern VOID *Aida_mv_br_type05[];
extern VOID *Aida_mv_br_type06[];
extern VOID *Aida_mv_br_type07[];
extern VOID *Aida_mv_br_type08[];
extern VOID *Aida_mv_br_type09[];
extern VOID *Aida_mv_br_type0a[];
extern VOID *Aida_mv_type00[];
extern VOID *Aida_mv05_w1_tbl[];
extern VOID *Aida_mv05_w1_br_tbl[];
extern VOID *Aida_em_at00_pl[];
extern VOID *Aida_em_at00_spl[];
extern VOID *Aida_em_at01[];
extern VOID *Aida_em_at05[];
extern UCHAR E_A_at_tbl[48];		/* DS 0x800a8914, Aida attack selector, indexed by enemy Id - 0x10 */
extern UCHAR Aida_St_pos[11];
extern VOID *Sherry_Move_type[11];
extern VOID *Sherry_mv_br_type00[];
extern VOID *Sherry_mv_br_type01[];
extern VOID *Sherry_mv_br_type02[];
extern VOID *Sherry_mv_br_type03[];
extern VOID *Sherry_mv_br_type04[];
extern VOID *Sherry_mv_br_type05[];
extern VOID *Sherry_mv_br_type06[];
extern VOID *Sherry_mv_br_type07[];
extern VOID *Sherry_mv_br_type08[];
extern VOID *Sherry_mv_br_type09[];
extern VOID *Sherry_mv_br_type0a[];
extern SHORT Sherry_St_pos[];

/* Routines */
extern VOID Spl_Root_ck_sys(register ENEMY_WORK *pEm);
extern VOID Aida_before(register ENEMY_WORK *pEm);
extern VOID Aida_after(register ENEMY_WORK *pEm);
extern ULONG Spl_soba_pos_set(register SHORT ll, register SHORT dir, register SHORT dir2, register SVECTOR *pPos);
extern VOID Spl_enemy_ck(register ENEMY_WORK *pEm);
extern ULONG Spl_em_pos_ck(register ENEMY_WORK *pEm, register ULONG Lim);
extern ULONG Spl_em_cdir_ck(register ENEMY_WORK *pEm);
extern ULONG Spl_pllock_get(register ENEMY_WORK *pEm, register ENEMY_WORK *pYo);
extern ULONG Spl_life_down(register ENEMY_WORK *pEm, register LONG Down_life, register ULONG Mode);
VOID Aida(register ENEMY_WORK *pEm);
/* Aida move routines also called directly from Spl_sce[0x16..0x1a] (SPL_SCE.C) */
extern VOID Aida_mv07(register ENEMY_WORK *pEm);
extern VOID Aida_mv08(register ENEMY_WORK *pEm);
extern VOID Aida_mv09(register ENEMY_WORK *pEm);


#endif /* SPL_AUTO_H_H */
