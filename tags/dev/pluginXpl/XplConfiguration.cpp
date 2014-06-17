#include "stdafx.h"
#include "XplConfiguration.h"
#include <shared/Log.h>

namespace cfg = shared::plugin::configuration;


CXplConfiguration::~CXplConfiguration()
{
}



bool CXplConfiguration::getStartXplhub() const
{
   return getValue<bool>("HubXplSection.checkbox");
}

std::string CXplConfiguration::getHubLocalIp() const
{
   return getValue<std::string>("HubXplSection.values.ipAddress");
}
