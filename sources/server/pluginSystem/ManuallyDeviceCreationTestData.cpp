#include "stdafx.h"
#include "ManuallyDeviceCreationTestData.h"

namespace pluginSystem
{

CManuallyDeviceCreationTestData::CManuallyDeviceCreationTestData(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command, const std::string& parameters)
   :m_command(command),
   m_parameters(parameters)
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

std::string CManuallyDeviceCreationTestData::toString() const
{
   std::stringstream str;
   str << shared::plugin::yPluginApi::IDeviceCommand::toString(m_command);
   return str.str();
}

} // namespace pluginSystem	
