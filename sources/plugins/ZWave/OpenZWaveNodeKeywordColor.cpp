#include "stdafx.h"
#include "OpenZWaveNodeKeywordColor.h"
#include "OpenZWaveHelpers.h"
#include <Poco/RegularExpression.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <Poco/Types.h>

COpenZWaveNodeKeywordColor::COpenZWaveNodeKeywordColor(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
   : COpenZWaveNodeKeywordBase(valueId),
     m_keyword(boost::make_shared<shared::plugin::yPluginApi::historization::CColorRGBW>(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode))
{
}

COpenZWaveNodeKeywordColor::~COpenZWaveNodeKeywordColor()
{
}

bool COpenZWaveNodeKeywordColor::sendCommand(const std::string& commandData)
{
   //commandData should contain an integer (unsigned) value 287454020
   const Poco::UInt32 value = shared::plugin::yPluginApi::IDeviceCommand::simpleNumericCommandHelperToUInt(commandData);
   m_keyword->set(value);

   //it must be converted to "#11223344"
   const std::string toRgbwFormat = (boost::format("#%08X") % value).str();
   return realSendCommand<std::string>(toRgbwFormat);

}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordColor::getLastKeywordValue()
{
   const std::string val  = extractLastValue<std::string>(); //#RRGGBBWW

   const std::string regex1_11 = "#(?<color>[0-9a-fA-F]{8})";

   const Poco::RegularExpression re(regex1_11);
   std::vector<std::string> results;
   const int resultCount = re.split(val, results);
   if (resultCount == 2)
   {
      std::stringstream ss;
      ss << std::hex << results[1];

      Poco::UInt32 valueInt = 0;
      ss >> valueInt;
      m_keyword->set(valueInt);
   } 
   else
   {
      m_keyword->set(0);
   }
   return m_keyword;
}

shared::CDataContainer COpenZWaveNodeKeywordColor::serialize()
{
   return m_keyword->getTypeInfo();
}

