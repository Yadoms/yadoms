#include "stdafx.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace standard {

   CSensorBasic::CSensorBasic()
   {
   }
   
   CSensorBasic::~CSensorBasic()
   {
   }
   
   const DeviceIdentifier CSensorBasic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return "";
   }
   
   MessageContent CSensorBasic::ExtractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      return data;
   }

} //namespace standard
} //namespace rules
} //namespace communication