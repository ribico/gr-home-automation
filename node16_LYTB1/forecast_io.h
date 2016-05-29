#define FORECAST_IO_SERVER "api.forecast.io"
#define API_KEY "f68f1acb8091e1299995085418c1d163"
#define COORDINATES "41.7901754,12.4102682"
#define OPTIONS "exclude=[hourly,minutely,daily,alerts,flags]"

#define USE_SSL
#include "http_request.h"

char forecast_io_response[MAX_CONTENT_SIZE];

inline String buildForecastIORequest(const char* day_time)
{
  return String("/forecast/") + String(API_KEY) + "/" + String(COORDINATES) + "," + day_time + "?" + String(OPTIONS);
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
  ForecastIO_Request(buildForecastIORequest(day_time).c_str());
}
