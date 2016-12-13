#pragma once

#include "OpenZWaveCommandClass.h"
#include <value_classes/Value.h>
#include "IOpenZWaveNodeKeyword.h"
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <Notification.h>
#include <Poco/Types.h>

class COpenZWaveNodeKeywordFactory
{
public:
   static boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(OpenZWave::ValueID& vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords);
   
   static const shared::plugin::yPluginApi::CStandardCapacity& getCapacity(const std::string& name, const std::string& unit, shared::plugin::yPluginApi::EKeywordDataType dataType);
private:
   static boost::shared_ptr<IOpenZWaveNodeKeyword> generateHistoriser(OpenZWave::ValueID& vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords);
   static boost::shared_ptr<IOpenZWaveNodeKeyword> generateStandardKeyword(const std::string& vLabel, OpenZWave::ValueID& value, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, const ECommandClass& commandClass);
   static boost::shared_ptr<IOpenZWaveNodeKeyword> generateSpecificKeyword(const std::string& vLabel, const std::string& unit, OpenZWave::ValueID& value, shared::plugin::yPluginApi::EKeywordAccessMode accessMode);
   
   static std::vector<boost::shared_ptr<shared::plugin::yPluginApi::CStandardCapacity> > m_declaredCapacities;
};

