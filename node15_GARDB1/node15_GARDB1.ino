
/*********************
BOARD GARDB1
Arduino UNO + ETH Shield on a IONO SOLO BOARD

***********************/
//#define DEBUG

#include "bconf/IonoEthernet.h"             // Define the board type

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_IONO.h"

#define WATERING_ZONE1		0
#define WATERING_ZONE2		1
#define WATERING_ZONE3		2
#define WATERING_ZONE4		3
#define LIGHT_NIGHT       4
#define LIGHT_GARDEN			5

inline void DefineTypicals()
{
	// Define logics for the node
	Set_T11(WATERING_ZONE1);
	Set_T11(WATERING_ZONE2);
	Set_T11(WATERING_ZONE3);
	Set_T11(WATERING_ZONE4);
	Set_T11(LIGHT_NIGHT);
	Set_T11(LIGHT_GARDEN);
}

inline void ReadInputs()
{
	DigIn(DI5, Souliss_T1n_ToggleCmd, LIGHT_NIGHT);          // Read inputs from DI5
	DigIn(DI6, Souliss_T1n_ToggleCmd, LIGHT_GARDEN);          // Read inputs from DI6
}

inline void ProcessLogics()
{
	Logic_T11(WATERING_ZONE1);                                  // Execute the logic for Relay 1
	Logic_T11(WATERING_ZONE2);                                  // Execute the logic for Relay 2
	Logic_T11(WATERING_ZONE3);                                  // Execute the logic for Relay 3
	Logic_T11(WATERING_ZONE4);                                  // Execute the logic for Relay 4
	Logic_T11(LIGHT_NIGHT);                                  // Execute the logic for Relay 5
	Logic_T11(LIGHT_GARDEN);                                  // Execute the logic for Relay 6
}

inline void SetOutputs()
{
	DigOut(DO1, Souliss_T1n_Coil, WATERING_ZONE1);              // Drive the Relay 1
	DigOut(DO2, Souliss_T1n_Coil, WATERING_ZONE2);              // Drive the Relay 2
	DigOut(DO3, Souliss_T1n_Coil, WATERING_ZONE3);              // Drive the Relay 3
	DigOut(DO4, Souliss_T1n_Coil, WATERING_ZONE4);              // Drive the Relay 4
	DigOut(DO5, Souliss_T1n_Coil, LIGHT_NIGHT);              // Drive the Relay 5
	DigOut(DO6, Souliss_T1n_Coil, LIGHT_GARDEN);              // Drive the Relay 6
}

inline void ProcessTimers()
{
	Timer_T11(WATERING_ZONE1);
	Timer_T11(WATERING_ZONE1);
	Timer_T11(WATERING_ZONE1);
	Timer_T11(WATERING_ZONE1);
	Timer_T11(LIGHT_NIGHT);
	Timer_T11(LIGHT_GARDEN);
}




void setup()
{
	grhOpenSerialOnDebug();
	InitIono();

	grhSetIpAddress(IP_ADDRESS_GARDB1);

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
