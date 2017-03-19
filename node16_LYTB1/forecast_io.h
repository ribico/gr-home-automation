// https://developer.forecast.io/docs/v2
// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682

// requesting time machine just current
// https://api.forecast.io/forecast/f68f1acb8091e1299995085418c1d163/41.7901754,12.4102682,2016-05-02T14:00:00?exclude=[minutely,daily,alerts,flags]
// returns about 360 bytes for current weather + many others for hourly

#define FORECAST_IO_SERVER "api.forecast.io"
#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[minutely,daily,alerts,flags]"

#define USE_SSL
#include "http_request.h"

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
    if (sendRequest(FORECAST_IO_SERVER, request) && skipResponseSection("hourly\":{"))
    {
      readReponseContent(forecast_io_response, MAX_CONTENT_SIZE);
    }
  }
}


inline void ForecastIO_CurrentWeatherRequest()
{
  ForecastIO_Request(ForecastIO_BuildCurrentWeatherRequest().c_str());
}


/*
after skipping response until houlry section:

    "summary":"Mostly cloudy throughout the day.",
    "icon":"partly-cloudy-day",
    "data":[{"time":1489878000,"summary":"Partly Cloudy","icon":"partly-cloudy-night","precipIntensity":0 .........
*/

String ForecastIO_ParseDayIcon(String sResponse)
{
  // extract hourly icon summary
  int idx_start = sResponse.indexOf("icon")+7;
  int idx_stop = sResponse.indexOf('\"', idx_start);
  String sIcon = sResponse.substring(idx_start, idx_stop);
  
  return sIcon;
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

void ForecastIO_SetColor(U8 slot, String sIcon)
{
  if(sIcon == ICON_CLEAR_DAY ||
    sIcon == ICON_CLEAR_NIGHT ||
      sIcon == ICON_WIND)
  {
    SetColor(slot, 0x04, 0x02, 0x00);
  }
  else if(sIcon == ICON_RAIN ||
     sIcon == ICON_SNOW ||
     sIcon == ICON_SLEET ||
     sIcon == ICON_HAIL ||
     sIcon == ICON_THUNDERSTORM ||
     sIcon == ICON_TORNADO)
  {
    SetColor(slot, 0x00, 0x00, 0x05);
  }
  else if(sIcon == ICON_FOG ||
     sIcon == ICON_CLOUDY)
  {
    SetColor(slot, 0x01, 0x01, 0x01);
  }
  else if(sIcon == ICON_PARTLY_CLOUDY_DAY ||
     sIcon == ICON_PARTLY_CLOUDY_NIGHT)
  {
    SetColor(slot, 0x01, 0x03, 0x01);
  }
  else
  {
    SetColor(slot, 0x01, 0x00, 0x00);
  }
}
