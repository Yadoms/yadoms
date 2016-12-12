#include "stdafx.h"
#include "Profile_D2_01_0E.h"

CProfile_D2_01_0E::CProfile_D2_01_0E(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : CProfile_D2_01_0D(deviceId,
                       api)
{
}

CProfile_D2_01_0E::~CProfile_D2_01_0E()
{
}

const std::string& CProfile_D2_01_0E::profile() const
{
   static const std::string profile("D2-01-0E");
   return profile;
}

const std::string& CProfile_D2_01_0E::title() const
{
   static const std::string title("Micro smart plug with 1 channel, and metering capabilities");
   return title;
}
