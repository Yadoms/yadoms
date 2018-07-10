#pragma once

#include "OpenZWaveNodeKeywordBase.h"
#include "historizers/UserCode.h"

class COpenZWaveNodeKeywordUserCode : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordUserCode(OpenZWave::ValueID& valueId, const std::string& vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CByteArrayTypeInfo &ti);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordUserCode();

   // IOpenZWaveKeyword implementation
   bool sendCommand(const std::string& commandData) override;
   boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() override;
   shared::CDataContainer serialize() override;
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<historizers::CUserCode> m_keyword;
};

