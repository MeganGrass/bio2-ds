/*
 * types.h - Shared base typedefs and migrated struct definitions
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\types.h
 */
#ifndef TYPES_H
#define TYPES_H

/* Place a large RAM buffer in its own section; the address is set in Makefile.link (group bss,org(...)) */
#define RAM_SECTION(s) __attribute__ ((section (s)))
/* Scratchpad memory (DS 1f800000, 0x400 bytes); a fixed address, not a real object */
#define ScratchPad	((UCHAR *)0x1F800000)

/* forward declarations: every migrated struct, declared once (C89 forbids repeated typedefs) */
typedef struct tagGLOBAL GLOBAL;
typedef struct tagRESULT_EX RESULT_EX;
typedef struct tagRESULT_REC RESULT_REC;
typedef struct tagRESULT_MOJI RESULT_MOJI;
typedef struct T_MOJI_TBL T_MOJI_TBL;
typedef struct TRANS TRANS;
typedef struct TITLE_WORK TITLE_WORK;
typedef struct TEX_MOVE_DATA TEX_MOVE_DATA;
typedef struct SSEQUENCE SSEQUENCE;
typedef struct SPL_FREE_WORK SPL_FREE_WORK;
typedef struct PSP PSP;
typedef struct OP_WORK OP_WORK;
typedef struct OP_MOJI_TBL OP_MOJI_TBL;
typedef struct OP_FADE OP_FADE;
typedef struct MSEQUENCE MSEQUENCE;
typedef struct JH_TBL JH_TBL;
typedef struct JH_HEAD JH_HEAD;
typedef struct ESP_WORK ESP_WORK;
typedef struct DesldState DesldState;
typedef struct DesldState DESLD_STATE;
typedef struct tagCNF_CHAR CNF_CHAR;
typedef struct tagCNF_ST CNF_ST;
typedef struct tagDOOR_DATA DOOR_DATA;
typedef struct tagDOOR_EX_LINE DOOR_EX_LINE;
typedef struct tagDOOR_SND_HEAD DOOR_SND_HEAD;
typedef struct tagFLOC FLOC;
typedef struct tagPL_VIEW_OFS PL_VIEW_OFS;
typedef struct tagSELECT_LIGHT SELECT_LIGHT;
typedef struct tagSELECT_MOJI SELECT_MOJI;
typedef struct tagSELECT_MOJI_REC SELECT_MOJI_REC;
typedef struct tagSELECT_NAME_UV SELECT_NAME_UV;
typedef struct tagSELECT_PL_INIT SELECT_PL_INIT;
typedef struct tagSELECT_SPRT_DEF SELECT_SPRT_DEF;
typedef struct tagSELECT_WORK SELECT_WORK;
typedef struct tagSELECT_XY SELECT_XY;
typedef struct tagCD tagCD;
typedef struct tagEDT_TABLE_WORK tagEDT_TABLE_WORK;
typedef struct tagGAME tagGAME;
typedef struct tagGLOBAL tagGLOBAL;
typedef struct tagMAIN tagMAIN;
typedef struct tagfpos tagfpos;
typedef struct tagAF_MOVE_TBL AF_MOVE_TBL;
typedef struct tagAOT_DATA AOT_DATA;
typedef struct tagATE ATE;
typedef struct tagATTACK_PRIM_F4 ATTACK_PRIM_F4;
typedef struct tagATTACK_SEQ ATTACK_SEQ;
typedef struct tagATTACK_WEAPON_EM ATTACK_WEAPON_EM;
typedef struct tagATTACK_WEAPON_EM_REN ATTACK_WEAPON_EM_REN;
typedef struct tagATTACK_WEAPON_TBL ATTACK_WEAPON_TBL;
typedef struct tagATTACK_WORK ATTACK_WORK;
typedef struct tagATTACK_WORK3 ATTACK_WORK3;
typedef struct tagAT_DATA AT_DATA;
typedef struct tagAT_TBL AT_TBL;
typedef struct tagAT_WORK AT_WORK;
typedef struct tagBGMCTR BGMCTR;
typedef struct tagBLOCK_DATA BLOCK_DATA;
typedef struct tagBLOOD_DATA BLOOD_DATA;
typedef struct tagBOM_WORK BOM_WORK;
typedef struct tagBOW_HIT_PARTS BOW_HIT_PARTS;
typedef struct tagBOX BOX;
typedef struct tagBOXLINE BOXLINE;
typedef struct tagBRANCH_TBL BRANCH_TBL;
typedef struct tagCAPTION CAPTION;
typedef struct tagCAPTION_ID CAPTION_ID;
typedef struct tagCAPTION_TBL CAPTION_TBL;
typedef struct tagCARD_SAVE_DATA CARD_SAVE_DATA;
typedef struct tagCC_PARTS_WORK CC_PARTS_WORK;
typedef struct tagCC_WORK CC_WORK;
typedef struct tagCFILE_INFO CFILE_INFO;
typedef struct tagCFILE_INFO_S CFILE_INFO_S;
typedef struct tagCNF_SPR CNF_SPR;
typedef struct tagCOM_WORK COM_WORK;
typedef struct tagCORPSE_SAVE_WORK CORPSE_SAVE_WORK;
typedef struct tagCORPSE_WORK CORPSE_WORK;
typedef struct tagCUTINFO CUTINFO;
typedef struct tagDATA_POS DATA_POS;
typedef struct tagDBUF DBUF;
typedef struct tagDB_WORK DB_WORK;
typedef struct tagDIEDEMO DIEDEMO;
typedef struct tagDIESPR DIESPR;
typedef struct tagDISP_POLY DISP_POLY;
typedef struct tagDM_ESP DM_ESP;
typedef struct tagDM_ESP2 DM_ESP2;
typedef struct tagDM_ESP3 DM_ESP3;
typedef struct tagDM_WEP_ESP DM_WEP_ESP;
typedef struct tagDOOR DOOR;
typedef struct tagDOOR_INFO DOOR_INFO;
typedef struct tagDOOR_WORK DOOR_WORK;
typedef struct tagDVIEW DVIEW;
typedef struct tagECG_POINT ECG_POINT;
typedef struct tagEM10_FREE EM10_FREE;
typedef struct tagEM20_BLOOD_TBL EM20_BLOOD_TBL;
typedef struct tagEM20_FREE EM20_FREE;
typedef struct tagEM21F EM21F;
typedef struct tagEM22_FREE EM22_FREE;
typedef struct tagEM23_ESP EM23_ESP;
typedef struct tagEM23_FREE EM23_FREE;
typedef struct tagEM23_WEP_ESP EM23_WEP_ESP;
typedef struct tagEM25_BLOOD_TBL EM25_BLOOD_TBL;
typedef struct tagEM25_FREE EM25_FREE;
typedef struct tagEM26_BLOOD_TBL EM26_BLOOD_TBL;
typedef struct tagEM26_CATCH_TBL EM26_CATCH_TBL;
typedef struct tagEM26_FREE EM26_FREE;
typedef struct tagEM27_FREE EM27_FREE;
typedef struct tagEM28_FREE EM28_FREE;
typedef struct tagEM28_P_FREE EM28_P_FREE;
typedef struct tagEM29_FREE EM29_FREE;
typedef struct tagEM2A_FREE EM2A_FREE;
typedef struct tagEM2B_FREE EM2B_FREE;
typedef struct tagEM2E_BLOOD_TBL EM2E_BLOOD_TBL;
typedef struct tagEM2E_FREE EM2E_FREE;
typedef struct tagEM2E_HAND EM2E_HAND;
typedef struct tagEM2E_HITSIZE EM2E_HITSIZE;
typedef struct tagEM2F_BLOOD_TBL EM2F_BLOOD_TBL;
typedef struct tagEM2F_FREE EM2F_FREE;
typedef struct tagEM2d_FREE EM2d_FREE;
typedef struct tagEM30_FREE EM30_FREE;
typedef struct tagEM30_PARTS_FREE EM30_PARTS_FREE;
typedef struct tagEM31_FREE EM31_FREE;
typedef struct tagEM33_BLOOD_TBL EM33_BLOOD_TBL;
typedef struct tagEM33_FREE EM33_FREE;
typedef struct tagEM34_FREE EM34_FREE;
typedef struct tagEM36_FREE EM36_FREE;
typedef struct tagEM37_FREE EM37_FREE;
typedef struct tagEM38_FREE EM38_FREE;
typedef struct tagEM3A_BLOOD_TBL EM3A_BLOOD_TBL;
typedef struct tagEM3A_FREE EM3A_FREE;
typedef struct tagEM3B_BLOOD_TBL EM3B_BLOOD_TBL;
typedef struct tagEM3B_FREE EM3B_FREE;
typedef struct tagEM_BIN_INFO EM_BIN_INFO;
typedef struct tagEM_TBL EM_TBL;
typedef struct tagEND_SPR END_SPR;
typedef struct tagENEMY_WORK ENEMY_WORK;
typedef struct tagESPECT_BASE ESPECT_BASE;
typedef struct tagFADE FADE;
typedef struct tagFILE_DATA FILE_DATA;
typedef struct tagFLAG_DISP_TBL FLAG_DISP_TBL;
typedef struct tagFLG_VIEW FLG_VIEW;
typedef struct tagFLOOR_DATA FLOOR_DATA;
typedef struct tagGROUPWK GROUPWK;
typedef struct tagHOLD_TBL HOLD_TBL;
typedef struct tagIN_DOOR_WORK IN_DOOR_WORK;
typedef struct tagIN_DOOR_WORK2 IN_DOOR_WORK2;
typedef struct tagITEM_DATA ITEM_DATA;
typedef struct tagITEM_MIX_DATA ITEM_MIX_DATA;
typedef struct tagITEM_WORK ITEM_WORK;
typedef struct tagJOINT_P JOINT_P;
typedef struct tagKAGE_WORK KAGE_WORK;
typedef struct tagKAGE_WORK9 KAGE_WORK9;
typedef struct tagKI KI;
typedef struct tagKI2 KI2;
typedef struct tagKI2_LINE_F3 KI2_LINE_F3;
typedef struct tagKO_TBL KO_TBL;
typedef struct tagLIGHT_CUT_DATA LIGHT_CUT_DATA;
typedef struct tagLIGHT_DATA LIGHT_DATA;
typedef struct tagLINE_2 LINE_2;
typedef struct tagLINE_SCR LINE_SCR;
typedef struct tagLOADTASK LOADTASK;
typedef struct tagMACRO_TBL MACRO_TBL;
typedef struct tagMACRO_WORK MACRO_WORK;
typedef struct tagMAP_DATA MAP_DATA;
typedef struct tagMAP_STAGE_INFO MAP_STAGE_INFO;
typedef struct tagMEDAMA MEDAMA;
typedef struct tagMEMORY_MANAGE MEMORY_MANAGE;
typedef struct tagMODE MODE;
typedef struct tagMODEL MODEL;
typedef struct tagMODEL_LINK_INFO MODEL_LINK_INFO;
typedef struct tagMODEL_WORK MODEL_WORK;
typedef struct tagMOJIDISP MOJIDISP;
typedef struct tagMONITOR MONITOR;
typedef struct tagMOVIE_TBL MOVIE_TBL;
typedef struct tagMWORK MWORK;
typedef struct tagOBJ_MODEL_WORK OBJ_MODEL_WORK;
typedef struct tagOM_PARTS_WORK OM_PARTS_WORK;
typedef struct tagPARTSST PARTSST;
typedef struct tagPARTSWK PARTSWK;
typedef struct tagPARTS_BOMB_FREE_WORK PARTS_BOMB_FREE_WORK;
typedef struct tagPARTS_BOW_FREE_WORK PARTS_BOW_FREE_WORK;
typedef struct tagPARTS_COLOR_CH_FREE_WORK PARTS_COLOR_CH_FREE_WORK;
typedef struct tagPARTS_FLY PARTS_FLY;
typedef struct tagPARTS_NECK_FREE_WORK PARTS_NECK_FREE_WORK;
typedef struct tagPARTS_RYU_FREE_WORK PARTS_RYU_FREE_WORK;
typedef struct tagPARTS_W PARTS_W;
typedef struct tagPDEMO PDEMO;
typedef struct tagPLAYER_WORK PLAYER_WORK;
typedef struct tagPL_MAG_TBL PL_MAG_TBL;
typedef struct tagPOLY_EFF POLY_EFF;
typedef struct tagPOLY_EFF2 POLY_EFF2;
typedef struct tagPOLY_EFF2_WORK POLY_EFF2_WORK;
typedef struct tagPOLY_F4_2 POLY_F4_2;
typedef struct tagPRIM_F4 PRIM_F4;
typedef struct tagPRIM_G4 PRIM_G4;
typedef struct tagPRIM_LINE_F3 PRIM_LINE_F3;
typedef struct tagPSP_DATA PSP_DATA;
typedef struct tagPXPY PXPY;
typedef struct tagP_XY P_XY;
typedef struct tagRCUT RCUT;
typedef struct tagREQUEST REQUEST;
typedef struct tagROOM_ENEMY_TBL ROOM_ENEMY_TBL;
typedef struct tagROOM_HEAD ROOM_HEAD;
typedef struct tagROOM_POS ROOM_POS;
typedef struct tagROOM_WORK ROOM_WORK;
typedef struct tagSAB SAB;
typedef struct tagSAVE_DATA SAVE_DATA;
typedef struct tagSCA_DATA SCA_DATA;
typedef struct tagSCA_HEAD SCA_HEAD;
typedef struct tagSCENARIO SCENARIO;
typedef struct tagSCE_AOT SCE_AOT;
typedef struct tagSCE_AOT2 SCE_AOT2;
typedef struct tagSCE_AT_DATA SCE_AT_DATA;
typedef struct tagSCE_EM_SET_WORK SCE_EM_SET_WORK;
typedef struct tagSCE_ITEM_DATA SCE_ITEM_DATA;
typedef struct tagSCE_TASK SCE_TASK;
typedef struct tagSC_UTIL SC_UTIL;
typedef struct tagSEARCH_AOT SEARCH_AOT;
typedef struct tagSEENTRY SEENTRY;
typedef struct tagSEQCTR SEQCTR;
typedef struct tagSET_EM_JMP SET_EM_JMP;
typedef struct tagSIGHT_TBL SIGHT_TBL;
typedef struct tagSIGHT_WORK SIGHT_WORK;
typedef struct tagSNDEFF SNDEFF;
typedef struct tagSPL_FREE SPL_FREE;
typedef struct tagSPRITEGp SPRITEGp;
typedef struct tagSPRT2 SPRT2;
typedef struct tagSP_WORK SP_WORK;
typedef struct tagSTATUS STATUS;
typedef struct tagSUBPLAY_WORK SUBPLAY_WORK;
typedef struct tagTASK TASK;
typedef struct tagTCB2 TCB2;
typedef struct tagTEX_MOVE_SEQ TEX_MOVE_SEQ;
typedef struct tagTMD_MEM TMD_MEM;
typedef struct tagTPOLY TPOLY;
typedef struct tagTSPRT TSPRT;
typedef struct tagT_EM_SET T_EM_SET;
typedef struct tagT_WORK T_WORK;
typedef struct tagUTILS UTILS;
typedef struct tagVCUT VCUT;
typedef struct tagVIEW_CTR VIEW_CTR;
typedef struct tagVIEW_ENV VIEW_ENV;
typedef struct tagVIEW_WORK VIEW_WORK;
typedef struct tagVM_DISP VM_DISP;
typedef struct tagVM_FRAME VM_FRAME;
typedef struct tagVM_WORK VM_WORK;
typedef struct tagVRAMVIEW VRAMVIEW;
typedef struct tagV_STR V_STR;
typedef struct tagWEP_BLOOD WEP_BLOOD;
typedef struct tagWOUND_WORK WOUND_WORK;
typedef struct tag_CARD _CARD;
typedef struct tag_TIM4 _TIM4;
typedef struct tagbyte_lh byte_lh;

