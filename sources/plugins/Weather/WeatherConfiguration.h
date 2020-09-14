#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>
#include <shared/ILocation.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CWeatherConfiguration
{
public:
   virtual ~CWeatherConfiguration() = default;

   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data);
   void trace() const;

   std::string apiKey() const;
   bool useSpecificLocation() const;
   boost::shared_ptr<const shared::ILocation> specificLocation() const;

private:
   yApi::YPluginConfiguration m_configuration;
};
