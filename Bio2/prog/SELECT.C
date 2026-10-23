/*
 * SELECT.C - Scenario/character select screen module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SELECT.C
 */
#include "TYPES.H"
#include "ROOM.H"
#include <ASM.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBPAD.H>
#include <LIBSND.H>
#include <MEMORY.H>
#include <STRINGS.H>
#include "SELECT.H"
#include "CD.H"
#include "CONFIG.H"
#include "DEFINES.H"
#include "DIESPR.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MONITOR.H"
#include "SND.H"
#include "SUB.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "FILECODE.H"


/* DS 0x801bfa1c (Select_mess_tim) */
UCHAR Select_mess_tim[4] = { 'P', 'L', 'W', 0 };

/* DS 0x801bfa20 (Select_mess_pl) */
UCHAR Select_mess_pl[4] = { 'P', 'L', 'D', 0 };

/* DS 0x801bfa24 (Select_str_bg) */
CHAR Select_str_bg[10] = {
	'S', 'e', 'l', 'e', 'c', 't', ' ', 'B',
	'G', 0 };

/* DS 0x801bfa30 (Select_str_sel) */
CHAR Select_str_sel[12] = { 'S', 'e', 'l', 'e', 'c', 't', 0 };	/* retail reserves 12 bytes here (0x801bfa30..0x801bfa3b, zero after the text) */

/* DS 0x801c2af0 (Select_sprt_def) */
SELECT_SPRT_DEF Select_sprt_def[12] = {
	{ 0, 151, 256, 9, 0, 176, 0, 1 },
	{ 256, 151, 64, 9, 64, 144, 0, 1 },
	{ 0, 8, 256, 24, 0, 120, 0, 1 },
	{ 256, 8, 64, 24, 0, 144, 0, 1 },
	{ 0, 154, 256, 80, 0, 176, 0, 1 },
	{ 256, 154, 64, 24, 64, 144, 0, 1 },
	{ 256, 178, 64, 24, 128, 144, 0, 1 },
	{ 256, 202, 64, 32, 192, 144, 0, 1 },
	{ 40, 115, 10, 20, 240, 30, 1, 2 },
	{ 170, 115, 10, 20, 240, 50, 1, 2 },
	{ 0, 232, 160, 10, 0, 120, 0, 1 },
	{ 160, 232, 160, 10, 0, 120, 0, 1 },
};

/* DS 0x801c2b80 (Select_pl_init) */
SELECT_PL_INIT Select_pl_init[4] = {
	{ 0, 0, -2250, 0, 0, 0, 0x0e, 0x00 },
	{ 2250, 0, 0, 0, 3584, 0, 0x00, 0x00 },
	{ 0, 0, 2250, 0, 3904, 0, 0x01, 0x00 },
	{ -2250, 0, 0, 0, 2048, 0, 0x0b, 0x00 },
};

/* DS 0x801c2bd0 (Select_moji_tbl) */
ULONG Select_moji_tbl[141][4] = {
	{ 0x00000000, 0x000a0008, 0x00000ae0, 0x00800200 }, { 0x009e000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e0096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x009e009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x009e00a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa0096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00aa009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00aa00a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b40096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00b4009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00b400a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be0096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00be009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00be00a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c80096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00c8009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00c800a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d20096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00d2009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00d200a6, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc000e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0016, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc001e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0026, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc002e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0036, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc003e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0046, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc004e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0056, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc005e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0066, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc006e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0076, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc007e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0086, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc008e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc0096, 0x000a0008, 0x00000000, 0x00800201 }, { 0x00dc009e, 0x000a0008, 0x00000000, 0x00800201 },
	{ 0x00dc00a6, 0x000a0008, 0x00000000, 0x00800201 },
};

