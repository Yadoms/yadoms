#include "stdafx.h"
#include "RfxLanXplConfiguration.h"


CRfxLanXplConfiguration::~CRfxLanXplConfiguration()
{
}



bool CRfxLanXplConfiguration::getStartXplhub() const
{
   return get<bool>("XplHub");
}

Poco::Net::NetworkInterface CRfxLanXplConfiguration::getXplNetworkInterface() const
{
   try
   {
      std::string interfaceName = get<std::string>("networkInterface");
      return Poco::Net::NetworkInterface::forName(interfaceName);
   }
   catch (std::exception & ex)
   {
      std::cerr << "Fail to get hub local ip " << ex.what() << std::endl;
   }

   //in case of exception, or bad network interface
   //initialize result with the first non loopback iface
   Poco::Net::NetworkInterface::NetworkInterfaceList netlist = Poco::Net::NetworkInterface::list();
   for (Poco::Net::NetworkInterface::NetworkInterfaceList::iterator i = netlist.begin(); i != netlist.end(); ++i)
   {
      if (!i->address().isLoopback())
      {
         return *i;
      }
   }
   
   return Poco::Net::NetworkInterface();
}
