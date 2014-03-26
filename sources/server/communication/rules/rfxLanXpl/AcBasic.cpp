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


} //namespace rfxLanXpl
} //namespace rules
} //namespace communication