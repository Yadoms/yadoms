#include "stdafx.h"
#include "OpenWeatherService.h"
#include "ForecastWeatherDevice.h"
#include "LiveWeatherDevice.h"
#include <shared/http/HttpRestHelpers.h>
#include <algorithm>


// OpenWeather gives up to 5 days forecast, by 3 hours, so 40 forecasts.
// It's too many data for us, we keep only 24 hours with 3 hours details,
// and compute day summary for the 5 days.
const unsigned int COpenWeatherService::NbForecastHours(8);
const unsigned int COpenWeatherService::NbForecastDays(5);
const std::string COpenWeatherService::LiveWeatherDeviceName("Live weather");
const std::string COpenWeatherService::ServiceName("OpenWeather");
const std::string COpenWeatherService::ForecastWeatherPerDayDevicePrefix("Forecast weather day + ");
const std::string COpenWeatherService::ForecastWeatherPerHourDevicePrefix("Forecast weather hour + ");

COpenWeatherService::COpenWeatherService(boost::shared_ptr<yApi::IYPluginApi> api,
                                         std::string apiKey)
   : m_api(std::move(api)),
     m_apiKey(move(apiKey))
{
}

boost::posix_time::time_duration COpenWeatherService::serviceRecommendedRetryDelay() const
{
   // See https://openweathermap.org/appid#work
   return boost::posix_time::minutes(10);
}

std::string COpenWeatherService::getForecastWeatherDeviceNameForDay(unsigned int forecastDay) const
{
   return ForecastWeatherPerDayDevicePrefix + std::to_string(forecastDay + 1);
}

std::string COpenWeatherService::getForecastWeatherDeviceNameForHour(unsigned int forecastHour) const
{
   return ForecastWeatherPerHourDevicePrefix + std::to_string((forecastHour + 1) * 3);
}

void COpenWeatherService::declareDevices() const
{
   const CLiveWeatherDevice liveWeatherDevice(LiveWeatherDeviceName);
   liveWeatherDevice.declareDevice(m_api,
                                   ServiceName);

   for (unsigned int forecastHour = 0; forecastHour < NbForecastHours; ++forecastHour)
   {
      CForecastWeatherDevice forecastWeatherDevice(getForecastWeatherDeviceNameForHour(forecastHour));
      forecastWeatherDevice.declareDevice(m_api,
                                          ServiceName);
   }

   for (unsigned int forecastDay = 0; forecastDay < NbForecastDays; ++forecastDay)
   {
      CForecastWeatherDevice forecastWeatherDevice(getForecastWeatherDeviceNameForDay(forecastDay));
      forecastWeatherDevice.declareDevice(m_api,
                                          ServiceName);
   }
}

void COpenWeatherService::requestWeather(boost::shared_ptr<const shared::ILocation> forLocation) const
{
   requestLiveWeather(*forLocation);
   requestForecastWeather(*forLocation);
}

boost::shared_ptr<shared::CDataContainer> COpenWeatherService::syncRequest(const std::string& url)
{
   try
   {
      YADOMS_LOG(debug) << "URL = " << url;
      const auto answer = shared::http::CHttpRestHelpers::sendJsonGetRequest(url);

      answer->printToLog(YADOMS_LOG(debug));

      if (answer->containsValue("cod") && answer->get<int>("cod") != 200)
         throw std::runtime_error(answer->get<std::string>("cod") + ", " +
            answer->getWithDefault<std::string>("message", "Unknown error"));

      return answer;
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(warning) << "Fail to request OpenWeather service, " << exception.what();
      throw;
   }
}

void COpenWeatherService::requestLiveWeather(const shared::ILocation& forLocation) const
{
   // General weather
   const auto weatherUrl(std::string("http://api.openweathermap.org/data/2.5/weather")
      + "?APPID=" + m_apiKey
      + "&units=metric"
      + "&lat=" + std::to_string(forLocation.latitude())
      + "&lon=" + std::to_string(forLocation.longitude()));
   const auto weatherData = syncRequest(weatherUrl);

   // UV index
   const auto uvIndexUrl(std::string("http://api.openweathermap.org/data/2.5/uvi")
      + "?APPID=" + m_apiKey
      + "&units=metric"
      + "&lat=" + std::to_string(forLocation.latitude())
      + "&lon=" + std::to_string(forLocation.longitude()));
   boost::shared_ptr<shared::CDataContainer> uvIndexData;
   try
   {
      uvIndexData = syncRequest(uvIndexUrl);
   }
   catch (const std::exception& e)
   {
      YADOMS_LOG(warning) << "UV live index not available, " << e.what();
   }

   processLiveWeatherAnswer(weatherData,
                            uvIndexData);
}

void COpenWeatherService::processLiveWeatherAnswer(const boost::shared_ptr<shared::CDataContainer>& weatherData,
                                                   const boost::shared_ptr<shared::CDataContainer>& uvIndexData) const
{
   try
   {
      YADOMS_LOG(information) << "Location name " << weatherData->get<std::string>("name");

      CLiveWeatherDevice weatherDevice(LiveWeatherDeviceName);

      if (weatherData->containsChildArray("weather"))
      {
         try
         {
            weatherDevice.setCondition(toYadomsCondition(
               weatherData->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("weather")[0]->get<int>("id")));
         }
         catch (const std::out_of_range& exOutOfRange)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, when \"weather.id\" = "
               << exOutOfRange.what();
         }
         catch (const std::exception& exception)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, " << exception.what();
         }
      }
      if (weatherData->containsValue("main.temp"))
         weatherDevice.setTemperature(weatherData->get<double>("main.temp"));
      if (weatherData->containsValue("main.temp_min"))
         weatherDevice.setTemperatureMin(weatherData->get<double>("main.temp_min"));
      if (weatherData->containsValue("main.temp_max"))
         weatherDevice.setTemperatureMax(weatherData->get<double>("main.temp_max"));
      if (weatherData->containsValue("main.humidity"))
         weatherDevice.setHumidity(weatherData->get<double>("main.humidity"));
      if (weatherData->containsValue("main.pressure"))
         weatherDevice.setPressure(weatherData->get<double>("main.pressure"));
      if (weatherData->containsValue("wind.speed"))
         weatherDevice.setWindSpeed(weatherData->get<double>("wind.speed"));
      if (weatherData->containsValue("wind.deg"))
         weatherDevice.setWindDirection(weatherData->get<int>("wind.deg"));

      weatherDevice.setRainForLast3h(weatherData->containsValue("rain.3h") ? weatherData->get<double>("rain.3h") : 0.0);
      weatherDevice.setSnowForLast3h(weatherData->containsValue("snow.3h") ? weatherData->get<double>("snow.3h") : 0.0);

      if (weatherData->containsValue("visibility"))
         weatherDevice.setVisibility(weatherData->get<int>("visibility"));
      if (uvIndexData && uvIndexData->containsValue("value"))
         weatherDevice.setUV(uvIndexData->get<double>("value"));

      weatherDevice.historize(m_api);
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Fail to process OpenWeather answer, " << exception.what();
      throw;
   }
}

void COpenWeatherService::requestForecastWeather(const shared::ILocation& forLocation) const
{
   // General weather
   const auto weatherUrl(std::string("http://api.openweathermap.org/data/2.5/forecast")
      + "?APPID=" + m_apiKey
      + "&units=metric"
      + "&lat=" + std::to_string(forLocation.latitude())
      + "&lon=" + std::to_string(forLocation.longitude()));
   const auto weatherData = syncRequest(weatherUrl);

   // UV index
   const auto uvIndexUrl(std::string("http://api.openweathermap.org/data/2.5/uvi/forecast")
      + "?APPID=" + m_apiKey
      + "&units=metric"
      + "&lat=" + std::to_string(forLocation.latitude())
      + "&lon=" + std::to_string(forLocation.longitude()));
   boost::shared_ptr<shared::CDataContainer> uvIndexData;
   try
   {
      uvIndexData = syncRequest(uvIndexUrl);
   }
   catch (const std::exception& e)
   {
      YADOMS_LOG(warning) << "UV forecast index not available, " << e.what();
   }

   processForecastWeatherAnswer(weatherData,
                                uvIndexData);
}

