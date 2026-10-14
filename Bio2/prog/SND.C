/*
 * SND.C - Sound system, BGM and sound effect module
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\SND.C
 */
#include "TYPES.H"
#include <LIBAPI.H>
#include <LIBCD.H>
#include <LIBDS.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBHMD.H>
#include <LIBSND.H>
#include <LIBSPU.H>
#include "CONFIG.H"
#include "SND.H"
#include "PAD.H"
#include "CD.H"
#include "DIESPR.H"
#include "DOOR.H"
#include "EM_SUB.H"
#include "ESP.H"
#include "ESP_R.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MD_SUB.H"
#include "MONITOR.H"
#include "OMA.H"
#include "PLAYER.H"
#include "SCA.H"
#include "SCE_AT.H"
#include "SUB.H"
#include "CONFIG.H"
#include "DIEDEMO.H"
#include "MOVIE.H"
#include "PL_WEP.H"
#include "ROOM.H"
#include "SCE2.H"
#include "FILECODE.H"
#include "flag.h"
#include "DIVCHK.H"

/* DS 0x800A80B0 (nroom): Bgm_room_tbl base index per stage */
UCHAR nroom[8] = {
	0x00, 0x1F, 0x3D, 0x4D, 0x60, 0x6C, 0x86, 0x00,
};

/* DS 0x800A80B8 (Core_edh): COREnn_EDH names (FILECODE.H), by core_type_no */
USHORT Core_edh[24] = {
	CORE00_EDH, CORE01_EDH, CORE00_EDH, CORE01_EDH,
	CORE00_EDH, CORE01_EDH, CORE00_EDH, CORE01_EDH,
	CORE00_EDH, CORE01_EDH, CORE00_EDH, CORE0B_EDH,
	CORE0C_EDH, CORE0D_EDH, CORE0E_EDH, CORE0F_EDH,
	CORE10_EDH, CORE11_EDH, CORE12_EDH, CORE13_EDH,
	CORE14_EDH, CORE15_EDH, CORE00_EDH, CORE00_EDH,
};

/* DS 0x800A80E8 (Core_vbd): COREnn_VB names (FILECODE.H), by core_type_no */
USHORT Core_vbd[24] = {
	CORE00_VB, CORE01_VB, CORE00_VB, CORE01_VB,
	CORE00_VB, CORE01_VB, CORE00_VB, CORE01_VB,
	CORE00_VB, CORE01_VB, CORE00_VB, CORE0B_VB,
	CORE0C_VB, CORE0D_VB, CORE0E_VB, CORE0F_VB,
	CORE10_VB, CORE11_VB, CORE12_VB, CORE13_VB,
	CORE14_VB, CORE15_VB, CORE00_VB, CORE00_VB,
};

/* DS 0x800A8118 (Arms_edh): ARMSnn_EDH names (FILECODE.H), by arms_type_no */
USHORT Arms_edh[20] = {
	ARMS01_EDH, ARMS01_EDH, ARMS02_EDH, ARMS03_EDH,
	ARMS04_EDH, ARMS05_EDH, ARMS06_EDH, ARMS07_EDH,
	ARMS08_EDH, ARMS09_EDH, ARMS0A_EDH, ARMS0B_EDH,
	ARMS0C_EDH, ARMS0D_EDH, ARMS0E_EDH, ARMS0F_EDH,
	ARMS10_EDH, ARMS11_EDH, ARMS12_EDH, ARMS13_EDH,
};

/* DS 0x800A8140 (Arms_vbd): ARMSnn_VB names (FILECODE.H), by arms_type_no */
USHORT Arms_vbd[20] = {
	ARMS01_VB, ARMS01_VB, ARMS02_VB, ARMS03_VB,
	ARMS04_VB, ARMS05_VB, ARMS06_VB, ARMS07_VB,
	ARMS08_VB, ARMS09_VB, ARMS0A_VB, ARMS0B_VB,
	ARMS0C_VB, ARMS0D_VB, ARMS0E_VB, ARMS0F_VB,
	ARMS10_VB, ARMS11_VB, ARMS12_VB, ARMS13_VB,
};

/* DS 0x800A8168 (Main_bgm): MAINnn_BGM names (FILECODE.H), by pBgm->Main */
USHORT Main_bgm[64] = {
	MAIN00_BGM, MAIN01_BGM, MAIN02_BGM, MAIN03_BGM,
	MAIN04_BGM, MAIN05_BGM, MAIN06_BGM, MAIN07_BGM,
	MAIN08_BGM, MAIN09_BGM, MAIN0A_BGM, MAIN0B_BGM,
	MAIN0C_BGM, MAIN0D_BGM, MAIN0E_BGM, MAIN0F_BGM,
	MAIN10_BGM, MAIN11_BGM, MAIN12_BGM, MAIN13_BGM,
	MAIN14_BGM, MAIN15_BGM, MAIN16_BGM, MAIN17_BGM,
	MAIN18_BGM, MAIN19_BGM, MAIN1A_BGM, MAIN1B_BGM,
	MAIN1C_BGM, MAIN1D_BGM, MAIN1E_BGM, MAIN1F_BGM,
	MAIN20_BGM, MAIN21_BGM, MAIN22_BGM, MAIN23_BGM,
	MAIN24_BGM, MAIN25_BGM, MAIN26_BGM, MAIN27_BGM,
	MAIN28_BGM, MAIN29_BGM, MAIN2A_BGM, MAIN2B_BGM,
	MAIN2C_BGM, MAIN2D_BGM, MAIN2E_BGM, MAIN2F_BGM,
	MAIN30_BGM, MAIN31_BGM, MAIN00_BGM, MAIN33_BGM,
	MAIN34_BGM, MAIN35_BGM, MAIN36_BGM, MAIN37_BGM,
	MAIN38_BGM, MAIN39_BGM, MAIN3A_BGM, MAIN00_BGM,
	MAIN00_BGM, MAIN00_BGM, MAIN00_BGM, MAIN00_BGM,
};

/* DS 0x800A81E8 (Sub_bgm): SUB_nn_BGM names (FILECODE.H), by pBgm->Sub */
USHORT Sub_bgm[64] = {
	SUB_00_BGM, SUB_01_BGM, SUB_02_BGM, SUB_03_BGM,
	SUB_04_BGM, SUB_05_BGM, SUB_06_BGM, SUB_07_BGM,
	SUB_08_BGM, SUB_09_BGM, SUB_0A_BGM, SUB_0B_BGM,
	SUB_0C_BGM, SUB_0D_BGM, SUB_0E_BGM, SUB_0F_BGM,
	SUB_10_BGM, SUB_11_BGM, SUB_12_BGM, SUB_13_BGM,
	SUB_14_BGM, SUB_15_BGM, SUB_16_BGM, SUB_17_BGM,
	SUB_18_BGM, SUB_19_BGM, SUB_1A_BGM, SUB_1B_BGM,
	SUB_1C_BGM, SUB_1D_BGM, SUB_1E_BGM, SUB_1F_BGM,
	SUB_20_BGM, SUB_21_BGM, SUB_22_BGM, SUB_23_BGM,
	SUB_24_BGM, SUB_25_BGM, SUB_26_BGM, SUB_27_BGM,
	SUB_28_BGM, SUB_29_BGM, SUB_2A_BGM, SUB_2B_BGM,
	SUB_2C_BGM, SUB_2D_BGM, SUB_2E_BGM, SUB_2F_BGM,
	SUB_30_BGM, SUB_31_BGM, SUB_32_BGM, SUB_33_BGM,
	SUB_34_BGM, SUB_35_BGM, SUB_36_BGM, SUB_37_BGM,
	SUB_00_BGM, SUB_00_BGM, SUB_00_BGM, SUB_00_BGM,
	SUB_00_BGM, SUB_00_BGM, SUB_00_BGM, SUB_00_BGM,
};

/* PSY-Q SPU init entry, not declared in LIBSPU.H. Called as _SpuInit(0); result unused. */
extern void _SpuInit(int mode);

/* Sound bank work buffers (declared extern in SND.H) */
UCHAR Bgm_main_vh_buf[0x6010] RAM_SECTION(".buf_bgm_main_vh_buf");	/* DS 0x801f2e00, main BGM VH/seq buffer */
UCHAR Arms_edt_buf[0xD00] RAM_SECTION(".buf_arms_edt_buf");	/* DS 0x801faa00, arms EDT/VH load buffer (also death-sound VH) */
/* DS 0x800a7fac (Cd_watchdog_cnt) */
ULONG Cd_watchdog_cnt;
/* DS 0x800dfadf (Fade_time) */
UCHAR Fade_time;
/* DS 0x800e8988 (Snd_se_dist_idx) */
INT Snd_se_dist_idx = 0;
/* DS 0x801ece00 (Bgm_sub_vh_buf) */
UCHAR Bgm_sub_vh_buf[0x6000] RAM_SECTION(".buf_bgm_sub_vh_buf");
/* DS 0x801f9d10 (Core_edt_buf) */
UCHAR Core_edt_buf[0xCF0] RAM_SECTION(".buf_core_edt_buf");

/* DS 0x80059438 (Snd_sys_init) */
void Snd_sys_init(void)
{
	long i;

	Snd_sys_init_sub();
	SsInit();
	SsSetTableSize((char *)Seq_table, 3, 1);
	SsSetTickMode(1);
	SsUtSetReverbType(3);
	do {
		i = SpuClearReverbWorkArea(3);
	} while (i == -1);
	SsUtReverbOn();
	SsUtSetReverbDepth(RevD_vol.left, RevD_vol.right);
	SsSetReservedVoice(22);
	Snd_sys_start(1);
	return;
}

/* DS 0x800594c8 (Snd_vab_close) */
void Snd_vab_close(int Slot, int Flg)
{
	if (Flg != 0) {
		SsVabClose((short)(signed char)Vab_id[Slot]);
		Vab_id[Slot] = -1;
	}
	return;
}

/* DS 0x80059514 (Snd_sys_init2) */
void Snd_sys_init2(void)
{
	int eos;
	int k;
	signed char *pVab;

	RoomVbSize = 0;
	MainVbSize = 0;
	Reverb_flg = 0;
	for (k = 0; k < 3; k++) {
		if (Seq_ctr[k].Flg != 0) {
			eos = SsIsEos((short)(signed char)Seq_ctr[k].Seq_no, 0);
			if (eos != 0) {
				SsSeqStop((short)(signed char)Seq_ctr[k].Seq_no);
			}
			Seq_ctr[k].Flg = 0;
		}
		if (-1 < (signed char)Seq_ctr[k].Seq_no) {
			SsSeqClose((short)(signed char)Seq_ctr[k].Seq_no);
			Seq_ctr[k].Seq_no = -1;
		}
	}
	_SpuInit(0);
	pVab = (signed char *)Vab_id;
	do {
		if (*pVab != -1) {
			SsVabClose((short)*pVab);
			*pVab = -1;
		}
		pVab++;
	} while (pVab < (signed char *)&Vab_id[7]);
	return;
}

/* DS 0x80059634 (Snd_sys_Allvoff) */
void Snd_sys_Allvoff(void)
{
	_SpuInit(0);
	return;
}

/* DS 0x80059654 (Snd_sys_init_sub) */
void Snd_sys_init_sub(void)
{
	int i;
	int j;
	ULONG *pRec;

	Snd_fade_state = 0;
	Fade_time = 0;
	UnkSndSysInitSubFlg = 0;
	UnknownSndSysFlg = 0;
	Main_vol.left = 0x7f;
	Main_vol.right = 0x7f;
	Rev_vol.left = 0x40;
	Rev_vol.right = 0x40;
	RevD_vol.left = 0x40;
	RevD_vol.right = 0x40;
	Cd_vol.left = 0x7f;
	Cd_vol.right = 0x7f;
	Vab_id[3] = -1;
	Vab_id[2] = -1;
	Vab_id[1] = -1;
	Vab_id[0] = -1;
	Vab_id[6] = -1;
	Vab_id[5] = -1;
	Vab_id[4] = -1;
	for (i = 23; i >= 0; i--) {
		Se_pri[i].Pri = 0;
		Se_pri[i].Vch = 0;
		pRec = (ULONG *)(sel_no + i * 0x20);
		for (j = 0; j < 8; j++) {
			pRec[j] = 0;
		}
	}
	EDT_SLOT_PTR(2) = (ULONG *)&Door_snd_head;
	EDT_SLOT_PTR(3) = (ULONG *)Arms_edt_buf;
	UnkSndSysFlg0 = 0;
	pEdt_adr[5].id_check = 0;
	pEdt_adr[5].pan_prog = 0;
	pEdt_adr[5].tone_pri = 0;
	pEdt_adr[5].monopoly = 0;
	pEdt_adr[4].id_check = 0;
	pEdt_adr[4].pan_prog = 0;
	pEdt_adr[4].tone_pri = 0;
	pEdt_adr[4].monopoly = 0;
	return;
}

