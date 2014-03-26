#pragma once

#include "../IRule.h"

namespace communication { namespace rules { namespace standard {

   class CSensorBasic : public IRule
   {
   public:
      CSensorBasic();
      virtual ~CSensorBasic();
      
      // IRule implementation
      virtual const DeviceIdentifier GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent ExtractMessageData(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
   };
   
} //namespace standard
} //namespace rules
} //namespace communication