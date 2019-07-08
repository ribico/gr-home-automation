
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

#define WATERING_TIMEOUT_CYCLES		36

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

	Set_T12(GARDB1_BUYING_POWER);
	Set_T12(GARDB1_SELLING_POWER);
	
	// set default values
	SetInput(GARDB1_WATERING_ZONE1, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE2, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE3, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE4, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE5, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_LIGHT_GARDEN, Souliss_T1n_AutoCmd);

	SetInput(GARDB1_BUYING_POWER, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_SELLING_POWER, Souliss_T1n_AutoCmd);
}

static unsigned long time_last_pulse = 0;
float fPower = 0.0;

inline void ReadInputs()
{
	U8 ret_sell = LowDigIn(DI5, Souliss_T1n_AutoCmd+100, GARDB1_SELLING_POWER);
	U8 ret_buy = LowDigIn(DI6, Souliss_T1n_AutoCmd+100, GARDB1_BUYING_POWER);

	if (ret_sell > Souliss_T1n_AutoCmd || ret_buy > Souliss_T1n_AutoCmd) // new pulse detected
	{
		unsigned long time_this_pulse = millis();
		// each pulse is set to be 1Wh = 1W*3.600s = 1W*3.600.000ms
		// W = 3.600.000 / delta time [ms]
		fPower = 3600000/(abs(time_this_pulse-time_last_pulse));
		time_last_pulse = time_this_pulse;

		if (ret_sell > Souliss_T1n_AutoCmd) // selling power to the grid
		{
			fPower = fPower * -1;
			SetInput(GARDB1_BUYING_POWER, Souliss_T1n_AutoCmd); // set to AUTO OFF.. do not wait the timer
		}
		else if(ret_buy > Souliss_T1n_AutoCmd) // buying power from the grid
		{
			SetInput(GARDB1_SELLING_POWER, Souliss_T1n_AutoCmd); // set to AUTO OFF.. do not wait the timer
		}
	}	
}

inline void ProcessLogics()
{
	Logic_T12(GARDB1_LIGHT_GARDEN);                                  // Execute the logic for Relay 6

	// to avoid permanent opening of watering valve
	// convert the first activation to AutoCmd + WATERING_TIMEOUT
	if( mInput(GARDB1_WATERING_ZONE1) == Souliss_T1n_OnCmd )
		SetInput(GARDB1_WATERING_ZONE1, Souliss_T1n_AutoCmd+WATERING_TIMEOUT_CYCLES);

	if( mInput(GARDB1_WATERING_ZONE2) == Souliss_T1n_OnCmd )
		SetInput(GARDB1_WATERING_ZONE2, Souliss_T1n_AutoCmd+WATERING_TIMEOUT_CYCLES);

	if( mInput(GARDB1_WATERING_ZONE3) == Souliss_T1n_OnCmd )
		SetInput(GARDB1_WATERING_ZONE3, Souliss_T1n_AutoCmd+WATERING_TIMEOUT_CYCLES);

	if( mInput(GARDB1_WATERING_ZONE4) == Souliss_T1n_OnCmd )
		SetInput(GARDB1_WATERING_ZONE4, Souliss_T1n_AutoCmd+WATERING_TIMEOUT_CYCLES);

	if( mInput(GARDB1_WATERING_ZONE5) == Souliss_T1n_OnCmd )
		SetInput(GARDB1_WATERING_ZONE5, Souliss_T1n_AutoCmd+WATERING_TIMEOUT_CYCLES);


	Logic_T12(GARDB1_WATERING_ZONE1);                                  // Execute the logic for Relay 1
	Logic_T12(GARDB1_WATERING_ZONE2);                                  // Execute the logic for Relay 2
	Logic_T12(GARDB1_WATERING_ZONE3);                                  // Execute the logic for Relay 3
	Logic_T12(GARDB1_WATERING_ZONE4);                                  // Execute the logic for Relay 4
	Logic_T12(GARDB1_WATERING_ZONE5);                                  // Execute the logic for Relay 5

	Logic_Power(GARDB1_TOTAL_POWER);

	Logic_T12(GARDB1_BUYING_POWER);
	Logic_T12(GARDB1_SELLING_POWER);
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

inline void ProcessSlowTimers()
{
	Timer_T12(GARDB1_WATERING_ZONE1);
	Timer_T12(GARDB1_WATERING_ZONE2);
	Timer_T12(GARDB1_WATERING_ZONE3);
	Timer_T12(GARDB1_WATERING_ZONE4);
	Timer_T12(GARDB1_WATERING_ZONE5);
}

inline void ProcessFastTimers()
{
	Timer_T12(GARDB1_LIGHT_GARDEN);
	Timer_T12(GARDB1_BUYING_POWER);
	Timer_T12(GARDB1_SELLING_POWER);
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
			ProcessFastTimers();
		}

		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		grh_SLOW_PeerJoin();

		SLOW_10s()
		{
			ImportAnalog(GARDB1_TOTAL_POWER, &fPower);
		}

		SLOW_50s()
		{
			ProcessSlowTimers();
		}

	}
}