/* DS 0x800597a4 (Snd_key_off_all) */
void Snd_key_off_all(void)
{
	int i;
	int Ofs;
	ULONG voice_bit;
	ULONG Voice_info[16];

	for (i = 23; i >= 0; i--) {
		voice_bit = 1 << i;
		if (SpuGetKeyStatus(voice_bit) != 0) {
			Voice_info[0] = voice_bit;
			Spu_voice_info_get(Voice_info);
			/* Voice_info[7]: SPU start address field */
			if ((ULONG)(Voice_info[7] - 0x14441) < 169999) {
				SpuSetKey(0, voice_bit);
			}
		}
	}
	for (Ofs = 0x2e0; Ofs >= 0; Ofs -= 0x20) {
		if ((*(short *)(sel_no + Ofs + 4) != 4) || (sel_no[Ofs + 1] != 3)) {
			sel_no[Ofs] = 0;
			*(short *)(sel_no + Ofs + 0x12) = 0;
		}
	}
	return;
}

/* DS 0x800598a8 (Snd_sys_init_sub2) */
void Snd_sys_init_sub2(void)
{
	int k;

	RoomVbSize = 0;
	MainVbSize = 0;
	Bgm.Sub = 0xff;
	Bgm.Main = 0xff;
	for (k = 0; k < 3; k++) {
		Seq_ctr[k].Flg = 0;
		Seq_ctr[k].Ctrl = 0xff;
		Seq_ctr[k].Seq_no = -1;
		Seq_ctr[k].Vab_id = -1;
		Seq_ctr[k].Vol.left = 0x6e;
		Seq_ctr[k].Vol.right = 0x6e;
	}
	return;
}

/* DS 0x8005990c (Snd_sys_start) */
void Snd_sys_start(UCHAR ck)
{
	if (ck != 0) {
		SpuInitHot();
	}
	SsSetMVol(Main_vol.left, Main_vol.right);
	SsSetRVol(Rev_vol.left, Rev_vol.right);
	SsSetSerialAttr(0, 0, 0);
	SsSetSerialVol(0, Cd_vol.left, Cd_vol.right);
	return;
}

/* DS 0x8005998c (Snd_sys_stereo) */
void Snd_sys_stereo(UCHAR sorm)
{
	DslATV volume[2];

	if ((sorm & 1) == 0) {
		Cd_set_stereo(1);
		SsSetStereo();
		volume[0].val0 = 127;
		volume[0].val1 = 0;
		volume[0].val2 = 127;
		volume[0].val3 = 0;
	} else {
		Cd_set_stereo(0);
		SsSetMono();
		volume[0].val0 = 63;
		volume[0].val1 = 63;
		volume[0].val2 = 63;
		volume[0].val3 = 63;
	}
	DsMix(volume);
	return;
}

/* DS 0x80059a00 (Snd_sys_core_set) */
void Snd_sys_core_set(UCHAR core_type_no, UCHAR sw)
{
	short sts;
	ULONG cd_ret;
	int outer;
	int trans_cnt;

	outer = 0;
	trans_cnt = 0;
	UnknownSndSysFlg = 1;
	do {
		while (1) {
			Task_sleep(1);
			SpuSetTransferMode(0);
			if ((signed char)Vab_id[4] != -1) {
				SsVabClose((short)(signed char)Vab_id[4]);
				Vab_id[4] = -1;
			}
			UnkSndFlg0 = (ULONG)Core_edt_buf;
			cd_ret = Cd_read((UINT)((USHORT *)Core_edh)[core_type_no], (ULONG *)Core_edt_buf, 1,
							 (UCHAR *)"CORE_EDH");
			pVh_tbl[4] = (ULONG *)(Core_edt_buf + EDT_VAB_OFS(Core_edt_buf, cd_ret));
			do {
				Task_sleep(1);
				sts = SsVabTransCompleted(0);
			} while (sts == 0);
			sts = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[4], 4, 0x1020);
			Vab_id[4] = (char)sts;
			if ((signed char)Vab_id[4] == -1) break;
			outer = 0;
			if (sw == 4) {
				Cd_read((UINT)((USHORT *)Core_vbd)[core_type_no], Bg_img_buf, 0,
						(UCHAR *)"CORE_VBD");
				sts = SsVabTransBody((UCHAR *)Bg_img_buf, (short)(signed char)Vab_id[4]);
				trans_cnt++;
				if (sts != -1) {
					Cd_watchdog_cnt = 0;
					while (sts = SsVabTransCompleted(0), sts == 0) {
						Snd_cd_watchdog();
						Task_sleep(1);
					}
					Task_sleep(1);
					UnknownSndSysFlg = 0;
					return;
				}
				Snd_vab_close((int)(signed char)Vab_id[4], 0x1020);
				if (trans_cnt == 8) goto core_load_done;
			} else {
				cd_ret = Cd_read_s((UINT)((USHORT *)Core_vbd)[core_type_no], 0, Font_tim_buf,
								   (int)(signed char)Vab_id[4], (UCHAR *)"CORE_VBD");
				if (cd_ret != 0xffffffff) {
					Pl_set_packet();
					Task_sleep(1);
					UnknownSndSysFlg = 0;
					return;
				}
			}
		}
		outer++;
		Snd_vab_close(4, 0);
	} while (outer != 8);
core_load_done:
	Task_sleep(1);
	return;
}

/* DS 0x80059c74 (Snd_sys_arms_set) */
void Snd_sys_arms_set(UCHAR arms_type_no)
{
	short sts;
	ULONG cd_ret;
	int vab_off;
	int wait_cnt;
	int retry_cnt;

	retry_cnt = 0;
	UnknownSndSysFlg = 1;
	while (1) {
		Task_sleep(1);
		SpuSetTransferMode(0);
		if ((signed char)Vab_id[1] != -1) {
			SsVabClose((short)(signed char)Vab_id[1]);
			Vab_id[1] = -1;
		}
		cd_ret = Cd_read((UINT)((USHORT *)Arms_edh)[arms_type_no], (ULONG *)Arms_edt_buf, 1,
						 (UCHAR *)"ARMS_EDT");
		vab_off = EDT_VAB_OFS(Arms_edt_buf, cd_ret);
		pVh_tbl[1] = (ULONG *)(Arms_edt_buf + vab_off);
		wait_cnt = 0x96;
		if (36000 < ((((VabHdr *)(Arms_edt_buf + vab_off))->fsize + -0xa10 +
					 (UINT)((VabHdr *)(Arms_edt_buf + vab_off))->ps * -0x200) & 0xfffffff0)) break;
		do {
			Task_sleep(1);
			sts = SsVabTransCompleted(0);
		} while (sts == 0);
		sts = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[1], 1, 0xb7a0);
		Vab_id[1] = (char)sts;
		if ((int)(signed char)Vab_id[1] == 0xffffffff) {
			retry_cnt++;
			Snd_vab_close(1, 0);
			if (retry_cnt == 8) {
				Task_sleep(1);
				return;
			}
		} else {
			retry_cnt = 0;
			cd_ret = Cd_read_s((UINT)((USHORT *)Arms_vbd)[arms_type_no], 0, Font_tim_buf, (int)(signed char)Vab_id[1],
							   (UCHAR *)"ARMS_VBD");
			if (cd_ret != 0xffffffff) {
				Pl_set_packet();
				Task_sleep(1);
				UnknownSndSysFlg = 0;
				return;
			}
		}
	}
	do {
		Task_sleep(1);
		wait_cnt = wait_cnt + -1;
	} while (wait_cnt != 0);
	return;
}

/* DS 0x80059e54 (Snd_sys_room_set) */
void Snd_sys_room_set(void)
{
	short sts;
	int retry_cnt;
	int trans_cnt;

	retry_cnt = 0;
	trans_cnt = 0;
	UnknownSndSysFlg = 1;
	do {
		while (1) {
			Task_sleep(1);
			SpuSetTransferMode(0);
			Snd_key_off_all();
			if ((signed char)Vab_id[2] != -1) {
				SsVabClose((short)(signed char)Vab_id[2]);
				Vab_id[2] = -1;
			}
			if ((signed char)Vab_id[3] != -1) {
				SsVabClose((short)(signed char)Vab_id[3]);
				Vab_id[3] = -1;
			}
			if ((G.pRoom)->pVh0 == (ULONG *)0x0) {
				RoomVbSize = 0;
				return;
			}
			EDT_SLOT_PTR(4) = (ULONG *)(G.pRoom)->pEdt0;
			Reverb_flg = (G.pRoom)->Reverb_lv << 2;
			pVh_tbl[2] = (G.pRoom)->pVh0;
			RoomVbSize = ((((VabHdr *)(G.pRoom)->pVh0)->fsize - 0xa20) +
						 (UINT)((VabHdr *)(G.pRoom)->pVh0)->ps * -0x200) & 0xfffffff0;
			UnkSndSysFlg0 = EDT_SLOT_WORD(4);
			do {
				Task_sleep(1);
				sts = SsVabTransCompleted(0);
			} while (sts == 0);
			sts = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[2], 2, 0x14440);
			Vab_id[2] = (char)sts;
			retry_cnt++;
			if ((signed char)Vab_id[2] != -1) break;
			Snd_vab_close(2, 0);
			if (retry_cnt == 8) goto room_load_done;
		}
		retry_cnt = 0;
		sts = SsVabTransBody((UCHAR *)(G.pRoom)->pVb0, (short)(signed char)Vab_id[2]);
		trans_cnt++;
		if (sts != -1) {
			Cd_watchdog_cnt = 0;
			while (sts = SsVabTransCompleted(0), sts == 0) {
				Snd_cd_watchdog();
				Task_sleep(1);
			}
			Task_sleep(1);
			UnknownSndSysFlg = 0;
			return;
		}
		Snd_vab_close((int)(signed char)Vab_id[2], 0x14440);
	} while (trans_cnt != 8);
room_load_done:
	Task_sleep(1);
	return;
}

/* DS 0x8005a09c (Snd_sys_enem_set) */
void Snd_sys_enem_set(void)
{
	short sts;
	ULONG cd_ret;
	ULONG snd_buf;
	UINT enem_type_no;
	int retry_cnt;

	retry_cnt = 0;
	UnknownSndSysFlg = 1;
	while (1) {
		Task_sleep(1);
		SpuSetTransferMode(0);
		enem_type_no = (UINT)G.Em_se_set_no;
		snd_buf = RoomVbSize + 0x14440;
		if ((signed char)Vab_id[3] != -1) {
			SsVabClose((short)(signed char)Vab_id[3]);
			Vab_id[3] = -1;
		}
		if (enem_type_no == 0xff) break;
		EDT_SLOT_PTR(5) = (ULONG *)&Enemy_edt;
		CD.Fsize = EnemSE_floc[enem_type_no * 2].size;
		CD.Fno = ENEMSE_VBS;
		CD.Fsector = (UINT)floc[ENEMSE_VBS].sector + (UINT)floc[ENEMSE_VBS].sec_high * 0x10000 +
					 (UINT)EnemSE_floc[enem_type_no * 2].sector +
					 (UINT)EnemSE_floc[enem_type_no * 2].sec_high * 0x10000;
		CD.Fsum = EnemSE_floc[enem_type_no * 2].sum;
		cd_ret = Cd_read(ENEMSE_VBS, (ULONG *)&Enemy_edt, 3, (UCHAR *)"ENEM_EDT");
		pVh_tbl[3] = (ULONG *)(&Enemy_edt.id_check + EDT_VAB_OFS(&Enemy_edt, cd_ret));
		do {
			Task_sleep(1);
			sts = SsVabTransCompleted(0);
		} while (sts == 0);
		sts = SsVabOpenHeadSticky((UCHAR *)pVh_tbl[3], 3, snd_buf);
		Vab_id[3] = (char)sts;
		if ((int)(signed char)Vab_id[3] == 0xffffffff) {
			retry_cnt++;
			Snd_vab_close(3, 0);
			if (retry_cnt == 8) {
				Task_sleep(1);
				return;
			}
		} else {
			retry_cnt = 0;
			enem_type_no = enem_type_no << 4 | 8;
			CD.Fsize = EnemSE_floc[enem_type_no >> 3].size;
			CD.Fno = ENEMSE_VBS;
			CD.Fsector = (UINT)floc[ENEMSE_VBS].sector + (UINT)floc[ENEMSE_VBS].sec_high * 0x10000 +
						 (UINT)*(USHORT *)((int)&EnemSE_floc[0].sector + enem_type_no) +
						 (UINT)(&EnemSE_floc[0].sec_high)[enem_type_no] * 0x10000;
			CD.Fsum = (&EnemSE_floc[0].sum)[enem_type_no];
			snd_buf = Cd_read_s(ENEMSE_VBS, 1, Font_tim_buf, (int)(signed char)Vab_id[3],
								(UCHAR *)"ENEM_VBD");
			if (snd_buf != 0xffffffff) {
				Pl_set_packet();
				Task_sleep(1);
				UnknownSndSysFlg = 0;
				return;
			}
		}
	}
	return;
}

