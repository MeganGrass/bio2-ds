/*
 * MEM_CARD.C - Memory card access module (MEMCARD overlay)
 * Resident Evil 2 Dual Shock Ver. (SLUS-00748) decompilation
 */

#include "TYPES.H"
#include <KERNEL.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include <MEMORY.H>
#include <STRINGS.H>
#include <TYPES.H>
#include "MAIN.H"
#include "CONFIG.H"
#include "SLD.H"
#include "MEM_CARD.H"
#include "CD.H"
#include "ESP_R.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "MONITOR.H"
#include "SCE1.H"
#include "SND.H"
#include "STATUS.H"
#include "ST_SUB04.H"
#include "SUB.H"
#include "TITLE.H"
#include "DOOR.H"
#include "GAME.H"
#include "FILECODE.H"
#include "flag.h"

/* DS 0x801bfa1c (Mc_search_pat) */
UCHAR Mc_search_pat[4] = {
	0x00, 0x00, 0x00, 0x00
};

/* DS 0x801bfa40 (Mc_name_tmpl) */
UCHAR Mc_name_tmpl[15] = {
	0x42, 0x41, 0x53, 0x4c, 0x55, 0x53, 0x2d, 0x30,
	0x30, 0x37, 0x34, 0x38, 0x30, 0x30, 0x00
};

/* DS 0x801bfad8 (Card_font) */
UCHAR Card_font[4] = {
	0x26, 0x00, 0x00, 0x00
};

