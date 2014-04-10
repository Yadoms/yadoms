#include "stdafx.h"
#include "AcBasic.h"
#include "tools/Random.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CAcBasic::CAcBasic()
   {
   }

   CAcBasic::~CAcBasic()
   {
   }

   // IRule implementation
   const DeviceIdentifier CAcBasic::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("address") + "-" +  msg.getBodyValue("unit");
   }

   MessageContent CAcBasic::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair("command", msg.getBodyValue("command")));
      if(msg.getBody().find("level") != msg.getBody().end())
         data.insert(std::make_pair("level", msg.getBodyValue("level")));
      return data;
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CAcBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;

      boost::shared_ptr<database::entities::CKeyword> mainKeyword(new database::entities::CKeyword());

      //COMMON
      mainKeyword->Name = "command";
      mainKeyword->Type = "enumeration";
      mainKeyword->Parameters = "on|off|preset";

      //LEVEL
      boost::shared_ptr<database::entities::CKeyword> levelKeyword(new database::entities::CKeyword());
      levelKeyword->Name = "level";
      levelKeyword->Type = "numeric";
      levelKeyword->Minimum = 0;
      levelKeyword->Maximum = 15;
      keywords.push_back(levelKeyword);

      keywords.push_back(mainKeyword);
      return keywords;
   }
   // [END] IRule implementation



   // ICommandRule implemntation
   boost::shared_ptr< shared::xpl::CXplMessage > CAcBasic::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
   {
      ////////////////////////////
      // do some checks
      ////////////////////////////

      //check the device address is valid
      std::string address = targetDevice.Address;
      std::vector<std::string> splittedAddress;
      boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

      if(splittedAddress.size() != 2)
      {
         throw shared::exception::CException("ac.basic protocol needs a device address matching pattern : <address>-<unit> ");
      }

      //check the command value
      communication::command::CDeviceCommand::CommandData content = deviceCommand.getCommandData();
      if(content.find("command") == content.end())
      {
         throw shared::exception::CException("ac.basic protocol needs a parameter 'command' ");
      }

      if( !boost::iequals(content["command"], "on") &&
         !boost::iequals(content["command"], "off") &
         !boost::iequals(content["command"], "preset"))
      {
         throw shared::exception::CException("ac.basic protocol needs a parameter 'command' that match 'on' or 'off' or 'preset'");
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
      newMessage->setMessageSchemaIdentifier(shared::xpl::CXplMessageSchemaIdentifier("ac", "basic"));

      //set the device addesss and unit (parse from argetDevice.Address)
      newMessage->addToBody("address", splittedAddress[0]);
      newMessage->addToBody("unit", splittedAddress[1]);

      //set the command
      newMessage->addToBody("command", content["command"]);

      //if there is any other data to send, just add key/value to bidy
      communication::command::CDeviceCommand::CommandData::const_iterator i;
      for(i=content.begin(); i!=content.end(); ++i)
      {
         if(!boost::iequals(i->first, "command"))
            newMessage->addToBody(i->first, i->second);
      }

      return newMessage;
   }

   std::string CAcBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("0x%1$08X-%2%") % tools::CRandom::generateRandomNumber<26>(false) % tools::CRandom::generateRandomNumber<4>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace rfxLanXpl
} //namespace rules
} //namespace communication