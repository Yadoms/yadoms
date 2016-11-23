#include "stdafx.h"
#include "OpenZWaveHelpers.h"
#include <Manager.h>
#include "OpenZWaveCommandClass.h"

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
