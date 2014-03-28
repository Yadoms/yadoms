#include "stdafx.h"
#include "AcBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CAcBasic::CAcBasic()
   {
   }

   CAcBasic::~CAcBasic()
   {
   }

   const DeviceIdentifier CAcBasic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("address") + "-" +  msg.getBodyValue("unit");
   }

   MessageContent CAcBasic::ExtractMessageData(shared::xpl::CXplMessage & msg)
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

} //namespace rfxLanXpl
} //namespace rules
} //namespace communication