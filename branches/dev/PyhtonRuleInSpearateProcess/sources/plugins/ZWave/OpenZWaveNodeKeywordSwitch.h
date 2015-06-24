#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/Switch.h>

class COpenZWaveNodeKeywordSwitch : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordSwitch(const std::string & keyword, OpenZWave::ValueID & valueId);
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData);
   virtual const shared::plugin::yPluginApi::historization::IHistorizable & getLastKeywordValue();
   // [END] IOpenZWaveKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   shared::plugin::yPluginApi::historization::CSwitch m_switch;
};
