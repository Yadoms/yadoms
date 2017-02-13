#include "stdafx.h"
#include "DeviceCommand.h"


namespace plugin_cpp_api
{
   CDeviceCommand::CDeviceCommand(const plugin_IPC::toPlugin::DeviceCommand& msg)
      : m_device(msg.device()),
        m_keyword(msg.keyword()),
        m_body(msg.body())
   {
   }

   CDeviceCommand::~CDeviceCommand()
   {
   }

   const std::string& CDeviceCommand::getDevice() const
   {
      return m_device;
   }

   const std::string& CDeviceCommand::getKeyword() const
   {
      return m_keyword;
   }

   const std::string& CDeviceCommand::getBody() const
   {
      return m_body;
   }
} // namespace plugin_cpp_api	


