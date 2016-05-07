
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

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------

inline void DefineTypicals()
{
	Set_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Set_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Souliss_SetT22(memory_map, ROW2B1_WINDOW_DINING);

	Set_Temperature(ROW2B1_TEMPERATURE);
	Set_Humidity(ROW2B1_HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_ToggleCmd, memory_map, ROW2B1_LIGHT_BALCONY2, true);
	Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, ROW2B1_LIGHT_BEDROOM3, true);
	DigInWindowToggle(IN3, ROW2B1_WINDOW_DINING);
}

inline void ProcessLogics()
{
	Logic_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Logic_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Souliss_Logic_T22(memory_map, ROW2B1_WINDOW_DINING, &data_changed, SHUTTER_SHORT_TIMEOUT);

	grh_Logic_Humidity(ROW2B1_HUMIDITY);
	grh_Logic_Temperature(ROW2B1_TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, ROW2B1_LIGHT_BALCONY2);
	DigOut(RELAY2, Souliss_T1n_Coil, ROW2B1_LIGHT_BEDROOM3);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  ROW2B1_WINDOW_DINING);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, ROW2B1_WINDOW_DINING);
}

inline void ProcessTimers()
{
	Timer_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Timer_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Timer_Windows(ROW2B1_WINDOW_DINING);

	th = dht.readHumidity();
	ImportAnalog(ROW2B1_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(ROW2B1_TEMPERATURE, &th);
}




void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_ROW2B1);
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B1, 0x0000);

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
