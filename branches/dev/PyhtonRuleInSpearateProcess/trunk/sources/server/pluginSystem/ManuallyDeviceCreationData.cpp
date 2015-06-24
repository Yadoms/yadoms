#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"

namespace pluginSystem
{

   CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& deviceName, const shared::CDataContainer & configuration)
      :m_deviceName(deviceName), m_configuration(configuration)
   {
   }
      
   CManuallyDeviceCreationData::~CManuallyDeviceCreationData() 
   {
   }

   const std::string& CManuallyDeviceCreationData::getDeviceName() const
   {
      return m_deviceName;
   }

   const shared::CDataContainer &  CManuallyDeviceCreationData::getConfiguration() const
   {
      return m_configuration;
   }
  

} // namespace pluginSystem	
