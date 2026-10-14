/*
 *	SCE2.C - SCD opcode implementations (effects / enemies / audio / light / misc)
 *	DS retail range: 0x80056428 (Se_on) .. 0x80059437 (end of Vib_fade_set, before Snd_sys_init)
 *
 *	Every body below was checked against the SLUS_00748 disassembly/decompile.
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include "OM.H"
#include "CONFIG.H"
#include "ROOM.H"
#include "ESP.H"		/* Esp_call*, Esp_kill*, Esp_ctrl, Esp[] */
#include "SCE0.H"		/* Em_kind_search, Stage_room_to_mapno */
#include "MIZU_SYS.H"	/* Div_prim_coord */
#include "SCE2.H"
#include "CD.H"
#include "DIEDEMO.H"
#include "DOOR.H"
#include "EM_SET.H"
#include "EM_SUB.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "LINE_SCR.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MOVIE.H"
#include "PAD.H"
#include "PL_WEP.H"
#include "SABUN.H"
#include "SCA.H"
#include "SCE1.H"
#include "SND.H"
#include "SPL.H"
#include "STATUS.H"
#include "SUB.H"
#include "GAME.H"
#include "PLAYER.H"
#include "SPL_SCE.H"
#include "FILECODE.H"
#include "flag.h"
#include "sce_defines.h"

/* DS 0x80011544: 3-character rodata label passed to Cd_read by Sce_Item_get
 * (text not resolved in Ghidra). */
static UCHAR Itp_mess[4];

/* --------------------------------------------------------------------------
 *	File statics / globals owned by SCE2
 * ------------------------------------------------------------------------ */

/* Sub-player (partner) speed tables: [Id][0] = ?, [Id][1] = motion speed.
 * Index = Id + 0x10 for Id < 0x10, Id - 0x40 otherwise.
 * NOTE: initializer bytes are not shown by Ghidra (no data defined); must be
 * extracted from the DS image. */
extern UCHAR Subpl_walk[32][2];			/* DS 0x800a7968 */
extern UCHAR Subpl_back[32][2];			/* DS 0x800a79a8 */
extern UCHAR Subpl_dir[32][2];			/* DS 0x800a79e8 */
extern UCHAR Subpl_ato[32][2];			/* DS 0x800a7a28 */
extern UCHAR Subpl_run[32][2];			/* DS 0x800a7a68 */
static UCHAR Em4a_zobiparts_tbl[7][2];	/* DS 0x800a7aa8 (unused in SCE2) */

UCHAR Itp_check_sum[100] = {
	0x2b, 0x0f, 0x94, 0xb6, 0xc9, 0x96, 0xef, 0xed, 0x52, 0x31,
	0x31, 0x31, 0xe0, 0xb2, 0x0a, 0x89, 0xf1, 0xaf, 0x3b, 0xdb,
	0x56, 0x0c, 0xde, 0x82, 0x71, 0x12, 0xa6, 0x0e, 0xaf, 0x8e,
	0xbe, 0x1e, 0x69, 0x74, 0x5a, 0x62, 0xf7, 0x6b, 0x3b, 0x4b,
	0xca, 0xf0, 0x9b, 0x8f, 0x3f, 0xc9, 0x25, 0x25, 0x58, 0x28,
	0x94, 0x03, 0x9d, 0x5e, 0x97, 0x3f, 0xca, 0xbb, 0x21, 0x7c,
	0x8d, 0xed, 0x68, 0xb7, 0x4b, 0x9d, 0x0f, 0xbf, 0x2b, 0x2b,
	0x2b, 0xd1, 0x22, 0x36, 0x5b, 0xce, 0xfa, 0xfa, 0x29, 0x68,
	0x2b, 0xb6, 0x52, 0x28, 0x82, 0x79, 0x94, 0xa1, 0xf2, 0x3b,
	0xfd, 0xe9, 0xf2, 0x92, 0x98, 0xd3, 0x9b, 0x78, 0x7c, 0x0f,
};				/* DS 0x800a7ab8 - item TIM checksums */
SCE_EM_SET_WORK *pSce_em;				/* DS 0x800d5be8 */

/* ==========================================================================
 *	Script record layouts. The handlers below read these records in place
 *	(pSce->Data points at the record). Each struct is the layout of one
 *	handler's record; the retail instruction that reads a field is cited.
 *	Where the retail load is unsigned but the port uses a signed field (or the
 *	reverse), the port's type is kept so the assembly stays identical.
 * ======================================================================== */

/* Se_on: SHORT D0 at +2, SHORT D1 at +4, SHORT vx/vy/vz at +6/+8/+10
 * (retail Se_on 0x80056428: lh a0,0x2(s0); lh a1,0x4(s0); lh v0,0x6(s0); lh v0,0x8(s0); lh v0,0xa(s0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT D0;
	SHORT D1;
	SHORT vx;
	SHORT vy;
	SHORT vz;
} SCE_SE_ON_REC;

/* Sca_id_set: USHORT Id at +2 (retail Sca_id_set 0x8005655c: lhu v0,0x2(a2)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Id;
} SCE_SCA_ID_REC;

/* Sce_espr_on: USHORT D0 at +2, USHORT Bit at +6, SHORT vx/vy/vz at +8/+10/+12, SHORT dir_y at +14
 * (retail Sce_espr_on 0x800565a4: lhu s0,0x2(s1); lhu a1,0x6(s1); lhu v1,0x8(s1); lh a1,0xe(s1)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT D0;
	UCHAR Dummy1[2];
	USHORT Bit;
	SHORT vx;
	SHORT vy;
	SHORT vz;
	SHORT dir_y;
} SCE_ESPR_REC;

/* Sce_espr_on2: as SCE_ESPR_REC but dir_y is USHORT (retail Sce_espr_on2 0x80056644: lhu a1,0xe(s1)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT D0;
	UCHAR Dummy1[2];
	USHORT Bit;
	SHORT vx;
	SHORT vy;
	SHORT vz;
	USHORT dir_y;
} SCE_ESPR2_REC;

/* Sce_espr3d_on: SCE_ESPR_REC plus SHORT Svec2[3] at +14/+16/+18 and SHORT dir_y at +20
 * (retail Sce_espr3d_on 0x800566f0: lhu v1,0x8(s0) .. lhu v1,0x12(s0); lh a1,0x14(s0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT D0;
	UCHAR Dummy1[2];
	USHORT Bit;
	SHORT vx;
	SHORT vy;
	SHORT vz;
	SHORT Svec2[3];
	SHORT dir_y;
} SCE_ESPR3D_REC;

/* Sce_espr3d_on2: as SCE_ESPR3D_REC but dir_y is USHORT (retail Sce_espr3d_on2 0x800567b4: lhu a1,0x14(s0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT D0;
	UCHAR Dummy1[2];
	USHORT Bit;
	SHORT vx;
	SHORT vy;
	SHORT vz;
	SHORT Svec2[3];
	USHORT dir_y;
} SCE_ESPR3D2_REC;

/* Plc_dest: SHORT Dest_x at +4 and Dest_z at +6 (retail Plc_dest 0x80056bd8: lhu v0,0x4(a3); lhu v0,0x6(a3);
 * retail loads are unsigned, the port's are signed) */
typedef struct {
	UCHAR Dummy[4];
	SHORT Dest_x;
	SHORT Dest_z;
} SCE_PLC_DEST_REC;

/* Plc_neck: SHORT Px/Py/Pz at +2/+4/+6 and USHORT Attr at +8 (retail Plc_neck 0x80056e5c: lh a3,0x2(v0);
 * lh t0,0x4(v0); lh t1,0x6(v0); lh t2,0x8(v0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Px;
	SHORT Py;
	SHORT Pz;
	USHORT Attr;
} SCE_PLC_NECK_REC;

/* Plc_stop: the four bytes of SCE_TASK.R_no_bak (ULONG at +0x170 of SCE_TASK; retail Plc_stop 0x80056ff0 stores and
 * loads it at sw v1,0x170(a0) / lw v0,0x170(a0)). Routine_0..3 are its byte lanes, low byte first. */
typedef struct {
	UCHAR Routine_0;
	UCHAR Routine_1;
	UCHAR Routine_2;
	UCHAR Routine_3;
} SCE_ROUT_BYTES;

/* Plc_flg: USHORT Val at +2 (retail Plc_flg 0x80057038: lhu a2,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Val;
} SCE_PLC_FLG_REC;

/* Plc_rot: SHORT Val at +2 (retail Plc_rot 0x800570bc: lhu v0,0x2(a1); the port reads it signed) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Val;
} SCE_PLC_ROT_REC;

/* Layout of the ITPS_ITP file image that Sce_Item_get loads at G.pFree_work (Fsize 0x3000, 2 sectors read). Set_item
 * (DS 0x80069adc) hands the table address to Pix_trans (DS 0x80069bb4), which uploads a 20x30 halfword rectangle
 * (sh 0x14 and 0x1e at sp+0x14/0x16, LoadImage) and, for items that need two icons, a second one at +0x4b0 (1200
 * bytes = 20 * 30 * 2), so the table is two 20x30 16-bit images. ST_SUB02.H St_item_tbl (0x801a21a0) is the same
 * object, 0x21a0 into St_load_buf (0x801a0000). Only the table offset and the 0x3000 file size are proven; the
 * data before and after it is not decoded. */
typedef struct {
	UCHAR	Head[0x21a0];
	USHORT	Item_img[2][30][20];
	UCHAR	Tail[0x3000 - 0x21a0 - sizeof(USHORT) * 2 * 30 * 20];
} SCE_ITP_BUF;

/* Sce_key_ck and Sce_trg_ck: USHORT Mask at +2 (retail Sce_key_ck 0x80057da8: lhu v1,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Mask;
} SCE_KEY_REC;

/* Sce_fade_set: SHORT Add at +4 (retail Sce_fade_set 0x80057ef0: lhu s3,0x4(s2), sign-extended) */
typedef struct {
	UCHAR Dummy[4];
	SHORT Add;
} SCE_FADE_REC;

/* Sce_fade_adjust: SHORT Kido at +2 (retail Sce_fade_adjust 0x80057fd8: lh a1,0x2(s0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Kido;
} SCE_FADE_ADJ_REC;

/* Xa_on: USHORT no at +2 (retail Xa_on 0x80058028: lhu a1,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT no;
} SCE_XA_REC;

/* Sce_line_start: USHORT Prio at +2 (retail Sce_line_start 0x80058214: lhu a1,0x2(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Prio;
} SCE_LINE_START_REC;

/* Sce_line_main: SHORT Slide_ofs at +2 and Copy_ofs at +4 (retail Sce_line_main 0x80058260: lh a1,0x2(v0); lh a2,0x4(v0)) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Slide_ofs;
	SHORT Copy_ofs;
} SCE_LINE_MAIN_REC;

/* Light_pos_set and Light_pos_set2: SHORT Val at +4 (retail Light_pos_set 0x800589d4: lhu v0,0x4(a1), the port
 * reads it signed) */
