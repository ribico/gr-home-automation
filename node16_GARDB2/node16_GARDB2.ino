
/*********************
BOARD GARDB2
Arduino MICRO

***********************/
//#define DEBUG

#include "bconf/StandardArduino.h"             // Define the board type

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"


inline void DefineTypicals()
{

}

inline void ReadInputs()
{
}

inline void ProcessLogics()
{
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

		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		SLOW_PeerJoin();
	}
}