/* DS 0x8005a338 (Snd_sys_pset) */
void Snd_sys_pset(void)
{
	short sts;
	char *pVab;

	pVab = &Vab_id[2];
	if ((signed char)*pVab != -1) {
		SsVabClose((short)(signed char)*pVab);
		*pVab = -1;
	}
	EDT_SLOT_PTR(4) = (ULONG *)(G.pRoom)->pEdt0;
	pVh_tbl[2] = (G.pRoom)->pVh0;
	UnkSndSysFlg0 = EDT_SLOT_WORD(4);
	do {
		sts = SsVabFakeHead((UCHAR *)pVh_tbl[2], 2, 0x14440);
		*pVab = (char)sts;
	} while ((signed char)*pVab == -1);
	do {
		sts = SsVabFakeBody((short)(signed char)*pVab);
		*pVab = (char)sts;
	} while ((signed char)*pVab == -1);
	return;
}

/* DS 0x8005a414 (Snd_bgm_data) */
void Snd_bgm_data(USHORT *pTbl)
{
	USHORT *pDst;
	USHORT *pSrc;
	int cnt0;

	cnt0 = 0x8e;
	pDst = (USHORT *)&G.SaveData.F_atari;
	pSrc = pTbl + 0x8e;
	do {
		pDst = pDst + -1;
		pSrc = pSrc + -1;
		cnt0 = cnt0 + -1;
		*pDst = *pSrc;
	} while (cnt0 != 0);
	return;
}

/* DS 0x8005a444 (Snd_bgm_set) */
void Snd_bgm_set(void)
{
	UCHAR bgm_main;
	UCHAR bgm_sub;
	BOOL changed;
	char res;
	USHORT vol16;

	if ((G.System_flg & SYS_DEMO_MODE) == 0) {
		pBgm = &Bgm_room_tbl[nroom[G.SaveData.Stage_no] + G.SaveData.Room_no];
		bgm_main = pBgm->Main;
		bgm_sub = pBgm->Sub;
		if (Snd_fade_state != 0) {
			do {
				Task_sleep(1);
			} while (Snd_fade_state != 0);
		}
		if ((bgm_main & 0x3f) == (Bgm.Main & 0x3f)) {
			changed = 0;
			if (bgm_main >> 6 == 0) {
				if (Seq_ctr[0].Flg == 2) {
					if (-1 < (signed char)Seq_ctr[0].Seq_no) {
						SsSeqPlay((short)(signed char)Seq_ctr[0].Seq_no, 0, 1);
						vol16 = (USHORT)
								((((UINT)Seq_ctr[0].Vol.left * (UINT)Volume_Bgm) / 100) *
								 0x10000 >> 0x10);
						SsSeqSetVol((short)(signed char)Seq_ctr[0].Seq_no, vol16, vol16);
					}
					Seq_ctr[0].Ctrl = 0;
				}
			} else if ((Seq_ctr[0].Flg != 0) && (Seq_ctr[0].Flg != 0xff)) {
				SsSndCrescendo((short)(signed char)Seq_ctr[0].Seq_no, 0x7f, 0x5a);
				Seq_ctr[0].Flg = 15;
			}
		} else {
			changed = 1;
			res = Snd_bgm_main();
			if (res == 0) {
				Seq_ctr[0].Ctrl = bgm_main >> 6;
			} else {
				Seq_ctr[0].Ctrl = 0xff;
			}
		}
		if (((bgm_sub & 0x3f) != (Bgm.Sub & 0x3f)) || (changed)) {
			res = Snd_bgm_sub();
			if (res == 0) {
				Seq_ctr[2].Ctrl = bgm_sub >> 7;
				Seq_ctr[1].Ctrl = bgm_sub >> 6 & 1;
			} else {
				Seq_ctr[1].Ctrl = 0xff;
				Seq_ctr[2].Ctrl = 0xff;
			}
		} else {
			if (((bgm_sub >> 6 & 1) != 0) && (Seq_ctr[1].Flg != 0)) {
				SsSndCrescendo((short)(signed char)Seq_ctr[1].Seq_no, 0x7f, 0x5a);
			}
			if (((signed char)bgm_sub < 0) && (Seq_ctr[2].Flg != 0)) {
				SsSndCrescendo((short)(signed char)Seq_ctr[2].Seq_no, 0x7f, 0x5a);
			}
		}
		Bgm.Main = pBgm->Main;
		Bgm.Sub = pBgm->Sub;
	}
	return;
}

/* DS 0x8005a714 (Snd_bgm_ck) */
void Snd_bgm_ck(void)
{
	BOOL more;
	int wait_cnt;

	if ((G.System_flg & SYS_DEMO_MODE) == 0) {
		pBgm = &Bgm_room_tbl[nroom[G.SaveData.Stage_no] + G.SaveData.Room_no];
		if ((pBgm->Main & 0x3f) == (Bgm.Main & 0x3f)) {
			if ((pBgm->Sub & 0x3f) != (Bgm.Sub & 0x3f)) {
				if ((Seq_ctr[1].Flg == 1) &&
					(SsIsEos((short)(signed char)Seq_ctr[1].Seq_no, 0) != 0)) {
					SsSndCrescendo((short)(signed char)Seq_ctr[1].Seq_no, 0x7f, 0x5a);
					Seq_ctr[1].Flg = 50;
				}
				if ((Seq_ctr[2].Flg == 1) &&
					(SsIsEos((short)(signed char)Seq_ctr[2].Seq_no, 0) != 0)) {
					SsSndCrescendo((short)(signed char)Seq_ctr[2].Seq_no, 0x7f, 0x5a);
					Seq_ctr[2].Flg = 50;
				}
			}
			wait_cnt = 0x59;
			do {
				Task_sleep(1);
				more = wait_cnt != 0;
				wait_cnt = wait_cnt + -1;
			} while (more);
		} else {
			Snd_bgm_fade_ON(90, 22);
		}
	}
	return;
}

/* DS 0x8005a888 (Snd_bgm_play_ck) */
void Snd_bgm_play_ck(void)
{
	if ((G.System_flg & SYS_DEMO_MODE) == 0) {
		if (((Seq_ctr[0].Ctrl != 0xff) && (Seq_ctr[0].Ctrl == 0)) &&
			(-1 < (signed char)Seq_ctr[0].Seq_no)) {
			SsSeqReplay((short)(signed char)Seq_ctr[0].Seq_no);
			Seq_ctr[0].Flg = 1;
		}
		if (((Seq_ctr[1].Ctrl != 0xff) && (Seq_ctr[1].Ctrl == 0)) &&
			(-1 < (signed char)Seq_ctr[1].Seq_no)) {
			SsSeqReplay((short)(signed char)Seq_ctr[1].Seq_no);
			Seq_ctr[1].Flg = 1;
		}
		if (((Seq_ctr[2].Ctrl != 0xff) && (Seq_ctr[2].Ctrl == 0)) &&
			(-1 < (signed char)Seq_ctr[2].Seq_no)) {
			SsSeqReplay((short)(signed char)Seq_ctr[2].Seq_no);
			Seq_ctr[2].Flg = 1;
		}
	}
	return;
}

/* DS 0x8005a97c (Snd_bgm_main) */
char Snd_bgm_main(void)
{
	short eos;
	UINT ret;
	ULONG cd_ret;
	int seq_off;
	int vab_off;
	BGM_FOOTER *pFt;
	USHORT vol16;
	int cd_size;
	ULONG *pDatam;
	int retry_cnt;
	int trans_cnt;

	ret = G.System_flg & SYS_DEMO_MODE;
	if (ret == 0) {
		UnknownSndSysFlg = 1;
		retry_cnt = 0;
		trans_cnt = 0;
main_bgm_retry:
		do {
			Task_sleep(1);
			SpuSetTransferMode(0);
			MainVbSize = 0;
			if (-1 < (signed char)Seq_ctr[0].Seq_no) {
				if (Seq_ctr[0].Flg != 0) {
					eos = SsIsEos((short)(signed char)Seq_ctr[0].Seq_no, 0);
					if (eos != 0) {
						SsSeqStop((short)(signed char)Seq_ctr[0].Seq_no);
					}
					Seq_ctr[0].Flg = 0;
				}
				SsSeqClose((short)(signed char)Seq_ctr[0].Seq_no);
				Seq_ctr[0].Seq_no = -1;
			}
			if ((signed char)Vab_id[5] != -1) {
				SsVabClose((short)(signed char)Vab_id[5]);
				Vab_id[5] = -1;
			}
			if (pBgm->Main == 0xff) goto main_bgm_fail;
			seq_off = (int)(signed char)Seq_ctr[1].Seq_no;
			if (-1 < seq_off) {
				if (Seq_ctr[1].Flg != 0) {
					eos = SsIsEos((short)(signed char)Seq_ctr[1].Seq_no, 0);
					if (eos != 0) {
						SsSeqStop((short)(signed char)Seq_ctr[1].Seq_no);
					}
					Seq_ctr[1].Flg = 0;
					seq_off = (int)(signed char)Seq_ctr[1].Seq_no;
				}
				SsSeqClose((short)seq_off);
				Seq_ctr[1].Seq_no = -1;
			}
			seq_off = (int)(signed char)Seq_ctr[2].Seq_no;
			if (-1 < seq_off) {
				if (Seq_ctr[2].Flg != 0) {
					eos = SsIsEos((short)(signed char)Seq_ctr[2].Seq_no, 0);
					if (eos != 0) {
						SsSeqStop((short)(signed char)Seq_ctr[2].Seq_no);
					}
					Seq_ctr[2].Flg = 0;
					seq_off = (int)(signed char)Seq_ctr[2].Seq_no;
				}
				SsSeqClose((short)seq_off);
				Seq_ctr[2].Seq_no = -1;
			}
			if ((signed char)Vab_id[6] != -1) {
				SsVabClose((short)(signed char)Vab_id[6]);
				Vab_id[6] = -1;
			}
			pDatam = (ULONG *)((UINT)(G.pFree_work + 0x10) & 0xfffffff0);
			cd_ret = Cd_read((UINT)((USHORT *)Main_bgm)[pBgm->Main & 0x3f], pDatam, 1,
							 (UCHAR *)"MAIN_BGM");
			if (cd_ret == 0xffffffff) goto main_bgm_fail;
			pFt = (BGM_FOOTER *)((UCHAR *)pDatam + cd_ret - sizeof(BGM_FOOTER));
			seq_off = pFt->seq_off;
			vab_off = pFt->vab_off;
			cd_size = pFt->cd_size;
			pVh_tbl[5] = (ULONG *)(Bgm_main_vh_buf + vab_off);
			Memcpy(Bgm_main_vh_buf, pDatam, cd_size);
			MainVbSize = (((VabHdr *)pVh_tbl[5])->fsize - 0xa20) +
						 (UINT)((VabHdr *)pVh_tbl[5])->ps * -0x200;
			if (0x38800 < MainVbSize) {
				MainVbSize = 0x96;
				do {
					Task_sleep(1);
					MainVbSize--;
				} while (MainVbSize != 0);
main_bgm_fail:
				ret = 0xffffffff;
				goto main_bgm_ret;
			}
			do {
				Task_sleep(1);
				eos = SsVabTransCompleted(0);
			} while (eos == 0);
			eos = SsVabOpenHeadSticky(Bgm_main_vh_buf + vab_off, 5, 0x42fc0);
			Vab_id[5] = (char)eos;
			retry_cnt++;
			if ((signed char)Vab_id[5] != -1) {
				retry_cnt = 0;
				eos = SsVabTransBody((UCHAR *)((int)pDatam + cd_size), (short)(signed char)Vab_id[5]);
				trans_cnt++;
				if (eos != -1) {
					Cd_watchdog_cnt = 0;
					while (eos = SsVabTransCompleted(0), eos == 0) {
						Snd_cd_watchdog();
						Task_sleep(1);
					}
					eos = SsSeqOpen((ULONG *)(Bgm_main_vh_buf + seq_off), (short)(signed char)Vab_id[5]);
					Seq_ctr[0].Seq_no = (char)eos;
					if (-1 < (signed char)Seq_ctr[0].Seq_no) {
						SsSeqPlay((short)(signed char)Seq_ctr[0].Seq_no, 0, 1);
						vol16 = (USHORT)
								((((UINT)Seq_ctr[0].Vol.left * (UINT)Volume_Bgm) / 100) *
								 0x10000 >> 0x10);
						SsSeqSetVol((short)(signed char)Seq_ctr[0].Seq_no, vol16, vol16);
					}
					Seq_ctr[0].Flg = 0;
					Task_sleep(1);
					ret = 0;
					UnknownSndSysFlg = 0;
					goto main_bgm_ret;
				}
				Snd_vab_close((int)(signed char)Vab_id[5], 0x42fc0);
				if (trans_cnt == 8) break;
				goto main_bgm_retry;
			}
			Snd_vab_close(5, 0);
		} while (retry_cnt != 8);
		ret = 8;
		Task_sleep(1);
	}
main_bgm_ret:
	return (char)ret;
}

