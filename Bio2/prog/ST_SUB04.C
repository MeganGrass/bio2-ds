/*
 * ST_SUB04.C - Status screen display routines (menus, items, files, ECG)
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ST_SUB04.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSPU.H>
#include "ST_SUB04.h"
#include "CD.H"
#include "EM_SUB.H"
#include "GAME.H"
#include "JOINT.H"
#include "MD_SUB.H"
#include "STATUS.H"
#include "ST_SUB00.H"
#include "ST_SUB02.H"
#include "SUB.H"
#include "MAIN.H"
#include "flag.h"

DR_MODE St_end_tag_c[2];		/* end-of-list tags for buffers 0,1, DS 0x800D6BF0 */

SPRT St_pool_c100[8];		/* Pool c100 sprites: 8, written by St_init_disp_file */

ST_POOL_C000 St_pool_c000;		/* itp and file select share this 0x50-byte pool */

SPRT St_itemget_sprt[16];		/* Item-get sprites: 8 x 2 = 16, written by St_init_disp_itp */

SPRT St_itp_sprt[2];		/* Itp sprites: 2, written by St_init_disp_itp */

LINE_F2 St_prim_f[64];		/* ECG lines: 32 x 2 = 64, written by St_init_disp_ECG */

SPRT St_prim_e[4];		/* ECG sprites: 4, written by St_init_disp_ECG; two buffers interleaved */

SPRT St_equip_sprt[14];		/* Equip sprites: 7 x 2 = 14, written by St_init_disp_equip */

SPRT St_itemlist_sprt[14];		/* Item list sprites: 7 x 2 = 14, written by St_init_disp_itemlist */

SPRT St_menu1_sprt[18];		/* Menu 1 sprites: up to 9 x 2 = 18, written by St_init_disp_menu1 */

SPRT St_menu0_sprt[16];		/* Menu 0 sprites: 8 x 2 = 16, written by St_init_disp_menu0 */

POLY_F4 St_frame_f4[2];		/* Frame polygons: 2, written by St_init_disp_frame */

SPRT St_frame_sprt[34];		/* Frame sprites: 17 x 2 = 34 (type != 0), written by St_init_disp_frame */

POLY_F4 St_message_f4[2];		/* Message polygons: 2, written by St_init_disp_message */

SPRT St_message_sprt[34];		/* Message sprites: 17 x 2 = 34, written by St_init_disp_message */

SPRT St_face_sprt2[20];		/* Face sub-sprites: 10 x 2 = 20, written by St_init_disp_face */

SPRT St_face_sprt[26];		/* Face sprites: 13 x 2 = 26, written by St_init_disp_face */

SPRT St_num_sprt[160];		/* Number sprites: 160, written by St_init_disp_num; two buffers interleaved */

SPRT St_cursol1_sprt[4];		/* Cursor 1: 2 x 2 sprites, written by St_init_disp_cursol1; two buffers interleaved */

SPRT St_cursol0_sprt[6];		/* Cursor 0: 3 x 2 sprites, written by St_init_disp_cursol0; two buffers interleaved */

/* Wall sprites: buffer 0 at [0], buffer 1 at [1]; St_disp_wall steps 2 per sprite (35 each) */
SPRT St_prim_d[70];

/* Retail tables (DS addresses in ST_SUB04.H comments); contents are read-only data in retail */
static SHORT N_pos[18][2] = {{6, 24},{46, 24},{6, 54},{46, 54},{6, 84},{46, 84},{6, 114},{46, 114},{6, 144},{46, 144},{46, -14},{-46, -15},{-67, -18},{-67, -18},{-67, 2},{-67, 22},{-67, 42},{-67, 62}};	/* DS 0x800AADF8 */
static UCHAR Cursol0_char_tbl[3][4] = {{208, 0, 22, 34},{210, 0, 40, 34},{230, 0, 22, 34}};	/* DS 0x800AAE40 */
static SHORT Cursol0_pos_tbl[9][2] = {{3, 1},{3, 1},{25, 1},{3, 1},{25, 1},{65, 1},{-37, 1},{-15, 1},{25, 1}};	/* DS 0x800AAE4C */
static UCHAR Cursol1_char_tbl[3][4] = {{160, 0, 22, 30},{180, 0, 22, 30},{236, 36, 20, 20}};	/* DS 0x800AAE70 */
static SHORT Cursol1_pos_tbl[9][2] = {{5, 3},{25, 3},{15, 8},{5, 3},{65, 3},{35, 8},{-35, 3},{25, 3},{-5, 8}};	/* DS 0x800AAE7C */
static UCHAR Face_char_tbl[13][4] = {{0, 200, 39, 10},{0, 214, 39, 20},{0, 235, 39, 20},{56, 164, 39, 4},{56, 168, 39, 2},{56, 171, 39, 4},{106, 92, 4, 59},{106, 152, 4, 59},{107, 242, 16, 14},{107, 242, 16, 14},{107, 242, 16, 14},{56, 178, 7, 7},{56, 178, 7, 7}};	/* DS 0x800AAEA0 */
static UCHAR Face_subchar_tbl[10][4] = {{44, 200, 39, 10},{44, 214, 39, 40},{56, 164, 39, 4},{56, 168, 39, 2},{56, 171, 39, 4},{106, 92, 4, 59},{106, 152, 4, 59},{107, 242, 16, 14},{107, 242, 16, 14},{107, 242, 16, 14}};	/* DS 0x800AAED4 */
static SHORT Face_pos_tbl[13][2] = {{4, 4},{4, 15},{4, 35},{4, 0},{4, 14},{4, 55},{0, 0},{42, 0},{-16, 0},{-16, 16},{-16, 32},{46, 16},{46, 44}};	/* DS 0x800AAEFC */
static SHORT Face_subpos_tbl[10][2] = {{4, 101},{4, 59},{4, 56},{4, 99},{4, 111},{0, 56},{42, 56},{-16, 74},{-16, 88},{-16, 102}};	/* DS 0x800AAF30 */
static UCHAR Message_char_tbl[17][4] = {{0, 147, 83, 4},{0, 88, 128, 4},{0, 151, 83, 4},{0, 84, 128, 4},{123, 92, 5, 26},{123, 151, 5, 23},{123, 174, 5, 26},{123, 233, 5, 23},{107, 242, 8, 14},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9}};	/* DS 0x800AAF58 */
static SHORT Message_pos_tbl[17][2] = {{0, 0},{83, 0},{0, 53},{83, 53},{0, 4},{0, 30},{206, 4},{206, 30},{211, 40},{219, 42},{235, 42},{251, 42},{267, 42},{283, 42},{299, 42},{315, 42},{331, 42}};	/* DS 0x800AAF9C */
static UCHAR Frame_char_tbl[17][4] = {{0, 147, 83, 4},{0, 72, 128, 12},{0, 151, 83, 4},{0, 84, 128, 4},{123, 92, 5, 82},{123, 174, 5, 82},{123, 92, 5, 59},{123, 174, 5, 59},{107, 242, 8, 14},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9},{80, 155, 16, 9}};	/* DS 0x800AAFE0 */
static SHORT Frame_pos_tbl[17][2] = {{0, 0},{83, -8},{0, 85},{83, 85},{0, 4},{206, 4},{0, 86},{206, 86},{211, 132},{219, 134},{235, 134},{251, 134},{267, 134},{283, 134},{299, 134},{315, 134},{331, 134}};	/* DS 0x800AB024 */
static UCHAR Menu0_char_tbl[8][4] = {{0, 46, 47, 10},{48, 46, 47, 10},{96, 46, 47, 10},{144, 46, 47, 10},{0, 164, 52, 12},{3, 164, 49, 12},{3, 164, 49, 12},{3, 164, 52, 12}};	/* DS 0x800AB068 */
static SHORT Menu0_pos_tbl[8][2] = {{53, 1},{102, 1},{151, 1},{200, 1},{49, 0},{101, 0},{150, 0},{199, 0}};	/* DS 0x800AB088 */
static UCHAR Menu1_char_tbl[9][4] = {{192, 56, 48, 16},{144, 56, 48, 16},{96, 56, 48, 16},{48, 56, 48, 16},{0, 176, 56, 13},{0, 189, 56, 21},{0, 189, 56, 21},{0, 189, 56, 21},{0, 189, 56, 21}};	/* DS 0x800AB0A8 */
static SHORT Menu1_pos_tbl[9][2] = {{4, 77},{4, 56},{4, 35},{4, 14},{0, 0},{0, 13},{0, 34},{0, 55},{0, 76}};	/* DS 0x800AB0CC */
static UCHAR Itemlist_char_tbl[7][4] = {{80, 30, 40, 30},{0, 0, 80, 60},{0, 211, 50, 41},{0, 140, 90, 3},{0, 143, 90, 4},{113, 92, 5, 150},{118, 92, 5, 150}};	/* DS 0x800AB0F0 */
static SHORT Itemlist_pos_tbl[7][2] = {{45, -35},{5, 3},{40, -41},{0, 0},{0, 153},{0, 3},{85, 3}};	/* DS 0x800AB10C */
static UCHAR Equip_char_tbl[7][4] = {{0, 150, 80, 30},{0, 0, 40, 30},{0, 155, 80, 3},{0, 158, 80, 6},{91, 164, 5, 39},{96, 164, 5, 39},{50, 211, 11, 39}};	/* DS 0x800AB128 */
static SHORT Equip_pos_tbl[7][2] = {{5, 6},{5, 6},{5, 36},{5, 0},{0, 0},{85, 0},{-7, 0}};	/* DS 0x800AB144 */
static UCHAR ECG_char_tbl[2][4] = {{0, 92, 99, 46},{0, 0, 46, 11}};	/* DS 0x800AB194 */
static UCHAR Itemget_char[8][4] = {{62, 201, 27, 20},{62, 221, 27, 20},{90, 218, 7, 19},{90, 218, 7, 19},{90, 218, 7, 19},{90, 218, 7, 19},{90, 218, 7, 19},{98, 218, 12, 19}};	/* DS 0x800AB19C */
static LONG v_p[3] = {0, 550, 1600};	/* DS 0x800AB1DC */
static LONG v_r[3] = {0, 0, 0};	/* DS 0x800AB1E8 */
static VECTOR pp = {0, 0, 0, 0};	/* DS 0x800AB1F4 */

