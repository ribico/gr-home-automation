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

#define WINDOW_LIVING		1			
#define WINDOW_KITCHEN		2	

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
	Souliss_SetT22(memory_map, WINDOW_LIVING);
	Souliss_SetT22(memory_map, WINDOW_KITCHEN);

	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	DigInWindowToggle(IN1, WINDOW_LIVING);
	DigInWindowToggle(IN2, WINDOW_KITCHEN);
}

inline void ProcessLogics()
{
	Souliss_Logic_T22(memory_map, WINDOW_LIVING, &data_changed, SHUTTER_SHORT_TIMEOUT);
	Souliss_Logic_T22(memory_map, WINDOW_KITCHEN, &data_changed, SHUTTER_LONG_TIMEOUT);
	
	grh_Logic_Humidity(HUMIDITY);
	grh_Logic_Temperature(TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T2n_Coil_Open,  WINDOW_LIVING);
	DigOut(RELAY2, Souliss_T2n_Coil_Close, WINDOW_LIVING);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  WINDOW_KITCHEN);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, WINDOW_KITCHEN);
}

inline void ProcessTimers()
{
	Timer_Windows(WINDOW_LIVING);
	Timer_Windows(WINDOW_KITCHEN);
	
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
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B2, RS485_ADDRESS_ROW2B1);

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