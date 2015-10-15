
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG

#include "bconf/DINo_v2_EthernetBridge_RS485.h"
#include "conf/SuperNode.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_DINo_v2.h"



#define LIGHT_LIVING_1			0
#define LIGHT_LIVING_2			1
#define LIGHT_LIVING_3			2
#define LIGHT_LIVING_4			3
#define LIGHT_LIVING_5			4
#define LIGHT_LIVING_6			5

// not connected directly to this board
#define EXT_TEMP						6
#define EXT_TEMP_H					7
#define EXT_UR							8
#define EXT_UR_H						9

#define LOFT_TEMP						10
#define LOFT_TEMP_H					11
#define LOFT_UR							12
#define LOFT_UR_H						13

#define ROW1B3_LIGHT_STAIRS			4
#define ROW1B4_LIGHT_ENTRANCE_1	1

inline void DefineTypicals()
{
	Set_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
	Set_Temperature(EXT_TEMP);
	Set_Humidity(EXT_UR);
	Set_Temperature(LOFT_TEMP);
	Set_Humidity(LOFT_UR);
}

inline void ReadInputs()
{
	if( LightsGroupIn(IN1, LIGHT_LIVING_1, LIGHT_LIVING_6) == MaCaco_DATACHANGED)
	{
		Send(RS485_ADDRESS_ROW1B3, ROW1B3_LIGHT_STAIRS, mInput(LIGHT_LIVING_5)); // -> LIGHT_STAIRS
		delay(50);
		Send(RS485_ADDRESS_ROW1B4, ROW1B4_LIGHT_ENTRANCE_1, mInput(LIGHT_LIVING_6)); // -> LIGHT_ENTRANCE_1
	}
}

inline void ProcessLogics()
{
	Logic_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
	grh_Logic_Temperature(EXT_TEMP);
	grh_Logic_Humidity(EXT_UR);
	grh_Logic_Temperature(LOFT_TEMP);
	grh_Logic_Humidity(LOFT_UR);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_LIVING_1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_LIVING_2);
	DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_LIVING_3);
	DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING_4);
	// LIGHT_LIVING_5 is remote on RS485_ADDRESS_ROW1B3 -> LIGHT_STAIRS
	// LIGHT_LIVING_6 is remote on RS485_ADDRESS_ROW1B4 -> LIGHT_ENTRANCE_1
}

inline void ProcessTimers()
{
	Timer_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_6);
}




void setup()
{
	grhOpenSerialOnDebug();
	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_ROW1B1);
	grhSetUSARTAddress(RS485_ADDRESS_ROW1B1, 0x0000);

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
