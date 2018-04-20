#include "stdafx.h"
#include "TeleInfoConfiguration.h"
#include <shared/Log.h>

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
   try {
      std::string temp = m_data.get<std::string>("EquipmentType.activeSectionText");
      if (temp == "1xTIC")
         return OneInput;
      else
         return TwoInputs;
   }
   catch (std::exception &e)
   {
      // By default ! To be compatible with v1.0.x
      YADOMS_LOG(trace) << e.what();
      YADOMS_LOG(information) << "1xTIC used by default";
      return OneInput;
   }
}

EInputsActivated CTeleInfoConfiguration::getInputsActivated() const
{
   return Input1Activated;
}