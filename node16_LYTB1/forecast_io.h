#define FORECAST_IO_SERVER "api.forecast.io"
#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[hourly,minutely,daily,alerts,flags]"

#define USE_SSL
#include "http_request.h"

inline void ForecastIO_WeatherRequest(char* response, const char* request)
{
  if (connect(FORECAST_IO_SERVER))
  {
    if (sendRequest(FORECAST_IO_SERVER, request) && skipResponseHeaders())
    {
      readReponseContent(response, sizeof(response));
    }
  }
}

inline String buildForecastIORequest(char* day_time)
{
  return String("/forecast/") + String(API_KEY) + "/" + String(COORDINATES) + "," + day_time + "?" + String(OPTIONS);
}
