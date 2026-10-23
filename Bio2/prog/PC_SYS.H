/*
 * PC_SYS.H - PC screen windows, keyboard and console text
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PC_SYS.H
 */
#ifndef PC_SYS_H
#define PC_SYS_H
#include <LIBGPU.H>
#include "TYPES.H"

/* ---- types migrated from data.h ---- */
struct tagSP_WORK {
	USHORT Pos_x;
	USHORT Pos_y;
	USHORT W;
	USHORT H;
	UCHAR U;
	UCHAR V;
	USHORT Clut;
	UCHAR Pri;
	UCHAR Semi;
	UCHAR Col;
	UCHAR Be_flg;
};

struct tagCOM_WORK {
	UCHAR Cx;
	UCHAR Cy;
	CHAR Kx;
	CHAR Ky;
	UCHAR Con_buf[12][30];
	UCHAR Con_col_buf[12][30];
	UCHAR Disp_wait;
	UCHAR Cursor_disp;
	UCHAR Wait;
	UCHAR Ccol;
	UCHAR nMoji;
	UCHAR Moji_code;
	UCHAR Kcursor_disp;
	CHAR Kcursor_add;
	UCHAR Key_timer;
	UCHAR Cut_no_old;
	UCHAR Key_input_flg;
	UCHAR Keta;
	ULONG Num;
	UCHAR In_buf[17];
	UCHAR* pMess_start;
	UCHAR* pMess;
	SP_WORK Sp_work[364];
	DR_MODE Dr_mode[2][8];
	SPRT Packet[2][364];
	LINE_F2 Line_work[2][4];
};

/* Init_work_tbl: SP_WORK template table  */
extern SP_WORK Init_work_tbl[];
/* DS 0x80010ba4: message string passed to Cprint from Keyboard_Input  */
extern UCHAR s_Pc_sys_msg_80010ba4[8];

void    Windows(void);
ULONG   Keyboard_Input(ULONG Length, ULONG Flg);
void    Keyboard_set(ULONG Be_flg, ULONG W, ULONG H, ULONG Col);
ULONG   Keyboard(void);
ULONG   Wframe_in_out(void);
void    Wframe_set(ULONG X, ULONG Y, ULONG X2, ULONG Y2);
ULONG   Wframe_move(ULONG X, ULONG Y, ULONG W, ULONG H);
ULONG   Get_moji_code(UCHAR Moji);
ULONG   Cprint(ULONG Speed, UCHAR *pMess, ULONG *Mend);
void    Console_put(UCHAR Moji);
void    Console_roll_up(void);
void    Console_clr(void);
void    Console_trans(ULONG Be_flg);
void    Sprite_Trans(ULONG nPacket);
void    Set_sp_work(SP_WORK *pSp, SP_WORK *pSw);
VOID    Dr_mode_set(void);



/* Globals defined in PC_SYS.C */
	extern COM_WORK* pCom_work;							// 0x800ea23c
#endif /* PC_SYS_H_H */
