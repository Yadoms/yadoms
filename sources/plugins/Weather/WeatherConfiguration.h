#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CWeatherConfiguration
{
public:
   virtual ~CWeatherConfiguration() = default;

   void initializeWith(const shared::CDataContainer& data);
   void trace() const;

   std::string apiKey() const;
   bool useSpecificLocation() const;
   double specificLocationLongitude() const;
   double specificLocationLatitude() const;

private:
   yApi::YPluginConfiguration m_configuration;
};
