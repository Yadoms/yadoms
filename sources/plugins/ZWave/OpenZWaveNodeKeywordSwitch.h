#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>

class COpenZWaveNodeKeywordSwitch : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordSwitch(const std::string & keyword, OpenZWave::ValueID & valueId);
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData);
   virtual const shared::plugin::yadomsApi::commands::IHistorizable & getLastKeywordValue();
   // [END] IOpenZWaveKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   shared::plugin::yadomsApi::commands::CSwitch m_switch;
};
