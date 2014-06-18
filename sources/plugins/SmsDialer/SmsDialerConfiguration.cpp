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

bool CSmsDialerConfiguration::hasPINCode() const
{
   return getValue<bool>("PINCode.checkbox");
}

std::string CSmsDialerConfiguration::getPhonePIN() const
{
   return hasPINCode() ? getValue<std::string>("PINCode.values.PIN") : shared::CStringExtension::EmptyString;
}