#include <TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBCD.H>

typedef char				CHAR;
typedef unsigned char		UCHAR;
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef int					INT;
typedef unsigned int		UINT;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef void (*CODE_PTR)();
typedef long int			LONG;
typedef unsigned long int	ULONG;

#define CONST				const

typedef void				VOID;

typedef UINT				BOOL;

/* ---- types migrated from data.h ---- */
struct tagbyte_lh {
	UCHAR low;
	UCHAR h;
};

typedef union WORK_LH {
	USHORT l;
	byte_lh hi;
} WORK_LH;

struct tagTPOLY {
	POLY_FT4 poly;
};

struct tagTSPRT {
	DR_MODE mode;
	SPRT sprt;
};

struct tagPOLY_F4_2 {
	ULONG* tag;
	UCHAR r0;
	UCHAR g0;
	UCHAR b0;
	UCHAR code;
	LONG pos[4];
};

struct tagLINE_2 {
	ULONG* tag;
	UCHAR r0;
	UCHAR g0;
	UCHAR b0;
	UCHAR code;
	LONG pos[2];
};

struct tagSPRT2 {
	SPRT Sp[2];
	DR_MODE Dm[2];
};

struct tagPRIM_F4 {
	DR_MODE mode;
	POLY_F4 prim;
};

