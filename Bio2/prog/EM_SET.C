/*
 * EM_SET.C - Enemy work allocation and EMD model loading
 * Resident Evil 2 Dual Shock (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\EM_SET.C
 *
 * DS retail range: 8001aa38 (Enemy_work_set) .. 8001babb (Emd_load_file)
 */

#include "TYPES.H"
#include "MAIN.H"
#include "SLD.H"
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBGS.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include <MALLOC.H>
#include "EM_SET.H"
#include "CD.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MONITOR.H"
#include "SABUN.H"
#include "SUB.H"
#include "DOOR.H"
#include "FILECODE.H"

/* DS 0x80100000 (Em_ov_bank) */
UCHAR Em_ov_bank[2][0xd000] RAM_SECTION(".buf_em_ov_bank");

/* DS 0x8009adf4 */
DATA_POS cdemd0_pos[300] = {
	{ 0x0, 0xcf4c }, { 0x1a, 0xcf4c }, { 0x34, 0x10420 }, { 0x55, 0x23dcc },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x9d, 0x10420 }, { 0xbe, 0x247dc },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x107, 0x20820 }, { 0x149, 0x23b5c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x191, 0x10420 }, { 0x1b2, 0x23d2c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1fa, 0x10420 }, { 0x21b, 0x23cac },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x263, 0x10420 }, { 0x284, 0x23a5c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x2cc, 0x10420 }, { 0x2ed, 0x24e98 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x337, 0x10420 }, { 0x358, 0x22604 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x39d, 0x20820 }, { 0x3df, 0x22798 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x424, 0x20820 }, { 0x466, 0x22604 },
	{ 0x4ab, 0x56fa }, { 0x4b6, 0x56fa }, { 0x4c1, 0x10420 }, { 0x4e2, 0x1d938 },
	{ 0x51e, 0x4a88 }, { 0x528, 0x4a88 }, { 0x532, 0x8220 }, { 0x543, 0x7e28 },
	{ 0x553, 0x7698 }, { 0x562, 0x7698 }, { 0x571, 0x10420 }, { 0x592, 0x2ab74 },
	{ 0x5e8, 0x48e8 }, { 0x5f2, 0x48e8 }, { 0x5fc, 0x20820 }, { 0x63e, 0x2e318 },
	{ 0x69b, 0x5da0 }, { 0x6a7, 0x5da0 }, { 0x6b3, 0x10420 }, { 0x6d4, 0x1fab0 },
	{ 0x714, 0x66d4 }, { 0x721, 0x66d4 }, { 0x72e, 0x10420 }, { 0x74f, 0xd0c8 },
	{ 0x76a, 0x10fa }, { 0x76d, 0x10fa }, { 0x770, 0x8220 }, { 0x781, 0x1a14 },
	{ 0x785, 0x389c }, { 0x78d, 0x389c }, { 0x795, 0x8220 }, { 0x7a6, 0x9ad0 },
	{ 0x7ba, 0x67b0 }, { 0x7c7, 0x67b0 }, { 0x7d4, 0x20820 }, { 0x816, 0x2df0c },
	{ 0x872, 0x1efc }, { 0x876, 0x1efc }, { 0x87a, 0x8220 }, { 0x88b, 0x3110 },
	{ 0x892, 0x4f10 }, { 0x89c, 0x4f10 }, { 0x8a6, 0x10420 }, { 0x8c7, 0x21c84 },
	{ 0x90b, 0x4d68 }, { 0x915, 0x4d68 }, { 0x91f, 0x18620 }, { 0x950, 0x22278 },
	{ 0x995, 0x1f4 }, { 0x996, 0x1f4 }, { 0x997, 0x8220 }, { 0x9a8, 0x3e5c },
	{ 0x9b0, 0x1528 }, { 0x9b3, 0x1528 }, { 0x9b6, 0x8220 }, { 0x9c7, 0x6020 },
	{ 0x9d4, 0x5a7a }, { 0x9e0, 0x5a7a }, { 0x9ec, 0x10420 }, { 0xa0d, 0x34ecc },
	{ 0xa77, 0x2464 }, { 0xa7c, 0x2464 }, { 0xa81, 0x8220 }, { 0xa92, 0x1578 },
	{ 0xa95, 0x76ac }, { 0xaa4, 0x76ac }, { 0xab3, 0x18620 }, { 0xae4, 0x2527c },
	{ 0xb2f, 0x7d58 }, { 0xb3f, 0x7d58 }, { 0xb4f, 0x18620 }, { 0xb80, 0x2f264 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xbdf, 0x67b8 }, { 0xbec, 0x67b8 }, { 0xbf9, 0x18620 }, { 0xc2a, 0x320a8 },
	{ 0xc8f, 0xb2d4 }, { 0xca6, 0xb2d4 }, { 0xcbd, 0x18620 }, { 0xcee, 0x42d94 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xd74, 0x5bb4 }, { 0xd80, 0x5bb4 }, { 0xd8c, 0x20620 }, { 0xdcd, 0x1b6d4 },
	{ 0xe04, 0x59f0 }, { 0xe10, 0x59f0 }, { 0xe1c, 0x8220 }, { 0xe2d, 0xe7f0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xe4a, 0x5a7a }, { 0xe56, 0x5a7a }, { 0xe62, 0x10420 }, { 0xe83, 0x34ecc },
	{ 0xeed, 0x4e80 }, { 0xef7, 0x4e80 }, { 0xf01, 0x10420 }, { 0xf22, 0xaa5c },
	{ 0xf38, 0x14d6 }, { 0xf3b, 0x14d6 }, { 0xf3e, 0x8220 }, { 0xf4f, 0xd98 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xf51, 0x270 }, { 0xf52, 0x270 }, { 0xf53, 0x8220 }, { 0xf64, 0xd810 },
	{ 0xf80, 0x37c }, { 0xf81, 0x37c }, { 0xf82, 0x8220 }, { 0xf93, 0x6574 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0xfa0, 0x18620 }, { 0xfd1, 0xd648 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0xfec, 0x10420 }, { 0x100d, 0x12df0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1033, 0x18620 }, { 0x1064, 0x12df0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x108a, 0x18620 }, { 0x10bb, 0x12638 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x10e0, 0x10420 }, { 0x1101, 0x120fc },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1126, 0x18620 }, { 0x1157, 0x12638 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x117c, 0x18620 }, { 0x11ad, 0xc834 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x11c7, 0x18620 }, { 0x11f8, 0x789c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1208, 0x18620 }, { 0x1239, 0xc834 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1253, 0x18620 }, { 0x1284, 0x27338 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x12d3, 0x10420 }, { 0x12f4, 0x1230c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1319, 0x18620 }, { 0x134a, 0x124d8 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x136f, 0x10420 }, { 0x1390, 0x12ae4 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x13b6, 0x10420 }, { 0x13d7, 0x124d8 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x13fc, 0x10420 }, { 0x141d, 0x12a34 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1443, 0x10420 }, { 0x1464, 0x124f0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1489, 0x18620 }, { 0x14ba, 0x12c2c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x14e0, 0x18620 }, { 0x1511, 0x130f0 }
};

