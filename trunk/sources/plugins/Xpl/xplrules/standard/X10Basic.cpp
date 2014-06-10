#include "stdafx.h"
#include "X10Basic.h"
#include <shared/tools/Random.h>

namespace xplrules { namespace standard {

   xplcore::CXplMessageSchemaIdentifier  CX10Basic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("x10.basic");

   CX10Basic::CX10Basic()
   {
   }

   CX10Basic::~CX10Basic()
   {
   }

   const xplcore::CXplMessageSchemaIdentifier CX10Basic::getProtocol()
   {
      return m_protocol;
   }

   const CDeviceIdentifier CX10Basic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      //TODO : manage list of device or list of house !
      std::string commercialName = msg.getBodyValue("device");
      if (msg.getBodyValue("type") == "cn")
         commercialName = "Chacon, Avidsen, NEXA smoke detector";
      if (msg.getBodyValue("type") == "mct" || msg.getBodyValue("type") == "mcw")
         commercialName = "Visonic PowerCode sensors";

      return CDeviceIdentifier(msg.getBodyValue("device"), commercialName, m_protocol, m_protocol);
   }

   MessageContent CX10Basic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair("command", msg.getBodyValue("command")));
      if(msg.getBody().find("level") != msg.getBody().end())
         data.insert(std::make_pair("level", msg.getBodyValue("level")));
      if(msg.getBody().find("data1") != msg.getBody().end())
         data.insert(std::make_pair("data1", msg.getBodyValue("data1")));
      if(msg.getBody().find("data2") != msg.getBody().end())
         data.insert(std::make_pair("data2", msg.getBodyValue("data2")));
      return data;
   }

   std::vector< boost::shared_ptr<CDeviceKeyword> > CX10Basic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;
      /*
      //command
      boost::shared_ptr<database::entities::CKeyword> commandKeyword(new database::entities::CKeyword());
      commandKeyword->Name = "command";
      commandKeyword->Type = "enumeration";
      commandKeyword->Parameters = "select|all_units_off|all_lights_on|all_lights_off|on|off|dim|bright|extended|hail_req|hail_ack|predim1|predim2|status|status_on|status_off";
      keywords.push_back(commandKeyword);


      if(boost::iequals(msg.getBodyValue("command"), "dim")||          
         boost::iequals(msg.getBodyValue("command"), "bright"))
      {
         //LEVEL
         boost::shared_ptr<database::entities::CKeyword> levelKeyword(new database::entities::CKeyword());
         levelKeyword->Name = "level";
         levelKeyword->Type = "numeric";
         levelKeyword->Minimum = 0;
         levelKeyword->Maximum = 100;
         keywords.push_back(levelKeyword);
      }

      if(boost::iequals(msg.getBodyValue("command"), "extended"))
      {
         //data1
         boost::shared_ptr<database::entities::CKeyword> data1Keyword(new database::entities::CKeyword());
         data1Keyword->Name = "data1";
         data1Keyword->Type = "numeric";
         data1Keyword->Minimum = 0;
         data1Keyword->Maximum = 255;
         keywords.push_back(data1Keyword);

         //data1
         boost::shared_ptr<database::entities::CKeyword> data2Keyword(new database::entities::CKeyword());
         data2Keyword->Name = "data2";
         data2Keyword->Type = "numeric";
         data2Keyword->Minimum = 0;
         data2Keyword->Maximum = 255;
         keywords.push_back(data2Keyword);

      }
      */
      return keywords;
   }

   
   /*

   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CX10Basic::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
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
      boost::shared_ptr< xplcore::CXplMessage > newMessage(new xplcore::CXplMessage());

      //the AC.BSACI XplMessage if a xpl-trig
      newMessage->setTypeIdentifier(xplcore::CXplMessage::kXplCommand);

      //set hop to 1
      newMessage->setHop(1);

      //set the target (rfxcom-lan-<hax mac address>)
      newMessage->setTarget(xplcore::CXplActor::parse(targetDevice.HardwareIdentifier()));

      //set the ac.basic
      newMessage->setMessageSchemaIdentifier(xplcore::CXplMessageSchemaIdentifier("x10", "basic"));

      //set the command
      newMessage->addToBody("command", content["command"]);

      //set the device addesss and unit (parse from argetDevice.Address)
      if(content.find("device") != content.end())
         newMessage->addToBody("device", content["device"]);
      else
         newMessage->addToBody("house", content["house"]);

      //set the acknowlegment if specified
      if(content.find("level") != content.end())
      {
         newMessage->addToBody("level", content["level"]);
      }

      //set the data1 if specified
      if(content.find("data1") != content.end())
      {
         newMessage->addToBody("data1", content["data1"]);
      }

      //set the data2 if specified
      if(content.find("data2") != content.end())
      {
         newMessage->addToBody("data2", content["data2"]);
      }

      return newMessage;
   }
   */
   std::string CX10Basic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }

} //namespace standard
} //namespace xplrules
