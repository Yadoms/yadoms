#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFanSubtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Fan Falmec subtype
   //--------------------------------------------------------------
   class CFanFalmec : public IFanSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         kRfxValue = sTypeFalmec
      };

      CFanFalmec();

      virtual ~CFanFalmec();

      // IFanSubtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      unsigned char toProtocolCmnd() const override;
      unsigned long idFromConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration) const override;
      // [END] IFanSubtype implementation

   private:
      unsigned char m_cmndByte;

      boost::shared_ptr<yApi::historization::CSwitch> m_light;
      boost::shared_ptr<yApi::historization::CEvent> m_powerOff;
      boost::shared_ptr<yApi::historization::CEvent> m_speed1;
      boost::shared_ptr<yApi::historization::CEvent> m_speed2;
      boost::shared_ptr<yApi::historization::CEvent> m_speed3;
      boost::shared_ptr<yApi::historization::CEvent> m_speed4;
      boost::shared_ptr<yApi::historization::CEvent> m_timer1;
      boost::shared_ptr<yApi::historization::CEvent> m_timer2;
      boost::shared_ptr<yApi::historization::CEvent> m_timer3;
      boost::shared_ptr<yApi::historization::CEvent> m_timer4;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
