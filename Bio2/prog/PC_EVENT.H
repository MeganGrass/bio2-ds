/*
 * PC_EVENT.H - Computer event screen prototypes and message strings
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\PC_EVENT.H
 */
#ifndef PC_EVENT_H
#define PC_EVENT_H
#include "TYPES.H"

void    Computer200(void);
void    Computer613(void);
void    Computer_init(ULONG Cut_no, ULONG Open_flg);
void    Computer_exit(ULONG Close_flg);

/* DS 0x80010c78: message string passed to Cprint (not yet identified) */
extern UCHAR s_Pc_event_msg_80010c78[];

/* Computer200/Computer613 message strings in the retail rodata (fixed DS addresses) */
extern UCHAR s_DOOR_LOCK_SERVICE________________80010bcc[];	/* DS 0x80010bcc */
extern UCHAR s_Hall_side_doors___2RELEASED_0__80010c00[];	/* DS 0x80010c00 */
extern UCHAR s_Hall_side_doors___2LOCKED_0__80010c24[];	/* DS 0x80010c24 */
extern UCHAR s__The_doors_can_be_unlocked_by_a___80010c48[];	/* DS 0x80010c48 */
extern UCHAR s__Checking_up_ID_CARD_025__025__0_80010c7c[];	/* DS 0x80010c7c */
extern UCHAR s__OK__Hall_side_doors_lock_releas_80010ca8[];	/* DS 0x80010ca8 */
extern UCHAR s_press_any_key___80010ccc[];	/* DS 0x80010ccc */
extern UCHAR s_Register_your_fingerprint___80010d0c[];	/* DS 0x80010d0c */
extern UCHAR s_UMBRELLA__RODEM__SYSTEM__Ver_5_0_80010d28[];	/* DS 0x80010d28 */
extern UCHAR s_Culture_Experiment_Room_Staff_Re_80010d60[];	/* DS 0x80010d60 */
extern UCHAR s_Accessing_020__020__020__020__02_80010d8c[];	/* DS 0x80010d8c */
extern UCHAR s_Enter_your_user_name__80010db4[];	/* DS 0x80010db4 */
extern UCHAR s_GUEST_80010dcc[];	/* DS 0x80010dcc */
extern UCHAR s__1This_name_is_not_in_the_resear_80010dd4[];	/* DS 0x80010dd4 */
extern UCHAR s_Register_your_fingerprint__80010e08[];	/* DS 0x80010e08 */
extern UCHAR s_Please_wait_025__025__025__025___80010e24[];	/* DS 0x80010e24 */
extern UCHAR s_Registration_complete____80010e4c[];	/* DS 0x80010e4c */
extern UCHAR s_Guest_registration_is_valid_for_2_80010e68[];	/* DS 0x80010e68 */



/* Globals defined in PC_EVENT.C */
	extern UCHAR pc_mes_data[160];						// 0x800a3dc8
	extern USHORT pc_mes_ofs[10];						// 0x800a3e68
#endif /* PC_EVENT_H_H */
