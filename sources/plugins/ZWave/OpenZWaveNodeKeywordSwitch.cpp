#include "stdafx.h"
#include "OpenZWaveNodeKeywordSwitch.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>

COpenZWaveNodeKeywordSwitch::COpenZWaveNodeKeywordSwitch(const std::string & keyword, OpenZWave::ValueID & valueId)
   :COpenZWaveNodeKeywordBase(valueId), m_switch(keyword)
{
}

bool COpenZWaveNodeKeywordSwitch::sendCommand(const std::string & commandData)
{
   m_switch.set(commandData);
   return realSendCommand<bool>(m_switch.isOn());
}

const shared::plugin::yadomsApi::commands::IHistorizable & COpenZWaveNodeKeywordSwitch::getLastKeywordValue()
{
   //m_valueId.
   m_switch.set(extractLastValue<bool>());
   return m_switch;
}