/* DS 0x8005ae50 (Snd_bgm_sub) */
char Snd_bgm_sub(void)
{
	BOOL more;
	short eos;
	UINT ret;
	ULONG cd_ret;
	int seq_off2;
	int seq_off;
	int vab_off;
	BGM_FOOTER *pFt;
	USHORT vol16;
	int cd_size;
	ULONG *pDatam;
	int retry_cnt;
	int trans_cnt;

	ret = G.System_flg & SYS_DEMO_MODE;
	if (ret == 0) {
		UnknownSndSysFlg = 1;
		retry_cnt = 0;
		trans_cnt = 0;
sub_bgm_retry:
		do {
			Task_sleep(1);
			SpuSetTransferMode(0);
			seq_off = (int)(signed char)Seq_ctr[1].Seq_no;
			if (-1 < seq_off) {
				if (Seq_ctr[1].Flg != 0) {
					eos = SsIsEos((short)(signed char)Seq_ctr[1].Seq_no, 0);
					if (eos != 0) {
						SsSeqStop((short)(signed char)Seq_ctr[1].Seq_no);
					}
					Seq_ctr[1].Flg = 0;
					seq_off = (int)(signed char)Seq_ctr[1].Seq_no;
				}
				SsSeqClose((short)seq_off);
				Seq_ctr[1].Seq_no = -1;
			}
			seq_off = (int)(signed char)Seq_ctr[2].Seq_no;
			if (-1 < seq_off) {
				if (Seq_ctr[2].Flg != 0) {
					eos = SsIsEos((short)(signed char)Seq_ctr[2].Seq_no, 0);
					if (eos != 0) {
						SsSeqStop((short)(signed char)Seq_ctr[2].Seq_no);
					}
					Seq_ctr[2].Flg = 0;
					seq_off = (int)(signed char)Seq_ctr[2].Seq_no;
				}
				SsSeqClose((short)seq_off);
				Seq_ctr[2].Seq_no = -1;
			}
			if ((signed char)Vab_id[6] != -1) {
				SsVabClose((short)(signed char)Vab_id[6]);
				Vab_id[6] = -1;
			}
			if (pBgm->Sub == 0xff) goto sub_bgm_fail;
			pDatam = (ULONG *)((UINT)(G.pFree_work + 0x10) & 0xfffffff0);
			cd_ret = Cd_read((UINT)((USHORT *)Sub_bgm)[pBgm->Sub & 0x3f], pDatam, 1,
							 (UCHAR *)"SUB_BGM");
			if (cd_ret == 0xffffffff) goto sub_bgm_fail;
			pFt = (BGM_FOOTER *)((UCHAR *)pDatam + cd_ret - 16);	/* sub footer starts 4 bytes before the main one */
			seq_off2 = pFt->seq_off;
			vab_off = pFt->vab_off;
			cd_size = pFt->cd_size;
			pVh_tbl[6] = (ULONG *)(Bgm_sub_vh_buf + vab_off);
			Memcpy(Bgm_sub_vh_buf, pDatam, cd_size);
			if (0x38800 < (((VabHdr *)pVh_tbl[6])->fsize - 0xa20) +
						  (UINT)((VabHdr *)pVh_tbl[6])->ps * -0x200 + MainVbSize) {
				retry_cnt = 0x95;
				do {
					more = retry_cnt != 0;
					retry_cnt = retry_cnt + -1;
				} while (more);
sub_bgm_fail:
				ret = 0xffffffff;
				goto sub_bgm_ret;
			}
			cd_ret = MainVbSize + 0x42fc0;
			do {
				Task_sleep(1);
				eos = SsVabTransCompleted(0);
			} while (eos == 0);
			eos = SsVabOpenHeadSticky(Bgm_sub_vh_buf + vab_off, 6, cd_ret);
			Vab_id[6] = (char)eos;
			retry_cnt++;
			if ((signed char)Vab_id[6] != -1) {
				retry_cnt = 0;
				eos = SsVabTransBody((UCHAR *)((int)pDatam + cd_size), (short)(signed char)Vab_id[6]);
				trans_cnt++;
				if (eos != -1) {
					Cd_watchdog_cnt = 0;
					while (eos = SsVabTransCompleted(0), eos == 0) {
						Snd_cd_watchdog();
						Task_sleep(1);
					}
					eos = SsSeqOpen((ULONG *)Bgm_sub_vh_buf, (short)(signed char)Vab_id[6]);
					Seq_ctr[1].Seq_no = (char)eos;
					if (-1 < (signed char)Seq_ctr[1].Seq_no) {
						SsSeqPlay((short)(signed char)Seq_ctr[1].Seq_no, 0, 1);
						vol16 = (USHORT)
								((((UINT)Seq_ctr[1].Vol.left * (UINT)Volume_Bgm) / 100) *
								 0x10000 >> 0x10);
						SsSeqSetVol((short)(signed char)Seq_ctr[1].Seq_no, vol16, vol16);
					}
					eos = SsSeqOpen((ULONG *)(Bgm_sub_vh_buf + seq_off2), (short)(signed char)Vab_id[6]);
					Seq_ctr[2].Seq_no = (char)eos;
					if (-1 < (signed char)Seq_ctr[2].Seq_no) {
						SsSeqPlay((short)(signed char)Seq_ctr[2].Seq_no, 0, 1);
						vol16 = (USHORT)
								((((UINT)Seq_ctr[2].Vol.left * (UINT)Volume_Bgm) / 100) *
								 0x10000 >> 0x10);
						SsSeqSetVol((short)(signed char)Seq_ctr[2].Seq_no, vol16, vol16);
					}
					ret = 0;
					Seq_ctr[1].Flg = 0;
					Seq_ctr[2].Flg = 0;
					UnknownSndSysFlg = 0;
					goto sub_bgm_ret;
				}
				Snd_vab_close((int)(signed char)Vab_id[6], cd_ret);
				if (trans_cnt == 8) break;
				goto sub_bgm_retry;
			}
			Snd_vab_close(6, 0);
		} while (retry_cnt != 8);
		ret = 8;
		Task_sleep(1);
	}
sub_bgm_ret:
	return (char)ret;
}

/* DS 0x8005b2e4 (Snd_bgm_fade_ON) */
void Snd_bgm_fade_ON(UCHAR time, UCHAR mode)
{
	if ((G.System_flg & SYS_DEMO_MODE) == 0) {
		Snd_fade_state = 1;
		Fade_mode = mode;
		Fade_time = time;
		if (Seq_ctr[0].Flg == 1) {
			SsSndCrescendo((short)(signed char)Seq_ctr[0].Seq_no, 0x7f, (UINT)time);
		}
		if (Seq_ctr[1].Flg == 1) {
			SsSndCrescendo((short)(signed char)Seq_ctr[1].Seq_no, 0x7f, (UINT)Fade_time);
		}
		if (Seq_ctr[2].Flg == 1) {
			SsSndCrescendo((short)(signed char)Seq_ctr[2].Seq_no, 0x7f, (UINT)Fade_time);
		}
	}
	return;
}

/* DS 0x8005b3b0 (Snd_bgm_fade) */
void Snd_bgm_fade(void)
{
	BOOL hit;
	int vol_l;
	int vol_r;
	short eos;
	long rev_st;
	int cnt;

	if ((G.System_flg & SYS_DEMO_MODE) == 0) {
		/* DS tests Snd_fade_state != 1 first: the state 0 and 2 code is inline and the state 1 (fade out) block follows it */
		if (Snd_fade_state != 1) {
			if (Snd_fade_state < 2) {
				if (Snd_fade_state == 0) {
					if ((5 < Seq_ctr[0].Flg) &&
						(hit = Seq_ctr[0].Flg == 6,
						 Seq_ctr[0].Flg = Seq_ctr[0].Flg + 0xff, hit)) {
						Seq_ctr[0].Flg = 0;
						eos = SsIsEos((short)(signed char)Seq_ctr[0].Seq_no, 0);
						if (eos != 0) {
							SsSeqStop((short)(signed char)Seq_ctr[0].Seq_no);
						}
					}
					if ((5 < Seq_ctr[1].Flg) &&
						(hit = Seq_ctr[1].Flg == 6,
						 Seq_ctr[1].Flg = Seq_ctr[1].Flg + 0xff, hit)) {
						Seq_ctr[1].Flg = 0;
						eos = SsIsEos((short)(signed char)Seq_ctr[1].Seq_no, 0);
						if (eos != 0) {
							SsSeqStop((short)(signed char)Seq_ctr[1].Seq_no);
						}
					}
					if ((5 < Seq_ctr[2].Flg) &&
						(hit = Seq_ctr[2].Flg == 6,
						 Seq_ctr[2].Flg = Seq_ctr[2].Flg + 0xff, hit)) {
						Seq_ctr[2].Flg = 0;
						eos = SsIsEos((short)(signed char)Seq_ctr[2].Seq_no, 0);
						if (eos != 0) {
							SsSeqStop((short)(signed char)Seq_ctr[2].Seq_no);
						}
					}
				}
			} else if (Snd_fade_state == 2) {
				_SpuInit(0);
				do {
					rev_st = SpuClearReverbWorkArea(3);
				} while (rev_st == -1);
				Fade_time = 0;
				Snd_fade_state = 0;
			}
		} else {
			Fade_time--;
			if ((char)Fade_mode == 24) {
				vol_l = (Main_vol.left * 0x62) / 100;
				vol_r = (Main_vol.right * 0x62) / 100;
				Main_vol.right = (USHORT)vol_r;
				Main_vol.left = (USHORT)vol_l;
				SsSetMVol((short)(vol_l * 0x10000 >> 0x10), (short)(vol_r * 0x10000 >> 0x10));
			}
			cnt = 3;
			if (Fade_time == 1) {
				do {
					cnt = cnt + -1;
					if (Seq_ctr[cnt].Flg != 0) {
						Seq_ctr[cnt].Flg = 0;
						eos = SsIsEos((short)(signed char)Seq_ctr[cnt].Seq_no, 0);
						if (eos != 0) {
							SsSeqStop((short)(signed char)Seq_ctr[cnt].Seq_no);
						}
					}
				} while (cnt != 0);
				if ((char)Fade_mode == 24) {
					Main_vol.left = 0x7f;
					Main_vol.right = 0x7f;
					SsSetMVol(0x7f, 0x7f);
				}
				Snd_fade_state = 2;
			}
		}
	}
	return;
}

/* VAB header view used by Snd_bgm_ctr (ProgAtr[] is 0x10 bytes each, program numbers are 1-based).
 * retail Snd_bgm_ctr 8005b6f0: main volume sb +0x18 (8005b99c), main pan sb +0x19 (8005b9cc);
 * program volume sb at ProgAtr +1 (8005b998, entry base -0xf), program pan sb at ProgAtr +4 (8005b9c4, -0xc). */
typedef struct {
	UCHAR Unk_00;
	UCHAR Mvol;					/* +0x01 program volume */
	UCHAR Unk_02[2];
	UCHAR Pan;					/* +0x04 program pan */
	UCHAR Unk_05[11];
} SND_PROG_ATR;

