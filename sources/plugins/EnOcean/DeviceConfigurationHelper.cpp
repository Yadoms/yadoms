#include "stdafx.h"
#include "DeviceConfigurationHelper.h"


CDeviceConfigurationHelper::CDeviceConfigurationHelper(const CProfileHelper& profile,
                                                       const std::string& manufacturerName)
   :m_deviceConfiguration(new_CDataContainerSharedPtr())
{
   m_deviceConfiguration->set("manufacturer", manufacturerName);
   m_deviceConfiguration->set("profile.activeSection", profile.profile());
}

CDeviceConfigurationHelper::~CDeviceConfigurationHelper()
{
}


const shared::CDataContainerSharedPtr& CDeviceConfigurationHelper::configuration() const
{
   return m_deviceConfiguration;
}
