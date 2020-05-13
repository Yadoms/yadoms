#pragma once

#include "../IRule.h"
#include "../ICommandRule.h"
#include <shared/enumeration/EnumHelpers.hpp>


namespace xplrules
{
   namespace rfxLanXpl
   {
      class CControlBasic : public IRule,
                            public ICommandRule
      {
      public:
         CControlBasic();
         virtual ~CControlBasic();

         //------------------------------------
         ///\brief Provide the xpl protocol implemented by this class
         ///\return the xpl protocol
         //------------------------------------
         static xplcore::CXplMessageSchemaIdentifier getProtocol();

         // IRule implementation
         CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage& msg) override;
         KeywordList identifyKeywords(xplcore::CXplMessage& msg) override;
         // [END] IRule implementation

         // ICommandRule implemntation
         boost::shared_ptr<xplcore::CXplMessage> createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                  const std::string& rfxAddress,
                                                                  const boost::shared_ptr<shared::CDataContainer>& innerDetails) override;
         std::string generateVirtualDeviceIdentifier() override;
         // [END] ICommandRule implemntation

      private:
         DECLARE_ENUM_HEADER(EType,
            ((Ninja))
            ((Mertik))
            ((Output))
         ) ;

         static xplcore::CXplMessageSchemaIdentifier m_protocol;

         static std::string m_keywordCurrent;
         static std::string m_keywordType;
         static std::string m_keywordDevice;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


