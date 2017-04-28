#include "stdafx.h"
#include "Blyss433.h"

namespace frames {
namespace outgoing {
   
      CBlyss433::CBlyss433()
      {
      }

      CBlyss433::~CBlyss433()
      {
      }
      
      const std::string CBlyss433::generateAsciiCommand(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
      {
         shared::CDataContainer details = api->getDeviceDetails(command->getDevice());
         shared::CDataContainer config = api->getDeviceConfiguration(command->getDevice());

         if (details.get<std::string>("protocol") == "blyss" && details.get<std::string>("frequency") == "433")
         {
            if(details.containsValue("id"))
               return (boost::format("ZIA++%1% BLYSS ID %2%\r\n") % command->getBody() % details.get<std::string>("id")).str();
            else
               return (boost::format("ZIA++%1% BLYSS %2%%3%\r\n") % command->getBody() % config.get<std::string>("groupCode") % config.get<int>("unitCode")).str();
         }
         return "";
      }
      
} //namespace incoming
} //namespace frames