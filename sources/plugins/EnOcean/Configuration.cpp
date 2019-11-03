#include "stdafx.h"
#include "Configuration.h"

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

CPairingHelper::EPairingMode CConfiguration::getPairingMode() const
{
   try
   {
      return m_data.get<std::string>("PairingMode") == "manual" ? CPairingHelper::kManual : CPairingHelper::kAuto;
   }
   catch (shared::exception::CInvalidParameter&)
   {
      return CPairingHelper::kAuto;
   }
}

