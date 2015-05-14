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

#define LIGHT_ENTRANCE_1	1			
#define LIGHT_ENTRANCE_2	2
#define LIGHT_BATH_1		3			
#define LIGHT_BATH_2		4

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
	Set_LightsGroup(LIGHT_ENTRANCE_1, LIGHT_ENTRANCE_2);
  	Set_LightsGroup(LIGHT_BATH_1, LIGHT_BATH_2);
	
	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	LightsGroupIn(IN1, LIGHT_ENTRANCE_1, LIGHT_ENTRANCE_2);
	LightsGroupIn(IN2, LIGHT_BATH_1, LIGHT_BATH_2);	
}

inline void ProcessLogics()
{
	Logic_LightsGroup(LIGHT_ENTRANCE_1, LIGHT_ENTRANCE_2);\
	Logic_LightsGroup(LIGHT_BATH_1, LIGHT_BATH_2);

	Logic_Humidity(HUMIDITY);
	Logic_Temperature(TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_ENTRANCE_1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_ENTRANCE_2);
	DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_BATH_1);
	DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_BATH_2);
}

inline void ProcessTimers()
{
	Timer_LightsGroup(LIGHT_ENTRANCE_1, LIGHT_ENTRANCE_2);
	Timer_LightsGroup(LIGHT_BATH_1, LIGHT_BATH_2);
	
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
	grhSetUSARTAddress(RS485_ADDRESS_ROW1B4, RS485_ADDRESS_ROW1B1);


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