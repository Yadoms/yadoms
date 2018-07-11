#include "stdafx.h"
#include "OpenZWaveNodeUserCodePlugin.h"
#include "OpenZWaveCommandClass.h"
#include <Manager.h>
#include <command_classes/UserCode.h>
#include "OpenZWaveNodeKeywordFactory.h"

COpenZWaveNodeUserCodePlugin::COpenZWaveNodeUserCodePlugin()
{
}

COpenZWaveNodeUserCodePlugin::~COpenZWaveNodeUserCodePlugin()
{
}

bool COpenZWaveNodeUserCodePlugin::isKeywordManagedByPlugin(OpenZWave::ValueID& vID, const uint32 homeId, const uint8 nodeId)
{
   ECommandClass commandClass(static_cast<int>(vID.GetCommandClassId()));
   if (commandClass == ECommandClass::kUserCode)
      return true;
   return false;
}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeUserCodePlugin::createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords)
{
   auto vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
   if (boost::istarts_with(vLabel, "Code "))
   {
      //if (OpenZWave::ValueRaw* value = static_cast<OpenZWave::ValueRaw*>(GetValue(_instance, i)))
      uint8 *d = NULL;
      uint8 size;
      if (OpenZWave::Manager::Get()->GetValueAsRaw(vID, &d, &size))
      {
         if (size >= 2)
         {
            if (d[0] == 0 && d[1] == 0)
            {
               //unused tag, do not create keyword
               return boost::shared_ptr<IOpenZWaveNodeKeyword>();
            }
         }
      }
   }

   return  COpenZWaveNodeKeywordFactory::createKeyword(vID, homeId, nodeId, includeSystemKeywords);
   
}

void COpenZWaveNodeUserCodePlugin::onKeywordValueUpdated(OpenZWave::ValueID& value)
{
  
}