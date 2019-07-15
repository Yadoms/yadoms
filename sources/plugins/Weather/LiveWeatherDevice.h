#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CLiveWeatherDevice
{
public:
   explicit CLiveWeatherDevice(const std::string& deviceName);
   virtual ~CLiveWeatherDevice() = default;

   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;


private:
   const std::string m_deviceName;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
};
