#include "stdafx.h"
#include "MessageSms.h"
#include "tools/Random.h"
#include "communication/rules/KeywordManager.h"
#include <shared/Log.h>

namespace communication { namespace rules { namespace extensions {


   std::string CMessageSms::m_keywordDevice = "device";
   std::string CMessageSms::m_keywordFrom = "from";
   std::string CMessageSms::m_keywordTo = "to";
   std::string CMessageSms::m_keywordContent = "content";
   std::string CMessageSms::m_keywordType = "type";
   std::string CMessageSms::m_keywordTypeValues = "message|acknowledgment|error";
   std::string CMessageSms::m_keywordAcknowledgment = "acknowledgment";
   std::string CMessageSms::m_keywordAcknowledgmentValues = "true|false";

   CMessageSms::CMessageSms()
   {
   }

   CMessageSms::~CMessageSms()
   {
   }

   // IRule implementation
   const CDeviceIdentifier CMessageSms::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return CDeviceIdentifier(msg.getBodyValue(m_keywordDevice));
   }

   MessageContent CMessageSms::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;

      BOOST_FOREACH( const MessageContent::value_type &i, msg.getBody())
      {
         if(!boost::iequals(i.first, m_keywordDevice))
            data.insert(i);
      }

      return data;      
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CMessageSms::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;

      //from
      keywords.push_back(communication::rules::CKeywordManager::createString(m_keywordFrom));

      //to
      keywords.push_back(communication::rules::CKeywordManager::createString(m_keywordTo));

      //type
      keywords.push_back(communication::rules::CKeywordManager::createEnumeration(m_keywordType, m_keywordTypeValues));

      //content
      keywords.push_back(communication::rules::CKeywordManager::createString(m_keywordContent));

      //acknowledgment
      keywords.push_back(communication::rules::CKeywordManager::createEnumeration(m_keywordAcknowledgment, m_keywordAcknowledgmentValues));

      return keywords;
   }
   // [END] IRule implementation



   // ICommandRule implemntation
   boost::shared_ptr< shared::xpl::CXplMessage > CMessageSms::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
   {
      ////////////////////////////
      // do some checks
      ////////////////////////////

      //check the device is valid
      std::string device = targetDevice.Address;

      //check the command value
      communication::command::CDeviceCommand::CommandData content = deviceCommand.getCommandData();
      if(content.find(m_keywordTo) == content.end())
      {
         throw shared::exception::CException( (boost::format("message.sms protocol needs a parameter '%1%'") % m_keywordTo).str());
      }

      if(content.find(m_keywordContent) == content.end())
      {
         throw shared::exception::CException( (boost::format("message.sms protocol needs a parameter '%1%'") % m_keywordContent).str());

      }


      ////////////////////////////
      // create the message
      ////////////////////////////

      //create the message
      boost::shared_ptr< shared::xpl::CXplMessage > newMessage(new shared::xpl::CXplMessage());

      //the AC.BSACI XplMessage if a xpl-trig
      newMessage->setTypeIdentifier(shared::xpl::CXplMessage::kXplCommand);

      //set hop to 1
      newMessage->setHop(1);

      //set the target (rfxcom-lan-<hax mac address>)
      newMessage->setTarget(shared::xpl::CXplActor::parse(targetDevice.HardwareIdentifier()));

      //set the ac.basic
      newMessage->setMessageSchemaIdentifier(shared::xpl::CXplMessageSchemaIdentifier("message", "sms"));

      //set the device addesss and unit (parse from argetDevice.Address)
      newMessage->addToBody(m_keywordDevice, device);

      //set the message target
      newMessage->addToBody(m_keywordTo, content[m_keywordTo]);

      //set the message body
      newMessage->addToBody(m_keywordContent, content[m_keywordContent]);

      //set the acknowlegment if specified
      if(content.find(m_keywordAcknowledgment) != content.end())
      {
         if(CKeywordManager::isEnumerationValue(content[m_keywordAcknowledgment], m_keywordAcknowledgmentValues))
         {
            newMessage->addToBody(m_keywordAcknowledgment, content[m_keywordAcknowledgment]);
         }
         else
         {
            YADOMS_LOG(warning) << "The value : '" << content[m_keywordAcknowledgment] << "' is not a valid enumeration one for acknowledgment";
         }
      }
      return newMessage;
   }

   std::string CMessageSms::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace extensions
} //namespace rules
} //namespace communication