/* DS 0x8009b754 */
UCHAR cdemd0_sum[300] = {
	106, 72, 181, 135, 0, 0, 12, 190, 0, 0, 147, 22, 0, 0, 76, 125,
	0, 0, 0, 0, 0, 0, 13, 248, 0, 0, 235, 111, 0, 0, 202, 236,
	0, 0, 156, 253, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 228, 80, 0, 0, 228, 253,
	222, 253, 250, 133, 18, 51, 26, 164, 162, 140, 37, 224, 20, 53, 219, 137,
	201, 235, 233, 85, 9, 40, 114, 7, 245, 210, 85, 195, 166, 135, 141, 20,
	218, 249, 23, 52, 47, 14, 51, 184, 248, 199, 8, 209, 47, 14, 27, 176,
	49, 18, 13, 71, 232, 201, 117, 79, 86, 119, 217, 21, 222, 255, 22, 117,
	71, 101, 60, 100, 29, 60, 232, 217, 0, 0, 0, 0, 252, 210, 150, 2,
	178, 147, 154, 152, 0, 0, 0, 0, 73, 106, 214, 68, 175, 142, 186, 28,
	0, 0, 0, 0, 86, 119, 60, 21, 40, 11, 71, 171, 76, 108, 51, 27,
	0, 0, 0, 0, 0, 0, 0, 0, 178, 173, 169, 55, 64, 33, 251, 158,
	0, 0, 106, 96, 0, 0, 160, 147, 0, 0, 0, 0, 0, 0, 40, 147,
	0, 0, 4, 61, 0, 0, 121, 108, 0, 0, 71, 61, 0, 0, 18, 58,
	0, 0, 51, 219, 0, 0, 104, 58, 0, 0, 42, 83, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 245, 68,
	0, 0, 86, 13, 0, 0, 37, 83, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 254, 13, 0, 0, 193, 86, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 90, 201, 0, 0, 81, 252, 0, 0, 131, 246
};

