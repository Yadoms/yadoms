#pragma once

#include "../IRule.h"

namespace xplrules { namespace standard {

   class CSensorBasic : public IRule
   {
   public:
      CSensorBasic();
      virtual ~CSensorBasic();
      
      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      //virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
   private:
      //-------------------------------------
      ///\brief   Rewrite a keyword
      ///\param [in] keyword The keyword to check
      ///\return the keyword renammed if needed
      //-------------------------------------
      std::string rewriteKeyword(const std::string & keyword);
   };
   
} //namespace standard
} //namespace xplrules
