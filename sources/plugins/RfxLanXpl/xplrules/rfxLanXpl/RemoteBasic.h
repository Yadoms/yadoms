#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      class CRemoteBasic : public IRule,
                           public IReadRule,
                           public ICommandRule,
                           public ISupportManuallyDeviceCreationRule
      {
      public:
         CRemoteBasic();
         virtual ~CRemoteBasic();

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
         DECLARE_ENUM_HEADER(ERemoteType,
            ((PC))
            ((Medion))
            ((Ati))
            ((AtiPlus))
         )


         static xplcore::CXplMessageSchemaIdentifier m_protocol;
         static std::string m_keywordKeys;
         static std::string m_keywordDevice;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


