#include "stdafx.h"
#include "TeleInfoConfiguration.h"

CTeleInfoConfiguration::CTeleInfoConfiguration()
{
}

CTeleInfoConfiguration::~CTeleInfoConfiguration()
{
}

void CTeleInfoConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CTeleInfoConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

EEquipmentType CTeleInfoConfiguration::getEquipmentType() const
{
   std::string temp = m_data.get<std::string>("EquipmentType");
   if (temp == "TIC1xInput")
      return OneInput;
   else if (temp == "TIC2xInputs")
      return TwoInputs;
}