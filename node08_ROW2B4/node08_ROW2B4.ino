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
	Set_T12(ROW2B4_WATERING_ZONE6);
	Set_T12(ROW2B4_KITCHEN_POWER);
	SetInput(ROW2B4_KITCHEN_POWER, Souliss_T1n_AutoCmd); // Initialize in Auto Mode

	Set_Temperature(ROW2B4_TEMPERATURE);
	Set_Humidity(ROW2B4_HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
//	Souliss_DigIn(IN1, Souliss_T1n_AutoCmd+2, memory_map, ROW2B4_WATERING_ZONE6, true);
	Souliss_DigIn(IN4, mInput(ROW2B4_KITCHEN_POWER)+36, memory_map, ROW2B4_KITCHEN_POWER, true);

}

inline void ProcessLogics()
{
	Logic_T12(ROW2B4_WATERING_ZONE6);
	Logic_T12(ROW2B4_KITCHEN_POWER);

	grh_Logic_Humidity(ROW2B4_HUMIDITY);
	grh_Logic_Temperature(ROW2B4_TEMPERATURE);
}

inline void SetOutputs()
{
	nDigOut(RELAY1, Souliss_T1n_Coil, ROW2B4_WATERING_ZONE6);
	nDigOut(RELAY4, Souliss_T1n_Coil, ROW2B4_KITCHEN_POWER);
}

inline void ProcessTimers()
{
	Timer_T12(ROW2B4_WATERING_ZONE6);
	// Timer for KITCHEN_POWER managed separately in a slower cycle

	th = dht.readHumidity();
	ImportAnalog(ROW2B4_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(ROW2B4_TEMPERATURE, &th);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B4, RS485_ADDRESS_ROW2B1);


	DefineTypicals();

	SetInput(ROW2B4_WATERING_ZONE6, Souliss_T1n_AutoCmd);
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

		SLOW_50s()
		{
			Timer_T12(ROW2B4_KITCHEN_POWER);
		}
	}
}
