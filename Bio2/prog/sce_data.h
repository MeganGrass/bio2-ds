class Resident_Evil_2_Bytecode {
public:

#pragma pack(push, 1)

	// 0x00	// Nop
	struct Nop {
		UCHAR Opcode;					// 0x00 // 0x00
	};

	// 0x01	// Evt_end
	struct Evt_end {
		UCHAR Opcode;					// 0x00 // 0x01
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x02	// Evt_next
	struct Evt_next {
		UCHAR Opcode;					// 0x00 // 0x02
	};

	// 0x03	// Evt_chain
	struct Evt_chain {
		UCHAR Opcode;					// 0x00 // 0x03
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Gosub;					// 0x02 // always 0x18 (opcode for Gosub)
		UCHAR Evt_no;					// 0x03 // Event number (numerical ID of SCD file)
	};

	// 0x04	// Evt_exec
	struct Evt_exec {
		UCHAR Opcode;					// 0x00 // 0x04
		UCHAR Task_level;				// 0x01 // Event_exec() argv[0] (typically 0xFF)
		UCHAR Gosub;					// 0x02 // always 0x18 (opcode for Gosub)
		UCHAR Evt_no;					// 0x03 // Event number (numerical ID of SCD file)
	};

	// 0x05	// Evt_kill
	struct Evt_kill {
		UCHAR Opcode;					// 0x00 // 0x05
		UCHAR Evt_no;					// 0x01 // Event number (numerical ID of SCD file)
	};

	// 0x06	// Ifel_ck
	struct Ifel_ck {
		UCHAR Opcode;					// 0x00 // 0x06
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT Ofs;						// 0x02 // Relative pointer to the end of the Ifel_ck block, beginning immediately after this bytecode
	};

	// 0x07	// Else_ck
	struct Else_ck {
		UCHAR Opcode;					// 0x00 // 0x07
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT Ofs;						// 0x02 // Relative pointer to the end of the Else_ck block, beginning immediately after this bytecode
	};

	// 0x08	// Endif
	struct Endif {
		UCHAR Opcode;					// 0x00 // 0x08			
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x09	// Sleep
	struct Sleep {
		UCHAR Opcode;					// 0x00 // 0x09
		UCHAR Sleeping;					// 0x01 // always 0x0A (opcode for Sleeping)
		USHORT Lcnt;					// 0x02 // Sleep timer counter
	};

	// 0x0A	// Sleeping
	struct Sleeping {
		UCHAR Opcode;					// 0x00 // 0x0A
		USHORT Lcnt;					// 0x01 // Sleep timer counter
	};

	// 0x0B	// Wsleep
	struct Wsleep {
		UCHAR Opcode;					// 0x00 // 0x0B
	};

	// 0x0C	// Wsleeping
	struct Wsleeping {
		UCHAR Opcode;					// 0x00 // 0x0C
	};

	// 0x0D	// For
	struct For {
		UCHAR Opcode;					// 0x00 // 0x0D
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT Ofs;						// 0x02 // Relative pointer to the end of the For block, beginning immediately after this bytecode
		USHORT Lcnt;					// 0x04 // Loop count
	};

	// 0x0E	// Next
	struct Next {
		UCHAR Opcode;					// 0x00 // 0x0E
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x0F	// While
	struct While {
		UCHAR Opcode;					// 0x00 // 0x0F
		UCHAR ConditionLen;				// 0x01	// Size of the bytecode that the While loop uses as a condition. Only one condition is allowed.
		USHORT Ofs;						// 0x02	// Relative pointer to the end of the While block, beginning immediately after this bytecode
	};

	// 0x10	// Ewhile
	struct Ewhile {
		UCHAR Opcode;					// 0x00 // 0x10
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x11	// Do
	struct Do {
		UCHAR Opcode;					// 0x00 // 0x11
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT Ofs;						// 0x02	// Relative pointer to the end of the Do block, beginning immediately after this bytecode
	};

	// 0x12	// Edwhile
	struct Edwhile {
		UCHAR Opcode;					// 0x00 // 0x12
		UCHAR ConditionLen;				// 0x01	// Size of the bytecode that the Do-While loop uses as a condition. Only one condition is allowed.
	};

	// 0x13	// Switch
	struct Switch {
		UCHAR Opcode;					// 0x00 // 0x13
		UCHAR Work_no;					// 0x01	// Data beginning at G.SaveData.F_atari
		USHORT Ofs;						// 0x02	// Relative pointer to the end of the Switch block, beginning immediately after this bytecode
	};

	// 0x14	// Case
	struct Case {
		UCHAR Opcode;					// 0x00 // 0x14
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT Ofs;						// 0x02	// Relative pointer to the end of the Case block, beginning immediately after this bytecode. Zero (0), if immediately followed by another Case
		USHORT Expression;				// 0x04	// Value to compare against the value referenced by the Switch's Work_no
	};

	// 0x15	// Default
	struct Default {
		UCHAR Opcode;					// 0x00 // 0x15
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x16	// Eswitch
	struct Eswitch {
		UCHAR Opcode;					// 0x00 // 0x16
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x17	// Goto
	struct Goto {
		UCHAR Opcode;					// 0x00	// 0x17
		UCHAR Ifel_ctr;					// 0x01	// pSce->Ifel_ctr, always 0xFF (0x01 on ROOM304, SUB05.SCD, only)
		UCHAR Loop_ctr;					// 0x02	// pSce->Loop_ctr, always 0xFF (0x00 on ROOM500, SUB04.SCD and SUB07.SCD, only)
		UCHAR Nop;						// 0x03	// always 0x00 (opcode for Nop)
		SHORT Ofs;						// 0x04	// Relative pointer, always references same script
	};

	// 0x18	// Gosub
	struct Gosub {
		UCHAR Opcode;					// 0x00 // 0x18
		UCHAR Evt_no;					// 0x01	// Event number (numerical ID of SCD file)
	};

	// 0x19	// Return
	struct Return {
		UCHAR Opcode;					// 0x00 // 0x19
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x1A	// Break
	struct Break {
		UCHAR Opcode;					// 0x00 // 0x1A
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
	};

	// 0x1B	// For2, alternative to For, utilizes a timer in the G.SaveData.Sce_work[] array
	struct For2 {
		UCHAR Opcode;					// 0x00 // 0x1B
		UCHAR Nop0;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Ofs;						// 0x02 // Relative pointer to the end of the For2 block, beginning immediately after this bytecode
		UCHAR Nop1;						// 0x04	// always 0x00 (opcode for Nop)
		UCHAR Flag;						// 0x05 // Data beginning at G.SaveData.F_atari
	};

	// 0x1C	// Break_point
	struct Break_point {
		UCHAR Opcode;					// 0x00 // 0x1C
	};

	// 0x1D	// Work_copy
	struct Work_copy {
		UCHAR Opcode;					// 0x00	// 0x1D
		UCHAR Work_no;					// 0x01	// Data beginning at G.SaveData.F_atari
		UCHAR Work_dest;				// 0x02	// Data beginning at G.SaveData.F_atari
		UCHAR DataType;					// 0x03	// 0 = uchar, 1 = short
	};

	// 0x21 // Ck
	struct Ck {
		UCHAR Opcode;					// 0x00	// 0x21	
		UCHAR Flag;						// 0x01	// 0 = G.System_flg
		// 1 = G.Status_flg
		// 2 = G.Stop_flg
		// 3 = G.SaveData.Scenario_flg
		// 4 = G.SaveData.Common_flg
		// 5 = G.SaveData.Room_flg
		// 6 = G.SaveData.Enemy_flg
		// 7 = G.SaveData.Enemy_flg2
		// 8 = G.SaveData.Item_flg
		// 9 = G.SaveData.Map_flg
		// 10 = G.Use_flg
		// 11 = Moji.Mess_flg
		// 12 = G.Room_enemy_flg
		// 13 = G.SaveData.Pri_be_flg [0]
		// 14 = G.SaveData.Pri_be_flg [1]
		// 15 = G.SaveData.Pri_be_flg [2]
		// 16 = G.SaveData.Pri_be_flg [3]
		// 17 = G.SaveData.Pri_be_flg [4]
		// 18 = G.SaveData.Pri_be_flg [5]
		// 19 = G.SaveData.Pri_be_flg [6]
		// 20 = G.SaveData.Pri_be_flg [7]
		// 21 = G.SaveData.Pri_be_flg [8]
		// 22 = G.SaveData.Pri_be_flg [9]
		// 23 = G.SaveData.Pri_be_flg [10]
		// 24 = G.SaveData.Pri_be_flg [11]
		// 25 = G.SaveData.Pri_be_flg [12]
		// 26 = G.SaveData.Pri_be_flg [13]
		// 27 = G.SaveData.Pri_be_flg [14]
		// 28 = G.SaveData.Pri_be_flg [15]
		// 29 = G.SaveData.Zapping_flg
		// 30 = G.Rbj_set_flg
		// 31 = G.SaveData.Key_flg
		// 32 = G.SaveData.Map_c_flg
		// 33 = G.SaveData.Map_i_flg
		// 34 = G.SaveData.Item_flg2
		// 35 = G.SaveData.Map_o_flg
		// 36 = G.ExData0[0]
		// 37 = G.ExData1[0]
		// 38 = G.ExData2[0]
		// 39 = G.ExData3[0]
		UCHAR Bit;						// 0x02	// Bit
		UCHAR OnOff;					// 0x03	// 0 = OFF, 1 = ON
	};

	// 0x22	// Set
	struct Set {
		UCHAR Opcode;					// 0x00	// 0x22
		UCHAR Flag;						// 0x01	// Same as Ck
		UCHAR Bit;						// 0x02	// Bit
		UCHAR Operator;					// 0x03	// 0 = OR, 1 = NOT, 7 = XOR
	};

	// 0x23	// Cmp
	struct Cmp {
		UCHAR Opcode;					// 0x00	// 0x23
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Flag;						// 0x02 // Data beginning at G.SaveData.F_atari
		UCHAR Operator;					// 0x03 // Accepted values: 0-6
		// 0 = Equal
		// 1 = Greater
		// 2 = Greater or Equal
		// 3 = Less
		// 4 = Less or Equal
		// 5 = Not Equal
		// 6 = Logical AND
		SHORT Num;						// 0x04 // Value to compare against the value referenced by Flag
	};

	// 0x24	// Save
	struct Save {
		UCHAR Opcode;					// 0x00	// 0x24
		UCHAR Flag;						// 0x01 // Data beginning at G.SaveData.F_atari
		SHORT Num;						// 0x02 // Value to save to the value referenced by Flag
	};

	// 0x25	// Copy
	struct Copy {
		UCHAR Opcode;					// 0x00	// 0x25
		UCHAR Flag0;					// 0x01 // Data beginning at G.SaveData.F_atari
		UCHAR Flag1;					// 0x02 // Data beginning at G.SaveData.F_atari, value to copy to the value referenced by Flag0
	};

	// 0x26	// Calc
	struct Calc {
		UCHAR Opcode;					// 0x00	// 0x26
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Operator;					// 0x02 // 0 = Addition
		// 1 = Subtraction
		// 2 = Multiplication
		// 3 = Division
		// 4 = MOD
		// 5 = OR
		// 6 = AND
		// 7 = XOR
		// 8 = NOT
		// 9 = Left Shift <<
		// 10 = Right Shift >> (unsigned)
		// 11 = Right Shift >> (signed)
		UCHAR Flag;						// 0x03	// Data beginning at G.SaveData.F_atari (always 0x10?)
		SHORT Num;						// 0x04	// Value to calculate with the value referenced by Flag
	};

	// 0x27	// Calc2
	struct Calc2 {
		UCHAR Opcode;					// 0x00	// 0x27
		UCHAR Operator;					// 0x01 // Same as Calc
		UCHAR Flag0;					// 0x02	// Data beginning at G.SaveData.F_atari (always 0x10?)
		UCHAR Flag1;					// 0x03	// Data beginning at G.SaveData.F_atari, value to calculate with the value referenced by Flag0
	};

	// 0x28	// Sce_rnd
	struct Sce_rnd {
		UCHAR Opcode;					// 0x00	// 0x28
	};

	// 0x29	// Cut_chg
	struct Cut_chg {
		UCHAR Opcode;					// 0x00	// 0x29
		UCHAR Cut_no;					// 0x01	// Cut_chg_main(Cut_no & 0x7f);
	};

	// 0x2A	// Cut_old
	struct Cut_old {
		UCHAR Opcode;					// 0x00	// 0x2A
	};

	// 0x2B	// Message_on
	struct Message_on {
		UCHAR Opcode;					// 0x00	// 0x2B
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Mess_no;					// 0x02	// Message number
		UCHAR Attr;						// 0x03	// Message attribute
		USHORT Stop_data;				// 0x04	// Mess_set(0, Attr | 0x300, Mess_no, Stop_data << 0x10)
	};

	// 0x2C	// Aot_set
	struct Aot_set {
		UCHAR Opcode;					// 0x00	// 0x2C
		UCHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02	// Sce Atari Type
		UCHAR Type;						// 0x03	// Sce Atari Flags
		UCHAR nFloor;					// 0x04	// Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05	// Extended Matrix Flag
		SHORT X;						// 0x06	// X Position
		SHORT Z;						// 0x08	// Z Position
		USHORT W;						// 0x0A	// Width Size
		USHORT D;						// 0x0C	// Depth Size
		USHORT Data0;					// 0x0E	// Unique per Id
		USHORT Data1;					// 0x10	// Unique per Id
		USHORT Data2;					// 0x12	// Unique per Id

		// Id	Name		Function
		// 
		// 0x00 NOTHING		Nothing
		//		DATA0 = variable
		//		DATA1 = variable
		//		DATA2 = variable
		// 
		// 0x03	NORMAL		Sce_Normal
		//		DATA0 = always zero (0)
		//		DATA1 = always zero (0)
		//		DATA2 = always zero (0)
		// 
		// 0x04	MESSAGE		Sce_Message
		//		DATA0 = Mess_no
		//		DATA1 = Attr
		//		DATA2 = Stop_data 
		// 
		// 0x05	EVENT		Sce_Event
		//		DATA0 = Task_level
		//		DATA1 = Evt_no | 0x18
		//		DATA2 = always zero (0)
		// 
		// 0x06	FLAG_CHG	Sce_Flg_chg
		//		DATA0 = Flag
		//		DATA1 = Bit
		//		DATA2 = Operator
		// 
		// 0x07	WATER		Sce_Water
		//		DATA0 = Div_max
		//		DATA1 = always zero (0)
		//		DATA2 = always zero (0)
		// 
		// 0x08	MOVE		Sce_Move
		//		DATA0 = Pos X
		//		DATA1 = Pos Y
		//		DATA2 = Pos Z
		// 
		// 0x09	SAVE		Sce_Save
		//		DATA0 = Numerical ID of Save Area string array
		//		DATA1 = always zero (0)
		//		DATA2 = always zero (0)
		// 
		// 0x0A	ITEMBOX		Sce_Itembox
		//		DATA0 = Numerical ID of MD1 model for Item Box Lid
		//		DATA1 = Numerical ID of TIM texture for Item Box Lid
		//		DATA2 = always zero (0)
		// 
		// 0x0B	DAMAGE		Sce_Damage
		//		DATA0 = Routine_1
		//		DATA1 = Health Count to subtract from Player
		//		DATA2 = always zero (0)
		// 
		// 0x0C	STATUS		Sce_Status
		//		DATA0 = always zero (0)
		//		DATA1 = always zero (0)
		//		DATA2 = always zero (0)
		// 
		// 0x0D	DRAWER		Sce_Hikidashi
		//		DATA0 = Key Flag Bit
		//		DATA1 = Item ID
		//		DATA2 = Message ID
		// 
		// 0x0E	COMPUTER	Sce_Windows
		//		DATA0 = Window_no (0x00 for RPD Main Hall, 0x01 for Umbrella Lab)
		//		DATA1 = ??? (0x0F if Window_no == 0x00, 0x06 if Window_no == 0x01)
		//		DATA2 = always zero (0)
	};

	// 0x2D	// Obj_model_set
	struct Obj_model_set {
		UCHAR Opcode;					// 0x00	// 0x2D
		UCHAR Om_no;					// 0x01	// Numerical ID of MD1 model
		UCHAR Id;						// 0x02	// G.Ob_model[Om_no].Id
		UCHAR Cc_ctr;					// 0x03	// Cc_work.Cc_parts[Cc_work.Ccol_no].Cc_ctr
		UCHAR Cc_wait;					// 0x04	// Cc_work.Cc_parts[Cc_work.Ccol_no].Cc_wait
		UCHAR Cc_num;					// 0x05	// Cc_work.Cc_parts[Cc_work.Ccol_no].Cc_num
		UCHAR nFloor;					// 0x06	// G.Ob_model[Om_no].nFloor
		UCHAR Super;					// 0x07	// G.Ob_model[Om_no].pSuper
		USHORT Type;					// 0x08	// G.Ob_model[Om_no].Type
		USHORT Be_flg;					// 0x0A	// G.Ob_model[Om_no].Be_flg
		SHORT Attribute;				// 0x0C	// G.Ob_model[Om_no].Attribute
		SHORT Pos_x;					// 0x0E	// G.Ob_model[Om_no].Pos_x
		SHORT Pos_y;					// 0x10	// G.Ob_model[Om_no].Pos_y
		SHORT Pos_z;					// 0x12	// G.Ob_model[Om_no].Pos_z
		SHORT Cdir_x;					// 0x14	// G.Ob_model[Om_no].Cdir_x
		SHORT Cdir_y;					// 0x16	// G.Ob_model[Om_no].Cdir_y
		SHORT Cdir_z;					// 0x18	// G.Ob_model[Om_no].Cdir_z
		SHORT Ofs_x;					// 0x1A	// G.Ob_model[Om_no].Atd[0].Ofs_x
		SHORT Ofs_y;					// 0x1C	// G.Ob_model[Om_no].Atd[0].Ofs_y
		SHORT Ofs_z;					// 0x1E	// G.Ob_model[Om_no].Atd[0].Ofs_z
		SHORT At_w;						// 0x20	// G.Ob_model[Om_no].Atd[0].At_w
		SHORT At_h;						// 0x22	// G.Ob_model[Om_no].Atd[0].At_h
		SHORT At_d;						// 0x24	// G.Ob_model[Om_no].Atd[0].At_d

		// Cc_ctr (always either 0x00 or 0x82)
		// SCR_CHG
		// TC_LOOP
		// COL_CHG

		// Be_flg
		// if(Id == 1) { Mizu_trans(Om, (Attribute >> 1 & 1)); }
		// else if((Be_flg & 0x10) != 0) bomb
		// else if((Be_flg & 8) == 0) && (Attribute & 0x10) == 0) neo
		// else line
	};

	// 0x2E	// Work_set
	struct Work_set {
		UCHAR Opcode;					// 0x00	// 0x2E
		UCHAR Work_type;				// 0x01	// Accepted values: 1-5
		// 1 = Player		// pSce->pWork = G.pEnemy[0];
		// 2 = Sub Player	// pSce->pWork = G.pEnemy[1];
		// 3 = Enemy		// pSce->pWork = G.pEnemy[Work_no];
		// 4 = Object		// pSce->pWork = G.Ob_model[Work_no];
		// 5 = Door			// pSce->pWork = pDwork[Work_no];
		CHAR Work_no;					// 0x02	// Enemy, Object or Door ID
	};

	// 0x2F	// Speed_set
	struct Speed_set {
		UCHAR Opcode;					// 0x00	// 0x2F
		UCHAR Spd_type;					// 0x01	// Accepted values: 0-11
		// 0 = pSce->Spd_x
		// 1 = pSce->Spd_y
		// 2 = pSce->Spd_z
		// 3 = pSce->Dspd_x
		// 4 = pSce->Dspd_y
		// 5 = pSce->Dspd_z
		// 6 = pSce->Aspd_x
		// 7 = pSce->Aspd_y
		// 8 = pSce->Aspd_z
		// 9 = pSce->Adspd_x
		// 10 = pSce->Adspd_y
		// 11 = pSce->Adspd_z
		USHORT Num;						// 0x02	// Speed value
	};

	// 0x30	// Add_speed
	struct Add_speed {
		UCHAR Opcode;					// 0x00	// 0x30
	};

	// 0x31	// Add_aspeed
	struct Add_aspeed {
		UCHAR Opcode;					// 0x00	// 0x31
	};

	// 0x32	// Pos_set
	struct Pos_set {
		UCHAR Opcode;					// 0x00	// 0x32
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Pos_x;					// 0x02	// pSce->pWork->Pos_x
		SHORT Pos_y;					// 0x04	// pSce->pWork->Pos_y
		SHORT Pos_z;					// 0x06	// pSce->pWork->Pos_z
	};

	// 0x33	// Dir_set
	struct Dir_set {
		UCHAR Opcode;					// 0x00	// 0x33
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Cdir_x;					// 0x02	// pSce->pWork->Cdir_x
		SHORT Cdir_y;					// 0x04	// pSce->pWork->Cdir_y
		SHORT Cdir_z;					// 0x06	// pSce->pWork->Cdir_z
	};

	// 0x34	// Member_set
	struct Member_set {
		UCHAR Opcode;					// 0x00	// 0x34
		UCHAR Member;					// 0x01	// Accepted values: 0x00-0x2B
		// pSce->pWork->Be_flg[0] = (USHORT)Num
		// pSce->pWork->Be_flg[1] = (USHORT)Num
		// pSce->pWork->Routine_0 = (UCHAR)Num
		// pSce->pWork->Routine_1 = (UCHAR)Num
		// pSce->pWork->Routine_2 = (UCHAR)Num
		// pSce->pWork->Routine_3 = (UCHAR)Num
		// pSce->pWork->Id = (UCHAR)Num
		// pSce->pWork->Type = (USHORT)Num
		// pSce->pWork->At_obj_no = (UCHAR)Num
		// pSce->pWork->At_sce_no = (UCHAR)Num
		// pSce->pWork->Attribute = Num
		// pSce->pWork->Pos_x = Num
		// pSce->pWork->Pos_y = Num
		// pSce->pWork->Pos_z = Num
		// pSce->pWork->Cdir_x = (USHORT)Num
		// pSce->pWork->Cdir_y = (USHORT)Num
		// pSce->pWork->Cdir_z = (USHORT)Num
		// pSce->pWork->nFloor = (UCHAR)Num
		// pSce->pWork->Status_flg = (USHORT)Num
		// pSce->pWork->Ground = (USHORT)Num
		// pSce->pWork->Dest_x = (USHORT)Num
		// pSce->pWork->Dest_z = (USHORT)Num
		// pSce->pWork->Sce_flg = (USHORT)Num
		// pSce->pWork->Sce_free0 = (USHORT)Num
		// pSce->pWork->Sce_free1 = (USHORT)Num
		// pSce->pWork->Sce_free2 = (USHORT)Num
		// pSce->pWork->Sce_free3 = (USHORT)Num
		// pSce->pWork->Spd_x = (UCHAR)Num
		// pSce->pWork->Spd_x = (USHORT)Num
		// pSce->pWork->Spd_y = (USHORT)Num
		// pSce->pWork->Spd_z = (USHORT)Num
		// pSce->pWork->Hokan_flg = (UCHAR)Num
		// pSce->pWork->Atd[0].Ofs_x = (USHORT)Num
		// pSce->pWork->Atd[0].Ofs_y = (USHORT)Num
		// pSce->pWork->Atd[0].Ofs_z = (USHORT)Num
		// pSce->pWork->Atd[0].At_w = (USHORT)Num
		// pSce->pWork->Atd[0].At_h = (USHORT)Num
		// pSce->pWork->Atd[0].At_d = (USHORT)Num
		// pSce->pWork->Parts0_pos_y = (USHORT)Num
		// pSce->pWork->Sca_old_x = (USHORT)Num
		// pSce->pWork->Sca_old_z = (USHORT)Num
		// pSce->pWork->Free[1] = (USHORT)Num
		// pSce->pWork->Free[6] = (USHORT)Num
		// pSce->pWork->Damage_cnt = (UCHAR)Num
		SHORT Num;						// 0x02	// Value to set to Member
	};

	// 0x35	// Member_set2
	struct Member_set2 {
		UCHAR Opcode;					// 0x00	// 0x35
		UCHAR Member;					// 0x01	// Same as Member_set
		UCHAR Flag;						// 0x02	// Data beginning at G.SaveData.F_atari, reference value to set to value referenced by Member
	};

	// 0x36	// Se_on
	struct Se_on {
		UCHAR Opcode;					// 0x00	// 0x36
		UCHAR VAB;						// 0x01	// Numerical ID of VAB file
		SHORT EDT;						// 0x02	// Numerical ID of EDT file
		SHORT Work_type;				// 0x04	// 0 = Pos(Pos_x, Pos_y, Pos_z)
		// 1 = Player
		// 2 = Sub Player
		// 3 = Enemy
		// 4 = Object
		SHORT Pos_x;					// 0x06	// X Position
		SHORT Pos_y;					// 0x08	// Y Position
		SHORT Pos_z;					// 0x0A	// Z Position
	};

	// 0x37	// Sca_id_set
	struct Sca_id_set {
		UCHAR Opcode;					// 0x00	// 0x37
		UCHAR Col_no;					// 0x01	// Numerical ID of collision data to modify
		USHORT Id;						// 0x02	// G.pRoom->pSca[Col_no]->Id = Id
	};

	// 0x38	// Flr_set
	struct Flr_set {
		UCHAR Opcode;					// 0x00	// 0x38
		UCHAR Flr_no;					// 0x01	// Numerical ID of floor data to modify
		UCHAR OnOff;					// 0x02	// 0 = OFF, 1 = ON
	};

	// 0x39	// Dir_ck
	struct Dir_ck {
		UCHAR Opcode;					// 0x00	// 0x39
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Vec_x;					// 0x02 // Goto00_ck() argv[1]
		SHORT Vec_z;					// 0x04 // Goto00_ck() argv[2]
		SHORT Add_dir;					// 0x06 // Goto00_ck() argv[3]
	};

	// 0x3A	// Sce_espr_on
	struct Sce_espr_on {
		UCHAR Opcode;					// 0x00	// 0x3A
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Esp_id;					// 0x02 // Numerical ID of ESP effect file
		UCHAR Esp_dt;					// 0x03 // pEspdt[Esp_dt]
		UCHAR Work_kind;				// 0x04	// Get_matrix() argv[0]
		UCHAR Work_no;					// 0x05	// Get_matrix() argv[1]
		USHORT Espmv;					// 0x06	// Esp_call() argv[0], or'd with Esp_type and Esp_dt
		SHORT Svec_x;					// 0x08	// 
		SHORT Svec_y;					// 0x0A	//
		SHORT Svec_z;					// 0x0C	//
		USHORT Dir_y;					// 0x0E	// Esp_call() argv[1]

		// Bit
		// Numerical ESP Id = ((Bit << 0x18) >> 0x18))					// ESP19.EFF, etc.
		// Numerical Index Id = (((Bit >> 8) << 0x10) >> 0x10 & 0xFF)	// pEspdt[Id]

		// if (Work_kind & 0x80) == 0
		//		switch(Work_kind)
		//		{
		//		case 0x00: 	pMatrix = GsIDMATRIX;				break;
		//		case 0x01: 	pMatrix = G.pEnemy[0]->M;			break;
		//		case 0x02: 	pMatrix = G.pEnemy[1]->M;			break;
		//		case 0x03: 	pMatrix = G.pEnemy[Work_no]->M;		break;
		//		case 0x04: 	pMatrix = G.Ob_model[Work_no].M;	break;
		//		}
		// else
		//		pEm = G.pEnemy[0]
		//		if ((Work_kind != 0) && (Work_kind != 1)) pEm = G.pEnemy[Work_no]
		//		pMatrix = pEm->pSin_parts_ptr[Work_kind & 0x1F].Workm
	};

	// 0x3B	// Door_aot_set
	struct Door_aot_set {
		UCHAR Opcode;					// 0x00	// 0x3B
		UCHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02	// Sce Atari Type (always 0x01, Sce_Door)
		UCHAR Type;						// 0x03	// Sce Atari Flags
		UCHAR nFloor;					// 0x04	// Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05	// Extended Matrix Flag
		SHORT X;						// 0x06	// X Position
		SHORT Z;						// 0x08	// Z Position
		USHORT W;						// 0x0A	// Width Size
		USHORT D;						// 0x0C	// Depth Size
		SHORT Next_pos_x;				// 0x0E	// X Position of Player in next RDT
		SHORT Next_pos_y;				// 0x10	// Y Position of Player in next RDT
		SHORT Next_pos_z;				// 0x12	// Z Position of Player in next RDT
		SHORT Next_cdir_y;				// 0x14	// Y Rotation of Player in next RDT
		UCHAR Next_stage;				// 0x16	// Numerical ID of Stage for next RDT file
		UCHAR Next_room;				// 0x17	// Numerical ID of Room for next RDT file
		UCHAR Next_cut;					// 0x18	// Numerical ID of Cut for next RDT file
		UCHAR Next_nfloor;				// 0x19	// Player distance from ground in next RDT (actual is nFloor * -1800)
		UCHAR Dtex_type;				// 0x1A	// Numerical ID of DO2 file
		UCHAR Door_type;				// 0x1B	// Numerical ID of DO2 animation
		UCHAR Knock_type;				// 0x1C	// Numerical ID of VAG sound from VAB soundbank (always 0x00)
		UCHAR Key_id;					// 0x1D	// G.Savegame.Key_flg bit (0x00 for unlocked door)
		UCHAR Key_type;					// 0x1E	// Numerical ID of Item required to unlock door
		UCHAR Free;						// 0x1F	// always 0x00

		// Next_stage
		// Value is zero-based, so 0x00 = ST1, 0x01 = ST2, etc.

		// Key_type
		// 0x00 = No key is required to open door
		// 0xFE = Door can be unlocked without a key
		// 0xFF = Door is locked from the other side
	};

	// 0x3C	// Cut_auto
	struct Cut_auto {
		UCHAR Opcode;					// 0x00	// 0x3C
		UCHAR OnOff;					// 0x01	// 1 = OFF, 0 = ON
	};

	// 0x3D	// Member_copy
	struct Member_copy {
		UCHAR Opcode;					// 0x00	// 0x3D
		CHAR Flag;						// 0x01	// Data beginning at G.SaveData.F_atari, reference value to copy to value referenced by Member
		CHAR Member;					// 0x02	// Same as Member_set
	};

	// 0x3E	// Member_cmp
	struct Member_cmp {
		UCHAR Opcode;					// 0x00	// 0x3E
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Member;					// 0x02	// Same as Member_set
		UCHAR Operator;					// 0x03	// Accepted values: 0-6
		// 0 = Equal
		// 1 = Greater
		// 2 = Greater or Equal
		// 3 = Less
		// 4 = Less or Equal
		// 5 = Not Equal
		// 6 = Logical AND
		SHORT Num;						// 0x04	// Value to compare to value referenced by Member
	};

	// 0x3F	// Plc_motion
	struct Plc_motion {
		UCHAR Opcode;					// 0x00 // 0x3F
		UCHAR Routine_1;				// 0x01	// pSce->pWork->Routine_1
		UCHAR Move_no;					// 0x02	// pSce->pWork->Move_no
		UCHAR Sce_flg;					// 0x03	// pSce->pWork->Sce_flg

		// Routine_0
		// Numerical ID of EDD/EMR file
	};

	// 0x40	// Plc_dest
	struct Plc_dest {
		UCHAR Opcode;					// 0x00	// 0x40
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Routine_1;				// 0x02	// pSce->pWork->Routine_1
		UCHAR Set_flg_no;				// 0x03	// pSce->pWork->Set_flg_no
		SHORT Dest_x;					// 0x04	// pSce->pWork->Dest_x
		SHORT Dest_z;					// 0x06	// pSce->pWork->Dest_z

		// if (pSce->pWork->Id < 0x10)
		// switch(Routine_0)
		// {
		// case 0x04:
		// case 0x12: Subpl_walk; break;
		// case 0x05: Subpl_run; break;
		// case 0x07: Subpl_back; break;
		// case 0x08: Subpl_ato; break;
		// case 0x09: Subpl_dir; break;
		// }
	};

	// 0x41	// Plc_neck
	struct Plc_neck {
		UCHAR Opcode;					// 0x00	// 0x41
		UCHAR Neck_flg;					// 0x01	// Accepted values: 0-6
		// 0	pSce->pWork->Neck_flg | 0x12
		// 1	pSce->pWork->Neck_flg | 0x04
		// 2	pSce->pWork->Neck_flg | 0x08
		// 3	pSce->pWork->Neck_flg | 0x2A
		// 4	pSce->pWork->Neck_flg | 0x58
		// 5	pSce->pWork->Neck_flg | 0x04
		// 6	pSce->pWork->Neck_flg = 0x00
		SHORT Neck_point_x;				// 0x02	// pSce->pWork->Neck_point_x
		SHORT Neck_point_y;				// 0x04	// pSce->pWork->Neck_point_y
		SHORT Neck_point_z;				// 0x06	// pSce->pWork->Neck_point_z
		SHORT Free;						// 0x08	// pSce->pWork->Free[6]
	};

	// 0x42	// Plc_ret
	struct Plc_ret {
		UCHAR Opcode;					// 0x00	// 0x42
	};

	// 0x43	// Plc_flg
	struct Plc_flg {
		UCHAR Opcode;					// 0x00	// 0x43
		UCHAR Operator;					// 0x01	// Accepted values: 0-2
		// 0 = OR
		// 1 = Equal
		// 2 = XOR
		USHORT Num;						// 0x02	// Value to calculate with the pSce->pWork->Sce_flg value
	};

	// 0x44	// Sce_em_set				// Incomplete, requires further research
	struct Sce_em_set {
		UCHAR Opcode;					// 0x00	// 0x44
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		CHAR Em_no;						// 0x02	// Enemy object pool ID
		UCHAR Id;						// 0x03	// Numerical ID of EMD file
		USHORT Type;					// 0x04	// 
		UCHAR nFloor;					// 0x06	// Distance from ground (actual is nFloor * -1800)
		UCHAR Sound_flg;				// 0x07	// 
		UCHAR Model_type;				// 0x08	// 
		UCHAR Em_set_flg;				// 0x09	// G.Savegame.Enemy_flg or G.Savegame.Enemy_flg2 bit
		SHORT Pos_x;					// 0x0A	// X Position
		SHORT Pos_y;					// 0x0C	// Y Position
		SHORT Pos_z;					// 0x0E	// Z Position
		SHORT Cdir_y;					// 0x10	// Y Rotation
		SHORT Motion;					// 0x12	// G.pFree_work->Timer0
		SHORT Ctr_flg;					// 0x14	// G.pFree_work->Timer1

		// Type
		// (& 0x4000 = 0) { G.pFree_work->Neck_flg = 0x92 }

		// Model_type
		// 0x80			Sce.pC_em animations
	};

	// 0x45	// Col_chg_set
	struct Col_chg_set {
		UCHAR Opcode;					// 0x00	// 0x45
		UCHAR Cc_ctr;					// 0x01	// Cc_work->Cc_parts[0].Cc_ctr | pCcp->Cc_parts[0].Cc_ctr & 0xC0
		UCHAR Cc_cnt;					// 0x02	// Cc_work->Cc_parts[0].Cc_cnt
		UCHAR Cc_wait;					// 0x03	// Cc_work->Cc_parts[0].Cc_wait
		UCHAR Cc_num;					// 0x04	// Cc_work->Cc_parts[0].Cc_num
	};

	// 0x46	// Aot_reset
	struct Aot_reset {
		UCHAR Opcode;					// 0x00	// 0x46
		UCHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02	// Sce Atari Type
		UCHAR Type;						// 0x03	// Sce Atari Flags
		USHORT Data0;					// 0x04	// Unique per Id
		USHORT Data1;					// 0x06	// Unique per Id
		USHORT Data2;					// 0x08	// Unique per Id
	};

	// 0x47	// Aot_on
	struct Aot_on {
		UCHAR Opcode;					// 0x00	// 0x47
		UCHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
	};

	// 0x48	// Super_set
	struct Super_set {
		UCHAR Opcode;					// 0x00	// 0x48
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Work_type;				// 0x02	// 0x00		pSce->pWork->pSuper = GsIDMATRIX
		// 0x40		pSce->pWork->pSuper = G.Player.M
		// 0x80		pSce->pWork->pSuper = G.Player.pTmd2
		// 0xC0		
		UCHAR Work_no;					// 0x03	// 
		//	
		//	if Work_type & 0x40
		// 		(if Work_no & 0x80) { pSce->pWork->pSuper = G.Player.pSin_parts_ptr[Work_no & 0x7F].Workm }
		//		else { pSce->pWork->pSuper = G.Player.M }
		SHORT Pos_x;					// 0x04	// pSce->pWork->pSuper->Pos_x
		SHORT Pos_y;					// 0x06	// pSce->pWork->pSuper->Pos_y
		SHORT Pos_z;					// 0x08	// pSce->pWork->pSuper->Pos_z
		SHORT Cdir_x;					// 0x0A	// pSce->pWork->pSuper->Cdir_x
		SHORT Cdir_y;					// 0x0C	// pSce->pWork->pSuper->Cdir_y
		SHORT Cdir_z;					// 0x0E	// pSce->pWork->pSuper->Cdir_z
	};

	// 0x49	// Super_reset
	struct Super_reset {
		UCHAR Opcode;					// 0x00	// 0x49
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Cdir_x;					// 0x02	// pSce->pWork->pSuper->Cdir_x
		SHORT Cdir_y;					// 0x04	// pSce->pWork->pSuper->Cdir_y
		SHORT Cdir_z;					// 0x06	// pSce->pWork->pSuper->Cdir_z
	};

	// 0x4A	// Plc_gun
	struct Plc_gun {
		UCHAR Opcode;					// 0x00	// 0x4A
		UCHAR Routine_0;				// 0x01	// pSce->pWork->Routine_0

		// Routine_0
		// Numerical ID of EDD/EMR file
	};

	// 0x4B	// Cut_replace
	struct Cut_replace {
		UCHAR Opcode;					// 0x00	// 0x4B
		UCHAR Fcut;						// 0x01	// G.pRoom->pVcut->Fcut
		UCHAR Tcut;						// 0x02	// G.pRoom->pVcut->Tcut
	};

	// 0x4C	// Sce_espr_kill
	struct Sce_espr_kill {
		UCHAR Opcode;					// 0x00	// 0x4C
		UCHAR Esp_id;					// 0x01	// Esp_kill() argv[0]
		UCHAR Esp_type;					// 0x02	// Esp_kill() argv[1]
		UCHAR Work_kind;				// 0x03	// Get_matrix() argv[0]
		UCHAR Work_no;					// 0x04	// Get_matrix() argv[1]
	};

	// 0x4D	// Door_model_set
	struct Door_model_set {
		UCHAR Opcode;					// 0x00	// 0x4D
		UCHAR Work_no;					// 0x01	// pDwork[Work_no]
		UCHAR Id;						// 0x02	// pDwork[Work_no]->Id
		UCHAR Type;						// 0x03	// pDwork[Work_no]->Type
		UCHAR Be_flg;					// 0x04	// pDwork[Work_no]->Be_flg
		UCHAR Model_no;					// 0x05	// pDwork[Work_no]->Model_no
		USHORT Attribute2;				// 0x06	// pDwork[Work_no]->Attribute2
		USHORT Attribute;				// 0x08	// pDwork[Work_no]->Attribute
		SHORT Pos_x;					// 0x0A	// pDwork[Work_no]->Pos_x
		SHORT Pos_y;					// 0x0C	// pDwork[Work_no]->Pos_y
		SHORT Pos_z;					// 0x0E	// pDwork[Work_no]->Pos_z
		SHORT Cdir_x;					// 0x10	// pDwork[Work_no]->Cdir_x
		SHORT Cdir_y;					// 0x12	// pDwork[Work_no]->Cdir_y
		SHORT Cdir_z;					// 0x14	// pDwork[Work_no]->Cdir_z

		// Attribute2
		// if (Attribute2 & 0x10)	pDwork[Work_no]->pSuper = pDwork[Attribute2 & 0xF]->Workm
		// else		pDwork[Work_no]->pSuper = GsWSMATRIX
		//
		// if (Attribute2 & 0x800)	pDoor[0]->Sound_flg = 1
		//
		// if ((Attribute2 & 0x8000) == 0) Main.Vloop = 0
	};

	// 0x4E	// Item_aot_set
	struct Item_aot_set {
		UCHAR Opcode;					// 0x00	// 0x4E
		CHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02	// Sce Atari Type (always 0x02, Sce_Item)
		UCHAR Type;						// 0x03	// Sce Atari Flags
		UCHAR nFloor;					// 0x04	// Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05	// Extended Matrix Flag
		SHORT X;						// 0x06	// X Position
		SHORT Z;						// 0x08	// Z Position
		USHORT W;						// 0x0A	// Width Size
		USHORT D;						// 0x0C	// Depth Size
		USHORT iItem;					// 0x0E	// Numerical ID of Item
		USHORT nItem;					// 0x10	// Numerical count of Item
		USHORT Item_flg;				// 0x12	// G.Savegame.Item_flg or G.Savegame.Item_flg2 Bit
		UCHAR Om_no;					// 0x14	// Numerical ID of MD1 model file (0xFF for none)
		UCHAR Action;					// 0x15	// G.Ob_model[Om_no].Free0
		// 0 = Stand to obtain
		// 1 = Kneel to obtain
		// 2 = G.Ob_model[Om_no].Be_flg = 0x80000000;
	};

	// 0x4F	// Sce_key_ck
	struct Sce_key_ck {
		UCHAR Opcode;					// 0x00	// 0x4F
		UCHAR Flag;						// 0x01	// Key flag to check
		USHORT Key;						// 0x02	// G.Key

		//if ((Key & Flag) == 0) {
		//	Flag = Flag ^ 1;
		//}
		//return Flag;
	};

	// 0x50	// Sce_trg_ck
	struct Sce_trg_ck {
		UCHAR Opcode;					// 0x00	// 0x50
		UCHAR Flag;						// 0x01	// Key flag to check
		USHORT Key_trg;					// 0x02	// G.Key_trg

		//if ((Key_trg & Flag) == 0) {
		//	Flag = Flag ^ 1;
		//}
		//return Flag;
	};

	// 0x51	// Sce_bgm_control
	struct Sce_bgm_control {
		UCHAR Opcode;					// 0x00	// 0x51
		UCHAR Work_no;					// 0x01	// Seq_ctr[Work_no]
		UCHAR Op;						// 0x02	// Accepted values: 1-5
		// 1 = Play
		// 2 = Stop
		// 3 = Replay
		// 4 = Pause
		// 5 = Decrescendo
		UCHAR Data0;					// 0x03	// Accepted values: 0-3
		UCHAR Data1;					// 0x04	// 
		UCHAR Data2;					// 0x05	// 
	};

	// 0x52	// Sce_espr_control
	struct Sce_espr_control {
		UCHAR Opcode;					// 0x00	// 0x52
		UCHAR Esp_id;					// 0x01	// Esp_kill() argv[0]
		UCHAR Esp_type;					// 0x02	// Esp_kill() argv[1]
		UCHAR Esp_rtn;					// 0x03	// Esp_kill() argv[2]
		UCHAR Work_kind;				// 0x04	// Get_matrix() argv[0]
		UCHAR Work_no;					// 0x05	// Get_matrix() argv[1]
	};

	// 0x53	// Sce_fade_set
	struct Sce_fade_set {
		UCHAR Opcode;					// 0x00	// 0x53
		UCHAR Work_no;					// 0x01	// Main.Fade[Work_no]
		UCHAR Hrate;					// 0x02	// Main.Fade[Work_no].Hrate
		UCHAR Mask_rgb;					// 0x03	// Main.Fade[Work_no].Mask_r/g/b
		SHORT Add;						// 0x04	// Fade_set() argv[1]

		// Mask
		// if (Mask & 4)	Main.Fade[Work_no].Mask_r = 0xFF
		// else		Main.Fade[Work_no].Mask_r = 0
		// 
		// if (Mask & 2)	Main.Fade[Work_no].Mask_g = 0xFF
		// else		Main.Fade[Work_no].Mask_g = 0
		// 
		// if (Mask & 1)	Main.Fade[Work_no].Mask_b = 0xFF
		// else		Main.Fade[Work_no].Mask_b = 0

		// Add
		// if ((Add << 0x10) < 0)	Main.Fade[Work_no].Kido + -0x8000
		// else 	Main.Fade[Work_no].Kido = 0
	};

	// 0x54	// Sce_espr3d_on
	struct Sce_espr3d_on {
		UCHAR Opcode;					// 0x00	// 0x54
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Esp_id;					// 0x02 // Numerical ID of ESP effect file
		UCHAR Esp_dt;					// 0x03 // pEspdt[Esp_dt]
		UCHAR Work_kind;				// 0x04 // Get_matrix() argv[0]
		UCHAR Work_no;					// 0x05 // Get_matrix() argv[1]
		USHORT Espmv;					// 0x06 // 
		SHORT Svec_x;					// 0x08 // 
		SHORT Svec_y;					// 0x0A // 
		SHORT Svec_z;					// 0x0C // 
		SHORT Dsv_x;					// 0x0E // 
		SHORT Dsv_y;					// 0x10 // 
		SHORT Dsv_z;					// 0x12 // 
		SHORT Dir_y;					// 0x14 // 
	};

	// 0x55	// Member_calc
	struct Member_calc {
		UCHAR Opcode;					// 0x00	// 0x55
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Operator;					// 0x02 // Same as Calc
		UCHAR Member;					// 0x03 // Same as Member_set
		SHORT Num;						// 0x04	// Value to calculate with the value referenced by Member
	};

	// 0x56	// Member_calc2
	struct Member_calc2 {
		UCHAR Opcode;					// 0x00	// 0x56
		UCHAR Operator;					// 0x01	// Same as Calc
		UCHAR Member;					// 0x02	// Same as Member_set
		UCHAR Work_no;					// 0x03	// Data beginning at G.SaveData.F_atari, value to calculate with the value referenced by Member
	};

	// 0x57	// Sce_bgmtbl_set
	struct Sce_bgmtbl_set {
		UCHAR Opcode;					// 0x00	// 0x57
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Stage;					// 0x02 // Numerical ID of Stage (Accepted values: 0-6)
		UCHAR Room;						// 0x03 // Numerical ID of Room (Accepted values: 0x00-0x2F)
		UCHAR Main;						// 0x04 // Numerical ID of Main BGM (Accepted values: 0x00-0x3F)
		UCHAR Sub;						// 0x05 // Numerical ID of Sub BGM (Accepted values: 0x00-0x3F)
		USHORT PANVT;					// 0x06 // 0x0000, 0x0040, 0x8000, 0x8040, 0xC000 or 0xC040

		// Stage
		// Value is zero-based, so 0x00 = ST1, 0x01 = ST2, etc.
	};

	// 0x58	// Plc_rot
	struct Plc_rot {
		UCHAR Opcode;					// 0x00	// 0x58
		UCHAR Free_no;					// 0x01	// Accepted values: 0-1
		// 0 = pSce->pWork->Sce_free0 = Sce_free
		// 1 = pSce->pWork->Sce_free1 = Sce_free
		USHORT Sce_free;				// 0x02	// 
	};

	// 0x59	// Xa_on
	struct Xa_on {
		UCHAR Opcode;					// 0x00	// 0x59
		UCHAR Mode;						// 0x01	// Accepted values: 0-1
		// 0 = G.pXa_sector
		// 1 = Xa_sector00
		USHORT Num;						// 0x02	// Numerical ID of XA file
	};

	// 0x5A	// Weapon_chg
	struct Weapon_chg {
		UCHAR Opcode;					// 0x00	// 0x5A
		UCHAR Plw_no;					// 0x01	// Numerical ID of PLW weapon file
	};

	// 0x5B	// Plc_cnt
	struct Plc_cnt {
		UCHAR Opcode;					// 0x00	// 0x5B
		UCHAR Move_cnt;					// 0x01	// pSce->pWork->Move_cnt
	};

	// 0x5C	// Sce_shake_on
	struct Sce_shake_on {
		UCHAR Opcode;					// 0x00 // 0x5C
		CHAR Slide_ofs;					// 0x01 // line_trans() argv[1] (cannot be zero)
		CHAR Copy_ofs;					// 0x02 // line_trans() argv[2] (cannot be zero)
	};

	// 0x5D	// Mizu_div_set				// Water_div_set
	struct Mizu_div_set {
		UCHAR Opcode;					// 0x00	// 0x5D
		UCHAR Div_max;					// 0x01	// Mizu_div_set() argv[0]
	};

	// 0x5E	// Keep_Item_ck
	struct Keep_Item_ck {
		UCHAR Opcode;					// 0x00	// 0x5E
		UCHAR Item_no;					// 0x01	// Numerical ID of Item

		// Iterate through G.SaveData.Item_work[i] to find Item_no
		// If item is not found, item can be added to inventory
	};

	// 0x5F	// Xa_vol
	struct Xa_vol {
		UCHAR Opcode;					// 0x00	// 0x5F
		UCHAR Cd_vol;					// 0x01	// CD.Vol (must be between 0x00 and 0x7F)
	};

	// 0x60	// Kage_set					// Shadow_set
	struct Kage_set {
		UCHAR Opcode;					// 0x00	// 0x60
		UCHAR Work_type;				// 0x01	// Accepted values: 1 and 3
		// 1 = Player		// pSce->pWork = G.pEnemy[0];
		// 3 = Enemy		// pSce->pWork = G.pEnemy[Work_no];
		CHAR Work_no;					// 0x02	// Enemy ID
		UCHAR R;		    			// 0x03	// pSce->pWork->pKage_work->Prim[0].r0
		UCHAR G;		    			// 0x04	// pSce->pWork->pKage_work->Prim[0].g0
		UCHAR B;		    			// 0x05	// pSce->pWork->pKage_work->Prim[0].b0
		USHORT Half_x;					// 0x06	// pSce->pWork->pKage_work->Half_x
		USHORT Half_z;					// 0x08	// pSce->pWork->pKage_work->Half_z
		USHORT Off_x;					// 0x0A	// pSce->pWork->pKage_work->Off_x
		USHORT Off_z;					// 0x0C	// pSce->pWork->pKage_work->Off_z
	};

	// 0x61	// Cut_be_set
	struct Cut_be_set {
		UCHAR Opcode;					// 0x00	// 0x61
		UCHAR Fcut;						// 0x01	// G.pRoom->pVcut[Vcut_num]->Fcut
		UCHAR Vcut_num;					// 0x02	// G.pRoom->pVcut[Vcut_num]
		UCHAR Be_flg;					// 0x03	// 0 = OFF, 1 = ON
	};

	// 0x62	// Sce_Item_lost
	struct Sce_Item_lost {
		UCHAR Opcode;					// 0x00	// 0x62
		UCHAR Item_no;					// 0x01	// Numerical ID of Item

		// Iterate through G.SaveData.Item_work to find Item_no
		// If found, item is removed from inventory:
		// 
		// if G.SaveData.Item_work[i].Id = Item_no
		//		G.SaveData.Item_work[i].Id = 0
		//		G.SaveData.Item_work[i].Num = 0
		//		G.SaveData.Item_work[i].Size = 0
	};

	// 0x63	// Plc_gun_eff
	struct Plc_gun_eff {
		UCHAR Opcode;					// 0x00	// 0x63
	};

	// 0x64	// Sce_espr_on2
	struct Sce_espr_on2 {
		UCHAR Opcode;					// 0x00	// 0x64
		UCHAR Dir_y_id0;				// 0x01	// Esp_call2() argv[2]
		UCHAR Esp_id;					// 0x02 // Numerical ID of ESP effect file
		UCHAR Esp_dt;					// 0x03 // pEspdt[Esp_dt]
		UCHAR Work_kind;				// 0x04	// Get_matrix() argv[0]
		UCHAR Work_no;					// 0x05	// Get_matrix() argv[1]
		USHORT Espmv;					// 0x06	// Esp_call2() argv[0]
		SHORT Svec_x;					// 0x08	// 
		SHORT Svec_y;					// 0x0A	// 
		SHORT Svec_z;					// 0x0C	// 
		USHORT Dir_y_id1;				// 0x0E	// Esp_call2() argv[2]
	};

	// 0x65	// Sce_espr_kill2
	struct Sce_espr_kill2 {
		UCHAR Opcode;					// 0x00	// 0x65
		UCHAR Esp_id;					// 0x01	// Esp_kill2() argv[0]
	};

	// 0x66	// Plc_stop
	struct Plc_stop {
		UCHAR Opcode;					// 0x00	// 0x66

		// if pSce->pWork->Routine_0 == 0x1404
		//		pSce->pWork->Routine_0 = pSce->R_no_bak[0]
		//		pSce->pWork->Routine_1 = pSce->R_no_bak[1]
		//		pSce->pWork->Routine_2 = pSce->R_no_bak[2]
		//		pSce->pWork->Routine_3 = pSce->R_no_bak[3]
		// else
		//		pSce->pWork->Routine_0 = 0x04
		//		pSce->pWork->Routine_1 = 0x14
		//		pSce->pWork->Routine_2 = 0x00
		//		pSce->pWork->Routine_3 = 0x00
	};

	// 0x67	// Aot_set_4p
	struct Aot_set_4p {
		UCHAR Opcode;					// 0x00 // 0x67
		UCHAR Aot;						// 0x01 // "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02 // Sce Atari Type
		UCHAR Type;						// 0x03 // Sce Atari Flags
		UCHAR nFloor;					// 0x04 // Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05 // Extended Matrix Flag
		SHORT Xz[4][2];					// 0x06 // X0, Z0, X1, Z1, X2, Z2, X3, Z3 Positions
		USHORT Data0;					// 0x16 // Unique per Id
		USHORT Data1;					// 0x18 // Unique per Id
		USHORT Data2;					// 0x1A // Unique per Id
	};

	// 0x68	// Door_aot_set_4p
	struct Door_aot_set_4p {
		UCHAR Opcode;					// 0x00 // 0x68
		UCHAR Aot;						// 0x01 // "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02 // Sce Atari Type (always 0x01, Sce_Door)
		UCHAR Type;						// 0x03 // Sce Atari Flags
		UCHAR nFloor;					// 0x04 // Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05 // Extended Matrix Flag
		SHORT Xz[4][2];					// 0x06 // X0, Z0, X1, Z1, X2, Z2, X3, Z3 Positions
		SHORT Next_pos_x;				// 0x16	// X Position of Player in next RDT
		SHORT Next_pos_y;				// 0x18	// Y Position of Player in next RDT
		SHORT Next_pos_z;				// 0x1A	// Z Position of Player in next RDT
		SHORT Next_cdir_y;				// 0x1C	// Y Rotation of Player in next RDT
		UCHAR Next_stage;				// 0x1E	// Numerical ID of Stage for next RDT file
		UCHAR Next_room;				// 0x1F	// Numerical ID of Room for next RDT file
		UCHAR Next_cut;					// 0x20	// Numerical ID of Cut for next RDT file
		UCHAR Next_nfloor;				// 0x21	// Player distance from ground in next RDT (actual is nFloor * -1800)
		UCHAR Dtex_type;				// 0x22	// Numerical ID of DO2 file
		UCHAR Door_type;				// 0x23	// Numerical ID of DO2 animation
		UCHAR Knock_type;				// 0x24	// Numerical ID of VAG sound from VAB soundbank (always 0x00)
		UCHAR Key_id;					// 0x25	// G.Savegame.Key_flg bit (0x00 for unlocked door)
		UCHAR Key_type;					// 0x26	// Numerical ID of Item required to unlock door
		UCHAR Free;						// 0x27	// always 0x00
	};

	// 0x69	// Item_aot_set_4p
	struct Item_aot_set_4p {
		UCHAR Opcode;					// 0x00 // 0x69
		UCHAR Aot;						// 0x01 // "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02 // Sce Atari Type (always 0x02, Sce_Item)
		UCHAR Type;						// 0x03 // Sce Atari Flags
		UCHAR nFloor;					// 0x04 // Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05 // Extended Matrix Flag
		SHORT Xz[4][2];					// 0x06 // X0, Z0, X1, Z1, X2, Z2, X3, Z3 Positions
		USHORT iItem;					// 0x16 // Numerical ID of Item
		USHORT nItem;					// 0x18 // Numerical count of Item
		USHORT Item_flg;				// 0x1A // G.Savegame.Item_flg or G.Savegame.Item_flg2 Bit
		UCHAR Om_no;					// 0x1C // Numerical ID of MD1 model file (0xFF for none)
		UCHAR Action;					// 0x1D // Predefined action taken to obtain
		// 0 = Stand to obtain
		// 1 = Kneel to obtain
	};

	// 0x6A	// Light_pos_set
	struct Light_pos_set {
		UCHAR Opcode;					// 0x00	// 0x6A
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Pos_no;					// 0x02 // 00, 01 or 02
		UCHAR XYZ;						// 0x03 // 11 = X, 12 = Y, 13 = Z
		SHORT Position;					// 0x04	// G.pRoom->pLight[G.SaveData.Cut_no]->Pos[Pos_no].X/Y/Z = Position
	};

	// 0x6B	// Light_kido_set			// Light_brightness_set
	struct Light_kido_set {
		UCHAR Opcode;					// 0x00	// 0x6B
		UCHAR Brit_no;					// 0x01 // Accepted values: 0-2
		USHORT Brightness;				// 0x02	// G.pRoom->pLight[G.SaveData.Cut_no]->L[Brit_no] = Brightness
	};

	// 0x6C	// Rbj_reset
	struct Rbj_reset {
		UCHAR Opcode;					// 0x00	// 0x6C

		// Sce.Rbj_reset_flg = 0x01
	};

	// 0x6D	// Sce_scr_move
	struct Sce_scr_move {
		UCHAR Opcode;					// 0x00	// 0x6D
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		SHORT Scrl_y;					// 0x02 // G.Scrl_y
	};

	// 0x6E	// Parts_set
	struct Parts_set {
		UCHAR Opcode;					// 0x00	// 0x6E
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Ptr;						// 0x02 // pSce->pWork->pSin_parts_ptr[Ptr]
		UCHAR Work_type;				// 0x03 // Accepted Values: 0-6
		// 0	pSce->pWork->pSin_parts_ptr[Ptr]->Poly_rgb[0]
		// 1	pSce->pWork->pSin_parts_ptr[Ptr]->Poly_rgb[2]
		// 2	pSce->pWork->pSin_parts_ptr[Ptr]->Be_flg[0]
		// 3	pSce->pWork->pSin_parts_ptr[Ptr]->Be_flg[2]
		// 4	pSce->pWork->pSin_parts_ptr[Ptr]->Cdir_x
		// 5	pSce->pWork->pSin_parts_ptr[Ptr]->Cdir_y
		// 6	pSce->pWork->pSin_parts_ptr[Ptr]->Cdir_z
		SHORT Num;						// 0x04 // Value to save to the value referenced by Work_type
	};

	// 0x6F	// Movie_on
	struct Movie_on {
		UCHAR Opcode;					// 0x00	// 0x6F
		UCHAR Str_num;					// 0x01 // Movie_tbl[Str_num]
	};

	// 0x70	// Splc_ret
	struct Splc_ret {
		UCHAR Opcode;					// 0x00	// 0x70
	};

	// 0x71	// Splc_sce
	struct Splc_sce {
		UCHAR Opcode;					// 0x00	// 0x71
	};

	// 0x72	// Super_on
	struct Super_on {
		UCHAR Opcode;					// 0x00	// 0x72
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Work_type;				// 0x02	// 0x00		pSce->pWork->pSuper = GsIDMATRIX
		// 0x40		pSce->pWork->pSuper = G.Player.M
		// 0x80		pSce->pWork->pSuper = G.Player.pTmd2
		// 0xC0		
		UCHAR Work_no;					// 0x03	// 
		//	
		//	if Work_type & 0x40
		// 		(if Work_no & 0x80) { pSce->pWork->pSuper = G.Player.pSin_parts_ptr[Work_no & 0x7F].Workm }
		//		else { pSce->pWork->pSuper = G.Player.M }
		SHORT Mat_x;					// 0x04	// MATRIX.t[0][0]
		SHORT Mat_y;					// 0x06	// MATRIX.t[0][1]
		SHORT Mat_z;					// 0x08	// MATRIX.t[0][2]
		SHORT Rot_x;					// 0x0A	// SVECTOR.vx
		SHORT Rot_y;					// 0x0C	// SVECTOR.vy
		SHORT Rot_z;					// 0x0E	// SVECTOR.vz
	};

	// 0x73	// Mirror_set
	struct Mirror_set {
		UCHAR Opcode;					// 0x00	// 0x73
		UCHAR Flag;						// 0x01	// G.Mirror_flg
		USHORT Position;				// 0x02	// G.Mirror_pos
		USHORT Min;						// 0x04	// G.Mirror_min
		USHORT Max;						// 0x06	// G.Mirror_max
	};

	// 0x74	// Sce_fade_adjust
	struct Sce_fade_adjust {
		UCHAR Opcode;					// 0x00	// 0x74
		UCHAR No;						// 0x01	// Fade_adjust() argv[0]
		SHORT Brightness;				// 0x02	// Fade_adjust() argv[1]
	};

	// 0x75	// Sce_espr3d_on2
	struct Sce_espr3d_on2 {
		UCHAR Opcode;					// 0x00	// 0x75
		UCHAR Dir_y_id0;				// 0x01	// Esp_call3D() argv[2]
		UCHAR Esp_id;					// 0x02 // Numerical ID of ESP effect file
		UCHAR Esp_dt;					// 0x03 // pEspdt[Esp_dt]
		UCHAR Work_kind;				// 0x04 // Get_matrix() argv[0]
		UCHAR Work_no;					// 0x05 // Get_matrix() argv[1]
		USHORT Espmv;					// 0x06 // Esp_call3D() argv[0]
		SHORT Svec_x;					// 0x08 //
		SHORT Svec_y;					// 0x0A //
		SHORT Svec_z;					// 0x0C //
		SHORT Dsv_x;					// 0x0E //
		SHORT Dsv_y;					// 0x10 //
		SHORT Dsv_z;					// 0x12 //
		USHORT Dir_y_id1;				// 0x14	// Esp_call3D() argv[2]
	};

	// 0x76	// Sce_Item_get
	struct Sce_Item_get {
		UCHAR Opcode;					// 0x00	// 0x76
		UCHAR Item_no;					// 0x01	// Numerical ID of Item
		UCHAR Num;						// 0x02	// Numerical count of Item

		// Add an item to the inventory
	};

	// 0x77	// Sce_line_start
	struct Sce_line_start {
		UCHAR Opcode;					// 0x00	// 0x77
		UCHAR Ot_no;					// 0x01	// line_mode_set() argv [0]
		USHORT Prio;					// 0x02	// line_mode_set() argv [1]
	};

	// 0x78	// Sce_line_main
	struct Sce_line_main {
		UCHAR Opcode;					// 0x00	// 0x78
		UCHAR Line_no;					// 0x01	// line_trans() argv [0]
		SHORT Slide_ofs;				// 0x02	// line_trans() argv [1]
		SHORT Copy_ofs;					// 0x04	// line_trans() argv [2]
	};

	// 0x79	// Sce_line_end
	struct Sce_line_end {
		UCHAR Opcode;					// 0x00	// 0x79
	};

	// 0x7A	// Sce_parts_bomb
	struct Sce_parts_bomb {
		UCHAR Opcode;					// 0x00	// 0x7A
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Ptr;						// 0x02 // pSce->pWork->pSin_parts_ptr[Ptr]
		UCHAR Poly_R;					// 0x03 // pSce->pWork->pSin_parts_ptr[Ptr]->Poly_rgb
		UCHAR Poly_G;					// 0x04 // pSce->pWork->pSin_parts_ptr[Ptr]->Poly_rgb
		UCHAR Poly_B;					// 0x05 // pSce->pWork->pSin_parts_ptr[Ptr]->Poly_rgb
		SHORT Free0;					// 0x06 // pSce->pWork->pSin_parts_ptr[Ptr]->Free[0]
		SHORT Free2;					// 0x08 // pSce->pWork->pSin_parts_ptr[Ptr]->Free[2]
		SHORT Free6;					// 0x0A // pSce->pWork->pSin_parts_ptr[Ptr]->Free[6]
		SHORT Free1;					// 0x0C // pSce->pWork->pSin_parts_ptr[Ptr]->Free[1]
		SHORT Free3;					// 0x0E // pSce->pWork->pSin_parts_ptr[Ptr]->Free[3]
	};

	// 0x7B	// Sce_parts_down
	struct Sce_parts_down {
		UCHAR Opcode;					// 0x00	// 0x7B
		UCHAR Ptr;						// 0x01 // pSce->pWork->pSin_parts_ptr[Ptr]
		SHORT Old_x;					// 0x02 // pSce->pWork->pSin_parts_ptr[Ptr]->Old_x
		SHORT Old_y;					// 0x04 // pSce->pWork->pSin_parts_ptr[Ptr]->Old_y
		SHORT Old_z;					// 0x06 // pSce->pWork->pSin_parts_ptr[Ptr]->Old_z
		SHORT Timer1;					// 0x08 // pSce->pWork->pSin_parts_ptr[Ptr]->Timer1
		SHORT Old_x2;					// 0x0A // pSce->pWork->pSin_parts_ptr[Ptr]->Old_x2
		SHORT Old_y2;					// 0x0C // pSce->pWork->pSin_parts_ptr[Ptr]->Old_y2
		SHORT Old_z2;					// 0x0E // pSce->pWork->pSin_parts_ptr[Ptr]->Old_z2
	};

	// 0x7C	// Light_color_set
	struct Light_color_set {
		UCHAR Opcode;					// 0x00	// 0x7C
		UCHAR Col_no;					// 0x01	// Accepted Values: 0-2
		UCHAR R;						// 0x02	// G.pRoom->pLight[G.SaveData.Cut_no]->Col[Col_no][0]
		UCHAR G;						// 0x03	// G.pRoom->pLight[G.SaveData.Cut_no]->Col[Col_no][1]
		UCHAR B;						// 0x04	// G.pRoom->pLight[G.SaveData.Cut_no]->Col[Col_no][2]
		UCHAR Nop;						// 0x05 // always 0x00 (opcode for Nop)
	};

	// 0x7D	// Light_pos_set2
	struct Light_pos_set2 {
		UCHAR Opcode;					// 0x00	// 0x7D
		UCHAR Lit_no;					// 0x01	// Accepted values: 0-15
		UCHAR Pos_no;					// 0x02	// Accepted values: 0-2
		UCHAR XYZ;						// 0x03	// 11 = X, 12 = Y, 13 = Z
		SHORT Position;					// 0x04	// G.pRoom->pLight[Lit_no].Pos[Pos_no].X/Y/Z = Position
	};

	// 0x7E	// Light_kido_set2
	struct Light_kido_set2 {
		UCHAR Opcode;					// 0x00	// 0x7E
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR Lit_no;					// 0x02	// Accepted values: 0-15
		UCHAR Brit_no;					// 0x03	// Accepted values: 0-2
		USHORT Brightness;				// 0x04	// G.pRoom->pLight[Lit_no].L[Brit_no] = Brightness
	};

	// 0x7F	// Light_color_set2
	struct Light_color_set2 {
		UCHAR Opcode;					// 0x00	// 0x7F
		UCHAR Lit_no;					// 0x01	// Accepted values: 0-15
		UCHAR Col_no;					// 0x02	// Accepted values: 0-2
		UCHAR R;						// 0x03	// G.pRoom->pLight[Lit_no].Col[Col_no][0]
		UCHAR G;						// 0x04	// G.pRoom->pLight[Lit_no].Col[Col_no][1]
		UCHAR B;						// 0x05	// G.pRoom->pLight[Lit_no].Col[Col_no][2]
	};

	// 0x80	// Se_vol
	struct Se_vol {
		UCHAR Opcode;					// 0x00	// 0x80
		UCHAR Volume_Se;				// 0x01	// Accepted values: 0-255
	};

	// 0x81	// Sce_Item_cmp (room60A, only)
	struct Sce_Item_cmp {
		UCHAR Opcode;					// 0x00 // 0x81
		UCHAR Operator;					// 0x01 // Same as Cmp
		UCHAR Num;						// 0x02 // Numerical count of Equipped/Used Item
	};

	// 0x82	// Clears all ESP tasks
	struct Sce_espr_kill_all {
		UCHAR Opcode;					// 0x00 // 0x82
		CHAR Work_kind;					// 0x01 // Get_matrix() argv[0]
		CHAR Work_no;					// 0x02 // Get_matrix() argv[1]
	};

	// 0x83	// Used only once (room410), disable poison, restore Life
	struct Plc_heal {
		UCHAR Opcode;					// 0x00 // 0x83
	};

	// 0x84	// Trigger an automatic map check
	struct St_map_hint {
		UCHAR Opcode;					// 0x00 // 0x84
		UCHAR Tag;						// 0x01 // St.Dmode[3][0].tag
	};

	// 0x85	// Used once (room109), check player/enemy position
	struct Sce_em_pos_ck {
		UCHAR Opcode;					// 0x00 // 0x85
		UCHAR Em_no0;					// 0x01 // G.pEnemy[Em_no0];
		UCHAR Em_no1;					// 0x02 // G.pEnemy[Em_no1];
		UCHAR Flg;						// 0x03 // Sca_ck_line() argv[3]
		USHORT Att;						// 0x04 // Sca_ck_line() argv[2]
	};

	// 0x86	// Check poison status, used for "Blue Herb is growing here."
	struct Poison_ck {
		UCHAR Opcode;					// 0x00 // 0x86
	};

	// 0x87	// Disable poison status, intended for Blue Herb usage
	struct Poison_clr {
		UCHAR Opcode;					// 0x00 // 0x87
	};

	// 0x88	// Alternate version of Sce_Item_lost
	struct Sce_Item_lost2 {
		UCHAR Opcode;					// 0x00 // 0x88
		UCHAR Item_no;					// 0x01 // Numerical ID of Item
		UCHAR Num;						// 0x02 // G.Savegame.Item_work[Item_no].Num
	};

	// 0x89	// Sets sub-player life to G.pEnemy[1]->Max_life (200 for Leon disk), restores health
	struct Plc_life_max {
		UCHAR Opcode;					// 0x00 // 0x89
	};

	// 0x8A	// Dual Shock Controller vibration
	struct Vib_set0 {
		UCHAR Opcode;					// 0x00	// 0x8A
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		USHORT data0;					// 0x02	//
		USHORT data1;					// 0x04	//
	};

	// 0x8B	// Dual Shock Controller vibration
	struct Vib_set1 {
		UCHAR Opcode;					// 0x00	// 0x8B
		UCHAR Id;						// 0x01	//
		USHORT data0;					// 0x02	//
		USHORT data1;					// 0x04	//
	};

	// 0x8C	// Dual Shock Controller vibration
	struct Vib_fade_set {
		UCHAR Opcode;					// 0x00	// 0x8C
		UCHAR Nop;						// 0x01 // always 0x00 (opcode for Nop)
		UCHAR data0;					// 0x02	//
		UCHAR data1;					// 0x03	//
		USHORT data2;					// 0x04	//
		USHORT data3;					// 0x06	//
	};

	// 0x8D	// Alternate version of Item_aot_set2 (EX Battle, only)
	struct Item_aot_set2 {
		UCHAR Opcode;					// 0x00	// 0x8D
		UCHAR Aot;						// 0x01	// "Ahead of Time" object pool ID
		UCHAR Id;						// 0x02	// Sce Atari Type (always 0x02, Sce_Item)
		UCHAR Type;						// 0x03	// Sce Atari Flags
		UCHAR nFloor;					// 0x04	// Distance from ground (actual is nFloor * -1800)
		UCHAR Super;					// 0x05	// Extended Matrix Flag
		SHORT X;						// 0x06	// X Position
		SHORT Z;						// 0x08	// Z Position
		USHORT W;						// 0x0A	// Width Size
		USHORT D;						// 0x0C	// Depth Size
		USHORT iItem;					// 0x0E	// Numerical ID of Item
		USHORT nItem;					// 0x10	// Numerical count of Item
		USHORT Item_flg;				// 0x12	// G.Savegame.Item_flg or G.Savegame.Item_flg2 Bit
		UCHAR Om_no;					// 0x14	// Numerical ID of MD1 model file (0xFF for none)
		UCHAR Action;					// 0x15	// Predefined action taken to obtain
		// 0 = Stand to obtain
		// 1 = Kneel to obtain
		UCHAR Rnd_flg;					// 0x16	// Something to do with G.Savegame.Item_flg/Item_flg2
		UCHAR Nop;						// 0x17 // always 0x00 (opcode for Nop)
	};

	// 0x8E	// Alternate version of Sce_em_set (EX Battle, only)
	struct Sce_em_set2 {
		UCHAR Opcode;					// 0x00	// 0x8E
		UCHAR Nop0;						// 0x01	// always 0x00 (opcode for Nop)
		UCHAR Em_no;					// 0x02	// Enemy object pool ID
		UCHAR Id;						// 0x03	// Numerical ID of EMD file
		USHORT Type;					// 0x04	// 
		UCHAR nFloor;					// 0x06	// Distance from ground (actual is nFloor * -1800)
		UCHAR Sound_flg;				// 0x07	// 
		UCHAR Model_type;				// 0x08	// 
		UCHAR Em_set_flg;				// 0x09	// G.Savegame.Enemy_flg or G.Savegame.Enemy_flg2 bit
		SHORT Pos_x;					// 0x0A	// X Position
		SHORT Pos_y;					// 0x0C	// Y Position
		SHORT Pos_z;					// 0x0E	// Z Position
		SHORT Cdir_y;					// 0x10	// Y Rotation
		SHORT Motion;					// 0x12	// G.pFree_work->Timer0
		SHORT Ctr_flg;					// 0x14	// G.pFree_work->Timer1
		UCHAR Rnd_flg;					// 0x16	// Something to do with G.Savegame.Enemy_flg/Enemy_flg2
		/*
			if ((Rnd() * 100) >> 8) < Rnd_flg) {
			  Rnd_flg = pSce_em[9];
			}
			else {
			  Rnd_flg = pSce_em[9];
			  p = p_00;
			}
			Flg_on(Em_set_flg, Rnd_flg);
		*/
		UCHAR Nop1;						// 0x17	// always 0x00 (opcode for Nop)
	};

#pragma pack(pop)

};