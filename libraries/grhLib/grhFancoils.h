#ifndef GRH_FANCOILS_H
#define GRH_FANCOILS_H

//int fancoils_status[2];

static U8 FANCOIL_OFF[] = {Souliss_T1n_OffCmd, Souliss_T1n_OffCmd, Souliss_T1n_OffCmd};
static U8 FANCOIL_SPEED1[] = {Souliss_T1n_OnCmd, Souliss_T1n_OffCmd, Souliss_T1n_OffCmd};
static U8 FANCOIL_SPEED2[] = {Souliss_T1n_OnCmd, Souliss_T1n_OnCmd, Souliss_T1n_OffCmd};
static U8 FANCOIL_SPEED3[] = {Souliss_T1n_OnCmd, Souliss_T1n_OnCmd, Souliss_T1n_OnCmd};

inline void Fancoil_Cmds(U8 idx, U8 status)
{
//	if (fancoils_status[idx] == status)
//		return;

//	fancoils_status[idx] = status;

	U16 addr = (idx % 2) ? RS485_ADDRESS_GTW1B1 : RS485_ADDRESS_GTW2B1;

/*	if(addr == RS485_ADDRESS_GTW1B1)
		Serial.print("Sending to GW1");
	else if(addr == RS485_ADDRESS_GTW2B1)
		Serial.print("Sending to GW2");
	else
		Serial.print("** ADDRESS ERROR **");

	Serial.print(" {");
	Serial.print(status);
	Serial.println(" }");
*/
	switch(status)
	{
		case 0:
		default:
		RemoteInputs(addr, 1, FANCOIL_OFF, 3);
		break;

		case 1:
		RemoteInputs(addr, 1, FANCOIL_SPEED1, 3);
		break;

		case 2:
		RemoteInputs(addr, 1, FANCOIL_SPEED2, 3);
		break;

		case 3:
		RemoteInputs(addr, 1, FANCOIL_SPEED2, 3);
		break;
	}
}


#define Fancoil_Off(idx)	Fancoil_Cmds(idx, 0)
#define Fancoil_Speed1(idx)	Fancoil_Cmds(idx, 1)
#define Fancoil_Speed2(idx)	Fancoil_Cmds(idx, 2)
#define Fancoil_Speed3(idx)	Fancoil_Cmds(idx, 3)


#endif