/* Position word used by print_num: x in the high half, y in the low half (retail adds them inline) */
#define St_num_pos(x, y)	((ULONG)((x) * 0x10000 + (y)))

/* Mode word for print_num: the item's Num_col, with 10 in bits 16-23 when Num_col bit 7 is set */
#define St_num_mode(col)	((ULONG)(col) | ((((col) & 0x80) ? 10 : 0) << 16))

/* DS 0x80072c1c (St_init_disp_num) */
VOID St_init_disp_num(VOID)
{
	register SPRT *sp;
	register ULONG i;

	sp = St_num_sprt;
	for (i = 0; i < 160; i++) {
		setSprt(sp);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->w = 8;
		sp->h = 9;
		sp++;
	}
}

/* DS 0x80072c6c (St_disp_num) */
VOID St_disp_num(VOID)
{
	register SPRT *sp;
	register ITEM_WORK *i_work;
	register ULONG i;
	register STATUS *pSt;
	register LONG x_add;
	register ITEM_DATA *pItem;
	register LONG y_add;
	register ULONG item_no;
	register UCHAR sub_rtn;
	register UCHAR col;

	pSt = &St;
	pItem = Item_data_tbl;
	sp = St_num_sprt;
	x_add = 0;
	if (G.Cbuf_id != 0) {
		sp = &St_num_sprt[1];
	}
	i_work = G.SaveData.Item_boxwork + 1;	/* slot 12 first: Item_work[] runs on into Item_boxwork[] */
	i = 13;
	do {
		col = pItem[i_work->Id].Item_num_col;
		i--;
		if (i_work->Size == 2 || G.SaveData.Max_item_num <= i) {
			sp += 8;
		} else {
			sp = (SPRT *)print_num(sp, i_work->Num,
				St_num_pos(N_pos[i][0] + pSt->Itemlist_pos_x, N_pos[i][1] + pSt->Itemlist_pos_y),
				St_num_mode(col));
		}
		i_work--;
	} while (i != 0);

	item_no = pSt->Equip_item_no;
	i_work = G.SaveData.Item_work + item_no;
	if (item_no == 0x80) {
		sp += 8;
	} else {
		col = pItem[i_work->Id].Item_num_col;
		if (i_work->Size == 0) {
			x_add = 0x14;
		}
		if (pSt->St_type == 1) {
			x_add += 0x36;
		}
		sp = (SPRT *)print_num(sp, i_work->Num,
			St_num_pos(N_pos[11][0] + pSt->Itemlist_pos_x + x_add, N_pos[11][1] + pSt->Itemlist_pos_y),
			St_num_mode(col));
	}

	if (pSt->St_type == 1) {
		sub_rtn = pSt->Sub_rtn1;
		if (sub_rtn == 3) {
			y_add = 0x52;
			i_work = G.SaveData.Item_boxwork + ((pSt->Ibox_sel + 5) & 0x3f);
		}
		if (sub_rtn == 2) {
			y_add = -0x26;
			i_work = G.SaveData.Item_boxwork + ((pSt->Ibox_sel - 1) & 0x3f);
		}
		/* y_add is set whenever Sub_rtn1 is 2 or 3, the only case that reads it */
		col = pItem[i_work->Id].Item_num_col;
		if (sub_rtn == 2 || sub_rtn == 3) {
			sp = (SPRT *)print_num(sp, i_work->Num,
				St_num_pos(N_pos[12][0] + pSt->Itemlist_pos_x, pSt->Itemlist_pos_y + pSt->Ibox_moxe + y_add),
				St_num_mode(col));
		}
		i = 5;
		do {
			i--;
			i_work = G.SaveData.Item_boxwork + ((pSt->Ibox_sel + i) & 0x3f);
			sp = (SPRT *)print_num(sp, i_work->Num,
				St_num_pos(N_pos[13 + i][0] + pSt->Itemlist_pos_x,
					N_pos[13 + i][1] + pSt->Itemlist_pos_y + pSt->Ibox_moxe),
				St_num_mode(pItem[i_work->Id].Item_num_col));
		} while (i != 0);
	}
	AddPrim(&G.Ot0[G.Cbuf_id][3], &St.Dmode[0x10][G.Cbuf_id].code[1]);
}

/* DS 0x8007300c (print_num) */
ULONG print_num(register SPRT *sp, register ULONG in, register ULONG pos, register ULONG mode)
{
	static UCHAR num[4];
	register ULONG *ot;
	register SHORT x_base;
	register SHORT x_add;
	register LONG i;
	register ULONG u;
	register SPRT *sp_last;

	ot = &G.Ot0[G.Cbuf_id][3];
	x_add = 0;
	if ((mode & 0x7f) == 4) {
		return (ULONG)(sp + 8);
	}
	x_base = (SHORT)(pos >> 16);
	if (in != 0xff) {
		num[0] = (UCHAR)(mode >> 16);
		num[3] = (UCHAR)(in / 100);
		num[2] = (UCHAR)((in % 100) / 10);
		num[1] = (UCHAR)(in % 10);
		if ((mode & 0x10) == 0) {
			for (i = 3; i >= 0; i--) {
				if (((num[3] != 0) || (i != 3 && (num[2] != 0 || i != 2))) && (num[0] != 0 || i != 0)) {
					sp->x0 = x_base + x_add;
					sp->y0 = (SHORT)pos;
					x_add += 7;
					sp->u0 = num[i] << 3;
					sp->v0 = 0x1a;
					sp->clut = GetClut(0x100, (mode & 0xf) + 0x1ec);
					AddPrim(ot, sp);
				}
				sp += 2;
			}
			return (ULONG)sp;
		}
	}
	u = 0x50;
	for (i = 2; i > 0; i--) {
		sp_last = sp;
		u += 8;
		sp->x0 = x_base + x_add;
		sp->y0 = (SHORT)pos;
		sp->u0 = u;
		sp->v0 = 0x1a;
		sp->clut = GetClut(0x100, (mode & 0xf) + 0x1ec);
		AddPrim(ot, sp_last);
		x_add += 7;
		sp += 2;
	}
	return (ULONG)(sp_last + 6);
}

/* DS 0x80073250 (St_init_disp_cursol0) */
VOID St_init_disp_cursol0(VOID)
{
	register SPRT *sp;
	register STATUS *pSt;
	register USHORT clut;
	register LONG i;
	register LONG j;

	pSt = &St;
	sp = St_cursol0_sprt;
	clut = GetClut(0x100, 0x1eb);
	for (i = 2; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Cursol0_char_tbl[i][0];
			sp->v0 = Cursol0_char_tbl[i][1];
			sp->w = Cursol0_char_tbl[i][2];
			sp->h = Cursol0_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	pSt->Mcursol0 = 0;
	pSt->blink_sw0 = 0;
	pSt->blink_timer0 = 0x32;
	pSt->Add_cp_y0 = 0;
	pSt->Add_cp_x0 = 0;
	pSt->Y0 = 0;
	pSt->X0 = 0;
}

/* DS 0x80073350 (St_disp_cursol0) */
VOID St_disp_cursol0(register ULONG type)
{
	register SPRT *sp;
	register ULONG *ot;
	register SHORT (*pos)[2];
	register LONG x;
	register LONG y;
	register LONG dx;
	register UCHAR blink;

	ot = &G.Ot0[G.Cbuf_id][2];
	if (St.Sub_rtn1 == 0) {
		if (St.blink_sw0 == 0) {
			blink = St.blink_timer0 + 3;
			if (St.blink_timer0 > 0x78) {
				St.blink_sw0 = 1;
				blink = St.blink_timer0 + 3;
			}
		} else {
			blink = St.blink_timer0 - 3;
			if (St.blink_timer0 < 0x32) {
				St.blink_sw0 = 0;
			}
		}
	} else {
		blink = 0x3c;
	}
	x = St.Itemlist_pos_x + St.X0;
	if (St.Icursol_main == 10) {
		dx = 0x28;
		y = St.Itemlist_pos_y + St.Y0 - 0x26;
	} else {
		dx = (St.Icursol_main & 1) * 0x28;
		y = St.Itemlist_pos_y + St.Y0 + (St.Icursol_main >> 1) * 0x1e;
	}
	sp = St_cursol0_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_cursol0_sprt[1];
	}
	St.blink_timer0 = blink;
	for (pos = &Cursol0_pos_tbl[type * 3 + 2]; pos >= &Cursol0_pos_tbl[type * 3]; pos--) {
		sp->x0 = (*pos)[0] + x + dx;
		sp->y0 = (*pos)[1] + y;
		sp->r0 = blink;
		sp->g0 = blink;
		sp->b0 = blink;
		AddPrim(ot, sp);
		sp += 2;
	}
	AddPrim(ot, &St.Dmode[0xb][G.Cbuf_id].code[1]);
}