/* DS 0x801c35e0 (Select_str) */
UCHAR Select_str[4][7][30] = {
	{
		{
			'A', 'd', 'a', ' ', 'W', 'o', 'n', 'g', ']', 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '7', 'C', 'o', 'l', 't', '.', 'S', '.', 'A', '.', 'A', '.', ']',
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'M', 'A', 'C', '1', '1', ']', 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'B', 'o', 'w', ' ', 'G', 'u', 'n', ']', 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'I', 'n', 'k', ' ', 'R', 'i', 'b', 'b', 'o', 'n', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'M', 'i', 'x', 'e', 'd', ' ', 'H', 'a', 'r', 'b', 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
	},
	{
		{
			'L', 'e', 'o', 'n', ' ', 'S', '.', 'K', 'e', 'n', 'n', 'e', 'd', 'y', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '7', 'H', '&', 'K', ' ', 'V', 'P', '7', '0', '.', ']', 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'R', 'e', 'm', 'i', 'n', 't', 'o', 'n', ' ', 'M', '1', '1',
			'0', '0', '-', 'P', '.', ']', 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'D', 'e', 's', 'e', 'r', 't', ' ', 'E', 'a', 'g', 'l', 'e',
			' ', '5', '0', 'A', '.', 'E', '.', ']', 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'I', 'n', 'k', ' ', 'R', 'i', 'b', 'b', 'o', 'n', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'F', '.', 'A', 'i', 'd', ' ', 'S', 'p', 'r', 'a', 'y', ']',
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'B', 'l', 'u', 'e', ' ', 'H', 'a', 'r', 'b', 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
	},
	{
		{
			'C', 'l', 'a', 'i', 'r', 'e', ' ', 'R', 'e', 'd', 'f', 'i', 'e', 'l', 'd', ']',
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '7', 'M', '7', '9', ' ', 'G', 'r', 'e', 'n', 'a', 'd', 'e', ' ',
			'L', 'a', 'u', 'n', 'c', 'h', 'e', 'r', 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'G', 'r', 'e', 'n', 'a', 'd', 'e', ' ', 'F', 'l', 'a', 'm',
			'e', ' ', 'R', 'o', 'u', 'n', 'd', 's', 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'G', 'r', 'e', 'n', 'a', 'd', 'e', ' ', 'A', 'c', 'i', 'd',
			' ', 'R', 'o', 'u', 'n', 'd', 's', ']', 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'I', 'n', 'k', ' ', 'R', 'i', 'b', 'b', 'o', 'n', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'F', '.', 'A', 'i', 'd', ' ', 'S', 'p', 'r', 'a', 'y', ']',
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'B', 'l', 'u', 'e', ' ', 'H', 'a', 'r', 'b', 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
	},
	{
		{
			'C', 'h', 'r', 'i', 's', ' ', 'R', 'e', 'd', 'f', 'i', 'e', 'l', 'd', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '7', 'B', 'e', 'r', 'e', 't', 't', 'a', '-', 'M', '9', '2', 'F',
			'S', '.', ']', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'R', 'e', 'm', 'i', 'n', 'g', 't', 'o', 'n', ' ', 'M', '1',
			'1', '0', '0', '.', ']', 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'R', 'o', 'c', 'k', 'e', 't', ' ', 'L', 'a', 'u', 'n', 'c',
			'h', 'e', 'r', ']', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'I', 'n', 'k', ' ', 'R', 'i', 'b', 'b', 'o', 'n', ']', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			'[', '0', '0', '5', 'F', '.', 'A', 'i', 'd', ' ', 'S', 'p', 'r', 'a', 'y', 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
	},
};

/* DS 0x801c3928 (Select_light_mtx) */
MATRIX Select_light_mtx[4];

/* DS 0x801c39a8 (Select_light_col) */
MATRIX Select_light_col[4];

/* DS 0x801c3a28 (Select_dir_L) */
ULONG Select_dir_L[4][4] = {
	{ 0x400, 0x0, 0xc00, 0x800 }, { 0x800, 0x400, 0x0, 0xc00 },
	{ 0xc00, 0x800, 0x400, 0x0 }, { 0x0, 0xc00, 0x800, 0x400 },
};

/* DS 0x801c3a68 (Select_dir_fpos) */
SHORT Select_dir_fpos[4][4] = {
	{ 0xe00, 0xf40, 0x800, 0x0 }, { 0x0, 0xe00, 0xf40, 0x800 },
	{ 0x800, 0x0, 0xe00, 0xf40 }, { 0xf40, 0x800, 0x0, 0xe00 },
};

/* DS 0x801c3a88 (Select_name_uv) */
SELECT_NAME_UV Select_name_uv[16] = {
	{ 40, 30, 0, 30 },
	{ 80, 30, 40, 30 },
	{ 40, 30, 120, 30 },
	{ 40, 30, 160, 30 },
	{ 40, 30, 200, 30 },
	{ 40, 30, 0, 60 },
	{ 40, 30, 40, 60 },
	{ 40, 30, 80, 60 },
	{ 40, 30, 120, 60 },
	{ 40, 30, 160, 60 },
	{ 40, 30, 200, 60 },
	{ 40, 30, 0, 90 },
	{ 40, 30, 40, 90 },
	{ 80, 30, 80, 90 },
	{ 40, 30, 160, 90 },
	{ 40, 30, 200, 90 },
};

/* DS 0x801c3ae8 (Select_sprt_pos) */
SELECT_XY Select_sprt_pos[6] = {
	{ 184, 168 },
	{ 224, 168 },
	{ 264, 168 },
	{ 184, 198 },
	{ 224, 198 },
	{ 264, 198 },
};

/* DS 0x801c3b00 (Select_cur_x) */
USHORT Select_cur_x[4][2] = {
	{ 65, 155 }, { 40, 170 }, { 60, 160 }, { 40, 170 },
};

/* DS 0x801c3b10 (Select_name_idx) */
UCHAR Select_name_idx[4][6] = {
	{ 0x04, 0x0d, 0xff, 0x06, 0x0c, 0x0f },
	{ 0x00, 0x05, 0x07, 0x0c, 0x0b, 0x0e },
	{ 0x03, 0x09, 0x0a, 0x0c, 0x0b, 0x0e },
	{ 0x08, 0x02, 0x0c, 0x01, 0xff, 0x0b },
};

/* DS 0x801c3b28 (Select_pl_cd) */
USHORT Select_pl_cd[4] = {
	PL0ECH_PLD, PL00CH_PLD, PL01CH_PLD, PL0BCH_PLD
};

/* DS 0x801c3b30 (Select_tim_cd) */
USHORT Select_tim_cd[4] = {
	PL0EW0D_PLW, PL00W02_PLW, PL01W09_PLW, PL0BW08_PLW
};

/* DS 0x801c3b38 (Select_main_proc_tbl) */
VOID *Select_main_proc_tbl[6] = {
	(VOID *)Select_init,		/* 0 */
	(VOID *)Select_fade_in,		/* 1 */
	(VOID *)Select_cursor_move,	/* 2 */
	(VOID *)Select_decide,		/* 3 */
	(VOID *)Select_fade_out,	/* 4 */
	(VOID *)Select_exit,		/* 5 */
};

/* DS 0x801c3b50 (Select_light_proc_tbl) */
VOID *Select_light_proc_tbl[4] = {
	(VOID *)Select_light_wait,	/* 0 */
	(VOID *)Select_light_up,	/* 1 */
	(VOID *)Select_light_on,	/* 2 */
	(VOID *)Select_light_down,	/* 3 */
};

/* DS 0x801c3b60 (Select_moji_end) */
ULONG Select_moji_end;

/* DS 0x801c3b68 (Select_view_unk) */
ULONG Select_view_unk;

/* DS 0x801c3b6c (Select_view_eye) */
LONG Select_view_eye[3];

/* DS 0x801c3b7c (Select_view_ref) */
LONG Select_view_ref[3];

/* DS 0x801c3b90 (Select_wk) */
SELECT_WORK Select_wk;

// 801BFAA8
/* DS 0x801bfaa8 (Select_main) */
VOID Select_main(VOID)
{
	Select_wk.Main_no = 0;
	Select_wk.Pad001 = 0;
	Select_wk.Moji_no = 0;
	Select_wk.Pad003 = 0;

	do {
		((VOID (*)(VOID))Select_main_proc_tbl[Select_wk.Main_no])();
		Select_pl_move();
		Select_pl_main();
		Select_moji_ctrl();
		Task_sleep(1);
	} while (TRUE);
}

// 801BFB3C
/* DS 0x801bfb3c (Select_init) */
VOID Select_init(VOID)
{
	BGMCTR	bgm;
	UCHAR	*pFree;
	ENEMY_WORK *pEmTop;
	ULONG	k;

	Task_suspend(0);
	Main.Vloop = 2;

	Select_wk.pHeap = (UCHAR *)SELECT_HEAP_BASE;
	Bg_set_mode(2, 0);
	Snd_sys_init2();

	pFree = G.pFree_work;
	bgm.Main = 0x39;
	bgm.Sub = 0xff;
	pBgm = &bgm;
	Seq_ctr[0].Ctrl = 0;
	Seq_ctr[0].Vol.left = 100;
	Seq_ctr[0].Vol.right = 100;
	G.pFree_work = Select_wk.pHeap;
	Snd_bgm_main();
	G.pFree_work = pFree;

	Snd_bgm_ctr(0x1000000);
	Snd_sys_core_set(0x15, 4);
	Select_bg_load();
	Main.Scrl_type = 1;
	Select_sprt_init();

	/* Four ENEMY_WORK blocks at the top of the heap */
	pEmTop = (ENEMY_WORK *)Select_wk.pHeap;
	Select_wk.pHeap = (UCHAR *)&pEmTop[4];
	for (k = 0; k < 4; k++) {
		G.pEnemy[k] = &pEmTop[k];
		pEmTop[k].Be_flg = 1;
		G.pEnemy[k]->Id = Select_pl_init[k].Id;
	}

	/* Load the four models; Cursor_no is the model index during the load */
	for (k = 0; k < 4; k++) {
		Select_wk.Cursor_no = (UCHAR)k;
		Select_wk.pHeap = (UCHAR *)Select_pl_load(G.pEnemy[k], (ULONG *)Select_wk.pHeap, (UCHAR)(5 + 2 * k), (UCHAR)((k << 1) & 0xfe));
	}

	/* Message window work, 0x20C0 bytes, sits after the models */
	Select_wk.pMoji = (SELECT_MOJI *)Select_wk.pHeap;
	Select_wk.pHeap += 0x20c0;

	for (k = 0; k < 4; k++) {
		Select_pl_set(G.pEnemy[k], k);
		Select_light_init(G.pEnemy[k]);
	}

	Select_view_init();

	Select_wk.Cursor_no = 1;
	Select_wk.Blink_cnt = 0;
	Select_wk.Blink_dir = 0;
	Select_wk.Flg = 0;
	/* Dummy[1] 0 hides models 0 and 3, 1 hides model 3; retail has a single shared
	 * Select_pl_hide(G.pEnemy[3]) call for both cases */
	if ((G.SaveData.Dummy[1] == 0) || (G.SaveData.Dummy[1] == 1)) {
		if (G.SaveData.Dummy[1] == 0) {
			Select_wk.Flg = 6;
			Select_pl_hide(G.pEnemy[0]);
		}
		else {
			Select_wk.Flg = 4;
		}
		Select_pl_hide(G.pEnemy[3]);
	}

	Fade_set(0x200, -0xc00, 7, 1);
	Bg_set_mode(0, 0);
	Select_wk.Main_no = 1;
}

/* DS 0x801bfdcc (Select_pl_hide) */
VOID Select_pl_hide(ENEMY_WORK *pEm)
{
	ULONG	n;
	PARTS_W	*pP;

	n = pEm->Parts_num;
	pP = pEm->pSin_parts_ptr;
	do {
		pP->Poly_rgb = 0;
		n--;
		pP++;
	} while (n != 0);
}

/* DS 0x801bfdf0 (Select_view_init) */
VOID Select_view_init(VOID)
{
	Select_view_eye[0] = 0x1446;
	Select_view_eye[1] = -0x960;
	Select_view_eye[2] = 0x30c;
	Select_view_unk = 0;
	Select_view_ref[0] = 0;
	Select_view_ref[1] = -0x7d0;
	Select_view_ref[2] = 0x82;
}

// 801BFE44
/* DS 0x801bfe44 (Select_sprt_init) */
VOID Select_sprt_init(VOID)
{
	ULONG	cb;
	ULONG	k;
	SPRT	*pSp;

	for (cb = 0; cb < 2; cb++) {
		/* Slots 0..7: grey sprites from Select_sprt_def[0..7] */
		for (k = 0; k < 8; k++) {
			pSp = &Select_wk.Prim[k].Sp[cb];
			pSp->r0 = 0x80;
			pSp->g0 = 0x80;
			pSp->b0 = 0x80;
			pSp->code = 0;
			pSp->x0 = Select_sprt_def[k].X;
			pSp->y0 = Select_sprt_def[k].Y;
			pSp->w = Select_sprt_def[k].W;
			pSp->h = Select_sprt_def[k].H;
			pSp->u0 = Select_sprt_def[k].U;
			pSp->v0 = Select_sprt_def[k].V;
			pSp->clut = 0x7bc0;
			SetSprt(pSp);
			SetSemiTrans(pSp, Select_sprt_def[k].Abr);
			SetDrawMode(&Select_wk.Prim[k].Dr[cb], 0, 0, GetTPage(1, Select_sprt_def[k].Tp, 640, 256), NULL);
		}

		/* Slots 8,9: black sprites from Select_sprt_def[8,9] */
		for (k = 8; k < 10; k++) {
			pSp = &Select_wk.Prim[k].Sp[cb];
			pSp->r0 = 0;
			pSp->g0 = 0;
			pSp->b0 = 0;
			pSp->code = 0;
			pSp->x0 = Select_sprt_def[k].X;
			pSp->y0 = Select_sprt_def[k].Y;
			pSp->w = Select_sprt_def[k].W;
			pSp->h = Select_sprt_def[k].H;
			pSp->u0 = Select_sprt_def[k].U;
			pSp->v0 = Select_sprt_def[k].V;
			pSp->clut = 0x7bc0;
			SetSprt(pSp);
			SetSemiTrans(pSp, Select_sprt_def[k].Abr);
			SetDrawMode(&Select_wk.Prim[k].Dr[cb], 0, 0, GetTPage(1, Select_sprt_def[k].Tp, 640, 256), NULL);
		}

		/* Slots 10,11: black sprites from Select_sprt_def[10,11] */
		for (k = 10; k < 12; k++) {
			pSp = &Select_wk.Prim[k].Sp[cb];
			pSp->r0 = 0;
			pSp->g0 = 0;
			pSp->b0 = 0;
			pSp->code = 0;
			pSp->x0 = Select_sprt_def[k].X;
			pSp->y0 = Select_sprt_def[k].Y;
			pSp->w = Select_sprt_def[k].W;
			pSp->h = Select_sprt_def[k].H;
			pSp->u0 = Select_sprt_def[k].U;
			pSp->v0 = Select_sprt_def[k].V;
			pSp->clut = 0x7bc0;
			SetSprt(pSp);
			SetSemiTrans(pSp, Select_sprt_def[k].Abr);
			SetDrawMode(&Select_wk.Prim[k].Dr[cb], 0, 0, GetTPage(1, Select_sprt_def[k].Tp, 640, 256), NULL);
		}

		/* Slots 12..17: six name label sprites, positions from Select_sprt_pos */
		for (k = 12; k < 18; k++) {
			pSp = &Select_wk.Prim[k].Sp[cb];
			pSp->r0 = 0x80;
			pSp->g0 = 0x80;
			pSp->b0 = 0x80;
			pSp->code = 0;
			pSp->x0 = Select_sprt_pos[k - 12].X;
			pSp->y0 = Select_sprt_pos[k - 12].Y;
			pSp->clut = 0x7c00;
			SetSprt(pSp);
			SetSemiTrans(pSp, 0);
			SetDrawMode(&Select_wk.Prim[k].Dr[cb], 0, 0, GetTPage(1, 0, 640, 256), NULL);
		}
	}
}

/* DS 0x801c026c (Select_pl_tim_load) */
VOID Select_pl_tim_load(ENEMY_WORK *pEm, ULONG *pWork)
{
	TIM_IMAGE	tim;
	TIM_IMAGE	*pT;
	LONG		*piVar;

	Cd_read(Select_tim_cd[Select_wk.Cursor_no], pWork, 0, Select_mess_tim);

	/* pWork[0] is the offset of the header; header word 3 is the TIM offset */
	piVar = (LONG *)((UCHAR *)pWork + pWork[0]);
	OpenTIM((ULONG *)((UCHAR *)pWork + piVar[3]));
	pT = ReadTIM(&tim);

	pT->prect->x = (USHORT)pEm->Tpage * 0x40 - 0x39c;
	pT->prect->y = 0xe0;
	pT->prect->w = 0x1c;
	pT->prect->h = 0x20;
	LoadImage(pT->prect, pT->paddr);
	DrawSync(0);

	pT->crect->x = 0xf0;
	pT->crect->y = pEm->Clut + 0x1e1;
	pT->crect->w = 0x10;
	LoadImage(pT->crect, pT->caddr);
	DrawSync(0);
}

/* DS 0x801c0378 (Select_pl_load) */
ULONG *Select_pl_load(ENEMY_WORK *pEm, ULONG *pWork, UCHAR Tpage, UCHAR Clut)
{
	LONG	*pHdr;
	ULONG	*pTmd;
	ULONG	Tim_adr;

	Cd_read(Select_pl_cd[Select_wk.Cursor_no], pWork, 0, Select_mess_pl);

	/* Header words: seq table, kan table, TMD, TIM (offsets from pWork) */
	pHdr = (LONG *)((UCHAR *)pWork + pWork[0]);
	pEm->pSeq_t_ptr = (ULONG *)((UCHAR *)pWork + pHdr[0]);
	pEm->Clut = Clut;
	pEm->Tpage = Tpage;
	pEm->pKan_t_ptr = (ULONG *)((UCHAR *)pWork + pHdr[1]);
	pEm->pTmd = (ULONG *)((UCHAR *)pWork + pHdr[2]);
	G.Cclut = Clut;
	G.Ctpage = Tpage;

	Mapping_tim((ULONG *)((UCHAR *)pWork + pHdr[3]));
	DrawSync(0);
	Mapping_tmd(1, pEm->pTmd, Tpage, Clut);

	pTmd = pEm->pTmd;
	pEm->pTmd = pTmd + 3;
	pEm->pTmd2 = pTmd + 10;

	/* The parts work goes over the TIM; Ada (Id 14) keeps 0x2800 bytes of it for Aida_Jh_init */
	pWork = (ULONG *)((UCHAR *)pWork + pHdr[3]);
	if (pEm->Id == 14) {
		Tim_adr = (ULONG)pWork;
		pWork = (ULONG *)((UCHAR *)pWork + 0x2800);
	}

	G.pPl_emd_top = (ULONG *)PartsWork_set(pEm, (ULONG)pWork);
	pWork = (ULONG *)PartsWork_link(pEm, (ULONG)G.pPl_emd_top, pEm->pKan_t_ptr, 1);

	if (pEm->Id == 14) {
		Aida_Jh_init(pEm, Tim_adr);
		pEm->pSin_parts_ptr->Be_flg |= 0x4000;
	}

	pEm->Parts0_pos_y = 4096;
	Select_pl_tim_load(pEm, pWork);
	return pWork;
}

/* DS 0x801c0520 (Select_mapping_tim) */
VOID Select_mapping_tim(ULONG *pTim)
{
	TIM_IMAGE	tim;
	TIM_IMAGE	*pT;
	SHORT		sX;

	OpenTIM(pTim);
	pT = ReadTIM(&tim);

	/* texture pages 0..15 are on VRAM row 0, 16.. on row 256 */
	sX = G.Ctpage * 64;
	if (G.Ctpage > 15) {
		sX -= 1024;
	}
	pT->prect->x = sX;
	pT->prect->y = (G.Ctpage >= 16) << 8;
	LoadImage(pT->prect, pT->paddr);

	G.Ctpage += (pT->prect->w + 63) / 64;

	if (pT->caddr != NULL) {
		pT->crect->y = G.Cclut + 480;
		LoadImage(pT->crect, pT->caddr);
		G.Cclut += pT->crect->h;
	}

	pT->crect->y++;
	pT->caddr = pTim + 0x4088;
	LoadImage(pT->crect, pTim + 0x4088);
	DrawSync(0);
}

/* DS 0x801c0674 (Select_bg_load) */
VOID Select_bg_load(VOID)
{
	Cd_read(SELECT_B_TIM, Bg_img_buf, 0, (UCHAR *)Select_str_bg);
	Cd_read(SELECT_W_TIM, SELECT_HEAP_BASE, 0, (UCHAR *)Select_str_sel);

	G.Ctpage = 0x1a;
	G.Cclut = 0x0f;
	Select_mapping_tim(SELECT_HEAP_BASE);
}

/* DS 0x801c06d4 (Select_claire_set) */
VOID Select_claire_set(ENEMY_WORK *pEm)
{
	PARTS_W	*pP;
	PARTS_W	*pRoot;
	ULONG	flg;

	pEm->Atd[0].At_w = 0x15e;
	pEm->Atd[0].At_d = 0x15e;
	pEm->Atd[0].W = 0x15e;
	pEm->Atd[0].D = 0x15e;
	pEm->Atd[0].Ofs_y = -0x546;
	pEm->Atd[0].At_h = 0x546;

	pP = pEm->pSin_parts_ptr;
	pP[0xf].Pos_x = -0x28;
	pP[0xf].Pos_y = -0x170;
	pP[0x10].Pos_x = -0x30;
	pP[0x10].Pos_y = -0x20;
	pP[0x11].Pos_x = -0x5b;
	pP[0x11].Pos_y = 0x61;
	pP[0x12].Pos_x = -1;
	pP[0x12].Pos_y = 0x7f;

	flg = pP[0xf].Be_flg;
	pP[0xf].Pos_z = 0;
	pP[0xf].Free[0] = 0;
	pP[0xf].Free[1] = 0;
	pP[0xf].Cdir_y = 0;
	pP[0xf].Be_flg = flg | 0x41;

	flg = pP[0x11].Be_flg;
	pP[0x10].Pos_z = 0;
	pP[0x10].Free[0] = 0;
	pP[0x10].Free[1] = 0;
	pP[0x10].Cdir_y = 0;
	pP[0x10].Be_flg |= 0x41;

	pP[0x11].Pos_z = 0;
	pP[0x11].Free[0] = 0;
	pP[0x11].Free[1] = 0;
	pP[0x11].Cdir_y = 0;
	pP[0x11].Be_flg = flg | 0x41;

	pP[0x12].Pos_z = 0;
	flg = pP[0x12].Be_flg;
	pP[0x12].Free[0] = 0;
	pP[0x12].Free[1] = 0;
	pP[0x12].Cdir_y = 0;
	pP[0x13].Be_flg = 1;
	pP[0x12].Be_flg = flg | 0x41;

	pP[0x13].pSuper = (ULONG *)&pEm->pSin_parts_ptr[0xb].Workm;
	pRoot = pEm->pSin_parts_ptr;
	pP[0x13].Pos_x = 0x40;
	pP[0x13].Pos_y = 0x3c0;
	pP[0x13].Pos_z = 0;
	pP[0x13].Timer3 = 0xff;
	pP[0x13].pOya_parts = (ULONG *)&pRoot->Pos_x;
}

/* DS 0x801c07f0 (Select_ada_set) */
VOID Select_ada_set(ENEMY_WORK *pEm)
{
	PARTS_W	*pP;

	pEm->Atd[0].Ofs_y = -0x546;
	pP = pEm->pSin_parts_ptr;
	pEm->Atd[0].At_w = 0x15e;
	pEm->Atd[0].At_h = 0x546;
	pEm->Atd[0].At_d = 0x15e;
	pEm->Atd[0].W = 0x15e;
	pEm->Atd[0].D = 0x15e;

	pP[0xf].pSuper = (ULONG *)&pP->Workm;
	pP[0xf].Pos_x = 0x42;
	pP[0xf].Be_flg = 0;
	pP[0xf].pOya_parts = &pP->Be_flg;
	pP[0xf].Pos_y = -0x28e;
	pP[0xf].Pos_z = 0;
	pP[0xf].Cdir_x = 0;
	pP[0xf].Cdir_y = 0;
	pP[0xf].Cdir_z = 0;
	pP[0xf].Free[0] = 0;
	pP[0xf].Free[1] = 0;
}

/* DS 0x801c0854 (Select_pl_set) */
VOID Select_pl_set(ENEMY_WORK *pEm, ULONG No)
{
	PARTS_W	*pP;
	ULONG	L;

	pP = pEm->pSin_parts_ptr;
	pEm->Neck_no = 8;
	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	pEm->Damage_cnt = 0;
	pEm->pEnemy_ptr = (ULONG *)pEm;
	pEm->pEnemy_neck = (ULONG *)&pEm->Be_flg;
	pEm->Neck_flg = 0;

	pP[8].Free[2] = 0x60;
	pP[8].Free[3] = 0x60;
	pP[8].Free[4] = 0x200;
	pP[8].Free[0] = 0;
	pP[8].Free[1] = 0;
	pP[8].Free[5] = 0x138;

	if (pEm->Id == 0x01) {
		Select_claire_set(pEm);
	}
	if (pEm->Id == 0x0e) {
		Select_ada_set(pEm);
	}

	pEm->Pos_x = Select_pl_init[No].Pos_x;
	pEm->Pos_y = Select_pl_init[No].Pos_y;
	pEm->Pos_z = Select_pl_init[No].Pos_z;
	pEm->Cdir_x = Select_pl_init[No].Cdir_x;
	pEm->Cdir_y = Select_pl_init[No].Cdir_y;
	pEm->Routine_0 = (UCHAR)No;
	pEm->Routine_1 = 0;
	pEm->Routine_2 = 0;
	pEm->Cdir_z = Select_pl_init[No].Cdir_z;

	/* Initial L for each model: 0x800 / 0x400 / 0 / 0xC00 */
	switch (No) {
	case 0:
		L = 0x800;
		break;
	case 1:
		L = 0x400;
		break;
	case 2:
		L = 0;
		break;
	case 3:
		L = 0xc00;
		break;
	default:
		L = 0;
		break;
	}
	if (No <= 3) {
		pEm->L_pl = L;
		pEm->L_spl = L;
	}

	pEm->Spd_y = 0;
	pEm->Fpos_y = Select_dir_fpos[1][No];
	*(UINT *)&pEm->Move_no = 0x0;		/* Move_no 0, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
}

// 801C09F4
/* DS 0x801c09f4 (Select_cursor_move) */
VOID Select_cursor_move(VOID)
{
	UCHAR	bVar;
	CHAR	cVar;

	/* Flg and Cursor_no are read straight from Select_wk at every test (retail keeps no copy
	 * of Flg); the compiler then folds "Cursor_no == c && Flg bit" into one word compare */
	if ((Select_wk.Flg & 0x01) == 0) {
		if (((G.Key_trg & 0x1000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
			Select_wk.Main_no = 3;
			Snd_se_stad(0x4060000, (VECTOR *)0);
			return;
		}

		if ((G.Padd1 & 0x8000) != 0) {
			/* Up is refused for cursor 1 with lock bit 1, and for cursor 0 with lock bit 2 */
			if (((Select_wk.Cursor_no == 1) && ((Select_wk.Flg & 0x02) != 0)) || ((Select_wk.Cursor_no == 0) && ((Select_wk.Flg & 0x04) != 0))) {
				goto update_rot;
			}
			bVar = Select_wk.Cursor_no - 1;
			Select_wk.Cursor_no = bVar;
			if ((bVar & 0x80) != 0) {
				Select_wk.Cursor_no = 3;
			}
			Select_pl_dir_set();
			Snd_se_stad(0x4040000, (VECTOR *)0);
		}

		/* Down is refused for cursor 2 with lock bit 2 */
		if (((G.Padd1 & 0x2000) != 0) && !((Select_wk.Cursor_no == 2) && ((Select_wk.Flg & 0x04) != 0))) {
			cVar = Select_wk.Cursor_no + 1;
			Select_wk.Cursor_no = (UCHAR)cVar;
			if (cVar > 3) {
				Select_wk.Cursor_no = 0;
			}
			Select_pl_dir_set();
			Snd_se_stad(0x4040000, (VECTOR *)0);
		}
	}

update_rot:
	Select_pl_rot();
}

/* DS 0x801c0b78 (Select_decide) */
VOID Select_decide(VOID)
{
	ENEMY_WORK	*pEm;

	pEm = G.pEnemy[Select_wk.Cursor_no];
	if (Select_wk.Cursor_no != 0) {
		*(UINT *)&pEm->Move_no = 0x2;		/* Move_no 2, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 3;
	}
	else {
		*(UINT *)&pEm->Move_no = 0x1;		/* Move_no 1, Move_cnt 0, Hokan_flg 0, Mplay_flg 0 (DS: one sw at +0x14c) */
		pEm->Routine_2 = 1;
	}

	Fade_set(0x100, -0x400, 7, 1);
	while (Fade_status(0) == 0) {
		Select_pl_move();
		Select_pl_main();
		Select_moji_ctrl();
		Task_sleep(1);
	}
	Fade_set(0x200, 0xc00, 7, 1);
	Select_wk.Main_no = 4;
}

// 801C0C34
/* DS 0x801c0c34 (Select_sprt_disp) */
VOID Select_sprt_disp(VOID)
{
	ULONG	k;
	ULONG	cb;
	SPRT	*pSp;
	DR_MODE	*pDr;
	UCHAR	blink;

	/* Like retail, G.Cbuf_id is read again at every use (AddPrim may touch globals) */

	/* Slots 0..7: grey sprites */
	for (k = 0; k < 8; k++) {
		cb = G.Cbuf_id;
		pSp = &Select_wk.Prim[k].Sp[cb];
		pDr = &Select_wk.Prim[k].Dr[cb];
		pSp->r0 = pSp->g0 = pSp->b0 = 0x80;
		AddPrim(&G.Ot1[G.Cbuf_id][2], pSp);
		AddPrim(&G.Ot1[G.Cbuf_id][2], pDr);
	}

	/* Slots 10,11: black sprites */
	for (k = 10; k < 12; k++) {
		cb = G.Cbuf_id;
		pSp = &Select_wk.Prim[k].Sp[cb];
		pDr = &Select_wk.Prim[k].Dr[cb];
		pSp->r0 = pSp->g0 = pSp->b0 = 0;
		AddPrim(&G.Ot1[G.Cbuf_id][3], pSp);
		AddPrim(&G.Ot1[G.Cbuf_id][3], pDr);
	}

	/* Cursor blink, only while the scenario cursor is shown (Main_no 0..2) */
	if (Select_wk.Main_no <= 2) {
		if ((Select_wk.Flg & 0x01) != 0) {
			Select_wk.Blink_cnt = 0;
			Select_wk.Blink_dir = 0;
		}
		else {
			if (Select_wk.Blink_dir == 0) {
				Select_wk.Blink_cnt += 8;
				if (Select_wk.Blink_cnt == 0x80) {
					Select_wk.Blink_dir = 1 - Select_wk.Blink_dir;
				}
			}
			else {
				Select_wk.Blink_cnt -= 8;
				if (Select_wk.Blink_cnt == 0) {
					Select_wk.Blink_dir = 1 - Select_wk.Blink_dir;
				}
			}

			/* Slots 8,9: cursor arrows, skipped when the cursor is locked: slot 8 for cursor 1
			 * with lock bit 1 and for cursor 0 with lock bit 2, slot 9 for cursor 2 with lock
			 * bit 2. Retail compares the two bytes separately here (unlike Select_cursor_move,
			 * where the compiler merged them into one word compare). */
			for (k = 8; k < 10; k++) {
				if (k == 8) {
					if (Select_wk.Cursor_no == 1) {
						if ((Select_wk.Flg & 0x02) != 0) {
							continue;
						}
					}
					if (Select_wk.Cursor_no == 0) {
						if ((Select_wk.Flg & 0x04) != 0) {
							continue;
						}
					}
				}
				else {
					if (Select_wk.Cursor_no == 2) {
						if ((Select_wk.Flg & 0x04) != 0) {
							continue;
						}
					}
				}
				cb = G.Cbuf_id;
				pSp = &Select_wk.Prim[k].Sp[cb];
				pDr = &Select_wk.Prim[k].Dr[cb];
				blink = Select_wk.Blink_cnt;
				pSp->r0 = pSp->g0 = pSp->b0 = blink;
				pSp->x0 = Select_cur_x[Select_wk.Cursor_no][k - 8];
				SetSemiTrans(pSp, 1);
				SetDrawMode(pDr, 0, 0, GetTPage(1, 1, 0x280, 0x100), NULL);
				AddPrim(&G.Ot1[G.Cbuf_id][2], pSp);
				AddPrim(&G.Ot1[G.Cbuf_id][2], pDr);
			}
		}
	}

	Select_name_disp(&Select_wk);
}

// 801C0F98
/* DS 0x801c0f98 (Select_name_disp) */
VOID Select_name_disp(SELECT_WORK *pSel)
{
	UCHAR		*pIdx;
	UCHAR		k;
	ULONG		cb;
	SPRT		*pSp;
	DR_MODE		*pDr;

	pIdx = Select_name_idx[pSel->Cursor_no];	/* DS lb: Cursor_no is signed */
	k = 12;
	do {
		if (pIdx[k - 12] != 0xff) {
			cb = G.Cbuf_id;
			pSp = &pSel->Prim[k].Sp[cb];
			pDr = &pSel->Prim[k].Dr[cb];
			pSp->r0 = pSp->g0 = pSp->b0 = 0x80;
			pSp->w = Select_name_uv[pIdx[k - 12]].W;
			pSp->h = Select_name_uv[pIdx[k - 12]].H;
			pSp->u0 = Select_name_uv[pIdx[k - 12]].U;
			pSp->v0 = Select_name_uv[pIdx[k - 12]].V;
			AddPrim(&G.Ot1[G.Cbuf_id][1], pSp);
			AddPrim(&G.Ot1[G.Cbuf_id][1], pDr);
		}
		k++;
	} while (k < 0x12);
}

/* DS 0x801c1148 (Select_exit) */
VOID Select_exit(VOID)
{
	Snd_bgm_fade_ON(0x1e, 0x18);
	Snd_sys_init2();

	G.Player.Id = Select_pl_init[Select_wk.Cursor_no].Id;

	Bg_set_mode(2, 0);
	Bg_set_mode(1, 0);
	Main.Scrl_type = 0;
	Task_signal(0);
	Task_exit();
}

/* DS 0x801c11cc (Select_ada_disp) */
VOID Select_ada_disp(ENEMY_WORK *pEm)
{
	MATRIX		mtx1;
	MATRIX		mtx2;
	PARTS_W		*pP;
	ULONG		cb;
	ULONG		Cv;
	ULONG		D0;

	SetColorMatrix(&COLORMATRIX);
	CompM(&GsWSMATRIX, &GsIDMATRIX, &mtx1);
	CompM(&LIGHTMATRIX, &GsIDMATRIX, &mtx2);
	SetRotMatrix(&mtx1);
	SetTransMatrix(&mtx1);
	SetLightMatrix(&mtx2);

	cb = G.Cbuf_id;
	pP = pEm->pSin_parts_ptr;
	Cv = pP->Poly_rgb;
	D0 = (pP->Attribute & 2) >> 1;
	SortGT3_neo((TMD_MEM *)pP->pTmd, (POLY_GT3 *)(pP->pPacket + cb * 10), Cv, D0);
	SortGT4_neo((TMD_MEM *)pP->pTmd2, (POLY_GT4 *)(pP->pPacket2 + cb * 13), Cv, D0);
}

// 801C12D4
/* DS 0x801c12d4 (Select_ada_sort) */
VOID Select_ada_sort(ENEMY_WORK *pEm)
{
	/* Retail keeps a 0x20-byte slot at sp+0x10 that is never read or written (the unused MATRIX M), so the stack layout matches retail */
	MATRIX		M;
	PARTS_W		*pH;
	MATRIX		*m2_00;
	MATRIX		*m2;

	(void)M;
	pH = pEm->pSin_parts_ptr;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);

	m2_00 = &pH->Workm;
	CompM((MATRIX *)pEm->M, (MATRIX *)pH->M, m2_00);
	m2 = &pH[1].Workm;
	CompM(m2_00, (MATRIX *)pH[1].M, m2);
	CompM(m2, (MATRIX *)pH[2].M, &pH[2].Workm);
	CompM(m2, (MATRIX *)pH[5].M, &pH[5].Workm);
	CompM(m2_00, (MATRIX *)pH[9].M, &pH[9].Workm);
	CompM(m2_00, (MATRIX *)pH[0xc].M, &pH[0xc].Workm);

	MulJh(pEm, pH, &Aida_jh0_head);
	MulJh(pEm, pH, &Aida_jh1_head);
	MulJh(pEm, pH, &Aida_jh2_head);
	MulJh(pEm, pH, &Aida_jh4_head);
	MulJh(pEm, pH, &Aida_jh5_head);

	Select_ada_disp(pEm);
}

/* DS 0x801c13f8 (Select_pl_sort) */
VOID Select_pl_sort(ENEMY_WORK *pEm)
{
	if (pEm->Id == 0x01) {
		Op_PartsSort05(pEm);
	}
	else if (pEm->Id == 0x0e) {
		Select_ada_sort(pEm);
	}
}

/* DS 0x801c1440 (Select_light_ctrl) */
VOID Select_light_ctrl(ENEMY_WORK *pEm)
{
	SELECT_LIGHT	*pLt;

	pLt = &Select_wk.Light[pEm->Routine_0];
	((VOID (*)(ENEMY_WORK *, ULONG *))Select_light_proc_tbl[pLt->Wait])(pEm, (ULONG *)&pLt->Wait);

	pLt->X = (SHORT)pEm->Pos_x + 0x4b0;
	pLt->Y = (SHORT)pEm->Pos_y - 1000;
	pLt->Z = (SHORT)pEm->Pos_z;

	/* The decompile shows Workm.t + 2; the disassembly passes parts+0x5C (= Workm.t) */
	Select_light_calc(pEm->pSin_parts_ptr->Workm.t, pEm);

	SetColorMatrix(&Select_light_col[pEm->Routine_0]);
	LIGHTMATRIX = Select_light_mtx[pEm->Routine_0];
	COLORMATRIX = Select_light_col[pEm->Routine_0];
}

/* DS 0x801c15e4 (Select_light_wait)
 * The jr ra at 0x801c1600 is the return of this function (Ghidra lists it as a separate
 * Select_light_dummy0), so it is not a function of its own. */
VOID Select_light_wait(ENEMY_WORK *pEm, ULONG *pLight)
{
	if ((ULONG)pEm->Routine_0 == Select_wk.Cursor_no) {
		*pLight = 1;
	}
}

/* DS 0x801c1608 (Select_light_up) */
VOID Select_light_up(ENEMY_WORK *pEm, ULONG *pLight)
{
	SELECT_LIGHT	*pLt = (SELECT_LIGHT *)pLight;
	ULONG		next;

	next = 3;
	if ((ULONG)pEm->Routine_0 == Select_wk.Cursor_no) {
		if (pLt->R < 0xdd) {
			pLt->R += 0x14;
			pLt->G += 0x14;
			pLt->B += 0x14;
			return;
		}
		next = 2;
	}
	*pLight = next;
}

/* DS 0x801c1664 (Select_light_on)
 * The jr ra at 0x801c1680 is the return of this function (Ghidra lists it as a separate
 * Select_light_dummy1), so it is not a function of its own. */
VOID Select_light_on(ENEMY_WORK *pEm, ULONG *pLight)
{
	if ((ULONG)pEm->Routine_0 != Select_wk.Cursor_no) {
		*pLight = 3;
	}
}

/* DS 0x801c1688 (Select_light_down) */
VOID Select_light_down(ENEMY_WORK *pEm, ULONG *pLight)
{
	SELECT_LIGHT	*pLt = (SELECT_LIGHT *)pLight;

	if ((ULONG)pEm->Routine_0 == Select_wk.Cursor_no) {
		*pLight = 1;
	}
	else if (pLt->R == 0) {
		*pLight = 0;
	}
	else {
		pLt->R -= 0x14;
		pLt->G -= 0x14;
		pLt->B -= 0x14;
	}
}

/* DS 0x801c16e4 (Select_light_init) */
VOID Select_light_init(ENEMY_WORK *pEm)
{
	SELECT_LIGHT	*pLt;
	SELECT_PL_INIT	*pPl;

	pLt = &Select_wk.Light[pEm->Routine_0];
	pPl = &Select_pl_init[pEm->Routine_0];

	pLt->On = 1;
	pLt->Wait = 0;
	pLt->Range = 8000;
	pLt->R = 0;
	pLt->G = 0;
	pLt->B = 0;
	pLt->X = (SHORT)pPl->Pos_x + 0x4b0;
	pLt->Y = (SHORT)pPl->Pos_y - 1000;
	pLt->Z = (SHORT)pPl->Pos_z;
}

/* DS 0x801c1798 (Select_light_calc) */
VOID Select_light_calc(LONG *pPos, ENEMY_WORK *pEm)
{
	SELECT_LIGHT	*pLt;
	VECTOR		vec;
	UCHAR		col[3];
	LONG		dist;
	LONG		range;
	LONG		k;

	pLt = &Select_wk.Light[pEm->Routine_0];

	vec.vx = (pPos[0] - pLt->X) >> 4;
	vec.vy = (pPos[1] - pLt->Y) >> 4;
	vec.vz = (pPos[2] - pLt->Z) >> 4;
	dist = SquareRoot0(vec.vx * vec.vx + vec.vz * vec.vz);

	range = (LONG)(pLt->Range >> 4);
	k = range - dist;
	if (k < 0) {
		k = 0;
	}

	col[0] = 0;
	col[1] = 0;
	col[2] = 0;
	/* Range == 0 gives black. Range below 16 makes the divisor zero; the original faults there
	 * (compiler divide check), which is not reproduced, so the colour is left black here. */
	if ((pLt->Range != 0) && (range != 0)) {
		col[0] = (UCHAR)(((LONG)pLt->R * k) / range);
		col[1] = (UCHAR)(((LONG)pLt->G * k) / range);
		col[2] = (UCHAR)(((LONG)pLt->B * k) / range);
	}

	Select_light_set(pEm->Routine_0, &vec, col);
	SetBackColor(0x40, 0x40, 0x40);
}

// 801C1968
/* DS 0x801c1968 (Select_light_set) */
VOID Select_light_set(ULONG No, VECTOR *pVec, UCHAR *pCol)
{
	SELECT_LIGHT	*pLt;
	MATRIX		*pMtx;
	MATRIX		*pCl;
	VECTOR		nrm;

	/* retail resolves the light, matrix and colour addresses before VectorNormal and keeps
	 * them in callee-saved registers; nrm sits at sp+0x10 and is read as full words */
	pLt = &Select_wk.Light[No];
	pMtx = &Select_light_mtx[No];
	pCl = &Select_light_col[No];
	VectorNormal(pVec, &nrm);

	pMtx->m[0][0] = nrm.vx * -pLt->On;
	pMtx->m[0][1] = nrm.vy * -pLt->On;
	pMtx->m[0][2] = nrm.vz * -pLt->On;

	pCl->m[0][0] = pCol[0] << 4;
	pCl->m[1][0] = pCol[1] << 4;
	pCl->m[2][0] = pCol[2] << 4;
}

// 801C1A58
/* DS 0x801c1a58 (Select_pl_trans) */
VOID Select_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos)
{
	/* Retail keeps a 0x30-byte slot at sp+0x10 that is never read or written. It holds the
	 * unused MATRIX M and VECTOR v of Joint_trans2 (same arguments), which this routine is
	 * cut down from; the (void) casts keep them without an unused-variable warning. */
	MATRIX	M;
	VECTOR	v;
	UCHAR	n;

	(void)M;
	(void)v;
	RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->M);
	Set_view(Select_view_eye, Select_view_ref);
	SetGeomOffset(0xa0, 0x78);
	SetGeomScreen(300);

	n = pEm->Parts_num;
	do {
		if ((pParts->Be_flg & 1) != 0) {
			ko_joint_trans2(pEm, pParts, pParts->Be_flg, &pParts->Workm);
		}
		n--;
		pParts++;
	} while (n != 0);
}

/* DS 0x801c1afc (Select_pl_main) */
VOID Select_pl_main(VOID)
{
	ULONG		i;
	ENEMY_WORK	*pEm;

	for (i = 0; i < 4; i++) {
		pEm = G.pEnemy[i];
		G.pEm = pEm;
		Select_light_ctrl(pEm);

		/* G.Player takes the model's matrix (0x14 bytes from M) and position */
		G.Player.pSin_parts_ptr = pEm->pSin_parts_ptr;
		memcpy(G.Player.M, pEm->M, 0x14);
		G.Player.Pos_x = pEm->Pos_x;
		G.Player.Pos_y = pEm->Pos_y;
		G.Player.Pos_z = pEm->Pos_z;

		Select_pl_sort(pEm);
		SetSpadStack();
		Select_pl_trans(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, &pEm->Pos_x);
		ResetSpadStack();
	}

	Select_sprt_disp();
}

// 801C1BF0
/* DS 0x801c1bf0 (Select_pl_move) */
VOID Select_pl_move(VOID)
{
	ULONG		i;
	ENEMY_WORK	*pEm;
	ULONG		ret;
	UCHAR		*pRt;

	/* DS walks the four models as one array from G.pEnemy[0] (Select_init allocates them together) */
	pEm = G.pEnemy[0];
	for (i = 0; i < 4; i++, pEm++) {
		G.pEm = pEm;
		pRt = &pEm->Routine_2;

		if (pEm->Id == 14) {
			/* Ada: state 0 moves the joints, state 1 moves until done, then latches state 2 */
			if (*pRt == 1) {
				ret = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x200);
				if (ret != 0) {
					*pRt = 2;
				}
			}
			else if (*pRt == 0) {
				Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x200);
			}
		}
		else if ((pEm->Id == 0x00) || (pEm->Id == 0x01) || (pEm->Id == 0x0b)) {
			switch (*pRt) {
			case 0:
				pEm->Move_no = 0;
				pEm->Move_cnt = 0;
				pEm->Hokan_flg = 0;
				pEm->Mplay_flg = 0;
				pEm->Timer0 = 0;
				*pRt = 1;
				/* fall through */
			case 1:
				ret = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x200);
				if (ret != 0) {
					pEm->Move_no = 1;
					pEm->Move_cnt = 0;
					pEm->Hokan_flg = 0;
					pEm->Mplay_flg = 0;
					*pRt = 2;
				}
				break;
			case 2:
				Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x200);
				break;
			case 3:
				ret = Joint_move(pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr, 0x200);
				*pRt = (ret != 0) ? 4 : 3;
				break;
			default:
				break;
			}
		}
	}
}

