#ifndef GRH_FANCOILS_H
#define GRH_FANCOILS_H

inline void Fancoil_Cmds(U8 addr, U8 cmd1, U8 cmd2, U8 cmd3)
{
	U8 cmd_buff[] = {cmd1, cmd2, cmd3};
	RemoteInputs(addr, 0, cmd_buff, 3);
}	

#define FANCOIL_ADDR(idx)	(idx % 2) ? RS485_ADDRESS_GTW1B1 : RS485_ADDRESS_GTW2B1

#define Fancoil_Off(idx)	Fancoil_Cmds(FANCOIL_ADDR(idx), Souliss_T1n_OffCmd, Souliss_T1n_OffCmd, Souliss_T1n_OffCmd)
#define Fancoil_Speed1(idx)	Fancoil_Cmds(FANCOIL_ADDR(idx), Souliss_T1n_OnCmd, Souliss_T1n_OffCmd, Souliss_T1n_OffCmd)
#define Fancoil_Speed2(idx)	Fancoil_Cmds(FANCOIL_ADDR(idx), Souliss_T1n_OnCmd, Souliss_T1n_OnCmd, Souliss_T1n_OffCmd)
#define Fancoil_Speed3(idx)	Fancoil_Cmds(FANCOIL_ADDR(idx), Souliss_T1n_OnCmd, Souliss_T1n_OnCmd, Souliss_T1n_OnCmd)


#endif