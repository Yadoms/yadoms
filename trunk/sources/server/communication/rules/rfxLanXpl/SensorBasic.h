#pragma once

#include "../IRule.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   class CSensorBasic : public IRule
   {
   public:
      CSensorBasic();
      virtual ~CSensorBasic();
      
      // IRule implementation
      virtual const DeviceIdentifier GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent ExtractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation

   private:
      bool isOregonDevice(shared::xpl::CXplMessage & msg);

   };
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication