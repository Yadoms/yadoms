#include "stdafx.h"
#include "Factory.h"
#include "Blyss433.h"

namespace frames {
namespace outgoing {
   
   boost::shared_ptr<IOutgoingFrame> CFactory::make(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
   {
         shared::CDataContainer details = api->getDeviceDetails(command->getDevice());
         if (details.get<std::string>("protocol") == "blyss" && details.get<std::string>("frequency") == "433")
         {
            return boost::make_shared<CBlyss433>();
         }
         return boost::shared_ptr<IOutgoingFrame>();
   }
   

      
} //namespace incoming
} //namespace frames