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
   std::string getForecastWeatherDeviceNameForDay(unsigned int forecastDay) const;
   std::string getForecastWeatherDeviceNameForHour(unsigned int forecastHour) const;
   static boost::shared_ptr<shared::CDataContainer> syncRequest(const std::string& url);
   void requestLiveWeather(boost::shared_ptr<const shared::ILocation> forLocation) const;
   void processLiveWeatherAnswer(const boost::shared_ptr<shared::CDataContainer>& weatherData,
                                 const boost::shared_ptr<shared::CDataContainer>& uvIndexData) const;
   void requestForecastWeather(boost::shared_ptr<const shared::ILocation> forLocation) const;
   void historize3HoursForecast(unsigned int hourIndex,
                                const boost::posix_time::ptime& forecastDatetime,
                                const boost::shared_ptr<shared::CDataContainer>& forecast) const;
   void historizeDaysForecast(const std::map<int, std::vector<boost::shared_ptr<shared::CDataContainer>>>& forecastDataByDay,
                              const std::map<int, double>& uvIndexByDay) const;
   void processForecastWeatherAnswer(const boost::shared_ptr<shared::CDataContainer>& weatherData,
                                     const boost::shared_ptr<shared::CDataContainer>& uvIndexData) const;
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
