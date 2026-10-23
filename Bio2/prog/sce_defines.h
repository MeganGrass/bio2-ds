
/*
*	Sce ID
*/
#define SCE_NOTHING						0
#define SCE_DOOR						1
#define SCE_ITEM						2
#define SCE_NORMAL						3
#define SCE_MESSAGE						4
#define SCE_EVENT						5
#define SCE_FLAG_CHG					6
#define SCE_WATER						7
#define SCE_MOVE						8
#define SCE_SAVE						9
#define SCE_ITEMBOX						10
#define SCE_DAMAGE						11
#define SCE_STATUS						12
#define SCE_HIKIDASHI					13		// Drawer
#define SCE_WINDOWS						14		// Computer


/*
*	Sce ID (November 1996)
*/
#define SCE_NOTHING_P					0
#define SCE_MESSAGE_P					1
#define SCE_DOOR_P						2
#define SCE_EVENT_P						3
#define SCE_FLAG_CHG_P					4
#define SCE_NORMAL_P					5
#define SCE_STATUS_P					6
#define SCE_UNKNOWN_P					7		// Modify Enemy Position?
#define SCE_WATER_P						8
#define SCE_ITEM_P						9
#define SCE_DAMAGE_P					10
#define SCE_MOVE_P						11
#define SCE_STAIRS_X					12
#define SCE_STAIRS_Z					13


/*
*	Sce Type
*/
#define SCE_AUTOMATIC					0x00
#define SCE_PLAYER						0x01
#define SCE_ENEMY						0x02
#define SCE_04							0x04
#define SCE_08							0x08
#define SCE_MANUAL						0x10
#define SCE_FRONT						0x20
#define SCE_UNDER						0x40
#define SCE_POINT4						0x80	// Xz[4][2] is used instead of X, Z, W, D


/*
*	Ck
*	Set
*/
#define FLAG_SYSTEM						0		// G.System_flg
#define FLAG_STATUS						1		// G.Status_flg
#define FLAG_STOP						2		// G.Stop_flg
#define FLAG_SCENARIO					3		// G.Savedata.Scenario_flg
#define FLAG_COMMON						4		// G.Savedata.Common_flg
#define FLAG_ROOM						5		// G.Savedata.Room_flg
#define FLAG_ENEMY						6		// G.Savedata.Enemy_flg
#define FLAG_ENEMY_2					7		// G.Savedata.Enemy_flg2
#define FLAG_ITEM						8		// G.Savedata.Item_flg
#define FLAG_MAP						9		// G.Savedata.Map_flg
#define FLAG_USE						10		// G.Use_flg
#define FLAG_MESSAGE					11		// Moji.Mess_flg
#define FLAG_ROOM_ENEMY					12		// G.Room_enemy_flg
#define FLAG_PRI_00						13		// G.SaveData.Pri_be_flg[0][0]
#define FLAG_PRI_01						14		// G.SaveData.Pri_be_flg[1][0]
#define FLAG_PRI_02						15		// G.SaveData.Pri_be_flg[2][0]
#define FLAG_PRI_03						16		// G.SaveData.Pri_be_flg[3][0]
#define FLAG_PRI_04						17		// G.SaveData.Pri_be_flg[4][0]
#define FLAG_PRI_05						18		// G.SaveData.Pri_be_flg[5][0]
#define FLAG_PRI_06						19		// G.SaveData.Pri_be_flg[6][0]
#define FLAG_PRI_07						20		// G.SaveData.Pri_be_flg[7][0]
#define FLAG_PRI_08						21		// G.SaveData.Pri_be_flg[8][0]
#define FLAG_PRI_09						22		// G.SaveData.Pri_be_flg[9][0]
#define FLAG_PRI_10						23		// G.SaveData.Pri_be_flg[10][0]
#define FLAG_PRI_11						24		// G.SaveData.Pri_be_flg[11][0]
#define FLAG_PRI_12						25		// G.SaveData.Pri_be_flg[12][0]
#define FLAG_PRI_13						26		// G.SaveData.Pri_be_flg[13][0]
#define FLAG_PRI_14						27		// G.SaveData.Pri_be_flg[14][0]
#define FLAG_PRI_15						28		// G.SaveData.Pri_be_flg[15][0]
#define FLAG_ZAPPING					29		// G.SaveData.Zapping_flg
#define FLAG_RBJ_SET					30		// G.Rbj_set_flg
#define FLAG_KEY						31		// G.SaveData.Key_flg
#define FLAG_MAP_C						32		// G.SaveData.Map_c_flg[1]
#define FLAG_MAP_I						33		// G.SaveData.Map_i_flg
#define FLAG_ITEM_2						34		// G.SaveData.Item_flg2
#define FLAG_MAP_O						35		// G.SaveData.Map_o_flg
#define FLAG_EX_0						36		// G.SaveData.ExData0[0]
#define FLAG_EX_1						37		// G.SaveData.ExData1[0]
#define FLAG_EX_2						38		// G.SaveData.ExData2[0]
#define FLAG_EX_3						39		// G.SaveData.ExData3[0]


