#pragma once

#include "OpenZWaveNodeKeywordBase.h"

class COpenZWaveNodeKeywordSwitch : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordSwitch(OpenZWave::ValueID & valueId);
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData);
   virtual std::string getLastKeywordValue();
   // [END] IOpenZWaveKeyword implementation
};
