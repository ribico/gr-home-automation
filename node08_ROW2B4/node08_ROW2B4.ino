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


//--------------------------------------
// USED FOR DHT SENSOR
#define TEMPERATURE			1
#define TEMPERATURE_1		2
#define HUMIDITY			3
#define HUMIDITY_1			4

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------


inline void DefineTypicals()
{
	
	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{

}

inline void ProcessLogics()
{

	grh_Logic_Humidity(HUMIDITY);
	grh_Logic_Temperature(TEMPERATURE);
}

inline void SetOutputs()
{

}

inline void ProcessTimers()
{
	
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
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B4, RS485_ADDRESS_ROW2B1);


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