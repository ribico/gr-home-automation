
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG

#include "bconf/DINo_v2_EthernetBridge_RS485.h"
#include "conf/SuperNode.h" 

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_DINo_v2.h"




#define LIGHT_BALCONY1		1			
#define LIGHT_BEDROOM1		2
#define WINDOW_BEDROOM1		3

//--------------------------------------
// USED FOR DHT SENSOR
#define TEMPERATURE			4
#define TEMPERATURE_1		5
#define HUMIDITY			6
#define HUMIDITY_1			7

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------

inline void DefineTypicals()
{
	Set_SimpleLight(LIGHT_BALCONY1);
	Set_SimpleLight(LIGHT_BEDROOM1);
	Souliss_SetT22(memory_map, WINDOW_BEDROOM1);

	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();	
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_ToggleCmd, memory_map, LIGHT_BALCONY1, true);\
	Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, LIGHT_BEDROOM1, true);\
	DigInWindowToggle(IN3, WINDOW_BEDROOM1);
}

inline void ProcessLogics()
{
	Logic_SimpleLight(LIGHT_BALCONY1);
	Logic_SimpleLight(LIGHT_BEDROOM1);
	Souliss_Logic_T22(memory_map, WINDOW_BEDROOM1, &data_changed, SHUTTER_LONG_TIMEOUT);

	Logic_Humidity(HUMIDITY);
	Logic_Temperature(TEMPERATURE);	
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_BALCONY1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_BEDROOM1);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  WINDOW_BEDROOM1);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, WINDOW_BEDROOM1);
}

inline void ProcessTimers()
{
	Timer_SimpleLight(LIGHT_BALCONY1);
	Timer_SimpleLight(LIGHT_BEDROOM1);
	Timer_Windows(WINDOW_BEDROOM1);

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

	grhSetIpAddress(IP_ADDRESS_BED1B1);
//	grhSetUSARTAddress(RS485_ADDRESS_BED1B1, 0x0000);

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