/* DS 0x8009b880 */
DATA_POS cdemd1_pos[300] = {
	{ 0x0, 0xcf4c }, { 0x1a, 0xcf4c }, { 0x34, 0x10420 }, { 0x55, 0x23f60 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x9d, 0x10420 }, { 0xbe, 0x24970 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x108, 0x20820 }, { 0x14a, 0x23cf0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x192, 0x10420 }, { 0x1b3, 0x23ec0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1fb, 0x10420 }, { 0x21c, 0x23e40 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x264, 0x10420 }, { 0x285, 0x23bf0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x2cd, 0x10420 }, { 0x2ee, 0x2502c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x339, 0x10420 }, { 0x35a, 0x22798 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x39f, 0x20820 }, { 0x3e1, 0x22798 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x426, 0x20820 }, { 0x468, 0x22798 },
	{ 0x4ad, 0x56fa }, { 0x4b8, 0x56fa }, { 0x4c3, 0x10420 }, { 0x4e4, 0x1d938 },
	{ 0x520, 0x4a88 }, { 0x52a, 0x4a88 }, { 0x534, 0x8220 }, { 0x545, 0x7fcc },
	{ 0x555, 0x7698 }, { 0x564, 0x7698 }, { 0x573, 0x10420 }, { 0x594, 0x2ab70 },
	{ 0x5ea, 0x48e8 }, { 0x5f4, 0x48e8 }, { 0x5fe, 0x20820 }, { 0x640, 0x2e318 },
	{ 0x69d, 0x5da0 }, { 0x6a9, 0x5da0 }, { 0x6b5, 0x10420 }, { 0x6d6, 0x1fab0 },
	{ 0x716, 0x66d4 }, { 0x723, 0x66d4 }, { 0x730, 0x10420 }, { 0x751, 0xd0c8 },
	{ 0x76c, 0x10fa }, { 0x76f, 0x10fa }, { 0x772, 0x8220 }, { 0x783, 0x1bb8 },
	{ 0x787, 0x389c }, { 0x78f, 0x389c }, { 0x797, 0x8220 }, { 0x7a8, 0x9ad0 },
	{ 0x7bc, 0x67b0 }, { 0x7c9, 0x67b0 }, { 0x7d6, 0x20820 }, { 0x818, 0x2df0c },
	{ 0x874, 0x1efc }, { 0x878, 0x1efc }, { 0x87c, 0x8220 }, { 0x88d, 0x28d8 },
	{ 0x893, 0x4f10 }, { 0x89d, 0x4f10 }, { 0x8a7, 0x10420 }, { 0x8c8, 0x21c84 },
	{ 0x90c, 0x4d68 }, { 0x916, 0x4d68 }, { 0x920, 0x18620 }, { 0x951, 0x23484 },
	{ 0x998, 0x1f4 }, { 0x999, 0x1f4 }, { 0x99a, 0x8220 }, { 0x9ab, 0x4000 },
	{ 0x9b3, 0x1528 }, { 0x9b6, 0x1528 }, { 0x9b9, 0x8220 }, { 0x9ca, 0x6020 },
	{ 0x9d7, 0x5a7a }, { 0x9e3, 0x5a7a }, { 0x9ef, 0x10420 }, { 0xa10, 0x34ecc },
	{ 0xa7a, 0x2464 }, { 0xa7f, 0x2464 }, { 0xa84, 0x8220 }, { 0xa95, 0x1578 },
	{ 0xa98, 0x76ac }, { 0xaa7, 0x76ac }, { 0xab6, 0x18620 }, { 0xae7, 0x2527c },
	{ 0xb32, 0x7d58 }, { 0xb42, 0x7d58 }, { 0xb52, 0x18620 }, { 0xb83, 0x2f264 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xbe2, 0x67b8 }, { 0xbef, 0x67b8 }, { 0xbfc, 0x18620 }, { 0xc2d, 0x320a8 },
	{ 0xc92, 0xb2d4 }, { 0xca9, 0xb2d4 }, { 0xcc0, 0x18620 }, { 0xcf1, 0x42d94 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xd77, 0x5bb4 }, { 0xd83, 0x5bb4 }, { 0xd8f, 0x20620 }, { 0xdd0, 0x1b6d4 },
	{ 0xe07, 0x59f0 }, { 0xe13, 0x59f0 }, { 0xe1f, 0x8220 }, { 0xe30, 0xe7f0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xe4d, 0x5a7a }, { 0xe59, 0x5a7a }, { 0xe65, 0x10420 }, { 0xe86, 0x34ecc },
	{ 0xef0, 0x4e80 }, { 0xefa, 0x4e80 }, { 0xf04, 0x10420 }, { 0xf25, 0xac00 },
	{ 0xf3b, 0x14d6 }, { 0xf3e, 0x14d6 }, { 0xf41, 0x8220 }, { 0xf52, 0xd98 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0xf54, 0x270 }, { 0xf55, 0x270 }, { 0xf56, 0x8220 }, { 0xf67, 0xd810 },
	{ 0xf83, 0x37c }, { 0xf84, 0x37c }, { 0xf85, 0x8220 }, { 0xf96, 0x6574 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0xfa3, 0x18620 }, { 0xfd4, 0xd648 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0xfef, 0x10420 }, { 0x1010, 0x13718 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1037, 0x18620 }, { 0x1068, 0xc600 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1081, 0x18620 }, { 0x10b2, 0x13718 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x10d9, 0x18620 }, { 0x110a, 0x12638 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x112f, 0x10420 }, { 0x1150, 0x120fc },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1175, 0x18620 }, { 0x11a6, 0xc834 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x11c0, 0x18620 }, { 0x11f1, 0x7a0c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1201, 0x18620 }, { 0x1232, 0xc834 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x124c, 0x18620 }, { 0x127d, 0x274cc },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x12cc, 0x10420 }, { 0x12ed, 0x478c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x12f6, 0x10420 }, { 0x1317, 0x1230c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x133c, 0x18620 }, { 0x136d, 0x124d8 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1392, 0x10420 }, { 0x13b3, 0x12ae4 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x13d9, 0x10420 }, { 0x13fa, 0x124d8 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x141f, 0x10420 }, { 0x1440, 0x12a34 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1466, 0x10420 }, { 0x1487, 0x124f0 },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x14ac, 0x18620 }, { 0x14dd, 0x12c2c },
	{ 0x0, 0x0 }, { 0x0, 0x0 }, { 0x1503, 0x18620 }, { 0x1534, 0x130f0 }
};

