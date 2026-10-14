/*
 * TITLE.C - Title screen, menus and Ex sub-routines
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\TITLE.C
 */
#include "TYPES.H"
#include "GAME.H"
#include "MAIN.H"
#include "CONFIG.H"
#include "SLD.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include "TITLE.h"
#include "CD.H"
#include "EM_SET.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "GAME.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "MOVIE.H"
#include "PAD.H"
#include "PLAYER.H"
#include "ROOM.H"
#include "SCE0.H"
#include "SND.H"
#include "SPL.H"
#include "STATUS.H"
#include "SUB.H"
#include "DOOR.H"
#include "PL_WEP.H"
#include "SCE2.H"
#include "FILECODE.H"
#include "flag.h"
/* TITLE work buffers (declared extern in TITLE.H) */
ULONG Title_bg_work_buf[0x96F8] RAM_SECTION(".buf_title_bg_work_buf");	/* DS 0x80132420, Title_BG CD load buffer */

/* TITLE overlay data (declared extern in TITLE.H). ROM items carry the TITLE.BIN bytes at their DS address; RAM items are zero-initialised. */
/* DS 0x80168000 (Ex_room_snd_buf), ROOM800.SND load buffer. Size 0x9C4C = floc[ROOM800_SND] byte size (FILE_LOC.H), the length Cd_read loads. */
UCHAR Ex_room_snd_buf[0x9C4C] RAM_SECTION(".buf_ex_room_snd_buf");

/* DS 0x8018d270 (Init_tbl_buf) */
UCHAR Init_tbl_buf[0x2D90] RAM_SECTION(".buf_init_tbl_buf");

/* DS 0x80196794 (Open_msg_00) */
UCHAR Open_msg_00[] = {
	0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x44, 0x41, 0x55, 0x00, 0x53, 0x41,
	0x4e, 0x41, 0x00, 0x4c, 0x3d, 0x4e, 0x50, 0x41, 0x40, 0x00, 0x3e, 0x55, 0xfc, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3d, 0x4a, 0x00, 0x51, 0x4a, 0x41, 0x4f, 0x3f, 0x3d, 0x4c, 0x3d, 0x3e,
	0x48, 0x41, 0x00, 0x40, 0x41, 0x4f, 0x50, 0x45, 0x4a, 0x55, 0x01, 0xfa, 0x01, 0xfd, 0x73, 0xfa,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x44, 0x45, 0x4f, 0x00, 0x45, 0x4f, 0x00, 0x46,
	0x51, 0x4f, 0x50, 0x00, 0x50, 0x44, 0x41, 0x00, 0x3e, 0x41, 0x43, 0x45, 0x4a, 0x4a, 0x45, 0x4a,
	0x43, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x42, 0x00, 0x50, 0x44, 0x41, 0x45, 0x4e,
	0x00, 0x53, 0x4b, 0x4e, 0x4f, 0x50, 0x00, 0x4a, 0x45, 0x43, 0x44, 0x50, 0x49, 0x3d, 0x4e, 0x41,
	0x01, 0xfa, 0x01, 0xfe, 0x73, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1d, 0x50, 0x00, 0x50, 0x44, 0x41, 0x00, 0x4f, 0x3d, 0x49, 0x41, 0x00, 0x50, 0x45, 0x49,
	0x41, 0x18, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x50, 0x00,
	0x50, 0x44, 0x41, 0x00, 0x4f, 0x3d, 0x49, 0x41, 0x00, 0x4c, 0x48, 0x3d, 0x3f, 0x41, 0x01, 0xfa,
	0x01, 0xfd, 0x5a, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x4b,
	0x51, 0x00, 0x44, 0x3d, 0x52, 0x41, 0x00, 0x50, 0x4b, 0x00, 0x4f, 0x51, 0x4e, 0x52, 0x45, 0x52,
	0x41, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x44, 0x45, 0x4f, 0x00,
	0x4a, 0x45, 0x43, 0x44, 0x50, 0x49, 0x3d, 0x4e, 0x41, 0xfa, 0x01, 0xfd, 0x5a, 0xfa, 0x00, 0xfc,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x4b, 0x00, 0x47, 0x4a, 0x4b, 0x53, 0x00,
	0x50, 0x44, 0x41, 0x00, 0x50, 0x4e, 0x51, 0x41, 0x00, 0x41, 0x4a, 0x40, 0x01, 0xfa, 0x01, 0xfe,
	0x32, 0xfa, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x35, 0x4b, 0x51, 0x00, 0x4b, 0x4a, 0x3f, 0x41,
	0x00, 0x3d, 0x43, 0x3d, 0x45, 0x4a, 0x00, 0x4f, 0x50, 0x41, 0x4c, 0x4c, 0x41, 0x40, 0x00, 0x45,
	0x4a, 0x50, 0x4b, 0xfa, 0x01, 0xfd, 0x3c, 0xfa, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x50, 0x44,
	0x41, 0x00, 0x53, 0x4b, 0x4e, 0x48, 0x40, 0x00, 0x4b, 0x42, 0x00, 0x4f, 0x51, 0x4e, 0x52, 0x45,
	0x52, 0x3d, 0x48, 0x00, 0x44, 0x4b, 0x4e, 0x4e, 0x4b, 0x4e, 0x01, 0xfa, 0x01, 0xfd, 0x3c, 0xfa,
	0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x23, 0x4b, 0x4b, 0x40, 0x00, 0x48, 0x51, 0x3f, 0x47, 0x01, 0x01, 0x01, 0xfa, 0x01, 0xfe, 0x23,
	0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x45, 0x43,
	0x44, 0x50, 0x45, 0x4a, 0x43, 0x00, 0x42, 0x4b, 0x41, 0x4f, 0x00, 0x45, 0x4f, 0xfc, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x4b, 0x50, 0x00, 0x50, 0x44, 0x41,
	0x00, 0x4b, 0x4a, 0x48, 0x55, 0x00, 0x53, 0x3d, 0x55, 0xfa, 0x01, 0xfd, 0x73, 0xfa, 0x00, 0xfc,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x4b, 0x00, 0x4f, 0x51, 0x4e, 0x52, 0x45,
	0x52, 0x41, 0x00, 0x50, 0x44, 0x45, 0x4f, 0x00, 0x44, 0x4b, 0x4e, 0x4e, 0x4b, 0x4e, 0x01, 0xfa,
	0x01, 0xfe, 0x73, 0xfa, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x23, 0x01, 0x01, 0x01, 0x00, 0x23, 0x3b, 0x32, 0x45, 0x4e, 0x51, 0x4f, 0x01, 0x01,
	0x01, 0xfa, 0x01, 0xfd, 0x73, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x25, 0x00, 0x44, 0x3d, 0x52, 0x41, 0x00, 0x50, 0x4b, 0x00, 0x3e, 0x4e, 0x45, 0x4a,
	0x43, 0x00, 0x45, 0x50, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x50, 0x4b, 0x00, 0x31, 0x49, 0x3e, 0x4e, 0x41, 0x48, 0x48, 0x3d, 0x01, 0x01, 0x01, 0xfa, 0x01,
	0xfe, 0x73, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x44, 0x41, 0x00, 0x4b, 0x4a, 0x48, 0x55,
	0x00, 0x50, 0x44, 0x45, 0x4a, 0x43, 0x00, 0x50, 0x44, 0x3d, 0x50, 0x00, 0x49, 0x3d, 0x50, 0x50,
	0x41, 0x4e, 0x4f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x4b, 0x53, 0x00, 0x45, 0x4f, 0x00, 0x55,
	0x4b, 0x51, 0x4e, 0x00, 0x42, 0x45, 0x43, 0x44, 0x50, 0x45, 0x4a, 0x43, 0x00, 0x4f, 0x4c, 0x45,
	0x4e, 0x45, 0x50, 0x01, 0xfa, 0x01, 0xfd, 0x73, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x4b,
	0x51, 0x4e, 0x00, 0x3d, 0x3f, 0x50, 0x45, 0x4b, 0x4a, 0x4f, 0x00, 0x53, 0x45, 0x48, 0x48, 0x00,
	0x40, 0x41, 0x50, 0x41, 0x4e, 0x49, 0x45, 0x4a, 0x41, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x50, 0x44,
	0x41, 0x00, 0x42, 0x3d, 0x50, 0x41, 0x00, 0x4b, 0x42, 0x00, 0x3d, 0x48, 0x48, 0x00, 0x44, 0x51,
	0x49, 0x3d, 0x4a, 0x47, 0x45, 0x4a, 0x40, 0x01, 0xfa, 0x01, 0xfe, 0x73
};

/* DS 0x80196a70 (Open_msg_ptr) */
USHORT Open_msg_ptr[6] = { 0x0000, 0x0085, 0x0121, 0x0190, 0x01f3, 0x0252 };

/* DS 0x80196a8c (Ex_battle_life) */
UCHAR Ex_battle_life[] = {
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00
};

/* DS 0x80196a9c (Moji_tile_tbl) */
TILE Moji_tile_tbl[4] = {
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 320, 40 },
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 320, 40 },
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 200, 320, 40 },
	{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 200, 320, 40 }
};

/* DS 0x80196adc (Moji_work_tbl) */
T_MOJI_TBL Moji_work_tbl[62] = {
	{ 33, 132, 256, 16, 0x00, 0x20, 0x08, 0x1e, 0x01, 0x01 },
	{ 33, 133, 256, 16, 0x00, 0x20, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 148, 256, 16, 0x00, 0x10, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 149, 256, 16, 0x00, 0x10, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 164, 256, 16, 0x00, 0x70, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 165, 256, 16, 0x00, 0x70, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 180, 256, 16, 0x00, 0x30, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 181, 256, 16, 0x00, 0x30, 0x08, 0x1e, 0x01, 0x02 },
	{ 40, 152, 128, 16, 0x00, 0x90, 0x08, 0x1e, 0x01, 0x01 },
	{ 40, 153, 128, 16, 0x00, 0x90, 0x08, 0x1e, 0x01, 0x02 },
	{ 168, 152, 256, 16, 0x80, 0x90, 0x08, 0x1e, 0x01, 0x01 },
	{ 168, 153, 256, 16, 0x80, 0x90, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 172, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 173, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 152, 256, 16, 0x00, 0x80, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 153, 256, 16, 0x00, 0x80, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 172, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 173, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 152, 128, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 153, 128, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x02 },
	{ 160, 152, 256, 16, 0x80, 0xc0, 0x08, 0x1e, 0x01, 0x01 },
	{ 160, 153, 256, 16, 0x80, 0xc0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 172, 256, 16, 0x00, 0xd0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 173, 256, 16, 0x00, 0xd0, 0x08, 0x1e, 0x01, 0x02 },
	{ 320, 102, 128, 24, 0x80, 0x70, 0x08, 0x1e, 0x00, 0x00 },
	{ 32, 172, 256, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 173, 256, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 192, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 193, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 198, 256, 24, 0x00, 0x58, 0x08, 0x1e, 0x00, 0x00 },
	{ 100, 40, 75, 40, 0x00, 0x00, 0x0b, 0x1c, 0x01, 0x01 },
	{ 25, 80, 128, 56, 0x00, 0x28, 0x0b, 0x1c, 0x01, 0x01 },
	{ 153, 80, 128, 56, 0x00, 0x60, 0x0b, 0x1c, 0x01, 0x01 },
	{ 281, 80, 12, 56, 0x00, 0x98, 0x0b, 0x1c, 0x01, 0x01 },
	{ 150, 20, 128, 200, 0x80, 0x00, 0x0b, 0x1d, 0x01, 0x01 },
	{ 18, 76, 128, 80, 0x00, 0x00, 0x0a, 0x1f, 0x01, 0x02 },
	{ 146, 76, 128, 80, 0x00, 0x50, 0x0a, 0x1f, 0x01, 0x02 },
	{ 274, 76, 128, 80, 0x00, 0xa0, 0x0a, 0x1f, 0x01, 0x02 },
	{ 156, 108, 96, 24, 0x20, 0xa0, 0x0a, 0x1f, 0x01, 0x01 },
	{ 252, 108, 56, 24, 0x20, 0xb8, 0x0a, 0x1f, 0x01, 0x01 },
	{ 156, 108, 96, 24, 0x20, 0xd0, 0x0a, 0x1f, 0x01, 0x02 },
	{ 252, 108, 56, 24, 0x20, 0xe8, 0x0a, 0x1f, 0x01, 0x02 },
	{ 34, 152, 256, 16, 0x00, 0x20, 0x08, 0x1e, 0x01, 0x01 },
	{ 34, 153, 256, 16, 0x00, 0x20, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 168, 256, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 169, 256, 16, 0x00, 0xc0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 184, 256, 16, 0x00, 0xe0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 185, 256, 16, 0x00, 0xe0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 200, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 201, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x02 },
	{ 0, 8, 256, 88, 0x00, 0x00, 0x0b, 0x1c, 0x01, 0x01 },
	{ 184, 8, 256, 88, 0x00, 0x58, 0x0b, 0x1c, 0x01, 0x01 },
	{ 0, 8, 256, 88, 0x00, 0x00, 0x0b, 0x1c, 0x01, 0x01 },
	{ 184, 8, 256, 88, 0x00, 0x58, 0x0b, 0x1c, 0x01, 0x01 },
	{ 96, 172, 32, 16, 0x40, 0xf0, 0x08, 0x1e, 0x01, 0x01 },
	{ 96, 173, 32, 16, 0x40, 0xf0, 0x08, 0x1e, 0x01, 0x02 },
	{ 144, 172, 32, 16, 0x70, 0xf0, 0x08, 0x1e, 0x01, 0x01 },
	{ 144, 173, 32, 16, 0x70, 0xf0, 0x08, 0x1e, 0x01, 0x02 },
	{ 192, 172, 32, 16, 0xa0, 0xf0, 0x08, 0x1e, 0x01, 0x01 },
	{ 192, 173, 32, 16, 0xa0, 0xf0, 0x08, 0x1e, 0x01, 0x02 },
	{ 32, 192, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x01 },
	{ 32, 193, 256, 16, 0x00, 0xb0, 0x08, 0x1e, 0x01, 0x02 }
};

