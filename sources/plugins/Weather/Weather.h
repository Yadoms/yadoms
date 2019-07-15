#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WeatherConfiguration.h"

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
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;
   void startMeasure(boost::shared_ptr<yApi::IYPluginApi> api);
   void processResult(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& result) const;

private:
   static const boost::posix_time::time_duration RequestPeriodicity;

   CWeatherConfiguration m_configuration;
   boost::shared_ptr<yApi::historization::CSwitch> m_connectedKw;
   boost::shared_ptr<yApi::historization::CDuration> m_pingKw;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
};