struct tagPRIM_G4 {
	DR_MODE mode;
	POLY_G4 prim;
};

struct tagPRIM_LINE_F3 {
	DR_MODE mode;
	LINE_F3 prim;
};

struct tag_TIM4 {
	LONG id;
	LONG flag;
	LONG cbnum;
	SHORT cx;
	SHORT cy;
	SHORT cw;
	SHORT ch;
	UCHAR clut[32];
	LONG pbnum;
	SHORT px;
	SHORT py;
	SHORT pw;
	SHORT ph;
	UCHAR image[2];
};

struct tag_CARD {
	UCHAR magic[2];
	UCHAR type;
	UCHAR blockEntry;
	UCHAR title[64];
	UCHAR reserve[28];
	UCHAR clut[32];
	UCHAR icon[3][128];
};

struct tagTCB2 {
	LONG Status;
	LONG Mode;
	LONG Zero;
	LONG At;
	LONG V0;
	LONG V1;
	LONG A0;
	LONG A1;
	LONG A2;
	LONG A3;
	LONG T0;
	LONG T1;
	LONG T2;
	LONG T3;
	LONG T4;
	LONG T5;
	LONG T6;
	LONG T7;
	LONG S0;
	LONG S1;
	LONG S2;
	LONG S3;
	LONG S4;
	LONG S5;
	LONG S6;
	LONG S7;
	LONG T8;
	LONG T9;
	LONG K0;
	LONG K1;
	LONG GP;
	LONG SP;
	LONG FP;
	LONG RA;
	LONG EPC;
	LONG MDHI;
	LONG MDLO;
	LONG SR;
	LONG CAUSE;
	LONG Dummy0;
	LONG Dummy1;
	LONG Dummy2;
	LONG System[6];
};



#endif /* TYPES_H_H */
