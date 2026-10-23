/*
 * PL_WEP_PL_WEP2.C - One compilation unit for the player weapon code.
 *
 * PL_WEP.C and PL_WEP2.C are one object. Neither file has a static name that the other defines,
 * so compiling them together is the same as compiling them as one file.
 * The Makefile builds this file to OBJ\PSX\BUILD\PL_WEP.OBJ.
 */
#include "PL_WEP.C"
#include "PL_WEP2.C"
