#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity1Subtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Scurity1 Meiantech subtype
   //--------------------------------------------------------------
   class CSecurity1Meiantech : public ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         rfxValue = sTypeMeiantech
      };

      CSecurity1Meiantech();

      virtual ~CSecurity1Meiantech();

      // ISecurity1Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char statusByte) override;
      unsigned char toProtocolState() const override;
      unsigned long idFromProtocol(const RBUF& rbuf) const override;
      // [END] ISecurity1Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        Status byte buffering
      //--------------------------------------------------------------
      unsigned char m_statusByte;

      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_motion;
      boost::shared_ptr<yApi::historization::CSwitch> m_panic;
      boost::shared_ptr<yApi::historization::CSwitch> m_tamper;
      boost::shared_ptr<yApi::historization::CArmingAlarm> m_armAlarm;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
