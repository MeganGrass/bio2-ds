/*
 * OMA.C - Enemy vs object-model collision and push/pull checks
 * Resident Evil 2 (SLUS-00748) decompilation
 * Source: D:\Bio2\prog\OMA.C
 */
#include "TYPES.H"
#include <ASM.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <STRINGS.H>
#include "MAIN.H"
#include "OBA.H"
#include "OMA.H"
#include "EM_SUB.H"
#include "SCA.H"
#include "SPL_AUTO.H"
#include "SUB.H"
#include "flag.h"

/* Oma_ck_oba - 0x80036ef8
   Enemy/player AT box vs. object model box.
   Flg 0: push the enemy out of the object.
   Flg 1: push the object out of the enemy.
   Flg 2: hit test only.
   Returns 1 on contact.
*/
/* DS 0x80036ef8 (Oma_ck_oba) */
ULONG Oma_ck_oba(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm, ULONG Flg)
{
	static ULONG Y_flg_old;     /* DS 0x800c3b38 */

	LONG    Dx, Dy, Dz;
	LONG    Wx, Wy, Wz;
	LONG    Dy2;
	LONG    Y_flg;
	LONG    Dold;
	LONG    Bak;
	LONG    Ox, Oz;     /* object box centre */
	LONG    Ep;         /* enemy box coordinate on the push axis */
	LONG    Push;
	LONG    Neg;        /* push toward the negative side */

	if ((pEm->Be_flg & 0x40) != 0)
		return 0;
	if ((pOm->Be_flg & 2) != 0)
		return 0;

	Wx = (ULONG)pEm->Atd[0].At_w + pOm->Atd[0].W;
	Wz = (ULONG)pEm->Atd[0].At_w + pOm->Atd[0].D;
	Dx = pOm->Atd[0].X - pEm->Atd[0].X;
	Dz = pOm->Atd[0].Z - pEm->Atd[0].Z;
	if ((ULONG)(Wx * 2) < (ULONG)(Dx + Wx))
		return 0;
	if ((ULONG)(Wz * 2) < (ULONG)(Dz + Wz))
		return 0;

	Dy    = pOm->Atd[0].Y - pEm->Atd[0].Y;
	Wy    = (ULONG)pEm->Atd[0].At_h + (ULONG)pOm->Atd[0].At_h;
	Dy2   = Wy * 2;
	Y_flg = (ULONG)(Dy + Wy) < (ULONG)Dy2;
	if (!Y_flg)
		return 0;
	if (Flg == 2)
		return 1;

	pEm->At_obj_no = (UCHAR)(pOm - G.Ob_model);
	pOm->At_em_no  = pEm->Work_no;

	Dold      = (ULONG)(((LONG)pOm->Old_y2 - (LONG)pEm->Old_y2) + Wy) < (ULONG)Dy2;
	Y_flg_old = Dold;

	/* not overlapping vertically last frame: resolve on the Y axis */
	if (Dold != Y_flg)
	{
		if (Flg != 0)
		{
			if (Dy < 0)
				Wy = -Wy;
			pOm->Pos_y    = (pEm->Atd[0].Y - pOm->Atd[0].Ofs_y) + Wy;
			pOm->Atd[0].Y = pOm->Pos_y + pOm->Atd[0].Ofs_y;
			return 1;
		}
		Bak = pOm->Atd[0].Y - pEm->Atd[0].Ofs_y;
		if (Dy < 0)
			pEm->Pos_y = Bak + 1 + Wy;
		else
			pEm->Pos_y = Bak - Wy;
		pEm->Atd[0].Y = pEm->Pos_y + pEm->Atd[0].Ofs_y;
		return 1;
	}

	/* choose the horizontal axis to resolve on */
	Ox = pOm->Atd[0].X;
	Oz = pOm->Atd[0].Z;
	if ((ULONG)(Wx * 2) < (ULONG)((Ox - pEm->Old_x2) + Wx))
	{
		/* was outside on X last frame */
		Ep  = pEm->Atd[0].X;
		Neg = (Ep - pEm->Old_x2) < 0;
		goto push_x;
	}
	if ((ULONG)(Wz * 2) < (ULONG)((Oz - pEm->Old_z2) + Wz))
	{
		/* was outside on Z last frame */
		Ep  = pEm->Atd[0].Z;
		Neg = (Ep - pEm->Old_z2) < 0;
		goto push_z;
	}
	{
		LONG xz = Dx * Wz;
		LONG zx = Dz * Wx;

		if (xz < 0) xz = -xz;
		if (zx < 0) zx = -zx;
		if (zx < xz)
		{
			Ep  = pEm->Atd[0].X;
			Neg = Dx < 0;
			goto push_x;
		}
		Ep  = pEm->Atd[0].Z;
		Neg = Dz < 0;
		goto push_z;
	}

push_x:
	if (Flg == 0)
	{
		Push = (Ox + (Neg ? (Wx + 1) : -(Wx + 1))) - Ep;
		pEm->Atd[0].X += Push;
		pEm->Pos_x    += Push;
		return 1;
	}
	Push = (Neg ? Ep - (Wx + 1) : Ep + (Wx + 1)) - Ox;
	pOm->Atd[0].X += Push;
	pOm->Pos_x    += Push;
	return 1;

push_z:
	if (Flg == 0)
	{
		Push = (Oz + (Neg ? (Wz + 1) : -(Wz + 1))) - Ep;
		pEm->Atd[0].Z += Push;
		pEm->Pos_z    += Push;
		return 1;
	}
	Push = (Neg ? Ep - (Wz + 1) : Ep + (Wz + 1)) - Oz;
	pOm->Atd[0].Z += Push;
	pOm->Pos_z    += Push;
	return 1;
}