typedef struct {
	UCHAR Unk_00[0x18];
	UCHAR Mvol;					/* +0x18 main volume */
	UCHAR Pan;					/* +0x19 main pan */
	UCHAR Unk_1a[6];
	SND_PROG_ATR Prog[16];		/* +0x20 ProgAtr[16] */
} SND_VAB_HDR;

/* DS 0x8005b6f0 (Snd_bgm_ctr) */
void Snd_bgm_ctr(ULONG code)
{
	UCHAR state;
	short eos;
	USHORT vol16;
	UINT ch;
	ULONG *pVh;
	char vol_val;

	if ((G.System_flg & SYS_DEMO_MODE) != 0) {
		return;
	}
	ch = code >> 0x1c;
	if (Seq_ctr[ch].Ctrl == 0xff) {
		return;
	}
	if ((signed char)Seq_ctr[ch].Seq_no == -1) {
		return;
	}
	pVh = pVh_tbl[5];
	if (ch != 0) {
		pVh = pVh_tbl[6];
	}
	switch (code >> 0x18 & 0xf) {
	default:
		goto bgm_ctr_default;
	case 1:
		vol16 = (USHORT)
				((((UINT)Seq_ctr[ch].Vol.left * (UINT)Volume_Bgm) / 100) * 0x10000 >> 0x10);
		SsSeqSetVol((short)(signed char)Seq_ctr[ch].Seq_no, vol16, vol16);
		SsSeqPlay((short)(signed char)Seq_ctr[ch].Seq_no, 1, 1);
		state = 1;
		break;
	case 2:
		if (Seq_ctr[ch].Flg == 0) goto seq_ctrl_done;
		eos = SsIsEos((short)(signed char)Seq_ctr[ch].Seq_no, 0);
		if (eos != 0) {
			SsSeqSetVol((short)(signed char)Seq_ctr[ch].Seq_no, 0, 0);
			SsSeqStop((short)(signed char)Seq_ctr[ch].Seq_no);
		}
		state = 2;
		break;
	case 3:
		vol16 = (USHORT)
				((((UINT)Seq_ctr[ch].Vol.left * (UINT)Volume_Bgm) / 100) * 0x10000 >> 0x10);
		SsSeqSetVol((short)(signed char)Seq_ctr[ch].Seq_no, vol16, vol16);
		SsSeqReplay((short)(signed char)Seq_ctr[ch].Seq_no);
		state = 1;
		break;
	case 4:
		SsSeqSetVol((short)(signed char)Seq_ctr[ch].Seq_no, 0, 0);
		SsSeqPause((short)(signed char)Seq_ctr[ch].Seq_no);
		state = 4;
		break;
	case 5:
		SsSndCrescendo((short)(signed char)Seq_ctr[ch].Seq_no, 0x7f, 0x5a);
		state = 50;
	}
	Seq_ctr[ch].Flg = state;
bgm_ctr_default:
seq_ctrl_done:
	vol_val = (char)(code >> 8) + -1;
	if ((code >> 8 & 0xff) != 0) {
		ch = code >> 0x10 & 0xff;
		if (ch == 0) {
			((SND_VAB_HDR *)pVh)->Mvol = vol_val;
		} else {
			((SND_VAB_HDR *)pVh)->Prog[ch - 1].Mvol = vol_val;
		}
	}
	ch = code >> 0x10 & 0xff;
	if ((code & 0xff) != 0) {
		if (ch == 0) {
			((SND_VAB_HDR *)pVh)->Pan = (char)code + -1;
		} else {
			((SND_VAB_HDR *)pVh)->Prog[ch - 1].Pan = (char)code + -1;
		}
	}
	return;
}

/* DS 0x8005b9f4 (Snd_bgm_tbl_set) */
void Snd_bgm_tbl_set(ULONG code)
{
	*(short *)&Bgm_room_tbl[(UCHAR)PANVT[(code >> 0x18) + 0x7f] + (code >> 0x10 & 0xff)] = (short)code;
	return;
}

/* Sound-effect request record: 0x20 bytes, one per SE channel. sel_no (the old global.h, 0x800d4f18) holds
 * 24 of them; channel ch_idx is at sel_no + ch_idx * 0x20. Snd_se_call (DS 0x8005c5e4) reads them through
 * a base at record +0xC (retail s0): active flag lbu 8005c648 (+0), position addiu 8005c66c (+0x14),
 * flg_3D lh 8005c658 (+0x12), pan lhu 8005c6cc and 8005c6d8 (+0xE, +0x10), vab_type lw 8005c670 (+4),
 * key-on args lh 8005c768..8005c774 and lh 8005c764 (+4, +6, +8, +0xA, +0xC), se_no lbu 8005c7a8 (+1).
 * Snd_se_stad (DS 0x8005ba28) fills the same record: vab_type, Pno and note are its locals. */
typedef struct {
	char   active;      /* +0x00 channel active flag */
	char   se_no;       /* +0x01 SE number */
	UCHAR  Unk_02[2];   /* +0x02 not read by Snd_se_call */
	SHORT  vab_type;    /* +0x04 vab type (read as int, short and USHORT views) */
	SHORT  Pno;         /* +0x06 program number, SsUtKeyOn arg 2 */
	SHORT  Tno;         /* +0x08 tone number, SsUtKeyOn arg 3 */
	SHORT  note;        /* +0x0A note, SsUtKeyOn arg 4 */
	SHORT  Unk_0c;      /* +0x0C fine tune, SsUtKeyOn arg 5 (no project name) */
	USHORT pan_left;    /* +0x0E pan volume left */
	USHORT pan_right;   /* +0x10 pan volume right */
	SHORT  flg_3D;      /* +0x12 0 = use pan, else Snd_se_3D flag */
	LONG   Pos_x;       /* +0x14 position x for Snd_se_3D */
	LONG   Pos_y;       /* +0x18 position y */
	LONG   Pos_z;       /* +0x1c position z */
} SND_SEL_REC;

/* DS 0x8005ba28 (Snd_se_stad) */
void Snd_se_stad(ULONG bits, VECTOR *pVec)
{
	UCHAR pri_ctl;
	UCHAR vab_flg;
	UCHAR rv_byte;
	UCHAR pri_ok;
	int vab_type;
	UINT bank;
	UINT SENo_b;
	UINT vch;
	UINT be_flg;
	UINT extra_cnt;
	UINT Pno;
	UINT note;
	UINT ofs_a;
	UINT k;
	UINT ch_k;
	ULONG *pVh;
	EDT_TABLE_WORK *pEdt_ent;
	VagAtr *pVag;
	SND_SEL_REC *pRec;
	ULONG   Frame_pad[4];		/* retail reserves 0x14-0x17, 0x1a-0x27 and never reads them */

	(void)Frame_pad;
	bank = bits >> 0x18;
	vab_type = (signed char)Vab_id[bank];
	if (vab_type == -1) {
		return;
	}
	SENo_b = bits >> 0x10 & 0xff;
	pEdt_ent = (EDT_TABLE_WORK *)EDT_SLOT_PTR(bank + 2) + SENo_b;
	note = bits & 0xff;
	if (*(int *)pEdt_ent == -1) {
		return;
	}
	if ((pEdt_ent->id_check & 0x80) != 0) {
		vab_type = pEdt_ent->id_check & 0x7f;
	}
	pVh = pVh_tbl[bank];
	pri_ctl = pEdt_ent->tone_pri;
	vch = pEdt_ent->monopoly & 0x1f;
	be_flg = (UINT)(pri_ctl >> 4);
	extra_cnt = (UINT)(pEdt_ent->monopoly >> 5);
	Pno = pEdt_ent->pan_prog & 0x7f;
	ofs_a = Pno * 0x200 + be_flg * 0x20 + 0x820;
	if ((vab_type == 4) && (3 < SENo_b)) {
se_rv_clear:
		rv_byte = ((VagAtr *)((int)pVh + ofs_a))->mode & 0xfb;
	} else {
		if (vab_type == 2) {
			rv_byte = ((VagAtr *)((int)pVh + ofs_a))->mode;
			if (((rv_byte & 4) != 0) && (Reverb_flg != 0)) {
				((VagAtr *)((int)pVh + ofs_a))->mode = rv_byte | Reverb_flg;
				goto se_rv_done;
			}
			goto se_rv_clear;
		}
		if ((Reverb_flg == 0) || (vab_type == 0)) goto se_rv_clear;
		rv_byte = ((VagAtr *)((int)pVh + ofs_a))->mode | Reverb_flg;
	}
	((VagAtr *)((int)pVh + ofs_a))->mode = rv_byte;
se_rv_done:
	vab_flg = ((VagAtr *)((int)pVh + ofs_a))->mode;
	pri_ok = Snd_se_pri_ck((UCHAR)vch, pri_ctl & 0xf);
	if (pri_ok != 0) {
		return;
	}

	/* First channel (monopoly slot vch), then extra_cnt following channels
	   that each take the next be_flg slot. DS writes the same record for
	   both; the extra channels also write the tone mode byte. */
	ch_k = vch;
	ofs_a = Pno * 0x200 + be_flg * 0x20 + 0x820;
	pVag = (VagAtr *)((int)pVh + ofs_a);
	Se_pri[ch_k].Pri = pri_ctl & 7;
	Se_pri[ch_k].Vch = (UCHAR)ch_k;
	pRec = (SND_SEL_REC *)sel_no + ch_k;
	pRec->active = 1;
	pRec->se_no = (char)SENo_b;
	pRec->vab_type = (SHORT)vab_type;
	pRec->Pno = (SHORT)Pno;
	pRec->Tno = (SHORT)be_flg;
	pRec->note = (SHORT)pVag->min;
	pRec->Unk_0c = (SHORT)pVag->shift;
	pRec->pan_left = pVag->vol;
	pRec->pan_right = pVag->vol;
	pRec->flg_3D = (SHORT)note;
	pRec->Pos_x = pVec->vx;
	pRec->Pos_y = pVec->vy;
	pRec->Pos_z = pVec->vz;
	if (extra_cnt != 0) {
		k = 0;
		do {
			k++;
			ch_k = vch + k;
			ofs_a = Pno * 0x200 + (be_flg + k) * 0x20 + 0x820;
			pVag = (VagAtr *)((int)pVh + ofs_a);
			pVag->mode = vab_flg;
			Se_pri[ch_k].Pri = pri_ctl & 7;
			Se_pri[ch_k].Vch = (UCHAR)ch_k;
			pRec = (SND_SEL_REC *)sel_no + ch_k;
			pRec->active = 1;
			pRec->se_no = (char)SENo_b;
			pRec->vab_type = (SHORT)vab_type;
			pRec->Pno = (SHORT)Pno;
			pRec->Tno = (SHORT)(be_flg + k);
			pRec->note = (SHORT)pVag->min;
			pRec->Unk_0c = (SHORT)pVag->shift;
			pRec->pan_left = pVag->vol;
			pRec->pan_right = pVag->vol;
			pRec->flg_3D = (SHORT)note;
			pRec->Pos_x = pVec->vx;
			pRec->Pos_y = pVec->vy;
			pRec->Pos_z = pVec->vz;
		} while (k < extra_cnt);
	}
	return;
}