/* DS 0x80196e84 (Info_tile) */
TILE Info_tile[2][4] = {
	{
		{ 0x00000000, 0x64, 0x64, 0x64, 0x00, 0, 24, 20, 195 },
		{ 0x00000000, 0x64, 0x64, 0x64, 0x00, 152, 24, 168, 195 },
		{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
		{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 }
	},
	{
		{ 0x00000000, 0x64, 0x64, 0x64, 0x00, 0, 24, 20, 195 },
		{ 0x00000000, 0x64, 0x64, 0x64, 0x00, 152, 24, 168, 195 },
		{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
		{ 0x00000000, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 }
	}
};

/* DS 0x80196f08 (Ex_mode_sel_flg) */
ULONG Ex_mode_sel_flg = 0x00000000;

/* DS 0x80196f0c (pRoom_ex) */
ULONG pRoom_ex = 0x00000000;

/* DS 0x80196f12 (Ex_room_cnt) */
UCHAR Ex_room_cnt = 0x00;

/* DS 0x80196f14 (Ex_room_snd_hdr) */
ULONG Ex_room_snd_hdr = 0x00000000;

/* DS 0x80196f18 (Ex_room_snd_a) */
ULONG Ex_room_snd_a = 0x00000000;

/* DS 0x80196f1c (Ex_room_snd_b) */
ULONG Ex_room_snd_b = 0x00000000;

/* DS 0x80196f90 (Sel_mode_flg) */
ULONG Sel_mode_flg = 0x00000000;

/* DS 0x80196f94 (Rookie_flg_bak) */
ULONG Rookie_flg_bak = 0x00000000;

/* DS 0x80196f98 (Game_mode_flg) */
ULONG Game_mode_flg = 0x00000000;

/* DS 0x80196f9c (Sound_sel_flg) */
ULONG Sound_sel_flg = 0x00000000;

/* DS 0x80196fa0 (Info_dr_mode) */
DR_MODE Info_dr_mode[2][4] = {
	{
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } }
	},
	{
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } },
		{ 0x00000000, { 0x00000000, 0x00000000 } }
	}
};

/* DS 0x80197000 (Ti) */
TITLE_WORK Ti = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		/* Main_rtn .. Cur */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		/* Disp_cnt .. Pad_0F */
	0x0000, 0, 0,		/* Timer, Kido, Add */
	0, 0, 0, 0,		/* Tile_kido, Bg_y, Pad_1A, Bg_cnt */
	{ 0x00000000, { 0x00000000, 0x00000000 } },		/* Moji0 */
	{ 0x00000000, { 0x00000000, 0x00000000 } }		/* Moji1 */
};

/* One 0x40-byte prim cell of the Title prim buffer (Prim_work_buf). Spr[] at +0x00/+0x14 (SPRT stride 0x14 with Cbuf_id), Dr[] at +0x28/+0x34 (DR_MODE stride 0xc with Cbuf_id). 0x28 + 0x18 = 0x40, the cell stride used by Moji_set_work. */
typedef struct {
	SPRT Spr[2];
	DR_MODE Dr[2];
} TITLE_PRIM;

/* DS 0x80197038 (Title_prim_buf, now a TITLE_PRIM pointer) */
TITLE_PRIM *Title_prim = 0;

/* DS 0x8019703c (Title_fade_val) */
USHORT Title_fade_val = 0x0000;

/* DS 0x8019703e (Title_fade_add) */
USHORT Title_fade_add = 0x0000;

/* DS 0x80197040 (Title_fade_val2) */
USHORT Title_fade_val2 = 0x0000;

/* DS 0x80197042 (Ti_ex_rtn) */
UCHAR Ti_ex_rtn = 0x00;

/* DS 0x80197043 (Ti_ex_cur) */
UCHAR Ti_ex_cur = 0x00;

// --- Ex overlay sub-routines (Title_ex_main rtn table) ---

static VOID Ex_fade(VOID);
static VOID Ex_rtn1(VOID);
static VOID Ex_pl_sel(VOID);
static VOID Ex_mode_sel(VOID);
static VOID Ex_mode_wait(VOID);

/* Retail Title_ex_tbl at 0x80196a7c, 4 entries (TITLE.BIN offset 0x6a7c):
   [0] 0x80190300 Ex_fade, [1] 0x801903fc Ex_rtn1, [2] 0x80190438 Ex_pl_sel, [3] 0x80190b84 Ex_mode_wait */
static VOID * Title_ex_tbl[4] = { (VOID *)Ex_fade, (VOID *)Ex_rtn1, (VOID *)Ex_pl_sel, (VOID *)Ex_mode_wait };

// --- forward declarations for static helpers ---
static VOID Title_moji_disp(ULONG Cur, ULONG Pass_mask, LONG Disp_dist, LONG Disp_dir);
static VOID Title_mode_disp(LONG Mode, LONG Sel_no);
static LONG Title_fade_wait(LONG Mirror);
static VOID Title_bg_init(VOID);
static VOID Title_game_mode_sel(VOID);
static VOID Title_logo_disp(UCHAR Kido_a, UCHAR Kido_b, LONG Semi_trans);
static VOID Title_bg_disp(VOID);
static VOID Title_moji_tile(VOID);
static VOID Moji_mode_disp(LONG Mode, ULONG Opt_a, ULONG Opt_b, LONG Row);
static VOID Moji_spy_disp(VOID);
static VOID Title_disp_flicker(VOID);

// --- Spy overlay sub-routines (Title_spy_main rtn table) ---
static VOID Spy_sub0(VOID);
static VOID Spy_sel(VOID);
static VOID Title_information(VOID);

/* Retail Title_spy_tbl at 0x80196e7c, 2 entries: Spy_sub0 (0x801934bc), Spy_sel (0x801936bc).
   0x80196e84 is Info_tile, not a table entry. */
static VOID * Title_spy_tbl[2] = { (VOID *)Spy_sub0, (VOID *)Spy_sel };

// -------------------------------------------------------

/* DS 0x801900ec (Title_ex_init) */
VOID Title_ex_init(VOID)
{
	Main.Vloop = 0;
	G.System_flg |= SYS_EX_BATTLE;
	Snd_sys_core_set(20, 0);
	Cd_read(EXTITLE1_TIM, Tex_tim_buf, 0, (UCHAR *)"Extitle1");
	G.Ctpage = 11;
	G.Cclut = 28;
	Mapping_tim(Tex_tim_buf);
	Main.Scrl_type = 1;
	Fade_set(0x203, 0, 7, 0);
	Fade_adjust(3, 0x7c00, 0, (RECT *)0x0);
	Cd_read(EXTITLE_TIM, Bg_img_buf, 0, (UCHAR *)"Extitle");
	Moji_set_work();
	Ti.Kido = 0xf8;
	Ti.Disp_add = 2;
	Ti.Bg_y = 0xf0;
	Title_fade_val = 0;
	Title_fade_add = 0;
	Title_fade_val2 = 0;
	Ti.Cur = 0;
	Ti.Add = 0;
	Ti.Disp_cnt = 0;
	Ti.Sub_rtn0 = 0;
	Ti.Sub_rtn1 = 0;
	Ti.Sub_rtn2 = 0;
	Ti.Sub_rtn3 = 0;
	Ti.Moji_kido = 0;
	Ti.Moji_kido2 = 0;
	Ti.Tile_kido = 0;
	Ti.Bg_cnt = -1;
	Ti.Main_rtn++;
	G.SaveData.Dummy[1] = (USHORT)G.ExData[1];
	Bg_set_mode(0, 0);
	Fade_off(0);
	Fade_off(1);
	Fade_off(2);
	Fade_set(0x203, 0, 7, 0);
	Fade_adjust(3, 0x7c00, 0, (RECT *)0x0);
}

/* DS 0x801902c4 (Title_ex_main) */
VOID Title_ex_main(VOID)
{
	(*(VOID (*)(VOID))Title_ex_tbl[Ti.Sub_rtn0])();
}

/* DS 0x80190300 (Ex_fade) */
static VOID Ex_fade(VOID)
{
	TITLE_WORK *ti = &Ti;

	Fade_adjust(3, (LONG)ti->Kido << 7, 0, (RECT *)0x0);
	if ((G.Padd1_trg & 0x9ff) != 0) {
		ti->Timer = 0x20;
		ti->Sub_rtn1 = 1;
	}
	if (ti->Sub_rtn1 == 0) {
		ti->Kido = ti->Kido + -4;
	} else {
		ti->Kido = ti->Kido + -0x20;
	}
	if (ti->Kido < 1) {
		if (ti->Sub_rtn1 == 0) {
			ti->Timer = 0x20;
		}
		ti->Kido = 0;
		ti->Sub_rtn1 = 0;
		ti->Sub_rtn0++;
		Fade_off(3);
	}
	Title_moji_disp(0, (ULONG)ti->Sub_rtn1, 0, 0);
}

/* DS 0x801903fc (Ex_rtn1) */
static VOID Ex_rtn1(VOID)
{
	(*(VOID (*)(VOID))(&Title_ex_tbl[2])[Ti.Sub_rtn1])();
}

