#include "stdafx.h"
#include "TeleInfoConfigurationMock.h"

TeleInfoConfigurationMock::TeleInfoConfigurationMock()
{}

void TeleInfoConfigurationMock::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{}

std::string TeleInfoConfigurationMock::getSerialPort() const
{
  return "\\\\.\\COM20";
}

EEquipmentType TeleInfoConfigurationMock::getEquipmentType() const
{
   return OneInput;
}

EInputsActivated TeleInfoConfigurationMock::getInputsActivated() const
{
   return Input1Activated;
}