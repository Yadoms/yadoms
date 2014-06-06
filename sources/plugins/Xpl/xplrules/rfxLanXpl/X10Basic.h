#pragma once

#include "../ICommandRule.h"

namespace xplrules { namespace rfxLanXpl {

   class CX10Basic : public ICommandRule
   {
   public:
      CX10Basic();
      virtual ~CX10Basic();
      
      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<CDeviceKeyword> > identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
      // ICommandRule implemntation
      //virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
