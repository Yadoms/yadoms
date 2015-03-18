#include "stdafx.h"
#include "DeviceCommand.h"

namespace pluginSystem
{

CDeviceCommand::CDeviceCommand(const std::string& targetDevice, const std::string& keyword, const std::string& body)
   :m_targetDevice(targetDevice), m_keyword(keyword), m_body(body), m_historizableObject(keyword, body)
{
}
      
CDeviceCommand::~CDeviceCommand() 
{
}

const std::string& CDeviceCommand::getTargetDevice() const
{
   return m_targetDevice;
}

const std::string& CDeviceCommand::getKeyword() const
{
   return m_keyword;
}

const std::string& CDeviceCommand::getBody() const
{
   return m_body;
}

const std::string CDeviceCommand::toString() const
{
   std::stringstream str;
   str << m_targetDevice << " = " << m_body;
   return str.str();
}

const shared::plugin::yPluginApi::historization::IHistorizable& CDeviceCommand::getHistorizableObject() const
{
   return m_historizableObject;
}

shared::CDataContainer CDeviceCommand::getTypeInfo() const
{
   return shared::CDataContainer();
}

} // namespace pluginSystem	