/* DS 0x8009c1e0 */
UCHAR cdemd1_sum[300] = {
	106, 72, 181, 189, 0, 0, 12, 90, 0, 0, 147, 190, 0, 0, 76, 155,
	0, 0, 0, 0, 0, 0, 13, 132, 0, 0, 235, 217, 0, 0, 202, 130,
	0, 0, 156, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 228, 80, 0, 0, 228, 80,
	222, 253, 250, 172, 18, 51, 26, 50, 162, 140, 37, 140, 20, 53, 219, 29,
	201, 235, 233, 85, 9, 40, 114, 7, 245, 210, 85, 175, 166, 135, 141, 206,
	218, 249, 23, 108, 47, 14, 51, 40, 248, 199, 8, 88, 47, 14, 27, 115,
	49, 18, 13, 232, 232, 201, 117, 93, 86, 119, 217, 252, 222, 255, 22, 117,
	71, 101, 60, 229, 29, 60, 232, 163, 0, 0, 0, 0, 252, 210, 150, 93,
	178, 147, 154, 48, 0, 0, 0, 0, 73, 106, 214, 68, 175, 142, 186, 17,
	0, 0, 0, 0, 86, 119, 60, 252, 40, 11, 71, 222, 76, 108, 51, 27,
	0, 0, 0, 0, 0, 0, 0, 0, 178, 173, 169, 55, 64, 33, 251, 158,
	0, 0, 106, 96, 0, 0, 160, 189, 0, 0, 185, 209, 0, 0, 40, 189,
	0, 0, 4, 61, 0, 0, 121, 108, 0, 0, 0, 0, 0, 0, 18, 58,
	0, 0, 51, 112, 0, 0, 104, 58, 0, 0, 42, 176, 0, 0, 118, 31,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 245, 68,
	0, 0, 86, 13, 0, 0, 37, 83, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 254, 13, 0, 0, 193, 86, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 90, 201, 0, 0, 81, 252, 0, 0, 131, 246
};


/* Top of the free work area: enemy works / parts packets must stay below */
/* the overlay load area at DS 0x80190000 (kept: fixed placement, a limit not a data object). */
#define EM_WORK_LIMIT	0x80190000

/* Size of one ENEMY_WORK in DS retail */
#define EM_WORK_SIZE	0x248

/* Enemy overlay bank size (two banks at Em_ov_bank) */
#define EM_OV_BANK_SIZE	0xd000

/* ------------------------------------------------------------------ */
/* Enemy_work_set                                    DS 0x8001aa38     */
/* Allocates G.Enemy_num ENEMY_WORKs from G.pFree_work, links them     */
/* into G.pEnemy[1..] (slot 0 is the player), clears Be_flg on each    */
/* and sets G.ppEnemy_end one past the last pointer.                   */
/* ------------------------------------------------------------------ */
/* DS 0x8001aa38 (Enemy_work_set) */
void Enemy_work_set(void)
{
	ULONG	Mem_top;
	ULONG	Em_num;
	LONG	i;

	Em_num       = G.Enemy_num;
	i            = -1;
	Mem_top      = (ULONG)G.pFree_work;
	G.pFree_work = G.pFree_work + Em_num * EM_WORK_SIZE;

	do
	{
		i++;
		G.pEnemy[i + 1] = (ENEMY_WORK *)Mem_top;
		((ENEMY_WORK *)Mem_top)->Be_flg = 0;
		Mem_top += EM_WORK_SIZE;
	} while (--Em_num != 0);

	G.ppEnemy_end = &G.pEnemy[i + 2];
}

/* ------------------------------------------------------------------ */
/* Em_set                                            DS 0x8001aaa0     */
/* Empty in the retail binary.                                         */
/* ------------------------------------------------------------------ */
/* DS 0x8001aaa0 (Em_set) */
void Em_set(void)
{
}

