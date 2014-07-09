#include "stdafx.h"
#include "DeviceCommand.h"

namespace pluginSystem
{

   CDeviceCommand::CDeviceCommand(const std::string& targetDevice, const std::string& keyword, const shared::CDataContainer& body)
   :m_targetDevice(targetDevice), m_keyword(keyword), m_body(body)
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

const shared::CDataContainer& CDeviceCommand::getBody() const
{
   return m_body;
}

const std::string CDeviceCommand::toString() const
{
   std::stringstream str;
   str << m_targetDevice << " = " << m_body.serialize();
   return str.str();
}

} // namespace pluginSystem	