/* Oma_ck_om - 0x80037258
   Object vs. object: push pOm2 out of pOm. Returns 1 on contact.
*/
/* DS 0x80037258 (Oma_ck_om) */
ULONG Oma_ck_om(OBJ_MODEL_WORK *pOm, OBJ_MODEL_WORK *pOm2)
{
	LONG    a, b, c, d, e, f, e2;
	LONG    Flg, Flg_old;
	LONG    Bak;
	LONG    Ofs_y1, Ofs_y2;
	LONG    Frame_pad[4];		/* DS 0x80037258 frame 0x10: stack slot kept by retail, never read */

	(void)Frame_pad;
	if (((pOm->Be_flg | pOm2->Be_flg) & 0x40) != 0)
		return 0;
	if ((pOm->Be_flg & pOm2->Be_flg & 0x200000) != 0)
		return 0;

	a = pOm2->Atd[0].X - pOm->Atd[0].X;
	b = pOm2->Atd[0].Z - pOm->Atd[0].Z;
	c = (ULONG)pOm->Atd[0].W + (ULONG)pOm2->Atd[0].W;
	d = (ULONG)pOm->Atd[0].D + (ULONG)pOm2->Atd[0].D;
	if ((ULONG)(c * 2) < (ULONG)(a + c))
		return 0;
	if ((ULONG)(d * 2) < (ULONG)(b + d))
		return 0;

	e   = pOm2->Atd[0].Y - pOm->Atd[0].Y;
	f   = (ULONG)pOm->Atd[0].At_h + (ULONG)pOm2->Atd[0].At_h;
	Flg = (ULONG)(e + f) <= (ULONG)(f * 2);
	if (!Flg)
		return 0;

	Ofs_y2  = pOm2->Atd[0].Ofs_y;
	Ofs_y1  = pOm->Atd[0].Ofs_y;
	Flg_old = (ULONG)(((pOm2->Old_y + Ofs_y2) - (pOm->Old_y + Ofs_y1)) + f) <= (ULONG)(f * 2);

	if (Flg_old)
	{
		/* already overlapping vertically: push horizontally on the shallower axis */
		LONG xz = a * d;

		e2 = b * c;
		if (xz < 0) xz = -xz;
		if (e2 < 0) e2 = -e2;
		if (e2 < xz)
		{
			Bak = pOm2->Pos_x;
			if (a < 0)
				pOm2->Pos_x = (pOm->Pos_x - 1) - c;
			else
				pOm2->Pos_x = pOm->Pos_x + 1 + c;
			pOm2->Atd[0].X += pOm2->Pos_x - Bak;
		}
		else
		{
			Bak = pOm2->Pos_z;
			if (b < 0)
				pOm2->Pos_z = (pOm->Pos_z - 1) - d;
			else
				pOm2->Pos_z = pOm->Pos_z + 1 + d;
			pOm2->Atd[0].Z += pOm2->Pos_z - Bak;
		}
	}
	else
	{
		/* landed on / hit from below */
		Bak = (pOm->Pos_y + Ofs_y1) - Ofs_y2;
		if (e < 0)
			pOm2->Pos_y = (Bak - 1) - f;
		else
			pOm2->Pos_y = Bak + 1 + f;
	}
	return 1;
}

/* Oma_ck_front - 0x80037450
   Is the object directly in front of pEm (used for pushing)?
*/
/* DS 0x80037450 (Oma_ck_front) */
ULONG Oma_ck_front(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm)
{
	SVECTOR Svec;
	LONG    X, Z;
	LONG    W, D;

	Svec.vz = 0;
	Svec.vx = (SHORT)(((ULONG)pEm->Atd[0].At_w * 0x78) / 100) + 0x80;
	Rot_vector(pEm->Cdir_y, &Svec, &Svec);

	W = pOm->Atd[0].W - 0x80;
	D = pOm->Atd[0].D - 0x80;
	X = pOm->Pos_x - pEm->Pos_x;
	Z = pOm->Pos_z - pEm->Pos_z;
	if (((ULONG)(X + W) > (ULONG)(W * 2)) && ((ULONG)(Z + D) > (ULONG)(D * 2)))
		return 0;

	if (X < 0) X = -X;
	if (Z < 0) Z = -Z;
	if (Z < X)
	{
		X  = (SHORT)(Svec.vx + (SHORT)pEm->Pos_x) - pOm->Atd[0].X;
		Z  = pEm->Pos_z - pOm->Atd[0].Z;
		D += pEm->Atd[0].W;
	}
	else
	{
		X  = pEm->Pos_x - pOm->Atd[0].X;
		Z  = (SHORT)(Svec.vz + (SHORT)pEm->Pos_z) - pOm->Atd[0].Z;
		W += pEm->Atd[0].W;
	}
	if ((ULONG)(X + W) > (ULONG)(W << 1))
		return 0;
	return (ULONG)(Z + D) <= (ULONG)(D << 1);
}

