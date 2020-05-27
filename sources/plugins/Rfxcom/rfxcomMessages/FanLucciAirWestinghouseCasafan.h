#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFanSubtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Fan FanLucciAir/Westinghouse/Casafan subtype
   //--------------------------------------------------------------
   class CFanLucciAirWestinghouseCasafan : public IFanSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         kRfxValueLucciAir = sTypeLucciAir,
         kRfxValueWestinghouse = sTypeWestinghouse,
         kRfxValueCasafan = sTypeCasafan
      };

      explicit CFanLucciAirWestinghouseCasafan(unsigned char subType);

      virtual ~CFanLucciAirWestinghouseCasafan();

      // IFanSubtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      unsigned char toProtocolCmnd() const override;
      unsigned long idFromConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration) const override;
      // [END] IFanSubtype implementation

   private:
      unsigned char m_subType;
      unsigned char m_cmndByte;

      boost::shared_ptr<yApi::historization::CSwitch> m_light;
      boost::shared_ptr<yApi::historization::CEvent> m_fanSpeedOff;
      boost::shared_ptr<yApi::historization::CEvent> m_fanSpeedLow;
      boost::shared_ptr<yApi::historization::CEvent> m_fanSpeedMed;
      boost::shared_ptr<yApi::historization::CEvent> m_fanSpeedHigh;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
