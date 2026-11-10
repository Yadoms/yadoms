#include "stdafx.h"
#include "RfxLanXplConfiguration.h"
#include <shared/Log.h>


void CRfxLanXplConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& configuration)
{
   m_contener.initializeWith(configuration);
}

bool CRfxLanXplConfiguration::getStartXplhub() const
{
   return m_contener.get<bool>("XplHub");
}

Poco::Net::NetworkInterface CRfxLanXplConfiguration::getXplNetworkInterface() const
{
   try
   {
      const auto interfaceName = m_contener.get<std::string>("networkInterface");
      return Poco::Net::NetworkInterface::forName(interfaceName);
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(error) << "Fail to get hub local ip " << ex.what() ;
   }

   //in case of exception, or bad network interface
   //initialize result with the first non loopback iface
   Poco::Net::NetworkInterface::NetworkInterfaceList netlist = Poco::Net::NetworkInterface::list();
   for (const auto& i : netlist)
   {
      if (!i.address().isLoopback())
      {
         return i;
      }
   }
   
   return Poco::Net::NetworkInterface();
}
