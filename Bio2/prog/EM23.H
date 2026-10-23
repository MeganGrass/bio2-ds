/* EM23.H - types used only by the EM23 overlay (retail layout). */
#ifndef EM23_H
#define EM23_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "ROOM.H"
/* retail callers use the full word Cdir_ck returns; EM_SUB.H declares a UCHAR result, which adds a byte mask */
#define Cdir_ck ((long (*)())Cdir_ck)
/* retail callers take Direction_ck's result as a short and pass short arguments; EM_SUB.H declares long */
#define Direction_ck ((short (*)(short, short, short, short))Direction_ck)

#ifdef OVL_27
#define Em123 Em223
#define Em123_Pldm Em223_Pldm
#endif

/* ---- functions of EM23.C ---- */
/* retail-only resident Dual Shock vibration helpers (config/retail/main.symbols.txt:
 * Pad_vib_set0 @0x8003947C, Pad_vib_fade_set @0x800395B8), called unprototyped */
extern void Pad_vib_set0();
extern void Pad_vib_fade_set();

extern void Em_seq_sound(ENEMY_WORK *pEm);
/* declared: retail SYM has no implicit-declaration block at their calls */
extern void Set_base_pos();
extern void Goto01(ENEMY_WORK *pEm, short Dir, short Add_dir);
extern void Cut_change(unsigned long Fc);
extern unsigned char Rnd();

void Em123(ENEMY_WORK *pEm);
static void Em23_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_ent_br(ENEMY_WORK *pEm);
static void Em23_ent(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_walk_br(ENEMY_WORK *pEm);
static void Em23_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_run_br(ENEMY_WORK *pEm);
static void Em23_run(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_turn_br(ENEMY_WORK *pEm);
static void Em23_turn(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Chit_br(ENEMY_WORK *pEm);
static void Em23_Chit(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_haziki_br(ENEMY_WORK *pEm);
static void Em23_haziki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_taiki_br(ENEMY_WORK *pEm);
static void Em23_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_last_br(ENEMY_WORK *pEm);
static void Em23_last(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Bite(ENEMY_WORK *pEm);
static void Em23_mouse_move(ENEMY_WORK *pEm);
static unsigned long Pl_bite_dm(ENEMY_WORK *pEm);
static unsigned long Player_stumble_ck(ENEMY_WORK *pEm);
static unsigned long Gas_get(ENEMY_WORK *pEm);
static long Em23_get_spd(long Rtime, long Ntime, long Dir);
static long Em23_get_spd_r(long Rtime, long Ntime, long Dir);
static void Earthquake_move(ENEMY_WORK *pEm);
static void Em23_damage_ck(ENEMY_WORK *pEm);
static void Em23_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_explosion(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_dm_escape(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_died(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_dm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_die_ck(ENEMY_WORK *pEm);
static void Em23_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void Em123_Pldm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Pl_die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em23_Pl_stumble(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_pl_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2a_pl_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Gas_explosion(ENEMY_WORK *pEm, unsigned long Rtn);
static void Pl_stumble_blood(unsigned long Rtn);
static void Em23_Head_flying(ENEMY_WORK *pEm);
static void Em23_Bomb_tbl_set(void);
static void Em23_Bomb_tbl_reset(void);
static void Em23_add_dm(unsigned long Att);
static unsigned long S_rnd(unsigned char N, unsigned long *pBuf, unsigned long *pBuf2);
static unsigned long Dm_eff(ENEMY_WORK *pEm);
static unsigned long Dm_eff_blood(ENEMY_WORK *pEm, unsigned long No, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Flg);
static void Dm_eff_3handgun(ENEMY_WORK *pEm);
static void Bite_blood(ENEMY_WORK *pEm);
static void Explosion_blood(ENEMY_WORK *pEm);
static void Dm_eff_Frame(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type);
static void Dm_eff_shotgun(ENEMY_WORK *pEm, unsigned long Num);
static void Em_explosion_set(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Dm_eff_spark(ENEMY_WORK *pEm);
static void Color_chg(ENEMY_WORK *pEm, unsigned long Num, unsigned long r, unsigned long g, unsigned long b);
static void Set_esp_w(ENEMY_WORK *pEm, unsigned long Id, unsigned long Type, unsigned long Zoom, unsigned long Cdir_y, unsigned long Parts_no, SVECTOR *pSvec);

#endif /* EM23_H */
