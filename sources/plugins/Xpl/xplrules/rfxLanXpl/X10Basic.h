#pragma once

#include "../ICommandRule.h"

namespace xplrules { namespace rfxLanXpl {

   class CX10Basic : public ICommandRule
   {
   public:
      CX10Basic();
      virtual ~CX10Basic();
      
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
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
   private:
      static xplcore::CXplMessageSchemaIdentifier m_protocol;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
