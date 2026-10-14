/*
 * SUB.H - Shared helper prototypes (VRAM, memory, light, rotation)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SUB.H
 */
#ifndef SUB_H
#define SUB_H
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagLIGHT_CUT_DATA {
	UCHAR Mag;
	UCHAR Mode[3];
	UCHAR Col[3][3];
	UCHAR Ambient[3];
	SHORT Pos[3][3];
	USHORT L[3];
};

struct tagMEMORY_MANAGE {
	VOID* pAddress;
	ULONG Size;
};
// D:\Bio2\prog\SUB.H


extern VOID Vram_clr(register USHORT x, register USHORT y, register USHORT w, register USHORT h);
extern VOID Memcpy(register VOID *p, register VOID *q, register LONG r);
extern VOID Memclr(register VOID *p, register LONG r);
extern VOID Mapping_tim(register ULONG *pTim_Adr);
extern ULONG Mapping_tmd(register ULONG Mode, register ULONG *pTmd_adr, register ULONG Ctp, register ULONG Ccl);
extern VOID Set_view(register LONG *pVp, register LONG *pVr);
extern VOID Set_light(register VECTOR *pV);
extern VOID Set_Light_data(register ULONG No, register VECTOR *pLig, register UCHAR *pRgb);
extern VOID Flg_on(register ULONG *p, register ULONG x);
extern VOID Flg_off(register ULONG *p, register ULONG x);
extern ULONG Flg_ck(register ULONG *p, register ULONG x);
extern VOID Rot_vector(register SHORT dir, register SVECTOR *p, register SVECTOR *q);
extern VOID Rot_vector_super(register SHORT dir, register MATRIX *pM, register SVECTOR *p, register SVECTOR *q);
extern VOID Rot_add_matrix(register LONG *pPos, register SHORT *pDir, register MATRIX *pM, register SVECTOR *Add_dir);
extern VOID Set_front_pos(register ENEMY_WORK *pEm);
extern VOID SetSpadStack(VOID);
extern VOID ResetSpadStack(VOID);
extern VOID SetSpadStack2(VOID);
extern VOID ResetSpadStack2(VOID);
extern ULONG Get_work(register VOID **p, register ULONG Size);
extern ULONG Free_work(register VOID **p);

extern MEMORY_MANAGE Mem_man[8];	// 800D5BA0		// 64 bytes
extern MEMORY_MANAGE *pCmem_man;	// 800AB204		// 8 bytes
extern UCHAR Prim_work_buf[];		// 8018F040		// Mapping_tmd scratch buffer



/* Globals defined in SUB.C */
	extern MATRIX Tmp_matrix[2];						// 0x800c3be0
	extern ULONG *pStack_bak;							// 0x800c3c28 - saved stack pointer (SetSpadStack stores sp here)
#endif /* SUB_H_H */
