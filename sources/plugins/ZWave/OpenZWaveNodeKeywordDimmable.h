#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/Dimmable.h>


class COpenZWaveNodeKeywordDimmable : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID & valueId, const std::string & vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordDimmable();

   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData);
   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue();
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CDimmable> m_keyword;
};