void COpenWeatherService::historize3HoursForecast(unsigned int hourIndex,
                                                  const boost::posix_time::ptime& forecastDatetime,
                                                  const boost::shared_ptr<shared::CDataContainer>& forecast) const
{
   CForecastWeatherDevice weatherDevice(getForecastWeatherDeviceNameForHour(hourIndex));

   weatherDevice.setForecastDatetime(forecastDatetime);

   if (forecast->containsChildArray("weather"))
   {
      try
      {
         weatherDevice.setCondition(toYadomsCondition(
            forecast->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("weather")[0]->get<int>("id")));
      }
      catch (const std::out_of_range& exOutOfRange)
      {
         YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, when \"weather.id\" = " <<
            exOutOfRange.what();
      }
      catch (const std::exception& exception)
      {
         YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, " << exception.what();
      }
   }

   if (forecast->containsValue("main.temp"))
      weatherDevice.setTemperature(forecast->get<double>("main.temp"));
   if (forecast->containsValue("main.temp_min"))
      weatherDevice.setTemperatureMin(forecast->get<double>("main.temp_min"));
   if (forecast->containsValue("main.temp_max"))
      weatherDevice.setTemperatureMax(forecast->get<double>("main.temp_max"));
   if (forecast->containsValue("main.humidity"))
      weatherDevice.setHumidity(forecast->get<double>("main.humidity"));
   if (forecast->containsValue("main.pressure"))
      weatherDevice.setPressure(forecast->get<double>("main.pressure"));
   if (forecast->containsValue("wind.speed"))
      weatherDevice.setWindSpeed(forecast->get<double>("wind.speed"));
   if (forecast->containsValue("wind.deg"))
      weatherDevice.setWindDirection(forecast->get<int>("wind.deg"));

   weatherDevice.setRainForNextPeriod(forecast->containsValue("rain.3h") ? forecast->get<double>("rain.3h") : 0.0);
   weatherDevice.setSnowForNextPeriod(forecast->containsValue("snow.3h") ? forecast->get<double>("snow.3h") : 0.0);

   weatherDevice.historize(m_api);
}

