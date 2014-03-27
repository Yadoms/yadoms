#include "stdafx.h"
#include "X10Basic.h"

namespace communication { namespace rules { namespace standard {

   CX10Basic::CX10Basic()
   {
   }

   CX10Basic::~CX10Basic()
   {
   }

   const DeviceIdentifier CX10Basic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return "";
   }

   MessageContent CX10Basic::ExtractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      return data;
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CX10Basic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;
      return keywords;
   }
} //namespace standard
} //namespace rules
} //namespace communication