/* DS 0x801bfb40 (Mc_icon_801bfb40) */
UCHAR Mc_icon_801bfb40[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x62, 0x82, 0x8c, 0x82, 0x81, 0x82, 0x89,
	0x82, 0x92, 0x82, 0x85, 0x82, 0x4f, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfb68 (Mc_icon_801bfb68) */
UCHAR Mc_icon_801bfb68[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x6b, 0x82, 0x85, 0x82, 0x8f, 0x82, 0x8e,
	0x81, 0x40, 0x81, 0x40, 0x82, 0x4f, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfb90 (Mc_icon_801bfb90) */
UCHAR Mc_icon_801bfb90[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x62, 0x82, 0x88, 0x82, 0x92, 0x82, 0x89,
	0x82, 0x93, 0x81, 0x40, 0x82, 0x4f, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfbb8 (Mc_icon_801bfbb8) */
UCHAR Mc_icon_801bfbb8[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x60, 0x82, 0x84, 0x82, 0x81, 0x81, 0x40,
	0x81, 0x40, 0x81, 0x40, 0x82, 0x4f, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfbe0 (Mc_icon_801bfbe0) */
UCHAR Mc_icon_801bfbe0[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x81, 0x48, 0x81, 0x48, 0x81, 0x48, 0x81, 0x48,
	0x81, 0x48, 0x81, 0x48, 0x81, 0x48, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfc08 (Mc_icon_801bfc08) */
UCHAR Mc_icon_801bfc08[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x62, 0x82, 0x8c, 0x82, 0x81, 0x82, 0x89,
	0x82, 0x92, 0x82, 0x85, 0x82, 0x61, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfc30 (Mc_icon_801bfc30) */
UCHAR Mc_icon_801bfc30[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x6b, 0x82, 0x85, 0x82, 0x8f, 0x82, 0x8e,
	0x82, 0x61, 0x81, 0x40, 0x81, 0x40, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfc58 (Mc_icon_801bfc58) */
UCHAR Mc_icon_801bfc58[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x62, 0x82, 0x8c, 0x82, 0x81, 0x82, 0x89,
	0x82, 0x92, 0x82, 0x85, 0x82, 0x60, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfc80 (Mc_icon_801bfc80) */
UCHAR Mc_icon_801bfc80[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x6b, 0x82, 0x85, 0x82, 0x8f, 0x82, 0x8e,
	0x82, 0x60, 0x81, 0x40, 0x81, 0x40, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfca8 (Mc_icon_801bfca8) */
UCHAR Mc_icon_801bfca8[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x67, 0x82, 0x95, 0x82, 0x8e, 0x82, 0x8b,
	0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfcd0 (Mc_icon_801bfcd0) */
UCHAR Mc_icon_801bfcd0[40] = {
	0x82, 0x71, 0x82, 0x64, 0x82, 0x51, 0x81, 0x40,
	0x81, 0x40, 0x82, 0x4f, 0x82, 0x4f, 0x81, 0x44,
	0x82, 0x73, 0x82, 0x8f, 0x82, 0x86, 0x82, 0x95,
	0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x5e,
	0x82, 0x4f, 0x82, 0x4f, 0x81, 0x5e, 0x00, 0x00
};

/* DS 0x801bfcf8 (Mc_icon_801bfcf8) */
UCHAR Mc_icon_801bfcf8[4] = {
	0x81, 0x40, 0x00, 0x00
};

/* DS 0x801bfcfc (Mc_icon_801bfcfc) */
UCHAR Mc_icon_801bfcfc[4] = {
	0x82, 0x64, 0x00, 0x00
};

/* DS 0x801bfd00 (Mc_icon_801bfd00) */
UCHAR Mc_icon_801bfd00[4] = {
	0x82, 0x71, 0x00, 0x00
};

/* DS 0x801bfd04 (Mc_icon_801bfd04) */
UCHAR Mc_icon_801bfd04[4] = {
	0x82, 0x74, 0x00, 0x00
};

/* DS 0x801bfd08 (Mc_icon_801bfd08) */
UCHAR Mc_icon_801bfd08[4] = {
	0x82, 0x61, 0x00, 0x00
};


/* DS 0x801c3e1c (Mess_text) */
UCHAR Mess_text[1092] = {
	0x20, 0x4b, 0x00, 0x4a, 0x4b, 0x50, 0x00, 0x4f, 0x3d, 0x52, 0x41, 0xfe, 0x00, 0x20, 0x4b, 0x00,
	0x4a, 0x4b, 0x50, 0x00, 0x48, 0x4b, 0x3d, 0x40, 0xfe, 0x00, 0x29, 0x41, 0x49, 0x4b, 0x4e, 0x55,
	0x00, 0x3f, 0x3d, 0x4e, 0x40, 0x00, 0x0d, 0xfe, 0x00, 0x29, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00,
	0x3f, 0x3d, 0x4e, 0x40, 0x00, 0x0e, 0xfe, 0x00, 0x20, 0x4b, 0x00, 0x4a, 0x4b, 0x50, 0xfe, 0x00,
	0x2f, 0x3d, 0x52, 0x41, 0x00, 0x4b, 0x4a, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0x00, 0x0d, 0x00, 0x4b,
	0x4e, 0x00, 0x0e, 0xfe, 0x00, 0x28, 0x4b, 0x3d, 0x40, 0x00, 0x42, 0x4e, 0x4b, 0x49, 0x00, 0x3f,
	0x3d, 0x4e, 0x40, 0x00, 0x0d, 0x00, 0x4b, 0x4e, 0x00, 0x0e, 0xfe, 0x00, 0x1f, 0x44, 0x41, 0x3f,
	0x47, 0x45, 0x4a, 0x43, 0x00, 0x49, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00, 0x3f, 0x3d, 0x4e, 0x40,
	0x38, 0x4f, 0xfe, 0x00, 0x2b, 0x52, 0x41, 0x4e, 0x53, 0x4e, 0x45, 0x50, 0x41, 0x1b, 0xfe, 0x00,
	0x35, 0x41, 0x4f, 0xfe, 0x00, 0x2a, 0x4b, 0xfe, 0x00, 0x22, 0x4b, 0x4e, 0x49, 0x3d, 0x50, 0x50,
	0x45, 0x4a, 0x43, 0x00, 0x45, 0x4f, 0x00, 0x4e, 0x41, 0x4d, 0x51, 0x45, 0x4e, 0x41, 0x40, 0xfe,
	0x00, 0x22, 0x4b, 0x4e, 0x49, 0x3d, 0x50, 0x00, 0x50, 0x44, 0x45, 0x4f, 0x00, 0x3f, 0x3d, 0x4e,
	0x40, 0xfe, 0x00, 0x20, 0x4b, 0x00, 0x4a, 0x4b, 0x50, 0x00, 0x42, 0x4b, 0x4e, 0x49, 0x3d, 0x50,
	0x00, 0x50, 0x44, 0x45, 0x4f, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0xfe, 0x00, 0x31, 0x4f, 0x41, 0x00,
	0x49, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0xfe, 0x00, 0x20, 0x4b, 0x4a,
	0x3a, 0x50, 0x00, 0x51, 0x4f, 0x41, 0x00, 0x49, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00, 0x3f, 0x3d,
	0x4e, 0x40, 0xfe, 0x00, 0x2a, 0x4b, 0x53, 0x00, 0x48, 0x4b, 0x3d, 0x40, 0x45, 0x4a, 0x43, 0x01,
	0x01, 0x01, 0xfe, 0x00, 0x2a, 0x4b, 0x00, 0x20, 0x3d, 0x50, 0x3d, 0xfe, 0x00, 0x1d, 0x4e, 0x41,
	0x00, 0x55, 0x4b, 0x51, 0x00, 0x4f, 0x51, 0x4e, 0x41, 0x00, 0x50, 0x4b, 0x00, 0x41, 0x54, 0x45,
	0x50, 0xfe, 0x00, 0x53, 0x45, 0x50, 0x44, 0x4b, 0x51, 0x50, 0x00, 0x4f, 0x3d, 0x52, 0x45, 0x4a,
	0x43, 0x1b, 0xfe, 0x00, 0x22, 0x3d, 0x45, 0x48, 0x41, 0x40, 0x00, 0x50, 0x4b, 0x00, 0x42, 0x4b,
	0x4e, 0x49, 0x3d, 0x50, 0x00, 0x50, 0x44, 0x41, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0xfe, 0x00, 0x1d,
	0x3f, 0x3f, 0x41, 0x4f, 0x4f, 0x00, 0x41, 0x4e, 0x4e, 0x4b, 0x4e, 0xfe, 0x00, 0x2a, 0x4b, 0x00,
	0x41, 0x49, 0x4c, 0x50, 0x55, 0x00, 0x3e, 0x48, 0x4b, 0x3f, 0x47, 0xfe, 0x00, 0x30, 0x44, 0x41,
	0x4e, 0x41, 0x00, 0x3d, 0x4e, 0x41, 0x00, 0x4a, 0x4b, 0x00, 0x42, 0x45, 0x48, 0x41, 0x4f, 0xfe,
	0x00, 0x29, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0x00, 0x45, 0x4f, 0x00,
	0x4a, 0x4b, 0x50, 0x00, 0x45, 0x4a, 0x4f, 0x41, 0x4e, 0x50, 0x41, 0x40, 0xfe, 0x00, 0x22, 0x3d,
	0x45, 0x48, 0x00, 0x45, 0x4a, 0x00, 0x4f, 0x3d, 0x52, 0x41, 0xfe, 0x00, 0x22, 0x3d, 0x45, 0x48,
	0x00, 0x45, 0x4a, 0x00, 0x4f, 0x3d, 0x52, 0x41, 0xfe, 0x00, 0x30, 0x44, 0x45, 0x4f, 0x00, 0x40,
	0x3d, 0x50, 0x3d, 0x00, 0x45, 0x4f, 0x00, 0x1f, 0x48, 0x3d, 0x45, 0x4e, 0x41, 0x3a, 0x4f, 0x00,
	0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00, 0x30, 0x44, 0x45, 0x4f, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0x00,
	0x45, 0x4f, 0x00, 0x28, 0x41, 0x4b, 0x4a, 0x3a, 0x4f, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00,
	0x22, 0x4b, 0x4e, 0x49, 0x3d, 0x50, 0x50, 0x45, 0x4a, 0x43, 0x00, 0x45, 0x4f, 0x00, 0x4e, 0x41,
	0x4d, 0x51, 0x45, 0x4e, 0x41, 0x40, 0xfe, 0x00, 0x2a, 0x4b, 0x50, 0x00, 0x21, 0x34, 0x00, 0x3e,
	0x3d, 0x50, 0x50, 0x48, 0x41, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00, 0x28, 0x41, 0x4b, 0x4a,
	0x1d, 0x00, 0x00, 0xfe, 0x00, 0x1f, 0x48, 0x3d, 0x45, 0x4e, 0x41, 0x1d, 0xfe, 0x00, 0x28, 0x41,
	0x4b, 0x4a, 0x1e, 0x00, 0x00, 0xfe, 0x00, 0x1f, 0x48, 0x3d, 0x45, 0x4e, 0x41, 0x1e, 0xfe, 0x00,
	0x24, 0x51, 0x4a, 0x47, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x30, 0x4b, 0x42, 0x51, 0x00, 0x00, 0x00,
	0xfe, 0x00, 0x28, 0x41, 0x4b, 0x4a, 0x0c, 0x00, 0x00, 0xfe, 0x00, 0x1f, 0x48, 0x3d, 0x45, 0x4e,
	0x41, 0x0c, 0xfe, 0x00, 0x1d, 0x40, 0x3d, 0x0c, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x1f, 0x44, 0x4e,
	0x45, 0x4f, 0x0c, 0x00, 0xfe, 0x00, 0x1b, 0x1b, 0x1b, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x38,
	0x0c, 0x0c, 0x38, 0x33, 0x3d, 0x45, 0x50, 0x45, 0x4a, 0x43, 0x00, 0x2e, 0x4b, 0x4b, 0x49, 0xfe,
	0x00, 0x24, 0x3d, 0x48, 0x48, 0xfe, 0x00, 0x20, 0x3d, 0x4e, 0x47, 0x00, 0x2e, 0x4b, 0x4b, 0x49,
	0xfe, 0x00, 0x2f, 0x50, 0x4b, 0x4e, 0x41, 0x4e, 0x4b, 0x4b, 0x49, 0x00, 0x28, 0xfe, 0x00, 0x2f,
	0x50, 0x4b, 0x4e, 0x41, 0x4e, 0x4b, 0x4b, 0x49, 0x00, 0x2e, 0xfe, 0x00, 0x1f, 0x4b, 0x4a, 0x50,
	0x4e, 0x4b, 0x48, 0x00, 0x2e, 0x4b, 0x4b, 0x49, 0x0d, 0xfe, 0x00, 0x1f, 0x4b, 0x4a, 0x50, 0x4e,
	0x4b, 0x48, 0x00, 0x2e, 0x4b, 0x4b, 0x49, 0x0e, 0xfe, 0x00, 0x33, 0x3d, 0x50, 0x41, 0x4e, 0x00,
	0x2c, 0x4b, 0x4b, 0x48, 0xfe, 0x00, 0x22, 0x3d, 0x3f, 0x50, 0x4b, 0x4e, 0x55, 0xfe, 0x00, 0x2c,
	0x51, 0x49, 0x4c, 0x00, 0x2e, 0x4b, 0x4b, 0x49, 0xfe, 0x00, 0x2f, 0x41, 0x3f, 0x51, 0x4e, 0x45,
	0x50, 0x55, 0x00, 0x2e, 0x4b, 0x4b, 0x49, 0xfe, 0x00, 0x29, 0x4b, 0x4a, 0x45, 0x50, 0x4b, 0x4e,
	0x00, 0x2e, 0x4b, 0x4b, 0x49, 0xfe, 0x00, 0x23, 0x51, 0x4a, 0x00, 0x2f, 0x44, 0x4b, 0x4c, 0xfe,
	0x00, 0x1f, 0x3d, 0x3e, 0x45, 0x4a, 0xfe, 0x00, 0x30, 0x4e, 0x3d, 0x45, 0x4a, 0xfe, 0x00, 0x2f,
	0x3f, 0x41, 0x4a, 0x3d, 0x4e, 0x45, 0x4b, 0x00, 0x0d, 0x4f, 0x50, 0xfe, 0x00, 0x2f, 0x3f, 0x41,
	0x4a, 0x3d, 0x4e, 0x45, 0x4b, 0x00, 0x0e, 0x4a, 0x40, 0xfe, 0x00, 0x10, 0x50, 0x44, 0x00, 0x2f,
	0x51, 0x4e, 0x52, 0x45, 0x52, 0x4b, 0x4e, 0xfe, 0x00, 0x21, 0x34, 0x00, 0x1e, 0x3d, 0x50, 0x50,
	0x48, 0x41, 0x00, 0x0d, 0xfe, 0x00, 0x21, 0x34, 0x00, 0x1e, 0x3d, 0x50, 0x50, 0x48, 0x41, 0x00,
	0x0e, 0xfe, 0x00, 0x21, 0x34, 0x00, 0x1e, 0x3d, 0x50, 0x50, 0x48, 0x41, 0x00, 0x0f, 0xfe, 0x00,
	0xee, 0x16, 0x16, 0xfe, 0x00, 0x16, 0xee, 0x17, 0xfe, 0x00, 0x2b, 0x4e, 0x45, 0x43, 0x45, 0x4a,
	0x3d, 0x48, 0xfe, 0x00, 0x1d, 0x4e, 0x4e, 0x3d, 0x4a, 0x43, 0x41, 0xfe, 0x00, 0x2b, 0x4e, 0x45,
	0x43, 0x45, 0x4a, 0x3d, 0x48, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00, 0x1d, 0x4e, 0x4e, 0x3d,
	0x4a, 0x43, 0x41, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00, 0x2a, 0x4b, 0x4e, 0x49, 0x3d, 0x48,
	0x00, 0x3d, 0x4a, 0x40, 0x00, 0x21, 0x3d, 0x4f, 0x55, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00,
	0x42, 0x4b, 0x4e, 0x00, 0x2b, 0x4e, 0x45, 0x43, 0x45, 0x4a, 0x3d, 0x48, 0x00, 0x43, 0x3d, 0x49,
	0x41, 0xfe, 0x00, 0x2e, 0x4b, 0x4b, 0x47, 0x45, 0x41, 0x00, 0x40, 0x3d, 0x50, 0x3d, 0xfe, 0x00,
	0x42, 0x4b, 0x4e, 0x00, 0x1d, 0x4e, 0x4e, 0x3d, 0x4a, 0x43, 0x41, 0x00, 0x43, 0x3d, 0x49, 0x41,
	0xfe, 0x00, 0x2a, 0x4b, 0x53, 0x00, 0x2f, 0x3d, 0x52, 0x45, 0x4a, 0x43, 0x01, 0x01, 0x01, 0xfe,
	0x00, 0x20, 0x4b, 0x4a, 0x3a, 0x50, 0x00, 0x4c, 0x51, 0x48, 0x48, 0x00, 0x4b, 0x51, 0x50, 0x00,
	0x50, 0x44, 0x41, 0x00, 0x49, 0x41, 0x49, 0x4b, 0x4e, 0x55, 0x00, 0x3f, 0x3d, 0x4e, 0x40, 0xfe,
	0x00, 0x00, 0x00, 0x00
};

/* DS 0x801c4260 (Mess_offs) */
USHORT Mess_offs[76] = {
	0x0000, 0x000d, 0x001a, 0x0029, 0x0038, 0x0040, 0x0055, 0x006c,
	0x0084, 0x0090, 0x0095, 0x0099, 0x00b1, 0x00c3, 0x00dc, 0x00ed,
	0x0104, 0x0114, 0x011d, 0x0133, 0x0144, 0x015f, 0x016d, 0x017d,
	0x0191, 0x01ae, 0x01bc, 0x01ca, 0x01e6, 0x0200, 0x0218, 0x022c,
	0x0235, 0x023e, 0x0247, 0x0250, 0x0259, 0x0262, 0x026b, 0x0274,
	0x027d, 0x0286, 0x028f, 0x0293, 0x02a1, 0x02a7, 0x02b2, 0x02bf,
	0x02cc, 0x02db, 0x02ea, 0x02f6, 0x02ff, 0x030a, 0x0319, 0x0327,
	0x0331, 0x0338, 0x033f, 0x034d, 0x035b, 0x0369, 0x0376, 0x0383,
	0x0390, 0x0395, 0x039a, 0x03a4, 0x03ad, 0x03bc, 0x03ca, 0x03e0,
	0x03f3, 0x0400, 0x0412, 0x0421
};

/* DS 0x801c431c (pMwork) */
MWORK *pMwork = (MWORK *)0x801c45a8;

/* DS 0x801c4320 (Mc_area_name) */
UCHAR Mc_area_name[21][29] = {
	{ 0x82, 0x76, 0x82, 0x81, 0x82, 0x89, 0x82, 0x94, 0x82, 0x89, 0x82, 0x8e, 0x82, 0x87, 0x81, 0x40,
	  0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f, 0x82, 0x8d, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x67, 0x82, 0x81, 0x82, 0x8c, 0x82, 0x8c, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
	  0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x63, 0x82, 0x81, 0x82, 0x92, 0x82, 0x8b, 0x81, 0x40, 0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f,
	  0x82, 0x8d, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x72, 0x82, 0x94, 0x82, 0x8f, 0x82, 0x92, 0x82, 0x85, 0x82, 0x92, 0x82, 0x8f, 0x82, 0x8f,
	  0x82, 0x8d, 0x81, 0x40, 0x82, 0x6b, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x72, 0x82, 0x94, 0x82, 0x8f, 0x82, 0x92, 0x82, 0x85, 0x82, 0x92, 0x82, 0x8f, 0x82, 0x8f,
	  0x82, 0x8d, 0x81, 0x40, 0x82, 0x71, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x62, 0x82, 0x8f, 0x82, 0x8e, 0x82, 0x94, 0x82, 0x92, 0x82, 0x8f, 0x82, 0x8c, 0x81, 0x40,
	  0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f, 0x82, 0x8d, 0x82, 0x50, 0x00, 0x00, 0x00 },
	{ 0x82, 0x62, 0x82, 0x8f, 0x82, 0x8e, 0x82, 0x94, 0x82, 0x92, 0x82, 0x8f, 0x82, 0x8c, 0x81, 0x40,
	  0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f, 0x82, 0x8d, 0x82, 0x51, 0x00, 0x00, 0x00 },
	{ 0x82, 0x76, 0x82, 0x81, 0x82, 0x94, 0x82, 0x85, 0x82, 0x92, 0x81, 0x40, 0x82, 0x6f, 0x82, 0x8f,
	  0x82, 0x8f, 0x82, 0x8c, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x65, 0x82, 0x81, 0x82, 0x83, 0x82, 0x94, 0x82, 0x8f, 0x82, 0x92, 0x82, 0x99, 0x81, 0x40,
	  0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x6f, 0x82, 0x95, 0x82, 0x8d, 0x82, 0x90, 0x81, 0x40, 0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f,
	  0x82, 0x8d, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x72, 0x82, 0x85, 0x82, 0x83, 0x82, 0x95, 0x82, 0x92, 0x82, 0x89, 0x82, 0x94, 0x82, 0x99,
	  0x81, 0x40, 0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f, 0x82, 0x8d, 0x00, 0x00, 0x00 },
	{ 0x82, 0x6c, 0x82, 0x8f, 0x82, 0x8e, 0x82, 0x89, 0x82, 0x94, 0x82, 0x8f, 0x82, 0x92, 0x81, 0x40,
	  0x82, 0x71, 0x82, 0x8f, 0x82, 0x8f, 0x82, 0x8d, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x66, 0x82, 0x95, 0x82, 0x8e, 0x81, 0x40, 0x82, 0x72, 0x82, 0x88, 0x82, 0x8f, 0x82, 0x90,
	  0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x62, 0x82, 0x81, 0x82, 0x82, 0x82, 0x89, 0x82, 0x8e, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
	  0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x73, 0x82, 0x92, 0x82, 0x81, 0x82, 0x89, 0x82, 0x8e, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
	  0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x72, 0x82, 0x83, 0x82, 0x85, 0x82, 0x8e, 0x82, 0x81, 0x82, 0x92, 0x82, 0x89, 0x82, 0x8f,
	  0x81, 0x40, 0x82, 0x50, 0x82, 0x93, 0x82, 0x94, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x72, 0x82, 0x83, 0x82, 0x85, 0x82, 0x8e, 0x82, 0x81, 0x82, 0x92, 0x82, 0x89, 0x82, 0x8f,
	  0x81, 0x40, 0x82, 0x51, 0x82, 0x8e, 0x82, 0x84, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x53, 0x82, 0x94, 0x82, 0x88, 0x81, 0x40, 0x82, 0x72, 0x82, 0x95, 0x82, 0x92, 0x82, 0x96,
	  0x82, 0x89, 0x82, 0x96, 0x82, 0x8f, 0x82, 0x92, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x64, 0x82, 0x77, 0x81, 0x40, 0x82, 0x61, 0x82, 0x81, 0x82, 0x94, 0x82, 0x94, 0x82, 0x8c,
	  0x82, 0x85, 0x81, 0x40, 0x82, 0x50, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x64, 0x82, 0x77, 0x81, 0x40, 0x82, 0x61, 0x82, 0x81, 0x82, 0x94, 0x82, 0x94, 0x82, 0x8c,
	  0x82, 0x85, 0x81, 0x40, 0x82, 0x51, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 },
	{ 0x82, 0x64, 0x82, 0x77, 0x81, 0x40, 0x82, 0x61, 0x82, 0x81, 0x82, 0x94, 0x82, 0x94, 0x82, 0x8c,
	  0x82, 0x85, 0x81, 0x40, 0x82, 0x52, 0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00 }
};

/* DS 0x801c4584 (Mess_ctrl) */
UCHAR Mess_ctrl[6] = {
	0xfe, 0xfd, 0xfb, 0xfa, 0xf9, 0xf8
};

/* DS 0x801c458c (Mc_slot_ofs) */
UCHAR Mc_slot_ofs[16] = {
	0x04, 0x06, 0x04, 0x06, 0x04, 0x06, 0x04, 0x06,
	0x04, 0x06, 0x00, 0x05, 0x00, 0x00, 0x03, 0x00
};

/* DS 0x801c45a0 (Mc_prev_slot) */
UCHAR Mc_prev_slot = 0x00;

/* ------------------------------------------------------------------ */
/* Mem_card — MEMCARD overlay task entry                                */
/* ------------------------------------------------------------------ */
/* DS 0x801bfd14 (Mem_card). If Task_status(0) returns 1, the task suspends level 0, runs Card_access
 * and signals level 0. Otherwise, if Task_status(2) returns 1, it suspends level 2, runs Card_access,
 * clears Main.Bg_chg_flg and signals level 2. If neither status is 1 it returns without signalling.
 * Both signalling paths end with Task_exit. */
void Mem_card(void)
{
	ULONG Level;

	if (Task_status(0) == 1) {
		Task_suspend(0);
		Card_access();
		Level = 0;
	} else {
		if (Task_status(2) != 1) {
			return;
		}
		Task_suspend(2);
		Card_access();
		Main.Bg_chg_flg = 0;
		Level = 2;
	}
	Task_signal(Level);
	Task_exit();
}

/* ------------------------------------------------------------------ */
/* Static helpers (no PSYQ symbol records; Ghidra MEMCARD::801bfd9c */
/* and MEMCARD::801bfe5c)                                           */
/* ------------------------------------------------------------------ */

/* Walks message Msg: sums glyph advances (msg_space width for codes
 * below 0x57 when Mode != 0, otherwise a fixed 14 px advance). Control
 * codes listed in Mess_ctrl skip their operand bytes; code 0 ends the
 * string. Returns the left X origin that centres the text in 320 px. */
/* DS 0x801bfd9c (Mess_center_x) */
static int Mess_center_x(int Msg, int Mode)
{
	int Width;
	UINT Ch;
	UINT Ctrl;
	UCHAR *pChar;

	Width = 0;
	pChar = &Mess_text[Mess_offs[Msg]];
	for (;;) {
		while ((Ch = *pChar) < 0xEE) {
			if (Mode == 0 || Ch > 0x56) {
				Width += 0xE;
			} else {
				Width += (signed char)msg_space[Ch * 2 + 1];
			}
			pChar++;
		}
		for (Ctrl = 0; Ctrl < 6; Ctrl++) {
			if (*pChar == Mess_ctrl[Ctrl]) {
				if (Ctrl == 0) {
					return (0x140 - Width) >> 1;
				}
				pChar += 2;
				break;
			}
		}
		if (Ctrl == 6) {
			pChar++;
		}
	}
}

/* Return pixel width of message Msg (same walk as Mess_center_x). */
/* DS 0x801bfe5c (Mess_width) */
static int Mess_width(int Msg)
{
	int Width;
	UINT Ch;
	UINT Ctrl;
	UCHAR *pChar;

	Width = 0;
	pChar = &Mess_text[Mess_offs[Msg]];
	for (;;) {
		while ((Ch = *pChar) < 0xEE) {
			if (Ch < 0x57) {
				Width += (signed char)msg_space[Ch * 2 + 1];
			} else {
				Width += 0xE;
			}
			pChar++;
		}
		for (Ctrl = 0; Ctrl < 6; Ctrl++) {
			if (*pChar == Mess_ctrl[Ctrl]) {
				if (Ctrl == 0) {
					return (short)Width;
				}
				pChar += 2;
				break;
			}
		}
		if (Ctrl == 6) {
			pChar++;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Card_access — main memory card menu state machine                    */
/* ------------------------------------------------------------------ */
/* DS 0x801bff10 (Card_access). The frame is 640 bytes here against 672 in retail. Callees
 * and call counts are the same as in retail. The array area (File_info, msg_buf, item_bak,
 * title_bak) ends at 0x238 in both builds; the frame difference is in the spill area
 * after it: retail keeps six byte-wide locals (lbu/sb at 0x238..0x260) and the
 * item_bak pointer (0x270) in 8-byte spaced slots, this compiler spills them
 * as 4-byte words. */
void Card_access(void)
{
	/* retail reserves 40 more bytes of locals (Card_access frame 672); never read */
	volatile int Rsv[10];
	MWORK *pMc;
	UCHAR sel_bak;
	UCHAR item_num;
	BOOL ready;
	short pos;
	long item_idx;
	ULONG u_tmp;
	int i_tmp;
	UINT res;
	char *pattern;
	char c_tmp;
	int Rno;
	UCHAR cur_u8;
	UINT cursor;
	UINT err_code;
	UINT blink_cnt;
	CFILE_INFO File_info;
	char *Title_buf;
	char msg_buf [72];
	UCHAR item_bak [48];
	char title_bak [24];
	char rpt_cnt;
	UCHAR cursor_max;
	UCHAR cursor_prev;
	UCHAR equip_bak;
	UCHAR Mode;
	UCHAR sel_cursor;
	UCHAR *pItem_bak;

	pMc = pMwork;
	Title_buf = (char *)File_info.title;
	Rno = 0;
	blink_cnt = 0;
	cursor = 0;
	Mode = (UCHAR)(G.System_flg >> 10) & 1;
	G.System_flg &= ~SYS_CARD_LOAD;
	Cardaccess_init(Mode);
	pMc->Pad_stop = 1;
	pMc->Card_err_flg = 0;
	pMc->Card_ready = 0;
	do {
		pMc->Card_poll_req = 0;
		if (pMc->Pad_stop == 0) {
			c_tmp = 6;
			if (((G.Padd1_trg & 0x5000) == 0) && (c_tmp = 2, rpt_cnt != 0)) {
				rpt_cnt = rpt_cnt + -1;
			} else {
				rpt_cnt = c_tmp;
				if (Rno == 2) {
					if ((G.Padd1 & 0x1000) != 0) {
						blink_cnt = 0;
						cursor--;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
					}
					if ((G.Padd1 & 0x4000) != 0) {
						blink_cnt = 0;
						cursor++;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
					}
					if ((int)(cursor << 0x18) < 0) {
						cursor = (UINT)cursor_max;
					}
					if ((UINT)cursor_max < (cursor & 0xff)) {
						cursor = 0;
					}
cursor_clamp:
					res = (UINT)cursor_max;
				} else {
					if (Rno == 7) {
						if (((G.Padd1 & 0x1000) == 0) || ((cursor & 0xff) != 0)) {
slot_down_ck:
							if (((G.Padd1 & 0x4000) == 0) || ((cursor & 0xff) != 4))
								goto cursor_step;
							if ((UCHAR)pMc->Scrl_ofs < 10) {
								pMc->Scrl_ofs++;
								Rno = 0x14;
							} else {
								if ((G.Padd1_trg & 0x4000) == 0) goto cursor_step;
								Rno = 0x15;
								cursor = 0;
								pMc->Scrl_ofs = 0;
								pMc->Pad_stop = 1;
							}
						} else if (pMc->Scrl_ofs == 0) {
							if ((G.Padd1_trg & 0x1000) == 0) goto slot_down_ck;
							Rno = 0x15;
							cursor = 4;
							pMc->Scrl_ofs = 10;
							pMc->Pad_stop = 1;
						} else {
							Rno = 0x13;
							pMc->Scrl_ofs = pMc->Scrl_ofs + -1;
							pMc->Scrl_slide_y = -0x14;
						}
cursor_move_se:
						blink_cnt = 0;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
						goto cursor_clamp;
					}
cursor_step:
					if (((G.Padd1 & 0x1000) != 0) && ((cursor & 0xff) != 0)) {
						blink_cnt = 0;
						cursor--;
						Snd_se_stad(0x4040000, (VECTOR *)0x0);
					}
					if ((G.Padd1 & 0x4000) == 0) goto cursor_clamp;
					res = (UINT)cursor_max;
					if ((cursor & 0xff) < res) {
						cursor++;
						goto cursor_move_se;
					}
				}
				if ((res != 0) && (res < (cursor & 0xff))) {
					cursor = (UINT)cursor_max;
				}
				pMc->Blink_phase = 0;
			}
		}
		sel_bak = sel_cursor;
		blink_cnt++;
		if ((cursor & 0xff) != (UINT)cursor_prev) {
			blink_cnt = 1;
		}
		blink_cnt &= 0x1f;
		pMc->Blink_phase = (char)(blink_cnt >> 4);
		cur_u8 = (UCHAR)cursor;
		cursor_prev = cur_u8;
		switch (Rno) {
		case SLOT_CHECK:
			res = Wslot_check();
			if (res - 1 < 2) {
				pMc->Port = (UCHAR)res & 1;
				if (pMc->Win_show_flg == 0) {
					Rno = 5;
				} else {
enter_state18:
					cursor = 0;
					blink_cnt = 0;
					Rno = 0x18;
				}
			} else {
				Rno = 2;
				if (res != 3) {
					blink_cnt = 0;
					pMc->Pad_stop = 0;
					cursor = 0;
					break;
				}
card_err_clear:
				pMc->Port = 0;
card_err_state:
				Rno = 3;
				err_code = 4;
			}
			break;
		case 1:
			cursor_max = 1;
			if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
cancel_ck:
				if ((G.Key_trg & 0x2000) != 0) goto cancel_snd;
			} else {
				if ((cursor & 0xff) == 1) goto cancel_snd;
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Rno = 0;
				cursor = 0;
				pMc->Pad_stop = cursor_max;
			}
			break;
		case SEL_SLOT:
			cursor_max = 2;
			if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0))
				goto cancel_ck;
			if ((cursor & 0xff) == 2) goto cancel_snd;
			Snd_se_stad(0x4060000, (VECTOR *)0x0);
			pMc->Port = cur_u8;
			pMc->Pad_stop = 1;
			if (pMc->Win_show_flg != 0) goto enter_state18;
			Rno = 5;
			break;
		case ERR_MESS:
			cursor_max = 0;
			cursor = 0;
			if (((err_code - 2) & 0xff) < 2) {
				pMc->Unk_11d5 = 1;
			}
			if (((G.Key_trg & 0x3000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				Rno = 1;
				pMc->Pad_stop = 0;
				pMc->Scrl_slide_y = 0;
				pMc->Scrl_ofs = 0;
				goto clear_flag_11d5;
			}
			break;
		case 4:
			Rno = 5;
			break;
		case 5:
			i_tmp = Mess_center_x(0x4b, 1);
			Rno = 6;
			Mess_print((int)(short)i_tmp, 0xb2, Mess_text + Mess_offs[7], 0x4002);
			i_tmp = Mess_center_x(0x4b, 1);
			Mess_print((int)(short)i_tmp, 0xc0, Mess_text + Mess_offs[75], 0x4002);
			Task_sleep(1);
			break;
		case 6:
			cursor_max = 5;
			pMc->Pad_stop = 0;
			err_code = Card_check(pMc->Port);
			res = err_code & 0xff;
			if (res == 0) {
				err_code = Card_format_check(pMc->Port);
				if ((err_code & 0xff) == 0) {
					err_code = Check_card(pMc->F_num);
					Card_search(pMc->Port, &File_info, Mc_search_pat);
					pMc->Card_total_size = File_info.total_size;
				}
				res = err_code & 0xff;
				if (res != 1) {
					if (res < 2) {
						if (res == 0) {
							if ((Mode == 1) && (Rno = 3, pMc->File_info.num == 0)) {
								err_code = 3;
							} else if ((Mode == 0) && ((pMc->File_info.num == 0 && (Rno = 3, pMc->Card_total_size == 0x1e000)))) {
								err_code = 2;
							} else {
								blink_cnt = 0;
								Rno = 7;
								cursor = G.Save_cursor & 0xf;
								pMc->Scrl_ofs = G.Save_cursor >> 4;
							}
						}
					} else {
						if (res != 2) goto res3_ck;
						Rno = 3;
						if (Mode == 1) {
							err_code = 9;
						} else {
							cursor = 1;
							Rno = 0xf;
						}
					}
					break;
				}
				goto card_err_state;
			}
			if (res != 0) {
				if (res < 3) goto card_err_state;
res3_ck:
				if (res == 3) {
					Rno = 3;
					err_code = 1;
				}
			}
			break;
		case 7:
			cursor_max = 4;
			pMc->Unk_11d5 = 1;
			G.Save_cursor = cur_u8 | pMc->Scrl_ofs << 4;
			if (((G.Key_trg & 0x1000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				if (pMwork->Ident_sel == 0) {
					c_tmp = pMc->Port;
					pattern = "BASLUS-00421*";
				} else {
					c_tmp = pMc->Port;
					pattern = "BASLUS-00748*";
				}
				sel_cursor = cur_u8;
				Card_search(c_tmp, &File_info, pattern);
				if (File_info.num == 0) {
					VSync(0);
					err_code = Card_check(pMc->Port);
					Rno = 3;
					if ((err_code & 0xff) != 0) {
						err_code = 4;
						goto clear_flag_11d5;
					}
				}
				Rno = 8;
				pMc->Pad_stop = 1;
			} else if ((G.Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				blink_cnt = 0;
				if (pMc->Win_show_flg == 0) {
					Rno = 0x16;
				} else {
					cursor = (UINT)(UCHAR)pMc->Ident_sel;
					Rno = 0x18;
				}
			} else {
				ready = Card_poll(pMc->Port);
				if (ready == 0) break;
				Rno = 3;
				err_code = 4;
				pMc->Port = 0;
			}
clear_flag_11d5:
			pMc->Unk_11d5 = 0;
			break;
		case 8:
			cursor = (UINT)sel_cursor;
			if (Mode == 0) {
				strcpy(title_bak, Title_buf + (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16);
				cur_u8 = 0;
				do {
					res = (UINT)cur_u8;
					cur_u8++;
					Title_buf[res + (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16] = 0;
				} while (cur_u8 < 0x16);
				if (pMwork->Ident_sel == 0) {
					Rno = (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16;
					strcpy(Title_buf + Rno, "BASLUS-0042100");
				} else {
					Rno = (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16;
					*(ULONG *)(Title_buf + Rno) = *(ULONG *)&Mc_name_tmpl[0];
					*(ULONG *)(Title_buf + 4 + Rno) = *(ULONG *)&Mc_name_tmpl[4];
					*(ULONG *)(Title_buf + 8 + Rno) = *(ULONG *)&Mc_name_tmpl[8];
					Title_buf[12 + Rno]     = Mc_name_tmpl[12];
					Title_buf[12 + Rno + 1] = Mc_name_tmpl[13];
					Title_buf[12 + Rno + 2] = Mc_name_tmpl[14];
				}
				Rno = (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16;
				Title_buf[12 + Rno] = Title_buf[12 + Rno] + (char)((cursor + (UCHAR)pMc->Scrl_ofs) / 10);
				res = cursor + (UCHAR)pMc->Scrl_ofs;
				Title_buf[12 + res * 0x16 + 1] =
					Title_buf[12 + res * 0x16 + 1] + (char)res + (char)(res / 10) * -10;
				Title_buf[12 + (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16 + 2] = 0;
				Rno = 0x12;
				if ((UCHAR)pMc->F_num[cursor + (UCHAR)pMc->Scrl_ofs] < 0xf) {
					pMc->Sel_row_bk = sel_bak;
					cursor = 1;
					pMc->Pad_stop = 0;
				} else {
					Rno = 9;
					if (pMc->Card_total_size == 0x1e000) {
						err_code = 2;
						Rno = 0x11;
						pMc->Sel_row_bk = sel_bak;
					}
				}
				break;
			}
			Rno = (cursor + (UCHAR)pMc->Scrl_ofs) * 0x80;
			if (pMc->F_num[cursor + (UCHAR)pMc->Scrl_ofs] == 15) {
				err_code = 3;
slot_err_show:
				Rno = 0x11;
				pMc->Pad_stop = 0;
				pMc->Sel_row_bk = sel_cursor;
				break;
			}
			if ((G.System_flg & SYS_EX_BATTLE) == 0) {
				if ((((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + Rno))->Status_bak & 0x4000) == 0) && (1 < (UINT)(((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + Rno))->Pl_id - 0xc))) && ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + Rno))->Pl_id & 1) != 0)) {
					err_code = 7;
					goto slot_err_show;
				}
			} else if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + Rno))->Status_bak & 0x4000) == 0) {
				err_code = 10;
				goto slot_err_show;
			}
			i_tmp = Mess_center_x(0x4b, 1);
			Rno = 0xd;
			Mess_print((int)(short)i_tmp, 0xba, Mess_text + Mess_offs[16], 0x4002);
			i_tmp = Mess_center_x(0x4b, 1);
			pos = (short)i_tmp;
			u_tmp = 0xca;
			goto msg_line2_print;
		case 9:
			Rno = Mess_center_x(0x4b, 1);
			pos = (short)Rno;
			Rno = 10;
			goto msg_pair_print;
		case 10:
			Rno = Mess_center_x(0x4b, 1);
			pos = (short)Rno;
			Rno = 0xb;
msg_pair_print:
			Mess_print((int)pos, 0xb2, Mess_text + Mess_offs[74], 0x4002);
			i_tmp = Mess_center_x(0x4b, 1);
			pos = (short)i_tmp;
			u_tmp = 0xc0;
msg_line2_print:
			Mess_print((int)pos, u_tmp, Mess_text + Mess_offs[75], 0x4002);
			break;
		case 0xb:
			pItem_bak = item_bak;
			Memcpy(pItem_bak, G.SaveData.Item_work, 0x2c);
			equip_bak = G.SaveData.Equip_no;
			item_idx = Search_item_id(0x1e);
			sel_bak = sel_cursor;
			cursor = (UINT)sel_cursor;
			item_num = G.SaveData.Item_work[item_idx].Num + 0xff;
			G.SaveData.Item_work[item_idx].Num = item_num;
			if (item_num == 0) {
				G.SaveData.Item_work[item_idx].Id   = 0;
				G.SaveData.Item_work[item_idx].Size = 0;
				Sort_item();
			}
			G.SaveData.Save_cnt++;
			Save_push();
			Memcpy(&pMc->Save_info[cursor + (UCHAR)pMc->Scrl_ofs], &G.SaveData.Game_play_time, 0x80);
			Card_mess_make(msg_buf, (ULONG)&G.SaveData,sel_bak + pMc->Scrl_ofs);
			err_code = Card_write(&G.SaveData, pMc->Port, Title_buf + (cursor + (UCHAR)pMc->Scrl_ofs) * 0x16, msg_buf, 0x800);
			if ((err_code & 0xff) == 0) {
				pMc->F_num[cursor + (UCHAR)pMc->Scrl_ofs] = 0;
				pMc->Msg_idx    = 0xff;
				pMc->Anim_step = 0;
				Rno = 0xc;
				pMc->Anim_wait = Mc_wait_tbl[0] >> 1;
				goto save_wait_tick;
			}
			err_code = 5;
			G.SaveData.Save_cnt--;
			Memcpy(G.SaveData.Item_work, pItem_bak, 0x2c);
			Rno = 3;
			G.SaveData.Equip_no = equip_bak;
			pMc->Pad_stop = 0;
			break;
		case 0xc:
save_wait_tick:
			c_tmp = pMc->Anim_wait;
			pMc->Anim_wait = c_tmp + -1;
			if ((char)(c_tmp + -1) == 0) {
				c_tmp = pMc->Msg_idx;
				pMc->Msg_idx = c_tmp + 1U;
				if ((int)((UINT)(UCHAR)(c_tmp + 1U) << 0x18) < 0)
					goto exit_card;
				c_tmp = pMc->Anim_step;
				pMc->Anim_step = c_tmp + 1U;
				pMc->Anim_wait = Mc_wait_tbl[(UCHAR)(c_tmp + 1U)] >> 1;
			}
			break;
		case 0xd:
			Rno = 0xe;
			break;
		case LOAD:
			cursor = (UINT)sel_cursor;
			res = Card_read(&pMc->Card_save, pMc->File_info.title[(UINT)(UCHAR)pMc->F_num[cursor + (UCHAR)pMc->Scrl_ofs]], 0x800, 0);
			err_code = 6;
			if ((res & 0xff) == 0) {
				memcpy16((ULONG *)&G.SaveData, (ULONG *)&pMc->Card_save, 0x798);
				if ((UINT)(G.SaveData.Pl_id - 0xc) < 2) {
					cursor = G.System_flg & ~SYS_TOFU_SURVIVOR;
					G.System_flg = cursor | SYS_TIMER;
					G.Next_cut_no = (UCHAR)G.SaveData.Cut_no;
					err_code = 1;
					if (G.SaveData.Pl_id == 0xd) {
						G.System_flg = cursor | SYS_TOFU_SURVIVOR | SYS_TIMER;
					}
				} else {
					G.Status_flg = G.SaveData.Status_bak;
					if ((G.SaveData.Status_bak & 0x4000) != 0) {
						G.ExData[3] = G.SaveData.Pl_id;
						G.ExData[1] = (UCHAR)G.SaveData.Dummy[1];
						G.ExData[2] = (UCHAR)G.SaveData.Sce_timer;
						Memcpy(G.ExData + 4, G.SaveData.ExData3 + 8, 0x3c);
					}
					if ((G.System_flg & SYS_EX_BATTLE) == 0) {
						if ((G.Status_flg & STAT_EX_SAVE) == 0) {
load_pop_exit:
							Load_pop();
exit_card:
							Cardaccess_exit((UINT)Mode);
							return;
						}
					} else {
						u_tmp = Flg_ck(G.SaveData.Common_flg, 0xfe);
						if (u_tmp != 0) goto load_pop_exit;
					}
					G.System_flg |= SYS_EX_BATTLE;
					G.Next_cut_no = (UCHAR)G.SaveData.Cut_no;
					err_code = 1;
				}
exit_check:
				if ((Mode != 0) || (Rno = 0x17, (G.System_flg & SYS_ENDING) == 0)) {
					if (Mode == 1) {
						G.System_flg |= SYS_CARD_LOAD;
					}
					goto exit_card;
				}
				cursor = 1;
			} else {
				Rno = 0x11;
				pMc->Sel_row_bk = sel_bak;
				pMc->Pad_stop = 0;
			}
			break;
		case 0xf:
			cursor_max = 1;
			if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
				if ((G.Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					Rno = 0;
					cursor = 0;
					pMc->Pad_stop = cursor_max;
				}
				goto card_poll_wait;
			}
			if ((cursor & 0xff) == 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Rno = 0x10;
			} else {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				cursor = 0;
				Rno = 1;
			}
			break;
		case CARD_CHECK:
			res = Card_check(pMc->Port);
			if ((res & 0xff) == 0) {
				err_code = Card_format_check(pMc->Port);
				if ((err_code & 0xff) == 2) {
					res = Card_format(pMc->Port);
					Rno = 5;
					if (res == 0) {
						G.Save_cursor = 0;
						pMc->Pad_stop = 1;
						break;
					}
				}
				err_code = 0;
				Rno = 3;
			} else {
				err_code = (UINT)((res & 0xff) == 1) << 2;
				cursor = 0;
				Rno = 3;
			}
			break;
		case 0x11:
			cursor = 0;
			cursor_max = 0;
			if (((G.Key_trg & 0x3000) != 0) || ((G.Padd_trg & 0x800) != 0)) {
				pMc->Pad_stop = 0;
				Snd_se_stad(0x4040000, (VECTOR *)0x0);
				Rno = 7;
				if ((err_code & 0xff) == 2) {
					Rno = 1;
					pMc->Scrl_slide_y = 0;
					pMc->Scrl_ofs = 0;
				} else {
					cursor_max = pMc->Sel_row_bk;
					cursor = (UINT)cursor_max;
					blink_cnt = 0;
				}
			}
			i_tmp = Mess_center_x((err_code & 0xff) + 0x14, 1);
			Mess_print((int)(short)i_tmp, 0xc4, Mess_text + Mess_offs[20 + (err_code & 0xff)], 0x4012);
card_poll_wait:
			ready = Card_poll(pMc->Port);
			if (ready != 0) goto card_err_clear;
			break;
		case 0x12:
			cursor_max = 1;
			if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
				if ((G.Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					Rno = 7;
					cursor_max = pMc->Sel_row_bk;
					cursor = (UINT)cursor_max;
					blink_cnt = 0;
				}
				goto card_poll_wait;
			}
			if ((cursor & 0xff) == 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Rno = 9;
				pMc->Pad_stop = cursor_max;
			} else {
				Snd_se_stad(0x4050000, (VECTOR *)0x0);
				Rno = 7;
			}
			blink_cnt = 0;
			cursor_max = pMc->Sel_row_bk;
			cursor = (UINT)cursor_max;
			break;
		case SCRL_UP:
			pos = pMc->Scrl_slide_y;
			pMc->Scrl_slide_y = pos + 0x14;
			if (pos == -0x14) {
scroll_done:
				Rno = 7;
				blink_cnt = 0;
			}
			break;
		case SCRL_DOWN:
			pos = pMc->Scrl_slide_y;
			pMc->Scrl_slide_y = pos + -0x14;
			if ((short)(pos + -0x14) == -0x14) {
				pMc->Scrl_slide_y = 0;
				goto scroll_done;
			}
			break;
		case 0x15:
			Rno = 7;
			blink_cnt = 0;
			pMc->Pad_stop = 0;
			break;
		case 0x16:
			/* Retail checks the other slot (Port ^ 1): if a card answers there the
			 * menu returns to the slot select (state 0), otherwise it leaves. */
			res = Card_check(pMc->Port ^ 1);
			cursor = 0;
			if (res != 0) {
				pMc->Card_err_flg = 1;
cancel_snd:
				if (pMc->Card_err_flg == 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
				}
				goto exit_check;
			}
			Rno = 0;
			break;
		case 0x17:
			cursor_max = 1;
			if ((G.Key_trg & 0x1000) != 0) {
				res = cursor & 0xff;
				cursor = 0;
				if (res == 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					goto exit_card;
				}
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Rno = 0;
			}
			if ((G.Key_trg & 0x2000) != 0) {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				Rno = 0;
			}
			break;
		case 0x18:
			pMc->Pad_stop = 0;
			cursor_max = 1;
			if (((G.Key_trg & 0x1000) == 0) && ((G.Padd_trg & 0x800) == 0)) {
				if ((G.Key_trg & 0x2000) != 0) {
					Snd_se_stad(0x4050000, (VECTOR *)0x0);
					Rno = 0x16;
					blink_cnt = 0;
					goto clear_flag_11d5;
				}
			} else {
				Snd_se_stad(0x4060000, (VECTOR *)0x0);
				pMc->Ident_sel = cur_u8;
				Rno = 5;
				pMc->Pad_stop = cursor_max;
			}
		}
		Card_mess_disp(Rno, (UINT)Mode, cursor & 0xff, err_code & 0xff);
		pMc->Card_ready = 0;
		if (pMc->Card_poll_req != 0) {
			i_tmp = Get_card_event();
			if (i_tmp == 1) {
				pMc->Card_ready = 1;
			}
			do {
				i_tmp = VSync(1);
			} while (i_tmp < 300);
		}
		Task_sleep(1);
	} while (1);
}

/* ------------------------------------------------------------------ */
/* Cardaccess_init — set up the memory-card screen: fades, background   */
/* load, draw areas and cursor positions. Mode 0 clears the flag that  */
/* lets the cursor move on entry. (Ghidra MEMCARD::801c143c)        */
/* ------------------------------------------------------------------ */
/* DS 0x801c143c (Cardaccess_init) */
void Cardaccess_init(char Mode)
{
	RECT Rc;
	ULONG Status;
	int Width;
	int Centre;
	short Pos;
	MWORK *pMw;

	pMw = pMwork;
	Fade_set(0x101, 0, 7, 3);
	Fade_adjust(1, 0, 0, (RECT *)0);
	Fade_set(0x102, 0, 7, 3);
	Fade_adjust(2, 0, 0, (RECT *)0);
	Fade_set(0x103, 0, 7, 1);
	Fade_adjust(3, 0, 0, (RECT *)0);
	Memclr(pMw, 0x47B);
	Main.Vloop = 2;
	Cd_read(TYPE00_PIX, Bg_img_buf, 0,(UCHAR *)"MEMORY CARD BG");
	Bg_set_mode(0, 0);
	Fade_set(0x200, -0x1800, 7, 1);
	while (Status = Fade_status(0), Status == 0) {
		Task_sleep(1);
	}

	/* Four draw-move records: Scrl_mask[row][col] */
	Width = Mess_width(0x22);
	Rc.x = (short)((0x68 - (short)Width) / 2);
	Width = Mess_width(0x22);
	Rc.w = (short)((0x68 - (short)Width) / 2) * -2 + 0x140;
	Rc.y = 0x114;
	Rc.h = 0xE;
	SetDrawMove(&pMw->Scrl_mask[0][0], &Rc, Rc.x, 0x24);
	Rc.y = 0x18C;
	Rc.h = 0x14;
	SetDrawMove(&pMw->Scrl_mask[1][0], &Rc, Rc.x, 0x9C);
	Rc.y = 0x24;
	Rc.h = 0xE;
	SetDrawMove(&pMw->Scrl_mask[0][1], &Rc, Rc.x, 0x114);
	Rc.y = 0x9C;
	Rc.h = 0x14;
	SetDrawMove(&pMw->Scrl_mask[1][1], &Rc, Rc.x, 0x18C);

	pMw->Bg_kido = 0;
	Width = Mess_width(0x22);
	pMw->Bg_rect.x = (short)((0x68 - (short)Width) / 2) - 4;
	pMw->Bg_rect.y = 0x32;
	Width = Mess_width(0x22);
	pMw->Bg_rect.w = (short)Width + 0xE2;
	pMw->Bg_rect.h = 0x6A;

	pMw->Win_kido = 0;
	Centre = Mess_center_x(0x46, 1);
	Pos = (short)Centre - 11;
	pMw->Win_rect[1].x = Pos;
	pMw->Win_rect[0].x = Pos;
	pMw->Win_rect[0].y = 0x3C;
	pMw->Win_rect[1].y = 0x88;
	pMw->Win_rect[1].w = 0xE6;
	pMw->Win_rect[0].w = 0xE6;
	pMw->Win_rect[1].h = 0x2A;
	pMw->Win_rect[0].h = 0x2A;

	pMw->Mark_step = 1;
	pMw->Win_show_flg = 1;
	pMw->Scrl_ofs = 0;
	pMw->Scrl_slide_y = 0;
	pMw->Arrow_tmr = 0;
	pMw->Pad_stop = 0;
	pMw->Unk_11d5 = 0;
	pMw->Mark_pulse = 0;
	if (Mode == 0 || (G.System_flg & SYS_EX_BATTLE) != 0) {
		pMw->Win_show_flg = 0;
	}
	pMw->Ident_sel = 0;
	if ((G.System_flg & (SYS_EX_BATTLE | SYS_ARRANGE)) != 0) {
		pMw->Ident_sel = 1;
	}
	Task_sleep(2);
}

/* ------------------------------------------------------------------ */
/* Cardaccess_exit — fade the card screen out and, unless Param is 0 or */
/* the system flag 0x200000 is set, request the Vloop and background    */
/* change. (Ghidra MEMCARD::801c1754)                               */
/* ------------------------------------------------------------------ */
/* DS 0x801c1754 (Cardaccess_exit) */
void Cardaccess_exit(int Param)
{
	ULONG Status;

	Fade_off(1);
	Fade_off(2);
	Fade_off(3);
	Fade_set(0x200, 0x1800, 7, 1);
	while (Status = Fade_status(0), Status == 0) {
		Task_sleep(1);
	}
	Fade_set(0x200, 0, 7, 1);
	Fade_adjust(0, 0x7FFF, 0xFFFFFF, (RECT *)0);
	Task_sleep(1);
	if (Param == 0 && (G.System_flg & SYS_ENDING) == 0) {
		Main.Vloop = 2;
		Main.Bg_chg_flg = 2;
	}
}

/* ------------------------------------------------------------------ */
/* Mess_disp — memory-card menu display dispatcher (overlay version)    */
/* ------------------------------------------------------------------ */
/* DS 0x801c1824 (Card_mess_disp). The frame is 152 bytes here against 184 in retail. Callees
 * are the retail calls (Card_blink_head in case 3, case 0xf and the 0x12/7..0x14 list);
 * the compiler here merges one of retail's 35 Mess_center_x call sites. The
 * case bodies follow the retail layout (2, 0x18, 0x12, 7.., 0xf, 1, 3, 0x17).
 * Retail spills the copies of Rno, Cursor and Err_code, the row counter and
 * the animation flag into 8-byte spaced slots (0x60..0x80) instead of the
 * argument home slots and 4-byte slots used here. */
void Card_mess_disp(int Rno, int Mode, UINT Cursor, int Err_code)
{
	/* retail reserves 32 more bytes of locals (Card_mess_disp frame 184); never read */
	volatile int Rsv[8];
	UCHAR ch;
	USHORT msg_off;
	BOOL flg;
	MWORK *pMc;
	short pos;
	int i_tmp;
	int i_tmp2;
	signed char slot_type;
	UCHAR *pChar;
	ULONG coord;
	UCHAR *p;
	ULONG curs_y;
	int i_tmp3;
	UINT len;
	UINT idx;
	UCHAR msg_buf [72];
	int scr_no;
	UINT cur_row;
	int err_no;
	UINT row;
	int anim_flg;

	pMc = pMwork;
	flg = 0;
	anim_flg = 0;
	scr_no = Rno;
	cur_row = Cursor;
	err_no = Err_code;
	switch (Rno) {
	case 0:
		break;
	case SEL_SLOT:
		if (Mode == 0) {
			i_tmp = Mess_center_x(5, 1);
			Mess_print((int)(short)i_tmp, 0x40, Mess_text + Mess_offs[5], 0x4002);
			i_tmp = Mess_center_x(3, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[0];
		} else {
			i_tmp = Mess_center_x(6, 1);
			Mess_print((int)(short)i_tmp, 0x40, Mess_text + Mess_offs[6], 0x4002);
			i_tmp = Mess_center_x(3, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[1];
		}
		Mess_print((int)pos, 0x90, Mess_text + msg_off, 0x4002);
		i_tmp = Mess_center_x(3, 1);
		Mess_print((int)(short)i_tmp, 0x60, Mess_text + Mess_offs[2], 0x4002);
		i_tmp = Mess_center_x(3, 1);
		Mess_print((int)(short)i_tmp, 0x78, Mess_text + Mess_offs[3], 0x4002);
		i_tmp = Mess_center_x(3, 1);
		coord = (int)(short)i_tmp - 0x10;
		curs_y = cur_row * 0x18 + 0x60;
		goto draw_cursor;
	case 0x18:
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 0x13, 0x2c, Mess_text + Mess_offs[68], 0x4002);
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 5, 0x40, Mess_text + Mess_offs[70], 0x4002);
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 5, 0x54, Mess_text + Mess_offs[71], 0x4002);
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 0x13, 0x78, Mess_text + Mess_offs[69], 0x4002);
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 5, 0x8c, Mess_text + Mess_offs[72], 0x4002);
		i_tmp = Mess_center_x(0x46, 1);
		Mess_print((int)(short)i_tmp - 5, 0xa0, Mess_text + Mess_offs[73], 0x4002);
		anim_flg = 1;
		if (pMwork->Blink_phase == 0) {
			if (cur_row == 0) {
				i_tmp = Mess_center_x(0x46, 1);
				pos = (short)i_tmp;
				coord = 0x2c;
			} else {
				i_tmp = Mess_center_x(0x46, 1);
				pos = (short)i_tmp;
				coord = 0x78;
			}
			Print14((int)pos - 0x21, coord, 0, 0, Card_font);
		}
		break;
	case 0x12:
		i_tmp = Mess_center_x(8, 1);
		Mess_print((int)(short)i_tmp, 0xb2, Mess_text + Mess_offs[8], 0x4012);
		Mess_print(0x8b, 0xc4, Mess_text + Mess_offs[9], 0x4002);
		Mess_print(0x8b, 0xd6, Mess_text + Mess_offs[10], 0x4002);
		Cursor_disp(0x7b, cur_row * 0x12 + 0xc4);
		i_tmp = Mess_width(0x22);
		Print14((0x68 - (short)i_tmp) / 2 - 7, (UINT)(UCHAR)pMc->Sel_row_bk * 0x14 + 0x38, 3, 0, Card_font);
	case 7:
	case 8:
	case 9:
	case 10:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0x11:
	case SCRL_UP:
	case SCRL_DOWN:
		Card_blink_head();
		row = 0;
		if (pMc->Scrl_slide_y != 0) goto slot_row_loop;
		while (row < 5) {
slot_row_loop:
			while (1) {
				i_tmp = 0;
				if (scr_no == 0x14) {
					idx = (row + (UCHAR)pMc->Scrl_ofs) - 1;
				} else {
					idx = row + (UCHAR)pMc->Scrl_ofs;
				}
				if (idx < 9) {
					msg_buf[0] = 0;
				} else {
					msg_buf[0] = (char)((idx + 1) / 10) + 12;
				}
				msg_buf[2] = 0xfe;
				msg_buf[1] = (char)(idx + 1) + (char)((idx + 1) / 10) * -10 + 12;
				i_tmp2 = Mess_width(0x22);
				i_tmp3 = row * 0x14 + 0x38;
				Mess_print((0x68 - (short)i_tmp2) / 2 + 2, pMc->Scrl_slide_y + i_tmp3, msg_buf, 0x4002);
				msg_buf[0] = 1;
				msg_buf[1] = 0xfe;
				i_tmp2 = Mess_width(0x22);
				Mess_print((0x68 - (short)i_tmp2) / 2 + 0x19, pMc->Scrl_slide_y + i_tmp3, msg_buf, 2);
				i_tmp2 = idx * 0x80;
				if ((UCHAR)pMc->F_num[idx] < 0xf) {
					if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Status_bak & 0x40000000) == 0) {
						if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id & 1) == 0) {
							i_tmp = 4;
							idx = (UINT)Mess_offs[31];
						} else {
							idx = (UINT)Mess_offs[32];
							i_tmp = 2;
						}
					} else if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id & 1) == 0) {
						idx = (UINT)Mess_offs[33];
						i_tmp = 4;
					} else {
						idx = (UINT)Mess_offs[34];
						i_tmp = 2;
					}
					Memcpy(msg_buf, Mess_text + idx, 0xe);
					if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Status_bak & 0x4000000) != 0) {
						i_tmp = 0;
					}
					if ((((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Status_bak & 0x4000) != 0) {
						msg_off = Mess_offs[38];
						if (((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id == 0) {
							msg_off = Mess_offs[37];
						}
						Memcpy(msg_buf, Mess_text + msg_off, 0xe);
						slot_type = ((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id;
						if (slot_type == 11) {
							Memcpy(msg_buf, Mess_text + Mess_offs[40], 0xe);
							slot_type = ((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id;
						}
						if (slot_type == 14) {
							Memcpy(msg_buf, Mess_text + Mess_offs[39], 0xe);
						}
						if ((UCHAR)((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id == 0x10) {
							Memcpy(msg_buf, Mess_text + Mess_offs[41], 0xe);
							i_tmp = 1;
						} else {
							pChar = msg_buf + (UCHAR)Mc_slot_ofs[(UCHAR)((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id];
							*pChar = *pChar + ((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Dummy16_lo;
							i_tmp = 1;
						}
					}
					slot_type = ((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id;
					if (slot_type == 12) {
						i_tmp = 3;
						Memcpy(msg_buf, Mess_text + Mess_offs[35], 0xe);
						slot_type = ((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Pl_id;
					}
					if (slot_type == 13) {
						i_tmp = 0;
						Memcpy(msg_buf, Mess_text + Mess_offs[36], 0xe);
					}
					pChar = msg_buf;
					idx = 0;
					ch = msg_buf[0];
					while (ch != 0xfe) {
						pChar++;
						idx++;
						ch = *pChar;
					}
					len = idx & 0xff;
					Memcpy(msg_buf + len, Mess_text + Mess_offs[42], 4);
					msg_buf[len + 1] =
						msg_buf[len + 1] +
						(char)(((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Save_cnt / 10);
					msg_buf[len + 2] =
						msg_buf[len + 2] +
						(char)((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Save_cnt +
						(char)(((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Save_cnt / 10) * -10;
					Memcpy(msg_buf + len + 4, Mess_text + Mess_offs[43 + (UINT)(UCHAR)((SAVE_BLOCK *)((UCHAR *)pMc->Save_info + i_tmp2))->Save_area], 0x18);
					if ((scr_no == 0xc) && (row == cur_row)) {
						flg = 0;
						if (pMc->Anim_wait == 1) {
							flg = msg_buf[(signed char)pMc->Msg_idx + 1] == 0xfe;
						}
						slot_type = pMc->Msg_idx;
						if (slot_type < 0) {
							msg_buf[0] = 0xfe;
						} else {
							if (((UCHAR)(msg_buf[slot_type] + 0x12) < 2) || (msg_buf[slot_type] == 0xf0)) {
								pMc->Msg_idx = slot_type + 1;
							}
							if ((UINT)Mc_prev_slot != (int)(signed char)pMc->Msg_idx) {
								if (msg_buf[(signed char)pMc->Msg_idx] == 0) {
									Snd_se_stad(0x2240000, (VECTOR *)0x0);
									pMc->Anim_wait = 10;
								} else {
									Snd_se_stad(0x2220000, (VECTOR *)0x0);
								}
							}
							msg_buf[(signed char)pMc->Msg_idx + 1] = 0xfe;
						}
						if ((int)(idx & 0xff) <= (int)(signed char)pMc->Msg_idx) {
							i_tmp2  = Mess_width(0x22);
							i_tmp3 = Mess_width(0x22);
							Mess_print((0x68 - (short)i_tmp2) / 2 + (short)i_tmp3 + 0x25, (int)pMc->Scrl_slide_y + row * 0x14 + 0x38, msg_buf + (idx & 0xff), 0x4002);
						}
						if (flg) {
							pMc->Msg_idx = 0xfe;
						}
					} else {
						i_tmp2  = Mess_width(0x22);
						i_tmp3 = Mess_width(0x22);
						Mess_print((0x68 - (short)i_tmp2) / 2 + (short)i_tmp3 + 0x25, (int)pMc->Scrl_slide_y + row * 0x14 + 0x38, msg_buf + (idx & 0xff), 0x4002);
					}
					msg_buf[idx & 0xff]       = 0xfe;
					msg_buf[(idx & 0xff) + 1] = 0;
				} else {
					if (pMc->Card_total_size == 0x1e000) {
						i_tmp = 3;
					}
					Memcpy(msg_buf, Mess_text + Mess_offs[17], 0xf);
				}
				i_tmp2 = Mess_width(0x22);
				Mess_print((0x68 - (short)i_tmp2) / 2 + 0x25, (int)pMc->Scrl_slide_y + row * 0x14 + 0x38, msg_buf, i_tmp << 4 | 0x4002);
				row++;
				if (pMc->Scrl_slide_y == 0) break;
				if (5 < row) goto slot_list_done;
			}
		}
slot_list_done:
		if (pMc->Port == 0) {
			i_tmp = Mess_center_x(2, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[2];
		} else {
			i_tmp = Mess_center_x(3, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[3];
		}
		Mess_print((int)pos, 0x14, Mess_text + msg_off, 0x4002);
		if ((1 < scr_no - 0x11U) && (scr_no != 0xc)) {
			i_tmp = Mess_width(0x22);
			Cursor_disp((0x68 - (short)i_tmp) / 2 - 7, cur_row * 0x14 + 0x38);
		}
		if ((scr_no == 0x11) && ((err_no - 2U < 2 || (err_no - 7U < 2)))) {
			i_tmp = Mess_width(0x22);
			Print14((0x68 - (short)i_tmp) / 2 - 7, (UINT)(UCHAR)pMc->Sel_row_bk * 0x14 + 0x38, 3, 0, Card_font);
		}
		flg = 1;
		if (scr_no != 0xc) {
			if (pMc->Scrl_slide_y != 0) {
				if (((Main.pDb)->Draw).ofs[1] == 0xf0) {
					AddPrim(G.pOt0 + 1, &pMc->Scrl_mask[0][1]);
					p = (UCHAR *)&pMc->Scrl_mask[1][1];
				} else {
					AddPrim(G.pOt0 + 1, &pMc->Scrl_mask[0][0]);
					p = (UCHAR *)&pMc->Scrl_mask[1][0];
				}
				AddPrim(G.pOt0 + 1, p);
			}
			msg_buf[0] = 0xfe;
			msg_buf[1] = 0xfe;
			slot_type = pMc->Arrow_tmr;
			pMc->Arrow_tmr = slot_type + 1U;
			if (0x1f < (UCHAR)(slot_type + 1U)) {
				pMc->Arrow_tmr = 0;
			}
			if (((UCHAR)pMc->Arrow_tmr < 0x10) || (pMc->Scrl_slide_y != 0)) {
				if (pMc->Scrl_ofs != 0) {
					msg_buf[0] = 0x9c;
					Mess_print(0x99, 0x24, msg_buf, 2);
				}
				if ((UCHAR)pMc->Scrl_ofs < 10) {
					msg_buf[0] = 11;
					Mess_print(0x99, 0x9c, msg_buf, 2);
				}
			}
		}
		break;
	case 0xf:
		if (pMwork->Port == 0) {
			i_tmp = Mess_center_x(2, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[2];
		} else {
			i_tmp = Mess_center_x(3, 1);
			pos = (short)i_tmp;
			msg_off = Mess_offs[3];
		}
		Mess_print((int)pos, 0x14, Mess_text + msg_off, 0x4002);
		i_tmp = Mess_center_x(0xb, 1);
		Mess_print((int)(short)i_tmp, 0x40, Mess_text + Mess_offs[11], 0x4012);
		i_tmp = Mess_center_x(0xd, 1);
		Mess_print((int)(short)i_tmp, 0x60, Mess_text + Mess_offs[12], 0x4002);
		i_tmp = Mess_center_x(0xd, 1);
		Mess_print((int)(short)i_tmp, 0x78, Mess_text + Mess_offs[13], 0x4002);
		i_tmp = Mess_center_x(0xd, 1);
		Cursor_disp((int)(short)i_tmp - 0x10, cur_row * 0x18 + 0x60);
		Card_blink_head();
		break;
	case 1:
		i_tmp = Mess_center_x(0xf, 1);
		Mess_print((int)(short)i_tmp, 0x60, Mess_text + Mess_offs[14], 0x4002);
		i_tmp = Mess_center_x(0xf, 1);
		Mess_print((int)(short)i_tmp, 0x78, Mess_text + Mess_offs[15], 0x4002);
		i_tmp = Mess_center_x(0xf, 1);
		coord = (int)(short)i_tmp - 0x10;
		curs_y = cur_row * 0x18 + 0x60;
		goto draw_cursor;
	case ERR_MESS:
		if (Err_code != 4) {
			if (pMwork->Port == 0) {
				i_tmp = Mess_center_x(2, 1);
				pos = (short)i_tmp;
				msg_off = Mess_offs[2];
			} else {
				i_tmp = Mess_center_x(3, 1);
				pos = (short)i_tmp;
				msg_off = Mess_offs[3];
			}
			Mess_print((int)pos, 0x14, Mess_text + msg_off, 0x4002);
		}
		Card_blink_head();
		i_tmp = Mess_center_x(err_no + 0x14, 1);
		Mess_print((int)(short)i_tmp, 0x60, Mess_text + Mess_offs[20 + err_no], 0x4012);
		break;
	case 0x17:
		i_tmp = Mess_center_x(0x12, 1);
		Mess_print((int)(short)i_tmp, 0x40, Mess_text + Mess_offs[18], 0x4012);
		i_tmp = Mess_center_x(0x12, 1);
		Mess_print((int)(short)i_tmp, 0x54, Mess_text + Mess_offs[19], 0x4012);
		i_tmp = Mess_center_x(10, 1);
		Mess_print((int)(short)i_tmp, 0x74, Mess_text + Mess_offs[9], 0x4002);
		i_tmp = Mess_center_x(10, 1);
		Mess_print((int)(short)i_tmp, 0x8c, Mess_text + Mess_offs[10], 0x4002);
		i_tmp = Mess_center_x(10, 1);
		coord = (int)(short)i_tmp - 0x10;
		curs_y = cur_row * 0x18 + 0x74;
draw_cursor:
		Cursor_disp(coord, curs_y);
		break;
	}
	if (flg) {
		pos = pMc->Bg_kido;
		pMc->Bg_kido = pos + 3;
		if (0x1e < (short)(pos + 3)) {
			pMc->Bg_kido = 0x1e;
		}
	} else {
		pMc->Bg_kido = 0;
	}
	if (anim_flg == 0) {
		pMc->Win_kido = 0;
	} else {
		pos = pMc->Win_kido;
		pMc->Win_kido = pos + 3;
		if (0x1e < (short)(pos + 3)) {
			pMc->Win_kido = 0x1e;
		}
	}
	Mc_prev_slot = pMc->Msg_idx;
	Fade_adjust(1, (int)pMc->Win_kido << 7, 0, &pMc->Win_rect[0]);
	Fade_adjust(2, (int)pMc->Win_kido << 7, 0, &pMc->Win_rect[1]);
	Fade_adjust(3, (int)pMc->Bg_kido << 7, 0, &pMc->Bg_rect);
	return;
}

/* ------------------------------------------------------------------ */
/* Cursor_disp — print the cursor glyph at (X, Y) when the card screen  */
/* is in its non-busy display state (Ghidra MEMCARD::801c2ac4)      */
/* ------------------------------------------------------------------ */
/* DS 0x801c2ac4 (Cursor_disp) */
void Cursor_disp(ULONG X, ULONG Y)
{
	if (pMwork->Blink_phase == 0) {
		Print14(X, Y, 0, 0, Card_font);
	}
}

/* ------------------------------------------------------------------ */
/* Search_f_num — fill F_num[0..14] with the slot index of each save    */
/* file, indexed by its two-digit number in the file title (0xF = none) */
/* (Ghidra MEMCARD::801c2b0c)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c2b0c (Search_f_num) */
void Search_f_num(UCHAR *F_num)
{
	UINT i;
	UINT Slot;

	for (i = 0; i < 15; i++) {
		F_num[i] = 0xF;
	}
	for (i = 0; i < 15; i++) {
		if (pMwork->File_info.num <= i) {
			return;
		}
		Slot = (pMwork->File_info.title[i][12] - 48) * 10 + pMwork->File_info.title[i][13] - 48;
		F_num[Slot] = (UCHAR)i;
	}
}

/* ------------------------------------------------------------------ */
/* Check_card — enumerate this game's save files on the card in the     */
/* current port and load each one's 128-byte save block into Save_info  */
/* (Ghidra MEMCARD::801c2b98)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c2b98 (Check_card) */
int Check_card(UCHAR *F_num)
{
	int i;
	int Ok;
	UINT Status;
	char *Ident;

	if (pMwork->Ident_sel == 0) {
		Ident = "BASLUS-00421*";
	} else {
		Ident = "BASLUS-00748*";
	}
	Card_search(pMwork->Port, &pMwork->File_info, Ident);
	Search_f_num(F_num);

	Ok = (pMwork->File_info.num != 0);
	for (i = 0; Ok && i < 15; i++) {
		if (F_num[i] != 0xF) {
			if (Card_read(&pMwork->Card_save.Game_play_time, pMwork->File_info.title[F_num[i]], 0x80, 0x200) != 0) {
				Ok = 0;
			} else {
				Memcpy(&pMwork->Save_info[i], &pMwork->Card_save.Game_play_time, 0x80);
			}
		}
	}
	if (Ok) {
		return 0;
	}

	Status = Card_check(pMwork->Port);
	if (Status == 0) {
		return 0;
	}
	if (Status == 3) {
		return 3;
	}
	return Card_format_check(pMwork->Port);
}

/* ------------------------------------------------------------------ */
/* Card_blink_head — advance the blink phase of the slot-select marker  */
/* and redraw it (Ghidra MEMCARD::801c2cdc)                         */
/* ------------------------------------------------------------------ */
/* DS 0x801c2cdc (Card_blink_head) */
void Card_blink_head(void)
{
	MWORK *pMw;
	signed char Phase;
	USHORT Off;

	pMw = pMwork;
	Phase = pMw->Mark_pulse + pMw->Mark_step;
	pMw->Mark_pulse = Phase;
	if (Phase == 0x0F || Phase == 0) {
		pMw->Mark_step = -(signed char)pMw->Mark_step;
	}
	if (pMw->Ident_sel == 0) {
		Off = Mess_offs[66];
	} else {
		Off = Mess_offs[67];
	}
	Mess_print(0x16, ((int)((UINT)pMw->Mark_pulse << 24) >> 27) + 0x24, &Mess_text[Off], 0x44002);
}

/* ------------------------------------------------------------------ */
/* Card_poll — return nonzero when the card on Port has an event ready  */
/* (Ghidra MEMCARD::801c2d9c)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c2d9c (Card_poll) */
BOOL Card_poll(int Port)
{
	MWORK *pMw;

	pMw = pMwork;
	if (_card_info(Port << 4) == 0) {
		return 0;
	}
	pMw->Card_poll_req = 1;
	return pMw->Card_ready != 0;
}

/* ------------------------------------------------------------------ */
/* Wslot_check — show the slot prompt and test both card slots; bit 0   */
/* of the result is slot 1 present, bit 1 is slot 2 present             */
/* (Ghidra MEMCARD::801c2de8)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c2de8 (Wslot_check) */
UINT Wslot_check(void)
{
	UCHAR Slot0;
	UINT Slot1;

	Mess_print((short)Mess_center_x(0x4B, 1), 0xB2, MESS_STR(7), 0x4002);
	Mess_print((short)Mess_center_x(0x4B, 1), 0xC0, MESS_STR(75), 0x4002);
	Task_sleep(2);
	Slot0 = Card_check(0) & 1;
	Slot1 = Card_check(1);
	return Slot0 | ((Slot1 & 1) << 1);
}

/* ------------------------------------------------------------------ */
/* Card_mess_make — fill the 38-byte card-file icon header Mess for the */
/* slot with cursor index Cursor: icon by character and status, the    */
/* save count, the save area name and the play-time marks. Save_data   */
/* is not read by the retail code.                                      */
/* (Ghidra MEMCARD::801c2e90)                                       */
/* ------------------------------------------------------------------ */
/* Memory card save header: 0x26 bytes of icon/status, then the 0x1B-byte save area name */
#define MC_HDR_NAME_OFS		0x26

/* DS 0x801c2e90 (Card_mess_make) */
void Card_mess_make(void *Mess, ULONG Save_data, char Cursor)
{
	UCHAR *Hdr;
	UCHAR *Src;
	UCHAR Num;

	(void)Save_data;
	Hdr = (UCHAR *)Mess;
	Num = (UCHAR)(Cursor + 1);

	if ((G.System_flg & SYS_EX_BATTLE) == 0) {
		if ((UCHAR)(G.SaveData.Pl_id - 0xC) < 2) {
			if (G.SaveData.Pl_id == 0xC) {
				Src = Mc_icon_801bfca8;
			} else {
				Src = Mc_icon_801bfcd0;
			}
		} else if ((G.SaveData.Status_bak & 0x40000000) == 0) {
			if ((G.SaveData.Pl_id & 1) == 0) {
				Src = Mc_icon_801bfc80;
			} else {
				Src = Mc_icon_801bfc58;
			}
		} else if ((G.SaveData.Pl_id & 1) == 0) {
			Src = Mc_icon_801bfc30;
		} else {
			Src = Mc_icon_801bfc08;
		}
	} else {
		if ((G.SaveData.Pl_id & 1) == 0) {
			Src = Mc_icon_801bfb68;
		} else {
			Src = Mc_icon_801bfb40;
		}
		Memcpy(Hdr, Src, 0x26);
		if (G.SaveData.Pl_id == 0x0B) {
			Memcpy(Hdr, Mc_icon_801bfb90, 0x26);
		}
		if (G.SaveData.Pl_id == 0x0E) {
			Memcpy(Hdr, Mc_icon_801bfbb8, 0x26);
		}
		if (G.SaveData.Pl_id != 0x10) {
			Hdr[0x1D] = Hdr[0x1D] + (char)G.SaveData.Sce_timer;
			goto icon_done;
		}
		Src = Mc_icon_801bfbe0;
	}
	Memcpy(Hdr, Src, 0x26);
icon_done:
	if (Num < 10) {
		Memcpy(Hdr + 10, Mc_icon_801bfcf8, 2);
	} else {
		Hdr[0x0B] = Hdr[0x0B] + Num / 10;
	}
	if ((G.SaveData.Status_bak & 0x4000000) != 0) {
		Memcpy(Hdr + 8, Mc_icon_801bfcfc, 2);
	}
	if ((G.SaveData.Status_bak & 0x20000000) != 0) {
		Memcpy(Hdr + 8, Mc_icon_801bfd00, 2);
	}
	if ((G.SaveData.Status_bak & 2) != 0) {
		Memcpy(Hdr + 8, Mc_icon_801bfd04, 2);
	}
	if ((G.SaveData.Status_bak & 0x4000) != 0) {
		Memcpy(Hdr + 8, Mc_icon_801bfd08, 2);
	}
	Hdr[0x0D] = Hdr[0x0D] + Num % 10;
	Hdr[0x21] = Hdr[0x21] + (char)(G.SaveData.Save_cnt / 10);
	Hdr[0x23] = Hdr[0x23] + (char)(G.SaveData.Save_cnt % 10);
	Memcpy(Hdr + MC_HDR_NAME_OFS, Mc_area_name[G.SaveData.Save_area], 0x1B);
}

/* ------------------------------------------------------------------ */
/* Save_push — copy the current game state into G.SaveData before a     */
/* save. Accumulates play time, sets status bits and copies player,     */
/* equipment, volume and key settings. (Ghidra MEMCARD::801c3228)   */
/* ------------------------------------------------------------------ */
/* DS 0x801c3228 (Save_push) */
void Save_push(void)
{
	int Now;
	ULONG *pSys_flg;

	pSys_flg = &G.System_flg;
	G.SaveData.Pl_id = G.Player.Id;
	G.SaveData.Pl_pos_x = (short)G.Player.Pos_x;
	G.SaveData.Pl_pos_y = (short)G.Player.Pos_y;
	G.SaveData.Pl_pos_z = (short)G.Player.Pos_z;
	G.SaveData.Dummy0 = G.Player.Cdir_y;

	if ((*pSys_flg & SYS_ENDING) == 0) {
		Now = VSync(-1);
		G.SaveData.Game_play_time += (Now - G.Game_time_offset);
		G.Game_time_offset = VSync(-1);
	} else {
		G.SaveData.Game_play_time = 0;
	}

	G.SaveData.Equip_id = St.Equip_item_id;
	G.SaveData.Equip_no = St.Equip_item_no;

	if ((*pSys_flg & SYS_EX_BATTLE) != 0) {
		G.Status_flg |= STAT_EX_SAVE;
		G.SaveData.Dummy16 = G.SaveData.Sce_timer;
		Memcpy((UCHAR *)G.SaveData.ExData3 + 8, G.ExData + 4, 0x3C);
	}
	if ((*pSys_flg & SYS_EASY_MODE) != 0) {
		G.Status_flg |= STAT_EASY_MODE;
	}
	if ((*pSys_flg & SYS_ARRANGE) != 0) {
		G.Status_flg |= STAT_ARRANGE_SAVED;
	}
	if ((*pSys_flg & 0x40) != 0) {
		G.Status_flg |= STAT_QUAKE_RASTA;
	}

	G.SaveData.Status_bak = G.Status_flg;
	G.SaveData.Pl_life = G.Player.Life;
	G.SaveData.Pl_poison_down = G.Pl_poison_down;
	G.SaveData.Pl_poison_timer = G.Pl_poison_timer;
	G.SaveData.Vol_Se = Volume_Se;
	G.SaveData.Vol_Bgm = Volume_Bgm;
	G.SaveData.Stereo = G.Sound_mode;
	G.SaveData.Key_idx = Main.Key_idx;
}

/* ------------------------------------------------------------------ */
/* Load_pop — restore game state from G.SaveData after a load. Inverse  */
/* of Save_push. (Ghidra MEMCARD::801c348c)                         */
/* ------------------------------------------------------------------ */
/* DS 0x801c348c (Load_pop) */
void Load_pop(void)
{
	G.Status_flg = G.SaveData.Status_bak;
	if ((G.SaveData.Status_bak & 0x4000000) != 0) {
		G.System_flg |= SYS_EASY_MODE;
	}
	if ((G.SaveData.Status_bak & 0x20000000) != 0) {
		G.System_flg |= SYS_ARRANGE;
	}
	if ((G.SaveData.Status_bak & 2) != 0) {
		G.System_flg |= 0x40;
	}

	G.Next_cut_no = (UCHAR)G.SaveData.Cut_no;
	St.Equip_item_id = G.SaveData.Equip_id;
	St.Equip_item_no = G.SaveData.Equip_no;
	G.Player.Life = G.SaveData.Pl_life;
	G.Pl_poison_down = G.SaveData.Pl_poison_down;
	G.Pl_poison_timer = G.SaveData.Pl_poison_timer;
	Volume_Se = G.SaveData.Vol_Se;
	Volume_Bgm = G.SaveData.Vol_Bgm;
	G.Sound_mode = G.SaveData.Stereo;
	Main.Key_idx = G.SaveData.Key_idx;
	Snd_sys_stereo(G.SaveData.Stereo);
}

/* ------------------------------------------------------------------ */
/* Clear_event_wait — discard four pending card events                  */
/* (Ghidra MEMCARD::801c35d8)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c35d8 (Clear_event_wait) */
void Clear_event_wait(void)
{
	TestEvent(Ev0);
	TestEvent(Ev1);
	TestEvent(Ev2);
	TestEvent(Ev3);
}

/* ------------------------------------------------------------------ */
/* Clear_event_wait2 — discard three pending card events                */
/* (Ghidra MEMCARD::801c3630)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3630 (Clear_event_wait2) */
void Clear_event_wait2(void)
{
	TestEvent(Ev10);
	TestEvent(Ev11);
	TestEvent(Ev13);
}

/* ------------------------------------------------------------------ */
/* Card_write — write a save header and body to the card file in slot   */
/* Port, then read the first 512 bytes back to verify it.               */
/* Head is a _CARD header; Cap is the body size in bytes (8 KB blocks   */
/* are derived from it). Retries three times; returns 0 on success,     */
/* 1 if the file cannot be opened, 2 on write or verify failure.        */
/* (Ghidra MEMCARD::801c3678)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3678 (Card_write) */
ULONG Card_write(void *Head, char Port, char *Name, char *Title, UINT Cap)
{
	_CARD *Hd;
	char Fname[32];
	UCHAR Verify[512];
	int Fd;
	int Try;
	UINT Wrote;
	UINT Attempt;
	long Got;

	Hd = (_CARD *)Head;
	memcpy(Fname, Mc_fname_tmpl, 6);
	memset(Fname + 6, 0, 26);
	Fname[2] += Port;
	strcpy(Fname + 5, Name);

	Hd->blockEntry = (UCHAR)(Cap >> 13);
	if ((Cap & 0x1FFF) != 0) {
		Hd->blockEntry++;
	}
	strcpy(Hd->title, Title);

	Fd = open(Fname, ((UINT)Hd->blockEntry << 16) | 0x200);
	close(Fd);

	Attempt = 0;
	for (;;) {
		for (Try = 3; (Fd = open(Fname, 2)) == -1; ) {
			if (--Try == 0) {
				return 1;
			}
		}
		for (Try = 3; (Wrote = write(Fd, Head, Cap)) != Cap; ) {
			if (--Try == 0) {
				return 2;
			}
		}
		close(Fd);

		Fd = open(Fname, 1);
		if (Fd == -1) {
			return 2;
		}
		Got = read(Fd, Verify, 0x200);
		close(Fd);

		Attempt++;
		if (Got == 0x200) {
			break;
		}
		if (Attempt > 2) {
			return 2;
		}
	}
	return 0;
}

/* ------------------------------------------------------------------ */
/* Card_read — read Cap bytes at Offset from the card file named Name   */
/* in the current port. Up to three attempts; returns 0 on success,     */
/* 2 on failure. (Ghidra MEMCARD::801c382c)                         */
/* ------------------------------------------------------------------ */
/* DS 0x801c382c (Card_read) */
ULONG Card_read(void *Head, char *Name, size_t Cap, off_t Offset)
{
	char Fname[32];
	int Fd;
	UINT Attempt;

	memcpy(Fname, Mc_fname_tmpl, 6);
	memset(Fname + 6, 0, 26);
	Fname[2] += pMwork->Port;
	strcpy(Fname + 5, Name);

	Attempt = 0;
	while ((Fd = open(Fname, 1)) == -1 || lseek(Fd, Offset, 0) == -1 || read(Fd, Head, Cap) != Cap) {
		Attempt++;
		if (Attempt > 2) {
			return 2;
		}
	}
	close(Fd);
	return 0;
}

/* ------------------------------------------------------------------ */
/* Card_search — list the save files on card Port whose names match     */
/* Pattern into Buff (title, size, count, total size).                  */
/* (Ghidra MEMCARD::801c394c)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c394c (Card_search) */
void Card_search(char Port, CFILE_INFO *Buff, char *Pattern)
{
	char Fname[32];
	struct DIRENTRY Dir;
	struct DIRENTRY *Found;
	UINT i;

	memcpy(Fname, Mc_fname_tmpl, 5);
	Fname[2] = Mc_fname_tmpl[2] + Port;
	strcpy(Fname + 5, Pattern);

	Buff->num = 0;
	Buff->total_size = 0;
	Found = firstfile(Fname, &Dir);
	if (Found == &Dir) {
		do {
			strcpy((char *)Buff->title[Buff->num], (char *)Dir.name);
			Buff->size[Buff->num] = Dir.size;
			Buff->total_size += Dir.size;
			Buff->num++;
			Found = nextfile(&Dir);
		} while (Found == &Dir);
	}

	for (i = 0; i < 15; i++) {
		if (i < Buff->num) {
			Buff->title[i][20] = 0;
		} else {
			Buff->title[i][0] = 0;
		}
	}
}

/* ------------------------------------------------------------------ */
/* Card_check — poll card status on Port until it settles. Returns 3 if */
/* an event reports a bad card. A settled status of 2 runs Card_clear   */
/* and reports 0; any other settled status is returned as is.           */
/* (Ghidra MEMCARD::801c3a98)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3a98 (Card_check) */
UINT Card_check(int Port)
{
	int Tries;
	UINT Status;
	UINT Last;

	Status = 0;
	Last = 0xFF;
	Tries = 8;
	Clear_event_wait();
	do {
		if (_card_info(Port << 4) != 0) {
			Status = Get_card_event();
			if (Status == 3) {
				return 3;
			}
			if ((Status & 1) == 0 && Status == Last) {
				break;
			}
			Last = Status;
		}
	} while (--Tries != 0);

	if (Status == 2) {
		Card_clear(Port);
		Status = 0;
	}
	return Status;
}

/* ------------------------------------------------------------------ */
/* Card_format_check — returns 0 when the card on Port loads cleanly,   */
/* otherwise the last event code after three tries.                     */
/* (Ghidra MEMCARD::801c3b50)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3b50 (Card_format_check) */
int Card_format_check(int Port)
{
	int Tries;
	int Event;

	Event = 0;
	Tries = 3;
	Clear_event_wait();
	do {
		if (_card_load(Port << 4) != 0) {
			Event = Get_card_event();
			if (Event == 0) {
				return 0;
			}
		}
	} while (--Tries != 0);
	return Event;
}

/* ------------------------------------------------------------------ */
/* Card_format — format the card on Port. Returns 1 on success, 0 on    */
/* failure (the libcard format result is inverted). The 6-byte key is   */
/* the card name head with the port number added to byte 2.            */
/* (Ghidra MEMCARD::801c3bc4)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3bc4 (Card_format) */
UINT Card_format(int Port)
{
	UCHAR Key[6];

	memcpy(Key, Mc_fname_tmpl, 6);
	Key[2] += Port;
	return format((char *)Key) ^ 1;
}

/* ------------------------------------------------------------------ */
/* Get_card_event — wait for one of four card events. Returns the index */
/* of the event that fired; 3 on timeout (about 250000 polls).          */
/* (Ghidra MEMCARD::801c3c1c)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3c1c (Get_card_event) */
ULONG Get_card_event(void)
{
	int Tries;

	Tries = 249999;
	do {
		if (TestEvent(Ev0) != 0) {
			return 0;
		}
		if (TestEvent(Ev1) != 0) {
			return 1;
		}
		if (TestEvent(Ev2) != 0) {
			return 2;
		}
	} while (TestEvent(Ev3) == 0 && --Tries != 0);
	return 3;
}

/* ------------------------------------------------------------------ */
/* Get_card_event2 — wait for one of three card events. Returns the     */
/* index of the event that fired; 3 on timeout (about 250000 polls).    */
/* (Ghidra MEMCARD::801c3cb4)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3cb4 (Get_card_event2) */
ULONG Get_card_event2(void)
{
	int Tries;

	Tries = 249999;
	do {
		if (TestEvent(Ev10) != 0) {
			return 0;
		}
		if (TestEvent(Ev11) != 0) {
			return 1;
		}
		if (TestEvent(Ev13) != 0) {
			return 2;
		}
		Tries--;
	} while (Tries != 0);
	return 3;
}

/* ------------------------------------------------------------------ */
/* Card_clear — erase the card on Port, retrying up to ten times. Returns */
/* 0 on success, otherwise the last event code.                         */
/* (Ghidra MEMCARD::801c3d34)                                       */
/* ------------------------------------------------------------------ */
/* DS 0x801c3d34 (Card_clear) */
int Card_clear(int Port)
{
	int Tries;
	int Event;

	Event = 2;
	Tries = 10;
	Clear_event_wait2();
	do {
		if (_card_clear(Port << 4) != 0) {
			Event = Get_card_event2();
			if (Event == 0) {
				return 0;
			}
		}
	} while (--Tries != 0);
	return Event;
}

/* DS 0x801bfd0c (Mc_fname_tmpl) */
UCHAR Mc_fname_tmpl[6] = {
	0x62, 0x75, 0x30, 0x30, 0x3a, 0x00
};

/* DS 0x801c42f8 (Mc_wait_tbl) - byte table, 34 entries; the 2 zero bytes after it are alignment before pMwork */
UCHAR Mc_wait_tbl[34] = {
	0x0a, 0x10, 0x0c, 0x12, 0x0c, 0x0f, 0x0b, 0x0d, 0x14, 0x13, 0x0d, 0x0c, 0x12, 0x10, 0x11, 0x13,
	0x0e, 0x12, 0x0e, 0x12, 0x0c, 0x14, 0x0e, 0x12, 0x10, 0x11, 0x13, 0x0e, 0x12, 0x0e, 0x12, 0x0c,
	0x14, 0x0e
};
