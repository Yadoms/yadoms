#pragma once

#include "../IRule.h"

namespace xplrules { namespace standard {

   class CSensorBasic : public IRule
   {
   public:
      CSensorBasic();
      virtual ~CSensorBasic();
      
      //------------------------------------
      ///\brief Provide the xpl protocol implemented by this class
      ///\return the xpl protocol
      //------------------------------------
      static const xplcore::CXplMessageSchemaIdentifier getProtocol();

      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<CDeviceKeyword> > identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
   private:
      //-------------------------------------
      ///\brief   Rewrite a keyword
      ///\param [in] keyword The keyword to check
      ///\return the keyword renammed if needed
      //-------------------------------------
      std::string rewriteKeyword(const std::string & keyword);

      static xplcore::CXplMessageSchemaIdentifier m_protocol;

   };
   
} //namespace standard
} //namespace xplrules
