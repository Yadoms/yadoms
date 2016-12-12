#include "stdafx.h"
#include "Profile_D2_01_0F.h"

CProfile_D2_01_0F::CProfile_D2_01_0F(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : CProfile_D2_01_0D(deviceId,
                       api)
{
}

CProfile_D2_01_0F::~CProfile_D2_01_0F()
{
}

const std::string& CProfile_D2_01_0F::profile() const
{
   static const std::string profile("D2-01-0F");
   return profile;
}

const std::string& CProfile_D2_01_0F::title() const
{
   static const std::string title("Slot-in module with 1 channel, no metering capabilities");
   return title;
}
