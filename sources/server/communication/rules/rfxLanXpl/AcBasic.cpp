#include "stdafx.h"
#include "AcBasic.h"

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
   void CAcBasic::fillMessage(boost::shared_ptr< shared::xpl::CXplMessage > & messageToFill, database::entities::CDevice & targetDevice, communication::command::CDeviceCommand & commandData)
   {
      communication::command::CDeviceCommand::CommandData content = commandData.getCommandData();
      if(content.find("command") != content.end())
      {
         shared::xpl::CXplMessageSchemaIdentifier acbasic("ac", "basic");
         messageToFill->setMessageSchemaIdentifier(acbasic);

         std::string address = targetDevice.Address;
         std::vector<std::string> splittedAddress;
         boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

         messageToFill->addToBody("command", content["command"]);
         messageToFill->addToBody("address", splittedAddress[0]);
         messageToFill->addToBody("unit", splittedAddress[1]);


         communication::command::CDeviceCommand::CommandData::const_iterator i;
         for(i=content.begin(); i!=content.end(); ++i)
         {
            if(!boost::iequals(i->first, "command"))
               messageToFill->addToBody(i->first, i->second);
         }
         
      }
   }
   // [END] ICommandRule implemntation

} //namespace rfxLanXpl
} //namespace rules
} //namespace communication