#include "stdafx.h"
#include "OpenZWaveHelpers.h"
#include <Manager.h>
#include "OpenZWaveCommandClass.h"

std::string COpenZWaveHelpers::GenerateKeywordName(OpenZWave::ValueID & value)
{
   ECommandClass commandClass((int)value.GetCommandClassId());
   std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(value);
   return (boost::format("%1%.%2%") % commandClass.toString() % vLabel).str();
}

std::string COpenZWaveHelpers::GenerateDeviceName(Poco::UInt32 homeId, Poco::UInt8 nodeId)
{
   return (boost::format("%1%.%2%") % homeId % (int)nodeId).str();
}