/* DS 0x80073578 (St_init_disp_cursol1) */
VOID St_init_disp_cursol1(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;

	sp = St_cursol1_sprt;	/* DS loads the base pointer into s0 before the GetClut call */
	clut = GetClut(0x100, 0x1eb);
	for (i = 1; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Cursol1_char_tbl[i][0];
			sp->v0 = Cursol1_char_tbl[i][1];
			sp->w = Cursol1_char_tbl[i][2];
			sp->h = Cursol1_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	St.blink_sw1 = 0;
	St.blink_timer1 = 0x50;
	St.Add_cp_y1 = 0;
	St.Add_cp_x1 = 0;
	St.Y1 = 0;
	St.X1 = 0;
	St.Mcursol1 = St.Mcursol0;
}

/* DS 0x80073674 (St_disp_cursol1) */
VOID St_disp_cursol1(register ULONG type)
{
	register SPRT *sp;
	register ULONG *ot;
	register SHORT (*pos)[2];
	register LONG x;
	register LONG y;
	register LONG dx;
	register UCHAR blink;

	ot = &G.Ot0[G.Cbuf_id][2];
	if (St.blink_sw1 == 0) {
		blink = St.blink_timer1 + 3;
		if (St.blink_timer1 > 0x7c) {
			St.blink_sw1 = 1;
			blink = St.blink_timer1 + 3;
		}
	} else {
		blink = St.blink_timer1 - 3;
		if (St.blink_timer1 < 0x46) {
			St.blink_sw1 = 0;
		}
	}
	x = St.Itemlist_pos_x + St.X1;
	if (St.Icursol_sub == 10) {
		dx = 0x28;
		y = St.Itemlist_pos_y + St.Y1 - 0x26;
	} else {
		dx = (St.Icursol_sub & 1) * 0x28;
		y = St.Itemlist_pos_y + St.Y1 + (St.Icursol_sub >> 1) * 0x1e;
	}
	sp = St_cursol1_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_cursol1_sprt[1];
	}
	St.blink_timer1 = blink;
	for (pos = &Cursol1_pos_tbl[type * 3 + 1]; pos >= &Cursol1_pos_tbl[type * 3]; pos--) {
		sp->x0 = (*pos)[0] + x + dx;
		sp->y0 = (*pos)[1] + y;
		sp->r0 = blink;
		sp->g0 = blink;
		sp->b0 = blink;
		AddPrim(ot, sp);
		sp += 2;
	}
	AddPrim(ot, &St.Dmode[0xc][G.Cbuf_id].code[1]);
}

