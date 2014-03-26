#include "stdafx.h"
#include "AcBasic.h"

namespace communication { namespace rules { namespace standard {

   CAcBasic::CAcBasic()
   {
   }

   CAcBasic::~CAcBasic()
   {
   }

   const DeviceIdentifier CAcBasic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return "";
   }

   MessageContent CAcBasic::ExtractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      return data;
   }

} //namespace standard
} //namespace rules
} //namespace communication