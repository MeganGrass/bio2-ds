/* em2b.c - reconstruction of D:\Bio2\prog\EM2B.C, the EM2B enemy module.  One source,
 * linked into two runtime overlays: 0x10 (enemy bin slot 0, 0x80100108..) and 0x2E
 * (slot 1, 0x8010D108..); the gate compiles it once per overlay with -DOVL_10 /
 * -DOVL_2E, which only selects the exported names (em2b_sub.h).  Retail EM2B.C
 * #includes EM2B_SUB.C ahead of its own tables and functions (one object, two SLD
 * files).  Types come from the object's retail SYM preamble (em2b.h); the tables are
 * the retail initialized data.  Engine routines are called undeclared unless the
 * retail SYM shows no implicit-declaration block at their calls (em2b_sub.h). */
#include "EM2B.H"

/* forward declarations: the retail SYM lists these objects first-declared here (defined below). */
static void (*Em2b_routine_0[4])();
static void (*Em2b_mv_br[9])();
static void (*Em2b_mv[9])();
static void (*Em2b_damage[3])();
static void (*Em2b_die[1])();
static void (*Em2b_pldm_tbl[3])();
static DM_ESP Blood_tbl[36];
#include "EM2B_SUB.C"

/* retail-only life values (0x80104C38..0x80104C98): the same two blocks the retail EM22.C data holds
 * (Em23/Em2a/Em2b life; names from Beta 2 EM22.C for the first block, addresses for the second).
 * Em2b_Init reads the Em2b pairs. */
