#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"
#include <shared/Log.h>

namespace pluginSystem
{

   CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& deviceName, const shared::CDataContainer & configuration)
      :m_deviceName(deviceName), m_configuration(configuration)
   {
         m_deviceType = configuration.get<std::string>("type");
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

   const shared::CDataContainer& CManuallyDeviceCreationData::getConfiguration() const
   {
      return m_configuration;
   }
  

} // namespace pluginSystem	