/* ------------------------------------------------------------------ */
/* Emd_load                                          DS 0x8001aaa8     */
/* Loads the EMD texture (TIM) and model file for pEm into Mem_top,    */
/* resolves the internal pointer table, optionally relocates data      */
/* (Model_type 0x80 / 0x40), maps the TMD and returns the next free    */
/* address after the mapped model.                                     */
/* ------------------------------------------------------------------ */
/* DS 0x8001aaa8 (Emd_load) */
ULONG Emd_load(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top)
{
	ULONG	nEm;
	ULONG	*Head;
	ULONG	End;
	ULONG	Tmdmem;
	LONG	Zensin;
	LONG	*Chg_id;

	nEm = pEm->Id;

	/* Zombie range 0x50..0x5a: zapping-dependent alternate models */
	if (nEm - 0x50 < 0xb)
	{
		if ((pEm->Id & 1) == 0)
		{
			if (Flg_ck(G.SaveData.Zapping_flg, 5))
				nEm = 0x58;
			if (Flg_ck(G.SaveData.Zapping_flg, 0xf))
				nEm = 0x5a;
		}
		else
		{
			if (Flg_ck(G.SaveData.Zapping_flg, 6))
				nEm = 0x59;
		}
	}
	Zensin = nEm - 0x10;

	/* Texture */
	Emd_load_file((Zensin << 2) | 2, (ULONG *)Mem_top);
	pEm->Clut  = G.Cclut;
	pEm->Tpage = G.Ctpage;
	Mapping_tim((ULONG *)Mem_top);
	Task_sleep(1);

	/* Model / animation data */
	Tmdmem = Emd_load_file((Zensin << 2) | 3, (ULONG *)Mem_top);

	Chg_id = (LONG *)(Mem_top + *(LONG *)Mem_top);
	pEm->pSa_dat         = (ULONG *)(Mem_top + Chg_id[0]);
	pEm->pSeq_t_ptr      = (ULONG *)(Mem_top + Chg_id[1]);
	pEm->pKan_t_ptr      = (ULONG *)(Mem_top + Chg_id[2]);
	pEm->pSub0_seq_t_ptr = (ULONG *)(Mem_top + Chg_id[3]);
	pEm->pSub0_kan_t_ptr = (ULONG *)(Mem_top + Chg_id[4]);
	pEm->pSub1_seq_t_ptr = (ULONG *)(Mem_top + Chg_id[5]);
	pEm->pSub1_kan_t_ptr = (ULONG *)(Mem_top + Chg_id[6]);
	pEm->Poly_rgb        = 0x808080;
	pEm->pTmd            = (ULONG *)(Mem_top + Chg_id[7]);

	/* Model_type 0x80: move the TMD down over the sub0 animation data */
	if (pEm->Model_type & 0x80)
	{
		memcpy16((ULONG *)(Mem_top + Chg_id[3]), (ULONG *)(Mem_top + Chg_id[7]), Tmdmem - Chg_id[7]);
		pEm->pTmd = (ULONG *)(Mem_top + Chg_id[3]);
	}

	/* Model_type 0x40: drop own base animations, share the player's */
	if (pEm->Model_type & 0x40)
	{
		memcpy16((ULONG *)(Mem_top + Chg_id[1]), (ULONG *)(Mem_top + Chg_id[3]), Tmdmem - Chg_id[3]);
		Zensin = (Chg_id[3] - Chg_id[1]) >> 2;
		pEm->pSub0_kan_t_ptr -= Zensin;
		pEm->pTmd            -= Zensin;
		pEm->pSub0_seq_t_ptr -= Zensin;
		pEm->pSub1_kan_t_ptr -= Zensin;
		pEm->pSub1_seq_t_ptr -= Zensin;
		pEm->pSeq_t_ptr       = G.Player.pSeq_t_ptr;
		pEm->pKan_t_ptr       = G.Player.pKan_t_ptr;
	}

	End = Mapping_tmd(0, pEm->pTmd, pEm->Tpage, pEm->Clut);
	Head       = pEm->pTmd;
	pEm->pTmd  = Head + 3;
	pEm->pTmd2 = Head + 10;

	return (ULONG)pEm->pTmd + (End & ~3) - 0xc;
}

