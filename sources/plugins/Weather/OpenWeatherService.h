#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IWeatherService.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class COpenWeatherService : public IWeatherService
{
public:
   COpenWeatherService(boost::shared_ptr<yApi::IYPluginApi> api,
                       std::string apiKey);
   virtual ~COpenWeatherService() = default;

   // IWeatherService implementation
   boost::posix_time::time_duration serviceRecommendedRetryDelay() const override;
   void declareDevices() const override;
   void requestWeather(boost::shared_ptr<const shared::ILocation> forLocation) const override;
   // [END] IWeatherService implementation

private:
   std::string getForecastWeatherDeviceNameForDay(int forecastDay) const;
   std::string getForecastWeatherDeviceNameForHour(int forecastHour) const;
   shared::CDataContainer syncRequest(const std::string& url) const;
   void requestLiveWeather(boost::shared_ptr<const shared::ILocation> forLocation) const;
   void processLiveWeatherAnswer(const shared::CDataContainer& weatherData) const;
   void requestForecastWeather(boost::shared_ptr<const shared::ILocation> forLocation) const;
   void historize3HoursForecast(int hourIndex,
                                const boost::posix_time::ptime& forecastDatetime,
                                const shared::CDataContainer& forecast) const;
   void historizeDaysForecast(const std::map<int, std::vector<shared::CDataContainer>>& forecastDataByDay) const;
   void processForecastWeatherAnswer(const shared::CDataContainer& weatherData) const;
   static yApi::historization::EWeatherCondition toYadomsCondition(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwThunderstorm(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwDrizzle(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwRain(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwSnow(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwAtmosphere(int owConditionCode);
   static yApi::historization::EWeatherCondition toYadomsConditionsFromOwClearClouds(int owConditionCode);

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_apiKey;
   static const std::string ServiceName;
   static const unsigned int NbForecastHours;
   static const unsigned int NbForecastDays;
   static const std::string LiveWeatherDeviceName;
   static const std::string ForecastWeatherPerDayDevicePrefix;
   static const std::string ForecastWeatherPerHourDevicePrefix;
};