/* DS 0x80190438 (Ex_pl_sel) */
static VOID Ex_pl_sel(VOID)
{
	if (Ti.Sub_rtn2 != 1) {
		if (1 < Ti.Sub_rtn2) {
			if (Ti.Sub_rtn2 == 2) {
				Ex_mode_sel();
			}
			return;
		}
		if (Ti.Sub_rtn2 == 0) {
			if (Ti.Timer == 0) {
				Ti.Sub_rtn2 = 1;
			}
			goto moji_disp;
		}
		return;
	}
	if ((int)G.System_flg < 0) {
		if (((G.Padd1 & 0x1000) != 0) && (Ti.Cur != 0)) {
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
			Ti.Cur += 0xff;
		}
		if (((G.Padd1 & 0x4000) != 0) && (Ti.Cur < 3)) {
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
			Ti.Cur++;
		}
	}
	if (((G.Key_trg & 0x1000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
		if (Ti.Cur == 2) {
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
		} else if (Ti.Cur < 3) {
			if (Ti.Cur == 1) {
				if (G.ExData[1] == 0) {
					Ti_ex_cur = 0;
					Ti.Sub_rtn1 = 1;
					Ti.Sub_rtn2 = 0;
					Pad_vib_set0(7, 0);
					Pad_vib_set1(9, 0xff, 0);
					Pad_vib_fade_set(4, 0xff, 0x3c, 10);
				} else {
					Snd_se_stad(0x4060000, (VECTOR *)0x0);
					Ti.Sub_rtn2 = 2;
				}
				goto disp_flicker;
			}
		} else if (Ti.Cur == 3) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			goto enter_sub_state;
		}
		Pad_vib_set0(7, 0);
		Pad_vib_set1(9, 0xff, 0);
		Pad_vib_fade_set(4, 0xff, 0x3c, 10);
enter_sub_state:
		Ti.Sub_rtn1 = 1;
		Ti.Sub_rtn2 = 0;
	}
disp_flicker:
	Title_disp_flicker();
moji_disp:
	Title_moji_disp((ULONG)Ti.Cur, 3, 0, 0);
}

/* DS 0x801906d8 (Ex_mode_sel) */
static VOID Ex_mode_sel(VOID)
{
	register LONG i;
	register LONG j;
	TITLE_WORK *ti = &Ti;

	switch (ti->Sub_rtn3) {
	case 0:
		ti->Moji_kido = 0x80;
		Ti_ex_rtn = 0;
		ti->Moji_move_cnt = 0;
		ti->Moji_kido2 = 0;
		ti->Disp_cnt = 0;
		ti->Disp_add = 2;
		ti->Cur = 0;
		Ti_ex_cur = 0;
		Ex_mode_sel_flg = 0;
		ti->Sub_rtn3 = 1;
		if (G.ExData[1] < 2) {
			for (i = 0; i < 2; i++) {
				for (j = 0; j < 2; j++) {
					Title_prim[54 + i].Spr[j].x0 = 0x78;
					Title_prim[56 + i].Spr[j].x0 = 0xa8;
					Title_prim[58 + i].Spr[j].x0 = 0x140;
				}
			}
		} else {
			for (i = 0; i < 2; i++) {
				for (j = 0; j < 2; j++) {
					Title_prim[54 + i].Spr[j].x0 = 0x60;
					Title_prim[56 + i].Spr[j].x0 = 0x90;
					Title_prim[58 + i].Spr[j].x0 = 0xc0;
				}
			}
		}
		/* fall through */
	case 1:
		ti->Moji_move_cnt++;
		ti->Moji_kido += 0xf0;
		ti->Moji_kido2 += 16;
		if (ti->Moji_move_cnt == 8) {
			ti->Moji_kido = 0;
			ti->Moji_kido2 = 0x80;
			ti->Sub_rtn3++;
		}
		break;
	case 2:
		if ((ti->Cur == 0) && ((int)G.System_flg < 0)) {
			if (((G.Padd1 & 0x8000) != 0) && (Ti_ex_cur != 0)) {
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
				Ti_ex_cur--;
			}
			if (((G.Padd1 & 0x2000) != 0) && (Ti_ex_cur < G.ExData[1])) {
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
				Ti_ex_cur++;
			}
		}
		if (((G.Padd1_trg & 0x1000) != 0) && (ti->Cur != 0)) {
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
			ti->Cur += 0xff;
		}
		if (((G.Padd1_trg & 0x4000) != 0) && (ti->Cur == 0)) {
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
			ti->Cur++;
		}
		if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
			if ((G.Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				ti->Sub_rtn3 = 3;
			}
			Title_disp_flicker();
		} else if (ti->Cur == 0) {
			ti->Sub_rtn3 = 4;
			Pad_vib_set0(7, 0);
			Pad_vib_set1(9, 0xff, 0);
			Pad_vib_fade_set(4, 0xff, 0x3c, 10);
		} else {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			ti->Sub_rtn3 = 3;
		}
		break;
	case 3:
		Ex_mode_sel_flg = 1;
		ti->Disp_add = 2;
		ti->Disp_cnt = 0;
		ti->Moji_move_cnt += 0xff;
		ti->Moji_kido += 16;
		ti->Moji_kido2 += 0xf0;
		if (ti->Moji_move_cnt == 0) {
			ti->Moji_kido = 0x80;
			ti->Moji_kido2 = 0;
			ti->Cur = 1;
			ti->Sub_rtn2 = 1;
			ti->Sub_rtn3 = 0;
		}
		break;
	case 4:
		if (Title_fade_wait(0) == 0) break;
		ti->Sub_rtn3++;
		break;
	case 5:
		ti->Main_rtn = 2;
		ti->Sub_rtn0 = 3;
		G.System_flg |= SYS_NEW_GAME;
		break;
	}
	Title_mode_disp((LONG)ti->Cur, (LONG)Ti_ex_cur);
	Title_moji_disp(1, 3, 0x18, Ex_mode_sel_flg);
}

/* DS 0x80190b84 (Ex_mode_wait) */
static VOID Ex_mode_wait(VOID)
{
	LONG fade_done;

	if (Ti.Sub_rtn2 != 1) {
		if (1 < Ti.Sub_rtn2) {
			if (Ti.Sub_rtn2 == 2) {
				if (Ti.Cur == 1) {
					Ti.Main_rtn = 2;
					Ti.Sub_rtn0 = 3;
					G.System_flg |= SYS_NEW_GAME;
				} else if (Ti.Cur < 2) {
					if (Ti.Cur == 0) {
						Ti.Main_rtn = 2;
						Ti.Sub_rtn0 = 2;
					}
				} else if (Ti.Cur == 2) {
					Ti.Main_rtn = 7;
				} else if (Ti.Cur == 3) {
					G.SaveData.Status_bak = 0;
					G.Status_flg = 0;
					Memclr(&G.Status_flg, 0x1419);
					G.System_flg &= ~SYS_EX_BATTLE;
					Snd_sys_core_set(17, 0);
					Title_bg_reload();
					Ti.Main_rtn = 1;
				}
			}
			goto moji_disp;
		}
		if (Ti.Sub_rtn2 != 0) goto moji_disp;
		Ti_ex_rtn = 0;
		Ti.Sub_rtn2++;
	}
	fade_done = Title_fade_wait((ULONG)(1 < Ti.Cur));
	if (fade_done != 0) {
		Ti.Sub_rtn2++;
	}
moji_disp:
	Title_moji_disp((ULONG)Ti.Cur, 3, 0, 0);
}

/* DS 0x80190d34 (Title_moji_disp) */
static VOID Title_moji_disp(ULONG Cur, ULONG Pass_mask, LONG Disp_dist, LONG Disp_dir)
{
	register LONG i;
	// display helper for moji (text sprite) overlay
	// sets Y positions, brightness, and queues primitives for each moji entry
	// Pass_mask selects which display passes are active (bit flags)
	// Disp_dist = displacement magnitude, Disp_dir = direction flag

	if ((Pass_mask & 1) != 0) {
		USHORT timer_dec = Ti.Timer - 1;
		if (Ti.Timer == 0) {
			// fade active moji brightness (only while flag at +0xd04 is set)
			if (Title_prim[52].Spr[G.Cbuf_id].r0 != 0) {
				for (i = 0; i < 2; i++) {
					SPRT *base = &Title_prim[52 + i].Spr[G.Cbuf_id];
					SPRT *base2 = &Title_prim[52 + i].Spr[G.Cbuf_id ^ 1];
					base->r0 = base->r0 + -8;
					base->b0 = base->r0;
					base->g0 = base->r0;
					base2->r0 = base2->r0 + -8;
				}
			}
		} else {
			LONG disp_fp = (LONG)timer_dec * (LONG)timer_dec * 0x80;
			SHORT disp_ofs;
			if (disp_fp < 0) disp_fp = disp_fp + 0x3ff;
			disp_ofs = (SHORT)(disp_fp >> 10);
			Ti.Timer = timer_dec;
			Title_prim[50].Spr[G.Cbuf_id].x0 = -disp_ofs;
			Title_prim[51].Spr[G.Cbuf_id].x0 = 0xbc - disp_ofs;
			Title_prim[52].Spr[G.Cbuf_id].x0 = disp_ofs;
			Title_prim[53].Spr[G.Cbuf_id].x0 = disp_ofs + 0xbc;
			if (Ti.Timer == 0) {
				// on the final tick, copy the +8 word of each entry from the other buffer
				for (i = 0; i < 4; i++) {
					Title_prim[50 + i].Spr[G.Cbuf_id].x0 = Title_prim[50 + i].Spr[G.Cbuf_id ^ 1].x0;
				}
			}
			for (i = 0; i < 4; i++) {
				Title_prim[50 + i].Spr[G.Cbuf_id].b0 = 0x80;
				Title_prim[50 + i].Spr[G.Cbuf_id].g0 = 0x80;
				Title_prim[50 + i].Spr[G.Cbuf_id].r0 = 0x80;
			}
		}
		AddPrim(G.pOt1 + 4, (VOID *)&Title_prim[50].Spr[G.Cbuf_id]);
		AddPrim(G.pOt1 + 4, (VOID *)&Title_prim[51].Spr[G.Cbuf_id]);
		AddPrim(G.pOt1 + 4, (VOID *)&Title_prim[52].Spr[G.Cbuf_id]);
		AddPrim(G.pOt1 + 4, (VOID *)&Title_prim[53].Spr[G.Cbuf_id]);
		AddPrim(G.pOt1 + 4, (VOID *)&Title_prim[50].Dr[G.Cbuf_id]);
	}
	if ((Ti.Timer == 0) && ((Pass_mask & 2) != 0)) {
		// draw moji selection sprites (4 rows x 2 entries), inline in the binary
		ULONG move_pos;
		ULONG row;
		ULONG col;

		if (Disp_dir == 0) {
			move_pos = 8 - Ti.Moji_move_cnt;
		} else {
			move_pos = Ti.Moji_move_cnt;
		}
		for (row = 0; row < 4; row++) {
			LONG row_base = row * 2;
			SHORT disp_ofs = (SHORT)(((ULONG)Disp_dist * move_pos * move_pos * move_pos * move_pos) >> 12);
			for (col = 0; col < 2; col++) {
				LONG idx = row_base + 0x2a + col;
				SPRT *p = &Title_prim[idx].Spr[G.Cbuf_id];
				LONG ent_idx = row_base + col;
				UCHAR kido = Ti.Moji_kido;

				// y position comes from Moji_work_tbl[idx].y (T_MOJI_TBL stride 14, base 80196ADC)
				p->y0 = Moji_work_tbl[idx].y;
				p->b0 = 0x80;
				p->g0 = 0x80;
				p->r0 = 0x80;
				if (Ti.Moji_move_cnt != 0) {
					if (row == Cur) {
						if (Disp_dir == 0) {
							p->y0 = p->y0 - ((SHORT)Disp_dist - disp_ofs);
						} else {
							p->y0 -= disp_ofs;
						}
					} else {
						p->b0 = kido;
						p->g0 = kido;
						p->r0 = kido;
					}
				}
				if (row == Cur) {
					if (Ti.Moji_move_cnt == 8) {
						/* cell (ent_idx + 42): col 0 of row r is cell 2r, the same cell as ent_idx */
						SPRT *q = &Title_prim[ent_idx + 42].Spr[G.Cbuf_id];
						q->clut = 0x7f80;
					} else {
						CHAR c;
						Title_prim[ent_idx + 42].Spr[G.Cbuf_id].clut = 0x7f80;
						c = (CHAR)(Ti.Disp_cnt + 0x80);
						p->b0 = c;
						p->g0 = c;
						p->r0 = c;
					}
				} else {
					SPRT *q = &Title_prim[ent_idx + 42].Spr[G.Cbuf_id];
					q->clut = 0x7f8c;
				}
				AddPrim(G.pOt1 + 1, (VOID *)p);
				AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[idx].Dr[G.Cbuf_id]);
			}
		}
	}
}

/* DS 0x80191400 (Title_mode_disp) */
static VOID Title_mode_disp(LONG Mode, LONG Sel_no)
{
	register ULONG i;
	register ULONG j;
	UCHAR kido2;
	CHAR disp_col;
	SPRT *pPrim;

	for (j = 0; j < 4; j++) {
		for (i = 0; i < 2; i++) {
			kido2 = Ti.Moji_kido2;
			pPrim = &Title_prim[j * 2 + i + 54].Spr[G.Cbuf_id];
			pPrim->b0 = Ti.Moji_kido2;
			pPrim->g0 = kido2;
			pPrim->r0 = kido2;
			Title_prim[j * 2 + i + 54].Spr[G.Cbuf_id].clut = 0x7f8c;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[j * 2 + i + 54].Spr[G.Cbuf_id]);
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[j * 2 + i + 54].Dr[G.Cbuf_id]);
		}
	}
	if (Mode == 0) {
		Title_prim[Sel_no * 2 + 54].Spr[G.Cbuf_id].clut = 0x7f80;
		pPrim = &Title_prim[Sel_no * 2 + 54].Spr[G.Cbuf_id];
	} else {
		Title_prim[60].Spr[G.Cbuf_id].clut = 0x7f80;
		pPrim = &Title_prim[60].Spr[G.Cbuf_id];
	}
	disp_col = Ti.Moji_kido2 + Ti.Disp_cnt;
	pPrim->b0 = disp_col;
	pPrim->g0 = disp_col;
	pPrim->r0 = disp_col;
}

/* DS 0x80191628 (Ex_battle_result) */
VOID Ex_battle_result(VOID)
{
	Task_lexecute(2, 7);
	Task_sleep(1);
	Ti.Main_rtn = 5;
	Ti.Tile_kido = 0;
	Ti.Bg_cnt = 0;
}

/* DS 0x80191670 (Title_fade_wait) */
static LONG Title_fade_wait(LONG Mirror)
{
	ULONG fade_sts;

	if (Mirror == 0) {
		if (Ti_ex_rtn != 1) {
			if (1 < Ti_ex_rtn) {
				if (Ti_ex_rtn == 2) {
					Fade_set(0x200, 0x80, 7, 1);
					fade_sts = 0x1000;
					goto fade_begin;
				}
				if (Ti_ex_rtn != 3) return 0;
				goto fade_check;
			}
			if (Ti_ex_rtn != 0) return 0;
			Snd_se_stad(0x4000000, (VECTOR *)0x0);
			Fade_set(0x100, -0x800, 7, 1);
			Ti_ex_rtn++;
		}
		fade_sts = Fade_status(0);
		if (fade_sts == 0) return 0;
		Ti_ex_rtn++;
	} else {
		if (Ti_ex_rtn == 0) {
			Fade_set(0x200, 0x1800, 7, 1);
			fade_sts = 0x200;
fade_begin:
			Fade_adjust(0, fade_sts, 0x101010, (RECT *)0x0);
			Ti_ex_rtn++;
		} else if (Ti_ex_rtn != 1) {
			return 0;
		}
fade_check:
		fade_sts = Fade_status(0);
		if (fade_sts != 0) {
			Fade_set(0x200, 0, 7, 1);
			Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0x0);
			return 1;
		}
	}
	return 0;
}

/* DS 0x80191808 (Title) */
VOID Title(VOID)
{
	/* Retail Title_rtn_tbl at 0x80196e40, 8 entries (TITLE.BIN offset 0x6e40), indexed by Ti.Main_rtn */
	static VOID * Title_rtn_tbl[8] = { (VOID *)Capcom_logo, (VOID *)Title_init, (VOID *)Title_main,
		(VOID *)Title_spy_init, (VOID *)Title_spy_main, (VOID *)Title_ex_init, (VOID *)Title_ex_main,
		(VOID *)Ex_battle_result };

	ULONG read_size;

	Pad_rep_set(0xf000, 0x60a);
	Ti.Main_rtn = 0;
	Title_prim = (TITLE_PRIM *)Prim_work_buf;
	Moji_set_work();
	if (G.ExData[0] != 0) {
		Ti.Main_rtn = 7;
		Snd_sys_core_set(17, 0);
		G.pFree_work = (UCHAR *)&Ex_room_snd_buf;
		do {
			read_size = Cd_read(ROOM800_SND, (ULONG *)G.pFree_work, 1, (UCHAR *)"ROOM800.SND");
		} while (read_size == 0xffffffff);
		Ex_room_cnt = 0;
		Ex_room_snd_a = ((ULONG)&Ex_room_snd_buf + 0xc0);
		Ex_room_snd_hdr = (ULONG)&Ex_room_snd_buf;
		Ex_room_snd_b = ((ULONG)&Ex_room_snd_buf + 0xce0);
		G.pRoom = (ROOM_WORK *)&pRoom_ex;
		G.pFree_work += read_size;
		Snd_sys_room_set();
		Title_bg_load();
	}
	do {
		(*(VOID (*)(VOID))Title_rtn_tbl[Ti.Main_rtn])();
		Task_sleep(1);
	} while (1);
}

