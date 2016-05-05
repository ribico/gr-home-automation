
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG

#include "SoulissFramework.h"

#include "bconf/DINo_v2_EthernetBridge_RS485.h"
#include "conf/SuperNode.h"

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
	Set_LightsGroup(ROW1B1_LIGHT_LIVING_1, ROW1B1_LIGHT_LIVING_6);

	Set_Temperature(ROW1B1_EXT_TEMP);
	Set_Humidity(ROW1B1_EXT_UR);

	Set_Temperature(ROW1B1_LOFT_TEMP);
	Set_Humidity(ROW1B1_LOFT_UR);

	Set_Temperature(ROW1B1_HVAC_BOILER_SANITARY_TEMP);
	Set_Temperature(ROW1B1_HVAC_BOILER_HEATING_TEMP);
	Set_Temperature(ROW1B1_HVAC_BOILER_BOTTOM_TEMP);

	Set_Temperature(ROW1B1_HVAC_HEATPUMP_FLOW_TEMP);
	Set_Temperature(ROW1B1_HVAC_HEATPUMP_RETURN_TEMP);

	Set_Temperature(ROW1B1_HVAC_FANCOILS_FLOW_TEMP);
	Set_Temperature(ROW1B1_HVAC_FANCOILS_RETURN_TEMP);
	Set_Temperature(ROW1B1_HVAC_FLOOR_FLOW_TEMP);
	Set_Temperature(ROW1B1_HVAC_FLOOR_RETURN_TEMP);

}

inline void ReadInputs()
{
	if( LightsGroupIn(IN1, ROW1B1_LIGHT_LIVING_1, ROW1B1_LIGHT_LIVING_6) == MaCaco_DATACHANGED)
	{
		Send(RS485_ADDRESS_ROW1B3, ROW1B3_LIGHT_STAIRS, mInput(ROW1B1_LIGHT_LIVING_5)); // -> LIGHT_STAIRS
		delay(50);
		Send(RS485_ADDRESS_ROW1B4, ROW1B4_LIGHT_ENTRANCE_1, mInput(ROW1B1_LIGHT_LIVING_6)); // -> LIGHT_ENTRANCE_1
	}
}

inline void ProcessLogics()
{
	Logic_LightsGroup(ROW1B1_LIGHT_LIVING_1, ROW1B1_LIGHT_LIVING_6);

	grh_Logic_Temperature(ROW1B1_EXT_TEMP);
	grh_Logic_Humidity(ROW1B1_EXT_UR);

	grh_Logic_Temperature(ROW1B1_LOFT_TEMP);
	grh_Logic_Humidity(ROW1B1_LOFT_UR);

	grh_Logic_Temperature(ROW1B1_HVAC_BOILER_SANITARY_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_BOILER_HEATING_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_BOILER_BOTTOM_TEMP);

	grh_Logic_Temperature(ROW1B1_HVAC_HEATPUMP_FLOW_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_HEATPUMP_RETURN_TEMP);

	grh_Logic_Temperature(ROW1B1_HVAC_FANCOILS_FLOW_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_FANCOILS_RETURN_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_FLOOR_FLOW_TEMP);
	grh_Logic_Temperature(ROW1B1_HVAC_FLOOR_RETURN_TEMP);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, ROW1B1_LIGHT_LIVING_1);
	DigOut(RELAY2, Souliss_T1n_Coil, ROW1B1_LIGHT_LIVING_2);
	DigOut(RELAY3, Souliss_T1n_Coil, ROW1B1_LIGHT_LIVING_3);
	DigOut(RELAY4, Souliss_T1n_Coil, ROW1B1_LIGHT_LIVING_4);
	// LIGHT_LIVING_5 is remote on RS485_ADDRESS_ROW1B3 -> LIGHT_STAIRS
	// LIGHT_LIVING_6 is remote on RS485_ADDRESS_ROW1B4 -> LIGHT_ENTRANCE_1
}

inline void ProcessTimers()
{
	Timer_LightsGroup(ROW1B1_LIGHT_LIVING_1, ROW1B1_LIGHT_LIVING_6);
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
