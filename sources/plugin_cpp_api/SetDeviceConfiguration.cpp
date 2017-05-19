#include "stdafx.h"
#include "SetDeviceConfiguration.h"


namespace plugin_cpp_api
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const plugin_IPC::toPlugin::SetDeviceConfiguration& msg)
      : m_device(msg.device()), m_configuration(msg.configuration())
   {
   }

   CSetDeviceConfiguration::~CSetDeviceConfiguration()
   {
   }

   const std::string& CSetDeviceConfiguration::name() const
   {
      return m_device;
   }

   const shared::CDataContainer& CSetDeviceConfiguration::configuration() const
   {
      return m_configuration;
   }
} // namespace plugin_cpp_api	


