#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"

namespace pluginSystem
{

   CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& deviceName, const std::string& deviceType, const shared::CDataContainerSharedPtr& configuration)
      :m_deviceName(deviceName), m_deviceType(deviceType), m_configuration(configuration)
   {
   }
      
   CManuallyDeviceCreationData::~CManuallyDeviceCreationData() 
   {
   }

   const std::string& CManuallyDeviceCreationData::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::string& CManuallyDeviceCreationData::getDeviceType() const
   {
      return m_deviceType;
   }

   const shared::CDataContainerSharedPtr& CManuallyDeviceCreationData::getConfiguration() const
   {
      return m_configuration;
   }
  

} // namespace pluginSystem	