/* DS 0x800a2594 (Up_add_tbl) */
SHORT Up_add_tbl[20] = {
	884, 1251, 581, 971, 800, 800, 800, 800,	/* +0x0 */
	800, 800, 800, 800, 800, 800, 800, 800,	/* +0x10 */
	800, 800, 800, 600,	/* +0x20 */
};

/* DS 0x800a25bc - short[20]. Retail indexes Down_add_tbl[Id+8] for Id >= 0x10 (index 24 and up), which reads past this table into Box_sub_tbl at 0x800a25e4. */
SHORT Down_add_tbl[20] = {
	884, 653, 854, 581, 1000, 1000, 1000, 1000,	/* +0x0 */
	1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,	/* +0x10 */
	1000, 1000, 1000, 600,	/* +0x20 */
};

/* DS 0x800a260c */
SHORT Up_add_Spl_tbl[32] = {
	0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0258, 0x0320, 0x0320,
	0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0258,
	0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320,
	0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0320, 0x0258,
};

/* DS 0x800c3b7c (Gnd_y) */
SHORT Gnd_y = 0x0000;

/* DS 0x800dcbc8 (Ground_in_flg) */
long Ground_in_flg = 0x00000000;

/* Oma_pl_updown_ck - 0x800375c8
   Player climb-up / jump-down check in front of the player.
   Id indexes Up_add_tbl / Down_add_tbl.
*/
/* DS 0x800375c8 (Oma_pl_updown_ck) */
ULONG Oma_pl_updown_ck(ULONG Id)
{
	SVECTOR         Sv;
	VECTOR          Add_pos;
	SHORT           Cdir_y;
	OBJ_MODEL_WORK  *pOm;
	OBJ_MODEL_WORK  *pOn_om;
	ULONG           Hit;
	LONG            Bak_y;

	Cdir_y = (G.Player.Cdir_y + 0x200) & 0xfc00;

	/* climb up */
	if (G.Player.nFloor < 0x1f)
	{
		Sv.vx = Up_add_tbl[Id];
		Sv.vz = 0;
		Rot_vector(Cdir_y, &Sv, &Sv);
		Add_pos.vx = Sv.vx + G.Player.Pos_x;
		Add_pos.vy = G.Player.Pos_y - 0x708;
		Add_pos.vz = Sv.vz + G.Player.Pos_z;
		Hit    = Sca_ck_hit(&Add_pos, 0, 1 << (G.Player.nFloor & 0x1f), 0x8000);
		pOn_om = Oma_on_check((ENEMY_WORK *)&G.Player);
		if ((((Hit & 2) != 0) || (pOn_om != NULL)) &&
			((SHORT)Sca_get_ground(&Add_pos, G.Player.Atd[0].At_w, 0x8000, 0) <= Add_pos.vy) &&
			((Ground_in_flg & 1) == 0) &&
			(Oma_obj_ck_all(&Add_pos, (ENEMY_WORK *)&G.Player) == 0))
		{
			G.Player.pOn_om    = pOn_om;
			G.Player.Routine_1 = 8;
			G.Player.Routine_2 = 0;
			G.Player.Routine_3 = 0;
			return 1;
		}
	}

	/* jump down */
	Sv.vx = Down_add_tbl[Id];
	Sv.vz = 0;
	Rot_vector(Cdir_y, &Sv, &Sv);
	Add_pos.vx = Sv.vx + G.Player.Pos_x;
	Add_pos.vz = Sv.vz + G.Player.Pos_z;
	Add_pos.vy = G.Player.Pos_y;
	Add_pos.vy = Sca_get_ground(&Add_pos, G.Player.Atd[0].At_w, (ULONG)G.Player.Sc_id << 8, 0);
	if (Gnd_y < Add_pos.vy)
		Add_pos.vy = Gnd_y;
	Hit = Sca_ck_hit(&Add_pos, 0, 1 << (G.Player.nFloor & 0x1f), (ULONG)G.Player.Sc_id << 8);

	if (G.Player.Pos_y >= Add_pos.vy)
		return 0;
	if (((Hit & 4) == 0) && (G.Player.pOn_om == NULL))
		return 0;

	for (pOm = G.Ob_model; pOm < G.pObj_work_end; pOm++)
	{
		if (((pOm->Be_flg & 1) != 0) && ((pOm->Be_flg & 2) == 0) && (pOm != G.Player.pOn_om))
		{
			if (Omd_in_check(&Add_pos, pOm, G.Player.Atd[0].At_w, 1) != 0)
				return 0;
			Bak_y      = Add_pos.vy;
			Add_pos.vy = G.Player.Pos_y;
			if (Omd_in_check(&Add_pos, pOm, G.Player.Atd[0].At_w, 1) != 0)
				return 0;
			Add_pos.vy = (SHORT)Bak_y;
		}
	}
	if (Oma_obj_ck_all(&Add_pos, (ENEMY_WORK *)&G.Player) != 0)
		return 0;

	if (G.Player.pOn_om != NULL)
		G.Player.pOn_om = NULL;
	G.Player.Routine_1 = 8;
	G.Player.Routine_2 = 2;
	G.Player.Routine_3 = 0;
	return 1;
}

/* DS 0x800a264c - short[32]; index Id+0x10 */
SHORT Down_add_Spl_tbl[32] = {
	1000, 1000, 1000, 1000, 1000, 600, 1000, 1000,	/* +0x0 */
	1000, 1000, 1000, 1000, 1000, 0, 0, 600,	/* +0x10 */
	1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,	/* +0x20 */
	1000, 1000, 1000, 1000, 1000, 1000, 1000, 600,	/* +0x30 */
};

/* Oma_Spl_updown_ck - 0x80037940
   Sub-player / enemy climb-up and jump-down check.
*/
/* DS 0x80037940 (Oma_Spl_updown_ck) */
ULONG Oma_Spl_updown_ck(ENEMY_WORK *pEm)
{
	SVECTOR         Sv;
	VECTOR          Add_pos;
	SHORT           Cdir_y;
	USHORT          Type;
	OBJ_MODEL_WORK  *pOm;
	ULONG           Hit;
	LONG            Bak_y;
	UCHAR           Rtn;

	Cdir_y = (pEm->Cdir_y + 0x200) & 0xfc00;

	/* climb up (NOTE: ids >= 0x10 index Up_add_tbl[Id - 4] in the binary) */
	if (pEm->Id < 0x10)
		Sv.vx = Up_add_Spl_tbl[pEm->Id + 0x10];
	else
		Sv.vx = Up_add_tbl[pEm->Id - 4];
	Sv.vz = 0;
	Rot_vector(Cdir_y, &Sv, &Sv);
	Add_pos.vx = Sv.vx + pEm->Pos_x;
	Add_pos.vy = pEm->Pos_y - 0x708;
	Add_pos.vz = Sv.vz + pEm->Pos_z;

	if ((pEm->Id == 0x0f) || (pEm->Id == 0x45) || (pEm->Id == 0x4f))
		Type = 0x800;
	else
		Type = 0x1000;
	Hit = Sca_ck_hit(&Add_pos, 0, 1 << (pEm->nFloor & 0x1f), Type);

	if (((Hit & 2) != 0) && (pEm->nFloor < 0x1f) &&
		(Add_pos.vy >= (SHORT)Sca_get_ground(&Add_pos, pEm->Atd[0].At_w, (ULONG)pEm->Sc_id << 8, 0)) &&
		((Ground_in_flg & 1) == 0) &&
		(Oma_obj_ck_all(&Add_pos, pEm) == 0))
	{
		if (((G.Status_flg & STAT_SUB_PLAYER) != 0) && (pEm == G.pEnemy[1]))
			Rtn = 8;
		else if (pEm->Routine_1 == 4)
			Rtn = 0x16;
		else
			Rtn = 0x18;
		pEm->Routine_1 = Rtn;
		pEm->Routine_2 = 0;
	}
	else
	{
		/* jump down (NOTE: ids >= 0x10 index Down_add_tbl[Id + 8] in the binary) */
		if (pEm->Id < 0x10)
			Sv.vx = Down_add_Spl_tbl[pEm->Id + 0x10];
		else
			Sv.vx = Down_add_tbl[pEm->Id + 8];
		Sv.vz = 0;
		Rot_vector(Cdir_y, &Sv, &Sv);
		Add_pos.vx = Sv.vx + pEm->Pos_x;
		Add_pos.vz = Sv.vz + pEm->Pos_z;
		Add_pos.vy = pEm->Pos_y;
		Add_pos.vy = Sca_get_ground(&Add_pos, pEm->Atd[0].At_w, (ULONG)pEm->Sc_id << 8, 0);

		if ((pEm->Id == 0x0f) || (pEm->Id == 0x45) || (pEm->Id == 0x4f))
			Type = 0x800;
		else
			Type = 0x1000;
		Hit = Sca_ck_hit(&Add_pos, 0, 1 << (pEm->nFloor & 0x1f), Type);

		if (Add_pos.vy <= pEm->Pos_y)
			return 0;
		if ((Hit & 4) == 0)
			return 0;

		for (pOm = G.Ob_model; pOm < G.pObj_work_end; pOm++)
		{
			if (((pOm->Be_flg & 1) != 0) && ((pOm->Be_flg & 2) == 0) && (pOm != pEm->pOn_om))
			{
				if (Omd_in_check(&Add_pos, pOm, pEm->Atd[0].At_w, 1) != 0)
					return 0;
				Bak_y      = Add_pos.vy;
				Add_pos.vy = pEm->Pos_y;
				if (Omd_in_check(&Add_pos, pOm, pEm->Atd[0].At_w, 1) != 0)
					return 0;
				Add_pos.vy = (SHORT)Bak_y;
			}
		}
		if (Oma_obj_ck_all(&Add_pos, pEm) != 0)
			return 0;

		if (((G.Status_flg & STAT_SUB_PLAYER) != 0) && (pEm == G.pEnemy[1]))
			Rtn = 8;
		else if (pEm->Routine_1 == 4)
			Rtn = 0x17;
		else
			Rtn = 0x19;
		pEm->Routine_1 = Rtn;
		pEm->Routine_2 = 2;
	}
	pEm->Routine_3 = 0;
	*(SHORT *)pEm->Free = pEm->Cdir_y;
	return 1;
}

