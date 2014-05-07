#include "stdafx.h"
#include "MessageSms.h"
#include "tools/Random.h"

namespace communication { namespace rules { namespace extensions {

   CMessageSms::CMessageSms()
   {
   }

   CMessageSms::~CMessageSms()
   {
   }

   // IRule implementation
   const DeviceIdentifier CMessageSms::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("device");
   }

   MessageContent CMessageSms::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;

      if(msg.getBody().find("from") != msg.getBody().end())
         data.insert(std::make_pair("from", msg.getBodyValue("from")));

      if(msg.getBody().find("type") != msg.getBody().end())
         data.insert(std::make_pair("type", msg.getBodyValue("type")));

      if(msg.getBody().find("content") != msg.getBody().end())
         data.insert(std::make_pair("content", msg.getBodyValue("content")));

      if(msg.getBody().find("to") != msg.getBody().end())
         data.insert(std::make_pair("to", msg.getBodyValue("to")));

      if(msg.getBody().find("body") != msg.getBody().end())
         data.insert(std::make_pair("body", msg.getBodyValue("body")));

      if(msg.getBody().find("acknowledgment") != msg.getBody().end())
         data.insert(std::make_pair("acknowledgment", msg.getBodyValue("acknowledgment")));

      return data;      
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CMessageSms::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;


      if(msg.getTypeIdentifier(), shared::xpl::CXplMessage::kXplTrigger)
      {
         //from
         boost::shared_ptr<database::entities::CKeyword> fromKeyword(new database::entities::CKeyword());
         fromKeyword->Name = "from";
         fromKeyword->Type = "string";
         keywords.push_back(fromKeyword);

         //type
         boost::shared_ptr<database::entities::CKeyword> typeKeyword(new database::entities::CKeyword());
         typeKeyword->Name = "type";
         typeKeyword->Type = "enumeration";
         typeKeyword->Parameters = "message|acknowledgment|error";
         keywords.push_back(typeKeyword);

         //content
         boost::shared_ptr<database::entities::CKeyword> contentKeyword(new database::entities::CKeyword());
         contentKeyword->Name = "from";
         contentKeyword->Type = "string";
         keywords.push_back(contentKeyword);

      }


      if(msg.getTypeIdentifier(), shared::xpl::CXplMessage::kXplCommand)
      {
         //to
         boost::shared_ptr<database::entities::CKeyword> toKeyword(new database::entities::CKeyword());
         toKeyword->Name = "to";
         toKeyword->Type = "string";
         keywords.push_back(toKeyword);

         //body
         boost::shared_ptr<database::entities::CKeyword> bodyKeyword(new database::entities::CKeyword());
         bodyKeyword->Name = "from";
         bodyKeyword->Type = "string";
         keywords.push_back(bodyKeyword);

         //acknowledgment
         boost::shared_ptr<database::entities::CKeyword> acknowledgmentKeyword(new database::entities::CKeyword());
         acknowledgmentKeyword->Name = "type";
         acknowledgmentKeyword->Type = "enumeration";
         acknowledgmentKeyword->Parameters = "true|false";
         keywords.push_back(acknowledgmentKeyword);
      }

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
      if(content.find("to") == content.end())
      {
         throw shared::exception::CException("message.sms protocol needs a parameter 'to' ");
      }

      if(content.find("body") == content.end())
      {
         throw shared::exception::CException("message.sms protocol needs a parameter 'body' ");
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
      newMessage->addToBody("device", device);
      
      //set the message target
      newMessage->addToBody("to", content["to"]);

      //set the message body
      newMessage->addToBody("body", content["body"]);

      //set the acknowlegment if specified
      if(content.find("acknowledgment") != content.end())
      {
         newMessage->addToBody("acknowledgment", content["acknowledgment"]);
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