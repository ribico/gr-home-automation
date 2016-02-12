/*********************
CLOUD_GW BOARD
Olimex_ESP8266EVB acting as GATEWAY for cloud streaming

***********************/

// Configure the framework
#include "bconf/Olimex_ESP8266EVB.h"              // Load the code directly on the ESP8266
#include "conf/Gateway_wLastin.h"
#include "conf/DynamicAddressing.h"

// Define the WiFi name and password
#define WIFICONF_INSKETCH
#define WiFi_SSID               "ribico-loft"
#define WiFi_Password           "12345678"

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <EEPROM.h>

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "ribico"
#define AIO_KEY         "e5d8a6ddc6e1898d8c7ad425e7e858697928a1c3"

//#define PUBSUB_H
/*** All configuration includes should be above this line ***/
#include "grhSoulissCommon.h"
#include "Souliss.h"
#include "grhSoulissNetwork.h"
#include "grhSoulissSlots.h"

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

const char HVAC_Boiler_Sanitary_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/HVAC_Boiler_Sanitary_Temp";
Adafruit_MQTT_Publish HVAC_Boiler_Sanitary_Temp = Adafruit_MQTT_Publish(&mqtt, HVAC_Boiler_Sanitary_Temp_FEED);
float HVAC_Boiler_Sanitary_Temp_last = 0.0;

const char HVAC_Floor_Flow_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/HVAC_Floor_Flow_Temp";
Adafruit_MQTT_Publish HVAC_Floor_Flow_Temp = Adafruit_MQTT_Publish(&mqtt, HVAC_Floor_Flow_Temp_FEED);
float HVAC_Floor_Flow_Temp_last = 0.0;

const char Bed1_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Bed1_Temp";
Adafruit_MQTT_Publish Bed1_Temp = Adafruit_MQTT_Publish(&mqtt, Bed1_Temp_FEED);

const char Bath1_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Bath1_Temp";
Adafruit_MQTT_Publish Bath1_Temp = Adafruit_MQTT_Publish(&mqtt, Bath1_Temp_FEED);

const char Bed2_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Bed2_Temp";
Adafruit_MQTT_Publish Bed2_Temp = Adafruit_MQTT_Publish(&mqtt, Bed2_Temp_FEED);

const char Living_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Living_Temp";
Adafruit_MQTT_Publish Living_Temp = Adafruit_MQTT_Publish(&mqtt, Living_Temp_FEED);

const char Bed3_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Bed3_Temp";
Adafruit_MQTT_Publish Bed3_Temp = Adafruit_MQTT_Publish(&mqtt, Bed3_Temp_FEED);

const char Bath2_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Bath_Temp";
Adafruit_MQTT_Publish Bath2_Temp = Adafruit_MQTT_Publish(&mqtt, Bath2_Temp_FEED);

const char Kitchen_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Kitchen_Temp";
Adafruit_MQTT_Publish Kitchen_Temp = Adafruit_MQTT_Publish(&mqtt, Kitchen_Temp_FEED);
float Kitchen_Temp_last = 0.0;

const char Ext_Temp_FEED[] PROGMEM = AIO_USERNAME "/feeds/Ext_Temp";
Adafruit_MQTT_Publish Ext_Temp = Adafruit_MQTT_Publish(&mqtt, Ext_Temp_FEED);

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup()
{

  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

    pinMode(BUILTIN_LED, OUTPUT);

    Initialize();

    // Connect to the WiFi network and get an address from DHCP
    GetIPAddress();
    SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp

    // This node will serve all the others in the network providing an address
    SetAddressingServer();

    // LIST OF NODES
  	SetAsPeerNode(IP_ADDRESS_ROW1B1,1);
  	SetAsPeerNode(RS485_ADDRESS_ROW1B2,2);
  	SetAsPeerNode(RS485_ADDRESS_ROW1B3,3);
  	SetAsPeerNode(RS485_ADDRESS_ROW1B4,4);

  	SetAsPeerNode(IP_ADDRESS_ROW2B1,5);
  	SetAsPeerNode(RS485_ADDRESS_ROW2B2,6);
  	SetAsPeerNode(RS485_ADDRESS_ROW2B3,7);
  	SetAsPeerNode(RS485_ADDRESS_ROW2B4,8);

  	SetAsPeerNode(IP_ADDRESS_BED1B1,9);

  	SetAsPeerNode(IP_ADDRESS_BED2B1,10);
  	SetAsPeerNode(RS485_ADDRESS_BED2B2,11);
  	SetAsPeerNode(RS485_ADDRESS_BED2B3,12);

  	SetAsPeerNode(RS485_ADDRESS_GTW1B1,13);
  	SetAsPeerNode(RS485_ADDRESS_GTW2B1,14);

  	// define static routing table (NOT STRICTLY NEEDED if using PEER_Join in peers)
  	RoutingTable(IP_ADDRESS_ROW1B1, RS485_ADDRESS_ROW1SN, 0);
  	RoutingTable(IP_ADDRESS_ROW2B1, RS485_ADDRESS_ROW2SN, 1);
  	RoutingTable(IP_ADDRESS_BED1B1, RS485_ADDRESS_BED1SN, 2);
  	RoutingTable(IP_ADDRESS_BED2B1, RS485_ADDRESS_BED2SN, 3);

}

void loop()
{
    EXECUTEFAST() {
        UPDATEFAST();
        FAST_GatewayComms();

        MQTT_connect();

        FAST_50ms()
        {
          Serial.print(".");
          if(LastIn_IsData(ROW1B1))
          {
            digitalWrite(BUILTIN_LED, LOW);

            Serial.println("********* ROW1B1 UPDATED");
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_1));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_2));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_3));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_4));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_5));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LIGHT_LIVING_6));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_EXT_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_EXT_UR));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LOFT_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_LOFT_UR));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_BOILER_SANITARY_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_BOILER_HEATING_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_BOILER_BOTTOM_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_HEATPUMP_FLOW_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_HEATPUMP_RETURN_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_FANCOILS_FLOW_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_FANCOILS_RETURN_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_FLOOR_FLOW_TEMP));
            Serial.println(LastIn_GetData(ROW1B1, ROW1B1_HVAC_FLOOR_RETURN_TEMP));

            double temp = LastIn_GetAnalog(ROW1B1, ROW1B1_HVAC_BOILER_SANITARY_TEMP);
            HVAC_Boiler_Sanitary_Temp.publish(temp);

            temp = LastIn_GetAnalog(ROW1B1, ROW1B1_HVAC_FLOOR_FLOW_TEMP);
            HVAC_Floor_Flow_Temp.publish(temp);
            }
            LastIn_ClearData(ROW1B1);
            digitalWrite(BUILTIN_LED, HIGH);
          }

          if(LastIn_IsData(ROW2B3))
          {
            digitalWrite(BUILTIN_LED, LOW);

            Serial.println("********* ROW2B3 UPDATED");

            double temp = LastIn_GetAnalog(ROW2B3, ROW2B3_TEMPERATURE);
            Kitchen_Temp.publish(temp);

            LastIn_ClearData(ROW2B3);
            digitalWrite(BUILTIN_LED, HIGH);
          }

      }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

//  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
//       Serial.println(mqtt.connectErrorString(ret));
//       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
//  Serial.println("MQTT Connected!");
}
