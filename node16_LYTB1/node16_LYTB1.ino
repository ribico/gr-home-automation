/*********************
LYTB1 BOARD (ESP-03)
LYT8266 Light bulb

Compiling Options:
  Board as Generic ESP8266 Module
  Flash Mode QIO
  Flash Frequency 40 MHz
  Upload Using Serial
  CPU Frequency 80 MHz
  Flash Size 1M (64K SPIFFS)
  Upload Speed 115200

***********************/
#include "SoulissFramework.h"

// Configure the framework
#include "bconf/LYT8266_LEDBulb.h"          // Load the code directly on the ESP8266

#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "grhSoulissCommon.h"

#include "Souliss.h"
#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"

#define TurnOnLED() // need to be implemented
#define ToggleLED() // need to be implemented

#define RED_STARTUP         0x50
#define GREEN_STARTUP       0x10
#define BLUE_STARTUP        0x00

// Define the WiFi name and password
#define WIFICONF_INSKETCH
#define WiFi_SSID               "ribico-loft"
#define WiFi_Password           "12345678"


inline void DefineTypicals()
{
  SetLYTLamps(LYTB1_LIGHT);
}

inline void ReadInputs()
{
}

inline void ProcessLogics()
{
}

inline void SetOutputs()
{
}

inline void ProcessTimers()
{
  LYTSleepTimer(LYTB1_LIGHT);
}

void setup()
{
  // Init the network stack and the bulb, turn on with a warm amber
  Initialize();
  InitLYT();

  /****
      Generally set a PWM output before the connection will lead the
      ESP8266 to reboot for a conflict on the FLASH write access.

      Here we do the configuration during the WebConfig and so we don't
      need to write anything in the FLASH, and the module can connect
      to the last used network.

      If you don't use the WebConfig use a dummy sketch that connects to
      your WiFi and then use this sketch
  ****/
  SetColor(LYTB1_LIGHT, RED_STARTUP, GREEN_STARTUP, BLUE_STARTUP);


  grhOpenSerialOnDebug();
  grhSetIpAddress(IP_ADDRESS_LYTB1);

  DefineTypicals();

  // Init the OTA
  ArduinoOTA.setHostname("souliss-LYTB1");
  ArduinoOTA.begin();
}

void loop()
{
	EXECUTEFAST() {
		UPDATEFAST();

    // Is an unusual approach, but to get fast response to color change we run the LYT logic and
    // basic communication processing at maximum speed.
    LogicLYTLamps(LYTB1_LIGHT);

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

  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
}
