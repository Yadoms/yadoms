#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFanSubtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Fan LucciAir Dc subtype
   //--------------------------------------------------------------
   class CFanLucciAirDc : public IFanSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         kRfxValue = sTypeLucciAirDC
      };

      CFanLucciAirDc();

      virtual ~CFanLucciAirDc();

      // IFanSubtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      unsigned char toProtocolCmnd() const override;
      unsigned long idFromConfiguration(const shared::CDataContainer& configuration) const override;
      // [END] IFanSubtype implementation

   private:
      unsigned char m_cmndByte;

      boost::shared_ptr<yApi::historization::CSwitch> m_light;
      boost::shared_ptr<yApi::historization::CSwitch> m_power;
      boost::shared_ptr<yApi::historization::CEvent> m_minus;
      boost::shared_ptr<yApi::historization::CEvent> m_plus;
      boost::shared_ptr<yApi::historization::CEvent> m_reverse;
      boost::shared_ptr<yApi::historization::CEvent> m_naturalFlow;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