/* DS 0x801c1df4 (Select_fade_in) */
VOID Select_fade_in(VOID)
{
	if (Fade_status(0) != 0) {
		Fade_set(0x200, 0, 7, 1);
		Select_wk.Main_no = 2;
	}
}

/* DS 0x801c1e38 (Select_fade_out) */
VOID Select_fade_out(VOID)
{
	if (Fade_status(0) != 0) {
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x7fff, 0xffffff, NULL);
		Select_wk.Main_no = 5;
	}
}

/* DS 0x801c1e94 (Select_pl_dir_set) */
VOID Select_pl_dir_set(VOID)
{
	ULONG		i;
	UCHAR		cur;
	ENEMY_WORK	*pEm;

	cur = Select_wk.Cursor_no;
	for (i = 0; i < 4; i++) {
		pEm = G.pEnemy[i];
		pEm->L_spl = Select_dir_L[cur][i];
		pEm->Spd_y = 0;
		pEm->Fpos_y = Select_dir_fpos[cur][i];
	}
	Select_wk.Moji_no = 0;
	Select_wk.Flg |= 0x01;
}

// 801C1F2C
/* DS 0x801c1f2c (Select_pl_rot) */
VOID Select_pl_rot(VOID)
{
	ULONG		i;
	ENEMY_WORK	*pEm;
	UCHAR		*pFlg;
	ULONG		uVal;
	SHORT		sVal;
	LONG		d;

	/* The four models are consecutive ENEMY_WORK blocks (see Select_init); retail steps the
	 * pointer by sizeof(ENEMY_WORK) instead of reading G.pEnemy[i] */
	pEm = G.pEnemy[0];
	for (i = 0; i < 4; i++, pEm++) {

		/* Turntable: L_pl is the current angle, L_spl the target angle */
		if (pEm->L_pl == pEm->L_spl) {
			if (pEm->L_spl == 0x1000) {
				pEm->L_pl = 0;
				pEm->L_spl = 0;
			}
			/* retail clears bit 0 of Flg through an address held in a register (la, lbu, sb) */
			pFlg = &Select_wk.Flg;
			*pFlg = *pFlg & 0xfe;
		}
		else {
			if ((pEm->L_pl > 0xbff) && (pEm->L_spl == 0)) {
				pEm->L_spl = 0x1000;
			}
			if ((pEm->L_pl == 0) && (pEm->L_spl > 0xbff)) {
				pEm->L_pl = 0x1000;
			}
			uVal = pEm->L_pl;
			if (pEm->L_spl <= uVal) {
				pEm->L_pl = uVal - 0x40;
			}
			else {
				pEm->L_pl = uVal + 0x40;
			}
			pEm->Pos_x = (rsin(pEm->L_pl) * 0x8ca) >> 12;
			pEm->Pos_z = (rcos(pEm->L_pl) * 0x8ca) >> 12;
		}

		/* Cdir_y eases towards Fpos_y */
		sVal = pEm->Cdir_y;
		if (sVal == pEm->Fpos_y) {
			if (sVal == 0x1000) {
				pEm->Cdir_y = 0;
				pEm->Fpos_y = 0;
			}
			pEm->Spd_y = 0;
		}
		else {
			if ((pEm->Fpos_y == 0) && (sVal > 3000)) {
				pEm->Fpos_y = 0x1000;
			}
			if ((pEm->Cdir_y == 0) && (pEm->Fpos_y > 3000)) {
				pEm->Cdir_y = 0x1000;
			}
			if (pEm->Spd_y == 0) {
				d = (LONG)pEm->Fpos_y - (LONG)pEm->Cdir_y;
				if (d < 0) {
					d += 0xf;
				}
				pEm->Spd_y = (SHORT)(d >> 4);
			}
			pEm->Cdir_y += pEm->Spd_y;
		}
	}
}

