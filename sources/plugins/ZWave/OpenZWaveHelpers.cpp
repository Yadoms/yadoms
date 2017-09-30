#include "stdafx.h"
#include "OpenZWaveHelpers.h"
#include <Manager.h>
#include "OpenZWaveCommandClass.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

std::string COpenZWaveHelpers::GenerateKeywordName(OpenZWave::ValueID& value)
{
   ECommandClass commandClass(static_cast<int>(value.GetCommandClassId()));
   auto vLabel = OpenZWave::Manager::Get()->GetValueLabel(value);
   return (boost::format("%1%.%2%.%3%") % commandClass.toString() % vLabel % static_cast<int>(value.GetInstance())).str();
}

std::string COpenZWaveHelpers::GenerateDeviceName(Poco::UInt32 homeId, Poco::UInt8 nodeId)
{
   return (boost::format("%1%.%2%") % homeId % static_cast<int>(nodeId)).str();
}

std::string COpenZWaveHelpers::IdentifyUnits(OpenZWave::ValueID& value)
{
   std::string units = OpenZWave::Manager::Get()->GetValueUnits(value);

   if (units.empty())
      units = shared::plugin::yPluginApi::CStandardUnits::NoUnit();
   else if(boost::iequals(units, "C"))
      units = shared::plugin::yPluginApi::CStandardUnits::DegreesCelcius();
   else if(boost::iequals(units, "F"))
      units = shared::plugin::yPluginApi::CStandardUnits::DegreesFarenheit();
   else if(boost::iequals(units, "cubic meters"))
      units = shared::plugin::yPluginApi::CStandardUnits::CubicMetre();
   else if (boost::iequals(units, "%"))
	  units = shared::plugin::yPluginApi::CStandardUnits::Percent();
   return units;
}

void COpenZWaveHelpers::GetBooleanValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help)
{
   help = OpenZWave::Manager::Get()->GetValueHelp(value);
   name = OpenZWave::Manager::Get()->GetValueLabel(value);
}

void COpenZWaveHelpers::GetStringValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help)
{
   help = OpenZWave::Manager::Get()->GetValueHelp(value);
   name = OpenZWave::Manager::Get()->GetValueLabel(value);
}

void COpenZWaveHelpers::GetIntegerValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, int & min, int & max, std::string & unit)
{
   help = OpenZWave::Manager::Get()->GetValueHelp(value);
   min = OpenZWave::Manager::Get()->GetValueMin(value);
   max = OpenZWave::Manager::Get()->GetValueMax(value);
   unit = OpenZWave::Manager::Get()->GetValueUnits(value);
   name = OpenZWave::Manager::Get()->GetValueLabel(value);
}


void COpenZWaveHelpers::GetDecimalValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, int & min, int & max, std::string & unit, int & precision)
{
   help = OpenZWave::Manager::Get()->GetValueHelp(value);
   min = OpenZWave::Manager::Get()->GetValueMin(value);
   max = OpenZWave::Manager::Get()->GetValueMax(value);
   unit = OpenZWave::Manager::Get()->GetValueUnits(value);
   name = OpenZWave::Manager::Get()->GetValueLabel(value);
   uint8 p;
   OpenZWave::Manager::Get()->GetValueFloatPrecision(value, &p);
   precision = p;
   //precision
}

void COpenZWaveHelpers::GetEnumValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, std::map<int, std::string> & values)
{
   help = OpenZWave::Manager::Get()->GetValueHelp(value);
   name = OpenZWave::Manager::Get()->GetValueLabel(value);


   std::vector<int> keys;
   std::vector<std::string> valuesList;

   OpenZWave::Manager::Get()->GetValueListValues(value, &keys);
   OpenZWave::Manager::Get()->GetValueListItems(value, &valuesList);
   
   values.clear();
   for (int i = 0; i < valuesList.size(); ++i)
   {
      values[keys[i]] = valuesList[i];
   }
}


void COpenZWaveHelpers::RetreiveOpenZWaveIds(const std::string& device, const std::string& keyword, uint32& homeId, uint8& nodeId, uint8& instance)
{
   std::vector<std::string> splittedDevice;
   boost::split(splittedDevice, device, boost::is_any_of("."), boost::token_compress_on);
   if (splittedDevice.size() < 2)
   {
      throw shared::exception::CException("The device id is invalid : not matching pattern : <homeId>.<nodeId>");
   }
   homeId = boost::lexical_cast<uint32>(splittedDevice[0]);
   nodeId = static_cast<uint8>(atoi(splittedDevice[1].c_str())); //dont use lexical cast for uint8, because it realize a string to char conversion: "2" is transform in '2' = 0x32

                                                                 //instance <class>.<keyword>.<instance>
   std::vector<std::string> splittedKeyword;
   boost::split(splittedKeyword, keyword, boost::is_any_of("."), boost::token_compress_on);
   if (splittedKeyword.size() > 2)
      instance = static_cast<uint8>(atoi(splittedKeyword[2].c_str())); //dont use lexical cast for uint8, because it realize a string to char conversion: "2" is transform in '2' = 0x32
   else
      instance = 1; //default instance value in OpenZWave
}

std::string COpenZWaveHelpers::GenerateDeviceId(const uint32 homeId, const uint8 nodeId)
{
   return (boost::format("%1%.%2%") % homeId % (int)nodeId).str();
}