#include "stdafx.h"
#include "OpenWeatherService.h"
#include <utility>
#include "LiveWeatherDevice.h"
#include "shared/http/StandardSession.h"
#include "shared/http/HttpMethods.h"


const std::string COpenWeatherService::LiveWeatherDeviceName("Weather");


COpenWeatherService::COpenWeatherService(boost::shared_ptr<yApi::IYPluginApi> api,
                                         std::string apiKey)
   : m_api(std::move(api)),
     m_apiKey(std::move(apiKey))
{
}

boost::posix_time::time_duration COpenWeatherService::serviceRecommendedRetryDelay() const
{
   // See https://openweathermap.org/appid#work
   return boost::posix_time::minutes(10);
}

void COpenWeatherService::declareDevices() const
{
   CLiveWeatherDevice liveWeatherDevice(LiveWeatherDeviceName);
   liveWeatherDevice.declareDevice(m_api);
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
      YADOMS_LOG(information) << "Location name " << weatherData.get<std::string>("name");

      CLiveWeatherDevice liveWeatherDevice(LiveWeatherDeviceName);

      if (weatherData.containsChildArray("weather"))
      {
         try
         {
            liveWeatherDevice.setCondition(toYadomsCondition(weatherData.get<std::vector<shared::CDataContainer>>("weather")[0].get("main")));
         }
         catch (const std::out_of_range& exOutOfRange)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, when \"weather.main\" = " << exOutOfRange.what();
         }
         catch (const std::exception& exception)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, " << exception.what();
         }
      }
      if (weatherData.containsValue("main.temp"))
         liveWeatherDevice.setTemperature(weatherData.get<double>("main.temp"));
      if (weatherData.containsValue("main.temp_min"))
         liveWeatherDevice.setTemperatureMin(weatherData.get<double>("main.temp_min"));
      if (weatherData.containsValue("main.temp_max"))
         liveWeatherDevice.setTemperatureMax(weatherData.get<double>("main.temp_max"));
      if (weatherData.containsValue("main.humidity"))
         liveWeatherDevice.setHumidity(weatherData.get<double>("main.humidity"));
      if (weatherData.containsValue("main.pressure"))
         liveWeatherDevice.setPressure(weatherData.get<double>("main.pressure"));
      if (weatherData.containsValue("wind.speed"))
         liveWeatherDevice.setWindSpeed(weatherData.get<double>("wind.speed"));
      if (weatherData.containsValue("wind.deg"))
         liveWeatherDevice.setWindDirection(weatherData.get<int>("wind.deg"));
      if (weatherData.containsValue("visibility"))
         liveWeatherDevice.setVisibility(weatherData.get<int>("visibility"));

      liveWeatherDevice.historize(m_api);
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Fail to process OpenWeather answer, " << exception.what();
      throw;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsCondition(const std::string& owCondition)
{
   // See https://openweathermap.org/weather-conditions

   if (owCondition == "Thunderstorm")
      return yApi::historization::EWeatherCondition::kStorm;
   if (owCondition == "Drizzle")
      return yApi::historization::EWeatherCondition::kRain;
   if (owCondition == "Rain")
      return yApi::historization::EWeatherCondition::kRain;
   if (owCondition == "Snow")
      return yApi::historization::EWeatherCondition::kSnow;

   if (owCondition == "Mist")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Smoke")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Haze")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Dust")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Fog")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Sand")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Ash")
      return yApi::historization::EWeatherCondition::kFog;
   if (owCondition == "Squall")
      return yApi::historization::EWeatherCondition::kStorm;
   if (owCondition == "Tornado")
      return yApi::historization::EWeatherCondition::kStorm;

   if (owCondition == "Clear")
      return yApi::historization::EWeatherCondition::kSunny;
   if (owCondition == "Clouds")
      return yApi::historization::EWeatherCondition::kCloudy;

   throw std::out_of_range(owCondition);
}
