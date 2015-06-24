#pragma once

#include "../ICommandRule.h"

namespace communication { namespace rules { namespace standard {

   class CX10Basic : public ICommandRule
   {
   public:
      CX10Basic();
      virtual ~CX10Basic();
      
      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent extractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< shared::xpl::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
   };
   
} //namespace standard
} //namespace rules
} //namespace communication