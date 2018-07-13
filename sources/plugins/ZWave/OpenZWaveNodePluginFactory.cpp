#include "stdafx.h"
#include "OpenZWaveNodePluginFactory.h"
#include "OpenZWaveNodeUserCodePlugin.h"
#include <Manager.h>
#include "OpenZWaveNode.h"

void COpenZWaveNodePluginFactory::createPlugins(const uint32 homeId, const uint8 nodeId, std::vector< boost::shared_ptr<IOpenZWaveNodePlugin> > & out, COpenZWaveNode * pMasterNode)
{
   int version = 0;
   if (nodeMatchCommandClass(homeId, nodeId, ECommandClass::kUserCode, version))
   {
      out.push_back(boost::make_shared<COpenZWaveNodeUserCodePlugin>(pMasterNode));
   }
}

bool COpenZWaveNodePluginFactory::nodeMatchCommandClass(const uint32 homeId, const uint8 nodeId, ECommandClass toTest, int & version)
{
   std::string name;
   unsigned char versionC;
   if (OpenZWave::Manager::Get()->GetNodeClassInformation(homeId, nodeId, (uint8)(int)toTest, &name, &versionC))
   {
      version = (int)versionC;
      return true;
   }
   return false;
}