/*********************
BOARD ROW1B2
DINO with ETH only acting as Peer

***********************/

//#define DEBUG
#include "SoulissFramework.h"

#include "bconf/DINo_v2.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"
#include "HW_Setup_DINo_v2.h"


inline void DefineTypicals()
{
	Set_Temperature(LOFTB2_HVAC_SPARE_SANITARY_WATER_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_EXT_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_INT_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP);
}

inline void ReadInputs()
{
	// input only coming from other boards
}

inline void ProcessLogics()
{
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_EXT_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_INT_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP);
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
	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_LOFTB2);

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
