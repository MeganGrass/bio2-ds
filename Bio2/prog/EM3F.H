/* EM3F.H - types used only by the EM3F overlay (retail layout). Shared types come from the project headers. */
#ifndef EM3F_H
#define EM3F_H

#include "TYPES.H"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include "CONFIG.H"
#include "ROOM.H"
#include "MAIN.H"
#include "EM_SUB.H"
#include "PLAYER.H"
#include "SCA.H"
#include "OMA.H"
#include "JOINT.H"

#ifdef OVL_3E
#define EM_MAIN		Em23f
#else
#define EM_MAIN		Em13f
#endif

/* ---- functions of EM3F.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em3f_init(ENEMY_WORK *pEm);
static void Em3f_main(ENEMY_WORK *pEm);
static void Em3f_wait(ENEMY_WORK *pEm);
static void func_801002C8(ENEMY_WORK *pEm);

#endif /* EM3F_H */
