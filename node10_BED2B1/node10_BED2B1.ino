
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




#define LIGHT_BATH1_1		1		
#define LIGHT_BATH1_2		2
#define LIGHT_BATH1_3		3
#define LIGHT_LIVING		4

//--------------------------------------
// USED FOR DHT SENSOR
#define TEMPERATURE			5
#define TEMPERATURE_1		6
#define HUMIDITY			7
#define HUMIDITY_1			8

#define REMOTE_LIGHT_GW1	9

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------

inline void DefineTypicals()
{
	Set_LightsGroup(LIGHT_BATH1_1, LIGHT_BATH1_3);
	Set_SimpleLight(LIGHT_LIVING);

	Set_Temperature(TEMPERATURE);
	Set_Humidity(HUMIDITY);
	dht.begin();	

	Set_SimpleLight(REMOTE_LIGHT_GW1);
}

inline void ReadInputs()
{
	LightsGroupIn(IN1, LIGHT_BATH1_1, LIGHT_BATH1_3);
	Souliss_DigIn(IN3, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LIVING, true);

	U8 ret = Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, REMOTE_LIGHT_GW1, true);
	if( ret != MaCaco_NODATACHANGED )
		RemoteInput(RS485_ADDRESS_GTW1B1, 4, ret);
}

inline void ProcessLogics()
{
	Logic_LightsGroup(LIGHT_BATH1_1, LIGHT_BATH1_3);
	Logic_SimpleLight(LIGHT_LIVING);

	grh_Logic_Humidity(HUMIDITY);
	grh_Logic_Temperature(TEMPERATURE);	
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_BATH1_1);
	DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_BATH1_2);
	DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_BATH1_3);
	DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING);
}

inline void ProcessTimers()
{
	Timer_LightsGroup(LIGHT_BATH1_1, LIGHT_BATH1_3);
	Timer_SimpleLight(LIGHT_LIVING);

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

	grhSetIpAddress(IP_ADDRESS_BED2B1);
	grhSetUSARTAddress(RS485_ADDRESS_BED2B1, 0x0000);

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