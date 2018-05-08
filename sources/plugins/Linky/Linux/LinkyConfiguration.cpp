#include "stdafx.h"
#include "LinkyConfiguration.h"

CLinkyConfiguration::CLinkyConfiguration()
{
}

CLinkyConfiguration::~CLinkyConfiguration()
{
}

void CLinkyConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CLinkyConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

EEquipmentType CLinkyConfiguration::getEquipmentType() const
{
   // For Linux/Raspberry, only one input
   return OneInput;
}

EInputsActivated CLinkyConfiguration::getInputsActivated() const
{
   // For Linux/Raspberry only the first input is activated at this time
   return Input1Activated;
}