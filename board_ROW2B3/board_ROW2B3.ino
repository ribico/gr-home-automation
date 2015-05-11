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
#include "HW_Setup_DINo_v2.h"

#define WINDOW_BED3		1			
#define WINDOW_BATH2	2	

//--------------------------------------
// USED FOR DHT SENSOR
#define TEMPERATURE			3
#define TEMPERATURE_1		4
#define HUMIDITY			5
#define HUMIDITY_1			6

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------


inline void DefineTypicals()
{
	Souliss_SetT22(memory_map, WINDOW_BED3);
	Souliss_SetT22(memory_map, WINDOW_BATH2);

	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	DigInWindowToggle(IN1, WINDOW_BED3);
	DigInWindowToggle(IN2, WINDOW_BATH2);
}

inline void ProcessLogics()
{
	Souliss_Logic_T22(memory_map, WINDOW_BED3, &data_changed, SHUTTER_SHORT_TIMEOUT);
	Souliss_Logic_T22(memory_map, WINDOW_BATH2, &data_changed, SHUTTER_SHORT_TIMEOUT);

	Logic_Humidity(HUMIDITY);
	Logic_Temperature(TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T2n_Coil_Open,  WINDOW_BED3);
	DigOut(RELAY2, Souliss_T2n_Coil_Close, WINDOW_BED3);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  WINDOW_BATH2);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, WINDOW_BATH2);
}

inline void ProcessTimers()
{
	Timer_Windows(WINDOW_BED3);
	Timer_Windows(WINDOW_BATH2);

	th = dht.readHumidity();
	ImportAnalog(HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(TEMPERATURE, &th);	
}


void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	PowerDownEthernet(); // pure RS485 node
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B3, RS485_ADDRESS_ROW2B1);


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