/*
 * CD.H - CD/XA streaming work area and disc file-location types
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\CD.H
 */
#ifndef CD_H
#define CD_H

#include <LIBDS.H>
#include <LIBSPU.H>
#include <LIBAPI.H>
#include <LIBSND.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagfpos {
	ULONG size;
	USHORT sector;
	UCHAR sec_high;
	UCHAR sum;
};


/* ---------------------------------------------------------------------------
 * FLOC  --  one entry in the disc file-location table (floc[])
 * ---------------------------------------------------------------------------*/
struct tagFLOC {
	unsigned long  size;
	unsigned short sector;
	unsigned char  sec_high;
	unsigned char  sum;
};

/* ---------------------------------------------------------------------------
 * tagCD  --  CD / XA streaming work area (0x880 bytes)       DS 0x800d5300
 *
 * Field offsets verified against the DS disassembly of CD.C.
 * (Ghidra calls the type "CD"; the C typedef uses the LOG tag name tagCD
 *  because the global variable itself is also called CD.)
 * ---------------------------------------------------------------------------*/
#define CD_RING_BUFFERS  12

struct tagCD {
	unsigned char   Cd_rtn_no;                     /* 0x000 */
	unsigned char   Xa_rtn_no;                     /* 0x001 */
	unsigned char   Check_sum_flg;                 /* 0x002 */
	unsigned char   Type;                          /* 0x003 */
	DslLOC          Fpos;                          /* 0x004 */
	unsigned long   Fsize;                         /* 0x008 */
	unsigned long   Fsize_now;                     /* 0x00c */
	unsigned long   Fsize_read;                    /* 0x010 */
	unsigned long   Fsector;                       /* 0x014 */
	unsigned short  Fsector_num;                   /* 0x018 */
	unsigned short  Fsector_read;                  /* 0x01a */
	unsigned short  Fno;                           /* 0x01c */
	unsigned char   Fsum;                          /* 0x01e */
	unsigned char   Sum;                           /* 0x01f */
	unsigned long   Sum_cnt;                       /* 0x020 */
	unsigned long   Start_sector;                  /* 0x024 */
	unsigned long   Current_sector;                /* 0x028 */
	unsigned long   End_sector;                    /* 0x02c */
	unsigned long  *Read_adr;                      /* 0x030 */
	unsigned char   Use_mode;                      /* 0x034 */
	unsigned char   Limit_break;                   /* 0x035 */
	unsigned char   Time_ctr;                      /* 0x036 */
	unsigned char   Status;                        /* 0x037 */
	unsigned char   Result[12];                    /* 0x038 */
	unsigned char   Param[4];                      /* 0x044 */
	unsigned char   Work_buf[2048];                /* 0x048 */
	DslFILTER       Filter;                        /* 0x848 */
	DslATV          Cdvol;                         /* 0x84c */
	unsigned long   Vol;                           /* 0x850 */
	unsigned long  *Buff_adr;                      /* 0x854 */
	unsigned char   Read_ctr;                      /* 0x858 */
	unsigned char   Write_ctr;                     /* 0x859 */
	unsigned char   First_flg;                     /* 0x85a */
	unsigned char   Stereo_flg;                    /* 0x85b */
	unsigned char   Ring_status[CD_RING_BUFFERS];  /* 0x85c */
	unsigned short  Ring_size[CD_RING_BUFFERS];    /* 0x868 */
};

/* ---------------------------------------------------------------------------
 * Globals defined in CD.C
 * ---------------------------------------------------------------------------*/
/* DS 0x800d5300 */
extern struct tagCD CD;

/* ---------------------------------------------------------------------------
 * Globals defined externally
 * ---------------------------------------------------------------------------*/
/* DS 0x800988a4 */
extern FLOC           floc[];
/* DS 0x80010818  (rodata initialiser of Xa_play's local Xa_file[9]) */
extern SHORT Xa_no[10];          // 0x80010818 (retail: 0x343..0x34b, 0)
/* DS 0x8009a37c */
extern unsigned short Xa_sector00[][3];

/* ---------------------------------------------------------------------------
 * CD / XA interface (DS order)
 * ---------------------------------------------------------------------------*/
void          Cd_init(void);                                         /* 800128c4 */
void Cd_system_control(void);                                /* 80012984 */
void          Xa_play(unsigned long mode, unsigned long no);         /* 800129b4 */
void          Xa_error(void);                                        /* 80012c2c */
void          Xa_control(void);                                      /* 80012c68 */
void          Xa_control_stop(void);                                 /* 80012ca4 */
void          Xa_control_init(void);                                 /* 80012cd8 */
void          Xa_seek(unsigned char intr, unsigned char *result);    /* 80012d30 */
void          Xa_ready_cb(unsigned char intr, unsigned char *result);/* 80012dc8 */
void          Xa_control_play(void);                                 /* 80012e70 */
void          Xa_control_end(void);                                  /* 80012f48 */
unsigned long Cd_read(unsigned long File_no, unsigned long *Address,
					  unsigned long Mode, unsigned char *Mess);      /* 80012fb8 */
unsigned long Cd_read_s(unsigned long File_no, unsigned long Mode,
						unsigned long *Address, unsigned long Vab_id,
						unsigned char *Mess);                        /* 800132b0 */
void          Cd_seek(unsigned char intr, unsigned char *result);    /* 80013714 */
void          Cd_ready_cb(unsigned char intr, unsigned char *result);/* 8001376c */
void          Cd_set_stereo(unsigned long mode);                     /* 80013a7c */
void          Set_volume(unsigned long vol);                         /* 80013ac8 */
void          Change_volume(unsigned long mode);                     /* 80013b4c */


#endif /* CD_H_H */
