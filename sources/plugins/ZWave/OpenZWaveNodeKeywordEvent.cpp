#include "stdafx.h"
#include "OpenZWaveNodeKeywordEvent.h"
#include "OpenZWaveHelpers.h"

COpenZWaveNodeKeywordEvent::COpenZWaveNodeKeywordEvent(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
   : COpenZWaveNodeKeywordBase(valueId),
     m_keyword(boost::make_shared<shared::plugin::yPluginApi::historization::CEvent>(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode))
{
}

COpenZWaveNodeKeywordEvent::~COpenZWaveNodeKeywordEvent()
{
}

bool COpenZWaveNodeKeywordEvent::sendCommand(const std::string& commandData)
{
   return pressButton();

}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordEvent::getLastKeywordValue()
{
   return m_keyword;
}

shared::CDataContainerSharedPtr COpenZWaveNodeKeywordEvent::serialize()
{
   return m_keyword->getTypeInfo();
}

