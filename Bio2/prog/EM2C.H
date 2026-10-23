/* EM2C.H - types used only by the EM2C overlay (retail layout). Shared types come from the project headers. */
#ifndef EM2C_H
#define EM2C_H

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

#ifdef OVL_2F
#define EM_MAIN		Em22c
#else
#define EM_MAIN		Em12c
#endif

/* ---- functions of EM2C.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em2c_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em2c_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void func_8010014C(ENEMY_WORK *pEm);

#endif /* EM2C_H */
