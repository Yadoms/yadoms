#include "stdafx.h"
#include "SetDeviceConfiguration.h"


namespace plugin_cpp_api
{
   CSetDeviceConfiguration::CSetDeviceConfiguration(const plugin_IPC::toPlugin::SetDeviceConfiguration& msg)
      : m_device(msg.device())
   {
      shared::CDataContainer t(msg.configuration());
      m_deviceType = t.get<std::string>("type");
      m_configuration = t.get<shared::CDataContainer>("configuration");
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
} // namespace plugin_cpp_api	


