/*
 * SCE0.H - Scenario (SCE) task struct, globals and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SCE0.H
 */
#ifndef SCE0_H
#define SCE0_H
#include "TYPES.H"
#include "EM_SUB.H"

/* ---- types migrated from data.h ---- */
struct tagMODEL_LINK_INFO {
	ULONG* Tim_adr;
	ULONG* Tmd_adr;
};

struct tagSCE_TASK {
	UCHAR Routine0;
	UCHAR Status;
	CHAR Sub_ctr;
	UCHAR Task_level;
	CHAR Ifel_ctr[4];
	CHAR Loop_ctr[4];
	CHAR Loop_if_class[4][4];
	UCHAR* Data;
	UCHAR* Lstack[4][4];
	UCHAR* Lbreak[4][4];
	SHORT Lcnt[4][4];
	ULONG Stack[4][8];
	ULONG* pS_SP;
	UCHAR* Ret_addr[4];
	ENEMY_WORK* pWork;
	SHORT Spd_x;
	SHORT Spd_y;
	SHORT Spd_z;
	SHORT Dspd_x;
	SHORT Dspd_y;
	SHORT Dspd_z;
	SHORT Aspd_x;
	SHORT Aspd_y;
	SHORT Aspd_z;
	SHORT Adspd_x;
	SHORT Adspd_y;
	SHORT Adspd_z;
	ULONG R_no_bak;
};


/* SCE opcode handler (entries of Sce_jmp_tbl, copied to the ScratchPad) */
typedef ULONG (*SCE_JMP_FUNC)(void *);

/* file-level externs */
/* DS 0x800d7860 */
struct tagSCENARIO {
	SCE_TASK Sce_task[14];
	ULONG Random_base;
	UCHAR* pCScd;
	ENEMY_WORK* pC_em;
	POLY_GT4* pMizu_div;
	UCHAR Type;
	UCHAR Cut_old;
	UCHAR C_id;
	UCHAR C_model_type;
	UCHAR C_kind;
	UCHAR Mizu_div_max;
	UCHAR Mizu_div_ctr;
	UCHAR Rbj_reset_flg;
	UCHAR Se_tmp0;
	UCHAR Se_tmp1;
	UCHAR Se_vol;
};

extern SCENARIO      Sce;
extern ULONG         Padd2_mode;
extern UCHAR         em_se_tbl[];
extern SCE_JMP_FUNC  Sce_jmp_tbl[143];
/* DS 0x800a7704 */
extern UCHAR         Em_kind_tbl[];

/* function prototypes (DS address order) */
void   Sce_test_init(void);                          /* 800529b4 */
void   Sce_rnd_item_set(void);                       /* 800529fc */
ULONG  Em_kind_search(ULONG Id);                     /* 80052af4 */
void   Sce_se_set(void);                             /* 80052b38 */
void   Sce_rnd_set(void);                            /* 80052ca0 */
void   Sce_model_init(void);                         /* 80052d14 */
void   Sce_work_clr(void);                           /* 80052e20 */
void   Sce_work_clr_at(void);                        /* 80052e7c */
void   Sce_work_clr_set(void);                       /* 80052f3c */
void   Sce_aot_init(void);                           /* 800530c0 */
void   Event_init(SCE_TASK *pSce, ULONG Evt_no);     /* 800530ec */
void   Event_exec(ULONG Task_level, ULONG Evt_no);   /* 80053138 */
void   Sce_col_chg_init(void);                       /* 80053244 */
void   Sce_mirror_init(void);                        /* 800532d8 */
void   Sce_kirakira_set(void);                       /* 80053394 */
void   Sce_fade_chk(void);                           /* 80053470 */
int    Stage_room_to_mapno(int Stage_no, int Room_no); /* 800534c4 */
void   Sce_scheduler_set(void);                      /* 80053528 */
void   Sce_scheduler(void);                          /* 80053644 */
void   Sce_scheduler_main(void);                     /* 800536c4 */


#endif /* SCE0_H_H */
