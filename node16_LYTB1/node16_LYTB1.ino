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

#include "forecast_io.h"
#include "node16_LYTB1_scheduler.h"

#define RED_STARTUP         0x50
#define GREEN_STARTUP       0x10
#define BLUE_STARTUP        0x00

#define EEPROM_ADDR_AQUARIUM_LIGHT_DEFAULT_DURATION 100
#define EEPROM_ADDR_WATERING_ZONE1_DEFAULT_DURATION 104
#define EEPROM_ADDR_WATERING_ZONE2_DEFAULT_DURATION 108
#define EEPROM_ADDR_WATERING_ZONE3_DEFAULT_DURATION 112
#define EEPROM_ADDR_WATERING_ZONE4_DEFAULT_DURATION 116
#define EEPROM_ADDR_WATERING_ZONE5_DEFAULT_DURATION 120
#define EEPROM_ADDR_WATERING_ZONE6_DEFAULT_DURATION 124
#define EEPROM_ADDR_SANITARY_WATER_PRODUCTION_DEFAULT_DURATION 128

inline void FlashForecastIOColor()
{
    ForecastIO_CurrentWeatherRequest();
//  grhSendUDPMessage(forecast_io_response);
    String sIcon = ForecastIO_ParseDayIcon(forecast_io_response);
    grhSendUDPMessage(sIcon.c_str());
    ForecastIO_SetColor(LYTB1_RGB_LIGHT, sIcon);
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
  SetColor(LYTB1_RGB_LIGHT, RED_STARTUP, GREEN_STARTUP, BLUE_STARTUP);

  grhSetIpAddress(IP_ADDRESS_LYTB1);

  // Define a logic to handle the bulb
  SetLYTLamps(LYTB1_RGB_LIGHT);

  EEPROM.begin(512); //initialize the EEPROM

  float def_value;

  Set_Analog_Setpoint(LYTB1_AQUARIUM_LIGHT_DURATION);
  EEPROM.get(EEPROM_ADDR_AQUARIUM_LIGHT_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = AQUARIUM_LIGHT_DEFAULT_DURATION;
	ImportAnalog(LYTB1_AQUARIUM_LIGHT_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE1_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE1_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE1_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE1_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE2_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE2_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE2_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE2_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE3_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE3_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE3_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE3_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE4_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE4_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE4_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE4_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE5_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE5_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE5_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE5_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_WATERING_ZONE6_DURATION);
  EEPROM.get(EEPROM_ADDR_WATERING_ZONE6_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = WATERING_ZONE6_DEFAULT_DURATION;
	ImportAnalog(LYTB1_WATERING_ZONE6_DURATION, &def_value);

  Set_Analog_Setpoint(LYTB1_SANITARY_WATER_PRODUCTION_DURATION);
  EEPROM.get(EEPROM_ADDR_SANITARY_WATER_PRODUCTION_DEFAULT_DURATION, def_value);
  if( isnan(def_value) )
    def_value = SANITARY_WATER_PRODUCTION_DEFAULT_DURATION;
	ImportAnalog(LYTB1_SANITARY_WATER_PRODUCTION_DURATION, &def_value);

  // copy initial value to mOutput to avoid calculation with NaN
  Logic_AnalogIn(LYTB1_AQUARIUM_LIGHT_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE1_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE2_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE3_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE4_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE5_DURATION);
  Logic_AnalogIn(LYTB1_WATERING_ZONE6_DURATION);
  Logic_AnalogIn(LYTB1_SANITARY_WATER_PRODUCTION_DURATION);


  UDP_DEBUG_BEGIN;
  SCHEDULER_BEGIN;

  FlashForecastIOColor();

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
    LogicLYTLamps(LYTB1_RGB_LIGHT);

    FAST_110ms()
    {
      if( Logic_AnalogIn(LYTB1_AQUARIUM_LIGHT_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_AQUARIUM_LIGHT_DEFAULT_DURATION, mOutputAsFloat(LYTB1_AQUARIUM_LIGHT_DURATION) );
        EEPROM.commit();
      }

      if( Logic_AnalogIn(LYTB1_WATERING_ZONE1_DURATION) )
      {      
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE1_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE1_DURATION) );
        EEPROM.commit();
      }      

      if( Logic_AnalogIn(LYTB1_WATERING_ZONE2_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE2_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE2_DURATION) );
        EEPROM.commit();
      }
      
      if( Logic_AnalogIn(LYTB1_WATERING_ZONE3_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE3_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE3_DURATION) );
        EEPROM.commit();
      }

      if( Logic_AnalogIn(LYTB1_WATERING_ZONE4_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE4_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE4_DURATION) );
        EEPROM.commit();
      }

      if( Logic_AnalogIn(LYTB1_WATERING_ZONE5_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE5_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE5_DURATION) );
        EEPROM.commit();
      }

      if( Logic_AnalogIn(LYTB1_WATERING_ZONE6_DURATION) )
      {      
        EEPROM.put( EEPROM_ADDR_WATERING_ZONE6_DEFAULT_DURATION, mOutputAsFloat(LYTB1_WATERING_ZONE6_DURATION) );
        EEPROM.commit();
      }

      if( Logic_AnalogIn(LYTB1_SANITARY_WATER_PRODUCTION_DURATION) )
      {
        EEPROM.put( EEPROM_ADDR_SANITARY_WATER_PRODUCTION_DEFAULT_DURATION, mOutputAsFloat(LYTB1_SANITARY_WATER_PRODUCTION_DURATION) );
        EEPROM.commit();
      }
    }

    SHIFT_11110ms(50)
    {
      SCHEDULER_TIME_UPDATE;
      grhSendUDPMessage(timeClient.getFormattedTime().c_str());

      AquariumLightScheduler();
    }
    SHIFT_11110ms(100)
    {
      WateringZone1Scheduler();
      WateringZone2Scheduler();
      WateringZone3Scheduler();
      WateringZone4Scheduler();
      WateringZone5Scheduler();
      WateringZone6Scheduler();
    }
    SHIFT_11110ms(150)
    {
      SanitaryWaterScheduler();
    }

    ProcessCommunication();

    FAST_PeerComms();
  }

  EXECUTESLOW() {
    UPDATESLOW();

    // Slowly shut down the lamp
    SLOW_10s()
    {
      // Slowly shut down the lamp
      LYTSleepTimer(LYTB1_RGB_LIGHT);
    }

    SLOW_1h()
    {
      // get the new weather forecast
      FlashForecastIOColor();
    }

    SLOW_PeerJoin();
  }

  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
}