/* DS 0x80073880 (St_init_disp_face) */
VOID St_init_disp_face(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;

	clut = GetClut(0, 0x1e4);
	if (G.Player.Id == 13) {
		Face_char_tbl[0][0] = 0x2a;
		Face_char_tbl[2][0] = 0;
		Face_char_tbl[2][1] = 0xb4;
		Face_char_tbl[1][1] = 0xb4;
		Face_char_tbl[0][1] = 0xb4;
	} else {
		if (G.Player.Id < 14) {
			Face_char_tbl[2][0] = 0x58;
			if (G.Player.Id != 12) {
				Face_char_tbl[2][0] = 0;
			}
		} else {
			Face_char_tbl[2][0] = 0x2c;
		}
		Face_char_tbl[0][1] = 200;
		Face_char_tbl[1][1] = 0xd6;
		Face_char_tbl[2][1] = 0xeb;
		Face_char_tbl[0][0] = Face_char_tbl[2][0];
	}
	Face_char_tbl[1][0] = Face_char_tbl[2][0];	/* DS sets this for both player branches */
	sp = St_face_sprt;
	for (i = 12; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Face_char_tbl[i][0];
			sp->v0 = Face_char_tbl[i][1];
			sp->w = Face_char_tbl[i][2];
			sp->h = Face_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	sp = St_face_sprt2;
	for (i = 9; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Face_subchar_tbl[i][0];
			sp->v0 = Face_subchar_tbl[i][1];
			sp->w = Face_subchar_tbl[i][2];
			sp->h = Face_subchar_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
}

/* DS 0x80073ab0 (St_disp_face) */
VOID St_disp_face(VOID)
{
	tagGLOBAL *pG = &G;
	STATUS *pSt = &St;
	register SPRT *sp;
	register SPRT *sp2;
	register ULONG *ot;
	register LONG j;
	register LONG sx;
	register LONG sy;
	register LONG x1;
	register LONG y1;
	register ULONG clut;
	register BOOL sub_on;

	sx = pSt->Status_pos_x - 0x33;
	sy = pSt->Status_pos_y - 0xd;
	x1 = pSt->Sub_pos_x + sx;
	y1 = pSt->Sub_pos_y + sy + 6;
	ot = &pG->Ot0[pG->Cbuf_id][6];
	sp = St_face_sprt;
	if (pG->Cbuf_id != 0) {
		sp = &St_face_sprt[1];
	}
	sp2 = St_face_sprt2;
	sub_on = (pSt->St_type == 0 && (pG->Status_flg & STAT_SUB_PLAYER) != 0);
	for (j = 12; j >= 3; j--) {
		sp->x0 = Face_pos_tbl[j][0] + sx;
		sp->y0 = Face_pos_tbl[j][1] + sy;
		AddPrim(ot, sp);
		sp += 2;
	}
	if (sub_on) {
		for (j = 9; j >= 2; j--) {
			sp2->x0 = Face_subpos_tbl[j][0] + x1;
			sp2->y0 = Face_subpos_tbl[j][1] + y1;
			AddPrim(ot, sp2);
			sp2 += 2;
		}
	}
	AddPrim(ot, &pSt->Dmode[9][pG->Cbuf_id].code[1]);
	clut = GetClut(0, 0x1e6);
	for (j = 2; j >= 0; j--) {
		sp->x0 = Face_pos_tbl[j][0] + sx;
		sp->y0 = Face_pos_tbl[j][1] + sy;
		sp->clut = clut;
		AddPrim(ot, sp);
		sp += 2;
	}
	if (sub_on) {
		for (j = 1; j >= 0; j--) {
			sp2->x0 = Face_subpos_tbl[j][0] + x1;
			sp2->y0 = Face_subpos_tbl[j][1] + y1;
			sp2->clut = clut;
			AddPrim(ot, sp2);
			sp2 += 2;
		}
	}
	AddPrim(ot, &pSt->Dmode[0x14][pG->Cbuf_id].code[1]);
}

/* DS 0x80073da0 (St_init_disp_message) */
VOID St_init_disp_message(VOID)
{
	register POLY_F4 *pf4;
	register SPRT *sp;
	register STATUS *pSt;
	register USHORT clut;
	register LONG i;
	register LONG j;

	pf4 = St_message_f4;
	sp = St_message_sprt;
	pSt = &St;
	clut = GetClut(0, 0x1e4);
	for (i = 16; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Message_char_tbl[i][0];
			sp->v0 = Message_char_tbl[i][1];
			sp->w = Message_char_tbl[i][2];
			sp->h = Message_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	for (j = 0; j < 2; j++) {
		setPolyF4(pf4);
		setSemiTrans(pf4, 1);
		pf4->r0 = 0x10;
		pf4->g0 = 0x10;
		pf4->b0 = 0x10;
		pf4++;
	}
	pSt->Message_pos_x = 7;
	pSt->Message_pos_y = 0xaa;
}

/* DS 0x80073ed0 (St_disp_message) */
VOID St_disp_message(VOID)
{
	register SPRT *sp;
	register POLY_F4 *pf4;
	register ULONG *ot;
	register SHORT (*pos)[2];
	register LONG x;
	register LONG y;

	x = St.Message_pos_x;
	y = St.Message_pos_y;
	ot = &G.Ot0[G.Cbuf_id][6];
	pf4 = St_message_f4;
	sp = St_message_sprt;
	if (G.Cbuf_id != 0) {
		pf4 = &St_message_f4[1];
		sp = &St_message_sprt[1];
	}
	/* retail stores the backing quad after the sprite loop; the stores are independent of it */
	pf4->x0 = x;
	pf4->y0 = y;
	pf4->x1 = x + 212;
	pf4->y1 = y;
	pf4->x2 = x;
	pf4->y2 = y + 53;
	pf4->x3 = x + 212;
	pf4->y3 = y + 53;
	pos = &Message_pos_tbl[17];
	do {
		pos--;
		sp->x0 = (*pos)[0] + x;
		sp->y0 = (*pos)[1] + y;
		AddPrim(ot, sp);
		sp += 2;
	} while (pos != Message_pos_tbl);
	AddPrim(ot, pf4);
	AddPrim(ot, &St.Dmode[8][G.Cbuf_id].code[1]);
}

/* DS 0x80074014 (St_init_disp_frame) */
VOID St_init_disp_frame(ULONG type)
{
	register SPRT *sp;
	register POLY_F4 *pf4;
	register USHORT clut;
	register LONG i;
	register LONG j;
	register LONG rows;
	register STATUS *status;

	status = &St;
	pf4 = St_frame_f4;
	sp = St_frame_sprt;
	clut = GetClut(0, 0x1e4);
	rows = 6;
	if (type != 0) {
		rows = 0x11;
	}
	for (i = rows - 1; i >= 0; i--) {
		for (j = 2; j != 0; j--) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Frame_char_tbl[i][0];
			sp->v0 = Frame_char_tbl[i][1];
			sp->w = Frame_char_tbl[i][2];
			sp->h = Frame_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	for (j = 0; j < 2; j++) {
		setPolyF4(pf4);
		setSemiTrans(pf4, 1);
		pf4->r0 = 0x10;
		pf4->g0 = 0x40;
		pf4->b0 = 0x10;
		pf4++;
	}
	status->Frame_pos_x = 7;
	status->Frame_pos_y = 0x4e;
}

/* DS 0x80074160 (St_disp_frame) */
VOID St_disp_frame(register ULONG type)
{
	register SPRT *sp;
	register POLY_F4 *pf4;
	register ULONG *ot;
	register LONG i;
	register LONG x;
	register LONG y;
	register LONG rows;
	register LONG h_add;
	ULONG saved_type;

	saved_type = type;	/* retail keeps a copy of type in its frame (sw a0,0x10(sp)) */
	x = St.Frame_pos_x;
	y = St.Frame_pos_y;
	ot = &G.Ot0[G.Cbuf_id][6];
	rows = 6;
	if (type == 0) {
		h_add = 0x5a;
		Frame_pos_tbl[2][1] = 0x55;
	} else {
		rows = 0x11;
		h_add = 0x93;
		Frame_pos_tbl[2][1] = 0x91;
	}
	pf4 = St_frame_f4;
	sp = St_frame_sprt;
	if (G.Cbuf_id != 0) {
		pf4 = &St_frame_f4[1];
		sp = &St_frame_sprt[1];
	}
	Frame_pos_tbl[3][1] = Frame_pos_tbl[2][1];
	for (i = rows - 1; i >= 0; i--) {
		sp->x0 = Frame_pos_tbl[i][0] + x;
		sp->y0 = Frame_pos_tbl[i][1] + y;
		AddPrim(ot, sp);
		sp += 2;
	}
	pf4->x0 = x;
	pf4->y0 = y;
	pf4->x1 = x + 0xd2;
	pf4->y1 = y;
	pf4->x2 = x;
	pf4->y2 = y + h_add;
	pf4->x3 = x + 0xd2;
	pf4->y3 = y + h_add;
	if (saved_type == 0) {
		AddPrim(ot, pf4);
	}
	AddPrim(ot, &St.Dmode[7][G.Cbuf_id].code[1]);
}

/* DS 0x80074300 (St_init_disp_menu0) */
VOID St_init_disp_menu0(VOID)
{
	register SPRT *sp;
	register STATUS *pSt;
	register USHORT clut;
	register LONG i;
	register LONG j;

	pSt = &St;
	sp = St_menu0_sprt;
	clut = GetClut(0, 0x1e4);
	for (i = 7; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Menu0_char_tbl[i][0];
			sp->v0 = Menu0_char_tbl[i][1];
			sp->w = Menu0_char_tbl[i][2];
			sp->h = Menu0_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	pSt->Menu0_pos_x = 0x3b;
	pSt->Menu0_pos_y = 0x10;
	pSt->Mcursol0 = 2;
}

/* DS 0x800743f8 (St_disp_menu0) */
VOID St_disp_menu0(VOID)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG i;
	register LONG x;
	register LONG y;
	register STATUS *pSt;

	pSt = &St;
	x = pSt->Menu0_pos_x;
	y = pSt->Menu0_pos_y;
	ot = &G.Ot0[G.Cbuf_id][2];
	sp = St_menu0_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_menu0_sprt[1];
	}
	i = 8;
	do {
		i--;
		sp->x0 = Menu0_pos_tbl[i][0] + x;
		sp->y0 = Menu0_pos_tbl[i][1] + y;
		AddPrim(ot, sp);
		sp += 2;
	} while (i > 4);
	AddPrim(ot, &St.Dmode[4][G.Cbuf_id].code[1]);
	do {
		i--;
		if (pSt->St_type == 0 && pSt->Mcursol0 == i) {
			if (pSt->Sub_rtn0 < 3) {
				sp->r0 = 0x80;
				sp->g0 = 0x80;
				sp->b0 = 0x80;
			} else {
				sp->r0 = 0x80;
				sp->g0 = 0x40;
				sp->b0 = 0x40;
			}
		} else {
			sp->r0 = 0x28;
			sp->g0 = 0x28;
			sp->b0 = 0x28;
		}
		sp->x0 = Menu0_pos_tbl[i][0] + x;
		sp->y0 = Menu0_pos_tbl[i][1] + y;
		sp->clut = GetClut(0x100, 0x1f0);
		AddPrim(ot, sp);
		sp += 2;
	} while (i != 0);
	AddPrim(ot, &St.Dmode[0xe][G.Cbuf_id].code[1]);
}

/* DS 0x80074610 (St_init_disp_menu1) */
VOID St_init_disp_menu1(register ULONG ctype, register ULONG wtype)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;
	register LONG rows;

	clut = GetClut(0, 0x1e4);
	rows = (wtype != 0) ? 9 : 8;
	if ((ctype & 1) == 0) {
		Menu1_char_tbl[3][0] = 0x30;
	} else {
		Menu1_char_tbl[3][0] = 0;
	}
	if ((ctype & 2) == 0) {
		Menu1_char_tbl[0][0] = 0xc0;
		Menu1_char_tbl[0][1] = 0x38;
	} else {
		Menu1_char_tbl[0][0] = 0x68;
		Menu1_char_tbl[0][1] = 0x13;
	}
	sp = St_menu1_sprt;
	for (i = rows - 1; i >= 4; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Menu1_char_tbl[i][0];
			sp->v0 = Menu1_char_tbl[i][1];
			sp->w = Menu1_char_tbl[i][2];
			sp->h = Menu1_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	clut = GetClut(0x100, 0x1ea);
	for (i = 3; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			if (i == (LONG)St.Mcursol1) {
				sp->r0 = 0x40;
				sp->g0 = 0x40;
			} else {
				sp->r0 = 0x80;
				sp->g0 = 0x80;
			}
			sp->b0 = 0x80;
			sp->u0 = Menu1_char_tbl[i][0];
			sp->v0 = Menu1_char_tbl[i][1];
			sp->w = Menu1_char_tbl[i][2];
			sp->h = Menu1_char_tbl[i][3];
			GetClut(0x100, 0x1ea);	/* retail calls GetClut again here and discards the result */
			sp->clut = clut;
			sp++;
		}
	}
	St.Menu1_pos_x = 0xa2;
	St.Menu1_pos_y = 0x46;
	St.Mcursol1 = 3;
}

/* DS 0x80074888 (St_disp_menu1) */
VOID St_disp_menu1(register ULONG wtype)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG i;
	register LONG j;
	register LONG x;
	register LONG y;
	register ULONG cursol;

	x = St.Menu1_pos_x;
	y = St.Menu1_pos_y;
	cursol = St.Mcursol1;
	ot = &G.Ot0[G.Cbuf_id][6];
	sp = St_menu1_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_menu1_sprt[1];
	}
	if (wtype == 0) {
		Menu1_pos_tbl[0][1] = 0x38;
		i = 8;
	} else {
		Menu1_pos_tbl[0][1] = 0x4d;
		i = 9;
	}
	do {
		i--;
		sp->x0 = Menu1_pos_tbl[i][0] + x;
		sp->y0 = Menu1_pos_tbl[i][1] + y;
		AddPrim(ot, sp);
		sp += 2;
	} while (i > 4);
	AddPrim(ot, &St.Dmode[10][G.Cbuf_id].code[1]);
	i = 4;
	do {
		i--;
		j = i;
		if (j == (LONG)cursol) {
			sp->r0 = 0x40;
			sp->g0 = 0x40;
		} else {
			sp->r0 = 0x80;
			sp->g0 = 0x80;
		}
		sp->b0 = 0x80;
		sp->x0 = Menu1_pos_tbl[j][0] + x;
		sp->y0 = Menu1_pos_tbl[j][1] + y;
		sp->clut = GetClut(0x100, 0x1ea);
		AddPrim(ot, sp);
		sp += 2;
	} while (i != 0);
	AddPrim(ot, &St.Dmode[0xf][G.Cbuf_id].code[1]);
}

