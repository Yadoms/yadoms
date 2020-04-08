#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"

namespace xplrules
{
   namespace rfxLanXpl
   {
      class CX10Security : public IRule,
                           public IReadRule,
                           public ICommandRule,
                           public ISupportManuallyDeviceCreationRule
      {
      public:
         CX10Security();
         virtual ~CX10Security();

         //------------------------------------
         ///\brief Provide the xpl protocol implemented by this class
         ///\return the xpl protocol
         //------------------------------------
         static xplcore::CXplMessageSchemaIdentifier getProtocol();

         // IRule implementation
         CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage& msg) override;
         KeywordList identifyKeywords(xplcore::CXplMessage& msg) override;
         // [END] IRule implementation

         // IReadRule implementation
         MessageContent extractMessageData(xplcore::CXplMessage& msg) override;
         // [END] IReadRule implementation

         // ICommandRule implemntation
         boost::shared_ptr<xplcore::CXplMessage> createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                  const std::string& rfxAddress,
                                                                  const shared::CDataContainerSharedPtr& innerDetails) override;
         std::string generateVirtualDeviceIdentifier() override;
         // [END] ICommandRule implemntation

         // ISupportManuallyDeviceCreationRule implementation
         CDeviceContainer generateDeviceParameters(shared::CDataContainerSharedPtr& configuration) const override;
         // [END] ISupportManuallyDeviceCreationRule implementation   

      private:

         static xplcore::CXplMessageSchemaIdentifier m_protocol;
         static std::string m_keywordCommand;
         static std::string m_keywordDevice;
         static std::string m_keywordType;
         static std::string m_keywordTamper;
         static std::string m_keywordLowBattery;
         static std::string m_keywordDelay;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


