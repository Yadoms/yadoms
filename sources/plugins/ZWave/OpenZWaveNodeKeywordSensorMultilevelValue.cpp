#include "stdafx.h"
#include "OpenZWaveNodeKeywordSensorMultilevelValue.h"

COpenZWaveNodeKeywordSensorMultilevelValue::COpenZWaveNodeKeywordSensorMultilevelValue(const std::string & keyword, OpenZWave::ValueID & valueId)
   :COpenZWaveNodeKeywordBase(valueId), m_switch(keyword)
{
}

bool COpenZWaveNodeKeywordSensorMultilevelValue::sendCommand(const std::string & commandData)
{
   m_switch.setCommand(commandData);
   return realSendCommand<bool>(m_switch.get());
}

const shared::plugin::yPluginApi::historization::IHistorizable & COpenZWaveNodeKeywordSensorMultilevelValue::getLastKeywordValue()
{
   //m_valueId.
   m_switch.set(extractLastValue<bool>());
   return m_switch;
}