/* Oma_front_ck - 0x80037d48
   Is the object directly in front of pEm (used for climbing)?
*/
/* DS 0x80037d48 (Oma_front_ck) */
ULONG Oma_front_ck(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm)
{
	SVECTOR Svec;
	LONG    X, Z;
	LONG    W, D;

	Svec.vz = 0;
	Svec.vx = (SHORT)(((ULONG)pEm->Atd[0].At_w * 0x87) / 100) + 0x80;
	Rot_vector(pEm->Cdir_y, &Svec, &Svec);

	X = pOm->Pos_x - pEm->Pos_x;
	Z = pOm->Pos_z - pEm->Pos_z;
	W = pOm->Atd[0].W - 0x80;
	D = pOm->Atd[0].D - 0x80;
	if (((ULONG)(X + W) > (ULONG)(W * 2)) && ((ULONG)(Z + D) > (ULONG)(D * 2)))
		return 0;

	if (X < 0) X = -X;
	if (Z < 0) Z = -Z;
	if (Z < X)
	{
		X = (SHORT)(Svec.vx + (SHORT)pEm->Pos_x);
		Z = pEm->Pos_z;
	}
	else
	{
		X = pEm->Pos_x;
		Z = (SHORT)(Svec.vz + (SHORT)pEm->Pos_z);
	}
	if ((ULONG)((X - pOm->Atd[0].X) + W) > (ULONG)(W * 2))
		return 0;
	return (ULONG)((Z - pOm->Atd[0].Z) + D) <= (ULONG)(D * 2);
}

/* Oma_on_check - 0x80037eac
   First climbable object on pEm's floor directly in front of it.
*/
/* DS 0x80037eac (Oma_on_check) */
OBJ_MODEL_WORK *Oma_on_check(ENEMY_WORK *pEm)
{
	OBJ_MODEL_WORK  *pOm;
	OBJ_MODEL_WORK  *pEnd;      /* G.pObj_work_end, kept in a register as retail does */

	pEnd = G.pObj_work_end;
	pOm = G.Ob_model;
	if (pOm >= pEnd) {
		return NULL;
	}
/* goto loop: a do/while form compiles to a larger frame in this build */
on_loop:
	if (((pOm->Be_flg & 0x101) == 0x101) &&
		(pEm->nFloor == pOm->nFloor) &&
		(pEm->pOn_om != pOm) &&
		(Oma_front_ck(pEm, pOm) != 0))
		return pOm;
	pOm++;
	if (pOm < pEnd) {
		goto on_loop;
	}
	return NULL;
}

