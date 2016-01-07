
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
#include "grhSoulissSlots.h"
#include "HW_Setup_DINo_v2.h"


#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------

inline void DefineTypicals()
{
	Set_LightsGroup(BED2B1_LIGHT_BATH1_1, BED2B1_LIGHT_BATH1_3);
	Set_SimpleLight(BED2B1_LIGHT_LIVING);

	Set_Temperature(BED2B1_TEMPERATURE);
	Set_Humidity(BED2B1_HUMIDITY);
	dht.begin();

	Set_SimpleLight(BED2B1_REMOTE_LIGHT_GW1);
}

inline void ReadInputs()
{
	LightsGroupIn(IN1, BED2B1_LIGHT_BATH1_1, BED2B1_LIGHT_BATH1_3);
	Souliss_DigIn(IN3, Souliss_T1n_ToggleCmd, memory_map, BED2B1_LIGHT_LIVING, true);

	U8 ret = Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, BED2B1_REMOTE_LIGHT_GW1, true);
	if( ret != MaCaco_NODATACHANGED )
		RemoteInput(RS485_ADDRESS_GTW1B1, 4, ret);
}

inline void ProcessLogics()
{
	Logic_LightsGroup(BED2B1_LIGHT_BATH1_1, BED2B1_LIGHT_BATH1_3);
	Logic_SimpleLight(BED2B1_LIGHT_LIVING);

	grh_Logic_Humidity(BED2B1_HUMIDITY);
	grh_Logic_Temperature(BED2B1_TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, BED2B1_LIGHT_BATH1_1);
	DigOut(RELAY2, Souliss_T1n_Coil, BED2B1_LIGHT_BATH1_2);
	DigOut(RELAY3, Souliss_T1n_Coil, BED2B1_LIGHT_BATH1_3);
	DigOut(RELAY4, Souliss_T1n_Coil, BED2B1_LIGHT_LIVING);
}

inline void ProcessTimers()
{
	Timer_LightsGroup(BED2B1_LIGHT_BATH1_1, BED2B1_LIGHT_BATH1_3);
	Timer_SimpleLight(BED2B1_LIGHT_LIVING);

	th = dht.readHumidity();
	ImportAnalog(BED2B1_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(BED2B1_TEMPERATURE, &th);
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