/* DS 0x801c212c (Select_moji_set) */
VOID Select_moji_set(ULONG *pDst, ULONG *pSrc)
{
	((SELECT_MOJI_REC *)pDst)->Word0 = ((SELECT_MOJI_REC *)pSrc)->Word0;
	((SELECT_MOJI_REC *)pDst)->Word1 = ((SELECT_MOJI_REC *)pSrc)->Word1;
	((SELECT_MOJI_REC *)pDst)->Word2 = ((SELECT_MOJI_REC *)pSrc)->Word2 + ((ULONG)GetClut(0, 0x1ef) << 16);
	pDst[3] = pSrc[3];		/* ot, r, g, visible bytes copied as-is */
}

// 801C21B4
/* DS 0x801c21b4 (Select_moji_dm_set) */
VOID Select_moji_dm_set(VOID)
{
	SELECT_MOJI	*pM;
	LONG		i;
	LONG		off;

	/* Eight DR_MODE packets per buffer at sizeof(SELECT_MOJI) past pM (stride 0x60 per buffer, 0xC per packet),
	 * filled from the last one down. Retail re-reads G.Cbuf_id for every use. */
	pM = Select_wk.pMoji;
	off = 0x60;
	i = 8;
	do {
		off -= 0xc;
		SetDrawMode((DR_MODE *)((UCHAR *)pM + sizeof(SELECT_MOJI) + G.Cbuf_id * 0x60 + off), 0, 0, 0x9a, NULL);
		i--;
		AddPrim(&G.Ot1[G.Cbuf_id][i], (UCHAR *)pM + sizeof(SELECT_MOJI) + G.Cbuf_id * 0x60 + off);
	} while (i != 0);
}

