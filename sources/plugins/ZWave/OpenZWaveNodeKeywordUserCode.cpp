#include "stdafx.h"
#include "OpenZWaveNodeKeywordUserCode.h"
#include "OpenZWaveHelpers.h"
#include <shared/plugin/yPluginApi/IDeviceCommand.h>

COpenZWaveNodeKeywordUserCode::COpenZWaveNodeKeywordUserCode(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CByteArrayTypeInfo &ti)
   : COpenZWaveNodeKeywordBase(valueId),
     m_keyword(boost::make_shared<historizers::CUserCode>(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode, ti))
{
}

COpenZWaveNodeKeywordUserCode::~COpenZWaveNodeKeywordUserCode()
{
}

bool COpenZWaveNodeKeywordUserCode::sendCommand(const std::string& commandData)
{
   m_keyword->set(commandData);
   return realSendCommand<std::string>(m_keyword->get());
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordUserCode::getLastKeywordValue()
{
   m_keyword->set(extractLastValue<std::string>());
   return m_keyword->getKeyword();
}

shared::CDataContainer COpenZWaveNodeKeywordUserCode::serialize()
{
   return m_keyword->getKeyword()->getTypeInfo();
}