typedef struct {
	UCHAR Dummy[4];
	USHORT Val;
} SCE_LIGHT_POS_REC;

/* Sce_bgmtbl_set: USHORT Unk_02, Unk_04, Unk_06 at +2/+4/+6, no names decoded
 * (retail Sce_bgmtbl_set 0x80057eb0: lhu a0,0x2(v0); 0x80057eb4: lhu v1,0x4(v0); 0x80057eb8: lhu v0,0x6(v0)) */
typedef struct {
	UCHAR Unk_00[2];
	USHORT Unk_02;
	USHORT Unk_04;
	USHORT Unk_06;
} SCE_BGMTBL_REC;

/* Kage_set: UCHAR bytes +1..+5 (retail Kage_set 0x800588b4: lbu v1,0x1(a0); 0x80058910: lb v0,0x2(a0);
 * 0x80058938/3c/40: lbu a2/a1/v0 at +3/+4/+5), USHORT Unk_06/08/0a/0c at +6/+8/+10/+12 (retail
 * 0x80058964/78/8c/a0: lhu v0,0x6/0x8/0xa/0xc(v0)). Names not decoded. */
typedef struct {
	UCHAR Unk_00;
	UCHAR Unk_01;
	UCHAR Unk_02;
	UCHAR Unk_03;
	UCHAR Unk_04;
	UCHAR Unk_05;
	USHORT Unk_06;
	USHORT Unk_08;
	USHORT Unk_0a;
	USHORT Unk_0c;
} SCE_KAGE_REC;

/* Sce_parts_bomb: UCHAR Unk_02 (parts index), Unk_03..05 (colour bytes) and SHORT Unk_06/08/0a/0c/0e (retail
 * Sce_parts_bomb 0x80058f80: lbu v1,0x2(a3); 0x80058fb8/bc/c0: lbu at +3/+4/+5; 0x80058fdc..0x8005900c: lhu at
 * +6/+8/+a/+c/+e; the port keeps them signed, so the assembly stays identical) */
typedef struct {
	UCHAR Unk_00;
	UCHAR Unk_01;
	UCHAR Unk_02;
	UCHAR Unk_03;
	UCHAR Unk_04;
	UCHAR Unk_05;
	SHORT Unk_06;
	SHORT Unk_08;
	SHORT Unk_0a;
	SHORT Unk_0c;
	SHORT Unk_0e;
} SCE_PARTS_BOMB_REC;

/* Light_kido_set: USHORT Val at +2 (retail Light_kido_set 0x80058a8c: lhu a2,0x2(a1)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Val;
} SCE_LIGHT_KIDO_REC;

/* Light_kido_set2: USHORT Val at +4 (retail Light_kido_set2 0x80058bfc: lhu a2,0x4(a1)) */
typedef struct {
	UCHAR Dummy[4];
	USHORT Val;
} SCE_LIGHT_KIDO2_REC;

/* Sce_scr_move: SHORT Scrl_y at +2 (retail Sce_scr_move 0x80058cb8: lhu v0,0x2(v0), the port reads it signed) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Scrl_y;
} SCE_SCR_MOVE_REC;

/* Mirror_set: SHORT Mirror_pos/Mirror_min/Mirror_max at +2/+4/+6 (retail Mirror_set 0x80058ef8: lhu v0,0x2/0x4/0x6;
 * the port reads them signed) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Mirror_pos;
	SHORT Mirror_min;
	SHORT Mirror_max;
} SCE_MIRROR_REC;

/* Sce_parts_down: SHORT fields at +2..+14 (retail Sce_parts_down 0x80059030: lhu v0,0x2(a2) .. lhu v0,0xe(a2);
 * the +8 field is stored as a byte into Timer1) */
typedef struct {
	UCHAR Dummy[2];
	SHORT Old_x;
	SHORT Old_z;
	SHORT Old_y;
	SHORT Timer1;
	SHORT Old_x2;
	SHORT Old_y2;
	SHORT Old_z2;
} SCE_PARTS_DOWN_REC;

/* Sce_em_pos_ck: USHORT Att at +4 (retail Sce_em_pos_ck 0x8005922c: lhu a2,0x4(a3)) */
typedef struct {
	UCHAR Dummy[4];
	USHORT Att;
} SCE_EM_POS_REC;

/* Vib_set0 and Vib_set1: USHORT Time at +2 and USHORT Attr at +4 (retail Vib_set0 0x80059348: lhu a0,0x2(v0);
 * lhu a1,0x4(v0)) */
typedef struct {
	UCHAR Dummy[2];
	USHORT Time;
	USHORT Attr;
} SCE_VIB_REC;

/* Vib_fade_set: USHORT Time at +4 and USHORT Attr at +6 (retail Vib_fade_set 0x800593e4: lhu a0,0x4(v0);
 * lhu a3,0x6(v0)) */
typedef struct {
	UCHAR Dummy[4];
	USHORT Time;
	USHORT Attr;
} SCE_VIB_FADE_REC;


/* ==========================================================================
 *	Se_on						DS 0x80056428
 *	Play a 3D sound effect at an object / enemy position (+ offset)
 * ======================================================================== */
/* DS 0x80056428 (Se_on) */
ULONG Se_on(SCE_TASK *pSce)
{
	UCHAR		*pData;
	UINT		D0;
	UINT		D1;
	ENEMY_WORK	*pW;		/* undefined for kind >= 5 (retail leaves in_v1 unset) */
	VECTOR		Vec;

	pData = pSce->Data;
	D1    = (UINT)((SCE_SE_ON_REC *)pData)->D1;
	D0    = (UINT)((SCE_SE_ON_REC *)pData)->D0;

	if ((D1 & 0xff) == WORK_TYPE_POSITION)
	{
		Vec.vx = 0;
		Vec.vy = 0;
		Vec.vz = 0;
	}
	else
	{
		switch (D1 & 0xff)
		{
		case WORK_TYPE_PLAYER:
			pW = G.pEnemy[0];
			break;
		case WORK_TYPE_SUB_PLAYER:
			pW = G.pEnemy[1];
			break;
		case WORK_TYPE_ENEMY:
			pW = G.pEnemy[(D1 >> 8) + 2];
			break;
		case WORK_TYPE_OBJECT:
			pW = (ENEMY_WORK *)&G.Ob_model[D1 >> 8];
			break;
		}
		Vec.vx = pW->Pos_x;
		Vec.vy = pW->Pos_y;
		Vec.vz = pW->Pos_z;
	}
	Vec.vx = ((SCE_SE_ON_REC *)pData)->vx + Vec.vx;
	Vec.vy = ((SCE_SE_ON_REC *)pData)->vy + Vec.vy;
	Vec.vz = ((SCE_SE_ON_REC *)pData)->vz + Vec.vz;

	Snd_se_stad(((ULONG)pData[1] << 24) | ((D0 & 0xff) << 16) | (D0 >> 8), &Vec);
	pSce->Data = pData + 12;
	return 1;
}

/* ==========================================================================
 *	Sca_id_set					DS 0x8005655c
 *	Replace the upper 9 bits of a collision block's Id
 * ======================================================================== */
