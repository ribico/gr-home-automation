/*********************
BOARD ROW1B2
DINO with RS485 only acting as Peer

***********************/

//#define DEBUG
#include "SoulissFramework.h"

#include "bconf/DINo_v2_RS485.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"
#include "HW_Setup_DINo_v2.h"

//--------------------------------------


inline void DefineTypicals()
{
	Souliss_SetT22(memory_map, ROW2B2_WINDOW_LIVING);
	Souliss_SetT22(memory_map, ROW2B2_WINDOW_KITCHEN);
}

inline void ReadInputs()
{
	DigInWindowToggle(IN1, ROW2B2_WINDOW_LIVING);
	DigInWindowToggle(IN2, ROW2B2_WINDOW_KITCHEN);
}

inline void ProcessLogics()
{
	Souliss_Logic_T22(memory_map, ROW2B2_WINDOW_LIVING, &data_changed, SHUTTER_SHORT_TIMEOUT);
	Souliss_Logic_T22(memory_map, ROW2B2_WINDOW_KITCHEN, &data_changed, SHUTTER_LONG_TIMEOUT);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T2n_Coil_Open,  ROW2B2_WINDOW_LIVING);
	DigOut(RELAY2, Souliss_T2n_Coil_Close, ROW2B2_WINDOW_LIVING);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  ROW2B2_WINDOW_KITCHEN);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, ROW2B2_WINDOW_KITCHEN);
}

inline void ProcessTimers()
{
	Timer_Windows(ROW2B2_WINDOW_LIVING);
	Timer_Windows(ROW2B2_WINDOW_KITCHEN);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B2, RS485_ADDRESS_ROW2B1);

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
		grh_SLOW_PeerJoin();
	}
}