/* DS 0x80191958 (Capcom_logo) */
VOID Capcom_logo(VOID)
{
	tagMAIN *m = &Main;

	Snd_sys_init2();
	Snd_sys_core_set(17, 0);
	m->Padoff_stop_flg[0][1] = 2;
	Task_sleep(1);
	if ((G.System_flg & SYS_INTRO_PENDING) == 0) {
		Title_bg_load();
		Title_bg_reload();
	} else {
		Title_bg_load();
		while (m->Padoff_stop_flg[0][1] != 0) {
			Task_sleep(1);
		}
		Init_movie_work(0);
		while ((G.System_flg & SYS_STR_MOVIE) != 0) {
			Task_sleep(1);
		}
		Title_bg_reload();
		if (m->Padoff_stop_flg[1][1] == 2) {
			G.System_flg &= ~SYS_INTRO_PENDING;
		}
	}
	m->Scrl_type = 1;
	Ti.Main_rtn = 1;
	if ((G.System_flg & SYS_TIMER) != 0) {
		Ti.Main_rtn = 3;
	}
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		Ti.Main_rtn = 5;
	}
}

/* DS 0x80191aa8 (Title_bg_load) */
VOID Title_bg_load(VOID)
{
	Cd_read(TIT_BG_TIM, Title_bg_work_buf, 0, (UCHAR *)"Title_BG");
	Cd_read(TMOJI_TIM, Tex_tim_buf, 0, (UCHAR *)"Tmoji_BG");
	G.Ctpage = 8;
	G.Cclut = 30;
	Mapping_tim(Tex_tim_buf);
}

/* DS 0x80191b10 (Title_bg_reload) */
VOID Title_bg_reload(VOID)
{
	memcpy16(Bg_img_buf, Title_bg_work_buf, 0x25814);
}

/* DS 0x80191b44 (Title_init) */
VOID Title_init(VOID)
{
	register ULONG i;
	register ULONG j;
	TILE *pTile;

	SetGeomOffset(0xa0, 0x78);
	Ti.Bg_y = -0x2e;
	Ti.Bg_cnt = -1;
	Main.Scrl_type = 1;
	Main.Vloop = 0;
	Ti.Sel_mode = 0;
	Ti.Cur = 0;
	Ti.Disp_cnt = 0;
	Ti.Disp_add = 1;
	Ti.Sub_rtn0 = 0;
	Ti.Sub_rtn1 = 0;
	Ti.Sub_rtn2 = 0;
	Ti.Sub_rtn3 = 0;
	Ti.Stereo_bak = 0;
	Ti.Timer = 0;
	Ti.Rookie_cnt = 0;
	Ti.Rookie_flg = 0;
	Ti.Tile_kido = 0;
	Title_fade_val = 0;
	Title_fade_add = 0x20;
	Title_fade_val2 = 0;
	if ((G.System_flg & SYS_INTRO_PENDING) == 0) {
		Bg_set_mode(0, 0);
		Title_bg_reload();
		Fade_set(0x200, 0, 7, 1);
		Fade_adjust(0, 0x7c00, 0, (RECT *)0x0);
		Ti.Add = -0x400;
		Ti.Kido = 0x7c00;
		Ti.Sub_rtn0 = 1;
	} else {
		G.System_flg &= ~SYS_INTRO_PENDING;
		Bg_set_mode(2, 0);
		Title_bg_reload();
		Fade_set(0x100, 0, 7, 1);
		Fade_adjust(0, 0x7f80, 0, (RECT *)0x0);
		Ti.Bg_y = 0xf0;
		Ti.Sub_rtn0 = 6;
	}
	for (i = 0; i < 2; i++) {
		pTile = Moji_tile_tbl;
		for (j = 0; j < 2; j++) {
			SetTile(pTile + i * 2);
			SetSemiTrans(pTile + i * 2, 1);
			pTile++;
		}
	}
	SetDrawMode((DR_MODE *)&Ti.Moji0, 0, 0, 200, (RECT *)0x0);
	SetDrawMode((DR_MODE *)&Ti.Moji1, 0, 0, 200, (RECT *)0x0);
	Moji_set_work();
	Ti.Main_rtn++;
	if ((G.Sound_mode & 0x20) == 0) {
		G.SaveData.Weapon_mugen &= 0x2001;
	} else {
		G.SaveData.Weapon_mugen |= 0xffffdf00;
	}
}

/* DS 0x80191df8 (Title_main) */
VOID Title_main(VOID)
{
	/* Retail Title_main_tbl at 0x80196e60, 7 entries (TITLE.BIN offset 0x6e60); entry [4] is 0x80192ac8 Title_game_mode_sel. */
	static VOID * Title_main_tbl[7] = { (VOID *)Title_main_wait, (VOID *)Title_main_select,
		(VOID *)Title_main_load, (VOID *)Title_main_game, (VOID *)Title_game_mode_sel,
		(VOID *)Title_main_option, (VOID *)Title_bg_init };

	(*(VOID (*)(VOID))Title_main_tbl[Ti.Sub_rtn0])();
	Title_bg_disp();
}

/* DS 0x80191e3c (Title_main_game) */
VOID Title_main_game(VOID)
{
	Task_chain(Game_loop);
}

/* DS 0x80191e64 (Title_main_wait) */
VOID Title_main_wait(VOID)
{
	ULONG fade_sts;
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;

	ti->Kido += ti->Add;
	Fade_set(0x200, 0, 7, 1);
	Fade_adjust(0, (LONG)ti->Kido, 0, (RECT *)0x0);
	fade_sts = Fade_status(0);
	if (fade_sts != 0) {
		ti->Add = 0;
		ti->Kido = -1;
	}
	if ((g->Padd1_trg & 0x9ff) == 0) {
		fade_sts = Fade_status(0);
		if (fade_sts == 0) goto fade_busy;
		if ((g->Padd1_trg & 0x9ff) != 0) goto skip_fade;
	} else {
skip_fade:
		Title_fade_val2 = 0x80;
		Title_fade_val = 0x80;
		Title_fade_add = 0;
		if ((g->Padd1_trg & 0x9ff) != 0) {
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
		}
	}
	ti->Add = -0x1000;
	ti->Timer = 0;
	ti->Sub_rtn0 = 1;
	ti->Sub_rtn1 = 0;
	ti->Sub_rtn2 = 0;
fade_busy:
	Moji_License();
}

/* DS 0x80191f94 (Title_bg_init) */
static VOID Title_bg_init(VOID)
{
	TITLE_WORK *ti = &Ti;

	if (ti->Sub_rtn1 != 1) {
		if (1 < ti->Sub_rtn1) {
			if (ti->Sub_rtn1 == 2) {
				ti->Sub_rtn0 = 1;
				ti->Sub_rtn1 = 0;
			}
			goto moji_tile;
		}
		if (ti->Sub_rtn1 != 0) goto moji_tile;
		Bg_set_mode(0, 0);
		ti->Tile_kido = 0x36;
		ti->Bg_cnt = 0x16;
		ti->Bg_y = 0;
	}
	ti->Sub_rtn0 = 1;
	ti->Sub_rtn1 = 0;
moji_tile:
	Title_moji_tile();
}

/* DS 0x80192058 (Title_main_select) */
VOID Title_main_select(VOID)
{
	ULONG fade_sts;
	ULONG Kido;
	TITLE_WORK *ti = &Ti;

	ti->Kido += ti->Add;
	Fade_set(0x200, 0, 7, 1);
	Kido = (ULONG)ti->Kido;
	Fade_adjust(0, Kido, 0, (RECT *)0x0);
	fade_sts = Fade_status(0);
	if (fade_sts != 0) {
		ti->Add = 0;
		ti->Kido = -1;
	}
	ti->Timer++;
	if (ti->Timer < 0x708) {
		if (G.Padd1_trg != 0) {
			Title_fade_val2 = 0x80;
			Title_fade_val = 0x80;
			Title_fade_add = 0;
			ti->Timer = 0;
		}
		switch (ti->Sub_rtn1) {
		case 0:
			Title_sel_menu();
			break;
		case 1:
			Title_sel_mode();
			break;
		case 2:
			Title_sel_sound();
			break;
		case 3:
			ti->Sub_rtn0 = ti->Sel_mode + 2;
			if (ti->Sub_rtn0 == 5) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Fade_set(0x200, 0x1800, 7, 1);
				Fade_adjust(0, 0x200, 0x101010, (RECT *)0x0);
				while (1) {
					fade_sts = Fade_status(0);
					if (fade_sts != 0) break;
					Title_bg_disp();
					Moji_Sel();
					Task_sleep(1);
				}
			} else {
				Snd_se_stad(0x4000000, (VECTOR *)0x0);
				Fade_set(0x100, -0x800, 7, 1);
				while (fade_sts = Fade_status(0), fade_sts == 0) {
					Moji_Sel();
					Title_bg_disp();
					Task_sleep(1);
				}
				Fade_set(0x200, 0x80, 7, 1);
				Fade_adjust(0, 0x200, 0x101010, (RECT *)0x0);
				while (fade_sts = Fade_status(0), fade_sts == 0) {
					Moji_Sel();
					Title_bg_disp();
					Task_sleep(1);
				}
			}
			Fade_set(0x200, 0, 7, 1);
			Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0x0);
			Task_sleep(1);
			break;
		case 4:
			Title_game_mode_sel();
			break;
		}
	} else {
		if (ti->Timer == 0x708) {
			ti->Add = 0x100;
			ti->Kido = 0;
		} else {
			fade_sts = Fade_status(0);
			if (fade_sts != 0) {
				Fade_set(0x200, 0, 7, 1);
				Fade_adjust(0, 0x7fff, 0, (RECT *)0x0);
				G.System_flg |= SYS_DEMO_MODE;
				Cd_read(G.Dummy8[2] + PDEMO00_DAT, (ULONG *)&Demo, 0, (UCHAR *)"PDEM.DAT");
				G.Dummy8[2]++;
				if (2 < G.Dummy8[2]) {
					G.Dummy8[2] = 0;
				}
				ti->Sub_rtn0 = 3;
			}
		}
		if ((G.Padd1_trg & 0x9ff) != 0) {
			Snd_se_stad(0x4050000, (VECTOR *)0x0);
			ti->Timer = 0;
			ti->Add = -0x1000;
			ti->Sub_rtn2 = 0;
		}
	}
	Moji_License();
}

/* DS 0x80192410 (Moji_Sel) */
VOID Moji_Sel(VOID)
{
	ULONG Mode;
	ULONG Dis;

	Moji_License();
	if (((G.System_flg & SYS_ARRANGE) == 0) && ((G.System_flg & 0x840) != 0x840)) {
		Dis = 0;
		if (Ti.Sel_mode == 1) {
			Moji_mode_disp((LONG)Ti.Cur, 0, 0, 0);
			Mode = 1;
			Dis = 0x14;
		} else {
			Mode = (ULONG)Ti.Cur;
		}
	} else {
		Moji_Game_mode((ULONG)Ti.Cur);
		Mode = 2;
		Dis = 0x21;
	}
	Moji_SelectMode(Mode, Mode, Dis, 0);
}

/* DS 0x801924c8 (Title_sel_menu) */
VOID Title_sel_menu(VOID)
{
	if (Ti.Sub_rtn2 == 0) {
		Ti.Disp_add = 2;
		Ti.Moji_move_cnt = 0;
		Ti.Disp_cnt = 0;
		Ti.Sub_rtn2 = 1;
	} else if (Ti.Sub_rtn2 != 1) {
		return;
	}
	if ((int)G.System_flg < 0) {
		if (((G.Padd1 & 0x4000) != 0) && (Ti.Cur < 3)) {
			Ti.Cur++;
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
		}
		if (((G.Padd1 & 0x1000) != 0) && (Ti.Cur != 0)) {
			Ti.Cur += 0xff;
			Snd_se_stad(0x4040000, (VECTOR *)0x0);
		}
	}
	Title_disp_flicker();
	Moji_SelectMode((ULONG)Ti.Cur, 0, 0, 0);
	if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
		return;
	}
	if (Ti.Cur == 1) {
		Snd_se_stad(0x4060000, (VECTOR *)0x0);
		Ti.Sub_rtn1 = 1;
	} else if (Ti.Cur < 2) {
		if (Ti.Cur == 0) {
			Ti.Sel_mode = 0;
			Ti.Sub_rtn1 = 3;
			Ti.Sub_rtn2 = 0;
			Pad_vib_set0(7, 0);
			Pad_vib_set1(9, 0xff, 0);
			Pad_vib_fade_set(4, 0xff, 0x3c, 10);
			return;
		}
	} else if (Ti.Cur == 2) {
		Snd_se_stad(0x4060000, (VECTOR *)0x0);
		Ti.Sel_mode = 2;
		Ti.Sub_rtn1 = 4;
	} else if (Ti.Cur == 3) {
		Ti.Sub_rtn1 = 3;
		Ti.Sub_rtn2 = 0;
		Ti.Sel_mode = 3;
		return;
	}
	Ti.Rookie_cnt = 0;
	Ti.Cur = 0;
	Ti.Sub_rtn2 = 0;
}

