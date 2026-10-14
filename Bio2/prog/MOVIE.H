/*
 * MOVIE.H - Movie playback structs, movie table and prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\MOVIE.H
 */
#ifndef MOVIE_H
#define MOVIE_H
#include <LIBGTE.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagV_STR {
	ULONG* Read_buff[2];
	ULONG Start_frame;
	ULONG End_frame;
	LONG* Func1;
	LONG* Func2;
	DVECTOR Read_header[2];
	ULONG Read_id;
	LONG Fd;
	ULONG Is_interleave;
	ULONG Subheader[8];
	ULONG Eof_flg;
};

/* ------------------------------------------------------------------ */
/* Structures (offsets verified against the DS disassembly)             */
/* ------------------------------------------------------------------ */

/* Movie table entry (32 bytes) */
struct tagMOVIE_TBL {
	ULONG   Filecode;       /* 0x00 index into floc[] */
	USHORT  N_frame;        /* 0x04 */
	USHORT  Caption_id;     /* 0x06 */
	USHORT  Cancel;         /* 0x08 skippable with the pad */
	USHORT  Disp_w;         /* 0x0a */
	USHORT  Isrgb24;        /* 0x0c */
	USHORT  pad_0e;
	ULONG   Pos;            /* 0x10 x | (y << 16) */
	USHORT  Work_top;       /* 0x14 0: 0x80198000, 1: 0x8014fc20, 2: G.pFree_work */
	USHORT  Rl_size;        /* 0x16 run-level buffer size (bytes) */
	USHORT  Attribute;      /* 0x18 */
	USHORT  Volume;         /* 0x1a percent of Volume_Bgm */
	USHORT  Free;           /* 0x1c start sector offset (x5) */
	USHORT  pad_1e;
};

struct tagVM_WORK {
	ULONG   *Rl_buff[2];    /* 0x00 */
	ULONG   *pImage_buff;   /* 0x08 */
	USHORT  End_frame;      /* 0x0c */
	USHORT  Movie_volume;   /* 0x0e */
	UCHAR   Rno;            /* 0x10 */
	UCHAR   Cbuf_id;        /* 0x11 */
	USHORT  Movie_w;        /* 0x12 */
	UCHAR   Movie_sync;     /* 0x14 */
	UCHAR   Status;         /* 0x15 */
	UCHAR   Eom;            /* 0x16 0 playing, 1 ended, 2 cancelled */
	UCHAR   Free0;          /* 0x17 */
	USHORT  Frame;          /* 0x18 */
	USHORT  Volume;         /* 0x1a */
	ULONG   Start_sector;   /* 0x1c */
	USHORT  Movie_cancel;   /* 0x20 (DS addition) */
};

struct tagVM_DISP {
	RECT    Rect;           /* 0x00 MDEC strip upload rect */
	TILE    Cinesco[2][2];  /* 0x08 */
	ULONG   Cine_switch;    /* 0x48 */
	USHORT  Pos_x;          /* 0x4c */
	USHORT  Cur_pos_x;      /* 0x4e */
	USHORT  Pos_y;          /* 0x50 */
	USHORT  Width;          /* 0x52 */
	USHORT  Size;           /* 0x54 */
	USHORT  Count;          /* 0x56 */
	USHORT  Loop;           /* 0x58 */
	USHORT  Attribute;      /* 0x5a */
	USHORT  Isrgb24;        /* 0x5c (DS addition) */
};

struct tagVM_FRAME {
	USHORT  No;             /* 0x00 current frame */
	USHORT  Old_no;         /* 0x02 previous frame */
	USHORT  Pic_h;          /* 0x04 */
	USHORT  Dummy;          /* 0x06 */
};

/* Fixed work areas selected by MOVIE_TBL.Work_top */
#define MOVIE_WORK_TOP1     0x8014fc20

/* CD stream ring buffer size in sectors */
#define MOVIE_RING_SIZE     0x16

/* ------------------------------------------------------------------ */
/* Data                                                                 */
/* ------------------------------------------------------------------ */
/* DS 0x800d7670 */ extern VM_WORK    Vm_work;
/* DS 0x800d75d8 */ extern VM_DISP    Vm_disp;
/* DS 0x800d763c */ extern VM_FRAME   *pVm_frm;
/* DS 0x800eae34 */ extern DslFILE    *pFile;
/* DS 0x800e8980 */ extern DslATV     *pAtv;
/* DS 0x800df338 */ extern UCHAR      *pMovie_result;
/* DS 0x800e2a88 */ extern UCHAR      Ret_Value[8];
/* DS 0x800d5b80 */ extern DslFILTER  Channel;
/* DS 0x800d8cd8 */ extern DslLOC     Loc;
/* DS 0x800a216c */ extern MOVIE_TBL  Movie_tbl[];

/* ------------------------------------------------------------------ */
/* Function prototypes                                                  */
/* ------------------------------------------------------------------ */
void    Init_movie_work(ULONG Id);                  /* 80032150 */
void    Movie(void);                                /* 80032340 */
ULONG   Open_movie(ULONG Movie_id);                 /* 80032ce8 */
ULONG   Play_movie(void);                           /* 80032fcc */
void    Close_movie(void);                          /* 80033118 */

#ifdef _MOVIE_C_
static void Movie_init(void);                       /* 80032450 */
static void Movie_main(void);                       /* 80032700 */
static void Movie_end(void);                        /* 80032984 */
static void Movie_exit(void);                       /* 80032ab8 */
static void RuntoImage(void);                       /* 8003317c */
static void Mdec_callback(void);                    /* 8003324c */
static void Movie_xa_start(void);                   /* 8003331c */
static void Movie_xa_stop(void);                    /* 80033370 */
static void Set_movie_volume2(ULONG Volume);        /* 800333c4 */
static LONG CdReset2(LONG Mode);                    /* 80033440 (no callers in DS) */
#endif


#endif /* MOVIE_H_H */