static unsigned short Em23_life = 300;
static unsigned short Em23_life_easy = 300;
static unsigned short Em2a_life[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short Em2a_life_easy[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short Em2b_life = 250;
static unsigned short Em2b_life_easy = 200;
static unsigned short D_80104C68 = 240;
static unsigned short D_80104C6A = 240;
static unsigned short D_80104C6C[10] = { 250, 450, 300, 300, 220, 300, 400, 300, 400, 350 };
static unsigned short D_80104C80[10] = { 180, 340, 220, 240, 175, 240, 310, 240, 310, 300 };
static unsigned short D_80104C94 = 250;
static unsigned short D_80104C96 = 200;

static void (*Em2b_routine_0[4])() = {
	Em2b_Init,
	Em2b_Move,
	Em2b_Damage,
	Em2b_Die
};
static void (*Em2b_mv_br[9])() = {
	Em2b_taiki_br,
	Em2b_walk_br,
	Em2a_att_t_br,
	Em2a_att_w_br,
	Em2b_dash_br,
	Em2a_att_t2_br,
	Em2a_where_br,
	Em2a_critical_br,
	Em2a_opening_br
};
static void (*Em2b_mv[9])() = {
	Em2b_taiki,
	Em2b_walk,
	Em2a_att_t,
	Em2a_att_w,
	Em2b_dash,
	Em2a_att_t2,
	Em2a_where,
	Em2a_critical,
	Em2a_opening
};
static void (*Em2b_damage[3])() = {
	Em2b_dm_no,
	Em2b_dm_f,
	Em2b_dm_b
};
static void (*Em2b_die[1])() = {
	Em2b_dead
};
static void (*Em2b_pldm_tbl[3])() = {
	Em2b_pl_die,
	Em2b_pl_dm_f,
	Em2b_pl_dm_b
};
/* retail-only (retail s1 0x801017E8 / s0 0x8010E7E8, 37 words; no Beta twin, name unknown):
 * puts the current cut's third light on the enemy (flame light; brighter when G.Cbuf_id).
 * Called from Em12b while EF->Retail_1e is set. */
static void func_801017E8(ENEMY_WORK *pEm)
{
	register LIGHT_CUT_DATA *pLc = &G.pRoom->pLight[ G.SaveData.Cut_no ];

	if (G.Cbuf_id) {
		pLc->Col[2][0] = 200;
		pLc->Col[2][1] = 95;
		pLc->Col[2][2] = 0;
		pLc->L[2] = 10000;
	}else {
		pLc->Col[2][0] = 170;
		pLc->Col[2][1] = 70;
		pLc->Col[2][2] = 0;
		pLc->L[2] = 7000;
	}
	pLc->Mode[2] = 0;
	pLc->Pos[2][0] = pEm->Pos_x;
	pLc->Pos[2][1] = pEm->Pos_y - 0x1194;
	pLc->Pos[2][2] = pEm->Pos_z;
}
void Em12b(ENEMY_WORK *pEm)
{
	register short Dir;

	Root_ck(pEm, &G.Player.Pos_x, 0, 0);

	if (!(G.Stop_flg & 0x20000000)) {

		if (pEm->Damage_cnt & 0x7F) pEm->Damage_cnt--;

		if (G.Player.Type == 0x11)
			pEm->Damage_cnt |= 0x80;
		else
			pEm->Damage_cnt &= 0x7F;

		if (EF(pEm)->Attribute & 2) {
			if ((G.Status_flg & 0x40000) && Tama_ck_i(G.Player.Type & 0xFFF)) {
				G.Status_flg &= 0xFFFBFFFF;

				if (!(pEm->Type & 0x4000)) {
					Dir = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

					Dir = (Dir - G.Player.Cdir_y) & 0xFFF;
					if (Dir > 0x800) Dir |= 0xF000;

					if ((G.Key & 0x40) && abs(Dir) < 300) {	/* retail: was 200 */
						*(unsigned long *)&pEm->Routine_0 = 3;
						G.Key &= ~0x40;
					}
				}
			}
		}

		pEm->Sce_free3 = 0;

		Em2b_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);

		if (!(EF(pEm)->Attribute & 4)) Breast_move(pEm);
		if (EF(pEm)->Retail_1e) func_801017E8(pEm);	/* retail */

		Em_seq_sound(pEm);

		if (EF(pEm)->Dm_eff_func) ((void (*)())EF(pEm)->Dm_eff_func)(pEm);
		if (EF(pEm)->At_eff_func) ((void (*)())EF(pEm)->At_eff_func)(pEm);

		Oba_ck_em(pEm);
		Sca_ck_em(pEm, 0x400);

		if (pEm->Life < 20000) pEm->Sce_free2 = 1;
		if (pEm->Life < 10000) pEm->Life = 10000;

		if ((signed char)EF(pEm)->At_wait > 0) EF(pEm)->At_wait--;

		EF(pEm)->Escape_cnt++;
	}

}
static void Em2b_Init(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	G.pPldamage_Em[0x2B - 0x10] = Em12b_Pldm;

	*(unsigned long *)&pEm->Routine_0 = 0x801;

	*(long *)&pEm->Move_no = 0;

	Joint_move(pEm, Kan, Seq, 0x1000);
	pEm->Neck_no = 5;	/* retail */

	pEm->Be_flg |= 0x1C000000;
	pEm->Damage_flg = 0;
	pEm->Sce_flg = 0;
	pEm->Down_cnt = 0;
	if (G.System_flg & 0x40) {	/* retail: life by mode (Beta 2: 0x4F1A) */
		if (G.System_flg & 0x20)
			pEm->Life = D_80104C94 + 20000;
		else
			pEm->Life = D_80104C96 + 20000;
	}else {
		if (G.System_flg & 0x20)
			pEm->Life = Em2b_life + 20000;
		else
			pEm->Life = Em2b_life_easy + 20000;
	}

	EF(pEm)->Attribute = 2;
	EF(pEm)->Breast_Rno = 0;
	EF(pEm)->Dm_eff_rtn = 0;
	EF(pEm)->At_eff_rtn = 0;
	EF(pEm)->Dm_eff_func = 0;
	EF(pEm)->At_eff_func = 0;
	EF(pEm)->At_wait = 0;
	EF(pEm)->Escape_cnt = 0;
	EF(pEm)->Retail_1e = 0;	/* retail */

	pEm->Sce_free0 = 0;
	pEm->Sce_free1 = 0;
	pEm->Sce_free2 = 0;
	pEm->Sce_free3 = 0;

	pEm->nOba = 1;
	pEm->Atd[0].Ofs_x = 0;
	pEm->Atd[0].Ofs_y = -1800;
	pEm->Atd[0].Ofs_z = 0;
	pEm->Atd[0].At_w = pEm->Atd[0].W = 1000;
	pEm->Atd[0].At_h = 1800;
	pEm->Atd[0].At_d = pEm->Atd[0].D = 1000;
	pEm->Eff_at_r = 800;

	Set_flame(pEm);

	Kage_work_set(&pEm->pKage_work, 0, 0x2EE0226, 0, &pEm->Pos_x);

}
static void Em2b_Move(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em2b_mv_br[pEm->Routine_1](pEm);
	Em2b_mv[pEm->Routine_1](pEm, Kan, Seq);

	Mot_play_init(pEm, EF(pEm)->Rno_bak);
	EF(pEm)->Rno_bak = *(unsigned long *)&pEm->Routine_0;
}
static void Em2b_taiki_br(ENEMY_WORK *pEm) {}
static void Em2b_taiki(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (!pEm->Routine_2) {
		;	/* code-less statement: retail SLD opens the block on this line */
		*(long *)&pEm->Move_no = 0xA05;
		Joint_move(pEm, Kan, Seq, 0x1000);

	}
}
static void Em2b_walk_br(ENEMY_WORK *pEm) {}
static void Em2b_walk(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir, Check_dis;

	Dir = Get_dir_pl(pEm);

	if (pEm->L_pl > 2800) pEm->Sce_free3 = 1;

	if (G.Player.Life < 0) {

		*(unsigned long *)&pEm->Routine_0 = 0x601; return;
	}

	if (EF(pEm)->Escape_cnt > 220 && abs(Dir) < 300 && pEm->L_pl > 3500) {
		*(unsigned long *)&pEm->Routine_0 = 0x401;
		EF(pEm)->Escape_cnt = 0;
		goto move;
	}

	if (G.Player.Life >= 0 && !EF(pEm)->At_wait) {

		Check_dis = 0x8FC; if ((Rnd() & 0x3F) == 1) Check_dis = 0xA28;

		if (G.Player.Life < 81) Check_dis = 0x76C;

		if (pEm->L_pl < Check_dis) {

			if (G.Player.Life < 41) {
				*(unsigned long *)&pEm->Routine_0 = 0x301; goto move;
			}

			if (abs(Dir) < 400) {

				if (EF(pEm)->Old_dir < Dir && (Rnd() & 3) < 3) {
					*(unsigned long *)&pEm->Routine_0 = 0x301; goto move;
				}
				if (Dir < 140 && (Rnd() & 3) < 3) {
					*(unsigned long *)&pEm->Routine_0 = 0x201; goto move; }
				else
					*(unsigned long *)&pEm->Routine_0 = 0x301;

				goto move;
			}
		}

		Check_dis = 0x1964; if ((Rnd() & 0x3F) == 0x17) Check_dis = 0x157C;

		if (Check_dis < pEm->L_pl && pEm->L_pl < 7500 && abs(Dir) < 300)
			if (!EF(pEm)->At_wait) {

				*(unsigned long *)&pEm->Routine_0 = 0x401; goto move;
			}

		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
	}

	move: if (!pEm->Routine_2) {
		;	/* code-less statement: retail SLD opens the block on this line */
		Mot_play_sp(pEm, 1, 15, 0);
		Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);

	}

	EF(pEm)->Old_dir = Dir;
}
static void Em2a_att_t_br(ENEMY_WORK *pEm) {}
static void Em2a_att_t(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;
	SVECTOR Svec = { 100, 0, 200, 0 };
	VECTOR Vec;

	Dir = Get_dir_pl(pEm);

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Move_cnt = 0;
		pEm->Routine_2++;

	case 1:
		if (Em2b_att_ck2(pEm, 10, 14, 19, 200, G.Player.Atd[0].At_w)) {
			Pad_vib_set0(8, 0);	/* retail: pad vibration */
			Pad_vib_fade_set(0xD, 0xC8, 0x28, 2);
			Pl_dm_set(pEm);
			Snd_se_enem(5, pEm);
			EF(pEm)->At_wait = 30;
			if (!(EF(pEm)->Attribute & 8)) {
				EF(pEm)->Escape_cnt = 0;
				Em2b_add_dm(15);
			}
		}

		pEm->Routine_2 += Mot_play_sp(pEm, 2, 15, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);

		if (EF(pEm)->Attribute & 8) {
			G.Stop_flg |= 0x1000000;
			if (pEm->Timer0)
				if (Dir > -150 && Dir < 300 && pEm->L_pl < 1800) {
					Goto00((ENEMY_WORK *)&G.Player, pEm->Pos_x, pEm->Pos_z, 0x80);

					Rot_vector(pEm->Cdir_y, &Svec, &Svec);
					Vec.vx = pEm->Pos_x + Svec.vx;
					Vec.vz = pEm->Pos_z + Svec.vz;
					Approach(&G.Player.Pos_x, &Vec, 150);
				}
		}

		if (pEm->Move_cnt == 20 && pEm->Timer0 && G.Player.Life >= 0)
			if (Dir > -150 && Dir < 300 && pEm->L_pl < 1800)
				*(unsigned long *)&pEm->Routine_0 = 0x501;

		if (pEm->Move_cnt == 20 && (EF(pEm)->Attribute & 8))
			*(unsigned long *)&pEm->Routine_0 = 0x501;

		break;

	case 2:
		*(unsigned long *)&pEm->Routine_0 = 0x101;

	}
}
static void Em2a_att_w_br(ENEMY_WORK *pEm) {}
static void Em2a_att_w(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	static unsigned long R_old_x, R_old_z, L_old_x, L_old_z, Tmp_x, Tmp_z;
	register unsigned long Tmp;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Move_cnt = 0;
		pEm->Routine_2++;
	case 1:
		EF(pEm)->Pold_x = L_old_x;
		EF(pEm)->Pold_z = L_old_z;

		Tmp = Em2b_att_ck2(pEm, 10, 15, 19, 170, G.Player.Atd[0].At_w * 3);

		Tmp_x = EF(pEm)->Pold_x;
		Tmp_z = EF(pEm)->Pold_z;

		EF(pEm)->Pold_x = R_old_x;
		EF(pEm)->Pold_z = R_old_z;

		Tmp |= Em2b_att_ck2(pEm, 16, 15, 19, 170, G.Player.Atd[0].At_w * 3) << 1;
		if (Tmp) {
			Pad_vib_set0(5, 0);	/* retail: pad vibration */
			Pad_vib_fade_set(0x12, 0xC8, 0x28, 1);
			Em2b_add_dm(25);	/* retail: was 15 */
			Snd_se_enem(5, pEm);
			EF(pEm)->Escape_cnt = 0;
			EF(pEm)->At_wait = 48;

			if (G.Player.Life < 0) {
				EF(pEm)->Attribute |= 8;
				G.System_flg |= 0x4000000;
				G.System_flg |= 0x100;
				*(unsigned long *)&pEm->Routine_0 = 0x701;
				G.Stop_flg |= 0x1000000;
				Pl_dm_set(pEm);
			}else
			if (Tmp & 1)
				Pl_dm_set_push(pEm, L_old_x, L_old_z, 10);
			else
				Pl_dm_set_push(pEm, R_old_x, R_old_z, 16);

		}
		L_old_x = Tmp_x;
		L_old_z = Tmp_z;
		R_old_x = EF(pEm)->Pold_x;
		R_old_z = EF(pEm)->Pold_z;

		if (G.Player.Life > 80)
			if (pEm->Move_cnt >= 10 && pEm->Move_cnt < 13 && pEm->L_pl > 2900) {
				*(unsigned long *)&pEm->Routine_0 = 0x101; break;
			}

		if (pEm->Move_cnt == 30 && !pEm->Timer0) {
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
			*(unsigned long *)&pEm->Routine_0 = 0x101;
			break;
		}

		pEm->Routine_2 += Mot_play_sp(pEm, 4, 15, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
		break;
	case 2:
		*(unsigned long *)&pEm->Routine_0 = 0x101;

	}
}
static void Em2b_dash_br(ENEMY_WORK *pEm) {}
static void Em2b_dash(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;

	switch (pEm->Routine_2) {
	case 0:
		EF(pEm)->Attribute |= 1;
		pEm->Timer0 = 0;
		pEm->Move_cnt = 0;
		pEm->Routine_2++;

	case 1:

		if (pEm->Move_cnt < 27 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 700)) {

			EF(pEm)->Attribute &= 0xFFFE;

			pEm->Move_cnt = 27;
			pEm->Hokan_flg = 0;
			Spd_get(pEm, Kan, Seq, 0);
		}

		if (pEm->Move_cnt >= 22 && pEm->Move_cnt < 26) {
			Svec.vx = 0;
			Svec.vy = 0;
			Svec.vz = 0;

			Set_esp_w(pEm, 1, 6, 0x800, pEm->Cdir_y, 9, &Svec);
			Set_esp_w(pEm, 1, 6, 0x800, pEm->Cdir_y, 10, &Svec);
			Set_esp_w(pEm, 1, 6, 0x800, pEm->Cdir_y, 11, &Svec);
		}

		if (Mot_play_sp(pEm, 8, 15, 0)) {
			EF(pEm)->Attribute &= 0xFFFE;
			*(unsigned long *)&pEm->Routine_0 = 0x101;
		}

		if (Em2b_att_ck2(pEm, 10, 27, 33, 400, G.Player.Atd[0].At_w * 2)) {

			Pl_dm_set_push(pEm, EF(pEm)->Pold_x, EF(pEm)->Pold_z, 10);
			Snd_se_enem(5, pEm);
			EF(pEm)->Escape_cnt = 0;
			Pad_vib_set0(4, 0);	/* retail: pad vibration */
			Pad_vib_fade_set(0x12, 0xDC, 0x28, 1);
			Em2b_add_dm(30);	/* retail: was 15 */
		}

		if (pEm->Move_cnt < 20)
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x40);
		else
			Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);

	}
}
static void Em2a_att_t2_br(ENEMY_WORK *pEm) {}
static void Em2a_att_t2(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	register PLAYER_WORK *pPl = &G.Player;

	SVECTOR Svec = { 900, 0, -180, 0 };
	static VECTOR Vec;

	switch (pEm->Routine_2) {
	case 0:

		pEm->Timer0 = 0;

		Rot_vector(pEm->Cdir_y, &Svec, &Svec);
		Vec.vx = pEm->Pos_x + Svec.vx;
		Vec.vz = pEm->Pos_z + Svec.vz;

		pEm->Routine_2++;
	case 1:

		if (Em2b_att_ck2(pEm, 10, 6, 10, 200, pPl->Atd[0].At_w)) {
			Pl_dm_set(pEm);
			Pad_vib_set0(3, 0);	/* retail: pad vibration */
			Pad_vib_fade_set(0xC, 0xB4, 0x28, 0);
			Snd_se_enem(5, pEm);
			if (!(EF(pEm)->Attribute & 8)) Em2b_add_dm(10);
		}

		if (pEm->Move_cnt == 15 && !(EF(pEm)->Attribute & 8)) {

			*(unsigned long *)&pEm->Routine_0 = 0x101;
			EF(pEm)->At_wait = 30;
		}

		Mot_play_sp(pEm, 9, 3, 0);
		Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);

		if (EF(pEm)->Attribute & 8) Approach(&pPl->Pos_x, &Vec, 200);
		SquareRoot0((pEm->Pos_x - Vec.vx) * (pEm->Pos_x - Vec.vx) + (pEm->Pos_z - Vec.vz) * (pEm->Pos_z - Vec.vz));

		if (pEm->Move_cnt == 41 && (EF(pEm)->Attribute & 8)) {
			pEm->Move_cnt = 10;
			pEm->Routine_2++;
		} break;

	case 2:

		if (pEm->Move_cnt == 11) {
			Pad_vib_set0(6, 2);	/* retail: pad vibration */
			Pad_vib_fade_set(0x1E, 0xF0, 0x78, 1);
			Pad_vib_set1(0x32, 0x78, 0x1E);
			Pad_vib_fade_set(0x14, 0xFF, 0x64, 0x60);
			Pad_vib_fade_set(0x11, 0xB4, 0x32, 0x74);
			*(unsigned long *)&G.Player.Routine_0 = 5;
			G.Player.pEnemy_ptr = (unsigned long *)pEm;
			G.Player.Cdir_y = pEm->Cdir_y + 0x800;
			G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
			G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
			Set_base_pos(pPl, Kan, Seq, 0);

			Color_chg(pEm, 8, 0xD0, 0x50, 0x50);
			Color_chg(pEm, 9, 0xD0, 0x50, 0x50);
			Color_chg(pEm, 10, 0xD0, 0x50, 0x50);
			Color_chg(pEm, 11, 0xD0, 0x50, 0x50);
		}

		pEm->Move_no = 3;
		if (Mot_play(pEm, 3, 3, 0)) {
			if (EF(pEm)->Attribute & 8)
				*(unsigned long *)&pEm->Routine_0 = 0x601;
			else
				*(unsigned long *)&pEm->Routine_0 = 0x101;
		}
		if (pEm->Move_cnt >= 12)
			Null_pos_set(pEm, Kan, Seq, 0);
		else
			Foot_set(pEm, 0);

	}
}
static void Em2a_where_br(ENEMY_WORK *pEm) {}
static void Em2a_where(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register short Dir;

	Dir = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);

	if (!pEm->Routine_2) {
		;	/* code-less statement: retail SLD opens the block on this line */
		if (G.Cbuf_id) Mot_play_sp(pEm, 1, 3, 0);
		Goto01(pEm, Dir, 8);
	}
}
static void Em2a_critical_br(ENEMY_WORK *pEm) {}
static void Em2a_critical(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{	/* retail: rewritten -- the player is pulled to a point 2000 in front of the enemy
	 * (Beta 2: both were placed at fixed spots once) */
	SVECTOR Svec = { 2000, 0, 0, 0 };
	VECTOR Vec;

	Rot_vector(pEm->Cdir_y, &Svec, &Svec);
	Vec.vx = pEm->Pos_x + Svec.vx;
	Vec.vz = pEm->Pos_z + Svec.vz;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;
		pEm->Be_flg |= 0xA;
		G.Player.Be_flg |= 0xA;
		pEm->Routine_2++;

	case 1:
		if (++pEm->Timer0 == 7) {
			pEm->Timer0 = 0;
			pEm->Routine_2++;
		}
		if (!pEm->Timer1) pEm->Timer1 = Approach(&G.Player.Pos_x, &Vec, 400);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100);
		Goto00((ENEMY_WORK *)&G.Player, pEm->Pos_x, pEm->Pos_z, 0x100);
		pEm->Move_cnt = 0;
		Mot_play(pEm, 2, 15, 0);
		break;

	case 2:
		if (++pEm->Timer0 == 15)
			*(unsigned long *)&pEm->Routine_0 = 0x201;

		pEm->Move_cnt = 0;
		Mot_play(pEm, 2, 15, 0);
	}
}
static void Em2a_opening_br(ENEMY_WORK *pEm) {}
static void Em2a_opening(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_2) {
	case 0:
		pEm->Move_cnt = 0;
		pEm->Type |= 0x4000;
		Mot_play_sp(pEm, 10, 0, 0);
		pEm->Timer0 = 0;	/* retail */
		pEm->Routine_2++;

	case 1:
		if (pEm->Sce_free0 == 1) {
			pEm->Type &= 0xBFFF;
			pEm->Routine_2++;
			pEm->Move_cnt = 1;
			EF(pEm)->Retail_1e = 1;	/* retail: flame light on */
		}

		break;

	case 2:
		pEm->Move_no = 10;
		if (pEm->Move_cnt < 16) {
			Mot_play_sp(pEm, 10, 0, 0);
			break; }
		if (pEm->Sce_free0 == 2) pEm->Routine_2++;

		break;

	case 3:
		pEm->Move_no = 10;
		pEm->Routine_2 += Mot_play_sp(pEm, 10, 0, 0);
		if (pEm->Sce_free0 == 3) *(unsigned long *)&pEm->Routine_0 = 0x101;
		break;

	case 4:	/* retail: hold 6 frames, then a looped motion */
		if (++pEm->Timer0 >= 6) pEm->Routine_2++;
		if (pEm->Sce_free0 == 3) *(unsigned long *)&pEm->Routine_0 = 0x101;
		break;

	case 5:
		Mot_play(pEm, 10, 3, 1);
		if (pEm->Sce_free0 == 3) *(unsigned long *)&pEm->Routine_0 = 0x101;
	}
}
static void Em2b_damage_ck(ENEMY_WORK *pEm)
{
	static unsigned long Buf0, Buf1, Buf2, Buf3;
	register unsigned long Rnd_adv;
	Rnd_adv = 0;

	EF(pEm)->Escape_cnt = 0;

	EF(pEm)->Dm_eff_rtn = 0;

	switch ((signed char)EF(pEm)->Dm_wep) {

	case 2:
	case 3:
	case 4:

		Dm_eff_blood(pEm, 1, 0, 6, 0x2000, 3);
	case 12: rnd_adv_one: switch (0) default: Rnd_adv = 1; /* [INFERRED] case12 owns the shared one-step outcome; 2..4 fall through here. */
		break;

	case 5:	/* same body as case 13 below; the compiler cross-jumps the two copies */
	case 6:
		Dm_eff_blood(pEm, 1, 0, 6, 0x2000, 3);
		break;

	case 7:
		Dm_eff_blood(pEm, 4, 0, 6, 0x2400, 3);
		Dm_eff_blood(pEm, 2, 0, 6, 0x2300, 2);
		Rnd_adv = 2;
		break;
	case 8:
		Dm_eff_blood(pEm, 4, 0, 6, 0x2600, 3);
		Dm_eff_blood(pEm, 4, 0, 6, 0x2500, 2);
		Rnd_adv = 2;
		break;
	case 9:
		Dm_eff_blood(pEm, 3, 4, 6, 0x1100, 7);
		Rnd_adv = 2;
		break;
	case 10:
		Dm_eff_blood(pEm, 5, 4, 6, 0x1100, 7);

		Rnd_adv = 1;
	case 16:	pEm->Damage_no = 0; break;	/* 10 falls into 16 */
	case 11:
		Dm_eff_blood(pEm, 3, 4, 9, 0x1100, 7);
		Rnd_adv = 2;
		break;

	case 13:
		Dm_eff_blood(pEm, 1, 0, 6, 0x2000, 3);
		break;
	case 14:
		Dm_eff_spark(pEm);
		Rnd_adv = 2;
		break;
	case 15:
		Dm_eff_0f(pEm);

		goto rnd_adv_one;

	case 1: case 17: case 19:	/* empty: retail's jump table spans 1..19 */
		break;
	case 18:
		Dm_eff_0f(pEm);
	}

	if (!(EF(pEm)->Attribute & 1)) {

		switch (Rnd_adv) {
		case 0:
			if (S_rnd(4, &Buf0, &Buf1)) {
				if (pEm->Damage_flg & 0x20)
					pEm->Damage_no = 2;
				else
					pEm->Damage_no = 1;

				return;
			}
		case 1:
			break;
		case 2:
			if (S_rnd(3, &Buf2, &Buf3)) {
				if (pEm->Damage_flg & 0x20) {
					pEm->Damage_no = 2;
					return;
				} pEm->Damage_no = 1;

				return;
			}
		}
	}

	pEm->Damage_no = 0;

}
static void Em2b_Damage(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	if (pEm->Routine_1 != 0xFF) {
		EF(pEm)->Dm_wep = pEm->Routine_1;
		pEm->Routine_1 = 0xFF;
		Em2b_damage_ck(pEm);
	}

	Em2b_damage[pEm->Damage_no](pEm, Kan, Seq); }
