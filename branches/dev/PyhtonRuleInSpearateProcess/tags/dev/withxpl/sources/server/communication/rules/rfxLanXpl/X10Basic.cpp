#include "stdafx.h"
#include "X10Basic.h"
#include "tools/Random.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CX10Basic::CX10Basic()
   {
   }

   CX10Basic::~CX10Basic()
   {
   }

   const CDeviceIdentifier CX10Basic::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return CDeviceIdentifier(msg.getBodyValue("device"));
   }

   MessageContent CX10Basic::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair("command", msg.getBodyValue("command")));
      if(msg.getBody().find("level") != msg.getBody().end())
         data.insert(std::make_pair("level", msg.getBodyValue("level")));
      if(msg.getBody().find("protocol") != msg.getBody().end())
         data.insert(std::make_pair("protocol", msg.getBodyValue("protocol")));
      return data;
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CX10Basic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;


      if(msg.getTypeIdentifier(), shared::xpl::CXplMessage::kXplStat)
      {
         //command
         boost::shared_ptr<database::entities::CKeyword> commandKeyword(new database::entities::CKeyword());
         commandKeyword->Name = "command";
         commandKeyword->Type = "enumeration";
         commandKeyword->Parameters = "on|off|dim|bright|all_lights_on|all_lights_off";
         keywords.push_back(commandKeyword);
      }


      if(msg.getTypeIdentifier(), shared::xpl::CXplMessage::kXplCommand)
      {

         //protocol
         boost::shared_ptr<database::entities::CKeyword> protocolKeyword(new database::entities::CKeyword());
         protocolKeyword->Name = "protocol";
         protocolKeyword->Type = "enumeration";
         protocolKeyword->Parameters = "arc|flamingo|koppla|waveman|harrison|he105|rts10";
         keywords.push_back(protocolKeyword);


         if(boost::iequals(msg.getBodyValue("protocol"), "koppla"))
         {
            //LEVEL
            boost::shared_ptr<database::entities::CKeyword> levelKeyword(new database::entities::CKeyword());
            levelKeyword->Name = "level";
            levelKeyword->Type = "numeric";
            levelKeyword->Minimum = 0;
            levelKeyword->Maximum = 100;
            keywords.push_back(levelKeyword);
         }

         //command
         boost::shared_ptr<database::entities::CKeyword> commandKeyword(new database::entities::CKeyword());
         commandKeyword->Name = "command";
         commandKeyword->Type = "enumeration";
         if(msg.getBody().find("protocol") == msg.getBody().end() ||          
            boost::iequals(msg.getBodyValue("protocol"), "koppla")||          
            boost::iequals(msg.getBodyValue("protocol"), "harrison"))
         {
            //if protocol is not defined (x10 by default) or koppla
            commandKeyword->Parameters = "on|off|dim|bright|all_lights_on|all_lights_off";
         }
         else if(boost::iequals(msg.getBodyValue("protocol"), "arc") || 
                 boost::iequals(msg.getBodyValue("protocol"), "waveman")|| 
                 boost::iequals(msg.getBodyValue("protocol"), "rts10"))
         {
            commandKeyword->Parameters = "on|off|all_lights_on|all_lights_off";
         }
         else if(boost::iequals(msg.getBodyValue("protocol"), "flamingo") ||
                 boost::iequals(msg.getBodyValue("protocol"), "he105"))
         {
            commandKeyword->Parameters = "on|off";
         }
         keywords.push_back(commandKeyword);
      }


      return keywords;
   }


   
   // ICommandRule implemntation
   boost::shared_ptr< shared::xpl::CXplMessage > CX10Basic::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
   {
      ////////////////////////////
      // do some checks
      ////////////////////////////

      //check the device is valid
      std::string device = targetDevice.Address;

      //check the command value
      communication::command::CDeviceCommand::CommandData content = deviceCommand.getCommandData();
      if(content.find("command") == content.end())
      {
         throw shared::exception::CException("message.sms protocol needs a parameter 'command' ");
      }

      if(content.find("device") == content.end() && content.find("house") == content.end())
      {
         throw shared::exception::CException("message.sms protocol needs a parameter 'device' or 'house' ");
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
      newMessage->setMessageSchemaIdentifier(shared::xpl::CXplMessageSchemaIdentifier("x10", "basic"));

      //set the device addesss and unit (parse from argetDevice.Address)
      newMessage->addToBody("device", content["device"]);
      
      //set the command
      newMessage->addToBody("command", content["command"]);

      //set the level if specified
      if(content.find("level") != content.end())
      {
         newMessage->addToBody("level", content["level"]);
      }

      //set the protocol if specified
      if(content.find("protocol") != content.end())
      {
         newMessage->addToBody("protocol", content["protocol"]);
      }
      return newMessage;
   }

   std::string CX10Basic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication