/*
 * PAD.H - Controller pad state and vibration request interface
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PAD.H
 */
#ifndef PAD_H
#define PAD_H
#include "TYPES.H"

/* 10-byte vibration request record (DS queue stride 10, 32 slots + end sentinel) */
typedef struct VIB_REQ
{
	UCHAR  Flg;			/* +0  active flag */
	UCHAR  Pow;			/* +1  motor power */
	USHORT Delay;		/* +2  start delay countdown */
	USHORT Time;		/* +4  remaining time countdown */
	short  Step;		/* +6  power change per frame (fade) */
	USHORT Pow_cur;		/* +8  power accumulator (power << 7) */
} VIB_REQ;

/* DS 0x800a26a0: button-mask table, 16 USHORT per Key_idx row */
extern USHORT Key_type[][16];

/* Pad globals (Pad_type1 is also declared in CONFIG.H and used by CONFIG.C;
 * Pad_act_flg is also used by PLAYER.C) */
extern UCHAR  Pad_type1;		/* DS 0x800e8984 pad mode: 4 digital, 7 analog */
extern UCHAR  Pad_type_old;		/* DS 0x800c3b40 pad mode of the previous frame */
extern UCHAR  Pad_act_flg;		/* DS 0x800e898c set when pad activity is seen this frame */
extern UCHAR  Pad_act_align[];	/* DS 0x8009dba4 PadSetActAlign table */

void Pad_set(void);
void    Pad_rep_set(ULONG Mask, USHORT Data);
void    Pad_vib_set0(int Time, USHORT Attr);
void    Pad_vib_set1(int Time, UCHAR Power, USHORT Attr);
void    Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, USHORT Attr);
void    Pad_vib_init(void);
UCHAR   SetPadActData(VIB_REQ *pReq);
VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq);
int     Pad_vib_count(VIB_REQ *pReq);



/* Globals defined in PAD.C */
	extern UCHAR PadActData[2];							// 0x800cbc20
	extern UCHAR UnknownPadInfoByte;					// 0x800d7654
	extern UCHAR UnknownPadSetFlag;						// 0x800e2a80
#endif /* PAD_H_H */