/* Oma_pull - 0x80037f60
   Keep pEm (standing on pOm) on top of the object; when walking off an edge
   that continues onto an adjacent object at the same height, stay on.
*/
/* DS 0x80037f60 (Oma_pull) */
void Oma_pull(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm)
{
	LONG            X, Z, W, D;
	LONG            X2, Z2;
	LONG            Now_at, Old_at;
	AT_DATA         *pOat;
	AT_DATA         *pOat2;
	AT_DATA         *pEat;
	OBJ_MODEL_WORK  *pOm2;
	LONG            Bak;
	LONG            WW, DD1, DD2, Flg;
	LONG            DD, WW1, WW2;
	LONG            Ep, Old_pos, Ww;
	ULONG           Frame_pad[6];		/* retail reserves 0x68 bytes and reads only sp+0x10..0x17 and sp+0x28 */
	SVECTOR         Dead_vec = { 200, 0, 0, 0 };	/* DS builds this from rodata and adds Atd.At_w to vx; never read */

	(void)Frame_pad;
	Dead_vec.vx += pEm->Atd->At_w;
	if ((pEm->Be_flg & 0x40) != 0)
		return;
	if ((pOm->Be_flg & 2) != 0)
		return;

	pEat = pEm->Atd;
	pEm->pOn_om = Oma_get_on_om(pEm, pEat);
	pOat = pOm->Atd;
	if (pEm->pOn_om != pOm)
		return;

	X  = pOat->X;
	Z  = pOat->Z;
	W  = -(LONG)(pEat->W >> 1) + pOat->W;
	D  = -(LONG)(pEat->W >> 1) + pOat->D;
	X2 = X - pEat->X;
	Z2 = Z - pEat->Z;

	/* bit0: off the top on X, bit1: off the top on Z (now / last frame) */
	Now_at = (ULONG)(W * 2) < (ULONG)(X2 + W);
	if ((ULONG)(D * 2) < (ULONG)(Z2 + D))
		Now_at |= 2;
	Old_at = (ULONG)(W * 2) < (ULONG)((X - (pEm->Old_x2 + pEat->Ofs_x)) + W);
	if ((ULONG)(D * 2) < (ULONG)((Z - (pEm->Old_z2 + pEat->Ofs_z)) + D))
		Old_at |= 2;

	/* walked off on X: look for a neighbouring object of the same top height along X */
	if ((Now_at & 1) != 0)
	{
		for (pOm2 = G.Ob_model; pOm2 < G.pObj_work_end; pOm2++)
		{
			pOat2 = pOm2->Atd;
			if ((pOm2 == pOm) || ((pOm2->Be_flg & 0x101) != 0x101) || ((pOm2->Be_flg & 2) != 0))
				continue;
			if (pOat->Y - pOat->At_h != pOat2->Y - pOat2->At_h)
				continue;

			X  = pOat->Z;
			WW = ((ULONG)pOat->D + pOat2->D) - pEat->W;
			if ((ULONG)((pOat2->Z - X) + WW) >= (ULONG)(WW * 2))
				continue;
			Ep = pEat->Z;
			if ((ULONG)((pOat2->Z - Ep) + pOat2->D) > (ULONG)(pOat2->D << 1))
				continue;
			Ww = (ULONG)pOat->W + pOat2->W + 200;
			if ((ULONG)((pOat2->X - pOat->X) + Ww) > (ULONG)(Ww * 2))
				continue;
			if ((LONG)((pOat->X - pOat2->X) ^ X2) <= 0)
				continue;

			DD1 = -(LONG)(pEat->D >> 1) + pOat->D;
			DD2 = -(LONG)(pEat->D >> 1) + pOat2->D;
			Flg = (ULONG)((X - Ep) + DD1) < (ULONG)(DD1 * 2);
			if ((ULONG)((pOat2->Z - Ep) + DD2) < (ULONG)(DD2 * 2))
				Flg |= 2;

			if (Flg == 3)
			{
				Old_pos = pEm->Old_z2 + pEat->Ofs_z;
				Now_at  = (ULONG)(DD1 * 2) < (ULONG)((X - Old_pos) + DD1);
				if ((ULONG)(DD2 * 2) < (ULONG)((pOat2->Z - Old_pos) + DD2))
					Now_at |= 2;
				if (Now_at == 0)
					return;
				Oma_pull_hosei_x(pEm, pOat, X2, W);
				return;
			}
			if ((Old_at == 0) && ((Flg & 1) != 0))
			{
				Oma_pull_hosei_x(pEm, pOat, X2, W);
				return;
			}
			Now_at = 2;
			if (Flg == 1)
			{
				Oma_pull_hosei_z(pEm, pOat2, pOat2->Z - Ep, D);
				return;
			}
			Old_pos = pEm->Old_z2 + pEat->Ofs_z;
			if (((ULONG)((X - Old_pos) + DD1) < (ULONG)(DD1 * 2)) &&
				((ULONG)((pOat2->Z - Old_pos) + DD2) < (ULONG)(DD2 * 2)))
			{
				Oma_pull_hosei_z(pEm, pOat, Z2, D);
				return;
			}
			if ((LONG)(Z2 ^ (X - pOat2->Z)) < 0)
				Oma_pull_hosei_x(pEm, pOat, X2, W);
		}
	}

	/* walked off on Z: look for a neighbouring object of the same top height along Z */
	if ((Now_at & 2) != 0)
	{
		for (pOm2 = G.Ob_model; pOm2 < G.pObj_work_end; pOm2++)
		{
			pOat2 = pOm2->Atd;
			if ((pOm2 == pOm) || ((pOm2->Be_flg & 0x101) != 0x101) || ((pOm2->Be_flg & 2) != 0))
				continue;
			if (pOat->Y - pOat->At_h != pOat2->Y - pOat2->At_h)
				continue;

			X  = pOat->X;
			DD = ((ULONG)pOat->W + pOat2->W) - pEat->W;
			if ((ULONG)((pOat2->X - X) + DD) >= (ULONG)(DD * 2))
				continue;
			Ep = pEat->X;
			if ((ULONG)((pOat2->X - Ep) + pOat2->W) > (ULONG)(pOat2->W << 1))
				continue;
			Z  = pOat->Z;
			Ww = (ULONG)pOat->D + pOat2->D + 200;
			if ((ULONG)((pOat2->Z - Z) + Ww) > (ULONG)(Ww * 2))
				continue;
			if ((LONG)((Z - pOat2->Z) ^ Z2) <= 0)
				continue;

			WW1 = -(LONG)(pEat->W >> 1) + pOat->W;
			WW2 = -(LONG)(pEat->W >> 1) + pOat2->W;
			Flg = (ULONG)((X - Ep) + WW1) < (ULONG)(WW1 * 2);
			if ((ULONG)((pOat2->X - Ep) + WW2) < (ULONG)(WW2 * 2))
				Flg |= 2;

			if (Flg == 3)
			{
				Old_pos = pEm->Old_x2 + pEat->Ofs_x;
				Now_at  = (ULONG)((X - Old_pos) + WW1) < (ULONG)(WW1 * 2);
				if ((ULONG)((pOat2->X - Old_pos) + WW2) < (ULONG)(WW2 * 2))
					Now_at |= 2;
				if (Now_at == 3)
					return;
				Oma_pull_hosei_z(pEm, pOat, Z2, D);
				return;
			}
			Now_at = 1;
			if ((Old_at == 0) && ((Flg & 1) != 0))
			{
				Oma_pull_hosei_z(pEm, pOat, Z2, D);
				return;
			}
			if (Flg == 1)
			{
				Oma_pull_hosei_x(pEm, pOat2, pOat2->X - Ep, W);
				return;
			}
			Old_pos = pEm->Old_x2 + pEat->Ofs_x;
			if (((ULONG)((X - Old_pos) + WW1) < (ULONG)(WW1 * 2)) &&
				((ULONG)((pOat2->X - Old_pos) + WW2) < (ULONG)(WW2 * 2)))
			{
				Oma_pull_hosei_x(pEm, pOat, X2, W);
				return;
			}
			if ((LONG)(X2 ^ (X - pOat2->X)) < 0)
				Oma_pull_hosei_z(pEm, pOat, Z2, D);
		}
	}

	/* just stepped off an edge: pull back onto the top */
	if ((Now_at & (Old_at ^ Now_at) & 1) != 0)
	{
		Bak = pEm->Pos_x;
		if (X2 >= 0)
			W = -W;
		pEm->Pos_x = (pOat->X - pEat->Ofs_x) + W;
		pEat->X   += pEm->Pos_x - Bak;
	}
	if ((Now_at & (Old_at ^ Now_at) & 2) != 0)
	{
		Bak = pEm->Pos_z;
		if (Z2 >= 0)
			D = -D;
		pEm->Pos_z = (pOat->Z - pEat->Ofs_z) + D;
		pEat->Z   += pEm->Pos_z - Bak;
	}
	pEm->pOn_om = Oma_get_on_om(pEm, pEat);
}

