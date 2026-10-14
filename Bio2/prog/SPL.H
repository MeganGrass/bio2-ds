/*
 * SPL.H - Spl enemy AI work structs, joint data and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SPL.H
 */
#ifndef SPL_H
#define SPL_H
#include "TYPES.H"
#include "EM_SUB.H"

/* ---- types migrated from data.h ---- */
struct tagSPL_FREE {
	SHORT Dest_x;
	SHORT Dest_z;
	USHORT Move_flg;
	USHORT Dest_ll;
	SHORT Sake_dir;
	UCHAR Em_pos_flg;
	SHORT Sake_timer;
	SHORT Back_spd;
};
// D:\Bio2\prog\SPL.H


struct TEX_MOVE_DATA {
	UCHAR Mo_u;
	UCHAR Mo_v;
	UCHAR Mo_w;
	UCHAR Mo_h;
	UCHAR Moto_tpage;
	UCHAR Saki_tpage;
	UCHAR Sa_u;
	UCHAR Sa_v;
};

struct JH_TBL {
	USHORT Point;
	USHORT Per;
};

struct JH_HEAD {
	UCHAR Parts0_no;
	UCHAR Parts1_no;
	USHORT Vn_num;
	SVECTOR *p0Vec;
	SVECTOR *p0Nor;
	SVECTOR *p0Ovec;
	SVECTOR *p0Onor;
	SVECTOR Parts0_off;
	SVECTOR *p1Vec;
	SVECTOR *p1Nor;
	SVECTOR *p1Ovec;
	SVECTOR *p1Onor;
	SVECTOR Parts1_off;
	JH_TBL *pJh;
};

/*
 * Named view of ENEMY_WORK.Free[12] (48 bytes) used as AI scratch by the
 * Spl / Aida / Sherry routines. Access through SPL_FREE(pEm).
 */
struct SPL_FREE_WORK {
	ULONG Step;			/* Free[0]            */
	SHORT Goal_x;			/* Free[1] low word   */
	SHORT Goal_z;			/* Free[1] high word  */
	USHORT Flg;			/* Free[2] low word   */
	USHORT Dist;			/* Free[2] high word  */
	SHORT Turn_dir;			/* Free[3] low word   */
	UCHAR Turn_sub;			/* Free[3] byte 2     */
	UCHAR Blink;			/* Free[3] byte 3     */
	SHORT Timer4;			/* Free[4] low word   */
	SHORT Timer4_hi;		/* Free[4] high word  */
	UCHAR Free5_b0;			/* Free[5] byte 0     */
	UCHAR Free5_b1;			/* Free[5] byte 1     */
	UCHAR Cool;			/* Free[5] byte 2     */
	UCHAR Free5_b3;			/* Free[5] byte 3     */
	ULONG Free6[6];			/* Free[6..11]        */
};

#define SPL_FREE(p)	((SPL_FREE_WORK *)(p)->Free)

/* Motion word: Move_no (byte 0), Move_cnt (byte 1), Hokan_flg (byte 2), Mplay_flg (byte 3).
 * DS writes the four bytes with one sw (word store). */
#define SPL_SET_MOVE(p, v)	(*(ULONG *)&(p)->Move_no = (ULONG)(v))

extern VOID Spl(register ENEMY_WORK *pEm);
extern ULONG Pl_jh_init(register ENEMY_WORK *pEm, register ULONG Mode);
extern VOID Pl_jh_move(register ENEMY_WORK *pEm);
extern VOID Spl_set(VOID);

extern ENEMY_WORK *pSpl_near_em[3];
extern SHORT Em_dir[3];
extern USHORT Em_l[3];

/* Per-kind walk / back / dir / ato / run speed tables, defined in SPL.C */
extern UCHAR Subpl_walk[32][2];		/* DS 0x800a8268 */
extern UCHAR Subpl_back[32][2];		/* DS 0x800a82a8 */
extern UCHAR Subpl_dir[32][2];		/* DS 0x800a82e8 */
extern UCHAR Subpl_ato[32][2];		/* DS 0x800a8328 */
extern UCHAR Subpl_run[32][2];		/* DS 0x800a8368 */
extern UCHAR Em4a_zobiparts_tbl[7][2];	/* DS 0x800a83a8 */