/*
*	Cmp
*	Member_cmp
*/
#define CMP_EQ							0		// equal
#define CMP_GT							1		// greater than
#define CMP_GE							2		// greater than or equal
#define CMP_LT							3		// less than
#define CMP_LE							4		// less than or equal
#define CMP_NE							5		// not equal
#define CMP_AND							6		// and


/*
*	Calc
*	Calc2
*/
#define CALC_ADD						0		// add
#define CALC_SUB						1		// subtract
#define CALC_MUL						2		// multiply
#define CALC_DIV						3		// divide
#define CALC_MOD						4		// modulo
#define CALC_OR							5		// or
#define CALC_AND						6		// and
#define CALC_XOR						7		// xor
#define CALC_NOT						8		// not
#define CALC_SL							9		// shift left
#define CALC_SR_16BIT					10		// shift right
#define CALC_SR_32BIT					11		// shift right


/*
*	Work_set
*	Se_on
*	Sce_esp (Work_kind)
*	Kage_set
*	Super_set (Work_no)
*/
#define WORK_TYPE_POSITION				0		// VECTOR (Se_on), GsIDMATRIX (Sce_esp)
#define WORK_TYPE_PLAYER				1		// G.pEnemy[0]
#define WORK_TYPE_SUB_PLAYER			2		// G.pEnemy[1]
#define WORK_TYPE_ENEMY					3		// G.pEnemy[Work_no]
#define WORK_TYPE_OBJECT				4		// G.Ob_model[Work_no]
#define WORK_TYPE_DOOR					5		// pDwork[Work_no]
#define WORK_TYPE_ALL					6		// 
#define WORK_TYPE_PL_PARTS				0x80	// G.Player.pTmd2 (Super_set), G.Player.pSin_parts_ptr (Sce_esp)
#define WORK_TYPE_SPL_PARTS				0xA0	// 
#define WORK_TYPE_EM_PARTS				0xC0	// 
#define WORK_TYPE_OM_PARTS				0xE0	// 


/*
*	Speed_set
*/
#define SPEED_SPD_X						0
#define SPEED_SPD_Y						1
#define SPEED_SPD_Z						2
#define SPEED_DSPD_X					3
#define SPEED_DSPD_Y					4
#define SPEED_DSPD_Z					5
#define SPEED_ASPD_X					6
#define SPEED_ASPD_Y					7
#define SPEED_ASPD_Z					8
#define SPEED_ADSPD_X					9
#define SPEED_ADSPD_Y					10
#define SPEED_ADSPD_Z					11


