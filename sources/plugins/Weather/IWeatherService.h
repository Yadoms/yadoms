#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class IWeatherService
{
public:
   virtual ~IWeatherService() = default;

   virtual boost::posix_time::time_duration serviceRecommendedRetryDelay() const = 0;
   virtual void declareDevices() const = 0;
   virtual void requestWeather(boost::shared_ptr<const shared::ILocation> forLocation) const = 0;
};