/* DS 0x80038664 (Oma_pull_hosei_x) */
void Oma_pull_hosei_x(ENEMY_WORK *pEm, AT_DATA *pOat, LONG X2, LONG W)
{
	LONG    Bak;
	AT_DATA *pEat;

	pEat = pEm->Atd;
	Bak  = pEm->Pos_x;
	if (X2 < 0)
		pEm->Pos_x = (pOat->X - pEat->Ofs_x) + W;
	else
		pEm->Pos_x = (pOat->X - pEat->Ofs_x) - W;
	pEat->X += pEm->Pos_x - Bak;
	pEm->pOn_om = Oma_get_on_om(pEm, pEat);
}

/* DS 0x800386d0 (Oma_pull_hosei_z) */
void Oma_pull_hosei_z(ENEMY_WORK *pEm, AT_DATA *pOat, LONG Z2, LONG D)
{
	LONG    Bak;
	AT_DATA *pEat;

	pEat = pEm->Atd;
	Bak  = pEm->Pos_z;
	if (Z2 < 0)
		pEm->Pos_z = (pOat->Z - pEat->Ofs_z) + D;
	else
		pEm->Pos_z = (pOat->Z - pEat->Ofs_z) - D;
	pEat->Z += pEm->Pos_z - Bak;
	pEm->pOn_om = Oma_get_on_om(pEm, pEat);
}

/* Oma_get_on_om - 0x8003873c
   Object whose top pEm is standing on (keeps the current one if none found).
*/
/* DS 0x8003873c (Oma_get_on_om) */
OBJ_MODEL_WORK *Oma_get_on_om(ENEMY_WORK *pEm, AT_DATA *pEat)
{
	OBJ_MODEL_WORK  *pOm2;
	LONG            W, D;
	LONG            Flg;

	for (pOm2 = G.Ob_model; pOm2 < G.pObj_work_end; pOm2++)
	{
		if (((pOm2->Be_flg & 0x101) == 0x101) &&
			((pOm2->Be_flg & 2) == 0) &&
			(pEm->Pos_y == pOm2->Atd[0].Y - pOm2->Atd[0].At_h))
		{
			W   = -(LONG)(pEat->W >> 1) + pOm2->Atd[0].W;
			D   = -(LONG)(pEat->W >> 1) + pOm2->Atd[0].D;
			Flg = (ULONG)(W * 2) < (ULONG)((pOm2->Atd[0].X - pEat->X) + W);
			if ((ULONG)(D * 2) < (ULONG)((pOm2->Atd[0].Z - pEat->Z) + D))
				Flg |= 2;
			if (Flg == 0)
				return pOm2;
		}
	}
	return pEm->pOn_om;
}

