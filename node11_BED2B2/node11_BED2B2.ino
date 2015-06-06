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

#define LIGHT_BEDROOM2_1			1
#define LIGHT_BEDROOM2_2			2
#define LIGHT_BEDROOM2_3			3		
#define LIGHT_BEDROOM2_4			4

//--------------------------------------
// USED FOR DHT SENSOR
#define TEMPERATURE			5
#define TEMPERATURE_1		6
#define HUMIDITY			7
#define HUMIDITY_1			8

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------


inline void DefineTypicals()
{
	Set_LightsGroup(LIGHT_BEDROOM2_1, LIGHT_BEDROOM2_4);

	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	LightsGroupIn(IN1, LIGHT_BEDROOM2_1, LIGHT_BEDROOM2_4);
	Souliss_DigIn(IN3, Souliss_T1n_ToggleCmd, memory_map, LIGHT_BEDROOM2_2, true);
	Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, LIGHT_BEDROOM2_3, true);
}

inline void ProcessLogics()
{
	Logic_LightsGroup(LIGHT_BEDROOM2_1, LIGHT_BEDROOM2_4);

	grh_Logic_Humidity(HUMIDITY);
	grh_Logic_Temperature(TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_BEDROOM2_1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_BEDROOM2_2);
	DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_BEDROOM2_3);
	DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_BEDROOM2_4);
}

inline void ProcessTimers()
{
	Timer_LightsGroup(LIGHT_BEDROOM2_1, LIGHT_BEDROOM2_4);
	
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
	grhSetUSARTAddress(RS485_ADDRESS_BED2B2, RS485_ADDRESS_BED2B1);

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