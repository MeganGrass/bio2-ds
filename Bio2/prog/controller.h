/*
 * controller.h - PSX pad button masks and game KEY_PAD flags
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\controller.h
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H


// PSYQ values for G.Padd
#define PAD_NULL			0x0000
#define PAD_L2				0x0001
#define PAD_R2				0x0002
#define PAD_L1				0x0004
#define PAD_R1				0x0008
#define PAD_TRIANGLE		0x0010
#define PAD_CIRCLE			0x0020
#define PAD_X				0x0040
#define PAD_SQUARE			0x0080
#define PAD_SELECT			0x0100
#define PAD_START			0x0800
#define PAD_UP				0x1000
#define PAD_RIGHT			0x2000
#define PAD_DOWN			0x4000
#define PAD_LEFT			0x8000


// Resident Evil 2 values for G.Key
enum KEY_PAD
{
	KEY_UP = (1 << 0),
	KEY_RIGHT = (1 << 1),
	KEY_DOWN = (1 << 2),
	KEY_LEFT = (1 << 3),
	KEY_AIM_UP = (1 << 4),
	KEY_AIM_DOWN = (1 << 5),
	KEY_AIM_FIRE = (1 << 6),
	KEY_INSPECT = (1 << 7),
	KEY_AIM = (1 << 8),
	KEY_RUN = (1 << 9),
	KEY_PREV = (1 << 10),
	KEY_NEXT = (1 << 11),
	KEY_CONFIRM = (1 << 12),
	KEY_CANCEL = (1 << 13),
	KEY_STATUS = (1 << 14),
	KEY_NULL = (1 << 15)
};


#endif /* CONTROLLER_H_H */
