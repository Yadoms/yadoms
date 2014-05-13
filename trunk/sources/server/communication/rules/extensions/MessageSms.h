#pragma once

#include "../ICommandRule.h"

namespace communication { namespace rules { namespace extensions {
   
   //--------------------------------
   ///\brief Class handler for xpl message.sms schema 
   //--------------------------------
   class CMessageSms : public ICommandRule
   {
   public:
      CMessageSms();
      virtual ~CMessageSms();
      
      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg);
      virtual MessageContent extractMessageData(shared::xpl::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg);
      // [END] IRule implementation
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< shared::xpl::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation

   private:
      static std::string m_keywordDevice;
      static std::string m_keywordFrom;
      static std::string m_keywordTo;
      static std::string m_keywordContent;
      static std::string m_keywordType;
      static std::string m_keywordTypeValues;
      static std::string m_keywordAcknowledgment;
      static std::string m_keywordAcknowledgmentValues;
   };
   
} //namespace extensions
} //namespace rules
} //namespace communication