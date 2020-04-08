#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CWebConnectionQualityConfiguration
{
public:
   virtual ~CWebConnectionQualityConfiguration();

   void initializeWith(const shared::CDataContainerSharedPtr& data);
   void trace() const;

   unsigned int periodicityInMinutes() const;

private:
   yApi::YPluginConfiguration m_configuration;
};

