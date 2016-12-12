#include "stdafx.h"
#include "DeviceConfigurationHelper.h"


CDeviceConfigurationHelper::CDeviceConfigurationHelper(const CProfileHelper& profile,
                                                       const std::string& manufacturerName)
{
   shared::CDataContainer profileNode;
   profileNode.set("content", "");
   profileNode.set("radio", "true");

   shared::CDataContainer contentProfilesNode;
   contentProfilesNode.set(profile.profile(), profileNode);

   shared::CDataContainer profilesNode;
   profilesNode.set("content", contentProfilesNode);
   profilesNode.set("activeSection", profile.profile());

   m_deviceConfiguration.set("manufacturer", manufacturerName);
   m_deviceConfiguration.set("profile", profilesNode);
}

CDeviceConfigurationHelper::~CDeviceConfigurationHelper()
{
}


const shared::CDataContainer& CDeviceConfigurationHelper::configuration() const
{
   return m_deviceConfiguration;
}
