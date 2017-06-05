
/*********************
BOARD GARDB1
Arduino UNO + ETH Shield on a IONO SOLO BOARD

***********************/
//#define DEBUG
#include "SoulissFramework.h"

#include "bconf/IonoEthernet.h"             // Define the board type

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"
#include "HW_Setup_IONO.h"

inline void DefineTypicals()
{
	// Define logics for the node
	Set_T12(GARDB1_LIGHT_GARDEN);

	Set_T12(GARDB1_WATERING_ZONE1);
	Set_T12(GARDB1_WATERING_ZONE2);
	Set_T12(GARDB1_WATERING_ZONE3);
	Set_T12(GARDB1_WATERING_ZONE4);
	Set_T12(GARDB1_WATERING_ZONE5);

	Set_Power(GARDB1_TOTAL_POWER);


	// set default values
	SetInput(GARDB1_WATERING_ZONE1, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE2, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE3, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE4, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE5, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_LIGHT_GARDEN, Souliss_T1n_AutoCmd);
}

double read_count = 0;
double Isum = 0;
float Irms;

#define I_OFFSET	0	// TODO: modify this
#define I_RATIO		1	// TODO: modify this

inline void ReadCurrent()
{
	float current = aRead(AI1) - I_OFFSET;
	read_count++;
	Isum += current * current;
}

inline void CalculateIrms()
{
	Irms = I_RATIO * sqrt(Isum / read_count);
	read_count = 0;
	Isum = 0;
}

inline void ReadInputs()
{
	//DigIn(DI5, Souliss_T1n_ToggleCmd, GARDB1_LIGHT_GARDEN);          // Read inputs from DI5
}

inline void ProcessLogics()
{
	Logic_T12(GARDB1_LIGHT_GARDEN);                                  // Execute the logic for Relay 6

	Logic_T12(GARDB1_WATERING_ZONE1);                                  // Execute the logic for Relay 1
	Logic_T12(GARDB1_WATERING_ZONE2);                                  // Execute the logic for Relay 2
	Logic_T12(GARDB1_WATERING_ZONE3);                                  // Execute the logic for Relay 3
	Logic_T12(GARDB1_WATERING_ZONE4);                                  // Execute the logic for Relay 4
	Logic_T12(GARDB1_WATERING_ZONE5);                                  // Execute the logic for Relay 5

	Logic_Power(GARDB1_TOTAL_POWER);
}

inline void SetOutputs()
{
	nDigOut(DO1, Souliss_T1n_Coil, GARDB1_WATERING_ZONE1);              // Drive the Relay 1
	nDigOut(DO2, Souliss_T1n_Coil, GARDB1_WATERING_ZONE2);              // Drive the Relay 2
	nDigOut(DO3, Souliss_T1n_Coil, GARDB1_WATERING_ZONE3);              // Drive the Relay 3
	nDigOut(DO4, Souliss_T1n_Coil, GARDB1_WATERING_ZONE4);              // Drive the Relay 4
	nDigOut(DO5, Souliss_T1n_Coil, GARDB1_WATERING_ZONE5);              // Drive the Relay 5
	nDigOut(DO6, Souliss_T1n_Coil, GARDB1_LIGHT_GARDEN);              // Drive the Relay 6
}

inline void ProcessTimers()
{
	Timer_T12(GARDB1_WATERING_ZONE1);
	Timer_T12(GARDB1_WATERING_ZONE2);
	Timer_T12(GARDB1_WATERING_ZONE3);
	Timer_T12(GARDB1_WATERING_ZONE4);
	Timer_T12(GARDB1_WATERING_ZONE5);
	Timer_T12(GARDB1_LIGHT_GARDEN);
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
		
		ReadCurrent();

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

		SLOW_10s()
		{
			CalculateIrms();
			ImportAnalog(GARDB1_TOTAL_POWER, &Irms);
		}
	}
}
