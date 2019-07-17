#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WeatherConfiguration.h"
#include "LiveWeatherDevice.h"
#include "IWeatherService.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

class CWeather : public plugin_cpp_api::IPlugin
{
public:
   CWeather() = default;
   virtual ~CWeather() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   void declareDevices() const;
   void requestWeather(boost::shared_ptr<yApi::IYPluginApi> api) const;
   boost::shared_ptr<const shared::ILocation> getLocation(boost::shared_ptr<yApi::IYPluginApi> api) const;

private:
   static const boost::posix_time::time_duration RequestPeriodicity;
   static const std::string LiveWeatherDeviceName;
   static const std::string ForecastWeatherDevicePrefix;
   static const int NbForecastDays;

   CWeatherConfiguration m_configuration;
   boost::shared_ptr<IWeatherService> m_weatherService;
   //TODO std::vector<ForcastPerDayKeywords> forcastPerDayKeywords;
   //TODO virer std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_forcastPerDayKeywords;
};
