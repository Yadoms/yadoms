#include "stdafx.h"
#include "LinkyConfiguration.h"
#include <shared/Log.h>

CLinkyConfiguration::CLinkyConfiguration()
{
}

CLinkyConfiguration::~CLinkyConfiguration()
{
}

void CLinkyConfiguration::initializeWith(const shared::CDataContainerSharedPtr& data)
{
   m_data.initializeWith(data);
}

std::string CLinkyConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

EEquipmentType CLinkyConfiguration::getEquipmentType() const
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

EInputsActivated CLinkyConfiguration::getInputsActivated() const
{
   try {
      if (getEquipmentType() == TwoInputs)
      {
         std::string temp = m_data.get<std::string>("EquipmentType.content.second.content.portEnabled");
         if (temp == "1xInputEnabled")
            return Input1Activated;
         else if (temp == "2xInputsEnabled")
            return Input2Activated;
         else
            return AllInputsActivated;
      }
      else // If it's the module with 1x port, we return port1Activated
         return Input1Activated;
   }
   catch (std::exception &e)
   {
      // By default ! To be compatible with v1.0.x
      YADOMS_LOG(trace) << e.what();
      YADOMS_LOG(information) << "first input activated used by default";
      return Input1Activated;
   }
}