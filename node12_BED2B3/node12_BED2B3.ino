/*********************
BOARD ROW1B2
DINO with RS485 only acting as Peer

***********************/

//#define DEBUG


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
	Souliss_SetT22(memory_map, BED2B3_WINDOW_BATH1);
	Souliss_SetT22(memory_map, BED2B3_WINDOW_BEDROOM2);

	Set_Temperature(BED2B3_TEMPERATURE);
	Set_Humidity(BED2B3_HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	DigInWindowToggle(IN1, BED2B3_WINDOW_BATH1);
	DigInWindowToggle(IN2, BED2B3_WINDOW_BEDROOM2);
}

inline void ProcessLogics()
{
	Souliss_Logic_T22(memory_map, BED2B3_WINDOW_BATH1, &data_changed, SHUTTER_SHORT_TIMEOUT);
	Souliss_Logic_T22(memory_map, BED2B3_WINDOW_BEDROOM2, &data_changed, SHUTTER_SHORT_TIMEOUT);

	grh_Logic_Humidity(BED2B3_HUMIDITY);
	grh_Logic_Temperature(BED2B3_TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T2n_Coil_Open,  BED2B3_WINDOW_BATH1);
	DigOut(RELAY2, Souliss_T2n_Coil_Close, BED2B3_WINDOW_BATH1);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  BED2B3_WINDOW_BEDROOM2);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, BED2B3_WINDOW_BEDROOM2);
}

inline void ProcessTimers()
{
	Timer_Windows(BED2B3_WINDOW_BATH1);
	Timer_Windows(BED2B3_WINDOW_BEDROOM2);

	th = dht.readHumidity();
	ImportAnalog(BED2B3_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(BED2B3_TEMPERATURE, &th);
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_BED2B3, RS485_ADDRESS_BED2B1);


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