/* Oma_ob_pull2 - 0x80038818
   Drag pEm back toward the top of pOm by at most Spd per axis.
*/
/* DS 0x80038818 (Oma_ob_pull2) */
void Oma_ob_pull2(ENEMY_WORK *pEm, OBJ_MODEL_WORK *pOm, ULONG Id, ULONG Spd)
{
	LONG    X, Z, W, D;
	LONG    Flg;
	ULONG   Spd_x;

	if ((pEm->Be_flg & 0x40) != 0)
		return;
	if ((pOm->Be_flg & 2) != 0)
		return;

	X = pOm->Atd[0].X - pEm->Atd[0].X;
	Z = pOm->Atd[0].Z - pEm->Atd[0].Z;
	W = -(LONG)(pEm->Atd[0].W >> 1) + pOm->Atd[0].W;
	D = -(LONG)(pEm->Atd[0].W >> 1) + pOm->Atd[0].D;
	Flg = (ULONG)(W * 2) < (ULONG)(X + W);
	if ((ULONG)(D * 2) < (ULONG)(Z + D))
		Flg |= 2;
	if (Flg == 0)
		return;

	if ((Flg & 1) != 0)
	{
		Spd_x = Spd;
		if ((ULONG)(((X < 0) ? -X : X) - W) < Spd)
			Spd_x = ((X < 0) ? -X : X) - W;
		if (X >= 0)
			Spd_x = -Spd_x;
		pEm->Pos_x    -= Spd_x;
		pEm->Atd[0].X -= Spd_x;
	}
	if ((Flg & 2) != 0)
	{
		if ((ULONG)(((Z < 0) ? -Z : Z) - D) < Spd)
			Spd = ((Z < 0) ? -Z : Z) - D;
		if (Z >= 0)
			Spd = -Spd;
		pEm->Pos_z    -= Spd;
		pEm->Atd[0].Z -= Spd;
	}
}

/* Omd_in_check - 0x80038950
   Point (with radius R) inside the object's box? Flg != 0 also checks height.
*/
/* DS 0x80038950 (Omd_in_check) */
ULONG Omd_in_check(VECTOR *pVec, OBJ_MODEL_WORK *pOm, LONG R, ULONG Flg)
{
	LONG    a, b;

	a = (ULONG)pOm->Atd[0].W + R;
	b = (ULONG)pOm->Atd[0].D + R;
	if ((ULONG)((pVec->vx - pOm->Atd[0].X) + a) > (ULONG)(a * 2))
		return 0;
	if ((ULONG)((pVec->vz - pOm->Atd[0].Z) + b) > (ULONG)(b * 2))
		return 0;
	if (Flg == 0)
		return 1;

	a = pOm->Atd[0].At_h - 1;
	if ((a >= 0) && ((ULONG)((pVec->vy - pOm->Atd[0].Y) + a) <= (ULONG)(a * 2 + 1)))
		return 1;
	return 0;
}

/* Oma_obj_ck_all - 0x800389e8
   Would pEm placed at pPos touch any other live enemy?
*/
/* DS 0x800389e8 (Oma_obj_ck_all) */
ULONG Oma_obj_ck_all(VECTOR *pPos, ENEMY_WORK *pEm)
{
	ENEMY_WORK  *pEm2;
	ENEMY_WORK  **ppEm;

	if ((pEm->Be_flg & 2) != 0)
		return 0;
	ppEm = G.pEnemy;
	if (ppEm == G.ppEnemy_end)
		return 0;
/* goto loop: a do/while form compiles to a larger frame in this build */
enemy_loop:
	pEm2 = *ppEm;
	if ((pEm != pEm2) && ((pEm2->Be_flg & 1) != 0) && (Oba_ck_only(pPos, pEm, pEm2) != 0))
		return 1;
	ppEm++;
	if (ppEm != G.ppEnemy_end)
		goto enemy_loop;
	return 0;
}

/* Oma_hit_ck_all - 0x80038a98
   First object model whose box contains pV (with pEm's size).
*/
/* DS 0x80038a98 (Oma_hit_ck_all) */
OBJ_MODEL_WORK *Oma_hit_ck_all(VECTOR *pV, ENEMY_WORK *pEm)
{
	OBJ_MODEL_WORK  *pOm;
	AT_DATA         *pEat;
	LONG            X, Z;
	LONG            W, H, D, R;

	if ((pEm->Be_flg & 0x40) != 0)
		return NULL;

	pEat = pEm->Atd;
	R    = pEat->W;
	for (pOm = G.Ob_model; pOm < G.pObj_work_end; pOm++)
	{
		if ((pOm->Be_flg & 2) != 0)
			continue;
		/* NOTE: the binary compares the object's X (not Y) against pV->vy here */
		X = pOm->Atd[0].X;
		H = (ULONG)pEat->At_h + pOm->Atd[0].At_h;
		if ((ULONG)((X - pV->vy) + H) >= (ULONG)(H * 2))
			continue;
		W = R + pOm->Atd[0].W;
		D = R + pOm->Atd[0].D;
		Z = pOm->Atd[0].Z - pV->vz;
		if (((ULONG)((X - pV->vx) + W) < (ULONG)(W * 2)) &&
			((ULONG)(Z + D) < (ULONG)(D * 2)))
			return pOm;
	}
	return NULL;
}

/* Om_Jump_ck - 0x80038b84
   Stubbed in DS: only initialises the local jump vector.
*/
/* DS 0x80038b84 (Om_Jump_ck) */
void Om_Jump_ck(ENEMY_WORK *pEm)
{
	/* DS 0x80038b84: retail body is empty but reserves a 0x28-byte frame. Stack 0x10-0x17
	   receives the 8-byte table {200,0} copied from 0x80010b9c; nothing reads it. */
	LONG  Frame_pad0[4];
	SHORT Frame_tbl[4] = {200, 0};
	LONG  Frame_pad1[4];

	(void)Frame_pad0;
	(void)Frame_tbl;
	(void)Frame_pad1;
}
