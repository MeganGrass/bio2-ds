/*
 * SND.H - Sound system prototypes, BGM and SE state
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SND.H
 */
#ifndef SND_H
#define SND_H
#include <LIBGS.H>
#include "TYPES.H"
#include "CD.H"
#include "CONFIG.H"

/* ---- types migrated from data.h ---- */
struct tagSNDEFF {
	USHORT flg;
	USHORT gyo;
	UCHAR ID;
	UCHAR _NO;
	UCHAR PRI;
	UCHAR CH;
	SHORT RVOL;
	SHORT LVOL;
};


/* SND.C - Sound system: init, BGM, SE */

void Snd_sys_init(void);
void Snd_sys_init2(void);
void Snd_sys_Allvoff(void);
void Snd_sys_init_sub(void);
void Snd_sys_init_sub2(void);
void Snd_sys_start(UCHAR ck);
void Snd_sys_stereo(UCHAR sorm);
void Snd_sys_core_set(UCHAR core_type_no, UCHAR sw);
void Snd_sys_arms_set(UCHAR arms_type_no);
void Snd_sys_room_set(void);
void Snd_sys_enem_set(void);
void Snd_sys_pset(void);
void Snd_bgm_data(USHORT *pTbl);
void Snd_bgm_set(void);
void Snd_bgm_ck(void);
void Snd_bgm_play_ck(void);
char Snd_bgm_main(void);
char Snd_bgm_sub(void);
void Snd_bgm_fade_ON(UCHAR time, UCHAR mode);
void Snd_bgm_fade(void);
void Snd_bgm_ctr(ULONG code);
void Snd_bgm_tbl_set(ULONG code);
void Snd_se_stad(ULONG bits, VECTOR *pVec);
void Snd_se_enem(UCHAR SENo, ENEMY_WORK *pEm);
void Snd_se_walk();	/* unprototyped: retail callers pass the footstep side unnarrowed; old-style definition in SND.C */
void Snd_se_call(void);
UCHAR Snd_se_pri_ck(UCHAR Sv_ch, UCHAR Sv_pri);
void Snd_se_3D(VECTOR *pVec, int flg_3D);
short Snd_se_dir_ck(long B_x, long B_z, long T_x, long T_z);
void Snd_vab_close(int Slot, int Flg);
void Snd_key_off_all(void);
void Snd_bgm_pause(int Resume);
void Snd_cd_watchdog(void);

/* Per-room BGM table (DS 0x800d46d0): index = nroom[Stage_no] + Room_no.
 * Alias of G.SaveData.Bgm_tbl (retail SaveData at DS 0x800d44a4, Bgm_tbl at +0x22c = 0x800d46d0). */
#define Bgm_room_tbl ((BGMCTR *)G.SaveData.Bgm_tbl)
/* BGM fade state: id_check byte of the pEdt_adr[0] slot (DS 0x800dbb70, offset 0), set by Snd_bgm_fade_ON */
#define Snd_fade_state (pEdt_adr[0].id_check)
/* BGM fade time countdown (DS 0x800dfadf) */
extern UCHAR Fade_time;

/* libsnd wrappers (named from PsyQ in Ghidra) */
/* SDK 4.7 LIBSND.LIB exports _SsSndCrescendo and no bare SsSndCrescendo symbol, so the macro below maps the name to it.
 * Retail BGM code (Snd_bgm_set, Snd_bgm_ck, Snd_bgm_fade_ON, Snd_bgm_ctr) calls DS 0x8007a120 directly. */
#define SsSndCrescendo _SsSndCrescendo
void SsSndCrescendo(SHORT seq_access_num, USHORT vol, INT time);
/* 3D SE distance index (0..0x7f), DS 0x800e8988 */
extern INT Snd_se_dist_idx;
/* CD read watchdog counter, DS 0x800a7fac (bumped by Snd_cd_watchdog) */
extern ULONG Cd_watchdog_cnt;

/* EDT table slot access (pEdt_adr[6], DS 800dbb70, declared in its owner header).
 * Slots 2..5 can hold a block pointer instead of the flag bytes. */
#define EDT_SLOT_PTR(n)  (*(ULONG **)&pEdt_adr[(n)])
#define EDT_SLOT_WORD(n) (*(ULONG *)&pEdt_adr[(n)])

/* EDT file footer: the VAB header offset is the word 8 bytes before the end of the file (cd_ret = bytes read) */
#define EDT_VAB_OFS(buf, len)	(*(int *)((UCHAR *)(buf) + (len) - 8))

/* BGM file footer: three words counted back from the end of the file read by Cd_read (cd_ret).
 * Main BGM footer starts at cd_ret-12; sub BGM footer starts at cd_ret-16 (retail -0x10). */
