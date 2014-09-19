#include "stdafx.h"
#include "OpenZWaveNodeKeywordSwitch.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>

COpenZWaveNodeKeywordSwitch::COpenZWaveNodeKeywordSwitch(OpenZWave::ValueID & valueId)
   :COpenZWaveNodeKeywordBase(valueId)
{
}

bool COpenZWaveNodeKeywordSwitch::sendCommand(const std::string & commandData)
{
   shared::plugin::yadomsApi::commands::CSwitch commandDetails(commandData); 
   return realSendCommand<bool>(commandDetails.isOn());
}

std::string COpenZWaveNodeKeywordSwitch::getLastKeywordValue()
{
   //m_valueId.
   bool switchState = extractLastValue<bool>();

   shared::plugin::yadomsApi::commands::CSwitch sw(switchState ? 100 : 0);
   return sw.format();
}