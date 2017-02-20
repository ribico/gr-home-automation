
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG
#include "SoulissFramework.h"

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
	Set_SimpleLight(BED1B1_LIGHT_BALCONY1);
	Set_SimpleLight(BED1B1_LIGHT_BEDROOM1);
	Souliss_SetT22(memory_map, BED1B1_WINDOW_BEDROOM1);

	Set_Temperature(BED1B1_TEMPERATURE);
	Set_Humidity(BED1B1_HUMIDITY);
	dht.begin();

	Set_SimpleLight(BED1B1_REMOTE_LIGHT_GW1);
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_ToggleCmd, memory_map, BED1B1_LIGHT_BALCONY1, true);
	Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, BED1B1_LIGHT_BEDROOM1, true);
	DigInWindowToggle(IN3, BED1B1_WINDOW_BEDROOM1);

	U8 ret = Souliss_DigIn(IN4, Souliss_T1n_ToggleCmd, memory_map, BED1B1_REMOTE_LIGHT_GW1, true);
	if( ret != MaCaco_NODATACHANGED )
		RemoteInput(RS485_ADDRESS_GTW1B1, 4, ret);
}

inline void ProcessLogics()
{
	Logic_SimpleLight(BED1B1_LIGHT_BALCONY1);
	Logic_SimpleLight(BED1B1_LIGHT_BEDROOM1);
	Souliss_Logic_T22(memory_map, BED1B1_WINDOW_BEDROOM1, &data_changed, SHUTTER_LONG_TIMEOUT);

	grh_Logic_Humidity(BED1B1_HUMIDITY);
	grh_Logic_Temperature(BED1B1_TEMPERATURE);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, BED1B1_LIGHT_BALCONY1);
	DigOut(RELAY2, Souliss_T1n_Coil, BED1B1_LIGHT_BEDROOM1);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  BED1B1_WINDOW_BEDROOM1);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, BED1B1_WINDOW_BEDROOM1);
}

inline void ProcessTimers()
{
	Timer_SimpleLight(BED1B1_LIGHT_BALCONY1);
	Timer_SimpleLight(BED1B1_LIGHT_BEDROOM1);
	Timer_Windows(BED1B1_WINDOW_BEDROOM1);

	th = grh_W_Average(mOutputAsFloat(BED1B1_HUMIDITY), dht.readHumidity());
	ImportAnalog(BED1B1_HUMIDITY, &th);

	th = grh_W_Average(mOutputAsFloat(BED1B1_TEMPERATURE), dht.readTemperature());
	ImportAnalog(BED1B1_TEMPERATURE, &th);
}




void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_BED1B1);
	grhSetUSARTAddress(RS485_ADDRESS_BED1B1, 0x0000);

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
		grh_SLOW_PeerJoin();
	}
}
