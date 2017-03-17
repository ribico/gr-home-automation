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

struct WeatherData
{
//  String summary;
  time_t t_time;
  String s_icon;
  float f_precipIntensity;
  float f_precipProbability;
};

#define WEATHER_DATA_COUNT 24
#define CURRENT_WEATHER_IDX (WEATHER_DATA_COUNT/2 - 1)
WeatherData wd[WEATHER_DATA_COUNT];

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

// day_time : should either be a UNIX time (that is, seconds since midnight GMT
// on 1 Jan 1970) or a string formatted as follows: [YYYY]-[MM]-[DD]T[HH]:[MM]:[SS]
// (with an optional time zone formatted as Z for GMT time or {+,-}[HH][MM]
// for an offset in hours or minutes). For the latter format, if no timezone is
//  present, local time (at the provided latitude and longitude) is assumed.
// (This string format is a subset of ISO 8601 time.
// An as example, 2013-05-06T12:00:00-0400.)
//
inline void ForecastIO_TimeMachineRequest(const char* day_time)
{
  ForecastIO_Request(ForecastIO_BuildTimeMachineRequest(day_time).c_str());
}

inline void ForecastIO_TimeMachineRequest(int offset_hours)
{
  if (offset_hours == 0)
  {
    ForecastIO_Request(ForecastIO_BuildCurrentWeatherRequest().c_str());
  }
  else
  {
    time_t offset_seconds = offset_hours * 3600;
    String sRequestedTime(wd[CURRENT_WEATHER_IDX].t_time + offset_seconds);
    ForecastIO_Request(ForecastIO_BuildTimeMachineRequest(sRequestedTime.c_str()).c_str());
  }
}

inline void ForecastIO_CurrentWeatherRequest()
{
  ForecastIO_TimeMachineRequest(0);
}


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

  int idx_start = sResponse.indexOf("time\":")+6;
  int idx_stop = sResponse.indexOf(',', idx_start);
  pWD->t_time = sResponse.substring(idx_start, idx_stop).toInt();

  idx_start = sResponse.indexOf("icon", idx_stop)+7;
  idx_stop = sResponse.indexOf('\"', idx_start);
  pWD->s_icon = sResponse.substring(idx_start, idx_stop);
//  grhSendUDPMessage(pWD->s_icon.c_str());

  idx_start = sResponse.indexOf("precipIntensity", idx_stop)+17;
  idx_stop = sResponse.indexOf(',', idx_start);
  pWD->f_precipIntensity = sResponse.substring(idx_start, idx_stop).toFloat();

  idx_start = sResponse.indexOf("precipProbability", idx_stop)+19;
  idx_stop = sResponse.indexOf(',', idx_start);
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

void ForecastIO_SetColor(U8 slot, WeatherData* pWD)
{
  if(pWD->s_icon == ICON_CLEAR_DAY ||
    pWD->s_icon == ICON_CLEAR_NIGHT ||
      pWD->s_icon == ICON_WIND)
  {
    SetColor(slot, 0xFF, 0xFF, 0x00);
  }
  else if(pWD->s_icon == ICON_RAIN ||
     pWD->s_icon == ICON_SNOW ||
     pWD->s_icon == ICON_SLEET ||
     pWD->s_icon == ICON_HAIL ||
     pWD->s_icon == ICON_THUNDERSTORM ||
     pWD->s_icon == ICON_TORNADO)
  {
    SetColor(slot, 0x00, 0x00, 0xFF);
  }
  else if(pWD->s_icon == ICON_FOG ||
     pWD->s_icon == ICON_CLOUDY)
  {
    SetColor(slot, 0xFF, 0xFF, 0xFF);
  }
  else if(pWD->s_icon == ICON_PARTLY_CLOUDY_DAY ||
     pWD->s_icon == ICON_PARTLY_CLOUDY_NIGHT)
  {
    SetColor(slot, 0xCC, 0xFF, 0xFF);
  }
  else
  {
    SetColor(slot, 0xFF, 0x00, 0x00);
  }
}
