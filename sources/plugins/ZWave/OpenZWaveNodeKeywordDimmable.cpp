#include "stdafx.h"
#include "OpenZWaveNodeKeywordDimmable.h"
#include "OpenZWaveHelpers.h"

COpenZWaveNodeKeywordDimmable::COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID & valueId, const std::string & vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
   : COpenZWaveNodeKeywordBase(valueId), m_keyword(new shared::plugin::yPluginApi::historization::CDimmable(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode))
{

}

COpenZWaveNodeKeywordDimmable::~COpenZWaveNodeKeywordDimmable()
{

}

bool COpenZWaveNodeKeywordDimmable::sendCommand(const std::string & commandData)
{
   m_keyword->set(commandData);
   return realSendCommand<int>(m_keyword->switchLevel());
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordDimmable::getLastKeywordValue()
{
   m_keyword->set(extractLastValue<int>());
   return m_keyword;
}
