#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"

namespace pluginSystem
{

CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& device, const std::string& keyword, const std::string& capacity, const std::string& parameters)
   :m_device(device), m_keyword(keyword), m_parameters(parameters)
{
}
      
CManuallyDeviceCreationData::~CManuallyDeviceCreationData() 
{
}

const std::string& CManuallyDeviceCreationData::getDevice() const
{
   return m_device;
}

const std::string& CManuallyDeviceCreationData::getKeyword() const
{
   return m_keyword;
}

const std::string& CManuallyDeviceCreationData::getCapcity() const
{
   return m_capacity;
}

const std::string& CManuallyDeviceCreationData::getParameters() const
{
   return m_parameters;
}

const std::string CManuallyDeviceCreationData::toString() const
{
   std::stringstream str;
   str << m_device << " = " << m_parameters;
   return str.str();
}

} // namespace pluginSystem	
