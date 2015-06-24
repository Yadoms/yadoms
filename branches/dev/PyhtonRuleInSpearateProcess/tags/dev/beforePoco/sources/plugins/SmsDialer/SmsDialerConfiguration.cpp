#include "stdafx.h"
#include "SmsDialerConfiguration.h"
#include <shared/StringExtension.h>


CSmsDialerConfiguration::~CSmsDialerConfiguration()
{
}

void CSmsDialerConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CSmsDialerConfiguration::getGammuPort() const
{
   return m_data.get<std::string>("Serial Port");
}

std::string CSmsDialerConfiguration::getGammuProtocol() const
{
   return m_data.get<std::string>("Protocol");
}

bool CSmsDialerConfiguration::hasPINCode() const
{
   return m_data.get<bool>("PINCode.checkbox");
}

std::string CSmsDialerConfiguration::getPhonePIN() const
{
   return hasPINCode() ? m_data.get<std::string>("PINCode.content.PIN") : shared::CStringExtension::EmptyString;
}

