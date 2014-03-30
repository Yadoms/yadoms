#include "stdafx.h"
#include "SmsDialerConfiguration.h"

namespace cfg = shared::plugin::configuration;


CSmsDialerConfiguration::~CSmsDialerConfiguration()
{
}

std::string CSmsDialerConfiguration::getGammuPort() const
{
   return getValue<std::string>("Serial Port");
}

std::string CSmsDialerConfiguration::getGammuProtocol() const
{
   return getValue<std::string>("Protocol");
}

