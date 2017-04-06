#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"

namespace pluginSystem
{

   CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& deviceName, const std::string& deviceModel, const shared::CDataContainer & configuration)
      :m_deviceName(deviceName), m_deviceModel(deviceModel), m_configuration(configuration)
   {
   }
      
   CManuallyDeviceCreationData::~CManuallyDeviceCreationData() 
   {
   }

   const std::string& CManuallyDeviceCreationData::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::string& CManuallyDeviceCreationData::getDeviceModel() const
   {
      return m_deviceModel;
   }

   const shared::CDataContainer &  CManuallyDeviceCreationData::getConfiguration() const
   {
      return m_configuration;
   }
  

} // namespace pluginSystem	
