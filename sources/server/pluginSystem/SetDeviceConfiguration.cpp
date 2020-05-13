#include "stdafx.h"
#include "SetDeviceConfiguration.h"

namespace pluginSystem
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const std::string& device, const shared::CDataContainerSharedPtr& configuration)
      : m_device(device), m_configuration(configuration)
   {
   }

   CSetDeviceConfiguration::~CSetDeviceConfiguration()
   {
   }

   const std::string& CSetDeviceConfiguration::name() const
   {
      return m_device;
   }

   const shared::CDataContainerSharedPtr& CSetDeviceConfiguration::configuration() const
   {
      return m_configuration;
   }
} // namespace pluginSystem	


