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
   CWeather();
   virtual ~CWeather() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   void declareDevices() const;
   void requestWeather(boost::shared_ptr<yApi::IYPluginApi> api);
   boost::shared_ptr<const shared::ILocation> getLocation(boost::shared_ptr<yApi::IYPluginApi> api) const;

private:
   static const boost::posix_time::time_duration RequestPeriodicity;
   static const boost::posix_time::time_duration RetryDelay;
   boost::shared_ptr<shared::event::CEventTimer> m_requestTimer;
   
   CWeatherConfiguration m_configuration;
   boost::shared_ptr<IWeatherService> m_weatherService;
   int m_requestTries;
};
