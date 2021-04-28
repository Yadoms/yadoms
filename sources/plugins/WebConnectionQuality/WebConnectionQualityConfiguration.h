#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CWebConnectionQualityConfiguration final
{
public:
   ~CWebConnectionQualityConfiguration() = default;

   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data);
   void trace() const;

   unsigned int periodicityInMinutes() const;

private:
   yApi::YPluginConfiguration m_configuration;
};
