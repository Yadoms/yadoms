#pragma once
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>
#include "shared/ILocation.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CWeatherLocation : public shared::ILocation
{
public:
   CWeatherLocation(double latitude,
                    double longitude);
   virtual ~CWeatherLocation() = default;
   double latitude() const override;
   double longitude() const override;
   double altitude() const override;
private:
   double m_latitude;
   double m_longitude;
};
