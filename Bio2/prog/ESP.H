#ifndef ESP_H
#define ESP_H
/*
 * ESP.H - Effect Sprite (ESP) work structures and ESP.C prototypes
 * D:\Bio2\prog\ESP.H
 *
 * Field offsets match the offset comments in the struct (CCPSX probe). Retail meaning
 * is checked for Routine0/1, Be_flg, Id, Type, pMat and DirY only. Free* names are unresolved.
 */

#include "TYPES.H"


#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif

/* Move-sequence entry (24 bytes); overlays ESP_WORK offsets 0x00..0x17. */
struct MSEQUENCE {
	UCHAR Routine0;		/* 00 */
	UCHAR Routine1;		/* 01 */
	UCHAR Free0;		/* 02 */
	UCHAR Free1;		/* 03 */
	USHORT Transx;		/* 04 */
	USHORT Transy;		/* 06 */
	CHAR Add_x;		/* 08 */
	CHAR Add_y;		/* 09 */
	CHAR Add_z;		/* 0A */
	UCHAR Free2;		/* 0B */
	SHORT Speed_x;		/* 0C */
	SHORT Speed_y;		/* 0E */
	SHORT Speed_z;		/* 10 */
	USHORT Free3;		/* 12 */
	USHORT Free4;		/* 14 */
	USHORT Free5;		/* 16 */
};

/* Animation frame entry (8 bytes); Time is at +2. */
struct SSEQUENCE {
	UCHAR pGp;		/* 00 */
	UCHAR nSpr;		/* 01 */
	UCHAR Time;		/* 02 */
	UCHAR Twh;		/* 03 */
	SHORT Hotx;		/* 04 */
	SHORT Hoty;		/* 06 */
};

struct ESP_WORK {
	UCHAR Routine0;		/* 00 */
	UCHAR Routine1;		/* 01 */
	UCHAR Free0;		/* 02 (frame/step counter; see Esp_01..Esp_22) */
	UCHAR Free1;		/* 03 (timer / step counter) */
	USHORT Transx;		/* 04 */
	USHORT Transy;		/* 06 */
	CHAR Add_x;		/* 08 */
	CHAR Add_y;		/* 09 */
	CHAR Add_z;		/* 0A */
	UCHAR Free2;		/* 0B */
	SHORT Speed_x;		/* 0C */
	SHORT Speed_y;		/* 0E */
	SHORT Speed_z;		/* 10 */
	USHORT Free3;		/* 12 */
	USHORT Free4;		/* 14 - low half; the 4-byte word at 14..17 is the
				 *      ground height, see ESP_GROUND_Y() in ESP_R.H */
	USHORT Free5;		/* 16 */
	USHORT Be_flg;		/* 18 */
	UCHAR Wk_no;		/* 1A */
	CHAR Padd0;		/* 1B */
	UCHAR Id;		/* 1C */
	UCHAR Id2;		/* 1D */
	UCHAR Type;		/* 1E */
	UCHAR Mov_cnt;		/* 1F */
	UCHAR Anm_time;		/* 20 */
	UCHAR Anm_cnt;		/* 21 */
	SHORT DirY;		/* 22 */
	SHORT Mov_x;		/* 24 */
	SHORT Mov_y;		/* 26 */
	SHORT Mov_z;		/* 28 */
	USHORT Tpage;		/* 2A */
	SHORT Hot_x;		/* 2C */
	SHORT Hot_y;		/* 2E */
	SHORT Hot_z;		/* 30 */
	USHORT Clut;		/* 32 */
	SHORT Pos_x;		/* 34 */
	SHORT Pos_y;		/* 36 */
	SHORT Pos_z;		/* 38 */
	USHORT Mag;		/* 3A */
	SHORT Old_x;		/* 3C */
	SHORT Old_y;		/* 3E */
	SHORT Old_z;		/* 40 */
	SHORT Padd1;		/* 42 */
	SHORT Cdir_x;		/* 44 */
	SHORT Cdir_y;		/* 46 */
	SHORT Cdir_z;		/* 48 */
	SHORT Padd2;		/* 4A */
	SHORT m[3][3];		/* 4C */
	SHORT Padd3;		/* 5E */
	LONG t[3];		/* 60 */
	MATRIX *pMat;		/* 6C */
	SSEQUENCE *pAnmseq;	/* 70 */
	SPRITEGp *pSprg;	/* 74 */
	MSEQUENCE *pMoveseq;	/* 78 */
};

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

extern ESP_WORK *pESP;
extern UCHAR Espid[16];
extern ULONG *pEspdt[64];
extern ULONG *pEspmv[64];
extern ESP_WORK Esp[96];
extern VOID *Esp_tblj[96];

/* ESP.C */
extern UCHAR Esp_init_C(VOID);
extern UCHAR Esp_init_R(VOID);
extern UCHAR Esp_call(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec);
extern UCHAR Esp_call3(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec);
extern UCHAR Esp_call2(ULONG bit, ULONG dir_y_id2, MATRIX *pmat, SVECTOR *svec);
extern UCHAR Esp_call3D(ULONG bit, LONG dir_y, MATRIX *pmat, SVECTOR *svec, SVECTOR *dsv);
extern UCHAR Esp_call3D2(ULONG bit, ULONG dir_y_id2, MATRIX *pmat, SVECTOR *svec, SVECTOR *dsv);
extern UCHAR Esp_kill(UCHAR id, UCHAR tp, MATRIX *pmat);
extern UCHAR Esp_kill2(ULONG id2);
extern VOID Esp_kill_all(VOID);		/* DS 0x8001d07c, name_registry.md */
extern VOID Esp_die(VOID);
extern UCHAR Esp_ctrl(UCHAR id, UCHAR tp, USHORT rtn, MATRIX *pmat);
extern UCHAR Esp_ctrl2(USHORT wk_no, USHORT rtn);
extern UCHAR Esp_ctrl3(UCHAR id, UCHAR tp, USHORT magx, USHORT sinx, MATRIX *pmat);
extern UCHAR Esp_ctrl4(UCHAR id, UCHAR tp, USHORT magy, USHORT siny, MATRIX *pmat);
extern VOID Esp_move(VOID);
extern UCHAR Esp_core_buf[0x217C];	/* DS 0x801fc400, CORE00.ESP load buffer */
extern void Esp_pos(LONG Run_rtn0); 	/* Run_rtn0 != 0: run Routine0 first */
/* Esp_pos_matrix temporaries: retail keeps them in the ScratchPad (0x1f800000 matw, 0x1f800020 vres, 0x1f80002c tmp). */
#define ESP_PM_MATW	((MATRIX *)(ScratchPad + 0x00))
#define ESP_PM_VRES	((VECTOR *)(ScratchPad + 0x20))
#define ESP_PM_TMP	((SVECTOR *)(ScratchPad + 0x2c))

extern VOID Esp_pos_matrix(VOID);	/* DS 0x8001d894, name_registry.md */


#endif /* ESP_H_H */
