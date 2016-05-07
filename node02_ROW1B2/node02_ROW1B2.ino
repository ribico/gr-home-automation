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

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------


inline void DefineTypicals()
{
	Set_LightsGroup(ROW1B2_LIGHT_KITCHEN_1, ROW1B2_LIGHT_KITCHEN_3);

	Set_Temperature(ROW1B2_TEMPERATURE);
	Set_Humidity(ROW1B2_HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	LightsGroupIn(IN1, ROW1B2_LIGHT_KITCHEN_1, ROW1B2_LIGHT_KITCHEN_3);
}

inline void ProcessLogics()
{
	Logic_LightsGroup(ROW1B2_LIGHT_KITCHEN_1, ROW1B2_LIGHT_KITCHEN_3);
	grh_Logic_Humidity(ROW1B2_HUMIDITY);
	grh_Logic_Temperature(ROW1B2_TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, ROW1B2_LIGHT_KITCHEN_1);
	DigOut(RELAY2, Souliss_T1n_Coil, ROW1B2_LIGHT_KITCHEN_2);
	DigOut(RELAY3, Souliss_T1n_Coil, ROW1B2_LIGHT_KITCHEN_3);
}

inline void ProcessTimers()
{
	Timer_LightsGroup(ROW1B2_LIGHT_KITCHEN_1, ROW1B2_LIGHT_KITCHEN_3);

	th = dht.readHumidity();
	ImportAnalog(ROW1B2_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(ROW1B2_TEMPERATURE, &th);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_ROW1B2, RS485_ADDRESS_ROW1B1);

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
