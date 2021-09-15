#include "stdafx.h"
#include "DeviceCommand.h"

#include <utility>

namespace pluginSystem
{
   CDeviceCommand::CDeviceCommand(std::string targetDevice,
                                  const boost::shared_ptr<const database::entities::CKeyword>& keyword,
                                  const std::string& body)
      : m_device(std::move(targetDevice)),
        m_keyword(keyword),
        m_body(body),
        m_historizableObject(keyword, body)
   {
   }

   const std::string& CDeviceCommand::getDevice() const
   {
      return m_device;
   }

   const std::string& CDeviceCommand::getKeyword() const
   {
      return m_keyword->Name();
   }

   const std::string& CDeviceCommand::getBody() const
   {
      return m_body;
   }

   const shared::plugin::yPluginApi::historization::IHistorizable& CDeviceCommand::getHistorizableObject() const
   {
      return m_historizableObject;
   }
} // namespace pluginSystem	


