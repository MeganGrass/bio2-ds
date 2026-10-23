/*
 * PC_EVENT.H - Computer event screen prototypes and message strings
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PC_EVENT.H
 */
#ifndef PC_EVENT_H
#define PC_EVENT_H
#include "TYPES.H"
/* PC_EVENT message and state enums (from the beta enum list in enum.h). Values follow that list and are not yet
 * checked against retail; no code uses these names yet. */
enum {
	MES_A00,
	MES_A10,
	MES_A11,
	MES_A20,
	MES_A21,
	MES_CARD00,
	MES_CARD01,
	MES_CARD10,
	MES_CARD20,
	MES_PRESS,
};
enum {
	DUMMY0,
	DUMMY1,
	MES_613_0,
	MES_613_1,
	MES_613_2,
	MES_613_3,
	MES_613_4,
	MES_613_5,
	MES_613_6,
	MES_613_7,
	MES_613_8,
	MES_613_9,
};
enum {
	E1_INIT,
	E1_A1,
	E1_A3,
	E1_A8,
	E1_A9,
	E1_A10,
	E1_A11,
	E1_A12,
	E1_A13,
	E1_PRESS_KEY,
	E1_PRESS_KEY2,
	E1_EXIT,
	E1_EXIT2,
	E1_EXIT3,
};
enum {
	E2_INIT,
	E2_SET_UP,
	E2_SET_UP2,
	E2_SET_UP3,
	E2_INPUT1,
	E2_INPUT2,
	E2_NO_GUEST,
	E2_GUEST,
	E2_ENTRY_END,
	E2_ENTRY_END2,
	E2_ENTRY_END3,
	E2_EXIT,
	E2_EXIT2,
};


void    Computer200(void);
void    Computer613(void);
void    Computer_init(ULONG Cut_no, ULONG Open_flg);
void    Computer_exit(ULONG Close_flg);

/* DS 0x80010c78: message string passed to Cprint  */
extern UCHAR s_Pc_event_msg_80010c78[4];

/* Computer200/Computer613 message strings in the retail rodata (fixed DS addresses) */
extern const UCHAR s_DOOR_LOCK_SERVICE________________80010bcc[52];	/* DS 0x80010bcc */
extern const UCHAR s_Hall_side_doors___2RELEASED_0__80010c00[36];	/* DS 0x80010c00 */
extern const UCHAR s_Hall_side_doors___2LOCKED_0__80010c24[36];	/* DS 0x80010c24 */
extern const UCHAR s__The_doors_can_be_unlocked_by_a___80010c48[48];	/* DS 0x80010c48 */
extern const UCHAR s__Checking_up_ID_CARD_025__025__0_80010c7c[44];	/* DS 0x80010c7c */
extern const UCHAR s__OK__Hall_side_doors_lock_releas_80010ca8[36];	/* DS 0x80010ca8 */
extern UCHAR s_press_any_key___80010ccc[16];	/* DS 0x80010ccc */
extern const UCHAR s_Register_your_fingerprint___80010d0c[28];	/* DS 0x80010d0c */
extern const UCHAR s_UMBRELLA__RODEM__SYSTEM__Ver_5_0_80010d28[56];	/* DS 0x80010d28 */
extern const UCHAR s_Culture_Experiment_Room_Staff_Re_80010d60[44];	/* DS 0x80010d60 */
extern const UCHAR s_Accessing_020__020__020__020__02_80010d8c[40];	/* DS 0x80010d8c */
extern const UCHAR s_Enter_your_user_name__80010db4[24];	/* DS 0x80010db4 */
extern UCHAR s_GUEST_80010dcc[8];	/* DS 0x80010dcc */
extern const UCHAR s__1This_name_is_not_in_the_resear_80010dd4[52];	/* DS 0x80010dd4 */
extern const UCHAR s_Register_your_fingerprint__80010e08[28];	/* DS 0x80010e08 */
extern const UCHAR s_Please_wait_025__025__025__025___80010e24[40];	/* DS 0x80010e24 */
extern const UCHAR s_Registration_complete____80010e4c[28];	/* DS 0x80010e4c */
extern const UCHAR s_Guest_registration_is_valid_for_2_80010e68[44];	/* DS 0x80010e68 */



/* Globals defined in PC_EVENT.C */
	extern UCHAR pc_mes_data[160];						// 0x800a3dc8
	extern USHORT pc_mes_ofs[10];						// 0x800a3e68
/* Computer terminal states, named from the retail handlers (Computer200 = DOOR LOCK SERVICE, retail 8003a9f4;
 * Computer613 = fingerprint and guest registration, retail 8003b264). The two switches are separate state machines. */
enum {
	C200_INIT = 0x0,
	C200_EXIT_1 = 0xb,
};
enum {
	C613_ENTER_USER_NAME = 0x4,
	C613_KEYBOARD_NAME = 0x5,
	C613_EXIT_1 = 0xb,
	C613_EXIT_0 = 0xc,
};
#endif /* PC_EVENT_H_H */