typedef struct BGM_FOOTER {
	int cd_size;	/* bytes of the file to copy into the VH buffer */
	int vab_off;	/* VAB header offset in the VH buffer */
	int seq_off;	/* sequence offset in the VH buffer */
} BGM_FOOTER;

/* Sound bank work buffers (formerly DAT_ addresses) */
extern UCHAR *Core_edt_buf;	/* DS 0x801f9d10, CORE EDT/VH load buffer (8 bytes before: Core_edt_buf_m8). Largest CORExx.EDH is 0xce8 bytes (CORE15), the gap to Arms_edt_buf is 0xcf0 */
extern UCHAR *Arms_edt_buf;	/* DS 0x801faa00, arms EDT/VH load buffer (also death-sound VH). Largest ARMSxx.EDH is 0xca8 bytes (ARMS11, ARMS13); the death-sound VH block is 0xc68 */
extern UCHAR *Bgm_main_vh_buf;	/* DS 0x801f2e00, main BGM VH/seq buffer. The copy length is the footer cd_size, at most 0x5860 over the 58 MAINxx.BGM files; the gap to Enemy_edt is 0x6010 */
extern UCHAR *Bgm_sub_vh_buf;	/* DS 0x801ece00, sub BGM VH/seq buffer. The copy length is the footer cd_size, at most 0x52b8 over the 56 SUB_xx.BGM files; the gap to Bgm_main_vh_buf is 0x6000 */
extern ULONG *Font_tim_buf;	/* DS 0x801bfa14, shared CD load buffer */
extern ULONG *Bg_img_buf;	/* DS 0x80198000, shared CD load buffer */



/* Globals defined in SND.C */
	extern UCHAR Enemy_edt[0xEA8];					// 0x801f8e10
	extern tagfpos EnemSE_floc[146];					// 0x800a7b1c
	extern UCHAR LRVT[128];								// 0x800a7fb0
	extern UCHAR PANVT[128];							// 0x800a8030
	extern UCHAR nroom[8];								// 0x800a80b0	// Bgm_room_tbl base index per stage
	extern USHORT Core_edh[24];						// 0x800a80b8	// COREnn_EDH names (FILECODE.H), by core_type_no
	extern USHORT Core_vbd[24];						// 0x800a80e8	// COREnn_VB names (FILECODE.H), by core_type_no
	extern USHORT Arms_edh[20];						// 0x800a8118	// ARMSnn_EDH names (FILECODE.H), by arms_type_no
	extern USHORT Arms_vbd[20];						// 0x800a8140	// ARMSnn_VB names (FILECODE.H), by arms_type_no
	extern USHORT Main_bgm[64];						// 0x800a8168	// MAINnn_BGM names (FILECODE.H), by pBgm->Main
	extern USHORT Sub_bgm[64];						// 0x800a81e8	// SUB_nn_BGM names (FILECODE.H), by pBgm->Sub
	extern UCHAR Reverb_flg;							// 0x800c4410
	extern SHORT Sedir2[4];								// 0x800cbc30
	extern CHAR Vab_id[7];								// 0x800d4c48
	extern SndVolume Cd_vol;							// 0x800d4c78
	extern SEENTRY Se_pri[24];							// 0x800d4ca0
	extern UCHAR sel_no[0x300];							// 0x800d4f18 (retail label Entory = sel_no + 8, an alias inside this table)
	extern SndVolume Pan_vol;							// 0x800d7598
	extern ULONG* pVh_tbl[8];							// 0x800d75a0
	extern SEQCTR Seq_ctr[3];							// 0x800d75c0
	extern SndVolume Rev_vol;							// 0x800d7658
	extern UCHAR Volume_Se;								// 0x800e8768
	extern UCHAR Volume_Bgm;							// 0x800eae3c
	extern SHORT UnkSndSysInitSubFlg;					// 0x800d783c
	extern BGMCTR Bgm;									// 0x800d7856
	extern SndVolume RevD_vol;							// 0x800d8ce4
	extern ULONG UnkSndFlg0;							// 0x800dbb88
	extern ULONG UnkSndSysFlg0;							// 0x800dbb8c
	extern SndVolume Main_vol;							// 0x800dcb9c
	extern UCHAR Fade_mode;								// 0x800df340
	extern BGMCTR* pBgm;								// 0x800dfd64
	extern ULONG UnknownSndSysFlg;						// 0x800e2a84
	extern CHAR Seq_table[528];							// 0x800e8770
	extern ULONG MainVbSize;							// 0x800eade4
	extern ULONG RoomVbSize;							// 0x800eae40
	extern SHORT Sedir;									// 0x800eae98
#endif /* SND_H_H */