/* DS 0x8005bd6c (Snd_se_enem) */
void Snd_se_enem(UCHAR SENo, ENEMY_WORK *pEm)
{
	EDT_TABLE_WORK *pEdt_ent;
	ULONG *pVh;
	UCHAR *pVag;
	UCHAR *pRec;
	UCHAR rv_byte;
	UINT vab_type;
	UINT vch;
	UINT Pno;
	UINT be_flg;
	UINT extra_cnt;
	UINT pri_ctl;
	int ofs_a;
	int ofs_v;

	if ((pEm->Be_flg & 0x2000) != 0) {
		SENo += 0x10;
	}
	pEdt_ent = (EDT_TABLE_WORK *)EDT_SLOT_PTR(5) + SENo;
	vab_type = (UINT)(signed char)Vab_id[3];
	if ((*(int *)pEdt_ent != -1) && (vab_type != 0xffffffff)) {
		if ((pEdt_ent->id_check & 0x80) != 0) {
			vab_type = pEdt_ent->id_check & 0x7f;
		}
		vch = pEdt_ent->monopoly & 0x1f;
		pri_ctl = pEdt_ent->tone_pri & 0xf;
		if (Snd_se_pri_ck((UCHAR)vch, pri_ctl) == 0) {
			/* DS addresses the 0x20-byte request record through one pointer (sel_no + vch * 0x20) */
			Pno = pEdt_ent->pan_prog & 0x7f;
			be_flg = pEdt_ent->tone_pri >> 4;
			extra_cnt = pEdt_ent->monopoly >> 5;
			ofs_v = vch * 2;
			ofs_a = Pno * 0x200 + be_flg * 0x20 + 0x820;
			pVag = (UCHAR *)pVh_tbl[3] + ofs_a;
			if (Reverb_flg != 0) {
				rv_byte = pVag[1] | Reverb_flg;
			} else {
				rv_byte = pVag[1] & 0xfb;
			}
			pVag[1] = rv_byte;
			pRec = sel_no + vch * 0x20;
			(&Se_pri[0].Pri)[ofs_v] = pri_ctl & 7;
			(&Se_pri[0].Vch)[ofs_v] = (UCHAR)vch;
			pRec[1] = SENo;
			pRec[0] = 1;
			*(USHORT *)(pRec + 4) = (USHORT)vab_type;
			*(USHORT *)(pRec + 6) = (USHORT)Pno;
			*(USHORT *)(pRec + 8) = (USHORT)be_flg;
			*(USHORT *)(pRec + 0xa) = (USHORT)pVag[6];
			*(USHORT *)(pRec + 0xc) = (USHORT)pVag[5];
			*(USHORT *)(pRec + 0xe) = (USHORT)pVag[2];
			*(USHORT *)(pRec + 0x10) = (USHORT)pVag[2];
			*(long *)(pRec + 0x14) = pEm->Pos_x;
			*(long *)(pRec + 0x18) = pEm->Pos_y;
			*(USHORT *)(pRec + 0x12) = 1;
			*(long *)(pRec + 0x1c) = pEm->Pos_z;
			if (extra_cnt != 0) {
				ofs_a = be_flg * 0x20 + 0x820;
				do {
					extra_cnt--;
					vch++;
					ofs_v += 2;
					ofs_a += 0x20;
					be_flg++;
					pVag = (UCHAR *)pVh_tbl[3] + (Pno * 0x200 + ofs_a);
					if (Reverb_flg != 0) {
						rv_byte = pVag[1] | Reverb_flg;
					} else {
						rv_byte = pVag[1] & 0xfb;
					}
					pVag[1] = rv_byte;
					pRec = sel_no + vch * 0x20;
					(&Se_pri[0].Pri)[ofs_v] = pri_ctl & 7;
					(&Se_pri[0].Vch)[ofs_v] = (UCHAR)vch;
					pRec[1] = SENo;
					pRec[0] = 1;
					*(USHORT *)(pRec + 4) = (USHORT)vab_type;
					*(USHORT *)(pRec + 6) = (USHORT)Pno;
					*(USHORT *)(pRec + 8) = (USHORT)be_flg;
					*(USHORT *)(pRec + 0xa) = (USHORT)pVag[6];
					*(USHORT *)(pRec + 0xc) = (USHORT)pVag[5];
					*(USHORT *)(pRec + 0xe) = (USHORT)pVag[2];
					*(USHORT *)(pRec + 0x10) = (USHORT)pVag[2];
					*(long *)(pRec + 0x14) = pEm->Pos_x;
					*(long *)(pRec + 0x18) = pEm->Pos_y;
					*(USHORT *)(pRec + 0x12) = 1;
					*(long *)(pRec + 0x1c) = pEm->Pos_z;
				} while (extra_cnt != 0);
			}
		}
	}
}

/* DS 0x8005c040 (Snd_se_walk) */
void Snd_se_walk(USHORT no, USHORT lr, ENEMY_WORK *pEm)
{
	UCHAR pri_ctl;
	EDT_TABLE_WORK *pEdt_tbl;
	UCHAR *pVag;
	UCHAR *pRec;
	UCHAR pri_ok;
	UCHAR rv_byte;
	int ofs_a;
	long pos_z;
	ULONG esp_bit;
	UINT vab_pno;
	UCHAR Sv_pri;
	int ofs_v;
	MATRIX *pMat;
	PARTS_W *pParts;
	EDT_TABLE_WORK *pEdt_sel;
	EDT_TABLE_WORK *pEdt_ent;
	USHORT sub_no;
	UINT ch;
	int vab_id_w;
	SVECTOR stsvec;
	USHORT se_no;
	ULONG *pVh_cur;
	ULONG Frame_pad[6];		/* retail reserves 0x58 bytes; its stack reads are sparse and sit below sp+0x20 or above sp+0x30 */

	(void)Frame_pad;
	if (pEm->Id == 13) {
		vab_id_w = (int)(signed char)Vab_id[4];
		se_no = 0xb;
		ch = (UINT)(lr != 4);
		pVh_cur = pVh_tbl[4];
		pEdt_sel = (EDT_TABLE_WORK *)UnkSndFlg0;
	} else {
		se_no = Floor_check(pEm->Pos_x, pEm->Pos_z, (UINT)pEm->nFloor);
		vab_id_w = (int)(signed char)Vab_id[2];
		pVh_cur = pVh_tbl[2];
		pEdt_sel = (EDT_TABLE_WORK *)EDT_SLOT_PTR(4);
		if (pEm->Id < 0x10) {
			ch = (UINT)(lr != 4);
		} else {
			ch = 3;
			if (lr == 4) {
				ch = 2;
			}
		}
	}
	pEdt_tbl = (EDT_TABLE_WORK *)EDT_SLOT_PTR(4);
	if (pEm->Water != 0) {
		se_no = 0x1a;
		pParts = pEm->pSin_parts_ptr;
		vab_id_w = (int)(signed char)Vab_id[2];
		pVh_cur = pVh_tbl[2];
		stsvec.vy = pEm->Water - (short)pParts[1].Workm.t[1];
		stsvec.vz = 0xa0;
		if (lr == 4) {
			stsvec.vz = -0xa0;
		}
		pEdt_sel = pEdt_tbl;
		if (pEm->Id == 0xf) {
			if (stsvec.vy < 500) {
				if (no == 0) {
					Pad_vib_fade_set(1, 0x82, 100, 0);
					Pad_vib_fade_set(9, 0x55, 10, 1);
				}
				if (no == 1) {
					Pad_vib_fade_set(3, 0x32, 100, 0);
					Pad_vib_fade_set(5, 0x55, 0, 3);
				}
			}
			stsvec.vy += 100;
			if (no == 0) {
				pMat = &pParts[1].Workm;
				stsvec.vx = 100;
				Esp_call(0x1a000e00, G.Player.Cdir_y, pMat, &stsvec);
				stsvec.vx = 0;
				Esp_call(0x1a000c00, G.Player.Cdir_y + 0x400, pMat, &stsvec);
				Esp_call(0x1a000c00, G.Player.Cdir_y + 0xc00, pMat, &stsvec);
			}
			if (no != 1) goto se_esp_done;
			stsvec.vx = 300;
			Esp_call(0x1a001200, G.Player.Cdir_y, &pParts[1].Workm, &stsvec);
			stsvec.vx = 0;
			Esp_call(0x1a000e00, G.Player.Cdir_y + 0x400, &pParts[1].Workm, &stsvec);
			esp_bit = 0x1a000e00;
		} else {
			if ((stsvec.vy < 0x4b0) && (pEm->Id < 0x10)) {
				if (no == 0) {
					Pad_vib_fade_set(1, 0x82, 100, 0);
					Pad_vib_fade_set(9, 0x55, 10, 1);
				}
				if (no == 1) {
					Pad_vib_fade_set(3, 0x32, 100, 0);
					Pad_vib_fade_set(5, 0x55, 10, 3);
				}
			}
			if (no == 0) {
				pMat = &pParts[1].Workm;
				stsvec.vx = 200;
				Esp_call(0x1a000e00, G.Player.Cdir_y, pMat, &stsvec);
				stsvec.vx = 0;
				Esp_call(0x1a000c00, G.Player.Cdir_y + 0x400, pMat, &stsvec);
				Esp_call(0x1a000c00, G.Player.Cdir_y + 0xc00, pMat, &stsvec);
			}
			if (no != 1) goto se_esp_done;
			stsvec.vx = 600;
			Esp_call(0x1a001600, G.Player.Cdir_y, &pParts[1].Workm, &stsvec);
			stsvec.vx = 0;
			Esp_call(0x1a001100, G.Player.Cdir_y + 0x400, &pParts[1].Workm, &stsvec);
			esp_bit = 0x1a001100;
		}
		Esp_call(esp_bit, G.Player.Cdir_y + 0xc00, &pParts[1].Workm, &stsvec);
	}
se_esp_done:
	if (pEm->pOn_om == (OBJ_MODEL_WORK *)0x0) {
		sub_no = no + (se_no & 0x7f);
	} else {
		sub_no = 6;
	}
	pEdt_ent = pEdt_sel + sub_no;
	if ((*(int *)pEdt_ent != -1) && (vab_id_w != -1)) {
		Sv_pri = (pEdt_ent->tone_pri & 0xf) - 1;
		pri_ok = Snd_se_pri_ck((UCHAR)ch, Sv_pri);
		if (pri_ok != 0) {
			ch = 0x17;
		}
		vab_pno = pEdt_ent->pan_prog & 0x7f;
		pri_ctl = pEdt_ent->tone_pri >> 4;
		ofs_a = vab_pno * 0x200 + (UINT)pri_ctl * 0x20 + 0x820;
		/* DS addresses the 0x20-byte request record through one pointer (sel_no + ch * 0x20) */
		pVag = (UCHAR *)pVh_cur + ofs_a;
		if (Reverb_flg == 0) {
			rv_byte = pVag[1] & 0xfb;
		} else {
			rv_byte = pVag[1] | Reverb_flg;
		}
		pVag[1] = rv_byte;
		ofs_v = ch * 2;
		pRec = sel_no + ch * 0x20;
		(&Se_pri[0].Pri)[ofs_v] = Sv_pri & 7;
		(&Se_pri[0].Vch)[ofs_v] = (UCHAR)ch;
		pRec[1] = (UCHAR)sub_no;
		pRec[0] = 1;
		*(USHORT *)(pRec + 4) = (USHORT)vab_id_w;
		*(USHORT *)(pRec + 6) = (USHORT)vab_pno;
		*(USHORT *)(pRec + 8) = (USHORT)pri_ctl;
		/* sub-variant: SE table entry + (Rnd() % 2) (DS signed remainder sequence) */
		*(USHORT *)(pRec + 0xa) = (USHORT)(pVag[6] + Rnd() % 2);
		*(USHORT *)(pRec + 0xc) = (USHORT)pVag[5];
		*(USHORT *)(pRec + 0xe) = (USHORT)pVag[2];
		*(USHORT *)(pRec + 0x10) = (USHORT)pVag[2];
		*(long *)(pRec + 0x14) = pEm->Pos_x;
		*(long *)(pRec + 0x18) = pEm->Pos_y;
		*(USHORT *)(pRec + 0x12) = 1;
		*(long *)(pRec + 0x1c) = pEm->Pos_z;
	}
	return;
}