/* DS 0x801926e0 (Title_sel_mode) */
VOID Title_sel_mode(VOID)
{
	ULONG sys_flg;
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;

	if (ti->Sub_rtn2 != 1) {
		if (1 < ti->Sub_rtn2) {
			if (ti->Sub_rtn2 != 2) {
				if (ti->Sub_rtn2 == 3) {
					Sel_mode_flg = 1;
					ti->Disp_cnt = 0;
					ti->Moji_move_cnt += 0xff;
					ti->Moji_kido += 16;
					ti->Moji_kido2 += 0xf0;
					if (ti->Moji_move_cnt == 0) {
						ti->Moji_kido = 0x80;
						ti->Moji_kido2 = 0;
						ti->Cur = 1;
						ti->Sub_rtn1 = 0;
						ti->Sub_rtn2 = 0;
						ti->Sub_rtn3 = 0;
					}
				}
				goto mode_disp;
			}
			if ((int)g->System_flg < 0) {
				if (((g->Padd1 & 0x4000) != 0) && (ti->Cur == 0)) {
					ti->Cur++;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
				if ((g->Padd1 & 0x1000) == 0) goto cur_check;
				if (ti->Cur != 0) {
					ti->Cur += 0xff;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
					goto cur_check;
				}
rookie_toggle:
				if (((g->Padd1_trg & 0x8000) != 0) && (ti->Rookie_flg == 1)) {
					ti->Rookie_flg = 0;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
				if (((g->Padd1_trg & 0x2000) != 0) && (ti->Rookie_flg == 0)) {
					ti->Rookie_flg = 1;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
			} else {
cur_check:
				if (ti->Cur == 0) goto rookie_toggle;
			}
			Title_disp_flicker();
			if ((g->Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				ti->Sub_rtn2 = 3;
				ti->Rookie_flg = (UCHAR)Rookie_flg_bak;
			}
			if (((g->Key_trg & 0x1000) != 0) || ((g->Padd_trg & 0x800) != 0)) {
				if (ti->Cur == 0) {
					sys_flg = g->System_flg | SYS_NEW_GAME | SYS_EASY_MODE;
					g->System_flg |= SYS_NEW_GAME;
					if (ti->Rookie_flg == 1) {
						g->System_flg = sys_flg;
					}
					ti->Sel_mode = 1;
					ti->Sub_rtn1 = 3;
					Pad_vib_set0(7, 0);
					Pad_vib_set1(9, 0xff, 0);
					Pad_vib_fade_set(4, 0xff, 0x3c, 10);
				} else if (ti->Cur == 1) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					ti->Sub_rtn2 = 3;
					ti->Rookie_flg = 0;
				}
			}
			goto mode_disp;
		}
		if (ti->Sub_rtn2 != 0) goto mode_disp;
		Rookie_flg_bak = (ULONG)ti->Rookie_flg;
		ti->Moji_kido = 0x80;
		ti->Disp_add = 2;
		ti->Moji_move_cnt = 0;
		ti->Moji_kido2 = 0;
		ti->Disp_cnt = 0;
		Sel_mode_flg = 0;
		ti->Sub_rtn2 = 1;
	}
	ti->Moji_move_cnt++;
	ti->Moji_kido += 0xf0;
	ti->Moji_kido2 += 16;
	if (ti->Moji_move_cnt == 8) {
		ti->Moji_kido = 0;
		ti->Moji_kido2 = 0x80;
		ti->Sub_rtn2++;
	}
mode_disp:
	Moji_mode_disp((LONG)ti->Cur, 0, 0, 0);
	Moji_SelectMode(1, 1, 0x14, Sel_mode_flg);
}

/* DS 0x80192ac8 (Title_game_mode_sel) */
static VOID Title_game_mode_sel(VOID)
{
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;

	if (ti->Sub_rtn2 != 1) {
		if (1 < ti->Sub_rtn2) {
			if (ti->Sub_rtn2 == 2) {
				if ((int)g->System_flg < 0) {
					if (((g->Padd1 & 0x4000) != 0) && (ti->Cur == 0)) {
						ti->Cur++;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
					}
					if (((g->Padd1 & 0x1000) != 0) && (ti->Cur != 0)) {
						ti->Cur += 0xff;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
					}
				}
				Title_disp_flicker();
				if ((g->Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					ti->Sub_rtn2 = 3;
				}
				if (((g->Key_trg & 0x1000) != 0) || ((g->Padd_trg & 0x800) != 0)) {
					if (ti->Cur == 0) {
						ti->Sel_mode = 1;
						ti->Sub_rtn1 = 3;
						g->System_flg |= SYS_ARRANGE | SYS_NEW_GAME | SYS_EASY_MODE;
						Pad_vib_set0(7, 0);
						Pad_vib_set1(9, 0xff, 0);
						Pad_vib_fade_set(4, 0xff, 0x3c, 10);
					} else {
						Snd_se_stad(0x4050000, (VECTOR *)0x0);
						ti->Sub_rtn2 = 3;
					}
				}
			} else if (ti->Sub_rtn2 == 3) {
				Game_mode_flg = 1;
				ti->Disp_cnt = 0;
				ti->Moji_move_cnt += 0xff;
				ti->Moji_kido += 16;
				ti->Moji_kido2 += 0xf0;
				if (ti->Moji_move_cnt == 0) {
					ti->Moji_kido = 0x80;
					ti->Moji_kido2 = 0;
					ti->Cur = 2;
					ti->Sub_rtn1 = 0;
					ti->Sub_rtn2 = 0;
					ti->Sub_rtn3 = 0;
				}
			}
			goto game_mode_disp;
		}
		if (ti->Sub_rtn2 != 0) goto game_mode_disp;
		ti->Moji_kido = 0x80;
		ti->Disp_add = 2;
		ti->Moji_move_cnt = 0;
		ti->Moji_kido2 = 0;
		ti->Disp_cnt = 0;
		Game_mode_flg = 0;
		ti->Sub_rtn2 = 1;
	}
	ti->Moji_move_cnt++;
	ti->Moji_kido += 0xf0;
	ti->Moji_kido2 += 16;
	if (ti->Moji_move_cnt == 8) {
		ti->Moji_kido = 0;
		ti->Moji_kido2 = 0x80;
		ti->Sub_rtn2++;
	}
game_mode_disp:
	Moji_Game_mode((ULONG)ti->Cur);
	Moji_SelectMode(2, 2, 0x21, Game_mode_flg);
}

/* DS 0x80192df8 (Title_sel_sound) */
VOID Title_sel_sound(VOID)
{
	ULONG bits;
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;

	if (ti->Sub_rtn2 != 1) {
		if (1 < ti->Sub_rtn2) {
			if (ti->Sub_rtn2 != 2) {
				if (ti->Sub_rtn2 == 3) {
					Sound_sel_flg = 1;
					ti->Disp_cnt = 0;
					ti->Disp_add = 2;
					ti->Moji_move_cnt += 0xff;
					ti->Moji_kido += 16;
					ti->Moji_kido2 += 0xf0;
					if (ti->Moji_move_cnt == 0) {
						ti->Moji_kido = 0x80;
						ti->Moji_kido2 = 0;
						ti->Cur = 2;
						ti->Sub_rtn1 = 0;
						ti->Sub_rtn2 = 0;
					}
				}
				goto sound_disp;
			}
			if (((g->Padd1_trg & 0x4000) != 0) && (ti->Cur == 0)) {
				ti->Cur++;
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
			}
			if ((g->Padd1_trg & 0x1000) == 0) {
cur_check:
				if (ti->Cur == 0) goto sound_toggle;
			} else {
				if (ti->Cur != 0) {
					ti->Cur += 0xff;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
					goto cur_check;
				}
sound_toggle:
				if (((g->Padd1_trg & 0x8000) != 0) && ((g->Sound_mode & 1) != 0)) {
					g->Sound_mode &= 0xfe;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
				if (((g->Padd1_trg & 0x2000) != 0) && ((g->Sound_mode & 1) == 0)) {
					g->Sound_mode |= 1;
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
				}
			}
			Title_disp_flicker();
			if ((g->Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				ti->Sub_rtn2 = 3;
				g->Sound_mode = ti->Stereo_bak;
			}
			if (((g->Key_trg & 0x1000) != 0) || ((g->Padd_trg & 0x800) != 0)) {
				Snd_sys_stereo(g->Sound_mode);
				bits = 0x4050000;
				if (ti->Cur == 0) {
					bits = 0x4060000;
				}
				Snd_se_stad(bits, (VECTOR *)0x0);
				ti->Sub_rtn2 = 3;
			}
			goto sound_disp;
		}
		if (ti->Sub_rtn2 != 0) goto sound_disp;
		ti->Moji_kido = 0x80;
		ti->Disp_add = 2;
		ti->Moji_move_cnt = 0;
		ti->Moji_kido2 = 0;
		ti->Disp_cnt = 0;
		Sound_sel_flg = 0;
		ti->Sub_rtn2 = 1;
	}
	ti->Moji_move_cnt++;
	ti->Moji_kido += 0xf0;
	ti->Moji_kido2 += 16;
	if (ti->Moji_move_cnt == 8) {
		ti->Moji_kido = 0;
		ti->Moji_kido2 = 0x80;
		ti->Sub_rtn2++;
	}
sound_disp:
	Moji_Sound((ULONG)ti->Cur);
	Moji_SelectMode(2, 2, 0x2f, Sound_sel_flg);
}

/* DS 0x8019315c (Title_main_load) */
VOID Title_main_load(VOID)
{
	ULONG sys_flg;
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;
	tagMAIN *m = &Main;

	m->Scrl_type = 0;
	g->System_flg |= SYS_CARD_LOAD;
	Task_lexecute(1, 1);
	Task_sleep(1);
	m->Scrl_type = 1;
	if ((g->System_flg & SYS_CARD_LOAD) == 0) {
		g->System_flg &= ~SYS_NEW_GAME;
		ti->Sub_rtn0 = 3;
	} else {
		sys_flg = g->System_flg & ~SYS_CARD_LOAD;
		if ((g->System_flg & SYS_TIMER) == 0) {
			if ((g->System_flg & SYS_EX_BATTLE) == 0) {
				g->System_flg = sys_flg;
				Title_bg_reload();
				ti->Main_rtn = 1;
			} else {
				ti->Main_rtn = 5;
				g->System_flg = sys_flg;
			}
		} else {
			ti->Main_rtn = 3;
			g->System_flg = sys_flg;
		}
	}
}

/* DS 0x80193250 (Title_main_option) */
VOID Title_main_option(VOID)
{
	TITLE_WORK *ti = &Ti;
	tagGLOBAL *g = &G;

	g->System_flg |= SYS_CONFIG;
	Task_lexecute(1, 4);
	Task_sleep(1);
	Main.Scrl_type = 1;
	g->System_flg &= ~SYS_CONFIG;
	Title_bg_reload();
	ti->Main_rtn = 1;
}

// --- Title_spy (spy/tofu overlay) ---

/* DS 0x801932d0 (Title_main_information) */
VOID Title_main_information(VOID)
{
}

/* DS 0x801932d8 (Title_spy_init) */
VOID Title_spy_init(VOID)
{
	ULONG cd_sect;
	CHAR *pFname;

	Main.Vloop = 0;
	if ((G.System_flg & SYS_TOFU_SURVIVOR) == 0) {
		Snd_sys_core_set(18, 0);
		cd_sect = SPY_LOGO_TIM;
		pFname = "Spy_logo";
	} else {
		Snd_sys_core_set(19, 0);
		cd_sect = TOFULOGO_TIM;
		pFname = "Toufu_logo";
	}
	Cd_read(cd_sect, Tex_tim_buf, 0, (UCHAR *)pFname);
	G.Ctpage = 11;
	G.Cclut = 28;
	Mapping_tim(Tex_tim_buf);
	Main.Scrl_type = 1;
	Fade_set(0x203, 0, 7, 0);
	Fade_adjust(3, 0x7c00, 0, (RECT *)0x0);
	if ((G.System_flg & SYS_TOFU_SURVIVOR) == 0) {
		cd_sect = TITLE_O_TIM;
		pFname = "Title_o";
	} else {
		cd_sect = TITLE_O2_TIM;
		pFname = "Title_o2";
	}
	Cd_read(cd_sect, Bg_img_buf, 0, (UCHAR *)pFname);
	Moji_set_work();
	Ti.Disp_add = 2;
	Ti.Bg_y = 0xf0;
	Title_fade_val = 0;
	Title_fade_add = 0;
	Title_fade_val2 = 0;
	Ti.Cur = 0;
	Ti.Add = 0;
	Ti.Kido = 0;
	Ti.Disp_cnt = 0;
	Ti.Sub_rtn0 = 0;
	Ti.Sub_rtn1 = 0;
	Ti.Sub_rtn2 = 0;
	Ti.Sub_rtn3 = 0;
	Ti.Moji_kido = 0;
	Ti.Moji_kido2 = 0;
	Ti.Tile_kido = 0;
	Ti.Bg_cnt = -1;
	Ti.Main_rtn++;
	Bg_set_mode(0, 0);
	Fade_off(0);
}

/* DS 0x80193480 (Title_spy_main) */
VOID Title_spy_main(VOID)
{
	(*(VOID (*)(VOID))Title_spy_tbl[Ti.Sub_rtn0])();
}

/* DS 0x801934bc (Spy_sub0) */
static VOID Spy_sub0(VOID)
{
	ULONG fade_sts;

	Fade_adjust(3, 0x7c00, 0, (RECT *)0x0);
	if (Ti.Sub_rtn1 == 1) {
		Ti.Moji_kido2++;
		Title_logo_disp(Ti.Moji_kido2, 0x80, 1);
		if (Ti.Moji_kido2 < 0x80) goto pad_check;
		Ti.Moji_kido2 = 0x80;
		Ti.Add = -0x100;
		Ti.Kido = 0x5a00;
	} else if (1 < Ti.Sub_rtn1) {
		if (Ti.Sub_rtn1 == 2) {
			Title_logo_disp(0x80, 0x80, 1);
			Ti.Kido += Ti.Add;
			Fade_adjust(3, (LONG)Ti.Kido, 0, (RECT *)0x0);
			fade_sts = Fade_status(3);
			if (fade_sts != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Ti.Kido = -1;
				Ti.Add = 0;
				Ti.Sub_rtn0 = 1;
			}
		}
		goto pad_check;
	} else if (Ti.Sub_rtn1 == 0) {
		Ti.Moji_kido += 2;
		Title_logo_disp(0, Ti.Moji_kido, 1);
		if (Ti.Moji_kido < 0x80) goto pad_check;
		Ti.Moji_kido = 0x80;
	}
	Ti.Sub_rtn1++;
pad_check:
	if ((G.Padd1_trg & 0x9ff) != 0) {
		Snd_se_stad(0x4060000, (VECTOR *)0x0);
		Ti.Add = -0x1000;
		Ti.Sub_rtn0++;
		if (Ti.Kido == 0) {
			Ti.Kido = 0x5a00;
		}
	}
}

/* DS 0x801936bc (Spy_sel) */
static VOID Spy_sel(VOID)
{
	ULONG fade_sts;
	SHORT Add;
	LONG vib_time;
	LONG pow_st;
	LONG pow_ed;
	USHORT vib_attr;

	Ti.Kido += Ti.Add;
	Fade_adjust(3, (LONG)Ti.Kido, 0, (RECT *)0x0);
	fade_sts = Fade_status(3);
	if (fade_sts != 0) {
		Ti.Add = 0;
		Ti.Kido = -1;
	}
	if (((G.Key_trg & 0x1000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
		fade_sts = Fade_status(3);
		if (fade_sts != 0) {
			if (Ti.Cur == 0) {
				Ti.Main_rtn = 2;
				Ti.Sub_rtn0 = 3;
				if ((G.System_flg & SYS_TOFU_SURVIVOR) == 0) {
					G.System_flg |= SYS_NEW_GAME;
					Pad_vib_set0(7, 0);
					Pad_vib_set1(9, 0xff, 0);
					vib_time = 4; pow_st = 0xff; pow_ed = 0x3c; vib_attr = 10;
				} else {
					G.System_flg |= SYS_NEW_GAME;
					Pad_vib_set0(7, 0);
					Pad_vib_set1(9, 0xff, 0);
					Pad_vib_fade_set(4, 0xff, 0x3c, 10);
					Pad_vib_set1(0x14, 0x78, 0x1e);
					vib_time = 0x10; pow_st = 0x78; pow_ed = 0x32; vib_attr = 0x32;
				}
				Pad_vib_fade_set(vib_time, pow_st, pow_ed, vib_attr);
				Snd_se_stad(0x4000000, (VECTOR *)0x0);
				Fade_set(0x100, -0x800, 7, 1);
				while (1) {
					fade_sts = Fade_status(0);
					Add = 0x80;
					if (fade_sts != 0) break;
					Title_logo_disp(0x80, 0x80, 0);
					Moji_spy_disp();
					Task_sleep(1);
				}
			} else {
				Ti.Main_rtn = 1;
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				Add = 0x1800;
				G.System_flg &= ~(SYS_TOFU_SURVIVOR | SYS_TIMER);
			}
			Fade_set(0x200, Add, 7, 1);
			Fade_adjust(0, 0x200, 0x101010, (RECT *)0x0);
			while (fade_sts = Fade_status(0), fade_sts == 0) {
				Title_logo_disp(0x80, 0x80, 0);
				Moji_spy_disp();
				Task_sleep(1);
			}
			Fade_set(0x200, 0, 7, 1);
			Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0x0);
			Task_sleep(1);
			if (Ti.Cur != 0) {
				Snd_sys_core_set(17, 0);
			}
			Title_bg_reload();
			return;
		}
		Ti.Add = -0x1000;
	}
	Title_disp_flicker();
	if (((G.Padd1_trg & 0x1000) != 0) && (Ti.Cur == 1)) {
		Snd_se_stad(0x4040000, (VECTOR *)0x0);
		Ti.Cur = 0;
	}
	if (((G.Padd1_trg & 0x4000) != 0) && (Ti.Cur == 0)) {
		Snd_se_stad(0x4040000, (VECTOR *)0x0);
		Ti.Cur = 1;
	}
	Ti.Moji_kido += 0x10;
	if (0x7f < Ti.Moji_kido) Ti.Moji_kido = 0x80;
	Ti.Moji_kido2 += 0x10;
	if (0x7f < Ti.Moji_kido2) Ti.Moji_kido2 = 0x80;
	Title_logo_disp(Ti.Moji_kido2, Ti.Moji_kido, 1);
	Moji_spy_disp();
}

/* DS 0x80193a7c (Title_information) */
static VOID Title_information(VOID)
{
	ULONG fade_sts;
	LONG i;
	LONG j;
	LONG state;
	LONG choice;
	LONG move_cnt;
	LONG ofs_x;
	LONG side;

	Task_suspend(0);
	Fade_set(0x200, 0x555, 7, 0);
	Fade_adjust(0, 0, 0, (RECT *)0x0);
	while (1) {
		fade_sts = Fade_status(0);
		if (fade_sts != 0) break;
		Task_sleep(1);
	}
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			USHORT tpage;
			SetTile(&Info_tile[i][j]);
			SetSemiTrans(&Info_tile[i][j], 1);
			tpage = GetTPage(1, 2, 0x200, 0);
			SetDrawMode(&Info_dr_mode[i][j], 0, 0, (ULONG)tpage, (RECT *)0x0);
		}
	}
	while (1) {
		Fade_set(0x200, -0x554, 7, 0);
		Fade_adjust(0, 0, 0, (RECT *)0x0);
		side = 0;
		ofs_x = 0;
		state = 0;
		choice = 0;
		move_cnt = 0;
		while (1) {
			switch (state) {
			case 0:
				fade_sts = Fade_status(0);
				if (fade_sts != 0) {
					state = 1;
				}
				break;
			case 1:
				move_cnt = 8;
				if ((side == 0) && ((G.Padd1_trg & 0x2000) != 0)) {
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
					state = 2;
				}
				if ((side == 1) && ((G.Padd1_trg & 0x8000) != 0)) {
					Snd_se_stad(0x4040000, (VECTOR *)0x0);
					state = 3;
				}
				if (((G.Key_trg & 0x1000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
					if (side == 0) {
						Snd_se_stad(0x4060000, (VECTOR *)0x0);
						choice = 1;
					} else {
						Snd_se_stad(0x4060000, (VECTOR *)0x0);
						choice = 2;
					}
					state = 4;
				}
				if ((G.Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					Ti.Main_rtn = 1;
					state = 4;
				}
				break;
			case 2:
				side = 1;
				ofs_x = 0x94 - (0x94 * move_cnt * move_cnt * move_cnt * move_cnt) / 0x1000;
				move_cnt--;
				if (move_cnt < 0) {
					state = 0;
				}
				break;
			case 3:
				side = 0;
				ofs_x = (0x94 * move_cnt * move_cnt * move_cnt * move_cnt) / 0x1000;
				move_cnt--;
				if (move_cnt < 0) {
					state = 0;
				}
				break;
			case 4:
				Fade_set(0x200, 0x555, 7, 0);
				Fade_adjust(0, 0, 0, (RECT *)0x0);
				state = 5;
				/* fall through */
			case 5:
				fade_sts = Fade_status(0);
				if (fade_sts != 0) {
					Bg_set_mode(2, 0);
					switch (choice) {
					case 1:
						state = 6;
						Init_movie_work(5);
						break;
					case 2:
						G.System_flg |= SYS_TIMER;
						/* fall through */
					case 0:
						Task_signal(0);
						Task_exit();
						break;
					}
				}
				break;
			case 6:
				while ((G.System_flg & SYS_STR_MOVIE) != 0) {
					Task_sleep(1);
				}
				Bg_set_mode(0, 0);
				Fade_adjust(0, 0x7fff, 0xffffff, (RECT *)0x0);
				goto restart;
			}
			Info_tile[G.Cbuf_id][0].w = ofs_x + 0x14;
			Info_tile[G.Cbuf_id][1].x0 = ofs_x + 0x98;
			Info_tile[G.Cbuf_id][1].w = 0xa8 - ofs_x;
			for (j = 0; j < 2; j++) {
				AddPrim(G.pOt1 + 1, &Info_tile[G.Cbuf_id][j]);
				AddPrim(G.pOt1 + 1, &Info_dr_mode[G.Cbuf_id][j]);
			}
			Task_sleep(1);
		}
restart:
		;
	}
}

/* DS 0x80193fb4 (Init_tbl_load) */
VOID Init_tbl_load(VOID)
{
	G.pFree_work = (UCHAR *)Tex_tim_buf;
	Cd_read(INIT_TBL_DAT, (ULONG *)Init_tbl_buf, 0, (UCHAR *)"Init_tbl");
	memcpy16((ULONG *)&G.SaveData, (ULONG *)(Init_tbl_buf + (ULONG)G.Dummy8[1] * 0x798), 0x798);
}

/* DS 0x80194040 (Moji_set_work) */
VOID Moji_set_work(VOID)
{
	register ULONG i;
	register ULONG j;

	UCHAR *pRate;
	UCHAR *pClutX;
	USHORT tpage;
	SPRT *p;

	for (j = 0; j < 2; j++) {
		for (i = 0; i < 0x3e; i++) {
			p = &Title_prim[i].Spr[j];
			p->r0 = 0x80; p->g0 = 0x80; p->b0 = 0x80; p->code = 0;
			p->x0 = Moji_work_tbl[i].x;
			p->y0 = Moji_work_tbl[i].y;
			p->w  = Moji_work_tbl[i].w;
			p->h  = Moji_work_tbl[i].h;
			p->u0 = Moji_work_tbl[i].u;
			p->v0 = Moji_work_tbl[i].v;
			p->clut = (Moji_work_tbl[i].clut_y + 0x1e0) * 0x40;
			SetSprt(p);
			SetSemiTrans(p, (ULONG)Moji_work_tbl[i].flag);
			pRate = &Moji_work_tbl[i].rate;
			pClutX = &Moji_work_tbl[i].clut_x;
			tpage = GetTPage(1, (ULONG)*pRate, (ULONG)*pClutX << 6, 0);
			SetDrawMode(&Title_prim[i].Dr[j], 0, 0, (ULONG)tpage, (RECT *)0x0);
		}
	}
	if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) {
		Title_prim[34].Spr[0].x0 += 0x10;
		Title_prim[34].Spr[0].y0 = Title_prim[34].Spr[0].y0 + -4;
		Title_prim[34].Spr[1].x0 += 0x10;
		Title_prim[34].Spr[1].y0 = Title_prim[34].Spr[1].y0 + -4;
	}
}

/* DS 0x80194240 (Moji_SelectMode) */
VOID Moji_SelectMode(ULONG Mode, ULONG No, ULONG Dis, ULONG Flg)
{
	SHORT disp_ofs;
	UCHAR kido_save;
	CHAR disp_col;
	USHORT tpage;
	SPRT *p;
	ULONG row_idx;
	ULONG row_base;
	ULONG move_pos;

	if (Flg == 0) {
		move_pos = 8 - Ti.Moji_move_cnt;
	} else {
		move_pos = (ULONG)Ti.Moji_move_cnt;
	}
	row_idx = 0;
	do {
		ULONG ent_idx;
		row_base = row_idx * 2;
		disp_ofs = (SHORT)(Dis * move_pos * move_pos * move_pos * move_pos >> 0xc);
		ent_idx = row_base;
		do {
			/* cell ent_idx: row_idx * 2 == row_base, so the row-Mode branches address the same cell as ent_idx */
			p = &Title_prim[ent_idx].Spr[G.Cbuf_id];
			p->y0 = Moji_work_tbl[ent_idx].y;
			p->b0 = 0x80;
			p->g0 = 0x80;
			p->r0 = 0x80;
			kido_save = Ti.Moji_kido;
			if (Ti.Moji_move_cnt != 0) {
				if (row_idx == No) {
					if (Flg == 0) {
						p->y0 = p->y0 - ((SHORT)Dis - disp_ofs);
					} else {
						p->y0 -= disp_ofs;
					}
				} else {
					p->b0 = Ti.Moji_kido;
					p->g0 = kido_save;
					p->r0 = kido_save;
				}
			}
			if (row_idx == Mode) {
				tpage = 0x7f80;
				if (Ti.Moji_move_cnt != 8) {
					disp_col = Ti.Disp_cnt + 0x80;
					p->b0 = disp_col;
					p->g0 = disp_col;
					p->r0 = disp_col;
				}
			} else {
				tpage = 0x7f8c;
			}
			Title_prim[ent_idx].Spr[G.Cbuf_id].clut = tpage;
			AddPrim(G.pOt1 + 1, p);
			ent_idx++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[ent_idx - 1].Dr[G.Cbuf_id]);
		} while (ent_idx < row_base + 2);
		row_idx++;
		if (3 < row_idx) return;
	} while (1);
}

/* DS 0x80194568 (Moji_mode_disp) */
static VOID Moji_mode_disp(LONG Mode, ULONG Opt_a, ULONG Opt_b, LONG Row)
{
	UCHAR kido2;
	LONG cell;
	SPRT *pPrim;
	CHAR disp_col;
	ULONG col;
	ULONG row;

	row = 0;
	do {
		col = 0;
		do {
			kido2 = Ti.Moji_kido2;
			cell = row * 2 + col + 8;
			col++;
			Title_prim[cell].Spr[G.Cbuf_id].b0 = Ti.Moji_kido2;
			Title_prim[cell].Spr[G.Cbuf_id].g0 = kido2;
			Title_prim[cell].Spr[G.Cbuf_id].r0 = kido2;
		} while (col < 2);
		row = row + 1; col = 0;
	} while (row < 3);
	cell = 0;
	do {
		if (Mode == 0) {
			if (Ti.Rookie_flg == 1) {
				Title_prim[cell + 8].Spr[G.Cbuf_id].clut = 0x7f8c;
				Title_prim[cell + 10].Spr[G.Cbuf_id].clut = 0x7f80;
				Row = 1;
			} else {
				Title_prim[cell + 10].Spr[G.Cbuf_id].clut = 0x7f8c;
				Title_prim[cell + 8].Spr[G.Cbuf_id].clut = 0x7f80;
				Row = 0;
			}
			Title_prim[cell + 12].Spr[G.Cbuf_id].clut = 0x7f8c;
		} else if (Mode == 1) {
			Title_prim[cell + 8].Spr[G.Cbuf_id].clut = 0x7f8c;
			Title_prim[cell + 10].Spr[G.Cbuf_id].clut = 0x7f8c;
			if (col == 0) {
				Title_prim[12].Spr[G.Cbuf_id].clut = 0x7f80;
			} else {
				Title_prim[cell + 12].Spr[G.Cbuf_id].clut = 0x7f8c;
			}
			Row = 2;
		}
		col++;
		cell++;
	} while (col < 2);
	row = 0;
	pPrim = &Title_prim[Row * 2 + 8].Spr[G.Cbuf_id];
	disp_col = Ti.Moji_kido2 + Ti.Disp_cnt;
	pPrim->b0 = disp_col;
	pPrim->g0 = disp_col;
	pPrim->r0 = disp_col;
	do {
		col = 0;
		do {
			cell = row * 2 + col + 8;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Spr[G.Cbuf_id]);
			col++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Dr[G.Cbuf_id]);
		} while (col < 2);
		row++;
	} while (row < 3);
}

/* DS 0x80194910 (Moji_Game_mode) */
VOID Moji_Game_mode(register ULONG Mode)
{
	UCHAR kido2;
	LONG cell;
	SPRT *pPrim;
	CHAR disp_col;
	LONG Row_idx;
	ULONG col;
	ULONG row;

	row = 0;
	do {
		col = 0;
		do {
			kido2 = Ti.Moji_kido2;
			cell = row * 2 + col + 0xe;
			col++;
			Title_prim[cell].Spr[G.Cbuf_id].b0 = Ti.Moji_kido2;
			Title_prim[cell].Spr[G.Cbuf_id].g0 = kido2;
			Title_prim[cell].Spr[G.Cbuf_id].r0 = kido2;
		} while (col < 2);
		row = row + 1; col = 0;
	} while (row < 2);
	cell = 0;
	do {
		if (Mode == 0) {
			Title_prim[cell + 14].Spr[G.Cbuf_id].clut = 0x7f80;
			Row_idx = 0;
			Title_prim[cell + 16].Spr[G.Cbuf_id].clut = 0x7f8c;
		} else if (Mode == 1) {
			Title_prim[cell + 14].Spr[G.Cbuf_id].clut = 0x7f8c;
			if (col == 0) {
				pPrim = &Title_prim[16].Spr[G.Cbuf_id];
			} else {
				pPrim = &Title_prim[cell + 14].Spr[G.Cbuf_id];
			}
			pPrim->clut = 0x7f80;
			Row_idx = 1;
		}
		col++;
		cell++;
	} while (col < 2);
	row = 0;
	pPrim = &Title_prim[Row_idx * 2 + 14].Spr[G.Cbuf_id];
	disp_col = Ti.Moji_kido2 + Ti.Disp_cnt;
	pPrim->b0 = disp_col;
	pPrim->g0 = disp_col;
	pPrim->r0 = disp_col;
	do {
		col = 0;
		do {
			cell = row * 2 + col + 0xe;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Spr[G.Cbuf_id]);
			col++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Dr[G.Cbuf_id]);
		} while (col < 2);
		row++;
	} while (row < 2);
}

/* DS 0x80194bd0 (Moji_Sound) */
VOID Moji_Sound(register ULONG Mode)
{
	UCHAR kido2;
	LONG cell;
	LONG sel_row;
	CHAR disp_col;
	ULONG col;
	ULONG row;
	SPRT *pSpr;

	row = 0;
	do {
		col = 0;
		do {
			kido2 = Ti.Moji_kido2;
			cell = row * 2 + col + 0x12;
			col++;
			Title_prim[cell].Spr[G.Cbuf_id].b0 = Ti.Moji_kido2;
			Title_prim[cell].Spr[G.Cbuf_id].g0 = kido2;
			Title_prim[cell].Spr[G.Cbuf_id].r0 = kido2;
		} while (col < 2);
		row = row + 1; col = 0;
	} while (row < 3);
	cell = 0;
	do {
		if (((LONG)Mode) == 0) {
			if ((G.Sound_mode & 1) == 0) {
				sel_row = cell;
				Title_prim[sel_row + 18].Spr[G.Cbuf_id].clut = 0x7f80;
				sel_row = 0;
				Title_prim[cell + 20].Spr[G.Cbuf_id].clut = 0x7f8c;
			} else {
				Title_prim[cell + 18].Spr[G.Cbuf_id].clut = 0x7f8c;
				sel_row = cell;
				Title_prim[sel_row + 20].Spr[G.Cbuf_id].clut = 0x7f80;
				sel_row = 1;
			}
			Title_prim[cell + 22].Spr[G.Cbuf_id].clut = 0x7f8c;
		} else {
			Title_prim[cell + 18].Spr[G.Cbuf_id].clut = 0x7f8c;
			Title_prim[cell + 20].Spr[G.Cbuf_id].clut = 0x7f8c;
			if (col == 0) {
				Title_prim[22].Spr[G.Cbuf_id].clut = 0x7f80;
			} else {
				Title_prim[cell + 22].Spr[G.Cbuf_id].clut = 0x7f8c;
			}
			sel_row = 2;
		}
		col++;
		cell++;
	} while (col < 2);
	row = 0;
	pSpr = &Title_prim[sel_row * 2 + 18].Spr[G.Cbuf_id];
	disp_col = Ti.Moji_kido2 + Ti.Disp_cnt;
	pSpr->b0 = disp_col;
	pSpr->g0 = disp_col;
	pSpr->r0 = disp_col;
	do {
		col = 0;
		do {
			cell = row * 2 + col + 0x12;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Spr[G.Cbuf_id]);
			col++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Dr[G.Cbuf_id]);
		} while (col < 2);
		row++;
	} while (row < 3);
}

