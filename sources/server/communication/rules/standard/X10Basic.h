#pragma once

#include "../IRule.h"

namespace communication { namespace rules { namespace standard {

   class CX10Basic : public IRule
   {
   public:
      CX10Basic();
      virtual ~CX10Basic();
      
      // IRule implementation
      virtual const DeviceIdentifier GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent ExtractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
   };
   
} //namespace standard
} //namespace rules
} //namespace communication