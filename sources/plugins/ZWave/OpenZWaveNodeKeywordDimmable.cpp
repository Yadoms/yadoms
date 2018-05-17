#include "stdafx.h"
#include "OpenZWaveNodeKeywordDimmable.h"
#include "OpenZWaveHelpers.h"
#include <shared/plugin/yPluginApi/IDeviceCommand.h>

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
   int v = fixValue(shared::plugin::yPluginApi::IDeviceCommand::simpleNumericCommandHelperToInt(commandData));
   m_keyword->set(v);
   return realSendCommand<Poco::Int32>(m_keyword->switchLevel());
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordDimmable::getLastKeywordValue()
{
   int v = fixValue(extractLastValue<Poco::Int32>());
   m_keyword->set(v);
   return m_keyword;
}

shared::CDataContainer COpenZWaveNodeKeywordDimmable::serialize()
{
   return m_keyword->getTypeInfo();
}

int COpenZWaveNodeKeywordDimmable::fixValue(int value)
{
   //it many cases zwave specs are not repected. some devices are dimmable from 0 to 99, or 0 to 100; or 1 to 100,...
   //to fix it quickly just round values at limits (1->0, and 99->100)
   if (value <= 1)
      return 0;
   if (value >= 99)
      return 100;
   return value;
}