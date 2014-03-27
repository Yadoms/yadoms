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

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CAcBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;
      return keywords;
   }

} //namespace standard
} //namespace rules
} //namespace communication