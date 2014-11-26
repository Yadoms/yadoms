#include "stdafx.h"
#include "RfxLanXplConfiguration.h"
#include <shared/Log.h>


CRfxLanXplConfiguration::~CRfxLanXplConfiguration()
{
}



bool CRfxLanXplConfiguration::getStartXplhub() const
{
   return get<bool>("HubXplSection.checkbox");
}

std::string CRfxLanXplConfiguration::getHubLocalIp() const
{
   try
   {
      std::string chosenNetwork = get<std::string>("HubXplSection.content.networkInterfaces.activeSection");
      if (boost::iequals(chosenNetwork, "one"))
         return get<std::string>("HubXplSection.content.networkInterfaces.content.one.networkInterface");
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(error) << "Fail to get hub local ip " << ex.what();
   }

   //in case of exception, or all netowkr chosen, return 0.0.0.0
   return "0.0.0.0";

}
