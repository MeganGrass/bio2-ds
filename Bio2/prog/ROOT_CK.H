#ifndef ROOT_CK_H
#define ROOT_CK_H
/*
 * ROOT_CK.H - Enemy root (route) and area check prototypes
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\ROOT_CK.H
 */

#include "TYPES.H"


/* Enemy route checks. Root_ck tests a route to Dest_area, Rnd_area picks a random area, Guide_check tests a
 * target point against the guide area, and kara_rootck2 is the route helper for the main body area. */
UCHAR   Root_ck(ENEMY_WORK *pEm, VECTOR *Dest, UCHAR Dest_area, UCHAR Mode);
UCHAR   Rnd_area(void);
UCHAR   Guide_check(short target_x, short target_z);
void    kara_rootck2(UCHAR hontai_area, UCHAR target_area, short target_x, short target_z);


#endif /* ROOT_CK_H_H */
