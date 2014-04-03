#pragma once

#include "../ICommandRule.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   class CAcBasic : public ICommandRule
   {
   public:
      CAcBasic();
      virtual ~CAcBasic();
      
      // IRule implementation
      virtual const DeviceIdentifier getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent extractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< shared::xpl::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand);
      // [END] ICommandRule implemntation
   };
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication