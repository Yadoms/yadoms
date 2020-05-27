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
      class CAcBasic : public IRule,
                       public IReadRule,
                       public ICommandRule,
                       public ISupportManuallyDeviceCreationRule
      {
      public:
         CAcBasic();
         virtual ~CAcBasic();

         //------------------------------------
         ///\brief Provide the xpl protocol implemented by this class
         ///\return the xpl protocol
         //------------------------------------
         static xplcore::CXplMessageSchemaIdentifier getProtocol();

         // IRule implementation
         CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage& msg) override;
         KeywordList identifyKeywords(xplcore::CXplMessage& msg) override;
         // [END] IRule implementation

         // IReadRule implemntation
         MessageContent extractMessageData(xplcore::CXplMessage& msg) override;
         // [END] IReadRule implemntation

         // ICommandRule implemntation
         boost::shared_ptr<xplcore::CXplMessage> createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                  const std::string& rfxAddress,
                                                                  const boost::shared_ptr<shared::CDataContainer>& innerDetails) override;
         std::string generateVirtualDeviceIdentifier() override;
         // [END] ICommandRule implemntation

         // ISupportManuallyDeviceCreationRule implementation
         CDeviceContainer generateDeviceParameters(boost::shared_ptr<shared::CDataContainer>& configuration) const override;
         // [END] ISupportManuallyDeviceCreationRule implementation  

         //-----------------------------------------------------
         ///\brief                     The switch state
         //-----------------------------------------------------
         DECLARE_ENUM_HEADER(EState,
            ((Off))
            ((On))
            ((Dim))
         ) ;

      private:

         static std::string m_keywordAddress;
         static std::string m_keywordUnit;
         static std::string m_keywordCommand;
         static std::string m_keywordCommandValues;
         static std::string m_keywordLevel;
         static xplcore::CXplMessageSchemaIdentifier m_protocol;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


