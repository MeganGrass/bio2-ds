/*
 * DIEDEMO_DIESPR.C - One compilation unit for the DIEDEMO overlay.
 *
 * DIEDEMO.C and DIESPR.C are one object: the retail DIEDEMO overlay holds the functions of both
 * (Ghidra places every Die_* function in the DIEDEMO namespace). Neither file has a static name
 * that the other defines, so compiling them together is the same as compiling them as one file.
 * The Makefile builds this file to OBJ\PSX\BUILD\DIEDEMO.OBJ with the DIEDEMO section prefix.
 */
#include "DIEDEMO.C"
#include "DIESPR.C"
