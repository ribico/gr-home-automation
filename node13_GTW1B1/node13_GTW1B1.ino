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


inline void DefineTypicals()
{
	Set_T12(GTW1B1_FAN_SPEED1);
	Set_T12(GTW1B1_FAN_SPEED2);
	Set_T12(GTW1B1_FAN_SPEED3);
	Set_SimpleLight(GTW1B1_LIGHT_GW);

	SetInput(GTW1B1_FAN_SPEED1, Souliss_T1n_AutoCmd);
  SetInput(GTW1B1_FAN_SPEED2, Souliss_T1n_AutoCmd);
  SetInput(GTW1B1_FAN_SPEED3, Souliss_T1n_AutoCmd);
}

inline void ReadInputs()
{
	// input only coming from other boards
}

inline void ProcessLogics()
{
	Logic_SimpleLight(GTW1B1_LIGHT_GW);

	Logic_T12(GTW1B1_FAN_SPEED2);
	Logic_T12(GTW1B1_FAN_SPEED3);

	if( mOutput(GTW1B1_FAN_SPEED2) == Souliss_T1n_AutoOnCoil || mOutput(GTW1B1_FAN_SPEED3) == Souliss_T1n_AutoOnCoil)
		SetInput(GTW1B1_FAN_SPEED1, Souliss_T1n_AutoCmd + 2); // needed always ON because it also drive the water flux valve

	Logic_T12(GTW1B1_FAN_SPEED1);
}

inline void SetOutputs()
{
	nDigOut(RELAY1, Souliss_T1n_OnCoil,  GTW1B1_FAN_SPEED1);
	nDigOut(RELAY2, Souliss_T1n_OnCoil, GTW1B1_FAN_SPEED2);
	nDigOut(RELAY3, Souliss_T1n_OnCoil,  GTW1B1_FAN_SPEED3);
	LowDigOut(RELAY4, Souliss_T1n_OnCoil, GTW1B1_LIGHT_GW);
}

inline void ProcessTimers()
{
	Timer_T12(GTW1B1_FAN_SPEED1);
	Timer_T12(GTW1B1_FAN_SPEED2);
	Timer_T12(GTW1B1_FAN_SPEED3);
	Timer_SimpleLight(GTW1B1_LIGHT_GW);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_GTW1B1, RS485_ADDRESS_ROW2B1);


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
