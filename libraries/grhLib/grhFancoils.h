#ifndef GRH_FANCOILS_H
#define GRH_FANCOILS_H

inline void FancoilGW1_Speed1()
{
	RemoteInput(RS485_ADDRESS_GTW1B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 1, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 2, Souliss_T1n_OffCmd);
}	

inline void FancoilGW1_Speed2()
{
	RemoteInput(RS485_ADDRESS_GTW1B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 1, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 2, Souliss_T1n_OffCmd);
}	

inline void FancoilGW1_Speed3()
{
	RemoteInput(RS485_ADDRESS_GTW1B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 1, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 2, Souliss_T1n_OnCmd);
}	

inline void FancoilGW1_Off()
{
	RemoteInput(RS485_ADDRESS_GTW1B1, 0, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 1, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW1B1, 2, Souliss_T1n_OffCmd);
}	

inline void FancoilGW2_Speed1()
{
	RemoteInput(RS485_ADDRESS_GTW2B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 1, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 2, Souliss_T1n_OffCmd);
}	

inline void FancoilGW2_Speed2()
{
	RemoteInput(RS485_ADDRESS_GTW2B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 1, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 2, Souliss_T1n_OffCmd);
}	

inline void FancoilGW2_Speed3()
{
	RemoteInput(RS485_ADDRESS_GTW2B1, 0, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 1, Souliss_T1n_OnCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 2, Souliss_T1n_OnCmd);
}	

inline void FancoilGW2_Off()
{
	RemoteInput(RS485_ADDRESS_GTW2B1, 0, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 1, Souliss_T1n_OffCmd);
	RemoteInput(RS485_ADDRESS_GTW2B1, 2, Souliss_T1n_OffCmd);
}
#endif