/* DS 0x80194f6c (Moji_spy_disp) */
static VOID Moji_spy_disp(VOID)
{
	CHAR disp_col;
	LONG cell;
	LONG cur_cell;
	ULONG ent_no;
	ULONG row;

	row = 0;
	do {
		LONG row_base;
		LONG col_idx;

		ent_no = 0x19;
		row_base = row * 2;
		cell = row_base + 0x19;
		col_idx = row_base;
		do {
			Title_prim[cell].Spr[G.Cbuf_id].b0 = 0x80;
			Title_prim[cell].Spr[G.Cbuf_id].g0 = 0x80;
			Title_prim[cell].Spr[G.Cbuf_id].r0 = 0x80;
			if (row == Ti.Cur) {
				cur_cell = (ent_no == 0x19) ? (LONG)(row * 2) : col_idx;
				Title_prim[cur_cell + 25].Spr[G.Cbuf_id].clut = 0x7f80;
				disp_col = Ti.Disp_cnt + 0x80;
				Title_prim[cell].Spr[G.Cbuf_id].b0 = disp_col;
				Title_prim[cell].Spr[G.Cbuf_id].g0 = disp_col;
				Title_prim[cell].Spr[G.Cbuf_id].r0 = disp_col;
			} else {
				Title_prim[col_idx + 25].Spr[G.Cbuf_id].clut = 0x7f8c;
			}
			cell = row_base + ent_no;
			ent_no++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Spr[G.Cbuf_id]);
			col_idx++;
			AddPrim(G.pOt1 + 1, (VOID *)&Title_prim[cell].Dr[G.Cbuf_id]);
			cell = row_base + ent_no;
		} while (ent_no < 0x1b);
		row++;
	} while (row < 2);
}

