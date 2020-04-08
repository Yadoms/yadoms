#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/ColorRGBW.h>

class COpenZWaveNodeKeywordColor : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordColor(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordColor();

   // IOpenZWaveKeyword implementation
   bool sendCommand(const std::string& commandData) override;
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() override;
   shared::CDataContainerSharedPtr serialize() override;
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CColorRGBW> m_keyword;
};

