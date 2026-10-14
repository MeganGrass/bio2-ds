#ifndef GTE_OPS_H
#define GTE_OPS_H
/*
 * GTE_OPS.H - GTE operation macros not provided by the PSY-Q SDK
 * Resident Evil 2 (SLUS-00748) decompilation
 *
 * The SDK (<PSYQ.H> -> LIBGTE.H, INLINE_C.H, GTEMAC.H) provides every gte_
 * macro the project uses except gte_ldIR0, which loads GTE data register 8
 * (IR0) from a CPU register.
 */

#include "TYPES.H"

#if defined(_MSC_VER)
/* Visual Studio parses sources with MSVC, which has no __asm__ keyword, so the
 * SDK's GCC inline-asm gte_ macros are invisible to IntelliSense. These no-op
 * stubs only keep IntelliSense quiet. The real build uses CCPSX (Makefile). */
#undef gte_ldIR0
#define gte_ldIR0(...) ((void)0)

#undef gte_ApplyRotMatrix
#define gte_ApplyRotMatrix(...) ((void)0)
#undef gte_ReadRotMatrix
#define gte_ReadRotMatrix(...) ((void)0)
#undef gte_SetBackColor
#define gte_SetBackColor(...) ((void)0)
#undef gte_SetColorMatrix
#define gte_SetColorMatrix(...) ((void)0)
#undef gte_SetLightMatrix
#define gte_SetLightMatrix(...) ((void)0)
#undef gte_SetRotMatrix
#define gte_SetRotMatrix(...) ((void)0)
#undef gte_SetTransMatrix
#define gte_SetTransMatrix(...) ((void)0)
#undef gte_avsz3
#define gte_avsz3(...) ((void)0)
#undef gte_avsz3_b
#define gte_avsz3_b(...) ((void)0)
#undef gte_avsz4
#define gte_avsz4(...) ((void)0)
#undef gte_avsz4_b
#define gte_avsz4_b(...) ((void)0)
#undef gte_gpf12
#define gte_gpf12(...) ((void)0)
#undef gte_gpf12_b
#define gte_gpf12_b(...) ((void)0)
#undef gte_gpl12
#define gte_gpl12(...) ((void)0)
#undef gte_gpl12_b
#define gte_gpl12_b(...) ((void)0)
#undef gte_ldclmv
#define gte_ldclmv(...) ((void)0)
#undef gte_ldlv0
#define gte_ldlv0(...) ((void)0)
#undef gte_ldlvl
#define gte_ldlvl(...) ((void)0)
#undef gte_ldrgb
#define gte_ldrgb(...) ((void)0)
#undef gte_ldsv
#define gte_ldsv(...) ((void)0)
#undef gte_ldv0
#define gte_ldv0(...) ((void)0)
#undef gte_ldv3
#define gte_ldv3(...) ((void)0)
#undef gte_ldv3c
#define gte_ldv3c(...) ((void)0)
#undef gte_nccs
#define gte_nccs(...) ((void)0)
#undef gte_ncct
#define gte_ncct(...) ((void)0)
#undef gte_nclip
#define gte_nclip(...) ((void)0)
#undef gte_nclip_b
#define gte_nclip_b(...) ((void)0)
#undef gte_rt
#define gte_rt(...) ((void)0)
#undef gte_rtir
#define gte_rtir(...) ((void)0)
#undef gte_rtps
#define gte_rtps(...) ((void)0)
#undef gte_rtps_b
#define gte_rtps_b(...) ((void)0)
#undef gte_rtpt
#define gte_rtpt(...) ((void)0)
#undef gte_rtpt_b
#define gte_rtpt_b(...) ((void)0)
#undef gte_rtv0
#define gte_rtv0(...) ((void)0)
#undef gte_rtv0_b
#define gte_rtv0_b(...) ((void)0)
#undef gte_rtv0tr
#define gte_rtv0tr(...) ((void)0)
#undef gte_stclmv
#define gte_stclmv(...) ((void)0)
#undef gte_stlvl
#define gte_stlvl(...) ((void)0)
#undef gte_stlvnl
#define gte_stlvnl(...) ((void)0)
#undef gte_stopz
#define gte_stopz(...) ((void)0)
#undef gte_stotz
#define gte_stotz(...) ((void)0)
#undef gte_strgb
#define gte_strgb(...) ((void)0)
#undef gte_strgb3_gt3
#define gte_strgb3_gt3(...) ((void)0)
#undef gte_stsv
#define gte_stsv(...) ((void)0)
#undef gte_stsxy
#define gte_stsxy(...) ((void)0)
#undef gte_stsxy3
#define gte_stsxy3(...) ((void)0)
#undef gte_stsxy3_gt3
#define gte_stsxy3_gt3(...) ((void)0)
#undef gte_stsz
#define gte_stsz(...) ((void)0)
#undef gte_stszotz
#define gte_stszotz(...) ((void)0)
#else
#define gte_ldIR0( r0 ) __asm__ volatile (			\
	"mtc2	%0, $8"						\
	:						\
	: "r"( r0 ) )
#endif /* _MSC_VER */


#endif /* GTE_OPS_H */
