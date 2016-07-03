#ifndef GRH_FANCOILS_H
#define GRH_FANCOILS_H

#include "grhSoulissSlots.h"

#define AUTO_CYCLES_COUNT 10

inline void Fancoil_AutoCmd(U8 idx, U8 speed)
{
	U8 slot = GTW1B1_FAN_SPEED1;
	switch(speed)
	{
		case 2:
		slot = GTW1B1_FAN_SPEED2;
		break;

		case 3:
		slot = GTW1B1_FAN_SPEED3;
		break;
	}

	U16 addr = (idx % 2) ? RS485_ADDRESS_GTW1B1 : RS485_ADDRESS_GTW2B1;
	RemoteInput(addr, slot, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT);
}

#endif