/* DS 0x80074a94 (St_init_disp_itemlist) */
VOID St_init_disp_itemlist(VOID)
{
	register SPRT *sp;
	register STATUS *pSt;
	register USHORT clut;
	register LONG i;
	register LONG j;

	pSt = &St;
	sp = St_itemlist_sprt;
	clut = GetClut(0, 0x1e4);
	if (G.Player.Id < 0xe) {
		Itemlist_char_tbl[0][1] = 0x1e;
	} else {
		Itemlist_char_tbl[0][1] = 0x3c;
	}
	for (i = 6; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Itemlist_char_tbl[i][0];
			sp->v0 = Itemlist_char_tbl[i][1];
			sp->w = Itemlist_char_tbl[i][2];
			sp->h = Itemlist_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
	pSt->Itemlist_pos_x = 0xdc;
	pSt->Itemlist_pos_y = 0x46;
}

/* DS 0x80074bb0 (St_disp_itemlist) */
VOID St_disp_itemlist(VOID)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG j;
	register LONG x;
	register LONG y;
	register ULONG max_num;

	x = St.Itemlist_pos_x;
	y = St.Itemlist_pos_y;
	max_num = G.SaveData.Max_item_num;
	Itemlist_pos_tbl[4][1] = (SHORT)((G.SaveData.Max_item_num - 6) * 0xf + 0x5d);
	ot = &G.Ot0[G.Cbuf_id][5];
	sp = St_itemlist_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_itemlist_sprt[1];
	}
	j = 7;
	do {
		j--;
		sp->w = 5;
		sp->h = (G.SaveData.Max_item_num >> 1) * 30;
		sp->x0 = Itemlist_pos_tbl[j][0] + x;
		sp->y0 = Itemlist_pos_tbl[j][1] + y;
		AddPrim(ot, sp);
		sp += 2;
	} while (j > 5);
	do {
		j--;
		sp->x0 = Itemlist_pos_tbl[j][0] + x;
		sp->y0 = Itemlist_pos_tbl[j][1] + y;
		if (St.St_type != 1 || j != 2) {
			AddPrim(ot, sp);
		}
		sp += 2;
	} while (j > 2);
	AddPrim(ot, &St.Dmode[5][G.Cbuf_id].code[1]);
	do {
		j--;
		sp->x0 = Itemlist_pos_tbl[j][0] + x;
		sp->y0 = Itemlist_pos_tbl[j][1] + y;
		if (j == 0) {
			if (G.SaveData.Item_work[10].Id == 0x4e) {
				sp->u0 = 0x50;
				sp->v0 = 0x5a;
			}
			if (G.SaveData.Item_work[10].Id == 0x52) {
				sp->u0 = 0x50;
				sp->v0 = 0x78;
			}
		} else {
			sp->w = 80;
			sp->h = (max_num >> 1) * 30;
		}
		sp->clut = GetClut(0, 0x1e5);
		AddPrim(ot, sp);
		sp += 2;
	} while (j != 0);
	AddPrim(ot, &St.Dmode[0x11][G.Cbuf_id].code[1]);
}

/* DS 0x80074e80 (St_init_disp_equip) */
VOID St_init_disp_equip(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;

	sp = St_equip_sprt;	/* DS loads the base pointer into s0 before the GetClut call */
	clut = GetClut(0, 0x1e4);
	for (i = 6; i >= 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = Equip_char_tbl[i][0];
			sp->v0 = Equip_char_tbl[i][1];
			sp->w = Equip_char_tbl[i][2];
			sp->h = Equip_char_tbl[i][3];
			sp->clut = clut;
			sp++;
		}
	}
}

/* DS 0x80074f4c (St_disp_equip) */
VOID St_disp_equip(VOID)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG j;
	register LONG x;
	register LONG y;
	register ULONG equip_no;
	register UCHAR size;

	equip_no = St.Equip_item_no;
	ot = &G.Ot0[G.Cbuf_id][4];
	y = St.Itemlist_pos_y - 0x29;
	size = G.SaveData.Item_work[equip_no].Size;
	sp = St_equip_sprt;
	if (G.Cbuf_id != 0) {
		sp = &St_equip_sprt[1];
	}
	x = St.Itemlist_pos_x - 52;
	j = 7;
	if (St.St_type == 1) {
		j = 6;
		sp += 2;
		x = St.Itemlist_pos_x;
	}
	do {
		j--;
		sp->x0 = Equip_pos_tbl[j][0] + x;
		sp->y0 = Equip_pos_tbl[j][1] + y;
		AddPrim(ot, sp);
		sp += 2;
	} while (j > 2);
	AddPrim(ot, &St.Dmode[6][G.Cbuf_id].code[1]);
	if (size == 0) {
		sp->x0 = Equip_pos_tbl[0][0] + x + 0x14;
		sp->y0 = Equip_pos_tbl[0][1] + y;
		sp->w = 0x28;
		sp->h = 0x1e;
	} else {
		sp->x0 = Equip_pos_tbl[0][0] + x;
		sp->y0 = Equip_pos_tbl[0][1] + y;
		sp->w = 0x50;
		sp->h = 0x1e;
		if (size == 2) {
			equip_no--;
		}
	}
	sp->u0 = (equip_no & 1) * 0x28;
	sp->v0 = (equip_no >> 1) * 0x1e;
	sp->clut = GetClut(0, 0x1e5);
	if (St.Equip_item_no != 0x80) {
		AddPrim(ot, sp);
	}
	sp += 2;
	sp->x0 = Equip_pos_tbl[1][0] + x;
	sp->y0 = Equip_pos_tbl[1][1] + y;
	sp->clut = GetClut(0, 0x1e5);
	AddPrim(ot, sp);
	AddPrim(ot, &St.Dmode[0x13][G.Cbuf_id].code[1]);
}

/* DS 0x800751ec (St_init_disp_wall) */
VOID St_init_disp_wall(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;

	clut = GetClut(0, 0x1e5);
	sp = St_prim_d;
	for (i = 0x46; i > 0; i--) {
		setSprt(sp);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->u0 = 0x50;
		sp->v0 = 0x96;
		sp->w = 0x30;
		sp->h = 0x30;
		sp->clut = clut;
		sp++;
	}
}

/* DS 0x8007526c (St_disp_wall) */
VOID St_disp_wall(VOID)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG i;
	register LONG j;
	register LONG x;
	register LONG y;

	sp = St_prim_d;
	if (G.Cbuf_id != 0) {
		sp = &St_prim_d[1];
	}
	ot = &G.Ot0[G.Cbuf_id][7];
	y = 0xc0;
	for (j = 5; j > 0; j--) {
		x = 0x150;
		for (i = 7; i > 0; i--) {
			x -= 0x30;
			sp->x0 = x;
			sp->y0 = y;
			AddPrim(ot, sp);
			sp += 2;
		}
		y -= 0x30;
	}
	AddPrim(ot, &St.Dmode[0x12][G.Cbuf_id].code[1]);
}

/* DS 0x80075360 (St_init_disp_ECG) */
VOID St_init_disp_ECG(VOID)
{
	register SPRT *sp;
	register LINE_F2 *lf;
	register USHORT clut;
	register LONG i;
	register LONG j;

	sp = St_prim_e;
	lf = St_prim_f;
	clut = GetClut(0, 0x1e4);
	for (i = 4; i >= 0; i -= 4) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = ECG_char_tbl[i / 4][0];
			sp->v0 = ECG_char_tbl[i / 4][1];
			sp->w = ECG_char_tbl[i / 4][2];
			sp->h = ECG_char_tbl[i / 4][3];
			sp->clut = clut;
			sp++;
		}
	}
	for (i = 0x40; i > 0; i--) {
		setLineF2(lf);
		lf->code |= 2;
		lf->r0 = 0x80;
		lf->g0 = 0x80;
		lf->b0 = 0x80;
		lf++;
	}
	St.Heal_x = 10;
	St.Cure_y = 0x46;
	St.Status_pos_x = 0x3a;
	St.Ecg_x = 0;
	St.Ecg_rtn = 0;
	St.Recover_type = 0;
	St.Status_pos_y = 0x1d;
}

