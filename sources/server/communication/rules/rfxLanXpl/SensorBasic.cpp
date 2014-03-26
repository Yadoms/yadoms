#include "stdafx.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CSensorBasic::CSensorBasic()
   {
   }
   
   CSensorBasic::~CSensorBasic()
   {
   }
   
   const DeviceIdentifier CSensorBasic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("device");
   }

   MessageContent CSensorBasic::ExtractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(msg.getBodyValue("type"), msg.getBodyValue("current")));
      return data;
   }
   


} //namespace rfxLanXpl
} //namespace rules
} //namespace communication