/* DS 0x8005c5e4 (Snd_se_call) */
void Snd_se_call(void)
{
	char cnt_c;
	int vol_l_s;
	int vol_r_s;
	long key_st;
	UINT vol_scale;
	SND_SEL_REC *pRec;
	char *pFlag;
	UINT ch_idx;
	USHORT vol_l16;
	USHORT vol_r16[11];
	ULONG Frame_pad[16];		/* retail reserves 0x98 bytes; only sp+0x10..0x1c and sp+0x60..0x62 are used */

	(void)Frame_pad;
	/* sel_no: 24 SND_SEL_REC records; record ch_idx is at sel_no + ch_idx * 0x20 */
	ch_idx = 0x18;
	do {
		ch_idx--;
		pRec = (SND_SEL_REC *)sel_no + ch_idx;
		pFlag = (char *)pRec;
		if (*pFlag != 0) {
			if (pRec->flg_3D == 0) {
				Pan_vol.left = pRec->pan_left;
				Pan_vol.right = pRec->pan_right;
			} else {
				Snd_se_3D((VECTOR *)&pRec->Pos_x, pRec->flg_3D);
				if (*(int *)&pRec->vab_type == 1) {
					Pan_vol.left += 10;
					if (0x7f < Pan_vol.left) {
						Pan_vol.left = 0x7f;
					}
					Pan_vol.right += 10;
					if (0x7f < Pan_vol.right) {
						Pan_vol.right = 0x7f;
					}
				}
			}
			vol_scale = (UINT)Volume_Se;
			if (vol_scale == 0) {
se_flag_clear:
				*pFlag = 0;
			} else {
				Pan_vol.left = (USHORT)((Pan_vol.left * vol_scale) / 100);
				Pan_vol.right = (USHORT)((Pan_vol.right * vol_scale) / 100);
				if (Se_pri[ch_idx].Vch < 0x10) {
					SsUtKeyOn(*(USHORT *)&pRec->vab_type, pRec->Pno, pRec->Tno,
							pRec->note, pRec->Unk_0c, Pan_vol.left, Pan_vol.right);
					*pFlag = 0;
				} else {
					if ((pRec->vab_type != 2) || (pRec->se_no != 0)) {
						SsUtKeyOnV((USHORT)Se_pri[ch_idx].Vch,
									 *(USHORT *)&pRec->vab_type, pRec->Pno, pRec->Tno,
									 pRec->note, pRec->Unk_0c, Pan_vol.left, Pan_vol.right);
						goto se_flag_clear;
					}
					SsUtGetVVol((USHORT)Se_pri[ch_idx].Vch, &vol_l16, vol_r16);
					vol_l_s = ((short)vol_l16 * 0x5a) / 100;
					vol_l16 = (USHORT)vol_l_s;
					vol_r_s = ((short)vol_r16[0] * 0x5a) / 100;
					vol_r16[0] = (USHORT)vol_r_s;
					SsUtSetVVol((USHORT)Se_pri[ch_idx].Vch,
								 (short)((UINT)(vol_l_s * 0x10000) >> 0x10),
								 (short)((UINT)(vol_r_s * 0x10000) >> 0x10));
					cnt_c = *pFlag;
					*pFlag = cnt_c + 1;
					if ((char)(cnt_c + 1) == 16) {
						*pFlag = 0;
					}
					Se_pri[ch_idx].Pri = 0;
				}
			}
		}
		if (((ch_idx < 0x16) || (key_st = SpuGetKeyStatus(1 << (ch_idx & 0x1f)), key_st == 3)) ||
			(key_st == 0)) {
			Se_pri[ch_idx].Pri = 0;
		}
		if (ch_idx == 0) {
			Snd_bgm_fade();
			return;
		}
	} while (1);
}

/* DS 0x8005c92c (Snd_se_pri_ck) */
UCHAR Snd_se_pri_ck(UCHAR Sv_ch, UCHAR Sv_pri)
{
	BOOL preempt;

	if ((Sv_pri & 7) < Se_pri[Sv_ch].Pri) {
		preempt = 1;
	} else {
		preempt = 0;
		if (Se_pri[Sv_ch].Pri == (Sv_pri & 7)) {
			preempt = (Sv_pri & 8) != 0;
		}
	}
	return preempt;
}

/* DS 0x8005c970 (Snd_se_3D) */
void Snd_se_3D(VECTOR *pVec, int flg_3D)
{
	BOOL right_side;
	UCHAR vol_idx;
	USHORT sedir_f;
	short d_p;
	short d_r;
	short cut_no;
	short ang_pv;
	short ang_rv;
	short ang_pz;
	short ang_rz;
	long dist_sq;
	long dist_tmp;
	int idx;
	ULONG occl;
	USHORT sedir_u;
	RCUT *pView;
	VECTOR ray[2];

	cut_no = G.SaveData.Cut_no;
	pView = (G.pRoom)->pRcut;
	ray[0].vx = pView[G.SaveData.Cut_no].View_p[0] - pVec->vx;
	if (ray[0].vx < 0) {
		ray[0].vx = -ray[0].vx;
	}
	ray[0].vy = pView[G.SaveData.Cut_no].View_p[1] - pVec->vy;
	if (ray[0].vy < 0) {
		ray[0].vy = -ray[0].vy;
	}
	ray[0].vz = pView[G.SaveData.Cut_no].View_p[2] - pVec->vz;
	if (ray[0].vz < 0) {
		ray[0].vz = -ray[0].vz;
	}
	dist_sq = SquareRoot0(ray[0].vx * ray[0].vx + ray[0].vz * ray[0].vz);
	dist_tmp = SquareRoot0(ray[0].vx * ray[0].vx + ray[0].vz * ray[0].vz);
	dist_sq = SquareRoot0(ray[0].vy * ray[0].vy + dist_sq * dist_tmp);
	Snd_se_dist_idx = dist_sq / 0xfa;
	if (0x7f < Snd_se_dist_idx) {
		Snd_se_dist_idx = 0x7f;
	}
	ang_pv = Snd_se_dir_ck(pView[cut_no].View_p[0], pView[cut_no].View_p[2],
						   pVec->vx, pVec->vz);
	ang_rv = Snd_se_dir_ck(pView[cut_no].View_p[0], pView[cut_no].View_p[2],
						   pView[cut_no].View_r[0], pView[cut_no].View_r[2]);
	ang_pz = Snd_se_dir_ck(0, 0, pView[cut_no].View_p[0], pView[cut_no].View_p[2]);
	ang_rz = Snd_se_dir_ck(0, 0, pView[cut_no].View_p[0], pView[cut_no].View_p[2]);
	d_p = (short)((int)ang_pv - (int)ang_pz);
	d_r = (short)((int)ang_rv - (int)ang_rz);
	if ((int)ang_pv - (int)ang_pz < (int)ang_rv - (int)ang_rz) {
		Sedir = (d_p + 0x1000) - d_r;
	} else {
		Sedir = d_p - d_r;
	}
	if (((Sedir == 0) || (Sedir == 0x1000)) || (Sedir == 0x800)) {
		Pan_vol.left = 0x7f;
		Pan_vol.right = 0x7f;
	} else {
		right_side = (Sedir & 0x800) == 0;
		sedir_u = Sedir & 0x7ff;
		sedir_f = Sedir & 0x400;
		Sedir = sedir_u;
		if (sedir_f != 0) {
			Sedir = 0x800 - sedir_u;
		}
		idx = (int)(short)Sedir;
		if (idx < 0) {
			idx += 7;
		}
		Sedir = (USHORT)(idx >> 3);
		Pan_vol.right = (USHORT)!right_side * (USHORT)LRVT[idx >> 3] + (USHORT)right_side * 0x7f;
		Pan_vol.left = (USHORT)!right_side * 0x7f + (USHORT)right_side * (USHORT)LRVT[idx >> 3];
	}
	Sedir2[0] = Sedir;
	vol_idx = PANVT[Snd_se_dist_idx];
	if (vol_idx < Pan_vol.left) {
		Pan_vol.left -= vol_idx;
	}
	vol_idx = PANVT[Snd_se_dist_idx];
	if (vol_idx < Pan_vol.right) {
		Pan_vol.right -= vol_idx;
	}
	ray[0].vx = pVec->vx;
	ray[0].vy = pVec->vy + -0x5dc;
	ray[0].vz = pVec->vz;
	if ((flg_3D != 2) &&
		(occl = Sca_ck_line((VECTOR *)pView[cut_no].View_p, ray, 0x8400, 1),
		 occl != 0)) {
		Pan_vol.left = (USHORT)(((UINT)Pan_vol.left * 0x41) / 100);
		Pan_vol.right = (USHORT)(((UINT)Pan_vol.right * 0x41) / 100);
	}
	if (0x7f < Pan_vol.left) {
		Pan_vol.left = 0x7f;
	}
	if (0x7f < Pan_vol.right) {
		Pan_vol.right = 0x7f;
	}
	return;
}

/* DS 0x8005cda4 (Snd_se_dir_ck) */
short Snd_se_dir_ck(long B_x, long B_z, long T_x, long T_z)
{
	short base_ang;
	USHORT ang;
	int slope;
	int dx;

	dx = T_x - B_x;
	if (dx == 0) {
		ang = 0x400;
		if (0 < T_z - B_z) {
			ang = 0xc00;
		}
	} else {
		slope = (T_z - B_z) * 0x1000;
		DIV_S(slope, slope, dx);	/* DS: div with zero and overflow break checks */
		slope = catan(slope);
		base_ang = 0;
		if (dx < 0) {
			base_ang = 0x800;
		}
		ang = (base_ang - (short)slope) & 0xfff;
	}
	return ang;
}

/* DS 0x8005ce30 (Snd_bgm_pause) */
void Snd_bgm_pause(int Resume)
{
	int i;
	USHORT Vol;

	for (i = 2; i >= 0; i--) {
		if (Resume == 0) {
			if ((Seq_ctr[i].Flg == 1) && (SsIsEos((short)(signed char)Seq_ctr[i].Seq_no, 0) != 0)) {
				SsSeqSetVol((short)(signed char)Seq_ctr[i].Seq_no, 0, 0);
				SsSeqPause((short)(signed char)Seq_ctr[i].Seq_no);
				Seq_ctr[i].Flg = 5;
			}
		}
		else if (Seq_ctr[i].Flg == 5) {
			Vol = (USHORT)(((UINT)Seq_ctr[i].Vol.left * (UINT)Volume_Bgm) / 100);
			SsSeqSetVol((short)(signed char)Seq_ctr[i].Seq_no, Vol, Vol);
			SsSeqReplay((short)(signed char)Seq_ctr[i].Seq_no);
			Seq_ctr[i].Flg = 1;
		}
	}
	return;
}

/* DS 0x8005cf70 (Snd_cd_watchdog) */
void Snd_cd_watchdog(void)
{
	Cd_watchdog_cnt++;
	if (100 < Cd_watchdog_cnt) {
		EnterCriticalSection();
		DeliverEvent(0xf0000009, 0x20);
		ExitCriticalSection();
	}
	return;
}

/* SDK 0x8007975c (no port name): reads the voice attributes that Snd_key_off_all tests. The caller passes the voice bit in word 0 and reads the start address in word 7; SpuVoiceAttr (LIBSPU.H) has the same layout. */
void Spu_voice_info_get(ULONG *pInfo)
{
	SpuGetVoiceAttr((SpuVoiceAttr *)pInfo);
}

