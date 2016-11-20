/*********************
BOARD ROW1B2
DINO with RS485 only acting as Peer

***********************/

//#define DEBUG
#include "SoulissFramework.h"

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

#define LED_KITCHEN_SECURITY	6

//--------------------------------------

inline void DefineTypicals()
{
	Set_T12(ROW2B4_WATERING_ZONE6);
	SetInput(ROW2B4_WATERING_ZONE6, Souliss_T1n_AutoCmd);

	Set_T12(ROW2B4_KITCHEN_POWER);
	SetInput(ROW2B4_KITCHEN_POWER, Souliss_T1n_AutoCmd); // Initialize in Auto Mode

	Set_T12(ROW2B4_LIGHT_EXT_DOOR);
	SetInput(ROW2B4_LIGHT_EXT_DOOR, Souliss_T1n_AutoCmd); // Initialize in Auto Mode

	Set_Temperature(ROW2B4_TEMPERATURE);
	Set_Humidity(ROW2B4_HUMIDITY);
	dht.begin();
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_OnCmd, memory_map, ROW2B4_LIGHT_EXT_DOOR, true);
	Souliss_DigInHold(IN4, mInput(ROW2B4_KITCHEN_POWER)+36, Souliss_T1n_AutoCmd, memory_map, ROW2B4_KITCHEN_POWER);
}

inline void ProcessLogics()
{
	Logic_T12(ROW2B4_WATERING_ZONE6);
	Logic_T12(ROW2B4_KITCHEN_POWER);
	Logic_T12(ROW2B4_LIGHT_EXT_DOOR);

	grh_Logic_Humidity(ROW2B4_HUMIDITY);
	grh_Logic_Temperature(ROW2B4_TEMPERATURE);
}

inline void SetOutputs()
{
	nDigOut(RELAY1, Souliss_T1n_Coil, ROW2B4_WATERING_ZONE6);
	nDigOut(RELAY2, Souliss_T1n_Coil, ROW2B4_LIGHT_EXT_DOOR);
	nDigOut(RELAY4, Souliss_T1n_Coil, ROW2B4_KITCHEN_POWER);
}

inline void ProcessTimers()
{
	Timer_T12(ROW2B4_WATERING_ZONE6);
	Timer_T12(ROW2B4_LIGHT_EXT_DOOR);
	// Timer for KITCHEN_POWER managed separately in a slower cycle

	th = dht.readHumidity();
	ImportAnalog(ROW2B4_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(ROW2B4_TEMPERATURE, &th);
}


void setup()
{
  pinMode(LED_KITCHEN_SECURITY, OUTPUT);

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

		if( mOutput(ROW2B4_KITCHEN_POWER) == Souliss_T1n_AutoOnCoil )
		{
			U8 remaining_cycles = mInput(ROW2B4_KITCHEN_POWER)-Souliss_T1n_AutoCmd;
			#ifdef DEBUG
				Serial.print("remaining_cycles : ");
				Serial.println(remaining_cycles);
			#endif

			if( remaining_cycles > 108) // >1.5 hours
			{
				FAST_2110ms()
				{
		    	digitalWrite(LED_KITCHEN_SECURITY, !digitalRead(LED_KITCHEN_SECURITY)); // toggle led
				}
			}
			else if( remaining_cycles > 72) // >1 hours
			{
				FAST_910ms()
				{
		    	digitalWrite(LED_KITCHEN_SECURITY, !digitalRead(LED_KITCHEN_SECURITY)); // toggle led
				}
			}
			else if( remaining_cycles > 36) // >0.5 hours
			{
				FAST_510ms()
				{
		    	digitalWrite(LED_KITCHEN_SECURITY, !digitalRead(LED_KITCHEN_SECURITY)); // toggle led
				}
			}
			else if( remaining_cycles > 12) // >10 min
			{
				FAST_210ms()
				{
		    	digitalWrite(LED_KITCHEN_SECURITY, !digitalRead(LED_KITCHEN_SECURITY)); // toggle led
				}
			}
			else if( remaining_cycles > 0) // at least one cycle
			{
				FAST_110ms()
				{
		    	digitalWrite(LED_KITCHEN_SECURITY, !digitalRead(LED_KITCHEN_SECURITY)); // toggle led
				}
			}
		}
		else if( mOutput(ROW2B4_KITCHEN_POWER) == Souliss_T1n_OnCoil )
		{
			digitalWrite(LED_KITCHEN_SECURITY, true);
		}
		else
		{
			digitalWrite(LED_KITCHEN_SECURITY, false);
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

		SLOW_50s()
		{
			Timer_T12(ROW2B4_KITCHEN_POWER);
		}
	}
}
