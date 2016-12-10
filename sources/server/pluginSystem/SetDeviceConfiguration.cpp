#include "stdafx.h"
#include "SetDeviceConfiguration.h"

namespace pluginSystem
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const std::string& device,
                                                    const shared::CDataContainer& configuration)
      : m_device(device),
        m_configuration(configuration)
   {
   }

   CSetDeviceConfiguration::~CSetDeviceConfiguration()
   {
   }

   const std::string& CSetDeviceConfiguration::device() const
   {
      return m_device;
   }

   const shared::CDataContainer& CSetDeviceConfiguration::configuration() const
   {
      return m_configuration;
   }
} // namespace pluginSystem	


