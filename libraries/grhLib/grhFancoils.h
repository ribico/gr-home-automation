#ifndef GRH_FANCOILS_H
#define GRH_FANCOILS_H

inline void Fancoil_AutoCmd(U8 idx)
{
	U16 addr = (idx % 2) ? RS485_ADDRESS_GTW1B1 : RS485_ADDRESS_GTW2B1;
	RemoteInput(addr, 1, Souliss_T1n_AutoCmd + 10);
}


#endif