static void Em2b_dm_no(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	*(unsigned long *)&pEm->Routine_0 = pEm->Tmp_routine;

	if (pEm->Routine_0 == 1)
		Em2b_mv[pEm->Routine_1](pEm, Kan, Seq);
	else
		*(unsigned long *)&pEm->Routine_0 = 0x101; }
static void Em2b_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 = 1;
		pEm->Timer0 = Rnd() & 1;
	case 1:
		pEm->Routine_3 += Mot_play_sp(pEm, pEm->Timer0 + 6, 3, 0);
		Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 8);
		break;

	case 2:
		*(unsigned long *)&pEm->Routine_0 = 0x101;

	}
}
static void Em2b_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	switch (pEm->Routine_3) {
	case 0:
		pEm->Routine_3 += Mot_play_sp(pEm, 5, 15, 0);
		break;

	case 1:
		*(unsigned long *)&pEm->Routine_0 = 0x101;

	}
}
static void Em2b_Die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{

	Em2b_die[0](pEm, Kan, Seq); }
static short Bak_pos_x, Bak_pos_z, Bak_dir_y, Bak_cut_no;
static unsigned long Rand;
static void Em2b_dead(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Rstart;

	register PARTS_W *pParts = G.Player.pSin_parts_ptr;
	register PARTS_W *pEm_Parts = pEm->pSin_parts_ptr;

	switch (pEm->Routine_2) {

	case 0:

		pEm->Be_flg |= 0xA;
		pEm->Damage_cnt |= 0x80;
		EF(pEm)->Bak_x = pEm->Pos_x;
		EF(pEm)->Bak_z = pEm->Pos_z;
		pEm->Pos_x = -0x38D3;
		pEm->Pos_z = -0x392C;
		pEm->Cdir_y = 0x598;
		pEm->Timer1 = 0;
		pEm->Timer2 = 0;	/* retail */
		EF(pEm)->Attribute |= 4;
		*(long *)&pEm->Move_no = 0xA05;
		Joint_move(pEm, Kan, Seq, 0x1000);

		G.Stop_flg |= 0x80000000;
		G.Stop_flg |= 0x1000000;
		G.Status_flg |= 0x10;	/* retail: the &= 0xF7FFFFFF that followed is gone */
		Bak_pos_x = G.Player.Pos_x;
		Bak_pos_z = G.Player.Pos_z;
		Bak_dir_y = G.Player.Cdir_y;
		Bak_cut_no = G.SaveData.Cut_no;
		G.Player.Pos_x = -0x4A51;
		G.Player.Pos_y = 200;
		G.Player.Pos_z = -0x517D;
		G.Player.Cdir_y = 0x1151;

		G.Status_flg |= 0x100;
		Cut_change(11);
		Main.Bg_chg_flg = 1;

		pEm->Sce_free1 = 1;
		pEm->Timer0 = 0;
		pEm->Routine_2++;

	case 1:

		if (pEm->Timer1)
			G.Key |= 0x100;
		else {
			G.Key |= 0x140;
			pEm->Timer2 = 1;	/* retail: frame counter for the Esp_kill below */
		}
		if (pEm->Timer2 == 8) Esp_kill(2, 13, 0);	/* retail */
		if ((signed char)pEm->Timer2 > 0) pEm->Timer2++;
		if (pEm->Sce_free1 == 2) {
			pEm->Timer1 = 1;
			G.Stop_flg &= 0x7FFFFFFF;
		}
		if (pEm->Sce_free1 != 3) break;

		G.Stop_flg |= 0x80000000;
		Cut_change(8);
		Main.Bg_chg_flg = 1;

		G.Player.Be_flg |= 0x80400;
		Pl_launcher_set(pEm, 10000);
		pEm->Timer0 = 0;
		pEm->Routine_2++;

	case 2:
		Rstart.vx = 600;
		Rstart.vz = 400;
		Rstart.vy = 0;
		if (pEm->Timer0 == 1) {
			Esp_call2(0x20D1000, 0x80, &pParts[11].Workm, &Rstart);
			Snd_se_stad(0x1000001, &G.Player.Pos_x);
		}
		if (pEm->Timer0 == 15) G.Stop_flg |= 0x10000000;
		if (++pEm->Timer0 == 40) pEm->Routine_2++;
		break;

	case 3:
		G.Stop_flg &= 0xEFFFFFFF;
		Esp_kill(3, 10, 0);
		Esp_kill2(0x80);
		Cut_change(9);
		Main.Bg_chg_flg = 1;

		Pl_launcher_set(pEm, 5000);
		pEm->Timer0 = 0;
		pEm->Routine_2++;

	case 4:
		Rstart.vx = 600;
		Rstart.vz = 400;
		Rstart.vy = 0;
		if (pEm->Timer0 == 1) {
			Esp_call2(0x20D1000, 0x81, &pParts[11].Workm, &Rstart);
			Snd_se_stad(0x1000001, &G.Player.Pos_x);
		}
		if (pEm->Timer0 == 7) G.Stop_flg |= 0x10000000;
		if (++pEm->Timer0 == 32) pEm->Routine_2++;
		break;

	case 5:
		G.Stop_flg &= 0xEFFFFFFF;
		Esp_kill(3, 10, 0);
		Esp_kill2(0x81);
		Cut_change(10);
		Main.Bg_chg_flg = 1;

		Pl_launcher_set(pEm, 3000);
		pEm->Timer0 = 0;
		pEm->Routine_2++;

	case 6:
		Rstart.vx = 600;
		Rstart.vz = 400;
		Rstart.vy = 0;
		if (pEm->Timer0 == 1) {

			Esp_call2(0x270A1000, 0x82, &pParts[11].Workm, &Rstart);
			Snd_se_stad(0x1000001, &G.Player.Pos_x);
		}
		if (++pEm->Timer0 == 10) {
			pEm->Routine_2++;
			Esp_kill2(0x82);
			Pad_vib_set0(6, 3);	/* retail: pad vibration */
			Pad_vib_fade_set(0x3C, 0xFA, 0, 0);
		}
		break;

	case 7:
		Esp_kill2(0x82);

		Main.Vloop = 5;
		G.Status_flg |= 4;
		pEm->Be_flg |= 0x400;
		Rstart.vx = pEm->Pos_x;
		Rstart.vy = -1800;
		Rstart.vz = pEm->Pos_z;
		Esp_call(0x31F5000, 0, &GsIDMATRIX, &Rstart);

		Snd_se_stad(0x10A0001, &pEm->Pos_x);
		Snd_se_stad(0x20F0001, &pEm->Pos_x);

		Rand = G.Random;

		Esp_kill2(0x90);
		Esp_kill2(0x91);
		Esp_kill2(0x92);
		Esp_kill2(0x93);
		Esp_kill2(0x94);
		Esp_kill2(0x95);
		Esp_kill2(0x96);
		Esp_kill2(0x97);
		Esp_kill2(0x98);
		Esp_kill2(0x99);
		Esp_kill2(0x9A);
		Esp_kill2(0x9B);
		Esp_kill2(0x9C);
		Esp_kill2(0x9D);
		Esp_kill2(0x9E);
		Xa_play(0, 3);	/* retail */
		G.Status_flg |= 0x20;	/* retail: cleared elsewhere when the XA ends; case 8 waits for it */

		Em2b_parts_fly_init(pEm);
		Em_explosion_set(pEm, 0);
		Em_explosion_set(pEm, 3);
		Em_explosion_set(pEm, 5);
		Em_explosion_set(pEm, 7);
		Em_explosion_set(pEm, 13);
		Em_explosion_set(pEm, 20);

		pEm->Timer0 = 0;
		pEm->Type |= 0x4000;
		pEm->Routine_2++;

	case 8:

		Em2b_parts_fly_move(pEm);

		if (pEm->Timer0++ < 14) {	/* retail: the frame count moved here */
			Rstart.vx = pEm->Pos_x + (signed char)Rnd() * 20;
			Rstart.vy = (signed char)Rnd() * 20 - 2500;
			Rstart.vz = pEm->Pos_z + (signed char)Rnd() * 20;
			Esp_call(0x31F5000, 0, &GsIDMATRIX, &Rstart);
		}

		if (pEm->Timer0 == 16) {
			G.Status_flg &= ~4;
			pEm_Parts[0].Be_flg = 0;
			pEm_Parts[3].Be_flg = 0;
			pEm_Parts[5].Be_flg = 0;
			pEm_Parts[7].Be_flg = 0;
			pEm_Parts[13].Be_flg = 0;
			pEm_Parts[20].Be_flg = 0;
			Em2b_parts_smoke(pEm);
		}

		if (!(G.Status_flg & 0x20)) {	/* retail: was ++Timer0 >= 85 */
			Em_dead_flg_on(pEm);

			G.Player.Pos_x = Bak_pos_x;
			G.Player.Pos_y = 0;
			G.Player.Pos_z = Bak_pos_z;
			G.Player.Cdir_y = Bak_dir_y;
			EF(pEm)->Retail_1e = 0;	/* retail: flame light off */
			pEm->Routine_2++;
		} break;

	case 9:
		if (pEm->Sce_free1 == 4) {

			Main.Vloop = 2;

			Em2b_parts_fly_end(pEm, EF(pEm)->Bak_x, EF(pEm)->Bak_z);

			pEm->Pos_x = EF(pEm)->Bak_x;
			pEm->Pos_z = EF(pEm)->Bak_z;

			Cut_change(Bak_cut_no);
			G.Stop_flg &= 0x7FFFFFFF;
			G.Stop_flg &= 0xFEFFFFFF;
			Main.Bg_chg_flg = 1;
			G.Player.Be_flg &= 0xFFF7FBFF;	/* retail: moved here from case 8 */

			pEm->Routine_2++;
		}
	}
}
void Em12b_Pldm(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	Em2b_pldm_tbl[pEm->Routine_1](pEm, Kan, Seq); }
static void Em2b_pl_die(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	SVECTOR Svec;
	register PARTS_W *pParts = pEm->pSin_parts_ptr;

	Svec.vy = -300;
	Svec.vz = 0;

	switch (pEm->Routine_2) {
	case 0:
		pEm->Timer0 = 0;
		pEm->Timer1 = 0;

		Svec.vx = 250;
		Esp_call(0x62300, 0, &pParts->Workm, &Svec);
		Esp_call(0x62300, 0, &pParts->Workm, &Svec);
		Svec.vx = 250;
		Esp_call(0x62300, 0x800, &pParts->Workm, &Svec);
		Esp_call(0x62300, 0x800, &pParts->Workm, &Svec);
		pEm->Routine_2++;

	case 1:
		*(long *)&pEm->Move_no = 0;
		Joint_move(pEm, Kan, Seq, 0x1000);
		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;

		pEm->Routine_2++;
		break;

	case 2:
		Joint_move(pEm, Kan, Seq, 0x1000);
		if (pEm->Move_cnt == 108) pEm->Routine_2++;

		pEm->Timer1++;

		if (pEm->Timer1 == 2) {
			if (pEm->Timer0)
				Svec.vx = 250;
			else
				Svec.vx = -250;

			pEm->Timer0 ^= 1;
			pEm->Timer1 = 0;
		}

		if (pEm->Move_cnt < 80) {
			Esp_call(0x32000, 0, &pParts->Workm, &Svec);
			Esp_call(0x62000, 0, &pParts->Workm, &Svec);
		}
		pEm->Cdir_y += 0x800;
		Null_pos_set(pEm, Kan, Seq, 0);
		pEm->Cdir_y -= 0x800;

	}
}
static void Em2b_pl_dm_f(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long Tmp;
	register short Dir;
	register ENEMY_WORK *pEnemy = (ENEMY_WORK *)pEm->pEnemy_ptr;
	SVECTOR Svec;

	Dir = pEnemy->Cdir_y;

	switch (pEm->Routine_2) {
	case 0:
		*(long *)&pEm->Move_no = 0x30003;
		Snd_se_stad(0x4010001, &pEm->Pos_x);	/* retail */
		pEm->Routine_2 = 1;
		pEm->Timer0 = 5;
		pEm->Timer1 = 0;
		pEm->Timer2 = 1;
		pEm->Spd_x = 700;
		pEm->Spd_z = 0;

		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;
	case 1:
		Add_spdXZ(pEm, Dir);
		if (!pEm->Timer1) {
			pEm->Timer1 = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

			if (!pEm->Timer1)
				pEm->Timer1 = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		}
		if (!--pEm->Timer0) pEm->Routine_2++;
		pEm->Spd_x -= 100;

		if (pEm->Sca_info & 1) {
			pEm->Routine_2 = 5;

			pEm->Timer2 = 0;	} break;

	case 2:
		*(long *)&pEm->Move_no = 0x30004;
		pEm->Timer0 = 0;
		pEm->Timer2 = 0;
		Snd_se_stad(0x22A0001, &pEm->Pos_x);	/* retail */
		pEm->Routine_2 = 3;
	case 3:
		Add_spdXZ(pEm, Dir);
		Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		Tmp += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		pEm->Routine_2 += Tmp;

		Svec.vx = Svec.vz = 0;
		Svec.vy = 100;
		pEm->Timer0++;
		if (pEm->Timer0 >= 6 && pEm->Timer0 < 14) {
			Set_esp_w(pEm, 4, 0x55, 0x448, 0, 4, &Svec);
			Set_esp_w(pEm, 4, 0x55, 0x448, 0, 7, &Svec);
		}

		pEm->Spd_x -= 10;
		if (pEm->Spd_x < 0) pEm->Spd_x = 0;
		break;

	case 4:
		*(unsigned long *)&pEm->Routine_0 = 1;
		pEm->Damage_cnt = 0;
		pEm->Neck_flg &= 0xED;
		break;

	case 5:
		*(long *)&pEm->Move_no = 0x30001;
		pEm->Routine_2++;
		Snd_se_stad(0x4020001, &pEm->Pos_x);	/* retail */
		Snd_se_stad(0x2290001, &pEm->Pos_x);

	case 6:
		Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		Tmp += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		if (Tmp) pEm->Routine_2++;
		break;

	case 7:
		*(long *)&pEm->Move_no = 0x30005;
		pEm->Routine_2++;

		pEm->Timer0 = 0;

	case 8:
		Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x10400);

		pEm->Timer0 -= 1 + Gacha_ch() * 5;
		if (pEm->Timer0 < 0) pEm->Routine_2 = 9;
		break;

	case 9:
		*(long *)&pEm->Move_no = 0x30002;
		pEm->Routine_2++;
		pEm->Damage_cnt = 0;

	case 10:
		Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		if (Tmp) {
			pEm->Routine_2 = 11;
			*(long *)&pEm->Move_no = 0x30006;
		}
		break;

	case 11:
		Tmp = Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400);

		if (!Tmp)
			Tmp = Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400);

		if (Tmp) pEm->Routine_2 = 4;

	}

	if (pEm->Timer2) Goto00(pEm, pEnemy->Pos_x, pEnemy->Pos_z, 0x20);
}
static void Em2b_pl_dm_b(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
	register unsigned long Tmp;
	register short Dir;
	register ENEMY_WORK *pEnemy = (ENEMY_WORK *)pEm->pEnemy_ptr;

	Dir = pEnemy->Cdir_y;

	switch (pEm->Routine_2) {
	case 0:
		Snd_se_stad(0x4020001, &pEm->Pos_x);	/* retail */
		*(long *)&pEm->Move_no = 0x30006;
		pEm->Routine_2 = 1;
		pEm->Timer1 = 10;
		pEm->Timer2 = 1;
		pEm->Spd_x = 300;
		pEm->Spd_z = 0;

		pEm->Damage_cnt |= 0x80;
		pEm->Neck_flg |= 0x12;

	case 1:

		Add_spdXZ(pEm, Dir);
		pEm->Routine_2 += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		if (--pEm->Timer1 > 0) {
			pEm->Spd_x -= 15;
		}else {
			pEm->Spd_x -= 25;
		}
		if (pEm->Spd_x < 0) pEm->Spd_x = 0;
		break;

	case 2:
		*(long *)&pEm->Move_no = 0x30002;
		pEm->Routine_2++;
		pEm->Timer2 = 0;
		pEm->Damage_cnt = 0;

	case 3:
		Tmp = Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		Tmp += Joint_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr, 0x400);

		if (Tmp) {
			pEm->Routine_2++;
			*(long *)&pEm->Move_no = 0x30006;
		}
		break;

	case 4:
		Tmp = Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400);

		if (!Tmp)
			Tmp = Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400);

		if (Tmp) {
			*(unsigned long *)&pEm->Routine_0 = 1;
			pEm->Damage_cnt = 0;
			pEm->Neck_flg &= 0xED;
		}

	}

	if (pEm->Timer2) Goto01(pEm, Dir, 0x20);
}
static void Em2b_Scenario(ENEMY_WORK *pEm, unsigned long Kan, unsigned long Seq)
{
}
