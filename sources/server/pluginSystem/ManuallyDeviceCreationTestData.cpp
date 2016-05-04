#include "stdafx.h"
#include "ManuallyDeviceCreationTestData.h"

namespace pluginSystem
{

CManuallyDeviceCreationTestData::CManuallyDeviceCreationTestData(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command, const std::string& parameters)
   :m_command(command), m_parameters(parameters)
{
}
      
CManuallyDeviceCreationTestData::~CManuallyDeviceCreationTestData() 
{
}

boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> CManuallyDeviceCreationTestData::getCommand() const
{
   return m_command;
}

const std::string& CManuallyDeviceCreationTestData::getDeviceParameters() const
{
   return m_parameters;
}

const std::string CManuallyDeviceCreationTestData::toString() const
{
   std::stringstream str;
   str << m_command->getDevice() << "." << m_command->getKeyword() << " = " << m_command->getBody() << "(" << m_parameters << ")";
   return str.str();
}

} // namespace pluginSystem	