/* ------------------------------------------------------------------ */
/* Set_enemy_work                                    DS 0x8001ad3c     */
/* Gets a work for enemy type Id.                                      */
/* 1) An idle work of the same Id is reused directly.                  */
/* 2) Otherwise a work with the same Id (pMoto) is cloned into a new   */
/*    block from G.pFree_work, using the first free pointer slot from  */
/*    G.pEnemy[2] on (32 slots max), and its parts work is linked.     */
/* Returns NULL when there is no prototype, no slot or no memory.      */
/* ------------------------------------------------------------------ */
/* DS 0x8001ad3c (Set_enemy_work) */
ENEMY_WORK *Set_enemy_work(UCHAR Id)
{
	LONG		i;
	LONG		Flg;
	ENEMY_WORK	*pEm;
	ENEMY_WORK	*pMoto;
	ENEMY_WORK	**ppEm;
	ENEMY_WORK	**pTmp = NULL;	/* G.ppEnemy_end before a grow; only read when Flg is set */
	ULONG		Mem_top;

	Flg   = 0;
	pMoto = NULL;

	/* Pass 1: reuse an idle work of the same type, remember a prototype */
	ppEm = G.pEnemy;
	do
	{
		pEm = *ppEm;
		if (pEm->Id == Id)
		{
			pMoto = pEm;
			if ((pEm->Be_flg & 1) == 0)
			{
				pEm->Routine_0 = 0;
				pEm->Routine_1 = 0;
				pEm->Routine_2 = 0;
				pEm->Routine_3 = 0;
				Init_Parts_work(pEm);
				pEm->Be_flg |= 1;
				G.Enemy_num++;
				return pEm;
			}
		}
		ppEm++;
	} while (ppEm != G.ppEnemy_end);

	if (pMoto == NULL)
		return NULL;

	/* Pass 2: first unused pointer slot from G.pEnemy[2] (32 slots) */
	pEm  = NULL;
	ppEm = &G.pEnemy[2];
	for (i = 0x20; ; ppEm++)
	{
		if (((*ppEm)->Be_flg & 1) == 0)
		{
			pEm = *ppEm;
			break;
		}
		if (--i == 0)
			break;
	}
	if (pEm == NULL)
		return NULL;

	if (ppEm >= G.ppEnemy_end)
	{
		pTmp          = G.ppEnemy_end;
		G.ppEnemy_end = ppEm + 1;
		Flg           = 1;
	}

	pEm     = (ENEMY_WORK *)G.pFree_work;
	Mem_top = (ULONG)pEm + EM_WORK_SIZE;
	*ppEm   = pEm;

	if (Mem_top >= EM_WORK_LIMIT)
	{
		pEm->Be_flg = 0;
		if (Flg)
			G.ppEnemy_end = pTmp;
		return NULL;
	}

	G.Enemy_num++;

	/* Clone model / animation data from the prototype */
	pEm->pSa_dat         = pMoto->pSa_dat;
	pEm->pSeq_t_ptr      = pMoto->pSeq_t_ptr;
	pEm->pKan_t_ptr      = pMoto->pKan_t_ptr;
	pEm->pSub0_seq_t_ptr = pMoto->pSub0_seq_t_ptr;
	pEm->pSub0_kan_t_ptr = pMoto->pSub0_kan_t_ptr;
	pEm->pSub1_seq_t_ptr = pMoto->pSub1_seq_t_ptr;
	pEm->pSub1_kan_t_ptr = pMoto->pSub1_kan_t_ptr;
	pEm->pRoom_seq_t_ptr = pMoto->pRoom_seq_t_ptr;
	pEm->pRoom_kan_t_ptr = pMoto->pRoom_kan_t_ptr;
	pEm->Poly_rgb        = 0x808080;
	pEm->pTmd            = pMoto->pTmd;
	pEm->Model_type      = pMoto->Model_type;
	pEm->Clut            = pMoto->Clut;
	pEm->pTmd2           = pEm->pTmd + 7;
	pEm->Tpage           = pMoto->Tpage;
	pEm->Id              = pMoto->Id;
	pEm->Type            = pMoto->Type;
	pEm->nFloor          = pMoto->nFloor;
	pEm->Ground          = pMoto->Ground;
	pEm->pTbefore_func   = pMoto->pTbefore_func;
	pEm->pTafter_func    = pMoto->pTafter_func;
	pEm->Sc_id           = pMoto->Sc_id;
	pEm->Atd[0]          = pMoto->Atd[0];
	pEm->Atd[1]          = pMoto->Atd[1];
	pEm->Atd[2]          = pMoto->Atd[2];
	pEm->Super_matrix    = pMoto->Super_matrix;
	pEm->Super_vector    = pMoto->Super_vector;
	pEm->pSuper          = pMoto->pSuper;
	pEm->Old_x           = pMoto->Old_x;
	pEm->Old_y           = pMoto->Old_y;
	pEm->Old_z           = pMoto->Old_z;
	pEm->Size            = pMoto->Size;
	pEm->Em_set_flg      = pMoto->Em_set_flg;
	pEm->nOba            = pMoto->nOba;
	pEm->Pos_x           = pMoto->Pos_x;
	pEm->Pos_y           = pMoto->Pos_y;

	/* Identity rotation (written as words: M[0][0..1], M[1][1..2], M[2][2]+pad) */
	pEm->M[0][0] = 0x1000;	pEm->M[0][1] = 0;
	pEm->M[1][1] = 0x1000;	pEm->M[1][2] = 0;
	pEm->M[2][2] = 0x1000;
	pEm->Be_flg  = 1;
	pEm->Cdir_x  = 0;
	pEm->Cdir_y  = 0;
	pEm->Cdir_z  = 0;
	pEm->M[0][2] = 0;		pEm->M[1][0] = 0;
	pEm->M[2][0] = 0;		pEm->M[2][1] = 0;
	pEm->Work_no = (UCHAR)i;

	pEm->Routine_0     = 0;
	pEm->Routine_1     = 0;
	pEm->Routine_2     = 0;
	pEm->Routine_3     = 0;
	pEm->Damage_flg    = 0;
	pEm->Damage_cnt    = 0;
	pEm->Status_flg    = 0;
	pEm->In_screen     = 0;
	pEm->Sce_flg       = 0;
	pEm->Spl_flg       = 0;
	pEm->Parts0_pos_y  = 0x1000;
	pEm->Sce_free0     = 0;
	pEm->Sce_free1     = 0;
	pEm->Sce_free2     = 0;
	pEm->Sce_free3     = 0;
	pEm->Timer0        = 0;
	pEm->Timer1        = 0;
	pEm->Pos_z         = pMoto->Pos_z;
	pEm->pTbefore_func = NULL;
	pEm->Root_ck_cnt   = G.Enemy_num & 7;
	pEm->pTafter_func  = NULL;
	pEm->pOn_om        = NULL;

	/* Link the parts work behind the enemy work */
	Mem_top = PartsWork_set(pEm, Mem_top);
	if (Mem_top < EM_WORK_LIMIT && Mem_top + Mem_ck_Parts_work(pMoto) < EM_WORK_LIMIT)
	{
		Mem_top = PartsWork_link(pEm, Mem_top, pEm->pKan_t_ptr, 0);
		Sa_dat_set(pEm, pEm->pSa_dat);
		G.pFree_work = (UCHAR *)Mem_top;
		return pEm;
	}

	/* Out of memory: roll back */
	pEm->Be_flg = 0;
	if (Flg)
		G.ppEnemy_end = pTmp;
	G.Enemy_num--;
	return NULL;
}

/* ------------------------------------------------------------------ */
/* Kill_enemy_work                                   DS 0x8001b250     */
/* ------------------------------------------------------------------ */
void Kill_enemy_work(ENEMY_WORK *pEm)
{
	pEm->Be_flg    = 0;
	pEm->Routine_0 = 0;
	pEm->Routine_1 = 0;
	pEm->Routine_2 = 0;
	pEm->Routine_3 = 0;
	G.Enemy_num--;
}

