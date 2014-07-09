#include "stdafx.h"
#include "RfxLanXplConfiguration.h"
#include <shared/Log.h>


CRfxLanXplConfiguration::~CRfxLanXplConfiguration()
{
}



bool CRfxLanXplConfiguration::getStartXplhub() const
{
   return get<bool>("HubXplSection.checkbox");
}

std::string CRfxLanXplConfiguration::getHubLocalIp() const
{
   return get<std::string>("HubXplSection.content.ipAddress");
}
