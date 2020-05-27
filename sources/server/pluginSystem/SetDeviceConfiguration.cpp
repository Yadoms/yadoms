#include "stdafx.h"
#include "SetDeviceConfiguration.h"

namespace pluginSystem
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const std::string& device, const boost::shared_ptr<shared::CDataContainer>& configuration)
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

   const boost::shared_ptr<shared::CDataContainer>& CSetDeviceConfiguration::configuration() const
   {
      return m_configuration;
   }
} // namespace pluginSystem	


