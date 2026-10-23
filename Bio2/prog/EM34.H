/* EM34.H - types used only by the EM34 overlay (retail layout). Shared types are in the project headers. */
#ifndef EM34_H
#define EM34_H

#include "TYPES.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "SPL.H"

/* The generic Free[] area of the overlay ENEMY_WORK begins 4 bytes after the shared ENEMY_WORK.Free (two halfwords precede it in retail). */
#define EM_FREE_AREA(pEm)	((UCHAR *)&(pEm)->Free[1])

/* Byte timer that follows the last EM34_FREE member in the generic Free[] area (no member of that name in the shared EM34_FREE). */
#define EM34_RETAIL_24(pEm)	(EM_FREE_AREA(pEm)[sizeof(EM34_FREE)])

/* Retail narrows the four Direction_ck arguments to 16 bits (lh) at the call; the shared prototype takes long. */
#define Direction_ck(Hx, Hz, Tx, Tz)	Direction_ck((short)(Hx), (short)(Hz), (short)(Tx), (short)(Tz))

#ifdef OVL_36
#define EM_MAIN		Em234
#define PLDAMEGE_EM	Pldamege_Em234
#define PLDIE_EM	Pldie_Em234
#else
#define EM_MAIN		Em134
#define PLDAMEGE_EM	Pldamege_Em134
#define PLDIE_EM	Pldie_Em134
#endif

/* ---- functions of EM34.C ---- */
static void Em34_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Move_type00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Move_type01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv00_br(ENEMY_WORK *pEm);
static void Em34_mv00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv01_br(ENEMY_WORK *pEm);
static void Em34_mv01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv02_br(ENEMY_WORK *pEm);
static void Em34_mv02(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv03_br(ENEMY_WORK *pEm);
static void Em34_mv03(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv04_br(ENEMY_WORK *pEm);
static void Em34_mv04(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv05_br(ENEMY_WORK *pEm);
static void Em34_mv05(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv06_br(ENEMY_WORK *pEm);
static void Em34_mv06(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv07_br(ENEMY_WORK *pEm);
static void Em34_mv07(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv08_br(ENEMY_WORK *pEm);
static void Em34_mv08(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv09_br(ENEMY_WORK *pEm);
static void Em34_mv09(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0a_br(ENEMY_WORK *pEm);
static void Em34_mv0a(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0b_br(ENEMY_WORK *pEm);
static void Em34_mv0b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0c_br(ENEMY_WORK *pEm);
static void Em34_mv0c(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0d_br(ENEMY_WORK *pEm);
static void Em34_mv0d(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0e_br(ENEMY_WORK *pEm);
static void Em34_mv0e(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv0f_br(ENEMY_WORK *pEm);
static void Em34_mv0f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_mv10_br(ENEMY_WORK *pEm);
static void Em34_mv10(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Damage_33(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Damage_34(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_pldm00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_pldie00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_pldie01(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_kuchi_mv0(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv1(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv2(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv3(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv4(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv5(ENEMY_WORK *pEm, PARTS_W *pP_ptr);
static void Em34_kuchi_mv6(ENEMY_WORK *pEm, PARTS_W *pP_ptr);

void PLDAMEGE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
void PLDIE_EM(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_Jh_move(ENEMY_WORK *pEm);
static void Em34_die00(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em34_dir_spd(ENEMY_WORK *pEm, short Dir, SVECTOR *pSpd);
static void Em34_hand_right(ENEMY_WORK *pEm, MATRIX *pM);
static void Em34_hand_left(ENEMY_WORK *pEm, MATRIX *pM);
static void Em34_core_set_dest();	/* old-style declaration: callers pass unnarrowed ints, no implicit-decl blocks (as retail) */

/* Most callees (Root_ck, Guide_check, Joint_move, Goto00_ck, Sca_ck_em, Oba_set_ofs, ...) and the
   later-defined statics of this file are called WITHOUT a prototype in scope (implicit int
   declarations, arguments passed unnarrowed), as the retail call sites show. */
extern unsigned char Rnd(void);
/* Old-style (unprototyped) file-scope declarations: retail has no implicit-declaration blocks at
   their first calls inside nested blocks, yet passes the arguments unnarrowed. */
unsigned long Joint_move();
unsigned long Jh_set();
void Add_speedXZ();
void Goto01();


void Goto00(); long Goto00_ck();	/* file-scope old-style declarations placed here: Em34_mv00_br above still declares Goto00_ck implicitly (its retail body block), while the nested blocks below have none */
void Goto00(); long Goto00_ck();	/* file-scope old-style declarations placed here: Em34_mv00_br above still declares Goto00_ck implicitly (its retail body block), while the nested blocks below have none */

#endif /* EM34_H */