/*
*	Member_set
*	Member_set2
*	Member_copy
*	Member_cmp
*	Member_calc
*	Member_calc2
*/
#define MEMBER_BE_FLAG0					0
#define MEMBER_BE_FLAG1					1
#define MEMBER_ROUTINE0					2
#define MEMBER_ROUTINE1					3
#define MEMBER_ROUTINE2					4
#define MEMBER_ROUTINE3					5
#define MEMBER_ID						6
#define MEMBER_TYPE						7
#define MEMBER_AT_OBJ_NO				8
#define MEMBER_AT_SCE_NO				9
#define MEMBER_ATTRIBUTE				10
#define MEMBER_POS_X					11
#define MEMBER_POS_Y					12
#define MEMBER_POS_Z					13
#define MEMBER_CDIR_X					14
#define MEMBER_CDIR_Y					15
#define MEMBER_CDIR_Z					16
#define MEMBER_NFLOOR					17
#define MEMBER_STATUS_FLAG				18
#define MEMBER_GROUND					19
#define MEMBER_DEST_X					20
#define MEMBER_DEST_Z					21
#define MEMBER_SCE_FLAG					22
#define MEMBER_SCE_FREE0				23
#define MEMBER_SCE_FREE1				24
#define MEMBER_SCE_FREE2				25
#define MEMBER_SCE_FREE3				26
#define MEMBER_SPD_X_BYTE				27
#define MEMBER_SPD_X					28
#define MEMBER_SPD_Y					29
#define MEMBER_SPD_Z					30
#define MEMBER_HOKAN_FLAG				31
#define MEMBER_OFS_X					32
#define MEMBER_OFS_Y					33
#define MEMBER_OFS_Z					34
#define MEMBER_AT_W						35
#define MEMBER_AT_H						36
#define MEMBER_AT_D						37
#define MEMBER_PARTS0_POS_Y				38
#define MEMBER_SCA_OLD_X				39
#define MEMBER_SCA_OLD_Z				40
#define MEMBER_FREE1					41
#define MEMBER_FREE2					42
#define MEMBER_DAMAGE_CNT				43


/*
*	Se_on
*/
#define SE_VAB_CORE						0
#define SE_VAB_ARMS						1
#define SE_VAB_ROOM						2
#define SE_VAB_ENEM						3
#define SE_VAB_DOOR						4


/*
*	Door_aot_set
*/
#define DOOR_NORMAL						0		// Knob on left, push forward
#define DOOR_REVERSE					1		// Knob on right, pull toward
#define DOOR_NORMAL2					2
#define DOOR_REVERSE2					3
#define DOOR_UP							4		// Ascending Lift
#define DOOR_DOWN						5		// Descending Lift
#define DOOR_NONE						6		// no door cutscene
#define DOOR_NOBU2						0x80

#define DOOR_KEY_TYPE_NONE				0		// No key is required to open door
#define DOOR_KEY_TYPE_UNLOCK			0xFE	// Door can be unlocked without a key
#define DOOR_KEY_TYPE_LOCKED			0xFF	// Door is locked from the other side


/*
*	Plc_dest
*	Plc_motion
*/
#define PLC_NOTHING						0
#define PLC_DEATH						1
#define PLC_FORWARD						4
#define PLC_RUN							5
#define PLC_IDLE						6
#define PLC_BACKWARD					7
#define PLC_BACKWARD_SLOW				8


/*
*	Plc_neck
*/
#define PLC_NECK_FRONT					0
#define PLC_NECK_COORD					1
#define PLC_NECK_ANGLE					2
#define PLC_NECK_YES					3
#define PLC_NECK_NO						4
#define PLC_NECK_TRACE					5
#define PLC_NECK_AUTO					6
#define PLC_NECK_FREE					7
#define PLC_NECK_INIT					8


/*
*	Plc_flg
*/
#define PLC_FLG_OR						0
#define PLC_FLG_EQUAL					1
#define PLC_FLG_XOR						2


/*
*	Plc_rot
*/
#define PLC_ROT_SCE_FREE0				0
#define PLC_ROT_SCE_FREE1				1


