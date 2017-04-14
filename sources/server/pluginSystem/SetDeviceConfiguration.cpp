#include "stdafx.h"
#include "SetDeviceConfiguration.h"

namespace pluginSystem
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const std::string& device,
                                                    const shared::CDataContainer& configuration)
      : m_device(device),
        m_deviceType(configuration.get<std::string>("type")),
        m_configuration(configuration)
   {
   }

   CSetDeviceConfiguration::~CSetDeviceConfiguration()
   {
   }

   const std::string& CSetDeviceConfiguration::name() const
   {
      return m_device;
   }

   const std::string& CSetDeviceConfiguration::type() const
   {
      return m_deviceType;
   }

   const shared::CDataContainer& CSetDeviceConfiguration::configuration() const
   {
      return m_configuration;
   }
} // namespace pluginSystem	


