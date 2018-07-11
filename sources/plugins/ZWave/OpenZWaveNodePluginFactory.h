#pragma once

#include "IOpenZWaveNodePlugin.h"
#include "OpenZWaveCommandClass.h"

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node (from OpenZWave)
///            Provide access to capacities and keywords
//--------------------------------------------------------------
class COpenZWaveNodePluginFactory
{
public:
   static void createPlugins(const uint32 homeId, const uint8 nodeId, std::vector< boost::shared_ptr<IOpenZWaveNodePlugin> > & out);

private:
   static bool nodeMatchCommandClass(const uint32 homeId, const uint8 nodeId, ECommandClass toTest, int & version);
};

