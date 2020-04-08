#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFanSubtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Fan FT1211R subtype
   //--------------------------------------------------------------
   class CFanFt1211R : public IFanSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         kRfxValue = sTypeFT1211R
      };

      CFanFt1211R();

      virtual ~CFanFt1211R();

      // IFanSubtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      unsigned char toProtocolCmnd() const override;
      unsigned long idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const override;
      // [END] IFanSubtype implementation

   private:
      unsigned char m_cmndByte;

      boost::shared_ptr<yApi::historization::CSwitch> m_light;
      boost::shared_ptr<yApi::historization::CSwitch> m_power;
      boost::shared_ptr<yApi::historization::CEvent> m_speed1;
      boost::shared_ptr<yApi::historization::CEvent> m_speed2;
      boost::shared_ptr<yApi::historization::CEvent> m_speed3;
      boost::shared_ptr<yApi::historization::CEvent> m_speed4;
      boost::shared_ptr<yApi::historization::CEvent> m_speed5;
      boost::shared_ptr<yApi::historization::CEvent> m_reverse;
      boost::shared_ptr<yApi::historization::CEvent> m_stopIn1Hour;
      boost::shared_ptr<yApi::historization::CEvent> m_stopIn4Hour;
      boost::shared_ptr<yApi::historization::CEvent> m_stopIn8Hour;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