/* ------------------------------------------------------------------ */
/* Em_move_tbl_set                                   DS 0x8001b274     */
/* Sets the move-table entry points inside both enemy overlay banks    */
/* (G.Em_ov_mtbl[bank][Id - 0x10]) and invalidates the bank info.      */
/* Ids 0x10..0x1f share overlay EM10.                                  */
/* ------------------------------------------------------------------ */
/* DS builds every table entry with its own "lui at / sw" (absolute G field address) and never
 * holds the G base in a register. CCPSX would share one G base register across the stores, so each
 * store is followed by an empty volatile asm, which makes CCPSX forget the cached address (no code). */
#define EM_BARRIER() __asm__ volatile("" : : )

/* DS 0x8001b274 (Em_move_tbl_set) */
void Em_move_tbl_set(void)
{
	VOID *p_b0_2c0;	/* values DS keeps in a register and stores several times */
	VOID *p_b0_4;
	VOID *p_b1_2c0;
	VOID *p_b1_4;

	G.Em_bin_info[0].Id = 0xff; EM_BARRIER();
	G.Em_bin_info[1].Id = 0xff; EM_BARRIER();
	p_b0_2c0 = &Em_ov_bank[0][0x2c0];
	G.Em_ov_mtbl[0][0x00] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x01] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x02] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x03] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x04] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x05] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x06] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x07] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x08] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x09] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0a] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0b] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0c] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0d] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0e] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x0f] = p_b0_2c0; EM_BARRIER();
	G.Em_ov_mtbl[0][0x10] = &Em_ov_bank[0][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x11] = &Em_ov_bank[0][0x13c]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x12] = &Em_ov_bank[0][0x2eb0]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x13] = &Em_ov_bank[0][0x190]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x14] = &Em_ov_bank[0][0x2d80]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x15] = &Em_ov_bank[0][0x0c8]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x16] = &Em_ov_bank[0][0x01c]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x17] = &Em_ov_bank[0][0x0e0]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x18] = &Em_ov_bank[0][0x0fc]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x19] = &Em_ov_bank[0][0x078]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x1a] = &Em_ov_bank[0][0x1194]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x1b] = &Em_ov_bank[0][0x187c]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x1c] = &Em_ov_bank[0][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x1d] = &Em_ov_bank[0][0x018]; EM_BARRIER();
	p_b0_4 = &Em_ov_bank[0][0x004];
	G.Em_ov_mtbl[0][0x1e] = p_b0_4; EM_BARRIER();
	G.Em_ov_mtbl[0][0x1f] = &Em_ov_bank[0][0x034]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x20] = &Em_ov_bank[0][0x13c]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x21] = &Em_ov_bank[0][0x0cc]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x23] = &Em_ov_bank[0][0x098]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x24] = &Em_ov_bank[0][0x134]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x26] = &Em_ov_bank[0][0x0bc]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x27] = &Em_ov_bank[0][0x178]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x28] = &Em_ov_bank[0][0x018]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x2a] = &Em_ov_bank[0][0x030]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x2b] = &Em_ov_bank[0][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x2e] = &Em_ov_bank[0][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x2f] = &Em_ov_bank[0][0x004]; EM_BARRIER();
	p_b1_2c0 = &Em_ov_bank[1][0x2c0];
	G.Em_ov_mtbl[1][0x00] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x01] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x02] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x03] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x04] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x05] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x06] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x07] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x08] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x09] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0a] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0b] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0c] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0d] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0e] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x0f] = p_b1_2c0; EM_BARRIER();
	G.Em_ov_mtbl[1][0x10] = &Em_ov_bank[1][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x11] = &Em_ov_bank[1][0x13c]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x12] = &Em_ov_bank[1][0x2eb0]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x13] = &Em_ov_bank[1][0x190]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x14] = &Em_ov_bank[1][0x2d80]; EM_BARRIER();
	G.Em_ov_mtbl[0][0x29] = p_b0_4; EM_BARRIER();
	G.Em_ov_mtbl[1][0x15] = &Em_ov_bank[1][0x0c8]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x16] = &Em_ov_bank[1][0x01c]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x17] = &Em_ov_bank[1][0x0e0]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x18] = &Em_ov_bank[1][0x0fc]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x19] = &Em_ov_bank[1][0x078]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x1a] = &Em_ov_bank[1][0x1194]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x1b] = &Em_ov_bank[1][0x187c]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x1c] = &Em_ov_bank[1][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x1d] = &Em_ov_bank[1][0x018]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x1f] = &Em_ov_bank[1][0x034]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x20] = &Em_ov_bank[1][0x13c]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x21] = &Em_ov_bank[1][0x0cc]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x23] = &Em_ov_bank[1][0x098]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x24] = &Em_ov_bank[1][0x134]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x26] = &Em_ov_bank[1][0x0bc]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x27] = &Em_ov_bank[1][0x178]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x28] = &Em_ov_bank[1][0x018]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x2a] = &Em_ov_bank[1][0x030]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x2b] = &Em_ov_bank[1][0x004]; EM_BARRIER();
	G.Em_ov_mtbl[1][0x2e] = &Em_ov_bank[1][0x004]; EM_BARRIER();
	p_b1_4 = &Em_ov_bank[1][0x004];
	G.Em_ov_mtbl[1][0x1e] = p_b1_4; EM_BARRIER();
	G.Em_ov_mtbl[1][0x29] = p_b1_4; EM_BARRIER();
	G.Em_ov_mtbl[1][0x2f] = &Em_ov_bank[1][0x004]; EM_BARRIER();
}