/* DS 0x8005655c (Sca_id_set) */
ULONG Sca_id_set(SCE_TASK *pSce)
{
	UCHAR		*pData;
	SCA_DATA	*pSca;

	pData = pSce->Data;
	pSca  = (SCA_DATA *)G.pRoom->pSca + pData[1] + 1;
	pSca->Id = (pSca->Id & 0x7f) | (((SCE_SCA_ID_REC *)pData)->Id & 0xff80);
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_on					DS 0x800565a4
 * ======================================================================== */
ULONG Sce_espr_on(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	D0;
	MATRIX	*pMat;
	SVECTOR	Svec;
	SVECTOR	Pad;

	(void)Pad;

	pData = pSce->Data;
	D0    = ((SCE_ESPR_REC *)pData)->D0;
	pMat  = Get_matrix((signed char)pData[4], (signed char)pData[5]);
	Svec.vx = ((SCE_ESPR_REC *)pData)->vx;
	Svec.vy = ((SCE_ESPR_REC *)pData)->vy;
	Svec.vz = ((SCE_ESPR_REC *)pData)->vz;
	Esp_call((D0 << 24) | ((D0 >> 8) << 16) | ((SCE_ESPR_REC *)pData)->Bit,
			 ((SCE_ESPR_REC *)pData)->dir_y, pMat, &Svec);
	pSce->Data = pData + 16;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_on2				DS 0x80056644
 * ======================================================================== */
ULONG Sce_espr_on2(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	D0;
	MATRIX	*pMat;
	SVECTOR	Svec;
	SVECTOR	Pad;

	(void)Pad;

	pData = pSce->Data;
	D0    = ((SCE_ESPR2_REC *)pData)->D0;
	pMat  = Get_matrix((signed char)pData[4], (signed char)pData[5]);
	Svec.vx = ((SCE_ESPR2_REC *)pData)->vx;
	Svec.vy = ((SCE_ESPR2_REC *)pData)->vy;
	Svec.vz = ((SCE_ESPR2_REC *)pData)->vz;
	Esp_call2((D0 << 24) | ((D0 >> 8) << 16) | ((SCE_ESPR2_REC *)pData)->Bit,
			  ((ULONG)((SCE_ESPR2_REC *)pData)->dir_y << 16) | pData[1], pMat, &Svec);
	pSce->Data = pData + 16;
	return 1;
}

/* ==========================================================================
 *	Sce_espr3d_on				DS 0x800566f0
 * ======================================================================== */
ULONG Sce_espr3d_on(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	D0;
	MATRIX	*pMat;
	SVECTOR	Svec;
	SVECTOR	Svec2;

	pData = pSce->Data;
	D0    = ((SCE_ESPR3D_REC *)pData)->D0;
	pMat  = Get_matrix((signed char)pData[4], (signed char)pData[5]);
	Svec.vx  = ((SCE_ESPR3D_REC *)pData)->vx;
	Svec.vy  = ((SCE_ESPR3D_REC *)pData)->vy;
	Svec.vz  = ((SCE_ESPR3D_REC *)pData)->vz;
	Svec2.vx = ((SCE_ESPR3D_REC *)pData)->Svec2[0];
	Svec2.vy = ((SCE_ESPR3D_REC *)pData)->Svec2[1];
	Svec2.vz = ((SCE_ESPR3D_REC *)pData)->Svec2[2];
	Esp_call3D((D0 << 24) | ((D0 >> 8) << 16) | ((SCE_ESPR3D_REC *)pData)->Bit,
			   ((SCE_ESPR3D_REC *)pData)->dir_y, pMat, &Svec, &Svec2);
	pSce->Data = pData + 22;
	return 1;
}

/* ==========================================================================
 *	Sce_espr3d_on2				DS 0x800567b4
 * ======================================================================== */
ULONG Sce_espr3d_on2(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	D0;
	MATRIX	*pMat;
	SVECTOR	Svec;
	SVECTOR	Svec2;

	pData = pSce->Data;
	D0    = ((SCE_ESPR3D2_REC *)pData)->D0;
	pMat  = Get_matrix((signed char)pData[4], (signed char)pData[5]);
	Svec.vx  = ((SCE_ESPR3D2_REC *)pData)->vx;
	Svec.vy  = ((SCE_ESPR3D2_REC *)pData)->vy;
	Svec.vz  = ((SCE_ESPR3D2_REC *)pData)->vz;
	Svec2.vx = ((SCE_ESPR3D2_REC *)pData)->Svec2[0];
	Svec2.vy = ((SCE_ESPR3D2_REC *)pData)->Svec2[1];
	Svec2.vz = ((SCE_ESPR3D2_REC *)pData)->Svec2[2];
	Esp_call3D2((D0 << 24) | ((D0 >> 8) << 16) | ((SCE_ESPR3D2_REC *)pData)->Bit,
				((ULONG)((SCE_ESPR3D2_REC *)pData)->dir_y << 16) | pData[1], pMat, &Svec, &Svec2);
	pSce->Data = pData + 22;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_kill				DS 0x80056884
 * ======================================================================== */
ULONG Sce_espr_kill(SCE_TASK *pSce)
{
	UCHAR	*pData;
	MATRIX	*pMat;

	pData = pSce->Data;
	pMat  = Get_matrix((signed char)pData[3], (signed char)pData[4]);
	Esp_kill(pSce->Data[1], pSce->Data[2], pMat);
	pSce->Data += 5;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_kill2				DS 0x800568e8
 * ======================================================================== */
ULONG Sce_espr_kill2(SCE_TASK *pSce)
{
	Esp_kill2((ULONG)pSce->Data[1]);
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_task				DS 0x80056930
 *	Kill every Esp[] entry that uses the given matrix (kind 6 = all)
 * ======================================================================== */
/* DS 0x80056930 (Sce_espr_task) */
ULONG Sce_espr_task(SCE_TASK *pSce)
{
	UCHAR		*pData;
	MATRIX		*pMat;
	ESP_WORK	*pEsp;
	int			i;

	pData = pSce->Data;
	if (pData[1] == WORK_TYPE_ALL)
	{
		Esp_kill_all();
	}
	else
	{
		pMat = Get_matrix((signed char)pData[1], (signed char)pData[2]);
		pEsp = Esp;
		for (i = 0; i < 0x60; i++, pEsp++)
		{
			if (pEsp->pMat == pMat)
			{
				pEsp->Routine1 = 0;
				pEsp->Routine0 = 0;
				pEsp->Be_flg   = 0;
			}
		}
	}
	pSce->Data += 3;
	return 1;
}

/* ==========================================================================
 *	Sce_espr_control			DS 0x800569d4
 * ======================================================================== */
ULONG Sce_espr_control(SCE_TASK *pSce)
{
	UCHAR	*pData;
	MATRIX	*pMat;

	pData = pSce->Data;
	pMat  = Get_matrix((signed char)pData[4], (signed char)pData[5]);
	Esp_ctrl(pData[1], pData[2], (USHORT)pData[3], pMat);
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Get_matrix					DS 0x80056a38
 *	Resolve a (kind, no) pair to a MATRIX.
 *	  kind 0..4 (bit 7 clear): GsIDMATRIX / pEnemy[0]->M / pEnemy[1]->M /
 *	                           pEnemy[2+no]->M / Ob_model[no].Workm
 *	  kind bit 7 set: pSin_parts_ptr[kind & 0x1f].Workm of the entity picked by
 *	                  bits 5..6 (0 = pEnemy[0], 1 = pEnemy[1], 2 = pEnemy[2+no])
 * ======================================================================== */
/* DS 0x80056a38 (Get_matrix) */
MATRIX *Get_matrix(long Work_kind, long Work_no)
{
	int			Kind;
	ENEMY_WORK	*pEm;

	Kind = (signed char)Work_kind;
	if ((Kind & 0x80) == 0)
	{
		if (Kind >= 5)
		{
			return NULL;
		}
		switch (Kind)
		{
		case WORK_TYPE_POSITION:
			return &GsIDMATRIX;
		case WORK_TYPE_PLAYER:
			return (MATRIX *)G.pEnemy[0]->M;
		case WORK_TYPE_SUB_PLAYER:
			return (MATRIX *)G.pEnemy[1]->M;
		case WORK_TYPE_ENEMY:
			return (MATRIX *)(G.pEnemy[2 + (signed char)Work_no]->M);
		case WORK_TYPE_OBJECT:
			return &G.Ob_model[(signed char)Work_no].Workm;
		}
		return NULL;
	}

	switch ((Kind & 0x60) >> 5)
	{
	case 0:
		pEm = G.pEnemy[0];
		break;
	case 1:
		pEm = G.pEnemy[1];
		break;
	case 2:
		pEm = G.pEnemy[2 + (signed char)Work_no];
		break;
	default:
		/* retail reads an undefined register here (in_a3); no entity */
		return NULL;
	}
	return (MATRIX *)&pEm->pSin_parts_ptr[Kind & 0x1f].Workm;
}

/* ==========================================================================
 *	Plc_motion					DS 0x80056b8c
 * ======================================================================== */
ULONG Plc_motion(SCE_TASK *pSce)
{
	UCHAR		*pData;
	ENEMY_WORK	*pEm;
	UINT		Rout;

	pData = pSce->Data;
	pEm   = pSce->pWork;
	pEm->Move_cnt = 0;
	pEm->Sce_free0 = 0;
	pEm->Sce_free1 = 0;
	Rout = ((UINT)pData[1] << 8) | 4;
	*(ULONG *)&pEm->Routine_0 = Rout;	/* DS stores the four Routine bytes as one word */
	pEm->Move_no   = pData[2];
	pEm->Sce_flg   = (USHORT)pData[3];
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Plc_dest					DS 0x80056bd8
 * ======================================================================== */
ULONG Plc_dest(SCE_TASK *pSce)
{
	UCHAR		*pData;
	ENEMY_WORK	*pEm;
	UINT		Id;
	UINT		Mode;
	UINT		Rout;

	pEm  = pSce->pWork;
	pData = pSce->Data;
	Id   = (UINT)pEm->Id;
	Mode = (UINT)pData[2];
	if (((pEm->Sce_flg & 4) == 0) || (pEm->Routine_1 != Mode))
	{
		Rout = (Mode << 8) | 4;
		*(ULONG *)&pEm->Routine_0 = Rout;	/* DS stores the four Routine bytes as one word */
	}
	if (Id < 0x10)
	{
		switch (Mode)
		{
		case PLC_FORWARD:
		case 0x12:
			pEm->Sce_free0 = (USHORT)Subpl_walk[Id + 0x10][1];
			break;
		case PLC_RUN:
			pEm->Sce_free0 = (USHORT)Subpl_run[Id + 0x10][1];
			break;
		case PLC_BACKWARD:
			pEm->Sce_free0 = (USHORT)Subpl_back[Id + 0x10][1];
			break;
		case PLC_BACKWARD_SLOW:
			pEm->Sce_free0 = (USHORT)Subpl_ato[Id + 0x10][1];
			break;
		case 9:
			pEm->Sce_free0 = (USHORT)Subpl_dir[Id + 0x10][1];
			break;
		}
	}
	else
	{
		switch (Mode)
		{
		case PLC_FORWARD:
		case 0x12:
			pEm->Sce_free0 = (USHORT)Subpl_walk[Id - 0x40][1];
			break;
		case PLC_RUN:
			pEm->Sce_free0 = (USHORT)Subpl_run[Id - 0x40][1];
			break;
		case PLC_BACKWARD:
			pEm->Sce_free0 = (USHORT)Subpl_back[Id - 0x40][1];
			break;
		case PLC_BACKWARD_SLOW:
			pEm->Sce_free0 = (USHORT)Subpl_ato[Id - 0x40][1];
			break;
		case 9:
			pEm->Sce_free0 = (USHORT)Subpl_dir[Id - 0x40][1];
			break;
		}
	}
	pEm->Sce_flg    = 0;
	pEm->Sce_free1  = 0;
	pEm->Set_flg_no = pData[3];
	pEm->Dest_x     = ((SCE_PLC_DEST_REC *)pData)->Dest_x;
	pEm->Dest_z     = ((SCE_PLC_DEST_REC *)pData)->Dest_z;
	pEm->Sce_free2  = pEm->Dest_x;
	pEm->Sce_free3  = pEm->Dest_z;
	pSce->Data = pData + 8;
	return 1;
}

/* ==========================================================================
 *	Plc_gun						DS 0x80056dc0
 * ======================================================================== */
ULONG Plc_gun(SCE_TASK *pSce)
{
	UCHAR		*pData;
	ENEMY_WORK	*pEm;
	UINT		Rout;

	pEm   = pSce->pWork;
	pData = pSce->Data;
	pEm->Sce_free0 = 0;
	pEm->Sce_free1 = 0;
	Rout = ((UINT)pData[1] << 8) | 4;
	*(ULONG *)&pEm->Routine_0 = Rout;	/* DS stores the four Routine bytes as one word */
	pSce->Data = pData + 2;
	return 1;
}

/* ==========================================================================
 *	Plc_gun_eff					DS 0x80056df8
 * ======================================================================== */
ULONG Plc_gun_eff(SCE_TASK *pSce)
{
	/* DS 0x80056df8 writes sp+0x00..0x0c (16-byte frame) with nine dead stores,
	   in this order, and never reads the block. The stores are kept as real
	   volatile writes so the frame and the store sequence match DS. */
	volatile ULONG	Dead[3];

	Dead[0] = 0x8c;
	Dead[1] = 0x25d;
	Dead[1] = 0x1f4;
	Dead[2] = 0;
	Dead[2] = -0x19;
	Dead[1] = 0x109;
	Dead[0] = 0x91;
	Dead[0] = 0x91;
	Dead[2] = -0x32;
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Plc_neck					DS 0x80056e5c
 * ======================================================================== */
ULONG Plc_neck(SCE_TASK *pSce)
{
	UCHAR		*pData;
	ENEMY_WORK	*pEm;
	PARTS_W		*pParts;
	UCHAR		Mode;
	short		Px;
	short		Py;
	short		Pz;

	pEm    = pSce->pWork;
	pData  = pSce->Data;
	Mode   = pData[1];
	pParts = pEm->pSin_parts_ptr;
	pEm->Neck_flg = 0x80;
	Px = ((SCE_PLC_NECK_REC *)pData)->Px;
	Py = ((SCE_PLC_NECK_REC *)pData)->Py;
	Pz = ((SCE_PLC_NECK_REC *)pData)->Pz;

	switch (Mode)
	{
	case PLC_NECK_FRONT:
		pEm->Neck_flg |= 0x12;
		break;
	case PLC_NECK_COORD:
	case PLC_NECK_TRACE:
		pEm->Neck_flg |= 4;
		break;
	case PLC_NECK_ANGLE:
		pEm->Neck_flg |= 8;
		break;
	case PLC_NECK_YES:
		pEm->Neck_flg |= 0x2a;
		break;
	case PLC_NECK_NO:
		pEm->Neck_flg |= 0x58;
		break;
	case PLC_NECK_AUTO:
		pEm->Neck_flg = 0;
		break;
	}
	pEm->Neck_point_x = Px;
	pEm->Neck_point_y = Py;
	pEm->Neck_point_z = Pz;
	if (Mode == PLC_NECK_TRACE)
	{
		/* Px is a work type (Py the enemy number); convert it to a G.pEnemy[] index */
		if (pEm->Neck_point_x == WORK_TYPE_PLAYER)
		{
			pEm->Neck_point_x = 0;
		}
		else
		{
			if (pEm->Neck_point_x == WORK_TYPE_SUB_PLAYER)
			{
				Px = 1;
			}
			else
			{
				Px = pEm->Neck_point_y + 2;
			}
			pEm->Neck_point_x = Px;
		}
		pEm->Neck_point_y = -0x8000;
	}
	/* USHORT store of the Attr field into part 8's Free[6] (short array) */
	pParts[8].Free[6] = ((SCE_PLC_NECK_REC *)pData)->Attr;
	pSce->Data = pData + 10;
	return 1;
}

/* ==========================================================================
 *	Plc_ret						DS 0x80056f9c
 * ======================================================================== */
ULONG Plc_ret(SCE_TASK *pSce)
{
	ENEMY_WORK	*pEm;

	pEm = pSce->pWork;
	pEm->Routine_0 = 1;
	pEm->Routine_1 = 0;
	pEm->Routine_2 = 0;
	pEm->Routine_3 = 0;
	if (pEm->Id < 0x10)
	{
		pEm->Type &= 0xfff;
	}
	pEm->Neck_flg = 0;
	pEm->Type &= 0xafff;
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Plc_stop					DS 0x80056ff0
 *	Push the current routine (4 bytes) into R_no_bak, or restore it when the
 *	routine is already the 0x1404 stop-marker.
 * ======================================================================== */
/* DS 0x80056ff0 (Plc_stop) */
ULONG Plc_stop(SCE_TASK *pSce)
{
	ENEMY_WORK	*pEm;
	ULONG		Rout;

	pEm  = pSce->pWork;
	/* DS reads and writes the four Routine bytes as one word (lw/sw at Routine_0) */
	Rout = *(ULONG *)&pEm->Routine_0;
	if (Rout == 0x1404)
	{
		/* restore the saved routine word */
		*(ULONG *)&pEm->Routine_0 = pSce->R_no_bak;
	}
	else
	{
		pEm = pSce->pWork;
		pSce->R_no_bak = Rout;
		*(ULONG *)&pEm->Routine_0 = 0x1404;
	}
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Plc_flg						DS 0x80057038
 *	mode 1: set Sce_flg; mode 0: OR; mode 2: XOR
 * ======================================================================== */
/* DS 0x80057038 (Plc_flg) */
ULONG Plc_flg(SCE_TASK *pSce)
{
	UCHAR		*pData;
	ENEMY_WORK	*pEm;
	USHORT		Val;

	pData = pSce->Data;
	Val   = ((SCE_PLC_FLG_REC *)pData)->Val;
	if (pData[1] == PLC_FLG_EQUAL)
	{
		pSce->pWork->Sce_flg = Val;
	}
	else if (pData[1] == PLC_FLG_OR)
	{
		pEm = pSce->pWork;
		pEm->Sce_flg |= Val;
	}
	else if (pData[1] == PLC_FLG_XOR)
	{
		pEm = pSce->pWork;
		pEm->Sce_flg ^= Val;
	}
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Plc_rot						DS 0x800570bc
 * ======================================================================== */
ULONG Plc_rot(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	if (pData[1] == PLC_ROT_SCE_FREE0)
	{
		pSce->pWork->Sce_free0 = ((SCE_PLC_ROT_REC *)pData)->Val;
	}
	else if (pData[1] == PLC_ROT_SCE_FREE1)
	{
		pSce->pWork->Sce_free1 = ((SCE_PLC_ROT_REC *)pData)->Val;
	}
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Plc_cnt						DS 0x80057120
 * ======================================================================== */
ULONG Plc_cnt(SCE_TASK *pSce)
{
	pSce->pWork->Move_cnt = pSce->Data[1];
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Sce_em_set					DS 0x8005714c
 *	Spawn an enemy unless its enemy flag is already set.
 * ======================================================================== */
/* DS 0x8005714c (Sce_em_set) */
ULONG Sce_em_set(SCE_TASK *pSce)
{
	UCHAR *pCid = &Sce.C_id;
	tagGLOBAL *pG = &G;
	ULONG	*pFlg;

	pSce_em = (SCE_EM_SET_WORK *)pSce->Data;
	if ((pG->SaveData.Stage_no > 2) || ((pG->Status_flg & STAT_SPECIAL_RUN) != 0))
	{
		pFlg = pG->SaveData.Enemy_flg2;
	}
	else
	{
		pFlg = pG->SaveData.Enemy_flg;
	}
	if ((pSce_em->Em_set_flg == 0xff) || (Flg_ck(pFlg, pSce_em->Em_set_flg) == 0))
	{
		{
			/* Spawn one enemy from the SCE_EM_SET record */
			UCHAR	*pNext;
			int		Id;
			UCHAR	Num;
			UCHAR	Ty;
			UCHAR	Se;
			ULONG	Rv;
			ENEMY_WORK	*pEw;
			ENEMY_WORK	*pEm_prev;

			Em_bin_load(pSce_em->Id);
			pEw = (ENEMY_WORK *)pG->pFree_work;
			Id  = (signed char)pSce_em->Em_no;	/* DS lb: signed (-1 selects the player slot) */
			if (Id + 2 != 1)
			{
				pG->Enemy_num++;
			}
			pG->pEnemy[Id + 2] = (ENEMY_WORK *)pG->pFree_work;
			Num = (UCHAR)(Id + 2);
			pEw->Work_no = Num;
			pNext = pG->pFree_work + sizeof(ENEMY_WORK);
			if (pG->ppEnemy_end < pG->pEnemy + Id + 3)
			{
				pG->ppEnemy_end = pG->pEnemy + Id + 3;
			}
			pG->pFree_work = pNext;
			pEw->Be_flg = 1;
			pEw->Se_type = pSce_em->Sound_flg;

			/* Se_tmp0 / Se_tmp1 bookkeeping */
			Se = pSce_em->Sound_flg;
			if (Sce.Se_tmp0 != Se)
			{
				if (Sce.Se_tmp0 != 0)
				{
					Sce.Se_tmp1 = Se;
				}
				else
				{
					Sce.Se_tmp0 = Se;
				}
			}

			pEw->Old_x = pSce_em->Pos_x;
			pEw->Pos_x = pSce_em->Pos_x;
			pEw->Old_y = pSce_em->Pos_y;
			pEw->Pos_y = pSce_em->Pos_y;
			pEw->Old_z = pSce_em->Pos_z;
			pEw->Pos_z = pSce_em->Pos_z;
			pEw->Sca_old_x = pSce_em->Pos_x;
			pEw->Cdir_x = 0;
			pEw->Cdir_z = 0;
			pEw->Sca_old_z = pSce_em->Pos_z;
			pEw->Cdir_y = pSce_em->Cdir_y;
			pEw->Id = pSce_em->Id;
			pEw->Type = pSce_em->Type;
			pEw->Em_set_flg = pSce_em->Em_set_flg;
			pEw->Model_type = pSce_em->Model_type;
			pEw->nFloor = pSce_em->nFloor;
			pEw->Ground = pSce_em->nFloor * -0x708;
			Ty = pEw->Id;

			*(ULONG *)&pEw->Routine_0 = 0;	/* DS clears the four Routine bytes with one word store */
			pEw->Damage_flg = 0;
			pEw->Damage_cnt = 0;
			pEw->Status_flg = 0;
			pEw->In_screen = 0;
			pEw->Sce_flg = 0;
			pEw->Parts0_pos_y = 0x1000;
			pEw->Sce_free0 = 0;
			pEw->Sce_free1 = 0;
			pEw->Sce_free2 = 0;
			pEw->Sce_free3 = 0;
			pEw->pTbefore_func = 0;
			pEw->pTafter_func = 0;
			pEw->pOn_om = 0;
			pEw->Kage_ofs = 0;
			pEw->Water = 0;
			pEw->L_pl = 0;
			pEw->L_spl = 0;

			pEw->Sc_id = (Ty < 0x40) ? 4 : 0x80;
			pEw->Atd[0].W = 0x1c2;
			pEw->Atd[0].D = 0x1c2;
			pEw->Atd[0].Ofs_x = 0;
			pEw->Atd[0].Ofs_z = 0;
			pEw->Atd[0].Ofs_y = (short)0xfa06;
			pEw->Atd[0].At_w = 0x1c2;
			pEw->Atd[0].At_d = 0x1c2;
			pEw->Atd[0].At_h = 0x5fa;
			pEw->nOba = 0;
			pEw->Root_ck_cnt = Num & 7;

			if (pEw->Id == (*pCid))
			{
				/* same model as the previous spawn: reuse its pointers */
				pEw->pKan_t_ptr = Sce.pC_em->pKan_t_ptr;
				pEw->pSeq_t_ptr = Sce.pC_em->pSeq_t_ptr;
				pEw->pTmd = Sce.pC_em->pTmd;
				pEw->pTmd2 = Sce.pC_em->pTmd2;
				pEw->pSub0_kan_t_ptr = Sce.pC_em->pSub0_kan_t_ptr;
				pEw->pSub0_seq_t_ptr = Sce.pC_em->pSub0_seq_t_ptr;
				pEw->pSub1_kan_t_ptr = Sce.pC_em->pSub1_kan_t_ptr;
				pEw->pSub1_seq_t_ptr = Sce.pC_em->pSub1_seq_t_ptr;
				pEw->pSa_dat = Sce.pC_em->pSa_dat;
				pEw->Tpage = Sce.pC_em->Tpage;
				pEw->Clut = Sce.pC_em->Clut;
			}
			else
			{
				(*pCid)         = pEw->Id;
				pEm_prev         = Sce.pC_em;
				Sce.C_model_type = pEw->Model_type;
				Sce.pC_em        = pEw;
				Rv = Em_kind_search(pEw->Id);
				if (Rv != Sce.C_kind)
				{
					pEw->Model_type &= 0x7f;
				}
				Sce.C_kind = (UCHAR)Rv;
				pNext = (UCHAR *)Emd_load((*pCid), pEw, (ULONG)pNext);
				Sce.Type   = 0;
				Sce.pCScd  = (UCHAR *)pG->pRoom->pScd;
				if ((pEw->Model_type & 0x80) != 0)
				{
					/* DS 0x80057514 reads the previous model (pEVar5 = Sce.pC_em before the reassignment) */
					pEw->pSub0_kan_t_ptr = pEm_prev->pSub0_kan_t_ptr;
					pEw->pSub0_seq_t_ptr = pEm_prev->pSub0_seq_t_ptr;
					pEw->pSub1_kan_t_ptr = pEm_prev->pSub1_kan_t_ptr;
					pEw->pSub1_seq_t_ptr = pEm_prev->pSub1_seq_t_ptr;
				}
			}
			Rv = PartsWork_set(pEw, (ULONG)pNext);
			pNext = (UCHAR *)PartsWork_link(pEw, Rv, pEw->pKan_t_ptr, 0);
			Sa_dat_set(pEw, pEw->pSa_dat);
			pG->pFree_work = pNext;
			if ((pG->Status_flg & STAT_MIRROR_MATRIX) != 0)
			{
				pG->pFree_work = (UCHAR *)Mirror_model_cp(pEw, (ULONG)pNext);
			}
			pEw->Move_no = 0;
			pEw->Timer0 = pSce_em->Motion;
			pEw->Timer1 = pSce_em->Ctr_flg;
			if ((pEw->Type & 0x4000) != 0)
			{
				pEw->Neck_flg = 0x92;
			}
		}
	}
	pSce->Data += 0x16;
	return 1;
}

/* ==========================================================================
 *	Sce_em_set2					DS 0x80057714
 *	Random spawn: roll record byte 0x16 as a percent; on success set the enemy's flag in
 *	ExData (rolled into Enemy_flg otherwise), then spawn as Sce_em_set.
 * ======================================================================== */
/* DS 0x80057714 (Sce_em_set2) */
ULONG Sce_em_set2(SCE_TASK *pSce)
{
	UCHAR *pCid = &Sce.C_id;
	tagGLOBAL *pG = &G;
	ULONG	*pFlg;		/* Enemy_flg / Enemy_flg2 (spawn check) */
	ULONG	*pExFlg;	/* ExData2 / ExData3 */
	ULONG	Map;
	UCHAR	Prob;
	long	Rv;

	pSce_em = (SCE_EM_SET_WORK *)pSce->Data;
	if ((pG->SaveData.Stage_no < 3) && ((pG->Status_flg & STAT_SPECIAL_RUN) == 0))
	{
		pFlg   = pG->SaveData.Enemy_flg;
		pExFlg = pG->SaveData.ExData2;
	}
	else
	{
		pFlg   = pG->SaveData.Enemy_flg2;
		pExFlg = pG->SaveData.ExData3;
	}
	Map = Stage_room_to_mapno(pG->SaveData.Stage_no, pG->SaveData.Room_no);
	if (Flg_ck(pG->SaveData.Map_flg, Map) == 0)
	{
		Prob = ((UCHAR *)pSce_em)[0x16];
		Rv   = (long)Rnd() * 100;
		if (Rv < 0)
		{
			Rv += 0xff;
		}
		/* DS reloads pSce_em and Em_set_flg in each branch */
		if ((ULONG)(Rv >> 8) < (ULONG)Prob)
		{
			Flg_on(pExFlg, pSce_em->Em_set_flg);
		}
		else
		{
			Flg_on(pFlg, pSce_em->Em_set_flg);
		}
	}
	if ((pSce_em->Em_set_flg == 0xff) || (Flg_ck(pFlg, pSce_em->Em_set_flg) == 0))
	{
		{
			/* Spawn one enemy from the SCE_EM_SET record */
			UCHAR	*pNext;
			int		Id;
			UCHAR	Num;
			UCHAR	Ty;
			UCHAR	Se;
			ULONG	Rv;
			ENEMY_WORK	*pEw;
			ENEMY_WORK	*pEm_prev;

			Em_bin_load(pSce_em->Id);
			pEw = (ENEMY_WORK *)pG->pFree_work;
			Id  = (signed char)pSce_em->Em_no;	/* DS lb: signed (-1 selects the player slot) */
			if (Id + 2 != 1)
			{
				pG->Enemy_num++;
			}
			pG->pEnemy[Id + 2] = (ENEMY_WORK *)pG->pFree_work;
			Num = (UCHAR)(Id + 2);
			pEw->Work_no = Num;
			pNext = pG->pFree_work + sizeof(ENEMY_WORK);
			if (pG->ppEnemy_end < pG->pEnemy + Id + 3)
			{
				pG->ppEnemy_end = pG->pEnemy + Id + 3;
			}
			pG->pFree_work = pNext;
			pEw->Be_flg = 1;
			pEw->Se_type = pSce_em->Sound_flg;

			/* Se_tmp0 / Se_tmp1 bookkeeping */
			Se = pSce_em->Sound_flg;
			if (Sce.Se_tmp0 != Se)
			{
				if (Sce.Se_tmp0 != 0)
				{
					Sce.Se_tmp1 = Se;
				}
				else
				{
					Sce.Se_tmp0 = Se;
				}
			}

			pEw->Old_x = pSce_em->Pos_x;
			pEw->Pos_x = pSce_em->Pos_x;
			pEw->Old_y = pSce_em->Pos_y;
			pEw->Pos_y = pSce_em->Pos_y;
			pEw->Old_z = pSce_em->Pos_z;
			pEw->Pos_z = pSce_em->Pos_z;
			pEw->Sca_old_x = pSce_em->Pos_x;
			pEw->Cdir_x = 0;
			pEw->Cdir_z = 0;
			pEw->Sca_old_z = pSce_em->Pos_z;
			pEw->Cdir_y = pSce_em->Cdir_y;
			pEw->Id = pSce_em->Id;
			pEw->Type = pSce_em->Type;
			pEw->Em_set_flg = pSce_em->Em_set_flg;
			pEw->Model_type = pSce_em->Model_type;
			pEw->nFloor = pSce_em->nFloor;
			pEw->Ground = pSce_em->nFloor * -0x708;
			Ty = pEw->Id;

			*(ULONG *)&pEw->Routine_0 = 0;	/* DS clears the four Routine bytes with one word store */
			pEw->Damage_flg = 0;
			pEw->Damage_cnt = 0;
			pEw->Status_flg = 0;
			pEw->In_screen = 0;
			pEw->Sce_flg = 0;
			pEw->Parts0_pos_y = 0x1000;
			pEw->Sce_free0 = 0;
			pEw->Sce_free1 = 0;
			pEw->Sce_free2 = 0;
			pEw->Sce_free3 = 0;
			pEw->pTbefore_func = 0;
			pEw->pTafter_func = 0;
			pEw->pOn_om = 0;
			pEw->Kage_ofs = 0;
			pEw->Water = 0;
			pEw->L_pl = 0;
			pEw->L_spl = 0;

			pEw->Sc_id = (Ty < 0x40) ? 4 : 0x80;
			pEw->Atd[0].W = 0x1c2;
			pEw->Atd[0].D = 0x1c2;
			pEw->Atd[0].Ofs_x = 0;
			pEw->Atd[0].Ofs_z = 0;
			pEw->Atd[0].Ofs_y = (short)0xfa06;
			pEw->Atd[0].At_w = 0x1c2;
			pEw->Atd[0].At_d = 0x1c2;
			pEw->Atd[0].At_h = 0x5fa;
			pEw->nOba = 0;
			pEw->Root_ck_cnt = Num & 7;

			if (pEw->Id == (*pCid))
			{
				/* same model as the previous spawn: reuse its pointers */
				pEw->pKan_t_ptr = Sce.pC_em->pKan_t_ptr;
				pEw->pSeq_t_ptr = Sce.pC_em->pSeq_t_ptr;
				pEw->pTmd = Sce.pC_em->pTmd;
				pEw->pTmd2 = Sce.pC_em->pTmd2;
				pEw->pSub0_kan_t_ptr = Sce.pC_em->pSub0_kan_t_ptr;
				pEw->pSub0_seq_t_ptr = Sce.pC_em->pSub0_seq_t_ptr;
				pEw->pSub1_kan_t_ptr = Sce.pC_em->pSub1_kan_t_ptr;
				pEw->pSub1_seq_t_ptr = Sce.pC_em->pSub1_seq_t_ptr;
				pEw->pSa_dat = Sce.pC_em->pSa_dat;
				pEw->Tpage = Sce.pC_em->Tpage;
				pEw->Clut = Sce.pC_em->Clut;
			}
			else
			{
				(*pCid)         = pEw->Id;
				pEm_prev         = Sce.pC_em;
				Sce.C_model_type = pEw->Model_type;
				Sce.pC_em        = pEw;
				Rv = Em_kind_search(pEw->Id);
				if (Rv != Sce.C_kind)
				{
					pEw->Model_type &= 0x7f;
				}
				Sce.C_kind = (UCHAR)Rv;
				pNext = (UCHAR *)Emd_load((*pCid), pEw, (ULONG)pNext);
				Sce.Type   = 0;
				Sce.pCScd  = (UCHAR *)pG->pRoom->pScd;
				if ((pEw->Model_type & 0x80) != 0)
				{
					/* DS 0x80057514 reads the previous model (pEVar5 = Sce.pC_em before the reassignment) */
					pEw->pSub0_kan_t_ptr = pEm_prev->pSub0_kan_t_ptr;
					pEw->pSub0_seq_t_ptr = pEm_prev->pSub0_seq_t_ptr;
					pEw->pSub1_kan_t_ptr = pEm_prev->pSub1_kan_t_ptr;
					pEw->pSub1_seq_t_ptr = pEm_prev->pSub1_seq_t_ptr;
				}
			}
			Rv = PartsWork_set(pEw, (ULONG)pNext);
			pNext = (UCHAR *)PartsWork_link(pEw, Rv, pEw->pKan_t_ptr, 0);
			Sa_dat_set(pEw, pEw->pSa_dat);
			pG->pFree_work = pNext;
			if ((pG->Status_flg & STAT_MIRROR_MATRIX) != 0)
			{
				pG->pFree_work = (UCHAR *)Mirror_model_cp(pEw, (ULONG)pNext);
			}
			pEw->Move_no = 0;
			pEw->Timer0 = pSce_em->Motion;
			pEw->Timer1 = pSce_em->Ctr_flg;
			if ((pEw->Type & 0x4000) != 0)
			{
				pEw->Neck_flg = 0x92;
			}
		}
	}
	pSce->Data += 0x18;
	return 1;
}

/* ==========================================================================
 *	Sce_key_ck					DS 0x80057da8
 * ======================================================================== */
ULONG Sce_key_ck(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	Res;
	USHORT	Mask;

	pData = pSce->Data;
	Res   = pData[1];
	Mask  = ((SCE_KEY_REC *)pData)->Mask;
	pSce->Data = pData + 4;
	if ((G.Key & Mask) == 0)
	{
		Res ^= 1;
	}
	return Res;
}

/* ==========================================================================
 *	Sce_trg_ck					DS 0x80057de4
 * ======================================================================== */
ULONG Sce_trg_ck(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	Res;
	USHORT	Mask;

	pData = pSce->Data;
	Res   = pData[1];
	Mask  = ((SCE_KEY_REC *)pData)->Mask;
	pSce->Data = pData + 4;
	if ((G.Key_trg & Mask) == 0)
	{
		Res ^= 1;
	}
	return Res;
}

/* ==========================================================================
 *	Sce_bgm_control				DS 0x80057e20
 * ======================================================================== */
ULONG Sce_bgm_control(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	Snd_bgm_ctr(((UINT)pData[1] << 28) | ((UINT)pData[2] << 24) |
				((UINT)pData[3] << 16) | ((UINT)pData[4] << 8) | (UINT)pData[5]);
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Sce_bgmtbl_set				DS 0x80057e98
 * ======================================================================== */
ULONG Sce_bgmtbl_set(SCE_TASK *pSce)
{
	SCE_BGMTBL_REC *pData;

	pData = (SCE_BGMTBL_REC *)pSce->Data;
	Snd_bgm_tbl_set(((UINT)pData->Unk_02 << 16) |
					(UINT)pData->Unk_04 | (UINT)pData->Unk_06);
	pSce->Data += 8;
	return 1;
}

/* ==========================================================================
 *	Sce_fade_set				DS 0x80057ef0
 * ======================================================================== */
ULONG Sce_fade_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	USHORT	Idx;
	short	Add;
	USHORT	Add_u;
	short	*pFade;
	RECT	Rc;

	pData = pSce->Data;
	Idx   = (USHORT)(pData[1] | (pData[2] << 8));
	Add   = ((SCE_FADE_REC *)pData)->Add;
	Add_u = (USHORT)Add;
	Rc.w  = 0x140;
	Rc.x  = 0;
	Rc.y  = 0;
	Rc.h  = 0xf0;
	Fade_set((USHORT)(pData[1] | (pData[2] << 8)), Add, pData[3], 7);
	Fade_adjust(pData[1], 0, 0, &Rc);
	pFade = &Main.Fade[(UINT)(UCHAR)Idx].Kido;
	if (Add != 0)
	{
		if (Add < 1)
		{
			*pFade = (short)(Add_u - 0x8000);
		}
		else
		{
			*pFade = 0;
		}
	}
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Sce_fade_adjust				DS 0x80057fd8
 * ======================================================================== */
ULONG Sce_fade_adjust(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	Fade_adjust(pData[1], ((SCE_FADE_ADJ_REC *)pData)->Kido, 0, NULL);
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Xa_on						DS 0x80058028
 * ======================================================================== */
ULONG Xa_on(SCE_TASK *pSce)
{
	Xa_play(pSce->Data[1], ((SCE_XA_REC *)pSce->Data)->no);
	G.Status_flg |= STAT_XA_VOICE;
	pSce->Data += 4;
	return 1;
}

/* ==========================================================================
 *	Xa_vol						DS 0x80058090
 * ======================================================================== */
ULONG Xa_vol(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	if (pData[1] < 0x80)
	{
		CD.Vol = pData[1];
	}
	pSce->Data = pData + 2;
	return 1;
}

/* ==========================================================================
 *	Se_vol						DS 0x800580cc
 *	Toggle a temporary SE volume scale (Sce.Se_vol holds the saved value).
 * ======================================================================== */
/* DS 0x800580cc (Se_vol) */
ULONG Se_vol(SCE_TASK *pSce)
{
	UCHAR	*pData;
	ULONG	Pad[2];

	(void)Pad;
	pData = pSce->Data;
	if (Sce.Se_vol == 0xff)
	{
		Sce.Se_vol = Volume_Se;
		Volume_Se  = (UCHAR)(((UINT)Volume_Se * (UINT)pData[1]) / 100);
	}
	else
	{
		Volume_Se  = Sce.Se_vol;
		Sce.Se_vol = 0xff;
	}
	pSce->Data = pData + 2;
	return 1;
}

/* ==========================================================================
 *	Sce_shake_on				DS 0x80058164
 *	Screen-shake: 240 line_trans calls with the given amplitude/offset
 * ======================================================================== */
/* DS 0x80058164 (Sce_shake_on) */
ULONG Sce_shake_on(SCE_TASK *pSce)
{
	signed char	Amp;	/* DS lb (signed) at +1 and +2 */
	signed char	Dir;
	int		Line_no;
	short	Ofs;

	Amp = pSce->Data[1];
	Dir = pSce->Data[2];
	Ofs   = (Main.pDb)->Draw.ofs[1];
	line_mode_set(0, 7);
	for (Line_no = 0; Line_no != 0xf0; Line_no++)
	{
		line_trans(Line_no, -(int)Amp, (int)Dir, (int)Ofs);
	}
	line_scr_end();
	pSce->Data += 3;
	return 1;
}

/* ==========================================================================
 *	Sce_line_start				DS 0x80058214
 * ======================================================================== */
ULONG Sce_line_start(SCE_TASK *pSce)
{
	line_mode_set((UINT)pSce->Data[1], (UINT)((SCE_LINE_START_REC *)pSce->Data)->Prio);
	pSce->Data += 4;
	return 1;
}

/* ==========================================================================
 *	Sce_line_main				DS 0x80058260
 * ======================================================================== */
ULONG Sce_line_main(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	line_trans((UINT)pData[1], (int)((SCE_LINE_MAIN_REC *)pData)->Slide_ofs, (int)((SCE_LINE_MAIN_REC *)pData)->Copy_ofs,
			   (int)(Main.pDb)->Draw.ofs[1]);
	pSce->Data += 6;
	return 1;
}

/* ==========================================================================
 *	Sce_line_end				DS 0x800582b8
 * ======================================================================== */
ULONG Sce_line_end(SCE_TASK *pSce)
{
	line_scr_end();
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Mizu_div_set				DS 0x800582f0
 *	Allocate Max blocks of 26 division polygons (13 rows x 2) in pFree_work.
 *	With Max == 0 retail returns the unadvanced Data pointer and does not
 *	advance the script (kept as-is).
 * ======================================================================== */
/* DS 0x800582f0 (Mizu_div_set) */
ULONG Mizu_div_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	*pPrim0;
	const UCHAR	*pCoord;
	UINT	Div_num;
	int		i;
	int		j;

	pData = pSce->Data;
	Sce.Mizu_div_max = pData[1];
	Div_num = Sce.Mizu_div_max;
	if (Div_num == 0)
	{
		return (ULONG)pData;
	}
	Sce.pMizu_div = (POLY_GT4 *)G.pFree_work;
	pPrim0 = G.pFree_work;
	G.pFree_work = G.pFree_work + Div_num * 0x548;
	do
	{
		for (i = 0; i < 13; i++)
		{
			pCoord = Div_prim_coord[i];
			for (j = 0; j < 2; j++)
			{
				/* Fill one GT4 division primitive (0x34 bytes) from an 8-byte coord row.
				 * Retail: header 00 00 00 0c; rgb 80 80 80 with code 3e in the first
				 * vertex and 00 in the rest (asm: lui t3,0x80 gives word 0x00808080);
				 * u = x*0x7f/0x12; v = y*7. */
				/* DS writes the tag word and the colour words as whole words */
				((POLY_GT4 *)pPrim0)->tag = 0x0c000000;
				*(ULONG *)&((POLY_GT4 *)pPrim0)->r0 = 0x3e808080;
				((POLY_GT4 *)pPrim0)->u0 = (UCHAR)(((int)pCoord[0] * 0x7f) / 0x12);
				((POLY_GT4 *)pPrim0)->v0 = (UCHAR)(pCoord[1] * 7);
				*(ULONG *)&((POLY_GT4 *)pPrim0)->r1 = 0x00808080;
				((POLY_GT4 *)pPrim0)->u1 = (UCHAR)(((int)pCoord[2] * 0x7f) / 0x12);
				((POLY_GT4 *)pPrim0)->v1 = (UCHAR)(pCoord[3] * 7);
				*(ULONG *)&((POLY_GT4 *)pPrim0)->r2 = 0x00808080;
				((POLY_GT4 *)pPrim0)->u2 = (UCHAR)(((int)pCoord[4] * 0x7f) / 0x12);
				((POLY_GT4 *)pPrim0)->v2 = (UCHAR)(pCoord[5] * 7);
				*(ULONG *)&((POLY_GT4 *)pPrim0)->r3 = 0x00808080;
				((POLY_GT4 *)pPrim0)->u3 = (UCHAR)(((int)pCoord[6] * 0x7f) / 0x12);
				((POLY_GT4 *)pPrim0)->v3 = (UCHAR)(pCoord[7] * 7);
				pPrim0 = pPrim0 + sizeof(POLY_GT4);
			}
		}
		Div_num--;
	} while (Div_num != 0);
	pSce->Data = pData + 2;
	return 1;
}

/* ==========================================================================
 *	Keep_Item_ck				DS 0x800584f0
 *	1 when the item is held, 0 otherwise
 * ======================================================================== */
/* DS 0x800584f0 (Keep_Item_ck) */
ULONG Keep_Item_ck(SCE_TASK *pSce)
{
	UCHAR	*pData;
	long	Idx;

	pData = pSce->Data;
	pSce->Data = pData + 2;
	Idx = Search_item_id((UINT)pData[1]);
	return (Idx >= 0) ? 1 : 0;
}

/* ==========================================================================
 *	Sce_Item_cmp				DS 0x80058528
 *	Compare the count of the equipped item with a value
 * ======================================================================== */
/* DS 0x80058528 (Sce_Item_cmp) */
ULONG Sce_Item_cmp(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	Num;
	UCHAR	Val;
	ULONG	Res;

	pData = pSce->Data;
	Num   = 0;
	if ((St.Equip_item_no & 0x80) == 0)
	{
		Num = G.SaveData.Item_work[St.Equip_item_no].Num;
	}
	pSce->Data = pData + 3;
	Val = pData[2];
	switch (pData[1])
	{
	case CMP_EQ:
		Res = (Num == Val);
		break;
	case CMP_GT:
		Res = (Val < Num);
		break;
	case CMP_GE:
		Res = (Num < Val) ^ 1;
		break;
	case CMP_LT:
		Res = (Num < Val);
		break;
	case CMP_LE:
		Res = (Val < Num) ^ 1;
		break;
	case CMP_NE:
		Res = (Num != Val);
		break;
	case CMP_AND:
		Res = ((Num & Val) != 0);
		break;
	default:
		Res = 1;
		break;
	}
	return Res;
}

/* ==========================================================================
 *	Sce_Item_lost				DS 0x800585e4
 * ======================================================================== */
ULONG Sce_Item_lost(SCE_TASK *pSce)
{
	long	Idx;

	Idx = Search_item_id((UINT)pSce->Data[1]);
	if (Idx >= 0)
	{
		G.SaveData.Item_work[Idx].Id   = 0;
		G.SaveData.Item_work[Idx].Num  = 0;
		G.SaveData.Item_work[Idx].Size = 0;
	}
	Sort_item();
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Sce_Item_lost2				DS 0x80058660
 *	Remove a count of an item; clear the slot when it reaches 0
 * ======================================================================== */
/* DS 0x80058660 (Sce_Item_lost2) */
ULONG Sce_Item_lost2(SCE_TASK *pSce)
{
	UCHAR	*pData;
	long	Idx;
	UCHAR	Num;

	pData = pSce->Data;
	pSce->Data = pData + 3;
	Idx = Search_item_id((UINT)pData[1]);
	if (Idx >= 0)
	{
		Num = G.SaveData.Item_work[Idx].Num - pData[2];
		G.SaveData.Item_work[Idx].Num = Num;
		if (Num == 0)
		{
			G.SaveData.Item_work[Idx].Id   = 0;
			G.SaveData.Item_work[Idx].Num  = 0;
			G.SaveData.Item_work[Idx].Size = 0;
		}
		Sort_item();
	}
	return 1;
}

/* ==========================================================================
 *	Weapon_chg					DS 0x80058704
 *	Equip a weapon item (0 = unequip)
 * ======================================================================== */
/* DS 0x80058704 (Weapon_chg) */
ULONG Weapon_chg(SCE_TASK *pSce)
{
	UCHAR	Id;
	long	Idx;
	ENEMY_WORK *pPl = (ENEMY_WORK *)&G.Player;

	Id = pSce->Data[1];
	G.Player.Type = (G.Player.Type & 0xf000) | (USHORT)Id;
	if (Id == 0)
	{
		St.Equip_item_no = 0x80;
		St.Equip_item_id = 0;
	}
	else
	{
		Idx = Search_item_id((UINT)Id);
		St.Equip_item_no = (UCHAR)Idx;
		St.Equip_item_id = Id;
	}
	Weapon_cls(pPl);
	Pl_weapon_ch(pPl);
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Sce_Item_get				DS 0x800587b8
 *	Load an item TIM from file 0xaa and register it
 * ======================================================================== */
/* DS 0x800587b8 (Sce_Item_get) */
ULONG Sce_Item_get(SCE_TASK *pSce)
{
	UINT	Id;
	UCHAR	*pWork;

	pWork = G.pFree_work;
	Id = (UINT)pSce->Data[1];
	CD.Fno    = ITPS_ITP;
	CD.Fsize  = 0x3000;
	CD.Fsector = (UINT)floc[ITPS_ITP].sec_high * 0x10000 + (UINT)floc[ITPS_ITP].sector + Id * 6;
	CD.Fsum    = Itp_check_sum[Id];
	Cd_read(ITPS_ITP, (ULONG *)pWork, 2, Itp_mess);
	Set_item(Id, (UINT)pSce->Data[2], (ULONG *)((SCE_ITP_BUF *)pWork)->Item_img);
	pSce->Data += 3;
	return 1;
}

/* ==========================================================================
 *	Kage_set					DS 0x80058898
 *	Set colour and four UV-ish shorts of an enemy's shadow work
 * ======================================================================== */
/* DS 0x80058898 (Kage_set) */
ULONG Kage_set(SCE_TASK *pSce)
{
	SCE_KAGE_REC *pData;
	ENEMY_WORK	*pEm;
	USHORT		*pKage;

	pData = (SCE_KAGE_REC *)pSce->Data;
	pKage = NULL;
	switch (pData->Unk_01)
	{
	case WORK_TYPE_PLAYER:
		pKage = (USHORT *)G.pEnemy[0]->pKage_work;
		break;
	case WORK_TYPE_SUB_PLAYER:
		pKage = (USHORT *)G.pEnemy[1]->pKage_work;
		break;
	case WORK_TYPE_ENEMY:
		pEm   = G.pEnemy[(signed char)pData->Unk_02 + 2];	/* DS lb: signed index */
		pKage = (USHORT *)pEm->pKage_work;
		break;
	default:
		/* DS leaves the shadow pointer (s0) unassigned for kinds 0 and >3 and goes on to use it */
		break;
	}
	/* DS reloads pSce->Data before every field (stores through pKage may alias it) */
	Kage_work_color_set((ULONG *)pKage, ((UINT)((SCE_KAGE_REC *)pSce->Data)->Unk_03) | ((UINT)((SCE_KAGE_REC *)pSce->Data)->Unk_04 << 8) | ((UINT)((SCE_KAGE_REC *)pSce->Data)->Unk_05 << 16));
	pKage[2] = ((SCE_KAGE_REC *)pSce->Data)->Unk_06;
	pKage[3] = ((SCE_KAGE_REC *)pSce->Data)->Unk_08;
	pKage[0] = ((SCE_KAGE_REC *)pSce->Data)->Unk_0a;
	pKage[1] = ((SCE_KAGE_REC *)pSce->Data)->Unk_0c;
	pSce->Data = pSce->Data + sizeof(SCE_KAGE_REC);
	return 1;
}

/* ==========================================================================
 *	Light_pos_set				DS 0x800589d4
 * ======================================================================== */
ULONG Light_pos_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	Axis;
	short	(*pPos_work)[3];

	pData = pSce->Data;
	pPos_work = (G.pRoom)->pLight[G.SaveData.Cut_no].Pos + pData[2];
	Axis  = pData[3];
	if (Axis == MEMBER_POS_Y)
	{
		(*pPos_work)[1] = ((SCE_LIGHT_POS_REC *)pData)->Val;
	}
	else if (Axis < MEMBER_POS_Z)
	{
		if (Axis == MEMBER_POS_X)
		{
			(*pPos_work)[0] = ((SCE_LIGHT_POS_REC *)pData)->Val;
		}
	}
	else if (Axis == MEMBER_POS_Z)
	{
		(*pPos_work)[2] = ((SCE_LIGHT_POS_REC *)pData)->Val;
	}
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Light_kido_set				DS 0x80058a8c
 * ======================================================================== */
ULONG Light_kido_set(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	(G.pRoom)->pLight[G.SaveData.Cut_no].L[pData[1]] = ((SCE_LIGHT_KIDO_REC *)pData)->Val;
	pSce->Data = pData + 4;
	return 1;
}

/* ==========================================================================
 *	Light_color_set				DS 0x80058ad8
 * ======================================================================== */
ULONG Light_color_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	(*pCol)[3];

	pData = pSce->Data;
	pCol  = (G.pRoom)->pLight[G.SaveData.Cut_no].Col + pData[1];
	(*pCol)[0] = pData[2];
	(*pCol)[1] = pData[3];
	(*pCol)[2] = pData[4];
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Light_pos_set2				DS 0x80058b40
 * ======================================================================== */
ULONG Light_pos_set2(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	Axis;
	short	(*pPos_work)[3];

	pData = pSce->Data;
	pPos_work = (G.pRoom)->pLight[pData[1]].Pos + pData[2];
	Axis  = pData[3];
	if (Axis == MEMBER_POS_Y)
	{
		(*pPos_work)[1] = ((SCE_LIGHT_POS_REC *)pData)->Val;
	}
	else if (Axis < MEMBER_POS_Z)
	{
		if (Axis == MEMBER_POS_X)
		{
			(*pPos_work)[0] = ((SCE_LIGHT_POS_REC *)pData)->Val;
		}
	}
	else if (Axis == MEMBER_POS_Z)
	{
		(*pPos_work)[2] = ((SCE_LIGHT_POS_REC *)pData)->Val;
	}
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Light_kido_set2				DS 0x80058bfc
 * ======================================================================== */
ULONG Light_kido_set2(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	(G.pRoom)->pLight[pData[2]].L[pData[3]] = ((SCE_LIGHT_KIDO2_REC *)pData)->Val;
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Light_color_set2			DS 0x80058c4c
 * ======================================================================== */
ULONG Light_color_set2(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UCHAR	(*pCol)[3];

	pData = pSce->Data;
	pCol  = (G.pRoom)->pLight[pData[1]].Col + pData[2];
	(*pCol)[0] = pData[3];
	(*pCol)[1] = pData[4];
	(*pCol)[2] = pData[5];
	pSce->Data = pData + 6;
	return 1;
}

/* ==========================================================================
 *	Sce_scr_move				DS 0x80058cb8
 * ======================================================================== */
ULONG Sce_scr_move(SCE_TASK *pSce)
{
	G.Scrl_y = ((SCE_SCR_MOVE_REC *)pSce->Data)->Scrl_y;
	pSce->Data += 4;
	return 1;
}

/* ==========================================================================
 *	Flr_set						DS 0x80058ce4
 *	Set / clear bit 0x80 of a room floor entry's flag (floor id at +6)
 * ======================================================================== */
/* DS 0x80058ce4 (Flr_set) */
ULONG Flr_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UINT	No;
	USHORT	*pFlr;

	pData = pSce->Data;
	No    = (UINT)pData[1];
	if (No < (USHORT)*(G.pRoom)->pFloor)
	{
		pFlr = (USHORT *)((G.pRoom)->pFloor + No * 6 + 6);
		if (pData[2] == 1)
		{
			*pFlr = *pFlr & 0xff7f;
		}
		else
		{
			*pFlr = *pFlr | 0x80;
		}
	}
	pSce->Data = pData + 3;
	return 1;
}

/* ==========================================================================
 *	Movie_on					DS 0x80058d64
 * ======================================================================== */
ULONG Movie_on(SCE_TASK *pSce)
{
	Init_movie_work((UINT)pSce->Data[1]);
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Splc_ret					DS 0x80058dac
 *	Splice (partner) return: reset routine, take the saved life
 * ======================================================================== */
/* DS 0x80058dac (Splc_ret) */
ULONG Splc_ret(SCE_TASK *pSce)
{
	ENEMY_WORK	*pEw;

	pEw = pSce->pWork;
	pEw->Routine_0 = 1;
	pEw->Routine_1 = 0;
	pEw->Routine_2 = 0;
	pEw->Routine_3 = 0;
	pEw->pEnemy_neck = &G.Player.Be_flg;
	pEw->Neck_flg = 0;
	pEw->Type = (pEw->Type & 0xafff) | 0x400;
	G.Status_flg |= STAT_SUB_PLAYER;
	pEw->Max_life = 200;
	pEw->Life = G.SaveData.Dummy[0];
	if ((pEw->Id == 0x45) || (pEw->Id == 0x4f))
	{
		*(USHORT *)&pEw->Free[2] = (USHORT)pEw->Free[2] & 0xdfff;
	}
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Splc_sce					DS 0x80058e48
 * ======================================================================== */
ULONG Splc_sce(SCE_TASK *pSce)
{
	ENEMY_WORK	*pEw;

	pEw = pSce->pWork;
	pEw->Routine_0 = 4;
	pEw->Routine_1 = 6;
	pEw->Routine_2 = 0;
	pEw->Routine_3 = 0;
	pEw->Neck_flg = 0x92;
	pEw->pEnemy_neck = &pEw->Be_flg;
	pEw->Type = (pEw->Type & 0xfbff) | 0x5000;
	G.Status_flg &= ~STAT_SUB_PLAYER;
	pEw->Life = -1;
	pEw->Max_life = -1;
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Evt_next2					DS 0x80058eb0
 * ======================================================================== */
ULONG Evt_next2(SCE_TASK *pSce)
{
	G.SaveData.Dummy1[1] = 0;
	G.SaveData.Dummy[0] = 200;
	G.pEnemy[1]->Life = 200;
	G.pEnemy[1]->Max_life = 200;
	pSce->Data++;
	return 1;
}

/* ==========================================================================
 *	Mirror_set					DS 0x80058ef8
 * ======================================================================== */
ULONG Mirror_set(SCE_TASK *pSce)
{
	G.Status_flg |= STAT_MIRROR_MATRIX;
	G.Mirror_flg = pSce->Data[1];
	G.Mirror_pos = ((SCE_MIRROR_REC *)pSce->Data)->Mirror_pos;
	G.Mirror_min = ((SCE_MIRROR_REC *)pSce->Data)->Mirror_min;
	G.Mirror_max = ((SCE_MIRROR_REC *)pSce->Data)->Mirror_max;
	pSce->Data += 8;
	return 1;
}

/* ==========================================================================
 *	Sce_parts_bomb				DS 0x80058f78
 * ======================================================================== */
ULONG Sce_parts_bomb(SCE_TASK *pSce)
{
	SCE_PARTS_BOMB_REC *pData;
	PARTS_W	*pP_ptr;

	pData  = (SCE_PARTS_BOMB_REC *)pSce->Data;
	pP_ptr = pSce->pWork->pSin_parts_ptr + pData->Unk_02;
	pP_ptr->Be_flg |= 0x4a;
	pP_ptr->Free[4] = 0;
	pP_ptr->Poly_rgb = (UINT)pData->Unk_03 | ((UINT)pData->Unk_04 << 8) | ((UINT)pData->Unk_05 << 16);
	pP_ptr->Free[0] = pData->Unk_06;
	pP_ptr->Free[2] = pData->Unk_08;
	pP_ptr->Free[6] = pData->Unk_0a;
	pP_ptr->Free[1] = pData->Unk_0c;
	pP_ptr->Free[3] = pData->Unk_0e;
	pSce->Data = (UCHAR *)pData + sizeof(SCE_PARTS_BOMB_REC);
	return 1;
}

/* ==========================================================================
 *	Sce_parts_down				DS 0x80059030
 * ======================================================================== */
ULONG Sce_parts_down(SCE_TASK *pSce)
{
	UCHAR	*pData;
	PARTS_W	*pP_ptr;

	pData  = pSce->Data;
	pP_ptr = pSce->pWork->pSin_parts_ptr + pData[1];
	pP_ptr->Timer3 = 0;
	pP_ptr->Be_flg |= 0x1062;
	pP_ptr->Old_x  = ((SCE_PARTS_DOWN_REC *)pData)->Old_x;
	pP_ptr->Old_z  = ((SCE_PARTS_DOWN_REC *)pData)->Old_z;
	pP_ptr->Old_y  = ((SCE_PARTS_DOWN_REC *)pData)->Old_y;
	pP_ptr->Timer1 = (char)((SCE_PARTS_DOWN_REC *)pData)->Timer1;
	pP_ptr->Old_x2 = ((SCE_PARTS_DOWN_REC *)pData)->Old_x2;
	pP_ptr->Old_y2 = ((SCE_PARTS_DOWN_REC *)pData)->Old_y2;
	pP_ptr->Old_z2 = ((SCE_PARTS_DOWN_REC *)pData)->Old_z2;
	pSce->Data = pData + 0x10;
	return 1;
}

/* ==========================================================================
 *	Col_chg_set					DS 0x800590e0
 *	Update the collision-change parameters of the matching Cc_work entry
 * ======================================================================== */
/* DS 0x800590e0 (Col_chg_set) */
ULONG Col_chg_set(SCE_TASK *pSce)
{
	UCHAR	*pData;
	UINT	Idx;
	ULONG	Pad[2];

	(void)Pad;
	Idx = 0;
	if (Cc_work.Ccol_no != 0)
	{
		do
		{
			if ((ENEMY_WORK *)Cc_work.Cc_parts[Idx].pOm == pSce->pWork)
			{
				pData = pSce->Data;
				Cc_work.Cc_parts[Idx].Cc_ctr  = pData[1] | (Cc_work.Cc_parts[Idx].Cc_ctr & 0xc0);
				Cc_work.Cc_parts[Idx].Cc_cnt  = pData[2];
				Cc_work.Cc_parts[Idx].Cc_wait = pData[3];
				Cc_work.Cc_parts[Idx].Cc_num  = pData[4];
				break;
			}
			Idx++;
		} while (Idx < Cc_work.Ccol_no);
	}
	pSce->Data += 5;
	return 1;
}

/* ==========================================================================
 *	Plc_heal					DS 0x8005918c
 * ======================================================================== */
ULONG Plc_heal(SCE_TASK *pSce)
{
	pSce->Data++;
	G.Pl_poison_timer = 0;
	G.Pl_poison_down  = 0;
	G.Player.Life     = G.Player.Max_life;
	return 1;
}

/* ==========================================================================
 *	St_map_hint					DS 0x800591c4
 * ======================================================================== */
ULONG St_map_hint(SCE_TASK *pSce)
{
	St.St_type = 4;
	Game.Doordemo_flg = 1;
	G.Status_flg |= STAT_STATUS_SCREEN;
	Game.Stop_bak = G.Stop_flg;
	/* low byte of St.dm00 (retail sb at St + 0xE02, which is dm00) */
	*(UCHAR *)&St.dm00 = pSce->Data[1];
	pSce->Data += 2;
	return 1;
}

/* ==========================================================================
 *	Sce_em_pos_ck				DS 0x8005922c
 *	Line check between two enemies' positions; 1 when clear
 * ======================================================================== */
/* DS 0x8005922c (Sce_em_pos_ck) */
ULONG Sce_em_pos_ck(SCE_TASK *pSce)
{
	UCHAR	*pData;
	VECTOR	Vec1;
	VECTOR	Vec2;
	ULONG	Res;

	pData = pSce->Data;
	Vec1.vx = G.pEnemy[pData[1]]->Pos_x;
	Vec1.vy = G.pEnemy[pData[1]]->Pos_y;
	Vec1.vz = G.pEnemy[pData[1]]->Pos_z;
	Vec2.vx = G.pEnemy[pData[2]]->Pos_x;
	Vec2.vy = G.pEnemy[pData[2]]->Pos_y;
	Vec2.vz = G.pEnemy[pData[2]]->Pos_z;
	pSce->Data = pData + 6;
	Res = Sca_ck_line(&Vec1, &Vec2, ((SCE_EM_POS_REC *)pData)->Att, pData[3]);
	return (Res == 0);
}

/* ==========================================================================
 *	Poison_ck					DS 0x800592f8
 * ======================================================================== */
ULONG Poison_ck(SCE_TASK *pSce)
{
	pSce->Data++;
	return (G.Pl_poison_timer != 0);
}

/* ==========================================================================
 *	Poison_clr					DS 0x80059318
 * ======================================================================== */
ULONG Poison_clr(SCE_TASK *pSce)
{
	G.Player.Routine_0 = 1;
	G.Player.Routine_1 = 0;
	G.Player.Routine_2 = 0;
	G.Player.Routine_3 = 0;
	pSce->Data++;
	G.Pl_poison_timer = 0;
	G.Pl_poison_down  = 0;
	return 1;
}

/* ==========================================================================
 *	Vib_set0					DS 0x80059348
 *	Small-motor rumble request (Time, Attr)
 * ======================================================================== */
/* DS 0x80059348 (Vib_set0) */
ULONG Vib_set0(SCE_TASK *pSce)
{
	Pad_vib_set0(((SCE_VIB_REC *)pSce->Data)->Time, ((SCE_VIB_REC *)pSce->Data)->Attr);
	pSce->Data += 6;
	return 1;
}

/* ==========================================================================
 *	Vib_set1					DS 0x80059394
 *	Large-motor rumble request (Time, Power, Attr)
 * ======================================================================== */
/* DS 0x80059394 (Vib_set1) */
ULONG Vib_set1(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	Pad_vib_set1(((SCE_VIB_REC *)pData)->Time, pData[1], ((SCE_VIB_REC *)pData)->Attr);
	pSce->Data += 6;
	return 1;
}

/* ==========================================================================
 *	Vib_fade_set				DS 0x800593e4
 *	Large-motor ramp request (Time, Pow_st, Pow_ed, Attr)
 * ======================================================================== */
/* DS 0x800593e4 (Vib_fade_set) */
ULONG Vib_fade_set(SCE_TASK *pSce)
{
	UCHAR	*pData;

	pData = pSce->Data;
	Pad_vib_fade_set(((SCE_VIB_FADE_REC *)pData)->Time, pData[2], pData[3], ((SCE_VIB_FADE_REC *)pData)->Attr);
	pSce->Data += 8;
	return 1;
}
