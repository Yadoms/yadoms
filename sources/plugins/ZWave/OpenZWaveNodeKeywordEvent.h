#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/Event.h>

class COpenZWaveNodeKeywordEvent : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordEvent(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordEvent();

   // IOpenZWaveKeyword implementation
   bool sendCommand(const std::string& commandData) override;
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() override;
   shared::CDataContainer serialize() override;
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CEvent> m_keyword;
};

