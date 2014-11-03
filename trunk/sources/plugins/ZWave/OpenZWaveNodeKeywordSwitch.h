#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yadomsApi/historization/Dimmable.h>

class COpenZWaveNodeKeywordSwitch : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordSwitch(const std::string & keyword, OpenZWave::ValueID & valueId);
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData);
   virtual const shared::plugin::yadomsApi::historization::IHistorizable & getLastKeywordValue();
   // [END] IOpenZWaveKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   shared::plugin::yadomsApi::historization::CDimmable m_switch;
};
