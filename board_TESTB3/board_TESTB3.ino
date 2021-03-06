/*********************
TEST BOARD DINO with RS485 only acting as Peer

***********************/

#define LIGHT_LIVING_1			1
#define LIGHT_LIVING_2			2
#define LIGHT_LIVING_3			3		
#define LIGHT_LIVING_4			4


//#define DEBUG


#include "bconf/DINo_v2_RS485.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_DINo_v2.h"

void setup()
{
	grhOpenSerialOnDebug();
	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_TESTB3, RS485_ADDRESS_TESTB2);


	// DEFINE TYPICALS
	Set_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);
}

void loop()
{ 
	EXECUTEFAST() {						
		UPDATEFAST();
		
		FAST_30ms() 
		{
			// SOULISS_READ_INPUTS
			LightsGroupIn(IN1, LIGHT_LIVING_1, LIGHT_LIVING_4);
		} 

		FAST_50ms() 
		{
			// SOULISS_PROCESS_LOGICS
			Logic_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);

			// SOULISS_SET_OUTPUT
			DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_LIVING_1);
			DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_LIVING_2);
			DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_LIVING_3);
			DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING_4); 
		}
		
		FAST_2110ms()
		{
			// SOULISS_PROCESS_TIMERS;
			Timer_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);
				
		}

		grhFastPeerComms();			
	}
	
	EXECUTESLOW() 
	{	
		UPDATESLOW();
		SLOW_PeerJoin();
	}
} 