/* DS 0x801c227c (Select_moji_init) */
VOID Select_moji_init(VOID)
{
	SELECT_MOJI	*pM;
	ULONG		i;

	pM = Select_wk.pMoji;
	Select_moji_clear();
	for (i = 0; i < 0x8d; i++) {
		Select_moji_set((ULONG *)&pM->Rec[i], Select_moji_tbl[i]);
	}
	pM->Blink = 0;
}

/* DS 0x801c22f4 (Select_moji_print) */
ULONG Select_moji_print(LONG Wait, UCHAR *pStr, ULONG *pEnd)
{
	SELECT_MOJI	*pM;
	UCHAR		*pCur;
	UCHAR		bVal;
	UCHAR		printed;

	pM = Select_wk.pMoji;
	if (*pEnd != 0) {
		return 0;
	}

	if (pM->pStr != pStr) {
		pM->pStr = pStr;
		pM->pCur = pStr;
		pM->Wait = 1;
	}
	bVal = pM->Wait;
	pM->Wait = bVal - 1;
	if ((UCHAR)(bVal - 1) != 0) {
		return 0;
	}

	pM->Wait = (UCHAR)Wait;
	pCur = pM->pCur;
	printed = 0;

	for (;;) {
		bVal = *pCur;
		if (bVal == 0) {
			pM->pStr = NULL;
						*pEnd = 1;
			return 1;
		}

		if (bVal == 93) {
			/* newline */
			pM->Col = 0;
			pM->Row++;
		}
		else {
			if (bVal < 94) {
				if (bVal == 91) {
					/* "[ddd": three digit delay in decimal, then stop for this frame */
					pM->Wait = (UCHAR)((pCur[1] - 48) * 100);
					pM->Wait = (UCHAR)(pM->Wait + (pCur[2] - 48) * 10);
					pM->Wait = (UCHAR)(pCur[3] + (pM->Wait - 48));
					pM->pCur = pCur + 4;
					return 0;
				}
			}
			else if (bVal == 124) {
				/* "|d": colour digit, two characters consumed */
				pM->Color = (UCHAR)(pCur[1] - 48);
				pM->pCur = pCur + 1;
				goto print_next;
			}

			printed = 1;
			pM->Text[pM->Col + pM->Row * 0x14] = bVal;
			pM->Attr[pM->Col + pM->Row * 0x14] = pM->Color;
			pM->Blink = 0x10;
			pM->Col++;
		}

print_next:
		if (pM->Col > 0x13) {
			pM->Col = 0;
			pM->Row++;
		}
		Snd_se_stad(0x4100000, (VECTOR *)0);

		pCur = pM->pCur + 1;
		pM->pCur = pCur;
		if ((Wait != 0) && (printed != 0)) {
			return 0;
		}
	}
}

