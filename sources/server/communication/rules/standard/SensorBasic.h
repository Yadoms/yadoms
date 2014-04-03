#pragma once

#include "../IRule.h"

namespace communication { namespace rules { namespace standard {

   class CSensorBasic : public IRule
   {
   public:
      CSensorBasic();
      virtual ~CSensorBasic();
      
      // IRule implementation
      virtual const DeviceIdentifier getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent extractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
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
} //namespace rules
} //namespace communication