/* DS 0x800754ac (St_disp_ECG) */
VOID St_disp_ECG(register ULONG type)
{
	register SPRT *sp;
	register LINE_F2 *lf;
	register ECG_POINT *pTbl;
	register ULONG *ot;
	register STATUS *pSt;
	register LONG i;
	register LONG diff;
	register ULONG dx;
	register UCHAR cv;
	register UCHAR *col;

	sp = St_prim_e;
	lf = St_prim_f;
	i = 32;
	pSt = &St;
	pTbl = Ecg_line_tbl[type];
	ot = &G.Ot0[G.Cbuf_id][2];
	if (G.Cbuf_id != 0) {
		lf = &St_prim_f[1];
		sp = &St_prim_e[1];
	}
	St.Ecg_x = (St.Ecg_x + 1) & 0x7f;
	switch (St.Recover_type) {
	case 0:		/* normal trace: 32 fading segments behind Ecg_x */
		col = Ecg_col_tbl[type];	/* {r, g, b, r step, g step, b step} */
		do {
			i--;
			dx = (ULONG)(pSt->Ecg_x - i);
			if (dx < 80) {
				cv = (UCHAR)i;
				lf->r0 = col[0] - col[3] * cv;
				lf->g0 = col[1] - col[4] * cv;
				lf->b0 = col[2] - col[5] * cv;
				lf->x0 = pSt->Status_pos_x + (SHORT)dx + 12;
				lf->x1 = pSt->Status_pos_x + (SHORT)dx + 12;
				lf->y0 = pSt->Status_pos_y + pTbl[dx].Y + 2;
				lf->y1 = pSt->Status_pos_y + pTbl[dx].Y + 2 + pTbl[dx].H;
				AddPrim(ot, lf);
			}
			lf += 2;
		} while (i != 0);
		break;
	case 1:		/* heal: green sweep from left to right */
		if (St.Ecg_rtn != 0) {
			St.Heal_x = 10;
			St.Recover_type = 0;
			St.Ecg_rtn = 0;
			St.Ecg_x = 0;
			break;
		}
		St.Heal_x += 3;
		do {
			i--;
			diff = pSt->Heal_x - i;
			if ((ULONG)(diff - 11) < 82) {
				lf->r0 = 0x10;
				lf->g0 = (UCHAR)~(i * 8);
				lf->b0 = 0x10;
				lf->x0 = pSt->Status_pos_x + diff;
				lf->x1 = pSt->Status_pos_x + diff;
				lf->y0 = pSt->Status_pos_y + 4;
				lf->y1 = pSt->Status_pos_y + 35;
				AddPrim(ot, lf);
			}
			lf += 2;
		} while (i != 0);
		if (St.Heal_x > 128) {
			St.Ecg_rtn = 1;
		}
		break;
	case 2:		/* cure: blue sweep from bottom to top */
		if (St.Ecg_rtn != 0) {
			St.Cure_y = 70;
			St.Recover_type = 0;
			St.Ecg_rtn = 0;
			St.Ecg_x = 0;
			break;
		}
		St.Cure_y -= 3;
		do {
			i--;
			diff = pSt->Cure_y - i;
			if ((ULONG)(diff - 3) < 33) {
				lf->r0 = 0x10;
				lf->g0 = 0x10;
				lf->b0 = (UCHAR)(i * 8);
				lf->x0 = pSt->Status_pos_x + 11;
				lf->x1 = pSt->Status_pos_x + 92;
				lf->y0 = pSt->Status_pos_y + diff;
				lf->y1 = pSt->Status_pos_y + diff;
				AddPrim(ot, lf);
			}
			lf += 2;
		} while (i != 0);
		if (St.Cure_y <= -21) {
			St.Ecg_rtn = 1;
		}
		break;
	case 3:		/* heal: yellow sweep from left to right */
		if (St.Ecg_rtn != 0) {
			St.Heal_x = 10;
			St.Recover_type = 0;
			St.Ecg_rtn = 0;
			St.Ecg_x = 0;
			break;
		}
		St.Heal_x += 3;
		do {
			i--;
			diff = pSt->Heal_x - i;
			if ((ULONG)(diff - 11) < 82) {
				cv = (UCHAR)(i * -8 - 1);
				lf->r0 = cv;
				lf->g0 = cv;
				lf->b0 = 0x10;
				lf->x0 = pSt->Status_pos_x + diff;
				lf->x1 = pSt->Status_pos_x + diff;
				lf->y0 = pSt->Status_pos_y + 4;
				lf->y1 = pSt->Status_pos_y + 35;
				AddPrim(ot, lf);
			}
			lf += 2;
		} while (i != 0);
		if (St.Heal_x > 128) {
			St.Ecg_rtn = 1;
		}
		break;
	}
	AddPrim(ot, (ULONG *)&St_end_tag_c[G.Cbuf_id]);
	sp->u0 = (UCHAR)((CHAR)type * 0x2e);
	sp->v0 = 0x23;
	sp->x0 = St.Status_pos_x + 47;
	sp->y0 = St.Status_pos_y + 25;
	sp->clut = GetClut(0x100, 0x1eb);
	AddPrim(ot, sp);
	AddPrim(ot, &St.Dmode[0xd][G.Cbuf_id].code[1]);
	sp += 2;
	sp->x0 = St.Status_pos_x + 2;
	sp->y0 = St.Status_pos_y;
	AddPrim(ot, sp);
	AddPrim(ot, &St.Dmode[3][G.Cbuf_id].code[1]);
}

/* DS 0x80075a00 (St_init_disp_itp) */
VOID St_init_disp_itp(register ULONG type)
{
	register SPRT *sp;
	register POLY_FT4 *pft4;
	register LONG i;
	register LONG j;

	sp = St_itp_sprt;
	for (i = 0; i < 2; i++) {
		setSprt(sp);
		setSemiTrans(sp, 1);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->u0 = 0;
		sp->v0 = 0;
		sp->w = 112;
		sp->h = 72;
		sp->clut = GetClut(0, 0x1e8);
		sp++;
	}
	if (type != 0) {
		pft4 = St_pool_c000.ft4;
		for (j = 0; j < 2; j++) {
			SetPolyFT4(pft4);
			pft4->u0 = 0;
			pft4->v0 = 0;
			pft4->u1 = 0x70;
			pft4->v1 = 0;
			pft4->u2 = 0;
			pft4->v2 = 0x48;
			pft4->u3 = 0x70;
			pft4->v3 = 0x48;
			pft4->r0 = 0x80;
			pft4->g0 = 0x80;
			pft4->b0 = 0x80;
			setSemiTrans(pft4, 1);
			pft4->clut = GetClut(0, 0x1e8);
			pft4->tpage = GetTPage(1, 0, 0x1c0, 0x100);
			pft4++;
		}
		sp = St_itemget_sprt;
		for (i = 7; i >= 0; i--) {
			for (j = 0; j < 2; j++) {
				setSprt(sp);
				sp->r0 = 0x80;
				sp->g0 = 0x80;
				sp->b0 = 0x80;
				sp->u0 = Itemget_char[i][0];
				sp->v0 = Itemget_char[i][1];
				sp->w = Itemget_char[i][2];
				sp->h = Itemget_char[i][3];
				sp->clut = GetClut(0, 0x1e4);	/* retail calls GetClut for every sprite */
				sp++;
			}
		}
		f_x = 0x10;
		f_y = 10;
		xx = 0x70;
		yy = 0x48;
		St_anim_cnt = 0;
	}
}

/* DS 0x80075c28 (St_disp_itp) */
VOID St_disp_itp(register ULONG type)
{
	register SPRT *sp;
	register POLY_FT4 *pft4;
	register ULONG *ot;
	register LONG sx;
	register LONG sy;
	register LONG x2;
	register tagGLOBAL *pG;
	register STATUS *pSt;
	ULONG   Frame_pad[2];		/* retail reserves sp+0x10-0x17 and never references it */

	(void)Frame_pad;
	pG = &G;
	pSt = &St;
	sp = St_itp_sprt;
	pft4 = St_pool_c000.ft4;
	ot = &pG->Ot0[pG->Cbuf_id][3];
	sx = pSt->Frame_pos_x + f_x;
	sy = pSt->Frame_pos_y + f_y;
	if (pG->Cbuf_id != 0) {
		sp = &St_itp_sprt[1];
	}
	x2 = pSt->Frame_pos_x + 16;
	if (type == 0) {
		x2 = pSt->Frame_pos_x + 50;
	}
	sp->x0 = x2;
	sp->y0 = pSt->Frame_pos_y + 10;
	AddPrim(ot, sp);
	AddPrim(ot, &pSt->Dmode[0x15][pG->Cbuf_id].code[1]);
	if (type != 0) {
		if (pG->Cbuf_id != 0) {
			pft4 = &St_pool_c000.ft4[1];
		}
		pft4->x0 = sx;
		pft4->y0 = sy;
		pft4->x1 = xx + sx;
		pft4->y1 = sy;
		pft4->x2 = sx;
		pft4->y2 = yy + sy;
		pft4->x3 = xx + sx;
		pft4->y3 = yy + sy;
		if (pSt->Sub_rtn1 == 2) {
			AddPrim(ot, pft4);
		}
		AddPrim(ot, &pSt->Dmode[0xa][pG->Cbuf_id].code[1]);
	}
}

/* DS 0x80075dc4 (St_init_disp_filesel) */
VOID St_init_disp_filesel(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;
	register CHAR cu;

	sp = St_pool_c000.sp;
	cu = 0x38;
	for (i = 2; i > 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = cu;
			sp->v0 = 0x80;
			sp->w = 8;
			sp->h = 8;
			clut = GetClut(0, 0x1e9);
			sp->clut = clut;
			sp++;
		}
		cu -= 8;
	}
}

