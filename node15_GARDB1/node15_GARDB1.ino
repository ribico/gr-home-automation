
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
#include "grhSoulissSlots.h"
#include "HW_Setup_IONO.h"

// cicles of 70s (about 1 min)
#define WATERING_DEFAULT_CICLES	10

inline void DefineTypicals()
{
	// Define logics for the node
	Set_T12(GARDB1_WATERING);

	Set_T12(GARDB1_WATERING_ZONE1);
	Set_T61(GARDB1_WATERING_TIME_ZONE1);

	Set_T12(GARDB1_WATERING_ZONE2);
	Set_T61(GARDB1_WATERING_TIME_ZONE2);

	Set_T12(GARDB1_WATERING_ZONE3);
	Set_T61(GARDB1_WATERING_TIME_ZONE3);

	Set_T12(GARDB1_WATERING_ZONE4);
	Set_T61(GARDB1_WATERING_TIME_ZONE4);

	Set_T12(GARDB1_WATERING_ZONE5);
	Set_T61(GARDB1_WATERING_TIME_ZONE5);

	Set_T12(GARDB1_WATERING_ZONE6); // handled by ROW2B4
	Set_T61(GARDB1_WATERING_TIME_ZONE6);

	Set_T12(GARDB1_LIGHT_GARDEN);
	Set_Power(GARDB1_TOTAL_POWER);


	// set default values
	SetInput(GARDB1_WATERING_ZONE1, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE2, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE3, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE4, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE5, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_WATERING_ZONE6, Souliss_T1n_AutoCmd);
	SetInput(GARDB1_LIGHT_GARDEN, Souliss_T1n_AutoCmd);

	float val = WATERING_DEFAULT_CICLES;
	ImportAnalog(GARDB1_WATERING_TIME_ZONE1, &val);
	ImportAnalog(GARDB1_WATERING_TIME_ZONE2, &val);
	ImportAnalog(GARDB1_WATERING_TIME_ZONE3, &val);
	ImportAnalog(GARDB1_WATERING_TIME_ZONE4, &val);
	ImportAnalog(GARDB1_WATERING_TIME_ZONE5, &val);
	ImportAnalog(GARDB1_WATERING_TIME_ZONE6, &val);
}

inline void ReadInputs()
{
	DigIn(DI5, Souliss_T1n_ToggleCmd, GARDB1_LIGHT_GARDEN);          // Read inputs from DI5

	Souliss_AnalogIn(AV1, memory_map, GARDB1_TOTAL_POWER, 0.1465 * 220 ,0);
}

inline void ProcessLogics()
{
	if( ((mInput(GARDB1_WATERING) == Souliss_T1n_OnCmd) || (mInput(GARDB1_WATERING) == Souliss_T1n_ToggleCmd)) &&
				(mOutput(GARDB1_WATERING) == Souliss_T1n_OffCoil) ) // command on or toggle when ouput is Off
	{
		SetWateringZones();
	}
	if( ((mInput(GARDB1_WATERING) == Souliss_T1n_OffCmd) || (mInput(GARDB1_WATERING) == Souliss_T1n_ToggleCmd)) &&
				(mOutput(GARDB1_WATERING) == Souliss_T1n_OnCoil) ) // command on or toggle when ouput is Off
	{
		UnsetWateringZones();
	}

	Logic_T12(GARDB1_WATERING);
	Logic_T12(GARDB1_WATERING_ZONE1);                                  // Execute the logic for Relay 1
	Logic_T12(GARDB1_WATERING_ZONE2);                                  // Execute the logic for Relay 2
	Logic_T12(GARDB1_WATERING_ZONE3);                                  // Execute the logic for Relay 3
	Logic_T12(GARDB1_WATERING_ZONE4);                                  // Execute the logic for Relay 4
	Logic_T12(GARDB1_WATERING_ZONE5);                                  // Execute the logic for Relay 5
	Logic_T12(GARDB1_WATERING_ZONE6);                                  // relay on board ROW2B4

	Logic_T12(GARDB1_LIGHT_GARDEN);                                  // Execute the logic for Relay 6

	Logic_Power(GARDB1_TOTAL_POWER);

	Logic_T61(GARDB1_WATERING_TIME_ZONE1);
	Logic_T61(GARDB1_WATERING_TIME_ZONE2);
	Logic_T61(GARDB1_WATERING_TIME_ZONE3);
	Logic_T61(GARDB1_WATERING_TIME_ZONE4);
	Logic_T61(GARDB1_WATERING_TIME_ZONE5);
	Logic_T61(GARDB1_WATERING_TIME_ZONE6);
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
	Timer_T12(GARDB1_WATERING);
	Timer_T12(GARDB1_LIGHT_GARDEN);
}

inline void ProcessWateringTimers()
{
	Timer_T12(GARDB1_WATERING_ZONE1);
	Timer_T12(GARDB1_WATERING_ZONE2);
	Timer_T12(GARDB1_WATERING_ZONE3);
	Timer_T12(GARDB1_WATERING_ZONE4);
	Timer_T12(GARDB1_WATERING_ZONE5);
}

inline void SetWateringZone(U8 slot_watering, U8 slot_time, U8& uDelay)
{
		mInput(slot_watering) =  Souliss_T1n_AutoCmd + (U8) mOutput(slot_time);
		mAuxiliary(slot_watering) = uDelay;

		if( mOutput(slot_watering) == Souliss_T1n_AutoState )
			uDelay += (U8) mOutput(slot_time);
}

inline void SetWateringZones()
{
		// set all watering zones
		U8 uDelay = 0;
		SetWateringZone(GARDB1_WATERING_ZONE1, GARDB1_WATERING_TIME_ZONE1, uDelay);
		SetWateringZone(GARDB1_WATERING_ZONE2, GARDB1_WATERING_TIME_ZONE2, uDelay);
		SetWateringZone(GARDB1_WATERING_ZONE3, GARDB1_WATERING_TIME_ZONE3, uDelay);
		SetWateringZone(GARDB1_WATERING_ZONE4, GARDB1_WATERING_TIME_ZONE4, uDelay);
		SetWateringZone(GARDB1_WATERING_ZONE5, GARDB1_WATERING_TIME_ZONE5, uDelay);
		SetWateringZone(GARDB1_WATERING_ZONE6, GARDB1_WATERING_TIME_ZONE6, uDelay);
}

inline void UnsetWateringZones()
{
		// unset all watering zones
		mInput(GARDB1_WATERING_ZONE1) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE1) = 0;

		mInput(GARDB1_WATERING_ZONE2) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE2) = 0;

		mInput(GARDB1_WATERING_ZONE3) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE3) = 0;

		mInput(GARDB1_WATERING_ZONE4) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE4) = 0;

		mInput(GARDB1_WATERING_ZONE5) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE5) = 0;

		mInput(GARDB1_WATERING_ZONE6) = Souliss_T1n_AutoCmd;
		mAuxiliary(GARDB1_WATERING_ZONE6) = 0;
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

			// activate balcony watering in board ROW2B4
			if( mOutput(GARDB1_WATERING_ZONE6) == Souliss_T1n_OnCoil )
				RemoteInput(RS485_ADDRESS_ROW2B4, ROW2B4_WATERING_ZONE6, Souliss_T1n_AutoCmd + 10);
		}

		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		SLOW_PeerJoin();

		SLOW_10s()
		{
			ProcessWateringTimers();
		}

	}
}
