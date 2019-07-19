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
            liveWeatherDevice.setCondition(toYadomsCondition(weatherData.get<std::vector<shared::CDataContainer>>("weather")[0].get<int>("id")));
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

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsCondition(int owConditionCode)
{
   // See https://openweathermap.org/weather-conditions

   enum EOwGroupCodes
   {
      kThunderstorm = 2,
      kDrizzle = 3,
      kRain = 5,
      kSnow = 6,
      kAtmosphere = 7,
      kClearClouds = 8
   };

   const auto group = owConditionCode / 100;
   switch (group)
   {
   case kThunderstorm:
      return toYadomsConditionsFromOwThunderstorm(owConditionCode);
   case kDrizzle:
      return toYadomsConditionsFromOwDrizzle(owConditionCode);
   case kRain:
      return toYadomsConditionsFromOwRain(owConditionCode);
   case kSnow:
      return toYadomsConditionsFromOwSnow(owConditionCode);
   case kAtmosphere:
      return toYadomsConditionsFromOwAtmosphere(owConditionCode);
   case kClearClouds:
      return toYadomsConditionsFromOwClearClouds(owConditionCode);
   default:
      throw std::out_of_range("OpenWeather condition code (weather[0].id");
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwThunderstorm(int owConditionCode)
{
   enum EOwThunderstormCodes
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

   switch (owConditionCode)
   {
   case kThunderstormWithLightRain:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstormWithRain:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstormWithHeavyRain:
      return yApi::historization::EWeatherCondition::kStormShowers;
   case kLightThunderstorm:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstorm:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kHeavyThunderstorm:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kRaggedThunderstorm:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstormWithLightDrizzle:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstormWithDrizzle:
      return yApi::historization::EWeatherCondition::kThunderstorm;
   case kThunderstormWithHeavyDrizzle:
      return yApi::historization::EWeatherCondition::kStormShowers;
   default:
      YADOMS_LOG(warning) << "Unknown Thunderstorm sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kThunderstorm;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwDrizzle(int owConditionCode)
{
   enum EOwDrizzleCodes
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

   switch (owConditionCode)
   {
   case kLightIntensityDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kHeavyIntensityDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kLightIntensityDrizzleRain:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kDrizzleRain:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kHeavyIntensityDrizzleRain:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kShowerRainAndDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kHeavyShowerRainAndDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   case kShowerDrizzle:
      return yApi::historization::EWeatherCondition::kDrizzle;
   default:
      YADOMS_LOG(warning) << "Unknown Drizzle sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kDrizzle;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwRain(int owConditionCode)
{
   enum EOwRainCodes
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

   switch (owConditionCode)
   {
   case kLightRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kModerateRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kHeavyIntensityRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kVeryHeavyRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kExtremeRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kFreezingRain:
      return yApi::historization::EWeatherCondition::kRain;
   case kLightIntensityShowerRain:
      return yApi::historization::EWeatherCondition::kShowers;
   case kShowerRain:
      return yApi::historization::EWeatherCondition::kShowers;
   case kHeavyIntensityShowerRain:
      return yApi::historization::EWeatherCondition::kShowers;
   case kRaggedShowerRain:
      return yApi::historization::EWeatherCondition::kShowers;
   default:
      YADOMS_LOG(warning) << "Unknown Rain sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kRain;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwSnow(int owConditionCode)
{
   enum EOwSnowCodes
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

   switch (owConditionCode)
   {
   case kLightSnow:
      return yApi::historization::EWeatherCondition::kSnow;
   case kSnow:
      return yApi::historization::EWeatherCondition::kSnow;
   case kHeavySnow:
      return yApi::historization::EWeatherCondition::kSnow;
   case kSleet:
      return yApi::historization::EWeatherCondition::kSleet;
   case kLightShowersleet:
      return yApi::historization::EWeatherCondition::kSleetStorm;
   case kShowerSleet:
      return yApi::historization::EWeatherCondition::kSleetStorm;
   case kLightRainAndSnow:
      return yApi::historization::EWeatherCondition::kRainMix;
   case kRainAndSnow:
      return yApi::historization::EWeatherCondition::kRainMix;
   case kLightShowersnow:
      return yApi::historization::EWeatherCondition::kSnow;
   case kShowerSnow:
      return yApi::historization::EWeatherCondition::kSnow;
   case kHeavyShowerSnow:
      return yApi::historization::EWeatherCondition::kSnow;
   default:
      YADOMS_LOG(warning) << "Unknown Snow sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kSnow;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwAtmosphere(int owConditionCode)
{
   enum EOwAtmosphereCodes
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

   switch (owConditionCode)
   {
   case kMist:
      return yApi::historization::EWeatherCondition::kFog;
   case kSmoke:
      return yApi::historization::EWeatherCondition::kFog;
   case kHaze:
      return yApi::historization::EWeatherCondition::kHaze;
   case kSandDustWhirls:
      return yApi::historization::EWeatherCondition::kWindy;
   case kFog:
      return yApi::historization::EWeatherCondition::kFog;
   case kSand:
      return yApi::historization::EWeatherCondition::kWindy;
   case kDust:
      return yApi::historization::EWeatherCondition::kWindy;
   case kVolcanicAsh:
      return yApi::historization::EWeatherCondition::kFog;
   case kSqualls:
      return yApi::historization::EWeatherCondition::kWindy;
   case kTornado:
      return yApi::historization::EWeatherCondition::kWindy;
   default:
      YADOMS_LOG(warning) << "Unknown Atmosphere sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kFog;
   }
}

yApi::historization::EWeatherCondition COpenWeatherService::toYadomsConditionsFromOwClearClouds(int owConditionCode)
{
   enum EOwAClearCloudsCodes
   {
      kClearSky = 800,
      kFewClouds = 801,
      kScatteredClouds = 802,
      kBrokenClouds = 803,
      kOvercastClouds = 804,
   };

   switch (owConditionCode)
   {
   case kClearSky:
      return yApi::historization::EWeatherCondition::kSunny;
   case kFewClouds:
      return yApi::historization::EWeatherCondition::kCloudy;
   case kScatteredClouds:
      return yApi::historization::EWeatherCondition::kCloudy;
   case kBrokenClouds:
      return yApi::historization::EWeatherCondition::kCloudy;
   case kOvercastClouds:
      return yApi::historization::EWeatherCondition::kCloudy;
   default:
      YADOMS_LOG(warning) << "Unknown Clear/Clouds sub-code " << owConditionCode;
      return yApi::historization::EWeatherCondition::kSunny;
   }
}
