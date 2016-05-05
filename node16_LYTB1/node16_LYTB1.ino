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
#include <WiFiClient.h>
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


// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682

// requesting time machine just current
// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682,2016-05-02T14:00:00?exclude=[hourly,minutely,daily,alerts,flags]
// returns about 360 bytes

#include "ArduinoJson.h"



byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[hourly,minutely,daily,alerts,flags]"

const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response


WiFiClient client;

struct WeatherData
{
//  String summary;
  String icon;
};

WeatherData wd[24];


// Open connection to the HTTP server
bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);

  bool ok = client.connect(hostName, 80);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  Serial.print("GET ");
  Serial.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}


// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }

  return ok;
}

// Read the body of the response from the HTTP server
void readReponseContent(char* content, size_t maxSize) {
  size_t length = client.readBytes(content, maxSize);
  content[length] = 0;
  Serial.println(content);
}

/*
{
  latitude: 41.7901754,
  longitude: 12.4102682,
  timezone: "Europe/Rome",
  offset: 2,
  currently: {
    time: 1462185305,
    summary: "Partly Cloudy",
    icon: "partly-cloudy-day",
    precipIntensity: 0.0033,
    precipProbability: 0.11,
    precipType: "rain",
    temperature: 59.7,
    apparentTemperature: 59.7,
    dewPoint: 46.74,
    humidity: 0.62,
    windSpeed: 8.93,
    windBearing: 314,
    visibility: 6.21,
    cloudCover: 0.43,
    pressure: 1009.18,
    ozone: 427.16
  }
}
*/
bool parseUserData(char* content, WeatherData* pWD) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // This is only required if you use StaticJsonBuffer.
  const size_t BUFFER_SIZE =
      JSON_OBJECT_SIZE(4)     // the currently object has 8 elements
      + JSON_OBJECT_SIZE(16);   // the "address" object has 5 elements

  // Allocate a temporary memory pool on the stack
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  // If the memory pool is too big for the stack, use this instead:
  // DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(content);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  // Here were copy the strings we're interested in
//  strcpy(wd->summary, root["currently"]["summary"]);
  pWD->icon = String((char*)root["currently"]["icon"]);
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string

  return true;
}

inline bool getWeather(String time, String summary)
{
  String server = "api.forecast.io";  // server's address
  String resource = "/forecast/" + String(API_KEY) + "/" + String(COORDINATES) + "," + time + "?" + String(OPTIONS);                    // http resource

  if (connect(server.c_str())) {
    if (sendRequest(server.c_str(), resource.c_str()) && skipResponseHeaders()) {
      char response[MAX_CONTENT_SIZE];
      readReponseContent(response, sizeof(response));

      if (parseUserData(response, (wd.icon).c_str())) {
//        printUserData(&userData);
      }
    }
    disconnect();
  }
}

  return true;
}


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