void COpenWeatherService::historizeDaysForecast(
   const std::map<int, std::vector<boost::shared_ptr<shared::CDataContainer>>>& forecastDataByDay,
   const std::map<int, double>& uvIndexByDay) const
{
   for (const auto& forecastDataForOneDay : forecastDataByDay)
   {
      const auto dayIndex = forecastDataForOneDay.first;
      boost::posix_time::ptime forecastDate;
      auto averageWeatherCondition = static_cast<int>(0);
      auto averageWeatherConditionDataCount = 0;
      auto averageTemperature = static_cast<double>(0.0f);
      auto averageTemperatureDataCount = 0;
      auto minTemperature = static_cast<double>(0.0f);
      auto minTemperatureDataCount = 0;
      auto maxTemperature = static_cast<double>(0.0f);
      auto maxTemperatureDataCount = 0;
      auto averageHumidity = static_cast<double>(0.0f);
      auto averageHumidityDataCount = 0;
      auto averagePressure = static_cast<double>(0.0f);
      auto averagePressureDataCount = 0;
      auto averageWindSpeed = static_cast<double>(0.0f);
      auto averageWindSpeedDataCount = 0;
      auto averageWindDeg = static_cast<int>(0);
      auto averageWindDegDataCount = 0;
      auto totalDayRain = static_cast<double>(0.0f);
      auto totalDaySnow = static_cast<double>(0.0f);

      for (const auto& forecastData : forecastDataForOneDay.second)
      {
         // For weather condition, we consider that an average does the job

         forecastDate = boost::posix_time::ptime(
            boost::posix_time::time_from_string(forecastData->get<std::string>("dt_txt")).date());

         try
         {
            averageWeatherCondition += toYadomsCondition(
               forecastData->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("weather")[0]->get<int>("id"));
            ++averageWeatherConditionDataCount;
         }
         catch (const std::out_of_range& exOutOfRange)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, when \"weather.id\" = "
               << exOutOfRange.what();
         }
         catch (const std::exception& exception)
         {
            YADOMS_LOG(warning) << "Can not convert weather condition from OpenWeather service, " << exception.what();
         }


         if (forecastData->containsValue("main.temp"))
         {
            averageTemperature += forecastData->get<double>("main.temp");
            ++averageTemperatureDataCount;
         }

         if (forecastData->containsValue("main.temp_min"))
         {
            minTemperature = minTemperatureDataCount
                                ? std::min(minTemperature, forecastData->get<double>("main.temp_min"))
                                : forecastData->get<double>("main.temp_min");
            ++minTemperatureDataCount;
         }

         if (forecastData->containsValue("main.temp_max"))
         {
            maxTemperature = maxTemperatureDataCount
                                ? std::max(maxTemperature, forecastData->get<double>("main.temp_max"))
                                : forecastData->get<double>("main.temp_max");
            ++maxTemperatureDataCount;
         }

         if (forecastData->containsValue("main.humidity"))
         {
            averageHumidity += forecastData->get<double>("main.humidity");
            ++averageHumidityDataCount;
         }
         if (forecastData->containsValue("main.pressure"))
         {
            averagePressure += forecastData->get<double>("main.pressure");
            ++averagePressureDataCount;
         }

         if (forecastData->containsValue("wind.speed"))
         {
            averageWindSpeed += forecastData->get<double>("wind.speed");
            ++averageWindSpeedDataCount;
         }

         if (forecastData->containsValue("wind.deg"))
         {
            averageWindDeg += forecastData->get<int>("wind.deg");
            ++averageWindDegDataCount;
         }

         totalDayRain += forecastData->containsValue("rain.3h") ? forecastData->get<double>("rain.3h") : 0.0;
         totalDaySnow += forecastData->containsValue("snow.3h") ? forecastData->get<double>("snow.3h") : 0.0;
      }

      averageWeatherCondition /= averageWeatherConditionDataCount;
      averageTemperature /= averageTemperatureDataCount;
      averageHumidity /= averageHumidityDataCount;
      averagePressure /= averagePressureDataCount;
      averageWindSpeed /= averageWindSpeedDataCount;
      averageWindDeg /= averageWindDegDataCount;

      CForecastWeatherDevice weatherDevice(getForecastWeatherDeviceNameForDay(dayIndex));

      weatherDevice.setForecastDatetime(forecastDate);
      if (averageWeatherConditionDataCount)
         weatherDevice.setCondition(yApi::historization::EWeatherCondition(averageWeatherCondition));
      if (averageTemperatureDataCount)
         weatherDevice.setTemperature(averageTemperature);
      if (minTemperatureDataCount)
         weatherDevice.setTemperatureMin(minTemperature);
      if (maxTemperatureDataCount)
         weatherDevice.setTemperatureMax(maxTemperature);
      if (averageHumidityDataCount)
         weatherDevice.setHumidity(averageHumidity);
      if (averagePressureDataCount)
         weatherDevice.setPressure(averagePressure);
      if (averageWindSpeedDataCount)
         weatherDevice.setWindSpeed(averageWindSpeed);
      if (averageWindDegDataCount)
         weatherDevice.setWindDirection(averageWindDeg);
      weatherDevice.setRainForNextPeriod(totalDayRain);
      weatherDevice.setSnowForNextPeriod(totalDaySnow);

      try
      {
         const auto uvValue = uvIndexByDay.at(dayIndex);
         weatherDevice.setUV(uvValue);
      }
      catch (const std::out_of_range&)
      {
         // No UV index for selected day
      }

      weatherDevice.historize(m_api);
   }
}

