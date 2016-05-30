// https://developer.forecast.io/docs/v2
// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682

// requesting time machine just current
// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682,2016-05-02T14:00:00?exclude=[hourly,minutely,daily,alerts,flags]
// returns about 360 bytes

#define FORECAST_IO_SERVER "api.forecast.io"
#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[hourly,minutely,daily,alerts,flags]"

#define USE_SSL
#include "http_request.h"
//#include "ArduinoJson.h"

char forecast_io_response[MAX_CONTENT_SIZE];

inline String ForecastIO_BuildTimeMachineRequest(const char* day_time)
{
  return String("/forecast/") + String(API_KEY) + "/" + String(COORDINATES) + "," + day_time + "?" + String(OPTIONS);
}

inline String ForecastIO_BuildCurrentWeatherRequest()
{
  return String("/forecast/") + String(API_KEY) + "/" + String(COORDINATES) + "?" + String(OPTIONS);
}

inline void ForecastIO_Request(const char* request)
{
  if (connect(FORECAST_IO_SERVER))
  {
    if (sendRequest(FORECAST_IO_SERVER, request) && skipResponseHeaders())
    {
      readReponseContent(forecast_io_response, MAX_CONTENT_SIZE);
    }
  }
}

inline void ForecastIO_TimeMachineRequest(const char* day_time)
{
  ForecastIO_Request(ForecastIO_BuildTimeMachineRequest(day_time).c_str());
}

inline void ForecastIO_CurrentWeatherRequest()
{
  ForecastIO_Request(ForecastIO_BuildCurrentWeatherRequest().c_str());
}

struct WeatherData
{
//  String summary;
  String s_icon;
  float f_precipIntensity;
  float f_precipProbability;
};

#define WEATHER_DATA_COUNT 24
#define CURRENT_WEATHER_IDX (WEATHER_DATA_COUNT/2 - 1)
WeatherData wd[WEATHER_DATA_COUNT];

/*
{
  "latitude":41.7901754,
  "longitude":12.4102682,
  "timezone":"Europe/Rome",
  "offset":2,
  "currently":
  {
    "time":1464645835,
    "summary":"Clear",
    "icon":"clear-night",
    "precipIntensity":0,
    "precipProbability":0,
    "temperature":60.87,
    "apparentTemperature":60.87,
    "dewPoint":54.33,
    "humidity":0.79,
    "windSpeed":5.65,
    "windBearing":166,
    "visibility":6.21,
    "cloudCover":0.03,
    "pressure":1015.71,
    "ozone":344.63
  }
}
*/
bool ForecastIO_ParseResponse(const char* response, WeatherData* pWD)
{
  String sResponse(response);

  int idx_start = sResponse.indexOf("icon")+7;
  int idx_stop = sResponse.indexOf('\"',idx_start);
  pWD->s_icon = sResponse.substring(idx_start, idx_stop);
//  grhSendUDPMessage(pWD->s_icon.c_str());

  idx_start = sResponse.indexOf("precipIntensity")+17;
  idx_stop = sResponse.indexOf(',',idx_start);
  pWD->f_precipIntensity = sResponse.substring(idx_start, idx_stop).toFloat();

  idx_start = sResponse.indexOf("precipProbability")+19;
  idx_stop = sResponse.indexOf(',',idx_start);
  pWD->f_precipProbability = sResponse.substring(idx_start, idx_stop).toFloat();

  return true;
}

inline bool ForecastIO_ParseResponse(uint8_t idx)
{
  return ForecastIO_ParseResponse(forecast_io_response, &(wd[idx]));
}


#define ICON_CLEAR_DAY "clear-day"
#define ICON_CLEAR_NIGHT "clear-night"
#define ICON_RAIN "rain"
#define ICON_SNOW "snow"
#define ICON_SLEET "sleet"
#define ICON_WIND "wind"
#define ICON_FOG "fog"
#define ICON_CLOUDY "cloudy"
#define ICON_PARTLY_CLOUDY_DAY "partly-cloudy-day"
#define ICON_PARTLY_CLOUDY_NIGHT "partly-cloudy-night"
#define ICON_HAIL "hail"
#define ICON_THUNDERSTORM "thunderstorm"
#define ICON_TORNADO "tornado"

void ForecastIO_SetColor(U8 slot)
{
  if(wd[CURRENT_WEATHER_IDX].s_icon == ICON_CLEAR_DAY ||
    wd[CURRENT_WEATHER_IDX].s_icon == ICON_CLEAR_NIGHT ||
      wd[CURRENT_WEATHER_IDX].s_icon == ICON_WIND)
  {
    SetColor(slot, 0xFF, 0xFF, 0x00);
  }
  else if(wd[CURRENT_WEATHER_IDX].s_icon == ICON_RAIN ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_SNOW ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_SLEET ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_HAIL ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_THUNDERSTORM ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_TORNADO)
  {
    SetColor(slot, 0x00, 0x00, 0xFF);
  }
  else if(wd[CURRENT_WEATHER_IDX].s_icon == ICON_FOG ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_CLOUDY)
  {
    SetColor(slot, 0xFF, 0xFF, 0xFF);
  }
  else if(wd[CURRENT_WEATHER_IDX].s_icon == ICON_PARTLY_CLOUDY_DAY ||
     wd[CURRENT_WEATHER_IDX].s_icon == ICON_PARTLY_CLOUDY_NIGHT)
  {
    SetColor(slot, 0xCC, 0xFF, 0xFF);
  }
  else
  {
    SetColor(slot, 0xFF, 0x00, 0x00);
  }
}
