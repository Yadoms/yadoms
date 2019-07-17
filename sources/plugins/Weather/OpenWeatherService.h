#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IWeatherService.h"
#include "LiveWeatherDevice.h"

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
   shared::CDataContainer syncRequest(const std::string& url) const;
   void processAnswer(const shared::CDataContainer& weatherData) const;

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_apiKey;
   static const std::string LiveWeatherDeviceName;
   boost::shared_ptr<CLiveWeatherDevice> m_liveWeatherDevice;
};
