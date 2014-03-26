#pragma once

#include "../IRule.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   class CAcBasic : public IRule
   {
   public:
      CAcBasic();
      virtual ~CAcBasic();
      
      // IRule implementation
      virtual const DeviceIdentifier GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent ExtractMessageData(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
   };
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication