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
      
   std::vector< boost::shared_ptr<database::entities::CKeyword> > CSensorBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;
      return keywords;
   }
} //namespace standard
} //namespace rules
} //namespace communication