#include "stdafx.h"
#include "OpenZWaveNodeKeywordDimmable.h"
#include "OpenZWaveHelpers.h"

COpenZWaveNodeKeywordDimmable::COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID& valueId,
                                                             const std::string& vLabel,
                                                             shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
   : COpenZWaveNodeKeywordBase(valueId),
     m_keyword(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(COpenZWaveHelpers::GenerateKeywordName(valueId),
                                                                                        accessMode))
{
}

COpenZWaveNodeKeywordDimmable::~COpenZWaveNodeKeywordDimmable()
{
}

bool COpenZWaveNodeKeywordDimmable::sendCommand(const std::string& commandData)
{
   m_keyword->set(commandData);
   return realSendCommand<int>(m_keyword->switchLevel());
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordDimmable::getLastKeywordValue()
{
   m_keyword->set(extractLastValue<int>());
   return m_keyword;
}

shared::CDataContainer COpenZWaveNodeKeywordDimmable::serialize()
{
   shared::CDataContainer result;
   result.set("type", "integer");
   result.set("int-min", 0);
   result.set("int-max", 100);
   return result;
}

