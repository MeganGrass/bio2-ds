/* EM3E.H - types used only by the EM3E overlay (retail layout). Shared types come from the project headers. */
#ifndef EM3E_H
#define EM3E_H

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

/* ENEMY_WORK.Free[] starts 4 bytes after the end of the Kage_ofs member (+0x218); the project struct has no
 * Retail_214/Retail_216 halfwords in front of it, so the overlays address it with this offset. */
#define OVL_FREE(p) ((ULONG *)((UCHAR *)(p)->Free + 4))

#ifdef OVL_3D
#define EM_MAIN		Em23e
#else
#define EM_MAIN		Em13e
#endif

/* ---- functions of EM3E.C ---- */
void EM_MAIN(ENEMY_WORK *pEm);
static void Em3e_init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3e_move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3e_wait(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);
static void Em3e_sce(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq);

#endif /* EM3E_H */