extern VOID *Spl_routine_0[9];
extern VOID *Spl_mv[28];
extern VOID *Spl_in[28];
extern CODE_PTR Spl_dm[4];
extern CODE_PTR Spl_die[1];
extern VOID *Spl_dead[1];

extern SHORT Spl_r_pos[2][6];

/* ROM tables used by the Spl scene / chenge routines (values live in the game image) */
extern UCHAR Spl_kuchi3_rom[];			/* DS 0x80011738 (33 bytes) */
extern UCHAR Spl_kuchi4_rom[];			/* DS 0x8001175c (12 bytes) */
extern UCHAR Spl_kubi3_rom[];			/* DS 0x80011768 (12 bytes) */
extern UCHAR Spl_kubi4_rom[];			/* DS 0x80011774 (5 bytes)  */
extern UCHAR Spl_chenge08_kubi3_rom[];		/* DS 0x800118d0 (14 bytes) */
extern UCHAR Spl_chenge05_kuchi3_rom[];		/* DS 0x8001177c (26 bytes) */
extern UCHAR Spl_chenge05_kuchi4_rom[];		/* DS 0x80011798 (6 bytes)  */
extern UCHAR Spl_chenge05_kubi3_rom[];		/* DS 0x800117a0 (4 bytes)  */
extern UCHAR Spl_chenge05_kubi4_rom[];		/* DS 0x800117a4 (5 bytes)  */
extern UCHAR Spl_chenge05_me3_rom[];		/* DS 0x800117ac (9 bytes)  */
extern UCHAR Spl_chenge05_me4_rom[];		/* DS 0x800117b8 (14 bytes) */
extern UCHAR Spl_chenge06_body3_rom[];		/* DS 0x800117c8 (32 bytes) */
extern UCHAR Spl_chenge06_body4_rom[];		/* DS 0x800117e8 (14 bytes) */
extern UCHAR Spl_chenge06_kao3_rom[];		/* DS 0x800117f8 (12 bytes) */
extern UCHAR Spl_chenge06_kao4_rom[];		/* DS 0x80011804 (5 bytes)  */
extern UCHAR Spl_chenge06_atama4_rom[];		/* DS 0x8001180c (10 bytes) */
extern UCHAR Spl_chenge07_body3_rom[];		/* DS 0x80011818 (67 bytes) */
extern UCHAR Spl_chenge07_body4_rom[];		/* DS 0x8001185c (36 bytes) */
extern UCHAR Spl_chenge07_kao3_rom[];		/* DS 0x80011880 (32 bytes) */
extern UCHAR Spl_chenge07_kao4_rom[];		/* DS 0x800118a0 */
extern UCHAR Spl_chenge07_sub3_rom[];		/* DS 0x800118ac */
extern UCHAR Spl_chenge07_sub4_rom[];		/* DS 0x800118b8 */
extern UCHAR Spl_chenge07_mune3_rom[];		/* DS 0x800118c0 (12 bytes) */
extern UCHAR Spl_chenge07_mune4_rom[];		/* DS 0x800118c8 (7 bytes)  */
extern UCHAR Spl_chenge09_body3_rom[];		/* DS 0x800118e0 */
extern UCHAR Spl_chenge09_body4_rom[];		/* DS 0x800118f4 */
extern UCHAR Spl_chenge09_kao3_rom[];		/* DS 0x800118fc */
extern UCHAR Spl_chenge09_kao4_rom[];		/* DS 0x80011918 */
extern UCHAR Spl_chenge10_body3_rom[];		/* DS 0x80011924 */
extern UCHAR Spl_chenge10_body4_rom[];		/* DS 0x80011948 */

/* Spl_Init per-Id tables: DS block 0x80011620..0x80011738 (see Spl_Init; per-array split not byte-verified) */
extern SHORT Spl_init_parts0_y_rom[];
extern SHORT Spl_init_ofs_y_rom[];
extern USHORT Spl_init_at_rom[];
extern SHORT Spl_init_kage_lo_rom[];
extern SHORT Spl_init_kage_hi_rom[];



/* Globals defined in SPL.C */
	extern DR_MOVE EM40_Prim;							// 0x800c3bb8
#endif /* SPL_H_H */
