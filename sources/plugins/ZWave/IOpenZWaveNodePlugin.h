#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include <value_classes/Value.h>
#include "OpenZWaveNodeConfiguration.h"

//--------------------------------------------------------------
/// \brief     Interface for node plugin
///            A plugin should represents particular feature (mostly COMMANDCLASS implementation)
//--------------------------------------------------------------
class IOpenZWaveNodePlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~IOpenZWaveNodePlugin()
   {
   }

   virtual bool isKeywordManagedByPlugin(OpenZWave::ValueID& value, const uint32 homeId, const uint8 nodeId) = 0;

   virtual boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords) = 0;
   
   virtual void onKeywordValueUpdated(OpenZWave::ValueID& value) = 0;
};

