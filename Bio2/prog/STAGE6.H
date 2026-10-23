/*
 * STAGE6.H - Stage 6 overlay work-data externs
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\STAGE6.H
 */
#ifndef STAGE6_H
#define STAGE6_H
#include "TYPES.H"
#include "SUB.H"
// D:\Bio2\prog\STAGE6.H
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif


/* Stage 6 setup routine and its data tables (defined in STAGE6.C). Sizes in the trailing comments are bytes. */
extern VOID Stage6(VOID);
extern USHORT Bs_size6[24][16];		// 8011B650		// 768 bytes
extern UCHAR Bs_check_sum6[24][16];		// 8011B950		// 384 bytes
extern USHORT Xa_sector06[61][3];		// 8011BAD0		// 366 bytes

/* Overlay work data used by STAGE6.C (addresses are STAGE6 overlay RAM, 8011xxxx) */
extern UCHAR Stage6_poly_tbl[];		// 8011B514		// 10 bytes per entry: tpage, clut, uv, size
extern UCHAR *Stage6_msg_ptr_tbl[];		// 8011B378		// message line pointers
extern UCHAR Stage6_msg_buf[];		// 8011BC78		// message print buffer
extern UCHAR Stage6_rnd_tbl[];		// 8011B640		// random clut index table
extern UCHAR Stage6_msg_len_tbl[];		// 8011B3C8		// chars per message line
extern USHORT Stage6_file_tbl[];		// 8011B648		// CD file numbers
extern UCHAR Stage6_obj_tbl[];		// 8011B5A0		// flash object table (8 bytes per entry)
extern UCHAR *Stage6_obj_ptr;		// 8011BCF0		// current object table pointer
extern ULONG Stage6_flash_mask;		// 8011BC64		// visibility bits for flash
extern USHORT Stage6_flash_timer;		// 8011BCDE		// flash sequence frame counter
extern SHORT Stage6_obj_num;		// 8011BCDA		// number of objects (DS lh/sh)
extern USHORT Stage6_flash_phase;		// 8011BCA2		// 0: timed sequence, 1: random flicker (DS lhu/sh)
extern UCHAR Stage6_rnd_clut;		// 8011BCE0		// random clut index
extern UCHAR Stage6_rnd_prev;		// 8011BCA4		// previous random clut index
extern SHORT Stage6_fade_step;		// 8011BC6C		// fade step
extern SHORT Stage6_fade_val;		// 8011BCD2		// fade value
extern UCHAR Stage6_fade_id;		// 8011BCA0		// Fade_status id
extern UCHAR Stage6_exit_req;		// 8011BC62		// exit requested
extern UCHAR Stage6_cut_no;		// 8011BC61		// next cut number
extern LIGHT_CUT_DATA *Stage6_light_save;		// 8011BCE8		// saved room light data
extern ULONG Stage6_msg_state;		// 8011BCD4		// message state machine (DS lw/sw)
extern ULONG Stage6_msg_timer;		// 8011BC9C		// message char timer (DS lw/sw, unsigned compares)
extern ULONG Stage6_msg_line;		// 8011BC98		// message line index (DS lw/sw)
extern ULONG Stage6_msg_char;		// 8011BC70		// char index in line (DS lw/sw)
extern ULONG Stage6_msg_i;		// 8011BCF8		// loop index for message draw (DS lw/sw)
extern UCHAR Stage6_msg_base;		// 8011BCE4		// message table base
extern ULONG Stage6_msg_idx;		// 8011BC68		// message length table index (DS lw/sw; runs past 255)
extern LONG Stage6_frame_cnt;		// 8011BCA8		// frame counter (DS lw/sw)
extern UCHAR Stage6_unk_bcdc;		// 8011BCDC		// unused in source
extern UCHAR Stage6_unk_bcd8;		// 8011BCD8		// unused in source
extern UCHAR Stage6_unk_bcec;		// 8011BCEC		// unused in source
extern USHORT Stage6_unk_bce2;		// 8011BCE2		// unused in source (DS sh)
extern USHORT Stage6_unk_bc6e;		// 8011BC6E		// unused in source

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

/* DS 0x80010804 (Stage6_main_ptr): stage main hook word in the image, set in STAGE6.C */
extern CODE_PTR Stage6_main_ptr;

#endif /* STAGE6_H_H */
