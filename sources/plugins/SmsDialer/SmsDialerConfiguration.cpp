#include "stdafx.h"
#include "SmsDialerConfiguration.h"
#include <shared/encryption/Xor.h>


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
   if (!hasPINCode())
      return std::string();

   // Uncrypt PIN code
   return shared::encryption::CXor::decryptBase64(m_data.get<std::string>("PINCode.content.PIN"));
}

