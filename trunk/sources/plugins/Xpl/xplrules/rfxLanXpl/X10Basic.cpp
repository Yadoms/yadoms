#include "stdafx.h"
#include "X10Basic.h"
#include <shared/tools/Random.h>

namespace xplrules { namespace rfxLanXpl {

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
      std::string commercialName = msg.getBodyValue("device");

      if (msg.getBody().find("protocol") != msg.getBody().end())
      {
         if (msg.getBodyValue("protocol") == "arc")
            commercialName = "KlikAanKlikUit, Chacon, HomeEasy, DomiaLite, Domia, ByeByeStandBy, ELRO AB600, NEXA Proove Intertechno Duwi";

         if (msg.getBodyValue("protocol") == "flamingo")
            commercialName = "Flamingo";

         if (msg.getBodyValue("protocol") == "koppla")
            commercialName = "Ikea Koppla";  
         
         if (msg.getBodyValue("protocol") == "he105")
            commercialName = "HomeEasy";

         if (msg.getBodyValue("protocol") == "rts10")
            commercialName = "Digimax";

         if (msg.getBodyValue("protocol") == "harrison")
            commercialName = "Harrison";
      }

      return CDeviceIdentifier(msg.getBodyValue("device"), commercialName, m_protocol, m_protocol);
   }

   MessageContent CX10Basic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair("command", msg.getBodyValue("command")));
      if(msg.getBody().find("level") != msg.getBody().end())
         data.insert(std::make_pair("level", msg.getBodyValue("level")));
      if(msg.getBody().find("protocol") != msg.getBody().end())
         data.insert(std::make_pair("protocol", msg.getBodyValue("protocol")));
      return data;
   }

   
   std::vector< boost::shared_ptr<CDeviceKeyword> > CX10Basic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;

      /*keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("command", "command", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, "{ values : [on,off,dim,bright,all_lights_on,all_lights_off] }")));
      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("protocol", "protocol", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, "{ values : [arc,flamingo,koppla,waveman,harrison,he105,rts10] }")));
      
      boost::property_tree::ptree details;
      details.put("min", 0);
      details.put("max", 100);
      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("level", "level", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, details)));
      */
      return keywords;
   }


   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CX10Basic::createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress)
   {
      ////////////////////////////
      // create the message
      ////////////////////////////

      //create the message
      boost::shared_ptr< xplcore::CXplMessage > newMessage(new xplcore::CXplMessage());

      //TODO : implement it

      return newMessage;
   }
   // [END] ICommandRule implemntation



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
   */
   std::string CX10Basic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation
} //namespace rfxLanXpl
} //namespace xplrules