/*
*	Super_set
*	Super_on
*/
#define SUPER_WORK_TYPE_IDMATRIX		0x00	// GsIDMATRIX
#define SUPER_WORK_TYPE_PLAYER			0x40	// G.Player
#define SUPER_WORK_TYPE_ENEMY			0x80	// G.pEnemy[n] (bit 7 of Sup_no selects pSin_parts_ptr)
#define SUPER_WORK_TYPE_OBJECT			0xC0	// G.Ob_model[n]


/*
*	Item_aot_set (Action)
*/
#define ITEM_ACTION_NORMAL				0		// Stand to obtain
#define ITEM_ACTION_KNEEL				1		// Kneel down to pickup
#define ITEM_ACTION_BE_FLG				2		// G.Ob_model[Om_no].Be_flg = 0x80000000
#define ITEM_ACTION_80					0x80
#define ITEM_ACTION_A0					0xA0
#define ITEM_ACTION_C0					0xC0
#define ITEM_ACTION_E0					0xE0


/*
*	Sce_bgm_control
*/
#define BGM_MAIN						0
#define BGM_SUB0						1
#define BGM_SUB1						2

#define BGM_NOP							0
#define BGM_START						1
#define BGM_STOP						2
#define BGM_REPLAY						3
#define BGM_PAUSE						4
#define BGM_FADEOUT						5

#define MAIN_VOL						0
#define PROG0_VOL						1
#define PROG1_VOL						2
#define PROG2_VOL						3


/*
*	Sce_fade_set
*/
#define FADE_WORK_RED					1
#define FADE_WORK_GREEN					2
#define FADE_WORK_BLUE					4


/*
*	Sce_bgmtbl_set
*/
#define BGM_PANVT_00					0x0000
#define BGM_PANVT_01					0x0040
#define BGM_PANVT_02					0x4000
#define BGM_PANVT_03					0x8000
#define BGM_PANVT_04					0xC000


/*
*	Xa_on
*/
#define XA_MODE_STAGE					0		// G.pXa_sector
#define XA_MODE_OTHER					1		// Xa_sector00


/*
*	Parts_set (Work_type)
*/
#define PARTS_POLY_RGB0					0		// pSin_parts_ptr[Ptr]->Poly_rgb[0]
#define PARTS_POLY_RGB1					1		// pSin_parts_ptr[Ptr]->Poly_rgb[2]
#define PARTS_BE_FLAG0					2		// pSin_parts_ptr[Ptr]->Be_flg[0]
#define PARTS_BE_FLAG1					3		// pSin_parts_ptr[Ptr]->Be_flg[1]
#define PARTS_CDIR_X					4		// pSin_parts_ptr[Ptr]->Cdir_x
#define PARTS_CDIR_Y					5		// pSin_parts_ptr[Ptr]->Cdir_y
#define PARTS_CDIR_Z					6		// pSin_parts_ptr[Ptr]->Cdir_z


/*
*	Obj_model_set
*/
#define OM_NORMAL						0
#define OM_MIZU							1		// Water
#define OM_HIT							2
#define OM_BOX							4

// Be_flg
#define OM_DISABLED						0x0000
#define OM_ENABLED						0x0001
#define OM_NO_COLLISION					0x0002
#define OM_0004							0x0004
#define OM_0008							0x0008	// Texture Transparency
#define OM_0010							0x0010
#define OM_0020							0x0020
#define OM_ENEM_SE_INS_MASK				0x0040
#define OM_PLAYER_PUSH					0x0080	// Push to move
#define OM_MV							0x0100	// Climb on top animation
#define OM_MO							0x0200	// Auto-Move when collide
#define OM_0400							0x0400
#define OM_0800							0x0800
#define OM_1000							0x1000
#define OM_SC_AT						0x2000
#define OM_OB_AT						0x4000
#define OM_NO_HOSEI						0x8000	// No Display

// Attribute
#define OM_LIGHT_ON						0x0000
#define OM_01							0x0001
#define OM_02							0x0002
#define OM_04							0x0004
#define OM_08							0x0008
#define OM_LIGHT_OFF					0x0010
#define OM_DISABLED_2					0x0020
