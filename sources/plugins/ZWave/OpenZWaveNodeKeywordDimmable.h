#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include <shared/plugin/yPluginApi/historization/Dimmable.h>
#include "../typeinfo/IntegerTypeInfo.h"


class COpenZWaveNodeKeywordDimmable : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordDimmable();

   // IOpenZWaveKeyword implementation
   bool sendCommand(const std::string& commandData) override;
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() override;
   shared::CDataContainer serialize() override;
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CDimmable> m_keyword;
};

