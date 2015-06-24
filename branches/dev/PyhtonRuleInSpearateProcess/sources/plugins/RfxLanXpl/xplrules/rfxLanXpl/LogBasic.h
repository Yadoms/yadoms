#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CLogBasic : public IRule, public IReadRule
   {
   public:
      CLogBasic();
      virtual ~CLogBasic();
      
      //------------------------------------
      ///\brief Provide the xpl protocol implemented by this class
      ///\return the xpl protocol
      //------------------------------------
      static const xplcore::CXplMessageSchemaIdentifier getProtocol();

      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual KeywordList identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
      // IReadRule implementation
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      // [END] IReadRule implementation
        
   private:
      static xplcore::CXplMessageSchemaIdentifier m_protocol;
      static std::string m_keywordType;
      static std::string m_keywordText;
      static std::string m_keywordCode;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