/* DS 0x800a7b1c (EnemSE_floc) */
tagfpos EnemSE_floc[146] = {
	{ 0x00000ca8, 0x0000, 0x00, 0x00 },
	{ 0x00011fe0, 0x0002, 0x00, 0x09 },
	{ 0x00000ca8, 0x0026, 0x00, 0x00 },
	{ 0x000128e0, 0x0028, 0x00, 0x36 },
	{ 0x00000ca8, 0x004e, 0x00, 0x00 },
	{ 0x00012810, 0x0050, 0x00, 0x1b },
	{ 0x00000ca8, 0x0076, 0x00, 0x00 },
	{ 0x00012ed0, 0x0078, 0x00, 0x5f },
	{ 0x00000ca8, 0x009e, 0x00, 0x00 },
	{ 0x00011b90, 0x00a0, 0x00, 0x75 },
	{ 0x00000ca8, 0x00c4, 0x00, 0x00 },
	{ 0x000124c0, 0x00c6, 0x00, 0x7a },
	{ 0x00000ca8, 0x00eb, 0x00, 0x00 },
	{ 0x0000f8f0, 0x00ed, 0x00, 0x7e },
	{ 0x00000ca8, 0x010d, 0x00, 0x00 },
	{ 0x0000bcf0, 0x010f, 0x00, 0x38 },
	{ 0x00000ca8, 0x0127, 0x00, 0x00 },
	{ 0x00015150, 0x0129, 0x00, 0x26 },
	{ 0x00000ca8, 0x0154, 0x00, 0x00 },
	{ 0x00015150, 0x0156, 0x00, 0x26 },
	{ 0x00000ca8, 0x0181, 0x00, 0x00 },
	{ 0x00007e50, 0x0183, 0x00, 0x5e },
	{ 0x00000ca8, 0x0193, 0x00, 0x00 },
	{ 0x000096d0, 0x0195, 0x00, 0x77 },
	{ 0x00000ca8, 0x01a8, 0x00, 0x00 },
	{ 0x00006880, 0x01aa, 0x00, 0x20 },
	{ 0x00000ca8, 0x01b8, 0x00, 0x00 },
	{ 0x000180e0, 0x01ba, 0x00, 0x56 },
	{ 0x00000ca8, 0x01eb, 0x00, 0x00 },
	{ 0x0000f0c0, 0x01ed, 0x00, 0x24 },
	{ 0x00000ca8, 0x020c, 0x00, 0x00 },
	{ 0x00016400, 0x020e, 0x00, 0x3e },
	{ 0x00000ea8, 0x023b, 0x00, 0x00 },
	{ 0x000094a0, 0x023d, 0x00, 0x53 },
	{ 0x00000ca8, 0x0250, 0x00, 0x00 },
	{ 0x0000f430, 0x0252, 0x00, 0x12 },
	{ 0x00000ca8, 0x0271, 0x00, 0x00 },
	{ 0x000102c0, 0x0273, 0x00, 0x1c },
	{ 0x00000ca8, 0x0294, 0x00, 0x00 },
	{ 0x0001ecc0, 0x0296, 0x00, 0x5c },
	{ 0x00000ca8, 0x02d4, 0x00, 0x00 },
	{ 0x0001dd20, 0x02d6, 0x00, 0x1a },
	{ 0x00000ca8, 0x0312, 0x00, 0x00 },
	{ 0x0001fe00, 0x0314, 0x00, 0x72 },
	{ 0x00000ca8, 0x0354, 0x00, 0x00 },
	{ 0x0000da70, 0x0356, 0x00, 0x10 },
	{ 0x00000ea8, 0x0372, 0x00, 0x00 },
	{ 0x0001d8b0, 0x0374, 0x00, 0x1f },
	{ 0x00000ea8, 0x03b0, 0x00, 0x00 },
	{ 0x00019530, 0x03b2, 0x00, 0x25 },
	{ 0x00000ea8, 0x03e5, 0x00, 0x00 },
	{ 0x00017a10, 0x03e7, 0x00, 0x2a },
	{ 0x00000ca8, 0x0417, 0x00, 0x00 },
	{ 0x0000d950, 0x0419, 0x00, 0x63 },
	{ 0x00000ca8, 0x0435, 0x00, 0x00 },
	{ 0x000014a0, 0x0437, 0x00, 0x2c },
	{ 0x00000ea8, 0x043a, 0x00, 0xff },
	{ 0x00005e50, 0x043c, 0x00, 0x3b },
	{ 0x00000ea8, 0x0448, 0x00, 0xff },
	{ 0x00005e30, 0x044a, 0x00, 0x01 },
	{ 0x00000ea8, 0x0456, 0x00, 0xff },
	{ 0x00007100, 0x0458, 0x00, 0x04 },
	{ 0x00000ea8, 0x0467, 0x00, 0xff },
	{ 0x00006130, 0x0469, 0x00, 0x22 },
	{ 0x00000ea8, 0x0476, 0x00, 0x00 },
	{ 0x0001d1a0, 0x0478, 0x00, 0x28 },
	{ 0x00000ea8, 0x04b3, 0x00, 0x00 },
	{ 0x0001c770, 0x04b5, 0x00, 0x67 },
	{ 0x00000ea8, 0x04ee, 0x00, 0x00 },
	{ 0x0001c770, 0x04f0, 0x00, 0x67 },
	{ 0x00000ea8, 0x0529, 0x00, 0x00 },
	{ 0x00019d50, 0x052b, 0x00, 0x32 },
	{ 0x00000ea8, 0x055f, 0x00, 0x00 },
	{ 0x0001d890, 0x0561, 0x00, 0x66 },
	{ 0x00000ea8, 0x059d, 0x00, 0x00 },
	{ 0x000191a0, 0x059f, 0x00, 0x1f },
	{ 0x00000ea8, 0x05d2, 0x00, 0x00 },
	{ 0x000180e0, 0x05d4, 0x00, 0x3b },
	{ 0x00000ea8, 0x0605, 0x00, 0x00 },
	{ 0x0001c1b0, 0x0607, 0x00, 0x4e },
	{ 0x00000ea8, 0x0640, 0x00, 0x00 },
	{ 0x0001c770, 0x0642, 0x00, 0x67 },
	{ 0x00000ea8, 0x067b, 0x00, 0x00 },
	{ 0x0001dd80, 0x067d, 0x00, 0x0a },
	{ 0x00000ea8, 0x06b9, 0x00, 0x00 },
	{ 0x0001dfb0, 0x06bb, 0x00, 0x2a },
	{ 0x00000ea8, 0x06f7, 0x00, 0x00 },
	{ 0x00019aa0, 0x06f9, 0x00, 0x0b },
	{ 0x00000ea8, 0x072d, 0x00, 0x00 },
	{ 0x000189e0, 0x072f, 0x00, 0x3e },
	{ 0x00000ea8, 0x0761, 0x00, 0x00 },
	{ 0x0001c770, 0x0763, 0x00, 0x67 },
	{ 0x00000ea8, 0x079c, 0x00, 0x00 },
	{ 0x00013850, 0x079e, 0x00, 0x20 },
	{ 0x00000ca8, 0x07c6, 0x00, 0x00 },
	{ 0x00012810, 0x07c8, 0x00, 0x1b },
	{ 0x00000ea8, 0x07ee, 0x00, 0x00 },
	{ 0x0001beb0, 0x07f0, 0x00, 0x64 },
	{ 0x00000ea8, 0x0828, 0x00, 0x00 },
	{ 0x0001d1a0, 0x082a, 0x00, 0x28 },
	{ 0x00000ea8, 0x0865, 0x00, 0x00 },
	{ 0x0001d1a0, 0x0867, 0x00, 0x28 },
	{ 0x00000ea8, 0x08a2, 0x00, 0x00 },
	{ 0x0001d2d0, 0x08a4, 0x00, 0x7e },
	{ 0x00000ea8, 0x08df, 0x00, 0x00 },
	{ 0x0001d3a0, 0x08e1, 0x00, 0x53 },
	{ 0x00000ea8, 0x091c, 0x00, 0x00 },
	{ 0x00019d50, 0x091e, 0x00, 0x34 },
	{ 0x00000ea8, 0x0952, 0x00, 0x00 },
	{ 0x00018c90, 0x0954, 0x00, 0x02 },
	{ 0x00000ea8, 0x0986, 0x00, 0x00 },
	{ 0x00018530, 0x0988, 0x00, 0x03 },
	{ 0x00000ea8, 0x09b9, 0x00, 0x00 },
	{ 0x0001bc60, 0x09bb, 0x00, 0x20 },
	{ 0x00000ea8, 0x09f3, 0x00, 0x00 },
	{ 0x00017c90, 0x09f5, 0x00, 0x6f },
	{ 0x00000ea8, 0x0a25, 0x00, 0x00 },
	{ 0x000185c0, 0x0a27, 0x00, 0x6d },
	{ 0x00000ea8, 0x0a58, 0x00, 0x00 },
	{ 0x0001f710, 0x0a5a, 0x00, 0x0f },
	{ 0x00000ea8, 0x0a99, 0x00, 0x00 },
	{ 0x0000c980, 0x0a9b, 0x00, 0x52 },
	{ 0x00000ea8, 0x0ab5, 0x00, 0x00 },
	{ 0x0001ca00, 0x0ab7, 0x00, 0x65 },
	{ 0x00000ea8, 0x0af1, 0x00, 0x00 },
	{ 0x00014c30, 0x0af3, 0x00, 0x31 },
	{ 0x00000ea8, 0x0b1d, 0x00, 0x00 },
	{ 0x00013b70, 0x0b1f, 0x00, 0x3c },
	{ 0x00000ea8, 0x0b47, 0x00, 0x00 },
	{ 0x00011e20, 0x0b49, 0x00, 0x58 },
	{ 0x00000ea8, 0x0b6d, 0x00, 0x00 },
	{ 0x00010890, 0x0b6f, 0x00, 0x3f },
	{ 0x00000ea8, 0x0b91, 0x00, 0x00 },
	{ 0x0000f7d0, 0x0b93, 0x00, 0x4d },
	{ 0x00000ca8, 0x0bb2, 0x00, 0x00 },
	{ 0x0000f430, 0x0bb4, 0x00, 0x12 },
	{ 0x00000ca8, 0x0bd3, 0x00, 0x00 },
	{ 0x0000f430, 0x0bd5, 0x00, 0x12 },
	{ 0x00000ea8, 0x0bf4, 0x00, 0x00 },
	{ 0x00012df0, 0x0bf6, 0x00, 0x60 },
	{ 0x00000ea8, 0x0c1c, 0x00, 0x00 },
	{ 0x0001d5c0, 0x0c1e, 0x00, 0x30 },
	{ 0x00000ea8, 0x0c59, 0x00, 0x00 },
	{ 0x0001c500, 0x0c5b, 0x00, 0x12 },
	{ 0x00000ea8, 0x0c94, 0x00, 0xff },
	{ 0x00017060, 0x0c96, 0x00, 0x37 }
};

/* DS 0x800a7fb0 (LRVT) */
UCHAR LRVT[128] = {
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7d, 0x7d, 0x7d, 0x7d, 0x7c,
	0x7c, 0x7c, 0x7b, 0x7b, 0x7b, 0x7a, 0x7a, 0x79, 0x79, 0x78, 0x77, 0x76, 0x75, 0x74, 0x73, 0x72,
	0x71, 0x70, 0x6f, 0x6e, 0x6d, 0x6c, 0x6b, 0x6a, 0x69, 0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0x62,
	0x61, 0x60, 0x5f, 0x5e, 0x5d, 0x5c, 0x5b, 0x5a, 0x59, 0x58, 0x57, 0x56, 0x55, 0x54, 0x53, 0x52,
	0x51, 0x50, 0x4f, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42,
	0x41, 0x40, 0x3f, 0x3e, 0x3d, 0x3c, 0x3b, 0x3a, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32,
	0x31, 0x30, 0x2f, 0x2e, 0x2d, 0x2c, 0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22,
	0x21, 0x20, 0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12
};

/* DS 0x800a8030 (PANVT) */
UCHAR PANVT[128] = {
	0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25,
	0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2a, 0x2a, 0x2a, 0x2b, 0x2b, 0x2b,
	0x2c, 0x2c, 0x2c, 0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2e, 0x2f, 0x2f, 0x2f, 0x30, 0x30, 0x30, 0x31,
	0x31, 0x31, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 0x36, 0x36,
	0x36, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x3a, 0x3a, 0x3a, 0x3b, 0x3b, 0x3b,
	0x3c, 0x3c, 0x3c, 0x3d, 0x3d, 0x3d, 0x3e, 0x3e, 0x3e, 0x3f, 0x3f, 0x3f, 0x40, 0x40, 0x40, 0x41,
	0x41, 0x41, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x46
};

/* DS 0x800c4410 (Reverb_flg) */
UCHAR Reverb_flg = 0;

/* DS 0x800d783c (UnkSndSysInitSubFlg) */
SHORT UnkSndSysInitSubFlg = 0;

/* DS 0x800dbb8c (UnkSndSysFlg0) */
ULONG UnkSndSysFlg0 = 0;

/* DS 0x800df340 (Fade_mode) */
UCHAR Fade_mode = 0;

/* DS 0x800eade4 (MainVbSize) */
ULONG MainVbSize = 0;

/* DS 0x800eae40 (RoomVbSize) */
ULONG RoomVbSize = 0;

/* DS 0x800eae98 (Sedir) */
SHORT Sedir = 0;

/* DS 0x800cbc30 (Sedir2) */
SHORT Sedir2[4];

/* DS 0x800d4c78 (Cd_vol) */
SndVolume Cd_vol;

/* DS 0x800d4ca0 (Se_pri) */
SEENTRY Se_pri[24];

/* DS 0x800d7598 (Pan_vol) */
SndVolume Pan_vol;

/* DS 0x800d765c (Rev_vol) */
SndVolume Rev_vol;

/* DS 0x800d7856 (Bgm) */
BGMCTR Bgm;

/* DS 0x800d8ce4 (RevD_vol) */
SndVolume RevD_vol;

/* DS 0x800dcb9c (Main_vol) */
SndVolume Main_vol;

/* DS 0x800e2a84 (UnknownSndSysFlg) */
ULONG UnknownSndSysFlg;

/* DS 0x800e8770 (Seq_table) */
CHAR Seq_table[516];

/* DS 0x801f8e10 (Enemy_edt) */
tagEDT_TABLE_WORK Enemy_edt;

/* DS 0x800d4c48 (Vab_id) */
CHAR Vab_id[7];

/* DS 0x800d4f18 (sel_no) */
UCHAR sel_no[0x300];

/* DS 0x800d4f20 (Entory) */
REQUEST Entory[23];

/* DS 0x800d75a0 (pVh_tbl) */
ULONG* pVh_tbl[8];

/* DS 0x800d75c0 (Seq_ctr) */
SEQCTR Seq_ctr[3];

/* DS 0x800dbb88 (UnkSndFlg0) */
ULONG UnkSndFlg0 = 0;

/* DS 0x800dfd64 (pBgm) */
BGMCTR* pBgm = 0;

/* DS 0x800e8768 (Volume_Se) */
UCHAR Volume_Se = 0;

/* DS 0x800eae3c (Volume_Bgm) */
UCHAR Volume_Bgm = 0;