/* DS 0x80075e98 (St_disp_filesel) */
VOID St_disp_filesel(VOID)
{
	register SPRT *sp;
	register ULONG *ot;
	register LONG i;
	register LONG sx;
	register CHAR cu5;
	register CHAR cu4;

	sp = St_pool_c000.sp;
	if (G.Cbuf_id != 0) {
		sp = &St_pool_c000.sp[1];
	}
	ot = &G.Ot0[G.Cbuf_id][5];
	sx = 0x187;
	cu5 = 0x38;
	cu4 = 0x30;
	for (i = 2; i > 0; i--) {
		sx -= 0xba;
		cu5 -= 0x10;
		cu4 -= 0x10;
		if ((ULONG)St.Sub_rtn1 == (ULONG)(i + 3)) {
			sp->u0 = cu4;
		} else {
			sp->u0 = cu5;
		}
		sp->v0 = 0x88;
		sp->x0 = St.Frame_pos_x + sx - 10;
		sp->y0 = St.Frame_pos_y + 0x36;
		AddPrim(ot, sp);
		sp += 2;
	}
	AddPrim(ot, &St.Dmode[0x15][G.Cbuf_id].code[1]);
}

/* DS 0x80075fd0 (St_init_disp_file) */
VOID St_init_disp_file(VOID)
{
	register SPRT *sp;
	register USHORT clut;
	register LONG i;
	register LONG j;
	register CHAR cu;
	FILE_DATA *pFdat;

	pFdat = &File_data[G.SaveData.File_tag_bk[St.F_sel_book * 8 + St.F_sel_file] * 2 + 1];
	sp = St_pool_c100;
	clut = GetClut(0, 0x1ea);
	for (i = 0; i < 2; i++) {
		setSprt(sp);
		setSemiTrans(sp, 1);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->u0 = 0;
		sp->v0 = 0;
		sp->w = 0x100;
		sp->h = 0x100 - (pFdat->Page_max | (pFdat->Back_h << 8));
		sp->clut = clut;
		sp++;
	}
	sp = &St_pool_c100[2];
	clut = GetClut(0, 0x1e9);
	for (i = 0; i < 2; i++) {
		setSprt(sp);
		setSemiTrans(sp, 1);
		sp->r0 = 0x80;
		sp->g0 = 0x80;
		sp->b0 = 0x80;
		sp->u0 = 0;
		sp->v0 = (UCHAR)(-pFdat->Page_max);
		sp->w = 0x80;
		*(FILE_DATA *)&sp->h = *pFdat;
		sp->clut = clut;
		sp++;
	}
	sp = &St_pool_c100[4];
	cu = 0x2a;
	for (i = 2; i > 0; i--) {
		for (j = 0; j < 2; j++) {
			setSprt(sp);
			setSemiTrans(sp, 1);
			sp->r0 = 0x80;
			sp->g0 = 0x80;
			sp->b0 = 0x80;
			sp->u0 = cu;
			sp->v0 = 0xc;
			sp->w = 0xc;
			sp->h = 0xd;
			sp->clut = GetClut(0x100, 0x1ec);
			sp++;
		}
		cu -= 0xe;
	}
	St.File_pos_x = 0x19;
	St.File_pos_y = 0x1e;
}

/* DS 0x800761b8 (St_disp_file) */
VOID St_disp_file(VOID)
{
	register SPRT *sp;
	register SPRT *sp2;
	register SPRT *sp3;
	register SPRT *sp4;
	register ULONG *ot;
	register UCHAR br;
	register FILE_DATA *pFd0;

	ot = &G.Ot0[G.Cbuf_id][5];
	pFd0 = &File_data[G.SaveData.File_tag_bk[St.F_sel_book * 8 + St.F_sel_file] * 2];
	sp = St_pool_c100;
	if (G.Cbuf_id != 0) {
		sp = &St_pool_c100[1];
	}
	sp->x0 = St.File_pos_x;
	sp->y0 = St.File_pos_y;
	AddPrim(ot, sp);
	AddPrim(ot, (ULONG *)&Map_end_tag[G.Cbuf_id]);
	sp2 = sp + 2;
	sp2->x0 = 100;
	sp2->y0 = 0x3c;
	AddPrim(ot, sp2);
	AddPrim(ot, &St.Dmode[0x15][G.Cbuf_id].code[1]);
	sp3 = sp + 4;
	if (St.F_page_no == pFd0->Page_max && pFd0->Back_h == 0) {
		sp3->w = 0x2a;
		sp3->h = 0xe;
		sp3->u0 = 0x38;
		sp3->v0 = 0xc;
		sp3->x0 = 0x118;
		sp3->y0 = 0x6e;
		sp3->clut = GetClut(0x100, 0x1ea);
		br = 0x30;
		if (St.Sub_rtn1 == 0x0e) {
			br = 0x80;
		}
	} else {
		sp3->w = 0xc;
		sp3->h = 0xd;
		sp3->u0 = 0x2a;
		sp3->v0 = 0xc;
		sp3->y0 = 0x6e;
		sp3->x0 = St.blink_sw0 * 3 + 0x11a;
		sp3->clut = GetClut(0x100, 0x1ec);
		br = 0x80;
		if (St.Sub_rtn1 != 0x0d && St.blink_sw0 == 0) {
			br = 0x40;
		}
	}
	sp3->r0 = br;
	sp3->g0 = br;
	sp3->b0 = br;
	if (St.Sub_rtn1 == 0x0d || St.Sub_rtn1 == 0x0e) {
		AddPrim(ot, sp3);
	}
	sp4 = sp + 6;
	if (St.Sub_rtn1 == 0x0d || St.blink_sw0 != 0) {
		br = 0x80;
	} else {
		br = 0x40;
	}
	sp4->r0 = br;
	sp4->g0 = br;
	sp4->b0 = br;
	sp4->y0 = 0x6e;
	sp4->x0 = St.blink_sw0 * -3 + 0xc;
	if (St.Sub_rtn1 == 0x0d && St.F_page_no != 0) {
		AddPrim(ot, sp4);
	}
	AddPrim(ot, (ULONG *)&Map_end_tag[2 + G.Cbuf_id]);
}

/* DS 0x80076498 (ST_Om_trans) */
VOID ST_Om_trans(VOID)
{
	register LONG i_tag;
	register LONG i_mdl;
	register LONG i_pos;
	register LONG i_row;
	register LONG i_off;
	register LONG clut_plus;
	register ULONG no_00;
	register ULONG no;

	no_00 = 3;
	SetGeomOffset(0x72, St.Frame_pos_y + 0x46);
	i_tag = 0x10;
	Set_view(v_p, v_r);
	v_p[1] = 0x226 - St.Goffset_add;
	i_mdl = 0xd98;
	i_pos = 0x1e;
	do {
		no_00--;
		model_disp((MODEL_WORK *)(CD.Work_buf + i_mdl + 0x488), 0, no_00);
		no = 8;
		i_row = i_pos - 3;
		do {
			no--;
			if (no_00 == (ULONG)St.F_sel_book && no == (ULONG)St.F_sel_file) {
				i_off = i_row * 0x74;
				clut_plus = 0x1e9;
			} else {
				if (no_00 == (ULONG)St.F_sel_book) {
					i_off = i_row * 8;
				} else {
					i_off = i_row << 3;
				}
				i_off = (i_off * 4 + i_row * -3) * 4;
				clut_plus = 0x1ea;
			}
			Texture_Move((MODEL_WORK *)((int)(St.Fmodel[1].M - 3) + i_off + 2), 0x17, 0, clut_plus);
			if (G.SaveData.File_tag_bk[i_tag + no] != 0xff) {
				model_disp(&St.Fmodel[i_row + 1], 1, no);
			}
			i_row--;
		} while (no != 0);
		i_tag -= 8;
		i_mdl -= 0x488;
		i_pos -= 10;
	} while (no_00 != 0);
}