// 801C255C
/* DS 0x801c255c (Select_moji_ctrl) */
VOID Select_moji_ctrl(VOID)
{
	UCHAR	cur;

	cur = Select_wk.Cursor_no;
	switch (Select_wk.Moji_no) {
	case 0:
		Select_moji_init();
		Select_moji_end = 0;
		Select_wk.Moji_no = 1;
		break;
	case 1:
		Select_moji_print(1, Select_str[cur][0], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 2;
		break;
	case 2:
		Select_moji_print(1, Select_str[cur][1], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 3;
		break;
	case 3:
		Select_moji_print(1, Select_str[cur][2], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 4;
		break;
	case 4:
		Select_moji_print(1, Select_str[cur][3], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 5;
		break;
	case 5:
		Select_moji_print(1, Select_str[cur][4], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 6;
		break;
	case 6:
		Select_moji_print(1, Select_str[cur][5], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		/* scenarios 0 and 3 have no seventh line */
		if ((cur == 3) || (cur == 0)) {
			Select_wk.Moji_no = 8;
		}
		else {
			Select_wk.Moji_no = 7;
		}
		break;
	case 7:
		Select_moji_print(1, Select_str[cur][6], &Select_moji_end);
		if (Select_moji_end == 0) {
			goto update_moji;
		}
		Select_wk.Moji_no = 8;
		break;
	default:
		break;
	}
	if (Select_moji_end != 0) {
		Select_moji_end = 0;
	}

update_moji:
	Select_moji_update(1);
	Select_moji_trans(0x8d);
}

/* DS 0x801c283c (Select_moji_clear) */
VOID Select_moji_clear(VOID)
{
	SELECT_MOJI	*pM;
	ULONG		k;

	pM = Select_wk.pMoji;
	for (k = 0; k < 0x8c; k++) {
		pM->Text[k] = 0x20;
		pM->Attr[k] = 0;
	}
	pM->Color = 0;
	pM->Row = 0;
	pM->Col = 0;
}

// 801C2880
/* DS 0x801c2880 (Select_moji_update) */
VOID Select_moji_update(LONG Disp)
{
	SELECT_MOJI	*pM;
	UCHAR		*pText;
	UCHAR		*pVis;
	UCHAR		*pEnd;
	ULONG		uVal;

	pM = Select_wk.pMoji;

	/* Records 1..140 are the text characters (Text[k] -> Rec[k + 1]). Like retail, the loop
	 * walks a pointer to the Vis byte of each record: Word1 is at pVis - 0xB, Word2 at
	 * pVis - 7. GetClut is called inside the loop, once per drawn character. */
	pEnd = (UCHAR *)&pM->Rec[141];
	pText = pM->Text;
	pVis = &pM->Rec[1].Vis;
	do {
		uVal = (ULONG)*pText - 0x20;
		if (*pText != 0x20) {
			*(ULONG *)(pVis - 0xb) = 8 | (10 << 16);
			*(ULONG *)(pVis - 7) = (((((uVal & 0xffff) >> 5) * 0xa00) & 0xffff) | ((uVal & 0x1f) << 3))
				+ ((ULONG)GetClut(0, 0x1ef) << 16);
			*pVis = (UCHAR)Disp;
		}
		pVis = pVis + sizeof(SELECT_MOJI_REC);
		pText++;
	} while (pVis < pEnd + 0xf);

	/* Record 0 is the blinking cursor */
	pM->Rec[0].Word0 = (ULONG)(USHORT)(pM->Col * 8 + 0xe) | ((ULONG)(USHORT)(pM->Row * 10 + 0xa0) << 16);
	pM->Rec[0].Word2 = (pM->Rec[0].Word2 & 0xffff) | 0x7bc00000;
	pM->Blink = (pM->Blink + 1) & 0x1f;
	if (Disp == 0) {
		pM->Rec[0].Vis = 0;
	}
	else {
		pM->Rec[0].Vis = pM->Blink & 0x10;
	}
}

// 801C29B0
/* DS 0x801c29b0 (Select_moji_trans) */
VOID Select_moji_trans(LONG Num)
{
	SELECT_MOJI	*pM;
	ULONG		*pW;
	ULONG		*pEnd;
	UCHAR		*pB;
	ULONG		*p;
	ULONG		*pNext;
	ULONG		*pP3;
	ULONG		*pOt;

	/* Like retail, the record is walked with two pointers (pW at its first word, pB at its
	 * Ot byte) and the packet with two (p and pP3 = p + 3, so p[1], p[2], p[3], p[4] are
	 * pP3[-2], pP3[-1], pP3[0], pP3[1]). */
	pM = Select_wk.pMoji;
	p = (ULONG *)((UCHAR *)pM + G.Cbuf_id * 0xb04 + 0xab8);
	pW = &pM->Rec[0].Word0;
	pEnd = pW + Num * 4;
	pB = &pM->Rec[0].Ot;
	pP3 = p + 3;
	pOt = G.Ot1[G.Cbuf_id];

	do {
		pNext = p;
		if (pB[3] != 0) {		/* Vis */
			/* 5-word textured sprite packet: tag, colour/command, xy, uv+clut, wh */
			p[0] = 0x04000000;
			pP3[-2] = ((ULONG)pB[1] * 0x1000000) + ((ULONG)pB[2] * 0x10101) + 0x64000000;	/* R, G */
			pP3[-1] = *pW;				/* Word0 */
			pP3[1] = *(ULONG *)(pB - 8);		/* Word1 */
			pNext = p + 5;
			pP3[0] = *(ULONG *)(pB - 4);		/* Word2 */
			pP3 += 5;
			AddPrim(&pOt[pB[0]], p);
		}
		pW += 4;
		pB = pB + sizeof(SELECT_MOJI_REC);
		p = pNext;
	} while (pW < pEnd);

	Select_moji_dm_set();
}