/* ------------------------------------------------------------------ */
/* Em_bin_load                                       DS 0x8001b710     */
/* Makes sure the move overlay (EMxx.BIN) for enemy Id is resident in  */
/* one of the two overlay banks. Ids 0x10..0x1f share overlay 0x10.    */
/* If already loaded the bank is just marked used; otherwise a free    */
/* bank is chosen and the read is started. G.Em_move_tbl[Id - 0x10]    */
/* is pointed at the move table inside the chosen bank.                */
/* NOTE (faithful to DS): when both banks are in use, i is left at -1  */
/* (Id 0x10 path) or 2 (other Ids) and the out-of-range bank is used.  */
/* ------------------------------------------------------------------ */
/* DS 0x8001b710 (Em_bin_load) */
void Em_bin_load(ULONG Id)
{
	LONG	i;
	ULONG	Em_id;
	ULONG	nEm;
	ULONG	unused_pad[2];	/* retail reserves 8 more stack bytes at 0x10(sp) and never reads them */

	if (G.Em_ov_mtbl[0][Id - 0x10] == NULL)
		return;

	Em_id = Id;
	if (Id - 0x10 < 0x10)
		Em_id = 0x10;

	if (Em_id == 0x10)
	{
		for (i = 1; i >= 0; i--)
		{
			if (G.Em_bin_info[i].Id == 0x10)
			{
				G.Em_bin_info[i].Use     = 1;
				G.Em_move_tbl[Id - 0x10] = G.Em_ov_mtbl[i][Id - 0x10];
				return;
			}
		}

		if (G.Em_bin_info[1].Use == 0)
			i = 1;
		else if (G.Em_bin_info[0].Use == 0)
			i = 0;
		/* else: i == -1 */
	}
	else
	{
		for (i = 0; i < 2; )
		{
			if (G.Em_bin_info[i++].Id == Em_id)
			{
				G.Em_bin_info[i - 1].Use = 1;
				return;
			}
		}

		if (G.Em_bin_info[0].Use == 0)
			i = 0;
		else if (G.Em_bin_info[1].Use == 0)
			i = 1;
		/* else: i == 2 */
	}

	/* load: */
	G.Em_bin_info[i].Use     = 1;
	G.Em_bin_info[i].Id      = (USHORT)Em_id;
	G.Em_move_tbl[Id - 0x10] = G.Em_ov_mtbl[i][Id - 0x10];

	nEm = ((Em_id - 0x10) << 2) + 1 - i;

	CD.Fno           = CDEMD0_EMS;
	CD.Fsize         = cdemd0_pos[nEm].Size;
	CD.Fsector       = cdemd0_pos[nEm].Offset;
	floc[CDEMD0_EMS].sum  = cdemd0_sum[nEm];
	CD.Fsector      += (floc[CDEMD0_EMS].sec_high << 16) + floc[CDEMD0_EMS].sector;
	CD.Fsum          = cdemd0_sum[nEm];

	Cd_read(CDEMD0_EMS, (ULONG *)Em_ov_bank[i], 3, (UCHAR *)"EMS");
}

/* ------------------------------------------------------------------ */
/* Emd_load_file                                     DS 0x8001b934     */
/* (retail-only) Reads EMD file Em_no to Address. Leon-side players    */
/* read the PL0 enemy archive (file 0x1d5), Claire-side players the    */
/* PL1 archive (file 0x1d6). Returns the Cd_read result (file size).   */
/* ------------------------------------------------------------------ */
/* DS 0x8001b934 (Emd_load_file) */
ULONG Emd_load_file(ULONG Em_no, ULONG *Address)
{
	ULONG	File_no;
	ULONG	Fsec;

	if (G.Player.Id == 0xd || G.Player.Id == 0xb
	 || (G.Player.Id != 0xe && (G.Player.Id & 1) == 0))
	{
		File_no         = CDEMD0_EMS;
		CD.Fno          = CDEMD0_EMS;
		EM_BARRIER();
		CD.Fsize        = cdemd0_pos[Em_no].Size;
		EM_BARRIER();
		Fsec            = cdemd0_pos[Em_no].Offset;
		CD.Fsector      = Fsec;
		EM_BARRIER();
		floc[CDEMD0_EMS].sum = cdemd0_sum[Em_no];
		EM_BARRIER();
		CD.Fsector      = Fsec + ((floc[CDEMD0_EMS].sec_high << 16) + floc[CDEMD0_EMS].sector);
		CD.Fsum         = cdemd0_sum[Em_no];
	}
	else
	{
		File_no         = CDEMD1_EMS;
		CD.Fno          = CDEMD1_EMS;
		EM_BARRIER();
		CD.Fsize        = cdemd1_pos[Em_no].Size;
		EM_BARRIER();
		Fsec            = cdemd1_pos[Em_no].Offset;
		CD.Fsector      = Fsec;
		EM_BARRIER();
		floc[CDEMD1_EMS].sum = cdemd1_sum[Em_no];
		EM_BARRIER();
		CD.Fsector      = Fsec + ((floc[CDEMD1_EMS].sec_high << 16) + floc[CDEMD1_EMS].sector);
		CD.Fsum         = cdemd1_sum[Em_no];
	}

	return Cd_read(File_no, Address, 3, (UCHAR *)"EMD");
}
