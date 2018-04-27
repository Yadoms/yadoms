#include "stdafx.h"
#include "TeleInfoConfigurationMock.h"

TeleInfoConfigurationMock::TeleInfoConfigurationMock()
{}

void TeleInfoConfigurationMock::initializeWith(const shared::CDataContainer& data)
{}

std::string TeleInfoConfigurationMock::getSerialPort() const
{
  return "/dev/ttyUSB0";
}

EEquipmentType TeleInfoConfigurationMock::getEquipmentType() const
{
   return OneInput;
}

EInputsActivated TeleInfoConfigurationMock::getInputsActivated() const
{
   return Input1Activated;
}