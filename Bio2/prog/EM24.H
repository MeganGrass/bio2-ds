/* EM24.H - types used only by the EM24 overlay (retail layout). */
#ifndef EM24_H
#define EM24_H

#include "TYPES.H"
#include "ROOM.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "MAIN.H"
#include "EM_SUB.H"
#include "JOINT.H"
#include "PLAYER.H"
#include "SCA.H"
#include "SND.H"
#include "SUB.H"
#include "flag.h"

/* the overlay calls Direction_ck with halfword arguments and result (retail narrows at the caller); the project prototype uses long */
/* retail calls Rnd without a prototype: no narrowing of the result at the call site */
#define Rnd ((unsigned char (*)())Rnd)
/* the project prototype returns UCHAR; the overlay calls Cdir_ck without a prototype */
#define Cdir_ck ((unsigned long (*)())Cdir_ck)
/* retail calls Rot_vector without a prototype: the direction is passed as an int, not narrowed to SHORT */
#define Rot_vector ((void (*)())Rot_vector)
/* the project prototype returns USHORT; the overlay calls Sca_get_dir without a prototype */
#define Sca_get_dir ((int (*)())Sca_get_dir)
#define Direction_ck ((short (*)(short, short, short, short))Direction_ck)

typedef struct {
    unsigned char Wall_dir_new;
    unsigned char Sence_lv;
    unsigned char Tongue_Rno;
    unsigned char Kage_Rno;
    unsigned short Kage_x;
    unsigned short Kage_z;
    SCA_DATA *pSca_dat;
    SCA_DATA *pDest_wall;
    ENEMY_WORK *pTarget;
    unsigned short Attribute;
    unsigned char At_cnt;
    unsigned char Wep_no;
    unsigned char Wall_at;
    unsigned char Wep_bak;
    unsigned char Gun_area;
    unsigned char Dm_eff_rtn;
    short Dest_x;
    short Dest_y;
    short Dest_z;
    short Sca_ofs;
    void (*Dm_eff_func)();
    /* retail-only members (names unknown; offsets from the ENEMY_WORK base, Free[] = +0x218;
     * evidence addresses below are the EM22 slot-1 ones, EM24 uses the same fields).
     * Beta 2's EM22_FREE ends at Dm_eff_func; retail code addresses four more fields inside Free[]: */
    unsigned char Retail_28; /* +0x240 byte: sb/lbu in Em22_tongue_move @0x801009C0..0x80100AE0 */
    char Retail_29;          /* +0x241 signed byte: Spd_x step of Em22_mv0a (sb -10/-8/-60 @0x80103EAC/0x80103F28/0x80104158, lbu+sign-extend @0x801041B8) */
    short Retail_2a;         /* +0x242 halfword: Spd_y step of Em22_mv0a (sh @0x80103EB0/0x80103F30, lhu @0x801041D4) */
    short Retail_2c;         /* +0x244 halfword: saved Pos_y (sh @0x80103E44; lh @0x80104138, 0x80106834, 0x8010694C) */
} EM24_FREE;

#ifdef OVL_EM24_S0
#define EM_MAIN		Em224
#define EM_PLDM		Em224_Pldm
#else
#define EM_MAIN		Em124
#define EM_PLDM		Em124_Pldm
#endif

/* ---- functions of EM24.C ---- */
/* retail-only pad vibration helpers (PAD.C), as named in config/retail/main.symbols.txt;
 * prototypes from references/bio2-ds/Bio2/prog/PAD.H:33,35 (Pad_vib_set0 @0x8003947C,
 * Pad_vib_fade_set @0x800395B8) */
extern void Pad_vib_set0(int Time, unsigned short Attr);
extern void Pad_vib_fade_set(int Time, int Pow_st, int Pow_ed, unsigned short Attr);
extern unsigned long Flg_ck(unsigned long *p, unsigned long x);	/* retail: used by Em24_Init */
/* EM24 sees prototypes EM22.C does not (the halfword argument loads/truncations prove them;
 * spellings as in Beta 2 EM2B_SUB.H) */

void EM_PLDM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

static void Em24_damage_ck(ENEMY_WORK *pEm);
static void Em24_die_ck(ENEMY_WORK *pEm);
static void Em24_dm_fall(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_80101704(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void func_80102B10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void func_801039B4(ENEMY_WORK *pEm);	/* retail-only (no Beta twin, name unknown) */
static void func_80105140(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static void func_801056B0(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);	/* retail-only (no Beta twin, name unknown) */
static long Get_dir_pl(ENEMY_WORK *pEm);	/* Beta 2 EM2B_SUB.C Get_dir_pl, also linked here in retail */
/* Set_esp_w / Set_esp_s / Get_dir_pl: EM2B_SUB.C code, defined at the end of this file */
static void Set_esp_w(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec);
static void Set_esp_s(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec);

static void Dm_eff_spark(ENEMY_WORK *pEm);
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b);

#endif /* EM24_H */
