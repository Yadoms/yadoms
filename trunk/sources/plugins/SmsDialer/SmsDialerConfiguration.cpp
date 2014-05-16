#include "stdafx.h"
#include "SmsDialerConfiguration.h"
#include <shared/StringExtension.h>

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

std::string CSmsDialerConfiguration::getPhonePIN() const
{
   return hasValue("PINCode.PIN") ? getValue<std::string>("PINCode.PIN") : shared::CStringExtension::EmptyString;
}