/* DS 0x80076640 (model_disp) */
VOID model_disp(register MODEL_WORK *pOm, register ULONG type, register ULONG no)
{
	register LONG px;
	MATRIX *m2;
	MATRIX Mx;
	MATRIX M;
	SVECTOR sv;
	SVECTOR rot;

	if (type == 0) {
		rot.vx = 0;
		rot.vy = 0;
		rot.vz = St.F_move_x + St.F_sel_book * 0x555 + (SHORT)no * -0x555;
		RotMatrix(&rot, &M);
		sv.vx = 0x1ae;
		sv.vy = 0;
		sv.vz = 0;
		ApplyMatrix(&M, &sv, (VECTOR *)&sv);
		pOm->Pos_x = sv.vx + pp.vx;
		pOm->Pos_y = sv.vy + pp.vy;
		pOm->Pos_z = sv.vz + pp.vz;
		if (St.Sub_rtn1 == 8) {
			pOm->dummy01 += 0x26;
		}
		if (St.Sub_rtn1 == 9) {
			pOm->dummy01 -= 0x26;
		}
		if (St.Sub_rtn1 == 0x0b) {
			pOm->Pos_y += 0x26;
		}
		if (St.Sub_rtn1 == 0x0c) {
			pOm->Pos_y -= 0x26;
		}
		if (St.Sub_rtn1 < 9) {
			px = pOm->Pos_x;
			if (px >= 0) {
				px += (St.Goffset_add + 2) * -2;
				pOm->Pos_x = px;
				if (px > 0) {
					goto model_disp_end;
				}
				pOm->Pos_x = 0;
			}
			px = pOm->Pos_x + (St.Goffset_add + 2) * 2;
			pOm->Pos_x = px;
			if (px >= 0) {
				goto model_disp_end;
			}
		}
		pOm->Pos_x = 0;
	} else {
		sv.vx = 0;
		rot.vx = 0;
		sv.vy = 0;
		rot.vy = 0;
		sv.vz = 0;
		rot.vz = 0;
		RotMatrix(&rot, &M);
		ApplyMatrix(&M, &sv, (VECTOR *)&sv);
		pOm->Pos_x = (sv.vx + pp.vx - no) - 0x17;
		pOm->Pos_y = sv.vy + pp.vy + no * 0x3e - 0xde;
		pOm->Pos_z = sv.vz + pp.vz + 0xe9;
	}
model_disp_end:
	RotMatrix((SVECTOR *)&pOm->Cdir_z, (MATRIX *)pOm->M);
	m2 = (MATRIX *)((pOm->Workm).m[0] + 2);
	CompM((MATRIX *)pOm[1].Attribute, (MATRIX *)pOm->M, m2);
	Set_light((VECTOR *)&pOm->Workm.t[1]);
	SetColorMatrix((MATRIX *)&COLORMATRIX);
	MulMatrix0((MATRIX *)&LIGHTMATRIX, (MATRIX *)m2, &Mx);
	SetLightMatrix(&Mx);
	CompM(&GsWSMATRIX, m2, &Mx);
	SetRotMatrix(&Mx);
	SetTransMatrix(&Mx);
	SortGT3_neo((TMD_MEM *)pOm->pTmd, (POLY_GT3 *)(pOm->pPacket + G.Cbuf_id * 10), (ULONG)pOm->pSuper, 0);
	SortGT4_neo((TMD_MEM *)pOm->pTmd2, (POLY_GT4 *)(pOm->pPacket2 + G.Cbuf_id * 0xd), (ULONG)pOm->pSuper, 0);
}

/* DS 0x800ab160 (Ecg_col_tbl) */
UCHAR Ecg_col_tbl[5][6] = {
	{ 0x20, 0xff, 0x20, 0x01, 0x08, 0x01 },
	{ 0xff, 0xff, 0x20, 0x08, 0x08, 0x01 },
	{ 0xff, 0x80, 0x20, 0x08, 0x04, 0x01 },
	{ 0xff, 0x20, 0x20, 0x08, 0x01, 0x01 },
	{ 0xff, 0x20, 0xff, 0x08, 0x01, 0x08 }
};

/* DS 0x800ab180 (Ecg_line_tbl) */
ECG_POINT* Ecg_line_tbl[5] = {
	Ecg_normal_tbl, Ecg_caution0_tbl, Ecg_caution1_tbl, Ecg_danger_tbl,
	Ecg_poison_tbl
};

/* DS 0x800aaad8 (Ecg_normal_tbl) */
ECG_POINT Ecg_normal_tbl[80] = {
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0d, 0x00 }, { 0x0c, 0x00 },
	{ 0x0c, 0x00 }, { 0x0d, 0x02 }, { 0x0f, 0x03 }, { 0x12, 0x02 }, { 0x14, 0x00 }, { 0x10, 0x04 }, { 0x08, 0x08 }, { 0x05, 0x03 },
	{ 0x04, 0x00 }, { 0x05, 0x03 }, { 0x08, 0x07 }, { 0x0f, 0x04 }, { 0x13, 0x05 }, { 0x18, 0x03 }, { 0x1b, 0x00 }, { 0x19, 0x02 },
	{ 0x15, 0x04 }, { 0x10, 0x05 }, { 0x0e, 0x02 }, { 0x0d, 0x00 }, { 0x0e, 0x02 }, { 0x10, 0x03 }, { 0x13, 0x00 }, { 0x13, 0x00 },
	{ 0x12, 0x00 }, { 0x10, 0x02 }, { 0x0e, 0x02 }, { 0x0d, 0x00 }, { 0x0c, 0x00 }, { 0x0d, 0x00 }, { 0x0e, 0x01 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }
};

/* DS 0x800aab78 (Ecg_caution0_tbl) */
ECG_POINT Ecg_caution0_tbl[80] = {
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0d, 0x00 }, { 0x0c, 0x00 }, { 0x0c, 0x00 },
	{ 0x0e, 0x00 }, { 0x0d, 0x02 }, { 0x0f, 0x03 }, { 0x12, 0x02 }, { 0x14, 0x00 }, { 0x10, 0x04 }, { 0x08, 0x08 }, { 0x06, 0x02 },
	{ 0x05, 0x00 }, { 0x06, 0x02 }, { 0x08, 0x05 }, { 0x0d, 0x02 }, { 0x0f, 0x00 }, { 0x10, 0x04 }, { 0x14, 0x02 }, { 0x16, 0x00 },
	{ 0x15, 0x00 }, { 0x10, 0x05 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0d, 0x00 }, { 0x0d, 0x00 }, { 0x0e, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }
};

/* DS 0x800aac18 (Ecg_caution1_tbl) */
ECG_POINT Ecg_caution1_tbl[80] = {
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x10, 0x00 }, { 0x10, 0x00 }, { 0x11, 0x00 }, { 0x11, 0x00 },
	{ 0x11, 0x00 }, { 0x10, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0b, 0x03 }, { 0x0a, 0x00 }, { 0x09, 0x00 }, { 0x0a, 0x04 }, { 0x0d, 0x03 }, { 0x10, 0x03 },
	{ 0x13, 0x00 }, { 0x14, 0x00 }, { 0x13, 0x00 }, { 0x12, 0x00 }, { 0x10, 0x02 }, { 0x0e, 0x02 }, { 0x0d, 0x00 }, { 0x0c, 0x00 },
	{ 0x0c, 0x00 }, { 0x0c, 0x00 }, { 0x0d, 0x00 }, { 0x0e, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }
};

/* DS 0x800aacb8 (Ecg_danger_tbl) */
ECG_POINT Ecg_danger_tbl[80] = {
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0d, 0x00 }, { 0x0d, 0x00 }, { 0x0e, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x10, 0x00 }, { 0x11, 0x02 }, { 0x11, 0x00 }, { 0x11, 0x00 }, { 0x0e, 0x03 },
	{ 0x0a, 0x04 }, { 0x09, 0x00 }, { 0x0a, 0x02 }, { 0x0c, 0x03 }, { 0x0f, 0x00 }, { 0x10, 0x00 }, { 0x10, 0x00 }, { 0x10, 0x00 },
	{ 0x10, 0x00 }, { 0x0f, 0x00 }, { 0x0e, 0x00 }, { 0x0d, 0x00 }, { 0x0d, 0x00 }, { 0x0e, 0x00 }, { 0x0e, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }
};

/* DS 0x800aad58 (Ecg_poison_tbl) */
ECG_POINT Ecg_poison_tbl[80] = {
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0e, 0x01 }, { 0x0d, 0x01 }, { 0x0c, 0x01 }, { 0x0c, 0x01 }, { 0x0d, 0x02 }, { 0x0f, 0x03 }, { 0x12, 0x02 }, { 0x14, 0x01 },
	{ 0x10, 0x04 }, { 0x08, 0x08 }, { 0x05, 0x03 }, { 0x04, 0x01 }, { 0x05, 0x03 }, { 0x08, 0x07 }, { 0x0f, 0x04 }, { 0x13, 0x05 },
	{ 0x18, 0x02 }, { 0x1a, 0x01 }, { 0x19, 0x01 }, { 0x0f, 0x0a }, { 0x0e, 0x01 }, { 0x0f, 0x01 }, { 0x10, 0x02 }, { 0x12, 0x01 },
	{ 0x11, 0x01 }, { 0x0a, 0x07 }, { 0x09, 0x01 }, { 0x0a, 0x02 }, { 0x0c, 0x04 }, { 0x10, 0x01 }, { 0x11, 0x01 }, { 0x12, 0x01 },
	{ 0x12, 0x01 }, { 0x12, 0x01 }, { 0x11, 0x01 }, { 0x10, 0x01 }, { 0x0f, 0x01 }, { 0x0f, 0x01 }, { 0x0f, 0x01 }, { 0x0f, 0x01 },
	{ 0x0f, 0x01 }, { 0x0c, 0x03 }, { 0x0a, 0x02 }, { 0x09, 0x01 }, { 0x0a, 0x06 }, { 0x10, 0x03 }, { 0x13, 0x01 }, { 0x13, 0x01 },
	{ 0x11, 0x02 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 },
	{ 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }, { 0x0f, 0x00 }
};
