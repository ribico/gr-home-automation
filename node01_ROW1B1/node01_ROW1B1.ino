
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG

#include "bconf/DINo_v2_EthernetBridge_RS485.h"
#include "conf/SuperNode.h" 

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_DINo_v2.h"



#define LIGHT_LIVING_1			0
#define LIGHT_LIVING_2			1
#define LIGHT_LIVING_3			2		
#define LIGHT_LIVING_4			3
#define LIGHT_LIVING_5			4
#define LIGHT_LIVING_6			5

inline void DefineTypicals()
{
	Set_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
}

inline void ReadInputs()
{
	if( LightsGroupIn(IN1, LIGHT_LIVING_1, LIGHT_LIVING_6) == MaCaco_DATACHANGED)
	{
		RemoteInput(RS485_ADDRESS_ROW1B3, 4, mInput(LIGHT_LIVING_5)); // -> LIGHT_STAIRS
		delay(50);
		RemoteInput(RS485_ADDRESS_ROW1B4, 1, mInput(LIGHT_LIVING_6)); // -> LIGHT_ENTRANCE_1
	}
}

inline void ProcessLogics()
{
	Logic_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_LIVING_1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_LIVING_2);
	DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_LIVING_3);
	DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING_4); 
	// LIGHT_LIVING_5 is remote on RS485_ADDRESS_ROW1B3 -> LIGHT_STAIRS
	// LIGHT_LIVING_6 is remote on RS485_ADDRESS_ROW1B4 -> LIGHT_ENTRANCE_1
}

inline void ProcessTimers()
{
	Timer_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
}




void setup()
{
	grhOpenSerialOnDebug();
	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_ROW1B1);
	grhSetUSARTAddress(RS485_ADDRESS_ROW1B1, 0x0000);

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