#include "stdafx.h"
#include "OpenWeatherService.h"
#include <utility>
#include "LiveWeatherDevice.h"
#include "shared/http/StandardSession.h"
#include "shared/http/HttpMethods.h"


const std::string COpenWeatherService::LiveWeatherDeviceName("Weather");


COpenWeatherService::COpenWeatherService(boost::shared_ptr<yApi::IYPluginApi> api,
                                         std::string apiKey)
   : m_api(api),
     m_apiKey(std::move(apiKey)),
     m_liveWeatherDevice(boost::make_shared<CLiveWeatherDevice>(LiveWeatherDeviceName))
{
}

boost::posix_time::time_duration COpenWeatherService::serviceRecommendedRetryDelay() const
{
   // See https://openweathermap.org/appid#work
   return boost::posix_time::minutes(10);
}

void COpenWeatherService::declareDevices() const
{
   m_liveWeatherDevice->declareDevice(m_api);
}

void COpenWeatherService::requestWeather(boost::shared_ptr<const shared::ILocation> forLocation) const
{
   const auto url(std::string("http://api.openweathermap.org/data/2.5/weather")
      + "?APPID=" + m_apiKey
      + "&units=metric"
      + "&lat=" + std::to_string(forLocation->latitude())
      + "&lon=" + std::to_string(forLocation->longitude()));

   const auto weatherData = syncRequest(url);
   processAnswer(weatherData);
}

shared::CDataContainer COpenWeatherService::syncRequest(const std::string& url) const
{
   try
   {
      YADOMS_LOG(debug) << "URL = " << url;
      const auto weatherData = shared::CHttpMethods::sendGetRequest(url);

      weatherData.printToLog(YADOMS_LOG(debug));

      if (weatherData.get<int>("cod") != 200)
         throw std::runtime_error(weatherData.get<std::string>("cod") + ", " +
            weatherData.getWithDefault<std::string>("message", "Unknown error"));

      return weatherData;
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Fail to request OpenWeather service, " << exception.what();
      throw;
   }
}

void COpenWeatherService::processAnswer(const shared::CDataContainer& weatherData) const
{
   try
   {
      CLiveWeatherDevice currentWeather("Current weather");
      if (weatherData.containsValue("main.temp"))
         currentWeather.setTemperature(weatherData.get<double>("main.temp"));

      //TODO process data
      //TODO historize data
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Fail to process OpenWeather answer, " << exception.what();
      throw;
   }
}
