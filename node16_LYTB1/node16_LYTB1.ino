/*********************
LYTB1 BOARD (ESP-03)
LYT8266 Light bulb

Compiling Options:
  Board as Generic ESP8266 Module
  Flash Mode DIO
  Flash Frequency 40 MHz
  Upload Using OTA
  CPU Frequency 80 MHz
  Flash Size 1M (64K SPIFFS)
  Port souliss-LYTB1 192.168.1.106

***********************/
#define WIFI_UDP_DEBUG

#include "SoulissFramework.h"

// **** Define the WiFi name and password ****
#define WIFICONF_INSKETCH
#define WiFi_SSID               "ribico-loft"
#define WiFi_Password           "12345678"

#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "bconf/LYT8266_LEDBulb.h"          // Load the code directly on the ESP8266
#include "grhSoulissCommon.h"

/*** All configuration includes should be above this line ***/
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"

#define USE_SSL
#include "http_request.h"

#define RED_STARTUP         0x50
#define GREEN_STARTUP       0x10
#define BLUE_STARTUP        0x00

#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[hourly,minutely,daily,alerts,flags]"

WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
 NTPClient timeClient(ntpUDP, "192.168.1.4", 3600, 60000);
=======
#include "forecast_io.h"
>>>>>>> origin/forecast_io_2

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

  grhSetIpAddress(IP_ADDRESS_LYTB1);

  // Define a logic to handle the bulb
  SetLYTLamps(LYTB1_LIGHT);

  UDP_DEBUG_BEGIN;

  for (int i=0; i<WEATHER_DATA_COUNT/2; i++)
  {
    ForecastIO_TimeMachineRequest(i);
    grhSendUDPMessage(forecast_io_response);
    ForecastIO_ParseResponse(CURRENT_WEATHER_IDX+i);
    grhSendUDPMessage(wd[CURRENT_WEATHER_IDX+i].s_icon.c_str());
    ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+i]);
  }
  
  timeClient.begin();

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
    ProcessCommunication();

    SHIFT_91110ms(0)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+0]);
    }
    SHIFT_91110ms(100)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+1]);
    }
    SHIFT_91110ms(200)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+2]);
    }
    SHIFT_91110ms(300)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+3]);
    }
    SHIFT_91110ms(400)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+4]);
    }
    SHIFT_91110ms(500)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+5]);
    }
    SHIFT_91110ms(600)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+6]);
    }
    SHIFT_91110ms(700)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+7]);
    }
    SHIFT_91110ms(800)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+8]);
    }
    SHIFT_91110ms(900)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+9]);
    }
    SHIFT_91110ms(1000)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+10]);
    }
    SHIFT_91110ms(1100)
    {
      ForecastIO_SetColor(LYTB1_LIGHT, &wd[CURRENT_WEATHER_IDX+11]);
    }


/*
    FAST_2110ms()
		{
      timeClient.update();
      grhSendUDPMessage(timeClient.getFormattedTime().c_str());
		}
*/
    FAST_PeerComms();
  }

  EXECUTESLOW() {
    UPDATESLOW();

    // Slowly shut down the lamp
    SLOW_10s() {
      LYTSleepTimer(LYTB1_LIGHT);
    }

    SLOW_1h()
    {
      // move all data of one cell since one hour passed
      for (int i=0; i<WEATHER_DATA_COUNT-1; i++)
      {
        wd[CURRENT_WEATHER_IDX+i].t_time = wd[CURRENT_WEATHER_IDX+i+1].t_time;
        wd[CURRENT_WEATHER_IDX+i].s_icon = wd[CURRENT_WEATHER_IDX+i+1].s_icon;
        wd[CURRENT_WEATHER_IDX+i].f_precipIntensity = wd[CURRENT_WEATHER_IDX+i+1].f_precipIntensity;
        wd[CURRENT_WEATHER_IDX+i].f_precipProbability = wd[CURRENT_WEATHER_IDX+i+1].f_precipProbability;
      }

      // get the new weather forecast
      ForecastIO_TimeMachineRequest(+11);
      ForecastIO_ParseResponse(CURRENT_WEATHER_IDX+11);
    }

    SLOW_PeerJoin();
  }

  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
}