/* DS 0x80195160 (Title_logo_disp) */
static VOID Title_logo_disp(UCHAR Kido_a, UCHAR Kido_b, LONG Semi_trans)
{
	LONG cell;
	SPRT *pPrim;
	ULONG cnt;

	cnt = 0;
	cell = 30;
	do {
		pPrim = &Title_prim[cell].Spr[G.Cbuf_id];
		pPrim->b0 = Kido_a;
		pPrim->g0 = Kido_a;
		pPrim->r0 = Kido_a;
		SetSemiTrans(pPrim, Semi_trans);
		AddPrim(G.pOt1 + 2, pPrim);
		cnt++;
		AddPrim(G.pOt1 + 2, (VOID *)&Title_prim[cell].Dr[G.Cbuf_id]);
		cell++;
	} while (cnt < 4);
	pPrim = &Title_prim[34].Spr[G.Cbuf_id];
	pPrim->b0 = Kido_b;
	pPrim->g0 = Kido_b;
	pPrim->r0 = Kido_b;
	AddPrim(G.pOt1 + 2, pPrim);
	AddPrim(G.pOt1 + 2, (VOID *)&Title_prim[34].Dr[G.Cbuf_id]);
}

// --- static display helpers ---

/* DS 0x801952d8 (Moji_License) */
VOID Moji_License(VOID)
{
	tagGLOBAL *g = &G;
	TITLE_PRIM *prim = Title_prim;

	AddPrim(g->pOt1 + 1, (VOID *)&prim[29].Spr[g->Cbuf_id]);
	AddPrim(g->pOt1 + 1, (VOID *)&prim[29].Dr[g->Cbuf_id]);
	Moji_Sample();
}


/* DS 0x80195370 (Moji_Sample) */
VOID Moji_Sample(VOID)
{
	tagGLOBAL *g = &G;
	TITLE_PRIM *prim = Title_prim;

	AddPrim(g->pOt1 + 1, (VOID *)&prim[24].Spr[g->Cbuf_id]);
	AddPrim(g->pOt1 + 1, (VOID *)&prim[24].Dr[g->Cbuf_id]);
}

/* DS 0x80195400 (Title_bg_disp) */
static VOID Title_bg_disp(VOID)
{
	UCHAR bright;
	LONG tmp_i;
	LONG tmp_j;
	ULONG Frame_pad[12];		/* retail reserves 0x48 bytes and only saves s0-s2/ra in them */

	(void)Frame_pad;

	if (Ti.Tile_kido != 0) {
		Ti.Tile_kido = Ti.Tile_kido + -2;
	}
	tmp_i = Ti.Bg_cnt + -1;
	if (-1 < Ti.Bg_cnt) {
		Ti.Bg_y = (SHORT)((tmp_i * tmp_i * 0x2e) / 0x1e4) + -0x2e;
		Ti.Bg_cnt = tmp_i;
	}
	Title_prim[35].Spr[G.Cbuf_id].y0 = Ti.Bg_y + 0x4c;
	Title_prim[36].Spr[G.Cbuf_id].y0 = Ti.Bg_y + 0x4c;
	Title_prim[37].Spr[G.Cbuf_id].y0 = Ti.Bg_y + 0x4c;
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[35].Spr[G.Cbuf_id]);
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[35].Dr[G.Cbuf_id]);
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[36].Spr[G.Cbuf_id]);
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[36].Dr[G.Cbuf_id]);
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[37].Spr[G.Cbuf_id]);
	AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[37].Dr[G.Cbuf_id]);
	if ((Ti.Bg_cnt < 0) && (Main.Fade[0].Kido < 1)) {
		if (Title_fade_val < 0x100) {
			tmp_j = (int)Title_fade_val + (ULONG)Title_fade_add;
			Title_fade_val = (SHORT)tmp_j;
			if (Title_fade_val2 < 0x80) {
				Title_fade_val2 += Title_fade_add;
			}
			if (tmp_j * 0x10000 >> 0x10 == 0x100) {
				Title_fade_val = 0xff;
				Title_fade_add = 0xfffc;
			}
		}
		if (((SHORT)Title_fade_add < 0) && (Title_fade_val == 0x7f)) {
			Title_fade_add = 0;
			Title_fade_val = 0x80;
			Title_fade_val2 = 0x80;
		}
	}
	if (0 < Title_fade_val) {
		SPRT *pSpr;
		bright = (UCHAR)Title_fade_val;
		pSpr = &Title_prim[38].Spr[G.Cbuf_id];
		pSpr->b0 = bright; pSpr->g0 = bright; pSpr->r0 = bright;
		bright = (UCHAR)Title_fade_val;
		pSpr = &Title_prim[39].Spr[G.Cbuf_id];
		pSpr->b0 = (UCHAR)Title_fade_val;
		pSpr->g0 = bright; pSpr->r0 = bright;
		AddPrim(G.pOt0 + 2, (VOID *)&Title_prim[38].Spr[G.Cbuf_id]);
		AddPrim(G.pOt0 + 2, (VOID *)&Title_prim[39].Spr[G.Cbuf_id]);
		AddPrim(G.pOt0 + 2, (VOID *)&Title_prim[38].Dr[G.Cbuf_id]);
	}
	if (0 < Title_fade_val2) {
		SPRT *pSpr;
		bright = (UCHAR)Title_fade_val2;
		pSpr = &Title_prim[40].Spr[G.Cbuf_id];
		pSpr->b0 = bright; pSpr->g0 = bright; pSpr->r0 = bright;
		bright = (UCHAR)Title_fade_val2;
		pSpr = &Title_prim[41].Spr[G.Cbuf_id];
		pSpr->b0 = (UCHAR)Title_fade_val2;
		pSpr->g0 = bright; pSpr->r0 = bright;
		AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[40].Spr[G.Cbuf_id]);
		AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[41].Spr[G.Cbuf_id]);
		AddPrim(G.pOt0 + 3, (VOID *)&Title_prim[40].Dr[G.Cbuf_id]);
	}
}

