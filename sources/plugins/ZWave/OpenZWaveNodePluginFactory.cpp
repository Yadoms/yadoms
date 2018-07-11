#include "stdafx.h"
#include "OpenZWaveNodePluginFactory.h"
#include "OpenZWaveNodeUserCodePlugin.h"
#include <Manager.h>

void COpenZWaveNodePluginFactory::createPlugins(const uint32 homeId, const uint8 nodeId, std::vector< boost::shared_ptr<IOpenZWaveNodePlugin> > & out)
{
   int version = 0;
   if (nodeMatchCommandClass(homeId, nodeId, ECommandClass::kUserCode, version))
   {
      out.push_back(boost::make_shared<COpenZWaveNodeUserCodePlugin>());
   }
}

bool COpenZWaveNodePluginFactory::nodeMatchCommandClass(const uint32 homeId, const uint8 nodeId, ECommandClass toTest, int & version)
{
   std::string name;
   unsigned char versionC;
   if (OpenZWave::Manager::Get()->GetNodeClassInformation(homeId, nodeId, (int)toTest, &name, &versionC))
   {
      version = (int)versionC;
      return true;
   }
   return false;
}