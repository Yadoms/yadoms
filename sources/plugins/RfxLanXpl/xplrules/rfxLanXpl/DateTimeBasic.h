#pragma once

#include "../IRule.h"
#include "../IReadRule.h"

namespace xplrules
{
   namespace rfxLanXpl
   {
      class CDateTimeBasic : public IRule,
                             public IReadRule
      {
      public:
         CDateTimeBasic();
         virtual ~CDateTimeBasic();

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

      private:
         static xplcore::CXplMessageSchemaIdentifier m_protocol;
         static std::string m_keywordDateTime;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