/* DS 0x80195940 (Title_moji_tile) */
static VOID Title_moji_tile(VOID)
{
	UCHAR bright;
	ULONG cbuf_idx;
	TITLE_WORK *ti = &Ti;

	cbuf_idx = (ULONG)G.Cbuf_id;
	bright = (UCHAR)ti->Tile_kido;
	Moji_tile_tbl[cbuf_idx + 2].r0 = (UCHAR)ti->Tile_kido;
	Moji_tile_tbl[cbuf_idx + 2].g0 = bright;
	Moji_tile_tbl[cbuf_idx + 2].b0 = bright;
	Moji_tile_tbl[cbuf_idx].r0 = bright;
	Moji_tile_tbl[cbuf_idx].g0 = bright;
	Moji_tile_tbl[cbuf_idx].b0 = bright;
	if (0 < ti->Tile_kido) {
		AddPrim(G.pOt0 + 4, Moji_tile_tbl + cbuf_idx);
		AddPrim(G.pOt0 + 4, Moji_tile_tbl + cbuf_idx + 2);
		SetDrawMode((DR_MODE *)(&ti->Moji0 + G.Cbuf_id), 0, 0, 200, (RECT *)0x0);
		AddPrim(G.pOt0 + 4, &ti->Moji0 + G.Cbuf_id);
	}
}

/* DS 0x80195a38 (Title_game_init) */
VOID Title_game_init(VOID)
{
	UCHAR core_type_no;
	UCHAR battle_life;
	ULONG msg_no;

	Game.Doordemo_flg = 3;
	UnknownPadInfoByte = 1;
	Main.Scrl_type = 0;
	Bg_set_mode(2, 0);
	Fade_off(0);
	G.SaveData.Stereo = (G.SaveData.Stereo | G.Sound_mode) & 1;
	if ((int)G.Status_flg < 0) {
		G.Dummy8[1] = 2;
	} else {
		G.Dummy8[1] = 0;
	}
	if ((G.Status_flg & STAT_SCENARIO_B) != 0) {
		G.Dummy8[1]++;
	}
	if ((G.System_flg & SYS_TIMER) != 0) {
		G.Dummy8[1] = 4;
		G.Status_flg |= STAT_SPECIAL_RUN;
	}
	if ((G.System_flg & SYS_EX_BATTLE) != 0) {
		G.Dummy8[1] = 5;
	}
	if ((G.System_flg & (SYS_EX_BATTLE | SYS_NEW_GAME)) == (SYS_EX_BATTLE | SYS_NEW_GAME)) {
		Task_lexecute(1, 3);
	}
	Task_sleep(1);
	if ((G.System_flg & (SYS_EX_BATTLE | SYS_DEMO_MODE | SYS_TIMER)) == 0) {
		Task_lexecute(1, 5);
		Task_sleep(1);
	}
	if ((G.System_flg & SYS_NEW_GAME) == 0) {
		if ((G.System_flg & SYS_EX_BATTLE) == 0) {
			if ((G.System_flg & SYS_DEMO_MODE) == 0) {
				if (G.SaveData.Save_area - 0xf < 2) {
					if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
						msg_no = (ULONG)Open_msg_ptr[0];
					} else {
						msg_no = (ULONG)Open_msg_ptr[1];
					}
				} else {
					msg_no = (ULONG)Open_msg_ptr[2];
				}
			} else {
				msg_no = (ULONG)Open_msg_ptr[3];
			}
		} else {
			msg_no = (ULONG)Open_msg_ptr[5];
		}
open_msg_set:
		Mess_set(0xb40000, 0x4c02, (ULONG)&Open_msg_00[msg_no], 0);
	} else {
		if ((G.System_flg & SYS_TIMER) != 0) {
			msg_no = (ULONG)Open_msg_ptr[4];
			goto open_msg_set;
		}
		if ((G.System_flg & SYS_EX_BATTLE) == 0) {
			msg_no = (ULONG)Open_msg_ptr[0];
			goto open_msg_set;
		}
	}
	if ((G.System_flg & SYS_NEW_GAME) == 0) {
		if ((G.System_flg & SYS_DEMO_MODE) == 0) {
			G.Player.Pos_x = (LONG)G.SaveData.Pl_pos_x;
			G.Player.Pos_y = (LONG)G.SaveData.Pl_pos_y;
			G.Player.Pos_z = (LONG)G.SaveData.Pl_pos_z;
			G.Player.Cdir_y = G.SaveData.Dummy0;
			G.Player.Id = G.SaveData.Pl_id;
			G.Player.nFloor = (CHAR)(G.SaveData.Pl_pos_y >> 0xf) -
				((CHAR)((int)G.SaveData.Pl_pos_y / 0x708) + (CHAR)(G.SaveData.Pl_pos_y >> 7));
			if ((G.System_flg & SYS_EX_BATTLE) != 0) {
				G.Font_type = 9;
			}
		} else {
			G.System_flg &= ~SYS_EASY_MODE;
			G.SaveData.Max_item_num = 8;
			G.Status_flg &= ~STAT_EASY_MODE;
			G.SaveData.Stage_no = (SHORT)Demo.Stage_no;
			G.Player.Pos_x = (LONG)Demo.Pos_x;
			G.Player.Pos_y = (LONG)Demo.Pos_y;
			G.Player.Pos_z = (LONG)Demo.Pos_z;
			G.SaveData.Room_no = (SHORT)Demo.Room_no;
			G.SaveData.Cut_no = (SHORT)Demo.Cut_no;
			G.SaveData.Equip_id = Demo.Equip_id;
			St.Equip_item_id = Demo.Equip_id;
			G.SaveData.Equip_no = Demo.Equip_no;
			St.Equip_item_no = Demo.Equip_no;
			G.Player.Id = Demo.Player_id;
			G.Player.Cdir_y = Demo.Cdir_y;
			G.Player.nFloor = (CHAR)(Demo.Pos_y >> 0xf) -
				((CHAR)((int)Demo.Pos_y / 0x708) + (CHAR)(Demo.Pos_y >> 7));
			Memcpy(G.SaveData.Item_work, Demo.Item_wk, 0x2c);
			G.SaveData.Pl_life = 200;
			Main.Padoff_stop_flg[1][1] = 0;
			Key_idx_bak = G.SaveData.Key_idx;
			G.SaveData.Key_idx = Demo.Key_idx;
		}
		G.SaveData.Room_no_old = 0xff;
		G.Player.Life = G.SaveData.Pl_life;
	} else {
		if ((G.System_flg & SYS_EASY_MODE) != 0) {
			G.Status_flg |= STAT_EASY_MODE;
		}
		Init_tbl_load();
		if ((G.System_flg & SYS_TIMER) == 0) {
			if ((G.System_flg & SYS_EX_BATTLE) == 0) {
				if ((G.Status_flg & STAT_SCENARIO_B) == 0) {
					G.Player.Pos_x = 0x4972;
					G.Player.Pos_z = -0xc5c;
					G.SaveData.Room_no = 0;
					G.Player.Cdir_y = 0x800;
				} else {
					G.SaveData.Room_no = 4;
					G.Player.Pos_x = -0x4600;
					G.Player.Pos_z = -0x54da;
					G.Player.Cdir_y = 400;
				}
				G.SaveData.Stage_no = 0;
				G.SaveData.Cut_no = 0;
				G.Player.Pos_y = 0;
				G.Player.nFloor = 0;
				G.SaveData.Dummy[0] = 200;
				G.SaveData.Dummy1[1] = 0;
				G.Player.Id = (int)G.Status_flg < 0;
				G.SaveData.Pl_life = 200;
				G.Player.Life = 200;
			} else {
				G.SaveData.Dummy[1] = (USHORT)G.ExData[1];
				G.SaveData.Sce_timer = Ti_ex_cur + 1;
				G.SaveData.Player_life = (SHORT)*(Ex_battle_life + (ULONG)G.Player.Id);
				Memcpy(G.SaveData.ExData3 + 8, G.ExData + 4, 0x3c);
				G.SaveData.Stage_no = 5;
				G.Font_type = 9;
				G.SaveData.Room_no = 0x12;
				G.SaveData.Cut_no = 1;
				G.Player.Pos_x = -0x450a;
				G.Player.Pos_z = -0x1ef3;
				G.Player.Cdir_y = -0x6ab;
				G.Player.Pos_y = 0;
				G.Player.nFloor = 0;
				G.Player.Life = 200;
				Sce_rnd_item_set();
			}
		} else {
			G.SaveData.Stage_no = 2;
			G.SaveData.Room_no = 4;
			G.SaveData.Cut_no = 3;
			G.Player.Pos_x = -0x1cff;
			G.Player.Pos_y = 0;
			G.Player.Pos_z = -0x5fac;
			G.Player.Cdir_y = 0x800;
			G.Player.nFloor = 0;
			if ((G.System_flg & SYS_TOFU_SURVIVOR) == 0) {
				G.Player.Id = 12;
				G.Player.Life = 200;
			} else {
				G.Player.Id = 13;
				G.Player.Life = 400;
			}
		}
		G.Pl_poison_timer = 0;
		G.Pl_poison_down = 0;
		G.Player.Old_x = (SHORT)G.Player.Pos_x;
		G.Player.Sca_old_x = (SHORT)G.Player.Pos_z;
		G.Player.Old_z = (SHORT)G.Player.Pos_z;
		G.Player.Ground = (SHORT)G.Player.Pos_y;
		G.SaveData.Key_idx = Main.Key_idx;
		Status_init();
	}
	Status_init_m();
	if ((G.Sound_mode & 0x20) == 0) {
		G.SaveData.Weapon_mugen &= 0x2001;
	} else {
		G.SaveData.Weapon_mugen |= 0xffffdf00;
	}
	switch (G.Dummy8[1]) {
	case 0: case 1: core_type_no = 0; break;
	case 2: case 3: core_type_no = 1; break;
	case 4:
		core_type_no = 12;
		if ((G.System_flg & SYS_TOFU_SURVIVOR) != 0) core_type_no = 13;
		break;
	case 5:
		battle_life = *(Ex_battle_life + (ULONG)G.Player.Id);
		if (battle_life == 1) core_type_no = 1;
		else if (battle_life < 2) { core_type_no = 0; if (battle_life != 0) goto skip_core_set; }
		else if (battle_life == 2) core_type_no = 14;
		else { core_type_no = 11; if (battle_life != 3) goto skip_core_set; }
		break;
	default: goto skip_core_set;
	}
	Snd_sys_core_set(core_type_no, 4);
skip_core_set:
	Snd_sys_init_sub2();
	Esp_init_C();
	Em_move_tbl_set();
	Set_stage();
	G.pEnemy[0] = (ENEMY_WORK *)&G.Player;
	G.Player.Be_flg = 1;
	G.pFree_work = G.pFree_work_head;
	SetSpadStack();
	Player_set((ENEMY_WORK *)&G.Player);
	ResetSpadStack();
	Spl_set();
	Kage_work_init();
	Kage_work9_init();
	Sce_test_init();
	Set_room();
	while ((Moji.Mess_flg & 0x80) != 0) {
		Task_sleep(1);
	}
	G.System_flg |= SYS_IN_GAME;
	G.Game_time_offset = VSync(-1);
	Main.Vloop = 2;
	Bg_set_mode(0, 0);
	Main.Bg_chg_flg = 1;
	Cut_change((int)G.SaveData.Cut_no);
	Cut_check(1);
	UnknownPadInfoByte = 0;
}

/* DS 0x801963f4 (Title_disp_flicker) */
static VOID Title_disp_flicker(VOID)
{
	Ti.Disp_cnt += Ti.Disp_add;
	if ((Ti.Disp_cnt == 0) || (0x30 < Ti.Disp_cnt)) {
		Ti.Disp_add = -Ti.Disp_add;
	}
}
