
/*********************
BOARD GARDB2
Arduino MICRO

***********************/
//#define DEBUG

#include "bconf/StandardArduino.h"             // Define the board type
#include "conf/usart.h"                     // USART RS485

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"

#define TEMP 		0
#define TEMP_H 	1

inline void DefineTypicals()
{
	Set_Temperature(TEMP);
}

inline void ReadInputs()
{
}

inline void ProcessLogics()
{
		grh_Logic_Temperature(TEMP);
}

inline void SetOutputs()
{
}

inline void ProcessTimers()
{
}




void setup()
{
	grhOpenSerialOnDebug();

	grhSetUSARTAddress(RS485_ADDRESS_GARDB2, RS485_ADDRESS_GARDB1);
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

//		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		SLOW_PeerJoin();
	}
}
