/*********************
BOARD ROW1B2
DINO with RS485 only acting as Peer

***********************/

//#define DEBUG


#include "bconf/DINo_v2_RS485.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_DINo_v2.h"

#define FAN_SPEED1			1
#define FAN_SPEED2			2		
#define FAN_SPEED3			3	
#define LIGHT_GW			4

inline void DefineTypicals()
{
	Set_T12(FAN_SPEED1);
	Set_T12(FAN_SPEED2);
	Set_T12(FAN_SPEED3);
	Set_SimpleLight(LIGHT_GW);
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_ToggleCmd, memory_map, LIGHT_GW, true);
}

inline void ProcessLogics()
{
	Logic_T12(FAN_SPEED1);
	Logic_T12(FAN_SPEED2);
	Logic_T12(FAN_SPEED3);
	Logic_SimpleLight(LIGHT_GW);

}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T2n_Coil_Close,  FAN_SPEED1);
	DigOut(RELAY2, Souliss_T2n_Coil_Close, FAN_SPEED2);
	DigOut(RELAY3, Souliss_T2n_Coil_Close,  FAN_SPEED3);
	LowDigOut(RELAY4, Souliss_T2n_Coil_Close, LIGHT_GW);
}

inline void ProcessTimers()
{
	Timer_T12(FAN_SPEED1);
	Timer_T12(FAN_SPEED2);
	Timer_T12(FAN_SPEED3);
	Timer_SimpleLight(LIGHT_GW);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_GTW2B1, RS485_ADDRESS_ROW2B1);


	DefineTypicals();
}

void loop()
{ 
	EXECUTEFAST() {						
		UPDATEFAST();
		
		FAST_30ms() 
		{
			ReadInputs();
		} 

		FAST_50ms() 
		{
			ProcessLogics();

			SetOutputs();
		}
		
		FAST_2110ms()
		{
			ProcessTimers();
		}

		grhFastPeerComms();			
	}
	
	EXECUTESLOW() 
	{	
		UPDATESLOW();
		SLOW_PeerJoin();
	}
} 