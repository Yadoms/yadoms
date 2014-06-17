#include "stdafx.h"
#include "RfxLanXplConfiguration.h"
#include <shared/Log.h>

namespace cfg = shared::plugin::configuration;


CRfxLanXplConfiguration::~CRfxLanXplConfiguration()
{
}



bool CRfxLanXplConfiguration::getStartXplhub() const
{
   return getValue<bool>("HubXplSection.checkbox");
}

std::string CRfxLanXplConfiguration::getHubLocalIp() const
{
   return getValue<std::string>("HubXplSection.values.ipAddress");
}
