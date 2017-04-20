#include "stdafx.h"
#include "ManuallyDeviceCreationData.h"


namespace plugin_cpp_api
{
   CManuallyDeviceCreationData::CManuallyDeviceCreationData(const std::string& deviceName, const std::string& deviceType, const std::string& configuration)
      : m_deviceName(deviceName), m_deviceType(deviceType), m_configuration(shared::CDataContainer(configuration))
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

   const shared::CDataContainer& CManuallyDeviceCreationData::getConfiguration() const
   {
      return m_configuration;
   }

} // namespace plugin_cpp_api	


