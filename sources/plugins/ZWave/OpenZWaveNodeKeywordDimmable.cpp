#include "stdafx.h"
#include "OpenZWaveNodeKeywordDimmable.h"
#include "OpenZWaveHelpers.h"
COpenZWaveNodeKeywordDimmable::COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti)
   : COpenZWaveNodeKeywordBase(valueId),
     m_keyword(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
{
}

COpenZWaveNodeKeywordDimmable::~COpenZWaveNodeKeywordDimmable()
{
}

bool COpenZWaveNodeKeywordDimmable::sendCommand(const std::string& commandData)
{
   m_keyword->set(commandData);
   return realSendCommand<Poco::UInt8>(m_keyword->switchLevel());
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordDimmable::getLastKeywordValue()
{
   m_keyword->set(extractLastValue<Poco::UInt8>());
   return m_keyword;
}

shared::CDataContainer COpenZWaveNodeKeywordDimmable::serialize()
{
   return m_keyword->getTypeInfo();
}