void COpenWeatherService::processForecastWeatherAnswer(const boost::shared_ptr<shared::CDataContainer>& weatherData,
                                                       const boost::shared_ptr<shared::CDataContainer>& uvIndexData)
const
{
   try
   {
      YADOMS_LOG(information) << "Location name " << weatherData->get<std::string>("city.name");

      auto forecastDataByDay = std::map<int, std::vector<boost::shared_ptr<shared::CDataContainer>>>();
      auto uvIndexByDay = std::map<int, double>();

      if (weatherData->containsChildArray("list"))
      {
         const auto& forecasts = weatherData->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("list");

         unsigned int hourIndex = 0;
         for (const auto& forecast : forecasts)
         {
            const auto forecastDatetime = boost::posix_time::time_from_string(forecast->get<std::string>("dt_txt"));

            if (hourIndex < NbForecastHours)
            {
               historize3HoursForecast(hourIndex,
                                       forecastDatetime,
                                       forecast);
            }

            if (forecastDatetime.date() > shared::currentTime::Provider().now().date())
            {
               const unsigned int dayIndex = (forecastDatetime.date() - shared::currentTime::Provider().now().date()).
                  days() - 1;
               if (dayIndex < NbForecastDays)
                  forecastDataByDay[dayIndex].push_back(forecast);
            }

            ++hourIndex;
         }
      }

      if (uvIndexData && uvIndexData->containsChildArray("list"))
      {
         const auto& forecasts = uvIndexData->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("list");

         for (const auto& forecast : forecasts)
         {
            const auto forecastDatetime = boost::posix_time::from_time_t(forecast->get<std::time_t>("dt"));

            if (forecastDatetime.date() > shared::currentTime::Provider().now().date())
            {
               const unsigned int dayIndex = (forecastDatetime.date() - shared::currentTime::Provider().now().date()).
                  days() - 1;
               if (dayIndex < NbForecastDays)
                  uvIndexByDay[dayIndex] = forecast->get<double>("uvi");
            }
         }
      }

      historizeDaysForecast(forecastDataByDay,
                            uvIndexByDay);
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Fail to process OpenWeather answer, " << exception.what();
      throw;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsCondition(int owConditionCode)
{
   // See https://openweathermap.org/weather-conditions

   enum class EOwGroupCodes
   {
      kThunderstorm = 2,
      kDrizzle = 3,
      kRain = 5,
      kSnow = 6,
      kAtmosphere = 7,
      kClearClouds = 8
   };

   const auto group = owConditionCode / 100;
   switch (static_cast<EOwGroupCodes>(group))
   {
   case EOwGroupCodes::kThunderstorm:
      return toYadomsConditionsFromOwThunderstorm(owConditionCode);
   case EOwGroupCodes::kDrizzle:
      return toYadomsConditionsFromOwDrizzle(owConditionCode);
   case EOwGroupCodes::kRain:
      return toYadomsConditionsFromOwRain(owConditionCode);
   case EOwGroupCodes::kSnow:
      return toYadomsConditionsFromOwSnow(owConditionCode);
   case EOwGroupCodes::kAtmosphere:
      return toYadomsConditionsFromOwAtmosphere(owConditionCode);
   case EOwGroupCodes::kClearClouds:
      return toYadomsConditionsFromOwClearClouds(owConditionCode);
   default:
      throw std::out_of_range("OpenWeather condition code (weather[0].id");
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwThunderstorm(int owConditionCode)
{
   enum class EOwThunderstormCodes
   {
      kThunderstormWithLightRain = 200,
      kThunderstormWithRain = 201,
      kThunderstormWithHeavyRain = 202,
      kLightThunderstorm = 210,
      kThunderstorm = 211,
      kHeavyThunderstorm = 212,
      kRaggedThunderstorm = 221,
      kThunderstormWithLightDrizzle = 230,
      kThunderstormWithDrizzle = 231,
      kThunderstormWithHeavyDrizzle = 232,
   };

   switch (static_cast<EOwThunderstormCodes>(owConditionCode))
   {
   case EOwThunderstormCodes::kThunderstormWithLightRain:
   case EOwThunderstormCodes::kThunderstormWithRain:
   case EOwThunderstormCodes::kLightThunderstorm:
   case EOwThunderstormCodes::kThunderstorm:
   case EOwThunderstormCodes::kHeavyThunderstorm:
   case EOwThunderstormCodes::kRaggedThunderstorm:
   case EOwThunderstormCodes::kThunderstormWithLightDrizzle:
   case EOwThunderstormCodes::kThunderstormWithDrizzle:
      return yApi::historization::EWeatherCondition::kThunderstorm;
      
   case EOwThunderstormCodes::kThunderstormWithHeavyRain:
   case EOwThunderstormCodes::kThunderstormWithHeavyDrizzle:
      return yApi::historization::EWeatherCondition::kStormShowers;

   default:
      YADOMS_LOG(warning) << "Unknown Thunderstorm sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kThunderstorm;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwDrizzle(int owConditionCode)
{
   enum class EOwDrizzleCodes
   {
      kLightIntensityDrizzle = 300,
      kDrizzle = 301,
      kHeavyIntensityDrizzle = 302,
      kLightIntensityDrizzleRain = 310,
      kDrizzleRain = 311,
      kHeavyIntensityDrizzleRain = 312,
      kShowerRainAndDrizzle = 313,
      kHeavyShowerRainAndDrizzle = 314,
      kShowerDrizzle = 321
   };

   switch (static_cast<EOwDrizzleCodes>(owConditionCode))
   {
   case EOwDrizzleCodes::kLightIntensityDrizzle:
   case EOwDrizzleCodes::kDrizzle:
   case EOwDrizzleCodes::kHeavyIntensityDrizzle:
   case EOwDrizzleCodes::kLightIntensityDrizzleRain:
   case EOwDrizzleCodes::kDrizzleRain:
   case EOwDrizzleCodes::kHeavyIntensityDrizzleRain:
   case EOwDrizzleCodes::kShowerRainAndDrizzle:
   case EOwDrizzleCodes::kHeavyShowerRainAndDrizzle:
   case EOwDrizzleCodes::kShowerDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;

   default:
      YADOMS_LOG(warning) << "Unknown Drizzle sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kDrizzle;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwRain(int owConditionCode)
{
   enum class EOwRainCodes
   {
      kLightRain = 500,
      kModerateRain = 501,
      kHeavyIntensityRain = 502,
      kVeryHeavyRain = 503,
      kExtremeRain = 504,
      kFreezingRain = 511,
      kLightIntensityShowerRain = 520,
      kShowerRain = 521,
      kHeavyIntensityShowerRain = 522,
      kRaggedShowerRain = 531
   };

   switch (static_cast<EOwRainCodes>(owConditionCode))
   {
   case EOwRainCodes::kLightRain:
   case EOwRainCodes::kModerateRain:
   case EOwRainCodes::kHeavyIntensityRain:
   case EOwRainCodes::kVeryHeavyRain:
   case EOwRainCodes::kExtremeRain:
   case EOwRainCodes::kFreezingRain:
      return yApi::historization::EWeatherCondition::kRain;

   case EOwRainCodes::kLightIntensityShowerRain:
   case EOwRainCodes::kShowerRain:
   case EOwRainCodes::kHeavyIntensityShowerRain:
   case EOwRainCodes::kRaggedShowerRain:
      return yApi::historization::EWeatherCondition::kShowers;

   default:
      YADOMS_LOG(warning) << "Unknown Rain sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kRain;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwSnow(int owConditionCode)
{
   enum class EOwSnowCodes
   {
      kLightSnow = 600,
      kSnow = 601,
      kHeavySnow = 602,
      kSleet = 611,
      kLightShowersleet = 612,
      kShowerSleet = 613,
      kLightRainAndSnow = 615,
      kRainAndSnow = 616,
      kLightShowersnow = 620,
      kShowerSnow = 621,
      kHeavyShowerSnow = 622
   };

   switch (static_cast<EOwSnowCodes>(owConditionCode))
   {
   case EOwSnowCodes::kSleet:
      return yApi::historization::EWeatherCondition::kSleet;

   case EOwSnowCodes::kLightShowersleet:
   case EOwSnowCodes::kShowerSleet:
      return yApi::historization::EWeatherCondition::kSleetStorm;

   case EOwSnowCodes::kLightRainAndSnow:
   case EOwSnowCodes::kRainAndSnow:
      return yApi::historization::EWeatherCondition::kRainMix;

   case EOwSnowCodes::kLightSnow:
   case EOwSnowCodes::kSnow:
   case EOwSnowCodes::kHeavySnow:
   case EOwSnowCodes::kLightShowersnow:
   case EOwSnowCodes::kShowerSnow:
   case EOwSnowCodes::kHeavyShowerSnow:
      return yApi::historization::EWeatherCondition::kSnow;
   default:
      YADOMS_LOG(warning) << "Unknown Snow sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kSnow;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwAtmosphere(int owConditionCode)
{
   enum class EOwAtmosphereCodes
   {
      kMist = 701,
      kSmoke = 711,
      kHaze = 721,
      kSandDustWhirls = 731,
      kFog = 741,
      kSand = 751,
      kDust = 761,
      kVolcanicAsh = 762,
      kSqualls = 771,
      kTornado = 781,
   };

   switch (static_cast<EOwAtmosphereCodes>(owConditionCode))
   {
   case EOwAtmosphereCodes::kMist:
   case EOwAtmosphereCodes::kSmoke:
   case EOwAtmosphereCodes::kFog:
   case EOwAtmosphereCodes::kVolcanicAsh:
      return yApi::historization::EWeatherCondition::kFog;

   case EOwAtmosphereCodes::kHaze:
      return yApi::historization::EWeatherCondition::kHaze;

   case EOwAtmosphereCodes::kSqualls:
   case EOwAtmosphereCodes::kTornado:
   case EOwAtmosphereCodes::kSandDustWhirls:
   case EOwAtmosphereCodes::kSand:
   case EOwAtmosphereCodes::kDust:
      return yApi::historization::EWeatherCondition::kWindy;

   default:
      YADOMS_LOG(warning) << "Unknown Atmosphere sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kFog;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwClearClouds(int owConditionCode)
{
   enum class EOwAClearCloudsCodes
   {
      kClearSky = 800,
      kFewClouds = 801,
      kScatteredClouds = 802,
      kBrokenClouds = 803,
      kOvercastClouds = 804,
   };

   switch (static_cast<EOwAClearCloudsCodes>(owConditionCode))
   {
   case EOwAClearCloudsCodes::kClearSky:
      return yApi::historization::EWeatherCondition::kSunny;
   case EOwAClearCloudsCodes::kFewClouds:
   case EOwAClearCloudsCodes::kScatteredClouds:
   case EOwAClearCloudsCodes::kBrokenClouds:
   case EOwAClearCloudsCodes::kOvercastClouds:
      return yApi::historization::EWeatherCondition::kCloudy;
   default:
      YADOMS_LOG(warning) << "Unknown Clear/Clouds sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kSunny;
   }
}
